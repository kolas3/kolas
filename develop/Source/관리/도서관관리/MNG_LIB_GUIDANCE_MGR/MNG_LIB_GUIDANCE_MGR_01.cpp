// MNG_LIB_GUIDANCE_MGR_01.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_LIB_GUIDANCE_MGR_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_GUIDANCE_MGR_01 dialog


CMNG_LIB_GUIDANCE_MGR_01::CMNG_LIB_GUIDANCE_MGR_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_LIB_GUIDANCE_MGR_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	INT m_type = 0;
}
CMNG_LIB_GUIDANCE_MGR_01::~CMNG_LIB_GUIDANCE_MGR_01()
{
	
}

BOOL CMNG_LIB_GUIDANCE_MGR_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CMNG_LIB_GUIDANCE_MGR_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_LIB_GUIDANCE_MGR_01)
	DDX_Control(pDX, IDC_RICHEDIT, m_richEdt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_LIB_GUIDANCE_MGR_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_LIB_GUIDANCE_MGR_01)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_rNOTICE, OnrNOTICE)
	ON_BN_CLICKED(IDC_rUSE, OnrUSE)
	ON_WM_CTLCOLOR()            
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIB_GUIDANCE_MGR_01 message handlers

VOID CMNG_LIB_GUIDANCE_MGR_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	ResizeCtrls( cx, cy );
}

VOID CMNG_LIB_GUIDANCE_MGR_01::ResizeCtrls( INT cx, INT cy )
{
	CStatic *pStatic;

	pStatic = ( CStatic * )GetDlgItem( IDC_sGROUP_1 );
	if( NULL == pStatic->GetSafeHwnd() )
		return;
	else
		pStatic->MoveWindow( 7, 3, cx- 15, 35 );
	
	pStatic = ( CStatic * )GetDlgItem( IDC_sGROUP_2 );
	if( NULL == pStatic->GetSafeHwnd() )
		return;
	else
		pStatic->MoveWindow( 7, 35, cx - 15, cy - 45 );
	
	if( NULL == m_richEdt.GetSafeHwnd() )
		return;
	else
		m_richEdt.MoveWindow( 14, 70, cx - 28, cy - 85 );
}

BOOL CMNG_LIB_GUIDANCE_MGR_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_LIB_GUIDANCE_MGR_01") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_LIB_GUIDANCE_MGR_01") );
		return false;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	 
}

// DM_MNG_LIB_GUIDANCE_MGR_01

VOID CMNG_LIB_GUIDANCE_MGR_01::OnrNOTICE() 
{
	// TODO: Add your control notification handler code here
	
	ShowData( 1 );
}

VOID CMNG_LIB_GUIDANCE_MGR_01::OnrUSE() 
{
	// TODO: Add your control notification handler code here
	
	ShowData( 2 );
}

VOID CMNG_LIB_GUIDANCE_MGR_01::ShowData( INT type )
{
	ShowStaticBar( type );
	m_type = type;
	
	CString date;
	m_richEdt.SetWindowText( FindData( type, date ) );	

	CStatic *pStatic = ( CStatic * )GetDlgItem( IDC_sUPDATE_DATE );
	CString output;
	if( date.GetLength() )
	{
		output.Format( _T("수정일자 : %s"), date );
		pStatic->SetWindowText( output );
	}
}

VOID CMNG_LIB_GUIDANCE_MGR_01::ShowStaticBar( INT type )
{
	CString output[ 2 ] = { _T("공지사항"), _T("이용안내") };
	CStatic *pStatic = ( CStatic * )GetDlgItem( IDC_sTYPE );
	pStatic->SetWindowText( output[ type - 1 ] );
}

CString CMNG_LIB_GUIDANCE_MGR_01::FindData( INT type, CString &date )
{
	CString where;
	where.Format( _T("G_TYPE = %d"), type );

	CString alias[ 2 ] = { _T("UPDATE_DATE"), _T("GUIDANCE") };
	CString data[ 2 ];
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_GUIDANCE_MGR_01") );
	pDM->RefreshDataManager( where );

	if( pDM->GetRowCount() )
	{
		pDM->GetCellData( alias, 2, data, pDM->GetRowCount() - 1 );
		date = data[ 0 ];
		return data[ 1 ];
	}
	else
		switch( type )
		{
			case 1 :
				AfxMessageBox( _T("입력된 공지사항이 없습니다.") );
				break;
			case 2 :
				AfxMessageBox( _T("입력된 이용안내이 없습니다.") );
				break;
			default :
				break;
		}

	return _T("");
}

VOID CMNG_LIB_GUIDANCE_MGR_01::SaveGuidanceInfo()
{
	if( !m_type )
	{
		AfxMessageBox( _T("입력 구분을 선택하여 주십시오") );
		return;
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_LIB_GUIDANCE_MGR_01") );
	
	CString query;
	query.Format( _T("DELETE FROM MN_LIB_GUIDANCE_TBL WHERE G_TYPE = %d;"), m_type );
	pDM->ExecuteQuery( query, 1 );
	
	const INT cnt = 4;
	CString data[ cnt ];
	GetCtrlData( pDM, data );

	CString alias[ cnt ] = {
		_T("REC_KEY"), _T("G_TYPE"), _T("GUIDANCE"), _T("UPDATE_DATE") };
	CString type[ cnt ] = {
		_T("NUMERIC"), _T("NUMERIC"), _T("STRING"), _T("STRING") };

	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < cnt; i++ )
		if( data[ i ].GetLength() )
			pDM->AddDBFieldData( alias[ i ], type[ i ], data[ i ] );
	pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("도서관안내"), __WFILE__, __LINE__ ), TRUE );

	pDM->MakeInsertFrame( _T("MN_LIB_GUIDANCE_TBL") );
	pDM->SendFrame();
	pDM->EndFrame();

	AfxMessageBox( _T("저장되었습니다.") );
}

VOID CMNG_LIB_GUIDANCE_MGR_01::GetCtrlData( CESL_DataMgr *pDM, CString data[] )
{
	pDM->MakeRecKey( data[ 0 ] );
	data[ 1 ].Format( _T("%d"), m_type );
	m_richEdt.GetWindowText( data[ 2 ] );

	CTime time = pDM->GetDBTime();
	data[ 3 ].Format( _T("%04d 년 %02d 월 %02d일"), time.GetYear(), time.GetMonth(), time.GetDay() );
}



HBRUSH CMNG_LIB_GUIDANCE_MGR_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
