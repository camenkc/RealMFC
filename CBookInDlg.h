#pragma once

#include "./DataIO/DataIO.h"
#include "./MyControls/MyControls.h"
// CBookInDlg 对话框

class CBookInDlg : public CDialogEx
{
private:
	DECLARE_DYNAMIC(CBookInDlg)

	CBookDataset* m_pBookDataset;

	void ClearEditText();

	int nMayNeedToBeDelete;//辅助变量 用来记录可能会被删除的那一行（双击选中的那一行）

public:
	CBookInDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBookInDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOOKINPUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult);
	//CListCtrl listBook;
	CMyCListCtrl listBook;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton4();
};
