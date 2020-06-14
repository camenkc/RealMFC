// CReaderViewDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CReaderViewDlg.h"
#include "afxdialogex.h"


// CReaderViewDlg 对话框

IMPLEMENT_DYNAMIC(CReaderViewDlg, CDialogEx)

CReaderViewDlg::CReaderViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGReaderView, pParent)
{
	pReaderDataset = new CReaderDataset();
}

CReaderViewDlg::~CReaderViewDlg()
{
}

void CReaderViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, ReaderList);
}


BEGIN_MESSAGE_MAP(CReaderViewDlg, CDialogEx)
END_MESSAGE_MAP()


// CReaderViewDlg 消息处理程序


BOOL CReaderViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//初始化列表
	pReaderDataset->readAllData();
	
	pReaderDataset->dataToListCtrl(&ReaderList);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
