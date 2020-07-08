// BO_LOC_3230.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3230.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3230 dialog


CBO_LOC_3230::CBO_LOC_3230(CESL_Mgr* pParent /*=NULL*/ , CLonaManageValue *pLona, CLoanShareManager* pLoanShareManager)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3230)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLoanShareManager = pLoanShareManager;

	m_pSpfWork = NULL;
	m_pSpfWork = new CBL_LOC_LOAN_MNG_PROC(this,pLona,m_pLoanShareManager);

	// KOL.UDF.022
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

	m_sIsUnityLoanService = pLona->m_sIsUnityLoanService;
}

CBO_LOC_3230::~CBO_LOC_3230()
{

	if( m_pSpfWork != NULL ) 
		delete m_pSpfWork;
	m_pSpfWork = NULL;
}

VOID CBO_LOC_3230::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3230)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3230, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3230)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnClose, OnbtnClose)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3230 message handlers

BOOL CBO_LOC_3230::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3230::OnClose() 
{
	EFS_BEGIN

	OnCancel();

	EFS_END
}

BOOL CBO_LOC_3230::OnInitDialog() 
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
	m_ViewerControl.SetViewerWidth( 8 , 90 );

	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridLOG);
	pGrid->SetRowHeight(0, 400);
	
	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_3230") );
	CESL_Grid *pGrid2 = (CESL_Grid*)(pCM->FindControl( _T("HistoryGrid") ));
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		pGrid2->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid2->m_bPrivacyPrintLogRecord = TRUE;
		pGrid2->m_strPrintInfo = _T("대출자료관리 이력보기");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3230::OnShowWindow(BOOL bShow, UINT nStatus) 
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

INT CBO_LOC_3230::InitSetStartPos()
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

VOID CBO_LOC_3230::OnbtnPREV() 
{
	EFS_BEGIN

	MoveIndex( 0 );	

	EFS_END
}

VOID CBO_LOC_3230::OnbtnNEXT() 
{
	EFS_BEGIN

	MoveIndex( 1 );

	EFS_END
}


CESL_Grid* CBO_LOC_3230::GetMainGridPtr()
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

INT CBO_LOC_3230::MoveIndex( INT nMode )
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

INT CBO_LOC_3230::ViewCurrentWindow()
{
EFS_BEGIN

	INT ids;

	SetMemberVariable();	
	
	CString sWhere;
	CString sBookRecKey;

	ids = GetDataMgrData( m_sParentDMAlias , _T("대출책KEY") , sBookRecKey , m_nCurrentIndex );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ViewCurrentWindow") );

	CString sDMAlias = m_sClientDMAlias;
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ViewCurrentWindow") );

	CESL_DataMgr *pDstDM = FindDM( _T("DM_BO_LOC_3230") );
	if( NULL == pDstDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ViewCurrentWindow") );

	CString sPublishForm;
	ids = GetPublishForm( sPublishForm );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("ViewCurrentWindow") );

	if( sPublishForm.Compare(_T("단행"))==0)
		sWhere.Format( _T(" B.REC_KEY = %s ") , sBookRecKey );
	else if( sPublishForm.Compare(_T("긴급"))==0)
		sWhere.Format( _T(" NB.REC_KEY = %s ") , sBookRecKey );
	else if( sPublishForm.Compare(_T("연속"))==0)
		sWhere.Format( _T(" B.REC_KEY = %s ") , sBookRecKey );

	ids = CLocCommonAPI::RefreshDataManager( this , pDM , sWhere );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("ViewCurrentWindow") );

	CString strValue1;
	ids = GetManageValue( _T("열람") , _T("공통") , _T("이용자관리") , _T("대출화면주민번호표시유무") , strValue1 );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("ViewCurrentWindow") );

	strValue1.TrimLeft();		strValue1.TrimRight();		strValue1.MakeUpper();

	CString sMemberClass;
	CString sLoanStatus;
	CString sMailSendDate;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("구분") , i , sMemberClass );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("ViewCurrentWindow") );
		
		ids = m_pSpfWork->ConvertMemberClassCodeToDesc( 0 , sMemberClass );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("ViewCurrentWindow") );

		ids = pDM->SetCellData( _T("구분") , sMemberClass , i);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("ViewCurrentWindow") );

		ids = pDM->GetCellData( _T("대출상태") , i , sLoanStatus );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1100 , _T("ViewCurrentWindow") );
		
		CString sLoanStatusDesc;
		ids = m_pSpfWork->ConvertLoanStatusCodeToDesc( 0 , sLoanStatusDesc, sLoanStatus);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1110 , _T("ViewCurrentWindow") );

		ids = pDM->SetCellData( _T("대출상태") , sLoanStatus , i);
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1120 , _T("ViewCurrentWindow") );

		if( _T("Y") != strValue1 )
		{
			CString strUserNo;
			ids = pDM->GetCellData( _T("대출자번호") , i , strUserNo );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1130 , _T("ViewCurrentWindow") );

			CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_sIsUnityLoanService );
			ids = pDM->SetCellData( _T("대출자번호") , strUserNo , i );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1160 , _T("ViewCurrentWindow") );
		}
	}

	pDstDM->FreeData();

	CLocCommonAPI::AddDM( pDM , pDstDM , this );

	pDstDM->SORT(_T("대출일") , TRUE );

	CString sCMAlias = m_sClientCMAlias;
	ids = AllControlDisplay( sCMAlias , m_nCurrentIndex );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1170 , _T("ViewCurrentWindow") );

	CString sEtcCMAlias = m_sClientEtcCMAlias;
	ids = AllControlDisplay( sEtcCMAlias , 0 );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1180 , _T("ViewCurrentWindow") );

	DisplayBookStatus();

	CESL_Grid *pGrid = GetMainGridPtr();
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1190 , _T("ViewCurrentWindow") );

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

VOID CBO_LOC_3230::OnbtnClose() 
{
	EFS_BEGIN

	OnClose();

	EFS_END
}


INT CBO_LOC_3230::CheckParentDMCount()
{
	EFS_BEGIN

	CESL_DataMgr *pParentDM = FindDM( m_sParentDMAlias );

	return pParentDM->GetRowCount();

	EFS_END
	return -1;

}


INT CBO_LOC_3230::GetPublishForm( CString &sPublishForm )
{
	EFS_BEGIN

	INT ids;

	ids = GetDataMgrData( m_sParentDMAlias  , _T("자료구분") , sPublishForm , m_nCurrentIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetPublishForm") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3230::SetMemberVariable()
{
	EFS_BEGIN

	INT ids;

	CString sPublishForm;
	ids = GetPublishForm( sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3220") );

	if( sPublishForm.Compare(_T("단행")) == 0 )
		m_sClientDMAlias = _T("DM_BO_LOC_3230_BO");
	else if( sPublishForm.Compare(_T("긴급")) == 0 )
		m_sClientDMAlias = _T("DM_BO_LOC_3230_NON_DB");
	else if( sPublishForm.Compare(_T("연속")) == 0 )
		m_sClientDMAlias = _T("DM_BO_LOC_3230_SE");

	m_sClientCMAlias = _T("CM_BO_LOC_3230");
	m_sClientEtcCMAlias = _T("CM_BO_LOC_3230_ETC_INFO");

	return 0;

	EFS_END
	return -1;

}


INT CBO_LOC_3230::SetMemberVariable2()
{
	EFS_BEGIN

	m_sParentCMAlias = _T("CM_BO_LOC_3200");
	m_sParentDMAlias = _T("DM_BO_LOC_3200");
	m_sGridAlias = _T("MainGrid");
	m_sSMAlias = _T("BO_LOC_3230");

	return 0;

	EFS_END
	return -1;

}


INT CBO_LOC_3230::DisplayBookStatus()
{
	INT ids;
	
	CString sDMAlias = _T("DM_BO_LOC_3230");
	
	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DisplayBookStatus") );
	
	const INT nFieldCnt = 5;
	TCHAR* sDMFieldName[nFieldCnt] =
	{
		_T("서명") , _T("저작자") , _T("발행자") , _T("등록번호") , _T("배가상태") 
	};
	
	TCHAR* sTitleName[nFieldCnt] =
	{
		_T("서  명") , _T("저작자") , _T("발행자") , _T("등록번호") , _T("배가상태") 
	};
	
	
	CString sData;
	for( INT i = 0 ; i < nFieldCnt ; i++ )
	{
		ids = pDM->GetCellData(sDMFieldName[i] , 0 , sData );
		if( ids < 0 ) 
		{
			CString sEMsg;
			sEMsg.Format( _T("pDM->GetCellData(sDMFieldName[i] , m_nCurrentIndex , sData ) \r\n sDMFieldName[i] : %s \r\n m_nCurrentIndex : %d ") , 
				sDMFieldName[i] , 0 );
			AfxMessageBox( sEMsg );
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DisplayBookStatus") );
		
		if( i == 4 )
		{
			ids = CLocCommonAPI::GetWorkingStatusDescToCode( sData , 1);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7, _T("CBO_LOC_3230::ViewCurrentWindow()") ); 
		}

		m_ViewerControl.SetControlData( sTitleName[i] , sData );
		
	}
	
	m_ViewerControl.Display();
	
	return 0;
}

HBRUSH CBO_LOC_3230::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
