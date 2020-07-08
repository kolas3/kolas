// ColorListCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "GenDlg.h"
#include "ColorListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorListCtrl

IMPLEMENT_DYNCREATE(CColorListCtrl, CListCtrl)

CColorListCtrl::CColorListCtrl()
{
	GridMode = false;
	AltRowMode = false;
	MaskMode = false;
	m_bUseVerticalLine = FALSE;
	m_cfVerticalLine = RGB(0,0,0);
	
	// KOL.UDF.022
	Gx = NULL;
	Gy = NULL;
	Rx = NULL;
	Ry = NULL;
	m_cfVerticalLine = NULL;
}

CColorListCtrl::~CColorListCtrl()
{
}


BEGIN_MESSAGE_MAP(CColorListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CColorListCtrl)
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnDeleteitem)
	ON_NOTIFY_REFLECT(LVN_DELETEALLITEMS, OnDeleteallitems)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, CustDraw)
	ON_NOTIFY_REFLECT(LVN_INSERTITEM, OnInsertitem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//BEGIN_MESSAGE_MAP(CColorListCtrl, CListView)
//END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CColorListCtrl drawing


/////////////////////////////////////////////////////////////////////////////
// CColorListCtrl diagnostics

#ifdef _DEBUG
void CColorListCtrl::AssertValid() const
{
	CListCtrl::AssertValid();
}

void CColorListCtrl::Dump(CDumpContext& dc) const
{
	CListCtrl::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CColorListCtrl message handlers
// 커스텀 드로우
void CColorListCtrl::CustDraw(NMHDR *pNotifyStruct, LRESULT *result)
{
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW) pNotifyStruct; // cast our generic structure to bigger/specialized strc
	int i = 0;
	long iRow = 0, iCol = 0;
	long iCIndex = 0, iCelIndex = 0, iFontIndex = 0, iRIndex = 0;

	switch(lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*result  = CDRF_NOTIFYITEMDRAW|CDRF_NOTIFYSUBITEMDRAW;	// request notifications for individual items
		break;

	case CDDS_ITEMPREPAINT:
		*result = CDRF_NOTIFYSUBITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:	// drawing subitem
		iRow = lplvcd->nmcd.dwItemSpec;	
		iCol = lplvcd->iSubItem;
		
		if(GridMode) DrawGrid(lplvcd, iRow, iCol); // grid mode
		if(AltRowMode)	DrawAltRow(lplvcd, iRow);//alternate row mode

		iRIndex = findDetails(iRow,ROW);	// individual cell,coloumn,row color setting
		iCIndex = findDetails(iCol,COL);
		iCelIndex = findDetails(iRow,CELL,iCol);
		iFontIndex = findDetails(iRow,FONT,iCol);
		
		// 열컬러, 행컬러, 셀컬러가 모두 설정이 안되어있을 때		
		if(iRIndex == -1 && iCIndex == -1 && iCelIndex == -1 )
		{
			if(!GridMode && !AltRowMode)
			{
				lplvcd->clrTextBk = GetBkColor();
			}
		}
		else
		{
			if(-1 != iCelIndex)			
				lplvcd->clrTextBk = CellColors[iCelIndex].cf;											
			else if(-1 != iRIndex)
				lplvcd->clrTextBk = RowColors[iRIndex].cf;
			else
				lplvcd->clrTextBk = ColColors[iCIndex].cf;
		}
		if(MaskMode)		// text mask mode
			lplvcd->clrText = InvertColor(lplvcd->clrTextBk);

		if(iFontIndex != -1)
			lplvcd->clrText = FontColors[iFontIndex].cf;		
		else
			lplvcd->clrText = GetSysColor(COLOR_WINDOWTEXT);

		*result = CDRF_NEWFONT;
		break;
	default:
		*result = CDRF_DODEFAULT;
	}
}

void CColorListCtrl::SetRowColor(COLORREF cf,long row)
{
	ColorStruct cs;
	cs.cf = cf;
	cs.id = row;
	RowColors.push_back(cs);
	RedrawWindow(NULL,NULL,RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

void CColorListCtrl::SetColColor(COLORREF cf,long col)
{
	ColorStruct cs;
	cs.cf = cf;
	cs.id = col;
	ColColors.push_back(cs);
	RedrawWindow(NULL,NULL,RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

void CColorListCtrl::SetCellColor(COLORREF cf,long row,long col)
{
	ColorStruct cs;
	cs.cf = cf;
	cs.id = row;
	cs.subId = col;
	CellColors.push_back(cs);
	RedrawWindow(NULL,NULL,RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

long CColorListCtrl::findDetails(long id,RecType rt,long subId)
{
	if( rt == ROW)
	{
		for(int i=0;i< RowColors.size(); i++)
		{
			if(RowColors[i].id == id)
				return i;
		}
	}

	if( rt == COL)
	{
		for(int i=0;i< ColColors.size(); i++)
		{
			if(ColColors[i].id == id)
				return i;
		}
	}

	if( rt == CELL)
	{
		for(int i=0;i< CellColors.size(); i++)
		{
			if( subId >=0){
				if(CellColors[i].id == id && CellColors[i].subId == subId)
					return i;
			}
		}
	}

	if( rt == FONT)
	{
		for(int i=0;i< FontColors.size(); i++)
		{
			if( subId >=0){
				if(FontColors[i].id == id && FontColors[i].subId == subId)
					return i;
			}
		}
	}
	return -1;
}

void CColorListCtrl::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	// TODO: Add your control notification handler code here
	long row  = pNMListView->iItem;
	long index = findDetails(row);
	
	for(int i=0;i< RowColors.size();i++)
	{
		if(RowColors[i].id == row){
			RowColors.erase(RowColors.begin()+i);
			i--;
			continue;
		}
		if(RowColors[i].id > row){
			RowColors[i].id--;
		}
	}
	for(i=0;i< CellColors.size();i++)
	{
		if(CellColors[i].id == row){
			CellColors.erase(CellColors.begin()+i);
			i--;
			continue;
		}
		if(CellColors[i].id > row){
			CellColors[i].id--;
		}
	}
	*pResult = 0;
}

void CColorListCtrl::OnDeleteallitems(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	RowColors.clear();
	ColColors.clear();
	CellColors.clear();
	*pResult = 0;
}

void CColorListCtrl::OnInsertitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	long row  = pNMListView->iItem;
	long col  = pNMListView->iSubItem;
		
	for(int i=0;i< RowColors.size();i++)
	{
		if(RowColors[i].id > row){
			RowColors[i].id++;
		}
	}

	for(i=0;i< CellColors.size();i++)
	{
	if(CellColors[i].id > row){
			CellColors[i].id++;
		}
	}
	
	for(i=0;i< FontColors.size();i++)
	{
	if(FontColors[i].id > row){
			FontColors[i].id++;
		}
	}

	*pResult = 0;
}

void CColorListCtrl::SetGridMode(bool TurnOn,COLORREF x,COLORREF y)
{
	Gx = y;
	Gy = x;
	GridMode = TurnOn;
	RedrawWindow(NULL,NULL,RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

void CColorListCtrl::SetAltRowColors(bool TurnOn,COLORREF x,COLORREF y)
{
	Rx = x;
	Ry = y;
	AltRowMode = TurnOn;
	RedrawWindow(NULL,NULL,RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

void CColorListCtrl::SetTextMask(bool TurnOn)
{
	MaskMode = TurnOn;
	RedrawWindow(NULL,NULL,RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

void CColorListCtrl::ClrRowColor(long row)
{
	long index = findDetails(row);
	if(index != -1)
		RowColors.erase(RowColors.begin()+index);
	RedrawWindow(NULL,NULL,RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

void CColorListCtrl::ClrColColor(long col)
{
	long index = findDetails(col,COL);
	if(index != -1)
		ColColors.erase(ColColors.begin()+index);
	RedrawWindow(NULL,NULL,RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

void CColorListCtrl::ClrCellColor(long row,long col)
{
	long index = findDetails(row,CELL,col);
	if(index != -1)
		CellColors.erase(CellColors.begin()+index);
	RedrawWindow(NULL,NULL,RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

COLORREF CColorListCtrl::InvertColor(COLORREF cf)
{
	return RGB(abs(220-GetRValue(cf)),abs(220-GetGValue(cf)),abs(220-GetBValue(cf)));
}

// 색상 초기화
void CColorListCtrl::ResetColors(void)
{
	RowColors.clear();
	ColColors.clear();
	CellColors.clear();
	FontColors.clear();
	GridMode = false;
	AltRowMode = false;
	MaskMode = false;
	RedrawWindow(NULL,NULL,RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

// 폰트색상 설정
void CColorListCtrl::SetFontColor(COLORREF cf, long row, long col)
{
	ColorStruct cs;
	cs.cf = cf;
	cs.id = row;
	cs.subId = col;
	FontColors.push_back(cs);
	RedrawWindow(NULL,NULL,RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

// DrawGrid
// 두가지 색으로 체스판과 같이 번갈아가며 그려줌
void CColorListCtrl::DrawGrid(LPNMLVCUSTOMDRAW lplvcd, long iRow, long iCol)
{
	if( iRow %2 == 1)
	{
		if( iCol %2 == 0)
			lplvcd->clrTextBk = Gx;
		else
			lplvcd->clrTextBk = Gy;
	}
	else
	{
		if( iCol %2 == 0)
			lplvcd->clrTextBk = Gy;
		else
			lplvcd->clrTextBk = Gx;
	}
}

// DrawAltRow
// 두가지색상으로 라인색깔을 번갈아가며 설정
void CColorListCtrl::DrawAltRow(LPNMLVCUSTOMDRAW lplvcd, long iRow)
{
	if( iRow %2 == 1)
		lplvcd->clrTextBk = Rx;
	else
		lplvcd->clrTextBk = Ry;
}


void CColorListCtrl::OnPaint()
{
	CListCtrl::OnPaint();	
	if((GetStyle() & LVS_TYPEMASK) == LVS_REPORT )
	{
		if(true==m_bUseVerticalLine)
		{
			DrawVerticalLine();
		}
	}
}

// 구분선 색상설정
void CColorListCtrl::SetVerticalLineColor(COLORREF cf)
{
	m_cfVerticalLine = cf;
}

// 구분선 보이기 설정
void CColorListCtrl::ShowVerticalLine(bool bShowLine)
{
	m_bUseVerticalLine = bShowLine;
}

// 구분선 그려주기
void CColorListCtrl::DrawVerticalLine()
{
	CClientDC dc(this);		
	CRect rect;
	GetClientRect(rect);
	CPen Pen, *OldPen;
	Pen.CreatePen(PS_SOLID,1,m_cfVerticalLine);
	OldPen = dc.SelectObject(&Pen);
	
	int top = rect.top;
	int borderx = 0 - GetScrollPos(SB_HORZ);
	int nColumnCount = GetItemCount();
	for(int i=0;i<nColumnCount;i++)
	{
		borderx += GetColumnWidth( i );			
		// if next border is outside client area, break out
		if( borderx >= rect.right ) break;			
		// Draw the line.			
		dc.MoveTo( borderx-1, top);
		dc.LineTo( borderx-1, rect.bottom );
	}	
	dc.SelectObject(OldPen);
}

BOOL CColorListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	/*
	HD_NOTIFY   *pHDN = (HD_NOTIFY*)lParam;
	if((pHDN->hdr.code == HDN_ITEMCHANGINGW || pHDN->hdr.code == HDN_ITEMCHANGINGA)) 
	{
		Invalidate(FALSE);
	}
	*/
	return CListCtrl::OnNotify(wParam, lParam, pResult);
}

void CColorListCtrl::ClrFontColor(long row, long col)
{
	long index = findDetails(row,FONT,col);
	if(index != -1)
		FontColors.erase(FontColors.begin()+index);
	RedrawWindow(NULL,NULL,RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}
