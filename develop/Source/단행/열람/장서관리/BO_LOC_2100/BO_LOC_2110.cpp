// BO_LOC_2110.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_2110.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2110 dialog


CBO_LOC_2110::CBO_LOC_2110(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_2110)
	m_pSpfWork = NULL;
	m_bChange = FALSE;
	//}}AFX_DATA_INIT
}

CBO_LOC_2110::~CBO_LOC_2110()
{
	if( m_pSpfWork != NULL)
	{
		delete m_pSpfWork;
		m_pSpfWork = NULL;
	}
}

VOID CBO_LOC_2110::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_2110)
	DDX_Control(pDX, IDC_cPAGE, m_cPageCombo);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_2110, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_2110)
	ON_BN_CLICKED(IDC_btnADD, OnbtnADD)
	ON_BN_CLICKED(IDC_radKEY1, OnradKEY1)
	ON_BN_CLICKED(IDC_radKEY2, OnradKEY2)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnCLEAR, OnbtnCLEAR)
	ON_BN_CLICKED(IDC_btnDELETE, OnbtnDELETE)
	ON_BN_CLICKED(IDC_btnFileLOAD, OnbtnFileLOAD)
	ON_BN_CLICKED(IDC_btnFileSave, OnbtnFileSave)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnSAVE, OnbtnSAVE)
	ON_CBN_SELCHANGE(IDC_cPAGE, OnSelchangecPAGE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_2110 message handlers

BOOL CBO_LOC_2110::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

BOOL CBO_LOC_2110::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_2110")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pSpfWork = new CBLRegNoFileProc( this );
	
	SetInitControl();
	
	m_cPageCombo.EnableWindow( false );
	
	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_LOC_2110_INFO"), _T("등록번호"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);
		pCtrl->SetRegCodeCombo((CWnd*)FindControl(_T("CM_BO_LOC_2110_INFO"), _T("등록구분")), 1);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_LOC_2110_INFO"), _T("바코드"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_2110::OnbtnADD() 
{
	EFS_BEGIN

	INT nMode = GetCurrentAddMode();
	
	CString sSpfPath;
	if( nMode == 0 ) 
		sSpfPath = DM_ONE_KEYBORD_ADD;
	else
		sSpfPath = DM_ONE_BARCODE_ADD;

	INT ids;
	ids = m_pSpfWork->SPFExcutePath(sSpfPath);
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnADD()") );

	if( nMode == 0 )
	{
		GetDlgItem(IDC_edtEDIT1)->SetWindowText(_T(""));
		GetDlgItem(IDC_edtEDIT1)->SetFocus();
	}
	if( nMode == 1 )
	{
		GetDlgItem(IDC_edtEDIT2)->SetWindowText(_T(""));
		GetDlgItem(IDC_edtEDIT1)->SetFocus();
	}

	m_bChange = TRUE;
	
	ShowGrid( true, true );

	EFS_END
}

INT CBO_LOC_2110::GetCurrentAddMode()
{
	EFS_BEGIN

	INT nMode = 0;
	if( ((CButton*)GetDlgItem( IDC_radKEY1 ))->GetCheck() == 1 )
		nMode = 0;
	else 
		nMode = 1;

	return nMode;

	EFS_END
	return -1;

}

VOID CBO_LOC_2110::SetInitControl()
{
	EFS_BEGIN

	((CButton*)GetDlgItem(IDC_radKEY1))->SetCheck(1);

	SetDisableControl();

	EFS_END
}

VOID CBO_LOC_2110::SetDisableControl()
{
	EFS_BEGIN

	INT nMode = 0;

	if( ((CButton*)GetDlgItem(IDC_radKEY1))->GetCheck() == 1 )
		nMode = 0;
	else
		nMode = 1;

	const INT nCnt = 3;
	INT nIDCGroup[nCnt] =
	{
		IDC_cmbEDIT , IDC_edtEDIT1 , IDC_edtEDIT2
	};

	INT nFirst = 0;
	INT nLast = 0;
	if( nMode == 0 ) 
	{
		nFirst = 0;
		nLast = 2;
	}
	else
	{
		nFirst = 2;
		nLast = 3;
	}

	for( INT i=0 ; i < nCnt ; i++ )
	{
		GetDlgItem(nIDCGroup[i])->EnableWindow(FALSE);
	}
	
	for( i=nFirst ; i < nLast ; i++ )
	{
		GetDlgItem(nIDCGroup[i])->EnableWindow(TRUE);
	}
	
	

	EFS_END
}

VOID CBO_LOC_2110::OnradKEY1() 
{
	EFS_BEGIN

	SetDisableControl();
	

	EFS_END
}

VOID CBO_LOC_2110::OnradKEY2() 
{
	EFS_BEGIN

	SetDisableControl();	

	EFS_END
}

VOID CBO_LOC_2110::OnbtnCLOSE() 
{
	EFS_BEGIN

	INT ids;

	ids = m_pSpfWork->SPFExcutePath(SAVE_REGNO_EDIT);
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnFileSave()") );		
	
	OnCancel();

	EFS_END
}

VOID CBO_LOC_2110::OnbtnCLEAR() 
{
	EFS_BEGIN

	INT ids;
	ids = m_pSpfWork->SPFExcutePath(DM_CLEAR);
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnCLEAR()") );

	m_bChange = TRUE;

	m_cPageCombo.EnableWindow( false );
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gridEDIT );
	pGrid->SetRows( 2 );
	pGrid->SetTextMatrix( 1, 0, _T("") );
	pGrid->SetTextMatrix( 1, 1, _T("") );
	pGrid->SetTextMatrix( 1, 2, _T("") );

	( (CEdit*) GetDlgItem (IDC_edtEDIT3) )->SetWindowText(_T("0"));

	EFS_END
}

VOID CBO_LOC_2110::OnbtnDELETE() 
{
	EFS_BEGIN

	DeleteRegNo();

	m_bChange = TRUE;

	EFS_END
}

VOID CBO_LOC_2110::OnbtnFileLOAD() 
{
	EFS_BEGIN

	CButton *pbCheck = ( CButton * )GetDlgItem( IDC_cCHECK_REG_NO );
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cCHECK_REGNO );
	if( pbCheck->GetCheck() )
		pCombo->SetCurSel( 0 );
	else
		pCombo->SetCurSel( 1 );
		
	INT ids;
	BOOL bCheck;

	bCheck = ( (CButton*) GetDlgItem(IDC_cCHECK_REG_NO2) )->GetCheck();
	if ( bCheck == TRUE )
	{
		ids = m_pSpfWork->SPFExcutePath(DM_LOAD_FILE);
	}
	else
	{
		ids = m_pSpfWork->SPFExcutePath(DM_LOAD_FILE2);
	}
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnFileLOAD()") );

	m_bChange = TRUE;

	if( !ids )
		ShowGrid();
	
	EFS_END
}

VOID CBO_LOC_2110::OnbtnFileSave() 
{
	EFS_BEGIN

	INT ids;
	ids = m_pSpfWork->SPFExcutePath(DM_SAVE_FILE);
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnFileSave()") );	
	

	EFS_END
}

VOID CBO_LOC_2110::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	
	INT ids;
	ids = m_pSpfWork->SPFExcutePath(OPEN_REGNO_EDIT);
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnFileSave()") );		

	m_bChange = FALSE;

	EFS_END
}

VOID CBO_LOC_2110::OnbtnSAVE() 
{
	EFS_BEGIN

	INT ids;
	ids = m_pSpfWork->SPFExcutePath(SAVE_REGNO_EDIT);
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnFileSave()") );		

	m_bChange = FALSE;

	EFS_END
}

BOOL CBO_LOC_2110::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
	{
		OnbtnCLOSE();
		return TRUE;
	}
	else if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		DWORD lstyle = GetStyle();
		HWND MsgH = pMsg->hwnd;
		if(	GetDlgItem(IDC_edtEDIT1)->m_hWnd==MsgH					|| 
			GetDlgItem(IDC_edtEDIT2)->m_hWnd==MsgH
		)
		{
			OnbtnADD();
			return TRUE;
		}
	}
	
	
	return CESL_Mgr::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_2110::ShowGrid( bool flag /* = true */, bool bRemoveDuplicateRegNo )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_2100_REG_NO") );
	INT rowCnt = pDM->GetRowCount();

	CString regNo, pastRegNo = _T("-1");

	rowCnt = pDM->GetRowCount();
	MakeRegNoPageCombo( ( rowCnt % PAGE_LIST_CNT ? rowCnt / PAGE_LIST_CNT + 1 : rowCnt / PAGE_LIST_CNT ) );

	CString strCurCnt;
	INT		nCurCnt;

	( (CEdit*) GetDlgItem ( IDC_edtEDIT3) )->GetWindowText(strCurCnt);
	nCurCnt = _ttoi(strCurCnt);	

	CString sRegNoCnt;
	sRegNoCnt.Format( _T("%d"), rowCnt );
	SetDlgItemText( IDC_edtEDIT3, sRegNoCnt );
	
	ViewGrid();
}

VOID CBO_LOC_2110::MakeRegNoPageCombo( INT pageCnt )
{
	m_cPageCombo.EnableWindow( true );
	CString sPage;
	m_cPageCombo.ResetContent();
	for( INT i = 0; i < pageCnt; i++ )
	{
		sPage.Format( _T("%d"), i + 1 );
		m_cPageCombo.InsertString( i, sPage );
	}

	m_cPageCombo.SetCurSel( 0 );
}

VOID CBO_LOC_2110::ViewGrid()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_2100_REG_NO") );	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gridEDIT );

	if( pDM->GetRowCount() == 0 )
	{
		OnbtnCLEAR();		
		return ;
	}

	INT page = m_cPageCombo.GetCurSel();

	INT from = page * PAGE_LIST_CNT;
	INT to = from + PAGE_LIST_CNT;

	
	INT lastIndex = pDM->GetRowCount() - 1;
	if( to > lastIndex )
		to = lastIndex + 1;
	
	pGrid->SetRows( to - from + 1 );
	CString rowNum;
	INT gridRow = 1;
	for( INT i = from; i < to; i++ )
	{
		rowNum.Format( _T("%d"), i + 1 );

		pGrid->SetTextMatrix( gridRow, 0, rowNum );
		pGrid->SetTextMatrix( gridRow, 1, _T("") );
		pGrid->SetTextMatrix( gridRow++, 2, pDM->GetCellData( _T("등록번호"), i ) );
	}
}

VOID CBO_LOC_2110::OnSelchangecPAGE() 
{
	// TODO: Add your control notification handler code here
	
	ViewGrid();
}

VOID CBO_LOC_2110::DeleteRegNo()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_2100_REG_NO") );

	if( pDM->GetRowCount() == 0 )
		return;

	CArray< INT, INT >checkedRow;
	if( !GetCheckedRow( checkedRow ) )
		return;

	for( INT i = checkedRow.GetSize() - 1; i > -1; i-- )
		pDM->DeleteRow( checkedRow.GetAt( i ) );

	ShowGrid( false, false );

	m_pSpfWork->DeleteGrid_UpdateHash();
}


INT CBO_LOC_2110::GetCheckedRow( CArray< INT, INT >&checkedRow )
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gridEDIT );

	for( INT i = 1; i < pGrid->GetRows(); i++ )
		if( _T("V") == pGrid->GetTextMatrix( i, 1 ) )
			checkedRow.Add( _ttoi( pGrid->GetTextMatrix( i, 0 ) ) - 1 );

	return checkedRow.GetSize();
}

BEGIN_EVENTSINK_MAP(CBO_LOC_2110, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_2110)
	ON_EVENT(CBO_LOC_2110, IDC_gridEDIT, -600 /* Click */, OnClickgridEDIT, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_2110::OnClickgridEDIT() 
{
	// TODO: Add your control notification handler code here
	
	CheckGridV();
}

VOID CBO_LOC_2110::CheckGridV()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gridEDIT );
	
	if( 1 != pGrid->GetCol() )
		return;
	
	if( _T("V") == pGrid->GetText() )
		pGrid->SetText( _T("") );
	else
		pGrid->SetText( _T("V") );
}

INT CBO_LOC_2110::SortDMByAlias( CESL_DataMgr *pDM, CString alias )
{
	INT col = pDM->FindColumn( alias );	
	if( col < 0 )
		return -1;
	
	return QSort( pDM, 0, pDM->GetRowCount() - 1, col );
}

INT CBO_LOC_2110::SwapDMRecord( CESL_DataMgr *pDM, INT from, INT to )
{
	if( from < 0 || to < 0 )
		return -1;

	DWORD *pFromRow	= ( DWORD * )pDM->data.GetAt( pDM->m_pDataPosition[ from ] );
	DWORD *pToRow	= ( DWORD * )pDM->data.GetAt( pDM->m_pDataPosition[ to	 ] );

	pDM->data.SetAt( pDM->m_pDataPosition[ from ], ( CObject * )pToRow	 );
	pDM->data.SetAt( pDM->m_pDataPosition[ to	], ( CObject * )pFromRow );
	
	return 0;
}

INT CBO_LOC_2110::QSort( CESL_DataMgr *pDM, INT left, INT right, INT col )
{
	if( left >= right )
		return 0;

	SwapDMRecord( pDM, left, ( left + right ) / 2 );
	INT last = left;

	for( INT i = left + 1; i <= right; i++ )
	{
		CString b1;
		b1.Format( _T("%s"), ( TCHAR * )( ( ( DWORD * )pDM->data.GetAt( pDM->m_pDataPosition[ i	 ] ) )[ col ] ) );
		CString b2;
		b2.Format( _T("%s"), ( TCHAR * )( ( ( DWORD * )pDM->data.GetAt( pDM->m_pDataPosition[ left ] ) )[ col ] ) );
		
		if( _tcscmp(	( TCHAR * )( ( ( DWORD * )pDM->data.GetAt( pDM->m_pDataPosition[ i	 ] ) )[ col ] ),
					( TCHAR * )( ( ( DWORD * )pDM->data.GetAt( pDM->m_pDataPosition[ left ] ) )[ col ] ) ) < 0 )
			SwapDMRecord( pDM, ++last, i );
	}

	SwapDMRecord( pDM, left, last );
	QSort( pDM, left, last - 1, col );
	QSort( pDM, last + 1, right, col );
	
	return 0;
}

VOID CBO_LOC_2110::DeleteRow( CESL_DataMgr *pDM, INT index )
{
	if( index > pDM->data.GetCount() )
		return;

	DWORD *pRow  = NULL;
	POSITION pos = pDM->m_pDataPosition[ index ];
	if( !pos )
		return;

	pRow = ( DWORD * )pDM->data.GetAt( pos );
	if( pRow )
		pDM->FreeRow( pRow, pDM->RefList.GetCount() );
	pDM->data.RemoveAt( pos );
}

HBRUSH CBO_LOC_2110::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
