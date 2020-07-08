// TransferSearch.cpp : implementation file
//

#include "stdafx.h"
#include "TransferSearch.h"
#include "..\..\..\공통\COMMON_DONATOR_MANAGER\DonatorNameGetterDlg.h"
#include "..\..\..\공통\COMMON_DONATOR_MANAGER\DonatorNameGetter_Search.h"
#include "..\SE_API\SeApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransferSearch dialog


CTransferSearch::CTransferSearch(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransferSearch)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMgr = pParent;
	m_pCM = NULL;
	m_nACQ_TYPE = 1; //default
	m_strQuery = _T("");
	m_nSearchType = 0; //default

}

CTransferSearch::~CTransferSearch()
{
}

VOID CTransferSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransferSearch)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransferSearch, CDialog)
	//{{AFX_MSG_MAP(CTransferSearch)
	ON_BN_CLICKED(IDC_btnTRA_SEARCH_DONATOR, OnbtnTRASEARCHDONATOR)
	ON_BN_CLICKED(IDC_btnTRA_SEARCH_CLEAR, OnbtnTRASEARCHCLEAR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransferSearch message handlers
BOOL CTransferSearch::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CTransferSearch::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("연속_수서_자료인계_검색") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_자료인계_검색") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_연속_수서_자료인계_검색"));
	if(!m_pCM) 	{
		AfxMessageBox(_T("CM Error : CM_연속_수서_자료인계_검색"));
		return FALSE;
	}		

	//수입구분에 따른 콘트롤 설정
	if(1 == m_nACQ_TYPE)
	{
		GetDlgItem(IDC_btnTRA_SEARCH_DONATOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtTRA_SEARCH_DONATOR)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_cmbTRA_SEARCH_PROVIDER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcTRA_SEARCH_PROVIDER)->SetWindowText(_T("기증자"));
	}
	
	//검색 구분을 설정 해준다.
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbTRA_SEARCH_TYPE);
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("인계대상자료") );
		pCombo->InsertString( 1, _T("인계된자료") );
		pCombo->SetCurSel(0);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CTransferSearch::MakeQuery()
{

	//책정보 관련
	CString ACQ_YEAR = _T("");					m_pCM->GetControlMgrData(_T("수입년도"),ACQ_YEAR);
	CString ACQ_DATE1 = _T("");					m_pCM->GetControlMgrData(_T("입수일자1"),ACQ_DATE1);
	CString ACQ_DATE2 = _T("");					m_pCM->GetControlMgrData(_T("입수일자2"),ACQ_DATE2);
	CString RECEIPT_NO1 = _T("");				m_pCM->GetControlMgrData(_T("접수번호1"),RECEIPT_NO1);
	CString RECEIPT_NO2 = _T("");				m_pCM->GetControlMgrData(_T("접수번호2"),RECEIPT_NO2);
	CString SHELF_LOC_CODE = _T("");			m_pCM->GetControlMgrData(_T("자료실"),SHELF_LOC_CODE);
	

	//서지 정보 관련 
	//구입 및 기증
	CString PROVIDER_CODE = _T("");				m_pCM->GetControlMgrData(_T("구입처"),PROVIDER_CODE);
	CString DONATOR_NAME = _T("");				m_pCM->GetControlMgrData(_T("기증자"),DONATOR_NAME);
	
	//공통
	CString ISSN = _T("");
	CString ISSN1 = _T("");						m_pCM->GetControlMgrData(_T("ISSN1"),ISSN1);
	CString ISSN2 = _T("");						m_pCM->GetControlMgrData(_T("ISSN2"),ISSN2);		
	CString PUB_FREQ = _T("");					m_pCM->GetControlMgrData(_T("간행빈도"),PUB_FREQ);
	
	CString TITLE1= _T("");						m_pCM->GetControlMgrData(_T("서명1"),TITLE1);
	CString TITLE2= _T("");						m_pCM->GetControlMgrData(_T("서명2"),TITLE2);
	CString PUBLISHER1 = _T("");				m_pCM->GetControlMgrData(_T("발행자1"),PUBLISHER1);
	CString PUBLISHER2 = _T("");				m_pCM->GetControlMgrData(_T("발행자2"),PUBLISHER2);
	
		
	PROVIDER_CODE = TrimAll(PROVIDER_CODE);
	DONATOR_NAME = TrimAll(DONATOR_NAME);
	ISSN1 = TrimAll(ISSN1);
	ISSN2 = TrimAll(ISSN2);
	PUB_FREQ = TrimAll(PUB_FREQ);
	TITLE1 = TrimAll(TITLE1);
	TITLE2 = TrimAll(TITLE2);
	PUBLISHER1 = TrimAll(PUBLISHER1);
	PUBLISHER2 = TrimAll(PUBLISHER2);


	//색인 범위 검색의 경우 
	//왼쪽만 입력  - [ IDX_ ] 형식에 catsearch 
	//오른쪽만 입력 - error!
	//양쪽다 입력 - [ IDX_I ] 형식에 범위 검색 
	
	CString strQuery = _T("");
	CString tmpquery = _T("");
	
	//수입 구분 조건 
	if(1 == m_nACQ_TYPE) {
		strQuery += _T("B.ACQ_CODE = '1'");
	} else if(2 == m_nACQ_TYPE) {
		strQuery += _T("B.ACQ_CODE = '2'");
	}

	//추가(5163)
	strQuery += _T(" AND I.REC_KEY = S.REC_KEY");
	
	//기존 K2UP
	strQuery += _T(" AND I.REC_KEY = B.SPECIES_KEY");
	strQuery += _T(" AND V.REC_KEY = B.VOL_KEY");
	strQuery += _T(" AND A.REC_KEY = B.ACQ_TYPE_VOL_KEY");
	strQuery += _T(" AND B.BOOK_TYPE = 'C'");

	
	//검색구분조건
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbTRA_SEARCH_TYPE);
	m_nSearchType = 0;
	if(pCombo) m_nSearchType = pCombo->GetCurSel();
	
	if(0 == m_nSearchType) //인계 대상 자료
	{
		strQuery += _T(" AND B.WORKING_STATUS = 'SEL111O'");
		strQuery += _T(" AND B.TRANSFER_YN = 'N'");
	}
	else if(1 == m_nSearchType) //인계된 자료(실시간 인계,실시간 배가);
	{
		strQuery += _T(" AND (B.WORKING_STATUS = 'SEL111O' OR B.WORKING_STATUS = 'SEL112N')");
		strQuery += _T(" AND B.TRANSFER_YN = 'Y'");
	}

	if(RECEIPT_NO1.IsEmpty() && !RECEIPT_NO2.IsEmpty())
	{
		AfxMessageBox(_T("접수번호 범위의 뒷부분만 입력 되었습니다!"));
		return -1;
	}


	//접수번호
	tmpquery.Empty();
	tmpquery = MakeIntRangeQuery(_T("B.RECEIPT_NO"),RECEIPT_NO1,RECEIPT_NO2);
	if(!tmpquery.IsEmpty())
	{
		strQuery += _T(" AND ");
		strQuery += tmpquery;
	}
	
	
	//입수일자
	if(ACQ_DATE1.IsEmpty() && !ACQ_DATE2.IsEmpty())
	{
		AfxMessageBox(_T("입수일자 범위의 뒷부분만 입력 되었습니다!"));
		return -2;
	}
	

	tmpquery.Empty();
	tmpquery = MakeFitAndRangeQuery(_T("B.ACQ_DATE"),ACQ_DATE1,ACQ_DATE2);
	if(!tmpquery.IsEmpty())
	{
		strQuery += _T(" AND ");
		strQuery += tmpquery;
	}
	


	if(!ACQ_YEAR.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery.Format(_T("B.ACQ_YEAR = '%s'"), ACQ_YEAR);
		strQuery += tmpquery;
	}
	
	
	if(!SHELF_LOC_CODE.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery.Format(_T("B.SHELF_LOC_CODE = '%s'"), SHELF_LOC_CODE);
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

	m_strQuery = strQuery;

	return 0;

}

CString CTransferSearch::MakeIDXRangeQuery(CString strFieldName, CString strData1, CString strData2)
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

CString CTransferSearch::MakeRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	CString tmpquery = _T("");	
	INT nOption;
	
	if(!strData1.IsEmpty()) nOption = 1;	
	if(!strData2.IsEmpty()) nOption = 2;	
	if(!strData1.IsEmpty() && !strData2.IsEmpty()) nOption = 3;

	if(nOption != 0)
	{
		if(nOption == 1) tmpquery.Format(_T("%s >= '%s'"),strFieldName,strData1);
		if(nOption == 2) tmpquery.Format(_T("%s <= '%s'"),strFieldName,strData2);
		if(nOption == 3) tmpquery.Format(_T("%s BETWEEN '%s' AND '%s'"),strFieldName,strData1,strData2);
	}

	return tmpquery;
}

CString CTransferSearch::MakeIntRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	CString tmpquery = _T("");	
	INT nOption;
	
	if(!strData1.IsEmpty()) nOption = 1;	
	if(!strData1.IsEmpty() && !strData2.IsEmpty()) nOption = 3;

	if(nOption != 0)
	{
		if(nOption == 1) tmpquery.Format(_T("%s = %s"),strFieldName,strData1);
		if(nOption == 3) tmpquery.Format(_T("%s BETWEEN %s AND %s"),strFieldName,strData1,strData2);
	}

	return tmpquery;
}

CString CTransferSearch::MakeFitAndRangeQuery(CString strFieldName, CString strData1, CString strData2)
{
	CString tmpquery = _T("");	
	INT nOption;
	
	if(!strData1.IsEmpty()) nOption = 1;	
	if(!strData1.IsEmpty() && !strData2.IsEmpty()) nOption = 3;

	if(nOption != 0)
	{
		if(nOption == 1) tmpquery.Format(_T("%s = '%s'"),strFieldName,strData1);
		if(nOption == 3) tmpquery.Format(_T("%s BETWEEN '%s' AND '%s'"),strFieldName,strData1,strData2);
	}

	return tmpquery;
}

CString CTransferSearch::TrimAll(CString strData)
{
	strData.TrimLeft();
	strData.TrimRight();

	return strData;
}

VOID CTransferSearch::SendQueryToParent()
{
	:: SendMessage(	m_pMgr->GetSafeHwnd(), SENDQUERY , 0, 0);
}

VOID CTransferSearch::OnbtnTRASEARCHDONATOR() 
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

VOID CTransferSearch::OnOK() 
{
	// TODO: Add extra validation here
	
	if(MakeQuery()<0) return;

	SendQueryToParent();

	return;

	CDialog::OnOK();
}

VOID CTransferSearch::OnbtnTRASEARCHCLEAR() 
{
	// TODO: Add your control notification handler code here
	m_pCM->AllControlClear();
}

BOOL CTransferSearch::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			OnOK(); 
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


VOID CTransferSearch::SetFocusOnTitle()
{
	//서명에 setfocus
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_edtTRA_SEARCH_TITLE1);
	if(pWnd) pWnd->SetFocus();
}
HBRUSH CTransferSearch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
