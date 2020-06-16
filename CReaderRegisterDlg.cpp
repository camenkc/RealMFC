// CReaderRegister.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CReaderRegisterDlg.h"
#include "afxdialogex.h"
#include "MFCTest03Dlg.h"
CEdit* pedit;

// CReaderRegisterDlg 对话框

IMPLEMENT_DYNAMIC(CReaderRegisterDlg, CDialogEx)

CReaderRegisterDlg::CReaderRegisterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGReaderRegist, pParent)
{
	pReaderDataset = new CReaderDataset;
	//读入所有的读者数据
}

CReaderRegisterDlg::~CReaderRegisterDlg()
{
}

void CReaderRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	GetDlgItem(IDC_EDIT3)->SetFocus();
	DDX_Control(pDX, IDC_EDIT2, ReaderNumberEditBox);
	DDX_Control(pDX, IDC_EDIT3, ReaderNameEditBox);
	DDX_Control(pDX, IDC_EDIT5, ReaderPassword);
	DDX_Control(pDX, IDC_EDIT6, ReaderPasswordRepeat);
}



// CReaderRegister 消息处理程序


BEGIN_MESSAGE_MAP(CReaderRegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CReaderRegisterDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

//按下了注册按钮
void CReaderRegisterDlg::OnBnClickedButton1()
{
	//自动填充读者号
	int maxReadrNum = pReaderDataset->getMaxVal("Id");
	maxReadrNum += 1;
	CString s1,s2,s3,ss3,s4;//读者号、账号名、密码、重复密码、用户权限
	s1.Format("%d", maxReadrNum);
	ReaderNumberEditBox.SetWindowTextA(s1);

	ReaderNameEditBox.GetWindowTextA(s2);
	if (s2.IsEmpty()) 
	{
		MessageBox(_T(CString("账号名不能为空")), _T(""), MB_OK | MB_ICONINFORMATION);
		GetDlgItem(IDC_EDIT3)->SetFocus();
		return;
	}
	ReaderPassword.GetWindowTextA(s3);
	if (s3.IsEmpty()) 
	{
		MessageBox(_T(CString("密码不能为空")), _T(""), MB_OK | MB_ICONINFORMATION);
		GetDlgItem(IDC_EDIT5)->SetFocus();
		return;
	}
	ReaderPasswordRepeat.GetWindowTextA(ss3);
	if (ss3 != s3) 
	{

		MessageBox(_T(CString("重复密码与密码不一致")), _T(""), MB_OK | MB_ICONINFORMATION);
		GetDlgItem(IDC_EDIT6)->SetFocus();
	
		pedit = (CEdit*)GetDlgItem(IDC_EDIT6);
			pedit->SetSel(0,-1);
		return;
	}
	if (maxReadrNum == 1)//表示是第一个创建的读者 用户权限为管理员
	{
		CReaderData tmpReaderData(maxReadrNum, s3.GetBuffer(), s2.GetBuffer(), 1);
		pReaderDataset->saveOneItemToFile(&tmpReaderData);
		MessageBox(_T(CString("你就是管理员！")), _T(""), MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		CReaderData tmpReaderData(maxReadrNum, s3.GetBuffer(), s2.GetBuffer(), 0);
		//判重 账号名有相同时 报错
		CReaderData* compareReaderData = pReaderDataset->getItemByName(s2.GetBuffer());
		if (compareReaderData != NULL) 
		{
			MessageBox(_T(CString("账号名已被注册")), _T(""), MB_OK | MB_ICONINFORMATION);
			return;
		}
		pReaderDataset->saveOneItemToFile(&tmpReaderData);
		MessageBox(_T(CString("成功注册！\n可使用账号名与密码登录！\n你的读书号为") + s1), _T(""), MB_OK | MB_ICONINFORMATION);
	
		if (::MessageBox(NULL,CString("是否使用该账户登录") ,"消息确认...", MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			CMFCTest03Dlg::NowLoginReader = maxReadrNum;

		}

		//ReaderNameEditBox.SetWindowTextA("");
		//OnInitDialog();
		//ReaderPassword.SetWindowTextA("");
		//ReaderPasswordRepeat.SetWindowTextA("");
	
		OnOK();
		OnClose();
	

	}
}


BOOL CReaderRegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	pReaderDataset->readAllData();
	//自动填充读者号
	int maxReadrNum = pReaderDataset->getMaxVal("Id");
	maxReadrNum += 1;
	CString s1, s2, s3, ss3, s4;//读者号、账号名、密码、重复密码、用户权限
	s1.Format("%d", maxReadrNum);
	ReaderNumberEditBox.SetWindowTextA(s1);
	return FALSE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL CReaderRegisterDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) 
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) 
		{
			OnBnClickedButton1();
			return TRUE;
		}
		else if (pMsg->message == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
