/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 



Module Name :   BOARDHDR.CPP
Abstract    :   SECURITY MANAGER of BOARD
Author      :     2001-07-10

Revision History: 

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "stdafx.h"
#include "../include/boardhdr_NDVR.h"
#include "../include/des32.h"

#pragma comment(lib, "../Library/des32.lib")
#pragma message("library is linking with 'des32.lib'")

/////////////////////////////////////////////////////////////////////////////////////////////
static unsigned char fixedkey[8] = {0x04, 0x02, 0x34, 0x28, 0x43, 0x76, 0x34, 0x95};     // random key

#ifdef __cplusplus
extern "C"
#endif
void BD_Encrypt(char *in, long len, char *out)
{
    int i, j, k, n;
    char chs[8];

    n = (int)((len + 7) / 8);
    k = 0;

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < 8; j++)
        {
            if (k < len)    chs[j] = *in++;         // set data
            else            chs[j] = 0x00;          // set 0x00
        }

        DES(chs, out, (char *)fixedkey, _ENDES);

        out += 8;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
#endif
void BD_Decrypt(char *in, int len, char *out)
{
    int i, n;

    n = (int)(len / 8);

    for(i = 0; i < n; i++)
    {
        DES(in, out, (char *)fixedkey, _DEDES);

        in  += 8;
        out += 8;
    }
}

