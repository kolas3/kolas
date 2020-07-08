// SMS_DLG_01_SEARCH.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_DLG_01_SEARCH.h"
#include "SMS_DLG_01.h"
#include "imm.h"
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_DLG_01_SEARCH dialog


CSMS_DLG_01_SEARCH::CSMS_DLG_01_SEARCH(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr( IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_DLG_01_SEARCH)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = pParent;
}

CSMS_DLG_01_SEARCH::~CSMS_DLG_01_SEARCH()
{
}

BOOL CSMS_DLG_01_SEARCH::Create(CWnd* pParentWnd)
{	
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd); 

EFS_END
return FALSE;

}	

VOID CSMS_DLG_01_SEARCH::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_DLG_01_SEARCH)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CSMS_DLG_01_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CSMS_DLG_01_SEARCH)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnSMS01_SEARCH, OnbtnSEARCH)
	ON_EN_SETFOCUS(IDC_edtSMS01_USER_NAME_FROM, OnSetfocusedtSMS01USERNAMEFROM)
	ON_EN_SETFOCUS(IDC_edtSMS01_USER_NAME_TO, OnSetfocusedtSMS01USERNAMETO)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_DLG_01_SEARCH message handlers

BOOL CSMS_DLG_01_SEARCH::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	if( InitESL_Mgr( _T("SMS_DLG_01_SEARCH") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : SMS_DLG_01_SEARCH") );
		return false;
	}
	
	// TODO: Add extra initialization here
	
	EnableReturnPlanDateCtrl();

	CButton *pBtn = ( CButton * )GetDlgItem( IDC_RADIO1 );
	pBtn->SetCheck( true );

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

	InitControl();

	CString strValue;
	GetManageValue(_T("열람"),_T("공통"),_T("대출자료관리"),_T("야간대출예약기능유무"),strValue);
	strValue.TrimLeft();strValue.TrimRight();
	strValue.MakeUpper();

	if( _T("Y") == strValue )
	{
		GetDlgItem(IDC_RADIO4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STA_SENDYN)->ShowWindow(SW_SHOW);
		CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_SENDYN);
		if( pCmb )
		{
			pCmb->ShowWindow(SW_SHOW);
			pCmb->SetCurSel(0);
		}

	}

	GetManageValue(_T("기타"), _T("공통"), _T("SMS발송"), _T("이동이력SMS발송여부"), strValue);
	strValue.TrimLeft();strValue.TrimRight();
	strValue.MakeUpper();
	if(_T("Y") == strValue)
	{
		m_strMobileSMS = _T("Y");
	}
	else
	{
		m_strMobileSMS = _T("N");
	}
	
	if( NULL == m_pInfo || _T("") == m_pInfo->MANAGE_CODE )
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("OnInitDialog"));
 		return FALSE;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CSMS_DLG_01_SEARCH::OnRadio1() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	EnableReturnPlanDateCtrl();

EFS_END
}

VOID CSMS_DLG_01_SEARCH::OnRadio2() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	EnableReturnPlanDateCtrl();

EFS_END
}

VOID CSMS_DLG_01_SEARCH::OnRadio3() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	EnableReturnPlanDateCtrl();

EFS_END
}

VOID CSMS_DLG_01_SEARCH::EnableReturnPlanDateCtrl()
{
EFS_BEGIN

	INT nType = GetSearchType();
    GetDlgItem(IDC_CMB_SENDYN)->EnableWindow(FALSE);
	CStatic *pStatic = ( CStatic * )GetDlgItem( IDC_sDATE );

	CSMS_DLG_01 *pParent	= ( CSMS_DLG_01 * )m_pParent;

	if( _T("Y") == m_strNightLoanReservationUseYn )
	{
		if( 2 == nType || 3 == nType )
		{	
			pStatic->SetWindowText( _T("반납예정일") );
		}
		else if( 4 == nType || 1 == nType )
		{
			pStatic->SetWindowText( _T("예약만기일") );

			if( 1 == nType )
			{
				GetDlgItem(IDC_CMB_SENDYN)->EnableWindow(TRUE);
			}
		}
		else
		{
			pStatic->SetWindowText( _T("반납예정일") );
		}
	}
	else
	{
		if( 1 == nType || 2 == nType )
		{	
			pStatic->SetWindowText( _T("반납예정일") );
		}
		else if( 3 == nType || 4 == nType )
		{
			pStatic->SetWindowText( _T("예약만기일") );

			if( 4 == nType )
			{
				GetDlgItem(IDC_CMB_SENDYN)->EnableWindow(TRUE);
			}
		}
		else
		{
			pStatic->SetWindowText( _T("반납예정일") );
		}	
	}

EFS_END
}

INT CSMS_DLG_01_SEARCH::GetSearchType()
{
EFS_BEGIN

	CButton *pBtn = NULL;

	pBtn = ( CButton * )GetDlgItem( IDC_RADIO4 );

	if( _T("Y") == m_strNightLoanReservationUseYn )
	{
		if( pBtn->GetCheck() )
		{
			return 1;
		}

		pBtn = ( CButton * )GetDlgItem( IDC_RADIO1 );
		if( pBtn->GetCheck() )
		{
			return 2;
		}

		pBtn = ( CButton * )GetDlgItem( IDC_RADIO2 );
		if( pBtn->GetCheck() )
		{
			return 3;
		}

		pBtn = ( CButton * )GetDlgItem( IDC_RADIO3 );
		if( pBtn->GetCheck() )
		{
			return 4;
		}
	}
	else
	{
		pBtn = ( CButton * )GetDlgItem( IDC_RADIO1 );
		if( pBtn->GetCheck() )
		{
			return 1;
		}

		pBtn = ( CButton * )GetDlgItem( IDC_RADIO2 );
		if( pBtn->GetCheck() )
		{
			return 2;
		}

		pBtn = ( CButton * )GetDlgItem( IDC_RADIO3 );
		if( pBtn->GetCheck() )
		{
			return 3;
		}
	}

	return 0;

EFS_END
return -1;

}

VOID CSMS_DLG_01_SEARCH::OnShowWindow(BOOL bShow, UINT nStatus) 
{
EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	if( bShow )
		EnableReturnPlanDateCtrl();

EFS_END
}


VOID CSMS_DLG_01_SEARCH::GetCondition( CString condition[] )
{
EFS_BEGIN

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

	pEdt = ( CEdit * )GetDlgItem( IDC_edtSMS01_LOAN_NUM_FROM );
	pEdt->GetWindowText( condition[ 4 ] );
	pEdt = ( CEdit * )GetDlgItem( IDC_edtSMS01_LOAN_NUM_TO );
	pEdt->GetWindowText( condition[ 5 ] );
	
	pEdt = ( CEdit * )GetDlgItem( IDC_edtSMS01_USER_NAME_FROM );
	pEdt->GetWindowText( condition[ 6 ] );
	pEdt = ( CEdit * )GetDlgItem( IDC_edtSMS01_USER_NAME_TO );
	pEdt->GetWindowText( condition[ 7 ] );	
	
	GetControlData( _T("CM_SMS_DLG_01_SEARCH") , _T("대출자생일1"), condition[ 8 ] );	
	GetControlData( _T("CM_SMS_DLG_01_SEARCH") , _T("대출자생일2") , condition[ 9 ] );	

	condition[ 10 ] = GetCtrlDate( IDC_dRETURN_PLAN_DATE_FROM );
	condition[ 11 ] = GetCtrlDate( IDC_dRETURN_PLAN_DATE_TO );

	INT ids = 0;
	CString sEcoMailUseYN = _T("");
	ids = GetManageValue( _T("기타")  , _T("공통") , _T("메일링시스템") , _T("사용여부") , sEcoMailUseYN );
	if( ids < 0 ) sEcoMailUseYN = _T("N");
	if ( _T("Y")!=sEcoMailUseYN )	return;

	condition[12] = TakeControlData( _T("그룹") );

EFS_END
}

CString CSMS_DLG_01_SEARCH::TakeControlData( CString alias, INT option )
{
EFS_BEGIN
	
	CString result;

	INT ids = 0;
	ids = GetControlData( _T("CM_SMS_DLG_01_SEARCH"), alias, result, -1, option );
	if( ids < 0 )
	{
		MessageBox( _T("Error : GetControlData() of CM_SMS_DLG_01_SEARCH"), _T("K2Up") );
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
			MessageBox( _T("Error , Wrong Option : TakeControlData of CM_SMS_DLG_01_SEARCH"), _T("K2Up") );
			return _T("");
			break;
	}

	return result;

EFS_END
return _T("");

}

CString CSMS_DLG_01_SEARCH::GetCtrlDate( UINT id )
{
EFS_BEGIN

	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )GetDlgItem( id );
	CTime time;
	if( GDT_VALID != pDate->GetTime( time ) )
		return _T("");

	CString date;
	date.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
	return date;

EFS_END
return _T("");

}

VOID CSMS_DLG_01_SEARCH::MakeCommonWhere( CString &where, CString condition[] )
{
EFS_BEGIN

	CString tmp;

	where.Format( _T(" (U.USER_CLASS!='3' OR U.MEMBER_CLASS!='1') AND ") );
	
	const INT cnt = 4;
	CString fieldName[ cnt ] = {
		_T("U.USER_POSITION_CODE"), _T("U.USER_CLASS_CODE"), _T("U.USER_CLASS") , _T("U.SHELF_LOC_CODE") };

	INT		i;
	for( i=0 ; i<cnt; i++ )
	{
		if( condition[ i ].GetLength() )
		{
			tmp.Format( _T("%s = '%s' AND "), fieldName[ i ], condition[ i ] );
			where += tmp;
		}
	}
	
	if( FALSE == condition[ 4 ].IsEmpty() && TRUE == condition[ 5 ].IsEmpty() )
	{
		tmp.Format( _T(" U.USER_NO = '%s' AND "), condition[ 4 ] );
		where += tmp;
	}
	else if( FALSE == condition[ 4 ].IsEmpty() && FALSE == condition[ 5 ].IsEmpty() )
	{
		tmp.Format( _T(" U.USER_NO BETWEEN '%s' AND '%s' AND "), condition[ 4 ], condition[ 5 ] );
		where += tmp;
	}
	
	if( FALSE == condition[ 6 ].IsEmpty() && TRUE == condition[ 7 ].IsEmpty() )
	{
		tmp.Format( _T(" U.IDX_USER_NAME BETWEEN '%s' AND '%s힝' AND "), condition[ 6 ] , condition[ 6 ] );
		where += tmp;
	}
	else if( FALSE == condition[ 6 ].IsEmpty() && FALSE == condition[ 7 ].IsEmpty() )
	{
		tmp.Format( _T(" U.IDX_USER_NAME BETWEEN '%s' AND '%s' AND "), condition[ 6 ], condition[ 7 ] );
		where += tmp;
	}

	if( FALSE == condition[ 8 ].IsEmpty() )
	{		

		if( FALSE == condition[ 9 ].IsEmpty() )
		{
			where += _T(" U.BIRTHDAY BETWEEN TO_DATE('") + condition[ 8 ] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + condition[ 9 ] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') AND ");
		}
		else
		{
			where += _T(" U.BIRTHDAY BETWEEN TO_DATE('") + condition[ 8 ] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + condition[ 8 ] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') AND ");
		}
	}
	
	if( FALSE == condition[12].IsEmpty() )
	{
		tmp.Format( 
			_T(" ( SELECT COUNT(*) ")
			_T("FROM MN_LOAN_USER_GROUP_LIST_TBL MLUGLT ")
			_T("WHERE MLUGLT.USER_KEY = U.REC_KEY AND ")
			_T("MLUGLT.GROUP_CODE='%s' ) > 0 AND ") , condition[12] );

		where += tmp;

	}

EFS_END
}

VOID CSMS_DLG_01_SEARCH::MakeExpWhere( CString &where, CString from, CString to )
{
EFS_BEGIN

	CString tmp;

	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_SENDYN);

	if( _T("Y") == m_strNightLoanReservationUseYn )
	{
		INT		nSearchType = GetSearchType();
		switch( nSearchType )
		{
			case 1 :
				if( TRUE != from.IsEmpty() && TRUE != to.IsEmpty() )
				{
					tmp.Format( _T(" L.RESERVATION_EXPIRE_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') AND L.RESERVATION_DATE = ( SELECT MIN( CL_TEMP2.RESERVATION_DATE) FROM LS_WORK_T01 CL_TEMP2 WHERE CL_TEMP2.BOOK_KEY = L.BOOK_KEY AND CL_TEMP2.STATUS IN ( '3' ) ) AND L.STATUS = '3' "), from, to );
				}
				else if( TRUE != from.IsEmpty() )
				{
					tmp.Format( _T(" L.RESERVATION_EXPIRE_DATE = TO_DATE('%s', 'YYYY/MM/DD') AND L.RESERVATION_DATE = ( SELECT MIN( CL_TEMP2.RESERVATION_DATE) FROM LS_WORK_T01 CL_TEMP2 WHERE CL_TEMP2.BOOK_KEY = L.BOOK_KEY AND CL_TEMP2.STATUS IN ( '3' ) ) AND L.STATUS = '3' "), from );
				}
				else 
				{
					tmp.Format( _T(" L.RESERVATION_EXPIRE_DATE IS NOT NULL AND L.RESERVATION_DATE = ( SELECT MIN( CL_TEMP2.RESERVATION_DATE) FROM LS_WORK_T01 CL_TEMP2 WHERE CL_TEMP2.BOOK_KEY = L.BOOK_KEY AND CL_TEMP2.STATUS IN ( '3' ) ) AND L.STATUS = '3' ") );
				}

				where += tmp;
				where += _T(" AND L.NIGHT_RESERVATION_LOAN IN ('Y','O') ");
				if( pCmb )
				{
					CString strValue;
					pCmb->GetLBText( pCmb->GetCurSel(), strValue );
					if( _T("Y") == strValue || _T("N") == strValue )
					{
						where += _T(" AND L.NIGHT_RESERVATION_SMS_SEND_YN='") + strValue + _T("' ");
					}
				}
				break;
			case 2 :
			case 3 :
				if( FALSE == from.IsEmpty() && FALSE == to.IsEmpty() )
				{
					tmp.Format( _T(" L.RETURN_PLAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') AND STATUS IN( '0', '2', 'L', 'D' ) "), from, to );
				}
				else if( FALSE == from.IsEmpty() )
				{
					tmp.Format( _T(" L.RETURN_PLAN_DATE = TO_DATE('%s','YYYY/MM/DD') "), from);
				}
				else
				{
					tmp.Format( _T(" STATUS IN( '0', '2', 'L', 'D' ) ") );
				}
				if(_T("Y") != m_strMobileSMS)
				{
					tmp += _T(" AND (L.L_DEVICE <> 'KOLASIII-이동도서관' OR L.L_DEVICE IS NULL)");
				}
				where += tmp;
				break;
			case 4 :
				if( TRUE != from.IsEmpty() && TRUE != to.IsEmpty() )
				{
					tmp.Format( _T(" L.UNMANNED_RESERVATION_LOAN IS NULL AND L.RESERVATION_EXPIRE_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') AND L.RESERVATION_DATE = ( SELECT MIN( CL_TEMP2.RESERVATION_DATE) FROM LS_WORK_T01 CL_TEMP2 WHERE CL_TEMP2.BOOK_KEY = L.BOOK_KEY AND CL_TEMP2.STATUS IN ( '3' ) ) AND L.STATUS = '3' "), from, to );
				}
				else if( TRUE != from.IsEmpty() )
				{
					tmp.Format( _T(" L.UNMANNED_RESERVATION_LOAN IS NULL AND L.RESERVATION_EXPIRE_DATE = TO_DATE('%s','YYYY/MM/DD') AND L.RESERVATION_DATE = ( SELECT MIN( CL_TEMP2.RESERVATION_DATE) FROM LS_WORK_T01 CL_TEMP2 WHERE CL_TEMP2.BOOK_KEY = L.BOOK_KEY AND CL_TEMP2.STATUS IN ( '3' ) ) AND L.STATUS = '3' "), from );
				}
				else
				{
					tmp.Format( _T(" L.UNMANNED_RESERVATION_LOAN IS NULL AND L.RESERVATION_EXPIRE_DATE IS NOT NULL AND L.RESERVATION_DATE = ( SELECT MIN( CL_TEMP2.RESERVATION_DATE) FROM LS_WORK_T01 CL_TEMP2 WHERE CL_TEMP2.BOOK_KEY = L.BOOK_KEY AND CL_TEMP2.STATUS IN ( '3' ) ) AND L.STATUS = '3' ") );
				}

				
				where += tmp;

				where += _T(" AND (L.NIGHT_RESERVATION_LOAN IS NULL OR L.NIGHT_RESERVATION_LOAN='N')");
				break;
			default :
				break;
		}
	}
	else
	{
		INT		nSearchType = GetSearchType();
		switch( nSearchType )
		{
			case 1 :
			case 2 :
				if( TRUE != from.IsEmpty() && TRUE != to.IsEmpty() )
				{
					tmp.Format( _T(" L.RETURN_PLAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') AND STATUS IN( '0', '2', 'L', 'D' ) "), from, to );
				}
				else if( TRUE != from.IsEmpty() )
				{
	 				tmp.Format( _T(" L.RETURN_PLAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') AND STATUS IN( '0', '2', 'L', 'D' ) "), from, from );
				}
				else
				{
					tmp.Format( _T(" STATUS IN( '0', '2', 'L', 'D' ) ") );
				}

				if(_T("Y") != m_strMobileSMS)
				{
					tmp += _T(" AND (L.L_DEVICE <> 'KOLASIII-이동도서관' OR L.L_DEVICE IS NULL)");
				}

				where += tmp;
				break;
			case 3 :				
				if( TRUE != from.IsEmpty() && TRUE != to.IsEmpty() )
				{				
	 				tmp.Format( _T(" L.UNMANNED_RESERVATION_LOAN IS NULL AND L.RESERVATION_EXPIRE_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') AND L.RESERVATION_DATE = ( SELECT MIN( CL_TEMP2.RESERVATION_DATE) FROM LS_WORK_T01 CL_TEMP2 WHERE CL_TEMP2.BOOK_KEY = L.BOOK_KEY AND CL_TEMP2.STATUS IN ( '3' ) ) AND L.STATUS = '3' "), from, to );
				}
				else if( TRUE != from.IsEmpty() )
				{
					tmp.Format( _T(" L.UNMANNED_RESERVATION_LOAN IS NULL AND L.RESERVATION_EXPIRE_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS') AND L.RESERVATION_DATE = ( SELECT MIN( CL_TEMP2.RESERVATION_DATE) FROM LS_WORK_T01 CL_TEMP2 WHERE CL_TEMP2.BOOK_KEY = L.BOOK_KEY AND CL_TEMP2.STATUS IN ( '3' ) ) AND L.STATUS = '3' "), from, from );	 				
				}
				else
				{
					tmp.Format( _T(" L.UNMANNED_RESERVATION_LOAN IS NULL AND L.RESERVATION_EXPIRE_DATE IS NOT NULL AND L.RESERVATION_DATE = ( SELECT MIN( CL_TEMP2.RESERVATION_DATE) FROM LS_WORK_T01 CL_TEMP2 WHERE CL_TEMP2.BOOK_KEY = L.BOOK_KEY AND CL_TEMP2.STATUS IN ( '3' ) ) AND L.STATUS = '3' ") );
				}

				where += tmp;
				break;
			default :
				break;
		}
	}

	CString		strManageCode;
	INT ids	 = GetControlData( _T("CM_SMS_DLG_01_SEARCH") , _T("관리구분") , strManageCode , -1 , 0 );
	if( 0 > ids )
	{
		return;
	}
	
	strManageCode.TrimLeft();
	strManageCode.TrimRight();
	
	if( TRUE != strManageCode.IsEmpty() )
	{
		tmp.Format(_T(" AND L.MANAGE_CODE='%s'"), strManageCode );
		where += tmp;
	}

	CString		strGetData;
	ids	 = GetControlData( _T("CM_SMS_DLG_01_SEARCH") , _T("대출자료실") , strGetData , -1 , 0 );
	if( 0 > ids )
	{
		return;
	}

	if( TRUE != strGetData.IsEmpty() )
	{
		tmp.Format(_T(" AND L.SHELF_LOC_CODE='%s'"), strGetData );
		where += tmp;
	}

EFS_END
}




INT CSMS_DLG_01_SEARCH::InitControl()
{
EFS_BEGIN
	
	INT ids;

	CString sEcoMailUseYN;

	ids = GetManageValue( _T("기타")  , _T("공통") , _T("메일링시스템") , _T("사용여부") , sEcoMailUseYN );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("MakeInitInfo") );
	}

	if( 0 != sEcoMailUseYN.Compare(_T("Y")) )
	{
		GetDlgItem(IDC_staSD01_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cSD01S_GROUP)->ShowWindow(SW_HIDE);
		return 0;
	}

	CESL_DataMgr	*pDM	=	FindDM(_T("DM_USER_GROUP_CODE_INFO"));
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("InitControl") );
	}

	CESL_ControlMgr*	pCM	=	FindCM(_T("CM_SMS_DLG_01_SEARCH"));
	if( NULL == pCM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("InitControl") );
	}

	CESL_ControlMultiComboBox* pCMCB	=	(CESL_ControlMultiComboBox*)(pCM->FindControl(_T("그룹")));
	if( NULL == pCMCB )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("InitControl") );
	}
	
	pCMCB->InitMultiComboBox(2);

	CString		sInsertCode[2];
	sInsertCode[0] = _T("   ");
	sInsertCode[1] = _T("적용안함");
	ids = pCMCB->SetData( sInsertCode , 2 );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("InitControl") );
	}

	INT	i;
	for(  i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("코드") , i , sInsertCode[0] );
		pDM->GetCellData( _T("이름") , i , sInsertCode[1] );

		sInsertCode[0].TrimLeft(); sInsertCode[0].TrimRight();
		if( sInsertCode[0].IsEmpty() ) 
		{
			continue;
		}
			
		ids = pCMCB->SetData( sInsertCode , 2 );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1100 , _T("InitControl") );
		}
	}
	
	pCMCB->SetCurSel(0);

	return 0;

EFS_END
return -1;

}

VOID CSMS_DLG_01_SEARCH::OnRadio4() 
{
EFS_BEGIN

	EnableReturnPlanDateCtrl();

EFS_END	
}

HBRUSH CSMS_DLG_01_SEARCH::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CSMS_DLG_01_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	if ( WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam )
	{
		if(		pMsg->hwnd	==	GetDlgItem( IDCANCEL )->GetSafeHwnd() )
		{
			OnCancel();
			return TRUE;
		}

		if(		pMsg->hwnd	==	GetDlgItem( IDC_btnSMS01_SEARCH )->GetSafeHwnd() 
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS01_LOAN_NUM_FROM )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS01_LOAN_NUM_TO )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS01_USER_NAME_FROM )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS01_USER_NAME_TO )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS01_BIRTHDAY_FROM )->GetSafeHwnd()				
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS01_BIRTHDAY_TO )->GetSafeHwnd()
			)
		{
			OnbtnSEARCH();
			return TRUE;
		}
		return FALSE;
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}

void CSMS_DLG_01_SEARCH::OnbtnSEARCH() 
{
EFS_BEGIN

	CSMS_DLG_01 *pParent	= ( CSMS_DLG_01 * )m_pParent;
	INT		nSearchType		= GetSearchType();

	if( _T("Y") == m_strNightLoanReservationUseYn )
	{
		switch( nSearchType )
		{
			case 1 :
				pParent->m_SMSType.Format( _T("NRL03") );
				break;
			case 2 :
				pParent->m_SMSType.Format( _T("SMS01") );
				break;
			case 3 :
				pParent->m_SMSType.Format( _T("SMS02") );
				break;
			case 4 :
				pParent->m_SMSType.Format( _T("SMS03") );
				break;
			default :
				AfxMessageBox( _T("문자 종류를 선택하여 주십시요") );
				return;
				break;
		}
	}
	else
	{
		switch( nSearchType )
		{			
			case 1 :
				pParent->m_SMSType.Format( _T("SMS01") );
				break;
			case 2 :
				pParent->m_SMSType.Format( _T("SMS02") );
				break;
			case 3 :
				pParent->m_SMSType.Format( _T("SMS03") );
				break;
			default :
				AfxMessageBox( _T("문자 종류를 선택하여 주십시요") );
				return;
				break;
		}
	}

	pParent->m_pSMSContentsEditor->SetSearchType( (nSearchType-1) );

	const INT cndCnt = 13;
	CString condition[ cndCnt ];
	GetCondition( condition );
	
	CString where;
	MakeCommonWhere( where, condition );
	MakeExpWhere( where, condition[ 10 ], condition[ 11 ] );
	pParent->SearchUser( where );

EFS_END	
}

void CSMS_DLG_01_SEARCH::OnSetfocusedtSMS01USERNAMEFROM() 
{
EFS_BEGIN

	CEdit *pEditUserNameFrom;
	pEditUserNameFrom = (CEdit*)GetDlgItem(IDC_edtSMS01_USER_NAME_FROM);
		
	HIMC hIME;
	DWORD dwConversion, dwSentence;
	
	hIME = ::ImmGetContext( pEditUserNameFrom->m_hWnd );
	if(hIME) 
	{
		ImmGetConversionStatus( hIME , &dwConversion , &dwSentence );
		
		dwConversion=IME_CMODE_HANGUL;
		
		ImmSetConversionStatus( hIME , dwConversion , dwSentence );
		
		ImmReleaseContext( pEditUserNameFrom->m_hWnd , hIME ); 
	} 

EFS_END			
}

void CSMS_DLG_01_SEARCH::OnSetfocusedtSMS01USERNAMETO() 
{
EFS_BEGIN

	CEdit *pEditUserNameTo;
	pEditUserNameTo = (CEdit*)GetDlgItem(IDC_edtSMS01_USER_NAME_TO);
		
	HIMC hIME;
	DWORD dwConversion, dwSentence;
	
	hIME = ::ImmGetContext( pEditUserNameTo->m_hWnd );
	if(hIME) 
	{
		ImmGetConversionStatus( hIME , &dwConversion , &dwSentence );
		
		dwConversion=IME_CMODE_HANGUL;
		
		ImmSetConversionStatus( hIME , dwConversion , dwSentence );
		
		ImmReleaseContext( pEditUserNameTo->m_hWnd , hIME ); 
	} 

EFS_END			
}

VOID CSMS_DLG_01_SEARCH::SetNightLoanReservationUseYn(CString strNightLoanReservationUseYn)
{
	m_strNightLoanReservationUseYn	=	strNightLoanReservationUseYn;
}
