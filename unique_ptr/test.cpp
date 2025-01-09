#include <gtest/gtest.h>
#include "unique.h"
#include "test_helper.h"


template <typename T>
class UniquePtrTest : public ::testing::Test 
{};

typedef ::testing::Types<int, std::string> MyTypes;

TYPED_TEST_SUITE(UniquePtrTest, MyTypes);


TYPED_TEST(UniquePtrTest, ConstructionAndDereference)
{
    UniquePtr<TypeParam> ptr(new TypeParam(TestHelper::getValue<TypeParam>()));

    EXPECT_EQ(*ptr, TestHelper::getValue<TypeParam>());
}


TYPED_TEST(UniquePtrTest, MoveConstructor)
{
    UniquePtr<TypeParam> ptr1(new TypeParam(TestHelper::getValue<TypeParam>()));
    UniquePtr<TypeParam> ptr2(std::move(ptr1));

    EXPECT_EQ(ptr1.get(), nullptr);
    EXPECT_EQ(*ptr2, TestHelper::getValue<TypeParam>());
}


TYPED_TEST(UniquePtrTest, MoveAssignment)
{
    UniquePtr<TypeParam> ptr1(new TypeParam(TestHelper::getValue<TypeParam>()));
    UniquePtr<TypeParam> ptr2(new TypeParam(TestHelper::getValue<TypeParam>()));

    ptr2 = std::move(ptr1);

    EXPECT_EQ(ptr1.get(), nullptr);
    EXPECT_EQ(*ptr2, TestHelper::getValue<TypeParam>());
}


TYPED_TEST(UniquePtrTest, Reset)
{
    UniquePtr<TypeParam> ptr(new TypeParam(TestHelper::getValue<TypeParam>()));

    EXPECT_EQ(*ptr, TestHelper::getValue<TypeParam>());

    ptr.reset();

    EXPECT_EQ(ptr.get(), nullptr);
}


TYPED_TEST(UniquePtrTest, Release)
{
    UniquePtr<TypeParam> ptr(new TypeParam(TestHelper::getValue<TypeParam>()));

    EXPECT_EQ(*ptr, TestHelper::getValue<TypeParam>());

    const TypeParam* ptr2 = ptr.release();

    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(*ptr2, TestHelper::getValue<TypeParam>());

    delete ptr2;
}


TYPED_TEST(UniquePtrTest, NotOperator)
{
    UniquePtr<TypeParam> ptr1(new TypeParam(TestHelper::getValue<TypeParam>()));
    UniquePtr<TypeParam> ptr2(nullptr);

    EXPECT_FALSE(!ptr1);
    EXPECT_TRUE(!ptr2);
}


TYPED_TEST(UniquePtrTest, Get)
{
    UniquePtr<TypeParam> ptr(new TypeParam(TestHelper::getValue<TypeParam>()));

    EXPECT_EQ(*ptr.get(), TestHelper::getValue<TypeParam>());
}


TYPED_TEST(UniquePtrTest, DereferencingNullPointer)
{
    UniquePtr<TypeParam> ptr(nullptr);

    EXPECT_EQ(ptr.get(), nullptr);
}


TYPED_TEST(UniquePtrTest, ExistingPointerAsInput)
{
    TypeParam *something = new TypeParam();
    UniquePtr<TypeParam> ptr(something);

    EXPECT_EQ(ptr.get(), something);
}


TYPED_TEST(UniquePtrTest, OperatorBool) {
    UniquePtr<TypeParam> ptr1(new TypeParam(TestHelper::getValue<TypeParam>()));
    EXPECT_TRUE(static_cast<bool>(ptr1));

    UniquePtr<int> ptr2(nullptr);
    EXPECT_FALSE(static_cast<bool>(ptr2));
}


TYPED_TEST(UniquePtrTest, MoveAssignmentRefrence)
{
    UniquePtr<TypeParam> ptr(new TypeParam(TestHelper::getValue<TypeParam>()));
    UniquePtr<TypeParam> ptr1;
    UniquePtr<TypeParam> ptr2;

    ptr2 = std::move(ptr1 = std::move(ptr));

    EXPECT_EQ(*ptr2, TestHelper::getValue<TypeParam>());
    EXPECT_EQ(ptr1.get(), nullptr);
    EXPECT_EQ(ptr.get(), nullptr);
}


TYPED_TEST(UniquePtrTest, SelfMoveAssignment)
{
    UniquePtr<TypeParam> ptr(new TypeParam(TestHelper::getValue<TypeParam>()));

    ptr = std::move(ptr);

    EXPECT_EQ(*ptr, TestHelper::getValue<TypeParam>());
}
