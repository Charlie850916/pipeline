#include "function.h"

void InitialImf()
{
    int i;
    fprintf(fp_r,"cycle 0\n");
    for(i=0 ; i<32 ; i++)
    {
        fprintf(fp_r,"$%02d: 0x%08X\n", i, s[i]);
    }
    fprintf(fp_r,"$HI: 0x%08X\n", HI);
    fprintf(fp_r,"$LO: 0x%08X\n", LO);
    fprintf(fp_r,"PC: 0x%08X\n", PC);
    fprintf(fp_r,"IF: 0x%08X\n", if2id.IS);
    fprintf(fp_r,"ID: NOP\n");
    fprintf(fp_r,"EX: NOP\n");
    fprintf(fp_r,"DM: NOP\n");
    fprintf(fp_r,"WB: NOP\n");
    fprintf(fp_r,"\n\n");

    return;
}

void PrintImf()
{
    int i;
    fprintf(fp_r,"cycle %d\n",cycle);
    for(i=0 ; i<32 ; i++)
    {
        if( s[i]!=s_p[i] )
        fprintf(fp_r,"$%02d: 0x%08X\n", i, s[i]);
        s_p[i] = s[i];
    }
    if(HI!=HI_p)
    {
        fprintf(fp_r,"$HI: 0x%08X\n", HI);
        HI_p = HI;
    }
    if(LO!=LO_p)
    {
        fprintf(fp_r,"$LO: 0x%08X\n", LO);
        LO_p = LO;
    }
    fprintf(fp_r,"PC: 0x%08X\n",PC);

    fprintf(fp_r,"IF: 0x%08X",if2id.inIS);
    if(stall) fprintf(fp_r," to_be_stalled");
    fprintf(fp_r,"\n");

    fprintf(fp_r,"ID: ");
    fprintf(fp_r,"%s",ID_c);
    if(stall) fprintf(fp_r," to_be_stalled");
    else
    {
       if(bforward_exmem2rs) fprintf(fp_r," fwd_EX-DM_rs_$%d", bforward_exmem2rs);
       if(bforward_exmem2rt) fprintf(fp_r," fwd_EX-DM_rt_$%d", bforward_exmem2rt);
    }
    fprintf(fp_r,"\n");

    fprintf(fp_r,"EX: ");
    fprintf(fp_r,"%s",EX_c);
    if(forward_exmem2rs) fprintf(fp_r," fwd_EX-DM_rs_$%d", forward_exmem2rs);
    if(forward_memwb2rs) fprintf(fp_r," fwd_DM-WB_rs_$%d", forward_memwb2rs);
    if(forward_exmem2rt) fprintf(fp_r," fwd_EX-DM_rt_$%d", forward_exmem2rt);
    if(forward_memwb2rt) fprintf(fp_r," fwd_DM-WB_rt_$%d", forward_memwb2rt);
    fprintf(fp_r,"\n");

    fprintf(fp_r,"DM: ");
    fprintf(fp_r,"%s",MEM_c);
    fprintf(fp_r,"\n");

    fprintf(fp_r,"WB: ");
    fprintf(fp_r,"%s",WB_c);
    fprintf(fp_r,"\n");

    fprintf(fp_r,"\n\n");
    return;
}

void OverFlow_add(int s, int t, int d)
{
    if( (s>0 && t>0 && d<=0) || (s<0 && t<0 && d>=0) ) fprintf(fp_err, "In cycle %d: Number Overflow\n", cycle);
    return;
}

int s0_Overwrite(int d)
{
    if(d==0)
    {
        s[0] = 0;
        fprintf(fp_err, "In cycle %d: Write $0 Error\n", cycle);
        return 1;
    }
    return 0;
}

int Misalignment(int i, int byte)
{
    if( i%byte != 0 )
    {
        halt = 1;
        fprintf(fp_err, "In cycle %d: Misalignment Error\n", cycle);
        return 1;
    }
    return 0;
}

int AddressOverflow(int sp, int b)
{
    if( sp > 1024-b || sp < 0)
    {
        halt = 1;
        fprintf(fp_err, "In cycle %d: Address Overflow\n", cycle);
        return 1;
    }
    return 0;
}
