// Kolas2up_ViewerControl.cpp: implementation of the CKolas2up_ViewerControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kolas2up_ViewerControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "FONT.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKolas2up_ViewerControl::CKolas2up_ViewerControl()
{
	m_nRowHeight = 20;
	m_nTagColWidth = 100;
	m_nDataColWidth = 150;
	m_nTagColAlign = 4;
	m_nDataColAlign = 1;

	LIST.RemoveAll();

	InitColorArray();
	m_GridColor = RGB(0, 0, 0);
	m_nGridHeight = 0;

	AddColor(RGB(255, 162, 99), RGB(0, 0, 0), RGB(189, 227, 231), RGB(0, 0, 0));
	AddColor(RGB(255, 223, 206), RGB(0, 0, 0), RGB(239, 247, 247), RGB(0, 0, 0));
}

CKolas2up_ViewerControl::~CKolas2up_ViewerControl()
{
	FreeLIST();
}

INT CKolas2up_ViewerControl::FreeLIST(VOID)
{
	INT nData = LIST.GetCount();
	if (nData == 0) return 0;
	CKolas2up_ViewerControlData * pData;
	for (INT i = 0; i < nData; i++) 
	{
		pData = NULL;
		pData = (CKolas2up_ViewerControlData*)LIST.RemoveHead();
		if (pData) delete pData;
	}
	LIST.RemoveAll();
	return 0;
}

BEGIN_MESSAGE_MAP(CKolas2up_ViewerControl, CMSHFlexGrid)
	//{{AFX_MSG_MAP(CKolas2up_ViewerControl)
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CKolas2up_ViewerControlData * CKolas2up_ViewerControl::AddControlData(VOID)
{
	CKolas2up_ViewerControlData *pData = new CKolas2up_ViewerControlData;
	if (pData == NULL) return NULL;

	LIST.AddTail((CObject*)pData);
	return pData;
}

INT CKolas2up_ViewerControl::SetControlData(CString strTAG, CString strDATA, BOOL bReplace/*=TRUE*/)
{
	CKolas2up_ViewerControlData * pData = NULL;
	if (bReplace == TRUE) {
		pData = FindControlData(strTAG);
	}
	if (pData == NULL) {
		pData = AddControlData();
		if (pData == NULL) return -1;
	}
	pData->TAG = strTAG;
	pData->DATA = strDATA;
	return 0;
}

CKolas2up_ViewerControlData * CKolas2up_ViewerControl::FindControlData(CString strTAG)
{
	INT nData = LIST.GetCount();
	if (nData == 0) return NULL;
	CKolas2up_ViewerControlData * pData;
	POSITION pos = LIST.GetHeadPosition();
	for (INT i = 0; i < nData; i++) 
	{
		pData = NULL;
		pData = (CKolas2up_ViewerControlData*)LIST.GetNext(pos);
		if (pData == NULL) continue;
		if (pData->TAG == strTAG) return pData;
	}
	return NULL;
}

CKolas2up_ViewerControlData * CKolas2up_ViewerControl::FindControlData(INT nRowIdx)
{
	INT nData = LIST.GetCount();
	if (nData == 0) return NULL;
	CKolas2up_ViewerControlData * pData;
	POSITION pos = LIST.GetHeadPosition();
	for (INT i = 0; i < nData; i++) 
	{
		pData = NULL;
		pData = (CKolas2up_ViewerControlData*)LIST.GetNext(pos);
		if (pData == NULL) continue;
		if (i == nRowIdx) return pData;
	}
	return NULL;
}

INT CKolas2up_ViewerControl::FindControlDataIdx(CString strTAG)
{
	INT nData = LIST.GetCount();
	if (nData == 0) return -1;
	CKolas2up_ViewerControlData * pData;
	POSITION pos = LIST.GetHeadPosition();
	for (INT i = 0; i < nData; i++) 
	{
		pData = NULL;
		pData = (CKolas2up_ViewerControlData*)LIST.GetNext(pos);
		if (pData == NULL) continue;
		if (pData->TAG == strTAG) return i;
	}
	return -1;
}

INT CKolas2up_ViewerControl::SetControlDataBK(CString strTAG, CString strPART, COLORREF c)
{
	CKolas2up_ViewerControlData * pData = FindControlData(strTAG);
	if (pData == NULL) return -1;
	if (strPART == _T("TAG")) pData->cTAG_bk = c;
	if (strPART == _T("DATA")) pData->cDATA_bk = c;
	return 0;
}

INT CKolas2up_ViewerControl::SetControlDataFK(CString strTAG, CString strPART, COLORREF c)
{
	CKolas2up_ViewerControlData * pData = FindControlData(strTAG);
	if (pData == NULL) return -1;
	if (strPART == _T("TAG")) pData->cTAG_fk = c;
	if (strPART == _T("DATA")) pData->cDATA_fk = c;
	return 0;
}

INT CKolas2up_ViewerControl::SetControlDataBK(INT nRowIdx, CString strPART, COLORREF c)
{
	CKolas2up_ViewerControlData * pData = FindControlData(nRowIdx);
	if (pData == NULL) return -1;
	if (strPART == _T("TAG")) pData->cTAG_bk = c;
	if (strPART == _T("DATA")) pData->cDATA_bk = c;
	return 0;
}

INT CKolas2up_ViewerControl::SetControlDataFK(INT nRowIdx, CString strPART, COLORREF c)
{
	CKolas2up_ViewerControlData * pData = FindControlData(nRowIdx);
	if (pData == NULL) return -1;
	if (strPART == _T("TAG")) pData->cTAG_fk = c;
	if (strPART == _T("DATA")) pData->cDATA_fk = c;
	return 0;
}

INT CKolas2up_ViewerControl::GetRowCount(VOID)
{
	return LIST.GetCount();
}

INT CKolas2up_ViewerControl::SetViewerRowHeight(INT nRowHeight)
{
	m_nRowHeight = nRowHeight;
	return 0;
}

INT CKolas2up_ViewerControl::SetViewerTagColWidth(INT nWidth)
{
	m_nTagColWidth = nWidth;
	return 0;
}

INT CKolas2up_ViewerControl::SetViewerDataColWidth(INT nWidth)
{
	m_nDataColWidth = nWidth;
	return 0;
}

BOOL CKolas2up_ViewerControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	INT nRowsScrolled = 1 * zDelta / 120;

    if (nRowsScrolled > 0)
        for (INT i = 0; i < nRowsScrolled; i++)
           this->PostMessage(WM_VSCROLL, SB_LINEUP, 0);
    else
        for (INT i = 0; i > nRowsScrolled; i--)
            this->PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);

	return CMSHFlexGrid::OnMouseWheel(nFlags, zDelta, pt);
}

INT CKolas2up_ViewerControl::Display(VOID)
{
	INT nRows = LIST.GetCount();
	if (nRows < 2) return -1;

	SetRows(nRows);
	SetCols(0, 2);
	SetColWidth(0, 0, m_nTagColWidth*15);
	SetColWidth(1, 0, m_nDataColWidth*15);
	SetColAlignment(0, m_nTagColAlign);
	SetColAlignment(1, m_nDataColAlign);

	INT nRow = GetRow();
	INT nCol = GetCol();

	SetRowHeight(-1, m_nRowHeight * 15);

	CKolas2up_ViewerControlData * pData;
	POSITION pos = LIST.GetHeadPosition();
	INT nColors = m_arrColorTagBK.GetSize();
	INT nCurColorIdx;
	COLORREF Tagbk, Tagfk, Databk, Datafk;
	COleFont font = GetFont();
	CY y = font.GetSize();
	double nRealWidth = y.Lo * 1.389 / 10000.0;
	INT nColWidth2 = GetColWidth(1, 0);
	INT nChars = (INT)((nColWidth2 / nRealWidth / 15) + 0.5);
	nChars  *= 2;

	for (INT i = 0; i < nRows; i++) 
	{
		pData = NULL;
		pData = (CKolas2up_ViewerControlData*)LIST.GetNext(pos);
		if (pData == NULL) continue;
		SetRow(i);

		if (nColors > 0) {
			nCurColorIdx = i % nColors;
			Tagbk = m_arrColorTagBK.GetAt(nCurColorIdx);
			Tagfk = m_arrColorTagFK.GetAt(nCurColorIdx);
			Databk = m_arrColorDataBK.GetAt(nCurColorIdx);
			Datafk = m_arrColorDataFK.GetAt(nCurColorIdx);
		}
		SetCol(0);
		SetText(pData->TAG);
		if (nColors == 0) {
			SetCellBackColor(pData->cTAG_bk);
			SetCellForeColor(pData->cTAG_fk);
		} else {
			SetCellBackColor(Tagbk);
			SetCellForeColor(Tagfk);
		}

		SetCol(1);
		SetText(pData->DATA);
		if (nColors == 0) {
			SetCellBackColor(pData->cDATA_bk);
			SetCellForeColor(pData->cDATA_fk);
		} else {
			SetCellBackColor(Databk);
			SetCellForeColor(Datafk);
		}
		INT nLen = pData->DATA.GetLength();
		INT nRows = nLen / nChars;
		INT nDiv = nLen % nChars;
		if (nDiv > 0) nRows += 1;
		if (nRows > 1) {
			SetRowHeight(i, (long)((m_nRowHeight * 15) + ((nRows-1) * (y.Lo / 10000 * 1.38) * 15)));
		}
	}
	if (m_nGridHeight == 0) {	
		SetGridLines(m_nGridHeight);
	} else {
		SetGridLines(m_nGridHeight);
		SetGridColor(m_GridColor);
	}
	return 0;
}

INT CKolas2up_ViewerControl::AddColor(COLORREF Tagbk, COLORREF Tagfk, COLORREF Databk, COLORREF Datafk)
{
	m_arrColorTagBK.Add(Tagbk);
	m_arrColorTagFK.Add(Tagfk);
	m_arrColorDataBK.Add(Databk);
	m_arrColorDataFK.Add(Datafk);
	return 0;
}

INT CKolas2up_ViewerControl::InitViewer(UINT ID, CWnd *pParentWnd)
{
	if (SubclassDlgItem(ID, pParentWnd) == FALSE) return -1;

	SetRows(2);
	SetCols(0, 2);
	for (INT i = 0; i < 2; i++) SetRowHeight(i, 0);
	return 0;
}

INT CKolas2up_ViewerControl::InitColorArray(VOID)
{
	m_arrColorTagBK.RemoveAll();
	m_arrColorTagFK.RemoveAll();
	m_arrColorDataBK.RemoveAll();
	m_arrColorDataFK.RemoveAll();
	return 0;
}

INT CKolas2up_ViewerControl::SetViewerWidth(INT nTagPercent, INT nDataPercent)
{
	CRect r;
	GetWindowRect(r);
	INT nWidth = r.right - r.left;

	SetViewerTagColWidth(nWidth * nTagPercent / 100);
	SetViewerDataColWidth(nWidth * nDataPercent / 100);
	return 0;
}