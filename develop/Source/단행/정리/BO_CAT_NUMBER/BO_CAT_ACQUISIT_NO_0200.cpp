// BO_CAT_ACQUISIT_NO_0200.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_ACQUISIT_NO_0200.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_ACQUISIT_NO_0200 dialog


CBO_CAT_ACQUISIT_NO_0200::CBO_CAT_ACQUISIT_NO_0200(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_ACQUISIT_NO_0200)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_ACQUISIT_NO_0200::~CBO_CAT_ACQUISIT_NO_0200()
{
}

VOID CBO_CAT_ACQUISIT_NO_0200::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_ACQUISIT_NO_0200)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_ACQUISIT_NO_0200, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_ACQUISIT_NO_0200)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_ACQUISIT_NO_0200 message handlers

BOOL CBO_CAT_ACQUISIT_NO_0200::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	m_sParentDMAlias = _T("");
	m_sParentGridAlias = _T("");
	m_pParentDM = NULL;

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_ACQUISIT_NO_0200::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_ACQUISIT_NO_0200")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CString sWorkTbl;
	GetControlData( _T("CM_BO_CAT_ACQUISIT_NO") , _T("작업구분"), sWorkTbl );
	if ( sWorkTbl == _T("마지막수입순번호관리") ) {
		m_sParentDMAlias = _T("DM_BO_CAT_ACQUISIT_NO");
		m_sParentGridAlias = _T("GRD_ACQUISIT_NO");
	}
	else if ( sWorkTbl == _T("누락수입순번호관리") ) {
		m_sParentDMAlias = _T("DM_BO_CAT_OMIT_NO");
		m_sParentGridAlias = _T("GRD_ACQUISIT_NO2");
	}

	m_pParentDM = FindDM( m_sParentDMAlias );
	if ( m_pParentDM == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_ACQUISIT_NO ERROR") );
		return FALSE;
	}

	Display(0);
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  

EFS_END
return FALSE;

}

VOID CBO_CAT_ACQUISIT_NO_0200::Display(INT nDir)
{
EFS_BEGIN

	INT idx;
	switch(nDir)
	{
	case 0:
		SelectMakeList(_T("CM_BO_CAT_ACQUISIT_NO"), m_sParentGridAlias );
		idx = SelectGetHeadPosition(_T("CM_BO_CAT_ACQUISIT_NO"), m_sParentGridAlias );
		if (idx < 0)
		{
			AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
			EndDialog(IDCANCEL);
			return;
		}
		break;
	case -1:
		idx = SelectGetPrev(_T("CM_BO_CAT_ACQUISIT_NO"), m_sParentGridAlias );
		if (idx == -1) {
			idx = SelectGetHeadPosition(_T("CM_BO_CAT_ACQUISIT_NO"), m_sParentGridAlias );
			AfxMessageBox(_T("처음 자료입니다."));
			return;
		}
		break;
	case 1:
		idx = SelectGetNext(_T("CM_BO_CAT_ACQUISIT_NO"), m_sParentGridAlias );
		if (idx == -1) {
			idx = SelectGetTailPosition(_T("CM_BO_CAT_ACQUISIT_NO"), m_sParentGridAlias );
			AfxMessageBox(_T("마지막 자료입니다."));
			return;
		}
		break;
	}
	
	m_idx = idx; 
	AllControlDisplay(_T("CM_BO_CAT_ACQUISIT_NO_0200"), m_idx);
	SetReverse(_T("CM_BO_CAT_ACQUISIT_NO"), m_sParentGridAlias, m_idx);
	m_sRecKey = m_pParentDM->GetCellData( _T("REC_KEY") , m_idx );

	CString sClassNo = m_pParentDM->GetCellData( _T("분류기호"), m_idx );
	CString sAcqNo = m_pParentDM->GetCellData( _T("수입순번호"), m_idx );
	SetControlData( _T("CM_BO_CAT_ACQUISIT_NO_0200"), _T("분류기호"), sClassNo );
	SetControlData( _T("CM_BO_CAT_ACQUISIT_NO_0200"), _T("수입순번호"), sAcqNo );

EFS_END
}

VOID CBO_CAT_ACQUISIT_NO_0200::OnbPREV() 
{
EFS_BEGIN

	Display(-1);		

EFS_END
}

VOID CBO_CAT_ACQUISIT_NO_0200::OnbNEXT() 
{
EFS_BEGIN

	Display(1);		

EFS_END
}

VOID CBO_CAT_ACQUISIT_NO_0200::OnOK() 
{
EFS_BEGIN

	INT ids;

	// 수정 작업 시작 
	ids = m_pParentDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return;
	}

	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_CAT_ACQUISIT_NO_0200"));
	ids = m_pParentDM->MakeControlMgrFiledData((CObject*)pCM);
	ids = m_pParentDM->MakeControlMgrUpdateFrame(_T("REC_KEY"), _T("STRING"), m_sRecKey, m_idx);

	ids = m_pParentDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("수정사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return;
	}
   	// 수정 작업 완료 

	// DM 변경 -> 뿌려주기
	CString strTmp;
	pCM->GetControlMgrData (_T("분류기호"), strTmp,-1);
	m_pParentDM->SetCellData( _T("분류기호") , strTmp, m_idx );
	pCM->GetControlMgrData (_T("수입순번호"), strTmp,-1);
	m_pParentDM->SetCellData( _T("수입순번호") , strTmp, m_idx );
	
	//AllControlDisplay(m_sParentDMAlias);

	CESL_ControlMgr *pCM1 = FindCM(_T("CM_BO_CAT_ACQUISIT_NO"));
	pCM1->AllControlDisplay(m_idx);

	CDialog::OnOK();

EFS_END
}

HBRUSH CBO_CAT_ACQUISIT_NO_0200::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
