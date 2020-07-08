// MNG_NEW_KEYWORD.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_NEW_KEYWORD.h"
#include "MNG_NEW_RELATED.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INSERT_MODE 0
#define UPDATE_MODE 1

/////////////////////////////////////////////////////////////////////////////
// CMNG_NEW_KEYWORD dialog


CMNG_NEW_KEYWORD::CMNG_NEW_KEYWORD(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_NEW_KEYWORD)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CMNG_NEW_KEYWORD::~CMNG_NEW_KEYWORD()
{
	
}


BOOL CMNG_NEW_KEYWORD::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);
}

VOID CMNG_NEW_KEYWORD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_NEW_KEYWORD)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_NEW_KEYWORD, CDialog)
	//{{AFX_MSG_MAP(CMNG_NEW_KEYWORD)
	ON_BN_CLICKED(IDC_CODE_PRE, OnCodePre)
	ON_BN_CLICKED(IDC_CODE_NEXT, OnCodeNext)
	ON_BN_CLICKED(IDC_SAVE_CODE, OnSaveCode)
	ON_BN_CLICKED(IDC_RWORD_INSERT, OnRwordInsert)
	ON_BN_CLICKED(IDC_RWORD_UPDATE, OnRwordUpdate)
	ON_BN_CLICKED(IDC_RWORD_DETELE, OnRwordDetele)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_NEW_KEYWORD message handlers

BOOL CMNG_NEW_KEYWORD::OnInitDialog() 
{
	CDialog::OnInitDialog();


	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_관리_분류기호_주제명_입력")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	

	// UPDATE
	if( OpenMode == UPDATE_MODE )
	{

		SelectMakeList(_T("CM_관리_분류기호_주제명_메인"), _T("MAIN_GRID"));
		m_INDEX = SelectGetHeadPosition(_T("CM_관리_분류기호_주제명_메인"), _T("MAIN_GRID"));

		if(m_INDEX < 0)
		{
			//에러처리
			ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
			EndDialog(0);
			return FALSE;
		}	
	
		GetDlgItem(IDC_CODE_PRE)->ShowWindow(true);
		GetDlgItem(IDC_CODE_NEXT)->ShowWindow(true);

		CESL_DataMgr *pDM;
		pDM = FindDM(_T("DM_관리_분류기호_주제명_메인"));
		m_strSubjectKey=pDM->GetCellData(_T("REC_KEY"),m_INDEX);
		
		m_pDMFrame = new CESL_DataMgr();
		m_pDMFrame->SetCONNECTION_INFO(pDM->CONNECTION_INFO);// DM for Frame


		Load_Related(m_strSubjectKey);
		
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2004.06.08 김소정 수정
		SetReverse(_T("CM_관리_분류기호_주제명_메인"), _T("MAIN_GRID"), m_INDEX);
		AllControlDisplay(_T("CM_관리_분류기호_주제명_입력"), m_INDEX);

	}
	else	// INSERT
	{
		CESL_DataMgr *pDM;
		pDM = FindDM(_T("DM_관리_분류기호_주제명_메인"));
		pDM->MakeRecKey(m_strSubjectKey);
		
		//CESL_DataMgr *m_pDMFrame;
		m_pDMFrame = new CESL_DataMgr();
		m_pDMFrame->SetCONNECTION_INFO(pDM->CONNECTION_INFO);// DM for Frame
	}


	// Start Frame +++++++++++++++++++++++++++++++++++
	INT ids = -1;
	ids = m_pDMFrame->StartFrame();
	if ( ids<0 )
	{
		AfxMessageBox(_T("StartFrame을 확인하세요."));
		return FALSE;
	}
	// +++++++++++++++++++++++++++++++++++++++++++++++++


	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



VOID CMNG_NEW_KEYWORD::OnSaveCode() 
{
EFS_BEGIN
	if( OpenMode == INSERT_MODE )
	{
		if ( INSERT() <0 ) return;
		AfxMessageBox(_T("입력 하였습니다."));
	}
	else
	{
		if ( UPDATE() < 0 ) return;
		AfxMessageBox(_T("수정 하였습니다."));

	}
	
	// DM삭제
	if ( m_pDMFrame != NULL)
	{
		delete m_pDMFrame;
		m_pDMFrame= NULL;
	}

	if ( OpenMode == UPDATE_MODE ) CDialog::OnOK();


	//RealtedDataSave();

EFS_END
}
INT CMNG_NEW_KEYWORD::RealtedDataSave()
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2004.06.07 김소정 수정
//Frame을 Insert/Update와 하나로 합침
EFS_BEGIN

	CESL_DataMgr *pDM = FindDM(_T("DM_관리_분류기호_주제명_관련어"));
	//FIND REC_KEY INDEX
	INT nCount = pDM->GetRowCount();
	if(nCount == 0 ) return 0;

	CString WorkFlag,strQry;

//	pDM->StartFrame();
	for ( INT i = 0 ; i<nCount ; i++)
	{
		WorkFlag=pDM->GetCellData(_T("작업구분"),i);

		if(WorkFlag == _T("I"))
		{
			strQry=_T("INSERT INTO CO_RELATED_SUBJECT_NAME_TBL(REC_KEY,SUBJECT_NAME_KEY,RELATED_SUBJECT_NAME,RELATED_SUBJECT_CODE) VALUES(");
			strQry +=pDM->GetCellData(_T("REC_KEY"),i) + _T(",");
			strQry +=pDM->GetCellData(_T("주제명키"),i) + _T(",");
			strQry +=_T("'")+pDM->GetCellData(_T("관련어"),i) + _T("',");
			strQry +=_T("'")+pDM->GetCellData(_T("관련어코드"),i) + _T("');");
			
			//pDM->AddFrame(strQry);
			m_pDMFrame->AddFrame(strQry);

		}
		if(WorkFlag == _T("U"))
		{
			strQry=_T("UPDATE CO_RELATED_SUBJECT_NAME_TBL SET ");
			strQry +=_T(" RELATED_SUBJECT_NAME ='")+pDM->GetCellData(_T("관련어"),i) + _T("',");
			strQry +=_T(" RELATED_SUBJECT_CODE ='")+pDM->GetCellData(_T("관련어코드"),i)+_T("'");
			strQry +=_T(" WHERE REC_KEY=")+pDM->GetCellData(_T("REC_KEY"),i)+_T(";") ;
			//pDM->AddFrame(strQry);
			m_pDMFrame->AddFrame(strQry);

		}
		/*if(WorkFlag == _T("D"))	
		{
			strQry=_T("DELETE FROM CO_RELATED_SUBJECT_NAME_TBL ");
			strQry +=_T(" WHERE REC_KEY=")+pDM->GetCellData(_T("REC_KEY"),i)+_T(";") ;
			//pDM->AddFrame(strQry);
			m_pDMFrame->AddFrame(strQry);
		}*/
	}
	
	if ( pDM != NULL ) pDM = NULL;

	return 0;
EFS_END
	return -1;
}

INT CMNG_NEW_KEYWORD::INSERT()
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2004.06.07 김소정 수정
EFS_BEGIN
	CString strName, strDESC, strRecKey;
	INT ids=-1;


	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_관리_분류기호_주제명_메인"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM 초기화 에러"));
		return -1;
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_관리_분류기호_주제명_입력"));
	if ( NULL == pCM ) 
	{
		AfxMessageBox(_T("CM 초기화 에러"));
		return -1;
	}
	
	// get data from CM
	GetControlData( _T("CM_관리_분류기호_주제명_입력"), _T("주제명"), strName );
	GetControlData( _T("CM_관리_분류기호_주제명_입력"), _T("주제명본문"), strDESC );
	strRecKey = pDM->GetCellData(_T("REC_KEY"), m_INDEX);

	strRecKey = m_strSubjectKey;

	INT n_index = pDM->GetRowCount();
	
	// set Date To DM
	pDM->InsertRow(-1);
	pDM->SetCellData(_T("REC_KEY"), strRecKey, n_index);
	pDM->SetCellData(_T("주제명"), strName, n_index);
	pDM->SetCellData(_T("주제명본문"), strDESC, n_index);
	// 수정일자
	CTime t = CTime::GetCurrentTime();
	CString sDate;
	sDate.Format ( _T("%4d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay() );
	pDM->SetCellData(_T("수정일자"), sDate, n_index);


	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, n_index, m_pDMFrame);
	

	// 관련어
	ids = RealtedDataSave();
	if ( ids<0 )
	{
		AfxMessageBox(_T("수정사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return -1;
	}

	ids = m_pDMFrame->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return -1;
	}

	ids = m_pDMFrame->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame 를 확인하세요"));
		return -1;
	}

	AllControlDisplay(_T("CM_관리_분류기호_주제명_메인"), n_index);


	return 0;
EFS_END
	return -1;
}


INT CMNG_NEW_KEYWORD::UPDATE()
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//2004.06.07 김소정 수정
EFS_BEGIN
	CString strName, strDESC, strRecKey;
	INT ids = -1;

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_관리_분류기호_주제명_메인"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM 초기화 에러"));
		return -1;
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_관리_분류기호_주제명_입력"));
	if ( NULL == pCM ) 
	{
		AfxMessageBox(_T("CM 초기화 에러"));
		return -1;
	}

	// get data from CM
	GetControlData( _T("CM_관리_분류기호_주제명_입력"), _T("주제명"), strName );
	GetControlData( _T("CM_관리_분류기호_주제명_입력"), _T("주제명본문"), strDESC );
	strRecKey = pDM->GetCellData(_T("REC_KEY"), m_INDEX);

	// set Date To DM
	pDM->SetCellData(_T("주제명"), strName, m_INDEX);
	pDM->SetCellData(_T("주제명본문"), strDESC, m_INDEX);
	// 수정일자
	CTime t = CTime::GetCurrentTime();
	CString sDate;
	sDate.Format ( _T("%4d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay() );
	pDM->SetCellData(_T("수정일자"), sDate, m_INDEX);

	// Start Frame ++++++++++++++++++++++++
	/*ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 을 확인하세요"));
		return -1;
	}*/
	
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, pDM->TBL_NAME, m_INDEX, m_pDMFrame, _T("REC_KEY"), _T("NUMERIC"), strRecKey);
	
	// 관련어
	ids = RealtedDataSave();
	if ( ids<0 )
	{
		AfxMessageBox(_T("수정사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return -1;
	}

	ids = m_pDMFrame->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("수정사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return -1;
	}

	ids = m_pDMFrame->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame 를 확인하세요"));
		return -1;
	}
	// End Frame ++++++++++++++++++++++++

	AllControlDisplay(_T("CM_관리_분류기호_주제명_메인"), m_INDEX);

	return 0;
EFS_END
	return -1;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


}
VOID CMNG_NEW_KEYWORD::DELETE_CODE()
{
EFS_BEGIN
	INT idx;
	SelectMakeList(_T("CM_관리_분류기호_주제명_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_관리_분류기호_주제명_메인"), _T("MAIN_GRID"));

	if(idx < 0)
	{
		//에러처리
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}	

	if(IDCANCEL == ESLAfxMessageBox(_T("삭제 작업을 하시겠습니까?"), MB_OKCANCEL))
		return;

	CESL_DataMgr *pDM = FindDM(_T("DM_관리_분류기호_주제명_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_관리_분류기호_주제명_메인") );
		return ;
	}

	INT ids = -1;

	ids = pDM->StartFrame();
	if ( ids<0 )
	{
		AfxMessageBox(_T("StartFrame을 확인하세요."));
		return;
	}

	INT delete_Count=0;
	CString strRecKey,strQry;
	strRecKey = pDM->GetCellData( _T("REC_KEY") , idx );

	strQry.Format(_T("DELETE FROM %s WHERE REC_KEY = %s;"), pDM->TBL_NAME ,strRecKey);
	pDM->AddFrame(strQry);
	strQry.Format(_T("DELETE FROM %s WHERE SUBJECT_NAME_KEY = %s;"), _T("CO_RELATED_SUBJECT_NAME_TBL") ,strRecKey);
	pDM->AddFrame(strQry);
	pDM->DeleteRow(idx);
	delete_Count++;

	idx = SelectGetNext(_T("CM_관리_분류기호_주제명_메인"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strRecKey = pDM->GetCellData( _T("REC_KEY") , idx - delete_Count);

		strQry.Format(_T("DELETE FROM %s WHERE REC_KEY = %s;"), pDM->TBL_NAME ,strRecKey);
		pDM->AddFrame(strQry);
		strQry.Format(_T("DELETE FROM %s WHERE SUBJECT_NAME_KEY = %s;"), _T("CO_RELATED_SUBJECT_NAME_TBL") ,strRecKey);
		pDM->AddFrame(strQry);

		pDM->DeleteRow(idx);
		delete_Count++;

		idx = SelectGetNext(_T("CM_관리_분류기호_주제명_메인"), _T("MAIN_GRID"));
	}

	ids = pDM->SendFrame();
	if ( ids<0 )
	{
		AfxMessageBox(_T("삭제작업 중 에러가 발생하였습니다."));
		return;
	}

	ids = pDM->EndFrame();
	if ( ids<0 )
	{
		AfxMessageBox(_T("EndFrame을 확인하세요."));
		return;
	}

	AllControlDisplay(_T("CM_관리_분류기호_주제명_메인"));
	AfxMessageBox(_T("삭제하였습니다."));
	
	return;
EFS_END
}


VOID CMNG_NEW_KEYWORD::OnCodePre() 
{
EFS_BEGIN
	
	INT idx;

	idx = SelectGetPrev(_T("CM_관리_분류기호_주제명_메인"), _T("MAIN_GRID"));
	if (idx < 0 )
	{
		ESLAfxMessageBox(_T("처음 자료 입니다.."));
		SelectGetHeadPosition(_T("CM_관리_분류기호_주제명_메인"), _T("MAIN_GRID"));
		return;
	}

	m_INDEX = idx ;
	
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_관리_분류기호_주제명_메인"));
	m_strSubjectKey=pDM->GetCellData(_T("REC_KEY"),m_INDEX);

	Load_Related(m_strSubjectKey);

	SetReverse(_T("CM_관리_분류기호_주제명_메인"), _T("MAIN_GRID"), m_INDEX);
	AllControlDisplay(_T("CM_관리_분류기호_주제명_입력"), m_INDEX);

EFS_END
}

VOID CMNG_NEW_KEYWORD::OnCodeNext() 
{
EFS_BEGIN
	
	INT idx;

	idx = SelectGetNext(_T("CM_관리_분류기호_주제명_메인"), _T("MAIN_GRID"));
	if (idx < 0 )
	{
		ESLAfxMessageBox(_T("마지막 자료 입니다.."));
		SelectGetTailPosition(_T("CM_관리_분류기호_주제명_메인"), _T("MAIN_GRID"));
		return;
	}

	m_INDEX = idx ;
	

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_관리_분류기호_주제명_메인"));
	m_strSubjectKey=pDM->GetCellData(_T("REC_KEY"),m_INDEX);

	Load_Related(m_strSubjectKey);

	SetReverse(_T("CM_관리_분류기호_주제명_메인"), _T("MAIN_GRID"), m_INDEX);
	AllControlDisplay(_T("CM_관리_분류기호_주제명_입력"), m_INDEX);


EFS_END
}


VOID CMNG_NEW_KEYWORD::Load_Related(CString strQry)
{
EFS_BEGIN

	CString Qry;
	Qry.Format(_T(" SUBJECT_NAME_KEY = %s "),strQry);
	CESL_DataMgr *pDM = FindDM(_T("DM_관리_분류기호_주제명_관련어"));
	pDM->RefreshDataManager(Qry);

EFS_END
}
	
/// 관련어 입력
VOID CMNG_NEW_KEYWORD::OnRwordInsert() 
{
EFS_BEGIN
	
	CMNG_NEW_RELATED Dlg(this);
	Dlg.OpenMode = 0;
	Dlg.strSubjectKey  = m_strSubjectKey;
	Dlg.DoModal();

	//AllControlDisplay(_T("CM_관리_분류기호_주제명_입력"));

EFS_END
}

/// 관련어 수정
VOID CMNG_NEW_KEYWORD::OnRwordUpdate() 
{
EFS_BEGIN
	INT n_idx = -1;
	SelectMakeList(_T("CM_관리_분류기호_주제명_입력"), _T("RELATED_GRID"));
	n_idx = SelectGetHeadPosition(_T("CM_관리_분류기호_주제명_입력"), _T("RELATED_GRID"));

	if(n_idx < 0)
	{
		//에러처리
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}

	/*CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_관리_분류기호_주제명_입력"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox(_T("DM 초기화 에러"));
		return ;
	}

	CString strTitle, strCode, strSubjectKey, strRecKey;

	strRecKey = pDM->GetCellData(_T("REC_KEY"), n_idx);
	strTitle = pDM->GetCellData(_T("관련어"), n_idx);
	strCode = pDM->GetCellData(_T("관련어코드"), n_idx);
	strSubjectKey = pDM->GetCellData(_T("주제명키"), n_idx);;
*/
	CMNG_NEW_RELATED Dlg(this);
	Dlg.OpenMode = 1;
//	Dlg.m_TITLE=strTitle;
//	Dlg.m_strCode=strCode;
//	Dlg.strSubjectKey  = strSubjectKey;
//	Dlg.m_RECKEY  = strRecKey;
	Dlg.DoModal();
	
	//AllControlDisplay(_T("CM_관리_분류기호_주제명_입력"));

EFS_END
}

/// 관련어 삭제
VOID CMNG_NEW_KEYWORD::OnRwordDetele() 
{
EFS_BEGIN
	INT n_idx = -1;
	SelectMakeList(_T("CM_관리_분류기호_주제명_입력"), _T("RELATED_GRID"));
	n_idx = SelectGetHeadPosition(_T("CM_관리_분류기호_주제명_입력"), _T("RELATED_GRID"));

	if(n_idx < 0)
	{
		//에러처리
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	
	CESL_DataMgr *pDM = FindDM(_T("DM_관리_분류기호_주제명_관련어"));
	if ( NULL == pDM )
	{
		AfxMessageBox(_T("DM 초기화 에러."));
		return ;
	}

	CESL_Grid* p_grid;
	p_grid = (CESL_Grid*)FindControl(_T("CM_관리_분류기호_주제명_입력"), _T("RELATED_GRID"));
	if ( p_grid == NULL )
	{
		AfxMessageBox(_T("GRID 초기화 에러"));
		return ;
	}
	
	//FIND REC_KEY INDEX
	INT nSelectCount = 0;
	nSelectCount = p_grid->SelectGetCount();
	if ( nSelectCount <= 0 ) return ;

	CString strRecKey;
	INT ids=-1;

	for ( INT i=0; i<nSelectCount; i++ )
	{
		strRecKey = pDM->GetCellData(_T("REC_KEY"), n_idx);
		
		// Add Frame
		m_pDMFrame->MakeDeleteFrame(pDM->TBL_NAME, _T("REC_KEY"), _T("STRING"), strRecKey);

		ids = p_grid->RemoveAt(n_idx);
		if ( ids<0 ) return;

		ids = pDM->DeleteRow(n_idx);
		if ( ids<0 ) return;

		// 삭제된 수만큼 그리드 row 수도 삭제됨.
		n_idx = p_grid->SelectGetNext() - (i+1);

	}// end of for
	

EFS_END
}

BEGIN_EVENTSINK_MAP(CMNG_NEW_KEYWORD, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_NEW_KEYWORD)
//	ON_EVENT(CMNG_NEW_KEYWORD, IDC_REALATED_GRID, -600 /* Click */, OnClickRealatedGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


VOID CMNG_NEW_KEYWORD::OnCancel() 
{
EFS_BEGIN
	// End Frame
	// DM삭제
	if ( m_pDMFrame != NULL)
	{
		m_pDMFrame->EndFrame();
		delete m_pDMFrame;
		m_pDMFrame= NULL;
	}
	CDialog::OnCancel();

EFS_END
}

HBRUSH CMNG_NEW_KEYWORD::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
