; ModuleID = './stressUnary.cpp'
source_filename = "./stressUnary.cpp"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx16.0.0"

; Function Attrs: mustprogress noinline nounwind ssp uwtable(sync)
define noundef i32 @_Z16provaAssignementi(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  %13 = alloca float, align 4
  %14 = alloca i32, align 4
  %15 = alloca i32, align 4
  %16 = alloca i32, align 4
  %17 = alloca i32, align 4
  %18 = alloca i8, align 1
  %19 = alloca i8, align 1
  store i32 %0, ptr %2, align 4
  store i32 5, ptr %3, align 4
  %20 = load i32, ptr %3, align 4
  %21 = add nsw i32 %20, 5
  store i32 %21, ptr %4, align 4
  store i32 10, ptr %5, align 4
  store i32 20, ptr %6, align 4
  store i32 0, ptr %7, align 4
  store i32 3, ptr %8, align 4
  store i32 0, ptr %9, align 4
  br label %22

22:                                               ; preds = %59, %1
  %23 = load i32, ptr %9, align 4
  %24 = add nsw i32 %23, 1
  store i32 %24, ptr %9, align 4
  %25 = load i32, ptr %9, align 4
  store i32 %25, ptr %10, align 4
  %26 = load i32, ptr %2, align 4
  %27 = sub nsw i32 0, %26
  store i32 %27, ptr %11, align 4
  %28 = load i32, ptr %5, align 4
  %29 = load i32, ptr %6, align 4
  %30 = add nsw i32 %28, %29
  store i32 %30, ptr %12, align 4
  %31 = load i32, ptr %12, align 4
  %32 = load i32, ptr %5, align 4
  %33 = sdiv i32 %31, %32
  %34 = sitofp i32 %33 to float
  store float %34, ptr %13, align 4
  %35 = load i32, ptr %5, align 4
  %36 = load i32, ptr %10, align 4
  %37 = add nsw i32 %35, %36
  store i32 %37, ptr %14, align 4
  %38 = load i32, ptr %5, align 4
  %39 = load i32, ptr %11, align 4
  %40 = add nsw i32 %38, %39
  store i32 %40, ptr %15, align 4
  %41 = load i32, ptr %12, align 4
  %42 = load i32, ptr %14, align 4
  %43 = add nsw i32 %41, %42
  %44 = load i32, ptr %15, align 4
  %45 = add nsw i32 %43, %44
  store i32 %45, ptr %7, align 4
  %46 = load i32, ptr %2, align 4
  %47 = icmp sgt i32 %46, 2
  br i1 %47, label %48, label %58

48:                                               ; preds = %22
  %49 = load i32, ptr %2, align 4
  store i32 %49, ptr %16, align 4
  %50 = load i32, ptr %5, align 4
  %51 = load i32, ptr %8, align 4
  %52 = add nsw i32 %50, %51
  store i32 %52, ptr %17, align 4
  %53 = load i32, ptr %16, align 4
  %54 = add nsw i32 %53, 2
  store i32 %54, ptr %16, align 4
  %55 = load i32, ptr %16, align 4
  %56 = load i32, ptr %2, align 4
  %57 = add nsw i32 %56, %55
  store i32 %57, ptr %2, align 4
  br label %58

58:                                               ; preds = %48, %22
  br label %59

59:                                               ; preds = %58
  %60 = load i32, ptr %9, align 4
  %61 = icmp slt i32 %60, 100
  br i1 %61, label %22, label %62, !llvm.loop !6

62:                                               ; preds = %59
  store i8 1, ptr %18, align 1
  %63 = load i8, ptr %18, align 1
  %64 = trunc i8 %63 to i1
  %65 = xor i1 %64, true
  %66 = zext i1 %65 to i8
  store i8 %66, ptr %19, align 1
  %67 = load i32, ptr %3, align 4
  %68 = load i32, ptr %9, align 4
  %69 = add nsw i32 %67, %68
  %70 = load i32, ptr %2, align 4
  %71 = add nsw i32 %69, %70
  ret i32 %71
}

; Function Attrs: mustprogress noinline nounwind ssp uwtable(sync)
define noundef i32 @_Z7icmTesti(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca [100 x i32], align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  %13 = alloca i32, align 4
  %14 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  store i32 10, ptr %3, align 4
  store i32 20, ptr %4, align 4
  store i32 0, ptr %5, align 4
  store i32 3, ptr %6, align 4
  store i32 0, ptr %8, align 4
  br label %15

15:                                               ; preds = %38, %1
  %16 = load i32, ptr %8, align 4
  %17 = icmp slt i32 %16, 100
  br i1 %17, label %18, label %41

18:                                               ; preds = %15
  %19 = load i32, ptr %3, align 4
  %20 = load i32, ptr %4, align 4
  %21 = add nsw i32 %19, %20
  store i32 %21, ptr %9, align 4
  %22 = load i32, ptr %9, align 4
  %23 = mul nsw i32 %22, 2
  store i32 %23, ptr %10, align 4
  %24 = load i32, ptr %9, align 4
  %25 = load i32, ptr %8, align 4
  %26 = add nsw i32 %24, %25
  store i32 %26, ptr %11, align 4
  %27 = getelementptr inbounds [100 x i32], ptr %7, i64 0, i64 0
  %28 = load i32, ptr %27, align 4
  store i32 %28, ptr %12, align 4
  %29 = load i32, ptr %9, align 4
  %30 = load i32, ptr %10, align 4
  %31 = add nsw i32 %29, %30
  %32 = load i32, ptr %8, align 4
  %33 = sext i32 %32 to i64
  %34 = getelementptr inbounds [100 x i32], ptr %7, i64 0, i64 %33
  store i32 %31, ptr %34, align 4
  %35 = load i32, ptr %10, align 4
  %36 = load i32, ptr %6, align 4
  %37 = add nsw i32 %35, %36
  store i32 %37, ptr %13, align 4
  br label %38

38:                                               ; preds = %18
  %39 = load i32, ptr %8, align 4
  %40 = add nsw i32 %39, 1
  store i32 %40, ptr %8, align 4
  br label %15, !llvm.loop !8

41:                                               ; preds = %15
  %42 = load i32, ptr %3, align 4
  %43 = add nsw i32 %42, 18
  store i32 %43, ptr %14, align 4
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
