#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<rpc/rpc.h>
#include"rdict.h"

#define RMACHINE "localhost"
CLIENT *handle;
int main()
{
    char word[MAXWORD+1];
    char cmd;
    int wrdlen;
    
    handle= clnt_create(RMACHINE,RDICTPROG,RDICTVERS, "tcp");
    if(handle==NULL)
    {
        printf("contact error\r\n");
        exit(1);
    }
    if(initw())
    {             
        printf("initw error\r\n");
        exit(1);
    }
    
    while(1)
    {
        wrdlen= nextin(&cmd, word);
        if(wrdlen<0)
            exit(0);    
        
        //printf("%c %s\n",cmd,word);      
        switch(cmd)
        {
            case 'i':
                if(insertw(word))
                    printf("insert fail\r\n");
                else
                    printf("insert %s\r\n", word);  
                break;
            case 'd':
                if(deletew(word))                    
                    printf("not found\r\n");
                else
                    printf("delete %s\r\n", word);
                break;
            case 'l':
                if(lookupw(word))                    
                    printf("not found\r\n");  
                else
                    printf("found %s\r\n", word);
                break;
            case 'q':       
                printf("exit\r\n");            
                exit(0);
                break;
            default:
                printf("command invalid\r\n");
                break;
        }  
    }

    clnt_destroy(handle);
    return 0;
}

int nextin(char* cmd, char* word)
{
    int i,ch;
    
    ch= getc(stdin);
    while(isspace(ch)||(ch=="\n"))
        ch= getchar();
    if(ch==EOF)
        return -1;    

    *cmd= (char)ch;

    getc(stdin);
    i= 0;
    while(!(isspace(ch)))
    {
        if(MAXWORD<++i)
        {
            printf("error: word long\r\n");
            exit(1);
        }      
        ch= getc(stdin);
        *word++= ch;     
    }
    *word++= 0;    
    
    return i;
}

