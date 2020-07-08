// SMS_DLG_03_BOOK_SEARCH.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_DLG_03_BOOK_SEARCH.h"
#include "..\..\공통\K2UP_Common\MakeSearchData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_DLG_03_BOOK_SEARCH dialog


CSMS_DLG_03_BOOK_SEARCH::CSMS_DLG_03_BOOK_SEARCH(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr( IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_DLG_03_BOOK_SEARCH)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = pParent;
}
CSMS_DLG_03_BOOK_SEARCH::~CSMS_DLG_03_BOOK_SEARCH()
{
}

BOOL CSMS_DLG_03_BOOK_SEARCH::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CSMS_DLG_03_BOOK_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_DLG_03_BOOK_SEARCH)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_DLG_03_BOOK_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CSMS_DLG_03_BOOK_SEARCH)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnSMSDlg03BookSearch, OnbtnBOOKSEARCH)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_DLG_03_BOOK_SEARCH message handlers

BOOL CSMS_DLG_03_BOOK_SEARCH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("SMS_DLG_03_BOOK_SEARCH") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : SMS_DLG_03_BOOK_SEARCH") );
		return false;
	}
	
	CDateTimeCtrl *pDateTimeCtrl = NULL;
	SYSTEMTIME systime;	
	const INT cnt = 2;
	UINT id[ cnt ] = {
		IDC_dRETURN_PLAN_DATE_FROM, IDC_dRETURN_PLAN_DATE_TO };
	for( INT i = 0; i < cnt; i++ )
	{
		pDateTimeCtrl = ( CDateTimeCtrl * )GetDlgItem( id[ i ] );
		pDateTimeCtrl->GetTime( &systime );
		DateTime_SetSystemtime( pDateTimeCtrl->m_hWnd ,GDT_VALID , &systime );
	}		

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSMS_DLG_03_BOOK_SEARCH::GetCondition( CString condition[] )
{
	condition[ 0 ] = TakeControlData( _T("관리구분") );
	condition[ 1 ] = TakeControlData( _T("매체구분") );

	CEdit *pEdt = NULL;
	pEdt = ( CEdit * )GetDlgItem( IDC_edtSMS03_TITLE );
	pEdt->GetWindowText( condition[ 2 ] );
	pEdt = ( CEdit * )GetDlgItem( IDC_edtSMS03_AUTHOR );
	pEdt->GetWindowText( condition[ 3 ] );

	condition[ 4 ] = GetCtrlDate( IDC_dRETURN_PLAN_DATE_FROM );
	condition[ 5 ] = GetCtrlDate( IDC_dRETURN_PLAN_DATE_TO );

	condition[ 6 ] = TakeControlData( _T("자료실") );
}

CString CSMS_DLG_03_BOOK_SEARCH::TakeControlData(CString alias, INT option)
{	
	CString result;

	INT ids = 0;
	ids = GetControlData( _T("CM_SMS_DLG_03_BOOK_SEARCH"), alias, result, -1, option );
	if( ids < 0 )
	{
		MessageBox( _T("Error : GetControlData() of CM_SMS_DLG_03_BOOK_SEARCH"), _T("K2Up") );
		return _T("");
	}
	
	switch( option )
	{
		case 0 :
			if( _T("0") == result )
				return _T("");
			break;
		case 1 :
			if( _T("적용안함") == result )
				return _T("");
			break;
		default :
			MessageBox( _T("Error , Wrong Option : TakeControlData of CM_SMS_DLG_03_BOOK_SEARCH"), _T("K2Up") );
			return _T("");
			break;
	}

	return result;
}

CString CSMS_DLG_03_BOOK_SEARCH::GetCtrlDate( UINT id )
{
	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )GetDlgItem( id );
	CTime time;
	if( GDT_VALID != pDate->GetTime( time ) )
		return _T("");
	
	CString date;
	date.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
	return date;
}

VOID CSMS_DLG_03_BOOK_SEARCH::GetBOBookFieldName( CString fieldName[] )
{
	fieldName[ 0 ] = _T("B.MANAGE_CODE");
	fieldName[ 1 ] = _T("B.MEDIA_CODE");
	fieldName[ 2 ] = _T("I.IDX_TITLE");
	fieldName[ 3 ] = _T("B.AUTHOR");
	fieldName[ 4 ] = _T("B.SHELF_DATE");

	// 2009.03.05 ADD BY KSJ : 검색 조건에 자료의 자료실(SHELF_LOC_CODE) 추가
	fieldName[ 5 ] = _T("B.SHELF_LOC_CODE");
}

VOID CSMS_DLG_03_BOOK_SEARCH::GetSEBookFieldName( CString fieldName[] )
{
	fieldName[ 0 ] = _T("B.MANAGE_CODE");
	fieldName[ 1 ] = _T("I.MEDIA_CODE");
	fieldName[ 2 ] = _T("I.IDX_TITLE");
	fieldName[ 3 ] = _T("I.AUTHOR");
	fieldName[ 4 ] = _T("B.SHELF_DATE");
	fieldName[ 5 ] = _T("B.SHELF_LOC_CODE");
}

CString CSMS_DLG_03_BOOK_SEARCH::MakeWhere( CString condition[], INT opt )
{
	CString fieldName[ 6 ];
	switch( opt )
	{
	case 1 :
		GetBOBookFieldName( fieldName );
		break;
	case 2 :
		GetSEBookFieldName( fieldName );
		break;
	default :
		break;
	}

	CString where, tmp;

	if( FALSE == condition[ 0 ].IsEmpty() )
	{
		tmp.Format( _T(" %s = '%s' AND"), fieldName[ 0 ], condition[ 0 ] );
		where += tmp;
	}

	if( FALSE == condition[ 1 ].IsEmpty() )
	{
		tmp.Format( _T(" %s = '%s' AND"), fieldName[ 1 ], condition[ 1 ] );
		where += tmp;
	}

	FILE *fp = _tfopen( _T("..\\CFG\\_newecosearch"), _T("rb") );

	if( FALSE == condition[ 2 ].IsEmpty() )
	{
		CMakeSearchData MakeSearchData(m_pParent);
		MakeSearchData.GetKeyWord(condition[ 2 ]);

		if( 1 == m_pInfo->GetSearchIndexType()  )
		{
			if( fp )
			{
				tmp.Format( _T(" I.REC_KEY IN( &ECOSEARCH( , %s, '%s' ) ) AND"), ( 1 == opt ? _T("BO_IDX_TITLE") : _T("SE_IDX_TITLE") ), condition[ 2 ] );
			}
			else
			{
				tmp.Format( _T(" &ECOSEARCH( I.REC_KEY, %s, '%s' ) AND"), ( 1 == opt ? _T("BO_IDX_TITLE") : _T("SE_IDX_TITLE") ), condition[ 2 ] );
			}
		}
		else
		{
			tmp.Format( _T(" CATSEARCH( %s, '%s*', NULL ) > 0 AND"), fieldName[ 2 ], condition[ 2 ] );
		}

		where += tmp;
	}
		
	if( FALSE == condition[ 3 ].IsEmpty() )
	{
		tmp.Format( _T(" %s LIKE '%%%s%%' AND"), fieldName[ 3 ], condition[ 3 ] );
		where += tmp;
	}
	if( FALSE == condition[ 6 ].IsEmpty() )
	{
		tmp.Format( _T(" %s = '%s' AND"), fieldName[ 5 ], condition[ 6 ] );
		where += tmp;
	}

	if( TRUE != condition[ 4 ].IsEmpty() && TRUE != condition[ 5 ].IsEmpty() )
	{
		tmp.Format( _T(" %s BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS')"), fieldName[ 4 ], condition[ 4 ], condition[ 5 ] );
	}
	else if( TRUE != condition[ 4 ].IsEmpty() )
	{
		tmp.Format( _T(" %s = '%s'"), fieldName[ 4 ], condition[ 4 ] );
	}
	else if( TRUE != condition[ 5 ].IsEmpty() )
	{
		AfxMessageBox( _T("~까지 검색은 적용되지 않습니다.") );
		tmp.Empty();
	}
	where += tmp;	
	
	if( _T("AND") == where.Right( 3 ) )
	{
		tmp = where.Left( where.GetLength() - 3 );
		where = tmp;
	}	
	
	return where;
}

HBRUSH CSMS_DLG_03_BOOK_SEARCH::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CSMS_DLG_03_BOOK_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	if ( WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam )
	{
		if(		pMsg->hwnd	==	GetDlgItem( IDCANCEL )->GetSafeHwnd() )
		{
			OnCancel();
			return TRUE;
		}

		if(		pMsg->hwnd	==	GetDlgItem( IDC_btnSMSDlg03BookSearch )->GetSafeHwnd() 
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS03_TITLE )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS03_AUTHOR )->GetSafeHwnd()		)
		{
			OnbtnBOOKSEARCH();
			return TRUE;
		}
		return FALSE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CSMS_DLG_03_BOOK_SEARCH::OnbtnBOOKSEARCH() 
{
	const INT cnt = 7;
	CString condition[ cnt ];
	GetCondition( condition );

	CSMS_DLG_03 *pParent = ( CSMS_DLG_03 * )m_pParent;
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbData );
	switch( pCombo->GetCurSel() )
	{
		case 0 :
			pParent->SearchBOBook( MakeWhere( condition, 1 ) );
			pParent->SearchSEBook( MakeWhere( condition, 2 ) );
			break;
		case 1 :
			pParent->SearchBOBook( MakeWhere( condition, 1 ) );
			break;
		case 2 :
			pParent->SearchSEBook( MakeWhere( condition, 2 ) );
			break;
		default :
			break;
	}		
}
