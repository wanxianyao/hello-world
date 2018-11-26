#pragma once
#include<WinSock2.h>  
#pragma comment(lib,"WS2_32.lib")  

#include <string>
using namespace std;

#define NET_OK 0
#define NET_ERR -1

enum ENetType
{
	TCP_CLIENT = 0,
	TCP_SERVER = 1,
	UDP_CLIENT = 2,
	UDP_SERVER = 3,
};

struct NetConfig
{
	string LocalIP;
	int LocalPort;
	string PeerIP;
	int PeerPort;
};

class INetClass
{
public:
	virtual int Recv(char *recvBuf, int len) = 0;
	virtual int Send(char *sendBuf, int len) = 0;
};

