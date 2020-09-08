#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in server;
	char message[1000];

	if (argc != 3)
	{
		printf("usage : %s <hostname> <port>\n", argv[1]);
		return -1;
	}

	//Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		printf("Could not create socket");
		return -1;
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));

	//Connect to remote server
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}

	puts("Connected\n");

	//keep communicating with server
	
	scanf("%s", message);
	//Send some data
	if (send(sock, message, strlen(message), 0) < 0)
	{
		puts("Send failed");
		return -1;
	}
	
	int size;
	read(sock, &size, sizeof(int));
	unsigned char* buf = malloc(size + 1);
	buf[size] = 0;
	read(sock, buf, size);

	int fd = open(message, O_CREAT | O_WRONLY);
	write(fd, buf, size);

	
	free(buf);
	close(sock);
	return 0;
}
