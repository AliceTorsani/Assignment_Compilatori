; ModuleID = './loopIf.ll'
source_filename = "./loopIf.cpp"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx16.0.0"

; Function Attrs: mustprogress noinline nounwind ssp uwtable(sync)
define noundef i32 @_Z16provaAssignementi(i32 noundef %0) #0 {
  %2 = add nsw i32 5, 5
  br label %3

3:                                                ; preds = %15, %1
  %.0 = phi i32 [ 0, %1 ], [ %4, %15 ]
  %4 = add nsw i32 %.0, 1
  %5 = sub nsw i32 0, %0
  %6 = add nsw i32 10, 20
  %7 = sdiv i32 %6, 10
  %8 = sitofp i32 %7 to float
  %9 = add nsw i32 10, %5
  %10 = add nsw i32 %6, %9
  %11 = icmp sgt i32 %0, 2
  br i1 %11, label %12, label %14

12:                                               ; preds = %3
  %13 = add nsw i32 10, 3
  br label %14

14:                                               ; preds = %12, %3
  br label %15

15:                                               ; preds = %14
  %16 = icmp slt i32 %4, 100
  br i1 %16, label %3, label %17, !llvm.loop !6

17:                                               ; preds = %15
  %18 = trunc i8 1 to i1
  %19 = xor i1 %18, true
  %20 = zext i1 %19 to i8
  %21 = add nsw i32 5, %4
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
