// CHistoryViewDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CHistoryViewDlg.h"
#include "afxdialogex.h"


// CHistoryViewDlg 对话框

IMPLEMENT_DYNAMIC(CHistoryViewDlg, CDialogEx)

CHistoryViewDlg::CHistoryViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGHistoryView, pParent)
{
	pHistoryDataset = new CHistoryDataset();
}

CHistoryViewDlg::~CHistoryViewDlg()
{
}

void CHistoryViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, HistoryList);
	DDX_Control(pDX, IDC_EDIT1, TargetHistoryEdit);
	DDX_Control(pDX, IDC_COMBO1, CmbForHistory);

}


BEGIN_MESSAGE_MAP(CHistoryViewDlg, CDialogEx)
	
	ON_BN_CLICKED(IDC_BUTTON2, &CHistoryViewDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CHistoryViewDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CHistoryViewDlg 消息处理程序


BOOL CHistoryViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	pHistoryDataset->readAllData();

	pHistoryDataset->dataToListCtrl(&HistoryList);

	CmbForHistory.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



void CHistoryViewDlg::OnBnClickedButton2()
{
	pHistoryDataset->dataToListCtrl(&HistoryList);
}


void CHistoryViewDlg::OnBnClickedButton1()
{
	CHistoryData tmp;
	//借阅历史编号; 借出书籍编号; 借出读者编号; 借出日期;归还日期
	int index = CmbForHistory.GetCurSel();
	CString sField;
	CString sVal;
	TargetHistoryEdit.GetWindowTextA(sVal);
	if (sVal.IsEmpty())
	{
		MessageBox(_T(CString("请输入参数")), _T(""), MB_OK | MB_ICONINFORMATION);
		TargetHistoryEdit.SetFocus();
		return;
	}
	else
	{
		if (index == 0)
		{
			sField = "Id";
		}
		else if (index == 1)
		{
			sField = "图书Id";
		}
		else if (index == 2)
		{
			sField = "读者Id";
		}
		else if (index == 3)
		{
			sField = "借出日期";
		}
		else if (index == 4)
		{
			sField = "归还日期";
		}
	}

	vector<CHistoryData> tepHistoryDataset;

	pHistoryDataset->selectData(tepHistoryDataset, sField, eEqual, sVal);

	pHistoryDataset->dataToListCtrl(&HistoryList, &tepHistoryDataset);
}

BOOL CHistoryViewDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		//当案件为enter时自动登录
		if (pMsg->wParam == VK_RETURN)
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
