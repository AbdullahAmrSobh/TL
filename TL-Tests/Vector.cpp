#include <gtest/gtest.h>

#include <TL/Containers.hpp>

using namespace TL;

// Test fixture for Vector tests
class VectorTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Basic functionality tests
TEST_F(VectorTest, DefaultConstructor)
{
    Vector<int> vec;
    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
}

TEST_F(VectorTest, SizeConstructor)
{
    Vector<int> vec(5);
    EXPECT_FALSE(vec.empty());
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec.capacity(), 5);
    for (size_t i = 0; i < 5; ++i)
    {
        EXPECT_EQ(vec[i], 0);
    }
}

TEST_F(VectorTest, SizeValueConstructor)
{
    Vector<int> vec(3, 42);
    EXPECT_FALSE(vec.empty());
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec.capacity(), 3);
    for (size_t i = 0; i < 3; ++i)
    {
        EXPECT_EQ(vec[i], 42);
    }
}

TEST_F(VectorTest, InitializerListConstructor)
{
    Vector<int> vec = {1, 2, 3, 4, 5};
    EXPECT_FALSE(vec.empty());
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec.capacity(), 5);
    for (size_t i = 0; i < 5; ++i)
    {
        EXPECT_EQ(vec[i], static_cast<int>(i + 1));
    }
}

TEST_F(VectorTest, CopyConstructor)
{
    Vector<int> original = {1, 2, 3};
    Vector<int> copy(original);
    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.capacity(), original.capacity());
    for (size_t i = 0; i < original.size(); ++i)
    {
        EXPECT_EQ(copy[i], original[i]);
    }
}

TEST_F(VectorTest, MoveConstructor)
{
    Vector<int> original = {1, 2, 3};
    Vector<int> moved(std::move(original));
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.capacity(), 3);
    EXPECT_TRUE(original.empty());
    EXPECT_EQ(original.capacity(), 0);
}

TEST_F(VectorTest, RangeConstructor)
{
    std::vector<int> source = {10, 20, 30};
    Vector<int> vec(source.begin(), source.end());
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);
}

// Element access tests
TEST_F(VectorTest, AtAccess)
{
    Vector<int> vec = {1, 2, 3};
    EXPECT_EQ(vec.at(0), 1);
    EXPECT_EQ(vec.at(1), 2);
    EXPECT_EQ(vec.at(2), 3);
    EXPECT_THROW(vec.at(3), std::exception);
}

TEST_F(VectorTest, OperatorBracketAccess)
{
    Vector<int> vec = {1, 2, 3};
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

TEST_F(VectorTest, FrontBackAccess)
{
    Vector<int> vec = {1, 2, 3};
    EXPECT_EQ(vec.front(), 1);
    EXPECT_EQ(vec.back(), 3);
}

TEST_F(VectorTest, DataAccess)
{
    Vector<int> vec = {1, 2, 3};
    int* data = vec.data();
    EXPECT_EQ(data[0], 1);
    EXPECT_EQ(data[1], 2);
    EXPECT_EQ(data[2], 3);
}

// Iterator tests
TEST_F(VectorTest, IteratorAccess)
{
    Vector<int> vec = {1, 2, 3};
    auto it = vec.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(it, vec.end());
}

TEST_F(VectorTest, ConstIteratorAccess)
{
    const Vector<int> vec = {1, 2, 3};
    auto it = vec.cbegin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(it, vec.cend());
}

TEST_F(VectorTest, ReverseIteratorAccess)
{
    Vector<int> vec = {1, 2, 3};
    auto it = vec.rbegin();
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(it, vec.rend());
}

TEST_F(VectorTest, RangeBasedForLoop)
{
    Vector<int> vec = {1, 2, 3, 4, 5};
    int sum = 0;
    for (const auto& val : vec)
    {
        sum += val;
    }
    EXPECT_EQ(sum, 15);
}

// Capacity tests
TEST_F(VectorTest, Reserve)
{
    Vector<int> vec;
    vec.reserve(10);
    EXPECT_EQ(vec.capacity(), 10);
    EXPECT_TRUE(vec.empty());
}

TEST_F(VectorTest, ShrinkToFit)
{
    Vector<int> vec = {1, 2, 3};
    vec.reserve(10);
    EXPECT_EQ(vec.capacity(), 10);
    vec.shrink_to_fit();
    EXPECT_EQ(vec.capacity(), 3);
}

// Modifier tests
TEST_F(VectorTest, PushBack)
{
    Vector<int> vec;
    vec.push_back(1);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], 1);

    vec.push_back(2);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[1], 2);
}

TEST_F(VectorTest, EmplaceBack)
{
    Vector<std::string> vec;
    vec.emplace_back("hello");
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], "hello");

    vec.emplace_back(3, 'x');
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[1], "xxx");
}

TEST_F(VectorTest, PopBack)
{
    Vector<int> vec = {1, 2, 3};
    vec.pop_back();
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.back(), 2);
}

TEST_F(VectorTest, Resize)
{
    Vector<int> vec = {1, 2, 3};
    vec.resize(5);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[3], 0);
    EXPECT_EQ(vec[4], 0);

    vec.resize(2);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
}

TEST_F(VectorTest, ResizeWithValue)
{
    Vector<int> vec = {1, 2, 3};
    vec.resize(5, 42);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[3], 42);
    EXPECT_EQ(vec[4], 42);
}

TEST_F(VectorTest, Clear)
{
    Vector<int> vec = {1, 2, 3};
    vec.clear();
    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
}

TEST_F(VectorTest, Insert)
{
    Vector<int> vec = {1, 3};
    auto it = vec.insert(vec.begin() + 1, 2);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
    EXPECT_EQ(*it, 2);
}

TEST_F(VectorTest, InsertMultiple)
{
    Vector<int> vec = {1, 4};
    auto it = vec.insert(vec.begin() + 1, 2, 2);
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 2);
    EXPECT_EQ(vec[3], 4);
}

TEST_F(VectorTest, InsertRange)
{
    Vector<int> vec = {1, 4};
    std::vector<int> to_insert = {2, 3};
    auto it = vec.insert(vec.begin() + 1, to_insert.begin(), to_insert.end());
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
    EXPECT_EQ(vec[3], 4);
}

TEST_F(VectorTest, Emplace)
{
    Vector<std::string> vec = {"hello", "world"};
    auto it = vec.emplace(vec.begin() + 1, 3, 'x');
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], "hello");
    EXPECT_EQ(vec[1], "xxx");
    EXPECT_EQ(vec[2], "world");
    EXPECT_EQ(*it, "xxx");
}

TEST_F(VectorTest, Erase)
{
    Vector<int> vec = {1, 2, 3, 4};
    auto it = vec.erase(vec.begin() + 1);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 3);
    EXPECT_EQ(vec[2], 4);
    EXPECT_EQ(*it, 3);
}

TEST_F(VectorTest, EraseRange)
{
    Vector<int> vec = {1, 2, 3, 4, 5};
    auto it = vec.erase(vec.begin() + 1, vec.begin() + 4);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 5);
    EXPECT_EQ(*it, 5);
}

TEST_F(VectorTest, Assign)
{
    Vector<int> vec = {1, 2, 3};
    vec.assign(2, 42);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], 42);
    EXPECT_EQ(vec[1], 42);
}

TEST_F(VectorTest, AssignRange)
{
    Vector<int> vec = {1, 2, 3};
    std::vector<int> source = {10, 20, 30, 40};
    vec.assign(source.begin(), source.end());
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);
    EXPECT_EQ(vec[3], 40);
}

TEST_F(VectorTest, AssignInitializerList)
{
    Vector<int> vec = {1, 2, 3};
    vec.assign({100, 200, 300, 400});
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec[0], 100);
    EXPECT_EQ(vec[1], 200);
    EXPECT_EQ(vec[2], 300);
    EXPECT_EQ(vec[3], 400);
}

// Assignment operator tests
TEST_F(VectorTest, CopyAssignment)
{
    Vector<int> original = {1, 2, 3};
    Vector<int> copy;
    copy = original;
    EXPECT_EQ(copy.size(), original.size());
    for (size_t i = 0; i < original.size(); ++i)
    {
        EXPECT_EQ(copy[i], original[i]);
    }
}

TEST_F(VectorTest, MoveAssignment)
{
    Vector<int> moved;
    {
        Vector<int> original = {1, 2, 3};
        moved = std::move(original);
    }
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[1], 2);
    EXPECT_EQ(moved[2], 3);
}

TEST_F(VectorTest, InitializerListAssignment)
{
    Vector<int> vec;
    vec = {1, 2, 3, 4, 5};
    EXPECT_EQ(vec.size(), 5);
    for (size_t i = 0; i < 5; ++i)
    {
        EXPECT_EQ(vec[i], static_cast<int>(i + 1));
    }
}

// Swap tests
// TEST_F(VectorTest, Swap)
// {
//     Vector<int> vec1 = {1, 2, 3};
//     Vector<int> vec2 = {4, 5, 6, 7};

//     vec1.swap(vec2);

//     EXPECT_EQ(vec1.size(), 4);
//     EXPECT_EQ(vec2.size(), 3);
//     EXPECT_EQ(vec1[0], 4);
//     EXPECT_EQ(vec2[0], 1);
// }

// Comparison operator tests
TEST_F(VectorTest, EqualityOperators)
{
    Vector<int> vec1 = {1, 2, 3};
    Vector<int> vec2 = {1, 2, 3};
    Vector<int> vec3 = {1, 2, 4};

    EXPECT_TRUE(vec1 == vec2);
    EXPECT_FALSE(vec1 == vec3);
    EXPECT_TRUE(vec1 != vec3);
    EXPECT_FALSE(vec1 != vec2);
}

TEST_F(VectorTest, ComparisonOperators)
{
    Vector<int> vec1 = {1, 2, 3};
    Vector<int> vec2 = {1, 2, 4};
    Vector<int> vec3 = {1, 2, 3, 4};

    EXPECT_TRUE(vec1 < vec2);
    EXPECT_TRUE(vec1 < vec3);
    EXPECT_TRUE(vec2 > vec1);
    EXPECT_TRUE(vec3 > vec1);
    EXPECT_TRUE(vec1 <= vec2);
    EXPECT_TRUE(vec1 <= vec1);
    EXPECT_TRUE(vec2 >= vec1);
    EXPECT_TRUE(vec1 >= vec1);
}

// Span conversion tests
TEST_F(VectorTest, SpanConversion)
{
    Vector<int> vec = {1, 2, 3, 4, 5};

    // Test as_span()
    Span<int> span(vec.data(), vec.size());
    EXPECT_EQ(span.size(), vec.size());
    EXPECT_EQ(span[0], vec[0]);
    EXPECT_EQ(span[4], vec[4]);

    // Test subspan with range
    Span<int> sub_span(vec.data() + 1, 3);
    EXPECT_EQ(sub_span.size(), 3);
    EXPECT_EQ(sub_span[0], 2);
    EXPECT_EQ(sub_span[1], 3);
    EXPECT_EQ(sub_span[2], 4);

    // Test implicit conversion
    Span<int> implicit_span = vec;
    EXPECT_EQ(implicit_span.size(), vec.size());
}

// TEST_F(VectorTest, SpanConstructor)
// {
//     Vector<int> source = {1, 2, 3, 4, 5};
//     auto span = source.as_span();

//     Vector<int> vec_from_span(span);
//     EXPECT_EQ(vec_from_span.size(), source.size());
//     for (size_t i = 0; i < source.size(); ++i)
//     {
//         EXPECT_EQ(vec_from_span[i], source[i]);
//     }
// }

// Growth and capacity tests
TEST_F(VectorTest, GrowthBehavior)
{
    Vector<int> vec;
    size_t initial_capacity = vec.capacity();

    // Add elements and check growth
    for (int i = 0; i < 100; ++i)
    {
        vec.push_back(i);
        EXPECT_EQ(vec.size(), static_cast<size_t>(i + 1));
        EXPECT_GE(vec.capacity(), vec.size());
    }

    // Verify all elements are correct
    for (int i = 0; i < 100; ++i)
    {
        EXPECT_EQ(vec[i], i);
    }
}

TEST_F(VectorTest, ReserveGrowth)
{
    Vector<int> vec;
    vec.reserve(100);
    EXPECT_EQ(vec.capacity(), 100);

    // Adding elements shouldn't trigger reallocation
    for (int i = 0; i < 100; ++i)
    {
        vec.push_back(i);
        EXPECT_EQ(vec.capacity(), 100);
    }
}

// Edge cases and error conditions
TEST_F(VectorTest, EmptyVectorOperations)
{
    Vector<int> vec;

    // These should not crash
    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);

    // Pop back on empty vector should assert
    // EXPECT_DEATH(vec.pop_back(), "");
}

TEST_F(VectorTest, OutOfBoundsAccess)
{
    Vector<int> vec = {1, 2, 3};

    // Accessing out of bounds should assert
    // EXPECT_DEATH(vec.at(3), "");
    // EXPECT_DEATH(vec[3], "");
}

// Allocator tests
TEST_F(VectorTest, CustomAllocator)
{
    // Test with default allocator
    Vector<int> vec1 = {1, 2, 3};
    EXPECT_EQ(vec1.size(), 3);

    // Test with custom allocator (if different from default)
    // This would require a custom allocator implementation
}

// Performance and memory tests
TEST_F(VectorTest, MemoryEfficiency)
{
    Vector<int> vec;
    vec.reserve(1000);

    // Add elements and check that capacity doesn't grow unnecessarily
    for (int i = 0; i < 1000; ++i)
    {
        vec.push_back(i);
        if (i < 1000)
        {
            EXPECT_EQ(vec.capacity(), 1000);
        }
    }
}

// // Non-member function tests
// TEST_F(VectorTest, NonMemberSwap)
// {
//     Vector<int> vec1 = {1, 2, 3};
//     Vector<int> vec2 = {4, 5, 6};

//     swap(vec1, vec2);

//     EXPECT_EQ(vec1[0], 4);
//     EXPECT_EQ(vec2[0], 1);
// }

// String vector tests
TEST_F(VectorTest, StringVector)
{
    Vector<std::string> vec = {"hello", "world", "test"};
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], "hello");
    EXPECT_EQ(vec[1], "world");
    EXPECT_EQ(vec[2], "test");

    vec.push_back("new");
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec[3], "new");
}

// Complex type tests
TEST_F(VectorTest, ComplexType)
{
    struct TestStruct
    {
        int x, y;
        std::string name;

        bool operator==(const TestStruct& other) const
        {
            return x == other.x && y == other.y && name == other.name;
        }
    };

    Vector<TestStruct> vec;
    vec.push_back({1, 2, "first"});
    vec.push_back({3, 4, "second"});

    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0].x, 1);
    EXPECT_EQ(vec[0].y, 2);
    EXPECT_EQ(vec[0].name, "first");
    EXPECT_EQ(vec[1].x, 3);
    EXPECT_EQ(vec[1].y, 4);
    EXPECT_EQ(vec[1].name, "second");
}

// Move semantics tests
TEST_F(VectorTest, MoveSemantics)
{
    Vector<std::string> vec1 = {"hello", "world"};
    Vector<std::string> vec2 = std::move(vec1);

    EXPECT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec2[0], "hello");
    EXPECT_EQ(vec2[1], "world");
    EXPECT_TRUE(vec1.empty());
}

// Copy elision tests
TEST_F(VectorTest, CopyElision)
{
    Vector<int> vec = Vector<int>{1, 2, 3, 4, 5};
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[4], 5);
}

// Exception safety tests
TEST_F(VectorTest, ExceptionSafety)
{
    Vector<int> vec = {1, 2, 3, 4, 5};

    // Test that vector remains in valid state after operations
    try
    {
        vec.push_back(6);
        EXPECT_EQ(vec.size(), 6);
        EXPECT_EQ(vec[5], 6);
    }
    catch (...)
    {
        // Vector should still be in valid state
        EXPECT_GE(vec.size(), 5);
    }
}