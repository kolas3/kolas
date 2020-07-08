// LonaManageValue.cpp : implementation file
//

#include "stdafx.h"
#include "LonaManageValue.h"
#include "..\..\..\..\관리\도서관관리\MNG_LIB_HOLIDAY_MGR\MNG_LIB_HOLIDAY_MGR_01.h"
#include "..\..\..\..\관리\도서관관리\FileManager\LibFileManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLonaManageValue dialog


CLonaManageValue::CLonaManageValue(CESL_Mgr* pParent,INT nMode)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLonaManageValue)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bLoad = FALSE;
	m_nMode = nMode;

	if(InitESL_Mgr(_T("SM_LOAN_MANAGE_VALUE")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return ;
	}

	m_sRegCodeDMAlias = _T("DM_등록구분");
	m_sSeCodeDMAlias = _T("DM_별치기호");
	m_sShelfLocCodeDMAlias = _T("DM_자료실");
	m_sUseLimitCodeDMAlias = _T("DM_이용제한구분");
	m_sUserPositionDMAlias = _T("DM_직급별관리구분값");
	m_sIndividualUserPositionDMAlias = _T("DM_직급별관리구분값_도서관별");
	m_sAllSettingValueDMAlias = _T("DM_잡다구리설정값");
	m_sAttestation = _T("N");
	m_strLibCodeList = _T("");

	m_sUnionLibInfoDMAlias    = _T("DM_BO_LOC_3100_LIB_INFO");
	m_pUnionLibRegLngDMAlias  = _T("DM_BO_LOC_3100_LIB_REGCODE_LENGTH");

	m_bIsLoanReturnMsgDisp = TRUE;
}

CLonaManageValue::~CLonaManageValue()
{
}	

VOID CLonaManageValue::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLonaManageValue)
	DDX_Control(pDX, IDC_LOAN_MANAGE_VALUE_PROGRESS, m_ctrlProgress);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CLonaManageValue, CDialog)
	//{{AFX_MSG_MAP(CLonaManageValue)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLonaManageValue message handlers

INT CLonaManageValue::MakeManageValue()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("MakeManageValue");
	CString strValue;
	
	SetProgress(0, _T("작업시작"));

	m_pAllSettingValueDM = FindDM(m_sAllSettingValueDMAlias);
	if(m_pAllSettingValueDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pAllSettingValueDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	m_pRegClassDM = FindDM(_T("DM_등록직급설정값"));
	if(m_pRegClassDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pRegClassDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(5, _T("등록구분별 대출 여부"));
	m_pRegCodeDM = FindDM(m_sRegCodeDMAlias);
	if(m_pRegCodeDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1040, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pRegCodeDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	SetProgress(10, _T("별치기호별 대출 여부"));
	m_pSeCodeDM = FindDM(m_sSeCodeDMAlias);
	if(m_pSeCodeDM == NULL)ERROR_MSG_RETURN_ERROR_CODE2(-1060, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pSeCodeDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(15, _T("이용제한구분별 대출 여부"));
	m_pUseLimitCodeDM = FindDM(m_sUseLimitCodeDMAlias);
	if(m_pUseLimitCodeDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1080, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pUseLimitCodeDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(16, _T("자료실별 대출 여부"));
	m_pUseShelfLocCodeDM = FindDM(m_sShelfLocCodeDMAlias);
	if(m_pUseShelfLocCodeDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1100, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pUseShelfLocCodeDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	SetProgress(20, _T("직급별 관리구분값"));
	m_pUserPositionDM = FindDM(m_sUserPositionDMAlias);
	if(m_pUserPositionDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1120, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pUserPositionDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	SetProgress(21, _T("도서관별 등록구분자리수"));
	m_pUnionLibRegLngDM = FindDM(m_pUnionLibRegLngDMAlias);
	if(m_pUnionLibRegLngDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1125, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pUnionLibRegLngDM, _T(" VALUE_NAME='등록구분자리수'"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	SetProgress(22, _T("도서관정보"));
	m_pUnionLibInfoDM = FindDM(m_sUnionLibInfoDMAlias);
	if(m_pUnionLibInfoDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1130, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pUnionLibInfoDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	m_pIndividualUserPositionDM = FindDM(m_sIndividualUserPositionDMAlias);
	if(m_pIndividualUserPositionDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2(-1140, strFuncName);
	ids = CLocCommonAPI::RefreshDataManager(this, m_pIndividualUserPositionDM, _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	CString sSql;
	sSql.Format(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE='ONELNERESERV'"));	
	m_pAllSettingValueDM->GetOneValueQuery(sSql, strValue);
	strValue.TrimLeft();	strValue.TrimRight();
	if(_T("Y") == strValue)	
	{
		m_bVolReserve = TRUE;
	}
	else
	{
		m_bVolReserve = FALSE;
	}

	m_strEachManageUseYN = _T("Y");

	ids = GetManageValue(_T("열람"), _T("공통"), _T("대출형식사용여부"), _T("관내"), m_sInLibLoanTpyeUse  );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	ids = GetManageValue(_T("열람"), _T("공통"), _T("대출형식사용여부"), _T("특별"), m_sSpeciealLoanTpyeUse);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	ids = GetManageValue(_T("열람"), _T("공통"), _T("대출형식사용여부"), _T("장기"), m_sLongLoanTpyeUse);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	ids = GetManageValue(_T("열람"), _T("공통"), _T("반납형식사용여부"), _T("일괄"), m_sAllLoanTpyeUse);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	ids = GetManageValue(_T("열람"), _T("공통"), _T("반납형식사용여부"), _T("소급"), m_sModifyLoanTpyeUse);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	ids = GetManageValue(_T("열람"), _T("공통"), _T("책두레시스템"), _T("책두레사용여부"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sBookCooperativeSys = _T("N");
	}
	else
	{
		m_sBookCooperativeSys = strValue;
	}
	
	if(m_sBookCooperativeSys == _T("Y"))
	{
		m_sIsLoanManageCode = _T("N");

		m_sIsReturnManageCode = _T("N");

		m_sIsOtherManageReturn = _T("N");
		
		ids = GetManageValue(_T("열람"), _T("공통"), _T("책두레시스템"), _T("책두레운영모드"), strValue);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
		if(strValue.IsEmpty()) 
		{
			m_sBookCooperativeMode = _T("I");
		}
		else
		{
			m_sBookCooperativeMode = strValue;
		}
	}
	else
	{
		m_sBookCooperativeMode = _T("I");

		ids = GetManageValue(_T("열람"), _T("공통"), _T("대출관리"), _T("다른관리구분값대출여부"), strValue);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
		if(strValue.IsEmpty())
		{
			m_sIsLoanManageCode = _T("N");
		}
		else
		{
			m_sIsLoanManageCode = strValue;
		}
		ids = GetManageValue(_T("열람"), _T("공통"), _T("대출관리"), _T("다른관리구분값반납여부"), strValue);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
		if(strValue.IsEmpty())
		{
			m_sIsReturnManageCode = _T("N");
		}
		else
		{
			m_sIsReturnManageCode = strValue;
		}
	
		ids = GetManageValue( _T("기타"), _T("공통"), _T("관리구분별반납설정"), _T(""), strValue);
		if(strValue.IsEmpty())
		{
			m_sIsOtherManageReturn = _T("N");
		}
		else
		{
			m_sIsOtherManageReturn = strValue;
		}
	}

	SetProgress(24, _T("개인 재대출 여부"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("대출관리"), _T("개인재대출여부"), m_sIsOneManReLoan );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(26, _T("가족 재대출 여부"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("대출관리"), _T("가족재대출여부"), m_sIsFamilyReLoan );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(26, _T("대출 이력 보여주기 여부"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("대출관리"), _T("재대출이력보여주기여부"), m_sIsViewLoanHistory );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(27, _T("재대출제한일"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("대출관리"), _T("재대출제한일"), m_sReLoanLimitDay );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	SetProgress(28, _T("연체료"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("연체관리"), _T("연체료"), m_sDelayMoney );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(30, _T("연체일 가중치"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("연체관리"), _T("연체일가중치"), m_sAddDelayDay );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(34, _T("연체일모드"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("연체관리"), _T("연체일모드"), m_sLoanStopMode);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(36, _T("대출 정지일 부여 여부"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("연체관리"), _T("대출정지일부여여부"), m_sIsLoanStop );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(40, _T("연체료 부여 여부"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("연체관리"), _T("연체료부여여부"), m_sIsDelayMoney );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(44, _T("반납연기예약우선순위"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("반납연기관리"), _T("반납연기예약우선순위"), m_sDelayBook );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(48, _T("반납연기자동수동부여"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("반납연기관리"), _T("반납연기자동수동부여"), m_sAutoDelayFunc );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(52, _T("반납연기기능사용여부"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("반납연기관리"), _T("반납연기기능사용여부"), m_sIsDelayFunc );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(56, _T("반납연기가능횟수"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("반납연기관리"), _T("반납연기가능횟수"), m_sDelayCnt );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(60, _T("대출정지일괄해제여부"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("대출정지일괄해제여부"), m_sIsAutoUnLoanStop );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(64, _T("예약자동취소여부"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("예약관리"), _T("예약자동취소여부"), m_sIsAutoCancelReserve);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(68, _T("예약허용인원"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("예약관리"), _T("예약허용인원"), m_sReserveUserCnt );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(70, _T("예약만기일자동부여여부"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("예약관리"), _T("예약만기일자동부여여부"), m_sIsAutoSetReservationDay );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(72, _T("대출자접근권한"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("대출자접근권한"), m_pInfo->USER_ID, m_sLoanUserAccessMode);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	ids = GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("가족회원기능사용여부"), m_sIsFamilyFunc );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(72, _T("부록관리값"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("부록관리"), _T("부록대출여부"), m_sIsLoanAppendix);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	ids = GetManageValue(_T("열람"), _T("공통"), _T("부록관리"), _T("부록반납여부"), m_sIsReturnAppendix);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	ids = GetManageValue(_T("열람"), _T("공통"), _T("부록관리"), _T("부록대출권수포함여부"), m_sIsIncludeLoanCnt);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	ids = GetManageValue(_T("열람"), _T("공통"), _T("부록관리"), _T("부록연체적용여부"), m_sIsAppendixLoanStop);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

	ids = GetManageValue(_T("열람"), _T("공통"), _T("반납연기관리"), _T("반납연기부여기준"), m_sDelayOption);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(90, _T("예약기능사용여부"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("예약관리"), _T("예약기능사용여부"), m_sReserveMode);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	
	SetProgress(95, _T("휴관일"));
	CLibFileManager HolyDayList;
	HolyDayList.SaveDataToFile(m_pRegClassDM, GetUserID());		
	
	SetProgress(96, _T("연체일에 +/- 할 날짜 값"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("연체관리"), _T("연체일부여일수"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	m_nDelayDayCnt = _ttoi(strValue);
	
	SetProgress(96, _T("대출정지일에 +/- 할 날짜 값"));
	ids = GetManageValue(_T("열람"), _T("공통"), _T("연체관리"), _T("대출정지일부여일수"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	m_nLoanStopDayCnt = _ttoi(strValue);

	ids = GetManageValue(_T("기타"), _T("공통"), _T("이동도서관"), _T("대출정책공유여부"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty())	
	{
		m_sLoanPS = _T("Y");
	}
	else
	{
		m_sLoanPS = strValue;
	}

	ids = GetManageValue(_T("열람"), _T("공통"), _T("예약관리"), _T("예약취소시메일발송여부"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sIsSendCancelMail = _T("N");
	}
	else
	{
		m_sIsSendCancelMail = strValue;
	}

	ids = GetManageValue(_T("기타"), _T("공통"), _T("순창도서관"), _T("순창도서관"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty())	
	{
		m_sIsSunchanglib = _T("N");
	}
	else
	{
		m_sIsSunchanglib = strValue;
	}	

	ids = GetManageValue(_T("기타"), _T("공통"), _T("5163"), _T("5163"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty())	
	{
		m_sIs5163lib = _T("N");
	}
	else
	{
		m_sIs5163lib = strValue;
	}

	ids = GetManageValue(_T("기타"), _T("공통"), _T("육사"), _T("육사"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty())
	{
		m_sIs64lib = _T("N");
	}
	else
	{
		m_sIs64lib = strValue;
	}

	ids = GetManageValue(_T("기타"), _T("공통"), _T("국방대"), _T("국방대"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty())
	{
		m_sIs90lib = _T("N");
	}
	else
	{
		m_sIs90lib = strValue;
	}

	ids = GetManageValue(_T("열람"), _T("공통"), _T("연체관리"), _T("누적연체횟수부여방법"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	m_sMethodDelayCntPlus = strValue;
	
	strValue = m_pInfo->MANAGE_CODE;
	m_sUserManageCode = strValue;

	ids = GetManageValue(_T("열람"), _T("공통"), _T("영수증출력"), _T("영수증출력사용여부"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty())
	{
		m_sUseReceipt = _T("N");
	}
	else
	{
		m_sUseReceipt = strValue;
	}

	ids = GetManageValue(_T("열람"), _T("공통"), _T("영수증출력"), _T("영수증출력사용형태"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty())	
	{
		m_sUseReceiptType = _T("NN");
	}
	else
	{
		m_sUseReceiptType = strValue;
	}

	GetReceiptAttestation(m_sAttestation);
	if(m_sAttestation == _T("N"))
	{		
		m_sUseReceipt = _T("N");
		m_sUseReceiptType = _T("NN");
	}

	ids = GetManageValue(_T("기타"), _T("공통"), _T("대출자공유관리"), _T("대출정지회원메시지출력"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue == _T("2"))	
	{
		m_sIsUnionUserStatus = _T("Y");
	}
	else
	{
		m_sIsUnionUserStatus = _T("N");
	}

	ids = GetManageValue(_T("기타"), _T("공통"), _T("대출자공유관리"), _T("통합대출책수제한"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sIsUnionLoanCnt = _T("N");
	}
	else
	{
		m_sIsUnionLoanCnt = strValue;
	}

	strValue.Empty();
	ids = GetManageValue(_T("기타"), _T("공통"), _T("대출자공유관리"), _T("제적회원메시지출력"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	strValue.TrimLeft( ); 
	strValue.TrimRight( );
	if ( strValue == _T("2") )
	{
		m_sIsUnionUserRemoveStatus = _T("Y");
	}
	else
	{
		m_sIsUnionUserRemoveStatus = _T("N");
	}

	ids = GetManageValue(_T("열람"), _T("공통"), _T("예약관리"), _T("예약만기휴관일포함유무"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sIsAddHolyDayReservation = _T("N");
	}
	else
	{
		m_sIsAddHolyDayReservation = strValue;
	}

	ids = GetManageValue(_T("열람"), _T("공통"), _T("연체관리"), _T("대출정지휴관일포함유무"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sIsAddHolyDayLoanStop = _T("N");
	}
	else
	{
		m_sIsAddHolyDayLoanStop = strValue;
	}

	ids = GetManageValue(_T("열람"), _T("공통"), _T("대출관리"), _T("회원증분실시대출불가여부"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sLoanIsLostUserCard = _T("N");
	}
	else
	{
		m_sLoanIsLostUserCard = strValue;
	}

	ids = GetManageValue(_T("열람"), _T("공통"), _T("수수료관리"), _T("수수료관리시스템사용유무"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sFeeMngSys = _T("N");
	}
	else
	{
		m_sFeeMngSys = strValue;
	}

	ids = GetManageValue(_T("열람"), _T("공통"), _T("대출자료관리"), _T("무인대출예약기능유무"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sUnmannedReserveSys = _T("N");
	}
	else
	{
		m_sUnmannedReserveSys = strValue;
	}

	ids = GetManageValue(_T("열람"), _T("공통"), _T("대출자료관리"), _T("야간대출예약기능유무"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sNightReserveSys = _T("N");
	}
	else
	{
		m_sNightReserveSys = strValue;
	}

	ids = GetManageValue(_T("열람"), _T("공통"), _T("통합대출자"), _T("통합대출권수사용여부"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sUnityLoanCntYN = _T("N");
	}
	else
	{
		m_sUnityLoanCntYN = strValue;
	}

	ids = GetManageValue(_T("열람"), _T("공통"), _T("예약관리"), _T("예약만기일부여시SMS발송여부"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sReserveExpireSMS = _T("N");
	}
	else
	{
		m_sReserveExpireSMS = strValue;
	}
	
	ids = GetManageValue(_T("기타"), _T("공통"), _T("메일링시스템"), _T("사용여부"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_sMailingSystemYN = _T("N");
	}
	else
	{
		m_sMailingSystemYN = strValue;
	}

	ids = GetManageValue(_T("열람"), _T("공통"), _T("연체관리"), _T("최대대출정지부여일수"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_strMaxLoanStopDay = _T("N");
	}
	else
	{
		m_strMaxLoanStopDay = strValue;
	}

	m_nUserBasketCnt = 100;

	ids = GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("대출자번호사용자ID로자동입력"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if( TRUE == strValue.IsEmpty() ) 
	{
		m_strIsAutoSetUsernoToUserid = _T("N");
	}
	else
	{
		m_strIsAutoSetUsernoToUserid = strValue;
	}

	ids = GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("주민등록번호뒷자리비밀번호로자동입력"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if( TRUE == strValue.IsEmpty() ) 
	{
		m_strIsAutoSetCivilnoToPassword = _T("N");
	}
	else
	{
		m_strIsAutoSetCivilnoToPassword = strValue;
	}

	ids = GetManageValue(_T("기타"), _T("공통"), _T("대출공유"), _T("대출반납공유버젼"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(TRUE == strValue.IsEmpty()) 
	{
		m_strLoanShare = _T("0");
	}
	else
	{
		m_strLoanShare = strValue;
	}
	ids = GetManageValue(_T("기타"), _T("공통"), _T("대출공유"), _T("대출자공유버젼"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(TRUE == strValue.IsEmpty()) 
	{
		m_strUserShare = _T("0");
	}
	else
	{
		m_strUserShare = strValue;
	}

	if(_T("0") == m_strUserShare)
	{
		m_sIsUnionUserStatus = _T("N");
		m_sIsUnionLoanCnt = _T("N");
		m_sIsUnionUserRemoveStatus = _T("N");
	}

	ids = GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(_T("Y") != strValue) 
	{
		m_sIsUnityLoanService = _T("N");
	}
	else
	{
		m_sIsUnityLoanService = strValue;
	}
	m_sIsShareInsert = _T("N");

	if(_T("Y") == m_sIsUnityLoanService)
	{
		// 통합대출권수사용여부
		m_sUnityLoanCntYN = _T("Y");
 		m_sIsUnionLoanCnt = _T("N");
		ids = GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("대출공유기록"), strValue);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
		if(_T("Y") != strValue)
		{
			m_sIsShareInsert = _T("N");
		}
		
		CESL_DataMgr tempDM;
		tempDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		sSql = _T("SELECT DISTINCT LIB_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE IS NOT NULL ORDER BY LIB_CODE");
		tempDM.RestructDataManager(sSql);
		m_strLibCodeList = _T("");
		for(INT i = 0; i < tempDM.GetRowCount(); i++)
		{
			tempDM.GetCellData(i, 0, strValue);
			if(!strValue.IsEmpty())
			{
				if(!m_strLibCodeList.IsEmpty())
				{
					m_strLibCodeList += _T(",");
				}
				m_strLibCodeList += strValue;
			}
		}
	}

	ids = GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("대출정지공유정책"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_strUserClassShare = _T("1");
	}
	else
	{
		m_strUserClassShare = strValue;
	}

	ids = GetManageValue(_T("열람"), _T("공통"), _T("연체관리"), _T("연체료미납시대출불가유무"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(strValue.IsEmpty()) 
	{
		m_ArrearCheckYN = _T("N");
	}
	else
	{
		m_ArrearCheckYN = strValue;
	}

	ids = GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("주민번호보안설정사용여부"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(_T("N")!=strValue)
	{
		m_strIsKLSecurityCivilNo = _T("Y");
	}
	else
	{
		m_strIsKLSecurityCivilNo = strValue;
	}

	ids = GetManageValue(_T("기타"), _T("공통"), _T("대출반납제어"), _T("대출반납제어사용여부"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	if(_T("Y") == strValue) {
		m_str3100ControlYN = strValue;
	} else {
		m_str3100ControlYN = _T("N");
	}

	ids = GetManageValue(_T("기타"), _T("공통"), _T("RFID책두레옵션"), _T("책두레타관반납관보안설정여부"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
	if(_T("Y")!=strValue) {
		m_strSetRFIDSecuReturnLibYN = _T("N");
	}
	else {
		m_strSetRFIDSecuReturnLibYN = strValue;
	}

	ids = GetManageValue(_T("열람"), _T("공통"), _T("대출관리"), _T("타시스템카드번호사용여부"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
	if(_T("Y")!=strValue) {
		m_strScccardNoYN = _T("N");
	}
	else {
		m_strScccardNoYN = strValue;
	}

	SetProgress(100, _T("완료"));

	return 0;

EFS_END
return -1;
}

INT CLonaManageValue::IsLoanCodeYN(CString sCodeAlias, CString sCode, CString &sValue)
{
EFS_BEGIN

	INT ids;

	if(sCode.IsEmpty())
	{
		sValue = _T("Y");
		return 0;
	}

	CESL_DataMgr *pCodeMgr;

	if(sCodeAlias.Compare(_T("등록구분"))==0)
	{
		pCodeMgr = m_pRegCodeDM;
	}
	else if(sCodeAlias.Compare(_T("별치기호"))==0)
	{
		pCodeMgr = m_pSeCodeDM;
	}
	else if(sCodeAlias.Compare(_T("이용제한구분"))==0)
	{
		pCodeMgr = m_pUseLimitCodeDM;
	}
	else if(sCodeAlias.Compare(_T("자료실구분"))==0)
	{
		pCodeMgr = m_pUseShelfLocCodeDM;
	}
	else 
	{
		sValue = _T("Y");
		return 0;
	}

	CString sLoanYN;
	CString sCodes;

	for(INT i = 0 ; i < pCodeMgr->GetRowCount() ; i++ )
	{
		ids = pCodeMgr->GetCellData(_T("코드"), i, sCodes);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("IsLoanCodeYN()")); 
		}
		sCodes.TrimLeft(); 
		sCodes.TrimRight();

		if(sCodes.Compare(sCode) == 0)
		{
			ids = pCodeMgr->GetCellData(_T("대출여부"), i, sLoanYN );
			if(0 > ids) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("IsLoanCodeYN()")); 
			}
			sLoanYN.TrimLeft(); 
			sLoanYN.TrimRight();
			sValue = sLoanYN;

			return 0;
		}
	}

	return 0;

EFS_END
return -1;
}

INT CLonaManageValue::GetUserPosition(CString sPosition, CString sAlias, CString &sValue)
{
EFS_BEGIN

	INT ids;

	CString sPositions;
	for(INT i = 0 ; i < m_pUserPositionDM->GetRowCount() ; i++ )
	{
		ids = m_pUserPositionDM->GetCellData(_T("코드"), i, sPositions);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("GetUserPosition"));
		}

		sPositions.TrimLeft(); 
		sPositions.TrimRight();

		if(sPosition.Compare(sPositions)==0)
		{
			ids = m_pUserPositionDM->GetCellData(sAlias, i, sValue);
			if(0 > ids)
			{
				AfxMessageBox(sAlias);
			}
			if(0 > ids)
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("GetUserPosition"));
			}

			return 0;
		}
	}
	
	return 0;

EFS_END
return -1;
}

INT CLonaManageValue::GetIndividualUserPosition(CString sPosition, CString sAlias, CString &sValue)
{
EFS_BEGIN

	INT ids;

	CString sPositions;
	for(INT i = 0 ; i < m_pIndividualUserPositionDM->GetRowCount() ; i++ )
	{
		ids = m_pIndividualUserPositionDM->GetCellData(_T("코드"), i, sPositions);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("GetIndividualUserPosition"));
		}

		sPositions.TrimLeft(); 
		sPositions.TrimRight();

		if(sPosition.Compare(sPositions)==0)
		{
			ids = m_pIndividualUserPositionDM->GetCellData(sAlias, i, sValue);
			if(0 > ids) 
			{
				AfxMessageBox(sAlias);
			}
			if(0 > ids) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("GetIndividualUserPosition"));
			}

			return 0;
		}
	}
	
	return 0;

EFS_END
return -1;
}

BOOL CLonaManageValue::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 	

	SetTimer(0, 100, NULL);
	SetTimer(1, 100, NULL);

	return TRUE;  

EFS_END
return FALSE;
}

BOOL CLonaManageValue::Create(CWnd* pParentWnd) 
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;
}

// 진행 상황을 표시해주는 함수
INT CLonaManageValue::SetProgress(INT nPos, CString sMsg )
{
EFS_BEGIN

	CString sPMsg;
	if(nPos == 0)
	{
		m_ctrlProgress.SetRange32(0, 100);
		GetDlgItem(IDC_staMSG)->SetWindowText(_T("시작"));

		return 0;
	}
	else if(nPos == 100)
	{
		sPMsg = _T("100%% \r\n완료");
	}
	else
	{
		sPMsg.Format(_T("%d%% 진행중... \r\n%s 구성중"), nPos, sMsg );
	}

	GetDlgItem(IDC_staMSG)->SetWindowText(sPMsg );

	m_ctrlProgress.SetPos(nPos);

	return 0;

EFS_END
return -1;
}

VOID CLonaManageValue::OnTimer(UINT nIDEvent) 
{
EFS_BEGIN

	if(nIDEvent == 0)
	{
		KillTimer(0);
		if(m_ctrlProgress.GetPos() == 100)
		{
			m_ctrlProgress.SetPos(0);
			OnCancel();
			m_bLoad = TRUE;
		}
		else
		{
			SetTimer(0, 100, NULL);
		}
	}
	else if(nIDEvent == 1 )
	{
		KillTimer(1);
		if(GetSafeHwnd() != NULL) 
		{
			MakeManageValue();
		}
		else
		{
			SetTimer(1, 100, NULL);
		}
	}
	CDialog::OnTimer(nIDEvent);

EFS_END
}

INT CLonaManageValue::ReLoad()
{
EFS_BEGIN

	ShowWindow(SW_SHOW);
	SetTimer(1, 50, NULL);
	SetTimer(0, 100, NULL);

	return 0;

EFS_END
return -1;
}

/// 잡다구리 DM에서 VALUE_1를 뽑아내는 함수
INT CLonaManageValue::GetManageValue(CString sGroup2, CString sGroup3, CString sAlias, CString sValueName, CString &sValue)
{
EFS_BEGIN

	INT ids;

	ids = CLocCommonAPI::GetManageValue(this, m_pAllSettingValueDM, sGroup2, sGroup3, sAlias, sValueName, sValue);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("GetManageValue"));
	}

	return 0;

EFS_END
return -1;
}

//2009.02.02 ADD BY PWR 
/// 잡다구리 DM에서 VALUE_NAME를 뽑아내는 함수
INT CLonaManageValue::GetManageValue_Name(CString sGroup2, CString sGroup3, CString sAlias, CString &sValueName)
{
EFS_BEGIN

	INT ids;

	ids = CLocCommonAPI::GetManageValue_Name(this, m_pAllSettingValueDM, sGroup2, sGroup3, sAlias, sValueName);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("GetManageValue_Name"));
	}

	return 0;

EFS_END
return -1;
}

/// 등록구분 직급별 환경값 참조하는 함수
INT CLonaManageValue::GetRegClass(CString sUserClass, CString sRegCode, CString sFieldName, CString &sValue)
{
EFS_BEGIN

	INT ids;

	sUserClass.TrimLeft(); 	
	sUserClass.TrimRight();
	sRegCode.TrimLeft();	
	sRegCode.TrimRight();

	CString sTmpUserClass, sTmpRegCode;
	for(INT i = 0 ; i < m_pRegClassDM->GetRowCount() ; i++ )
	{
		ids = m_pRegClassDM->GetCellData(_T("직급코드"), i, sTmpUserClass);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("GetRegClass"));
		}

		ids = m_pRegClassDM->GetCellData(_T("등록구분"), i, sTmpRegCode);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("GetRegClass"));
		}

		sTmpUserClass.TrimLeft(); 	
		sTmpUserClass.TrimRight();
		sTmpRegCode.TrimLeft();	
		sTmpRegCode.TrimRight();

		if(sTmpUserClass.Compare(sUserClass)==0 && (sTmpRegCode.Compare(sRegCode)==0))
		{
			ids = m_pRegClassDM->GetCellData(sFieldName, i, sValue);
			if(0 > ids)
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-1020, _T("GetRegClass"));
			}
		}
	}

	return 0;

EFS_END
return -1;
}

INT CLonaManageValue::GetReceiptManageValue(CString &f_strPrINTBarcode, CString &f_strPrINTBarcodeAll, CString &f_strPrINTItem, CString &f_strPrINTMode, CString &f_strDeviceType, CString &f_strPreview, CString &f_strNotify )
{	
	// 2008.04.14 ADD BY PWR 
	// 공지사항 : PRINT_NOTIFY 추가
	CStdioFile file ;	
	CString	strPrINTBarcode,strPrINTBarcodeAll;
	CString	strPrINTItem;
	CString	strPrINTMode;
	CString strTemp;
	CString strDeviceType;
	CString strPreview;
	CString strNotify;
	INT		idx;

	strTemp = _T("..\\cfg\\ReceiptPrINT.cfg") ;	
	if(!file.Open (strTemp,CFile::modeRead | CFile::typeBinary)) 
	{
		return FALSE ;
	}
	TCHAR cUni;
	file.Read(&cUni, sizeof(TCHAR));
	if(0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	CString strLine ;
	CString sTemp ;
	while(file.ReadString (strLine))
	{
		strLine.TrimLeft();
		if(strLine.Left(1) == _T("#") || strLine.IsEmpty()) 
		{
			continue;		
		}
		if(strLine.Left(1) == _T("[") || strLine.IsEmpty())
		{
			continue;		
		}

		idx = strLine.Find(_T("="));
		if(0 <= idx)
		{
			sTemp = strLine.Left(idx);
			sTemp.TrimLeft();
			sTemp.TrimRight();
			if(sTemp == _T("PRINT_BARCODE")) 
			{					
				strPrINTBarcode = strLine.Mid(idx+1);
				strPrINTBarcode.TrimLeft();
				strPrINTBarcode.TrimRight();
			} 
			if(sTemp == _T("PRINT_BARCODE_ALL")) 
			{					
				strPrINTBarcodeAll = strLine.Mid(idx+1);
				strPrINTBarcodeAll.TrimLeft();
				strPrINTBarcodeAll.TrimRight();
			} 
			if(sTemp == _T("PRINT_ITEM")) 
			{	
				strPrINTItem = strLine.Mid(idx+1);
				strPrINTItem.TrimLeft();
				strPrINTItem.TrimRight();
			} 
			if(sTemp == _T("PRINT_MODE")) 
			{
				strPrINTMode = strLine.Mid(idx+1);
				strPrINTMode.TrimLeft();
				strPrINTMode.TrimRight();
			} 
			if(sTemp == _T("PRINT_DEVICE")) 
			{
				strDeviceType = strLine.Mid(idx+1);
				strDeviceType.TrimLeft();
				strDeviceType.TrimRight();
			} 
			if(sTemp == _T("PRINT_PREVIEW")) 
			{
				strPreview = strLine.Mid(idx+1);
				strPreview.TrimLeft();
				strPreview.TrimRight();
			}
			if(sTemp == _T("PRINT_NOTIFY")) 
			{
				strNotify = strLine.Mid(idx+1);
				strNotify.TrimLeft();
				strNotify.TrimRight();
			}
		}
	}
	file.Close () ;

	f_strPrINTBarcode	= strPrINTBarcode;
	f_strPrINTBarcodeAll= strPrINTBarcodeAll;
	f_strPrINTItem		= strPrINTItem;
	f_strPrINTMode		= strPrINTMode;
	f_strDeviceType		= strDeviceType;
	f_strPreview		= strPreview;
	f_strNotify			= strNotify;

	GetReceiptAttestation(m_sAttestation);
	if(m_sAttestation == _T("N"))
	{
		f_strPrINTMode = _T("2");
	}
	
	return 0;
}

INT CLonaManageValue::SetReceiptManageValue(CString f_strPrINTBarcode, CString f_strPrINTBarcodeAll, CString f_strPrINTItem, CString f_strPrINTMode, CString f_strDeviceType, CString f_strPreview, CString f_strNotify )
{
	CStdioFile file;
	CString strFilePath;
	CString	strConfig;

	strFilePath = _T("..\\cfg\\ReceiptPrINT.cfg") ;	
	if(!file.Open (strFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary ))
	{
		return FALSE;
	}

	if(0 == file.GetLength())
	{			
		TCHAR cUni = 0xFEFF;
		file.Write(&cUni, sizeof(TCHAR));
	}
	
	// 2008.04.14 ADD BY PWR 
	// 공지사항 : PRINT_NOTIFY 추가

	strConfig.Format(_T("[RECEIPT_PRINT]\r\n")
		             _T("PRINT_BARCODE = %s\r\n")
					 _T("PRINT_BARCODE_ALL = %s\r\n")
					 _T("PRINT_ITEM = %s\r\n")
					 _T("PRINT_MODE = %s\r\n")
					 _T("PRINT_DEVICE = %s\r\n")
					 _T("PRINT_PREVIEW = %s\r\n")
					 _T("PRINT_NOTIFY = %s\r\n"),
					 f_strPrINTBarcode, 
					 f_strPrINTBarcodeAll, 
					 f_strPrINTItem, 
					 f_strPrINTMode,
					 f_strDeviceType,
					 f_strPreview,
					 f_strNotify );
	file.WriteString(strConfig);
	file.Close();

	return 0;
}

VOID CLonaManageValue::GetReceiptAttestation(CString &Value)
{
	// 인증코드 
	CStdioFile file ;		
	CString strTemp;		
	CString strLine;
	INT		ids;

	m_sAttestation = _T("");
	strTemp = _T("..\\cfg\\ReceiptPrINTCode.cfg") ;	
	if(!file.Open (strTemp,CFile::modeRead | CFile::typeBinary)) 
	{
		m_sAttestation = _T("N");
		return;
	}
		TCHAR cUni;
		file.Read(&cUni, sizeof(TCHAR));
		if(0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
	
	ids = file.ReadString (strLine);
	if(ids) m_sAttestation = strLine;
	if(m_sAttestation == _T("N") || m_sAttestation.IsEmpty())
	{
		m_sAttestation = _T("N");
		Value = _T("N");
	}
	file.Close();
}


HBRUSH CLonaManageValue::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}

BOOL CLonaManageValue::GetLibDMValue(CString strLibCode, CString strAlias)
{
	EFS_BEGIN
	
	INT i = 0, nRowCnt = 0;
	CString strLibCodeTemp, strValue;
	CESL_DataMgr* pLibDM = FindDM(_T("DM_BO_LOC_3100_LIB_INFO"));
	
	nRowCnt = pLibDM->GetRowCount();
	for ( i=0; i<nRowCnt; i++ ) {
		pLibDM->GetCellData( _T("도서관부호") , i , strLibCodeTemp );		
		
		if ( strLibCode.Compare(strLibCodeTemp) == 0 ) {
			if(strAlias.IsEmpty() == TRUE){
				return TRUE;
			}
			strValue = pLibDM->GetCellData(strAlias, i);
			if(strValue.Compare(_T("Y")) == 0)
				return TRUE;
			break;
		}
	}	

	return FALSE;
	EFS_END
		return FALSE;
}

VOID CLonaManageValue::GetLibDMValue(CString strLibCode, CString strAlias, CString &strValue)
{
	EFS_BEGIN
	
	INT i = 0, nRowCnt = 0;
	CString strLibCodeTemp;
	CESL_DataMgr* pLibDM = FindDM(_T("DM_BO_LOC_3100_LIB_INFO"));
	
	nRowCnt = pLibDM->GetRowCount();
	for ( i=0; i<nRowCnt; i++ ) {
		pLibDM->GetCellData( _T("도서관부호") , i , strLibCodeTemp );		
		
		if ( strLibCode.Compare(strLibCodeTemp) == 0 ) {
			if(strAlias.IsEmpty() == TRUE){
				return ;
			}
			strValue = pLibDM->GetCellData(strAlias, i);
			break;
		}
	}	

	return ;
	EFS_END
	return;
}

