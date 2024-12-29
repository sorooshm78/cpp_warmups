#include <gtest/gtest.h>
#include "shared.h"


TEST(SharedPtrTest, DefaultConstructor)
{
    SharedPtr<int> ptr;
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(ptr.use_count(), 0);
}


TEST(SharedPtrTest, ParameterizedConstructor)
{
    SharedPtr<int> ptr(new int(42));
    EXPECT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr, 42);
    EXPECT_EQ(ptr.use_count(), 1);
}

TEST(SharedPtrTest, CopyConstructor)
{
    SharedPtr<int> ptr1(new int(42));
    SharedPtr<int> ptr2(ptr1);
    EXPECT_EQ(ptr1.get(), ptr2.get());
    EXPECT_EQ(*ptr1, *ptr2);
    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
}

TEST(SharedPtrTest, CopyAssignment)
{
    SharedPtr<int> ptr1(new int(42));
    SharedPtr<int> ptr2;
    ptr2 = ptr1;
    EXPECT_EQ(ptr1.get(), ptr2.get());
    EXPECT_EQ(*ptr1, *ptr2);
    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
}

TEST(SharedPtrTest, MoveConstructor)
{
    SharedPtr<int> ptr1(new int(42));
    SharedPtr<int> ptr2(std::move(ptr1));
    EXPECT_EQ(ptr1.get(), nullptr);
    EXPECT_EQ(ptr2.use_count(), 1);
    EXPECT_EQ(*ptr2, 42);
}


TEST(SharedPtrTest, MoveAssignment)
{
    SharedPtr<int> ptr1(new int(42));
    SharedPtr<int> ptr2;
    ptr2 = std::move(ptr1);
    EXPECT_EQ(ptr1.get(), nullptr);
    EXPECT_EQ(ptr2.use_count(), 1);
    EXPECT_EQ(*ptr2, 42);
}


TEST(SharedPtrTest, DestructorReducesCount)
{
    SharedPtr<int> ptr1(new int(42));
    {
        SharedPtr<int> ptr2 = ptr1;
        EXPECT_EQ(ptr1.use_count(), 2);
        EXPECT_EQ(ptr2.use_count(), 2);
    }
    EXPECT_EQ(*ptr1, 42);
    EXPECT_EQ(ptr1.use_count(), 1);
}


TEST(SharedPtrTest, Reset)
{
    SharedPtr<int> ptr(new int(42));
    EXPECT_NE(ptr.get(), nullptr);
    EXPECT_EQ(ptr.use_count(), 1);
    ptr.reset();
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(ptr.use_count(), 0);
}


TEST(SharedPtrTest, UseCount)
{
    SharedPtr<int> ptr1(new int(42));
    SharedPtr<int> ptr2(ptr1);
    SharedPtr<int> ptr3 = ptr2;

    EXPECT_EQ(ptr1.use_count(), 3);
    EXPECT_EQ(ptr2.use_count(), 3);
    EXPECT_EQ(ptr3.use_count(), 3);

    ptr3.reset();
    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
    EXPECT_EQ(ptr3.use_count(), 0);

    ptr2.reset();
    EXPECT_EQ(ptr1.use_count(), 1);
    EXPECT_EQ(ptr2.use_count(), 0);
    EXPECT_EQ(ptr3.use_count(), 0);
}


TEST(SharedPtrTest, BoolOperator)
{
    SharedPtr<int> ptr1;
    EXPECT_FALSE(ptr1);

    SharedPtr<int> ptr2(new int(42));
    EXPECT_TRUE(ptr2);
}


TEST(SharedPtrTest, DereferenceOperators)
{
    SharedPtr<int> ptr(new int(42));
    EXPECT_EQ(*ptr, 42);
    *ptr = 24;
    EXPECT_EQ(*ptr, 24);
}


TEST(SharedPtrTest, NotOperator)
{
    SharedPtr<int> ptr1;
    EXPECT_TRUE(!ptr1);

    SharedPtr<int> ptr2(new int(42));
    EXPECT_FALSE(!ptr2);
}


TEST(SharedPtrTest, CopyAssignmentRefrence)
{
    SharedPtr<int> ptr(new int(42));
    SharedPtr<int> ptr1;
    SharedPtr<int> ptr2;

    ptr2 = ptr1 = ptr;

    EXPECT_EQ(*ptr, 42);
    EXPECT_EQ(*ptr1, 42);
    EXPECT_EQ(*ptr2, 42);
}

TEST(SharedPtrTest, MoveAssignmentRefrence)
{
    SharedPtr<int> ptr(new int(30));
    SharedPtr<int> ptr1;
    SharedPtr<int> ptr2;

    ptr2 = std::move(ptr1) = std::move(ptr);

    EXPECT_EQ(*ptr2, 30);

    EXPECT_EQ(ptr.use_count(), 0);
    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
}

