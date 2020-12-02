#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>

#define SIZE sizeof(struct sockaddr_in)
#define MAX_SIZE 2048

#define MY_PORT_S 2008
#define MY_PORT_R 2009

void *recvMsg(void *parms);
int sendMsg(char *msg, int size, char* ip, int port);
int sendMsg2(char *msg, int size, struct sockaddr* sock, int sock_len);

int main()
{
    pthread_t tid;
    if (pthread_create(&tid, NULL, recvMsg, NULL))
    {
        printf("thread create fail\n");
    }

    char msg[MAX_SIZE];
    sprintf(msg, "testMsg");
    sendMsg(msg, strlen(msg), "127.0.0.1", 2007);

    while (1)
    {
        sleep(1);
    }
}

void* recvMsg(void* parms)
{
    struct sockaddr_in mySock = {AF_INET, MY_PORT_R, INADDR_ANY};
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int ret = bind(sockfd, (struct sockaddr*)&mySock, sizeof(mySock));
    if (ret < 0)
    {
        extern int errno;
        printf("ret(%d), errno(%d, %s)\n", ret, errno, strerror(errno));
    }   

    struct sockaddr_in sock;
    int sock_len;
    char msg[MAX_SIZE];
    while (1)
    {
        memset(msg, 0, MAX_SIZE);
        int ret = recvfrom(sockfd, &msg, MAX_SIZE, 0, (struct sockaddr*)&sock, &sock_len);
        printf("%d msg received\n", ret);
        if (ret < 0)
        {
            extern int errno;
            printf("ret(%d), errno(%d, %s)\n", ret, errno, strerror(errno));
        }
        else
        {
            printf("%d msg received\n", ret);
            printf("recv from : %s\n", msg);
            sock.sin_port += 1;
        }
    }
    close(sockfd);
}

int sendMsg2(char *msg, int size, struct sockaddr* sock, int sock_len)
{
    struct sockaddr_in mySock = {AF_INET, MY_PORT_S, INADDR_ANY};
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int ret = bind(sockfd, (struct sockaddr*)&mySock, sizeof(mySock));
    if (ret < 0)
    {
        extern int errno;
        printf("ret(%d), errno(%d, %s)\n", ret, errno, strerror(errno));
    }  
    sendto(sockfd, msg, size, 0, sock, sock_len);
    close(sockfd);
}

int sendMsg(char *msg, int size, char* ip, int port)
{
    struct sockaddr_in mySock = {AF_INET, MY_PORT_S, INADDR_ANY};
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int ret = bind(sockfd, (struct sockaddr*)&mySock, sizeof(mySock));
    if (ret < 0)
    {
        extern int errno;
        printf("ret(%d), errno(%d, %s)\n", ret, errno, strerror(errno));
    }
    struct sockaddr_in sock = {AF_INET, port, INADDR_ANY};
    sendto(sockfd, msg, size, 0, (struct sockaddr*)&sock, sizeof(sock));
    close(sockfd);
}
