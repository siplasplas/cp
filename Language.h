#ifndef CPG_LANGUAGE_H
#define CPG_LANGUAGE_H

#include <string>
#include <vector>
#include <filesystem>

using namespace std;

struct Language {
    string name;
    string iso_code;
    bool use_ascii;
    vector<string> charsets;
    u16string alphabet;
    void read(ifstream &inStream);
    string key(string line);
    static string subQuotes(string line);
};

class Languages {
    vector<Language> languages;
public:
    void readFromFile(const filesystem::path &path);
};

#endif //CPG_LANGUAGE_H
