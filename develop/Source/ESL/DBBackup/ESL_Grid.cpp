// ESL_Grid.cpp : implementation file
//

#include "stdafx.h"
#include "ESL_Grid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CESL_Grid

CESL_Grid::CESL_Grid()
{
	m_cSelectType = 'S';
	m_sSelectString = _T("V");
	m_nSelectIdx = 0;		
	m_bGridEdit = false;		
	m_bMenu = false;			
	m_bSort = false;			
	m_bCheck = false;			

	m_arraySelect.RemoveAll();

	m_nReverse = -1;
}

CESL_Grid::~CESL_Grid()
{
	m_arraySelect.RemoveAll();
}


BEGIN_MESSAGE_MAP(CESL_Grid, CMSHFlexGrid)
	//{{AFX_MSG_MAP(CESL_Grid)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(4000, 4003, UserSelectProc)
END_MESSAGE_MAP()


INT CESL_Grid::InitColumn(INT col)
{
	if ( col < 1 ) return -1;
	RemoveAll();
	SetCols(0, col+1);
	SetColWidth(0, 0, 500);
	SetColAlignment(0, 4);

	return 0;
}

INT CESL_Grid::SetColumnInfo(INT col, CString str, INT width, INT alignment)
{
	if ( col > GetCols(0)-2 ) return -1;

	SetColAlignmentFixed(col+1, 4);
	SetTextMatrix(0, col+1, str);
	SetColWidth(col+1, 0, width*15);
	SetColAlignment(col+1, alignment);
	
	return 0;
}

INT CESL_Grid::GetColumnInfo(INT col, CString& str, INT& width, INT& alignment)
{
	if ( col < 0 ) return -1;
	if ( col > GetCols(0)-2 ) return -1;

	str = GetTextMatrix(0, col+1);
	width = GetColWidth(col+1, 0)/15;
	alignment = GetColAlignment(col+1);
	
	return 0;
}

INT CESL_Grid::SetRowInfo(INT row, CString str)
{
	if ( row < 0 ) return -1;

	INT rows;
	rows = GetRows();
	if ( row >= rows-1 ) {
		SetRows(row+2);
	}

	SetTextMatrix(row+1, 0, str);
	
	return 0;
}

INT CESL_Grid::GetRowInfo(INT row, CString& str)
{
	if ( row < 0 ) return -1;
	if ( row > GetRows()-2 ) return -1;

	str = GetTextMatrix(row+1, 0);
	
	return 0;
}

INT CESL_Grid::SetSelectionInfo(TCHAR type, CString str, INT idx)
{
	m_cSelectType = type;
	m_sSelectString = str;
	m_nSelectIdx = idx;

	return 0;
}

INT CESL_Grid::RemoveAll()
{
	Clear();
	SetRows(2);
	return 0;
}

INT CESL_Grid::GetCount()
{
	INT cnt;
	cnt = GetRows();
	cnt--;
	if (cnt<0) cnt = -1;
	return cnt;
}

INT CESL_Grid::GetColCount()
{
	INT cnt;
	cnt = GetCols(0);
	cnt--;
	if (cnt<0) cnt = -1;
	return cnt;
}

INT CESL_Grid::GetAt(INT row, INT col, CString& str)
{
	if ( row < 0 || col < 0 ) return -1;
	if ( row > GetRows() - 2 || col > GetCols(0) - 2 ) return -1;

	str = GetTextMatrix(row+1, col+1);
	return 0;
}

CString CESL_Grid::GetAt(INT row, INT col)
{
	CString str;
	
	INT nError;
	nError = GetAt(row, col, str);
	if ( nError < 0 ) {
		AfxMessageBox(_T("GetAt Error"));
		return _T("");
	}

	return str;
}

INT CESL_Grid::SetAt(INT row, INT col, CString str)
{
	if ( row < 0 || col < 0 ) return -1;
	if ( col >= GetCols(0) - 1 ) return -1;

	INT rows;
	rows = GetRows();
	if ( row >= rows-1 ) {
		SetRows(row+2);
	}

	SetTextMatrix(row+1, col+1, str);

	return 0;
}

INT CESL_Grid::RemoveAt(INT row)
{
	if ( row < 0 ) return -1;
	RemoveItem(row+1);
	return 0;
}

INT CESL_Grid::SelectRemoveAll()
{
	m_arraySelect.RemoveAll();
	return 0;
}

INT CESL_Grid::SelectMakeList()
{
	INT i;

	SelectRemoveAll();

	INT cnt;
	cnt = GetCount();

	for ( i = 0 ; i < cnt ; i++ ) {

		CString str;

		switch(m_cSelectType) {
		case 'N':
			m_arraySelect.AddTail(i);
			break;

		case 'C':
			break;

		case 'S':
			GetAt(i, m_nSelectIdx, str);
			if ( str == m_sSelectString ) m_arraySelect.AddTail(i);
			break;

		default:
			break;
		}
	}


	if ( m_arraySelect.GetCount() > 0 ) {
		m_arrayPos = m_arraySelect.GetHeadPosition();
	} else {
		m_arrayPos = NULL;
	}

	return 0;
}

INT CESL_Grid::SelectGetCount()
{
	return m_arraySelect.GetCount();
}

INT CESL_Grid::SelectGetHeadPosition()
{
	if ( m_arraySelect.GetCount() == 0 ) return -1;

	m_arrayPos = m_arraySelect.GetHeadPosition();

	return m_arraySelect.GetAt(m_arrayPos);
}

INT CESL_Grid::SelectGetTailPosition()
{
	if ( m_arraySelect.GetCount() == 0 ) return -1;

	m_arrayPos = m_arraySelect.GetTailPosition();

	return m_arraySelect.GetAt(m_arrayPos);
}

INT CESL_Grid::SelectGetNext()
{
	if ( m_arraySelect.GetCount() == 0 ) return -1;

	m_arraySelect.GetNext(m_arrayPos);
	if ( m_arrayPos == NULL ) return -1;

	INT sel;
	sel = m_arraySelect.GetAt(m_arrayPos);

	return sel;
}

INT CESL_Grid::SelectGetPrev()
{
	if ( m_arraySelect.GetCount() == 0 ) return -1;

	m_arraySelect.GetPrev(m_arrayPos);
	if ( m_arrayPos == NULL ) return -1;

	INT sel;
	sel = m_arraySelect.GetAt(m_arrayPos);

	return sel;
}

INT CESL_Grid::SelectGetIdx()
{
	if ( m_arraySelect.GetCount() == 0 ) return -1;

	if ( m_arrayPos == NULL ) return -1;

	return m_arraySelect.GetAt(m_arrayPos);
}

INT CESL_Grid::SetReverse(INT row)
{
	INT i;

	COLORREF blue, white, black;
	//blue = RGB(43,79,119);			
	blue = RGB(228,243,251);			//E4F3FB
	white = RGB(255,255,255);
	black = RGB(0,0,0);

	INT cols;
	cols = GetCols(0);

	if ( m_nReverse >= 0 && m_nReverse+1 < GetRows()) {
		SetRow(m_nReverse+1);
		for ( i = 1 ; i < cols ; i++ ) {
			SetCol(i);
			SetCellBackColor(white);
			SetCellForeColor(black);
		}
	}

	if ( row >= 0 ) {

		SetRow(row+1);
		for ( i = 1 ; i < cols ; i++ ) {
			SetCol(i);
			SetCellBackColor(blue);
			//SetCellForeColor(white);
			SetCellForeColor(black);
		}
		m_nReverse = row;
	}

	return 0;
}

INT CESL_Grid::DisplayNumber()
{
	INT i;
	INT rows = GetRows();

	for ( i = 1 ; i < rows ; i++ ) {
		CString str;
		str.Format(_T("%d"), i);
		SetTextMatrix(i, 0, str);
	}

	return 0;
}

INT CESL_Grid::GetIdx()
{
	return GetRow()-1;
}

INT CESL_Grid::PostChoiceMessage()
{
    this->GetParent()->PostMessage(ESL_GRID_CHOICE, (DWORD)this);
    return 0;
}

INT CESL_Grid::SetCheck(INT row, bool check)
{
	if ( row < 0 ) return -1;
	if ( m_cSelectType == 'N' ) return -1;


	if ( check ) {

		switch(m_cSelectType) {
		case 'C':
			break;

		case 'S':
			SetAt(row, m_nSelectIdx, m_sSelectString);
			break;

		default:
			break;
		}

	} else {

		SetAt(row, m_nSelectIdx, _T(""));
	}

	return 0;
}

INT CESL_Grid::GetCheck(INT row, bool &check)
{
	if ( row < 0 ) return false;
	if ( m_cSelectType == 'N' ) return -1;

	check = false;

	CString str;
	switch(m_cSelectType) {
	case 'C':
		break;

	case 'S':
		GetAt(row, m_nSelectIdx, str);
		if ( str == m_sSelectString ) check = true;
		break;

	default:
		break;
	}

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CESL_Grid message handlers

BOOL CESL_Grid::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_RETURN ) {
			TRACE(_T("CESL_Grid::PreTranslateMessage(VK_RETURN)\n"));
			return true;
		} else if ( pMsg->wParam == VK_ESCAPE ) {
			TRACE(_T("CESL_Grid::PreTranslateMessage(VK_ESCAPE)\n"));
			return true;
		}
	}
	
	return CMSHFlexGrid::PreTranslateMessage(pMsg);
}

VOID CESL_Grid::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( !m_bMenu ) return;

    CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, 4000, _T("선정"));
	menu.AppendMenu(MF_STRING, 4001, _T("비선정"));
	menu.AppendMenu(MF_STRING, 4002, _T("전체선정"));
	menu.AppendMenu(MF_STRING, 4003, _T("전체비선정"));

    CRect rect;
	GetWindowRect(&rect);

	menu.TrackPopupMenu(TPM_LEFTALIGN, rect.left + point.x, rect.top + point.y, this);

	CMSHFlexGrid::OnRButtonDown(nFlags, point);
}


VOID CESL_Grid::UserSelectProc(UINT  nIDC)
{
	if ( m_cSelectType == 'N' ) return;

	INT i;

	INT top, bottom;
	top = GetRow()-1;
	bottom = GetRowSel()-1;

	TRACE(_T("top:%d bottom:%d\n"), top, bottom);

	INT tmp;
	if ( top < bottom ) {
		tmp = bottom;
		bottom = top;
		top = tmp;
	}

	INT max;
	max = GetRows() - 1;

	switch (nIDC) {
	case 4000:
		for ( i = bottom ; i <= top ; i++ ) 
			SetCheck(i, true);
		break;

	case 4001:
		for ( i = bottom ; i <= top ; i++ ) 
			SetCheck(i, false);
		break;

	case 4002:
		for ( i = 0 ; i < max ; i++ ) 
			SetCheck(i, true);
		break;

	case 4003:
		for ( i = 0 ; i < max ; i++ ) 
			SetCheck(i, false);
		break;

	default:
		break;
	}
    PostChoiceMessage();
}

VOID CESL_Grid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMSHFlexGrid::OnLButtonDown(nFlags, point);

	if ( !m_bCheck ) return;
	if ( m_cSelectType == 'N' ) return;

	INT col;
	col = GetCol();
	if ( m_nSelectIdx != col-1 ) return;

	bool check;
	GetCheck(GetRow()-1, check);
	if ( check ) 
		SetCheck(GetRow()-1, false);
	else 
		SetCheck(GetRow()-1, true);
    PostChoiceMessage();

}

BOOL CESL_Grid::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	INT rtn;
	rtn = CMSHFlexGrid::OnMouseWheel(nFlags, zDelta, pt);

	INT row = GetRow();
	INT max = GetRows();

	if ( zDelta > 0 ) {
		// up
		if ( row <= 1 ) return rtn;
		SetRow(row-1);
		if ( GetTopRow() > 1 ) SetTopRow(GetTopRow()-1);

	} else {
		// down
		if ( row >= max - 1 ) return rtn;
		SetRow(row+1);
		SetTopRow(GetTopRow()+1);
	}

	return rtn;
}

