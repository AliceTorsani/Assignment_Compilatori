; ModuleID = 'test/tripCount.m2r.ll'
source_filename = "test/tripCount.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z6sameTCPiS_(ptr noundef %0, ptr noundef %1) #0 {
  br label %3

3:                                                ; preds = %10, %2
  %.01 = phi i32 [ 0, %2 ], [ %11, %10 ]
  %4 = icmp slt i32 %.01, 15
  br i1 %4, label %5, label %12

5:                                                ; preds = %3
  %6 = sext i32 %.01 to i64
  %7 = getelementptr inbounds i32, ptr %0, i64 %6
  %8 = load i32, ptr %7, align 4
  %9 = add nsw i32 %8, 1
  store i32 %9, ptr %7, align 4
  br label %10

10:                                               ; preds = %5
  %11 = add nsw i32 %.01, 1
  br label %3, !llvm.loop !6

12:                                               ; preds = %3
  br label %13

13:                                               ; preds = %20, %12
  %.0 = phi i32 [ 0, %12 ], [ %21, %20 ]
  %14 = icmp slt i32 %.0, 15
  br i1 %14, label %15, label %22

15:                                               ; preds = %13
  %16 = sext i32 %.0 to i64
  %17 = getelementptr inbounds i32, ptr %1, i64 %16
  %18 = load i32, ptr %17, align 4
  %19 = sub nsw i32 %18, 1
  store i32 %19, ptr %17, align 4
  br label %20

20:                                               ; preds = %15
  %21 = add nsw i32 %.0, 1
  br label %13, !llvm.loop !8

22:                                               ; preds = %13
  ret void
}

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z10sameTC_refPiS_i(ptr noundef %0, ptr noundef %1, i32 noundef %2) #0 {
  br label %4

4:                                                ; preds = %11, %3
  %.01 = phi i32 [ 0, %3 ], [ %12, %11 ]
  %5 = icmp slt i32 %.01, %2
  br i1 %5, label %6, label %13

6:                                                ; preds = %4
  %7 = sext i32 %.01 to i64
  %8 = getelementptr inbounds i32, ptr %0, i64 %7
  %9 = load i32, ptr %8, align 4
  %10 = add nsw i32 %9, 1
  store i32 %10, ptr %8, align 4
  br label %11

11:                                               ; preds = %6
  %12 = add nsw i32 %.01, 1
  br label %4, !llvm.loop !9

13:                                               ; preds = %4
  br label %14

14:                                               ; preds = %21, %13
  %.0 = phi i32 [ 0, %13 ], [ %22, %21 ]
  %15 = icmp slt i32 %.0, %2
  br i1 %15, label %16, label %23

16:                                               ; preds = %14
  %17 = sext i32 %.0 to i64
  %18 = getelementptr inbounds i32, ptr %1, i64 %17
  %19 = load i32, ptr %18, align 4
  %20 = sub nsw i32 %19, 1
  store i32 %20, ptr %18, align 4
  br label %21

21:                                               ; preds = %16
  %22 = add nsw i32 %.0, 1
  br label %14, !llvm.loop !10

23:                                               ; preds = %14
  ret void
}

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z15sameTC_ref_diffPiS_i(ptr noundef %0, ptr noundef %1, i32 noundef %2) #0 {
  %4 = sub nsw i32 %2, 5
  br label %5

5:                                                ; preds = %12, %3
  %.01 = phi i32 [ 0, %3 ], [ %13, %12 ]
  %6 = icmp slt i32 %.01, %2
  br i1 %6, label %7, label %14

7:                                                ; preds = %5
  %8 = sext i32 %.01 to i64
  %9 = getelementptr inbounds i32, ptr %0, i64 %8
  %10 = load i32, ptr %9, align 4
  %11 = add nsw i32 %10, 1
  store i32 %11, ptr %9, align 4
  br label %12

12:                                               ; preds = %7
  %13 = add nsw i32 %.01, 1
  br label %5, !llvm.loop !11

14:                                               ; preds = %5
  br label %15

15:                                               ; preds = %22, %14
  %.0 = phi i32 [ 0, %14 ], [ %23, %22 ]
  %16 = icmp slt i32 %.0, %4
  br i1 %16, label %17, label %24

17:                                               ; preds = %15
  %18 = sext i32 %.0 to i64
  %19 = getelementptr inbounds i32, ptr %1, i64 %18
  %20 = load i32, ptr %19, align 4
  %21 = sub nsw i32 %20, 1
  store i32 %21, ptr %19, align 4
  br label %22

22:                                               ; preds = %17
  %23 = add nsw i32 %.0, 1
  br label %15, !llvm.loop !12

24:                                               ; preds = %15
  ret void
}

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z13sameTC_staticPiS_(ptr noundef %0, ptr noundef %1) #0 {
  br label %3

3:                                                ; preds = %10, %2
  %.01 = phi i32 [ 0, %2 ], [ %11, %10 ]
  %4 = icmp slt i32 %.01, 5
  br i1 %4, label %5, label %12

5:                                                ; preds = %3
  %6 = sext i32 %.01 to i64
  %7 = getelementptr inbounds i32, ptr %0, i64 %6
  %8 = load i32, ptr %7, align 4
  %9 = add nsw i32 %8, 1
  store i32 %9, ptr %7, align 4
  br label %10

10:                                               ; preds = %5
  %11 = add nsw i32 %.01, 1
  br label %3, !llvm.loop !13

12:                                               ; preds = %3
  br label %13

13:                                               ; preds = %20, %12
  %.0 = phi i32 [ 0, %12 ], [ %21, %20 ]
  %14 = icmp slt i32 %.0, 5
  br i1 %14, label %15, label %22

15:                                               ; preds = %13
  %16 = sext i32 %.0 to i64
  %17 = getelementptr inbounds i32, ptr %1, i64 %16
  %18 = load i32, ptr %17, align 4
  %19 = sub nsw i32 %18, 1
  store i32 %19, ptr %17, align 4
  br label %20

20:                                               ; preds = %15
  %21 = add nsw i32 %.0, 1
  br label %13, !llvm.loop !14

22:                                               ; preds = %13
  ret void
}

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z18sameTC_static_diffPiS_(ptr noundef %0, ptr noundef %1) #0 {
  br label %3

3:                                                ; preds = %10, %2
  %.01 = phi i32 [ 0, %2 ], [ %11, %10 ]
  %4 = icmp slt i32 %.01, 5
  br i1 %4, label %5, label %12

5:                                                ; preds = %3
  %6 = sext i32 %.01 to i64
  %7 = getelementptr inbounds i32, ptr %0, i64 %6
  %8 = load i32, ptr %7, align 4
  %9 = add nsw i32 %8, 1
  store i32 %9, ptr %7, align 4
  br label %10

10:                                               ; preds = %5
  %11 = add nsw i32 %.01, 1
  br label %3, !llvm.loop !15

12:                                               ; preds = %3
  br label %13

13:                                               ; preds = %20, %12
  %.0 = phi i32 [ 0, %12 ], [ %21, %20 ]
  %14 = icmp slt i32 %.0, 1
  br i1 %14, label %15, label %22

15:                                               ; preds = %13
  %16 = sext i32 %.0 to i64
  %17 = getelementptr inbounds i32, ptr %1, i64 %16
  %18 = load i32, ptr %17, align 4
  %19 = sub nsw i32 %18, 1
  store i32 %19, ptr %17, align 4
  br label %20

20:                                               ; preds = %15
  %21 = add nsw i32 %.0, 1
  br label %13, !llvm.loop !16

22:                                               ; preds = %13
  ret void
}

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z12sameTC_equalPiS_ii(ptr noundef %0, ptr noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  %5 = icmp eq i32 %2, %3
  br i1 %5, label %6, label %27

6:                                                ; preds = %4
  br label %7

7:                                                ; preds = %14, %6
  %.01 = phi i32 [ 0, %6 ], [ %15, %14 ]
  %8 = icmp slt i32 %.01, %2
  br i1 %8, label %9, label %16

9:                                                ; preds = %7
  %10 = sext i32 %.01 to i64
  %11 = getelementptr inbounds i32, ptr %0, i64 %10
  %12 = load i32, ptr %11, align 4
  %13 = add nsw i32 %12, 1
  store i32 %13, ptr %11, align 4
  br label %14

14:                                               ; preds = %9
  %15 = add nsw i32 %.01, 1
  br label %7, !llvm.loop !17

16:                                               ; preds = %7
  br label %17

17:                                               ; preds = %24, %16
  %.0 = phi i32 [ 0, %16 ], [ %25, %24 ]
  %18 = icmp slt i32 %.0, %3
  br i1 %18, label %19, label %26

19:                                               ; preds = %17
  %20 = sext i32 %.0 to i64
  %21 = getelementptr inbounds i32, ptr %1, i64 %20
  %22 = load i32, ptr %21, align 4
  %23 = sub nsw i32 %22, 1
  store i32 %23, ptr %21, align 4
  br label %24

24:                                               ; preds = %19
  %25 = add nsw i32 %.0, 1
  br label %17, !llvm.loop !18

26:                                               ; preds = %17
  br label %27

27:                                               ; preds = %26, %4
  ret void
}

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z15sameTC_modifiedPiS_ii(ptr noundef %0, ptr noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  br label %5

5:                                                ; preds = %12, %4
  %.01 = phi i32 [ 0, %4 ], [ %13, %12 ]
  %6 = icmp slt i32 %.01, %3
  br i1 %6, label %7, label %14

7:                                                ; preds = %5
  %8 = sext i32 %.01 to i64
  %9 = getelementptr inbounds i32, ptr %0, i64 %8
  %10 = load i32, ptr %9, align 4
  %11 = add nsw i32 %10, 1
  store i32 %11, ptr %9, align 4
  br label %12

12:                                               ; preds = %7
  %13 = add nsw i32 %.01, 1
  br label %5, !llvm.loop !19

14:                                               ; preds = %5
  br label %15

15:                                               ; preds = %22, %14
  %.0 = phi i32 [ 0, %14 ], [ %23, %22 ]
  %16 = icmp slt i32 %.0, %3
  br i1 %16, label %17, label %24

17:                                               ; preds = %15
  %18 = sext i32 %.0 to i64
  %19 = getelementptr inbounds i32, ptr %1, i64 %18
  %20 = load i32, ptr %19, align 4
  %21 = sub nsw i32 %20, 1
  store i32 %21, ptr %19, align 4
  br label %22

22:                                               ; preds = %17
  %23 = add nsw i32 %.0, 1
  br label %15, !llvm.loop !20

24:                                               ; preds = %15
  ret void
}

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z15sameTC_assignedPiS_ii(ptr noundef %0, ptr noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  br label %5

5:                                                ; preds = %12, %4
  %.01 = phi i32 [ 0, %4 ], [ %13, %12 ]
  %6 = icmp slt i32 %.01, 15
  br i1 %6, label %7, label %14

7:                                                ; preds = %5
  %8 = sext i32 %.01 to i64
  %9 = getelementptr inbounds i32, ptr %0, i64 %8
  %10 = load i32, ptr %9, align 4
  %11 = add nsw i32 %10, 1
  store i32 %11, ptr %9, align 4
  br label %12

12:                                               ; preds = %7
  %13 = add nsw i32 %.01, 1
  br label %5, !llvm.loop !21

14:                                               ; preds = %5
  br label %15

15:                                               ; preds = %22, %14
  %.0 = phi i32 [ 0, %14 ], [ %23, %22 ]
  %16 = icmp slt i32 %.0, 20
  br i1 %16, label %17, label %24

17:                                               ; preds = %15
  %18 = sext i32 %.0 to i64
  %19 = getelementptr inbounds i32, ptr %1, i64 %18
  %20 = load i32, ptr %19, align 4
  %21 = sub nsw i32 %20, 1
  store i32 %21, ptr %19, align 4
  br label %22

22:                                               ; preds = %17
  %23 = add nsw i32 %.0, 1
  br label %15, !llvm.loop !22

24:                                               ; preds = %15
  ret void
}

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z20sameTC_assignedEqualPiS_ii(ptr noundef %0, ptr noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  br label %5

5:                                                ; preds = %12, %4
  %.01 = phi i32 [ 0, %4 ], [ %13, %12 ]
  %6 = icmp slt i32 %.01, 15
  br i1 %6, label %7, label %14

7:                                                ; preds = %5
  %8 = sext i32 %.01 to i64
  %9 = getelementptr inbounds i32, ptr %0, i64 %8
  %10 = load i32, ptr %9, align 4
  %11 = add nsw i32 %10, 1
  store i32 %11, ptr %9, align 4
  br label %12

12:                                               ; preds = %7
  %13 = add nsw i32 %.01, 1
  br label %5, !llvm.loop !23

14:                                               ; preds = %5
  br label %15

15:                                               ; preds = %22, %14
  %.0 = phi i32 [ 0, %14 ], [ %23, %22 ]
  %16 = icmp slt i32 %.0, 15
  br i1 %16, label %17, label %24

17:                                               ; preds = %15
  %18 = sext i32 %.0 to i64
  %19 = getelementptr inbounds i32, ptr %1, i64 %18
  %20 = load i32, ptr %19, align 4
  %21 = sub nsw i32 %20, 1
  store i32 %21, ptr %19, align 4
  br label %22

22:                                               ; preds = %17
  %23 = add nsw i32 %.0, 1
  br label %15, !llvm.loop !24

24:                                               ; preds = %15
  ret void
}

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z15sameTC_equalIn2PiS_ii(ptr noundef %0, ptr noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  %5 = icmp sle i32 %2, %3
  br i1 %5, label %6, label %30

6:                                                ; preds = %4
  %7 = icmp sge i32 %2, %3
  br i1 %7, label %8, label %29

8:                                                ; preds = %6
  br label %9

9:                                                ; preds = %16, %8
  %.01 = phi i32 [ 0, %8 ], [ %17, %16 ]
  %10 = icmp slt i32 %.01, %2
  br i1 %10, label %11, label %18

11:                                               ; preds = %9
  %12 = sext i32 %.01 to i64
  %13 = getelementptr inbounds i32, ptr %0, i64 %12
  %14 = load i32, ptr %13, align 4
  %15 = add nsw i32 %14, 1
  store i32 %15, ptr %13, align 4
  br label %16

16:                                               ; preds = %11
  %17 = add nsw i32 %.01, 1
  br label %9, !llvm.loop !25

18:                                               ; preds = %9
  br label %19

19:                                               ; preds = %26, %18
  %.0 = phi i32 [ 0, %18 ], [ %27, %26 ]
  %20 = icmp slt i32 %.0, %3
  br i1 %20, label %21, label %28

21:                                               ; preds = %19
  %22 = sext i32 %.0 to i64
  %23 = getelementptr inbounds i32, ptr %1, i64 %22
  %24 = load i32, ptr %23, align 4
  %25 = sub nsw i32 %24, 1
  store i32 %25, ptr %23, align 4
  br label %26

26:                                               ; preds = %21
  %27 = add nsw i32 %.0, 1
  br label %19, !llvm.loop !26

28:                                               ; preds = %19
  br label %29

29:                                               ; preds = %28, %6
  br label %30

30:                                               ; preds = %29, %4
  ret void
}

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z21sameTC_differentStartPiS_ii(ptr noundef %0, ptr noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  br label %5

5:                                                ; preds = %12, %4
  %.01 = phi i32 [ 7, %4 ], [ %13, %12 ]
  %6 = icmp slt i32 %.01, 15
  br i1 %6, label %7, label %14

7:                                                ; preds = %5
  %8 = sext i32 %.01 to i64
  %9 = getelementptr inbounds i32, ptr %0, i64 %8
  %10 = load i32, ptr %9, align 4
  %11 = add nsw i32 %10, 1
  store i32 %11, ptr %9, align 4
  br label %12

12:                                               ; preds = %7
  %13 = add nsw i32 %.01, 1
  br label %5, !llvm.loop !27

14:                                               ; preds = %5
  br label %15

15:                                               ; preds = %22, %14
  %.0 = phi i32 [ 0, %14 ], [ %23, %22 ]
  %16 = icmp slt i32 %.0, 15
  br i1 %16, label %17, label %24

17:                                               ; preds = %15
  %18 = sext i32 %.0 to i64
  %19 = getelementptr inbounds i32, ptr %1, i64 %18
  %20 = load i32, ptr %19, align 4
  %21 = sub nsw i32 %20, 1
  store i32 %21, ptr %19, align 4
  br label %22

22:                                               ; preds = %17
  %23 = add nsw i32 %.0, 1
  br label %15, !llvm.loop !28

24:                                               ; preds = %15
  ret void
}

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z22sameTC_differentUpdatePiS_ii(ptr noundef %0, ptr noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  br label %5

5:                                                ; preds = %12, %4
  %.01 = phi i32 [ 0, %4 ], [ %13, %12 ]
  %6 = icmp slt i32 %.01, 15
  br i1 %6, label %7, label %14

7:                                                ; preds = %5
  %8 = sext i32 %.01 to i64
  %9 = getelementptr inbounds i32, ptr %0, i64 %8
  %10 = load i32, ptr %9, align 4
  %11 = add nsw i32 %10, 1
  store i32 %11, ptr %9, align 4
  br label %12

12:                                               ; preds = %7
  %13 = add nsw i32 %.01, 1
  br label %5, !llvm.loop !29

14:                                               ; preds = %5
  br label %15

15:                                               ; preds = %23, %14
  %.0 = phi i32 [ 0, %14 ], [ %24, %23 ]
  %16 = icmp slt i32 %.0, 15
  br i1 %16, label %17, label %25

17:                                               ; preds = %15
  %18 = add nsw i32 %.0, 1
  %19 = sext i32 %18 to i64
  %20 = getelementptr inbounds i32, ptr %1, i64 %19
  %21 = load i32, ptr %20, align 4
  %22 = sub nsw i32 %21, 1
  store i32 %22, ptr %20, align 4
  br label %23

23:                                               ; preds = %17
  %24 = add nsw i32 %18, 1
  br label %15, !llvm.loop !30

25:                                               ; preds = %15
  ret void
}

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z29sameTC_differentTripDirectionPiS_ii(ptr noundef %0, ptr noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  br label %5

5:                                                ; preds = %12, %4
  %.01 = phi i32 [ 0, %4 ], [ %13, %12 ]
  %6 = icmp slt i32 %.01, 15
  br i1 %6, label %7, label %14

7:                                                ; preds = %5
  %8 = sext i32 %.01 to i64
  %9 = getelementptr inbounds i32, ptr %0, i64 %8
  %10 = load i32, ptr %9, align 4
  %11 = add nsw i32 %10, 1
  store i32 %11, ptr %9, align 4
  br label %12

12:                                               ; preds = %7
  %13 = add nsw i32 %.01, 1
  br label %5, !llvm.loop !31

14:                                               ; preds = %5
  br label %15

15:                                               ; preds = %22, %14
  %.0 = phi i32 [ 15, %14 ], [ %23, %22 ]
  %16 = icmp sgt i32 %.0, 0
  br i1 %16, label %17, label %24

17:                                               ; preds = %15
  %18 = sext i32 %.0 to i64
  %19 = getelementptr inbounds i32, ptr %1, i64 %18
  %20 = load i32, ptr %19, align 4
  %21 = sub nsw i32 %20, 1
  store i32 %21, ptr %19, align 4
  br label %22

22:                                               ; preds = %17
  %23 = add nsw i32 %.0, -1
  br label %15, !llvm.loop !32

24:                                               ; preds = %15
  ret void
}

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z16sameTC_doWAndForPiS_ii(ptr noundef %0, ptr noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  br label %5

5:                                                ; preds = %12, %4
  %.01 = phi i32 [ 0, %4 ], [ %13, %12 ]
  %6 = icmp slt i32 %.01, 15
  br i1 %6, label %7, label %14

7:                                                ; preds = %5
  %8 = sext i32 %.01 to i64
  %9 = getelementptr inbounds i32, ptr %0, i64 %8
  %10 = load i32, ptr %9, align 4
  %11 = add nsw i32 %10, 1
  store i32 %11, ptr %9, align 4
  br label %12

12:                                               ; preds = %7
  %13 = add nsw i32 %.01, 1
  br label %5, !llvm.loop !33

14:                                               ; preds = %5
  %15 = icmp sgt i32 15, 0
  br i1 %15, label %16, label %26

16:                                               ; preds = %14
  br label %17

17:                                               ; preds = %23, %16
  %.0 = phi i32 [ 0, %16 ], [ %22, %23 ]
  %18 = sext i32 %.0 to i64
  %19 = getelementptr inbounds i32, ptr %1, i64 %18
  %20 = load i32, ptr %19, align 4
  %21 = sub nsw i32 %20, 1
  store i32 %21, ptr %19, align 4
  %22 = add nsw i32 %.0, 1
  br label %23

23:                                               ; preds = %17
  %24 = icmp slt i32 %22, 15
  br i1 %24, label %17, label %25, !llvm.loop !34

25:                                               ; preds = %23
  br label %26

26:                                               ; preds = %25, %14
  ret void
}

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z15sameTC_infinitePiS_ii(ptr noundef %0, ptr noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  br label %5

5:                                                ; preds = %5, %4
  %6 = getelementptr inbounds i32, ptr %0, i64 0
  %7 = load i32, ptr %6, align 4
  %8 = add nsw i32 %7, 1
  store i32 %8, ptr %6, align 4
  br label %5, !llvm.loop !35
}

; Function Attrs: mustprogress noinline norecurse nounwind uwtable
define dso_local noundef i32 @main() #1 {
  %1 = alloca [15 x i32], align 16
  %2 = alloca [15 x i32], align 16
  %3 = getelementptr inbounds [15 x i32], ptr %1, i64 0, i64 0
  %4 = getelementptr inbounds [15 x i32], ptr %2, i64 0, i64 0
  call void @_Z6sameTCPiS_(ptr noundef %3, ptr noundef %4)
  %5 = getelementptr inbounds [15 x i32], ptr %1, i64 0, i64 0
  %6 = getelementptr inbounds [15 x i32], ptr %2, i64 0, i64 0
  call void @_Z10sameTC_refPiS_i(ptr noundef %5, ptr noundef %6, i32 noundef 12)
  %7 = getelementptr inbounds [15 x i32], ptr %1, i64 0, i64 0
  %8 = getelementptr inbounds [15 x i32], ptr %2, i64 0, i64 0
  call void @_Z15sameTC_ref_diffPiS_i(ptr noundef %7, ptr noundef %8, i32 noundef 12)
  %9 = getelementptr inbounds [15 x i32], ptr %1, i64 0, i64 0
  %10 = getelementptr inbounds [15 x i32], ptr %2, i64 0, i64 0
  call void @_Z13sameTC_staticPiS_(ptr noundef %9, ptr noundef %10)
  %11 = getelementptr inbounds [15 x i32], ptr %1, i64 0, i64 0
  %12 = getelementptr inbounds [15 x i32], ptr %2, i64 0, i64 0
  call void @_Z18sameTC_static_diffPiS_(ptr noundef %11, ptr noundef %12)
  %13 = getelementptr inbounds [15 x i32], ptr %1, i64 0, i64 0
  %14 = getelementptr inbounds [15 x i32], ptr %2, i64 0, i64 0
  call void @_Z12sameTC_equalPiS_ii(ptr noundef %13, ptr noundef %14, i32 noundef 12, i32 noundef 15)
  %15 = getelementptr inbounds [15 x i32], ptr %1, i64 0, i64 0
  %16 = getelementptr inbounds [15 x i32], ptr %2, i64 0, i64 0
  call void @_Z12sameTC_equalPiS_ii(ptr noundef %15, ptr noundef %16, i32 noundef 12, i32 noundef 12)
  %17 = getelementptr inbounds [15 x i32], ptr %1, i64 0, i64 0
  %18 = getelementptr inbounds [15 x i32], ptr %2, i64 0, i64 0
  call void @_Z15sameTC_modifiedPiS_ii(ptr noundef %17, ptr noundef %18, i32 noundef 12, i32 noundef 15)
  %19 = getelementptr inbounds [15 x i32], ptr %1, i64 0, i64 0
  %20 = getelementptr inbounds [15 x i32], ptr %2, i64 0, i64 0
  call void @_Z15sameTC_assignedPiS_ii(ptr noundef %19, ptr noundef %20, i32 noundef 12, i32 noundef 15)
  %21 = getelementptr inbounds [15 x i32], ptr %1, i64 0, i64 0
  %22 = getelementptr inbounds [15 x i32], ptr %2, i64 0, i64 0
  call void @_Z20sameTC_assignedEqualPiS_ii(ptr noundef %21, ptr noundef %22, i32 noundef 10, i32 noundef 10)
  %23 = getelementptr inbounds [15 x i32], ptr %1, i64 0, i64 0
  %24 = getelementptr inbounds [15 x i32], ptr %2, i64 0, i64 0
  call void @_Z15sameTC_equalIn2PiS_ii(ptr noundef %23, ptr noundef %24, i32 noundef 14, i32 noundef 16)
  ret i32 0
}

attributes #0 = { mustprogress noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress noinline norecurse nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

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
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
!13 = distinct !{!13, !7}
!14 = distinct !{!14, !7}
!15 = distinct !{!15, !7}
!16 = distinct !{!16, !7}
!17 = distinct !{!17, !7}
!18 = distinct !{!18, !7}
!19 = distinct !{!19, !7}
!20 = distinct !{!20, !7}
!21 = distinct !{!21, !7}
!22 = distinct !{!22, !7}
!23 = distinct !{!23, !7}
!24 = distinct !{!24, !7}
!25 = distinct !{!25, !7}
!26 = distinct !{!26, !7}
!27 = distinct !{!27, !7}
!28 = distinct !{!28, !7}
!29 = distinct !{!29, !7}
!30 = distinct !{!30, !7}
!31 = distinct !{!31, !7}
!32 = distinct !{!32, !7}
!33 = distinct !{!33, !7}
!34 = distinct !{!34, !7}
!35 = distinct !{!35, !7}
