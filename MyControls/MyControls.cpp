#include "pch.h"
#include "MyControls.h"
//-----------------------------------------------
/***********************************************
Start CMyHeaderCtrl define
************************************************/
//-----------------------------------------------
CMyHeaderCtrl::CMyHeaderCtrl(){
	m_Format = "";

	m_R = m_G = m_B = 247;

	m_Gradient   =  1.0;
	m_fMagFactor =  1;
	m_fontHeight =  15;
	m_fontWith   =  0;
	m_color      =  RGB(0,0,0);
}

CMyHeaderCtrl::~CMyHeaderCtrl(){

}

BEGIN_MESSAGE_MAP(CMyHeaderCtrl, CHeaderCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
END_MESSAGE_MAP()

// CMyHeaderCtrl ��Ϣ�������
BOOL CMyHeaderCtrl::OnEraseBkgnd(CDC *pDC){
	//�������������Ĭ�ϴ���
	//return CHeaderCtrl::OnEraseBkgnd(pDC);

	return FALSE;
}

COLORREF CMyHeaderCtrl::doColor(int nIndex){
	int R = m_R - nIndex * m_Gradient; 
	int G = m_G - nIndex * m_Gradient;
	int B = m_B - nIndex * m_Gradient;

	R = R < 0 ? 0 : R;
	G = G < 0 ? 0 : G;
	B = B < 0 ? 0 : B;

	return RGB(R, G, B);
}

void CMyHeaderCtrl::OnPaint(){
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	CRect headerRect;
	CDC MemDC;
	CBitmap MemMap;

	GetClientRect(&rect);
	GetItemRect(0, &headerRect);
	//GetDlgItem(0)->GetWindowRect(&headerRect);
	rect.bottom = headerRect.Height();

	MemDC.CreateCompatibleDC(&dc);
	MemMap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	MemDC.SelectObject(&MemMap);

	// ��Ϊ��ͼ��Ϣ���� CHeaderCtrl::OnPaint()
	int nItem;

	nItem = GetItemCount();//�õ��м�����Ԫ

	////////
	CFont newFont;
	newFont.CreatePointFont(110 ,"΢���ź�");
	MemDC.SelectObject(&newFont);

	int nIndex = 0;

	////////
	for(int i = 0; i < nItem; i++){
		if(m_HChar.GetSize() <= i){
			break;
		}

		CRect tRect;

		GetItemRect(i, &tRect);//�õ�Item�ĳߴ�

		CRect nRect(tRect);//�����ߴ絽�µ�������

		if(i > 0){//�����ָ��ߵĵط�
			nRect.left++;
		}else if(i == nItem - 1){
			nRect.right++;
		}

		
		//�������屳��
		nIndex = 0;
		for(int j = tRect.top; j<= tRect.bottom; j++){
			nRect.bottom = nRect.top + 1;

			CBrush _brush;

			_brush.CreateSolidBrush(doColor(nIndex++));//������ˢ

			MemDC.FillRect(&nRect,&_brush); //��䱳��

			_brush.DeleteObject(); //�ͷŻ�ˢ

			nRect.top = nRect.bottom;
		}

		MemDC.SetBkMode(TRANSPARENT);

		MemDC.SetTextColor(m_color);

		UINT nFormat = 1;

		if (m_Format[i]=='0'){
			nFormat = DT_LEFT;

			tRect.left+=3;
		}else if (m_Format[i]=='1'){
			nFormat = DT_CENTER;
		}else if (m_Format[i]=='2'){
			nFormat = DT_RIGHT;

			tRect.right-=3;
		}
						
		MemDC.DrawTextA(m_HChar[i], -1, &tRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	}

	//��ͷ��ʣ�ಿ��
	CRect rtRect;

	CRect clientRect;

	GetItemRect(nItem - 1,rtRect);

	GetClientRect(clientRect);

	rtRect.left = rtRect.right+1;

	rtRect.right = clientRect.right;

	CRect nRect(rtRect);

	nIndex = 0;
	//�������屳��
	for(int j = rtRect.top;j<=rtRect.bottom;j++){
		nRect.bottom = nRect.top+1;

		CBrush _brush;

		_brush.CreateSolidBrush(doColor(nIndex++));//������ˢ

		MemDC.FillRect(&nRect,&_brush); //��䱳��

		_brush.DeleteObject(); //�ͷŻ�ˢ

		nRect.top = nRect.bottom;
	}

	newFont.DeleteObject();

	//���
	dc.BitBlt(0,0,rect.Width(),rect.Height(),&MemDC,0,0,SRCCOPY);
	
	MemDC.DeleteDC();

	MemMap.DeleteObject();
}

LRESULT CMyHeaderCtrl::OnLayout( WPARAM wParam, LPARAM lParam ){
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);

	HD_LAYOUT &hdl = *( HD_LAYOUT * ) lParam;

	RECT *prc = hdl.prc;

	WINDOWPOS *pwpos = hdl.pwpos;

	int nHeight = (int)(pwpos->cy * m_fMagFactor);

	pwpos->cy = nHeight;

	prc->top = nHeight;

	return lResult;
}
//-----------------------------------------------
/***********************************************
End CMyHeaderCtrl define

Start CMyCListCtrl define
************************************************/
//-----------------------------------------------
struct stColor{
	int nRow;
	int nCol;
	COLORREF rgb;
};

CMyCListCtrl::CMyCListCtrl(){ 
	m_fRowHFactor =  1.0;
	m_fontHeight  =  12;
	m_fontWith    =  0;
	m_nSelColumn  =  -1;
	m_color       =  RGB(0,0,0);
}

void freePtr(CPtrList &ListPtr){
	POSITION pos, temp;
    
	for(pos = ListPtr.GetHeadPosition(); pos;){
        temp = pos;
        stColor *node = (stColor*) ListPtr.GetNext(pos);
     
		if(node){
            delete node; 
            ListPtr.RemoveAt(temp);
        }
    }
}

CMyCListCtrl::~CMyCListCtrl(){
	freePtr(m_ptrListCol);
	freePtr(m_ptrListItem);
	freePtr(m_colTextColor);
	freePtr(m_ItemTextColor);
}

BEGIN_MESSAGE_MAP(CMyCListCtrl, CListCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
	///////////////////
	ON_WM_MEASUREITEM()
	ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()

void CMyCListCtrl::PreSubclassWindow(){
	ModifyStyle(0,LVS_OWNERDRAWFIXED);

	CListCtrl::PreSubclassWindow();

	CHeaderCtrl *pHeader = GetHeaderCtrl();

	m_Header.SubclassWindow(pHeader->GetSafeHwnd());
}



////////////////////////
void CMyCListCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct){
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CMyCListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct){
	if (m_fRowHFactor > 1.0){
		lpMeasureItemStruct->itemHeight *= m_fRowHFactor;
	}
}

int CMyCListCtrl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem){
	m_Header.m_HChar.Add(lpszColumnHeading);

	if (nFormat==LVCFMT_LEFT){
		m_Header.m_Format = m_Header.m_Format + "0";
	}else if (nFormat==LVCFMT_CENTER){
		m_Header.m_Format = m_Header.m_Format + "1";
	}else if (nFormat==LVCFMT_RIGHT){
		m_Header.m_Format = m_Header.m_Format + "2";
	}else{
		m_Header.m_Format = m_Header.m_Format + "1";
	}

	return CListCtrl::InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,nSubItem);
}

//���ñ�ͷ����ɫ  Gradient - ����ϵ�������屳����,���ý�����Ϊ0
void CMyCListCtrl::SetHeaderBKColor(int R, int G, int B, int Gradient){ 
	m_Header.m_R = R;

	m_Header.m_G = G;

	m_Header.m_B = B;

	m_Header.m_Gradient = Gradient;
}

//���ñ�ͷ�߶�(����)
void CMyCListCtrl::SetHeaderHeightFactor(float m_fMagFactor){
	m_Header.m_fMagFactor = (m_fMagFactor > 1.0 ? m_fMagFactor : 1.0);
}


//�����и߱���
void CMyCListCtrl::SetRowHeigtFactor(float m_fFactor){
	m_fRowHFactor = (m_fFactor > 1.0 ? m_fFactor : 1.0);

	CRect rcWin;

	GetWindowRect(&rcWin);

	WINDOWPOS wp;

	wp.hwnd = m_hWnd;

	wp.cx = rcWin.Width();

	wp.cy = rcWin.Height();

	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;

	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}

//��������ɫ
bool CMyCListCtrl::FindColColor(int col,COLORREF &color){
	for (POSITION pos = m_ptrListCol.GetHeadPosition();pos != NULL;){
		stColor *pColor = (stColor*)m_ptrListCol.GetNext(pos);

		if(pColor->nCol == col){
			color = pColor->rgb;
			return true;
		}
	}

	return false;
}

//������ɫ
bool CMyCListCtrl::FindItemColor(int col,int row,COLORREF &color){
	for (POSITION pos = m_ptrListItem.GetHeadPosition();pos!=NULL;){
		stColor *pColor = (stColor*)m_ptrListItem.GetNext(pos);

		if (pColor->nCol == col && pColor->nRow == row){
			color = pColor->rgb;
			return true;
		}
	}

	return false;
}

//��������ɫ
void CMyCListCtrl::SetColColor(int col, COLORREF color){
	stColor *pColor  = new stColor;

	pColor->nCol = col;

	pColor->rgb = color;

	m_ptrListCol.AddTail(pColor);
}

//���ø�����ɫ
void CMyCListCtrl::SetItemColor(int col,int row,COLORREF color) {
	stColor *pColor  = new stColor;

	pColor->nCol = col;

	pColor->nRow = row;

	pColor->rgb = color;

	m_ptrListItem.AddTail(pColor);
}

//����ͷ�������͸�
void CMyCListCtrl::SetHeaderFontHW(int nHeight,int nWith){
	m_Header.m_fontHeight = nHeight;

	m_Header.m_fontWith = nWith;
}

void CMyCListCtrl::SetHeaderTextColor(COLORREF color){ //����ͷ��������ɫ
	m_Header.m_color = color;
}

BOOL CMyCListCtrl::SetTextColor(COLORREF cr){ //����������ɫ
	m_color = cr;

	return TRUE;
}

void CMyCListCtrl::SetFontHW(int nHeight,int nWith){ //��������ߺͿ�
	m_fontHeight = nHeight;

	m_fontWith = nWith;
}

void CMyCListCtrl::SetColTextColor(int col,COLORREF color){
	stColor *pColor = new stColor;

	pColor->nCol = col;

	pColor->rgb = color;

	m_colTextColor.AddTail(pColor);
}

bool CMyCListCtrl::FindColTextColor(int col,COLORREF &color){
	for (POSITION pos = m_colTextColor.GetHeadPosition();pos!=NULL;){
		stColor *pColor = (stColor*)m_colTextColor.GetNext(pos);

		if (pColor->nCol==col){
			color = pColor->rgb;
			return true;
		}
	}

	return false;
}

//���ҵ�Ԫ��������ɫ
bool CMyCListCtrl::FindItemTextColor(int col,int row,COLORREF &color){
	for (POSITION pos = m_ItemTextColor.GetHeadPosition(); pos != NULL;){
		stColor *pColor = (stColor*)m_ItemTextColor.GetNext(pos);

		if (pColor->nCol == col && pColor->nRow == row){
			color = pColor->rgb;
			return true;
		}
	}

	return false;
}

void CMyCListCtrl::SetItemTextColor(int col,int row,COLORREF color){
	stColor *pColor = new stColor;

	pColor->nCol = col;

	pColor->nRow = row;

	pColor->rgb = color;

	m_ItemTextColor.AddTail(pColor);

}

//��ӦWM_ERASEBKGND��Ϣ
BOOL CMyCListCtrl::OnEraseBkgnd(CDC *pDC){
	//����Ĭ�ϴ���
	//return CListCtrl::OnEraseBkgnd(pDC);

	return FALSE;
}

//��ӦWM_PAINT��Ϣ
void CMyCListCtrl::OnPaint(){	
	CPaintDC dc(this);

	CRect rect;
	CRect headerRect;
	CDC MemDC;
	CBitmap MemMap;

	GetClientRect(&rect);
	GetDlgItem(0)->GetWindowRect(&headerRect);

	MemDC.CreateCompatibleDC(&dc);
	MemMap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	MemDC.SelectObject(&MemMap);
	MemDC.FillSolidRect(&rect,RGB(255, 255, 255));

	CPen pen(PS_SOLID, 1, RGB(88, 88, 88));
	MemDC.SelectObject(&pen);
	MemDC.MoveTo(0, headerRect.Height());
	MemDC.LineTo(rect.Width(), headerRect.Height());
	
	//����Ĭ�ϵ�OnPaint(),��ͼ�λ����ڴ�DC��
	DefWindowProc(WM_PAINT,(WPARAM)MemDC.m_hDC,(LPARAM)0);

	//���
	dc.BitBlt(0, headerRect.Height(),rect.Width(),rect.Height(),&MemDC, 0, headerRect.Height(),SRCCOPY);
	
	pen.DeleteObject();

	MemDC.DeleteDC();

	MemMap.DeleteObject();
}

void CMyCListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS){
	CDC dc;
	dc.Attach(lpDIS->hDC);//��ȡDC

	int nItemIndex = lpDIS->itemID;//��ȡ��ǰ�е�����

	CRect ListRect = lpDIS->rcItem;//��ȡ����һ�еľ��δ�С

	CHeaderCtrl* pHead = GetHeaderCtrl();//��ȡ��ͷ�ؼ�ָ��

	int nHeadCount = pHead->GetItemCount();//��ȡһ�е��е���Ŀ

	CRect TempRect;

	int nColumnIndex = 0;

	COLORREF txtColor;

	//����ÿһ��ʱ������һ��һ�д����ҽ��л��Ƶ�
	while (nColumnIndex < nHeadCount){
		pHead->GetItemRect(nColumnIndex,TempRect);//��ȡ��ǰ�У���ColumnIndex�е�Rect��С

		TempRect.top = ListRect.top;//top��bottom��һ������������Ҫ��ֵ

		TempRect.bottom = ListRect.bottom;

		if(nColumnIndex == 0){
			//TempRect.left += 1;
		}else if(nColumnIndex == nHeadCount - 1){
			//TempRect.right -= 1;
		}

		if (m_nSelColumn == nColumnIndex){//ѡ���еı���
			DrawBKGround(&dc,TempRect,lpDIS,nColumnIndex, TRUE);
		}else{//����ûѡ���еı������Ʊ���
			DrawBKGround(&dc,TempRect,lpDIS, nColumnIndex, FALSE);
		}

		//��ÿ�еĵ�һ�л���ͼ��
		//if(nColumnIndex == 0) DrawImage(&dc,TempRect.TopLeft(),nItemIndex);

		//���� nItemIndex�С�ColumnIndex�е�����
		CString strText = GetItemText(nItemIndex,nColumnIndex);

		if(nColumnIndex == 0){
			TempRect.left += 16 + 4;
		}else{
			TempRect.left += 4;
		}

		txtColor = RGB(0, 0, 0);

		//�Ȳ��� ��Ԫ���ı���ɫ����û�ҵ�������� �� ���ı���ɫ
		if(!FindItemTextColor(nColumnIndex, nItemIndex, txtColor)){
			FindColTextColor(nColumnIndex, txtColor);
		}

		dc.SetTextColor(txtColor);

		dc.DrawText(strText, &TempRect, DT_VCENTER | DT_SINGLELINE | DT_CENTER);

		nColumnIndex++;
	}

	if (lpDIS->itemState & ODS_FOCUS){
		CRect RtTemp = ListRect;

		RtTemp.DeflateRect(1, 1);

		dc.DrawFocusRect(RtTemp);
	}

	dc.Detach();
}

void CMyCListCtrl::DrawBKGround(CDC* pDC,CRect Rt,LPDRAWITEMSTRUCT lpDIS, int nColIndex, BOOL bColumnSel){
	if (lpDIS->itemState & ODS_SELECTED){//ѡ�������װ���
		pDC->FillSolidRect(&Rt,GetSysColor(COLOR_HIGHLIGHT));

		pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	}else{//��ѡ���вʵ׺���		
		if(FindColTextColor(nColIndex,m_tmpColor)){
			pDC->SetTextColor(m_tmpColor);
		}else{
			pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		}

		if (bColumnSel){//ĳһ�б�ѡ�У���ɫ����
			if (lpDIS->itemID % 2 == 0){
				pDC->FillSolidRect(&Rt,RGB(255,255,255));
				//pDC->FillSolidRect(&Rt,RGB(145,180,222));
			}else{
				pDC->FillSolidRect(&Rt,RGB(255,255,255));
				//pDC->FillSolidRect(&Rt,RGB(233,233,233));
			}
		}else{
			if (lpDIS->itemID % 2 == 0){
				pDC->FillSolidRect(&Rt,RGB(255,255,255));
				//pDC->FillSolidRect(&Rt,RGB(165,203,247));
			}else{
				pDC->FillSolidRect(&Rt,RGB(255,255,255));
				//pDC->FillSolidRect(&Rt,RGB(210,210,210));
			}
		}
	}
}
//-----------------------------------------------
/***********************************************
End CMyCListCtrl define
************************************************/
//-----------------------------------------------
