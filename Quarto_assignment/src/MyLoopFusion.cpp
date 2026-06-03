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

namespace {
    
    struct LoopFusionPass : PassInfoMixin<LoopFusionPass> {
   
bool isIntermediateBlockSafe(BasicBlock *BB) {
    for (Instruction &I : *BB) {
        // Ignoriamo terminatori, nodi PHI e istruzioni di debug
        if (I.isTerminator() || isa<PHINode>(I) || I.isDebugOrPseudoInst()) {
            continue;
        }
        
        // Ignoriamo gli intrinseci di gestione memoria (es. lifetime_start)
        if (auto *II = dyn_cast<IntrinsicInst>(&I)) {
            if (II->getIntrinsicID() == Intrinsic::lifetime_start ||
                II->getIntrinsicID() == Intrinsic::lifetime_end) {
                continue;
            }
        }
        
        // --- Ignora l'assegnamento al contatore (es. i = 0) ---
        if (auto *Store = dyn_cast<StoreInst>(&I)) {
            // Otteniamo il puntatore della memoria in cui la store sta scrivendo,
            // rimuovendo eventuali cast di tipo superficiali
            Value *Dest = Store->getPointerOperand()->stripPointerCasts();
            
            // Se la destinazione è una variabile locale (AllocaInst),
            // è quasi certamente l'inizializzazione del contatore del for (int i = 0).
            if (isa<AllocaInst>(Dest)) {
                // Ignoriamo l'istruzione e andiamo avanti!
                continue; 
            }
        }

        // Se l'istruzione scrive in memoria (ed è sopravvissuta al check sopra, quindi NON è 
        // una variabile locale) o ha altri effetti collaterali, blocchiamo l'adiacenza.
        if (I.mayWriteToMemory() || I.mayHaveSideEffects()) {
            errs() << "  [DEBUG] Adiacenza bloccata dall'istruzione: " << I << "\n";
            return false;
        }
    }
    return true;
}
bool areLoopsAdjacent(Loop *L0, Loop *L1) {
    BranchInst *L0Guard = L0->getLoopGuardBranch(); 
    BranchInst *L1Guard = L1->getLoopGuardBranch();

    // Caso 1: Entrambi i loop sono guarded
    if (L0Guard && L1Guard) {
        if (L0Guard->getCondition() == L1Guard->getCondition()) {
            BasicBlock *GuardBlock = L0Guard->getParent();
            BasicBlock *NonLoopSuccessor = nullptr;
            
            for (BasicBlock *Succ : successors(GuardBlock)) {
                if (!L0->contains(Succ)) {
                    NonLoopSuccessor = Succ;
                    break;
                }
            }
            
            if (NonLoopSuccessor && NonLoopSuccessor == L1->getLoopPreheader()) {
                return isIntermediateBlockSafe(NonLoopSuccessor);
            }
        }
        return false;
    } 
    
    // Caso 2: Nessuno dei due loop è guarded
    if (!L0Guard && !L1Guard) {
        BasicBlock *L0Exit = L0->getExitBlock();
        if (L0Exit != nullptr && L0Exit == L1->getLoopPreheader()) {
            return isIntermediateBlockSafe(L0Exit);
        }
        return false;
    }

    return false;
}   

private:
    bool areLoopsControlFlowEquivalent(Loop *L0, Loop *L1, DominatorTree &DT, PostDominatorTree &PDT) {
        BasicBlock *HeaderL0 = L0->getHeader();
        BasicBlock *HeaderL1 = L1->getHeader();

        return DT.dominates(HeaderL0, HeaderL1) && PDT.dominates(HeaderL1, HeaderL0);
    }

    bool areLoopsAtSameLevel(Loop *L0, Loop *L1) {
        // Controlla che abbiano la stessa profondità di annidamento
        return L0->getLoopDepth() == L1->getLoopDepth();
    }

public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
        LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
        DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);
        PostDominatorTree &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);

        for (Loop *L0 : LI) {
            for (Loop *L1 : LI) {
                if (L0 == L1) continue;
            // EVITA STAMPE INVERSE (e logica superflua): 
            // Se L0 non domina l'header di L1, significa che L0 non precede L1 nel codice. 
            // Ignoriamo la coppia senza neanche stampare.
                if (!DT.dominates(L0->getHeader(), L1->getHeader())) {
                continue;
              }
                if (areLoopsAtSameLevel(L0, L1) && areLoopsAdjacent(L0, L1) && areLoopsControlFlowEquivalent(L0, L1, DT, PDT)) {
                    errs() << "I loop sono adiacenti e hanno flussi di controllo equivalenti. Procedo con la fusione.\n";
                    // TODO: Implementazione della fusione dei loop L0 e L1
                } else {
                    errs() << "I loop non sono adiacenti o non hanno flussi di controllo equivalenti. Non posso fonderli.\n";
                }
            }
        }
        
        // Modifica in base alle tue necessità, se alteri il CFG potresti non preservare tutto.
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
                  if (Name == "my-loop-fusion") {
                    FPM.addPass(LoopFusionPass());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getLoopFusionPluginInfo();
}
