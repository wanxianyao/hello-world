
// MFCApplication1Dlg.cpp : ʵ���ļ�
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


// CMFCApplication1Dlg �Ի���



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


// CMFCApplication1Dlg ��Ϣ�������

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
<<<<<<< HEAD
	p = NULL;
=======
	m.InitNet();
	p = NULL;
	pClient = NULL;
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
