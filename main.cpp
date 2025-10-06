#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

#define BUF_SIZE 1024

void error_handling(char *message);
void unix_error(char *msg); /* Unix-style error */

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    int str_len = 0;

    if(argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        unix_error("serv_socket() error");

    struct sockaddr_in local_addr;
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr)) == -1)
        unix_error("bind() error ");

    struct sockaddr_in client_addr;
    socklen_t client_addrlen;

    sleep(20);
    printf("recv \n");
    for(int i=0; i<3; i++)
    {

        str_len = recvfrom(sock, message, BUF_SIZE - 1, 0,
                           (struct sockaddr*)&client_addr, &client_addrlen);
        message[str_len] = 0;
        printf("Message %d %s \n", i+1, message);
    }

    close(sock);

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
