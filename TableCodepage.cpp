#include "TableCodepage.h"

#include <utility>

int TableCodepage::minCharLen() {
    return 1;
}

int TableCodepage::maxCharLen() {
    return 1;
}

TableCodepage::TableCodepage(std::string name, uint16_t *convTable) : Codepage(std::move(name)), convTable(convTable) {
}

TableCodepage::TableCodepage(std::string name, uint16_t *convTable, std::pair<uint16_t, uint16_t> *bestTable)
        : TableCodepage(std::move(name), convTable) {
    this->bestTable = bestTable;
}

std::u32string TableCodepage::toU32(std::string_view str) {
    return std::u32string();
}

std::string TableCodepage::fromU32(u32string_view dstr) {
    return std::string();
}

