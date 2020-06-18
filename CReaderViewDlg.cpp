// CReaderViewDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CReaderViewDlg.h"
#include "afxdialogex.h"
#include "MFCTest03Dlg.h"


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
	DDX_Control(pDX, IDC_COMBO1, CmbForReader);
	DDX_Control(pDX, IDC_EDIT1, TargetReaderEdit);
}


BEGIN_MESSAGE_MAP(CReaderViewDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CReaderViewDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CReaderViewDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &CReaderViewDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CReaderViewDlg 消息处理程序


BOOL CReaderViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//初始化列表
	pReaderDataset->readAllData();
	if (CMFCTest03Dlg::NowLoginReader != 1)//非管理员看不到账号、用户权限
	{
		pReaderDataset->dataToListCtrl(&ReaderList,NULL,1);
	}
	else
	{
		pReaderDataset->dataToListCtrl(&ReaderList);
	}
	const int loginreader = CMFCTest03Dlg::NowLoginReader;
	TargetReaderEdit.SetFocus();
	CmbForReader.SetCurSel(0);
	


	return FALSE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//应用筛选条件
void CReaderViewDlg::OnBnClickedOk()
{
	int index = CmbForReader.GetCurSel();//获得当前选择的为列表当前的第几个 0代表第一个
	CString sVal;
	TargetReaderEdit.GetWindowTextA(sVal);
	CString sField;
	if (index == 0) {//选择的是读者号
		sField = "Id";
	}
	else {//选择的是账号名
		sField = "姓名";
	}
	if (sVal.IsEmpty())
	{
		MessageBox(_T(CString("请输入参数")), _T(""), MB_OK | MB_ICONINFORMATION);
		TargetReaderEdit.SetFocus();
		TargetReaderEdit.SetSel(0, -1);
		return;
	}
	vector<CReaderData> tmpReaderDataset;//临时的读者集 存放需要找到的目标读者

	pReaderDataset->selectData(tmpReaderDataset, sField, eEqual, sVal);
	if (CMFCTest03Dlg::NowLoginReader != 1)//非管理员看不到账号密码
	{
		pReaderDataset->dataToListCtrl(&ReaderList, &tmpReaderDataset, 1);
	}
	else
	{
		pReaderDataset->dataToListCtrl(&ReaderList, &tmpReaderDataset);
		
	}
	const int loginreader = CMFCTest03Dlg::NowLoginReader;




}

//回到初始状态
void CReaderViewDlg::OnBnClickedButton1()
{
	if (CMFCTest03Dlg::NowLoginReader != 1)//非管理员看不到账号、用户权限
	{
		pReaderDataset->dataToListCtrl(&ReaderList, NULL, 1);
	}
	else
	{
		pReaderDataset->dataToListCtrl(&ReaderList);
	}
	TargetReaderEdit.SetFocus();
	TargetReaderEdit.SetSel(0, -1);
	const int loginreader = CMFCTest03Dlg::NowLoginReader;



}
BOOL CReaderViewDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		//当按下回车时自动查询
		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedOk();
			return TRUE;

		}
		else if (pMsg->message == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}



void CReaderViewDlg::OnBnClickedCancel()
{
	OnOK();
	OnClose();
}
