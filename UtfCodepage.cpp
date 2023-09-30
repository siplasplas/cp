//
// Created by andrzej on 9/30/23.
//

#include "UtfCodepage.h"

#include <utility>

int UtfCodepage::minCharLen() {
    return width;
}

int UtfCodepage::maxCharLen() {
    return 4;
}

UtfCodepage::UtfCodepage(std::string name, int width, bool binEndian) : Codepage(std::move(name)),
                                                                        width(width), binEndian(binEndian) {
}

std::u32string UtfCodepage::toU32(std::string_view str) {
    UTF utf;
    switch (width) {
        case 1: return utf.u8to32(str);
        case 2: {
            u16string_view sv16((char16_t*)str.data(), str.size()/2);
            if (!binEndian)
                return utf.u16to32(sv16);
            else {
                std::u16string str16;
                str16 = sv16;
                UTF::swapIt(str16);
                return utf.u16to32(str16);
            }
        }
        case 4: {
            u32string_view sv32((char32_t*)str.data(), str.size()/4);
            std::u32string str32;
            str32 = sv32;
            if (binEndian)
                UTF::reverseIt(str32);
            return str32;
        }
        default: return {};
    }
}

std::string UtfCodepage::fromU32(u32string_view dstr) {
    UTF utf;
    switch (width) {
        case 1: {
            //return utf.u32to8(dstr);
        }
    }
    return std::string();
}
