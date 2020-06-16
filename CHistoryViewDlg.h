#pragma once

#include "./DataIO/DataIO.h"
#include "./MyControls/MyControls.h"

// CHistoryViewDlg 对话框

class CHistoryViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoryViewDlg)
	CHistoryDataset* pHistoryDataset;
public:
	CHistoryViewDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CHistoryViewDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGHistoryView };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//CListCtrl HistoryList;
	CMyCListCtrl HistoryList;
	CEdit TargetHistoryEdit;
	CComboBox CmbForHistory;
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
