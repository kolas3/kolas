// SMS_DLG_02_SEARCH.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_DLG_02_SEARCH.h"
#include "SMS_DLG_02.h"
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSMS_DLG_02_SEARCH::CSMS_DLG_02_SEARCH(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr( IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_DLG_02_SEARCH)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = pParent;
}

CSMS_DLG_02_SEARCH::~CSMS_DLG_02_SEARCH()
{
}

BOOL CSMS_DLG_02_SEARCH::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CSMS_DLG_02_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_DLG_02_SEARCH)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_DLG_02_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CSMS_DLG_02_SEARCH)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED( IDC_btnSEARCH , OnbtnSearch)
	ON_BN_CLICKED( IDC_radPossessBook , OnbtnPossessBook)
	ON_BN_CLICKED( IDC_radProcessing , OnbtnProcessing)
	ON_BN_CLICKED( IDC_radCancel , OnbtnCancle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_DLG_02_SEARCH message handlers

BOOL CSMS_DLG_02_SEARCH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("SMS_DLG_02_SEARCH") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : SMS_DLG_02_SEARCH") );
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

	((CButton*)GetDlgItem(IDC_radPossessBook))->SetCheck(TRUE);
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CSMS_DLG_02_SEARCH::GetCtrlDate(UINT id)
{
	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )GetDlgItem( id );
	CTime time;
	if( GDT_VALID != pDate->GetTime( time ) )
		return _T("");
	
	CString date;
	date.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
	return date;
}


VOID CSMS_DLG_02_SEARCH::GetCondition( CString condition[] )
{
	const INT edtCnt = 5;
	UINT edtID[ edtCnt ] = { IDC_edtSMS02_USER_NAME_FROM,	IDC_edtSMS02_USER_NAME_TO, 
							 IDC_edtSMS02_USERNO_FROM,	IDC_edtSMS02_USERNO_TO,	IDC_edtSMS02_TITLE };
	
	CEdit *pEdt = NULL;
	for( INT i = 0; i < edtCnt; i++ )
	{
		pEdt = ( CEdit * )GetDlgItem( edtID[ i ] );
		pEdt->GetWindowText( condition[ i ] );
	}

	CButton *pRadCancle = NULL;
	pRadCancle = ( CButton * )GetDlgItem( IDC_radCancel );
	if( !pRadCancle->GetCheck() )
	{
		condition[ 5 ] = GetCtrlDate( IDC_dRETURN_PLAN_DATE_FROM );
		condition[ 6 ] = GetCtrlDate( IDC_dRETURN_PLAN_DATE_TO );
	}

	condition[ 7 ] = GetCtrlDate( IDC_dAPPLICANT_DATE_FROM );
	condition[ 8 ] = GetCtrlDate( IDC_dAPPLICANT_DATE_TO );
}

CString CSMS_DLG_02_SEARCH::MakeWhere( CString condition[] )
{
	const INT fieldCnt = 7;
	CString fieldName[ fieldCnt ];
	
	CString where, tmp;

	if( FALSE == condition[ 0 ].IsEmpty() && TRUE == condition[ 1 ].IsEmpty() )
	{
		tmp.Format( _T(" IDX_APPLICANT_NAME BETWEEN '%s' AND '%s힝' AND"), condition[ 0 ] , condition[ 0 ]);
		where += tmp;
	}
	else if( FALSE == condition[ 0 ].IsEmpty() && FALSE == condition[ 1 ].IsEmpty() )
	{
		tmp.Format( _T(" IDX_APPLICANT_NAME BETWEEN '%s' AND '%s' AND"), condition[ 0 ], condition[ 1 ] );
		where += tmp;
	}

	if( FALSE == condition[ 2 ].IsEmpty() && TRUE == condition[ 3 ].IsEmpty() )
	{
		tmp.Format( _T(" USER_NO = '%s' AND "), condition[ 2 ] );
		where += tmp;
	}
	else if( FALSE == condition[ 2 ].IsEmpty() && FALSE == condition[ 3 ].IsEmpty() )
	{
		tmp.Format( _T(" USER_NO BETWEEN '%s' AND '%s' AND "), condition[ 2 ], condition[ 3 ] );
		where += tmp;
	}

	if( FALSE == condition[ 4 ].IsEmpty() )
	{
		tmp.Format( _T(" INDEX_TITLE LIKE '%%%s%%' AND"), condition[ 4 ] );
		where += tmp;
	}

	CButton *pRadCancle = NULL;
	pRadCancle = ( CButton * )GetDlgItem( IDC_radCancel );
	if( !pRadCancle->GetCheck() )
	{
		if( FALSE == condition[ 5 ].IsEmpty() && FALSE == condition[ 6 ].IsEmpty() )
		{ 
			tmp.Format( _T(" FURNISH_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') AND"), condition[ 5 ], condition[ 6 ] );
			where += tmp;
		}
		else if( FALSE == condition[ 5 ].IsEmpty() )
		{
			tmp.Format( _T(" FURNISH_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') AND"), condition[ 5 ], condition[ 5 ] );
			where += tmp;
		}
		else if( FALSE == condition[ 6 ].IsEmpty() )
			AfxMessageBox( _T("비치일자는 ~까지 검색이 적용되지 않습니다.") );
	}

	if( FALSE == condition[ 7 ].IsEmpty() && FALSE == condition[ 8 ].IsEmpty() )
	{ 
		tmp.Format( _T(" APPLICANT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') AND"), condition[ 7 ], condition[ 8 ] );
		where += tmp;
	}
	else if( FALSE == condition[ 7 ].IsEmpty() )
	{
		tmp.Format( _T(" APPLICANT_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') AND"), condition[ 7 ], condition[ 7 ] );
		where += tmp;
	}
	else if( FALSE == condition[ 8 ].IsEmpty() )
	AfxMessageBox( _T("신청일자는 ~까지 검색이 적용되지 않습니다.") );
	
	INT ids = 0, nFurnishStatus;
	ids = GetSearchType();
	if( 1 == ids)
	{
		nFurnishStatus = 3;
	}	
	else if( 2 == ids)
	{
		nFurnishStatus = 2;
	}
	else if( 3 == ids)
	{
		nFurnishStatus = 4;
	}
	tmp.Format( _T(" FURNISH_STATUS = '%d'") , nFurnishStatus);	
	
	where += tmp;
	
	return where;
}

HBRUSH CSMS_DLG_02_SEARCH::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CSMS_DLG_02_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	if ( WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam )
	{
		if(		pMsg->hwnd	==	GetDlgItem(IDCANCEL)->GetSafeHwnd() )
		{
			OnCancel();
			return TRUE;
		}

		if(		pMsg->hwnd	==	GetDlgItem( IDC_btnSEARCH )->GetSafeHwnd() 
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS02_USER_NAME_FROM )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS02_USER_NAME_TO )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS02_USERNO_FROM )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS02_USERNO_TO )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS02_TITLE )->GetSafeHwnd()				)
		{
			OnbtnSearch();
			::SetFocus( pMsg->hwnd );
			return TRUE;
		}
		return FALSE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CSMS_DLG_02_SEARCH::OnbtnSearch() 
{
	const INT ctrlCnt = 9;
	CString condition[ ctrlCnt ];
	GetCondition( condition );

	CSMS_DLG_02 *pParent = ( CSMS_DLG_02 * )m_pParent;

	INT	nSearchType	= GetSearchType();
	switch( nSearchType )
	{
	case 1 :
		pParent->m_SMSType.Format( _T("SMS04") );
		break;
	case 2 :
		pParent->m_SMSType.Format( _T("SMS18") );
		break;
	case 3 :
		pParent->m_SMSType.Format( _T("SMS19") );
		break;
	default :
		AfxMessageBox( _T("문자 종류를 선택하여 주십시요") );
		return;
		break;
	}
	pParent->m_pSMSContentsEditor->SetSearchType( (nSearchType-1) );
	pParent->SearchData( MakeWhere( condition ) );
}

INT CSMS_DLG_02_SEARCH::GetSearchType()
{
EFS_BEGIN
		
	CButton *pBtn = NULL;

	pBtn = ( CButton * )GetDlgItem( IDC_radPossessBook );
	if( pBtn->GetCheck() )
	{
		return 1;
	}
	
	pBtn = ( CButton * )GetDlgItem( IDC_radProcessing );
	if( pBtn->GetCheck() )
	{
		return 2;
	}
	
	pBtn = ( CButton * )GetDlgItem( IDC_radCancel );
	if( pBtn->GetCheck() )
	{
		return 3;
	}	

	return 0;
	
EFS_END
	return -1;	
}

void CSMS_DLG_02_SEARCH::OnbtnPossessBook() 
{
	GetDlgItem(IDC_dRETURN_PLAN_DATE_FROM)->EnableWindow(TRUE);
	GetDlgItem(IDC_dRETURN_PLAN_DATE_TO)->EnableWindow(TRUE);

}

void CSMS_DLG_02_SEARCH::OnbtnProcessing() 
{
	GetDlgItem(IDC_dRETURN_PLAN_DATE_FROM)->EnableWindow(FALSE);
	GetDlgItem(IDC_dRETURN_PLAN_DATE_TO)->EnableWindow(FALSE);

	const INT nCnt = 2;
	INT nIDC[nCnt] =
	{
		IDC_dRETURN_PLAN_DATE_FROM , IDC_dRETURN_PLAN_DATE_TO
	};
	UnCheck(nIDC);
}

void CSMS_DLG_02_SEARCH::OnbtnCancle() 
{
	GetDlgItem(IDC_dRETURN_PLAN_DATE_FROM)->EnableWindow(FALSE);
	GetDlgItem(IDC_dRETURN_PLAN_DATE_TO)->EnableWindow(FALSE);

	INT nIDC[2] = 
	{
 		IDC_dRETURN_PLAN_DATE_FROM , IDC_dRETURN_PLAN_DATE_TO
	};

	UnCheck(nIDC);
}

void CSMS_DLG_02_SEARCH::UnCheck(INT nIDC[2]) 
{
	SYSTEMTIME systime;	
	CDateTimeCtrl* pTimeCtrl = NULL;
	
 	const INT nCnt = 2;
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[i]);
		pTimeCtrl->GetTime(&systime);
		DateTime_SetSystemtime(pTimeCtrl->m_hWnd , GDT_NONE , &systime );
	}
}
