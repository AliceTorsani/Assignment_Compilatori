#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"
#include "llvm/Transforms/Utils/LoopSimplify.h"
#include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/PostDominators.h"
#include <algorithm>

using namespace llvm;

//TODO controllare che ci sia un solo exit block
//TODO controllare quando viene controllato il preheader

namespace {

// New PM implementation
// Prima di eseguite il passo, eseguire nella cartella /test:
// opt -passes="loop-simplify,loop-rotate" Test.m2r.ll -S -o Test.simplified.ll
// Dopodichè eseguire il passo 
// Invocare il passo nella cartella /test nel seguente modo:
// opt -S -load-pass-plugin ../build/libLoopFusion.so -p my-loop-fusion Test.simplified.ll -o Test.optimized.ll 
struct LoopFusion: PassInfoMixin<LoopFusion> {

  // Entry point del Function Pass
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {

    outs() << "\n------------------------\nRunning LoopFusionPass on function: "
           << F.getName() << "\n--------------\n";

        // Recupera la loop tree della funzione
        LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
        ScalarEvolution &SE = AM.getResult<ScalarEvolutionAnalysis>(F);
        
        // Recupera l'analisi delle dipendenze 
        DependenceInfo &DI = AM.getResult<DependenceAnalysis>(F);
        
        // Recupero l'albero delle dipendenze e delle post-dipendenze
        DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);
        PostDominatorTree &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);  
        
        // Inizia dai top-level loops
        std::vector<Loop*> topLevelLoops = LI.getTopLevelLoops();
        
        if(topLevelLoops.size() > 0) {
            std::reverse(topLevelLoops.begin(), topLevelLoops.end());
            processLoopSiblings(topLevelLoops, SE, DI, DT, PDT);
        }

    return PreservedAnalyses::all();
  }

  private:
    bool verbose = true;

    //========================================================
    // Normalizza il Trip Count in base alla struttura del loop
    // (Top-Tested vs Bottom-Tested)
    //========================================================
    const SCEV *normalizeTripCount(Loop *L, const SCEV *TC, ScalarEvolution &SE) {
        if (isa<SCEVCouldNotCompute>(TC)) 
            return TC;

        BasicBlock *ExitingBlock = L->getExitingBlock();
        BasicBlock *LatchBlock = L->getLoopLatch();

        // Se il blocco di uscita coincide con il Latch, è un do-while (bottom-tested)
        // Aggiungiamo matematicamente 1 al BackedgeTakenCount per ottenere i giri reali
        if (ExitingBlock && LatchBlock && ExitingBlock == LatchBlock) {
            // Creiamo la costante SCEV '1' con lo stesso tipo (es. i32 o i64) del Trip Count
            const SCEV *One = SE.getOne(TC->getType());
            // Restituiamo (TC + 1)
            return SE.getAddExpr(TC, One);
        }

        // Se il blocco di uscita è l'Header (for/while classico), 
        // il conto restituito da SCEV è già pari alle iterazioni reali.
        return TC;
    }

    //========================================================
    // 4. Verifica le dipendenze a distanza negativa (Backward)
    // tra due loop.
    //========================================================
    bool hasNegativeDistanceDependence(Loop *L0, Loop *L1, DependenceInfo &DI) {
        
        // Iteriamo su tutti i Basic Block del primo loop
        for (BasicBlock *BB0 : L0->blocks()) {
            // Iteriamo su tutte le istruzioni di BB0
            for (Instruction &I0 : *BB0) {
                
                // Analizziamo solo istruzioni che leggono o scrivono in memoria
                if (!I0.mayReadOrWriteMemory()) continue;

                // Iteriamo sui Basic Block del secondo loop
                for (BasicBlock *BB1 : L1->blocks()) {
                    // Iteriamo sulle istruzioni di BB1
                    for (Instruction &I1 : *BB1) {
                        
                        if (!I1.mayReadOrWriteMemory()) continue;

                        // Chiediamo all'analisi se c'è dipendenza 
                        // 'true' indica SameSD (controlla anche scalari)
                        auto Dep = DI.depends(&I0, &I1, true);

                        // Se esiste una dipendenza
                        if (Dep) {
                            unsigned Levels = Dep->getLevels();
                            
                            for (unsigned Level = 1; Level <= Levels; ++Level) {
                                // Recuperiamo la direzione della dipendenza 
                                unsigned Dir = Dep->getDirection(Level);
                                
                                // Verifichiamo se è Backward (GT - Greater Than)
                                if (Dir & Dependence::DVEntry::GT) {
                                    if (verbose) {
                                        errs() << "  -> Fallito: Trovata dipendenza a distanza negativa (GT) tra:\n";
                                        errs() << "     L0 Inst: "; I0.print(errs()); errs() << "\n";
                                        errs() << "     L1 Inst: "; I1.print(errs()); errs() << "\n";
                                    }
                                    return true; // Trovata dipendenza negativa, non si può fondere
                                }
                            }
                        }
                    }
                }
            }
        }
        
        // Nessuna dipendenza a distanza negativa trovata
        return false;
    }
    //==============================================================
    // Controllo che abbiano il control flow equivalente
    //==============================================================
   bool areLoopsControlFlowEquivalent(Loop *L0, Loop *L1, DominatorTree &DT, PostDominatorTree &PDT)  {
        BasicBlock *HeaderL0 = L0->getHeader();
        BasicBlock *HeaderL1 = L1->getHeader();
        return DT.dominates(HeaderL0, HeaderL1) && PDT.dominates(HeaderL1, HeaderL0);  
        }
    //========================================================
    // Visita ricorsiva dei siblings
    //========================================================
    void processLoopSiblings(ArrayRef<Loop *> Loops, ScalarEvolution &SE, DependenceInfo &DI, DominatorTree &DT, PostDominatorTree &PDT) {
        // Controlla coppie consecutive di siblings
        for (unsigned i = 0; i+1 < Loops.size(); ++i) {
            Loop *L0 = Loops[i];
            Loop *L1 = Loops[i + 1];

            if(verbose) {
                errs() << "\n============================================\n";
                errs() << "Checking adjacency between:\n";
                errs() << "  L0 header: "; L0->getHeader()->printAsOperand(outs(), false); errs() << "\n";
                errs() << "  L1 header: "; L1->getHeader()->printAsOperand(outs(), false); errs() << "\n";
                errs() << "\tl'exiting block di L0: "; 
                if (L0->getExitingBlock()) L0->getExitingBlock()->printAsOperand(outs(), false);
                errs() << "\n";
            }

            // 1. CONTROLLO ADIACENZA
            if (!areAdjacent(L0, L1)) {
                if(verbose) errs() << "  --> NOT adjacent\n";
                continue; // Salta al prossimo paio di loop
            }
            if(verbose) errs() << "  --> Adjacent loops found!\n";
            

            // 2. CALCOLO DEL TRIP COUNT DEI LOOP
            bool sameTripCount = false;
            if(verbose) outs() << "\n---CONTROLLO TRIP COUNT---\n";
            
            const SCEV *TC0 = SE.getSymbolicMaxBackedgeTakenCount(L0);
            const SCEV *TC1 = SE.getSymbolicMaxBackedgeTakenCount(L1);
            
            // Normalizziamo i Trip Count in base alla struttura dei loop
            TC0 = normalizeTripCount(L0, TC0, SE);
            TC1 = normalizeTripCount(L1, TC1, SE);
            
            if (isa<SCEVCouldNotCompute>(TC0) || isa<SCEVCouldNotCompute>(TC1)) {
                if (verbose) errs() << "Impossibile calcolare il Trip Count per L0 o L1.\n";
            } else {
                if (TC0 == TC1) {
                    sameTripCount = true;
                    if (verbose) errs() << "I due loop hanno lo stesso Trip Count esatto (Puntatori SCEV identici)!\n";
                } 
                else {
                    const SCEV *Difference = SE.getMinusSCEV(TC0, TC1);
                    if (Difference->isZero()) {
                        sameTripCount = true;
                        if (verbose) errs() << "I due loop hanno lo stesso Trip Count esatto (Differenza matematica = 0)!\n";
                    } else {
                        if (verbose) {
                            errs() << "I Trip Count differiscono.\n";
                            if (isa<SCEVConstant>(Difference)) {
                                errs() << "  -> Nota: Differiscono solo per un valore costante.\n";
                            }
                        }
                    }
                }
            } 
            
            if (!sameTripCount) {
                if(verbose) errs() << "Fusione interrotta: il numero di iterazioni non corrisponde.\n";
                continue; // Salta al prossimo paio di loop
            }
            if (verbose) errs()<<"\n---CONTROLLO CONTROL FLOW ---\n";
            if (!areLoopsControlFlowEquivalent(L0, L1, DT, PDT)){
            errs() << "\nI DUE LOOP NON HANNO LO STESSO CONTROL FLOW \n";
            errs()<< "\n Passo ai prossimi due loop da analizzare \n";
            continue;
            }   
            errs()<<"Hanno lo stesso control flow i due loop \n";
            // 4. CONTROLLO DIPENDENZE
            if(verbose) outs() << "\n---CONTROLLO DIPENDENZE---\n";
            
            if (hasNegativeDistanceDependence(L0, L1, DI) ) {
                if (verbose) errs() << "I loop NON possono essere fusi (Dipendenza a distanza negativa riscontrata).\n";
            } else {
                if (verbose) errs() << "\n*** I loop L0 e L1 possono essere fusi! (Tutte le condizioni soddisfatte) ***\n";
                
                // TODO: Qui inizierai ad implementare la Trasformazione
            }
        }

        // Ricorsione sui subloops
        for (Loop *L : Loops) {
            if(L->getSubLoops().size() != 0)
                processLoopSiblings(L->getSubLoops(), SE, DI,DT,PDT);
        }
    }

    //========================================================
    // Verifica completa di adiacenza
    //========================================================
    bool areAdjacent(Loop *L0, Loop *L1) {

        if (!L0 || !L1)
            return false;

        if(verbose) {
            outs() << "Il loop L0 è guarded? " << L0->isGuarded() << "\n";
            outs() << "Il loop L1 è guarded? " << L1->isGuarded() << "\n";
        }
        
        //----------------------------------------------------
        // CASO 1: loop guarded
        //----------------------------------------------------
        if (L0->isGuarded() && L1->isGuarded()) {

            errs() << "caso 1: i due loop sono guarded\n";
            
            if (!haveSameGuard(L0, L1)){
                errs() << "I due loop non hanno la stessa guardia:\n\t";
                return false;
            }

            errs() << "I due loop hanno la stessa guardia\n";

            BranchInst *GuardBI1 = L1->getLoopGuardBranch();

            if (!GuardBI1)
                return false;

            BasicBlock *GuardBB = GuardBI1->getParent();

            if (!isSimpleGuardBlock(GuardBB)){
                errs() <<"Il guard block del secondo loop non è pulito e ha istruzioni intermedie\n";
                return false;
            }

            errs() << "Il guard block del secondo loop è pulito\n";

            BranchInst *GuardBI0 = L0->getLoopGuardBranch();

            if (!GuardBI0)
                return false;
            
            BasicBlock *NonLoopSucc = nullptr;

            for (BasicBlock *Succ : GuardBI0->successors()) {
                if (!L0->contains(Succ)) {
                    NonLoopSucc = Succ;
                    break;
                }
            }

            if (!NonLoopSucc)
                return false;

            return NonLoopSucc == GuardBB;
        }

        //----------------------------------------------------
        // CASO 2: loop non guarded
        //----------------------------------------------------
        if(!L0->isGuarded() && !L1->isGuarded()){

            errs() <<"caso 2: i due loop non sono guarded\n";

            BasicBlock *ExitBlock = L0->getExitBlock();
            if (verbose) {
                outs() << "\texit block LO: ";
                if (ExitBlock) ExitBlock->printAsOperand(outs(), false);
                errs() << "\n";
            }

            BasicBlock *Preheader = L1->getLoopPreheader();
            
            if (!ExitBlock)
                return false;
            
            if(!Preheader) {
                Preheader=L1->getHeader();
            }

            if (ExitBlock != Preheader)
                return false;

            if (!isEmptyPreheader(Preheader)){
                errs() << "Ci sono istruzioni intermedie nel preheader del secondo loop\n";
                return false;
            }

            errs() << "Il preheader del secondo loop è pulito: non ci sono istruzioni intermedie\n";

            return true;
        }

        errs() << "Ho trovato un loop guarded e uno non guarded, non possono essere fusi\n";
        return false;
    }

    //========================================================
    // Verifica che il preheader sia vuoto
    //========================================================
    bool isEmptyPreheader(BasicBlock *BB) {
        Instruction *Terminator = BB->getTerminator();
        for (Instruction &I : *BB) {
            if (&I == Terminator) continue;
            if (I.isDebugOrPseudoInst()) continue;
            if (isa<PHINode>(&I)) continue;
            return false;
        }
        return true;
    }

    //========================================================
    // Verifica che il guard block sia semplice
    //========================================================
    bool isSimpleGuardBlock(BasicBlock *BB) {
        for (Instruction &I : *BB) {
            if (I.isDebugOrPseudoInst()) continue;
            if (isa<PHINode>(&I)) continue;
            if (isa<BranchInst>(&I)) continue;
            if (isa<ICmpInst>(&I)) continue;
            if (isa<FCmpInst>(&I)) continue;
            return false;
        }
        return true;
    }

    //========================================================
    // Verifica che due loop guarded abbiano la stessa guardia
    //========================================================
    bool haveSameGuard(Loop *L0, Loop *L1) {
        if(verbose) outs() << "\n---CONTROLLO GUARDIE---\n";
        
        if (!L0->isGuarded() || !L1->isGuarded())
            return false;

        BranchInst *GuardBI0 = L0->getLoopGuardBranch();
        BranchInst *GuardBI1 = L1->getLoopGuardBranch();

        if (!GuardBI0 || !GuardBI1)
            return false;

        if (GuardBI0 == GuardBI1)
            return true;

        Value *Cond0 = GuardBI0->getCondition();
        Value *Cond1 = GuardBI1->getCondition();
        
        if(verbose) { 
            outs() << "\tcondizione 1: "; Cond0->print(outs()); 
            outs() << "\n\tcondizione 2: "; Cond1->print(outs()); outs() << "\n";
        }
            
        if(ICmpInst* CondInst0 = dyn_cast<ICmpInst>(Cond0)) {
            if(ICmpInst* CondInst1 = dyn_cast<ICmpInst>(Cond1)) {

                if(CondInst0->getPredicate() == CondInst1->getPredicate()) {
                    if((CondInst0->getOperand(0) == CondInst1->getOperand(0)) &&
                    (CondInst0->getOperand(1) == CondInst1->getOperand(1))) {
                        if(verbose){outs() << "corrispondono\n";}
                        return true;
                    }
                }
                if(CondInst0->getPredicate() == CondInst1->getSwappedPredicate()) {
                    if((CondInst0->getOperand(0) == CondInst1->getOperand(1)) &&
                    (CondInst0->getOperand(1) == CondInst1->getOperand(0))) {
                        if(verbose){outs() << "sono equivalenti\n";}
                        return true;
                    }
                }
            }
        }
        return false;
    }

    static bool isRequired() { return true; }
};  
}//namespace

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
                    FPM.addPass(LoopFusion());
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
