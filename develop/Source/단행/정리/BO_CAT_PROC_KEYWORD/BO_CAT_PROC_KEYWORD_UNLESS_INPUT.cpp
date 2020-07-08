// BO_CAT_PROC_KEYWORD_UNLESS_INPUT.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_PROC_KEYWORD_UNLESS_INPUT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_KEYWORD_UNLESS_INPUT dialog


CBO_CAT_PROC_KEYWORD_UNLESS_INPUT::CBO_CAT_PROC_KEYWORD_UNLESS_INPUT(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_KEYWORD_UNLESS_INPUT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_KEYWORD_UNLESS_INPUT::~CBO_CAT_PROC_KEYWORD_UNLESS_INPUT()
{
}

VOID CBO_CAT_PROC_KEYWORD_UNLESS_INPUT::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_KEYWORD_UNLESS_INPUT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_KEYWORD_UNLESS_INPUT, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_KEYWORD_UNLESS_INPUT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_KEYWORD_UNLESS_INPUT message handlers

BOOL CBO_CAT_PROC_KEYWORD_UNLESS_INPUT::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_KEYWORD_UNLESS_INPUT::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_UNLESS_WORD_0200")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_KEYWORD_UNLESS_INPUT::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here
	CString sWord;
	GetControlData( _T("CM_BO_CAT_UNLESS_WORD_0200"), _T("불용어"), sWord );
	if ( sWord.IsEmpty() ) {
		AfxMessageBox( _T("불용어를 입력하세요.") );
		return;
	}
	
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_UNLESS_WORD_0000"));
	if (!pDM) return;

	INT m_idx;
	m_idx = pDM->GetRowCount();

	//불용어 중복체크
	for ( INT i = 0 ; i < m_idx ; i++ ) {
		CString sDmWord = pDM->GetCellData( _T("불용어"), i );
		if ( sDmWord == sWord ) {
			AfxMessageBox( _T("이미 입력한 불용어입니다.") );
			return;
		}
	}

	//rec_key 만들기
	CString sRecKey;
	//pDM->MakeRecKey( sRecKey, table_name=_T(""), _T("CO_ACCESSION_REC_TBL"), _T("REC_KEY"));
	pDM->MakeRecKey( sRecKey );
	INT ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return;
	}


	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sRecKey);
	pDM->AddDBFieldData(_T("UNLESS_WORD"), _T("STRING"), sWord );

	ids = pDM->MakeInsertFrame(_T("CO_UNLESS_WORD_TBL"));
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("MakeInsertFrame 를 확인하세요"));
		return;
	}

	ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return;
	}
	ids = pDM->EndFrame();	
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame 를 확인하세요"));
		return;
	}
	AfxMessageBox(_T("저장완료"));

	/*
	CESL_ControlMgr *pCM1 = FindCM(_T("CM_BO_CAT_UNLESS_WORD_0000"));
	AllControlDisplay(_T("DM_BO_CAT_UNLESS_WORD_0000"));	
	pCM1->AllControlDisplay(m_idx);
	*/
	pDM->SetCellData( _T("불용어"), sWord, m_idx+1 );

//	INT ids;
	ids = AllControlDisplay(_T("CM_BO_CAT_PROC_KEYWORD_UNLESS"));

	CDialog::OnOK();

EFS_END
}

HBRUSH CBO_CAT_PROC_KEYWORD_UNLESS_INPUT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}