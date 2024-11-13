#ifndef LC3_HPP
#define LC3_HPP

#include <cstdint>
#define MEM_SIZE 1 << 16


using Byte = unsigned char;
using Word = uint16_t;


class LCThree{
public:
 
    Word memPeek();
    Word memRead();
    void memWrite(Word w,Word addr);


    void run();
    void reset();


//private:
   //registers
    Word registers[9];
    Word PC;

    //memory
    Word memory[MEM_SIZE];

    //opcodes
    typedef enum
    {
        OP_BR = 0, /* branch */
        OP_ADD,    /* add  */
        OP_LD,     /* load */
        OP_ST,     /* store */
        OP_JSR,    /* jump register */
        OP_AND,    /* bitwise and */
        OP_LDR,    /* load register */
        OP_STR,    /* store register */
        OP_RTI,    /* unused */
        OP_NOT,    /* bitwise not */
        OP_LDI,    /* load indirect */
        OP_STI,    /* store indirect */
        OP_JMP,    /* jump */
        OP_RES,    /* reserved (unused) */
        OP_LEA,    /* load effective address */
        OP_TRAP    /* execute trap */
    } Opcode;

    enum{
        R0,
        R1,
        R2,
        R3,
        R4,
        R5,
        R6,
        R7,
        FLAGS,
    };

    const Word FL_POS = 1 << 0; /* P */
    const Word FL_ZRO = 1 << 1; /* Z */
    const Word FL_NEG = 1 << 2; /* N */

    void add(Word& instr);
    void op_and(Word& instr);
    void br(Word& instr);
    void jmp(Word& instr);
    void ret(Word& instr);
    void jsr(Word& instr);
    void ld(Word& instr);
    void ldi(Word& instr);
    void ldr(Word& instr);
    void lea(Word& instr);
    void op_not(Word& instr);
    void ret(Word& instr);
    void setcc(uint8_t reg);
    Word sign_extend(Word x, int bit_count);



};
#endif //LC3_HPP