// CReaderRegister.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CReaderRegisterDlg.h"
#include "afxdialogex.h"


// CReaderRegisterDlg 对话框

IMPLEMENT_DYNAMIC(CReaderRegisterDlg, CDialogEx)

CReaderRegisterDlg::CReaderRegisterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGReaderRegist, pParent)
{

}

CReaderRegisterDlg::~CReaderRegisterDlg()
{
}

void CReaderRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CReaderRegisterDlg, CDialogEx)
END_MESSAGE_MAP()


// CReaderRegister 消息处理程序
