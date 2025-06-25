#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

struct VirtualMachine
{
    char R[8]; // char (-128..127)
    char PC = 0;
    bool CF, OF, UF, ZF;
    char MEM[64];

    void initPC();
    void initFlags();
    void initRegs();
    void initMEM();
    void initAll();
    void Flags();
    void store(int, char);
    char load(int address);
    void input(int index);
    void runner();
    int main();


};

#endif