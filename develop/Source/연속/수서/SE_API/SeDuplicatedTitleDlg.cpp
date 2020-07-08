// SeDuplicatedTitleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeDuplicatedTitleDlg.h"
#include "SeApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeDuplicatedTitleDlg dialog


CSeDuplicatedTitleDlg::CSeDuplicatedTitleDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeDuplicatedTitleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pDM = NULL;
	m_strCurrentTitle = _T("");
	

}
CSeDuplicatedTitleDlg::~CSeDuplicatedTitleDlg()
{
}


VOID CSeDuplicatedTitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeDuplicatedTitleDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeDuplicatedTitleDlg, CDialog)
	//{{AFX_MSG_MAP(CSeDuplicatedTitleDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeDuplicatedTitleDlg message handlers

BOOL CSeDuplicatedTitleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("연속_수서_서명복본조사"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_서명복본조사") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_연속_수서_서명복본조사"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	m_pDM = FindDM(_T("DM_연속_수서_서명복본조사"));
	

	if(!m_pDM || !m_pCM || !m_pGrid) {
		AfxMessageBox( _T("DM,CM,grid Error : 연속_수서_서명복본조사") );
		return FALSE;
	}	
	
	//test
	CSeApi api(this);
	api.MakeIndexWord(m_strCurrentTitle);
	m_strCurrentTitle.TrimLeft();
	m_strCurrentTitle.TrimRight();


	if(m_strCurrentTitle.IsEmpty()) {
		AfxMessageBox(_T("현재 서명이 입력되지 않았습니다!"));
		EndDialog(0);
		return FALSE;
	}
		
	InspectDuplicatedTitle();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CSeDuplicatedTitleDlg::InspectDuplicatedTitle()
{
	CString strQuery;
	
	if(ORA_CATSEARCH == m_pInfo->GetSearchIndexType())
	{
		strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND CATSEARCH(I.IDX_TITLE, '%s*', NULL)>0"),m_strCurrentTitle);
	}
	else
	{
		strQuery.Format(_T("I.REC_KEY = S.REC_KEY AND &ECOSEARCH(I.REC_KEY,SE_IDX_TITLE, '%s*')"),m_strCurrentTitle);
	}

	m_pDM->RefreshDataManager(strQuery);
	INT nRowCnt = m_pDM->GetRowCount();

	if(0 == nRowCnt) {
		AfxMessageBox(_T("서명복본조사 결과가 없습니다!"));
		EndDialog(0);
		return 0;
	}
	
	m_pCM->SetControlMgrData(_T("현재서명"),m_strCurrentTitle);

	CString msg;
	msg.Format(_T("[ %d ] 건의 유사한 서명이 검색되었습니다"),nRowCnt);
	GetDlgItem(IDC_stcDUPLICATE_TITLE)->SetWindowText(msg);

	CString strPURCHASE_YN,strDONATE_YN;

	for(INT i=0;i<nRowCnt;i++)
	{
		strPURCHASE_YN = _T("");
		strDONATE_YN = _T("");
		strPURCHASE_YN = m_pDM->GetCellData(_T("SS_구입여부"),i);
		strDONATE_YN = m_pDM->GetCellData(_T("SS_기증여부"),i);
				
		if(_T("Y") == strPURCHASE_YN &&  _T("Y") == strDONATE_YN)
		{
			m_pDM->SetCellData(_T("UDF_수입구분"),_T("구입/기증"),i);
		}
		else if(_T("N") == strPURCHASE_YN &&  _T("Y") == strDONATE_YN)
		{
			m_pDM->SetCellData(_T("UDF_수입구분"),_T("기증"),i);
		}
		else if(_T("Y") == strPURCHASE_YN &&  _T("N") == strDONATE_YN)
		{
			m_pDM->SetCellData(_T("UDF_수입구분"),_T("구입"),i);
		}		
		
	}
	
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("간행빈도코드"),_T("UDF_간행빈도"));
	m_pGrid->Display();
	

	return 0;
}


HBRUSH CSeDuplicatedTitleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
