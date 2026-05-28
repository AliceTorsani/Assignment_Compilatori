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
#include "llvm/Analysis/PostDominators.h"

using namespace llvm;

// Invocare il passo nella cartella /test nel seguente modo: 
// opt -S -load-pass-plugin ../build/LoopFusion.so -p loop-fusion Test.m2r.ll -o Test.optimized.ll
namespace {
    
    struct LoopFusionPass :  PassInfoMixin<LoopFusionPass> {
    
    bool areLoopsAdjacent(Loop *L0, Loop *L1) {
    // Verifica presenza di blocco di guardia per L0
    if (BranchInst *L0Guard = L0->getLoopGuardBranch()) {
        // Se L0 è guarded, identifica il blocco di guardia e i suoi successori
        BasicBlock *GuardBlock = L0Guard->getParent();
        BasicBlock *NonLoopSuccessor = nullptr;
        
        // Identificazione del successore che non entra nel body di L0
        for (BasicBlock *Succ : successors(GuardBlock)) {
           // Se il successore non è contenuto in L0, è il candidato per essere il preheader di L1
            if (!L0->contains(Succ)) {
                NonLoopSuccessor = Succ;
                break;
            }
        }
        
        // Se i loop sono guarded, il successore non loop del guard branch di L0 
        // deve essere l'entry block di L1
        return NonLoopSuccessor == L1->getLoopPreheader(); 
    } 
    if (L1->getLoopPreheader != nullptr){
      return false
    }
    // Se i loop non sono guarded, l'exit block di L0 deve essere 
    // direttamente il preheader di L1
    return L0->getExitBlock() == L1->getLoopPreheader(); 
    }
    private:
    bool areLoopsControlFlowEquivalent(Loop *L0, Loop *L1, DominatorTree &DT, PostDominatorTree &PDT) {
        BasicBlock *HeaderL0 = L0->getHeader();
        BasicBlock *HeaderL1 = L1->getHeader();

        return DT.dominates(HeaderL0, HeaderL1) && PDT.dominates(HeaderL1, HeaderL0);
    }
  public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
        errs()<<"Stampa di prova ";
        LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
        // Restituisce l'albero dei dominatori e dei post-dominatori
        DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);
        PostDominatorTree &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);

        for (Loop *L0 : LI) {
            for (Loop *L1 : LI) {
                if (L0 == L1) continue;

                if (areLoopsAdjacent(L0, L1) && areLoopsControlFlowEquivalent(L0, L1, DT, PDT)) {
                    // Implementazione della fusione dei loop L0 e L1
                    errs()<<"I loop sono adiacenti e hanno flussi di controllo equivalenti. Procedo con la fusione.\n";
                } else {
                    errs()<<"I loop non sono adiacenti o non hanno flussi di controllo equivalenti. Non posso fonderli.\n";
                }
            }
        }
        
        return PreservedAnalyses::all();
    }
    };
}
//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getLoopFusionPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "LoopFusion", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "loop-fusion") {
                    FPM.addPass(LoopFusionPass());
                    return true;
                  }
                  return false;
                });
          }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize LoopFusionPass when added to the pass pipeline on the
// command line, i.e. via '-passes=loop-fusion'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getLoopFusionPluginInfo();
}

