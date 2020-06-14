// CHistoryViewDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CHistoryViewDlg.h"
#include "afxdialogex.h"


// CHistoryViewDlg 对话框

IMPLEMENT_DYNAMIC(CHistoryViewDlg, CDialogEx)

CHistoryViewDlg::CHistoryViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGHistoryView, pParent)
{
	pHistoryDataset = new CHistoryDataset();
}

CHistoryViewDlg::~CHistoryViewDlg()
{
}

void CHistoryViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, HistoryList);
}


BEGIN_MESSAGE_MAP(CHistoryViewDlg, CDialogEx)
END_MESSAGE_MAP()


// CHistoryViewDlg 消息处理程序


BOOL CHistoryViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	pHistoryDataset->readAllData();

	pHistoryDataset->dataToListCtrl(&HistoryList);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
