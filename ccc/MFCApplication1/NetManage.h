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
	
<<<<<<< HEAD
private:
=======
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
	bool ValidateConfig(NetConfig *pConf, ENetType eType);
	bool IsIPAddressValid(const char* pszIPAddr);
	bool IsPortValid(int port);
};

