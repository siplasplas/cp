#ifndef CPG_CODEPAGE_H
#define CPG_CODEPAGE_H


#include <string>

class Codepage {
protected:
    std::string name;
public:
    explicit Codepage(std::string name);
    virtual int minCharLen() = 0;
    virtual int maxCharLen() = 0;
};

#endif //CPG_CODEPAGE_H
