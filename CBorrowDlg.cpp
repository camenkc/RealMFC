// CBorrowDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CBorrowDlg.h"
#include "afxdialogex.h"


// CBorrowDlg 对话框

IMPLEMENT_DYNAMIC(CBorrowDlg, CDialogEx)

CBorrowDlg::CBorrowDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGBookBorrow, pParent)
{

}

CBorrowDlg::~CBorrowDlg()
{
}

void CBorrowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBorrowDlg, CDialogEx)
END_MESSAGE_MAP()


// CBorrowDlg 消息处理程序
