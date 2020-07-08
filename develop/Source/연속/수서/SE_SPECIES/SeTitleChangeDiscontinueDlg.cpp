// SeTitleChangeDiscontinueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeTitleChangeDiscontinueDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeTitleChangeDiscontinueDlg dialog


CSeTitleChangeDiscontinueDlg::CSeTitleChangeDiscontinueDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeTitleChangeDiscontinueDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeTitleChangeDiscontinueDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_pCM = NULL;
	m_OldMarc.Init();
	m_pNewMarc = NULL;
	m_strOldTitle = _T("");
	m_strNewTitle = _T("");
	m_strOldISSN1 = _T("");
	m_strOldISSN2 = _T("");
	m_strNewISSN1 = _T("");
	m_strNewISSN2 = _T("");
	m_strNewFirstVol = _T("");

	m_strSPECIES_KEY = _T("");
	m_nACQ_TYPE = 1;
	
    m_strOldSecondIndicator = _T("");
	m_strNewSecondIndicator = _T("");

	m_strDiscontinueYear = _T("");
	m_strDiscontinueReason = _T("");
}

CSeTitleChangeDiscontinueDlg::~CSeTitleChangeDiscontinueDlg()
{
}

VOID CSeTitleChangeDiscontinueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeTitleChangeDiscontinueDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeTitleChangeDiscontinueDlg, CDialog)
	//{{AFX_MSG_MAP(CSeTitleChangeDiscontinueDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeTitleChangeDiscontinueDlg message handlers
BOOL CSeTitleChangeDiscontinueDlg::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

BOOL CSeTitleChangeDiscontinueDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if(InitESL_Mgr(_T("연속_수서_종_서명변경_폐간"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_종_서명변경_폐간") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_연속_수서_종_서명변경_폐간"));

	if(!m_pDM || !m_pCM ) {
		AfxMessageBox( _T("DM,CM Error : 연속_수서_종_서명변경_폐간") );
		return FALSE;
	}	

	InitControls();


	//현재 정보 보여주기 
	CString strQuery;
	CString strResult;
	CString strFIRST_VOL;
	CString strLAST_VOL;
	CString strSPECIES_KEY;
	strSPECIES_KEY = m_pDM->GetCellData(_T("SS_종KEY"),0);
	m_strSPECIES_KEY = strSPECIES_KEY; //update를 위해 종KEY를 멤버변수로 가지고 있는다 

	strQuery.Format(_T("SELECT VOL_TITLE FROM SE_VOL_TBL WHERE SPECIES_KEY = %s AND VOL_SORT_NO = (SELECT MIN(VOL_SORT_NO) FROM SE_VOL_TBL WHERE SPECIES_KEY = %s)"),strSPECIES_KEY,strSPECIES_KEY);
	m_pDM->GetOneValueQuery(strQuery,strFIRST_VOL);
	if(!strFIRST_VOL.IsEmpty()) m_pCM->SetControlMgrData(_T("현최초입수권호"),strFIRST_VOL);
	

	strQuery.Format(_T("SELECT VOL_TITLE FROM SE_VOL_TBL WHERE SPECIES_KEY = %s AND VOL_SORT_NO = (SELECT MAX(VOL_SORT_NO) FROM SE_VOL_TBL WHERE SPECIES_KEY = %s)"),strSPECIES_KEY,strSPECIES_KEY);
	m_pDM->GetOneValueQuery(strQuery,strLAST_VOL);
	if(!strLAST_VOL.IsEmpty()) m_pCM->SetControlMgrData(_T("현최종입수권호"),strLAST_VOL);
	

	m_pCM->SetControlMgrData(_T("현서명"),m_strOldTitle);
	m_pCM->SetControlMgrData(_T("현ISSN1"),m_strOldISSN1);	
	m_pCM->SetControlMgrData(_T("현ISSN2"),m_strOldISSN2);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CSeTitleChangeDiscontinueDlg::InitControls()
{
	//제 2 지시기호 설정 
	CComboBox * pCombo = NULL;
	

	//780 tag
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbDISCONTINUE_OLD_INDICATOR );
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("0 - ...을(를) 개제") );
		pCombo->InsertString( 1, _T("1 - ...을(를) 일부 개제") ); 
		pCombo->InsertString( 2, _T("4 - ...과 ...을(를) 합병") );
		pCombo->InsertString( 3, _T("5 - ...을(를) 흡수") );
		pCombo->InsertString( 4, _T("6 - ...을(를) 일부 흡수") );
		pCombo->InsertString( 5, _T("7 - ...으로부터 분리") );
		pCombo->SetCurSel( 0 );
	}

	
	//785 tag
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbDISCONTINUE_NEW_INDICATOR );
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("0 - ...으로(로) 개제") );
		pCombo->InsertString( 1, _T("1 - ...의 한 부분으로 개제") ); 
		pCombo->InsertString( 2, _T("4 - ...으로(로) 흡수") );
		pCombo->InsertString( 3, _T("5 - ...으의 한 부분으로 흡수") );
		pCombo->InsertString( 4, _T("6 - ...과(와) ...으로(로) 분리") );
		pCombo->InsertString( 5, _T("7 - ...에 합병") );
		pCombo->InsertString( 6, _T("8 - ...으로(로) 소급변경") );
		pCombo->SetCurSel( 0 );
	}


}

VOID CSeTitleChangeDiscontinueDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	m_pCM->GetControlMgrData(_T("새서명"),m_strNewTitle);

	if(m_strNewTitle.IsEmpty())
	{
		AfxMessageBox(_T("새서명이 입력되지 않았습니다!"));
		return;
	}

	if(m_strOldTitle == m_strNewTitle) 
	{
		AfxMessageBox(_T("새서명이 현서명과 동일합니다!"));
		return;
	}
	
	
	CString strFIRST_VOL,strLAST_VOL,strTmp;

	//마크 그룹 설정
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	m_pCM->GetControlMgrData(_T("새ISSN1"),m_strNewISSN1);	
	m_pCM->GetControlMgrData(_T("새ISSN2"),m_strNewISSN2);
	m_pCM->GetControlMgrData(_T("폐간년도"),m_strDiscontinueYear);
	m_pCM->GetControlMgrData(_T("폐간사유"),m_strDiscontinueReason);
	m_pCM->GetControlMgrData(_T("새최초입수권호"),m_strNewFirstVol);
	m_pCM->GetControlMgrData(_T("현최초입수권호"),strFIRST_VOL);
	m_pCM->GetControlMgrData(_T("현최종입수권호"),strLAST_VOL);
	
	
	//선행저록 처리(780)
	//지기 기호
	CComboBox * pCombo = NULL;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbDISCONTINUE_OLD_INDICATOR );
	INT nCurSel = 0;
	if(pCombo) nCurSel = pCombo->GetCurSel();
	
	CString strIndicator = _T("");
	CString strMarcTag = _T("");

	switch(nCurSel)
	{
	case 0:
		strIndicator = _T("00");
		break;
	case 1:
		strIndicator = _T("01");
		break;
	case 2:
		strIndicator = _T("04");
		break;
	case 3:
		strIndicator = _T("05");
		break;
	case 4:
		strIndicator = _T("06");
		break;
	case 5:
		strIndicator = _T("07");
		break;
	}
	
	//기존의 780tag을 지운다.
	m_pInfo->pMarcMgr->DeleteField(m_pNewMarc,_T("780"));


	//서명 
	strMarcTag = _T("780$t!") + strIndicator;
	m_pInfo->pMarcMgr->SetItem(m_pNewMarc,strMarcTag,m_strOldTitle,_T(""),ADD_SUBFIELD);
	

	//관계
	if(!strFIRST_VOL.IsEmpty() || !strLAST_VOL.IsEmpty())
	{
		strTmp = _T("");
		strTmp = strFIRST_VOL + _T("-") + strLAST_VOL;
		strMarcTag = _T("780$g!") + strIndicator;
		m_pInfo->pMarcMgr->SetItem(m_pNewMarc,strMarcTag,strTmp,_T(""),ADD_SUBFIELD);
	}
	
	//ISSN
	if(!m_strOldISSN1.IsEmpty())
	{
		CString strISSN;
		strISSN = m_strOldISSN1 + _T("-") + m_strOldISSN2;
		strMarcTag = _T("780$x!") + strIndicator;
		m_pInfo->pMarcMgr->SetItem(m_pNewMarc,strMarcTag,strISSN,_T(""),ADD_SUBFIELD);
	}


	//후속저록 처리(785) 및 폐간처리 
	//지기 기호
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbDISCONTINUE_NEW_INDICATOR );
	nCurSel = 0;
	if(pCombo) nCurSel = pCombo->GetCurSel();
	
	switch(nCurSel)
	{
	case 0:
		strIndicator = _T("00");
		break;
	case 1:
		strIndicator = _T("01");
		break;
	case 2:
		strIndicator = _T("04");
		break;
	case 3:
		strIndicator = _T("05");
		break;
	case 4:
		strIndicator = _T("06");
		break;
	case 5:
		strIndicator = _T("07");
		break;
	case 6:
		strIndicator = _T("08");
		break;
	}


	//서명 
	strMarcTag = _T("785$t!") + strIndicator;
	m_pInfo->pMarcMgr->SetItem(&m_OldMarc,strMarcTag,m_strNewTitle,_T(""),ADD_SUBFIELD);


	//관계
	if(!m_strNewFirstVol.IsEmpty())
	{
		strTmp = _T("");
		strTmp = m_strNewFirstVol + _T("-"); 
		strMarcTag = _T("785$g!") + strIndicator;
		m_pInfo->pMarcMgr->SetItem(&m_OldMarc,strMarcTag,strTmp,_T(""),ADD_SUBFIELD);
	}
	
	//ISSN
	if(!m_strNewISSN1.IsEmpty())
	{
		CString strISSN;
		strISSN = m_strNewISSN1 + _T("-") + m_strNewISSN2;
		strMarcTag = _T("785$x!") + strIndicator;		
		m_pInfo->pMarcMgr->SetItem(&m_OldMarc,strMarcTag,strISSN,_T(""),ADD_SUBFIELD);
	}
	
	//008TAG처리 
	//간행상태
	m_pInfo->pMarcMgr->SetItem(&m_OldMarc,_T("008@6"),_T("d")); 
	
	//폐간년 
	if(!m_strDiscontinueYear.IsEmpty())
	{
		m_pInfo->pMarcMgr->SetItem(&m_OldMarc,_T("008@11-14"),m_strDiscontinueYear);
	}
	
	CDialog::OnOK();
}

HBRUSH CSeTitleChangeDiscontinueDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
