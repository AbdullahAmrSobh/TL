#include <gtest/gtest.h>

#include <TL/Bits.hpp>

#include <cstdint>

// Test fixture for Vector tests
class BitsTest : public ::testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(BitsTest, ByteSwap)
{
    uint8_t  u8  = 0xF0;
    uint16_t u16 = 0xF0F0;
    uint32_t u32 = 0xF0F0B0B0;
    uint64_t u64 = 0xF0F0B0B0ABCABC;

    int8_t   i8  = -0x10;
    int16_t  i16 = -0x1010;
    int32_t  i32 = -0x1010B0B0;
    int64_t  i64 = -0x1010B0B0ABCABC;

    float    f = 123.456f;
    double   d = 789.123;

// Macro to check if bits are swapped correctly
#define CHECK_BITSWAP(TYPE, VALUE)                                   \
    do                                                               \
    {                                                                \
        TYPE orig     = (VALUE);                                     \
        TYPE swapped  = TL::ByteSwap(orig);                          \
        TYPE expected = 0;                                           \
        for (size_t i = 0; i < sizeof(TYPE) * 8; ++i)                \
        {                                                            \
            if ((orig >> i) & 1)                                     \
                expected |= (TYPE(1) << (sizeof(TYPE) * 8 - 1 - i)); \
        }                                                            \
        EXPECT_EQ(swapped, expected);                                \
    } while (0)

// Macro for floating point types
#define CHECK_BITSWAP_FLOAT(TYPE, VALUE)                                        \
    do                                                                          \
    {                                                                           \
        TYPE orig  = (VALUE);                                                   \
        using UInt = std::conditional_t<sizeof(TYPE) == 4, uint32_t, uint64_t>; \
        UInt orig_bits;                                                         \
        memcpy(&orig_bits, &orig, sizeof(TYPE));                                \
        UInt swapped_bits = TL::ByteSwap(orig_bits);                            \
        UInt expected     = 0;                                                  \
        for (size_t i = 0; i < sizeof(TYPE) * 8; ++i)                           \
        {                                                                       \
            if ((orig_bits >> i) & 1)                                           \
                expected |= (UInt(1) << (sizeof(TYPE) * 8 - 1 - i));            \
        }                                                                       \
        EXPECT_EQ(swapped_bits, expected);                                      \
    } while (0)

    // Check using macro for unsigned
    CHECK_BITSWAP(uint8_t, u8);
    CHECK_BITSWAP(uint16_t, u16);
    CHECK_BITSWAP(uint32_t, u32);
    CHECK_BITSWAP(uint64_t, u64);

    // Check using macro for signed
    CHECK_BITSWAP(int8_t, i8);
    CHECK_BITSWAP(int16_t, i16);
    CHECK_BITSWAP(int32_t, i32);
    CHECK_BITSWAP(int64_t, i64);

    // Check using macro for float/double
    CHECK_BITSWAP_FLOAT(float, f);
    CHECK_BITSWAP_FLOAT(double, d);

#undef CHECK_BITSWAP
#undef CHECK_BITSWAP_FLOAT
}