#include "lc3.hpp"

Word LCThree::memPeek(){
    return memory[PC];
}

Word LCThree::memRead(){
    return memory[PC++];
}

void LCThree::memWrite(Word w,Word addr){
    memory[addr] = w;
}

void LCThree::reset(){

    //zero out the memory
    for(Word& w : memory){
        w = 0x0000;
    }

    PC = 0x3000; // reset the PC
    registers[FLAGS] = FL_ZRO; // set zero flag
}

Word LCThree::sign_extend(Word x, int bit_count)
{
    if ((x >> (bit_count - 1)) & 1) {
        x |= (0xFFFF << bit_count);
    }
    return x;
}


void LCThree::add(Word& instr){
    Byte dr = (instr >> 9) & 0x7;
    Byte sr1 = (instr >> 6) & 0x7;
    Byte imm = (instr >> 5) & 0x1;

    if(imm){
         Word operand = sign_extend(instr & 0x1F, 5);
         registers[dr] = registers[sr1] + operand;

    }else{
        Byte sr2 = instr & 0x7;
        registers[dr] = registers[sr1] + registers[sr2];
    }

}



void LCThree::op_and(Word& instr){
    Byte dr = (instr >> 9) & 0x7;
    Byte sr1 = (instr >> 6) & 0x7;
    Byte imm = (instr >> 5) & 0x1;

    if(imm){
         Word operand = sign_extend(instr & 0x1F, 5);
         registers[dr] = registers[sr1] & operand;

    }else{
        Byte sr2 = instr & 0x7;
        registers[dr] = registers[sr1] & registers[sr2];
    }
}

void LCThree::br(Word& instr){
    Word cond = (instr >> 9) & 0x7;
    Word offset = sign_extend(instr & 0x1FF, 9);
    if(cond & FLAGS){
        PC += offset;
    }    
}

void LCThree::jmp(Word& instr){
    Word base_r = (instr >> 6) & 0x7;
    PC = registers[base_r];
}

void LCThree::ret(Word& instr){
    PC = registers[R7];
}

void LCThree::jsr(Word& instr){
    registers[R7] = PC;
    Word bit_11 = (instr >> 11) & 0x1;

    if(bit_11){
        PC += sign_extend(instr & 0x7FF, 11); 
    }else{
        Word base_r = (instr >> 6) & 0x7;
        PC = base_r;
    }
}

void LCThree::ld(Word& instr){
    Word dr = (instr >> 9) & 0x7;
    Word pc_offset = instr & 0x100;

    Word addr = PC + sign_extend(instr & 0x1FF, 9);
    registers[dr] = memory[addr];
    setcc(dr);
}

void LCThree::ldi(Word& instr){
    Word dr = (instr >> 9) & 0x7;
    Word pc_offset = instr & 0x100;

    Word addr0 = PC + sign_extend(instr & 0x1FF, 9);
    Word addr = memory[addr0];
    registers[dr] = memory[addr];
    setcc(dr); 
}

void LCThree::ldr(Word& instr){
    Word dr = (instr >> 9) & 0x7;
    Word baseR = (instr >> 6) & 0x7;
    Word pc_offset = instr & 0x100;

    Word addr = registers[baseR] + sign_extend(instr & 0x3f, 6); 

    registers[dr] = memory[addr];
    setcc(dr);


}

void LCThree::lea(Word& instr){
    Word dr = (instr >> 9) & 0x7;
    Word pc_offset = instr & 0x100;

    Word addr = PC + sign_extend(instr & 0x1FF, 9);
    registers[dr] = addr;
    setcc(dr);
}

void LCThree::op_not(Word& instr){
    Word dr = (instr >> 9) & 0x7;
    Word sr = (instr >> 6) & 0x7;

    registers[dr] = ~registers[sr];
    setcc(dr);
}

void LCThree::ret(Word& instr){
    PC = memory[R7];
}

void LCThree::setcc(uint8_t reg){
    if(registers[reg] < 0){
        registers[FLAGS] &= 0x4;
    }
    else if(registers[reg] == 0){
        registers[FLAGS] &= 0x2;
    }
    else{
        registers[FLAGS] &= 0x1;
    }

    
}

void LCThree::run(){

    bool running = true;

    while(running){
        Word instr = memRead();
        Byte op = instr >> 12;

        switch(op){
            case OP_ADD:
                add(instr);
                break;
            case OP_AND:
                op_and(instr);
                break;
            case OP_BR:
                op_and(instr);
                break;
            case OP_JMP:
                jmp(instr);
                break;
            case OP_JSR:
                jsr(instr);
                break;
            case OP_LD:
                ld(instr);
                break;
            case OP_LDI:
                ldi(instr);
                break;
            case OP_LDR:
                ldr(instr);
                break;
            case OP_LEA:
                lea(instr);
                break;
            case OP_NOT:
                op_not(instr);
                break;
            
            
        }
    }
}