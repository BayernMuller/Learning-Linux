#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

class SocketServer
{
public:
    SocketServer(int port)
    {
        m_sockDesc = socket(AF_INET, SOCK_STREAM, 0);
        if (m_sockDesc == -1)
        {
            throw "Could not create socket";
        }
        m_Server.sin_family = AF_INET;
	    m_Server.sin_addr.s_addr = INADDR_ANY;
	    m_Server.sin_port = htons(port);
        if (bind(m_sockDesc, (struct sockaddr *)&m_Server, sizeof(m_Server)) < 0)
        {
            throw "bind failed. Error";
        }
    }

    void Listen()
    {
        if (listen(m_sockDesc, 3) == -1)
        {
            throw "bind failed. Error";
        }
    }

    void Accept()
    {
        int c = sizeof(struct sockaddr_in);
        m_ClientSock = accept(m_sockDesc, (struct sockaddr *)&m_Client, (socklen_t*)&c);
        if (m_ClientSock < 0)
        {
            throw "accept failed";
        }
    }

    int Send(const char* buf, int size)
    {
        return send(m_ClientSock, buf, size, 0);
    }

    int Receive(char buf[], int size)
    {
        int read_size = read(m_ClientSock, buf, size);
        buf[read_size] = 0;
        return read_size;
    }

    ~SocketServer()
    {
        close(m_ClientSock);
	    close(m_sockDesc);
    }

private:
    int m_sockDesc, m_ClientSock;
    sockaddr_in m_Server;
    sockaddr_in m_Client;
};

