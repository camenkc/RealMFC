// CBookReturn.cpp: 实现文件
//

#include "pch.h"
#include "MFCTest03.h"
#include "CBookReturn.h"
#include "afxdialogex.h"


// CBookReturn 对话框

IMPLEMENT_DYNAMIC(CBookReturn, CDialogEx)

CBookReturn::CBookReturn(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGBookReturn, pParent)
{

}

CBookReturn::~CBookReturn()
{
}

void CBookReturn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBookReturn, CDialogEx)
END_MESSAGE_MAP()


// CBookReturn 消息处理程序
