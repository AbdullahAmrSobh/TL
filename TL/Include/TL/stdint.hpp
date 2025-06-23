#pragma once

#include <cstdint>
#include <limits>

using B8     = bool;
using U8     = uint8_t;
using U16    = uint16_t;
using U32    = uint32_t;
using U64    = uint64_t;
using I8     = int8_t;
using I16    = int16_t;
using I32    = int32_t;
using I64    = int64_t;

static constexpr U8 U8Max = std::numeric_limits<U8>::max();
static constexpr U8 U8Min = std::numeric_limits<U8>::min();

static constexpr U16 U16Max = std::numeric_limits<U16>::max();
static constexpr U16 U16Min = std::numeric_limits<U16>::min();

static constexpr U32 U32Max = std::numeric_limits<U32>::max();
static constexpr U32 U32Min = std::numeric_limits<U32>::min();

static constexpr U64 U64Max = std::numeric_limits<U64>::max();
static constexpr U64 U64Min = std::numeric_limits<U64>::min();


static constexpr I8 I8Max = std::numeric_limits<I8>::max();
static constexpr I8 I8Min = std::numeric_limits<I8>::min();

static constexpr I16 I16Max = std::numeric_limits<I16>::max();
static constexpr I16 I16Min = std::numeric_limits<I16>::min();

static constexpr I32 I32Max = std::numeric_limits<I32>::max();
static constexpr I32 I32Min = std::numeric_limits<I32>::min();

static constexpr I64 I64Max = std::numeric_limits<I64>::max();
static constexpr I64 I64Min = std::numeric_limits<I64>::min();
