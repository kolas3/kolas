// BO_LOC_SECEDE.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_SECEDE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_SECEDE dialog


CBO_LOC_SECEDE::CBO_LOC_SECEDE(CESL_Mgr* pParent, CLoanShareManager* pLoanShareManager)
	: CESL_Mgr(CBO_LOC_SECEDE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_SECEDE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pLoanShareManager = pLoanShareManager;

	m_SpfWork = NULL;
	m_SpfWork = new BL_LOC_USER_PROC( this , m_pLoanShareManager );

	m_strIsChangeSecuriy = _T("");
}

CBO_LOC_SECEDE::~CBO_LOC_SECEDE()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
	if( m_SpfWork != NULL)
		delete m_SpfWork;
	m_SpfWork = NULL;
}

VOID CBO_LOC_SECEDE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_SECEDE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_SECEDE, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_SECEDE)
	ON_BN_CLICKED(IDC_BTN_SECEDER_SEARCH, OnBtnSecederSearch)
	ON_BN_CLICKED(IDC_BTN_SECEDE, OnBtnSecede)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_SECEDE message handlers

BOOL CBO_LOC_SECEDE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	if(InitESL_Mgr(_T("BO_LOC_SECEDE")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}	

	INT ids = GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), m_strIsChangeSecuriy);
	if(0>ids) return FALSE;

	CESL_Grid* pGrid = (CESL_Grid*)this->FindControl( _T("CM_BO_LOC_SECEDER_LIST"), _T("MAIN_GRID") );
	if(!m_pInfo->m_bUserInfoExportYN)
	{	
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("탈퇴처리목록");
	}

	if( 0 > CopySecederList() )
	{
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_SECEDE::OnBtnSecederSearch() 
{
EFS_BEGIN

	if( 1 == SecederSearch() )
	{
		AfxMessageBox( _T("탈퇴신청 이용자가 검색 되었습니다."), MB_ICONINFORMATION );
	}

EFS_END	
}

VOID CBO_LOC_SECEDE::OnBtnSecede() 
{
EFS_BEGIN

	SecedeProc();	

EFS_END	
}

VOID CBO_LOC_SECEDE::OnBtnClose() 
{
EFS_BEGIN

	OnCancel();	

EFS_END	
}

INT CBO_LOC_SECEDE::SecederSearch(CString strWhere/*=_T("")*/)
{
EFS_BEGIN

	INT		ids , nRowCnt , i;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_SECEDER_LIST") );
	if( !pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SecederSearch") );

	if(TRUE == m_pInfo->m_bKLUse)
	{
		if(strWhere.IsEmpty())
		{
			strWhere.Format(_T(" (CLU.KL_MEMBER_YN <> 'Y' OR CLU.KL_MEMBER_YN IS NULL) "));
		}
	}

	ids = pDM->RefreshDataManager( strWhere );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SecederSearch") );

	nRowCnt = pDM->GetRowCount();
	if( 0 == nRowCnt )
	{
		AfxMessageBox( _T("탈퇴 처리할 이용자가 없습니다."), MB_ICONINFORMATION );
		return 0;
	}
	ids = CLocCommonAPI::ChangeSecuriyCivilNo( this , pDM , -1, _T("주민번호") , m_sIsUnityLoanService );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SecederSearch") );

	CString strGetData;
	CStringArray asDes1,asDes2,asDes3;

	if(_T("Y") != m_strIsChangeSecuriy || TRUE == m_pInfo->m_bLibSecuUse)
	{
		CString strUserNo;
		for(INT i = 0; i < nRowCnt; i++)
		{
			if(_T("Y") != m_strIsChangeSecuriy)
			{
				ids = pDM->GetCellData( _T("대출자번호"), i , strUserNo );
				if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1040, _T("SecederSearch"));
				CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_sIsUnityLoanService );
				ids = pDM->SetCellData( _T("대출자번호"), strUserNo , i );
				if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("SecederSearch") );
			}

			if(TRUE == m_pInfo->m_bLibSecuUse)
			{
				pDM->GetCellData(_T("주민번호"), i, strGetData);
				asDes1.Add(strGetData);
				pDM->GetCellData(_T("핸드폰"), i, strGetData);
				asDes2.Add(strGetData);
				pDM->GetCellData(_T("이메일"), i, strGetData);
				asDes3.Add(strGetData);
			}
		}
	}

	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < nRowCnt)
	{
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->CheckConnection())
			{
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes2);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes3);
					p3000Api->CloseSocket();	
				}
				for(i = 0; i < nRowCnt; i++)
				{
					pDM->SetCellData(_T("주민번호"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("핸드폰"), asDes2.GetAt(i), i);
					pDM->SetCellData(_T("이메일"), asDes3.GetAt(i), i);
				}
			}
			delete p3000Api;
			p3000Api = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
	}
	
	ids = m_SpfWork->UserCodeToDesc( pDM );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1110 , _T("SecederSearch") );

	this->AllControlDisplay( _T("CM_BO_LOC_SECEDER_LIST") );	
	
	return 1;

EFS_END	
	return -1;
}

INT CBO_LOC_SECEDE::SecedeProc()
{
EFS_BEGIN
	INT		ids;
	
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_SECEDER_LIST") );
	if( !pDM )	ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_SECEDE::SecedeProc()") );
	if( 0 == pDM->GetRowCount() )
	{
		AfxMessageBox( _T("탈퇴 처리할 이용자가 없습니다."), MB_ICONINFORMATION );
		return 1;
	}

	CESL_DataMgr* pParentDM = FindDM( _T("DM_BO_LOC_3300_LIST") );
	if( !pParentDM ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_SECEDE::SecedeProc()") );
	INT		nParentDMCnt = pParentDM->GetRowCount();
	
	CESL_Grid* pGrid = (CESL_Grid*)this->FindControl( _T("CM_BO_LOC_SECEDER_LIST"), _T("MAIN_GRID") );
	if( !pGrid ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_SECEDE::SecedeProc()") );

	pGrid->SelectMakeList();
	INT nSelectCount = pGrid->SelectGetCount();

	if( 0 == nSelectCount )
	{
		AfxMessageBox( _T("탈퇴 처리할 이용자를 선택해주십시오."), MB_ICONINFORMATION );		
		return 1;
	}	

	if( IDNO == AfxMessageBox( _T("탈퇴 처리 이후에는 회원정보를 복구 할 수 없습니다.\r\n")
		                       _T("계속 진행 하시겠습니까?"), MB_YESNO ) )
	{
		AfxMessageBox( _T("취소되었습니다."), MB_ICONSTOP);
		return 1;
	}

	if ( m_strLibCode.IsEmpty() )
	{
		m_strLibCode = m_pInfo->LIB_CODE;
	}

	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO( this->m_pInfo->CONNECTION_INFO );

	const INT nFieldCnt = 45;
	CString strFieldAlias[nFieldCnt] = {
		_T("USER_NO"),_T("USER_ID"),_T("PASSWORD"),_T("FID_KEY"),_T("BIRTHDAY"),_T("BIRTHDAY_TYPE"),_T("H_ADDR1"),
		_T("W_ADDR1"),_T("H_ZIPCODE"),_T("W_ZIPCODE"),_T("H_PHONE"),_T("W_PHONE"),_T("EXCHANGE_NO"),_T("DEPARTMENT_NAME"),
		_T("CLASS_NAME"),_T("HANDPHONE"),_T("IDX_HANDPHONE"),_T("E_MAIL"),_T("INFERIOR_REASON"),_T("SCHOOL"),_T("GRADE"),_T("CLASS"),_T("CLASS_NO"),
		_T("MAIL_SEND_AREA"),_T("LOAN_COUNT"),_T("RESERVATION_COUNT"),_T("REMARK"),_T("REISSUE_CNT"),_T("NEW_USER_MAIL_DATE"),_T("SHELF_LOC_CODE"),
		_T("USER_PICTURE"),_T("USER_PICTURE_TYPE"),_T("LOST_USER_CARD"),_T("SMS_USE_YN"),_T("MONETA_INFORM_PHONE_NUM"),_T("MONETA_INFORM_DATE"),
		_T("CARD_PASSWORD"),_T("USER_DEFINE_CODE1"),_T("USER_DEFINE_CODE2"),_T("USER_DEFINE_CODE3"),_T("OFFICE"),
		_T("WORKNO"),_T("ARMY_CLASS"),_T("MAILING_USE_YN"), _T("CIVIL_NO")
	};
	
	INT		nIndex;
	CString strRecKey;
	
	pGrid->SelectGetTailPosition();
	for( INT i=nSelectCount-1;i>=0;i-- )
	{
		nIndex = pGrid->SelectGetIdx();

		ids = pDM->GetCellData( _T("REC_KEY"), nIndex, strRecKey );
		if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_SECEDE::SecedeProc()") );

		CString strMailUseYN;
		CString strMailAddress;
		ids = pDM->GetCellData( _T("이메일수신유무"), nIndex, strMailUseYN );
		if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_SECEDE::SecedeProc()") );
		
		strMailUseYN.TrimLeft();strMailUseYN.TrimRight();
		strMailUseYN.MakeUpper();
		if( _T("Y") == strMailUseYN )
		{
			ids = pDM->GetCellData( _T("이메일"), nIndex, strMailAddress );
			if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CBO_LOC_SECEDE::SecedeProc()") );

			strMailAddress.TrimLeft();strMailAddress.TrimRight();
			if( strMailAddress.GetLength() )
			{
			}
		}

		CString sUserNo, sUserName;
		ids = pDM->GetCellData( _T("대출자번호"), nIndex, sUserNo );
		if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CBO_LOC_SECEDE::SecedeProc()") );
		ids = pDM->GetCellData( _T("대출자명"), nIndex, sUserName );
		if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CBO_LOC_SECEDE::SecedeProc()") );
		ids = SetSecedeLog(strRecKey, sUserNo, sUserName);
		if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CBO_LOC_SECEDE::SecedeProc()") );
		
		CKolas2up_MobileApi* pMoblieApi = CLocCommonAPI::InitMobileApi( this , &DM );
		DM.StartFrame();

		for( INT j=0;j<nFieldCnt;j++ )
		{
			ids = DM.AddDBFieldData( strFieldAlias[j], _T("NUMERIC"), _T("NULL")  );
			if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CBO_LOC_SECEDE::SecedeProc()") );
		}

		ids = DM.AddDBFieldData( _T("USER_CLASS"), _T("STRING"), _T("3"), TRUE );
		if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_SECEDE::SecedeProc()") );

		ids = DM.AddDBFieldData( _T("MEMBER_CLASS"), _T("STRING"), _T("1"), TRUE );
		if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_SECEDE::SecedeProc()") );
		
		ids = DM.AddDBFieldData( _T("SECEDE_DATE"), _T("NUMERIC"), _T("SYSDATE"), TRUE );
		if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_SECEDE::SecedeProc()") );

		ids = DM.AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("탈퇴처리"), __WFILE__, __LINE__ ), TRUE );
		if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_SECEDE::SecedeProc()") );

		pMoblieApi->MakeUpdateFrame( _T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey );		

		ids = pMoblieApi->SendFrame();
		if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CBO_LOC_SECEDE::SecedeProc()") );

		DM.EndFrame();
		CLocCommonAPI::MobileApiEndLog( pMoblieApi );		
		
		DM.StartFrame();
		CString strQuery;
		strQuery.Format(_T("DELETE CO_AGENT_INFO_TBL WHERE USER_KEY = %s;"), strRecKey);
		DM.AddFrame(strQuery);
		strQuery.Format(_T("DELETE CO_AGREEMENT_INFO_TBL WHERE USER_KEY = %s;"), strRecKey);
		DM.AddFrame(strQuery);
		DM.EndFrame();

		UPLOAD_SHARE_INFO UploadShareInfo;
		UploadShareInfo.nType = 0;
		
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
		UploadShareInfo.UploadShareLoanInfo.sType = _T("D");		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = strRecKey;
		UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;		
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );	
		
		pGrid->SelectGetPrev();
		pDM->DeleteRow( nIndex );
		
		for(INT k=0;k<nParentDMCnt;k++ )
		{
			CString strParentRecKey;
			ids = pParentDM->GetCellData(_T("REC_KEY"),k,strParentRecKey);
			if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("CBO_LOC_SECEDE::SecedeProc()") );

			strParentRecKey.TrimLeft();strParentRecKey.TrimRight();
			if( strParentRecKey == strRecKey )
			{
				pParentDM->DeleteRow(k);
				break;
			}
		}
	}	

	this->ControlDisplay( _T("CM_BO_LOC_3300"), _T("MainGrid") );
	this->AllControlDisplay( _T("CM_BO_LOC_SECEDER_LIST") );

	AfxMessageBox( _T("탈퇴 처리가 완료 되었습니다."), MB_ICONINFORMATION );

	return 0;

EFS_END
	return -1;
}

INT CBO_LOC_SECEDE::CopySecederList()
{
EFS_BEGIN
	
	INT		ids;
	INT		nMode; 
	INT		nIndex;
	CString strWhere;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3300_LIST") );
	if( !pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_SECEDE::CopySecederList()") );		
	if( 0 < pDM->GetRowCount() )
	{
		CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3300"), _T("MainGrid") );
		if( !pGrid ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_SECEDE::CopySecederList()") );		

		pGrid->SelectMakeList();
		INT nSelectCount = pGrid->SelectGetCount();
		
		if( nSelectCount )
		{
			nMode = 1;
			pGrid->SelectGetHeadPosition();
		}
		else
		{
			nMode = 0;
			nSelectCount = pDM->GetRowCount();
		}

		CString strInferiorClass,strRecKey,strRecKeyList, strKLMemberYN;
		for( INT i=0;i<nSelectCount;i++ )
		{
			if( nMode )
			{
				nIndex = pGrid->SelectGetIdx();
			}
			else
			{
				nIndex = i;
			}
			
			ids = pDM->GetCellData( _T("불량회원구분"), nIndex, strInferiorClass );
			if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_SECEDE::CopySecederList()") );
			strInferiorClass.TrimLeft();	strInferiorClass.TrimRight();

			if(TRUE == m_pInfo->m_bKLUse)
			{
				ids = pDM->GetCellData( _T("통합회원전환여부"), nIndex, strKLMemberYN );
				if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_SECEDE::CopySecederList()") );
				strKLMemberYN.TrimLeft();	strKLMemberYN.TrimRight();
			}
			else
			{
				strKLMemberYN = _T("N");
			}

			if( (_T("3") == strInferiorClass || _T("탈퇴") == strInferiorClass || _T("탈퇴회원") == strInferiorClass) && _T("Y")!=strKLMemberYN )
			{
				ids = pDM->GetCellData( _T("REC_KEY"), nIndex, strRecKey );
				if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_SECEDE::CopySecederList()") );
				strRecKey.TrimLeft();strRecKey.TrimRight();

				if( !strRecKeyList.IsEmpty() )
				{
					strRecKeyList += _T(", ");
				}

				strRecKeyList += strRecKey;
			}

			if( nMode )
			{
				pGrid->SelectGetNext();
			}
		}	
		
		if( !strRecKeyList.IsEmpty() )
		{
			strWhere = _T("CLU.REC_KEY IN ( ") + strRecKeyList + _T(" )");
		}
	}

	return SecederSearch( strWhere );

EFS_END
	return -1;
}

INT CBO_LOC_SECEDE::SendSecedeMail(CString strUserKey)
{
EFS_BEGIN

	CString strWhere;
	
	strWhere.Format( _T(" REC_KEY = %s "), strUserKey );

	return 0;	
EFS_END
	return -1;
}

HBRUSH CBO_LOC_SECEDE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

INT CBO_LOC_SECEDE::SetSecedeLog(CString strUserKey, CString strUserNo, CString strUserName)
{
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3300_LIST") );

	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( m_pInfo->MODE );
	MobileApi.SetDM( pDM );
	MobileApi.StartLog();

	int ids = 0;

	pDM->InitDBFieldData();
	pDM->StartFrame();
	
	MobileApi.AddDBFieldData(_T("WORKER_IP"), _T("STRING"), m_pInfo->LOCAL_IP, TRUE);
	MobileApi.AddDBFieldData(_T("WORKER_ID"), _T("STRING"), m_pInfo->USER_ID, TRUE);
	MobileApi.AddDBFieldData(_T("WORKER_NAME"), _T("STRING"), m_pInfo->USER_NAME, TRUE);	
	MobileApi.AddDBFieldData(_T("WORKER_SYS"), _T("STRING"), _T("KOLASIII"), TRUE);
	MobileApi.AddDBFieldData(_T("USER_KEY"), _T("NUMERIC"), strUserKey, TRUE);
	MobileApi.AddDBFieldData(_T("USER_NO"), _T("STRING"), strUserNo, TRUE);
	MobileApi.AddDBFieldData(_T("USER_NAME"), _T("STRING"), strUserName, TRUE);				
	MobileApi.AddDBFieldData(_T("WORK_DATE"), _T("NUMERIC"), _T("SYSDATE"), TRUE);
	MobileApi.AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("이용자탈퇴"), __WFILE__, __LINE__ ), TRUE);

	ids = MobileApi.MakeInsertFrame(_T("CO_LOAN_USER_SECEDE_TBL"), _T("REC_KEY"), _T("NUMERIC"), _T("ESL_SEQ.NEXTVAL"));
	if( 0 > ids ) 
		ERROR_MSG_RETURN_ERROR_CODE2(-1150, _T("SetSecedeLog"));

	pDM->SendFrame();
	pDM->EndFrame();

	return 0;

}