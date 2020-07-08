#include "stdafx.h"
#include "CheckLoanBook.h"
#include "..\..\..\..\관리\도서관관리\FileManager\LibFileManager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckLoanBook dialog


CCheckLoanBook::CCheckLoanBook(CESL_Mgr* pParent /*=NULL*/ , CLonaManageValue *pManageValue/*=NULL*/ )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckLoanBook)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sUserDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	m_sExpLoanDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");

	m_pManageValue = pManageValue;

}
CCheckLoanBook::~CCheckLoanBook()
{
}

VOID CCheckLoanBook::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckLoanBook)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CCheckLoanBook, CDialog)
	//{{AFX_MSG_MAP(CCheckLoanBook)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


INT CCheckLoanBook::IsLoan( CString sCodeAlias , CString sCode )
{
	EFS_BEGIN

	INT ids;

	CString sValue;

	ids = m_pManageValue->IsLoanCodeYN( sCodeAlias , sCode , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsLoan") ); 

	if( sValue.Compare(_T("N")) == 0 ) return 1;

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::IsUseObject( CString &sUseObjectCode )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pExpLoanDM = FindDM(m_sExpLoanDMAlias);
	if( pExpLoanDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("IsUseObject") );

	CESL_DataMgr *pUserDM = FindDM( m_sUserDMAlias );
	if( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("IsUseObject") );

	CString sUserClassCode;
	ids = pUserDM->GetCellData( _T("대출자직급") , 0 , sUserClassCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("IsUseObject") ); 

	CString sUseObjCode;

	ids = m_pManageValue->GetUserPosition( sUserClassCode , _T("이용대상구분") , sUseObjCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("IsUseObject") ); 

	if( sUseObjCode.IsEmpty() ) sUseObjCode = _T("0");

	if( sUseObjCode.Compare(_T("Y"))==0 ) 
		return 0;

	CString sExpUseObjCode;
	ids = pExpLoanDM->GetCellData( _T("이용대상구분") , 0 , sExpUseObjCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("IsUseObject") ); 

	if( sExpUseObjCode.Compare(_T("AD"))!=0 )
		return 0;
	else
	{
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("이용대상구분") , sExpUseObjCode , sUseObjectCode );
		return 1;
	}

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetReturnDelayDay( CString &sReturnDelayDay , CString sRegCode )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pUserDM = FindDM( m_sUserDMAlias );
	if( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("GetReturnDelayDay") );

	if( pUserDM->GetRowCount() < 1 ) return 1;

	CString sUserClassCode;
	ids = pUserDM->GetCellData( _T("대출자직급") , 0 , sUserClassCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("GetReturnDelayDay") ); 

	ids = m_pManageValue->GetUserPosition( sUserClassCode , _T("반납연기일") , sReturnDelayDay );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetReturnDelayDay") ); 

	if( sRegCode.Compare(_T("NO_REG_CODE")) != 0 )
	{
		CString sValue;
		
		ids = m_pManageValue->GetRegClass( sUserClassCode , sRegCode , _T("반납연기일수") , sValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetReturnDelayDay") ); 
		
		if( !sValue.IsEmpty() || (_ttoi(sValue)!=0) ) sReturnDelayDay = sValue;
	}

	if( sReturnDelayDay.IsEmpty() ) sReturnDelayDay = _T("0");


	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetLoanBookCnt( INT &nMaxLoanBookCnt )
{
	EFS_BEGIN

	CESL_DataMgr *pUserDM = FindDM( m_sUserDMAlias );
	if( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("GetLoanBookCnt") );

	INT ids;
	if( pUserDM->GetRowCount() < 1 ) return 1;

	CString sUserClassCode;
	ids = pUserDM->GetCellData( _T("대출자직급") , 0 , sUserClassCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("GetLoanBookCnt") ); 

	CString sResult;
	ids = m_pManageValue->GetUserPosition( sUserClassCode , _T("총대출가능수") , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetLoanBookCnt") ); 

	if( sResult.IsEmpty() ) sResult = _T("0");

	nMaxLoanBookCnt = _ttoi( sResult );

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetReserveBookCnt( INT &nMaxReserveBookCnt )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pUserDM = FindDM( m_sUserDMAlias );
	if( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("GetReserveBookCnt") );

	if( pUserDM->GetRowCount() < 1 ) return 1;

	CString sUserClassCode;
	ids = pUserDM->GetCellData( _T("대출자직급") , 0 , sUserClassCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("GetReserveBookCnt") ); 

	CString sResult;
	ids = m_pManageValue->GetUserPosition( sUserClassCode , _T("총예약가능수") , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetReserveBookCnt") ); 

	if( sResult.IsEmpty() ) sResult = _T("0");

	nMaxReserveBookCnt = _ttoi( sResult );

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::IsUseManageCode()
{
	EFS_BEGIN

	CString sValue;

	sValue = m_pManageValue->m_sIsLoanManageCode;

	if( sValue.Compare(_T("N"))==0 ) return 1;

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::IsUseReturnManageCode()
{
	EFS_BEGIN

	CString sValue;

	sValue = m_pManageValue->m_sIsReturnManageCode;

	if( sValue.Compare(_T("N"))==0 ) return 1;

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::IsLoanManageCode(CString sBookManageCode)
{
	EFS_BEGIN

	INT ids;

	CString sExpManageCode;
	
	if( sBookManageCode.IsEmpty() )
	{
		CESL_DataMgr *pExpLoanDM = FindDM(m_sExpLoanDMAlias);
		if( pExpLoanDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("IsLoanManageCode") );


		ids = pExpLoanDM->GetCellData( _T("관리구분") , 0 , sExpManageCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("IsLoanManageCode") ); 
	}
	else
		sExpManageCode = sBookManageCode;

	CString sManageCode = m_pManageValue->m_sUserManageCode;

	sExpManageCode.TrimLeft(); sExpManageCode.TrimRight();
	sManageCode.TrimLeft(); sManageCode.TrimRight();

	if( sExpManageCode.Compare( sManageCode ) == 0 ) return 1;

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::IsReturnManageCode(CString sBookManageCode)
{
	EFS_BEGIN

	CString sExpManageCode;
	
	sExpManageCode = sBookManageCode;

	CString sManageCode = m_pManageValue->m_sUserManageCode;
	
	sExpManageCode.TrimLeft(); sExpManageCode.TrimRight();
	sManageCode.TrimLeft(); sManageCode.TrimRight();

	if( sExpManageCode.Compare( sManageCode ) == 0 ) return 1;

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::IsReLoanBook(CString strLoanDate)
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pExpLoanDM = FindDM(m_sExpLoanDMAlias);
	if( pExpLoanDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("IsReLoanBook") );

	CESL_DataMgr *pUserDM = FindDM( m_sUserDMAlias );
	if( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("IsReLoanBook") );

	CString sValue;

	sValue = m_pManageValue->m_sIsOneManReLoan;

	CString sReLoanLimitDay;
	sReLoanLimitDay = m_pManageValue->m_sReLoanLimitDay;

	INT nReLoanMode = 0;
	INT nReLoanYN = 0;
	INT nFamilyReLoanYN = 0;
	if( sValue.Compare(_T("N")) == 0 )
	{
		nReLoanYN = 1;
		sValue = m_pManageValue->m_sIsFamilyReLoan;
		if( sValue.Compare(_T("N")) == 0 )
			nFamilyReLoanYN = 1;
	}
	else
		nReLoanYN = 0;	

	CString sSql;
	CString sBookRecKey;
	CString sUserRecKey;
	CString sFamilyRecKey;

	ids = pExpLoanDM->GetCellData( _T("책KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-2 , _T("IsReLoanBook") ); 

	ids = pUserDM->GetCellData( _T("대출자KEY") , 0 , sUserRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-3 , _T("IsReLoanBook") ); 

	ids = pUserDM->GetCellData( _T("가족KEY") , 0 , sFamilyRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-4 , _T("IsReLoanBook") ); 

	INT nCount=0;
	INT nOneManCnt=0;
	INT nFamilyCnt=0;
	INT nFamilyReservationCnt = 0;

	
	CString strLimitWhere;
	if( _ttoi(sReLoanLimitDay) != 0 )
	{
	
		strLimitWhere = _T("AND TO_DATE('") + strLoanDate + _T("','YYYY/MM/DD') - TO_DATE(CL.RETURN_DATE,'YYYY/MM/DD') < ") + sReLoanLimitDay ;
	}

	sSql.Format( _T(" SELECT SUM(CNT) FROM ( ")
		         _T(" SELECT COUNT(1) AS CNT FROM CO_LOAN_USER_TBL CLU, LH_STORAGE_V01 CL ")
				 _T(" WHERE CL.STATUS IN ('0','2','1','5','6','L','R','D') ")
				 _T(" AND CL.USER_KEY = CLU.REC_KEY ")
				 _T(" AND CL.BOOK_KEY = %s ")
				 _T(" AND CL.USER_KEY = %s ")
				 _T(" %s )"), sBookRecKey , sUserRecKey, strLimitWhere );

	ids = CLocCommonAPI::GetOneValueQuery( this , pExpLoanDM , sSql , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-5 , _T("IsReLoanBook") );

	nOneManCnt = _ttoi(sValue);

	if( !sFamilyRecKey.IsEmpty() )
	{
		sSql.Format( _T(" SELECT SUM(CNT) FROM ( ")
					 _T(" SELECT COUNT(1) AS CNT FROM CO_LOAN_USER_TBL CLU, LH_STORAGE_V01 CL ")
					 _T(" WHERE CL.STATUS IN ('0','2','1','5','6','L','R','D') ")
					 _T(" AND CL.USER_KEY = CLU.REC_KEY ")
					 _T(" AND CL.BOOK_KEY = %s ")
					 _T(" AND ( CLU.FID_KEY = %s ) ")
					 _T(" %s )"), sBookRecKey , sFamilyRecKey, strLimitWhere );

		ids = CLocCommonAPI::GetOneValueQuery( this , pExpLoanDM , sSql , sValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("IsReLoanBook") );
		nFamilyCnt = _ttoi(sValue);

		sSql.Format( _T(" SELECT COUNT(1) FROM CO_LOAN_USER_TBL CLU, LS_WORK_T01 CL ")
					 _T(" WHERE CL.STATUS IN ('0', '3') ")
					 _T(" AND CL.USER_KEY = CLU.REC_KEY ")
					 _T(" AND CL.BOOK_KEY = %s ")
					 _T(" AND CLU.FID_KEY = %s ")
					 , sBookRecKey , sFamilyRecKey );

		ids = CLocCommonAPI::GetOneValueQuery( this , pExpLoanDM , sSql , sValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("IsReLoanBook") );
		nFamilyReservationCnt = _ttoi(sValue);
	}


	if( nOneManCnt > 0 )
	{
		if( nReLoanYN == 1 )
		{
			return 11;
		}
		else
		{
			return 1;
		}
	}
	else if( nFamilyCnt > 0 )
	{
		if( nFamilyReLoanYN == 1 )
		{
			return 22;
		}
		else
		{
			return 2;
		}
	}
	
	else if( nFamilyReservationCnt > 0 )
	{
		if( nFamilyReLoanYN == 1 )
		{
			return 33;
		}
	}

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetReturnExpDay( INT &nExpDay , CString sRegCode , CTime& tStartDate , BOOL bRegCodeYN)
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pUserDM = FindDM( m_sUserDMAlias );
	if( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("GetReturnExpDay") );

	CString sUserClassCode;

	ids = pUserDM->GetCellData( _T("대출자직급") , 0 , sUserClassCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-2 , _T("GetReturnExpDay") ); 

	CString sResult;
	ids = m_pManageValue->GetUserPosition( sUserClassCode , _T("대출기한") , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetReturnExpDay") );

	if( bRegCodeYN )
	{
		CString sValue;
		ids = m_pManageValue->GetRegClass( sUserClassCode , sRegCode , _T("대출기한") , sValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GetReturnExpDay") );

		if( !sValue.IsEmpty() ) sResult = sValue;
	}

	nExpDay = _ttoi( sResult );

	if( nExpDay )
	{
		ids = GetAddHolyDay( nExpDay , tStartDate );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetReturnExpDay") );
	}

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetDelayMoney( long &nDelayMoneyPerDay )
{
	EFS_BEGIN


	CString sValue;

	sValue = m_pManageValue->m_sDelayMoney;


	nDelayMoneyPerDay = _ttoi(sValue);


	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetLoanStopDateOption( INT &nDelayMode )
{
	EFS_BEGIN


	CString sValue;

	sValue = m_pManageValue->m_sLoanStopMode;


	nDelayMode = _ttoi(sValue);

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::IsSetLoanStopDate()
{
	EFS_BEGIN


	CString sValue;

	sValue = m_pManageValue->m_sIsLoanStop;

	if( sValue.Compare(_T("N"))==0)
		return 1;

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::IsSetDelayMoney()
{
	EFS_BEGIN


	CString sValue;

	sValue = m_pManageValue->m_sIsDelayMoney;

	if( sValue.Compare(_T("N"))==0)
		return 1;

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetDelayLoanMode( INT &nDelayMode )
{
	EFS_BEGIN

	CString sValue;

	sValue = m_pManageValue->m_sDelayBook;

	nDelayMode = _ttoi( sValue );

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetReservationExpireDay( INT &nReservationExpireDay )
{
	EFS_BEGIN

	INT ids;
	
	CESL_DataMgr *pUserDM = FindDM( m_sUserDMAlias );
	if( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("GetReservationExpireDay") );

	if( pUserDM->GetRowCount() < 1 ) return 1;

	CString sUserClassCode;
	ids = pUserDM->GetCellData( _T("대출자직급") , 0 , sUserClassCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("GetReservationExpireDay") ); 

	CString sResult;
	ids = m_pManageValue->GetUserPosition( sUserClassCode , _T("예약만기일수") , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetReservationExpireDay") ); 

	if( sResult.IsEmpty() ) sResult = _T("0");

	nReservationExpireDay = _ttoi( sResult );

	if( _T("Y") == m_pManageValue->m_sIsAddHolyDayReservation )
	{
		ids = GetAddTotalHolyDay( nReservationExpireDay );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetReservationExpireDay") );
	}
	else
	{
		ids = GetAddHolyDay( nReservationExpireDay );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetReservationExpireDay") );
	}

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetReservationExpireDay( CString sPositionCode , INT &nReservationExpireDay )
{
	EFS_BEGIN

	INT ids;
	CString sResult;
	ids = m_pManageValue->GetUserPosition( sPositionCode , _T("예약만기일수") , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetReservationExpireDay") ); 

	if( sResult.IsEmpty() ) sResult = _T("0");

	nReservationExpireDay = _ttoi( sResult );

	if( _T("Y") == m_pManageValue->m_sIsAddHolyDayReservation )
	{
		ids = GetAddTotalHolyDay( nReservationExpireDay );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetReservationExpireDay") );
	}
	else
	{
		ids = GetAddHolyDay( nReservationExpireDay );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetReservationExpireDay") );
	}

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetAutoChangeUserClass()
{
	EFS_BEGIN


	CString sValue;

	sValue = m_pManageValue->m_sIsAutoUnLoanStop;

	if( sValue.Compare(_T("N"))==0) return 1;

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetAutoChangeReserveCancel()
{
	EFS_BEGIN


	CString sValue;

	sValue = m_pManageValue->m_sIsAutoCancelReserve;

	if( sValue.Compare(_T("N"))==0) return 1;

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetIsDelayFunc()
{
	EFS_BEGIN


	CString sValue;

	sValue = m_pManageValue->m_sIsDelayFunc;

	if( sValue.Compare(_T("N"))==0) return 1;

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetAutoDelayFunc()
{
	EFS_BEGIN


	CString sValue;

	sValue = m_pManageValue->m_sAutoDelayFunc;

	if( sValue.Compare(_T("N"))==0) return 1;

	return 0;

	EFS_END
	return -1;

}


INT CCheckLoanBook::GetDelayCnt(CString &sValue)
{
	EFS_BEGIN


	sValue = m_pManageValue->m_sDelayCnt;

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetReserveUserCnt(CString &sValue)
{
	EFS_BEGIN

	sValue = m_pManageValue->m_sReserveUserCnt;
	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetReserveMode(CString &sValue)
{
	EFS_BEGIN

	sValue = m_pManageValue->m_sReserveMode;
	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetDelayAddDay( INT &nDelayAddDay )
{
	EFS_BEGIN

	nDelayAddDay = _ttoi(m_pManageValue->m_sAddDelayDay);
	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetLoanUserAccessMode(INT &nMode )
{
	EFS_BEGIN

	nMode = _ttoi(m_pManageValue->m_sLoanUserAccessMode );

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::IsLoanReturnTypeUse( CString sGroupType , CString sType )
{
	EFS_BEGIN

	CString sValue;
	if( sGroupType.Compare(_T("대출"))==0 )
	{
		if( sType.Compare(_T("특별"))==0 )
			sValue = m_pManageValue->m_sSpeciealLoanTpyeUse;
		else if( sType.Compare(_T("관내")) == 0 )
			sValue = m_pManageValue->m_sInLibLoanTpyeUse;	
		else if( sType.Compare(_T("장기")) == 0 )
			sValue = m_pManageValue->m_sLongLoanTpyeUse;
	}
	else
	{
		if( sType.Compare(_T("일괄"))==0 )
			sValue = m_pManageValue->m_sAllLoanTpyeUse ;
		else 
			sValue = m_pManageValue->m_sModifyLoanTpyeUse;

	}

	if( sValue.Compare(_T("N"))==0 )
		return 1;

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::IsFamilyFunc()
{
	EFS_BEGIN

	if( m_pManageValue->m_sIsFamilyFunc.Compare(_T("N"))==0 ) return 1;
	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetAutoSetReserveExpireDay()
{
	EFS_BEGIN

	if( m_pManageValue->m_sIsAutoSetReservationDay.Compare(_T("N"))==0) return 1;
	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetAddHolyDay( INT &nExpDay )
{
	EFS_BEGIN
	
	CString strGetData;
	CTime tTime = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
	CTime tStartDate(tTime.GetYear(), tTime.GetMonth(), tTime.GetDay(), 0, 0, 0);

	GetAddHolyDay( nExpDay , tStartDate );

	return 0;

	EFS_END
	return -1;

}

INT CCheckLoanBook::GetAddHolyDay( INT &nExpDay , CTime tStartDate )
{
	EFS_BEGIN
			
	INT nHolyDayCnt = nExpDay;
	CTime tTargetDate;
	while( TRUE )
	{
		tTargetDate = tStartDate + CTimeSpan( nHolyDayCnt , 0 , 0 , 0 );
		
		nHolyDayCnt = GetHolyDay( tTargetDate );
		if( nHolyDayCnt == 0 ) break;
		
		nExpDay += nHolyDayCnt;
		tStartDate = tTargetDate;
	}
	
	return 0;
	
	EFS_END
		return -1;
	
}

INT CCheckLoanBook::GetHolyDay( CTime tTargetDate )
{
	EFS_BEGIN

	INT nHolyDayCnt = 0;
	CStringList sYearList;
	CStringList sMonthList;
	CStringList sDayList;

	CLibFileManager HolyDayList;

	HolyDayList.GetLibHoliDayInfo( sYearList , sMonthList , sDayList );

	COleDateTime tHolyDate;

	POSITION PosYear , PosMonth , PosDay;
	CStringArray asDay;
	CString sYear , sMon , sDays;
	for( INT i = 0 ; i < sYearList.GetCount() ; i++ )
	{
		PosYear = sYearList.FindIndex(i);
		PosMonth = sMonthList.FindIndex(i);
		PosDay = sDayList.FindIndex(i);

		sYear = sYearList.GetAt(PosYear);
		sMon = sMonthList.GetAt(PosMonth);
		sDays = sDayList.GetAt(PosDay);
		CLocCommonAPI::MakeArrayList( sDays , _T(",") , asDay );

		for( INT j = 0 ; j < asDay.GetSize() ; j++ )
		{
			tHolyDate = COleDateTime( _ttoi(sYear) , _ttoi(sMon) , _ttoi(asDay.GetAt(j)) , 0 , 0 , 0 );
			
			if( tHolyDate == COleDateTime( tTargetDate.GetYear() , tTargetDate.GetMonth() , tTargetDate.GetDay() , 0 , 0, 0 ) )
			{
				nHolyDayCnt++;
			}
		}

	}

	return nHolyDayCnt;


	EFS_END
	return -1;

}

INT CCheckLoanBook::IsAppendixLoan()
{
	EFS_BEGIN

	if( m_pManageValue->m_sIsLoanAppendix.Compare( _T("Y") ) == 0 )
		return 0;

	return 1;

	EFS_END
	return -1;
}

INT CCheckLoanBook::IsAppendixIncludeLoanCnt()
{
	EFS_BEGIN

	if( IsAppendixLoan() == 1 )
		return 1;

	if( m_pManageValue->m_sIsIncludeLoanCnt.Compare( _T("Y") ) == 0 )
		return 0;

	return 1;

	EFS_END
	return -1;
}

INT CCheckLoanBook::GetDelayOption()
{
	EFS_BEGIN
		
	return _ttoi(m_pManageValue->m_sDelayOption);
		
	EFS_END
	return -1;
}

INT CCheckLoanBook::IsViewReLoanHistory()
{
	EFS_BEGIN

	if( m_pManageValue->m_sIsViewLoanHistory.Compare(_T("Y")) == 0  )
		return 0;
	else
		return 1;

	EFS_END

	return -1;
}

INT CCheckLoanBook::GetDelayDayCnt()
{
	EFS_BEGIN

	return m_pManageValue->m_nDelayDayCnt;

	EFS_END

	return -1;
}

INT CCheckLoanBook::GetLoanStopDayCnt()
{
	EFS_BEGIN

	return m_pManageValue->m_nLoanStopDayCnt;

	EFS_END

	return -1;
}

bool CCheckLoanBook::GetLoanPS()
{
	EFS_BEGIN

	CString sLoanPS = m_pManageValue->m_sLoanPS;
	sLoanPS.MakeUpper();
	
	if( sLoanPS.Compare(_T("N")) == 0 )
		return FALSE;

	return TRUE;

	EFS_END
		
	return TRUE;
}

INT CCheckLoanBook::GetUserManageCode(CString &sUserManageCode)
{
	EFS_BEGIN

	sUserManageCode = m_pManageValue->m_sUserManageCode;

	return 0;

	EFS_END

	return -1;


}

bool CCheckLoanBook::IsSunchang()
{
	EFS_BEGIN

	if( m_pManageValue->m_sIsSunchanglib.Compare(_T("Y")) == 0 )
		return TRUE;

	return FALSE;

	EFS_END
	return FALSE;
}


INT CCheckLoanBook::GetMethodDelayCntPlus( CString &sValue )
{
	EFS_BEGIN
	
	sValue = m_pManageValue->m_sMethodDelayCntPlus;

	return 0;

	EFS_END

	return -1;
}


BOOL CCheckLoanBook::IsSendCancelMail()
{
	EFS_BEGIN
	
	if( m_pManageValue->m_sIsSendCancelMail.Compare(_T("Y")) == 0 )
		return TRUE;

	return FALSE;

	EFS_END

	return FALSE;
}

INT CCheckLoanBook::IsOtherManageReturn()
{
EFS_BEGIN

	CString strIsReturn;

	if( _T("Y") == m_pManageValue->m_strEachManageUseYN )
	{
		strIsReturn = m_pManageValue->m_sIsOtherManageReturn;
		if( _T("Y") == strIsReturn )
		{
			return 1;
		}
	}
	
	return 0;

EFS_END
	return -1;	
}

INT CCheckLoanBook::IsAppendixReturn()
{
EFS_BEGIN

	if( 0 == m_pManageValue->m_sIsReturnAppendix.Compare(_T("Y")) )
	{
		return 0;
	}

	return 1;

EFS_END
	return -1;
}

INT CCheckLoanBook::GetAddTotalHolyDay(INT &nExpDay)
{
EFS_BEGIN
	
	CString strGetData;
	CTime tTime = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
	CTime tStartDate(tTime.GetYear(), tTime.GetMonth(), tTime.GetDay(), 0, 0, 0);

	return GetAddTotalHolyDay( nExpDay , tStartDate );	

EFS_END
	return -1;
}

INT CCheckLoanBook::GetAddTotalHolyDay(INT &nExpDay, CTime tStartDate)
{
EFS_BEGIN
	
	CTime tTargetDate;

	for( INT i=0;i<=nExpDay;i++ )
	{
		tTargetDate = tStartDate + CTimeSpan(i, 0, 0, 0);

		if( GetHolyDay(tTargetDate) )
		{
			nExpDay++;
		}
	}		

	return 0;

EFS_END
	return -1;
}

INT CCheckLoanBook::GetReservationCancelLimitCnt(INT &nLimitCnt)
{
EFS_BEGIN

	INT ids;
	
	CESL_DataMgr *pUserDM = FindDM( m_sUserDMAlias );
	if( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("GetReservationCancelLimitCnt") );

	if( pUserDM->GetRowCount() < 1 ) return 1;

	CString sUserClassCode;
	ids = pUserDM->GetCellData( _T("대출자직급") , 0 , sUserClassCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("GetReservationCancelLimitCnt") ); 


	CString sResult;
	ids = m_pManageValue->GetUserPosition( sUserClassCode , _T("예약취소제한갯수") , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetReservationCancelLimitCnt") ); 

	if( sResult.IsEmpty() ) 
		sResult = _T("0");

	nLimitCnt = _ttoi( sResult );

	return 0;
EFS_END
	return -1;
}

INT CCheckLoanBook::GetReservationCancelLimitPeriod(INT &nLimitPeriod)
{
EFS_BEGIN
	
	INT ids;
	
	CESL_DataMgr *pUserDM = FindDM( m_sUserDMAlias );
	if( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("GetReservationCancelLimitPeriod") );

	if( pUserDM->GetRowCount() < 1 ) return 1;

	CString sUserClassCode;
	ids = pUserDM->GetCellData( _T("대출자직급") , 0 , sUserClassCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("GetReservationCancelLimitPeriod") ); 


	CString sResult;
	ids = m_pManageValue->GetUserPosition( sUserClassCode , _T("예약취소제한적용기간") , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetReservationCancelLimitPeriod") ); 

	if( sResult.IsEmpty() ) 
		sResult = _T("0");

	nLimitPeriod = _ttoi( sResult );

	return 0;
EFS_END
	return -1;
}

INT CCheckLoanBook::GetTLLoanBookCnt(INT &nMaxTLLoanBookCnt)
{
	CESL_DataMgr *pUserDM = FindDM( m_sUserDMAlias );
	if( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("GetTLLoanBookCnt") );

	INT ids;
	if( pUserDM->GetRowCount() < 1 ) return 1;

	CString sUserClassCode;
	ids = pUserDM->GetCellData( _T("대출자직급") , 0 , sUserClassCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("GetTLLoanBookCnt") ); 

	CString sResult;
	ids = m_pManageValue->GetUserPosition( sUserClassCode , _T("이동총대출가능수") , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetTLLoanBookCnt") ); 

	if( sResult.IsEmpty() ) sResult = _T("0");

	nMaxTLLoanBookCnt = _ttoi( sResult );

	return 0;
}

INT CCheckLoanBook::GetOtherLoanBookCnt(INT &nMaxLoanBookCnt)
{
	CESL_DataMgr *pUserDM = FindDM( m_sUserDMAlias );
	if( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("GetOtherLoanBookCnt") );

	INT ids;
	if( pUserDM->GetRowCount() < 1 ) return 1;

	CString sUserClassCode;
	ids = pUserDM->GetCellData( _T("대출자직급") , 0 , sUserClassCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("GetOtherLoanBookCnt") ); 

	CString sResult;
	ids = m_pManageValue->GetUserPosition( sUserClassCode , _T("타관총대출가능수") , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetOtherLoanBookCnt") ); 

	if( sResult.IsEmpty() ) sResult = _T("0");

	nMaxLoanBookCnt = _ttoi( sResult );

	return 0;
}

INT CCheckLoanBook::GetUnityLoanBookCnt(INT &nMaxLoanBookCnt)
{
	CESL_DataMgr *pUserDM = FindDM( m_sUserDMAlias );
	if( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("GetUnityLoanBookCnt") );

	INT ids;
	if( pUserDM->GetRowCount() < 1 ) return 1;

	CString sUserClassCode;
	ids = pUserDM->GetCellData( _T("대출자직급") , 0 , sUserClassCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("GetUnityLoanBookCnt") ); 

	CString sResult;
	ids = m_pManageValue->GetUserPosition( sUserClassCode , _T("통합총대출가능수") , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetUnityLoanBookCnt") ); 

	if( sResult.IsEmpty() ) sResult = _T("0");

	nMaxLoanBookCnt = _ttoi( sResult );

	return 0;
}

INT CCheckLoanBook::GetIndividualLoanBookCnt( INT &nMaxLoanBookCnt )
{
	EFS_BEGIN

	CESL_DataMgr *pUserDM = FindDM( m_sUserDMAlias );
	if( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("GetIndividualLoanBookCnt") );

	INT ids;
	if( pUserDM->GetRowCount() < 1 ) return 1;

	CString sUserClassCode;
	ids = pUserDM->GetCellData( _T("대출자직급") , 0 , sUserClassCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("GetIndividualLoanBookCnt") ); 

	CString sResult;
	ids = m_pManageValue->GetIndividualUserPosition( sUserClassCode , _T("총대출가능수") , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetIndividualLoanBookCnt") ); 

	if( sResult.IsEmpty() ) sResult = _T("0");

	nMaxLoanBookCnt = _ttoi( sResult );

	return 0;

	EFS_END
	return -1;

}