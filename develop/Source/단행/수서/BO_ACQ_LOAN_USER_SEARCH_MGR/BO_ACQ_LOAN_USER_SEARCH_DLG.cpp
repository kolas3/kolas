// BO_ACQ_LOAN_USER_SEARCH_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_LOAN_USER_SEARCH_DLG.h"
// 2011.09.08 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_LOAN_USER_SEARCH_DLG dialog


CBO_ACQ_LOAN_USER_SEARCH_DLG::CBO_ACQ_LOAN_USER_SEARCH_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_LOAN_USER_SEARCH_DLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_LOAN_USER_SEARCH_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentMgr = pParent;
	pMakeSearchData = new CMakeSearchData(pParent);	
	m_pDM = NULL ;
	m_pCM = NULL ;
	m_pGrid = NULL ;
	m_bAutoSearch = FALSE;  // 2005-01-05
	m_sSelectRecKey.Empty();
	m_strUserID.Empty();  // 2005-01-05
	m_nOpenMode = 0;

	// 2012.11.06 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2차
	m_strFurnishCnt = _T("0");
}

// 소멸자
CBO_ACQ_LOAN_USER_SEARCH_DLG::~CBO_ACQ_LOAN_USER_SEARCH_DLG()
{
	if (pMakeSearchData)
	{
		delete pMakeSearchData; 
		pMakeSearchData = NULL;
	}

}


VOID CBO_ACQ_LOAN_USER_SEARCH_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_LOAN_USER_SEARCH_DLG)
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_LOAN_USER_SEARCH_DLG, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_LOAN_USER_SEARCH_DLG)
	ON_BN_CLICKED(IDC_btnSearch, OnbtnSearch)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_LOAN_USER_SEARCH_DLG message handlers

BOOL CBO_ACQ_LOAN_USER_SEARCH_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr(_T("K2UPGRADE_대출자간략조회")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	m_pDM = FindDM(_T("DM_대출자간략조회"));
	m_pCM = FindCM(_T("CM_대출자간략조회"));

	m_pGrid = (CESL_Grid*)FindControl( _T("CM_대출자간략조회") , _T("MainGrid") );
	if( m_pGrid == NULL ) 
	{
		AfxMessageBox(_T("InitESL_Grid Error!"));
		return FALSE;
	}

	// 2005-01-24 
	// 5163의 경우에는 대출자번호에 넣고 공공도서관의 경우에는 대출자명에 넣는다.
	//
	// 육사에서는 5163도 _T("Y")이지만, 대출자이름에 대출자번호가 나오지 않는다.
	// 2005-09-28 강봉수
	//
	CString strIs5163, strIs64;
	GetManageValue(_T("기타"), _T("공통"), _T("5163"), _T("5163"), strIs5163);
	GetManageValue(_T("기타"), _T("공통"), _T("육사"), _T("육사"), strIs64);
	//=====================================================
	//2008.11.25 ADD BY PJW : 대출자번호 자동검색가능하도록 추가
// 	if( _T("Y") == strIs5163 && _T("Y") != strIs64 )
// 		m_pCM->SetControlMgrData(_T("대출자번호"), m_strUserID);
// 	else
// 		m_pCM->SetControlMgrData(_T("대출자명"), m_strUserID);
	if( _T("Y") == strIs5163 && _T("Y") != strIs64 )
	{
		m_pCM->SetControlMgrData(_T("대출자번호"), m_strUserID);
	}
	else
	{
		m_pCM->SetControlMgrData(_T("대출자명"), m_strUserName);
		m_pCM->SetControlMgrData(_T("대출자번호"), m_strUserID);
	}
	//=====================================================

	if( m_bAutoSearch == TRUE )		OnbtnSearch();

	//===================================================
	//2010.05.17 ADD BY PJW : 대출자번호는 다이얼로그 초기화에는 보여주지 않는다.
	CEdit* pEdtUserNO = (CEdit*)GetDlgItem(IDC_edtUserNO);
	pEdtUserNO->SetWindowText(_T(""));
	//===================================================

	// 2010.09.03 ADD BY KSJ : 통합도서서비스 사용여부
	GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), m_sIsUnityLoanService );
	m_sIsUnityLoanService.TrimLeft();	m_sIsUnityLoanService.TrimRight();
	m_sIsUnityLoanService.MakeUpper();
	if(_T("Y")!=m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;
}

VOID CBO_ACQ_LOAN_USER_SEARCH_DLG::OnbtnSearch() 
{
	INT ids;
	CString strMsg, sWhere;		
	strMsg = sWhere = _T("");

	// 2007.04.06 update by pdw {{++++++++++++++++++++++++++++++++++++++++++++++++
	// 검색조건 유무 확인 방식 변경
	CString strUserNo, strUserName;
	m_pCM->GetControlMgrData(_T("대출자번호"), strUserNo);
	strUserNo.TrimLeft();strUserNo.TrimRight();
	m_pCM->GetControlMgrData(_T("대출자명"), strUserName);
	strUserName.TrimLeft();strUserName.TrimRight();
	if( strUserNo.GetLength() == 0 && strUserName.GetLength() == 0 )
	{
		ids = AfxMessageBox( _T("검색 조건이 부족하여 검색 시간이 오래 걸릴 수 있습니다.\r\n")
							 _T("무시하고 검색하시겠습니까? \r\n( 예 : 검색 , 아니오 : 검색 중지 ) ")
							 , MB_ICONWARNING | MB_YESNO );
		if( ids == IDNO )	return ;
	}
	// ----------------------------------------------------------------------}}
	// sql문 만들기
	ids = GetSearchWhereSql( sWhere );
	if( ids < 0 ) return ;

	m_Progress.SetText(_T(""));
	m_Progress.SetPos(0);	
	ids = m_pDM->RefreshDataManager( sWhere, &m_Progress );
	CString strValue1;
	ChangeSecuriyCivilNo(m_pDM,-1,_T("주민등록번호"));
	GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue1 );
	if( _T("Y") != strValue1 )
	{
		// 2010.09.03 ADD BY KSJ : 공통함수로 변경하도록 수정
		//DEL ChangeUserNo(m_pDM,_T("대출자번호"));
		CLocCommonAPI::ChangeSecuriyUserNo( this , m_pDM , -1 , _T("대출자번호") , m_sIsUnityLoanService );
	}

	// 2011.09.08 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < m_pDM->GetRowCount())
	{
		CString strEnc;
		CStringArray asDes1, asDes2, asDes3, asDes4;
		for(INT i = 0; i < m_pDM->GetRowCount(); i++)
		{
			m_pDM->GetCellData(_T("E_MAIL"), i, strEnc);
			asDes1.Add(strEnc);
			m_pDM->GetCellData(_T("근무지전화"), i, strEnc);
			asDes2.Add(strEnc);
			m_pDM->GetCellData(_T("자택전화"), i, strEnc);
			asDes3.Add(strEnc);
			m_pDM->GetCellData(_T("주민등록번호"), i, strEnc);
			asDes4.Add(strEnc);
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
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes2);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes3);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes4);
					pApi->CloseSocket();	
				}
				for(i = 0; i < m_pDM->GetRowCount(); i++)
				{
					m_pDM->SetCellData(_T("E_MAIL"), asDes1.GetAt(i), i);
					m_pDM->SetCellData(_T("근무지전화"), asDes2.GetAt(i), i);
					m_pDM->SetCellData(_T("자택전화"), asDes3.GetAt(i), i);
					m_pDM->SetCellData(_T("주민등록번호"), asDes4.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
	}

	m_Progress.SetPos(100);
	strMsg.Format(_T("검색결과 : %d 건"), m_pDM->GetRowCount());
	m_Progress.SetText(strMsg);	
	
	// 소속, 직급은 desc 로 보여준다.
	// 2006.01.03 강봉수 DM/CM수정
	//ids = UserCodeToDesc();	
	if( ids >= 0 )	m_pGrid->Display();	
}

INT CBO_ACQ_LOAN_USER_SEARCH_DLG::GetSearchWhereSql( CString &sWhere ) 
{
	// 2007.04.02 add by pdw {{++++++++++++++++++++++++++++++++++++++++++++++++
	// 탈퇴회원 검색에서 제외되도록 변경
	// 2007.07.26 UPDATE BY PJW : 이용자 검색시 주민번호 사용안함으로 입력했을 경우 비치희망에서 검색되도록 수정
	// sWhere += _T(" USER_NO IS NOT NULL AND CIVIL_NO IS NOT NULL ");
	/*	
	CString strQuery, strValue;
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '열람' AND GROUP_3 = '공통' AND CLASS_ALIAS = '이용자관리' AND VALUE_NAME = '주민등록번호사용유무'"));
	m_pDM->GetOneValueQuery(strQuery, strValue);
	if ( strValue == 'N' )
	{
		sWhere += _T(" USER_NO IS NOT NULL");
	}
	else
	{
		sWhere += _T(" USER_NO IS NOT NULL AND CIVIL_NO IS NOT NULL ");		
	}
	*/
	// 2007.07.26 UPDATE BY PJW : 개인보호 정책 문제로 검색조건에서 주민번호 제외
	//++2008.05.13 UPDATE BY PWR {{+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//sWhere += _T(" USER_NO IS NOT NULL");
	//=====================================================
	//2008.11.25 UPDATE BY PJW : 통합시스템 타도서관 이용자도 비치신청할수 있음으로 MANAGE_CODE 제외 한다.
// 	sWhere += _T(" (CLU.USER_CLASS!='3' OR CLU.MEMBER_CLASS!='1') AND MANAGE_CODE = UDF_MANAGE_CODE");
	sWhere += _T(" (CLU.USER_CLASS!='3' OR CLU.MEMBER_CLASS!='1')");
	//=====================================================
	//=====================================================
	//--2008.05.13 UPDATE BY PWR -----------------------------------------------------}}
	
	// ----------------------------------------------------------------------}}
	// 대출자번호
	// 20070618 ADD PJW : 대출자 번호 	if( sWhere.GetLength() > 1 )	sWhere += _T(" AND "); 추가
	CString strData;
	m_pCM->GetControlMgrData(_T("대출자번호"), strData);
	
	if( !strData.IsEmpty() )
	{
		//=====================================================
		//2008.11.25 REM BY PJW : 대출자 번호는 KEYWORD를 생성하지 않는다.
// 		pMakeSearchData->GetKeyWord(strData);
		//=====================================================

		if( sWhere.GetLength() > 1 )	sWhere += _T(" AND ");
		sWhere += _T(" USER_NO = '") + strData + _T("' ");
	}
	//=====================================================
	// 대출자명
	m_pCM->GetControlMgrData(_T("대출자명"), strData);
	if( !strData.IsEmpty() )
	{
		pMakeSearchData->GetKeyWord(strData);
		if( sWhere.GetLength() > 1 )	sWhere += _T(" AND ");
		sWhere += _T(" IDX_USER_NAME LIKE ('") + strData + _T("%') ");		
	}
	

	return 0;
}


VOID CBO_ACQ_LOAN_USER_SEARCH_DLG::OnOK() 
{
	// 선택된 그리드의 rec_key 값을 멤버변수 m_sSelectRecKey에 저장한다.
	INT nRowIdx = -1;
	nRowIdx = m_pGrid->GetIdx();

	CString strSelectRecKey = _T("");
	strSelectRecKey = m_pDM->GetCellData( _T("REC_KEY"), nRowIdx );
	//===================================================
	//2010.11.17 ADD BY PJW : 비치희망제한 추가
	INT nOpenMode = 0;
	nOpenMode = GetOpenMode();
	if( 1 == nOpenMode && _T("") != strSelectRecKey )
	{
		CString strQuery = _T("");
		CString strLimitPeriod = _T("");
		CString strLimitCnt = _T("");
		CString strReservation = _T("");
		INT nLimitPeriod = 0;
		INT nLimitCnt = 0;
		INT nReservation = 0;
		//1. 예약취소제한적용기간 구하기
		strQuery.Format(_T("SELECT NVL(PREENGAGE_CANCEL_LIMIT_PERIOD, 0) FROM CD_CODE_TBL WHERE CLASS=31 AND CODE=(SELECT USER_CLASS_CODE FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s)"), strSelectRecKey);
		m_pDM->GetOneValueQuery(strQuery, strLimitPeriod);
		//2. 예약취소제한갯수 구하기
		strQuery.Format(_T("SELECT NVL(PREENGAGE_CANCEL_LIMIT_COUNT, 0) FROM CD_CODE_TBL WHERE CLASS=31 AND CODE=(SELECT USER_CLASS_CODE FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s)"), strSelectRecKey);
		m_pDM->GetOneValueQuery(strQuery, strLimitCnt);
		//3. 예약취소제한 적용기간동안의 예약취소한 건수 구하기
		//*주의사항:1번과2번으로구한값이 둘다 0이상일때만 제한한다.
		//(1번과2번 둘중 하나라도 0이면 아래 쿼리 수행안하고 제한안함)
		nLimitPeriod = _ttoi(strLimitPeriod);
		nLimitCnt = _ttoi(strLimitCnt);
		if ( 0 < nLimitPeriod && 0 < nLimitCnt )
		{
			CString strSubQuery_1 = _T("");
			CString strSubQuery_2 = _T("");
			strSubQuery_1.Format(_T("SELECT COUNT(1) FROM LS_WORK_T01 WHERE STATUS='4' AND USER_KEY=%s AND RESERVATION_CANCEL_TYPE='0' AND RESERVE_CANCEL_DATE BETWEEN TO_DATE(SYSDATE-%s, 'YYYY/MM/DD') AND TO_DATE(SYSDATE, 'YYYY/MM/DD')+0.99999"), strSelectRecKey, strLimitPeriod);
			strSubQuery_2.Format(_T("SELECT COUNT(1) FROM CO_LOAN_TBL WHERE STATUS='4' AND USER_KEY=%s AND RESERVATION_CANCEL_TYPE='0' AND RESERVE_CANCEL_DATE BETWEEN TO_DATE(SYSDATE-%s, 'YYYY/MM/DD') AND TO_DATE(SYSDATE, 'YYYY/MM/DD')+0.99999"), strSelectRecKey, strLimitPeriod);
			strQuery.Format(_T("SELECT (%s) + (%s) FROM DUAL"), strSubQuery_1, strSubQuery_2);
			m_pDM->GetOneValueQuery(strQuery, strReservation);
			nReservation = _ttoi(strReservation);
			//4. 3번에서 구한 예약취소한 건수가 예약취소제한갯수보다 같거나 크면 제한한다.
			//EX: 3번에서구한예약취소한건수 >= 2번에서구한예약취소제한갯수 이면 제한
			if( nReservation >= nLimitCnt )
			{
				AfxMessageBox(_T("비치희망 예약신청이 제한된 이용자 입니다."));
				return;
			}
		}
	}
	//===================================================	
	
	//JOB.2020.0004 ADD BY KYJ : 회원구분 비치희망신청 제한기능 추가
	CString strMemberClass = _T("");
	
	if( m_pInfo->m_bRestictionToApply == TRUE ){
		strMemberClass = m_pDM->GetCellData( _T("회원구분"), nRowIdx );
		if (strMemberClass != _T("정회원"))
		{
			AfxMessageBox(_T("준회원,비회원은 희망자료 신청이 제한됩니다."));
			return;
		}
	}

	m_sSelectRecKey = strSelectRecKey;
	//job.2018.0071
	//before
	//==============================================================================
	//m_strFurnishCnt = m_pDM->GetCellData( _T("신청건수"), nRowIdx );
	//==============================================================================
	//after 
	//==============================================================================
	if ( m_pInfo->m_bFurnishWishLimitYN )
	{
		m_strFurnishCnt = m_pDM->GetCellData( _T("신청건수"), nRowIdx );
	}
	else if (  m_pInfo->m_bFurnishLimitByTheWeek )
	{
		m_strFurnishCnt = m_pDM->GetCellData( _T("이번주신청건수"), nRowIdx );
	}
	//==============================================================================

	if( m_sSelectRecKey.IsEmpty() )
		CDialog::OnCancel();	
	else
		CDialog::OnOK();
}

INT CBO_ACQ_LOAN_USER_SEARCH_DLG::UserCodeToDesc() 
{
	// 대출소속정보 , 대출직급정보
	const INT nCodeCnt = 2;
	TCHAR *sCodeAlias[nCodeCnt] =
	{
		_T("대출소속정보") , _T("대출직급정보") 
	};
	TCHAR *sFieldAlias[nCodeCnt] =
	{
		_T("대출소속정보") , _T("대출직급정보") 
	};

	INT ids;
	for( INT i = 0 ; i < nCodeCnt ; i++ )
	{
		ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM( m_pDM , sCodeAlias[i] , sFieldAlias[i] );
		if( ids < 0 ) return -1;
	}

	return 0;
}

VOID CBO_ACQ_LOAN_USER_SEARCH_DLG::SetUserID(CString strUserID)
{
	m_strUserID = strUserID;
	if( m_strUserID.IsEmpty() == FALSE )	m_bAutoSearch = TRUE;
}
//=====================================================
//2008.11.25 ADD BY PJW : 대출자번호 자동검색가능하도록 추가
VOID CBO_ACQ_LOAN_USER_SEARCH_DLG::SetUserName(CString strUserName)
{
	m_strUserName = strUserName;
	if( m_strUserName.IsEmpty() == FALSE )	m_bAutoSearch = TRUE;
}
//=====================================================
// 2005.01.05 엔터키를 치면 검색이 되도록 추가하였다. 추가하니까.. 편하지?
BOOL CBO_ACQ_LOAN_USER_SEARCH_DLG::PreTranslateMessage(MSG* pMsg) 
{
 	if( pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		// 2006-01-03 강봉수 검색은 에디트박스에 있을때만 해야한다.(버튼이 안눌러짐)
		//GetDlgItem(IDOK)->SetFocus();
		if( GetDlgItem(IDC_edtUserName)->m_hWnd == pMsg->hwnd ||
			GetDlgItem(IDC_edtUserNO)->m_hWnd == pMsg->hwnd )
		{
			OnbtnSearch();
		//PostMessage(WM_COMMAND, IDC_btnSearch, BN_CLICKED);
		return TRUE;
	}		
		else if( GetDlgItem(IDC_gBO_LOAN_USER)->m_hWnd == pMsg->hwnd )	return TRUE;
	}		
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CBO_ACQ_LOAN_USER_SEARCH_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

//===================================================
//2009.10.08 ADD BY LKS : 주민번호/대출자번호 치환
INT CBO_ACQ_LOAN_USER_SEARCH_DLG::CivilNoCheck( CString sCivilNo )
{
	EFS_BEGIN

	INT ids;
	INT nDashIndex;
	CString sCivilNo1,sCivilNo2;

	if( sCivilNo.GetLength() != 14 )
		return 1;
	
	nDashIndex = sCivilNo.Find(_T("-"));

	if( nDashIndex != 6 ) 
		return 2;

	sCivilNo1 = sCivilNo.Mid(0,nDashIndex);
	sCivilNo2 = sCivilNo.Mid(nDashIndex+1);

	ids = CivilNoCheck( sCivilNo1 , sCivilNo2 );
	if( ids != 0 ) return 3;

	return 0;


	EFS_END
	return -1;

}

INT CBO_ACQ_LOAN_USER_SEARCH_DLG::CivilNoCheck( CString sCivilNo1, CString sCivilNo2 )
{
	EFS_BEGIN

	// 2007.2.9 add by pdw
	// 자릿수 체크
	if( 6 != sCivilNo1.GetLength() || 7 != sCivilNo2.GetLength() )
	{
		return -4;
	}

	INT sum, i; 

	sum = 0;

	// 생년 월일 체크
    // 3.생년월일의 값체크
    // 3.1.생년월의 체크
    INT nMonth = _ttoi(sCivilNo1.Mid(2,2));
    if(nMonth < 1 || nMonth > 12)
	{
        return -1;
    }

    // 3.2.생년일의 체크
    INT nDay = _ttoi(sCivilNo1.Mid(4,2));
    if(nMonth < 1 || nMonth > 31)
	{
        return -2;
    }
    
    // 4. 남성인지 여성인지 체크. 2002.08.06 이종은 추가 --> 외국인 주민등록 시작 번호 : 5, 6 2005.02.17소정추가: 7,8
    if( (sCivilNo2.GetAt(0)!='1') && (sCivilNo2.GetAt(0)!= '2') && (sCivilNo2.GetAt(0)!='3') && (sCivilNo2.GetAt(0)!='4') 
		&& (sCivilNo2.GetAt(0)!= '5') && (sCivilNo2.GetAt(0)!= '6') && (sCivilNo2.GetAt(0)!= '7') && (sCivilNo2.GetAt(0)!= '8') )
	{
        return -3;
    }

	if( sCivilNo2.GetAt(0)=='5' || (sCivilNo2.GetAt(0) == '6') || (sCivilNo2.GetAt(0)== '7') || (sCivilNo2.GetAt(0)== '8'))
	{
		INT ids;
		ids = CheckForeignCivilNum( sCivilNo1 + sCivilNo2 );
		if( ids != 0 ) return -1;
	}
	else
	{
		for( i = 0 ; i < 12 ; i++) 
		{ 
			if(i < 6) 
				sum += (sCivilNo1.GetAt(i)-'0') * (i+2); 
			if( i > 5 && i < 8 ) 
				sum += (sCivilNo2.GetAt(i-6)-'0') * (i+2); 
			
			if( i > 7 ) 
				sum += (sCivilNo2.GetAt(i-6)-'0') * (i-6); 
		} 
		
		INT nCheckDigit = 11-(sum%11);
		nCheckDigit = nCheckDigit%10;
		if( nCheckDigit != (sCivilNo2.GetAt(6)-'0') ) 
			return -1;
	}
	
	return 0;

	EFS_END
	return -1;

}

// 외국인 주민등록 번호 체크섬 검사
INT CBO_ACQ_LOAN_USER_SEARCH_DLG::CheckForeignCivilNum( CString civilNum )
{
/*
실제 외국인 주민 번호
641103-6220195
671210-6220196
670124-6100292
660515-6220197
650730-6220192
	*/
	
	// civilNum : 하이픈이 없는 연속된 13개 숫자
	
	
	const INT cnt = 13;
	INT num[ cnt ];
	INT sum = 0, odd = 0;
	for( INT i = 0; i < cnt; i++ )
		num[ i ] = _ttoi( civilNum.Mid( i, 1 ) );
	
	odd = num[ 7 ] * 10 + num[ 8 ];
	
	if( odd % 2 != 0 )
		return 1;
	
	if( 
		num[ 11 ] != 6 && 
		num[ 11 ] != 7 && 
		num[ 11 ] != 8 &&
		num[ 11 ] != 9 )
		return 2;
	
	INT checkNum[] = { 2, 3, 4, 5, 6, 7, 8, 9, 2, 3, 4, 5 };
	
	for( i = 0, sum = 0; i < 12; i++ )
		sum += ( num[ i ] *= checkNum[ i ] );
	sum = 11 - ( sum % 11 );
	
	if( sum >= 10 )
		sum -= 10;
	sum += 2;
	
	if( sum >= 10 )
		sum -= 10;
	
	if( sum != num[ 12 ] )
		return 3;
	
	return 0;
}

INT CBO_ACQ_LOAN_USER_SEARCH_DLG::ChangeSecuriyCivilNo( CESL_DataMgr* pDM , INT nIndex , CString sAlias )
{
	EFS_BEGIN

	// 2010.09.13 ADD BY KSJ : 주민번호 표현방식 변경
//DEL 	CString sCivilNo;
//DEL 	INT nStartIndex = 0;
//DEL 	INT nFinishIndex = pDM->GetRowCount();
//DEL 	
//DEL 	if( nIndex != -1 )
//DEL 	{
//DEL 		nStartIndex = nIndex;
//DEL 		nFinishIndex = nIndex + 1;
//DEL 	}
//DEL 
//DEL 	for( INT i = nStartIndex ; i < nFinishIndex ; i++ )
//DEL 	{
//DEL 		ids = pDM->GetCellData( sAlias , i , sCivilNo );
//DEL 
//DEL 		ChangeSecuriyCivilNo( sCivilNo );
//DEL 
//DEL 		pDM->SetCellData( sAlias , sCivilNo , i );
//DEL 		
//DEL 	}
	CLocCommonAPI::ChangeSecuriyCivilNo(this,pDM,nIndex,sAlias,m_sIsUnityLoanService);

	return 0;

	EFS_END

	return -1;
}

INT CBO_ACQ_LOAN_USER_SEARCH_DLG::ChangeSecuriyCivilNo( CString &sCivilNo )
{
	EFS_BEGIN

	// 2010.09.13 ADD BY KSJ : 주민번호 표현방식 변경
//DEL 	INT nDIndex = sCivilNo.Find(_T("-"));
//DEL 	INT nStartIndex = 6;
//DEL 
//DEL 	if( nDIndex > -1 )
//DEL 	{
//DEL 		nStartIndex = nDIndex + 1;
//DEL 	}
//DEL 
//DEL 	INT nLength = sCivilNo.GetLength();
//DEL 
//DEL 	for( INT i = nStartIndex ; i < nLength ; i++ )
//DEL 	{
//DEL 		sCivilNo.SetAt( i , '*' );
//DEL 	}
	CLocCommonAPI::ChangeSecuriyCivilNo(this,sCivilNo,m_sIsUnityLoanService);

	return 0;

	EFS_END

	return -1;

}

VOID CBO_ACQ_LOAN_USER_SEARCH_DLG::ChangeUserNo( CESL_DataMgr* pDM,CString sAlias)
{
	INT nCnt = pDM->GetRowCount();
	CString strUserNo;
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		INT ids = pDM->GetCellData( sAlias , i , strUserNo );

//DEL 		CString strBuf = strUserNo;
//DEL 		if( 13 <= strUserNo.GetLength() )
//DEL 			strBuf = strUserNo.Mid(0,6) + _T("-") + strUserNo.Mid(6);
//DEL 
//DEL 		ids = CivilNoCheck( strBuf );
//DEL 		if( 0 == ids )
//DEL 		{
//DEL 			// 2009.12.14 ADD BY CJY : 주민번호 표시방법 변경
//DEL 			//ids = CLocCommonAPI::ChangeSecuriyCivilNo( NULL, strUserNo );
//DEL 			strUserNo.Format(_T("%s%s******"),strBuf.Left(6),strBuf.Mid(7,1));
//DEL 
//DEL 			pDM->SetCellData( sAlias , strUserNo , i );
//DEL 		}
		CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_sIsUnityLoanService );
		pDM->SetCellData( sAlias , strUserNo , i );
	}
}
//===================================================