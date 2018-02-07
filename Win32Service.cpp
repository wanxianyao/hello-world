// Win32Service.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Win32Service.h"
#include "HPNatServer.h"

//////////////////////////////////////////////////////////////////////////
//定义全局函数变量  
void Init();
BOOL IsInstalled();
BOOL Install();
BOOL Uninstall();
BOOL RunSerivce();

void WINAPI ServiceMain(DWORD   dwNumServicesArgs, LPSTR   *lpServiceArgVectors);
void WINAPI ServiceStrl(DWORD dwOpcode);
TCHAR szServiceName[128] = _T("HELP_NAT");
BOOL bInstall = FALSE;
SERVICE_STATUS_HANDLE hServiceStatus = NULL;
SERVICE_STATUS status;
DWORD dwThreadID = 0;
CIOThread	g_IOThread;
bool		g_MainThreadExit = false;
bool		g_ExitThread = false;
HANDLE		g_ExitEvent;

//void __stdcall ThreadCallBack(DWORD pUser, int iUserData)
//{
//	while (true)
//	{
//		if (g_IOThread.IsExit())
//			break;
//		Sleep(1000);
//	}
//	g_MainThreadExit = true;
//}


void __stdcall ThreadCallBack(DWORD pUser, int iUserData)
{
	CHPNatServer natServer;
	string sip;
	string soldip;

	char strUrl[MAX_PATH] = { 0 };
	char strPort[MAX_PATH] = { 0 };
	char strID[MAX_PATH] = { 0 };
	GetPrivateProfileString(_T("config"), _T("URL"), "", strUrl, MAX_PATH, _T("c:\\config.ini"));
	GetPrivateProfileString(_T("config"), _T("port"), "", strPort, MAX_PATH, _T("c:\\config.ini"));
	GetPrivateProfileString(_T("config"), _T("ID"), "", strID, MAX_PATH, _T("c:\\config.ini"));

	while (true)
	{
		if (g_ExitThread)
		{
			g_MainThreadExit = true;
			break;
		}

		if (natServer.GetIP(sip) == 1)
		{
			DWORD dw = WaitForSingleObject(g_ExitEvent, 1000 * 60 * 5);
			switch (dw)
			{
			case WAIT_OBJECT_0:
				LOG_ERROR("收到退出信号量1");
				break;
			case WAIT_TIMEOUT:
				LOG_ERROR("等待时间超过5分钟1");
				break;
			case WAIT_FAILED:
				LOG_ERROR("函数调用失败，比如传递了一个无效的句柄1");
				break;
			}
			continue;
		}

		if (soldip != sip)
		{
			if (natServer.SetIP(strID, strUrl, strPort, sip) == 1)
			{
				break;
			}

			soldip = sip;
		}
							
		//5分钟设置一次
		//Sleep(1000 * 60 * 5);
		DWORD dw = WaitForSingleObject(g_ExitEvent, 1000 * 60 * 5);
		switch (dw)
		{
		case WAIT_OBJECT_0:
			LOG_ERROR("收到退出信号量");
			break;
		case WAIT_TIMEOUT:			
			LOG_ERROR("等待时间超过5分钟");
			break;
		case WAIT_FAILED:
			LOG_ERROR("函数调用失败，比如传递了一个无效的句柄");
			break;
		}
	}
	g_MainThreadExit = true;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaData.wVersion != MAKEWORD(2, 2))
	{
		WSACleanup();
		return 1;
	}

	g_ExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	Init();
	SERVICE_TABLE_ENTRY st[] =
	{
		{ szServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL, NULL }
	};
	if (strcmp(lpCmdLine, "/i") == 0)
	{
		if (!IsInstalled())
			Install();
	}
	else if (strcmp(lpCmdLine, "/u") == 0)
	{
		if (IsInstalled())
			Uninstall();
	}
	else if (strcmp(lpCmdLine, "/r") == 0)
	{
		if (!IsInstalled())
			Install();

		//		int iret = DebugDlg(hInstance, hPrevInstance, lpCmdLine, SW_HIDE);
	}
	else
	{
		if (!::StartServiceCtrlDispatcher(st))
		{
		}
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////
void Init()
{
	hServiceStatus = NULL;
	status.dwServiceType = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
	status.dwCurrentState = SERVICE_START_PENDING;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	status.dwWin32ExitCode = 0;
	status.dwServiceSpecificExitCode = 0;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;
}

void WINAPI ServiceMain(DWORD   dwNumServicesArgs, LPSTR   *lpServiceArgVectors)
{
	// Register the control request handler  

	status.dwCurrentState = SERVICE_START_PENDING;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	//注册服务控制  
	hServiceStatus = RegisterServiceCtrlHandler(szServiceName, ServiceStrl);
	if (hServiceStatus == NULL)
	{
		return;
	}
	SetServiceStatus(hServiceStatus, &status);
	status.dwWin32ExitCode = S_OK;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;
	status.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hServiceStatus, &status);
	g_MainThreadExit = false;
	//g_IOThread.StartThread(1, ThreadCallBack, 0, true);
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadCallBack, (LPVOID)0, 0, &dwThreadID);
	
	while (!g_MainThreadExit)
	{
		Sleep(500);
	}
	
	status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(hServiceStatus, &status);
	::TerminateProcess(GetCurrentProcess(), 0);
}
void WINAPI ServiceStrl(DWORD dwOpcode)
{
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
		status.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(hServiceStatus, &status);
		g_ExitThread = true;
		SetEvent(g_ExitEvent);
		//g_IOThread.StopThread();
		break;
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		status.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(hServiceStatus, &status);
		g_ExitThread = true;
		SetEvent(g_ExitEvent);
		//g_IOThread.StopThread();
		break;
	default:
		break;
	}
}
BOOL IsInstalled()
{
	BOOL bResult = FALSE;

	//打开服务控制管理器   
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		//打开服务   
		SC_HANDLE hService = ::OpenService(hSCM, szServiceName, SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}
BOOL Install()
{
	//OutputDebugString("wanxy install\n");
	if (IsInstalled())
		return TRUE;

	//打开服务控制管理器   
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		return FALSE;
	}

	// Get the executable file path   
	TCHAR szFilePath[MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, MAX_PATH);

	//创建服务   
	SC_HANDLE hService = ::CreateService(
		hSCM, szServiceName, "海普外网地址查询",
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
		szFilePath, NULL, NULL, _T(""), NULL, NULL);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		return FALSE;
	}

	::StartService(hService, 0, NULL);

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);
	return TRUE;
}
BOOL Uninstall()
{
	//	if (!IsInstalled())
	//		return TRUE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		return FALSE;
	}

	//SC_HANDLE hService = ::OpenService(hSCM, szServiceName, SERVICE_STOP | DELETE);
	SC_HANDLE hService = ::OpenService(hSCM, szServiceName, SERVICE_STOP | DELETE);
	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		return FALSE;
	}
	SERVICE_STATUS status;
	QueryServiceStatus(hService, &status);
	if (status.dwCurrentState == SERVICE_RUNNING)
	{
		::ControlService(hService, SERVICE_CONTROL_STOP, &status);
		while (true)
		{
			QueryServiceStatus(hService, &status);
			if (status.dwCurrentState == SERVICE_STOPPED)
				break;
			Sleep(100);
		}
	}
	//删除服务   
	BOOL bDelete = ::DeleteService(hService);
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	if (bDelete)
		return TRUE;

	return FALSE;
}