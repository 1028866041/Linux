#include<stdio.h>
#include<rpc/rpc.h>
#include"rdict.h"

extern CLIENT *handle;
static int *ret;
/*rpc stub interface*/
int initw()
{
    ret= initw_1(0, handle);
    return ret== 0?1:0;
}

int insertw(char *word)
{
    char **arg;
    arg = &word;
    ret = insertw_1(arg, handle);
    return ret== 0?1:0;
}

int deletew(char *word)
{
    char **arg;
    arg = &word;
    ret = deletew_1(arg, handle);
    return ret== 0?1:0;
}

int lookupw(char *word)
{
    char **arg;
    arg = &word;
    ret = lookupw_1(arg, handle);
    return ret== 0?1:0;
}

