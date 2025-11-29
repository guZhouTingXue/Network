#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>

#define BUF_SIZE 1024

void error_handling(const char *message);
void unix_error(const char *msg); /* Unix-style error */

int main(int argc, char* argv[])
{
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    socklen_t buf_size;
    socklen_t optlen = sizeof(buf_size);

    int state;
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &buf_size, &optlen);
    if(state == -1)
        unix_error("getsockopt error for TCP socket");
    else
        printf("TCP sock sendbuffer size : %d\n", buf_size);

    buf_size = 4600;
    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &buf_size, optlen);
    if(state == -1)
        unix_error("setsockopt error for TCP socket");
    else
    {
        state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &buf_size, &optlen);
        if(state == -1)
            unix_error("getsockopt error for TCP socket");
        else
            printf("TCP sock sendbuffer size changed to : %d\n", buf_size);
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
         read_cnt = read(client_fd, message, sizeof(message)); 
         if(read_cnt == 0)
            break;
        write(client_fd, message, read_cnt);
    }

    shutdown(client_fd, SHUT_WR);
    printf("shutdown\n");
    read_cnt = read(client_fd, message, BUF_SIZE -1);
    message[read_cnt] = 0;
    printf("Message from client: %s\n", message);

    close(client_fd);
    close(sock);

    return 0;
}

void error_handling(const char *message)
{
	fputs(message, stderr);
	fprintf(stderr, " %s", strerror(errno));
	fputc('\n', stderr);
	exit(1);
}

void unix_error(const char *msg) /* Unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}
