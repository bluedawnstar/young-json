#pragma once

#include <exception>

#include "jsonvalue.h"
#include "jsonencoding.h"

namespace youngJSON {

enum EncodingType {
    etUTF8,
    etUTF16
};

class invalid_format_error : public std::logic_error {
public:
    explicit invalid_format_error(const std::string& msg) : logic_error(msg) {
    }

    explicit invalid_format_error(const char* msg = "invalid json format") : logic_error(msg) {
    }
};


class JsonReader {
public:

public:
    JsonReader(EncodingType type = etUTF8) : encodingType(type) {
        p = pEnd = nullptr;
    }

    std::shared_ptr<JsonValue> read(const unsigned char* src, int length) {
        p = src;
        pEnd = p + length;

        return readValue();
    }

    std::shared_ptr<JsonValue> read(const std::string& src) {
        return read((const unsigned char*)src.data(), (int)src.length());
    }

private:
    void checkOutOfRange() {
        if (p >= pEnd)
            throw std::range_error("out of range");
    }

    void skipSpace() {
        while (p < pEnd && isspace(*p))
            ++p;
        checkOutOfRange();
    }

    unsigned int hexToUInt(int ch) {
        return isdigit(ch) ? ch - '0' : toupper(ch) - 'A' + 10;
    }

    std::string readString() {
        std::string res;

        skipSpace();
        if (*p != '\"') {
            throw invalid_format_error();
        } else {
            while (*++p != '\"') {
                if (*p == '\\') {
                    switch (*++p) {
                    case 'b': res += '\b'; break;
                    case 'f': res += '\f'; break;
                    case 'n': res += '\n'; break;
                    case 'r': res += '\r'; break;
                    case 't': res += '\t'; break;
                    case 'u':
                    {
                        unsigned int code = 0;
                        code += hexToUInt(*++p) << 12;
                        code += hexToUInt(*++p) << 8;
                        code += hexToUInt(*++p) << 4;
                        code += hexToUInt(*++p);
                        if (encodingType == etUTF8)
                            appendUTF8(res, code);
                        else {
                            res += code >> 8;
                            res += code & 0xff;
                        }
                        break;
                    }
                    default:
                        res += *p;
                    }
                } else {
                    res += *p;
                }
            }
            ++p;
        }

        return res;
    }

    std::string readKeyword() {
        std::string res;
        while (p < pEnd && isalpha(*p))
            res += *p++;
        return res;
    }

    bool isValidNumberChar(int ch) {
        return *p == '-' || *p == '+' || *p == '.' || *p == 'e' || *p == 'E' || isdigit(*p);
    }

    std::string readNumber() {
        std::string res;
        while (p < pEnd && isValidNumberChar(*p))
            res += *p++;
        return res;
    }

    std::shared_ptr<JsonValue> readObject() {
        auto res = std::make_shared<ValueObject>();

        int cnt = 0;

        p++; // '{'
        while (p < pEnd) {
            skipSpace();
            if (*p == '}')
                break;

            if (cnt > 0) {
                if (*p++ != ',')
                    throw invalid_format_error();
                skipSpace();
            }

            auto name = readString();
            skipSpace();
            if (*p++ != ':')
                throw invalid_format_error();
            skipSpace();

            res->add(name, readValue());
            cnt++;
        }
        if (*p++ != '}')
            throw invalid_format_error();

        return res;
    }

    std::shared_ptr<JsonValue> readArray() {
        auto res = std::make_shared<ValueArray>();

        int cnt = 0;

        p++; // '['
        while (p < pEnd) {
            skipSpace();

            if (*p == ']')
                break;

            if (cnt > 0) {
                if (*p++ != ',')
                    throw invalid_format_error();
                skipSpace();
            }

            res->add(readValue());
            cnt++;
        }
        if (*p++ != ']')
            throw invalid_format_error();

        return res;
    }

    std::shared_ptr<JsonValue> readValue() {
        skipSpace();

        if (*p == '{')
            return readObject();
        else if (*p == '[')
            return readArray();
        else if (*p == '\"')
            return std::make_shared<ValueString>(readString());
        else if (isalpha(*p)) {
            auto keyword = readKeyword();
            if (keyword == "true")
                return std::make_shared<ValueTrue>();
            else if (keyword == "false")
                return std::make_shared<ValueFalse>();
            else if (keyword == "null")
                return std::make_shared<ValueNull>();
        } else {
            if (isValidNumberChar(*p))
                return std::make_shared<ValueNumber>(readNumber());
        }

        throw invalid_format_error();
    }

private:
    const unsigned char*    p;
    const unsigned char*    pEnd;

    EncodingType            encodingType;
};

}
