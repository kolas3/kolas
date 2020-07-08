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
CBO_CAT_REG_0520::~CBO_CAT_REG_0520()
{
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
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0520 message handlers

BOOL CBO_CAT_REG_0520::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_REG_0520::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_REG_0520")) < 0)
	//if (InitESL_Mgr(_T("BO_CAT_REG")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	Display(0);
		
	pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
	if ( pDM == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_REG_0500 ERROR") );
		return FALSE;
	}
	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );

	// 040317_HSS 뜰때 그리드 다시 뿌려줄 필요 없음
	//CESL_ControlMgr *pCM1 = FindCM(_T("CM_BO_CAT_REG_0500"));
	//pCM1->AllControlDisplay(m_idx);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

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
		SelectMakeList(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
		idx = SelectGetHeadPosition(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
		if (idx < 0)
		{
			AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
			EndDialog(IDCANCEL);
			return;
		}
		break;
	case -1:
		idx = SelectGetPrev(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
		if (idx == -1) {
			idx = SelectGetHeadPosition(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
			AfxMessageBox(_T("처음 자료입니다."));
			return;
		}
		break;
	case 1:
		idx = SelectGetNext(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
		if (idx == -1) {
			idx = SelectGetTailPosition(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
			AfxMessageBox(_T("마지막 자료입니다."));
			return;
		}
		break;
	}
	/*
	if ( idx == 0 ) {
		GetDlgItem(IDC_bPREV)->EnableWindow(false);
	} else {
		GetDlgItem(IDC_bPREV)->EnableWindow(true);
	}

	if ( idx == SelectGetCount(_T("CM_MAIN_SAMPLE"), _T("MainGrid")) -1) {
		GetDlgItem(IDC_bNEXT)->EnableWindow(false);
	} else {
		GetDlgItem(IDC_bNEXT)->EnableWindow(true);
	}
	*/	
	m_idx = idx; 
	AllControlDisplay(_T("CM_BO_CAT_REG_0520"), m_idx);
	SetReverse(_T("CM_BO_CAT_REG_0500"), _T("GRID"), m_idx);

EFS_END
}

VOID CBO_CAT_REG_0520::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here

	ModifyGroup(m_idx);
	
	CDialog::OnOK();

EFS_END
}


INT CBO_CAT_REG_0520::ModifyGroup(INT idx)
{
	INT ids;
    CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
	
	if (!pDM) return -1;
	
	
	///수정 작업 시작 
	CString strID;
	//pCESLMgr->GetControlData(_T("CM_BO_CAT_REG_0520"), _T("등록작업자"), strID);
	//strID = pDM->GetCellData ( idx , 0 );
	strID = pDM->GetCellData ( _T("REC_KEY"), idx );
	//pDM->GetCellData ( _T("REC_KEY"), idx, strID );

	ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}

	pDM->InitDBFieldData();
	
	///수정 작업 시작  
	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_CAT_REG_0520"));

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //2003.10.20 이학중 수정
	CString strAccYear ;
	CString strAccCode ;
	pCM->GetControlMgrData (_T("수입년도"),strAccYear,-1);
    pCM->GetControlMgrData(_T("수입구분"),strAccCode,-1) ;
    pDM->AddDBFieldData (_T("ACCESSION_REC_MAKE_YEAR"),_T("STRING"),strAccYear) ;
    //pDM->AddDBFieldData (_T("ACQUISIT_CODE"),_T("STRING"),strAccCode) ;
	
	CString strAccDis = _T("");
	if(strAccCode == _T("1")) strAccDis = _T("구입");
	else if (strAccCode == _T("2")) strAccDis = _T("기증");
	else if (strAccCode == _T("3")) strAccDis = _T("교환");
	else if (strAccCode.IsEmpty()) strAccDis = _T("적용안함");

	pDM->SetCellData (_T("수입구분"),strAccDis,idx) ;
	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	ids = pDM->MakeControlMgrFiledData((CObject*)pCM);
	ids = pDM->MakeControlMgrUpdateFrame(_T("REC_KEY"), _T("STRING"), strID, idx);

	// 2005.06.27 ADD BY PDJ
	// 원부에 비고란 추가
	

	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("원부정보수정"),WORK_LOG));

	//ids = pDM->MakeControlMgrUpdateFrame(_T("REGISTRANT"), _T("STRING"), strID, idx);
	//ids = pDM->MakeControlMgrUpdateFrame(_T("등록작업자"), _T("STRING"), strID);
   	///수정 작업 완료 


	ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("수정사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return -1;
	}
  
	ids = pDM->EndFrame();
	if ( 0 > ids ) return -1;

	CESL_ControlMgr *pCM1 = FindCM(_T("CM_BO_CAT_REG_0500"));
	AllControlDisplay(_T("DM_BO_CAT_REG_0500"));

	pCM1->AllControlDisplay(idx);
 
	return 0;
}

VOID CBO_CAT_REG_0520::OnbNEXT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	Display(1);		

	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );
	

EFS_END
}

VOID CBO_CAT_REG_0520::OnbPREV() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	Display(-1);		

	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );
	

EFS_END
}


HBRUSH CBO_CAT_REG_0520::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
