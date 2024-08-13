#pragma once

#include "Binary.hpp"
#include "Json.hpp"

namespace TL
{
    template<typename T>
    inline static void Process(Encoder& encoder, const T& value) { value.Serialize(encoder); }

    template<typename T>
    inline static void Process(Encoder& encoder, const T* values, size_t count)
    {
        for (size_t i = 0; i < count; i++)
        {
            Process(encoder, values[i]);
        }
    }

    template<typename T>
    inline static void Process(Decoder& decoder, T& value)
    {
        value.Deserialize(decoder);
    }

    template<typename T>
    inline static void Process(Decoder& decoder, T* values, size_t count)
    {
        for (size_t i = 0; i < count; i++)
        {
            Process(decoder, values[i]);
        }
    }

#define TL_DEFINE_ENCODER(type, code)                                               \
    template<>                                                                      \
    inline void Encoder<x>(BinaryEncoder & archive, const x& value) code template<> \
    inline void Encoder<x>(JsonEncoder & archive, const x& value) code

#define TL_DEFINE_DECODER(type, code)                                               \
    template<>                                                                      \
    inline void Decoder<x>(BinaryEncoder & archive, const x& value) code template<> \
    inline void Decoder<x>(JsonEncoder & archive, const x& value) code

    TL_DEFINE_ENCODER(bool,     { archive.Write(value); });
    TL_DEFINE_ENCODER(char,     { archive.Write(value); });
    TL_DEFINE_ENCODER(uint8_t,  { archive.Write(value); });
    TL_DEFINE_ENCODER(uint16_t, { archive.Write(value); });
    TL_DEFINE_ENCODER(uint32_t, { archive.Write(value); });
    TL_DEFINE_ENCODER(uint64_t, { archive.Write(value); });
    TL_DEFINE_ENCODER(int8_t,   { archive.Write(value); });
    TL_DEFINE_ENCODER(int16_t,  { archive.Write(value); });
    TL_DEFINE_ENCODER(int32_t,  { archive.Write(value); });
    TL_DEFINE_ENCODER(int64_t,  { archive.Write(value); });
    TL_DEFINE_ENCODER(float,    { archive.Write(value); });
    TL_DEFINE_ENCODER(double,   { archive.Write(value); });

    TL_DEFINE_DECODER(bool,     { archive.Read(value); });
    TL_DEFINE_DECODER(char,     { archive.Read(value); });
    TL_DEFINE_DECODER(uint8_t,  { archive.Read(value); });
    TL_DEFINE_DECODER(uint16_t, { archive.Read(value); });
    TL_DEFINE_DECODER(uint32_t, { archive.Read(value); });
    TL_DEFINE_DECODER(uint64_t, { archive.Read(value); });
    TL_DEFINE_DECODER(int8_t,   { archive.Read(value); });
    TL_DEFINE_DECODER(int16_t,  { archive.Read(value); });
    TL_DEFINE_DECODER(int32_t,  { archive.Read(value); });
    TL_DEFINE_DECODER(int64_t,  { archive.Read(value); });
    TL_DEFINE_DECODER(float,    { archive.Read(value); });
    TL_DEFINE_DECODER(double,   { archive.Read(value); });

    // clang-format on

} // namespace TL