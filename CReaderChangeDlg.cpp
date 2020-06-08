// CReaderChangeDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CReaderChangeDlg.h"
#include "afxdialogex.h"


// CReaderChangeDlg 对话框

IMPLEMENT_DYNAMIC(CReaderChangeDlg, CDialogEx)

CReaderChangeDlg::CReaderChangeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGReaderEdit, pParent)
{

}

CReaderChangeDlg::~CReaderChangeDlg()
{
}

void CReaderChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CReaderChangeDlg, CDialogEx)
END_MESSAGE_MAP()


// CReaderChangeDlg 消息处理程序
