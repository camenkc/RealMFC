#pragma once

#include "./DataIO/DataIO.h"
#include "./MyControls/MyControls.h"

// CReaderChangeDlg 对话框

class CReaderChangeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReaderChangeDlg)

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
};
