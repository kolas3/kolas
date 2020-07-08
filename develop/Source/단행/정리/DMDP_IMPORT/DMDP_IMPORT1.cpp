// DMDP_IMPORT1.cpp : implementation file
//

#include "stdafx.h"
#include "DMDP_IMPORT1.h"
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
// CDMDP_IMPORT dialog
CMarcAdjLauncher m_MarcAdjLaunCher;

CDMDP_IMPORT::CDMDP_IMPORT(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDMDP_IMPORT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = pParent;

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pDM_Species = NULL;
	m_pDM_Index = NULL;
	m_pDM_Book = NULL;

//	m_MarcAdjLaunCher = NULL;
}


CDMDP_IMPORT::~CDMDP_IMPORT()
{
	
}


VOID CDMDP_IMPORT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDMDP_IMPORT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDMDP_IMPORT, CDialog)
	//{{AFX_MSG_MAP(CDMDP_IMPORT)
	ON_BN_CLICKED(IDC_bSearch, OnbSearch)
	ON_BN_CLICKED(IDC_bImport, OnbImport)
	ON_BN_CLICKED(IDC_bClose, OnbClose)
	ON_WM_CTLCOLOR() 
	ON_CBN_SELCHANGE(IDC_cmbSHELF_CODE_MA, OnSelchangecmbSHELFCODEMA)
	ON_BN_CLICKED(IDC_btnREQUES, OnbtnREQUES)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMDP_IMPORT message handlers

BOOL CDMDP_IMPORT::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CDMDP_IMPORT::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	m_pCatApi = new CCatApi_BO(this);
	
	if (InitESL_Mgr(_T("SM_단행_정리_학위논문반입")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	
	m_pCM = FindCM(_T("CM_단행_정리_학위논문반입"));
	if ( m_pCM == NULL ) 
	{
		AfxMessageBox(_T("FindCM Error"));
		return FALSE;
	}
	

	m_pDM = FindDM(_T("DM_단행_정리_학위논문반입"));
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
	if ( m_pDM_Book == NULL ) 
	{
		AfxMessageBox(_T("FindDM Error"));
		return FALSE;
	}		
	
	m_pGrid = static_cast<CESL_Grid*>(FindControl(_T("CM_단행_정리_학위논문반입"), _T("GRID")));
	if (m_pGrid == NULL) return FALSE;
		
	InitComboControl();	


	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
//	m_MarcAdjLaunCher = new CMarcAdjLauncher(this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDMDP_IMPORT::InitComboControl()
{
	m_pMDataStatusCombo = new CJComboBox;
	m_pMDataStatusCombo->SubclassDlgItem( IDC_cDataStatus, this );
	m_pMDataStatusCombo->FormatList( 1, DT_LEFT, false , EDGE );
	m_pMDataStatusCombo->RequireFlag = FALSE;

	m_pMDataStatusCombo->AddItem( _T("신청"), 0, 0 ); //A01
	m_pMDataStatusCombo->AddItem( _T("완료"), 0, 1 ); //B01	

	/****************************************************************
	 ****************************************************************/
	//20080229 ADD BY PJW : 학위구분 추가 IDC_ACADEMIC_CLASS
	m_pMDataAcademicClass = new CJComboBox;
	m_pMDataAcademicClass->SubclassDlgItem( IDC_ACADEMIC_CLASS, this );
	m_pMDataAcademicClass->FormatList( 1, DT_LEFT, false , EDGE );
	m_pMDataAcademicClass->RequireFlag = FALSE;

	m_pMDataAcademicClass->AddItem( _T("적용안함"), 0, 0 );
	m_pMDataAcademicClass->AddItem( _T("안보"), 0, 1 ); 
	m_pMDataAcademicClass->AddItem( _T("석사"), 0, 2 ); 
	m_pMDataAcademicClass->AddItem( _T("야간석사"), 0, 3 ); 
	/****************************************************************
	 ****************************************************************/
	
	//기본값을 신청으로 셋팅
	m_pMDataStatusCombo->SetCurSel( 0 );

	GetDlgItem(IDC_btnREQUES)->ShowWindow(SW_HIDE);
	m_pMDataAcademicClass->SetCurSel( 2 );

	/***************************************************************************
	 ***************************************************************************/
	//20080225 ADD BY PJW : 등록 번호 코드 DM으로 초기화
	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_cmbREGCODE_CLASS2);
	pCMB->SelectString(_T("DM"));
	CESL_ControlMultiComboBox* pCMB1 = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_cmbSHELF_CODE_MA);
	pCMB1->SelectString(_T("DM"));
	/***************************************************************************
	 ***************************************************************************/

	return true;
}


HBRUSH CDMDP_IMPORT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


VOID CDMDP_IMPORT::OnbSearch() 
{
	CString strWhere = MakeWhereQuery();
	m_pDM->RefreshDataManager( strWhere );
	INT i = m_pDM->GetRowCount();

	/*****************************************************************
	 *****************************************************************/
	//20080225 ADD BY PJW : DM에 초기값 적용
	CString strShelfCode;
	CString strPage;
	CString strTempPage;
	INT nFindChar;
	CESL_ControlMultiComboBox* pCMB1 = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_cmbSHELF_CODE_MA);
	pCMB1->GetData(strShelfCode);

	for( INT j = 0 ; j < i ; j++ )
	{
		m_pDM->GetCellData(_T("페이지")  , j	       ,strPage);
		strTempPage.Format(_T("%sp.")	 , strPage);
		m_pDM->SetCellData(_T("페이지")  , strTempPage , j);
		m_pDM->SetCellData(_T("분류기호"), _T("393.8")     , j);
		m_pDM->SetCellData(_T("저자기호"), _T("국384ㅅ")   , j);
		m_pDM->SetCellData(_T("별치기호"), strShelfCode, j);
		
	}
	/*****************************************************************
	 *****************************************************************/

	m_pCM->AllControlDisplay();	
}

VOID CDMDP_IMPORT::OnbImport() 
{
	ExecuteDMDPImport();		
}

VOID CDMDP_IMPORT::OnbClose() 
{
	CDialog::OnCancel();
}

CString CDMDP_IMPORT::MakeWhereQuery()
{
	CString strWhere;
	// 0 - 삭제
	// 1 - 신청
	// 2 - 완료
	INT nDataStatusidx = m_pMDataStatusCombo->GetCurSel();
	switch ( nDataStatusidx )
	{
	case 0:
			strWhere += _T("STATUS = '0' ");
			( (CButton*) GetDlgItem (IDC_bImport) )->EnableWindow(TRUE);
			break;
	case 1:
			strWhere += _T("STATUS = '1' ");
			( (CButton*) GetDlgItem (IDC_bImport) )->EnableWindow(FALSE);
			break;	
	}

	return strWhere;
}

INT CDMDP_IMPORT::ExecuteDMDPImport()
{
	CString strQuery;
	CString REC_KEY;
	CString strStatus;
	INT nRowCount;

	m_pGrid->SelectMakeList();	
	nRowCount = m_pGrid->SelectGetCount();

	if ( nRowCount <= 0 ) 
	{
		AfxMessageBox(_T("반입할 자료를 선택해 주세요"));
		return FALSE;
	}
		
	INT		i;
	INT		nIndex;	
	INT		ids;

	m_pDM_Species->InsertRow(-1);
	m_pDM_Index->InsertRow(-1);
	m_pDM_Book->InsertRow(-1);
	
	INT nGridIdx;
	m_pGrid->SelectGetHeadPosition();
	for ( i=0 ; i<nRowCount ; i++ )
	{	
		nGridIdx = m_pGrid->SelectGetIdx();
		m_pDM->GetCellData(_T("상태"), nGridIdx, strStatus);
		if (  strStatus == _T("완료") ) continue;

		CMarc *pMarc = NULL;
		pMarc = new CMarc;

		ids = m_pDM->StartFrame();
		if ( 0 > ids ) 
		{
			m_pDM->EndFrame();
			return FALSE;
		}

		// 마크 생성
		if ( !MakeImportMarc( pMarc , nGridIdx ) ) 
		{		
			m_pDM->EndFrame();
			return FALSE;
		}
		// 각종 DM 생성
		if ( !ExecuteMarcImport( nGridIdx ) ) 
		{
			m_pDM->EndFrame();
			return FALSE;
		}

		/*
		// 수정화면 띄우기
		CESL_Grid *pGrid = NULL;

		pGrid = m_pGrid;
		m_MarcAdjLaunCher.SetParentInfo(m_pDM_Species, m_pDM_Index, pGrid, m_pGrid->GetRow() - 1);
		m_MarcAdjLaunCher.LaunchMarcAdjust(this, this, _T(""), MNG_IMPORT_WORKING, MODE_MONO);		
		*/
		m_pDM->GetCellData(_T("REC_KEY"), nGridIdx, REC_KEY );		
		strQuery.Format(_T("UPDATE DMDP_IMPORT_TMP_TBL SET STATUS='1' WHERE REC_KEY = %s;"), REC_KEY);
		m_pDM->InitDBFieldData();
		ids = m_pDM->AddFrame(strQuery);
		if (ids < 0) 
		{
			m_pDM->EndFrame();
			return FALSE;
		}

		m_pDM->SendFrame();

		// 반입화면에 완료 처리
		m_pGrid->SetTextMatrix(nGridIdx+1, 2, _T("완료"));
		m_pDM->SetCellData(_T("상태"), _T("완료"), nGridIdx);

		// 기본화면 그리드에 입력
		
		
		// 다음자료로 이동
		nGridIdx = m_pGrid->SelectGetNext();
	}

	/*
	m_pDM->InitDBFieldData();
	ids = m_pDM->SendFrame(TRUE);
	if ( ids )
	{
		AfxMessageBox(_T("반입 실패."));
		m_pDM->EndFrame();
	}
	*/
	m_pDM->EndFrame();
	
	// 검색화면 그리드 반입자료 지우기
	m_pGrid->SelectGetTailPosition();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		nGridIdx = m_pGrid->SelectGetIdx();

		m_pDM->DeleteRow(nGridIdx);
		m_pGrid->RemoveAt(nGridIdx);
		
		nGridIdx = m_pGrid->SelectGetPrev();
	}	


	AfxMessageBox(_T("정리중 자료로 반입되었습니다.\n정리작업을 하여 주십시오."));
		
	return 0;
}

BOOL CDMDP_IMPORT::MakeImportMarc(CMarc *pMarc, INT nGridIdx)
{
	//반입용 마크를 생성한다
	CMap<CString, LPCTSTR, CString, LPCTSTR> mapMarcAlias_DMAlias;
	InitMapMarcTag( mapMarcAlias_DMAlias );

	CString strStreamMarc = MakeStreamMarc( pMarc, mapMarcAlias_DMAlias, nGridIdx );
	if ( strStreamMarc.IsEmpty() ) return FALSE;	

	m_pInfo->pMarcMgr->Decoding( strStreamMarc, pMarc );	
	m_pInfo->pMarcMgr->Encoding( pMarc, strStreamMarc );
	
	//코드값들을 적용힌다
	CodeApply ();

	//생성된 StreamMarc를 종 Tbl에 넣는다
	m_pDM_Species->SetCellData(_T("BS_MARC"), strStreamMarc, 0 ) ;
	
	return TRUE;
}

BOOL CDMDP_IMPORT::ExecuteMarcImport(INT nGridIdx)
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
	nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
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
			strRegNo = m_pParent->m_pInfo->MakeRegNo(strRegNo.Left(nRegCodeLength), strRegNo.Mid(nRegCodeLength) );
		}
	}
	//=====================================================
	
	
	ids = DM_Book.SetCellData( strHD_Book + _T("_등록번호"), strRegNo, 0 );

	//정리상태
	DM_Species.SetCellData( strHD_Species + _T("_정리상태"), _T("0") , 0 );

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
		
	//반입 수행
	ids = m_pCatApi->DMDPImportSpecies(&DM_Species, &DM_Index, &DM_Book, &DM_Volume, NULL, NULL, NULL, MNG_IMPORT_WORKING );
	if ( ids < 0 ) 
	{
		AfxMessageBox( m_pCatApi->GetLastError() );
		return FALSE;
	}	

	// 목차초록반입
	CString strSpeciesKey;
	DM_Species.GetCellData(_T("BS_종KEY"), 0, strSpeciesKey);
	ids = MakeContents(nGridIdx, strSpeciesKey);
	if ( ids ) 
	{
		AfxMessageBox(_T("목차,초록 반입 실패"));
		return ids;
	}

	return TRUE;
}

INT CDMDP_IMPORT::MakeMarcAdjustDM(	CESL_DataMgr *pDM_From_Species, 
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

	if ( nRowCount <= 0 ) return 0;

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

BOOL CDMDP_IMPORT::ExtraQuery(INT nGridIdx)
{
	return TRUE;
}


VOID CDMDP_IMPORT::InitMapMarcTag( CMap<CString, LPCTSTR, CString, LPCTSTR>& mapMarcAlias_DMAlias )
{
	//pmapMarcAlias_DMAlias->RemoveAll( );
	//mapMarcAlias_DMAlias.SetAt(_T("008@0-39"),_T("008발행년"));
	mapMarcAlias_DMAlias.SetAt(_T("245$a"),_T("논문제목"));
	/*************************************************************************
	 *************************************************************************/
	//20080228 UPDATE BY PJW : 245$h -> 245$x 로 반입되도록 수정 
	//mapMarcAlias_DMAlias.SetAt("245$h","논문제목_영문");
	mapMarcAlias_DMAlias.SetAt(_T("245$x"),_T("논문제목_영문"));
	/*************************************************************************
	 *************************************************************************/
	mapMarcAlias_DMAlias.SetAt(_T("245$d"),_T("저자"));
	/*************************************************************************
	 *************************************************************************/
	//20080225 DELETE PJW : 1. 국방대 요청 650 -> 653 으로 반입
	//mapMarcAlias_DMAlias.SetAt("650$a","주제어");
	//mapMarcAlias_DMAlias.SetAt("650$a","주제어_영문");
	/*************************************************************************
	 *************************************************************************/
	mapMarcAlias_DMAlias.SetAt(_T("260$b"),_T("발행처"));
	mapMarcAlias_DMAlias.SetAt(_T("260$c"),_T("학위년도"));
	mapMarcAlias_DMAlias.SetAt(_T("300$a"),_T("페이지"));
	mapMarcAlias_DMAlias.SetAt(_T("502$a"),_T("학위구분"));	
	mapMarcAlias_DMAlias.SetAt(_T("502$a"),_T("학위명"));
	mapMarcAlias_DMAlias.SetAt(_T("502$c"),_T("세부학위명_전공"));
	mapMarcAlias_DMAlias.SetAt(_T("502$d"),_T("학위년도"));	
	/*************************************************************************
	 *************************************************************************/
	//20080225 ADD PJW : 1. 국방대 요청 650 -> 653 으로 반입
	//					 2. 056,090 디폴트로 반입(056 : 393.8 ,저자기호 : 국 384ㅅ )
	//                   3. 별가치기호(049$f ) DM으로
	//					 4. 지도교수 500$a, 653$a 반입
	//					 5. DM에 분류기호,저자기호,별치기호 추가 
	//.Lookup
	//1.
	//mapMarcAlias_DMAlias.SetAt("653$a","주제어");
	//mapMarcAlias_DMAlias.Lookup("653$a","주제어_영문");
	//2.
	mapMarcAlias_DMAlias.SetAt(_T("056$a"),_T("분류기호"));
	mapMarcAlias_DMAlias.SetAt(_T("090$a"),_T("분류기호"));
	mapMarcAlias_DMAlias.SetAt(_T("090$b"),_T("저자기호"));
	//3.
	mapMarcAlias_DMAlias.SetAt(_T("049$f"),_T("별치기호"));
	//4.
	mapMarcAlias_DMAlias.SetAt(_T("500$a"),_T("지도교수"));
	//mapMarcAlias_DMAlias.Lookup("653$a","지도교수");
	/*************************************************************************
	 *************************************************************************/
	/*************************************************************************
	 *************************************************************************/
	//20080229 ADD BY PJW : 090$c, 049$v 에 교번 추가
	mapMarcAlias_DMAlias.SetAt(_T("090$c"),_T("교번"));
	mapMarcAlias_DMAlias.SetAt(_T("049$v"),_T("교번"));

	/*************************************************************************
	 *************************************************************************/
}

CString CDMDP_IMPORT::MakeStreamMarc( CMarc *pMarc , 
									   CMap<CString, LPCTSTR, 
									   CString, LPCTSTR>& mapMarcAlias_DMAlias, 
									   INT nGridIdx )
{
	INT ids = -1;
	CString strMarcAlias;
	CString strDMAlias;
	CString strItemData;
	CString strStreamMarc = _T("");
	INT nCode;
	CString strCode;
	CString strAcademic;
	nCode = 31;
	strCode.Format( _T("%ca"), (TCHAR)nCode );

	POSITION pos = mapMarcAlias_DMAlias.GetStartPosition();

	/***************************************************************
	 ***************************************************************/
	//20080225 ADD BY PJW : 653$a 키워드화 코드
	CString strSub;
	CString strSubUSA;
	CString strProfessor;
	CString strConvertData;
	
	m_pDM->GetCellData( _T("주제어"), nGridIdx, strSub );
	m_pDM->GetCellData( _T("주제어_영문"), nGridIdx, strSubUSA );
	m_pDM->GetCellData( _T("지도교수"), nGridIdx, strProfessor );

	
	strSub.TrimLeft();		 strSub.TrimRight();
	strSubUSA.TrimLeft();	 strSubUSA.TrimRight();
	strProfessor.TrimLeft(); strProfessor.TrimRight();
	strSub.MakeUpper();
	strSubUSA.MakeUpper();
	strProfessor.MakeUpper();

	strConvertData = ConvertStrData( strSub		 );
	strSub = strConvertData;
	strConvertData = ConvertStrData( strSubUSA	 );
	strSubUSA = strConvertData;
	strConvertData = ConvertStrData( strProfessor );
	strProfessor = strConvertData;

	if( "" != strSub )
	{
		m_pInfo->pMarcMgr->SetItem(pMarc, "653$a", strSub);
	}
	if( "" != strSubUSA )
	{
		m_pInfo->pMarcMgr->SetItem(pMarc, "653$a", strSubUSA);
	}
	if( "" != strProfessor )
	{
		m_pInfo->pMarcMgr->SetItem(pMarc, "653$a", strProfessor);
	}
	//"석사논문", "학위논문", "석사학위논문",전공명,strCode
	CString strMajor; 
	m_pDM->GetCellData( _T("세부학위명_전공"), nGridIdx, strMajor );
	strAcademic.Format(_T("석사논문%s학위논문%s석사학위논문%s%s"), strCode, strCode, strCode,strMajor);
	m_pInfo->pMarcMgr->SetItem(pMarc, _T("653$a"), strAcademic);
	m_pInfo->pMarcMgr->SetItem(pMarc, _T("300$c"), _T("26cm"));

	/***************************************************************
	 ***************************************************************/
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
		

		if (ids >= 0)
			ids = m_pInfo->pMarcMgr->SetItem(pMarc, strMarcAlias, strItemData);
	}

	//등록번호 부여하여 049 Tag 생성
	MakeRegNO049Tag( pMarc, nGridIdx );
	m_pInfo->pMarcMgr->Encoding(pMarc, strStreamMarc );

	return strStreamMarc;
}

VOID CDMDP_IMPORT::CodeApply()
{
	EFS_BEGIN

	BeginWaitCursor();
	
	INT nRowCount = m_pDM_Index->GetRowCount();
	for (INT row = 0; row < nRowCount; row++)
		CodeApply(row);
	
	EndWaitCursor();


	EFS_END
}

VOID CDMDP_IMPORT::CodeApply(INT nIdxDM)
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


INT CDMDP_IMPORT::MakeDMFromCM(CESL_ControlMgr *pCM, 
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


BOOL CDMDP_IMPORT::MakeRegNO049Tag( CMarc *pMarc, INT nGridIdx )
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

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	INT iLastNum = _ttoi( strLasuNum.GetBuffer(0) );
	iLastNum ++;		

	CString strTmp;	
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	strTmp.Format(_T("%010d"), iLastNum );
// 
// 	//등록번호 만들기
// 	CString strRegNo = strRegCode + strTmp;
// 
// 	m_pDM->SetCellData(_T("등록번호"), strRegNo, nGridIdx );


	//등록번호 만들기
	CString strRegNo;
	strTmp.Format(_T("%d"), iLastNum );
	strRegNo = m_pParent->m_pInfo->MakeRegNo(strRegCode , strTmp);
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

	/***************************************************
	 ***************************************************/
	//20080228 ADD BY PJW : 교번 마크 추가
	CString strschoolNo;
	m_pDM->GetCellData( "교번", nGridIdx, strschoolNo );
	m_pInfo->pMarcMgr->SetItem( pMarc, "049$v", strschoolNo );
	/***************************************************
	 ***************************************************/

	m_pDM_Book->SetCellData( _T("BB_등록구분_코드"), strRegCode, 0 );

	CString strUpdateLastNum;
	strUpdateLastNum.Format(_T("%d"), iLastNum );
	CString strUpdateQuery = _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = ") + strUpdateLastNum  +_T(" WHERE KIND_CODE = 'REG_NO' AND DIVISION_VALUE = '") + strRegCode + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE;");
	ids = m_pDM->AddFrame( strUpdateQuery );
	if ( ids < 0 ) return FALSE;

	m_pDM->SendFrame(FALSE);

	return TRUE;
}

INT CDMDP_IMPORT::MakeContents(INT nGridIdx, CString strSpeciesKey)
{
	CString strContentsYN, strAbstractsYN;
	CString strContents, strAbstracts;
	CString strTocKey;	
	CString strConKey;
	CString strAbsKey;
	INT		ids;

	ids = m_pDM->GetCellData(_T("목차유무"), nGridIdx, strContentsYN );
	if ( ids ) return ids;
	ids = m_pDM->GetCellData(_T("초록유무"), nGridIdx, strAbstractsYN );
	if ( ids ) return ids;

	ids = m_pDM->GetCellData(_T("목차"), nGridIdx, strContents );
	if ( ids ) return ids;
	ids = m_pDM->GetCellData(_T("초록"), nGridIdx, strAbstracts );
	if ( ids ) return ids;

	m_pDM->StartFrame();
	m_pDM->InitDBFieldData();

	if ( strContentsYN == _T("Y")  || !strContents.IsEmpty()  || 
		 strAbstractsYN == _T("Y") || !strAbstracts.IsEmpty() )
	{
		m_pDM->MakeRecKey(strTocKey);
	}
	else
	{
		// 반입할 목차 초록이 없으면 RETURN
		return 0;
	}	

	// TOC_COMMON 생성
	m_pDM->InitDBFieldData();
	m_pDM->AddDBFieldData( _T("REC_KEY"), _T("STRING"), strTocKey);	
	m_pDM->AddDBFieldData( _T("SPECIES_KEY"), _T("STRING"), strSpeciesKey);
	if ( strContentsYN == _T("Y")  || !strContents.IsEmpty() )
	{
		m_pDM->MakeRecKey(strConKey);
		m_pDM->AddDBFieldData( _T("CONTENTS_KEY"), _T("STRING"), strConKey);
	}

	if ( strAbstractsYN == _T("Y") || !strAbstracts.IsEmpty() )
	{
		m_pDM->MakeRecKey(strAbsKey);
		m_pDM->AddDBFieldData( _T("ABSTRACTS_KEY"), _T("STRING"), strAbsKey);
	}	
	m_pDM->AddDBFieldData( _T("SPECIES_CLASS"), _T("STRING"), _T("0"));
	m_pDM->AddDBFieldData( _T("VOL_SORT_NO"), _T("STRING"), _T("10"));
	m_pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );
	m_pDM->MakeInsertFrame(_T("BO_TOC_COMMON_TBL"));

	// 목차 생성
	if ( strContentsYN == _T("Y")  || !strContents.IsEmpty() )
	{		
		m_pDM->InitDBFieldData();
		m_pDM->AddDBFieldData( _T("REC_KEY"), _T("STRING"), strConKey);	
		m_pDM->AddDBFieldData( _T("TOC_COMMON_KEY"), _T("STRING"), strTocKey);	
		m_pDM->AddDBFieldData( _T("CONTENTS"), _T("STRING"), strContents);	
		m_pDM->AddDBFieldData( _T("USE_LIMIT_CODE"), _T("STRING"), _T("GM"));	
		m_pDM->AddDBFieldData( _T("CREATE_DATE"), _T("NUMERIC"), _T("SYSDATE"));
		m_pDM->MakeInsertFrame(_T("BO_CONTENTS_TBL"));		
	}		

	// 초록 생성
	if ( strAbstractsYN == _T("Y")  || !strAbstracts.IsEmpty() )
	{		
		m_pDM->InitDBFieldData();
		m_pDM->AddDBFieldData( _T("REC_KEY"), _T("STRING"), strAbsKey);	
		m_pDM->AddDBFieldData( _T("TOC_COMMON_KEY"), _T("STRING"), strTocKey);	
		m_pDM->AddDBFieldData( _T("ABSTRACTS"), _T("STRING"), strAbstracts);	
		m_pDM->AddDBFieldData( _T("USE_LIMIT_CODE"), _T("STRING"), _T("GM"));	
		m_pDM->AddDBFieldData( _T("CREATE_DATE"), _T("NUMERIC"), _T("SYSDATE"));
		m_pDM->MakeInsertFrame(_T("BO_ABSTRACTS_TBL"));		
	}

	/*
	ids = m_pDM->SendFrame();
	if ( ids < 0 )
	{
		AfxMessageBox(_T("목차, 초록 반입 실패"));
	}
	*/

	return 0;

}

/******************************************************************
 ******************************************************************/
//20080225 ADD BY PJW : 색인어 만들기위한 데이터 변환함수
CString CDMDP_IMPORT::ConvertStrData( CString strData)
{
	INT nFindChar;
	INT ids;
	INT nLength;
	CString strConvertData;
	CString strConvertTempData;
	CString strFindData;
	CString strNextFindData;
	INT nCode;
	CString strCode;

	CMakeSearchData MakeData(m_pParent);

	nCode = 31;
	strCode.Format( _T("%ca"), (TCHAR)nCode );

	strData.Replace(_T(","),_T(" "));
	strData.Replace(_T("."),_T(" "));
	strData.Replace(_T("/"),_T(" "));
	strData.Replace(_T("("),_T(" "));
	strData.Replace(_T(")"),_T(" "));
	strData.Replace(_T("{"),_T(" "));
	strData.Replace(_T("}"),_T(" "));

	nFindChar = strData.Find(_T(" "));
	nLength = strData.GetLength();
	
	if( 1 < nFindChar )
	{
		strFindData = strData.Mid(0,nFindChar);
		strNextFindData = strData.Mid(nFindChar, nLength - nFindChar);
		strFindData.TrimLeft();		strFindData.TrimRight();
		strNextFindData.TrimLeft(); strNextFindData.TrimRight();
	}
	else
	{
		strFindData = strData;
		strNextFindData.Format(_T(""));
		strFindData.TrimLeft();		strFindData.TrimRight();
	}

	while( 1 )
	{
		//한자 한글 변환
		ids = MakeData.ChangeHanjaToHangul( strFindData );	
		if (ids < 0) 
		{			
			AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
			strConvertData.Format(_T("ERROR"));
			return strConvertData;
		}
		
		//공백, 특수문자 제거
		ids = MakeData.GetKeyWord( strFindData );				
		if (ids < 0) 
		{			
			AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
			strConvertData.Format(_T("ERROR"));
			return strConvertData;
		}

		if( "" != strConvertData)
		{
			strConvertTempData = strConvertData;
			strConvertData.Format( _T("%s%s%s"), strConvertTempData, strCode, strFindData );
		}
		else 
		{
			strConvertData.Format( _T("%s"), strFindData );
		}

		if( "" != strNextFindData )
		{
			nFindChar = strNextFindData.Find(_T(" "));
			nLength = strNextFindData.GetLength();
			if( 1 < nFindChar )
			{
				strFindData = strNextFindData.Mid( 0, nFindChar);
				strConvertTempData = strNextFindData.Mid( nFindChar, nLength - nFindChar );
				strNextFindData = strConvertTempData;
				strFindData.TrimLeft();		strFindData.TrimRight();
				strNextFindData.TrimLeft(); strNextFindData.TrimRight();
			}
			else
			{
				strFindData = strNextFindData;
				strNextFindData.Format(_T(""));
				strFindData.TrimLeft();		strFindData.TrimRight();
			}
			
			
		}
		else 
		{
			break;
		}
	}
	
	return strConvertData;
}

void CDMDP_IMPORT::OnbtnREQUES() 
{
	RrquestHandling();	
}

INT CDMDP_IMPORT::RrquestHandling()
{
	CString strQuery;
	CString REC_KEY;
	CString strStatus;
	INT nRowCount;

	m_pGrid->SelectMakeList();	
	nRowCount = m_pGrid->SelectGetCount();

	if ( nRowCount <= 0 ) 
	{
		AfxMessageBox(_T("신청처리할 자료를 선택해주세요."));
		return FALSE;
	}
		
	INT		i;
	INT		nIndex;	
	INT		ids;

	m_pDM_Species->InsertRow(-1);
	m_pDM_Index->InsertRow(-1);
	m_pDM_Book->InsertRow(-1);
	
	INT nGridIdx;
	m_pGrid->SelectGetHeadPosition();
	for ( i=0 ; i<nRowCount ; i++ )
	{	
		nGridIdx = m_pGrid->SelectGetIdx();
		m_pDM->GetCellData(_T("상태"), nGridIdx, strStatus);
		if (  strStatus == _T("신청") ) continue;
		
		m_pDM->GetCellData(_T("REC_KEY"), nGridIdx, REC_KEY );		
		CString strQuery;
		CString strCount;
		CString strSub;
		CString strMessage;

		strQuery.Format(_T("SELECT COUNT(1) FROM BO_SPECIES_TBL WHERE REC_KEY = %s"), REC_KEY);
		m_pDM->GetCellData( _T("논문제목"), nGridIdx, strSub); 
		m_pDM->GetOneValueQuery( strQuery, strCount );
		if( 1 <= _ttoi( strCount ) )
		{
			strMessage.Format(_T("[ 논문제목 : %s ]\n자료를 DB삭제후 신청처리 해주세요."), strSub);
			AfxMessageBox(strMessage);
			m_pGrid->SetTextMatrix(nGridIdx+1, 1, _T(""));
			m_pGrid->SelectMakeList();
			nRowCount = m_pGrid->SelectGetCount();

			if ( 0 >= nRowCount ) 
			{
				AfxMessageBox(_T("신청처리할 자료를 선택해주세요."));
				return FALSE;
			}
			m_pGrid->SelectGetHeadPosition();
			i--;
			continue;
		}
		strQuery.Format(_T("SELECT COUNT(1) FROM BO_DELETE_SPECIES_TBL WHERE REC_KEY = %s"), REC_KEY);
		m_pDM->GetOneValueQuery( strQuery, strCount );
		if( 1 <= _ttoi( strCount ) )
		{
			strMessage.Format(_T("[ 논문제목 : %s ]\n재활용자료관리에서 삭제 후 신청처리 해주세요."), strSub);
			AfxMessageBox(strMessage);
			m_pGrid->SetTextMatrix(nGridIdx+1, 1, _T(""));
			m_pGrid->SelectMakeList();			
			nRowCount = m_pGrid->SelectGetCount();

			if ( 0 >=  nRowCount) 
			{
				AfxMessageBox(_T("신청처리할 자료를 선택해주세요."));
				return FALSE;
			}
			m_pGrid->SelectGetHeadPosition();
			i--;
			
			continue;
		}

		ids = m_pDM->StartFrame();
		strQuery.Format(_T("UPDATE DMDP_IMPORT_TMP_TBL SET STATUS='0' WHERE REC_KEY = %s;"), REC_KEY);
		m_pDM->InitDBFieldData();
		ids = m_pDM->AddFrame(strQuery);
		if (ids < 0) 
		{
			m_pDM->EndFrame();
			return FALSE;
		}

		m_pDM->SendFrame();

		// 반입화면에 신청 처리
		m_pGrid->SetTextMatrix(nGridIdx+1, 2, _T("신청"));
		m_pDM->SetCellData(_T("상태"), _T("신청"), nGridIdx);

		// 기본화면 그리드에 입력
		
		
		// 다음자료로 이동
		nGridIdx = m_pGrid->SelectGetNext();
	}

	m_pDM->EndFrame();
	
	// 검색화면 그리드 반입자료 지우기
	m_pGrid->SelectGetTailPosition();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		nGridIdx = m_pGrid->SelectGetIdx();
		
		m_pDM->DeleteRow(nGridIdx);
		if( 0 != nGridIdx)
		{
			m_pGrid->RemoveAt(nGridIdx);
		}
		
		nGridIdx = m_pGrid->SelectGetPrev();	
	}	

	m_pGrid->Display();
	


	AfxMessageBox(_T("신청처리 되었습니다.\n신청으로 재검색 하여주십시오."));
		
	return 0;
}

/*****************************************************************
 *****************************************************************/
//20080225 ADD BY PJW : DM에 변경된 별치기호 적용
void CDMDP_IMPORT::OnSelchangecmbSHELFCODEMA() 
{
	INT i = m_pDM->GetRowCount();

	CString strShelfCode;
	CESL_ControlMultiComboBox* pCMB1 = (CESL_ControlMultiComboBox *)GetDlgItem(IDC_cmbSHELF_CODE_MA);
	pCMB1->GetData(strShelfCode);

	for( INT j = 0 ; j < i ; j++ )
	{
		m_pDM->SetCellData(_T("별치기호"), strShelfCode, j);
	
	}	
}
/*****************************************************************
 *****************************************************************/