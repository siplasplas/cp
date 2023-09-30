//
// Created by andrzej on 9/30/23.
//

#include <fstream>
#include <iostream>
#include "Language.h"
#include "utf/UTF.hpp"

using namespace std;

void Languages::readFromFile(const filesystem::path &path) {
    ifstream infile(path);
    while (!infile.eof()) {
        Language lang;
        lang.read(infile);
        languages.push_back(lang);
        string line;
        getline(infile, line);
    }
}

string Language::key(string line) {
    int pos1 = line.find('=');
    if (pos1 < 0) throw runtime_error("not found =");
    return line.substr(0, pos1);
}

string Language::subQuotes(string line) {
    int pos1 = line.find('\"');
    if (pos1 < 0) throw runtime_error("not found begin quote");
    int pos2 = line.find('\"', pos1 + 1);
    if (pos2 < 0) throw runtime_error("not found end quote");
    return line.substr(pos1 + 1, pos2 - pos1 - 1);
}

void Language::read(ifstream &inStream) {
    string line;
    getline(inStream, line);
    cout << line << endl;
    name = subQuotes(line);
    assert(key(line) == "name");
    getline(inStream, line);
    iso_code = subQuotes(line);
    assert(key(line) == "iso_code");
    getline(inStream, line);
    getline(inStream, line);
    getline(inStream, line);
    UTF utf;
    alphabet = utf.u8to16(subQuotes(line));
    assert(key(line) == "alphabet");
}

