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
	DDX_Control(pDX, IDC_COMBO1, CmbForReader);
	DDX_Control(pDX, IDC_EDIT1, TargetReaderEdit);
}


BEGIN_MESSAGE_MAP(CReaderViewDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CReaderViewDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CReaderViewDlg::OnBnClickedButton1)
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

	vector<CReaderData> tmpReaderDataset;//临时的读者集 存放需要找到的目标读者

	pReaderDataset->selectData(tmpReaderDataset, sField, eEqual, sVal);

	pReaderDataset->dataToListCtrl(&ReaderList, &tmpReaderDataset);
	
}

//回到初始状态
void CReaderViewDlg::OnBnClickedButton1()
{
	pReaderDataset->dataToListCtrl(&ReaderList);
}
