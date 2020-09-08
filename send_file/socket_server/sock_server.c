#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int socket_desc, client_sock, c, read_size;
	struct sockaddr_in server, client;
	char client_message[2000];

	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
		return -1;
	}
	puts("Socket created");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(atoi(argv[1]));

	//Bind
	if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("bind failed. Error");
		return -1;
	}
	puts("bind done");

	//Listen
	if (listen(socket_desc, 3) == -1)
	{
		perror("bind failed. Error");
		return -1;
	}

	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	//accept connection from an incoming client
	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	puts("Connection accepted");

	read_size = read(client_sock, client_message, sizeof(client_message));
	client_message[read_size] = 0;
	printf("%s request received.\n", client_message);
	//memset(&client_message, 0, sizeof(client_message));
	
	
	struct stat info;
	stat(client_message, &info);
	int size = info.st_size;
	printf("%d\n", size);
	unsigned char* buf = malloc(info.st_size);
	int fd = open(client_message, O_RDONLY);
	if (fd == -1)
	{
		printf("open error\n");
	}
	read(fd, buf, info.st_size);
	printf(buf);
	send(client_sock, &size, sizeof(int), 0);
	send(client_sock, buf, size, 0);
	printf("end");
	close(fd);
	close(client_sock);
	close(socket_desc);
	free(buf);
	return 0;
}
