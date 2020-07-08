 // DonateInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DonateInfoDlg.h"
#include "../COMMON_DONATOR_MANAGER/DonatorNameGetter_Search.h"
#include "../COMMON_DONATOR_MANAGER/DonatorNameGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDonateInfoDlg dialog


CDonateInfoDlg::CDonateInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CInfoDlg(pParent, CDonateInfoDlg::IDD)
{
	//{{AFX_DATA_INIT(CDonateInfoDlg)
	m_nRegYN = -1;
	m_strPriceChar = _T("");
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pCmbPriceChar = NULL;
	m_strDonatorInfoKey = _T("");
	m_strBeforeDonatorName = _T("");
	m_strBeforeDonatorInfoKey = _T("");
}

CDonateInfoDlg::~CDonateInfoDlg()
{
	
}

VOID CDonateInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonateInfoDlg)
	DDX_Radio(pDX, IDC_radREG_YES, m_nRegYN);
	DDX_CBString(pDX, IDC_cmbBSFORM_PRICE_CHARACTER, m_strPriceChar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonateInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CDonateInfoDlg)
	ON_BN_CLICKED(IDC_btnBSFORM_SEARCH_DONATOR, OnbtnBSFORMSEARCHDONATOR)
	ON_BN_CLICKED(IDC_radREG_YES, OnradREGYES)
	ON_BN_CLICKED(IDC_radREG_NO, OnradREGNO)
	ON_CBN_SELCHANGE(IDC_cmbBSFORM_PRICE_TYPE, OnSelchangecmbBSFORMPRICETYPE)
	ON_CBN_KILLFOCUS(IDC_cmbBSFORM_PRICE_CHARACTER, OnKillfocuscmbBSFORMPRICECHARACTER)
	ON_CBN_SELCHANGE(IDC_cmbBSFORM_PRICE_CHARACTER, OnSelchangecmbBSFORMPRICECHARACTER)
	ON_BN_CLICKED(IDC_btnBSFORM_BEFORE_DONATOR, OnbtnBSFORMBEFOREDONATOR)
	ON_MESSAGE(SET_TAB_ORDER, OnSetTabOrder)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonateInfoDlg message handlers

BOOL CDonateInfoDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CDonateInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("K2UPGRADE_단행_종_기증정보")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	
	//CM설정
	m_pCM = FindCM(_T("CM_단행_종_기증정보"));	
	if( m_pCM == NULL )
	{
		ESLAfxMessageBox(_T("[CM_단행_종_기증정보]를 설정할 수 없습니다."));
		return FALSE;
	}

	if( Init() == FALSE )
	{
		ESLAfxMessageBox(_T("기증정보 화면을 초기화 할 수 없습니다."));
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;
}

BOOL CDonateInfoDlg::Init()
{
	CESL_ControlComboBox* pCombo = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_주문정산타입"));

	m_pCmbPriceChar = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_가격성격"));
	if( m_pCmbPriceChar == NULL )	return FALSE;

	if( pCombo )  pCombo->SetCurSel(0);

	if( GetOpType() != SINSERT && 
		(GetSpeciesType() == BO_DONATE || GetSpeciesType() == BO_NONDONATE) )
	{
		GetDlgItem(IDC_radREG_YES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_radREG_NO )->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_radREG_YES)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_radREG_NO )->ShowWindow(SW_HIDE);
	}

	if( GetOpType() == SINSERT )
	{
		/************************************************************
		 ************************************************************/
		//20080424 ADD PJW : 입력일에 셋팅 
		CString strInputDate;
		CString strDate; 
		CTime t = CTime::GetCurrentTime();
		strDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
		GetDM()->SetCellData(_T("BP_입력일자"), strDate, 0);
		m_pCM->SetControlMgrData(_T("BP_입력일자")		, strInputDate);
		m_pCM->AllControlDisplay(0);
		/************************************************************
		 ************************************************************/
	}

	switch( GetSpeciesType() )
	{
	case BO_DONATE:
	{
		GetDlgItem(IDC_stcBSFORM_NONDONATE_REASON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtBSFORM_NONDONATE_REASON)->ShowWindow(SW_HIDE);
		SetWorkingStatus(_T("BOA211O"));
		m_nRegYN = 0;
	}
		break;

	case BO_NONDONATE:
	{
		SetWorkingStatus(_T("BOA212O"));
		m_nRegYN = 1;
	}
		break;

	case BO_REGOBJ:
	{
		GetDlgItem(IDC_stcBSFORM_NONDONATE_REASON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtBSFORM_NONDONATE_REASON)->ShowWindow(SW_HIDE);
		SetWorkingStatus(_T("BOA211O"));
		m_nRegYN = 0;
	}
		break;
	default:
	{
		GetDlgItem(IDC_stcBSFORM_NONDONATE_REASON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtBSFORM_NONDONATE_REASON)->ShowWindow(SW_HIDE);
	}
		break;
	}

	UpdateData(FALSE);

	return TRUE;
}

// 기증자 검색 버튼
VOID CDonateInfoDlg::OnbtnBSFORMSEARCHDONATOR() 
{
	CDonatorNameGetter_Search searchDlg(this);
	
	searchDlg.SetOpenMode(-1000);

	CString strPrice;
	CString strCharacter;
	CString strOrderPriceInfo;
	CString strCause;
	CString strName;
	CString strInputDate;
	CString strCurrency;

	m_pCM->GetControlMgrData(_T("BP_가격")			, strPrice);
	m_pCM->GetControlMgrData(_T("BP_가격성격")		, strCharacter);
	m_pCM->GetControlMgrData(_T("BP_가격잡정보")	, strOrderPriceInfo);
	m_pCM->GetControlMgrData(_T("BP_기증부당사유")	, strCause);
	m_pCM->GetControlMgrData(_T("BP_기증자명")		, strName);
	m_pCM->GetControlMgrData(_T("BP_입력일자")		, strInputDate);
	m_pCM->GetControlMgrData(_T("BP_화폐구분")		, strCurrency);

	GetDM()->SetCellData(_T("BP_가격")			, strPrice			, 0);
	GetDM()->SetCellData(_T("BP_가격성격")		, strCharacter		, 0);
	GetDM()->SetCellData(_T("BP_가격잡정보")	, strOrderPriceInfo	, 0);
	GetDM()->SetCellData(_T("BP_기증부당사유")	, strCause			, 0);
	GetDM()->SetCellData(_T("BP_기증자명")		, strName			, 0);
	GetDM()->SetCellData(_T("BP_입력일자")		, strInputDate		, 0);
	GetDM()->SetCellData(_T("BP_화폐구분")		, strCurrency		, 0);

	if( IDOK == searchDlg.DoModal() )
	{
		CDonatorNameGetterDlg searchResult(this);
		if( IDOK == searchResult.DoModal() )
		{
			GetDM()->SetCellData(_T("BP_기증자명"), searchResult.GetDonatorName(), 0);
			GetDM()->SetCellData(_T("BP_기증자정보KEY"), searchResult.GetDonatorKey(), 0);
			//검색되었으면 기증자 정보키 멤버에 저장
			m_strDonatorInfoKey = searchResult.GetDonatorKey();
			//20070903 ADD PJW : 입력일에 셋팅 
			if( strInputDate == _T("") )
			{
				CString strDate;
				CTime t = CTime::GetCurrentTime();
				strDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
				GetDM()->SetCellData(_T("BP_입력일자"), strDate, 0);
			}
		}
	}	

	// 2007.04.12 ADD REM BY PDJ
	// 기증자 버튼을 누른후 닫기 누르면 DM에 있는 값으로 초기화 된다.
	// 그러므로 Allcontrol 하면 안됨.
	m_pCM->AllControlDisplay(0);
}

INT CDonateInfoDlg::ValidateData()
{
	INT nPos;
	CString strPrice, strTmpData, strDonator, strRecKey, strQuery;

	m_pCM->GetControlMgrData(_T("BP_가격"), strPrice);
	
	nPos = strPrice.Find(_T("."));
	if( nPos > 0 )	strTmpData = strPrice.Left(nPos);
	else strTmpData = strPrice;

	if( strTmpData.GetLength() > 8 )
	{
		ESLAfxMessageBox(_T("가격정보는 소수점위 자료가 8자리 이상일 수 없습니다."));
		return -1;
	}

	if( nPos > 0 )
	{
		strTmpData = strPrice.Mid(nPos+1);
		if( strTmpData.GetLength() > 2 ) 
		{
			ESLAfxMessageBox(_T("가격정보는 소수점이하 둘째자리까지만 입력 가능합니다."));
			return -1;
		}	
	}

	m_pCM->GetControlMgrData(_T("BP_기증자명"), strDonator);
	// 2005-11-14 황정영 추가
	// 기증자 정보 테이블에서 기증자이름으로 검색한다. 기증자 이름이 중복됬을 경우 가 있으므로 ..
	// 추가적인 처리가 필요하다. 
	if( strDonator.IsEmpty() == FALSE )
	{
		CESL_DataMgr pDM;
		pDM.SetCONNECTION_INFO(GetDM()->CONNECTION_INFO);
		strQuery.Format(_T("SELECT REC_KEY FROM CO_DONATOR_TBL WHERE DONATOR = '%s'"), strDonator);
		
		pDM.RestructDataManager(strQuery);
		if(pDM.GetRowCount() == 1)
		{
			m_strDonatorInfoKey = pDM.GetCellData(0,0);
		}
		else if( pDM.GetRowCount() > 1 )
		{

			if(m_strDonatorInfoKey.IsEmpty() == TRUE )
			{
				if(GetOpType() == SINSERT)
				{
					AfxMessageBox(_T("기증자이름이 중복됩니다. \n 검색하여 선정해 주십시오"));
					return -1;
				}
				else
				{
					GetDM()->GetCellData(_T("BP_기증자정보KEY"),0,m_strDonatorInfoKey);
				}
			}
			else
			{
				CString strDonInfoKey;
				for(INT i = 0; i < pDM.GetRowCount() ; i++)
				{
					if( m_strDonatorInfoKey == pDM.GetCellData(i,0))
						break;
					if( i == pDM.GetRowCount()-1 )
						m_strDonatorInfoKey = _T("");
				}
			}
		}
		else
		{
			//m_strDonatorInfoKey == _T("");
			AfxMessageBox(_T("관리되지 않는 기증자 정보는 저장할 수 없습니다."));
			return -1;
		}
	}
	else 
	{
		m_strDonatorInfoKey = _T("");
	}
	/*if( strDonator.IsEmpty() == FALSE )
	{
		strQuery.Format(_T("SELECT REC_KEY FROM CO_DONATOR_TBL WHERE DONATOR = '%s'"), strDonator);
		GetDM()->GetOneValueQuery(strQuery, m_strDonatorInfoKey);
	}
	else 
	{
		m_strDonatorInfoKey = _T("");
	}*/

	if( GetDonatorInfo() == TRUE && m_strDonatorInfoKey.IsEmpty() == TRUE )
	{
		ESLAfxMessageBox(_T("기증자정보 입력은 필수 사항입니다."));
		return -1;
	}
	else
	{
		m_strBeforeDonatorName = strDonator;
		m_strBeforeDonatorInfoKey = m_strDonatorInfoKey;

		//====================================================================
		//2009.05.20 ADD BY LKS : 기증일자도 보여주도록 수정
		m_pCM->GetControlMgrData(_T("BP_입력일자"), m_strBeforeDonateDate, 0);
		//====================================================================
	}

	return 0;
}

INT CDonateInfoDlg::ApplyData()
{
	INT nBookCount, i, ids;
	CString strVolPriceSum;
	
	GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0);

	nBookCount = GetBookDM()->GetRowCount();
	for ( i=0; i<nBookCount; i++ )
	{
		// 가격성격 설정
		GetBookDM()->SetCellData(_T("BB_가격성격"), m_strPriceChar, i);
	}

	GetDM()->SetCellData(_T("BP_기증자정보KEY"), m_strDonatorInfoKey, 0);
	
	if( GetFormApi()->GetVolumeCount(GetBookDM()) == 1 )
	{
		nBookCount = GetBookDM()->GetRowCount();
		for( i=0; i<nBookCount; i++)
		{
			GetBookDM()->SetCellData(_T("BB_가격잡정보"), GetDM()->GetCellData(_T("BP_가격잡정보"), 0), i);
		}
	}

	if( GetApplyPriceInfo() == TRUE )
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
	
	return 0;
}

INT CDonateInfoDlg::ApplyCalculation()
{
	INT ids, nPrice;
	DOUBLE dPrice;
	CString strPriceType, strTemp, str950$b, strTotalPrice;
	CStringArray arrIgnoreList;
	

	ids = GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0);
	if( ids < 0 )	return -1;	
	
	ids = m_pCM->GetControlMgrData(_T("BP_주문정산타입"), strPriceType);
	if( ids < 0 )	return -1;
	
	CESL_ControlComboBox* pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_주문정산타입"));

	ids = m_pCM->GetControlMgrData(_T("BP_가격"), strTemp);
	if( ids < 0 )	return -1;
	
	nPrice = _ttoi(strTemp);
	
	// 2004.09.14
	//dPrice = (DOUBLE)atof(strTemp.GetBuffer(0));

	TCHAR* pEndPtr = NULL;
	dPrice = _tcstod(strTemp.GetBuffer(0), &pEndPtr);

	// 가격 == 종가격의 값
	if( strPriceType == _T("종가격") && 
		( GetApplyVolPriceFromSpeciesPrice() == FALSE || GetFormApi()->GetVolumeCount(GetBookDM()) == 1) )
	{
		// 가격을 1/n으로 나누어 각각의 권에 적용시킨다.
		// 2004.09.14  nPrice->dPrice
		// 2004.12.06
		m_pCM->GetControlMgrData(_T("BP_화폐구분"), strTemp);
		ids = GetFormApi()->ApplySpeciesPriceToVolPrice(GetBookDM(), dPrice, strTotalPrice);
		if( ids < 0 )	return -1;

		SetApplyVolPriceFromSpeciesPrice(TRUE);
	}
	// 가격 == 각각 권의 가격을 합한값
	else if (strPriceType == _T("권가격"))
	{
		if( GetFormApi()->GetVolumeCount(GetBookDM()) > 1 )
		{
			if( IDNO == ESLAfxMessageBox(_T("설정하신 권가격을 각 권에 적용하시겠습니까?"), MB_YESNO) )
			{
				pCmb->SetCurSel(0);
				Display(0);
				GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0);
				return 0;
			}
		}
		// 2004.09.14 nPrice->dPrice
		// 2004.12.06
		m_pCM->GetControlMgrData(_T("BP_화폐구분"), strTemp);
		ids = GetFormApi()->ApplyVolPrice(GetBookDM(), dPrice, strTotalPrice, str950$b);
		if( ids < 0 )	return -1;
		//m_pCM->SetControlMgrData(_T("BP_가격"), str950$b);
		m_pCM->SetControlMgrData(_T("BP_가격"), GetBookDM()->GetCellData(_T("BB_가격"), 0));

		GetDM()->SetCellData(_T("BP_가격"), str950$b, 0);
		//if(pCmb) pCmb->SetCurSel(0);
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

	
	arrIgnoreList.Add(_T("BP_가격"));
	ids = GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0, _T(""), &arrIgnoreList);
	
		return 0;
}

INT CDonateInfoDlg::Display(INT idx)
{
	INT nIdx = 0;
	CESL_ControlEdit* pEdt		 = (CESL_ControlEdit*)m_pCM->FindControl(_T("BP_가격"));
	CESL_ControlComboBox* pCombo = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_주문정산타입"));

	GetFormApi()->GetRepresentationBook(GetBookDM(), nIdx);

	if (GetFormApi()->GetVolumeCount(GetBookDM()) == 1)
	{
		if( nIdx < 0 )	nIdx = 0;

		GetDM()->SetCellData(_T("BP_가격잡정보"), GetBookDM()->GetCellData(_T("BB_가격잡정보"), nIdx), idx);

		//확장개선 사항에 맞게 수정
		if ( GetBookDM()->GetCellData(_T("BB_가격"), nIdx) != _T("0.00")	&&
			 GetBookDM()->GetCellData(_T("BB_가격"), nIdx) != _T("0.0")		&&
			 GetBookDM()->GetCellData(_T("BB_가격"), nIdx) != _T("0")		 )
		{		
			GetDM()->SetCellData(_T("BP_가격"), GetBookDM()->GetCellData(_T("BB_가격"), nIdx), idx);
		}
		else
		{
			GetDM()->SetCellData(_T("BP_가격"), _T(""), idx);
		}

		if( pEdt )		pEdt->SetReadOnly(FALSE);
		if( pCombo )	pCombo->EnableWindow(TRUE);
	}
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
			if( nIdx < 0 )	nIdx = 0;
			GetBookDM()->GetCellData(_T("BB_가격"), nIdx, strVolPrice);
			GetDM()->SetCellData(_T("BP_가격"), strVolPrice, idx);
			if( pEdt )	pEdt->SetReadOnly(FALSE);
		}		
	}

	// 가격성격을 뿌려준다.
	m_strPriceChar = GetBookDM()->GetCellData(_T("BB_가격성격"), nIdx);
	m_strPriceChar.TrimLeft();		m_strPriceChar.TrimRight();	
	GetDM()->SetCellData(_T("BP_가격성격"), m_strPriceChar, idx);
	UpdateData(FALSE);
	OnKillfocuscmbBSFORMPRICECHARACTER();

	//===================================================
	//2009.09.14 ADD BY PJW : 기증자는 검색해서 입력 받도록 한다.
	CEdit* pEditDonator = (CEdit*)GetDlgItem(IDC_edtBSFORM_DONATOR);
	pEditDonator->SetReadOnly(TRUE);
	//===================================================
	
	return m_pCM->AllControlDisplay(idx);
}

VOID CDonateInfoDlg::OnradREGYES() 
{
	UpdateData(TRUE);

	ShowCtrlByRegYN();
}

VOID CDonateInfoDlg::OnradREGNO() 
{
	UpdateData(TRUE);

	ShowCtrlByRegYN();
}

VOID CDonateInfoDlg::ShowCtrlByRegYN()
{
	INT cnt, i;

	if( m_nRegYN == 0 )
	{
		GetDlgItem(IDC_stcBSFORM_NONDONATE_REASON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtBSFORM_NONDONATE_REASON)->ShowWindow(SW_HIDE);
		SetWorkingStatus(_T("BOA211O"));//기증접수자료
		::PostMessage(pParentMgr->GetSafeHwnd(), WORKING_STATUS_CHANGE, (DWORD)BO_DONATE, 0);
	}
	else if( m_nRegYN == 1 )
	{
		GetDlgItem(IDC_stcBSFORM_NONDONATE_REASON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtBSFORM_NONDONATE_REASON)->ShowWindow(SW_SHOW);
		SetWorkingStatus(_T("BOA212O"));//기증부당자료
		::PostMessage(pParentMgr->GetSafeHwnd(), WORKING_STATUS_CHANGE, (DWORD)BO_NONDONATE, 0);
	}
	
	cnt = GetBookDM()->GetRowCount();
	for( i=0 ; i<cnt ; i++ )
	{
		if( GetBookDM()->GetCellData(_T("BB_DB_FLAG"), i).IsEmpty() )
		{
			GetBookDM()->SetCellData(_T("BB_DB_FLAG"), _T("U"), i);
		}

		GetBookDM()->SetCellData(_T("BB_자료상태"), GetWorkingStatus(), i);
	}
}

INT CDonateInfoDlg::ClearScreen()
{
	CESL_ControlEdit * pEdt = (CESL_ControlEdit*)m_pCM->FindControl(_T("BP_가격"));
	if(pEdt) pEdt->SetReadOnly(FALSE);
	return m_pCM->AllControlClear();
}

BOOL CDonateInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			return TRUE;
		else if(pMsg->wParam == VK_TAB)
		{
			if(pMsg->hwnd == GetDlgItem(IDC_edtBSFORM_NONDONATE_REASON)->GetSafeHwnd())
			{
				pParentMgr->PostMessage(SET_TAB_ORDER, 0, 1);	
				return TRUE;
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_edtBSFORM_PRICE_OTHER_INFO)->GetSafeHwnd() && !GetDlgItem(IDC_edtBSFORM_NONDONATE_REASON	)->IsWindowVisible())
			{
				pParentMgr->PostMessage(SET_TAB_ORDER, 0, 1);	
				return TRUE;
			}
		}
	}
	if (pMsg->message == WM_CHAR && pMsg->hwnd == GetDlgItem(IDC_edtBSFORM_PRICE)->GetSafeHwnd())
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

VOID CDonateInfoDlg::OnSelchangecmbBSFORMPRICETYPE() 
{
	Display(0);	
}

INT CDonateInfoDlg::AllControlClear()
{
	return GetFormApi()->AllControlClear(m_pCM, NULL);
}

UINT CDonateInfoDlg::OnSetTabOrder(LPARAM lParam, WPARAM wParam)
{
	GetDlgItem(IDC_cmbBSFORM_PRICE_TYPE)->SetFocus();
	return 0;
}



VOID CDonateInfoDlg::OnKillfocuscmbBSFORMPRICECHARACTER() 
{
	INT i, idx;
	CString strData;		

	UpdateData(TRUE);
	m_strPriceChar.TrimLeft();	m_strPriceChar.TrimRight();	

	// 현재 선택되어진 인덱스가 -1 이라면 새로 추가중이다.
	// 새로추가된 단어가 콤보안에 없다면 추가한다.
	idx = m_pCmbPriceChar->GetCurSel();
	//================================
	//20080630 ADD BY PJW : ""이 없으면 ""을 추가해준다.
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
	if( idx < 0 )
	{
		for( i=0; i<m_pCmbPriceChar->GetCount(); i++ )
		{
			m_pCmbPriceChar->GetLBText(i, strData);
			if( strData == m_strPriceChar )		break;
		}

		if( i >= m_pCmbPriceChar->GetCount() )
		{
			m_pCmbPriceChar->AddString(m_strPriceChar);
		}
	}
	
	// 추가된 단어가 소트되었으므로 다시 그단어를 찾아 인덱스를 설정한다.
	for( i=0; i<m_pCmbPriceChar->GetCount(); i++ )
	{
		m_pCmbPriceChar->GetLBText(i, strData);
		if( strData == m_strPriceChar )	break;
	}

	m_pCmbPriceChar->SetCurSel(i);

	OnSelchangecmbBSFORMPRICECHARACTER();
}

VOID CDonateInfoDlg::OnSelchangecmbBSFORMPRICECHARACTER() 
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
		pEdt->SetData(_T(""));
		pEdt->SetReadOnly(TRUE);
		//==========================================
		//20080630 UPDATE BY PJW : SetCurSel(0)로하면 적용안함이 나오지 않을수 있다.
		//pMcmb->SetCurSel(0);
		INT nIdx;
		nIdx = pMcmb->SelectString(_T("    "));
		pMcmb->SetCurSel(nIdx);
		//==========================================
		pMcmb->EnableWindow(FALSE);
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
//2005-11-14 황정영 추가 : 이전 기증자 버튼을 사용한다.
VOID CDonateInfoDlg::OnbtnBSFORMBEFOREDONATOR() 
{
	INT nRow;
	nRow = GetDM()->GetRowCount();
	if(1 > nRow)
	{
		AfxMessageBox(_T("이전 기증자정보가 존재하지 않습니다"));
		return;
	}
	else
	{
		CString strDonatorKey, strDonatorName, strQuery;
		strDonatorKey.Empty();
		strDonatorName.Empty();
		
/*		GetDM()->GetCellData(_T("BP_기증자정보KEY"),nRow-1, strDonatorKey);
		if( strDonatorKey.IsEmpty() )
		{
			strDonatorName = m_strBeforeDonatorName;
		}
		else
		{
			m_strDonatorInfoKey = strDonatorKey;
			strQuery.Format(_T("SELECT DONATOR FROM CO_DONATOR_TBL WHERE REC_KEY = '%s'"), strDonatorKey);
			GetDM()->GetOneValueQuery(strQuery, strDonatorName);
		}*/
		strDonatorName = m_strBeforeDonatorName;
		m_strDonatorInfoKey = m_strBeforeDonatorInfoKey;
		GetDlgItem(IDC_edtBSFORM_DONATOR)->SetWindowText(strDonatorName);

		//====================================================================
		//2009.05.20 ADD BY LKS : 기증일자도 보여주도록 수정
		m_pCM->SetControlMgrData(_T("BP_입력일자"), m_strBeforeDonateDate);
		//====================================================================
	}	
}

HBRUSH CDonateInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}
