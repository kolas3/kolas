// DonateInfoChange.cpp : implementation file
//

#include "stdafx.h"
#include "DonateInfoChange.h"
#include "DataChangeManager.h"
#include "../COMMON_DONATOR_MANAGER/DonatorNameGetter_Search.h"
#include "../COMMON_DONATOR_MANAGER/DonatorNameGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDonateInfoChange dialog
// 생성자
CDonateInfoChange::CDonateInfoChange(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDonateInfoChange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonateInfoChange)
	m_nRegYn	= 0;
	m_bDonator = TRUE;
	m_bRegYn = TRUE;
	m_bRemark = TRUE;
	m_bCurrencyCode = TRUE;  // 2004.09.08 TREU로 변경.
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pDM_BOOK = NULL;
	m_pCM = NULL;
	m_pMainGrid = NULL;

	m_bDonatorInfo = FALSE;
	
	m_strDonatorKey = _T("");
	m_strDonatorName = _T("");
}

CDonateInfoChange::~CDonateInfoChange()
{
}
VOID CDonateInfoChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonateInfoChange)
	DDX_Check(pDX, IDC_chkDONATOR, m_bDonator);
	DDX_Check(pDX, IDC_chkREGYN, m_bRegYn);
	DDX_Check(pDX, IDC_chkREMARK, m_bRemark);
	DDX_Radio(pDX, IDC_radREG, m_nRegYn);
	DDX_Check(pDX, IDC_chkCURRENCY_CODE, m_bCurrencyCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonateInfoChange, CDialog)
	//{{AFX_MSG_MAP(CDonateInfoChange)
	ON_BN_CLICKED(IDC_chkDONATOR, OnchkDONATOR)
	ON_BN_CLICKED(IDC_chkREGYN, OnchkREGYN)
	ON_BN_CLICKED(IDC_chkREMARK, OnchkREMARK)
	ON_BN_CLICKED(IDC_radNOREG, OnradNOREG)
	ON_BN_CLICKED(IDC_radREG, OnradREG)
	ON_BN_CLICKED(IDC_btnSEARCH_DONATOR, OnbtnSEARCHDONATOR)
	ON_EN_KILLFOCUS(IDC_edtDONATOR, OnKillfocusedtDONATOR)
	ON_BN_CLICKED(IDC_chkCURRENCY_CODE, OnchkCURRENCYCODE)
	ON_EN_CHANGE(IDC_edtDONATOR, OnChangeedtDONATOR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonateInfoChange message handlers

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 기증자 체크박스에 체크여부에 따라 리소스를 표시한다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnchkDONATOR() 
{
	m_bDonator = !m_bDonator;
	if( m_bDonator == TRUE ) 
	{
		GetDlgItem(IDC_edtDONATOR		)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnSEARCH_DONATOR)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_edtDONATOR		)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnSEARCH_DONATOR)->EnableWindow(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 등록/미등록 체크박스에 체크여부에 따라 리소스를 표시한다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnchkREGYN() 
{
	m_bRegYn = !m_bRegYn;
	if( m_bRegYn == TRUE ) 
	{
		GetDlgItem(IDC_radREG	)->EnableWindow(TRUE);
		GetDlgItem(IDC_radNOREG	)->EnableWindow(TRUE);
		
		if( m_nRegYn == 0 )		GetDlgItem(IDC_edtNOREG_REASON)->EnableWindow(FALSE);
		else GetDlgItem(IDC_edtNOREG_REASON)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_radREG)->EnableWindow(FALSE);
		GetDlgItem(IDC_radNOREG)->EnableWindow(FALSE);
		GetDlgItem(IDC_edtNOREG_REASON)->EnableWindow(FALSE);
	}	
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 비고 체크박스에 체크여부에 따라 리소스를 표시한다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnchkREMARK() 
{
	m_bRemark = !m_bRemark;
	if( m_bRemark == TRUE )		GetDlgItem(IDC_edtPC_REMARKS)->EnableWindow(TRUE);
	else	GetDlgItem(IDC_edtPC_REMARKS)->EnableWindow(FALSE);

	UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 등록/미등록중 등록을 선택할경우에 따라 리소스를 표시한다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnradNOREG() 
{
	UpdateData(TRUE);

	if( m_nRegYn == 0 )		GetDlgItem(IDC_edtNOREG_REASON)->EnableWindow(FALSE);
	else GetDlgItem(IDC_edtNOREG_REASON)->EnableWindow(TRUE);

	UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 등록/미등록중 미등록을 선택할경우에 따라 리소스를 표시한다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnradREG() 
{
	UpdateData(TRUE);

	if( m_nRegYn == 0 )		GetDlgItem(IDC_edtNOREG_REASON)->EnableWindow(FALSE);
	else GetDlgItem(IDC_edtNOREG_REASON)->EnableWindow(TRUE);

	UpdateData(FALSE);
}

// 초기화
BOOL CDonateInfoChange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// SM설정
	if( InitESL_Mgr(_T("K2UPGRADE_기증정보변경")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	
	// CM/DM설정
	m_pCM = FindCM(_T("CM_기증정보변경"));		
	if( m_pCM == NULL )
	{
		ESLAfxMessageBox(_T("[CM_기증정보변경]을 설정할 수 없습니다."));
		return FALSE;
	}

	// 리소스 표시여부를 설정한다.
	OnchkDONATOR();		// 기증자
	OnchkREGYN();		// 등록/미등록
	OnchkREMARK();		// 비고
	OnchkCURRENCYCODE();// 화폐구분 2004.09.08 OnchkCURRENCYCODE() 추가.
	

	// 기증자입력여부 필수사항을 체크한다.
	SetDonatorInfo();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 일괄변경버튼
//
// [ 미니스펙 ]
// 1. 기증자정보를 체크한다.
// 2. 데이터 변경을 위하여 데이터변경클래스를 생성하고 설정한다.
// 3. 변경할 기증자정보의 조건을 설정한다.
// 4. 설정된 조건을 데이터변경클래스에 설정하고 [기증정보변경]을 처리한다.
//
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnOK() 
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT ids;
	CDataChangeManager mgr(this);
	CDonateInfoChangeCondition cont;

	UpdateData(FALSE);

	//=======================================================================
	// 1. 기증자정보를 체크한다.
	//=======================================================================
	ids = CheckDonatorInfo();
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("기증자정보가 올바르지 않습니다."));
		return;
	}
	
	//=======================================================================
	// 2. 데이터 변경을 위하여 데이터변경클래스를 생성하고 설정한다.
	//=======================================================================
	mgr.InitTempData();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM_BOOK);
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("Grid"), (DWORD*)m_pMainGrid);
	mgr.SetTempData(_T("DonatorKey"), m_strDonatorKey.GetBuffer(0));
	//=======================================================================
	// 3. 변경할 기증자정보의 조건을 설정한다.
	//=======================================================================
	cont.SetDonator(m_bDonator);
	cont.SetRegYn(m_bRegYn);
	cont.SetRemark(m_bRemark);
	cont.SetNRegYn(m_nRegYn);
	// 2004.09.08 추가.
	cont.SetCurrencyCode(m_bCurrencyCode);


	//=======================================================================
	// 4. 설정된 조건을 데이터변경클래스에 설정하고 [기증정보변경]을 처리한다.
	//=======================================================================
	mgr.SetTempPtr(_T("Condition"), (DWORD*)&cont);
	ids = mgr.SPFExecutePath(_T("기증정보변경"));
	if( ids < 0 )
	{
		//에러처리
		ESLAfxMessageBox(_T("선정하신 자료의 기증정보를 변경할 수 없습니다."));
		CDialog::OnCancel();
	}	
	ESLAfxMessageBox(_T("선정하신 자료의 기증정보를 변경하였습니다."));

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 기증자를 검색하여 선택한다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnbtnSEARCHDONATOR() 
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	CDonatorNameGetter_Search searchDlg(this);

	//=======================================================================
	// 1. 기증자 검색창을 열어서 기증자를 검색한다.
	// 2. 검색결과 중에 하나를 선택했다면 기증자 이름을 CM에 설정하고 기증자키를 멤버변수로 저장한다.
	//=======================================================================
	searchDlg.SetOpenMode(-1000);
	if( IDOK == searchDlg.DoModal() )
	{
		CDonatorNameGetterDlg searchResult(this);
		if( IDOK == searchResult.DoModal() )
		{
			m_strDonatorKey = searchResult.GetDonatorKey();
			m_strDonatorName = searchResult.GetDonatorName();
			m_pCM->SetControlMgrData(_T("BP_기증자"), m_strDonatorName);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - DB에서 기증자정보필수입력사항여부를 체크한다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::SetDonatorInfo()
{
	CString strValue;

	GetManageValue(_T("수서"), _T("공통"), _T("기증자정보필수입력사항여부"), _T(""), strValue);
	
	if( strValue == _T("Y") )	m_bDonatorInfo = TRUE;
	else						m_bDonatorInfo = FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 기증자를 체크한다.
//
// [ 미니스펙 ]
// 1. 기증자가 체크되어 있지 않다면 그냥 리턴한다.
// 2. CM에서 기증자명을 가져온다.
// 3. 기증자이름이 없는지 체크한다.
// 4. 기증자이름으로 기증자테이블에 검색한다.
// 5. 검색결과에 따라 기증자를 재설정한다.
// 6. 기증자 입력사항이 필수인지 확인하여 리턴 한다.
//
/////////////////////////////////////////////////////////////////////////////
INT CDonateInfoChange::CheckDonatorInfo()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT cnt;
	BOOL bValidDonatorInfo = FALSE;
	CString strDonatorInfo, strQuery, strTmpData;
	CESL_DataMgr* pDM = FindDM(_T("DM_기증자관리"));

	
	//=======================================================================
	// 1. 기증자가 체크되어 있지 않다면 그냥 리턴한다.
	//=======================================================================
	if( m_bDonator == FALSE )	return 0;
	
	//=======================================================================
	// 2. CM에서 기증자명을 가져온다.
	//=======================================================================
	m_pCM->GetControlMgrData(_T("BP_기증자"), strDonatorInfo);

	//=======================================================================
	// 3. 기증자이름이 없는지 체크한다.
	// 4. 기증자이름으로 기증자테이블에 검색한다.
	// 5. 검색결과에 따라 기증자를 재설정한다.
	//=======================================================================
	if( strDonatorInfo.IsEmpty() == TRUE )
	{
		bValidDonatorInfo = FALSE;
		//===========================================================
		//20080416 ADD BY PJW : 등록되지 않은 기증자일경우 리턴한다.
		//20080529 REM BY PJW : 기증자가 NULL일경우 는 -1을 리턴하지 않는다.
		//return -1;
		//===========================================================
	}
	else
	{
		// 2005-12-02 강봉수 추가
		// 기증자정보키가 없다면 다시 검색하라
		if( m_strDonatorKey.IsEmpty() == TRUE )
		{
		strQuery.Format(_T("DONATOR = '%s'"), strDonatorInfo);
		pDM->RefreshDataManager(strQuery);

			cnt = pDM->GetRowCount();
			// 검색결과 없음
			if ( cnt == 0 )
			{
				bValidDonatorInfo = FALSE;
				//===========================================================
				//20080416 ADD BY PJW : 등록되지 않은 기증자일경우 리턴한다.
				//20080529 UPDATE BY PJW : 기증자가 없을경우 에디트 박스를 NULL로 셋팅한다.
				ESLAfxMessageBox(_T("기증자정보가 올바르지 않습니다."));
				m_pCM->SetControlMgrData(_T("BP_기증자"), _T("") );
				
				//return -1;
				//===========================================================
			}
			// 검색결과 하나 - 동일한 이름 없음
			else if	( cnt == 1 )
			{
				bValidDonatorInfo = TRUE;
				m_strDonatorKey = pDM->GetCellData(_T("기증자정보KEY"), 0);
			}
			// 검색결과 다수 - 동명이인이 있음
			else if	( cnt > 1 )
			{
					// 전의 기증자와 다를경우 동명이인이라면 다시 기증자를 선택하도록 한다.
				CDonatorNameGetterDlg dlg(this);
					if( IDOK == dlg.DoModal() )
				{
					m_pCM->SetControlMgrData(_T("BP_기증자"), dlg.GetDonatorName());
					m_strDonatorKey = dlg.GetDonatorKey();
					bValidDonatorInfo = TRUE;
				}
			}
			// 검색하지 못함
			else
			{
				ESLAfxMessageBox(_T("기증자를 검색할 수 없습니다."));
				return -1;
			}
		}
		else	bValidDonatorInfo = TRUE;
	}

	//=======================================================================
	// 6. 기증자 입력사항이 필수인지 확인하여 리턴 한다.
	//=======================================================================
	if( m_bDonatorInfo == TRUE )
	{
		if( bValidDonatorInfo == TRUE )		return 0;
		else return -1;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 기증자 에디트박스 KillFocus 할때마다 기증자정보를 검사한다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnKillfocusedtDONATOR() 
{
	INT ids = CheckDonatorInfo();

	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("기증자 정보가 올바르지 않습니다."));
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 화폐구분 체크박스에 체크여부에 따라 리소스를 표시한다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnchkCURRENCYCODE() 
{
	m_bCurrencyCode = !m_bCurrencyCode;

	if( m_bCurrencyCode == TRUE )	GetDlgItem(IDC_cmbPC_CURRENCY_CLASS)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbPC_CURRENCY_CLASS)->EnableWindow(FALSE);

	UpdateData(FALSE);	
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 기증자이름을 변경한다면 기증자정보키를 지운다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CDonateInfoChange::OnChangeedtDONATOR() 
{
	CString strDonator = _T("");
	GetDlgItemText(IDC_edtDONATOR, strDonator);
	if( strDonator != m_strDonatorName )	m_strDonatorKey.Empty();
}

HBRUSH CDonateInfoChange::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
