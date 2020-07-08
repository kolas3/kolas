// LocMailInfo.cpp : implementation file
//

#include "stdafx.h"
#include "LocMailInfo.h"
#include "MailAccountMgr.h"
// 2011.09.07 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocMailInfo dialog


CLocMailInfo::CLocMailInfo(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	// KOL.UDF.022
	m_sSMAlias = _T("");
	m_sLibName = _T("");
	m_sEcoMailUseYN = _T("");
	m_strTO_EMP_CODE = _T("");
	m_sTitle = _T("");
	m_sBody = _T("");
	m_sEMail = _T("");
	m_sRecipientName = _T("");
	m_sCCName = _T("");
	m_sCCAddress = _T("");
	m_sLoanDMAlias = _T("");
	m_sUserDMAlias = _T("");
	m_sMailModeDMAlias = _T("");

	m_pLoanMailDM = NULL;
	m_pTmpLoanMailDM = NULL;
	m_pUserMailDM = NULL;
	m_pMailModeInfoDM = NULL;
	m_pLoanKeyDM = NULL;
	m_pMailAliasDM = NULL;	
	m_pSendMailMgr = NULL;	
	m_pMailRecord = NULL;

}

CLocMailInfo::~CLocMailInfo()
{
	if( m_pMailRecord != NULL ) delete m_pMailRecord;
	m_pMailRecord = NULL;
	
	if( m_pSendMailMgr != NULL ) delete m_pSendMailMgr;
	m_pSendMailMgr = NULL;
}


VOID CLocMailInfo::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocMailInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CLocMailInfo, CDialog)
	//{{AFX_MSG_MAP(CLocMailInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocMailInfo message handlers


// 메일 기초 정보 구성
// 1. 메일 종류별 기초 정보 구성
INT CLocMailInfo::MakeInitInfo()
{
	EFS_BEGIN

	INT ids;

	ids = m_pMailModeInfoDM->RefreshDataManager(_T(""));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeInitInfo") );

	INT itest = m_pMailModeInfoDM->GetRowCount();
	m_pMailModeInfoDM->SORT( _T("메일종류") );

	/// 도서관 이름 가져오기
	CString sLibInfo[1] =
	{
		_T("LIB_NAME")
	};
	CLocCommonAPI::GetLibInfo( sLibInfo , sLibInfo , 1 , this);
	m_sLibName = sLibInfo[0];
	
	/// 메일링 시스템 사용 도서관인지 알아본다.
	ids = GetManageValue( _T("기타")  , _T("공통") , _T("메일링시스템") , _T("사용여부") , m_sEcoMailUseYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeInitInfo") );

	if( m_sEcoMailUseYN.IsEmpty() ) m_sEcoMailUseYN = _T("N");

	return 0;

	EFS_END
	return -1;

}

// SQL WHERE절을 이용해 메일 레코드 구성
// DM을 DB에서 구성하여 메일을 보낼수 잇도록 정보를 구성한다.
INT CLocMailInfo::MakeMailInfo( INT nMailClass , CString sWhere )
{
	EFS_BEGIN

	INT ids;

	// 메일 정보 DM 구성
	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );

	ids = MakeSql( nMailClass , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeMailInfo") );
	if( ids > 0 ) return ids;

	ids = pLocDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeMailInfo") );

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pLocDM->GetRowCount() && _T("DM_LOC_MAIL_INFO") == pLocDM->GETDMAlias())
	{
		CString strEnc;
		CStringArray asDes1;
		for(INT i = 0; i < pLocDM->GetRowCount(); i++)
		{
			pLocDM->GetCellData(_T("E_MAIL"), i, strEnc);
			asDes1.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pLocDM->GetRowCount(); i++)
				{
					pLocDM->SetCellData(_T("E_MAIL"), asDes1.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	// 청구기호 생성
	// 2007.04.04 update by pdw {{+++++++++++++++++++++++++++++++++++++++
	// 탈퇴회원추가
	if( ( nMailClass != 8 ) && ( nMailClass != 20 ) )
		CLocCommonAPI::MakeShelfCode( this , pLocDM->DB_MGR_ALIAS );
	// ----------------------------------------------------------------}}

	// nMailClass가 0에서 7일 경우 긴급 대출 자료도 더해주어야 한다. + 연속
	// 8일 경우엔 해줄 필요가 없다.
	ids = AddNonDBBookDM( nMailClass , sWhere , pLocDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeMailInfo") );

	ids = MakeMailRecord( nMailClass , pLocDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeMailInfo") );

	return 0;

	EFS_END
	return -1;

}

INT CLocMailInfo::MakeMailInfo( INT nMailClass , CStringArray &saWhere )
{
	EFS_BEGIN
		
	INT ids;
	
	// 메일 정보 DM 구성
	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );

	pLocDM->FreeData();

	CString sWhere;

	INT nWhereCnt = saWhere.GetSize();

	CESL_DataMgr TmpDM;
	ids = CLocCommonAPI::MakeCopyDM( this , pLocDM , &TmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeMailInfo") );

	for( INT i = 0 ; i < nWhereCnt ; i++ )
	{
		sWhere = saWhere.GetAt(i);
		ids = MakeSql( nMailClass , sWhere );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeMailInfo") );

		TmpDM.FreeData();
		ids = TmpDM.RefreshDataManager(sWhere);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeMailInfo") );

		// 2011.09.07 ADD : 3000Api
		if(TRUE == m_pInfo->m_bLibSecuUse && 0 < TmpDM.GetRowCount() && _T("DM_LOC_MAIL_INFO") == pLocDM->GETDMAlias())
		{
			CString strEnc;
			CStringArray asDes1;
			for(INT i = 0; i < TmpDM.GetRowCount(); i++)
			{
				TmpDM.GetCellData(_T("E_MAIL"), i, strEnc);
				asDes1.Add(strEnc);
			}
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
			if(NULL != pApi)
			{
				if(TRUE == pApi->CheckConnection())
				{
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
						pApi->CloseSocket();	
					}
					for(i = 0; i < TmpDM.GetRowCount(); i++)
					{
						TmpDM.SetCellData(_T("E_MAIL"), asDes1.GetAt(i), i);
					}
				}
				delete pApi;
				pApi = NULL;
			}
			asDes1.RemoveAll(); asDes1.FreeExtra();
		}

		ids = CLocCommonAPI::AddDM( &TmpDM , pLocDM , this );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeMailInfo") );		
	}
	
	
	// 청구기호 생성
	if( nMailClass != 8 )
		CLocCommonAPI::MakeShelfCode( this , pLocDM->DB_MGR_ALIAS );
	
	
	// nMailClass가 0에서 7일 경우 긴급 대출 자료도 더해주어야 한다. + 연속
	ids = AddNonDBBookDM( nMailClass , sWhere , pLocDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeMailInfo") );
	
	ids = MakeMailRecord( nMailClass , pLocDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeMailInfo") );
	
	return 0;
	
	EFS_END
		return -1;
	
}

// nMailClass가 0에서 7일 경우 긴급 대출 자료도 더해주어야 한다. + 연속 자료
INT CLocMailInfo::AddNonDBBookDM( INT nMailClass , CString sWhere , CESL_DataMgr *pLocDM )
{
	EFS_BEGIN

	INT ids;

	// 2005.04.08 UPDATE BY PDJ
	if( !( (nMailClass==0) || (nMailClass==1) || (nMailClass==2) || 
		   (nMailClass==3) || (nMailClass==4) || (nMailClass==5) || 
		   (nMailClass==6) || (nMailClass==7) || (nMailClass==9) || (nMailClass==10)) )
	{		
		return 0;
	}

	// 2005.04.08 ADD BY PDJ
	// 5163일 경우만 9가 반납예정이다.
	// 공공은 9가 감사장. 10이 반납예정이다.
	if(m_pSendMailMgr->Check_5163MailSystem() == TRUE)
	{
		if ( nMailClass==10 ) return 0;
	}
	else
	{
		if ( nMailClass==9 ) return 0;
	}
		

	CString sNonDBBookDMAlias = _T("DM_LOC_MAIL_INFO_NON_DB");

	ids = MakeSql( nMailClass , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeLocDMAddTmpDM") );

	// 긴급대출일 경우 book_tbl의 Alias는 BB가 아니라 BL이다.
	// DB에 기본 Condition으로 들어가 있어서 부득이 변경함.
	// 아예 소스레벨로 내려야만 가능하므로 변환을 따로함.
	sWhere.Replace(_T("BB."), _T("NB."));	

	CESL_DataMgr *pNonDM = FindDM( sNonDBBookDMAlias );
	if( pNonDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("AddNonDBBookDM") );

	// 2009.02.25 ADD BY PWR : 현황으로 조인해야하는것은 LS_WORK_T01로 테이블 치환
	if( nMailClass == 0 || nMailClass == 1 || nMailClass == 2 || 
		nMailClass == 3 || nMailClass == 7 || nMailClass == 10 )
	{
		pNonDM->TBL_NAME.Replace(_T("LH_STORAGE_V01"), _T("LS_WORK_T01"));
	}
	else
	{
		pNonDM->TBL_NAME.Replace(_T("LS_WORK_T01"), _T("LH_STORAGE_V01"));
	}

	ids = pNonDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("AddNonDBBookDM") );

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pNonDM->GetRowCount())
	{
		CString strEnc;
		CStringArray asDes1;
		for(INT i = 0; i < pNonDM->GetRowCount(); i++)
		{
			pNonDM->GetCellData(_T("E_MAIL"), i, strEnc);
			asDes1.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pNonDM->GetRowCount(); i++)
				{
					pNonDM->SetCellData(_T("E_MAIL"), asDes1.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	ids = CLocCommonAPI::AddDM( pNonDM , pLocDM , this ); 
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("AddNonDBBookDM") );

	CString sSEDBBookDMAlias = _T("DM_LOC_MAIL_INFO_SE_DB");

	CESL_DataMgr *pSEDM = FindDM( sSEDBBookDMAlias );
	if( pSEDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("AddNonDBBookDM") );

	// 2009.02.25 ADD BY PWR : 현황으로 조인해야하는것은 LS_WORK_T01로 테이블 치환
	if( nMailClass == 0 || nMailClass == 1 || nMailClass == 2 || 
		nMailClass == 3 || nMailClass == 7 || nMailClass == 10 )
	{
		pSEDM->TBL_NAME.Replace(_T("LH_STORAGE_V01"), _T("LS_WORK_T01"));
	}
	else
	{
		pSEDM->TBL_NAME.Replace(_T("LS_WORK_T01"), _T("LH_STORAGE_V01"));
	}

	sWhere.Replace(_T("NB."), _T("BB."));

	ids = pSEDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("AddNonDBBookDM") );

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pSEDM->GetRowCount())
	{
		CString strEnc;
		CStringArray asDes1;
		for(INT i = 0; i < pSEDM->GetRowCount(); i++)
		{
			pSEDM->GetCellData(_T("E_MAIL"), i, strEnc);
			asDes1.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pSEDM->GetRowCount(); i++)
				{
					pSEDM->SetCellData(_T("E_MAIL"), asDes1.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	ids = CLocCommonAPI::AddDM( pSEDM , pLocDM , this ); 
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("AddNonDBBookDM") );

	// 청구기호 생성
	if( nMailClass != 8 )		
		CLocCommonAPI::MakeShelfCode( this , pLocDM->DB_MGR_ALIAS );

	return 0;

	EFS_END
	return -1;

}

// DM으로 메일 레코드를 구성한다.
INT CLocMailInfo::MakeMailRecord( INT nMailClass , CESL_DataMgr *pLocDM )
{
	EFS_BEGIN

	INT ids;

	// 1. 정렬할 값을 가져온다.
	CArray < CString , CString > saOrder;

	ids = GetOrderArray( nMailClass , saOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailRecord") );


	// 2. 정렬을 한다.
	ids = CLocCommonAPI::SortDM( this , pLocDM , saOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeMailRecord") );

	// 3. 메일 레코드의 값을 생성한다.
	// 4. 메일 레코드에 추가한다.
	ids = MakeAndAddMailRecord( nMailClass , pLocDM , saOrder );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeMailRecord") );
	}

	return 0;

	EFS_END
	return -1;

}

// DM으로 메일 레코드 구성하여 추가
INT CLocMailInfo::MakeAndAddMailRecord( INT nMailClass , CESL_DataMgr *pLocDM , CArray < CString , CString > &saOrder )
{
	EFS_BEGIN

	INT ids;

	CString	strCheckMailClass;
	INT	nRowCount;
	INT nCheckMailClass;
	INT	i;
	INT	nRowIndex;

	nRowCount = m_pMailModeInfoDM->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("메일종류") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				nRowIndex = i;
				break;
			}
		}
	}	

	// 2006-11-21 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++
	// 메일을 못찾았다면 아웃~
	if ( i == nRowCount ) return 1;
	// ---------------------------------------------------------------

	if( m_sEcoMailUseYN.Compare(_T("Y")) == 0 )
	{
		INT nMailSendCnt = 0;
		CString sMailClass;
		sMailClass.Format( _T("%d") , nMailClass );

		CString sMailInfoKey;
		CString sMailName;

		ids = m_pMailModeInfoDM->GetCellData( _T("메일정보KEY") , nRowIndex , sMailInfoKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("MakeAndAddMailRecord") );

		ids = m_pMailModeInfoDM->GetCellData( _T("메일이름") , nRowIndex , sMailName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("MakeAndAddMailRecord") );

		CString sTmpResult;
		CString sResult;

		CString sLoanKeyList;
		CString sLoanKey;
		CString sUserKey;
		BOOL bAddRecord = FALSE;
		
		INT nMailCnt = pLocDM->GetRowCount();	

		if( nMailCnt == 0 )
		{
			AfxMessageBox( _T("메일을 구성할 수 없습니다. "));
			return 1010;
		}

		pLocDM->StartFrame();
		for( INT i = 0 ; i < nMailCnt ; i++ )
		{

			if( i != pLocDM->GetRowCount()-1 )
			{
				for( INT k = 0 ; k < saOrder.GetSize() ; k++ )
				{
					ids = pLocDM->GetCellData( saOrder.GetAt(k) , i , sTmpResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeAndAddMailRecord") );

					ids = pLocDM->GetCellData( saOrder.GetAt(k) , i+1 , sResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeAndAddMailRecord") );

					if( sTmpResult.Compare( sResult ) != 0 )
						bAddRecord = TRUE;
				}
			}
			else
			{
				bAddRecord = TRUE;
			}

			if( sMailClass.Compare(_T("8")) != 0 && sMailClass.Compare(_T("20")) != 0 )
			{
				ids = pLocDM->GetCellData( _T("대출/반납정보KEY") , i , sLoanKey );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeAndAddMailRecord") );				

				if( !sLoanKeyList.IsEmpty() ) sLoanKeyList += _T(",");
				sLoanKeyList += sLoanKey;
			}

			if( saOrder.GetSize() == 0 ) bAddRecord = TRUE;


			if( bAddRecord )
			{
				if( sMailClass.Compare(_T("8")) != 0 && sMailClass.Compare(_T("20")) != 0 )
				{
					ids = pLocDM->GetCellData( _T("이용자KEY") , i , sUserKey );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeAndAddMailRecord") );
				}
				else
				{
					ids = pLocDM->GetCellData( _T("사용자KEY") , i , sUserKey );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("MakeAndAddMailRecord") );
				}

				// 2009.02.26 ADD BY CJY : 관리구분 항목 추가
				CString strManageCode;
				if(NULL != m_pInfo && _T("") != m_pInfo->MANAGE_CODE)
				{
					strManageCode = m_pInfo->MANAGE_CODE;
				}
				else
				{
					strManageCode = _T("UDF_MANAGE_CODE");
				}
				/// Temp에 INSERT하기
				pLocDM->InitDBFieldData();
				pLocDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , _T("esl_mail_seq.NextVal") );
				pLocDM->AddDBFieldData( _T("MAIL_KEY") , _T("NUMERIC") , sMailInfoKey );
				pLocDM->AddDBFieldData( _T("LOAN_KEY_LIST") , _T("STRING") , sLoanKeyList );
				pLocDM->AddDBFieldData( _T("USER_KEY") , _T("NUMERIC") , sUserKey );
				pLocDM->AddDBFieldData( _T("LIB_NAME") , _T("STRING") , m_sLibName );
				pLocDM->AddDBFieldData( _T("MAIL_CLASS") , _T("STRING") , sMailClass );

				// 2009.02.26 ADD BY CJY : 관리구분 항목 추가
				pLocDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , strManageCode);
				pLocDM->MakeInsertFrame(_T("MN_MAIL_TEMP_TBL"));

				sUserKey.Empty();
				sLoanKeyList.Empty();
				bAddRecord = FALSE;

				nMailSendCnt++;
			}

		}

		ids = pLocDM->SendFrame();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("MakeAndAddMailRecord") );

		pLocDM->EndFrame();

		CString sMsg;
		sMsg.Format( _T("[%s] %d건이 메일 보내기 대기중인 자료로 구성되었습니다.") , sMailName , nMailSendCnt );
		AfxMessageBox( sMsg );

	}
	else
	{
		CString strMailClass;
		strMailClass.Format( _T("%d") , nMailClass );

		m_pSendMailMgr->DeleteAllMailRecord();	

		// 메일 바디를 구성한다.
		CString sTmpResult;
		CString sResult;

		CString sBody;
		CString sTmpBody;
		BOOL bAddRecord = FALSE;
		
		
		// 김용배 추가
		// 메일 발송건수를 100건으로 제한
		// 초과시 100건으로 목록 구성
		INT mailCnt = pLocDM->GetRowCount();

		/*
		if( mailCnt > POSSIBLE_MAIL_CNT )
		{
			CString msg;
			msg.Format( 
				_T("1회 메일 발송건수가 %d 건으로 제한되어있습니다.\n")
				_T("메일 수신자 목록을 %d 명까지 구성하였습니다."),
				POSSIBLE_MAIL_CNT, POSSIBLE_MAIL_CNT );
			MessageBox( msg );
			mailCnt = POSSIBLE_MAIL_CNT;
		}
		*/
		CString strMailInfoKey;
		CString strMailName;

		ids = m_pMailModeInfoDM->GetCellData( _T("메일정보KEY") , nRowIndex , strMailInfoKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1000 , _T("MakeAndAddMailRecord") );

		ids = m_pMailModeInfoDM->GetCellData( _T("메일이름") , nRowIndex , strMailName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1010 , _T("MakeAndAddMailRecord") );

		CString strLoanKeyList;
		CString strLoanKey;
		CString strUserKey;

		for( INT i = 0 ; i < mailCnt ; i++ )
		{

			if( i != pLocDM->GetRowCount()-1 )
			{
				for( INT k = 0 ; k < saOrder.GetSize() ; k++ )
				{
					ids = pLocDM->GetCellData( saOrder.GetAt(k) , i , sTmpResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeAndAddMailRecord") );

					ids = pLocDM->GetCellData( saOrder.GetAt(k) , i+1 , sResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeAndAddMailRecord") );

					if( sTmpResult.Compare( sResult ) != 0 )
						bAddRecord = TRUE;
				}
			}
			else
			{
				bAddRecord = TRUE;
			}

			if( strMailClass.Compare(_T("8")) != 0 )
			{
				ids = pLocDM->GetCellData( _T("대출/반납정보KEY") , i , strLoanKey );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("MakeAndAddMailRecord") );				

				if( !strLoanKeyList.IsEmpty() ) strLoanKeyList += _T(",");
				strLoanKeyList += strLoanKey;
			}

			ids = m_pMailModeInfoDM->GetCellData( _T("중간글") , nRowIndex , sTmpBody );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("MakeAndAddMailRecord") );

			ids = MakeAliasToRealValue(sTmpBody,pLocDM,i);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeAndAddMailRecord") );

			//2004-12-30 leehakjoong
			//sBody += _T("<br><br>") + sTmpBody;
			sBody += _T("<br>") + sTmpBody;

			if( saOrder.GetSize() == 0 ) bAddRecord = TRUE;

			if( bAddRecord )
			{
				ids = SettingMailInfo( nMailClass , pLocDM , i , sBody );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeAndAddMailRecord") );

				m_pMailRecord->m_sBody = m_sBody;
				m_pMailRecord->m_sRecipientAddress = m_sEMail;	//전자우편
				m_pMailRecord->m_sRecipientName	 = m_sRecipientName;	//신청자이름
				m_pMailRecord->m_sSubject = m_sTitle;
				
				// 2009.03.03 ADD BY CJY : 데몬전송용 추가
				if( strMailClass.Compare(_T("8")) != 0 )
				{
					ids = pLocDM->GetCellData( _T("이용자KEY") , i , strUserKey );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeAndAddMailRecord") );
				}
				else
				{
					ids = pLocDM->GetCellData( _T("사용자KEY") , i , strUserKey );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("MakeAndAddMailRecord") );
				}

				m_pMailRecord->m_sMailInfoKey = strMailInfoKey;
				m_pMailRecord->m_sLoanKeyList = strLoanKeyList;
				m_pMailRecord->m_sUserKey = strUserKey;
				m_pMailRecord->m_sLibName = m_sLibName;
				m_pMailRecord->m_sMailClass = strMailClass;
			
				CString strManageCode;
				if(NULL != m_pInfo && _T("") != m_pInfo->MANAGE_CODE)
				{
					m_pMailRecord->m_sManageCode = m_pInfo->MANAGE_CODE;
				}
				else
				{
					m_pMailRecord->m_sManageCode = _T("UDF_MANAGE_CODE");
				}

				//2004-12-15 
				m_pMailRecord->TO_EMP_CODE = m_strTO_EMP_CODE ;
				m_pSendMailMgr->AddMailRecord(m_pMailRecord);
// 				ids = AddMailRecord();
// 				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeAndAddMailRecord") );

				sBody.Empty();

				bAddRecord = FALSE;
			}

		}
	}


	return 0;

	EFS_END
	return -1;

}

// 레코드에 추가
INT CLocMailInfo::SettingMailInfo( INT nMailClass , CESL_DataMgr *pLocDM , INT nIndex , CString sBody )
{
	EFS_BEGIN

	INT ids;

	CString	strCheckMailClass;
	INT	nRowCount;
	INT nCheckMailClass;
	INT	i;
	INT	nRowIndex;

	nRowCount = m_pMailModeInfoDM->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("메일종류") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				nRowIndex = i;
				break;
			}
		}
	}
	

	// 받는 사람 이름
	ids = pLocDM->GetCellData( _T("이용자명") , nIndex , m_sRecipientName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SettingMailInfo") );

	// 받는 사람 EMAIL
	ids = pLocDM->GetCellData( _T("E_MAIL") , nIndex , m_sEMail );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SettingMailInfo") );



	//2004-12-15 
	m_strTO_EMP_CODE = _T("") ;
	ids = pLocDM->GetCellData( _T("대출자번호") , nIndex , m_strTO_EMP_CODE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SettingMailInfo") );



/*
	// 보내는 사람 이름
	ids = m_pMailModeInfoDM->GetCellData( _T("보내는사람이름") , nMailClass , m_sCCName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SettingMailInfo") );

	// 보내는 사람 EMAIL
	ids = m_pMailModeInfoDM->GetCellData( _T("보내는사람E_MAIL") , nMailClass , m_sCCAddress );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("SettingMailInfo") );
*/	
	// 메일 제목
	ids = m_pMailModeInfoDM->GetCellData( _T("제목") , nRowIndex , m_sTitle );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("SettingMailInfo") );

	ids = MakeAliasToRealValue( m_sTitle , pLocDM , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("SettingMailInfo") );

	// 메일 고정 내용
	CString sHeader,sTail;
	ids = m_pMailModeInfoDM->GetCellData( _T("머릿글") , nRowIndex , sHeader );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("SettingMailInfo") );

	ids = m_pMailModeInfoDM->GetCellData( _T("꼬릿글") , nRowIndex , sTail );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("SettingMailInfo") );

	m_sBody = sHeader + _T("\r\n") + sBody + _T("\r\n") + sTail;

	ids = MakeAliasToRealValue( m_sBody , pLocDM , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("SettingMailInfo") );

	m_sBody.Replace( _T("\r\n") , _T("<br>") );
	m_sBody.Replace( _T("\n") , _T("<br>") );

	return 0;

	EFS_END
	return -1;

}

// 메일 내용 추가
INT CLocMailInfo::AddMailRecord()
{
	EFS_BEGIN

// 2009.03.03 DEL BY CJY : 미사용 함수
// 	///Mail Record 객체 설정
// 	m_pMailRecord->m_sBody = m_sBody;
// 	m_pMailRecord->m_sRecipientAddress = m_sEMail;	//전자우편
// 	m_pMailRecord->m_sRecipientName	 = m_sRecipientName;	//신청자이름
// 	m_pMailRecord->m_sSubject = m_sTitle;
// 
// 
// 	//2004-12-15 
// 	m_pMailRecord->TO_EMP_CODE = m_strTO_EMP_CODE ;
// 
// 
// /*
// 	m_pMailRecord->m_sCCName    = m_sCCName;	// 보내는 사람
// 	m_pMailRecord->m_sCCAddress = m_sCCAddress;			// 보내는 사람 주소	
// */		
// 	// 2005.03.04 UPDATE BY PDJ
// 	// 5163일 경우 TO_EMP_CODE(대출자번호)가 00000~99999 의 범위가 아니라면 
// 	// 메일을 발송하지 않는다.
// 	/*
// 	if(m_pSendMailMgr->Check_5163MailSystem() == TRUE)
// 	{
// 		INT nEmpCode;
// 		nEmpCode = _ttoi(m_pMailRecord->TO_EMP_CODE);
// 		if ( !(nEmpCode >= 0 && nEmpCode <= 99999) ) 
// 		{
// 			return 0;
// 		}
// 	}
// 	*/
// 	///Mail Record 객체 추가
// 	m_pSendMailMgr->AddMailRecord(m_pMailRecord);

	return 0;

	EFS_END
	return -1;

}
// 알리아스를 VALUE로 변경하는 작업
INT CLocMailInfo::MakeAliasToRealValue( CString &sString , CESL_DataMgr *pLocDM , INT nIndex )
{
	EFS_BEGIN

	INT ids;



	CString sAlias;
	CString sValue;
	for( INT i = 0 ; i < m_pMailAliasDM->GetRowCount() ; i++ )
	{
		sValue.Empty();

		ids = m_pMailAliasDM->GetCellData( _T("알리아스") , i , sAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeAndAddMailRecord") );

		ids = pLocDM->GetCellData( sAlias , nIndex , sValue );

		if( sAlias.Compare(_T("도서관명")) == 0 )
//			sValue = sLibInfo[0];
			sValue = m_sLibName;

		sAlias = _T("%") + sAlias + _T("%");
		sString.Replace( sAlias , sValue );
	}

	sString.Replace( _T("''") , _T("'") );

	return 0;

	EFS_END
	return -1;

}


// 메일 종류에 따라 정렬 조건을 가져온다.
INT CLocMailInfo::GetOrderArray( INT nMailClass , CArray < CString , CString > &saOrder )
{
	EFS_BEGIN

	INT ids;

	CString sMailClass;
	CString sOrder;

	for( INT i = 0 ; i < m_pMailModeInfoDM->GetRowCount(); i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("메일종류") , i , sMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetOrderArray") );

		if( _ttoi(sMailClass)==nMailClass ) 
		{
			ids = m_pMailModeInfoDM->GetCellData( _T("통합조건") , i , sOrder ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetOrderArray") );
			sOrder.TrimLeft(); sOrder.TrimRight();

			if( sOrder.IsEmpty() ) return 0;

			GetOrderArray( saOrder , sOrder );

			return 0;
		}
	}

	return 0;

	EFS_END
	return -1;

}

// sOrder을 ','로구분하여 saOrder에 집어 넣는다.
INT CLocMailInfo::GetOrderArray(CArray < CString , CString > &saOrder,CString sOrder)
{
	EFS_BEGIN

	INT nCommaIndex;

	saOrder.RemoveAll();
	
	bool bIsEnd = FALSE;
	while(TRUE)
	{
		nCommaIndex=sOrder.Find(_T(","),0);
		if( nCommaIndex < 0 )
			bIsEnd = TRUE;

		if( nCommaIndex > -1 )
		{
			saOrder.Add( sOrder.Mid(0,nCommaIndex) );
			sOrder = sOrder.Mid( nCommaIndex+1 , sOrder.GetLength()-nCommaIndex);
		}
		else
			saOrder.Add( sOrder );


		if( bIsEnd ) return 0;
	}
	return 0;

	EFS_END
	return -1;

}

// 메일 종류에 따라 사용되는 DM이 다르다.
CESL_DataMgr* CLocMailInfo::FindUseLocDM( INT nMailClass )
{
EFS_BEGIN

	// 2009.02.25 ADD BY PWR : 현황으로 조인해야하는것은 LS_WORK_T01로 테이블 치환
	if( nMailClass == 0 || nMailClass == 1 || nMailClass == 2 || 
		nMailClass == 3 || nMailClass == 7 || nMailClass == 10 )
	{
		m_pLoanMailDM->TBL_NAME.Replace(_T("LH_STORAGE_V01"), _T("LS_WORK_T01"));
	}
	else
	{
		m_pLoanMailDM->TBL_NAME.Replace(_T("LS_WORK_T01"), _T("LH_STORAGE_V01"));
	}

	if( (nMailClass >= 0) && (nMailClass <= 7) || (nMailClass == 9) || (nMailClass == 10))
	{
		
		return m_pLoanMailDM;
	}
	// 2007.04.04 UPDATE BY PDW : 탈퇴안내메일 추가
	else if( (nMailClass == 8) || (nMailClass == 20) )
	{
		return m_pUserMailDM;
	}

	return NULL;

	EFS_END
	return NULL;

}

CESL_DataMgr* CLocMailInfo::FindTempUseLocDM( INT nMailClass )
{
	EFS_BEGIN

	// 2009.02.25 ADD BY PWR : 현황으로 조인해야하는것은 LS_WORK_T01로 테이블 치환
	if( nMailClass == 0 || nMailClass == 1 || nMailClass == 2 || 
		nMailClass == 3 || nMailClass == 7 || nMailClass == 10 )
	{
		m_pTmpLoanMailDM->TBL_NAME.Replace(_T("LH_STORAGE_V01"), _T("LS_WORK_T01"));
	}
	else
	{
		m_pTmpLoanMailDM->TBL_NAME.Replace(_T("LS_WORK_T01"), _T("LH_STORAGE_V01"));
	}

	// 2005.03.08 UPDATE BY PDJ
	// 9번이 반납예정안내로 9까지 돌아야 함.
	if( (nMailClass >= 0) && (nMailClass <= 10) )
	//if( (nMailClass >= 0) && (nMailClass <= 8) )
	{
		return m_pTmpLoanMailDM;
	}
	else if( nMailClass == 8 )
	{
		return NULL;
	}

	return NULL;	

	EFS_END
	return NULL;

}

INT CLocMailInfo::MakeChangeAliasDM()
{
	EFS_BEGIN

	INT ids;

	CString sFilePath = _T("..\\cfg\\MailAlias\\MailAlias.txt");

	CStdioFile fFile;
	ids = fFile.Open( sFilePath , CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary );
	if( ids == 0 )
	{
		AfxMessageBox(_T("알리아스 파일을 여는데 실패했습니다."));
		return 1;
	}
		TCHAR cUni;
		fFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fFile.SeekToBegin();
		}
	
	m_pMailAliasDM->FreeData();

	CString sString;
	CString sAlias;
	CString sDesc;
	INT nIndex;

	while( fFile.ReadString( sString ) )
	{
		nIndex = -1;
		sAlias.Empty(); sDesc.Empty();
		sString.TrimLeft();  sString.TrimRight();
		if( sString.IsEmpty() ) continue;
		
		nIndex = sString.Find(_T("|"));

		if( nIndex < 0 ) 
		{
			sAlias = sString;
		}
		else
		{
			sAlias = sString.Mid( 0 , nIndex );
			sDesc = sString.Mid( nIndex+1 , sString.GetLength() - nIndex );
		}
		sAlias.TrimLeft(); sAlias.TrimRight();
		sDesc.TrimLeft(); sDesc.TrimRight();
		
		ids = m_pMailAliasDM->SetCellData( _T("알리아스") , sAlias , -1  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeChangeAliasDM") );
	}

	fFile.Close();

	return 0;

	EFS_END
	return -1;

}


INT CLocMailInfo::SendMail( INT nMailClass , bool bShowWindow )
{
	EFS_BEGIN

	INT ids;

	if( IsUseMailClass( nMailClass ) != 0 )
		return 1;

	/// ECO Mailing System이라면 메일을 보내지 않는다.
	if( m_sEcoMailUseYN.Compare(_T("Y")) == 0 ) 
		return 2;

	// 2006.12.21 ADD BY PDJ
	// 업무별 담당자메일을 세팅한다.
	// 해당사항이 없을 경우 기존의 공통 설정을 유지
	CString strMailType;
	CString strMailServer;
	CString strAccountYN;
	CString strAccount;
	CString strID;
	CString strPassword;
	CString strSenderName;
	CString strMailAddress;

	strMailType = GetMailType(nMailClass);
	if ( strMailType != _T("") )
	{		
		CMailAccountMgr MailAccountMgr;
		MailAccountMgr.GetMailAccount(strMailType, strAccount, strID, strPassword, strSenderName, strMailAddress);
		MailAccountMgr.GetMailServer(strMailServer, strAccountYN);		
	}

	// 2007.09.04 update by pdw +++++++++++++++++++++++++++++++++++++++++++++++++++++++	
	// bShowWindow 값에 따른 POPUP으로 변경 
	/*
	if ( strMailAddress != _T("") )
	{
		m_pSendMailMgr->DoModal(MAIL_SERVER_USER, strMailServer, strMailAddress, strAccountYN, strID, strPassword);
	}
	else if( bShowWindow )
	{
		m_pSendMailMgr->DoModal(MAIL_SERVER_CFGFILE);
	}
	else
	{
		m_pSendMailMgr->SetMailServerInfoFromCfgFile(TRUE);
		m_pSendMailMgr->SendMail(FALSE,TRUE);
	}
	*/
	if( bShowWindow )
	{
		if( strMailAddress.IsEmpty() )
		{
			m_pSendMailMgr->DoModal(MAIL_SERVER_CFGFILE);
		}
		else
		{
			m_pSendMailMgr->DoModal(MAIL_SERVER_USER, strMailServer, strMailAddress, strAccountYN, strID, strPassword);
		}
	}
	else
	{
		if( strMailAddress.IsEmpty() )
		{
			m_pSendMailMgr->SetMailServerInfoFromCfgFile(TRUE);
			m_pSendMailMgr->SendMail(FALSE,TRUE);
		}
		else
		{
			m_pSendMailMgr->SetDefaultMailServer( strMailServer, strMailAddress, strAccountYN, strID, strPassword );
			m_pSendMailMgr->SendMail(FALSE,TRUE);
		}
	}
	// --------------------------------------------------------------------------------

	ids = MakeSuccessYNList( nMailClass );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SendMail") );

	ids = InsertDBSuccessYN( nMailClass );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SendMail") );

	return 0;

	EFS_END
	return -1;

}

// 메일이 발송 성공된 자료를 INSERT한다.
INT CLocMailInfo::InsertDBSuccessYN( INT nMailClass )
{
	EFS_BEGIN

	INT ids;

	CString sAlias;
	CString sLoanKey;
	CString sMailClass;
	CString sYN;
	CString sTableName = _T("CO_LOAN_MAIL_SEND_TBL");
	sMailClass.Format( _T("%d") , nMailClass );

	if( nMailClass == 8 ) 
	{
		sAlias = _T("사용자KEY");
	}
	else if( nMailClass > -1 && (nMailClass < 10))
	{
		sAlias = _T("대출/반납정보KEY");
	}
	else
		return 0;

	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertDBSuccessYN") );

	CString sRecKey;
	for( INT i = 0 ; i < pLocDM->GetRowCount() ; i++ )
	{
		sRecKey.Empty();
		ids = pLocDM->GetCellData( _T("성공여부") , i , sYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertDBSuccessYN") );

		if( sYN.Compare(_T("Y")) != 0 ) continue;

		// DB에 INSERT
		// REC_KEY 가져오기
		pLocDM->StartFrame();
		pLocDM->InitDBFieldData();

		ids = pLocDM->MakeRecKey(sRecKey);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertDBSuccessYN") );

		ids = pLocDM->GetCellData( sAlias , i , sLoanKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertDBSuccessYN") );

		pLocDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pLocDM->AddDBFieldData( _T("LOAN_KEY") , _T("NUMERIC") , sLoanKey );
		pLocDM->AddDBFieldData( _T("MAIL_CLASS") , _T("STRING") , sMailClass );
		pLocDM->AddDBFieldData( _T("MAIL_SEND_DATE") , _T("NUMERIC") , _T("SYSDATE") );

		pLocDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , _T("UDF_MANAGE_CODE") );

		pLocDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("메일보내기"), __WFILE__, __LINE__ ), TRUE );
		pLocDM->MakeInsertFrame( sTableName );
		
		ids = pLocDM->SendFrame();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertDBSuccessYN") );
		
		// 2006-03-27 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 세션이 줄지않는다.
		ids = pLocDM->EndFrame();
		// -----------------------------------------------------------------------------------
	}

	return 0;

	EFS_END
	return -1;

}

// 메일의 발송 성공 여부를 DM에 기록한다.
INT CLocMailInfo::MakeSuccessYNList( INT nMailClass )
{
	EFS_BEGIN


	INT nMailIndex = 0;
	INT ids;
	bool bNextMailRecord = FALSE;
	INT nFailCode;
	INT	i;

	CString sResult;
	CString sTmpResult;
	CString sYN;
	CString sEMail;
	CString sFailReason;
	CString	strLoanNum;

	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSuccessYNList") );

	CArray < CString , CString > saOrder;
	ids = GetOrderArray( nMailClass , saOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSuccessYNList") );

	/*
	// 2005.03.08 ADD BY PDJ
	// 5163일 경우 대출자번호가 00000~99999인경우 
	// 메일 발송 대상에서 제외되므로 체크하지 않는다.
	INT	nRowCount;
	nRowCount = pLocDM->GetRowCount();
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		ids = pLocDM->GetCellData( _T("대출자번호") , nRowCount-1- i , strLoanNum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeAndAddMailRecord") );
		if ( !(_ttoi(strLoanNum) >= 0 && _ttoi(strLoanNum) <= 99999) )
		{
			ids = pLocDM->DeleteRow(nRowCount-1- i);
		}
	}
	*/


	for( i = 0 ; i < pLocDM->GetRowCount() ; i++ )
	{

		if( i != pLocDM->GetRowCount()-1 )
		{
			for( INT k = 0 ; k < saOrder.GetSize() ; k++ )
			{
				ids = pLocDM->GetCellData( saOrder.GetAt(k) , i , sTmpResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeAndAddMailRecord") );

				ids = pLocDM->GetCellData( saOrder.GetAt(k) , i+1 , sResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeAndAddMailRecord") );

				if( sTmpResult.Compare( sResult ) != 0 )
					bNextMailRecord = TRUE;
			}
		}

		nFailCode = m_pSendMailMgr->GetMailRecordSendState( nMailIndex );
		if( nFailCode==0 ) sYN = _T("Y");
		else
		{
			if( nFailCode == -3 ) sFailReason = _T("메일 서버에 접속이 실패하였습니다.");
			if( nFailCode == -2 ) sFailReason = _T("E_MAIL 주소가 등록되어 있지 않은 이용자입니다.");
			if( nFailCode == 1 ) sFailReason = _T("메일을 발송하지 않았습니다.");
			if( nFailCode == -1 ) sFailReason = _T("알수 없는 오류입니다.메일 서버 주소를 확인해주십시오.");

			sYN = _T("N");
		}

		if( saOrder.GetSize() == 0 ) bNextMailRecord = TRUE;

		ids = pLocDM->SetCellData( _T("성공여부") ,sYN ,i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeSuccessYNList") );
		ids = pLocDM->SetCellData( _T("실패사유") ,sFailReason ,i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeSuccessYNList") );


		if( bNextMailRecord )
		{
			nMailIndex++;
			bNextMailRecord = FALSE;
		}
	}
	
	return 0;

	EFS_END
	return -1;

}

INT CLocMailInfo::SetInitValue()
{
	EFS_BEGIN

	m_sSMAlias = _T("SM_LOC_MAIL_INFO");
	m_sLoanDMAlias = _T("DM_LOC_MAIL_INFO");
	m_sUserDMAlias = _T("DM_USER_MAIL_INFO");
	m_sMailModeDMAlias = _T("DM_LOC_MAIL_MODE_INFO");

	INT ids;
	ids = InitESL_Mgr(m_sSMAlias);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CLocMailInfo") );

	m_pLoanMailDM = FindDM( m_sLoanDMAlias );
	if( m_pLoanMailDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CLocMailInfo") );

	m_pUserMailDM = FindDM( m_sUserDMAlias );
	if( m_pUserMailDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CLocMailInfo") );

	m_pMailModeInfoDM = FindDM( m_sMailModeDMAlias );
	if( m_pMailModeInfoDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CLocMailInfo") );

	m_pMailAliasDM = FindDM( _T("DM_CHANGE_ALIAS") );
	if( m_pMailAliasDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CLocMailInfo") );

	m_pTmpLoanMailDM = FindDM( _T("DM_TEMP_LOC_MAIL_INFO") );
	if( m_pTmpLoanMailDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CLocMailInfo") );

	ids = MakeInitInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CLocMailInfo") );

	ids = MakeChangeAliasDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CLocMailInfo") );

	// 메일 정보
	m_pMailRecord = new SendMailRecord;

	m_pMailRecord->m_nFileAttachCnt = 0;
	m_pMailRecord->m_pFileAttach = _T("");
	m_pMailRecord->m_sEncoding = _T("text/html");

	m_pSendMailMgr = new CSendMailMgr(this);


	return 0;

	EFS_END
	return -1;

}


INT CLocMailInfo::MakeSql( INT nMailClass , CString &sWhere )
{
	EFS_BEGIN

	INT ids;

	CString sDefaultWhere;

	CString	strCheckMailClass;
	INT	nRowCount;
	INT nCheckMailClass;
	INT	i;
	INT	nRowIndex;

	nRowIndex = 0;
	nRowCount = m_pMailModeInfoDM->GetRowCount();

	// 2007.09.03 update by pdw ++++++++++++++++++++++++++++++++++++++++++
	// 메일의 설정에 따라 사용여부가 안되어 있을수 있다.
	// 메일을 찾았는지 확인한다.
	BOOL bFind = FALSE;

	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("메일종류") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				nRowIndex = i;
				bFind  = TRUE;
				break;
			}
		}
	}	

	if( !bFind )
		return 100;
	// ------------------------------------------------------------------

	ids = m_pMailModeInfoDM->GetCellData( _T("기본SQL") , nRowIndex , sDefaultWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSql") );
	sDefaultWhere.TrimLeft(); sDefaultWhere.TrimRight();

	// 2005.01.25 ADD BY SSM
	// 대출일자 타입 변경
	sDefaultWhere.Replace(_T("CL.RESERVATION_DATE"), _T("to_char(CL.RESERVATION_DATE, 'YYYY/MM/DD')"));
	sDefaultWhere.Replace(_T("CL_TEMP.RESERVATION_DATE"), _T("to_char(CL_TEMP.RESERVATION_DATE, 'YYYY/MM/DD')"));
	// SSM C++

	sDefaultWhere.Replace( _T("''") , _T("'") );
	sDefaultWhere.Replace( _T("''") , _T("'") );

	if( sDefaultWhere.IsEmpty() ) return 0;

	if( sWhere.GetLength() > 1 )
		sWhere += _T(" AND ");

	sWhere += sDefaultWhere;

	return 0;

	EFS_END
	return -1;

}

INT CLocMailInfo::GetMailCnt()
{
	EFS_BEGIN

	return m_pSendMailMgr->GetCountMailRecord();

	EFS_END
	return -1;

}

// 일괄 보내기 기능을 사용할 수 있을지 검사한다.
// 하나라도 사용할 수 있다면 사용가능이다.
// return 0 : 사용가능
// return 1 : 사용불가
INT CLocMailInfo::IsUseBatchSendMail()
{
	EFS_BEGIN

	CString	strCheckMailClass;
	INT ids;

	CString sData;
	for( INT i = 0 ; i < m_pMailModeInfoDM->GetRowCount() ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("메일종류") , i , strCheckMailClass );
		ids = IsUseBatchSendMail(_ttoi(strCheckMailClass));
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsUseBatchSendMail") );
		if( ids == 0 ) return 0;
	}

	return 1;

	EFS_END
	return -1;

}

// 일괄 보내기 기능을 사용할 수 잇는지 검사한다.
// return 0 : 사용 가능
// return 1 : 사용 불가
INT CLocMailInfo::IsUseBatchSendMail( INT nMailClass )
{
	EFS_BEGIN

	CString sData;
	CString	strCheckMailClass;
	INT ids;	
	INT nCheckMailClass;
	INT	i;
	INT	nRowCount;
	
	nRowCount = m_pMailModeInfoDM->GetRowCount();

	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("메일종류") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsUseBatchSendMail") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				ids = m_pMailModeInfoDM->GetCellData( _T("일괄자동보내기기능사용여부") , i , sData );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsUseBatchSendMail") );
				sData.TrimRight(); sData.TrimLeft();
				if( sData.Compare(_T("Y")) == 0 ) 
				{
					return 0;
				}
			}
		}
	}	

	return 1;

	EFS_END
	return -1;

}

// 일괄 보내기 기능의 메일 정보 구성
INT CLocMailInfo::BatchMakeMailInfo( INT nMailClass )
{
	EFS_BEGIN

	INT ids;

	CString sWhere;

	ids = MakeBatchWhereSql( nMailClass , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("BatchMakeMailInfo") );

	// 메일 정보 DM 구성
	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );

	// 단행 자료
	ids = pLocDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeMailInfo") );

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pLocDM->GetRowCount() && _T("DM_LOC_MAIL_INFO") == pLocDM->GETDMAlias())
	{
		CString strEnc;
		CStringArray asDes1;
		for(INT i = 0; i < pLocDM->GetRowCount(); i++)
		{
			pLocDM->GetCellData(_T("E_MAIL"), i, strEnc);
			asDes1.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pLocDM->GetRowCount(); i++)
				{
					pLocDM->SetCellData(_T("E_MAIL"), asDes1.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	// 청구기호 생성
	if( nMailClass != 8 )		
		CLocCommonAPI::MakeShelfCode( this , pLocDM->DB_MGR_ALIAS );	

	// 긴급 대출 자료 + 연속 자료더하기
	ids = AddNonDBBookDM( nMailClass , sWhere , pLocDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeMailInfo") );


	ids = MakeMailRecord( nMailClass , pLocDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeMailInfo") );

	return 0;

	EFS_END
	return -1;

}

// 일괄 보내기 기능 사용
INT CLocMailInfo::MakeBatchWhereSql( INT nMailClass , CString &sWhere )
{
	EFS_BEGIN

	INT ids;


	CString sBatchValue;
	CString sBatchWhere;
	CString sReSendYN;

	// 기본 WHERE SQL문 가져오기
	ids = MakeSql( nMailClass , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeBatchWhereSql") );

	CString	strCheckMailClass;
	INT	nRowCount;
	INT nCheckMailClass;
	INT	i;
	INT	nRowIndex;

	nRowCount = m_pMailModeInfoDM->GetRowCount();

	nRowIndex = 0;
	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("메일종류") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				nRowIndex = i;
				break;
			}
		}
	}	
	
	ids = m_pMailModeInfoDM->GetCellData( _T("조건값") , nRowIndex , sBatchValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeBatchWhereSql") );

	ids = m_pMailModeInfoDM->GetCellData( _T("재발송여부") , nRowIndex , sReSendYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeBatchWhereSql") );	

	CString sLibInfo[1] =
	{
		_T("MANAGE_CODE")
	};

	CString	m_sLibName;
	CLocCommonAPI::GetLibInfo( sLibInfo , sLibInfo , 1 , this);
	m_sLibName = sLibInfo[0];

	switch( nMailClass )
	{
	
	case 0:
		//sBatchWhere.Format( _T(" (SYSDATE-CL.RESERVATION_DATE)>%s ") , sBatchValue );
		sBatchWhere.Format( _T(" CL.RESERVATION_DATE < TO_CHAR( SYSDATE - (%d+1) , 'YYYYMMDD' ) ") , _ttoi(sBatchValue) );
		break;

	case 1:
		//sBatchWhere.Format( _T(" (SYSDATE-CL.RESERVATION_DATE)>%s ") , sBatchValue );
		sBatchWhere.Format( _T(" CL.RESERVATION_DATE < TO_CHAR( SYSDATE - (%d+1) , 'YYYYMMDD' ) ") , _ttoi(sBatchValue) );
		break;

	case 2:
		//sBatchWhere.Format( _T(" (SYSDATE-CL.RETURN_PLAN_DATE)>%s ") , sBatchValue );
		// 2005.02.28 UPDATE BY PDJ
		// 조건 잘못됨
		sBatchWhere.Format( _T(" CL.RETURN_PLAN_DATE < TO_CHAR( SYSDATE - (%d-1) , 'YYYYMMDD' ) ") , _ttoi(sBatchValue) );
		//sBatchWhere.Format( _T(" CL.RETURN_PLAN_DATE > TO_CHAR( SYSDATE + %d , 'YYYYMMDD' ) ") , _ttoi(sBatchValue) );
		break;

	case 3:
		//sBatchWhere.Format( _T(" (SYSDATE-CL.LOAN_DATE)<%s ") , sBatchValue );
		sBatchWhere.Format( _T(" CL.LOAN_DATE BETWEEN TO_CHAR( SYSDATE - %d, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE , 'YYYYMMDD') ") , _ttoi(sBatchValue) );
		break;

	case 4:
		//sBatchWhere.Format( _T(" (SYSDATE-CL.RETURN_DATE)<%s ") , sBatchValue );
		//sBatchWhere.Format( _T(" CL.RETURN_DATE BETWEEN TO_CHAR( SYSDATE, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE + %d, 'YYYYMMDD') ") , _ttoi(sBatchValue)-1 );
		sBatchWhere.Format( _T(" CL.RETURN_DATE BETWEEN TO_CHAR( SYSDATE - %d, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE, 'YYYYMMDD') ") , _ttoi(sBatchValue) );
		break;

	case 5:
		//sBatchWhere.Format( _T(" (SYSDATE-CL.RETURN_DATE)<%s ") , sBatchValue );
		//sBatchWhere.Format( _T(" CL.RETURN_DATE BETWEEN TO_CHAR( SYSDATE, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE + %d, 'YYYYMMDD') ") , _ttoi(sBatchValue)-1 );
		sBatchWhere.Format( _T(" CL.RETURN_DATE BETWEEN TO_CHAR( SYSDATE - %d, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE, 'YYYYMMDD') ") , _ttoi(sBatchValue) );
		break;

	case 6:
		//sBatchWhere.Format( _T(" (SYSDATE-CL.RESERVE_CANCEL_DATE)<%s ") , sBatchValue );
		//sBatchWhere.Format( _T(" CL.RESERVE_CANCEL_DATE BETWEEN TO_CHAR( SYSDATE, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE + %d, 'YYYYMMDD') ") , _ttoi(sBatchValue)-1 );
		sBatchWhere.Format( _T(" CL.RESERVE_CANCEL_DATE BETWEEN TO_CHAR( SYSDATE-%d, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE, 'YYYYMMDD') ") , _ttoi(sBatchValue) );
		break;

	case 7:
		//sBatchWhere.Format( _T(" (SYSDATE-CL.DELAY_RETURN_DATE)<%s ") , sBatchValue );
		//sBatchWhere.Format( _T(" CL.DELAY_RETURN_DATE BETWEEN TO_CHAR( SYSDATE, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE + %d, 'YYYYMMDD') ") , _ttoi(sBatchValue)-1 );
		sBatchWhere.Format( _T(" CL.DELAY_RETURN_DATE BETWEEN TO_CHAR( SYSDATE-%d, 'YYYYMMDD' ) AND TO_CHAR (SYSDATE, 'YYYYMMDD') ") , _ttoi(sBatchValue) );
		break;

	case 9:
		{
			if ( m_sLibName.IsEmpty() )
			{
				sBatchWhere.Format( _T(" CL.RETURN_PLAN_DATE BETWEEN TO_CHAR( SYSDATE, 'YYYYMMDD' ) AND ECO$F_G_ISHOLIDAY$I(TO_CHAR (SYSDATE + %d, 'YYYYMMDD')) ") , _ttoi(sBatchValue) );
			}
			else
			{
				//반납예정 추가
				//sBatchWhere.Format( _T(" (CL.RETURN_PLAN_DATE-SYSDATE)<=%s ") , sBatchValue );		
				sBatchWhere.Format( _T(" CL.RETURN_PLAN_DATE BETWEEN TO_CHAR( SYSDATE, 'YYYYMMDD' ) AND ECO$F_G_ISHOLIDAY$I(TO_CHAR (SYSDATE + %d, 'YYYYMMDD'),'%s') ") , _ttoi(sBatchValue), m_sLibName );
			}		
		}	
		break;
	default:
		break;

	}

	if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

	sWhere += _T("(") + sBatchWhere + _T(")") ;

	if( sReSendYN.Compare(_T("N")) == 0 )
	{
		CString sBatchWhere2;
		sBatchWhere2.Format(_T(" AND ((SELECT COUNT(*) FROM CO_LOAN_MAIL_SEND_TBL WHERE MAIL_CLASS = '%d' AND LOAN_KEY = CL.REC_KEY )=0)") 
			, nMailClass);
		sWhere += sBatchWhere2;
	}

	// 2005.03.22 ADD BY PDJ
	// 5163일 경우 일괄보내기시 대출자 번호가 00000~99999인 자료만 보내도록 한다.
	if(m_pSendMailMgr->Check_5163MailSystem() == TRUE)
	{
		sWhere += _T(" AND CLU.USER_NO BETWEEN '00000' AND '99999' AND LENGTH(CLU.USER_NO)=5 ");
	}	
	// 2007.08.08 add by pdw
	// 일반도서관일 경우 e-mail이 있는곳만 발송한다.
	else
	{
		sWhere += _T(" AND CLU.E_MAIL IS NOT NULL ");
	}
	
	// 오늘 보냈던 자료는 보내지 않는다.
	// 2007.08.09 update by pdw
	// 재발송여부가 Y일경우만 조건추가
	if( sReSendYN.Compare(_T("Y")) == 0 )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

		CString sBatchWhere3;
		sBatchWhere3.Format( _T(" ((SELECT COUNT(*) FROM CO_LOAN_MAIL_SEND_TBL ")
								_T("WHERE MAIL_CLASS='%d' AND LOAN_KEY=CL.REC_KEY AND to_char(MAIL_SEND_DATE,'YYYY/MM/DD')=to_char(SYSDATE,'YYYY/MM/DD'))=0) ") , nMailClass );

		sWhere += sBatchWhere3;
	}

	return 0;
	

	EFS_END
	return -1;

}

// 메일 종류별 이름 가져오기
CString CLocMailInfo::GetMailName( INT nMailClass )
{
	EFS_BEGIN

	CString sMailName;	
	CString	strCheckMailClass;
	INT ids;	
	INT nCheckMailClass;
	INT	i;
	INT	nRowCount;

	sMailName = _T("");
	nRowCount = m_pMailModeInfoDM->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("메일종류") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				ids = m_pMailModeInfoDM->GetCellData( _T("메일이름") , i , sMailName );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );
				break;
			}
		}
	}	

	return sMailName;

	EFS_END
	return _T("");

}

// 메일을 보내고 난뒤
// 성공한 DM안의 레코드의 갯수를 세주는 함수
INT CLocMailInfo::GetLogDMCnt( INT nMailClass , INT &nDMRCnt , INT &nSuccessDMRCnt , INT &nFailDMRCnt)
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetLogDMCnt") );

	nDMRCnt = pLocDM->GetRowCount();

	nSuccessDMRCnt = 0;
	nFailDMRCnt = 0;

	CString sYN;
	
	// 2005.03.08 ADD BY PDJ 
	// 5163이면 메일카운트를 셀때 Groupping한 건수로 체크
	CString	strLoanNo;
	CString	strPrevLoanNo;
	BOOL	b5163Flag;

	b5163Flag = m_pSendMailMgr->Check_5163MailSystem();	
	strLoanNo.Empty();
	strPrevLoanNo.Empty();

	for( INT i = 0 ; i < nDMRCnt ; i++ )
	{
		// 2005.03.08 ADD BY PDJ
		ids = pLocDM->GetCellData( _T("대출자번호") , i , strLoanNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetLogDMCnt") );
		if ( b5163Flag) 
		{
			if ( strLoanNo == strPrevLoanNo ) continue;
		}

		ids = pLocDM->GetCellData( _T("성공여부") , i , sYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetLogDMCnt") );
		sYN.TrimLeft(); sYN.TrimRight();

		if( sYN.Compare(_T("Y"))==0 )
			nSuccessDMRCnt++;
		else
			nFailDMRCnt++;

		// 2005.03.08 ADD BY PDJ
		strPrevLoanNo = strLoanNo;
	}

	// 2005.03.08 ADD BY PDJ
	// 전체 카운트는 성공+실패
	if ( b5163Flag) 
	{
		nDMRCnt = nSuccessDMRCnt+nFailDMRCnt;
	}

	return 0;

	EFS_END
	return -1;

}

// 실패 로그를 pDstDM에 기록
INT CLocMailInfo::AddFailLog( INT nMailClass , CESL_DataMgr *pDstDM )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetLogDMCnt") );

	const INT nAliasCnt = 4;
	TCHAR *sDMFieldAlias[nAliasCnt] =
	{
		_T("이용자명") , _T("서명") , _T("등록번호") , _T("실패사유") 
	};

	CString sYN;
	CString sData;
	for( INT i = 0 ; i < pLocDM->GetRowCount() ; i++ )
	{
		ids = pLocDM->GetCellData( _T("성공여부") , i , sYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("AddFailLog") );
		sYN.TrimLeft(); sYN.TrimRight();

		if( sYN.Compare(_T("Y"))==0 ) continue;

		pDstDM->InsertRow(-1);

		for( INT j = 0 ; j < nAliasCnt ; j++ )
		{
			ids = pLocDM->GetCellData( sDMFieldAlias[j] , i , sData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("AddFailLog") );
			sData.TrimLeft(); sData.TrimRight();

			ids = pDstDM->SetCellData( sDMFieldAlias[j] , sData , pDstDM->GetRowCount()-1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("AddFailLog") );
		}

		ids = pDstDM->SetCellData( _T("메일종류") , GetMailName(nMailClass) , pDstDM->GetRowCount()-1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("AddFailLog") ); 
	}

	return 0;

	EFS_END
	return -1;

}



INT CLocMailInfo::GetMailClassFromName( CString sMailName )
{
	EFS_BEGIN

	INT ids;

	CString sTmpMailName;
	CString strMailClass;

	for( INT i = 0 ; i < m_pMailModeInfoDM->GetRowCount() ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("메일이름") , i , sTmpMailName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailClassFromName") );

		if( sMailName.Compare( sTmpMailName ) == 0 )
		{
			ids = m_pMailModeInfoDM->GetCellData( _T("메일종류") , i , strMailClass );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailClassFromName") );
			return _ttoi(strMailClass);
		}
	}

	return 100;

	EFS_END
	return -1;

}

INT CLocMailInfo::MakeMailInfo( INT nMailClass , CESL_DataMgr *pSrcDM , CESL_Grid *pGrid )
{
	EFS_BEGIN

	INT ids;

	// 메일 정보 DM 구성
	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );

	pLocDM->FreeData();

	ids = MakeLocDM( nMailClass , pSrcDM , pGrid );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );
	if( ids > 0 ) return ids;

	ids = MakeMailRecord( nMailClass , pLocDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeMailInfo") );

	return 0;

	EFS_END
	return -1;

}

INT CLocMailInfo::MakeLocDM( INT nMailClass , CESL_DataMgr *pSrcDM , CESL_Grid *pGrid )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeMailInfo") );

	pGrid->SelectMakeList();

	INT nIndex;
	INT nSelectGetCount = pGrid->SelectGetCount();
	pGrid->SelectGetHeadPosition();

	CString sRecKeyList;
	CString sRecKey;
	CString sWhere;
	CString sMailingUseYN;
	for( INT i = 0 ; i < nSelectGetCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		ids = pSrcDM->GetCellData( _T("메일수신여부") , nIndex , sMailingUseYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );

		if ( sMailingUseYN == _T("N") )
		{
			pGrid->SelectGetNext();
			continue;
		}
		
		ids = pSrcDM->GetCellData( _T("대출/반납정보KEY") , nIndex , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );

		if( i == 0 || ( i > 300 && ((i)%300)==1 ) ) 
			sRecKeyList = sRecKey ;
		else 
		{
			if( sRecKeyList.IsEmpty() )
			{
				sRecKeyList = sRecKey ;
			}
			else
			{
				sRecKeyList += _T(",") + sRecKey;
			}
		}

		// 2007.04.11 update by pdw {{+++++++++++++++++++++++++++++++++++++++++++++
		// 이용자의 마지막이 메일링 사용안함이라면 이건 처리되지 않기 때문에 변경		
		/*
		if( ((i!=0)&&(i%300==0)) || (i==nSelectGetCount-1) )
		{
			sWhere.Format( _T(" CL.REC_KEY IN ( %s ) ") , sRecKeyList );

			// TMP DM을 구성한 후 LCO DM에 ADD한다.
			ids = MakeLocDMAddTmpDM(nMailClass , sWhere);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeMailInfo") );

			ids = AddNonDBBookDM( nMailClass , sWhere , pLocDM );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeMailInfo") );			
		}
		*/
		if( (i!=0) && (i%300==0) )
		{
			sWhere.Format( _T(" CL.REC_KEY IN ( %s ) ") , sRecKeyList );

			// TMP DM을 구성한 후 LCO DM에 ADD한다.
			ids = MakeLocDMAddTmpDM(nMailClass , sWhere);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeMailInfo") );

			ids = AddNonDBBookDM( nMailClass , sWhere , pLocDM );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeMailInfo") );	
			
			sRecKeyList.Empty();
		}		
		// ----------------------------------------------------------------------}}

		pGrid->SelectGetNext();
	}

	if( !sRecKeyList.IsEmpty() )
	{
		sWhere.Format( _T(" CL.REC_KEY IN ( %s ) ") , sRecKeyList );

		// TMP DM을 구성한 후 LCO DM에 ADD한다.
		ids = MakeLocDMAddTmpDM(nMailClass , sWhere);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeMailInfo") );

		ids = AddNonDBBookDM( nMailClass , sWhere , pLocDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeMailInfo") );			
	}
	
	if( pLocDM->GetRowCount() == 0 )
	{
		AfxMessageBox( _T("선택하신 자료중에 메일을 보낼수 있는 자료가 존재하지 않습니다.") );
		return 1;
	}

	return 0;

	EFS_END
	return -1;

}

INT CLocMailInfo::MakeLocDMAddTmpDM( INT nMailClass , CString sWhere )
{
	EFS_BEGIN

	INT ids;

	ids = MakeSql( nMailClass , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeLocDMAddTmpDM") );

	CESL_DataMgr *pTmpLocDM = FindTempUseLocDM( nMailClass );
	if( pTmpLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeLocDMAddTmpDM") );

	ids = pTmpLocDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeLocDMAddTmpDM") );

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pTmpLocDM->GetRowCount())
	{
		CString strEnc;
		CStringArray asDes1;
		for(INT i = 0; i < pTmpLocDM->GetRowCount(); i++)
		{
			pTmpLocDM->GetCellData(_T("E_MAIL"), i, strEnc);
			asDes1.Add(strEnc);
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();	
				}
				for(i = 0; i < pTmpLocDM->GetRowCount(); i++)
				{
					pTmpLocDM->SetCellData(_T("E_MAIL"), asDes1.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );

	ids = CLocCommonAPI::AddDM( pTmpLocDM , pLocDM , this );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeLocDMAddTmpDM") );

	// 청구기호 생성
	if( nMailClass != 8 )		
		CLocCommonAPI::MakeShelfCode( this , pLocDM->DB_MGR_ALIAS );

	return 0;

	EFS_END
	return -1;

}

INT CLocMailInfo::IsUseMailClass( INT nMailClass )
{
	EFS_BEGIN

	INT ids;

	CString	strCheckMailClass;
	INT	nRowCount;
	INT nCheckMailClass;
	INT	i;
	INT	nRowIndex;

	nRowIndex = -1;
	nRowCount = m_pMailModeInfoDM->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("메일종류") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				nRowIndex = i;
				break;
			}
		}
	}

	if ( nRowIndex == -1 )
	{
		return 1;
	}
	else
	{
		CString sYN;
		ids = m_pMailModeInfoDM->GetCellData( _T("기능사용여부") , nRowIndex , sYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsUseMailClass") );

		if( sYN.Compare( _T("N") )== 0 )
			return 1;
	}

	return 0;

	EFS_END
	return -1;

}

INT CLocMailInfo::DMAndGridSetting( INT nMailClass , CESL_DataMgr *pDM , CESL_Grid *pGrid )
{
	EFS_BEGIN

	INT ids;

	if( nMailClass > 1 ) return 0;

	CString sLoanKey;

	INT nSelectCnt = pGrid->SelectGetCount();

	pGrid->SelectGetHeadPosition();

	CString sToday;
	
	// 2010.08.27 UPDATE BY PWR : 로그인시 기록한 현재일자 사용
	CString strGetData;
	sToday = (CLocCommonAPI::GetCurrentDateTime(this,strGetData)).Format( _T("%Y/%m/%d") );
// 	sToday = (CLocCommonAPI::GetDBTime(this)).Format( _T("%Y/%m/%d") );

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		ids = pDM->GetCellData( _T("대출/반납정보KEY") , nIndex , sLoanKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DMAndGridSetting") );

		ids = GetSuccessYN( nMailClass , sLoanKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DMAndGridSetting") );

		if( ids > 0 ) continue;



		// 성공
		// DM UPDATE
		ids = pDM->SetCellData( GetMailDMAlias(nMailClass), sToday , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("DMAndGridSetting") );

		// 그리드 업데이트
		ids = CLocCommonAPI::InsertGridRow( this , pGrid , nIndex , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("DMAndGridSetting") );
	}

	return 0;

	EFS_END
	return -1;

}

CString CLocMailInfo::GetMailDMAlias( INT nMailClass )
{
	EFS_BEGIN

	const INT nCnt = 8;
	CString sMailDMFieldAlias[nCnt] =
	{
		_T("예약안내(반납시)메일_발송일") , _T("예약안내(비치자료예약)메일_발송일") , _T("반납독촉메일_발송일") , 
		_T("대출안내메일_발송일") , _T("반납안내(연체안된자료)메일_발송일") , _T("반납확인(연체된자료)메일_발송일") , 
		_T("예약취소메일_발송일") , _T("반납연기메일_발송일")
	};

	return sMailDMFieldAlias[nMailClass];

	EFS_END
	return _T("");

}

INT CLocMailInfo::GetSuccessYN( INT nMailClass , CString sLoanKey )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pLocDM = FindUseLocDM( nMailClass );
	if( pLocDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeMailInfo") );

	CString sTmpLoanKey;
	CString sYN;
	for( INT i = 0 ; i < pLocDM->GetRowCount() ; i++ )
	{
		ids = pLocDM->GetCellData( _T("대출/반납정보KEY") , i , sTmpLoanKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetSuccessYN") );

		if( sLoanKey.Compare(sTmpLoanKey)!=0 ) continue;

		ids = pLocDM->GetCellData( _T("성공여부") , i , sYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetSuccessYN") );

		if( sYN.Compare(_T("Y"))==0 ) return 0;
	}

	return 1;

	EFS_END
	return -1;

}

/*
EFS_BEGIN

	INT ids;

	CString	strCheckMailClass;
	INT	nRowCount;
	INT nCheckMailClass;
	INT	i;
	INT	nRowIndex;

	nRowIndex = -1;
	nRowCount = m_pMailModeInfoDM->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("메일종류") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				nRowIndex = i;
				break;
			}
		}
	}

	if ( nRowIndex == -1 )
	{
		return 1;
	}
	else
	{
		CString sYN;
		ids = m_pMailModeInfoDM->GetCellData( _T("기능사용여부") , nRowIndex , sYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsUseMailClass") );

		if( sYN.Compare( _T("N") )== 0 )
			return 1;
	}

	return 0;

	EFS_END
	return -1;
*/

CString CLocMailInfo::GetMailType(INT nMailClass)
{
	EFS_BEGIN

	INT ids;

	CString	strCheckMailClass;
	CString strMailType;
	INT	nRowCount;
	INT nCheckMailClass;
	INT	i;
	INT	nRowIndex;

	nRowIndex = -1;
	nRowCount = m_pMailModeInfoDM->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		ids = m_pMailModeInfoDM->GetCellData( _T("메일종류") , i , strCheckMailClass );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetMailName") );

		if ( !strCheckMailClass.IsEmpty() )
		{
			nCheckMailClass = _ttoi(strCheckMailClass);
			if ( nCheckMailClass == nMailClass )
			{
				nRowIndex = i;
				break;
			}
		}
	}

	if ( nRowIndex == -1 )
	{
		return _T("");
	}
	else
	{		
		ids = m_pMailModeInfoDM->GetCellData( _T("메일타입") , nRowIndex , strMailType );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsUseMailClass") );
		return strMailType;
	}

	return _T("");

	EFS_END
	return _T("");
}