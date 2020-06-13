// CReaderLoginDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CReaderLoginDlg.h"
#include "MFCTest03Dlg.h"
#include "afxdialogex.h"


// CReaderLoginDlg 对话框

IMPLEMENT_DYNAMIC(CReaderLoginDlg, CDialogEx)

CReaderLoginDlg::CReaderLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGReaderLogin, pParent)
{
	pReaderDataset = NULL;
}

CReaderLoginDlg::~CReaderLoginDlg()
{
}

void CReaderLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	GetDlgItem(IDC_EDIT1)->SetFocus();
	DDX_Control(pDX, IDC_EDIT1, ReaderNameEdit);
	DDX_Control(pDX, IDC_EDIT7, ReaderPasswordEdit);
}


BEGIN_MESSAGE_MAP(CReaderLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CReaderLoginDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CReaderLoginDlg 消息处理程序

//按下登录按钮
void CReaderLoginDlg::OnBnClickedButton1()
{
	CString s1, s2;//账号名、密码
	ReaderNameEdit.GetWindowTextA(s1);
	ReaderPasswordEdit.GetWindowTextA(s2);
	int tmp=pReaderDataset->CheckIfHasTheReader(s1, s2);
	if (tmp) 
	{
		MessageBox(_T(CString("登陆成功，欢迎你，")+s1), _T(""), MB_OK | MB_ICONINFORMATION);
		CMFCTest03Dlg::NowLoginReader = tmp;
		OnOK();
		OnClose();
	}
	else 
	{
		MessageBox(_T(CString("用户不存在或密码错误")), _T(""), MB_OK | MB_ICONINFORMATION);
		GetDlgItem(IDC_EDIT7)->SetWindowTextA("");
		GetDlgItem(IDC_EDIT1)->SetFocus();
	}
}


BOOL CReaderLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	pReaderDataset = new CReaderDataset;
	pReaderDataset->readAllData();

	return FALSE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL CReaderLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		//当案件为enter时自动登录
		if (pMsg->wParam == VK_RETURN )
		{
			OnBnClickedButton1();
			return TRUE;

		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
