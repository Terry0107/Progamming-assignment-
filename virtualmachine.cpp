#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main () {
    cout << "Virtual Machine Simulator" << endl;
    char registers[8] = {0}; 

    registers[0] = 5; 
    cout << "After MOV 5 , R0: R0 =" << (int) registers[0] << endl;

    registers[1] = 10;
    cout << "After MOV 10 , R1 : R1 = " << (int) registers[1] << endl;
    
    registers[0] = registers[0] + registers[1];
    cout << "After ADD R1, R0: R0 =" << (int) registers[0] << endl;

    registers[0] = registers[0] - 3; 
    cout << "After SUB 3, R0: R0 = " << (int) registers[0] << endl;

}
