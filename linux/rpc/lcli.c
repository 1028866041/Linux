#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<sys/socket.h>
#include<string.h>

#define MAXWORD 512
int nwords=0;
char *dict = NULL;
int main()
{
    char *word;
    char cmd;
    int wrdlen;
    
    dict= (char *)malloc(MAXWORD+1);
    if(dict== NULL)
        return-1;
    word= (char*)malloc(MAXWORD+1);
    memset(word, 0, MAXWORD);
    while(1)
    {
        wrdlen= nextin(&cmd, word);
        if(wrdlen< 0)
            return -1;
		
        //printf("%c %s\n",cmd,word);      
        switch(cmd)
        {
            case 'i':
                if(insertw(word)>0)
                    printf("insert %s\r\n", word);       
                break;
            case 'd':
                if(deletew(word))
                    printf("delete %s\r\n", word);
                else
                    printf("not found\r\n");
                break;
            case 'l':
                if(lookupw(word))
                    printf("found %s\r\n", word);
                else
                    printf("not found\r\n");       
                break;
            case 'q':                            
                free(dict);
                free(word);
                printf("exit\r\n");            
                exit(0);
                break;
            default:
                printf("command invalid\r\n");
                break;
        }    
    }

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

int insertw(char* word)
{
    strcpy(&dict[nwords], word);
    nwords++;    
    return nwords;
}

int deletew(char* word)
{
    int i= 0;
    for(i=0;i< nwords; i++)
    { 
        if(strcmp(word, &dict[i])==0)
        {            
            nwords--;
            strcpy(&dict[nwords], &dict[i]);
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
        if(strcmp(&dict[i], word)==0)
            return 1;
    }    
    return 0;
}

