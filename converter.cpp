#include <iostream>
#include "Args.h"

using namespace std;

void printHelp() {
    cout << "convcp options input_file output_file" << endl;
    cout << "available formats: " << endl;
}


string readToUTF8(const string &inFile, int format) {
    return std::string();
}

void saveFromUTF8(string str, const string &outFile, int format) {

}

void convert(const string &inFile, const string &outFile, int inFormat, int outFormat) {
    string u8 = readToUTF8(inFile, inFormat);
    saveFromUTF8(u8, outFile, outFormat);
}

int main(int argc, char *argv[]) {
    Args args(argc, argv);
    if (argc < 2)
        printHelp();
    else {
        args.parse();
        if (args.error())
            printHelp();
        else convert(args.inFile, args.outFile, args.inFormat, args.outFormat);
    }
}
