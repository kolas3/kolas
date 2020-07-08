#include "stdafx.h"
#include <afxinet.h>
#include "BL_LOC_USER_PROC.h"
#include "FamilySearchDlg.h"
#include "..\\..\\..\\..\\공통\\IndexToolKit\\Index.h"
#include "SBL.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define SHARE_CO_LOAN_USER_TBL_PROC_REC_KEY _T("SHARE_CO_LOAN_USER_TBL_PROC_REC_KEY")
#define SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS _T("SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS")
#define SHARE_CO_LOAN_USER_TBL_FAMILY_REC_KEY _T("SHARE_CO_LOAN_USER_TBL_FAMILY_REC_KEY")
#define SHARE_CO_LOAN_USER_TBL_FAMILY_UPDATE_FIELDS _T("SHARE_CO_LOAN_USER_TBL_FAMILY_UPDATE_FIELDS")

BL_LOC_USER_PROC::BL_LOC_USER_PROC(CESL_Mgr *pWnd , CLoanShareManager* pLoanShareManager )
	: CSPF_Work(pWnd)
{
	m_pLoanShareManager = pLoanShareManager;	
	m_nAutoMode = -1;
	m_bSetedManageValue[0] = FALSE;
	m_bSetedManageValue[1] = FALSE;
	m_strUserID = pWnd->GetUserID();
	m_strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	
	m_sIsUnityLoanService = _T("");
	
	m_nLTMemberSaveMode = 0;
	m_p3000Api = NULL;
	m_bConnect = FALSE;
	
	m_strCertifyCode = _T("");
	m_strCertifyFailCode = _T("");
	m_strCertifyDate = _T("");
	m_strCertifySex = _T("");
	m_strCertifyWorker = _T("");
	m_strCertifyName = _T("");
	m_strCertifyCivilNo = _T("");

	
	m_strInsertCivilNo = _T("");
}

BL_LOC_USER_PROC::~BL_LOC_USER_PROC()
{
	if(NULL != m_p3000Api)
	{
		delete m_p3000Api;
		m_p3000Api = NULL;
	}
}

INT	BL_LOC_USER_PROC::SPFExcutePath( CString sPathAlias )
{
	INT ids = -1;

	DWORD nSTART , nFINISH;
	nSTART = GetTickCount();
	
	if( sPathAlias.Compare( _T("이용자관리_입력") )== 0 )
	{
		ids = InputLoanUser();
	}
	else if( sPathAlias.Compare( _T("이용자관리_수정") )== 0 )
	{
		ids = UpdateLoanUser();
	}
	else if( sPathAlias.Compare( _T("이용자_삭제") )== 0 )
	{
		ids = DeleteLoanUser();
	}
	else if( sPathAlias.Compare( _T("이용자검색") )== 0 )
	{
		ids = SearchLoanUser();
	}	
	else if( sPathAlias.Compare( _T("가족ID찾기") )== 0 )
	{
		ids = LoadFamilyID();
	}
	else if( sPathAlias.Compare( _T("이용자_제적처리") )== 0 )
	{
		ids = RemoveLoanUser();
	}
	else if( sPathAlias.Compare( _T("이용자_일괄변경") )== 0 )
	{
		ids = ChangeLoanUserValue();
	}
	else if( sPathAlias.Compare( _T("센터업로드") )== 0 )
	{
		ids = CenterUpload();
	}
	else if( sPathAlias.Compare( _T("이용자관리_대출자번호부여") )== 0 )
	{
		ids = LoanNoReGrant();		
	}	
	

	if(0 > ids)
	{
		AfxMessageBox(_T("기능을 정상적으로 수행하지 못하였습니다."));
		return -1;
	}

	nFINISH = GetTickCount();
	CString strMsg;
	strMsg.Format(_T("(Finish) - [%s] 걸린 시간 :  [%d.%d sec]"), sPathAlias , (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	CESL_DataMgr DM;
	DM.QueryLog(strMsg.GetBuffer(0));

	return 0;
}

INT BL_LOC_USER_PROC::InputLoanUser()
{
EFS_BEGIN

	BEGIN_SBL();

	INT ids;
	CString strFuncName = _T("InputLoanUser");
	CString sDMAlias = _T("DM_BO_LOC_3300");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->FreeData();
	ids = pDM->InsertRow( -1 );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	
	INT nRowIndex = pDM->GetRowCount()-1;
	ids = CMToDM( pDM );
	if(0 > ids) return ids;

	CString sInsertSQL;
	CKolas2up_MobileApi* pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , pDM );
	pDM->StartFrame();
	
	
	m_bConnect = FALSE;
	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
	{
		if(NULL != m_p3000Api)
		{
			delete m_p3000Api;
			m_p3000Api = NULL;
		}
		m_p3000Api = new CBO_LOC_3000Api(m_pParentMgr);
		if(NULL != m_p3000Api)
		{
			if(TRUE == m_p3000Api->CheckConnection())
			{
				m_bConnect = TRUE;
			}
		}
	}

	
	ids = DMToSQL( pDM , 0 , pMobileApi );
	if(0 > ids) 
	{
		if(NULL != m_p3000Api)
		{
			delete m_p3000Api;
			m_p3000Api = NULL;
		}
		return ids;
	}

	
	ids = InsertFamilyMember( pDM, pMobileApi );
	if(0 > ids)
	{
		if(NULL != m_p3000Api)
		{
			delete m_p3000Api;
			m_p3000Api = NULL;
		}
		return ids;
	}
	ids = UpdateFamilyMember( pDM, pMobileApi );
	if(0 > ids) 
	{
		if(NULL != m_p3000Api)
		{
			delete m_p3000Api;
			m_p3000Api = NULL;
		}
		return ids;
	}
	
	if(NULL != m_p3000Api)
	{
		delete m_p3000Api;
		m_p3000Api = NULL;
	}
	
	ids = SendFrameSQL( pDM , pMobileApi );
	if(0 > ids) return ids;
	CLocCommonAPI::MobileApiEndLog( pMobileApi );
	pDM->EndFrame();
	
	
	UploadUserImage();

	SendEMail(pDM);
	

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
	UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
	CString sRecKey;
	GetTempData( SHARE_CO_LOAN_USER_TBL_PROC_REC_KEY , sRecKey );
	UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
	
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	
	
	const int FAMILY_UPLOAD_COUNT = m_aFamily_UPLOAD_SHARE_INFO.GetSize();
	for(INT nFamilyUploadCnt = 0; nFamilyUploadCnt < FAMILY_UPLOAD_COUNT; nFamilyUploadCnt++)
	{
		UPLOAD_SHARE_INFO FamilyUploadShareInfo;
		FamilyUploadShareInfo = m_aFamily_UPLOAD_SHARE_INFO.GetAt(nFamilyUploadCnt);
		m_pLoanShareManager->SendLoanInfoUpload(FamilyUploadShareInfo);
	}
	m_aFamily_UPLOAD_SHARE_INFO.RemoveAll();


	ids = GridView(0);
	if(0 > ids) return ids;

	CESL_DataMgr *pSBLDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3300_LIST") );
	INT nRowCount = pSBLDM->GetRowCount();
	END_SBL(nRowCount);

	return 0;	

EFS_END
return -1;
}

INT BL_LOC_USER_PROC::SendEMail( CESL_DataMgr *pDM )
{
EFS_BEGIN
	//JOB.2019.0073 : 충돌 License
// 
// 	if( m_pParentMgr->m_pInfo->MODE == 10000 ) return 0;
// 
// 	INT ids;
// 	CString strFuncName = _T("SendEMail");
// 
// 	
// 	CString sYN;
// 	ids = m_pParentMgr->GetManageValue( _T("열람") , _T("공통") , _T("이용자관리") , _T("입력시신규회원메일발송여부") , sYN );
// 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
// 
// 	if( sYN.Compare(_T("N")) == 0 ) return 0;
// 
// 	CString sUserKey;
// 	CString sMailingUseYN;
// 	CString sWhere;
// 
// 	ids = pDM->GetCellData( _T("REC_KEY") , 0 , sUserKey );
// 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
// 
// 	ids = pDM->GetCellData( _T("메일수신여부") , 0 , sMailingUseYN );
// 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
// 
// 	if ( sMailingUseYN == _T("N") ) return 0;
// 
// 	sWhere.Format( _T(" REC_KEY = %s ") , sUserKey );
// 
// 	CLocMailMgrAPI MailMgr(m_pParentMgr);
// 	MailMgr.CreateMailInfo();
// 
// 	MailMgr.MakeRecord( 8 , sWhere , FALSE );

	return 0;

EFS_END
return -1;
}

INT BL_LOC_USER_PROC::UpdateLoanUser()
{
EFS_BEGIN
	

	INT ids;
	CString strFuncName = _T("UpdateLoanUser");
	CString sDMAlias = _T("DM_BO_LOC_3300");

	CString sMode;
	ids = m_pParentMgr->GetControlData( _T("CM_BO_LOC_3320") , _T("모드") , sMode );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if( _ttoi(sMode) != 3)
	{
		ids = CheckMoveToDM();
		if(0 > ids) return ids;
	}

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	
	ids = CMToDM( pDM );
	if( ids < 0 )
	{
		pDM->DeleteRow(pDM->GetRowCount() - 1);
		return ids;
	}		
	
	
	CString sUserName, sOldUserNo, sNewUserNo;
	pDM->GetCellData( _T("이름"), 0, sUserName );
	m_BackUpDM.GetCellData( _T("대출자번호"), 0, sOldUserNo );
	pDM->GetCellData( _T("대출자번호"), 0, sNewUserNo );


	CString sUpdatedFields_Log = GetCompareDM_DetailLog(pDM , &m_BackUpDM );

	
	CString sUpdatedFields;
	sUpdatedFields = GetCompareDM(pDM, &m_BackUpDM);

	CString sInsertSQL;

	CKolas2up_MobileApi* pMoblieApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , pDM );

	pDM->StartFrame();

	
	m_bConnect = FALSE;
	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
	{
		if(NULL != m_p3000Api)
		{
			delete m_p3000Api;
			m_p3000Api = NULL;
		}
		m_p3000Api = new CBO_LOC_3000Api(m_pParentMgr);
		if(NULL != m_p3000Api)
		{
			if(TRUE == m_p3000Api->CheckConnection())
			{
				m_bConnect = TRUE;
			}
		}
	}

	
	
	ids = DMToSQL(pDM, 1, pMoblieApi , sUpdatedFields_Log);
	if(0 > ids)
	{
		if(NULL != m_p3000Api)
		{
			delete m_p3000Api;
			m_p3000Api = NULL;
		}
		pDM->DeleteRow(pDM->GetRowCount() - 1);
		return ids;
	}	
	
	ids = InsertFamilyMember( pDM, pMoblieApi );
	if(0 > ids) 
	{
		if(NULL != m_p3000Api)
		{
			delete m_p3000Api;
			m_p3000Api = NULL;
		}
		return ids;
	}
	ids = UpdateFamilyMember( pDM, pMoblieApi );
	if(0 > ids) 
	{
		if(NULL != m_p3000Api)
		{
			delete m_p3000Api;
			m_p3000Api = NULL;
		}
		return ids;
	}

	if(NULL != m_p3000Api)
	{
		delete m_p3000Api;
		m_p3000Api = NULL;
	}

	
	ids = SendFrameSQL( pDM , pMoblieApi );
	if( ids < 0 )
	{
		pDM->DeleteRow(pDM->GetRowCount() - 1);
		return ids;
	}

	CLocCommonAPI::MobileApiEndLog( pMoblieApi );
	pDM->EndFrame();

	
	UploadUserImage();
	

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;	
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
	UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
	CString sRecKey;
	GetTempData( SHARE_CO_LOAN_USER_TBL_PROC_REC_KEY , sRecKey );
	
	
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
	if ( sUpdatedFields != _T("") )
	{		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = sUpdatedFields;		
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	}
		
	const int FAMILY_UPLOAD_COUNT = m_aFamily_UPLOAD_SHARE_INFO.GetSize();
	for(int nFamilyUploadCnt = 0; nFamilyUploadCnt < FAMILY_UPLOAD_COUNT; nFamilyUploadCnt++)
	{
		UPLOAD_SHARE_INFO FamilyUploadShareInfo;
		FamilyUploadShareInfo = m_aFamily_UPLOAD_SHARE_INFO.GetAt(nFamilyUploadCnt);
		m_pLoanShareManager->SendLoanInfoUpload(FamilyUploadShareInfo);
	}
	m_aFamily_UPLOAD_SHARE_INFO.RemoveAll();

	CString strFamilyRecKeyList;
	GetTempData(SHARE_CO_LOAN_USER_TBL_FAMILY_REC_KEY, strFamilyRecKeyList);

	if( !strFamilyRecKeyList.IsEmpty() )
	{
		if( sUpdatedFields.Find(_T("H_ADDR1"), 0 ) >= 0 || 
			sUpdatedFields.Find(_T("H_ZIPCODE"), 0 ) >= 0 ||
			sUpdatedFields.Find(_T("H_PHONE"), 0 ) >= 0 )
		{
			INT nCurPos=0;
			INT nPrevPos=0;
			CString strFamilyRecKey;

			while( (nCurPos=strFamilyRecKeyList.Find(_T(","),nPrevPos)) > 0 )
			{
				strFamilyRecKey=strFamilyRecKeyList.Mid(nPrevPos,nCurPos-nPrevPos);
				UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
				UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("H_ADDR1,H_ZIPCODE,H_PHONE");
				UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = strFamilyRecKey;
				m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );

				nPrevPos = nCurPos+1;
			}

			strFamilyRecKey=strFamilyRecKeyList.Mid(nPrevPos);
			UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
			UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("H_ADDR1,H_ZIPCODE,H_PHONE");
			UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = strFamilyRecKey;
			m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
		}
	}

	if( _ttoi(sMode) == 3) return 0;	
	ids = GridView(1);
	if( ids < 0 )
	{
		pDM->DeleteRow(pDM->GetRowCount() - 1);
		return ids;
	}

	return 0;
	
EFS_END
return -1;
}

INT BL_LOC_USER_PROC::CMToDM( CESL_DataMgr *pDM , INT nRowIndex )
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("CMToDM");
	CString sCMAlias = _T("CM_BO_LOC_3320");
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if(NULL==pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	INT	nControlCnt;
	const INT nAddControlCnt = 4;

	TCHAR *sControlAlias[52]  = {
		_T("대출자번호")	 , _T("교번/순번/군번")	 , _T("발급횟수")	   , _T("사용자ID")	   , _T("비밀번호")		  ,	
		_T("비밀번호확인")	 , _T("가족ID")			 , _T("이름")		   , _T("주민등록번호"), _T("소속")			  ,	
		_T("직급")			 , _T("우편발송지역")	 , _T("회원구분")	   , _T("자택우편번호"), _T("자택주소")		  ,	
		_T("자택전화")		 , _T("휴대폰1")		 , _T("휴대폰2")	   , _T("휴대폰3")	   , _T("EMAIL1")		  ,	
		_T("EMAIL2")		 , _T("근무지우편번호")	 , _T("근무지주소")	   , _T("근무지전화")  , _T("근무지내선번호") ,	
		_T("학교명")		 , _T("학년")			 , _T("반")			   , _T("부서명")	   , _T("직급명")		  ,	
		_T("회원상태")		 , _T("등록일자")		 , _T("수정일자")	   , _T("제적일자")	   , _T("대출정지일")	  ,	
		_T("비고")			 , _T("대출정지회원사유"), _T("대출책수")	   , _T("연체책수")	   , _T("총연체료")		  ,	
		_T("연체횟수")		 , _T("가족KEY")		 , _T("비고")		   , _T("생일")		   , _T("생일TYPE")		  ,	
		_T("자료실")		 , _T("회원증분실")		 , _T("회원증비밀번호"), _T("근무처")	   , _T("사용자정의코드1"),	
		_T("사용자정의코드2"), _T("사용자정의코드3")
	};		
	
	TCHAR *sAddControlAlias[nAddControlCnt]  = 
	{ 
		_T("메일수신여부"), _T("무인대출제한"), _T("SMS수신여부"), _T("제2연락처")
	};
	
	nControlCnt = 52;	
	CString sGetData[52];
	CString sAddGetData[nAddControlCnt];
	CString strNumType;
	
	INT nAutoMode;
	ids = IsAutoMode( nAutoMode );
	if(0>ids) return ids;
	CString sMode;
	ids = m_pParentMgr->GetControlData( _T("CM_BO_LOC_3320") , _T("모드") , sMode );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	for( INT i = 0 ; i < nControlCnt ; i++ )
	{
		if ( TRUE != m_bUseUserDefineCode && ( 49 == i || 50 == i || 51 == i ) )
		{
			continue;
		}

		ids = pCM->GetControlMgrData( sControlAlias[i] , sGetData[i] , -1 , 0 );
		if(0>ids)
		{
			sGetData[i] = _T("");
			continue;
		}
			
		if( 20 == i )
		{
			CESL_Control_Element *eControl = pCM->FindControlElement( sControlAlias[i] );
			if(NULL==eControl) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			CComboBox *pComboBox = (CComboBox*)(m_pParentMgr->GetDlgItem( eControl->CTRL_ID ));
			if(NULL==pComboBox) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			pComboBox->GetWindowText(sGetData[i]);
		}
		
		if( 0 == i )
		{
		
			if( (0==sMode.Compare(_T("0"))) && (4!=m_nLTMemberSaveMode) )
			{			
				CString civilNum;
				
				if( 2 == nAutoMode && TRUE != IsInputUserNo() )
				{
					
					pCM->GetControlMgrData(_T("이용자번호구분"), strNumType );
					if(0>GetAutoUserNo( sGetData[ 0 ], strNumType ) ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				}
				
				else if( 1 == nAutoMode && TRUE != IsInputUserNo() )
				{
					ids = pCM->GetControlMgrData( sControlAlias[8] , sGetData[8] , -1 , 0 );
					if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

					sGetData[0] = sGetData[8];
					sGetData[0].Replace(_T("-"),_T(""));					
				}
				
				else if( 3 == nAutoMode && TRUE != IsInputUserNo() )
				{
					ids = pCM->GetControlMgrData( sControlAlias[3] , sGetData[3] , -1 , 0 );
					if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

					sGetData[0] = sGetData[3];					
				}
			} 
			
			else if( (_T("1")==sMode||_T("3")==sMode) && (2==m_nLTMemberSaveMode) )
			{
				
				ids = KL_CheckUserNo(sGetData[0]);
			}

			
 			SetTempData(_T("대출자번호"), CComVariant(sGetData[0]));
		} 

		if( 3 == i ) 
		{ 
			sGetData[i].TrimLeft();		sGetData[i].TrimRight();
			sGetData[i].Replace( _T(" ") , _T("") );
		}			
		if(7 == i) 
		{
			sGetData[i].TrimLeft();		sGetData[i].TrimRight();
			SetTempData(_T("이름"), CComVariant(sGetData[i]));  
		}

		CString strCivilNoDisplayCheck;
		GetTempData(_T("주민등록번호사용여부"), strCivilNoDisplayCheck);
		if(8 == i && strCivilNoDisplayCheck.CompareNoCase(_T("Y")) == 0)
		{
			CString strMode;
			INT ids = m_pParentMgr->GetControlData( _T("CM_BO_LOC_3320") , _T("모드") , strMode );
			if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			
			if(_T("0")!=strMode)
			{
				CString strCivilNo = pDM->GetCellData(_T("주민등록번호"), 0);
				if(strCivilNo==sGetData[i])
				{				
					strCivilNo = pDM->GetCellData(_T("주민등록번호_CRYPTO"), 0);
					if(!strCivilNo.IsEmpty()) m_pParentMgr->ESL_DES_Decryption(strCivilNo,sGetData[i]);
				}
			}
		}
	}
	
	
	if(3 != nAutoMode && sMode==_T("0"))
	{
		if(_T("Y") == m_strIsAutoSetUsernoToUserid)
 		{
 			sGetData[3] = sGetData[0];
 		}
	}

	for( i = 0 ; i < nAddControlCnt ; i++ )
	{		
		if(_T("메일수신여부") == sAddControlAlias[i])
		{
			
			BOOL bCheck;
			bCheck = ( (CButton*) m_pParentMgr->GetDlgItem (IDC_chkMAILUSEYN) )->GetCheck();
			if ( TRUE == bCheck ) 
			{
				sAddGetData[i]=_T("Y");
			}
			else 
			{
				sAddGetData[i]=_T("N");
			}
		}
		else if(_T("SMS수신여부") == sAddControlAlias[i])
		{
			BOOL bCheck;
			bCheck = ((CButton*)m_pParentMgr->GetDlgItem(IDC_chkSMSUSEYN))->GetCheck();
			if ( TRUE == bCheck ) 
			{
				sAddGetData[i]=_T("Y");
			}
			else 
			{
				sAddGetData[i]=_T("N");
			}
		}
		else
		{
			
			ids = pCM->GetControlMgrData( sAddControlAlias[i] , sAddGetData[i] , -1 , 0 );
			if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
	}	

	INT	nDMFieldAliasCnt;
	TCHAR *sDMFieldAlias[ 56 ] =
	{
		_T("REC_KEY")		, _T("대출자번호")		, _T("사용자ID")		, _T("비밀번호")		, _T("가족KEY")			,
		_T("가족ID")		, _T("이름")			, _T("생일")			, _T("주민등록번호")	, _T("자택주소")		,
		_T("근무지주소")	, _T("자택우편번호")	, _T("근무지우편번호")	, _T("자택전화")		, _T("근무지전화")		,
		_T("근무지내선번호"), _T("부서명")			, _T("직급명")			, _T("휴대폰")			, _T("E_MAIL")			,
		_T("등록일자")		, _T("불량회원구분")	, _T("불량회원사유")	, _T("회원구분")		, _T("학교")			,
		_T("학년")			, _T("반")				, _T("교번/순번/군번")	, _T("대출소속정보")	, _T("대출직급정보")	,
		_T("우편발송지역")	, _T("총대출책수")		, _T("총예약책수")		, _T("연체횟수")		, _T("대출정지일")		,
		_T("최종대출일")	, _T("최종수정일")		, _T("휴대폰1")			, _T("휴대폰2")			, _T("휴대폰3")			,
		_T("EMAIL1")		, _T("EMAIL2")			, _T("선정")			, _T("연체책수")		, _T("총연체료")		,
		_T("발급횟수")		, _T("제적일자")		, _T("비고")			, _T("생일TYPE")		, _T("자료실")			, 
		_T("회원증분실")	, _T("회원증비밀번호")	, _T("근무처")			, _T("사용자정의코드1")	, _T("사용자정의코드2")	, 
		_T("사용자정의코드3")
	};

	
	
	
	TCHAR *sDMAddFieldAlias[ nAddControlCnt ] =
	{
		_T("메일수신여부"), _T("무인대출제한"), _T("SMS수신여부"), _T("제2연락처")
	};

	nDMFieldAliasCnt = 56;
	INT nRelateNumber[ 56 ] = {
		-1, 0 , 3 , 4 , 41,
		6 , 7 , 43, 8 , 14,
		22, 13, 21, 15, 23,
		24, 28, 29, -1, -1,
		31, 30, 36, 12, 25,
		26, 27, 1 , 9 , 10,
		11, 37, -1, 40, 34,
		-1, 32, 16, 17, 18,
		19, 20, -1, 38, 39,
		2 , 33, 42, 44, 45,
		46, 47, 48, 49, 50, 
		51
	};	

	INT nRow = nRowIndex;

	for( i=0 ; i<nDMFieldAliasCnt ; i++ )
	{
		if( nRelateNumber[i] == -1 ) 
		{
			continue;
		}

		if(		TRUE != m_bUseUserDefineCode 
			&& ( 49 == nRelateNumber[i] || 50 == nRelateNumber[i] || 51 == nRelateNumber[i] ) )
		{
			continue;
		}

		ids = pDM->SetCellData( sDMFieldAlias[i] , sGetData[ nRelateNumber[i] ] ,  nRow );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}

	for( i=0 ; i<nAddControlCnt ; i++ )
	{		
		ids = pDM->SetCellData( sDMAddFieldAlias[i] , sAddGetData[i] ,  nRow );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}

	CString sPhoneNumber;
	CString sEMail;
	
	if( 0 != sGetData[16].Compare(_T("적용안함")) && TRUE != sGetData[17].IsEmpty() && TRUE != sGetData[18].IsEmpty() )
	{
		sPhoneNumber.Format( _T("%s-%s-%s") , sGetData[16] , sGetData[17] , sGetData[18] );
	}

	ids = pDM->SetCellData( sDMFieldAlias[18] , sPhoneNumber ,  nRow );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	if( 0 != sGetData[20].Compare(_T("적용안함")) && TRUE != sGetData[19].IsEmpty() && TRUE != sGetData[20].IsEmpty() )
	{
		sEMail.Format( _T("%s@%s") , sGetData[19] , sGetData[20] );
		
		sEMail.Replace(_T("@@"), _T("@"));
	}

	ids = pDM->SetCellData( sDMFieldAlias[19] , sEMail ,  nRow );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	
	CString sMailSendArea;
	if( 0 == sGetData[11].Compare(_T("자택")) )
	{
		sMailSendArea = _T("H");
	}
	else
	{
		sMailSendArea = _T("W");
	}

	ids = pDM->SetCellData( sDMFieldAlias[30] , sMailSendArea ,  nRow );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	
	CString idxHandPhone = sPhoneNumber;
	CString idxName		 = sGetData[ 7 ];

	CIndex index;
	index.MakeIndex( idxHandPhone );
	index.MakeIndex( idxName );
	
	ids = pDM->SetCellData( sDMFieldAlias[18] , sPhoneNumber ,  nRow );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	ids = pDM->SetCellData( sDMFieldAlias[19] , sEMail ,  nRow );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->SetCellData( _T("IDX_핸드폰"), idxHandPhone, nRow );
	pDM->SetCellData( _T("IDX_이름")  , idxName,	  nRow );

	
	CString strManageCode;
	ids = pCM->GetControlMgrData( _T("회원관리구분") , strManageCode , -1 , 0 );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->SetCellData( _T("회원관리구분"), strManageCode, nRow );
	
	return 0;

EFS_END
return -1;
}

INT BL_LOC_USER_PROC::DMToSQL( CESL_DataMgr *pDM , INT nMode , CKolas2up_MobileApi *pMobileApi , CString sLog_Detail )
{
EFS_BEGIN

	INT ids, i, j;
	CString strFuncName = _T("DMToSQL");
	CStringArray asUpdateFields;
	
	INT nRow = pDM->GetRowCount()-1;	
	INT nDMFieldAliasCnt;
	
	const INT nDMAddFieldAliasCnt = 5;
	
	TCHAR *sDMFieldAlias[ 56 ] =
	{
		_T("REC_KEY")		, _T("대출자번호")		, _T("사용자ID")		, _T("비밀번호")		, _T("가족KEY")			,	
		_T("가족ID")		, _T("이름")			, _T("생일")			, _T("주민등록번호")	, _T("자택주소")		,	
		_T("근무지주소")	, _T("자택우편번호")	, _T("근무지우편번호")	, _T("자택전화")		, _T("근무지전화")		,	
		_T("근무지내선번호"), _T("부서명")			, _T("직급명")			, _T("휴대폰")			, _T("E_MAIL")			,	
		_T("등록일자")		, _T("불량회원구분")	, _T("불량회원사유")	, _T("회원구분")		, _T("학교")			,	
		_T("학년")			, _T("반")				, _T("교번/순번/군번")	, _T("대출소속정보")	, _T("대출직급정보")	,	
		_T("우편발송지역")	, _T("총대출책수")		, _T("총예약책수")		, _T("연체횟수")		, _T("대출정지일")		,	
		_T("최종대출일")	, _T("최종수정일")		, _T("발급횟수")		, _T("휴대폰1")			, _T("휴대폰2")			,	
		_T("휴대폰3")		, _T("EMAIL1")			, _T("EMAIL2")			, _T("선정")			, _T("연체책수")		,	
		_T("총연체료")		, _T("제적일자")		, _T("비고")			, _T("생일TYPE")		, _T("자료실")			,	
		_T("회원증분실")	, _T("회원증비밀번호")	, _T("근무처")			, _T("사용자정의코드1")	, _T("사용자정의코드2")	,
		_T("사용자정의코드3")
	};
	
	
	TCHAR *sDMAddFieldAlias[ nDMAddFieldAliasCnt ] =
	{
		_T("메일수신여부"), _T("신청제한일"),       _T("복귀만료횟수"), _T("무인대출제한"), _T("제2연락처")
	};

	nDMFieldAliasCnt = 56;
	
	if( nMode == 0 )
	{
		CString sRecKey;
		ids = pMobileApi->MakeRecKey( sRecKey );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		ids = pDM->SetCellData( sDMFieldAlias[0] , sRecKey ,  nRow );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	
	CString sGetData[ 56 ];
	CString sAddGetData[nDMAddFieldAliasCnt];
	
	for( i = 0 ; i < nDMFieldAliasCnt ;i++ )
	{
		if ( !m_bUseUserDefineCode && ( i==54 || i==55 ) )
			continue;
		
		pDM->GetCellData( sDMFieldAlias[i] , nRow , sGetData[i] );
	}	

	if( sGetData[23].Compare(_T("정회원"))==0)
	{
		sGetData[23] = _T("0");
	}
	else if( sGetData[23].Compare(_T("비회원"))==0)
	{
		sGetData[23] = _T("1");
	}
	
	else if( sGetData[23].Compare(_T("준회원"))==0)
	{
		sGetData[23] = _T("2");
	}
	else
	{
		sGetData[23] = _T("0");
	}
	
	if( sGetData[21].Compare(_T("정상"))==0 || (sGetData[21].Compare(_T("정상회원"))==0))
	{
		sGetData[21] = _T("0");
	}
	else if( sGetData[21].Compare(_T("대출정지"))==0)
	{
		sGetData[21] = _T("1");
	}
	else if( sGetData[21].Compare(_T("제적"))==0 || (sGetData[21].Compare(_T("제적회원"))==0))
	{
		sGetData[21] = _T("2");
	}
	
	else if( sGetData[21].Compare(_T("탈퇴"))==0 || (sGetData[21].Compare(_T("탈퇴회원"))==0))
	{
		sGetData[21] = _T("3");
	}
	else
	{
		sGetData[21] = _T("0");
	}
	
	m_strInsertCivilNo = sGetData[8];
	
	CString strEncryptionCivilNo = _T("");
	if(_T("Y")==m_sIsUnityLoanService)
	{
		m_pParentMgr->ESL_DES_Encryption(sGetData[8], strEncryptionCivilNo);
		
		if(_T("N")!=m_strIsKLSecurityCivilNo) 
		{
			ids = CLocCommonAPI::ChangeSecuriyCivilNo( m_pParentMgr , sGetData[8] , m_sIsUnityLoanService );
			
			ids = pDM->SetCellData( _T("주민등록번호") , sGetData[8] ,  nRow );
		}
	}
	
	if(m_pParentMgr->m_pInfo->m_bVisitorPwEncryption)
	{
		
		CString strTmp = sGetData[3];
		if(64 > strTmp.GetLength())
		{
			sGetData[3] = CLocCommonAPI::StringToSHA256(sGetData[3]);
			ids = pDM->SetCellData( _T("비밀번호") , sGetData[3],  nRow );
		}

	}
	
	if(m_pParentMgr->m_pInfo->m_bCardPwEncryption)
	{
		
		CString strTmp = sGetData[51];
		if(64 > strTmp.GetLength())
		{
			sGetData[51] = CLocCommonAPI::StringToSHA256(sGetData[51]);
			ids = pDM->SetCellData( _T("회원증비밀번호") , sGetData[51],  nRow );
		}
	}

	for( i = 0 ; i < nDMAddFieldAliasCnt ;i++ )
	{	
		pDM->GetCellData( sDMAddFieldAlias[i] , nRow , sAddGetData[i] );
	}

	INT nInsertFieldCnt;
	TCHAR *sInsertField[ 47 ] =
	{
		_T("REC_KEY")			, _T("USER_NO")				, _T("USER_ID")				, _T("PASSWORD")		, _T("FID_KEY")				, 
		_T("NAME")				, _T("BIRTHDAY")			, _T("CIVIL_NO")			, _T("H_ADDR1")			, _T("W_ADDR1")				, 
		_T("H_ZIPCODE")			, _T("W_ZIPCODE")			, _T("H_PHONE")				, _T("W_PHONE")			, _T("EXCHANGE_NO")			, 
		_T("DEPARTMENT_NAME")	, _T("CLASS_NAME")			, _T("HANDPHONE")			, _T("E_MAIL")			, _T("REG_DATE")			, 
		_T("USER_CLASS")		, _T("INFERIOR_REASON")		, _T("MEMBER_CLASS")		, _T("SCHOOL")			, _T("GRADE")				, 
		_T("CLASS")				, _T("CLASS_NO")			, _T("USER_POSITION_CODE")	, _T("USER_CLASS_CODE")	, _T("MAIL_SEND_AREA")		, 
		_T("LOAN_COUNT")		, _T("RESERVATION_COUNT")	, _T("DELAY_COUNT")			, _T("LOAN_STOP_DATE")	, _T("LAST_LOAN_DATE")		, 
		_T("LAST_MODIFY_DATE")	, _T("REISSUE_CNT")			, _T("REMOVE_DATE")			, _T("REMARK")			, _T("BIRTHDAY_TYPE")		, 
		_T("SHELF_LOC_CODE")	, _T("LOST_USER_CARD")		, _T("CARD_PASSWORD")		, _T("OFFICE")			, _T("USER_DEFINE_CODE1")	, 
		_T("USER_DEFINE_CODE2")	, _T("USER_DEFINE_CODE3")
	};


	
	TCHAR *sAddInsertField[ nDMAddFieldAliasCnt ] =
	{
		_T("MAILING_USE_YN"),  _T("ILL_APL_STOP_DATE"),  _T("ILL_EXPIRE_RETURN_CNT"), _T("SELF_LOAN_STATION_LIMIT"), _T("SECOND_PHONE")
	};

	INT nRelateNumber[ 47 ] =
	{
		0 , 1 , 2 , 3 , 4 ,
		6 , 7 , 8 , 9 , 10 ,
		11 , 12 , 13 , 14 , 15 , 
		16 , 17 , 18 , 19 , 20 , 
		21 , 22 , 23 , 24 , 25 ,
		26 , 27 , 28 , 29 , 30 ,
		31 , 32 , 33 , 34 , 35 ,
		36 , 37 , 46 , 47 , 48 , 
		49 , 50 , 51 , 52 , 53 , 
		54 , 55	
	};
	CString sType[ 47 ] =
	{
		_T("NUMERIC")	, _T("STRING")	, _T("STRING"), _T("STRING"), _T("NUMERIC")	,
		_T("STRING")	, _T("STRING")	, _T("STRING"), _T("STRING"), _T("STRING")	, 
		_T("STRING")	, _T("STRING")	, _T("STRING"), _T("STRING"), _T("STRING")	, 
		_T("STRING")	, _T("STRING")	, _T("STRING"), _T("STRING"), _T("STRING")	, 
		_T("STRING")	, _T("STRING")	, _T("STRING"), _T("STRING"), _T("STRING")	,
		_T("STRING")	, _T("STRING")	, _T("STRING"), _T("STRING"), _T("STRING")	, 
		_T("STRING")	, _T("STRING")	, _T("STRING"), _T("STRING"), _T("STRING")	, 
		_T("STRING")	, _T("NUMERIC")	, _T("STRING"), _T("STRING"), _T("STRING")	,
		_T("STRING")	, _T("STRING")	, _T("STRING"), _T("STRING"), _T("STRING")	,
		_T("STRING")	, _T("STRING")		
	};

	
	CString sAddType[nDMAddFieldAliasCnt] =
	{
		_T("STRING"), _T("NUMERIC"), _T("NUMERIC"), _T("STRING"), _T("STRING")
	};

	nInsertFieldCnt = 47;
	
	CString strGetData;
	CTime t = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);

	pDM->InitDBFieldData();
	for( i = 0 ; i < nInsertFieldCnt ;i++ )
	{
		if ( !m_bUseUserDefineCode && ( nRelateNumber[i] == 53 || nRelateNumber[i] == 54 || nRelateNumber[i] == 55 ) )
			continue;

		sGetData[nRelateNumber[i]].TrimRight();

		if( i == 0 ) continue;

		
		if( ((i==19) && (nMode==0))  || ((i==35) && (nMode==1)) )

		{
			sGetData[nRelateNumber[i]] = t.Format( _T("%Y/%m/%d") );
			
			if( i == 19 )
			{
				ids = pDM->SetCellData( _T("등록일자") , sGetData[nRelateNumber[i]] , 0 );
				if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			}

			
			CString strTime = t.Format( _T("%H:%M") );
			
			if( i == 19 )
			{
				ids = pDM->SetCellData( _T("등록일자시간") , strTime , 0 );
				if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			}
			else if( i == 35 )
			{				
				ids = pDM->SetCellData( _T("최종수정일"), sGetData[nRelateNumber[i]], 0 );			
				if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				ids = pDM->SetCellData( _T("최종수정일자시간") , strTime , 0 );
				if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			}
		}

		if( sGetData[nRelateNumber[i]].IsEmpty() ) 
		{
			sType[i] = _T("NUMERIC");
			sGetData[nRelateNumber[i]] = _T("NULL");
		}
		
		
		if( i == 19 && !sGetData[ nRelateNumber[i] ].IsEmpty() && sGetData[nRelateNumber[i]] != _T("NULL"))
		{
			CString strBuf;
			strBuf.Format(_T("TO_DATE( '%s %s', 'YYYY/MM/DD HH24:MI' )"), 
				sGetData[ nRelateNumber[i] ], pDM->GetCellData(_T("등록일자시간"), 0) );
			sGetData[ nRelateNumber[i] ] = strBuf;
			sType[i] = _T("NUMERIC");
		}
		else if( i == 35 && !sGetData[ nRelateNumber[i] ].IsEmpty() && sGetData[nRelateNumber[i]] != _T("NULL") )
		{
			CString strBuf;
			strBuf.Format(_T("TO_DATE( '%s %s', 'YYYY/MM/DD HH24:MI' )"), 
				sGetData[ nRelateNumber[i] ], pDM->GetCellData(_T("최종수정일자시간"), 0) );
			sGetData[ nRelateNumber[i] ] = strBuf;
			sType[i] = _T("NUMERIC");
		}
		
		if(NULL != m_p3000Api && TRUE == m_bConnect)
		{
			if(_T("NULL") != sGetData[nRelateNumber[i]])
			{
				if( _T("CARD_PASSWORD") == sInsertField[i] || _T("CIVIL_NO") == sInsertField[i] || _T("E_MAIL") == sInsertField[i] || _T("HANDPHONE") == sInsertField[i] || _T("H_ADDR1") == sInsertField[i] || _T("H_PHONE") == sInsertField[i] || _T("W_ADDR1") == sInsertField[i] || _T("W_PHONE") == sInsertField[i] || _T("PASSWORD") == sInsertField[i])
				{
					if(TRUE == m_p3000Api->ConnectSocket()) 
					{
						CStringArray DataArray;
						DataArray.Add(sGetData[nRelateNumber[i]]);
						if(TRUE == m_p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), sInsertField[i], DataArray))
						{
							sGetData[nRelateNumber[i]] = DataArray.GetAt(0);
						}
						DataArray.RemoveAll();
						DataArray.FreeExtra();
						m_p3000Api->CloseSocket();
					}
				}
			}
		}
		ids = pDM->AddDBFieldData( sInsertField[i] , sType[i] , sGetData[nRelateNumber[i]],TRUE);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		asUpdateFields.Add(sInsertField[i]);
	}

	for( i = 0 ; i < nDMAddFieldAliasCnt ;i++ )
	{
		if( sAddGetData[i].IsEmpty() ) 
		{
			sAddType[i] = _T("NUMERIC");
			sAddGetData[i] = _T("NULL");
		}
		
		if(_T("ILL_APL_STOP_DATE") == sAddInsertField[i] && _T("NULL") != sAddGetData[i])
		{
			CString strBuf;
			strBuf.Format(_T("TO_DATE( '%s', 'YYYY/MM/DD' )"), sAddGetData[i]);
			sAddGetData[i] = strBuf;
			sAddType[i] = _T("NUMERIC");
		}
		
		if(NULL != m_p3000Api && TRUE == m_bConnect)
		{
			if(_T("NULL") != sAddGetData[i])
			{
				if(_T("SECOND_PHONE") == sAddInsertField[i])
				{
					if(TRUE == m_p3000Api->ConnectSocket()) 
					{
						CStringArray DataArray;
						DataArray.Add(sAddGetData[i]);
						if(TRUE == m_p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), sAddInsertField[i], DataArray))
						{
							sAddGetData[i] = DataArray.GetAt(0);
						}
						DataArray.RemoveAll();
						DataArray.FreeExtra();
						m_p3000Api->CloseSocket();
					}
				}
			}
		}
		ids = pDM->AddDBFieldData( sAddInsertField[i] , sAddType[i] , sAddGetData[ i ],TRUE);		
	}

	
	CString strIdxName, strIdxPhone;
	strIdxName = pDM->GetCellData(_T("IDX_이름"), nRow);
	strIdxPhone = pDM->GetCellData(_T("IDX_핸드폰"), nRow);
	
	if(NULL != m_p3000Api && TRUE == m_bConnect)
	{
		if(_T("NULL") != strIdxPhone)
		{
			if(TRUE == m_p3000Api->ConnectSocket()) 
			{
				CStringArray DataArray;
				DataArray.Add(strIdxPhone);
				if(TRUE == m_p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("IDX_HANDPHONE"), DataArray))
				{
					strIdxPhone = DataArray.GetAt(0);
				}
				DataArray.RemoveAll();
				DataArray.FreeExtra();
				m_p3000Api->CloseSocket();
			}
		}
	}
	pDM->AddDBFieldData(_T("IDX_USER_NAME"), _T("STRING"), strIdxName, true);
	pDM->AddDBFieldData(_T("IDX_HANDPHONE"), _T("STRING"), strIdxPhone, true);
	asUpdateFields.Add( _T("IDX_USER_NAME") );
	asUpdateFields.Add( _T("IDX_HANDPHONE") );
	
	
	if(_T("Y")==m_sIsUnityLoanService)
	{
		pDM->AddDBFieldData( _T("CRYPTO_CIVIL_NO"), _T("STRING"), strEncryptionCivilNo, true );
		asUpdateFields.Add( _T("CRYPTO_CIVIL_NO") );
	}

	CString sValue;
	if( m_bSetedManageValue[ 1 ] )
		sValue = m_sSMSInfo;
	else
	{
		CString sSql;	
		m_bSetedManageValue[ 1 ] = TRUE;

		
		ids = m_pParentMgr->m_pInfo->GetSMSUseMode();
		if(ids == 0)		sValue = _T("N");
		else if(ids == 1)	sValue = _T("Y");
		else				sValue = _T("N");
	}
			
	if( sValue.Compare(_T("N")) == 0 || sValue.IsEmpty() )
	{
	}
	else
	{
		CString sSMSYN;
		GetTempData( _T("SMS수신여부") , sSMSYN );
		
		pDM->AddDBFieldData( _T("SMS_USE_YN") , _T("STRING") , sSMSYN );
	}

	
	CString strManageCode;
	ids = pDM->GetCellData( _T("회원관리구분"), nRow, strManageCode );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	strManageCode.TrimLeft();strManageCode.TrimRight();
	pDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , strManageCode );

	
	CString monetaYN;
	GetTempData( _T("MONETA_SERVICE_YN"), monetaYN );
	if( _T("Y") == monetaYN && !UploadMONETAInfo( sGetData[ 20 ], sGetData[ 18 ], sGetData[ 8 ], GetLibCode() ) )
	{
		pDM->AddDBFieldData( _T("MONETA_INFORM_DATE"),      _T("STRING"), sGetData[ 20 ] );
		pDM->AddDBFieldData( _T("MONETA_INFORM_PHONE_NUM"), _T("STRING"), sGetData[ 18 ] );
	}		
	
	CString strKLImportYN;
	GetTempData(_T("KL_IMPORT_YN"), strKLImportYN);
	if (strKLImportYN.CompareNoCase(_T("Y")) == 0)
	{
		pDM->AddDBFieldData(_T("KL_IMPORT_YN"), _T("STRING"), _T("Y"));
	}
	
 	CString strQuery;
	if( nMode == 0 )
	{
		
		if(!m_strCertifyCode.IsEmpty())
		{
			strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET IPIN_HASH=NULL,GPIN_SEX=NULL,IPIN_DATE=NULL,IPIN_RESULT=NULL,CERTIFY_WORKER=NULL WHERE IPIN_HASH='%s';"), m_strCertifyCode);
			pDM->AddFrame(strQuery);
		}
		
		pDM->AddDBFieldData( _T("CENTER_UPLOAD_CHECK") , _T("STRING") , _T("N") );
		pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T(""), __WFILE__, __LINE__ ), TRUE );
		
		pDM->AddDBFieldData(_T("IPIN_HASH"), _T("STRING"), m_strCertifyCode);
		pDM->AddDBFieldData(_T("GPIN_SEX"), _T("STRING"), m_strCertifySex);
		CString strDate;
		strDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), m_strCertifyDate);
		pDM->AddDBFieldData(_T("IPIN_DATE"), _T("NUMERIC"), strDate);
		pDM->AddDBFieldData(_T("IPIN_RESULT"), _T("STRING"), m_strCertifyFailCode);
		pDM->AddDBFieldData(_T("CERTIFY_WORKER"), _T("STRING"), m_strCertifyWorker);
		ids = pMobileApi->MakeInsertFrame( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sGetData[0] );


		strQuery.Format(_T("INSERT INTO CO_LOAN_USER_EDIT_TBL")
			_T("(REC_KEY,TYPE,WORKER_IP,WORKER_ID,WORKER_NAME,WORKER_SYS,USER_KEY,WORK_DATE,FIRST_WORK,LOG_DETAIL,USER_TYPE) ")
			_T("VALUES(ESL_SEQ.NEXTVAL,'I','%s','%s','%s','KOLASIII',%s,SYSDATE,'%s' , '%s' , 'U' );")
			, m_pParentMgr->m_pInfo->LOCAL_IP
			, m_pParentMgr->m_pInfo->USER_ID
			, m_pParentMgr->m_pInfo->USER_NAME
			, sGetData[0]
			, m_pParentMgr->GetWorkLogMsg(_T("이용자입력"),__WFILE__,__LINE__)
			, sLog_Detail
			);

		
		pDM->AddFrame(strQuery);
	}
	else if( nMode == 1 )
	{
		if (!m_strCertifySex.IsEmpty())
		{
			pDM->AddDBFieldData(_T("GPIN_SEX"), _T("STRING"), m_strCertifySex);
		}

		if(m_bRemarkEditYN == TRUE)
		{
			ids= pMobileApi->AddDBFieldData( _T("REMARK_EDIT_DATE"), _T("NUMERIC"), _T("SYSDATE"), TRUE );
			if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		pMobileApi->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T(""), __WFILE__, __LINE__ ), TRUE );
		ids = pMobileApi->MakeUpdateFrame( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sGetData[0] );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		CString strLogData = _T("");
		pDM->GetCellData(_T("WORKER_DO"), 0, strLogData);

		strQuery.Format(_T("INSERT INTO CO_LOAN_USER_EDIT_TBL")
			_T("(REC_KEY,TYPE,WORKER_IP,WORKER_ID,WORKER_NAME,WORKER_SYS,USER_KEY,WORK_DATE,FIRST_WORK,WORKER_DO,LOG_DETAIL,USER_TYPE) ")
			_T("VALUES(ESL_SEQ.NEXTVAL,'U','%s','%s','%s','KOLASIII',%s,SYSDATE,'%s','%s' , '%s' , 'U');")
			, m_pParentMgr->m_pInfo->LOCAL_IP
			, m_pParentMgr->m_pInfo->USER_ID
			, m_pParentMgr->m_pInfo->USER_NAME
			, sGetData[0]
			, m_pParentMgr->GetWorkLogMsg(_T("이용자수정"),__WFILE__,__LINE__)
			, strLogData
			, sLog_Detail
			);

		
		pDM->AddFrame(strQuery);

		
		if(m_strBeforeUserNo != m_strAfterUserNo)
		{
			strQuery.Format(_T("UPDATE BO_FURNISH_TBL SET USER_NO='%s' WHERE USER_NO='%s';"), m_strAfterUserNo, m_strBeforeUserNo);
			pMobileApi->AddFrame(strQuery);
		}
		
		if( !sGetData[4].IsEmpty() )
		{
			CESL_DataMgr SyncDM;
			SyncDM.SetCONNECTION_INFO( pDM->CONNECTION_INFO );
		
			strQuery.Format( _T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE FID_KEY=%s AND REC_KEY<>%s"), sGetData[4], sGetData[0]);
			ids = SyncDM.RestructDataManager(strQuery);
			if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			CString SyncRecKey;
			CString strSyncSql;

			CString strShareRecKeyList;

			CString strAddressHome = sGetData[9];
			CString strZipCodeHome = sGetData[11];
			CString strPhoneHome   = sGetData[13];
			
			if( strAddressHome == _T("NULL") )
				strAddressHome = _T("");
			
			if( strZipCodeHome == _T("NULL") )
				strZipCodeHome = _T("");

			if( strPhoneHome == _T("NULL") )
				strPhoneHome = _T("");

			for(INT i=0;i<SyncDM.GetRowCount();i++)
			{
				ids = SyncDM.GetCellData(i, 0, SyncRecKey);
				if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

				
				strSyncSql.Format( _T("UPDATE CO_LOAN_USER_TBL ")
									 _T(" SET H_ADDR1='%s',")
										   _T("H_ZIPCODE='%s',")
										   _T("H_PHONE='%s' ") 
									_T(" WHERE REC_KEY=%s;"), strAddressHome, strZipCodeHome, 
									strPhoneHome, SyncRecKey);
				pMobileApi->AddFrame(strSyncSql);



				strQuery.Format(_T("INSERT INTO CO_LOAN_USER_EDIT_TBL")
					_T("(REC_KEY,TYPE,WORKER_IP,WORKER_ID,WORKER_NAME,WORKER_SYS,USER_KEY,WORK_DATE,FIRST_WORK , USER_TYPE) ")
					_T("VALUES(ESL_SEQ.NEXTVAL,'U','%s','%s','%s','KOLASIII',%s,SYSDATE,'%s' , 'U');")
					, m_pParentMgr->m_pInfo->LOCAL_IP
					, m_pParentMgr->m_pInfo->USER_ID
					, m_pParentMgr->m_pInfo->USER_NAME
					, SyncRecKey
					, m_pParentMgr->GetWorkLogMsg(_T("이용자수정"),__WFILE__,__LINE__));

				

				pDM->AddFrame(strQuery);

				if(strShareRecKeyList.IsEmpty())
					strShareRecKeyList = SyncRecKey;
				else
					strShareRecKeyList = strShareRecKeyList+_T(",")+SyncRecKey;
			}		

			SetTempData(SHARE_CO_LOAN_USER_TBL_FAMILY_REC_KEY, CComVariant(strShareRecKeyList));
			SetTempData(SHARE_CO_LOAN_USER_TBL_FAMILY_UPDATE_FIELDS, CComVariant(_T("H_ADDR1,H_ZIPCODE,H_PHONE")));
		}
	}

	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sUpdateFields;
	for( INT c = 0 ; c < asUpdateFields.GetSize() ; c++ )
	{
		if( c != 0 ) sUpdateFields += _T(",");
		sUpdateFields += asUpdateFields.GetAt(c);
	}

	sUpdateFields.Replace(_T(",OFFICE"),_T(""));

	SetTempData(SHARE_CO_LOAN_USER_TBL_PROC_REC_KEY , CComVariant(sGetData[0]) );
	SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS , CComVariant(sUpdateFields) );
	
	return 0;

EFS_END
return -1;
}



INT BL_LOC_USER_PROC::SendFrameSQL( CESL_DataMgr *pDM , CKolas2up_MobileApi* pMobileApi )
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SendFrameSQL");
	
	ids = pMobileApi->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->InitDBFieldData();

	return 0;

EFS_END
return -1;
}


INT BL_LOC_USER_PROC::LoadFamilyID()
{
EFS_BEGIN

	CString strFuncName = _T("LoadFamilyID");
	CString sCMAlias = _T("CM_BO_LOC_3320");

	CString sFamilyID;
	m_pParentMgr->GetControlData( sCMAlias , _T("가족ID") , sFamilyID );

	CFamilySearchDlg FamilyDlg( m_pParentMgr );

	FamilyDlg.SetFamilyID( sFamilyID );
	
	FamilyDlg.m_sIsUnityLoanService = m_sIsUnityLoanService;
	FamilyDlg.DoModal();
	
	if( FamilyDlg.GetCloseState() == 1 )
		return 0;

	CString sFamilyRecKey;
	FamilyDlg.GetFamilyID( sFamilyID , sFamilyRecKey );

	m_pParentMgr->SetControlData( sCMAlias , _T("가족ID") , sFamilyID );
	m_pParentMgr->SetControlData( sCMAlias , _T("가족KEY") , sFamilyRecKey );
	
	CString sZipCodeHome, sAddressHome, sPhoneHome;	
	FamilyDlg.GetAddressHome( sZipCodeHome, sAddressHome, sPhoneHome );
	if ( !sAddressHome.IsEmpty() )
	{
		m_pParentMgr->SetControlData( sCMAlias , _T("자택우편번호") , sZipCodeHome );
		m_pParentMgr->SetControlData( sCMAlias , _T("자택주소") , sAddressHome );
	}

	if ( !sPhoneHome.IsEmpty() )
	{
		m_pParentMgr->SetControlData( sCMAlias , _T("자택전화") , sPhoneHome );
	}

	return 0;

EFS_END
return -1;
}


INT BL_LOC_USER_PROC::SearchLoanUser()
{
EFS_BEGIN

	BEGIN_SBL();

	INT ids;
	CString strFuncName = _T("SearchLoanUser");
	CString sWhere;

	
	ids = GetSearchWhereSql( sWhere );
	if(0 != ids) return ids;

	CString sDMAlias = _T("DM_BO_LOC_3300_LIST");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pDM , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	ids = CLocCommonAPI::ChangeSecuriyCivilNo( m_pParentMgr , pDM , -1 , _T("주민등록번호") , m_sIsUnityLoanService );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString strValue1;
	ids = m_pParentMgr->GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue1 );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	strValue1.TrimLeft();strValue1.TrimRight();
	strValue1.MakeUpper();	
	INT i , nRowCnt = pDM->GetRowCount();
	CString strUserNo;

	
	CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6;

	for(i = 0; i < nRowCnt; i++)
	{
		ids = pDM->GetCellData(_T("대출자번호"), i, strUserNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		if(_T("Y") != strValue1)
		{
			CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , strUserNo , m_sIsUnityLoanService );
		}
		ids = pDM->SetCellData(_T("대출자번호_VIEW"), strUserNo, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		
		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
		{
			CString strGetData;
			pDM->GetCellData(_T("주민등록번호"), i, strGetData);
			asDes1.Add(strGetData);
			pDM->GetCellData(_T("E_MAIL"), i, strGetData);
			asDes2.Add(strGetData);
			pDM->GetCellData(_T("자택전화"), i, strGetData);
			asDes3.Add(strGetData);
			pDM->GetCellData(_T("집주소"), i, strGetData);
			asDes4.Add(strGetData);
			pDM->GetCellData(_T("핸드폰"), i, strGetData);
			asDes5.Add(strGetData);
			pDM->GetCellData(_T("제2연락처"), i, strGetData);
			asDes6.Add(strGetData);
		}
	}

	
	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse && 0 < nRowCnt)
	{
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(m_pParentMgr);
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
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes2);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes3);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes4);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes5);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("SECOND_PHONE"), asDes6);
					pApi->CloseSocket();	
				}
				for(i = 0; i < nRowCnt; i++)
				{
					pDM->SetCellData(_T("주민등록번호"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("E_MAIL"), asDes2.GetAt(i), i);
					pDM->SetCellData(_T("자택전화"), asDes3.GetAt(i), i);
					pDM->SetCellData(_T("집주소"), asDes4.GetAt(i), i);
					pDM->SetCellData(_T("핸드폰"), asDes5.GetAt(i), i);
					pDM->SetCellData(_T("제2연락처"), asDes6.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
		asDes6.RemoveAll(); asDes6.FreeExtra();
	}

	CESL_DataMgr *pSBLDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3300_LIST") );
	INT nRowCount = pSBLDM->GetRowCount();
	END_SBL(nRowCount);

	if( pDM->GetRowCount() == 0 )
		AfxMessageBox(_T("조건에 일치하는 이용자가 존재하지 않습니다."));
	
	return 0;

EFS_END
return -1;
}

INT BL_LOC_USER_PROC::SearchAddLoanUser(CStringArray& aUserKey)
{
EFS_BEGIN
	
	CString strTemp;
	INT		Count;
	INT		ids;
	INT		nIndex;
	CString strFuncName = _T("SearchAddLoanUser");

	CString sDMAlias = _T("DM_BO_LOC_3300_LIST");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	nIndex = pDM->GetRowCount();

	CString sCMAlias = _T("CM_BO_LOC_3300");
	CString	sGridAlias = _T("MainGrid");
	CESL_Grid* pGrid = (CESL_Grid*)m_pParentMgr->FindControl( sCMAlias , sGridAlias );
	if( pGrid == NULL ) return 0;

	
	CString strUserKey;	
	int nUserKeyCnt = 0;
	int nRowCnt = 0;
	for(nUserKeyCnt = 0; nUserKeyCnt < aUserKey.GetSize(); nUserKeyCnt++)
	{
		strUserKey = aUserKey.GetAt(nUserKeyCnt);

		for (nRowCnt = 0 ; nRowCnt < nIndex ; nRowCnt++ )
		{
			strTemp = pDM->GetCellData(_T("REC_KEY"), nRowCnt);
			if ( strUserKey == strTemp )
			{
				pGrid->SetReverse(nRowCnt);
				aUserKey.RemoveAt(nUserKeyCnt);
				nUserKeyCnt--;
				break;

			}
		}
	}
	if(0 == aUserKey.GetSize())
	{
		return 0;
	}

	CString strWhere = _T("REC_KEY IN (");
	for(nUserKeyCnt = 0; nUserKeyCnt < aUserKey.GetSize(); nUserKeyCnt++)
	{
		strWhere = strWhere + aUserKey.GetAt(nUserKeyCnt) + _T(",");
	}
	strWhere.TrimRight(_T(","));
	strWhere = strWhere + _T(")");	

	CESL_DataMgr TmpDM;
	CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , &TmpDM );		
	TmpDM.RefreshDataManager(strWhere);

	
	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse && 0 < TmpDM.GetRowCount())
	{
		CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6;
		CString strGetData;
		for(INT i = 0; i < TmpDM.GetRowCount(); i++)
		{
			TmpDM.GetCellData(_T("주민등록번호"), i, strGetData);
			asDes1.Add(strGetData);
			TmpDM.GetCellData(_T("E_MAIL"), i, strGetData);
			asDes2.Add(strGetData);
			TmpDM.GetCellData(_T("자택전화"), i, strGetData);
			asDes3.Add(strGetData);
			TmpDM.GetCellData(_T("집주소"), i, strGetData);
			asDes4.Add(strGetData);
			TmpDM.GetCellData(_T("핸드폰"), i, strGetData);
			asDes5.Add(strGetData);
			TmpDM.GetCellData(_T("제2연락처"), i, strGetData);
			asDes6.Add(strGetData);
		}
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(m_pParentMgr);
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
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes2);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes3);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes4);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes5);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("SECOND_PHONE"), asDes6);
					pApi->CloseSocket();
				}
				for(i = 0; i < TmpDM.GetRowCount(); i++)
				{
					TmpDM.SetCellData(_T("주민등록번호"), asDes1.GetAt(i), i);
					TmpDM.SetCellData(_T("E_MAIL"), asDes2.GetAt(i), i);
					TmpDM.SetCellData(_T("자택전화"), asDes3.GetAt(i), i);
					TmpDM.SetCellData(_T("집주소"), asDes4.GetAt(i), i);
					TmpDM.SetCellData(_T("핸드폰"), asDes5.GetAt(i), i);
					TmpDM.SetCellData(_T("제2연락처"), asDes6.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
		asDes6.RemoveAll(); asDes6.FreeExtra();
	}

	
	
	ids = CLocCommonAPI::ChangeSecuriyCivilNo( m_pParentMgr , &TmpDM , -1 , _T("주민등록번호") , m_sIsUnityLoanService );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	ids = CLocCommonAPI::AddDM( &TmpDM ,pDM , m_pParentMgr);	
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	Count = TmpDM.GetRowCount();			

	if ( Count > 0 )
	{		
		ids = UserCodeToDesc( sDMAlias );
		if( ids < 0 ) return ids;

		ids = m_pParentMgr->AllControlDisplay( sCMAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		

		if ( nIndex >= 0 )
		{
			pGrid->SetReverse(nIndex);
		}		
	}
	
	return 0;

EFS_END
return -1;
}
INT BL_LOC_USER_PROC::SearchAddLoanUser(CString strID, CString strCivilNo)
{
EFS_BEGIN

	CString sWhere;	
	CString strTemp;
	INT		Count;
	INT		i;
	INT		ids;
	INT		nIndex;
	CString strFuncName = _T("SearchAddLoanUser");

	CString sDMAlias = _T("DM_BO_LOC_3300_LIST");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	nIndex = pDM->GetRowCount();

	CString sCMAlias = _T("CM_BO_LOC_3300");
	CString	sGridAlias = _T("MainGrid");
	CESL_Grid* pGrid = (CESL_Grid*)m_pParentMgr->FindControl( sCMAlias , sGridAlias );
	if( pGrid == NULL ) return 0;

	
	for(i = 0; i < nIndex; i++)
	{
		if ( strCivilNo != _T("") )
		{
			strTemp = pDM->GetCellData(_T("주민등록번호"), i);
			if ( strCivilNo == strTemp )
			{
				pGrid->SetReverse(i);
				return 0;
			}
		}
		else if ( strID != _T("") )
		{
			strTemp = pDM->GetCellData(_T("사용자ID"), i);
			if ( strID == strTemp )
			{
				pGrid->SetReverse(i);
				return 0;
			}
		}
	}

	
	if ( _T("") != strID )
	{
		sWhere.Format(_T("USER_ID='%s'"), strID);
	}

	if ( _T("") != strCivilNo )
	{	
		
		if(_T("Y")==m_sIsUnityLoanService)
		{
			m_pParentMgr->ESL_DES_Encryption(strCivilNo,strCivilNo);
			sWhere.Format(_T("CRYPTO_CIVIL_NO='%s'"), strCivilNo);
		}
		else
		{
			
			if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
			{
				CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(m_pParentMgr);
				if(NULL != pApi)
				{
					if(TRUE == pApi->ConnectSocket()) 
					{
						CStringArray DataArray;
						DataArray.Add(strCivilNo);
						if(TRUE == pApi->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), DataArray))
						{
							strCivilNo = DataArray.GetAt(0);
						}
						DataArray.RemoveAll(); DataArray.FreeExtra();
						pApi->CloseSocket();	
					}
					delete pApi;
					pApi = NULL;
				}
			}
			sWhere.Format(_T("CIVIL_NO='%s'"), strCivilNo);
		}
	}	

	CESL_DataMgr TmpDM;
	CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , &TmpDM );		
	TmpDM.RefreshDataManager(sWhere);	

	
	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse && 0 < TmpDM.GetRowCount())
	{
		CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6;
		CString strGetData;
		for(i = 0; i < TmpDM.GetRowCount(); i++)
		{
			TmpDM.GetCellData(_T("주민등록번호"), i, strGetData);
			asDes1.Add(strGetData);
			TmpDM.GetCellData(_T("E_MAIL"), i, strGetData);
			asDes2.Add(strGetData);
			TmpDM.GetCellData(_T("자택전화"), i, strGetData);
			asDes3.Add(strGetData);
			TmpDM.GetCellData(_T("집주소"), i, strGetData);
			asDes4.Add(strGetData);
			TmpDM.GetCellData(_T("핸드폰"), i, strGetData);
			asDes5.Add(strGetData);
			TmpDM.GetCellData(_T("제2연락처"), i, strGetData);
			asDes6.Add(strGetData);
		}
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(m_pParentMgr);
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
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes2);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes3);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes4);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes5);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("SECOND_PHONE"), asDes6);
					pApi->CloseSocket();	
				}
				for(i = 0; i < TmpDM.GetRowCount(); i++)
				{
					TmpDM.SetCellData(_T("주민등록번호"), asDes1.GetAt(i), i);
					TmpDM.SetCellData(_T("E_MAIL"), asDes2.GetAt(i), i);
					TmpDM.SetCellData(_T("자택전화"), asDes3.GetAt(i), i);
					TmpDM.SetCellData(_T("집주소"), asDes4.GetAt(i), i);
					TmpDM.SetCellData(_T("핸드폰"), asDes5.GetAt(i), i);
					TmpDM.SetCellData(_T("제2연락처"), asDes6.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
		asDes6.RemoveAll(); asDes6.FreeExtra();
	}

	
	
 	ids = CLocCommonAPI::ChangeSecuriyCivilNo( m_pParentMgr , &TmpDM , -1 , _T("주민등록번호") , m_sIsUnityLoanService);
 	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);



	ids = CLocCommonAPI::AddDM( &TmpDM ,pDM , m_pParentMgr);	
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	Count = TmpDM.GetRowCount();			

	if ( Count > 0 )
	{		
		ids = UserCodeToDesc( sDMAlias );
		if( ids < 0 ) return ids;

		ids = m_pParentMgr->AllControlDisplay( sCMAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		

		if ( nIndex >= 0 )
		{
			pGrid->SetReverse(nIndex);
		}		
	}
	
	return 0;

EFS_END
return -1;
}


INT BL_LOC_USER_PROC::GetSearchWhereSql( CString &sWhere )
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetSearchWhereSql");
	
	INT nSFieldCnt = 0;

	CString sCMAlias = _T("CM_BO_LOC_3310");
	
	sWhere.Empty();

	
	const INT nControlCnt = 39;
	TCHAR *sControlAlias[nControlCnt] =
	{
		_T("소속")			, _T("직급")			, _T("회원상태")		, _T("주민등록번호")	, _T("대출자번호1")		, 
		_T("대출자번호2")	, _T("대출자명1")		, _T("대출자명2")		, _T("등록일자1")		, _T("등록일자2")		, 
		_T("최종대출일자1") , _T("최종대출일자2")	, _T("누적연체일1")		, _T("누적연체일2")		, _T("미대출기간1")		, 
		_T("미대출기간2")	, _T("대출정지일자1")	, _T("대출정지일자2")	, _T("누적연체횟수1")	, _T("누적연체횟수2")	, 
		_T("E_MAIL")		, _T("대출횟수")		, _T("대출횟수이상이하"), _T("가족ID")			, _T("핸드폰")			, 
		_T("집전화")		, _T("대출자생일1")		, _T("대출자생일2")		, _T("자료실")			, _T("회원관리구분")	, 
		_T("집주소")		, _T("최종수정일1")		, _T("최종수정일2")		, _T("회원구분")		, _T("개인정보제공재동의일자1") , 
		_T("개인정보제공재동의일자2")	, _T("파기대상자1")		, _T("파기대상자2") 
		
		,_T("이용자ID") 
	};
	
	CString sGetData[ nControlCnt ];

	
	CIndex index;
	for( INT i = 0 ; i < nControlCnt ; i++ )
	{
		ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		sGetData[i].TrimRight();
		
		
		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse && _T("집주소") == sControlAlias[i]) continue;

		
		index.RemoveSpace( sGetData[i] );
		index.RemoveChar( sGetData[i], '\'' );
	}
	
	
	
	sGetData[ 24 ].TrimLeft();
	sGetData[ 24 ].TrimRight();
	sGetData[ 24 ].Replace( _T(" "), _T("") );
	sGetData[ 24 ].Replace( _T("-"), _T("") );
	
	if( !sGetData[4].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[5].IsEmpty() )
			sWhere += _T(" USER_NO BETWEEN '") + sGetData[4] + _T("' AND '") + sGetData[5]+ _T("' ");
		else
			sWhere += _T(" USER_NO = '") + sGetData[4] + _T("' ");

		nSFieldCnt++;
	}

	
	if( !sGetData[6].IsEmpty() )
	{		
		sGetData[6].MakeUpper();
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[7].IsEmpty() )
		{			
			sGetData[7].MakeUpper();
			sWhere += _T(" IDX_USER_NAME BETWEEN '") + sGetData[6] + _T("' AND '") + sGetData[7]+ _T("' ");
		}
		else
		{			
			sWhere += _T(" IDX_USER_NAME BETWEEN '") + sGetData[6] + _T("' AND '") + sGetData[6]+ _T("힝' ");			
		}
		nSFieldCnt++;
	}
	
	if( !sGetData[38].IsEmpty() )
	{		
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		
		sWhere += _T(" CLU.USER_ID LIKE '") + sGetData[38] + _T("%' ");
		
		nSFieldCnt++;
	}
	
	
	CString sBuf, sTmpWhere;
	if( !sGetData[3].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sGetData[ 3 ];
		TCHAR cTmp;
		for( INT j = 0; j < sGetData[ 3 ].GetLength(); j++ )
		{
			cTmp = sGetData[ 3 ].GetAt( j );
			if( cTmp > 47 && cTmp < 58 )
				sBuf += cTmp;
		}
		
		
		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
		{
			CString strData = sGetData[3];
			CString strEnc1, strEnc2;
			if(13 == strData.GetLength())
			{
				strEnc1 = strData;
				strEnc2 = strData.Left(6) + _T("-") + strData.Right(7);
			}
			else
			{
				strEnc1 = strData;
				strEnc2.Empty();
			}
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
			if(NULL != pApi)
			{
				CStringArray DataArray;
				DataArray.Add(strEnc1);
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), DataArray);
					pApi->CloseSocket();
				}
				strEnc1 = DataArray.GetAt(0);
				DataArray.RemoveAll(); DataArray.FreeExtra();
				if(!strEnc2.IsEmpty())
				{
					DataArray.Add(strEnc2);
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), DataArray);
						pApi->CloseSocket();
					}
					strEnc2 = DataArray.GetAt(0);
					DataArray.RemoveAll(); DataArray.FreeExtra();
				}	
				delete pApi;
				pApi = NULL;
			}
			if(!strEnc2.IsEmpty())
			{
				sTmpWhere.Format(_T(" CIVIL_NO IN('%s','%s')"), strEnc1, strEnc2);
			}
			else
			{
				sTmpWhere.Format(_T(" CIVIL_NO='%s' "), strEnc1);
			}
		}
		else
		{
			if( 13 == sBuf.GetLength() )
			{
				sTmpWhere.Format( _T(" CIVIL_NO IN( '%s', '%s-%s' ) "), sBuf, sBuf.Left( 6 ), sBuf.Right( 7 ) );
			}
			else
			{
				sTmpWhere.Format( _T(" CIVIL_NO LIKE '%s%%' "), sBuf );
			}
			if(_T("Y")==m_sIsUnityLoanService)
			{
				sTmpWhere.Format( _T(" CIVIL_NO LIKE '%s%%' "), sGetData[3] );
			}
		}

		sWhere += sTmpWhere;
	}

	
	if( !sGetData[26].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[27].IsEmpty() )
			sWhere += _T(" CLU.BIRTHDAY BETWEEN TO_DATE('") + sGetData[26] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + sGetData[27] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		else
			sWhere += _T(" CLU.BIRTHDAY BETWEEN TO_DATE('") + sGetData[26] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + sGetData[26] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");

		nSFieldCnt++;
	}

	
	CString sModeAlias;
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Control_Element *pCE = pCM->FindControlElement( _T("윈도우모드") );
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	(m_pParentMgr->GetDlgItem( pCE->CTRL_ID ))->GetWindowText( sModeAlias );
	
	if( sModeAlias.Compare( _T("상세검색") ) == 0 )
	{
		if( nSFieldCnt == 0 )
		{
			ids = MessageBox( m_pParentMgr->m_hWnd , _T("검색 조건이 입력되지 않아 검색 시간이 오래 걸릴 수 있습니다.\r\n")
													 _T("무시하고 검색하시겠습니까? \r\n( 예 : 검색 , 아니오 : 검색 중지 ) ")
												   , _T("이용자 검색 경고") , MB_ICONWARNING | MB_YESNO );
			if( ids == IDNO )
				return 1;
		}

		return 0;
	}

	
	
	if( !sGetData[ 24 ].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		
		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
		{
			CString strEnc = sGetData[24];
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
			if(NULL != pApi)
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					CStringArray DataArray;
					DataArray.Add(strEnc);
					if(TRUE == pApi->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("IDX_HANDPHONE"), DataArray))
					{
						strEnc = DataArray.GetAt(0);
					}
					DataArray.RemoveAll();
					DataArray.FreeExtra();
					pApi->CloseSocket();
				}
				delete pApi;
				pApi = NULL;
			}
			sWhere += _T("IDX_HANDPHONE='") + strEnc + _T("'");
		}
		else
		{
			sWhere += _T("IDX_HANDPHONE LIKE '%") + sGetData[ 24 ] + _T("%'");
		}
	}

	
	sGetData[ 25 ].TrimLeft();
	sGetData[ 25 ].TrimRight();
	if( !sGetData[ 25 ].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		
		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
		{
			CString strEnc = sGetData[25];
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
			if(NULL != pApi)
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					CStringArray DataArray;
					DataArray.Add(strEnc);
					if(TRUE == pApi->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), DataArray))
					{
						strEnc = DataArray.GetAt(0);
					}
					DataArray.RemoveAll();
					DataArray.FreeExtra();
					pApi->CloseSocket();
				}
				delete pApi;
				pApi = NULL;
			}
			sWhere += _T("H_PHONE='") + strEnc + _T("'");
		}
		else
		{
			sWhere += _T("H_PHONE LIKE '%") + sGetData[ 25 ] + _T("%'");
		}
	}

	
	sGetData[ 30 ].TrimLeft();
	sGetData[ 30 ].TrimRight();
	if( !sGetData[ 30 ].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		
		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
		{
			CString strData;
			strData.Format(_T("H_ZIPCODE IN (SELECT ZIP_CODE FROM CO_ZIP_CODE_TBL WHERE SIDO LIKE '%%%s%%' OR GUGUN LIKE '%%%s%%' OR DONG LIKE '%%%s%%')"), sGetData[30], sGetData[30], sGetData[30]);
			sWhere += strData;
		}
		else
		{
			sWhere += _T("H_ADDR1 LIKE '%") + sGetData[ 30 ] + _T("%'");
		}

		nSFieldCnt++;
	}

	
	sGetData[0].TrimRight();
	if( sGetData[0].Compare(_T("0")) != 0 && !sGetData[0].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" USER_POSITION_CODE = '") + sGetData[0] + _T("' ");
	}

	
	sGetData[28].TrimRight();
	if( sGetData[28].Compare(_T("0")) != 0 && !sGetData[28].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" SHELF_LOC_CODE = '") + sGetData[28] + _T("' ");
	}

	
	if( sGetData[1].Compare(_T("0")) != 0 && !sGetData[1].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" USER_CLASS_CODE = '") + sGetData[1] + _T("' ");
	}

	
	if( sGetData[29].Compare(_T("0")) != 0 && !sGetData[29].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" MANAGE_CODE = '") + sGetData[29] + _T("' ");
	}

	
	if( sGetData[2].Compare(_T("0")) != 0 && !sGetData[2].IsEmpty() && (sGetData[2].Compare(_T("적용안함"))) != 0)
	{
		CString sCode;
		if( sGetData[2].Compare(_T("정상")) == 0 || (sGetData[2].Compare(_T("정상회원")) == 0) ) 
		{
			sCode = _T("0");
		}
		else if( sGetData[2].Compare(_T("대출정지")) == 0 ) 
		{
			sCode = _T("1");
		}
		else if( sGetData[2].Compare(_T("제적")) == 0 || (sGetData[2].Compare(_T("제적회원")) == 0) ) 
		{
			sCode = _T("2");
		}
		
		else if( sGetData[2].Compare(_T("탈퇴")) == 0 || (sGetData[2].Compare(_T("탈퇴회원")) == 0) ) 
		{
			sCode = _T("3");
		}

		if( sWhere.GetLength() > 1 ) 
		{
			sWhere += _T(" AND ");
		}

		sWhere += _T(" USER_CLASS = '") + sCode + _T("' ");
	}

	
	if( 0 != sGetData[33].Compare(_T("0")) && TRUE != sGetData[33].IsEmpty() && ( 0 != sGetData[33].Compare(_T("적용안함"))) )
	{
		CString sCode;
		if( 0 == sGetData[33].Compare( _T("정회원")) ) 
		{
			sCode = _T("0");
		}
		else if( 0 == sGetData[33].Compare( _T("비회원")) ) 
		{
			sCode = _T("1");
		}
		else if( 0 == sGetData[33].Compare( _T("준회원")) ) 
		{
			sCode = _T("2");
		}

		if( 1 < sWhere.GetLength() ) 
		{
			sWhere += _T(" AND ");
		}

		sWhere += _T(" MEMBER_CLASS = '") + sCode + _T("' ");
	}
	
	
	if( !sGetData[8].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[9].IsEmpty() )
			sWhere += _T(" REG_DATE BETWEEN TO_DATE('") + sGetData[8] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + sGetData[9] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		else
			sWhere += _T(" REG_DATE BETWEEN TO_DATE('") + sGetData[8] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + sGetData[8] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");

		nSFieldCnt++;
	}

	
	if( !sGetData[10].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[11].IsEmpty() )
			sWhere += _T(" LAST_LOAN_DATE BETWEEN TO_DATE('") + sGetData[10] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + sGetData[11] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		else
			sWhere += _T(" LAST_LOAN_DATE BETWEEN TO_DATE('") + sGetData[10] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + sGetData[10] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");

		nSFieldCnt++;
	}

	
	
	if( !sGetData[18].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[19].IsEmpty() )
			sWhere += _T(" DELAY_COUNT BETWEEN ") + sGetData[18] + _T(" AND ") + sGetData[19]+ _T(" ");
		else
			sWhere += _T(" DELAY_COUNT = ") + sGetData[18] + _T(" ");

		nSFieldCnt++;
		
	}
	
	
	if( !sGetData[12].IsEmpty() )
	{
		CString sColumnName = _T("DELAYDAY_COUNT");
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[13].IsEmpty() )
			sWhere += sColumnName +
					CString(_T(" BETWEEN ")) + sGetData[12] + _T(" AND ") + sGetData[13]+ _T(" ");
		else
			sWhere += sColumnName + _T(" = ") + sGetData[12] + _T(" ");

		nSFieldCnt++;
	}

	
	if( !sGetData[20].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

		
		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
		{
			CString strEnc = sGetData[20];
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
			if(NULL != pApi)
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					CStringArray DataArray;
					DataArray.Add(strEnc);
					if(TRUE == pApi->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), DataArray))
					{
						strEnc = DataArray.GetAt(0);
					}
					DataArray.RemoveAll();
					DataArray.FreeExtra();
					pApi->CloseSocket();
				}
				delete pApi;
				pApi = NULL;
			}
			sWhere += _T("E_MAIL='") + strEnc + _T("'");
		}
		else
		{
			sWhere += _T(" E_MAIL LIKE '") + sGetData[20] + _T("%%' ");
		}

		nSFieldCnt++;
	}
	
	
	
	
	INT nLoanCnt = _ttoi(sGetData[21]);	
	if( sGetData[22].Compare(_T("이상")) == 0 )	
		sGetData[22] = _T(">=");
	else
		sGetData[22] = _T("<=");

	if( !sGetData[14].IsEmpty() )
	{
		if( nLoanCnt == 0 )
		{
			sGetData[21] = _T("0");
			sGetData[22] = _T("=");
		}

		CString sDefaultSql = _T(" ( (SELECT COUNT(*) FROM LH_STORAGE_V01 CL WHERE CL.STATUS IN ('0','1','2') AND CL.USER_KEY=CLU.REC_KEY ");
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[15].IsEmpty() )
			sWhere += sDefaultSql + _T(" AND CL.LOAN_DATE BETWEEN TO_DATE('")+sGetData[14]+_T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('")+sGetData[15]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ) ") + sGetData[22] + sGetData[21] + _T(") ");
		else
			sWhere += sDefaultSql + _T(" AND CL.LOAN_DATE BETWEEN TO_DATE('")+sGetData[14]+_T(" 00:00:00') AND TO_DATE('")+sGetData[14]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') )") + sGetData[22] + sGetData[21] + _T(") ");

		
		if( !sGetData[15].IsEmpty() )
		{
			sWhere += _T(" AND CLU.REG_DATE <= TO_DATE('") + sGetData[15] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}
		else
		{
			sWhere += _T(" AND CLU.REG_DATE <= TO_DATE('") + sGetData[14] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}

		nSFieldCnt++;	
	}
	else
	{
		if( !sGetData[21].IsEmpty() )
		{
			CString sDefaultSql = _T(" ( (SELECT COUNT(*) FROM LH_STORAGE_V01 CL WHERE CL.STATUS IN ('0','1','2') AND CL.USER_KEY=CLU.REC_KEY ");
			if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
			if( !sGetData[15].IsEmpty() )
				sWhere += sDefaultSql + _T(")") + sGetData[22] + sGetData[21] + _T(") ");
			else
				sWhere += sDefaultSql + _T(")") + sGetData[22] + sGetData[21] + _T(") ");

			nSFieldCnt++;
		}
	}

	
	if( !sGetData[16].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[17].IsEmpty() )
			sWhere += _T(" LOAN_STOP_DATE BETWEEN TO_DATE('") + sGetData[16] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + sGetData[17] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		else
			sWhere += _T(" LOAN_STOP_DATE BETWEEN TO_DATE('") + sGetData[16] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + sGetData[16] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");

		nSFieldCnt++;
	}

	
	if( !sGetData[31].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[32].IsEmpty() )
			sWhere += _T(" LAST_MODIFY_DATE BETWEEN TO_DATE('") + sGetData[31] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + sGetData[32] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		else
			sWhere += _T(" LAST_MODIFY_DATE BETWEEN TO_DATE('") + sGetData[31] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + sGetData[31] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");

		nSFieldCnt++;
	}

	
	if(TRUE == m_pParentMgr->m_pInfo->m_bCivilNoSearch)
	{
		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse) if(NULL == m_p3000Api) m_p3000Api = new CBO_LOC_3000Api(m_pParentMgr);
		CESL_ControlMgr *pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3310"));
		CESL_Control_Element *pCE = pCM->FindControlElement(_T("대출자번호가주민번호"));
		if(TRUE == ((CButton*)m_pParentMgr->GetDlgItem(pCE->CTRL_ID))->GetCheck())
		{
			if(1 < sWhere.GetLength()) sWhere += _T(" AND ");
			if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
			{
				if(TRUE == m_p3000Api->CheckTarget(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO")))
				{
					sWhere += _T("REPLACE(damo.dec_varchar('KOLAS','CO_LOAN_USER_TBL','CIVIL_NO',CIVIL_NO),'-','')=USER_NO");
				}
				else
				{
					sWhere += _T("REPLACE(CIVIL_NO,'-','')=USER_NO");
				}
			}
			else
			{
				sWhere += _T("REPLACE(CIVIL_NO,'-','')=USER_NO");
			}
			nSFieldCnt++;
		}
		pCE = pCM->FindControlElement(_T("비밀번호가주민번호"));
		if(TRUE == ((CButton*)m_pParentMgr->GetDlgItem(pCE->CTRL_ID))->GetCheck())
		{
			if(1 < sWhere.GetLength()) sWhere += _T(" AND ");
			if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
			{
				BOOL bCheck1 = FALSE;
				bCheck1 = m_p3000Api->CheckTarget(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"));
				BOOL bCheck2 = FALSE;
				bCheck2 = m_p3000Api->CheckTarget(_T("CO_LOAN_USER_TBL"), _T("PASSWORD"));
				if(TRUE == bCheck1 && TRUE == bCheck2)
				{
					sWhere += _T("SUBSTR(damo.dec_varchar('KOLAS','CO_LOAN_USER_TBL','CIVIL_NO',CIVIL_NO),8,7)=damo.dec_varchar('KOLAS','CO_LOAN_USER_TBL','PASSWORD',PASSWORD)");
				}
				else if(TRUE == bCheck1)
				{
					sWhere += _T("SUBSTR(damo.dec_varchar('KOLAS','CO_LOAN_USER_TBL','CIVIL_NO',CIVIL_NO),8,7)=PASSWORD");
				}
				else if(TRUE == bCheck2)
				{
					sWhere += _T("SUBSTR(CIVIL_NO,8,7)=damo.dec_varchar('KOLAS','CO_LOAN_USER_TBL','PASSWORD',PASSWORD)");
				}
				else
				{
					sWhere += _T("SUBSTR(CIVIL_NO,8,7)=PASSWORD");
				}
			}
			else
			{
				sWhere += _T("SUBSTR(CIVIL_NO,8,7)=PASSWORD");
			}
			nSFieldCnt++;
		}
		if(NULL != m_p3000Api)
		{
			delete m_p3000Api;
			m_p3000Api = NULL;
		}
	}

	if( !sGetData[23].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" (SELECT  MAX(ID) FROM CO_LOAN_FAMILY_ID_TBL CLFI WHERE CLFI.REC_KEY = CLU.FID_KEY ) LIKE ('") + sGetData[23] + _T("%') ");
		nSFieldCnt++;
	}

	
	if( !sGetData[34].IsEmpty() )
	{
		CString sDay;
		ids = m_pParentMgr->GetManageValue( _T("열람") , _T("공통") , _T("개인정보보호") , _T("재동의대상기간") , sDay );
		if(0 != ids) 
		{
			sDay = _T("90");
		}

		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[35].IsEmpty() )			
			sWhere += _T(" REC_KEY IN (SELECT USER_KEY FROM CO_AGREEMENT_INFO_TBL WHERE EXPIRE_DATE BETWEEN TO_DATE('") + sGetData[34] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + sGetData[35] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS')+") + sDay + _T(" AND AGREE_YN = 'Y' AND DECODE(STATEMENT_ALIAS,  '책이음서비스', 'Y',  '개인정보 수집/이용', 'N') = DECODE(KL_MEMBER_YN, 'Y', 'Y', 'N', 'N', NULL, 'N', '', 'N') )");			
		else			
			sWhere += _T(" REC_KEY IN (SELECT USER_KEY FROM CO_AGREEMENT_INFO_TBL WHERE EXPIRE_DATE BETWEEN TO_DATE('") + sGetData[34] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('") + sGetData[34] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS')+") + sDay + _T(" AND AGREE_YN = 'Y' AND DECODE(STATEMENT_ALIAS,  '책이음서비스', 'Y',  '개인정보 수집/이용', 'N') = DECODE(KL_MEMBER_YN, 'Y', 'Y', 'N', 'N', NULL, 'N', '', 'N') )");		

		nSFieldCnt++;
	}

	
	if( !sGetData[36].IsEmpty() )
	{	
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[37].IsEmpty() )
			sWhere += _T(" (( REC_KEY IN (SELECT USER_KEY FROM CO_AGREEMENT_INFO_TBL WHERE (EXPIRE_DATE BETWEEN TO_DATE('") + sGetData[36] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS')-1 AND TO_DATE('") + sGetData[37] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS')-1 AND AGREE_YN = 'Y' AND DECODE(STATEMENT_ALIAS,  '책이음서비스', 'Y',  '개인정보 수집/이용', 'N') = DECODE(KL_MEMBER_YN, 'Y', 'Y', 'N', 'N', NULL, 'N', '', 'N')) OR EXPIRE_DATE IS NULL ) ) OR NOT REC_KEY IN (SELECT USER_KEY FROM CO_AGREEMENT_INFO_TBL WHERE DECODE(STATEMENT_ALIAS,  '책이음서비스', 'Y',  '개인정보 수집/이용', 'N') = DECODE(KL_MEMBER_YN, 'Y', 'Y', 'N', 'N', NULL, 'N', '', 'N') )) ");
		else
			sWhere += _T(" (( REC_KEY IN (SELECT USER_KEY FROM CO_AGREEMENT_INFO_TBL WHERE (EXPIRE_DATE BETWEEN TO_DATE('") + sGetData[36] + _T(" 00:00:00','YYYY/MM/DD HH24:MI:SS')-1 AND TO_DATE('") + sGetData[36] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS')-1 AND AGREE_YN = 'Y' AND DECODE(STATEMENT_ALIAS,  '책이음서비스', 'Y',  '개인정보 수집/이용', 'N') = DECODE(KL_MEMBER_YN, 'Y', 'Y', 'N', 'N', NULL, 'N', '', 'N')) OR EXPIRE_DATE IS NULL ) ) OR NOT REC_KEY IN (SELECT USER_KEY FROM CO_AGREEMENT_INFO_TBL WHERE DECODE(STATEMENT_ALIAS,  '책이음서비스', 'Y',  '개인정보 수집/이용', 'N') = DECODE(KL_MEMBER_YN, 'Y', 'Y', 'N', 'N', NULL, 'N', '', 'N') ))");

		nSFieldCnt++;
	}

	if( nSFieldCnt == 0 )
	{
		ids = MessageBox( m_pParentMgr->m_hWnd , _T("검색 조건이 부족하여 검색 시간이 오래 걸릴 수 있습니다.\r\n")
			_T("무시하고 검색하시겠습니까? \r\n( 예 : 검색 , 아니오 : 검색 중지 ) ")
			, _T("이용자 검색 경고") , MB_ICONWARNING | MB_YESNO );
		if( ids == IDNO )
			return 1;
	}

	
	return 0;

EFS_END
return -1;
}



INT BL_LOC_USER_PROC::UserCodeToDesc( CString sDMAlias )
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UserCodeToDesc");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	
	const INT nCodeCnt = 2;
	TCHAR *sCodeAlias[nCodeCnt] =
	{
		_T("대출소속정보"), _T("대출직급정보")
	};
	TCHAR *sFieldAlias[nCodeCnt] =
	{
		_T("대출소속정보"), _T("대출직급정보")
	};
	
	for( INT i = 0 ; i < nCodeCnt ; i++ )
	{
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , sCodeAlias[i] , sFieldAlias[i] );
	}

	
	CString sCodeData, sCode, sDesc;
	for( i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("회원관리구분") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분"), sCode, sDesc );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		ids = pDM->SetCellData( _T("회원관리구분설명") , sDesc , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = pDM->GetCellData( _T("불량회원구분") , i , sCodeData );
		if( ids < 0 && ids != -4012 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		if( sCodeData.IsEmpty() || ids== -4012 ) continue;

		if( (sCodeData.Compare(_T("0"))==0) || (sCodeData.Compare(_T("정상"))==0) || (sCodeData.Compare(_T("정상회원"))==0))
			sCodeData = _T("정상회원");
		else if( (sCodeData.Compare(_T("1"))==0) || (sCodeData.Compare(_T("대출정지"))==0))
			sCodeData = _T("대출정지");
		else if( (sCodeData.Compare(_T("2"))==0) || (sCodeData.Compare(_T("제적"))==0) || (sCodeData.Compare(_T("제적회원"))==0))
			sCodeData = _T("제적회원");
		else if( (sCodeData.Compare(_T("3"))==0) || (sCodeData.Compare(_T("탈퇴"))==0) || (sCodeData.Compare(_T("탈퇴회원"))==0))
			sCodeData = _T("탈퇴회원");
		
		ids = pDM->SetCellData( _T("불량회원구분") , sCodeData , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}

	return 0;

EFS_END
return -1;
}


INT BL_LOC_USER_PROC::UserCodeToDesc( CESL_DataMgr *pDM )
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UserCodeToDesc");

	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	
	const INT nCodeCnt = 2;
	TCHAR *sCodeAlias[nCodeCnt] =
	{
		_T("대출소속정보"), _T("대출직급정보")
	};
	TCHAR *sFieldAlias[nCodeCnt] =
	{
		_T("대출소속정보"), _T("대출직급정보")
	};
	
	for( INT i = 0 ; i < nCodeCnt ; i++ )
	{
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , sCodeAlias[i] , sFieldAlias[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}

	
	CString sCodeData;
	for( i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("불량회원구분") , i , sCodeData );
		if( ids < 0 && ids != -4012 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		if( sCodeData.IsEmpty() || ids== -4012 ) continue;

		if( (sCodeData.Compare(_T("0"))==0) || (sCodeData.Compare(_T("정상"))==0) || (sCodeData.Compare(_T("정상회원"))==0))
			sCodeData = _T("정상회원");
		else if( (sCodeData.Compare(_T("1"))==0) || (sCodeData.Compare(_T("대출정지"))==0))
			sCodeData = _T("대출정지");
		else if( (sCodeData.Compare(_T("2"))==0) || (sCodeData.Compare(_T("제적"))==0) || (sCodeData.Compare(_T("제적회원"))==0))
			sCodeData = _T("제적회원");
		else if( (sCodeData.Compare(_T("3"))==0) || (sCodeData.Compare(_T("탈퇴"))==0) || (sCodeData.Compare(_T("탈퇴회원"))==0))
			sCodeData = _T("탈퇴회원");
		ids = pDM->SetCellData( _T("불량회원구분") , sCodeData , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	}

	return 0;

EFS_END
return -1;
}

INT BL_LOC_USER_PROC::GridView( INT nMode )
{
EFS_BEGIN

	INT ids;
	INT nRowIndex;
	CString strFuncName = _T("GridView");
	INT	nSrcDMCount = 1;

	CString sSrcDMAlias = _T("DM_BO_LOC_3300");
	CString sDstDMAlias = _T("DM_BO_LOC_3300_LIST");

	const INT nFieldCnt = 15;
	
	const INT nAddFieldCnt = 4;
	TCHAR *sSrcFieldAlias[nFieldCnt] = 
	{
		_T("REC_KEY"),	_T("가족KEY"),		_T("대출자번호"),		_T("이름"),			_T("주민등록번호") , 
		_T("자택전화"), _T("불량회원구분"),	_T("대출소속정보"),		_T("대출직급정보"),	_T("총대출책수") , 
		_T("E_MAIL"),	_T("등록일자"),		_T("교번/순번/군번"),	_T("회원관리구분"), _T("휴대폰")
	};
	TCHAR *sDstFieldAlias[nFieldCnt] = 
	{
		_T("REC_KEY"),	_T("가족KEY"),		_T("대출자번호"),	_T("이름"),			_T("주민등록번호") , 
		_T("자택전화"),	_T("불량회원구분"),	_T("대출소속정보"),	_T("대출직급정보"),	_T("총대출책수") , 
		_T("E_MAIL"),	_T("등록일자"),		_T("교번"),			_T("회원관리구분"), _T("핸드폰")
	};

	TCHAR *sAddSrcFieldAlias[nAddFieldCnt] = 
	{	
		_T("메일수신여부"),	_T("SMS수신여부"), _T("제2연락처")
		,_T("생일")
	};
	TCHAR *sAddDstFieldAlias[nAddFieldCnt] = 
	{
		_T("메일수신여부"),	_T("SMS수신여부"), _T("제2연락처")
		,_T("생년월일")
	};

	if( nMode == 1 )
	{	
		ids = GetRowIndex( nRowIndex );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	else if( nMode == 0 )
	{
		CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDstDMAlias );
		if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		nRowIndex = pDM->GetRowCount();

		
		nSrcDMCount = m_pParentMgr->GetRowCount(sSrcDMAlias);
		for( INT i=0; i<nSrcDMCount; i++ )
		{
			pDM->InsertRow(-1);
		}
	}
	
	
	CString strValue;
	ids = m_pParentMgr->GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	strValue.TrimLeft();		strValue.TrimRight();
	strValue.MakeUpper();

	CString sGetData;
	CString sAddGetData;
	for( INT j=0; j<nSrcDMCount; j++ )
	{
		for( INT i = 0 ; i < nFieldCnt ; i++ )
		{
			ids = m_pParentMgr->GetDataMgrData( sSrcDMAlias , sSrcFieldAlias[i] , sGetData , j );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			
			if( i == 4 )
			{
				CLocCommonAPI::ChangeSecuriyCivilNo( m_pParentMgr , sGetData , m_sIsUnityLoanService );
			}
			
			
			if( i == 11 )
			{
				CString strBuf;
				ids = m_pParentMgr->GetDataMgrData( sSrcDMAlias , _T("등록일자시간") , strBuf , j );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

				sGetData = sGetData + _T(" ") + strBuf;
			}

			ids = m_pParentMgr->SetDataMgrData( sDstDMAlias , sDstFieldAlias[i] , sGetData , nRowIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			
			if( 2 == i )
			{
				
				if( _T("Y") != strValue )
				{
					CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , sGetData , m_sIsUnityLoanService );
				}

				
				ids = m_pParentMgr->SetDataMgrData( sDstDMAlias , _T("대출자번호_VIEW") , sGetData , nRowIndex );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			}
		}

		
		
		
		for( i = 0 ; i < nAddFieldCnt ; i++ )
		{
			ids = m_pParentMgr->GetDataMgrData( sSrcDMAlias , sAddSrcFieldAlias[i] , sAddGetData , j );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			
			ids = m_pParentMgr->SetDataMgrData( sDstDMAlias , sAddDstFieldAlias[i] , sAddGetData , nRowIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}		
		
		ids = GeneralGridVIew(nRowIndex);
		if( ids < 0 ) return ids;

		nRowIndex++;
	}

	CESL_Grid *pGrid = GetGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	long nRow = pGrid->GetRow()-1;
	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3300"));
	if(NULL != pCM)
	{
		pCM->AllControlDisplay();
	}
	pGrid->SetReverse((int)nRow);

	return 0;

EFS_END
return -1;
}

INT BL_LOC_USER_PROC::GeneralGridVIew( INT nRowIndex )
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GeneralGridVIew");

	CString sDMAlias = _T("DM_BO_LOC_3300_LIST");
	
	ids = UserCodeToDesc( sDMAlias );
	if( ids < 0 ) return ids;

	CString sCMAlias = _T("CM_BO_LOC_3300");

	if( nRowIndex == -1 )
	{
		ids = m_pParentMgr->AllControlDisplay( sCMAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	else
	{
		CESL_Grid *pGrid = GetGridPtr();
		if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = CLocCommonAPI::InsertGridRow( m_pParentMgr , pGrid , nRowIndex , nRowIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	return 0;

EFS_END
return -1;
}

INT BL_LOC_USER_PROC::GetRowIndex( INT &nRowIndex )
{
EFS_BEGIN

	CString strFuncName = _T("GetRowIndex");
	CString sCMAlias = _T("CM_BO_LOC_3300");
	CString sGridAlias = _T("MainGrid");
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_Grid *pGrid = ((CESL_Grid*)(pCM->FindControl( sGridAlias )));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	nRowIndex = pGrid->GetIdx();

	return 0;

EFS_END
return -1;
}


INT BL_LOC_USER_PROC::DeleteLoanUser()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("DeleteLoanUser");

	
	ids = IsSelectedGrid();
	if( ids < 0 ) return ids;
	if( ids > 0 ) return 1;
	
	CString strMsg;
	strMsg.Format(_T("[경고] 이용자 삭제를 선택하셨습니다.\r\n")
		_T("삭제하는 이용자의 이력도 같이 삭제되어 통계에 영향을 줄 수 있습니다.\r\n")
		_T("삭제하시겠습니까?"));
	ids = MessageBox(m_pParentMgr->m_hWnd, strMsg, _T("이용자 삭제"), MB_YESNO|MB_ICONWARNING);
	if(IDYES == ids)
	{
		ids = DeleteDBProc();
		if( ids < 0 ) return ids;
	}
	
	return 0;

EFS_END
return -1;
}

INT BL_LOC_USER_PROC::IsSelectedGrid()
{
EFS_BEGIN

	CString strFuncName = _T("IsSelectedGrid");

	CESL_Grid *pGrid = GetGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pGrid->SelectMakeList();

	INT nSelectCount = pGrid->SelectGetCount();

	if( nSelectCount < 1 )
	{
		ViewMessageBox( _T("먼저 처리할 목록을 선택해주십시오") );
		return 1;
	}
	return 0;

EFS_END
return -1;
}


INT BL_LOC_USER_PROC::DeleteDBProc()
{
EFS_BEGIN

	BEGIN_SBL();

	INT ids;
	CString strFuncName = _T("DeleteDBProc");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3300_LIST") );
	if(NULL==pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CESL_Grid *pGrid = GetGridPtr();
	if(NULL==pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	ids = CheckMoveToDM();
	if(0 > ids) return ids;

	INT nDeliveryCnt = 0;
	INT nSuccessCnt = 0;
	INT nBookCnt;
	INT nIndex;
	INT nRowCount;
	CString sRecKey , sLoanBookCnt , sReserveBookCnt;
	pGrid->SelectGetTailPosition();
	nRowCount = pGrid->SelectGetCount();
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		
		ids = m_pParentMgr->GetDataMgrData( _T("DM_BO_LOC_3300_LIST"), _T("REC_KEY") , sRecKey , nIndex );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);

		
		CString strKLMemberYN;
		if(TRUE == m_pParentMgr->m_pInfo->m_bKLUse)
		{
			ids = m_pParentMgr->GetDataMgrData( _T("DM_BO_LOC_3300_LIST"), _T("통합회원전환여부") , strKLMemberYN , nIndex );
			if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);
		}
		else
		{
			strKLMemberYN = _T("N");
		}

		
		if( 10000 == m_pParentMgr->m_pInfo->MODE || 30000 == m_pParentMgr->m_pInfo->MODE )
		{
			if(_T("Y")==strKLMemberYN) 
			{
				pGrid->SelectGetPrev();
				continue;
			}
		}

		
		CString	strWhere, strValue;		
		strWhere.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 ")
						 _T("WHERE USER_KEY=%s AND STATUS IN ('0','2','3')"), sRecKey);
		ids = pDM->GetOneValueQuery(strWhere, strValue);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1090, strFuncName);
		nBookCnt = _ttoi(strValue);
		
		CString sUserNo, sUserName, sCivilNo;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3300_LIST"), _T("대출자번호"), sUserNo, nIndex);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1060, strFuncName);
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3300_LIST"), _T("이름"), sUserName, nIndex);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1070, strFuncName);
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3300_LIST"), _T("주민등록번호"), sCivilNo, nIndex);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1080, strFuncName);

		
		
		strWhere.Format( _T("SELECT COUNT(1) FROM CO_DELIVERY_TBL ")
						 _T("WHERE USER_KEY=%s AND STATUS IN ('0','1')"), sRecKey);
		ids = pDM->GetOneValueQuery( strWhere, strValue );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1090, strFuncName);

		if( _ttoi(strValue) )
		{
			nDeliveryCnt++;
			pGrid->SelectGetPrev();
			continue;
		}

		if( nBookCnt > 0 ) 
		{
			pGrid->SelectGetPrev();
			continue;
		}		
		
		CString strUseILL;
		m_pParentMgr->GetManageValue( _T("열람") , _T("공통") , _T("책두레시스템") , _T("책두레사용여부") , strUseILL);
		if(_T("Y") == strUseILL)
		{
 			CString strUserKey;
 			ids = pDM->GetCellData(_T("REC_KEY"), nIndex, strUserKey);
			if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1100, strFuncName);
			
			CString strSQLQuery;
			CString strSQLResult;

			strSQLQuery.Format(
				_T("SELECT COUNT(1) FROM ILL_TRANS_TBL WHERE USER_KEY = %s AND ")
				_T("STATE IN ('0001', '0002', '0004', '0014') AND RETURN_LIBCODE IS NULL"),
				strUserKey);

			ids = pDM->GetOneValueQuery(strSQLQuery, strSQLResult);
			if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1110, strFuncName);
			
			if(0 < _ttoi(strSQLResult)) 
			{
				pGrid->SelectGetPrev();
				continue;
			}
		}

		CKolas2up_MobileApi MobileApi;
		MobileApi.SetMode( m_pParentMgr->m_pInfo->MODE );
		MobileApi.SetDM( pDM );
		MobileApi.StartLog();

		pDM->InitDBFieldData();
		pDM->StartFrame();
		ids = MobileApi.MakeDeleteFrame( _T("CO_LOAN_USER_TBL"), _T("REC_KEY") , _T("NUMERIC") , sRecKey );		
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1120, strFuncName);
		
		ids = MobileApi.MakeInsertFrame( _T("CO_LOAN_USER_TBL"), _T("REC_KEY") , _T("NUMERIC") , sRecKey );		
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1120, strFuncName);
		
		ids = MobileApi.MakeDeleteFrame(_T("CO_LOAN_TBL"), _T("USER_KEY"), _T("NUMERIC"), sRecKey);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1130, strFuncName);
		ids = MobileApi.MakeDeleteFrame(_T("LS_WORK_T01"), _T("USER_KEY"), _T("NUMERIC"), sRecKey);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1140, strFuncName);
		
		MobileApi.AddDBFieldData(_T("WORKER_IP"), _T("STRING"), m_pParentMgr->m_pInfo->LOCAL_IP, TRUE);
		MobileApi.AddDBFieldData(_T("WORKER_ID"), _T("STRING"), m_pParentMgr->m_pInfo->USER_ID, TRUE);
		MobileApi.AddDBFieldData(_T("WORKER_NAME"), _T("STRING"), m_pParentMgr->m_pInfo->USER_NAME, TRUE);
		
		MobileApi.AddDBFieldData(_T("WORKER_SYS"), _T("STRING"), _T("KOLASIII"), TRUE);
		MobileApi.AddDBFieldData(_T("USER_KEY"), _T("NUMERIC"), sRecKey, TRUE);
		MobileApi.AddDBFieldData(_T("USER_NO"), _T("STRING"), sUserNo, TRUE);
		MobileApi.AddDBFieldData(_T("USER_NAME"), _T("STRING"), sUserName, TRUE);
		MobileApi.AddDBFieldData(_T("CIVIL_NO"), _T("STRING"), sCivilNo, TRUE);
		
		CLocCommonAPI::ChangeSecuriyCivilNo(m_pParentMgr, sCivilNo, m_sIsUnityLoanService);
		MobileApi.AddDBFieldData(_T("WORK_DATE"), _T("NUMERIC"), _T("SYSDATE"), TRUE);
		MobileApi.AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("이용자삭제"), __WFILE__, __LINE__ ), TRUE);

		
		MobileApi.AddDBFieldData(_T("USER_TYPE"), _T("STRING"), _T("U"), TRUE);
		

		ids = MobileApi.MakeInsertFrame(_T("CO_LOAN_USER_DEL_TBL"), _T("REC_KEY"), _T("NUMERIC"), _T("ESL_SEQ.NEXTVAL"));
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1150, strFuncName);		
		if(_T("Y")==strKLMemberYN)
		{
			CKL_UserManagerDlg dlg(m_pParentMgr);
			dlg.SetMode(8);
			dlg.SetUserNo(sUserNo);
			if(IDOK != dlg.DoModal())
			{
				pDM->EndFrame();
				pGrid->SelectGetPrev();
				continue;
			}
		}
		ids = MobileApi.SendFrame();
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1160, strFuncName);
		pDM->EndFrame();

		UPLOAD_SHARE_INFO UploadShareInfo;
		UploadShareInfo.nType = 0;
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
		UploadShareInfo.UploadShareLoanInfo.sType = _T("D");		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		
		UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;		
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
		
		MobileApi.EndLog();

		pDM->DeleteRow( nIndex );
		pGrid->SelectGetPrev();
		nSuccessCnt++;
	}

	ids = GeneralGridVIew();
	if(0>ids) return ids;

	CESL_DataMgr *pSBLDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3300_LIST") );
	INT nCount = pSBLDM->GetRowCount();
	END_SBL(nCount);

	CString sMsg;
	if( nSuccessCnt < nRowCount )
	{
		
		if( (10000 == m_pParentMgr->m_pInfo->MODE || 30000 == m_pParentMgr->m_pInfo->MODE) && _T("Y")==m_sIsUnityLoanService )
		{
			sMsg.Format( _T("선택하신 총 '%d'명의 이용자중 '%d'명만 삭제가 되었습니다. \r\n")						 
						 _T("삭제하지 못한 이 용자는현재 대출(배달신청중, 책두레진행중, 상호대차대출중 포함) \r\n")						 
						 _T("또는 예약중인 이용자, 통합이용자 입니다.")
						 , nRowCount , nSuccessCnt );
		}
		else
		{
			sMsg.Format( _T("선택하신 총 '%d'명의 이용자중 '%d'명만 삭제가 되었습니다. \r\n")						 
						 _T("삭제하지 못한 이용자는 현재 대출(배달신청중, 책두레진행중, 상호대차대출중 포함) \r\n")						 
						 _T("또는 예약중인 이용자입니다.")
						 , nRowCount , nSuccessCnt );
		}
	}
	else
	{
		sMsg.Format( _T("선택하신 '%d'명의 이용자가 모두 삭제되었습니다.") , nSuccessCnt );
	}
	AfxMessageBox( sMsg, MB_ICONWARNING );

	return 0;

EFS_END
return -1;
}


VOID BL_LOC_USER_PROC::ViewMessageBox( CString sMsg )
{
	MessageBox( m_pParentMgr->m_hWnd , sMsg , _T("이용자 관리") , MB_ICONINFORMATION | MB_OK );
}


CESL_Grid* BL_LOC_USER_PROC::GetGridPtr()
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


INT BL_LOC_USER_PROC::CheckMoveToDM()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("CheckMoveToDM");

	CString sDMAlias = _T("DM_BO_LOC_3300_LIST");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CESL_Grid *pGrid = GetGridPtr();
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	INT nCheckColIndex = -1;
	CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , _T("선정") , nCheckColIndex );

	CString sGetData;
	for( INT i = 0 ; i < pGrid->GetCount() ; i++ )
	{
		if( nCheckColIndex != -1 )
		{
			ids = pGrid->GetAt( i , nCheckColIndex , sGetData );
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		ids = pDM->SetCellData( _T("선정") , sGetData , i );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}

	return 0;

EFS_END
return -1;
}



INT BL_LOC_USER_PROC::RemoveLoanUser()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("IsLoanStopDate");

	
	ids = IsSelectedGrid();
	if(0>ids) return ids;
	if(0<ids) return 1;

	
	ids = RemoveDBProc();
	if(0>ids) return ids;
	
	return 0;

EFS_END
return -1;
}


INT BL_LOC_USER_PROC::RemoveDBProc()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("RemoveDBProc");
	CString sTableName = _T("CO_LOAN_USER_TBL");

	CString sDMAlias = _T("DM_BO_LOC_3300_LIST");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if(NULL==pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CESL_Grid *pGrid = GetGridPtr();
	if(NULL==pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	ids = CheckMoveToDM();
	if(0>ids) return ids;

	INT nSuccessCnt = 0;
	INT nIndex;
	INT nRowCount;
	CString sRecKey;
	CString sInferiorClass = _T("2"); 
	
	CString strMemberClass = _T("1"); 

	pGrid->SelectGetTailPosition();
	nRowCount = pGrid->SelectGetCount();

	CString sMsg;
	sMsg.Format( _T("'%d'명의 이용자가 선택되었습니다. 정말로 제적처리하시겠습니까?") , nRowCount );
	ids = MessageBox( m_pParentMgr->m_hWnd , sMsg , _T("제적처리") , MB_ICONQUESTION|MB_YESNO);
	if( ids == IDNO ) return 1;


	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		
		ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("REC_KEY") , sRecKey , nIndex );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		CKolas2up_MobileApi MobileApi;
		MobileApi.SetMode( m_pParentMgr->m_pInfo->MODE );
		MobileApi.SetDM( pDM );
		MobileApi.StartLog();

		pDM->StartFrame();
		pDM->InitDBFieldData();
		ids = pDM->AddDBFieldData( _T("USER_CLASS") , _T("STRING") , sInferiorClass );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		ids = pDM->AddDBFieldData( _T("MEMBER_CLASS") , _T("STRING") , strMemberClass );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		CString sRemoveDate = _T("SYSDATE");
		ids = pDM->AddDBFieldData( _T("REMOVE_DATE") , _T("NUMERIC") , sRemoveDate );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		MobileApi.AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
		ids = MobileApi.MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		ids = MobileApi.SendFrame();
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		pDM->EndFrame();
		MobileApi.EndLog();

		
		UPLOAD_SHARE_INFO UploadShareInfo;
		UploadShareInfo.nType = 0;
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
		UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("USER_CLASS,MEMBER_CLASS,REMOVE_DATE");
		
		UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );

		pDM->SetCellData( _T("불량회원구분") , sInferiorClass , nIndex ); 
		
		pDM->SetCellData( _T("회원구분") , sInferiorClass , nIndex );
		pGrid->SelectGetPrev();
		nSuccessCnt++;
	}

	ids = GeneralGridVIew();
	if(0>ids) return ids;

	sMsg.Format( _T("선택하신 '%d'명의 이용자가 모두 제적 처리되었습니다.") , nSuccessCnt );

	ViewMessageBox( sMsg );

	return 0;

EFS_END
return -1;
}



INT BL_LOC_USER_PROC::ChangeLoanUserValue()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ChangeLoanUserValue");
	
	ids = IsSelectedGrid();
	if( ids < 0 ) return ids;
	if( ids > 0 ) return 1;

	
	ids = ChangeDBProc();
	if( ids < 0 ) return ids;
	return 0;

EFS_END
return -1;
}



INT BL_LOC_USER_PROC::ChangeDBProc()
{
EFS_BEGIN

	CString strFuncName = _T("ChangeDBProc");

	INT ids;
	CString sTableName = _T("CO_LOAN_USER_TBL");
	CString sDMAlias = _T("DM_BO_LOC_3300_LIST");
	CString	strSerialNo;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CESL_Grid *pGrid = GetGridPtr();
	if( NULL == pGrid ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString sGetData[12];
	
	CString sCMAlias = _T("CM_BO_LOC_3370");
	
	TCHAR *sControlAlias[12] = { _T("소속체크") , _T("직급체크") , _T("연체수체크") , _T("대출정지일체크"), _T("누적연체일수") , _T("대출소속정보") , _T("대출직급정보") , _T("연체수") , _T("대출정지일"), _T("대출자번호부여체크"), _T("회원상태체크"), _T("회원상태")   };	
	
	TCHAR *sDBFieldAlias[6] = { _T("USER_POSITION_CODE") , _T("USER_CLASS_CODE") , _T("DELAY_COUNT") , _T("LOAN_STOP_DATE"), _T("DELAYDAY_COUNT"), _T("USER_NO") };
	TCHAR *sDMFieldAlias[5] = { _T("대출소속정보") , _T("대출직급정보") , _T("연체수") , _T("대출정지일"), _T("누적연체일수") };
	INT i;

	for( i = 0 ; i < 12 ; i++ ) 
	{
		ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias[i] , sGetData[i] , -1 , 0 );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		sGetData[i].TrimLeft(); sGetData[i].TrimRight();
	}

	
	ids = CheckMoveToDM();
	if( 0 > ids ) return ids;

	INT nSuccessCnt = 0;
	INT nIndex;
	INT nRowCount;
	CString sRecKey;
	CString strNumType;
	INT j;

	
	pGrid->SelectGetHeadPosition();

	ids = m_pParentMgr->GetControlData( sCMAlias, _T("이용자번호구분"), strNumType);
	nRowCount = pGrid->SelectGetCount();

	
	
	CString strKLMemberYN = _T("N");
    
	BOOL bCheck = FALSE;
	
	CString strBeforeUserNo, strAfterUserNo, strBeforeLoanStopDate, strAfterLoanStopDate;
	
	INT nNoUserSecedeCount = 0;
	
	INT nNoUserNormalCount = 0;

	for( i = 0 ; i < nRowCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
			
		if(TRUE == m_pParentMgr->m_pInfo->m_bKLUse)
		{
			ids = pDM->GetCellData(_T("통합회원전환여부"), nIndex, strKLMemberYN);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		
		if(10000 == m_pParentMgr->m_pInfo->MODE && TRUE == m_pParentMgr->m_pInfo->m_bKLUse)
		{
			if(!sGetData[9].IsEmpty())
			{
				if(_T("Y") == strKLMemberYN)
				{
					pGrid->SetAt(nIndex, pGrid->m_nSelectIdx, _T(""));
					bCheck = TRUE;
					continue;
				}
			}
		}
		if(TRUE == m_pParentMgr->m_pInfo->m_bKLUse)
		{
			
			ids = pDM->GetCellData(_T("대출자번호"), nIndex, strBeforeUserNo);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			strAfterUserNo = strBeforeUserNo;
			
			ids = pDM->GetCellData(_T("대출정지일"), nIndex, strBeforeLoanStopDate);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			strAfterLoanStopDate = strBeforeLoanStopDate;
		}
		
		ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("REC_KEY") , sRecKey , nIndex );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		CKolas2up_MobileApi MobileApi;
		MobileApi.SetMode( m_pParentMgr->m_pInfo->MODE );
		MobileApi.SetDM( pDM );
		MobileApi.StartLog();

		if ( !sGetData[ 9 ].IsEmpty() )
		{			
			GetAutoUserNo(strSerialNo, strNumType, _T("DM_BO_LOC_3300_LIST"));
		}
		else
		{		
			strSerialNo.Empty();
		}

		pDM->StartFrame();
		pDM->InitDBFieldData();

		
		if(_T("") != sGetData[10])
		{		
			CString strUserClass = _T("0");
			if(_T("정상회원") == sGetData[11]) strUserClass = _T("0");
			else if(_T("대출정지") == sGetData[11]) strUserClass = _T("1");
			else if(_T("제적회원") == sGetData[11]) strUserClass = _T("2");
			else if(_T("탈퇴회원") == sGetData[11]) strUserClass = _T("3");
			
			CString strLoanCount = _T("0");
			CString strReservCount = _T("0");
			CString strIllCount = _T("0");
			CString strDelayCount = _T("0");
			CString strLoanStopRemainDay = _T("0");
			INT nLoanStopRemainDay = 0;
			ids = pDM->GetCellData(_T("현재대출책수"), nIndex, strLoanCount);
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			ids = pDM->GetCellData(_T("현재예약책수"), nIndex, strReservCount);
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			ids = pDM->GetCellData(_T("현재책두레신청책수"), nIndex, strIllCount);
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			ids = pDM->GetCellData(_T("현재연체책수"), nIndex, strDelayCount);
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			ids = pDM->GetCellData(_T("남은대출정지일수"), nIndex, strLoanStopRemainDay);
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			
			nLoanStopRemainDay = _ttoi(strLoanStopRemainDay);

			if( ( _T("0") != strLoanCount || _T("0") != strReservCount || _T("0") != strIllCount  ) && _T("3") == strUserClass)
			{
				nNoUserSecedeCount++;
			}
			else if( (_T("0") != strDelayCount || 0 < nLoanStopRemainDay ) && _T("0") == strUserClass )
			{
				nNoUserNormalCount++;
			}
			else
			{
				pDM->AddDBFieldData( _T("USER_CLASS"), _T("STRING"), strUserClass );
				pDM->SetCellData(_T("불량회원구분"), strUserClass, nIndex);
			}
		}

		for( j = 0 ; j < 6 ; j++ )
		{
			
			if ( 5 == j )
			{
				if ( !sGetData[ 9 ].IsEmpty() )
				{					
					pDM->AddDBFieldData( _T("USER_NO"), _T("STRING"), strSerialNo );
										
					strAfterUserNo = strSerialNo;
				}				

				continue;
			}
			if ( 4 == j )
			{
				if( !sGetData[ j ].IsEmpty() )
					pDM->AddDBFieldData( sDBFieldAlias[ j ], _T("NUMERIC"), sGetData[ j ] );

				continue;	
			}
			
			if( sGetData[j].IsEmpty() ) continue;
			
			TRACE( _T("%s - %s - %s"), sGetData[ j ], sGetData[ j + 5 ], sDBFieldAlias[ j ] );
			
			if( sGetData[j+5].IsEmpty() )
			{
				ids = pDM->AddDBFieldData( sDBFieldAlias[j] , _T("NUMERIC") , _T("NULL") );	
			}
			else
			{
				ids = pDM->AddDBFieldData( sDBFieldAlias[j] , _T("STRING") , sGetData[j+5] );				
				
				if(_T("LOAN_STOP_DATE") == sDBFieldAlias[j])
				{
					strAfterLoanStopDate = sGetData[j+5];
				}
				

			}
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		MobileApi.AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
		ids = MobileApi.MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		ids = MobileApi.SendFrame();
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		pDM->EndFrame();

		MobileApi.EndLog();
				
		if(TRUE == m_pParentMgr->m_pInfo->m_bKLUse && _T("Y") == strKLMemberYN)
		{
			if(strBeforeUserNo != strAfterUserNo)
			{
				CFrameWriter Writer(m_pParentMgr->m_pInfo);
				Writer.setCommand(_T("IL_K24_LOC_B01_SERVICE"));
				Writer.addRecord();
				Writer.addElement(_T("통합대출자번호"), strBeforeUserNo);
				Writer.addElement(_T("재부여통합대출자번호"), strAfterUserNo);
				CString strSysdate;
				CTime tCurTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strSysdate);
				Writer.addElement(_T("수정일자"), strSysdate);
				Writer.addElement(_T("수정도서관부호"), m_pParentMgr->m_pInfo->LIB_CODE);
				CString strGpinHash;
				ids = pDM->GetCellData(_T("G-PINHASH인증코드"), nIndex, strGpinHash);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				Writer.addElement(_T("G-PINHASH인증코드"), strGpinHash);
				CStringArray astrFrames;
				astrFrames.Add(Writer.getFrame());
				CFrameSender Sender(m_pParentMgr->m_pInfo);
				Sender.SendTempTbl(astrFrames, m_pParentMgr->GetWorkLogMsg( _T("이용자관리일괄변경"), __WFILE__, __LINE__ ));
			}
			if(strBeforeLoanStopDate != strAfterLoanStopDate)
			{
				CFrameWriter Writer(m_pParentMgr->m_pInfo);
				Writer.setCommand(_T("IL_K28_LOC_B01_SERVICE"));
				Writer.addRecord();
				Writer.addElement(_T("통합대출자번호"), strAfterUserNo);
				CString strUserClass;
				ids = pDM->GetCellData(_T("불량회원구분"), nIndex, strUserClass);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				if(_T("2") == strUserClass || _T("제적회원") == strUserClass)
				{
					Writer.addElement(_T("회원상태"), _T("2"));
				}
				else if(_T("3") == strUserClass || _T("탈퇴회원") == strUserClass)
				{
					Writer.addElement(_T("회원상태"), _T("3"));
				}
				else
				{
					Writer.addElement(_T("회원상태"), _T("1"));
				}
				CString strMemberClass;
				ids = pDM->GetCellData(_T("회원구분"), nIndex, strMemberClass);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				Writer.addElement(_T("회원구분"), strMemberClass);
				Writer.addElement(_T("대출정지일"), strAfterLoanStopDate);
				CString strSysdate;
				CTime tCurTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strSysdate);
				Writer.addElement(_T("대출정지부여일"), strSysdate);
				Writer.addElement(_T("최종정보수정일"), strSysdate);
				Writer.addElement(_T("수정도서관부호"), m_pParentMgr->m_pInfo->LIB_CODE);
				CStringArray astrFrames;
				astrFrames.Add(Writer.getFrame());
				CFrameSender Sender(m_pParentMgr->m_pInfo);
				Sender.SendTempTbl(astrFrames, m_pParentMgr->GetWorkLogMsg( _T("이용자관리일괄변경"), __WFILE__, __LINE__ ));
			}
		}

		for( j = 0 ; j < 4 ; j++ )
		{
			
			if ( j == 3 )
			{								
				if ( !strSerialNo.IsEmpty() )
				{
					ids = pDM->SetCellData( _T("대출자번호") , strSerialNo , nIndex );
					if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					
					ids = pDM->SetCellData( _T("대출자번호_VIEW") , strSerialNo , nIndex );
					if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				}

				continue;
			}
			if( sGetData[j].IsEmpty() ) continue;
			ids = pDM->SetCellData( sDMFieldAlias[j] , sGetData[j+5] , nIndex );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		
		UPLOAD_SHARE_INFO UploadShareInfo;
		UploadShareInfo.nType = 0;
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
		UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("USER_CLASS,USER_POSITION_CODE,USER_CLASS_CODE,DELAY_COUNT,LOAN_STOP_DATE,DELAYDAY_COUNT,USER_NO");
		
		UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
		
		pGrid->SelectGetNext();
		nSuccessCnt++;
	}

	ids = GeneralGridVIew();
	if( 0 > ids ) return ids;

	CString sMsg;
	
	
	if(10000 == m_pParentMgr->m_pInfo->MODE && TRUE == bCheck)
	{
		sMsg.Format( _T("통합회원의 대출자번호는 이동도서관에서 부여할 수 없습니다.\r\n'%d'명의 이용자가 변경 처리되었습니다.") , nSuccessCnt );
		ViewMessageBox(sMsg);
	}
	else
	{
		if(0 < nNoUserSecedeCount)
		{
			sMsg.Format( _T("선택하신 '%d'명의 이용자가 모두 변경 처리되었습니다. \r\n'%d'명의 사용자는 대출, 예약, 책두레 신청내역이 존재하여 탈퇴처리되지 않았습니다.") , nSuccessCnt, nNoUserSecedeCount );
		}
		else if(0 < nNoUserNormalCount)
		{
			sMsg.Format( _T("선택하신 '%d'명의 이용자가 모두 변경 처리되었습니다. \r\n'%d'명의 사용자는 연체내역이 존재하여 정상회원으로 변경되지 않았습니다.") , nSuccessCnt, nNoUserNormalCount );
		}
		else
		{
		sMsg.Format( _T("선택하신 '%d'명의 이용자가 모두 변경 처리되었습니다.") , nSuccessCnt );
	}

	}


	ViewMessageBox( sMsg );

	return 0;

EFS_END
return -1;
}

INT BL_LOC_USER_PROC::IsAutoMode( INT &nAutoMode )
{
EFS_BEGIN
	
	CString strFuncName = _T("IsAutoMode");

	if( m_bSetedManageValue[ 0 ] )
	{
		nAutoMode = m_nAutoMode;
		return 0;
	}
		
	INT ids = 0;
	CString sValue;
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3300") );
	if(!pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	ids = m_pParentMgr->GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("대출자번호자동부여여부"), sValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	sValue.TrimLeft();
	switch( sValue.GetAt( 0 ) )
	{
		case 'U' :
			nAutoMode = 3;
			break;
		case 'A' :
			nAutoMode = 1;
			break;
		case 'Y' :
			nAutoMode = 2;
			break;
		case 'N' :
		default :
			nAutoMode = 0;
			break;
	}

	
	if(TRUE == m_pParentMgr->m_pInfo->m_bCertifyUse) nAutoMode = 2;
	
	m_nAutoMode = nAutoMode;
	m_bSetedManageValue[ 0 ] = TRUE;
	
	return 0;

EFS_END
return -1;
}


INT BL_LOC_USER_PROC::GetAutoUserNo( CString &sUserNo, CString strNumType, CString strDMAlias, BOOL sUpdate )
{
EFS_BEGIN

	INT	ids;
	CString strFuncName = _T("GetAutoUserNo");

	CString sSql;
	CESL_DataMgr *pDM;
	
	
	if(strDMAlias.IsEmpty())
	{
		pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3300") );
		if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	else 
	{
		pDM = m_pParentMgr->FindDM( strDMAlias );
		if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	
	
	sSql.Format( _T("SELECT COUNT(1) FROM CO_LAST_NUMBER_TBL ")
				 _T("WHERE DIVISION_VALUE='%s' AND PUBLISH_FORM_CODE='CO' ")
				 _T("AND KIND_CODE='USR_NO' AND MANAGE_CODE=UDF_MANAGE_CODE ")
				 , strNumType );
	CString sCount;
	ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr, pDM, sSql, sCount );
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	if( _ttoi(sCount) < 1 )
	{
		CKolas2up_MobileApi MobileApi;
		MobileApi.SetMode( m_pParentMgr->m_pInfo->MODE );
		MobileApi.SetDM( pDM );
		MobileApi.StartLog();
		CString sRecKey;
		MobileApi.MakeRecKey( sRecKey );
		MobileApi.StartFrame();
		MobileApi.InitDBFieldData();
		MobileApi.AddDBFieldData( _T("PUBLISH_FORM_CODE"),	_T("STRING"), _T("CO") );
		MobileApi.AddDBFieldData( _T("KIND_CODE"),			_T("STRING"), _T("USR_NO") );
		MobileApi.AddDBFieldData( _T("DIVISION_VALUE"),		_T("STRING"), strNumType );
		MobileApi.AddDBFieldData( _T("LAST_NUMBER"),		_T("STRING"), _T("0") );
		MobileApi.AddDBFieldData( _T("MANAGE_CODE"),		_T("STRING"), _T("UDF_MANAGE_CODE") );
		MobileApi.AddDBFieldData( _T("FIRST_WORK"),			_T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
		MobileApi.MakeInsertFrame( _T("CO_LAST_NUMBER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey );
		MobileApi.SendFrame();
		MobileApi.EndFrame();
		MobileApi.EndLog();
	}	

	
	CString sLastNumber;
	
	if( sUpdate == TRUE )
	{
		
		
		sSql.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
					 _T("WHERE DIVISION_VALUE='%s' AND PUBLISH_FORM_CODE='CO' AND KIND_CODE='USR_NO' ")
					 _T("AND MANAGE_CODE=UDF_MANAGE_CODE FOR UPDATE")
					 , strNumType );
		ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr, pDM, sSql, sLastNumber );
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		
		
		ids = 1;
		INT nLastNumber = _ttoi(sLastNumber);
		while(TRUE)
		{
			nLastNumber++;
			ids = GetUserNoDuplicate(pDM, strNumType, nLastNumber);
			if(1>ids) break;
		}
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		
		sSql.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d ")
					 _T("WHERE PUBLISH_FORM_CODE='CO' AND KIND_CODE='USR_NO' AND DIVISION_VALUE='%s' ")
					 _T("AND MANAGE_CODE=UDF_MANAGE_CODE;")
					 , nLastNumber, strNumType );
		CKolas2up_MobileApi MobileApi;
		MobileApi.SetMode( m_pParentMgr->m_pInfo->MODE );
		MobileApi.SetDM( pDM );
		MobileApi.StartLog();
		MobileApi.StartFrame();
		MobileApi.AddFrame(sSql);
		MobileApi.SendFrame();
		MobileApi.EndFrame();
		MobileApi.EndLog();

		CString strIpinFlag;
		GetTempData(_T("아이핀사용여부"), strIpinFlag);
		CString strMypinFlag;
		GetTempData(_T("MYPIN사용여부"), strMypinFlag);
		CString strPhoneCertifyFlag;
		GetTempData(_T("휴대폰인증사용여부"), strPhoneCertifyFlag);
		if(TRUE == m_pParentMgr->m_pInfo->m_bCertifyUse || strIpinFlag.CompareNoCase(_T("Y")) == 0 || strMypinFlag.CompareNoCase(_T("Y")) == 0 || strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0)
		{
			GetUserNoDigit(sUserNo, strNumType, nLastNumber);
		}
		else
		{
			sUserNo.Format(_T("%s%06d"), strNumType, nLastNumber);
		}
		return 0;
	}
	
	else
	{
		
		sSql.Format( _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
					 _T("WHERE DIVISION_VALUE='%s' AND PUBLISH_FORM_CODE='CO' AND KIND_CODE='USR_NO' ")
					 _T("AND MANAGE_CODE=UDF_MANAGE_CODE")
					 , strNumType );
		ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr, pDM, sSql, sLastNumber );
		if(ids<0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		
		
		ids = 1;
		INT nLastNumber = _ttoi(sLastNumber);
		while(TRUE)
		{
			nLastNumber++;
			ids = GetUserNoDuplicate(pDM, strNumType, nLastNumber);
			if(1>ids) break;
		}
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		CString strIpinFlag;
		GetTempData(_T("아이핀사용여부"), strIpinFlag);
		CString strMypinFlag;
		GetTempData(_T("MYPIN사용여부"), strMypinFlag);
		CString strPhoneCertifyFlag;
		GetTempData(_T("휴대폰인증사용여부"), strPhoneCertifyFlag);
		if(TRUE == m_pParentMgr->m_pInfo->m_bCertifyUse || strIpinFlag.CompareNoCase(_T("Y")) == 0 || strMypinFlag.CompareNoCase(_T("Y")) == 0 || strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0)
		{
			GetUserNoDigit(sUserNo, strNumType, nLastNumber);
		}
		else
		{
			sUserNo.Format(_T("%s%06d"), strNumType, nLastNumber);
		}
		return 0;
	}

	return 0;

EFS_END
return -1;
}


INT BL_LOC_USER_PROC::GetUserNoDuplicate( CESL_DataMgr *pDM, CString strNumType, INT nLastNumber )
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetUserNoDuplicate");

	CString sUserNo, sSql, sCount;

		CString strIpinFlag;
		GetTempData(_T("아이핀사용여부"), strIpinFlag);
		CString strMypinFlag;
		GetTempData(_T("MYPIN사용여부"), strMypinFlag);
		CString strPhoneCertifyFlag;
		GetTempData(_T("휴대폰인증사용여부"), strPhoneCertifyFlag);
		if(TRUE == m_pParentMgr->m_pInfo->m_bCertifyUse || strIpinFlag.CompareNoCase(_T("Y")) == 0 || strMypinFlag.CompareNoCase(_T("Y")) == 0 || strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0)
	{
		GetUserNoDigit(sUserNo, strNumType, nLastNumber);
	}
	else
	{
		sUserNo.Format(_T("%s%06d"), strNumType, nLastNumber);
	}
	
	
	if(_T("Y") == m_strIsAutoSetUsernoToUserid)
	{
		sSql.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE (USER_NO='%s' OR USER_ID='%s')"), sUserNo, sUserNo);
	}
	else
	{
		sSql.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE USER_NO='%s'"), sUserNo );
	}
	ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr, pDM, sSql, sCount );
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	INT nCnt = _ttoi(sCount);
	if(0<nCnt)	return nCnt;

	return 0;

EFS_END
return -1;
}


INT BL_LOC_USER_PROC::CenterUpload()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("CenterUpload");

	
	ids = CenterUploadReady();
	if( ids != 0 ) return ids;

	
	ids = CenterUploadDBProc();
	if( ids != 0 ) return ids;
	
	return 0;

EFS_END
return -1;
}


INT BL_LOC_USER_PROC::CenterUploadReady()
{
EFS_BEGIN
	
	INT ids;
	CString strFuncName = _T("CenterUploadReady");

	CString sMode;	

	ids = GetTempData( _T("센터업로드모드") , sMode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if( _ttoi(sMode)==0 )
	{
		ids = CenterUploadReadyGridUserKey();
	}
	else
	{
		ids = CenterUploadReadyDBUserKey();		
	}
	if( ids < 0 ) return ids;
	if( ids > 0 )
	{
		AfxMessageBox( _T("센터 업로드할 자료가 존재하지 않습니다.") );
		return 1;
	}
	
	return 0;
	
EFS_END
return -1;
}

INT BL_LOC_USER_PROC::CenterUploadDBProc()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("CenterUploadDBProc");

	CString sLibKey,sLibCode;

	ids = GetTempData( _T("도서관KEY") , sLibKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	ids = GetTempData( _T("도서관부호") , sLibCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	
	CESL_DataMgr* pUserKeyDM = m_pParentMgr->FindDM( _T("DM_CENTER_UPLOAD_USER_KEY") );
	if( pUserKeyDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CCO_CENTER_USER_UPLOAD UploadDlg(m_pParentMgr,pUserKeyDM,sLibKey,sLibCode);
	UploadDlg.DoModal();
		
	return 0;
	
EFS_END
return -1;
}

INT BL_LOC_USER_PROC::CenterUploadReadyGridUserKey()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("CenterUploadReadyGridUserKey");

	CESL_Grid *pGrid = GetGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt == 0 )
		return 1;

	pGrid->SelectGetHeadPosition();

	CESL_DataMgr* pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3300_LIST"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_DataMgr* pUserKeyDM = m_pParentMgr->FindDM(_T("DM_CENTER_UPLOAD_USER_KEY"));
	if( pUserKeyDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pUserKeyDM->FreeData();

	INT nIndex;
	INT nInsertIndex;
	CString sUserKey;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		ids = pDM->GetCellData( _T("REC_KEY") , nIndex , sUserKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		pUserKeyDM->InsertRow(-1);
		nInsertIndex = pUserKeyDM->GetRowCount()-1;

		ids = pUserKeyDM->SetCellData( _T("이용자KEY") , sUserKey , nInsertIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		pGrid->SelectGetNext();
	}

	return 0;

EFS_END
return -1;
}

INT BL_LOC_USER_PROC::CenterUploadReadyDBUserKey()
{
EFS_BEGIN
		
	INT ids;
	CString strFuncName = _T("CenterUploadReadyDBUserKey");

	
	CString sCMAlias = _T("CM_BO_LOC_3398");

	CESL_ControlMgr* pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sCenterWhere;
	const INT nCenterCondCnt = 3;
	
	CString sCenterDBFieldName = _T("CENTER_UPLOAD_CHECK");
	
	CString sCenterControlAlias[nCenterCondCnt] = { _T("적용한대출자") , _T("미적용대출자") , _T("실패한대출자") };
	CString sCenterValue[nCenterCondCnt] = { _T("Y") , _T("N") , _T("F") };
	
	CButton* pCheckBox;
	CESL_Control_Element* pCE;
	for( INT i = 0 ; i < nCenterCondCnt ; i++ )
	{
		pCE = (CESL_Control_Element*)pCM->FindControlElement(sCenterControlAlias[i]);
		pCheckBox = (CButton*)(m_pParentMgr->GetDlgItem(pCE->CTRL_ID));
		
		
		if( pCheckBox->GetCheck() == 1 )
		{
			if( !sCenterWhere.IsEmpty() ) sCenterWhere += _T(" OR ");
			sCenterWhere += sCenterDBFieldName + _T(" = '") + sCenterValue[i] + _T("'");
		}
	}
	
	CESL_DataMgr* pUserKeyDM = m_pParentMgr->FindDM(_T("DM_CENTER_UPLOAD_USER_KEY"));
	if( pUserKeyDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	ids = pUserKeyDM->RefreshDataManager(sCenterWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if( pUserKeyDM->GetRowCount() < 1 ) return 1;

	
	return 0;
	
EFS_END
return -1;
}

INT BL_LOC_USER_PROC::CenterUploadOneUser()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("CenterUploadOneUser");

	CString sLibKey , sLibCode;

	
	GetTempData( _T("도서관KEY") , sLibKey );
	GetTempData( _T("도서관부호") , sLibCode );

	if( sLibKey.IsEmpty() ) return 1;
	if( sLibCode.IsEmpty() ) return 2;
	
	
	CString sLocalUserKey;
	CString sDMAlias = _T("DM_BO_LOC_3300");
	ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("REC_KEY") , sLocalUserKey , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_DataMgr* pUserKeyDM = m_pParentMgr->FindDM( _T("DM_CENTER_UPLOAD_USER_KEY") );
	if( pUserKeyDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pUserKeyDM->FreeData();

	pUserKeyDM->SetCellData( _T("이용자KEY") , sLocalUserKey , -1 );

	CCO_CENTER_USER_UPLOAD UploadUser(m_pParentMgr,pUserKeyDM,sLibKey,sLibCode);
	UploadUser.DoModal();

	return 0;

EFS_END
return -1;
}

INT BL_LOC_USER_PROC::UploadUserImage()
{
EFS_BEGIN

	
	if( m_pParentMgr->m_pInfo->MODE == 10000 || (m_pParentMgr->m_pInfo->MODE == 30000) )
		return 0;

	CString sFilePath;
	GetTempData( _T("사진경로") , sFilePath );

	CString sRecKey;
	
	CESL_DataMgr* pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3300") );
	pDM->GetCellData( _T("REC_KEY") , 0 , sRecKey );

	if( sFilePath.IsEmpty() )
	{
		pDM->StartFrame();
		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("USER_PICTURE") , _T("STRING") , _T("") );
		pDM->AddDBFieldData( _T("USER_PICTURE_TYPE") , _T("STRING") , _T("") );
		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
		pDM->MakeUpdateFrame( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		pDM->SendFrame();
		pDM->EndFrame();
		
		return 0;
	}
	
	INT ids;

	CFile file;
	ids = file.Open(sFilePath, CFile::modeRead | CFile::typeBinary);
	if( ids == 0 )
	{
		return 0;
	}
	
	INT nFileSize = file.GetLength();
	BYTE* pBuffer = (BYTE*)malloc(nFileSize+ 1024);
	file.ReadHuge(pBuffer, nFileSize);
	file.Close();

	ids = pDM->LOBUpdate( _T("CO_LOAN_USER_TBL") , _T("REC_KEY"), _T("NUMERIC") , sRecKey, _T("USER_PICTURE") , pBuffer, nFileSize);
	if (ids < 0) {
		AfxMessageBox(_T("Upload Image File Fail!!"));
		free (pBuffer);
		return -2;
	}
	free(pBuffer);	
	pDM->StartFrame();
	CString sExt = sFilePath.Right(3);
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("USER_PICTURE_TYPE") , _T("STRING") , sExt );
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	pDM->MakeUpdateFrame( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	pDM->SendFrame();
	pDM->EndFrame();

	return 0;

EFS_END
return -1;
}

CString BL_LOC_USER_PROC::GetLibCode()
{
	CString sLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	return sLibCode;
}

INT BL_LOC_USER_PROC::UploadMONETAInfo( CString informDate, CString phoneNum, CString civilNum, CString libCode )
{
	CString fileName, filePath;
	CTime time = CTime::GetCurrentTime();
	fileName.Format( _T("%06s-%04d%02d%02d-%02d%02d%02d.DAT"), libCode, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
	filePath.Format( _T("..\\tunning\\%s"), fileName );
	
	FILE *fp;
	if( !( fp = _tfopen( filePath, _T("w+b") ) ) )
		return -1;
		
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);		

	phoneNum.Replace( _T("-"), _T("") );
	civilNum.Replace( _T("-"), _T("") );
	CString civilNum_1, civilNum_2;
	civilNum_1.Format( _T("%s"), civilNum.Left( 6 ) );
	civilNum_2.Format( _T("%s"), civilNum.Right( 7 ) );	
	
	_ftprintf( fp, _T("COMMAND = 0010 ; 0010=가입, 0030=탈퇴\r\n") );
	_ftprintf( fp, _T("PHONE_NUMBER = %s ; 핸드폰번호\r\n"), phoneNum );
	_ftprintf( fp, _T("MEMBER_CODE = %06s%07s ; 주민번호\r\n"), civilNum_1, civilNum_2 );
	_ftprintf( fp, _T("DATA = %06s=0000%07s%06s ; TrackII 데이터\r\n"), civilNum_1, civilNum_2, libCode );
	fclose( fp );
	
	CString ip;
	UINT port = NULL;
	
	if( GetFTPInfo( ip, port ) )
		return -2;

	CInternetSession session;
	CFtpConnection *pFtp = NULL;

    if( !( pFtp = session.GetFtpConnection( ip, SKT_FTP_LOGIN_ID , SKT_FTP_LOGIN_PW , port, SKT_FTP_PASSIVE_YN ) ) )
		return -3;
	
	if( !pFtp->SetCurrentDirectory( SKT_FTP_UPLOAD_DIR ) )
		return -4;

	if( !pFtp->PutFile( filePath, fileName ) )
		return -5;

	return 0;
}

INT BL_LOC_USER_PROC::GetFTPInfo( CString &ip, UINT &port )
{
	CStdioFile fp;
	if( !fp.Open( SKT_CFG_FILE_NAME, CFile::modeRead | CFile::typeBinary ) )
		return -1;

		TCHAR cUni;
		fp.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fp.SeekToBegin();
		}
	
	CString str;
	while( fp.ReadString( str ) )
	{
		if( str.Find( _T("IP") ) > -1 )
		{
			if( fp.ReadString( str ) )
				ip = str;
		}
		else if( str.Find( _T("PORT") ) > -1 )
		{
			if( fp.ReadString( str ) )
				port = _ttoi( str );
		}		
	}

	return 0;
}

INT BL_LOC_USER_PROC::LoanNoReGrant()
{
	CString strFuncName = _T("LoanNoReGrant");
	CString sCMAlias = _T("CM_BO_LOC_3320");
	CString	strSerialNo;
	CString strNumType;

	CESL_DataMgr* pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3300"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	pCM->GetControlMgrData(_T("이용자번호구분"), strNumType );
	GetAutoUserNo(strSerialNo, strNumType);
	pCM->SetControlMgrData(_T("대출자번호"), strSerialNo );	
	pDM->ORACLE_Commit();

	return 0;
}


INT BL_LOC_USER_PROC::IsInputUserNo()
{
	INT ids;
	CString strFuncName = _T("IsInputUserNo");
	
	CString sCMAlias = _T("CM_BO_LOC_3320");
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( !pCM ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sUserNo;
	ids = pCM->GetControlMgrData(_T("대출자번호"), sUserNo );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	if ( sUserNo.IsEmpty() || sUserNo.Compare( _T("[자동생성]") ) == 0 || sUserNo.Compare( _T("[주민번호로생성]") ) == 0 || sUserNo.Compare( _T("[사용자ID로생성]") ) == 0 )
	{
		return 0;
	}

	
	CESL_DataMgr* pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3300") );
	if ( !pDM ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sQuery;
	CString sCnt;

	sQuery.Format( _T("SELECT COUNT(*) FROM CO_LOAN_USER_TBL WHERE USER_NO = '%s'"), sUserNo );
	pDM->GetOneValueQuery( sQuery, sCnt );

	if ( _ttoi( sCnt ) > 0 )
	{
		CString sMsg;
		
		if ( m_nAutoMode == 1 )
		{
			sMsg.Format( _T("'%s' 대출자번호는 이미 사용중입니다.\r\n대출자번호를 주민등록번호로 입력합니다."), sUserNo );
		}
		else if ( m_nAutoMode == 2 )
		{
			sMsg.Format( _T("'%s' 대출자번호는 이미 사용중입니다.\r\n대출자번호를 일려번호로 입력합니다."), sUserNo );
		}

		AfxMessageBox( sMsg);
		return 0;
	}
	
	return 1;
}

CString BL_LOC_USER_PROC::GetCompareDM(CESL_DataMgr *pSrcDM , CESL_DataMgr *pDstDM )
{
	TCHAR *sDMFieldAlias[ 51 ] =
	{
		_T("REC_KEY")		 , _T("대출자번호")		, _T("사용자ID")	  , _T("비밀번호")	  , _T("가족KEY")		 ,	
		_T("이름")			 , _T("생일")			, _T("주민등록번호")  , _T("자택주소")	  , _T("근무지주소")	 ,	
		_T("자택우편번호")	 , _T("근무지우편번호")	, _T("자택전화")	  , _T("근무지전화")  , _T("근무지내선번호") ,	
		_T("부서명")		 , _T("직급명")			, _T("휴대폰")		  , _T("E_MAIL")	  , _T("등록일자")		 , 	
		_T("불량회원구분")	 , _T("불량회원사유")	, _T("회원구분")	  , _T("학교")		  , _T("학년")			 , 	
		_T("반")			 , _T("교번/순번/군번")	, _T("대출소속정보")  , _T("대출직급정보"), _T("우편발송지역")	 , 	
		_T("총대출책수")	 , _T("총예약책수")		, _T("연체횟수")	  , _T("대출정지일")  , _T("최종대출일")	 , 	
		_T("최종수정일")	 , _T("발급횟수")		, _T("제적일자")	  , _T("비고")		  , _T("생일TYPE")		 ,  
		_T("자료실")		 , _T("회원증분실")		, _T("회원증비밀번호"), _T("근무처")	  , _T("사용자정의코드1"),  
		_T("사용자정의코드2"), _T("사용자정의코드3"), _T("학부")		  , _T("학과")		  , _T("메일수신여부")	 ,	
		_T("SMS수신여부")
	};

	CString strUpdateField[ 51 ] =
	{
		_T("REC_KEY")		   , _T("USER_NO")			, _T("USER_ID")			  , _T("PASSWORD")		 , _T("FID_KEY")		  ,
		_T("NAME")			   , _T("BIRTHDAY")			, _T("CIVIL_NO")		  , _T("H_ADDR1")		 , _T("W_ADDR1")		  , 
		_T("H_ZIPCODE")		   , _T("W_ZIPCODE")		, _T("H_PHONE")			  , _T("W_PHONE")		 , _T("EXCHANGE_NO")	  , 
		_T("DEPARTMENT_NAME")  , _T("CLASS_NAME")		, _T("HANDPHONE")		  , _T("E_MAIL")		 , _T("REG_DATE")		  , 
		_T("USER_CLASS")	   , _T("INFERIOR_REASON")	, _T("MEMBER_CLASS")	  , _T("SCHOOL")		 , _T("GRADE")			  , 
		_T("CLASS")			   , _T("CLASS_NO")			, _T("USER_POSITION_CODE"), _T("USER_CLASS_CODE"), _T("MAIL_SEND_AREA")	  , 
		_T("LOAN_COUNT")	   , _T("RESERVATION_COUNT"), _T("DELAY_COUNT")		  , _T("LOAN_STOP_DATE") , _T("LAST_LOAN_DATE")	  , 
		_T("LAST_MODIFY_DATE") , _T("REISSUE_CNT")		, _T("REMOVE_DATE")		  , _T("REMARK")		 , _T("BIRTHDAY_TYPE")	  , 
		_T("SHELF_LOC_CODE")   , _T("LOST_USER_CARD")	, _T("CARD_PASSWORD")	  , _T("OFFICE")		 , _T("USER_DEFINE_CODE1"), 
		_T("USER_DEFINE_CODE2"), _T("USER_DEFINE_CODE3"), _T("CLASS_GROUP")		  , _T("COURSE")		 , _T("MAILING_USE_YN")	  ,
		_T("SMS_USE_YN")
	};
	
	CString strSource;
	CString strDest;
	CString ChangeFields;
	CString strOldMemberClass;
	CString strOldInferiorClass;
	
	ChangeFields = _T("");	
	for ( INT i = 1 ; i < 51 ; i ++ )
	{		
		pSrcDM->GetCellData( sDMFieldAlias[i], 0, strSource );
		if ( i == 22 )
		{			
			if( strSource.Compare(_T("정회원"))==0)
			{
				strSource = _T("0");
			}
			else if( strSource.Compare(_T("비회원"))==0)
			{
				strSource = _T("1");
			}
			else
			{
				strSource = _T("0");
			}
			strOldMemberClass = strSource;
		} 
		else if ( i == 20 )
		{			
			if( strSource.Compare(_T("정상"))==0 || (strSource.Compare(_T("정상회원"))==0))
			{
				strSource = _T("0");
			}
			else if( strSource.Compare(_T("대출정지"))==0)
			{
				strSource = _T("1");
			}
			else if( strSource.Compare(_T("제적"))==0 || (strSource.Compare(_T("제적회원"))==0))
			{
				strSource = _T("2");
			}
			else
			{
				strSource = _T("0");
			}
			strOldInferiorClass = strSource;
		}
		pDstDM->GetCellData( sDMFieldAlias[i], 0, strDest );
		if ( strSource != strDest )
		{
			ChangeFields += strUpdateField[i] + _T(",");			
			if( _T("NAME") == strUpdateField[i] )
				ChangeFields += _T("IDX_USER_NAME,");
			else if( _T("HANDPHONE") == strUpdateField[i] )
				ChangeFields += _T("IDX_HANDPHONE,");
		}
	}	
	
	if ( m_BackUpDM.GetRowCount() > 0 ) m_BackUpDM.DeleteRow(0);	
	CLocCommonAPI::AddDM( pSrcDM, pDstDM , m_pParentMgr);	
	
	pDstDM->SetCellData( sDMFieldAlias[20], strOldInferiorClass, 0 ); 
	pDstDM->SetCellData( sDMFieldAlias[22], strOldMemberClass, 0 ); 
	

	if ( !ChangeFields.IsEmpty() ) ChangeFields = ChangeFields.Left(ChangeFields.GetLength()-1);
	
	return ChangeFields;
}

INT	BL_LOC_USER_PROC::InsertFamilyMember( CESL_DataMgr *pDM, CKolas2up_MobileApi* pMoblieApi )
{
	INT ids;
	CString strFuncName = _T("InsertFamilyMember");
	
    
	INT nAutoMode;
	ids = IsAutoMode( nAutoMode );
	if( ids < 0 ) return ids;

	const INT	nFieldCnt = 19;
	CString strFamilyDMFieldAlias[nFieldCnt] =
	{
		_T("대출자번호"),   _T("대출자ID"),     _T("비밀번호"),    _T("대출자명"),
		_T("IDX_대출자명"), _T("주민등록번호"), _T("생일"),        _T("생일TYPE"),
		_T("핸드폰"),       _T("IDX_핸드폰"),   _T("E_MAIL"),      _T("대출자소속"),
		_T("대출자직급"),   _T("비고"),         _T("SMS수신여부"), _T("메일수신여부"),
		_T("회원상태"),     _T("가족KEY"),		_T("발급횟수")
	};

	CString strCopyDMFieldAlias[nFieldCnt] = 
	{
		_T("대출자번호"),   _T("사용자ID"),     _T("비밀번호"),    _T("이름"),
		_T("IDX_이름"),		_T("주민등록번호"), _T("생일"),        _T("생일TYPE"),
		_T("휴대폰"),       _T("IDX_핸드폰"),   _T("E_MAIL"),      _T("대출소속정보"), 
		_T("대출직급정보"), _T("비고"),         _T("SMS수신여부"), _T("메일수신여부"),
		_T("불량회원구분"), _T("가족KEY"),		_T("발급횟수")
	};

	const int nOricinalCnt = 12;
	CString strDMFieldAlias[nOricinalCnt] =
	{
		_T("자택주소"),   _T("자택우편번호"),    _T("자택전화"),	    _T("등록일자"),
		_T("회원구분"),   _T("우편발송지역"),     _T("자료실"), 
		_T("회원증분실"), _T("사용자정의코드1"), _T("사용자정의코드2"), _T("사용자정의코드3"),
		_T("회원관리구분")
	};

	CString strCopyDMFieldName[nFieldCnt] =
	{
		_T("USER_NO"),			_T("USER_ID"),       _T("PASSWORD"),   _T("NAME"),
		_T("IDX_USER_NAME"),	_T("CIVIL_NO"),      _T("BIRTHDAY"),   _T("BIRTHDAY_TYPE"),
		_T("HANDPHONE"),        _T("IDX_HANDPHONE"), _T("E_MAIL"),     _T("USER_POSITION_CODE"),
		_T("USER_CLASS_CODE"),	_T("REMARK"),        _T("SMS_USE_YN"), _T("MAILING_USE_YN"),
		_T("USER_CLASS"),       _T("FID_KEY"),		 _T("REISSUE_CNT")
	};

	CString strDMFieldName[nOricinalCnt] =
	{
		_T("H_ADDR1"),		  _T("H_ZIPCODE"),		    _T("H_PHONE"),	         _T("REG_DATE"),
		_T("MEMBER_CLASS"),   _T("MAIL_SEND_AREA"),     /*_T("LAST_MODIFY_DATE"),*/  _T("SHELF_LOC_CODE"),
		_T("LOST_USER_CARD"), _T("USER_DEFINE_CODE1"),	_T("USER_DEFINE_CODE2"), _T("USER_DEFINE_CODE3"),
		_T("MANAGE_CODE")
	};

	CString strCopyDMFieldType[nFieldCnt] =
	{
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"),
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"),
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"),
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"),
		_T("STRING"), _T("NUMERIC"), _T("NUMERIC")
	};

	CString strDMFieldType[nOricinalCnt] =
	{
		_T("STRING"), _T("STRING"), _T("STRING"), _T("NUMERIC"),
		_T("STRING"), _T("STRING"), /*_T("STRING"),*/ _T("STRING"),
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"),
		_T("STRING")
	};

	CESL_DataMgr* pInsertDM = m_pParentMgr->FindDM(_T("DM_FAMILY_ADD_LIST"));
	if( NULL == pInsertDM ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	INT	nInsertCnt = pInsertDM->GetRowCount();
	
	
	for( INT i=0; i<nInsertCnt; i++ )
	{
		INT nGetDataIdx = pDM->GetRowCount() - 1;
		
		pDM->InsertRow(-1);

		INT nInsertIdx = pDM->GetRowCount() - 1;

		CString strData;
		for( INT j=0; j<nOricinalCnt;j++ )
		{
			ids = pDM->GetCellData(strDMFieldAlias[j], nGetDataIdx, strData);
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			
			if( _T("회원증분실") == strDMFieldAlias[j] )
			{
				strData = _T("N");	
			}
			else if( _T("등록일자") == strDMFieldAlias[j] )
			{
				strData = _T("SYSDATE");
			}
			
			else if( _T("회원구분") == strDMFieldAlias[j] )
			{
				strData = _T("0");
			}
			ids = pDM->SetCellData(strDMFieldAlias[j], strData, nInsertIdx);
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		
		for( INT k=0; k<nFieldCnt; k++ )
		{
			
			if(_T("Y") == m_strIsAutoSetUsernoToUserid)
			{
				if(0 == k)
				{
					ids = pInsertDM->GetCellData(strFamilyDMFieldAlias[0], i, strData);
					if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

					ids = pDM->SetCellData(strCopyDMFieldAlias[0], strData, nInsertIdx);
					if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					ids = pDM->SetCellData(strCopyDMFieldAlias[1], strData, nInsertIdx);
					if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

					continue;
				}
				else if(1 == k)
				{
					continue;
				}
			}
			ids = pInsertDM->GetCellData(strFamilyDMFieldAlias[k], i, strData);
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			ids = pDM->SetCellData(strCopyDMFieldAlias[k], strData, nInsertIdx);
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}		
		
		if( 2 == nAutoMode )
		{
			ids = pInsertDM->GetCellData(_T("대출자번호구분"), i, strData);
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			CString strUserNo;
			ids = GetAutoUserNo(strUserNo, strData, _T("DM_FAMILY_ADD_LIST"));
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			ids = pDM->SetCellData(_T("대출자번호"), strUserNo, nInsertIdx);
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			
			if(_T("Y") == m_strIsAutoSetUsernoToUserid)
			{
				if(3 != nAutoMode)
				{
					ids = pDM->SetCellData(_T("사용자ID"), strUserNo, nInsertIdx);
					if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				}
			}
		}

		CString strRecKey;		
		ids = pMoblieApi->MakeRecKey( strRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		ids = pDM->SetCellData( _T("REC_KEY") , strRecKey ,  nInsertIdx );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		pDM->InitDBFieldData();

		CString strGetCopyDMData[nFieldCnt];
		for( INT m=0; m<nFieldCnt; m++ )
		{
			ids = pDM->GetCellData(strCopyDMFieldAlias[m], nInsertIdx, strGetCopyDMData[m]);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			if( !strGetCopyDMData[m].IsEmpty() )
			{
				
				if(NULL != m_p3000Api && TRUE == m_bConnect)
				{
					if( _T("CARD_PASSWORD") == strCopyDMFieldName[m] || _T("CIVIL_NO") == strCopyDMFieldName[m] || _T("E_MAIL") == strCopyDMFieldName[m] || _T("HANDPHONE") == strCopyDMFieldName[m] || _T("IDX_HANDPHONE") == strCopyDMFieldName[m] || _T("PASSWORD") == strCopyDMFieldName[m])
					{
						if(TRUE == m_p3000Api->ConnectSocket())
						{
							CStringArray DataArray;
							DataArray.Add(strGetCopyDMData[m]);
							if(TRUE == m_p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), strCopyDMFieldName[m], DataArray))
							{
								strGetCopyDMData[m] = DataArray.GetAt(0);
							}
							DataArray.RemoveAll(); DataArray.FreeExtra();
							m_p3000Api->CloseSocket();
						}
					}
				}
				ids = pDM->AddDBFieldData(strCopyDMFieldName[m] , strCopyDMFieldType[m] , strGetCopyDMData[m],TRUE);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			}
		}

		CString strGetDMData[nOricinalCnt];
		for( INT n=0; n<nOricinalCnt; n++ )
		{
			ids = pDM->GetCellData(strDMFieldAlias[n], nInsertIdx, strGetDMData[n]);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			if( !strGetDMData[n].IsEmpty() )
			{
				
				if(NULL != m_p3000Api && TRUE == m_bConnect)
				{
					if(_T("H_ADDR1") == strDMFieldName[n] || _T("H_PHONE") == strDMFieldName[n])
					{
						if(TRUE == m_p3000Api->ConnectSocket())
						{
							CStringArray DataArray;
							DataArray.Add(strGetDMData[n]);
							if(TRUE == m_p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), strDMFieldName[n], DataArray))
							{
								strGetDMData[n] = DataArray.GetAt(0);
							}
							DataArray.RemoveAll(); DataArray.FreeExtra();
							m_p3000Api->CloseSocket();
						}
					}
				}
				ids = pDM->AddDBFieldData(strDMFieldName[n] , strDMFieldType[n] , strGetDMData[n],TRUE);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			}			
		}		
		ids = pDM->AddDBFieldData( _T("SELF_LOAN_STATION_LIMIT"), _T("STRING"), _T("N"), TRUE );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = pDM->AddDBFieldData( _T("CENTER_UPLOAD_CHECK") , _T("STRING") , _T("N") );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		ids = pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T(""), __WFILE__, __LINE__ ), TRUE );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);


		if (m_sIsUnityLoanService.CompareNoCase(_T("Y")) == 0)
		{
			ids = pInsertDM->GetCellData(_T("주민등록번호_CRYPTO"), i, strData);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			ids = pDM->AddDBFieldData( _T("CRYPTO_CIVIL_NO") , _T("STRING") , strData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		ids = pMoblieApi->MakeInsertFrame( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , strRecKey );		
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);


		CString strQuery;
		strQuery.Format(_T("INSERT INTO CO_LOAN_USER_EDIT_TBL")
			_T("(REC_KEY,TYPE,WORKER_IP,WORKER_ID,WORKER_NAME,WORKER_SYS,USER_KEY,WORK_DATE,FIRST_WORK , USER_TYPE) ")
			_T("VALUES(ESL_SEQ.NEXTVAL,'I','%s','%s','%s','KOLASIII',%s,SYSDATE,'%s' , 'U');")
			, m_pParentMgr->m_pInfo->LOCAL_IP
			, m_pParentMgr->m_pInfo->USER_ID
			, m_pParentMgr->m_pInfo->USER_NAME
			, strRecKey
			, m_pParentMgr->GetWorkLogMsg(_T("가족입력"),__WFILE__,__LINE__));


		pDM->AddFrame(strQuery);		
		
		pDM->DeleteRow( pDM->GetRowCount() - 1 );		
		
		UPLOAD_SHARE_INFO UploadShareInfo;
		UploadShareInfo.nType = 0;
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
		UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
		UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = strRecKey;
		UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
		m_aFamily_UPLOAD_SHARE_INFO.Add(UploadShareInfo);
	}		
	
	pInsertDM->FreeData();

	return 0;
}


INT	BL_LOC_USER_PROC::UpdateFamilyMember( CESL_DataMgr *pDM, CKolas2up_MobileApi* pMoblieApi )
{
	INT ids;
	CString strFuncName = _T("UpdateFamilyMember");
	
    
	INT nAutoMode;
	ids = IsAutoMode( nAutoMode );
	if( ids < 0 ) return ids;

	const INT	nFieldCnt = 19;
	CString strFamilyDMFieldAlias[nFieldCnt] =
	{
		_T("대출자번호"),   _T("대출자ID"),     _T("비밀번호"),    _T("대출자명"),
		_T("IDX_대출자명"), _T("주민등록번호"), _T("생일"),        _T("생일TYPE"),
		_T("핸드폰"),       _T("IDX_핸드폰"),   _T("E_MAIL"),      _T("대출자소속"),
		_T("대출자직급"),   _T("비고"),         _T("SMS수신여부"), _T("메일수신여부"),
		_T("회원상태"),     _T("가족KEY"),		_T("발급횟수")
	};

	CString strCopyDMFieldAlias[nFieldCnt] = 
	{
		_T("대출자번호"),   _T("사용자ID"),     _T("비밀번호"),    _T("이름"),
		_T("IDX_이름"),		_T("주민등록번호"), _T("생일"),        _T("생일TYPE"),
		_T("휴대폰"),       _T("IDX_핸드폰"),   _T("E_MAIL"),      _T("대출소속정보"), 
		_T("대출직급정보"), _T("비고"),         _T("SMS수신여부"), _T("메일수신여부"),
		_T("불량회원구분"), _T("가족KEY"),		_T("발급횟수")
	};

	const int nOricinalCnt = 9;
	CString strDMFieldAlias[nOricinalCnt] =
	{
		_T("자택주소"),			_T("자택우편번호"),		_T("자택전화"),		
		_T("우편발송지역"),		_T("최종수정일"),		_T("자료실"),		_T("사용자정의코드1"),	
		_T("사용자정의코드2"),	_T("사용자정의코드3")
	};

	CString strCopyDMFieldName[nFieldCnt] =
	{
		_T("USER_NO"),			_T("USER_ID"),       _T("PASSWORD"),   _T("NAME"),
		_T("IDX_USER_NAME"),	_T("CIVIL_NO"),      _T("BIRTHDAY"),   _T("BIRTHDAY_TYPE"),
		_T("HANDPHONE"),		_T("IDX_HANDPHONE"), _T("E_MAIL"),     _T("USER_POSITION_CODE"),
		_T("USER_CLASS_CODE"),	_T("REMARK"),        _T("SMS_USE_YN"), _T("MAILING_USE_YN"),
		_T("USER_CLASS"),		_T("FID_KEY"),		 _T("REISSUE_CNT")
	};

	CString strDMFieldName[nOricinalCnt] =
	{
		_T("H_ADDR1"),			 _T("H_ZIPCODE"),			_T("H_PHONE"),			
		_T("MAIL_SEND_AREA"),    _T("LAST_MODIFY_DATE"),	_T("SHELF_LOC_CODE"),	_T("USER_DEFINE_CODE1"),	
		_T("USER_DEFINE_CODE2"), _T("USER_DEFINE_CODE3")
	};

	CString strCopyDMFieldType[nFieldCnt] =
	{
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"),
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"),
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"),
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"),
		_T("STRING"), _T("NUMERIC"), _T("NUMERIC")
	};

	CString strDMFieldType[nOricinalCnt] =
	{
		_T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING")
	};

	CESL_DataMgr* pInsertDM = m_pParentMgr->FindDM(_T("DM_FAMILY_SEARCH_LIST"));
	if( NULL == pInsertDM ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	INT	nInsertCnt = pInsertDM->GetRowCount();
	
	
	for( INT i=0; i<nInsertCnt; i++ )
	{
		INT nGetDataIdx = pDM->GetRowCount() - 1;
		pDM->InsertRow(-1);

		INT nInsertIdx = pDM->GetRowCount() - 1;

		CString strData;
		for( INT j=0; j<pDM->m_nCols;j++ )
		{
			ids = pDM->GetCellData(nGetDataIdx, j, strData);
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			ids = pDM->SetCellData(nInsertIdx, j, strData);
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		
		for( INT k=0; k<nFieldCnt; k++ )
		{
			ids = pInsertDM->GetCellData(strFamilyDMFieldAlias[k], i, strData);

			if(strData == _T("MA2008000357"))
			{
				int q = 0;
			}

			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			ids = pDM->SetCellData(strCopyDMFieldAlias[k], strData, nInsertIdx);
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		
		CString strRecKey;
		pInsertDM->GetCellData(_T("REC_KEY"), i, strRecKey);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		ids = pDM->SetCellData(_T("REC_KEY"), strRecKey, nInsertIdx);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		pDM->InitDBFieldData();

		CString strGetCopyDMData[nFieldCnt];
		for( INT m=0; m<nFieldCnt; m++ )
		{
			ids = pDM->GetCellData(strCopyDMFieldAlias[m], nInsertIdx, strGetCopyDMData[m]);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			if( !strGetCopyDMData[m].IsEmpty() )
			{				
				if(NULL != m_p3000Api && TRUE == m_bConnect)
				{
					if( _T("CARD_PASSWORD") == strCopyDMFieldName[m] || _T("CIVIL_NO") == strCopyDMFieldName[m] || _T("E_MAIL") == strCopyDMFieldName[m] || _T("HANDPHONE") == strCopyDMFieldName[m] || _T("IDX_HANDPHONE") == strCopyDMFieldName[m] || _T("H_ADDR1") == strCopyDMFieldName[m] || _T("H_PHONE") == strCopyDMFieldName[m] || _T("W_ADDR1") == strCopyDMFieldName[m] || _T("W_PHONE") == strCopyDMFieldName[m] || _T("PASSWORD") == strCopyDMFieldName[m] || _T("SECOND_PHONE") == strCopyDMFieldName[m])
					{ 
						if(TRUE == m_p3000Api->ConnectSocket())
						{
							CStringArray DataArray;
							DataArray.Add(strGetCopyDMData[m]);
							if(TRUE == m_p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), strCopyDMFieldName[m], DataArray))
							{
								strGetCopyDMData[m] = DataArray.GetAt(0);
							}
							DataArray.RemoveAll(); DataArray.FreeExtra();
							m_p3000Api->CloseSocket();
						}
					}
				}
				ids = pDM->AddDBFieldData(strCopyDMFieldName[m] , strCopyDMFieldType[m] , strGetCopyDMData[m], TRUE);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			}
			else if( _T("가족KEY") == strCopyDMFieldAlias[m] && strGetCopyDMData[m].IsEmpty() )
			{
				ids = pDM->AddDBFieldData(strCopyDMFieldName[m] , strCopyDMFieldType[m] , _T("NULL"), TRUE);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			}			
		}

		CString strGetDMData[nOricinalCnt];
		for( INT n=0; n<nOricinalCnt; n++ )
		{
			ids = pDM->GetCellData(strDMFieldAlias[n], nInsertIdx, strGetDMData[n]);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			if( !strGetDMData[n].IsEmpty() )
			{
				
				if(NULL != m_p3000Api && TRUE == m_bConnect)
				{
					if( _T("CARD_PASSWORD") == strDMFieldName[n] || _T("CIVIL_NO") == strDMFieldName[n] || _T("E_MAIL") == strDMFieldName[n] || _T("HANDPHONE") == strDMFieldName[n] || _T("IDX_HANDPHONE") == strDMFieldName[n] || _T("H_ADDR1") == strDMFieldName[n] || _T("H_PHONE") == strDMFieldName[n] || _T("W_ADDR1") == strDMFieldName[n] || _T("W_PHONE") == strDMFieldName[n] || _T("PASSWORD") == strDMFieldName[n] || _T("SECOND_PHONE") == strDMFieldName[n])
					{
						if(TRUE == m_p3000Api->ConnectSocket())
						{
							CStringArray DataArray;
							DataArray.Add(strGetDMData[n]);
							if(TRUE == m_p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), strDMFieldName[n], DataArray))
							{
								strGetDMData[n] = DataArray.GetAt(0);
							}
							DataArray.RemoveAll(); DataArray.FreeExtra();
							m_p3000Api->CloseSocket();
						}
					}
				}
				ids = pDM->AddDBFieldData(strDMFieldName[n] , strDMFieldType[n] , strGetDMData[n],TRUE);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			}
		}

		ids = pMoblieApi->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T(""), __WFILE__, __LINE__ ), TRUE );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = pMoblieApi->MakeUpdateFrame( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , strRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);


		CString strQuery;
		strQuery.Format(_T("INSERT INTO CO_LOAN_USER_EDIT_TBL")
			_T("(REC_KEY,TYPE,WORKER_IP,WORKER_ID,WORKER_NAME,WORKER_SYS,USER_KEY,WORK_DATE,FIRST_WORK , USER_TYPE) ")
			_T("VALUES(ESL_SEQ.NEXTVAL,'U','%s','%s','%s','KOLASIII',%s,SYSDATE,'%s' , 'U');")
			, m_pParentMgr->m_pInfo->LOCAL_IP
			, m_pParentMgr->m_pInfo->USER_ID
			, m_pParentMgr->m_pInfo->USER_NAME
			, strRecKey
			, m_pParentMgr->GetWorkLogMsg(_T("가족수정"),__WFILE__,__LINE__));		

		pDM->AddFrame(strQuery);		
		pDM->DeleteRow( pDM->GetRowCount() - 1 );	

		
		UPLOAD_SHARE_INFO UploadShareInfo;
		UploadShareInfo.nType = 0;
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
		UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = strRecKey;
		UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
 		CString sUpdateFields;
 		for(INT nCnt = 0; nCnt < nFieldCnt; nCnt++)
 		{
 			sUpdateFields = sUpdateFields + _T(",") + strCopyDMFieldName[nCnt];
 		}		
 		sUpdateFields.TrimLeft(_T(","));
 		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = sUpdateFields;		
		m_aFamily_UPLOAD_SHARE_INFO.Add(UploadShareInfo);
	}

	pInsertDM->FreeData();


	return 0;
}

INT	BL_LOC_USER_PROC::KL_CheckUserNo( CString &strUserNo )
{
	CString strFuncName = _T("KL_CheckUserNo");

	BOOL bIsKLUserNo = TRUE;
	CString strTemp = strUserNo;
	INT nLenth = strTemp.GetLength();

	
	if(14>nLenth)
	{
		bIsKLUserNo = FALSE;
	}
	
	if(bIsKLUserNo && 5!=m_nLTMemberSaveMode)
	{
		for(INT i=0; i<nLenth; i++)
		{
			if(!isdigit(strTemp[i]))
			{
				bIsKLUserNo = FALSE;
				break;
			}
		}
	}
	if(bIsKLUserNo)
	{
		CESL_DataMgr *pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3300") );
		if(NULL==pDM) return -1010;
		CString strUserKey = pDM->GetCellData(_T("REC_KEY"),0);

		CString strQuery, strResult;
		CESL_DataMgr TempDM;
		TempDM.SetCONNECTION_INFO(_T(""));

		strQuery.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE USER_NO='%s' AND REC_KEY <> %s"), strUserNo, strUserKey);
		TempDM.GetOneValueQuery(strQuery,strResult);
		strResult.TrimLeft();	strResult.TrimRight();
		if(0<_ttoi(strResult)) bIsKLUserNo = FALSE;
	}
	
	if(!bIsKLUserNo)
	{
		CESL_ControlMgr *pCM = m_pParentMgr->FindCM( _T("CM_BO_LOC_3320") );
		if( NULL == pCM ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		CString strNumType;
		pCM->GetControlMgrData(_T("이용자번호구분"), strNumType );
		if( 0 > GetAutoUserNo( strUserNo, strNumType ) ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}

	return 0;
}


void BL_LOC_USER_PROC::GetUserNoDigit(CString& strUserNo, CString strNumType, int nLastNumber)
{
	
	BOOL bCheck = FALSE;
	if(8 == strNumType.GetLength())
	{
		bCheck = TRUE;
		for(int i = 0; i < 8; i++)
		{
			TCHAR cValue = strNumType.Mid(i, 1).GetAt(0);
			if(cValue < '0' && cValue > '9')
			{
				bCheck = FALSE;
				break;
			}
		}
	}
	if(TRUE == bCheck)
	{
		CString strData;
		strData.Format(_T("%s%05d"), strNumType, nLastNumber);
		int nSum = 0;
		const int nLength = 13;
		int nValue[nLength] = {2,3,4,5,6,7,8,9,2,3,4,5,6};
		for(int i = 0; i < nLength; i++)
		{
			nSum += _ttoi(strData.Mid(i,1)) * nValue[i];
		}
		int nDigit = 0;
		int x = (nSum % 11);
		if(0 == x) nDigit = x;
		else nDigit = (10 - x);
		strUserNo.Format(_T("%s%d"), strData, nDigit);
	}
	else
	{
		strUserNo.Format(_T("%s%06d"), strNumType, nLastNumber);
	}
}

CString BL_LOC_USER_PROC::GetCompareDM_DetailLog(CESL_DataMgr *pSrcDM , CESL_DataMgr *pDstDM )
{
	
	TCHAR *sDMFieldAlias[ 51 ] =
	{
		_T("REC_KEY")		 , _T("대출자번호")		, _T("사용자ID")	  , _T("비밀번호")	  , _T("가족KEY")		 ,	
		_T("이름")			 , _T("생일")			, _T("주민등록번호")  , _T("자택주소")	  , _T("근무지주소")	 ,	
		_T("자택우편번호")	 , _T("근무지우편번호")	, _T("자택전화")	  , _T("근무지전화")  , _T("근무지내선번호") ,	
		_T("부서명")		 , _T("직급명")			, _T("휴대폰")		  , _T("E_MAIL")	  , _T("등록일자")		 , 	
		_T("불량회원구분")	 , _T("불량회원사유")	, _T("회원구분")	  , _T("학교")		  , _T("학년")			 , 	
		_T("반")			 , _T("교번/순번/군번")	, _T("대출소속정보")  , _T("대출직급정보"), _T("우편발송지역")	 , 	
		_T("총대출책수")	 , _T("총예약책수")		, _T("연체횟수")	  , _T("대출정지일")  , _T("최종대출일")	 , 	
		_T("최종수정일")	 , _T("발급횟수")		, _T("제적일자")	  , _T("비고")		  , _T("생일TYPE")		 ,  
		_T("자료실")		 , _T("회원증분실")		, _T("회원증비밀번호"), _T("근무처")	  , _T("사용자정의코드1"),  
		_T("사용자정의코드2"), _T("사용자정의코드3"), _T("학부")		  , _T("학과")		  , _T("메일수신여부")	 ,	
		_T("SMS수신여부")
	};


	CString strSource;
	CString strDest;
	CString ChangeFields = _T("");
	CString strOldMemberClass;
	CString strOldInferiorClass;

	CString	add_str;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;

	

	ChangeFields = _T("");	
	for ( INT i = 1 ; i < 51 ; i ++ )
	{
		pSrcDM->GetCellData( sDMFieldAlias[i], 0, strSource );

		if ( i == 22 )
		{			
			if( strSource.Compare(_T("정회원"))==0)
			{
				strSource = _T("0");
			}
			else if( strSource.Compare(_T("비회원"))==0)
			{
				strSource = _T("1");
			}
			else
			{
				strSource = _T("0");
			}
			strOldMemberClass = strSource;
		} 
		else if ( i == 20 )
		{			
			if( strSource.Compare(_T("정상"))==0 || (strSource.Compare(_T("정상회원"))==0))
			{
				strSource = _T("0");
			}
			else if( strSource.Compare(_T("대출정지"))==0)
			{
				strSource = _T("1");
			}
			else if( strSource.Compare(_T("제적"))==0 || (strSource.Compare(_T("제적회원"))==0))
			{
				strSource = _T("2");
			}
			else
			{
				strSource = _T("0");
			}
			strOldInferiorClass = strSource;
		}

		pDstDM->GetCellData( sDMFieldAlias[i], 0, strDest );

		FIELD_ALIAS = sDMFieldAlias[i];
		OutputDebugString( _T("FIELD_ALIAS:") + FIELD_ALIAS + _T("   src :") + strSource + _T("   dest :") + strDest + _T("\n") );

		if ( strSource != strDest )
		{
			if( ChangeFields.GetLength() > 0 ) add_str = _T(",");
			
			add_str += FIELD_ALIAS;
			ChangeFields += add_str;
			OutputDebugString( add_str + _T("\n") );
		}
	}


	OutputDebugString( _T("ChangeFields->") + ChangeFields + _T("\n") );

	return ChangeFields;	
}