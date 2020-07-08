// MNGCommonUserDefineCode.cpp : implementation file
//

#include "stdafx.h"
#include "MNGCommonUserDefineCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNGCommonUserDefineCode dialog


CMNGCommonUserDefineCode::CMNGCommonUserDefineCode(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNGCommonUserDefineCode)
	m_sCodeAlias1 = _T("");
	m_sCodeAlias2 = _T("");
	m_sCodeAlias3 = _T("");
	m_sCodeAlias4 = _T("");
	m_sCodeAlias5 = _T("");
	m_sCodeAlias6 = _T("");
	m_sCodeAlias7 = _T("");
	m_sCodeAlias8 = _T("");
	m_sCodeAlias9 = _T("");
	m_nCode1UseYN = -1;
	m_nCode2UseYN = -1;
	m_nCode3UseYN = -1;
	m_nCode4UseYN = -1;
	m_nCode5UseYN = -1;
	m_nCode6UseYN = -1;	
	m_nCode7UseYN = -1;
	m_nCode8UseYN = -1;
	m_nCode9UseYN = -1;
	//}}AFX_DATA_INIT
}
CMNGCommonUserDefineCode::~CMNGCommonUserDefineCode()
{
	
}

VOID CMNGCommonUserDefineCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNGCommonUserDefineCode)
	DDX_Text(pDX, IDC_edtCODE_ALIAS1, m_sCodeAlias1);
	DDX_Text(pDX, IDC_edtCODE_ALIAS2, m_sCodeAlias2);
	DDX_Text(pDX, IDC_edtCODE_ALIAS3, m_sCodeAlias3);
	DDX_Text(pDX, IDC_edtCODE_ALIAS4, m_sCodeAlias4);
	DDX_Text(pDX, IDC_edtCODE_ALIAS5, m_sCodeAlias5);
	DDX_Text(pDX, IDC_edtCODE_ALIAS6, m_sCodeAlias6);
	DDX_Text(pDX, IDC_edtCODE_ALIAS7, m_sCodeAlias7);
	DDX_Text(pDX, IDC_edtCODE_ALIAS8, m_sCodeAlias8);
	DDX_Text(pDX, IDC_edtCODE_ALIAS9, m_sCodeAlias9);
	DDX_Radio(pDX, IDC_radUSER_DEFINE_BOOK_CODE1_USE_Y, m_nCode1UseYN);
	DDX_Radio(pDX, IDC_radUSER_DEFINE_BOOK_CODE2_USE_Y, m_nCode2UseYN);
	DDX_Radio(pDX, IDC_radUSER_DEFINE_BOOK_CODE3_USE_Y, m_nCode3UseYN);
	DDX_Radio(pDX, IDC_radUSER_DEFINE_LOANER_CODE1_USE_Y, m_nCode4UseYN);
	DDX_Radio(pDX, IDC_radUSER_DEFINE_LOANER_CODE2_USE_Y, m_nCode5UseYN);
	DDX_Radio(pDX, IDC_radUSER_DEFINE_LOANER_CODE3_USE_Y, m_nCode6UseYN);	
	DDX_Radio(pDX, IDC_radUSER_DEFINE_SERIES_CODE1_USE_Y, m_nCode7UseYN);
	DDX_Radio(pDX, IDC_radUSER_DEFINE_SERIES_CODE2_USE_Y, m_nCode8UseYN);
	DDX_Radio(pDX, IDC_radUSER_DEFINE_SERIES_CODE3_USE_Y, m_nCode9UseYN);
	//}}AFX_DATA_MAP
}

BOOL CMNGCommonUserDefineCode::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

BEGIN_MESSAGE_MAP(CMNGCommonUserDefineCode, CDialog)
	//{{AFX_MSG_MAP(CMNGCommonUserDefineCode)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_BOOK_CODE1_USE_N, OnradChange)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_BOOK_CODE1_USE_Y, OnradChange)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_BOOK_CODE2_USE_N, OnradChange)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_BOOK_CODE2_USE_Y, OnradChange)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_BOOK_CODE3_USE_N, OnradChange)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_BOOK_CODE3_USE_Y, OnradChange)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_LOANER_CODE1_USE_N, OnradChange)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_LOANER_CODE1_USE_Y, OnradChange)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_LOANER_CODE2_USE_N, OnradChange)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_LOANER_CODE2_USE_Y, OnradChange)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_LOANER_CODE3_USE_N, OnradChange)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_LOANER_CODE3_USE_Y, OnradChange)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_SERIES_CODE1_USE_N, OnradChange)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_SERIES_CODE1_USE_Y, OnradChange)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_SERIES_CODE2_USE_N, OnradChange)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_SERIES_CODE2_USE_Y, OnradChange)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_SERIES_CODE3_USE_N, OnradChange)
	ON_BN_CLICKED(IDC_radUSER_DEFINE_SERIES_CODE3_USE_Y, OnradChange)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNGCommonUserDefineCode message handlers

BOOL CMNGCommonUserDefineCode::OnInitDialog() 
{
EFS_BEGIN
	CDialog::OnInitDialog();
	
	INT ids = -1;

	if ( InitESL_Mgr( _T("K2UPGRADE_공통환경관리")) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error") );
		return FALSE;
	}

	ids = ViewScreen( );
	if ( ids < 0 ) return FALSE;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
EFS_END
return FALSE;
}

VOID CMNGCommonUserDefineCode::OnradChange() 
{
	UpdateData( TRUE );

	if ( 0==m_nCode1UseYN )	((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS1))->EnableWindow( TRUE );
	else					((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS1))->EnableWindow( FALSE );

	if ( 0==m_nCode2UseYN )	((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS2))->EnableWindow( TRUE );
	else					((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS2))->EnableWindow( FALSE );

	if ( 0==m_nCode3UseYN )	((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS3))->EnableWindow( TRUE );
	else					((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS3))->EnableWindow( FALSE );

	if ( 0==m_nCode4UseYN )	((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS4))->EnableWindow( TRUE );
	else					((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS4))->EnableWindow( FALSE );

	if ( 0==m_nCode5UseYN )	((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS5))->EnableWindow( TRUE );
	else					((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS5))->EnableWindow( FALSE );

	if ( 0==m_nCode6UseYN )	((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS6))->EnableWindow( TRUE );
	else					((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS6))->EnableWindow( FALSE );

	if ( 0==m_nCode7UseYN )	((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS7))->EnableWindow( TRUE );
	else					((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS7))->EnableWindow( FALSE );

	if ( 0==m_nCode8UseYN )	((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS8))->EnableWindow( TRUE );
	else					((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS8))->EnableWindow( FALSE );

	if ( 0==m_nCode9UseYN )	((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS9))->EnableWindow( TRUE );
	else					((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS9))->EnableWindow( FALSE );

	UpdateData( FALSE );
}

INT CMNGCommonUserDefineCode::ViewScreen()
{
EFS_BEGIN
	INT ids = -1;
	CString sCodeYN = _T("");
	CString sClassAlias = _T("");

	////////////////////////////////////////////////////
	// 도서에 대한 사용자정의 코드 사용여부 가져오기
	////////////////////////////////////////////////////
	ids = GetCodeManageValue( _T("91"), sCodeYN, sClassAlias );
	if ( ids < 0 ) return -10;
	else if ( _T("Y")==sCodeYN )
	{
		m_nCode1UseYN = 0;
		m_sCodeAlias1 = sClassAlias;
	}
	else if ( _T("N")==sCodeYN )
	{
		m_nCode1UseYN = 1;
		((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS1))->EnableWindow( FALSE );
	}

	ids = GetCodeManageValue( _T("92"), sCodeYN, sClassAlias );
	if ( ids < 0 ) return -11;
	else if ( _T("Y")==sCodeYN )
	{
		m_nCode2UseYN = 0;
		m_sCodeAlias2 = sClassAlias;
	}
	else if ( _T("N")==sCodeYN )
	{
		m_nCode2UseYN = 1;
		((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS2))->EnableWindow( FALSE );
	}

	ids = GetCodeManageValue( _T("93"), sCodeYN, sClassAlias );
	if ( ids < 0 ) return -12;
	else if ( _T("Y")==sCodeYN )
	{
		m_nCode3UseYN = 0;
		m_sCodeAlias3 = sClassAlias;
	}
	else if ( _T("N")==sCodeYN )
	{
		m_nCode3UseYN = 1;
		((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS3))->EnableWindow( FALSE );
	}

	////////////////////////////////////////////////////
	// 대출자에 대한 사용자정의 코드 사용여부 가져오기
	////////////////////////////////////////////////////
	ids = GetCodeManageValue( _T("94"), sCodeYN, sClassAlias );
	if ( ids < 0 ) return -13;
	else if ( _T("Y")==sCodeYN )
	{
		m_nCode4UseYN = 0;
		m_sCodeAlias4 = sClassAlias;
	}
	else if ( _T("N")==sCodeYN )
	{
		m_nCode4UseYN = 1;
		((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS4))->EnableWindow( FALSE );
	}

	ids = GetCodeManageValue( _T("95"), sCodeYN, sClassAlias );
	if ( ids < 0 ) return -14;
	else if ( _T("Y")==sCodeYN )
	{
		m_nCode5UseYN = 0;
		m_sCodeAlias5 = sClassAlias;
	}
	else if ( _T("N")==sCodeYN )
	{
		m_nCode5UseYN = 1;
		((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS5))->EnableWindow( FALSE );
	}

	ids = GetCodeManageValue( _T("96"), sCodeYN, sClassAlias );
	if ( ids < 0 ) return -15;
	else if ( _T("Y")==sCodeYN )
	{
		m_nCode6UseYN = 0;
		m_sCodeAlias6 = sClassAlias;
	}
	else if ( _T("N")==sCodeYN )
	{
		m_nCode6UseYN = 1;
		((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS6))->EnableWindow( FALSE );
	}

	////////////////////////////////////////////////////
	// 연속에 대한 사용자정의 코드 사용여부 가져오기
	////////////////////////////////////////////////////
	ids = GetCodeManageValue( _T("103"), sCodeYN, sClassAlias );
	if ( ids < 0 ) return -16;
	else if ( _T("Y")==sCodeYN )
	{
		m_nCode7UseYN = 0;
		m_sCodeAlias7 = sClassAlias;
	}
	else if ( _T("N")==sCodeYN )
	{
		m_nCode7UseYN = 1;
		((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS7))->EnableWindow( FALSE );
	}

	ids = GetCodeManageValue( _T("101"), sCodeYN, sClassAlias );
	if ( ids < 0 ) return -17;
	else if ( _T("Y")==sCodeYN )
	{
		m_nCode8UseYN = 0;
		m_sCodeAlias8 = sClassAlias;
	}
	else if ( _T("N")==sCodeYN )
	{
		m_nCode8UseYN = 1;
		((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS8))->EnableWindow( FALSE );
	}

	ids = GetCodeManageValue( _T("102"), sCodeYN, sClassAlias );
	if ( ids < 0 ) return -18;
	else if ( _T("Y")==sCodeYN )
	{
		m_nCode9UseYN = 0;
		m_sCodeAlias9 = sClassAlias;
	}
	else if ( _T("N")==sCodeYN )
	{
		m_nCode9UseYN = 1;
		((CEdit*)GetDlgItem(IDC_edtCODE_ALIAS9))->EnableWindow( FALSE );
	}

	UpdateData( FALSE );

	return 0;
EFS_END
return -1;
}

INT CMNGCommonUserDefineCode::GetCodeManageValue(CString sClassCode, CString &sCodeYN, CString &sClassAlias)
{
EFS_BEGIN
	const INT SELECT_CNT = 2;
	INT ids = -1;
	CString sQuery = _T("");
	CString sGetData[2] = { _T(""), _T("") };

	CESL_DataMgr* pSelectDM = ( CESL_DataMgr* ) FindDM( _T("DM_공통환경관리") );
	if ( NULL==pSelectDM ) return -10;

	sQuery.Format( _T("SELECT USE_YN, CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = %s"), sClassCode );

	ids = pSelectDM->GetValuesQuery( sQuery, sGetData, SELECT_CNT );
	if ( ids < 0 )
	{
		sCodeYN = _T("N");
		sClassAlias = _T("");
		return 1001;
	}

	sCodeYN = sGetData[0];
	sClassAlias = sGetData[1];

	return 0;
EFS_END
return -1;
}
// code = 61 ~ 66 => 91~96
INT CMNGCommonUserDefineCode::SaveScreen()
{
EFS_BEGIN
	INT ids = -1;
	CString sSetYN = _T("");
	CString sSetAlias = _T("");

	UpdateData( TRUE );

	////////////////////////////////////////////////////
	// 도서에 대한 사용자정의 코드 사용여부 반영
	////////////////////////////////////////////////////
	if ( 0==m_nCode1UseYN )
	{
		sSetYN = _T("Y");
		ids = CheckLimitWord( m_sCodeAlias1.GetBuffer( 0 ) );
		if ( 0!=ids )	return (ProcessError( ids ));
		sSetAlias = m_sCodeAlias1;
	}
	else if ( 1==m_nCode1UseYN )
	{
		sSetYN = _T("N");
		sSetAlias = _T("");
	}
	ids = SetCodeManageValue( _T("91"), sSetYN, sSetAlias );
	if ( ids < 0 ) return -10;

	if ( 0==m_nCode2UseYN )
	{
		sSetYN = _T("Y");
		ids = CheckLimitWord( m_sCodeAlias2.GetBuffer( 0 ) );
		if ( 0!=ids )	return (ProcessError( ids ));
		sSetAlias = m_sCodeAlias2;
	}
	else if ( 1==m_nCode2UseYN )
	{
		sSetYN = _T("N");
		sSetAlias = _T("");
	}
	ids = SetCodeManageValue( _T("92"), sSetYN, sSetAlias );
	if ( ids < 0 ) return -11;

	if ( 0==m_nCode3UseYN )
	{
		sSetYN = _T("Y");
		ids = CheckLimitWord( m_sCodeAlias3.GetBuffer( 0 ) );
		if ( 0!=ids )	return (ProcessError( ids ));
		sSetAlias = m_sCodeAlias3;
	}
	else if ( 1==m_nCode3UseYN )
	{
		sSetYN = _T("N");
		sSetAlias = _T("");
	}
	ids = SetCodeManageValue( _T("93"), sSetYN, sSetAlias );
	if ( ids < 0 ) return -12;

	////////////////////////////////////////////////////
	// 대출자에 대한 사용자정의 코드 사용여부 반영
	////////////////////////////////////////////////////
	if ( 0==m_nCode4UseYN )
	{
		sSetYN = _T("Y");
		ids = CheckLimitWord( m_sCodeAlias4.GetBuffer( 0 ) );
		if ( 0!=ids )	return (ProcessError( ids ));
		sSetAlias = m_sCodeAlias4;
	}
	else if ( 1==m_nCode4UseYN )
	{
		sSetYN = _T("N");
		sSetAlias = _T("");
	}
	ids = SetCodeManageValue( _T("94"), sSetYN, sSetAlias );
	if ( ids < 0 ) return -13;

	if ( 0==m_nCode5UseYN )
	{
		sSetYN = _T("Y");
		ids = CheckLimitWord( m_sCodeAlias5.GetBuffer( 0 ) );
		if ( 0!=ids )	return (ProcessError( ids ));
		sSetAlias = m_sCodeAlias5;
	}
	else if ( 1==m_nCode5UseYN )
	{
		sSetYN = _T("N");
		sSetAlias = _T("");
	}
	ids = SetCodeManageValue( _T("95"), sSetYN, sSetAlias );
	if ( ids < 0 ) return -14;

	if ( 0==m_nCode6UseYN )
	{
		sSetYN = _T("Y");
		ids = CheckLimitWord( m_sCodeAlias6.GetBuffer( 0 ) );
		if ( 0!=ids )	return (ProcessError( ids ));
		sSetAlias = m_sCodeAlias6;
	}
	else if ( 1==m_nCode6UseYN )
	{
		sSetYN = _T("N");
		sSetAlias = _T("");
	}
	ids = SetCodeManageValue( _T("96"), sSetYN, sSetAlias );
	if ( ids < 0 ) return -15;

	////////////////////////////////////////////////////
	// 도서에 대한 사용자정의 코드 사용여부 반영
	////////////////////////////////////////////////////
	if ( 0==m_nCode7UseYN )
	{
		sSetYN = _T("Y");
		ids = CheckLimitWord( m_sCodeAlias7.GetBuffer( 0 ) );
		if ( 0!=ids )	return (ProcessError( ids ));
		sSetAlias = m_sCodeAlias7;
	}
	else if ( 1==m_nCode7UseYN )
	{
		sSetYN = _T("N");
		sSetAlias = _T("");
	}
	ids = SetCodeManageValue( _T("103"), sSetYN, sSetAlias );
	if ( ids < 0 ) return -16;

	if ( 0==m_nCode8UseYN )
	{
		sSetYN = _T("Y");
		ids = CheckLimitWord( m_sCodeAlias8.GetBuffer( 0 ) );
		if ( 0!=ids )	return (ProcessError( ids ));
		sSetAlias = m_sCodeAlias8;
	}
	else if ( 1==m_nCode8UseYN )
	{
		sSetYN = _T("N");
		sSetAlias = _T("");
	}
	ids = SetCodeManageValue( _T("101"), sSetYN, sSetAlias );
	if ( ids < 0 ) return -17;

	if ( 0==m_nCode9UseYN )
	{
		sSetYN = _T("Y");
		ids = CheckLimitWord( m_sCodeAlias9.GetBuffer( 0 ) );
		if ( 0!=ids )	return (ProcessError( ids ));
		sSetAlias = m_sCodeAlias9;
	}
	else if ( 1==m_nCode9UseYN )
	{
		sSetYN = _T("N");
		sSetAlias = _T("");
	}
	ids = SetCodeManageValue( _T("102"), sSetYN, sSetAlias );
	if ( ids < 0 ) return -18;

	ESLAfxMessageBox( _T("코드설정값을 변경하였습니다.") );

	return 0;
EFS_END
return -1;
}

INT CMNGCommonUserDefineCode::SetCodeManageValue(CString sClassCode, CString sUseYN, CString sUserDefineAlias)
{
EFS_BEGIN
	BOOL bUpdate = TRUE;
	INT ids = -1;
	CString sTableName = _T("");
	CString sRecKey = _T("");
	CString sPreDefineAlias = _T(""); 

	CESL_DataMgr* pUIDM = ( CESL_DataMgr* ) FindDM( _T("DM_공통환경관리") );
	if ( NULL==pUIDM ) return -10;

	ids = pUIDM->StartFrame( );
	if ( ids < 0 ) return -11;

	pUIDM->InitDBFieldData( );
	sTableName = _T("MN_USER_DEFINE_TBL");
	bUpdate = MakeRecKey( pUIDM, sTableName, sClassCode, sRecKey );

	ids = pUIDM->AddDBFieldData( _T("USE_YN"), _T("STRING"), sUseYN );
	if ( ids < 0 ) return -12;
	ids = pUIDM->AddDBFieldData( _T("CLASS_CODE"), _T("NUMERIC"), sClassCode );
	if ( ids < 0 ) return -14;
	ids = pUIDM->AddDBFieldData( _T("CLASS_ALIAS"), _T("STRING"), sUserDefineAlias );
	if ( ids < 0 ) return -15;
	if ( bUpdate )
	{
		ids = pUIDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("사용자정의코드설정"), __WFILE__, __LINE__ ) );
		if ( ids < 0 ) return -16;
		ids = pUIDM->MakeUpdateFrame( sTableName, _T("REC_KEY"), _T("NUMERIC"), sRecKey );
		if ( ids < 0 ) return -17;
	}
	else
	{
		ids = pUIDM->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC"), sRecKey );
		if ( ids < 0 ) return -16;
		ids = pUIDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(_T("사용자정의코드설정"), __WFILE__, __LINE__ ) );
		if ( ids < 0 ) return -17;
		ids = pUIDM->MakeInsertFrame( sTableName );
		if ( ids < 0 ) return -18;
	}

	pUIDM->InitDBFieldData( );
	sTableName = _T("CD_CLASS_CODE_TBL");
	bUpdate = MakeRecKey( pUIDM, sTableName, sClassCode, sRecKey );

	GetPreDefineAlias( sClassCode, sPreDefineAlias );

	ids = pUIDM->AddDBFieldData( _T("USE_YN"), _T("STRING"), sUseYN );
	if ( ids < 0 ) return -12;

	if ( bUpdate )
	{
		ids = pUIDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("사용자정의코드설정"), __WFILE__, __LINE__ ) );
		if ( ids < 0 ) return -16;
		ids = pUIDM->MakeUpdateFrame( sTableName, _T("REC_KEY"), _T("NUMERIC"), sRecKey );
		if ( ids < 0 ) return -17;
	}
	else if ( !bUpdate && _T("Y")==sUseYN )
	{
		ids = pUIDM->AddDBFieldData( _T("CLASS_CODE"), _T("NUMERIC"), sClassCode );
		if ( ids < 0 ) return -13;
		ids = pUIDM->AddDBFieldData( _T("CLASS_NAME"), _T("STRING"), sPreDefineAlias );
		if ( ids < 0 ) return -14;
		ids = pUIDM->AddDBFieldData( _T("FIX_YN"), _T("STRING"), _T("N") );
		if ( ids < 0 ) return -15;
		ids = pUIDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(_T("사용자정의코드설정"), __WFILE__, __LINE__ ) );
		if ( ids < 0 ) return -18;
		ids = pUIDM->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC"), sRecKey );
		if ( ids < 0 ) return -19;
		ids = pUIDM->MakeInsertFrame( sTableName );
		if ( ids < 0 ) return -20;
	}

	ids = pUIDM->SendFrame( );
	if ( ids < 0 ) return -21;
	ids = pUIDM->EndFrame( );
	if ( ids < 0 ) return -22;

	return 0;
EFS_END
return -1;
}

BOOL CMNGCommonUserDefineCode::MakeRecKey(CESL_DataMgr *pSelectDM, CString sTableName, CString sClassCode, CString &sRecKey)
{
EFS_BEGIN
	BOOL bReturn = FALSE;
	INT ids = 0;
	CString sQuery = _T("");

	////////////////////////////////////////////////
	// 1. 이전에 코드값이 사용되었었는지 조사한다.
	sQuery.Format( _T("SELECT REC_KEY FROM %s WHERE CLASS_CODE = %s"), sTableName, sClassCode );
	ids = pSelectDM->GetOneValueQuery( sQuery, sRecKey );
	if ( (ids < 0) || (sRecKey.IsEmpty( )) ) 
	{
		ids = pSelectDM->MakeRecKey( sRecKey );
		if ( ids < 0 ) sRecKey = _T("");
	}
	else if ( !sRecKey.IsEmpty( ) )
		bReturn = TRUE;

	return bReturn;
EFS_END
return FALSE;
}

INT CMNGCommonUserDefineCode::CheckLimitWord(TCHAR* szClassAlias)
{
EFS_BEGIN
	const INT MAX_CHAR_LEN = 10;

	INT nStrLength = 0;
	INT nCharCnt = 0;
	TCHAR chComp = '\0';
	TCHAR chExtra = '\0';

	nStrLength = _tcsclen( szClassAlias );
	if ( 0==nStrLength )	return 101;
	if ( nStrLength <= (MAX_CHAR_LEN/2) ) return 0;

	for ( INT i = 0 ; i < nStrLength ; i++ )
	{
		chComp = szClassAlias[i];
		chExtra = szClassAlias[ (i+1) ];

		if ( chComp&0x80 )
		{
			i++;
			nCharCnt += 2;
		}
		else
			nCharCnt++;

		if ( nCharCnt > MAX_CHAR_LEN )	return 102;
	}

	return 0;
EFS_END
return -1;
}

INT CMNGCommonUserDefineCode::ProcessError(INT nError)
{
	if ( 101==nError )
		AfxMessageBox( _T("설명값을 입력하지 않으셨습니다.") );
	else if ( 102==nError )
		AfxMessageBox( _T("한글 5자(10Byte) 이하로 이름을 정의하셔야 합니다.") );

	return nError;
}

VOID CMNGCommonUserDefineCode::GetPreDefineAlias(CString sClassCode, CString &sAlias)
{
		 if ( _T("91")==sClassCode )	sAlias = _T("도서분류코드1");
	else if ( _T("92")==sClassCode )	sAlias = _T("도서분류코드2");
	else if ( _T("93")==sClassCode )	sAlias = _T("도서분류코드3");
	else if ( _T("94")==sClassCode )	sAlias = _T("대출자분류코드1");
	else if ( _T("95")==sClassCode )	sAlias = _T("대출자분류코드2");
	else if ( _T("96")==sClassCode )	sAlias = _T("대출자분류코드3");
	else if ( _T("103")==sClassCode )	sAlias = _T("연속분류코드1");
	else if ( _T("101")==sClassCode )	sAlias = _T("연속분류코드2");
	else if ( _T("102")==sClassCode )	sAlias = _T("연속분류코드3");
}

HBRUSH CMNGCommonUserDefineCode::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

