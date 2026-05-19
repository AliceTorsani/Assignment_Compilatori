; ModuleID = 'loopIf.m2r.ll'
source_filename = "./loopIf.cpp"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx16.0.0"

; Function Attrs: mustprogress noinline nounwind ssp uwtable(sync)
define noundef i32 @_Z16provaAssignementi(i32 noundef %0) #0 {
  %2 = add nsw i32 5, 5
  %3 = add nsw i32 10, 20
  %4 = sdiv i32 %3, 10
  %5 = sitofp i32 %4 to float
  %6 = add nsw i32 10, 3
  br label %7

7:                                                ; preds = %15, %1
  %.0 = phi i32 [ 0, %1 ], [ %8, %15 ]
  %8 = add nsw i32 %.0, 1
  %9 = sub nsw i32 0, %0
  %10 = add nsw i32 10, %9
  %11 = add nsw i32 %3, %10
  %12 = icmp sgt i32 %0, 2
  br i1 %12, label %13, label %14

13:                                               ; preds = %7
  br label %14

14:                                               ; preds = %13, %7
  br label %15

15:                                               ; preds = %14
  %16 = icmp slt i32 %8, 100
  br i1 %16, label %7, label %17, !llvm.loop !6

17:                                               ; preds = %15
  %18 = trunc i8 1 to i1
  %19 = xor i1 %18, true
  %20 = zext i1 %19 to i8
  %21 = add nsw i32 5, %8
  %22 = add nsw i32 %21, %0
  ret i32 %22
}

attributes #0 = { mustprogress noinline nounwind ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }

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
