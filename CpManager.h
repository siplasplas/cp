#ifndef CPG_CPMANAGER_H
#define CPG_CPMANAGER_H

#include <vector>
#include "Codepage.h"

class CpManager {
    std::vector<Codepage*> codepages;
public:
    CpManager();
    virtual ~CpManager();
};

#endif //CPG_CPMANAGER_H
