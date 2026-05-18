; ModuleID = 'loopannidati.m2r.ll'
source_filename = "./loopannidati.cpp"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx16.0.0"

; Function Attrs: mustprogress noinline norecurse nounwind ssp uwtable(sync)
define noundef i32 @main() #0 {
  %1 = add nsw i32 1, 2
  %2 = mul nsw i32 1, 2
  br label %3

3:                                                ; preds = %20, %0
  %.02 = phi i32 [ 0, %0 ], [ %21, %20 ]
  %4 = icmp slt i32 %.02, 200
  br i1 %4, label %5, label %22

5:                                                ; preds = %3
  br label %6

6:                                                ; preds = %17, %5
  %.01 = phi i32 [ 0, %5 ], [ %16, %17 ]
  %7 = add nsw i32 %.02, 1
  br label %8

8:                                                ; preds = %13, %6
  %.0 = phi i32 [ 0, %6 ], [ %12, %13 ]
  %9 = add nsw i32 2, %.01
  %10 = add nsw i32 %1, %.0
  %11 = add nsw i32 %7, %9
  %12 = add nsw i32 %.0, 1
  br label %13

13:                                               ; preds = %8
  %14 = icmp slt i32 %12, 200
  br i1 %14, label %8, label %15, !llvm.loop !6

15:                                               ; preds = %13
  %16 = add nsw i32 %.01, 1
  br label %17

17:                                               ; preds = %15
  %18 = icmp slt i32 %16, 200
  br i1 %18, label %6, label %19, !llvm.loop !8

19:                                               ; preds = %17
  br label %20

20:                                               ; preds = %19
  %21 = add nsw i32 %.02, 1
  br label %3, !llvm.loop !9

22:                                               ; preds = %3
  %23 = add nsw i32 0, 1
  ret i32 0
}

attributes #0 = { mustprogress noinline norecurse nounwind ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 26, i32 0]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"Homebrew clang version 19.1.7"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
