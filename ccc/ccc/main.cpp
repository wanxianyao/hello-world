#include <stdio.h>
#include <string>
using namespace std;

#include "NetManage.h"
#include "TcpClient.h"
#include "TcpServer.h"


int main()
{
	CNetManage m;
	m.InitNet();
	NetConfig conf;
	bool bClient = false;
	if (bClient)
	{
		conf.PeerIP = "127.0.0.1";
		conf.PeerPort = 8080;
		CTcpClient *p = (CTcpClient *)m.CreateNetInstance(TCP_CLIENT, conf);
		p->Connect();
		p->Send("hello", 5);
		char buf[100] = { 0 };
		p->Recv(buf, 100);
		delete p;
	}
	else
	{
		conf.LocalIP = "0.0.0.0";
		conf.LocalPort = 26000;
		CTcpServer *p = (CTcpServer *)m.CreateNetInstance(TCP_SERVER, conf);
		if (p)
		{
			p->Run();
		}

		getchar();
		//Sleep(60 * 1000);

		p->Stop();

		Sleep(60 * 1000);
	}

	return 0;
}