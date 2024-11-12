#include "lc3.hpp"
#include <iostream>
//execution cycle
//read instruction at pc
//incrment pc
//do shit

int main(){
    LCThree vm;
    vm.reset();
    //0001 010 000 000 001
    vm.memWrite(0x1401, 0x3000);
    vm.registers[vm.R0] = 0x1;
    vm.registers[vm.R1] = 0x5; 
    

    Word instr = vm.memRead();
    vm.add(instr);

    std::cout << instr << "\n";

    std::cout << vm.registers[vm.R2] << "\n"; //should be three

    return 0;
}