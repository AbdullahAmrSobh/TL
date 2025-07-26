#pragma once
#include "TL/Block.hpp"
#include "TL/Serialization/SerializeTraits.hpp"
#include "TL/Flags.hpp"
#include "TL/Containers.hpp"

#include <vector>
#include <unordered_map>
#include <iostream>
#include <optional>
#include <filesystem>
#include <fstream>
#include <string>

#include <nlohmann/json.hpp>

namespace TL
{
    class JsonArchive
    {
    public:
        JsonArchive() = default;

        // Constructor for reading from a stream
        JsonArchive(std::istream& stream)
        {
            stream >> m_json;
        }

        // Constructor for writing to a stream
        JsonArchive(std::ostream& stream, bool dummy)
            : m_output_stream(&stream)
        {
        }

        template<Serializable<JsonArchive> T>
        void Encode(const char* name, const T& value)
        {
            // For serializable objects, call their Serialize method
            // We'll store them as nested objects in the JSON
            JsonArchive nested_archive;
            value.Serialize(nested_archive);
            m_json[name] = nested_archive.m_json;
        }

        template<Deserializable<JsonArchive> T>
        void Decode(const char* name, T& value)
        {
            // For deserializable objects, call their Deserialize method
            if (m_json.contains(name))
            {
                JsonArchive nested_archive;
                nested_archive.m_json = m_json[name];
                value.Deserialize(nested_archive);
            }
        }

        // Write the JSON to the output stream
        void WriteToStream()
        {
            if (m_output_stream)
            {
                *m_output_stream << m_json.dump(4); // Pretty print with 4 spaces
            }
        }

        // Read JSON from input stream
        void ReadFromStream(std::istream& stream)
        {
            stream >> m_json;
        }

        // Direct access to the underlying JSON object
        nlohmann::json& GetJson() { return m_json; }
        const nlohmann::json& GetJson() const { return m_json; }

        // Static methods for file operations
        template<typename T>
        static void Save(const T& object, const char* path)
        {
            std::ofstream file{path};
            if (file.is_open())
            {
                TL::JsonArchive archive{file, true};
                archive.Encode("root", object);
                archive.WriteToStream();
            }
        }

        template<typename T>
        static T Load(const char* path)
        {
            T object{};
            std::ifstream file{path};
            if (file.is_open())
            {
                TL::JsonArchive archive{file};
                archive.Decode("root", object);
            }
            return object;
        }

    private:
        nlohmann::json m_json;
        std::ostream* m_output_stream = nullptr;
    };

    // Macro to generate encode/decode functions for basic types
    #define TL_JSON_BASIC_TYPE(TYPE) \
        inline static void Encode(JsonArchive& archive, const char* name, TYPE value) \
        { \
            archive.GetJson()[name] = value; \
        } \
        inline static void Decode(JsonArchive& archive, const char* name, TYPE& value) \
        { \
            if (archive.GetJson().contains(name)) \
            { \
                value = archive.GetJson()[name]; \
            } \
        }

    // Macro to generate encode/decode functions for basic types that need special handling
    #define TL_JSON_SPECIAL_TYPE(TYPE, ENCODE_EXPR, DECODE_EXPR) \
        inline static void Encode(JsonArchive& archive, const char* name, TYPE value) \
        { \
            archive.GetJson()[name] = ENCODE_EXPR; \
        } \
        inline static void Decode(JsonArchive& archive, const char* name, TYPE& value) \
        { \
            if (archive.GetJson().contains(name)) \
            { \
                DECODE_EXPR; \
            } \
        }

    // Specialized encode/decode functions for basic types
    inline static void Encode(JsonArchive& archive, const char* name, Block block)
    {
        // For blocks, we'll store as base64 encoded string
        // This is a simplified version - you might want more sophisticated handling
        String base64_data = "base64_encoded_data"; // Placeholder
        archive.GetJson()[name] = base64_data;
    }

    inline static void Decode(JsonArchive& archive, const char* name, Block block)
    {
        // Decode from base64 string back to block
        if (archive.GetJson().contains(name))
        {
            // Placeholder implementation
            String base64_data = archive.GetJson()[name];
            // Convert base64 back to block data
        }
    }

    // Generate encode/decode functions for all basic types
    TL_JSON_BASIC_TYPE(bool)
    TL_JSON_BASIC_TYPE(uint8_t)
    TL_JSON_BASIC_TYPE(uint16_t)
    TL_JSON_BASIC_TYPE(uint32_t)
    TL_JSON_BASIC_TYPE(uint64_t)
    TL_JSON_BASIC_TYPE(int8_t)
    TL_JSON_BASIC_TYPE(int16_t)
    TL_JSON_BASIC_TYPE(int32_t)
    TL_JSON_BASIC_TYPE(int64_t)
    TL_JSON_BASIC_TYPE(float)
    TL_JSON_BASIC_TYPE(double)
    TL_JSON_BASIC_TYPE(unsigned long)

    // Special handling for char (convert to/from string)
    TL_JSON_SPECIAL_TYPE(char, String(1, value),
        String str = archive.GetJson()[name];
        if (!str.empty()) { value = str[0]; })

    // String serialization - prefer TL::String, but support std::string too
    TL_JSON_BASIC_TYPE(String)
    TL_JSON_BASIC_TYPE(std::string)

    // Add std::vector support through template specialization
    template<typename T>
    inline static void Encode(JsonArchive& archive, const char* name, const std::vector<T>& value)
    {
        nlohmann::json array = nlohmann::json::array();
        for (const auto& item : value)
        {
            JsonArchive item_archive;
            Encode(item_archive, "item", item);
            array.push_back(item_archive.GetJson()["item"]);
        }
        archive.GetJson()[name] = array;
    }

    template<typename T>
    inline static void Decode(JsonArchive& archive, const char* name, std::vector<T>& value)
    {
        if (archive.GetJson().contains(name) && archive.GetJson()[name].is_array())
        {
            value.clear();
            for (const auto& item : archive.GetJson()[name])
            {
                T decoded_item{};
                JsonArchive item_archive;
                item_archive.GetJson()["item"] = item;
                Decode(item_archive, "item", decoded_item);
                value.push_back(decoded_item);
            }
        }
    }

    // Vector serialization - prefer TL::Vector, but support std::vector too
    template<typename T>
    inline static void Encode(JsonArchive& archive, const char* name, const Vector<T>& value)
    {
        nlohmann::json array = nlohmann::json::array();
        for (const auto& item : value)
        {
            JsonArchive item_archive;
            Encode(item_archive, "item", item);
            array.push_back(item_archive.GetJson()["item"]);
        }
        archive.GetJson()[name] = array;
    }

    template<typename T>
    inline static void Decode(JsonArchive& archive, const char* name, Vector<T>& value)
    {
        if (archive.GetJson().contains(name) && archive.GetJson()[name].is_array())
        {
            value.clear();
            for (const auto& item : archive.GetJson()[name])
            {
                T decoded_item{};
                JsonArchive item_archive;
                item_archive.GetJson()["item"] = item;
                Decode(item_archive, "item", decoded_item);
                value.push_back(decoded_item);
            }
        }
    }

    // Map serialization - prefer TL::Map, but support std::unordered_map too
    template<typename Key, typename Value>
    inline static void Encode(JsonArchive& archive, const char* name, const Map<Key, Value>& value)
    {
        nlohmann::json object = nlohmann::json::object();
        for (const auto& [key, val] : value)
        {
            JsonArchive key_archive, val_archive;
            Encode(key_archive, "key", key);
            Encode(val_archive, "value", val);
            String key_str = key_archive.GetJson()["key"].dump();
            object[key_str] = val_archive.GetJson()["value"];
        }
        archive.GetJson()[name] = object;
    }

    template<typename Key, typename Value>
    inline static void Decode(JsonArchive& archive, const char* name, Map<Key, Value>& value)
    {
        if (archive.GetJson().contains(name) && archive.GetJson()[name].is_object())
        {
            value.clear();
            for (const auto& [key_str, val_json] : archive.GetJson()[name].items())
            {
                Key key{};
                Value val{};

                JsonArchive key_archive, val_archive;
                key_archive.GetJson()["key"] = nlohmann::json::parse(key_str);
                val_archive.GetJson()["value"] = val_json;

                Decode(key_archive, "key", key);
                Decode(val_archive, "value", val);

                value[key] = val;
            }
        }
    }

    // Support for std::unordered_map as well
    template<typename Key, typename Value>
    inline static void Encode(JsonArchive& archive, const char* name, const std::unordered_map<Key, Value>& value)
    {
        nlohmann::json object = nlohmann::json::object();
        for (const auto& [key, val] : value)
        {
            JsonArchive key_archive, val_archive;
            Encode(key_archive, "key", key);
            Encode(val_archive, "value", val);
            String key_str = key_archive.GetJson()["key"].dump();
            object[key_str] = val_archive.GetJson()["value"];
        }
        archive.GetJson()[name] = object;
    }

    template<typename Key, typename Value>
    inline static void Decode(JsonArchive& archive, const char* name, std::unordered_map<Key, Value>& value)
    {
        if (archive.GetJson().contains(name) && archive.GetJson()[name].is_object())
        {
            value.clear();
            for (const auto& [key_str, val_json] : archive.GetJson()[name].items())
            {
                Key key{};
                Value val{};

                JsonArchive key_archive, val_archive;
                key_archive.GetJson()["key"] = nlohmann::json::parse(key_str);
                val_archive.GetJson()["value"] = val_json;

                Decode(key_archive, "key", key);
                Decode(val_archive, "value", val);

                value[key] = val;
            }
        }
    }

    // Set serialization - prefer TL::Set, but support std::unordered_set too
    template<typename T>
    inline static void Encode(JsonArchive& archive, const char* name, const Set<T>& value)
    {
        nlohmann::json array = nlohmann::json::array();
        for (const auto& item : value)
        {
            JsonArchive item_archive;
            Encode(item_archive, "item", item);
            array.push_back(item_archive.GetJson()["item"]);
        }
        archive.GetJson()[name] = array;
    }

    template<typename T>
    inline static void Decode(JsonArchive& archive, const char* name, Set<T>& value)
    {
        if (archive.GetJson().contains(name) && archive.GetJson()[name].is_array())
        {
            value.clear();
            for (const auto& item : archive.GetJson()[name])
            {
                T decoded_item{};
                JsonArchive item_archive;
                item_archive.GetJson()["item"] = item;
                Decode(item_archive, "item", decoded_item);
                value.insert(decoded_item);
            }
        }
    }

    // Support for std::unordered_set as well
    template<typename T>
    inline static void Encode(JsonArchive& archive, const char* name, const std::unordered_set<T>& value)
    {
        nlohmann::json array = nlohmann::json::array();
        for (const auto& item : value)
        {
            JsonArchive item_archive;
            Encode(item_archive, "item", item);
            array.push_back(item_archive.GetJson()["item"]);
        }
        archive.GetJson()[name] = array;
    }

    template<typename T>
    inline static void Decode(JsonArchive& archive, const char* name, std::unordered_set<T>& value)
    {
        if (archive.GetJson().contains(name) && archive.GetJson()[name].is_array())
        {
            value.clear();
            for (const auto& item : archive.GetJson()[name])
            {
                T decoded_item{};
                JsonArchive item_archive;
                item_archive.GetJson()["item"] = item;
                Decode(item_archive, "item", decoded_item);
                value.insert(decoded_item);
            }
        }
    }

    // Optional serialization
    template<typename T>
    inline static void Encode(JsonArchive& archive, const char* name, const Optional<T>& value)
    {
        if (value.has_value())
        {
            JsonArchive nested_archive;
            Encode(nested_archive, "value", value.value());
            archive.GetJson()[name] = nested_archive.GetJson()["value"];
        }
        else
        {
            archive.GetJson()[name] = nullptr;
        }
    }

    template<typename T>
    inline static void Decode(JsonArchive& archive, const char* name, Optional<T>& value)
    {
        if (archive.GetJson().contains(name) && !archive.GetJson()[name].is_null())
        {
            T decoded_value{};
            JsonArchive nested_archive;
            nested_archive.GetJson()["value"] = archive.GetJson()[name];
            Decode(nested_archive, "value", decoded_value);
            value = decoded_value;
        }
        else
        {
            value = Nullopt;
        }
    }

    // Support for std::optional as well
    template<typename T>
    inline static void Encode(JsonArchive& archive, const char* name, const std::optional<T>& value)
    {
        if (value.has_value())
        {
            JsonArchive nested_archive;
            Encode(nested_archive, "value", value.value());
            archive.GetJson()[name] = nested_archive.GetJson()["value"];
        }
        else
        {
            archive.GetJson()[name] = nullptr;
        }
    }

    template<typename T>
    inline static void Decode(JsonArchive& archive, const char* name, std::optional<T>& value)
    {
        if (archive.GetJson().contains(name) && !archive.GetJson()[name].is_null())
        {
            T decoded_value{};
            JsonArchive nested_archive;
            nested_archive.GetJson()["value"] = archive.GetJson()[name];
            Decode(nested_archive, "value", decoded_value);
            value = decoded_value;
        }
        else
        {
            value = std::nullopt;
        }
    }

    // Clean up macros
    #undef TL_JSON_BASIC_TYPE
    #undef TL_JSON_SPECIAL_TYPE

} // namespace TL