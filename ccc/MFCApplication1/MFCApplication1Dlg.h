
// MFCApplication1Dlg.h : ͷ�ļ�
//

#pragma once
#include "NetManage.h"
#include "TcpClient.h"
#include "TcpServer.h"

// CMFCApplication1Dlg �Ի���
class CMFCApplication1Dlg : public CDialogEx
{
// ����
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	static int CALLBACK RecvCb(SOCKET fd, char* sRecv, int len, void* pParma);
	static int CALLBACK AcceptCb(SOCKET fd, void* pParma);
	static int CALLBACK RecvCliCb(SOCKET fd, char* sRecv, int len, void* pParma);
	static int CALLBACK ExitCb(SOCKET fd, void* pParma);

	CNetManage m;
	CTcpServer *p;
	CTcpClient *pClient;
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnStart2();
	afx_msg void OnBnClickedBtnStop2();
	afx_msg void OnBnClickedBtnSend2();
};
