#pragma once
#include "INetClass.h"

class CTcpClient : public INetClass
{
public:
	CTcpClient(NetConfig config);
	~CTcpClient();

	int Recv(char *recvBuf, int len);
	int Send(char *sendBuf, int len);

	int Connect();

private:
	NetConfig m_config;
	SOCKET m_fd;
};

