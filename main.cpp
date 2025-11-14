#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>

#define BUF_SIZE 1024

void error_handling(char *message);
void unix_error(char *msg); /* Unix-style error */

int main(int argc, char* argv[])
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    struct hostent *host = gethostbyaddr((void*)&addr.sin_addr, sizeof(addr.sin_addr), AF_INET);
    if(!host)
    {
         fprintf(stderr, "gethost error: %s\n", hstrerror(h_errno));
         exit(1);
    }
    //打印host 信息 和 前面一致
    printf("name:%s\n", host->h_name);
    for(int i=0; host->h_aliases[i]; i++)
        printf("Aliases %d: %s \n", host->h_aliases[i]);
    printf("Address type:%s\n",
           (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
    for(int i=0; host->h_addr_list[i]; i++)
        printf("IP addr %d: %s \n", i+1,
               inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));


    return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fprintf(stderr, " %s", strerror(errno));
	fputc('\n', stderr);
	exit(1);
}

void unix_error(char *msg) /* Unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}
