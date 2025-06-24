#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#include <stdexcept>
using namespace std;

struct VirtualMachine {
    // Registers and memory
    char R[8] = {0};     // R0-R7 initialized to 0
    char MEM[64] = {0};  // 64 bytes of memory initialized to 0
    unsigned char PC = 0; // Program Counter initialized to 0
    
    // Flags
    bool CF = false; // Carry Flag
    bool OF = false; // Overflow Flag
    bool UF = false; // Underflow Flag
    bool ZF = false; // Zero Flag

    // Initialization functions (from teammate's code)
    void initPC() { PC = 0; }
    void initFlags() { CF = OF = UF = ZF = false; }
    void initRegs() {
        for (char &val : R) {
            val = 0;
        }
    }
    void initMEM() {
        for (char &val : MEM) {
            val = 0;
        }
    }
    void initAll() {
        initFlags();
        initPC();
        initMEM();
        initRegs();
    }

    // 6.1 I/O Operations (from teammate's code with improvements)
    void updateFlags() {
        // More comprehensive flag update that considers all registers
        // This will be called after operations that affect flags
        ZF = true;
        for (int i = 0; i < 8; i++) {
            if (R[i] != 0) {
                ZF = false;
                break;
            }
        }
        
        // OF/UF are set by individual operations
        // CF is set when there's overflow or underflow
        CF = OF || UF;
    }

    void input(int regIndex) {
        int ch;
        cout << "? ";
        cin >> ch;
        R[regIndex] = static_cast<char>(ch);
        
        // Update flags based on input
        OF = (R[regIndex] > 127);
        UF = (R[regIndex] < -128);
        ZF = (R[regIndex] == 0);
        CF = OF || UF;
    }

    void output(int regIndex) {
        cout << "Output: " << static_cast<int>(R[regIndex]) << endl;
    }

    // 6.2 MOV Operations (from teammate's code with improvements)
    void movImmediate(char value, int destReg) {
        R[destReg] = value;
        // Immediate moves don't typically affect flags unless specified
    }

    void movRegister(int srcReg, int destReg) {
        R[destReg] = R[srcReg];
        // Register moves don't typically affect flags unless specified
    }

    void movMemoryIndirect(int addrReg, int destReg) {
        R[destReg] = MEM[static_cast<unsigned char>(R[addrReg])];
        // Memory moves don't typically affect flags unless specified
    }

    // Memory operations (from teammate's code with improvements)
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

    // 6.3 Arithmetic Operations (our implementation)
    void add(int srcReg, int destReg) {
        int result = R[destReg] + R[srcReg];
        R[destReg] = static_cast<char>(result);
        
        // Update flags
        OF = (result > 127);
        UF = (result < -128);
        ZF = (R[destReg] == 0);
        CF = OF || UF;
    }

    void sub(int srcReg, int destReg) {
        int result = R[destReg] - R[srcReg];
        R[destReg] = static_cast<char>(result);
        
        // Update flags
        OF = (result > 127);
        UF = (result < -128);
        ZF = (R[destReg] == 0);
        CF = OF || UF;
    }

    void mul(int srcReg, int destReg) {
        int result = R[destReg] * R[srcReg];
        R[destReg] = static_cast<char>(result);
        
        // Update flags
        OF = (result > 127);
        UF = (result < -128);
        ZF = (R[destReg] == 0);
        CF = OF || UF;
    }

    void div(int srcReg, int destReg) {
        if (R[srcReg] == 0) {
            throw runtime_error("Division by zero");
        }
        int result = R[destReg] / R[srcReg];
        R[destReg] = static_cast<char>(result);
        
        // Update flags
        OF = (result > 127);
        UF = (result < -128);
        ZF = (R[destReg] == 0);
        CF = OF || UF;
    }

    // 6.4 Increment/Decrement Operations (our implementation)
    void inc(int destReg) {
        int result = R[destReg] + 1;
        R[destReg] = static_cast<char>(result);
        
        // Update flags
        OF = (result > 127);
        UF = (result < -128);
        ZF = (R[destReg] == 0);
        CF = OF || UF;
    }

    void dec(int destReg) {
        int result = R[destReg] - 1;
        R[destReg] = static_cast<char>(result);
        
        // Update flags
        OF = (result > 127);
        UF = (result < -128);
        ZF = (R[destReg] == 0);
        CF = OF || UF;
    }

    // Display VM state
    void displayState() {
        cout << "Registers: ";
        for (int i = 0; i < 8; i++) {
            cout << "R" << i << "=" << static_cast<int>(R[i]) << " ";
        }
        cout << "\nFlags: CF=" << CF 
             << " OF=" << OF 
             << " UF=" << UF 
             << " ZF=" << ZF 
             << "\nPC: " << static_cast<int>(PC) << "\n";
    }
};

// Function to parse register index
int parseRegister(const string& reg) {
    if (reg.size() != 2 || reg[0] != 'R') {
        throw runtime_error("Invalid register format");
    }
    int regNum = reg[1] - '0';
    if (regNum < 0 || regNum > 7) {
        throw runtime_error("Register number out of range");
    }
    return regNum;
}

// Function to parse immediate value
char parseImmediate(const string& val) {
    return static_cast<char>(stoi(val));
}

// Function to parse memory address
int parseAddress(const string& addr) {
    if (addr[0] == '[' && addr.back() == ']') {
        return stoi(addr.substr(1, addr.size() - 2));
    }
    return stoi(addr);
}

// Main instruction execution function
void executeInstruction(VirtualMachine& vm, const string& instruction) {
    istringstream iss(instruction);
    string opcode;
    iss >> opcode;
    
    if (opcode == "INPUT") {
        string dest;
        iss >> dest;
        vm.input(parseRegister(dest));
    }
    else if (opcode == "OUT") {
        string src;
        iss >> src;
        vm.output(parseRegister(src));
    }
    else if (opcode == "MOV") {
        string src, dest;
        char comma;
        iss >> src >> comma >> dest;
        
        if (src[0] == 'R') {
            // MOV R1, R0
            vm.movRegister(parseRegister(src), parseRegister(dest));
        } 
        else if (src[0] == '[') {
            // MOV [R1], R0
            vm.movMemoryIndirect(parseRegister(src.substr(1, src.size() - 2)), 
                              parseRegister(dest));
        }
        else {
            // MOV 10, R0
            vm.movImmediate(parseImmediate(src), parseRegister(dest));
        }
    }
    else if (opcode == "ADD") {
        string src, dest;
        char comma;
        iss >> src >> comma >> dest;
        vm.add(parseRegister(src), parseRegister(dest));
    }
    else if (opcode == "SUB") {
        string src, dest;
        char comma;
        iss >> src >> comma >> dest;
        vm.sub(parseRegister(src), parseRegister(dest));
    }
    else if (opcode == "MUL") {
        string src, dest;
        char comma;
        iss >> src >> comma >> dest;
        vm.mul(parseRegister(src), parseRegister(dest));
    }
    else if (opcode == "DIV") {
        string src, dest;
        char comma;
        iss >> src >> comma >> dest;
        vm.div(parseRegister(src), parseRegister(dest));
    }
    else if (opcode == "INC") {
        string dest;
        iss >> dest;
        vm.inc(parseRegister(dest));
    }
    else if (opcode == "DEC") {
        string dest;
        iss >> dest;
        vm.dec(parseRegister(dest));
    }
    else {
        throw runtime_error("Unknown instruction: " + opcode);
    }
    
    // Increment program counter
    vm.PC++;
}

// Function to execute a program
void executeProgram(VirtualMachine& vm, const vector<string>& program) {
    vm.initAll(); // Reset VM state
    
    while (vm.PC < program.size()) {
        cout << "Executing: " << program[vm.PC] << endl;
        executeInstruction(vm, program[vm.PC]);
        vm.displayState();
        cout << "-----------------" << endl;
    }
}

// Test function
void testAllOperations() {
    VirtualMachine vm;
    
    // Test all operations
    cout << "Testing all operations:\n";
    vector<string> program = {
        // 6.1 I/O operations
        "INPUT R0",          // Input a value
        "OUT R0",            // Output the value
        
        // 6.2 MOV operations
        "MOV 10, R1",        // Immediate to register
        "MOV R1, R2",        // Register to register
        "MOV 20, R3",
        "STORE R3, 10",      // Store to memory (using teammate's store function)
        "MOV [10], R4",      // Memory to register
        
        // 6.3 Arithmetic operations
        "ADD R1, R2",        // R2 = R2 + R1 (10 + 10 = 20)
        "SUB R1, R2",        // R2 = R2 - R1 (20 - 10 = 10)
        "MUL R1, R2",        // R2 = R2 * R1 (10 * 10 = 100)
        "MOV 2, R5",
        "DIV R5, R2",        // R2 = R2 / R5 (100 / 2 = 50)
        
        // 6.4 Increment/Decrement
        "INC R2",            // R2 = 51
        "DEC R2",            // R2 = 50
    };
    
    executeProgram(vm, program);
}

int main() {
    try {
        testAllOperations();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}