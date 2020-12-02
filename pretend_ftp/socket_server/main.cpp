#include <iostream>
#include "FTPServer.h"
using namespace std;

int main(int argc, char *argv[]) try
{
	FTPServer server(8000);
	server.Run();
}
catch (const char* str)
{
	cout << str << endl;
}