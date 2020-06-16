// CReaderChangeDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "MFCTest03Dlg.h"
#include "CReaderChangeDlg.h"
#include "afxdialogex.h"


// CReaderChangeDlg 对话框

IMPLEMENT_DYNAMIC(CReaderChangeDlg, CDialogEx)

CReaderChangeDlg::CReaderChangeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGReaderEdit, pParent)
{
	pReaderDataset = new CReaderDataset;
	pReaderDataset->readAllData();
}

CReaderChangeDlg::~CReaderChangeDlg()
{
}

void CReaderChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, OriginPassword);
	DDX_Control(pDX, IDC_EDIT2, TobeChangeName);
	DDX_Control(pDX, IDC_EDIT3, TobeChangePassword);
	DDX_Control(pDX, IDC_EDIT4, TobeChangePasswordRepeat);

	TobeChangeName.SetWindowTextA(CString(pReaderDataset->getNameById(CMFCTest03Dlg::NowLoginReader)));
}


BEGIN_MESSAGE_MAP(CReaderChangeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CReaderChangeDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CReaderChangeDlg::OnBnClickedButton2)

END_MESSAGE_MAP()


// CReaderChangeDlg 消息处理程序

//尝试修改读者信息
void CReaderChangeDlg::OnBnClickedButton1()
{
	
	//由读者码和原密码检查是否密码正确
	CString s1;
	CString s2, s3, s4;
	TobeChangeName.GetWindowTextA(s2);
	TobeChangePassword.GetWindowTextA(s3);
	TobeChangePasswordRepeat.GetWindowTextA(s4);

	OriginPassword.GetWindowTextA(s1);
	if (s1.IsEmpty()) 
	{
		MessageBox(_T(CString("请输入原密码")), _T(""), MB_OK | MB_ICONINFORMATION);
		return;
	}
	int tmp = pReaderDataset->CheckIfHasTheReader(CMFCTest03Dlg::NowLoginReader, s1);
	if (!tmp) 
	{
		MessageBox(_T(CString("原密码错误")), _T(""), MB_OK | MB_ICONINFORMATION);
		OriginPassword.SetSel(0, -1);
	}else if (s2.IsEmpty())
	{
		MessageBox(_T(CString("账户名字不能为空")), _T(""), MB_OK | MB_ICONINFORMATION);
		TobeChangeName.SetSel(0, -1);
	}else if (s3.IsEmpty())
	{
		MessageBox(_T(CString("密码不能为空")), _T(""), MB_OK | MB_ICONINFORMATION);
		TobeChangePassword.SetSel(0, -1);
		}
	else if (s4.Compare(s3) == 0)
	{
		MessageBox(_T(CString("重复密码不匹配")), _T(""), MB_OK | MB_ICONINFORMATION);
		TobeChangePasswordRepeat.SetSel(0, -1);
	}else
	{
		pReaderDataset->ChangeNameById(CMFCTest03Dlg::NowLoginReader, s2);
		pReaderDataset->ChangePasswordById(CMFCTest03Dlg::NowLoginReader, s3);
		MessageBox(_T(CString("账户资料已修改")), _T(""), MB_OK | MB_ICONINFORMATION);
	}
	pReaderDataset->saveAllDataToFile();
	OnOK();
	OnClose();
}

void CReaderChangeDlg::OnBnClickedButton2()
{
	OnOK();
	OnClose();
}
