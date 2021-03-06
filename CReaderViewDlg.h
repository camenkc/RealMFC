﻿#pragma once


#include "./DataIO/DataIO.h"
#include "./MyControls/MyControls.h"
// CReaderViewDlg 对话框

class CReaderViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReaderViewDlg)

public:
	CReaderViewDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CReaderViewDlg();

	CReaderDataset* pReaderDataset;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGReaderView };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//CListCtrl ReaderList;
	CMyCListCtrl ReaderList;
	virtual BOOL OnInitDialog();
	CComboBox CmbForReader;
	CEdit TargetReaderEdit;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCancel();
};
