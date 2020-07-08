// SePurchaseSearch.cpp : implementation file
//

#include "stdafx.h"
#include "SePurchaseSearch.h"
#include "..\SE_API\SePurchaseNoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseSearch dialog


CSePurchaseSearch::CSePurchaseSearch(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSePurchaseSearch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSePurchaseSearch)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pMgr = pParent;
	m_strQuery = _T("");
	m_strSEQ_NO = _T("");
	m_strSEQ_YEAR = _T("");
}

CSePurchaseSearch::~CSePurchaseSearch()
{
}

VOID CSePurchaseSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSePurchaseSearch)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSePurchaseSearch, CDialog)
	//{{AFX_MSG_MAP(CSePurchaseSearch)
	ON_BN_CLICKED(IDC_btnPUR_SEARCH, OnbtnSEARCH)
	ON_BN_CLICKED(IDC_btnPUR_SEQ_NO, OnbtnPURSEQNO)
	ON_BN_CLICKED(IDC_btnPUR_SEARCH_CLEAR, OnbtnPURSEARCHCLEAR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseSearch message handlers
BOOL CSePurchaseSearch::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CSePurchaseSearch::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
		
	if(InitESL_Mgr( _T("연속_수서_구입자료관리_검색") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_구입자료관리_검색") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_연속_구입자료관리_검색"));
	
	if(!m_pCM ) {
		AfxMessageBox( _T("CM Error : 연속_수서_구입자료관리_검색") );
		return false;
	}

	//입력 구분을 설정 해준다.
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_INPUT_TYPE);
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("화면") );
		pCombo->InsertString( 1, _T("자료반입") );
		pCombo->InsertString( 2, _T("파일반입") );
		pCombo->InsertString( 3, _T("전체") );
		pCombo->SetCurSel( 0 );
	}

	//자료상태를 설정해준다
	pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_WORKING_STATUS);
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("구입대상") );
		pCombo->InsertString( 1, _T("주문중") );
		pCombo->InsertString( 2, _T("전체") );
		pCombo->SetCurSel( 0 );
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSePurchaseSearch::SendQueryToParent()
{
	:: SendMessage(	m_pMgr->GetSafeHwnd(), SENDQUERY , 0, 0);
}

CString CSePurchaseSearch::MakeRangeQuery(CString strFieldName, CString strData1, CString strData2)
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

CString CSePurchaseSearch::MakeIntRangeQuery(CString strFieldName, CString strData1, CString strData2)
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


VOID CSePurchaseSearch::OnbtnSEARCH()
{
	if(MakeQuery()<0) return;

	SendQueryToParent();

}

INT CSePurchaseSearch::MakeQuery()
{

	CString INPUT_DATE1 = _T("");				m_pCM->GetControlMgrData(_T("입력일자1"),INPUT_DATE1);
	CString INPUT_DATE2 = _T("");				m_pCM->GetControlMgrData(_T("입력일자2"),INPUT_DATE2);		
	CString ORDER_YEAR = _T("");				m_pCM->GetControlMgrData(_T("차수년도"),ORDER_YEAR);
	CString END_DATE1 = _T("");					m_pCM->GetControlMgrData(_T("구독만기일1"),END_DATE1);
	CString END_DATE2 = _T("");					m_pCM->GetControlMgrData(_T("구독만기일2"),END_DATE2);
	CString SEQ_NO1 = _T("");					m_pCM->GetControlMgrData(_T("차수번호1"),SEQ_NO1);

	
	INPUT_DATE1 = TrimAll(INPUT_DATE1);
	INPUT_DATE2 = TrimAll(INPUT_DATE2);
	ORDER_YEAR = TrimAll(ORDER_YEAR);
	END_DATE1 = TrimAll(END_DATE1);
	END_DATE2 = TrimAll(END_DATE2);
	SEQ_NO1 = TrimAll(SEQ_NO1);

	
	CString tmpquery = _T("");
	CString strQuery = _T("");


	//차수테이블과의 조인 조건 
	strQuery = _T("S.ORDER_SEQ_NO = N.SEQ_NO AND S.ORDER_YEAR = N.ACQ_YEAR");


	//입력구분
	CString INPUT_TYPE;
	INT nType = 0;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_INPUT_TYPE);
	if(pCombo) nType = pCombo->GetCurSel();
	
	
	if(nType != 3) //전체가 아니면  
	{
		strQuery += _T(" AND ");
		INPUT_TYPE.Format(_T("%d"),nType);
		tmpquery.Format(_T("S.INPUT_TYPE = '%s'"), INPUT_TYPE);
		strQuery += tmpquery;
	}

	

	//작업상태
	pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_WORKING_STATUS);
	if(pCombo) nType = pCombo->GetCurSel();

	if(nType != 2) //전체 가 아니면 
	{
		strQuery += _T(" AND ");
		INPUT_TYPE.Format(_T("%d"),nType);
		tmpquery.Format(_T("N.WORKING_STATUS = '%s'"), INPUT_TYPE);
		strQuery += tmpquery;

	}


	//차수년도
	m_strSEQ_YEAR = _T("");


	if(!ORDER_YEAR.IsEmpty())
	{
		strQuery += _T(" AND ");
		tmpquery.Format(_T("S.ORDER_YEAR = '%s'"), ORDER_YEAR);
		strQuery += tmpquery;

		//차수년도 저장
		m_strSEQ_YEAR = ORDER_YEAR;
	}
	
	
	//차수번호(일단은 일치검색)
	m_strSEQ_NO = _T("");

	if(!SEQ_NO1.IsEmpty())
	{

		strQuery += _T(" AND ");
		tmpquery.Format(_T("S.ORDER_SEQ_NO = '%s'"), SEQ_NO1);
		strQuery += tmpquery;
	
		//차수 번호 저장
		m_strSEQ_NO = SEQ_NO1;
	}




	if(INPUT_DATE1.IsEmpty() && !INPUT_DATE2.IsEmpty())
	{
		AfxMessageBox(_T("입력일자 범위의 뒷부분만 입력 되었습니다!"));
		return -1;
	}



	//입력일
	tmpquery.Empty();
	tmpquery = MakeRangeQuery(_T("S.INPUT_DATE"),INPUT_DATE1,INPUT_DATE2);
	if(!tmpquery.IsEmpty())
	{
		strQuery += _T(" AND ");
		strQuery += tmpquery;
	}

	
	if(END_DATE1.IsEmpty() && !END_DATE2.IsEmpty())
	{
		AfxMessageBox(_T("구독만기일 범위의 뒷부분만 입력 되었습니다!"));
		return -1;
	}
	

	//구독만기일
	tmpquery.Empty();
	tmpquery = MakeRangeQuery(_T("S.END_DATE"),END_DATE1,END_DATE2);
	if(!tmpquery.IsEmpty())
	{

		strQuery += _T(" AND ");
		strQuery += tmpquery;
	}


	m_strQuery = strQuery;
	
	return 0;

}

CString CSePurchaseSearch::TrimAll(CString strData)
{
	strData.TrimLeft();
	strData.TrimRight();

	return strData;
}

VOID CSePurchaseSearch::OnbtnPURSEQNO() 
{
	// TODO: Add your control notification handler code here
	// 차수 번호 보기 
	CSePurchaseNoDlg dlg(this);
	dlg.m_nOpenMode = -1;

	if(dlg.DoModal() != IDOK) return;
	
	m_pCM->SetControlMgrData(_T("차수년도"),dlg.m_strYear);
	m_pCM->SetControlMgrData(_T("차수번호1"),dlg.m_strPurchaseNo);
	

	//차수 작업 상태 설정 
	CString strWorking_status;
	strWorking_status = dlg.m_strWorking_status;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_WORKING_STATUS);
	if(pCombo)
	{
		if(_T("0") == strWorking_status) pCombo->SetCurSel(0);
		else if(_T("1") == strWorking_status) pCombo->SetCurSel(1);
	}

	//입력 구분은 무조건 _T("전체")로 설정
	pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_INPUT_TYPE);
	if(pCombo) pCombo->SetCurSel(3);
	

	//자동 검색
	OnbtnSEARCH();

}

BOOL CSePurchaseSearch::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			OnbtnSEARCH();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CSePurchaseSearch::OnbtnPURSEARCHCLEAR() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_INPUT_TYPE);
	if(pCombo)
	{
		pCombo->SetCurSel( 0 );
	}

	m_pCM->AllControlClear();
}

HBRUSH CSePurchaseSearch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
