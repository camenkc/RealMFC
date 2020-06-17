#pragma once

#include "./DataIO/DataIO.h"
#include "./MyControls/MyControls.h"
// CBackupDlg 对话框

class CBackupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBackupDlg)

	CBookBackup* pBackupset;
public:
	CBackupDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBackupDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGBackup };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBackup();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CEdit KeyEnter;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
