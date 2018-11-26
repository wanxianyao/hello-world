
// MFCApplication1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

<<<<<<< HEAD

=======
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
<<<<<<< HEAD
	ON_BN_CLICKED(IDOK, &CMFCApplication1Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFCApplication1Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_STOP, &CMFCApplication1Dlg::OnBnClickedBtnStop)
=======
	ON_BN_CLICKED(IDC_BTN_START, &CMFCApplication1Dlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CMFCApplication1Dlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_SEND, &CMFCApplication1Dlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_START2, &CMFCApplication1Dlg::OnBnClickedBtnStart2)
	ON_BN_CLICKED(IDC_BTN_STOP2, &CMFCApplication1Dlg::OnBnClickedBtnStop2)
	ON_BN_CLICKED(IDC_BTN_SEND2, &CMFCApplication1Dlg::OnBnClickedBtnSend2)
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
<<<<<<< HEAD
	p = NULL;
=======
	m.InitNet();
	p = NULL;
	pClient = NULL;
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

<<<<<<< HEAD


void CMFCApplication1Dlg::OnBnClickedOk()
{
	m.InitNet();
	NetConfig conf;
	conf.LocalIP = "0.0.0.0";
	conf.LocalPort = 26000;
=======
int CALLBACK CMFCApplication1Dlg::RecvCb(SOCKET fd, char* sRecv, int len, void* pParma)
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
	
	char sbuf[100] = {0};
	sprintf(sbuf, "[%s:%d]Recv: %s\n", sip, port, sRecv);
	CMFCApplication1Dlg *p = (CMFCApplication1Dlg *)pParma;
	p->SetDlgItemText(IDC_EDIT1, sbuf);

	return NET_OK;
}

int CALLBACK CMFCApplication1Dlg::AcceptCb(SOCKET fd, void* pParma)
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
	
	//test
	//struct sockaddr_in addr;
	//int32_t addr_len = sizeof(addr);
	//int32_t ret = getsockname(fd, (struct sockaddr *)&addr, &addr_len);
	//if (ret == 0)
	//{
	//	printf("getsockname succ:%s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	//}

	//memset(&addr, 0, sizeof(addr));
	//ret = getpeername(fd, (struct sockaddr *)&addr, &addr_len);
	//if (ret == 0)
	//{
	//	printf("getpeername succ:%s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	//}
	//end

	char sbuf[100] = { 0 };
	sprintf(sbuf, "%s:%d", sip, port);
	CMFCApplication1Dlg *p = (CMFCApplication1Dlg *)pParma;
	int index = ((CComboBox*)p->GetDlgItem(IDC_COMBO1))->AddString(_T(sbuf));
	((CComboBox*)p->GetDlgItem(IDC_COMBO1))->SetItemData(index, (DWORD_PTR)fd);
	return NET_OK;
}

void CMFCApplication1Dlg::OnBnClickedBtnStart()
{
	NetConfig conf;
	conf.LocalIP = "0.0.0.0";
	conf.LocalPort = 26000;
	conf.cbAcp = AcceptCb;
	conf.pAcpcbParam = this;
	conf.cb = RecvCb;
	conf.pcbParam = this;
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
	p = (CTcpServer *)m.CreateNetInstance(TCP_SERVER, conf);
	if (p)
	{
		p->Run();
	}
<<<<<<< HEAD
}


=======

	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
}
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe


void CMFCApplication1Dlg::OnBnClickedBtnStop()
{
	p->Stop();
	delete p;
<<<<<<< HEAD
}


void CMFCApplication1Dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
=======
	TRACE("delete");
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
}


void CMFCApplication1Dlg::OnBnClickedBtnSend()
{
	SOCKET fd = (SOCKET)((CComboBox*)GetDlgItem(IDC_COMBO1))->GetItemData(((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel());
	p->Send(fd, "hello", 5);
}

int CALLBACK CMFCApplication1Dlg::RecvCliCb(SOCKET fd, char* sRecv, int len, void* pParma)
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
	sprintf(sbuf, "[%s:%d]Recv: %s\n", sip, port, sRecv);
	CMFCApplication1Dlg *p = (CMFCApplication1Dlg *)pParma;
	p->SetDlgItemText(IDC_EDIT2, sbuf);

	return NET_OK;
}

int CALLBACK CMFCApplication1Dlg::ExitCb(SOCKET fd, void* pParma)
{
	CMFCApplication1Dlg *p = (CMFCApplication1Dlg *)pParma;
	p->GetDlgItem(IDC_BTN_START2)->EnableWindow(TRUE);
	p->GetDlgItem(IDC_BTN_STOP2)->EnableWindow(FALSE);
	return NET_OK;
}

void CMFCApplication1Dlg::OnBnClickedBtnStart2()
{
	NetConfig conf;
	conf.PeerIP = "127.0.0.1";
	conf.PeerPort = 26000;
	conf.cb = RecvCliCb;
	conf.pcbParam = this;
	conf.cbAcp = ExitCb;
	conf.pAcpcbParam = this;
	pClient = (CTcpClient *)m.CreateNetInstance(TCP_CLIENT, conf);
	if (pClient)
	{
		pClient->Run();
	}

	GetDlgItem(IDC_BTN_START2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP2)->EnableWindow(TRUE);
}


void CMFCApplication1Dlg::OnBnClickedBtnStop2()
{
	pClient->Stop();
	delete pClient;
	GetDlgItem(IDC_BTN_START2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP2)->EnableWindow(FALSE);
}


void CMFCApplication1Dlg::OnBnClickedBtnSend2()
{
	pClient->Send("hello", 5);
}
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
