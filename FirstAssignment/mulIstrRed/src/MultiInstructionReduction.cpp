#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

//-----------------------------------------------------------------------------
// TestPass implementation
//-----------------------------------------------------------------------------
// No need to expose the internals of the pass to the outside world - keep
// everything in an anonymous namespace.
namespace {


// New PM implementation
struct MultiInstructionReduction: PassInfoMixin<MultiInstructionReduction> {
  // Main entry point, takes IR unit to run the pass on (&F) and the
  // corresponding pass manager (to be queried if need be)
  
PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    bool Changed = false; //Utile per tenere traccia se abbiamo fatto modifiche
   //Iter è un iteratore sui BasicBlock della funzione
    for (auto Iter = F.begin(); Iter != F.end(); ++Iter) {
        BasicBlock &B = *Iter;
        std::vector<Instruction*> InstrsToRemove;

        for (Instruction &Inst : B) {
                
            // 1. Cerchiamo l'istruzione di sottrazione (rappresenta "c = a - 1")
            if (Inst.getOpcode() == Instruction::Sub) {
                Value *SubOp0 = Inst.getOperand(0); // Questo dovrebbe essere 'a'
                Value *SubOp1 = Inst.getOperand(1); // Questo dovrebbe essere '1'

            // 2. Verifichiamo che stiamo effettivamente sottraendo 1
                ConstantInt *CSub = dyn_cast<ConstantInt>(SubOp1);
                if (CSub && CSub->isOne()) {
                    // 3. Ora guardiamo da dove arriva 'a'. È il risultato di un'istruzione?
                    if (Instruction *AddInst = dyn_cast<Instruction>(SubOp0)) {
                        // È un'istruzione di addizione? (rappresenta "a = b + 1")
                        if (AddInst->getOpcode() == Instruction::Add) {
                            Value *AddOp0 = AddInst->getOperand(0); 
                            Value *AddOp1 = AddInst->getOperand(1); 

                            Value *B_Value = nullptr; // Qui salveremo la nostra 'b'

                            // 4. Controlliamo se uno dei due operandi dell'Add è la costante 1
                            // (Dobbiamo controllare entrambi i lati per via della proprietà commutativa: b+1 o 1+b)
                            ConstantInt *CAdd0 = dyn_cast<ConstantInt>(AddOp0);
                            ConstantInt *CAdd1 = dyn_cast<ConstantInt>(AddOp1);
                            if (CAdd1 && CAdd1->isOne()) {
                                B_Value = AddOp0; // Pattern: b + 1 -> 'b' è il primo operando
                            } else if (CAdd0 && CAdd0->isOne()) {
                                B_Value = AddOp1; // Pattern: 1 + b -> 'b' è il secondo operando
                            }

                            // 5. Se abbiamo trovato esattamente la sequenza, applichiamo l'ottimizzazione
                            if (B_Value != nullptr) {
                                // Sostituiamo tutti gli usi dell'istruzione Sub ('c') con 'b'
                                Inst.replaceAllUsesWith(B_Value);
                                    
                                // Segniamo l'istruzione Sub per essere rimossa
                                InstrsToRemove.push_back(&Inst);
                                Changed = true;
                                    
                                // NOTA: Non rimuoviamo AddInst ("a = b + 1") qui. 
                                // Se 'a' non viene più usata da nessun'altra parte nel programma, 
                                // un successivo passo di Dead Code Elimination (DCE) la eliminerà in automatico.
                                }
                            }
                        }
                    }
                }
            }

            // Rimuoviamo le istruzioni morte alla fine dell'analisi del blocco
            for (Instruction *I : InstrsToRemove) {
                I->eraseFromParent();
            }
        }

    // Se abbiamo cambiato qualcosa, le analisi precedenti non sono più valide
    if (Changed) {
        return PreservedAnalyses::none();
    }
    return PreservedAnalyses::all();
}
};
}//namespace
//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getMultiInstructionReductionPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "MultiInstructionReduction", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback( [](StringRef Name, FunctionPassManager &FPM,ArrayRef<PassBuilder::PipelineElement>)  //Using these callbacks, callers can parse both a single pass name, as well as entire sub-pipelines, and populate the PassManager instance accordingly. 
              {
                if (Name == "multi-instruction-reduction") {
                  FPM.addPass(MultiInstructionReduction());
                  return true;
                }
                return false;
              });
          }
        };
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize TestPass when added to the pass pipeline on the
// command line, i.e. via '-passes=test-pass'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getMultiInstructionReductionPluginInfo();
}

  
