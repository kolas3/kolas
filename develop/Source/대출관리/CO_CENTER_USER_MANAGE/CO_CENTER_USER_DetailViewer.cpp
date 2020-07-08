// CO_CENTER_USER_DetailViewer.cpp : implementation file
//

#include "stdafx.h"
//#include _T("co_center_user_manage.h")
#include "CO_CENTER_USER_DetailViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_DetailViewer dialog


CCO_CENTER_USER_DetailViewer::CCO_CENTER_USER_DetailViewer( CESL_Mgr *pParent /*=NULL*/ )
	: CESL_Mgr( IDD, pParent )
{
	//{{AFX_DATA_INIT(CCO_CENTER_USER_DetailViewer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CCO_CENTER_USER_DetailViewer::~CCO_CENTER_USER_DetailViewer()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}
BOOL CCO_CENTER_USER_DetailViewer::Create( CWnd *pParentWnd )
{	
	return CDialog::Create( IDD, pParentWnd ); 
}	

VOID CCO_CENTER_USER_DetailViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCO_CENTER_USER_DetailViewer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCO_CENTER_USER_DetailViewer, CDialog)
	//{{AFX_MSG_MAP(CCO_CENTER_USER_DetailViewer)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bCENTER_USER_BEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bCENTER_USER_NEXT, OnbNEXT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCO_CENTER_USER_DetailViewer message handlers

VOID CCO_CENTER_USER_DetailViewer::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	if( bShow )
		Display( 0 );
}

VOID CCO_CENTER_USER_DetailViewer::SetRowIndex( CArray< INT, INT >&rowIndex )
{
	for( INT i = 0; i < rowIndex.GetSize(); i++ )
		m_rowIndex.Add( rowIndex.GetAt( i ) );
}

VOID CCO_CENTER_USER_DetailViewer::Display( INT flag )
{
	m_grid.Clear();
	m_index += flag;
	
	CESL_DataMgr* pDM = FindDM( _T("DM_CO_CENTER_USER_MANAGER_01_USER_LIST") );

	const INT cnt = 34;
	CString alias[ cnt ];
	GetAlias( alias );
	CString data[ cnt ];

	pDM->GetCellData( alias, cnt, data, m_rowIndex.GetAt( m_index ) );
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("자료실구분"), alias[ 31 ] );
	CString tmpTitle;
	for( INT i = 0; i < cnt; i++ )
		switch( i )
		{
			case  6 :
			case  8 :
			case 18 :
			case 31 :
			case 32 :
				continue;
				break;
			case  5 :
				m_grid.SetControlData( _T("사용자ID"), data[ i ] );
				break;
			case 10 :
				m_grid.SetControlData( _T("양력(+), 음력(-)"), data[ i ] );
				break;
			case 11 :
				m_grid.SetControlData( alias[ i ], ConvertCivilNum( data[ i ] ) );
				break;
			case 12 :
			case 13 :
			case 14 :
			case 15 :
			case 16 :
			case 17 :
				tmpTitle = alias[ i ];
				tmpTitle.Replace( _T("_"), _T(" ") );
				m_grid.SetControlData( tmpTitle, data[ i ] );
				break;
			case 29 :
				m_grid.SetControlData( alias[ i ], _T("H") == data[ i ] ? _T("자택") : _T("근무지") );
				break;
			default :
				m_grid.SetControlData( alias[ i ], data[ i ] );
				break;
		}		
	m_grid.Display();

	SetMoveBtnEanble();

	CString sParentCMAlias = _T("CM_CO_CENTER_USER_MANAGER_01");
	CString sControlAlias = _T("센터이용자정보그리드");	
	
	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( pCM == NULL ) return ;
	
	CESL_Grid *pGrid = ( CESL_Grid * )( pCM->FindControl( sControlAlias ) );
	if( pGrid == NULL ) 
		return ;

	pGrid->SetReverse( m_rowIndex.GetAt( m_index ) );	
}

BOOL CCO_CENTER_USER_DetailViewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	
	m_grid.InitViewer( IDC_gridCENTERUSER, this );
	m_grid.SetViewerWidth( 35 , 60 );
	
	m_index = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCO_CENTER_USER_DetailViewer::GetAlias( CString alias[] )
{
	alias[  0 ] = _T("생성도서관이름");
	alias[  1 ] = _T("생성일");
	alias[  2 ] = _T("최종수정도서관이름");
	alias[  3 ] = _T("최종수정일");
	alias[  4 ] = _T("대출자번호");
	alias[  5 ] = _T("대출자ID");
	alias[  6 ] = _T("비밀번호");
	alias[  7 ] = _T("이름");
	alias[  8 ] = _T("이름색인");
	alias[  9 ] = _T("생년월일");
	alias[ 10 ] = _T("양음력");
	alias[ 11 ] = _T("주민등록번호");
	alias[ 12 ] = _T("주소_집");
	alias[ 13 ] = _T("주소_근무지");
	alias[ 14 ] = _T("우편번호_집");
	alias[ 15 ] = _T("우편번호_근무지");
	alias[ 16 ] = _T("전화번호_집");
	alias[ 17 ] = _T("전화번호_근무지");
	alias[ 18 ] = _T("내선번호");
	alias[ 19 ] = _T("근무지부서명");
	alias[ 20 ] = _T("근무지직급명");
	alias[ 21 ] = _T("핸드폰");
	alias[ 22 ] = _T("E-MAIL");
	alias[ 23 ] = _T("등록일");
	alias[ 24 ] = _T("제적일");
	alias[ 25 ] = _T("학교");
	alias[ 26 ] = _T("학년");
	alias[ 27 ] = _T("반");
	alias[ 28 ] = _T("교번");
	alias[ 29 ] = _T("우편발송지역");
	alias[ 30 ] = _T("비고");
	alias[ 31 ] = _T("자료실코드");
	alias[ 32 ] = _T("이용자사진이름");
	alias[ 33 ] = _T("회원증분실여부");
}

VOID CCO_CENTER_USER_DetailViewer::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here
	
	Display( -1 );
}

VOID CCO_CENTER_USER_DetailViewer::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
	Display( 1 );
}

VOID CCO_CENTER_USER_DetailViewer::SetMoveBtnEanble()
{
	CButton *pBtn = NULL;
	const INT cnt = 2;
	bool flag[ cnt ] = { 
		true, true };
	UINT id[ cnt ] = {
		IDC_bCENTER_USER_BEFORE, IDC_bCENTER_USER_NEXT };

	if( !m_index )
		flag[ 0 ] = false;

	if( m_index == m_rowIndex.GetSize() - 1 )
		flag[ 1 ] = false;

	for( INT i = 0; i < cnt; i++ )
	{
		pBtn = ( CButton * )GetDlgItem( id[ i ] );
		pBtn->EnableWindow( flag[ i ] );
	}
}

CString CCO_CENTER_USER_DetailViewer::ConvertCivilNum( CString civilNum )
{
	// 2009.10.08 UPDATE BY KSJ : 주민번호 표시 방식 변경
//DEL 	civilNum.Replace( _T("-"), _T("") );
//DEL 	if( 13 != civilNum.GetLength() )
//DEL 		return civilNum;
//DEL 
//DEL 	CString frontNum = civilNum.Left( 6 );
//DEL 	civilNum.Format( _T("%s-*******"), frontNum );
	CLocCommonAPI::ChangeSecuriyCivilNo( this , civilNum );
	
	return civilNum;
}

HBRUSH CCO_CENTER_USER_DetailViewer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// 여기까지 추가

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
