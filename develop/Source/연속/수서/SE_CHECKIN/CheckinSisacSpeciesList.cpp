// CheckinSisacSpeciesList.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinSisacSpeciesList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinSisacSpeciesList dialog


CCheckinSisacSpeciesList::CCheckinSisacSpeciesList(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckinSisacSpeciesList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCheckinSisacSpeciesList::~CCheckinSisacSpeciesList()
{
}
BOOL CCheckinSisacSpeciesList::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

void CCheckinSisacSpeciesList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinSisacSpeciesList)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckinSisacSpeciesList, CDialog)
	//{{AFX_MSG_MAP(CCheckinSisacSpeciesList)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinSisacSpeciesList message handlers

BOOL CCheckinSisacSpeciesList::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strQry,strTemp;

	if(InitESL_Mgr(_T("연속_수서_체크인_SISAC"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_체크인_SISAC") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_연속_체크인_SISAC"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	m_pDM = FindDM(_T("DM_체크인_색인"));
	
	if(!m_pDM || !m_pCM || !m_pGrid) {
		AfxMessageBox( _T("DM,CM,grid Error : DM_체크인_색인, CM_연속_체크인_SISAC") );
		return false;
	}
	
	m_pDM->FreeData();
	strQry+=_T(" I.REC_KEY = S.REC_KEY ");
	strQry+=_T(" AND I.REC_KEY = P.SPECIES_KEY ");
	strTemp.Format(_T(" AND I.ST_ISSN='%s' "),m_strISSN);

	strQry+=strTemp;	
	m_pDM->RefreshDataManager(strQry);

	m_pGrid->Display();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCheckinSisacSpeciesList::OnCancel() 
{
	
	CDialog::OnCancel();
}

void CCheckinSisacSpeciesList::OnOK() 
{
	int idx = m_pGrid->GetIdx();
	m_strSpeciesKey = m_pDM->GetCellData(_T("ISS_종KEY"),idx);
	CDialog::OnOK();
}

BEGIN_EVENTSINK_MAP(CCheckinSisacSpeciesList, CDialog)
    //{{AFX_EVENTSINK_MAP(CCheckinSisacSpeciesList)
	ON_EVENT(CCheckinSisacSpeciesList, IDC_gridCHEKIN_SISAC, -601 /* DblClick */, OnDblClickgridCHEKINSISAC, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCheckinSisacSpeciesList::OnDblClickgridCHEKINSISAC() 
{
	int idx = m_pGrid->GetIdx();
	m_strSpeciesKey = m_pDM->GetCellData(_T("ISS_종KEY"),idx);
	CDialog::OnOK();
}

HBRUSH CCheckinSisacSpeciesList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
