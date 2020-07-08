// SMS_DLG_03_SEARCH.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_DLG_03_SEARCH.h"
#include "SMS_DLG_03.h"
// 2011.09.08 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_DLG_03_SEARCH dialog


CSMS_DLG_03_SEARCH::CSMS_DLG_03_SEARCH(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr( IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_DLG_03_SEARCH)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = pParent;
}

CSMS_DLG_03_SEARCH::~CSMS_DLG_03_SEARCH()
{
}

BOOL CSMS_DLG_03_SEARCH::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CSMS_DLG_03_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_DLG_03_SEARCH)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_DLG_03_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CSMS_DLG_03_SEARCH)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnSMSDlg03Search, OnbtnUSERSEARCH)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_DLG_03_SEARCH message handlers

BOOL CSMS_DLG_03_SEARCH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("SMS_DLG_03_SEARCH") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : SMS_DLG_03_SEARCH") );
		return false;
	}
	
	InitControl();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSMS_DLG_03_SEARCH::GetCondition(CString condition[])
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cINFERIOR );
	CEdit *pEdt = NULL;
	condition[ 0 ] = TakeControlData( _T("소속") );
	condition[ 1 ] = TakeControlData( _T("직급") );

	INT		nCurSel	=	pCombo->GetCurSel();
	if( nCurSel )
	{
		condition[ 2 ].Format( _T("%d"), nCurSel - 1 );
	}

	condition[ 3 ] = TakeControlData( _T("자료실") );
	
	pEdt = ( CEdit * )GetDlgItem( IDC_edtSMS03LOAN_NUM_FROM );
	pEdt->GetWindowText( condition[ 4 ] );
	pEdt = ( CEdit * )GetDlgItem( IDC_edtSMS03_LOAN_NUM_TO );
	pEdt->GetWindowText( condition[ 5 ] );	
	pEdt = ( CEdit * )GetDlgItem( IDC_edtSMS03_USER_NAME_FROM );
	pEdt->GetWindowText( condition[ 6 ] );
	pEdt = ( CEdit * )GetDlgItem( IDC_edtSMS03_USER_NAME_TO );
	pEdt->GetWindowText( condition[ 7 ] );
	
	GetControlData( _T("CM_SMS_DLG_03_SEARCH") , _T("대출자생일1"), condition[ 8 ] );
	GetControlData( _T("CM_SMS_DLG_03_SEARCH") , _T("대출자생일2") , condition[ 9 ] );
	
	condition[ 10 ] = TakeControlData( _T("그룹") );
}

CString CSMS_DLG_03_SEARCH::TakeControlData( CString alias, INT option )
{	
	CString result;

	INT ids = 0;
	ids = GetControlData( _T("CM_SMS_DLG_03_SEARCH"), alias, result, -1, option );
	if( ids < 0 )
	{
		MessageBox( _T("Error : GetControlData() of CM_SMS_DLG_03_SEARCH"), _T("K2Up") );
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
			MessageBox( _T("Error , Wrong Option : TakeControlData of CM_SMS_DLG_03_SEARCH"), _T("K2Up") );
			return _T("");
			break;
	}

	return result;
}

CString CSMS_DLG_03_SEARCH::MakeWhere( CString condition[] )
{
	CString where, tmp;

	where.Format( _T(" (U.USER_CLASS!='3' OR U.MEMBER_CLASS!='1') AND") );

	const INT cnt = 4;
	CString fieldName[ cnt ] = {	_T("U.USER_POSITION_CODE"),	_T("U.USER_CLASS_CODE"), 
									_T("U.USER_CLASS"),			_T("U.SHELF_LOC_CODE") };
	for( INT i = 0; i < cnt; i++ )
	{
		if( condition[ i ].GetLength() )
		{
			tmp.Format( _T(" %s = '%s' AND"), fieldName[ i ], condition[ i ] );
			where += tmp;
		}
	}
	
	if( TRUE != condition[ 4 ].IsEmpty() && TRUE == condition[ 5 ].IsEmpty() )
	{
		tmp.Format( _T(" U.USER_NO = '%s' AND"), condition[ 4 ] );
		where += tmp;
	}
	else if( TRUE != condition[ 4 ].IsEmpty() && TRUE != condition[ 5 ].IsEmpty() )
	{
		tmp.Format( _T(" U.USER_NO BETWEEN '%s' AND '%s' AND"), condition[ 4 ], condition[ 5 ] );
		where += tmp;
	}
	
	if( TRUE != condition[ 6 ].IsEmpty() && TRUE == condition[ 7 ].IsEmpty() )
	{
		tmp.Format( _T(" U.IDX_USER_NAME LIKE '%s%%' AND"), condition[ 6 ] );
		where += tmp;
	}
	else if( TRUE != condition[ 6 ].IsEmpty() && TRUE != condition[ 7 ].IsEmpty() )
	{
		tmp.Format( _T(" U.IDX_USER_NAME BETWEEN '%s' AND '%s' AND"), condition[ 6 ], condition[ 7 ] );
		where += tmp;
	}

	if( TRUE != condition[ 8 ].IsEmpty() )
	{	

		if( TRUE != condition[ 9 ].IsEmpty() )
		{
			where += _T(" U.BIRTHDAY BETWEEN TO_DATE('") + condition[ 8 ] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + condition[ 9 ] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') AND");
		}
		else
		{
			where += _T(" U.BIRTHDAY BETWEEN TO_DATE('") + condition[ 8 ] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + condition[ 8 ] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') AND");
		}
	}
	
	if( TRUE != condition[10].IsEmpty() )
	{
		tmp.Format( 
			_T(" ( SELECT COUNT(*) ")
			_T("FROM MN_LOAN_USER_GROUP_LIST_TBL MLUGLT ")
			_T("WHERE MLUGLT.USER_KEY = U.REC_KEY AND ")
			_T("MLUGLT.GROUP_CODE='%s' ) > 0 AND") , condition[10] );

		where += tmp;
	}

	if( _T("AND") == where.Right( 3 ) )
	{
		return where.Left( where.GetLength() - 3 );
	}

	return where;
}



INT CSMS_DLG_03_SEARCH::InitControl()
{
EFS_BEGIN

	INT ids;

	CString sEcoMailUseYN;

	ids = GetManageValue( _T("기타")  , _T("공통") , _T("메일링시스템") , _T("사용여부") , sEcoMailUseYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeInitInfo") );

	if( sEcoMailUseYN.Compare(_T("Y")) != 0 )
	{
		GetDlgItem(IDC_staSD01_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cSD03S_GROUP)->ShowWindow(SW_HIDE);
		return 0;
	}


	CESL_DataMgr* pDM = FindDM(_T("DM_USER_GROUP_CODE_INFO"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitControl") );

	CESL_ControlMgr* pCM = FindCM(_T("CM_SMS_DLG_03_SEARCH"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitControl") );

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("그룹")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("InitControl") );

	CString sInsertCode[2];
	
	pCMCB->InitMultiComboBox(2);

	sInsertCode[0] = _T("   ");
	sInsertCode[1] = _T("적용안함");
	ids = pCMCB->SetData( sInsertCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InitControl") );

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("코드") , i , sInsertCode[0] );
		pDM->GetCellData( _T("이름") , i , sInsertCode[1] );

		sInsertCode[0].TrimLeft(); sInsertCode[0].TrimRight();
		if( sInsertCode[0].IsEmpty() ) continue;
			
		ids = pCMCB->SetData( sInsertCode , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InitControl") );
	}
	
	pCMCB->SetCurSel(0);

	return 0;

EFS_END
return -1;

}

HBRUSH CSMS_DLG_03_SEARCH::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CSMS_DLG_03_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	if ( WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam )
	{
		if(		pMsg->hwnd	==	GetDlgItem( IDCANCEL )->GetSafeHwnd() )
		{
			OnCancel();
			return TRUE;
		}

		if(		pMsg->hwnd	==	GetDlgItem( IDC_btnSMSDlg03Search )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS03LOAN_NUM_FROM )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS03_LOAN_NUM_TO )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS03_USER_NAME_FROM )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS03_USER_NAME_TO )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS03_BIRTHDAY_FROM )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS03_BIRTHDAY_TO )->GetSafeHwnd()
			)
		{
			OnbtnUSERSEARCH();
			return TRUE;
		}
		return FALSE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CSMS_DLG_03_SEARCH::OnbtnUSERSEARCH() 
{
	const INT cnt = 11;
	CString condition[ cnt ];
	GetCondition( condition );
	
	CSMS_DLG_03 *pParent = ( CSMS_DLG_03 * )m_pParent;
	pParent->SearchUser( MakeWhere( condition ) );	
}
