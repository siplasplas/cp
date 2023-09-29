#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

string inFile = "../www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP1250.TXT";
string inFileBest = "../www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WindowsBestFit/bestfit1250.txt";

string trimRight(const string &str) {
    const auto strEnd = str.find_last_not_of(" \t\r");
    if (strEnd == string::npos)
        return ""; // no content
    return str.substr(0, strEnd + 1);
}

void readTwo(const string &line, int &a, int &b) {
    int pos1 = line.find('\t');
    string s1 = line.substr(0, pos1);
    size_t idx;
    a = stoi(s1, &idx, 16);
    int pos2 = line.find('\t', pos1 + 1);
    string s2 = line.substr(pos1 + 1, pos2 - pos1 - 1);
    if (s2.empty() || s2[0] == ' ')
        b = 0xfffd;
    else
        b = stoi(s2, &idx, 16);
}

void readMBtable(ifstream &infile, uint16_t tab[]) {
    int i = 0;
    for (string line; getline(infile, line);) {
        line = trimRight(line);
        if (line.empty()) continue;
        if (line[0] == '#') continue;
        int a, b;
        readTwo(line, a, b);
        if (a != i)
            throw runtime_error("MBTable mismatch");
        if (a >= 128)
            tab[a - 128] = b;
        else if (a != b)
            throw runtime_error("7bit ascii code mismatch");
        i++;
        if (i==256)
            break;
    }
}

void processFile(const string &filename) {
    uint16_t tab[128];
    ifstream infile(filename);
    readMBtable(infile, tab);
    for (int i = 128; i < 256; i += 16) {
        for (int j = i; j < i + 16; j++)
            cout << tab[j - 128] << ",";
        cout << endl;
    }
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
    if (mbSize!=256)
        throw runtime_error("MBTABLE size must be 256");
    readMBtable(infile, tab);
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


int main() {
    //processFile(inFile);
    processBest(inFileBest);
    return 0;
}
