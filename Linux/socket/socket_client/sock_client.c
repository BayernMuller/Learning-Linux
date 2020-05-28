#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

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
	while (1)
	{
		scanf("%s", message);
		//Send some data
		if (send(sock, message, strlen(message), 0) < 0)
		{
			puts("Send failed");
			return -1;
		}
	}
	close(sock);
	return 0;
}