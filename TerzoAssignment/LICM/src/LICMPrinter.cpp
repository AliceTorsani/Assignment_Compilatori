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
struct LICMPrinter: PassInfoMixin<LICMPrinter>{

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {

    // Otteniamo l'analisi dei loop per la funzione
        LoopInfo &LI = AM.getResult<LoopAnalysis>(F);

    // Otteniamo il dominator tree
    DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);
    

    // Iteriamo su tutti i loop top-level
    for (Loop *L : LI) {
        std::vector<Instruction*> invInst = processLoop(L, DT);
        
        
        //TODO: Spostare le istruzioni candidata nel preheader
        
        // outs() << "\n\n--------ANALISI DEL PREHEADER E SPOSTAMENTO DELLE ISTRUZIONI--------\n\n";
        // //controllo se esiste un pre-header
        // BasicBlock *preHeader;
        // if (L->getLoopPreheader() == nullptr) {
        //     outs() << "\n\nnon vi è preheader per iniziare\n\n";
            
        //     preHeader->Create(F.getContext(), "preheader", &F, L->getHeader());
        // } else  {
        //     preHeader = L->getLoopPreheader();
        // }
        // if(invInst.size() != 0) {
        //     invInst[0]->insertInto(preHeader, preHeader->begin());
        //     for(int i=1; i < invInst.size(); i++) {
        //         invInst[i]->insertAfter(invInst[i-1]);
        //     }
        // }
    }


    return PreservedAnalyses::all();
  }

  private:

    bool isInvariantDependent(Instruction &I, std::vector<Instruction*> invariants, Loop *L) {
        if (!(isa<BinaryOperator>(&I) || isa<UnaryInstruction>(&I) || I.isCast()))
            return false;
        
        // Analizziamo tutti gli operandi dell'istruzione
        // prendiamo gli usi (operands)
        for (Value *Op : I.operands()) {

            // Caso 1: costante → sempre invariant
            if (isa<Constant>(Op))
                continue;

            // Caso 2: valore definito da un'altra istruzione
            // risaliamo alla definizione
            if (Instruction *OpInst = dyn_cast<Instruction>(Op)) {

                // Se definito FUORI dal loop → ok
                if (!L->contains(OpInst))
                    continue;

                // Se definito nel loop ma già invariant → ok
                // (Cioè se il vettore contiene già l'istruzione -> ok)
                if (std::find(invariants.begin(),
                                invariants.end(),
                                OpInst) != invariants.end())
                {
                    continue;
                }

                // Altrimenti NON invariant
                return false;
            }

        }
        // Qualsiasi altro caso → conservativo → non invariant
        return true;
    }

    // Analizza un loop e identifica le istruzioni invariant
    std::vector<Instruction*> processLoop(Loop *L, DominatorTree &DT) {
        

        // Vettore delle istruzioni invariant trovate
        std::vector<Instruction*> InvariantInsts;

            // Scorriamo tutti i basic block del loop
            for (BasicBlock *BB : L->blocks()) {

                outs() << "\nblocco del loop: ";
                BB->printAsOperand(outs(), false);
                outs() << "\nil quale domina: ";
                
                SmallVector<BasicBlock*> discendenti;
                DT.getDescendants(BB, discendenti);

                for(auto c : discendenti) {
                    outs() << "\n\t ⊢";
                    c->printAsOperand(outs(), false);
                }


                // Scorriamo tutte le istruzioni del blocco
                for (Instruction &I : *BB) {

                    // Controlla se è loop invariant
                    if (isLoopInvariant(I, L, InvariantInsts)) {

                        // Se il vettore non contiene già l'istruzione, allora la inseriamo
                        // (evito di inserire duplicati)
                        if (std::find(InvariantInsts.begin(),
                                        InvariantInsts.end(),
                                        &I) == InvariantInsts.end())
                        {
                            // Aggiungiamo al vettore
                            InvariantInsts.push_back(&I);
                        }
                        
                    }
                }
            }
            outs() << "\n\n--------fine loop--------\n\n";
        

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


        //VERIFICO CHE LE ISTRUZIONI LOOP-INVARIANT SI TROVINO IN BLOCCHI CHE DOMINANO TUTTE LE USCITE DEL LOOP
        std::vector<Instruction *> notInWholeDom;
        
        //    1. vado a prendere ogni exit block del loop
        SmallVector<BasicBlock*> exitBlocks;
        L->getExitBlocks(exitBlocks);
        
        //    2. esamino ogni istruzione LoopInvariant
        for (Instruction *LoopInvInst : InvariantInsts) {
            
            //prendo il nodo corrispondente al BB dell'istruzione corrente
            SmallVector<BasicBlock*> discendenti;
            DT.getDescendants(LoopInvInst->getParent(), discendenti);
            
            //controllo se ogni exitBlock è tra i discendenti del nodo
            bool hasAllEB = true;
            for(auto eB : exitBlocks) {
                outs() << "exit block esaminato: ";
                eB->printAsOperand(errs(), false);
                
                bool isEbInChild = false;
                
                //controllo se tra i discendenti del nodo corrente c'è l'exitBlock
                for(auto dB : discendenti) {
                    outs() << " con basic block: ";
                    dB->printAsOperand(errs(), false);
                    outs() << "\n\t\t\t ";


                    if(dB == eB) { isEbInChild=true; }
                }
                if(!isEbInChild) { hasAllEB = false; } //TODO: break?
            }

            if(!hasAllEB) {
                
                outs() << "\nIstruzione il cui blocco NON domina tutte le uscite: ";
                LoopInvInst->print(errs());
                outs() << "\n";

                //inserisco nel vettore delle istruzioni da rimuovere
                notInWholeDom.push_back(LoopInvInst);
                
            } else {
                outs() << "\nIstruzione il cui blocco DOMINA tutte le uscite: ";
                LoopInvInst->print(errs());
                outs() << "\n";
            }
            
        }
        
        
        //rimuovo le istruzioni di notInWholeDom da InvariantInsts
        for (int i=0; i < InvariantInsts.size(); i++) {
            for (Instruction *j : notInWholeDom) {
                if(InvariantInsts.at(i) == j) {
                    InvariantInsts.erase(InvariantInsts.begin()+i);
                    outs() << "eliminata l'istruzione: ";
                    j->print(errs());
                    outs() << "\n";
                }
            }
        }

        // Controlla se tutte le istruzioni invarianti da cui questa dipende sono state spostate
        for (int i=0; i < InvariantInsts.size(); i++) {
            if (false) {
            //if(!isInvariantDependent(*InvariantInsts[i], InvariantInsts, L)) {
                InvariantInsts.erase(InvariantInsts.begin()+i);
                outs() << "eliminata l'istruzione: ";
                InvariantInsts[i]->print(errs());
                outs() << "\n";
            }
        }



        return InvariantInsts;

    }

    // Funzione che verifica se una istruzione è loop-invariant
    bool isLoopInvariant(Instruction &I,
                         Loop *L, std::vector<Instruction*> &InvariantInsts) {
        
        // Escludiamo istruzioni con effetti collaterali (store, chiamate a funzione, ecc.)
        if (I.mayHaveSideEffects())
            return false;

        // Escludiamo terminatori (branch, return, ecc.)
        if (I.isTerminator())
            return false;

        // Escludiamo istruzioni PHI
        if (isa<PHINode>(&I))
            return false;
        
        // Escludiamo le load
        if (isa<LoadInst>(&I))
            return false;

        // Controlla che le istruzioni del tipo a=5 vengano analizzate
        // Se l'istruzione non è binaria, allora non la analizziamo
        // e se non è un'istruzione del tipo a=5 allora non la analizziamo
        // Considera SOLO:
        // - binary operations
        // - unary instructions

        if (!(isa<BinaryOperator>(&I) || isa<UnaryInstruction>(&I) || I.isCast()))
            return false;
        
        // Analizziamo tutti gli operandi dell'istruzione
        // prendiamo gli usi (operands)
        for (Value *Op : I.operands()) {

            // Caso 1: costante → sempre invariant
            if (isa<Constant>(Op))
                continue;

            // Caso 2: valore definito da un'altra istruzione
            // risaliamo alla definizione
            if (Instruction *OpInst = dyn_cast<Instruction>(Op)) {

                // Se definito FUORI dal loop → ok
                if (!L->contains(OpInst))
                    continue;

                // Se definito nel loop ma già invariant → ok
                // (Cioè se il vettore contiene già l'istruzione -> ok)
                if (std::find(InvariantInsts.begin(),
                                InvariantInsts.end(),
                                OpInst) != InvariantInsts.end())
                {
                    continue;
                }

                // Se definita nel loop:
                // verifico ricorsivamente
                // controllo ricorsivo per controllare
                // se l'istruzione che definsce l'operando è anch'essa loop invariant
                if (isLoopInvariant(*OpInst, L, InvariantInsts))
                    continue;

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
llvm::PassPluginLibraryInfo getLICMPrinterPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "LICMPrinter", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "LICMPrinter") {
                    FPM.addPass(LICMPrinter());
                    return true;
                  }
                  return false;
                });
          }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize LICMPrinter when added to the pass pipeline on the
// command line, i.e. via '-passes=loop-invariant-code-motion'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getLICMPrinterPluginInfo();
}
