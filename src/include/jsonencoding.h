#pragma once

#include <string>

namespace youngJSON {

inline void appendUTF8(std::string& s, unsigned int code) {
    if (code <= 0x00007F)
        s += code;
    else if (code <= 0x0007FF) {
        s += (code >> 6) | 0xC0;
        s += (code & 0x3F) | 0x80;
    } else if (code <= 0x00FFFF) {
        s += ( code >> 12        ) | 0xE0;
        s += ((code >>  6) & 0x3F) | 0x80;
        s += ( code        & 0x3F) | 0x80;
    } else if (code <= 0x10FFFF) {
        s += ( code >> 18        ) | 0xF0;
        s += ((code >> 12) & 0x3F) | 0x80;
        s += ((code >>  6) & 0x3F) | 0x80;
        s += ( code        & 0x3F) | 0x80;
    }
}

inline std::string toUTF8(unsigned int code) {
    std::string res;
    appendUTF8(res, code);
    return res;
}

}