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
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(BST_CHECKED);
	flag = 1;
}


BEGIN_MESSAGE_MAP(CReturnDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CReturnDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CReturnDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO1, &CReturnDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CReturnDlg::OnBnClickedRadio2)
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

	CString ReaderId, BookId;
	ReaderId.Format("%d", CMFCTest03Dlg::NowLoginReader);

	if (flag == 1)
	{
		if (s1.IsEmpty())
		{
			
			MessageBox(_T(CString("请输入图书编号")), _T(""), MB_OK | MB_ICONINFORMATION);
			ReturnBookId.SetFocus();
			return;
		}
		else
		{
			
			if (pBookDataset->CheckIfHaveTheBookById(s1)!=1)
			{
				MessageBox(_T(CString("未找到该图书，请检查图书编号\n或通知管理员书籍已被删除")), _T(""), MB_OK | MB_ICONINFORMATION);
				ReturnBookId.SetFocus();
				ReturnBookId.SetSel(0, -1);
				return;
			}
			else
			{
				CString tmp = pBorrowDataset->CheckIfHasBorrowData(s1, ReaderId);
				if (tmp.Compare("") != 0) 
				{
					//新建一条History记录
					int nHistoryId = pHistoryDataset->getMaxVal("Id");
					nHistoryId++;
					int nBookId = _ttoi(s1);
					int nBorrowId = _ttoi(tmp);
					pBookDataset->IncBookNum(s1);
					CBorrowData* TarBorrowData = pBorrowDataset->getItemByKeyVal("Id", nBorrowId);
					CString BorrowDate = (*TarBorrowData)[5];
					CString BorrowTime = (*TarBorrowData)[6];
					CHistoryData addData(nHistoryId, nBookId, CMFCTest03Dlg::NowLoginReader, BorrowDate.GetBuffer(), BorrowTime.GetBuffer());
					pHistoryDataset->saveOneItemToFile(&addData);
					//从BorrowDataset中删除tmp编号
					pBorrowDataset->deleteItemByKeyVal("Id", nBorrowId);
					pBorrowDataset->saveAllDataToFile();
					pBookDataset->saveAllDataToFile();
					MessageBox(_T(CString("已归还图书:\n")+pBookDataset->GetBookNameById(s1)), _T(""), MB_OK | MB_ICONINFORMATION);
					ReturnBookId.SetWindowTextA("");
					ReturnBookName.SetWindowTextA("");
					ReturnBookId.SetFocus();
				
					return;
				}
				else 
				{
					MessageBox(_T(CString("您没有借出此本书")), _T(""), MB_OK | MB_ICONINFORMATION);
					ReturnBookId.SetFocus();
					ReturnBookId.SetSel(0, -1);
					return;
				}
			}
		}
	}
	else
	{
		if (s2.IsEmpty())
		{

			MessageBox(_T(CString("请输入图书名称")), _T(""), MB_OK | MB_ICONINFORMATION);
			ReturnBookName.SetFocus();
			return;
		}
		else
		{
			if (pBookDataset->CheckIfHaveTheBookByName(s2)!=1)
			{
				MessageBox(_T(CString("未找到该图书，请检查图书名称\n或通知管理员书籍已被删除")), _T(""), MB_OK | MB_ICONINFORMATION);
				ReturnBookName.SetFocus();
				ReturnBookName.SetSel(0, -1);
				return;
			}
			else
			{
				CString tmp = pBorrowDataset->CheckIfHasBorrowDataByName(s2, ReaderId);
				if (tmp.Compare("") != 0)
				{
					//新建一条History记录
					int nHistoryId = pHistoryDataset->getMaxVal("Id");
					nHistoryId++;
					int nBookId = _ttoi(pBookDataset->GetBookIdByName(s2));
					int nBorrowId = _ttoi(tmp);
					pBookDataset->IncBookNum(pBookDataset->GetBookIdByName(s2));
					CBorrowData* TarBorrowData = pBorrowDataset->getItemByKeyVal("Id", nBorrowId);
					CString BorrowDate = (*TarBorrowData)[3];
					CString BorrowTime = (*TarBorrowData)[4];
					CHistoryData addData(nHistoryId, nBookId, CMFCTest03Dlg::NowLoginReader, BorrowDate.GetBuffer(), BorrowTime.GetBuffer());
					pHistoryDataset->saveOneItemToFile(&addData);
					//从BorrowDataset中删除tmp编号
					pBorrowDataset->deleteItemByKeyVal("Id", nBorrowId);
					pBorrowDataset->saveAllDataToFile();
					pBookDataset->saveAllDataToFile();
					MessageBox(_T(CString("已归还图书：\n")+s2), _T(""), MB_OK | MB_ICONINFORMATION);
					ReturnBookId.SetWindowTextA("");
					ReturnBookName.SetWindowTextA("");
					ReturnBookId.SetFocus();
					return;

				}
				else {
					MessageBox(_T(CString("您没有借出此本书")), _T(""), MB_OK | MB_ICONINFORMATION);
					ReturnBookName.SetFocus();
					ReturnBookName.SetSel(0, -1);
					return;
				}
			}
		}
	}

}



void CReturnDlg::OnBnClickedButton1()
{
	OnOK();
	OnClose();
}


BOOL CReturnDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (CMFCTest03Dlg::MayBeReturnBookId != 0) {
		CString tmp; tmp.Format("%d", CMFCTest03Dlg::MayBeReturnBookId);
		ReturnBookId.SetWindowTextA(tmp);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CReturnDlg::OnBnClickedRadio1()
{
	flag = 1;
	ReturnBookId.EnableWindow(TRUE);
	ReturnBookName.EnableWindow(FALSE);
	ReturnBookId.SetFocus();
	ReturnBookId.SetSel(0, -1);
}


void CReturnDlg::OnBnClickedRadio2()
{
	flag = 2;
	ReturnBookId.EnableWindow(FALSE);
	ReturnBookName.EnableWindow(TRUE);
	ReturnBookName.SetFocus();
	ReturnBookName.SetSel(0, -1);
}
BOOL CReturnDlg::PreTranslateMessage(MSG* pMsg)
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

