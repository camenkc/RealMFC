#pragma once



#include "./DataIO/DataIO.h"
#include "./MyControls/MyControls.h"
// CBorrowDlg 对话框

class CBorrowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBorrowDlg)

public:
	CBorrowDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBorrowDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGBookBorrow };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
