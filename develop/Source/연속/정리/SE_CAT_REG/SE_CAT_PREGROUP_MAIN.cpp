// SE_CAT_PREGROUP_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_PREGROUP_MAIN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "SE_CAT_GIVE_REGNO.h"
#include "MultiSortDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_PREGROUP_MAIN dialog


CSE_CAT_PREGROUP_MAIN::CSE_CAT_PREGROUP_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CAT_PREGROUP_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CSE_CAT_PREGROUP_MAIN::~CSE_CAT_PREGROUP_MAIN()
{
}
BOOL CSE_CAT_PREGROUP_MAIN::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);
}

INT CSE_CAT_PREGROUP_MAIN::DoSort()
{
	CString strDMAlias;
	CString strCMAlias;
	
	strDMAlias = _T("DM_연속_정리_가원부_메인");
	strCMAlias = _T("CM_연속_정리_가원부_메인");
	

	CESL_DataMgr *pDM = FindDM(strDMAlias);
	if (pDM == NULL) return -1;

	CMultiSortDlg dlg(this);

	CESL_Grid * pGrid = (CESL_Grid*)FindControl(strCMAlias, _T("MAIN_GRID"));
	if(pGrid == NULL)
		return -1;
	
	if (dlg.DoModal() == IDOK)
	{
		pDM->SORT(dlg.m_nSortCount, dlg.m_arrStrSortAlias, dlg.m_arrIntSortCond, ESL_DATAMGR_SORT_ASC);

		pGrid->Display();
	}
	return 0;
}

VOID CSE_CAT_PREGROUP_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_PREGROUP_MAIN)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_PREGROUP_MAIN, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_PREGROUP_MAIN)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_PREGROUP_MAIN message handlers

BOOL CSE_CAT_PREGROUP_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_연속_정리_가원부_메인")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_Grid * pGrid = (CESL_Grid*)FindControl(_T("CM_연속_정리_가원부_메인"), _T("MAIN_GRID"));
	if(pGrid) pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



INT CSE_CAT_PREGROUP_MAIN::GiveAllRegNo()
{

	CSE_CAT_GIVE_REGNO dlg(this);
	dlg.DoModal();

	return 0;
}

INT CSE_CAT_PREGROUP_MAIN::BackToRegData()
{
	INT idx;
	SelectMakeList(_T("CM_연속_정리_가원부_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_연속_정리_가원부_메인"), _T("MAIN_GRID"));

	if(idx < 0)
	{
		//에러처리
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;
	}	

	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_가원부_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_연속_정리_가워부_메인") );
		return -1;
	}

	CString BookKey,strStatus;
	strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

	if (strStatus != _T("SER113O")) //가원부 
	{
		//에러처리
		ESLAfxMessageBox(_T("가원부 자료만 처리 할 수 있습니다."));
		return -1;
	}

	idx = SelectGetNext(_T("CM_연속_정리_가원부_메인"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

		if (strStatus != _T("SER113O")) //가원부 
		{
			//에러처리
			ESLAfxMessageBox(_T("가원부 자료만 처리 할 수 있습니다."));
			return -1;
		}

		idx = SelectGetNext(_T("CM_연속_정리_가원부_메인"), _T("MAIN_GRID"));
	}



	if(IDCANCEL == ESLAfxMessageBox(_T("등록인계자료 상태로 복귀 작업을 하시겠습니까?"), MB_OKCANCEL))
		return -1;



	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_연속_정리_가원부_메인"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_연속_정리_가원부_메인 error") );
		return -1;
	}


	//대상 책 키 가져오기
	SelectMakeList(_T("CM_연속_정리_가원부_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_연속_정리_가원부_메인"), _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_책KEY") , idx );

	// 변경 spl 실행
	INT Dm_index=0;
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("ACCESSION_REC_KEY"), _T("STRING"), _T("") );
	pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("SER111N") );
	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("SER113O") );
	pDM->MakeUpdateFrame(_T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);	
	
	pDM->DeleteRow(idx);
	Dm_index++;
	
	idx = SelectGetNext(_T("CM_연속_정리_가원부_메인"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_책KEY") , (idx - Dm_index) );

		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("ACCESSION_REC_KEY"), _T("STRING"), _T("") );
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("SER111N") );
		pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), _T("SER113O") );
		pDM->MakeUpdateFrame(_T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, (idx - Dm_index));

		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;


		idx = SelectGetNext(_T("CM_연속_정리_가원부_메인"), _T("MAIN_GRID"));
	}

	pDM->SendFrame();

	CESL_Grid *pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_연속_정리_가원부_메인"), _T("MAIN_GRID"));
	pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	pGrid_MainGrid_All->Display();

	AfxMessageBox(_T("복귀 되었습니다."));

	return 0;


}
INT CSE_CAT_PREGROUP_MAIN::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN
    if (strGridAlias == _T("MAIN_GRID")) {
        if (strDMFieldAlias == _T("BB_자료상태_코드")) {
            if ( str == _T("SER111N")) {
				str = _T("등록인계자료");
			}
			else if ( str == _T("SER112N") ) {
				str = _T("등록자료");
			}
			else if ( str == _T("SER113O") ) {
				str = _T("가원부자료");
			}
        }
    }
	return 0;


EFS_END
return -1;

}

VOID CSE_CAT_PREGROUP_MAIN::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID);
	if (pGrid->GetSafeHwnd() == NULL) return;

	pGrid->MoveWindow(-2, 35, cx+2, cy);	
	
}

HBRUSH CSE_CAT_PREGROUP_MAIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
