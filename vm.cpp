#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

////////////// Registers and memory //////////////
struct VirtualMachine {
   char R[8] = {0};
   char MEM[64] = {0};
   unsigned char PC = 0;

////////////// Flags //////////////
   bool CF = false;
   bool OF = false;
   bool UF = false;
   bool ZF = false;

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

////////////// 6.3 Arithmetic Operations - ALICE //////////////
void add(int srcReg, int destReg) {
   int result = R[destReg] + R[srcReg];
   R[destReg] = static_cast<char>(result);

   // Update flags 
   OF = (result > 127);
   UF = (result < -128);
   ZF = (R[destReg] ==0);
   CF = OF || UF ;
}

void sub(int srcReg, int destReg) {
   int result = R[destReg] - R[srcReg];
   R[destReg] = static_cast<char>(result);

   // Update flags 
   OF = (result > 127);
   UF = (result < -128);
   ZF = (R[destReg] ==0);
   CF = OF || UF ;
}

void mul(int srcReg, int destReg) {
   int result = R[destReg] * R[srcReg];
   R[destReg] = static_cast<char>(result);

   // Update flags 
   OF = (result > 127);
   UF = (result < -128);
   ZF = (R[destReg] ==0);
   CF = OF || UF ;
}

void div(int srcReg, int destReg) {
   if(R[srcReg] == 0){
      throw runtime_error("Division by zero");
   }
   int result = R[destReg] / R[srcReg];
   R[destReg] = static_cast<char>(result);

   // Update flags 
   OF = (result > 127);
   UF = (result < -128);
   ZF = (R[destReg] ==0);
   CF = OF || UF ;
}

////////////// 6.4 Increment and Decrement Operations - ALICE //////////////
void inc(int destReg) {
   int result = R[destReg] + 1;
   R[destReg] = static_cast<char>(result);

   // Update flags 
   OF = (result > 127);
   UF = (result < -128);
   ZF = (R[destReg] ==0);
   CF = OF || UF ;
}

void dec(int destReg) {
   int result = R[destReg] - 1;
   R[destReg] = static_cast<char>(result);

   // Update flags 
   OF = (result > 127);
   UF = (result < -128);
   ZF = (R[destReg] ==0);
   CF = OF || UF ;
}




}
