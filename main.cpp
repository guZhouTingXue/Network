#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

void error_handling(char *message);
void unix_error(char *msg); /* Unix-style error */

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
	int str_len = 0;
	int idx = 0, read_len = 0;

    if(argc != 3)
    {
    	printf("Usage : %s <IP> <port>\n", argv[0]);
    	exit(1);
    }
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    	unix_error("serv_socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); 
    serv_addr.sin_port = htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		unix_error("connect() error!");
	}
	
	while(read_len = read(sock, &message[idx++], 1))
	{
		//printf("read_len:%d\n", read_len);
		if(read_len == -1)
			unix_error("read() error!");
		str_len += read_len;
	}
	printf("read_len:%d\n", read_len);
	
	printf("Message from server :%s\n", message);
	printf("Function read call count: %d\n", str_len);
    close(sock	);
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