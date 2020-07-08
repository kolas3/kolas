// BO_CAT_REG_NUMBER_0200.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_REG_NUMBER_0200.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_NUMBER_0200 dialog


CBO_CAT_REG_NUMBER_0200::CBO_CAT_REG_NUMBER_0200(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_NUMBER_0200)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_REG_NUMBER_0200::~CBO_CAT_REG_NUMBER_0200()
{
}

VOID CBO_CAT_REG_NUMBER_0200::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_NUMBER_0200)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_NUMBER_0200, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_NUMBER_0200)
	ON_BN_CLICKED(IDC_bIDOK, OnbIDOK)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_NUMBER_0200 message handlers

BOOL CBO_CAT_REG_NUMBER_0200::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_REG_NUMBER_0200")) < 0)
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

	CESL_DataMgr *pDM_REG;
	pDM_REG = FindDM(_T("DM_BO_CAT_REG_NUMBER"));
	if ( pDM_REG == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_REG_NUMBER ERROR") );
		m_bInitDlg = FALSE;
		return FALSE;
	}
	sRecKey = pDM_REG->GetCellData( _T("REC_KEY") , m_idx );

	CString sWord, sHanjaWord;
	sWord = pDM_REG->GetCellData( _T("구분값"), m_idx );
	sHanjaWord = pDM_REG->GetCellData( _T("번호"), m_idx );
	SetControlData( _T("CM_BO_CAT_REG_NUMBER_0200"), _T("구분값"), sWord );
	SetControlData( _T("CM_BO_CAT_REG_NUMBER_0200"), _T("번호"), sHanjaWord );

	if ( sRecKey.IsEmpty() ) {
		AfxMessageBox( _T("REC_KEY가 없습니다.") );
		EndDialog(IDCANCEL);
		m_bInitDlg = FALSE;		
		return FALSE;
	}

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분코드를 6자리 이하로 제한
	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_eRegNumberCode);
	pEdit->SetLimitText(6);
	//=====================================================

// 	m_Dlg_brush.CreateSolidBrush(RGB(239, 239, 239));

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_REG_NUMBER_0200::Display(INT nDir)
{
EFS_BEGIN

	INT idx;
	switch(nDir)
	{
	case 0:
		SelectMakeList(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));
		idx = SelectGetHeadPosition(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));
		if (idx < 0)
		{
			AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
			//EndDialog(IDCANCEL);
			return;
		}
		break;
	case -1:
		idx = SelectGetPrev(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));
		if (idx == -1) {
			idx = SelectGetHeadPosition(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));
			AfxMessageBox(_T("처음 자료입니다."));
			return;
		}
		break;
	case 1:
		idx = SelectGetNext(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));
		if (idx == -1) {
			idx = SelectGetTailPosition(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"));
			AfxMessageBox(_T("마지막 자료입니다."));
			return;
		}
		break;
	}
	
	m_idx = idx; 
	//AllControlDisplay(_T("CM_BO_CAT_LAST_NUMBER"), m_idx);
	SetReverse(_T("CM_BO_CAT_LAST_NUMBER"), _T("GRD_LAST_NUMBER2"), m_idx);

EFS_END
}

VOID CBO_CAT_REG_NUMBER_0200::OnbIDOK() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	INT ids;
    CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_NUMBER"));	
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
	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_CAT_REG_NUMBER_0200"));
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
  
	/*
	CBO_CAT_REG_0520 * pREG_0520;
	INT tmp = pREG_0520->m
	*/
	
	//pCESLMgr->AllControlDisplay(_T("DM_BO_CAT_REG_0500"));
	CESL_ControlMgr *pCM1 = FindCM(_T("CM_BO_CAT_LAST_NUMBER"));
	AllControlDisplay(_T("DM_BO_CAT_REG_NUMBER"));	
	pCM1->AllControlDisplay(m_idx);

EFS_END
}

HBRUSH CBO_CAT_REG_NUMBER_0200::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
