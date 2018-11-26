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
	NetCallback cbAcp;     //�ͻ���ʱ����Ϊ�˳��ص�����, �����ʱ��Acceptһ������ʱ�Ļص�����
	void* pAcpcbParam;     //cbAcp�Ĳ���
	RecvCallback cb;       //select���յ�����ʱ�������ݵĻص�
	void* pcbParam;		   //cb�Ĳ���
};

class INetClass
{
public:
	virtual int Run() = 0;
	virtual int Stop() = 0;
};

