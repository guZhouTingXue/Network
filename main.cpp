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
