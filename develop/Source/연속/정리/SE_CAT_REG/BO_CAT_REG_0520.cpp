// BO_CAT_REG_0520.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_REG_0520.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0520 dialog


CBO_CAT_REG_0520::CBO_CAT_REG_0520(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_0520)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CBO_CAT_REG_0520::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_0520)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_0520, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_0520)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0520 message handlers

BOOL CBO_CAT_REG_0520::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_REG_0520::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SE_CAT_REG_0520")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	Display(0);
		
	m_pParentDM = FindDM(_T("DM_SE_CAT_REG_0500"));
	if ( m_pParentDM == NULL ) {
		AfxMessageBox ( _T("DM_SE_CAT_REG_0500 ERROR") );
		return FALSE;
	}
	m_sRecKey = m_pParentDM->GetCellData( _T("REC_KEY") , m_idx );
	
	m_pCM = FindCM(_T("CM_SE_CAT_REG_0520"));
	m_pCM->AllControlDisplay(m_idx);

	return TRUE; 

EFS_END
return FALSE;

}

VOID CBO_CAT_REG_0520::Display(INT nDir)
{
EFS_BEGIN

	INT idx;
	switch(nDir)
	{
	case 0:
		SelectMakeList(_T("CM_SE_CAT_REG_0500"), _T("GRID"));
		idx = SelectGetHeadPosition(_T("CM_SE_CAT_REG_0500"), _T("GRID"));
		if (idx < 0)
		{
			AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
			EndDialog(IDCANCEL);
			return;
		}
		break;
	case -1:
		idx = SelectGetPrev(_T("CM_SE_CAT_REG_0500"), _T("GRID"));
		if (idx == -1) {
			idx = SelectGetHeadPosition(_T("CM_SE_CAT_REG_0500"), _T("GRID"));
			AfxMessageBox(_T("처음 자료입니다."));
			return;
		}
		break;
	case 1:
		idx = SelectGetNext(_T("CM_SE_CAT_REG_0500"), _T("GRID"));
		if (idx == -1) {
			idx = SelectGetTailPosition(_T("CM_SE_CAT_REG_0500"), _T("GRID"));
			AfxMessageBox(_T("마지막 자료입니다."));
			return;
		}
		break;
	}
	
	m_idx = idx;
	AllControlDisplay(_T("CM_SE_CAT_REG_0520"), m_idx);
	SetReverse(_T("CM_SE_CAT_REG_0500"), _T("GRID"), m_idx);

EFS_END
}

VOID CBO_CAT_REG_0520::OnOK() 
{
EFS_BEGIN

	ModifyGroup(m_idx);
	
	CDialog::OnOK();

EFS_END
}


INT CBO_CAT_REG_0520::ModifyGroup(INT idx)
{
	INT ids;
	
	///수정 작업 시작 
	CString strID;
	//pCESLMgr->GetControlData(_T("CM_SE_CAT_REG_0520"), _T("등록작업자"), strID);
	strID = m_pParentDM->GetCellData ( idx , 0 );

	ids = m_pParentDM->StartFrame();
	if ( ids < 0 ) {
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}
	m_pParentDM->InitDBFieldData();
	
	///수정 작업 시작  
	CESL_ControlMgr *pCM = FindCM(_T("CM_SE_CAT_REG_0520"));

	CString strAccYear ;
	CString strAccCode ;
	pCM->GetControlMgrData (_T("수입년도"),strAccYear,-1);
    pCM->GetControlMgrData(_T("수입구분"),strAccCode,-1) ;
    m_pParentDM->AddDBFieldData (_T("ACCESSION_REC_MAKE_YEAR"),_T("STRING"),strAccYear) ;
	
	CString strAccDis = _T("");
	if(strAccCode == _T("1")) strAccDis = _T("구입");
	else if (strAccCode == _T("2")) strAccDis = _T("기증");
	else if (strAccCode == _T("3")) strAccDis = _T("교환");
	else if (strAccCode.IsEmpty()) strAccDis = _T("적용안함");

	m_pParentDM->SetCellData (_T("수입구분"),strAccDis,idx) ;

	ids = m_pParentDM->MakeControlMgrFiledData((CObject*)pCM);
	if ( ids < 0 ) {
		AfxMessageBox(_T("MakeControlMgrFiledData Error"));
		return -1;
	}
	ids = m_pParentDM->MakeControlMgrUpdateFrame(_T("REC_KEY"), _T("STRING"), strID, idx);
	if ( ids < 0 ) {
		AfxMessageBox(_T("MakeControlMgrUpdateFrame Error"));
		return -1;
	}

	//ids = m_pParentDM->MakeControlMgrUpdateFrame(_T("REGISTRANT"), _T("STRING"), strID, idx);
	//ids = m_pParentDM->MakeControlMgrUpdateFrame(_T("등록작업자"), _T("STRING"), strID);

	ids = m_pParentDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("수정사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return -1;
	}

	ids = m_pParentDM->EndFrame();
	if ( 0 > ids ) return -1;

	AllControlDisplay(_T("DM_SE_CAT_REG_0500"));
	m_pCM->AllControlDisplay(idx);
 
	return 0;
}

VOID CBO_CAT_REG_0520::OnbNEXT() 
{
EFS_BEGIN

	Display(1);		

	m_sRecKey = m_pParentDM->GetCellData( _T("REC_KEY") , m_idx );
	
EFS_END
}

VOID CBO_CAT_REG_0520::OnbPREV() 
{
EFS_BEGIN

	Display(-1);		

	m_sRecKey = m_pParentDM->GetCellData( _T("REC_KEY") , m_idx );

EFS_END
}
