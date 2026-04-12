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
struct FirstAssignment: PassInfoMixin<FirstAssignment> {
  // Main entry point, takes IR unit to run the pass on (&F) and the
  // corresponding pass manager (to be queried if need be)
  
  
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {

    for (auto Iter = F.begin(); Iter != F.end(); ++Iter) {
      BasicBlock &B = *Iter;
      std::vector<Instruction*> InstrsToRemove;
      //prendo tutte le istruzioni del BB una alla volta
      for (Instruction &inst : B) {
        if (inst.getOpcode() == Instruction::Add) {
            Value *op1 = inst.getOperand(0);
            Value *op2 = inst.getOperand(1);
            if (op1==0 && op2!=0){
              //Sostituisco tutti gli usi dell
              inst.replaceAllUsesWith(op2);
              //Rimuovo l'istruzione add
              InstrsToRemove.push_back(&inst);
            } else if (op1 !=0 && op2==0){
              //bisogna sostituire l'istruzione con un'istruzione di assegnamento del primo operando al risultato dell'add
              inst.replaceAllUsesWith(op1);
              InstrsToRemove.push_back(&inst);
            }
        } else if (inst.getOpcode()==Instruction::Mul){
            Value *op1 = inst.getOperand(0);
            Value *op2 = inst.getOperand(1);
            if (op1==1 && op2!=1){
              //Sostituisco tutti gli usi dell'istruzione di moltiplicazione con il secondo operando
              inst.replaceAllUsesWith(op2);
              //Rimuovo l'istruzione di moltiplicazione
              InstrsToRemove.push_back(&inst);
            } else if (op1!=1 && op2==1){
              //Sostituisco tutti gli usi dell'istruzione di moltiplicazione con il primo operando
              inst.replaceAllUsesWith(op1);
              //Rimuovo l'istruzione di moltiplicazione
              InstrsToRemove.push_back(&inst);
            }
        }
      }
      //Rimuovo tutte le istruzioni morte
      for (Instruction *inst : InstrsToRemove) {
        inst->eraseFromParent();
      }
    }

    return PreservedAnalyses::all();
  }
  
  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }
};
} // namespace

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getFirstAssignmentPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "FirstAssignment", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback( [](StringRef Name, FunctionPassManager &FPM,ArrayRef<PassBuilder::PipelineElement>)  //Using these callbacks, callers can parse both a single pass name, as well as entire sub-pipelines, and populate the PassManager instance accordingly. 
              {
                if (Name == "first-assignment") {
                  FPM.addPass(FirstAssignment());
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
  return getFirstAssignmentPluginInfo();
}

  
