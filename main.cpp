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

    if(argc != 3)
    {
        printf("Usage : %s Server<ip> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        unix_error("serv_socket() error");


    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    int read_cnt = 0;
    FILE *fp = fopen("receive.txt", "wb");
    if(!fp)
    {
        printf("fopen() error");
        exit(-1);
    }

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        unix_error("connect() error");
    printf("connected\n");
    while( (read_cnt = read(sock, message, 1)) != 0 )
    {
        //message[read_cnt] = 0;
        //printf("data:%s\n", message);
        fwrite((void*)message, 1, read_cnt, fp);
    }

    puts("Received file data");
    write(sock, "Thank you", 10);
    fclose(fp);
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
