
// MFCApplication1Dlg.h : 头文件
//

#pragma once
#include "NetManage.h"
#include "TcpClient.h"
#include "TcpServer.h"

// CMFCApplication1Dlg 对话框
class CMFCApplication1Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
