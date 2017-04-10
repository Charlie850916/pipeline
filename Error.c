#include "function.h"

int OverFlow_add(int s, int t, int d)
{
    if( (s>0 && t>0 && d<=0) || (s<0 && t<0 && d>=0) ) return 1;
    return 0;
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
    if( i%byte != 0) return 1;
    return 0;
}

int AddressOverflow(int sp, int b)
{
    if( sp > 1024-b || sp < 0) return 1;
    return 0;
}
