#pragma once
#include "NetManage.h"
#include "TcpClient.h"
#include "TcpServer.h"
#include <mutex>

#define RECVLEN 1024
class CNetToolFrameWnd : public CWindowWnd, public INotifyUI
{
public:
	CNetToolFrameWnd();
	~CNetToolFrameWnd();

	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };

	void Init();

	void OnPrepare();

	void Notify(TNotifyUI& msg);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	static int CALLBACK RecvCliCb(SOCKET fd, char* sRecv, int len, void* pParma);
	static int CALLBACK ExitCb(SOCKET fd, void* pParma);

	static int CALLBACK RecvCb(SOCKET fd, char* sRecv, int len, void* pParma);
	static int CALLBACK AcceptCb(SOCKET fd, void* pParma);
public:
	CPaintManagerUI m_pm;

private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;

	std::mutex m_combLock;
	bool m_bVisible;
	CNetManage  m_Net;
	CTcpServer* m_pTcpServer;
	CTcpClient* m_pTcpClient;
	char* m_pRecvbuf;
	int m_nTotalRecv;
	int m_nSend;
};

