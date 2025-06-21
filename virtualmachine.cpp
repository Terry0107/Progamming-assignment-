#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;


void executeADD(char registers []){
    registers[0]= registers[0] + registers[1];
     cout << "After ADD R1, R0 : R0 = " << (int) registers[0] << endl;

}


int main()
{
    cout << "Virtual Machine Simulator" << endl;
    char registers[8] = {0};

    registers[0] = 5;
    cout << "After MOV 5 , R0: R0 =" << (int)registers[0] << endl;

 registers[1] = 10;
    cout << "After MOV 10 , R1 : R1 = " << (int)registers[1] << endl;

 executeADD (registers); 

 registers[0] = registers[0] + registers[1];
    cout << "After ADD R1, R0: R0 =" <<(int)registers[0] << endl;

 registers[0] = registers[0] - 3;
    cout << "After SUB 3, R0: R0 = " <<(int)registers[0] << endl;

 registers[0] = registers[0] * 2;
    cout << "After MUL 2 , R0 : R0 = "<<(int)registers[0] << endl;

 registers[0] = registers[0] / 3;
    cout << "After DIV 3 , R0 : R0 = "<<(int)registers[0] << endl;

 registers[0] = registers[0] + 1;
    cout << "After INC R0 : R0 = " <<(int)registers[0] << endl;

 registers[0] = registers[0] - 1;
    cout << "After DEC R0 : R0 = " <<(int)registers[0] << endl;

 registers[0] = registers[0] << 1;
    cout << "After SHL R0 : R0 " << (int)registers[0] << endl; 

 registers[0] = registers [0] >> 1; 
    cout << "After SHR R0, 1 : R0 =" << (int)registers[0] << endl;

    unsigned char temp_val = (unsigned char) registers[0];
    unsigned char wrapped_bit = (temp_val >> 7) & 1;
    temp_val= temp_val << 1;
    temp_val = temp_val | wrapped_bit; 
    
    
 registers[0] = (char)temp_val;
    cout << "After ROL R0 , 1 : R0 = " << (int)registers[0] << endl; 



    unsigned char temp_val_ror = (unsigned char) registers[0];
    unsigned char wrapped_bit_ror = temp_val_ror & 1;
    temp_val_ror = temp_val_ror >> 1;
    temp_val_ror = temp_val_ror | (wrapped_bit_ror << 7); 

 registers[0] = (char) temp_val_ror; 
  cout << "After ROR R0, 1 : R0 = " << (int)registers[0] << endl; 

 char input_char; 
  cout << "Input for R0 (enter a single digit or character) : ?"; 
  cin >> input_char; 

 registers[0] = input_char; 
   cout << "After Input R0 : R0 = " << (int) registers[0] << " ( ASCII value)" << endl; 

   cout << "After OUT R0 : Displaying R0 value = " << (int) registers[0] << endl; 

    return 0;
}
