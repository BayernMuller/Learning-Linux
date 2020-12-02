#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h>
#include <stdlib.h>

void* readThread(void* arg)
{
	
}

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

	while (1)
	{
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

		pid_t pid = fork();
        if (pid == 0)
        {
            while (1)
            {
                char buf[1024] = { 0, };
                int ret = read(client_sock, buf, 1024);
                if (ret <= 0)
                {
                    printf("%d Client READ ERROR\n", client_sock);	
                    break;
                }
                printf("%d - %s\n", client_sock, buf);
            }
            close(client_sock);
        }
	}
	close(socket_desc);
	return 0;
}
