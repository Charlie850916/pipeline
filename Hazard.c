#include "function.h"

int LoadStall_rs()
{
    if( (id2ex.isMEM && id2ex.LS) && (id2ex.save_reg == Get_rs(if2id.IS) ) && (Get_rs(if2id.IS) != 0) ) return 1;
    return 0;
}

int LoadStall_rt()
{
    if( (id2ex.isMEM && id2ex.LS) && (id2ex.save_reg == Get_rt(if2id.IS) ) && (Get_rt(if2id.IS) != 0) ) return 1;
    return 0;
}

int BeqBneStall()
{
    if(id2ex.isWB  && ( id2ex.save_reg == Get_rs(if2id.IS) || id2ex.save_reg == Get_rt(if2id.IS) ) && id2ex.save_reg != 0 ) return 1;
    if(mem2wb.isWB && (mem2wb.save_reg == Get_rs(if2id.IS) || mem2wb.save_reg == Get_rt(if2id.IS)) && mem2wb.save_reg != 0 && (MEM_c[1] == 'W' || MEM_c[1] == 'B' || MEM_c[1] == 'H') ) return 1;  
    return 0;
}

int JrBgtzStall()
{
    if(id2ex.isWB  && id2ex.save_reg == Get_rs(if2id.IS) && id2ex.save_reg != 0 ) return 1;
    if(mem2wb.isWB && mem2wb.save_reg == Get_rs(if2id.IS) && mem2wb.save_reg != 0  && MEM_c[0] == 'L' && (MEM_c[1] == 'W' || MEM_c[1] == 'B' || MEM_c[1] == 'H') ) return 1;
    return 0;
}

int Stall_R_type(unsigned int func)
{
    if(func == 0x20 || func == 0x21 || func == 0x22 || func == 0x24 || func == 0x25 || func == 0x26 || func == 0x27 || func == 0x28 || func == 0x2a || func == 0x18 || func == 0x19) return ( LoadStall_rs() || LoadStall_rt() );
    else if(func == 0x00 || func == 0x02 || func == 0x03) return LoadStall_rt();
    else if(func == 0x08) return JrBgtzStall();
    return 0;
}

int Stall_I_type(unsigned int opcode)
{
    if(opcode == 0x04 || opcode == 0x05) return BeqBneStall();
    else if(opcode == 0x07) return JrBgtzStall();
    else if(opcode == 0x2b || opcode == 0x29 || opcode == 0x28) return LoadStall_rs() || LoadStall_rt();
    else if(opcode != 0x0f && opcode != 0x02 && opcode != 0x03) return LoadStall_rs();
    return 0;
}

int MemWBForwardRs(unsigned int rs)
{
    if( mem2wb.isWB && mem2wb.save_reg != 0 &&
            !( ex2mem.isWB && ex2mem.save_reg != 0 && ex2mem.save_reg == rs ) &&
            mem2wb.save_reg == rs)
    {
        return rs;
    }
    return 0;
}

int MemWBForwardRt(unsigned int rt)
{
    if( mem2wb.isWB && mem2wb.save_reg != 0 &&
            !( ex2mem.isWB && ex2mem.save_reg != 0 && ex2mem.save_reg == rt ) &&
            mem2wb.save_reg == rt)
    {
        return rt;
    }
    return 0;
}

int ExMemForwardRs(unsigned int rs)
{
    if( ex2mem.isWB && ex2mem.save_reg != 0 && ex2mem.save_reg == rs ) return rs;
    return 0;
}

int ExMemForwardRt(unsigned int rt)
{
    if( ex2mem.isWB && ex2mem.save_reg != 0 && ex2mem.save_reg == rt ) return rt;
    return 0;
}

int Fwd_exmem2rs()
{
    if(stall) return 0;
    if(id2ex.oper.type == 'R')
    {
        if(id2ex.oper.func == 0x20 ||
                id2ex.oper.func == 0x21 ||
                id2ex.oper.func == 0x22 ||
                id2ex.oper.func == 0x24 ||
                id2ex.oper.func == 0x25 ||
                id2ex.oper.func == 0x26 ||
                id2ex.oper.func == 0x27 ||
                id2ex.oper.func == 0x28 ||
                id2ex.oper.func == 0x2a ||
                id2ex.oper.func == 0x18 ||
                id2ex.oper.func == 0x19)
        {
            return ExMemForwardRs(id2ex.rs);
        }
    }
    else if(id2ex.oper.func == 0x08 ||
            id2ex.oper.func == 0x09 ||
            id2ex.oper.func == 0x23 ||
            id2ex.oper.func == 0x21 ||
            id2ex.oper.func == 0x25 ||
            id2ex.oper.func == 0x20 ||
            id2ex.oper.func == 0x24 ||
            id2ex.oper.func == 0x2b ||
            id2ex.oper.func == 0x29 ||
            id2ex.oper.func == 0x28 ||
            id2ex.oper.func == 0x0c ||
            id2ex.oper.func == 0x0d ||
            id2ex.oper.func == 0x0e ||
            id2ex.oper.func == 0x0a)
    {
        return ExMemForwardRs(id2ex.rs) ;
    }
    return 0;
}

int Fwd_memwb2rs()
{
    if(stall) return 0;
    if(id2ex.oper.type == 'R')
    {
        if(id2ex.oper.func == 0x20 ||
                id2ex.oper.func == 0x21 ||
                id2ex.oper.func == 0x22 ||
                id2ex.oper.func == 0x24 ||
                id2ex.oper.func == 0x25 ||
                id2ex.oper.func == 0x26 ||
                id2ex.oper.func == 0x27 ||
                id2ex.oper.func == 0x28 ||
                id2ex.oper.func == 0x2a ||
                id2ex.oper.func == 0x18 ||
                id2ex.oper.func == 0x19 )
        {
            return MemWBForwardRs(id2ex.rs);
        }
    }
    else if(id2ex.oper.func == 0x08 ||
            id2ex.oper.func == 0x09 ||
            id2ex.oper.func == 0x23 ||
            id2ex.oper.func == 0x21 ||
            id2ex.oper.func == 0x25 ||
            id2ex.oper.func == 0x20 ||
            id2ex.oper.func == 0x24 ||
            id2ex.oper.func == 0x2b ||
            id2ex.oper.func == 0x29 ||
            id2ex.oper.func == 0x28 ||
            id2ex.oper.func == 0x0c ||
            id2ex.oper.func == 0x0d ||
            id2ex.oper.func == 0x0e ||
            id2ex.oper.func == 0x0a)
    {
        return MemWBForwardRs(id2ex.rs) ;
    }
    return 0;
}

int Fwd_exmem2rt()
{
    if(stall) return 0;
    if(id2ex.oper.type == 'R')
    {
        if(id2ex.oper.func == 0x20 ||
                id2ex.oper.func == 0x21 ||
                id2ex.oper.func == 0x22 ||
                id2ex.oper.func == 0x24 ||
                id2ex.oper.func == 0x25 ||
                id2ex.oper.func == 0x26 ||
                id2ex.oper.func == 0x27 ||
                id2ex.oper.func == 0x28 ||
                id2ex.oper.func == 0x2a ||
                id2ex.oper.func == 0x18 ||
                id2ex.oper.func == 0x19 ||
                id2ex.oper.func == 0x00 ||
                id2ex.oper.func == 0x02 ||
                id2ex.oper.func == 0x03)
        {
            return ExMemForwardRt(id2ex.rt);
        }
    }
    else
    { 
       if(id2ex.oper.func == 0x2b || id2ex.oper.func == 0x29 || id2ex.oper.func == 0x28) return ExMemForwardRt(id2ex.rt);
    }
    return 0;
}

int Fwd_memwb2rt()
{
    if(stall) return 0;
    if(id2ex.oper.type == 'R')
    {
        if(id2ex.oper.func == 0x20 ||
                id2ex.oper.func == 0x21 ||
                id2ex.oper.func == 0x22 ||
                id2ex.oper.func == 0x24 ||
                id2ex.oper.func == 0x25 ||
                id2ex.oper.func == 0x26 ||
                id2ex.oper.func == 0x27 ||
                id2ex.oper.func == 0x28 ||
                id2ex.oper.func == 0x2a ||
                id2ex.oper.func == 0x18 ||
                id2ex.oper.func == 0x19 ||
                id2ex.oper.func == 0x00 ||
                id2ex.oper.func == 0x02 ||
                id2ex.oper.func == 0x03)
        {
            return MemWBForwardRt(id2ex.rt);
        }
    }
    else
    {
       if(id2ex.oper.func == 0x2b || id2ex.oper.func == 0x29 || id2ex.oper.func == 0x28) return MemWBForwardRt(id2ex.rt);
    }
    return 0;
}

int BranchFwd_exmem2rs()
{
    if(GetOpcode(if2id.IS) == 0x04 || GetOpcode(if2id.IS) == 0x05 || GetOpcode(if2id.IS) == 0x07 || (GetOpcode(if2id.IS) == 0x00 && Get_func(if2id.IS) == 0x08 ) ) return ExMemForwardRs( Get_rs(if2id.IS) );
    return 0;
}

int BranchFwd_exmem2rt()
{
    if(GetOpcode(if2id.IS) == 0x04 || GetOpcode(if2id.IS) == 0x05) return ExMemForwardRt( Get_rt(if2id.IS) );
    return 0;
}
