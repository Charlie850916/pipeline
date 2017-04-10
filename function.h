#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int isWriteReg;
    int isOverwriteHILO;
    int isWriteHILO;
    unsigned int save_reg;
    int data;
    int HI;
    int LO;
    int num_over;
    int addr_mis;
    int addr_over;
}RegWriteAndError;

typedef struct{
    int isWB;
    int isMEM;
    int LS; // 1 for load, 0 for save
    int byte;
    int sign; // 1 for sign, 0 for unsign
    unsigned int save_reg;
    int data;
}EXMEM;

typedef struct{
    int isWB;
    unsigned int save_reg;
    int data;
}MEMWB;

typedef struct{
    char type;
    unsigned int func;
}ALUoper;

typedef struct{
    int isWB;
    int isMEM;
    int isEX;
    int LS;
    int byte;
    int sign;
    unsigned int rs;
    unsigned int rt;
    unsigned int shamt;
    int im;
    int jalPC;
    ALUoper oper;
    unsigned int save_reg;
}IDEX;

typedef struct{
    unsigned int inIS;   
    unsigned int IS;
}IFID;

MEMWB mem2wb;
EXMEM ex2mem;
IDEX id2ex;
IFID if2id;
RegWriteAndError rwae;

int s[32], s_p[32], s_tmp[32];

int d_mem[1024];

unsigned int i_mem[256];

unsigned int PC, LO, HI, i_num, d_num, HI_p, LO_p, HI_tmp, LO_tmp;

FILE *fp_r, *fp_err;

char ID_c[6], EX_c[6], MEM_c[6], WB_c[6];

int cycle, overwriteHL, halt, stall, flush, branchPC;

int fwd_rs, fwd_rt, bfwd_rs, bfwd_rt;

int forward_exmem2rs, forward_exmem2rt, forward_memwb2rs, forward_memwb2rt;

int bforward_exmem2rs, bforward_exmem2rt, bforward_memwb2rs, bforward_memwb2rt;

char c_id[10], c_ex[10], c_mem[10], c_rb[10];

void Initial();

void InitialImf();

void PrintImf();

void Ending();

unsigned int Little2Big(unsigned int i);

unsigned int GetLineN(int n, FILE *fp);

unsigned int GetOpcode(unsigned int i);

unsigned int Get_rs(unsigned int i);

unsigned int Get_rt(unsigned int i);

unsigned int Get_rd(unsigned int i);

unsigned int Get_C(unsigned int i);

unsigned int Get_func(unsigned int i);

short int Get_i(unsigned int i);

int Get_addr(unsigned int i);

ALUoper Get_oper(unsigned int opcode, unsigned int func);

int ALU(ALUoper oper, int ALU0, int ALU1);

int s0_Overwrite(int d);

int OverFlow_add(int s, int t, int d);

int Misalignment(int i, int byte);

int AddressOverflow(int sp, int b);

int Stall_R_type(unsigned int func);

int Stall_I_type(unsigned int opcode);

int Fwd_exmem2rs();

int Fwd_exmem2rt();

int Fwd_memwb2rs();

int Fwd_memwb2rt();

int BranchFwd_exmem2rs();

int BranchFwd_exmem2rt();

int BranchFwd_memwb2rs();

int BranchFwd_memwb2rt();

void Instruction_Detect(unsigned int opcode, unsigned int func);

void RegWriteANDError();

void WB();

void MEM();

void EX();

void ID();

void IF();
