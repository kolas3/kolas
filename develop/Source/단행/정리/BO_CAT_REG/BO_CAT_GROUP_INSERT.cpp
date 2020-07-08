// BO_CAT_GROUP_INSERT.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_GROUP_INSERT.h"
#include "BO_CAT_MAKE_preGROUP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_GROUP_INSERT dialog


CBO_CAT_GROUP_INSERT::CBO_CAT_GROUP_INSERT(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_GROUP_INSERT)
	m_Year = _T("");
	//}}AFX_DATA_INIT
}



CBO_CAT_GROUP_INSERT::~CBO_CAT_GROUP_INSERT()
{
	
}

BOOL CBO_CAT_GROUP_INSERT::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);

	
}
VOID CBO_CAT_GROUP_INSERT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_GROUP_INSERT)
	DDX_Text(pDX, IDC_GROUP_YEAR, m_Year);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_GROUP_INSERT, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_GROUP_INSERT)
	ON_BN_CLICKED(IDC_GROUP_SEARCH, OnGroupSearch)
	ON_BN_CLICKED(IDC_bSaveNo, OnbSaveNo)
	ON_BN_CLICKED(IDC_NEW_GROUP, OnNewGroup)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_GROUP_INSERT message handlers

BOOL CBO_CAT_GROUP_INSERT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here


	if(OpenMode == 0) //가원부 작성 
	{
		if (InitESL_Mgr(_T("SM_단행_정리_가원부대장_추가")) < 0)
		{
			AfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}

		m_DM_ALIAS = _T("DM_단행_정리_등록_메인"); 
		m_CM_ALIAS = _T("CM_단행_정리_등록_메인");
	}
	else if(OpenMode == 1) // 가원부 자료 - 가원부 추가 복귀 관리 
	{
		if (InitESL_Mgr(_T("SM_단행_정리_가원부대장_추가")) < 0)
		{
			AfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}

		m_DM_ALIAS = _T("DM_단행_정리_가원부_메인");
		m_CM_ALIAS = _T("CM_단행_정리_가원부_메인");
	}
	else if(OpenMode == 2) // 원부 대장 관리 - 원부 추가 복귀 관리
	{
		if (InitESL_Mgr(_T("SM_단행_정리_원부대장_추가")) < 0)
		{
			AfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}
		
		GetDlgItem(IDC_NEW_GROUP)->ShowWindow(FALSE);

		m_DM_ALIAS = _T("DM_단행_정리_원부_메인"); 
		m_CM_ALIAS = _T("CM_단행_정리_원부_메인");
	}

	INT idx;
	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));

	if(idx < 0)
	{
		//에러처리
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}	

	if(OpenMode == 0) //등록인계 자료인지 확인 
	{
		CString strStatus;
		CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리_등록_메인"));
		strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

		if ( strStatus != _T("BOR111N"))
		{
			ESLAfxMessageBox(_T("등록된 자료가 있습니다."));
			EndDialog(IDCANCEL);
			return FALSE;
		}

		while ( idx != -1 ) {
			
			strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );
			
			if ( strStatus != _T("BOR111N"))
			{
				ESLAfxMessageBox(_T("등록된 자료가 있습니다."));
				EndDialog(IDCANCEL);
				return FALSE;
			}
			
			
			idx = SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
		}


	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 		
	DisplayData();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CBO_CAT_GROUP_INSERT::DisplayData()
{
	CESL_DataMgr *pDM;

	pDM = FindDM(m_DM_ALIAS);

	// 040226_HSS 수정 LocalTime으로 Select +++++++++++++++
	//CTime mTime = pDM->GetDBTime();
	CTime mTime = CTime::GetCurrentTime();
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Year.Format(_T("%d"), mTime.GetYear());

	UpdateData(FALSE);

	OnGroupSearch();

	return 0;
}

BEGIN_EVENTSINK_MAP(CBO_CAT_GROUP_INSERT, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_GROUP_INSERT)
	ON_EVENT(CBO_CAT_GROUP_INSERT, IDC_GROUPGRID, -600 /* Click */, OnClickGroupgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_CAT_GROUP_INSERT::OnClickGroupgrid() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid *pGrid;

	if (OpenMode == 2)
		pGrid=( CESL_Grid * )FindControl(_T("CM_단행_정리_원부대장_추가"), _T("GRID"));
	else
		pGrid=( CESL_Grid * )FindControl(_T("CM_단행_정리_가원부대장_추가"), _T("GRID"));
		
		
	INT row = pGrid->GetRow();
	
	if (row == 0) return;
	if (row > 0) {
		pGrid->SetReverse(row-1);
		m_GROUP_NO = pGrid->GetAt(row-1,0);
		m_GROUP_REC_KEY = pGrid->GetAt(row-1,4);
	}
	
}

VOID CBO_CAT_GROUP_INSERT::OnGroupSearch() 
{
	// TODO: Add your control notification handler code here
	CString strQry,tmpQry;
	INT ids;

	CESL_DataMgr *pDM;

	if( OpenMode == 0 )
	{
		pDM = FindDM(_T("DM_단행_정리_가원부대장_추가"));
	}
	else if(OpenMode == 1 )
	{
		pDM = FindDM(_T("DM_단행_정리_가원부대장_추가"));
	}
	else if(OpenMode == 2 )
	{
		pDM = FindDM(_T("DM_단행_정리_원부대장_추가"));
	}

	UpdateData(TRUE);

	m_Year.TrimLeft();
	m_Year.TrimRight();


	if( m_Year != _T("") )
	{
		tmpQry.Format(_T(" ACCESSION_REC_MAKE_YEAR = %s "), m_Year);
		strQry = strQry + tmpQry;
	}	

	ids = pDM->RefreshDataManager(strQry);
	if (ids < 0 )
	{
		AfxMessageBox(_T("RefreshDataManager 를 확인해 주세요"));
		return;
	}

	CESL_ControlMgr *pCM;
	if( OpenMode == 0 )
	{
		pCM = FindCM(_T("CM_단행_정리_가원부대장_추가"));
	}
	else if(OpenMode == 1 )
	{
		pCM = FindCM(_T("CM_단행_정리_가원부대장_추가"));
	}
	else if(OpenMode == 2 )
	{
		pCM = FindCM(_T("CM_단행_정리_원부대장_추가"));
	}



	pCM->AllControlDisplay();

}

VOID CBO_CAT_GROUP_INSERT::OnbSaveNo() 
{
	// TODO: Add your control notification handler code here
	INT idx,Dm_index=0;
	CString BookKey;


	m_GROUP_REC_KEY.TrimLeft();
	m_GROUP_REC_KEY.TrimRight();

	if( m_GROUP_REC_KEY == _T("") )
	{
		AfxMessageBox(_T("원부를 선택하여 주십시요!"));	
		return ;
	}

	CESL_DataMgr *pDM ;
	CESL_ControlMgr *pCM;
	pCM = FindCM(m_CM_ALIAS);
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_단행_정리_원부_메인 error") );
		return ;
	}
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	pDM = FindDM(m_DM_ALIAS);
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_단행_정리_원부_메인") );
		return ;
	}

	//대상 책 키 가져오기
	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_책KEY") , idx );

	// 변경 spl 실행
	INT ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return;
	}

	pDM->InitDBFieldData();
	if( OpenMode == 0 )
	{
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOR113O") );
	}

	pDM->AddDBFieldData(_T("ACCESSION_REC_KEY"), _T("NUMERIC"), m_GROUP_REC_KEY );
	ids = pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);
	if ( 0 > ids ) return;

	// 040226_HSS IDX_BO_TBL, IDX_TMP_BO_TBL 추가 +++++++++++++++++++++++++++++++
	CString sRec_Key;
	if( OpenMode == 0 )
	{
		sRec_Key = pDM->GetCellData( _T("IBS_종KEY") , idx );

		pDM->InitDBFieldData ();
		pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC113N") );
		pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRec_Key, idx);

		CString strToday, sQuery;
		INT ids;
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		sQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
// 				_T("VALUES (ESL_SEQ.NEXTVAL, %s, 'I', sysdate);") ,sRec_Key);
// 
// 		ids = pDM->AddFrame( sQuery );
// 		if ( 0 > ids ) 
// 		{
// 			AfxMessageBox(_T("StartFrame 를 확인하세요"));
// 			return;
// 		}
		//=====================================================

		
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	/*	
	ids = pDM->SendFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("SendFrame 를 확인하세요"));
		return ;
	}
	*/
	if ( OpenMode == 0)
	{
		pDM->SetCellData(_T("BB_자료상태_코드"), _T("BOR113O"), idx);
		pGrid->DisplayLine(idx);
	}
	else if ( OpenMode == 1)
	{
		pDM->SetCellData(_T("가원부번호"), m_GROUP_NO, idx);
		pGrid->DisplayLine(idx);

	}
	else if( OpenMode == 2)
	{
		pDM->SetCellData(_T("원부번호"), m_GROUP_NO, idx);
		pGrid->DisplayLine(idx);

	}
		
	pGrid->DisplayLine(idx);
	
	idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_책KEY") , idx );

		/*
		ids = pDM->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return;
		}
		*/

		pDM->InitDBFieldData();
		
		if( OpenMode == 0 )
		{
			pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOR113O") );
		}
	
		pDM->AddDBFieldData(_T("ACCESSION_REC_KEY"), _T("NUMERIC"), m_GROUP_REC_KEY );
		ids = pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);
		if ( 0 > ids ) return;

		// 040226_HSS IDX_BO_TBL, IDX_TMP_BO_TBL 추가 +++++++++++++++++++++++++++++++
		if( OpenMode == 0 )
		{
			sRec_Key = pDM->GetCellData( _T("IBS_종KEY") , idx );

			pDM->InitDBFieldData ();
			pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), _T("BOC113N") );
			pDM->MakeUpdateFrame(_T("IDX_BO_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRec_Key, idx);

			CString strToday, sQuery;
			INT ids;

			//=====================================================
			//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 			sQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (REC_KEY, SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE) ")
// 					_T("VALUES (ESL_SEQ.NEXTVAL, %s, 'I', sysdate);") ,sRec_Key);
// 
// 			ids = pDM->AddFrame( sQuery );
// 			if ( 0 > ids ) 
// 			{
// 				AfxMessageBox(_T("StartFrame 를 확인하세요"));
// 				return;
// 			}
			//=====================================================
			
		}
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
		if ( OpenMode == 0)
		{
			pDM->SetCellData(_T("BB_자료상태_코드"), _T("BOR113O"), idx);
			pGrid->DisplayLine(idx);

		}else if( OpenMode == 1)
		{
			pDM->SetCellData(_T("가원부번호"), m_GROUP_NO, idx);
			pGrid->DisplayLine(idx);

		}else if( OpenMode == 2) 
		{
			pDM->SetCellData(_T("원부번호"), m_GROUP_NO, idx);
			pGrid->DisplayLine(idx);

		}

		idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	}

	ids = pDM->SendFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("SendFrame 를 확인하세요"));
		return;
	}
	
	if ( OpenMode == 0)
	{
		pGrid->Display();
	}

	AfxMessageBox(_T("변경 되었습니다."));

}

VOID CBO_CAT_GROUP_INSERT::OnNewGroup() 
{
	// TODO: Add your control notification handler code here
	CBO_CAT_MAKE_preGROUP Dlg(this);
	Dlg.DoModal();

	INT ids = Dlg.GetCreateYN();
	if (ids>0) DisplayData();

}
HBRUSH CBO_CAT_GROUP_INSERT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
