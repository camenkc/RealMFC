
// MFCTest03Dlg.h: 头文件
//
#include"CReaderViewDlg.h"
#pragma once


// CMFCTest03Dlg 对话框
class CMFCTest03Dlg : public CDialogEx
{
// 构造
	CBorrowDataset* pBorrowDataset;

public:
	CMFCTest03Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGIndex };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuExit();
	afx_msg void OnBookInput();
	afx_msg void OnViewBook();
	afx_msg void OnBorrowBook();
	afx_msg void OnReturnBook();
	afx_msg void OnReaderChange();
	afx_msg void OnReaderLogin();
	afx_msg void OnRegister();
	afx_msg void OnBackup();
	afx_msg void RefreshMainDlg();//刷新主对话框的ListCtrl

public:
	static int NowLoginReader;//静态成员 表示当前登录的人的标识码 0为未登录 
							//这样在全局都可以设置这个变量
	static int MayBeReturnBookId;
	CEdit NowReaderCode;
	CReaderDataset* pReaderDataset;

	afx_msg void OnLogoutClicked();
	afx_msg void OnReaderView();
	afx_msg void OnBorrowView();
	afx_msg void OnHistoryVIewDlg();
	CMyCListCtrl MainBorrowList;
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
};
