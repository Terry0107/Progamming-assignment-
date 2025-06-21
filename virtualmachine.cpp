#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main()
{
    cout << "Virtual Machine Simulator" << endl;
    char registers[8] = {0};

    registers[0] = 5;
    cout << "After MOV 5 , R0: R0 =" << (int)registers[0] << endl;

    registers[1] = 10;
    cout << "After MOV 10 , R1 : R1 = " << (int)registers[1] << endl;

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
    
    return 0;
}
