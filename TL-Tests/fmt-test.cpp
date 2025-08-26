#include <gtest/gtest.h>

#include <TL/Fmt.hpp>

#include <cstdint>

class FmtTest : public ::testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(FmtTest, FormatInteger)
{
    TL::String result = std::format("Value: {}", 42);
    EXPECT_EQ(result, "Value: 42");
}

TEST_F(FmtTest, FormatMultipleArguments)
{
    TL::String result = std::format("{} + {} = {}", 2, 3, 5);
    EXPECT_EQ(result, "2 + 3 = 5");
}

TEST_F(FmtTest, FormatString)
{
    TL::String name = "Alice";
    TL::String result = std::format("Hello, {}!", name);
    EXPECT_EQ(result, "Hello, Alice!");
}

TEST_F(FmtTest, FormatHex)
{
    int value = 255;
    TL::String result = std::format("Hex: {:#x}", value);
    EXPECT_EQ(result, "Hex: 0xff");
}

TEST_F(FmtTest, FormatFloat)
{
    double pi = 3.14159;
    TL::String result = std::format("Pi: {:.2f}", pi);
    EXPECT_EQ(result, "Pi: 3.14");
}

TEST_F(FmtTest, FormatWithPadding)
{
    int value = 7;
    TL::String result = std::format("Padded: {:04}", value);
    EXPECT_EQ(result, "Padded: 0007");
}

TEST_F(FmtTest, FormatNegativeNumber)
{
    int value = -123;
    TL::String result = std::format("Neg: {}", value);
    EXPECT_EQ(result, "Neg: -123");
}

TEST_F(FmtTest, FormatUnsigned)
{
    uint32_t value = 1234567890;
    TL::String result = std::format("Unsigned: {}", value);
    EXPECT_EQ(result, "Unsigned: 1234567890");
}

TEST_F(FmtTest, FormatChar)
{
    char c = 'A';
    TL::String result = std::format("Char: {}", c);
    EXPECT_EQ(result, "Char: A");
}

TEST_F(FmtTest, FormatEscapedBraces)
{
    TL::String result = std::format("{{}}");
    EXPECT_EQ(result, "{}");
}

TEST_F(FmtTest, AsciiString)
{
    TL::String world = "World";
    TL::StringView worldView = world;

    TL::String result0 = std::format("Hello, {}!", "World");
    EXPECT_EQ(result0, "Hello, World!");
    TL::String result1 = std::format("Hello, {}!", world);
    EXPECT_EQ(result1, "Hello, World!");
    TL::String result2 = std::format("Hello, {}!", worldView);
    EXPECT_EQ(result2, "Hello, World!");
}

// TEST_F(FmtTest, WString)
// {
//     TL::WString world = L"عالم";
//     TL::WStringView worldView = world;

//     TL::WString result0 = std::format(L"مرحبا يا {}!", L"عالم");
//     EXPECT_EQ(result0, L"!مرحبا يا عالم");
//     TL::WString result1 = std::format(L"مرحبا يا {}!", world);
//     EXPECT_EQ(result1, L"!مرحبا يا عالم");
//     TL::WString result2 = std::format(L"مرحبا يا {}!", worldView);
//     EXPECT_EQ(result2, L"!مرحبا يا عالم");
// }

// TODO: Add other test cases.