; ModuleID = 'isNotUnary.ll'
source_filename = "./isNotUnary.cpp"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx16.0.0"

; Function Attrs: mustprogress noinline nounwind ssp uwtable(sync)
define noundef i32 @_Z16provaAssignementi(i32 noundef %0) #0 {
  %2 = add nsw i32 5, 5
  br label %3

3:                                                ; preds = %14, %1
  %.02 = phi i32 [ %0, %1 ], [ %6, %14 ]
  %.01 = phi i8 [ 1, %1 ], [ %9, %14 ]
  %.0 = phi i32 [ %0, %1 ], [ %.1, %14 ]
  %4 = icmp slt i32 %.02, 100
  br i1 %4, label %5, label %23

5:                                                ; preds = %3
  %6 = add nsw i32 %.02, 1
  %7 = trunc i8 %.01 to i1
  %8 = xor i1 %7, true
  %9 = zext i1 %8 to i8
  %10 = icmp sgt i32 %.0, 2
  br i1 %10, label %11, label %14

11:                                               ; preds = %5
  %12 = add nsw i32 %.0, 2
  %13 = add nsw i32 %.0, %12
  br label %14

14:                                               ; preds = %11, %5
  %.1 = phi i32 [ %13, %11 ], [ %.0, %5 ]
  %15 = sub nsw i32 0, %.1
  %16 = add nsw i32 10, 20
  %17 = sdiv i32 %16, 10
  %18 = sitofp i32 %17 to float
  %19 = add nsw i32 10, %6
  %20 = add nsw i32 10, %15
  %21 = add nsw i32 %16, %19
  %22 = add nsw i32 %21, %20
  br label %3, !llvm.loop !6

23:                                               ; preds = %3
  %24 = trunc i8 1 to i1
  %25 = xor i1 %24, true
  %26 = zext i1 %25 to i8
  %27 = add nsw i32 5, %.02
  %28 = add nsw i32 %27, %.0
  ret i32 %28
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
