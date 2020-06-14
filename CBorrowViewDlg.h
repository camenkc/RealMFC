#pragma once

#include "./DataIO/DataIO.h"
#include "./MyControls/MyControls.h"

// CBorrowViewDlg 对话框

class CBorrowViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBorrowViewDlg)
	CBorrowDataset* pBorrowDataset;
public:
	CBorrowViewDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBorrowViewDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGBorrowVIew };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//CListCtrl BorrowList;
	CMyCListCtrl BorrowList;
	virtual BOOL OnInitDialog();
	CEdit TargetBorrowEdit;
	CComboBox CmbForBorrow;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
