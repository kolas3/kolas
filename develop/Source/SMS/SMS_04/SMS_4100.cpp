// SMS_4100.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_4100.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_4100 dialog


CSMS_4100::CSMS_4100(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_4100)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSMSContentsEditor	=	NULL;
}
CSMS_4100::~CSMS_4100()
{
	if( NULL != m_pSMSContentsEditor )
	{
		delete m_pSMSContentsEditor;
		m_pSMSContentsEditor = NULL;
	}
}


VOID CSMS_4100::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_4100)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_4100, CDialog)
	//{{AFX_MSG_MAP(CSMS_4100)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabUSER, OnSelchangetabUSER)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_4100 message handlers

BOOL CSMS_4100::OnInitDialog() 
{
	CDialog::OnInitDialog();

	INT ids = InitESL_Mgr( _T("SMS_4100") );
	if( ids < 0 ) 
	{
		AfxMessageBox( _T("InitESL_Mgr Error") );
	}
	
	InitScreen();

	const INT gridCnt = 1;
	CString gridAlias[ gridCnt ] = { _T("검색된이용자그리드") };
	CArray< CString, CString >removeListArray;
	CESL_Grid *pGrid = NULL;
	for( INT i = 0; i < gridCnt; i++ )
	{
		pGrid = ( CESL_Grid * )FindControl( _T("CM_SMS_4100"), gridAlias[ i ] );
		if( !pGrid )
			return -1;

		pGrid->InitPopupMenuItem();
		pGrid->AddPopupMenuItem( ( CView * )pMain, this->VIEW_STYLE, &removeListArray );

		if(!m_pInfo->m_bUserInfoExportYN)
		{
			pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
		}
		else
		{
			pGrid->m_bPrivacyPrintLogRecord = TRUE;
			pGrid->m_strPrintInfo = _T("공지사항안내 SMS이력조회");
		}
	}

	InitGroupCode();


	CStringList SMSList;

	SMSList.AddTail( _T("SMS05") );
	SMSList.AddTail( _T("SMS06") );
	SMSList.AddTail( _T("SMS09") );

	m_pSMSContentsEditor = new CSMSContentsEditDlg( this, SMSList );
	m_pSMSContentsEditor->Create( this );
	m_pSMSContentsEditor->CenterWindow();
	m_pSMSContentsEditor->ShowWindow( false );

	GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), m_sIsUnityLoanService);
	if(_T("Y")!=m_sIsUnityLoanService)
	{
		m_sIsUnityLoanService = _T("N");
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



BOOL CSMS_4100::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CSMS_4100::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if( GetSafeHwnd() == NULL ) return;

	CWnd* pWnd[1];
	CRect WndPos[1];

	INT nIDC[1] =
	{
		IDC_MAINGRID 
	};

	for( INT i = 0 ; i < 1 ; i++ )
	{
		
		pWnd[i] = GetDlgItem(nIDC[i]);

		if( pWnd[i]->GetSafeHwnd() == NULL ) return ;

		pWnd[i]->GetWindowRect(WndPos[i]);

	}

	pWnd[0]->MoveWindow( 0 , 0  , cx  , cy );	
	
	
}

INT CSMS_4100::InitScreen()
{
	EFS_BEGIN
	
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabUSER);
	
	pTab->InsertItem( 0 , _T("검색된 이용자") );

	return 0;

	EFS_END

	return -1;
}


INT CSMS_4100::InitMessageTab()
{
	EFS_BEGIN

	
	return 0;

	EFS_END
	
	return -1;
}

INT CSMS_4100::InitUserTab()
{
	EFS_BEGIN


	return 0;

	EFS_END

	return -1;
}

INT CSMS_4100::GetSelectMessageTabIndex()
{
	EFS_BEGIN

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabMESSAGEOPTION);

	return pTab->GetCurSel();

	EFS_END

	return -1;

}


VOID CSMS_4100::OnSelchangetabUSER(NMHDR* pNMHDR, LRESULT* pResult) 
{
	InitUserTab();
	
	*pResult = 0;
}




INT CSMS_4100::InitGroupCode()
{
EFS_BEGIN


	INT ids;

	CString sEcoMailUseYN;
	ids = GetManageValue( _T("기타")  , _T("공통") , _T("메일링시스템") , _T("사용여부") , sEcoMailUseYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitGroupCode") );

	if( sEcoMailUseYN.Compare(_T("Y")) != 0 )
	{
		return 0;
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_USER_GROUP_CODE_INFO"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("InitGroupCode") );

	ids = pDM->RefreshDataManager(_T(""));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("InitGroupCode") ); 
	

	return 0;

EFS_END
return -1;
}

HBRUSH CSMS_4100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


VOID CSMS_4100::ViewSMSContentsEditor()
{
EFS_BEGIN
	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_4100") ); // Jong
	INT		nDMRowCnt	=	pDM->GetRowCount();
	if( 1 > nDMRowCnt )
	{
		AfxMessageBox( _T("문자 발송 대상자를 검색하여 주십시요") );
		return;
	}
	m_pSMSContentsEditor->ShowWindow( TRUE );

EFS_END
}
