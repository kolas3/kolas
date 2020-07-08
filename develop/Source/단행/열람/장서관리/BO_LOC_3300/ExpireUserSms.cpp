// ExpireUserSms.cpp : implementation file
//

#include "stdafx.h"
#include "ExpireUserSms.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString g_strKakaoUsed;
/////////////////////////////////////////////////////////////////////////////
// CExpireUserSms dialog


CExpireUserSms::CExpireUserSms(CESL_Mgr* pParent )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CExpireUserSms)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentMgr = pParent;
	// JMJ Coderay
	m_pSMSContentsEditor = NULL;
	m_SMSType = _T("");
}


void CExpireUserSms::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExpireUserSms)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExpireUserSms, CDialog)
	//{{AFX_MSG_MAP(CExpireUserSms)
	ON_BN_CLICKED(IDC_BTN_SMS_CLOSE, OnBtnSmsClose)
	ON_BN_CLICKED(IDC_BTN_SMS_SEND, OnBtnSmsSend)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CExpireUserSms::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CExpireUserSms::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(InitESL_Mgr(_T("ExpireUserSMS")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	CESL_ControlMgr* pCM = FindCM(_T("CM_ExpireUserSMS"));
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("ExpireGrid") ));
	if(!m_pInfo->m_bUserInfoExportYN)
	{	
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("만료회원SMS안내");
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CExpireUserSms message handlers

INT CExpireUserSms::UserExpireSendSMS()
{
	INT ids = 0, nIndex, i, nSelectCnt = 0;
	INT nSuccessCnt = 0, nNoInfo = 0, nError = 0, nSendCnt = 0;
	CString strFuncName = _T("UserExpireSendSMS"), strMessage, strSendCnt;
	CString strUserKey, strUserHandphone, strUserName, strUserExpireDate, strUserLastSendDate;
	CString strErrorMsg;
	CString strQuery;

	SMS_CONNECTION_INFO SMS_ConnectionInfo;
	SMS_MSG_INFO SMS_MsgInfo, SMS_MsgInfo_16, SMS_MsgInfo_17;	
	CString strMsg = _T("");
	
	
	CSMS_MakeMsg MakeSMS;

	CStringList SMSList;
	SMSList.AddTail( _T("SMS16") );
	SMSList.AddTail( _T("SMS17") );
	
	m_pSMSContentsEditor = new CSMSContentsEditDlg( this, SMSList );
	m_pSMSContentsEditor->Create( this );
	m_pSMSContentsEditor->CenterWindow();
	m_pSMSContentsEditor->ShowWindow( false );


	m_pSMSContentsEditor->GetSMSInfo( _T("SMS16"), SMS_MsgInfo_16 , SMS_ConnectionInfo, 1 );
	m_pSMSContentsEditor->GetSMSInfo( _T("SMS17"), SMS_MsgInfo_17 , SMS_ConnectionInfo, 1 );
		
	CESL_DataMgr* pDM = FindDM(_T("DM_ExpireUserSMS"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1001, strFuncName);

	COleDateTime m_tCurrentTime = COleDateTime::GetCurrentTime();	
	COleDateTime tCurrentTime( m_tCurrentTime.GetYear(),m_tCurrentTime.GetMonth(),m_tCurrentTime.GetDay(),0,0,0);

	nSelectCnt = pDM->GetRowCount();
	if(nSelectCnt == 0)
	{
		AfxMessageBox(_T("발송할 데이터가 없습니다."));
		return -1;
	}
	for( i = 0 ; i < nSelectCnt ; i++ )
	{
		
		ids = pDM->GetCellData( _T("이용자키") , i , strUserKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1002, strFuncName);
		ids = pDM->GetCellData( _T("이용자이름") , i , strUserName);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1003, strFuncName);
		ids = pDM->GetCellData( _T("핸드폰번호") , i , strUserHandphone );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1004, strFuncName);
		ids = pDM->GetCellData( _T("파기일") , i , strUserExpireDate );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1005, strFuncName);
		ids = pDM->GetCellData( _T("발송횟수") , i , strSendCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1005, strFuncName);
		ids = pDM->GetCellData( _T("최근발송일") , i , strUserLastSendDate );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1006, strFuncName);

		if(	strUserKey.IsEmpty() == TRUE ||	strUserName.IsEmpty() == TRUE || strUserHandphone.IsEmpty() == TRUE || strUserExpireDate.IsEmpty() == TRUE)
		{
			nError++;
			pDM->SetCellData(_T("성공여부"), _T("X"), i);
			if(strUserExpireDate.IsEmpty() == TRUE)
				strErrorMsg.Format(_T("동의를 받은 대상이 아님(만료일없음)"));
			else if(strUserHandphone.IsEmpty() == TRUE)
				strErrorMsg.Format(_T("핸드폰번호가 입력되지 않음"));
			else if(strUserName.IsEmpty() == TRUE)
				strErrorMsg.Format(_T("이름이 없음"));
			else 
				strErrorMsg.Format(_T("정의되지 않음"));

			pDM->SetCellData(_T("실패사유"), strErrorMsg, i);
			continue;
		}

		if(10 == strUserLastSendDate.GetLength())
		{
			COleDateTime tLastTime(_ttoi(strUserLastSendDate.Left(4)),_ttoi(strUserLastSendDate.Mid(5, 2)),_ttoi(strUserLastSendDate.Right(2)),0,0,0);
			COleDateTimeSpan tSpan = tCurrentTime - tLastTime;
			INT nLastDay = (INT)tSpan.GetDays();
			if(nLastDay <= 7)
			{	
				strMessage.Format(_T("%s 이용자는 최근 일주일 내 보낸 이력이 있습니다.\r\n다시 보내시겠습니까?"), strUserName);
				if(AfxMessageBox(strMessage, MB_YESNO) == IDNO)
					continue;
			}
		}
		


		m_SMSType = _T("SMS16");
		SMS_MsgInfo = SMS_MsgInfo_16;
		if(10 == strUserExpireDate.GetLength())
		{
			COleDateTime tExpireTime(_ttoi(strUserExpireDate.Left(4)),_ttoi(strUserExpireDate.Mid(5, 2)),_ttoi(strUserExpireDate.Right(2)),0,0,0);
			COleDateTimeSpan tSpan = tExpireTime - tCurrentTime;
			INT nExpireDay = (INT)tSpan.GetDays();
			if(nExpireDay > 0)
			{
				m_SMSType = _T("SMS17");
				SMS_MsgInfo = SMS_MsgInfo_17;
			}
		}else{
			nError++;
			pDM->SetCellData(_T("성공여부"), _T("X"), i);
			strErrorMsg.Format(_T("만료일자가 정상적이지 않음.(YYYY/MM/DD)"));
			pDM->SetCellData(_T("실패사유"), strErrorMsg, i);
			continue;
		}

		
		MakeSMS.MakeSMSMsg( m_SMSType, &SMS_MsgInfo, strUserName, strUserExpireDate, _T(""), _T("1"), strMsg );
		ids = CallProcedure( strUserKey, strUserHandphone, strUserName, strMsg, m_SMSType);
		if(ids == -1){
			nError++;
			pDM->SetCellData(_T("성공여부"), _T("X"), i);
			strErrorMsg.Format(_T("발송실패"));
			pDM->SetCellData(_T("실패사유"), strErrorMsg, i);
		}else{
			nSuccessCnt++;
			pDM->SetCellData(_T("성공여부"), _T("O"), i);
			nSendCnt = _ttoi(strSendCnt);
			strSendCnt.Format(_T("%d"), nSendCnt + 1);
			pDM->SetCellData(_T("발송횟수"), strSendCnt, i);

	
			strQuery.Format(_T("INSERT INTO CO_USER_EXPIRE_SMS_LOG_TBL (REC_KEY, USER_KEY, USER_NAME, HANDPHONE, SEND_MESSAGE, EXPIRE_DATE, SEND_DATE, MN_USER_ID) VALUES ")
							_T("(ESL_SEQ.NEXTVAL, %s, '%s', '%s', '%s', '%s', sysdate, '%s');"), 
							strUserKey, strUserName, strUserHandphone, strMsg, strUserExpireDate, m_pInfo->USER_ID);
			pDM->ExecuteQuery(strQuery, TRUE);
			
		}

	}
	AllControlDisplay(_T("CM_ExpireUserSMS"));
	
	
	strMessage.Format(	_T("전  체 : %d건\r\n")
						_T("성  공 : %d건\r\n")
						_T("실  패 : %d건\r\n")
						_T("\r\n처리되었습니다."), nSelectCnt, nSuccessCnt, nError);
	AfxMessageBox(strMessage);


	
	return ids;
}

INT CExpireUserSms::CallProcedure(CString strUserNo, CString strHandPhone, CString strName, CString strMessage, CString strType)
{	
	INT ids;

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);
	
	CString sFirst_Work;
	sFirst_Work.Format( _T("%s"), GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ) );
	
	CString sQuery, sDividCnt, sSendMsg, sSendNum, sRecevNum, sUserKey, sLoanKey, sUserName;
	sRecevNum = strHandPhone;
	CCfgFileApi cfgFileApi;
	CString strCfgFilePath = _T("..\\Cfg\\SMS_Callback.cfg");
	cfgFileApi.SetFile(strCfgFilePath);
	CFileFind ff;
	if(ff.FindFile(strCfgFilePath))
	{
		CString strCallbackTarget;
		cfgFileApi.GetData(_T("회신번호설정적용대상"), strCallbackTarget);
		CString strValue;
		if(_T("1") == strCallbackTarget)
		{
			cfgFileApi.GetData(_T("회신번호"), strValue);
			if(!strValue.IsEmpty()) sSendNum = strValue;
		}
	}
	sRecevNum.Replace(_T("-"), _T(""));
	
	sSendMsg = strMessage;
	sSendMsg.Replace(_T("'"), _T("''"));
	sUserKey = strUserNo;
	sLoanKey = _T("0");
	sUserName = strName;

	if (g_strKakaoUsed.IsEmpty())
	{
		sQuery.Format( 
			_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='KAKAO플러스친구시스템정보' AND value_name='KAKAO플러스친구사용유무' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		TmpDM.GetOneValueQuery(sQuery, g_strKakaoUsed);
		
		if (g_strKakaoUsed.IsEmpty())
		{
			g_strKakaoUsed = _T("N");
		}
	}

	if (g_strKakaoUsed.CompareNoCase(_T("Y")) == 0)
	{
		sQuery.Format( _T("KAKAOPLUS_TOT_SEND_PROC(%s, %s, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');")
								, sUserKey, sLoanKey, sSendNum, sRecevNum, sSendMsg // 1,2,3,4,5
								, _T(""), m_pInfo->MANAGE_CODE, strType				// 6,7,8
								, m_pInfo->USER_NAME, sFirst_Work, sUserName,		// 9,10,11
								m_pInfo->LIB_NAME , sSendMsg );
	}
	else
	{
		sQuery.Format( _T("SMS_TOT_SEND_PROC(%s,%s,'%s','%s','%s','%s','%s','%s','%s','%s','%s');")
				, sUserKey, sLoanKey, sSendNum, sRecevNum, sSendMsg 	// 1,2,3,4,5
				, _T(""), m_pInfo->MANAGE_CODE, strType				// 6,7,8
				, m_pInfo->USER_NAME, sFirst_Work, sUserName );			// 9,10,11
	}

	ids = TmpDM.ExecuteQuery( sQuery, TRUE );
	if(0>ids)
	{
		return -1;
	}
	return 0;
	
}

CESL_Grid* CExpireUserSms::GetGridPtr()
{
	EFS_BEGIN
		
	CString sCMAlias = _T("CM_BO_LOC_3300");
	CString sGridAlias = _T("MainGrid");
	
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) return NULL;
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	
	return pGrid;
	
	EFS_END
	
	return NULL;
}

void CExpireUserSms::OnBtnSmsClose() 
{
	OnCancel();
}

void CExpireUserSms::OnBtnSmsSend() 
{
	UserExpireSendSMS();
}

void CExpireUserSms::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(bShow == TRUE){
		CString strFuncName = _T("ExpireUserSMS::Init"),
				strGetUserKey = _T(""), strUserKey = _T(""),
				strWhere = _T("");

		INT ids = 0, nIndex, i, nSelectCnt = 0;
		CESL_Grid *pGrid = GetGridPtr();
		if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2(-1000, strFuncName);
		
		CESL_DataMgr* pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3300_LIST"));
		if( pDM == NULL ) ERROR_MSG_RETURN_VOID2( -1001, strFuncName);
		
		CESL_DataMgr* pMainDM = FindDM(_T("DM_ExpireUserSMS"));
		if( pDM == NULL ) ERROR_MSG_RETURN_VOID2( -1002, strFuncName);
		
		CESL_DataMgr* pTempDM = FindDM(_T("DM_ExpireUserSMS_Temp"));
		if( pDM == NULL ) ERROR_MSG_RETURN_VOID2( -1003, strFuncName);


		pGrid->SelectMakeList();
		
		nSelectCnt = pGrid->SelectGetCount();
		

		if(nSelectCnt == 0){
			AfxMessageBox(_T("이용자를 선정해주시기 바랍니다."));
			pMainDM->FreeData();
			pTempDM->FreeData();
			OnCancel();
		}else if(nSelectCnt > 1000){
			AfxMessageBox(_T("선정된 데이터가 1000건이 넘습니다.\r\n\r\n데이터 건수에 따라 오래걸릴 수 있습니다."));
		}
		
		
		pMainDM->FreeData();

		for( i = 0 ; i < nSelectCnt ; i++ )
		{
			nIndex = pGrid->SelectGetIdx();
			
			ids = pDM->GetCellData( _T("REC_KEY") , nIndex , strGetUserKey );
			if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1010, strFuncName);
			
			strUserKey += strGetUserKey;
			
			if(i > 0 && (i%1000 == 0) || i == nSelectCnt - 1){
				strWhere.Format(_T(" REC_KEY IN (%s)"), strUserKey);
				pTempDM->RefreshDataManager(strWhere);
				pMainDM->AddDM(pTempDM);
				strUserKey = _T("");
			}
			if( (1 < nSelectCnt && i == 0) || ( i%1000 != 0 && i < nSelectCnt)){
				strUserKey += _T(",");
			}
			
			pGrid->SelectGetNext();
			
		}
		pTempDM->FreeData();
		AllControlDisplay(_T("CM_ExpireUserSMS"));
	}
}
