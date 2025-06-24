#include <iostream>
using namespace std;

struct VirtualMachine
{
    char R[8]; // char (-128..127)
    char PC = 0;
    bool CF, OF, UF, ZF;
    char MEM[64];

    void initPC() { PC = 0; }
    void initFlags() { CF = OF = UF = ZF = false; }
    void initRegs()
    {
        for (char &val : R)
        {
            val = 0;
        }
    }

    void initMEM()
    {
        for (char &val : MEM)
        {
            val = 0;
        }
    }

    void initAll()
    {
        initFlags();
        initPC();
        initMEM();
        initPC();
    }
    //Part 6.1
    void Flags()
    {
        if (R[0] > 127)
            OF = true;
        else
            OF = false;

        if (R[0] < -128)
            UF = true;
        else
            UF = false;

        if (R[0] == 0)
            ZF = true;
        else
            ZF = false;  
    }

    void store(int address, char val)
    {
        // do bound checking and overflow and zero
        MEM[address] = val;
    }

    char load(int address)
    {
        // checking
        return MEM[address];
    }

    void input(int index)
    {
        int ch;
        cout << "?";
        cin >> ch;
        R[index] = ch;
    }
};

// MOV 28, R0
// ROL R0, 1
// 0 0 0 1 1 1 0 0  == 28
// 0 0 1 1 1 0 0 0  ==>> rotated value
// covert back into decimal and store into R[0]

// a function to execute the assembly program
void runner()
{
    VirtualMachine vm;
    vm.initAll();
    vm.input(0);//Part 6.1
    vm.Flags();//Part 6.1
    cout << "R[0] = " << int(vm.R[0]) << endl;//Part 6.1
    vm.R[0] = 10;//Part6.2
    vm.R[0] = vm.R[1];//Part6.2
    vm.R[3] = vm.load(vm.R[1]); //Part6.2


    

    

    



    // loop to read asm file and execute each line
    // split the line into 3 components
    // 1. command
    // 2. operand1
    // 3. operand2
}

int main()
{
    runner();
    return 0;
}

// MOV 10, R0   ==> R[0] = 10
// MOV 10, R1   ==> R[1] = 10
// SUB R0, R1   ==> R[1] = R[1] - R[0] ==> 10-10 = 0
//      causes the ZF = true

// MOV 200, R0   ==> overflow  ==> R0 (-128..127)
//    OF = true and CF = true

// MOV -300, R1
// MOV 10, R2
// ADD R1, R2 ==> -300 + 10 = -290 ===> store in R[2]
//     -290 < -128 ==> therfore it is underflow
//     UF = true;

// MOV