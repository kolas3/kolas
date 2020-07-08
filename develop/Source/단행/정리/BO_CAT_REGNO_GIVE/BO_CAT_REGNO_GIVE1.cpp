// BO_CAT_REGNO_GIVE1.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_REGNO_GIVE1.h"
#include "BO_CAT_REGNO_0305.h"
#include "BO_CAT_REGNO_0310.h"
#include "..\BO_CAT_LAST_NUMBER\BO_CAT_LAST_NUMBER_0000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// BO_CAT_REGNO_GIVE dialog


BO_CAT_REGNO_GIVE::BO_CAT_REGNO_GIVE(CESL_Mgr* pParent, CString sRecKey)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(BO_CAT_REGNO_GIVE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strBookKeys = sRecKey;
	
	m_pDM = NULL;

	m_nOpenMode = 0;

	m_bSetDropNo = FALSE;

	m_pDM_Accession = NULL;	
}

BO_CAT_REGNO_GIVE::~BO_CAT_REGNO_GIVE()
{	
	
}

BOOL BO_CAT_REGNO_GIVE::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID BO_CAT_REGNO_GIVE::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(BO_CAT_REGNO_GIVE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(BO_CAT_REGNO_GIVE, CDialog)
	//{{AFX_MSG_MAP(BO_CAT_REGNO_GIVE)
	ON_BN_CLICKED(IDC_btnCancel, OnbtnCancel)
	ON_BN_CLICKED(IDC_bACCESSIONNO_GIVE, OnbACCESSIONNOGIVE)
	ON_BN_CLICKED(IDC_bAccessionInput, OnbAccessionInput)
	ON_BN_CLICKED(IDC_bMissNumber, OnbMissNumber)
	ON_BN_CLICKED(IDC_bLastNumber, OnbLastNumber)
	ON_WM_CTLCOLOR() 
	ON_EN_KILLFOCUS(IDC_eACCESSION_REC_NO, OnKillfocuseACCESSIONRECNO)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// BO_CAT_REGNO_GIVE message handlers

BOOL BO_CAT_REGNO_GIVE::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
		
	if (InitESL_Mgr(_T("BO_CAT_REG_0300_NEW")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	// 모드 - 1:정리, 2:수서
	if (m_strBookKeys.IsEmpty()) m_nMode = 1;
	else m_nMode = 2;

	INT ids = IntDialogNum();
	if (ids<0) return FALSE;

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

INT BO_CAT_REGNO_GIVE::IntDialogNum()
{
EFS_BEGIN

	CString sUserName = m_pInfo->USER_NAME;
	if ( sUserName == _T("") ) 
		sUserName = m_pInfo->USER_ID;

	SetControlData( _T("CM_BO_CAT_REG_0300"), _T("등록자"), sUserName );
	
	m_pDM = FindDM(_T("DM_BO_CAT_REG_0300"));
	if ( m_pDM == NULL ) 
	{
		AfxMessageBox( _T("DM_BO_CAT_REG_0300") );
		return -10;
		EndDialog(IDCANCEL);
	}

	INT ids = -1;
	INT idx = -1;
	CString strRegCode;
	CString strSpeciesKey;		// 종키 추가 040225_HSS_추가

	CStringArray aBookKeyList;
	CString strRecKeyTemp;

	INT nCnt = 0; // 040325_HSS 1000개씩 자르기 위해 카운트 세기
	CESL_DataMgr * pDM_1200_CONTROL = FindDM( _T("DM_BO_CAT_REGNO") );

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	if (m_strBookKeys.IsEmpty())
	{
		// 이종은 추가 : DM_BO_CAT_REGNO 에 선정된 순서대로 책키, 등록구분 코드 를 넣는다.
		//CESL_DataMgr * pDM_1200_CONTROL = FindDM( _T("DM_BO_CAT_REGNO") );
		if (!pDM_1200_CONTROL)
		{
			AfxMessageBox(_T("DM_BO_CAT_REGNO Find Error"));
			EndDialog(IDCANCEL);
			return -10;
		}
		pDM_1200_CONTROL->FreeData();

		SelectMakeList(m_strParentCMAlias, _T("MAIN_GRID"));
		ids = SelectGetCount(m_strParentCMAlias, _T("MAIN_GRID"));
		if (ids < 1)
		{
			// 에러
			AfxMessageBox(_T("선정된 자료가 없습니다."));	
			EndDialog(IDCANCEL);
			m_bInitDlg = FALSE;
			return -10;
		}

		CESL_DataMgr *pDM_Main = FindDM(m_strParentDMAlias);
		if ( NULL == pDM_Main ) {
			AfxMessageBox ( _T("DM_단행_정리_등록_메인 error") );
			EndDialog(IDCANCEL);
			return -10;
		}

		idx = SelectGetHeadPosition(m_strParentCMAlias, _T("MAIN_GRID"));

		// 040317_HSS 수입구분 입력
		CString strAcq_Code;
		strAcq_Code = pDM_Main->GetCellData(_T("BB_수입구분"), idx);
		SetControlData(_T("CM_BO_CAT_REG_0300"), _T("수입구분"), strAcq_Code );

		// 책키와 등록구분 코드를 넣는다.
		while ( idx >= 0 ) 
		{
			// 040325_HSS 추가 카운트 세기
			nCnt++;
			strRecKeyTemp = pDM_Main->GetCellData( _T("BB_책KEY") , idx );
			strRegCode = pDM_Main->GetCellData(_T("BB_등록구분_코드"), idx);
			strSpeciesKey = pDM_Main->GetCellData(_T("IBS_종KEY"), idx);

			pDM_1200_CONTROL->InsertRow(-1);
			INT nRow_1200_CONTROL = pDM_1200_CONTROL->GetRowCount() - 1;
			pDM_1200_CONTROL->SetCellData(_T("BB_책KEY"), strRecKeyTemp, nRow_1200_CONTROL);
			pDM_1200_CONTROL->SetCellData(_T("BB_등록구분_코드"), strRegCode, nRow_1200_CONTROL);
			pDM_1200_CONTROL->SetCellData(_T("BB_종키"), strSpeciesKey, nRow_1200_CONTROL);

			if (m_strBookKeys.GetLength() > 0 ) m_strBookKeys += _T(", ");
			m_strBookKeys += strRecKeyTemp;

			
			/*******************************************************
			 *******************************************************/
			// 20080128 UPDATE BY PJW : 1000의 배수를 선정했을 에러나지 않도록 수정 
			idx = SelectGetNext(m_strParentCMAlias, _T("MAIN_GRID"));
			if( idx < 0 )
			{
				break;
			}
			
			if ( (nCnt % 1000) == 0 ) {
				aBookKeyList.Add( m_strBookKeys );
				m_strBookKeys = _T("");
			}
			/*
			// 040325_HSS 수정 - 1000개씩 잘라서 CStringArray에 넣기
			if ( (nCnt % 1000) == 0 ) {
				aBookKeyList.Add( m_strBookKeys );
				m_strBookKeys = _T("");
			}
			idx = SelectGetNext(m_strParentCMAlias, _T("MAIN_GRID"));
			*/
			/*******************************************************
			 *******************************************************/

			
		}
	}
	else 
	{	
		//수서모드일때 누락등록번호적용 없애기
		//GetDlgItem(IDC_bMissNumber)->ShowWindow(SW_HIDE);
	}


	aBookKeyList.Add( m_strBookKeys );

	// 부록등록번호 자동부여 체크
	CString sQueryYn = _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_2='기타' AND GROUP_3='공통' AND CLASS_ALIAS='부록등록번호자동부여여부' AND MANAGE_CODE = UDF_MANAGE_CODE");
	CString sItem;
	m_pDM->GetOneValueQuery( sQueryYn , sItem );

	CString sAppRecKey;
	if ( sItem == _T("Y") ) {		// 자동부여일 경우 부록도 포함하여 등록번호 부여
		CESL_DataMgr DM_APPENDIX;
		DM_APPENDIX.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

		CString sAppQuery = _T("SELECT REC_KEY, REG_CODE, SPECIES_KEY FROM bo_book_tbl WHERE REG_CODE IS NOT NULL AND REG_NO IS NULL AND MANAGE_CODE = UDF_MANAGE_CODE AND book_appendix_flag = 'A' AND species_key IN (SELECT species_key FROM bo_book_tbl WHERE rec_key IN ( ") + m_strBookKeys + _T(" ) )");
		DM_APPENDIX.RestructDataManager(sAppQuery);
		INT iAppCount = DM_APPENDIX.GetRowCount();
		for ( INT i = 0 ; i < iAppCount ; i++ ) {
			sAppRecKey = sAppRecKey + _T(", ") + DM_APPENDIX.GetCellData( i, 0 );
			
			pDM_1200_CONTROL->InsertRow(-1);
			INT nRow_1200_CONTROL = pDM_1200_CONTROL->GetRowCount() - 1;
			
			strRecKeyTemp = DM_APPENDIX.GetCellData( i, 0 );
			strRegCode = DM_APPENDIX.GetCellData( i, 1 );
			strSpeciesKey = DM_APPENDIX.GetCellData( i, 2 );

			pDM_1200_CONTROL->SetCellData(_T("BB_책KEY"), strRecKeyTemp, nRow_1200_CONTROL);
			pDM_1200_CONTROL->SetCellData(_T("BB_등록구분_코드"), strRegCode, nRow_1200_CONTROL);
			pDM_1200_CONTROL->SetCellData(_T("BB_종키"), strSpeciesKey, nRow_1200_CONTROL);
		}
		if ( sAppRecKey.GetLength() > 2 ) {
			sAppRecKey = sAppRecKey.Mid(2);
			aBookKeyList.Add( sAppRecKey );
		}
	}

	// Query 만들기
	m_strWhere.Format( _T(" REC_KEY IN (%s) GROUP BY REG_CODE"), m_strBookKeys );
	CString strWhere;
	//strWhere.Format( _T("REC_KEY IN (%s)"), m_strBookKeys );
	strWhere.Format( _T("REC_KEY IN (%s)"), aBookKeyList.GetAt(0) );

	// 연속
	if(m_nOpenMode == 3)
		m_pDM->TBL_NAME = _T("SE_BOOK_TBL");

	if (m_pDM->RefreshDataManager(strWhere) < 0)	
		return -10;
	
	INT iRowCount = m_pDM->GetRowCount();
	if ( iRowCount < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
		return -10;
	}

	if ( aBookKeyList.GetSize() > 1 ) {
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
		for ( INT i = 1 ; i < aBookKeyList.GetSize() ; i++ ) {
			CString sQuery = _T("SELECT COUNT(REG_CODE), REG_CODE FROM BO_BOOK_TBL WHERE REC_KEY IN ( ") + aBookKeyList.GetAt(i) + _T(" ) GROUP BY REG_CODE");
			dm.RestructDataManager(sQuery);
			for ( INT j = 0 ; j < dm.GetRowCount() ; j++ ) {
				CString sRegCode = dm.GetCellData( j, 1 );
				INT iFind = 0;
				for ( INT k = 0 ; k < m_pDM->GetRowCount() ; k++ ) {
					if ( sRegCode == m_pDM->GetCellData( _T("BB_등록구분_코드"), k ) ) {
						INT iSum = _ttoi( dm.GetCellData( j, 0 ) ) + _ttoi( m_pDM->GetCellData( _T("책수"), k ) );
						CString sSum;
						sSum.Format( _T("%d"), iSum );
						m_pDM->SetCellData( _T("책수"), sSum, k );
						iFind = 1;
						break;
					}
				}
				if ( iFind == 0 ) {
					m_pDM->InsertRow(-1);
					//m_pDM->SetCellData( _T("BB_등록구분_코드"), dm.GetCellData( j, 1 ), m_pDM->GetRowCount() );
					//m_pDM->SetCellData( _T("책수"), dm.GetCellData( j, 0 ), m_pDM->GetRowCount() );
					CString sTmp = dm.GetCellData( j, 1 );
					INT iTmp = m_pDM->GetRowCount();
					CString sTmp1 = dm.GetCellData( j, 0 );
					m_pDM->SetCellData( _T("BB_등록구분_코드"), dm.GetCellData( j, 1 ), m_pDM->GetRowCount()-1 );
					m_pDM->SetCellData( _T("책수"), dm.GetCellData( j, 0 ), m_pDM->GetRowCount()-1 );
				}
			}
			//등록번호 부여된 자료 찾기
			CString sQueryCount;
			sQueryCount.Format(_T("SELECT COUNT(TRIM(REG_NO)) FROM BO_BOOK_TBL WHERE REC_KEY IN(%s)"), aBookKeyList.GetAt(i) );
			CString sCount;	
			m_pDM->GetOneValueQuery( sQueryCount , sCount );
			if ( sCount != _T("0") ) {
				AfxMessageBox (_T("등록번호 부여된 자료가 있습니다."));
				m_bInitDlg = FALSE;
				return -10;
			}
		}
	}
	
	//등록번호 부여된 자료 찾기
	CString sQueryCount;
	//sQueryCount.Format(_T("SELECT COUNT(REG_NO) FROM BO_BOOK_TBL WHERE REC_KEY IN(%s)"), m_strBookKeys);
	sQueryCount.Format(_T("SELECT COUNT(TRIM(REG_NO)) FROM BO_BOOK_TBL WHERE REC_KEY IN(%s)"), aBookKeyList.GetAt(0) );

	CString sCount;	
	m_pDM->GetOneValueQuery( sQueryCount , sCount );
	if ( sCount != _T("0") ) {
		AfxMessageBox (_T("등록번호 부여된 자료가 있습니다."));
		m_bInitDlg = FALSE;
		return -10;
	}

	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_CAT_REG_0300"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_REG error") );
		return -10;
	}
	pCM->AllControlDisplay();	

	//등록번호 그리드에 넣기
	INT iBookCount_tot = 0;
	CESL_Grid * pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM->FindControl(_T("REG_0300_GRID"));
	pGrid_sub->SetSort(FALSE);

	iRowCount = m_pDM->GetRowCount();

	for( INT i = 1 ; i <= iRowCount ; i++ ) 
	{
		CString sRegCode = m_pDM->GetCellData( _T("BB_등록구분_코드"), i-1 );
		CString sBookCount = m_pDM->GetCellData( _T("책수"), i-1 );
				
		CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND DIVISION_VALUE = '") + sRegCode + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE");

		//마지막등록번호		
		CString sLastNum;	
		m_pDM->GetOneValueQuery( sQuery , sLastNum );
		
		//등록구분이 없으면 새로추가		
		if ( sLastNum == _T("") ) {
			ids = m_pDM->StartFrame();
			if ( 0 > ids ) return -1;
			CString sRecKey;
			ids = m_pDM->MakeRecKey( sRecKey );
			if ( 0 > ids ) return -1;
			CString sQuery_reg;
			CTime time = CTime::GetCurrentTime();
			CString sYear;
			sYear.Format( _T("%04d"), time.GetYear() );
			sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER, MANAGE_CODE) ")
							  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s', UDF_MANAGE_CODE );") 
//DEL 			sQuery_reg.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , PUBLISH_FORM_CODE , KIND_CODE , DIVISION_VALUE , MAKE_YEAR , LAST_NUMBER) ")
//DEL 							  _T("VALUES ( '%s' , '%s' , '%s' , '%s' , '%s' , '%s' );") 
							  , sRecKey , _T("CO") , _T("REG_NO") , sRegCode , sYear , _T("0"));
			ids = m_pDM->AddFrame( sQuery_reg );
			if ( 0 > ids ) return -1;

			ids = m_pDM->SendFrame();			
			if (ids < 0) {
				AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다(DM_REG)."));
				return -10;
			}
			ids = m_pDM->EndFrame();
			if ( 0 > ids ) return -1;
		}

		//등록번호 넣기
		INT iLastNum = _ttoi( sLastNum.GetBuffer(0) );
		iLastNum ++;		
		sLastNum.Format( _T("%d") , iLastNum );
		m_pDM->SetCellData( _T("마지막번호"), sLastNum ,i-1 );
		//등록번호 12자리		
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		CString sNum10;
// 		sNum10.Format ( _T("%010d") , iLastNum );
// 
// 		//시작, 마지막등록번호
// 		CString sStartRegNo, sEndRegNo;
// 		sStartRegNo.Format( _T("%s%s"), sRegCode, sNum10 );
		//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
		//시작, 마지막등록번호
		CString strRegNum;
		CString sStartRegNo, sEndRegNo;

		strRegNum.Format(_T("%d"), iLastNum);
		sStartRegNo = m_pInfo->MakeRegNo(sRegCode, strRegNum );
		strRegNum = sStartRegNo.Mid(nRegCodeLength) ;
		//=====================================================
		
		m_pDM->SetCellData( _T("처음등록번호"), sStartRegNo, 0 );
		
		//책수
		INT iBookCount;
		iBookCount = _ttoi(sBookCount.GetBuffer(0));
		iLastNum = iLastNum + iBookCount -1 ;
		iBookCount_tot = iBookCount_tot + iBookCount;
		CString sTmp;
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		sTmp.Format( _T("%010d") , iLastNum );		
// 		sLastNum = sNum10 + _T(" ~ ") + sRegCode + sTmp;
// 		pGrid_sub->SetTextMatrix( i, 3, sRegCode + sLastNum);
// 		//마지막 등록번호
// 		sEndRegNo.Format( _T("%s%s"), sRegCode, sTmp );
		sTmp.Format( _T("%d") , iLastNum );
		sTmp = m_pInfo->MakeRegNo(sRegCode , sTmp);
		sLastNum = sStartRegNo + _T(" ~ ") + sTmp;
		pGrid_sub->SetTextMatrix( i, 3, sLastNum);
		//마지막 등록번호
		sEndRegNo.Format( _T("%s"), sTmp );

		//=====================================================
		

		
		m_pDM->SetCellData( _T("마지막등록번호"), sEndRegNo, 0 );
	}

	//마지막 원부번호 검색
	CTime time = CTime::GetCurrentTime();
	INT iYear = time.GetYear();
	CString sYear;
	sYear.Format( _T("%d"), iYear );

	CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'ACC_NO' AND MAKE_YEAR = '") + sYear + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE");
	CString sResult;
	m_pDM->GetOneValueQuery( sQuery , sResult );
	INT iAccNum;
	iAccNum = _ttoi(sResult.GetBuffer(0));
	iAccNum ++;
	sResult.Format( _T("%d") , iAccNum );
	SetControlData( _T("CM_BO_CAT_REG_0300") , _T("원부번호") , sResult);

	m_strAccNo = sResult;
	
	//책수 넣기
	CString sBookCount;
	sBookCount.Format( _T("%d") , iBookCount_tot );

	SetControlData( _T("CM_BO_CAT_REG_0300"), _T("책수"), sBookCount );

	return 0;

EFS_END
return -1;
}

INT BO_CAT_REGNO_GIVE::AccessionInput()
{
EFS_BEGIN

	//원부 insert 시작
	//값 가져오기
	CString sPROVIDER, sCLASS_NAME, sACCESSION_REC_NO;
	CString sACCESSION_REC_MAKE_YEAR;
	INT ids = -1;


	GetControlData(_T("CM_BO_CAT_REG_0300"), _T("수입년도"), sACCESSION_REC_MAKE_YEAR);
	GetControlData(_T("CM_BO_CAT_REG_0300"), _T("원부번호"), sACCESSION_REC_NO);

	//원부번호가 입력되었는지 확인
	//TRUE-입력, FALSE-없음
	BOOL bInputAccessionNo = (sACCESSION_REC_NO.IsEmpty()) ? FALSE : TRUE;	
	

	//원부번호가 입력되었을 경우
	if ( bInputAccessionNo == TRUE ) 
	{	
		// 050104_소정 추가 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 원부 마지막번호는 입력된 년도의 마지막번호로 다시 검색해서 비교
		CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'ACC_NO' AND MAKE_YEAR = '") + sACCESSION_REC_MAKE_YEAR + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE");
		CString sResult;
		m_pDM->GetOneValueQuery( sQuery , sResult );
		INT iAccNum;
		iAccNum = _ttoi(sResult.GetBuffer(0));
		iAccNum ++;
		sResult.Format( _T("%d") , iAccNum );

		m_strAccNo = sResult;
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//마지막 원부번호보다 크면 안된다.
		if ( _ttoi(sACCESSION_REC_NO) > _ttoi(m_strAccNo) ) {
			AfxMessageBox( _T("원부번호는 마지막원부번호 보다 작아야 합니다.") );
			return -1;
		}
			

		m_pDM_Accession = FindDM(_T("DM_BO_CAT_REG_0500"));
		if (!m_pDM_Accession) return -1;

		ids = m_pDM_Accession->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return -1;
		}

		//원부정보를 DM에 저장한다.
		MakeDMInfo( sACCESSION_REC_NO , sACCESSION_REC_MAKE_YEAR );
		
		if ( _ttoi(sACCESSION_REC_NO) == _ttoi(m_strAccNo) ) 
		{		
			//마지막 번호 Table에 마지막 원부번호 Update Or Insert
			if ( !AccNoUpdateOrInsertLastNoTbl() ) return -1;
		}

	}

	return 0;

EFS_END
return -1;

}

INT BO_CAT_REGNO_GIVE::BookDmInsert()
{
EFS_BEGIN

	//등록번호부여 DM
	CESL_DataMgr * pDM_1200_CONTROL;
	pDM_1200_CONTROL = FindDM( _T("DM_BO_CAT_REGNO") );
	if ( NULL == pDM_1200_CONTROL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_1200_CONTROL error") );
		return FALSE;
	}

	CString strWhere;
	//strWhere = _T("REC_KEY IN(") + sRecKey + _T(") ORDER BY REG_CODE");
	strWhere = _T("ROWNUM > 0 ORDER BY REG_CODE");
	if (pDM_1200_CONTROL->RefreshDataManager(strWhere) < 0)	return false;
	INT iRowControlCount = pDM_1200_CONTROL->GetRowCount();
	if ( pDM_1200_CONTROL->GetRowCount() < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}
	//iSpeciesCount = iRowControlCount;
	
	for ( INT j = 1 ; j <= iRowControlCount ; j ++ ) {
		CString sControlCodeSpecies;
		pDM_1200_CONTROL->GetCellData( _T("BB_등록구분_코드") , j-1 , sControlCodeSpecies );
	}

	return 0;

EFS_END
return -1;

}

INT BO_CAT_REGNO_GIVE::BookUpdate()
{
EFS_BEGIN

	CString sQuery;
	INT ids = -1;
	
	//메인그리드
	CESL_DataMgr * pDM_REG_0000;
	
	if ( m_nMode == 1 ) {
		pDM_REG_0000 = FindDM(m_strParentDMAlias);
		if ( NULL == pDM_REG_0000 ) {
			AfxMessageBox ( _T("DM_BO_CAT_REG error") );
			return -1;
		}
	}

	CESL_DataMgr * pDM_REG_0300;
	pDM_REG_0300 = FindDM( _T("DM_BO_CAT_REG_0300") );
	if ( NULL == pDM_REG_0300 ) {
		AfxMessageBox( _T("pDM_REG_0300 error") );
		return -1;
	}
	INT iRowCount = pDM_REG_0300->GetRowCount();

	//등록번호부여 DM
	CESL_DataMgr * pDM_1200_CONTROL;
	pDM_1200_CONTROL = FindDM( _T("DM_BO_CAT_REGNO") );
	if(m_nOpenMode==3)
	{
		pDM_1200_CONTROL->TBL_NAME=_T("SE_BOOK_TBL");
	}

	if ( NULL == pDM_1200_CONTROL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_1200_CONTROL error") );
		return FALSE;
	}
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 050408_원부번호 없어도 등록번호 부여 가능하도록 추가
	if ( NULL == m_pDM_Accession )
	{
		m_pDM_Accession = FindDM(_T("DM_BO_CAT_REG_0500"));
		if (!m_pDM_Accession) return -1;

		ids = m_pDM_Accession->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return -1;
		}
	}
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// 중복된 등록번호가 있는지 체크
	if ( m_bSetDropNo == FALSE ) {		// 마지막번호 적용시
		CString sStartRegNo, sEndRegNo;
		sStartRegNo = pDM_REG_0300->GetCellData( _T("처음등록번호"), 0 );
		sEndRegNo = pDM_REG_0300->GetCellData( _T("마지막등록번호"), 0 );
		CString sQueryCount;

		if(m_nOpenMode == 1)	// 단행
			sQueryCount.Format( _T("select count(TRIM(reg_no)) from BO_BOOK_TBL WHERE REG_NO >= '%s' AND REG_NO <= '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), sStartRegNo, sEndRegNo );
		else if(m_nOpenMode == 3)	// 연속
			sQueryCount.Format( _T("select count(TRIM(reg_no)) from SE_BOOK_TBL WHERE REG_NO >= '%s' AND REG_NO <= '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), sStartRegNo, sEndRegNo );

		CString sCount;	//시작 등록번호
		m_pDM->GetOneValueQuery( sQueryCount , sCount );
		if ( sCount != _T("0") ) {
			AfxMessageBox (_T("소장자료중에 중복된 등록번호가 있습니다."));
			m_bInitDlg = FALSE;
			return -1;
		}
	}

	INT iRowControlCount = pDM_1200_CONTROL->GetRowCount();
	if (iRowControlCount < 1)
		return -1;

	//원부번호
	CString sAccNo;
	GetControlData( _T("CM_BO_CAT_REG_0300"), _T("원부번호"), sAccNo );

	CString sRecKey;				// 책키
	CString sControlCodeSpecies;	// 등록구분
	CString sSpeciesKey;			// 종키


	//누락등록번호적용
	if ( m_bSetDropNo == TRUE ) {
		CESL_DataMgr *pDM_0310_DROP;
		pDM_0310_DROP = FindDM(_T("DM_BO_CAT_REG_311_DROP"));
		if ( pDM_0310_DROP == NULL ) return -1;
		INT iDropCount = pDM_0310_DROP->GetRowCount();
		
		//pDM_0310_DROP sort
		for ( INT i = 0 ; i < iDropCount-1 ; i++ ) {
			CString sRegCode1 = pDM_0310_DROP->GetCellData( _T("누락등록번호"), i );
			for ( INT j = i+1 ; j < iDropCount ; j++ ) {
				CString sRegCode2 = pDM_0310_DROP->GetCellData( _T("누락등록번호"), j );
				if ( sRegCode1 < sRegCode2 ) {
					CString sTmp;
					sTmp = sRegCode1;
					sRegCode1 = sRegCode2;
					sRegCode2 = sTmp;
					pDM_0310_DROP->SetCellData( _T("누락등록번호"), sRegCode1, i );
					pDM_0310_DROP->SetCellData( _T("누락등록번호"), sRegCode2, j );
				}
			}			
		}

		for ( INT j = 1 ; j <= iRowControlCount ; j ++ ) {

			INT iBookRow = iRowControlCount - j;
			pDM_1200_CONTROL->GetCellData( _T("BB_책KEY") , iBookRow , sRecKey );
			CString sRegNo;
			pDM_0310_DROP->GetCellData( _T("누락등록번호"), j-1 , sRegNo );
			pDM_1200_CONTROL->GetCellData( _T("BB_종키") ,  j-1 , sSpeciesKey );

			if(m_nOpenMode == 3)	// 연속인 경우 제본된 책도 같이 변경
			{
				sQuery.Format( _T("UPDATE SE_BOOK_TBL SET REG_NO = '%s' , ACCESSION_REC_KEY = %s, WORKING_STATUS = 'SER112N', REG_DATE = '%s', LAST_WORK='%s'  WHERE (REC_KEY =%s) OR (BINDING_BOOK_KEY = %s);") , sRegNo, m_strAccRecKey, CTime::GetCurrentTime().Format(_T("%Y/%m/%d")), GetWorkLogMsg(_T("등록번호부여"),WORK_LOG), sRecKey, sRecKey );
			}
			else
			{
				sQuery.Format( _T("UPDATE BO_BOOK_TBL SET REG_NO = '%s' , ACCESSION_REC_KEY = %s, WORKING_STATUS = 'BOR112N', REG_DATE = '%s', LAST_WORK='%s' WHERE REC_KEY =%s;") , sRegNo, m_strAccRecKey, CTime::GetCurrentTime().Format(_T("%Y/%m/%d")), GetWorkLogMsg(_T("등록번호부여"),WORK_LOG), sRecKey );
			}
			ids = m_pDM_Accession->AddFrame( sQuery );
			if ( 0 > ids ) return -1;

			// 040225_HSS IDX_BO_TBL, IDX_TMP_BO_TBL 추가 ++++++++++++++
			if(m_nOpenMode == 1)	// 단행일 경우만
			{
				pDM_1200_CONTROL->GetCellData( _T("BB_종키") , iBookRow , sSpeciesKey );
				sQuery.Format( _T("UPDATE IDX_BO_TBL SET WORKING_STATUS = 'BOR112N', LAST_WORK='%s' WHERE REC_KEY =%s;")
								, GetWorkLogMsg(_T("등록번호부여"),WORK_LOG), sSpeciesKey );
				ids = m_pDM_Accession->AddFrame( sQuery );
				if ( 0 > ids ) return -1;

				//=====================================================
				//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 				sQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
// 						_T("VALUES (ESL_SEQ.NEXTVAL, %s, 'I', sysdate);"), sSpeciesKey);
// 				ids = m_pDM_Accession->AddFrame( sQuery );
// 				if ( 0 > ids ) return -1;
				//=====================================================				
			}
			// +++++++++++++++++++++++++++++++++++++++++

			m_aBookKey.Add(sRecKey);
			m_aRegNo.Add(sRegNo);

			if ( m_nMode == 1 ) {
				//메인 그리드에 등록번호 반영				
				for ( INT i = 0 ; i < pDM_REG_0000->GetRowCount() ; i ++ ) {					
					if ( sRecKey == pDM_REG_0000->GetCellData( _T("BB_책KEY") , i ) ) {						
						pDM_REG_0000->SetCellData( _T("BB_등록번호") , sRegNo , i );
						pDM_REG_0000->SetCellData( _T("원부번호"), sAccNo, i );

						if( m_nOpenMode == 1) 
						{
							pDM_REG_0000->SetCellData( _T("BB_자료상태_코드"), _T("BOR112N"), i );
						}
						if( m_nOpenMode == 3) 
						{
							pDM_REG_0000->SetCellData( _T("BB_자료상태_코드"), _T("SER112N"), i );
						}

						break;
					}
				}				
			}

			//누락등록번호 테이블 삭제
			CString sDropRegCode, sDropRegNo;
			//=====================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 			sDropRegCode.Format( _T("%s") , sRegNo.Left(2) );
// 			sDropRegNo.Format( _T("%d"), _ttoi( sRegNo.Mid(3).GetBuffer(0) ) );
			//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
			sDropRegCode.Format( _T("%s") , sRegNo.Left(nRegCodeLength) );
			sDropRegNo.Format( _T("%d"), _ttoi( sRegNo.Mid(nRegCodeLength).GetBuffer(0) ) );
			//=====================================================
			
			sQuery.Format( _T("DELETE FROM CO_DROP_ACCESSION_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_CODE ='%s' AND REG_NO = %s;") , sDropRegCode, sDropRegNo );
			ids = m_pDM_Accession->AddFrame( sQuery );
			if ( 0 > ids ) return -1;
		}
	}
	else if ( m_bSetDropNo == FALSE ) 
	{
		for ( INT j = 1 ; j <= iRowControlCount ; j ++ ) 
		{

			pDM_1200_CONTROL->GetCellData( _T("BB_등록구분_코드") , j - 1, sControlCodeSpecies );
			pDM_1200_CONTROL->GetCellData( _T("BB_책KEY") , j - 1, sRecKey );
			pDM_1200_CONTROL->GetCellData( _T("BB_종키") ,  j - 1 , sSpeciesKey );
			
			for ( INT k = 1 ; k <= iRowCount ; k ++ ) {
				CString sControlCode;
				pDM_REG_0300->GetCellData( _T("BB_등록구분_코드") , k-1 , sControlCode );
				if ( sControlCode == sControlCodeSpecies ) {
					CString sControlNo;
					pDM_REG_0300->GetCellData( _T("마지막번호") , k-1 , sControlNo );
					CString sRegNo;
					//=====================================================
					//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 					sRegNo.Format( _T("%s%010s"), sControlCode, sControlNo );

					sRegNo = m_pInfo->MakeRegNo( sControlCode, sControlNo );
					//=====================================================
					

					if(m_nOpenMode == 3)	// 연속인 경우 제본된 책도 같이 변경
					{
						if ( m_strAccRecKey.IsEmpty() )	// 원부번호 없음_050408
							sQuery.Format( _T("UPDATE SE_BOOK_TBL SET REG_NO = '%s' , REG_DATE = '%s', WORKING_STATUS = 'SER112N', LAST_WORK='%s'  WHERE (REC_KEY =%s) OR (BINDING_BOOK_KEY = '%s');") , sRegNo, CTime::GetCurrentTime().Format(_T("%Y/%m/%d")), GetWorkLogMsg(_T("등록번호부여"),WORK_LOG), sRecKey, sRecKey );
						else
							sQuery.Format( _T("UPDATE SE_BOOK_TBL SET REG_NO = '%s' , REG_DATE = '%s', ACCESSION_REC_KEY = %s, WORKING_STATUS = 'SER112N', LAST_WORK='%s'  WHERE (REC_KEY =%s) OR (BINDING_BOOK_KEY = '%s');") , sRegNo, CTime::GetCurrentTime().Format(_T("%Y/%m/%d")), m_strAccRecKey , GetWorkLogMsg(_T("등록번호부여"),WORK_LOG), sRecKey, sRecKey );
					}
					else
					{
						if ( m_strAccRecKey.IsEmpty() )	// 원부번호 없음_050408
							sQuery.Format( _T("UPDATE BO_BOOK_TBL SET REG_NO = '%s' , REG_DATE = '%s', WORKING_STATUS = 'BOR112N', LAST_WORK='%s'  WHERE REC_KEY =%s;") , sRegNo, CTime::GetCurrentTime().Format(_T("%Y/%m/%d")), GetWorkLogMsg(_T("등록번호부여"),WORK_LOG), sRecKey );
						else
							sQuery.Format( _T("UPDATE BO_BOOK_TBL SET REG_NO = '%s' , REG_DATE = '%s', ACCESSION_REC_KEY = %s, WORKING_STATUS = 'BOR112N', LAST_WORK='%s'  WHERE REC_KEY =%s;") , sRegNo, CTime::GetCurrentTime().Format(_T("%Y/%m/%d")), m_strAccRecKey , GetWorkLogMsg(_T("등록번호부여"),WORK_LOG), sRecKey );
					}

					ids = m_pDM_Accession->AddFrame( sQuery );
					if ( 0 > ids ) return -1;

					// 040225_HSS IDX_BO_TBL, IDX_TMP_BO_TBL 추가 ++++++++++++++
					if(m_nOpenMode == 1)	// 단행일 경우만
					{
						sQuery.Format( _T("UPDATE IDX_BO_TBL SET WORKING_STATUS = 'BOR112N', LAST_WORK='%s' WHERE REC_KEY =%s;") , GetWorkLogMsg(_T("등록번호부여"),WORK_LOG), sSpeciesKey );
						ids = m_pDM_Accession->AddFrame( sQuery );
						if ( 0 > ids ) return -1;
						
						//=====================================================
						//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 						sQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
// 								_T("VALUES (ESL_SEQ.NEXTVAL, %s, 'I', sysdate);") ,sSpeciesKey);
// 						ids = m_pDM_Accession->AddFrame( sQuery );
// 						if ( 0 > ids ) return -1;
						//=====================================================
						
					}
					// +++++++++++++++++++++++++++++++++++++++++

					m_aBookKey.Add(sRecKey);
					m_aRegNo.Add(sRegNo);

					if ( m_nMode == 1 ) {
						//메인 그리드에 등록번호 반영
						
						for ( INT i = 0 ; i < pDM_REG_0000->GetRowCount() ; i ++ ) {
							if ( sRecKey == pDM_REG_0000->GetCellData( _T("BB_책KEY") , i ) ) {
								pDM_REG_0000->SetCellData( _T("BB_등록번호") , sRegNo , i );
								pDM_REG_0000->SetCellData( _T("원부번호"), sAccNo, i );

								if( m_nOpenMode == 1 ) 
								{
									pDM_REG_0000->SetCellData( _T("BB_자료상태_코드"), _T("BOR112N"), i );
								}
								if( m_nOpenMode == 3 ) 
								{
									pDM_REG_0000->SetCellData( _T("BB_자료상태_코드"), _T("SER112N"), i );
								}

								break;
							}
						}						
					}

					INT iControlNo = _ttoi ( sControlNo.GetBuffer(0) );				
					sControlNo.Format( _T("%d") , iControlNo+1 );
					pDM_REG_0300->SetCellData( _T("마지막번호") , sControlNo , k-1 );

					//누락등록번호 테이블 삭제
					CString sDropRegCode, sDropRegNo;
					//=====================================================
					//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 					sDropRegCode.Format( _T("%s") , sRegNo.Left(2) );
// 					sDropRegNo.Format( _T("%d"), _ttoi( sRegNo.Mid(3).GetBuffer(0) ) );
					
					sDropRegCode.Format( _T("%s") , sRegNo.Left(nRegCodeLength) );
					sDropRegNo.Format( _T("%d"), _ttoi( sRegNo.Mid(nRegCodeLength).GetBuffer(0) ) );
					//=====================================================
					
					sQuery.Format( _T("DELETE FROM CO_DROP_ACCESSION_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_CODE ='%s' AND ACCESSION_REC_NO = %s;") , sDropRegCode, sDropRegNo );
					ids = m_pDM_Accession->AddFrame( sQuery );
					if ( 0 > ids ) return -1;
				}
			}		
		}

		//마지막번호 테이블 UPDATE
		for ( INT l = 1 ; l <= pDM_REG_0300->GetRowCount() ; l++ ) {
			CString sLastNum = pDM_REG_0300->GetCellData( _T("마지막번호"), l-1 );
			CString sRegCode = pDM_REG_0300->GetCellData( _T("BB_등록구분_코드"), l-1 );		

			INT iLastNum = _ttoi( sLastNum.GetBuffer(0) );
			iLastNum = iLastNum - 1;
			sLastNum.Format( _T("%d"), iLastNum );
			//마지막등록번호		
			sQuery.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = '%s', LAST_WORK='%s' WHERE division_value = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE;") , sLastNum , GetWorkLogMsg(_T("등록번호부여"),WORK_LOG), sRegCode );
			//INT iCh = pDM_REG_0300->AddFrame( sQuery );
			INT iCh = m_pDM_Accession->AddFrame( sQuery );
			if ( 0 > iCh ) return -1;
		}



	}	

	ids = m_pDM_Accession->SendFrame();
	if ( ids < 0 ) {
		AfxMessageBox( _T("pDM_REG_0300->SendFrame() error") );
		return -1;
	}

	ids = m_pDM_Accession->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame 를 확인하세요"));
		return -1;
	}

	if( m_nOpenMode == 1)
	{
		AllControlDisplay(m_strParentCMAlias);			
	}
	else
	{
		AllControlDisplay(m_strParentCMAlias);
	}
	
	return 0;

EFS_END
return -1;

}

INT BO_CAT_REGNO_GIVE::MarcInsert()
{
EFS_BEGIN

	//마크에 등록번호저장
	INT ids;
	CESL_DataMgr DM_REG;
	DM_REG.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	ids = DM_REG.StartFrame();	
	if ( 0 > ids ) return -1;

	CESL_DataMgr DM_SPECIES;
	DM_SPECIES.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	CString sQuery;
	//sQuery.Format( _T("SELECT DISTINCT SPECIES_KEY FROM BO_BOOK_TBL WHERE REC_KEY IN (%s)"), m_strBookKeys );
	sQuery.Format( _T("SELECT REC_KEY, MARC FROM BO_SPECIES_TBL WHERE REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE REC_KEY IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE)"), m_strBookKeys );
	DM_SPECIES.RestructDataManager( sQuery );
	INT iSpeciesCount = DM_SPECIES.GetRowCount();

	/*
	CString sSpeciesKey;
	for ( INT i = 1 ; i <= iSpeciesCount ; i++ ) { 
		sSpeciesKey = sSpeciesKey + DM_SPECIES.GetCellData( i, 0 );
	}
	*/

	//pDM_REG_0000->Find
	for ( INT j = 0 ; j < iSpeciesCount ; j++ ) {
		CString sSpeciesRecKey = DM_SPECIES.GetCellData( j, 0 );
		CString sMarc = DM_SPECIES.GetCellData( j, 1 );

		//종키리스트 add
		m_aSpeciesKey.Add( sSpeciesRecKey );

		//CString sQuery_reg = _T("SELECT REG_NO FROM BO_BOOK_TBL WHERE REG_NO IS NOT NULL AND SPECIES_KEY = '") + sSpeciesRecKey + _T("' ORDER BY REG_NO");
		CString sQuery_reg = _T("SELECT REG_NO FROM BO_BOOK_TBL WHERE REG_NO IS NOT NULL AND SPECIES_KEY = '") + sSpeciesRecKey + _T("'");
		DM_REG.RestructDataManager(sQuery_reg);
		INT iRowCount_reg = DM_REG.GetRowCount();

		CMarc m_marc;
		DM_REG.SetMarcMgr(m_pInfo->pMarcMgr);
		m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

		//decoding
		CString strTagCode, strErrMsg, strHelpMsg;	
		if(m_pInfo->pMarcMgr->Decoding(sMarc, &m_marc))
		{
			POSITION pos;
			pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
			while (pos)
			{
				m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);	
				UpdateData(FALSE);
			}	
		}

		for ( INT k = 0 ; k < iRowCount_reg ; k ++ ) {
			CString sRegNo = DM_REG.GetCellData( k , 0 );
			//최소등록번호 add
			if ( k == 0 ) {
				m_aMinRegNo.Add( sRegNo );
			}

			//마크 구조체에 등록번호 추가
			m_pInfo->pMarcMgr->SetItem(&m_marc, _T("049$l"), sRegNo);			
		}

		//encoding	//마크에 저장		
		m_pInfo->pMarcMgr->Encoding(&m_marc, sMarc);
		/*
		sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC = '%s' WHERE REC_KEY ='%s';") , sMarc, sSpeciesRecKey );	
		ids = DM_REG.AddFrame( sQuery );
		*/
		DM_REG.InitDBFieldData();				
		DM_REG.AddDBFieldData(_T("MARC"), _T("STRING"), sMarc, 1);
		ids = DM_REG.MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("STRING"), sSpeciesRecKey );
		if ( 0 > ids ) return -1;
		//ids = m_pDM_Accession->AddFrame( sQuery );
	}

	ids = DM_REG.SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다(DM_REG)."));
		return -1;
	}
	ids = DM_REG.EndFrame();
	if ( 0 > ids ) return -1;

	return 0;

EFS_END
return -1;

}

VOID BO_CAT_REGNO_GIVE::OnbtnCancel() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	//DestroyWindow()Cancel;	
	EndDialog(IDCANCEL);

EFS_END
}

VOID BO_CAT_REGNO_GIVE::OnbACCESSIONNOGIVE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	INT ids = -1;

	ids = AccessionInput();  //원부입력 	
	if ( ids < 0 ) {
		return;
	}

	ids = BookUpdate();    //책정보 업데이트
	if ( ids < 0 ) {
		return;
	}

	AfxMessageBox( _T("등록번호를 부여하였습니다.") );
	EndDialog(IDOK);

EFS_END
}

VOID BO_CAT_REGNO_GIVE::OnbAccessionInput() 
{
EFS_BEGIN

	//원부정보 입력
	CBO_CAT_REGNO_0305 dlg(this);
	//2019.07.25 JOB.2019.0065 ADD BY KYJ 입력된 원부 정보 지워지지 않도록 수정
	CString strAccYear = _T(""), strAccNo = _T("");
	GetControlData(_T("CM_BO_CAT_REG_0300"), _T("수입년도"), strAccYear);
	GetControlData(_T("CM_BO_CAT_REG_0300"), _T("원부번호"), strAccNo);
	dlg.strAccNo = strAccNo;
	dlg.strAccYear = strAccYear;
	dlg.DoModal();

EFS_END
}

VOID BO_CAT_REGNO_GIVE::OnbMissNumber() 
{
EFS_BEGIN

	//누락등록번호 적용
	DropNoDlgOpen();

EFS_END
}

VOID BO_CAT_REGNO_GIVE::OnbLastNumber() 
{
EFS_BEGIN

	//마지막번호 관리
	BO_CAT_LAST_NUMBER_0000 dlg(0,this);
	dlg.DoModal();

	//2007.08.10 ADD BY PDJ
	// 누락등록번호 사용인 경우 갱신하지 않는다.
	if ( m_bSetDropNo == FALSE )
	{
		IntDialogNum();
	}

EFS_END
}

VOID BO_CAT_REGNO_GIVE::DropNoDlgOpen()
{
EFS_BEGIN

	CBO_CAT_REGNO_0310 dlg(this, m_strWhere);
	if (m_nOpenMode == 3)
		dlg.m_bSE = TRUE;
	
	CESL_DataMgr * pDM_REG_0300;
	pDM_REG_0300 = FindDM( _T("DM_BO_CAT_REG_0300") );
	if ( NULL == pDM_REG_0300 ) {
		AfxMessageBox( _T("pDM_REG_0300 error") );
		return;
	}
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	if( IDOK == dlg.DoModal() ) {
		m_bSetDropNo = TRUE;

		//그리드에 넣기
		CESL_DataMgr *pDM_0310_DROP;
		pDM_0310_DROP = FindDM(_T("DM_BO_CAT_REG_311_DROP"));
		
		INT iRowCount = pDM_0310_DROP->GetRowCount();
		if ( iRowCount < 1 ) {
			return;
		}

		CESL_ControlMgr *pCM_SUB;
		pCM_SUB = FindCM(_T("CM_BO_CAT_REG_0300"));
		pCM_SUB->FindControl(_T("REG_0300_GRID"));
		
		CESL_Grid * pGrid_sub = NULL;
		pGrid_sub = (CESL_Grid*)pCM_SUB->FindControl(_T("REG_0300_GRID"));

		pGrid_sub->SetRows(iRowCount+1);

		for ( INT i = 1 ; i <= iRowCount ; i ++ ) {
			//등록구분
			CString sRegCode;
			pDM_0310_DROP->GetCellData( i - 1 , 0 , sRegCode );
			//=====================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 			pGrid_sub->SetTextMatrix( i, 1, sRegCode.Left(2) );
			
			pGrid_sub->SetTextMatrix( i, 1, sRegCode.Left(nRegCodeLength) );
			//=====================================================
			
			pGrid_sub->SetTextMatrix( i, 2, _T("1") );
			pGrid_sub->SetTextMatrix( i, 3, sRegCode );

			if ( i == 1 ) {
				pDM_REG_0300->SetCellData( _T("처음등록번호"), sRegCode, 0 );
			}
			if ( i == iRowCount ) {
				pDM_REG_0300->SetCellData( _T("마지막등록번호"), sRegCode, 0 );
			}
		}		
	}

	return;

EFS_END
return;
}

BOOL BO_CAT_REGNO_GIVE::AccNoUpdateOrInsertLastNoTbl()
{
EFS_BEGIN

	//마지막 원부번호 UPDATE
		INT ids = -1;
		CString sAccNo;
		GetControlData( _T("CM_BO_CAT_REG_0300") , _T("원부번호") , sAccNo );
		INT iAccNoGlo = _ttoi( m_strAccNo.GetBuffer(0) );
		INT iAccNo = _ttoi( sAccNo.GetBuffer(0) );
		if ( iAccNoGlo > iAccNo ) 
		{
			sAccNo.Format( _T("%d") , iAccNoGlo-1 );
		}

		//년도
		CString sYear,sQuery,sCount,tREC_KEY;
		GetControlData( _T("CM_BO_CAT_REG_0300"), _T("수입년도"), sYear );

		//조건에 해당하는 마지막 마지막 원부번호가 없을 경우
		sQuery.Format(_T("Select COUNT(*) from CO_LAST_NUMBER_TBL where KIND_CODE = 'ACC_NO' and MAKE_YEAR = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE "),sYear);	

		m_pDM_Accession->GetOneValueQuery( sQuery , sCount );
		if ( sCount == _T("0") ) 
		{
			ids = m_pDM_Accession->MakeRecKey( tREC_KEY );
			if ( 0 > ids ) return FALSE;
			sQuery.Format( _T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , KIND_CODE , MAKE_YEAR , LAST_NUMBER, MANAGE_CODE ) VALUES ( %s , 'ACC_NO' , '%s' , '%s', UDF_MANAGE_CODE );") , tREC_KEY , sYear , _T("1"));
//DEL 			sQuery.Format( _T("INSERT INTO CO_LAST_NUMBER_TBL ( REC_KEY , KIND_CODE , MAKE_YEAR , LAST_NUMBER ) VALUES ( %s , 'ACC_NO' , '%s' , '%s'  );") , tREC_KEY , sYear , _T("1"));
			ids = m_pDM_Accession->AddFrame( sQuery );
			if ( 0 > ids ) return FALSE;
		}
		else
		{
			sQuery.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = '%s', LAST_WORK='%s' WHERE MAKE_YEAR = '%s' AND KIND_CODE = 'ACC_NO' AND MANAGE_CODE = UDF_MANAGE_CODE;") , sAccNo, GetWorkLogMsg(_T("등록번호부여"),WORK_LOG) ,sYear );
			ids = m_pDM_Accession->AddFrame( sQuery );
			if ( 0 > ids ) return FALSE;
		}



		return TRUE;

EFS_END
return FALSE;
}

BOOL BO_CAT_REGNO_GIVE::MakeDMInfo( CString sACCESSION_REC_NO , CString sACCESSION_REC_MAKE_YEAR )
{
EFS_BEGIN

	INT iSwFind = 0;	// 부여된 원부번호가 있으면 1 이다.
	INT ids = -1;

	//원부번호찾기
	CString sQueryCount = _T("select rec_key from co_accession_rec_tbl WHERE accession_rec_no = '") + sACCESSION_REC_NO + _T("' AND ACCESSION_REC_MAKE_YEAR = '") + sACCESSION_REC_MAKE_YEAR + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE");
	m_pDM_Accession->GetOneValueQuery( sQueryCount , m_strAccRecKey );

	iSwFind = ( m_strAccRecKey != _T("") )  ? 1 : 0;


	//DM작업
	m_pDM_Accession->InitDBFieldData();

	CString sCLASS_NAME, sPROVIDER;
	CString sCLASS_WORKER, sCLASS_DATE, sCATALOG_WORKER, sCATALOG_DATE;
	CString sLABELING_WORKER, sLABELING_DATE, sPROCESSING_WORKER, sPROCESSING_COMPLETE_DATE;
	CString sYBK_TRANSFER_DATE, sSHELF_WORKER, sSHELF_DATE;

	CESL_DataMgr *pDM_0305;
	pDM_0305 = FindDM(_T("DM_BO_CAT_REG_0305"));
	if ( NULL == pDM_0305 ) 
	{
		AfxMessageBox ( _T("Find DM_BO_CAT_REG_0305 error") );
		return -1;
	}

	//2019.07.25 JOB.2019.0065 ADD BY KYJ 입력된 원부 정보 지워지지 않도록 수정
	//'원부정보 입력'을 통해 정보가 입력된 경우룰 RowCount가 1로 치고
	//그 외에 경우에는 DB에서 정보를 조회
	if( pDM_0305->GetRowCount() == 0 && !sACCESSION_REC_NO.IsEmpty() && !sACCESSION_REC_MAKE_YEAR.IsEmpty() ) {
		CString strWhere = _T("");
		strWhere.Format(_T(" ACCESSION_REC_NO = '%s' AND ACCESSION_REC_MAKE_YEAR = '%s' AND ROWNUM = '1' "),sACCESSION_REC_NO,sACCESSION_REC_MAKE_YEAR);
		pDM_0305->RefreshDataManager(strWhere);
	}	

	ids = pDM_0305->GetCellData( _T("분류작업자"), 0, sCLASS_WORKER );
	ids = pDM_0305->GetCellData( _T("분류일자"), 0, sCLASS_DATE );
	ids = pDM_0305->GetCellData( _T("목록작업자"), 0, sCATALOG_WORKER );
	ids = pDM_0305->GetCellData( _T("목록일자"), 0, sCATALOG_DATE );
	ids = pDM_0305->GetCellData( _T("장비작업자"), 0, sLABELING_WORKER );
	ids = pDM_0305->GetCellData( _T("장비일자"), 0, sLABELING_DATE );
	ids = pDM_0305->GetCellData( _T("정리완료자"), 0, sPROCESSING_WORKER );
	ids = pDM_0305->GetCellData( _T("정리완료일자"), 0, sPROCESSING_COMPLETE_DATE );
	ids = pDM_0305->GetCellData( _T("인계일자"), 0, sYBK_TRANSFER_DATE );
	ids = pDM_0305->GetCellData( _T("배가작업자"), 0, sSHELF_WORKER );
	ids = pDM_0305->GetCellData( _T("배가일자"), 0, sSHELF_DATE );
	ids = pDM_0305->GetCellData( _T("직명"), 0, sCLASS_NAME );


	CString sDIVISION_NAME, sREGISTRANT, sREG_DATE, sACQUISIT_CODE;

	GetControlData(_T("CM_BO_CAT_REG_0300"), _T("과명"), sDIVISION_NAME);
	GetControlData(_T("CM_BO_CAT_REG_0300"), _T("등록자"), sREGISTRANT);
	GetControlData(_T("CM_BO_CAT_REG_0300"), _T("등록년월일"), sREG_DATE);
	GetControlData(_T("CM_BO_CAT_REG_0300"), _T("수입구분"), sACQUISIT_CODE);



	m_pDM_Accession->AddDBFieldData(_T("CLASS_WORKER"), _T("STRING"), sCLASS_WORKER);
	m_pDM_Accession->AddDBFieldData(_T("CLASS_DATE"), _T("STRING"), sCLASS_DATE);
	m_pDM_Accession->AddDBFieldData(_T("CATALOG_WORKER"), _T("STRING"), sCATALOG_WORKER);
	m_pDM_Accession->AddDBFieldData(_T("CATALOG_DATE"), _T("STRING"), sCATALOG_DATE);
	m_pDM_Accession->AddDBFieldData(_T("LABELING_WORKER"), _T("STRING"), sLABELING_WORKER);
	m_pDM_Accession->AddDBFieldData(_T("LABELING_DATE"), _T("STRING"), sLABELING_DATE);
	m_pDM_Accession->AddDBFieldData(_T("PROCESSING_WORKER"), _T("STRING"), sPROCESSING_WORKER);
	m_pDM_Accession->AddDBFieldData(_T("PROCESSING_COMPLETE_DATE"), _T("STRING"), sPROCESSING_COMPLETE_DATE);
	m_pDM_Accession->AddDBFieldData(_T("LOC_TRANSFER_DATE"), _T("STRING"), sYBK_TRANSFER_DATE);
	m_pDM_Accession->AddDBFieldData(_T("SHELF_WORKER"), _T("STRING"), sSHELF_WORKER);
	m_pDM_Accession->AddDBFieldData(_T("SHELF_DATE"), _T("STRING"), sSHELF_DATE);
	m_pDM_Accession->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("등록번호부여"),WORK_LOG));

	m_pDM_Accession->AddDBFieldData(_T("PROVIDER"), _T("STRING"), sPROVIDER);
	m_pDM_Accession->AddDBFieldData(_T("DIVISION_NAME"), _T("STRING"), sDIVISION_NAME);
	m_pDM_Accession->AddDBFieldData(_T("CLASS_NAME"), _T("STRING"), sCLASS_NAME);
	m_pDM_Accession->AddDBFieldData(_T("REGISTRANT"), _T("STRING"), sREGISTRANT);
	m_pDM_Accession->AddDBFieldData(_T("ACCESSION_REC_MAKER"), _T("STRING"), sREGISTRANT);
	m_pDM_Accession->AddDBFieldData(_T("REG_DATE"), _T("STRING"), sREG_DATE);
	m_pDM_Accession->AddDBFieldData(_T("ACCESSION_REC_NO"), _T("STRING"), sACCESSION_REC_NO);
	m_pDM_Accession->AddDBFieldData(_T("ACCESSION_REC_MAKE_YEAR"), _T("STRING"), sACCESSION_REC_MAKE_YEAR);
	m_pDM_Accession->AddDBFieldData(_T("ACQUISIT_CODE"), _T("STRING"), sACQUISIT_CODE);
	m_pDM_Accession->AddDBFieldData(_T("PUBLISH_FORM_CODE"), _T("STRING"), _T("MO") );

	m_pDM_Accession->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );



	if ( iSwFind == 0 ) 
	{
		ids = m_pDM_Accession->MakeRecKey( m_strAccRecKey );
		m_pDM_Accession->AddDBFieldData(_T("REC_KEY"), _T("STRING"), m_strAccRecKey);
		m_pDM_Accession->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(_T("등록번호부여"),WORK_LOG));

		ids = m_pDM_Accession->MakeInsertFrame(_T("CO_ACCESSION_REC_TBL"));
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("MakeInsertFrame 를 확인하세요"));
			return -1;
		}
	}
	else 
	{
		if (0>m_pDM_Accession->MakeUpdateFrame( _T("CO_ACCESSION_REC_TBL"), _T("REC_KEY"), _T("STRING"), m_strAccRecKey))
		{
			AfxMessageBox(_T("MakeUpdateFrame 를 확인하세요"));
			return -1;
		}
		
	}

	return 0;

EFS_END
return -1;
}


HBRUSH BO_CAT_REGNO_GIVE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

//====================================================
//2008.07.14 ADD BY PJW : 원부번호 KillFocuse할때마다 원부정보입력창의 정보를 셋팅한다.
void BO_CAT_REGNO_GIVE::OnKillfocuseACCESSIONRECNO() 
{
 	CString strWhere;
	CString strQuery;
	CString strValue;
	CString strAccessionRecNo,sACCESSION_REC_MAKE_YEAR;
	CESL_DataMgr *pDM;
	INT nCnt;
	
	pDM = FindDM(_T("DM_BO_CAT_REG_0305"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_REG error") );
		return;
	}

	GetControlData(_T("CM_BO_CAT_REG_0300"), _T("수입년도"), sACCESSION_REC_MAKE_YEAR);
	
	GetDlgItem(IDC_eACCESSION_REC_NO)->GetWindowText(strAccessionRecNo);
	if(strAccessionRecNo.IsEmpty())
	{
		return;
	}
	strQuery.Format(_T("SELECT COUNT(1) FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO = %s AND MANAGE_CODE = UDF_MANAGE_CODE AND ACCESSION_REC_MAKE_YEAR = '%s'"),strAccessionRecNo,sACCESSION_REC_MAKE_YEAR);
	pDM->GetOneValueQuery(strQuery, strValue);
	nCnt = _ttoi(strValue);
	
	if( 1 > nCnt )
	{
		return;
	}
	else if( 1 < nCnt )
	{
		AfxMessageBox(_T("해당 원부번호가 두개이상 존재합니다."));
		GetDlgItem(IDC_eACCESSION_REC_NO)->SetWindowText(_T(""));
		return;
	}	

	//2019.07.25 JOB.2019.0065 ADD BY KYJ 입력된 원부 정보 지워지지 않도록 수정	
	//strWhere.Format(_T("ACCESSION_REC_NO = %s"), strAccessionRecNo);
	strWhere.Format(_T("ACCESSION_REC_NO = '%s' AND ACCESSION_REC_MAKE_YEAR = '%s' "), strAccessionRecNo,sACCESSION_REC_MAKE_YEAR);
	pDM->RefreshDataManager(strWhere);
}
//====================================================
