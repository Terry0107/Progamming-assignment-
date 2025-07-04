#include "virtualmachine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

void VirtualMachine::initPC() { PC = 0; }
void VirtualMachine::initFlags() { F.CF = F.OF = F.UF = F.ZF = false; }
void VirtualMachine::initRegs() { for (char &val : R) val = 0; }
void VirtualMachine::initMEM() { for (char &val : MEM) val = 0; }
void VirtualMachine::initAll() {
    initFlags(); initRegs(); initMEM(); initPC();
}
void VirtualMachine::updateFlags(char result) {
    F.ZF = (result == 0);
    F.OF = (result > 127);
    F.UF = (result < -128);
    F.CF = F.OF || F.UF;
}
void VirtualMachine::store(int address, char val) { MEM[address] = val; }
char VirtualMachine::load(int address) { return MEM[address]; }
void VirtualMachine::input(int index) {
    int ch; cout << "?"; cin >> ch; R[index] = ch;
}
void VirtualMachine::add(int srcReg, int destReg) {
    int result = R[destReg] + R[srcReg];
    R[destReg] = static_cast<char>(result);
    updateFlags(R[destReg]);
}
void VirtualMachine::sub(int srcReg, int destReg) {
    int result = R[destReg] - R[srcReg];
    R[destReg] = static_cast<char>(result);
    updateFlags(R[destReg]);
}
void VirtualMachine::mul(int srcReg, int destReg) {
    int result = R[destReg] * R[srcReg];
    R[destReg] = static_cast<char>(result);
    updateFlags(R[destReg]);
}
void VirtualMachine::div(int srcReg, int destReg) {
    if (R[srcReg] == 0) throw runtime_error("Division by zero");
    int result = R[destReg] / R[srcReg];
    R[destReg] = static_cast<char>(result);
    updateFlags(R[destReg]);
}
void VirtualMachine::inc(int destReg) {
    int result = R[destReg] + 1;
    R[destReg] = static_cast<char>(result);
    updateFlags(R[destReg]);
}
void VirtualMachine::dec(int destReg) {
    int result = R[destReg] - 1;
    R[destReg] = static_cast<char>(result);
    updateFlags(R[destReg]);
}

// MOV 28, R0
// ROL R0, 1
// 0 0 0 1 1 1 0 0  == 28
// 0 0 1 1 1 0 0 0  ==>> rotated value
// covert back into decimal and store into R[0]

// a function to execute the assembly program

void runner() {
    VirtualMachine vm;
    vm.initAll();

    vm.input(0); //Part 6.1
    cout << "R[0] = " << int(vm.R[0]) << endl; //Part 6.1

    vm.R[0] = 10; //Part6.2
    vm.R[0] = vm.R[1]; //Part6.2
    vm.R[3] = vm.load(vm.R[1]);  //Part6.2

    // loop to read asm file and execute each line
    // split the line into 3 components
    // 1. command
    // 2. operand1
    // 3. operand2
}

SimpleVM::SimpleVM() : R(8, 0) {}

bool SimpleVM::load(const std::string &filename) {
    std::ifstream in(filename);
    if (!in) return false;

    std::string line;
    while (std::getline(in, line)) {
        line = trim(line);
        if (!line.empty()) prog.push_back(line);
    }

    return true;
}


void SimpleVM::run() {
    PC = 0;
    while (PC < prog.size()) {
        execLine(prog[PC]);
        PC++;
    }
}
    void SimpleVM::dump() {
        cout << "| Registers: |";
        for (int i = 0; i < 8; ++i) {
            cout << setw(3) << (int)R[i] << "  |";
            if (i == 7) cout << "#";
        }
        cout << "\n|---|---|---|---|---|---|---|---|---|\n";

        cout << "| Flags    | :   | " << F.ZF << "   | " << F.CF << "   | " << F.OF << "   | " << F.UF << "#    |    |    |    |\n";

        cout << "| PC       | :   | " << PC << "    |    |    |    |    |    |    |\n\n";

        cout << "Memory :\n\n";
        for (int i = 0; i < 56; i += 8) {
            for (int j = 0; j < 8; ++j) {
                cout << setw(2) << (int)MEM[i+j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    void SimpleVM::updateFlags(char result) {
        F.ZF = (result == 0);
        F.OF = (result > 127);
        F.UF = (result < -128);
        F.CF = F.OF || F.UF;
    }

// Remove extra spaces from a line
    string SimpleVM::trim(const string &s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        size_t end = s.find_last_not_of(" \t\r\n");
        return (start == string::npos ? "" : s.substr(start, end - start + 1));
}

// Utility to remove square brackets
string SimpleVM::stripBrackets(const string &s) {
    if (s.front() == '[' && s.back() == ']') {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

    // Handle one line of code
void SimpleVM::execLine(const string &line) {
    // Remove inline comments
    string cleanLine = line;
    size_t commentPos = cleanLine.find(';');
    if (commentPos != string::npos)
        cleanLine = cleanLine.substr(0, commentPos);

    stringstream ss(trim(cleanLine));
    string cmd;
    ss >> cmd;
    for (char &c : cmd) c = toupper(c);

    string op1, op2;
    string operands;

    if (cmd == "SHL" || cmd == "SHR" || cmd == "ROL" || cmd == "ROR") {
        getline(ss, operands);
        size_t commaPos = operands.find(',');
        if (commaPos == string::npos) {
            cout << "[ERROR] Malformed shift/rotate instruction: " << line << endl;
            return;
        }
        op1 = trim(operands.substr(0, commaPos));
        op2 = trim(operands.substr(commaPos + 1));
        stringstream inner;
        inner << op1 << " , " << op2;
        shiftOrRotate(inner, cmd);

    } else if (cmd == "MOV" || cmd == "LOAD" || cmd == "STORE" || cmd == "ADD" || cmd == "SUB" || cmd == "MUL" || cmd == "DIV") {
        getline(ss, operands);
        size_t commaPos = operands.find(',');
        if (commaPos == string::npos) {
            cout << "[ERROR] Missing comma in operands: " << operands << endl;
            return;
        }
        op1 = trim(operands.substr(0, commaPos));
        op2 = trim(operands.substr(commaPos + 1));

        if (cmd == "MOV") {
            if (op1[0] == '[') {
                string regName = stripBrackets(op1);
                if (regName[0] == 'R' && isdigit(regName[1])) {
                    int reg = regName[1] - '0';
                    int addr = static_cast<unsigned char>(R[reg]);
                    int dst = op2[1] - '0';
                    R[dst] = MEM[addr];
                    updateFlags(R[dst]);
                }
            } else if (op1[0] == 'R') {
                int src = op1[1] - '0';
                int dst = op2[1] - '0';
                R[dst] = R[src];
                updateFlags(R[dst]);
            } else {
                try {
                    int val = stoi(op1);
                    int dst = op2[1] - '0';
                    R[dst] = val;
                    updateFlags(R[dst]);
                } catch (...) {
                    cout << "[ERROR] Invalid immediate value in MOV: " << op1 << endl;
                    return;
                }
            }
        } else if (cmd == "LOAD") {
            int dstReg = op2[1] - '0';
            try {
                if (op1[0] == '[') {
                    string regName = stripBrackets(op1);
                    if (regName[0] == 'R' && isdigit(regName[1])) {
                        int reg = regName[1] - '0';
                        int addr = static_cast<unsigned char>(R[reg]);
                        R[dstReg] = MEM[addr];
                    } else {
                        cout << "[ERROR] Invalid LOAD register: " << op1 << endl;
                        return;
                    }
                } else {
                    int addr = stoi(op1);
                    R[dstReg] = MEM[addr];
                }
            } catch (...) {
                cout << "[ERROR] Invalid LOAD address: " << op1 << endl;
                return;
            }
            updateFlags(R[dstReg]);

        } else if (cmd == "STORE") {
            if (op1[0] != 'R' || op1.length() < 2 || !isdigit(op1[1])) {
                cout << "[ERROR] Invalid source register: " << op1 << endl;
                return;
            }
            int srcReg = op1[1] - '0';
            try {
                if (op2[0] == '[') {
                    string regName = stripBrackets(op2);
                    if (regName[0] == 'R' && isdigit(regName[1])) {
                        int dstReg = regName[1] - '0';
                        int addr = static_cast<unsigned char>(R[dstReg]);
                        MEM[addr] = R[srcReg];
                    } else {
                        cout << "[ERROR] Invalid STORE register: " << regName << endl;
                        return;
                    }
                } else {
                    int addr = stoi(op2);
                    MEM[addr] = R[srcReg];
                }
            } catch (...) {
                return;
            }
            updateFlags(R[srcReg]);

        } else if (cmd == "ADD" || cmd == "SUB" || cmd == "MUL" || cmd == "DIV") {
            int dstReg = op2[1] - '0';

            try {
                int srcValue;

                if (op1[0] == 'R') {
                srcValue = R[op1[1]-'0'];
            } else {
                srcValue = stoi(op1);
            }

            if (cmd == "ADD") {
                R[dstReg] += srcValue;
            } else if (cmd == "SUB") {
                R[dstReg] -= srcValue;
            } else if (cmd == "MUL") {
                R[dstReg] *= srcValue;
            } else if (cmd == "DIV") {
                if (srcValue == 0) throw runtime_error("Division by zero");
                R[dstReg] /= srcValue;
            }
                
                updateFlags(R[dstReg]);
                cout << cmd << " " << op1 << ", " << op2 << " = " << (int)R[dstReg] << endl;
        } catch (exception &e) {
            cout << "[ERROR] " << e.what() << " in " << cmd << " operation" << endl;
        }
        }

        } else if (cmd == "INC" || cmd == "DEC") {
            ss >> op1;
            if (op1[0] != 'R' || !isdigit(op1[1])) {
                cout << "[ERROR] Invalid register in " << cmd << ": " << op1 << endl;
                return;
            }
            int reg = op1[1] - '0';
            if (cmd == "INC") {
                R[reg]++;
            } else {
               R[reg]--;
            }
            updateFlags(R[reg]);

    } else if (cmd == "INPUT") {
        ss >> op1;
        int r = op1[1] - '0';
        int val;
        cout << "? ";
        cin >> val;
        R[r] = val;
        updateFlags(R[r]);

    } else if (cmd == "DISPLAY") {
        ss >> op1;
        int r = op1[1] - '0';
        cout << "R[" << r << "] = " << (int)R[r] << endl;

    } else {
        cout << "Unsupported command: " << cmd << endl;
    }
}


void SimpleVM::shiftOrRotate(stringstream &ss, const string &op) {
    string reg;
    char comma;
    int count;

    if (!(ss >> reg >> comma >> count)) {
        cout << "[ERROR] Malformed shift/rotate instruction.\n";
        return;
    }

    if (reg.size() < 2 || reg[0] != 'R' || !isdigit(reg[1])) {
        cout << "[ERROR] Invalid register format: " << reg << endl;
        return;
    }

    int r = reg[1] - '0';
    if (r < 0 || r >= 8) {
        cout << "[ERROR] Register out of range: " << reg << endl;
        return;
    }

    unsigned char value = R[r];

    if (op == "SHL") {
        F.CF = value & 0x80;
        value <<= 1;
    } else if (op == "SHR") {
        F.CF = value & 0x01;
        value >>= 1;
    } else if (op == "ROL") {
        F.CF = value & 0x80;
        value = value << 1;
        if (F.CF) value |= 0x01;
    } else if (op == "ROR") {
        F.CF = value & 0x01;
        value = value >> 1;
        if (F.CF) value |= 0x80;
    }

    R[r] = value;
    F.ZF = (R[r] == 0);
    F.OF = false;
    F.UF = false;

    cout << op << " " << reg << ", " << count << " = " << (int)R[r] << " | CF = " << F.CF << endl;
}


    void SimpleVM::dumpToFile(const string &filename) {
    ofstream out(filename);

    out << "Registers: ";
    for (int i = 0; i < 8; ++i) {
        out << setw(2) << setfill('0') << (int)R[i] << " ";
    }
    out << "#\n";

    out << "Flags    : " << F.ZF << " " << F.CF << " " << F.OF << " " << F.UF << "#\n";
    out << "PC       : " << PC << "\n";

    out << "Memory :\n";
    for (int i = 0; i < 64; ++i) {
        out << setw(2) << setfill('0') << (int)MEM[i] << " ";
        if ((i + 1) % 8 == 0) out << "\n";
    }
    out << "#\n";
};

int main() {
    VirtualMachine vm;
    vm.initAll();
    vm.input(0);
    cout << "R[0] = " << (int)vm.R[0] << endl;

    cout << "\n--- Running Program from prog1.asm ---\n";
    SimpleVM simpleVM;
    if (!simpleVM.load("prog1.asm")) {
        cout << "Failed to load file\n";
        return 1;
    }

    simpleVM.run();
    simpleVM.dumpToFile("output.txt");
    simpleVM.dump();

    return 0;
}