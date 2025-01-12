#include <thread>
#include <gtest/gtest.h>
#include "shared.h"
#include "test_helper.h"


template <typename T>
class SharedPtrTest : public ::testing::Test 
{};

typedef ::testing::Types<int, std::string> MyTypes;

TYPED_TEST_SUITE(SharedPtrTest, MyTypes);


TYPED_TEST(SharedPtrTest, DefaultConstructor)
{
    SharedPtr<TypeParam> ptr;
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(ptr.use_count(), 0);
}


TYPED_TEST(SharedPtrTest, ParameterizedConstructor)
{
    SharedPtr<TypeParam> ptr(new TypeParam(TestHelper::getValue<TypeParam>()));
    EXPECT_NE(ptr.get(), nullptr);
    EXPECT_EQ(*ptr, TestHelper::getValue<TypeParam>());
    EXPECT_EQ(ptr.use_count(), 1);
}


TYPED_TEST(SharedPtrTest, CopyConstructor)
{
    SharedPtr<TypeParam> ptr1(new TypeParam(TestHelper::getValue<TypeParam>()));
    SharedPtr<TypeParam> ptr2(ptr1);
    EXPECT_EQ(ptr1.get(), ptr2.get());
    EXPECT_EQ(*ptr1, *ptr2);
    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
}


TYPED_TEST(SharedPtrTest, CopyAssignment)
{
    SharedPtr<TypeParam> ptr1(new TypeParam(TestHelper::getValue<TypeParam>()));
    SharedPtr<TypeParam> ptr2;
    ptr2 = ptr1;
    EXPECT_EQ(ptr1.get(), ptr2.get());
    EXPECT_EQ(*ptr1, *ptr2);
    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
}


TYPED_TEST(SharedPtrTest, MoveConstructor)
{
    SharedPtr<TypeParam> ptr1(new TypeParam(TestHelper::getValue<TypeParam>()));
    SharedPtr<TypeParam> ptr2(std::move(ptr1));
    EXPECT_EQ(ptr1.get(), nullptr);
    EXPECT_EQ(ptr2.use_count(), 1);
    EXPECT_EQ(*ptr2, TestHelper::getValue<TypeParam>());
}


TYPED_TEST(SharedPtrTest, MoveAssignment)
{
    SharedPtr<TypeParam> ptr1(new TypeParam(TestHelper::getValue<TypeParam>()));
    SharedPtr<TypeParam> ptr2;
    ptr2 = std::move(ptr1);
    EXPECT_EQ(ptr1.get(), nullptr);
    EXPECT_EQ(ptr2.use_count(), 1);
    EXPECT_EQ(*ptr2, TestHelper::getValue<TypeParam>());
}


TYPED_TEST(SharedPtrTest, DestructorReducesCount)
{
    SharedPtr<TypeParam> ptr1(new TypeParam(TestHelper::getValue<TypeParam>()));
    {
        SharedPtr<TypeParam> ptr2 = ptr1;
        EXPECT_EQ(ptr1.use_count(), 2);
        EXPECT_EQ(ptr2.use_count(), 2);
    }
    EXPECT_EQ(*ptr1, TestHelper::getValue<TypeParam>());
    EXPECT_EQ(ptr1.use_count(), 1);
}


TYPED_TEST(SharedPtrTest, Reset)
{
    SharedPtr<TypeParam> ptr(new TypeParam(TestHelper::getValue<TypeParam>()));
    EXPECT_NE(ptr.get(), nullptr);
    EXPECT_EQ(ptr.use_count(), 1);
    ptr.reset();
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(ptr.use_count(), 0);
}


TYPED_TEST(SharedPtrTest, UseCount)
{
    SharedPtr<TypeParam> ptr1(new TypeParam(TestHelper::getValue<TypeParam>()));
    SharedPtr<TypeParam> ptr2(ptr1);
    SharedPtr<TypeParam> ptr3 = ptr2;

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


TYPED_TEST(SharedPtrTest, BoolOperator)
{
    SharedPtr<TypeParam> ptr1;
    EXPECT_FALSE(ptr1);

    SharedPtr<TypeParam> ptr2(new TypeParam(TestHelper::getValue<TypeParam>()));
    EXPECT_TRUE(ptr2);
}


TYPED_TEST(SharedPtrTest, DereferenceOperators)
{
    SharedPtr<TypeParam> ptr(new TypeParam(TestHelper::getValue<TypeParam>()));
    EXPECT_EQ(*ptr, TestHelper::getValue<TypeParam>());
    *ptr = TestHelper::getValue<TypeParam>();
    EXPECT_EQ(*ptr, TestHelper::getValue<TypeParam>());
}


TYPED_TEST(SharedPtrTest, NotOperator)
{
    SharedPtr<TypeParam> ptr1;
    EXPECT_TRUE(!ptr1);

    SharedPtr<TypeParam> ptr2(new TypeParam(TestHelper::getValue<TypeParam>()));
    EXPECT_FALSE(!ptr2);
}


TYPED_TEST(SharedPtrTest, CopyAssignmentRefrence)
{
    SharedPtr<TypeParam> ptr(new TypeParam(TestHelper::getValue<TypeParam>()));
    SharedPtr<TypeParam> ptr1;
    SharedPtr<TypeParam> ptr2;

    ptr2 = ptr1 = ptr;

    EXPECT_EQ(*ptr, TestHelper::getValue<TypeParam>());
    EXPECT_EQ(*ptr1, TestHelper::getValue<TypeParam>());
    EXPECT_EQ(*ptr2, TestHelper::getValue<TypeParam>());
}


TYPED_TEST(SharedPtrTest, MoveAssignmentRefrence1)
{
    SharedPtr<TypeParam> ptr(new TypeParam(TestHelper::getValue<TypeParam>()));
    SharedPtr<TypeParam> ptr1;
    SharedPtr<TypeParam> ptr2;

    ptr2 = std::move(ptr1 = std::move(ptr));

    EXPECT_EQ(*ptr2, TestHelper::getValue<TypeParam>());

    EXPECT_EQ(ptr.use_count(), 0);
    EXPECT_EQ(ptr1.use_count(), 0);
    EXPECT_EQ(ptr2.use_count(), 1);
}


TYPED_TEST(SharedPtrTest, MoveAssignmentRefrence2)
{
    SharedPtr<TypeParam> ptr(new TypeParam(TestHelper::getValue<TypeParam>()));
    SharedPtr<TypeParam> ptr1;
    SharedPtr<TypeParam> ptr2;

    ptr2 = std::move(ptr1) = std::move(ptr);

    EXPECT_EQ(*ptr2, TestHelper::getValue<TypeParam>());

    EXPECT_EQ(ptr.use_count(), 0);
    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
}


TYPED_TEST(SharedPtrTest, SelfCopyAssignment)
{
    SharedPtr<TypeParam> ptr(new TypeParam(TestHelper::getValue<TypeParam>()));

    ptr = ptr;

    EXPECT_EQ(*ptr,  TestHelper::getValue<TypeParam>());
}


TYPED_TEST(SharedPtrTest, SelfMoveAssignment)
{
    SharedPtr<TypeParam> ptr(new TypeParam(TestHelper::getValue<TypeParam>()));

    ptr = std::move(ptr);

    EXPECT_EQ(*ptr,  TestHelper::getValue<TypeParam>());
}


template <typename T>
void thread_func_copy(SharedPtr<T> ptr, int copies) {
    for (int i = 0; i < copies; ++i) {
        SharedPtr<T> local_ptr = ptr;
    }
}


TYPED_TEST(SharedPtrTest, ThreadSafty)
{
    SharedPtr<TypeParam> ptr(new TypeParam(TestHelper::getValue<TypeParam>()));
    const int num_copies1 = 100000;
    const int num_copies2 = 500000;

    std::thread thread1(thread_func_copy<TypeParam>, ptr, num_copies1);
    std::thread thread2(thread_func_copy<TypeParam>, ptr, num_copies2);

    thread1.join();
    thread2.join();

    EXPECT_EQ(ptr.use_count(), 1);
}
