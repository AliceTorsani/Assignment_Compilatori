#include "llvm/IR/PassManager.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include <vector>
#include <algorithm> // Necessario per std::find

using namespace llvm;
namespace {

struct MyLoopInvariantCodeMotionPass : public PassInfoMixin<MyLoopInvariantCodeMotionPass> {
    
    PreservedAnalyses run(Loop &L, LoopAnalysisManager &LAM, LoopStandardAnalysisResults &AR, LPMUpdater &U) {
        
        // Unico vettore per memorizzare le istruzioni invarianti mantenendo l'ordine di inserimento
        std::vector<Instruction*> LoopInvariantInsts;

        // FASE A: Identificazione delle istruzioni Loop Invariant
        // Iteriamo sui blocchi del loop
        for (BasicBlock *BB : L.blocks()) {
            // Iteriamo sulle singole istruzioni
            for (Instruction &I : *BB) {
                
                // 1. Filtri di sicurezza: saltiamo istruzioni problematiche per la code motion
                if (I.isTerminator() || 
                    isa<LoadInst>(&I) || //load
                    isa<StoreInst>(&I) || //store
                    isa<PHINode>(&I) || //phi node
                    I.mayHaveSideEffects()) //istruzioni con side effect 
                    {
                    continue;
                }

                // 2. Controllo degli operandi dell'istruzione
                bool AllOperandsInvariant = true;
                
                for (Use &Op : I.operands()) {
                    Value *V = Op.get();

                    // Se è una costante, è invariante
                    if (isa<Constant>(V)) {
                        continue;
                    }
                    
                    // Se è un argomento della funzione (definito prima del loop), è invariante
                    if (isa<Argument>(V)) {
                        continue;
                    }

                    // Se l'operando è il risultato di un'altra istruzione...
                    if (Instruction *OpInst = dyn_cast<Instruction>(V)) {
                        
                        // ...ed è definita in un blocco fuori dal loop, è invariante
                        if (!L.contains(OpInst)) {
                            continue;
                        }
                        
                        // ...ed è definita dentro il loop, controlliamo se l'abbiamo già marcata come invariante.
                        // Usiamo std::find per cercare linearmente all'interno del nostro vettore.
                        if (std::find(LoopInvariantInsts.begin(), LoopInvariantInsts.end(), OpInst) != LoopInvariantInsts.end()) {
                            continue;
                        }
                    }

                    // Se l'operando non rispetta nessuna delle condizioni sopra, l'istruzione non è invariante
                    AllOperandsInvariant = false;
                    break;
                }

                // Se abbiamo verificato che tutti gli operandi sono invarianti, salviamo l'istruzione
                if (AllOperandsInvariant) {
                    LoopInvariantInsts.push_back(&I);
                }
            }
            
        }
        return PreservedAnalyses::all();
    }
};

} // end anonymous namespace

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getLoopInvariantCodeModePluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "LoopInvariantCodeModePass", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "loop-invariant-code-mode") {
                    FPM.addPass(LoopInvariantCodeModePass());
                    return true;
                  }
                  return false;
                });
          }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize LoopInvariantCodeModePass when added to the pass pipeline on the
// command line, i.e. via '-passes=loop-invariant-code-mode'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getLoopInvariantCodeModePluginInfo();
}