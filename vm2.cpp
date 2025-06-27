#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
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

    // 6.1 I/O Operations
    void input(int index) {
        int ch;
        cout << "?";
        cin >> ch;
        R[index] = static_cast<char>(ch);
        updateFlags(R[index]);
    }

    void output(int regIndex) {
        cout << "Output: " << static_cast<int>(R[regIndex]) << endl;
    }

    // 6.2 MOV Operations
    void movImmediate(char value, int destReg) {
        R[destReg] = value;
    }

    void movRegister(int srcReg, int destReg) {
        R[destReg] = R[srcReg];
    }

    void movMemoryIndirect(int addrReg, int destReg) {
        R[destReg] = MEM[static_cast<unsigned char>(R[addrReg]) % 64];
    }

    // 6.3 Arithmetic Operations
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

    // 6.4 Increment/Decrement Operations
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

    void store(int address, char val) {
        if (address < 0 || address >= 64) {
            throw runtime_error("Memory address out of bounds");
        }
        MEM[address] = val;
    }

    char load(int address) {
        if (address < 0 || address >= 64) {
            throw runtime_error("Memory address out of bounds");
        }
        return MEM[address];
    }

    void dump() {
        cout << "Registers: ";
        for (int i = 0; i < 8; i++) {
            printf("%02X ", static_cast<unsigned char>(R[i]));
        }
        cout << "#\n";

        cout << "Flags    : " << F.ZF << " " << F.CF << " " << F.OF << " " << F.UF << "#\n";
        cout << "PC       : " << static_cast<int>(PC) << "\n\n";

        cout << "Memory   :\n";
        for (int i = 0; i < 64; i++) {
            printf("%02X ", static_cast<unsigned char>(MEM[i]));
            if ((i + 1) % 8 == 0) cout << "\n";
        }
        cout << "#\n";
    }
};

void runner() {
    VirtualMachine vm;
    vm.initAll();

    // Test 6.1 I/O
    vm.input(0);
    cout << "R[0] = " << static_cast<int>(vm.R[0]) << endl;

    // Test 6.2 MOV
    vm.movImmediate(10, 1);
    vm.movRegister(1, 2);
    vm.store(10, 20);
    vm.movMemoryIndirect(1, 3); // R1 contains 10, so load MEM[10] which is 20

    // Test 6.3 Arithmetic
    vm.add(1, 2); // R2 = 10 + 10 = 20
    vm.sub(1, 2); // R2 = 20 - 10 = 10
    vm.mul(1, 2); // R2 = 10 * 10 = 100
    vm.movImmediate(2, 4);
    vm.div(4, 2); // R2 = 100 / 2 = 50

    // Test 6.4 Increment/Decrement
    vm.inc(2); // R2 = 50 + 1 = 51
    vm.dec(2); // R2 = 51 - 1 = 50

    vm.dump();
}

int main() {
    try {
        runner();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}