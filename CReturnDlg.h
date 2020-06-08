#pragma once



#include "./DataIO/DataIO.h"
#include "./MyControls/MyControls.h"
// CReturnDlg 对话框

class CReturnDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReturnDlg)

public:
	CReturnDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CReturnDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGBookReturn };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
