// PurchaseInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PurchaseInfoDlg.h"
// 2004.09.01
#include "..\BO_DATA_CHANGE_MANAGER\EXCHANGERATE_MGR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPurchaseInfoDlg dialog
// 생성자
CPurchaseInfoDlg::CPurchaseInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CInfoDlg(pParent, CPurchaseInfoDlg::IDD)
{
	//==================================
	//20080616 UPDATE BY PJW : 클레스 위저드 실행시 에러나서 밖으로 뺌
	m_strPriceChar = _T("");
	//==================================
	//{{AFX_DATA_INIT(CPurchaseInfoDlg)
	//m_strPriceChar = _T("");
	//}}AFX_DATA_INIT
	//==================================
	m_pCM = NULL;
	m_pCmbPriceChar	= NULL;
	m_dOrderPrice = 0.0;
	m_strDisplayOrderPrice = _T("0");
}

CPurchaseInfoDlg::~CPurchaseInfoDlg()
{
	
}

VOID CPurchaseInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPurchaseInfoDlg)
	DDX_CBString(pDX, IDC_cmbBSFORM_PRICE_CHARACTER, m_strPriceChar);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPurchaseInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CPurchaseInfoDlg)
	ON_CBN_SELCHANGE(IDC_cmbBSFORM_PRICE_TYPE, OnSelchangecmbBSFORMPRICETYPE)
	ON_EN_KILLFOCUS(IDC_edtBSFORM_PRICE_DC, OnKillfocusedtBSFORMPRICEDC)
	ON_EN_KILLFOCUS(IDC_edtBSFORM_PRICE, OnKillfocusedtBSFORMPRICE)
	ON_CBN_SELCHANGE(IDC_cmbBSFORM_CURRENCY_CLASS, OnSelchangecmbBSFORMCURRENCYCLASS)
	ON_BN_CLICKED(IDC_btnEXCHANGE_RATE, OnbtnEXCHANGERATE)
	ON_CBN_SELCHANGE(IDC_cmbBSFORM_PRICE_CHARACTER, OnSelchangecmbBSFORMPRICECHARACTER)
	ON_CBN_KILLFOCUS(IDC_cmbBSFORM_PRICE_CHARACTER, OnKillfocuscmbBSFORMPRICECHARACTER)
	ON_MESSAGE(SET_TAB_ORDER, OnSetTabOrder)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPurchaseInfoDlg message handlers

BOOL CPurchaseInfoDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

// 초기화
BOOL CPurchaseInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// SM 설정
	if( InitESL_Mgr(_T("K2UPGRADE_단행_종_구입정보")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		

	// CM/DM 설정
	m_pCM = FindCM(_T("CM_단행_종_구입정보_UP"));	
	if( m_pCM == NULL )
	{
		ESLAfxMessageBox(_T("[CM_단행_종_구입정보_UP]을 설정할 수 없습니다."));
		return FALSE;
	}

	// 가격성격 콤보 설정
	m_pCmbPriceChar = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_가격성격"));
	if( m_pCmbPriceChar == NULL )	return FALSE;

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  
}

BOOL CPurchaseInfoDlg::Init()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 가격 검사
// [ 미니스펙 ]
// 1. CM에서 가격과 화폐구분값을 읽어 온다.
// 2. 가격에 소수점이 있는지 확인한다.
// 3. 한국화폐일경우에는 가격에는 소수점이 적용되지 않게 한다.
/////////////////////////////////////////////////////////////////////////////
INT CPurchaseInfoDlg::ValidateData()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT nPos = 0;
	CString strPrice = _T("") ;
	CString strCurrencyCode = _T("");		
	CString strTmpData = _T("");
	

	// 가격 검사
	//=======================================================================
	// 1. CM에서 가격과 화폐구분값을 읽어 온다.
	//=======================================================================
	m_pCM->GetControlMgrData(_T("BP_가격"), strPrice);
	m_pCM->GetControlMgrData(_T("BP_화폐구분"), strCurrencyCode,-1,0);	

	//=======================================================================
	// 2. 가격에 소수점이 있는지 확인한다.
	// 3. 한국화폐일경우에는 가격에는 소수점이 적용되지 않게 한다.
	//=======================================================================
	
	nPos = strPrice.Find(_T("."));
	// 2.
	if(nPos > 0)
	{
		// 3.
		if(strCurrencyCode == _T("\\"))
		{
			ESLAfxMessageBox(_T("한국화폐일 경우 가격에 소수점을 사용할 수 없습니다."));
			m_pCM->SetControlMgrData(_T("BP_가격"), strPrice.Left(nPos));
		}

		strTmpData = strPrice.Mid(nPos+1);
		if( strTmpData.GetLength() > 2)
		{
			ESLAfxMessageBox(_T("가격정보는 소수점이하 둘째자리까지만 입력 가능합니다."));
			return -1;
		}
		strTmpData.Empty();
		strTmpData = strPrice.Left(nPos);
	}
	else strTmpData = strPrice;

	if(strTmpData.GetLength() > 12)
	{
		ESLAfxMessageBox(_T("가격정보는 소수점위 자료가 12자리 이상일 수 없습니다."));
		return -1;
	}
	//--가격검사 끝.--

	// 발주가격 검사
	strPrice.Empty();
	strTmpData.Empty();
	m_pCM->GetControlMgrData(_T("BP_발주가격"), strPrice);
	nPos = strPrice.Find(_T("."));
	if(nPos > 0) strTmpData = strPrice.Left(nPos);
	else strTmpData = strPrice;

	if(strTmpData.GetLength() > 12)
	{
		ESLAfxMessageBox(_T("가격정보는 소수점위 자료가 12자리 이상일 수 없습니다."));
		return -1;
	}
	//--발주가격검사 끝.--

	// 할인율 검사
	strPrice.Empty();
	strTmpData.Empty();
	m_pCM->GetControlMgrData(_T("BP_할인율"), strPrice);
	if(strPrice.IsEmpty()) return 0;
	
	nPos = strPrice.Find(_T("."));
	if(nPos > 0) 
	{
		strTmpData = strPrice.Mid(nPos+1);
		if(strTmpData.GetLength() > 2)
		{
			ESLAfxMessageBox(_T("할인율정보는 소수점이하 둘째자리까지만 입력 가능합니다."));
			return -1;
		}
		strTmpData.Empty();
		strTmpData = strPrice.Left(nPos);
	}
	else strTmpData = strPrice;

	if(strTmpData.GetLength() > 3)
	{
		ESLAfxMessageBox(_T("할인율정보는 소수점위 자료가 3자리 이상일 수 없습니다."));
		return -1;
	}

	DOUBLE dTmpData;
	//dTmpData = (DOUBLE)atof(strPrice.GetBuffer(0));
	TCHAR* pEndPtr = NULL;
	dTmpData = _tcstod(strPrice.GetBuffer(0), &pEndPtr);
	if(dTmpData > 100.0)
	{
		ESLAfxMessageBox(_T("할인율 정보가 100보다 클 수 없습니다."));
		m_pCM->SetControlMgrData(_T("BP_할인율"), _T("0"));
		return -1;
	}
	//--할인율검사 끝.--

	
	return 0;
}

INT CPurchaseInfoDlg::ApplyData()
{
	INT ids, i, nBookCount;
	DOUBLE dPrice, dDisc, dExchange;
	CString strVolPriceSum, strPrice, strDisc, strExchange, strData, strTmpData = _T("");

	nBookCount = GetBookDM()->GetRowCount();
	for ( i=0; i<nBookCount; i++ )
	{
		// 가격성격 설정
		GetBookDM()->SetCellData(_T("BB_가격성격"), m_strPriceChar, i);
		
		// 2006.01.17 강봉수 - 가격정책사항반영, 책발주가격을 설정한다.
		dPrice = dDisc = dExchange = 0.0;
		strPrice	= GetBookDM()->GetCellData(_T("BB_가격"), 0);
		strDisc		= GetDM()->GetCellData(_T("BP_할인율"), 0);
		strExchange	= GetDM()->GetCellData(_T("BP_환율"), 0);	
		//dPrice		= (DOUBLE)atof(strPrice.GetBuffer(0)	);
		TCHAR* pEndPtr = NULL;
		dPrice = _tcstod(strPrice.GetBuffer(0), &pEndPtr);

		//dExchange	= (DOUBLE)atof(strExchange.GetBuffer(0)	);
		dPrice = _tcstod(strExchange.GetBuffer(0), &pEndPtr);

		//dDisc		= (DOUBLE)atof(strDisc.GetBuffer(0)		);
		dDisc		= _tcstod(strDisc.GetBuffer(0), &pEndPtr);

		dPrice = (dPrice - ((dPrice*dDisc)/100)) * dExchange;
		if( dPrice == 0 )
		{
			GetBookDM()->SetCellData(_T("BB_발주가격_책"), _T("0"), i);
		}
		else
		{
			// 바인딩 +++++
			EDBM_BIND_DATA_TYPE* pBind = NULL;
			InitEDBMBindDataType( &pBind, 0, 1);	
			strPrice.Format(_T("%f"), dPrice);
			AddEDBMBindDataByString(pBind, STRING , _T(":PRICE"), strPrice.GetBuffer(0) );		
			GetDM()->GetOneValueQuery(_T("SELECT GET_ORDERPRICE(:PRICE, UDF_MANAGE_CODE) FROM DUAL"), strData, TRUE, pBind);
//DEL 			GetDM()->GetOneValueQuery(_T("SELECT GET_ORDERPRICE(:PRICE) FROM DUAL"), strData, TRUE, pBind);
			GetBookDM()->SetCellData(_T("BB_발주가격_책"), strData, i);
		}
	}

	GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0);
	if( GetFormApi()->GetVolumeCount(GetBookDM()) == 1 )
	{
		for ( i=0; i<nBookCount; i++ )
		{
			GetBookDM()->SetCellData(_T("BB_가격잡정보"), GetDM()->GetCellData(_T("BP_가격잡정보"), 0), i);
		}
	}

	if( GetApplyPriceInfo() )
	{
		ids = ApplyCalculation();
		if( ids < 0 )
		{
			AfxMessageBox(_T("가격정보 적용 실패"));
			return -1;
		}
		else if( ids > 0 )
		{
			AfxMessageBox(_T("권별정보에 가격이 빠져있는 권이 있습니다."));
			return -1;
		}
	}
	//가격정보 설정
	GetVolPriceSum(strVolPriceSum);
	GetDM()->SetCellData(_T("BP_가격"), strVolPriceSum, 0);	

	strTmpData.Format(_T("%f"), m_dOrderPrice);
	GetDM()->SetCellData(_T("BP_발주가격"), strTmpData, 0);

	return 0;
}

INT CPurchaseInfoDlg::ApplyCalculation()
{
	INT ids = -1;
	DOUBLE dPrice = 0;
	CString strPriceType, strTemp, strValue, str950$b, strTotalPrice;	
	
	ids = GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0);
	if( ids < 0 )	return -1;

	ids = m_pCM->GetControlMgrData(_T("BP_주문정산타입"), strPriceType);
	if( ids < 0 )	return -1;	

	ids = m_pCM->GetControlMgrData(_T("BP_가격"), strTemp);
	if( ids < 0 )	return -1;

	CESL_ControlComboBox* pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_주문정산타입"));	

	//dPrice = (DOUBLE)atof(strTemp.GetBuffer(0));
	TCHAR* pEndPtr = NULL;
	dPrice = _tcstod(strTemp.GetBuffer(0), &pEndPtr);
	
	// 가격 == 종가격의 값
	if (strPriceType == _T("종가격") && (!GetApplyVolPriceFromSpeciesPrice() || GetFormApi()->GetVolumeCount(GetBookDM()) == 1))
	{
		// 가격을 1/n으로 나누어 각각의 권에 적용시킨다.
		// 2004.09.14 nPrice->dPrice
		// 2004.12.06
		m_pCM->GetControlMgrData(_T("BP_화폐구분"), strTemp);
		ids = GetFormApi()->ApplySpeciesPriceToVolPrice(GetBookDM(), dPrice, strTotalPrice);
		if( ids < 0 )	return -1;
		SetApplyVolPriceFromSpeciesPrice(TRUE);
	}
	// 가격 == 각각 권의 가격을 합한값
	else if( strPriceType == _T("권가격") )
	{
		if(GetFormApi()->GetVolumeCount(GetBookDM()) > 1)
		{
			if( GetDlgItem(IDC_cmbBSFORM_PRICE_TYPE)->IsWindowEnabled() == TRUE )
			{
				if( IDNO == ESLAfxMessageBox(_T("설정하신 권가격을 각 권에 적용하시겠습니까?"), MB_YESNO) )
				{
					if( pCmb )	pCmb->SetCurSel(0);
					Display(0);
					GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0);
					return 0;
				}
			}
		}
		// 2004.09.14  nPrice->dPrice
		// 2004.12.06
		m_pCM->GetControlMgrData(_T("BP_화폐구분"), strTemp);
		ids = GetFormApi()->ApplyVolPrice(GetBookDM(), dPrice, strTotalPrice, str950$b);
		if (ids < 0) return -1;
		//m_pCM->SetControlMgrData(_T("BP_가격"), str950$b);

		m_pCM->SetControlMgrData(_T("BP_가격"), GetBookDM()->GetCellData(_T("BB_가격"), 0));

		GetDM()->SetCellData(_T("BP_가격"), str950$b, 0);
		
		Display(0);
	}
	// 비매품일경우 권가격을 모두 0으로 수정한다.
	// 2005-12-15 강봉수 추가	
	else if( m_strPriceChar == _T("비매품") )
	{
		m_pCM->GetControlMgrData(_T("BP_화폐구분"), strTemp);
		ids = GetFormApi()->ApplyVolPrice(GetBookDM(), dPrice, strTotalPrice, str950$b);
		if( ids < 0 )	return -1;

		m_pCM->SetControlMgrData(_T("BP_가격"), GetBookDM()->GetCellData(_T("BB_가격"), 0));

		GetDM()->SetCellData(_T("BP_가격"), str950$b, 0);	
		
		Display(0);
	}

	// 발주가격을 계산한다.
	SetOrderPrice();

	CStringArray arrIgnoreList;	
	arrIgnoreList.Add(_T("BP_가격"));
	ids = GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0, _T(""), &arrIgnoreList);
	
	return 0;
}

INT CPurchaseInfoDlg::Display(INT idx)
{
	INT nIdx;
	CString strPriceType, strExchangeRate, strCurrencyCode, strPrice;
	CESL_ControlComboBox* pCombo = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_주문정산타입"));
	CESL_ControlEdit* pEdt = (CESL_ControlEdit*)m_pCM->FindControl(_T("BP_가격"));
	
	GetFormApi()->GetRepresentationBook(GetBookDM(), nIdx);

	// 단권본일경우
	if( GetFormApi()->GetVolumeCount(GetBookDM()) == 1 )
	{
		if( nIdx < 0 )	nIdx = 0;
		GetDM()->SetCellData(_T("BP_가격잡정보"), GetBookDM()->GetCellData(_T("BB_가격잡정보"), nIdx), idx);
		
		//확장개선 사항에 맞게 수정
		if ( GetBookDM()->GetCellData(_T("BB_가격"), nIdx) != _T("0.00") &&
			 GetBookDM()->GetCellData(_T("BB_가격"), nIdx) != _T("0.0") &&
			 GetBookDM()->GetCellData(_T("BB_가격"), nIdx) != _T("0")		 )
		{
			GetDM()->SetCellData(_T("BP_가격"), GetBookDM()->GetCellData(_T("BB_가격"), nIdx), idx);
		}
		else
		{
			GetDM()->SetCellData(_T("BP_가격"), _T(""), idx);
		}

		if( pEdt )	pEdt->SetReadOnly(FALSE);
		if( pCombo )	pCombo->EnableWindow(TRUE);
	}
	// 다권본일경우
	else 
	{
		if( pCombo )	pCombo->EnableWindow(FALSE);
		
		if( pCombo->GetCurSel() == 0 )
		{
			CString strVolPriceSum;
			GetVolPriceSum(strVolPriceSum);
			GetDM()->SetCellData(_T("BP_가격"), strVolPriceSum, idx);
			
			if( pEdt )	pEdt->SetReadOnly(TRUE);
		}
		else 
		{
			CString strVolPrice;
			if( nIdx < 0 ) nIdx = 0;
			GetBookDM()->GetCellData(_T("BB_가격"), nIdx, strVolPrice);
			GetDM()->SetCellData(_T("BP_가격"), strVolPrice, idx);
			if( pEdt )	pEdt->SetReadOnly(FALSE);
		}
	}
	// 2005-01-26 환율에디트박스에 환율 보여줌.
	GetDM()->GetCellData(_T("BP_환율"), nIdx, strExchangeRate);
	CEdit* pEdit;
	pEdit = (CEdit*)GetDlgItem(IDC_edtEXCHANGE_RATE);
	pEdit->SetWindowText(strExchangeRate);	
	
	// CM 전체를 다시 뿌려준다.
	m_pCM->AllControlDisplay(idx);

	// 가격성격을 뿌려준다.
	m_strPriceChar = GetBookDM()->GetCellData(_T("BB_가격성격"), nIdx);
	m_strPriceChar.TrimLeft();		m_strPriceChar.TrimRight();	
	GetDM()->SetCellData(_T("BP_가격성격"), m_strPriceChar, idx);
	UpdateData(FALSE);

	OnKillfocuscmbBSFORMPRICECHARACTER();
	
	// 발주가격계산
	SetOrderPrice();

	GetDlgItem(IDC_edtBSFORM_ORDER_PRICE)->EnableWindow(FALSE);

	//가격검사 한번 더 한다. 
	CString strTmpData = _T("");
	strPrice = _T("");

	m_pCM->GetControlMgrData(_T("BP_가격"), strPrice);
	m_pCM->GetControlMgrData(_T("BP_화폐구분"), strCurrencyCode,-1,0);	
	
	INT  nPos = strPrice.Find(_T("."));
	// 2.
	if(nPos > 0)
	{
		// 3.
		if(strCurrencyCode == _T("\\"))
		{
			//ESLAfxMessageBox(_T("한국화폐일 경우 가격에 소수점을 사용할 수 없습니다."));
			m_pCM->SetControlMgrData(_T("BP_가격"), strPrice.Left(nPos));
		}
	}	

	return 0;
}

VOID CPurchaseInfoDlg::OnSelchangecmbBSFORMPRICETYPE() 
{
	Display(0);
}

VOID CPurchaseInfoDlg::OnKillfocusedtBSFORMPRICEDC() 
{	
	INT ids = ValidateData();
	if(ids < 0) return ;
	SetOrderPrice();
}

INT CPurchaseInfoDlg::ClearScreen()
{
	CESL_ControlEdit * pEdt = (CESL_ControlEdit*)m_pCM->FindControl(_T("BP_가격"));
	if(pEdt) pEdt->SetReadOnly(FALSE);
	return m_pCM->AllControlClear();
}

DOUBLE CPurchaseInfoDlg::OrderPrice_Calculate()
{
	DOUBLE dRate, dOrderPrice, dExchangeRate;
	CString strBookPriceSum, strDiscRate, strExchangeRate;
	
	GetBookPriceSum(strBookPriceSum);

	m_pCM->GetControlMgrData(_T("BP_할인율"), strDiscRate);
	if( strDiscRate.IsEmpty() == TRUE )
	{
		strDiscRate = _T("0");
		m_pCM->SetControlMgrData(_T("BP_할인율"), strDiscRate);
	}
	GetDM()->SetCellData(_T("BP_할인율"), strDiscRate, 0);
	GetDM()->GetCellData(_T("BP_환율"), 0, strExchangeRate);

	//dRate			= (DOUBLE)atof(strDiscRate.GetBuffer(0)		);
	//dOrderPrice		= (DOUBLE)atof(strBookPriceSum.GetBuffer(0)	);
	//dExchangeRate	= (DOUBLE)atof(strExchangeRate.GetBuffer(0)	);
	TCHAR* pEndPtr = NULL;
	dRate = _tcstod(strDiscRate.GetBuffer(0), &pEndPtr);
	dOrderPrice = _tcstod(strBookPriceSum.GetBuffer(0), &pEndPtr);
	dExchangeRate = _tcstod(strExchangeRate.GetBuffer(0), &pEndPtr);
	
	// 발주가격 계산 완료
	dOrderPrice = (dOrderPrice - ((dOrderPrice*dRate)/100)) * dExchangeRate;

	return dOrderPrice;
}

VOID CPurchaseInfoDlg::SetOrderPrice()
{
	// 2006.01.11 강봉수 - 발주가격을 절사단위 설정값으로 가져온다.	
	m_dOrderPrice = OrderPrice_Calculate();
	CString strOrderPrice, strBind;	
	strBind = _T("");
	m_strDisplayOrderPrice = _T("0");
	if( m_dOrderPrice > 0 )
	{
		EDBM_BIND_DATA_TYPE* pBind = NULL;	
		InitEDBMBindDataType( &pBind, 0, 1);	
		strBind.Format(_T("%f"), m_dOrderPrice );
		AddEDBMBindDataByString(pBind, STRING , _T(":PRICE"), strBind.GetBuffer(0) );
		GetDM()->GetOneValueQuery(_T("SELECT GET_ORDERPRICE(:PRICE, UDF_MANAGE_CODE) FROM DUAL"), m_strDisplayOrderPrice, TRUE, pBind);
//DEL 		GetDM()->GetOneValueQuery(_T("SELECT GET_ORDERPRICE(:PRICE) FROM DUAL"), m_strDisplayOrderPrice, TRUE, pBind);
	}
	
	m_pCM->SetControlMgrData(_T("BP_발주가격"), m_strDisplayOrderPrice);		

	/*
	DOUBLE dOrderPrice = OrderPrice_Calculate();
	CString strIs64			= _T("");
	CString strIs90			= _T("");
	CString strOrderPrice	= _T("");
	INT nOrderPrice = 0;
	// 육사일 경우에는 발주가격을 _T("원") 단위 반올림 한다.
	// 2005-09-29 강봉수
	//
	GetManageValue(_T("기타"), _T("공통"), _T("육사"), _T(""), strIs64);
	GetManageValue(_T("기타"), _T("공통"), _T("국방대"), _T(""), strIs90);
	if( strIs64 == _T("Y") )
	{
		dOrderPrice /= 10;
		nOrderPrice = (INT)(dOrderPrice+0.5);
		nOrderPrice *= 10;
		strOrderPrice.Format(_T("%d"), nOrderPrice);
	}
	//2005-11-23 황정영 : 국방대는 소수점 2째자리 절사여도 한화일경우 
	//원단위로 표시함
	else if( strIs90 == _T("Y") )
	{
		CString strCurrency = _T("");
		strCurrency = GetDM()->GetCellData(_T("BP_화폐구분"), 0);
		if(strCurrency == _T("\\"))
		{
			strOrderPrice.Format(_T("%d"), (INT)dOrderPrice);
		}
		else
			strOrderPrice = TruncPrice(GetDM(), dOrderPrice);
	}
	else	// 육사 이외~ 공공
	{
		// 모든 화폐 발주가격 소수점 2자리
		// 2005-09-29 강봉수
		//		
		strOrderPrice = TruncPrice(GetDM(), dOrderPrice);
	}
	m_pCM->SetControlMgrData(_T("BP_발주가격"), strOrderPrice);
	*/
}

VOID CPurchaseInfoDlg::SetDBOrderPrice()
{
	CString strOrderPrice = _T("");
	strOrderPrice.Format(_T("%f"), OrderPrice_Calculate() );
	m_pCM->SetControlMgrData(_T("BP_발주가격"), strOrderPrice);	
}

BOOL CPurchaseInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			return TRUE;
		else if(pMsg->wParam == VK_TAB)
		{
			if(pMsg->hwnd == GetDlgItem(IDC_edtBSFORM_PRICE_OTHER_INFO)->GetSafeHwnd())
			{
				pParentMgr->PostMessage(SET_TAB_ORDER, 0, 1);
				return TRUE;
			}
		}
	}
	if (pMsg->message == WM_CHAR && 
		(pMsg->hwnd == GetDlgItem(IDC_edtBSFORM_PRICE)->GetSafeHwnd() || 
		 pMsg->hwnd == GetDlgItem(IDC_edtBSFORM_ORDER_PRICE)->GetSafeHwnd() ||
		 pMsg->hwnd == GetDlgItem(IDC_edtBSFORM_PRICE_DC)->GetSafeHwnd()))
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

INT CPurchaseInfoDlg::AllControlClear()
{
	return GetFormApi()->AllControlClear(m_pCM, NULL);
}

VOID CPurchaseInfoDlg::OnKillfocusedtBSFORMPRICE() 
{
	INT ids = ValidateData();
	if( ids < 0 ) return ;

	ApplyCalculation();
}

UINT CPurchaseInfoDlg::OnSetTabOrder(LPARAM lParam, WPARAM wParam)
{
	GetDlgItem(IDC_cmbBSFORM_PRICE_TYPE)->SetFocus();
	return 0;
}


VOID CPurchaseInfoDlg::OnSelchangecmbBSFORMCURRENCYCLASS() 
{
	CString strExchangeRate, strCurrencyCode;
	CEdit* pEdit = NULL;

	// 2006-02-17 KBS - 화폐구분변경시 환율 적용(_T("Y"))값을 자동으로 읽어들인다.
	UpdateData(TRUE);
	m_pCM->GetControlMgrData(_T("BP_화폐구분"), strCurrencyCode);

	// 적용안함일 경우 환율값을 입력할 수 없으므로 _T("1")로 기본설정한다.
	if( strCurrencyCode.IsEmpty() == TRUE )	strExchangeRate = _T("1");
	else
	{
		CString query;
		query.Format(_T("SELECT exchange_rate ")
					 _T("FROM co_exchange_rate_tbl ")
					 _T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND user_code_mng_key in ")
//DEL 					 _T("WHERE user_code_mng_key in ")
					 _T("(SELECT rec_key FROM cd_code_tbl WHERE code LIKE '%s%%') ")
					 _T("AND APPLY_YN = 'Y' ")
					 _T("AND INPUT_DATE >= (SELECT TRUNC(SYSDATE, 'YEAR') FROM DUAL) ")
					 _T("AND INPUT_DATE < (SELECT ADD_MONTHS(TRUNC(SYSDATE, 'YEAR'), 12) FROM DUAL)")
					 , strCurrencyCode, strCurrencyCode	);
		GetDM()->GetOneValueQuery(query, strExchangeRate);

		// 적용된 값이 없을 경우에는 환율정보 창을 띄운다.
		if( strExchangeRate.IsEmpty() )
		{
			if(IDYES == ESLAfxMessageBox(_T("선정한 화폐구분에 대한 환율정보가 없습니다. \n환율정보를 설정하시겠습니까?"), MB_YESNO|MB_ICONQUESTION) )
			{
				CEXCHANGERATE_MGR dlg(pParentMgr, strCurrencyCode);
				if( IDOK == dlg.DoModal() )		strExchangeRate = dlg.m_exchangerate;
				else							strExchangeRate = _T("1");
			}
			else	strExchangeRate = _T("1");
		}
	}
	// 변경된 환율값을 에디트박스에 보여준다.	
	pEdit = (CEdit*)GetDlgItem(IDC_edtEXCHANGE_RATE);
	if( pEdit ) 	pEdit->SetWindowText(strExchangeRate);
	// 환율을 DM에 SetCellData 해준다.
	GetDM()->SetCellData(_T("BP_환율"), strExchangeRate, 0); // == FindDM(_T("DMUP_단행_종_구입정보"))->SetCellData(_T("BP_환율"), strExchangeRate, 0);	
	
	// KBS - 변경된 환율을 즉시 적용하여 구입가격을 계산한다.	
	SetOrderPrice();
	
	/*
	// 2004.09.01 환율정보 설정해야함 BP_환율
	CString strCurrencyCode;
	m_pCM->GetControlMgrData(_T("BP_화폐구분"), strCurrencyCode);
	CEXCHANGERATE_MGR dlg(pParentMgr, strCurrencyCode);
	if(IDOK == dlg.DoModal())
	{
		//환율을 DM에 SetCellData 해주고 적용되었다는 메시지 보여줌.
		FindDM(_T("DMUP_단행_종_구입정보"))->SetCellData(_T("BP_환율"), dlg.m_exchangerate, 0);

		// 2005-01-26 변경된 환율값을 에디트박스에 보여준다.
		//	m_pCM->GetControlMgrData(_T("BP_환율"), strExchangeRate);
		//	GetDM()->SetCellData(_T("BP_환율"), strExchangeRate, 0);

		ESLAfxMessageBox(_T("변경된 환율이 적용되었습니다."), MB_ICONINFORMATION);
	}
	*/

}

// 2005-01-26 환율 버튼을 눌러 환율을 적용한다.
VOID CPurchaseInfoDlg::OnbtnEXCHANGERATE() 
{
	CString strCurrencyCode;

	m_pCM->GetControlMgrData(_T("BP_화폐구분"), strCurrencyCode);
	CEXCHANGERATE_MGR dlg(pParentMgr, strCurrencyCode);
	if(IDOK == dlg.DoModal())
	{
		//환율을 DM에 SetCellData 해주고 적용되었다는 메시지 보여줌.
		FindDM(_T("DMUP_단행_종_구입정보"))->SetCellData(_T("BP_환율"), dlg.m_exchangerate, 0);

		// 2005-01-26 변경된 환율값을 에디트박스에 보여준다.
		CEdit * pEdit;
		pEdit = (CEdit*)GetDlgItem(IDC_edtEXCHANGE_RATE);
		pEdit->SetWindowText(dlg.m_exchangerate);

		AfxMessageBox(_T("변경된 환율이 적용되었습니다."), MB_ICONINFORMATION);
		
		// 변경된 환율을 즉시 적용
		//
		SetOrderPrice();
	}	
}

VOID CPurchaseInfoDlg::OnSelchangecmbBSFORMPRICECHARACTER() 
{
	INT idx;
	CESL_ControlEdit* pEdt = (CESL_ControlEdit*)m_pCM->FindControl(_T("BP_가격"));
	CESL_ControlMultiComboBox* pMcmb = (CESL_ControlMultiComboBox*)m_pCM->FindControl(_T("BP_화폐구분"));
	
	if( pEdt == NULL || pMcmb == NULL ) return;
	
	idx = m_pCmbPriceChar->GetCurSel();
	if( idx < 0 )	return;

	UpdateData(TRUE);

	m_pCmbPriceChar->GetLBText(idx, m_strPriceChar);
	if( m_strPriceChar == _T("비매품") )	
	{
		INT nIdx;
		pEdt->SetWindowText(_T(""));
		m_pCM->SetControlMgrData(_T("BP_발주가격"), _T(""));
		//==========================================
		//20080616 UPDATE BY PJW : SetCurSel(0)로하면 적용안함이 나오지 않을수 있다.
		//pMcmb->SetCurSel(0);
		nIdx = pMcmb->SelectString(_T("    "));
		pMcmb->SetCurSel(nIdx);
		//==========================================
	
		pMcmb->EnableWindow(FALSE);
		pEdt->SetReadOnly(TRUE);
		GetDlgItem(IDC_edtEXCHANGE_RATE)->SetWindowText(_T(""));
		GetDlgItem(IDC_cmbBSFORM_PRICE_TYPE)->EnableWindow(FALSE);	
	}
	else
	{
		pMcmb->EnableWindow(TRUE);
		// 전집처리되지 않은 자료만 수정이 가능하다.
		if( GetFormApi()->GetVolumeCount(GetBookDM()) == 1 )
		{
		pEdt->SetReadOnly(FALSE);
			GetDlgItem(IDC_cmbBSFORM_PRICE_TYPE)->EnableWindow(TRUE);
		}
	}

	UpdateData(FALSE);
}

VOID CPurchaseInfoDlg::OnKillfocuscmbBSFORMPRICECHARACTER() 
{
	//===================================================
	//20080620 REM BY PJW : 재사용을 위해서 함수로 뺀다.
//	INT i;
//	CString strData;		

//	UpdateData(TRUE);
//	m_strPriceChar.TrimLeft();	m_strPriceChar.TrimRight();	

//	// 콤보안에 없다면 추가한다
//	{
//		for( i=0; i<m_pCmbPriceChar->GetCount(); i++ )
//		{
//			m_pCmbPriceChar->GetLBText(i, strData);
//			if( strData == m_strPriceChar )		break;
//		}
//
//		if( i >= m_pCmbPriceChar->GetCount() )
//		{
//			m_pCmbPriceChar->AddString(m_strPriceChar);
//		}
//	}
//	
//	// 추가된 단어가 소트되었으므로 다시 그단어를 찾아 인덱스를 설정한다.
//	for( i=0; i<m_pCmbPriceChar->GetCount(); i++ )
//	{
//		m_pCmbPriceChar->GetLBText(i, strData);
//		if( strData == m_strPriceChar )	break;
//	}
//
//	m_pCmbPriceChar->SetCurSel(i);
	//===================================================

	//===================================================
	//20080620 ADD BY PJW : 가격성격을 추가 OR SEL변경
	FormPriceCharacterAdd(); 
	//===================================================

	OnSelchangecmbBSFORMPRICECHARACTER();
}
//===================================================
//20080620 ADD BY PJW : 가격성격을 추가 OR SEL변경
VOID CPurchaseInfoDlg::FormPriceCharacterAdd() 
{
	INT i;
	CString strData;		

	UpdateData(TRUE);
	m_strPriceChar.TrimLeft();	m_strPriceChar.TrimRight();	
	//================================
	//20080620 ADD BY PJW : ""이 없으면 ""을 추가해준다.
	for( i=0; i<m_pCmbPriceChar->GetCount(); i++ )
	{
		m_pCmbPriceChar->GetLBText(i, strData);
		if( strData == _T("") )		break;
	}
	if( i >= m_pCmbPriceChar->GetCount() )
	{
		m_pCmbPriceChar->AddString(_T(""));
	}
	//================================
	// 콤보안에 없다면 추가한다
	for( i=0; i<m_pCmbPriceChar->GetCount(); i++ )
	{
		m_pCmbPriceChar->GetLBText(i, strData);
		if( strData == m_strPriceChar )		break;
	}

	if( i >= m_pCmbPriceChar->GetCount() )
	{
		m_pCmbPriceChar->AddString(m_strPriceChar);
	}
	
	// 추가된 단어가 소트되었으므로 다시 그단어를 찾아 인덱스를 설정한다.
	for( i=0; i<m_pCmbPriceChar->GetCount(); i++ )
	{
		m_pCmbPriceChar->GetLBText(i, strData);
		if( strData == m_strPriceChar )	break;
	}

	m_pCmbPriceChar->SetCurSel(i);
}
//===================================================

VOID CPurchaseInfoDlg::SetOrderPrice_InDM(CESL_DataMgr *pDM, INT index)
{
	pDM->SetCellData(_T("BP_발주가격"), m_strDisplayOrderPrice, index);
}

HBRUSH CPurchaseInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
