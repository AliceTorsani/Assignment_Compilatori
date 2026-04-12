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
struct AlgebraicIdentity: PassInfoMixin<AlgebraicIdentity> {
  // Main entry point, takes IR unit to run the pass on (&F) and the
  // corresponding pass manager (to be queried if need be)
  
PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    bool Changed = false; //Utile per tenere traccia se abbiamo fatto modifiche
    for (auto Iter = F.begin(); Iter != F.end(); ++Iter) { //Iter è un iteratore sui BasicBlock della funzione
        BasicBlock &B = *Iter;
        std::vector<Instruction*> InstrsToRemove;
        
        for (Instruction &inst : B) { //vado a analizzare ogni istruzione del blocco base
            
            // =========================
            // ADDIZIONE
            // =========================
            if (inst.getOpcode() == Instruction::Add) {
                Value *op1 = inst.getOperand(0);
                Value *op2 = inst.getOperand(1);

                // Proviamo a vedere se op1 o op2 sono costanti intere
                ConstantInt *C1 = dyn_cast<ConstantInt>(op1);
                ConstantInt *C2 = dyn_cast<ConstantInt>(op2);

                // Se C1 esiste ed è uguale a 0 (0 + X)
                if (C1 && C1->isZero()) {
                    inst.replaceAllUsesWith(op2);
                    InstrsToRemove.push_back(&inst);
                    Changed = true;
                } 
                // Altrimenti se C2 esiste ed è uguale a 0 (X + 0)
                else if (C2 && C2->isZero()) {
                    inst.replaceAllUsesWith(op1);
                    InstrsToRemove.push_back(&inst);
                    Changed = true;
                }
            } 
            // =========================
            // MOLTIPLICAZIONE
            // =========================
            else if (inst.getOpcode() == Instruction::Mul) {
                Value *op1 = inst.getOperand(0);
                Value *op2 = inst.getOperand(1);

                ConstantInt *C1 = dyn_cast<ConstantInt>(op1);
                ConstantInt *C2 = dyn_cast<ConstantInt>(op2);

                // Se C1 esiste ed è uguale a 1 (1 * X)
                if (C1 && C1->isOne()) {
                    inst.replaceAllUsesWith(op2);
                    InstrsToRemove.push_back(&inst);
                    Changed = true;
                } 
                // Altrimenti se C2 esiste ed è uguale a 1 (X * 1)
                else if (C2 && C2->isOne()) {
                    inst.replaceAllUsesWith(op1);
                    InstrsToRemove.push_back(&inst);
                    Changed = true;
                }
            }
        }
        
        // Rimuovo tutte le istruzioni morte alla fine del blocco base
        for (Instruction *inst : InstrsToRemove) {
            inst->eraseFromParent();
        }
    }

    // Se abbiamo cambiato qualcosa, le analisi precedenti non sono più valide
    if (Changed) {
        return PreservedAnalyses::none();
    }
    return PreservedAnalyses::all();
}
};
}
//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getAlgebraicIdentityPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "AlgebraicIdentity", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback( [](StringRef Name, FunctionPassManager &FPM,ArrayRef<PassBuilder::PipelineElement>)  //Using these callbacks, callers can parse both a single pass name, as well as entire sub-pipelines, and populate the PassManager instance accordingly. 
              {
                if (Name == "algebraic-identity") {
                  FPM.addPass(AlgebraicIdentity());
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
  return getAlgebraicIdentityPluginInfo();
}

  
