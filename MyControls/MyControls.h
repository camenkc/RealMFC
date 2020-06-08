#ifndef __MYCONTROLS__
#define __MYCONTROLS__
#include "afxcmn.h"

//-----------------------------------------------
/***********************************************
    ʾ����ͨ���̳У��� CListCtrl �ؼ�ӵ���Լ��Ļ��Ʒ���Զ��屳�� δʵ�֣�
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

	CString m_Format; //��ʾ�������͵���������,0��ʾ����룬1��ʾ�м���룬2��ʾ�Ҷ���

public:
	int m_R;
	int m_G;
	int m_B;

	float m_Gradient;  // �����屳��������ϵ��

	float m_fMagFactor;  //��ͷ�߶�(����),

	int m_fontHeight; //����߶�

	int m_fontWith;   //������

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

	float m_fRowHFactor;//�иߣ�������

	COLORREF m_color;
	COLORREF m_tmpColor;

	//���� CListCtrl ���и� InsertColumn ��Ա���������ź������麯��
	//������ ֻ��ͨ�� ���Ƿ�ʽ��д ����ʹ ������ virtual Ҳû�ã�
	//����ζ�� InsertColumn �޷���̬��
	//����ʹ�÷�ʽ ���ý���
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading,int nFormat = LVCFMT_LEFT, 
		int nWidth = -1, int nSubItem = -1);

	//Gradient - ����ϵ�������屳����,���ý�����Ϊ0
	//һ��רҵ�������������������ͼ��ֱ����ͼ
	void SetHeaderBKColor(int R, int G, int B, int Gradient);

	// ���ñ�ͷ�߶ȱ���
	void SetHeaderHeightFactor(float m_fMagFactor);

	 //�����и߱���
	void SetRowHeigtFactor(float m_fFactor);

	CPtrList m_ptrListCol;  //��������ɫ

	CPtrList m_ptrListItem; //����Item��ɫ��

	CPtrList m_colTextColor; //������������ɫ

	CPtrList m_ItemTextColor; //���浥Ԫ��������ɫ

	bool FindColColor(int col ,COLORREF &color); //��������ɫ

	bool FindItemColor(int col,int row,COLORREF &color);

	bool FindColTextColor(int col,COLORREF &color); //������������ɫ

	bool FindItemTextColor(int col,int row,COLORREF &color);//���ҵ�Ԫ��������ɫ

	void SetColColor(int col,COLORREF color);  //��������ɫ

	void SetItemColor(int col,int row,COLORREF color); //����Item��ɫ

	void SetColTextColor(int col,COLORREF color);   //�������ı���ɫ

	void SetItemTextColor(int col,int row,COLORREF color);

	void SetHeaderFontHW(int nHeight,int nWith); //���ñ�ͷ�����С

	void SetHeaderTextColor(COLORREF color);	

	BOOL SetTextColor(COLORREF cr);

	void SetFontHW(int nHeight,int nWith);  //��������ĸߺͿ�

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