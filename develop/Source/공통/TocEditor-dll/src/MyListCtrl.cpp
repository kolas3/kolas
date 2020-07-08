// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include _T("BFO_MAIN.h")
#include "MyListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl

CMyListCtrl::CMyListCtrl()
{
	m_arSortAsc.RemoveAll();
	m_arSortType.RemoveAll();
}

CMyListCtrl::~CMyListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMyListCtrl)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl message handlers

INT CMyListCtrl::AddColumn	(
								TCHAR   *szText	,
								INT		Index	,
								INT		cx		, //=100
								INT		fmt		  //=LVCFMT_LEFT;
							)
{
	LV_COLUMN	LvColumn;

	LvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	LvColumn.fmt		= fmt; //LVCFMT_LEFT;
	LvColumn.iSubItem	= Index;
	LvColumn.cx			= cx;
	LvColumn.pszText	= szText;

	InsertColumn( Index, &LvColumn );

	if (Index>= m_arSortAsc.GetSize())
		SetSortColumns(Index);

	return 0;
}

INT CMyListCtrl::AddItemData(
								TCHAR   *szText	,
								INT		iCol	, 
								INT		iIcon		  
							)
{
	LV_ITEM		LvItem;
	INT			iActualItem;
	INT			Index;

	Index = GetItemCount();
	LvItem.mask     = LVIF_TEXT | (iIcon >= 0 ? LVIF_IMAGE : 0);
	LvItem.iItem    = Index;
	LvItem.iSubItem = iCol;

	LvItem.pszText  = szText;
	LvItem.iImage   = iIcon;

	iActualItem = InsertItem(&LvItem);
	
	if (iCol>= m_arSortAsc.GetSize())
		SetSortColumns(iCol);


	return 0;
}

INT CMyListCtrl::AddItemData(
								TCHAR   *szText	,
								INT		Index	, 
								INT		iCol	, 
								INT		iIcon		  
							)
{
	LV_ITEM		LvItem;
	INT			iActualItem;

	LvItem.mask     = LVIF_TEXT | (iIcon >= 0 ? LVIF_IMAGE : 0);
	LvItem.iItem    = Index;
	LvItem.iSubItem = iCol;

	LvItem.pszText  = szText;
	LvItem.iImage   = iIcon;

	iActualItem = InsertItem(&LvItem);

	if (iCol>= m_arSortAsc.GetSize())
		SetSortColumns(iCol);

	return 0;
}

INT CMyListCtrl::SetItemData1(
								TCHAR   *szText	,
								INT		Index	,
								INT		iCol	, 
								INT		iIcon		  
							)
{
	LV_ITEM		LvItem;

	LvItem.mask     = LVIF_TEXT | (iIcon >= 0 ? LVIF_IMAGE : 0);
	LvItem.iItem    = Index;
	LvItem.iSubItem = iCol;

	LvItem.pszText  = szText;
	LvItem.iImage   = iIcon;

	SetItem(&LvItem);

	if (iCol>= m_arSortAsc.GetSize())
		SetSortColumns(iCol);

	return 0;
}

INT CMyListCtrl::SetItemData1(
								INT     szText	,
								INT		Index	,
								INT		iCol	, 
								INT		iIcon		  
							)
{
	LV_ITEM		LvItem;

	LvItem.mask     = LVIF_TEXT | (iIcon >= 0 ? LVIF_IMAGE : 0);
	LvItem.iItem    = Index;
	LvItem.iSubItem = iCol;

    TCHAR ctmp[256];
    _stprintf( ctmp, _T("%d"), szText );
	LvItem.pszText  = ctmp;
	LvItem.iImage   = iIcon;

	SetItem(&LvItem);

	if (iCol>= m_arSortAsc.GetSize())
		SetSortColumns(iCol);

	return 0;
}

INT CMyListCtrl::SetItemData1(
								TCHAR    szText	,
								INT		Index	,
								INT		iCol	, 
								INT		iIcon		  
							)
{
	LV_ITEM		LvItem;

	LvItem.mask     = LVIF_TEXT | (iIcon >= 0 ? LVIF_IMAGE : 0);
	LvItem.iItem    = Index;
	LvItem.iSubItem = iCol;

    TCHAR    ctmp[256];
    ctmp[0] = szText; ctmp[1] = _T('\0');
	LvItem.pszText  = ctmp;
	LvItem.iImage   = iIcon;

	SetItem(&LvItem);

	if (iCol>= m_arSortAsc.GetSize())
		SetSortColumns(iCol);

	return 0;
}

INT CMyListCtrl::SetStyle	(
								long	Style
							)
{
	long lStyleOld = GetWindowLong( m_hWnd, GWL_STYLE );
	lStyleOld     &= ~(LVS_TYPEMASK);
	lStyleOld     |= Style;
	SetWindowLong( m_hWnd, GWL_STYLE, lStyleOld );

	return 0;
}

INT CMyListCtrl::ChangeIcon(
								INT		Index	,
								INT		iIcon
							)
{
	LV_ITEM		LvItem;

	LvItem.mask     = LVIF_IMAGE;
	LvItem.iItem    = Index;
	LvItem.iSubItem = 0;

	LvItem.iImage   = iIcon;

	SetItem(&LvItem);

	return 0;
}

INT CMyListCtrl::ResetControl()
{
    DeleteAllItems( );
    while( 1 ) {
        if ( !DeleteColumn( 0 ) ) break;
    }

    return 0;
}


INT CMyListCtrl::DeleteControl()
{
    DeleteAllItems( );
    return 0;
}


BOOL CMyListCtrl::SortTextItems(INT nCol, BOOL bAscending, INT cols, INT low, INT high, BOOL bString, BOOL bIsMain/*=TRUE*/)
{
	CString tempHidden;

    if (high == -1) high = GetItemCount()-1;

    INT lo = low;
    INT hi = high;

    if( hi <= lo ) return FALSE;

    CString midItem = GetItemText( (lo+hi)/2, nCol );

	// loop through the list until indices cross
	midItem.MakeLower();	// 1.23
	while( lo <= hi )
	{
		// Find the first element that is greater than or equal to the partition 
		// element starting from the left Index.
		CString lowText = GetItemText(lo, nCol);	// 1.23
		lowText.MakeLower();						// 1.23
		CString highText = GetItemText(hi, nCol);	// 1.23
		highText.MakeLower();						// 1.23

		if( bAscending )
			//while (lo < high  && GetItemText(lo, nCol) < midItem)	// before ver 1.23
			while (lo < high  && Compare(lowText, midItem, bString)<0 )					// 1.23
			{
				++lo;
				lowText = GetItemText(lo, nCol);	// 1.23
				lowText.MakeLower();				// 1.23
			}
		else
			//while (lo < high && GetItemText(lo, nCol) > midItem)	// before ver 1.23
			while (lo < high  && Compare(lowText, midItem, bString)>0)					// 1.23
			{
				++lo;
				lowText = GetItemText(lo, nCol);	// 1.23
				lowText.MakeLower();				// 1.23
			}

		// Find an element that is smaller than or equal to  the partition 
		// element starting from the right Index.
		if( bAscending )
			//while (hi > low && GetItemText(hi, nCol) > midItem)	// before ver 1.23
			while (hi > low && Compare(highText, midItem, bString) >0 )					// 1.23
			{
				--hi;	
				highText = GetItemText(hi, nCol);	// 1.23
				highText.MakeLower();				// 1.23
			}
		else
			//while (hi > low && GetItemText(hi, nCol) < midItem)	// before 1.23
			while (hi > low && Compare(highText,midItem, bString) <0 )					// 1.23
			{
				--hi;
				highText = GetItemText(hi, nCol);	// 1.23
				highText.MakeLower();				// 1.23
			}

		// If the indexes have not crossed, swap if the items are not equal
		if (lo <= hi)
		{
			// swap only if the items are not equal
			if (Compare(GetItemText(lo, nCol) ,GetItemText(hi, nCol), bString)!= 0)
			{
				for (INT col = 0; col < cols; col++)
				{
					CString data= GetItemText(lo, col);
					BOOL bCheck= GetCheck(lo);
					SetItemText(lo, col, GetItemText(hi, col));
					SetCheck(lo, GetCheck(hi));
					SetItemText(hi, col, data);
					SetCheck(hi, bCheck);
				}
			}
			++lo;
			--hi;
		}
	}

	// If the right index has not reached the left side of array
    // must now sort the left partition.
    if( low < hi ) {
		SortTextItems(nCol, bAscending, cols, low, hi, bString, bIsMain );
	}

    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if( lo < high ) {
		SortTextItems(nCol, bAscending, cols, lo, high, bString, bIsMain );
	}

    return TRUE;
}

VOID CMyListCtrl::SortListView(INT nCol, BOOL bIsMain/*=FALSE*/)
{
	INT bAsc= 1;
	BOOL bStringOrNumber;
	for (INT i=0; i<m_arSortAsc.GetSize();i++)
	{
		if (i== nCol)
		{
			bStringOrNumber= m_arSortType.GetAt(i);

			if (m_arSortAsc.GetAt(i)==0)
			{
				for (INT j=0; j<m_arSortAsc.GetSize();j++)
					m_arSortAsc.ElementAt(j)=0;	
				m_arSortAsc.ElementAt(i)=1;
				bAsc=1;
			}
			else
			{
				for (INT j=0; j<m_arSortAsc.GetSize();j++)
					m_arSortAsc.ElementAt(j)=0;	
				m_arSortAsc.ElementAt(i)=0;
				bAsc=0;
			}
			break;
		}
	}

	SortTextItems(nCol, bAsc, m_arSortAsc.GetSize(), 0, -1, bStringOrNumber, bIsMain);

}

VOID CMyListCtrl::SetSortColumns(INT nCols)
{
	m_arSortAsc.RemoveAll();
	for (INT i=0; i<=nCols; i++)
	{
		m_arSortAsc.Add(0);
	}

	m_arSortType.RemoveAll();
	for (i=0; i<=nCols; i++)
	{
		m_arSortType.Add(0);
	}
}

VOID CMyListCtrl::SetSortType(INT idx, INT nType/*0:string 1:INTeger*/)
{
	m_arSortType.ElementAt(idx)= nType;
}

INT CMyListCtrl::Compare(CString str1, CString str2, BOOL bString)
{
	if (bString==0)
	{
		if (str1>str2)
			return 1;
		else if (str1<str2)
			return -1;
		else 
			return 0;
	}
	else
	{
		if (_ttol(str1)>_ttol(str2))
			return 1;
		else if (_ttol(str1)<_ttol(str2))
			return -1;
		else 
			return 0;
	}

}
