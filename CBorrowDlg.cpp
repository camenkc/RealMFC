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
	pReaderDataset = new CReaderDataset();
}

CBorrowDlg::~CBorrowDlg()
{

}

void CBorrowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, BorrowBookId);
	DDX_Control(pDX, IDC_EDIT2, BorrowBookName);
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(BST_CHECKED);

}


BEGIN_MESSAGE_MAP(CBorrowDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CBorrowDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CBorrowDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_RADIO1, &CBorrowDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CBorrowDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CBorrowDlg 消息处理程序

//点击了确认借阅按钮
void CBorrowDlg::OnBnClickedButton1()
{
	pBookDataset->readAllData();
	pBorrowDataset->readAllData();
	pReaderDataset->readAllData();
	CString s1, s2;
	BorrowBookId.GetWindowTextA(s1);
	BorrowBookName.GetWindowTextA(s2);
	if (flag == 1)
	{
		int tmp = pBookDataset->CheckIfHaveTheBookById(s1);
		if (s1.IsEmpty())
		{
			MessageBox(_T(CString("请输入图书编号")), _T(""), MB_OK | MB_ICONINFORMATION);
			BorrowBookId.SetFocus();
			return;
		}
		if (tmp)
		{//需要借阅这样一本书

			int BorrowNumber = pBorrowDataset->getMaxVal("Id");
			BorrowNumber++;
			int BookId = _ttoi(s1);
			if (pBookDataset->HasBookLeftById(s1) == false)
			{
				MessageBox(_T(CString("图书库存不足")), _T(""), MB_OK | MB_ICONINFORMATION);
				BorrowBookId.SetFocus();
				return;
			}
			pBookDataset->DecBookNum(s1);
			CBookData* BookData = pBookDataset->getItemByKeyVal("Id", BookId);
			CReaderData* ReaderData = pReaderDataset->getItemByKeyVal("Id", CMFCTest03Dlg::NowLoginReader);
			CString BookName = (*BookData)[1];
			CString	ReaderName = (*ReaderData)[2];

			CBorrowData* addData = new CBorrowData(BorrowNumber, CMFCTest03Dlg::NowLoginReader, BookId, ReaderName.GetBuffer(), BookName.GetBuffer());
			pBorrowDataset->saveOneItemToFile(addData);
			pBookDataset->saveAllDataToFile();
			delete addData;
			MessageBox(_T(CString("图书借阅成功\n您借阅的图书为：") + pBookDataset->GetBookNameById(s1)), _T(""), MB_OK | MB_ICONINFORMATION);
			BorrowBookId.SetWindowTextA("");
			BorrowBookName.SetWindowTextA("");
			BorrowBookId.SetFocus();
			return;

		}
		else
		{
			MessageBox(_T(CString("此图书不存在或已删除")), _T(""), MB_OK | MB_ICONINFORMATION);
			BorrowBookId.SetFocus();
			BorrowBookId.SetSel(0, -1);
			return;

		}
	}
	else
	{
		if (s2.IsEmpty())
		{
			MessageBox(_T(CString("请输入图书名称")), _T(""), MB_OK | MB_ICONINFORMATION);
			BorrowBookName.SetFocus();
			return;
		}
		int avlb = pBookDataset->CheckIfHaveTheBookById(pBookDataset->CheckIfHaveTheBookByName(s2));
		if (avlb)
		{
			CString tmp = pBookDataset->CheckIfHaveTheBookByName(s2);
			if (tmp.Compare("") != 0)
			{
				//需要借阅这样一本书
				int BorrowNumber = pBorrowDataset->getMaxVal("Id");
				BorrowNumber++;
				int BookId = _ttoi(tmp);
				if (pBookDataset->HasBookLeftById(tmp) == false)
				{
					MessageBox(_T(CString("图书库存不足")), _T(""), MB_OK | MB_ICONINFORMATION);
					BorrowBookName.SetFocus();
					return;
				}
				pBookDataset->DecBookNum(tmp);
				CBookData* BookData = pBookDataset->getItemByKeyVal("Id", BookId);
				CReaderData* ReaderData = pReaderDataset->getItemByKeyVal("Id", CMFCTest03Dlg::NowLoginReader);
				CString BookName = (*BookData)[1];
				CString	ReaderName = (*ReaderData)[2];

				CBorrowData* addData = new CBorrowData(BorrowNumber, CMFCTest03Dlg::NowLoginReader, BookId, ReaderName.GetBuffer(), BookName.GetBuffer());
				pBorrowDataset->saveOneItemToFile(addData);
				pBookDataset->saveAllDataToFile();
				delete addData;
				MessageBox(_T(CString("图书借阅成功\n您借阅的图书为：") + s2), _T(""), MB_OK | MB_ICONINFORMATION);
				BorrowBookId.SetWindowTextA("");
				BorrowBookName.SetWindowTextA("");
				BorrowBookName.SetFocus();
			}

		}
		else
		{
			MessageBox(_T(CString("此图书不存在或已删除")), _T(""), MB_OK | MB_ICONINFORMATION);
			BorrowBookName.SetFocus();
			BorrowBookName.SetSel(0, -1);
			return;
		}

	}

}


void CBorrowDlg::OnBnClickedButton2()
{
	OnOK();
	OnClose();
}


void CBorrowDlg::OnBnClickedRadio1()
{
	flag = 1;
	BorrowBookId.EnableWindow(TRUE);
	BorrowBookName.EnableWindow(FALSE);
	BorrowBookId.SetFocus();
	BorrowBookId.SetSel(0, -1);
}


void CBorrowDlg::OnBnClickedRadio2()
{
	flag = 2;
	BorrowBookId.EnableWindow(FALSE);
	BorrowBookName.EnableWindow(TRUE);
	BorrowBookName.SetFocus();
	BorrowBookName.SetSel(0, -1);
}
BOOL CBorrowDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) 
	{
		//当案件为enter时查询
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

