#pragma once
#include "INetClass.h"

class CNetManage
{
public:
	CNetManage();
	~CNetManage();

	void InitNet();
	void Cleanup();
	INetClass* CreateNetInstance(ENetType eNetType, NetConfig config);
	
private:
	bool ValidateConfig(NetConfig *pConf, ENetType eType);
	bool IsIPAddressValid(const char* pszIPAddr);
	bool IsPortValid(int port);
};

