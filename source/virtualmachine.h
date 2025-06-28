#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Flags {
    bool CF = false;
    bool OF = false;
    bool UF = false;
    bool ZF = false;
};

struct VirtualMachine {
    char R[8] = {0};
    char PC = 0;
    Flags F;
    char MEM[64] = {0};

    void initPC();
    void initFlags();
    void initRegs();
    void initMEM();
    void initAll();
    void updateFlags(char result);
    void store(int address, char val);
    char load(int address);
    void input(int index);
    void add(int srcReg, int destReg);
    void sub(int srcReg, int destReg);
    void mul(int srcReg, int destReg);
    void div(int srcReg, int destReg);
    void inc(int destReg);
    void dec(int destReg);
};

class SimpleVM {
    vector<char> R;
    Flags F;
    vector<string> prog;
    size_t PC = 0;

public:
    char MEM[64] = {};

    SimpleVM();
    bool load(const string &filename);
    void run();
    void dump();
    void dumpToFile(const string &filename);
    void updateFlags(char result);

private:
    void execLine(const string &line);
    void shiftOrRotate(stringstream &ss, const string &op);
    string trim(const string &s);
    string stripBrackets(const string &s);
};

void runner();

#endif // VIRTUALMACHINE_H