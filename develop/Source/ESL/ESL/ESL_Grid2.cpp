// ESL_Grid2.cpp : implementation file
//

#include "stdafx.h"
#include "ESL_Grid2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ESL_Mgr.h"
#include "ESL_GridData.h"
#include "ESL_UnicodeFileIO.h"
#include "ESL_MBCSFileIO.h"
#include "ESLDlg_GridConfig.h"
#include "..\KOLAS3_MAIN\KOLAS3_MAINView.h"
#include "..\KOLAS3_MAIN\RCView.h"

#include "efs.h"
#include "ESLDlg_ExcelExportWait.h"
#include "XLEzAutomation.h"

UINT GridDisplayThread2(LPVOID pParam)
{
	EFS_BEGIN
		TRACE (_T("(Start) - Grid2 display Therad \n"));
	DWORD START, FINISH;
	START = GetTickCount();
	CESL_Grid2 *pGrid = (CESL_Grid2*)pParam;
	CESL_GridData *pData = (CESL_GridData *)pGrid->m_pData;
	CESL_GridColumnInfo *parrColumnInfo[64];
	CESL_Mgr *pMgr = pData->pMgr;
	
	pData->m_Status = THREAD_RUNNING;
	if (pData->m_pArrColumnInfo == NULL) {
		pData->m_Status = THREAD_NOTRUN;
		SetEvent(pGrid->m_hThread);
		return 0;
	}
	INT nColumns = pData->m_pArrColumnInfo->GetCount();
	if (nColumns == 0) {
		pData->m_Status = THREAD_NOTRUN;
		SetEvent(pGrid->m_hThread);
		return 0;
	}
	CString strDMAlias = _T("");
	INT ids, i, j;
	POSITION pos;
	for (i = 0; i < nColumns; i++) {
		pos = pData->m_pArrColumnInfo->FindIndex(i);
		if (pos == NULL) continue;
		parrColumnInfo[i] = (CESL_GridColumnInfo*)pData->m_pArrColumnInfo->GetAt(pos);
		if (strDMAlias.GetLength() == 0) strDMAlias = parrColumnInfo[i]->strColumnDMAlias;	
	}
	if (strDMAlias.GetLength() == 0) {
		pData->m_Status = THREAD_NOTRUN;
		SetEvent(pGrid->m_hThread);
		return 0;
	}
	CESL_DataMgr *pDM = pData->pMgr->FindDM(strDMAlias);
	if (pDM == NULL) {
		pData->m_Status = THREAD_NOTRUN;
		SetEvent(pGrid->m_hThread);
		return 0;
	}
	INT nRows = pDM->GetRowCount();
	if (pData->m_pRealRowCount) *pData->m_pRealRowCount = nRows;
	if (nRows == 0) ((CESL_Grid2*)pData->pGrid2)->SetRows(2);
	else ((CESL_Grid2*)pData->pGrid2)->SetRows(nRows+1);
	
	CString strData;
	DWORD *pRow;
	pos = pDM->data.GetHeadPosition();
	for (i = 0; i < nRows; i++) {
		if (pos == NULL) break;
		pRow  = (DWORD*)pDM->data.GetNext(pos);
		if (pRow == NULL) continue;
		strData.Format(_T("%d"), i+1);
		((CESL_Grid2*)pData->pGrid2)->SetTextMatrix(i+1, 0, strData);
		for (j = 0; j < nColumns; j++) {
			if (parrColumnInfo[j]->nColumnPos < 0) continue;
			strData = _T("");
			if (parrColumnInfo[j]->nColumnPos == 1000000) {
				ids = pDM->GetCellData(parrColumnInfo[j]->strColumnDMFieldAlias, i, strData);
			} else {
				strData = pDM->GetCellData(pRow, parrColumnInfo[j]->nColumnPos);
				ids = 0;
			}
			if (ids <0) continue;
			pData->GridDisplayEventProc(i, j, parrColumnInfo[j]->strColumnDMFieldAlias, strData);
			((CESL_Grid2*)pData->pGrid2)->SetTextMatrix(i+1, j+1, strData);
		}		
	}
	
	
	pData->m_Status = THREAD_NOTRUN;
	FINISH = GetTickCount();
	TRACE(_T("(Finish) - Grid2 display Therad [%d.%d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);
	
	SetEvent(pGrid->m_hThread);
	
	return 0;
	EFS_END
		return 0;
}

void CESL_Grid2::DisplayToolTip()
{
			INT nRow = GetMouseRow();
			INT nRowCnt = this->GetRows();

			int nCol;
			nCol = this->GetMouseCol();
			
			CString strFieldData;
			strFieldData = this->GetTextMatrix(nRow,nCol);
			CRect textSize;
			
			CString strFieldHeader;
			INT nLineWidth, alignment;
			
			GetColumnInfo(nCol-1, strFieldHeader, nLineWidth, alignment);		
			
			INT nTextWidth, nMaxLength;
			nTextWidth = 0;
			nMaxLength = 0;
		
			CRect cellRect;
			cellRect.right=300;
			CRect textRect;
			textRect = this->CalculateTextSize(strFieldData,cellRect);
			nTextWidth = textRect.Width();
						
			TOOLINFO ti;
			ti.cbSize = sizeof(TOOLINFO);
			ti.uFlags=TTF_SUBCLASS;
			ti.hwnd = GetSafeHwnd();
			ti.uId=0;
			
			if(nTextWidth + 6 > nLineWidth && nCol != 0 && nRow != 0){	
				ti.lpszText = (LPTSTR)strFieldData.GetBuffer(0);}
			else
				ti.lpszText = _T("");

			if(nRow == nRowCnt-1)
			{
				CScrollBar*  sBar = this->GetScrollBarCtrl(SB_VERT);
				SCROLLINFO sf;
				this->GetScrollInfo(SB_VERT,&sf);
				
				if(sf.nMax==1)
				{
					
					int heightSum=0;
					for(int i=0; i<nRowCnt;++i)
					{
						heightSum += (GetRowHeight(i)/15);
					}
					POINT pt;
					GetCursorPos(&pt);
					ScreenToClient(&pt);
					
					if(pt.y > heightSum)
					{
						ti.lpszText = _T("");
					}
				}
			}
			
			::SendMessage(hTip,TTM_UPDATETIPTEXT,0,(LPARAM)(LPARAM)&ti);
			
			strFieldData.ReleaseBuffer();
}

CESL_Grid2::CESL_Grid2(CESL_Control_Element * ELEMENT)
{
	this->ELEMENT = ELEMENT;
	m_cSelectType = 'S';
	m_sSelectString = _T("V");
	m_nSelectIdx = 0;		
	m_bGridEdit = false;		
	m_bMenu = false;			
	m_bSort = false;			
	m_bCheck = false;	
	m_bPasswordUse = FALSE;
	
	m_arraySelect.RemoveAll();
	
	m_nReverse = -1;
	
	m_pSelEditor = NULL;
	
	CESL_GridData *pData = new CESL_GridData;
	pData->m_Status = THREAD_NOTRUN;
	m_pData = (CObject*)pData;
	
	m_pWorkDlg = NULL;

	InitPopupMenuItem();
	m_pMenu = NULL;
	m_pParentView = NULL;
	
	FreeColumnInfo();
	
	m_fpEventHandler = NULL;
	
	m_strFileName = _T("..\\CFG\\Grid\\_Grid.cfg");
	m_strISBOLD = _T("보통");
	m_nHeight = 17;
	m_nLINECOLOR_R=192;		m_nLINECOLOR_G=192;		m_nLINECOLOR_B=192;
	m_cLINECOLOR = RGB(m_nLINECOLOR_R, m_nLINECOLOR_G, m_nLINECOLOR_B);
	m_nBACKCOLOR_R=255;		m_nBACKCOLOR_G=255;		m_nBACKCOLOR_B=255;
	m_cBACKCOLOR = RGB(m_nBACKCOLOR_R, m_nBACKCOLOR_G, m_nBACKCOLOR_B);
	m_nFontSize = 9;
	m_nFONTCOLOR_R = 100;		m_nFONTCOLOR_G=100;		m_nFONTCOLOR_B=100;
	
	m_bGRIDSORT = TRUE;
	m_nRealRowCount = 0;
	m_bProtectSelectColumn = FALSE;
	
	m_hThread = CreateEvent(NULL, FALSE, FALSE, _T("Thread Event"));
	m_mGridEditMsg.message = -1; 
	
	m_arrSelEditor.RemoveAll();
	
	m_bFroceReverse = TRUE;

	m_bPasswordStatus = TRUE;

	m_bPasswordUse = FALSE;
	
	m_pFindWordDlg = NULL;

	hTip = NULL;

	m_uUseKeyFlag = 0;

	m_bPrivacyPrintLogRecord = FALSE;
	m_strPrintInfo = _T("");
}

CESL_Grid2::~CESL_Grid2()
{
	m_arraySelect.RemoveAll();
	if ( m_pSelEditor != NULL ) {
		delete m_pSelEditor;
		m_pSelEditor = NULL;
	}
	
	CESL_GridData *pData = (CESL_GridData *)m_pData;
	if (pData) {
		delete pData;
		m_pData = NULL;
	}
	
	FreePopupMenuItem();
	
	FreeColumnInfo();
	FreeWorkDlg();
	
	if( m_pFindWordDlg )
	{
		delete m_pFindWordDlg;
		m_pFindWordDlg = NULL;
	}

	CloseHandle(m_hThread);
}


BEGIN_MESSAGE_MAP(CESL_Grid2, CMSHFlexGridInterface)
//{{AFX_MSG_MAP(CESL_Grid2)
ON_WM_LBUTTONDBLCLK()
ON_MESSAGE(ESL_GRID_MODIFY, OnGridSelEditor)
ON_WM_RBUTTONDOWN()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEWHEEL()
ON_WM_KEYDOWN()
ON_WM_TIMER()
ON_WM_MOUSEMOVE()
//}}AFX_MSG_MAP
ON_COMMAND_RANGE(4000, 4005, UserSelectProc)
ON_COMMAND_RANGE(32001, 32500, MainToolBarProc)
ON_MESSAGE(MSG_GRID_CONFIG, OnGridConfig)
ON_MESSAGE(WM_USER+1907, OnExcelExport)
END_MESSAGE_MAP()


INT CESL_Grid2::InitColumn(INT col)
{
	EFS_BEGIN
		
		if ( col < 1 ) return -1;
		RemoveAll();
		SetCols(0, col+1);
		SetColWidth(0, 0, 500);
		SetColAlignment(0, 4);
		
		return 0;
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::SetColumnInfo(INT col, CString str, INT width, INT alignment)
{
	EFS_BEGIN
		
		if ( col > GetCols(0)-2 ) return -1;
		INT nCol = GetCol();
		INT nRow = GetRow();
		
		SetColAlignmentFixed(col+1, 4);
		SetTextMatrix(0, col+1, str);
		SetColWidth(col+1, 0, width*15);
		SetColAlignment(col+1, alignment);
		if (m_strISBOLD == _T("진하게")) {
			SetCol(col+1);
			SetRow(0);
		}
		
		SetCol(nCol);
		SetRow(nRow);
		return 0;
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::GetColumnInfo(INT col, CString& str, INT& width, INT& alignment)
{
	EFS_BEGIN
		
		if ( col < 0 ) return -1;
		if ( col > GetCols(0)-2 ) return -1;
		
		str = GetTextMatrix(0, col+1);
		width = GetColWidth(col+1, 0)/15;
		alignment = GetColAlignment(col+1);
		return 0;
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::SetRowInfo(INT row, CString str)
{
	EFS_BEGIN
		
		if ( row < 0 ) return -1;
		
		INT rows;
		rows = GetRows();
		if ( row >= rows-1 ) {
			SetRows(row+2);
		}
		
		SetTextMatrix(row+1, 0, str);
		
		return 0;
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::GetRowInfo(INT row, CString& str)
{
	EFS_BEGIN
		
		if ( row < 0 ) return -1;
		if ( row > GetRows()-2 ) return -1;
		
		str = GetTextMatrix(row+1, 0);
		
		return 0;
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::SetSelectionInfo(TCHAR type, CString str, INT idx)
{
	EFS_BEGIN


	if(0 <= m_nSelectIdx)
	{		
		CMSHFlexGridInterface::SetCellType(0, m_nSelectIdx+1, RUNTIME_CLASS(CGridCell));
	}
		
	m_cSelectType = type;
	m_sSelectString = str;
	m_nSelectIdx = idx;

	if(0 <= m_nSelectIdx)
	{
		CMSHFlexGridInterface::SetCellType(0, m_nSelectIdx+1, RUNTIME_CLASS(CGridCellCheck));
	}
	
	return 0;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::RemoveAll()
{
	EFS_BEGIN
		
		Clear();
	SetRows(2);
	return 0;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::RemoveAll2()
{
	EFS_BEGIN
		
		SetRows(2);
	INT nCols = GetCols(0);
	for (INT i = 0; i < nCols; i++) {
		SetTextMatrix(1, i, _T(""));
	}
	return 0;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::GetCount()
{
	EFS_BEGIN
		
		INT cnt;
	cnt = GetRows();
	cnt--;
	if (cnt<0) cnt = -1;
	return cnt;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::GetColCount()
{
	EFS_BEGIN
		
		INT cnt;
	cnt = GetCols(0);
	cnt--;
	if (cnt<0) cnt = -1;
	return cnt;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::GetAt(INT row, INT col, CString& str)
{
	EFS_BEGIN
		
		if ( row < 0 || col < 0 ) return -1;
		if ( row > GetRows() - 2 || col > GetCols(0) - 2 ) return -1;
		
		str = GetTextMatrix(row+1, col+1);
		return 0;
		
		EFS_END
			return -1;
		
}

CString CESL_Grid2::GetAt(INT row, INT col)
{
	EFS_BEGIN
		
		CString str;
	
	INT nError;
	nError = GetAt(row, col, str);
	if ( nError < 0 ) {
		MessageBox(_T("GetAt Error"), _T("Warnning!"), MB_ICONEXCLAMATION);
		return _T("");
	}
	
	return str;
	
	EFS_END
		return _T("");
	
}

INT CESL_Grid2::SetAt(INT row, INT col, CString str)
{
	EFS_BEGIN
		
		if ( row < 0 || col < 0 ) return -1;
		if ( col >= GetCols(0) - 1 ) return -1;
		
		INT rows;
		rows = GetRows();
		if ( row >= rows-1 ) {
			SetRows(row+2);
		}
		
		SetTextMatrix(row+1, col+1, str);
		
		return 0;
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::RemoveAt(INT row)
{
	EFS_BEGIN
		
		if ( row < 0 ) return -1;
		RemoveItem(row+1);
		return 0;
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::SelectRemoveAll()
{
	EFS_BEGIN
		
		m_arraySelect.RemoveAll();
	return 0;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::SelectMakeList()
{
	EFS_BEGIN
		
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
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::SelectGetCount()
{
	EFS_BEGIN
		
		return m_arraySelect.GetCount();
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::SelectGetHeadPosition()
{
	EFS_BEGIN
		
		if ( m_arraySelect.GetCount() == 0 ) return -1;
		
		m_arrayPos = m_arraySelect.GetHeadPosition();
		
		return m_arraySelect.GetAt(m_arrayPos);
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::SelectGetTailPosition()
{
	EFS_BEGIN
		
		if ( m_arraySelect.GetCount() == 0 ) return -1;
		
		m_arrayPos = m_arraySelect.GetTailPosition();
		
		return m_arraySelect.GetAt(m_arrayPos);
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::SelectGetNext()
{
	EFS_BEGIN
		
		if ( m_arraySelect.GetCount() == 0 ) return -1;
		if (m_arrayPos == NULL) return -1;
		
		m_arraySelect.GetNext(m_arrayPos);
		if ( m_arrayPos == NULL ) return -1;
		
		INT sel;
		sel = m_arraySelect.GetAt(m_arrayPos);
		
		return sel;
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::SelectGetPrev()
{
	EFS_BEGIN
		
		if ( m_arraySelect.GetCount() == 0 ) return -1;
		if (m_arrayPos == NULL) return -1;
		
		m_arraySelect.GetPrev(m_arrayPos);
		if ( m_arrayPos == NULL ) return -1;
		
		INT sel;
		sel = m_arraySelect.GetAt(m_arrayPos);
		
		return sel;
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::SelectGetIdx()
{
	EFS_BEGIN
		
		if ( m_arraySelect.GetCount() == 0 ) return -1;
		
		if ( m_arrayPos == NULL ) return -1;
		
		return m_arraySelect.GetAt(m_arrayPos);
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::SetReverse(INT row)
{
	EFS_BEGIN
	
	INT i;
	
	COLORREF blue, white, black;
	blue = RGB(228,243,251);
	white = RGB(255,255,255);
	black = RGB(0,0,0);
		
	INT col;
    col = GetCol();
    
	INT cols;
	cols = GetCols(0);

	SetRedraw (FALSE) ;
	
	if ( m_nReverse >= 0 && m_nReverse+1 < GetRows() && m_nReverse != row ) {	
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
			SetCellForeColor(black);
		}
		m_nReverse = row;
	}
	
	SetRedraw (TRUE) ;
	
	SetCol(col);
	
	return 0;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::DisplayNumber()
{
	EFS_BEGIN
		
		INT i;
	INT rows = GetRows();
	
	for ( i = 1 ; i < rows ; i++ ) {
		CString str;
		str.Format(_T("%d"), i);
		SetTextMatrix(i, 0, str);
	}
	
	return 0;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::GetIdx()
{
	EFS_BEGIN
		
		return GetRow()-1;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::GridEdit(INT row, INT col)
{
	EFS_BEGIN
		
		if ( row < 0 || col < 0 ) return -1;
		
		if (SELEDITOR_GetColumnCount() > 0) {
			if (SELEDITOR_FindColumn((UINT)col) < 0) return 0;
		}
		
		if ( m_pSelEditor != NULL ) {
			delete m_pSelEditor;
			m_pSelEditor = NULL;
		}
		
		CString str;
		GetAt(row, col, str);
		
		m_pSelEditor = new CESL_GridSelEditor;
		m_pSelEditor->row = row;
		m_pSelEditor->col = col;
		m_pSelEditor->parent = this;
		
		m_pSelEditor->Create(ES_LEFT|ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD, CRect(0,0,10,20), GetParent(), 645);
		
		CRect rect,gridRect;
		RECT parentRect;
		GetWindowRect(&gridRect);
		GetParent()->GetClientRect(&parentRect);
		
		::ClientToScreen(GetParent()->m_hWnd,(POINT*)&(parentRect.left));
		
		gridRect.top -= parentRect.top;
		gridRect.left -= parentRect.left;
		
		rect.left = gridRect.left + (long)((float)GetCellLeft()/15.0f);
		rect.top = gridRect.top + (long)((float)GetCellTop()/15.0f);
		rect.right = rect.left + (long)((float)GetCellWidth()/15.0f);
		rect.bottom = rect.top + (long)((float)GetCellHeight()/15.0f);
		m_pSelEditor->MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());
		
		
		m_pSelEditor->ShowWindow(TRUE);
		m_pSelEditor->SetFocus();
		m_pSelEditor->SetWindowText(str);
		
		if (m_mGridEditMsg.message > 0) {
			m_pSelEditor->PostMessage(m_mGridEditMsg.message, m_mGridEditMsg.wParam, m_mGridEditMsg.lParam);
			m_mGridEditMsg.message = -1;
		}
		
		return 0;
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::SetCheck(INT row, bool check)
{
	EFS_BEGIN
		
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
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::GetCheck(INT row, bool &check)
{
	EFS_BEGIN
		
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
		
		EFS_END
			return -1;
		
}


/////////////////////////////////////////////////////////////////////////////
// CESL_Grid2 message handlers

VOID CESL_Grid2::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	EFS_BEGIN	
		
	CString strOrigAlias;
	INT		nColumnCheck;
	INT col = GetCol();
	if (GetMouseRow() == 0) {
		if (!ELEMENT) return;
		if (m_bGRIDSORT == TRUE) {
			CESL_Mgr *pMgr = (CESL_Mgr*)ELEMENT->WorkWnd;
			if (!pMgr) return;
			POSITION pos = m_arrColumnInfo.FindIndex(col-1);
			if (pos == NULL) return;
			CESL_GridColumnInfo *pColumnInfo = (CESL_GridColumnInfo*)m_arrColumnInfo.GetAt(pos);
			CESL_DataMgr *pDM = pMgr->FindDM(pColumnInfo->strColumnDMAlias);
			if (!pDM) return;
			
			if (pDM->GetRowCount() == 0) return;

			nColumnCheck = -1;
			strOrigAlias = _T("");
			if ( pColumnInfo->strColumnDMFieldAlias == _T("IBS_표제사항") ||
				pColumnInfo->strColumnDMFieldAlias == _T("ISS_표제사항") ||
				pColumnInfo->strColumnDMFieldAlias == _T("본표제")       ||
				pColumnInfo->strColumnDMFieldAlias == _T("서명")         ||
				pColumnInfo->strColumnDMFieldAlias == _T("IBS_본표제")   ||
				pColumnInfo->strColumnDMFieldAlias == _T("TITLE_INFO")   ||
				pColumnInfo->strColumnDMFieldAlias == _T("표제사항")	  ||
				pColumnInfo->strColumnDMFieldAlias == _T("UDF_표제사항")	)
			{				
				nColumnCheck = pDM->FindColumn(_T("정렬용본표제"));
				if ( nColumnCheck >= 0 )
				{
					strOrigAlias = pColumnInfo->strColumnDMFieldAlias;
					pColumnInfo->strColumnDMFieldAlias = _T("정렬용본표제");
				}
			}
			
			pDM->SORT(pColumnInfo->strColumnDMFieldAlias, pColumnInfo->IsDesc);
			
			if ( strOrigAlias != _T("") && nColumnCheck >= 0 )
			{
				pColumnInfo->strColumnDMFieldAlias = strOrigAlias;
			}
			
			
			if (pColumnInfo->IsDesc == TRUE) pColumnInfo->IsDesc = FALSE;
			else pColumnInfo->IsDesc = TRUE;
			Display();
			pMgr->PostMessage(MSG_GRID_DISPLAY);
		}
	} else {
		if ( m_nSelectIdx == col-1 && m_bCheck ) return;
		if ( m_bGridEdit ) GridEdit(GetRow()-1, GetCol()-1);	
	}
	
	CMSHFlexGridInterface::OnLButtonDblClk(nFlags, point);
	
	
	EFS_END
}

VOID CESL_Grid2::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CMSHFlexGridInterface::OnLButtonDown(nFlags, point);
		
	if (ELEMENT) 
	{
		if (ELEMENT->LINK_DATA.Find(_T(":")) != -1) 
		{
			if (m_nRealRowCount == 0)
			{
				return;
			}
		}
	}
	if (GetMouseRow() == 0) return;
	if (m_bFroceReverse)
	{
		SetReverse(GetRow()-1);
	}
}

VOID CESL_Grid2::OnLButtonUp(UINT nFlags, CPoint point) 
{
	EFS_BEGIN				
				
	CMSHFlexGridInterface::OnLButtonUp(nFlags, point);

	if (m_nRealRowCount == 0)
	{
		return ;
	}

	INT col = GetCol();
	if ( m_bCheck && m_cSelectType != 'N' && m_nSelectIdx == col-1) {
		if (GetMouseCol() != 0) 
		{
			if(0 == GetMouseRow())
			{				
				CGridCellCheck* pGrid = (CGridCellCheck*)CMSHFlexGridInterface::GetCell(0, col);
								
				if(pGrid)
				{
					BOOL bCheck = pGrid->GetCheck();
					pGrid->OnClick(point);
					BOOL bCheck2 = pGrid->GetCheck();

					if(bCheck != bCheck2)
					{
						if(bCheck2)
						{
							pGrid->SetCheck();
							UserSelectProc(4002);
						}
						else
						{
							pGrid->SetCheck(FALSE);
							UserSelectProc(4003);
						}
					}
				}
			}
			else
			{
				bool check;
				GetCheck(GetRow()-1, check);
				if ( check )
				{
					SetCheck(GetRow()-1, false);
				}
				else 
				{
					SetCheck(GetRow()-1, true);
				}
			}
		}
	}
	
	EFS_END
}

VOID CESL_Grid2::OnGridSelEditor(WPARAM w, LPARAM l)
{
	EFS_BEGIN
		
		if ( w == 0 ) {
			
		} else if ( w == 1 ) {
			
			CString str;
			m_pSelEditor->GetWindowText(str);
			SetAt(m_pSelEditor->row, m_pSelEditor->col, str);
			
			::PostMessage(GetParent()->GetSafeHwnd(), ESL_GRID_MODIFY, m_pSelEditor->row, m_pSelEditor->col);
		} 
		
		if ( m_pSelEditor != NULL ) {
			delete m_pSelEditor;
			m_pSelEditor = NULL;
		}
		
		SetFocus();
		
		EFS_END
}

BOOL CESL_Grid2::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN
		if ( pMsg->message == WM_KEYDOWN ) {
			if ( pMsg->wParam == VK_RETURN ) {
				CRect rect;
				GetWindowRect(rect);
				CPoint pos;
				pos = rect.CenterPoint();
				SetCursorPos(pos.x, pos.y);
				PostMessage(WM_LBUTTONDBLCLK, NULL, NULL);
				return true;
			} else if ( pMsg->wParam == VK_ESCAPE ) {
				return true;
			} else if ( pMsg->wParam == VK_F11 ) {
				if(ESL_GRID_NOT_USE_F11 != (m_uUseKeyFlag & ESL_GRID_NOT_USE_F11) )
				{
					PostMessage(MSG_GRID_CONFIG);
				}
				return true;
			} else if ( pMsg->wParam == VK_F12 ) {
				if(ESL_GRID_NOT_USE_F12 != (m_uUseKeyFlag & ESL_GRID_NOT_USE_F12) )
				{
					PostMessage(WM_USER+1907);
				}
				return true;
			} else if ( pMsg->wParam == VK_F1 ) {
				return false;
			}
			if ((_TUCHAR)pMsg->wParam >= 0x80 || _istalnum(pMsg->wParam))
			{
				m_mGridEditMsg.message = pMsg->message;
				m_mGridEditMsg.wParam = pMsg->wParam;
				m_mGridEditMsg.lParam = pMsg->lParam;
				
				if ( pMsg->message == WM_KEYDOWN && GetKeyState(VK_CONTROL) < 0 && pMsg->wParam == VK_F2 ) 
				{
					
				}
				else
				{
				}
			}
			if( VK_F3 == pMsg->wParam )
			{	

				if( NULL == m_pFindWordDlg )
				{
					m_pFindWordDlg = new CESLDlg_FindWord;
					m_pFindWordDlg->Create(this);
					m_pFindWordDlg->CenterWindow();
				}
				m_pFindWordDlg->SetParentGrid(this);
				m_pFindWordDlg->ShowWindow(SW_SHOW);
			}	
		}
		else if (pMsg->message == WM_KEYUP)
		{
			if (pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN || pMsg->wParam == VK_PRIOR || pMsg->wParam == VK_NEXT)
			{
				if (m_bFroceReverse) SetReverse(GetRow()-1);
			}
		}
		
		return CMSHFlexGridInterface::PreTranslateMessage(pMsg);
		
		EFS_END
			return FALSE;
		
}

VOID CESL_Grid2::OnRButtonDown(UINT nFlags, CPoint point) 
{
	EFS_BEGIN
		
		if( GetMouseCol() != m_nSelectIdx+1 )
		{
			if( GetRow() != GetMouseRow() )		SetRow(GetMouseRow());
		}
		
		if( GetCol() != GetMouseCol() )		SetCol(GetMouseCol());
		
		INT col = GetCol();
		CRect rect;
		GetWindowRect(&rect);
		
		if( m_pParentView )
		{
			if( m_bCheck && m_nSelectIdx == col-1 )
			{
				if( m_bMenu )
				{
					CMenu menu;
					menu.CreatePopupMenu();
					menu.AppendMenu(MF_STRING, 4000, _T("선정"));
					menu.AppendMenu(MF_STRING, 4001, _T("비선정"));
					menu.AppendMenu(MF_STRING, 4002, _T("전체선정"));
					menu.AppendMenu(MF_STRING, 4003, _T("전체비선정"));
					menu.AppendMenu(MF_SEPARATOR);
					menu.AppendMenu(MF_STRING, 4004, _T("그리드환경설정"));
					menu.AppendMenu(MF_STRING, 4005, _T("그리드엑셀출력"));
					
					menu.TrackPopupMenu(TPM_LEFTALIGN, rect.left + point.x, rect.top + point.y, this);
				}
			}
			else
			{

			}
		}
		else
		{
			if( m_bCheck && m_nSelectIdx == col-1 )
			{
				if( m_bMenu )
				{
					CMenu menu;
					menu.CreatePopupMenu();
					menu.AppendMenu(MF_STRING, 4000, _T("선정"));
					menu.AppendMenu(MF_STRING, 4001, _T("비선정"));
					menu.AppendMenu(MF_STRING, 4002, _T("전체선정"));
					menu.AppendMenu(MF_STRING, 4003, _T("전체비선정"));
					menu.AppendMenu(MF_SEPARATOR);
					menu.AppendMenu(MF_STRING, 4004, _T("그리드환경설정"));
					menu.AppendMenu(MF_STRING, 4005, _T("그리드엑셀출력"));
					
					menu.TrackPopupMenu(TPM_LEFTALIGN, rect.left + point.x, rect.top + point.y, this);
				}
			}
		}
		
		CMSHFlexGridInterface::OnRButtonDown(nFlags, point);
		
		EFS_END
}

VOID CESL_Grid2::OnMouseMove(UINT nFlags, CPoint point) 
{
	EFS_BEGIN
		CMSHFlexGridInterface::OnMouseMove(nFlags,point);
		DisplayToolTip();
	EFS_END
}


VOID CESL_Grid2::MainToolBarProc(UINT nIDC)
{
	EFS_BEGIN
		
		m_pParentView->PostMessage(WM_COMMAND, nIDC);
	
	return;
	
	EFS_END
}

VOID CESL_Grid2::UserSelectProc(UINT  nIDC)
{
	EFS_BEGIN
		
		if ( m_cSelectType == 'N' ) return;
		
		INT i;
		
		INT top, bottom;
		top = GetRow()-1;
		
		bottom = GetRowSel()-1;
		
		INT tmp;
		if ( top < bottom ) {
			tmp = bottom;
			bottom = top;
			top = tmp;
		}
		
		INT max;
		max = GetRows() - 1;
		
		switch( nIDC )
		{
		case 4000:
			{
				for ( i=bottom ; i<=top ; i++ ) 	SetCheck(i, true);
			}
			break;
			
		case 4001:
			{
				for ( i=bottom ; i<=top ; i++ )		SetCheck(i, false);
			}
			break;
			
		case 4002:
			{
				for ( i=0 ; i<max ; i++ )			SetCheck(i, true);
			}
			break;
			
		case 4003:
			{
				for ( i=0 ; i<max ; i++ ) 			SetCheck(i, false);
			}
			break;
			
		case 4004:		PostMessage(MSG_GRID_CONFIG);			
			break;
			
		case 4005:		PostMessage(WM_USER+1907);
			break;
			
		default:
			break;
		}
		
		EFS_END
}

INT CESL_Grid2::FindLinkDMAndAlias(INT nCol, CString &strDMAlias, CString &strDMFieldAlias) // nCol은 0부터 시작하는 Column 즉 Line Number가 나오는 Fixed Column이 0
{
	EFS_BEGIN
		
		strDMAlias = _T("");		strDMFieldAlias = _T("");
	CString LINK_DATA = ELEMENT->LINK_DATA;
	if (UseUserDefine()) {
		LINK_DATA = USER_LINK_DATA;
	}
	LINK_DATA.TrimLeft();				LINK_DATA.TrimRight();
	if (LINK_DATA.GetLength() == 0) return -1;
	
	CString strColumn, strTmpDMAlias, strTmpDMFieldAlias, szTmp;
	for (INT i = 0; true; i++) {
		strColumn = _T("");
		strColumn = LINK_DATA.SpanExcluding(_T(","));
		strColumn.TrimLeft();		strColumn.TrimRight();
		if (strColumn.GetLength() > 0) {
			if (ParseMgrAndColumn(strColumn, strTmpDMAlias, strTmpDMFieldAlias) >= 0) {
				if (i == nCol-1) {
					strDMAlias = strTmpDMAlias;
					strDMFieldAlias = strTmpDMFieldAlias;
					return 0;
				}
			}
		}
		if (strColumn == LINK_DATA) break;
		szTmp = LINK_DATA.Mid(strColumn.GetLength()+1);
		if (szTmp.GetLength() == 0) break;
		LINK_DATA = szTmp;
	}
	
	return -1;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::InitColumnLinkDMAndAlias(VOID)
{
	EFS_BEGIN
		
		if (ELEMENT == NULL) return 0;
		CString LINK_DATA = ELEMENT->LINK_DATA;
		if (UseUserDefine()) {
			LINK_DATA = USER_LINK_DATA;
		}
		LINK_DATA.TrimLeft();				LINK_DATA.TrimRight();
		if (LINK_DATA.GetLength() == 0) return -1;
		
		FreeColumnInfo();
		CESL_Mgr *pMgr = (CESL_Mgr*)ELEMENT->WorkWnd;
		CString strColumn, strTmpDMAlias, strTmpDMFieldAlias, szTmp;
		m_arrColumnInfo.RemoveAll();
		CESL_GridColumnInfo *pColumnInfo;
		for (INT i = 0; true; i++) {
			pColumnInfo = new CESL_GridColumnInfo;
			pColumnInfo->strColumnDMAlias = _T("");
			pColumnInfo->strColumnDMFieldAlias = _T("");
			pColumnInfo->IsDesc = FALSE;
			strColumn = _T("");
			strColumn = LINK_DATA.SpanExcluding(_T(","));
			strColumn.TrimLeft();		strColumn.TrimRight();
			if (strColumn.GetLength() > 0) {
				if (ParseMgrAndColumn(strColumn, strTmpDMAlias, strTmpDMFieldAlias) >= 0) {
					pColumnInfo->strColumnDMAlias = strTmpDMAlias;
					pColumnInfo->strColumnDMFieldAlias = strTmpDMFieldAlias;
					if (strTmpDMAlias.GetLength() > 0 && strTmpDMFieldAlias.GetLength() > 0) {
						CESL_DataMgr *pDM = pMgr->FindDM(strTmpDMAlias);
						if (strTmpDMFieldAlias.Find('.', 0) != -1) {
							pColumnInfo->nColumnPos = 1000000;		// 마크 컬럼임
						} else {
							if (pDM) pColumnInfo->nColumnPos = pDM->FindColumn(strTmpDMFieldAlias);
						}
					}
				}
			}
			m_arrColumnInfo.AddTail((CObject*)pColumnInfo);
			if (strColumn == LINK_DATA) break;
			szTmp = LINK_DATA.Mid(strColumn.GetLength()+1);
			if (szTmp.GetLength() == 0) break;
			LINK_DATA = szTmp;
		}
		
		return 0;
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::InitColumnInfo(VOID)
{
	EFS_BEGIN
		
		INT nColumns = m_arrColumnInfo.GetCount();
	if (nColumns == 0) return -1;
	CESL_GridColumnInfo *pColumnInfo;
	POSITION pos = m_arrColumnInfo.GetHeadPosition();
	CESL_Mgr *pMgr = (CESL_Mgr*)ELEMENT->WorkWnd;
	if (pMgr == NULL) return -2;
	for (INT i = 0; i < nColumns; i++) {
		pColumnInfo = NULL;
		pColumnInfo = (CESL_GridColumnInfo*)m_arrColumnInfo.GetNext(pos);
		if (pColumnInfo == NULL) continue;
		CESL_DataMgr *pDM = pMgr->FindDM(pColumnInfo->strColumnDMAlias);
		if (pDM == NULL) continue;
		if (pColumnInfo->strColumnDMFieldAlias.Find('.', 0) != -1) {
			pColumnInfo->nColumnPos = 1000000;
		} else {
			if (pDM) pColumnInfo->nColumnPos = pDM->FindColumn(pColumnInfo->strColumnDMFieldAlias);
		}
	}
	return 0;
	
	EFS_END
		return -1;
	
}

BOOL CESL_Grid2::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	EFS_BEGIN
		
	INT nRowsScrolled = 1 * zDelta / 120;
	
    if (nRowsScrolled > 0)
        for (INT i = 0; i < nRowsScrolled; i++)
			this->PostMessage(WM_VSCROLL, SB_LINEUP, 0);
		else
			for (INT i = 0; i > nRowsScrolled; i--)
				this->PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
			
			return CMSHFlexGridInterface::OnMouseWheel(nFlags, zDelta, pt);
			
			EFS_END
				return FALSE;
			
}

VOID CESL_Grid2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_F2 || nChar == VK_RETURN) {
		CRect rect;
		GetWindowRect(rect);
		CPoint pos;
		pos = rect.CenterPoint();
		SetCursorPos(pos.x, pos.y);
		PostMessage(WM_LBUTTONDBLCLK, NULL, NULL);
		return;
	}
	
	CMSHFlexGridInterface::OnKeyDown(nChar, nRepCnt, nFlags);
}

INT CESL_Grid2::Display(INT index)
{
	EFS_BEGIN
		
		CESL_GridData *pData = (CESL_GridData *)m_pData;
	if (pData == NULL) return -1;
	if (pData->m_Status == THREAD_RUNNING) return -1;
	if (m_bProtectSelectColumn == TRUE) RefreshSelectedRows();
	RemoveAll2();
	InitColumnInfo();
	pData->m_pArrColumnInfo = &m_arrColumnInfo;
	if (pData->m_Status == THREAD_RUNNING) return -1;
	pData->pMgr = (CESL_Mgr*)ELEMENT->WorkWnd;
	pData->pGrid = NULL;
	pData->pGrid2 = this;
	pData->m_strGridAlias = ELEMENT->CTRL_ALIAS;
	pData->m_pOwnerWnd = ELEMENT->WorkWnd;
	pData->m_fpEventHandler = m_fpEventHandler;
	pData->m_pRealRowCount = &m_nRealRowCount;
	
	BOOL bExcelExport = FALSE;
	if( this->IsWindowVisible() )
	{
		CESL_GridColumnInfo *parrColumnInfo[64];
		CString strDMAlias = _T("");
		INT nColumns = pData->m_pArrColumnInfo->GetCount();
		POSITION pos;
		
		for (INT i=0; i<nColumns; i++) 
		{
			pos = pData->m_pArrColumnInfo->FindIndex(i);
			if (pos == NULL) continue;
			parrColumnInfo[i] = (CESL_GridColumnInfo*)pData->m_pArrColumnInfo->GetAt(pos);
			if (strDMAlias.GetLength() == 0) strDMAlias = parrColumnInfo[i]->strColumnDMAlias;	
		}
		
		if( strDMAlias.GetLength() )
		{
			CESL_DataMgr *pDM = pData->pMgr->FindDM(strDMAlias);
			if (NULL != pDM) 
			{
				INT nRows = pDM->GetRowCount();	
				
				CString strNotify;
				
				if( 5001 <= nRows && 30001 > nRows )
				{
					strNotify.Format(_T("[5000]건 이상 검색되었습니다.  \r\n ")
						_T("  엑셀로 출력하시겠습니까?\r\n\r\n")
						_T("예      : 엑셀출력\r\n")
						_T("아니오: 그리드출력"));	

				}
				else if( 30001 <= nRows )
				{
					strNotify.Format(_T("[30000]건 이상 검색되어 엑셀로 출력합니다.\r\n"));	
					
					AfxMessageBox(strNotify, MB_ICONINFORMATION);
					
					bExcelExport = TRUE;
				}
				
				if( bExcelExport )
				{
					INT ids;
					DWORD START, FINISH;
					INT nWidth, nHeight;
					TCHAR TmpDir[1024] ;
					TCHAR szLineData[10000];
					CRect rect, DlgRect;
					CString strPathName, strData, strTmpData ;
					CFile file;
					
					ZeroMemory(TmpDir, 1024);
					GetCurrentDirectory(1024,TmpDir);
					
					TRACE (_T("(Start) - Excel Export \n"));
					
					GetClientRect(&rect);		
					nWidth	= DlgRect.Width();
					nHeight = DlgRect.Height();
					
					DlgRect = rect;
					DlgRect.top		= (nHeight - 50) / 2;
					DlgRect.bottom	= DlgRect.top + 50;
					DlgRect.left	= (nWidth - 175) / 2;
					DlgRect.right	= DlgRect.left + 175;
					
					CESLDlg_ExcelExportWait WaitDlg(this);
					WaitDlg.Create(IDD_DLG_MSG_EXCELEXPORTWAIT,this);
					WaitDlg.MoveWindow( DlgRect );
					WaitDlg.CenterWindow(NULL);
					WaitDlg.ShowWindow(TRUE);	
					WaitDlg.m_Progress1.SetPos(0);
					WaitDlg.m_Progress1.SetRange(0,100);

					BOOL	bIsExcel	= FALSE,
							bIsHancell	= FALSE,
							bIsCSV		= FALSE
							;
					bIsExcel	= WaitDlg.m_bIsExcel;
					bIsHancell	= WaitDlg.m_bIsHancell;
					bIsCSV		= WaitDlg.m_bIsCSV;
					
					if(WaitDlg.m_nDialogReturn != IDOK)
					{
						return -1;
					}
					if(m_bPrivacyPrintLogRecord && 0 < pDM->GetRowCount()){
						if(NULL != ELEMENT){
							CESL_Mgr *pMgr = (CESL_Mgr*)ELEMENT->WorkWnd;
							if (pMgr) {
								CString printType = _T("E");
								if(bIsHancell == TRUE) printType = _T("H");
								else if(bIsCSV == TRUE) printType = _T("C");
								if(pMgr->PrivacyPrintLogRecord(pDM, m_strPrintInfo, printType, 0 ) < 0 ) {
									return 0 ;
								}
							}
						}			
					}

					CString strTotalData;
					CString strTmp2;
					FLOAT	nProgressStep=0;
					FLOAT	nProgress =0;
					CStringArray strTotalExportLine;	
					strTotalExportLine.RemoveAll();
					
					BOOL bRemainder = FALSE;
					nProgressStep=(float)100/nRows;
					
					DWORD *pRow;
					pos = pDM->data.GetHeadPosition();
					
					START = GetTickCount();
					
					strData.Empty();	
					strTmpData.Empty();	
					memset(szLineData, 0, 10000);

					for( INT k=0; k<nColumns+1; k++ )				
					{						
						strData = GetTextMatrix(0, k);			
						strTmpData = strTmpData + _T("\"") + strData + _T("\"") + _T("\t");							
					}
					
					strTmpData = strTmpData + _T("\n");
					
					_stprintf(szLineData, _T("%s"), strTmpData );						
					
					INT nLen = _tcslen(szLineData);
					for (k=0 ; k < nLen ; k++)
					{
						if ( szLineData[k] == 13 ) 
						{
							szLineData[k] = 32;						
						}
					}							
					strTotalData += szLineData;
					
					for (i = 0; i < nRows; i++) 
					{
						strData.Empty();	
						strTmpData.Empty();	
						memset(szLineData, 0, 10000);
						
						if (pos == NULL) break;
						pRow  = (DWORD*)pDM->data.GetNext(pos);
						if (pRow == NULL) continue;
						
						strData.Format(_T("%d"), i+1);
						
						strTmpData = strTmpData + _T("\"") + strData + _T("\"") + _T("\t");
						
						for (INT j = 0; j < nColumns; j++) 
						{
							if (parrColumnInfo[j]->nColumnPos < 0) 
							{
								strTmpData = strTmpData + _T("\"\"\t");							
								continue;
							}
							
							strData = _T("");
							if (parrColumnInfo[j]->nColumnPos == 1000000) 
							{
								ids = pDM->GetCellData(parrColumnInfo[j]->strColumnDMFieldAlias, i, strData);
							} else 
							{
								strData = pDM->GetCellData(pRow, parrColumnInfo[j]->nColumnPos);
								ids = 0;
							}
							if (ids <0) continue;	
							
							if	   (strData == _T("BOT111O")) strData = _T("기초자료");
							else if(strData == _T("BOT112O")) strData = _T("반입자료");
							else if(strData == _T("BOT211O")) strData = _T("수서복본추기자료");
							else if(strData == _T("BOT212O")) strData = _T("정리복본추기자료");
							else if(strData == _T("BOA111N")) strData = _T("구입대상자료");
							else if(strData == _T("BOA112N")) strData = _T("주문자료");
							else if(strData == _T("BOA113O")) strData = _T("미납품자료");
							else if(strData == _T("BOA114O")) strData = _T("구입인계대상자료");
							else if(strData == _T("BOA211O")) strData = _T("기증접수자료");
							else if(strData == _T("BOA212O")) strData = _T("기증부당자료");
							else if(strData == _T("BOR111N")) strData = _T("등록인계자료");
							else if(strData == _T("BOR112N")) strData = _T("등록자료");
							else if(strData == _T("BOR113O")) strData = _T("가원부번호부여자료");
							else if(strData == _T("BOC111O")) strData = _T("복본조사대상자료");
							else if(strData == _T("BOC112O")) strData = _T("분류대상자료");
							else if(strData == _T("BOC113N")) strData = _T("정리대상자료");
							else if(strData == _T("BOC114O")) strData = _T("열람인계대상자료");
							else if(strData == _T("BOL111O")) strData = _T("열람인계자료");
							else if(strData == _T("BOL112N")) strData = _T("배가자료");
							else if(strData == _T("BOL113O")) strData = _T("수리제본자료");
							else if(strData == _T("BOL114O")) strData = _T("파손자료");
							else if(strData == _T("BOL115O")) strData = _T("제적자료");
							else if(strData == _T("BOL116O")) strData = _T("재정리자료");
							else if(strData == _T("BOL211O")) strData = _T("관외대출자료");
							else if(strData == _T("BOL212O")) strData = _T("관내대출자료");
							else if(strData == _T("BOL411O")) strData = _T("단행책두레업무진행자료");
							else if(strData == _T("BOL511O")) strData = _T("통합도서타관반납");
							else if(strData == _T("BOL213O")) strData = _T("특별대출자료");
							else if(strData == _T("BOL214O")) strData = _T("긴급대출자료");
							else if(strData == _T("BOL215O")) strData = _T("배달자료");
							else if(strData == _T("BOL218O")) strData = _T("상호대차자료");
							else if(strData == _T("BOL219O")) strData = _T("책나래대출자료");
							else if(strData == _T("BOL221O")) strData = _T("관외대출자료");
							else if(strData == _T("BOL222O")) strData = _T("관내대출자료");
							else if(strData == _T("BOL223O")) strData = _T("특별대출자료");
							else if(strData == _T("BOL224O")) strData = _T("긴급대출자료");
							else if(strData == _T("BOL225O")) strData = _T("배달자료");
							else if(strData == _T("BOL251N")) strData = _T("수리제본자료");
							else if(strData == _T("BOL252N")) strData = _T("분실자료");
							else if(strData == _T("BOL253N")) strData = _T("제적자료");
							else if(strData == _T("BOL217O")) strData = _T("장기대출자료");
							
							strTmpData = strTmpData + _T("\"") + strData + _T("\"") + _T("\t");
						}						
						
						nProgress += nProgressStep;
						WaitDlg.m_Progress1.SetPos((INT)nProgress);
						
						strTmpData = strTmpData + _T("\n");
						
						_stprintf(szLineData, _T("%s"), strTmpData );
						
						
						INT nLen = _tcslen(szLineData);
						for (int k=0 ; k < nLen ; k++)
						{
							if ( szLineData[k] == 13 ) 
							{
								szLineData[k] = 32;						
							}
						}	
						
						strTotalData += szLineData;
						if ( i%1000 == 0 ) 
						{
							strTotalExportLine.Add(strTotalData);
							strTotalData = _T("");
							bRemainder = FALSE;
						}
						else if ( i%1000 == 1)
						{
							bRemainder = TRUE;
						}
					}
					
					if ( bRemainder )
					{
						strTotalExportLine.Add(strTotalData);
						strTotalData = _T("");
					}
					
					for ( i=0 ; i<strTotalExportLine.GetSize() ; i++ )
					{
						strTotalData += strTotalExportLine.GetAt(i);
					}
					
					WaitDlg.m_Progress1.SetPos(100);
					
					CXLEzAutomation XL;			
					XL.ExportCString(strTotalData);
					
					FINISH = GetTickCount();
					TRACE(_T("(Finish) - Excel Export [%d.%d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);
					
					SetCurrentDirectory(TmpDir);					
					
					return 0;
				}			
			}
		}
	}
	
	::AfxBeginThread(GridDisplayThread2, this);
	
	if (GetParent()->IsWindowVisible())
	{
		if ( GetParentFrame() != NULL ) 
		{
			GetParentFrame()->BeginModalState();
		}
	}
	
	MSG msg;
	while(TRUE)
	{
		if (::PeekMessage(&msg, GetSafeHwnd(), 1, 1, PM_NOREMOVE))
			AfxGetApp()->PumpMessage();
		
		if (WaitForSingleObject(m_hThread, 0) == WAIT_OBJECT_0)
			break;
		
	}

	if (GetParent()->IsWindowVisible())
	{
		if ( GetParentFrame() != NULL ) 
		{
			GetParentFrame()->EndModalState();
		}
	}

	SetRow(1);
	SetCol(1);
	if (m_bProtectSelectColumn == TRUE) ReshowSelectedRows();
	
	return 0;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::FreeWorkDlg(VOID)
{
	if (m_pWorkDlg) {
		delete m_pWorkDlg;
		m_pWorkDlg = NULL;
	}
	return 0;
}

INT CESL_Grid2::CountLinkColumns(VOID)
{	
	EFS_BEGIN
		
		INT count = 0;
	if (ELEMENT->INIT_VALUE.GetLength() == 0) return 0;
	for (INT i = 0; i < ELEMENT->INIT_VALUE.GetLength(); i++) {
		if (ELEMENT->INIT_VALUE.GetAt(i) == ',') count++;
	}
	return count+1;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::ParseMgrAndColumn(CString link_data, CString &MgrAlias, CString &ColumnAlias)
{
	EFS_BEGIN
		
	MgrAlias = _T("");
	ColumnAlias = _T("");
	CString data;
	if (link_data.GetAt(0) == ':') {
		INT mgrpos = link_data.Find('.', 1);
		if (mgrpos == -1) return -1;
		MgrAlias = link_data.Mid(1, mgrpos-1);
		ColumnAlias = link_data.Mid(mgrpos+1);
	}
	return 0;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::UnSelectAll(VOID)
{
	EFS_BEGIN
		
		UserSelectProc(4003);
	return 0;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::SetColumnInfo(VOID)
{
	EFS_BEGIN
		
	InitGridConfig();
	if (UseUserDefine()) {
		UserSetColumnInfo();
		return 0;
	}
	CString init_value = ELEMENT->INIT_VALUE;
	if (init_value.GetLength() == 0) return 0;
	RemoveAll();
	SetCols(0, CountLinkColumns()+1);
	CString column_info, sztemp;
	CString name, align, size;
	INT ialign, isize;
	INT first;
	for (INT i = 0; true; i++) {
		column_info.Empty();
		column_info = init_value.SpanExcluding(_T(","));
		if (column_info.GetLength() > 0) {
			name.Empty();
			align.Empty();
			size.Empty();
			first = 0;
			first = column_info.Find(':', 0);
			name = column_info.Mid(1, first-1);
			align = column_info.Mid(first+1, column_info.Find(':', first+1)-(first+1));
			first = column_info.Find(':', first+1);
			size = column_info.Mid(first+1, column_info.Find(']', 0)-(first+1));
			ialign = 4;
			isize = 50;
			if (align.GetLength()) ialign = _ttoi(align.GetBuffer(0));
			if (size.GetLength()) isize = _ttoi(size.GetBuffer(0));
			SetColumnInfo(i, name, isize, ialign);
		}
		if (column_info == init_value) break;
		sztemp = init_value.Mid(column_info.GetLength()+1);
		if (sztemp.GetLength() == 0) break;
		init_value = sztemp;
	}
	InitColumnLinkDMAndAlias();
	return 0;
	
	EFS_END
		return -1;
	
}

long CESL_Grid2::GridGetRow()
{
	EFS_BEGIN
		
		return GetRow();
	
	EFS_END
		return -1;
	
}	

VOID CESL_Grid2::OnTimer(UINT nIDEvent) 
{
	EFS_BEGIN
		if (nIDEvent == 0) {
			CESL_GridData *pData = (CESL_GridData *)m_pData;
			if (pData->m_Status == THREAD_NOTRUN) {
				KillTimer(0);
				SetRowHeight(-1, m_nHeight*15);
				SetRowHeight(0, 391);
				GetParentFrame()->EnableWindow(TRUE);
				SetRow(1);
				SetCol(1);
				if (m_bProtectSelectColumn == TRUE) ReshowSelectedRows();
			}
		}
		
		if (nIDEvent == 200) {
		}
		CMSHFlexGridInterface::OnTimer(nIDEvent);
		
		EFS_END
}

void CESL_Grid2::InitToolTip()
{
 	if(NULL != hTip)
 	{
 		return;
 	}
	hTip = CreateWindowEx(WS_EX_TOPMOST,TOOLTIPS_CLASS,NULL,0,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		GetSafeHwnd(),NULL,AfxGetInstanceHandle(),NULL);
	
	
	int x =GetSystemMetrics(SM_CXSCREEN);
	int y =GetSystemMetrics(SM_CYSCREEN);
	
	TOOLINFO ti;
	RECT rt ;
	GetClientRect(&rt);

	rt.right=x;
	rt.bottom=y;
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags=TTF_SUBCLASS;
	ti.hwnd = GetSafeHwnd();
	ti.uId=0;
	ti.lpszText = _T("");
	ti.rect = rt;
	
	::SendMessage(hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti);
	::SendMessage(hTip,TTM_ACTIVATE,(WPARAM)TRUE,0);
	
}

CRect CESL_Grid2::CalculateTextSize(const CString strText, const CRect rTextRect)
{			
	CDC* pDC = GetDC();			
	CFont* OldFont = (CFont*)pDC->SelectStockObject(DEFAULT_GUI_FONT);
	
	pDC->SelectObject(font);
	CRect rText = rTextRect;
	pDC->DrawText(strText, &rText, DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	
	
	pDC->SelectObject(OldFont);
	ReleaseDC(pDC);
	
	return rText;
}

bool CESL_Grid2::UseUserDefine(VOID)
{
	EFS_BEGIN
		
		CString filename;
	filename.Format(_T("..\\cfg\\grid\\%s_%s.cfg"), ELEMENT->CM_ALIAS, ELEMENT->CTRL_ALIAS);
#ifdef _UNICODE
	CESL_UnicodeFileIO fd;
#elif _MBCS 
	CESL_MBCSFileIO fd;
#endif
	TCHAR szbuf[2048];
	INT ids, pLen, BufSize = 2047;
	ids = fd.Open(filename);
	if (ids < 0) return false;
	pLen = BufSize;
	ids = fd.GetFileLine(szbuf, &pLen, BufSize);
	if (ids == _TEOF) {
		fd.Close();
		return false;
	}
	if (szbuf[0] == '1') {
		ids = fd.GetFileLine(szbuf, &pLen, BufSize);
		if (ids == _TEOF) {
			fd.Close();
			return false;
		}
		USER_INIT_VALUE.Format(_T("%s"), szbuf);
		ids = fd.GetFileLine(szbuf, &pLen, BufSize);
		if (ids == _TEOF) {
			fd.Close();
			return false;
		}
		USER_LINK_DATA.Format(_T("%s"), szbuf);
		fd.Close();
		return true;
	}
	fd.Close();
	return false;
	
	EFS_END
		return FALSE;
	
}

INT CESL_Grid2::UserSetColumnInfo(VOID)
{
	EFS_BEGIN
		
		INT i;
	INT columncount = 0;
	if (USER_INIT_VALUE.GetLength() == 0) return 0;
	for (i = 0; i < USER_INIT_VALUE.GetLength(); i++) {
		if (USER_INIT_VALUE.GetAt(i) == ',') columncount++;
	}
	InitColumn(columncount+1);
	
	CString init_value = USER_INIT_VALUE;
	if (init_value.GetLength() == 0) return 0;
	CString column_info, sztemp;
	CString name, align, size;
	INT ialign, isize;
	INT first;
	for (i = 0; true; i++) {
		column_info.Empty();
		column_info = init_value.SpanExcluding(_T(","));
		if (column_info.GetLength() > 0) {
			name.Empty();
			align.Empty();
			size.Empty();
			first = 0;
			first = column_info.Find(':', 0);
			name = column_info.Mid(1, first-1);
			align = column_info.Mid(first+1, column_info.Find(':', first+1)-(first+1));
			first = column_info.Find(':', first+1);
			size = column_info.Mid(first+1, column_info.Find(']', 0)-(first+1));
			ialign = 4;
			isize = 50;
			if (align.GetLength()) ialign = _ttoi(align.GetBuffer(0));
			if (size.GetLength()) isize = _ttoi(size.GetBuffer(0));
			SetColumnInfo(i, name, isize, ialign);
		}
		if (column_info == init_value) break;
		sztemp = init_value.Mid(column_info.GetLength()+1);
		if (sztemp.GetLength() == 0) break;
		init_value = sztemp;
	}
	InitColumnLinkDMAndAlias();
	return 0;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::OnGridConfig(VOID)
{
	EFS_BEGIN

	if(NULL == ELEMENT) return 0;
	CESL_Mgr *pMgr = (CESL_Mgr*)ELEMENT->WorkWnd;
	if (!pMgr) return 0;
	CString strThisGridAlias;
	strThisGridAlias.Format(_T("%s.%s"), pMgr->SM_ALIAS, ELEMENT->CTRL_ALIAS);
	strThisGridAlias.TrimLeft();			strThisGridAlias.TrimRight();
	strThisGridAlias.MakeUpper();
	CStdioFile fd;
	BOOL IsAbleEdit = TRUE;
	if (fd.Open(_T("..\\CFG\\EditDisableGridList.cfg"), CStdioFile::modeRead|CFile::typeBinary) == TRUE) {
		TCHAR cUni;
		fd.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fd.SeekToBegin();
		}
		CString strLine;
		while(TRUE) {
			if (fd.ReadString(strLine) == FALSE) break;
			strLine.TrimLeft();			strLine.TrimRight();
			if (strLine.GetLength() == 0) continue;
			if (strLine.GetAt(0) == '#') continue;
			
			strLine.MakeUpper();
			if (strThisGridAlias == strLine) {
				IsAbleEdit = FALSE;
				break;
			}
		}
		fd.Close();
	}
	if (IsAbleEdit == FALSE) {
		MessageBox(_T("사용자 환경 설정을 할 수 없는 그리드 입니다.[Disable로 설정되었습니다.]"), _T("Warnning!"), MB_ICONEXCLAMATION);
		return 0;
	}
	
	CString strStandardDMAlias = _T("");
	if (ELEMENT->LIST_DATA.GetLength() > 0) {
		strStandardDMAlias = ELEMENT->LIST_DATA;
	} else {
		INT pos1, pos2;
		pos1 = pos2 = -1;
		pos1 = ELEMENT->LINK_DATA.Find(_T(":"), 0);
		if (pos1 != -1) {
			pos2 = ELEMENT->LINK_DATA.Find(_T("."), pos1);
		}
		if (pos1 != -1 && pos2 != -1) {
			strStandardDMAlias = ELEMENT->LINK_DATA.Mid(pos1+1, pos2-pos1-1);
		}
	}
	if (strStandardDMAlias.GetLength() == 0) {
		MessageBox(_T("사용자 환경 설정을 할 수 없는 그리드 입니다.[정보를 찾을 수 없습니다.]"), _T("Warnning!"), MB_ICONEXCLAMATION);
		return 0;
	}
	
	CESLDlg_GridConfig dlg;
	
	CESL_DataMgr *pDM = pMgr->FindDM(strStandardDMAlias);
	if (!pDM) {
		MessageBox(_T("사용자 환경 설정을 할 수 없는 그리드 입니다.[데이타를 찾을 수 없습니다.]"), _T("Warnning!"), MB_ICONEXCLAMATION);
		return 0;
	}
	dlg.nALIAS = pDM->GetAllAlias(dlg.ALIAS, 512);
	
	dlg.CM_ALIAS = ELEMENT->CM_ALIAS;
	dlg.GRID_ALIAS = ELEMENT->CTRL_ALIAS;
	dlg.DEFAULT_INIT_VALUE = ELEMENT->INIT_VALUE;
	dlg.DEFAULT_LINK_DATA = ELEMENT->LINK_DATA;
	dlg.DM = strStandardDMAlias;
	dlg.DoModal();

	SetRow(0);
	SetCol(0);
	SetColumnInfo();
	Display();
	return 0;
	
	EFS_END
		return -1;
	
}

VOID CESL_Grid2::OnExcelExport(VOID)
{
	EFS_BEGIN
		
	INT nWidth, nHeight, i, j, nRow, nCol;
	TCHAR TmpDir[1024] ;
	CString szLineData;
	CRect rect, DlgRect;
	CString strPathName, strData, strTmpData ;
	CFile file;
	
	ZeroMemory(TmpDir, 1024);
	GetCurrentDirectory(1024,TmpDir);
	
	DWORD START, FINISH;
	TRACE (_T("(Start) - Excel Export \n"));
	START = GetTickCount();
	
	
	GetClientRect(&rect);		
	nWidth	= DlgRect.Width();
	nHeight = DlgRect.Height();
	
	DlgRect = rect;
	DlgRect.top		= (nHeight - 50) / 2;
	DlgRect.bottom	= DlgRect.top + 50;
	DlgRect.left	= (nWidth - 175) / 2;
	DlgRect.right	= DlgRect.left + 175;
	
	CESLDlg_ExcelExportWait WaitDlg(this);
	WaitDlg.Create(IDD_DLG_MSG_EXCELEXPORTWAIT, this);
	WaitDlg.MoveWindow( DlgRect );
	WaitDlg.CenterWindow(NULL);
	WaitDlg.ShowWindow(TRUE);	
	WaitDlg.m_Progress1.SetPos(0);
	WaitDlg.m_Progress1.SetRange(0,100);

	CString strTotalData;
	CString strTmp2;
	FLOAT	nProgressStep=0;
	FLOAT	nProgress =0;
	CStringArray strTotalExportLine;	
	
	nRow = GetRows();
	nCol = GetCols(0);	
	nProgressStep=(float)100/nRow;
	BOOL bRemainder;	
	
	strTotalExportLine.RemoveAll();
	bRemainder = FALSE;
	BOOL bChkUserDefineGrid;
	bChkUserDefineGrid = TRUE;

	if(NULL != ELEMENT)
	{
		CESL_Mgr *pMgr = (CESL_Mgr*)ELEMENT->WorkWnd;
		if (pMgr) 
		{
			CString strThisGridAlias;
			strThisGridAlias.Format(_T("%s.%s"), pMgr->SM_ALIAS, ELEMENT->CTRL_ALIAS);
			strThisGridAlias.TrimLeft();			strThisGridAlias.TrimRight();
			strThisGridAlias.MakeUpper();
			CStdioFile fd;
			BOOL IsAbleEdit = TRUE;
			if (fd.Open(_T("..\\CFG\\EditDisableGridList.cfg"), CStdioFile::modeRead|CFile::typeBinary) == TRUE) {
				TCHAR cUni;
				fd.Read( &cUni, sizeof(TCHAR) );
				if( 0xFEFF != cUni )
				{
					fd.SeekToBegin();
				}
				CString strLine;
				while(TRUE) {
					if (fd.ReadString(strLine) == FALSE) break;
					strLine.TrimLeft();			strLine.TrimRight();
					if (strLine.GetLength() == 0) continue;
					if (strLine.GetAt(0) == '#') continue;
					
					strLine.MakeUpper();
					if (strThisGridAlias == strLine) {
						IsAbleEdit = FALSE;
						break;
					}
				}
				fd.Close();
			}
			if (IsAbleEdit == FALSE) {
				bChkUserDefineGrid = FALSE;
			}
		};
		
		CString strStandardDMAlias = _T("");
		if (ELEMENT->LIST_DATA.GetLength() > 0) {
			strStandardDMAlias = ELEMENT->LIST_DATA;
		} else {
			INT pos1, pos2;
			pos1 = pos2 = -1;
			pos1 = ELEMENT->LINK_DATA.Find(_T(":"), 0);
			if (pos1 != -1) {
				pos2 = ELEMENT->LINK_DATA.Find(_T("."), pos1);
			}
			if (pos1 != -1 && pos2 != -1) {
				strStandardDMAlias = ELEMENT->LINK_DATA.Mid(pos1+1, pos2-pos1-1);
			}
		}

		if (strStandardDMAlias.GetLength() == 0) 
		{
			bChkUserDefineGrid = FALSE;
		}
		
		CESLDlg_GridConfig dlg;
		
		CESL_DataMgr *pDM = pMgr->FindDM(strStandardDMAlias);
		if (!pDM) 
		{
			bChkUserDefineGrid = FALSE;
		}			
	}
	for( i=0; i<nRow; i++ )
	{
		strData.Empty();	strTmpData.Empty();	

		for( j=0; j<nCol; j++ )
		{	

			if ( GetColWidth(j, 0) > 0 )
 			{
 				strData = GetTextMatrix(i, j);			
 				strTmpData = strTmpData + _T("\"") + strData + _T("\"") + _T("\t");						
 			}
			else if( bChkUserDefineGrid == FALSE )
			{
				strData = GetTextMatrix(i, j);			
 				strTmpData = strTmpData + _T("\"") + strData + _T("\"") + _T("\t");					
			}
		}		
		nProgress += nProgressStep;
		WaitDlg.m_Progress1.SetPos((INT)nProgress);
		
		strTmpData = strTmpData + _T("\n");
		
		strTmpData.Replace(_T("\r\n"), _T(" "));
		szLineData = strTmpData;
		
		strTotalData += szLineData;
		if ( i%1000 == 0 ) 
		{
			strTotalExportLine.Add(strTotalData);
			strTotalData = _T("");
			bRemainder = FALSE;
		}
		else if ( i%1000 == 1)
		{
			bRemainder = TRUE;
		}		
	}
	
	if ( bRemainder )
	{
		strTotalExportLine.Add(strTotalData);
		strTotalData = _T("");
	}
	
	for ( i=0 ; i<strTotalExportLine.GetSize() ; i++ )
	{
		strTotalData += strTotalExportLine.GetAt(i);
	}
	
	WaitDlg.m_Progress1.SetPos(100);
	
	CXLEzAutomation XL;		
	
	XL.ExportCString(strTotalData);
	
	FINISH = GetTickCount();
	TRACE(_T("(Finish) - Excel Export [%d.%d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);
	
	SetCurrentDirectory(TmpDir);
	return;
	EFS_END
}

INT CESL_Grid2::OnExcelSetColumnInfo(VOID)
{
	EFS_BEGIN
		
		return 0;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::AddPopupMenuItem(CView *pView, TCHAR VIEW_STYLE, CArray <CString, CString> *pRemoveListArray/*=NULL*/)
{
	EFS_BEGIN
		
		if (!pView) return -2;
		if (!m_pMenu) return -1;
		if (VIEW_STYLE == 'U') return -3;
		
		CObList *pListToolBar = NULL;
		CKOLAS3_MAINView *pMAINView = NULL;
		CRCView *pRCView = NULL;
		if (VIEW_STYLE == 'M') {
			pMAINView = (CKOLAS3_MAINView *)pView;
			pListToolBar = &pMAINView->m_listToolBar;
		}
		if (VIEW_STYLE == 'R') {
			pRCView = (CRCView *)pView;
			pListToolBar = &pRCView->m_listToolBar;
		}
		m_pParentView = pView;
		if (pListToolBar == NULL) return -4;
		
		INT nToolbars = pListToolBar->GetCount();
		POSITION pos = pListToolBar->GetHeadPosition();
		CToolBar * pToolBar;
		INT nButtons;
		INT i, j;
		UINT nBtnCmdId;
		CString strBtnText;
		for (i = 0; i < nToolbars; i++) {
			pToolBar = (CToolBar *)pListToolBar->GetNext(pos);
			if (!pToolBar) continue;
			CToolBarCtrl &rpToolBarCtrl = pToolBar->GetToolBarCtrl();
			nButtons = rpToolBarCtrl.GetButtonCount();
			for (j = 0; j < nButtons; j++) {
				nBtnCmdId = pToolBar->GetItemID(j);
				pToolBar->GetButtonText(j, strBtnText);
				if (strBtnText.GetLength() == 0) continue;
				if (FindInPopupMenuRemoveList(strBtnText, pRemoveListArray) == TRUE) continue;
				if (rpToolBarCtrl.IsButtonHidden(nBtnCmdId) > 0) continue;	// nonzero is hidden
				if (rpToolBarCtrl.IsButtonEnabled(nBtnCmdId) > 0) {
					m_pMenu->AppendMenu(MF_STRING | MF_ENABLED, nBtnCmdId, strBtnText);
				} else {
					m_pMenu->AppendMenu(MF_STRING | MF_GRAYED, nBtnCmdId, strBtnText);
				}
			}
			if (i != nToolbars-1) {
				m_pMenu->AppendMenu(MF_SEPARATOR);
			}
		}
		
		return 0;
		
		EFS_END
			return -1;
		
}

BOOL CESL_Grid2::FindInPopupMenuRemoveList(CString &strRemoveText, CArray <CString, CString> *pRemoveListArray)
{
	EFS_BEGIN
		
		if (pRemoveListArray == NULL) return false;
		INT nRemoveList = pRemoveListArray->GetSize();
		for (INT i = 0; i < nRemoveList; i++) {
			if (pRemoveListArray->GetAt(i) == strRemoveText) return true;
		}
		return false;
		
		EFS_END
			return FALSE;
		
}

INT CESL_Grid2::InitPopupMenuItem(VOID)
{
	EFS_BEGIN
		
		if (m_pMenu == NULL) {
			m_pMenu = new CMenu;
			m_pMenu->CreatePopupMenu();
		}
		return 0;
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::FreePopupMenuItem(VOID)
{
	EFS_BEGIN
		
		if (m_pMenu) {
			m_pMenu->DestroyMenu();
			delete m_pMenu;
			m_pMenu = NULL;
		}
		return 0;
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::FreeColumnInfo(VOID)
{
	EFS_BEGIN
		
		INT nCount = m_arrColumnInfo.GetCount();
	if (nCount == 0) return 0;
	CESL_GridColumnInfo *pColumnInfo;
	POSITION pos = m_arrColumnInfo.GetHeadPosition();
	for (INT i = 0; i < nCount; i++) {
		pColumnInfo = NULL;
		pColumnInfo = (CESL_GridColumnInfo*)m_arrColumnInfo.GetNext(pos);
		if (pColumnInfo) {
			delete pColumnInfo;
			pColumnInfo = NULL;
		}
	}
	m_arrColumnInfo.RemoveAll();
	return 0;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::InitGridConfig(VOID)
{
	EFS_BEGIN
		
		if (ReadConfigFile() < 0) return -1;
		font = GetFont();
		CY cy = font.GetSize();
		cy.Lo = m_nFontSize * 10000;
		font.SetName(m_fFont.lfFaceName);
		if (m_fFont.lfItalic == 0) font.SetItalic(FALSE);
		else font.SetItalic(TRUE);
		if (m_fFont.lfUnderline == 0) font.SetUnderline(FALSE);
		else font.SetUnderline(TRUE);
		if (m_fFont.lfStrikeOut == 0) font.SetStrikethrough(FALSE);
		else font.SetStrikethrough(TRUE);
		
		font.SetSize(cy);
		font.SetBold(FALSE);
		SetRefFont(font);
		SetGridColor(RGB(m_nLINECOLOR_R, m_nLINECOLOR_G, m_nLINECOLOR_B));
		SetBackColor(RGB(m_nBACKCOLOR_R, m_nBACKCOLOR_G, m_nBACKCOLOR_B));
		SetForeColorBand(0, RGB(m_nFONTCOLOR_R, m_nFONTCOLOR_G, m_nFONTCOLOR_B));
		
		SetRowHeight(-1, m_nHeight*15);
		SetRowHeight(0, 391);
		SetRowHeight(1, m_nHeight*15);

		return 0;
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::ReadConfigFile(VOID)
{
	EFS_BEGIN
		
		CStdioFile fd;
	if (!fd.Open(m_strFileName, CFile::modeRead | CFile::typeBinary)) {
		return -1;
	}
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	CString str;
	CString strHeader, strData;
	INT pos;
	while(TRUE) {
		str = _T("");
		if (fd.ReadString(str) == FALSE) break;
		if (str.GetLength() == 0) break;
		strHeader = _T("");			strData = _T("");
		pos = str.Find('=', 0);
		if (pos == -1) continue;
		strHeader = str.Left(pos);
		strData = str.Right(str.GetLength() - pos-1);
		
		strHeader.TrimLeft();		strHeader.TrimRight();
		strData.TrimLeft();		strData.TrimRight();
		
		SetData(strHeader, strData);
	}
	fd.Close();
	return 0;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::SetData(CString strHeader, CString strData)
{
	EFS_BEGIN
		
		if (strHeader == _T("TITLEBOLD")) m_strISBOLD = strData;
		if (strHeader == _T("LINEHEIGHT")) m_nHeight = _ttoi(strData);
		
		if (strHeader == _T("FONT_SIZE")) m_nFontSize = _ttoi(strData);
		if (strHeader == _T("FONTCOLOR_R")) m_nFONTCOLOR_R = _ttoi(strData);
		if (strHeader == _T("FONTCOLOR_G")) m_nFONTCOLOR_G = _ttoi(strData);
		if (strHeader == _T("FONTCOLOR_B")) m_nFONTCOLOR_B = _ttoi(strData);
		if (strHeader == _T("FONT_lfHeight")) m_fFont.lfHeight = _ttoi(strData);
		if (strHeader == _T("FONT_lfWidth")) m_fFont.lfWidth = _ttoi(strData);
		if (strHeader == _T("FONT_lfEscapement")) m_fFont.lfEscapement = _ttoi(strData);
		if (strHeader == _T("FONT_lfOrientation")) m_fFont.lfOrientation = _ttoi(strData);
		if (strHeader == _T("FONT_lfWeight")) m_fFont.lfWeight = _ttoi(strData);
		if (strHeader == _T("FONT_lfItalic")) m_fFont.lfItalic = _ttoi(strData);
		if (strHeader == _T("FONT_lfUnderline")) m_fFont.lfUnderline = _ttoi(strData);
		if (strHeader == _T("FONT_lfStrikeOut")) m_fFont.lfStrikeOut = _ttoi(strData);
		if (strHeader == _T("FONT_lfCharSet")) m_fFont.lfCharSet = _ttoi(strData);
		if (strHeader == _T("FONT_lfOutPrecision")) m_fFont.lfOutPrecision = _ttoi(strData);
		if (strHeader == _T("FONT_lfClipPrecision")) m_fFont.lfClipPrecision = _ttoi(strData);
		if (strHeader == _T("FONT_lfQuality")) m_fFont.lfQuality = _ttoi(strData);
		if (strHeader == _T("FONT_lfPitchAndFamily")) m_fFont.lfPitchAndFamily = _ttoi(strData);
		if (strHeader == _T("FONT_lfFaceName")) _stprintf(m_fFont.lfFaceName, _T("%s"), strData);
		
		if (strHeader == _T("LINECOLOR_R")) m_nLINECOLOR_R = _ttoi(strData);
		if (strHeader == _T("LINECOLOR_G")) m_nLINECOLOR_G = _ttoi(strData);
		if (strHeader == _T("LINECOLOR_B")) m_nLINECOLOR_B = _ttoi(strData);
		
		if (strHeader == _T("BACKCOLOR_R")) m_nBACKCOLOR_R = _ttoi(strData);
		if (strHeader == _T("BACKCOLOR_G")) m_nBACKCOLOR_G = _ttoi(strData);
		if (strHeader == _T("BACKCOLOR_B")) m_nBACKCOLOR_B = _ttoi(strData);
		
		return 0;
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::DisplayLine(INT nRowIdx)
{
	EFS_BEGIN
		
	CESL_Mgr *pMgr = (CESL_Mgr *)ELEMENT->WorkWnd;
	CESL_GridData *pData = (CESL_GridData *)m_pData;
	if (pData == NULL) return -1;
	pData->m_fpEventHandler = m_fpEventHandler;
	pData->m_pOwnerWnd = ELEMENT->WorkWnd;
	pData->m_strGridAlias = ELEMENT->CTRL_ALIAS;
	
	InitColumnInfo();
	CESL_GridColumnInfo *parrColumnInfo[64];
	INT nColumns = m_arrColumnInfo.GetCount();
	if (nColumns == 0) return 0;
	CString strDMAlias = _T("");
	INT ids, i, j;
	POSITION pos;
	for (i = 0; i < nColumns; i++) {
		pos = m_arrColumnInfo.FindIndex(i);
		if (pos == NULL) continue;
		parrColumnInfo[i] = (CESL_GridColumnInfo*)m_arrColumnInfo.GetAt(pos);
		if (strDMAlias.GetLength() == 0) strDMAlias = parrColumnInfo[i]->strColumnDMAlias;	
	}
	if (strDMAlias.GetLength() == 0) return 0;
	CESL_DataMgr *pDM = pMgr->FindDM(strDMAlias);
	if (pDM == NULL) return 0;
	INT nRows = pDM->GetRowCount();
	m_nRealRowCount = nRows;
	if (nRowIdx == -1) nRowIdx = nRows-1;
	if (nRowIdx > nRows-1) return 0;
	if (GetRows() <= nRowIdx+1) {
		if (nRows == 0) SetRows(2);
		else SetRows(nRows+1); 
	}
	
	CString strSelected;
	if (m_bProtectSelectColumn == TRUE) strSelected = GetTextMatrix(nRowIdx+1, m_nSelectIdx+1);
	CString strData;
	for (j = 0; j < nColumns; j++) {
		if (parrColumnInfo[j]->nColumnPos < 0) continue;
		strData = _T("");
		if (parrColumnInfo[j]->nColumnPos == 1000000) {
			ids = pDM->GetCellData(parrColumnInfo[j]->strColumnDMFieldAlias, nRowIdx, strData);
		} else {
			strData = pDM->GetCellData(nRowIdx, parrColumnInfo[j]->nColumnPos);
		}
		pData->GridDisplayEventProc(nRowIdx, j, parrColumnInfo[j]->strColumnDMFieldAlias, strData);
		SetTextMatrix(nRowIdx+1, j+1, strData);
	}						

	CString strRowNum;
	strRowNum.Format(_T("%d"), nRowIdx+1);
	SetTextMatrix(nRowIdx+1, 0, strRowNum);

	if (m_bProtectSelectColumn == TRUE) SetTextMatrix(nRowIdx+1, m_nSelectIdx+1, strSelected);
	return 0;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::SetSort(BOOL bFlag)
{
	EFS_BEGIN
		
		m_bGRIDSORT = bFlag;
	return 0;
	
	EFS_END
		return -1;
	
}

INT CESL_Grid2::RefreshSelectedRows(VOID)
{
	EFS_BEGIN
		
		if (m_bCheck != TRUE) return -1;
		
		m_arrSelectedRows.RemoveAll();
		INT nRows = GetRows();
		CString str;
		for (INT i = 1; i < nRows; i++) {
			str = GetTextMatrix(i, m_nSelectIdx+1);
			if (str.GetLength() > 0) m_arrSelectedRows.Add(i);
		}
		return 0;
		
		EFS_END
			return -1;
		
}

INT CESL_Grid2::ReshowSelectedRows(VOID)
{
	EFS_BEGIN
		
		if (m_bCheck != TRUE) return -1;
		
		INT nRows = GetRows();
		INT nData = m_arrSelectedRows.GetSize();
		
		INT nRowIdx;
		for (INT i = 0; i < nData; i++) {
			nRowIdx = m_arrSelectedRows.GetAt(i);
			if (nRowIdx < nRows) SetTextMatrix(nRowIdx, m_nSelectIdx+1, _T("V")); 
		}
		return 0;
		
		EFS_END
			return -1;
		
}

VOID CESL_Grid2::SetProtectSelectColumn(BOOL bFlag)
{
	EFS_BEGIN
		
		m_bProtectSelectColumn = bFlag;	
	
	EFS_END
}

INT CESL_Grid2::SELEDITOR_AddColumn(UINT nColumn)
{
	m_arrSelEditor.Add(nColumn);
	return 0;
}

INT CESL_Grid2::SELEDITOR_RemoveColumn(UINT nColumn)
{
	INT nArrayIdx = SELEDITOR_FindColumn(nColumn);
	if (nArrayIdx < 0) return -1;
	m_arrSelEditor.RemoveAt(nArrayIdx);
	
	return 0;
}

INT CESL_Grid2::SELEDITOR_FindColumn(UINT nColumn)
{
	INT nDatas = m_arrSelEditor.GetSize();
	UINT nData;
	for (INT i = 0; i < nDatas; i++) {
		nData = m_arrSelEditor.GetAt(i);
		if (nData == nColumn) return i;
	}
	return -1;
}

INT CESL_Grid2::SELEDITOR_RemoveAllColumns(VOID)
{
	m_arrSelEditor.RemoveAll();
	return 0;
}

INT CESL_Grid2::SELEDITOR_GetColumnCount(VOID)
{
	return m_arrSelEditor.GetSize();
}

INT CESL_Grid2::ReadExcelExportConfigFile(short &title_size, short &title_bk, short &title_fk, bool &title_bold, bool &title_italic, short &data_size, short &data_bk, short &data_fk, bool &data_bold, bool &data_italic)
{
	CStdioFile fd;
	
	title_size = 9;
	title_bk = 0;
	title_fk = 1;
	title_bold = FALSE;
	title_italic = FALSE;
	data_size = 9;
	data_bk = 0;
	data_fk = 1;
	data_bold = FALSE;
	data_italic = FALSE;
	
	if (fd.Open(_T("..\\CFG\\GridExcelExport.CFG"), CStdioFile::modeRead|CFile::typeBinary) == FALSE) return 0;
	
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	CString strLine, strTag, strData;
	INT nPos;
	while(TRUE) {
		if (fd.ReadString(strLine) == FALSE) break;
		strLine.TrimLeft();			strLine.TrimRight();
		if (strLine.GetLength() == 0) continue;
		if (strLine.GetAt(0) == '#') continue;
		if (strLine.GetAt(0) == '.') continue;
		
		nPos = strLine.Find('=', 0);
		if (nPos == -1) continue;
		strTag = strLine.Mid(0, nPos);
		strTag.TrimLeft();			strTag.TrimRight();
		strTag.MakeUpper();
		strData = strLine.Mid(nPos+1);
		strData.TrimLeft();			strData.TrimRight();
		
		if (strTag.GetLength() == 0) continue;
		if (strData.GetLength() == 0) continue;
		
		if (strTag == _T("TITLE_FONT_SIZE")) title_size = _ttoi(strData);
		if (strTag == _T("TITLE_BK_COLOR")) title_bk = _ttoi(strData);
		if (strTag == _T("TITLE_FK_COLOR")) title_fk = _ttoi(strData);
		if (strTag == _T("TITLE_BOLD")) {
			strData.MakeUpper();
			if (strData == _T("TRUE")) title_bold = TRUE;
			else title_bold = FALSE;
		}
		if (strTag == _T("TITLE_ITALIC")) {
			strData.MakeUpper();
			if (strData == _T("TRUE")) title_italic = TRUE;
			else title_italic = FALSE;
		}
		
		if (strTag == _T("DATA_FONT_SIZE")) data_size = _ttoi(strData);
		if (strTag == _T("DATA_BK_COLOR")) data_bk = _ttoi(strData);
		if (strTag == _T("DATA_FK_COLOR")) data_fk = _ttoi(strData);
		if (strTag == _T("DATA_BOLD")) {
			strData.MakeUpper();
			if (strData == _T("TRUE")) data_bold = TRUE;
			else data_bold = FALSE;
		}
		if (strTag == _T("DATA_ITALIC")) {
			strData.MakeUpper();
			if (strData == _T("TRUE")) data_italic = TRUE;
			else data_italic = FALSE;
		}
	}
	
	fd.Close();
	
	return 0;
}