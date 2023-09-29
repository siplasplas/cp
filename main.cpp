#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

string inFile = "../www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP1250.TXT";
string inFileBest = "../www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WindowsBestFit/bestfit1250.txt";
string CP936 = "../www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP936.TXT";
string CP936Best = "../www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WindowsBestFit/bestfit936.txt";

string trimRight(const string &str) {
    const auto strEnd = str.find_last_not_of(" \t\r");
    if (strEnd == string::npos)
        return ""; // no content
    return str.substr(0, strEnd + 1);
}

template<typename I>
std::string n2hexstr(I w, size_t hex_len = sizeof(I) << 1) {
    static const char *digits = "0123456789ABCDEF";
    std::string rc(hex_len, '0');
    for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
        rc[i] = digits[(w >> j) & 0x0f];
    return rc;
}

bool readTwo(const string &line, int &a, int &b) {
    int pos1 = line.find('\t');
    if (pos1 < 0)
        pos1 = line.find("  ");
    if (pos1 < 0)
        throw runtime_error("can't read two hex integers");
    string s1 = line.substr(0, pos1);
    size_t idx;
    a = stoi(s1, &idx, 16);
    int pos2 = line.find('\t', pos1 + 1);
    string s2;
    if (pos2 < 0)
        pos2 = line.find("  ", pos1 + 1);
    s2 = line.substr(pos1 + 1, pos2 - pos1 - 1);
    if (pos2 < 0)
        pos2 = line.length();
    if (s2.empty() || s2[0] == ' ') {
        int pos3 = line.find('\t', pos2 + 1);
        string s3 = line.substr(pos2 + 1, pos3 - pos2 - 1);
        if (s3 == "#DBCS LEAD BYTE") {
            b = a;
            return true;
        } else {
            b = 0xfffd;
            return false;
        }
    } else
        b = stoi(s2, &idx, 16);
    return false;
}

void readMBtable(ifstream &infile, int mbSize, uint16_t tab[], uint16_t *dbcsroot[]) {
    for (int i = 0; i < 128; i++) {
        tab[i] = 0xfffd;
        dbcsroot[i] = nullptr;
    }
    int i = 0;
    for (string line; getline(infile, line);) {
        line = trimRight(line);
        if (line.empty()) continue;
        if (line[0] == '#') continue;
        int a, b;
        bool dbcs = readTwo(line, a, b);
        if (a >= 128)
            tab[a - 128] = dbcs ? 0 : b;
        else if (a != b)
            throw runtime_error("7bit ascii code mismatch");
        if (dbcs) {
            cout << "dbcs" << n2hexstr(b, 2) << endl;
            if (!dbcsroot[a - 128]) {
                dbcsroot[a - 128] = new uint16_t[256];
                memset(dbcsroot[a - 128], 0, 256 * sizeof(uint16_t));
            }
        }
        i++;
        if (i==mbSize)
            break;
    }
}

void readDBCStables(ifstream &infile, uint16_t *dbcsroot[]) {
    for (string line; getline(infile, line);) {
        line = trimRight(line);
        if (line.empty()) break;
        int a, b;
        readTwo(line, a, b);
        dbcsroot[(a>>8)-128][a & 0xff] = b;
    }
}

void processFile(const string &filename) {
    uint16_t tab[128];
    ifstream infile(filename);
    readMBtable(infile, 256, tab, nullptr);
    for (int i = 128; i < 256; i += 16) {
        for (int j = i; j < i + 16; j++)
            cout << tab[j - 128] << ",";
        cout << endl;
    }
}

//1 or 2 bytes for code
void processFile12(const string &filename) {
    uint16_t tab[128];
    uint16_t* dbcsroot[128];
    ifstream infile(filename);
    readMBtable(infile, 256, tab, dbcsroot);
    readDBCStables(infile, dbcsroot);
}

void processBest(const string &filename) {
    uint16_t tab[128];
    ifstream infile(filename);
    int mbSize = 0;
    for (string line; getline(infile, line);) {
        line = trimRight(line);
        if (line.empty()) continue;
        if (line.substr(0, 7) == "MBTABLE") {
            mbSize = stoi(line.substr(8));
            break;
        }
    }
    string line1;
    getline(infile, line1);
    line1 = trimRight(line1);
    if (!line1.empty())
        throw runtime_error("no empty line");
    readMBtable(infile, mbSize, tab, nullptr);
    int wcSize = 0;
    for (string line; getline(infile, line);) {
        line = trimRight(line);
        if (line.empty()) continue;
        if (line.substr(0, 7) == "WCTABLE") {
            wcSize = stoi(line.substr(8));
            break;
        }
    }
    vector<pair<uint16_t, uint8_t>> bestv;
    getline(infile, line1);
    line1 = trimRight(line1);
    if (!line1.empty())
        throw runtime_error("no empty line");
    for (int i = 0; i < wcSize; i++) {
        string line;
        getline(infile, line);
        line = trimRight(line);
        int a, b;
        readTwo(line, b, a);
        if (a < 128) {
            if (a != b)
                bestv.emplace_back(make_pair(b, a));
        } else {
            if (a > 255)
                throw runtime_error("ascii code mismatch");
            if (tab[a - 128] != b)
                bestv.emplace_back(make_pair(b, a));
        }
    }
}

void processBest12(const string &filename) {
    uint16_t tab[128];
    uint16_t *dbcsroot[128];
    ifstream infile(filename);
    int mbSize = 0;
    for (string line; getline(infile, line);) {
        line = trimRight(line);
        if (line.empty()) continue;
        if (line.substr(0, 7) == "MBTABLE") {
            mbSize = stoi(line.substr(8));
            break;
        }
    }
    string line1;
    getline(infile, line1);
    line1 = trimRight(line1);
    if (!line1.empty())
        throw runtime_error("no empty line");
    readMBtable(infile, mbSize, tab, dbcsroot);
    string line;
    for (; getline(infile, line);) {
        line = trimRight(line);
        if (!line.empty()) break;
    }
    if (line.find("DBCSRANGE  1") != 0) throw runtime_error("no DBCSRANGE  1");
    getline(infile, line);
    getline(infile, line);
    int rangeFrom, rangeTo;
    readTwo(line, rangeFrom, rangeTo);
    int wcSize = 0;
    for (string line; getline(infile, line);) {
        line = trimRight(line);
        if (line.empty()) continue;
        if (line.substr(0, 7) == "WCTABLE") {
            wcSize = stoi(line.substr(8));
            break;
        }
    }
    vector<pair<uint16_t, uint8_t>> bestv;
    getline(infile, line1);
    line1 = trimRight(line1);
    if (!line1.empty())
        throw runtime_error("no empty line");
    for (int i = 0; i < wcSize; i++) {
        string line;
        getline(infile, line);
        line = trimRight(line);
        int a, b;
        readTwo(line, b, a);
        if (a < 128) {
            if (a != b)
                bestv.emplace_back(make_pair(b, a));
        } else {
            if (a > 255) {}
            else if (tab[a - 128] != b)
                bestv.emplace_back(make_pair(b, a));
        }
    }
}

int main() {
    //processFile(inFile);
    //processBest(inFileBest);
    //processFile12(CP936);
    processBest12(CP936Best);
    return 0;
}
