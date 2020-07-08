// BO_CAT_HANJA_WORD_0200.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_HANJA_WORD_0200.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_HANJA_WORD_0200 dialog


CBO_CAT_HANJA_WORD_0200::CBO_CAT_HANJA_WORD_0200(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_HANJA_WORD_0200)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_HANJA_WORD_0200::~CBO_CAT_HANJA_WORD_0200()
{
}

VOID CBO_CAT_HANJA_WORD_0200::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_HANJA_WORD_0200)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_HANJA_WORD_0200, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_HANJA_WORD_0200)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_HANJA_WORD_0200 message handlers

BOOL CBO_CAT_HANJA_WORD_0200::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}
VOID CBO_CAT_HANJA_WORD_0200::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID);
	if (pGrid->GetSafeHwnd() == NULL) return;

//	CButton *pButton = (CButton*)GetDlgItem(IDC_bDELETE);
//	if (pButton->GetSafeHwnd() == NULL) return;
	
//	pButton->SetWindowPos(NULL, cx/2, 0, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	pGrid->MoveWindow(0, 0, cx, cy);
	

EFS_END
}

BOOL CBO_CAT_HANJA_WORD_0200::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_MNG_HANJA_0200")) < 0)
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

VOID CBO_CAT_HANJA_WORD_0200::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here
	CString sWord, sHanjaWord;
	GetControlData( _T("CM_BO_CAT_MNG_HANJA_0200"), _T("한글단어"), sWord );
	GetControlData( _T("CM_BO_CAT_MNG_HANJA_0200"), _T("한자단어"), sHanjaWord );
	if ( sWord.IsEmpty() || sHanjaWord.IsEmpty() ) {
		AfxMessageBox( _T("단어를 입력하세요.") );
		return;
	}
	
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_MNG_HANJA"));
	if (!pDM) return;

	INT m_idx;
	m_idx = pDM->GetRowCount();

	//조사 중복체크
	for ( INT i = 0 ; i < m_idx ; i++ ) {
		CString sDmWord = pDM->GetCellData( _T("한자단어"), i );
		if ( sDmWord == sHanjaWord ) {
			AfxMessageBox( _T("이미 입력한 한자단어 입니다.") );
			return;
		}
	}

	//rec_key 만들기
	CString sRecKey;
	//pDM->MakeRecKey( sRecKey, table_name=_T(""), _T("CO_ACCESSION_REC_TBL"), _T("REC_KEY"));
	pDM->MakeRecKey( sRecKey );
	pDM->StartFrame();

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sRecKey);
	pDM->AddDBFieldData(_T("KOR_LANG"), _T("STRING"), sWord );
	pDM->AddDBFieldData(_T("CHI_LANG"), _T("STRING"), sHanjaWord );

	pDM->MakeInsertFrame(_T("CO_HANJA_WORD_TBL"));

	INT ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return;
	}
	pDM->EndFrame();	
	AfxMessageBox(_T("저장완료"));

	pDM->InsertRow(-1);
	pDM->SetCellData( _T("한글단어"), sWord, m_idx );
	pDM->SetCellData( _T("한자단어"), sHanjaWord, m_idx );

	/*
	CESL_ControlMgr *pCM1 = FindCM(_T("CM_BO_CAT_UNLESS_WORD_0000"));
	AllControlDisplay(_T("DM_BO_CAT_UNLESS_WORD_0000"));	
	pCM1->AllControlDisplay(m_idx);
	*/
	/*
	CString sTmp = pDM->GetCellData( _T("단어"), m_idx );
	sTmp = pDM->GetCellData( _T("조사"), m_idx+1 );
	pDM->SetCellData( _T("조사"), sWord, m_idx+1 );
	*/

	//AllControlDisplay(_T("CM_BO_CAT_AUXILIARY"));

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_MNG_HANJA"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_MNG_HANJA error") );
		return;
	}
	pCM->AllControlDisplay();

	//DestroyWindow();
	CDialog::OnOK();

EFS_END
}

HBRUSH CBO_CAT_HANJA_WORD_0200::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
