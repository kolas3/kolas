// BO_CAT_UNLESS_WORD_0200.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_UNLESS_WORD_0200.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_UNLESS_WORD_0200 dialog


CBO_CAT_UNLESS_WORD_0200::CBO_CAT_UNLESS_WORD_0200(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_UNLESS_WORD_0200)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_UNLESS_WORD_0200::~CBO_CAT_UNLESS_WORD_0200()
{
}

BOOL CBO_CAT_UNLESS_WORD_0200::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_UNLESS_WORD_0200::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_UNLESS_WORD_0200)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_UNLESS_WORD_0200, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_UNLESS_WORD_0200)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_UNLESS_WORD_0200 message handlers

BOOL CBO_CAT_UNLESS_WORD_0200::OnInitDialog() 
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

VOID CBO_CAT_UNLESS_WORD_0200::OnOK() 
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
	pDM->StartFrame();

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sRecKey);
	pDM->AddDBFieldData(_T("UNLESS_WORD"), _T("STRING"), sWord );

	pDM->MakeInsertFrame(_T("CO_UNLESS_WORD_TBL"));

	INT ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return;
	}
	pDM->EndFrame();	
	AfxMessageBox(_T("저장완료"));

	pDM->InsertRow(-1);
	pDM->SetCellData( _T("불용어"), sWord, m_idx );

	/*
	CESL_ControlMgr *pCM1 = FindCM(_T("CM_BO_CAT_UNLESS_WORD_0000"));
	AllControlDisplay(_T("DM_BO_CAT_UNLESS_WORD_0000"));	
	pCM1->AllControlDisplay(m_idx);
	*/
	pDM->SetCellData( _T("불용어"), sWord, m_idx+1 );

	AllControlDisplay(_T("CM_BO_CAT_UNLESS_WORD_0000"));

	//DestroyWindow();
	CDialog::OnOK();

EFS_END
}

INT CBO_CAT_UNLESS_WORD_0200::Delete()
{
EFS_BEGIN

	//CESL_DataMgr *pCM;
	CESL_ControlMgr *pCM;
	pCM = FindCM( _T("CM_BO_CAT_UNLESS_WORD_0000") );
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_UNLESS_WORD_0000 ERROR") );
		return -1;
	}

	SelectMakeList(_T("CM_BO_CAT_UNLESS_WORD_0000"), _T("GRD_UNLESS_WORD"));
	INT idx = SelectGetTailPosition(_T("CM_BO_CAT_UNLESS_WORD_0000"), _T("GRD_UNLESS_WORD"));
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
		return -1;
	}

	if (MessageBox(_T("선정한 자료를 삭제하시겠습니까?"), _T("삭제 확인"), MB_YESNO) == IDYES)
	{
		CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_UNLESS_WORD_0000"));
		pDM->StartFrame();

		CString sAccRecKey, sAccRecKeyTmp;
		do
		{
			CString strCondition;
			pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

			sAccRecKeyTmp += _T(",") + strCondition;		//키 조합

			pDM->MakeDeleteFrame(_T("CO_UNLESS_WORD_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
			pDM->DeleteRow(idx);

			idx = SelectGetPrev(_T("CM_BO_CAT_UNLESS_WORD_0000"), _T("GRD_UNLESS_WORD"));			
		}		
		while (idx > 0);

		sAccRecKey = sAccRecKeyTmp.Mid(1);
		CString sQuery;
		sQuery.Format( _T("DELETE FROM CO_UNLESS_WORD WHERE REC_KEY IN (%s);") , sAccRecKey );
		INT iCh = pDM->AddFrame(sQuery);

		INT ids = pDM->SendFrame();
		if (ids < 0) {
			AfxMessageBox(_T("삭제사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
			return -1;
		}
		pDM->EndFrame();
	}
	AllControlDisplay(_T("CM_BO_CAT_UNLESS_WORD_0000"));

	return 0;

EFS_END
return -1;

}
HBRUSH CBO_CAT_UNLESS_WORD_0200::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
