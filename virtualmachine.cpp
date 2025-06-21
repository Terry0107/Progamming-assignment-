#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main () {
    cout << "Virtual Machine Simulator" << endl;
    int registers[8] = {0}; 

    registers[0] = 5; 
    cout << "After MOV 5 , R0: R0 =" << registers[0] << endl;

    registers[1] = 10;
    cout << "After MOV 10 , R1 : R1 = " << registers[1] << endl;
    
}
