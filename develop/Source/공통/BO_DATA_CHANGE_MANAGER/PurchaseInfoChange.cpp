// PurchaseInfoChange.cpp : implementation file
//

#include "stdafx.h"
#include "PurchaseInfoChange.h"
#include "EXCHANGERATE_MGR.h"
#include "DataChangeManager.h"
#include "PurchaseInfoChangeCondition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPurchaseInfoChange dialog


CPurchaseInfoChange::CPurchaseInfoChange(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CPurchaseInfoChange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPurchaseInfoChange)
	m_bCurrencyCode = TRUE;
	m_bDiscountRate = TRUE;
	m_bExchangeRate = TRUE;
	m_bProvider = TRUE;
	m_bRemark = TRUE;
	m_bBookCount = TRUE;
	//20070604 ADD PJW 학처 체크박스추가
	m_bApplicant = TRUE;

	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pDM_BOOK = NULL;
	m_pCM = NULL;
	m_pMainGrid = NULL;
	m_nSpeciesType = 0;
}

CPurchaseInfoChange::~CPurchaseInfoChange()
{
}

VOID CPurchaseInfoChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPurchaseInfoChange)
	DDX_Control(pDX, IDC_cmbPC_EXCHANGERATE, m_Price);
	DDX_Check(pDX, IDC_chkCURRENCY_CODE, m_bCurrencyCode);
	DDX_Check(pDX, IDC_chkDISCOUNT_RATE, m_bDiscountRate);
	DDX_Check(pDX, IDC_chkPROVIDER, m_bProvider);
	DDX_Check(pDX, IDC_chkREMARK, m_bRemark);
	DDX_Check(pDX, IDC_chkBOOKCOUNT, m_bBookCount);
	//20070604 ADD PJW 학처 체크박스추가
	DDX_Check(pDX, IDC_chkAPPLICANT, m_bApplicant);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPurchaseInfoChange, CDialog)
	//{{AFX_MSG_MAP(CPurchaseInfoChange)
	ON_BN_CLICKED(IDC_chkCURRENCY_CODE, OnchkCURRENCYCODE)
	ON_BN_CLICKED(IDC_chkDISCOUNT_RATE, OnchkDISCOUNTRATE)
	ON_BN_CLICKED(IDC_chkPROVIDER, OnchkPROVIDER)
	ON_BN_CLICKED(IDC_chkREMARK, OnchkREMARK)
	ON_CBN_SELCHANGE(IDC_cmbPC_CURRENCY_CLASS, OnSelchangecmbPCCURRENCYCLASS)
	ON_BN_CLICKED(IDC_btnPC_EXCHANGE_RATE_MGR, OnbtnPCEXCHANGERATEMGR)
	ON_EN_KILLFOCUS(IDC_edtPC_ORDER_DISCOUNT_RATE, OnKillfocusedtPCORDERDISCOUNTRATE)
	ON_BN_CLICKED(IDC_chkBOOKCOUNT, OnchkBOOKCOUNT)
	ON_BN_CLICKED(IDC_chkAPPLICANT, OnchkAPPLICANT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPurchaseInfoChange message handlers

BOOL CPurchaseInfoChange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_구입정보변경")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM설정
	m_pCM = FindCM(_T("CM_구입정보변경"));		
	
	OnSelchangecmbPCCURRENCYCLASS();
	OnchkCURRENCYCODE();
	OnchkDISCOUNTRATE();
	OnchkPROVIDER();
	OnchkREMARK();
	ShowControlBySpeciesType();
	OnchkBOOKCOUNT();
	//20070604 ADD PJW 학처 체크박스추가
	OnchkAPPLICANT();

//	CESL_Mgr * m_pParentMgr;
	CString str64;
//	m_pParentMgr->GetManageValue( _T("기타"), _T("공통"), _T("육사"), _T("육사"), str64 );
	GetManageValue( _T("기타"), _T("공통"), _T("육사"), _T("육사"), str64 );
	if ( str64 != _T("Y") )
	{
		
		//GetDlgItem(IDC_cmbPC_CURRENCY_CLASS)->EnableWindow(FALSE);
		//GetDlgItem(IDC_chkAPPLICANT)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_cmbPC_CURRENCY_CLASS)->EnableWindow(FALSE);
		//20070604 ADD PJW 학처 체크박스추가
		GetDlgItem(IDC_chkAPPLICANT)->ShowWindow(FALSE);
		GetDlgItem(IDC_cAPPLICAND_DEPT)->ShowWindow(FALSE);
	}

	EnableThemeDialogTexture(GetSafeHwnd());	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CPurchaseInfoChange::OnchkCURRENCYCODE() 
{
	m_bCurrencyCode = !m_bCurrencyCode;
	m_bExchangeRate = !m_bExchangeRate;

	if(m_bCurrencyCode) 
	{
		GetDlgItem(IDC_cmbPC_CURRENCY_CLASS)->EnableWindow(TRUE);
		GetDlgItem(IDC_cmbPC_CURRENCY_CLASS_PRE)->EnableWindow(TRUE);
		GetDlgItem(IDC_cmbPC_EXCHANGERATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnPC_EXCHANGE_RATE_MGR)->EnableWindow(TRUE);	
	}
	else 
	{
		GetDlgItem(IDC_cmbPC_CURRENCY_CLASS)->EnableWindow(FALSE);
		GetDlgItem(IDC_cmbPC_CURRENCY_CLASS_PRE)->EnableWindow(FALSE);
		GetDlgItem(IDC_cmbPC_EXCHANGERATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnPC_EXCHANGE_RATE_MGR)->EnableWindow(FALSE);	
	}
	UpdateData(FALSE);
}


VOID CPurchaseInfoChange::OnchkDISCOUNTRATE() 
{
	// TODO: Add your control notification handler code here
	m_bDiscountRate = !m_bDiscountRate;
	if(m_bDiscountRate) GetDlgItem(IDC_edtPC_ORDER_DISCOUNT_RATE)->EnableWindow(TRUE);
	else GetDlgItem(IDC_edtPC_ORDER_DISCOUNT_RATE)->EnableWindow(FALSE);	
	UpdateData(FALSE);
}


VOID CPurchaseInfoChange::OnchkPROVIDER() 
{
	// TODO: Add your control notification handler code here
	m_bProvider = !m_bProvider;
	if(m_bProvider) GetDlgItem(IDC_cmbPC_PROVIDER)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbPC_PROVIDER)->EnableWindow(FALSE);	
	UpdateData(FALSE);
}

VOID CPurchaseInfoChange::OnchkREMARK() 
{
	// TODO: Add your control notification handler code here
	m_bRemark = !m_bRemark;
	if(m_bRemark) GetDlgItem(IDC_edtPC_REMARKS)->EnableWindow(TRUE);
	else GetDlgItem(IDC_edtPC_REMARKS)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

VOID CPurchaseInfoChange::OnchkBOOKCOUNT() 
{
	// TODO: Add your control notification handler code here
	m_bBookCount = !m_bBookCount;
	if(m_bBookCount) GetDlgItem(IDC_edtBOOKCOUNT)->EnableWindow(TRUE);
	else GetDlgItem(IDC_edtBOOKCOUNT)->EnableWindow(FALSE);
	UpdateData(FALSE);	
}

//20070604 ADD PJW 학처 체크박스추가
VOID CPurchaseInfoChange::OnchkAPPLICANT()
{
	// TODO: Add your control notification handler code here
	m_bApplicant = !m_bApplicant;
	if(m_bApplicant) GetDlgItem(IDC_cAPPLICAND_DEPT)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cAPPLICAND_DEPT)->EnableWindow(FALSE);
	UpdateData(FALSE);	
}


VOID CPurchaseInfoChange::OnOK() 
{
	UpdateData(TRUE);  // 2005-02-03
	INT ids = ValidateData();
	if(ids < 0) return ;

	CString strExchangeRate;
	CComboBox *pCombo;

	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbPC_EXCHANGERATE );
	pCombo->GetWindowText( strExchangeRate );

	CDataChangeManager mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM_BOOK);
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("Grid"), (DWORD*)m_pMainGrid);
	mgr.SetExchangeRate( strExchangeRate );

	CPurchaseInfoChangeCondition cont;
	cont.SetCurrencyCode(m_bCurrencyCode);
	cont.SetDiscountRate(m_bDiscountRate);
	cont.SetExchangeRate(m_bExchangeRate);
	cont.SetProvider(m_bProvider);
	cont.SetRemark(m_bRemark);
	cont.SetBookCount(m_bBookCount);
	//20070604 ADD PJW 학처 체크박스추가
	cont.SetApplicant(m_bApplicant);

	
	mgr.SetTempPtr(_T("Condition"), (DWORD*)&cont);
	ids = mgr.SPFExecutePath(_T("구입정보변경"));
	if(ids < 0)
	{
		//에러처리
		ESLAfxMessageBox(_T("선정하신 자료의 구입정보를 변경할 수 없습니다."));
		CDialog::OnCancel();
		return ;
	}	
	
	ESLAfxMessageBox(_T("선정하신 자료의 구입정보를 변경하였습니다."));
	CDialog::OnOK();	
}

VOID CPurchaseInfoChange::OnSelchangecmbPCCURRENCYCLASS() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	if(!m_bCurrencyCode) return ;
	CDataChangeManager mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	
	// 최근 환율 정보 대신에 적용여부가 Y인 환율정보를 추출한다.
	// 2005-09-28 강봉수 -> 차장님 요구
	//
	INT ids = mgr.SPFExecutePath(_T("최근환율정보추출"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정한 화폐구분에 환율정보가 없습니다. 환율정보를 설정해 주십시요."));
		GetDlgItem(IDC_btnPC_EXCHANGE_RATE_MGR)->SetFocus();
		m_Price.ResetContent();
		return;
	}
	CString strExchangeRate;
	mgr.GetTempData(_T("ExchangeRate"), strExchangeRate);
	

	//환율정보 반영
	//
	m_Price.ResetContent();

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	CString year;

	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	year = result;

	CString strCurrencyCode;
	m_pCM->GetControlMgrData(_T("BP_화폐구분"), strCurrencyCode);
	
	CString query, startdate, enddate, key;
	query.Format(_T("select rec_key from cd_code_tbl where class = 23 and code like '%s%%'"), strCurrencyCode); 
	dm.GetOneValueQuery(query, key);
	startdate.Format(_T("%s/%s/%s"), year, _T("01"), _T("01"));
	enddate.Format(_T("%s/%s/%s"), year, _T("12"), _T("31"));
	query.Format(_T("SELECT EXCHANGE_RATE FROM CO_EXCHANGE_RATE_TBL WHERE USER_CODE_MNG_KEY = %s AND INPUT_DATE >= '%s' AND INPUT_DATE <= '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), key, startdate, enddate);
//DEL 	query.Format(_T("SELECT EXCHANGE_RATE FROM CO_EXCHANGE_RATE_TBL WHERE USER_CODE_MNG_KEY = %s AND INPUT_DATE >= '%s' AND INPUT_DATE <= '%s'"), key, startdate, enddate);
	ids = dm.RestructDataManager(query);

	// 적용여부에 'Y'가 된 환율을 적용 한다.
	// 2005-09-28 강봉수 - 차장님 요구
	//		
	for ( INT nIdx = 0 ;nIdx < dm.GetRowCount() ; nIdx++ )
	{
		m_Price.InsertString( nIdx, dm.GetCellData( nIdx, 0 ));
		if(dm.GetCellData(nIdx,0) == strExchangeRate) ids = nIdx;
	}

	if(strExchangeRate.IsEmpty()) m_Price.SetCurSel(0);
	else
	{
		m_Price.SetCurSel(ids);
	}	
	//
	m_pCM->SetControlMgrData(_T("BP_환율"), strExchangeRate);
}

VOID CPurchaseInfoChange::OnbtnPCEXCHANGERATEMGR() 
{
	// TODO: Add your control notification handler code here
	CString strCurrencyCode;
	m_pCM->GetControlMgrData(_T("BP_화폐구분"), strCurrencyCode);
	if(m_bCurrencyCode == 0  || strCurrencyCode.IsEmpty() || strCurrencyCode == _T("") )
	{
		ESLAfxMessageBox(_T("화폐구분을 선정해 주십시요."));
		return ;
	}
	CEXCHANGERATE_MGR dlg(this, strCurrencyCode);
	CString exchangerate;
	if(IDOK == dlg.DoModal())
	{
		exchangerate = dlg.m_exchangerate;
		m_pCM->SetControlMgrData(_T("BP_환율"), exchangerate);
	}

	OnSelchangecmbPCCURRENCYCLASS();
	return ;	
}

INT CPurchaseInfoChange::ValidateData()
{
	CString strTmpData, strDiscRate;
	//if(!m_bDiscountRate) return 0;

	// 2005-02-02 할인율이나 환율이 체크가 되어 있을 경우 책수도 체크가 되어 있으면 변경할 수 없다는 메시지 띄우고 리턴한다.
	if( (m_bDiscountRate || m_bExchangeRate) && m_bBookCount)
	{
		ESLAfxMessageBox(_T("할인율/환율과 책수는 동시에 변경할 수 없습니다."));
		return -1;
	}

	// 2005-02-02 책수가 1보다 작으면 안된다.
	if(m_bBookCount)
	{
		m_pCM->GetControlMgrData(_T("BB_책수"), strTmpData);
		if(strTmpData.IsEmpty())
		{
			ESLAfxMessageBox(_T("책수를 입력하십시오."));
			return -1;
		}
		if(_ttoi(strTmpData) < 1)
		{
			ESLAfxMessageBox(_T("책수는 1보다 작을 수 없습니다."));
			m_pCM->SetControlMgrData(_T("BB_책수"), _T("1"));
			return -1;
		}
	}
	
	if(m_bDiscountRate)
	{
		m_pCM->GetControlMgrData(_T("BP_할인율"), strDiscRate);
		if(strDiscRate.IsEmpty()) 
		{
			ESLAfxMessageBox(_T("할인율을 입력하십시오."));
			return -1;
		}
		INT nPos = strDiscRate.Find(_T("."));
		if(nPos > 0) strTmpData = strDiscRate.Left(nPos);
		if(strTmpData.GetLength() > 3)
		{
			ESLAfxMessageBox(_T("할인율정보는 소수점위 자료가 3자리 이상일 수 없습니다."));
			return -1;
		}

		DOUBLE dTmpData;
		//dTmpData = (DOUBLE)atof(strDiscRate.GetBuffer(0));
		TCHAR* pEndPtr = NULL;
		dTmpData = _tcstod(strDiscRate.GetBuffer(0), &pEndPtr);

		if(dTmpData > 100.0)
		{
			ESLAfxMessageBox(_T("할인율 정보가 100보다 클 수 없습니다."));
			m_pCM->SetControlMgrData(_T("BP_할인율"), _T("0"));
			return -1;
		}
	}

	if(m_bExchangeRate)
	{
//		m_pCM->GetControlMgrData(_T("BP_환율"), strTmpData);
//		if(strTmpData.IsEmpty()) 
//		{
//			ESLAfxMessageBox(_T("환율을 입력하십시오."));
//			return -1;
//		}
		//수정
		CString strTmp;
		CComboBox *pCombo;

		pCombo = ( CComboBox * )GetDlgItem( IDC_cmbPC_EXCHANGERATE );
		pCombo->GetWindowText( strTmp );
		if(strTmp.IsEmpty()) 
		{
			ESLAfxMessageBox(_T("환율을 입력하십시오."));
			return -1;
		}

//		for ( INT nIdx = 0 ; nIdx < m_pDM->GetRowCount() ; nIdx++ ) 
//		{
//			m_pDM->SetCellData( _T("BP_할인율"), strTmp, nIdx ) ;
//		}
		//BP_할인율
	}

	return 0;
}



VOID CPurchaseInfoChange::OnKillfocusedtPCORDERDISCOUNTRATE() 
{
	/*
	UpdateData(FALSE);
	ValidateData();
	*/
}

BOOL CPurchaseInfoChange::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_CHAR && pMsg->hwnd == GetDlgItem(IDC_edtPC_ORDER_DISCOUNT_RATE)->GetSafeHwnd() || pMsg->message == WM_CHAR && pMsg->hwnd == GetDlgItem(IDC_edtBOOKCOUNT)->GetSafeHwnd() )
    {
		INT nVirtKey = (INT) pMsg->wParam;
		if ((nVirtKey < '0' || nVirtKey > '9') && nVirtKey != '.' && nVirtKey != 8)
		{
			ESLAfxMessageBox(_T("숫자만 입력하실 수 있습니다.")); 
            return TRUE;
		}
    }	
	
	return CDialog::PreTranslateMessage(pMsg);
}



VOID CPurchaseInfoChange::ShowControlBySpeciesType()
{
	if(1 == m_nSpeciesType)
	{
		GetDlgItem(IDC_chkBOOKCOUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_edtBOOKCOUNT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_chkBOOKCOUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_edtBOOKCOUNT)->EnableWindow(FALSE);
	}
}

HBRUSH CPurchaseInfoChange::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
