; ModuleID = 'test/LoopAnnidati.m2r.ll'
source_filename = "loopannidati.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%"class.std::ios_base::Init" = type { i8 }

@_ZStL8__ioinit = internal global %"class.std::ios_base::Init" zeroinitializer, align 1
@__dso_handle = external hidden global i8
@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 65535, ptr @_GLOBAL__sub_I_loopannidati.cpp, ptr null }]

; Function Attrs: noinline uwtable
define internal void @__cxx_global_var_init() #0 section ".text.startup" {
  call void @_ZNSt8ios_base4InitC1Ev(ptr noundef nonnull align 1 dereferenceable(1) @_ZStL8__ioinit)
  %1 = call i32 @__cxa_atexit(ptr @_ZNSt8ios_base4InitD1Ev, ptr @_ZStL8__ioinit, ptr @__dso_handle) #3
  ret void
}

declare void @_ZNSt8ios_base4InitC1Ev(ptr noundef nonnull align 1 dereferenceable(1)) unnamed_addr #1

; Function Attrs: nounwind
declare void @_ZNSt8ios_base4InitD1Ev(ptr noundef nonnull align 1 dereferenceable(1)) unnamed_addr #2

; Function Attrs: nounwind
declare i32 @__cxa_atexit(ptr, ptr, ptr) #3

; Function Attrs: mustprogress noinline norecurse nounwind uwtable
define dso_local noundef i32 @main() #4 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca double, align 8
  %4 = alloca double, align 8
  %5 = alloca i32, align 4
  %6 = alloca double, align 8
  %7 = alloca i32, align 4
  %8 = alloca double, align 8
  %9 = alloca double, align 8
  %10 = alloca i32, align 4
  %11 = alloca double, align 8
  %12 = alloca double, align 8
  %13 = alloca double, align 8
  store i32 0, ptr %1, align 4
  store i32 200, ptr %2, align 4
  store double 1.000000e+01, ptr %3, align 8
  store double 2.000000e+01, ptr %4, align 8
  store i32 0, ptr %5, align 4
  br label %14

14:                                               ; preds = %59, %0
  %15 = load i32, ptr %5, align 4
  %16 = icmp slt i32 %15, 200
  br i1 %16, label %17, label %62

17:                                               ; preds = %14
  %18 = load double, ptr %3, align 8
  %19 = load double, ptr %4, align 8
  %20 = fadd double %18, %19
  store double %20, ptr %6, align 8
  store i32 0, ptr %7, align 4
  br label %21

21:                                               ; preds = %55, %17
  %22 = load i32, ptr %7, align 4
  %23 = icmp slt i32 %22, 200
  br i1 %23, label %24, label %58

24:                                               ; preds = %21
  %25 = load double, ptr %3, align 8
  %26 = load i32, ptr %5, align 4
  %27 = sitofp i32 %26 to double
  %28 = fadd double %25, %27
  %29 = call double @sqrt(double noundef %28) #3
  store double %29, ptr %8, align 8
  %30 = load double, ptr %3, align 8
  %31 = fmul double %30, 2.000000e+00
  store double %31, ptr %9, align 8
  store i32 0, ptr %10, align 4
  br label %32

32:                                               ; preds = %51, %24
  %33 = load i32, ptr %10, align 4
  %34 = icmp slt i32 %33, 200
  br i1 %34, label %35, label %54

35:                                               ; preds = %32
  %36 = load double, ptr %4, align 8
  %37 = load i32, ptr %7, align 4
  %38 = sitofp i32 %37 to double
  %39 = fadd double %36, %38
  %40 = call double @sqrt(double noundef %39) #3
  store double %40, ptr %11, align 8
  %41 = load double, ptr %6, align 8
  %42 = load i32, ptr %10, align 4
  %43 = sitofp i32 %42 to double
  %44 = fadd double %41, %43
  %45 = call double @sqrt(double noundef %44) #3
  store double %45, ptr %12, align 8
  %46 = load double, ptr %8, align 8
  %47 = load double, ptr %11, align 8
  %48 = fadd double %46, %47
  %49 = load double, ptr %12, align 8
  %50 = fadd double %48, %49
  store double %50, ptr %13, align 8
  br label %51

51:                                               ; preds = %35
  %52 = load i32, ptr %10, align 4
  %53 = add nsw i32 %52, 1
  store i32 %53, ptr %10, align 4
  br label %32, !llvm.loop !6

54:                                               ; preds = %32
  br label %55

55:                                               ; preds = %54
  %56 = load i32, ptr %7, align 4
  %57 = add nsw i32 %56, 1
  store i32 %57, ptr %7, align 4
  br label %21, !llvm.loop !8

58:                                               ; preds = %21
  br label %59

59:                                               ; preds = %58
  %60 = load i32, ptr %5, align 4
  %61 = add nsw i32 %60, 1
  store i32 %61, ptr %5, align 4
  br label %14, !llvm.loop !9

62:                                               ; preds = %14
  %63 = load i32, ptr %1, align 4
  ret i32 %63
}

; Function Attrs: nounwind
declare double @sqrt(double noundef) #2

; Function Attrs: noinline uwtable
define internal void @_GLOBAL__sub_I_loopannidati.cpp() #0 section ".text.startup" {
  call void @__cxx_global_var_init()
  ret void
}

attributes #0 = { noinline uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind }
attributes #4 = { mustprogress noinline norecurse nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Debian clang version 14.0.6"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
