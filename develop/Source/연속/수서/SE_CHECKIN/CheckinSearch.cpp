// CheckinSearch.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinSearch.h"
#include "..\..\..\공통\COMMON_DONATOR_MANAGER\DonatorNameGetterDlg.h"
#include "..\..\..\공통\COMMON_DONATOR_MANAGER\DonatorNameGetter_Search.h"
#include "..\SE_API\SeApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinSearch dialog


CCheckinSearch::CCheckinSearch(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckinSearch)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMgr = pParent;
	m_nACQ_TYPE = 1; //default
	m_pCM = NULL;
	m_strQuery = _T("");
}

CCheckinSearch::~CCheckinSearch()
{
}

VOID CCheckinSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinSearch)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CCheckinSearch::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BEGIN_MESSAGE_MAP(CCheckinSearch, CDialog)
	//{{AFX_MSG_MAP(CCheckinSearch)
	ON_BN_CLICKED(IDC_btnCHE_DONATOR_SEARCH, OnbtnCHEDONATORSEARCH)
	ON_BN_CLICKED(IDC_btnCHE_SEARCH_CLEAR, OnbtnCHESEARCHCLEAR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinSearch message handlers

VOID CCheckinSearch::OnOK() 
{
	// TODO: Add extra validation here
	if(MakeQuery()<0) return;

	SendQueryToParent();

	return;

	CDialog::OnOK();
}

BOOL CCheckinSearch::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	if( InitESL_Mgr( _T("연속_수서_체크인_검색") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_서지정보관리_검색") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_연속_체크인_검색"));
	if( m_pCM == NULL )
	{
		AfxMessageBox(_T("CM Error : CM_연속_체크인_검색"));
		return FALSE;
	}		
	
	//------------------------------------------------------------
	// 2005-10-19 황정영: 육사이면 검색조건에 연속구분, 자료실을 추가한다.
	//------------------------------------------------------------
	CString str64;
	GetManageValue(_T("기타") , _T("공통") , _T("육사") , _T("육사") , str64);
	
	if( str64 == _T("Y") )
	{
		GetDlgItem(IDC_edtCHE_ACQYEAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ACQYEAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtCHE_RECEIPTNO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_RECEIPTNO)->ShowWindow(SW_HIDE);
		CString strTime;
		CTime time = CTime::GetCurrentTime();
		strTime.Format(_T("%04d/%02d/%02d"),time.GetYear(), time.GetMonth(), time.GetDay());
		m_pCM->SetControlMgrData(_T("입수지연일2"),strTime);
		GetDlgItem(IDC_edtCHE_ACQ_DELAY_DATE2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_ACQ_DELAY)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_cmbCHE_DIV_CODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbCHE_LOCATION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LOCATION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DIV_CODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtCHE_ACQYEAR)->MoveWindow(311+53, 104, 56, 20);
		GetDlgItem(IDC_STATIC_ACQYEAR)->MoveWindow(254+53, 108, 56, 20);
		GetDlgItem(IDC_edtCHE_RECEIPTNO)->MoveWindow(421+53, 104, 56, 20);
		GetDlgItem(IDC_STATIC_RECEIPTNO)->MoveWindow(370+53, 108, 51, 20);
		GetDlgItem(IDC_edtCHE_ACQ_DELAY_DATE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ACQ_DELAY)->ShowWindow(SW_HIDE);
	}
	
	//수입구분에 따른 콘트롤 설정
	if(1 == m_nACQ_TYPE)
	{
		GetDlgItem(IDC_btnCHE_DONATOR_SEARCH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtCHE_DONATOR)->ShowWindow(SW_HIDE);
	}
	else
	{
		if( str64 == _T("Y") )
		{
			CRect rc, rc2;
			this->GetWindowRect(rc);
			MoveWindow(rc.left,rc.top,rc.Width(),rc.Height()+28);
			
			GetDlgItem(IDC_STATIC_GROUP_BOX)->GetClientRect(rc2);
			GetDlgItem(IDC_STATIC_GROUP_BOX)->MoveWindow(rc2.left,rc2.top,rc2.Width(),rc2.Height()+28);
			
			GetDlgItem(IDC_cmbCHE_LOCATION)->MoveWindow(75, 184, 166, 20);
			GetDlgItem(IDC_STATIC_LOCATION)->MoveWindow(15, 190, 56, 20);
			
		}
		else
		{
			GetDlgItem(IDC_cmbCHE_LOCATION)->MoveWindow(312, 156, 166, 20);
			GetDlgItem(IDC_STATIC_LOCATION)->MoveWindow(254, 162, 56, 20);	
		}

		GetDlgItem(IDC_cmbCHE_PROVIDER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcCHE_PROVIDER)->SetWindowText(_T("기증자"));
		GetDlgItem(IDC_cmbCHE_DIV_CODE)->MoveWindow(312, 126, 166, 20);
		GetDlgItem(IDC_STATIC_DIV_CODE)->MoveWindow(254, 132, 56, 20);
		GetDlgItem(IDC_edtCHE_ACQYEAR)->MoveWindow(311+53, 128, 56, 20);
		GetDlgItem(IDC_STATIC_ACQYEAR)->MoveWindow(254+53, 132, 56, 20);
		GetDlgItem(IDC_edtCHE_RECEIPTNO)->MoveWindow(421+53, 128, 56, 20);
		GetDlgItem(IDC_STATIC_RECEIPTNO)->MoveWindow(370+53, 132, 51, 20);
//		GetDlgItem(IDC_cmbCHE_LOCATION)->MoveWindow(312, 156, 166, 20);
//		GetDlgItem(IDC_STATIC_LOCATION)->MoveWindow(254, 162, 56, 20);
	}
	m_pCM->AllControlDisplay();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CCheckinSearch::MakeQuery()
{

	CString str64;
	GetManageValue(_T("기타") , _T("공통") , _T("육사") , _T("육사") , str64);
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
	
	CString ACQ_DELAY_DATE1		= _T("");		m_pCM->GetControlMgrData(_T("입수예정일"),ACQ_DELAY_DATE1);
	CString ACQ_DELAY_DATE2		= _T("");		m_pCM->GetControlMgrData(_T("입수지연일2"),ACQ_DELAY_DATE2);
	
	//2005-11-08 접수번호 조건 추가 황정영
	CString ACQYEAR				= _T("");		m_pCM->GetControlMgrData(_T("수입년도"),ACQYEAR);
	CString RECEIPTNO			= _T("");		m_pCM->GetControlMgrData(_T("접수번호"),RECEIPTNO);
		
	PROVIDER_CODE = TrimAll(PROVIDER_CODE);
	DONATOR_NAME = TrimAll(DONATOR_NAME);
	ISSN1 = TrimAll(ISSN1);
	ISSN2 = TrimAll(ISSN2);
	PUB_FREQ = TrimAll(PUB_FREQ);
	TITLE1 = TrimAll(TITLE1);
	TITLE2 = TrimAll(TITLE2);
	PUBLISHER1 = TrimAll(PUBLISHER1);
	PUBLISHER2 = TrimAll(PUBLISHER2);
	ACQ_DELAY_DATE1 = TrimAll(ACQ_DELAY_DATE1);
	ACQ_DELAY_DATE2 = TrimAll(ACQ_DELAY_DATE2);
	ACQYEAR = TrimAll(ACQYEAR);
	RECEIPTNO = TrimAll(RECEIPTNO);
	


	//색인 범위 검색의 경우 
	//왼쪽만 입력  - [ IDX_ ] 형식에 catsearch 
	//오른쪽만 입력 - error!
	//양쪽다 입력 - [ IDX_I ] 형식에 범위 검색 
	
	CString strQuery = _T("");
	CString tmpquery = _T("");

	//수입 구분 조건 

	if(1 == m_nACQ_TYPE) {
		strQuery += _T("I.REC_KEY = S.REC_KEY AND ");
		strQuery += _T("I.REC_KEY = P.SPECIES_KEY");
		if(!PROVIDER_CODE.IsEmpty())
		{
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" P.PROVIDER_CODE = '%s'"), PROVIDER_CODE);
			strQuery += tmpquery;
		}
		if(ACQYEAR.IsEmpty() == FALSE && RECEIPTNO.IsEmpty() == FALSE) 
		{
			tmpquery.Format(_T(" AND I.REC_KEY IN")
						_T(" (SELECT B.SPECIES_KEY") 
						_T(" FROM SE_BOOK_TBL B")
						_T(" WHERE B.ACQ_CODE = '1' AND")
						_T(" B.ACQ_YEAR = '%s' AND")
						_T(" B.RECEIPT_NO = '%s')")
						, ACQYEAR, RECEIPTNO);
			strQuery += tmpquery;
		}

	} 
	else if(2 == m_nACQ_TYPE) {
		strQuery += _T("I.REC_KEY = S.REC_KEY AND ");
		strQuery += _T("I.REC_KEY = D.SPECIES_KEY");
		
		if(!DONATOR_NAME.IsEmpty())
		{
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" D.DONATOR_NAME = '%s'"), DONATOR_NAME);
			strQuery += tmpquery;
		}
		
		if(ACQYEAR.IsEmpty() == FALSE && RECEIPTNO.IsEmpty() == FALSE) 
		{
			tmpquery.Format(_T(" AND I.REC_KEY IN")
						_T(" (SELECT B.SPECIES_KEY") 
						_T(" FROM SE_BOOK_TBL B")
						_T(" WHERE B.ACQ_CODE = '2' AND")
						_T(" B.ACQ_YEAR = '%s' AND")
						_T(" B.RECEIPT_NO = '%s')")
						, ACQYEAR, RECEIPTNO);
			strQuery += tmpquery;
		}
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
	
	//------------------------------------------------------------
	// 64일때 조건을 추가하여 검색 한다.
	//------------------------------------------------------------
	CString SE_DIV_CODE, LOCATION;
	
	if( str64 == 'Y')
	{
		SE_DIV_CODE = _T("");					m_pCM->GetControlMgrData(_T("연속구분"),SE_DIV_CODE);
		LOCATION    = _T("");					m_pCM->GetControlMgrData(_T("자료실"),LOCATION);

		SE_DIV_CODE = TrimAll(SE_DIV_CODE);
		LOCATION    = TrimAll(LOCATION);

		if(SE_DIV_CODE.IsEmpty() == FALSE)
		{
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" I.USER_DEFINED_CODE1 = '%s'"), SE_DIV_CODE);
			strQuery += tmpquery;
		}
		
		if(LOCATION.IsEmpty() == FALSE)
		{
			strQuery += _T(" AND ");
			tmpquery.Format(_T(" I.PLACE_INFO = '%s'"), LOCATION);
			strQuery += tmpquery;
		}			
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


	if(!ACQ_DELAY_DATE1.IsEmpty())
	{
		if( str64 == _T("Y") )
		{
			if(!ACQ_DELAY_DATE2.IsEmpty())
			{
				if(1 == m_nACQ_TYPE) 
				{
					strQuery += _T(" AND ");
					tmpquery.Format(_T("GET_EXP_START_DATE(P.ACQ_EXP_TYPE,P.ACQ_EXP_KEY) BETWEEN '%s' AND '%s'"),ACQ_DELAY_DATE1, ACQ_DELAY_DATE2);
					strQuery += tmpquery;
					
				}
				else if(2 == m_nACQ_TYPE) 
				{
					strQuery += _T(" AND ");
					tmpquery.Format(_T("GET_EXP_START_DATE(D.ACQ_EXP_TYPE,D.ACQ_EXP_KEY) BETWEEN '%s' AND '%s'"),ACQ_DELAY_DATE1, ACQ_DELAY_DATE2);
					strQuery += tmpquery;
					
				}
			}
			else
			{
				if(1 == m_nACQ_TYPE) 
				{
					strQuery += _T(" AND ");
					tmpquery.Format(_T("GET_EXP_START_DATE(P.ACQ_EXP_TYPE,P.ACQ_EXP_KEY) >= '%s'"),ACQ_DELAY_DATE1);
					strQuery += tmpquery;
					
				}
				else if(2 == m_nACQ_TYPE) 
				{
					strQuery += _T(" AND ");
					tmpquery.Format(_T("GET_EXP_START_DATE(D.ACQ_EXP_TYPE,D.ACQ_EXP_KEY) >= '%s'"),ACQ_DELAY_DATE1);
					strQuery += tmpquery;					
				}
			}
		}
		else
		{
			if(1 == m_nACQ_TYPE) 
			{
				strQuery += _T(" AND ");
				tmpquery.Format(_T("GET_EXP_START_DATE(P.ACQ_EXP_TYPE,P.ACQ_EXP_KEY) <= '%s'"),ACQ_DELAY_DATE1);
				strQuery += tmpquery;
			
			}
			else if(2 == m_nACQ_TYPE) 
			{
				strQuery += _T(" AND ");
				tmpquery.Format(_T("GET_EXP_START_DATE(D.ACQ_EXP_TYPE,D.ACQ_EXP_KEY) <= '%s'"),ACQ_DELAY_DATE1);
				strQuery += tmpquery;

			}
		}
	}
	else
	{
		if(str64 == _T("Y"))
		{
			if( !ACQ_DELAY_DATE2.IsEmpty() )
			{
				if(1 == m_nACQ_TYPE) 
				{
					strQuery += _T(" AND ");
					tmpquery.Format(_T("GET_EXP_START_DATE(P.ACQ_EXP_TYPE,P.ACQ_EXP_KEY) <= '%s'"),ACQ_DELAY_DATE2);
					strQuery += tmpquery;
					
				}
				else if(2 == m_nACQ_TYPE) 
				{
					strQuery += _T(" AND ");
					tmpquery.Format(_T("GET_EXP_START_DATE(D.ACQ_EXP_TYPE,D.ACQ_EXP_KEY) <= '%s'"),ACQ_DELAY_DATE2);
					strQuery += tmpquery;
				}
			}
		}
	}
	
	m_strQuery = strQuery;

	return 0;

}

CString CCheckinSearch::MakeIDXRangeQuery(CString strFieldName, CString strData1, CString strData2)
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

CString CCheckinSearch::MakeRangeQuery(CString strFieldName, CString strData1, CString strData2)
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

CString CCheckinSearch::TrimAll(CString strData)
{
	strData.TrimLeft();
	strData.TrimRight();

	return strData;
}

VOID CCheckinSearch::SendQueryToParent()
{
	:: SendMessage(	m_pMgr->GetSafeHwnd(), SENDQUERY , 0, 0);
}

VOID CCheckinSearch::OnbtnCHEDONATORSEARCH() 
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

VOID CCheckinSearch::OnbtnCHESEARCHCLEAR() 
{
	// TODO: Add your control notification handler code here
	m_pCM->AllControlClear();
	
}

BOOL CCheckinSearch::PreTranslateMessage(MSG* pMsg) 
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

VOID CCheckinSearch::SetFocusOnTitle()
{
	//서명에 setfocus
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_edtCHE_TITLE1);
	if(pWnd) pWnd->SetFocus();
}

HBRUSH CCheckinSearch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
