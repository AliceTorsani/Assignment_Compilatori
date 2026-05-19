; ModuleID = 'stressUnary.m2r.ll'
source_filename = "./stressUnary.cpp"
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

7:                                                ; preds = %19, %1
  %.01 = phi i32 [ 0, %1 ], [ %8, %19 ]
  %.0 = phi i32 [ %0, %1 ], [ %.1, %19 ]
  %8 = add nsw i32 %.01, 1
  %9 = sub nsw i32 0, %.0
  %10 = add nsw i32 10, %8
  %11 = add nsw i32 10, %9
  %12 = add nsw i32 %3, %10
  %13 = add nsw i32 %12, %11
  %14 = icmp sgt i32 %.0, 2
  br i1 %14, label %15, label %18

15:                                               ; preds = %7
  %16 = add nsw i32 %.0, 2
  %17 = add nsw i32 %.0, %16
  br label %18

18:                                               ; preds = %15, %7
  %.1 = phi i32 [ %17, %15 ], [ %.0, %7 ]
  br label %19

19:                                               ; preds = %18
  %20 = icmp slt i32 %8, 100
  br i1 %20, label %7, label %21, !llvm.loop !6

21:                                               ; preds = %19
  %22 = trunc i8 1 to i1
  %23 = xor i1 %22, true
  %24 = zext i1 %23 to i8
  %25 = add nsw i32 5, %8
  %26 = add nsw i32 %25, %.1
  ret i32 %26
}

; Function Attrs: mustprogress noinline nounwind ssp uwtable(sync)
define noundef i32 @_Z7icmTesti(i32 noundef %0) #0 {
  %2 = alloca [100 x i32], align 4
  %3 = add nsw i32 10, 20
  %4 = mul nsw i32 %3, 2
  %5 = add nsw i32 %3, %4
  %6 = add nsw i32 %4, 3
  br label %7

7:                                                ; preds = %15, %1
  %.0 = phi i32 [ 0, %1 ], [ %16, %15 ]
  %8 = icmp slt i32 %.0, 100
  br i1 %8, label %9, label %17

9:                                                ; preds = %7
  %10 = add nsw i32 %3, %.0
  %11 = getelementptr inbounds [100 x i32], ptr %2, i64 0, i64 0
  %12 = load i32, ptr %11, align 4
  %13 = sext i32 %.0 to i64
  %14 = getelementptr inbounds [100 x i32], ptr %2, i64 0, i64 %13
  store i32 %5, ptr %14, align 4
  br label %15

15:                                               ; preds = %9
  %16 = add nsw i32 %.0, 1
  br label %7, !llvm.loop !8

17:                                               ; preds = %7
  %18 = add nsw i32 10, 18
  ret i32 0
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
!8 = distinct !{!8, !7}
