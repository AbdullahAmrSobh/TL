#pragma once

#include "TL/Serialization/SerializeTraits.hpp"
#include "TL/Block.hpp"

#include <nlohmann/json.hpp>

namespace TL
{
    class JsonArchive
    {
        friend void Encode(JsonArchive& archive, const char* name, Block block);
        friend void Decode(JsonArchive& archive, const char* name, Block block);
        friend void Encode(JsonArchive& archive, const char* name, bool value);
        friend void Decode(JsonArchive& archive, const char* name, bool& value);
        friend void Encode(JsonArchive& archive, const char* name, char value);
        friend void Decode(JsonArchive& archive, const char* name, char& value);
        friend void Encode(JsonArchive& archive, const char* name, uint8_t value);
        friend void Decode(JsonArchive& archive, const char* name, uint8_t& value);
        friend void Encode(JsonArchive& archive, const char* name, uint16_t value);
        friend void Decode(JsonArchive& archive, const char* name, uint16_t& value);
        friend void Encode(JsonArchive& archive, const char* name, uint32_t value);
        friend void Decode(JsonArchive& archive, const char* name, uint32_t& value);
        friend void Encode(JsonArchive& archive, const char* name, uint64_t value);
        friend void Decode(JsonArchive& archive, const char* name, uint64_t& value);
        friend void Encode(JsonArchive& archive, const char* name, int8_t value);
        friend void Decode(JsonArchive& archive, const char* name, int8_t& value);
        friend void Encode(JsonArchive& archive, const char* name, int16_t value);
        friend void Decode(JsonArchive& archive, const char* name, int16_t& value);
        friend void Encode(JsonArchive& archive, const char* name, int32_t value);
        friend void Decode(JsonArchive& archive, const char* name, int32_t& value);
        friend void Encode(JsonArchive& archive, const char* name, int64_t value);
        friend void Decode(JsonArchive& archive, const char* name, int64_t& value);
        friend void Encode(JsonArchive& archive, const char* name, float value);
        friend void Decode(JsonArchive& archive, const char* name, float& value);
        friend void Encode(JsonArchive& archive, const char* name, double value);
        friend void Decode(JsonArchive& archive, const char* name, double& value);

    public:
        template<Serializable<JsonArchive> T>
        void Encode(const T& value);

        template<Deserializable<JsonArchive> T>
        void Decode(T& value);

    private:
        nlohmann::json m_json;
    };

    // clang-format off
    inline static void Encode(JsonArchive& archive, const char* name, Block block)     {  archive.m_json[name] = value;  }
    inline static void Decode(JsonArchive& archive, const char* name, Block block)     {  value = archive.m_json[name];  }
    inline static void Encode(JsonArchive& archive, const char* name, bool value)      {  archive.m_json[name] = value;  }
    inline static void Decode(JsonArchive& archive, const char* name, bool& value)     {  value = archive.m_json[name];  }
    inline static void Encode(JsonArchive& archive, const char* name, char value)      {  archive.m_json[name] = value;  }
    inline static void Decode(JsonArchive& archive, const char* name, char value)      {  value = archive.m_json[name];  }
    inline static void Encode(JsonArchive& archive, const char* name, uint8_t value)   {  archive.m_json[name] = value;  }
    inline static void Decode(JsonArchive& archive, const char* name, uint8_t& value)  {  value = archive.m_json[name];  }
    inline static void Encode(JsonArchive& archive, const char* name, uint16_t value)  {  archive.m_json[name] = value;  }
    inline static void Decode(JsonArchive& archive, const char* name, uint16_t& value) {  value = archive.m_json[name];  }
    inline static void Encode(JsonArchive& archive, const char* name, uint32_t value)  {  archive.m_json[name] = value;  }
    inline static void Decode(JsonArchive& archive, const char* name, uint32_t& value) {  value = archive.m_json[name];  }
    inline static void Encode(JsonArchive& archive, const char* name, uint64_t value)  {  archive.m_json[name] = value;  }
    inline static void Decode(JsonArchive& archive, const char* name, uint64_t& value) {  value = archive.m_json[name];  }
    inline static void Encode(JsonArchive& archive, const char* name, int8_t value)    {  archive.m_json[name] = value;  }
    inline static void Decode(JsonArchive& archive, const char* name, int8_t& value)   {  value = archive.m_json[name];  }
    inline static void Encode(JsonArchive& archive, const char* name, int16_t value)   {  archive.m_json[name] = value;  }
    inline static void Decode(JsonArchive& archive, const char* name, int16_t& value)  {  value = archive.m_json[name];  }
    inline static void Encode(JsonArchive& archive, const char* name, int32_t value)   {  archive.m_json[name] = value;  }
    inline static void Decode(JsonArchive& archive, const char* name, int32_t& value)  {  value = archive.m_json[name];  }
    inline static void Encode(JsonArchive& archive, const char* name, int64_t value)   {  archive.m_json[name] = value;  }
    inline static void Decode(JsonArchive& archive, const char* name, int64_t& value)  {  value = archive.m_json[name];  }
    inline static void Encode(JsonArchive& archive, const char* name, float value)     {  archive.m_json[name] = value;  }
    inline static void Decode(JsonArchive& archive, const char* name, float& value)    {  value = archive.m_json[name];  }
    inline static void Encode(JsonArchive& archive, const char* name, double value)    {  archive.m_json[name] = value;  }
    inline static void Decode(JsonArchive& archive, const char* name, double& value)   {  value = archive.m_json[name];  }

    // clang-format on

} // namespace TL