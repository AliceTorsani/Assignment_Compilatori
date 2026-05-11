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
// Pass per identificare istruzioni loop-invariant e blocchi di uscita
struct LoopInvariantCodeMotion: PassInfoMixin<LoopInvariantCodeMotion>{

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {

    // Otteniamo l'analisi dei loop per la funzione
    LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
    
    // NUOVO: Calcoliamo / Otteniamo il Dominator Tree per la funzione
    DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);

    // Iteriamo su tutti i loop top-level
    for (Loop *L : LI) {
      // Passiamo anche il Dominator Tree alla funzione che processa il loop
      processLoop(L, DT);
    }

    return PreservedAnalyses::all();
  }

  private:

    // Analizza un loop, identifica le istruzioni invariant e trova le uscite
    void processLoop(Loop *L, DominatorTree &DT) {

        // --- 1. RICERCA DELLE ISTRUZIONI LOOP-INVARIANT ---
        // Vettore delle istruzioni invariant trovate
        std::vector<Instruction*> InvariantInsts;

        // Scorriamo tutti i basic block del loop
        for (BasicBlock *BB : L->blocks()) {

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
        
        // DEBUG: stampa le istruzioni trovate
        for (Instruction *I : InvariantInsts) {
            errs() << "Loop Invariant: ";
            I->print(errs());
            errs() << "\n";
        }


        // --- 2. RICERCA DELLE USCITE DEL LOOP ---
        // Utilizziamo getExitBlocks per trovare tutti i successori che si trovano FUORI dal loop.
        SmallVector<BasicBlock *, 8> ExitBlocks;
        L->getExitBlocks(ExitBlocks);

        errs() << "--- Loop Exits (Successori fuori dal loop) ---\n";
        for (BasicBlock *ExitBB : ExitBlocks) {
            errs() << "Exit Block: ";
            // Stampa il nome del blocco in modo leggibile (es: %4)
            ExitBB->printAsOperand(errs(), false); 
            errs() << "\n";
        }
        // --- 4. VALUTAZIONE CANDIDATE E CODE MOTION (Dalla riga rossa in poi) ---
        
        // Otteniamo il preheader del loop, che sarà la destinazione delle nostre istruzioni
        BasicBlock *Preheader = L->getLoopPreheader();

        if (Preheader) {
            bool Changed = true;
            // Eseguiamo cicli finché riusciamo a spostare almeno un'istruzione.
            // Questo implementa in modo iterativo la regola: "Spostare l'istruzione... 
            // se tutte le istruzioni invarianti da cui questa dipende sono state spostate"
            while (Changed) {
                Changed = false;

                for (auto It = InvariantInsts.begin(); It != InvariantInsts.end(); ) {
                    Instruction *I = *It;
                    BasicBlock *InstBB = I->getParent();

                    // RIGA ROSSA: Verificare che il blocco domini TUTTE le uscite del loop
                    bool DominatesAllExits = true;
                    for (BasicBlock *ExitBB : ExitBlocks) {
                        if (!DT.dominates(InstBB, ExitBB)) {
                            DominatesAllExits = false;
                            break;
                        }
                    }

                    // Verifica delle dipendenze: un'istruzione candidata   può essere spostata
                    // SOLO se tutte le istruzioni da cui dipende (i suoi operandi) si trovano 
                    // già al di fuori del loop (ad esempio, nel preheader).
                    bool DependenciesResolved = true;
                    for (Value *Op : I->operands()) {
                        if (Instruction *OpInst = dyn_cast<Instruction>(Op)) {
                            // Se l'istruzione che definisce l'operando è ancora nel loop,
                            // non possiamo spostare l'istruzione corrente.
                            if (L->contains(OpInst)) {
                                DependenciesResolved = false;
                                break;
                            }
                        }
                    }

                    // Se domina tutte le uscite e le dipendenze sono risolte, procediamo alla Code Motion
                    if (DominatesAllExits && DependenciesResolved) {
                        errs() << ">>> Code Motion: Sposto nel preheader l'istruzione: ";
                        I->print(errs());
                        errs() << "\n";

                        // Spostiamo l'istruzione alla fine del preheader (esattamente prima del branch/terminatore)
                        I->moveBefore(Preheader->getTerminator());

                        // Rimuoviamo l'istruzione dal vettore delle invarianti per non riprocessarla
                        // e segnaliamo che c'è stato un cambiamento per fare una nuova iterazione
                        It = InvariantInsts.erase(It);
                        Changed = true;
                    } else {
                        // Passiamo all'istruzione successiva
                        ++It; 
                    }
                }
            }
        } else {
            errs() << "Attenzione: Nessun preheader disponibile. Code motion annullata per questo loop.\n";
        }

        // --- 3. RICORSIONE SUI SOTTO-LOOP ---
        for (Loop *SubLoop : L->getSubLoops()) {
            processLoop(SubLoop, DT);
        }
        
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

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getLoopInvariantCodeMotionPluginInfo();
}
