#include "function.h"

int main()
{
    Initial();

    cycle = 1;

    while(!halt)
    {
        RegWriteANDError();

        if(BranchFwd_exmem2rs()) bfwd_rs = ex2mem.data;
        bforward_exmem2rs = BranchFwd_exmem2rs();
        if(BranchFwd_memwb2rs()) bfwd_rs = mem2wb.data;
        bforward_memwb2rs = BranchFwd_memwb2rs();
        if(BranchFwd_exmem2rt()) bfwd_rt = ex2mem.data;
        bforward_exmem2rt = BranchFwd_exmem2rt();
        if(BranchFwd_memwb2rt()) bfwd_rt = mem2wb.data;
        bforward_memwb2rt = BranchFwd_memwb2rt();

        if(Fwd_exmem2rs()) fwd_rs = ex2mem.data;
        forward_exmem2rs = Fwd_exmem2rs();
        if(Fwd_memwb2rs()) fwd_rs = mem2wb.data;
        forward_memwb2rs = Fwd_memwb2rs();
        if(Fwd_exmem2rt()) fwd_rt = ex2mem.data;
        forward_exmem2rt = Fwd_exmem2rt();
        if(Fwd_memwb2rt()) fwd_rt = mem2wb.data;
        forward_memwb2rt = Fwd_memwb2rt();

        WB();
        MEM();
        EX();
        ID();
        IF();

        if(!halt) PrintImf();

        if( GetOpcode(if2id.IS)==0x3f && ID_c[0]=='H' && EX_c[0]=='H' && MEM_c[0]=='H' && WB_c[0]=='H' && !rwae.isWriteReg && !rwae.isWriteHILO) halt = 1;

        if(!stall) PC = PC + 4; 
        if(flush) PC = branchPC;

        if(cycle == 150) halt = 1;
        cycle++;
    }

    Ending();

    return 0;
}
