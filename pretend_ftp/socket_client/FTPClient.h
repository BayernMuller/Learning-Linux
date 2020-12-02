#include <pthread.h>
#include <iostream>
#include <errno.h>
#include <sys/stat.h>
#include <sstream>
#include <iterator>
#include <list>
#include "SockClient.h"
using namespace std;


class FTPClient : public SocketClient
{
    using SocketClient::SocketClient;
public:
    void Run()
    {
        Connect();
        if (pthread_create(&m_Tid, NULL, readThread, (void*)this))
            throw "thread crate error";
        m_bBlock = false;
        while (true)
        {
            char command[1024] = {0,};
            cout << "\033[;32mftp: \033[0m";
            cin.getline(command, 1024);
            Send(command, strlen(command));
            istringstream iss(command);
            istream_iterator<string> iter(iss), end;
            
            list<string> args(iter, end);
            if (args.front() == "put")
                put(args);
            else if (args.front() == "mput")
                mput(args);
            
            m_bBlock = true;
            while (m_bBlock);
        }
    }

private:
    static void* readThread(void* arg)
    {
        auto* client = static_cast<FTPClient*>(arg);
        while (true)
        {
            int length = 0;
            int fd = STDOUT_FILENO;
            char type = 0;
            char buf[1024 + 1] = {0,};
            client->Receive((char*)&length, sizeof(int));
            client->Receive(&type, sizeof(char));
            if (type == 'F')
            {
                char name[128] = {0,};
                int nameLength;
                client->Receive((char*)&nameLength, sizeof(int));
                client->Receive(name, nameLength);
                fd = open(name, O_CREAT | O_TRUNC | O_WRONLY, 0744);
            }
            client->recvOut(fd, length);
            if (type == 'M')
                client->m_bBlock = false;
        }
    }

    bool sendFile(const char* path)
    {
        int fd = open(path, O_RDONLY);
        if (fd == -1)
        {
            return false;
        }
        int red, cnt = 0;
        char buf[1024] = {0,};
        struct stat info;
        fstat(fd, &info);
        Send((char*)&info.st_size, sizeof(int));
        while (0 < (red = read(fd, buf, 1024)))
        {
            Send(buf, red);
            memset(buf, 0, 1024);
        }
        close(fd);
        return true;
    }

    int recvOut(int fd, int len)
    {
        int red = 0, size;
        char buf[1024] = {0,};
        size = len > 1024 ? 1024 : len;
        while ((red = Receive(buf, size)) > 0)
        {
            write(fd, buf, red);
            len -= red;
            size = len > 1024 ? 1024 : len;
            memset(buf, 0, 1024);
        }
        if (fd != STDOUT_FILENO)
            close(fd);
        return 0;
    }

    bool put(const list<string>& args)
    {
        sendFile((++args.begin())->c_str());
        return true;
    }

    bool mput(const list<string>& args)
    {
        for (auto itr = ++args.begin(); itr != args.end(); itr++)
        {
           sendFile((++args.begin())->c_str());
        }
        return true;
    }

private:
    pthread_t m_Tid;
    bool m_bBlock;
};