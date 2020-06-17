// CBackupDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CBackupDlg.h"
#include "afxdialogex.h"


// CBackupDlg 对话框

IMPLEMENT_DYNAMIC(CBackupDlg, CDialogEx)

CBackupDlg::CBackupDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGBackup, pParent)
{

}

CBackupDlg::~CBackupDlg()
{
}

void CBackupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, KeyEnter);
}


BEGIN_MESSAGE_MAP(CBackupDlg, CDialogEx)
	
	ON_BN_CLICKED(IDC_BUTTON1, &CBackupDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CBackupDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CBackupDlg 消息处理程序



void CBackupDlg::OnBnClickedButton1()
{
	CString key;
	KeyEnter.GetWindowTextA(key);
	if (key.IsEmpty())
	{
		MessageBox(_T(CString("请输入加密秘钥")), _T(""), MB_OK | MB_ICONINFORMATION);
		KeyEnter.SetFocus();
		KeyEnter.SetSel(0, -1);
		return;
	}
	else
	{
		if (pBackupset->BackupBykey(key))
		{
			MessageBox(_T(CString("备份成功，请记住备份秘钥\n"+key)), _T(""), MB_OK | MB_ICONINFORMATION);
			KeyEnter.SetFocus();
			return;
		}
		else
		{
			MessageBox(_T(CString("备份失败，请检查备份文件是否存在" + key)), _T(""), MB_OK | MB_ICONINFORMATION);
			KeyEnter.SetFocus();
			KeyEnter.SetSel(0, -1);
			return;
		}
	}
}


void CBackupDlg::OnBnClickedButton2()
{
	CString key;
	KeyEnter.GetWindowTextA(key);
	if (key.IsEmpty())
	{
		MessageBox(_T(CString("请输入加密秘钥")), _T(""), MB_OK | MB_ICONINFORMATION);
		KeyEnter.SetFocus();
		KeyEnter.SetSel(0, -1);
		return;
	}
	else
	{
		if (pBackupset->LoadingBykey(key))
		{
			MessageBox(_T(CString("恢复成功")), _T(""), MB_OK | MB_ICONINFORMATION);
			KeyEnter.SetFocus();
			return;
		}
		else
		{
			MessageBox(_T(CString("恢复失败，请检查备份文档" )), _T(""), MB_OK | MB_ICONINFORMATION);
			KeyEnter.SetFocus();
			KeyEnter.SetSel(0, -1);
			return;
		}
	}
}
