// MarcGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Font.h"
#include "MarcGridCtrl.h"
#include "MarcEditor.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcGridCtrl

CMarcGridCtrl::CMarcGridCtrl()
{
	m_pMarc = NULL;
	m_pMarcMgr = NULL;
	m_pGridEdit = NULL;
	m_pGridCombo = NULL;

	m_pParentEditor = NULL;


	m_nSelectedRow = -1;
	m_nSelectedCol = -1;

	m_nReversedRow = -1;

	m_nMode = NORMAL_MODE;

	m_strCustom = _T("사용자정의");

	m_nArrColWidth[0] = 120;
	m_nArrColWidth[1] = 60;
	m_nArrColWidth[2] = 200;
}

CMarcGridCtrl::~CMarcGridCtrl()
{
	if (m_pGridEdit != NULL) delete m_pGridEdit;
	if (m_pGridCombo != NULL) delete m_pGridCombo;
}

BEGIN_MESSAGE_MAP(CMarcGridCtrl, CWnd)
	//{{AFX_MSG_MAP(CMarcGridCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_MESSAGE(WM_GRID_EDIT_MODIFY, OnGridEditChange)
	ON_MESSAGE(WM_GRID_COMBO_MODIFY, OnGridComboChange)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcGridCtrl Operations

VOID CMarcGridCtrl::Init(CMarcMgr* pMarcMgr, CMarc* pMarc)
{
	EFS_BEGIN

	SetMarcMgr(pMarcMgr);
	SetMarc(pMarc);

	//InitUserAlias();
	Clear();
	SetRows(2);
	SetGridFormat();

	EFS_END
}

INT CMarcGridCtrl::ApplyAutoIndicator(CString &strKeyValue, CString strText)
{
	EFS_BEGIN

	if (strKeyValue < 5) return -1;

	CStringArray StringArray;
	
	StringArray.Add(_T("245$a=(?20:10"));
	StringArray.Add(_T("440$a=(?10:00"));

	CString strKey;
	CString strCondition;
	CString strThen;
	CString strElse;
	CString strResult;
	CString strOriginKeyValue = strKeyValue;
	strKeyValue = strKeyValue.Left(5);


	INT nCount = StringArray.GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		_stscanf(StringArray.GetAt(idx).GetBuffer(0), _T("%[^=]=%[^?]?%[^:]:%s"), 
			strKey.GetBuffer(MAX_PATH), strCondition.GetBuffer(MAX_PATH), strThen.GetBuffer(MAX_PATH), strElse.GetBuffer(MAX_PATH));

		strKey.ReleaseBuffer();
		strCondition.ReleaseBuffer();
		strThen.ReleaseBuffer();
		strElse.ReleaseBuffer();

		if (strKey != strKeyValue) continue;

		if (strText.Find(strCondition) < 0)
		{
			strResult.Format(_T("%s!%s"), strKeyValue, strElse);
		}
		else
		{
			strResult.Format(_T("%s!%s"), strKeyValue, strThen);
		}

		strKeyValue = strResult;
		return 0;
	}

	strKeyValue = strOriginKeyValue;
	
	return 0;

	EFS_END
	return -1;

}

VOID CMarcGridCtrl::SetGridColWidth(INT nAliasWidth /*= -1*/, INT nColWidth /*= -1*/, INT nDataWidth /*= -1*/)
{
	EFS_BEGIN

	if (nAliasWidth != -1)
		m_nArrColWidth[0] = nAliasWidth;
	if (nColWidth != -1)
		m_nArrColWidth[1] = nColWidth;
	if (nDataWidth != -1)
		m_nArrColWidth[2] = nDataWidth;

	EFS_END
}

VOID CMarcGridCtrl::SetGridFormat()
{
	EFS_BEGIN

	SetCols(0, 4);
	SetRowHeight(-1, 350);
	SetColWidth(0, 0, 0);

	SetColumnInfo(0, _T("설명"), m_nArrColWidth[0], 1);
	SetColumnInfo(1, _T("기호"), m_nArrColWidth[1], 1);
	SetColumnInfo(2, _T("내용"), m_nArrColWidth[2], 1);

	// 일반 사용 모드
	if (m_nMode == NORMAL_MODE)
	{
		SetColWidth(2, 0, 0);
	}
	// 기본값 설정 모드
	else if (m_nMode == SETUP_MODE)
	{
			
	}

	EFS_END
}

VOID CMarcGridCtrl::Display()
{
	EFS_BEGIN

	INT ids;

	Clear();
	SetRows(2);

	SetGridFormat();

	CArray<CString, CString&> arrayItem;
	
	INT nCount = m_arrayUserAlias.GetSize();
	if (nCount == 0) return;

	// SortUserAlias
	//SortUserAlias((CStringArray*)&m_arrayUserAlias);

	INT idx = 0;
	INT n = 0;
	INT nArraySize = 0;
	CString strArr[MAX_PATH][3];
	CString strKeyValue;
	CString strItemData;
	for (INT i = 0; i < nCount; i++)
	{
		arrayItem.RemoveAll();

		if (!m_pMarcMgr->GetKeyValueFromAlias(m_arrayUserAlias.GetAt(i), strKeyValue))
			continue;

		ids = m_pMarcMgr->GetItem(m_pMarc, m_arrayUserAlias.GetAt(i), strItemData, &arrayItem);
		if (ids > 0)
		{
			nArraySize = arrayItem.GetSize();
			for (idx = 0; idx < nArraySize; idx++)
			{
				strItemData = arrayItem.GetAt(idx);
				strItemData.TrimLeft();
				strItemData.TrimRight();

				strArr[n][0] = m_arrayUserAlias.GetAt(i);
				strArr[n][1] = strKeyValue;
				strArr[n][2] = strItemData;

				n++;
			}
			
		}
		else
		{
			strArr[n][0] = m_arrayUserAlias.GetAt(i);
			strArr[n][1] = strKeyValue;
			n++;
		}	
	}

	// Display
	SetRows(n + 2);
	for (idx = 0; idx < n; idx++)
	{
		SetTextMatrix(idx + 1, 1, strArr[idx][0]);
		SetTextMatrix(idx + 1, 2, strArr[idx][1]);
		SetTextMatrix(idx + 1, 3, strArr[idx][2]);
	}

	SetGridFormat();


	/*
	CString strKeyValue;
	CString strItemData;
	for (INT i = 0; i < nCount; i++)
	{
		arrayItem.RemoveAll();
		strKeyValue.Empty();
		strItemData.Empty();
		
		if (!m_pMarcMgr->GetKeyValueFromAlias(m_arrayUserAlias.GetAt(i), strKeyValue))
			continue;

		ids = m_pMarcMgr->GetItem(m_pMarc, m_arrayUserAlias.GetAt(i), strItemData, &arrayItem);
		if (ids > 0)
		{
			for (INT idx = 0; idx < arrayItem.GetSize(); idx++)
			{
				strItemData = arrayItem.GetAt(idx);
				strItemData.TrimLeft();
				strItemData.TrimRight();

				SetTextMatrix(GetRows() - 1, 1, m_arrayUserAlias.GetAt(i));
				SetTextMatrix(GetRows() - 1, 2, strKeyValue);
				SetTextMatrix(GetRows() - 1, 3, strItemData);
				SetRows(GetRows() + 1);
			}
		}
		else
		{
			SetTextMatrix(GetRows() - 1, 1, m_arrayUserAlias.GetAt(i));
			SetTextMatrix(GetRows() - 1, 2, strKeyValue);
			SetRows(GetRows() + 1);
		}
	}
	*/

	EFS_END
}

INT CMarcGridCtrl::SetColumnInfo(INT nCol, CString strTitle, INT nWidth, INT nAlignment)
{
	EFS_BEGIN

	if ( nCol > GetCols(0)-2 ) return -1;

	SetColAlignmentFixed(nCol + 1, 4);
	SetTextMatrix(0, nCol + 1, strTitle);
	SetColWidth(nCol + 1, 0, nWidth * 15);
	SetColAlignment(nCol + 1, nAlignment);
	
	return 0;

	EFS_END
	return -1;

}

VOID CMarcGridCtrl::InitUserAlias()
{
	EFS_BEGIN

	m_arrayUserAlias.RemoveAll();

	EFS_END
}

VOID CMarcGridCtrl::AddUserAlias(CString strAlias)
{
	EFS_BEGIN

	// 기존에 있는 alias이면 추가하지 않는다.
	INT nCount = m_arrayUserAlias.GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		if (strAlias == m_arrayUserAlias.GetAt(idx)) return;
	}

	m_arrayUserAlias.Add(strAlias);

	EFS_END
}

INT CMarcGridCtrl::SortUserAlias(CStringArray *pStringArray)
{
	EFS_BEGIN

	if (pStringArray == NULL) return -1;

	INT nCount = pStringArray->GetSize();

	CString (*pStr)[2] = new CString[nCount][2];

	// KOL.UDF.022
	CString strAlias = _T("");
	CString strKeyValue = _T("   ");

	INT n = 0;

	for (INT idx = 0; idx < nCount; idx++)
	{
		strAlias = pStringArray->GetAt(idx);
		if (strAlias.IsEmpty()) continue;

		m_pMarcMgr->m_mapMarcAlias.Lookup(strAlias, strKeyValue);
		if (strKeyValue.GetLength() < 4) continue;

		pStr[idx][0] = strAlias;
		pStr[idx][1] = strKeyValue.Left(3);

		n++;
	}

	CString strTemp;
	for (idx = 0; idx < n - 1; idx++)
	{
		for (INT j = idx + 1; j < n; j++)
		{
			if (_ttoi(pStr[idx][1]) > _ttoi(pStr[j][1]))
			{
				// Swap
				strTemp = pStr[j][0];
				pStr[j][0] = pStr[idx][0];
				pStr[idx][0] = strTemp;

				strTemp = pStr[j][1];
				pStr[j][1] = pStr[idx][1];
				pStr[idx][1] = strTemp;

			}
		}
	}

	// 다시 넣어준다.
	pStringArray->RemoveAll();
	for (idx = 0; idx < n; idx++)
	{
		pStringArray->Add(pStr[idx][0]);
	}

	if (pStr)
		delete [] pStr;

	
	return 0;

	EFS_END
	return -1;

}

VOID CMarcGridCtrl::InitLimitTag()
{
	EFS_BEGIN

	m_arrayLimitTag.RemoveAll();

	EFS_END
}

VOID CMarcGridCtrl::AddLimitTag(CString strTagCode)
{
	EFS_BEGIN

	m_arrayLimitTag.Add(strTagCode);

	EFS_END
}

INT CMarcGridCtrl::SetReverse(INT nRow)
{
	EFS_BEGIN

	COLORREF blue, white, black;
	blue = RGB(228,243,251);
	white = RGB(255,255,255);	
	black = RGB(0,0,0);

	INT i;
    INT nCol = GetCol();
	INT nCols = GetCols(0);
	
	if ( m_nReversedRow >= 0 && m_nReversedRow < GetRows()) {
		SetRow(m_nReversedRow);
		for ( i = 1 ; i < nCols ; i++ ) {
			SetCol(i);
			SetCellBackColor(white);
			SetCellForeColor(black);
		}
	}
	if ( nRow >= 0 ) {
		SetRow(nRow);
		for ( i = 1 ; i < nCols ; i++ ) {
			SetCol(i);
			SetCellBackColor(blue);
			SetCellForeColor(black);
		}
		m_nReversedRow = nRow;
	}

    SetCol(nCol);

	return 0;

	EFS_END
	return -1;

}

VOID CMarcGridCtrl::ShowControl(CGridEdit::EditMode nEditMode)
{
	EFS_BEGIN

	INT nRow = GetRow();
	INT nCol = GetCol();

	if (nRow < 0 || nCol < 0) return;

	if (nCol == 1 && nRow == GetRows() - 1)
	{
		ShowComboBox(nRow, nCol);
	}	
	if (nCol == 3 && nRow != GetRows() - 1)
	{
		ShowEditBox(nRow, nCol, nEditMode);
	}
	if (nCol == 2 && GetTextMatrix(nRow, 1) == m_strCustom)
	{
		ShowEditBox(nRow, nCol, nEditMode);
	}

	EFS_END
}

VOID CMarcGridCtrl::ShowComboBox(INT nRow, INT nCol)
{
	EFS_BEGIN

	if (m_pMarcMgr == NULL) return;

	POSITION pos = m_pMarcMgr->m_mapMarcAlias.GetStartPosition();
	if (pos == NULL) return;

	if (m_pGridCombo == NULL) return;
	
	CRect rect;
	GetCellRect(rect);
	m_pGridCombo->MoveWindow(rect);
	m_pGridCombo->ResetContent();
		
	CStringArray StringArray;
	
	while (pos != NULL)
	{
		CString strAlias;
		CString strKeyValue;

		m_pMarcMgr->m_mapMarcAlias.GetNextAssoc(pos, strAlias, strKeyValue);

		//if (!IsLimitTag(strKeyValue.Left(3)))
		if (!IsLimitTag(strKeyValue))
			StringArray.Add(strAlias);
	}

	SortUserAlias(&StringArray);
	INT nCount = StringArray.GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		m_pGridCombo->InsertString(idx, StringArray.GetAt(idx));
		m_pGridCombo->SetItemHeight(idx, rect.Height());
	}

	m_pGridCombo->SetCurSel(0);
	
	m_pGridCombo->ShowDropDown();
	m_pGridCombo->Invalidate();
	m_pGridCombo->ShowWindow(SW_SHOW);
	m_pGridCombo->SetFocus();

	m_nSelectedRow = nRow;
	m_nSelectedCol = nCol;

	EFS_END
}

VOID CMarcGridCtrl::ShowEditBox(INT nRow, INT nCol, CGridEdit::EditMode nEditMode)
{
	EFS_BEGIN

	if (m_pGridEdit == NULL) return;

	CRect rect;
	GetCellRect(rect);

	CString strEditItem = GetTextMatrix(nRow, 1);
	if ( strEditItem == "낱권ISBN" || strEditItem == "낱권ISBN부가기호") return;

	m_pGridEdit->MoveWindow(rect);

	m_pGridEdit->m_nEditMode = nEditMode;
	
	m_pGridEdit->ShowWindow(SW_SHOW);
	m_pGridEdit->SetFocus();
	m_pGridEdit->BringWindowToTop();

	// 텍스트 설정
	if (nEditMode == CGridEdit::editCustom)
		m_pGridEdit->SetWindowText(GetTextMatrix(nRow, nCol));
	else
		m_pGridEdit->SetWindowText(NULL);	

	m_nSelectedRow = nRow;
	m_nSelectedCol = nCol;

	EFS_END
}

VOID CMarcGridCtrl::GetCellRect(CRect &rect)
{
	EFS_BEGIN

	CRect gridRect;
	CRect parentRect;
	GetWindowRect(&gridRect);
	GetParent()->GetClientRect(&parentRect);

	::ClientToScreen(GetParent()->m_hWnd,(POINT*)&(parentRect.left));

	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;

	rect.left	= gridRect.left	+ (long)((float)GetCellLeft() / 15.0f);
	rect.top	= gridRect.top	+ (long)((float)GetCellTop() / 15.0f);
	rect.right	= rect.left		+ (long)((float)GetCellWidth() / 15.0f) - 1;
	rect.bottom = rect.top		+ (long)((float)GetCellHeight() / 15.0f) - 1;

	EFS_END
}

BOOL CMarcGridCtrl::IsLimitTag(CString strTagCode)
{
	EFS_BEGIN

	// strTagCode = strKeyValue : 지시기호때문에..

	INT nCount = m_arrayLimitTag.GetSize();
	if (nCount == 0) return FALSE;

	// !가 있으면 지시기호까지 체크한다.
	CString strInd;
	INT nFind = strTagCode.Find(_T("!"));
	if (nFind >= 0)
	{
		strInd = strTagCode.Mid(nFind + 1);
		strTagCode = strTagCode.Left(3);
	}
	else
		strTagCode = strTagCode.Left(3);

	for (INT i = 0; i < nCount; i++)
	{
		if (!strInd.IsEmpty())
		{
			if (strTagCode + strInd == m_arrayLimitTag.GetAt(i))
				return TRUE;
		}
		else
		{
			if (strTagCode == m_arrayLimitTag.GetAt(i))
			return TRUE;
		}
	}

	return FALSE;

	EFS_END
	return FALSE;

}

INT CMarcGridCtrl::GetCurrentUserMarcAlias(CStringArray *pArrayMarcAlias)
{
	EFS_BEGIN

	if (pArrayMarcAlias == NULL) return -1;

	INT nCount = GetRows();

	CString strText;
	for (INT i = 1; i < nCount; i++)
	{
		strText = GetTextMatrix(i, 1);
		if (strText.IsEmpty() || strText == m_strCustom) continue;
		pArrayMarcAlias->Add(strText);
	}

	return 0;

	EFS_END
	return -1;

}

INT CMarcGridCtrl::MakeDefaultMarc()
{
	if (!m_pMarcMgr || !m_pMarc) return -1;

	m_pMarc->Init();

	// 현재 있는 keyvalue와 데이터를 가지고 마크를 만든다.

	CString strKeyValue;
	CString strItemData;

	INT nRows = GetRows();
	for (INT nRow = 0; nRow < nRows; nRow++)
	{
		strKeyValue = GetTextMatrix(nRow, 2);
		strItemData = GetTextMatrix(nRow, 3);

		if (strKeyValue.IsEmpty()) continue;

		// 자동 지시기호를 적용한다.
		ApplyAutoIndicator(strKeyValue, strItemData);

		// 마크에 데이터를 넣는다.
		m_pMarcMgr->SetItem(m_pMarc, strKeyValue, strItemData);
	}

	// 자동 지시기호 식별기호 생성을 위해 신텍스를 한번 체크한다.
	m_pMarcMgr->CheckSimpleSyntax(m_pMarc);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMarcGridCtrl message handlers

VOID CMarcGridCtrl::PreSubclassWindow() 
{
	EFS_BEGIN

	// 콤보 박스 생성
	if (m_pGridCombo != NULL)
	{
		delete m_pGridCombo;
		m_pGridCombo = NULL;
	}

	m_pGridCombo = new CGridCombo;
	m_pGridCombo->Create(WS_CHILD | WS_VISIBLE| WS_VSCROLL| CBS_DROPDOWNLIST | CBS_AUTOHSCROLL, CRect(1, 1, 500, 300), GetParent(), IDC_COMBO_ALIAS);
	m_pGridCombo->parent = this;
	m_pGridCombo->ShowWindow(SW_HIDE);

	// 에디트 박스 생성
	if (m_pGridEdit != NULL)
	{
		delete m_pGridEdit;
		m_pGridEdit = NULL;
	}

	// 에디트 박스를 생성한다.
	m_pGridEdit = new CGridEdit;
	m_pGridEdit->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE, CRect(0, 0, 0, 0), GetParent(), IDC_EDIT_DATA);
	m_pGridEdit->parent = this;
	m_pGridEdit->ShowWindow(SW_HIDE);	
	m_Font.CreatePointFont(90, _T("Microsoft Sans Serif")); 
	m_pGridEdit->SetFont(&m_Font); 	

	CWnd::PreSubclassWindow();

	EFS_END
}

BOOL CMarcGridCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	EFS_BEGIN

	INT nRowsScrolled = 1 * zDelta / 120;

    if (nRowsScrolled >= 0)
        for (INT i = 0; i < nRowsScrolled; i++)
           this->PostMessage(WM_VSCROLL, SB_LINEUP, 0);
    else
        for (INT i = 0; i > nRowsScrolled; i--)
			this->PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);

	return CMSHFlexGrid::OnMouseWheel(nFlags, zDelta, pt);

	EFS_END
	return FALSE;
}

VOID CMarcGridCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	CMSHFlexGrid::OnLButtonDown(nFlags, point);
	SetReverse(GetRow());	

	EFS_END
}

VOID CMarcGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	CMSHFlexGrid::OnLButtonDblClk(nFlags, point);

	ShowControl(CGridEdit::editCustom);

	EFS_END
}

VOID CMarcGridCtrl::OnGridEditChange(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	if (m_pGridEdit == NULL) return;
	if (!::IsWindowVisible(m_pGridEdit->GetSafeHwnd())) return;

	if ( wParam == 1 ) 
	{
		CString strText;
		
		m_pGridEdit->GetWindowText(strText);
		
		CString strKeyValue = GetTextMatrix(m_nSelectedRow, 2);
	
		if (m_nSelectedCol == 2)
		{
			SetTextMatrix(m_nSelectedRow, m_nSelectedCol, strText);
		}
		if (m_nSelectedCol == 3)
		{
			if (m_nMode == NORMAL_MODE)
			{
				//if (IsLimitTag(strKeyValue.Left(3)))
				if (IsLimitTag(strKeyValue))
					goto END;
				
				CString strOldItemData = GetTextMatrix(m_nSelectedRow, 3);
				if (strText == strOldItemData)
					goto END;

				ApplyAutoIndicator(strKeyValue, strText);
				/*
				if (m_pMarcMgr->SetItem(m_pMarc, strKeyValue, strText, strOldItemData) >= 0)
				{
					SetTextMatrix(m_nSelectedRow, m_nSelectedCol, strText);

					if (m_pParentEditor != NULL)
					{					
						((CMarcEditor*)m_pParentEditor)->UpdateEditCtrls(GRID_CTRL);
					}
				}
				*/
				if ( strKeyValue == _T("245$d") ) 
				{
					/*
					INT nPos;
					INT nPrevPos;

					nPos = 0;
					nPrevPos = 0;
					for( i=0; true; i++)
					{
						strData1.Empty();
						pos = strText.Mid(nPrevPos).FindOneOf(_T(",;"));
						if( pos == -1 )		strData1 = strText.Mid(nPrevPos);
						else strData1 = strText.Mid(nPrevPos, pos );
						
						if ( nPrevPos > 0 ) 
						{
							strPunc = strText.Mid(nPrevPos-1, 1 );
							if ( strPunc.FindOneOf(_T(",;")) < 0 ) 
							{
								strPunc = _T("");				
							}
						}
						else
						{
							strPunc = _T("");
						}
						
						nPrevPos = pos+nPrevPos+1;
						if( i==0 )
						{
							pMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$d"), strData1);
						}
						else
						{
							strData1.TrimLeft();	strData1.TrimRight();
							if( strData1.GetLength() > 0 )	pMgr->m_pInfo->pMarcMgr->SetItemWithFunc(pMarc, _T("245$e"), strData1, strPunc, _T(""), ADD_DEFAULT);
						}
						if( pos == -1 )		break;
					}
					*/					
					BOOL bEqual = FALSE;
					CString s245d;
					CString strPunc;
					INT ids;
					// 2008.04.21 UPDATE BY PDJ
					// ,는 그리드편집차에서 구두점으로 처리하지 않음
					// 강부장님 지시
					//int iPos = strText.FindOneOf( ",;" );
					int iPos = strText.FindOneOf( _T(";") );
					s245d = _T("");
					if ( iPos > 0 ) { 
						s245d = strText.Left(iPos);
						strPunc = strText.Mid(iPos, 1 );
						ids = m_pMarcMgr->SetItem(m_pMarc, _T("245$d"), s245d, strOldItemData );
						CString s245e = strText.Mid(iPos+1);
						//20080516 UPDATE PJW : ";" 만 검색하도록 변경
						//INT iPosE = s245e.FindOneOf( _T(",;") );
						INT iPosE = s245e.FindOneOf( _T(";") );
						CString s245Right = s245e;
						while ( iPosE >= 0 ) {						
							s245e = s245Right.Left(iPosE);
							m_pMarcMgr->SetItemWithFunc(m_pMarc, _T("245$e"), s245e, strPunc );
							strPunc = s245Right.Mid(iPosE, 1 );
							s245Right = s245Right.Mid(iPosE+1);
							/*******************************************************
							 *******************************************************/
							//20080103 UPDATE PJW : "," ";" 둘다 검색하도록 변경
							//20080516 UPDATE PJW : ";" 만 검색하도록 변경
							//iPosE = s245Right.Find( _T(";") );	
							iPosE = s245Right.Find( _T(";") );	
							/*******************************************************
							 *******************************************************/
														
						}
						m_pMarcMgr->SetItemWithFunc(m_pMarc, _T("245$e"), s245Right, strPunc );
					}
					else
					{
						ids = m_pMarcMgr->SetItem(m_pMarc, _T("245$d"), strText, strOldItemData );
						s245d = strText;												
					}
					
					//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
					
					//if (m_pMarcMgr->SetItem(m_pMarc, strKeyValue, s245d, strOldItemData) >= 0)
					
					//if ( m_pMarcMgr->SetItem(m_pMarc, _T("245$e"), _T("dsf") ) >= 0 ) {
					SetTextMatrix(m_nSelectedRow, m_nSelectedCol, s245d);

					if (m_pParentEditor != NULL)
					{					
						((CMarcEditor*)m_pParentEditor)->UpdateEditCtrls(GRID_CTRL);
					}
					//}
				}
				else {
					if (m_pMarcMgr->SetItem(m_pMarc, strKeyValue, strText, strOldItemData) >= 0)
					{
						CArray<CString, CString&> array202;
						CString sItem;
						m_pMarcMgr->GetField( m_pMarc, _T("020"), sItem, &array202 );
						m_pMarcMgr->DeleteField( m_pMarc, _T("020") );
						CString sGt, sTt;
						sGt.Format( _T("%c"), 31 );
						sTt.Format( _T("%c"), 30 );
						CString s020a, s020g;
						for ( INT i = 0 ; i < array202.GetSize() ; i++ ) {
							if ( array202.GetAt(i).Left(1) == _T(" ") ) {
								if ( array202.GetAt(i).GetLength() != 8 ) {
									m_pMarcMgr->InsertField( m_pMarc, _T("020") + array202.GetAt(i) );
								}
							}
							else {								
								sItem = array202.GetAt(i);
								CString s020Ori = sItem;								
								INT iPos = sItem.Find( _T("a") );
								if ( iPos > -1 ) {
									sItem = sItem.Mid(iPos+1);
									iPos = sItem.Find( sGt );
									if ( iPos < 0 ) {
										iPos = sItem.Find( sTt );
									}
									if ( iPos != 0 ) {
										s020a = sItem.Left(iPos);
									}
								}
								sItem = s020Ori;
								iPos = sItem.Find( _T("g") );
								if ( iPos > -1 ) {
									sItem = sItem.Mid(iPos+1);
									iPos = sItem.Find( sGt );
									if ( iPos < 0 ) {
										iPos = sItem.Find( sTt );
									}
									if ( iPos != 0 ) {
										s020g = sItem.Left(iPos);
									}
								}
								//m_pMarcMgr->InsertField( m_pMarc, _T("020") + array202.GetAt(i) );
							}
						}
						CString s0201 = _T("0201 ");
						INT i020Yes = 0;
						if ( s020a != _T("") ) {
							CString s020aOri;
							s020aOri.Format( _T("%sa%s"), sGt, s020a );
							s0201 = s0201 + s020aOri;
							i020Yes = 1;
						}
						if ( s020g != _T("") ) {
							CString s020gOri;
							s020gOri.Format( _T("%sg%s"), sGt, s020g );
							s0201 = s0201 + s020gOri;
							i020Yes = 1;
						}
						if ( i020Yes == 1 ) {
							CString s0201Ori;
							s0201Ori.Format( _T("%s%s"), s0201, sTt );
							m_pMarcMgr->InsertField( m_pMarc, s0201Ori );
						}

						SetTextMatrix(m_nSelectedRow, m_nSelectedCol, strText);

						if (m_pParentEditor != NULL)
						{					
							((CMarcEditor*)m_pParentEditor)->UpdateEditCtrls(GRID_CTRL);
						}
					}
				}
			}
			else if (m_nMode == SETUP_MODE)
			{
				SetTextMatrix(m_nSelectedRow, m_nSelectedCol, strText);
			}
		}
	}

END:
	m_pGridEdit->ShowWindow(SW_HIDE);
	
	SetFocus();
	PostMessage(WM_KEYDOWN, lParam, 0);
	PostMessage(WM_KEYUP, lParam, 0);

	EFS_END
}

VOID CMarcGridCtrl::OnGridComboChange(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	if (m_pGridCombo == NULL) return;
	if (!::IsWindowVisible(m_pGridCombo->GetSafeHwnd())) return;

	if ( wParam == 1 ) 
	{
		CString strText;
		
		m_pGridCombo->GetLBText(m_pGridCombo->GetCurSel(), strText);
		SetTextMatrix(m_nSelectedRow, m_nSelectedCol, strText);
		AddUserAlias(strText);

		CString strKeyValue;
		m_pMarcMgr->GetKeyValueFromAlias(strText, strKeyValue);
		SetTextMatrix(m_nSelectedRow, 2, strKeyValue);
		
		SetRows(GetRows() + 1);
		SetRowHeight(-1, 350);

		SetFocus();

		if (strText == m_strCustom)
		{
			SetCol(2);
			ShowEditBox(m_nSelectedRow, 2, CGridEdit::editCustom);
		}
	}
	else
		SetFocus();

	m_pGridCombo->ShowWindow(SW_HIDE);	

	EFS_END
}

VOID CMarcGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	EFS_BEGIN

	if (m_pMarcMgr != NULL)
	{
		// Delete Key
		if (nChar == VK_DELETE && GetRow() != GetRows() - 1)
		{
			if (m_nMode == NORMAL_MODE)
			{
				CString strKeyValue = GetTextMatrix(GetRow(), 2);
				//if (!IsLimitTag(strKeyValue.Left(3)))
				if (!IsLimitTag(strKeyValue))
				{
					CString strItemData = GetTextMatrix(GetRow(), 3);

					if (!strItemData.IsEmpty())
					{
						if (m_pMarcMgr->DeleteItem(m_pMarc, strKeyValue, strItemData) >= 0)
						{	
							if (m_pParentEditor != NULL)
								((CMarcEditor*)m_pParentEditor)->UpdateEditCtrls(GRID_CTRL);
							RemoveItem(GetRow());
							SetReverse(GetRow());
							return;
						}
					}
					else
					{
						RemoveItem(GetRow());
						SetReverse(GetRow());
						return;
					}
				}
			}
			else if (m_nMode == SETUP_MODE)
			{
				RemoveItem(GetRow());
				SetReverse(GetRow());
				return;
			}
		}
	}

	CMSHFlexGrid::OnKeyDown(nChar, nRepCnt, nFlags);

	EFS_END
}

BOOL CMarcGridCtrl::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN			

	if (pMsg->message == WM_KEYDOWN)
	{
		if ( pMsg->wParam == VK_TAB )
		{
			((CMarcEditor*)m_pParentEditor)->SetEditFocus();
			return TRUE;
		}
		else if ((pMsg->wParam == VK_RETURN || pMsg->wParam == VK_F2)&& GetRow() != GetRows() -1)
		{
			ShowControl(CGridEdit::editCustom);
			return TRUE;
		}

		if ((_TUCHAR)pMsg->wParam >= 0x80 || _istalnum(pMsg->wParam) || pMsg->wParam == 96)
		{
			if (m_pGridEdit)
			{
				m_pGridEdit->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
				ShowControl(CGridEdit::editEasy);
				return TRUE;
			}
		}
	}
	else if (pMsg->message == WM_KEYUP)
	{
		if (pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN)
		{
			SetReverse(GetRow());
		}
	}
	
	return CWnd::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;

}
