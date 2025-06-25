#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

//These are the flags to tell us if something special happened
struct Flags {
    bool OF = false; //Overflow happened (we're not using this now)
    bool UF = false; //Underflow happened (also not using it now)
    bool CF = false; //Carry flag (bit was pushed out)
    bool ZF = false; //Zero flag (result is 0)
};

//This function helps clean up a line by removing spaces before and after
string trim(const string &s) {
      size_t start = s.find_first_not_of(" \t\r\n"); //find where the actual text starts and ends
      size_t end = s.find_last_not_of(" \t\r\n"); //if we found nothing, return an empty string

      return (start == string::npos ? "" : s.substr(start, end - start + 1));
   }


class VirtualMachine {
    vector<char> R;       //We have 8 simple registers, R0 to R7
    Flags F;              //These are our result flags
    vector<string> prog;  //This stores the lines of our .asm file
    size_t PC = 0;        //PC = Program Counter, keeps track of which line we're on

public:
    VirtualMachine() : R(8, 0) {} //When we start, all registers are set to 0

    //This function loads our .asm program from a file
    bool load(const string &filename) {
        ifstream in(filename);
        if (!in) return false; //If the file doesn't open, return false
        string line;
        while (getline(in, line)) {
            line = trim(line);              //remove extra spaces
            if (!line.empty()) prog.push_back(line); //only keep lines with content
        }
        return true;
    }

    //This function runs the program line by line
    void run() {
        PC = 0;
        while (PC < prog.size()) {
            execLine(prog[PC]); //execute one line
            PC++;               //go to the next line
        }
    }

    //This prints what's inside our registers and the flags after running
    void dump() {
        cout << "Registers: ";
        for (char c : R) cout << (int)c << ' ';
        cout << "\nFlags: ZF=" << F.ZF << " CF=" << F.CF << " OF=" << F.OF << " UF=" << F.UF << "\n\n";

    }

public:
    //This handles one instruction like SHL, ROL, etc.
    void execLine(const string &line) {
        stringstream ss(trim(line)); //clean up the line
        string cmd;
        ss >> cmd;
        for (char &c : cmd) c = toupper(c); //make sure command is in capital letters

        //We only support these instructions for now
        if (cmd == "SHL" || cmd == "SHR" || cmd == "ROL" || cmd == "ROR") {
            shiftOrRotate(ss, cmd);
        } else {
            cout << "This part of the VM only supports SHL, SHR, ROL, ROR commands.\n";

        }
    }

    //This function does the actual shifting or rotating
    void shiftOrRotate(stringstream &ss, const string &op) {
        string reg;
        char comma;
        int count;
        ss >> reg >> comma >> count; //something like: R0, 1

        int r = reg[1] - '0';         //Get the register number from R0, R1, etc.
        unsigned char value = R[r];   //Get the value stored in that register

        if (op == "SHL") {
            F.CF = value & 0x80;      //Check if the leftmost bit is 1
            value <<= 1;              //Shift all bits to the left
        } else if (op == "SHR") {
            F.CF = value & 0x01;      //Check if the rightmost bit is 1
            value >>= 1;              //Shift all bits to the right
        } else if (op == "ROL") {
            F.CF = value & 0x80;      //Save the bit that might fall off
            value = value << 1;       //Shift left
            if (F.CF) value |= 0x01;  //If we had a 1 fall off, put it back on the right
        } else if (op == "ROR") {
            F.CF = value & 0x01;      //Save the bit that might fall off
            value = value >> 1;       //Shift right
            if (F.CF) value |= 0x80;  //If we had a 1 fall off, put it back on the left
        }

        R[r] = value;           //Save the new value in the register
        F.ZF = (R[r] == 0);     //Set the zero flag if result is 0
        F.OF = false;           //Overflow and underflow are not needed now
        F.UF = false;

        //Print what just happened
        cout << op << " " << reg << " = " << (int)R[r] << " | CF = " << F.CF << endl;
    }
};

int main() {
    VirtualMachine vm;

    //Try loading the code from a file
    if (vm.load("sample1.asm")) {
        vm.run();     //run all the code
        vm.dump();    //print final result
    } else {
        //If no file found, we just test some sample lines
        cout << "Couldn't find 'sample1.asm' So running a few test lines:\n";

         // These are the test instructions we will try 
        vector<string> test = {
            "MOV 2, R0",
            "SHL R0, 1",
            "ROL R0, 1"
        };

        for (string line : test) {
            vm.execLine(line); //run each line one by one
        }

        vm.dump(); //show what’s inside the registers and flags now
    }

    return 0;
   }