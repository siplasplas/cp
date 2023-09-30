#ifndef CPG_BIGTABLECODEPAGE_H
#define CPG_BIGTABLECODEPAGE_H

#include "Codepage.h"

class BigTableCodepage: public Codepage {
public:
    int minCharLen() override;
    int maxCharLen() override;
};

#endif //CPG_BIGTABLECODEPAGE_H
