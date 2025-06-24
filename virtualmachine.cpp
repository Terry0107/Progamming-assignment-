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
    char operand1 = registers[0];
    char operand2_val = 3;
    int result_int = (int)operand1 - (int)operand2_val;
    
    registers[0] = (char)result_int;
    cout << "After SUB 3, R0 : R0 = " << (int) registers[0] << endl; 

    ZF = (registers[0] == 0); 

    OF = ((operand1 > 0 && operand2_val < 0 && registers[0] < 0 ) ||
       (operand1 < 0 && operand2_val > 0 && registers[0] > 0 )); 
    OF = (operand1 > 0 && result_int < -128) || 
       (operand1 < 0 && result_int > 127 );
    
    UF = (operand1 > 0 && registers[0] < 0 && (operand1 - operand2_val) < -128 );
    CF = ((unsigned char )operand1 < (unsigned char )operand2_val);
    displayFlags();
 }

 void executeSHL(char registers []){
    char original_r0 = registers[0];
    registers[0] = registers[0] << 1; 

     cout << "After SHL R0, 1 : R0 =" << (int) registers[0] << endl;

     ZF = (registers[0] == 0); 
     CF = ((original_r0 & 0x80)!= 0); 
     OF = (((original_r0 >>6) & 1) !=  ((registers[0] >> 6) & 1)); 
     OF = ((original_r0 < 0) != (registers[0] < 0 )) && ((original_r0 ^ registers[0] ) & 0x80); 
     OF = (((original_r0 & 0x40) >>6) != ((registers[0] & 0x80) >>7 ));
     UF = false;
     displayFlags();
     }

     void executeSHR(char registers[]){
        char original_r0 = registers[0]; 
        registers[0] = registers[0] >> 1;
         cout << "After SHR R0, 1 : R0 = " << (int) registers[0] << endl; 

         ZF = (registers[0] == 0); 
         CF = ((original_r0 & 0x01 ) !=0);
         OF = false;
         UF = false; 
         displayFlags();
 }

 void executeROL(char registers []){
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
    temp_val_ror = temp_val_ror | (wrapped_bit_ror << 7); 
    registers[0] = (char)temp_val_ror;
    cout <<"After ROR R0, 1 : R0 =" << (int)registers[0] << endl; 
    
    ZF = (registers[0] == 0); 
    CF = (wrapped_bit_ror != 0); 
    OF = false; 
    UF = false; 
    displayFlags();
 }

 void executeMUL(char registers[]){
    char operand1 = registers[0]; 
    char operand2_val = 2;

    int result_int = (int)operand1 * (int)operand2_val;
    registers[0] = (char)result_int;

     cout << "After MUL 2 , R0 : R0 =" << (int)registers[0] << endl; 

     ZF = (registers[0] ==0); 
     OF = (result_int > 127 || result_int < -128);
     UF = (operand1 < 0 && operand2_val > 0 && registers[0] > 0); 
     CF = (((unsigned char )operand1 *(unsigned char)operand2_val) > 255);
    displayFlags();

 }

 void executeDIV(char registers[]){
    char operand1 = registers[0]; 
    char operand2_val = 3; 
    
    if (operand2_val == 0){
        cout << "Error: Divison by zero!" << endl; 
      ZF = true; 
      OF = false;
      UF = false; 
      CF = false; 
      displayFlags();
      return; 
    }
    int result_int = (int)operand1 / (int)operand2_val;
    registers[0] = (char)result_int; 

       cout << "After DIV 3, R0 : R0 =" << (int)registers[0] << endl; 
      ZF = (registers[0] == 0);
      OF = false;
      UF = false;
      CF = false; 
      displayFlags();
}
 void executeINC(char registers[]){
    char original_r0 = registers[0];
    int result_int = (int)original_r0 + 1;

    registers[0] = (char)result_int;

     cout << "After INC R0 : R0 =" << (int)registers[0] << endl;
     ZF = (registers[0]==0);
     OF = (original_r0 == 127 && registers[0] == -128);
     UF = false;
     CF = (original_r0 == (char)255 );
     displayFlags();

 }
 
 void executeDEC(char registers[]){
    char original_r0 = registers[0];
    int result_int = (int)original_r0 - 1;

    registers[0] = (char)result_int;

    cout << "After DEC R0 : R0 =" << (int)registers[0]<< endl;
    ZF = (registers[0]==0);
    OF = (original_r0 == -128 && registers[0] == 127);
    UF = (original_r0 == -128 && registers[0] == 127);
    CF = (original_r0 == 0 && registers[0] == (char)255);
    displayFlags();
 }

int main(){
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
 cout << "\n--- Testing ADD with potential POSITIVE OVERFLOW --- \n";
 executeADD (registers); 

 registers[0] = -5;
 registers[1] = 5;
  cout << "\n--- Testing ADD with ZERO result --- \n"; 
 executeADD (registers); 

   registers[0] = -120; 
   registers[1] = -20;
   cout << "\n--- Testing ADD with potential NEGATIVE OVERFLOW ( UNDERFLOW) ---\n";
 executeADD (registers); 

 registers[0] = 15; 
 cout << "\n--- Testing SUB (R0 = 15, SUB 3) ---\n";
 executeSUB(registers);

 registers[0] = 0; 
  cout << "\n--- Testing SUB (R0=0, SUB 3) ---\n ";
  executeSUB(registers);

  registers[0] = -120;
  cout << "\n--- Testing SUB (R0=-127, SUB 3) ---\n ";
  executeSUB(registers);

 
 registers[0] = 50; 
    cout << "\n--- Testing MUL (R0=50, MUL 2) ---\n";
    executeMUL(registers);

 registers[0] = 70;
    cout << "\n--- Testing MUL (R0=70, MUL 2)---(Positive Overflow)\n";
    executeMUL(registers);

 registers[0] = -70; 
    cout << "\n--- Testing MUL (R0= -70, MUL 2)---(Negative overflow/underflow)\n";
    executeMUL(registers);

 registers[0] = 0; 
    cout << "\n--- Testing MUL (R0=0, MUL 2)---(Zero result)\n";
    executeMUL(registers);

 registers[0] =10; 
   cout << "\n--- Testing DIV (R0=10, DIV 0)---(Divison by zero)\n";
  executeDIV(registers);
  char original_r1 = registers[1];
  registers[1] = 0; 
  executeDIV(registers);
 registers[1] = original_r1;

    

 registers[0] = 5; 
   cout << "\n--- Testing INC (R0=5)---\n";
    executeINC(registers);

 registers[0] = 127; 
  cout << "\n--- Testing INC (R0=127)---(Positive Overflow)\n";
   executeINC(registers);

 registers[0] = -1;
  cout << "\n--- Testing INC (R0=-1)---(Zero Result) \n ";
   executeINC(registers);

 registers[0] = (char)255;
  cout << "\n--- Testin INC (R0 =(char) 255) --- (unsgined carry )\n";
    executeINC(registers);

 registers[0] = 5;
  cout << "\n--- Testing DEC (R0=5)---\n";
   executeDEC(registers);

 registers[0] = -128;
  cout << "\n--- Testing DEC (R0=-128)---(Negative Overflow)\n";
   executeDEC(registers);

 registers[0] = 1;
  cout << "\n --- Testing DEC (R0 = 1) --- (Zero Result)\n";
   executeDEC(registers);

 registers[0] = 0;
  cout << "\n--- Testing DEC (R0=0)--- (unsigned Carry/Borrow)\n";
   executeDEC(registers);


 cout << "\n--- Testing SHL (R0=5) ---\n"; 
 registers[0] = 5; 
 executeSHL(registers); 

 cout << "\n--- Testing SHL (R0= 64, POSITIVE overflow ) ---\n";
 registers[0] = 64; 
 executeSHL(registers); 

 cout << "\n--- Testing SHL (R0= -64, negative overflow ) ---\n";
 registers[0] = -64; 
 executeSHL(registers);


 cout <<  "\n--- Testing SHR (R0= 10) ---\n";
 registers[0] = 10; 
 executeSHR(registers);

 cout << "\n--- Testing SHR (R0=7) ---\n";
 registers[0] = 7;
 executeSHR(registers);

 cout << "\n--- Testing ROL (R0=5) ---\n";
 registers[0] = 5; 
 executeROL(registers);


 cout << "\n--- Testing ROL (R0=-128) ---\n";
 registers[0] = -128; 
 executeROL(registers);


 cout << "\n--- Testing ROR (R0=10) --- \n";
 registers[0] = 10; 
 executeROR(registers);

 cout << "\n--- Testing ROR (R0=1)--- \n";
 registers[0] = 1; 
 executeROR(registers); 


 char input_char; 
  cout << "Input for R0 (enter a single digit or character) : ?"; 
  cin >> input_char; 
 registers[0] = input_char; 
   cout << "After Input R0 : R0 = " << (int) registers[0] << " ( ASCII value)" << endl; 

   cout << "After OUT R0 : Displaying R0 value = " << (int) registers[0] << endl; 

    return 0;
}
 
