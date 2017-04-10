#include "function.h"

unsigned int GetLineN(int n, FILE *fp)
{
    fpos_t pos = n;
    unsigned int buff;

    fsetpos(fp,&pos);
    fread(&buff,sizeof(unsigned int),1,fp);

    return Little2Big(buff);
}

unsigned int Little2Big(unsigned int i)
{
    unsigned int b0,b1,b2,b3;

    b0 = (i & 0x000000ff) << 24u;
    b1 = (i & 0x0000ff00) << 8u;
    b2 = (i & 0x00ff0000) >> 8u;
    b3 = (i & 0xff000000) >> 24u;

    return b0 | b1 | b2 | b3;
}

unsigned int GetOpcode(unsigned int i)
{
    return (i >> 26) & 0x0000003f;
}

unsigned int Get_rs(unsigned int i)
{
    return (i >> 21) & 0x0000001f;
}

unsigned int Get_rt(unsigned int i)
{
    return (i >> 16) & 0x0000001f;
}

unsigned int Get_rd(unsigned int i)
{
    return (i >> 11) & 0x0000001f;
}

unsigned int Get_C(unsigned int i)
{
    return (i >> 6) & 0x0000001f;
}

unsigned int Get_func(unsigned int i)
{
    return i & 0x0000003f;
}

short int Get_i(unsigned int i)
{
    return (i & 0x0000ffff);
}

int Get_addr(unsigned int i)
{
    return (i & 0x03ffffff);
}

ALUoper Get_oper(unsigned int opcode, unsigned int func)
{
    ALUoper oper;
    if(opcode == 0x00)
    {
        oper.type = 'R';
        oper.func = func;
    }
    else
    {
        oper.type = 'I';
        oper.func = opcode;
    }
    return oper;
}

void Instruction_Detect(unsigned int opcode, unsigned int func)
{
    if(opcode == 0x00) // R
    {
        if(func == 0x20) // add
        {
            ID_c[0] = 'A';
            ID_c[1] = 'D';
            ID_c[2] = 'D';
            ID_c[3] = '\0';
        }
        else if(func == 0x21) // addu
        {
            ID_c[0] = 'A';
            ID_c[1] = 'D';
            ID_c[2] = 'D';
            ID_c[3] = 'U';
            ID_c[4] = '\0';
        }
        else if(func == 0x22) // sub
        {
            ID_c[0] = 'S';
            ID_c[1] = 'U';
            ID_c[2] = 'B';
            ID_c[3] = '\0';
        }
        else if(func == 0x24) // and
        {
            ID_c[0] = 'A';
            ID_c[1] = 'N';
            ID_c[2] = 'D';
            ID_c[3] = '\0';
        }
        else if(func == 0x25) // or
        {
            ID_c[0] = 'O';
            ID_c[1] = 'R';
            ID_c[2] = '\0';
        }
        else if(func == 0x26) // xor
        {
            ID_c[0] = 'X';
            ID_c[1] = 'O';
            ID_c[2] = 'R';
            ID_c[3] = '\0';
        }
        else if(func == 0x27) // nor
        {
            ID_c[0] = 'N';
            ID_c[1] = 'O';
            ID_c[2] = 'R';
            ID_c[3] = '\0';
        }
        else if(func == 0x28) // nand
        {
            ID_c[0] = 'N';
            ID_c[1] = 'A';
            ID_c[2] = 'N';
            ID_c[3] = 'D';
            ID_c[4] = '\0';
        }
        else if(func == 0x2a) // slt
        {
            ID_c[0] = 'S';
            ID_c[1] = 'L';
            ID_c[2] = 'T';
            ID_c[3] = '\0';
        }
        else if(func == 0x00) // sll
        {
            ID_c[0] = 'S';
            ID_c[1] = 'L';
            ID_c[2] = 'L';
            ID_c[3] = '\0';
        }
        else if(func == 0x02) // srl
        {
            ID_c[0] = 'S';
            ID_c[1] = 'R';
            ID_c[2] = 'L';
            ID_c[3] = '\0';
        }
        else if(func == 0x03) // sra
        {
            ID_c[0] = 'S';
            ID_c[1] = 'R';
            ID_c[2] = 'A';
            ID_c[3] = '\0';
        }
        else if(func == 0x08) // jr
        {
            ID_c[0] = 'J';
            ID_c[1] = 'R';
            ID_c[2] = '\0';
        }
        else if(func == 0x18) // mult
        {
            ID_c[0] = 'M';
            ID_c[1] = 'U';
            ID_c[2] = 'L';
            ID_c[3] = 'T';
            ID_c[4] = '\0';
        }
        else if(func == 0x19) // multu
        {
            ID_c[0] = 'M';
            ID_c[1] = 'U';
            ID_c[2] = 'L';
            ID_c[3] = 'T';
            ID_c[4] = 'U';
            ID_c[5] = '\0';
        }
        else if(func == 0x10) // mfhi
        {
            ID_c[0] = 'M';
            ID_c[1] = 'F';
            ID_c[2] = 'H';
            ID_c[3] = 'I';
            ID_c[4] = '\0';
        }
        else if(func == 0x12) // mflo
        {
            ID_c[0] = 'M';
            ID_c[1] = 'F';
            ID_c[2] = 'L';
            ID_c[3] = 'O';
            ID_c[4] = '\0';
        }
    }
    else if(opcode == 0x02) // j
    {
        ID_c[0] = 'J';
        ID_c[1] = '\0';
    }
    else if(opcode == 0x03) // jal
    {
        ID_c[0] = 'J';
        ID_c[1] = 'A';
        ID_c[2] = 'L';
        ID_c[3] = '\0';
    }
    else if(opcode == 0x08) // addi
    {
        ID_c[0] = 'A';
        ID_c[1] = 'D';
        ID_c[2] = 'D';
        ID_c[3] = 'I';
        ID_c[4] = '\0';
    }
    else if(opcode == 0x09) // addiu
    {
        ID_c[0] = 'A';
        ID_c[1] = 'D';
        ID_c[2] = 'D';
        ID_c[3] = 'I';
        ID_c[4] = 'U';
        ID_c[5] = '\0';
    }
    else if(opcode == 0x23) // lw
    {
        ID_c[0] = 'L';
        ID_c[1] = 'W';
        ID_c[2] = '\0';
    }
    else if(opcode == 0x21) // lh
    {
        ID_c[0] = 'L';
        ID_c[1] = 'H';
        ID_c[2] = '\0';
    }
    else if(opcode == 0x25) // lhu
    {
        ID_c[0] = 'L';
        ID_c[1] = 'H';
        ID_c[2] = 'U';
        ID_c[3] = '\0';
    }
    else if(opcode == 0x20) // lb
    {
        ID_c[0] = 'L';
        ID_c[1] = 'B';
        ID_c[2] = '\0';
    }
    else if(opcode == 0x24) // lbu
    {
        ID_c[0] = 'L';
        ID_c[1] = 'B';
        ID_c[2] = 'U';
        ID_c[3] = '\0';
    }
    else if(opcode == 0x2b) // sw
    {
        ID_c[0] = 'S';
        ID_c[1] = 'W';
        ID_c[2] = '\0';
    }
    else if(opcode == 0x29) // sh
    {
        ID_c[0] = 'S';
        ID_c[1] = 'H';
        ID_c[2] = '\0';
    }
    else if(opcode == 0x28) // sb
    {
        ID_c[0] = 'S';
        ID_c[1] = 'B';
        ID_c[2] = '\0';
    }
    else if(opcode == 0x0f) // lui
    {
        ID_c[0] = 'L';
        ID_c[1] = 'U';
        ID_c[2] = 'I';
        ID_c[3] = '\0';
    }
    else if(opcode == 0x0c) // andi
    {
        ID_c[0] = 'A';
        ID_c[1] = 'N';
        ID_c[2] = 'D';
        ID_c[3] = 'I';
        ID_c[4] = '\0';
    }
    else if(opcode == 0x0d) // ori
    {
        ID_c[0] = 'O';
        ID_c[1] = 'R';
        ID_c[2] = 'I';
        ID_c[3] = '\0';
    }
    else if(opcode == 0x0e) // nori
    {
        ID_c[0] = 'N';
        ID_c[1] = 'O';
        ID_c[2] = 'R';
        ID_c[3] = 'I';
        ID_c[4] = '\0';
    }
    else if(opcode == 0x0a) // slti
    {
        ID_c[0] = 'S';
        ID_c[1] = 'L';
        ID_c[2] = 'T';
        ID_c[3] = 'I';
        ID_c[4] = '\0';
    }
    else if(opcode == 0x04) // beq
    {
        ID_c[0] = 'B';
        ID_c[1] = 'E';
        ID_c[2] = 'Q';
        ID_c[3] = '\0';
    }
    else if(opcode == 0x05) // bne
    {
        ID_c[0] = 'B';
        ID_c[1] = 'N';
        ID_c[2] = 'E';
        ID_c[3] = '\0';
    }
    else if(opcode == 0x07) // bgtz
    {
        ID_c[0] = 'B';
        ID_c[1] = 'G';
        ID_c[2] = 'T';
        ID_c[3] = 'Z';
        ID_c[4] = '\0';
    }
    else if(opcode == 0x3f) // halt
    {
        ID_c[0] = 'H';
        ID_c[1] = 'A';
        ID_c[2] = 'L';
        ID_c[3] = 'T';
        ID_c[4] = '\0';
    }
    return;
}
