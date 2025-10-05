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
    char message[10] = {0};
	int str_len = 0;

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
        unix_error("connect() error");
    else
        puts("Connected ... \n");

    float operand0, operand1;
    char *p = nullptr;
    message[0] = 0x66;

    printf("Operand0:");
    scanf("%f", &operand0);
    printf("\n");
    p = (char*)&operand0;
    message[1] = *(p + 3);
    message[2] = *(p + 2);
    message[3] = *(p + 1);
    message[4] = *(p + 0);

    printf("Operand1:");
    scanf("%f", &operand1);
    printf("\n");
    p = (char*)&operand0;
    message[5] = *(p + 3);
    message[6] = *(p + 2);
    message[7] = *(p + 1);
    message[8] = *(p + 0);

    printf("Operator:");
    scanf(" %c", &message[9]);
    printf("\n");

    write(sock, message, 10);
    char recvMsg[BUF_SIZE];
    int recv_len = 0, recv_cnt = 0;
    float result = 0.0;
    bool recvResult = false;
    while(recv_len < 5)
    {
        recv_cnt = read(sock, &recvMsg[recv_len], sizeof(recvMsg) - recv_len);
        recv_len += recv_cnt;
        if(recv_len >= 5)
        {
            for(int i=0; i<recv_len; i++)
            {
                if(recvMsg[i] == 0x66)
                {
                    char *pr = (char*)&result;
                    pr[3] = recvMsg[i + 1];
                    pr[2] = recvMsg[i + 2];
                    pr[1] = recvMsg[i + 3];
                    pr[0] = recvMsg[i + 4];
                    printf("result:%f\n", result);
                    recvResult = true;
                }
            }
        }
    }
    if(!recvResult)
        printf("Not found\n");
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
