#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

class SocketClient
{
public:
	SocketClient(char* addr, int port)
	{
		m_Sock = socket(AF_INET, SOCK_STREAM, 0);
		if (m_Sock == -1)
		{
			throw "socket create error";
		}
		m_Server.sin_addr.s_addr = inet_addr(addr);
		m_Server.sin_family = AF_INET;
		m_Server.sin_port = htons(port);
	}

	void Connect()
	{
		if (connect(m_Sock, (struct sockaddr *)&m_Server, sizeof(m_Server)) < 0)
		{
			throw "connect error";
		}
	}

	int Send(const char* buf, int size)
	{
		return send(m_Sock, buf, size, 0);
	}

	int Receive(char buf[], int size)
    {
        int read_size = read(m_Sock, buf, size);
        return read_size;
    }

	~SocketClient()
	{
		close(m_Sock);
	}

private:
	sockaddr_in m_Server;
	int m_Sock;
};


