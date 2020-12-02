#include <iostream>
#include "FTPClient.h"
using namespace std;

int main(int argc, char *argv[]) try
{
	FTPClient client(argv[1], atoi(argv[2]));
	client.Run();
}
catch (const char* str)
{
	cout << str << endl;
}