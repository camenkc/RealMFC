// CBorrowDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CBorrowDlg.h"
#include "MFCTest03Dlg.h"
#include "afxdialogex.h"


// CBorrowDlg 对话框

IMPLEMENT_DYNAMIC(CBorrowDlg, CDialogEx)

CBorrowDlg::CBorrowDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGBookBorrow, pParent)
{
	pBookDataset = new CBookDataset();
	pBorrowDataset = new CBorrowDataset();
}

CBorrowDlg::~CBorrowDlg()
{

}

void CBorrowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, BorrowBookId);
	DDX_Control(pDX, IDC_EDIT2, BorrowBookName);
}


BEGIN_MESSAGE_MAP(CBorrowDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CBorrowDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CBorrowDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CBorrowDlg 消息处理程序

//点击了确认借阅按钮
void CBorrowDlg::OnBnClickedButton1()
{
	pBookDataset->readAllData();
	pBorrowDataset->readAllData();
	CString s1,s2;
	BorrowBookId.GetWindowTextA(s1);		
	BorrowBookName.GetWindowTextA(s2);
	if (!s1.IsEmpty()) {
		int tmp = pBookDataset->CheckIfHaveTheBookById(s1);
		
		if (tmp) {//需要借阅这样一本书
			int BorrowNumber = pBorrowDataset->getMaxVal("Id");
			BorrowNumber++;
			int BookId = _ttoi(s1);
			CBorrowData *addData= new CBorrowData(BorrowNumber, CMFCTest03Dlg::NowLoginReader, BookId);
			pBorrowDataset->saveOneItemToFile(addData);
			delete addData;
			MessageBox(CString("图书借阅成功\n您借阅的图书为：") + pBookDataset->GetBookNameById(s1));
		}
		else {
			MessageBox("此图书不存在或已删除");
		}
	}
	else {
		if (s2.IsEmpty()) {
			MessageBox("错误，未指定图书");
			return;
		}
		CString tmp = pBookDataset->CheckIfHaveTheBookByName(s2);
		if (tmp.Compare("") != 0) {
			BorrowBookId.SetWindowTextA(tmp);
			//需要借阅这样一本书
			int BorrowNumber = pBorrowDataset->getMaxVal("Id");
			BorrowNumber++;
			int BookId = _ttoi(tmp);
			CBorrowData* addData = new CBorrowData(BorrowNumber, CMFCTest03Dlg::NowLoginReader, BookId);
			pBorrowDataset->saveOneItemToFile(addData);
			delete addData; 
			MessageBox(CString("图书借阅成功\n您借阅的图书为：") + pBookDataset->GetBookNameById(s1));

		}

	}

}


void CBorrowDlg::OnBnClickedButton2()
{
	OnOK();
	OnClose();
}
