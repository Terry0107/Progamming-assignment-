#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

struct Flags {
    bool CF = false;
    bool OF = false;
    bool UF = false;
    bool ZF = false;
};

struct VirtualMachine {
    char R[8] = {0}; // char (-128..127)
    char PC = 0;
    Flags F;
    char MEM[64] = {0};

    void initPC() { PC = 0; }
    void initFlags() { F.CF = F.OF = F.UF = F.ZF = false; }
    void initRegs() { for (char &val : R) val = 0; }
    void initMEM() { for (char &val : MEM) val = 0; }
    
    void initAll() {
        initFlags();
        initRegs();
        initMEM();
        initPC();
    }

    void updateFlags(char result) {
        F.ZF = (result == 0);
        F.OF = (result > 127);
        F.UF = (result < -128);
        F.CF = F.OF || F.UF;
    }

    void store(int address, char val) {
        MEM[address] = val;   // do bound checking and overflow and zero
    }

    char load(int address) {
        return MEM[address]; // checking
    }

    void input(int index) {
        int ch;
        cout << "?";
        cin >> ch;
        R[index] = ch;
    }

// part 6.3
    void add(int srcReg, int destReg) {
        int result = R[destReg] + R[srcReg];
        R[destReg] = static_cast<char>(result);
        updateFlags(R[destReg]);
    }

    void sub(int srcReg, int destReg) {
        int result = R[destReg] - R[srcReg];
        R[destReg] = static_cast<char>(result);
        updateFlags(R[destReg]);
    }

    void mul(int srcReg, int destReg) {
        int result = R[destReg] * R[srcReg];
        R[destReg] = static_cast<char>(result);
        updateFlags(R[destReg]);
    }

    void div(int srcReg, int destReg) {
        if (R[srcReg] == 0) {
            throw runtime_error("Division by zero");
        }
        int result = R[destReg] / R[srcReg];
        R[destReg] = static_cast<char>(result);
        updateFlags(R[destReg]);
    }

// part 6.4
    void inc(int destReg) {
        int result = R[destReg] + 1;
        R[destReg] = static_cast<char>(result);
        updateFlags(R[destReg]);
    }
    
    void dec(int destReg) {
        int result = R[destReg] - 1;
        R[destReg] = static_cast<char>(result);
        updateFlags(R[destReg]);
    }
};

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

// Remove extra spaces from a line
string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == string::npos ? "" : s.substr(start, end - start + 1));
}

class SimpleVM {
    vector<char> R;
    Flags F;
    vector<string> prog;
    size_t PC = 0;

public:
    SimpleVM() : R(8, 0) {}
    char MEM[64] = {};  // memory initialized to 0
      
    // Load assembly code from file
    bool load(const string &filename) {
        ifstream in(filename);
        if (!in) return false;
        string line;
        while (getline(in, line)) {
            line = trim(line);
            if (!line.empty()) prog.push_back(line);
        }
        return true;
    }
         // Run the whole program
    void run() { 
      PC = 0;
        PC = 0;
        while (PC < prog.size()) {
            execLine(prog[PC]);
            PC++;
        }
    }

    void dump() {
    cout << "Registers: ";
    for (char c : R)
        printf("%02X ", static_cast<unsigned char>(c));
    cout << "#\n";

    cout << "Flags    : " << F.ZF << " " << F.CF << " " << F.OF << " " << F.UF << "#\n";
    cout << "PC       : " << (int)PC << "\n\n";

    cout << "Memory   :\n";
    for (int i = 0; i < 64; i++) {
        printf("%02X ", static_cast<unsigned char>(MEM[i]));
        if ((i + 1) % 8 == 0) cout << "\n";
    }
    cout << "#\n";
}

    // Handle one line of code
    void execLine(const string &line) {
    stringstream ss(trim(line));
    string cmd;
    ss >> cmd;
    for (char &c : cmd) c = toupper(c);

    if (cmd == "SHL" || cmd == "SHR" || cmd == "ROL" || cmd == "ROR") {
        shiftOrRotate(ss, cmd);
    } 
    else if (cmd == "MOV") {
        int value;
        string reg;
        char comma;
        ss >> value >> comma >> reg;
        int r = reg[1] - '0';
        R[r] = value;
    } 
    else if (cmd == "ADD" || cmd == "SUB" || cmd == "MUL" || cmd == "DIV") {
        string reg1, reg2;
        char comma;
        ss >> reg1 >> comma >> reg2;
        int r1 = reg1[1] - '0';
        int r2 = reg2[1] - '0';
        int result;

        try {
            if (cmd == "ADD") {
                result = R[r2] + R[r1];
            } else if (cmd == "SUB") {
                result = R[r2] - R[r1];
            } else if (cmd == "MUL") {
                result = R[r2] * R[r1];
            } else if (cmd == "DIV") {
                if (R[r1] == 0) throw runtime_error("Division by zero");
                result = R[r2] / R[r1];
            }

            R[r2] = static_cast<char>(result);

            // Update Flags
                F.ZF = (R[r2] == 0);
                F.OF = (result > 127);
                F.UF = (result < -128);
                F.CF = F.OF || F.UF;

                cout << cmd << " " << reg1 << ", " << reg2 << " = " << (int)R[r2] << endl;
            } catch (exception &e) {
                cerr << "Error in " << cmd << ": " << e.what() << endl;
            }
        }
        else if (cmd == "INC" || cmd == "DEC") {
            string reg;
            ss >> reg;
            int r = reg[1] - '0';
            int result;

            if (cmd == "INC") result = R[r] + 1;
            else result = R[r] - 1;

            R[r] = static_cast<char>(result);

            // Update Flags
            F.ZF = (R[r] == 0);
            F.OF = (result > 127);
            F.UF = (result < -128);
            F.CF = F.OF || F.UF;

            cout << cmd << " " << reg << " = " << (int)R[r] << endl;
        }
        else {
            cout << "Unsupported command: " << cmd << endl;
        }
    }

    void shiftOrRotate(stringstream &ss, const string &op) {
        string reg;
        char comma;
        int count;
        ss >> reg >> comma >> count;

        int r = reg[1] - '0';
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

        cout << op << " " << reg << " = " << (int)R[r] << " | CF = " << F.CF << endl;
    }
};

int main() {
    SimpleVM vm;

    // Try to load the assembly instructions from a file
    if (vm.load("prog2.asm")) {
        cout << "Running instructions from sample1.asm...\n";
        vm.run();  // Run all loaded instructions
    } else {
        cout << "sample1.asm not found, running test instructions...\n";

        // Test commands to use if file doesn't exist
        vector<string> test = {
            "MOV 28, R0",
            "SHL R0, 1",
            "ROL R0, 1"
        };

        // Run each test line manually
        for (string line : test) {
            vm.execLine(line);
        }
    }

    // Print out everything (registers, flags, memory)
    vm.dump();
    return 0;
}
