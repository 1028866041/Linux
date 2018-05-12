#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<rpc/rpc.h>
#include"rdict.h"

char dict[DICTSIZ][MAXWORD+1];
int nwords= 0;

int initw()
{
    nwords=0;
    return 1;
}

int insertw(char* word)
{
    strcpy(dict[nwords], word);
    nwords++;    
    return nwords;
}

int deletew(char* word)
{
    int i= 0;
    for(i=0; i<nwords; i++)
    { 
        if(strcmp(word, dict[i])==0)
        {            
            nwords--;
            strcpy(dict[nwords], dict[i] );
            return 1;
        }
    }    
    return 0;
}

int lookupw(char *word)
{
    int i;
    for(i=0; i<nwords; i++)
    {
        if(strcmp(dict[i], word)==0)
            return 1;
    }    
    return 0;
}

