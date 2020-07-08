// MNG_NEW_INDEX.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_NEW_INDEX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INSERT_MODE 0
#define UPDATE_MODE 1

/////////////////////////////////////////////////////////////////////////////
// CMNG_NEW_INDEX dialog


CMNG_NEW_INDEX::CMNG_NEW_INDEX(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_NEW_INDEX)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CMNG_NEW_INDEX::~CMNG_NEW_INDEX()
{
	
}


BOOL CMNG_NEW_INDEX::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);
}

VOID CMNG_NEW_INDEX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_NEW_INDEX)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_NEW_INDEX, CDialog)
	//{{AFX_MSG_MAP(CMNG_NEW_INDEX)
	ON_BN_CLICKED(IDC_SAVE_CODE, OnSaveCode)
	ON_BN_CLICKED(IDC_CODE_PRE, OnCodePre)
	ON_BN_CLICKED(IDC_CODE_NEXT, OnCodeNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_NEW_INDEX message handlers

BOOL CMNG_NEW_INDEX::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_관리_분류기호_연동색인어_입력")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
		
	if( OpenMode == UPDATE_MODE )
	{

		SelectMakeList(_T("CM_관리_분류기호_연동색인어_메인"), _T("MAIN_GRID"));
		m_INDEX = SelectGetHeadPosition(_T("CM_관리_분류기호_연동색인어_메인"), _T("MAIN_GRID"));


		if(m_INDEX < 0)
		{
			//에러처리
			ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
			EndDialog(0);
			return FALSE;
		}	
	
		GetDlgItem(IDC_CODE_PRE)->ShowWindow(true);
		GetDlgItem(IDC_CODE_NEXT)->ShowWindow(true);

		CESL_ControlMgr *pCM;
		pCM = FindCM(_T("CM_관리_분류기호_연동색인어_입력"));
	
		SetReverse(_T("CM_관리_분류기호_연동색인어_메인"), _T("MAIN_GRID"), m_INDEX);
		pCM->AllControlDisplay(m_INDEX);

	}	

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_NEW_INDEX::OnSaveCode() 
{
EFS_BEGIN

	if( OpenMode == INSERT_MODE)
	{
		if( INSERT() < 0 ) return;
		AfxMessageBox(_T("입력 하였습니다."));
	}
	else
	{
		if( UPDATE() < 0 ) return;	
		AfxMessageBox(_T("수정 하였습니다."));
	}

	//CDialog::OnOK();

EFS_END
}
INT CMNG_NEW_INDEX::INSERT()
{
EFS_BEGIN
	CString strIndexWord, strKDC, strDDC, strRecKey;
	INT ids = -1;

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_관리_분류기호_연동색인어_메인"));
	if ( NULL == pDM )
	{
		AfxMessageBox(_T("DM 초기화 에러"));
		return -1;
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_관리_분류기호_연동색인어_입력"));
	if ( NULL == pCM )
	{
		AfxMessageBox(_T("CM 초기화 에러"));
		return -1;
	}

	pDM->MakeRecKey(strRecKey);
	// get data from CM
	GetControlData( _T("CM_관리_분류기호_연동색인어_입력"), _T("색인어"), strIndexWord );
	GetControlData( _T("CM_관리_분류기호_연동색인어_입력"), _T("KDC분류기호"), strKDC );
	GetControlData( _T("CM_관리_분류기호_연동색인어_입력"), _T("DDC분류기호"), strDDC );
	
	INT n_index = pDM->GetRowCount();
	
	// set Date To DM
	pDM->InsertRow(-1);
	pDM->SetCellData(_T("REC_KEY"), strRecKey, n_index);
	pDM->SetCellData(_T("색인어"), strIndexWord, n_index);
	pDM->SetCellData(_T("KDC분류기호"), strKDC, n_index);
	pDM->SetCellData(_T("DDC분류기호"), strDDC, n_index);
	
	
	ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 을 확인하세요"));
		return -1;
	}

	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, n_index, pDM);	

	ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return -1;
	}

	ids = pDM->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame 을 확인하세요"));
		return -1;
	}

	AllControlDisplay(_T("CM_관리_분류기호_연동색인어_메인"), n_index);

	return 0;
EFS_END
	return -1;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2004.06.07 김소정 수정
INT CMNG_NEW_INDEX::UPDATE()
{
EFS_BEGIN
	CString strIndexWord, strKDC, strDDC, strRecKey;
	INT ids=-1;


	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_관리_분류기호_연동색인어_메인"));
	if ( NULL == pDM )
	{
		AfxMessageBox(_T("DM 초기화 에러"));
		return -1;
	}
	

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_관리_분류기호_연동색인어_입력"));
	if ( NULL == pCM )
	{
		AfxMessageBox(_T("CM 초기화 에러"));
		return -1;
	}

	// get data from CM
	GetControlData( _T("CM_관리_분류기호_연동색인어_입력"), _T("색인어"), strIndexWord );
	GetControlData( _T("CM_관리_분류기호_연동색인어_입력"), _T("KDC분류기호"), strKDC );
	GetControlData( _T("CM_관리_분류기호_연동색인어_입력"), _T("DDC분류기호"), strDDC );
	strRecKey = pDM->GetCellData(_T("REC_KEY"), m_INDEX);

	// set Date To DM
	pDM->SetCellData(_T("색인어"),	strIndexWord, m_INDEX);
	pDM->SetCellData(_T("KDC분류기호"), strKDC, m_INDEX);
	pDM->SetCellData(_T("DDC분류기호"), strDDC, m_INDEX);
	// 수정일자
	CTime t = CTime::GetCurrentTime();
	CString sDate;
	sDate.Format ( _T("%4d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay() );
	pDM->SetCellData(_T("수정일자"), sDate, m_INDEX);

	// Start Frame
	ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 을 확인하세요"));
		return -1;
	}
	
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, m_INDEX, pDM, _T("REC_KEY"), _T("NUMERIC"), strRecKey);	
	
	ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("수정사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return -1;
	}
	
	ids = pDM->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame 을 확인하세요"));
		return -1;
	}// End Frame

	AllControlDisplay(_T("CM_관리_분류기호_연동색인어_메인"), m_INDEX);
	

	return 0;
EFS_END
	return -1;

}
VOID CMNG_NEW_INDEX::DELETE_CODE()
{
EFS_BEGIN
	INT n_idx;
	SelectMakeList(_T("CM_관리_분류기호_연동색인어_메인"), _T("MAIN_GRID"));
	n_idx = SelectGetHeadPosition(_T("CM_관리_분류기호_연동색인어_메인"), _T("MAIN_GRID"));

	if(n_idx < 0)
	{
		//에러처리
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}	

	if(IDCANCEL == AfxMessageBox(_T("삭제 작업을 하시겠습니까?"), MB_OKCANCEL))	return;

	CESL_DataMgr *pDM = FindDM(_T("DM_관리_분류기호_연동색인어_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM초기화 에러: DM_관리_분류기호_연동색인어_메인") );
		return ;
	}

	CESL_Grid* p_grid;
	p_grid = (CESL_Grid*)FindControl(_T("CM_관리_분류기호_연동색인어_메인"), _T("MAIN_GRID"));
	if ( p_grid == NULL )
	{
		AfxMessageBox(_T("GRID 초기화 에러"));
		return ;
	}

	//FIND REC_KEY INDEX
	INT nSelectCount = 0;
	nSelectCount = p_grid->SelectGetCount();
	if ( nSelectCount <= 0 ) return ;

	INT ids=-1;
	ids = pDM->StartFrame();
	if ( ids<0 )
	{
		AfxMessageBox(_T("StartFrame을 확인하세요"));
		return;
	}

	CString strRecKey;
	for ( INT i=0; i<nSelectCount; i++ )
	{
		strRecKey = pDM->GetCellData(_T("REC_KEY"), n_idx);
		
		// Add Frame
		pDM->MakeDeleteFrame(pDM->TBL_NAME, _T("REC_KEY"), _T("STRING"), strRecKey);

		ids = p_grid->RemoveAt(n_idx);
		if ( ids<0 ) return;

		ids = pDM->DeleteRow(n_idx);
		if ( ids<0 ) return;

		// 삭제된 수만큼 그리드 row 수도 삭제됨.
		n_idx = p_grid->SelectGetNext() - (i+1);

	}// end of for
/*
	strQry.Format(_T("DELETE FROM %s WHERE REC_KEY = %s;"), pDM->TBL_NAME ,strRecKey);
	pDM->AddFrame(strQry);
	pDM->DeleteRow(idx);
	delete_Count++;

	idx = SelectGetNext(_T("CM_관리_분류기호_연동색인어_메인"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strRecKey = pDM->GetCellData( _T("REC_KEY") , idx - delete_Count);

		strQry.Format(_T("DELETE FROM %s WHERE REC_KEY = %s;"), pDM->TBL_NAME ,strRecKey);
		pDM->AddFrame(strQry);
		pDM->DeleteRow(idx);
		delete_Count++;

		idx = SelectGetNext(_T("CM_관리_분류기호_연동색인어_메인"), _T("MAIN_GRID"));
	}
*/
	ids = pDM->SendFrame();
	if ( ids<0 )
	{
		AfxMessageBox(_T("삭제작업 중 에러가 발생하였습니다."));
		return;
	}
	ids = pDM->EndFrame();
	if ( ids<0 )
	{
		AfxMessageBox(_T("EndFrame을 확인하세요"));
		return;
	}


	AllControlDisplay(_T("CM_관리_분류기호_연동색인어_메인"));

	return;
EFS_END
}

VOID CMNG_NEW_INDEX::OnCodePre() 
{
	// TODO: Add your control notification handler code here
	INT idx;
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_관리_분류기호_연동색인어_입력"));

	idx = SelectGetPrev(_T("CM_관리_분류기호_연동색인어_메인"), _T("MAIN_GRID"));
	if (idx < 0 )
	{
		ESLAfxMessageBox(_T("처음 자료 입니다.."));
		SelectGetHeadPosition(_T("CM_관리_분류기호_연동색인어_메인"), _T("MAIN_GRID"));
		return;
	}

	m_INDEX = idx ;
	SetReverse(_T("CM_관리_분류기호_연동색인어_메인"), _T("MAIN_GRID"), m_INDEX);
	pCM->AllControlDisplay(m_INDEX);		
}

VOID CMNG_NEW_INDEX::OnCodeNext() 
{
	// TODO: Add your control notification handler code here
	INT idx;
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_관리_분류기호_연동색인어_입력"));

	idx = SelectGetNext(_T("CM_관리_분류기호_연동색인어_메인"), _T("MAIN_GRID"));
	if (idx < 0 )
	{
		ESLAfxMessageBox(_T("마지막 자료 입니다.."));
		SelectGetTailPosition(_T("CM_관리_분류기호_연동색인어_메인"), _T("MAIN_GRID"));
		return;
	}

	m_INDEX = idx ;
	SetReverse(_T("CM_관리_분류기호_연동색인어_메인"), _T("MAIN_GRID"), m_INDEX);
	pCM->AllControlDisplay(m_INDEX);		
}

HBRUSH CMNG_NEW_INDEX::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


