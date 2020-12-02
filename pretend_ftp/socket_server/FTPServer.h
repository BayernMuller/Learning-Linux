#include "SocketServer.h"
#include <map>
#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
using namespace std;

class FTPServer : public SocketServer
{
    using SocketServer::SocketServer;
public:
    void Run()
    {
        Listen();
	    Accept();
        while (true)
        {
            excuteCommand();
        }
    }

private:
    void excuteCommand()
    {
        static const map<string, void(FTPServer::*)(const list<string>&)> command
        {
            {"ls", &FTPServer::ls},
            {"cd", &FTPServer::cd},
            {"pwd", &FTPServer::pwd},
            {"get", &FTPServer::get},
            {"put", &FTPServer::put},
            {"mget", &FTPServer::mget},
            {"mput", &FTPServer::mput},
            {"exit", &FTPServer::exit}
        };
        int len = 0;
        char buf[4096] = {0,};
        if (Receive(buf, 4096) <= 0)
        {
            cout << "Connection Lost" << endl;
            ::exit(0);
        }
        istringstream iss(buf);
        istream_iterator<string> iter(iss), end;
        list<string> args(iter, end);
        cout << buf << endl;
        auto method = command.find(args.front());
        if (method != command.end())
        {
            (this->*(method->second))(args);
        }
        else
        {
            sendMessage("No Command\n");
        }
    }

    void recvFile(const char* path)
    {
        int fd = creat(path, 0744);
        int red = 0, size, len;
        char buf[1024] = {0,};
        Receive((char*)&len, sizeof(int));
        size = len > 1024 ? 1024 : len;
        while ((red = Receive(buf, size)) > 0)
        {
            write(fd, buf, red);
            len -= red;
            size = len > 1024 ? 1024 : len;
            memset(buf, 0, 1024);
        }
        close(fd);
    }

    bool sendFile(const char* path)
    {
        int fd = open(path, O_RDONLY);
        if (fd == -1)
        {
            char str[128];
            sprintf(str, "%s does not exist.\n", path);
            sendMessage(str);
            return false;
        }
        int red, cnt = 0;
        char buf[1024] = {0,};
        struct stat info;

        int nameLength = strlen(path);
        char type = 'F';
        fstat(fd, &info);
        Send((char*)&info.st_size, sizeof(int));
        Send((char*)&type, sizeof(char));
        Send((char*)&nameLength, sizeof(int));
        Send(path, nameLength);
        while (0 < (red = read(fd, buf, 1024)))
        {
            Send(buf, red);
            memset(buf, 0, 1024);
        }
        close(fd);
        return true;
    }

    void sendMessage(const char* str)
    {
        char type = 'M';
        int len = strlen(str);
        Send((char*)&len, sizeof(int));
        Send((char*)&type, sizeof(char));
        Send(str, len);
    }

private:
    void ls(const list<string>& args)
    {
        DIR *dirp; 
        struct dirent *dentry; 
        if ((dirp = opendir(".")) == NULL) 
            throw "ls error";
        string str;
        str.reserve(1024);
        while(dentry = readdir(dirp)) 
        { 
            if(dentry->d_ino != 0) 
            {
                str += dentry->d_name;
                str += '\n';
            }
        }
        sendMessage(str.c_str());
    }

    void cd(const list<string>& args)
    {
        if (chdir((*++args.begin()).c_str()) == -1)
        {
            sendMessage("No directory\n");
            return;
        }
        sendMessage(string("Entered to " + *++args.begin() + '\n').c_str());
    }

    void pwd(const list<string>& args)
    {
        char buf[128] = {0,};
        getcwd(buf, 128);
        int len = strlen(buf);
        buf[len] = '\n';
        sendMessage(buf);
    }

    void get(const list<string>& args)
    {
        if (args.size() < 2)
        {
            sendMessage("No argument\n");
            return;
        }
        if (sendFile((*++args.begin()).c_str()) == false)
        {
            sendMessage("No file to send\n");
        }
    }

    void put(const list<string>& args)
    {
        if (args.size() < 2)
        {
            sendMessage("No argument\n");
            return;
        }
        recvFile((*++args.begin()).c_str());
        sendMessage(string((*++args.begin()) + "\n").c_str());
    }

    void mget(const list<string>& args)
    {
        if (args.size() < 2)
        {
            sendMessage("No argument\n");
            return;
        }
        int cnt = 0;
        string message;
        message.reserve(1024);
        for (auto itr = ++args.begin(); itr != args.end(); itr++)
        {
            if (itr->front() == '*')
            {
                string type(++itr->begin(), itr->end());
                struct dirent *dentry; 
                DIR *dirp = opendir(".");
                while(dentry = readdir(dirp)) 
                { 
                    if(dentry->d_ino != 0) 
                    {
                        if (type.empty() || strstr(dentry->d_name, type.c_str()))
                        {
                            struct stat info;
                            lstat(dentry->d_name, &info);
                            if (S_ISREG(info.st_mode))
                            {
                                sendFile(dentry->d_name);
                                message.append(dentry->d_name);
                                message.append("\n");
                                cnt++;
                            }
                        }
                    }
                }
                continue;
            }
            sendFile(itr->c_str());
            message.append(itr->c_str());
            message.append("\n");
            cnt++;
        }
        if (!cnt)
        {
            sendMessage("No file to send\n");
            return;
        }
        sendMessage(message.c_str());
    }

    void mput(const list<string>& args)
    {
        if (args.size() < 2)
        {
            sendMessage("No argument");
            return;
        }
        string str;
        str.reserve(1024);
        for (auto itr = ++args.begin(); itr != args.end(); itr++)
        {
            recvFile(itr->c_str());
            str.append(*itr);
            str.append("\n");
        }
        str.append("was sent\n");
        sendMessage(str.c_str());
    }

    void exit(const list<string>& args)
    {
        ::exit(0);
    }
};