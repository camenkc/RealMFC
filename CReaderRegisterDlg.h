#pragma once

#include "./DataIO/DataIO.h"
#include "./MyControls/MyControls.h"

// CReaderRegister 对话框

class CReaderRegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReaderRegisterDlg)

public:
	CReaderRegisterDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CReaderRegisterDlg();

	CReaderDataset* pReaderDataset=NULL;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGReaderRegist };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	//DECLARE_MESSAGE_MAP()
public:
	CEdit ReaderNumberEditBox;
	CEdit ReaderNameEditBox;
	CEdit ReaderPassword;
	CEdit ReaderPasswordRepeat;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
