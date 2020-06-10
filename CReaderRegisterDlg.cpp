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
	pReaderDataset = new CReaderDataset;
	//读入所有的读者数据
}

CReaderRegisterDlg::~CReaderRegisterDlg()
{
}

void CReaderRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
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
	pReaderDataset->readAllData();
	//自动填充读者号
	int maxReadrNum = pReaderDataset->getMaxVal("Id");
	maxReadrNum += 1;
	CString s1,s2,s3,ss3,s4;//读者号、账号名、密码、重复密码、用户权限
	s1.Format("%d", maxReadrNum);
	ReaderNumberEditBox.SetWindowTextA(s1);

	ReaderNameEditBox.GetWindowTextA(s2);
	if (s2.IsEmpty()) {
		MessageBox("账号名不能为空");
		return;
	}
	ReaderPassword.GetWindowTextA(s3);
	if (s3.IsEmpty()) {
		MessageBox("密码不能为空");
		return;
	}
	ReaderPasswordRepeat.GetWindowTextA(ss3);
	if (ss3.IsEmpty()) {
		MessageBox("重复密码不能为空");
		return;
	}
	if (ss3 != s3) {
		MessageBox("密码与重复密码不一致");
		return;
	}
	if (maxReadrNum == 1)//表示是第一个创建的读者 用户权限为管理员
	{
		CReaderData tmpReaderData(maxReadrNum, s3.GetBuffer(), s2.GetBuffer(), 1);
		pReaderDataset->saveOneItemToFile(&tmpReaderData);
		MessageBox("你就是管理员！");
	}
	else
	{
		CReaderData tmpReaderData(maxReadrNum, s3.GetBuffer(), s2.GetBuffer(), 0);
		//判重 账号名有相同时 报错
		CReaderData* compareReaderData = pReaderDataset->getItemByName(s2.GetBuffer());
		if (compareReaderData != NULL) {
			MessageBox("账号名已被注册");
			return;
		}
		pReaderDataset->saveOneItemToFile(&tmpReaderData);
		MessageBox(CString("成功注册！\n可使用账号名+密码登录！\n为方便管理，你的读书号为")+s1);
		ReaderNameEditBox.SetWindowTextA("");
		OnInitDialog();
		ReaderPassword.SetWindowTextA("");
		ReaderPasswordRepeat.SetWindowTextA("");
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
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
