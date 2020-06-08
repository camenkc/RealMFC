#pragma once


#include "./DataIO/DataIO.h"
#include "./MyControls/MyControls.h"
// CDlgBookView 对话框

class CDlgBookView : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBookView)

	CBookDataset* m_pBookDataset;//图书数据集

	vector<CString> aField;
public:
	CDlgBookView(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgBookView();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGBOOKVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMyCListCtrl list;
	CComboBox cmbForSort;
	CComboBox cmbForSel;
	CComboBox cmbForCmp;

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CComboBox cmbForSelCmp;
	CEdit edText;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
//	afx_msg void OnCbnSelendcancelCombo1();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnEnChangeEdit1();
};
