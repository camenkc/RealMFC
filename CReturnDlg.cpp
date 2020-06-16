// CReturnDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CReturnDlg.h"
#include "MFCTest03Dlg.h"
#include "afxdialogex.h"

// CReturnDlg 对话框

IMPLEMENT_DYNAMIC(CReturnDlg, CDialogEx)

CReturnDlg::CReturnDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGBookReturn, pParent)
{
	pBorrowDataset = new CBorrowDataset();
	pHistoryDataset = new CHistoryDataset();
	pBookDataset = new CBookDataset();
}

CReturnDlg::~CReturnDlg()
{
}

void CReturnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT3, ReturnBookId);
	DDX_Control(pDX, IDC_EDIT2, ReturnBookName);
}


BEGIN_MESSAGE_MAP(CReturnDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CReturnDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CReturnDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CReturnDlg 消息处理程序

//确认归还图书
void CReturnDlg::OnBnClickedButton2()
{
	/*
	归还图书的流程：
	首先判断是否在在借记录里有这本书和借出人
	接下来在历史记录里创建一条新记录 将历史记录编号、借出人编号、借出书编号、借出日期、时间、归还日期、时间
	将在借记录中的那一条记录删除

	涉及到的数据集：CBorrowDataset、CReturnDataset、CBookDataset
	*/
	pBookDataset->readAllData();
	pBorrowDataset->readAllData();
	pHistoryDataset->readAllData();
	CString s1, s2;
	ReturnBookId.GetWindowTextA(s1);
	ReturnBookName.GetWindowTextA(s2);

	if (s1.IsEmpty()) {
		if (s2.IsEmpty()) {
			MessageBox(_T(CString("请输入图书编号或者图书名称")), _T(""), MB_OK | MB_ICONINFORMATION);
		
		}
		else {
			CString BookId = pBookDataset->CheckIfHaveTheBookByName(s2);
			if (BookId.Compare("") != 0) {
				s1 = BookId;
			}
			else {
				MessageBox(_T(CString("未找到该图书，请检查图书名称\n或通知管理员书籍已被删除")), _T(""), MB_OK | MB_ICONINFORMATION);
			}
		}
	}

	CString ReaderId;
	ReaderId.Format("%d", CMFCTest03Dlg::NowLoginReader);
	//查询是否有这样的在借记录(读者编号、图书编号) tmp记录的是在借编号 以便马上删除用
	CString tmp = pBorrowDataset->CheckIfHasBorrowData(s1, ReaderId);
	if (tmp.Compare("") != 0) {
		//新建一条History记录
		int nHistoryId = pHistoryDataset->getMaxVal("Id");
		nHistoryId++;
		int nBookId = _ttoi(s1);
		int nBorrowId = _ttoi(tmp);
		CBorrowData* TarBorrowData = pBorrowDataset->getItemByKeyVal("Id", nBorrowId);
		CString BorrowDate = (*TarBorrowData)[3];
		CString BorrowTime = (*TarBorrowData)[4];
		CHistoryData addData(nHistoryId, nBookId, CMFCTest03Dlg::NowLoginReader, BorrowDate.GetBuffer(), BorrowTime.GetBuffer());
		pHistoryDataset->saveOneItemToFile(&addData);
		//从BorrowDataset中删除tmp编号
		pBorrowDataset->deleteItemByKeyVal("Id", nBorrowId);
		pBorrowDataset->saveAllDataToFile();
		MessageBox(_T(CString("已归还")), _T(""), MB_OK | MB_ICONINFORMATION);
	
	}
	else {
		MessageBox(_T(CString("您没有借出此本书")), _T(""), MB_OK | MB_ICONINFORMATION);
		return;
	}



}


void CReturnDlg::OnBnClickedButton1()
{
	OnOK();
	OnClose();
}
