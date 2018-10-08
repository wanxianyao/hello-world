#pragma once
#include<WinSock2.h>  
#pragma comment(lib,"WS2_32.lib")  

#include <string>
//using namespace std;

#define NET_OK 0
#define NET_ERR -1

#define BUFLEN 1024 * 1024

typedef int (CALLBACK *NetCallback)(SOCKET fd, void* pParma);
typedef int (CALLBACK *RecvCallback)(SOCKET fd, char* sRecv, int len, void* pParma);

enum ENetType
{
	TCP_CLIENT = 0,
	TCP_SERVER = 1,
	UDP_CLIENT = 2,
	UDP_SERVER = 3,
};

struct NetConfig
{
	std::string LocalIP;
	int LocalPort;
	std::string PeerIP;
	int PeerPort;
	NetCallback cbAcp;     //客户端时候作为退出回调函数, 服务端时作Accept一个连接时的回调函数
	void* pAcpcbParam;     //cbAcp的参数
	RecvCallback cb;       //select接收到数据时候处理数据的回调
	void* pcbParam;		   //cb的参数
};

class INetClass
{
public:
	virtual int Run() = 0;
	virtual int Stop() = 0;
};

