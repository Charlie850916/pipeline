#include "function.h"

void RegWriteANDError()
{
    if(rwae.isWriteReg)
    {
        s[rwae.save_reg] = rwae.data;
        s0_Overwrite(rwae.save_reg);
        rwae.isWriteReg = 0;
    }
    if(rwae.isWriteHILO)
    {
        if(rwae.isOverwriteHILO) fprintf(fp_err, "In cycle %d: Overwrite HI-LO registers\n", cycle);
        rwae.isOverwriteHILO = 1;
        HI = rwae.HI;
        LO = rwae.LO;
        rwae.isWriteHILO = 0;
    }
    if(rwae.addr_over)
    {
        halt = 1;
        fprintf(fp_err, "In cycle %d: Address Overflow\n", cycle);
    }
    if(rwae.addr_mis)
    {
        halt = 1;
        fprintf(fp_err, "In cycle %d: Misalignment Error\n", cycle);
    }
    if(rwae.num_over)
    {
        fprintf(fp_err, "In cycle %d: Number Overflow\n", cycle);
        rwae.num_over = 0; 
    }
    return;
}

void WB()
{
    int i;
    for(i = 0 ; i < 6 ; i++) WB_c[i] = MEM_c[i];

    if(mem2wb.isWB == 1)
    {
        rwae.isWriteReg = 1;
        rwae.data = mem2wb.data;
        rwae.save_reg = mem2wb.save_reg;
    }

    return;
}

void MEM()
{
    int i;
    for(i = 0 ; i < 6 ; i++) MEM_c[i] = EX_c[i];

    mem2wb.isWB = ex2mem.isWB;
    mem2wb.save_reg = ex2mem.save_reg;

    if(MEM_c[0] == 'N' && MEM_c[1] == 'O' && MEM_c[2] == 'P')
    {
         mem2wb.isWB = 0;
         mem2wb.save_reg = 0;
         return;
    }

    if(ex2mem.isMEM == 1)
    {
        if(AddressOverflow(ex2mem.data,ex2mem.byte))
        {
            rwae.addr_over = 1;
            rwae.addr_mis = Misalignment(ex2mem.data,ex2mem.byte);
            return;
        }
        if(Misalignment(ex2mem.data,ex2mem.byte))
        {
            rwae.addr_mis = 1; 
            return;
        }
        if(ex2mem.LS == 1) // load
        {
            switch(ex2mem.byte)
            {
            case 1:
                if(ex2mem.sign) // lb
                    mem2wb.data = d_mem[ex2mem.data] << 24 >> 24;
                else // lbu
                    mem2wb.data = d_mem[ex2mem.data];
                break;
            case 2:
                if(ex2mem.sign) // lh
                    mem2wb.data = (d_mem[ex2mem.data]<<24>>16) | (d_mem[ex2mem.data+1]);
                else // lhu
                    mem2wb.data = (d_mem[ex2mem.data]<<8) | (d_mem[ex2mem.data+1]);
                break;
            case 4: // lw
                mem2wb.data = (d_mem[ex2mem.data]<<24) | (d_mem[ex2mem.data+1]<<16) | (d_mem[ex2mem.data+2]<<8) | (d_mem[ex2mem.data+3]);
                break;
            }
        }
        else // save
        {
            switch(ex2mem.byte)
            {
            case 1: // sb
                d_mem[ex2mem.data] = s[ex2mem.save_reg] & 0x000000ff ;
                break;
            case 2: // sh
                d_mem[ex2mem.data] = (s[ex2mem.save_reg] >> 8) & 0x000000ff ;
                d_mem[ex2mem.data+1] = s[ex2mem.save_reg] & 0x000000ff ;
                break;
            case 4: // sw
                d_mem[ex2mem.data] = (s[ex2mem.save_reg] >> 24) & 0x000000ff ;
                d_mem[ex2mem.data+1] = (s[ex2mem.save_reg] >> 16) & 0x000000ff ;
                d_mem[ex2mem.data+2] = (s[ex2mem.save_reg] >> 8) & 0x000000ff ;
                d_mem[ex2mem.data+3] = s[ex2mem.save_reg] & 0x000000ff ;
                break;
            }
        }
    }
    else mem2wb.data = ex2mem.data;

    return;
}

void EX()
{
    long long int a, b;
    int i;
    int ALU0, ALU1;

    if(stall)
    {
        EX_c[0] = 'N';
        EX_c[1] = 'O';
        EX_c[2] = 'P';
        EX_c[3] = '\0';
        ex2mem.isMEM = 0;
        ex2mem.isWB = 0;
        ex2mem.save_reg = 0;
        return;
    }
    else for(i = 0 ; i < 6 ; i++) EX_c[i] = ID_c[i];

    if(EX_c[0] == 'N' && EX_c[1] == 'O' && EX_c[2] == 'P')
    {
        ex2mem.isMEM = 0;
        ex2mem.isWB = 0;
        ex2mem.save_reg = 0;
        return;
    }

    ex2mem.sign = id2ex.sign;
    ex2mem.byte = id2ex.byte;
    ex2mem.LS = id2ex.LS;
    ex2mem.save_reg = id2ex.save_reg;
    ex2mem.isWB = id2ex.isWB;
    ex2mem.isMEM = id2ex.isMEM;

    if(id2ex.isEX == 1)
    {
        if(id2ex.oper.type == 'R')
        {
            if(id2ex.oper.func == 0x00 || id2ex.oper.func == 0x02 || id2ex.oper.func == 0x03) // sll sra srl
            {
                if(forward_exmem2rt || forward_memwb2rt) ALU0 = fwd_rt;
                else ALU0 = s[id2ex.rt];
                ALU1 = id2ex.shamt;
            }
            else
            {
                if(forward_exmem2rs || forward_memwb2rs) ALU0 = fwd_rs;
                else ALU0 = s[id2ex.rs];
                if(forward_exmem2rt || forward_memwb2rt) ALU1 = fwd_rt;
                else ALU1 = s[id2ex.rt];
            }
        }
        else
        {
            if(forward_exmem2rs || forward_memwb2rs) ALU0 = fwd_rs;
            else ALU0 = s[id2ex.rs];
            ALU1 = id2ex.im;
        }
        if(id2ex.oper.type == 'R' && id2ex.oper.func == 0x18) // mult
        {
            rwae.isWriteHILO = 1;
            a = ALU0;
            b = ALU1;
            rwae.HI = a*b >> 32;
            rwae.LO = a*b & 0x00000000ffffffff;
        }
        else if(id2ex.oper.type == 'R' && id2ex.oper.func == 0x19) // multu
        {
            rwae.isWriteHILO = 1;
            a = ( ALU0 & 0x00000000ffffffff);
            b = ( ALU1 & 0x00000000ffffffff);
            rwae.HI = a*b >> 32;
            rwae.LO = a*b & 0x00000000ffffffff ;
        }
        else if(id2ex.oper.type == 'R' && id2ex.oper.func == 0x10) // mfhi
        {
            rwae.isOverwriteHILO = 0;
            ex2mem.data = HI;
        }
        else if(id2ex.oper.type == 'R' && id2ex.oper.func == 0x12) // mflo
        {
            rwae.isOverwriteHILO = 0;
            ex2mem.data = LO;
        }
        else ex2mem.data = ALU(id2ex.oper, ALU0, ALU1);
    }
    else ex2mem.data = id2ex.jalPC;

    return;
}

void ID()
{
    int opcode = GetOpcode(if2id.IS);

    id2ex.rs = Get_rs(if2id.IS);
    id2ex.rt = Get_rt(if2id.IS);
    id2ex.oper = Get_oper(opcode, Get_func(if2id.IS));

    if(!stall) Instruction_Detect(opcode, Get_func(if2id.IS));

    if(flush)
    {
        stall = 0;
        flush = 0;
        ID_c[0] = 'N';
        ID_c[1] = 'O';
        ID_c[2] = 'P';
        ID_c[3] = '\0';
        id2ex.isEX = 0;
        id2ex.isMEM = 0;
        id2ex.isWB = 0;
        id2ex.rs = 0;
        id2ex.rt = 0;
        return;
    }

    flush = 0;
    stall = 0;
    if(opcode == 0x00)
    {
        if(Stall_R_type(Get_func(if2id.IS)))
        {
            stall = 1;
            id2ex.isEX = 0;
            id2ex.isMEM = 0;
            id2ex.isWB = 0;
            id2ex.rs = 0;
            id2ex.rt = 0;
            return;
        }
    }
    else
    {
        if(Stall_I_type(opcode))
        {
            stall = 1;
            id2ex.isEX = 0;
            id2ex.isMEM = 0;
            id2ex.isWB = 0;
            return;
        }
    }

    if(opcode == 0x00) // R_type
    {
        id2ex.isEX = 1;
        id2ex.isMEM = 0;
        id2ex.isWB = 1;
        id2ex.shamt = Get_C(if2id.IS);
        id2ex.save_reg = Get_rd(if2id.IS);

        if(Get_rd(if2id.IS) == 0 && Get_rt(if2id.IS) == 0 && Get_C(if2id.IS) == 0 && Get_func(if2id.IS) == 0x00) // nop
        {
            ID_c[0] = 'N';
            ID_c[1] = 'O';
            ID_c[2] = 'P';
            ID_c[3] = '\0';
            id2ex.isEX = 0;
            id2ex.isWB = 0;
            return;
        }

        if(Get_func(if2id.IS) == 0x08) //jr
        {
            flush = 1;
            id2ex.isEX = 0;
            id2ex.isWB = 0;
            if(bforward_exmem2rs || bforward_memwb2rs) branchPC = bfwd_rs; 
            else branchPC = s[id2ex.rs];
            return;
        }

        if(Get_func(if2id.IS) == 0x18 || Get_func(if2id.IS) == 0x19) id2ex.isWB = 0; // mult multu

    }
    else if(opcode == 0x02 || opcode == 0x03) // J_type
    {
        flush = 1;
        id2ex.isEX = 0;
        id2ex.isMEM = 0;
        id2ex.isWB = 0;
        if(opcode == 0x03) // jal
        {
            id2ex.isWB = 1;
            id2ex.save_reg = 31;
            id2ex.jalPC = PC;
        }
        branchPC = ((PC+4)&0xf0000000) | Get_addr(if2id.IS) << 2;
    }
    else if(opcode == 0x3f) // halt
    {
        id2ex.isEX = 0;
        id2ex.isMEM = 0;
        id2ex.isWB = 0;
        return;
    }
    else // I_type
    {
        id2ex.isEX = 1;
        id2ex.isMEM = 0;
        id2ex.isWB = 1;

        short int im = Get_i(if2id.IS);
        if(opcode == 0x0c || opcode == 0x0d || opcode == 0x0e) id2ex.im = im&0x0000ffff; // andi ori nori
        else id2ex.im = im;

        id2ex.save_reg = id2ex.rt;

        switch(opcode)
        {
        case 0x23: // lw
            id2ex.isMEM = 1;
            id2ex.LS = 1;
            id2ex.byte = 4;
            break;
        case 0x21: // lh
            id2ex.isMEM = 1;
            id2ex.LS = 1;
            id2ex.byte = 2;
            id2ex.sign = 1;
            break;
        case 0x25: // lhu
            id2ex.isMEM = 1;
            id2ex.LS = 1;
            id2ex.byte = 2;
            id2ex.sign = 0;
            break;
        case 0x20: // lb
            id2ex.isMEM = 1;
            id2ex.LS = 1;
            id2ex.byte = 1;
            id2ex.sign = 1;
            break;
        case 0x24: // lbu
            id2ex.isMEM = 1;
            id2ex.LS = 1;
            id2ex.byte = 1;
            id2ex.sign = 0;
            break;
        case 0x2b: // sw
            id2ex.isMEM = 1;
            id2ex.LS = 0;
            id2ex.byte = 4;
            id2ex.isWB = 0;
            break;
        case 0x29: // sh
            id2ex.isMEM = 1;
            id2ex.LS = 0;
            id2ex.byte = 2;
            id2ex.isWB = 0;
            break;
        case 0x28: // sb
            id2ex.isMEM = 1;
            id2ex.LS = 0;
            id2ex.byte = 1;
            id2ex.isWB = 0;
            break;
        }

        if(opcode == 0x04 || opcode == 0x05 || opcode == 0x07) // beq bne bgtz
        {
            int $s, $t;
            if(bforward_exmem2rs || bforward_memwb2rs) $s = bfwd_rs;
            else $s = s[id2ex.rs];
            if(bforward_exmem2rt || bforward_memwb2rt) $t = bfwd_rt;
            else $t = s[id2ex.rt];

            id2ex.isWB = 0;
            id2ex.isEX = 0;

            switch(opcode)
            {
            case 0x04: // beq
                if($s == $t) 
                { 
                   flush = 1;   
                   branchPC = PC + 4 * (Get_i(if2id.IS));
                }
                return;
            case 0x05: // bne
                if($s != $t) 
                {
                   flush = 1;
                   branchPC = PC + 4 * (Get_i(if2id.IS));
                }
                return;
            case 0x07: // bgtz
                if($s > 0) 
                { 
                   flush = 1;
                   branchPC = PC + 4 * (Get_i(if2id.IS));
                } 
                return;
            }
        }
    }

    return;
}

void IF()
{
    if(!stall) if2id.IS = i_mem[PC/4];
    return;
}
