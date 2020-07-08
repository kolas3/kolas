// AcqInfoSearch.cpp : implementation file
//

#include "stdafx.h"

#include "AcqInfoSearch.h"
#include "..\SE_API\SeApi.h"
#include "..\..\..\공통\COMMON_DONATOR_MANAGER\DonatorNameGetterDlg.h"
#include "..\..\..\공통\COMMON_DONATOR_MANAGER\DonatorNameGetter_Search.h"

#define SENDQUERY WM_USER+937

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAcqInfoSearch dialog


CAcqInfoSearch::CAcqInfoSearch(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CAcqInfoSearch)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMgr=pParent;
}

CAcqInfoSearch::~CAcqInfoSearch()
{
}


void CAcqInfoSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcqInfoSearch)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CAcqInfoSearch::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


BEGIN_MESSAGE_MAP(CAcqInfoSearch, CDialog)
	//{{AFX_MSG_MAP(CAcqInfoSearch)
	ON_BN_CLICKED(IDC_btnCHE_DONATOR_SEARCH, OnbtnCHEDONATORSEARCH)
	ON_CBN_SELCHANGE(IDC_cmbBIB_ACQ_TYPE, OnSelchangecmbBIBACQTYPE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcqInfoSearch message handlers

VOID CAcqInfoSearch::SetFocusOnTitle()
{
	//서명에 setfocus
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_edtCHE_TITLE1);
	if(pWnd) pWnd->SetFocus();
}

VOID CAcqInfoSearch::SendQueryToParent()
{
	:: SendMessage(	m_pMgr->GetSafeHwnd(), SENDQUERY , 0, 0);
}

void CAcqInfoSearch::OnOK() 
{
	// TODO: Add extra validation here
	if(MakeQuery()<0) return;

	SendQueryToParent();

	return;

	CDialog::OnOK();
}

CString CAcqInfoSearch::TrimAll(CString strData)
{
	strData.TrimLeft();
	strData.TrimRight();

	return strData;
}

CString CAcqInfoSearch::MakeIDXRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	
	CString tmpquery;
	CSeApi api(this);

	if(strData2.IsEmpty()) // catsearch
	{ 
		api.MakeIndexWord(strData1);

if(ORA_CATSEARCH == m_pInfo->GetSearchIndexType())
		tmpquery.Format(_T(" CATSEARCH(I.IDX_%s, '%s*', NULL)>0"),strFieldName, strData1);
else		
		tmpquery.Format(_T("&ECOSEARCH(I.REC_KEY,SE_IDX_%s, '%s*')"),strFieldName, strData1);
	
		
	}
	else // between
	{
		api.MakeIndexWord(strData1);
		api.MakeIndexWord(strData2);
		tmpquery.Format(_T("I.IDX_I%s BETWEEN '%s' AND '%s'"),strFieldName,strData1,strData2);
	}

	return tmpquery;


}

INT CAcqInfoSearch::MakeQuery()
{
	//구입 및 기증
	CString PROVIDER_CODE		= _T("");		m_pCM->GetControlMgrData(_T("구입처"),PROVIDER_CODE);
	CString DONATOR_NAME		= _T("");		m_pCM->GetControlMgrData(_T("기증자"),DONATOR_NAME);
	
	//공통
	CString ISSN				= _T("");
	CString ISSN1				= _T("");		m_pCM->GetControlMgrData(_T("ISSN1"),ISSN1);
	CString ISSN2				= _T("");		m_pCM->GetControlMgrData(_T("ISSN2"),ISSN2);		
	CString PUB_FREQ			= _T("");		m_pCM->GetControlMgrData(_T("간행빈도"),PUB_FREQ);
	CString PUB_STATUS			= _T("");		m_pCM->GetControlMgrData(_T("간행상태"),PUB_STATUS);
	
	CString TITLE1				= _T("");		m_pCM->GetControlMgrData(_T("서명1"),TITLE1);
	CString TITLE2				= _T("");		m_pCM->GetControlMgrData(_T("서명2"),TITLE2);
	CString PUBLISHER1			= _T("");		m_pCM->GetControlMgrData(_T("발행자1"),PUBLISHER1);
	CString PUBLISHER2			= _T("");		m_pCM->GetControlMgrData(_T("발행자2"),PUBLISHER2);
	
	CString ACQ_DATE1		= _T("");		m_pCM->GetControlMgrData(_T("ACQ_DATE1"),ACQ_DATE1);
	CString ACQ_DATE2		= _T("");		m_pCM->GetControlMgrData(_T("ACQ_DATE2"),ACQ_DATE2);
	
		
	PROVIDER_CODE = TrimAll(PROVIDER_CODE);
	DONATOR_NAME = TrimAll(DONATOR_NAME);
	ISSN1 = TrimAll(ISSN1);
	ISSN2 = TrimAll(ISSN2);
	PUB_FREQ = TrimAll(PUB_FREQ);
	TITLE1 = TrimAll(TITLE1);
	TITLE2 = TrimAll(TITLE2);
	PUBLISHER1 = TrimAll(PUBLISHER1);
	PUBLISHER2 = TrimAll(PUBLISHER2);
	ACQ_DATE1 = TrimAll(ACQ_DATE1);
	ACQ_DATE2 = TrimAll(ACQ_DATE2);

	//색인 범위 검색의 경우 
	//왼쪽만 입력  - [ IDX_ ] 형식에 catsearch 
	//오른쪽만 입력 - error!
	//양쪽다 입력 - [ IDX_I ] 형식에 범위 검색 
	
	CString strQuery = _T("");
	CString tmpquery = _T("");

	// 기본조건
	strQuery += _T("I.REC_KEY=V.SPECIES_KEY AND V.REC_KEY=A.VOL_KEY ");

	// 구입 / 기증
	if(1 == m_nACQ_TYPE) {
		if(!PROVIDER_CODE.IsEmpty())
		{
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" A.PROVIDER = '%s'"), PROVIDER_CODE);
			strQuery += tmpquery;
		}
	} 
	else if(2 == m_nACQ_TYPE) {
		if(!DONATOR_NAME.IsEmpty())
		{
			strQuery += _T(" AND I.REC_KEY = D.SPECIES_KEY");
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" D.DONATOR_NAME = '%s'"), DONATOR_NAME);
			strQuery += tmpquery;
		}
	}

	// 수입구분 조건
	if(1 == m_nACQ_TYPE) {
		tmpquery.Format(_T(" AND A.ACQ_CODE = '1' "));
		strQuery += tmpquery;		
	} 
	else if(2 == m_nACQ_TYPE) {
		tmpquery.Format(_T(" AND A.ACQ_CODE = '2' "));
		strQuery += tmpquery;		
	}
	else if(3 == m_nACQ_TYPE) {
		tmpquery.Format(_T(" AND A.ACQ_CODE = '3' "));
		strQuery += tmpquery;		
	}
	
	if(TITLE1.IsEmpty() && !TITLE2.IsEmpty()) {
		AfxMessageBox(_T("ERROR!: 서명 검색은 오른쪽 항목만으로 할 수 없습니다"));
		return -1;
	}

	if(PUBLISHER1.IsEmpty() && !PUBLISHER2.IsEmpty()) {
		AfxMessageBox(_T("ERROR!: 발행자 검색은 오른쪽 항목만으로 할 수 없습니다"));
		return -2;
	}


	//MakeIDXRangeQuery
	if(!TITLE1.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery = MakeIDXRangeQuery(_T("TITLE"), TITLE1, TITLE2);
		strQuery += tmpquery;
	}
		
	if(!PUBLISHER1.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery = MakeIDXRangeQuery(_T("PUBLISHER"), PUBLISHER1, PUBLISHER2);
		strQuery += tmpquery;
	}
	


	//일치 검색 
	if(!PUB_FREQ.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery.Format(_T(" I.PUB_FREQ = '%s'"), PUB_FREQ);
		strQuery += tmpquery;
	}

	if(!PUB_STATUS.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery.Format(_T(" I.PUB_STATUS = '%s'"), PUB_STATUS);
		strQuery += tmpquery;
	}	
	
	//ISSN
	if(!ISSN1.IsEmpty())
	{
		if(!ISSN2.IsEmpty())
		{
			ISSN.Format(_T("%s-%s"),ISSN1,ISSN2);
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" I.ST_ISSN = '%s'"), ISSN);
			strQuery += tmpquery;
		}

	}

	// ACQ_DATE1
	if(!ACQ_DATE1.IsEmpty())
	{
		if(!ACQ_DATE1.IsEmpty())
		{
			tmpquery.Format(_T(" AND (select min(ACQ_DATE) from se_book_tbl sb where sb.acq_type_vol_key=a.rec_key) >= '%s' AND \
							 (select max(ACQ_DATE) from se_book_tbl sb where sb.acq_type_vol_key=a.rec_key) <= '%s' "), ACQ_DATE1,ACQ_DATE2);
			strQuery += tmpquery;		
		}
	}	

	m_strQuery = strQuery;

	return 0;

}

BOOL CAcqInfoSearch::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	if( InitESL_Mgr( _T("연속_수서_입수자료관리_검색") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_입수자료관리_검색") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_연속_입수자료관리_검색"));
	if( m_pCM == NULL )
	{
		AfxMessageBox(_T("CM Error : CM_연속_입수자료관리_검색"));
		return FALSE;
	}		
	
	//수입구분에 따른 콘트롤 설정
	if(1 == m_nACQ_TYPE)
	{
		m_pCM->SetControlMgrData(_T("수입구분"),_T("1"));
		GetDlgItem(IDC_btnCHE_DONATOR_SEARCH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtCHE_DONATOR)->ShowWindow(SW_HIDE);
	}
	else
	{
		m_pCM->SetControlMgrData(_T("수입구분"),_T("2"));
		GetDlgItem(IDC_cmbCHE_PROVIDER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcCHE_PROVIDER)->SetWindowText(_T("기증자"));
	}
	m_pCM->AllControlDisplay();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAcqInfoSearch::OnbtnCHEDONATORSEARCH() 
{
	// TODO: Add your control notification handler code here
	CDonatorNameGetter_Search searchDlg(this);
	searchDlg.SetOpenMode(-1000);
	if(IDOK == searchDlg.DoModal())
	{
		CDonatorNameGetterDlg searchResult(this);
		if(IDOK == searchResult.DoModal())
		{
			m_pCM->SetControlMgrData(_T("기증자"),searchResult.GetDonatorName());
		}
	}
}

void CAcqInfoSearch::OnSelchangecmbBIBACQTYPE() 
{
	// TODO: Add your control notification handler code here
	
	CString strTmp;
	m_pCM->GetControlMgrData(_T("수입구분"),strTmp);
	strTmp.TrimLeft();	
	strTmp.TrimRight(); 

	if( _T("1") == strTmp) { //구입 		

		GetDlgItem(IDC_cmbCHE_PROVIDER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcCHE_PROVIDER)->SetWindowText(_T("구입처"));		
		GetDlgItem(IDC_edtCHE_DONATOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnCHE_DONATOR_SEARCH)->ShowWindow(SW_HIDE);
		m_nACQ_TYPE=1;
		
	} else if( _T("2") == strTmp) { //기증

		GetDlgItem(IDC_cmbCHE_PROVIDER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcCHE_PROVIDER)->SetWindowText(_T("기증자"));		
		GetDlgItem(IDC_edtCHE_DONATOR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnCHE_DONATOR_SEARCH)->ShowWindow(SW_SHOW);		
		m_nACQ_TYPE=2;
	} 
	else if( _T("3") == strTmp) { //편입
		GetDlgItem(IDC_cmbCHE_PROVIDER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcCHE_PROVIDER)->SetWindowText(_T(""));		
		GetDlgItem(IDC_edtCHE_DONATOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnCHE_DONATOR_SEARCH)->ShowWindow(SW_HIDE);	
		m_nACQ_TYPE=3;
	}
	else
	{
		GetDlgItem(IDC_cmbCHE_PROVIDER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcCHE_PROVIDER)->SetWindowText(_T(""));		
		GetDlgItem(IDC_edtCHE_DONATOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnCHE_DONATOR_SEARCH)->ShowWindow(SW_HIDE);	
		m_nACQ_TYPE=-1;
	}
	
}


HBRUSH CAcqInfoSearch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}