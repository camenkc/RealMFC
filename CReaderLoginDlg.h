#pragma once


#include "./DataIO/DataIO.h"
#include "./MyControls/MyControls.h"
// CReaderLoginDlg 对话框

class CReaderLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReaderLoginDlg)

public:
	CReaderLoginDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CReaderLoginDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGReaderLogin };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
