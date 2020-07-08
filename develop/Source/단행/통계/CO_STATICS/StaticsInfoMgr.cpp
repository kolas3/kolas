// StaticsInfoMgr.cpp : implementation file
//

#include "stdafx.h"
#include "StaticsInfoMgr.h"
#include <afxdisp.h>
// 2011.09.09 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define DIVCHAR			_T(",")
/////////////////////////////////////////////////////////////////////////////
// CStaticsInfoMgr dialog


CStaticsInfoMgr::CStaticsInfoMgr(CESL_Mgr* pParent /*=NULL*/, CString strMenuName )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CStaticsInfoMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sSMAlias = _T("STATICS_INFO_MGR");
	m_sXYSettingCMAlias = _T("CM_BO_STATICS_1120");

	m_pSearchCM = NULL;
	m_pOrderStaticsDM = NULL;

	m_strMenuName = strMenuName;

	// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
	m_pAxisCode_CellClick = NULL;
	// KOL.RED.2018.211 ---------------------------------------------------------------------------

}

CStaticsInfoMgr::~CStaticsInfoMgr()
{
	if( m_pOrderStaticsDM ) 
		delete[] m_pOrderStaticsDM;
	m_pOrderStaticsDM = NULL;

	if( m_pPrintMgr )
		delete m_pPrintMgr;
	m_pPrintMgr = NULL;

	// 2012.11.20 ADD BY PWR : 2012년도 도서관 통합서비스 확대구축 사업 2차 (속도개선)
	m_dmCodeMgr.FreeData();
}

VOID CStaticsInfoMgr::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStaticsInfoMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CStaticsInfoMgr, CDialog)
	//{{AFX_MSG_MAP(CStaticsInfoMgr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticsInfoMgr message handlers

// ##############################################################################################################
// [ 함수설명 ]
//   통계에 필요한 기본 정보 구성
// 
// [ 미니스펙 ]
//   
// ##############################################################################################################
INT CStaticsInfoMgr::MakeDMInfo()
{	
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. SM Init
	// ==========================================================================================================
	ids = InitESL_Mgr( m_sSMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeDMInfo()") );
		
	// ==========================================================================================================
	// 2. Manage Value 설정 
	// ==========================================================================================================
	ids = SetManageValue();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	// ==========================================================================================================
	// 3. DM 설정
	// ==========================================================================================================
	// 예정 컬럼명 DM
	m_pExpColumnDM = FindDM( _T("DM_BO_STATICS_COLUMN_NAME") );
	if( !m_pExpColumnDM ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	// 사용자 구성 검색 조건 정보 DM
	m_pUserMakeSearchCondDM = FindDM( _T("DM_BO_STATICS_USER_MAKE_CONDITION_INFO") );
	if( !m_pUserMakeSearchCondDM ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	m_pResultDM = FindDM( _T("DM_BO_STATICS_RESULT") );
	if( !m_pResultDM ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	m_pRealResultDM = FindDM( _T("DM_BO_STATICS_REAL_RESULT") );
	if( !m_pRealResultDM ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	m_pXSumDM = FindDM( _T("DM_XSUM") );
	if( !m_pXSumDM ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	m_pYSumDM = FindDM( _T("DM_YSUM") );
	if( !m_pYSumDM ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	m_pXPriceSumDM = FindDM( _T("DM_X_PRICE_SUM") );
	if( !m_pXPriceSumDM ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	m_pYPriceSumDM = FindDM( _T("DM_Y_PRICE_SUM") );
	if( !m_pYPriceSumDM ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	m_pUserSearchCond = FindDM( _T("DM_BO_STATICS_USER_SEARCH_COND") );
	if( !m_pUserSearchCond ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	// 통계 기본 정보 DM 구성하기
	m_pStaticsMainInfoDM = FindDM( _T("DM_BO_STATICS_MAIN_INFO") );
	if( !m_pStaticsMainInfoDM ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	ids = MakeStaticsMainInfoDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	// 컬럼 알리아스 정보 DM 구성하기
	m_pColumnAliasInfoDM = FindDM( _T("DM_BO_STATICS_COLUMN_ALIAS_INFO") );
	if( !m_pColumnAliasInfoDM ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("CStaticsInfoMgr::MakeDMInfo()") );

	ids = MakeColumnAliasInfoDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::MakeDMInfo()") );
	
	// 사용자검색 조건 정보 DM 구성하기
	ids = MakeUserSearchCondDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -16 , _T("CStaticsInfoMgr::MakeDMInfo()") );
	
	// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//
	// 2018.07.11 ADD BY KHY: 통계결과 리스트 제공
/*//NEW------------------------------------------------------------------------*/					
	m_pResultListDM = FindDM( _T("DM_BO_STATICS_LIST") );
	if( !m_pResultListDM ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CStaticsInfoMgr::MakeDMInfo()") );
/*//END------------------------------------------------------------------------*/
	// KOL.RED.2018.211 ---------------------------------------------------------------------------


	// 2012.11.20 ADD BY PWR : 2012년도 도서관 통합서비스 확대구축 사업 2차 (속도개선)
	CString strQuery = _T("SELECT CLASS, TRIM(CODE), DESCRIPTION FROM CD_CODE_TBL ORDER BY CLASS, CODE");
	m_dmCodeMgr.SetCONNECTION_INFO(_T(""));
	m_dmCodeMgr.InitDataMgrRef(3);
	m_dmCodeMgr.SetDataMgrRef(0, _T("CLASS"), _T("CLASS"), _T("str"), _T(""), _T(""));
	m_dmCodeMgr.SetDataMgrRef(1, _T("CODE"), _T("CODE"), _T("str"), _T(""), _T(""));
	m_dmCodeMgr.SetDataMgrRef(2, _T("DESCRIPTION"), _T("DESCRIPTION"), _T("str"), _T(""), _T(""));
	m_dmCodeMgr.RestructDataManager(strQuery);

	// ==========================================================================================================
	// 4. Print 설정
	// ==========================================================================================================	
	m_pPrintMgr = new CStatPrintMgr(this);

	return 0;	

EFS_END
	return -1;
}

// ##############################################################################################################
// [ 함수설명 ]
//   기본 정보 구성
//   FILE에서 얻어와 구성한다.   
// [ 미니스펙 ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::MakeStaticsMainInfoDM()
{
EFS_BEGIN

	INT ids;
	
	// ==========================================================================================================
	// 1. 도서관 Flag에 따라 Path가 달라진다.
	// ==========================================================================================================
	// 경로 확인
	CString strDir;
	GetDir( strDir );

	// 18.09.27 JMJ KOL.RED.2018.008
	// 책두레 사용 도서관은 책두레 자료 포함
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	CString strQuery;
	CString strValue;
	strQuery.Format(_T("SELECT MIN(VALUE_1) FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='책두레시스템' AND VALUE_NAME='책두레사용여부' AND MANAGE_CODE = '%s'"),m_pInfo->MANAGE_CODE);
	dm.GetOneValueQuery(strQuery,strValue);			
	CString sMenuName = GetMenuName();	


	CString strPath;
	if ( m_sIs64lib == _T("Y") )	        strPath = strDir + _T("기본정보_육사.txt");
	else if ( m_sIsNDlib == _T("Y") )		strPath = strDir + _T("기본정보_국방대.txt");
	else if ( m_sIs5163lib == _T("Y") )		strPath = strDir + _T("기본정보_5163.txt");

	// 18.09.27 JMJ KOL.RED.2018.008
	// 책두레 사용 도서관은 책두레 자료 포함
	else if (strValue.Compare(_T("Y"))==0 && sMenuName.Compare( LOAN_STATICS ) == 0)	strPath = strDir + _T("기본정보_책두레포함.txt");
	else if (sMenuName.Compare(BOOK_STATICS) == 0)
	{
		CESL_ControlMgr* pCM = pParentMgr->FindCM(_T("CM_BO_LOC_2100"));		
		CESL_Control_Element* pIncludeAppendix = pCM->FindControlElement( _T("부록포함체크") );		
		CButton* btn1 = (CButton*)(pParentMgr->GetDlgItem( pIncludeAppendix->CTRL_ID ));
		if( btn1 == NULL ) {
			btn1 = (CButton*)(pParentMgr->pParentMgr->GetDlgItem( pIncludeAppendix->CTRL_ID ));
		}
		if( btn1 == NULL || btn1->GetCheck() == FALSE ) 
		{
			strPath = strDir + _T("기본정보.txt");
		}
		else
		{
			strPath = strDir + _T("기본정보_부록.txt");			
		}
	}
	else								strPath = strDir + _T("기본정보.txt");

	// ==========================================================================================================
	// 2. 선택된 파일로 DM 구성
	// ==========================================================================================================
	ids = MakeFileToDM( strPath , m_pStaticsMainInfoDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeStaticsMainInfoDM()") );

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ 함수설명 ]
//   컬럼알리아스 정보 구성
//   FILE에서 얻어와 구성한다.   
// [ 미니스펙 ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::MakeColumnAliasInfoDM()
{
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. 도서관 Flag에 따라 Path가 달라진다.
	// ==========================================================================================================
	// 경로 확인
	CString strDir;
	GetDir( strDir );

	CString strPath;
	if ( m_sIsNDlib == _T("Y") )			strPath = strDir + _T("컬럼알리아스정보_국방대.txt");
	else if ( m_sIs5163lib == _T("Y") )		strPath = strDir + _T("컬럼알리아스정보_5163.txt");
	else								strPath = strDir + _T("컬럼알리아스정보.txt");	

	// ==========================================================================================================
	// 2. 선택된 파일로 DM 구성
	// ==========================================================================================================
	ids = MakeFileToDM( strPath , m_pColumnAliasInfoDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeColumnAliasInfoDM()") );

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ 함수설명 ]
//   사용자 검색조건 정보 구성
//   FILE에서 얻어와 구성한다.   
// [ 미니스펙 ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::MakeUserSearchCondDM()
{
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. 도서관 Flag에 따라 Path가 달라진다.
	// ==========================================================================================================
	// 경로 확인
	CString strDir;
	GetDir( strDir );

	CString strPath = strDir + _T("사용자검색조건정보.txt");

	// ==========================================================================================================
	// 2. 선택된 파일로 DM 구성
	// ==========================================================================================================
	ids = MakeFileToDM( strPath , m_pUserMakeSearchCondDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeUserSearchCondDM()") );

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ 함수설명 ]
//   파일 정보를 DM으로 구성한다.
//   
// [ 미니스펙 ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::MakeFileToDM( CString sFilePath , CESL_DataMgr *pDM )
{
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. 파일 열기
	// ==========================================================================================================
	CStdioFile fMainInfo;
	ids = fMainInfo.Open( sFilePath , CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary );
	if( ids==0 )
	{
		AfxMessageBox( _T("파일명이 잘못되었습니다.\n올바른 경로인지 확인해 주십시오.") );
		return 1;
    }
	TCHAR cUni;
	fMainInfo.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fMainInfo.SeekToBegin();
	}

	if( fMainInfo.GetLength() == 0 ) 
	{
		fMainInfo.Close();
		return 0;
	}

	// ==========================================================================================================
	// 2. 컬럼 리스트 구성
	//    최상단의 첫줄을 컬럼명으로 본다
	// ==========================================================================================================
	CString strLine;
	CArray < CString , CString > asColumnList;

	fMainInfo.ReadString( strLine );
	strLine.TrimRight(); strLine.TrimLeft();

	ids = CLocCommonAPI::MakeArrayList( strLine , _T("|") , asColumnList );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeStaticsMainInfoDM()") );

	// ==========================================================================================================
	// 3. DM에 입력
	// ==========================================================================================================
	CArray < CString , CString > asDataList;

    while ( fMainInfo.ReadString( strLine ) ) 
	{
        strLine.TrimRight(); strLine.TrimLeft();
		if( strLine.IsEmpty() ) continue;

		// 2011.10.04 ADD BY PWR : 통합회원여부 추가 관련
		if(FALSE == m_pInfo->m_bKLUse)
		{
			if(-1 < sFilePath.Find(_T("컬럼알리아스정보")) && -1 < strLine.Find(_T("통합회원구분별"))) continue;
		}

		pDM->InsertRow(-1);

		ids = CLocCommonAPI::MakeArrayList( strLine , _T("|") , asDataList );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeStaticsMainInfoDM()") );

		for( INT i = 0 ; i < asDataList.GetSize() ; i++ )
		{
			CString strManageCode;
			CString strData;
			strData = asDataList.GetAt(i);
			strManageCode.Format(_T("'%s'"), m_pInfo->MANAGE_CODE);
			strData.Replace(_T("UDF_MANAGE_CODE"), strManageCode);

			// 2011.10.04 ADD BY PWR : 통합회원여부 추가 관련
			if(FALSE == m_pInfo->m_bKLUse) strData.Replace(_T(",통합회원구분별"), _T(""));
			ids = pDM->SetCellData( asColumnList.GetAt(i) , strData , pDM->GetRowCount()-1 );
			if( ids < 0 )
			{
				CString sMsg;
				sMsg.Format( _T("LINE : %s , pDM->SetCellData(%s,%s,%d) "), strLine ,  asColumnList.GetAt(i) , asDataList.GetAt(i) , pDM->GetRowCount()-1 );
				AfxMessageBox( sMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::MakeStaticsMainInfoDM()") );
		}
	}

	fMainInfo.Close();

	return 0;

EFS_END
	return -1;
}

// 컬럼의 내용을 얻어온다.
// nMode에 따라 설명이나 코드를 가져온다.
INT CStaticsInfoMgr::GetColumnNames( CString sStaticsClass , CString sAlias , CStringArray &asColumnName , INT nMode )
{
EFS_BEGIN

	INT ids;
	
	asColumnName.RemoveAll();

	if( m_pExpColumnDM->GetRowCount() < 1 )
		m_pExpColumnDM->FreeData();

	CString sTableName;
	CString sExpColumnName;

	// 테이블 이름 설정
	// 검색 조건 설정
	CString sColumnAlias;
	if( nMode == 2 )
		sColumnAlias = _T("예정코드");
	else 
		sColumnAlias = _T("예정설명");
	ids = GetCellData( m_pColumnAliasInfoDM , _T("알리아스명") , sAlias , _T("테이블명") , sTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetColumnNames") );

	ids = GetCellData( m_pColumnAliasInfoDM , _T("알리아스명") , sAlias , sColumnAlias , sExpColumnName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetColumnNames") );

	// 만약 테이블이 존재하지 않는다면
	if( sTableName.IsEmpty() )
	{
		// 년월일에 관련된 알리아스가 존재한다면 그걸루 계산한다. 
		CString sDate1 , sDate2;
		if( (sAlias.Compare(_T("년"))==0) || (sAlias.Compare(_T("월"))==0) || (sAlias.Compare(_T("일"))==0) || (sAlias.Compare(_T("시간대별"))==0) )
		{
			ids = GetTimeColumnNames( sStaticsClass , sAlias , asColumnName );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetColumnNames") );

		}
		// 2009.10.19 ADD BY CJY : 성별추가
		// 이곳에서 칼럼이름을 추가해주고, 칼럼알리아스정보파일의 성별항목의 그룹여부를 Y에서 N으로 변경하였음

 		// 2012.11.20 UPDATE BY BJS : 2012년도 도서관 통합서비스 확대구축 사업 2차 (개인정보보호 실명인증 사용에 따른 성별 통계 형식)
		//if(sAlias.Compare(_T("성별"))==0 )
		if(sAlias.Compare(_T("성별"))==0 || sAlias.Compare(_T("성별2"))==0 )
 		{
			asColumnName.Add(_T("남"));
 			asColumnName.Add(_T("여"));
 			asColumnName.Add(_T("알수없음"));
			
 		}
		
		if( asColumnName.GetSize() != 0 ) return 0;

		if( sExpColumnName.IsEmpty() ) return 0;
	
		// 순위라면 그냥 간다.
		ids = CLocCommonAPI::MakeArrayList( sExpColumnName , _T(",") , asColumnName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetColumnNames") );
	}
	// 테이블이 존재한다면
	else
	{
		// 2012.11.20 UPDATE BY PWR : 2012년도 도서관 통합서비스 확대구축 사업 2차 (속도개선)
		CString sOption,sTableName,sCodeField,sDescField;
		ids = GetCellData( m_pColumnAliasInfoDM , _T("알리아스명") , sAlias , _T("WHERE절") , sOption );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetColumnNames") );
		ids = GetCellData( m_pColumnAliasInfoDM , _T("알리아스명") , sAlias , _T("테이블명") , sTableName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetColumnNames") );
		
		// 18.09.27 JMJ KOL.RED.2018.008
		m_pExpColumnDM->TBL_NAME = sTableName;

		ids = GetCellData( m_pColumnAliasInfoDM , _T("알리아스명") , sAlias , _T("예정코드") , sCodeField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetColumnNames") );
		ids = GetCellData( m_pColumnAliasInfoDM , _T("알리아스명") , sAlias , _T("예정설명") , sDescField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetColumnNames") );
		if(_T("CD_CODE_TBL") == sTableName)
		{ // 코드값은 이미 가져온 값으로 활용
			// 13/11/18 박대우 : 2012년도 기능개선패치 수정작업(20130604005)
			//                   지역내대출정지를 별개 서비스로 가져오도록 수정
/*//BEFORE-----------------------------------------------------------------------
			CString strValue;
			CString strClass = _T("");
			bool bIsClass = FALSE;
			int nFirstIdx = sOption.Find(_T("="));
			if(0 < nFirstIdx)
			{
				int nLastIdx = sOption.Find(_T("ORDER"));
				if(0 < nLastIdx && nFirstIdx < nLastIdx)
				{
					strClass = sOption.Mid(nFirstIdx+1, nLastIdx-nFirstIdx-1);
					strClass.TrimLeft(); strClass.TrimRight();
				}
			}
			if(strClass.IsEmpty()) return 0;
			for(INT i = 0; i < m_dmCodeMgr.GetRowCount(); i++)
			{
				m_dmCodeMgr.GetCellData(_T("CLASS"), i, strValue);
				if(strValue == strClass)
				{
					bIsClass = TRUE;
					if(2 == nMode)
					{ // CODE
						m_dmCodeMgr.GetCellData(_T("CODE"), i, strValue);
					}
					else
					{ // DESC
						m_dmCodeMgr.GetCellData(_T("DESCRIPTION"), i, strValue);
					}
					asColumnName.Add(strValue);
				}
				else
				{ // ORDER BY CLASS, CODE 이므로 해당 CLASS였다가 바뀌면 중지
					if(TRUE == bIsClass)
					{
						break;
					}
				}
			}
*///AFTER------------------------------------------------------------------------
			CString strClass;
			int nFind1 = sOption.Find(_T("="));
			if(0 < nFind1)
			{
				int nFind2 = sOption.Find(_T("ORDER"));
				if (0 < nFind2 && nFind1 < nFind2)
				{
					strClass = sOption.Mid(nFind1+1, nFind2-nFind1-1);
					strClass.TrimLeft(); strClass.TrimRight();
				}
			}

			if (strClass.IsEmpty()) 
			{
				return 0;
			}

			CString strValueType = (nMode == 2) ? _T("CODE") : _T("DESCRIPTION");
			CString strValue;

			int nCount = m_dmCodeMgr.GetRowCount();
			int nIndex = nCount;
			while (--nIndex >= 0)
			{
				m_dmCodeMgr.GetCellData(_T("CLASS"), nIndex, strValue);
				if (strClass.CompareNoCase(strValue) == 0)
				{
					m_dmCodeMgr.GetCellData(strValueType, nIndex, strValue);
					strValue.TrimLeft();strValue.TrimRight();
					
					if (strValue.Compare(_T("적용안함")) == 0 || (strValue.IsEmpty()))
					{
						asColumnName.Add(strValue); // 마지막에 추가되도록
					}
					else
					{
						// KOL.RED.2018.210 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
						//asColumnName.InsertAt(0, strValue);	// 역순으로 입력되도록					
						if (nMode == 9)
						{
							CString strCode;
							m_dmCodeMgr.GetCellData(_T("CODE"), nIndex, strCode);

							CString strDispValue;
							strDispValue.Format(_T("%s(%s)"), strValue, strCode);

							asColumnName.InsertAt(0, strDispValue);	// 역순으로 입력되도록					
						}
						else
						{
						asColumnName.InsertAt(0, strValue);	// 역순으로 입력되도록					
					}
						// KOL.RED.2018.210 ---------------------------------------------------------------------------						
					}
				}
				else
				{
					if (asColumnName.GetSize())
						break; // 한번이라도 입력이 되었다면
				}
			}
/*//END------------------------------------------------------------------------*/
		}
		else
		{
			m_pExpColumnDM->TBL_NAME = sTableName;
			m_pExpColumnDM->SetDataMgrRef( 0 , _T("코드명") , sCodeField , _T("str") , _T("") ,  _T("") );
			m_pExpColumnDM->SetDataMgrRef( 1 , _T("컬럼명") , sDescField , _T("str") , _T("") ,  _T("") );
			ids = m_pExpColumnDM->RefreshDataManager(sOption);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetColumnNames") );

			CString sColumn;
			CString sColumnAlias;
			CString sColumnName;
			CString sEmptyColumn;
			BOOL IsEmptyColumn = FALSE;

			if ( sAlias==_T("분류기호별(DDC)") && m_sIs64lib == _T("Y") )
			{
				sColumnAlias = _T("컬럼명");
				m_pExpColumnDM->SetCellData(sColumnAlias, _T("3\n355-359"), 4 );
			}

			for( INT i = 0 ; i < m_pExpColumnDM->GetRowCount() ; i++ )
			{
				if( nMode != 2 ) sColumnAlias = _T("컬럼명");
				else sColumnAlias = _T("코드명");
				
				ids = m_pExpColumnDM->GetCellData( sColumnAlias , i , sColumn );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetColumnNames") );

				ids = m_pExpColumnDM->GetCellData( _T("컬럼명") , i , sColumnName );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetColumnNames") );

				sColumnName.TrimLeft(); sColumnName.TrimRight();
				if( sColumnName.Compare(_T("적용안함")) == 0 || (sColumnName.IsEmpty()) )
				{
					IsEmptyColumn = TRUE;
					sEmptyColumn = sColumn;
					continue;
				}

				asColumnName.Add( sColumn );
			}

			if( IsEmptyColumn )
				asColumnName.Add( sEmptyColumn );
		}
	}

	return 0;

EFS_END
	return -1;
}


INT CStaticsInfoMgr::GetCellData( CESL_DataMgr *pDM , CString sConditionAlias , CString sConditionData , CString sGetAlias , CString &sGetData )
{
	EFS_BEGIN

	INT ids;

	CString sTmpData;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( sConditionAlias , i , sTmpData );
		if( ids < 0 ) continue;
		/*
		if( ids < 0 )
		{
			CString sMsg;
			sMsg.Format( _T("ALIAS : %s , Data : %s ") , sConditionAlias , sTmpData );
			AfxMessageBox( sMsg );
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetCellData") );
		*/
		if( sTmpData.Compare( sConditionData ) == 0 )
		{
			ids = pDM->GetCellData( sGetAlias , i , sGetData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetCellData") );

			return 0;
		}
	}

	return 1;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::SetXYAlias( CString sStaticsClass , CString sXAlias , CString sYAlias , CString sOption , CString sOrder )
{
	EFS_BEGIN

	INT ids;
	

	// 14/08/20 박대우 : 성별 추가
/*// BEFORE ----------------------------------------------------------------------------------
	// 2012.11.20 ADD BY BJS : 2012년도 도서관 통합서비스 확대구축 사업 2차 (개인정보보호 실명인증 사용에 따른 성별 통계 형식)
	// 개인정보보호 사용 시 컬럼알리아스정보의 성별 대신 성별2를 사용(GPIN_SEX)하여 성별 통계를 낸다
	CString strQuery,strTemp,strCertify;
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='개인정보보호' AND VALUE_NAME='실명인증사용여부' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_pStaticsMainInfoDM->GetOneValueQuery(strQuery,strTemp);
	if(_T("Y") == strTemp) strCertify.Format(_T("실명인증사용"));

	if(strCertify.Compare(_T("실명인증사용"))==0 && sXAlias.Compare(_T("성별"))==0)
	{
		sXAlias.Format(_T("성별2"));
	}
*/// AFTER -----------------------------------------------------------------------------------
	// 주민등록번호 사용유무에 따라서 동작하도록 기능 변경
	if (sXAlias.Compare(_T("성별")) == 0)
	{
		CString strQuery;
		CString strValue;
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='이용자관리' AND VALUE_NAME='주민등록번호사용유무' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_pStaticsMainInfoDM->GetOneValueQuery(strQuery, strValue);

		if (strValue.CompareNoCase(_T("Y")) != 0)
		{
			sXAlias.Format(_T("성별2"));
		}
	}
//*/ END -------------------------------------------------------------------------------------
	
	///////////////////////////////////////////////////////////////////////////////////////////////


 	ids = SetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("X축선택알리아스") , sXAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetXYAlias") );

	ids = SetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("Y축선택알리아스") , sYAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SetXYAlias") );

	ids = SetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("단위") , sOption );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetXYAlias") );

	ids = SetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("순위여부") , sOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetXYAlias") );

	return 0;

	EFS_END
	return -1;

}

// 
INT CStaticsInfoMgr::SetCellData( CESL_DataMgr *pDM , CString sCAlias , CString sCData , CString sIAlias , CString &sIData )
{
EFS_BEGIN

	INT ids;
	
	CString sTmpData;
	for ( INT i = 0;i < pDM->GetRowCount();i++ )
	{
		ids = pDM->GetCellData( sCAlias , i , sTmpData );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::SetCellData()") );

		if ( sTmpData.Compare( sCData ) != 0 ) continue;

		ids = pDM->SetCellData( sIAlias , sIData , i );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::SetCellData()") );

		return 0;
	}

	return 1;

EFS_END
	return -1;
}

INT CStaticsInfoMgr::GetXYAlias( CString sStaticsClass , CString &sXAlias , CString &sYAlias , CString &sOption , CString &sOrder )
{
	EFS_BEGIN

	INT ids;

	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("X축선택알리아스") , sXAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetXYAlias") );

	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("Y축선택알리아스") , sYAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetXYAlias") );

	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("단위") , sOption );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetXYAlias") );

	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("순위여부") , sOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetXYAlias") );

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::GetUserSearchCondFieldName( CString sAlias , CString &sFieldName )
{
	EFS_BEGIN

	INT ids;

	ids = GetCellData( m_pUserMakeSearchCondDM , _T("알리아스") , sAlias , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetUserSearchCondFieldName") );

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::SetUserCondition( CString sStaticsClass , CString sWhere )
{
	EFS_BEGIN

	INT ids = SetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("사용자검색조건") , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetXYAlias") );

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::GetUserCondition( CString sStaticsClass , CString &sWhere )
{
	EFS_BEGIN

	INT ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("사용자검색조건") , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetXYAlias") );

	return 0;

	EFS_END
	return -1;

}

// ##############################################################################################################
// [ 함수설명 ]
//   조건 생성
//   진짜 결과 DM을 생성하는데 주력한다.
//
// [ 미니스펙 ]
// 
// ##############################################################################################################
INT CStaticsInfoMgr::MakeStaticsCondition( CString sStaticsClass )
{
EFS_BEGIN

	INT ids;

	CString sUserCondition;
	CString sXAlias;
	CString sYAlias;

	// ==========================================================================================================
	// 1. 사용자 검색 조건이 생성되어 있는지 조사
	//    장서 점검통계라면 무시
	// ==========================================================================================================	
	if( GetMenuName().Compare( BOOK_STATICS ) != 0 )
	{		
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("사용자검색조건") , sUserCondition );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeStaticsCondition()") );

		if ( sUserCondition.IsEmpty() )
		{
			AfxMessageBox( _T("조건 설정을 먼저 하시고 통계를 진행하십시오.") );
			return 101;
		}
	}

	// ==========================================================================================================	
	// 2. 행, 열이 생성되어 있는지 조사
	// ==========================================================================================================	
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("X축선택알리아스") , sXAlias );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeStaticsCondition()") );
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("Y축선택알리아스") , sYAlias );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::MakeStaticsCondition()") );

	if( sXAlias.IsEmpty() || sYAlias.IsEmpty() )
	{
		AfxMessageBox( _T("행, 열 설정을 먼저 하시고 통계를 진행하십시오.") );
		return 102;
	}

	// ==========================================================================================================
	// 3. 통계 결과 DM 구성
	// ==========================================================================================================
	ids = MakeRealDM( sStaticsClass , sXAlias , sYAlias );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::MakeStaticsCondition()") );

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ 함수설명 ]
//   실제 결과 DM 구성
//   DM의 형태를 만든다.
// 
// [ 미니스펙 ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::MakeRealDM( CString sStaticsClass , CString sXAlias , CString sYAlias )
{
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. DM초기화
	// ==========================================================================================================
	m_pRealResultDM->FreeData();
	m_pRealResultDM->FreeDMRef();
	
	// ==========================================================================================================
	// 2. 사용될 컬럼 코드값 가져오기
	// ==========================================================================================================
	INT nCanUseXColumnName , nCanUseYColumnName;
	CStringArray saXColumnName;
	CStringArray saYColumnName;

	nCanUseXColumnName = GetColumnNames( sStaticsClass , sXAlias , saXColumnName , 2 );
	if( nCanUseXColumnName < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeRealDM()") );
	
	nCanUseYColumnName = GetColumnNames( sStaticsClass , sYAlias , saYColumnName , 2 );
	if( nCanUseYColumnName < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeRealDM()") );	

	// ==========================================================================================================
	// 3. DM 구성하기
	// ==========================================================================================================
	INT nColumnCnt;
	nColumnCnt = saXColumnName.GetSize()+1;
	m_pRealResultDM->InitDataMgrRef(nColumnCnt);

	ids = m_pRealResultDM->SetDataMgrRef( 0 , _T("컬럼명") , _T(""),_T("") ,_T(""),_T("") );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::MakeRealDM()") );

	// 컬럼명 설정
	for ( INT i = 0;i < saXColumnName.GetSize();i++ )
	{
		ids = m_pRealResultDM->SetDataMgrRef( i + 1, saXColumnName.GetAt(i), _T(""), _T("str"), _T("0"), _T("udf") );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::MakeRealDM()") );
	}

	m_pRealResultDM->m_nCols = nColumnCnt;

	// 예상로우 갯수 만큼 Insert하고 초기값을 지정한다.
	for ( i = 0;i < saYColumnName.GetSize();i++ )
	{
		m_pRealResultDM->InsertRow(-1);
		ids = m_pRealResultDM->SetCellData( _T("컬럼명") ,  saYColumnName.GetAt(i) , m_pRealResultDM->GetRowCount()-1 );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::MakeRealDM()") );

		for ( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			// ????-??-?? UPDATE BY ??? +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// 육사 군사분류기호대 예외처리
			// 보완 작업이 있어야 할지도.......
			CString sNotValue;
			if ( m_sIs64lib == _T("Y") && sXAlias == _T("분류기호별(DDC)") && j == 4 ) 
				sNotValue.Format( _T("0 %s 0"), DIVCHAR );
			else
				sNotValue.Format( _T("0") );
			// --------------------------------------------------------------------------------------------------
			ids = m_pRealResultDM->SetCellData( saXColumnName.GetAt(j-1) , sNotValue , m_pRealResultDM->GetRowCount() - 1 );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::MakeRealDM()") );
		}
	}

	return 0;

EFS_END
	return -1;
}


// 통계 값 가져오기
INT CStaticsInfoMgr::RunStatics( CString sStaticsClass , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;
	CString sXFieldName,sYFieldName;
	CString sXGroupYN , sYGroupYN;

	// 통계를 내기 위한 기본값들 가져오기
	ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
	INT nPos = 1;
		
	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM 테이블 이름 추가
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);

		
		// 기본 검색 조건에 사용자검색조건,그룹조건을 포함시킨다.
		CString sWhere;

		CString sGroupSql , sStaticWhere , sSendWhere;
		if( sXGroupYN.Compare(_T("N"))==0 && (sYGroupYN.Compare(_T("N"))==0) )
		{
			CString sXData , sYData , sTmp;
			// 프로그래스바 검색
			pTotalProgress->SetRange32( 0 , (m_pRealResultDM->RefList.GetCount()+1)*(m_pRealResultDM->GetRowCount()+1) );
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
				for( INT k = 0 ; k < m_pRealResultDM->GetRowCount() ; k++ )
				{
					ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
				
					ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , sXData,sYData ,sXGroupYN,sYGroupYN , i , sWhere );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

					ids = FillStaticsValue( sXFieldName,sYFieldName,sXData,sYData,sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

					pTotalProgress->SetPos(nPos); nPos++;
				}
			}	
		}
		else if( sXGroupYN.Compare(_T("Y"))==0 && (sYGroupYN.Compare(_T("N"))==0) )
		{
			pTotalProgress->SetRange32( 0 , m_pRealResultDM->GetRowCount() );

			CString sYData;
			for( INT k = 0 ; k < m_pRealResultDM->GetRowCount() ; k++ )
			{
				ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

				ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),sYData , sXGroupYN,sYGroupYN ,i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

				ids = FillStaticsValue( sXFieldName,sYFieldName,_T(""),sYData,sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

				pTotalProgress->SetPos( k+1 );
			}

		}
		else if( sXGroupYN.Compare(_T("N"))==0 && (sYGroupYN.Compare(_T("Y"))==0) )
		{
			pTotalProgress->SetRange32( 0 , m_pRealResultDM->GetRowCount() );

			CString sXData , sTmp;
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

				ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , sXData,_T("") , sXGroupYN,sYGroupYN ,i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

				ids = FillStaticsValue( sXFieldName,sYFieldName,sXData,_T(""),sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

				pTotalProgress->SetPos( j+1 );
			}	
		}
		else if( sXGroupYN.Compare(_T("Y"))==0 && (sYGroupYN.Compare(_T("Y"))==0) )
		{
			pTotalProgress->SetRange32( 0 , 1 );

			ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , sXGroupYN,sYGroupYN ,i , sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

			ids = FillStaticsValue( sXFieldName,sYFieldName,_T(""),_T(""),sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
	
			pTotalProgress->SetPos(1);
		}
		
	}

	// 합계 구성
	ids = MakeTotalCount( sStaticsClass , saTableName , sXFieldName,sYFieldName , pProgressCtrl , pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );


	// 코드값을 설명값으로 바꾼다.
	ids = CodeToDesc(sStaticsClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );


	return 0;

	EFS_END
	return -1;

}


// 통계 값 가져오기
INT CStaticsInfoMgr::RunStatics( CString sStaticsClass , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN

	// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if( m_pAxisCode_CellClick != NULL )
		m_pAxisCode_CellClick->Clear();
	// KOL.RED.2018.211 ---------------------------------------------------------------------------

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;
	CStringArray saXFieldName,saYFieldName;
	CString sXGroupYN , sYGroupYN;

	// 통계를 내기 위한 기본값들 가져오기
	ids = GetRunStaticsCond(sStaticsClass,saTableName,sXAlias,sYAlias,saXFieldName,saYFieldName,sXGroupYN,sYGroupYN);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
	INT nPos = 1;
	
	CString sRegNoList;
	CString sRegNo;
	bool IsSend;

	//=====================================================
	//2008.11.24 UPDATE BY PJW : 등록번호갯수 제한을 1000개 이하로 바꾼다.
	// 	INT nLimitCnt = 50;
    INT nLimitCnt = 999;
	//=====================================================
	
	for( INT n = 0 ; n < pRegNoDM->GetRowCount() ; n++ )
	{
		IsSend = FALSE;

		ids = pRegNoDM->GetCellData( _T("등록번호") , n , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

		if( !sRegNoList.IsEmpty() )
			if( !sRegNo.IsEmpty() )
				sRegNoList += _T(",");
		if( !sRegNo.IsEmpty() )
			sRegNoList += _T("'") + sRegNo + _T("'");

		if( (n!=0) && (n%nLimitCnt==0) ) IsSend = TRUE;
		if( n == pRegNoDM->GetRowCount()-1 ) IsSend = TRUE;

		if( !IsSend ) continue;

		if( sRegNoList.IsEmpty() ) continue;

		for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
		{
			// DM 초기하
			ids = InitStaticsDM( sStaticsClass ,saTableName.GetAt(i) , sXAlias , sYAlias , saXFieldName.GetAt(i) , saYFieldName.GetAt(i) , sXGroupYN , sXGroupYN );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

			// DM 테이블 이름 추가
			m_pResultDM->TBL_NAME = saTableName.GetAt(i);

			
			// 기본 검색 조건에 사용자검색조건,그룹조건을 포함시킨다.
			CString sWhere,sSendWhere;
			if( sXGroupYN.Compare(_T("N"))==0 && (sYGroupYN.Compare(_T("N"))==0) )
			{
				CString sXData , sYData , sTmp;
				// 프로그래스바 검색
				pTotalProgress->SetRange32( 0 , (m_pRealResultDM->RefList.GetCount()+1)*(m_pRealResultDM->GetRowCount()+1) );
				for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
				{
					ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
					if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
					for( INT k = 0 ; k < m_pRealResultDM->GetRowCount() ; k++ )
					{
						ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
						if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

						ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , saXFieldName.GetAt(i) , saYFieldName.GetAt(i) , sXData,sYData , sXGroupYN,sYGroupYN ,i , sWhere );
						if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

						sSendWhere = _T(" B.REG_NO IN (")+ sRegNoList +_T(") AND ") + sWhere;

						ids = FillStaticsValue( saXFieldName.GetAt(i),saYFieldName.GetAt(i),sXData,sYData,sXGroupYN,sYGroupYN,sSendWhere,pProgressCtrl);
						if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

						pTotalProgress->SetPos(nPos); nPos++;
					}
				}	
			}
			else if( sXGroupYN.Compare(_T("Y"))==0 && (sYGroupYN.Compare(_T("N"))==0) )
			{
				pTotalProgress->SetRange32( 0 , m_pRealResultDM->GetRowCount() );

				CString sYData;
				for( INT k = 0 ; k < m_pRealResultDM->GetRowCount() ; k++ )
				{
					ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

					ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , saXFieldName.GetAt(i) , saYFieldName.GetAt(i) , _T(""),sYData , sXGroupYN,sYGroupYN ,i , sWhere );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

					sSendWhere = _T(" B.REG_NO IN (")+ sRegNoList +_T(") AND ") + sWhere;

					ids = FillStaticsValue( saXFieldName.GetAt(i),saYFieldName.GetAt(i),_T(""),sYData,sXGroupYN,sYGroupYN,sSendWhere,pProgressCtrl);
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

					pTotalProgress->SetPos( k+1 );
				}

			}
			else if( sXGroupYN.Compare(_T("N"))==0 && (sYGroupYN.Compare(_T("Y"))==0) )
			{
				pTotalProgress->SetRange32( 0 , m_pRealResultDM->GetRowCount() );

				CString sXData , sTmp;
				for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
				{
					ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
					if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

					ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , saXFieldName.GetAt(i) , saYFieldName.GetAt(i) , sXData,_T("") , sXGroupYN,sYGroupYN ,i , sWhere );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

					sSendWhere = _T(" B.REG_NO IN (")+ sRegNoList +_T(") AND ") + sWhere;

					ids = FillStaticsValue( saXFieldName.GetAt(i),saYFieldName.GetAt(i),sXData,_T(""),sXGroupYN,sYGroupYN,sSendWhere,pProgressCtrl);
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

					pTotalProgress->SetPos( j+1 );
				}	
			}
			else if( sXGroupYN.Compare(_T("Y"))==0 && (sYGroupYN.Compare(_T("Y"))==0) )
			{
				pTotalProgress->SetRange32( 0 , 1 );

				ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , saXFieldName.GetAt(i) , saYFieldName.GetAt(i) , _T(""),_T("") ,sXGroupYN,sYGroupYN , i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

				sSendWhere = _T(" B.REG_NO IN (")+ sRegNoList +_T(") AND ") + sWhere;

				ids = FillStaticsValue( saXFieldName.GetAt(i),saYFieldName.GetAt(i),_T(""),_T(""),sXGroupYN,sYGroupYN,sSendWhere,pProgressCtrl);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
		
				pTotalProgress->SetPos(1);
			}
			
		}
		sRegNoList.Empty();
	}

	// 합계 구성
	ids = MakeTotalCount3( sStaticsClass , saTableName , saXFieldName,saYFieldName , pProgressCtrl , pTotalProgress , pRegNoDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

	// 가격 구성
	ids = MakePriceSum3( sStaticsClass , saTableName , saXFieldName,saYFieldName , pProgressCtrl , pTotalProgress , pRegNoDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

	// 코드값을 설명값으로 바꾼다.
	ids = CodeToDesc(sStaticsClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );


	return 0;

	EFS_END
	return -1;

}

// 대출 / 반납 통계 
// 너무 이상혀 ㅡㅡ;;
// 일별로 계를 내서 합산하는 방식으로 중복된 책이나 이용자를 제거하는 방식으로 하고 있다.
// 
// 통계 값 가져오기
INT CStaticsInfoMgr::RunLoanStatics( CString sStaticsClass , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;
	CString sXFieldName,sYFieldName;
	CString sXGroupYN , sYGroupYN;
	CString sTableName;

	// 통계를 내기 위한 기본값들 가져오는 건지 셋팅하는건지
	ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
	INT nPos = 1;

	CString sResultField;
	if( sXAlias.Compare(_T("년")) == 0 || (sXAlias.Compare(_T("월")) == 0) || (sXAlias.Compare(_T("일")) == 0) )
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물") , sResultField );
	else
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물2") , sResultField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );

	// 통계중 일별로 통계를 내야하는것이 존재하는지 검사
	CString sDayField;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("일별") , sDayField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	sDayField.TrimLeft(); sDayField.TrimRight();

	if( !sDayField.IsEmpty() )
	{
		ids = GetCellData( m_pUserMakeSearchCondDM , _T("알리아스") , sDayField , _T("FieldName") , sDayField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunLoanStatics") );
	}
	
	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM 테이블 이름 추가
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);

		sTableName = saTableName.GetAt(i);

		
		// 기본 검색 조건에 사용자검색조건,그룹조건을 포함시킨다.
		CString sWhere;
		
		CString sTableSql;
		CString sSelectFields;
		CString sGroupByFields;


		CString sGroupSql , sStaticWhere , sSendWhere;
		if( sXGroupYN.Compare(_T("N"))==0 && (sYGroupYN.Compare(_T("N"))==0) )
		{
			CString sXData , sYData , sTmp;
			// 프로그래스바 검색
			pTotalProgress->SetRange32( 0 , (m_pRealResultDM->RefList.GetCount()+1)*(m_pRealResultDM->GetRowCount()+1) );
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
				for( INT k = 0 ; k < m_pRealResultDM->GetRowCount() ; k++ )
				{
							
					ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

					ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , sXData,sYData ,sXGroupYN,sYGroupYN , i , sWhere );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );
					
					if( !sDayField.IsEmpty() )
					{
						CString sTmpXFieldName , sTmpYFieldName;
						sSelectFields.Empty();
						sGroupByFields.Empty();
						sTableSql.Empty();
						
						if( sXFieldName.Find(_T(".")) > - 1 )
							sTmpXFieldName = sXFieldName.Mid(sXFieldName.Find(_T("."))+1);
						if( sYFieldName.Find(_T(".")) > - 1 )
							sTmpYFieldName = sYFieldName.Mid(sYFieldName.Find(_T("."))+1);

						sSelectFields.Format( _T("%s AS CNT") , sResultField );
						sGroupByFields.Format( _T("%s") ,  sDayField );
						
						sTableSql.Format( _T("( SELECT %s FROM %s WHERE %s GROUP BY %s ) V") , sSelectFields , sTableName , sWhere , sGroupByFields );

						m_pResultDM->TBL_NAME = sTableSql;
						ids = FillStaticsValue( sTmpXFieldName,sTmpYFieldName,sXData,sYData,sXGroupYN,sYGroupYN,_T("1=1"),pProgressCtrl);
						if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
					}
					else
					{
						ids = FillStaticsValue( sXFieldName,sYFieldName,sXData,sYData,sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
						if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
					}

					pTotalProgress->SetPos(nPos); nPos++;
				}
			}	
		}
		else if( sXGroupYN.Compare(_T("Y"))==0 && (sYGroupYN.Compare(_T("N"))==0) )
		{
			pTotalProgress->SetRange32( 0 , m_pRealResultDM->GetRowCount() );

			CString sYData;
			for( INT k = 0 ; k < m_pRealResultDM->GetRowCount() ; k++ )
			{
				ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

				ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),sYData , sXGroupYN,sYGroupYN ,i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

				if( !sDayField.IsEmpty() )
				{
					CString sTmpXFieldName , sTmpYFieldName;
					sSelectFields.Empty();
					sGroupByFields.Empty();
					sTableSql.Empty();
					
					if( sXFieldName.Find(_T(".")) > - 1 )
						sTmpXFieldName = sXFieldName.Mid(sXFieldName.Find(_T("."))+1);
					
					sSelectFields.Format( _T("%s as XFIELD,%s AS CNT") , sXFieldName , sResultField );
					sGroupByFields.Format( _T("%s,%s") , sXFieldName , sDayField );
					
					sTableSql.Format( _T("( SELECT %s FROM %s WHERE %s GROUP BY %s ) V") , sSelectFields , sTableName , sWhere , sGroupByFields );
					
					m_pResultDM->TBL_NAME = sTableSql;
//					ids = FillStaticsValue( sTmpXFieldName,sTmpYFieldName,_T(""),sYData,sXGroupYN,sYGroupYN,_T("1=1"),pProgressCtrl);
					ids = FillStaticsValue( _T("XFIELD"),sTmpYFieldName,_T(""),sYData,sXGroupYN,sYGroupYN,_T("1=1"),pProgressCtrl);
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
				}
				else
				{
					ids = FillStaticsValue( sXFieldName,sYFieldName,_T(""),sYData,sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
				}
				

				pTotalProgress->SetPos( k+1 );
			}

		}
		else if( sXGroupYN.Compare(_T("N"))==0 && (sYGroupYN.Compare(_T("Y"))==0) )
		{
			pTotalProgress->SetRange32( 0 , m_pRealResultDM->GetRowCount() );

			CString sXData , sTmp;
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

				ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , sXData,_T("") , sXGroupYN,sYGroupYN ,i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

				if( !sDayField.IsEmpty() )
				{
					CString sTmpXFieldName , sTmpYFieldName;
					sSelectFields.Empty();
					sGroupByFields.Empty();
					sTableSql.Empty();
					
					if( sYFieldName.Find(_T(".")) > - 1 )
						sTmpYFieldName = sYFieldName.Mid(sYFieldName.Find(_T("."))+1);
					
					sSelectFields.Format( _T("%s as YFIELD,%s AS CNT") , sYFieldName , sResultField );
					sGroupByFields.Format( _T("%s,%s") , sYFieldName , sDayField );
					
					sTableSql.Format( _T("( SELECT %s FROM %s WHERE %s GROUP BY %s ) V") , sSelectFields , sTableName , sWhere , sGroupByFields );
					
					m_pResultDM->TBL_NAME = sTableSql;
					ids = FillStaticsValue( sTmpXFieldName,_T("YFIELD"),sXData,_T(""),sXGroupYN,sYGroupYN,_T("1=1"),pProgressCtrl);
//					ids = FillStaticsValue( sTmpXFieldName,sTmpYFieldName,sXData,_T(""),sXGroupYN,sYGroupYN,_T("1=1"),pProgressCtrl);
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
				}
				else
				{
					ids = FillStaticsValue( sXFieldName,sYFieldName,sXData,_T(""),sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
				}

				pTotalProgress->SetPos( j+1 );
			}	
		}
		else if( sXGroupYN.Compare(_T("Y"))==0 && (sYGroupYN.Compare(_T("Y"))==0) )
		{
			pTotalProgress->SetRange32( 0 , 1 );

			ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , sXGroupYN,sYGroupYN ,i , sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );

			if( !sDayField.IsEmpty() )
			{
				CString sTmpXFieldName , sTmpYFieldName;
				sSelectFields.Empty();
				sGroupByFields.Empty();
				sTableSql.Empty();
				
				if( sXFieldName.Find(_T(".")) > - 1 )
					sTmpXFieldName = sXFieldName.Mid(sXFieldName.Find(_T("."))+1);
				if( sYFieldName.Find(_T(".")) > - 1 )
					sTmpYFieldName = sYFieldName.Mid(sYFieldName.Find(_T("."))+1);
				
				sSelectFields.Format( _T("%s as XFIELD ,%s as YFIELD,%s AS CNT") , sXFieldName , sYFieldName , sResultField );
				sGroupByFields.Format( _T("%s,%s,%s") , sXFieldName , sYFieldName , sDayField );
				
				sTableSql.Format( _T("( SELECT %s FROM %s WHERE %s GROUP BY %s ) V") , sSelectFields , sTableName , sWhere , sGroupByFields );
				
				m_pResultDM->TBL_NAME = sTableSql;
//				ids = FillStaticsValue( sTmpXFieldName,sTmpYFieldName,_T(""),_T(""),sXGroupYN,sYGroupYN,_T("1=1"),pProgressCtrl);
				ids = FillStaticsValue( _T("XFIELD"),_T("YFIELD"),_T(""),_T(""),sXGroupYN,sYGroupYN,_T("1=1"),pProgressCtrl);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
			}
			else
			{
				ids = FillStaticsValue( sXFieldName,sYFieldName,_T(""),_T(""),sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );
			}
			
			pTotalProgress->SetPos(1);
		}
		
	}

	// 합계 구성
	ids = MakeTotalCount( sStaticsClass , saTableName , sXFieldName,sYFieldName , sXAlias , sXGroupYN , sYGroupYN ,  pProgressCtrl , pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );


	// 코드값을 설명값으로 바꾼다.
	ids = CodeToDesc(sStaticsClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );


	return 0;

	EFS_END
	return -1;

}

// 2009.01.05 ADD BY PWR 
// ** 책두레통계
INT CStaticsInfoMgr::RunCooperativeStatics( CString sStaticsClass , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress )
{
EFS_BEGIN

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;
	CString sXFieldName,sYFieldName;
	CString sXGroupYN , sYGroupYN;

	// 통계를 내기 위한 기본값들 가져오기
	ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunCooperativeStatics") );
	INT nPos = 1;
		
	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM 테이블 이름 추가
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);

		// 기본 검색 조건에 사용자검색조건,그룹조건을 포함시킨다.
		CString sWhere;

		CString sGroupSql , sStaticWhere , sSendWhere;
		if( sXGroupYN.Compare(_T("N"))==0 && (sYGroupYN.Compare(_T("N"))==0) )
		{
			CString sXData , sYData , sTmp;
			// 프로그래스바 검색
			pTotalProgress->SetRange32( 0 , (m_pRealResultDM->RefList.GetCount()+1)*(m_pRealResultDM->GetRowCount()+1) );
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunCooperativeStatics") );
				for( INT k = 0 ; k < m_pRealResultDM->GetRowCount() ; k++ )
				{
					ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunCooperativeStatics") );

					ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , sXData,sYData ,sXGroupYN,sYGroupYN , i , sWhere );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunCooperativeStatics") );

					ids = FillStaticsValue( sXFieldName,sYFieldName,sXData,sYData,sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunCooperativeStatics") );

					pTotalProgress->SetPos(nPos); nPos++;
				}
			}	
		}
		else if( sXGroupYN.Compare(_T("Y"))==0 && (sYGroupYN.Compare(_T("N"))==0) )
		{
			pTotalProgress->SetRange32( 0 , m_pRealResultDM->GetRowCount() );

			CString sYData;
			for( INT k = 0 ; k < m_pRealResultDM->GetRowCount() ; k++ )
			{
				ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunCooperativeStatics") );

				ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),sYData , sXGroupYN,sYGroupYN ,i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunCooperativeStatics") );

				ids = FillStaticsValue( sXFieldName,sYFieldName,_T(""),sYData,sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunCooperativeStatics") );

				pTotalProgress->SetPos( k+1 );
			}

		}
		else if( sXGroupYN.Compare(_T("N"))==0 && (sYGroupYN.Compare(_T("Y"))==0) )
		{
			pTotalProgress->SetRange32( 0 , m_pRealResultDM->GetRowCount() );

			CString sXData , sTmp;
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunCooperativeStatics") );

				ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , sXData,_T("") , sXGroupYN,sYGroupYN ,i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunCooperativeStatics") );

				ids = FillStaticsValue( sXFieldName,sYFieldName,sXData,_T(""),sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunCooperativeStatics") );

				pTotalProgress->SetPos( j+1 );
			}	
		}
		else if( sXGroupYN.Compare(_T("Y"))==0 && (sYGroupYN.Compare(_T("Y"))==0) )
		{
			pTotalProgress->SetRange32( 0 , 1 );

			ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , sXGroupYN,sYGroupYN ,i , sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunCooperativeStatics") );

			ids = FillStaticsValue( sXFieldName,sYFieldName,_T(""),_T(""),sXGroupYN,sYGroupYN,sWhere,pProgressCtrl);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunCooperativeStatics") );
	
			pTotalProgress->SetPos(1);
		}
	}

	// 합계 구성
	ids = MakeTotalCount( sStaticsClass , saTableName , sXFieldName,sYFieldName , pProgressCtrl , pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunCooperativeStatics") );

	// 코드값을 설명값으로 바꾼다.
	ids = CodeToDesc(sStaticsClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunCooperativeStatics") );

	return 0;

EFS_END
return -1;
}


//###############################################################################################################
// [ 함수설명 ]
//   DB에서 값을 가져온후 결과 DM에 기록한다.
//
// [ 미니스펙 ]
//
//###############################################################################################################
INT CStaticsInfoMgr::GetDBAndMakeRealResultDM( CString sSendWhere , CProgressCtrl *pProgress )
{
EFS_BEGIN

	CString strNormal, strArmy;
	INT ids;

	ids = m_pResultDM->RefreshDataManager( sSendWhere , pProgress );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::GetDBAndMakeRealResultDM()") );

	INT nXIndex , nYIndex;
	CString sXAlias , sYAlias , sResult , sPreResult;

	for ( INT i = 0;i < m_pResultDM->GetRowCount();i++ )
	{
		ids = m_pResultDM->GetCellData( _T("X축알리아스") , i , sXAlias );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::GetDBAndMakeRealResultDM()") );

		ids = m_pResultDM->GetCellData( _T("Y축알리아스") , i , sYAlias );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::GetDBAndMakeRealResultDM()") );

		ids = m_pResultDM->GetCellData( _T("결과물") , i , sResult );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::GetDBAndMakeRealResultDM()") );

		// 인덱스 찾기
		ids = GetDMIndex( m_pRealResultDM , sXAlias,sYAlias,nXIndex,nYIndex);
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::GetDBAndMakeRealResultDM()") );

		// 값 입력
		//sResult.Format( _T("%d") , _ttoi(sPreResult) + _ttoi(sResult) );
		// 2005.07.28 ADD BY PDJ
		// 군사분류기호 처리
		// ? ( ? ) 형식에 대한 연산
		if ( m_sIs64lib == _T("Y") && (sXAlias==_T("355-9") || sXAlias==_T("3") )) 
		{		
			CString strNormalResult;

			if ( sXAlias==_T("3") )
			{
				m_pRealResultDM->GetCellData(_T("3"),nYIndex, strNormalResult);
				DividNormalArmy(strNormalResult, strNormal, strArmy);
				sResult.Format( _T("%d %s %d") , _ttoi(sResult), DIVCHAR, _ttoi(strArmy) );
			}
			else
			{
				m_pRealResultDM->GetCellData(_T("3"),nYIndex, strNormalResult);
				DividNormalArmy(strNormalResult, strNormal, strArmy);
				sResult.Format( _T("%d %s %d") , _ttoi(strNormal), DIVCHAR, _ttoi(sResult) );
			}				
			
			ids = m_pRealResultDM->SetCellData( _T("3"), sResult , nYIndex);
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::GetDBAndMakeRealResultDM()") );
		}
		else
		{	
			sPreResult = m_pRealResultDM->GetCellData(sXAlias,nYIndex);				
			sResult.Format( _T("%d") , _ttoi(sPreResult) + _ttoi(sResult) );
			ids = m_pRealResultDM->SetCellData( sXAlias , sResult , nYIndex);
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CStaticsInfoMgr::GetDBAndMakeRealResultDM()") );		
		}
	}

	return 0;

EFS_END
	return -1;
}


INT CStaticsInfoMgr::GetDMIndex( CESL_DataMgr *pDM , CString sXAlias , CString sYAlias , INT &nXIndex , INT &nYIndex )
{
	EFS_BEGIN

	INT ids;

	nXIndex = -1;
	nYIndex = -1;

	CString sTmpYAlias;

	// X축 검사
	CString sGroupYN;
	INT refcount = pDM->RefList.GetCount();
	CESL_DataMgr::reference * pRef;
	POSITION pos;
	pos = pDM->RefList.GetHeadPosition();
	CString query, fields, isudf;
	
	// 2005.07.27 ADD BY PDJ
	// 육사일 경우 _T("355-9") 로 나오는 항목을 분류_T("3")으로 리턴 _T("일반(군사)") 형태임
	if ( m_sIs64lib == _T("Y") && sXAlias==_T("355-9")) 
	{
		sXAlias = _T("3");				
	}

	for( INT i = 0 ; i < pDM->RefList.GetCount() ; i++ )
	{
		pRef = NULL;
		pRef = (CESL_DataMgr::reference*)pDM->RefList.GetNext(pos);		

		if( pRef->FIELD_ALIAS.Compare(sXAlias) == 0 )
		{
			nXIndex = i;
			break;
		}
	}


	if( nXIndex == -1 )
	{
		pRef = new CESL_DataMgr::reference;
		pRef->DATA_TYPE = _T(""); pRef->FIELD_NAME = _T(""); pRef->INIT_VALUE = _T("");
		pRef->FIELD_ALIAS = sXAlias;

		pDM->RefList.AddTail((CObject*)pRef);

		INT nColCnt = pDM->RefList.GetCount();

		pDM->m_nCols = nColCnt;

		ids = PostInsertDMColumn( pDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetDMIndex") );

		nXIndex = pDM->RefList.GetCount() - 1;
	}

	// Y축 검사
	for( i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( i , 0 , sTmpYAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetDMIndex") );

		if( sTmpYAlias.Compare( sYAlias ) == 0 )
			nYIndex = i;
	}

	if( nYIndex == -1 )
	{
		pDM->InsertRow(-1);
		pDM->SetCellData( pDM->GetRowCount()-1 , 0 , sYAlias );

		nYIndex = pDM->GetRowCount() - 1;
	}

	return 0;

	EFS_END
	return -1;

}


// 통계에 사용될 기본 정보 참조
// 1. TableName
// 2. X축 Y축 알리아스
// 3. X축 필드 , Y축 필드
// 4. Group 여부
INT CStaticsInfoMgr::GetRunStaticsCond( CString sStaticsClass , CStringArray &saTableName , CString &sXAlias , CString &sYAlias ,CStringArray &saXFieldName,CStringArray &saYFieldName,CString &sXGroupYN,CString &sYGroupYN)
{
	EFS_BEGIN
		
	INT ids;
	
	// 1. TABLE NAME 구성
	CString sTableName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("검색테이블이름") , sTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitRunStatics") );
	
	ids = CLocCommonAPI::MakeArrayList( sTableName , _T("/") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitRunStatics") );
	
	
	// DM 컬럼명(필드명) 추가
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("X축선택알리아스") , sXAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("Y축선택알리아스") , sYAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	
	CString sResultField;
	CString sXFieldName , sYFieldName;
	ids = GetFieldName( sStaticsClass , sXAlias , sXFieldName , sXGroupYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	ids = GetFieldName( sStaticsClass , sYAlias , sYFieldName , sYGroupYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );

	ids = CLocCommonAPI::MakeArrayList( sXFieldName , _T("/") , saXFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitRunStatics") );
	ids = CLocCommonAPI::MakeArrayList( sYFieldName , _T("/") , saYFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitRunStatics") );
	
	

	// 합계 DM 초기화
	m_pXSumDM->FreeData();
	m_pYSumDM->FreeData();
	
	return 0;
	
	EFS_END
		return -1;
	
}

// 통계에 사용될 DM 초기화
INT CStaticsInfoMgr::InitStaticsDM( CString sStaticsClass , CString sTableName , CString sXAlias , CString sYAlias ,CString sXFieldName,CString sYFieldName,CString sXGroupYN,CString sYGroupYN) 
{
	EFS_BEGIN

	INT ids;

	CString sResultField;
	
	if( sXAlias.Compare(_T("년")) == 0 || (sXAlias.Compare(_T("월")) == 0) || (sXAlias.Compare(_T("일")) == 0) )
	{
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물") , sResultField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	}
	else
	{
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물2") , sResultField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );		
	}

	// 필드명 설정
	INT refcount = m_pResultDM->RefList.GetCount();
	CESL_DataMgr::reference * pRef;
	POSITION pos;
	pos = m_pResultDM->RefList.GetHeadPosition();
	CString query, fields, isudf;
	for( INT i = 0 ; i < m_pResultDM->RefList.GetCount() ; i++ )
	{
		pRef = NULL;
		pRef = (CESL_DataMgr::reference*)m_pResultDM->RefList.GetNext(pos);
		
		if( pRef->FIELD_ALIAS.Compare(_T("X축알리아스")) == 0 )
		{
			pRef->FIELD_NAME = sXFieldName;
			pRef->FIELD_TYPE = _T("");
		}
		else if( pRef->FIELD_ALIAS.Compare(_T("Y축알리아스")) == 0 )
		{
			pRef->FIELD_NAME = sYFieldName;
			pRef->FIELD_TYPE = _T("");
		}
		else if( pRef->FIELD_ALIAS.Compare(_T("결과물")) == 0 )
		{
			pRef->FIELD_NAME = sResultField;
		}
		
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitRunStatics") );
		
	}

	return 0;

	EFS_END

	return -1;
}

// 통계에 사용될 DM 초기화
INT CStaticsInfoMgr::InitRunStatics( CString sStaticsClass , CStringArray &saTableName , CString &sXAlias , CString &sYAlias ,CString &sXFieldName,CString &sYFieldName,CString &sXGroupYN,CString &sYGroupYN
									// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
									, bool	noFreeXYSum
									// KOL.RED.2018.211 ---------------------------------------------------------------------------
									)
{
	EFS_BEGIN

	INT ids;

	// 1. TABLE NAME 구성
	CString sTableName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("검색테이블이름") , sTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitRunStatics") );

	ids = CLocCommonAPI::MakeArrayList( sTableName , _T("/") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitRunStatics") );


	// DM 컬럼명(필드명) 추가
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("X축선택알리아스") , sXAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("Y축선택알리아스") , sYAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );

	CString sResultField;
	ids = GetFieldName( sStaticsClass , sXAlias , sXFieldName , sXGroupYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	ids = GetFieldName( sStaticsClass , sYAlias , sYFieldName , sYGroupYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );

	if( sXAlias.Compare(_T("년")) == 0 || (sXAlias.Compare(_T("월")) == 0) || (sXAlias.Compare(_T("일")) == 0) )
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물") , sResultField );
	else
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물2") , sResultField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );

	if( sResultField.IsEmpty() )
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물") , sResultField );

	CString sDayField;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("일별") , sDayField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	sDayField.TrimLeft(); sDayField.TrimRight();

	if( !sDayField.IsEmpty() )
	{
		sResultField.Format( _T("SUM(CNT)") , sDayField );
	}
	
	
	// 필드명 설정
	INT refcount = m_pResultDM->RefList.GetCount();
	CESL_DataMgr::reference * pRef;
	POSITION pos;
	pos = m_pResultDM->RefList.GetHeadPosition();
	CString query, fields, isudf;

	CString sTmpTableName = saTableName.GetAt(0);
	CStringArray saTmpTableName;
	CLocCommonAPI::MakeArrayList( sTmpTableName , _T(","), saTmpTableName );
	sTmpTableName = saTmpTableName.GetAt(0);
	CLocCommonAPI::MakeArrayList( sTmpTableName , _T(" "), saTmpTableName ); 
	sTmpTableName = saTmpTableName.GetAt(1);

	for( INT i = 0 ; i < m_pResultDM->RefList.GetCount() ; i++ )
	{
		pRef = NULL;
		pRef = (CESL_DataMgr::reference*)m_pResultDM->RefList.GetNext(pos);

		if( pRef->FIELD_ALIAS.Compare(_T("X축알리아스")) == 0 )
		{
			if( sXFieldName.Find(_T(".")) < 0 )
			{
				sXFieldName = sTmpTableName + _T(".") + sXFieldName;
			}

			pRef->FIELD_NAME = sXFieldName;
			pRef->FIELD_TYPE = _T("");
		}
		else if( pRef->FIELD_ALIAS.Compare(_T("Y축알리아스")) == 0 )
		{
			pRef->FIELD_NAME = sYFieldName;
			pRef->FIELD_TYPE = _T("");

			if( sYFieldName.Find(_T(".")) < 0 )
			{
				sYFieldName = sTmpTableName + _T(".") + sYFieldName;
			}
		}
		else if( pRef->FIELD_ALIAS.Compare(_T("결과물")) == 0 )
		{
			pRef->FIELD_NAME = sResultField;
		}

		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitRunStatics") );

	}

	//csj
	if( noFreeXYSum == false )
	{
	// 합계 DM 초기화
	m_pXSumDM->FreeData();
	m_pYSumDM->FreeData();
	}

	return 0;

	EFS_END
	return -1;

}


INT CStaticsInfoMgr::CodeToDesc( CString sStaticsClass )
{
	EFS_BEGIN

	INT ids;
	CString sXAlias , sYAlias , sXCode , sYCode;

	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("X축선택알리아스") , sXAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("Y축선택알리아스") , sYAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunStatics") );

	ids = GetCellData( m_pColumnAliasInfoDM , _T("알리아스명") , sXAlias , _T("설명") , sXCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );
	ids = GetCellData( m_pColumnAliasInfoDM , _T("알리아스명") , sYAlias , _T("설명") , sYCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("RunStatics") );

	CString sCode;
	if( !sXCode.IsEmpty() )
	{
		CString sFieldName , sFieldType , sInitValue , sDataType;
		for( INT i = 1 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
		{
			ids = m_pRealResultDM->GetColumnRefInfo( i , sCode , sFieldName , sFieldType , sInitValue , sDataType );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("RunStatics") );

			// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
			// 셀클릭용 데이터 저장
			CString		sCode_Temp = sCode;
			// KOL.RED.2018.211 ---------------------------------------------------------------------------

			// 코드가 회원 구분이라면
			if( sXAlias.Compare(_T("회원구분별"))==0 )
				ids = ConvertInferiorCodeToDesc( sCode , sCode );
			// 2006-03-03 UPDATE BY PDW(Jr) ===================================================
			// 국방대 사회과학 통계 추가
			else if( sXAlias.Compare(_T("분류기호별"))==0 || 
				(sXAlias.Compare(_T("KDC별"))==0) || 
				(sXAlias.Compare(_T("DDC별"))==0) || 
				(sXAlias.Compare(_T("LC별"))==0)  ||
				(sXAlias.Compare(_T("분류기호별(DDC)"))==0) ||
				(sXAlias.Compare(_T("분류기호별(사회과학)"))==0)
				)
				ids = ConvertClassNo(sStaticsClass,sCode,sCode,sXAlias);
			// ================================================================================
			else if( sXAlias.Compare(_T("배가상태별")) == 0 )
			{
				ids = CLocCommonAPI::GetWorkingStatusDescToCode( sCode , 1 );
			}
			else
				ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( sXCode , sCode , sCode );
			/*
			if( ids < 0 )
			{
				CString sEMsg;
				sEMsg.Format( _T(" sXAlias : %s , sXCode : %s , sCode : %s ") , sXAlias , sXCode , sCode );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("RunStatics") );
			*/

			ids = m_pRealResultDM->SetDataMgrRef( i , sCode , sFieldName , sFieldType , sInitValue , sDataType );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("RunStatics") );

			// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
			// 셀클릭용 데이터 저장
			if( m_pAxisCode_CellClick != NULL )
			{
				m_pAxisCode_CellClick->SetCodeToDesc( 0 , i-1 , sXAlias , sCode_Temp , sCode );
			}
			// KOL.RED.2018.211 ---------------------------------------------------------------------------
		}
	}
	if( !sYCode.IsEmpty() )
	{
		for( INT i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
		{
			ids = m_pRealResultDM->GetCellData( i , 0 , sCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("RunStatics") );

			// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
			// 셀클릭용 데이터 저장
			CString		sCode_Temp = sCode;
			// KOL.RED.2018.211 ---------------------------------------------------------------------------

			CString sTmpCode;
			sTmpCode = sCode;

			if( sYAlias.Compare(_T("회원구분별"))==0 )
				ids = ConvertInferiorCodeToDesc( sCode , sCode );
			// 2006-03-03 UPDATE BY PDW(Jr) ===================================================
			// 국방대 사회과학 통계 추가
			else if( sYAlias.Compare(_T("분류기호별"))==0 ||
				(sYAlias.Compare(_T("KDC별"))==0) || 
				(sYAlias.Compare(_T("DDC별"))==0) || 
				(sYAlias.Compare(_T("LC별"))==0)  ||
				(sYAlias.Compare(_T("분류기호별(DDC)"))==0) ||
				(sYAlias.Compare(_T("분류기호별(DDC)"))==0) )
			{
				ids = ConvertClassNo(sStaticsClass,sCode,sCode,sYAlias);
			}
			// ================================================================================
			else if( sYAlias.Compare(_T("배가상태별")) == 0 )
			{
				ids = CLocCommonAPI::GetWorkingStatusDescToCode( sCode , 1 );
			}
			else if(sYAlias.Compare(_T("진행상태별")) == 0 )
			{
				if(_T("0001") == sCode)
				{
					sCode = _T("신청된자료");
				}
				else if(_T("0002") == sCode)
				{
					sCode = _T("요청된자료");
				}
				else if(_T("0004") == sCode)
				{
					sCode = _T("발송된자료");
				}
				else if(_T("0014") == sCode)
				{
					sCode = _T("입수된자료");
				}
				else if(_T("0008") == sCode)
				{
					sCode = _T("복귀중자료");
				}
				else if(_T("0009") == sCode)
				{
					sCode = _T("완료된자료");
				}
				else if(_T("0011") == sCode)
				{
					sCode = _T("발송거절된자료");
				}
				else if(_T("0012") == sCode)
				{
					sCode = _T("요청거절된자료");
				}
				else if(_T("0016") == sCode)
				{
					sCode = _T("신청취소된자료(이용자)");
				}
			}
			else if(sYAlias.Compare(_T("취소유형별")) == 0 )
			{
				if(_T("0001") == sCode)
				{
					sCode = _T("이용자취소");
				}
				else if(_T("0002") == sCode)
				{
					sCode = _T("요청거절");
				}
				else if(_T("0003") == sCode)
				{
					sCode = _T("무응답/미대출");
				}
				else if(_T("0004") == sCode)
				{
					sCode = _T("발송거절(도서대출불가)");
				}
				else if(_T("0010") == sCode)
				{
					sCode = _T("완료");
				}
				else if(_T("0099") == sCode)
				{
					sCode = _T("기타");
				}
			}
			// 2011.10.04 ADD BY PWR : 통합회원여부 추가
			else if(_T("통합회원구분별") == sYAlias)
			{
				if(_T("Y") == sCode)
				{
					sCode = _T("통합회원");
				}
				else if(_T("N") == sCode)
				{
					sCode = _T("일반회원");
				}
				else
				{
					sCode = _T("알수없음");
				}
			}
			else
			{
				// KOL.RED.2018.210 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				//ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( sYCode , sCode , sCode );

				CString strCodeBuf = sCode;
				ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( sYCode , sCode , sCode );

				CString strDispBuf;
				strDispBuf.Format(_T("%s(%s)"), sCode, strCodeBuf);

				sCode = strDispBuf;
				// KOL.RED.2018.210 ---------------------------------------------------------------------------
			}
			if( ids < 0 )
				sCode = sTmpCode;

			/*
			if( ids < 0 )
			{
				CString sEMsg;
				sEMsg.Format( _T(" sYAlias : %s , sYCode : %s , sCode : %s ") , sYAlias , sYCode , sCode );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("RunStatics") );
			*/

			ids = m_pRealResultDM->SetCellData( i , 0 , sCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("RunStatics") );

			// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
			// 셀클릭용 데이터 저장
			if( m_pAxisCode_CellClick != NULL )
			{
				m_pAxisCode_CellClick->SetCodeToDesc( 1 , i , sYAlias , sCode_Temp , sCode );
			}
			// KOL.RED.2018.211 ---------------------------------------------------------------------------

		}
	}
	// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	else
	{
		// 코드 변환할것이 없으면 코드값 그대로 넣는다.
		for( INT i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
		{
			ids = m_pRealResultDM->GetCellData( i , 0 , sCode );
			if( m_pAxisCode_CellClick != NULL )
			{
				m_pAxisCode_CellClick->SetCodeToDesc( 1 , i , sYAlias , sCode , sCode );
			}
		}
	}
	// KOL.RED.2018.211 ---------------------------------------------------------------------------

	return 0;

	EFS_END
	return -1;

}

// 회원구분별 코드 DESC로 변경
INT CStaticsInfoMgr::ConvertInferiorCodeToDesc( CString sCode , CString &sDesc )
{
	EFS_BEGIN

	if( sCode.Compare(_T("0"))==0 ) sDesc = _T("정회원");
	if( sCode.Compare(_T("1"))==0 ) sDesc = _T("비회원");
	// 2007.04.03 add by pdw {{++++++++++++++++++++++++++++++++++++++++++++
	if( sCode.Compare(_T("2"))==0 ) sDesc = _T("준회원");
	// ------------------------------------------------------------------}}

	return 0;

	EFS_END
	return -1;

}

// ##############################################################################################################
// [ 함수설명 ]
//   결과를 그리드에 뿌려주기
//
// [ 미니스펙 ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::StaticsResultViewGrid( CString sStaticsClass , CMSHFlexGrid *pGrid , bool bDupDel , bool bZeroShow , CString sMenuName)
{
EFS_BEGIN

	INT ids;
	
	if ( m_pRealResultDM->GetRowCount() < 1 || !pGrid->GetSafeHwnd() ) return 0;

	CString sSumAlias;
	if ( !bDupDel )		sSumAlias = _T("중복허용_합계");
	else        		sSumAlias = _T("중복삭제_합계");

	// ==========================================================================================================
	// 1. 그리드 설정
	// ==========================================================================================================
	pGrid->SetRedraw(FALSE);
	pGrid->Clear();

	pGrid->SetRows( m_pRealResultDM->GetRowCount()+2 );
	pGrid->SetCols( 0 , m_pRealResultDM->RefList.GetCount()+1 );

	// ==========================================================================================================
	// 2. X축 컬럼 뿌려주기
	// ==========================================================================================================
	INT refcount = m_pRealResultDM->RefList.GetCount();
	CESL_DataMgr::reference * pRef;
	POSITION pos;
	pos = m_pRealResultDM->RefList.GetHeadPosition();
	CString query, fields, isudf;
	CString sResult;

	//2012.10.26 ADD BY BJS : 2012년도 도서관 통합서비스 확대구축 사업 2차 (성비 표현)
	//x축 알리아스를 가져온다
	CString sXAlias;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("X축선택알리아스") , sXAlias );
	////////////////////////////////////////////////////////////////
	
	for( INT i = 0 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		pRef = NULL;
		pRef = (CESL_DataMgr::reference*)m_pRealResultDM->RefList.GetNext(pos);

		pGrid->SetTextMatrix( 0 , i ,  pRef->FIELD_ALIAS );

		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( pRef->FIELD_ALIAS , j , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::StaticsResultViewGrid()") );

			//2012.10.26 ADD BY BJS : 2012년도 도서관 통합서비스 확대구축 사업 2차 (성비 표현)
			//대출자 및 대출자 등록통계에서 통계 시 비율을 표현
			if(sStaticsClass == _T("대출자 통계") || sStaticsClass == _T("대출자 등록통계") )
			{	
				if((sXAlias.Compare(_T("성별"))==0 && i > 0) || (sXAlias.Compare(_T("성별2"))==0 && i > 0))
				{
					CString sSumAll,sPercent;
					double dSumAll,dResult,dSum;
					ids = m_pXSumDM->GetCellData( sSumAlias , j , sSumAll );
					dSumAll=_ttoi(sSumAll); dResult=_ttoi(sResult);
					dSum=(dResult/dSumAll)*100;

					if(dSum<0) dSum=0;

					sPercent.Format(_T("%.1f"), dSum);			
					sResult = sResult + _T(" (") + sPercent + _T("%)");						
				}			
			}
			///////////////////////////////////////////////////////////////////////////////////
			pGrid->SetTextMatrix( j+1 , i , sResult );
		}
	}

	// ==========================================================================================================
	// 3. 세로 합계 뿌려주기
	// ==========================================================================================================
	pGrid->SetTextMatrix( m_pRealResultDM->GetRowCount()+1 , 0 , _T("합계") );	
	for( i = 0 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		for( INT j = 0 ; j < m_pYSumDM->GetRowCount() ; j++ )
		{
			ids = m_pYSumDM->GetCellData( sSumAlias , j , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 ,  _T("CStaticsInfoMgr::StaticsResultViewGrid()") );

			// 2012.10.26 ADD BY BJS : 2012년도 도서관 통합서비스 확대구축 사업 2차 (성비 표현)
			//대출자 및 대출자 등록통계에서 통계 시 비율을 표현	
			ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("X축선택알리아스") , sXAlias );
			if(sStaticsClass == _T("대출자 통계") || sStaticsClass == _T("대출자 등록통계") )
			{
				if(sXAlias.Compare(_T("성별"))==0 || sXAlias.Compare(_T("성별2"))==0)
				{
					CString sSumAll,sPercent;
					double dSumAll,dResult,dSum;
					ids = m_pXSumDM->GetCellData( sSumAlias ,m_pXSumDM->GetRowCount()-1 , sSumAll );
					dSumAll=_ttoi(sSumAll); dResult=_ttoi(sResult);
					dSum=(dResult/dSumAll)*100;

					if(dSum<0) dSum=0;

					sPercent.Format(_T("%.1f"), dSum);				
					sResult = sResult + _T(" (") + sPercent + _T("%)");	
				}
			}
			//////////////////////////////////////////////////////////////////
			pGrid->SetTextMatrix( m_pRealResultDM->GetRowCount()+1 , j+1 , sResult );
		}		
	}

	// ==========================================================================================================
	// 4. 가로 합계 뿌려주기
	// ==========================================================================================================
	INT nSumCol = m_pRealResultDM->RefList.GetCount();
	pGrid->SetTextMatrix( 0 , nSumCol , _T("합계") );

	CString sSum;
	for( i = 0 ; i < m_pXSumDM->GetRowCount() ; i++ )
	{
		ids = m_pXSumDM->GetCellData( sSumAlias , i , sSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 ,  _T("CStaticsInfoMgr::StaticsResultViewGrid()") );

		// 2012.10.26 ADD BY BJS : 2012년도 도서관 통합서비스 확대구축 사업 2차 (성비 표현)
		//대출자 및 대출자 등록통계에서 통계 시 비율을 표현
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("X축선택알리아스") , sXAlias );
		if(sStaticsClass == _T("대출자 통계") || sStaticsClass == _T("대출자 등록통계") )
		{
			if(sXAlias.Compare(_T("성별"))==0 || sXAlias.Compare(_T("성별2"))==0 )
			{
				sSum=sSum + _T(" (100.0%)");
			}
		}
		///////////////////////////////////////////////////
		pGrid->SetTextMatrix( i+1 , nSumCol , sSum );
	}

	pGrid->SetTextMatrix( 0 , 0 , _T("") );

	// ==========================================================================================================
	// 5. 합계 0 보여주기 
	//    데이터 가공없이 그대로 보여준다.
	// ========================================================================================================== 
	if( bZeroShow )
	{
		SetGridCellAlignment(pGrid);
		pGrid->SetRedraw(TRUE);
		pGrid->Refresh();
		return 0;
	}

	// ========================================================================================================== 
	// 6. 합계가 0인 자료 보여주기 말기
	// ========================================================================================================== 
	// 가로 먼저
	INT nGridRow = pGrid->GetRows();
	INT nGridCol = pGrid->GetCols(0);

	CString sSpeciesSum , sVolSum, sBookSum;
	for( i = nGridRow-2 ; i > 0 ; i-- )
	{
		m_pXSumDM->GetCellData( sSumAlias , i-1 , sSum );

		// 2005.07.28 ADD BY PDJ ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 육사일 경우 군사분류기호의 합계 0을 체크
		if ( m_sIs64lib == _T("Y") && sSum.FindOneOf( DIVCHAR ) > -1 )
		{			
			// _T("(") 기호가 있다는것은 숫자가 있어서 연산을 한것이므로
			// count가 0인지만 체크하므로 직접 셀필요는 없다.
			sSum = _T("1");
		}
		// ------------------------------------------------------------------------------------------------------

		// ======================================================================================================
		// 7. 통계별 셀의 데이터 형식이 다르다.
		// ======================================================================================================
		if ( sMenuName.Compare( LOAN_STATICS ) == 0 || sMenuName.Compare( REPAIR_BOOK_STATICS ) == 0 || sMenuName.Compare( COOPERATIVE_STATICS ) == 0 )
		{
			if ( _ttoi(sSum) == 0 )
				GridDeleteRow( i , pGrid );
		}
		else if ( sMenuName.Compare( WORKING_STATICS ) == 0 || sMenuName.Compare( HOLDING_STATICS ) == 0 )
		{
			MakeSpeciesBookCnt( sSum , sSpeciesSum , sBookSum );

			if ( _ttoi(sSpeciesSum) == 0 && _ttoi(sBookSum) == 0 )
				GridDeleteRow( i , pGrid );
		}
		// 2006-03-30 ADD BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		else if ( sMenuName.Compare( SE_HOLDING_STATICS ) == 0 )
		{
			if(sStaticsClass == _T("자료실별이용통계") || sStaticsClass == _T("신규구입도서이용통계"))
			{
				MakeSpeciesBookCnt( sSum , sSpeciesSum , sBookSum );
				if ( _ttoi(sSpeciesSum) == 0 && _ttoi(sBookSum) == 0 )
					GridDeleteRow( i , pGrid );
			}else
			{
				MakeSpeciesVolBookCnt( sSum , sSpeciesSum , sVolSum, sBookSum );
				if ( _ttoi(sSpeciesSum) == 0 && _ttoi(sVolSum) == 0 && _ttoi(sBookSum) == 0 )
					GridDeleteRow( i , pGrid );
				
			}
			/*MakeSpeciesVolBookCnt( sSum , sSpeciesSum , sVolSum, sBookSum );
			if ( _ttoi(sSpeciesSum) == 0 && _ttoi(sVolSum) == 0 && _ttoi(sBookSum) == 0 )
				GridDeleteRow( i , pGrid );
				*/
		}
		// -----------------------------------------------------------------------------------------------------
	}

	SetGridCellAlignment(pGrid);

	pGrid->SetRedraw(TRUE);
	pGrid->Refresh();

	return 0;

EFS_END
	return -1;
}


// 결과를 그리드에 뿌려주기
INT CStaticsInfoMgr::StaticsResultViewGrid3( CString sStaticsClass , CMSHFlexGrid *pGrid , bool bDupDel , bool bZeroShow , CString sMenuName)
{
	EFS_BEGIN

	INT ids;
	
	if( m_pRealResultDM->GetRowCount() < 1 ) return 0;

	CString sSumAlias;
	if( !bDupDel )
		sSumAlias = _T("중복허용_합계");
	else
		sSumAlias = _T("중복삭제_합계");

	if( pGrid->GetSafeHwnd() == NULL ) return 0;

	pGrid->SetRedraw(FALSE);

	// 그리드에 뿌려준다.
	pGrid->Clear();

	pGrid->SetRows( m_pRealResultDM->GetRowCount()+3 );
	pGrid->SetCols( 0 , m_pRealResultDM->RefList.GetCount()+2 );

	// X축 컬럼 뿌려주기
	INT refcount = m_pRealResultDM->RefList.GetCount();
	CESL_DataMgr::reference * pRef;
	POSITION pos;
	pos = m_pRealResultDM->RefList.GetHeadPosition();
	CString query, fields, isudf;

	CString sResult;
	for( INT i = 0 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		pRef = NULL;
		pRef = (CESL_DataMgr::reference*)m_pRealResultDM->RefList.GetNext(pos);

		pGrid->SetTextMatrix( 0 , i ,  pRef->FIELD_ALIAS );

		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( pRef->FIELD_ALIAS , j , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

			pGrid->SetTextMatrix( j+1 , i , sResult );
		}

	}

	// 세로 합계 뿌려주기
	pGrid->SetTextMatrix( m_pRealResultDM->GetRowCount()+1 , 0 , _T("합계") );	
	for( i = 0 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{		
		
		for( INT j = 0 ; j < m_pYSumDM->GetRowCount() ; j++ )
		{
			ids = m_pYSumDM->GetCellData( sSumAlias , j , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

			pGrid->SetTextMatrix( m_pRealResultDM->GetRowCount()+1 , j+1 , sResult );
		}		
	}


	// 가로 합계 뿌려주기
	INT nSumCol = m_pRealResultDM->RefList.GetCount();
	pGrid->SetTextMatrix( 0 , nSumCol , _T("합계") );

	CString sSum;
	for( i = 0 ; i < m_pXSumDM->GetRowCount() ; i++ )
	{
		ids = m_pXSumDM->GetCellData( sSumAlias , i , sSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

		pGrid->SetTextMatrix( i+1 , nSumCol , sSum );
	}

	// 가격 뿌려주기
	// 세로 가격 뿌려주기
	pGrid->SetTextMatrix( m_pRealResultDM->GetRowCount()+2 , 0 , _T("가격") );	
	for( i = 0 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		for( INT j = 0 ; j < m_pYPriceSumDM->GetRowCount() ; j++ )
		{
			ids = m_pYPriceSumDM->GetCellData( sSumAlias , j , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

			//pGrid->SetTextMatrix( m_pRealResultDM->GetRowCount()+2 , j+1 , sResult );
			pGrid->SetTextMatrix( m_pRealResultDM->GetRowCount()+2 , j+1 , ConvertPrice( sResult ) );
		}
	}

	// 가로 가격 뿌려주기
	pGrid->SetTextMatrix( 0 , nSumCol+1 , _T("가격") );

	for( i = 0 ; i < m_pXPriceSumDM->GetRowCount() ; i++ )
	{
		ids = m_pXPriceSumDM->GetCellData( sSumAlias , i , sSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

		if( i == m_pXPriceSumDM->GetRowCount()-1 )
			pGrid->SetTextMatrix( i+2 , nSumCol+1 , ConvertPrice( sSum ) );
			//pGrid->SetTextMatrix( i+2 , nSumCol+1 , sSum );
		else
			pGrid->SetTextMatrix( i+1 , nSumCol+1 , ConvertPrice( sSum ) );
			//pGrid->SetTextMatrix( i+1 , nSumCol+1 , sSum );
	}


	pGrid->SetTextMatrix( 0 , 0 , _T("") );

	if( bZeroShow )
	{
		SetGridCellAlignment(pGrid);
		pGrid->SetRedraw(TRUE);
		pGrid->Refresh();

		return 0;
	}


	// 합계가 0인 자료 보여주기 말기
	// 가로 먼저
	INT nGridRow = pGrid->GetRows();
	INT nGridCol = pGrid->GetCols(0);

	CString sSpeciesSum , sBookSum;
	for( i = nGridRow-3 ; i > 0 ; i-- )
	{
		m_pXSumDM->GetCellData( sSumAlias , i-1 , sSum );

		if( _ttoi(sSum) == 0 )
			GridDeleteRow( i , pGrid );
	}

	/* 열은 안보여주기
	for( i = nGridCol-3 ; i > 0 ; i-- )
	{
		m_pYSumDM->GetCellData( sSumAlias , i-1 , sSum );
		if( _ttoi(sSum) == 0 )
			GridDeleteCol( i , pGrid );

	}
	*/
	SetGridCellAlignment(pGrid);

	pGrid->SetRedraw(TRUE);
	pGrid->Refresh();

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::IsEqualGridColumn( CMSHFlexGrid *pGrid , CString sXAlias , CString sYAlias , INT &nXIndex , INT &nYIndex , INT nIndex )
{
	EFS_BEGIN

	CString sTmpAlias;
	nXIndex = -1;
	nYIndex = -1;
	// X축 가져오기
	for( INT i = 1 ; i < pGrid->GetCols(0) ; i++ )
	{

		if( nIndex == 0 )
		{
			pGrid->SetTextMatrix( 0 , i , sXAlias );
		}

		sTmpAlias = pGrid->GetTextMatrix( 0 , i );

		if( sTmpAlias.Compare( sXAlias ) == 0 ) 
			nXIndex = i;
	}


	// Y축 가져오기
	for( i = 1 ; i < pGrid->GetRows() ; i++ )
	{

		if( nIndex == 0 )
		{
			pGrid->SetTextMatrix( i , 0 , sYAlias );
		}

		sTmpAlias = pGrid->GetTextMatrix( i , 0 );

		if( sTmpAlias.Compare( sYAlias ) == 0 ) 
			nYIndex = i;

		
	}


	return 0;

	EFS_END
	return -1;

}

// 필드명 얻어오기
INT CStaticsInfoMgr::GetFieldName( CString sStaticsClass , CString sAlias , CString &sFieldName , CString &sGroupYN )
{
	EFS_BEGIN

	CString sFunctionName;
	INT ids;

	ids = GetCellData( m_pColumnAliasInfoDM , _T("알리아스명") , sAlias , _T("검색필드명") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetFieldName") );

	// 2005.07.21 ADD BY PDJ
	// SELECT 절에 FUNCTION을 이용할 경우에 대한 처리
	if ( ids > 0 )
	{		
		INT		nStartPos, nEndPos;
		
		nStartPos = sAlias.Find(_T("("), 0 );	
		nEndPos = sAlias.Find(_T(")"), 0 );	
		sFunctionName = sAlias;
		sAlias = sAlias.Mid(nStartPos+1, nEndPos-nStartPos-1);
		
		ids = GetCellData( m_pColumnAliasInfoDM , _T("알리아스명") ,sAlias , _T("검색필드명") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetFieldName") );

		// 이름부분만 Field명으로 교체 ( 펑션유지 )
		sFunctionName.Replace(sAlias, sFieldName);
		sFieldName = sFunctionName;		
	}
	
	ids = GetCellData( m_pColumnAliasInfoDM , _T("알리아스명") , sAlias , _T("그룹여부") , sGroupYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetFieldName") );
	
	// 순위 너무 어렵당 ㅡㅡ;;
	// 다음 기회에 하장..  지금은 ㅡㅡ;;
	CString sSpecialFieldAlias , sSpecialField;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("년월일") , sSpecialFieldAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetFieldName") );

	ids = GetCellData( m_pUserMakeSearchCondDM   , _T("알리아스")  , sSpecialFieldAlias , _T("FieldName") , sSpecialField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GetFieldName") );

	// 2012.11.24 UPDATE BY BJS : 2012년도 도서관 통합서비스 확대구축 사업 2차 (회원증 발급횟수통계 추가에 따른 쿼리 변경)
	// 회원증발급통계 산출시 쿼리 변경
	if( sAlias.Compare(_T("년")) == 0 )
	{
		if( sStaticsClass.Compare(_T("회원증발급통계"))==0 )
		{
			sSpecialField=_T("CM.REISSUE_DATE");
		}

		sFieldName.Format( _T("to_char( %s , 'YYYY' )") , sSpecialField );
	}

	else if( sAlias.Compare(_T("월")) == 0 )
	{
		if( sStaticsClass.Compare(_T("회원증발급통계"))==0 )
		{
			sSpecialField=_T("CM.REISSUE_DATE");
		}

		sFieldName.Format( _T("to_char( %s , 'YYYY/MM' )") , sSpecialField );
	}
	else if( sAlias.Compare(_T("일")) == 0 )
	{
		if( sStaticsClass.Compare(_T("회원증발급통계"))==0 )
		{
			sSpecialField=_T("CM.REISSUE_DATE");
		}

		sFieldName.Format( _T("to_char( %s , 'YYYY/MM/DD' )") , sSpecialField );
	}

	else if( sAlias.Compare(_T("시간대별")) == 0 )
	{
		if( sStaticsClass.Compare(_T("회원증발급통계"))==0 )
		{
			sSpecialField=_T("CM.REISSUE_DATE");
		}

		//++2008.09.02 DEL BY LKS {{++
//REMOVE▶ 		if( m_pInfo->MODE == 10000 || (m_pInfo->MODE == 30000) )
//REMOVE▶ 		{
//REMOVE▶ 			sFieldName.Format( _T("extract(HOUR FROM %s)") , sSpecialField );
//REMOVE▶ 		}
		//--2008.09.02 DEL BY LKS --}}
//		else
			sFieldName.Format( _T("to_char( %s , 'HH24' )") , sSpecialField );
	}
	else if( sAlias.Compare(_T("순위")) == 0 )
	{
	}


	return 0;

	EFS_END
	return -1;

}


INT CStaticsInfoMgr::GetTimeColumnNames( CString sStaticsClass , CString sAlias , CStringArray &asArray  )
{
	EFS_BEGIN

	INT ids;

	CString sControlAlias;

	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("년월일") , sControlAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetTimeColumnNames") );

	if( m_pSearchCM == NULL ) return 0;

	ids = MakeUserSearchList( sStaticsClass );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetTimeColumnNames") );

	// 저장된 조건에서 가져온다.
	
	
	CString sDate1,sDate2;
	ids = GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias+_T("1") , _T("값") , sDate1 );
	if( ids < 0 ) 
	{
		CString sMsg;
		sMsg.Format( _T("m_pSearchCM->GetControlMgrData( %s )") , sControlAlias+_T("1") );
		AfxMessageBox( sMsg );
	}
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetTimeColumnNames") );
	
	ids = GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias+_T("2") , _T("값") , sDate2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetTimeColumnNames") );

	m_pUserSearchCond->FreeData();

// 2009.05.20 UPDATE BY CJY : 일 조건이 없어도 검색가능하도록 수정
	if( sAlias.Compare(_T("시간대별")) == 0 )
	{
		CString sHour;
		for( INT i = 0 ; i < 24 ; i++ )
		{
			sHour.Format(_T("%02d"),i);
			asArray.Add(sHour);
		}

		return 0;
	}

	if( sDate1.IsEmpty() ) return 0;

	// 050126_소정수정
	// CTime -> COleDateTime 로 변경
	COleDateTime tDate1( _ttoi(sDate1.Mid(0,4)) , _ttoi(sDate1.Mid(5,2)) , _ttoi(sDate1.Mid(8,2)) , 0 , 0 , 0 );
	COleDateTime tDate2;
//	이전소스
//  	if( sDate1.IsEmpty() ) return 0;
// 
// 	// 050126_소정수정
// 	// CTime -> COleDateTime 로 변경
// 	COleDateTime tDate1( _ttoi(sDate1.Mid(0,4)) , _ttoi(sDate1.Mid(5,2)) , _ttoi(sDate1.Mid(8,2)) , 0 , 0 , 0 );
// 	COleDateTime tDate2;
// 
// 	if( sAlias.Compare(_T("시간대별")) == 0 )
// 	{
// 		CString sHour;
// 		for( INT i = 0 ; i < 24 ; i++ )
// 		{
// 			sHour.Format(_T("%02d"),i);
// 			asArray.Add(sHour);
// 		}
// 
// 		return 0;
// 	}

	if( sDate2.IsEmpty() )
		tDate2 = tDate1;
	else
		tDate2 = COleDateTime( _ttoi(sDate2.Mid(0,4)) , _ttoi(sDate2.Mid(5,2)) , _ttoi(sDate2.Mid(8,2)) , 0 , 0 , 0 );
	


	while(TRUE)
	{		
		if( tDate1 > tDate2 )
			break;
		
		if( sAlias.Compare(_T("년")) == 0 )
			asArray.Add( tDate1.Format(_T("%Y")) );
		else if( sAlias.Compare(_T("월")) == 0 )
			asArray.Add( tDate1.Format(_T("%Y/%m")) );
		else if( sAlias.Compare(_T("일")) == 0 )
			asArray.Add( tDate1.Format(_T("%Y/%m/%d")) );
		
		if( sAlias.Compare(_T("년"))==0 )
			GetNextYear(tDate1);
		else if( sAlias.Compare(_T("월"))==0 )
			GetNextMonth(tDate1);
		else if( sAlias.Compare(_T("일"))==0 )
			GetNextDay(tDate1);
	}

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::GetNextYear( COleDateTime &tTime )
{
	EFS_BEGIN

	INT nYear;
	nYear = tTime.GetYear()+1;

	tTime = COleDateTime( nYear , 1 , 1 , 0 , 0 , 0  );

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::GetNextMonth( COleDateTime &tTime )
{
	EFS_BEGIN

	INT nYear,nMonth;
	nYear = tTime.GetYear();
	nMonth = tTime.GetMonth()+1;

	if( nMonth > 12 )
	{
		nYear ++;
		nMonth = nMonth - 12;
	}

	tTime = COleDateTime( nYear , nMonth , 1 , 0 , 0 , 0  );

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::GetNextDay( COleDateTime &tTime )
{
	EFS_BEGIN

	COleDateTimeSpan tOneDay( 1 , 0 , 0 , 0 );
	tTime += tOneDay;

	return 0;

	EFS_END
	return -1;

}

// DM에 컬럼 추가하기
INT CStaticsInfoMgr::SetSearchCM( CESL_ControlMgr *pCM )
{
	EFS_BEGIN

	m_pSearchCM = pCM;
	
	return 0;

	EFS_END
	return -1;

}

// nMode == 0 : X축
// nMode == 1 : Y축
INT CStaticsInfoMgr::SetDataMgrRef( CESL_DataMgr *pDM , CString sCondition , CString sInitValue , CString sGroupYN , INT nMode)
{
	EFS_BEGIN

	CString sUdf;
	CString sAlias;

	if( sGroupYN.Compare(_T("Y")) == 0 ) sUdf = _T("");
	else sUdf = _T("udf");

	if( nMode == 0 ) 
		sAlias= _T("X축알리아스");
	else if( nMode == 1 )
		sAlias = _T("Y축알리아스");
	else 
		sAlias = _T("결과물");

	INT refcount = pDM->RefList.GetCount();
	CESL_DataMgr::reference * pRef;
	POSITION pos;
      	pos = pDM->RefList.GetHeadPosition();
	CString query, fields, isudf;
	for( INT i = 0 ; i < pDM->RefList.GetCount() ; i++ )
	{
		pRef = NULL;
		pRef = (CESL_DataMgr::reference*)pDM->RefList.GetNext(pos);

		if( pRef->FIELD_ALIAS.Compare(sAlias) == 0 )
		{
			pRef->INIT_VALUE = sInitValue;
			pRef->DATA_TYPE = sUdf;
			pRef->FIELD_NAME = sCondition;
		}
	}

	return 0;

	EFS_END
	return -1;

}


// 합계 구성
INT CStaticsInfoMgr::MakeTotalCount( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName, CString sYFieldName , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	// 세로 합계 구성
	ids = YSum(sStaticsClass,saTableName,sXFieldName , sYFieldName ,pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("MakeTotalCount") );

	// 가로 합계 구성
	ids = XSum(sStaticsClass,saTableName,sXFieldName , sYFieldName ,pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-2 , _T("MakeTotalCount") );

	return 0;

	EFS_END
	return -1;

}


/// 대출/반납 통계의 합계 구성 
/// 20040204 - 조병걸 - 사용안함
INT CStaticsInfoMgr::MakeTotalCount( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName, CString sYFieldName , CString sXAlias , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;
		
	// 세로 합계 구성
	ids = YLoanSum(sStaticsClass,saTableName,sXFieldName , sYFieldName ,sXAlias,pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("MakeTotalCount") );
	
	// 가로 합계 구성
	ids = XLoanSum(sStaticsClass,saTableName,sXFieldName , sYFieldName ,sXAlias,pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-2 , _T("MakeTotalCount") );
	return 0;
	
	EFS_END
	return -1;
	
}

//###############################################################################################################
// [ 함수설명 ]
//   대출/반납 통계의 합계 구성 
//   20040204 - 조병걸 - 그룹인 합계는 다르게 내도록 수정
//
// [ 미니스펙 ]
//
//###############################################################################################################
INT CStaticsInfoMgr::MakeTotalCount( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName, CString sYFieldName , CString sXAlias , CString sXGroupYN , CString sYGroupYN ,  CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress )
{
EFS_BEGIN
		
	INT ids;

	// ==========================================================================================================
	// 1. 합계 DM초기화
	// ==========================================================================================================
	m_pXSumDM->FreeData();
	m_pYSumDM->FreeData();

	// ==========================================================================================================
	// 2. 중복 허용 합계 구성
	// ==========================================================================================================
	ids = XYLoanSum_duplicate_allow();
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeTotalCount()") );

	// ==========================================================================================================
	// 3. 중복 삭제 합계 구성
	//    그룹 여부에 따라 달라짐
	// ==========================================================================================================
	ids = XLoanSum_duplicate_delete(sStaticsClass,saTableName,sXFieldName , sYFieldName ,sXAlias,pTotalProgress);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeTotalCount()") );

	return 0;
	
EFS_END
	return -1;	
}

//###############################################################################################################
// [ 함수설명 ]
// 20040204 조병걸 - 대출반납 중복 허용 합계
//
// [ 미니스펙 ]
//
//###############################################################################################################
INT CStaticsInfoMgr::XYLoanSum_duplicate_allow()
{
EFS_BEGIN

	INT ids;
	
	CString sSumAlias = _T("중복허용_합계");
	CString sAlias;
	CString sSum;
	CString sData;
	CESL_DataMgr::reference *Ref;
	CString strNormal, strArmy, strNormal2, strArmy2;
	POSITION pos;	

	// ==========================================================================================================
	// 1. 세로 중복 허용 합계 구성
	// ==========================================================================================================
	for ( INT i = 1;i < m_pRealResultDM->RefList.GetCount();i++ )
	{
		sSum.Empty();
		pos = m_pRealResultDM->RefList.FindIndex(i);
		Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
		sAlias = Ref->FIELD_ALIAS;
		
		for ( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( sAlias , j , sData ); 
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::XYLoanSum_duplicate_allow()") );
			
			if ( m_sIs64lib == _T("Y") && (sData.FindOneOf(_T(",")) > -1 || sSum.FindOneOf( DIVCHAR ) > -1) )
			{	
				// UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// 입력할 데이터도 분리해야한다.
				DividNormalArmy(sSum, strNormal, strArmy);				
				DividNormalArmy(sData, strNormal2, strArmy2);
				sSum.Format( _T("%d %s %d") , _ttoi(strNormal) + _ttoi(strNormal2) ,DIVCHAR , _ttoi(strArmy) + _ttoi(strArmy2) );
				// ----------------------------------------------------------------------------------------------
			}
			else
				sSum.Format( _T("%d") , _ttoi(sSum)+_ttoi(sData) );			
		}
		
		if( m_pYSumDM->GetRowCount() < i )	m_pYSumDM->InsertRow(-1);
		
		m_pYSumDM->SetCellData( sSumAlias , sSum ,i-1 );
	}	

	// ==========================================================================================================
	// 2. 가로 중복 허용 합계 구성
	// ==========================================================================================================
	sAlias.Empty();
	sSum.Empty();
	sData.Empty();
	CString sTotalSum;
	Ref = NULL;
	for( i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
	{
		sSum.Empty();
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			pos = m_pRealResultDM->RefList.FindIndex(j);
			Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
			sAlias = Ref->FIELD_ALIAS;
			
			ids = m_pRealResultDM->GetCellData( sAlias ,i , sData ); 
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::XYLoanSum_duplicate_allow()") );
			
			if ( m_sIs64lib == _T("Y") && (sData.FindOneOf( DIVCHAR ) > -1 || sSum.FindOneOf( DIVCHAR ) > -1) )
			{
				// UPDATE BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// 입력할 데이터도 분리해야한다.
				DividNormalArmy(sSum, strNormal, strArmy);				
				DividNormalArmy(sData, strNormal2, strArmy2);
				sSum.Format( _T("%d") , _ttoi(strNormal) + _ttoi(strNormal2) + _ttoi(strArmy) + _ttoi(strArmy2) );
				// ----------------------------------------------------------------------------------------------
			}
			else
				sSum.Format( _T("%d") , _ttoi(sSum)+_ttoi(sData) );
		}
		
		sTotalSum.Format( _T("%d") , _ttoi(sTotalSum) + _ttoi(sSum) );
		
		if( m_pXSumDM->GetRowCount() < i+1 )	m_pXSumDM->InsertRow(-1);
		
		m_pXSumDM->SetCellData( sSumAlias , sSum , i);		
	}
	
	if( m_pXSumDM->GetRowCount() < i+1 )	m_pXSumDM->InsertRow(-1);
	
	m_pXSumDM->SetCellData( sSumAlias , sTotalSum , i );
	
	return 0;

EFS_END
	return -1;
}

//###############################################################################################################
// [ 함수설명 ]
//   대출통계 중복제거 합계 
// 
// [ 미니스펙 ]
//
//###############################################################################################################
INT CStaticsInfoMgr::XLoanSum_duplicate_delete( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CString sXAlias , CProgressCtrl *pTotalProgress )
{
EFS_BEGIN
	
	INT ids;

	INT i = 0;
	
	// ==========================================================================================================
	// 1. 기간 통계 구분
	// ==========================================================================================================
	CString sResultFieldName;
	if ( sXAlias.Compare(_T("년")) == 0 || (sXAlias.Compare(_T("월")) == 0) || (sXAlias.Compare(_T("일")) == 0) )
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물") , sResultFieldName );
	else
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물2") , sResultFieldName );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	
	// ==========================================================================================================
	// 2. DM 테이블 이름 추가
	// ==========================================================================================================
	m_pResultDM->TBL_NAME = saTableName.GetAt(0);
		
	CStringArray saTableAlias;

	ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
			
	// ==========================================================================================================
	// 3. 가로 합계 계산
	//    where 절을 구성하여 DM을 재구성한후 결과DM에 덮어쓴다.
	// ==========================================================================================================
	// SELECT 필드 설정
	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("Y") , 1 );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sResultFieldName , _T("0") , _T("Y") , 2 );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	
	// WHERE절 구성 		
	CString sWhere;
	ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , sYFieldName , _T("") , _T("") , _T("N") , _T("Y") ,i , sWhere );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );

	sWhere += _T(" GROUP BY ") + sYFieldName;
		
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	
	CString sYSumAlias;
	CString sTmpResult , sResult;
	CString sYData , sTmp;	
	INT nIndex;

	// 알리아스에 맞는 인덱스를 구하여 데이터를 입력한다.
	for ( INT k = 0;k < m_pResultDM->GetRowCount();k++ )
	{
		ids = m_pResultDM->GetCellData( _T("Y축알리아스") , k , sYSumAlias );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
		
		nIndex = -1;
		for ( INT j = 0;j < m_pRealResultDM->GetRowCount();j++ )
		{
			ids = m_pRealResultDM->GetCellData( j , 0 , sYData );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
			
			if ( sYData.Compare( sYSumAlias ) == 0 ) nIndex = j;
		}
		
		if ( nIndex == -1 ) continue;
		
		ids = m_pResultDM->GetCellData( _T("결과물") , k , sResult );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
		
		if ( _ttoi(sResult) == 0 )	sResult = _T("0");
		
		ids = m_pXSumDM->SetCellData( _T("중복삭제_합계") , sResult , nIndex );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );		
	}

	// ==========================================================================================================
	// 4. 세로 합계 계산
	//    where 절을 구성하여 DM을 재구성한후 결과DM에 덮어쓴다.
	// ==========================================================================================================
	// SELECT 필드 설정
	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("Y") , 0 );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sResultFieldName , _T("0") , _T("Y") , 2 );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
		
	// WHERE절 생성
	sWhere.Empty();
	ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , sXFieldName , _T("") , _T("") , _T("") ,_T("Y") , _T("N") , i , sWhere );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );

	sWhere += _T(" GROUP BY ") + sXFieldName;
	
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -16 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );

	CString sXSumAlias;
	sTmpResult.Empty() , sResult.Empty();
	CString sXData;
	sTmp.Empty();	

	for ( k = 0 ; k < m_pResultDM->GetRowCount() ; k++ )
	{
		ids = m_pResultDM->GetCellData( _T("X축알리아스") , k , sXSumAlias );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
		
		nIndex = -1;

		// 2005.07.28 ADD BY PDJ ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 육사 군사분류 합계 부분 예외처리
		if ( m_sIs64lib == _T("Y") && ( sXSumAlias==_T("355-9") ) )  nIndex = 4;		
		else
		{		
			for ( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if ( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );

				if ( sXData.Compare( sXSumAlias ) == 0 ) nIndex = j;
			}
		}		
		
		if ( nIndex == -1 ) continue;

		ids = m_pResultDM->GetCellData( _T("결과물") , k , sResult );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );

		if ( m_sIs64lib == _T("Y") && ( sXSumAlias==_T("355-9") || sXSumAlias==_T("3") ) ) 
		{
			CString strNormal, strArmy;
			CString sPrevData;
			ids = m_pYSumDM->GetCellData( _T("중복삭제_합계") , nIndex-1 , sPrevData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -32 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );				

			DividNormalArmy(sPrevData, strNormal, strArmy);

			if ( sXSumAlias==_T("3") )
				sResult.Format(_T("%d %s %d"), _ttoi(strNormal)+_ttoi(sResult), DIVCHAR, _ttoi(strArmy) );
			else
				sResult.Format(_T("%d %s %d"), _ttoi(strNormal), DIVCHAR, _ttoi(strArmy)+_ttoi(sResult) );			
		}

		if ( m_sIs64lib == _T("Y") && ( sXSumAlias==_T("355-9") || sXSumAlias==_T("3") ) ) 		
		{
			CString strNormal, strArmy;
			DividNormalArmy(sResult, strNormal, strArmy);

			if( _ttoi(strArmy) == 0 && _ttoi(strNormal) == 0 )	sResult = _T("0");		
		}
		else
			if( _ttoi(sResult) == 0 )	sResult = _T("0");		
		// ------------------------------------------------------------------------------------------------------

		ids = m_pYSumDM->SetCellData( _T("중복삭제_합계") , sResult , nIndex-1  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );				
	}
	
	// ==========================================================================================================
	// 5. 총합계
	// ==========================================================================================================
	// SELECT 필드 설정
	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -22 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sResultFieldName , _T("0") , _T("Y") , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -24 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	
	// WHERE절 생성
	ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
	
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -25 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
			
	ids = m_pResultDM->GetCellData( _T("결과물") , 0 , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -26 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	
	if( _ttoi(sResult) == 0 )	sResult = _T("0");
	
	ids = m_pXSumDM->SetCellData( _T("중복삭제_합계") , sResult , m_pXSumDM->GetRowCount()-1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -27 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );	
	
	// ==========================================================================================================
	// 6. 모든 합계중 비어있는 공간은 0으로 만들기
	// ==========================================================================================================
	for( INT m = 0 ; m < m_pXSumDM->GetRowCount() ; m++ )
	{
		ids = m_pXSumDM->GetCellData( _T("중복삭제_합계") , m , sTmp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -28 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );

		sTmp.TrimLeft(); sTmp.TrimRight();
		if( !sTmp.IsEmpty() ) continue;

		ids = m_pXSumDM->SetCellData( _T("중복삭제_합계") , _T("0") , m );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -29 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	}

	for( m = 0 ; m < m_pYSumDM->GetRowCount() ; m++ )
	{
		ids = m_pYSumDM->GetCellData( _T("중복삭제_합계") , m , sTmp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -30 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
		
		sTmp.TrimLeft(); sTmp.TrimRight();
		if( !sTmp.IsEmpty() ) continue;
		
		ids = m_pYSumDM->SetCellData( _T("중복삭제_합계") , _T("0") , m );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -31 , _T("CStaticsInfoMgr::XLoanSum_duplicate_delete()") );
	}
	
	return 0;	

EFS_END
	return -1;
}


INT CStaticsInfoMgr::MakeTotalCount3( CString sStaticsClass , CStringArray &saTableName , CStringArray &saXFieldName, CStringArray &saYFieldName , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN

	INT ids;

	// 세로 구성
	ids = YSum3(sStaticsClass,saTableName,saXFieldName , saYFieldName ,pTotalProgress , pRegNoDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("") );

	// 가로 합계 구성
	ids = XSum3(sStaticsClass,saTableName,saXFieldName , saYFieldName ,pTotalProgress , pRegNoDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("") );

	return 0;


	EFS_END
	return -1;

}

// 가격 합산 구성하기
INT CStaticsInfoMgr::MakePriceSum3( CString sStaticsClass , CStringArray &saTableName , CStringArray &saXFieldName, CStringArray &saYFieldName , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN

	INT ids;

	// 세로 합계 구성
	ids = YPriceSum3(sStaticsClass,saTableName,saXFieldName , saYFieldName ,pTotalProgress , pRegNoDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("") );

	// 가로 합계 구성
	ids = XPriceSum3(sStaticsClass,saTableName,saXFieldName , saYFieldName ,pTotalProgress , pRegNoDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("") );

	return 0;


	EFS_END
	return -1;

}

INT CStaticsInfoMgr::PostInsertDMColumn( CESL_DataMgr *pDM )
{
	EFS_BEGIN

	INT ids;

	INT nInsertCnt = pDM->RefList.GetCount();

	CString sData;

	INT nRowCount = pDM->GetRowCount();

	for( INT i = 0 ; i < nRowCount; i++ )
	{
		pDM->InsertRow(i);
		
		for( INT j = 0 ; j < nInsertCnt ; j++ )
		{
			if( j == nInsertCnt-1 )
				sData = _T("0");
			else
			{
				ids = pDM->GetCellData( i+1 , j , sData );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PostInsertDMColumn") );

			}

			ids = pDM->SetCellData( i , j , sData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PostInsertDMColumn") );

		}	
		pDM->DeleteRow( i+1 , nInsertCnt-1 );

		CString sTest;
		pDM->GetCellData( i , 0 , sTest );
	}

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::MakeStaticSql( CStringArray &saTableAlias ,CString sXFieldName ,  CString sXData , CString sYFieldName , CString sYData , CString &sInStaticWhere , CString &sOutStaticWhere )
{
	EFS_BEGIN

	sInStaticWhere.Empty();
	sOutStaticWhere.Empty();

	CString sTmpWhere;
	CString sTmpField;
	CString sTmpData;

	for( INT i = 0 ; i < 2 ; i++ )
	{
		sTmpWhere.Empty();
		sTmpData.Empty();

		if( i == 0 )
		{
			sTmpField = sXFieldName;
			sTmpData = sXData;
		}
		else
		{
			sTmpField = sYFieldName;
			sTmpData = sYData;
		}

		if( sTmpField.IsEmpty() ) continue;
		
		if( sTmpData.IsEmpty() )
			sTmpWhere.Format( _T(" %s IS NULL ") , sTmpField );
		else
			sTmpWhere.Format( _T(" %s = '%s' ") , sTmpField , sTmpData );
		
		if( IsFieldFromTable( sTmpField , saTableAlias ) == 1 )
		{
			if( sOutStaticWhere.GetLength() > 0 ) sOutStaticWhere += _T(" AND ");
			sOutStaticWhere += sTmpWhere;
		}
		else
		{
			if( sInStaticWhere.GetLength() > 0 ) sInStaticWhere += _T(" AND ");
			sInStaticWhere += sTmpWhere;
		}

	}

	return 0;

	EFS_END
	return -1;

}


// 세로 합계 구성
INT CStaticsInfoMgr::YSum( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	m_pYSumDM->FreeData();
	// 세로 합계 구성
	pTotalProgress->SetRange32( 0 , saTableName.GetSize()*(m_pRealResultDM->RefList.GetCount()+m_pRealResultDM->GetRowCount()) );
	INT nPos = 0;

	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM 테이블 이름 추가
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);
		
		CStringArray saTableAlias;

		ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

		// 3.3. WHERE절 구성 
		CString sWhere;
		CString sTmpResult , sResult;
		CString sXData , sTmp;
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			sWhere.Empty();
			ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
			if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

			// SELECT 필드 설정
			ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
			ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );


			// WHERE절 생성
			ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , sXFieldName , _T("") , sXData , _T("") ,_T("N") , _T("N")  , i , sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeTotalCount") );

			ids = m_pResultDM->RefreshDataManager( sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

			// RESULT DM -> REAL RESULT DM 으로 이동
			// RESULT DM -> REAL RESULT DM 으로 이동
			if( m_pYSumDM->GetRowCount() < j )
				m_pYSumDM->InsertRow(-1);

			ids = m_pYSumDM->GetCellData( _T("중복삭제_합계") , j-1 , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			ids = m_pResultDM->GetCellData( _T("결과물") , 0 , sTmpResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			if ( m_sIs64lib == _T("Y") && sTmpResult.FindOneOf( DIVCHAR ) > -1 )
			{				
				CString strNormal, strArmy, strNormal2, strArmy2;
				
				DividNormalArmy(sResult, strNormal, strArmy);				
				DividNormalArmy(sTmpResult, strNormal2, strArmy2);
				sResult.Format( _T("%d %s %d") , _ttoi(strNormal) + _ttoi(strNormal2),DIVCHAR , _ttoi(strArmy) + _ttoi(strArmy2) );
			}
			else
			{
				sResult.Format( _T("%d") , _ttoi(sTmpResult) + _ttoi(sResult) );
			}
			
			ids = m_pYSumDM->SetCellData( _T("중복삭제_합계") , sResult , j-1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			pTotalProgress->SetPos( nPos+1 ); nPos++;

		}
	}

	// 중복 허용 합계 구성

	CString sSumAlias = _T("중복허용_합계");
	CString sAlias;
	CString sSum;
	CString sData;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	for( i = 1 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		sSum.Empty();
		pos = m_pRealResultDM->RefList.FindIndex(i);
		Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
		sAlias = Ref->FIELD_ALIAS;

		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( sAlias , j , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );

			if ( m_sIs64lib == _T("Y") && sData.FindOneOf( DIVCHAR ) > -1 )
			{				
				CString strNormal, strArmy, strNormal2, strArmy2;
				
				DividNormalArmy(sSum, strNormal, strArmy);				
				DividNormalArmy(sData, strNormal2, strArmy2);
				sSum.Format( _T("%d %s %d") , _ttoi(strNormal) + _ttoi(strNormal2), DIVCHAR, _ttoi(strArmy) + _ttoi(strArmy2) );
			}
			else
			{
				sSum.Format( _T("%d") , _ttoi(sSum)+_ttoi(sData) );
			}
		}
		
		if( m_pYSumDM->GetRowCount() < i )
			m_pYSumDM->InsertRow(-1);

		m_pYSumDM->SetCellData( sSumAlias , sSum ,i-1 );
	}	

	return 0;

	EFS_END
	return -1;

}

// 가로 합계 구성
INT CStaticsInfoMgr::XSum( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	m_pXSumDM->FreeData();

	// 중복 삭제 합계 구성
	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM 테이블 이름 추가
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);
		
		CStringArray saTableAlias;

		ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

		// 3.3. WHERE절 구성 
		CString sWhere;
		/*
		, sDefaultCond , sUserCond;
		*/

		/*
		ids = GetDefaultCond( sStaticsClass ,sDefaultCond ,sUserCond ,sWhere );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );
		*/
		
		CString sInStaticWhere , sOutStaticWhere , sSendWhere;		
		CString sYData;
		CString sTmpResult , sResult;
		for( INT k = 0 ; k < m_pRealResultDM->GetRowCount()+1 ; k++ )
		{
			sWhere.Empty();
			if( k == m_pRealResultDM->GetRowCount() )
				sYData = _T("합계");
			else
			{
				ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			}

			// SELECT 필드 설정
			ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
			ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );
			
			// WHERE절 생성
			sSendWhere = sWhere;
			/*
			if( sYData.Compare(_T("합계"))==0 )
			{
				sInStaticWhere = _T("");
				sOutStaticWhere = _T("");
			}
			else
			{
				ids = MakeStaticSql( saTableAlias , _T("") , _T("") , sYFieldName , sYData , sInStaticWhere , sOutStaticWhere);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
				if( sUserCond.GetLength() > 6&& !sInStaticWhere.IsEmpty() ) 
					sInStaticWhere = CString( _T(" AND ")  + sInStaticWhere );
				if( sUserCond.GetLength() > 6&& !sOutStaticWhere.IsEmpty() ) 
					sOutStaticWhere = CString( _T(" AND ")  + sOutStaticWhere );

			}
			*/
			if( sYData.Compare(_T("합계"))==0 )
				ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
			else
				ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , sYFieldName , _T("") , sYData , _T("N") , _T("N") ,i , sWhere );

			/*
			sSendWhere.Replace( _T("%안그룹조건%") , sInStaticWhere );
			sSendWhere.Replace( _T("%밖그룹조건%") , sOutStaticWhere );
			*/
			
			ids = m_pResultDM->RefreshDataManager( sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			// RESULT DM -> REAL RESULT DM 으로 이동
			if( m_pXSumDM->GetRowCount() < k+1 )
				m_pXSumDM->InsertRow(-1);

			ids = m_pXSumDM->GetCellData( _T("중복삭제_합계") , k , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			ids = m_pResultDM->GetCellData( _T("결과물") , 0 , sTmpResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			sResult.Format( _T("%d") , _ttoi(sTmpResult) + _ttoi(sResult) );
			
			ids = m_pXSumDM->SetCellData( _T("중복삭제_합계") , sResult , k );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			pTotalProgress->SetPos( (i+1)*(k+1)+(saTableName.GetSize()*m_pRealResultDM->RefList.GetCount()) );
		}	
	}

	// 중복 허용 합계 구성
	CString sSumAlias = _T("중복허용_합계");
	CString sAlias;
	CString sSum;
	CString sData;
	CString sTotalSum;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	for( i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
	{
		sSum.Empty();
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			pos = m_pRealResultDM->RefList.FindIndex(j);
			Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
			sAlias = Ref->FIELD_ALIAS;

			ids = m_pRealResultDM->GetCellData( sAlias ,i , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );

			sSum.Format( _T("%d") , _ttoi(sSum)+_ttoi(sData) );
		}
		
		sTotalSum.Format( _T("%d") , _ttoi(sTotalSum) + _ttoi(sSum) );

		if( m_pXSumDM->GetRowCount() < i+1 )
			m_pXSumDM->InsertRow(-1);

		m_pXSumDM->SetCellData( sSumAlias , sSum , i);
		
	}

	if( m_pXSumDM->GetRowCount() < i+1 )
		m_pXSumDM->InsertRow(-1);

	m_pXSumDM->SetCellData( sSumAlias , sTotalSum , i );
	
	return 0;

	EFS_END
	return -1;

}

// 세로 합계 구성 - 20040204 사용안함
INT CStaticsInfoMgr::YLoanSum( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CString sXAlias , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	CString sResultField;
	if( sXAlias.Compare(_T("년")) == 0 || (sXAlias.Compare(_T("월")) == 0) || (sXAlias.Compare(_T("일")) == 0) )
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물") , sResultField );
	else
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물2") , sResultField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );

	SetDataMgrRef( m_pResultDM , sResultField , _T("0") , _T("Y") , 2 );

	m_pYSumDM->FreeData();
	// 세로 합계 구성
	pTotalProgress->SetRange32( 0 , saTableName.GetSize()*(m_pRealResultDM->RefList.GetCount()+m_pRealResultDM->GetRowCount()) );
	INT nPos = 0;

	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM 테이블 이름 추가
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);
		
		CStringArray saTableAlias;

		ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

		// 3.3. WHERE절 구성 
		CString sWhere;
		CString sTmpResult , sResult;
		CString sXData , sTmp;
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			sWhere.Empty();
			ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
			if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

			// SELECT 필드 설정
			ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
			ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );


			// WHERE절 생성
			ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , sXFieldName , _T("") , sXData , _T("") ,_T("N") , _T("N")  , i , sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeTotalCount") );

			ids = m_pResultDM->RefreshDataManager( sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

			// RESULT DM -> REAL RESULT DM 으로 이동
			// RESULT DM -> REAL RESULT DM 으로 이동
			if( m_pYSumDM->GetRowCount() < j )
				m_pYSumDM->InsertRow(-1);

			ids = m_pYSumDM->GetCellData( _T("중복삭제_합계") , j-1 , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			ids = m_pResultDM->GetCellData( _T("결과물") , 0 , sTmpResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			sResult.Format( _T("%d") , _ttoi(sTmpResult) + _ttoi(sResult) );
			
			ids = m_pYSumDM->SetCellData( _T("중복삭제_합계") , sResult , j-1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			pTotalProgress->SetPos( nPos+1 ); nPos++;

		}
	}

	// 중복 허용 합계 구성
	CString sSumAlias = _T("중복허용_합계");
	CString sAlias;
	CString sSum;
	CString sData;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	for( i = 1 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		sSum.Empty();
		pos = m_pRealResultDM->RefList.FindIndex(i);
		Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
		sAlias = Ref->FIELD_ALIAS;

		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( sAlias , j , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );

			sSum.Format( _T("%d") , _ttoi(sSum)+_ttoi(sData) );
		}
		
		if( m_pYSumDM->GetRowCount() < i )
			m_pYSumDM->InsertRow(-1);

		m_pYSumDM->SetCellData( sSumAlias , sSum ,i-1 );
	}	

	return 0;

	EFS_END
	return -1;

}

// 가로 합계 구성 - 20040204 사용안함
INT CStaticsInfoMgr::XLoanSum( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CString sXAlias , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	CString sResultField;
	if( sXAlias.Compare(_T("년")) == 0 || (sXAlias.Compare(_T("월")) == 0) || (sXAlias.Compare(_T("일")) == 0) )
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물") , sResultField );
	else
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물2") , sResultField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	
	SetDataMgrRef( m_pResultDM , sResultField , _T("0") , _T("Y") , 2 );

	m_pXSumDM->FreeData();

	// 중복 삭제 합계 구성
	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM 테이블 이름 추가
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);
		
		CStringArray saTableAlias;

		ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

		// 3.3. WHERE절 구성 
		CString sWhere;
		/*
		, sDefaultCond , sUserCond;
		*/

		/*
		ids = GetDefaultCond( sStaticsClass ,sDefaultCond ,sUserCond ,sWhere );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );
		*/
		
		CString sInStaticWhere , sOutStaticWhere , sSendWhere;		
		CString sYData;
		CString sTmpResult , sResult;
		for( INT k = 0 ; k < m_pRealResultDM->GetRowCount()+1 ; k++ )
		{
			sWhere.Empty();
			if( k == m_pRealResultDM->GetRowCount() )
				sYData = _T("합계");
			else
			{
				ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			}

			// SELECT 필드 설정
			ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
			ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );
			
			// WHERE절 생성
			sSendWhere = sWhere;
			/*
			if( sYData.Compare(_T("합계"))==0 )
			{
				sInStaticWhere = _T("");
				sOutStaticWhere = _T("");
			}
			else
			{
				ids = MakeStaticSql( saTableAlias , _T("") , _T("") , sYFieldName , sYData , sInStaticWhere , sOutStaticWhere);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );
				if( sUserCond.GetLength() > 6&& !sInStaticWhere.IsEmpty() ) 
					sInStaticWhere = CString( _T(" AND ")  + sInStaticWhere );
				if( sUserCond.GetLength() > 6&& !sOutStaticWhere.IsEmpty() ) 
					sOutStaticWhere = CString( _T(" AND ")  + sOutStaticWhere );

			}
			*/
			if( sYData.Compare(_T("합계"))==0 )
				ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
			else
				ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , sYFieldName , _T("") , sYData , _T("N") , _T("N") ,i , sWhere );

			/*
			sSendWhere.Replace( _T("%안그룹조건%") , sInStaticWhere );
			sSendWhere.Replace( _T("%밖그룹조건%") , sOutStaticWhere );
			*/
			
			ids = m_pResultDM->RefreshDataManager( sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			// RESULT DM -> REAL RESULT DM 으로 이동
			if( m_pXSumDM->GetRowCount() < k+1 )
				m_pXSumDM->InsertRow(-1);

			ids = m_pXSumDM->GetCellData( _T("중복삭제_합계") , k , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			ids = m_pResultDM->GetCellData( _T("결과물") , 0 , sTmpResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			sResult.Format( _T("%d") , _ttoi(sTmpResult) + _ttoi(sResult) );
			
			ids = m_pXSumDM->SetCellData( _T("중복삭제_합계") , sResult , k );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			pTotalProgress->SetPos( (i+1)*(k+1)+(saTableName.GetSize()*m_pRealResultDM->RefList.GetCount()) );
		}	
	}

	// 중복 허용 합계 구성
	CString sSumAlias = _T("중복허용_합계");
	CString sAlias;
	CString sSum;
	CString sData;
	CString sTotalSum;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	for( i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
	{
		sSum.Empty();
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			pos = m_pRealResultDM->RefList.FindIndex(j);
			Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
			sAlias = Ref->FIELD_ALIAS;

			ids = m_pRealResultDM->GetCellData( sAlias ,i , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );

			sSum.Format( _T("%d") , _ttoi(sSum)+_ttoi(sData) );
		}
		
		sTotalSum.Format( _T("%d") , _ttoi(sTotalSum) + _ttoi(sSum) );

		if( m_pXSumDM->GetRowCount() < i+1 )
			m_pXSumDM->InsertRow(-1);

		m_pXSumDM->SetCellData( sSumAlias , sSum , i);
		
	}

	if( m_pXSumDM->GetRowCount() < i+1 )
		m_pXSumDM->InsertRow(-1);

	m_pXSumDM->SetCellData( sSumAlias , sTotalSum , i );
	
	return 0;

	EFS_END
	return -1;

}


// 가로 합계 구성
INT CStaticsInfoMgr::XSum3( CString sStaticsClass , CStringArray &saTableName , CStringArray &saXFieldName , CStringArray &saYFieldName , CProgressCtrl *pTotalProgress , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN

	INT ids;

	m_pXSumDM->FreeData();

	CString sRegNoList;
	CString sRegNo;
	bool IsSend;
	//=====================================================
	//2008.11.24 UPDATE BY PJW : 등록번호갯수 제한을 1000개 이하로 바꾼다.
	// 	INT nLimitCnt = 50;
    INT nLimitCnt = 999;
	//=====================================================

	for( INT n = 0 ; n < pRegNoDM->GetRowCount() ; n++ )
	{
		IsSend = FALSE;

		ids = pRegNoDM->GetCellData( _T("등록번호") , n , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

		if( !sRegNoList.IsEmpty() )
			if( !sRegNo.IsEmpty() )
				sRegNoList += _T(",");
		if( !sRegNo.IsEmpty() )
			sRegNoList += _T("'") + sRegNo + _T("'");

		if( (n!=0) && (n%nLimitCnt==0) ) IsSend = TRUE;
		if( n == pRegNoDM->GetRowCount()-1 ) IsSend = TRUE;

		if( !IsSend ) continue;

		if( sRegNoList.IsEmpty() ) continue;

		// 중복 삭제 합계 구성
		for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
		{
			// DM 테이블 이름 추가
			m_pResultDM->TBL_NAME = saTableName.GetAt(i);
			
			CStringArray saTableAlias;

			ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

			// 3.3. WHERE절 구성 
			CString sWhere,sSendWhere;		
			CString sYData;
			CString sTmpResult , sResult;
			for( INT k = 0 ; k < m_pRealResultDM->GetRowCount()+1 ; k++ )
			{
				sWhere.Empty();
				if( k == m_pRealResultDM->GetRowCount() )
					sYData = _T("합계");
				else
				{
					ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				}

				// SELECT 필드 설정
				ids = SetDataMgrRef( m_pResultDM , saXFieldName.GetAt(i) , _T("0") , _T("N") , 0 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
				ids = SetDataMgrRef( m_pResultDM , saYFieldName.GetAt(i) , _T("0") , _T("N") , 1 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );
				
				// WHERE절 생성
				sSendWhere = sWhere;
				if( sYData.Compare(_T("합계"))==0 )
					ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
				else
				// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
				//ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , saYFieldName.GetAt(i) , _T("") , sYData , _T("N") , _T("N") ,i , sWhere );
				{
					ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , saYFieldName.GetAt(i) , _T("") , sYData , _T("N") , _T("N") ,i , sWhere );
					// 셀클릭용 데이터 저장
					if( m_pAxisCode_CellClick != NULL )
					{
						m_pAxisCode_CellClick->AddFieldNameVal( 1 , k , i , saYFieldName.GetAt(i) , sYData );
					}
				}
				// KOL.RED.2018.211 ---------------------------------------------------------------------------

				sSendWhere = _T(" B.REG_NO IN (") + sRegNoList +_T(")");
				if( !sWhere.IsEmpty() ) 
					sSendWhere += _T(" AND ") + sWhere ;

				ids = m_pResultDM->RefreshDataManager( sSendWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				// RESULT DM -> REAL RESULT DM 으로 이동
				if( m_pXSumDM->GetRowCount() < k+1 )
					m_pXSumDM->InsertRow(-1);

				ids = m_pXSumDM->GetCellData( _T("중복삭제_합계") , k , sResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				ids = m_pResultDM->GetCellData( _T("결과물") , 0 , sTmpResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				sResult.Format( _T("%d") , _ttoi(sTmpResult) + _ttoi(sResult) );
				
				ids = m_pXSumDM->SetCellData( _T("중복삭제_합계") , sResult , k );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				pTotalProgress->SetPos( (i+1)*(k+1)+(saTableName.GetSize()*m_pRealResultDM->RefList.GetCount()) );
			}	
		}
		sRegNoList.Empty();
	}

	// 중복 허용 합계 구성
	CString sSumAlias = _T("중복허용_합계");
	CString sAlias;
	CString sSum;
	CString sData;
	CString sTotalSum;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	for( INT i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
	{
		sSum.Empty();
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			pos = m_pRealResultDM->RefList.FindIndex(j);
			Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
			sAlias = Ref->FIELD_ALIAS;

			ids = m_pRealResultDM->GetCellData( sAlias ,i , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );

			sSum.Format( _T("%d") , _ttoi(sSum)+_ttoi(sData) );
		}
		
		sTotalSum.Format( _T("%d") , _ttoi(sTotalSum) + _ttoi(sSum) );

		if( m_pXSumDM->GetRowCount() < i+1 )
			m_pXSumDM->InsertRow(-1);

		m_pXSumDM->SetCellData( sSumAlias , sSum , i);
		
	}

	if( m_pXSumDM->GetRowCount() < i+1 )
		m_pXSumDM->InsertRow(-1);

	m_pXSumDM->SetCellData( sSumAlias , sTotalSum , i );
	
	return 0;

	EFS_END
	return -1;

}

// 세로 합계 구성
INT CStaticsInfoMgr::YSum3( CString sStaticsClass , CStringArray &saTableName , CStringArray &saXFieldName , CStringArray &saYFieldName , CProgressCtrl *pTotalProgress , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN


	INT ids;
	// 세로 합계 구성
	pTotalProgress->SetRange32( 0 , saTableName.GetSize()*(m_pRealResultDM->RefList.GetCount()+m_pRealResultDM->GetRowCount()) );

	m_pYSumDM->FreeData();

	CString sRegNoList;
	CString sRegNo;
	bool IsSend;
	//=====================================================
	//2008.11.24 UPDATE BY PJW : 등록번호갯수 제한을 1000개 이하로 바꾼다.
	// 	INT nLimitCnt = 50;
    INT nLimitCnt = 999;
	//=====================================================
	for( INT n = 0 ; n < pRegNoDM->GetRowCount() ; n++ )
	{
		IsSend = FALSE;

		ids = pRegNoDM->GetCellData( _T("등록번호") , n , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

		if( !sRegNoList.IsEmpty() )
			if( !sRegNo.IsEmpty() )
				sRegNoList += _T(",");
		if( !sRegNo.IsEmpty() )
			sRegNoList += _T("'") + sRegNo + _T("'");

		if( (n!=0) && (n%nLimitCnt==0) ) IsSend = TRUE;
		if( n == pRegNoDM->GetRowCount()-1 ) IsSend = TRUE;

		if( !IsSend ) continue;

		if( sRegNoList.IsEmpty() ) continue;

		for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
		{
			// DM 테이블 이름 추가
			m_pResultDM->TBL_NAME = saTableName.GetAt(i);
			
			CStringArray saTableAlias;

			ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

			// 3.3. WHERE절 구성 
			CString sWhere,sSendWhere;
			CString sTmpResult , sResult;
			CString sXData , sTmp;
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				sWhere.Empty();
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

				// SELECT 필드 설정
				ids = SetDataMgrRef( m_pResultDM , saXFieldName.GetAt(i) , _T("0") , _T("N") , 0 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
				ids = SetDataMgrRef( m_pResultDM , saYFieldName.GetAt(i) , _T("0") , _T("N") , 1 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );

				// WHERE절 생성
				ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , saXFieldName.GetAt(i) , _T("") , sXData , _T("") , _T("N") , _T("N") , i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeTotalCount") );

				// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
				// 셀클릭용 데이터 저장
				if( m_pAxisCode_CellClick != NULL )
				{
					m_pAxisCode_CellClick->AddFieldNameVal( 0 , j-1 , i , saXFieldName.GetAt(i) , sXData );
				}
				// KOL.RED.2018.211 ---------------------------------------------------------------------------

				sSendWhere = _T(" B.REG_NO IN (") + sRegNoList + _T(")");
				if( !sWhere.IsEmpty() ) 
					sSendWhere += _T(" AND ") + sWhere ;

				ids = m_pResultDM->RefreshDataManager( sSendWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

				// RESULT DM -> REAL RESULT DM 으로 이동
				// RESULT DM -> REAL RESULT DM 으로 이동
				if( m_pYSumDM->GetRowCount() < j )
					m_pYSumDM->InsertRow(-1);

				ids = m_pYSumDM->GetCellData( _T("중복삭제_합계") , j-1 , sResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				ids = m_pResultDM->GetCellData( _T("결과물") , 0 , sTmpResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				sResult.Format( _T("%d") , _ttoi(sTmpResult) + _ttoi(sResult) );
				
				ids = m_pYSumDM->SetCellData( _T("중복삭제_합계") , sResult , j-1 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				pTotalProgress->SetPos( (i+1)*(j+1) );

			}
		}
		sRegNoList.Empty();
	}
	// 중복 허용 합계 구성
	CString sSumAlias = _T("중복허용_합계");
	CString sAlias;
	CString sSum;
	CString sData;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	for( INT i = 1 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		sSum.Empty();
		pos = m_pRealResultDM->RefList.FindIndex(i);
		Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
		sAlias = Ref->FIELD_ALIAS;

		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( sAlias , j , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );

			sSum.Format( _T("%d") , _ttoi(sSum)+_ttoi(sData) );
		}
		
		if( m_pYSumDM->GetRowCount() < i )
			m_pYSumDM->InsertRow(-1);

		m_pYSumDM->SetCellData( sSumAlias , sSum ,i-1 );
	}	

	return 0;

	EFS_END
	return -1;

}

// 가로 합계 구성
INT CStaticsInfoMgr::XPriceSum3( CString sStaticsClass , CStringArray &saTableName , CStringArray &saXFieldName , CStringArray &saYFieldName , CProgressCtrl *pTotalProgress , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN

	INT ids;

	m_pXPriceSumDM->FreeData();

	CString sRegNoList;
	CString sRegNo;
	bool IsSend;
	//=====================================================
	//2008.11.24 UPDATE BY PJW : 등록번호갯수 제한을 1000개 이하로 바꾼다.
	// 	INT nLimitCnt = 50;
    INT nLimitCnt = 999;
	//=====================================================
	for( INT n = 0 ; n < pRegNoDM->GetRowCount() ; n++ )
	{
		IsSend = FALSE;

		ids = pRegNoDM->GetCellData( _T("등록번호") , n , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

		if( !sRegNoList.IsEmpty() )
			if( !sRegNo.IsEmpty() )
				sRegNoList += _T(",");
		if( !sRegNo.IsEmpty() )
			sRegNoList += _T("'") + sRegNo + _T("'");

		if( (n!=0) && (n%nLimitCnt==0) ) IsSend = TRUE;
		if( n == pRegNoDM->GetRowCount()-1 ) IsSend = TRUE;

		if( !IsSend ) continue;

		if( sRegNoList.IsEmpty() ) continue;

		// 중복 삭제 합계 구성
		for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
		{
			// DM 테이블 이름 추가
			m_pResultDM->TBL_NAME = saTableName.GetAt(i);
			
			CStringArray saTableAlias;

			ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

			// 3.3. WHERE절 구성 
			CString sWhere,sSendWhere;		
			CString sYData;
			CString sTmpResult , sResult;
			for( INT k = 0 ; k < m_pRealResultDM->GetRowCount()+1 ; k++ )
			{
				sWhere.Empty();
				if( k == m_pRealResultDM->GetRowCount() )
					sYData = _T("합계");
				else
				{
					ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				}

				// SELECT 필드 설정
				ids = SetDataMgrRef( m_pResultDM , saXFieldName.GetAt(i) , _T("0") , _T("N") , 0 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
				ids = SetDataMgrRef( m_pResultDM , saYFieldName.GetAt(i) , _T("0") , _T("N") , 1 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );
				ids = SetDataMgrRef( m_pResultDM , _T("SUM(PRICE)") , _T("0") , _T("Y") , 2 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );
				
				// WHERE절 생성
				sSendWhere = sWhere;
				if( sYData.Compare(_T("합계"))==0 )
					ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
				else
					ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , saYFieldName.GetAt(i) , _T("") , sYData , _T("N") , _T("N") ,i , sWhere );


				sSendWhere = _T(" B.REG_NO IN (") + sRegNoList +_T(")");
				if( !sWhere.IsEmpty() ) 
					sSendWhere += _T(" AND ") + sWhere ;

				ids = m_pResultDM->RefreshDataManager( sSendWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				// RESULT DM -> REAL RESULT DM 으로 이동
				if( m_pXPriceSumDM->GetRowCount() < k+1 )
					m_pXPriceSumDM->InsertRow(-1);

				ids = m_pXPriceSumDM->GetCellData( _T("중복삭제_합계") , k , sResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				ids = m_pResultDM->GetCellData( _T("결과물") , 0 , sTmpResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				sResult.Format( _T("%d") , _ttoi(sTmpResult) + _ttoi(sResult) );
				
				ids = m_pXPriceSumDM->SetCellData( _T("중복삭제_합계") , sResult , k );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				pTotalProgress->SetPos( (i+1)*(k+1)+(saTableName.GetSize()*m_pRealResultDM->RefList.GetCount()) );
			}	
		}
		sRegNoList.Empty();
	}

	ids = SetDataMgrRef( m_pResultDM , _T("COUNT(*)") , _T("0") , _T("Y") , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );
	
	return 0;

	EFS_END
	return -1;

}

// 세로 합계 구성
INT CStaticsInfoMgr::YPriceSum3( CString sStaticsClass , CStringArray &saTableName , CStringArray &saXFieldName , CStringArray &saYFieldName , CProgressCtrl *pTotalProgress , CESL_DataMgr *pRegNoDM )
{
	EFS_BEGIN

	INT ids;
	// 세로 합계 구성
	pTotalProgress->SetRange32( 0 , saTableName.GetSize()*(m_pRealResultDM->RefList.GetCount()+m_pRealResultDM->GetRowCount()) );

	m_pYPriceSumDM->FreeData();

	CString sRegNoList;
	CString sRegNo;
	bool IsSend;
	//=====================================================
	//2008.11.24 UPDATE BY PJW : 등록번호갯수 제한을 1000개 이하로 바꾼다.
	// 	INT nLimitCnt = 50;
    INT nLimitCnt = 999;
	//=====================================================
	for( INT n = 0 ; n < pRegNoDM->GetRowCount() ; n++ )
	{
		IsSend = FALSE;

		ids = pRegNoDM->GetCellData( _T("등록번호") , n , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

		if( !sRegNoList.IsEmpty() )
			if( !sRegNo.IsEmpty() )
				sRegNoList += _T(",");
		if( !sRegNo.IsEmpty() )
			sRegNoList += _T("'") + sRegNo + _T("'");

		if( (n!=0) && (n%nLimitCnt==0) ) IsSend = TRUE;
		if( n == pRegNoDM->GetRowCount()-1 ) IsSend = TRUE;

		if( !IsSend ) continue;

		if( sRegNoList.IsEmpty() ) continue;

		for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
		{
			// DM 테이블 이름 추가
			m_pResultDM->TBL_NAME = saTableName.GetAt(i);
			
			CStringArray saTableAlias;

			ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

			// 3.3. WHERE절 구성 
			CString sWhere,sSendWhere;
			CString sTmpResult , sResult;
			CString sXData , sTmp;
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				sWhere.Empty();
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

				// SELECT 필드 설정
				ids = SetDataMgrRef( m_pResultDM , saXFieldName.GetAt(i) , _T("0") , _T("N") , 0 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
				
				ids = SetDataMgrRef( m_pResultDM , saYFieldName.GetAt(i) , _T("0") , _T("N") , 1 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );

				ids = SetDataMgrRef( m_pResultDM , _T("SUM(PRICE)") , _T("0") , _T("Y") , 2 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );

				// WHERE절 생성
				ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , saXFieldName.GetAt(i) , _T("") , sXData , _T("") , _T("N") , _T("N") , i , sWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeTotalCount") );

				sSendWhere = _T(" B.REG_NO IN (") + sRegNoList + _T(")");
				if( !sWhere.IsEmpty() ) 
					sSendWhere += _T(" AND ") + sWhere ;

				ids = m_pResultDM->RefreshDataManager( sSendWhere );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

				// RESULT DM -> REAL RESULT DM 으로 이동
				// RESULT DM -> REAL RESULT DM 으로 이동
				if( m_pYPriceSumDM->GetRowCount() < j )
					m_pYPriceSumDM->InsertRow(-1);

				ids = m_pYPriceSumDM->GetCellData( _T("중복삭제_합계") , j-1 , sResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				ids = m_pResultDM->GetCellData( _T("결과물") , 0 , sTmpResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				sResult.Format( _T("%d") , _ttoi(sTmpResult) + _ttoi(sResult) );
				
				ids = m_pYPriceSumDM->SetCellData( _T("중복삭제_합계") , sResult , j-1 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
				
				pTotalProgress->SetPos( (i+1)*(j+1) );

			}
		}
		sRegNoList.Empty();
	}

	ids = SetDataMgrRef( m_pResultDM , _T("COUNT(*)") , _T("0") , _T("Y") , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );

	return 0;

	EFS_END
	return -1;

}

// 기본조건과 사용자 조건 +
INT CStaticsInfoMgr::GetDefaultCond( CString sStaticsClass ,CString &sDefaultCond ,CString &sUserCond  ,CString &sWhere )
{
	EFS_BEGIN

	INT ids;

	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("기본조건") , sDefaultCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetDefaultCond") );
	
	sWhere = sDefaultCond;
	
	// 사용자조건 검색
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("사용자검색조건") , sUserCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("GetDefaultCond") );
	
	sDefaultCond.TrimLeft(); sDefaultCond.TrimRight(); sUserCond.TrimLeft(); sUserCond.TrimRight();

	// 사용자 검색 조건을 조건으로 변경한다.
	// 테이블 이름 구성하기
	CString sTableName;
	CString sInWhere,sOutWhere;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("검색테이블이름") , sTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetDefaultCond") );
	ids = GetBoSearchWhere( sStaticsClass , sTableName , sInWhere , sOutWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("GetDefaultCond") );

	sWhere.Replace( _T("%안조건%") , sInWhere );	
	sWhere.Replace( _T("%밖조건%") , sOutWhere );
	
	return 0;

	EFS_END
	return -1;

}



// 조건을 구성하여 값을 Real DM에 집에 넣는다.
INT CStaticsInfoMgr::FillStaticsValue( CString sXFieldName, CString sYFieldName, CString sXData , CString sYData , CString sXGroupYN , CString sYGroupYN , CString sWhere , /*CString sUserCond ,*/ CProgressCtrl *pProgressCtrl )
{
	EFS_BEGIN

	OutputDebugString( _T("FillStaticsValue : ") +  sXFieldName + _T(" : ") + sYFieldName + _T(" : ") + sXData + _T(" : ") + sYData + _T(" \n"));

	INT ids;
	CString sInStaticWhere,sOutStaticWhere;	// 그룹땜시 들어가는 조건 : %안그룹조건% , %밖그룹조건%
	CString sSendWhere;							// 보낼 SQL문
	CString sGroupSql;							// Group By SQL문
	CString sTmpXData,sTmpYData;				
	CString sTmpXFieldName , sTmpYFieldName;

	CString sTableName = m_pResultDM->TBL_NAME;

	CStringArray saTableAlias;

	ids = MakeTableAliasArray( sTableName , saTableAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );


	if( sXGroupYN.Compare(_T("Y"))== 0 )
	{
		sTmpXData = _T("");
		sTmpXFieldName = _T("");
	}
	else
	{
		sTmpXData = sXData;
		sTmpXFieldName = sXFieldName;
	}

	if( sYGroupYN.Compare(_T("Y"))== 0 )
	{
		sTmpYData = _T("");
		sTmpYFieldName = _T("");
	}
	else
	{
		sTmpYData = sYData;
		sTmpYFieldName = sYFieldName;
	}


	// SELECT 필드 설정
	ids = SetDataMgrRef( m_pResultDM , sXFieldName , sTmpXData , sXGroupYN ,  0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , sTmpYData , sYGroupYN , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("RunStatics") );

	// WHERE 조건 설정
	sSendWhere = sWhere;

	/*
	ids = MakeStaticSql( saTableAlias ,sTmpXFieldName ,  sTmpXData , sTmpYFieldName , sTmpYData , sInStaticWhere , sOutStaticWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

	if( sUserCond.GetLength() > 6&& !sInStaticWhere.IsEmpty() ) 
		sInStaticWhere = CString( _T(" AND ")  + sInStaticWhere );
	if( sUserCond.GetLength() > 6&& !sOutStaticWhere.IsEmpty() ) 
		sOutStaticWhere = CString( _T(" AND ")  + sOutStaticWhere );

	sSendWhere.Replace( _T("%안그룹조건%") , sInStaticWhere );
	sSendWhere.Replace( _T("%밖그룹조건%") , sOutStaticWhere );
	*/

	// GROUP BY 설정
	if( sXGroupYN.Compare(_T("Y"))== 0 )
		sGroupSql += _T(" ") + sXFieldName + _T(" ");
	if( sYGroupYN.Compare(_T("Y"))== 0 )
	{
		if( !sGroupSql.IsEmpty() ) sGroupSql += _T(" , ");
		sGroupSql += _T(" ") + sYFieldName + _T(" ");
	}
	if( !sGroupSql.IsEmpty() ) sGroupSql = (_T(" GROUP BY ") + sGroupSql);

	sSendWhere += sGroupSql;

	// RESULT DM -> REAL RESULT DM 으로 이동
	ids = GetDBAndMakeRealResultDM( sSendWhere , pProgressCtrl );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunStatics") );

	return 0;

	EFS_END
	return -1;

}


INT CStaticsInfoMgr::MakeTableAliasArray( CString sTableName , CStringArray &saTableAlias )
{
	EFS_BEGIN

	INT ids;

	CStringArray saTableArray;
	CStringArray saTempArray;
	
	ids = CLocCommonAPI::MakeArrayList( sTableName , _T(",") , saTableArray );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTableAliasArray") );

	for( INT i = 0 ; i < saTableArray.GetSize() ; i++ )
	{
		ids = CLocCommonAPI::MakeArrayList( saTableArray.GetAt(i) , _T(" ") , saTempArray );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTableAliasArray") );

		if( saTempArray.GetSize() == 2 )
			saTableAlias.Add( saTempArray.GetAt(1) );
	}

	return 0;

	EFS_END
	return -1;

}

// 테이블 이름속에 있으면 return 1 없으면 return 0
// CO_TEST A , CD_TEST_B B 
// -> CO_TEST A , CO_TEST_B B 로 나눈다.
// -> A
INT CStaticsInfoMgr::IsFieldFromTable( CString sFieldName , CStringArray &saTableArray )
{
	EFS_BEGIN

	INT ids;

	CStringArray saOneTableArray;
	CStringArray saOneTableArray2;
	CStringArray saFieldArray;
	bool IsIn = FALSE;

	// 필드 이름에 테이블 정보가 있지 않다면 항상 밖조건이 된다.
	if( sFieldName.Find(_T(".")) < 0 )
		return 1;

	INT nRemoveIndex = sFieldName.Find(_T("("));

	if( nRemoveIndex > 0 )
		sFieldName = sFieldName.Mid( nRemoveIndex+1 , sFieldName.GetLength() - nRemoveIndex );

	sFieldName.TrimLeft(); sFieldName.TrimRight();

	ids = CLocCommonAPI::MakeArrayList( sFieldName , _T(".") , saFieldArray );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTableAliasArray") );


	INT nIndex;
	CString sFieldTbl;
	CString sOutTbl;
	for( INT i = 0 ; i < saTableArray.GetSize() ; i++ )
	{
		saOneTableArray.RemoveAll();
		CLocCommonAPI::MakeArrayList( saTableArray.GetAt(i) , _T(",") , saOneTableArray ); 



		for( INT j = 0 ; j < saOneTableArray.GetSize() ; j++ )
		{
			saOneTableArray2.RemoveAll();
			CLocCommonAPI::MakeArrayList( saOneTableArray.GetAt(j) , _T(" ") , saOneTableArray2 );

			nIndex = saOneTableArray2.GetSize()-1;

			sOutTbl = saOneTableArray2.GetAt(nIndex); sOutTbl.TrimLeft(); sOutTbl.TrimRight();
			sFieldTbl = saFieldArray.GetAt(0); sFieldTbl.TrimLeft(); sFieldTbl.TrimRight();

			if( sFieldTbl.Find(_T("(")) >= 0 )
			{
				CStringArray saRealFieldTablAlias;
				CLocCommonAPI::MakeArrayList( sFieldTbl , _T("(") , saRealFieldTablAlias );
				sFieldTbl = saRealFieldTablAlias.GetAt(saRealFieldTablAlias.GetSize()-1 );
			}
			
			// 2007.08.10 update by pdw
			
			if( sOutTbl.Compare(sFieldTbl) == 0 )
				IsIn = TRUE;
			
//			if( sFieldTbl.Compare(sOutTbl) >= 0 )
//				IsIn = TRUE;
		}
	}

	if( IsIn ) return 1;
	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::DeBugDM()
{
	EFS_BEGIN

	OnShowDMViewer(0);
	return 0;

	EFS_END
	return -1;

}


// 순위 DM 초기화 하기
INT CStaticsInfoMgr::MakeOrderStaticsDM( CString sStaticsClass , CProgressCtrl *pProgressCtrl,CProgressCtrl *pTotalProgressCtrl )
{
	EFS_BEGIN

	INT ids;

	CString sXAlias , sYAlias , sOption , sOrder;

	ids = GetXYAlias( sStaticsClass , sXAlias , sYAlias , sOption , sOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStaticsDM") );

	// DM의 갯수는 단위의 갯수에 따라 달라진다.
	CStringArray asColumnName;
	if( sOption.Compare(_T("전체")) ==  0 )
	{
		asColumnName.Add( sOption );
	}
	else
	{
		ids = GetColumnNames( sStaticsClass , sOption , asColumnName , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeOrderStaticsDM") );
	}

	m_nOrderStaticsDMCnt = asColumnName.GetSize();

	if( m_pOrderStaticsDM != NULL ) 
		delete[] m_pOrderStaticsDM;
	m_pOrderStaticsDM = NULL;

	// 테이블 이름 구성하기
	CStringArray saTableName;
	CString sTableName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("검색테이블이름") , sTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );

	ids = CLocCommonAPI::MakeArrayList( sTableName , _T("/") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitRunStatics") );

	m_pOrderStaticsDM = new CESL_DataMgr*[saTableName.GetSize()+1];

	for( INT i = 0 ; i < saTableName.GetSize()+1 ; i++ )
	{
		m_pOrderStaticsDM[i] = new CESL_DataMgr[m_nOrderStaticsDMCnt];
	}

	// DB에 보내서 값 가져오기
	// 18.09.27 JMJ KOL.RED.2018.008
	if( _T("최대대출자료") != sStaticsClass && _T("최대대출자료(책두레포함)") != sStaticsClass )
	{
		ids = MakeOrderStatics( sStaticsClass , sXAlias , sOption , asColumnName , pProgressCtrl,pTotalProgressCtrl );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeOrderStaticsDM") );
		if( ids > 0 ) return ids;
	}
	else
	{
		ids = MakeBestLoanStatics( sStaticsClass , sXAlias , sOption , asColumnName , pProgressCtrl,pTotalProgressCtrl );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeOrderStaticsDM") );
		if( ids > 0 ) return ids;
	}

	// 정렬하고 순위 메기기
	ids = MakeSumOrderStatics( sStaticsClass , saTableName.GetSize() );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeOrderStaticsDM") );

	return 0;

	EFS_END
	return -1;

}

// 순위 DM 컬럼과 테이블 이름 조건 추가하기 , DB에 보내서 실제 통계 가져오기
INT CStaticsInfoMgr::MakeOrderStatics( CString sStaticsClass , CString sXAlias , CString sOption , CStringArray &asColumnName , CProgressCtrl *pProgressCtrl,CProgressCtrl *pTotalProgressCtrl)
{
	EFS_BEGIN

	INT ids;

	CString sTableName;	// 테이블 이름
	CString sWhere,sDefaultCond,sUserCond;		// 기본 조건
	CStringArray saFieldAliasList;
	CStringArray asFieldAlias;	// 필드 알리아스 
	CStringArray saTableName;
	CStringArray saFieldNameList;
	CStringArray saTableAlias;
	CString sResultField;	//결과물
	CString sResultField2;
	CStringArray saWhere;
	CString sOrderBy;	// 순위

	CString sTmpTableName;	// 가상 테이블 ( SELECT COUNT(*) , ... FROM ... WHERE .. GROUP BY .. ORDER BY.. 가 포함 되어 잇는 )

	CESL_DataMgr *pDM;

	

	// 조건절 구성하기
	// 기본 조건에 사용자 조건만 들어가면 된다.
	ids = GetDefaultCond( sStaticsClass , sDefaultCond , sUserCond , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );

	if( sUserCond.IsEmpty() )
	{
		AfxMessageBox( _T("조건 설정을 먼저 하시고 통계를 진행하십시오.") );
		return 101;
	}
	CLocCommonAPI::MakeArrayList( sWhere , _T(";") , saWhere );

	// 필드 알리아스 구성
	ids = CLocCommonAPI::MakeArrayList( sXAlias , _T("/") , saFieldAliasList );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );


	// 테이블 이름 구성하기
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("검색테이블이름") , sTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );
	ids = CLocCommonAPI::MakeArrayList( sTableName , _T("/") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitRunStatics") );


	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물") , sResultField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeOrderStatics") );
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물2") , sResultField2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeOrderStatics") );
	
	// 2007.09.03 add by pdw
	// 최대대출자료 연령별 지원을 위해
	CString strOptionBuf = sOption;

	ids = GetCellData( m_pColumnAliasInfoDM , _T("알리아스명") , sOption , _T("검색필드명") , sOption );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetFieldName") );	

	CString sOrderNumber;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("순위여부") , sOrderNumber );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );

	//++2008.09.02 DEL BY LKS {{++
//REMOVE▶ 	if( m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000 )
//REMOVE▶ 		sOrderBy.Format( _T(" LIMIT 0,%s ") , sOrderNumber );
	//--2008.09.02 DEL BY LKS --}}
//	else
		sOrderBy.Format( _T(" ROWNUM <= %s ") , sOrderNumber );

	CString sFieldName , sGroupYN;

	CString sInWhere;
	sUserCond.Empty();
	ids = GetBoSearchWhere( sStaticsClass , saTableName.GetAt(0) , sUserCond , sInWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

	sUserCond += sInWhere;

	INT nIndex;
	pTotalProgressCtrl->SetRange32( 0 , saTableName.GetSize() );
	for( INT k = 0 ; k < saTableName.GetSize() ; k++ )
	{
		pDM = &m_pOrderStaticsDM[k][0];

		ids = MakeTableAliasArray( saTableName.GetAt(k) , saTableAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

		

		if( saFieldAliasList.GetSize()-1 < k ) nIndex = 0;
		else nIndex = k;
		ids = CLocCommonAPI::MakeArrayList( saFieldAliasList.GetAt(nIndex) , _T(",") , asFieldAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );
		INT nSumIndex = 0;
		CString strSumAlais = _T("");
		for( INT i = 0 ; i < m_nOrderStaticsDMCnt ; i++ )
		{
			sTmpTableName = _T(" SELECT ");

			// DB CONNECTION 초기화
			pDM->SetCONNECTION_INFO(CLocCommonAPI::GetDBConnection(m_pInfo->MODE));


			// 컬럼 구성하기
			pDM->InitDataMgrRef( asFieldAlias.GetSize()+1 );

			//++2008.09.02 DEL BY LKS {{++
//REMOVE▶ 			if( m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000 )
//REMOVE▶ 				pDM->SetDataMgrRef( 0 , _T("순위") , _T("") , _T("") , _T("") , _T("udf") );
			//--2008.09.02 DEL BY LKS --}}
			//else
				pDM->SetDataMgrRef( 0 , _T("순위") , _T("ROWNUM") , _T("") , _T("") , _T("") );

			CString sGroupBy;
			for( INT j = 0 ; j < asFieldAlias.GetSize() ; j++ )
			{
				ids = GetFieldName( sStaticsClass , asFieldAlias.GetAt(j) , sFieldName , sGroupYN );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );

				ids = CLocCommonAPI::MakeArrayList( sFieldName , _T("/") , saFieldNameList );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );

				if( saFieldNameList.GetSize()-1 < k ) nIndex = 0;
				else nIndex = k;

				sFieldName = saFieldNameList.GetAt(nIndex);
				if( sFieldName.Find(_T("사용자조건")) > -1 )
				{
					CString sWhere2;					
					sWhere2 = sUserCond;
					if( sWhere2.Mid(0,4).Compare( _T(" AND") ) != 0 )
						sWhere2 = _T(" AND ") + sWhere2;

					if( sOption.Compare(_T("전체")) != 0 )
					{
						if( IsFieldFromTable( sOption , saTableAlias ) == 0 )
						{
							if( !asColumnName.GetAt(i).IsEmpty() )
								sWhere2 += _T(" AND ") + sOption + _T("= '") + asColumnName.GetAt(i) + _T("' ") ;
							else
								sWhere2 += _T(" AND ") + sOption + _T(" IS NULL ") ;
						}
					}

					sFieldName.Replace( _T("%사용자조건%") , sWhere2 );
				}


				// SELECT해올 필드 구성
				if( j !=0 )
				{
					sTmpTableName += _T(",");
				}

				if( sFieldName.Find(_T("COUNT(")) >= 0 )
				{
					sTmpTableName += sFieldName + _T(" AS CNT");
					pDM->SetDataMgrRef( j+1 , asFieldAlias.GetAt(j) , _T("CNT") , _T("") , _T("") , _T("") );	
				}
				else
				{			
					CString sTmpFieldName;
					if( sFieldName.Find(_T(".")) < 0 )
						sFieldName = _T("CLU.") + sFieldName;

					if( sFieldName.Find(_T(".")) > -1 && (sFieldName.Find(_T(",")) > -1) )
						sTmpFieldName = sFieldName.Mid( sFieldName.Find(_T("."))+1 , sFieldName.Find(_T(",")) - sFieldName.Find(_T(".")) - 1 );
					else if( sFieldName.Find(_T(".")) > -1 )
					{
						// 2005.07.26
						// 펑션을 이용해도 token이 되도록 변경
						CString strTmp;
						strTmp = sFieldName;
						strTmp.Replace(_T("CLFI."), _T(""));
						strTmp.Replace(_T("CL."), _T(""));
						strTmp.Replace(_T("CLU."), _T(""));
						sTmpFieldName = strTmp;

						// 2005.07.26 REM BY PDJ
						//sTmpFieldName = sFieldName.Mid( sFieldName.Find(_T("."))+1 );
					}

					if ( sFieldName.Find(_T("$"),0) < 0 )
					{
						// WHERE절 구성
						sTmpTableName += _T("MAX(") + sFieldName + _T(") AS ") + sTmpFieldName;

						// 2007.07.10 ADD BY PDW
						// 최대대출자료 쿼리 튜닝 관련 수정작업
						// 18.09.27 JMJ KOL.RED.2018.008
						if( _T("최대대출자료") == sStaticsClass || _T("최대대출자료(책두레포함)") == sStaticsClass )
						{
							sTmpTableName = _T("SELECT MAX(CL.REC_KEY) AS REC_KEY ");
						}
					}
					else
					{
						// 2005.07.26 ADD BY PDJ
						// WHERE절의 펑션빼내기
						CString strFieldName, strFieldAlias;
						INT		nStartPos, nEndPos;
						
						nStartPos = sFieldName.Find(_T("("), 0 );	
						nEndPos = sFieldName.Find(_T(")"), 0 );							
						strFieldName = sFieldName.Mid(nStartPos+1, nEndPos-nStartPos-1);
						strFieldAlias =  strFieldName.Mid( strFieldName.Find(_T("."))+1 );
						sTmpTableName += _T("MAX(") + strFieldName + _T(") AS ") + strFieldAlias;
					}
					// DM 구성
					// 2005.07.30 ADD BY PDJ 
					// Function을 이용했을 경우 Column Alias의 이름 추출
					if ( asFieldAlias.GetAt(j).FindOneOf(_T("$")) > -1 )
					{
						CString strFieldAlias;
						MakeFunctionFieldName(asFieldAlias.GetAt(j), strFieldAlias);
						pDM->SetDataMgrRef( j+1 , strFieldAlias , sTmpFieldName , _T("") , _T("") , _T("") );
					}
					else
					{
						pDM->SetDataMgrRef( j+1 , asFieldAlias.GetAt(j) , sTmpFieldName , _T("") , _T("") , _T("") );
					}
				}
			}
			
			CString sSendWhere = saWhere.GetAt(k);
			if( IsFieldFromTable( sOption , saTableAlias ) == 1)
			{
				if( sOption.Compare(_T("전체")) != 0 )
				{
					if( sOption.Find(_T(".")) < 0 )
						sOption = _T("CLU.") + sOption;

					if( !sWhere.IsEmpty() ) sSendWhere += _T(" AND ");
						if( !asColumnName.GetAt(i).IsEmpty() )
							sSendWhere += sOption + _T("= '") + asColumnName.GetAt(i) + _T("' ") ;
						else
							sSendWhere += sOption + _T(" IS NULL ") ;
				}
			}

			// 2007.09.03 add by pdw
			// 최대대출자료 연령별 지원을 위해
			// 18.09.27 JMJ KOL.RED.2018.008
			//if(_T("최대대출자료") == sStaticsClass && _T("연령별") == strOptionBuf )
			if((_T("최대대출자료") == sStaticsClass && _T("연령별") == strOptionBuf) || (_T("최대대출자료(책두레포함)") == sStaticsClass && _T("연령별") == strOptionBuf) )
			{
				if( !sWhere.IsEmpty() ) sSendWhere += _T(" AND ");
						
				if( !asColumnName.GetAt(i).IsEmpty() )
				{
					sSendWhere += _T(" (SELECT ") + sOption + _T(" FROM CO_LOAN_USER_TBL CLU WHERE CLU.REC_KEY=CL.USER_KEY) = '") + asColumnName.GetAt(i) + _T("' ") ;
				}
				else
				{					
					sSendWhere += _T(" (SELECT ") + sOption + _T(" FROM CO_LOAN_USER_TBL CLU WHERE CLU.REC_KEY=CL.USER_KEY) IS NULL ") ;
				}
			}

			// 이거만 where 절에 넣으면 됨
			pDM->QueryLog( sSendWhere.GetBuffer( 0 ) );
			
			////////////////////////////////////////////////////////////////////////////////////////////////////
			// 병걸씨 미안해요. 병걸씨 만든거 이해를 못해서 요거만 그냥 요렇게 만들께요.
			// 병걸씨 통계가 너무 어려워서... -,.-;;....
			// 김용배 수정
			// 2004-04-09
			////////////////////////////////////////////////////////////////////////////////////////////////////
			// (2004-0915_M_HAN...)왜 이렇게 갔는지 모르겠음. 이 부분 때문에 '최대대출자료'의 순위가 
			// 항상 100위까지 나와 민원이 들어옴. 이 부분을 지우고, 이전 버전으로 원상복귀가 된 것 같음
			////////////////////////////////////////////////////////////////////////////////////////////////////
			//if( _T("최대대출자료") == sStaticsClass && !( m_pInfo->MODE == 10000 || (m_pInfo->MODE == 30000) ) )
			//{
			//	CString query;
			//	query.Format(
			//				_T("SELECT /*+ ORDERED USE_NL( V, C ) INDEX( C CO_LOAN_TBL_PK ) */ ")
			//				_T("ROWNUM, C.REG_NO, C.TITLE, C.AUTHOR, C.PUBLISHER, SUBSTR( C.CALL_NO, 1, 20 ), V.CNT ")
			//				_T("FROM ( ")
			//				_T("	 SELECT CL.BOOK_KEY AS BOOKKEY, MAX( CL.REC_KEY ) AS CLKEY, COUNT( * ) AS CNT FROM CO_LOAN_USER_TBL CLU, LH_STORAGE_V01 CL ")
			//				_T("	 WHERE ")
			//				_T(" %s ")
			//				_T("	 GROUP BY CL.BOOK_KEY ORDER BY CNT DESC ) V,")
			//				_T("	 LH_STORAGE_V01 C ")
			//				_T("WHERE C.REC_KEY = V.CLKEY AND ROWNUM <= 100 "),
			//				sSendWhere );
			//
			//	pDM->RestructDataManager( query, pProgressCtrl );
			//}
			//else
			////////////////////////////////////////////////////////////////////////////////////////////////////
			if( m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000 )
			{
				CString sOrder;
				for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
				{
					sOrder.Format(_T("%d") , i+1 );
					pDM->SetCellData(_T("순위") , sOrder , i );
				}
			}

			if ( !(m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000) )
			{				
				// Group By 삽입
				sSendWhere += _T(" GROUP BY ") + sResultField2;
				
				// Order By 삽입
				// 2007.07.10 update by pdw 
				// 최대대출자료 통계 튜닝 관련 order by 변경
				// 18.09.27 JMJ KOL.RED.2018.008
				//if ( _T("최대대출자료") == sStaticsClass ) //등록번호로 정렬 추가_050420	
				if ( _T("최대대출자료") == sStaticsClass || _T("최대대출자료(책두레포함)") == sStaticsClass ) 
					sSendWhere += _T(" ORDER BY CNT DESC");
				else if ( _T("다독자") == sStaticsClass ) //사용자번호로 정렬 추가_050420	
					sSendWhere += _T(" ORDER BY CNT DESC, USER_NO");
				else 
					sSendWhere += _T(" ORDER BY CNT DESC");
				
				sTmpTableName += _T(" FROM ") + saTableName.GetAt(k) + _T(" WHERE ") + sSendWhere;
				// 2007.07.10 ADD BY PDW
				// 최대대출자료 쿼리 튜닝 관련 수정작업
				// 18.09.27 JMJ KOL.RED.2018.008
				if( _T("최대대출자료") != sStaticsClass && _T("최대대출자료(책두레포함)") != sStaticsClass )
				{
					sTmpTableName = _T("(") + sTmpTableName +_T(") V");
				}
				
				// 2007.07.10 ADD BY PDW
				// 최대대출자료 쿼리 튜닝 관련 수정작업
				// 18.09.27 JMJ KOL.RED.2018.008
				if( _T("최대대출자료") == sStaticsClass || _T("최대대출자료(책두레포함)") == sStaticsClass )
				{
					// 2007.08.20 ADD BY PDW
					// 쿼리 튜닝
					// ( SELECT * FROM  ( K) WHERE ROWNUM <= 10 ) V
					sTmpTableName = _T("( SELECT * FROM ( ") + sTmpTableName + _T(" ) WHERE ") + sOrderBy + _T(" ) V ");
//					sOrderBy = _T(" CL2.REC_KEY=V.REC_KEY ");
					sTmpTableName = _T("LH_STORAGE_V01 CL2,") + sTmpTableName;
				}
				
				// 테이블 구성하기
				pDM->TBL_NAME = sTmpTableName;
				
				if( m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000  )
				{
					// 18.09.27 JMJ KOL.RED.2018.008
					if( _T("최대대출자료") == sStaticsClass || _T("최대대출자료(책두레포함)") == sStaticsClass )
					{ 
						pDM->EXTRA_CONDITION = _T(" CL2.REC_KEY=V.REC_KEY ");
					}
					else
					{
						pDM->EXTRA_CONDITION = sOrderBy;
					}
				}
				else
				{
					// 18.09.27 JMJ KOL.RED.2018.008
					if( _T("최대대출자료") == sStaticsClass || _T("최대대출자료(책두레포함)") == sStaticsClass )
					{
						pDM->CONDITION = _T(" CL2.REC_KEY=V.REC_KEY ");
					}
					else
					{
						pDM->CONDITION = sOrderBy;
					}					
				}
				
				// DB에 보내기
				ids = pDM->RefreshDataManager( _T("") , pProgressCtrl);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitOrderStaticsDM") );
								
				// 2011.09.09 ADD : 3000Api
				if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pDM->GetRowCount())
				{
					BOOL bCheckConnect = FALSE;
					CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
					if(NULL != pApi)
					{
						if(TRUE == pApi->CheckConnection())
						{
							INT nC, nR, nCIdx, nRIdx;
							CString strColumnAlias;
							CString strFieldAlias, strFieldName, strFieldType, strFieldValue, strDataType;
							nR = pDM->GetRowCount();
							nC = pDM->GetColCount();						
							for(nCIdx = 0; nCIdx < nC; nCIdx++)
							{
								pDM->GetColumnRefInfo(nCIdx, strFieldAlias, strFieldName, strFieldType, strFieldValue, strDataType);
								if( (sStaticsClass == _T("다독자") || sStaticsClass == _T("연체자통계")) &&	(strFieldName == _T("H_ADDR1") || strFieldName == _T("H_PHONE") || strFieldName == _T("HANDPHONE")) )
								{
									CStringArray DataArray;
									for(nRIdx = 0; nRIdx < nR; nRIdx++)
									{
										pDM->GetCellData(nRIdx, nCIdx, strFieldValue);
										DataArray.Add(strFieldValue);
									}
									if(TRUE == pApi->ConnectSocket())
									{
										if(TRUE == pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), strFieldName, DataArray))
										{
											for(nRIdx = 0; nRIdx < nR; nRIdx++)
											{
												pDM->SetCellData(nRIdx, nCIdx, DataArray.GetAt(nRIdx));
											}
										}
										pApi->CloseSocket();
									}
									DataArray.RemoveAll(); DataArray.FreeExtra();
								}
							}
						}
						delete pApi;
						pApi = NULL;
					}
				} // END IF Lib-Secu
			}

			pDM->EXTRA_CONDITION = _T("");
			pDM->CONDITION = _T("");

			// 코드 -> 설명
// 			m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("대출소속정보") , _T("소속") );
			// 2009.10.12 ADD BY PWR : 대출자번호 주민번호일때 *표시
			for(INT k = 0; k < pDM->GetRowCount(); k++)
			{
				// 코드->설명
				CString strCode;
				pDM->GetCellData(_T("소속"), k, strCode);
				m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("대출소속정보"), strCode, strCode);
				pDM->SetCellData(_T("소속"), strCode, k);
				// 대출자번호 주민번호일때 *표시
				CString strUserNo;
				pDM->GetCellData(_T("대출자번호"), k, strUserNo);
				if(_T("Y") != m_sIsCivilNoHide )
				{
					// 2010.09.08 ADD BY KSJ : 통합도서서비스 사용여부에 따라 주민번호 표시방법 변경
//DEL 					CString strBuf = strUserNo;
//DEL 					if(13 == strUserNo.GetLength() )
//DEL 					{
//DEL 						strBuf = strUserNo.Mid(0,6) + _T("-") + strUserNo.Mid(6);
//DEL 					}				
//DEL 					ids = CLocCommonAPI::CivilNoCheck(strBuf);
//DEL 					if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );
//DEL 					if(0 == ids)
//DEL 					{
//DEL 						// 2009.12.14 ADD BY CJY : 주민번호 표시방법 변경
//DEL 						strUserNo.Format(_T("%s%s******"),strBuf.Left(6),strBuf.Mid(7,1));
//DEL 						if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );
//DEL 					}
					CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_sIsUnityLoanService );
				}
				pDM->SetCellData(_T("대출자번호"), strUserNo, k);
			}

			if( i != m_nOrderStaticsDMCnt-1 ) pDM++;
		}
		pTotalProgressCtrl->SetPos( (k+1) );		
	}


	return 0;

	EFS_END
	return -1;

}

///{{++
/// 2008.04.08 add by pdw
/// 최대대출자료 통계는 별도로 쿼리를 생성한다
/// 복본자료를 같이 통계 내기 위해 BO_BOOK_TBL, SE_BOOK_TBL이 필요하다
/// 통계 형태가 레퍼런스로 지원할 수 없기 때문에 
/// 소스안에 고정 시켰다
/*
SELECT ROWNUM, REG_NO, TITLE, AUTHOR, PUBLISHER, CALL_NO, CNT  FROM ( SELECT *
           FROM ( SELECT MAX(CL.REG_NO) AS REG_NO,MAX(CL.TITLE) AS TITLE,MAX(CL.AUTHOR) AS AUTHOR,MAX(CL.PUBLISHER) AS PUBLISHER,MAX(CL.CALL_NO) AS CALL_NO,
                         COUNT(1) AS CNT
                    FROM LH_STORAGE_V01 CL,
                         BO_BOOK_TBL BB
                   WHERE CL.BOOK_KEY=BB.REC_KEY
                     AND CL.STATUS IN ('0','1','2','5','6')
                     AND CL.LOAN_KEY IS NULL
                     AND CL.CLASS_NO_TYPE='1'
                     AND CL.MANAGE_CODE='MA'
                     AND CL.PUBLISH_FORM_CODE='MO'
                     AND CL.LOAN_DATE BETWEEN TO_DATE('2008/01/07 00:00:00','YYYY/MM/DD HH24:MI:SS')
                                          AND TO_DATE('2008/04/07 00:00:00','YYYY/MM/DD HH24:MI:SS')
                   GROUP BY BB.SPECIES_KEY, BB.VOL_CODE
                  UNION 
                  SELECT MAX(CL.REG_NO) AS REG_NO,MAX(CL.TITLE) AS TITLE,MAX(CL.AUTHOR) AS AUTHOR,MAX(CL.PUBLISHER) AS PUBLISHER,MAX(CL.CALL_NO) AS CALL_NO,
                         COUNT(1) AS CNT
                    FROM LH_STORAGE_V01 CL,
                         SE_BOOK_TBL BB
                   WHERE CL.BOOK_KEY=BB.REC_KEY
                     AND CL.STATUS IN ('0','1','2','5','6')
                     AND CL.LOAN_KEY IS NULL
                     AND CL.CLASS_NO_TYPE='1'
                     AND CL.MANAGE_CODE='MA'
                     AND CL.PUBLISH_FORM_CODE='SE'
                     AND CL.LOAN_DATE BETWEEN TO_DATE('2008/01/07 00:00:00','YYYY/MM/DD HH24:MI:SS')
                                          AND TO_DATE('2008/04/07 00:00:00','YYYY/MM/DD HH24:MI:SS')
                   GROUP BY BB.SPECIES_KEY, BB.VOL_CODE
                ) ORDER BY CNT DESC ) WHERE ROWNUM <=10
*/
INT CStaticsInfoMgr::MakeBestLoanStatics( CString sStaticsClass , CString sXAlias , CString sOption , CStringArray &asColumnName , CProgressCtrl *pProgressCtrl,CProgressCtrl *pTotalProgressCtrl)
{
EFS_BEGIN

	INT		ids;

	///{{++	
	/// SELECT FIELD 생성
	/// 1. VIEW 테이블에 사용되는 SELECT FIELD를 만든다
	/// 2. 통계용 SELECT FIELD를 만든다	
	CStringArray	saFieldAlias;
	CStringArray    saFieldNames;
	INT				nFieldAliasCount;
	CString         strFieldName;
	CString			strFieldAlias;
	CString			strGroupYN;
	CString			strViewSelectField;
	CString			strStaticSelectField;	

	ids = CLocCommonAPI::MakeArrayList(sXAlias, _T(","), saFieldAlias);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("MakeBestLoanStatics") );
	}

	nFieldAliasCount = saFieldAlias.GetSize();

	for( INT i=0; i<nFieldAliasCount; i++ )
	{
		ids = GetFieldName(sStaticsClass, saFieldAlias.GetAt(i), strFieldName, strGroupYN);
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("MakeBestLoanStatics") );
		}

		/// COUNT(*)는 별도 처리 한다
		if( -1 < strFieldName.Find(_T("COUNT")) )
		{
			strFieldAlias = _T("CNT");			
		}
		else
		{
			strFieldAlias = strFieldName;			

			if( -1 < strFieldName.Find(_T(",")) )
			{
				strFieldAlias = strFieldAlias.Mid( strFieldAlias.Find(_T("."))+1 , strFieldAlias.Find(_T(",")) - strFieldAlias.Find(_T("."))-1 );
				strFieldName  = _T("CL.") + strFieldAlias;
			}
			else
			{
				strFieldAlias.Replace(_T("CL."),_T(""));
			}

			strFieldName = _T("MAX(") + strFieldName + _T(")");			
		}

		if( TRUE != strViewSelectField.IsEmpty() )
		{
			strViewSelectField += _T(",");
		}
		
		/// MAX 함수를 붙여준 후 알리아스 처리를 한다
		strViewSelectField += ( strFieldName + _T(" AS ") + strFieldAlias );

		if( TRUE != strStaticSelectField.IsEmpty() )
		{
			strStaticSelectField += _T(",");
		}
		
		strStaticSelectField += strFieldAlias;

		saFieldNames.Add(strFieldAlias);
	}

	/// ROWNUM 추가
	strStaticSelectField = _T("ROWNUM,") + strStaticSelectField;
	///}}--

	///{{++
	/// WHERE 절을 생성한다
	CString	strWhere;
	CString	strDefaultCondition;
	CString	strUserCondition;
	CString	strTableName;

	/// 테이블 이름 구성하기
	ids = GetCellData(m_pStaticsMainInfoDM, _T("통계종류"), sStaticsClass, _T("검색테이블이름"), strTableName);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("MakeBestLoanStatics") );
	}

	/// 기본조건절 구성하기
	ids = GetDefaultCond(sStaticsClass, strDefaultCondition, strUserCondition, strWhere);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("MakeBestLoanStatics"));
	}

	if( TRUE == strUserCondition.IsEmpty() )
	{
		AfxMessageBox(_T("조건 설정을 먼저 하시고 통계를 진행하십시오."), MB_ICONINFORMATION);
		return 101;
	}
	///}}--

	///{{++
	/// GROUP BY 생성
	CString strResult;
	CString strGroupBy;

	ids = GetCellData(m_pStaticsMainInfoDM, _T("통계종류"), sStaticsClass, _T("결과물"), strResult);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("MakeBestLoanStatics") );
	}

	ids = GetCellData(m_pStaticsMainInfoDM, _T("통계종류"), sStaticsClass, _T("결과물2"), strGroupBy);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("MakeBestLoanStatics") );
	}
	///}}--

	///{{++
	/// ROWNUM 조건 생성
	CString strRowNum;
	
	ids = GetCellData(m_pStaticsMainInfoDM, _T("통계종류"), sStaticsClass, _T("순위여부"), strRowNum);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("MakeBestLoanStatics") );
	}

	strRowNum = _T(" ROWNUM <= ") + strRowNum;
	///}}--
	
	///{{++
	/// 통계 단위 설정
	CString strUnitFieldName;

	ids = GetCellData(m_pColumnAliasInfoDM, _T("알리아스명"), sOption, _T("검색필드명"), strUnitFieldName);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("MakeBestLoanStatics") );
	}

	/// 연령은 별도로 처리한다
	if( _T("연령별") == sOption )
	{		
		strUnitFieldName = _T("(SELECT ") + strUnitFieldName + _T(" FROM CO_LOAN_USER_TBL CLU WHERE CLU.REC_KEY=CL.USER_KEY)");
	}
	///}}--

	///{{++
	/// 단위에 따라 처리 한다
	CString strBoTable;
	CString strSeTable;
	CString strViewTable;
	CString strQuery;

	CESL_DataMgr* pDM = NULL;
	CString       strUnitCondition;

	pTotalProgressCtrl->SetRange32(0 , m_nOrderStaticsDMCnt);

	
	for( i=0 ; i<m_nOrderStaticsDMCnt ; i++ )
	{
		/// DM 설정
		pDM = &m_pOrderStaticsDM[0][i];
		pDM->SetCONNECTION_INFO(CLocCommonAPI::GetDBConnection(m_pInfo->MODE));

		/// DM 컬럼 구성
		pDM->InitDataMgrRef(nFieldAliasCount+1);

		pDM->SetDataMgrRef(0, _T("순위"), _T("ROWNUM"), _T(""), _T(""), _T(""));

		INT j;
		for( j=0 ; j<nFieldAliasCount ; j++ )
		{
			pDM->SetDataMgrRef(j+1, saFieldAlias.GetAt(j), saFieldNames.GetAt(j), _T(""), _T(""), _T(""));
		}


		if( _T("전체") != sOption )
		{
			if( TRUE == asColumnName.GetAt(i).IsEmpty() )
			{
				strUnitCondition = strUnitFieldName + _T(" IS NULL AND ");
			}
			else 
			{
				strUnitCondition = strUnitFieldName + _T("='") + asColumnName.GetAt(i) +_T("' AND ");
			}

			// 2009.08.07 UPDATE BY KSJ : 기본 Where 절에 AND 조건이 누적되어 조건이 형성 되는 문제로 인하여 DELETE
			//strWhere += (_T(" AND ") + strUnitCondition);
		}

		// 2009.08.07 UPDATE BY KSJ : 기본 Where 절에 AND 조건이 누적되어 조건이 형성 되는 문제로 인하여 DELETE
		//DEL 		strBoTable.Format(_T("SELECT %s FROM %s, BO_BOOK_TBL BB ")
		//DEL 			              _T("WHERE %s ")
		//DEL 						  _T("AND CL.PUBLISH_FORM_CODE='MO' ")
		//DEL 						  _T("GROUP BY %s "), strViewSelectField, strTableName, strWhere, strGroupBy);
		//DEL 
		//DEL 		strSeTable.Format(_T("SELECT %s FROM %s, SE_BOOK_TBL BB ")
		//DEL 			              _T("WHERE %s ")
		//DEL 						  _T("AND CL.PUBLISH_FORM_CODE='SE' ")
		//DEL 						  _T("GROUP BY %s "), strViewSelectField, strTableName, strWhere, strGroupBy);
		strBoTable.Format(_T("SELECT %s FROM %s, BO_BOOK_TBL BB ")
						  _T("WHERE %s ")
						  _T("AND %s")
						  _T("CL.PUBLISH_FORM_CODE='MO' ")
						  _T("GROUP BY %s "), strViewSelectField, strTableName, strWhere, strUnitCondition , strGroupBy);

		strSeTable.Format(_T("SELECT %s FROM %s, SE_BOOK_TBL BB ")
						  _T("WHERE %s ")
						  _T("AND %s")
						  _T("CL.PUBLISH_FORM_CODE='SE' ")
						  _T("GROUP BY %s "), strViewSelectField, strTableName, strWhere, strUnitCondition , strGroupBy);		

		strViewTable.Format(_T("SELECT * FROM (%s UNION %s) ORDER BY CNT DESC"), strBoTable, strSeTable);

		strQuery.Format(_T("SELECT %s FROM (%s) WHERE %s"), strStaticSelectField, strViewTable, strRowNum);

		// 2009.08.07 DELETE BY KSJ : Query 로그는 RestructDataManager 수행 하면서 기록하게됨. 중복 기록으로 인하여 DELETE
		/// 로그 생성
//DEL 		pDM->QueryLog(strQuery.GetBuffer(0));

		ids = pDM->RestructDataManager(strQuery);
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("MakeBestLoanStatics") );
		}

		pTotalProgressCtrl->SetPos(i);		
	}
	///}}--

	return 0;

EFS_END
return -1;
}
///}}--

INT CStaticsInfoMgr::ViewTab( CString sStaticsClass , CTabCtrl *pTab )
{
	EFS_BEGIN

	INT ids;


	CString sXAlias , sYAlias , sOption , sOrder;

	ids = GetXYAlias( sStaticsClass , sXAlias , sYAlias , sOption , sOrder );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ViewTab") );
	}

	// DM의 갯수는 단위의 갯수에 따라 달라진다.
	CStringArray asColumnName;
	if( 0 == sOption.Compare(_T("전체")) )
	{
		asColumnName.Add( sOption );
	}
	else
	{
		ids = GetColumnNames( sStaticsClass , sOption , asColumnName );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ViewTab") );
		}
	}

	pTab->DeleteAllItems();
	INT i;
	for( i = 0 ; i < asColumnName.GetSize() ; i++ )
	{
		pTab->InsertItem( i , asColumnName.GetAt(i) );
	}

	return 0;

	EFS_END
	return -1;

}

INT CStaticsInfoMgr::OrderStaticsView( CString sStaticsClass , CMSHFlexGrid *pGrid , INT nIndex )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM;

	CStringArray saTableName;
	CString sTableName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("검색테이블이름") , sTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );

	CString sOrderNumber;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("순위여부") , sOrderNumber );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );

	ids = CLocCommonAPI::MakeArrayList( sTableName , _T("/") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitRunStatics") );

	pDM = &m_pOrderStaticsDM[saTableName.GetSize()][nIndex];	

	// 그리드에 뿌려준다.
	pGrid->Clear();		

	if ( pDM->GetRowCount() == 0 ) 
	{
		pGrid->SetRows( 2 );
		pGrid->SetRedraw(TRUE);
		return 0;
	}
	
	INT nViewRowCnt = pDM->GetRowCount();
	if( nViewRowCnt > _ttoi(sOrderNumber) )
		nViewRowCnt = _ttoi(sOrderNumber);


	pGrid->SetRows( nViewRowCnt+1 );
	pGrid->SetCols( 0 , pDM->RefList.GetCount() );
	if( pDM->GetRowCount() < 2 ) 
		pGrid->SetRows( 2 );

	// X축 컬럼 뿌려주기
	INT refcount = pDM->RefList.GetCount();
	CESL_DataMgr::reference * pRef;
	POSITION pos;
	pos = pDM->RefList.GetHeadPosition();
	CString query, fields, isudf;


	CString sResult;
	CString strAlias;	

	for( INT i = 0 ; i < pDM->RefList.GetCount() ; i++ )
	{
		pRef = NULL;
		pRef = (CESL_DataMgr::reference*)pDM->RefList.GetNext(pos);

		strAlias = pRef->FIELD_ALIAS;
		if ( strAlias.Find(_T("$"), 0 ) > -1 )
		{	
			MakeFunctionFieldName(strAlias, strAlias);
		}
		
		pGrid->SetTextMatrix( 0 , i ,  strAlias );
		

		for( INT j = 0 ; j < nViewRowCnt ; j++ )
		{
			ids = pDM->GetCellData( pRef->FIELD_ALIAS , j , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );
			pGrid->SetTextMatrix( j+1 , i , sResult );
		}

	}

	pGrid->SetFixedCols(1);
	pGrid->SetFixedRows(1);

	
	// 그리드 왼쪽 정렬하기
	// 김용배 추가
	for( i = 0; i < pGrid->GetCols( 0 ); i++ )
		pGrid->SetColAlignment( i, 1 );
	
	pGrid->SetRedraw(TRUE);
	pGrid->Refresh();	

	return 0;

	EFS_END
	return -1;

}

// 여러개의 DM의 계를 합쳐서 정렬까지 하고 순위까지 매긴다.
INT CStaticsInfoMgr::MakeSumOrderStatics( CString sStaticsClass , INT nTableCnt )
{
	EFS_BEGIN

	INT ids;

	CString sResultAlias;
	// 계 합치기
	INT nTargetIndex = nTableCnt;
	
	// 구성
	CESL_DataMgr *pTargetDM;
	CESL_DataMgr *pSrcDM;
	CString sTargetData;
	CString sSrcData;
	CString sAlias;

	// 결과 알리아스
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물") , sResultAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeSumOrderStatics") );

	CESL_DataMgr *pTestDM;
	pTestDM = new CESL_DataMgr[11];

	// 컬럼 구성
	CString sFieldAlias,sFieldName,sFieldType,sInitValue,sDataType;
	for( INT i = 0 ; i < m_nOrderStaticsDMCnt ; i++ )
	{
		pTargetDM = &m_pOrderStaticsDM[nTargetIndex][i];

		pSrcDM = &m_pOrderStaticsDM[0][i];

		// DB CONNECTION 초기화
		pTargetDM->SetCONNECTION_INFO(CLocCommonAPI::GetDBConnection(m_pInfo->MODE));

		pTargetDM->InitDataMgrRef( pSrcDM->RefList.GetCount() );
		pTargetDM->m_nCols = pSrcDM->RefList.GetCount();

		POSITION pos;
		CESL_DataMgr::reference *Ref;
		for( INT k = 0 ; k < pSrcDM->RefList.GetCount() ; k++ )
		{
			pos = pSrcDM->RefList.FindIndex(k);
			Ref = (CESL_DataMgr::reference*)(pSrcDM->RefList.GetAt(pos));

			sFieldAlias = Ref->FIELD_ALIAS;
			sFieldType = _T("str");
			if( sFieldAlias.Compare( sResultAlias ) == 0 )
				sFieldType = _T("int");

			pTargetDM->SetDataMgrRef( k , sFieldAlias , _T("") , sFieldType  , _T("")  , sFieldType );			
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSumOrderStatics") );
		}		
	}

	// 데이타 복사
	for( i = 0 ; i < m_nOrderStaticsDMCnt ; i++ )
	{
		pTargetDM = &m_pOrderStaticsDM[nTargetIndex][i];

		for( INT j = 0 ; j < nTableCnt   ; j++ )
		{
			pSrcDM = &m_pOrderStaticsDM[j][i];

			for( INT k = 0 ; k < pSrcDM->RefList.GetCount() ; k++ )
			{
				CESL_DataMgr::reference *Ref;
				POSITION pos = pSrcDM->RefList.FindIndex(k);
				Ref = (CESL_DataMgr::reference*)(pSrcDM->RefList.GetAt(pos));
				sAlias = Ref->FIELD_ALIAS;
				for( INT m = 0 ; m < pSrcDM->GetRowCount() ; m++ )
				{
					if( pTargetDM->GetRowCount() < m+1 )
						pTargetDM->InsertRow(-1);

					ids = pSrcDM->GetCellData( sAlias , m , sSrcData );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSumOrderStatics") );

					ids = pTargetDM->GetCellData( sAlias , m , sTargetData );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSumOrderStatics") );

					if( sAlias.Compare(sResultAlias) == 0 )
						sTargetData.Format( _T("%d") , _ttoi(sTargetData)+_ttoi(sSrcData) );
					else
						sTargetData = sSrcData;

					sTargetData.Replace( _T("\r\n") , _T(" ") );
					sTargetData.Replace( _T("\n") , _T(" ") );

					ids = pTargetDM->SetCellData( sAlias , sTargetData , m );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeSumOrderStatics") );

				}
			}
		}	
	}

	return 0;

EFS_END
return -1;
}

// 열을 삭제한다.
INT CStaticsInfoMgr::GridDeleteRow( INT nIndex , CMSHFlexGrid *pGrid )
{
	EFS_BEGIN

	INT nCols = pGrid->GetCols(0);
	INT nRows = pGrid->GetRows();

	CString sData;

	for( INT j = nIndex ; j < nRows-1 ; j++ )
	{
		for( INT i = 0 ; i < nCols ; i++ )
		{
			sData = pGrid->GetTextMatrix( j+1 , i );
			pGrid->SetTextMatrix( j , i , sData );
		}
	}


	pGrid->SetRows( pGrid->GetRows()-1 );

	return 0;

	EFS_END
	return -1;

}

// 행을 삭제한다.
INT CStaticsInfoMgr::GridDeleteCol( INT nIndex , CMSHFlexGrid *pGrid )
{
	EFS_BEGIN

	INT nRows = pGrid->GetRows();
	INT nCols = pGrid->GetCols(0);

	CString sData;

	for( INT j = nIndex ; j < nCols-1 ; j++ )
	{
		for( INT i = 0 ; i < nRows ; i++ )
		{
			sData = pGrid->GetTextMatrix( i , j+1 );
			pGrid->SetTextMatrix( i , j  , sData );
		}
	}

	pGrid->SetCols( 0 , pGrid->GetCols(0)-1 );

	return 0;

	EFS_END
	return -1;

}

// 분류기호구분
INT CStaticsInfoMgr::ConvertClassNo(CString sStaticsClass ,  CString sCode,CString &sDesc , CString sClassNoType )
{
	EFS_BEGIN

 	INT ids;

	if( sClassNoType.IsEmpty() )
		return 0;
//		sClassNoType = _T("분류기호별");

	CStringArray asColumnCode;
	CStringArray asColumnName;
	ids = GetColumnNames( sStaticsClass , sClassNoType, asColumnCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("ConvertClassNo") );
	ids = GetColumnNames( sStaticsClass , sClassNoType , asColumnName , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("ConvertClassNo") );

	for( INT i = 0 ; i < asColumnName.GetSize() ; i++ )
	{
		if( asColumnCode.GetAt(i).Compare(sCode)==0 )
		{
			sDesc = asColumnName.GetAt(i);
			return 0;
		}
	}

	return 0;

	EFS_END
	return -1;

}

// ##############################################################################################################
// [ 함수설명 ]
//   설정파일이 있는 디렉토리 경로 리턴
//   서울시 통계에 대한 예외처리가 있음
//
// [ 미니스펙 ]                          
//
// ##############################################################################################################
VOID CStaticsInfoMgr::GetDir( CString &sDir )
{
EFS_BEGIN

	CString strMenuName = GetMenuName();

	// ==========================================================================================================
	// CFG 파일유무로 서울시 통계와 일반 통계를 구별한다.
	// ==========================================================================================================
	CFile File;
	// 1. 일반 도서관 통계
	if( File.Open( _T("..\\cfg\\_seoul") , CFile::modeRead | CFile::typeBinary ) == 0 )
	{
		// 대출통계는 MySQL을 구분한다.
		if ( strMenuName.Compare( LOAN_STATICS ) == 0 )
			sDir = _T("..\\cfg\\통계\\대출통계\\");
		else if ( strMenuName.Compare( WORKING_STATICS ) == 0 )
			sDir = _T("..\\cfg\\통계\\정리통계\\");
		else if ( strMenuName.Compare( HOLDING_STATICS ) == 0 )
			sDir = _T("..\\cfg\\통계\\소장통계\\");
		else if ( strMenuName.Compare( BOOK_STATICS ) == 0 )
			sDir = _T("..\\cfg\\통계\\장서점검통계\\");
		else if ( strMenuName.Compare( REPAIR_BOOK_STATICS ) == 0 )
			sDir = _T("..\\cfg\\통계\\단행_수리제본_통계\\");
		else if ( strMenuName.Compare( SE_HOLDING_STATICS ) == 0 )
			sDir = _T("..\\cfg\\통계\\연속_소장통계\\");
		//++2009.01.05 ADD BY PWR {{++
		// 책두레통계 추가
		else if ( strMenuName.Compare( COOPERATIVE_STATICS ) == 0 )
			sDir = _T("..\\cfg\\통계\\책두레통계\\");
		//--2009.01.05 ADD BY PWR --}}
	}
	// 2. 서울시 통계
	else
	{
		if ( strMenuName.Compare( LOAN_STATICS ) == 0 )
			sDir = _T("..\\cfg\\통계_서울시\\대출통계\\");
		else if ( strMenuName.Compare( WORKING_STATICS ) == 0 )
			sDir = _T("..\\cfg\\통계_서울시\\정리통계\\");
		else if ( strMenuName.Compare( HOLDING_STATICS ) == 0 )
			sDir = _T("..\\cfg\\통계_서울시\\소장통계\\");
		else if ( strMenuName.Compare( BOOK_STATICS ) == 0 )
			sDir = _T("..\\cfg\\통계_서울시\\장서점검통계\\");
		else if ( strMenuName.Compare( REPAIR_BOOK_STATICS ) == 0 )
			sDir = _T("..\\cfg\\통계_서울시\\단행_수리제본_통계\\");
		
		File.Close();
	}
	
EFS_END
}

// ##############################################################################################################
// [ 함수설명 ]
//   장서 관리의 통계
//   작업종, 소장종 통계에 같이 사용된다.
// 
// [ 미니스펙 ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::RunBoStatics( CString sStaticsClass , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgressCtrl )
{
EFS_BEGIN

	// 1. 종수 통계를 가져온다.
	// 2. 책수 통계를 가져오면서 종수 통계에 값을 ADD한다.
	// 3. 종수 합계를 가져온다. 
	// 4. 책수 합계를 가져온다.
	// 5. 종수 합계에 책수 합계를 ADD한다.
	// 끝~~

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;	
	CString sXFieldName,sYFieldName;
	CString sXGroupYN , sYGroupYN;
	CString sResultFieldName;
	CStringArray saResultFieldName;
	CString sWhere;

	// =========================================================================================================
	// 1. 통계를 내기 위한 기본값들 가져오기
	// =========================================================================================================
	ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::RunBoStatics()") );

	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물") , sResultFieldName );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::RunBoStatics()") );

	CLocCommonAPI::MakeArrayList( sResultFieldName , _T("/") , saResultFieldName );
	
	CString sXData,sYData,sTmp;
	pTotalProgressCtrl->SetRange32( 0 , (m_pRealResultDM->RefList.GetCount()*m_pRealResultDM->GetRowCount()*2) );
	INT nProgressCnt = 0;

	// =========================================================================================================
	// 2. 종 , 책의 통계를 만든다.
	// =========================================================================================================
	for ( INT i = 0;i < saTableName.GetSize();i++ )
	{
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);

		pTotalProgressCtrl->SetRange32( 0 , 1 );

		ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , _T("Y") , _T("Y") ,i , sWhere );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::RunBoStatics()") );

		sWhere += _T(" Group By ") + sXFieldName + _T(" , ") + sYFieldName;

		ids = FillStaticsValue2( sXFieldName,sYFieldName,saResultFieldName.GetAt(i),_T(""),_T(""),_T("Y"),_T("Y"),sWhere,i, pProgressCtrl);
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::RunBoStatics()") );

		pTotalProgressCtrl->SetPos(1);
	}

	// =========================================================================================================
	// 3. 합계 구성
	// =========================================================================================================
	ids = MakeTotalCount2( sStaticsClass , saTableName , sXFieldName,sYFieldName , pProgressCtrl , pTotalProgressCtrl);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::RunBoStatics()") );

	// =========================================================================================================
	// 4. 코드값을 설명값으로 바꾼다.
	// =========================================================================================================
	ids = CodeToDesc(sStaticsClass);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::RunBoStatics()") );

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ 함수설명 ]
//   작업종, 소장종 통계 WHERE절 얻어오기
//
// [ 미니스펙 ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::GetBoWhere( CString sStaticsClass,CString sTableName,CString sXFieldName,CString sYFieldName,CString sXData,CString sYData,CString sXGroupYN , CString sYGroupYN , INT nMode,CString &sWhere )
{
EFS_BEGIN

	INT ids;

	CStringArray saWhere;
	CString sMenuName = GetMenuName();
	CString sInWhere;	// 안조건
	CString sOutWhere;	// 밖조건

	// ==========================================================================================================
	// 1. 기본 조건 가져오기
	// ==========================================================================================================
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("기본조건") , sWhere );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::GetBoWhere()") );

	CLocCommonAPI::MakeArrayList( sWhere , _T("/") , saWhere );

	if( saWhere.GetSize() < nMode+1 )
		nMode = 0;
	
	sWhere = saWhere.GetAt(nMode);
	
	// ==========================================================================================================
	// 2. 검색조건에서 생성된 조건 얻어오기
	// ==========================================================================================================
	if ( sMenuName.Compare(BOOK_STATICS) != 0 )
	{
		ids = GetBoSearchWhere( sStaticsClass , sTableName , sInWhere , sOutWhere );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::GetBoWhere()") );
		if ( ids > 0 ) return ids;
	}

	// ==========================================================================================================
	// 3. Group에 의해서 생성된 조건 구성하기
	// ==========================================================================================================
	CStringArray asTableName;
	CLocCommonAPI::MakeArrayList( sTableName , _T(",") , asTableName );

	CString sTmpWhere;
	if( !sXFieldName.IsEmpty() && (sXGroupYN.Compare(_T("N"))==0) )
	{
		sXData.Replace( _T("'"), _T("''") );
		
		if( !sXData.IsEmpty() )
			sTmpWhere.Format( _T(" AND %s='%s'"),sXFieldName,sXData );
		else
			sTmpWhere.Format( _T(" AND ( %s='%s' OR %s IS NULL ) "),sXFieldName,sXData,sXFieldName);

		if( IsFieldFromTable( sXFieldName , asTableName ) == 1 )
			sOutWhere += sTmpWhere;
		else
			sInWhere += sTmpWhere;
	}

	if( !sYFieldName.IsEmpty() && (sYGroupYN.Compare(_T("N"))==0) )
	{
		sYData.Replace( _T("'"), _T("''") );
		
		if( !sYData.IsEmpty() )
			sTmpWhere.Format( _T(" AND %s='%s'"),sYFieldName,sYData );
		else
			sTmpWhere.Format( _T(" AND ( %s='%s' OR %s IS NULL ) "),sYFieldName,sYData,sYFieldName);
		if( IsFieldFromTable( sYFieldName , asTableName ) == 1 )
			sOutWhere += sTmpWhere;
		else
			sInWhere += sTmpWhere;
	}

	// 메크로 적용
	sWhere.Replace( _T("%안조건%") , sInWhere );
	sWhere.Replace( _T("%밖조건%") , sOutWhere );

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ 함수설명 ]
//   안조건절과 밖조건절 구성하기
//
// [ 미니스펙 ]
// 
// ##############################################################################################################
INT CStaticsInfoMgr::GetBoSearchWhere( CString sStaticsClass , CString sTableName , CString &sInWhere , CString &sOutWhere )
{
EFS_BEGIN

	INT ids;

	CString sMenuName = GetMenuName();

	// ==========================================================================================================
	// 1. 컨트롤 알리아스별 값 DM 구성
	// ==========================================================================================================
	ids = MakeUserSearchList( sStaticsClass );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::GetBoSearchWhere()") );
	if( ids > 0 ) return ids;

	// ==========================================================================================================
	// 2. 메뉴 명에 따라 달라진다.
	// ==========================================================================================================
	CString sPrintSearchWhere;

	if( sMenuName.Compare(LOAN_STATICS) == 0 )
	{
		bool bFlag = FALSE;
		if( _T("대출자 등록통계") == sStaticsClass )			bFlag = TRUE;
		ids = GetSearchWhere1110( sTableName , sInWhere , sOutWhere , sPrintSearchWhere, bFlag );
	}
	else if( sMenuName.Compare(WORKING_STATICS) == 0 )
		ids = GetSearchWhere1111( sTableName , sInWhere , sOutWhere , sPrintSearchWhere );
	else if( sMenuName.Compare(HOLDING_STATICS) == 0 )
		ids = GetSearchWhere1112( sStaticsClass,sTableName , sInWhere , sOutWhere , sPrintSearchWhere );
	else if( sMenuName.Compare(REPAIR_BOOK_STATICS) == 0 )
		ids = GetSearchWhere1440( sTableName , sInWhere , sOutWhere , sPrintSearchWhere );
	//++2009.01.06 ADD BY PWR {{++
	// 책두레통계 추가
	else if( sMenuName.Compare(COOPERATIVE_STATICS) == 0 )
		ids = GetSearchWhereCooperative( sTableName , sInWhere , sOutWhere , sPrintSearchWhere );
	//--2009.01.06 ADD BY PWR --}}

	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::GetBoSearchWhere()") );
	if( ids > 0 ) return ids;

	// ==========================================================================================================
	// 3. 출력용 검색조건
	// ==========================================================================================================
	ids = SetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("사용자검색조건_출력") , sPrintSearchWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::GetBoSearchWhere()") );
	if( ids > 0 ) return ids;

	return 0;

EFS_END
	return -1;
}


INT CStaticsInfoMgr::MakeUserSearchList(CString sStaticsClass)
{
	EFS_BEGIN

	INT ids;
	// 검색 조건에서 생성된 값 가져오기
	CString sSearchCond;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("사용자검색조건") , sSearchCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeUserSearchList") );

	if( sSearchCond.IsEmpty() )
	{
//		AfxMessageBox( _T("사용자 조건을 먼저 입력하십시요") );
		return 1;
	}

	CStringArray saSearchCond;
	CStringArray saOneSearchCond;
	ids = CLocCommonAPI::MakeArrayList( sSearchCond , _T(",") , saSearchCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeUserSearchList") );

	m_pUserSearchCond->FreeData();

	for( INT i = 0 ; i < saSearchCond.GetSize() ; i++ )
	{
		saOneSearchCond.RemoveAll();
		
		ids = CLocCommonAPI::MakeArrayList( saSearchCond.GetAt(i) , _T("=") , saOneSearchCond );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeUserSearchList") );
		
		if( saOneSearchCond.GetSize() != 2 ) continue;

		m_pUserSearchCond->InsertRow(-1);
		ids = m_pUserSearchCond->SetCellData( _T("컨트롤알리아스") , saOneSearchCond.GetAt(0) , m_pUserSearchCond->GetRowCount()-1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeUserSearchList") );
		ids = m_pUserSearchCond->SetCellData( _T("값") , saOneSearchCond.GetAt(1) , m_pUserSearchCond->GetRowCount()-1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeUserSearchList") );
	}

	return 0;


	EFS_END
	return -1;

}

INT CStaticsInfoMgr::FillStaticsValue2( CString sXFieldName , CString sYFieldName , CString sResultFieldName , CString sXData , CString sYData , CString sXGroupYN , CString sYGroupYN , CString sWhere , INT nMode , CProgressCtrl *pProgressCtrl )
{
	EFS_BEGIN

	INT ids;

	

	// SELECT 필드 설정
	ids = SetDataMgrRef( m_pResultDM , sXFieldName ,_T("") , sXGroupYN ,  0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("") , sYGroupYN , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("RunStatics") );
	ids = SetDataMgrRef( m_pResultDM , sResultFieldName , _T("0") , _T("Y") , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("RunStatics") );

	// WHERE 조건 설정
	CString sSendWhere = sWhere;
	
	//===================================================
	//2009.10.05 ADD BY PJW : 일자별 조건이 들어가면 일자로 정렬한다.
	if( sYFieldName == _T("TO_CHAR(B.REG_DATE,'YYYY/MM/DD')") )
	{
		sSendWhere += _T(" ORDER BY TO_CHAR(B.REG_DATE,'YYYY/MM/DD')");
	}
	else if( sYFieldName == _T("TO_CHAR(B.SHELF_DATE,'YYYY/MM/DD')") )
	{
		sSendWhere += _T(" ORDER BY TO_CHAR(B.SHELF_DATE,'YYYY/MM/DD')");
	}
	else if( sYFieldName == _T("TO_CHAR(B.CHECKIN_DATE,'YYYY/MM/DD')") )
	{
		sSendWhere += _T(" ORDER BY TO_CHAR(B.CHECKIN_DATE,'YYYY/MM/DD')");
	}
	//===================================================

	// RESULT DM -> REAL RESULT DM 으로 이동
	ids = m_pResultDM->RefreshDataManager( sSendWhere , pProgressCtrl );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue2") );

	INT nXIndex , nYIndex;
	CString sXAlias , sYAlias , sResult , sPreResult;

	for( INT i = 0 ; i < m_pResultDM->GetRowCount() ; i++ )
	{
		ids = m_pResultDM->GetCellData( _T("결과물") , i , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("FillStaticsValue2") );

		ids = m_pResultDM->GetCellData( _T("X축알리아스") , i , sXAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("FillStaticsValue2") );

		ids = m_pResultDM->GetCellData( _T("Y축알리아스") , i , sYAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("FillStaticsValue2") );
		
		// --장서 통계
		// 인덱스 찾기
		ids = GetDMIndex( m_pRealResultDM , sXAlias,sYAlias,nXIndex,nYIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("FillStaticsValue2") );

		// 값 입력
		if ( m_sIs64lib == _T("Y") && sXAlias==_T("355-9") )
			ids = m_pRealResultDM->GetCellData( _T("3") , nYIndex , sPreResult );
		else
			ids = m_pRealResultDM->GetCellData( sXAlias , nYIndex , sPreResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("FillStaticsValue2") );
				
		if( nMode == 0 )
		{
			// 2005.07.28 ADD BY PDJ
			// 군사분류기호 처리
			// ? ( ? ) 형식에 대한 연산
			if ( m_sIs64lib == _T("Y") && ( sXAlias==_T("355-9") || sXAlias==_T("3") ) ) 
			{
				CString strNormal, strArmy, sTemp;
				DividNormalArmy2(sPreResult, sXAlias, sResult, nMode, &sTemp);
				ids = m_pRealResultDM->SetCellData( _T("3") , sTemp , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("FillStaticsValue2") );
			}
			else
			{
				sResult.Format( _T("%ld") , _ttoi(sPreResult) + _ttoi(sResult) );
				ids = m_pRealResultDM->SetCellData( sXAlias , sResult , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("FillStaticsValue2") );
			}
		}
		else if( nMode == 1 )
		{
			if ( m_sIs64lib == _T("Y") && ( sXAlias==_T("355-9") || sXAlias==_T("3") ) ) 
			{
				CString strNormal, strArmy, sTemp;
				DividNormalArmy2(sPreResult, sXAlias, sResult, nMode, &sTemp);		
				ids = m_pRealResultDM->SetCellData( _T("3") , sTemp , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("FillStaticsValue2") );
			}
			else
			{   
				sResult.Format( _T("%ld\r\n(%ld)") , _ttoi(sPreResult) , _ttoi(sResult) );							
				ids = m_pRealResultDM->SetCellData( sXAlias , sResult , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -102 , _T("FillStaticsValue2") );
		
			}

		}
	}


	return 0;

	EFS_END
	return -1;

}

// 합계 구성 - 작업,소장 자료 통계 합계 구성
INT CStaticsInfoMgr::MakeTotalCount2( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName, CString sYFieldName , CProgressCtrl *pProgressCtrl , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

//	bool IsInsert = TRUE;

/*	
	// 세로 합계 구성
	ids = YSum2(sStaticsClass,saTableName,sXFieldName , sYFieldName ,pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("") );

	// 가로 합계 구성
	ids = XSum2(sStaticsClass,saTableName,sXFieldName , sYFieldName ,pTotalProgress);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("") );
*/

	/// 20040223 조병걸 수정
	/// 속도 빠르게 하기 위해 합계도 그룹핑으로 계산하기
	m_pYSumDM->FreeData();
	m_pXSumDM->FreeData();
	
	ids = XYSum2( sStaticsClass , saTableName , sXFieldName , sYFieldName , pTotalProgress );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("MakeTotalCount2") );


	return 0;


	EFS_END
	return -1;

}

// 합계 구성 - 소장 자료 통계 합계 구성
INT CStaticsInfoMgr::XYSum2( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	/// 중복 허용 합계 계산
	ids = XYSum2_duplication_allow();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("XYSum2") );

	/// 중복 삭제 합계 계산
	ids = XYSum2_duplication_delete( sStaticsClass , saTableName , sXFieldName , sYFieldName , pTotalProgress );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XYSum2") );

	//===================================================
	//2009.05.20 ADD BY PJW : 가격을 변환 한다.
	INT nRowCnt;
	INT i;
	POSITION pos;
	CString strPrice;
	CString strConverPrice;
	CString strResultAlias;
	CString sDupAddAlias;
	CString sDupDelAlias;

	CESL_DataMgr::reference *pRef;

	sDupAddAlias = _T("중복허용_합계");
	sDupDelAlias = _T("중복삭제_합계");

	if( sStaticsClass == _T("가격합산통계") )
	{
		nRowCnt = m_pRealResultDM->GetRowCount();
		for( i = 0 ; i < nRowCnt ; i++ )
		{
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				pos = m_pRealResultDM->RefList.FindIndex(j);
				pRef = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
				strResultAlias = pRef->FIELD_ALIAS;
					
				m_pRealResultDM->GetCellData( strResultAlias, i, strPrice );
				strConverPrice = ConvertPrice( strPrice );
				m_pRealResultDM->SetCellData( strResultAlias, strConverPrice, i );
			}		
		}
		

		nRowCnt = m_pYSumDM->GetRowCount();
		for( i = 0 ; i < nRowCnt ; i++ )
		{
			m_pYSumDM->GetCellData( sDupAddAlias, i, strPrice );
			strConverPrice = ConvertPrice( strPrice );
			m_pYSumDM->SetCellData( sDupAddAlias, strConverPrice, i );

			m_pYSumDM->GetCellData( sDupDelAlias, i, strPrice );
			strConverPrice = ConvertPrice( strPrice );
			m_pYSumDM->SetCellData( sDupDelAlias, strConverPrice, i );
		}

		nRowCnt = m_pXSumDM->GetRowCount();
		for( i = 0 ; i < nRowCnt ; i++ )
		{
			m_pXSumDM->GetCellData( sDupAddAlias, i, strPrice );
			strConverPrice = ConvertPrice( strPrice );
			m_pXSumDM->SetCellData( sDupAddAlias, strConverPrice, i );

			m_pXSumDM->GetCellData( sDupDelAlias, i, strPrice );
			strConverPrice = ConvertPrice( strPrice );
			m_pXSumDM->SetCellData( sDupDelAlias, strConverPrice, i );
		}
	}
	//===================================================
	
	return 0;

	EFS_END

	return -1;
}

//###############################################################################################################
// [ 함수설명 ]
//   중복 삭제 합계 - 모드 그룹화 되있다고 가정 - 책수는 똑같다고 가정
//
// [ 미니스펙 ]
//
//###############################################################################################################
INT CStaticsInfoMgr::XYSum2_duplication_delete(CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CProgressCtrl *pTotalProgress )
{
EFS_BEGIN

	INT ids;

	CStringArray saResultFieldName;
	CString sResultFieldName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물") , sResultFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	
	// 2012.11.20 ADD BY PWR : 2012년도 도서관 통합서비스 확대구축 사업 2차 (가격합산통계 속도개선)
	if(_T("가격합산통계") == sStaticsClass) return 0;

	CLocCommonAPI::MakeArrayList( sResultFieldName , _T("/") , saResultFieldName );
	
	INT i = 0;
	
	// DM 테이블 이름 추가
	m_pResultDM->TBL_NAME = saTableName.GetAt(i);
	
	CStringArray saTableAlias;	
	ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );	

	/// 종수만 한꺼번에 가져온다.
	// ==========================================================================================================
	/// 가로 합계 계산
	// ==========================================================================================================	
	// SELECT 필드 설정
	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("Y") , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

	// pjw
//	ids = SetDataMgrRef( m_pResultDM , _T("COUNT( DISTINCT S.REC_KEY )") , _T("0") , _T("Y") , 2 );
//	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	if( _T("자료실별이용통계") == sStaticsClass ||   _T("신규구입도서이용통계")== sStaticsClass)
	{
		ids = SetDataMgrRef( m_pResultDM , _T("COUNT( DISTINCT CL.BOOK_KEY )") , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	}
	else
	{
		ids = SetDataMgrRef( m_pResultDM , _T("COUNT( DISTINCT S.REC_KEY )") , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	}
	
	// 3.3. WHERE절 구성 
	CString sWhere;
		
	ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , sYFieldName , _T("") , _T("") , _T("N") , _T("Y") ,i , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

	sWhere += _T(" GROUP BY ") + sYFieldName;
		
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

	
	CString sYSumAlias , sDuplicateAllowSum;
	CString sTmpResult , sResult;
	CString sYData , sTmp;
	CString sSpeciesCnt , sBookCnt;
	INT nIndex;
	for( INT k = 0 ; k < m_pResultDM->GetRowCount() ; k++ )
	{
		ids = m_pResultDM->GetCellData( _T("Y축알리아스") , k , sYSumAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
		nIndex = -1;
		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( j , 0 , sYData );
			if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
			
			if( sYData.Compare( sYSumAlias ) == 0 ) nIndex = j;
		}
		
		if( nIndex == -1 ) continue;
		
		ids = m_pResultDM->GetCellData( _T("결과물") , k , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
		ids = m_pXSumDM->GetCellData( _T("중복허용_합계") , nIndex , sDuplicateAllowSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
		MakeSpeciesBookCnt( sDuplicateAllowSum , sSpeciesCnt , sBookCnt );
		//pjw수정
		if( _T("자료실별이용통계") == sStaticsClass ||   _T("신규구입도서이용통계")== sStaticsClass)
		{
			if( _ttoi(sBookCnt) == 0 )
			sResult = _T("0");
			else
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );
			}
			
		}else
		{
			if( _ttoi(sResult) == 0 )
			sResult = _T("0");
			else if( 0 != _ttoi(sResult) && 0 == _ttoi(sBookCnt) )
				sResult.Format( _T("%d") , _ttoi(sResult));
			else
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );
			}
		}
		
		/*if( _ttoi(sResult) == 0 )
		sResult = _T("0");
		else
		{
			sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );
		}*/

		ids = m_pXSumDM->SetCellData( _T("중복삭제_합계") , sResult , nIndex  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
	}

	// ==========================================================================================================
	// 세로 합계 계산
	// ==========================================================================================================
	// 3.3. WHERE절 구성 
	sWhere.Empty();
	
	// SELECT 필드 설정
	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("Y") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	// pjw
//	ids = SetDataMgrRef( m_pResultDM , _T("count(DISTINCT S.REC_KEY)") , _T("0") , _T("Y") , 2 );
//	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	if( _T("자료실별이용통계") == sStaticsClass ||_T("신규구입도서이용통계") == sStaticsClass  )
	{
		ids = SetDataMgrRef( m_pResultDM , _T("count(DISTINCT CL.BOOK_KEY)") , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	}
	else
	{
		ids = SetDataMgrRef( m_pResultDM , _T("count(DISTINCT S.REC_KEY)") , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	}	
	
	
	// WHERE절 생성
	ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , sXFieldName , _T("") , _T("") , _T("") ,_T("Y") , _T("N") , i , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -16 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

	sWhere += _T(" GROUP BY ") + sXFieldName;
	
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

	CString sXSumAlias;
	sDuplicateAllowSum.Empty();
	sTmpResult.Empty() , sResult.Empty();
	CString sXData;
	sTmp.Empty();
	sSpeciesCnt.Empty() , sBookCnt.Empty();
	
	for( k = 0 ; k < m_pResultDM->GetRowCount() ; k++ )
	{
		
		ids = m_pResultDM->GetCellData( _T("X축알리아스") , k , sXSumAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
		nIndex = -1;
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
			if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

			if( sXData.Compare( sXSumAlias ) == 0 ) nIndex = j;

			//##################################################################################################
			if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )		nIndex = 4;
			//##################################################################################################
		}

		if( nIndex == -1 ) continue;

		ids = m_pResultDM->GetCellData( _T("결과물") , k , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

		ids = m_pYSumDM->GetCellData( _T("중복허용_합계") , nIndex-1 , sDuplicateAllowSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
		// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 육사 군사분류기호 예외처리
		if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )
		{
			// sDuplicateAllowSum 에서 분리 작업
			CString sSpecies, sBook;
			DividArmyCell( sDuplicateAllowSum, sSpecies, sBook );			
			
			sSpecies = sSpecies.Mid( sSpecies.Find( DIVCHAR ) + 1 );
			sBook = sBook.Mid( sBook.Find( DIVCHAR ) + 1 );
			sDuplicateAllowSum.Format( _T("%d\r\n(%d)"), _ttoi( sSpecies ), _ttoi( sBook ) );
		}
		// ------------------------------------------------------------------------------------------------------

		MakeSpeciesBookCnt( sDuplicateAllowSum , sSpeciesCnt , sBookCnt );
	
		
		//PJW수정
		if( _T("자료실별이용통계") == sStaticsClass ||   _T("신규구입도서이용통계")== sStaticsClass)
		{
			if( _ttoi(sBookCnt) == 0 )
			sResult = _T("0");
			else
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );

				// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// 육사 군사분류기호 예외처리
				if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )
				{
					CString sTemp;
					ids = m_pYSumDM->GetCellData( _T("중복삭제_합계") , nIndex-1, sTemp );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -34 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

					CString sForSpeci, sForBook;
					MakeSpeciesBookCnt( sTemp, sForSpeci, sForBook );

					ids = m_pResultDM->GetCellData( _T("결과물") , k , sResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

					sResult.Format( _T("%d %s %d\r\n(%d %s %d)"), _ttoi(sForSpeci), DIVCHAR, _ttoi(sResult), _ttoi(sForBook), DIVCHAR, _ttoi(sBookCnt) );
				}
			}
			
		}else
		{
			if( _ttoi(sResult) == 0 )
			sResult = _T("0");
			else if( 0 != _ttoi(sResult) && 0 == _ttoi(sBookCnt) )
				sResult.Format( _T("%d") , _ttoi(sResult));
			else
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );

				// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// 육사 군사분류기호 예외처리
				if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )
				{
					CString sTemp;
					ids = m_pYSumDM->GetCellData( _T("중복삭제_합계") , nIndex-1, sTemp );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -34 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

					CString sForSpeci, sForBook;
					MakeSpeciesBookCnt( sTemp, sForSpeci, sForBook );

					ids = m_pResultDM->GetCellData( _T("결과물") , k , sResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

					sResult.Format( _T("%d %s %d\r\n(%d %s %d)"), _ttoi(sForSpeci), DIVCHAR, _ttoi(sResult), _ttoi(sForBook), DIVCHAR, _ttoi(sBookCnt) );
				}
			}
		}
		/*if( _ttoi(sResult) == 0 )
			sResult = _T("0");
		else
		{
			sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );

			// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// 육사 군사분류기호 예외처리
			if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )
			{
				CString sTemp;
				ids = m_pYSumDM->GetCellData( _T("중복삭제_합계") , nIndex-1, sTemp );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -34 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

				CString sForSpeci, sForBook;
				MakeSpeciesBookCnt( sTemp, sForSpeci, sForBook );

				ids = m_pResultDM->GetCellData( _T("결과물") , k , sResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

				sResult.Format( _T("%d %s %d\r\n(%d %s %d)"), _ttoi(sForSpeci), DIVCHAR, _ttoi(sResult), _ttoi(sForBook), DIVCHAR, _ttoi(sBookCnt) );
			}
			// --------------------------------------------------------------------------------------------------
		}*/

		ids = m_pYSumDM->SetCellData( _T("중복삭제_합계") , sResult , nIndex-1  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -22 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );				
	}
	
	// ==========================================================================================================
	// 총합계
	// ==========================================================================================================
	// SELECT 필드 설정

	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -24 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	ids = SetDataMgrRef( m_pResultDM , saResultFieldName.GetAt(i) , _T("0") , _T("Y") , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -25 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	
	// WHERE절 생성
	ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
	
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -26 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
			
	ids = m_pResultDM->GetCellData( _T("결과물") , 0 , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -27 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

	ids = m_pXSumDM->GetCellData( _T("중복허용_합계") , m_pXSumDM->GetRowCount()-1 , sDuplicateAllowSum );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -28 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	
	MakeSpeciesBookCnt( sDuplicateAllowSum , sSpeciesCnt , sBookCnt );
	
	if( _ttoi(sResult) == 0 )
		sResult = _T("0");
	else if( 0 != _ttoi(sResult) && 0 == _ttoi(sBookCnt) )
		sResult.Format( _T("%d") , _ttoi(sResult));
	else
		sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );	
	
	ids = m_pXSumDM->SetCellData( _T("중복삭제_합계") , sResult , m_pXSumDM->GetRowCount()-1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -29 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	
	// ==========================================================================================================
	// 모든 합계중 비어있는 공간은 0으로 만들기
	// ==========================================================================================================
	
		
	for( INT m = 0 ; m < m_pXSumDM->GetRowCount() ; m++ )
	{
		ids = m_pXSumDM->GetCellData( _T("중복삭제_합계") , m , sTmp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -30 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

		sTmp.TrimLeft(); sTmp.TrimRight();
		if( !sTmp.IsEmpty() ) continue;

		ids = m_pXSumDM->SetCellData( _T("중복삭제_합계") , _T("0") , m );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -31 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	}

	for( m = 0 ; m < m_pYSumDM->GetRowCount() ; m++ )
	{
		ids = m_pYSumDM->GetCellData( _T("중복삭제_합계") , m , sTmp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -32 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
		sTmp.TrimLeft(); sTmp.TrimRight();
		if( !sTmp.IsEmpty() ) continue;
		
		ids = m_pYSumDM->SetCellData( _T("중복삭제_합계") , _T("0") , m );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -33 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	}
	return 0;

EFS_END
	return -1;
}

//###############################################################################################################
// [ 함수설명 ]
//   중복 허용 합계 계산
//
// [ 미니스펙 ]
//
//###############################################################################################################
INT CStaticsInfoMgr::XYSum2_duplication_allow()
{
EFS_BEGIN

	INT ids;
	
	// ==========================================================================================================
	// 중복 허용 합계 구성
	CString sSumAlias = _T("중복허용_합계");
	CString sAlias;
	CString sSpeciesSum,sBookSum;
	CString sData;
	CString sSpeciesCnt,sBookCnt;
	CString sTotalSpeciesSum,sTotalBookSum;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	// ==========================================================================================================
	
	// ==========================================================================================================
	// 1. 가로 합계 구성
	// ==========================================================================================================
	for( INT i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
	{
		sSpeciesSum.Empty(); sBookSum.Empty();
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			pos = m_pRealResultDM->RefList.FindIndex(j);
			Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
			sAlias = Ref->FIELD_ALIAS;
			
			ids = m_pRealResultDM->GetCellData( sAlias ,i , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::XYSum2_duplication_allow()") );
			
			if ( m_sIs64lib == _T("Y") && sAlias ==_T("3") )
			{
				MakeArmySpeciesBookCnt( sData, &sData );
				MakeSpeciesBookCnt( sData , sSpeciesCnt , sBookCnt );
			
				sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
				sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
			}
			else
			{
				MakeSpeciesBookCnt( sData , sSpeciesCnt , sBookCnt );
				
				sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
				sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
			}
		}
		
		sTotalSpeciesSum.Format( _T("%d") , _ttoi(sTotalSpeciesSum) + _ttoi(sSpeciesSum) );
		sTotalBookSum.Format( _T("%d") , _ttoi(sTotalBookSum) + _ttoi(sBookSum) );
		
		if( m_pXSumDM->GetRowCount() < i+1 )
			m_pXSumDM->InsertRow(-1);
		
		if( (_ttoi(sSpeciesSum) == 0) && (_ttoi(sBookSum) == 0) )
			sData = _T("0");
		else if( (_ttoi(sSpeciesSum) != 0) && (_ttoi(sBookSum) == 0) )
			sData.Format( _T("%s") , sSpeciesSum );
		else
			sData.Format( _T("%s\r\n(%s)") , sSpeciesSum , sBookSum );
		
		m_pXSumDM->SetCellData( sSumAlias , sData , i);

	}
	
	if( m_pXSumDM->GetRowCount() < i+1 )
		m_pXSumDM->InsertRow(-1);
	
	if( (_ttoi(sTotalSpeciesSum) == 0) && (_ttoi(sTotalBookSum) == 0) )
		sData = _T("0");
	else if( (_ttoi(sTotalSpeciesSum) != 0) && (_ttoi(sTotalBookSum) == 0) )
			sData.Format( _T("%s") , sTotalSpeciesSum );
	else
		sData.Format( _T("%s\r\n(%s)") , sTotalSpeciesSum , sTotalBookSum );
	
	m_pXSumDM->SetCellData( sSumAlias , sData , i );

	// ==========================================================================================================
	// 중복 허용 합계 구성
	sAlias.Empty();
	sSpeciesSum.Empty();
	sBookSum.Empty();
	sData.Empty();
	sSpeciesCnt.Empty();
	sBookCnt.Empty();
	// ==========================================================================================================
	
	// ==========================================================================================================
	// 2. 세로 합계 구성
	// ==========================================================================================================
	for( i = 1 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		sSpeciesSum.Empty(); sBookSum.Empty();
		pos = m_pRealResultDM->RefList.FindIndex(i);
		Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
		sAlias = Ref->FIELD_ALIAS;
			
		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( sAlias , j , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::XYSum2_duplication_allow()") );
						
			// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// 육사의 군사분류기호 합계를 나타내기 위한 예외 처리
			if ( m_sIs64lib == _T("Y") && sAlias ==_T("3") )
			{
				// 육사는 종에다가 전부 기록한다.
				MakeArmySum( sSpeciesSum , sData, sSpeciesSum );				
			}
			else
			{
				MakeSpeciesBookCnt( sData , sSpeciesCnt , sBookCnt );
			
				sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
				sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
			}
			// --------------------------------------------------------------------------------------------------
		}
		
		if( m_pYSumDM->GetRowCount() < i )
			m_pYSumDM->InsertRow(-1);
		
		if ( m_sIs64lib == _T("Y") && sAlias ==_T("3") )
			sData.Format( _T("%s"), sSpeciesSum );
		else
		{
			if( (_ttoi(sSpeciesSum) == 0) && (_ttoi(sBookSum) == 0) )
				sData = _T("0");
			else if( (_ttoi(sSpeciesSum) != 0) && (_ttoi(sBookSum) == 0) )
				sData.Format( _T("%s") , sSpeciesSum );			
			else
				sData.Format( _T("%s\r\n(%s)") , sSpeciesSum , sBookSum );			
		}
		
		m_pYSumDM->SetCellData( sSumAlias , sData ,i-1 );		
	}	
		
	return 0;
EFS_END
	return -1;
}

// 세로 합계 구성 - 20040203 사용안함 조병걸 수정
INT CStaticsInfoMgr::YSum2( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	CStringArray saResultFieldName;
	CString sResultFieldName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물") , sResultFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunBoStatics") );

	CLocCommonAPI::MakeArrayList( sResultFieldName , _T("/") , saResultFieldName );

	// 세로 합계 구성
	pTotalProgress->SetRange32( 0 , saTableName.GetSize()*(m_pRealResultDM->RefList.GetCount()+m_pRealResultDM->GetRowCount()) );

	m_pYSumDM->FreeData();

	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM 테이블 이름 추가
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);
		
		CStringArray saTableAlias;

		ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue") );

		// 3.3. WHERE절 구성 
		CString sWhere;
				
		CString sTmpResult , sResult;
		CString sXData , sTmp;
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
			if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

			// SELECT 필드 설정
			ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
			ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );
			ids = SetDataMgrRef( m_pResultDM , saResultFieldName.GetAt(i) , _T("0") , _T("Y") , 2 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("RunStatics") );


			// WHERE절 생성
			ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , sXFieldName , _T("") , sXData , _T("") ,_T("N") , _T("N") , i , sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeTotalCount") );
	
			ids = m_pResultDM->RefreshDataManager( sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );

			// RESULT DM -> REAL RESULT DM 으로 이동
			if( m_pYSumDM->GetRowCount() < j )
				m_pYSumDM->InsertRow(-1);

			ids = m_pYSumDM->GetCellData( _T("중복삭제_합계") , j-1 , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			ids = m_pResultDM->GetCellData( _T("결과물") , 0 , sTmpResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			if( i == 0 )
				sResult.Format( _T("%d") , _ttoi(sTmpResult)  );
			else if( i == 1 )
			{
				if( (_ttoi(sResult) == 0) && (_ttoi(sTmpResult) == 0) )
					sResult = _T("0");
				else
					sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sTmpResult) );
			}
			
			ids = m_pYSumDM->SetCellData( _T("중복삭제_합계") , sResult , j-1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			pTotalProgress->SetPos( (i+1)*(j+1) );

		}
	}

	// 중복 허용 합계 구성
	CString sSumAlias = _T("중복허용_합계");
	CString sAlias;
	CString sSpeciesSum,sBookSum;
	CString sData;
	CString sSpeciesCnt,sBookCnt;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	for( i = 1 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		sSpeciesSum.Empty(); sBookSum.Empty();
		pos = m_pRealResultDM->RefList.FindIndex(i);
		Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
		sAlias = Ref->FIELD_ALIAS;

		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( sAlias , j , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );

			MakeSpeciesBookCnt( sData , sSpeciesCnt , sBookCnt );

			sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
			sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
		}
		
		if( m_pYSumDM->GetRowCount() < i )
			m_pYSumDM->InsertRow(-1);

		if( (_ttoi(sSpeciesSum) == 0) && (_ttoi(sBookSum) == 0) )
			sData = _T("0");
		else
			sData.Format( _T("%s\r\n(%s)") , sSpeciesSum , sBookSum );

		m_pYSumDM->SetCellData( sSumAlias , sData ,i-1 );
	}	

	return 0;

	EFS_END
	return -1;

}


// 가로 합계 구성 - 20040203 사용안함 - 조병걸 수정
INT CStaticsInfoMgr::XSum2( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName , CString sYFieldName , CProgressCtrl *pTotalProgress )
{
	EFS_BEGIN

	INT ids;

	CStringArray saResultFieldName;
	CString sResultFieldName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물") , sResultFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunBoStatics") );

	CLocCommonAPI::MakeArrayList( sResultFieldName , _T("/") , saResultFieldName );

	m_pXSumDM->FreeData();

	// 중복 삭제 합계 구성
	CString sWhere;
	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM 테이블 이름 추가
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);
		
		// 3.3. WHERE절 구성 
		CString sYData;
		CString sTmpResult , sResult;
		for( INT k = 0 ; k < m_pRealResultDM->GetRowCount()+1 ; k++ )
		{
			if( k == m_pRealResultDM->GetRowCount() )
				sYData = _T("합계");
			else
			{
				ids = m_pRealResultDM->GetCellData( k , 0 , sYData );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			}

			// SELECT 필드 설정
			ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeTotalCount") );
			ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeTotalCount") );
			ids = SetDataMgrRef( m_pResultDM , saResultFieldName.GetAt(i) , _T("0") , _T("Y") , 2 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("RunStatics") );
			
			// WHERE절 생성
			if( sYData.Compare(_T("합계"))==0 )
				ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
			else
				ids = GetBoWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , sYFieldName , _T("") , sYData , _T("N") , _T("N") ,i , sWhere );
			
			ids = m_pResultDM->RefreshDataManager( sWhere );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			// RESULT DM -> REAL RESULT DM 으로 이동
			if( m_pXSumDM->GetRowCount() < k+1 )
				m_pXSumDM->InsertRow(-1);

			ids = m_pXSumDM->GetCellData( _T("중복삭제_합계") , k , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			ids = m_pResultDM->GetCellData( _T("결과물") , 0 , sTmpResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			if( i == 0 )
				sResult.Format( _T("%ld") , _ttoi(sTmpResult) );
			else if( i == 1 )
			{
				if( (_ttoi(sResult) == 0) && (_ttoi(sTmpResult) == 0) )
					sResult = _T("0");
				else
					sResult.Format( _T("%ld\r\n(%ld)") , _ttoi(sResult) , _ttoi(sTmpResult) );
			}
			
			ids = m_pXSumDM->SetCellData( _T("중복삭제_합계") , sResult , k );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeTotalCount") );
			
			pTotalProgress->SetPos( (i+1)*(k+1)+(saTableName.GetSize()*m_pRealResultDM->RefList.GetCount()) );
		}	
	}

	// 중복 허용 합계 구성
	CString sSumAlias = _T("중복허용_합계");
	CString sAlias;
	CString sSpeciesSum,sBookSum;
	CString sData;
	CString sSpeciesCnt,sBookCnt;
	CString sTotalSpeciesSum,sTotalBookSum;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	for( i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
	{
		sSpeciesSum.Empty(); sBookSum.Empty();
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			pos = m_pRealResultDM->RefList.FindIndex(j);
			Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
			sAlias = Ref->FIELD_ALIAS;

			ids = m_pRealResultDM->GetCellData( sAlias ,i , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("XSum") );

			MakeSpeciesBookCnt( sData , sSpeciesCnt , sBookCnt );

			sSpeciesSum.Format( _T("%ld") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
			sBookSum.Format( _T("%ld") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
		}
		
		sTotalSpeciesSum.Format( _T("%ld") , _ttoi(sTotalSpeciesSum) + _ttoi(sSpeciesSum) );
		sTotalBookSum.Format( _T("%ld") , _ttoi(sTotalBookSum) + _ttoi(sBookSum) );

		if( m_pXSumDM->GetRowCount() < i+1 )
			m_pXSumDM->InsertRow(-1);

		if( (_ttoi(sSpeciesSum) == 0) && (_ttoi(sBookSum) == 0) )
			sData = _T("0");
		else
			sData.Format( _T("%s\r\n(%s)") , sSpeciesSum , sBookSum );

		m_pXSumDM->SetCellData( sSumAlias , sData , i);
		
	}

	if( m_pXSumDM->GetRowCount() < i+1 )
		m_pXSumDM->InsertRow(-1);

	if( (_ttoi(sTotalSpeciesSum) == 0) && (_ttoi(sTotalBookSum) == 0) )
		sData = _T("0");
	else
		sData.Format( _T("%s\r\n(%s)") , sTotalSpeciesSum , sTotalBookSum );

	m_pXSumDM->SetCellData( sSumAlias , sData , i );
	
	return 0;

	EFS_END
	return -1;

}


INT CStaticsInfoMgr::MakeSpeciesBookCnt( CString sData ,CString &sSpeciesCnt ,CString &sBookCnt )
{
	EFS_BEGIN

	INT	 nPIndex = sData.Find(_T("("));
	INT	 nLIndex = sData.Find(_T(")"));

	sSpeciesCnt.Empty();
	sBookCnt.Empty();

	if( (nPIndex > -1 && nLIndex > -1) )
	{
		sSpeciesCnt = sData.Mid(0,nPIndex);
		sBookCnt = sData.Mid( nPIndex+1 , nLIndex-nPIndex-1 );
	}

	if( -1 == nPIndex && -1 == nLIndex )
	{
		sSpeciesCnt = sData;
	}

	return 0;

	EFS_END
	return -1;
}


INT CStaticsInfoMgr::GetSearchWhere1111( CString sTableName , CString &sInWhere , CString &sOutWhere , CString &sPrintSearchWhere )
{
	EFS_BEGIN

	INT ids;

	// 검색 조건 - 이거 노가다 어쩔수 없당 ㅡㅜ
	CStringArray saTableName;
	CString sDBField;

	ids = CLocCommonAPI::MakeArrayList( sTableName , _T(",") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );

	// 수입구분 , 관리구분 , 분류기호구분 , 수입년도
	const INT nCnt1 = 4;
	TCHAR *sControlAlias1[nCnt1] =
	{
		_T("수입구분") , _T("관리구분") , _T("분류기호구분") , _T("수입년도") 
	};
	INT sFieldType1[nCnt1] =
	{
		ONESTRING , ONESTRING , ONESTRING , ONENUMERIC
	};

	CString sWhere;
	CString sValue;
	CString sFieldName;
	for( INT i = 0 ; i < nCnt1 ; i++ )
	{
		sValue.Empty();
		sWhere.Empty();

		GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias1[i] , _T("값") , sValue );

		if( sValue.IsEmpty() ) continue;

		// 필드명 가져오기
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , sControlAlias1[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

		if( sFieldType1[i] == ONESTRING )
			sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue);
		else if( sFieldType1[i] == ONENUMERIC )
			sWhere.Format( _T(" %s = %s ") , sFieldName , sValue );

		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;

		if( i != 3 )
		{
			if( i == 2 )
				m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("분류표구분") , sValue , sValue );
			else
				m_pInfo->pCodeMgr->ConvertCodeToDesc( sControlAlias1[i] , sValue , sValue );
		}
		 

		/// 수입 년도만 보여주기
		if( i == 3 )
			sPrintSearchWhere += sControlAlias1[i] + CString(_T(":")) + sValue + SPACE_STRING;
	}

	// 등록번호
	sWhere.Empty();
	const INT nCnt2 = 3;
	TCHAR *sControlAlias2[nCnt2] =
	{
		_T("등록번호구분") , _T("등록번호1") , _T("등록번호2") 
	};


	CString sValue1,sValue2,sValue3;

	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias2[0] , _T("값") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias2[1] , _T("값") , sValue2 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias2[2] , _T("값") , sValue3 );

	if( !sValue1.IsEmpty() )
	{
		if( !sValue2.IsEmpty() )
		{
			CString sRegNo1 , sRegNo2;
			sRegNo1 = sValue1+sValue2;
			sRegNo2 = sValue1+sValue3;

			CLocCommonAPI::MakeRegNoSpaceToZero( sRegNo1 );

//			if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
			sPrintSearchWhere += CString(_T("등록번호:"));

			if( !sValue3.IsEmpty() )
			{
				CLocCommonAPI::MakeRegNoSpaceToZero( sRegNo2 );
				sWhere.Format( _T(" B.REG_NO BETWEEN '%s' AND '%s' ") ,  sRegNo1 , sRegNo2 );
				sPrintSearchWhere += sRegNo1 + _T("~") + sRegNo2;
			}
			else
			{
				sWhere.Format( _T(" B.REG_NO = '%s' ") ,  sRegNo1 );
				sPrintSearchWhere += sRegNo1;
			}

			sPrintSearchWhere += SPACE_STRING;
		}
//		else
//			sWhere.Format( _T(" B.REG_CODE = '%s' ") ,  sValue1 );
	}

	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( _T("B.REG_NO") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}		 

	// 원부번호 , 등록일자
	// 조병걸 수정 2003년 12우러 5일 등록일자(원부) 추가
	sWhere.Empty();
	const INT nCnt3 = 6;
	TCHAR *sControlAlias3[nCnt3] =
	{
		_T("원부번호1") , _T("원부번호2") , _T("등록일자1") , _T("등록일자2") , _T("등록일자(원부)1") , _T("등록일자(원부)2")
	};


	CString sValue4,sValue5,sValue6;
	sValue1.Empty(); sValue2.Empty(); sValue3.Empty();
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias3[0] , _T("값") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias3[1] , _T("값") , sValue2 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias3[2] , _T("값") , sValue3 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias3[3] , _T("값") , sValue4 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias3[4] , _T("값") , sValue5 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias3[5] , _T("값") , sValue6 );
	

	// 원부번호
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , _T("원부번호") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

	if( !sValue1.IsEmpty() )
	{
//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
		sPrintSearchWhere += CString(_T("원부번호:"));

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN %s AND %s ") , sFieldName , sValue1 , sValue2 );		
			sPrintSearchWhere += sValue1 + _T("~") + sValue2;
		}
		else
		{
			sPrintSearchWhere += sValue1;
			sWhere.Format( _T(" %s = %s ") , sFieldName , sValue1 );
		}
		sPrintSearchWhere += SPACE_STRING;
	}

	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( _T("B.A") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}

	sWhere.Empty();
	// 등록일자
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , _T("등록일자") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

	if( !sValue3.IsEmpty() )
	{
//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
//		sPrintSearchWhere += CString(_T("[등록일자:")) + sValue3;
		
		if( !sValue4.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue3 , sValue4 );
//			sPrintSearchWhere += _T("~") + sValue4;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue3 , sValue3 );
//		sPrintSearchWhere += _T("]");
		
	}
	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}
	
	sWhere.Empty();
	// 등록일자(원부)
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , _T("등록일자(원부)") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );
	
	if( !sValue5.IsEmpty() )
	{
		//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
		//		sPrintSearchWhere += CString(_T("[등록일자:")) + sValue3;
		
		if( !sValue6.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue5 , sValue6 );
			//			sPrintSearchWhere += _T("~") + sValue4;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue5 , sValue5 );
		//		sPrintSearchWhere += _T("]");
		
	}

	if( !sWhere.IsEmpty() )
	{
		sOutWhere += _T(" AND ") + sWhere;
	}

	// 부록자료 포함 여부
	sValue.Empty();
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , _T("부록자료포함") , _T("값") , sValue );
	
	if( !sValue.IsEmpty() )
	{
		sFieldName = _T("B.BOOK_APPENDIX_FLAG");

		sWhere = _T(" B.BOOK_APPENDIX_FLAG = 'B' ");
		
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;

//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
//		sPrintSearchWhere += CString(_T("[부록자료포함:N]"));	
	}
	else
	{
//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
//		sPrintSearchWhere += CString(_T("[부록자료포함:Y]"));	
	}

	if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += SPACE_STRING ;
	sPrintSearchWhere += CString(_T("단위:종(책)"));

	return 0;

	EFS_END
	return -1;
}

INT CStaticsInfoMgr::GetSearchWhere1112( CString sStaticsClass ,CString sTableName , CString &sInWhere , CString &sOutWhere , CString &sPrintSearchWhere )
{
	EFS_BEGIN

	INT ids;

	// 검색 조건 - 이거 노가다 어쩔수 없당 ㅡㅜ
	CStringArray saTableName;
	CString sDBField;

	ids = CLocCommonAPI::MakeArrayList( sTableName , _T(",") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );

	// 수입구분 , 관리구분 , 분류기호구분 , 수입년도
	const INT nCnt1 = 4;
	TCHAR *sControlAlias1[nCnt1] =
	{
		_T("배가부서") , _T("관리구분") , _T("분류기호구분") , _T("수입구분")
	};
	INT sFieldType1[nCnt1] =
	{
		ONESTRING , ONESTRING , ONESTRING , ONESTRING 
	};


	CString sWhere;
	CString sValue;
	CString sFieldName;
	for( INT i = 0 ; i < nCnt1 ; i++ )
	{
		sValue.Empty();
		sWhere.Empty();

		GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias1[i] , _T("값") , sValue );

		if( sValue.IsEmpty() ) continue;

		// 필드명 가져오기
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , sControlAlias1[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

		if( sFieldType1[i] == ONESTRING )
			sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue);
		else if( sFieldType1[i] == ONENUMERIC )
			sWhere.Format( _T(" %s = %s ") , sFieldName , sValue );

		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;

		if( i == 0 )
			m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분") , sValue , sValue );
		else if( i == 2 )
			m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("분류표구분") , sValue , sValue );
		else
			m_pInfo->pCodeMgr->ConvertCodeToDesc( sControlAlias1[i] , sValue , sValue );
	}

	// 자료 상태
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , _T("자료상태") , _T("값") , sValue );
	
	if( !sValue.IsEmpty() )
	{
		if( sValue.Compare( _T("소장자료(비치자료+대출중자료+수리제본자료+파손자료+분실자료)")) == 0 )
		{
			// 17/07/20 김혜영 : 책나래대출자료 상태 추가
/*//BEFORE-----------------------------------------------------------------------------
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL113O', 'BOL252N', 'BOL114O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차(BOL218O)자료상태 추가
			//2010.01.14 UPDATE BY PJW : 무인반납(비치자료에 포함)(BOL215O), 긴급대출자료(BOL214O), 배달자료(BOL2185), 장기대출자료추가함(BOL217O)
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL215O', 'BOL113O', 'BOL252N', 'BOL114O' ) ") );
			// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL252N', 'BOL511O' , 'BOL611O') ") );
			//sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL252N', 'BOL511O' ) ") );
			//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL219O', 'BOL252N', 'BOL411O', 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/
		}
		else if( sValue.Compare(_T("소장자료(비치자료+대출중자료)")) == 0 )
		{
			// 17/07/20 김혜영 : 책나래대출자료 상태 추가
/*//BEFORE-----------------------------------------------------------------------------
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
			//2010.01.14 UPDATE BY PJW : 무인반납(비치자료에 포함)(BOL215O), 긴급대출자료(BOL214O), 배달자료(BOL218O), 장기대출자료추가함(BOL217O)
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL215O' ) ") );
			// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL511O' , 'BOL611O') ") );
			//sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL511O' ) ") );
			//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL219O', 'BOL411O', 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/
		}
		else if( sValue.Compare(_T("비치자료")) == 0 )
		{
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N' ) ") );
		}
		else if( sValue.Compare(_T("대출중자료")) == 0 )
		{
			// 17/07/20 김혜영 : 책나래대출자료 상태 추가
/*//BEFORE-----------------------------------------------------------------------------
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
			//2010.01.14 UPDATE BY PJW : 무인반납(비치자료에 포함)(BOL216O), 긴급대출자료(BOL214O), 배달자료(BOL215O), 장기대출자료추가함(BOL217O)
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL211O', 'BOL212O', 'BOL213O', 'BOL215O' ) ") );
			// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL217O', 'BOL218O', 'BOL511O' , 'BOL611O') ") );
			//sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL217O', 'BOL218O', 'BOL511O' ) ") );
			//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL217O', 'BOL218O', 'BOL219O', 'BOL411O', 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/
		}
		else if( sValue.Compare(_T("수리제본자료")) == 0 )
		{
			//===================================================
			//2010.01.05 ADD BY PJW : 무인반납(비치자료에 포함)(BOL216O) 함
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL113O', 'BOL216O' ) ") );
			//===================================================
			
		}
		else if( sValue.Compare(_T("분실자료")) == 0 )
		{
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL252N' ) ") );
		}
		else if( sValue.Compare(_T("제적자료")) == 0 )
		{
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL115O' ) ") );
		}
		else if( sValue.Compare(_T("파손자료")) == 0 )
		{
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL114O' ) ") );
		}
		// 김용배추가 - 2003/12/10
		// 재정리자료 관련 추가
		else if( sValue.Compare( _T("소장자료(비치자료+대출중자료+수리제본자료+파손자료+분실자료+재정리자료)") ) == 0 )
		{
			// 17/07/20 김혜영 : 책나래대출자료 상태 추가
/*//BEFORE-----------------------------------------------------------------------------
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL113O', 'BOL252N', 'BOL114O', 'BOL116O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
			//2010.01.14 UPDATE BY PJW : 무인반납(비치자료에 포함)(BOL215O), 긴급대출자료(BOL214O), 배달자료(BOL2185), 장기대출자료추가함(BOL217O)
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL215O', 'BOL113O', 'BOL252N', 'BOL114O', 'BOL116O' ) ") );
			// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL252N', 'BOL511O' , 'BOL611O') ") );
			//sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL252N', 'BOL511O' ) ") );
			//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL219O', 'BOL252N', 'BOL411O', 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/
		}
		else if( sValue.Compare( _T("소장자료(비치자료+대출중자료+재정리자료)") ) == 0 )
		{
			// 17/07/20 김혜영 : 책나래대출자료 상태 
/*//BEFORE-----------------------------------------------------------------------------
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL116O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL215O', 'BOL116O' ) ") );
			// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL511O' , 'BOL611O') ") );
			//sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL511O' ) ") );
			//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL219O', 'BOL411O', 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/
		}
		else if( sValue.Compare( _T("소장자료(비치자료+대출중자료+수리제본자료+파손자료+분실자료+재정리자료+제적자료)") ) == 0 )
		{
			// 17/07/20 김혜영 : 책나래대출자료 상태 추가
/*//BEFORE-----------------------------------------------------------------------------
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL113O', 'BOL252N', 'BOL114O', 'BOL116O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL215O', 'BOL113O', 'BOL252N', 'BOL114O', 'BOL116O', 'BOL115O' ) ") );
			// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL115O', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL252N', 'BOL511O' , 'BOL611O') ") );
			//sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL115O', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL252N', 'BOL511O' ) ") );
			//=====================================================
*///AFTER------------------------------------------------------------------------------
// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL112N', 'BOL113O', 'BOL114O', 'BOL115O', 'BOL116O', 'BOL211O', 'BOL212O', 'BOL213O', 'BOL214O', 'BOL215O', 'BOL216O', 'BOL217O', 'BOL218O', 'BOL219O', 'BOL252N', 'BOL411O', 'BOL511O' , 'BOL611O') ") );
/*//END-------------------------------------------------------------------------------*/
		}
		else if( sValue.Compare( _T("재정리자료") ) == 0 )
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'BOL116O' ) ") );
		
		if( IsFieldFromTable( _T("B.WORKING_STATUS"), saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;		

		if( (sValue.Compare(_T("소장자료(비치자료+대출중자료+수리제본자료+파손자료+분실자료)")) != 0) &&
			(sValue.Compare(_T("소장자료(비치자료+대출중자료)")) != 0) )
			sPrintSearchWhere += CString(_T("자료상태:")) + sValue + SPACE_STRING;		
	}	

	// 등록번호
	sWhere.Empty();
	const INT nCnt2 = 3;
	TCHAR *sControlAlias2[nCnt2] =
	{
		_T("등록번호구분"), _T("등록번호1"), _T("등록번호2") 
	};


	CString sValue1,sValue2,sValue3;

	GetCellData( m_pUserSearchCond, _T("컨트롤알리아스"), sControlAlias2[0], _T("값"), sValue1 );
	GetCellData( m_pUserSearchCond, _T("컨트롤알리아스"), sControlAlias2[1], _T("값"), sValue2 );
	GetCellData( m_pUserSearchCond, _T("컨트롤알리아스"), sControlAlias2[2], _T("값"), sValue3 );

	if( !sValue1.IsEmpty() )
	{
		if( !sValue2.IsEmpty() )
		{
			CString sRegNo1 , sRegNo2;
			sRegNo1 = sValue1+sValue2;
			sRegNo2 = sValue1+sValue3;

			CLocCommonAPI::MakeRegNoSpaceToZero( sRegNo1 );

//			if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
			sPrintSearchWhere += CString(_T("등록번호:")) + sRegNo1;

			if( !sValue3.IsEmpty() )
			{
				CLocCommonAPI::MakeRegNoSpaceToZero( sRegNo2 );
				sWhere.Format( _T(" B.REG_NO BETWEEN '%s' AND '%s' ") ,  sRegNo1 , sRegNo2 );
				sPrintSearchWhere += _T("~") + sRegNo2;
			}
			else
				sWhere.Format( _T(" B.REG_NO = '%s' ") ,  sRegNo1 );

			sPrintSearchWhere += SPACE_STRING;
		}
//		else
//			sWhere.Format( _T(" B.REG_CODE = '%s' ") ,  sValue1 );
	}

	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( _T("B.REG_NO") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}		 

	// 배가일자
	sWhere.Empty();
	const INT nCnt3 = 2;
	TCHAR *sControlAlias3[nCnt3] =
	{
		_T("배가일자1") , _T("배가일자2")
	};

	sValue1.Empty(); sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias3[0] , _T("값") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias3[1] , _T("값") , sValue2 );

	// 배가일자
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , _T("배가일자") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

	if( !sValue1.IsEmpty() )
	{
//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
		sPrintSearchWhere += CString(_T("배가일자:")) + sValue1;

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
			sPrintSearchWhere += CString(_T("~")) + sValue2;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
		sPrintSearchWhere += SPACE_STRING;
		
	}

	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}

	// 배가변경일자
	sWhere.Empty();
	const INT nCnt4 = 2;
	TCHAR *sControlAlias4[nCnt4] =
	{
		_T("배가변경일자1") , _T("배가변경일자2")
	};
	
	sValue1.Empty(); sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias4[0] , _T("값") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias4[1] , _T("값") , sValue2 );
	
	// 배가변경일자
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , _T("배가변경일자") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );
	
	if( !sValue1.IsEmpty() )
	{
//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
		sPrintSearchWhere += CString(_T("배가변경일자:")) + sValue1;

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
			sPrintSearchWhere += CString(_T("~")) + sValue2;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
		sPrintSearchWhere += SPACE_STRING;
	}
	
	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}

	
	///대출일자추가 pjw
	sWhere.Empty();
	const INT nCnt6 = 2;
	TCHAR *sControlAlias6[nCnt6] =
	{
		_T("대출일자1") , _T("대출일자2")
	};
	
	sValue1.Empty();sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias6[0] , _T("값") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias6[1] , _T("값") , sValue2 );
		
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , _T("대출일자") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );
	
	if( !sValue1.IsEmpty() )
	{
		sPrintSearchWhere += CString(_T("대출일자:")) + sValue1;

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
			sPrintSearchWhere += CString(_T("~")) + sValue2;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
		sPrintSearchWhere += SPACE_STRING;
	}
	
	if( !sWhere.IsEmpty() )
	{
		/*
		// 필드명 가져오기
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , sControlAlias1[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );
			/*
		sFieldName = _T("B.BOOK_APPENDIX_FLAG");

		sWhere = _T(" B.BOOK_APPENDIX_FLAG = 'B' ");
		
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
		*/
		if( _T("자료실별이용통계") == sStaticsClass ||   _T("신규구입도서이용통계")== sStaticsClass)
		{
			sOutWhere += _T(" AND ") + sWhere;
			sInWhere += _T(" AND ") + sWhere;
		}
		else
		{
			if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
				sOutWhere += _T(" AND ") + sWhere;
			else
				sInWhere += _T(" AND ") + sWhere;
		}
	}	

	///자료상태변경일자추가 pjw
	sWhere.Empty();
	const INT nCnt7 = 2;
	TCHAR *sControlAlias7[nCnt7] =
	{
		_T("자료상태변경일자1") , _T("자료상태변경일자2")
	};
	
	sValue1.Empty();sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias7[0] , _T("값") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias7[1] , _T("값") , sValue2 );
		
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , _T("자료상태변경일자") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );
	
	if( !sValue1.IsEmpty() )
	{
		sPrintSearchWhere += CString(_T("자료상태변경일자:")) + sValue1;

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
			sPrintSearchWhere += CString(_T("~")) + sValue2;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
		sPrintSearchWhere += SPACE_STRING;
	}
	
	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}	

	///제적일자 추가
	sWhere.Empty();
	const INT nCnt8 = 2;
	TCHAR *sControlAlias8[nCnt8] =
	{
		_T("제적일자1") , _T("제적일자2")
	};
	
	sValue1.Empty();sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias8[0] , _T("값") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias8[1] , _T("값") , sValue2 );
		
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , _T("제적일자") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );
	
	if( !sValue1.IsEmpty() )
	{
		sPrintSearchWhere += CString(_T("제적일자:")) + sValue1;

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
			sPrintSearchWhere += CString(_T("~")) + sValue2;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
		sPrintSearchWhere += SPACE_STRING;
	}
	
	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}	

	// 20040203 조병걸 수정 - 원부작성년도 추가
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , _T("원부작성년도") , _T("값") , sValue );

	
	if( !sValue.IsEmpty() )
	{
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , _T("원부작성년도") , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

		sPrintSearchWhere += CString(_T("원부작성년도:"));

		sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue );

		sPrintSearchWhere += sValue;
		sPrintSearchWhere += SPACE_STRING;
		
	}
	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( _T("B.A") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}


	// 원부번호 , 등록일자
	sWhere.Empty();
	const INT nCnt5 = 2;
	TCHAR *sControlAlias5[nCnt5] =
	{
		_T("원부번호1") , _T("원부번호2") 
	};
	
	sValue1.Empty(); sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias5[0] , _T("값") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias5[1] , _T("값") , sValue2 );	
	
	// 원부번호
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , _T("원부번호") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );
	
	if( !sValue1.IsEmpty() )
	{
//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
		sPrintSearchWhere += CString(_T("원부번호:"));
		
		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN %s AND %s ") , sFieldName , sValue1 , sValue2 );		
			sPrintSearchWhere += sValue1 + _T("~") + sValue2;
		}
		else
		{
			sPrintSearchWhere += sValue1;
			sWhere.Format( _T(" %s = %s ") , sFieldName , sValue1 );
		}
		sPrintSearchWhere += SPACE_STRING;
	}
	
	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( _T("B.A") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}

	

	// 부록자료 포함 여부
	sWhere.IsEmpty();
	sValue.Empty();
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , _T("부록자료포함") , _T("값") , sValue );
	
	if( !sValue.IsEmpty() )
	{
		sFieldName = _T("B.BOOK_APPENDIX_FLAG");

		sWhere = _T(" B.BOOK_APPENDIX_FLAG = 'B' ");
		
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;

//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
//		sPrintSearchWhere += CString(_T("[부록자료포함:N"));
	}
	else
	{
//		if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
//		sPrintSearchWhere += CString(_T("[부록자료포함:Y"));		
	}

	if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += SPACE_STRING ;
	//PJW수정
	if(sStaticsClass == _T("자료실별이용통계") || sStaticsClass == _T("신규구입도서이용통계"))
	{
		sPrintSearchWhere += CString(_T("단위:대출이력책(소장책)"));
	}else
	{
		sPrintSearchWhere += CString(_T("단위:종(책)"));
	}
//	sPrintSearchWhere += CString(_T("단위:종(책)"));
	
	return 0;


	EFS_END
	return -1;

}

// 대출/반납 통계에서 안조건과 밖조건 구하기 
INT CStaticsInfoMgr::GetSearchWhere1110( CString sTableName , CString &sInWhere , CString &sOutWhere , CString &sPrintSearchWhere, bool flag )
{
	EFS_BEGIN

	INT ids;

	// 검색 조건 - 이거 노가다 어쩔수 없당 ㅡㅜ
	CStringArray saTableName;
	CString sDBField;

	ids = CLocCommonAPI::MakeArrayList( sTableName , _T(",") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );

	// 2007.06.17 add by pdw
	CString sStaticsClass;
	GetControlData( _T("CM_BO_STATICS_1100") , _T("통계종류") , sStaticsClass , -1 , 0 );

	// 분류기호구분,관리구분
	const INT nCnt1 = 2;
	TCHAR *sControlAlias1[nCnt1] =
	{
		_T("분류기호구분") , _T("관리구분") 
	};
	INT sFieldType1[nCnt1] =
	{
		ONESTRING , ONESTRING 
	};

	CString sWhere;
	CString sValue;
	CString sFieldName;
	for( INT i = 0 ; i < nCnt1 ; i++ )
	{
		sValue.Empty();
		sWhere.Empty();

		GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias1[i] , _T("값") , sValue );

		if( sValue.IsEmpty() ) continue;

		// 필드명 가져오기
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , sControlAlias1[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetSearchWhere1110") );

		// 2008.06.13 ADD BY PDW {{++
		// 대출자 등록통계 >> 관리구분
		if( 0 == sStaticsClass.Compare(_T("대출자 등록통계")) &&
			1 == i )
		{
			// 18.09.27 JMJ KOL.RED.2018.008
			// 책두레자료 대출처리시에 LS_WORK_T0.manage_code 를 대출관으로 변경. 소장관이던 대출처리관이던 문제없이 동작하게 한다.
			// CL.MANAGE_CODE 를 바꿔서 CL.BOOK_MANAGE_CODE 로 했다. 다시 대출자 등록통계일때는 MANAGE_CODE로 변경되게 한다.
			INT nDivPos = sFieldName.Find(_T("_"));
			//INT nDivPos = sFieldName.Find(_T("."));
			sFieldName = sFieldName.Mid(nDivPos+1);
		}
		// 2008.06.13 ADD BY PDW --}}

		// 18.09.27 JMJ KOL.RED.2018.008
		if( sFieldType1[i] == ONESTRING )
		{
			if((_T("대출 자료 통계") == sStaticsClass || _T("대출 자료 통계(책두레포함)") == sStaticsClass) && i == 0)
			{
				sWhere.Format( _T(" (%s = '%s' OR %s IS NULL) ") , sFieldName , sValue , sFieldName);
			}
			else
				sWhere.Format( _T(" (%s = '%s') ") , sFieldName , sValue);
		}
		else if( sFieldType1[i] == ONENUMERIC )
		{
			if((_T("대출 자료 통계") == sStaticsClass || _T("대출 자료 통계(책두레포함)") == sStaticsClass) && i == 0)
			{
				sWhere.Format( _T(" (%s = '%s' OR %s IS NULL) ") , sFieldName , sValue , sFieldName);
			}
			else
				sWhere.Format( _T(" (%s = '%s') ") , sFieldName , sValue);
		}


		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;

		if( i == 0 )
			m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("분류표구분") , sValue , sValue );
		else
			m_pInfo->pCodeMgr->ConvertCodeToDesc( sControlAlias1[i] , sValue , sValue );

		sPrintSearchWhere += sControlAlias1[i] + CString(_T(":")) + sValue + SPACE_STRING;
	}

	// 2007.07.10 update pdw 
	// 최대대출자료 통계 쿼리 튜닝관련 수정
	CString strBestLoanBookWhere;

	// 20040223 조병걸 수정 : 우편번호 , 주소 추가
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , _T("우편번호") , _T("값") , sValue );
	if( !sValue.IsEmpty() )		
	{
		sFieldName = _T("CLU.H_ZIPCODE");
		
		sPrintSearchWhere += _T("우편번호") + CString(_T(":")) + sValue + SPACE_STRING;
				
		sWhere.Format( _T(" %s LIKE '%s%%' ") , sFieldName , sValue );
		
		// 18.09.27 JMJ KOL.RED.2018.008
		if( _T("최대대출자료") == sStaticsClass || _T("최대대출자료(책두레포함"))
		{
			strBestLoanBookWhere = sWhere;
		}
		else
		{
			if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
				sOutWhere += _T(" AND ") + sWhere;
			else
				sInWhere += _T(" AND ") + sWhere;
		}
	}
	
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , _T("주소") , _T("값") , sValue );
	if( !sValue.IsEmpty() )		
	{
		sFieldName = _T("CLU.H_ADDR1");
		
		sPrintSearchWhere += _T("주소") + CString(_T(":")) + sValue + SPACE_STRING;
		
		sWhere.Format( _T(" %s LIKE '%%%s%%' ") , sFieldName , sValue );
		
		// 18.09.27 JMJ KOL.RED.2018.008
		if( _T("최대대출자료") == sStaticsClass || _T("최대대출자료(책두레포함)") == sStaticsClass )
		{
			if( strBestLoanBookWhere.GetLength() )
			{
				strBestLoanBookWhere += _T(" AND ");
			}
			strBestLoanBookWhere += sWhere;
		}
		else
		{
			if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
				sOutWhere += _T(" AND ") + sWhere;
			else
				sInWhere += _T(" AND ") + sWhere;
		}
	}

	// 18.09.27 JMJ KOL.RED.2018.008
	if( _T("최대대출자료") == sStaticsClass || _T("최대대출자료(책두레포함)") == sStaticsClass )
	{
		if( strBestLoanBookWhere.GetLength() )
		{
			strBestLoanBookWhere = _T(" CL.USER_KEY IN ( SELECT REC_KEY FROM CO_LOAN_USER_TBL CLU WHERE ") + strBestLoanBookWhere + _T(" ) ");

			if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
				sOutWhere += _T(" AND ") + strBestLoanBookWhere;
			else
				sInWhere += _T(" AND ") + strBestLoanBookWhere;
		}		
	}

	// 대출 형태 , 반납형태
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , _T("대출형태") , _T("값") , sValue );
	
	if( sValue.Compare(_T("전체")) != 0 )
	{

		if( !sValue.IsEmpty() )		
		{
			sFieldName = _T("CL.LOAN_TYPE_CODE");
					
			sPrintSearchWhere += _T("대출형태") + CString(_T(":")) + sValue + SPACE_STRING;
			
			if( sValue.Compare(_T("일반")) == 0 )
				sValue = _T("0");
			else if( sValue.Compare(_T("특별")) == 0 )
				sValue = _T("1");
			else if( sValue.Compare(_T("관내")) == 0 )
				sValue = _T("2");
			else if( sValue.Compare(_T("무인")) == 0 )
				sValue = _T("3");
			// 2005.09.27 ADD BY PDW(Jr) +++++++++++++++++++++++
			// 장기 상태 추가 
			else if( sValue.Compare(_T("장기")) == 0 )
				sValue = _T("4");
			// _________________________________________________

			sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue );
			
			if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
				sOutWhere += _T(" AND ") + sWhere;
			else
				sInWhere += _T(" AND ") + sWhere;

		}
	}
	
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , _T("반납형태") , _T("값") , sValue );
	
	if( sValue.Compare(_T("전체")) != 0 )
	{
		if( !sValue.IsEmpty() )
		{
		
			sFieldName = _T("CL.RETURN_TYPE_CODE");
			
			sPrintSearchWhere += _T("반납형태") + CString(_T(":")) + sValue + SPACE_STRING;
			
			if( sValue.Compare(_T("일반")) == 0 )
				sValue = _T("0");
			else if( sValue.Compare(_T("일괄")) == 0 )
				sValue = _T("1");
			else if( sValue.Compare(_T("소급")) == 0 )
				sValue = _T("2");
			else if( sValue.Compare(_T("무인")) == 0 )
				sValue = _T("3");
			
			sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue );
			
			if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
				sOutWhere += _T(" AND ") + sWhere;
			else
				sInWhere += _T(" AND ") + sWhere;
		}		
		
	}
	
	// 2007.06.22 ADD BY PDW +++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 대출형태 추가
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , _T("대출상태") , _T("값") , sValue );
	
	if( sValue.Compare(_T("전체")) != 0 )
	{
		if( !sValue.IsEmpty() )
		{
		
			sFieldName = _T("CL.STATUS");
			
			sPrintSearchWhere += _T("대출상태") + CString(_T(":")) + sValue + SPACE_STRING;
			
			if( sValue.Compare(_T("대출")) == 0 )
				sValue = _T("0");
			else if( sValue.Compare(_T("반납")) == 0 )
				sValue = _T("1");
			else if( sValue.Compare(_T("반납연기")) == 0 )
				sValue = _T("2");
			else if( sValue.Compare(_T("예약")) == 0 )
				sValue = _T("3");
			else if( sValue.Compare(_T("예약취소")) == 0 )
				sValue = _T("4");

			sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue );
			
			if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
				sOutWhere += _T(" AND ") + sWhere;
			else
				sInWhere += _T(" AND ") + sWhere;
		}		
		
	}
	// ---------------------------------------------------------------------------

	// 자료실 조건
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , _T("자료실_조건") , _T("값") , sValue );

	if( !sValue.IsEmpty() )
	{
		CStringArray saShelfLocCode;
		CLocCommonAPI::MakeArrayList( sValue , _T("|") , saShelfLocCode );
		
		if( flag )
			sFieldName = _T("CLU.SHELF_LOC_CODE");
		else
			sFieldName = _T("CL.SHELF_LOC_CODE");
		
		// 2012.10.25 ADD BY BJS : 2012년도 도서관 통합서비스 확대구축 사업 2차 (미대출자 통계에서 자료실 조건 반영 안되는거 수정)
		if (_T("미대출자 통계")==sStaticsClass )
		{
			sFieldName = _T("CLU.SHELF_LOC_CODE");
		}

		sPrintSearchWhere += _T("자료실:") ;

		for( INT m = 0 ; m < saShelfLocCode.GetSize() ; m++ )
		{
			if( !sWhere.IsEmpty() ) sWhere += _T(",");
			sWhere += _T("'") + saShelfLocCode.GetAt(m) + _T("'");

			sValue = saShelfLocCode.GetAt(m);

			m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분") , sValue , sValue );
			if( m != 0 ) sPrintSearchWhere += _T(",");
			sPrintSearchWhere += sValue;
		}

		sPrintSearchWhere += SPACE_STRING;

		sWhere = sFieldName + _T(" IN (") + sWhere + _T(")");

		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
		
	}



	// 등록일,대출일,반납일,반납예정일
	sWhere.Empty();
	const INT nCnt3 = 12;
	TCHAR *sControlAlias3[nCnt3] =
	{
		_T("등록일1") , _T("등록일2") , _T("대출일1") , _T("대출일2") , 
		_T("반납일1") , _T("반납일2") , _T("예약일1") , _T("예약일2") , 
		_T("예약취소일1"), _T("예약취소일2"),
		_T("반납예정일1") , _T("반납예정일2") 
	};

	TCHAR *sDBAlias3[nCnt3] =
	{
		_T("등록일") , _T("등록일") , _T("대출일") , _T("대출일") , 
		_T("반납일") , _T("반납일") , _T("예약일") , _T("예약일") ,
		_T("예약취소일") , _T("예약취소일"),
		_T("반납예정일") , _T("반납예정일") 
	};

	CString sValue1,sValue2;	

	// 등록일 , 대출일 , 반납일
	for( i = 0 ; i < nCnt3 ; i++ )
	{
		sWhere.Empty();
		sValue1.Empty(); sValue2.Empty();
	
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , sDBAlias3[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

		GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias3[i] , _T("값") , sValue1 );
		GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias3[i+1] , _T("값") , sValue2 );

		if( !sValue1.IsEmpty() )
		{
			if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
			sPrintSearchWhere += sDBAlias3[i] + CString(_T(":")) + sValue1;

			/* 2007.06.17 add by pdw
			*/
			// 18.09.27 JMJ KOL.RED.2018.008
			if ((_T("최대대출자료")==sStaticsClass && _T("등록일")==sDBAlias3[i]) || (_T("최대대출자료(책두레포함)")==sStaticsClass && _T("등록일")==sDBAlias3[i]) )
			{
				sFieldName=_T("BB.REG_DATE");
			}
			// 2012.10.25 ADD BY BJS : 2012년도 도서관 통합서비스 확대구축 사업 2차 (회원증 발급횟수통계 추가에 따른 쿼리 변경)
			else if (_T("회원증발급통계")==sStaticsClass )
			{
				sFieldName = _T("CM.REISSUE_DATE");
			}
			///////////////////////////

	
			if( !sValue2.IsEmpty() )
			{
				sWhere.Format( _T(" %s BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
				sPrintSearchWhere += _T("~") + sValue2;
			}
			else
				sWhere.Format( _T(" %s BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
			
			sPrintSearchWhere += SPACE_STRING;			
		}

		if( !sWhere.IsEmpty() )
		{
			// 18.09.27 JMJ KOL.RED.2018.008
			//if( _T("최대대출자료")==sStaticsClass && _T("등록일")==sDBAlias3[i] ) 
			if( (_T("최대대출자료")==sStaticsClass && _T("등록일")==sDBAlias3[i]) || (_T("최대대출자료(책두레포함)")==sStaticsClass && _T("등록일")==sDBAlias3[i]) ) 
			{
				sOutWhere += _T(" AND ") + sWhere;
			}
			else
			{
				if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
					sOutWhere += _T(" AND ") + sWhere;
				else
					sInWhere += _T(" AND ") + sWhere;
			}
		}

		i++;
	}

	// 2011.10.04 ADD BY PWR : 통합회원여부 추가
	// 18.09.27 JMJ KOL.RED.2018.008
	if( _T("최대대출자료") != sStaticsClass && _T("최대대출자료(책두레포함)") != sStaticsClass )
	{
		GetCellData(m_pUserSearchCond, _T("컨트롤알리아스"), _T("통합회원여부"), _T("값"), sValue);
		if(!sValue.IsEmpty() && _T("전체") != sValue)
		{
			if(_T("일반회원") == sValue)
			{
				sWhere.Format(_T(" (CLU.KL_MEMBER_YN IS NULL OR CLU.KL_MEMBER_YN='N') "), sFieldName, sFieldName);
				if(!sPrintSearchWhere.IsEmpty()) sPrintSearchWhere += _T(",");
				sPrintSearchWhere += _T("통합회원여부:일반회원 ");
			}
			else if(_T("통합회원") == sValue)
			{
				sWhere.Format(_T(" CLU.KL_MEMBER_YN='Y' "), sFieldName);
				if(!sPrintSearchWhere.IsEmpty()) sPrintSearchWhere += _T(",");
				sPrintSearchWhere += _T("통합회원여부:통합회원 ");
			}
			if(1 == IsFieldFromTable(_T("CLU.KL_MEMBER_YN"), saTableName))
			{
				sOutWhere += _T(" AND ") + sWhere;
			}
			else
			{
				sInWhere += _T(" AND ") + sWhere;
			}
		}
	}

	// 2009.03.27 ADD BY KSJ : 검색조건이 길 경우 화면 출력에서 짤리는 문제로 인하여 조건이 일정 길이 이상이면 개행문자를 추가한다.
	INT		nWhereLength	=	sPrintSearchWhere.GetLength();
	if(		_T("최대대출자료")		== sStaticsClass 
		// 18.09.27 JMJ KOL.RED.2018.008
		// 최대대출자료에 책두레 포함
		||	_T("최대대출자료(책두레포함)")		== sStaticsClass 
		||	_T("다독자")			== sStaticsClass 
		||  _T("가족별다독자")		== sStaticsClass	
		||  _T("연체자통계")	== sStaticsClass	)
	{
		if( 52 < nWhereLength )
		{
			CString sLeft, sRight;
			sLeft = sPrintSearchWhere.Left(52);
			sRight = sPrintSearchWhere.Mid(52);
			sPrintSearchWhere = sLeft + _T("\r\n") + sRight;
		}
	}

	return 0;

	EFS_END
	return -1;

}

// 2009.01.06 ADD BY PWR
// ** 책두레통계에서 안조건과 밖조건구하기
INT CStaticsInfoMgr::GetSearchWhereCooperative( CString sTableName , CString &sInWhere , CString &sOutWhere , CString &sPrintSearchWhere )
{
EFS_BEGIN

	INT ids;
	CStringArray saTableName;
	CString sDBField;

	ids = CLocCommonAPI::MakeArrayList( sTableName , _T(",") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );

	CString sStaticsClass;
	GetControlData( _T("CM_COOPERATIVE_STATISTICS_MAIN") , _T("통계종류") , sStaticsClass , -1 , 0 );

	// 관리구분, 진행상황
	const INT nCnt1 = 2;
	TCHAR *sControlAlias1[nCnt1] =
	{
		_T("관리구분"), _T("진행상태")
	};
	INT sFieldType1[nCnt1] =
	{
		ONESTRING, ONESTRING
	};

	CString sWhere;
	CString sValue;
	CString sFieldName;
	for( INT i = 0 ; i < nCnt1 ; i++ )
	{
		sValue.Empty();
		sWhere.Empty();

		if(sControlAlias1[i] == _T("관리구분"))
		{
			// 18.09.27 JMJ KOL.RED.2018.008
			if(sStaticsClass==_T("제공자료통계(발송)"))
			{
				sControlAlias1[i] = _T("제공관리구분");
			}
			//대출자료통계에서도 신청관리구분 참조하도록 반영
			else if(sStaticsClass==_T("요청자료통계(입수)") || sStaticsClass==_T("대출 자료 통계") ||  sStaticsClass==_T("대출자료통계") || sStaticsClass==_T("대출자통계") )
 			{
				sControlAlias1[i] = _T("신청관리구분");
			}
			//반납자 통계 여기로 이동
			else if(sStaticsClass==_T("반납자료통계") || sStaticsClass==_T("반납자통계") )
			{
				sControlAlias1[i] = _T("반납관리구분");
			}
			else
				sControlAlias1[i] = _T("관리구분");
		}

		GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias1[i] , _T("값") , sValue );

		if( sValue.IsEmpty() ) continue;
		// 적용안함이면 조건에 추가안한다.
		if(sValue == _T("----")) continue;

		// 필드명 가져오기
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , sControlAlias1[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetSearchWhere1110") );		

		if( sFieldType1[i] == ONESTRING )
			sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue);
		else if( sFieldType1[i] == ONENUMERIC )
			sWhere.Format( _T(" %s = %s ") , sFieldName , sValue );

		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
		
		/*
		if( i == 0 )// 분류기호구분
			m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("분류표구분") , sValue , sValue );
		else		// 관리구분
			m_pInfo->pCodeMgr->ConvertCodeToDesc( sControlAlias1[i] , sValue , sValue );
		*/
		m_pInfo->pCodeMgr->ConvertCodeToDesc( sControlAlias1[i] , sValue , sValue );

		sPrintSearchWhere += sControlAlias1[i] + CString(_T(":")) + sValue + SPACE_STRING;
	}

	// 신청일, 제공일, 등록일
	sWhere.Empty();
//	const INT nCnt3 = 4;
	// 18.09.27 JMJ KOL.RED.2018.008
	// 책두레통계 입수일 추가
	const INT nCnt3 = 10;
	TCHAR *sControlAlias3[nCnt3] =
	{
		_T("신청일1") , _T("신청일2") , _T("제공일1") , _T("제공일2"),
		_T("책두레대출일1") , _T("책두레대출일2") , _T("책두레반납일1") , _T("책두레반납일2"),
		_T("입수일1") , _T("입수일2")

//		_T("신청일1") , _T("신청일2") , _T("제공일1") , _T("제공일2")
	};

	TCHAR *sDBAlias3[nCnt3] =
	{
		_T("신청일") , _T("신청일") , _T("제공일") , _T("제공일"),
		_T("책두레대출일") , _T("책두레대출일") , _T("책두레반납일") , _T("책두레반납일"),
		_T("입수일") , _T("입수일")
//		_T("신청일") , _T("신청일") , _T("제공일") , _T("제공일")
	};

	CString sValue1,sValue2;
	for( i = 0 ; i < nCnt3 ; i++ )
	{
		sWhere.Empty();
		sValue1.Empty(); sValue2.Empty();
	
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , sDBAlias3[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

		GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias3[i] , _T("값") , sValue1 );
		GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias3[i+1] , _T("값") , sValue2 );

		if( !sValue1.IsEmpty() )
		{
			if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += _T(",");
			sPrintSearchWhere += sDBAlias3[i] + CString(_T(":")) + sValue1;

			if( !sValue2.IsEmpty() )
			{
				sWhere.Format( _T(" %s BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
				sPrintSearchWhere += _T("~") + sValue2;
			}
			else
				sWhere.Format( _T(" %s BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
			
			sPrintSearchWhere += SPACE_STRING;			
		}

		if( !sWhere.IsEmpty() )
		{
			if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
				sOutWhere += _T(" AND ") + sWhere;
			else
				sInWhere += _T(" AND ") + sWhere;
		}

		i++;
	}

	return 0;

EFS_END
return -1;
}

// ##############################################################################################################
// [ 함수설명 ]
//   메뉴 알리아스를 리턴한다.
//   멤버만을 리턴중이지만 차후 기능개선 가능성을 위해 독립된 함수로 추출
//
// [ 미니스펙 ]
//
// ##############################################################################################################
CString CStaticsInfoMgr::GetMenuName()
{
EFS_BEGIN

	return m_strMenuName;

EFS_END
	return _T("");
}

INT CStaticsInfoMgr::GetSearchWhere1440( CString sTableName , CString &sInWhere , CString &sOutWhere , CString &sPrintSearchWhere )
{
	EFS_BEGIN

	INT ids;

	// 검색 조건 - 이거 노가다 어쩔수 없당 ㅡㅜ
	CStringArray saTableName;
	CString sDBField;

	ids = CLocCommonAPI::MakeArrayList( sTableName , _T(",") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("") );

	// 차수년도
	const INT nCnt1 = 1;
	TCHAR *sControlAlias1[nCnt1] =
	{
		_T("차수년도")
	};
	INT sFieldType1[nCnt1] =
	{
		ONESTRING 
	};


	CString sWhere;
	CString sValue;
	CString sFieldName;
	for( INT i = 0 ; i < nCnt1 ; i++ )
	{
		sValue.Empty();
		sWhere.Empty();

		GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias1[i] , _T("값") , sValue );

		if( sValue.IsEmpty() ) continue;

		// 필드명 가져오기
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , sControlAlias1[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

		if( sFieldType1[i] == ONESTRING )
			sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue);
		else if( sFieldType1[i] == ONENUMERIC )
			sWhere.Format( _T(" %s = %s ") , sFieldName , sValue );

		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;

		sPrintSearchWhere += sControlAlias1[i] +CString(_T(":"))+ sValue + SPACE_STRING;
		
		 
	}


	// 차수
	sWhere.Empty();
	const INT nCnt3 = 2;
	TCHAR *sControlAlias3[nCnt3] =
	{
		_T("차수1") , _T("차수2") 
	};


	CString sValue1,sValue2;
	sValue1.Empty(); sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias3[0] , _T("값") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias3[1] , _T("값") , sValue2 );


	// 원부번호
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , _T("차수") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetBoSearchWhere") );

	if( !sValue1.IsEmpty() )
	{
		sPrintSearchWhere += _T("차수:")+sValue1;
		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN %s AND %s ") , sFieldName , sValue1 , sValue2 );		
			sPrintSearchWhere += _T("~")+sValue2;
		}
		else
			sWhere.Format( _T(" %s = %s ") , sFieldName , sValue1 );
		sPrintSearchWhere += SPACE_STRING;
	}

	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}
	return 0;

	EFS_END

	return -1;

}

// ##############################################################################################################
// [ 함수설명 ]
//   결과 DM을 출력한다.
//
// [ 미니스펙 ]
// ##############################################################################################################
INT CStaticsInfoMgr::PrintResult( CString sStaticsClass , bool bDupDel , bool bDelZero  , CTabCtrl* pTab )
{
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. 순위, 일반 통계 구별
	// ==========================================================================================================
	CString sXAlias , sYAlias , sOption , sOrder;
	ids = GetXYAlias( sStaticsClass , sXAlias , sYAlias , sOption , sOrder );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::PrintResult()") );	

	if( sOrder.Compare( _T("-1") ) == 0 )
	{
		ids = MakeStatPrintDM( sStaticsClass , bDupDel , bDelZero );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::PrintResult()") );
		if( ids > 0 ) return ids;
	}
	else
	{	
		ids = MakeRankingPrintDM( sStaticsClass , pTab );
		if( ids > 0 ) return ids;
	}
	
	return 0;
EFS_END
	return -1;
}


// ##############################################################################################################
// [ 함수설명 ]
//   일반 통계 결과 DM을 출력한다.
//
// [ 미니스펙 ]
// ##############################################################################################################
INT CStaticsInfoMgr::MakeStatPrintDM( CString sStaticsClass , bool bDupDel , bool bDelZero )
{
EFS_BEGIN

	INT ids;
	CString sXAlias,sYAlias,sPrintName,sPrintTitle,sSearchCond;	

	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("X축선택알리아스") , sXAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeStatPrintDM()") );
	
	// 2012.11.20 ADD BY BJS : 2012년도 도서관 통합서비스 확대구축 사업 2차 (개인정보보호 실명인증 사용에 따른 성별 통계 출력)
	if(sXAlias.Compare(_T("성별2"))==0)	sXAlias = _T("성별");

	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("Y축선택알리아스") , sYAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeStatPrintDM()") );
	
	CESL_DataMgr* pPrintDM;
	// UPDATE BY 김용배 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 장서점검 통계에 가격이 추가되면서 따로 처리해 준다.	
	if( _T("장서점검통계") == sStaticsClass )
	{
		pPrintDM = m_pPrintMgr->GetPrintDM( _T("장서관리_분류기호별") );
		MakePrintDMFromGrid( pPrintDM );
	}
	else
	{
		pPrintDM = m_pPrintMgr->GetPrintDM( sXAlias );
		if( !pPrintDM ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::MakeStatPrintDM()") );

		ids = MakePrintDMFromResultDM( sStaticsClass , bDupDel , pPrintDM , sXAlias , bDelZero );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::MakeStatPrintDM()") );
		if( ids > 0 ) return ids;
	}
	// ----------------------------------------------------------------------------------------------------------

	// UPDATE BY 김용배 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 장서점검 통계에 가격추가하며 rpt 파일 변경
	if( _T("장서점검통계") == sStaticsClass )
		sPrintName.Format( _T("장서점검_분류기호별통계") );
	// 2006-04-07 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 연속 통계 분리
	else
	{
		sPrintName.Format( _T("열람통계_%s통계") , sXAlias );
		if ( m_strMenuName == SE_HOLDING_STATICS )
			sPrintName.Format( _T("열람통계_연속_%s통계") , sXAlias );
	}
	// ----------------------------------------------------------------------------------------------------------

	if( sXAlias.Compare(_T("년")) == 0 || (sXAlias.Compare(_T("월")) == 0) || (sXAlias.Compare(_T("일")) == 0) )
	{		
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("년월일") , sXAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::MakeStatPrintDM()") );
	}	sPrintTitle.Format( _T("행 : '%s' , 열 : '%s'") , sYAlias , sXAlias );
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("사용자검색조건_출력") , sSearchCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::MakeStatPrintDM()") );

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return 1;
	}
	
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);

	if( m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000 )
	{
		CString sRptFileFullPath;
		sRptFileFullPath.Format(_T("..\\rpt\\%s.rpt") , sPrintName );
		
		ids = pSIReportManager->InitWorkFormFile( sRptFileFullPath , sStaticsClass , 1 );
		if (ids < 0) {
			CString sEMsg;
			sEMsg.Format(_T("File로 부터 SI Report 정보를 가져오지 못하였습니다.[%s]") , sRptFileFullPath);
			AfxMessageBox(sEMsg);
			return 2;
		}
	}
	else
	{
		ids = pSIReportManager->GetSIReportFileFromDB(sPrintName);
		if (ids < 0) {
			AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
			return 1;
		}	
	}

	///{{++
	/// 2008.04.17 update by pdw
	/// 통계 제목을 변경할수 있도록 한다
//	pSIReportManager->SetTempVariable( _T("통계제목") , sStaticsClass );	
	GetControlData(_T("CM_BO_STATICS_1100") , _T("통계제목") , sStaticsClass , -1 , 0 );
	pSIReportManager->SetTempVariable( _T("통계제목") , sStaticsClass );
	///}}--

	pSIReportManager->SetTempVariable( _T("통계부제") , sPrintTitle );
	pSIReportManager->SetTempVariable( _T("열") , sXAlias );
	pSIReportManager->SetTempVariable( _T("행") , sYAlias );
	pSIReportManager->SetTempVariable( _T("검색조건") , sSearchCond );
	
	pSIReportManager->SetDataMgr(0, pPrintDM , 0);
	
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	

	return 0;

EFS_END
	return -1;
}

// 결과를 그리드에 뿌려주기
INT CStaticsInfoMgr::MakePrintDMFromResultDM( CString sStaticsClass , bool bDupDel , CESL_DataMgr* pTargetDM , CString sXAlias , bool bDelZero )
{
	EFS_BEGIN

	INT ids;

	pTargetDM->FreeData();
	for( INT i = 0 ; i < pTargetDM->RefList.GetCount() ; i++ )
	{
		pTargetDM->SetDataMgrRef( i , _T("-1") , _T("") , _T("") , _T("") , _T("") );
	}
	
	if( m_pRealResultDM->GetRowCount() < 1 ) return 0;

	CString sSumAlias;
	if( !bDupDel )
		sSumAlias = _T("중복허용_합계");
	else
		sSumAlias = _T("중복삭제_합계");

	INT nLimitCnt = pTargetDM->RefList.GetCount();
	INT nStatColCnt = m_pRealResultDM->RefList.GetCount();

	// DM 메모리 구성하기
	CString sTmpSum;
	for( i = 0 ; i < m_pRealResultDM->GetRowCount()+2 ; i++ )
	{
		// 합계가 0인 행 입력안하기
		if( !bDelZero )
		{
			if( i !=0 && i != m_pRealResultDM->GetRowCount()+1 )
			{
				m_pXSumDM->GetCellData( sSumAlias , i-1 , sTmpSum );
				if( _ttoi(sTmpSum) == 0 )
					continue;
			}
		}
		pTargetDM->InsertRow(-1);
	}


	// X축 컬럼 뿌려주기
	INT refcount = m_pRealResultDM->RefList.GetCount();
	CESL_DataMgr::reference * pRef;
	POSITION pos;
	pos = m_pRealResultDM->RefList.GetHeadPosition();
	CString query, fields, isudf;

	CString sResult;
	CString sColumnAlias;

	INT nCopyColCnt = nStatColCnt;
	if( nStatColCnt > nLimitCnt )
	{
		CString sMsg;
		sMsg.Format(_T("열의 갯수가 너무 많아 출력물에 모두 표시할수 없습니다.\r\n나머지 부분은 [%d]번째 이상부터 재산출해 출력을 해야합니다.") , nLimitCnt-1 );
		AfxMessageBox( sMsg );
		nCopyColCnt = nLimitCnt;
	}
	
	INT nInsertIndex;
	for( i = 0 ; i < nCopyColCnt ; i++ )
	{
		pRef = NULL;
		pRef = (CESL_DataMgr::reference*)m_pRealResultDM->RefList.GetNext(pos);

		sResult = pRef->FIELD_ALIAS;
		sResult.TrimLeft(); sResult.TrimRight();

		if( sResult.IsEmpty() || (sResult.Compare(_T("적용안함")) == 0)  )
		{
			sResult = _T("적용안함");
		}

		sColumnAlias.Format(_T("%d"),i);
		pTargetDM->SetDataMgrRef( i , sColumnAlias , _T("") , _T("") , _T("") , _T("") );
			
		if( sResult.Compare(_T("컬럼명")) == 0 )
			sResult = _T("");
		else
		{
			if( sXAlias.Compare(_T("일")) == 0 )
			{
				sResult = sResult.Right(2);
			}
		}
		pTargetDM->SetCellData( 0 , i , sResult );

		nInsertIndex = 1;
		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{

			ids = m_pXSumDM->GetCellData( sSumAlias , j , sTmpSum );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

			if( !bDelZero && _ttoi(sTmpSum) == 0 )
				continue;
			
			ids = m_pRealResultDM->GetCellData( pRef->FIELD_ALIAS , j , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

			if( sResult.Compare(_T("적용안함")) == 0 )
			{
				sResult = _T("적용안함");
			}

			pTargetDM->SetCellData( nInsertIndex , i , sResult );

			nInsertIndex++;
		}

	}



	// 세로 합계 뿌려주기
	pTargetDM->SetCellData( nInsertIndex , 0 , _T("합계") );	
	for( i = 0 ; i < nCopyColCnt ; i++ )
	{
		for( INT j = 0 ; j < nCopyColCnt-1 ; j++ )
		{
			ids = m_pYSumDM->GetCellData( sSumAlias , j , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

			pTargetDM->SetCellData( nInsertIndex , j+1 , sResult );
		}
	}


	// 가로 합계 뿌려주기
//	INT nSumCol = m_pRealResultDM->RefList.GetCount();
	INT nSumCol = nCopyColCnt;
	sColumnAlias.Format( _T("%d") , nLimitCnt-1 );
	pTargetDM->SetCellData( 0 , nLimitCnt-1 , _T("합계") );
	pTargetDM->SetDataMgrRef( nLimitCnt-1 , sColumnAlias , _T("") , _T("") , _T("") , _T("") );
	
	CString sSum;
	nInsertIndex = 1;
	for( i = 0 ; i < m_pXSumDM->GetRowCount() ; i++ )
	{
		ids = m_pXSumDM->GetCellData( sSumAlias , i , sSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("StaticsResultViewGrid") );

		// 16/07/20 김혜영 : 대출자료통계/반납자료통계시 출력이 되지 않는 오류 수정
/*// BEFORE ----------------------------------------------------------------------------------
		if( !bDelZero && _ttoi(sSum) == 0 )
*/// AFTER ----------------------------------------------------------------------------------
		if( !bDelZero && _ttoi(sSum) == 0  && !((_T("대출 자료 통계") == sStaticsClass || _T("반납자료통계") == sStaticsClass) && pTargetDM->GetRowCount() <= 2 ) )
//*/ END -------------------------------------------------------------------------------------	
			continue;

		pTargetDM->SetCellData( nInsertIndex , nLimitCnt-1 , sSum );
		nInsertIndex++;
	}

	
	
	
	/*
	if( _T("장서점검통계") == sStaticsClass )
	{
		// 가로 가격 합계
		pTargetDM->SetCellData( 0, 12, _T("가격합계") );
		for( i = 0; i < m_pXPriceSumDM->GetRowCount(); i++ )
			pTargetDM->SetCellData( i + 1, 12, m_pXPriceSumDM->GetCellData( _T("중복삭제_합계"), i ) );
		
		// 세로 가격 합계
		pTargetDM->InsertRow( - 1 );
		INT lastRow = pTargetDM->GetRowCount();
		pTargetDM->SetCellData( lastRow, 0, _T("가격합계") );
		for( i = 0; i < m_pYPriceSumDM->GetRowCount(); i++ )
			pTargetDM->SetCellData( lastRow, i + 1 , m_pYPriceSumDM->GetCellData( _T("중복삭제_합계"), i ) );
	}*/
	
	
	
	
	
	
	
	return 0;

	EFS_END
	return -1;

}


INT CStaticsInfoMgr::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

	EFS_END
	return -1;

}



/// 출력물 DM구성
INT CStaticsInfoMgr::MakeRankingPrintDM( CString sStaticsClass , CTabCtrl* pTab )
{
	EFS_BEGIN
		
	CStringArray saTableName;
	CString sTableName;
	INT ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("검색테이블이름") , sTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeOrderStatics") );
		
	ids = CLocCommonAPI::MakeArrayList( sTableName , _T("/") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitRunStatics") );
		
	CESL_DataMgr* pSrcDM;
	CESL_DataMgr* pPrintDM;

	pPrintDM = m_pPrintMgr->GetPrintDM( sStaticsClass );
	if( pPrintDM == NULL )
	{
		AfxMessageBox(_T("지원되는 출력물이 존재하지 않습니다."));
		return 1;
	}
	
	CString sPrintTitle,sSearchCond,sOption;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("사용자검색조건_출력") , sSearchCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakePrintDM") );
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("단위") , sOption );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakePrintDM") );


	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return 1;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);

	pSIReportManager->m_nDefaultPaperType = PAPER_A4_PORTRAIT;

	if( m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000 )
	{
		CString sRptFileFullPath;
		sRptFileFullPath.Format(_T("..\\rpt\\%s.rpt") , sStaticsClass );
		
		ids = pSIReportManager->InitWorkFormFile( sRptFileFullPath , sStaticsClass , 1 );
		if (ids < 0) {
			CString sEMsg;
			sEMsg.Format(_T("File로 부터 SI Report 정보를 가져오지 못하였습니다.[%s]") , sRptFileFullPath);
			AfxMessageBox(sEMsg);
			return 2;
		}
	}
	else
	{
		ids = pSIReportManager->GetSIReportFileFromDB(sStaticsClass);
		if (ids < 0) {
			AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
			return 2;
		}
		
	}


	pSIReportManager->SetDataMgr( 0 , pPrintDM , 0 );

	ids = pSIReportManager->DeleteTempPageFiles();

	// 2007.06.20 update by pdw
	// 대출횟수통계 예외처리	
	// 18.09.27 JMJ KOL.RED.2018.008
	if( _T("대출횟수통계")==sStaticsClass || _T("대출횟수통계(책두레포함)")==sStaticsClass )
	{
		///{{++
		/// 2008.04.17 update by pdw
		/// 통계 제목을 변경할수 있도록 한다
	//	pSIReportManager->SetTempVariable( _T("통계제목") , sStaticsClass );	
		GetControlData(_T("CM_BO_STATICS_1100") , _T("통계제목") , sStaticsClass , -1 , 0 );
		pSIReportManager->SetTempVariable( _T("통계제목") , sStaticsClass );
		///}}--
		pSIReportManager->SetTempVariable( _T("검색조건") , _T("검색조건 : ") + sSearchCond );

		ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);		// 워크폼 Manual 반복 모드
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("PrintUserList") );
	}
	else
	{
		CStringArray asColumnName;
		if( sOption.Compare(_T("전체")) != 0 )
		{
			ids = GetColumnNames( sStaticsClass , sOption , asColumnName );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeOrderStaticsDM") );
		}
		else
			asColumnName.Add(sOption);

		for( INT i = 0 ; i < asColumnName.GetSize() ; i++ )
		{
			pSrcDM = &m_pOrderStaticsDM[saTableName.GetSize()][i];
			
			CLocCommonAPI::AddDM( pSrcDM , pPrintDM , this );

			if( pPrintDM->GetRowCount() == 0 ) continue;

			if(sOption.Compare(_T("전체")) != 0 )
				sPrintTitle.Format( _T("단위 : %s(%s)") , sOption , asColumnName.GetAt(i) );
			else
				sPrintTitle.Format( _T("") );

			if ( sPrintTitle == _T("") )
			{			
				pSIReportManager->SetTempVariable( _T("통계단위제목") , sStaticsClass );
			}
			else
			{
				pSIReportManager->SetTempVariable( _T("통계단위제목") , sPrintTitle );
			}

			///{{++
			/// 2008.04.17 add by pdw
			/// 통계 제목을 변경할수 있도록 한다
			CString strTitle;
			GetControlData(_T("CM_BO_STATICS_1100") , _T("통계제목") , strTitle , -1 , 0 );
			pSIReportManager->SetTempVariable( _T("통계제목") , strTitle );
			///}}--

			pSIReportManager->SetTempVariable( _T("검색조건") , _T("검색조건 : ") + sSearchCond );

			ids = pSIReportManager->MAKEPAGE_DrawWorkForm(-1);		// 워크폼 Manual 반복 모드
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("PrintUserList") );

			pPrintDM->FreeData();
		}
	}

	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
		
	return 0;
	
	EFS_END
		
	return -1;
	
}


// 그리드의 모든 셀을 가운데 정렬로 
INT CStaticsInfoMgr::SetGridCellAlignment(CMSHFlexGrid* pGrid)
{
	EFS_BEGIN

	INT nColCnt = pGrid->GetCols(0);
	INT nRowCnt = pGrid->GetRows();

	
	
	for( INT i = 0 ; i < nColCnt ; i++ )
	{
		if( i == 0 )
			pGrid->SetColAlignment(i,1);
		else
			pGrid->SetColAlignment(i,4);

		pGrid->SetRow(0);
		pGrid->SetCol(i);
		pGrid->SetCellFontBold(TRUE);
		pGrid->SetCellAlignment(4);

	}

	for( INT j = nRowCnt-1 ; j >= 0 ; j-- )
	{
		pGrid->SetCol(0);
		pGrid->SetRow(j);
		pGrid->SetCellFontBold(TRUE);	
	}

	return 0;

	EFS_END

	return -1;
}

CString CStaticsInfoMgr::ConvertPrice(CString price)
{
	INT len = price.GetLength();
	if( len < 4 )
		return price;

	INT cnt = 0;
	CString tmp;
	for( INT i = len -1; i > -1; i-- )
	{
		cnt++;
		tmp = price.Mid( i, 1 ) + tmp;

		if( 3 == cnt )
		{
			tmp = _T(",") + tmp;
			cnt = 0;
		}		
	}
	
	if( ',' == tmp.GetAt( 0 ) )
		price = tmp.Mid( 1, tmp.GetLength() - 1 );
	else
		price = tmp;
	
	return price;
}



INT CStaticsInfoMgr::MakePrintDMFromGrid( CESL_DataMgr *pDM )
{
	CString CMAlias = _T("CM_BO_LOC_2100_STATICS_GRID");
	CESL_ControlMgr *pCM = FindCM( CMAlias );

	CESL_Grid *pGrid = ( CESL_Grid * )( pCM->FindControl( _T("통계그리드") ) );

	INT cols = pGrid->GetCols( 0 );
	INT rows = pGrid->GetRows();

	pDM->FreeData();
	CString colAlias;
	for( INT i = 0; i < rows; i++ )
	{
		pDM->InsertRow( -1 );
		for( INT j = 0; j < cols; j++ )
		{
			colAlias.Format( _T("%d"), j );
			pDM->SetCellData( colAlias, pGrid->GetTextMatrix( i, j ), i );
		}
	}
	
	return 0;
}

// 2005.07.28 ADD BY PDJ
// 분류기호 3대의 일반과 군사분류기호를 분리하여 리턴
INT CStaticsInfoMgr::DividNormalArmy(CString strClassNoIn, CString &f_strNormalOut, CString &f_strArmyOut)
{
	INT	nStartPos, nEndPos;
	CString strNormal, strArmy;
	
	// ADD BY PDW(jr) 
	// 형식 변경 
	strClassNoIn.Replace( _T(" "), _T("") );
	nStartPos = strClassNoIn.FindOneOf( DIVCHAR );
	nEndPos	  = strClassNoIn.GetLength();
	if ( nStartPos == -1 )
	{					
		// 일반자료가 먼저일 경우
		strArmy = _T("0");
		strNormal = strClassNoIn;		
	}				
	else if (nStartPos == 0)
	{
		// 군사자료가 먼저일 경우	
		strNormal = _T("0");
		strArmy = strClassNoIn;		
	}
	else
	{
		// 조합이 되어 있는경우
		strNormal = strClassNoIn.Left(nStartPos);
		strArmy   = strClassNoIn.Mid(nStartPos+1, nEndPos-nStartPos-1);
	}

	if ( strNormal.GetLength()	== 0 ) strNormal=_T("0");
	if ( strArmy.GetLength()	== 0 ) strArmy=_T("0");

	f_strNormalOut = strNormal;
	f_strArmyOut = strArmy;	

	return 0;
}

INT CStaticsInfoMgr::MakeFunctionFieldName(CString strFuncNameIn, CString &strFieldNameOut)
{	
	INT nStartPos;
	INT nEndPos;

	if ( strFuncNameIn.Find(_T("$"), 0 ) > -1 )
	{	
		nStartPos = strFuncNameIn.Find(_T("("), 0 );
		nEndPos = strFuncNameIn.Find(_T(")"), 0 );			
		strFieldNameOut = strFuncNameIn.Mid(nStartPos+1, nEndPos-nStartPos-1);
	}
	else
	{
		strFieldNameOut = strFuncNameIn;
	}
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 소장종 통계에서 군사자료 관련 종/책 갯수를 설정하기 위해 
// 입력된 데이터에 해당 알리아스 부분의  책권수를 부여한다.
/////////////////////////////////////////////////////////////////////////////////////////////
INT CStaticsInfoMgr::DividNormalArmy2(CString sPreData, CString sXAlias, CString sData, INT nMode, CString *psGetData)
{
	CString sGetNormal, sGetArmy;
	///////////////////////////////////////////////////////////////////////
	// 1. 종정보를 입력
	///////////////////////////////////////////////////////////////////////
	if ( nMode == 0 )
	{		
		DividNormalArmy( sPreData, sGetNormal, sGetArmy);
		if ( sXAlias == _T("3") )
		{
			psGetData->Format( _T("%d %s %d"), _ttoi( sGetNormal ) + _ttoi( sData ), DIVCHAR, _ttoi( sGetArmy ) );
		}
		else
		{
			psGetData->Format( _T("%d %s %d"), _ttoi( sGetNormal ), DIVCHAR, _ttoi( sGetArmy ) + _ttoi( sData ) );
		}
	}
	///////////////////////////////////////////////////////////////////////
	// 2. 책정보를 입력
	///////////////////////////////////////////////////////////////////////
	else if ( nMode == 1 )
	{
		INT nPos = sPreData.Find( _T("\r\n") );
		CString sGetBoNormal, sGetBoArmy, sTemp;
		// 2.1) 새로 입력
		if ( nPos == -1 )
		{
			DividNormalArmy( sPreData, sGetNormal, sGetArmy);
			if ( sXAlias == _T("3") )
			{
				sGetBoNormal.Format( _T("%d") , _ttoi(sData) );
				sGetBoArmy = _T("0");
			}
			else
			{
				sGetBoNormal = _T("0");
				sGetBoArmy.Format( _T("%d") , _ttoi(sData) );
			}			
		}
		// 2.2) 기존정보 갱신
		else
		{
			sTemp = sPreData.Mid( nPos + 2 );
			sPreData = sPreData.Mid( 0, nPos );
			sTemp.Replace(_T("("),_T(""));sTemp.Replace(_T(")"),_T(""));
			sTemp.TrimLeft();sTemp.TrimRight();
			DividNormalArmy( sTemp, sGetNormal, sGetArmy);

			if ( sXAlias == _T("3") )
			{
				sGetBoNormal.Format( _T("%d") ,  _ttoi(sData) );						
				sGetBoArmy = sGetArmy;
			}
			else
			{		
				sGetBoArmy.Format( _T("%d") , _ttoi(sData) );
				sGetBoNormal = sGetNormal;
			}
		}

		sTemp.Format( _T("(%d %s %d)"), _ttoi( sGetBoNormal ), DIVCHAR, _ttoi( sGetBoArmy ) );
		psGetData->Format( _T("%s\r\n%s") , sPreData, sTemp );
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 육사 소장종 통계 합계를 위한 작업
//////////////////////////////////////////////////////////////////////////////////////////
INT CStaticsInfoMgr::MakeArmySpeciesBookCnt(CString sData, CString *sGetData)
{
	INT nPos = sData.Find( _T("\r\n") );
	
	//////////////////////////////////////////////////////////////////////////////////
	// 1. 한줄일경우
	//////////////////////////////////////////////////////////////////////////////////
	if ( nPos == -1 )
	{
		//ex) 5 / 6    둘다 종의 갯수다 -> 11로 리턴한다.
		// 스페이스바 제거 
		sData.Replace( _T(" "), _T("") );
		nPos = sData.Find( DIVCHAR, 0 );
		// 구분이 없다 
		if ( nPos == -1 ) 
			*sGetData = sData;
		else
		{
			CString sValue1, sValue2;
			sValue1 = sData.Mid( 0, nPos );
			sValue2 = sData.Mid( nPos + 1 );
			sGetData->Format( _T("%d"), _ttoi( sValue1 ) + _ttoi( sValue2 ) );
		}

	}
	//////////////////////////////////////////////////////////////////////////////////
	// 2. 두줄일경우
	//////////////////////////////////////////////////////////////////////////////////
	else
	{
		// ex)  5 / 6     종, 책을 합쳐서 11\r\n(13)으로 리턴한다.
		//    ( 6 / 7 ) 
		// 종책을 분리
		CString sValue1, sValue2;
	    CString sSpeices = sData.Mid( 0, nPos );
		CString sBook    = sData.Mid( nPos + 2 );

		// 종 카운트 합치기 
	    sSpeices.Replace( _T(" "), _T("") );
		nPos = sSpeices.Find( DIVCHAR, 0 );		
		if ( nPos != -1 ) 
		{
			sValue1 = sSpeices.Mid( 0, nPos );
			sValue2 = sSpeices.Mid( nPos + 1 );
			sSpeices.Format( _T("%d"), _ttoi( sValue1 ) + _ttoi( sValue2 ) );
		}

		// 책 카운트 합치기
		sBook.Replace( _T(" "), _T("") );
		sBook.Replace( _T("("), _T("") );sBook.Replace( _T(")"), _T("") );
		nPos = sBook.Find( DIVCHAR, 0 );
		if ( nPos != -1 ) 
		{
			sValue1 = sBook.Mid( 0, nPos );
			sValue2 = sBook.Mid( nPos + 1 );
			sBook.Format( _T("%d"), _ttoi( sValue1 ) + _ttoi( sValue2 ) );
		}

		sGetData->Format( _T("%s\r\n(%s)"), sSpeices, sBook );
	}

	return 0;
}

// ##############################################################################################################
// [ 함수설명 ]
//   기본 DB 조회 정보 설정
//
// [ 미니스펙 ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::SetManageValue()
{
	INT ids;

	// ==========================================================================================================
	// 1. 도서관 플래그 설정
	// ==========================================================================================================
	ids = GetManageValue( _T("기타") , _T("공통") , _T("육사") ,  _T("육사")  , m_sIs64lib );
	if ( ids < 0)	ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::SetManageValue()") );
	m_sIs64lib.TrimLeft();m_sIs64lib.TrimRight();

	ids = GetManageValue( _T("기타") , _T("공통") , _T("국방대") , _T("")     , m_sIsNDlib );
	if ( ids < 0)	ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::SetManageValue()") );
	m_sIsNDlib.TrimLeft();m_sIsNDlib.TrimRight();

	ids = GetManageValue( _T("기타") , _T("공통") , _T("5163") ,   _T("5163") , m_sIs5163lib );
	if ( ids < 0)	ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::SetManageValue()") );
	m_sIs5163lib.TrimLeft();m_sIs5163lib.TrimRight();

	// 2009.10.12 ADD BY PWR : 대출자번호 주민번호일때 *표시
	ids = GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), m_sIsCivilNoHide);
	if ( ids < 0)	ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::SetManageValue()") );
	m_sIsCivilNoHide.TrimLeft();m_sIsCivilNoHide.TrimRight();

	// 2010.09.08 ADD BY KSJ : 통합도서서비스 사용여부
	GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), m_sIsUnityLoanService );
	m_sIsUnityLoanService.TrimLeft();	m_sIsUnityLoanService.TrimRight();
	m_sIsUnityLoanService.MakeUpper();
	if(_T("Y")!=m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");

	return 0;	
}

// ##############################################################################################################
// [ 함수설명 ]
//   연속_소장종통계 검색조건절 구하기.
//
// [ 미니스펙 ]                          
// ##############################################################################################################
INT CStaticsInfoMgr::GetSearchWhere1113(CString sStaticsClass,CString sTableName, CString &sInWhere, CString &sOutWhere, CString &sPrintSearchWhere)
{
EFS_BEGIN

	INT ids;
		
	// ==========================================================================================================
	// 1. 통계에 사용되는 테이블 리스트 생성
	// ==========================================================================================================
	CStringArray saTableName;
	ids = CLocCommonAPI::MakeArrayList( sTableName , _T(",") , saTableName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );

	// ==========================================================================================================
	// 2. 수입구분 , 관리구분 , 분류기호구분 조건 생성
	// ==========================================================================================================
	const INT nCnt1 = 3;
	TCHAR *sControlAlias1[nCnt1] = 
	{	
		_T("배가부서") , _T("관리구분") , _T("분류기호구분") 
	};
	INT sFieldType1[nCnt1] =
	{
		ONESTRING , ONESTRING , ONESTRING 
	};

	CString sWhere;
	CString sValue;
	CString sFieldName;

	for ( INT i = 0;i < nCnt1;i++ )
	{
		sValue.Empty();sWhere.Empty();

		// 해당 컨트롤의 값을 구한다.
		GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias1[i] , _T("값") , sValue );
		if( sValue.IsEmpty() ) continue;

		// 컨트롤에 해당하는 필드명 가져오기
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , sControlAlias1[i] , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );

		// 필드타입에 따른 조건 입력방식 분기
		if( sFieldType1[i] == ONESTRING )			sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue);
		else if( sFieldType1[i] == ONENUMERIC )		sWhere.Format( _T(" %s = %s ") , sFieldName , sValue );

		// 안조건과 밖조건 분기
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;

		// 코드를 설명으로 변경 ㅡㅡ? 왜 들어간건지 
		if( i == 0 )		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분") , sValue , sValue );
		else if( i == 2 )	m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("분류표구분") , sValue , sValue );
		else    			m_pInfo->pCodeMgr->ConvertCodeToDesc( sControlAlias1[i] , sValue , sValue );
	}

	// ==========================================================================================================
	// 3. 자료 상태 조건 생성     
	// ==========================================================================================================
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , _T("자료상태") , _T("값") , sValue );
	
	if( !sValue.IsEmpty() )
	{
		if( sValue.Compare( _T("소장자료(비치+실시간배가+대출중+수리제본+파손+분실)")) == 0 )
		{
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O' , 'SEL213O' , 'SEL216O', 'SEL214O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' , 'SEL213O' , 'SEL216O', 'SEL214O' ) ") );
			// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' , 'SEL213O' , 'SEL216O', 'SEL214O', 'SEL411O', 'SEL511O', 'SEL611O' ) ") );
			//=====================================================
		}
		else if( sValue.Compare(_T("소장자료(비치+실시간배가+대출중)")) == 0 )
		{
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' ) ") );
			// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O', 'SEL411O', 'SEL511O', 'SEL611O' ) ") );
			//=====================================================
			
		}
		else if( sValue.Compare(_T("비치자료+실시간배가자료")) == 0 )
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' ) ") );
		else if( sValue.Compare(_T("대출중자료")) == 0 )
		{
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' ) ") );
			// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' , 'SEL411O', 'SEL511O', 'SEL611O' ) ") );
			//=====================================================
		}
		else if( sValue.Compare(_T("수리제본자료")) == 0 )
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL213O'  ) ") );
		else if( sValue.Compare(_T("분실자료")) == 0 )
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL216O' ) ") );
		else if( sValue.Compare(_T("제적자료")) == 0 )
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL215O' ) ") );
		else if( sValue.Compare(_T("파손자료")) == 0 )
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL214O' ) ") );
		else if( sValue.Compare( _T("소장자료(비치+실시간배가+대출중+수리제본+파손+분실+재정리)") ) == 0 )
		{
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O' , 'SEL213O' , 'SEL216O', 'SEL214O', 'SEL217O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' , 'SEL213O' , 'SEL216O', 'SEL214O', 'SEL217O' ) ") );
			// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' , 'SEL213O' , 'SEL216O', 'SEL214O', 'SEL217O', 'SEL411O', 'SEL511O', 'SEL611O' ) ") );
			//=====================================================
		}
		else if( sValue.Compare( _T("소장자료(비치+실시간배가+대출중+재정리)") ) == 0 )
		{
//			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O' , 'SEL217O' ) ") );
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' , 'SEL217O' ) ") );
			// 18.09.27 JMJ KOL.RED.2018.008
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL212N' , 'SEL112N' , 'SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL315O' , 'SEL217O', 'SEL411O', 'SEL511O', 'SEL611O' ) ") );
			//=====================================================
		}
		else if( sValue.Compare( _T("재정리자료") ) == 0 )
			sWhere.Format( _T(" B.WORKING_STATUS IN ( 'SEL217O' ) ") );
		
		if( IsFieldFromTable( _T("B.WORKING_STATUS") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;		

		if( (sValue.Compare(_T("소장자료(비치+실시간배가+대출중+수리제본+파손+분실)")) != 0) &&
			(sValue.Compare(_T("소장자료(비치+실시간배가+대출중)")) != 0) )
			sPrintSearchWhere += CString(_T("자료상태:")) + sValue + SPACE_STRING;		
	}	

	// ==========================================================================================================
	// 4. 등록번호 조건생성
	// ==========================================================================================================
	sWhere.Empty();
	const INT nCnt2 = 3;
	TCHAR *sControlAlias2[nCnt2] =
	{
		_T("등록번호구분") , _T("등록번호1") , _T("등록번호2") 
	};

	CString sValue1,sValue2,sValue3;

	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias2[0] , _T("값") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias2[1] , _T("값") , sValue2 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias2[2] , _T("값") , sValue3 );

	if( !sValue1.IsEmpty() && !sValue2.IsEmpty() )
	{
		CString sRegNo1 , sRegNo2;
		sRegNo1 = sValue1+sValue2;
		sRegNo2 = sValue1+sValue3;

		CLocCommonAPI::MakeRegNoSpaceToZero( sRegNo1 );

		sPrintSearchWhere += CString(_T("등록번호:")) + sRegNo1;

		if( !sValue3.IsEmpty() )
		{
			CLocCommonAPI::MakeRegNoSpaceToZero( sRegNo2 );
			sWhere.Format( _T(" B.REG_NO BETWEEN '%s' AND '%s' ") ,  sRegNo1 , sRegNo2 );
			sPrintSearchWhere += _T("~") + sRegNo2;
		}
		else
			sWhere.Format( _T(" B.REG_NO = '%s' ") ,  sRegNo1 );

		sPrintSearchWhere += SPACE_STRING;	
	}

	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( _T("B.REG_NO") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}		 

	// ==========================================================================================================
	// 5. 배가일자 조건생성
	// ==========================================================================================================
	sWhere.Empty();
	const INT nCnt3 = 2;
	TCHAR *sControlAlias3[nCnt3] =
	{
		_T("배가일자1") , _T("배가일자2")
	};

	sValue1.Empty(); sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias3[0] , _T("값") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias3[1] , _T("값") , sValue2 );
	
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , _T("배가일자") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );

	if( !sValue1.IsEmpty() )
	{
		sPrintSearchWhere += CString(_T("배가일자:")) + sValue1;

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
			sPrintSearchWhere += CString(_T("~")) + sValue2;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
		
		sPrintSearchWhere += SPACE_STRING;		
	}

	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}

	// ==========================================================================================================
	// 6. 배가변경일자 조건생성
	// ==========================================================================================================
	sWhere.Empty();
	const INT nCnt4 = 2;
	TCHAR *sControlAlias4[nCnt4] =
	{
		_T("배가변경일자1") , _T("배가변경일자2")
	};
	
	sValue1.Empty();sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias4[0] , _T("값") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias4[1] , _T("값") , sValue2 );
		
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , _T("배가변경일자") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );
	
	if( !sValue1.IsEmpty() )
	{
		sPrintSearchWhere += CString(_T("배가변경일자:")) + sValue1;

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
			sPrintSearchWhere += CString(_T("~")) + sValue2;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
		sPrintSearchWhere += SPACE_STRING;
	}
	
	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}

	// ==========================================================================================================
	// 7. 대출일자 조건생성 추가 pjw
	// ==========================================================================================================
	sWhere.Empty();
	const INT nCnt6 = 2;
	TCHAR *sControlAlias6[nCnt6] =
	{
		_T("대출일자1") , _T("대출일자2")
	};

	sValue1.Empty(); sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias6[0] , _T("값") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias6[1] , _T("값") , sValue2 );
	
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , _T("대출일자") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );

	if( !sValue1.IsEmpty() )
	{
		sPrintSearchWhere += CString(_T("대출일자:")) + sValue1;

		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue2 );
			sPrintSearchWhere += CString(_T("~")) + sValue2;
		}
		else
			sWhere.Format( _T(" %s BETWEEN to_date('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('%s 23:59:59','YYYY/MM/DD HH24:MI:SS') ") , sFieldName , sValue1 , sValue1 );
		
		sPrintSearchWhere += SPACE_STRING;		
	}

	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}

	// ==========================================================================================================
	// 8. 원부작성년도 조건생성
	// ==========================================================================================================
	sValue.Empty();
	sWhere.Empty();
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , _T("원부작성년도") , _T("값") , sValue );
	
	if( !sValue.IsEmpty() )
	{
		ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , _T("원부작성년도") , _T("FieldName") , sFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );

		sPrintSearchWhere += CString(_T("원부작성년도:"));

		sWhere.Format( _T(" %s = '%s' ") , sFieldName , sValue );

		sPrintSearchWhere += sValue;
		sPrintSearchWhere += SPACE_STRING;		
	}
	if( !sWhere.IsEmpty() )
	{
		if( IsFieldFromTable( _T("B.A") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}

	// ==========================================================================================================
	// 9. 원부번호 조건생성
	// ==========================================================================================================
	sWhere.Empty();
	const INT nCnt5 = 2;
	TCHAR *sControlAlias5[nCnt5] =
	{
		_T("원부번호1") , _T("원부번호2") 
	};
	
	sValue1.Empty(); sValue2.Empty(); 
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias5[0] , _T("값") , sValue1 );
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , sControlAlias5[1] , _T("값") , sValue2 );	
		
	ids = GetCellData(m_pUserMakeSearchCondDM , _T("알리아스") , _T("원부번호") , _T("FieldName") , sFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::GetSearchWhere1113()") );
	
	if( !sValue1.IsEmpty() )
	{
		sPrintSearchWhere += CString(_T("원부번호:"));
		
		if( !sValue2.IsEmpty() )
		{
			sWhere.Format( _T(" %s BETWEEN %s AND %s ") , sFieldName , sValue1 , sValue2 );		
			sPrintSearchWhere += sValue1 + _T("~") + sValue2;
		}
		else
		{
			sPrintSearchWhere += sValue1;
			sWhere.Format( _T(" %s = %s ") , sFieldName , sValue1 );
		}
		sPrintSearchWhere += SPACE_STRING;
	}
	
	if( !sWhere.IsEmpty() )
	{

		if( IsFieldFromTable( _T("B.A") , saTableName ) == 1 )
			sOutWhere += _T(" AND ") + sWhere;
		else
			sInWhere += _T(" AND ") + sWhere;
	}	

	// ==========================================================================================================
	// 10. 부록자료 포함 여부 조건생성
	// ==========================================================================================================
	// 2006-04-03 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 제본상태와 복합적으로 조건을 생성한다.	
	sValue.Empty();
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , _T("제본상태") , _T("값") , sValue );
	CString sBinded;
	if ( sValue == _T("적용안함") )		  sBinded = _T(" 'B' , 'C' ");
	else if ( sValue == _T("제본등록") )  sBinded = _T(" 'B' ");
	else if ( sValue == _T("낱권등록") )  sBinded = _T(" 'C' ");

	sWhere.IsEmpty();
	sValue.Empty();
	GetCellData( m_pUserSearchCond , _T("컨트롤알리아스") , _T("부록자료포함") , _T("값") , sValue );
	
	if( !sValue.IsEmpty() )
		sWhere.Format(_T(" B.BOOK_TYPE IN (%s) ") , sBinded);
	else
		sWhere.Format(_T(" B.BOOK_TYPE IN (%s, 'A' ) ") , sBinded);

	sFieldName = _T("B.BOOK_TYPE");		
	if( IsFieldFromTable( sFieldName , saTableName ) == 1 )
		sOutWhere += _T(" AND ") + sWhere;
	else
		sInWhere += _T(" AND ") + sWhere;
	// ----------------------------------------------------------------------------------------------------------

	//PJW수정
	if(sStaticsClass == _T("자료실별이용통계") || sStaticsClass == _T("신규구입도서이용통계"))
	{
		sPrintSearchWhere += CString(_T("단위:대출이력책(소장책)"));
	}else
	{
		sPrintSearchWhere += CString(_T("단위:종[권](책)"));
	}
	/*if( !sPrintSearchWhere.IsEmpty() ) sPrintSearchWhere += SPACE_STRING ;
	sPrintSearchWhere += CString(_T("단위:종[권](책)"));
	*/
	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ 함수설명 ]
//   연속 장서 관리의 소장종 통계
// 
// [ 미니스펙 ]   
//
// ##############################################################################################################
INT CStaticsInfoMgr::RunSeStatics(CString sStaticsClass, CProgressCtrl *pProgressCtrl, CProgressCtrl *pTotalProgressCtrl)
{
EFS_BEGIN

	// 1. 종수 통계를 가져온다.
	// 2. 권수 통계를 가져온다. 종수 통계에 값을 ADD한다.	
	// 4. 책수 통계를 가져오면서 종권수 통계에 값을 ADD한다.
	// 5. 종수 합계를 가져온다. 
	// 6. 권수 합계를 가져온다. 
	// 7. 책수 합계를 가져온다.
	// 8. 종수 합계에 권수 합계를 ADD한다.
	// 9. 종권수 합계에 책수 합계를 ADD한다.
	// 끝~~

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;	
	CString sXFieldName,sYFieldName;
	CString sXGroupYN , sYGroupYN;
	CString sResultFieldName;
	CStringArray saResultFieldName;
	CString sWhere;

	// =========================================================================================================
	// 1. 통계를 내기 위한 기본값들 가져오기
	// =========================================================================================================
	ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::RunBoStatics()") );

	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물") , sResultFieldName );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::RunBoStatics()") );

	CLocCommonAPI::MakeArrayList( sResultFieldName , _T("/") , saResultFieldName );
	
	CString sXData,sYData,sTmp;
	pTotalProgressCtrl->SetRange32( 0 , (m_pRealResultDM->RefList.GetCount()*m_pRealResultDM->GetRowCount()*2) );
	INT nProgressCnt = 0;

	// =========================================================================================================
	// 2. 종 , 권 , 책의 통계를 만든다.//2007.04.13 pjw수정:자료실별,신규구입도서 이용통계
	// =========================================================================================================
	if(sStaticsClass == _T("자료실별이용통계") || sStaticsClass == _T("신규구입도서이용통계"))
	{
		for( INT i = 0;i < 2;i++ )
		{
			m_pResultDM->TBL_NAME = saTableName.GetAt(i);	
			
			pTotalProgressCtrl->SetRange32( 0 , 1 );

			ids = GetSeWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , _T("Y") , _T("Y") ,i , sWhere );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::RunBoStatics()") );

			sWhere += _T(" Group By ") + sXFieldName + _T(" , ") + sYFieldName;

			ids = FillStaticsValue3(sStaticsClass, sXFieldName,sYFieldName,saResultFieldName.GetAt(i),_T(""),_T(""),_T("Y"),_T("Y"),sWhere,i, pProgressCtrl);
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::RunBoStatics()") );

			pTotalProgressCtrl->SetPos(1);
		}
	}else
	{
		for( INT i = 0;i < 3;i++ )
		{
			m_pResultDM->TBL_NAME = saTableName.GetAt(i);	
			
			pTotalProgressCtrl->SetRange32( 0 , 1 );

			ids = GetSeWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , _T("Y") , _T("Y") ,i , sWhere );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::RunBoStatics()") );

			sWhere += _T(" Group By ") + sXFieldName + _T(" , ") + sYFieldName;

			ids = FillStaticsValue3(sStaticsClass, sXFieldName,sYFieldName,saResultFieldName.GetAt(i),_T(""),_T(""),_T("Y"),_T("Y"),sWhere,i, pProgressCtrl);
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::RunBoStatics()") );

			pTotalProgressCtrl->SetPos(1);
		}
	}
	/*for( INT i = 0;i < 3;i++ )
	{
		m_pResultDM->TBL_NAME = saTableName.GetAt(i);	
		
		pTotalProgressCtrl->SetRange32( 0 , 1 );
		ids = GetSeWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , _T("Y") , _T("Y") ,i , sWhere );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::RunBoStatics()") );

		sWhere += _T(" Group By ") + sXFieldName + _T(" , ") + sYFieldName;
		ids = FillStaticsValue3( sXFieldName,sYFieldName,saResultFieldName.GetAt(i),_T(""),_T(""),_T("Y"),_T("Y"),sWhere,i, pProgressCtrl);
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::RunBoStatics()") );

		pTotalProgressCtrl->SetPos(1);
	}*/
	// =========================================================================================================
	// 3. 합계 구성
	// =========================================================================================================
	ids = MakeTotalCount4( sStaticsClass , saTableName , sXFieldName,sYFieldName , pProgressCtrl , pTotalProgressCtrl);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::RunBoStatics()") );
	
	
	// =========================================================================================================
	// 4. 코드값을 설명값으로 바꾼다.
	// =========================================================================================================
	ids = CodeToDesc(sStaticsClass);
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::RunBoStatics()") );

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ 함수설명 ]
//   연속 소장종 통계 WHERE절 얻어오기
//
// [ 미니스펙 ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::GetSeWhere(CString sStaticsClass, CString sTableName, CString sXFieldName, CString sYFieldName, CString sXData, CString sYData, CString sXGroupYN, CString sYGroupYN, INT nMode, CString &sWhere)
{
EFS_BEGIN

	INT ids;

	CStringArray saWhere;
	CString sMenuName = GetMenuName();
	CString sInWhere;	// 안조건
	CString sOutWhere;	// 밖조건

	// ==========================================================================================================
	// 1. 기본 조건 가져오기
	// ==========================================================================================================
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("기본조건") , sWhere );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::GetBoWhere()") );

	CLocCommonAPI::MakeArrayList( sWhere , _T("/") , saWhere );

	if( saWhere.GetSize() < nMode+1 )
		nMode = 0;
	
	sWhere = saWhere.GetAt(nMode);
	
	// ==========================================================================================================
	// 2. 검색조건에서 생성된 조건 얻어오기
	// ==========================================================================================================
	if ( sMenuName.Compare(BOOK_STATICS) != 0 )
	{
		ids = GetSeSearchWhere( sStaticsClass , sTableName , sInWhere , sOutWhere );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::GetBoWhere()") );
		if ( ids > 0 ) return ids;
	}

	// ==========================================================================================================
	// 3. Group에 의해서 생성된 조건 구성하기
	// ==========================================================================================================
	CStringArray asTableName;
	CLocCommonAPI::MakeArrayList( sTableName , _T(",") , asTableName );

	CString sTmpWhere;
	if( !sXFieldName.IsEmpty() && (sXGroupYN.Compare(_T("N"))==0) )
	{
		sXData.Replace( _T("'"), _T("''") );
		
		if( !sXData.IsEmpty() )
			sTmpWhere.Format( _T(" AND %s='%s'"),sXFieldName,sXData );
		else
			sTmpWhere.Format( _T(" AND ( %s='%s' OR %s IS NULL ) "),sXFieldName,sXData,sXFieldName);

		if( IsFieldFromTable( sXFieldName , asTableName ) == 1 )
			sOutWhere += sTmpWhere;
		else
			sInWhere += sTmpWhere;
	}

	if( !sYFieldName.IsEmpty() && (sYGroupYN.Compare(_T("N"))==0) )
	{
		sYData.Replace( _T("'"), _T("''") );
		
		if( !sYData.IsEmpty() )
			sTmpWhere.Format( _T(" AND %s='%s'"),sYFieldName,sYData );
		else
			sTmpWhere.Format( _T(" AND ( %s='%s' OR %s IS NULL ) "),sYFieldName,sYData,sYFieldName);
		if( IsFieldFromTable( sYFieldName , asTableName ) == 1 )
			sOutWhere += sTmpWhere;
		else
			sInWhere += sTmpWhere;
	}

	// 메크로 적용
	sWhere.Replace( _T("%안조건%") , sInWhere );
	sWhere.Replace( _T("%밖조건%") , sOutWhere );

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ 함수설명 ]
//   안조건절과 밖조건절 구성하기
//
// [ 미니스펙 ]
// 
// ##############################################################################################################
INT CStaticsInfoMgr::GetSeSearchWhere(CString sStaticsClass, CString sTableName, CString &sInWhere, CString &sOutWhere)
{
EFS_BEGIN

	INT ids;

	CString sMenuName = GetMenuName();

	// ==========================================================================================================
	// 1. 컨트롤 알리아스별 값 DM 구성
	// ==========================================================================================================
	ids = MakeUserSearchList( sStaticsClass );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::GetBoSearchWhere()") );
	if( ids > 0 ) return ids;

	// ==========================================================================================================
	// 2. 메뉴 명에 따라 달라진다.
	// ==========================================================================================================
	CString sPrintSearchWhere;

    if( sMenuName.Compare(SE_HOLDING_STATICS) == 0 )
		ids = GetSearchWhere1113(sStaticsClass, sTableName , sInWhere , sOutWhere , sPrintSearchWhere );

	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::GetBoSearchWhere()") );
	if( ids > 0 ) return ids;

	// ==========================================================================================================
	// 3. 출력용 검색조건
	// ==========================================================================================================
	ids = SetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("사용자검색조건_출력") , sPrintSearchWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::GetBoSearchWhere()") );
	if( ids > 0 ) return ids;

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ 함수설명 ]
//   연속_소장종통계 통계 산출
//   종 결과 권 결과 책 결과를 같이 표현해 준다.
//
// [ 미니스펙 ]
//
// ##############################################################################################################
//CString sStaticsClass 추가 :: pjw
INT CStaticsInfoMgr::FillStaticsValue3(CString sStaticsClass,CString sXFieldName, CString sYFieldName, CString sResultFieldName, CString sXData, CString sYData, CString sXGroupYN, CString sYGroupYN, CString sWhere, INT nMode, CProgressCtrl *pProgressCtrl)
{
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. SELECT 필드 설정
	// ==========================================================================================================
	ids = SetDataMgrRef( m_pResultDM , sXFieldName ,_T("") , sXGroupYN ,  0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunStatics") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("") , sYGroupYN , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("RunStatics") );
	ids = SetDataMgrRef( m_pResultDM , sResultFieldName , _T("0") , _T("Y") , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("RunStatics") );

	// ==========================================================================================================
	// 2. WHERE 조건 설정
	// ==========================================================================================================
	CString sSendWhere = sWhere;

	// ==========================================================================================================
	// 3. RESULT DM -> REAL RESULT DM 으로 이동
	// ==========================================================================================================
	ids = m_pResultDM->RefreshDataManager( sSendWhere , pProgressCtrl );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsValue2") );

	INT nXIndex , nYIndex;
	CString sXAlias , sYAlias , sResult , sPreResult;



	for( INT i = 0 ; i < m_pResultDM->GetRowCount() ; i++ )
	{
		ids = m_pResultDM->GetCellData( _T("결과물") , i , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("FillStaticsValue2") );

		ids = m_pResultDM->GetCellData( _T("X축알리아스") , i , sXAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("FillStaticsValue2") );

		ids = m_pResultDM->GetCellData( _T("Y축알리아스") , i , sYAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("FillStaticsValue2") );
		
		// 인덱스 찾기
		ids = GetDMIndex( m_pRealResultDM , sXAlias,sYAlias,nXIndex,nYIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("FillStaticsValue2") );

		// 값 입력		
		ids = m_pRealResultDM->GetCellData( sXAlias , nYIndex , sPreResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("FillStaticsValue2") );
				
		// 종, 권, 책의 값을 한곳에 표시 하자!!//pjw수정해야할부분 총 합계 합계부분 수정	
	
		if(sStaticsClass == _T("자료실별이용통계") || sStaticsClass == _T("신규구입도서이용통계"))
		{
			if( nMode == 0 )
			{
				sResult.Format( _T("%d") , _ttoi(sPreResult) + _ttoi(sResult) );
				ids = m_pRealResultDM->SetCellData( sXAlias , sResult , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("FillStaticsValue2") );			
			}
			else if( nMode == 1 )
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sPreResult) , _ttoi(sResult) );							
				ids = m_pRealResultDM->SetCellData( sXAlias , sResult , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -102 , _T("FillStaticsValue2") );
			}
		}else
		{
			if( nMode == 0 )
			{
				sResult.Format( _T("%d") , _ttoi(sPreResult) + _ttoi(sResult) );
				ids = m_pRealResultDM->SetCellData( sXAlias , sResult , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("FillStaticsValue2") );			
			}
			else if( nMode == 1 )
			{
				sResult.Format( _T("%d\r\n[%d]") , _ttoi(sPreResult) , _ttoi(sResult) );							
				ids = m_pRealResultDM->SetCellData( sXAlias , sResult , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -102 , _T("FillStaticsValue2") );
			}
			else if( nMode == 2 )
			{
				sResult.Format( _T("%s\r\n(%d)") , sPreResult , _ttoi(sResult) );							
				ids = m_pRealResultDM->SetCellData( sXAlias , sResult , nYIndex);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -102 , _T("FillStaticsValue2") );
			}
		}
	}

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ 함수설명 ]
//   연속_소장종통계 합계
//
// [ 미니스펙 ]
//
// ##############################################################################################################

INT CStaticsInfoMgr::MakeTotalCount4( CString sStaticsClass , CStringArray &saTableName , CString sXFieldName, CString sYFieldName, CProgressCtrl *pProgressCtrl, CProgressCtrl *pTotalProgress)
{
EFS_BEGIN

	INT ids;

	m_pYSumDM->FreeData();
	m_pXSumDM->FreeData();
	
	/// 중복 허용 합계 계산pjw수정-New_XYSum4_duplication(_allow,_delete추가)

		
	if(sStaticsClass == _T("자료실별이용통계") || sStaticsClass == _T("신규구입도서이용통계"))
	{
		ids = New_XYSum4_duplication_allow();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeTotalCount4()") );

		/// 중복 삭제 합계 계산
		ids = New_XYSum4_duplication_delete( sStaticsClass , saTableName , sXFieldName , sYFieldName , pTotalProgress );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeTotalCount4()") );
	}else
	{
		ids = XYSum4_duplication_allow();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeTotalCount4()") );

		/// 중복 삭제 합계 계산
		ids = XYSum4_duplication_delete( sStaticsClass , saTableName , sXFieldName , sYFieldName , pTotalProgress );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeTotalCount4()") );
	}
/*	ids = XYSum4_duplication_allow();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::MakeTotalCount4()") );

	/// 중복 삭제 합계 계산
	ids = XYSum4_duplication_delete( sStaticsClass , saTableName , sXFieldName , sYFieldName , pTotalProgress );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::MakeTotalCount4()") );
*/

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ 함수설명 ]
//   연속_소장종통계 값을 분리하는 함수 
//
// [ 미니스펙 ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::MakeSpeciesVolBookCnt(CString sDuplicateAllowSum, CString& sSpeciesCnt, CString& sVolCnt, CString& sBookCnt)
{
EFS_BEGIN

	sSpeciesCnt.Empty();sVolCnt.Empty();sBookCnt.Empty();

	// 권수 표시가 분리 플래그의 대상이 된다.
	INT nVolSpos, nVolEpos;
	nVolSpos = sDuplicateAllowSum.Find( _T("[") );
	nVolEpos = sDuplicateAllowSum.Find( _T("]") );

	if ( nVolSpos > -1 && nVolEpos > -1 )
	{
		/// 종수 구하기 
		sSpeciesCnt.Format( _T("%s"), sDuplicateAllowSum.Left( nVolSpos ) );
		sSpeciesCnt.Replace( _T("\r\n") , _T("") );
		
		/// 권수 
		sVolCnt.Format( _T("%s"), sDuplicateAllowSum.Mid( nVolSpos + 1, nVolEpos - (nVolSpos + 1) ) );
		
		/// 책수
		if ( sDuplicateAllowSum.GetLength() >=  nVolEpos + 1 )
		{
			sBookCnt.Format( _T("%s"), sDuplicateAllowSum.Mid( nVolEpos + 1 ) );
			sBookCnt.Replace( _T("\r\n") , _T("") );
			sBookCnt.Replace( _T("(") , _T("") );
			sBookCnt.Replace( _T(")") , _T("") );
		}
	}
	else
	{
		if ( sDuplicateAllowSum.GetLength() )
			sSpeciesCnt.Format( _T("%s") , sDuplicateAllowSum );
	}

	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ 함수설명 ]
//   연속_소장종통계 합계 중복삭제 합계
//   쿼리로 가져온다.
//   책수는 같다고 본다.
//   중복포함 합계 이후에 실행되어야 한다.
//
// [ 미니스펙 ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::XYSum4_duplication_delete(CString sStaticsClass, CStringArray &saTableName, CString sXFieldName, CString sYFieldName, CProgressCtrl *pTotalProgress)
{
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. 통계를 낼 DM에 테이블 이름 추가	
	// ==========================================================================================================
	CString sResultFieldName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물") , sResultFieldName );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );

	CStringArray saResultFieldName;
	CLocCommonAPI::MakeArrayList( sResultFieldName , _T("/") , saResultFieldName );
	
	INT i = 0;	
	
	m_pResultDM->TBL_NAME = saTableName.GetAt(i);
	
	CStringArray saTableAlias;
	
	ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );	

	CString strCountField;
	CString sYSumAlias , sDuplicateAllowSum;
	CString sTmpResult , sResult;
	CString sYData , sTmp;
	CString sSpeciesCnt , sVolCnt, sBookCnt;
	INT nIndex;
	CString sWhere;
	/// 가로 합계 계산 //////////////////////////////////////////////////////////////////////////////////////////		
	// 연속은 권에 대한 합계 쿼리를 한번 더 수행해야한다.	
	for ( INT ii = 0;ii < 2;ii++ )
	{
		// ==========================================================================================================
		// 2. 종,권 가로 합계 SELECT 필드 설정
		// ==========================================================================================================
	   
		if ( !ii ) strCountField = _T("COUNT( DISTINCT S.REC_KEY )");
		else       strCountField = _T("COUNT( DISTINCT V.REC_KEY )"); 
		
		ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
		ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("Y") , 1 );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
		ids = SetDataMgrRef( m_pResultDM , strCountField , _T("0") , _T("Y") , 2 );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );

		// ==========================================================================================================
		// 3. 종,권 가로 합계 WHERE절 구성	
		// ==========================================================================================================
		ids = GetSeWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , sYFieldName , _T("") , _T("") , _T("N") , _T("Y") ,i , sWhere );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );

		sWhere += _T(" GROUP BY ") + sYFieldName;
				
		ids = m_pResultDM->RefreshDataManager( sWhere );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );

		for ( INT k = 0;k < m_pResultDM->GetRowCount();k++ )
		{
			ids = m_pResultDM->GetCellData( _T("Y축알리아스") , k , sYSumAlias );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
			
			nIndex = -1;
			for ( INT j = 0;j < m_pRealResultDM->GetRowCount();j++ )
			{
				ids = m_pRealResultDM->GetCellData( j , 0 , sYData );
				if ( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
				
				if ( sYData.Compare( sYSumAlias ) == 0 ) nIndex = j;
			}
			
			if ( nIndex == -1 ) continue;
			
			ids = m_pResultDM->GetCellData( _T("결과물") , k , sResult );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
			
			ids = m_pXSumDM->GetCellData( _T("중복허용_합계") , nIndex , sDuplicateAllowSum );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
			
			MakeSpeciesVolBookCnt( sDuplicateAllowSum , sSpeciesCnt , sVolCnt , sBookCnt );
			
			if( _ttoi(sResult) == 0 )
				sResult = _T("0");
			else
			{
				if ( !ii )
						sResult.Format( _T("%d\r\n[%d]\r\n(%d)") , _ttoi(sResult) , _ttoi(sVolCnt) , _ttoi(sBookCnt) );
					else
						sResult.Format( _T("%d\r\n[%d]\r\n(%d)") , _ttoi(sSpeciesCnt) , _ttoi(sResult) , _ttoi(sBookCnt) );
						
			}
			
			ids = m_pXSumDM->SetCellData( _T("중복삭제_합계") , sResult , nIndex  );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );			
		}	
	}

	/// 세로 합계 계산 //////////////////////////////////////////////////////////////////////////////////////////		
	// 연속은 권에 대한 합계 쿼리를 한번 더 수행해야한다.	
	sWhere.Empty();
	for ( ii = 0;ii < 2;ii++ )
	{
		// ==========================================================================================================
		// 4. 종,권 가로 합계 SELECT 필드 설정  
		// ==========================================================================================================
			 if ( !ii ) strCountField = _T("COUNT( DISTINCT S.REC_KEY )");
			 else       strCountField = _T("COUNT( DISTINCT V.REC_KEY )"); 
	
		ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("Y") , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 ,_T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
		ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
		ids = SetDataMgrRef( m_pResultDM , strCountField , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
		
		// ==========================================================================================================
		// 5. 종,권 가로 합계 WHERE절 구성	
		// ==========================================================================================================
		ids = GetSeWhere( sStaticsClass , saTableName.GetAt(i) , sXFieldName , _T("") , _T("") , _T("") ,_T("Y") , _T("N") , i , sWhere );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -16 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );

		sWhere += _T(" GROUP BY ") + sXFieldName;
		
		ids = m_pResultDM->RefreshDataManager( sWhere );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );

		CString sXSumAlias;
		sDuplicateAllowSum.Empty();
		sTmpResult.Empty() , sResult.Empty();
		CString sXData;
		sTmp.Empty();
		sSpeciesCnt.Empty() , sBookCnt.Empty();
		for( INT k = 0 ; k < m_pResultDM->GetRowCount() ; k++ )
		{
			ids = m_pResultDM->GetCellData( _T("X축알리아스") , k , sXSumAlias );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
			
			nIndex = -1;
			for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
			{
				ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
				if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );

				if( sXData.Compare( sXSumAlias ) == 0 ) nIndex = j;
			}

			if( nIndex == -1 ) continue;

			ids = m_pResultDM->GetCellData( _T("결과물") , k , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );

			ids = m_pYSumDM->GetCellData( _T("중복허용_합계") , nIndex-1 , sDuplicateAllowSum );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );
			
			MakeSpeciesVolBookCnt( sDuplicateAllowSum , sSpeciesCnt , sVolCnt , sBookCnt );
			if( _ttoi(sResult) == 0 )
				sResult = _T("0");
			else
			{
				if ( !ii )
					sResult.Format( _T("%d\r\n[%d]\r\n(%d)") , _ttoi(sResult) , _ttoi(sVolCnt) , _ttoi(sBookCnt) );
				else
					sResult.Format( _T("%d\r\n[%d]\r\n(%d)") , _ttoi(sSpeciesCnt) , _ttoi(sResult) , _ttoi(sBookCnt) );
			}

			ids = m_pYSumDM->SetCellData( _T("중복삭제_합계") , sResult , nIndex-1  );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -22 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );					
		}
	}
	
		/// 총합계 //////////////////////////////////////////////////////////////////////////////////////////////////
	// 연속은 권에 대한 합계쿼리를 한번더 수행해야 한다.
	for ( ii = 0;ii < 2;ii++ )
	{
		// ==========================================================================================================
		// 6. 총합계 SELECT 필드 설정  
		// ==========================================================================================================
		ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );					
		ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -24 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );	
		ids = SetDataMgrRef( m_pResultDM , saResultFieldName.GetAt(ii) , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -25 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );					

		// ==========================================================================================================
		// 7. 총합계 WHERE절 생성
		// ==========================================================================================================
		sWhere.Empty();
		ids = GetSeWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
		
		ids = m_pResultDM->RefreshDataManager( sWhere);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -26 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );					
				
		ids = m_pResultDM->GetCellData( _T("결과물") , 0 , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -27 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );					

		ids = m_pXSumDM->GetCellData( _T("중복허용_합계") , m_pXSumDM->GetRowCount()-1 , sDuplicateAllowSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -28 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );					
		
		MakeSpeciesVolBookCnt( sDuplicateAllowSum , sSpeciesCnt , sVolCnt, sBookCnt );

		if( _ttoi(sResult) == 0 )//그리드에 총계 실제 값 나타나는 부분
			sResult = _T("0");
		else
		{
			if ( !ii )
				sResult.Format( _T("%d\r\n[%d]\r\n(%d)") , _ttoi(sResult) , _ttoi(sVolCnt) , _ttoi(sBookCnt) );
			else
				sResult.Format( _T("%d\r\n[%d]\r\n(%d)") , _ttoi(sSpeciesCnt) , _ttoi(sResult) , _ttoi(sBookCnt) );
		}
		
		ids = m_pXSumDM->SetCellData( _T("중복삭제_합계") , sResult , m_pXSumDM->GetRowCount()-1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -29 , _T("CStaticsInfoMgr::XYSum4_duplication_delete()") );					
	}	
	
	// ==========================================================================================================
	// 8. 모든 합계중 비어있는 공간은 0으로 만들기
	// ==========================================================================================================
	for( INT m = 0 ; m < m_pXSumDM->GetRowCount() ; m++ )
	{
		ids = m_pXSumDM->GetCellData( _T("중복삭제_합계") , m , sTmp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -30 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );

		sTmp.TrimLeft(); sTmp.TrimRight();
		if( !sTmp.IsEmpty() ) continue;

		ids = m_pXSumDM->SetCellData( _T("중복삭제_합계") , _T("0") , m );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -31 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	}

	for( m = 0 ; m < m_pYSumDM->GetRowCount() ; m++ )
	{
		ids = m_pYSumDM->GetCellData( _T("중복삭제_합계") , m , sTmp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -32 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
		
		sTmp.TrimLeft(); sTmp.TrimRight();
		if( !sTmp.IsEmpty() ) continue;
		
		ids = m_pYSumDM->SetCellData( _T("중복삭제_합계") , _T("0") , m );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -33 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	}
	
	return 0;

EFS_END
	return -1;
}

// ##############################################################################################################
// [ 함수설명 ]PJW작성-연속 신규구입도서이용통계,자료실별이용통계 
//   연속_소장종통계 합계 중복삭제 합계
//   쿼리로 가져온다.
//   책수는 같다고 본다.
//   중복포함 합계 이후에 실행되어야 한다.
//
// [ 미니스펙 ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::New_XYSum4_duplication_delete(CString sStaticsClass, CStringArray &saTableName, CString sXFieldName, CString sYFieldName, CProgressCtrl *pTotalProgress)
{
EFS_BEGIN

	INT ids;

	CStringArray saResultFieldName;
	CString sResultFieldName;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물") , sResultFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	
	CLocCommonAPI::MakeArrayList( sResultFieldName , _T("/") , saResultFieldName );
	
	INT i = 0;
	
	// DM 테이블 이름 추가
	m_pResultDM->TBL_NAME = saTableName.GetAt(i);
	
	CStringArray saTableAlias;	
	ids = MakeTableAliasArray( saTableName.GetAt(i) , saTableAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );	

	/// 종수만 한꺼번에 가져온다.
	// ==========================================================================================================
	/// 가로 합계 계산
	// ==========================================================================================================	
	// SELECT 필드 설정
	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("Y") , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );

	// pjw
//	ids = SetDataMgrRef( m_pResultDM , _T("COUNT( DISTINCT S.REC_KEY )") , _T("0") , _T("Y") , 2 );
//	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	if( _T("자료실별이용통계") == sStaticsClass ||   _T("신규구입도서이용통계")== sStaticsClass)
	{
		ids = SetDataMgrRef( m_pResultDM , _T("COUNT( DISTINCT CL.BOOK_KEY )") , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	}
	else
	{
		ids = SetDataMgrRef( m_pResultDM , _T("COUNT( DISTINCT S.REC_KEY )") , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	}
	
	// 3.3. WHERE절 구성 
	CString sWhere;
		
	ids = GetSeWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , sYFieldName , _T("") , _T("") , _T("N") , _T("Y") ,i , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );

	sWhere += _T(" GROUP BY ") + sYFieldName;
		
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );

	
	CString sYSumAlias , sDuplicateAllowSum;
	CString sTmpResult , sResult;
	CString sYData , sTmp;
	CString sSpeciesCnt , sBookCnt;
	INT nIndex;
	for( INT k = 0 ; k < m_pResultDM->GetRowCount() ; k++ )
	{
		ids = m_pResultDM->GetCellData( _T("Y축알리아스") , k , sYSumAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
		
		nIndex = -1;
		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( j , 0 , sYData );
			if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
			
			if( sYData.Compare( sYSumAlias ) == 0 ) nIndex = j;
		}
		
		if( nIndex == -1 ) continue;
		
		ids = m_pResultDM->GetCellData( _T("결과물") , k , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
		
		ids = m_pXSumDM->GetCellData( _T("중복허용_합계") , nIndex , sDuplicateAllowSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
		
		MakeSpeciesBookCnt( sDuplicateAllowSum , sSpeciesCnt , sBookCnt );
		
		//pjw수정
		if( _T("자료실별이용통계") == sStaticsClass ||   _T("신규구입도서이용통계")== sStaticsClass)
		{
			if( _ttoi(sBookCnt) == 0 )
			sResult = _T("0");
			else
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );
			}
			
		}else
		{
			if( _ttoi(sResult) == 0 )
			sResult = _T("0");
			else
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );
			}
		}
		/*if( _ttoi(sResult) == 0 )
			sResult = _T("0");
		else
		{
			sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );
		}*/
		
		ids = m_pXSumDM->SetCellData( _T("중복삭제_합계") , sResult , nIndex  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
	}

	// ==========================================================================================================
	// 세로 합계 계산
	// ==========================================================================================================
	// 3.3. WHERE절 구성 
	sWhere.Empty();
	
	// SELECT 필드 설정
	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("Y") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	// pjw
//	ids = SetDataMgrRef( m_pResultDM , _T("count(DISTINCT S.REC_KEY)") , _T("0") , _T("Y") , 2 );
//	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	if( _T("자료실별이용통계") == sStaticsClass ||_T("신규구입도서이용통계") == sStaticsClass  )
	{
		ids = SetDataMgrRef( m_pResultDM , _T("count(DISTINCT CL.BOOK_KEY)") , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	}
	else
	{
		ids = SetDataMgrRef( m_pResultDM , _T("count(DISTINCT S.REC_KEY)") , _T("0") , _T("Y") , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	}	
	
	
	// WHERE절 생성
	ids = GetSeWhere( sStaticsClass , saTableName.GetAt(i) , sXFieldName , _T("") , _T("") , _T("") ,_T("Y") , _T("N") , i , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -16 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );

	sWhere += _T(" GROUP BY ") + sXFieldName;
	
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );

	CString sXSumAlias;
	sDuplicateAllowSum.Empty();
	sTmpResult.Empty() , sResult.Empty();
	CString sXData;
	sTmp.Empty();
	sSpeciesCnt.Empty() , sBookCnt.Empty();
	
	for( k = 0 ; k < m_pResultDM->GetRowCount() ; k++ )
	{
		
		ids = m_pResultDM->GetCellData( _T("X축알리아스") , k , sXSumAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
		
		nIndex = -1;
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			ids = m_pRealResultDM->GetColumnRefInfo( j , sXData , sTmp , sTmp , sTmp , sTmp );
			if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );

			if( sXData.Compare( sXSumAlias ) == 0 ) nIndex = j;

			//##################################################################################################
			if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )		nIndex = 4;
			//##################################################################################################
		}

		if( nIndex == -1 ) continue;

		ids = m_pResultDM->GetCellData( _T("결과물") , k , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

		ids = m_pYSumDM->GetCellData( _T("중복허용_합계") , nIndex-1 , sDuplicateAllowSum );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
		
		// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 육사 군사분류기호 예외처리
		if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )
		{
			// sDuplicateAllowSum 에서 분리 작업
			CString sSpecies, sBook;
			DividArmyCell( sDuplicateAllowSum, sSpecies, sBook );			
			
			sSpecies = sSpecies.Mid( sSpecies.Find( DIVCHAR ) + 1 );
			sBook = sBook.Mid( sBook.Find( DIVCHAR ) + 1 );
			sDuplicateAllowSum.Format( _T("%d\r\n(%d)"), _ttoi( sSpecies ), _ttoi( sBook ) );
		}
		// ------------------------------------------------------------------------------------------------------

		MakeSpeciesBookCnt( sDuplicateAllowSum , sSpeciesCnt , sBookCnt );
	
		//pjw수정
		if( _T("자료실별이용통계") == sStaticsClass ||   _T("신규구입도서이용통계")== sStaticsClass)
		{
			if( _ttoi(sBookCnt) == 0 )
			sResult = _T("0");
			else
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );
				// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// 육사 군사분류기호 예외처리
			if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )
			{
				CString sTemp;
				ids = m_pYSumDM->GetCellData( _T("중복삭제_합계") , nIndex-1, sTemp );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -34 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

				CString sForSpeci, sForBook;
				MakeSpeciesBookCnt( sTemp, sForSpeci, sForBook );

				ids = m_pResultDM->GetCellData( _T("결과물") , k , sResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

				sResult.Format( _T("%d %s %d\r\n(%d %s %d)"), _ttoi(sForSpeci), DIVCHAR, _ttoi(sResult), _ttoi(sForBook), DIVCHAR, _ttoi(sBookCnt) );
			}
			// --------------------------------------------------------------------------------------------------
		
			}
			
		}else
		{
			if( _ttoi(sResult) == 0 )
			sResult = _T("0");
			else
			{
				sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );
				// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// 육사 군사분류기호 예외처리
				if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )
				{
					CString sTemp;
					ids = m_pYSumDM->GetCellData( _T("중복삭제_합계") , nIndex-1, sTemp );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -34 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );	

					CString sForSpeci, sForBook;
					MakeSpeciesBookCnt( sTemp, sForSpeci, sForBook );

					ids = m_pResultDM->GetCellData( _T("결과물") , k , sResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

					sResult.Format( _T("%d %s %d\r\n(%d %s %d)"), _ttoi(sForSpeci), DIVCHAR, _ttoi(sResult), _ttoi(sForBook), DIVCHAR, _ttoi(sBookCnt) );
				}
			// --------------------------------------------------------------------------------------------------
			}
		}
	/*	if( _ttoi(sResult) == 0 )
			sResult = _T("0");
		else
		{
			sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );

			// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// 육사 군사분류기호 예외처리
			if ( m_sIs64lib == _T("Y") && sXSumAlias == _T("355-9") )
			{
				CString sTemp;
				ids = m_pYSumDM->GetCellData( _T("중복삭제_합계") , nIndex-1, sTemp );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -34 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

				CString sForSpeci, sForBook;
				MakeSpeciesBookCnt( sTemp, sForSpeci, sForBook );

				ids = m_pResultDM->GetCellData( _T("결과물") , k , sResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

				sResult.Format( _T("%d %s %d\r\n(%d %s %d)"), _ttoi(sForSpeci), DIVCHAR, _ttoi(sResult), _ttoi(sForBook), DIVCHAR, _ttoi(sBookCnt) );
			}
			// --------------------------------------------------------------------------------------------------
		}*/

		ids = m_pYSumDM->SetCellData( _T("중복삭제_합계") , sResult , nIndex-1  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -22 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );				
	}
	
	// ==========================================================================================================
	// 총합계
	// ==========================================================================================================
	// SELECT 필드 설정

	ids = SetDataMgrRef( m_pResultDM , sXFieldName , _T("0") , _T("N") , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	ids = SetDataMgrRef( m_pResultDM , sYFieldName , _T("0") , _T("N") , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -24 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	ids = SetDataMgrRef( m_pResultDM , saResultFieldName.GetAt(i) , _T("0") , _T("Y") , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -25 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	
	// WHERE절 생성
	ids = GetSeWhere( sStaticsClass , saTableName.GetAt(i) , _T("") , _T("") , _T("") , _T("") , _T("N") , _T("N") , i , sWhere );
	
	ids = m_pResultDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -26 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
			
	ids = m_pResultDM->GetCellData( _T("결과물") , 0 , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -27 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );

	ids = m_pXSumDM->GetCellData( _T("중복허용_합계") , m_pXSumDM->GetRowCount()-1 , sDuplicateAllowSum );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -28 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	
	MakeSpeciesBookCnt( sDuplicateAllowSum , sSpeciesCnt , sBookCnt );
	
	if( _ttoi(sResult) == 0 )
		sResult = _T("0");
	else
		sResult.Format( _T("%d\r\n(%d)") , _ttoi(sResult) , _ttoi(sBookCnt) );	
	
	ids = m_pXSumDM->SetCellData( _T("중복삭제_합계") , sResult , m_pXSumDM->GetRowCount()-1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -29 , _T("CStaticsInfoMgr::XYSum2_duplication_delete()") );
	
	
	// ==========================================================================================================
	// 모든 합계중 비어있는 공간은 0으로 만들기
	// ==========================================================================================================
	for( INT m = 0 ; m < m_pXSumDM->GetRowCount() ; m++ )
	{
		ids = m_pXSumDM->GetCellData( _T("중복삭제_합계") , m , sTmp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -30 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );

		sTmp.TrimLeft(); sTmp.TrimRight();
		if( !sTmp.IsEmpty() ) continue;

		ids = m_pXSumDM->SetCellData( _T("중복삭제_합계") , _T("0") , m );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -31 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	}


	for( m = 0 ; m < m_pYSumDM->GetRowCount() ; m++ )
	{
		ids = m_pYSumDM->GetCellData( _T("중복삭제_합계") , m , sTmp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -32 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
		
		sTmp.TrimLeft(); sTmp.TrimRight();
		if( !sTmp.IsEmpty() ) continue;
		
		ids = m_pYSumDM->SetCellData( _T("중복삭제_합계") , _T("0") , m );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -33 , _T("CStaticsInfoMgr::New_XYSum4_duplication_delete()") );
	}
	
	return 0;
EFS_END
	return -1;
}

// ##############################################################################################################
// [ 함수설명 ]
//   연속_소장종통계 중복포함 합계
//   통계 결과를 로우별로 컬럼별로 합친다.
//
// [ 미니스펙 ]
//
// ##############################################################################################################
INT CStaticsInfoMgr::XYSum4_duplication_allow()
{
EFS_BEGIN

	INT ids;
	CString sSumAlias = _T("중복허용_합계");
	CString sAlias;
	CString sSpeciesSum, sVolSum, sBookSum;
	CString sData;
	CString sSpeciesCnt, sVolCnt, sBookCnt;
	CString sTotalSpeciesSum, sTotalVolSum, sTotalBookSum;
	CESL_DataMgr::reference *Ref;
	POSITION pos;

	// ==========================================================================================================
	// 1. 가로 합계
	// ==========================================================================================================
	for ( INT i = 0;i < m_pRealResultDM->GetRowCount();i++ )
	{
		sSpeciesSum.Empty();sVolSum.Empty();sBookSum.Empty();
		for ( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			pos = m_pRealResultDM->RefList.FindIndex(j);
			Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
			sAlias = Ref->FIELD_ALIAS;
			
			ids = m_pRealResultDM->GetCellData( sAlias ,i , sData ); 
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::XYSum4_duplication_allow()") );
		
			MakeSpeciesVolBookCnt( sData , sSpeciesCnt , sVolCnt , sBookCnt );
			
			sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
			sVolSum.Format( _T("%d") , _ttoi(sVolSum)+_ttoi(sVolCnt) );
			sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
		}
		
		sTotalSpeciesSum.Format( _T("%d") , _ttoi(sTotalSpeciesSum) + _ttoi(sSpeciesSum) );
		sTotalVolSum.Format( _T("%d") , _ttoi(sTotalVolSum) + _ttoi(sVolSum) );
		sTotalBookSum.Format( _T("%d") , _ttoi(sTotalBookSum) + _ttoi(sBookSum) );
		
		if ( m_pXSumDM->GetRowCount() < i+1 )
			m_pXSumDM->InsertRow(-1);
		
		if ( (_ttoi(sSpeciesSum) == 0) && (_ttoi(sVolSum) == 0) && (_ttoi(sBookSum) == 0) )
			sData = _T("0");
		else
			sData.Format( _T("%s\r\n[%s]\r\n(%s)") , sSpeciesSum , sVolSum , sBookSum );
		
		m_pXSumDM->SetCellData( sSumAlias , sData , i);		
	}
	
	// ==========================================================================================================
	// 2. 중복 토탈합계를 구한다.
	// ==========================================================================================================
	if ( m_pXSumDM->GetRowCount() < i+1 )
		m_pXSumDM->InsertRow(-1);
	
	if ( (_ttoi(sTotalSpeciesSum) == 0) && (_ttoi(sTotalVolSum) == 0) && (_ttoi(sTotalBookSum) == 0) )
		sData = _T("0");
	else
		sData.Format( _T("%s\r\n[%s]\r\n(%s)") , sTotalSpeciesSum , sTotalVolSum , sTotalBookSum );
	
	m_pXSumDM->SetCellData( sSumAlias , sData , i );
	
	// ==========================================================================================================
	/// 3. 세로 합계
	// ==========================================================================================================	
	sAlias.Empty();
	sSpeciesSum.Empty();
	sVolSum.Empty();
	sBookSum.Empty();
	sData.Empty();
	sSpeciesCnt.Empty();
	sVolCnt.Empty();
	sBookCnt.Empty();


	for ( i = 1;i < m_pRealResultDM->RefList.GetCount();i++ )
	{
		sSpeciesSum.Empty();sVolSum.Empty();sBookSum.Empty();
		pos = m_pRealResultDM->RefList.FindIndex(i);
		Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
		sAlias = Ref->FIELD_ALIAS;
		
		for ( INT j = 0;j < m_pRealResultDM->GetRowCount();j++ )
		{
			ids = m_pRealResultDM->GetCellData( sAlias , j , sData ); 
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::XYSum4_duplication_allow()") );
		
			MakeSpeciesVolBookCnt( sData , sSpeciesCnt , sVolCnt , sBookCnt );
			sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
			sVolSum.Format( _T("%d") , _ttoi(sVolSum)+_ttoi(sVolCnt) );
			sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
		}
		
		if ( m_pYSumDM->GetRowCount() < i )
			m_pYSumDM->InsertRow(-1);
		
		if ( (_ttoi(sSpeciesSum) == 0) && (_ttoi(sVolSum) == 0) && (_ttoi(sBookSum) == 0) )
			sData = _T("0");
		else
			sData.Format( _T("%s\r\n[%s]\r\n(%s)") , sSpeciesSum , sVolSum , sBookSum );
		
		m_pYSumDM->SetCellData( sSumAlias , sData ,i-1 );
	}	
		
	return 0;

EFS_END
	return -1;
}


// ##############################################################################################################
// [ 함수설명 ]
//   연속_소장종통계(신규구입이용통계,자료실별이용통계) 중복포함 합계
//   통계 결과를 로우별로 컬럼별로 합친다.
//
// [ 미니스펙 ]pjw작성
//
// ##############################################################################################################
INT CStaticsInfoMgr::New_XYSum4_duplication_allow()
{
EFS_BEGIN

	INT ids;
	
	// ==========================================================================================================
	// 중복 허용 합계 구성
	CString sSumAlias = _T("중복허용_합계");
	CString sAlias;
	CString sSpeciesSum,sBookSum;
	CString sData;
	CString sSpeciesCnt,sBookCnt;
	CString sTotalSpeciesSum,sTotalBookSum;
	CESL_DataMgr::reference *Ref;
	POSITION pos;
	// ==========================================================================================================
	
	// ==========================================================================================================
	// 1. 가로 합계 구성
	// ==========================================================================================================
	for( INT i = 0 ; i < m_pRealResultDM->GetRowCount() ; i++ )
	{
		sSpeciesSum.Empty(); sBookSum.Empty();
		for( INT j = 1 ; j < m_pRealResultDM->RefList.GetCount() ; j++ )
		{
			pos = m_pRealResultDM->RefList.FindIndex(j);
			Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
			sAlias = Ref->FIELD_ALIAS;
			
			ids = m_pRealResultDM->GetCellData( sAlias ,i , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::New_XYSum4_duplication_allow()") );
			
			if ( m_sIs64lib == _T("Y") && sAlias ==_T("3") )
			{
				MakeArmySpeciesBookCnt( sData, &sData );
				MakeSpeciesBookCnt( sData , sSpeciesCnt , sBookCnt );
			
				sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
				sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
			}
			else
			{
				MakeSpeciesBookCnt( sData , sSpeciesCnt , sBookCnt );
				
				sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
				sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
			}
		}
		
		sTotalSpeciesSum.Format( _T("%d") , _ttoi(sTotalSpeciesSum) + _ttoi(sSpeciesSum) );
		sTotalBookSum.Format( _T("%d") , _ttoi(sTotalBookSum) + _ttoi(sBookSum) );
		
		if( m_pXSumDM->GetRowCount() < i+1 )
			m_pXSumDM->InsertRow(-1);
		
		if( (_ttoi(sSpeciesSum) == 0) && (_ttoi(sBookSum) == 0) )
			sData = _T("0");		
		else  
			sData.Format( _T("%s\r\n(%s)") , sSpeciesSum , sBookSum );
		
		m_pXSumDM->SetCellData( sSumAlias , sData , i);
		
	}
	
	if( m_pXSumDM->GetRowCount() < i+1 )
		m_pXSumDM->InsertRow(-1);
	
	if( (_ttoi(sTotalSpeciesSum) == 0) && (_ttoi(sTotalBookSum) == 0) )
		sData = _T("0");
	else
		sData.Format( _T("%s\r\n(%s)") , sTotalSpeciesSum , sTotalBookSum );
	
	m_pXSumDM->SetCellData( sSumAlias , sData , i );

	// ==========================================================================================================
	// 중복 허용 합계 구성
	sAlias.Empty();
	sSpeciesSum.Empty();
	sBookSum.Empty();
	sData.Empty();
	sSpeciesCnt.Empty();
	sBookCnt.Empty();
	// ==========================================================================================================
	
	// ==========================================================================================================
	// 2. 세로 합계 구성
	// ==========================================================================================================
	for( i = 1 ; i < m_pRealResultDM->RefList.GetCount() ; i++ )
	{
		sSpeciesSum.Empty(); sBookSum.Empty();
		pos = m_pRealResultDM->RefList.FindIndex(i);
		Ref = (CESL_DataMgr::reference*)(m_pRealResultDM->RefList.GetAt(pos));
		sAlias = Ref->FIELD_ALIAS;
			
		for( INT j = 0 ; j < m_pRealResultDM->GetRowCount() ; j++ )
		{
			ids = m_pRealResultDM->GetCellData( sAlias , j , sData ); 
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::New_XYSum4_duplication_allow()") );
						
			// 2006-04-12 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// 육사의 군사분류기호 합계를 나타내기 위한 예외 처리
			if ( m_sIs64lib == _T("Y") && sAlias ==_T("3") )
			{
				// 육사는 종에다가 전부 기록한다.
				MakeArmySum( sSpeciesSum , sData, sSpeciesSum );				
			}
			else
			{
				MakeSpeciesBookCnt( sData , sSpeciesCnt , sBookCnt );
			
				sSpeciesSum.Format( _T("%d") , _ttoi(sSpeciesSum)+_ttoi(sSpeciesCnt) );
				sBookSum.Format( _T("%d") , _ttoi(sBookSum)+_ttoi(sBookCnt) );
			}
			// --------------------------------------------------------------------------------------------------
		}
		
		if( m_pYSumDM->GetRowCount() < i )
			m_pYSumDM->InsertRow(-1);
		
		if ( m_sIs64lib == _T("Y") && sAlias ==_T("3") )
			sData.Format( _T("%s"), sSpeciesSum );
		else
		{
			if( (_ttoi(sSpeciesSum) == 0) && (_ttoi(sBookSum) == 0) )
				sData = _T("0");
			else
				sData.Format( _T("%s\r\n(%s)") , sSpeciesSum , sBookSum );			
		}
		
		m_pYSumDM->SetCellData( sSumAlias , sData ,i-1 );		
	}	
		
	return 0;
EFS_END
	return -1;
}

//###############################################################################################################
// [ 함수설명 ]
//   육사 통계의 합계를 구한다.
//   Ex)
//    3,8  +  5,6  -->  8,14
//   (5,9)   (5,6)    (10,15)
//
// [ 미니스펙 ]
//
//###############################################################################################################
INT CStaticsInfoMgr::MakeArmySum(CString sData1, CString sData2, CString &sGetData)
{
EFS_BEGIN

	if ( !sData1.GetLength() )	sData1 = _T("0");
	if ( !sData2.GetLength() )	sData2 = _T("0");

	/************************************************************************************************************
	종과 책을 분리 시켜 동일한 형태로 만든다. 

		5,10  ->    5,10     9,26  두개로 분리한다.
	   (9,26) ->	
	************************************************************************************************************/
	// ==========================================================================================================
	// 1. sData1을 종과 책으로 분리
	// ==========================================================================================================
	CString sData1SpeCnt, sData1BookCnt;	
	DividArmyCell( sData1, sData1SpeCnt, sData1BookCnt );

	// ==========================================================================================================
	// 2. sData2을 종과 책으로 분리
	// ==========================================================================================================
	CString sData2SpeCnt, sData2BookCnt;
	DividArmyCell( sData2, sData2SpeCnt, sData2BookCnt );

	// ==========================================================================================================
	// 3. 분리된 결과를 종류에 맞게 서로 합친다.
	// ==========================================================================================================
	CString sTemp1, sTemp2, sTemp3, sTemp4;
	CString sSpeResult, sBookResult;

	// 종 부터
	DividNormalArmy( sData1SpeCnt, sTemp1, sTemp2 );
	DividNormalArmy( sData2SpeCnt, sTemp3, sTemp4 );
	sSpeResult.Format( _T("%d %s %d"), _ttoi( sTemp1 ) + _ttoi( sTemp3 ), DIVCHAR , _ttoi( sTemp2 ) + _ttoi( sTemp4 ) );
	
	// 책 부터
	DividNormalArmy( sData1BookCnt, sTemp1, sTemp2 );
	DividNormalArmy( sData2BookCnt, sTemp3, sTemp4 );
	sBookResult.Format( _T("(%d %s %d)"), _ttoi( sTemp1 ) + _ttoi( sTemp3 ), DIVCHAR , _ttoi( sTemp2 ) + _ttoi( sTemp4 ) );
	// ==========================================================================================================
	// 4. 마무리
	// ==========================================================================================================
	sGetData.Format( _T("%s\r\n%s"), sSpeResult, sBookResult );

	return 0;
EFS_END
	return -1;
}

//###############################################################################################################
// [ 함수설명 ]
//   육사 군사셀을 종과 책으로 분리한다.
//   Ex)
//    3,8  -->  3,8   5,9
//   (5,9) 
//
// [ 미니스펙 ]
//
//###############################################################################################################
INT CStaticsInfoMgr::DividArmyCell(CString sData, CString &sSpecies, CString &sBook)
{
EFS_BEGIN
	
	sData.Replace( _T(" "), _T("") );
	sData.Replace( _T("("), _T("") );sData.Replace( _T(")"), _T("") );

	INT nPos = sData.Find( _T("\r\n") );
	
	// ==========================================================================================================
	// 1. 개행문자가 없다면 종만 존재한다고 본다.
	// ==========================================================================================================
	if ( nPos == -1 )
	{
		nPos = sData.Find( DIVCHAR );

		if ( nPos == -1 )
		{
			// 이럴경우가 있을지는 모르지만 ;
			sSpecies = sData + _T(",0");
		}
		else
			sSpecies = sData;
		
		sBook = _T("0,0");		
	}
	// ==========================================================================================================
	// 2. 종책을 분리한다.
	// ==========================================================================================================
	else
	{
		sSpecies = sData.Mid( 0, nPos );
		sBook    = sData.Mid( nPos + 2 );		
	}

	return 0;
EFS_END
	return -1;
}


// KOL.RED.2018.211 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//

// 2018.07.11 ADD BY KHY: 통계결과 리스트 제공
/*//NEW------------------------------------------------------------------------*/					
INT CStaticsInfoMgr::RunBoStaticsList( CString sStaticsClass, CString sXValue, CString sYValue )
{
EFS_BEGIN

	// 1. 종수 통계를 가져온다.
	// 2. 책수 통계를 가져오면서 종수 통계에 값을 ADD한다.
	// 3. 종수 합계를 가져온다. 
	// 4. 책수 합계를 가져온다.
	// 5. 종수 합계에 책수 합계를 ADD한다.
	// 끝~~

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;	
	CString sXFieldName,sYFieldName;
	CString sXGroupYN , sYGroupYN;
	CString sWhere;

	// =========================================================================================================
	// 1. 통계를 내기 위한 기본값들 가져오기
	// =========================================================================================================	
	//ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN);

	//csj
	ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN,true);

	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::RunBoStaticsList()") );
	

	CString sTmpWhere;
	// =========================================================================================================
	// 2. 종 , 책의 통계를 만든다.
	// =========================================================================================================
	for ( INT i = 0;i < saTableName.GetSize();i++ )
	{
		m_pResultListDM->TBL_NAME = saTableName.GetAt(i);

		ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , _T("Y") , _T("Y") ,i , sWhere );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::RunBoStaticsList()") );

		ids = GetFieldName( sStaticsClass , sXAlias , sXFieldName , sXGroupYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::RunBoStaticsList()") );
		ids = GetFieldName( sStaticsClass , sYAlias , sYFieldName , sYGroupYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::RunBoStaticsList()") );

		if(!sXValue.IsEmpty())
		{
			if(sXValue != _T("NULL"))
			{
				sTmpWhere.Format( _T(" AND %s='%s'"),sXFieldName, sXValue);
			}
			else 
			{
				sTmpWhere.Format( _T(" AND %s IS NULL"),sXFieldName);
			}
			sWhere += sTmpWhere;
		}
		if(!sYValue.IsEmpty())
		{
			if(sYValue != _T("NULL"))
			{
				sTmpWhere.Format( _T(" AND %s='%s'"),sYFieldName, sYValue);
			}
			else 
			{
				sTmpWhere.Format( _T(" AND %s IS NULL"),sYFieldName);
			}
			sWhere += sTmpWhere;
		}

		ids = FillStaticsListValue2( sStaticsClass, sYFieldName, sWhere );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::RunBoStaticsList()") );
	}

	return 0;

EFS_END
	return -1;
}

INT CStaticsInfoMgr::FillStaticsListValue2(CString sStaticsClass, CString sYFieldName, CString sWhere)
{
	EFS_BEGIN

	INT ids;
	// SELECT 필드 설정
	// hykim
	/*
	ids = SetDataMgrRef( m_pResultListDM , _T("DISTINCT B.REG_NO") , _T(""), _T("Y"),  0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsListValue2") );
	ids = SetDataMgrRef( m_pResultListDM , _T("(SELECT TITLE_INFO FROM IDX_BO_TBL WHERE REC_KEY = B.SPECIES_KEY)") , _T("") , _T("Y"), 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("FillStaticsListValue2") );
	*/
	CString strListField;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("출력물") , strListField );
	if ( ids < 0  || strListField.IsEmpty() ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsListValue2()") );

	CStringArray saListField;
	ids = CLocCommonAPI::MakeArrayList( strListField , _T("/") , saListField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("FillStaticsListValue2()") );

	ids = SetDataMgrRefList( m_pResultListDM , _T("DISTINCT B.REC_KEY") , _T(""), _T("Y"), 0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("FillStaticsListValue2") );

	for(INT i = 0; i < saListField.GetSize() ; i++)
	{
		ids = SetDataMgrRefList( m_pResultListDM , saListField.GetAt(i) , _T(""), _T("Y"), i+1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("FillStaticsListValue2") );
	}

	// WHERE 조건 설정
	CString sSendWhere = sWhere;
	
	//===================================================
	//2009.10.05 ADD BY PJW : 일자별 조건이 들어가면 일자로 정렬한다.
	if( sYFieldName == _T("TO_CHAR(B.REG_DATE,'YYYY/MM/DD')") )
	{
		sSendWhere += _T(" ORDER BY TO_CHAR(B.REG_DATE,'YYYY/MM/DD')");
	}
	else if( sYFieldName == _T("TO_CHAR(B.SHELF_DATE,'YYYY/MM/DD')") )
	{
		sSendWhere += _T(" ORDER BY TO_CHAR(B.SHELF_DATE,'YYYY/MM/DD')");
	}
	else if( sYFieldName == _T("TO_CHAR(B.CHECKIN_DATE,'YYYY/MM/DD')") )
	{
		sSendWhere += _T(" ORDER BY TO_CHAR(B.CHECKIN_DATE,'YYYY/MM/DD')");
	}
	//===================================================

	// RESULT DM -> REAL RESULT DM 으로 이동
	ids = m_pResultListDM->RefreshDataManager( sSendWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("FillStaticsListValue2") );

	return 0;

	EFS_END
	return -1;
}


INT CStaticsInfoMgr::RunSeStaticsList(CString sStaticsClass, CString sXValue, CString sYValue)
{
EFS_BEGIN

	// 1. 종수 통계를 가져온다.
	// 2. 권수 통계를 가져온다. 종수 통계에 값을 ADD한다.	
	// 4. 책수 통계를 가져오면서 종권수 통계에 값을 ADD한다.
	// 5. 종수 합계를 가져온다. 
	// 6. 권수 합계를 가져온다. 
	// 7. 책수 합계를 가져온다.
	// 8. 종수 합계에 권수 합계를 ADD한다.
	// 9. 종권수 합계에 책수 합계를 ADD한다.
	// 끝~~

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;	
	CString sXFieldName,sYFieldName;
	CString sXGroupYN , sYGroupYN;	
	CString sWhere;

	// =========================================================================================================
	// 1. 통계를 내기 위한 기본값들 가져오기
	// =========================================================================================================
	//ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN);

	//csj
	ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN , true);

	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CStaticsInfoMgr::RunSeStaticsList()") );

	CString sXData,sYData,sTmp;
	INT nProgressCnt = 0;
	CString sTmpWhere;

	// =========================================================================================================
	// 2. 종 , 권 , 책의 통계를 만든다.
	// =========================================================================================================
	if(sStaticsClass == _T("자료실별이용통계") || sStaticsClass == _T("신규구입도서이용통계"))
	{
		for( INT i = 0; i < 2; i++ )
		{
			m_pResultListDM->TBL_NAME = saTableName.GetAt(i);	
			
			ids = GetSeWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , _T("Y") , _T("Y") ,i , sWhere );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::RunSeStaticsList()") );

			ids = GetFieldName( sStaticsClass , sXAlias , sXFieldName , sXGroupYN );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::RunBoStaticsList()") );
			ids = GetFieldName( sStaticsClass , sYAlias , sYFieldName , sYGroupYN );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::RunBoStaticsList()") );

			if(sXValue != "")
			{
				if(sXValue != _T("NULL"))
				{
					sTmpWhere.Format( _T(" AND %s='%s'"),sXFieldName, sXValue);
				}
				else 
				{
					sTmpWhere.Format( _T(" AND %s IS NULL"),sXFieldName);
				}
				sWhere += sTmpWhere;
			}
			if(sYValue != "")
			{
				if(sYValue != _T("NULL"))
				{
					sTmpWhere.Format( _T(" AND %s='%s'"),sYFieldName, sYValue);
				}
				else 
				{
					sTmpWhere.Format( _T(" AND %s IS NULL"),sYFieldName);
				}
				sWhere += sTmpWhere;
			}
				
			ids = FillStaticsListValue3(sStaticsClass, sWhere);
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::RunSeStaticsList()") );
		}
	}else
	{
		for( INT i = 0; i < 3; i++ )
		{
			m_pResultListDM->TBL_NAME = saTableName.GetAt(i);	
			
			ids = GetSeWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , _T(""),_T("") , _T("Y") , _T("Y") ,i , sWhere );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CStaticsInfoMgr::RunSeStaticsList()") );

			ids = GetFieldName( sStaticsClass , sXAlias , sXFieldName , sXGroupYN );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CStaticsInfoMgr::RunBoStaticsList()") );
			ids = GetFieldName( sStaticsClass , sYAlias , sYFieldName , sYGroupYN );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CStaticsInfoMgr::RunBoStaticsList()") );

			if(sXValue != "")
			{
				if(sXValue != _T("NULL"))
				{
					sTmpWhere.Format( _T(" AND %s='%s'"),sXFieldName, sXValue);
				}
				else 
				{
					sTmpWhere.Format( _T(" AND %s IS NULL"),sXFieldName);
				}
				sWhere += sTmpWhere;
			}
			if(sYValue != "")
			{
				if(sYValue != _T("NULL"))
				{
					sTmpWhere.Format( _T(" AND %s='%s'"),sYFieldName, sYValue);
				}
				else 
				{
					sTmpWhere.Format( _T(" AND %s IS NULL"),sYFieldName);
				}
				sWhere += sTmpWhere;
			}

			ids = FillStaticsListValue3(sStaticsClass, sWhere);
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CStaticsInfoMgr::RunSeStaticsList()") );
		}
	}

	return 0;

EFS_END
	return -1;
}

INT CStaticsInfoMgr::FillStaticsListValue3(CString sStaticsClass, CString sWhere)
{
EFS_BEGIN

	INT ids;

	// ==========================================================================================================
	// 1. SELECT 필드 설정
	// ==========================================================================================================
	/*
	ids = SetDataMgrRef( m_pResultListDM , _T("DISTINCT B.REG_NO") , _T(""), _T("Y"),  0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsListValue3") );
	ids = SetDataMgrRef( m_pResultListDM , _T("(SELECT TITLE_INFO FROM IDX_SE_TBL WHERE REC_KEY = B.SPECIES_KEY)") , _T("") , _T("Y"), 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("FillStaticsListValue3") );
	*/
	CString strListField;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("출력물") , strListField );
	if ( ids < 0 || strListField.IsEmpty() ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsListValue3()") );

	CStringArray saListField;
	ids = CLocCommonAPI::MakeArrayList( strListField , _T("/") , saListField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("FillStaticsListValue3()") );

	ids = SetDataMgrRefList( m_pResultListDM , _T("DISTINCT B.REC_KEY") , _T(""), _T("Y"), 0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("FillStaticsListValue3") );

	for(INT i = 0; i < saListField.GetSize(); i++)
	{
		ids = SetDataMgrRefList( m_pResultListDM , saListField.GetAt(i) , _T(""), _T("Y"), i+1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("FillStaticsListValue3") );
	}	

	// ==========================================================================================================
	// 2. WHERE 조건 설정
	// ==========================================================================================================
	CString sSendWhere = sWhere;

	// ==========================================================================================================
	// 3. RESULT DM -> REAL RESULT DM 으로 이동
	// ==========================================================================================================
	ids = m_pResultListDM->RefreshDataManager( sSendWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("FillStaticsListValue3") );
	return 0;

EFS_END
	return -1;
}

INT CStaticsInfoMgr::RunLoanStaticsList( CString sStaticsClass , CString sXValue, CString sYValue, bool bDupDel )
{
	EFS_BEGIN

	INT ids;

	CStringArray saTableName;
	CString sXAlias,sYAlias;
	CString sXFieldName,sYFieldName;
	CString sXGroupYN , sYGroupYN;
	CString sTableName;
	
	INT nPos = 1;

	// 통계를 내기 위한 기본값들 가져오는 건지 셋팅하는건지
	//ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN);

	//csj
	ids = InitRunStatics(sStaticsClass,saTableName,sXAlias,sYAlias,sXFieldName,sYFieldName,sXGroupYN,sYGroupYN , true);

	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunLoanStaticsList") );

	CString sResultField;
	if( sXAlias.Compare(_T("년")) == 0 || (sXAlias.Compare(_T("월")) == 0) || (sXAlias.Compare(_T("일")) == 0) )
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물") , sResultField );
	else
		ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("결과물2") , sResultField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RunLoanStaticsList") );


	// 통계중 일별로 통계를 내야하는것이 존재하는지 검사
	CString sDayField;
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("일별") , sDayField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InitRunStatics") );
	sDayField.TrimLeft(); sDayField.TrimRight();

	if( !sDayField.IsEmpty() )
	{
		ids = GetCellData( m_pUserMakeSearchCondDM , _T("알리아스") , sDayField , _T("FieldName") , sDayField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RunLoanStatics") );
	}

	for( INT i = 0 ; i < saTableName.GetSize() ; i++ )
	{
		// DM 테이블 이름 추가
		m_pResultListDM->TBL_NAME = saTableName.GetAt(i);

		sTableName = saTableName.GetAt(i);
		
		// 기본 검색 조건에 사용자검색조건,그룹조건을 포함시킨다.
		CString sWhere;
		
		ids = GetBoWhere(sStaticsClass , saTableName.GetAt(i) , sXFieldName , sYFieldName , sXValue, sYValue , _T("Y"), _T("Y"), i, sWhere);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RunLoanStaticsList") );

		ids = FillStaticsListValue( sStaticsClass, sXFieldName, sYFieldName, sXValue, sYValue, sWhere, bDupDel, sResultField, sDayField);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("RunLoanStaticsList") );
	}
	return 0;

	EFS_END
	return -1;

}

// 조건을 구성하여 값을 Real DM에 집에 넣는다.
INT CStaticsInfoMgr::FillStaticsListValue( CString sStaticsClass, CString sXFieldName, CString sYFieldName, CString sXData, CString sYData, CString sWhere, bool bDupDel, CString sResultField, CString sDayField)
{
	EFS_BEGIN

	INT ids;	
	CString sSendWhere;							// 보낼 SQL문	
	CString sTmpXData;
	CString strListField;
	CString sTableName = m_pResultListDM->TBL_NAME;
	
	ids = GetCellData( m_pStaticsMainInfoDM , _T("통계종류") , sStaticsClass , _T("출력물") , strListField );
	if ( ids < 0 || strListField.IsEmpty() ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("FillStaticsListValue()") );

	CStringArray saListField;
	ids = CLocCommonAPI::MakeArrayList( strListField , _T("/") , saListField );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("FillStaticsListValue()") );

	sResultField = GetRecKey(sResultField);

	if(!sResultField.IsEmpty() && (sXData.IsEmpty() || sYData.IsEmpty()))
	{	
		if(!bDupDel && sDayField.IsEmpty()) // 중복포함
		{
			ids = SetDataMgrRefList( m_pResultListDM , sResultField , _T(""), _T("Y"), 0);
		}
		else // 중복삭제
		{ 
			ids = SetDataMgrRefList( m_pResultListDM , _T("DISTINCT ") + sResultField , _T(""), _T("Y"), 0);
		}
	}
	else
	{
		if(sDayField.IsEmpty() || sResultField.IsEmpty())
		{
			ids = SetDataMgrRefList( m_pResultListDM , sResultField , _T(""), _T("N"), 0);
		}
		else // 중복삭제
		{
			ids = SetDataMgrRefList( m_pResultListDM , _T("DISTINCT ") + sResultField , _T(""), _T("Y"), 0);
		}
	}
	
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("FillStaticsListValue") );

	for(INT i = 0; i < saListField.GetSize(); i++)
	{
		ids = SetDataMgrRefList( m_pResultListDM , saListField.GetAt(i) , _T(""), _T("Y"), i+1);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("FillStaticsListValue") );
	}	

	if(!sWhere.IsEmpty())
	{
		CString sTmpWhere = _T("");
		if(sXData != "")
		{
			if(sXData != _T("NULL"))
			{
				sTmpWhere.Format( _T(" AND %s='%s'"),sXFieldName, sXData);
			}
			else 
			{
				sTmpWhere.Format( _T(" AND %s IS NULL"),sXFieldName);
			}
			sWhere += sTmpWhere;
		}
		if(sYData != "")
		{
			if(sYData != _T("NULL"))
			{
				sTmpWhere.Format( _T(" AND %s='%s'"),sYFieldName, sYData);
			}
			else 
			{
				sTmpWhere.Format( _T(" AND %s IS NULL"),sYFieldName);
			}
			sWhere += sTmpWhere;
		}
	}

	// WHERE 조건 설정
	sSendWhere = sWhere;

	CString sWhereSql;
	CString sSelectFields;
	CString sGroupByFields;

	if( !sDayField.IsEmpty() && !sResultField.IsEmpty() )
	{
		sSelectFields.Empty();
		sGroupByFields.Empty();
		sWhereSql.Empty();
		
		sSelectFields.Format( _T("%s") , sResultField );
		sGroupByFields.Format( _T("%s, %s") ,  sDayField, sResultField);			
		sWhereSql.Format( _T(" AND %s IN ( SELECT %s FROM %s WHERE %s GROUP BY %s )"), sResultField, sResultField , sTableName , sWhere , sGroupByFields );

		sSendWhere += sWhereSql;
	}

	// RESULT DM -> REAL RESULT DM 으로 이동
	ids = m_pResultListDM->RefreshDataManager( sSendWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("FillStaticsListValue") );

	return 0;

	EFS_END
	return -1;

}

// 통계종류에 따른 대상목록종류 결정
// Return Value (0: USER LIST, 1: BOOK LIST)
INT CStaticsInfoMgr::GetListType(CString sStaticsClass)
{

	if( sStaticsClass == _T("대출자 등록통계") || sStaticsClass == _T("미대출자 통계") || sStaticsClass == _T("대출자 통계") ||
	    sStaticsClass == _T("반납자 통계") || sStaticsClass == _T("회원증발급통계") )
	{
		return 0;
	}
	else //대출자료통계/반납자료통계/반납연기자료통계/야간대출자료통계
	{
		return 1;
	}
}

CString CStaticsInfoMgr::GetRecKey(CString sResultField)
{
	// 중복삭제를 위한 키값 찾기
	INT nStart, nEnd;
	CString keyField;
	nStart = sResultField.Find(_T("("));
	nEnd = sResultField.Find(_T(")"))-1;
	sResultField = sResultField.Mid(nStart+1, nEnd-nStart);
	sResultField.Replace(_T("DISTINCT"), _T(""));
	sResultField.Replace(_T("*"), _T(""));
	sResultField.TrimLeft();sResultField.TrimRight();
	return sResultField;
}

INT CStaticsInfoMgr::SetDataMgrRefList( CESL_DataMgr *pDM, CString sCondition, CString sInitValue, CString sGroupYN, INT nCol )
{
	EFS_BEGIN

	CString sUdf;

	if( sGroupYN.Compare(_T("Y")) == 0 ) sUdf = _T("");
	else sUdf = _T("udf");

	POSITION pos = pDM->RefList.FindIndex(nCol);
	if (pos == NULL) return -1;

	CESL_DataMgr::reference * pRef = (CESL_DataMgr::reference*)pDM->RefList.GetAt(pos);
	if (pRef == NULL) return -2;

	pRef->INIT_VALUE = sInitValue;
	pRef->DATA_TYPE = sUdf;
	pRef->FIELD_NAME = sCondition;

	return 0;

	EFS_END
	return -1;

}
/*//END------------------------------------------------------------------------*/


void	CStaticsInfoMgr::SetInit_CStatics_AXISVal_CELLCLICK(CStatics_AXISVal_CELLCLICK* p)
{
	m_pAxisCode_CellClick = p;
	m_pAxisCode_CellClick->Clear();
}


//-----------------------------------------------------------------------------

void	_CStatics_AXISVal_CELLCLICK_Alias::Clear()
{
	arStr_FieldName.RemoveAll();
	arStr_FieldValue.RemoveAll();
}

void	_CStatics_AXISVal_CELLCLICK_Alias::AddFieldNameVal( int index_Table , CString name , CString val )
{
	if( arStr_FieldName.GetSize() <= index_Table )
	{
		arStr_FieldName.Add( name );
		arStr_FieldValue.Add( val );
	}else{
		arStr_FieldName[index_Table]  = name;
		arStr_FieldValue[index_Table] = val;
	}
}

CString		_CStatics_AXISVal_CELLCLICK_Alias::GetWhere( int index_Table )
{
	if( arStr_FieldName.GetSize() <= index_Table ) return "";
	CString	where;
	where.Format( _T(" %s = '%s' ") , arStr_FieldName[index_Table]  , arStr_FieldValue[index_Table] );
	return where;
}
//-----------------------------------------------------------------------------


CStatics_AXISVal_CELLCLICK::CStatics_AXISVal_CELLCLICK()
{

}

CStatics_AXISVal_CELLCLICK::~CStatics_AXISVal_CELLCLICK()
{
	Clear();
}

void	CStatics_AXISVal_CELLCLICK::Clear()
{
	for( int i = 0 ; i < 2 ; i++ )	
	{
		CArray<_CStatics_AXISVal_CELLCLICK_Alias*,_CStatics_AXISVal_CELLCLICK_Alias*> *pAxis = GetAxis( i );
		for( int o = 0 ; o < pAxis->GetSize(); o++ )
		{
			_CStatics_AXISVal_CELLCLICK_Alias* p = pAxis->GetAt(o);
			p->Clear();
			delete p;
		}
		pAxis->RemoveAll();
	}
}

CArray< _CStatics_AXISVal_CELLCLICK_Alias*,_CStatics_AXISVal_CELLCLICK_Alias*>* CStatics_AXISVal_CELLCLICK::GetAxis( int axis )
{
	if( axis == 0 )
	{
		return &m_arAXISVal_X;
	}else if( axis == 1 ){
		return &m_arAXISVal_Y;
	}
	return NULL;
}	


void	CStatics_AXISVal_CELLCLICK::AddFieldNameVal( int axis , int arIdx , int index_Table , CString name , CString val )
{
	CArray<_CStatics_AXISVal_CELLCLICK_Alias*,_CStatics_AXISVal_CELLCLICK_Alias*>* pAxis = GetAxis( axis );

	_CStatics_AXISVal_CELLCLICK_Alias* pField = NULL;
	if( pAxis->GetSize() <= arIdx )
	{
		pField = new _CStatics_AXISVal_CELLCLICK_Alias();
		pAxis->Add( pField );
		pField->axis = axis;
	}

	if( pAxis->GetSize() <= arIdx )
	{
		// 에러!!! , 순차적으로 안들어왔다.
		// 이렇게 오는경우는 없겠지만..
		AfxMessageBox( _T("에러 : CStatics_AXISVal_CELLCLICK::AddFieldNameVal ") );
		return;
	}

	pField = pAxis->GetAt( arIdx );
	pField->AddFieldNameVal( index_Table , name , val );
}

void	CStatics_AXISVal_CELLCLICK::SetCodeToDesc( int axis , int arIdx , CString Alias , CString Code , CString Desc )
{
	CArray<_CStatics_AXISVal_CELLCLICK_Alias*,_CStatics_AXISVal_CELLCLICK_Alias*>* pAxis = GetAxis( axis );
	if( pAxis->GetSize() <= arIdx )
	{
		AfxMessageBox( _T("에러 : CStatics_AXISVal_CELLCLICK::SetCodeToDesc ") );
		return;
	}
	_CStatics_AXISVal_CELLCLICK_Alias*	pField = pAxis->GetAt( arIdx );
	pField->Code = Code;
	pField->Desc = Desc;
}

// int axis : 축 가로 세로
// int index_Table : 테이블 인덱스 ( 단행 테이블 , 연속테이블 )
// CString Desc_Click : 그리드에서 클릭한 셀의 축 컬럼 이름 ( ESL_Grid 에서 가져옴 )
// 
int		CStatics_AXISVal_CELLCLICK::GetWhere( int axis , int index_Table , CString Desc_Click , CString& out_Where )
{	
	out_Where = _T("");

	CArray<_CStatics_AXISVal_CELLCLICK_Alias*,_CStatics_AXISVal_CELLCLICK_Alias*>* pAxis = GetAxis( axis );
	
	int i;
	_CStatics_AXISVal_CELLCLICK_Alias* pField = NULL;
	for( i = 0 ; i < pAxis->GetSize() ; i ++)
	{
		_CStatics_AXISVal_CELLCLICK_Alias* p = pAxis->GetAt(i);
		if( p->Desc == Desc_Click )
		{
			pField = p;
			break;
		}
	}

	if( pField == NULL ) return -1;

	CString where_field = pField->GetWhere( index_Table );
	if( where_field.IsEmpty() )
	{
		return -1;
	}
	where_field = _T(" AND ") + where_field;
	out_Where = where_field;
	return 0;
}

//-----------------------------------------------------------------------------

//
// KOL.RED.2018.211 ---------------------------------------------------------------------------
