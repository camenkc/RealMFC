#pragma once

#include "./DataIO/DataIO.h"
#include "./MyControls/MyControls.h"

// CReaderChangeDlg 对话框

class CReaderChangeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReaderChangeDlg)
	CReaderDataset* pReaderDataset;
public:
	CReaderChangeDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CReaderChangeDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGReaderEdit };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit OriginPassword;
	CEdit TobeChangeName;
	CEdit TobeChangePassword;
	afx_msg void OnBnClickedButton1();
	CEdit TobeChangePasswordRepeat;
	afx_msg void OnBnClickedButton2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
