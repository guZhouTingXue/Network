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

    FILE *fp = NULL;
    fp = fopen("1.txt", "rb");
    if(!fp)
    {
        perror("fopen() error\n");
        exit(-1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        unix_error("serv_socket() error");


    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        unix_error("bind() error");

    if(listen(sock, 1) == -1)
        unix_error("listen() error");

    struct sockaddr_in client_addr;
    socklen_t client_addrlen;
    int client_fd;
    client_fd = accept(sock, (struct sockaddr*)&client_addr, &client_addrlen);
    if( client_fd == -1)
        unix_error("listen() error");
    int read_cnt = 0;

    while(1)
    {
        read_cnt = fread((void*)message, 1, 2, fp);
        if(read_cnt < 2)
        {
            write(client_fd, message, read_cnt);
            break;
        }
        write(client_fd, message, 2);
    }

    shutdown(client_fd, SHUT_WR);
    printf("shutdown\n");
    read_cnt = read(client_fd, message, BUF_SIZE -1);
    message[read_cnt] = 0;
    printf("Message from client: %s\n", message);

    fclose(fp);
    close(client_fd);
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
