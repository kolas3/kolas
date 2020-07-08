// BasicInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BasicInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBasicInfoDlg dialog


CBasicInfoDlg::CBasicInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CInfoDlg(pParent, CBasicInfoDlg::IDD)
{
	//{{AFX_DATA_INIT(CBasicInfoDlg)
	m_strPriceChar = _T("");
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pCmbPriceChar = NULL;
}

CBasicInfoDlg::~CBasicInfoDlg()
{
}


VOID CBasicInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBasicInfoDlg)
	DDX_CBString(pDX, IDC_cmbBSFORM_PRICE_CHARACTER, m_strPriceChar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBasicInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CBasicInfoDlg)
	ON_CBN_KILLFOCUS(IDC_cmbBSFORM_PRICE_CHARACTER, OnKillfocuscmbBSFORMPRICECHARACTER)
	ON_CBN_SELCHANGE(IDC_cmbBSFORM_PRICE_CHARACTER, OnSelchangecmbBSFORMPRICECHARACTER)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBasicInfoDlg message handlers

BOOL CBasicInfoDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBasicInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_단행_종_기본정보")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM설정
	m_pCM = FindCM(_T("CM_단행_종_기본정보"));		
	m_pCM->SetControlMgrData(_T("BP_주문정산타입"), _T("종가격"));

	if( Init() == FALSE )
	{
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CBasicInfoDlg::ValidateData()
{
	INT nPos;
	CString strPrice, strTmpData;

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

	return 0;
}

INT CBasicInfoDlg::ApplyData()
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

	if( GetFormApi()->GetVolumeCount(GetBookDM()) == 1 )
	{
		for( i=0; i<nBookCount; i++)
		{
			GetBookDM()->SetCellData(_T("BB_가격잡정보"), GetDM()->GetCellData(_T("BP_가격잡정보"), 0), i);			
		}
	}

	if( GetApplyPriceInfo() )
	{
		ids = ApplyCalculation();
		if (ids < 0)
		{
			AfxMessageBox(_T("가격정보 적용 실패"));
			return -1;
		}
		else if (ids > 0)
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

INT CBasicInfoDlg::ApplyCalculation()
{
	INT ids = -1;
	ids = GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0);
	
	CString strPriceType;
	ids = m_pCM->GetControlMgrData(_T("BP_주문정산타입"), strPriceType);
	if (ids < 0) return -1;
	
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_주문정산타입"));
	
	CString strTemp;
	ids = m_pCM->GetControlMgrData(_T("BP_가격"), strTemp);
	if (ids < 0) return -1;
	
	INT nPrice = _ttoi(strTemp);
	
	// 2004.09.14
	DOUBLE dPrice;

	//atof    = _tcstod(x, TCHAR* s);
//	dPrice = (DOUBLE)atof(strTemp.GetBuffer(0));
	TCHAR* pEndPtr = NULL;
	dPrice = _tcstod(strTemp.GetBuffer(0), &pEndPtr);

	CString str950$b, strTotalPrice;
	// 가격 == 종가격의 값
	if (strPriceType == _T("종가격") && (!GetApplyVolPriceFromSpeciesPrice() || GetFormApi()->GetVolumeCount(GetBookDM()) == 1))
	{
		// 가격을 1/n으로 나누어 각각의 권에 적용시킨다.
		// 2004.09.14  nPrice->dPrice
		// 2004.12.06
		m_pCM->GetControlMgrData(_T("BP_화폐구분"), strTemp);
		ids = GetFormApi()->ApplySpeciesPriceToVolPrice(GetBookDM(), dPrice, strTotalPrice);
		if (ids < 0) return -1;
		SetApplyVolPriceFromSpeciesPrice(TRUE);
	}
	// 가격 == 각각 권의 가격을 합한값
	else if (strPriceType == _T("권가격"))
	{
		if(GetFormApi()->GetVolumeCount(GetBookDM()) > 1)
		{
			if(IDNO == ESLAfxMessageBox(_T("설정하신 권가격을 각 권에 적용하시겠습니까?"), MB_YESNO))
			{
				pCmb->SetCurSel(0);
				Display(0);
				GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0);
				return 0;
			}
		}
		// 2004.12.06
		m_pCM->GetControlMgrData(_T("BP_화폐구분"), strTemp);
		ids = GetFormApi()->ApplyVolPrice(GetBookDM(), dPrice, strTotalPrice, str950$b);
		if (ids < 0) return -1;
		//m_pCM->SetControlMgrData(_T("BP_가격"), str950$b);
		m_pCM->SetControlMgrData(_T("BP_가격"), GetBookDM()->GetCellData(_T("BB_가격"), 0));

		GetDM()->SetCellData(_T("BP_가격"), str950$b, 0);
		//if(pCmb) pCmb->SetCurSel(0);
		Display(0);
	}
	CStringArray arrIgnoreList;	arrIgnoreList.Add(_T("BP_가격"));
	ids = GetFormApi()->MakeDMFromCM(m_pCM, GetDM(), 0, _T(""), &arrIgnoreList);
	
	return 0;
}

INT CBasicInfoDlg::Display(INT idx)
{
	INT nIdx = 0;
	if( GetFormApi()->GetVolumeCount(GetBookDM()) == 1 )
	{
		GetFormApi()->GetRepresentationBook(GetBookDM(), nIdx);
		if(nIdx < 0) nIdx = 0;
		GetDM()->SetCellData(_T("BP_가격잡정보"), GetBookDM()->GetCellData(_T("BB_가격잡정보"), nIdx), 0);
		GetDM()->SetCellData(_T("BP_가격"), GetBookDM()->GetCellData(_T("BB_가격"), nIdx), 0);
		CESL_ControlEdit * pEdt = (CESL_ControlEdit*)m_pCM->FindControl(_T("BP_가격"));
		if(pEdt) pEdt->SetReadOnly(FALSE);
	}
	else
	{
		CESL_ControlComboBox * pCombo = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_주문정산타입"));
		if( pCombo->GetCurSel() == 0 )
		{
			CString strVolPriceSum;
			GetVolPriceSum(strVolPriceSum);
			GetDM()->SetCellData(_T("BP_가격"), strVolPriceSum, idx);
			CESL_ControlEdit * pEdt = (CESL_ControlEdit*)m_pCM->FindControl(_T("BP_가격"));
			if(pEdt) pEdt->SetReadOnly(TRUE);
		}
		else 
		{
			CString strVolPrice;
			GetFormApi()->GetRepresentationBook(GetBookDM(), nIdx);
			if(nIdx < 0) nIdx = 0;
			GetBookDM()->GetCellData(_T("BB_가격"), nIdx, strVolPrice);
			GetDM()->SetCellData(_T("BP_가격"), strVolPrice, idx);
			CESL_ControlEdit * pEdt = (CESL_ControlEdit*)m_pCM->FindControl(_T("BP_가격"));
			if(pEdt) pEdt->SetReadOnly(FALSE);
		}
	}

	// 가격성격을 뿌려준다.
	m_strPriceChar = GetBookDM()->GetCellData(_T("BB_가격성격"), nIdx);
	m_strPriceChar.TrimLeft();		m_strPriceChar.TrimRight();	
	GetDM()->SetCellData(_T("BP_가격성격"), m_strPriceChar, idx);
	UpdateData(FALSE);
	OnKillfocuscmbBSFORMPRICECHARACTER();	

	return m_pCM->AllControlDisplay(idx);
}

INT CBasicInfoDlg::ClearScreen()
{
	CESL_ControlEdit * pEdt = (CESL_ControlEdit*)m_pCM->FindControl(_T("BP_가격"));
	if(pEdt) pEdt->SetReadOnly(FALSE);
	return m_pCM->AllControlClear();
}

BOOL CBasicInfoDlg::Init()
{
	CESL_ControlComboBox * pCombo = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_주문정산타입"));
	if( pCombo ) pCombo->SetCurSel(0);

	m_pCmbPriceChar = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_가격성격"));
	if( m_pCmbPriceChar == NULL )	return FALSE;
	return TRUE;
}

BOOL CBasicInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			return TRUE;
		else if(pMsg->wParam == VK_TAB && pMsg->hwnd == GetDlgItem(IDC_edtBSFORM_PRICE_OTHER_INFO)->GetSafeHwnd())
		{
			pParentMgr->PostMessage(SET_TAB_ORDER, 0, 1);
			return TRUE;
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

VOID CBasicInfoDlg::OnSelchangecmbBSFORMPRICETYPE() 
{
	Display(0);	
}

INT CBasicInfoDlg::AllControlClear()
{
	return GetFormApi()->AllControlClear(m_pCM, NULL);
}

UINT CBasicInfoDlg::OnSetTabOrder(LPARAM lParam, WPARAM wParam)
{
	GetDlgItem(IDC_cmbBSFORM_PRICE_TYPE)->SetFocus();
	return 0;
}


VOID CBasicInfoDlg::OnKillfocuscmbBSFORMPRICECHARACTER() 
{
	INT i, idx;
	CString strData;		

	UpdateData(TRUE);
	m_strPriceChar.TrimLeft();	m_strPriceChar.TrimRight();	

	// 현재 선택되어진 인덱스가 -1 이라면 새로 추가중이다.
	// 새로추가된 단어가 콤보안에 없다면 추가한다.
	idx = m_pCmbPriceChar->GetCurSel();
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
	
	
	//===================================================
	//2009.05.20 ADD BY LKS : 수정일 경우 가격성격에 '공백성격'을 추가한다.
	
	if(idx>=0)
	{
		CString Temp;
		m_pCmbPriceChar->GetLBText(0, Temp);
		if(!Temp.IsEmpty())
			m_pCmbPriceChar->AddString(_T(""));
	}
	//===================================================

	// 추가된 단어가 소트되었으므로 다시 그단어를 찾아 인덱스를 설정한다.
	for( i=0; i<m_pCmbPriceChar->GetCount(); i++ )
	{
		m_pCmbPriceChar->GetLBText(i, strData);
		if( strData == m_strPriceChar )	break;
	}

	m_pCmbPriceChar->SetCurSel(i);

	OnSelchangecmbBSFORMPRICECHARACTER();
	
}

VOID CBasicInfoDlg::OnSelchangecmbBSFORMPRICECHARACTER() 
{
	INT idx;
	CESL_ControlEdit* pEdt = (CESL_ControlEdit*)m_pCM->FindControl(_T("BP_가격"));
	CESL_ControlComboBox* pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_화폐구분"));
	
	if( pEdt == NULL ) return;
	
	idx = m_pCmbPriceChar->GetCurSel();
	if( idx < 0 )	return;

	m_pCmbPriceChar->GetLBText(idx, m_strPriceChar);
	if( m_strPriceChar == _T("비매품") )	
	{
		pEdt->SetData(_T("0"));
		pEdt->SetReadOnly(TRUE);
		pCmb->SetCurSel(0);
		pCmb->EnableWindow(FALSE);
	}
	else
	{
		pEdt->SetReadOnly(FALSE);
		pCmb->EnableWindow(TRUE);
	}
}

HBRUSH CBasicInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
