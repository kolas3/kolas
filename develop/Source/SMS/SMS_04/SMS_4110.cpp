// SMS_4110.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_4110.h"
#include "StdioFileEx.h"
#include "..\CXroshotManager\SMSSendDlg.h"
#include "SMS_4100.h"
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_4110 dialog


CSMS_4110::CSMS_4110(CESL_Mgr* pParent , /*CSMSContentsEditDlg *pContentsEditDlg ,*/ CString strMenuAlias )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_4110)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sMenuAlias = strMenuAlias;	
	m_pParent = pParent;
}

CSMS_4110::~CSMS_4110()
{
}

VOID CSMS_4110::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_4110)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_4110, CDialog)
	//{{AFX_MSG_MAP(CSMS_4110)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnSEND, OnbtnSEND)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnSMS04Search, OnbtnSMS04Search)
	ON_BN_CLICKED(IDC_btnSMS04FileInput, OnbtnSMS04FileInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_4110 message handlers

BOOL CSMS_4110::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids = InitESL_Mgr( _T("SMS_4110") );
	if( ids < 0 ) 
	{
		AfxMessageBox( _T("InitESL_Mgr Error") );
	}
	
	InitScreen();

	InitControl();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSMS_4110::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

INT CSMS_4110::InitScreen()
{
	EFS_BEGIN

	((CButton*)GetDlgItem(IDC_radSEARCHGRID))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_radNEW))->SetCheck(TRUE);

	((CComboBox*)GetDlgItem(IDC_cmbINFERIORCLASS2))->SetCurSel(1);
	((CButton*)GetDlgItem(IDC_chkHANDPHONE))->SetCheck(TRUE);
	
	return 0;

	EFS_END

	return -1;
}

VOID CSMS_4110::OnbtnCLOSE() 
{
	OnCancel();	
}

void CSMS_4110::OnbtnSMS04Search() 
{
	AfxGetApp()->DoWaitCursor(1);
	
	INT ids = SearchUser();

	AfxGetApp()->DoWaitCursor(-1);
}


INT CSMS_4110::SearchUser()
{
	EFS_BEGIN

	INT ids;

	CString sWhere;
	ids = GetSqlWhereFromCM(sWhere);
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SearchUser") );
	}

	if( 0 < ids )
	{
		return 1;
	}

	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_4100") );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SearchUser") );
	}

	ids = pDM->RefreshDataManager( sWhere );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SearchUser") );
	}

	INT		nRowCnt	=	pDM->GetRowCount();
	if( 0 == nRowCnt )
	{
		AfxMessageBox( _T("검색된 결과가 존재하지 않습니다.") );
	}
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CString strEnc;
		CStringArray asDes1, asDes2;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("주민등록번호"), i, strEnc);
			asDes1.Add(strEnc);
			pDM->GetCellData(_T("휴대폰번호"), i, strEnc);
			asDes2.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes2);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("주민등록번호"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("휴대폰번호"), asDes2.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
	}

	INT		i;
	if( 1 == ((CButton*)GetDlgItem(IDC_chkHANDPHONE))->GetCheck() )
	{
		CString	strHandPhone;
		for( i=nRowCnt-1 ; i>0 ; i-- )
		{
			ids = pDM->GetCellData( _T("휴대폰번호") , i , strHandPhone );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SearchUser") );
			}

			strHandPhone.TrimLeft();		strHandPhone.TrimRight();
			strHandPhone.Replace( _T("-") , _T("") );
			strHandPhone.Replace( _T(" ") , _T("") );
			if( TRUE == strHandPhone.IsEmpty() )
			{
				pDM->DeleteRow( i );
			}
		}

		nRowCnt	=	pDM->GetRowCount();
	}
		
	CString		strBuf , strDesc , strCode , strUserClass , strUserPosition , strCivilNo;
	for( i=0 ; i<nRowCnt ; i++ )
	{
		ids = pDM->GetCellData( _T("이용자번호"), i, strCode );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("SearchUser") );
		}

		CLocCommonAPI::ChangeSecuriyUserNo( this, strCode , m_sIsUnityLoanService );

		ids = pDM->SetCellData( _T("대출자번호설명"), strCode, i );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("SearchUser") );
		}
		
		strDesc.Empty();
		ids = pDM->GetCellData( _T("대출직급정보"), i, strUserClass );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("SearchUser") );
		}

		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출직급정보") , strUserClass , strDesc );
		ids = pDM->SetCellData( _T("직급설명"), strDesc, i );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("SearchUser") );
		}

		strDesc.Empty();
		ids = pDM->GetCellData( _T("대출소속정보"), i, strUserPosition );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("SearchUser") );
		}

		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출소속정보") , strUserPosition , strDesc );
		ids = pDM->SetCellData( _T("소속설명"), strDesc, i );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("SearchUser") );
		}

		ids = pDM->GetCellData( _T("주민등록번호"), i, strCivilNo );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1100 , _T("SearchUser") );
		}

		CLocCommonAPI::ChangeSecuriyCivilNo ( this , strCivilNo , m_sIsUnityLoanService );

		ids = pDM->SetCellData( _T("주민등록번호"), strCivilNo, i );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1110 , _T("SearchUser") );
		}
	}

	CString sData;
	for( i=0 ; i<nRowCnt ;i++ )
	{
		ids = pDM->GetCellData( _T("회원상태") , i , sData );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1120 , _T("SearchUser") );
		}

		if( sData.Compare(_T("0")) == 0 )
		{
			sData = _T("일반회원");
		}
		else if( sData.Compare(_T("1")) == 0 )
		{
			sData = _T("대출정지");
		}
		else if( sData.Compare(_T("2")) == 0 )
		{
			sData = _T("제적회원");
		}

		ids = pDM->SetCellData( _T("회원상태") , sData , i );		
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1130 , _T("SearchUser") );
		}
	}

	AllControlDisplay( _T("CM_SMS_4100") );

	return 0;

	EFS_END

	return -1;
}

INT CSMS_4110::GetSqlWhereFromCM( CString &sWhere , INT nMode )
{
EFS_BEGIN

	INT ids;

	CString strCMAlias = _T("CM_SMS_4110");

	sWhere.Format( _T(" (U.USER_CLASS!='3' OR U.MEMBER_CLASS!='1') ") );

	const INT nCnt = 13;
	CString strControlAlias[nCnt] =
	{
		_T("대출소속정보"),		_T("대출직급정보"), _T("회원상태"), 
		_T("SMS수신여부"),		_T("그룹"),			_T("자료실"),
		_T("대출자번호1"),		_T("대출자번호2"),	_T("대출자이름1"),
        _T("대출자이름2"),	    _T("대출자생일1"),  _T("회원구분"),
		_T("대출자생일2")
	};

	CString strGetData[nCnt];

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = GetControlData( strCMAlias , strControlAlias[i] , strGetData[i] , -1 , 0 );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("GetSqlWhereFromCM") );
		}

		strGetData[i].TrimLeft();			strGetData[i].TrimRight();
	}

	if( 0 == strGetData[2].Compare(_T("적용안함")) )
	{
		strGetData[2] = _T("");
	}
	else if( 0 == strGetData[2].Compare(_T("일반회원")) )
	{
		strGetData[2] = _T("0");
	}
	else if( 0 == strGetData[2].Compare(_T("대출정지")) )
	{
		strGetData[2] = _T("1");
	}
	else if( 0 == strGetData[2].Compare(_T("제적회원")) )
	{
		strGetData[2] = _T("2");
	}
	if( 0 == strGetData[11].Compare(_T("적용안함")) )
	{
		strGetData[11] = _T("");
	}
	else if( 0 == strGetData[11].Compare(_T("정회원")) )
	{
		strGetData[11] = _T("0");
	}
	else if( 0 == strGetData[11].Compare(_T("비회원")) )
	{
		strGetData[11] = _T("1");
	}
	else if( 0 == strGetData[11].Compare(_T("준회원")) )
	{
		strGetData[11] = _T("2");
	}
	
	if( TRUE != strGetData[0].IsEmpty() )
	{
		if( TRUE != sWhere.IsEmpty() ) 
		{
			sWhere += _T(" AND ");
		}

		sWhere += _T("U.USER_POSITION_CODE = '") + strGetData[0] + _T("' ");
	}
	
	if( TRUE != strGetData[1].IsEmpty() )
	{
		if( TRUE != sWhere.IsEmpty() ) 
		{
			sWhere += _T(" AND ");
		}

		sWhere += _T("U.USER_CLASS_CODE = '") + strGetData[1] + _T("' ");		
	}
	
	if( TRUE != strGetData[2].IsEmpty() )
	{
		if( TRUE != sWhere.IsEmpty() ) 
		{
			sWhere += _T(" AND ");
		}

		sWhere += _T("U.USER_CLASS = '") + strGetData[2] + _T("' ");		
	}

	if( TRUE != strGetData[11].IsEmpty() )
	{
		if( TRUE != sWhere.IsEmpty() ) 
		{
			sWhere += _T(" AND ");
		}

		sWhere += _T("U.MEMBER_CLASS = '") + strGetData[11] + _T("' ");		
	}

	if( 0 == nMode )
	{
		if( 0 == strGetData[3].Compare(_T("전체")) )
		{
			strGetData[3] = _T("");
		}

		if( TRUE != strGetData[3].IsEmpty() )
		{
			if( TRUE != sWhere.IsEmpty() ) 
			{
				sWhere += _T(" AND ");
			}

			if( 0 == strGetData[3].Compare(_T("Y")) )
			{
				sWhere += _T("U.SMS_USE_YN = 'Y'");
			}
			else if( 0 == strGetData[3].Compare(_T("N")) )
			{
				sWhere += _T("( U.SMS_USE_YN = 'N' OR U.SMS_USE_YN IS NULL )");
			}
		}
		
		if( 1 == ((CButton*)GetDlgItem(IDC_chkHANDPHONE))->GetCheck() )
		{
			if( TRUE != sWhere.IsEmpty() ) 
			{
				sWhere += _T(" AND ");
			}

			sWhere += _T(" U.HANDPHONE IS NOT NULL ");
		}
	}

	if( TRUE != strGetData[4].IsEmpty() )
	{
		if( TRUE != sWhere.IsEmpty() ) 
		{
			sWhere += _T(" AND ");
		}

		sWhere += 
			_T(" ( SELECT COUNT(*) ")
			_T("FROM MN_LOAN_USER_GROUP_LIST_TBL MLUGLT ")
			_T("WHERE MLUGLT.USER_KEY = U.REC_KEY AND ")
			_T("MLUGLT.GROUP_CODE='")+ strGetData[4] +_T("' ) > 0 ") ;
	}

	if( TRUE != strGetData[5].IsEmpty() )
	{
		if( TRUE != sWhere.IsEmpty() ) 
		{
			sWhere += _T(" AND ");
		}

		sWhere += _T("U.SHELF_LOC_CODE = '") + strGetData[5] + _T("' ");		
	}

	if( TRUE != strGetData[6].IsEmpty() )
	{
		if( TRUE != sWhere.IsEmpty() ) 
		{
			sWhere += _T(" AND ");
		}

		if( TRUE != strGetData[7].IsEmpty() )
		{
			sWhere += _T(" U.USER_NO BETWEEN '") + strGetData[6] + _T("' AND '") + strGetData[7] + _T("' ");
		}
		else
		{
			sWhere += _T(" U.USER_NO = '") + strGetData[6] + _T("' ");
		}
	}

	if( TRUE != strGetData[8].IsEmpty() )
	{
		if( TRUE != sWhere.IsEmpty() ) 
		{
			sWhere += _T(" AND ");
		}

		if( TRUE != strGetData[9].IsEmpty() )
		{
			sWhere += _T(" U.IDX_USER_NAME BETWEEN '") + strGetData[8] + _T("' AND '") + strGetData[9] + _T("' ");
		}
		else
		{
			sWhere += _T(" U.IDX_USER_NAME BETWEEN '") + strGetData[8] + _T("' AND '") + strGetData[8] + _T("힝' ");
		}
	}

	if( TRUE != strGetData[10].IsEmpty() )
	{
		if( TRUE != sWhere.IsEmpty() ) 
		{
			sWhere += _T(" AND ");
		}

		if( TRUE != strGetData[12].IsEmpty() )
		{
			sWhere += _T(" U.BIRTHDAY BETWEEN TO_DATE('") + strGetData[10] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + strGetData[12] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}
		else
		{
			sWhere += _T(" U.BIRTHDAY BETWEEN TO_DATE('") + strGetData[10] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + strGetData[10] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}
	}

	return 0;

EFS_END

	return -1;
}

VOID CSMS_4110::OnbtnSEND() 
{
	AfxGetApp()->DoWaitCursor(1);

	INT ids = SendSMSMsg();

	AfxGetApp()->DoWaitCursor(-1);
	
}

INT CSMS_4110::SendSMSMsg()
{
EFS_BEGIN

	INT ids;

	CString sSMSType = GetSMSType();	

	CESL_DataMgr* pDM = FindDM(_T("DM_SMS_4100"));
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SendSMS") );
	}

	CESL_DataMgr TempDM;

	CLocCommonAPI::MakeCopyDM( this , pDM , &TempDM );

	if( 0 == sSMSType.Compare( _T("SMS09") ) )
	{
		ids = MakeDMFromContentDlg( &TempDM );
	}

	INT		nRowCnt		=	TempDM.GetRowCount();
	if( 0 == nRowCnt )
	{
		if( 1 == ((CButton*)GetDlgItem(IDC_radSEARCHGRID))->GetCheck() )
		{
			ids = MakeDMFromGrid(&TempDM,pDM);
		}
		else
		{
			ids = SearchToTempDM(&TempDM);
		}
	}

	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SendSMS") );
	}

	if( 0 < ids ) 
	{
		return 1;
	}

	if( 0 == nRowCnt ) 
	{
		AfxMessageBox( _T("문자를 보낼 이용자가 존재하지 않습니다.") );
		return 2;
	}

	ids = SendSMSFromTempDM( &TempDM );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SendSMS") );
	}

	return 0;

EFS_END

	return -1;

}

INT CSMS_4110::MakeDMFromGrid(CESL_DataMgr* pTempDM,CESL_DataMgr *pSrcDM)
{
	EFS_BEGIN
		
	INT ids;
	
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_SMS_4100") , _T("검색된이용자그리드") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeDMFromGrid") );

	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt == 0 ) return 0;

	INT nSelectedIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nSelectedIndex = pGrid->SelectGetIdx();

		pTempDM->InsertRow(-1);
		ids = CLocCommonAPI::AddDM( pSrcDM , pTempDM , nSelectedIndex , i , this );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeDMFromGrid") );

		pGrid->SelectGetNext();
	}

	return 0;

	EFS_END

	return -1;
}



INT CSMS_4110::SearchToTempDM(CESL_DataMgr* pTempDM)
{
	EFS_BEGIN
		
	INT ids;
	
	CString sWhere;
	
	ids = GetSqlWhereFromCM( sWhere , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchToTempDM") );

	if( !sWhere.IsEmpty() )
		sWhere += _T(" AND ");
	sWhere += _T(" SMS_USE_YN = 'Y' ");
	if( !sWhere.IsEmpty() )
		sWhere += _T(" AND ");
	sWhere += _T(" HANDPHONE IS NOT NULL ");

	ids = pTempDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchToTempDM") );

	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pTempDM->GetRowCount())
	{
		CString strEnc;
		CStringArray asDes1, asDes2;
		for(INT i = 0; i < pTempDM->GetRowCount(); i++)
		{
			pTempDM->GetCellData(_T("주민등록번호"), i, strEnc);
			asDes1.Add(strEnc);
			pTempDM->GetCellData(_T("휴대폰번호"), i, strEnc);
			asDes2.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes2);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pTempDM->GetRowCount(); i++)
				{
					pTempDM->SetCellData(_T("주민등록번호"), asDes1.GetAt(i), i);
					pTempDM->SetCellData(_T("휴대폰번호"), asDes2.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
	}

	return 0;

	EFS_END

	return -1;
}

INT CSMS_4110::SendSMSFromTempDM( CESL_DataMgr* pTempDM )
{
EFS_BEGIN

	INT ids;

	CString				strSMSType = GetSMSType();

	SMS_MSG_INFO		SMS_MsgInfo;
	SMS_CONNECTION_INFO SMS_ConnectionInfo;

	CSMS_MakeMsg	MakeSMS(this);
	CSMS_Env_Info	SMSEnvInfo(this);

	CString			strUserName , strSendPhone , strMsg , strData , strUserKey , strSMSReceiptYN;
	CStringList		slName , slPhonNum , slContent , slUserKey , slLoanKey , slSMSReceiptYN;
	CStringArray	asPositionCode , asClassCode , asInferiorClass , asUserID , asBirthDay , asCivilNo;

	const INT nExAliasCnt = 6;
	CString sExAlias[nExAliasCnt] =
	{
		_T("대출소속정보"),	_T("대출직급정보"),	_T("회원상태코드") , 
		_T("이용자ID"),		_T("생일"),			_T("주민등록번호")
	};
	
	INT		i , j , nRowCnt;
	nRowCnt		=	pTempDM->GetRowCount();	
	for( i=0 ; i<nRowCnt ; i++ )
	{
		ids = pTempDM->GetCellData( _T("이용자명") , i , strUserName );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SendSMSFromTempDM") );
		}

		ids = pTempDM->GetCellData( _T("휴대폰번호") , i , strSendPhone );
		if( 0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SendSMSFromTempDM") );
		}
		
		MakeSMS.MakeSMSMsg( strSMSType , &SMS_MsgInfo , strUserName , _T("") , _T("") , _T("0") , strMsg );

		slName.AddTail( strUserName );
		slPhonNum.AddTail( strSendPhone );
		slContent.AddTail( strMsg );
		
		ids = pTempDM->GetCellData( _T("이용자KEY"), i , strUserKey );
		if( 0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SendSMSFromTempDM") );
		}

		ids = pTempDM->GetCellData( _T("SMS_USE_YN"), i , strSMSReceiptYN );
		if( 0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("SendSMSFromTempDM") );
		}

		slUserKey.AddTail( strUserKey );
		slLoanKey.AddTail( _T("0") );
		slSMSReceiptYN.AddTail( strSMSReceiptYN );

		for( j=0 ; j<nExAliasCnt ; j++ )
		{
			ids = pTempDM->GetCellData( sExAlias[j] , i , strData );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SendSMSFromTempDM") );
			}

			switch(j)
			{
			case 0:
				asPositionCode.Add( strData );
				break;
			case 1:
				asClassCode.Add( strData );
				break;
			case 2:
				asInferiorClass.Add( strData );
				break;
			case 3:
				asUserID.Add( strData );
				break;
			case 4:
				asBirthDay.Add( strData );
				break;
			case 5:
				asCivilNo.Add( strData );
				break;
			}
		}
		
	}

	CSMSSendDlg dlg( this, SMS_ConnectionInfo, strSMSType, SMS_MsgInfo.sCallBackNumber, 
					slUserKey , slLoanKey , slName , slPhonNum , slContent , slSMSReceiptYN , 0 );
	dlg.SetInfoEx(  asPositionCode ,	asClassCode , asInferiorClass , asUserID , asBirthDay , asCivilNo );
	dlg.DoModal();		
	
	return 0;

EFS_END

	return -1;
}

CString CSMS_4110::GetSMSType()
{
EFS_BEGIN

	if( ((CButton*)GetDlgItem(IDC_radNEW))->GetCheck() == 1 )
		return _T("SMS05");
	else if( ((CButton*)GetDlgItem(IDC_radEVENT))->GetCheck() == 1 )
		return _T("SMS06");
	else if( ((CButton*)GetDlgItem(IDC_radNORMAL))->GetCheck() == 1 )
		return _T("SMS09");

	return _T("");
EFS_END

	return -1;

}

INT CSMS_4110::MakeDMFromContentDlg(CESL_DataMgr* pTempDM)
{
EFS_BEGIN

	return 0;

EFS_END
	return -1;
}


INT CSMS_4110::InitControl()
{
EFS_BEGIN

	INT ids;

	CString sEcoMailUseYN;
	ids = GetManageValue( _T("기타")  , _T("공통") , _T("메일링시스템") , _T("사용여부") , sEcoMailUseYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeInitInfo") );

	if( sEcoMailUseYN.Compare(_T("Y")) != 0 )
	{
		GetDlgItem(IDC_sta4110_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_c4110_GROUP)->ShowWindow(SW_HIDE);
		return 0;
	}


	CESL_DataMgr* pDM = FindDM(_T("DM_USER_GROUP_CODE_INFO"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitControl") );

	CESL_ControlMgr* pCM = FindCM(_T("CM_SMS_4110"));
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

HBRUSH CSMS_4110::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CSMS_4110::PreTranslateMessage(MSG* pMsg) 
{
	if ( WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam )
	{
		if(		pMsg->hwnd	==	GetDlgItem( IDC_btnCLOSE )->GetSafeHwnd() )
		{
			OnCancel();
			return TRUE;
		}

		CWnd *pDateCtrl = (CWnd::FromHandle(pMsg->hwnd))->GetParent();			
		if(		pMsg->hwnd	==	GetDlgItem( IDC_btnSMS04Search )->GetSafeHwnd() 
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS04_USER_NO1 )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS04_USER_NO2 )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS04_USER_NAME1 )->GetSafeHwnd()
			||	pMsg->hwnd	==	GetDlgItem( IDC_edtSMS04_USER_NAME2 )->GetSafeHwnd()			
			||	pDateCtrl->GetSafeHwnd()	==	GetDlgItem(IDC_edtSMS04_USER_BIRTHDAY1)->GetSafeHwnd()
			||	pDateCtrl->GetSafeHwnd()	==	GetDlgItem(IDC_edtSMS04_USER_BIRTHDAY2)->GetSafeHwnd()
			)
		{
			OnbtnSMS04Search();
			return TRUE;
		}

		if(		pMsg->hwnd	==	GetDlgItem( IDC_btnSMS04FileInput )->GetSafeHwnd() )
		{
			OnbtnSMS04FileInput();
			return TRUE;
		}
		
		return FALSE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CSMS_4110::OnbtnSMS04FileInput() 
{
	INT			ids;
	CStringArray strWhere;
	
	ids		=	MakeUserSearchList( strWhere );
	if( 0 != ids )
	{
		return;
	}
	
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_4100") );
	if( NULL == pDM ) 
	{
		return;
	}

	CESL_DataMgr* pDM_Users = new CESL_DataMgr;
	pDM_Users->InitDMPKFromFile(_T("DM_SMS_4100"));
	pDM_Users->SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	for( int j = 0 ; j < strWhere.GetSize() ; j++ )
	{
		ids = pDM_Users->RefreshDataManager( strWhere.GetAt(j) );
		if( 0 < ids) 
		{
			AfxMessageBox(_T("이용자 검색에 문제가 발생했습니다."));
			return;
		}
		
		pDM->AddDM(pDM_Users);
	}
	
	pDM_Users->FreeData();
	pDM_Users = NULL;

	INT		i , nRowCnt;
	nRowCnt		=	pDM->GetRowCount();	
	if( 0 == nRowCnt )
	{
		AfxMessageBox( _T("검색된 결과가 존재하지 않습니다.") );
	}

	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CString strEnc;
		CStringArray asDes1, asDes2;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("주민등록번호"), i, strEnc);
			asDes1.Add(strEnc);
			pDM->GetCellData(_T("휴대폰번호"), i, strEnc);
			asDes2.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes2);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("주민등록번호"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("휴대폰번호"), asDes2.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
	}

	CString		strBuf , strDesc , strCode , strUserClass , strUserPosition , strData;
	for( i=0 ; i<nRowCnt ; i++ )
	{
		pDM->GetCellData( _T("이용자번호"), i, strCode );

		CLocCommonAPI::ChangeSecuriyUserNo( this, strCode , m_sIsUnityLoanService );
		pDM->SetCellData( _T("대출자번호설명"), strCode, i );
		
		strDesc.Empty();
		pDM->GetCellData( _T("대출직급정보"), i, strUserClass );
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출직급정보") , strUserClass , strDesc );
		ids = pDM->SetCellData( _T("직급설명"), strDesc, i );
		if( 0 > ids )
		{
			return;
		}

		strDesc.Empty();
		pDM->GetCellData( _T("대출소속정보"), i, strUserPosition );
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출소속정보") , strUserPosition , strDesc );
		ids = pDM->SetCellData( _T("소속설명"), strDesc, i );
		if( 0 > ids )
		{
			return;
		}

		ids = pDM->GetCellData( _T("회원상태") , i , strData );
		if( 0 > ids ) 
		{
			return;
		}

		if( strData.Compare(_T("0")) == 0 )
		{
			strData = _T("일반회원");
		}
		else if( strData.Compare(_T("1")) == 0 )
		{
			strData = _T("대출정지");
		}
		else if( strData.Compare(_T("2")) == 0 )
		{
			strData = _T("제적회원");
		}

		pDM->SetCellData( _T("회원상태") , strData , i );
	}

	AllControlDisplay( _T("CM_SMS_4100") );
}

INT CSMS_4110::MakeUserSearchList( CStringArray &strWhere )
{
EFS_BEGIN

	TCHAR TmpDir[500];
	GetCurrentDirectory( 500 , TmpDir );
	
	CString		strClientPath , strFilePath;
	CFileDialog fileDlg(	TRUE , _T("Text file(*.txt)") , _T("*.txt") , 
							OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("txt(*.txt)|*.txt|") , NULL );

	if( IDOK != fileDlg.DoModal() )
	{
		return 1010;
	}

 	strFilePath = fileDlg.GetPathName();
	SetCurrentDirectory( TmpDir );
	
	CString		strLine , strTempLine , strTemp;
	CStdioFileEx	fFile;
	if( fFile.Open( strFilePath ,  CFile::modeRead | CFile::typeText) == FALSE ) 
			{
		AfxMessageBox( strFilePath );
		return -1010;
	}

	INT		nFileLength		=	fFile.GetLength();
	if( 1 > nFileLength )
	{
		AfxMessageBox( _T("비어 있는 File 입니다. File을 확인하여 주십시요.") , MB_ICONINFORMATION );
		return 1020;
	}

	TCHAR cUni;
	fFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fFile.SeekToBegin();
		fFile.Read( &cUni, 3 );
		if(  0xBBEF == cUni  )
		{
			fFile.SetCodePage(CP_UTF8);
		}
		else
		{
			fFile.SetCodePage(CP_ACP);
			fFile.SeekToBegin();
		}
	}

	//JOB.2019.0099 : 이용자 1000명 이상 조회되도록 수정
	int cnt = 0;
	CStringArray strUserNos;
	
	while( fFile.ReadString( strLine ) )
	{		
		strLine.TrimLeft();		strLine.TrimRight();
		strLine.Replace(_T("'") , _T("") );

		if( TRUE == strLine.IsEmpty() )
		{
			continue;
		}

		if( cnt > 0 && cnt%1000 == 0 ){
			strUserNos.Add(strTemp);
			strTemp = _T("");
		}

		if( TRUE != strTemp.IsEmpty() )
		{
			strTemp		+=	_T(" , ");
		}

		strTempLine.Format( _T("'%s'") , strLine );
		strTemp		+=	strTempLine;

		cnt+=1;
	}

	fFile.Close();

	if( strTemp.GetLength() > 0 ) strUserNos.Add(strTemp);
	
	for(INT i = 0 ; i < strUserNos.GetSize() ; i++ )
	{		
		strWhere.Add(_T(" USER_NO IN (") + strUserNos.GetAt(i) + _T(")") );
	}

	return 0;

EFS_END
return -1;
}
