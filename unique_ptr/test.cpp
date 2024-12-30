#include <gtest/gtest.h>
#include "unique.h"


class Something
{
public:
    int returnZero()
    {
        return 0;
    }
};


TEST(UniquePtrTest, ConstructionAndDereference)
{
    UniquePtr<int> ptr(new int(10));

    EXPECT_EQ(*ptr, 10);
}


TEST(UniquePtrTest, MoveConstructor)
{
    UniquePtr<int> ptr1(new int(20));
    UniquePtr<int> ptr2(std::move(ptr1));

    EXPECT_EQ(ptr1.get(), nullptr);
    EXPECT_EQ(*ptr2, 20);
}


TEST(UniquePtrTest, MoveAssignment)
{
    UniquePtr<int> ptr1(new int(30));
    UniquePtr<int> ptr2(new int(40));

    ptr2 = std::move(ptr1);

    EXPECT_EQ(ptr1.get(), nullptr);
    EXPECT_EQ(*ptr2, 30);
}


TEST(UniquePtrTest, Reset)
{
    UniquePtr<int> ptr(new int(50));

    EXPECT_EQ(*ptr, 50);

    ptr.reset();

    EXPECT_EQ(ptr.get(), nullptr);
}


TEST(UniquePtrTest, Release)
{
    UniquePtr<int> ptr(new int(50));

    EXPECT_EQ(*ptr, 50);

    const int* ptr2 = ptr.release();

    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(*ptr2, 50);

    delete ptr2;
}


TEST(UniquePtrTest, NotOperator)
{
    UniquePtr<int> ptr1(new int(60));
    UniquePtr<int> ptr2(nullptr);

    EXPECT_FALSE(!ptr1);
    EXPECT_TRUE(!ptr2);
}


TEST(UniquePtrTest, Get)
{
    UniquePtr<int> ptr(new int(70));

    EXPECT_EQ(*ptr.get(), 70);
}

TEST(UniquePtrTest, CallFunctionClass)
{
    UniquePtr<Something> ptr(new Something);

    EXPECT_EQ(ptr.get()->returnZero(), 0);
}


TEST(UniquePtrTest, DereferencingNullPointer)
{
    UniquePtr<Something> ptr(nullptr);

    EXPECT_EQ(ptr.get(), nullptr);
}


TEST(UniquePtrTest, ExistingPointerAsInput)
{
    Something *something = new Something();
    UniquePtr<Something> ptr(something);

    EXPECT_EQ(ptr.get(), something);
}


TEST(UniquePtrTest, OperatorBool) {
    UniquePtr<int> ptr1(new int(42));
    EXPECT_TRUE(static_cast<bool>(ptr1));

    UniquePtr<int> ptr2(nullptr);
    EXPECT_FALSE(static_cast<bool>(ptr2));
}


TEST(UniquePtrTest, MoveAssignmentRefrence)
{
    UniquePtr<int> ptr(new int(30));
    UniquePtr<int> ptr1;
    UniquePtr<int> ptr2;

    ptr2 = std::move(ptr1 = std::move(ptr));

    EXPECT_EQ(*ptr2, 30);
    EXPECT_EQ(ptr1.get(), nullptr);
    EXPECT_EQ(ptr.get(), nullptr);
}
