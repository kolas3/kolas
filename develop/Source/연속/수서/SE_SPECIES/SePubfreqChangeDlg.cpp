// SePubfreqChangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SePubfreqChangeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSePubfreqChangeDlg dialog


CSePubfreqChangeDlg::CSePubfreqChangeDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSePubfreqChangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSePubfreqChangeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pDM = NULL;
	m_pCM = NULL;
	m_strOldData = _T("");
	m_strNewData = _T("");
	m_strDate = _T("");
	m_strReason = _T("");
}


CSePubfreqChangeDlg::~CSePubfreqChangeDlg()
{
}


VOID CSePubfreqChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSePubfreqChangeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSePubfreqChangeDlg, CDialog)
	//{{AFX_MSG_MAP(CSePubfreqChangeDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSePubfreqChangeDlg message handlers

BOOL CSePubfreqChangeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("연속_수서_종_간행빈도변경"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_종_간행빈도변경") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_연속_수서_종_간행빈도변경"));

	if(!m_pDM || !m_pCM ) {
		AfxMessageBox( _T("DM,CM Error : 연속_수서_종_간행빈도변경") );
		return FALSE;
	}	
	
	if(m_strOldData.IsEmpty())
	{
		AfxMessageBox( _T("기존데이터를 찾을 수 없습니다!") );
		return FALSE;
	}	
	
	CString strQuery;
	CString strResult;
	CString strLAST_VOL;
	CString strSPECIES_KEY;
	strSPECIES_KEY = m_pDM->GetCellData(_T("SS_종KEY"),0);
	strQuery.Format(_T("SELECT VOL_TITLE FROM SE_VOL_TBL WHERE SPECIES_KEY = %s AND VOL_SORT_NO = (SELECT MAX(VOL_SORT_NO) FROM SE_VOL_TBL WHERE SPECIES_KEY = %s)"),strSPECIES_KEY,strSPECIES_KEY);
	m_pDM->GetOneValueQuery(strQuery,strResult);
	if(!strResult.IsEmpty()) m_pCM->SetControlMgrData(_T("최종권호"),strResult);
	
	m_pCM->SetControlMgrData(_T("현간행빈도"),m_strOldData);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSePubfreqChangeDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_pCM->GetControlMgrData(_T("새간행빈도"),m_strNewData);

	if(m_strOldData == m_strNewData)
	{
		AfxMessageBox(_T("변경된 사항이 없습니다!"));
		return;
	}

	m_pCM->GetControlMgrData(_T("변경사유"),m_strReason);
	m_pCM->GetControlMgrData(_T("변경일자"),m_strDate);


	CDialog::OnOK();
}

HBRUSH CSePubfreqChangeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
