// CBookInDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CBookInDlg.h"
#include "afxdialogex.h"

// CBookInDlg 对话框

IMPLEMENT_DYNAMIC(CBookInDlg, CDialogEx)

CBookInDlg::CBookInDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BOOKINPUT, pParent)
{
	m_pBookDataset = NULL;
}

CBookInDlg::~CBookInDlg()
{
	if (m_pBookDataset)delete m_pBookDataset;
}

void CBookInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, listBook);
}


BEGIN_MESSAGE_MAP(CBookInDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CBookInDlg::OnLvnItemchangedList2)
	ON_BN_CLICKED(IDC_BUTTON3, &CBookInDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CBookInDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CBookInDlg::OnBnClickedButton2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CBookInDlg::OnNMDblclkList2)
	ON_BN_CLICKED(IDC_BUTTON4, &CBookInDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CBookInDlg 消息处理程序


void CBookInDlg::OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

//返回主界面
void CBookInDlg::OnBnClickedButton3()
{
	OnClose();
	OnCancel();
	OnDestroy();
	PostNcDestroy();//四行 功能与按右上角叉键的退出一致
}

//清楚EditText编辑框 文字
void CBookInDlg::ClearEditText() {
	char* aName[6] = { "Id","书名","书号","作者","出版社","单价" };
	UINT aId[6] = { IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4, IDC_EDIT5, IDC_EDIT6 };

	for (int i = 0; i < 6; i++) {
		if (GetDlgItem(aId[i]) == NULL) {
			AfxMessageBox(aName[i] + (CString)" 对象的文本框不存在");
			return;
		}
		GetDlgItem(aId[i])->SetWindowTextA("");
	}

	try
	{
		if (m_pBookDataset) {
			int MaxId = m_pBookDataset->getMaxVal("Id") + 1;
			CString s;
			s.Format("%d", MaxId);
			GetDlgItem(aId[0])->SetWindowTextA(s);
		}
		GetDlgItem(aId[1])->SetFocus();
	}
	catch (CString &e)
	{
		AfxMessageBox(e);
	}
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
}

//保存数据
void CBookInDlg::OnBnClickedButton1()
{
	if (m_pBookDataset == NULL) return;
	try
	{
		char* aName[6] = { "Id","书名","书号","作者","出版社","单价" };
		UINT aId[6] = {IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4, IDC_EDIT5, IDC_EDIT6};
		//应该验证输入合法性
		vector <CString> aStr;
		CString s;
		for (int i = 0; i < 6; i++) {
			if (GetDlgItem(aId[i]) == NULL) { throw CString(aName[i] + (CString)" 对象文本不存在"); }
			
			GetDlgItem(aId[i])->GetWindowTextA(s);

			//删除前后空格
			s = s.Trim();
			if (s.IsEmpty()) {
				throw CString("请输入正确的" + (CString)aName[i]);
			}

			if (i == 5) {
				//正则表达式 使得此时必须为数字
			}
			aStr.push_back(s);
		}

		if (aStr.size() != 6) {
			throw CString("aStr.size() != 6");
		}
		CBookData bookData(
			atoi(aStr[0]),
			aStr[1].GetBuffer(),
			aStr[2].GetBuffer(),
			aStr[3].GetBuffer(),
			aStr[4].GetBuffer(),
			atof(aStr[5])
			);
	

		//判断是否是已有的记录 
		CBookData* pOldData = m_pBookDataset->getItemByKeyVal("Id", bookData.nBookId);
		if (pOldData!=NULL) {
			//非空 记录存在
			*pOldData = bookData;//无指针无需深拷贝
			m_pBookDataset->saveAllDataToFile();
		}
		else {//记录不存在 直接追加
			m_pBookDataset->saveOneItemToFile(&bookData);
		}
		OnBnClickedButton2();
	}
	catch (CString &e)
	{
		AfxMessageBox(e);
	}
}

//刷新数据
void CBookInDlg::OnBnClickedButton2()
{
	ClearEditText();
	if (m_pBookDataset == NULL) return; 

	try
	{
		//m_pBookDataset->readAllData();
		m_pBookDataset->dataOrder(CBookData::compByIdAsc);
		m_pBookDataset->dataToListCtrl(&listBook);
	}
	catch (CString& eStr) {
		AfxMessageBox(eStr);
	}
}


BOOL CBookInDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//初始化：
	//头行高为系统的1.2倍
	listBook.SetHeaderHeightFactor(1.2);
	//内容行高为1.3倍
	listBook.SetRowHeigtFactor(1.3);
	//第二列 文本为蓝色
	listBook.SetColTextColor(1, RGB(0, 0, 255));
	//第2列 第3行 
	listBook.SetItemTextColor(1,2, RGB(0, 255, 0));

	m_pBookDataset = new CBookDataset;

	try
	{
		m_pBookDataset->readAllData();
		m_pBookDataset->dataOrder(CBookData::compByIdDec);
		m_pBookDataset->dataToListCtrl(&listBook);

		ClearEditText();//主要是用来设定书号
	}
	catch (CString &eStr) {
		AfxMessageBox (eStr);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//预处理消息
BOOL CBookInDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		//当案件为enter和escape的时候不自动退出
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
			return TRUE;	//返回1表示消息到此为止
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//列表控件被双击了
void CBookInDlg::OnNMDblclkList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
	UINT aId[6] = { IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4, IDC_EDIT5, IDC_EDIT6 };

	//获取被双击的行
	int nRowSel = listBook.GetNextItem(-1, LVNI_SELECTED);
	nMayNeedToBeDelete = nRowSel;
	if (nRowSel < 0) {
		return;
	}

	CWnd* pWnd;
	for (int i = 0; i < 6; i++) {
		pWnd = GetDlgItem(aId[i]);
		if (pWnd) {
			pWnd->SetWindowTextA(listBook.GetItemText(nRowSel, i));
		}

	}

	GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
}

//当删除节点按下后 
void CBookInDlg::OnBnClickedButton4()
{
	CString s = "是否删除本条记录？";
	if (::MessageBoxA(NULL, s.GetBuffer(), "确认删除...", MB_YESNO | MB_ICONQUESTION) == IDYES) {
		m_pBookDataset->deleteOneRow(nMayNeedToBeDelete);
		ClearEditText();
		OnBnClickedButton2();
		m_pBookDataset->saveAllDataToFile();
	}
}
