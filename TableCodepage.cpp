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
        : Codepage(std::move(name)), convTable(convTable) {
    this->bestTable = bestTable;
}

