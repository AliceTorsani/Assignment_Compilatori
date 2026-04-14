#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>

using namespace llvm;

namespace {

struct AlgebraicIdentity : PassInfoMixin<AlgebraicIdentity> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
        bool Changed = false;
        
        // Iteriamo sui BasicBlock della funzione
        for (BasicBlock &B : F) { 
            std::vector<Instruction*> InstrsToRemove;
            
            // Analizziamo ogni istruzione del blocco base
            for (Instruction &inst : B) { 
                BinaryOperator *BinOp = dyn_cast<BinaryOperator>(I);
                if (!BinOp) continue; // Se non lo è, passiamo alla prossima
                // =========================
                // ADDIZIONE
                // =========================
                if (inst.getOpcode() == Instruction::Add) {
                    Value *op1 = inst.getOperand(0);
                    Value *op2 = inst.getOperand(1);

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
        return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
    }
};

} // Fine namespace anonimo

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION, "AlgebraicIdentity", LLVM_VERSION_STRING,
        [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM, ArrayRef<PassBuilder::PipelineElement>) {
                    // ATTENZIONE: Questo è il nome da usare nel terminale!
                    if (Name == "algebraic-identity") { 
                        FPM.addPass(AlgebraicIdentity());
                        return true;
                    }
                    return false;
                });
        }
    };
}
  
