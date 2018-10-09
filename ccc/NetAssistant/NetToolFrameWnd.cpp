#include "stdafx.h"
#include "NetToolFrameWnd.h"
#include "ControlEx.h"
#include "LoginFrameWnd.h"

//client
int CALLBACK CNetToolFrameWnd::RecvCliCb(SOCKET fd, char* sRecv, int len, void* pParma)
{
	int port;
	char *sip;
	struct sockaddr local_addr;
	int length = sizeof(sockaddr);
	if (getpeername(fd, &local_addr, &length) == 0)
	{
		struct sockaddr_in* sin = (struct sockaddr_in*)(&local_addr);
		port = ntohs(sin->sin_port);
		sip = inet_ntoa(sin->sin_addr);
	}

	CNetToolFrameWnd *p = (CNetToolFrameWnd *)pParma;				
	memset(p->m_pRecvbuf, 0, BUFLEN);
	sprintf(p->m_pRecvbuf, "[%s:%d]: %s\n", sip, port, sRecv);
	
	CRichEditUI* pEdit = static_cast<CRichEditUI*>(p->m_pm.FindControl(_T("edit_recv")));
	if (pEdit)
	{
		//获取接收总长度，先取得之前的文本长度，然后加上本次接收长度和前缀
		CDuiString str = pEdit->GetText();
		p->m_nTotalRecv = len + p->m_nTotalRecv;
		str += p->m_pRecvbuf;
		pEdit->SetText(str.GetData());

		CLabelUI *pLabel = static_cast<CLabelUI*>(p->m_pm.FindControl(_T("label_recv")));
		if (pLabel)
		{
			char sbuf[20] = { 0 };
			sprintf(sbuf, " 接收：%d\n", p->m_nTotalRecv);
			pLabel->SetText(sbuf);
		}
	}

	return NET_OK;
}

int CALLBACK CNetToolFrameWnd::ExitCb(SOCKET fd, void* pParma)
{
	CNetToolFrameWnd *p = (CNetToolFrameWnd *)pParma;

	CButtonUI* pCon = static_cast<CButtonUI*>(p->m_pm.FindControl(_T("btn_connect")));
	pCon->SetVisible(true);

	CButtonUI* pDiscon = static_cast<CButtonUI*>(p->m_pm.FindControl(_T("btn_disconnect")));
	pDiscon->SetVisible(false);

	CComboUI *pComb = static_cast<CComboUI*>(p->m_pm.FindControl(_T("proto_combo")));
	pComb->SetEnabled(true);
	CEditUI *pEditIP = static_cast<CEditUI*>(p->m_pm.FindControl(_T("serverip")));
	CEditUI *pEditPort = static_cast<CEditUI*>(p->m_pm.FindControl(_T("serverport")));
	pEditIP->SetEnabled(true);
	pEditPort->SetEnabled(true);

	return NET_OK;
}

// server
int CALLBACK CNetToolFrameWnd::RecvCb(SOCKET fd, char* sRecv, int len, void* pParma)
{
	CNetToolFrameWnd *p = (CNetToolFrameWnd *)pParma;

	if (sRecv == NULL && len == -1)
	{
		//断链处理
		p->m_combLock.lock();
		CComboUI* pcomb = static_cast<CComboUI*>(p->m_pm.FindControl(_T("client_combo")));
		if (pcomb)
		{
			//int index = pcomb->GetCurSel();
			//CListLabelElementUI *pItem = (CListLabelElementUI*)pcomb->GetItemAt(index);
			for (int i = 0; i < pcomb->GetCount(); i++)
			{
				CListLabelElementUI *pItem = (CListLabelElementUI*)pcomb->GetItemAt(i);
				if ((SOCKET)pItem->GetTag() == fd)
				{
					pcomb->RemoveAt(i);
					break;
				}
			}			
		}
		p->m_combLock.unlock();
	}
	else
	{
		int port;
		char *sip;
		struct sockaddr local_addr;
		int length = sizeof(sockaddr);
		if (getpeername(fd, &local_addr, &length) == 0)
		{
			struct sockaddr_in* sin = (struct sockaddr_in*)(&local_addr);
			port = ntohs(sin->sin_port);
			sip = inet_ntoa(sin->sin_addr);
		}

		char sbuf[100] = { 0 };
		sprintf(sbuf, "[%s:%d]: %s\n", sip, port, sRecv);

		memset(p->m_pRecvbuf, 0, BUFLEN);
		sprintf(p->m_pRecvbuf, "[%s:%d]: %s\n", sip, port, sRecv);

		CRichEditUI* pEdit = static_cast<CRichEditUI*>(p->m_pm.FindControl(_T("edit_recv")));
		if (pEdit)
		{
			//获取接收总长度，先取得之前的文本长度，然后加上本次接收长度和前缀
			CDuiString str = pEdit->GetText();
			p->m_nTotalRecv = len + p->m_nTotalRecv;
			str += p->m_pRecvbuf;
			pEdit->SetText(str.GetData());

			CLabelUI *pLabel = static_cast<CLabelUI*>(p->m_pm.FindControl(_T("label_recv")));
			if (pLabel)
			{
				char sbuf[20] = { 0 };
				sprintf(sbuf, " 接收：%d\n", p->m_nTotalRecv);
				pLabel->SetText(sbuf);
			}
		}

	}
	return NET_OK;
}

int CALLBACK CNetToolFrameWnd::AcceptCb(SOCKET fd, void* pParma)
{
	int port;
	char *sip;
	struct sockaddr local_addr;
	int length = sizeof(sockaddr);
	if (getpeername(fd, &local_addr, &length) == 0)
	{
		struct sockaddr_in* sin = (struct sockaddr_in*)(&local_addr);
		port = ntohs(sin->sin_port);
		sip = inet_ntoa(sin->sin_addr);
	}

	char sbuf[100] = { 0 };
	sprintf(sbuf, "%s:%d", sip, port);
	CNetToolFrameWnd *p = (CNetToolFrameWnd *)pParma;
	CComboUI* pCombCli = static_cast<CComboUI*>(p->m_pm.FindControl(_T("client_combo")));
	if (pCombCli)
	{
		CListLabelElementUI* pListElement = new CListLabelElementUI;
		pListElement->SetText(sbuf);
		pListElement->SetTag((UINT_PTR)fd);
		pCombCli->Add(pListElement);
	}
	return NET_OK;
}

CNetToolFrameWnd::CNetToolFrameWnd()
{
	m_bVisible = false;
	m_pTcpServer = NULL;
	m_pTcpClient = NULL;
	m_pRecvbuf = new char[BUFLEN];
	m_nTotalRecv = 0;
	m_nSend = 0;
}


CNetToolFrameWnd::~CNetToolFrameWnd()
{
	m_Net.Cleanup();
	if (m_pRecvbuf)
	{
		delete [] m_pRecvbuf;
		m_pRecvbuf = NULL;
	}
}

void CNetToolFrameWnd::Init()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));

	m_Net.InitNet();
}

void CNetToolFrameWnd::OnPrepare()
{
	CLoginFrameWnd* pLoginFrame = new CLoginFrameWnd();
	if (pLoginFrame == NULL) { Close(); return; }
	pLoginFrame->Create(m_hWnd, _T(""), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
	//pLoginFrame->SetIcon(IDI_ICON_DUILIB);
	pLoginFrame->CenterWindow();
	pLoginFrame->ShowModal();
}

void CNetToolFrameWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("windowinit"))
	{
		OnPrepare();
	}
	else if (msg.sType == _T("click")) 
	{
		if (msg.pSender == m_pCloseBtn) {
			PostQuitMessage(0);
			return;
		}
		else if (msg.pSender == m_pMinBtn) {
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return;
		}
		else if (msg.pSender == m_pMaxBtn) {
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return;
		}
		else if (msg.pSender == m_pRestoreBtn) {
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return;
		}
		else if (msg.pSender->GetName() == _T("btn_connect"))
		{
			//1 获取下拉框取得类型
			//2 获取IP 端口创建SOCKET
			//3 改变界面状态
			CButtonUI* pControl = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_disconnect")));
			if (pControl)
			{
				CComboUI *pComb = static_cast<CComboUI*>(m_pm.FindControl(_T("proto_combo")));
				if (pComb)
				{
					CEditUI *pEditIP = static_cast<CEditUI*>(m_pm.FindControl(_T("serverip")));
					CEditUI *pEditPort = static_cast<CEditUI*>(m_pm.FindControl(_T("serverport")));

					int iType = pComb->GetCurSel();
					if (iType == 0)
					{
						//TCP CLIENT
						NetConfig conf;					
						conf.PeerIP = pEditIP->GetText().GetData();
						conf.PeerPort = atoi(pEditPort->GetText().GetData());					
						conf.cb = RecvCliCb;
						conf.pcbParam = this;
						conf.cbAcp = ExitCb;
						conf.pAcpcbParam = this;
						m_pTcpClient = (CTcpClient *)m_Net.CreateNetInstance(TCP_CLIENT, conf);
						if (m_pTcpClient)
						{
							if (m_pTcpClient->Run() == NET_ERR)
							{
								::MessageBox(m_hWnd, _T("连接失败"), _T("提示信息"), 0);
								return;
							}
							
							pEditIP->SetEnabled(false);
							pEditPort->SetEnabled(false);
						}
						else
						{
							//todo 错误处理
							::MessageBox(m_hWnd, _T("服务器IP或者端口输入错误"), _T("提示信息"), 0);
							return;
						}
					}
					else if (iType == 1)
					{
						//TCP SERVER
						NetConfig conf;
						conf.LocalIP = pEditIP->GetText().GetData();
						conf.LocalPort = atoi(pEditPort->GetText().GetData());
						conf.cb = RecvCb;
						conf.pcbParam = this;
						conf.cbAcp = AcceptCb;
						conf.pAcpcbParam = this;
						m_pTcpServer = (CTcpServer *)m_Net.CreateNetInstance(TCP_SERVER, conf);
						if (m_pTcpServer)
						{
							if (m_pTcpServer->Run() == NET_ERR)
							{
								::MessageBox(m_hWnd, _T("启动服务失败"), _T("提示信息"), 0);
								return;
							}							
						}
						else
						{
							//todo 错误处理
							::MessageBox(m_hWnd, _T("服务器IP或者端口输入错误"), _T("提示信息"), 0);
							return;
						}
					}

					pComb->SetEnabled(false);
				}
				msg.pSender->SetVisible(false);
				pControl->SetVisible(true);
			}		
		}
		else if (msg.pSender->GetName() == _T("btn_disconnect"))
		{
			CButtonUI* pControl = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_connect")));
			if (pControl)
			{
				CComboUI *pComb = static_cast<CComboUI*>(m_pm.FindControl(_T("proto_combo")));
				if (pComb)
				{
					CEditUI *pEditIP = static_cast<CEditUI*>(m_pm.FindControl(_T("serverip")));
					CEditUI *pEditPort = static_cast<CEditUI*>(m_pm.FindControl(_T("serverport")));

					int iType = pComb->GetCurSel();
					if (iType == 0)
					{
						//TCP CLIENT					
						if (m_pTcpClient)
						{
							m_pTcpClient->Stop();
							delete m_pTcpClient;
							m_pTcpClient = NULL;
						}						

						pEditIP->SetEnabled(true);
						pEditPort->SetEnabled(true);
					}
					else if (iType == 1)
					{
						//TCP SERVER
						if (m_pTcpServer)
						{
							m_pTcpServer->Stop();
							delete m_pTcpServer;
							m_pTcpServer = NULL;
						}
						m_combLock.lock();
						//清除COMBOX
						CComboUI* pcomb = static_cast<CComboUI*>(m_pm.FindControl(_T("client_combo")));
						if (pcomb)
						{
							pcomb->RemoveAll();
						}

						m_combLock.unlock();
					}

					pComb->SetEnabled(true);
				}
				msg.pSender->SetVisible(false);
				pControl->SetVisible(true);
			}
		}
		else if (msg.pSender->GetName() == _T("btn_send"))
		{
			CComboUI *pComb = static_cast<CComboUI*>(m_pm.FindControl(_T("proto_combo")));
			if (pComb)
			{
				int iType = pComb->GetCurSel();
				if (iType == 0)
				{
					//TCP CLIENT
					CRichEditUI* pEdit = static_cast<CRichEditUI*>(m_pm.FindControl(_T("edit_send")));
					CDuiString str = pEdit->GetText();
					if (m_pTcpClient)
					{
						int nRet = m_pTcpClient->Send((char *)str.GetData(), str.GetLength());
						m_nSend = nRet + m_nSend;
						CLabelUI *pLabel = static_cast<CLabelUI*>(m_pm.FindControl(_T("label_send")));
						if (pLabel)
						{
							char sbuf[20] = { 0 };
							sprintf(sbuf, " 发送：%d\n", m_nSend);
							pLabel->SetText(sbuf);
						}
					}
				}
				else if (iType == 1)
				{
					//TCP SERVER
					CComboUI *pCombCli = static_cast<CComboUI*>(m_pm.FindControl(_T("client_combo")));
					if (pCombCli)
					{
						int index = pCombCli->GetCurSel();
						CListLabelElementUI *pItem = (CListLabelElementUI*)pCombCli->GetItemAt(index);
						if (pItem)
						{
							CDuiString strCli = pItem->GetText();
							SOCKET fd = (SOCKET)pItem->GetTag();
							CRichEditUI* pEdit = static_cast<CRichEditUI*>(m_pm.FindControl(_T("edit_send")));
							CDuiString str = pEdit->GetText();
							if (m_pTcpServer)
							{
								int nRet = m_pTcpServer->Send(fd, (char *)str.GetData(), str.GetLength());
								m_nSend = nRet + m_nSend;
								CLabelUI *pLabel = static_cast<CLabelUI*>(m_pm.FindControl(_T("label_send")));
								if (pLabel)
								{
									char sbuf[20] = { 0 };
									sprintf(sbuf, " 发送：%d\n", m_nSend);
									pLabel->SetText(sbuf);
								}
							}
						}
					}
				}
			}
			
		}
		else if (msg.pSender->GetName() == _T("btn_reset"))
		{
			CLabelUI *pLabel1 = static_cast<CLabelUI*>(m_pm.FindControl(_T("label_recv")));
			CLabelUI *pLabel2 = static_cast<CLabelUI*>(m_pm.FindControl(_T("label_send")));
			if (pLabel1 && pLabel2)
			{
				pLabel1->SetText(" 接收：0");
				pLabel1->SetText(" 发送：0");
				m_nTotalRecv = 0;
				m_nSend = 0;
			}
		}
		else if (msg.pSender->GetName() == _T("btn_clear_recv"))
		{
			CRichEditUI* pEdit = static_cast<CRichEditUI*>(m_pm.FindControl(_T("edit_recv")));
			pEdit->SetText("");
		}
		else if (msg.pSender->GetName() == _T("btn_clear_send"))
		{
			CRichEditUI* pEdit = static_cast<CRichEditUI*>(m_pm.FindControl(_T("edit_send")));
			pEdit->SetText("");
		}
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		CDuiString name = msg.pSender->GetName();
		if (name == _T("proto_combo"))
		{			
			CVerticalLayoutUI* pControl = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("client_area_comb")));
			if (pControl)
			{
				//根据协议类型隐藏窗口
				CComboUI *pComb = static_cast<CComboUI*>(m_pm.FindControl(_T("proto_combo")));
				if (pComb)
				{
					int iType = pComb->GetCurSel();
					if (iType == 0)
					{
						//TCP CLIENT
						pControl->SetVisible(false);
						CEditUI *pIP = static_cast<CEditUI*>(m_pm.FindControl(_T("serverip")));
						pIP->SetText("");
						CEditUI *pPort = static_cast<CEditUI*>(m_pm.FindControl(_T("serverport")));
						pPort->SetText("");
					}
					else if (iType == 1)
					{
						//TCP SERVER
						pControl->SetVisible(true);
						CEditUI *pIP = static_cast<CEditUI*>(m_pm.FindControl(_T("serverip")));
						pIP->SetText("0.0.0.0");
						CEditUI *pPort = static_cast<CEditUI*>(m_pm.FindControl(_T("serverport")));
						pPort->SetText("26000");

					}
				}

				//CComboUI *pCom = static_cast<CComboUI*>(msg.pSender);
				//CDuiString str = pCom->GetName();
				//pControl->SetVisible(!m_bVisible);

				//m_bVisible = !m_bVisible;
			}			
		}
	}
}

LRESULT CNetToolFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CDialogBuilderCallbackEx cb;
	CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0, &cb, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();
	return 0;
}

LRESULT CNetToolFrameWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CNetToolFrameWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}

LRESULT CNetToolFrameWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CNetToolFrameWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CNetToolFrameWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CNetToolFrameWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	// 		if( !::IsZoomed(*this) ) {
	// 			RECT rcSizeBox = m_pm.GetSizeBox();
	// 			if( pt.y < rcClient.top + rcSizeBox.top ) {
	// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
	// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
	// 				return HTTOP;
	// 			}
	// 			else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
	// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
	// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
	// 				return HTBOTTOM;
	// 			}
	// 			if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
	// 			if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	// 		}

	RECT rcCaption = m_pm.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
		if (pControl && _tcscmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0 &&
			_tcscmp(pControl->GetClass(), DUI_CTR_OPTION) != 0 &&
			_tcscmp(pControl->GetClass(), DUI_CTR_TEXT) != 0)
			return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CNetToolFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CNetToolFrameWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	lpMMI->ptMaxPosition.x = rcWork.left;
	lpMMI->ptMaxPosition.y = rcWork.top;
	lpMMI->ptMaxSize.x = rcWork.right;
	lpMMI->ptMaxSize.y = rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT CNetToolFrameWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) {
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(*this) != bZoomed) {
		if (!bZoomed) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(false);
		}
	}
	return lRes;
}

LRESULT CNetToolFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}
