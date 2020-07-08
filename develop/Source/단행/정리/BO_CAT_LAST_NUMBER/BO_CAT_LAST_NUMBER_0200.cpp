// BO_CAT_LAST_NUMBER_0200.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_LAST_NUMBER_0200.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_LAST_NUMBER_0200 dialog


CBO_CAT_LAST_NUMBER_0200::CBO_CAT_LAST_NUMBER_0200(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_LAST_NUMBER_0200)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_LAST_NUMBER_0200::~CBO_CAT_LAST_NUMBER_0200()
{
}

VOID CBO_CAT_LAST_NUMBER_0200::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_LAST_NUMBER_0200)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_LAST_NUMBER_0200, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_LAST_NUMBER_0200)
	ON_BN_CLICKED(IDC_bIDOK, OnbIDOK)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_LAST_NUMBER_0200 message handlers

BOOL CBO_CAT_LAST_NUMBER_0200::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_LAST_NUMBER_0200::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_LAST_NUMBER_0200")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	Display(0);
	if ( m_idx < 0 ) {
		EndDialog(IDCANCEL);
		m_bInitDlg = FALSE;		
		return FALSE;
	}

	//CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_LAST_NUMBER"));
	if ( pDM == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_LAST_NUMBER ERROR") );
		m_bInitDlg = FALSE;
		return FALSE;
	}
	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );

	CString sWord, sHanjaWord;
	sWord = pDM->GetCellData( _T("번호"), m_idx );
	sHanjaWord = pDM->GetCellData( _T("년도"), m_idx );
	SetControlData( _T("CM_BO_CAT_LAST_NUMBER_0200"), _T("년도"), sHanjaWord );
	SetControlData( _T("CM_BO_CAT_LAST_NUMBER_0200"), _T("번호"), sWord );

	if ( sRecKey.IsEmpty() ) {
		m_bInitDlg = FALSE;		
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_LAST_NUMBER_0200::OnbIDOK() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	INT ids;
    CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_LAST_NUMBER"));	
	if (!pDM) return;
		
	///수정 작업 시작 
	CString strID;
	//pCESLMgr->GetControlData(_T("CM_BO_CAT_REG_0520"), _T("등록작업자"), strID);
	strID = pDM->GetCellData ( _T("REC_KEY") ,m_idx );

	ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return;
	}

	
	///수정 작업 시작  
	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_CAT_LAST_NUMBER_0200"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("FindCM error") );
		return;
	}
	ids = pDM->MakeControlMgrFiledData((CObject*)pCM);
	ids = pDM->MakeControlMgrUpdateFrame(_T("REC_KEY"), _T("STRING"), strID, m_idx);
	//ids = pDM->MakeControlMgrUpdateFrame(_T("REGISTRANT"), _T("STRING"), strID, idx);
	//ids = pDM->MakeControlMgrUpdateFrame(_T("등록작업자"), _T("STRING"), strID);
   	///수정 작업 완료 


	ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("수정사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return;
	}
	else {
		AfxMessageBox(_T("저장하였습니다."));
	}
  
	//ids = pDM->EndFrame();
	/*
	CBO_CAT_REG_0520 * pREG_0520;
	INT tmp = pREG_0520->m
	*/
	
	//pCESLMgr->AllControlDisplay(_T("DM_BO_CAT_REG_0500"));
	CESL_ControlMgr *pCM1 = FindCM(_T("CM_BO_CAT_LAST_NUMBER"));
	AllControlDisplay(_T("CM_BO_CAT_LAST_NUMBER"));	
	pCM1->AllControlDisplay(m_idx);
 
	//DestroyWindow();

EFS_END
}

VOID CBO_CAT_LAST_NUMBER_0200::Display(INT nDir)
{
EFS_BEGIN

	INT idx;
	switch(nDir)
	{
	case 0:
		SelectMakeList(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER"));
		idx = SelectGetHeadPosition(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER"));
		if (idx < 0)
		{
			AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
			//EndDialog(IDCANCEL);
			return;
		}
		break;
	case -1:
		idx = SelectGetPrev(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER"));
		if (idx == -1) {
			idx = SelectGetHeadPosition(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER"));
			AfxMessageBox(_T("처음 자료입니다."));
			return;
		}
		break;
	case 1:
		idx = SelectGetNext(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER"));
		if (idx == -1) {
			idx = SelectGetTailPosition(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER"));
			AfxMessageBox(_T("마지막 자료입니다."));
			return;
		}
		break;
	}
	
	m_idx = idx; 
	//AllControlDisplay(_T("CM_BO_CAT_LAST_NUMBER"), m_idx);
	SetReverse(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER"), m_idx);

EFS_END
}

VOID CBO_CAT_LAST_NUMBER_0200::OnbPREV() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	Display(-1);		
	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );
/*
	CString sWord, sHanjaWord;
	sWord = pDM->GetCellData( _T("한글단어"), m_idx );
	sHanjaWord = pDM->GetCellData( _T("한자단어"), m_idx );

	SetControlData( _T("CM_BO_CAT_MNG_WORD_0300"), _T("한글단어"), sWord );
	SetControlData( _T("CM_BO_CAT_MNG_WORD_0300"), _T("한자단어"), sHanjaWord );
	*/

EFS_END
}

VOID CBO_CAT_LAST_NUMBER_0200::OnbNEXT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	Display(1);
	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );

EFS_END
}

HBRUSH CBO_CAT_LAST_NUMBER_0200::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
