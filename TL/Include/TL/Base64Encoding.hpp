#pragma once

#include <vector>
#include <string>

namespace TL
{
    static const char kBase64Alphabet[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    std::string base64Encode(const std::vector<uint8_t>& data)
    {
        std::string result;
        size_t      len = data.size();
        result.reserve(((len + 2) / 3) * 4);
        size_t i = 0;
        while (i < len)
        {
            uint8_t  octet_a = i < len ? data[i++] : 0;
            uint8_t  octet_b = i < len ? data[i++] : 0;
            uint8_t  octet_c = i < len ? data[i++] : 0;
            uint32_t triple  = (octet_a << 16) | (octet_b << 8) | octet_c;
            result.push_back(kBase64Alphabet[(triple >> 18) & 0x3F]);
            result.push_back(kBase64Alphabet[(triple >> 12) & 0x3F]);
            result.push_back((i > len + 1) ? '=' : kBase64Alphabet[(triple >> 6) & 0x3F]);
            result.push_back((i > len) ? '=' : kBase64Alphabet[triple & 0x3F]);
        }
        return result;
    }

    std::vector<uint8_t> base64Decode(const std::string& input)
    {
        if (input.size() % 4 != 0)
            throw std::runtime_error("Invalid base64 length");

        auto decode_char = [](char c) -> int
        {
            if ('A' <= c && c <= 'Z')
                return c - 'A';
            if ('a' <= c && c <= 'z')
                return c - 'a' + 26;
            if ('0' <= c && c <= '9')
                return c - '0' + 52;
            if (c == '+')
                return 62;
            if (c == '/')
                return 63;
            if (c == '=')
                return -1; // padding
            throw std::runtime_error("Invalid base64 character");
        };

        std::vector<uint8_t> out;
        out.reserve((input.size() / 4) * 3);

        for (size_t i = 0; i < input.size(); i += 4)
        {
            int      c1 = decode_char(input[i]);
            int      c2 = decode_char(input[i + 1]);
            int      c3 = decode_char(input[i + 2]);
            int      c4 = decode_char(input[i + 3]);

            uint32_t triple = ((c1 & 0x3F) << 18) |
                              ((c2 & 0x3F) << 12) |
                              ((c3 & 0x3F) << 6) |
                              ((c4 & 0x3F));

            out.push_back((triple >> 16) & 0xFF);
            if (input[i + 2] != '=')
                out.push_back((triple >> 8) & 0xFF);
            if (input[i + 3] != '=')
                out.push_back(triple & 0xFF);
        }

        return out;
    }

} // namespace TL