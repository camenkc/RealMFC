// CReturnDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CReturnDlg.h"
#include "afxdialogex.h"


// CReturnDlg 对话框

IMPLEMENT_DYNAMIC(CReturnDlg, CDialogEx)

CReturnDlg::CReturnDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGBookReturn, pParent)
{

}

CReturnDlg::~CReturnDlg()
{
}

void CReturnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CReturnDlg, CDialogEx)
END_MESSAGE_MAP()


// CReturnDlg 消息处理程序
