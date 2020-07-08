// COOPERATIVE_STATISTICS_PREVIEW.cpp : implementation file
//

#include "stdafx.h"
#include "COOPERATIVE_STATISTICS_PREVIEW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCOOPERATIVE_STATISTICS_PREVIEW dialog


CCOOPERATIVE_STATISTICS_PREVIEW::CCOOPERATIVE_STATISTICS_PREVIEW(CESL_Mgr* pParent, CStaticsInfoMgr *pSInfoMgr)
	: CESL_Mgr(CCOOPERATIVE_STATISTICS_PREVIEW::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCOOPERATIVE_STATISTICS_PREVIEW)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pSInfoDM = pSInfoMgr->m_pStaticsMainInfoDM;
	m_pSInfoMgr = pSInfoMgr;

}


void CCOOPERATIVE_STATISTICS_PREVIEW::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCOOPERATIVE_STATISTICS_PREVIEW)
	DDX_Control(pDX, IDC_treeCOLUMMN_NAME, m_ctrlTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCOOPERATIVE_STATISTICS_PREVIEW, CDialog)
	//{{AFX_MSG_MAP(CCOOPERATIVE_STATISTICS_PREVIEW)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TVN_BEGINDRAG, IDC_treeCOLUMMN_NAME, OnBegindragtreeCOLUMMNNAME)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCOOPERATIVE_STATISTICS_PREVIEW message handlers

BOOL CCOOPERATIVE_STATISTICS_PREVIEW::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CCOOPERATIVE_STATISTICS_PREVIEW::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pParentCM = FindCM( _T("CM_COOPERATIVE_STATISTICS_MAIN") );
	if( m_pParentCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );

	m_TreeImage.Create(IDB_TREEBITMAP, 16, 0, RGB(255, 255, 255));

	m_bLDragging = FALSE;

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;
}

INT CCOOPERATIVE_STATISTICS_PREVIEW::InitScreen()
{
EFS_BEGIN

	INT ids;
	CString sStaticsClass;

	ids = m_pParentCM->GetControlMgrData( _T("통계종류") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );
	
	ids = SettingTree(sStaticsClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitScreen") );

	InitGrid();

	m_sXAlias = _T("");
	m_sYAlias = _T("");
	m_sOption = _T("");
	m_sOrder = _T("");

	ids = m_pSInfoMgr->GetXYAlias( sStaticsClass , m_sXAlias , m_sYAlias , m_sOption , m_sOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );

	// 다시 재구성하기
	ids = ViewExpColumnName( sStaticsClass, m_sXAlias , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );

	ids = ViewExpColumnName( sStaticsClass , m_sYAlias , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );;

	// OnSize 발생시키기
	ReSizeControl();

	return 0;

EFS_END
return -1;
}

INT CCOOPERATIVE_STATISTICS_PREVIEW::SettingTree( CString sStaticsClass )
{
EFS_BEGIN

	CString sEcolasSVersion;
	GetManageValue( _T("기타") , _T("공통") , _T("프로그램버젼") , _T("ECOLAS-S Version") , sEcolasSVersion );

	INT ids;

	m_ctrlTree.SetImageList( &m_TreeImage , TVSIL_NORMAL );

	m_ctrlTree.DeleteAllItems();

	TVINSERTSTRUCTW tvinsert;

	// 기본
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE;
	tvinsert.item.state = 0;
	tvinsert.item.stateMask = 0;
	tvinsert.item.cchTextMax = 60;
	tvinsert.item.cChildren = 0;
	tvinsert.item.iSelectedImage = 0;
	
	const INT nCnt = 2;
	TCHAR *sTitleName[nCnt][3] =
	{
		{ _T("열") , _T("X축알리아스") } , 
		{ _T("행") , _T("Y축알리아스") , _T("단위") }
	};

	HTREEITEM hItem;

	INT nIndex = GetCurrentDMIndex( sStaticsClass );
	if( nIndex < 0 ) return 0;

	CArray < CString,CString> asFieldName;
	CString sFieldNames;

	for( INT i = 0 ; i < 2 ; i++ )
	{
		asFieldName.RemoveAll();
	
		tvinsert.item.iImage = i+1;

		tvinsert.hParent = NULL;
		
		tvinsert.item.pszText = sTitleName[i][0];

		hItem = m_ctrlTree.InsertItem(&tvinsert);
		
		tvinsert.hParent = hItem;
		
		ids = m_pSInfoDM->GetCellData( sTitleName[i][1] , nIndex , sFieldNames );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SettingTree") );

		CLocCommonAPI::MakeArrayList( sFieldNames , _T(",") , asFieldName );

		// 2. X축 내용 구성 , Y축 내용 구성
		for( INT j = 0 ; j < asFieldName.GetSize();  j++ )
		{
			if( asFieldName.GetAt(j).Compare(_T("학년반별")) == 0 )
			{
				if( sEcolasSVersion != _T("U") && sEcolasSVersion != _T("L") )
					continue;
			}

			tvinsert.item.iImage = 3;

			TCHAR sTemp[200];
			_tcscpy( sTemp , asFieldName.GetAt(j).GetBuffer(0) );

			tvinsert.item.pszText = sTemp;
			m_ctrlTree.InsertItem( &tvinsert );
		}
		m_ctrlTree.Expand(hItem,TVE_EXPAND);
	}
	
	return 0;

EFS_END
return -1;
}

INT CCOOPERATIVE_STATISTICS_PREVIEW::GetCurrentDMIndex( CString sStaticsClass )
{
EFS_BEGIN

	INT ids;

	CString sTmpClass;
	for( INT i = 0 ; i < m_pSInfoDM->GetRowCount(); i++ )
	{
		ids = m_pSInfoDM->GetCellData( _T("통계종류") , i , sTmpClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetCurrentDMIndex") );

		if( sStaticsClass.Compare( sTmpClass ) == 0 ) 
		{
			return i;
		}
	}

	return 0;

EFS_END
return -1;
}

VOID CCOOPERATIVE_STATISTICS_PREVIEW::OnShowWindow(BOOL bShow, UINT nStatus) 
{
EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	
	InitScreen();

EFS_END
}

INT CCOOPERATIVE_STATISTICS_PREVIEW::ViewExpColumnName( CString sStaticsClass , CString sAlias , INT nPosition )
{
EFS_BEGIN

	if( sAlias.IsEmpty() )
		return 0;

	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridEXP_GRID);

	CStringArray asColumnName;
	CString strAlias;
	INT	nStartPos;
	INT	nEndPos;
	INT ids ;

	ids = m_pSInfoMgr->GetColumnNames( sStaticsClass , sAlias  , asColumnName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewExpColumnName") );

	if( asColumnName.GetSize() < 1 ) return 0;

	InitGrid(nPosition);
	if( nPosition == 1 )
	{
		pGrid->SetCols( 0 , asColumnName.GetSize() + 1 );

		for( INT i = 0 ; i < asColumnName.GetSize() ; i++ )
		{
			// Function사용했을 경우 미리보기 화면의 이름 설정
			if ( asColumnName.GetAt(i).Find(_T("$"), 0 ) > -1 )
			{	
				nStartPos = asColumnName.GetAt(i).Find(_T("("), 0 );
				nEndPos = asColumnName.GetAt(i).Find(_T(")"), 0 );			
				strAlias = asColumnName.GetAt(i).Mid(nStartPos+1, nEndPos-nStartPos-1);
				asColumnName.SetAt(i, strAlias);
			}
			pGrid->SetTextMatrix( 0 , i+1 , asColumnName.GetAt(i) );
		}
		m_sXAlias = sAlias;
	}
	else if( nPosition == 2 )
	{
		pGrid->SetRows( asColumnName.GetSize() + 1 );		
		for( INT i = 0 ; i < asColumnName.GetSize() ; i++ )
		{
			// Function사용했을 경우 미리보기 화면의 이름 설정
			if ( asColumnName.GetAt(i).Find(_T("$"), 0 ) > -1 )
			{	
				nStartPos = asColumnName.GetAt(i).Find(_T("("), 0 );
				nEndPos = asColumnName.GetAt(i).Find(_T(")"), 0 );			
				strAlias = asColumnName.GetAt(i).Mid(nStartPos+1, nEndPos-nStartPos-1);
				asColumnName.SetAt(i, strAlias);
			}
			pGrid->SetTextMatrix( i+1 , 0 , asColumnName.GetAt(i) );
		}
		m_sYAlias = sAlias;
	}
	pGrid->SetRowHeight(0,500);
	return 0;

EFS_END
return -1;
}

VOID CCOOPERATIVE_STATISTICS_PREVIEW::ReSizeControl()
{
EFS_BEGIN

	if( GetSafeHwnd() == NULL ) return;

	INT cx , cy;
	CRect WndPos;
	GetWindowRect(WndPos);
	cx = WndPos.Width();
	cy = WndPos.Height();
	
	// 예상모습 문자
	if( GetDlgItem(IDC_staRIGHTTITLE)->GetSafeHwnd() == NULL ) return;
	CRect WndRect;
	GetDlgItem(IDC_staRIGHTTITLE)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_staRIGHTTITLE)->MoveWindow( WndRect.left , WndRect.top , cx - WndRect.left - 10 , WndRect.Height() );
	
	// 트리
	if( GetDlgItem(IDC_treeCOLUMMN_NAME)->GetSafeHwnd() == NULL ) return;
	GetDlgItem(IDC_treeCOLUMMN_NAME)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_treeCOLUMMN_NAME)->MoveWindow( WndRect.left , WndRect.top , WndRect.Width() , cy - WndRect.top - 50 );
	
	INT nTop;
	nTop = WndRect.top;
	
	// 그리드
	if( GetDlgItem(IDC_gridEXP_GRID)->GetSafeHwnd() == NULL ) return;
	
	
	GetDlgItem(IDC_gridEXP_GRID)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	
	GetDlgItem(IDC_gridEXP_GRID)->MoveWindow( WndRect.left , nTop , cx - WndRect.left - 10 , cy - nTop - 50 );
	GetDlgItem(IDC_gridEXP_GRID)->RedrawWindow();
	
	// 세로 구분선
	if( GetDlgItem(IDC_staCERO)->GetSafeHwnd() == NULL ) return;
	GetDlgItem(IDC_staCERO)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_staCERO)->MoveWindow( WndRect.left , WndRect.top , WndRect.Width() , cy - WndRect.top - 50 );

	return ;

EFS_END
}

INT CCOOPERATIVE_STATISTICS_PREVIEW::SaveXY()
{
EFS_BEGIN

	INT ids;
	
	if( m_sXAlias.IsEmpty() || m_sYAlias.IsEmpty()  )
	{
		AfxMessageBox( _T("행열이 선택되어 있지 않습니다. \r\n왼쪽 이름의 항목을 오른쪽의 예상모습에 '드래그&드롭'하십시오.") );
		return 1;
	}
	if(  m_sOrder.IsEmpty() )
	{
		AfxMessageBox( _T("순위의 범위를 입력해주십시오.") );
		return 2;
	}
		
	CString sStaticsClass;
	ids = m_pParentCM->GetControlMgrData( _T("통계종류") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveXY") );
	
	m_pSInfoMgr->SetXYAlias( sStaticsClass , m_sXAlias , m_sYAlias , m_sOption , m_sOrder );
	
	return 0;

EFS_END
return -1;
}

INT CCOOPERATIVE_STATISTICS_PREVIEW::InitGrid(INT nPosition)
{
EFS_BEGIN

	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridEXP_GRID);
	
	if( nPosition == -1 || nPosition == 2 )
	{
		pGrid->SetRows(2);
	}
	
	if( nPosition == -1 || nPosition == 1 )
	{
		pGrid->SetCols(0,2);
	}

	if( nPosition == -1 , nPosition == -1 )
	{
		pGrid->SetTextMatrix(0,1,_T(""));
		pGrid->SetTextMatrix(1,0,_T(""));
	}

	return 0;

EFS_END
return -1;
}

VOID CCOOPERATIVE_STATISTICS_PREVIEW::OnBegindragtreeCOLUMMNNAME(NMHDR* pNMHDR, LRESULT* pResult) 
{
EFS_BEGIN

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	POINT point;
	GetCursorPos(&point);
	m_ctrlTree.ScreenToClient(&point);

	UINT uFlags;

	HTREEITEM hItem = m_ctrlTree.HitTest(point, &uFlags);
	if (hItem == NULL) return;

	if( _T("단위") == m_ctrlTree.GetItemText( hItem ) )
		return;
	
	if ((TVHT_ONITEM & uFlags))
	   m_ctrlTree.SelectItem(hItem);	

	m_hitemDrag = pNMTreeView->itemNew.hItem;
	m_hitemDrop = NULL;

	m_pDragImage = m_ctrlTree.CreateDragImage(m_hitemDrag);
	if( !m_pDragImage )
		return;

	m_bLDragging = TRUE;
	m_pDragImage->BeginDrag(0, CPoint(0,0));
	POINT pt = pNMTreeView->ptDrag;
	ClientToScreen( &pt );
	m_pDragImage->DragEnter(NULL, pt);
	SetCapture();
	

	m_ctrlTree.SelectItem(hItem);	

	*pResult = 0;

EFS_END
}

VOID CCOOPERATIVE_STATISTICS_PREVIEW::OnMouseMove(UINT nFlags, CPoint point) 
{
EFS_BEGIN
	
	HTREEITEM	hitem;
	UINT		flags;

	INT ids;
	INT nPosition=-1;

	if (m_bLDragging)
	{
		POINT pt = point;
		ClientToScreen( &pt );
		CImageList::DragMove(pt);
		if ((hitem = m_ctrlTree.HitTest(point, &flags)) != NULL)
		{
			m_hitemDrop = hitem;
			CImageList::DragShowNolock(TRUE);			
		}

		IsDropPosition( point , nPosition );
	}

	ids = GridColoring(nPosition);
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnMouseMove") );
		
	CDialog::OnMouseMove(nFlags, point);

EFS_END
}

VOID CCOOPERATIVE_STATISTICS_PREVIEW::OnLButtonUp(UINT nFlags, CPoint point) 
{
EFS_BEGIN
	
	if (m_bLDragging)
	{
		m_bLDragging = FALSE;
		CImageList::DragLeave(this);
		CImageList::EndDrag();
		ReleaseCapture();

		delete m_pDragImage;

		INT ids;
		INT nPosition;
		if( IsDropPosition( point , nPosition ) )
		{
			ids = SetExpStatics(nPosition);
			if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnLButtonUp") );
		}

		if( m_hitemDrag == m_hitemDrop )
			return;
	}

	CDialog::OnLButtonUp(nFlags, point);

EFS_END
}

// ** 드랍할수 있는 영역인지 검사 
BOOL CCOOPERATIVE_STATISTICS_PREVIEW::IsDropPosition( CPoint point , INT &nPosition )
{
EFS_BEGIN

	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridEXP_GRID);
	
	CRect GridRect;
	pGrid->GetWindowRect(&GridRect);

	ScreenToClient( GridRect );

	if( (( GridRect.top < point.y ) && ( GridRect.bottom > point.y ))
		&& (( GridRect.left < point.x ) && ( GridRect.right > point.x )) )
	{
		long nFixedCellHeight;
		long nFixedCellWidth;

		nFixedCellHeight = GetGridHeight( 0 , 0 , pGrid );
		nFixedCellWidth = GetGridWidth( 0 , 0 , pGrid );

		long nHeight;
		long nWidth;
		
		CString sParentName;

		HTREEITEM hSelectItem =  m_ctrlTree.GetSelectedItem();
		HTREEITEM hSelectParentItem = m_ctrlTree.GetParentItem(hSelectItem);
		if( hSelectParentItem == NULL )
		{
			nPosition = -1;
			return FALSE;
		}
		
		sParentName = m_ctrlTree.GetItemText(hSelectParentItem);
	
		if( sParentName.Compare(_T("열")) == 0 )
		{	
		
			// X축 검사
			nHeight = GetGridHeight( 0 , 0 , pGrid );
			nWidth = GetGridWidth( 1 , pGrid->GetCols(0)-1 , pGrid );

			if( ( GridRect.top < point.y ) && ( GridRect.top + nHeight > point.y )
				&& ( GridRect.left + nFixedCellWidth < point.x ) && ( GridRect.left + nFixedCellWidth + nWidth > point.x ) 
			)
			{
				nPosition = 1;
				return TRUE;
			}
		}
		else if( sParentName.Compare(_T("행")) == 0 )
		{	
			// Y축 검사	
			nHeight = GetGridHeight( 1 , pGrid->GetRows()-1 , pGrid );
			nWidth = GetGridWidth( 0 , 0 , pGrid );

			if( ( GridRect.top + nFixedCellHeight < point.y ) && ( GridRect.top + nFixedCellHeight + nHeight > point.y )
				&& ( GridRect.left < point.x ) && ( GridRect.left + nFixedCellWidth > point.x ) 
			)
			{
				nPosition = 2;
				return TRUE;
			}
		}
	}

	return FALSE;

EFS_END
return FALSE;
}

VOID CCOOPERATIVE_STATISTICS_PREVIEW::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);

	ReSizeControl();

EFS_END
}

VOID CCOOPERATIVE_STATISTICS_PREVIEW::OnLButtonDown(UINT nFlags, CPoint point) 
{
EFS_BEGIN

	HTREEITEM hItem = m_ctrlTree.GetSelectedItem();

	m_ctrlTree.Select( hItem , TVGN_CARET );
	
	CDialog::OnLButtonDown(nFlags, point);

EFS_END
}

INT CCOOPERATIVE_STATISTICS_PREVIEW::GridColoring(INT nPosition)
{
EFS_BEGIN

	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridEXP_GRID);

	INT nDefaultRow=pGrid->GetRow();
	INT nDefaultCol=pGrid->GetCol();
	
	pGrid->SetRow(0);
	pGrid->SetCol(0);
	DWORD nXColor = pGrid->GetCellBackColor();
	DWORD nYColor = pGrid->GetCellBackColor();
	DWORD nTmpColor;

	if( nPosition == 1 )
	{
		nXColor = RGB(105,105,105);
	}
	else if( nPosition == 2 )
	{
		nYColor = RGB(105,105,105);
	}

	for( INT i = pGrid->GetCols(0)-1 ; i > 0 ; i-- )
	{
		pGrid->SetRow(0);
		pGrid->SetCol(i);
		nTmpColor = pGrid->GetCellBackColor();
		pGrid->SetCellBackColor(nXColor);
	}
	for( i = pGrid->GetRows()-1 ; i > 0 ; i-- )
	{
		pGrid->SetRow(i);
		pGrid->SetCol(0);
		pGrid->SetCellBackColor(nYColor);
	}

	return 0;

EFS_END
return -1;
}

INT CCOOPERATIVE_STATISTICS_PREVIEW::SetExpStatics(INT nPosition)
{
EFS_BEGIN

	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridEXP_GRID);

	INT ids;

	CString sAlias;

	HTREEITEM hItem = m_ctrlTree.GetSelectedItem();
	
	sAlias = m_ctrlTree.GetItemText( hItem );

	CString sStaticsClass;
	ids = m_pParentCM->GetControlMgrData( _T("통계종류") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewExpColumnName") );

	ids = ViewExpColumnName(sStaticsClass , sAlias,nPosition);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetExpStatics") );

	return 0;

EFS_END
return -1;
}

long CCOOPERATIVE_STATISTICS_PREVIEW::GetGridHeight( long nStart , long nFinish , CMSHFlexGrid *pGrid )
{
EFS_BEGIN

	long nHeight=0;

	for( INT i = nStart ; i < nFinish+1 ; i++ )
	{
		nHeight += pGrid->GetRowHeight(i);
	}

	return nHeight/15;

EFS_END
return -1;
}

long CCOOPERATIVE_STATISTICS_PREVIEW::GetGridWidth( long nStart , long nFinish , CMSHFlexGrid *pGrid )
{
EFS_BEGIN

	long nWidth=0;

	for( INT i = nStart ; i < nFinish+1 ; i++ )
	{
		pGrid->SetCol(i);
		nWidth += pGrid->GetCellWidth();
	}

	return nWidth/15;

EFS_END
return -1;
}
