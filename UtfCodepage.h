#ifndef CPG_UTFCODEPAGE_H
#define CPG_UTFCODEPAGE_H

#include "Codepage.h"

class UtfCodepage: public Codepage {
    int width;
    bool binEndian;
public:
    UtfCodepage(std::string name, int width, bool binEndian);
    int minCharLen() override;
    int maxCharLen() override;
};


#endif //CPG_UTFCODEPAGE_H
