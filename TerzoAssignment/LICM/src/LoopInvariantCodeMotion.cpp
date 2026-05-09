#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"

#include <vector>
#include <unordered_set>

using namespace llvm;

namespace {

// New PM implementation
// Invocare il passo nella cartella /test nel seguente modo: 
// opt -S -load-pass-plugin ../build/libLoopInvariantCodeMotion.so -p loop-invariant-code-motion Test.m2r.ll -o Test.optimized.ll
// Pass per identificare istruzioni loop-invariant (senza code motion)

/*NOTE DI PROGETTO
• Calcolare le reaching definitions
• Trovare le istruzioni loop-invariant                                           | attraverso isLoopInvariant
• Calcolare i dominatori (dominance tree)                                        |
• Trovare le uscite del loop (i successori fuori dal loop)                       |
• Le istruzioni candidate alla code motion:
    • Sono loop invariant                                                       | attraverso isLoopInvariant
    • Si trovano in blocchi che dominano tutte le uscite del loop               | Tramite domtree
    • Assegnano un valore a variabili non assegnate altrove nel loop            | La forma SSA lo garantisce per costruzione
    • Si trovano in blocchi che dominano tutti i blocchi nel loop che usano la  | La forma SSA lo garantisce per costruzione
            variabile a cui si sta assegnando un valore
• Eseguire una ricerca depth-first dei blocchi
• Spostare l’istruzione candidata nel preheader se tutte le istruzioni
invarianti da cui questa dipende sono state spostate
*/
struct LoopInvariantCodeMotion: PassInfoMixin<LoopInvariantCodeMotion>{

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {

    // Otteniamo l'analisi dei loop per la funzione
    LoopInfo &LI = AM.getResult<LoopAnalysis>(F);

    // Otteniamo il dominator tree
    DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);
    

    // Iteriamo su tutti i loop top-level
    for (Loop *L : LI) {
        processLoop(L, DT);
    }


    return PreservedAnalyses::all();
  }

  private:

    // Analizza un loop e identifica le istruzioni invariant
    void processLoop(Loop *L, DominatorTree &DT) {
        std::unordered_set<Instruction*> CMCandidates;

        // Vettore delle istruzioni invariant trovate
        std::vector<Instruction*> InvariantInsts;

        // Set per lookup veloce (serve per iterazione a punto fisso)
        std::unordered_set<Instruction*> InvariantSet;

        // Scorriamo tutti i basic block del loop
        for (BasicBlock *BB : L->blocks()) {

            // Scorriamo tutte le istruzioni del blocco
            for (Instruction &I : *BB) {

                // Controlla se è loop invariant
                if (isLoopInvariant(I, L, InvariantSet)) {

                    // Aggiungiamo al vettore
                    InvariantInsts.push_back(&I);

                    // Segniamo nel set
                    InvariantSet.insert(&I);

                    //Segniamo nelle candidate alla CM
                    CMCandidates.insert(&I);
                }
            }
        }
    

        // DEBUG: stampa le istruzioni trovate
        for (Instruction *I : InvariantInsts) {
            errs() << "Loop Invariant: ";
            I->print(errs());
            errs() << "\n";
        }

        // Ricorsione sui sotto-loop 
        for (Loop *SubLoop : L->getSubLoops()) {
            processLoop(SubLoop, DT);
        }


        //Cerco i blocchi che dominano tutte le uscite del loop---
        std::vector<BasicBlock*> commonDominators;
        //    1. vado a prendere ogni exit block del loop
        SmallVector<BasicBlock*> exitBlocks;
        L->getExitBlocks(exitBlocks);
        //    2. esamino ogni nodo del Dominator Tree
        // TODO: far partire il codice dal loop
        for (auto *DTN : depth_first(DT.getRootNode())) {
            
            //controllo se ogni exitBlock è tra i figli del nodo
            bool hasAllEB = true;
            for(auto eB : exitBlocks) {
                bool isEbInChild = false;
                //controllo se tra i figli del nodo corrente c'è l'exitBlock
                for(auto c : DTN->children()) {
                    if(c->getBlock() == eB) { isEbInChild=true; }
                }
                if(!isEbInChild) { hasAllEB = false; }
            }
            if(hasAllEB) {
                commonDominators.push_back(DTN->getBlock());
            }
    
        }
        //A fine iterazione commonDominators contiene ogni blocco che domina ogni exitBlock, quindi ogni blocco candidato a CodeMotion per punto 2
        //Riempio il set con le candidate alla Code Motion
        for(BasicBlock* b : commonDominators) {
            for(Instruction &I : *b) {
                // TODO: controllare se non complica il loop
                CMCandidates.insert(&I);
            }
        }

    }

    // Funzione che verifica se una istruzione è loop-invariant
    bool isLoopInvariant(Instruction &I,
                         Loop *L,
                         std::unordered_set<Instruction*> &InvariantSet) {

        // Escludiamo istruzioni con effetti collaterali
        if (I.mayHaveSideEffects())
            return false;

        // Escludiamo terminatori (branch, return, ecc.)
        if (I.isTerminator())
            return false;


        // Analizziamo tutti gli operandi dell'istruzione
        // prendiamo gli usi (operands)
        for (Value *Op : I.operands()) {

            // Caso 1: costante → sempre invariant
            if (isa<Constant>(Op))
                continue;

            // Caso 3: valore definito da un'altra istruzione
            // risaliamo alla definizione
            if (Instruction *OpInst = dyn_cast<Instruction>(Op)) {

                // Se definito FUORI dal loop → ok
                if (!L->contains(OpInst))
                    continue;

                // Se definito nel loop ma già invariant → ok
                if (InvariantSet.count(OpInst))
                    continue;

                // Controlla se l'istruzione che definisce l'operando è loopInvariant
                if (isLoopInvariant(*OpInst, L, InvariantSet)) {
                    continue;
                }

                // Altrimenti NON invariant
                return false;
            }

            // Qualsiasi altro caso → conservativo → non invariant
            return false;
        }

        // Tutti gli operandi soddisfano le condizioni → invariant
        return true;
    }


    static bool isRequired() { return true; }
};  
}//namespace

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getLoopInvariantCodeMotionPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "LoopInvariantCodeMotion", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "loop-invariant-code-motion") {
                    FPM.addPass(LoopInvariantCodeMotion());
                    return true;
                  }
                  return false;
                });
          }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize LoopInvariantCodeMotion when added to the pass pipeline on the
// command line, i.e. via '-passes=loop-invariant-code-motion'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getLoopInvariantCodeMotionPluginInfo();
}