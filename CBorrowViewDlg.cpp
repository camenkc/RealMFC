// CBorrowViewDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CBorrowViewDlg.h"
#include "afxdialogex.h"


// CBorrowViewDlg 对话框

IMPLEMENT_DYNAMIC(CBorrowViewDlg, CDialogEx)

CBorrowViewDlg::CBorrowViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGBorrowVIew, pParent)
{
	pBorrowDataset = new CBorrowDataset();
}

CBorrowViewDlg::~CBorrowViewDlg()
{
}

void CBorrowViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, BorrowList);
}


BEGIN_MESSAGE_MAP(CBorrowViewDlg, CDialogEx)
END_MESSAGE_MAP()


// CBorrowViewDlg 消息处理程序


BOOL CBorrowViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	pBorrowDataset->readAllData();

	pBorrowDataset->dataToListCtrl(&BorrowList);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
