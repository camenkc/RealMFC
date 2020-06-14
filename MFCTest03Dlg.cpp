
// MFCTest03Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCTest03.h"
#include "MFCTest03Dlg.h"
#include "CBorrowDlg.h"
#include "CReturnDlg.h"
#include "CReaderChangeDlg.h"
#include "CReaderLoginDlg.h"
#include "CReaderRegisterDlg.h"
#include "afxdialogex.h"
#include"CBookInDlg.h"
#include "CReaderViewDlg.h"
#include "CBorrowViewDlg.h"
#include "CHistoryViewDlg.h"
#include"CDlgBookView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCTest03Dlg 对话框



CMFCTest03Dlg::CMFCTest03Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGIndex, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICONMuQ);
}

void CMFCTest03Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, NowReaderCode);
}

BEGIN_MESSAGE_MAP(CMFCTest03Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_32773, &CMFCTest03Dlg::OnMenuExit)
	ON_COMMAND(ID_32771, &CMFCTest03Dlg::OnBookInput)
	ON_COMMAND(ID_32772, &CMFCTest03Dlg::OnViewBook)
	ON_COMMAND(ID_32779, &CMFCTest03Dlg::OnBorrowBook)
	ON_COMMAND(ID_32780, &CMFCTest03Dlg::OnReturnBook)
	ON_COMMAND(ID_32781, &CMFCTest03Dlg::OnReaderChange)
	ON_COMMAND(ID_32777, &CMFCTest03Dlg::OnReaderLogin)
	ON_COMMAND(ID_32778, &CMFCTest03Dlg::OnRegister)
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_32776, &CMFCTest03Dlg::OnLogoutClicked)
	ON_COMMAND(ID_32782, &CMFCTest03Dlg::OnReaderView)
	ON_COMMAND(ID_Menu, &CMFCTest03Dlg::OnBorrowView)
	ON_COMMAND(ID_32784, &CMFCTest03Dlg::OnHistoryVIewDlg)
END_MESSAGE_MAP()


// CMFCTest03Dlg 消息处理程序

BOOL CMFCTest03Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCTest03Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCTest03Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCTest03Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//从菜单栏推出系统
void CMFCTest03Dlg::OnMenuExit()
{
	CString s;
	s = "退出系统确认";

	if (::MessageBox(NULL, s, "消息确认...", MB_YESNO | MB_ICONQUESTION) == IDYES) {
		CDialogEx::OnOK();
	}
}

//录入图书系统
void CMFCTest03Dlg::OnBookInput()
{
	//AfxMessageBox("录入图书信息");
	if (NowLoginReader == 1)
	{
		CBookInDlg dlg;
		dlg.DoModal();//模态对话框
	}
	else
	{
		::MessageBox(NULL, CString("您不为管理员！"), CString(""),MB_OK | MB_ICONQUESTION);
	}
}

//
void CMFCTest03Dlg::OnViewBook()
{
	CDlgBookView dlg;
	dlg.DoModal();
}

//按下借阅图书的按钮
void CMFCTest03Dlg::OnBorrowBook()
{
	CBorrowDlg dlg;
	dlg.DoModal();
}

//按下归还图书的按钮
void CMFCTest03Dlg::OnReturnBook()
{
	CReturnDlg dlg;
	dlg.DoModal();
}

//按下修改读者信息按钮
void CMFCTest03Dlg::OnReaderChange()
{
	CReaderChangeDlg dlg;
	dlg.DoModal();
}

//按下登录按钮
void CMFCTest03Dlg::OnReaderLogin()
{
	CReaderLoginDlg dlg;
	dlg.DoModal();
	CString tmp;
	tmp.Format("%d", CMFCTest03Dlg::NowLoginReader);
	NowReaderCode.SetWindowTextA(tmp);
	if (NowLoginReader != 0) 
	{
		//当前有人登录了 那么把菜单中的账号登录disable 把退出enable
		CMenu* menu=GetMenu();//当前对话框的menu就是要修改的menu
		CMenu* subMenu = menu->GetSubMenu(1);//第2个Menu栏(用户信息)
		CMenu*  subsubMenu= subMenu->GetSubMenu(0);//的第一个Menu栏
		//第一个参数0 表示第一个按钮 后面表示变灰
		//网上有很多其他说法 但我觉得是必须Disable 和 变灰 同时进行才能实现Disable
		//参见：
		///https://blog.csdn.net/Jingle912/article/details/72357145?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.nonecase
		subsubMenu->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED); 
		//同时退出账号的按钮和图书借阅Enable了
		subMenu->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
		subMenu->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		menu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
		
		DrawMenuBar();
	}
}

//按下注册按钮
void CMFCTest03Dlg::OnRegister()
{
	if (NowLoginReader != 0)
	{
		if (::MessageBox(NULL, CString("注册新账号需退出当前账号，是否退出"), "消息确认...", MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			NowLoginReader = 0;
			NowReaderCode.SetWindowTextA("0");
			CMenu* subMenu = GetMenu()->GetSubMenu(1);
			subMenu->EnableMenuItem(1, MF_BYPOSITION | MF_GRAYED);
			CMenu* subsubMenu = subMenu->GetSubMenu(0);
			subsubMenu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
			subMenu->EnableMenuItem(3, MF_BYPOSITION | MF_GRAYED);
			MessageBox("账号已成功退出","", MB_OK | MB_ICONINFORMATION);
		}
	}
	CReaderRegisterDlg dlg;
	dlg.DoModal();
	CString tmp;
	tmp.Format("%d", CMFCTest03Dlg::NowLoginReader);
	NowReaderCode.SetWindowTextA(tmp);
	if (NowLoginReader != 0)
	{
		CMenu* menu = GetMenu();//当前对话框的menu就是要修改的menu
		CMenu* subMenu = menu->GetSubMenu(1);//第2个Menu栏(用户信息)
		CMenu* subsubMenu = subMenu->GetSubMenu(0);//的第一个Menu栏
		subsubMenu->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED);
		subMenu->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
		subMenu->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		menu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);

		DrawMenuBar();
	}
}


//按下退出账户按钮
void CMFCTest03Dlg::OnLogoutClicked()
{
	CString s;
	s = "退出账户确认";

	if (::MessageBox(NULL, s, "消息确认...", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{



		NowLoginReader = 0;
		NowReaderCode.SetWindowTextA("0");
		CMenu* subMenu = GetMenu()->GetSubMenu(1);
		subMenu->EnableMenuItem(1, MF_BYPOSITION | MF_GRAYED);
		CMenu* subsubMenu = subMenu->GetSubMenu(0);
		subsubMenu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
		subMenu->EnableMenuItem(3, MF_BYPOSITION | MF_GRAYED);
		MessageBox("账号已成功退出", "", MB_OK | MB_ICONINFORMATION);
	}
}

//用来浏览整体的读者信息
void CMFCTest03Dlg::OnReaderView()
{
	CReaderViewDlg dlg;
	dlg.DoModal();
}

//用来浏览整体在借记录
void CMFCTest03Dlg::OnBorrowView()
{
	CBorrowViewDlg dlg;
	dlg.DoModal();
}

//用来浏览历史记录
void CMFCTest03Dlg::OnHistoryVIewDlg()
{
	CHistoryViewDlg dlg;
	dlg.DoModal();
}
