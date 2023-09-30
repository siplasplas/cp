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
