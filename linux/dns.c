#include<stdio.h>
#include <netdb.h>
#include <arpa/inet.h>


int main(int argc, char** argv[])
{
	int i= 0;
	char str[32] = {0};
	struct hostent* h;	
	
	if(argc!= 2)
	{
		printf("useage:\r\n");
		return;
	}
	
	if((h= gethostbyname(argv[1]))==NULL)
	{
		herror("hostname error\r\n");
		return;
	}
	
	printf("%s\r\n", h->h_name);
	/*printf("%s\r\n", inet_ntoa(*((struct in_addr *)h->h_addr)));*/
		
    for (i = 0;  h->h_aliases[i]; i++)
        printf("%s\n", h->h_aliases[i]);

    for (i = 0; h->h_addr_list[i]; i++)
        printf("%s\n", inet_ntop(h->h_addrtype,  h->h_addr_list[i], str, sizeof(str)-1));

	return;
}
