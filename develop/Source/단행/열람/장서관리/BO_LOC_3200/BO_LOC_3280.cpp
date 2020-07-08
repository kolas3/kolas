// BO_LOC_3280.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3280.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3280 dialog


CBO_LOC_3280::CBO_LOC_3280(CESL_Mgr* pParent /*=NULL*/ , CLonaManageValue *pLona, CLoanShareManager* pLoanShareManager)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3280)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLoanShareManager = pLoanShareManager;

	m_pSpfWork = NULL;
	m_nViewMode = 0;
	m_nCurrentIndex = 0;
	m_nLastIndex = 0;
	m_nFirstIndex = 0;
	bFirstView = FALSE;
	m_sParentDMAlias = _T("");
	m_sParentCMAlias = _T("");
	m_sGridAlias = _T("");
	m_sClientCMAlias = _T("");
	m_sClientDMAlias = _T("");
	m_sSMAlias = _T("");
	m_sClientEtcCMAlias = _T("");


	SetMemberVariable2();
}
CBO_LOC_3280::~CBO_LOC_3280()
{
}

VOID CBO_LOC_3280::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3280)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3280, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3280)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnClose, OnbtnClose)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3280 message handlers



BOOL CBO_LOC_3280::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN
		
		// TODO: Add your specialized code here and/or call the base class
		
		return CDialog::Create(IDD, pParentWnd);
	
	EFS_END
		return FALSE;
	
}



BOOL CBO_LOC_3280::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(m_sSMAlias) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	m_ViewerControl.InitViewer( IDC_gridLOG2 , this );
	m_ViewerControl.SetViewerWidth( 20 , 75 );

	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridLOG);
	pGrid->SetRowHeight(0, 400);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3280::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);

	INT ids;
	
	bFirstView = TRUE;

	InitSetStartPos();

	ids = ViewCurrentWindow();
	if( ids < 0 ) return;

	bFirstView = FALSE;
	

	EFS_END
}

INT CBO_LOC_3280::InitSetStartPos()
{
	EFS_BEGIN

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) return -1;

	INT nTmpIndex;
	if( bFirstView ) nTmpIndex = pGrid->GetIdx();
	else nTmpIndex = pGrid->SelectGetIdx();

	pGrid->SelectMakeList();
	INT nSelectCount = pGrid->SelectGetCount();
	if( nSelectCount > 0 ) m_nViewMode = 1;
	else m_nViewMode = 0;

	if( m_nViewMode == 0 )
	{
		m_nCurrentIndex = pGrid->GetIdx();
		m_nLastIndex = pGrid->GetCount() - 1;
		m_nFirstIndex = 0;
	}
	else
	{
		m_nLastIndex = pGrid->SelectGetTailPosition();
		m_nFirstIndex = pGrid->SelectGetHeadPosition();
		for( INT i = 0 ; i < nSelectCount-1 ; i++ )
		{
			if( nTmpIndex == pGrid->SelectGetIdx() )
				break;
			pGrid->SelectGetNext();
		}
		if( i==nSelectCount ) m_nCurrentIndex = pGrid->SelectGetHeadPosition();
		else m_nCurrentIndex = pGrid->SelectGetIdx();
	}
	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3280::OnbtnPREV() 
{
	EFS_BEGIN

	MoveIndex( 0 );	

	EFS_END
}

VOID CBO_LOC_3280::OnbtnNEXT() 
{
	EFS_BEGIN

	MoveIndex( 1 );

	EFS_END
}


CESL_Grid* CBO_LOC_3280::GetMainGridPtr()
{
	EFS_BEGIN

	CESL_ControlMgr *pCM = FindCM( m_sParentCMAlias );
	if( pCM == NULL ) return NULL;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( m_sGridAlias ));
	if( pGrid == NULL ) return NULL;

	return pGrid;

	EFS_END
	return NULL;

}

INT CBO_LOC_3280::MoveIndex( INT nMode )
{	
	EFS_BEGIN

	INT ids;
	ids = InitSetStartPos();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3220::MoveIndex( INT nMode )") );

	if( m_nViewMode == 0 )
	{
		if( nMode == 0 && ( m_nCurrentIndex != m_nFirstIndex )) m_nCurrentIndex--;
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) ) m_nCurrentIndex++;
	}
	else
	{
		CESL_Grid *pGrid = GetMainGridPtr();
		if( nMode == 0 )
		{
			m_nCurrentIndex = pGrid->SelectGetPrev();
		}
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) )
		{
			m_nCurrentIndex = pGrid->SelectGetNext();
		}
	}

	ViewCurrentWindow();

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3280::ViewCurrentWindow()
{
	EFS_BEGIN

	INT ids;

	SetMemberVariable();	
	
	CString sWhere;
	CString sLoanKey;

	ids = GetDataMgrData( m_sParentDMAlias , _T("대출/반납정보KEY") , sLoanKey , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_1320::ViewCurrentWindow()") );

	CString sDMAlias = m_sClientDMAlias;
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_1320::ViewCurrentWindow()") );

	CString sPublishForm;
	ids = GetPublishForm( sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3280::ViewCurrentWindow()") );

	sWhere.Format( _T(" CLMS.LOAN_KEY = %s ") , sLoanKey );

	ids = CLocCommonAPI::RefreshDataManager( this , pDM , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3280::ViewCurrentWindow()") );

	CString sMailClass;
	CString sLoanStatus;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("메일종류") , i , sMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3280::ViewCurrentWindow()") );
		
		ids = CLocCommonAPI::ConvertMailClassCodeToDesc( this, sMailClass , 1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_3280::ViewCurrentWindow()") ); 

		ids = pDM->SetCellData( _T("메일종류") , sMailClass , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3280::ViewCurrentWindow()") ); 

	}

	CString sCMAlias = m_sClientCMAlias;
	ids = AllControlDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3220::ViewCurrentWindow()") );

	DisplayBookStatus();

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CBO_LOC_3220::ViewCurrentWindow()") );

	GetDlgItem(IDC_btnPREV)->EnableWindow( TRUE );
	GetDlgItem(IDC_btnNEXT)->EnableWindow( TRUE );

	if( m_nCurrentIndex == m_nFirstIndex )
		GetDlgItem(IDC_btnPREV)->EnableWindow( FALSE );	
	if( m_nCurrentIndex == m_nLastIndex )
		GetDlgItem(IDC_btnNEXT)->EnableWindow( FALSE );
	
	pGrid->SetReverse( m_nCurrentIndex );

	return 0;


	EFS_END
	return -1;

}

VOID CBO_LOC_3280::OnbtnClose() 
{
	EFS_BEGIN

	OnCancel();

	EFS_END
}


INT CBO_LOC_3280::CheckParentDMCount()
{
	EFS_BEGIN

	CESL_DataMgr *pParentDM = FindDM( m_sParentDMAlias );

	return pParentDM->GetRowCount();

	EFS_END
	return -1;

}


INT CBO_LOC_3280::GetPublishForm( CString &sPublishForm )
{
	EFS_BEGIN

	INT ids;

	ids = GetDataMgrData( m_sParentDMAlias  , _T("자료구분") , sPublishForm , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetPublishForm") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3280::SetMemberVariable()
{
	EFS_BEGIN

	INT ids;

	CString sPublishForm;
	ids = GetPublishForm( sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3220") );

	m_sClientDMAlias = _T("DM_BO_LOC_3280");
	m_sClientCMAlias = _T("CM_BO_LOC_3280");
	return 0;

	EFS_END
	return -1;

}


INT CBO_LOC_3280::SetMemberVariable2()
{
	EFS_BEGIN

	m_sParentCMAlias = _T("CM_BO_LOC_3200");
	m_sParentDMAlias = _T("DM_BO_LOC_3200");
	m_sGridAlias = _T("MainGrid");
	m_sSMAlias = _T("BO_LOC_3280");

	return 0;

	EFS_END
	return -1;

}


INT CBO_LOC_3280::DisplayBookStatus()
{
	INT ids;
	
	CString sDMAlias = _T("DM_BO_LOC_3200");
	
	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DisplayBookStatus") );
	
	const INT nFieldCnt = 6;
	TCHAR* sDMFieldName[nFieldCnt] =
	{
		_T("자료구분") , _T("본표제") , _T("저작자") , _T("발행자") , _T("등록번호") , _T("배가상태") 
	};
	
	TCHAR* sTitleName[nFieldCnt] =
	{
		_T("자료구분") , _T("서  명") , _T("저작자") , _T("발행자") , _T("등록번호") , _T("배가상태") 
	};
	
	
	CString sData;
	for( INT i = 0 ; i < nFieldCnt ; i++ )
	{
		ids = pDM->GetCellData(sDMFieldName[i] , m_nCurrentIndex , sData );
		if( ids < 0 ) 
		{
			CString sEMsg;
			sEMsg.Format( _T("pDM->GetCellData(sDMFieldName[i] , m_nCurrentIndex , sData ) \r\n sDMFieldName[i] : %s \r\n m_nCurrentIndex : %d ") , 
				sDMFieldName[i] , 0 );
			AfxMessageBox( sEMsg );
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DisplayBookStatus") );
		
		m_ViewerControl.SetControlData( sTitleName[i] , sData );
		
	}
	
	m_ViewerControl.Display();
	
	return 0;
}


HBRUSH CBO_LOC_3280::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
