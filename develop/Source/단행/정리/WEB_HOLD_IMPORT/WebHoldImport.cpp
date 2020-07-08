// WebHoldImport.cpp : implementation file
//

#include "stdafx.h"
#include "WebHoldImport.h"

#include "BO_CAT_AUTO_REGNO_GIVE.h"

// 공통 - 문자변환용 
#include "..\..\..\공통\K2UP_Common/MakeSearchData.h"

#include "..\..\..\ESL\MARC\MarcToolKit\CreateAutorCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebHoldImport dialog


CWebHoldImport::CWebHoldImport(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CWebHoldImport)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM = NULL;
	m_pMDataFormCombo = NULL;
	m_pMDataStatusCombo = NULL;

	m_pDM_Species = NULL;
	m_pDM_Index = NULL;
	m_pDM_Book = NULL;

	m_pCatApi = NULL;

}

CWebHoldImport::~CWebHoldImport()
{
	
}

VOID CWebHoldImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebHoldImport)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWebHoldImport, CDialog)
	//{{AFX_MSG_MAP(CWebHoldImport)
	ON_BN_CLICKED(IDC_bClose, OnbClose)
	ON_BN_CLICKED(IDC_bImport, OnbImport)
	ON_BN_CLICKED(IDC_bSearch, OnbSearch)
	ON_BN_CLICKED(IDC_bDelete, OnbDelete)
	ON_CBN_SELCHANGE(IDC_cDataStatus, OnSelchangecDataStatus)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebHoldImport message handlers

VOID CWebHoldImport::OnbClose() 
{
	//닫기 버튼
	CDialog::OnCancel();
}


BOOL CWebHoldImport::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_pCatApi = new CCatApi_BO(this);

	if (InitESL_Mgr(_T("SM_단행_정리_웹반입")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_단행_정리_웹반입"));
	if ( m_pCM == NULL ) 
	{
		AfxMessageBox(_T("FindCM Error"));
		return FALSE;
	}

	m_pDM = FindDM(_T("DM_단행_정리_웹반입"));
	if ( m_pDM == NULL ) 
	{
		AfxMessageBox(_T("FindDM Error"));
		return FALSE;
	}
	
	m_pDM_Species = FindDM ( _T("DM_단행_정리DM_종정보") );
	if ( m_pDM_Species == NULL ) 
	{
		AfxMessageBox(_T("FindDM Error"));
		return FALSE;
	}

	m_pDM_Index = FindDM (_T("DM_단행_정리DM_색인정보") );
	if ( m_pDM_Index == NULL ) 
	{
		AfxMessageBox(_T("FindDM Error"));
		return FALSE;
	}

	m_pDM_Book = FindDM (_T("DM_단행_정리DM_책정보") );
	if ( m_pDM_Index == NULL ) 
	{
		AfxMessageBox(_T("FindDM Error"));
		return FALSE;
	}

	m_pGrid = static_cast<CESL_Grid*>(FindControl(_T("CM_단행_정리_웹반입"), _T("GRID")));
	if (m_pGrid == NULL) return FALSE;


	//자료유형, 자료상태 콤보박스 초기화
	if ( !InitComboControl() ) return FALSE;

	//초기 설정된 ComboBox 값으로 검색 쿼리를 수행
	CString strWhere = MakeWhereQuery();
	m_pDM->RefreshDataManager( strWhere );

	m_pCM->AllControlDisplay();

	EnableThemeDialogTexture(GetSafeHwnd()); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CWebHoldImport::InitComboControl()
{
	//Combo_Box 값 초기화
	//자료유형 콤보박스
	m_pMDataFormCombo = new CJComboBox;
	m_pMDataFormCombo->SubclassDlgItem( IDC_cDataForm, this );
	m_pMDataFormCombo->FormatList( 1, DT_LEFT, false , EDGE );
	m_pMDataFormCombo->RequireFlag = FALSE;
	
	//자료유형은 별치기호를 이용
	m_pMDataFormCombo->AddItem( _T("연수결과"), 0, 0 );		//AC
	m_pMDataFormCombo->AddItem( _T("연구용역"), 0, 1 );		//RW
	m_pMDataFormCombo->AddItem( _T("부서보유자료"), 0, 2 );	//PH
	
	m_pMDataFormCombo->SetCurSel( 0 );


	//자료상태 콤보박스
	m_pMDataStatusCombo = new CJComboBox;
	m_pMDataStatusCombo->SubclassDlgItem( IDC_cDataStatus, this );
	m_pMDataStatusCombo->FormatList( 1, DT_LEFT, false , EDGE );
	m_pMDataStatusCombo->RequireFlag = FALSE;

	m_pMDataStatusCombo->AddItem( _T("신청"), 0, 0 ); //A01
	m_pMDataStatusCombo->AddItem( _T("완료"), 0, 1 ); //B01
	m_pMDataStatusCombo->AddItem( _T("삭제"), 0, 2 ); //C01
	
	//기본값을 신청으로 셋팅
	m_pMDataStatusCombo->SetCurSel( 1 );


	return TRUE;
}

VOID CWebHoldImport::OnbImport() 
{
	//웹 반입
	ExecuteWebImport();
}

VOID CWebHoldImport::OnbDelete() 
{
	//삭제 대상 자료에 대해 삭제 처리
	m_pGrid->SelectMakeList();
	INT nGridIdx = m_pGrid->SelectGetHeadPosition();
	if ( nGridIdx < 0 )
	{
		AfxMessageBox(_T("삭제할 데이터를 선정해 주세요"));
		return;
	}

	if (MessageBox(_T("선정한 자료를 삭제하시겠습니까?"), _T("삭제 확인"), MB_YESNO) != IDYES)
	{
		return;
	}

	INT ids = -1;
	while ( nGridIdx != -1 ) 
	{
		/*
		CString strWorkingStatus;
		m_pDM->GetCellData(_T("자료상태"),  nGridIdx , strWorkingStatus ) ;
		if ( strWorkingStatus != _T("C01") ) 
		{
			AfxMessageBox(_T("자료상태가 '삭제'인 자료만 삭제하실 수 있습니다."));
			return ;
		}
		*/

		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
		ids = dm.StartFrame();
		if ( ids < 0 ) return;
		
		CString strReckey;
		m_pDM->GetCellData(_T("REC_KEY"), nGridIdx , strReckey);

		CString strQuery;
		strQuery = _T("DELETE WEB_IMPORT_TMP_TBL WHERE REC_KEY = '") + strReckey + _T("';");

		dm.AddFrame(strQuery);
		ids = dm.SendFrame();
		if ( ids < 0 ) return;

		ids = dm.EndFrame();
		if ( ids < 0 ) return;

		nGridIdx = m_pGrid->SelectGetNext();
	}

	AfxMessageBox( _T("정상적으로 삭제 되었습니다."));
	OnbSearch();

	return;
		
}

CString CWebHoldImport::MakeWhereQuery()
{
	//WHERE 조건절을 생성..
	CString strWhere = _T("");
	//자료유형, 자료상태에 따라 Where 조건절을 생성한다~!!
	// 0 - 부서보유자료
	// 1 - 연구용역
	// 2 - 연수결과
	INT nDataFormidx = m_pMDataFormCombo->GetCurSel();

	switch ( nDataFormidx )
	{
	case 0:
			strWhere = _T(" SEPARATE_SHELF_CODE = 'PH' ");
			break;
	case 1:
			strWhere = _T(" SEPARATE_SHELF_CODE = 'RW' ");
			break;
	case 2:
			strWhere = _T(" SEPARATE_SHELF_CODE = 'AC' ");
			break;
	}

	// 0 - 삭제
	// 1 - 신청
	// 2 - 완료
	INT nDataStatusidx = m_pMDataStatusCombo->GetCurSel();
	switch ( nDataStatusidx )
	{
	case 0:
			strWhere += _T(" AND WORKING_STATUS = 'C01' ");
			break;
	case 1:
			strWhere += _T(" AND WORKING_STATUS = 'A01' ");
			break;
	case 2:
			strWhere += _T(" AND WORKING_STATUS = 'B01' ");
			break;
	}

	return strWhere;
}

VOID CWebHoldImport::OnbSearch() 
{
	CString strWhere = MakeWhereQuery();
	m_pDM->RefreshDataManager( strWhere );
	INT i = m_pDM->GetRowCount();

	m_pCM->AllControlDisplay();
	
}

BOOL CWebHoldImport::ExecuteWebImport()
{
	m_pGrid->SelectMakeList();
	INT nGridIdx = m_pGrid->SelectGetHeadPosition();
	if ( nGridIdx < 0 ) 
	{
		AfxMessageBox(_T("반입할 자료를 선택해 주세요"));
		return FALSE;
	}

	UpdateData();
	m_pDM_Species->InsertRow(-1);
	m_pDM_Index->InsertRow(-1);
	m_pDM_Book->InsertRow(-1);

	INT ids = -1;
	while ( nGridIdx != -1 ) 
	{

		CMarc *pMarc = NULL;
		pMarc = new CMarc;

		ids = m_pDM->StartFrame();
		if ( 0 > ids ) 
		{
			m_pDM->EndFrame();
			return FALSE;
		}

		//Marc 생성
		if ( !MakeImportMarc( pMarc , nGridIdx ) ) return FALSE;

		if ( !ExecuteMarcImport( nGridIdx ) ) return FALSE;		

		
		//추가 업데이트 쿼리 작성
		//WEB_IMPORT_TMP_TBL 에 상태 변경
		//DL팀 추가 쿼리 작성


		//정상적으로 반입이 되었으면 WEB_IMPORT_TMP_TBL TBL의 상태값을 변경한다.
		if ( !ExtraQuery( nGridIdx ) ) return FALSE;

		ids = m_pDM->SendFrame();
		if ( 0 > ids ) 
		{
			m_pDM->EndFrame();
			return FALSE;
		}

		ids = m_pDM->EndFrame();
		if ( 0 > ids ) 
		{
			m_pDM->EndFrame();
			return FALSE;
		}


		nGridIdx = m_pGrid->SelectGetNext();
	}

	AfxMessageBox(_T("정상적으로 반입되었습니다"));
	OnbSearch();

	return TRUE;
}

BOOL CWebHoldImport::MakeImportMarc(CMarc *pMarc, INT nGridIdx )
{
	//반입용 마크를 생성한다
	CMap<CString, LPCTSTR, CString, LPCTSTR> mapMarcAlias_DMAlias;
	InitMapMarcTag( mapMarcAlias_DMAlias );

	CString strStreamMarc = MakeStreamMarc( pMarc, mapMarcAlias_DMAlias, nGridIdx );
	if ( strStreamMarc.IsEmpty() ) return FALSE;
	

	m_pInfo->pMarcMgr->Decoding( strStreamMarc, pMarc );
	
	INT ids = -1;
	ids = MakeCallNo( pMarc , nGridIdx);
	if ( ids < 0 ) return FALSE;

	m_pInfo->pMarcMgr->Encoding( pMarc, strStreamMarc );
	
	//코드값들을 적용힌다
	CodeApply ();

	//생성된 StreamMarc를 종 Tbl에 넣는다
	m_pDM_Species->SetCellData(_T("BS_MARC"), strStreamMarc, 0 ) ;

	return TRUE;
}

VOID CWebHoldImport::InitMapMarcTag( CMap<CString, LPCTSTR, 
									 CString, LPCTSTR>& mapMarcAlias_DMAlias )
{
	//pmapMarcAlias_DMAlias->RemoveAll( );
	mapMarcAlias_DMAlias.SetAt(_T("008@0-39"),_T("008발행년"));
	mapMarcAlias_DMAlias.SetAt(_T("245$d"),_T("저자"));
	mapMarcAlias_DMAlias.SetAt(_T("245$a"),_T("서명"));
	mapMarcAlias_DMAlias.SetAt(_T("300$a"),_T("페이지"));
	mapMarcAlias_DMAlias.SetAt(_T("260$b"),_T("발행자"));
	mapMarcAlias_DMAlias.SetAt(_T("260$c"),_T("발행년"));
	mapMarcAlias_DMAlias.SetAt(_T("950$b"),_T("가격"));
	mapMarcAlias_DMAlias.SetAt(_T("020$a"),_T("ISBN"));
	mapMarcAlias_DMAlias.SetAt(_T("505$a"),_T("비고"));
}

CString CWebHoldImport::MakeStreamMarc( CMarc *pMarc , 
									   CMap<CString, LPCTSTR, 
									   CString, LPCTSTR>& mapMarcAlias_DMAlias, 
									   INT nGridIdx )
{
	INT ids = -1;
	CString strMarcAlias;
	CString strDMAlias;
	CString strItemData;
	CString strStreamMarc = _T("");

	POSITION pos = mapMarcAlias_DMAlias.GetStartPosition();
	while (pos)
	{		
		mapMarcAlias_DMAlias.GetNextAssoc(pos, strMarcAlias, strDMAlias);
		ids = m_pDM->GetCellData(strDMAlias, nGridIdx, strItemData);
		if ( strDMAlias == _T("008발행년") )
		{
			CString result;
			CTime t = CTime::GetCurrentTime();
			CString strTmpYear;
			strTmpYear.Format(_T("%d"), t.GetYear());
			strTmpYear = strTmpYear.Mid(2);
			strItemData.Format(_T("%s%02d%02d"), strTmpYear, t.GetMonth(), t.GetDay());
			ids = 0;
		}
		
		if ( strDMAlias == _T("가격") ) 
		{
			if ( strItemData.Left(1) != _T("\\") )
			{
				CString strTmp;
				strTmp.Format (_T("\\%s"), strItemData );
				strItemData = strTmp;
			}
		}
		
		if ( strMarcAlias == _T("008@0-39") ) 
		{
			for ( INT i = strItemData.GetLength() ; i < 40 ; i++ )
			{
				if ( i < 38 ) 
					strItemData = strItemData + _T(" ");
				else 
				{
					//연구용역 자료일경우 국가코드에 AK 넣어준다
					INT nDataFormidx = m_pMDataFormCombo->GetCurSel();
					if ( nDataFormidx == 1 )
					{
						strItemData = strItemData + _T("AK");
						break;
					}
					else
					{
						strItemData = strItemData + _T("  ");
						break;
					}
				}
			}		
		}

		if (ids >= 0)
			ids = m_pInfo->pMarcMgr->SetItem(pMarc, strMarcAlias, strItemData);
	}

	//등록번호 부여하여 049 Tag 생성
	MakeRegNO049Tag( pMarc, nGridIdx );
	m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc );

	return strStreamMarc;
}

BOOL CWebHoldImport::MakeRegNO049Tag( CMarc *pMarc, INT nGridIdx )
{
	
	INT ids = -1;
	INT nTotalSelectGridCnt = m_pGrid->SelectGetCount();
	CString strRegCode;
	m_pCM->GetControlMgrData(_T("BB_등록구분"), strRegCode );

	//마지막번호를 가져옴.
	CString strGetLastNumQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE = 'REG_NO' AND DIVISION_VALUE = '") + strRegCode + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE");
	CString strLasuNum;
	ids = m_pDM->GetOneValueQuery( strGetLastNumQuery, strLasuNum );
	if ( ids < 0 ) return FALSE;

	//마지막 번호 TBL에 Lock를 건다
//	CString strTblLockQuery = _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = LAST_NUMBER WHERE KIND_CODE = 'REG_NO' AND DIVISION_VALUE = '") + strRegCode + _T("';");
//	ids = m_pDM->AddFrame( strTblLockQuery );
//	if ( ids < 0 ) return FALSE;

	INT iLastNum = _ttoi( strLasuNum.GetBuffer(0) );
	iLastNum ++;		

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	
	CString strTmp;	
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	strTmp.Format(_T("%010d"), iLastNum );
// 
// 	//등록번호 만들기
// 	CString strRegNo = strRegCode + strTmp;
	
	CString strRegNo;
	strTmp.Format(_T("%d"), iLastNum );
	//등록번호 만들기
	strRegNo = m_pInfo->MakeRegNo(strRegCode, strTmp);
	strTmp = strRegNo.Mid(nRegCodeLength);
	//=====================================================
	

	m_pDM->SetCellData(_T("등록번호"), strRegNo, nGridIdx );
	
	//등록번호 넣기
	m_pInfo->pMarcMgr->SetItem( pMarc, _T("049$l"), strRegNo );
	m_pDM_Book->SetCellData(_T("BB_등록번호"), strRegNo , 0 );


	//별치기호 있음 넣기
	CString strShelfCode;
	m_pDM->GetCellData( _T("별치기호"), nGridIdx, strShelfCode );
	m_pDM_Book->SetCellData( _T("BB_별치기호_코드"), strShelfCode, 0 );
	m_pInfo->pMarcMgr->SetItem( pMarc, _T("049$f"), strShelfCode );

	m_pDM_Book->SetCellData( _T("BB_등록구분_코드"), strRegCode, 0 );

	CString strUpdateLastNum;
	strUpdateLastNum.Format(_T("%d"), iLastNum );
	CString strUpdateQuery = _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = ") + strUpdateLastNum  +_T(" WHERE KIND_CODE = 'REG_NO' AND DIVISION_VALUE = '") + strRegCode + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE;");
	ids = m_pDM->AddFrame( strUpdateQuery );
	if ( ids < 0 ) return FALSE;

	return TRUE;
}


VOID CWebHoldImport::CodeApply()
{
	EFS_BEGIN

	BeginWaitCursor();
	
	INT nRowCount = m_pDM_Index->GetRowCount();
	for (INT row = 0; row < nRowCount; row++)
		CodeApply(row);
	
	EndWaitCursor();


	EFS_END
}

VOID CWebHoldImport::CodeApply(INT nIdxDM)
{
	EFS_BEGIN

	// HSS_040524 헤더세팅
	CString strHD_Species, strHD_Index;
	strHD_Species = _T("BS");
	strHD_Index = _T("IBS");

	// TODO : 마크 구분별~ 코드 적용
	INT ids = -1;
	
	CStringArray arrIgnorAlias;
	arrIgnorAlias.Add( strHD_Index + _T("_형식구분_코드"));
	arrIgnorAlias.Add( strHD_Index + _T("_매체구분_코드"));

	// 형태 구분, 매체구분을 알맞게 넣어준다.

	CMarc marc;
	CString strStreamMarc;
	CString strFormCode;
	CString strMatCode;

	// 형태 구분, 매체구분을 알맞게 넣어준다.
	m_pCM->GetControlMgrData(_T("IBS_형식구분_코드"), strFormCode ) ;
	m_pCM->GetControlMgrData(_T("IBS_매체구분_코드"), strMatCode ) ;


	m_pDM_Index->SetCellData( strHD_Index + _T("_형식구분_코드"), strFormCode, nIdxDM);
	m_pDM_Index->SetCellData( strHD_Index + _T("_매체구분_코드"), strMatCode, nIdxDM);

	
	MakeDMFromCM(m_pCM, m_pDM_Species, -1, nIdxDM, strHD_Species, &arrIgnorAlias);
	MakeDMFromCM(m_pCM, m_pDM_Index, -1 , nIdxDM, strHD_Index, &arrIgnorAlias);

	EFS_END
}



INT CWebHoldImport::MakeDMFromCM(CESL_ControlMgr *pCM, 
								  CESL_DataMgr *pDM, 
								  INT nAppMode /*= -1*/,
								  INT nIdx /*= -1*/, 
								  CString strToAliasHeader /*= _T("")*/, 
								  CStringArray *pArrIgnoreAlias /*= NULL*/)
{
	EFS_BEGIN

	if (pCM == NULL || pDM == NULL) return -1;
	
	INT i = 0;
	INT ids = -1;
	INT nFind = -1;

	if (nIdx == -1)
	{
		ids = pDM->InsertRow(-1);
		if (ids < 0) return -1;
		nIdx = pDM->GetRowCount() - 1;
	}

	CString strAlias;
	CString strItemData;
	CESL_Control_Element *pCE = NULL;
	
	POSITION pos = pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		
		strAlias = pCE->GetAlias();
		if (strAlias.IsEmpty()) continue;

		// 안보이는지 체크
		if (!reinterpret_cast<CWnd*>(pCM->FindControl(strAlias))->IsWindowEnabled()) continue;

		// Control 값을 가져온다
		ids = pCM->GetControlMgrData(strAlias, strItemData);
		if (ids < 0) continue;

		// 헤더 변경
		if (!strToAliasHeader.IsEmpty())
		{
			nFind = strAlias.Find(_T("_"));
			if (nFind >= 0)
				strAlias = strAlias.Mid(nFind + 1);
			strAlias = strToAliasHeader + _T("_") + strAlias;
		}

		// Default Ignore List
		// 단행
		if ( strAlias == _T("IBS_수입구분_코드") ) continue;

		// 다르면 DB FLAG를 바꾸어준다.
		if (pDM->GetCellData(strAlias, nIdx) != strItemData)
		{
			ids = pDM->SetCellData(strAlias, strItemData, nIdx);
			ids = pDM->SetCellData(strToAliasHeader + _T("_DB_FLAG"), _T("I"), nIdx);
		}
		if (ids < 0) continue;
	}
	return 0;

	EFS_END
	return -1;
}


INT CWebHoldImport::MakeCallNo(CMarc *pMarc, INT nGridIdx )
{

	CCreateAutorCode *pCreateAuthorCode = NULL;

	CString strClass;
	m_pCM->GetControlMgrData(_T("IBS_분류표구분"), strClass );

	CString strClassNo;
	m_pDM->GetCellData(_T("분류번호"), nGridIdx, strClassNo );

	if (strClass.IsEmpty())
	{
		AfxMessageBox( _T("분류표구분을 선택하세요.") );
		return -1;
	}

	CString strClassCodeTag;
	if (strClass == _T("1"))
	{
		strClassCodeTag = _T("056$a");
	}
	else if (strClass == _T("2"))
	{
		strClassCodeTag = _T("082$a");
	}
	else
		return -1;

	m_pInfo->pMarcMgr->SetItem( pMarc, strClassCodeTag , strClassNo );
	m_pInfo->pMarcMgr->SetItem( pMarc, _T("090$a") , strClassNo );

	CString s090a = strClassNo;

	CESL_DataMgr *pDmAuthorRule =  FindDM( _T("DM_BO_CAT_PROC_0300_AUTHOR_RULE") );
	if ( pDmAuthorRule == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_AUTHOR_RULE error") );
		return -1;
	}

	CString strPublishFormCode;
	INT ids = -1;
	strPublishFormCode = _T("MO");
	
	CString sFormCode;
	CString sSubRegCode;
	CString strItemData;
	m_pCM->GetControlMgrData(_T("IBS_형식구분_코드"), sFormCode) ;
	m_pCM->GetControlMgrData(_T("IBS_보조등록구분_코드"), sSubRegCode) ;


	// 분류기호를 조건으로 하고 검색을 해서 없으면, 분류기호가 NULL인 자료를 검색한다.
	CString strWhere;
	strWhere.Format(_T("PUBLISH_FORM_CODE = '%s' AND FORM_CODE = '%s' AND SUB_REG_CODE = '%s' AND CLASS_CODE = '%s'")
					 , strPublishFormCode, sFormCode, sSubRegCode, strClass);

	ids = pDmAuthorRule->RefreshDataManager(strWhere);
	if (ids < 0) return -1;
	if ( pDmAuthorRule->GetRowCount() < 1 ) {
		// 분류기호가 NULL 인것으로 다시 검색한다.
		strWhere.Format(_T("PUBLISH_FORM_CODE = '%s' AND FORM_CODE = '%s' AND SUB_REG_CODE = '%s' AND CLASS_CODE IS NULL")
					 , strPublishFormCode, sFormCode, sSubRegCode);
		ids = pDmAuthorRule->RefreshDataManager(strWhere);

		if (ids < 0) return -1;
		if (pDmAuthorRule->GetRowCount() < 1) {
			AfxMessageBox (_T("저자기호설정이 잘못되었습니다.\n코드 값에 맞는 저자기호 설정이 있는지 확인해주세요"));
			return -1;
		}
	}
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDmAuthorRule, _T("표준저자기호"), _T("생성방법"));
	INT nAuthorCount = pDmAuthorRule->GetRowCount();
		
	CString strTagCount;
	CString strTag;
	CString strSubfield;
	CString strRuleType;
	if ( nAuthorCount > 0 ) 
	{
		pDmAuthorRule->GetCellData( _T("TAG_CNT"), 0, strTagCount );
		pDmAuthorRule->GetCellData( _T("TAG"), 0, strTag );
		pDmAuthorRule->GetCellData( _T("식별기호"), 0, strSubfield );
		pDmAuthorRule->GetCellData( _T("생성방법"), 0, strRuleType );
	}

		if (strRuleType != _T("수입순번호")) 
	{	
		CString strFilePath;
		// 저자기호 설정에 맞는 마크 데이터를 가져온다.
		INT iTagCnt = _ttoi( strTagCount.GetBuffer(0) );
		for ( INT i = 1 ; i <= iTagCnt ; i++ ) {
			CString sTagSub;
			if ( i == 1 ) {
				if ( strTag.GetLength() < 3 || strSubfield.GetLength() < 1 ) {
					AfxMessageBox( _T("저자기호설정이 잘못되었습니다.") );
					return -1;
				}
				sTagSub = strTag.Left(3) + _T("$") + strSubfield.Left(1);
			}
			else {
				INT iStart = (i-1)*4;
				INT iTmp = (i-1)*2 +1;
				if ( strTag.GetLength() < iStart+3 || strSubfield < iTmp ) {
					AfxMessageBox( _T("저자기호설정이 잘못되었습니다.") );
					return -1;
				}
				sTagSub = strTag.Mid(iStart, 3) + _T("$") + strSubfield.Mid( (i-1)*2 , 1 );
			}
			strItemData = _T("");
			m_pInfo->pMarcMgr->GetItem(pMarc, sTagSub, strItemData);
			if ( strItemData != _T("") ) {
				break;
			}
		}

		CMakeSearchData MakeData(this);

		//한자 한글 변환
		ids = MakeData.ChangeHanjaToHangul( strItemData );	
		if (ids < 0) 
		{			
			AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
			return -1;
		}
		
		//공백, 특수문자 제거
		ids = MakeData.GetKeyWord( strItemData );				
		if (ids < 0) 
		{			
			AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
			return -1;
		}

		// 데이터가 널이면
		if (strItemData.IsEmpty())
		{
			AfxMessageBox (_T("저자기호설정이 잘못되었습니다."));
			return -1;
		}

		// 저자기호 가지고 오기 strItemData : result입니다.
		
		// 장일세
		if (strRuleType == _T("장일세"))
		{
			if (!pCreateAuthorCode)
				pCreateAuthorCode = new CCreateAutorCode;

			CString strAuthorAddData;
			CString strAuthorCode;

			ids = pCreateAuthorCode->GetJangIlSeaAuthorData(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();

			if (ids < 0) return -1;

			strItemData = strAuthorCode;
		}
		//정필모
		else if( strRuleType == _T("정필모") )
		{
			if (!pCreateAuthorCode)
				pCreateAuthorCode = new CCreateAutorCode;
			
			CString strAuthorAddData;
			CString strAuthorCode;
			
			pCreateAuthorCode->GetJungPilMoAuthorData(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();
			
			if (ids < 0) return -1;
			
			strItemData = strAuthorCode;
			
		}
		//동양서저자기호
		else if( strRuleType == _T("동양서저자기호") )
		{
			if (!pCreateAuthorCode)
				pCreateAuthorCode = new CCreateAutorCode;

			CString strAuthorAddData;
			CString strAuthorCode;

			strAuthorCode = pCreateAuthorCode->GetOrientBookData(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();

			if (ids < 0) return -1;

			strItemData = strAuthorCode;

		}
		else if ( strRuleType == _T("카터셈본") )
		{
			if (!pCreateAuthorCode)
				pCreateAuthorCode = new CCreateAutorCode;
			
			CString strAuthorAddData;
			CString strAuthorCode;
			
			strAuthorCode = pCreateAuthorCode->GetCutterSanbornAuthorData(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();
			
			if (ids < 0) return -1;
			
			strItemData = strAuthorCode;
		
		}
		else if ( strRuleType == _T("ELORD") ) 
		{

			if (!pCreateAuthorCode)
				pCreateAuthorCode = new CCreateAutorCode;
			
			CString strAuthorAddData;
			CString strAuthorCode;
			
			//Elord 저자기호 생성시 저자에 공백이 있을경우 공백이전까지 짤라서 처리한다.
			INT iTagCnt = _ttoi( strTagCount.GetBuffer(0) );
			for ( INT i = 1 ; i <= iTagCnt ; i++ ) {
				CString sTagSub;
				if ( i == 1 ) {
					sTagSub = strTag.Left(3) + _T("$") + strSubfield.Left(1);
				}
				else {
					INT iStart = (i-1)*3+1;
					INT iTmp = (i-1)*2 +1;
					sTagSub = strTag.Mid(iStart, 3) + _T("$") + strSubfield.Mid( (i-1)*2 , 1 );
				}
				
				m_pInfo->pMarcMgr->GetItem(pMarc, sTagSub, strItemData);
				if ( strItemData != _T("") ) {
					break;
				}
			}

			ids = pCreateAuthorCode->GetElordAuthorDate(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();
			
			if (ids < 0) return -1;
			
			strItemData = strAuthorCode;
			
		}
		else
		{
			if (strItemData.GetLength()<2)
			{
				AfxMessageBox( _T("동양서의 저자는 한글 1자리이상이거나 숫자2자리 이상이어야 합니다.") );
				return -1;
			}
				
			CString sData = strItemData.Left(2);
			CString sAuthorMid, sAuthorNo;
			if ( strItemData.Mid(2,1) == _T(" ") ) 
				sAuthorMid = _T(",") + strItemData.Mid(3,2);
			else 
				sAuthorMid = strItemData.Mid(2,2);
			
			if ( sAuthorMid != _T("") ) 
			{			
				//파일경로
				strFilePath = _T("..\\CFG\\") + strRuleType + _T(".txt");
				
				FILE *fp;
				fp = _tfopen( strFilePath , _T("rb") );
				
				if ( fp == NULL ) {
					AfxMessageBox( _T("저자기호설정 파일이 없습니다.") );
					return -1;
				}
				
				//++++++++++++++++++++++++++++++++++++++++++
				//Unicode일땐 2byte건너띄기
				FILE* fpUni = fp;
			
				if( 0xFF == fgetc(fpUni) ) 
				{
					if( 0xFE != fgetc(fpUni) )
					{
						fseek(fpUni, 0, SEEK_SET);
					}
				}
				else
				{
					fseek(fpUni, 0, SEEK_SET);
				}
				//------------------------------------------
				
				TCHAR cLine[256];
				while( _fgetts( cLine, 256, fp ) ) {
					CString sLine;
					sLine.Format( _T("%s"), cLine );
					CString tnp = sLine.Mid(0,3);
					tnp.Replace( _T(" "), NULL );
					if ( sAuthorMid == tnp ) {
						INT iPos = sLine.Find(_T("|"));
						sAuthorNo = sLine.Mid( iPos+1, 3 );
						sAuthorNo.Replace( _T(" "), NULL );
						if ( sAuthorMid.Left(1) == _T(",") ) {
							sAuthorNo = _T(",") + sAuthorNo;
						}
						break;
					}				
				}
			}
			
			if ( sAuthorNo.IsEmpty())
			{
				AfxMessageBox( _T("저자기호를 찾을 수 없습니다. 저자기호설정 파일을 확인하세요.") );
			}
			
			strItemData = sData + sAuthorNo;
		}
		
		//초성만들기
		CString sTitle;
		m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$a"), sTitle);

		// 관칭 제거
		CIndex::RemoveAllParenthesis(sTitle);

		//한자 한글 변환
		ids = MakeData.ChangeHanjaToHangul( sTitle );	
		if (ids < 0) 
		{			
			AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
			return -1;
		}
		
		//공백, 특수문자 제거
		ids = MakeData.GetKeyWord( sTitle );				
		if (ids < 0) 
		{			
			AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
			return -1;
		}
		
		// 공백이 없는 두자를 가져오자
		INT nCount = 0;
		CString sFirstWord;
		INT nLenght = sTitle.GetLength();
		for (INT idx = 0; idx < nLenght; idx++)
		{
			TCHAR tch = sTitle.GetAt(idx);
			if (!_istspace(tch)) 
			{
				sFirstWord += tch;
				nCount++;
			}
			
			if (nCount == 2) break;
		}

/* 수정 사항 : 저자기호생성시 리재철2표에서 서명의 첫글자를 가져올지 초성을 가져올지를 설정 
 * 수정 날짜 : 2003.12.05
 * 수정 사람 : 김홍수 
 */

		CString strResult;
		CString strQuery = _T(" SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_1 = 'L' AND GROUP_2 = '정리' and CLASS_ALIAS = '저자기호생성방법' AND MANAGE_CODE = UDF_MANAGE_CODE");
		
		//m_pDM_Parent->GetOneValueQuery(strQuery, strResult);
		pDmAuthorRule->GetOneValueQuery(strQuery, strResult);


		if ( strResult != _T("서명첫자")) {
			if ( sFirstWord != _T("") ) {
				FILE *fpFirst;
				strFilePath = _T("..\\CFG\\FIRST_CHAR_TBL.txt");
				fpFirst = _tfopen( strFilePath , _T("rb") );			
				if ( fpFirst == NULL ) {
					AfxMessageBox( _T("FIRST_CHAR_TBL.tx 파일이 없습니다.") );
					return -1;
				}
				
				//++++++++++++++++++++++++++++++++++++++++++
				//Unicode일땐 2byte건너띄기
				FILE* fpUni = fpFirst;
			
				if( 0xFF == fgetc(fpUni) ) 
				{
					if( 0xFE != fgetc(fpUni) )
					{
						fseek(fpUni, 0, SEEK_SET);
					}
				}
				else
				{
					fseek(fpUni, 0, SEEK_SET);
				}
				//------------------------------------------
				
				TCHAR cLine[256];
				while( _fgetts( cLine, 256, fpFirst ) ) {
					CString sLine;
					sLine.Format( _T("%s"), cLine );
					if ( sLine.Left(1) == _T(".") ) {
						continue;
					}
					CString sTmp = sLine.Left(2);
					if ( sFirstWord == sLine.Left(2) ) {
						INT iPos = sLine.Find(_T("|"));
						CString sResult = sLine.Mid( iPos+1, 2 );
						strItemData = strItemData + sResult;
						break;
					}
				}
			}
		}
		else 
			strItemData = strItemData + sFirstWord;

		m_pInfo->pMarcMgr->SetItem(pMarc, _T("090$a"), s090a);
		m_pInfo->pMarcMgr->SetItem(pMarc, _T("090$b"), strItemData);
	}
	
	// 단행일경우 090$c 만들기
	CArray<CString, CString&> pArray;
	CString sItemData;
	m_pInfo->pMarcMgr->GetItem(pMarc, _T("049$v"), sItemData, &pArray);
		
	CString sRegPre;
	INT iCnt = pArray.GetSize();
	for ( INT i = 1 ; i <= iCnt ; i ++ ){
		CString sReg = pArray.GetAt(i-1);
		if ( 1 == i ) {
			sItemData = sReg;
		}
		else if ( sRegPre != sReg ) {
			sItemData = sItemData + _T(",") + sReg;
		}
		sRegPre = sReg;
	}

	if ( sItemData != _T("") ) {
		m_pInfo->pMarcMgr->SetItem(pMarc, _T("090$c"), sItemData);
	}


	return 0;

}

BOOL CWebHoldImport::ExecuteMarcImport( INT nGridIdx )
{

	// 종DM
	CESL_DataMgr DM_Species;
	DM_Species.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	DM_Species.InitDMPKFromFile(_T("DM_단행_정리DM_종정보"));
	DM_Species.SetMarcMgr(m_pInfo->pMarcMgr);

	// 색인DM
	CESL_DataMgr DM_Index;
	DM_Index.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	DM_Index.InitDMPKFromFile(_T("DM_단행_정리DM_색인정보"));
	DM_Index.SetMarcMgr(m_pInfo->pMarcMgr);
	
	// 책DM
	CESL_DataMgr DM_Book;
	DM_Book.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	DM_Book.InitDMPKFromFile(_T("DM_단행_정리DM_책정보"));
	DM_Book.SetMarcMgr(m_pInfo->pMarcMgr);

	// 권DM
	CESL_DataMgr DM_Volume;
	DM_Volume.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	DM_Volume.InitDMPKFromFile(_T("DM_단행_정리DM_권정보"));
	DM_Volume.SetMarcMgr(m_pInfo->pMarcMgr);

	INT ids = -1;
	INT idxDM = -1;
	INT nCount = -1;

	INT nColCount = -1;

	INT nRowTo = 0;
	INT nRowsTo = 0;

	CString strData;
	CString strErrorMsg;
    CString nTotalCnt;
    CString strTotalMsg;
    CString nFalseCnt;
	CString nTrueCnt;

	// HSS_040524 헤더세팅
	CString strHD_Book, strHD_Species, strHD_Index;
	strHD_Species = _T("BS");
	strHD_Book = _T("BB");
	strHD_Index = _T("IBS");

	// 마크정리 DM으로 변경한다.
	MakeMarcAdjustDM(m_pDM_Species, 0, m_pDM_Index, 0, &DM_Species, &DM_Index, &DM_Book, &DM_Volume);


	//등록일 적용
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	ids = DM_Book.SetCellData( strHD_Book + _T("_등록일"), result, 0);

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//등록번호 12자리 맨들기
	CString strRegNo;
	ids = m_pDM_Book->GetCellData( strHD_Book + _T("_등록번호"), 0, strRegNo);
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
// 		if ( strRegNo.GetLength() > 2 ) {
// 			CString sRight;
// 			sRight.Format( _T("%010s") , strRegNo.Mid(2) );
// 			strRegNo = strRegNo.Left(2) + sRight;
// 		}
// 	}

	//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	if ( strRegNo != _T("") && strRegNo.GetLength() < 12 ) {
		if ( strRegNo.GetLength() > nRegCodeLength ) {
			strRegNo = m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength) );
		}
	}
	//=====================================================
	
	ids = DM_Book.SetCellData( strHD_Book + _T("_등록번호"), strRegNo, 0 );

	//정리상태
	DM_Species.SetCellData( strHD_Species + _T("_정리상태"), _T("1") , 0 );

	//REC_KEY 처리
	//Web 반입시에는 web 반입 TBL과 Rec_key가 같아야 한다.
	DM_Species.SetCellData( strHD_Species + _T("_종KEY"), m_pDM->GetCellData(_T("REC_KEY"), nGridIdx ), 0 );

	//별치기호 등록구분 적용
	CString strRegCode;
	strRegCode.Empty();
	m_pCM->GetControlMgrData( _T("BB_등록구분"), strRegCode);
	DM_Book.SetCellData(_T("BB_등록구분_코드"), strRegCode , 0);

	CString strShelfCode;
	strShelfCode.Empty();
	m_pDM->GetCellData( _T("별치기호"), nGridIdx, strShelfCode );
	m_pDM_Book->SetCellData( _T("BB_별치기호_코드"), strShelfCode, 0 );
	DM_Book.SetCellData( _T("BB_별치기호_코드"), strShelfCode, 0 );

	m_pDM_Index->SetCellData( _T("IBS_별치기호_코드"), strShelfCode, 0 );
	DM_Index.SetCellData( _T("IBS_별치기호_코드"), strShelfCode, 0 );

	// 0 - 부서보유자료
	// 1 - 연구용역
	// 2 - 연수결과
	INT nDataFormidx = m_pMDataFormCombo->GetCurSel();
	switch ( nDataFormidx )
	{
		case 1:
			m_pDM_Index->SetCellData( _T("IBS_한국정부기관부호"), _T("AK"), 0 );
			break;
	}

	// 2005.08.03 ADD BY PDJ
	// 5163 웹반입일 경우 배가실을 설정된 값으로 세팅한다.
	CString	strShelfLocCode;
	INT nBookCnt;	
	INT	i;
	
	m_pCM->GetControlMgrData( _T("IBS_자료실구분_코드"), strShelfLocCode);
	nBookCnt = DM_Book.GetRowCount();
	for ( i=0 ; i<nBookCnt ; i++ )
	{
		DM_Book.SetCellData(_T("BB_배가위치부호"), strShelfLocCode, i );
	}
	//반입 수행
	ids = m_pCatApi->WebImportSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume, NULL, NULL, NULL, MNG_IMPORT );
	if ( ids < 0 ) 
	{
		AfxMessageBox( m_pCatApi->GetLastError() );
		return FALSE;
	}

	return TRUE;
}

INT CWebHoldImport::MakeMarcAdjustDM(	CESL_DataMgr *pDM_From_Species, 
										INT nFromSpeciesRow, 
										CESL_DataMgr *pDM_From_Index, 
										INT nFromIndexRow, 
										CESL_DataMgr *pDM_To_Species, 
										CESL_DataMgr *pDM_To_Index, 
										CESL_DataMgr *pDM_To_Book, 
										CESL_DataMgr *pDM_To_Volume)
{
	EFS_BEGIN

	if (!pDM_From_Species ||
		!pDM_From_Index ||
		!pDM_To_Index ||
		!pDM_To_Species ||
		!pDM_To_Book ||
		!pDM_To_Volume ||
		nFromSpeciesRow < 0 ||
		nFromIndexRow < 0) return -1;

	// 데이터 초기화
	pDM_To_Species->FreeData();
	pDM_To_Index->FreeData();
	pDM_To_Book->FreeData();
	pDM_To_Volume->FreeData();

	pDM_To_Species->InsertRow(-1);
	pDM_To_Index->InsertRow(-1);
	pDM_To_Book->InsertRow(-1);
	pDM_To_Volume->InsertRow(-1);
	

	// 종 카피
	INT ii = pDM_To_Species->GetRowCount();
	m_pCatApi->CopyDM(pDM_From_Species, nFromSpeciesRow, pDM_To_Species, 0);
	// 색인 카피
	m_pCatApi->CopyDM(pDM_From_Index, nFromIndexRow, pDM_To_Index, 0);

	// 마크싱크를 맞추기 전에 049에 해당하는 정보를 책 DM에 넣는다...
	CMarc marc;
	CString strStreamMarc = pDM_To_Species->GetCellData( _T("BS_MARC"), 0);
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);

	CString strRegNo;
	CString strItemData;
	CArray<CString, CString&> arrItem;
	m_pInfo->pMarcMgr->GetItem(&marc, _T("049$l"), strItemData, &arrItem);
	INT nRowCount = arrItem.GetSize();

	// 049 Tag이 있으면 다시 만든다.
	if (nRowCount >= 0) 
	{	
		pDM_To_Book->FreeData();
	}

	// 049 Tag의 개수만큼 책을 만든다.
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		pDM_To_Book->InsertRow(-1);

		// 책부록 플래그
		pDM_To_Book->SetCellData(_T("BB_책부록플래그"), _T("B"), idx);

		// DB FLAG 세팅
		pDM_To_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);

		// 종 -> 책
		m_pCatApi->CopyDM(pDM_To_Species, 0, pDM_To_Book, idx, _T("BB"));
		// 색인 -> 책
		m_pCatApi->CopyDM(pDM_To_Index, 0, pDM_To_Book, idx, _T("BB"));
	}

	// 대표책 설정
	INT nRepBookIdx = m_pCatApi->SetRepresentationBook(pDM_To_Book, _T("BB"));

	// 마크와 싱크맞춤
	m_pCatApi->ConvertMarcToCatDM(&marc, pDM_To_Species, pDM_To_Index, pDM_To_Book, pDM_To_Volume);

	return 0;

	EFS_END
	return -1;

}

VOID CWebHoldImport::OnSelchangecDataStatus() 
{
	// 2007.06.25 REM BY PDJ
	// 5163요구사항으로 모든자료삭제 가능하다록
	/*
	INT nDataStatusidx = m_pMDataStatusCombo->GetCurSel();
	if ( 0 == nDataStatusidx )
		GetDlgItem(IDC_bDelete)->ShowWindow(SW_SHOW);
	else 
		GetDlgItem(IDC_bDelete)->ShowWindow(SW_HIDE);
	*/	
}

BOOL CWebHoldImport::ExtraQuery( INT nGridIdx )
{

	//WEB_IMPORT_TMP_TBL 에 상태 변경 쿼리
	INT ids = -1;
	CString strQuery;
	strQuery = _T("UPDATE WEB_IMPORT_TMP_TBL SET WORKING_STATUS = 'B01' WHERE REC_KEY ='") + m_pDM->GetCellData(_T("REC_KEY"), nGridIdx) + _T("';");
	ids = m_pDM->AddFrame( strQuery );
	if ( ids < 0 ) return FALSE;

	//DL 팀 요구사항
	//DL_CONTENTS_MAP_TBL 에 입력해주어야 할 사항

	//생성해서 만들어주어야 할 값
	//REC_KEY, REG_DATE

	//DB에서 값을 가져와 입력해야 할 값
	//SPECIES_KEY, PATH, FILE_NAME, , SERVER_ID, ACCESSION_NO,

	//기본값 적용
	//TYPE				= ????????
	//VOL_SORT_NO		='10'
	//DB_FLAG			='M'
	//MAX_SERVICE		=DB 기본값
	//SERVICE_CLASS		=DB 기본값

	CString strServerId;
	strQuery = _T("SELECT SERVER_ID FROM DL_CONTENTS_SERVER_TBL WHERE TYPE = 'PDF' AND ACTIVE_FLAG = '0'");
	ids = m_pDM->GetOneValueQuery ( strQuery , strServerId );
//	if ( ids < 0 ) return FALSE;

	CString strRecKey;
	strRecKey.Empty();
	ids = m_pDM->MakeRecKey( strRecKey );
	if ( ids < 0 ) return FALSE;
	
	CString strRegDate;
	CTime t = CTime::GetCurrentTime();
	strRegDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	m_pDM->InitDBFieldData();
    m_pDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"), strRecKey );
	m_pDM->AddDBFieldData (_T("SPECIES_KEY"),_T("NUMERIC"), m_pDM->GetCellData(_T("REC_KEY"), nGridIdx ) );
	m_pDM->AddDBFieldData (_T("VOL_SORT_NO"),_T("NUMERIC"), _T("10") );
	m_pDM->AddDBFieldData (_T("DB_FLAG"),_T("STRING"), _T("M") );
	m_pDM->AddDBFieldData (_T("ACCESSION_NO"),_T("STRING"), m_pDM->GetCellData(_T("등록번호"), nGridIdx ) );
	m_pDM->AddDBFieldData (_T("REG_DATE"),_T("STRING"), strRegDate );
	m_pDM->AddDBFieldData (_T("MAX_SERVICE"), _T("STRING") , _T("9999") );
	m_pDM->AddDBFieldData (_T("SERVICE_CLASS"),_T("STRING"), _T("0") );
	m_pDM->AddDBFieldData (_T("TYPE"),_T("STRING"), _T("PDF") );
	//m_pDM->AddDBFieldData (_T("SERVER_ID"),_T("STRING"), strServerId );
	m_pDM->AddDBFieldData (_T("SERVER_ID"),_T("NUMERIC"), m_pDM->GetCellData(_T("SERVER_ID"), nGridIdx ) );

	m_pDM->AddDBFieldData (_T("PATH"),_T("STRING"), m_pDM->GetCellData(_T("PATH"), nGridIdx ) );
	m_pDM->AddDBFieldData (_T("FILENAME"),_T("STRING"), m_pDM->GetCellData(_T("FILENAME"), nGridIdx ) );
//	m_pDM->AddDBFieldData (_T("ORI_FILENAME"),_T("STRING"), m_pDM->GetCellData(_T("ORI_FILENAME"), nGridIdx ));

    ids = m_pDM->MakeInsertFrame (_T("DL_CONTENTS_MAP_TBL"));
	if ( ids < 0 ) return FALSE;


	//IDX_BO_TBL 에 원문유무가 있을경우 WONMUN_YN에 _T("Y") 로 셋팅
	strQuery = _T("UPDATE IDX_BO_TBL SET WONMUN_YN = 'Y' WHERE REC_KEY = ") + m_pDM->GetCellData(_T("REC_KEY"), nGridIdx ) + _T(";");
	ids = m_pDM->AddFrame( strQuery );
	if ( ids < 0 ) return FALSE;

	return TRUE;

}

HBRUSH CWebHoldImport::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
