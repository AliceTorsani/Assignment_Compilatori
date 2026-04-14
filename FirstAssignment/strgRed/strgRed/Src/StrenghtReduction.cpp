#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
using namespace llvm;

namespace {

class StrengthReductionPass : public PassInfoMixin<StrengthReductionPass> {
public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
        bool Changed = false;

        for (BasicBlock &BB : F) {
            // Iterazione classica sicura: incrementiamo l'iteratore PRIMA di operare sull'istruzione,
            // così se la cancelliamo non facciamo crashare il ciclo.
            for (auto It = BB.begin(); It != BB.end(); ) {
                Instruction *I = &*It++; 
                
                // Proviamo a convertire l'istruzione in una generica operazione binaria (+, -, *, /)
                BinaryOperator *BinOp = dyn_cast<BinaryOperator>(I);
                if (!BinOp) continue; // Se non lo è, passiamo alla prossima

                // ***************************** 
                //  MOLTIPLICAZIONE in caso di costante
                // *****************************
                if (BinOp->getOpcode() == Instruction::Mul) {
                    Value *Op0 = BinOp->getOperand(0);
                    Value *Op1 = BinOp->getOperand(1);

                    Value *X = nullptr;
                    ConstantInt *CI = nullptr;

                    // Controllo se la costante è a destra o a sinistra
                    if (ConstantInt *C = dyn_cast<ConstantInt>(Op1)) {
                        CI = C;
                        X = Op0;
                    } else if (ConstantInt *C = dyn_cast<ConstantInt>(Op0)) {
                        CI = C;
                        X = Op1;
                    }

                    // Se abbiamo trovato una costante da almeno uno dei due lati
                    if (CI) {
                        APInt Val = CI->getValue();
                        IRBuilder<> Builder(BinOp);

                        // Caso 1: potenza di 2 perfetta → shift (es: x * 8 -> x << 3)
                        if (Val.isPowerOf2()) {
                            unsigned Shift = Val.logBase2();
                            Value *Shl = Builder.CreateShl(X, Shift);
                            BinOp->replaceAllUsesWith(Shl);
                            BinOp->eraseFromParent();
                            Changed = true;
                            continue;
                        }

                        // Caso 2: 2^n - 1 → (x << n) - x  (es: 15 = 16 - 1) 
                        //caso in cui la costante è una potenza di 2 meno 1, possiamo riscrivere l'operazione come uno shift seguito da una sottrazione
                        APInt PlusOne = Val + 1;
                        if (PlusOne.isPowerOf2()) {
                            unsigned Shift = PlusOne.logBase2();
                            Value *Shl = Builder.CreateShl(X, Shift);
                            Value *Sub = Builder.CreateSub(Shl, X);
                            BinOp->replaceAllUsesWith(Sub);
                            BinOp->eraseFromParent();
                            Changed = true;
                            continue;
                        }

                        // Caso 3: 2^n + 1 → (x << n) + x  (es: 9 = 8 + 1)
                        //caso in cui la costante è una potenza di 2 più 1, possiamo riscrivere l'operazione come uno shift seguito da un'addizione
                        APInt MinusOne = Val - 1;
                        if (MinusOne.isPowerOf2()) {
                            unsigned Shift = MinusOne.logBase2();
                            Value *Shl = Builder.CreateShl(X, Shift);
                            Value *Add = Builder.CreateAdd(Shl, X);
                            BinOp->replaceAllUsesWith(Add);
                            BinOp->eraseFromParent();
                            Changed = true;
                            continue;
                        }
                    }
                }

                // ======================
                // DIVISIONE
                // ======================
                // UDIV sta per unsigned division, SDIV per signed division. 
                //In entrambi i casi, se il divisore è una potenza di 2, possiamo sostituire la divisione con uno shift.
                if (BinOp->getOpcode() == Instruction::UDiv || BinOp->getOpcode() == Instruction::SDiv) {
                    
                    Value *X = BinOp->getOperand(0); // Il dividendo
                    Value *Y = BinOp->getOperand(1); // Il divisore

                    // Nella divisione l'ordine è fisso, la costante DEVE essere il divisore (Y)
                    if (ConstantInt *CI = dyn_cast<ConstantInt>(Y)) {
                        APInt Val = CI->getValue();

                        if (Val.isPowerOf2()) {
                            unsigned Shift = Val.logBase2();
                            IRBuilder<> Builder(BinOp);
                            Value *Shifted;

                            if (BinOp->getOpcode() == Instruction::UDiv) {
                                // unsigned → logical shift
                                Shifted = Builder.CreateLShr(X, Shift);
                            } else {
                                // signed → arithmetic shift
                                Shifted = Builder.CreateAShr(X, Shift);
                            }

                            BinOp->replaceAllUsesWith(Shifted);
                            BinOp->eraseFromParent();
                            Changed = true;
                            continue;
                        }
                    }
                }
            }
        }

        return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
    }
};

} // namespace

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getStrengthReductionPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "StrengthReduction", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback( [](StringRef Name, FunctionPassManager &FPM,ArrayRef<PassBuilder::PipelineElement>)  //Using these callbacks, callers can parse both a single pass name, as well as entire sub-pipelines, and populate the PassManager instance accordingly. 
              {
                if (Name == "StrengthReduction") {
                  FPM.addPass(StrengthReductionPass());
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
  return getStrengthReductionPluginInfo();
}

  
