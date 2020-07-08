// MSHFlexGridInterface.cpp: implementation of the CMSFlexGridInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSHFlexGridInterface.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMSHFlexGridInterface::CMSHFlexGridInterface()
{
	m_lRow = 0;
	m_lCol = 0;
	m_bIsEditSel = FALSE;
	m_bIsMain = TRUE;

	CGridCtrl::EnableDragAndDrop(FALSE);
	CGridCtrl::SetDefCellHeight(16);
	CGridCtrl::SetDefCellWidth(32);
	CGridCtrl::SetDefCellMargin(1);

	SetRows(1);
	SetCols(0, 2);
	CGridCtrl::SetFixedRowCount(1);
	CGridCtrl::SetFixedColumnCount(1);

	SetDefaultColor();
}

CMSHFlexGridInterface::~CMSHFlexGridInterface()
{
}

long CMSHFlexGridInterface::GetRows()
{
	return GetRowCount();
}
VOID CMSHFlexGridInterface::SetRows(long nNewValue)
{
	INT nMinRow = GetFixedRowCount() + 1;
	if(nNewValue < nMinRow)
	{
		SetRowCount(nMinRow);
	}
	else
	{
		SetRowCount(nNewValue);
	}
}
long CMSHFlexGridInterface::GetCols(long BandNumber)
{
	return GetColumnCount();	
}
VOID CMSHFlexGridInterface::SetCols(long BandNumber, long nNewValue)
{
	CGridCtrl::SetColumnCount(nNewValue);
}
long CMSHFlexGridInterface::GetFixedRows()
{
	return CGridCtrl::GetFixedRowCount();
}
VOID CMSHFlexGridInterface::SetFixedRows(long nNewValue)
{
	SetFixedRowCount(nNewValue);
}
long CMSHFlexGridInterface::GetFixedCols()
{
	return CGridCtrl::GetFixedColumnCount();
}
VOID CMSHFlexGridInterface::SetFixedCols(long nNewValue)
{
	SetFixedColumnCount(nNewValue);
}
short CMSHFlexGridInterface::GetVersion()
{
	return 0;
}
CString CMSHFlexGridInterface::GetFormatString()
{
	return _T("");
}
VOID CMSHFlexGridInterface::SetFormatString(LPCTSTR lpszNewValue)
{
}
long CMSHFlexGridInterface::GetTopRow()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetTopRow(long nNewValue)
{
	SetFocusCell(nNewValue, 0);
}
long CMSHFlexGridInterface::GetLeftCol()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetLeftCol(long nNewValue)
{
}
long CMSHFlexGridInterface::GetRow()
{
	return m_lRow;
}
VOID CMSHFlexGridInterface::SetRow(long nNewValue)
{
	m_lRow = nNewValue;
}
long CMSHFlexGridInterface::GetCol()
{	
	return m_lCol;
}
VOID CMSHFlexGridInterface::SetCol(long nNewValue)
{
	m_lCol = nNewValue;
}
long CMSHFlexGridInterface::GetRowSel()
{	
	return GetFocusCell().row;
}
VOID CMSHFlexGridInterface::SetRowSel(long nNewValue)
{
}
long CMSHFlexGridInterface::GetColSel()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetColSel(long nNewValue)
{
}
CString CMSHFlexGridInterface::GetText()
{
	return _T("");
}
VOID CMSHFlexGridInterface::SetText(LPCTSTR lpszNewValue)
{
	
}
DWORD CMSHFlexGridInterface::GetBackColor()
{
	return RGB(255, 255, 255);
}
VOID CMSHFlexGridInterface::SetBackColor(DWORD newValue)
{
	
}
DWORD CMSHFlexGridInterface::GetForeColor()
{
	return RGB(0,0,0);
}
VOID CMSHFlexGridInterface::SetForeColor(DWORD newValue)
{
	CGridCtrl::SetGridColor(newValue);
}
DWORD CMSHFlexGridInterface::GetBackColorBand(long BandNumber)
{
	return RGB(0,0,0);
}
VOID CMSHFlexGridInterface::SetBackColorBand(long BandNumber, DWORD newValue)
{
}
DWORD CMSHFlexGridInterface::GetForeColorBand(long BandNumber)
{
	return RGB(0,0,0);
}
VOID CMSHFlexGridInterface::SetForeColorBand(long BandNumber, DWORD newValue)
{
}
DWORD CMSHFlexGridInterface::GetBackColorHeader(long BandNumber)
{
	return RGB(0,0,0);
}
VOID CMSHFlexGridInterface::SetBackColorHeader(long BandNumber, DWORD newValue)
{
}
DWORD CMSHFlexGridInterface::GetForeColorHeader(long BandNumber)
{
	return RGB(0,0,0);
}
VOID CMSHFlexGridInterface::SetForeColorHeader(long BandNumber, DWORD newValue)
{
}
DWORD CMSHFlexGridInterface::GetBackColorIndent(long BandNumber)
{
	return RGB(0,0,0);
}
VOID CMSHFlexGridInterface::SetBackColorIndent(long BandNumber, DWORD newValue)
{
}
DWORD CMSHFlexGridInterface::GetBackColorFixed()
{
	return RGB(0,0,0);
}
VOID CMSHFlexGridInterface::SetBackColorFixed(DWORD newValue)
{
	CGridCtrl::SetFixedBkColor(newValue);
}
DWORD CMSHFlexGridInterface::GetForeColorFixed()
{
	return RGB(0,0,0);
}
VOID CMSHFlexGridInterface::SetForeColorFixed(DWORD newValue)
{
	CGridCtrl::SetFixedTextColor(newValue);
}
DWORD CMSHFlexGridInterface::GetBackColorSel()
{
	return RGB(0,0,0);
}
VOID CMSHFlexGridInterface::SetBackColorSel(DWORD newValue)
{
}
DWORD CMSHFlexGridInterface::GetForeColorSel()
{
	return RGB(0,0,0);
}
VOID CMSHFlexGridInterface::SetForeColorSel(DWORD newValue)
{
}
DWORD CMSHFlexGridInterface::GetBackColorBkg()
{
	return CGridCtrl::GetBkColor();	
}
VOID CMSHFlexGridInterface::SetBackColorBkg(DWORD newValue)
{
	CGridCtrl::SetBkColor(newValue);
}
DWORD CMSHFlexGridInterface::GetBackColorUnpopulated()
{
	return RGB(0,0,0);
}
VOID CMSHFlexGridInterface::SetBackColorUnpopulated(DWORD newValue)
{
}
BOOL CMSHFlexGridInterface::GetWordWrap()
{
	return FALSE;
}
VOID CMSHFlexGridInterface::SetWordWrap(BOOL bNewValue)
{
	
}
COleFont CMSHFlexGridInterface::GetFont()
{
	return NULL;
}
VOID CMSHFlexGridInterface::SetRefFont(LPDISPATCH newValue)
{
}
float CMSHFlexGridInterface::GetFontWidth()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetFontWidth(float newValue)
{
}
COleFont CMSHFlexGridInterface::GetFontFixed()
{
	return NULL;
}
VOID CMSHFlexGridInterface::SetRefFontFixed(LPDISPATCH newValue)
{
}
float CMSHFlexGridInterface::GetFontWidthFixed()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetFontWidthFixed(float newValue)
{
}
COleFont CMSHFlexGridInterface::GetFontBand(long BandNumber)
{
	return NULL;
}
VOID CMSHFlexGridInterface::SetRefFontBand(long BandNumber, LPDISPATCH newValue)
{
}
float CMSHFlexGridInterface::GetFontWidthBand(long BandNumber)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetFontWidthBand(long BandNumber, float newValue)
{
}
COleFont CMSHFlexGridInterface::GetFontHeader(long BandNumber)
{
	return NULL;
}
VOID CMSHFlexGridInterface::SetRefFontHeader(long BandNumber, LPDISPATCH newValue)
{
}
float CMSHFlexGridInterface::GetFontWidthHeader(long BandNumber)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetFontWidthHeader(long BandNumber, float newValue)
{
}
CString CMSHFlexGridInterface::GetCellFontName()
{
	return _T("");
}
VOID CMSHFlexGridInterface::SetCellFontName(LPCTSTR lpszNewValue)
{
}
float CMSHFlexGridInterface::GetCellFontSize()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetCellFontSize(float newValue)
{
}
BOOL CMSHFlexGridInterface::GetCellFontBold()
{
	return FALSE;
}
VOID CMSHFlexGridInterface::SetCellFontBold(BOOL bNewValue)
{
}
BOOL CMSHFlexGridInterface::GetCellFontItalic()
{	
	return FALSE;
}
VOID CMSHFlexGridInterface::SetCellFontItalic(BOOL bNewValue)
{
}
BOOL CMSHFlexGridInterface::GetCellFontUnderline()
{
	return FALSE;
}
VOID CMSHFlexGridInterface::SetCellFontUnderline(BOOL bNewValue)
{
}
BOOL CMSHFlexGridInterface::GetCellFontStrikeThrough()
{
	return FALSE;
}
VOID CMSHFlexGridInterface::SetCellFontStrikeThrough(BOOL bNewValue)
{
}
float CMSHFlexGridInterface::GetCellFontWidth()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetCellFontWidth(float newValue)
{
}
long CMSHFlexGridInterface::GetTextStyle()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetTextStyle(long nNewValue)
{
}
long CMSHFlexGridInterface::GetTextStyleFixed()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetTextStyleFixed(long nNewValue)
{
}
long CMSHFlexGridInterface::GetTextStyleBand(long BandNumber)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetTextStyleBand(long BandNumber, long nNewValue)
{
}
long CMSHFlexGridInterface::GetTextStyleHeader(long BandNumber)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetTextStyleHeader(long BandNumber, long nNewValue)
{
}
BOOL CMSHFlexGridInterface::GetScrollTrack()
{
	return FALSE;
}
VOID CMSHFlexGridInterface::SetScrollTrack(BOOL bNewValue)
{
}
long CMSHFlexGridInterface::GetFocusRect()
{
	
	return 0;
}
VOID CMSHFlexGridInterface::SetFocusRect(long nNewValue)
{
	
}
long CMSHFlexGridInterface::GetHighLight()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetHighLight(long nNewValue)
{
	
}
BOOL CMSHFlexGridInterface::GetRedraw()
{
		return FALSE;
}
VOID CMSHFlexGridInterface::SetRedraw(BOOL bNewValue)
{
	CGridCtrl::SetRedraw(bNewValue);
}
long CMSHFlexGridInterface::GetScrollBars()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetScrollBars(long nNewValue)
{
}
long CMSHFlexGridInterface::GetMouseRow()
{
	CPoint Pos;
	GetCursorPos(&Pos);
	ScreenToClient(&Pos);
	CCellID Cell = CGridCtrl::GetCellFromPt(Pos);
		
	return Cell.row;
}
long CMSHFlexGridInterface::GetMouseCol()
{
	CPoint Pos;
	GetCursorPos(&Pos);
	ScreenToClient(&Pos);
	CCellID Cell = CGridCtrl::GetCellFromPt(Pos);
		
	return Cell.col;
}
long CMSHFlexGridInterface::GetCellLeft()
{
	return 0;
}
long CMSHFlexGridInterface::GetCellTop()
{
	return 0;
}
long CMSHFlexGridInterface::GetCellWidth()
{
	return 0;
}
long CMSHFlexGridInterface::GetCellHeight()
{
	return 0;
}
long CMSHFlexGridInterface::GetRowHeightMin()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetRowHeightMin(long nNewValue)
{
}
long CMSHFlexGridInterface::GetFillStyle()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetFillStyle(long nNewValue)
{
}
long CMSHFlexGridInterface::GetGridLines()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetGridLines(long nNewValue)
{
}
long CMSHFlexGridInterface::GetGridLinesFixed()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetGridLinesFixed(long nNewValue)
{
}
DWORD CMSHFlexGridInterface::GetGridColor()
{
	return RGB(0,0,0);
}
VOID CMSHFlexGridInterface::SetGridColor(DWORD newValue)
{
}
DWORD CMSHFlexGridInterface::GetGridColorFixed()
{
	return RGB(0,0,0);
}
VOID CMSHFlexGridInterface::SetGridColorFixed(DWORD newValue)
{
}
DWORD CMSHFlexGridInterface::GetGridColorUnpopulated()
{
	return RGB(0,0,0);
}
VOID CMSHFlexGridInterface::SetGridColorUnpopulated(DWORD newValue)
{
}
DWORD CMSHFlexGridInterface::GetGridColorBand(long BandNumber)
{
	return RGB(0,0,0);
}
VOID CMSHFlexGridInterface::SetGridColorBand(long BandNumber, DWORD newValue)
{
}
DWORD CMSHFlexGridInterface::GetGridColorHeader(long BandNumber)
{
	return RGB(0,0,0);
}
VOID CMSHFlexGridInterface::SetGridColorHeader(long BandNumber, DWORD newValue)
{
}
DWORD CMSHFlexGridInterface::GetGridColorIndent(long BandNumber)
{
	return RGB(0,0,0);
}
VOID CMSHFlexGridInterface::SetGridColorIndent(long BandNumber, DWORD newValue)
{
}
DWORD CMSHFlexGridInterface::GetCellBackColor()
{
	GV_ITEM Item;
	Item.row = GetRow();
	Item.col = GetCol();
	Item.mask = GVIF_BKCLR;

	CGridCtrl::GetItem(&Item);
	
	return Item.crBkClr;
}
VOID CMSHFlexGridInterface::SetCellBackColor(DWORD newValue)
{	
	GV_ITEM Item;
	Item.row = GetRow();
	Item.col = GetCol();
	Item.mask = GVIF_BKCLR;
	Item.crBkClr = newValue;

	CGridCtrl::SetItem(&Item);
}
DWORD CMSHFlexGridInterface::GetCellForeColor()
{
	GV_ITEM Item;
	Item.row = GetRow();
	Item.col = GetCol();
	Item.mask = GVIF_FGCLR;

	CGridCtrl::GetItem(&Item);
	
	return Item.crFgClr;
}
VOID CMSHFlexGridInterface::SetCellForeColor(DWORD newValue)
{
	GV_ITEM Item;
	Item.row = GetRow();
	Item.col = GetCol();
	Item.mask = GVIF_FGCLR;
	Item.crFgClr = newValue;

	CGridCtrl::SetItem(&Item);
}
short CMSHFlexGridInterface::GetCellAlignment()
{
	UINT uFormat = CGridCtrl::GetItemFormat(GetRow(), GetCol());
	return GetAlignmentToFormat(uFormat);
}
VOID CMSHFlexGridInterface::SetCellAlignment(short nNewValue)
{
	UINT uFormat = CGridCtrl::GetItemFormat(GetRow(), GetCol());

	uFormat = GetDeleteAlignmentFormat(uFormat);
	uFormat |= GetFormatToAlignment(nNewValue);

	CGridCtrl::SetItemFormat(GetRow(), GetCol(), uFormat);
}
long CMSHFlexGridInterface::GetCellTextStyle()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetCellTextStyle(long nNewValue)
{
}
short CMSHFlexGridInterface::GetCellPictureAlignment()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetCellPictureAlignment(short nNewValue)
{
}
CString CMSHFlexGridInterface::GetClip()
{
	return _T("");
}
VOID CMSHFlexGridInterface::SetClip(LPCTSTR lpszNewValue)
{
}
VOID CMSHFlexGridInterface::SetSort(short nNewValue)
{
}
long CMSHFlexGridInterface::GetSelectionMode()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetSelectionMode(long nNewValue)
{
}
long CMSHFlexGridInterface::GetMergeCells()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetMergeCells(long nNewValue)
{
}
BOOL CMSHFlexGridInterface::GetAllowBigSelection()
{
		return FALSE;
}
VOID CMSHFlexGridInterface::SetAllowBigSelection(BOOL bNewValue)
{
}
long CMSHFlexGridInterface::GetAllowUserResizing()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetAllowUserResizing(long nNewValue)
{
}
long CMSHFlexGridInterface::GetBorderStyle()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetBorderStyle(long nNewValue)
{
}
long CMSHFlexGridInterface::GetHWnd()
{
	return 0;
}
BOOL CMSHFlexGridInterface::GetEnabled()
{
	return FALSE;
}
VOID CMSHFlexGridInterface::SetEnabled(BOOL bNewValue)
{
}
long CMSHFlexGridInterface::GetAppearance()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetAppearance(long nNewValue)
{
}
long CMSHFlexGridInterface::GetMousePointer()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetMousePointer(long nNewValue)
{
}
COleDispatchDriver CMSHFlexGridInterface::GetMouseIcon()
{
	return NULL;
}
VOID CMSHFlexGridInterface::SetRefMouseIcon(LPDISPATCH newValue)
{
}
long CMSHFlexGridInterface::GetPictureType()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetPictureType(long nNewValue)
{
}
COleDispatchDriver CMSHFlexGridInterface::GetPicture()
{
	return NULL;
}
COleDispatchDriver CMSHFlexGridInterface::GetCellPicture()
{
	return NULL;
}
VOID CMSHFlexGridInterface::SetRefCellPicture(LPDISPATCH newValue)
{
}
CString CMSHFlexGridInterface::GetTextArray(long Index)
{
	return _T("");
}
VOID CMSHFlexGridInterface::SetTextArray(long Index, LPCTSTR lpszNewValue)
{
}
short CMSHFlexGridInterface::GetColAlignment(long Index)
{
	if(0 > Index || Index >= GetCols(0))
	{
		return 0;
	}

	UINT uFormat = m_arColFormat[Index];
	return GetAlignmentToFormat(uFormat);
}
VOID CMSHFlexGridInterface::SetColAlignment(long Index, short nNewValue)
{
	if(0 > Index || Index >= GetCols(0))
	{
		return;
	}

	UINT uFormat = m_arColFormat[Index];
	uFormat = 0;
	uFormat = GetDeleteAlignmentFormat(m_cellDefault.GetFormat());
	uFormat |= GetFormatToAlignment(nNewValue);

	m_arColFormat[Index] = uFormat;


	LONG lMaxRow = GetRows();
	for(LONG lRow = 1; lRow < lMaxRow; lRow++)
	{
		CGridCellBase* pCell = GetCell(lRow, Index);
		if(pCell)
		{
			pCell->SetFormat(m_arColFormat[Index]);						
		}
	}	
	CGridCtrl::RedrawColumn(Index);
}
short CMSHFlexGridInterface::GetColAlignmentFixed(long Index)
{	
	if(0 > Index || Index >= GetCols(0))
	{
		return 0;
	}

	UINT uFormat = GetItemFormat(0, Index);
	return GetAlignmentToFormat(uFormat);
}
VOID CMSHFlexGridInterface::SetColAlignmentFixed(long Index, short nNewValue)
{
	if(0 > Index || Index >= GetCols(0))
	{
		return;
	}

	UINT uFormat = GetItemFormat(0, Index);
	uFormat = GetDeleteAlignmentFormat(uFormat);
	uFormat |= GetFormatToAlignment(nNewValue);

	CGridCtrl::SetItemFormat(0, Index, uFormat);
}
short CMSHFlexGridInterface::GetColAlignmentBand(long BandNumber, long BandColIndex)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetColAlignmentBand(long BandNumber, long BandColIndex, short nNewValue)
{
}
short CMSHFlexGridInterface::GetColAlignmentHeader(long BandNumber, long BandColIndex)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetColAlignmentHeader(long BandNumber, long BandColIndex, short nNewValue)
{
}
long CMSHFlexGridInterface::GetColWidth(long Index, long BandNumber)
{
	return CGridCtrl::GetColumnWidth(Index);
}
VOID CMSHFlexGridInterface::SetColWidth(long Index, long BandNumber, long nNewValue)
{
	if(-1 == Index)
	{
		CGridCtrl::SetDefCellWidth(nNewValue/15);
	}
	else
	{
		CGridCtrl::SetColumnWidth(Index, nNewValue/15);
	}
}
long CMSHFlexGridInterface::GetRowHeight(long Index)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetRowHeight(long Index, long nNewValue)
{
	if(-1 == Index)
	{
		CGridCtrl::SetDefCellHeight(nNewValue/15);
		CGridCtrl::Invalidate();
	}
	else
	{
		CGridCtrl::SetRowHeight(Index, nNewValue/15);
	}
}
BOOL CMSHFlexGridInterface::GetMergeRow(long Index)
{
	return FALSE;
}
VOID CMSHFlexGridInterface::SetMergeRow(long Index, BOOL bNewValue)
{
}
BOOL CMSHFlexGridInterface::GetMergeCol(long Index)
{
	return FALSE;
}
VOID CMSHFlexGridInterface::SetMergeCol(long Index, BOOL bNewValue)
{
}
VOID CMSHFlexGridInterface::SetRowPosition(long Index, long nNewValue)
{
}
VOID CMSHFlexGridInterface::SetColPosition(long Index, long BandNumber, long nNewValue)
{
}
long CMSHFlexGridInterface::GetRowData(long Index)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetRowData(long Index, long nNewValue)
{
}
long CMSHFlexGridInterface::GetColData(long Index)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetColData(long Index, long nNewValue)
{
	
}
CString CMSHFlexGridInterface::GetTextMatrix(long Row, long Col)
{
	return CGridCtrl::GetItemText(Row, Col);
}
VOID CMSHFlexGridInterface::SetTextMatrix(long Row, long Col, LPCTSTR lpszNewValue)
{
	CGridCtrl::SetItemText(Row, Col, lpszNewValue);
	InvalidateCellRect(Row, Col);
}
VOID CMSHFlexGridInterface::AddItem(LPCTSTR Item, const VARIANT& Index)
{
}
VOID CMSHFlexGridInterface::RemoveItem(long Index)
{

}
VOID CMSHFlexGridInterface::Clear()
{
	
}
VOID CMSHFlexGridInterface::Refresh()
{
}
VOID CMSHFlexGridInterface::ClearStructure()
{
}
LPUNKNOWN CMSHFlexGridInterface::GetDataSource()
{
	return NULL;
}
VOID CMSHFlexGridInterface::SetRefDataSource(LPUNKNOWN newValue)
{
}
CString CMSHFlexGridInterface::GetDataMember()
{
	return _T("");
}
VOID CMSHFlexGridInterface::SetDataMember(LPCTSTR lpszNewValue)
{
}
BOOL CMSHFlexGridInterface::GetRowIsVisible(long Index)
{
	return FALSE;
}
BOOL CMSHFlexGridInterface::GetColIsVisible(long Index)
{
	return FALSE;
}
long CMSHFlexGridInterface::GetRowPos(long Index)
{
	return 0;
}
long CMSHFlexGridInterface::GetColPos(long Index)
{
	return 0;
}
short CMSHFlexGridInterface::GetGridLineWidth()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetGridLineWidth(short nNewValue)
{
}
short CMSHFlexGridInterface::GetGridLineWidthFixed()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetGridLineWidthFixed(short nNewValue)
{
}
short CMSHFlexGridInterface::GetGridLineWidthUnpopulated()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetGridLineWidthUnpopulated(short nNewValue)
{
}
short CMSHFlexGridInterface::GetGridLineWidthBand(long BandNumber)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetGridLineWidthBand(long BandNumber, short nNewValue)
{
}
short CMSHFlexGridInterface::GetGridLineWidthHeader(long BandNumber)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetGridLineWidthHeader(long BandNumber, short nNewValue)
{
}
short CMSHFlexGridInterface::GetGridLineWidthIndent(long BandNumber)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetGridLineWidthIndent(long BandNumber, short nNewValue)
{
}
BOOL CMSHFlexGridInterface::GetRightToLeft()
{
	return FALSE;
}
VOID CMSHFlexGridInterface::SetRightToLeft(BOOL bNewValue)
{
}
CRecordset1 CMSHFlexGridInterface::GetRecordset()
{
	return NULL;
}
VOID CMSHFlexGridInterface::SetRefRecordset(LPDISPATCH newValue)
{
}
VARIANT CMSHFlexGridInterface::GetRowset()
{
	tagVARIANT VARIANT = {0};
	return VARIANT;
}
VOID CMSHFlexGridInterface::SetRefRowset(const VARIANT& newValue)
{
}
long CMSHFlexGridInterface::GetColHeader(long BandNumber)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetColHeader(long BandNumber, long nNewValue)
{
}
CString CMSHFlexGridInterface::GetColHeaderCaption(long BandNumber, long BandColIndex)
{
	return _T("");
}
VOID CMSHFlexGridInterface::SetColHeaderCaption(long BandNumber, long BandColIndex, LPCTSTR lpszNewValue)
{
}
long BandColIndex()
{
	return 0;
}
long CMSHFlexGridInterface::GetBandData(long BandData)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetBandData(long BandData, long nNewValue)
{
}
long CMSHFlexGridInterface::GetBandDisplay()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetBandDisplay(long nNewValue)
{
}
BOOL CMSHFlexGridInterface::GetBandExpandable(long BandNumber)
{
	return FALSE;
}
VOID CMSHFlexGridInterface::SetBandExpandable(long BandNumber, BOOL bNewValue)
{
}
long CMSHFlexGridInterface::GetGridLinesBand(long BandNumber)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetGridLinesBand(long BandNumber, long nNewValue)
{
}
long CMSHFlexGridInterface::GetGridLinesHeader(long BandNumber)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetGridLinesHeader(long BandNumber, long nNewValue)
{
}
long CMSHFlexGridInterface::GetGridLinesIndent(long BandNumber)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetGridLinesIndent(long BandNumber, long nNewValue)
{
}
long CMSHFlexGridInterface::GetGridLinesUnpopulated()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetGridLinesUnpopulated(long nNewValue)
{
}
long CMSHFlexGridInterface::GetBandIndent(long BandNumber)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetBandIndent(long BandNumber, long nNewValue)
{
}
long CMSHFlexGridInterface::GetBandLevel()
{
	return 0;
}
long CMSHFlexGridInterface::GetBands()
{
	return 0;
}
long CMSHFlexGridInterface::GetCellType()
{
	return 0;
}
long CMSHFlexGridInterface::GetRowSizingMode()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetRowSizingMode(long nNewValue)
{
}
CString CMSHFlexGridInterface::GetDataField(long BandNumber, long BandColIndex)
{
	return _T("");
}
BOOL CMSHFlexGridInterface::GetRowExpandable()
{
	return FALSE;
}
BOOL CMSHFlexGridInterface::GetRowExpanded()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetRowExpanded(BOOL bNewValue)
{
}
VOID CollapseAll(long BandNumber)
{
}
VOID CMSHFlexGridInterface::ExpandAll(long BandNumber)
{
}
short CMSHFlexGridInterface::GetColWordWrapOption(long Index)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetColWordWrapOption(long Index, short nNewValue)
{
}
short CMSHFlexGridInterface::GetColWordWrapOptionBand(long BandNumber, long BandColIndex)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetColWordWrapOptionBand(long BandNumber, long BandColIndex, short nNewValue)
{
}
short CMSHFlexGridInterface::GetColWordWrapOptionHeader(long BandNumber, long BandColIndex)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetColWordWrapOptionHeader(long BandNumber, long BandColIndex, short nNewValue)
{
}
short CMSHFlexGridInterface::GetColWordWrapOptionFixed(long Index)
{
	return 0;
}
VOID CMSHFlexGridInterface::SetColWordWrapOptionFixed(long Index, short nNewValue)
{
}
long CMSHFlexGridInterface::GetOLEDropMode()
{
	return 0;
}
VOID CMSHFlexGridInterface::SetOLEDropMode(long nNewValue)
{
}
VOID CMSHFlexGridInterface::OLEDrag()
{
}


BEGIN_MESSAGE_MAP(CMSHFlexGridInterface, CGridCtrl)
//{{AFX_MSG_MAP(CMSHFlexGridInterface)
ON_WM_LBUTTONDBLCLK()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_RBUTTONDOWN()
ON_WM_MOUSEWHEEL()
ON_WM_KEYDOWN()
ON_WM_TIMER()
ON_WM_MOUSEMOVE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CMSHFlexGridInterface::OnLButtonDown(UINT nFlags, CPoint point)
{
	CGridCtrl::OnLButtonDown(nFlags, point);
	
	if(m_LeftClickDownCell.col < m_nFixedCols)
	{
		m_LeftClickDownCell.col = m_nFixedCols;
	}
	if(m_LeftClickDownCell.row < m_nFixedRows)
	{
		m_LeftClickDownCell.row = m_nFixedCols;
	}

	SetCol(m_LeftClickDownCell.col);
	SetRow(m_LeftClickDownCell.row);
}

void CMSHFlexGridInterface::OnLButtonUp(UINT nFlags, CPoint point)
{
 	UINT uBakState = 0, uState = 0;

	if(FALSE == m_bIsEditSel && m_idCurrentCell.row >= 0 && m_idCurrentCell.col >= 0)
	{
		uState = uBakState = GetItemState(m_idCurrentCell.row, m_idCurrentCell.col);
		uState |= GVNI_READONLY;
		GetItemState(m_idCurrentCell.row, m_idCurrentCell.col);
 		SetItemState(m_idCurrentCell.row, m_idCurrentCell.col, uState);
	}

	CGridCtrl::OnLButtonUp(nFlags, point);

	if(FALSE == m_bIsEditSel && m_idCurrentCell.row >= 0 && m_idCurrentCell.col >= 0)
	{
		SetItemState(m_idCurrentCell.row, m_idCurrentCell.col, uBakState);
	}
}

void CMSHFlexGridInterface::OnRButtonDown(UINT nFlags, CPoint point)
{
	CGridCtrl::OnRButtonDown(nFlags, point);
}

BOOL CMSHFlexGridInterface::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return CGridCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CMSHFlexGridInterface::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(m_bIsEditSel)
	{
		CGridCtrl::OnLButtonDblClk(nFlags, point);
	}
}

void CMSHFlexGridInterface::OnMouseMove(UINT nFlags, CPoint point)
{
	CGridCtrl::OnMouseMove(nFlags, point);
}

void CMSHFlexGridInterface::OnTimer(UINT nIDEvent)
{
	CGridCtrl::OnTimer(nIDEvent);
}

void CMSHFlexGridInterface::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CGridCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMSHFlexGridInterface::SetDefaultColor(BOOL bIsMainGrid)
{
	if(bIsMainGrid)
	{
		CGridCtrl::SetFixedTextColor(RGB(0xFF, 0xFF, 0xFF));
		CGridCtrl::SetFixedBkColor(RGB(0x63, 0xAB, 0xE7));

		CGridCtrl::SetTextColor(RGB(0x00, 0x00, 0x00));
		CGridCtrl::SetBkColor(RGB(0xEF, 0xEF, 0xEF));

		SetGridBkColor(RGB(0xEF, 0xEF, 0xEF));
	}	
	else
	{
		CGridCtrl::SetFixedTextColor(RGB(0x24, 0x24, 0x24));
		CGridCtrl::SetFixedBkColor(RGB(0xE1, 0xE1, 0xE1));

		CGridCtrl::SetTextColor(RGB(0x00, 0x00, 0x00));
		CGridCtrl::SetBkColor(RGB(0xEF, 0xEF, 0xEF));

		SetGridBkColor(RGB(0xEF, 0xEF, 0xEF));
	}
}


UINT CMSHFlexGridInterface::GetFormatToAlignment(UINT nAlignment)
{	
	UINT uFormat = 0;

	INT n1 = 0, n2 = 0;
	n1 = nAlignment / 3;
	n2 = nAlignment % 3;
	
	if(1 == n1)
	{
		uFormat |= DT_CENTER;
	}
	else if(2 == n1)
	{
		uFormat |= DT_RIGHT;
	}

	if(1 == n2)
	{
		uFormat |= DT_VCENTER;
	}
	else if(2 == n2)
	{
		uFormat |= DT_BOTTOM;
	}

	return uFormat;
}
UINT CMSHFlexGridInterface::GetAlignmentToFormat(UINT uFormat)
{
	UINT uAlign = 0;
	if(DT_CENTER == (uFormat & DT_CENTER))
	{
		uAlign += 3;
	}
	else if(DT_RIGHT == (uFormat & DT_RIGHT))
	{
		uAlign += 6;
	}

	if(DT_VCENTER == (uFormat & DT_VCENTER))
	{
		uAlign += 1;
	}
	else if(DT_BOTTOM == (uFormat & DT_BOTTOM))
	{
		uAlign += 2;
	}

	return uAlign;
}

UINT CMSHFlexGridInterface::GetDeleteAlignmentFormat(UINT uFormat)
{
//	uFormat &= (DT_WORDBREAK|DT_SINGLELINE|DT_EXPANDTABS|DT_TABSTOP|DT_NOCLIP|DT_EXTERNALLEADING|DT_CALCRECT|DT_NOPREFIX|DT_INTERNAL|DT_EDITCONTROL|DT_PATH_ELLIPSIS|DT_END_ELLIPSIS|DT_MODIFYSTRING|DT_RTLREADING|DT_WORD_ELLIPSIS);
	uFormat &= ~(DT_LEFT|DT_CENTER|DT_RIGHT|DT_TOP|DT_VCENTER|DT_BOTTOM);
	return uFormat;
}
