#include "function.h"

void Initial()
{
    FILE *fp_i, *fp_d;
    int i, buff;

    fp_i = fopen("iimage.bin", "rb");
    fp_d = fopen("dimage.bin", "rb");
    fp_r = fopen("a.rpt", "wb");
    fp_err = fopen("b.rpt", "wb");

    HI = 0x0000000;
    LO = 0x0000000;
    HI_p = 0x00000000;
    LO_p = 0x00000000;

    for(i=0 ; i<32 ; i++)
    {
        s[i] = 0x00000000;
        s_p[i] = 0x00000000;
    }

    for(i=0 ; i<256 ; i++) i_mem[i] = 0;
    for(i=0 ; i<1024 ; i++) d_mem[i] = 0;

    PC = GetLineN(0x00, fp_i);
    i_num = GetLineN(0x04, fp_i);
    s[29] = GetLineN(0x00, fp_d);
    d_num = GetLineN(0x04, fp_d);

    s_p[29] = s[29];

    for(i=0 ; i<i_num ; i++)
    {
        i_mem[PC/4+i] = GetLineN(i*4+8,fp_i);
    }

    for(i=0 ; i<d_num ; i++)
    {
        buff = ( GetLineN(i*4+8, fp_d) ) ;
        d_mem[i*4] = (buff >> 24) & 0x000000ff;
        d_mem[i*4+1] = (buff >> 16) & 0x000000ff;
        d_mem[i*4+2] = (buff >> 8) & 0x000000ff;
        d_mem[i*4+3] = buff & 0x000000ff;
    }

    fclose(fp_i);
    fclose(fp_d);

    for(i=0 ; i<32 ; i++) s_tmp[i] = s[i];
    HI_tmp = HI;
    LO_tmp = LO;

    overwriteHL = 0;
    halt = 0;
    stall = 0;
    flush = 0;
    rwae.isWriteReg = 0;
    rwae.isOverwriteHILO = 0;
    rwae.isWriteHILO = 0;
    rwae.addr_mis = 0;
    rwae.addr_over = 0;
    rwae.num_over = 0;
    mem2wb.isWB = 0;
    ex2mem.isMEM = 0;
    ex2mem.isWB = 0;
    id2ex.isEX = 0;
    id2ex.isMEM = 0;
    id2ex.isWB = 0;
    if2id.IS = 0x00000000;

    ID_c[0] = 'N';
    ID_c[1] = 'O';
    ID_c[2] = 'P';
    ID_c[3] = '\0';

    EX_c[0] = 'N';
    EX_c[1] = 'O';
    EX_c[2] = 'P';
    EX_c[3] = '\0';

    MEM_c[0] = 'N';
    MEM_c[1] = 'O';
    MEM_c[2] = 'P';
    MEM_c[3] = '\0';

    WB_c[0] = 'N';
    WB_c[1] = 'O';
    WB_c[2] = 'P';
    WB_c[3] = '\0';

    InitialImf();
    if2id.IS = i_mem[PC/4];
    PC = PC + 4;

    return;
}

void Ending()
{
    fclose(fp_r);
    fclose(fp_err);
    return;
}
