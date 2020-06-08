// CReaderLoginDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CReaderLoginDlg.h"
#include "afxdialogex.h"


// CReaderLoginDlg 对话框

IMPLEMENT_DYNAMIC(CReaderLoginDlg, CDialogEx)

CReaderLoginDlg::CReaderLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGReaderLogin, pParent)
{

}

CReaderLoginDlg::~CReaderLoginDlg()
{
}

void CReaderLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CReaderLoginDlg, CDialogEx)
END_MESSAGE_MAP()


// CReaderLoginDlg 消息处理程序
