// CDlgBookView.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CDlgBookView.h"
#include "afxdialogex.h"


// CDlgBookView 对话框

IMPLEMENT_DYNAMIC(CDlgBookView, CDialogEx)

CDlgBookView::CDlgBookView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGBOOKVIEW, pParent)
{
	m_pBookDataset = NULL;
	
	char* aName[7] = { "Id","书名","书号","作者","出版社","单价","库存" };

	for (int i = 0; i < 7; i++) {
		aField.push_back(aName[i]);
	}
	
}

CDlgBookView::~CDlgBookView()
{
	if (m_pBookDataset) delete m_pBookDataset;
}

void CDlgBookView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, list);
	DDX_Control(pDX, IDC_COMBO1, cmbForSort);
	DDX_Control(pDX, IDC_COMBO2, cmbForSel);
	DDX_Control(pDX, IDC_COMBO3, cmbForCmp);
	DDX_Control(pDX, IDC_COMBO4, cmbForSelCmp);
	DDX_Control(pDX, IDC_EDIT1, edText);
}


BEGIN_MESSAGE_MAP(CDlgBookView, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON1, &CDlgBookView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgBookView::OnBnClickedButton2)
//	ON_CBN_SELENDCANCEL(IDC_COMBO1, &CDlgBookView::OnCbnSelendcancelCombo1)
ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgBookView::OnCbnSelchangeCombo1)
ON_CBN_SELCHANGE(IDC_COMBO3, &CDlgBookView::OnCbnSelchangeCombo3)
ON_CBN_SELCHANGE(IDC_COMBO2, &CDlgBookView::OnCbnSelchangeCombo2)
ON_CBN_SELCHANGE(IDC_COMBO4, &CDlgBookView::OnCbnSelchangeCombo4)
ON_EN_CHANGE(IDC_EDIT1, &CDlgBookView::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CDlgBookView 消息处理程序


BOOL CDlgBookView::OnInitDialog()//初始化控件
{

	CDialogEx::OnInitDialog();
	//初始化：
	//头行高为系统的1.2倍
	list.SetHeaderHeightFactor(1.2);
	//内容行高为1.3倍
	list.SetRowHeigtFactor(1.3);
	//第二列 文本为蓝色
	list.SetColTextColor(1, RGB(0, 0, 255));

	m_pBookDataset = new CBookDataset;

	try {
		//读取全部图书数据
		m_pBookDataset->readAllData();

		m_pBookDataset->dataOrder(CBookData::compByIdAsc);

		//把数据显示到列表控件里
		m_pBookDataset->dataToListCtrl(&list);//////////////////////////

		m_pBookDataset->SetBookNoLeftToRed(&list);
		for (int i = 0; i<aField.size(); i++) {
			cmbForSel.AddString(aField[i]);
			cmbForSort.AddString(aField[i]);
		}

		if (aField.size() > 0)
		{
			cmbForSel.SetCurSel(0);
			cmbForSort.SetCurSel(0);
			cmbForSelCmp.SetCurSel(0);
		}


	}
	catch (CString& eStr) {
		AfxMessageBox(eStr);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
}


BOOL CDlgBookView::PreTranslateMessage(MSG* pMsg)//预处理窗口消息 （放置按了enter和escape后就退出
{
	
	if (pMsg->message == WM_KEYDOWN) {
		//当案件为enter和escape的时候不自动退出
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
			return TRUE;	//返回1表示消息到此为止
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


//应用按钮
void CDlgBookView::OnBnClickedButton1()
{
	try {
		if (m_pBookDataset == NULL) {
			throw CString("m_pBookDataset is NULL");
		}

		CString sVal, sField, sCmp;
		edText.GetWindowTextA(sVal);
		cmbForSel.GetWindowTextA(sField);
		cmbForSelCmp.GetWindowTextA(sCmp);

		//去空格
		sVal = sVal.Trim();

		if (sVal.IsEmpty()) {
			throw CString("请输入 [" + sField + "] 的值");
		}

		ESelect eSelect;
		if (sCmp == "=") {
			eSelect = eEqual;
		}
		else if (sCmp == ">") {
			eSelect = eGreater;
		}
		else if (sCmp == ">=") {
			eSelect = eGreaterOrEqual;
		}
		else if (sCmp == "<") {
			eSelect = eLess;
		}
		else if (sCmp == "<=") {
			eSelect = eLessOrEqual;
		}
		else if (sCmp == "Like") {
			eSelect = eLike;
		}
		else {
			throw CString("无效的比较方式：" + sCmp);
		}

		//新的图书数据集 放在 aDataset
		vector<CBookData> aDataset;

		m_pBookDataset->selectData(aDataset, sField, eSelect, sVal);

		m_pBookDataset->dataToListCtrl(&list, &aDataset);

		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	}
	catch (CString& eStr) {
		AfxMessageBox(eStr);
	}
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
}


void CDlgBookView::OnBnClickedButton2()
{
	try {
		if (m_pBookDataset == NULL) {
			throw CString("m_pBookDataset is NULL");
		}

		CString sFieldForSort, sSortType;
		cmbForSort.GetWindowTextA(sFieldForSort);
		cmbForCmp.GetWindowTextA(sSortType);

		if (sFieldForSort == "Id" && sSortType == "升序") {
			m_pBookDataset->dataOrder(CBookData::compByIdAsc);
		}
		else if (sFieldForSort == "Id" && sSortType == "降序") {
			m_pBookDataset->dataOrder(CBookData::compByIdDec);
		}
		else if (sFieldForSort == "书名" && sSortType == "升序") {
			m_pBookDataset->dataOrder(CBookData::compByNameAsc);
		}
		else if (sFieldForSort == "书名" && sSortType == "降序") {
			m_pBookDataset->dataOrder(CBookData::compByNameDec);
		}
		else if (sFieldForSort == "作者" && sSortType == "升序") {
			m_pBookDataset->dataOrder(CBookData::compByAuthorAsc);
		}
		else if (sFieldForSort == "作者" && sSortType == "降序") {
			m_pBookDataset->dataOrder(CBookData::compByAuthorDec);
		}
		else if (sFieldForSort == "出版社" && sSortType == "升序") {
			m_pBookDataset->dataOrder(CBookData::compByPubHouseAsc);
		}
		else if (sFieldForSort == "出版社" && sSortType == "降序") {
			m_pBookDataset->dataOrder(CBookData::compByPubHouseDec);
		}
		else if (sFieldForSort == "单价" && sSortType == "升序") {
			m_pBookDataset->dataOrder(CBookData::compByPriceAsc);
		}
		else if (sFieldForSort == "单价" && sSortType == "降序") {
			m_pBookDataset->dataOrder(CBookData::compByPriceDec);
		}
		else if (sFieldForSort == "库存" && sSortType == "升序") {
			m_pBookDataset->dataOrder(CBookData::compByLeftAsc);
		}
		else if (sFieldForSort == "库存" && sSortType == "降序") {
			m_pBookDataset->dataOrder(CBookData::compByLeftDec);
		}



		m_pBookDataset->dataToListCtrl(&list);

		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	}
	catch (CString& eStr) {
		AfxMessageBox(eStr);
	}

	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
}


//void CDlgBookView::OnCbnSelendcancelCombo1()
//{
//	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
//}


void CDlgBookView::OnCbnSelchangeCombo1()
{
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
}


void CDlgBookView::OnCbnSelchangeCombo3()
{
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
}


void CDlgBookView::OnCbnSelchangeCombo2()
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
}


void CDlgBookView::OnCbnSelchangeCombo4()
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
}


void CDlgBookView::OnEnChangeEdit1()
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

