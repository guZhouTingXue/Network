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
    int tcp_sock, upd_sock;
    int sock_type;
    socklen_t optlen;

    tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    upd_sock = socket(PF_INET, SOCK_DGRAM, 0);

    printf("SOCK_STREAM: %d\n", SOCK_STREAM);
    printf("SOCK_DGRAM: %d\n", SOCK_DGRAM);

    int state;
    state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, &sock_type, &optlen);
    if(state == -1)
        unix_error("getsockopt error for TCP socket");
    else
        printf("TCP socket type: %d\n", sock_type);

    state = getsockopt(upd_sock, SOL_SOCKET, SO_TYPE, &sock_type, &optlen);
    if(state == -1)     
        unix_error("getsockopt error for UDP socket");
    else
        printf("UDP socket type: %d\n", sock_type);


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
