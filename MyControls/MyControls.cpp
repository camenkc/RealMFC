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

// CMyHeaderCtrl 消息处理程序
BOOL CMyHeaderCtrl::OnEraseBkgnd(CDC *pDC){
	//清除背景，屏蔽默认处理
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

	// 不为绘图消息调用 CHeaderCtrl::OnPaint()
	int nItem;

	nItem = GetItemCount();//得到有几个单元

	////////
	CFont newFont;
	newFont.CreatePointFont(110 ,"微软雅黑");
	MemDC.SelectObject(&newFont);

	int nIndex = 0;

	////////
	for(int i = 0; i < nItem; i++){
		if(m_HChar.GetSize() <= i){
			break;
		}

		CRect tRect;

		GetItemRect(i, &tRect);//得到Item的尺寸

		CRect nRect(tRect);//拷贝尺寸到新的容器中

		if(i > 0){//留出分割线的地方
			nRect.left++;
		}else if(i == nItem - 1){
			nRect.right++;
		}

		
		//绘制立体背景
		nIndex = 0;
		for(int j = tRect.top; j<= tRect.bottom; j++){
			nRect.bottom = nRect.top + 1;

			CBrush _brush;

			_brush.CreateSolidBrush(doColor(nIndex++));//创建画刷

			MemDC.FillRect(&nRect,&_brush); //填充背景

			_brush.DeleteObject(); //释放画刷

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

	//画头部剩余部分
	CRect rtRect;

	CRect clientRect;

	GetItemRect(nItem - 1,rtRect);

	GetClientRect(clientRect);

	rtRect.left = rtRect.right+1;

	rtRect.right = clientRect.right;

	CRect nRect(rtRect);

	nIndex = 0;
	//绘制立体背景
	for(int j = rtRect.top;j<=rtRect.bottom;j++){
		nRect.bottom = nRect.top+1;

		CBrush _brush;

		_brush.CreateSolidBrush(doColor(nIndex++));//创建画刷

		MemDC.FillRect(&nRect,&_brush); //填充背景

		_brush.DeleteObject(); //释放画刷

		nRect.top = nRect.bottom;
	}

	newFont.DeleteObject();

	//输出
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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

//设置表头背景色  Gradient - 渐变系数，立体背景用,不用渐变设为0
void CMyCListCtrl::SetHeaderBKColor(int R, int G, int B, int Gradient){ 
	m_Header.m_R = R;

	m_Header.m_G = G;

	m_Header.m_B = B;

	m_Header.m_Gradient = Gradient;
}

//设置表头高度(倍率)
void CMyCListCtrl::SetHeaderHeightFactor(float m_fMagFactor){
	m_Header.m_fMagFactor = (m_fMagFactor > 1.0 ? m_fMagFactor : 1.0);
}


//高置行高倍率
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

//查找列颜色
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

//查找颜色
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

//设置列颜色
void CMyCListCtrl::SetColColor(int col, COLORREF color){
	stColor *pColor  = new stColor;

	pColor->nCol = col;

	pColor->rgb = color;

	m_ptrListCol.AddTail(pColor);
}

//设置格子颜色
void CMyCListCtrl::SetItemColor(int col,int row,COLORREF color) {
	stColor *pColor  = new stColor;

	pColor->nCol = col;

	pColor->nRow = row;

	pColor->rgb = color;

	m_ptrListItem.AddTail(pColor);
}

//设置头部字体宽和高
void CMyCListCtrl::SetHeaderFontHW(int nHeight,int nWith){
	m_Header.m_fontHeight = nHeight;

	m_Header.m_fontWith = nWith;
}

void CMyCListCtrl::SetHeaderTextColor(COLORREF color){ //设置头部字体颜色
	m_Header.m_color = color;
}

BOOL CMyCListCtrl::SetTextColor(COLORREF cr){ //设置字体颜色
	m_color = cr;

	return TRUE;
}

void CMyCListCtrl::SetFontHW(int nHeight,int nWith){ //设置字体高和宽
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

//查找单元格字体颜色
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

//响应WM_ERASEBKGND消息
BOOL CMyCListCtrl::OnEraseBkgnd(CDC *pDC){
	//屏蔽默认处理
	//return CListCtrl::OnEraseBkgnd(pDC);

	return FALSE;
}

//响应WM_PAINT消息
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
	
	//调用默认的OnPaint(),把图形画在内存DC上
	DefWindowProc(WM_PAINT,(WPARAM)MemDC.m_hDC,(LPARAM)0);

	//输出
	dc.BitBlt(0, headerRect.Height(),rect.Width(),rect.Height(),&MemDC, 0, headerRect.Height(),SRCCOPY);
	
	pen.DeleteObject();

	MemDC.DeleteDC();

	MemMap.DeleteObject();
}

void CMyCListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS){
	CDC dc;
	dc.Attach(lpDIS->hDC);//获取DC

	int nItemIndex = lpDIS->itemID;//获取当前行的索引

	CRect ListRect = lpDIS->rcItem;//获取整个一行的矩形大小

	CHeaderCtrl* pHead = GetHeaderCtrl();//获取标头控件指针

	int nHeadCount = pHead->GetItemCount();//获取一行的列的数目

	CRect TempRect;

	int nColumnIndex = 0;

	COLORREF txtColor;

	//绘制每一行时，按照一列一列从左到右进行绘制的
	while (nColumnIndex < nHeadCount){
		pHead->GetItemRect(nColumnIndex,TempRect);//获取当前行，第ColumnIndex列的Rect大小

		TempRect.top = ListRect.top;//top和bottom不一样，所以这里要赋值

		TempRect.bottom = ListRect.bottom;

		if(nColumnIndex == 0){
			//TempRect.left += 1;
		}else if(nColumnIndex == nHeadCount - 1){
			//TempRect.right -= 1;
		}

		if (m_nSelColumn == nColumnIndex){//选中列的背景
			DrawBKGround(&dc,TempRect,lpDIS,nColumnIndex, TRUE);
		}else{//绘制没选中列的背景绘制背景
			DrawBKGround(&dc,TempRect,lpDIS, nColumnIndex, FALSE);
		}

		//对每行的第一列绘制图标
		//if(nColumnIndex == 0) DrawImage(&dc,TempRect.TopLeft(),nItemIndex);

		//绘制 nItemIndex行、ColumnIndex列的文字
		CString strText = GetItemText(nItemIndex,nColumnIndex);

		if(nColumnIndex == 0){
			TempRect.left += 16 + 4;
		}else{
			TempRect.left += 4;
		}

		txtColor = RGB(0, 0, 0);

		//先查找 单元格文本颜色，若没找到，则查找 列 的文本颜色
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
	if (lpDIS->itemState & ODS_SELECTED){//选中行蓝底白字
		pDC->FillSolidRect(&Rt,GetSysColor(COLOR_HIGHLIGHT));

		pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	}else{//非选中行彩底黑字		
		if(FindColTextColor(nColIndex,m_tmpColor)){
			pDC->SetTextColor(m_tmpColor);
		}else{
			pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		}

		if (bColumnSel){//某一列被选中，颜色加深
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
