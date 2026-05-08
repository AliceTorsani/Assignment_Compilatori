; ModuleID = 'provaTest.withUnary.ll'
source_filename = "./provaTest.cpp"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx16.0.0"

; Function Attrs: mustprogress noinline nounwind ssp uwtable(sync)
define noundef i32 @_Z16provaAssignementi(i32 noundef %0) #0 {
  %2 = add nsw i32 5, 5
  br label %3

3:                                                ; preds = %11, %1
  %.01 = phi i32 [ %0, %1 ], [ %6, %11 ]
  %.0 = phi i32 [ %0, %1 ], [ %.1, %11 ]
  %4 = icmp slt i32 %.01, 100
  br i1 %4, label %5, label %21

5:                                                ; preds = %3
  %6 = add nsw i32 %.01, 1
  %7 = icmp sgt i32 %.0, 2
  br i1 %7, label %8, label %11

8:                                                ; preds = %5
  %9 = add nsw i32 %.0, 2
  %10 = add nsw i32 %.0, %9
  br label %11

11:                                               ; preds = %8, %5
  %.1 = phi i32 [ %10, %8 ], [ %.0, %5 ]
  %12 = sub nsw i32 0, %.1
  %13 = add nsw i32 10, 20
  %14 = sdiv i32 %13, 10
  %15 = sitofp i32 %14 to float
  %16 = add nsw i32 10, %6
  %17 = add nsw i32 10, %12
  %18 = add nsw i32 %13, %16
  %19 = add nsw i32 %18, %17
  %20 = fneg float %15
  br label %3, !llvm.loop !6

21:                                               ; preds = %3
  %22 = trunc i8 1 to i1
  %23 = xor i1 %22, true
  %24 = zext i1 %23 to i8
  %25 = add nsw i32 5, %.01
  %26 = add nsw i32 %25, %.0
  ret i32 %26
}

; Function Attrs: mustprogress noinline nounwind ssp uwtable(sync)
define noundef i32 @_Z7icmTesti(i32 noundef %0) #0 {
  %2 = alloca [100 x i32], align 4
  br label %3

3:                                                ; preds = %15, %1
  %.0 = phi i32 [ %0, %1 ], [ %16, %15 ]
  %4 = icmp slt i32 %.0, 100
  br i1 %4, label %5, label %17

5:                                                ; preds = %3
  %6 = add nsw i32 10, 20
  %7 = mul nsw i32 %6, 2
  %8 = add nsw i32 %6, %.0
  %9 = getelementptr inbounds [100 x i32], ptr %2, i64 0, i64 0
  %10 = load i32, ptr %9, align 4
  %11 = add nsw i32 %6, %7
  %12 = sext i32 %.0 to i64
  %13 = getelementptr inbounds [100 x i32], ptr %2, i64 0, i64 %12
  store i32 %11, ptr %13, align 4
  %14 = add nsw i32 %7, 3
  br label %15

15:                                               ; preds = %5
  %16 = add nsw i32 %.0, 1
  br label %3, !llvm.loop !8

17:                                               ; preds = %3
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
