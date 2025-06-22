#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;


 bool OF = false; 
 bool UF = false;
 bool CF = false;
 bool ZF = false; 

 void displayFlags() {
    cout << "\n--- Current Flags state --- " << endl; 
    cout << "OF (Overflow): " << (OF ? "True" : "False") << endl; 
    cout << "UF (Underflow): " << (UF ? "True" : "False") << endl; 
    cout << "CF (Carry): " << (CF ? "True" : "False") << endl; 
    cout << "ZF (Zero): " << (ZF ? "True" : "False") << endl; 
    cout << "-----------------------\n " << endl; 

 }




 void executeADD(char registers []){
    char operand1 = registers[0];
    char operand2 = registers[1];
    int result_int= (int)operand1 + (int)operand2; 

 registers[0]= (char)result_int; 
    cout << "After ADD R1, R0 : R0 = " << (int) registers[0] << endl;

    ZF = (registers [0] == 0);
    OF = ((operand1 > 0 && operand2 > 0 && registers[0] < 0) ||( operand1 < 0 && operand2 < 0 < 0 && registers[0] > 0)); 
    UF = (operand1 < 0 && operand2 < 0 && registers[0] >= 0); 
    CF = (((unsigned char ) operand1 + (unsigned char)operand2)  > 255); 
    
    displayFlags(); 
    
 }      
   



 void executeSUB(char registers[] ){
 registers[0] = registers[0] - 3; 
    cout << "After SUB 3, R0 : R0 =" << (int) registers[0] << endl; 
 }

 void executeSHL(char registers []) {
    registers[0] = registers[0] << 1; 
     cout << "After SHL R0, 1 : R0 =" << (int) registers[0] << endl;

     ZF = (registers[0] == 0); 
     CF = ((original_r0 & 0x80)!= 0); 
     OF = (((original_r0 >>6) & 1) !=  ((registers[0] >> 6) & 1)); 
     OF = ((original_r0 < 0) != (registers[0] < 0 )) && ((original_r0 ^ registers[0] ) & 0x80); 
     OF = (((original_r0 & 0x40) >>6) != ((registers[0] & 0x80) >>7 ));
     UF = false;
     displayFlags();

     void executeSHR(char registers []){
        char original_r0 = registers[0]; 
        registers[0] = registers[0] >> 1;
         cout << "After SHR R0, 1 : R0 = " << (int) registers[0] << endl; 

         ZF = (registers[0] == 0); 
         CF = ((original_r0 & 0x01 ) !=0);
         OF = false;
         UF = false; 
         DisplayFlags ();
 }

 void executeROL (char registers []) {
    unsigned char temp_val_rol = (unsigned char ) registers[0];
    unsigned char wrapped_bit_rol = (temp_val_rol >> 7 ) & 1; 
    temp_val_rol = temp_val_rol << 1; 
    temp_val_rol = temp_val_rol| wrapped_bit_rol; 
    registers[0] = (char)temp_val_rol;
    cout << "After ROL R0, 1 : R0 =" << (int)registers[0] << endl; 

    ZF = (registers[0] == 0);
    CF = (wrapped_bit_rol != 0);    
    OF = false;
    UF = false; 
    displayFlags(); 
 }

 void executeROR (char registers[] ) {
    unsigned char temp_val_ror = (unsigned char) registers[0]; 
    unsigned char wrapped_bit_ror= temp_val_ror & 1;
    temp_val_ror = temp_val_ror >> 1; 
    temp_val_ror = temp_val_ror | (wrapped_bit_ ror << 7); 
    registers[0] = (char)temp_val_ror;
    cout <<"After ROR R0, 1 : R0 =" << (int)registers[0] << endl; 
    
    ZF = (registers[0] == 0); 
    CF = (wrapped_bit_ror != 0); 
    OF = false; 
    UF = false; 
    displayFlags();
 }


int main() {

    cout << "Virtual Machine Simulator" << endl;
    char registers[8] = {0};
    
    displayFlags();

    registers[0] = 5;
    cout << "After MOV 5 , R0: R0 =" << (int)registers[0] << endl;

   registers[1] = 10;
    cout << "After MOV 10 , R1 : R1 = " << (int)registers[1] << endl;

 executeADD (registers); 
 registers[0] = 120; 
 registers[1] = 20; 
 cout << "\n--- Testing ADD with potential POSITIVE OVERFLOW --- \n"
 executeADD (registers); 
 registers[0] = -5;
 registers[1] = 5;
  cout << "\n--- Testing ADD with ZERO result --- \n"; 
 executeADD (registers); 
   registers[0] = -120; 
   registers[1] = -20
   cout << "\n--- Testing ADD with potential NEGATIVE OVERFLOW ( UNDERFLOW) ---\n"
 executeADD (registers); 
 registers[0] = 15; 
 cout << "\nAfter restting R0 to 15\n" displayFlags();
 
 executeSUB (registers);


 
 registers[0] = registers[0] * 2;
    cout << "After MUL 2 , R0 : R0 = "<<(int)registers[0] << endl;

 registers[0] = registers[0] / 3;
    cout << "After DIV 3 , R0 : R0 = "<<(int)registers[0] << endl;

 registers[0] = registers[0] + 1;
    cout << "After INC R0 : R0 = " <<(int)registers[0] << endl;

 registers[0] = registers[0] - 1;
    cout << "After DEC R0 : R0 = " <<(int)registers[0] << endl;

 executeSHL(registers);  
 registers[0] = 5; 
  cout << "\n--- Testing SHL (R0=5) ---\n";
 executeSHL(registers); 
 registers[0] = 64; 
  cout << "\n--- Testing SHL (R0= 64, POSITIVE overflow ) ---\n";
 executeSHL(registers); 
 registers[0] = -64; 
  cout << "\n--- Testing SHL (R0= -64, negative overflow ) ---\n";
 executeSHL(registers);
 registers[0] = 10; 
 cout <<  "\n--- Testing SHR (R0= 10) ---\n";
 executeSHR(registers);
 registers[0] = 7;
 cout << "\n--- Testing SHR (R0=7) ---\n";
 executeSHR(registers);
 register[0] = 5; 
 cout << "\n--- Testing ROL (R0=5) ---\n";
 executeROL(registers);
 register[0] = -128; 
 cout << "\n--- Testing ROL (R0=-128) ---\n";
 executeROL(registers);
 registers[0] = 10; 
 cout << "\n--- Testing ROR (R0=10) --- \n";
 executeROR(registers);
 registers[0] = 1; 
 cout << "\n--- Testing ROR (R0=1)--- \n";
 executeROR(registers); 


 char input_char; 
  cout << "Input for R0 (enter a single digit or character) : ?"; 
  cin >> input_char; 

 registers[0] = input_char; 
   cout << "After Input R0 : R0 = " << (int) registers[0] << " ( ASCII value)" << endl; 

   cout << "After OUT R0 : Displaying R0 value = " << (int) registers[0] << endl; 

    return 0;

}
