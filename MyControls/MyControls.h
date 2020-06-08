#ifndef __MYCONTROLS__
#define __MYCONTROLS__
#include "afxcmn.h"

//-----------------------------------------------
/***********************************************
    示例：通过继承，让 CListCtrl 控件拥有自己的绘制风格（自定义背景 未实现）
    Start CMyCListCtrl define
************************************************/
//-----------------------------------------------
class CMyHeaderCtrl : public CHeaderCtrl{
public:
	CMyHeaderCtrl();

	virtual ~CMyHeaderCtrl();
private:
	COLORREF doColor(int nIndex);

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnEraseBkgnd(CDC *pDC);

	afx_msg void OnPaint();

	CStringArray m_HChar;

	CString m_Format; //表示对齐类型的整型数组,0表示左对齐，1表示中间对齐，2表示右对齐

public:
	int m_R;
	int m_G;
	int m_B;

	float m_Gradient;  // 画立体背景，渐变系数

	float m_fMagFactor;  //表头高度(倍数),

	int m_fontHeight; //字体高度

	int m_fontWith;   //字体宽度

	COLORREF m_color;

	LRESULT OnLayout( WPARAM wParam, LPARAM lParam );
};

//-----------------------------------------------
/***********************************************
    End CMyHeaderCtrl define

    Start CMyCListCtrl define
************************************************/
//-----------------------------------------------
class CMyCListCtrl : public CListCtrl{
public:
	CMyHeaderCtrl m_Header;

    int m_fontHeight;
	int m_fontWith;

	float m_fRowHFactor;//行高（倍数）

	COLORREF m_color;
	COLORREF m_tmpColor;

	//基类 CListCtrl 中有个 InsertColumn 成员函数，很遗憾不是虚函数
	//故子类 只能通过 覆盖方式重写 （即使 左修饰 virtual 也没用）
	//这意味着 InsertColumn 无法动态绑定
	//具体使用方式 课堂讲解
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading,int nFormat = LVCFMT_LEFT, 
		int nWidth = -1, int nSubItem = -1);

	//Gradient - 渐变系数，立体背景用,不用渐变设为0
	//一般专业软件，可让美工做背景图，直接贴图
	void SetHeaderBKColor(int R, int G, int B, int Gradient);

	// 设置表头高度倍率
	void SetHeaderHeightFactor(float m_fMagFactor);

	 //设置行高倍率
	void SetRowHeigtFactor(float m_fFactor);

	CPtrList m_ptrListCol;  //保存列颜色

	CPtrList m_ptrListItem; //保存Item颜色表

	CPtrList m_colTextColor; //保存列字体颜色

	CPtrList m_ItemTextColor; //保存单元格字体颜色

	bool FindColColor(int col ,COLORREF &color); //查找列颜色

	bool FindItemColor(int col,int row,COLORREF &color);

	bool FindColTextColor(int col,COLORREF &color); //查找列字体颜色

	bool FindItemTextColor(int col,int row,COLORREF &color);//查找单元格字体颜色

	void SetColColor(int col,COLORREF color);  //设置列颜色

	void SetItemColor(int col,int row,COLORREF color); //设置Item颜色

	void SetColTextColor(int col,COLORREF color);   //设置列文本颜色

	void SetItemTextColor(int col,int row,COLORREF color);

	void SetHeaderFontHW(int nHeight,int nWith); //设置表头字体大小

	void SetHeaderTextColor(COLORREF color);	

	BOOL SetTextColor(COLORREF cr);

	void SetFontHW(int nHeight,int nWith);  //设置字体的高和宽

public:
	CMyCListCtrl();
	
	virtual ~CMyCListCtrl();

public:
	virtual BOOL OnEraseBkgnd(CDC *pDC);

	virtual void OnPaint();

protected:
	DECLARE_MESSAGE_MAP()

	virtual void PreSubclassWindow();
public:
	 afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

     void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

public:
	int m_nSelColumn;

	void DrawBKGround(CDC* pDC, CRect Rt, LPDRAWITEMSTRUCT lpDIS, int nColIndex, BOOL bColumnSel);
	//void DrawImage(CDC* pDC,CPoint Pt,int nItem);

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
};
//-----------------------------------------------
/***********************************************
    End CMyCListCtrl define
************************************************/
//-----------------------------------------------

#endif