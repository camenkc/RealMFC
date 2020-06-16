// CBorrowViewDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CBorrowViewDlg.h"
#include "afxdialogex.h"


// CBorrowViewDlg 对话框

IMPLEMENT_DYNAMIC(CBorrowViewDlg, CDialogEx)

CBorrowViewDlg::CBorrowViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGBorrowVIew, pParent)
{
	pBorrowDataset = new CBorrowDataset();
}

CBorrowViewDlg::~CBorrowViewDlg()
{
}

void CBorrowViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, BorrowList);
	DDX_Control(pDX, IDC_EDIT1, TargetBorrowEdit);
	DDX_Control(pDX, IDC_COMBO1, CmbForBorrow);
}


BEGIN_MESSAGE_MAP(CBorrowViewDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBorrowViewDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CBorrowViewDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CBorrowViewDlg 消息处理程序


BOOL CBorrowViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	pBorrowDataset->readAllData();

	pBorrowDataset->dataToListCtrl(&BorrowList);

	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//应用数据筛选
void CBorrowViewDlg::OnBnClickedOk()
{
	CBorrowData tmp;
	//借出编号; 借出书籍编号; 借出读者编号; 借出日期;
	int index = CmbForBorrow.GetCurSel();
	CString sField;
	CString sVal;
	TargetBorrowEdit.GetWindowTextA(sVal);
	if (index == 0) {
		sField = "Id";
	}
	else if (index == 1) {
		sField = "图书Id";
	}
	else if (index == 2) {
		sField = "读者Id";
	}
	else if (index == 3) {
		sField = "日期";
	}

	vector<CBorrowData> tmpBorrowDataset;

	pBorrowDataset->selectData(tmpBorrowDataset, sField, eEqual, sVal);

	pBorrowDataset->dataToListCtrl(&BorrowList, &tmpBorrowDataset);

	
}

//回到初始状态
void CBorrowViewDlg::OnBnClickedButton1()
{
	pBorrowDataset->dataToListCtrl(&BorrowList);
}
