// BO_LOC_2190.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_2190.h"

#include "BO_LOC_21001.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2190 dialog


CBO_LOC_2190::CBO_LOC_2190(CESL_Mgr* pParent,CString strMenuAlias )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_2190)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sStaticsClass = _T("장서점검통계");
	m_nMode = 0;

	m_strMenuName = strMenuAlias;

	m_pAxisVal_Statics = NULL;
}

CBO_LOC_2190::~CBO_LOC_2190()
{
	if( m_pSInfoMgr != NULL ) 
		delete m_pSInfoMgr;
	m_pSInfoMgr = NULL;
	
	if( m_pSpfWork != NULL)
	{
		delete m_pSpfWork;
		m_pSpfWork = NULL;
	}
}

VOID CBO_LOC_2190::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_2190)
	DDX_Control(pDX, IDC_treeCOLUMMN_NAME, m_ctrlTree);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_2190, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_2190)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TVN_BEGINDRAG, IDC_treeCOLUMMN_NAME, OnBegindragtreeCOLUMMNNAME)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(NM_CLICK, IDC_treeCOLUMMN_NAME, OnClicktreeCOLUMMNNAME)
	ON_BN_CLICKED(IDC_btnINIT, OnbtnINIT)
	ON_BN_CLICKED(IDC_bRUN, OnbtnSAVE)
	ON_BN_CLICKED(IDC_bCLOSE, OnbtnCLOSE)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_chkSUMZERODEL, OnchkSUMZERODEL)
	ON_BN_CLICKED(IDC_btnDEBUG, OnbtnDEBUG)
	ON_BN_CLICKED(IDC_bRUN2, OnbRUN2)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2190 message handlers
BOOL CBO_LOC_2190::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

INT CBO_LOC_2190::InitScreen()
{
	EFS_BEGIN

	//2020.03.27 JOB.2020.0021 add by kyj : 위치이동
	m_pSpfWork = NULL;
	m_pSpfWork = new CBLBookCheckProc(this);
	
	m_pSInfoMgr = new CStaticsInfoMgr(this,m_strMenuName);
	m_pSInfoMgr->MakeDMInfo();
	
	m_pSInfoDM = m_pSInfoMgr->m_pStaticsMainInfoDM;
	
	m_pSInfoMgr->SetInit_CStatics_AXISVal_CELLCLICK( m_pAxisVal_Statics );
	//

	INT ids;

	ids = SettingTree(m_sStaticsClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitScreen") );

	InitGrid();

	m_sXAlias = _T("");
	m_sYAlias = _T("");
	m_sOption = _T("");
	m_sOrder = _T("");

	ids = m_pSInfoMgr->GetXYAlias( m_sStaticsClass , m_sXAlias , m_sYAlias , m_sOption , m_sOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );

	ids = ViewExpColumnName( m_sStaticsClass, m_sXAlias , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );

	ids = ViewExpColumnName( m_sStaticsClass , m_sYAlias , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitScreen") );;

	CRect WndRect;

	GetWindowRect(WndRect);
	WndRect.bottom++;

	MoveWindow(WndRect);
	WndRect.bottom--;
	MoveWindow(WndRect);


	return 0;

	EFS_END
	return -1;

}

BOOL CBO_LOC_2190::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	SetIcon(LoadIcon(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE );	

	m_TreeImage.Create(IDB_TREEBITMAP, 16, 0, RGB(255, 255, 255));
	m_bLDragging = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}


INT CBO_LOC_2190::SettingTree( CString sStaticsClass )
{
	EFS_BEGIN

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

	for( INT i = m_nMode ; i < 2 ; i++ )
	{
		asFieldName.RemoveAll();
	
		tvinsert.item.iImage = i+1;

		tvinsert.hParent = NULL;
		if( m_nMode == 1 )
			tvinsert.item.pszText = sTitleName[i][2];
		else
			tvinsert.item.pszText = sTitleName[i][0];

		hItem = m_ctrlTree.InsertItem(&tvinsert);
		
		tvinsert.hParent = hItem;
		
		ids = m_pSInfoDM->GetCellData( sTitleName[i][1] , nIndex , sFieldNames );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SettingTree") );

		CLocCommonAPI::MakeArrayList( sFieldNames , _T(",") , asFieldName );

		for( INT j = 0 ; j < asFieldName.GetSize();  j++ )
		{
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

VOID CBO_LOC_2190::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	
	InitScreen();


	EFS_END
}


INT CBO_LOC_2190::GetCurrentDMIndex( CString sStaticsClass )
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

VOID CBO_LOC_2190::OnBegindragtreeCOLUMMNNAME(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	POINT point;
	GetCursorPos(&point);
	m_ctrlTree.ScreenToClient(&point);

	UINT uFlags;

	HTREEITEM hItem = m_ctrlTree.HitTest(point, &uFlags);
	if (hItem == NULL) return;

	if ((TVHT_ONITEM & uFlags))
	   m_ctrlTree.SelectItem(hItem);	

	m_hitemDrag = pNMTreeView->itemNew.hItem;
	m_hitemDrop = NULL;

	m_pDragImage = m_ctrlTree.CreateDragImage(m_hitemDrag);  // get the image list for dragging

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

VOID CBO_LOC_2190::OnMouseMove(UINT nFlags, CPoint point) 
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

	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnLButtonUp") );
	
	
	CDialog::OnMouseMove(nFlags, point);

	EFS_END
}

VOID CBO_LOC_2190::OnLButtonUp(UINT nFlags, CPoint point) 
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

VOID CBO_LOC_2190::OnClicktreeCOLUMMNNAME(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN



	*pResult = 0;

	EFS_END
}


INT CBO_LOC_2190::InitGrid(INT nPosition)
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

INT CBO_LOC_2190::SetExpStatics(INT nPosition)
{
	EFS_BEGIN

	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridEXP_GRID);

	INT ids;

	CString sAlias;

	HTREEITEM hItem = m_ctrlTree.GetSelectedItem();
	
	sAlias = m_ctrlTree.GetItemText( hItem );

	if( m_nMode == 0 )
	{
		ids = ViewExpColumnName(m_sStaticsClass , sAlias,nPosition);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetExpStatics") );
	}


	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_2190::ViewExpColumnName( CString sStaticsClass , CString sAlias , INT nPosition )
{
	EFS_BEGIN

	if( sAlias.IsEmpty() )
		return 0;

	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridEXP_GRID);

	CStringArray asColumnName;
	
	INT ids ;

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
			pGrid->SetTextMatrix( 0 , i+1 , asColumnName.GetAt(i) );
		}
		m_sXAlias = sAlias;
	}
	else if( nPosition == 2 )
	{
		pGrid->SetRows( asColumnName.GetSize() + 1 );		
		for( INT i = 0 ; i < asColumnName.GetSize() ; i++ )
		{
			pGrid->SetTextMatrix( i+1 , 0 , asColumnName.GetAt(i) );
		}
		m_sYAlias = sAlias;
	}
	return 0;

	EFS_END
	return -1;

}

BOOL CBO_LOC_2190::IsDropPosition( CPoint point , INT &nPosition )
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

			HTREEITEM hSelectItem =  m_ctrlTree.GetSelectedItem();
			HTREEITEM hSelectParentItem = m_ctrlTree.GetParentItem(hSelectItem);
			if( hSelectParentItem == NULL )
			{
				nPosition = -1;
				return FALSE;
			}

			CString sParentName = m_ctrlTree.GetItemText(hSelectParentItem);
			
			if ( sParentName == _T("열") )
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
			else if ( sParentName == _T("행") )
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

long CBO_LOC_2190::GetGridHeight( long nStart , long nFinish , CMSHFlexGrid *pGrid )
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


long CBO_LOC_2190::GetGridWidth( long nStart , long nFinish , CMSHFlexGrid *pGrid )
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

VOID CBO_LOC_2190::OnbtnINIT() 
{
	EFS_BEGIN

	if( m_nMode == 0 )
		m_pSInfoMgr->SetXYAlias( m_sStaticsClass , _T("") , _T("") , _T("전체") , _T("-1") );
	else if( m_nMode == 1 )
		m_pSInfoMgr->SetXYAlias( m_sStaticsClass , m_sXAlias , m_sYAlias , _T("전체") , _T("100") );

	InitScreen();

	EFS_END
}

VOID CBO_LOC_2190::OnbtnSAVE() 
{
	EFS_BEGIN

	INT ids;

	ids = SaveSetXYAlias();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnSAVE") );
	if( ids > 0 ) return;
	
	AfxGetApp()->DoWaitCursor( 1 );
	ids = RunStatics();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnSAVE") );
	if( ids > 0 ){
		AfxGetApp()->DoWaitCursor( -1 );
		return;
	}
	AfxGetApp()->DoWaitCursor( -1 );

	AfxMessageBox( _T("통계가 완료되었습니다."));


	EFS_END
}

INT CBO_LOC_2190::SaveSetXYAlias()
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
		AfxMessageBox( _T("순위의 범위를 입력해주십시요.") );
		return 2;
	}

	ids = m_pSInfoMgr->SetXYAlias( m_sStaticsClass , m_sXAlias , m_sYAlias , m_sOption , m_sOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveSetXYAlias") );

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_2190::RunStatics()
{
	EFS_BEGIN

	INT ids;

	CString sStaticsClass = _T("장서점검통계");

	CESL_DataMgr *pRegNoDM;
	CString sFieldName = _T("LOST_LIST_FILE");
	CProgressCtrl *pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_prgSTATICS);
	CProgressCtrl *pTotalProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_prgSTATICS2);

	pRegNoDM = m_pSpfWork->MakeTempRegNoDM();
	if( pRegNoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeBookCheckResultDMFromPreBookCheckInfo") );

	ids = m_pSpfWork->MakeRegNoFromBLOB( sFieldName , pRegNoDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
	if( ids > 0 )
	{
		AfxMessageBox( _T("장서 점검 번호를 선택하십시요") );
		if( pRegNoDM !=NULL ) 
			delete pRegNoDM;
		pRegNoDM = NULL;
		return 1;
	}

	if( pRegNoDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("통계를 출력할 목록이 존재하지 않습니다.") );
		if( pRegNoDM !=NULL ) 
			delete pRegNoDM;
		pRegNoDM = NULL;
		return 2;
	}

	ids = m_pSInfoMgr->MakeStaticsCondition(m_sStaticsClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
	if( ids > 0 )
	{
		if( pRegNoDM !=NULL ) 
			delete pRegNoDM;
		pRegNoDM = NULL;
		return ids;
	}

	ids = m_pSInfoMgr->RunStatics( m_sStaticsClass ,pProgressCtrl,pTotalProgressCtrl,pRegNoDM);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
	
	ShowGrid();

	if( pRegNoDM !=NULL ) 
		delete pRegNoDM;
	pRegNoDM = NULL;

	CBO_LOC_2100* pPar2100 = (CBO_LOC_2100*)pParentMgr;

	pPar2100->m_nMode_Statics	= m_nMode;
	pPar2100->m_sXAlias_Statics	= m_sXAlias;
	pPar2100->m_sYAlias_Statics = m_sYAlias;

	pPar2100->m_cDM_StaticsRegNo.FreeData();
	m_pSpfWork->MakeRegNoFromBLOB( sFieldName , &pPar2100->m_cDM_StaticsRegNo );

	return 0;

	EFS_END
	return -1;

}



VOID CBO_LOC_2190::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

VOID CBO_LOC_2190::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	if( GetSafeHwnd() == NULL ) return;

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

	if( GetDlgItem(IDC_bRUN2)->GetSafeHwnd() == NULL ) return;
	GetDlgItem(IDC_bRUN2)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_bRUN2)->MoveWindow( cx - 18 - WndRect.Width()*2 , cy-35 , WndRect.Width() , WndRect.Height() );

	if( GetDlgItem(IDC_bRUN)->GetSafeHwnd() == NULL ) return;
	GetDlgItem(IDC_bRUN)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_bRUN)->MoveWindow( cx - 36 - WndRect.Width()*3 , cy-35 , WndRect.Width() , WndRect.Height() );

	if( GetDlgItem(IDC_bCLOSE)->GetSafeHwnd() == NULL ) return;
	GetDlgItem(IDC_bCLOSE)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_bCLOSE)->MoveWindow( cx - 10 - WndRect.Width() , cy-35 , WndRect.Width() , WndRect.Height() );

	if( GetDlgItem(IDC_prgSTATICS2)->GetSafeHwnd() == NULL ) return;
	GetDlgItem(IDC_prgSTATICS2)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	INT nProgressWidth = WndRect.Width();
	GetDlgItem(IDC_prgSTATICS2)->MoveWindow( cx - 320 - nProgressWidth , cy-18 , WndRect.Width() , WndRect.Height() );

	if( GetDlgItem(IDC_prgSTATICS)->GetSafeHwnd() == NULL ) return;
	GetDlgItem(IDC_prgSTATICS)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_prgSTATICS)->MoveWindow( cx - 320 - WndRect.Width() - nProgressWidth , cy-18 , WndRect.Width() , WndRect.Height() );

	if( GetDlgItem(IDC_chkSUMZERODEL)->GetSafeHwnd() == NULL ) return;
	GetDlgItem(IDC_chkSUMZERODEL)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_chkSUMZERODEL)->MoveWindow( cx - 320 - nProgressWidth , cy-33 , WndRect.Width() , WndRect.Height() );


	EFS_END
}


INT CBO_LOC_2190::GridColoring(INT nPosition)
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

VOID CBO_LOC_2190::OnLButtonDown(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	HTREEITEM hItem = m_ctrlTree.GetSelectedItem();

	m_ctrlTree.Select( hItem , TVGN_CARET );
	
	CDialog::OnLButtonDown(nFlags, point);

	EFS_END
}


VOID CBO_LOC_2190::OnClose()
{
	EFS_BEGIN

	CDialog::OnClose();

	EFS_END
}

VOID CBO_LOC_2190::OnDestroy() 
{
	EFS_BEGIN

	CDialog::OnDestroy();	

	EFS_END
}

VOID CBO_LOC_2190::OnchkSUMZERODEL() 
{
	EFS_BEGIN

	ShowGrid();

	EFS_END
}

INT CBO_LOC_2190::ShowGrid()
{
	EFS_BEGIN

	INT ids;

	CString sParentCMAlias = _T("CM_BO_LOC_2100_STATICS_GRID");
	CString sParentGridAlias = _T("통계그리드");

	bool bDupDel = TRUE;
	bool bZeroShow = TRUE;
	
	if( ((CButton*)GetDlgItem(IDC_chkSUMZERODEL))->GetCheck() == 1 )
		bZeroShow = FALSE;

	CESL_ControlMgr *pParentCM = pParentMgr->FindCM(sParentCMAlias);
	if( pParentCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pParentCM->FindControl( sParentGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("RunStatics") );

	CMSHFlexGrid *pParentGrid = (CMSHFlexGrid*)pGrid;

	ids = m_pSInfoMgr->StaticsResultViewGrid3(m_sStaticsClass,pParentGrid,bDupDel , bZeroShow , m_pSInfoMgr->GetMenuName());
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );	

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_2190::OnbtnDEBUG() 
{
	EFS_BEGIN

	m_pSInfoMgr->DeBugDM();	
		
	EFS_END
}

VOID CBO_LOC_2190::OnbRUN2() 
{
	EFS_BEGIN

	INT ids;
	
	bool bDupDel = TRUE;
	bool bZeroShow = TRUE;
	
	if( ((CButton*)GetDlgItem(IDC_chkSUMZERODEL))->GetCheck() == 1 )
		bZeroShow = FALSE;
	
	ids = m_pSInfoMgr->PrintResult( _T("장서점검통계") , bDupDel , bZeroShow , NULL );
	if( ids > 0 ) return ;
	
	
	EFS_END
		
	return ;	
}

HBRUSH CBO_LOC_2190::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
