#include "function.h"

int ALU(ALUoper oper, int ALU0, int ALU1)
{
    if(oper.type == 'R') // R type
    {
        unsigned int i = ALU0;
        switch(oper.func)
        {
        case 0x20: // add
            rwae.num_over = OverFlow_add(ALU0, ALU1, ALU0 + ALU1);
            return ALU0 + ALU1;
        case 0x21: // addu
            return ALU0 + ALU1;
        case 0x22: // sub
            rwae.num_over = OverFlow_add(ALU0, -ALU1, ALU0 - ALU1);
            return ALU0 - ALU1;
        case 0x24: // and
            return ALU0 & ALU1;
        case 0x25: // or
            return ALU0 | ALU1;
        case 0x26: // xor
            return ALU0 ^ ALU1;
        case 0x27: // nor
            return ~(ALU0 | ALU1);
        case 0x28: // nand
            return ~(ALU0 & ALU1);
        case 0x2a: // slt
            return ALU0 < ALU1;
        case 0x00: // sll
            return ALU0 << ALU1;
        case 0x02: // srl
            i = ALU0;
            i = i >> ALU1;
            return i;
        case 0x03: // sra
            return ALU0 >> ALU1;
        }
    }
    else // I type
    {
        switch(oper.func)
        {
        case 0x08: // addi
            rwae.num_over = OverFlow_add(ALU0, ALU1, ALU0 + ALU1);
            return ALU0 + ALU1;
        case 0x09: // addiu
            return ALU0 + ALU1;
        case 0x23: // lw
            rwae.num_over = OverFlow_add(ALU0, ALU1, ALU0 + ALU1);
            return ALU0 + ALU1;
        case 0x21: // lh
            rwae.num_over = OverFlow_add(ALU0, ALU1, ALU0 + ALU1);
            return ALU0 + ALU1;
        case 0x25: // lhu
            rwae.num_over = OverFlow_add(ALU0, ALU1, ALU0 + ALU1);
            return ALU0 + ALU1;
        case 0x20: // lb
            rwae.num_over = OverFlow_add(ALU0, ALU1, ALU0 + ALU1);
            return ALU0 + ALU1;
        case 0x24: // lbu
            rwae.num_over = OverFlow_add(ALU0, ALU1, ALU0 + ALU1);
            return ALU0 + ALU1;
        case 0x2b: // sw
            rwae.num_over = OverFlow_add(ALU0, ALU1, ALU0 + ALU1);
            return ALU0 + ALU1;
        case 0x29: // sh
            rwae.num_over = OverFlow_add(ALU0, ALU1, ALU0 + ALU1);
            return ALU0 + ALU1;
        case 0x28: // sb
            rwae.num_over = OverFlow_add(ALU0, ALU1, ALU0 + ALU1);
            return ALU0 + ALU1;
        case 0x0f: // lui
            return ALU1 << 16;
        case 0x0c: // andi
            return ALU0 & ALU1;
        case 0x0d: // ori
            return ALU0 | ALU1;
        case 0x0e: // nori
            return ~(ALU0 | ALU1);
        case 0x0a: // slti
            return ALU0 < ALU1;
        }
    }
    return 0;
}
