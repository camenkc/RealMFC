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
}


BEGIN_MESSAGE_MAP(CReaderChangeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CReaderChangeDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CReaderChangeDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CReaderChangeDlg 消息处理程序

//尝试修改读者信息
void CReaderChangeDlg::OnBnClickedButton1()
{
	
	pReaderDataset->readAllData();
	//由读者码和原密码检查是否密码正确
	CString s1;
	OriginPassword.GetWindowTextA(s1);
	if (s1.IsEmpty()) {
		MessageBox("请输入原密码"); return;
	}
	int tmp = pReaderDataset->CheckIfHasTheReader(CMFCTest03Dlg::NowLoginReader, s1);
	if (!tmp) {
		MessageBox("原密码错误"); return;
	}
	CString s2, s3;
	TobeChangeName.GetWindowTextA(s2);
	TobeChangePassword.GetWindowTextA(s3);
	if (!s2.IsEmpty()) {
		pReaderDataset->ChangeNameById(CMFCTest03Dlg::NowLoginReader, s2);
		MessageBox("账户名称已改变");
		TobeChangeName.SetWindowTextA("");
	}
	if (!s3.IsEmpty()) {
		CString s4;
		TobeChangePasswordRepeat.GetWindowTextA(s4);
		if (s4.Compare(s3) == 0) {
			pReaderDataset->ChangePasswordById(CMFCTest03Dlg::NowLoginReader, s3);
			MessageBox("账户密码已改变");
			TobeChangePassword.SetWindowTextA("");
			TobeChangePasswordRepeat.SetWindowTextA("");
		}
		else {
			MessageBox("重复密码不匹配，账户密码未修改");
		}
	}
	pReaderDataset->saveAllDataToFile();
}

void CReaderChangeDlg::OnBnClickedButton2()
{
	OnOK();
	OnClose();
}
