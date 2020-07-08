// BO_CAT_PREGROUP_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PREGROUP_MAIN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BO_CAT_GIVE_REGNO.h"
/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PREGROUP_MAIN dialog


CBO_CAT_PREGROUP_MAIN::CBO_CAT_PREGROUP_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PREGROUP_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSortItemSet = NULL;

}

CBO_CAT_PREGROUP_MAIN::~CBO_CAT_PREGROUP_MAIN()
{
	

}

BOOL CBO_CAT_PREGROUP_MAIN::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);

	
}
VOID CBO_CAT_PREGROUP_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PREGROUP_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_PREGROUP_MAIN, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PREGROUP_MAIN)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PREGROUP_MAIN message handlers

BOOL CBO_CAT_PREGROUP_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_단행_정리_가원부_메인")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_Grid * pGrid = (CESL_Grid*)FindControl(_T("CM_단행_정리_가원부_메인"), _T("MAIN_GRID"));
	if(pGrid) pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;	

	if (pGrid == NULL) return FALSE;
	pGrid->InitPopupMenuItem();
	pGrid->AddPopupMenuItem(static_cast<CView*>(pMain), this->VIEW_STYLE);

	InitSortItem();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CBO_CAT_PREGROUP_MAIN::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID);
	if (pGrid->GetSafeHwnd() == NULL) return;

	pGrid->MoveWindow(-2, 35, cx + 4, cy-35);	

}

INT CBO_CAT_PREGROUP_MAIN::GiveAllRegNo()
{

	CBO_CAT_GIVE_REGNO dlg(this);
	dlg.DoModal();

	return 0;
}

INT CBO_CAT_PREGROUP_MAIN::BackToRegData()
{
	INT idx;
	SelectMakeList(_T("CM_단행_정리_가원부_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_단행_정리_가원부_메인"), _T("MAIN_GRID"));

	if(idx < 0)
	{
		//에러처리
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;
	}	

	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_가원부_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_단행_정리_가워부_메인") );
		return -1;
	}

	CString BookKey,strStatus,strSpeciesKey;
	strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

	if (strStatus != _T("BOR113O")) //등록자료 
	{
		//에러처리
		ESLAfxMessageBox(_T("가원부 자료만 처리 할 수 있습니다."));
		return -1;
	}

	idx = SelectGetNext(_T("CM_단행_정리_가원부_메인"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

		if (strStatus != _T("BOR113O")) //등록자료 
		{
			//에러처리
			ESLAfxMessageBox(_T("가원부 자료만 처리 할 수 있습니다."));
			return -1;
		}

		idx = SelectGetNext(_T("CM_단행_정리_가원부_메인"), _T("MAIN_GRID"));
	}



	if(IDCANCEL == ESLAfxMessageBox(_T("등록인계자료 상태로 복귀 작업을 하시겠습니까?"), MB_OKCANCEL))
		return -1;



	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_단행_정리_가원부_메인"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_단행_정리_가원부_메인 error") );
		return -1;
	}


	//대상 책 키 가져오기
	SelectMakeList(_T("CM_단행_정리_가원부_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_단행_정리_가원부_메인"), _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_책KEY") , idx );

	// 변경 spl 실행
	INT Dm_index=0;
	INT ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("ACCESSION_REC_KEY"), _T("STRING"), _T("") );
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOR111N") );
	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("BOR113O") );
	ids = pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);	
	if ( 0 > ids ) return -1;

	// 040225_HSS IDX_BO_TBL 추가 ++++++++++++++
	strSpeciesKey = pDM->GetCellData( _T("IBS_종KEY") , idx );
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOR111N") );
	ids = pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey, idx);	
	if ( 0 > ids ) return -1;
	// +++++++++++++++++++++++++++++++++++++++++

	// 040226_HSS IDX_TMP_BO_TBL 추가 ++++++++++++++
	CString sQuery;
	CString strToday;
	CTime t = CTime::GetCurrentTime();
	strToday.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	sQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
// 				_T("VALUES (ESL_SEQ.NEXTVAL, %s, 'I', '%s');")
// 				,strSpeciesKey, strToday);
// 	ids = pDM->AddFrame( sQuery );
// 	if ( 0 > ids ) return -1;
	//=====================================================
	
	// +++++++++++++++++++++++++++++++++++++++++

	pDM->DeleteRow(idx);
	Dm_index++;
	
	idx = SelectGetNext(_T("CM_단행_정리_가원부_메인"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_책KEY") , (idx - Dm_index) );

		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("ACCESSION_REC_KEY"), _T("STRING"), _T("") );
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOR111N") );
		pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("BOR113O") );
		ids = pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, (idx - Dm_index));
		if ( 0 > ids ) return -1;

		// 040225_HSS IDX_BO_TBL 추가 ++++++++++++++
		strSpeciesKey = pDM->GetCellData( _T("IBS_종KEY") , (idx - Dm_index) );
		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOR111N") );
		ids = pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey, (idx - Dm_index));	
		if ( 0 > ids ) return -1;
		// +++++++++++++++++++++++++++++++++++++++++

		// 040226_HSS IDX_TMP_BO_TBL 추가 ++++++++++++++
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		sQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
// 					_T("VALUES (ESL_SEQ.NEXTVAL, %s, 'I', '%s');")
// 					,strSpeciesKey, strToday);
// 		ids = pDM->AddFrame( sQuery );
// 		if ( 0 > ids ) return -1;
		//=====================================================
		
		// +++++++++++++++++++++++++++++++++++++++++

		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;


		idx = SelectGetNext(_T("CM_단행_정리_가원부_메인"), _T("MAIN_GRID"));
	}

	ids = pDM->SendFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("SendFrame 를 확인하세요"));
		return -1;
	}

	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_단행_정리_가원부_메인"), _T("MAIN_GRID"));
	pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	pGrid_MainGrid_All->Display();

	AfxMessageBox(_T("복귀 되었습니다."));

	return 0;


}
INT CBO_CAT_PREGROUP_MAIN::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN
    if (strGridAlias == _T("MAIN_GRID")) {
        if (strDMFieldAlias == _T("BB_자료상태_코드")) {
            if ( str == _T("BOR111N")) {
				str = _T("등록인계자료");
			}
			else if ( str == _T("BOR112N") ) {
				str = _T("등록자료");
			}
			else if ( str == _T("BOR113O") ) {
				str = _T("가원부자료");
			}
        }
    }
	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_PREGROUP_MAIN::ShowSortDlg()
{
	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_가원부_메인"));
	
	if(m_pSortItemSet == NULL || pDM == NULL) 
	{
		ESLAfxMessageBox(_T("정렬화면을 초기화 할 수 없습니다."));
		return;
	}
	if(pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("정렬할 자료가 없습니다."));
		return ;
	}
	
	CDataSortDlg dlg(this);
	dlg.SetMainDM(pDM);
	dlg.SetSortItemSet(m_pSortItemSet);
	dlg.SetOpenMode(1);
	if(IDOK == dlg.DoModal())
		((CESL_Grid*)(FindCM(_T("CM_단행_정리_가원부_메인"))->FindControl(_T("MAIN_GRID"))))->Display();
}

BOOL CBO_CAT_PREGROUP_MAIN::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	CESL_Grid * pGrid = (CESL_Grid *)FindControl(_T("CM_단행_정리_가원부_메인"), _T("MAIN_GRID"));
	CESL_DataMgr * pDM = FindDM(_T("DM_단행_정리_가원부_메인"));
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(pGrid);
	m_pSortItemSet->InitDataManager(pDM->CONNECTION_INFO, _T("DM_정렬항목설정"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MAIN_GRID"));
/*
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("BB_수입구분"));
	pTmpData->Add(_T("BP_수입년도"));
	pTmpData->Add(_T("BP_그룹번호"));
	pTmpData = m_pSortItemSet->GetTailSortList();
	pTmpData->Add(_T("UDF_일련번호"));
	pTmpData->Add(_T("BB_일련번호"));
	
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("BB_자료상태_설명"), _T("BB_자료상태_코드"));
*/
	
	m_pSortItemSet->ReadSortItem();
	return TRUE;
}

HBRUSH CBO_CAT_PREGROUP_MAIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}