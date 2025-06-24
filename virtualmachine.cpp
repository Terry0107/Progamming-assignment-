#include <iostream>
using namespace std;
   
// Flags - they update based on result of operations
 bool OF = false; // Overflow - when result > 127 or < -128
 bool UF = false; // Underflow - if result is below -128
 bool CF = false; // Carry - for carry in addition or borrow in subtatction
 bool ZF = false; // Zero - if result is 0 


 // Function to update all flags 
 void updateFlags(int result, int operan1 = 0, int operan2 = 0, string op = "" ) {
    ZF = (result == 0 ); // result zero?
    OF = (result > 127 || result < -128); // overflow check
    UF = (result < -128 ); // underflow check
    if (op == "ADD")CF = ((unsigned char )operan1 + (unsigned char )operan2 > 255);
    else if (op == "SUB") CF = ((unsigned char ) operan1 < (unsigned char ) operan2);
    else CF = false; // For MUL & DIV we dont care about carry 

 }

 


 // adds two registers and stores result in destination
 void ADD(char registers [], int src, int dst){
     int result = (int)registers[dst] + (int)registers[src];
     cout << "ADD R " << src << ", R " << dst << "=>";
     registers[dst] = (char)result;
     cout << (int)registers[dst] << endl;
     updateFlags(result, registers[src], registers[dst], "ADD");
 }      
   


 // subtract source from destination register
 void SUB(char registers[], int src, int dst){
   int result = (int)registers[dst] - (int)registers[src];
   cout << "SUB R" << src << ", R" << dst << "=>";
   registers[dst] = (char)result;
   cout << (int)registers[dst] << endl; 
   updateFlags(result, registers[dst], registers[src], "SUB");
 }
 
 // multiply two register values and store result 
  void MUL(char registers[], int src, int dst){
   int result = (int)registers[dst] * (int)registers[src];
   cout << "MUL R" << src << ", R" << dst << "=>";
   registers[dst] = (char)result;
   cout << (int)registers[dst] << endl;
   updateFlags(result);
  }
   
  // divide destination by source register if not zero
  void DIV(char registers[], int src, int dst){
   if (registers[src]== 0){
      cout << "DIV R" << src << ", R" << dst << "=> Error: Divide by zero" << endl;
      ZF = true; OF = UF = CF = false;
      return;
   }
   int result = (int)registers[dst] / (int)registers[src];
   cout << "DIV R" << src << ", R" << dst << "=>";
   registers[dst] = (char)result;
   cout << (int)registers[dst] << endl;
   updateFlags(result);
  }

  // just adds 1 to a register
   void INC (char registers[], int dst){
      int result = (int)registers[dst] + 1;
      cout <<  "INC R" << dst << "=>";
      registers[dst] = (char)result;
       cout << (int)registers[dst] << endl;
       updateFlags(result);
   }

   // just subtracts 1 from a register
   void DEC(char registers[], int dst){
      int result = (int)registers[dst] - 1;
      cout << "DEC R " << dst << "=>";
      registers[dst] = (char)result;
      cout << (int)registers[dst] << endl;
      updateFlags(result);
   }

   // shit left (basically multiply by 2)
   void SHL (char registers[], int dst) {
      unsigned char value = (unsigned char)registers[dst];
       CF = (value & 0x80)!= 0; // check leftmost bit  
       value <<= 1; 
       registers[dst] = (char)value;
       ZF = (registers[dst] ) == 0;
       OF = false;
       UF = false; 
       cout << "SHL R" << dst << "=>" << (int)registers[dst] << endl;

   }
     
    // shift right (basically divide by 2)
     void SHR (char registers[], int dst ){
      unsigned char value = (unsigned char) registers[dst];
      CF = (value & 0x01)!=0; // checl rightmost bit
      value >>=1;
      registers[dst] = (char)value;
       ZF = (registers[dst] == 0);
       OF = false;
       UF = false; 
       cout << "SHR R " << dst << "=>" << (int)registers[dst] << endl;
      
     }

    // rotate left (bit from left goes to right)
 void ROL (char registers[], int dst){
   unsigned char value = (unsigned char)registers[dst];
    CF = (value & 0x80)!=0;
    value = (value << 1) | (CF ? 1 : 0);
    registers[dst] = (char)value;
    ZF = (registers[dst] == 0);
    OF = false;
    UF = false; 
    cout << "ROL R" << dst << "=>" << (int)registers[dst] << endl;
   
 }

 //rotate right (bit from right goes to left )
 void ROR (char registers[], int dst ) {
   unsigned char value = (unsigned char)registers[dst];
   CF = (value & 0x01)!=0;
   value = (value >> 1 ) | (CF ? 0x80 : 0);
   registers[dst] = (char) value;
   ZF = (registers[dst]== 0 );
   OF = false; 
   UF = false; 
   cout << "ROR R" << dst << "=>" << (int)registers[dst] << endl;
 }
 


 // testing everything 
int main(){
    
   char reg [8] = {5,2,10,0,-128,127,1,0};

    cout << "\n-- ADD: R0 = R0 + R1 --\n";
    ADD (reg, 1,0);

    cout << "\n-- SUB: R0 = R0 - R2--\n";
    SUB (reg , 2,0);

    cout << "\n-- MUL :R2 = R2 * R1 --\n";
    MUL (reg, 1,2);

    cout << "\n-- SUB : R2 = R2 / R1 -- \n";
    DIV (reg, 1,2);

    cout << "\n-- INC: R3++ --\n";
    INC (reg, 3);

    cout << "\n-- DEC: R3-- -- \n";
    DEC (reg, 3);

    cout <<"\n-- INC on 127 (overflow case) -- \n";
    INC (reg, 5);

    cout << "\n-- DEC on -128 (underflow case) -- \n";
    DEC(reg,4);

    cout << "\n-- DIV BY ZERO(error) --\n";
    DIV (reg, 7,0);

    cout << "\n-- SHL R0 -- \n";
    SHL (reg, 0);

    cout << "\n-- SHR R0 --\n";
    SHR (reg, 0);

    cout << "\n-- ROL R0 --\n";
    ROL (reg, 0);

    cout << "\n-- ROR R0 --- \n";
    ROR (reg, 0);
 
    return 0;
}
 
