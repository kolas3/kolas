// BO_STATICS_1120.cpp : implementation file
//

#include "stdafx.h"
#include "BO_STATICS_1120.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_STATICS_1120 dialog


CBO_STATICS_1120::CBO_STATICS_1120(CESL_Mgr* pParent , CStaticsInfoMgr *pSInfoMgr )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_STATICS_1120)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSInfoDM = pSInfoMgr->m_pStaticsMainInfoDM;
	m_pSInfoMgr = pSInfoMgr;	
}

CBO_STATICS_1120::~CBO_STATICS_1120()
{
	
}
VOID CBO_STATICS_1120::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_STATICS_1120)
	DDX_Control(pDX, IDC_treeCOLUMMN_NAME, m_ctrlTree);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_STATICS_1120, CDialog)
	//{{AFX_MSG_MAP(CBO_STATICS_1120)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TVN_BEGINDRAG, IDC_treeCOLUMMN_NAME, OnBegindragtreeCOLUMMNNAME)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(NM_CLICK, IDC_treeCOLUMMN_NAME, OnClicktreeCOLUMMNNAME)
	ON_BN_CLICKED(IDC_btnINIT, OnbtnINIT)
	ON_BN_CLICKED(IDC_btnSAVE, OnbtnSAVE)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_STATICS_1120 message handlers
BOOL CBO_STATICS_1120::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

INT CBO_STATICS_1120::InitScreen()
{
	EFS_BEGIN

	INT ids;
	CString sStaticsClass,sOrder;

	ids = m_pParentCM->GetControlMgrData( _T("통계종류") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );
	
	ids = m_pSInfoMgr->GetCellData( m_pSInfoMgr->m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass ,  _T("순위여부") , sOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );

	if( sOrder.Compare(_T("-1")) == 0 ) 
		m_nMode = 0;
	else 
		m_nMode = 1;

	ShowHideControl();

	ids = SettingTree(sStaticsClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitScreen") );

	InitGrid();

	m_sXAlias = _T("");
	m_sYAlias = _T("");
	m_sOption = _T("");
	m_sOrder = _T("");

	ids = m_pSInfoMgr->GetXYAlias( sStaticsClass , m_sXAlias , m_sYAlias , m_sOption , m_sOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );

	ids = ViewExpColumnName( sStaticsClass, m_sXAlias , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );

	ids = ViewExpColumnName( sStaticsClass , m_sYAlias , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );;

	if( m_nMode == 1 )
	{
		MakeTab(m_sOption);
		MakeEdtOrder(m_sOrder);
	}

	ReSizeControl();

	return 0;

	EFS_END
	return -1;

}

BOOL CBO_STATICS_1120::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	m_pParentCM = FindCM( _T("CM_BO_STATICS_1100") );
	if( m_pParentCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );
	

	m_TreeImage.Create(IDB_TREEBITMAP, 16, 0, RGB(255, 255, 255));
	
	m_bLDragging = FALSE;
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}


INT CBO_STATICS_1120::SettingTree( CString sStaticsClass )
{
	EFS_BEGIN

	CString sEcolasSVersion;
	GetManageValue( _T("기타") , _T("공통") , _T("프로그램버젼") , _T("ECOLAS-S Version") , sEcolasSVersion );

	INT ids;

	m_ctrlTree.SetImageList( &m_TreeImage , TVSIL_NORMAL );

	m_ctrlTree.DeleteAllItems();

	TVINSERTSTRUCTW tvinsert;

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
	CString strChkColRow;

	for( INT i = m_nMode ; i < 2 ; i++ )
	{
		asFieldName.RemoveAll();
	
		tvinsert.item.iImage = i+1;

		tvinsert.hParent = NULL;
		if( m_nMode == 1 )
		{
			tvinsert.item.pszText = sTitleName[i][2];
			strChkColRow = sTitleName[i][2];
		}
		else
		{
			tvinsert.item.pszText = sTitleName[i][0];
			strChkColRow = sTitleName[i][0];
		}

		hItem = m_ctrlTree.InsertItem(&tvinsert);
		
		tvinsert.hParent = hItem;
		
		ids = m_pSInfoDM->GetCellData( sTitleName[i][1] , nIndex , sFieldNames );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SettingTree") );

		CLocCommonAPI::MakeArrayList( sFieldNames , _T(",") , asFieldName );

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
 
			if( sFieldNames == _T("분류기호별") && strChkColRow == _T("열"))
			{
				CString strTemp;
				strTemp.Format(_T("분류기호별(DDC)"));
				_tcscpy( sTemp , strTemp.GetBuffer(0) );

				tvinsert.item.pszText = sTemp;
				m_ctrlTree.InsertItem( &tvinsert );
			}
		}
		m_ctrlTree.Expand(hItem,TVE_EXPAND);
		
	}

	
	return 0;

	EFS_END
	return -1;

}

VOID CBO_STATICS_1120::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	
	InitScreen();


	EFS_END
}


INT CBO_STATICS_1120::GetCurrentDMIndex( CString sStaticsClass )
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

VOID CBO_STATICS_1120::OnBegindragtreeCOLUMMNNAME(NMHDR* pNMHDR, LRESULT* pResult) 
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

	m_pDragImage = m_ctrlTree.CreateDragImage(m_hitemDrag);  // get the image list for dragging
	// CreateDragImage() returns NULL if no image list
	// associated with the tree view control
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

VOID CBO_STATICS_1120::OnMouseMove(UINT nFlags, CPoint point) 
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

	if( m_nMode == 0 )
		ids = GridColoring(nPosition);
	else if( m_nMode == 1 )
		ids = TabColoring(nPosition);

	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnLButtonUp") );
	
	
	CDialog::OnMouseMove(nFlags, point);

	EFS_END
}

VOID CBO_STATICS_1120::OnLButtonUp(UINT nFlags, CPoint point) 
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

VOID CBO_STATICS_1120::OnClicktreeCOLUMMNNAME(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN



	*pResult = 0;

	EFS_END
}


INT CBO_STATICS_1120::InitGrid(INT nPosition)
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

INT CBO_STATICS_1120::SetExpStatics(INT nPosition)
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

	if( m_nMode == 0 )
	{
		ids = ViewExpColumnName(sStaticsClass , sAlias,nPosition);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetExpStatics") );
	}
	else if( m_nMode == 1 )
	{
		MakeTab( sStaticsClass , sAlias );
	}


	return 0;

	EFS_END
	return -1;

}

INT CBO_STATICS_1120::ViewExpColumnName( CString sStaticsClass , CString sAlias , INT nPosition )
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

	if (nPosition == 2)
		ids = m_pSInfoMgr->GetColumnNames(sStaticsClass, sAlias, asColumnName, 9);
	else
	ids = m_pSInfoMgr->GetColumnNames( sStaticsClass , sAlias  , asColumnName );
	
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewExpColumnName") );

	if( asColumnName.GetSize() < 1 && (m_nMode != 1)  ) return 0;

	if( (nPosition == 1) && (m_nMode == 1) )
	{
		ids = CLocCommonAPI::MakeArrayList( sAlias , _T(",") , asColumnName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewExpColumnName") );
	}

	InitGrid(nPosition);
	if( nPosition == 1 )
	{
		pGrid->SetCols( 0 , asColumnName.GetSize() + 1 );

		for( INT i = 0 ; i < asColumnName.GetSize() ; i++ )
		{
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
			if ( asColumnName.GetAt(i).Find(_T("$"), 0 ) > -1 )
			{	
				nStartPos = asColumnName.GetAt(i).Find(_T("("), 0 );
				nEndPos = asColumnName.GetAt(i).Find(_T(")"), 0 );			
				strAlias = asColumnName.GetAt(i).Mid(nStartPos+1, nEndPos-nStartPos-1);
				asColumnName.SetAt(i, strAlias);
				
			}
			pGrid->SetTextMatrix( i+1 , 0 , asColumnName.GetAt(i) );
			pGrid->SetRowHeight(i,500);
		}
		m_sYAlias = sAlias;
	}

	return 0;

	EFS_END
	return -1;

}

BOOL CBO_STATICS_1120::IsDropPosition( CPoint point , INT &nPosition )
{
	EFS_BEGIN

	if( m_nMode == 0 )
	{
		CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridEXP_GRID);
		
		CRect GridRect;
		pGrid->GetWindowRect(&GridRect);

		ScreenToClient( GridRect );

		if( (( GridRect.top < point.y ) && ( GridRect.bottom > point.y ))
			&& (( GridRect.left < point.x ) && ( GridRect.right > point.x )) 
		)
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
	}
	else if( m_nMode == 1 )
	{
		CTabCtrl *pTab = ((CTabCtrl*)(GetDlgItem( IDC_tabOPTION )));
		
		CRect TabRect;
		pTab->GetWindowRect(&TabRect);

		ScreenToClient( TabRect );
		
		if( TabRect.top < point.y && TabRect.bottom > point.y && TabRect.left < point.x && TabRect.right > point.x )
		{
			nPosition = 3;
			return TRUE;
		}

	}

	return FALSE;

	EFS_END
	return FALSE;

}

long CBO_STATICS_1120::GetGridHeight( long nStart , long nFinish , CMSHFlexGrid *pGrid )
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


long CBO_STATICS_1120::GetGridWidth( long nStart , long nFinish , CMSHFlexGrid *pGrid )
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

VOID CBO_STATICS_1120::OnbtnINIT() 
{
	EFS_BEGIN

	INT ids;

	CString sStaticsClass;
	ids = m_pParentCM->GetControlMgrData( _T("통계종류") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("InitScreen") );

	if( m_nMode == 0 )
		m_pSInfoMgr->SetXYAlias( sStaticsClass , _T("") , _T("") , _T("전체") , _T("-1") );
	else if( m_nMode == 1 )
		m_pSInfoMgr->SetXYAlias( sStaticsClass , m_sXAlias , m_sYAlias , _T("전체") , _T("100") );

	InitScreen();

	EFS_END
}

VOID CBO_STATICS_1120::OnbtnSAVE() 
{
	EFS_BEGIN

	INT ids;

	ids = SaveXY();

	EFS_END
}

VOID CBO_STATICS_1120::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

VOID CBO_STATICS_1120::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	

	ReSizeControl();

	EFS_END
}


INT CBO_STATICS_1120::GridColoring(INT nPosition)
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

VOID CBO_STATICS_1120::OnLButtonDown(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	HTREEITEM hItem = m_ctrlTree.GetSelectedItem();

	m_ctrlTree.Select( hItem , TVGN_CARET );
	
	CDialog::OnLButtonDown(nFlags, point);

	EFS_END
}

VOID CBO_STATICS_1120::ShowHideControl()
{
	EFS_BEGIN

	INT nShowHide = SW_HIDE;
	
	if( m_nMode == 1)
		nShowHide = SW_SHOW;

	const INT nCnt = 4;
	INT nIDC[nCnt] =
	{
		IDC_staORDER , IDC_edtORDER , IDC_staORDER2 , IDC_tabOPTION
	};

	for( INT i = 0 ; i < nCnt ; i++ )
		GetDlgItem( nIDC[i] )->ShowWindow(nShowHide);

	EFS_END
}

INT CBO_STATICS_1120::MakeTab(CString sOption)
{
	EFS_BEGIN

	INT ids;

	CString sStaticsClass;
	ids = m_pParentCM->GetControlMgrData( _T("통계종류") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );

	MakeTab( sStaticsClass , sOption );

	return 0;

	EFS_END
	return -1;

}

INT CBO_STATICS_1120::MakeTab( CString sStaticsClass , CString sAlias )
{
	EFS_BEGIN

	INT ids;

	CTabCtrl *pTab = ((CTabCtrl*)(GetDlgItem( IDC_tabOPTION )));

	pTab->DeleteAllItems();

	CStringArray saColumnName;

	if( sAlias.Compare( _T("전체") )==0 )
	{
		saColumnName.Add(sAlias);
	}
	else
	{
		ids = m_pSInfoMgr->GetColumnNames( sStaticsClass , sAlias , saColumnName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );
	}

	for( INT i = 0 ; i < saColumnName.GetSize() ; i++ )
	{
		pTab->InsertItem(i,saColumnName.GetAt(i));
	}

	m_sOption = sAlias;

	return 0;

	EFS_END
	return -1;

}

INT CBO_STATICS_1120::MakeEdtOrder( CString sOrder )
{
	EFS_BEGIN

	GetDlgItem(IDC_edtORDER)->SetWindowText( sOrder );

	return 0;

	EFS_END
	return -1;

}

INT CBO_STATICS_1120::TabColoring( INT nPosition )
{
	EFS_BEGIN

	CTabCtrl *pTab = ((CTabCtrl*)(GetDlgItem( IDC_tabOPTION )));

	bool bHighlight = TRUE;

	if( nPosition == -1 )
		bHighlight = FALSE;	
	
	for( INT i = 0 ; i < pTab->GetItemCount() ; i++ )
		pTab->HighlightItem( i , bHighlight );

	return 0;

	EFS_END
	return -1;

}


INT CBO_STATICS_1120::SaveXY()
{
	EFS_BEGIN

	INT ids;
	
	if( (GetDlgItem(IDC_edtORDER))->IsWindowVisible() )
		GetDlgItem(IDC_edtORDER)->GetWindowText( m_sOrder );
	
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

VOID CBO_STATICS_1120::ReSizeControl()
{
	EFS_BEGIN

	if( GetSafeHwnd() == NULL ) return;

	INT cx , cy;
	CRect WndPos;
	GetWindowRect(WndPos);
	cx = WndPos.Width();
	cy = WndPos.Height();
	
	if( GetDlgItem(IDC_staRIGHTTITLE)->GetSafeHwnd() == NULL ) return;
	CRect WndRect;
	GetDlgItem(IDC_staRIGHTTITLE)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_staRIGHTTITLE)->MoveWindow( WndRect.left , WndRect.top , cx - WndRect.left - 10 , WndRect.Height() );
	
	if( GetDlgItem(IDC_staLINE)->GetSafeHwnd() == NULL ) return;
	GetDlgItem(IDC_staLINE)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_staLINE)->MoveWindow( -1 , cy - 45 , cx+2 , WndRect.Height() );
	
	if( GetDlgItem(IDC_treeCOLUMMN_NAME)->GetSafeHwnd() == NULL ) return;
	GetDlgItem(IDC_treeCOLUMMN_NAME)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_treeCOLUMMN_NAME)->MoveWindow( WndRect.left , WndRect.top , WndRect.Width() , cy - WndRect.top - 50 );
	
	INT nTop;
	if( m_nMode == 0 )
	nTop = WndRect.top;
	
	if( GetDlgItem(IDC_tabOPTION)->GetSafeHwnd() == NULL ) return;
	GetDlgItem(IDC_tabOPTION)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_tabOPTION)->MoveWindow( WndRect.left , WndRect.top , cx - WndRect.left - 10 , WndRect.Height() );
	
	if( GetDlgItem(IDC_gridEXP_GRID)->GetSafeHwnd() == NULL ) return;
	
	if( m_nMode == 1 )
	nTop = WndRect.bottom+1;
	
	GetDlgItem(IDC_gridEXP_GRID)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	
	GetDlgItem(IDC_gridEXP_GRID)->MoveWindow( WndRect.left , nTop , cx - WndRect.left - 10 , cy - nTop - 50 );
	GetDlgItem(IDC_gridEXP_GRID)->RedrawWindow();
	
	if( GetDlgItem(IDC_staCERO)->GetSafeHwnd() == NULL ) return;
	GetDlgItem(IDC_staCERO)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_staCERO)->MoveWindow( WndRect.left , WndRect.top , WndRect.Width() , cy - WndRect.top - 50 );
		
	if( GetDlgItem(IDC_btnINIT)->GetSafeHwnd() == NULL ) return;
	GetDlgItem(IDC_btnINIT)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_btnINIT)->MoveWindow( WndRect.left , cy-35 , WndRect.Width() , WndRect.Height() );
	
	if( GetDlgItem(IDC_btnSAVE)->GetSafeHwnd() == NULL ) return;
	GetDlgItem(IDC_btnSAVE)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_btnSAVE)->MoveWindow( cx - 20 - WndRect.Width()*2 , cy-35 , WndRect.Width() , WndRect.Height() );
	
	if( GetDlgItem(IDC_btnCLOSE)->GetSafeHwnd() == NULL ) return;
	GetDlgItem(IDC_btnCLOSE)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_btnCLOSE)->MoveWindow( cx - 10 - WndRect.Width() , cy-35 , WndRect.Width() , WndRect.Height() );
	

	return ;

	EFS_END
}

BOOL CBO_STATICS_1120::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
		if ( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
			return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}



HBRUSH CBO_STATICS_1120::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
