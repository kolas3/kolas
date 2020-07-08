// AcqInfoChange.cpp : implementation file
//

#include "stdafx.h"
#include "AcqInfoChange.h"
#include "DonateInfoChangeCondition.h"
#include "DataChangeManager.h"

//20070611 ADD PJW : 기증자 검색추가
#include "../COMMON_DONATOR_MANAGER/DonatorNameGetter_Search.h"
#include "../COMMON_DONATOR_MANAGER/DonatorNameGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAcqInfoChange dialog

// 생성자
CAcqInfoChange::CAcqInfoChange(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CAcqInfoChange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAcqInfoChange)
	m_bCurrencyCode = TRUE;  // 2005-02-22
	m_bRemark = TRUE;
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pDM_BOOK = NULL;
	m_pCM = NULL;
	m_pMainGrid = NULL;
	//20070611 ADD PJW : 기증자 검색추가
	m_bDonatorInfo = FALSE;
	m_strDonatorKey = _T("");
	m_strDonatorName = _T("");
}

CAcqInfoChange::~CAcqInfoChange()
{
}



VOID CAcqInfoChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcqInfoChange)
	DDX_Check(pDX, IDC_chkCURRENCY_CODE, m_bCurrencyCode);
	DDX_Check(pDX, IDC_chkREMARK, m_bRemark);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAcqInfoChange, CDialog)
	//{{AFX_MSG_MAP(CAcqInfoChange)
	ON_BN_CLICKED(IDC_chkCURRENCY_CODE, OnchkCURRENCYCODE)
	ON_BN_CLICKED(IDC_chkREMARK, OnchkREMARK)
	ON_BN_CLICKED(IDC_btnSEARCH_DONATOR1, OnbtnSEARCHDONATOR)
	ON_EN_KILLFOCUS(IDC_edtDONATOR1, OnKillfocusedtDONATOR)
	ON_EN_CHANGE(IDC_edtDONATOR1, OnChangeedtDONATOR)
	ON_CBN_SELCHANGE(IDC_cmbBSDIVNO_ACQ_CODE, OnSelchangecmbBSDIVNOACQCODE)
	ON_CBN_SELCHANGE(IDC_cWORK_CODE, OnSelchangecWORKCODE)
	ON_CBN_SELCHANGE(IDC_cmbBSDIVNO_GROUP_NO, OnSelchangecmbBSDIVNOGROUPNO)	
	ON_BN_CLICKED(IDC_chkDONATOR1, OnchkDONATOR)
	ON_BN_CLICKED(IDC_chkPROVIDER, OnchkPROVIDER)
	ON_BN_CLICKED(IDC_chkAcqCode, OnchkAcqCode)
	ON_WM_CTLCOLOR()
	ON_CBN_KILLFOCUS(IDC_cWORK_CODE, OnKillfocuscWORKCODE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcqInfoChange message handlers
// 초기화
BOOL CAcqInfoChange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// SM설정
	if( InitESL_Mgr(_T("K2UPGRADE_수서정보변경")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	// CM/DM설정
	m_pCM = FindCM(_T("CM_수서정보변경"));		
	if(m_pCM == NULL )
	{
		ESLAfxMessageBox(_T("[CM_수서정보변경]을 설정할 수 없습니다."));
		return FALSE;
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_기증자관리"));

	if( pDM == NULL )
	{
		GetDlgItem(IDC_chkDONATOR1)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_edtDONATOR1)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_btnSEARCH_DONATOR1)->ShowWindow(SW_HIDE);		
	}
	



	OnchkCURRENCYCODE();

	OnchkREMARK();
	//20070611 ADD PJW : 기증자 검색추가
	OnchkDONATOR();
	OnchkPROVIDER();
	
	// 기증자입력여부 필수사항을 체크한다.
	SetDonatorInfo();	

	// 2007.12.16 ADD BY PDJ
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	m_pCM->SetControlMgrData(_T("BP_수입년도"), result);
	
	//===================================
	//20080603 ADD BY PJW : 반입자료관리에서는 수입구분체크박스를 선정할수 없도록 한다.
	CESL_DataMgr* pTempDM = FindDM(_T("DM_반입자료관리"));
	if( pTempDM != NULL )
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_chkAcqCode);
		pEdit->EnableWindow(FALSE);
		GetDlgItem(IDC_chkPROVIDER)->EnableWindow(FALSE);
		GetDlgItem(IDC_cmbPC_PROVIDER)->EnableWindow(FALSE);	
	}	
	//===================================
	GetDlgItem(IDC_cmbBSDIVNO_ACQ_CODE	)->EnableWindow(FALSE);
	GetDlgItem(IDC_cmbBSDIVNO_GROUP_NO	)->EnableWindow(FALSE);
	GetDlgItem(IDC_cmbBSDIVNO_WORK_NO	)->EnableWindow(FALSE);
	GetDlgItem(IDC_cWORK_CODE			)->EnableWindow(FALSE);
	GetDlgItem(IDC_btnBSDIVNO_CREATE	)->EnableWindow(FALSE);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;
}

VOID CAcqInfoChange::OnchkCURRENCYCODE() 
{
	m_bCurrencyCode = !m_bCurrencyCode;
	if( m_bCurrencyCode == TRUE )		GetDlgItem(IDC_cmbPC_CURRENCY_CLASS)->EnableWindow(TRUE);
	else GetDlgItem(IDC_cmbPC_CURRENCY_CLASS)->EnableWindow(FALSE);

	UpdateData(FALSE);		
}

VOID CAcqInfoChange::OnchkREMARK() 
{
	m_bRemark = !m_bRemark;
	if( m_bRemark == TRUE )				GetDlgItem(IDC_edtPC_REMARKS)->EnableWindow(TRUE);
	else	GetDlgItem(IDC_edtPC_REMARKS)->EnableWindow(FALSE);

	UpdateData(FALSE);	
}

VOID CAcqInfoChange::OnOK() 
{
	// 

	INT ids;
	CDataChangeManager mgr(this);
	CDonateInfoChangeCondition cont;	
	UpdateData(TRUE);

	CString strTmpData;
	BOOL bCheck;	

	mgr.InitTempData();
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	mgr.SetTempPtr(_T("BOOK_DM"), (DWORD*)m_pDM_BOOK);
	mgr.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	mgr.SetTempPtr(_T("Grid"), (DWORD*)m_pMainGrid);

	bCheck = ( (CButton*) GetDlgItem(IDC_chkAcqCode) )->GetCheck();
	if ( bCheck == TRUE )
	{
		CString strAcqCode;
		m_pCM->GetControlMgrData(_T("BP_수입구분_코드"), strAcqCode);

		// 비치희망 정보가 있을 경우 ACQ_Key도 고려해야 한다.
		// * 수입구분 변경일 경우 구입,기증정보를 생성해야 하며
		//   기존 정보는 삭제해야 한다.
		// * 구입에서 구입 또는 기증에서 기증으로 으로 변경시
		//   차수정보가 변경되어야 하는가? 그렇다.
		// * 수입구분이 적용 안되었을 경우 
		//   기증자변경시 기능자료만
		//   구입처변경시 구입자료만 가능하도록 처리해야함.		
		mgr.SetTempData(_T("BP_수입구분"), strAcqCode.GetBuffer(0) );
				
		// 1. 구입관련 변경
		if ( strAcqCode == _T("1") )
		{			
			// 수입년도
			m_pCM->GetControlMgrData(_T("BP_수입년도"), strTmpData);
			mgr.SetTempData(_T("BP_수입년도"), strTmpData.GetBuffer(0) );
			// 업무구분
			m_pCM->GetControlMgrData(_T("BS_업무구분"), strTmpData);
			mgr.SetTempData(_T("BS_업무구분"), strTmpData.GetBuffer(0) );

			// 차수번호(그룹번호)
			m_pCM->GetControlMgrData(_T("BP_그룹번호"), strTmpData);
			mgr.SetTempData(_T("BP_그룹번호"), strTmpData.GetBuffer(0) );

			// 작업번호
			m_pCM->GetControlMgrData(_T("BP_작업번호"), strTmpData);
			mgr.SetTempData(_T("BP_작업번호"), strTmpData.GetBuffer(0) );
		}
		// 2. 기증관련 변경
		else 
		{
			// 수입년도			
			m_pCM->GetControlMgrData(_T("BP_수입년도"), strTmpData);
			mgr.SetTempData(_T("BP_수입년도"), strTmpData.GetBuffer(0) );

			// 접수번호
			m_pCM->GetControlMgrData(_T("BP_그룹번호"), strTmpData);
			mgr.SetTempData(_T("BP_그룹번호"), strTmpData.GetBuffer(0) );
		}		
	}
	
	// 구입처 변경
	bCheck = ( (CButton*) GetDlgItem(IDC_chkPROVIDER) )->GetCheck();
	if ( bCheck == TRUE )
	{
		m_pCM->GetControlMgrData(_T("BP_구입처"), strTmpData);
		mgr.SetTempData(_T("BP_구입처"), strTmpData.GetBuffer(0) );
	}

	// 기증자 변경
	bCheck = ( (CButton*) GetDlgItem(IDC_chkDONATOR1) )->GetCheck();
	if ( bCheck == TRUE )
	{
		//=======================================================================
		// 1. 기증자정보를 체크한다.
		//=======================================================================
		ids = CheckDonatorInfo();
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("기증자정보가 올바르지 않습니다."));
			return;
		}	
		
		//20070611 ADD PJW : 기증자 검색추가
		mgr.SetTempData(_T("DonatorKey"), m_strDonatorKey.GetBuffer(0) );	
		//20070611 ADD PJW : 기증자 검색추가
		cont.SetDonator(m_bDonator);
	}


	// 3. 공통정보 변경
	cont.SetCurrencyCode(m_bCurrencyCode);
	cont.SetRemark(m_bRemark);	

	mgr.SetTempPtr(_T("Condition"), (DWORD*)&cont);	
	ids = mgr.SPFExecutePath(_T("수서정보변경"));
	if(ids < 0)
	{
		//에러처리
		ESLAfxMessageBox(_T("선정하신 자료의 수서정보를 변경할 수 없습니다."));
		CDialog::OnCancel();
	}
	else 
	{
		ESLAfxMessageBox(_T("선정하신 자료의 수서정보를 변경하였습니다."));
	}

	CDialog::OnOK();
}

//20070611 ADD PJW : 기증자 검색추가

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 기증자 체크박스에 체크여부에 따라 리소스를 표시한다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CAcqInfoChange::OnchkDONATOR() 
{
	m_bDonator = ( (CButton*) GetDlgItem ( IDC_chkDONATOR1 ) )->GetCheck();

	BOOL bCheck2;
	bCheck2 = ( (CButton*) GetDlgItem(IDC_chkAcqCode) )->GetCheck();
	if ( bCheck2 && m_bDonator )
	{
		// 수입구분을 기증으로 변경할 경우 구입처를 변경하면 안된다.
		CString strAcqCode;
		m_pCM->GetControlMgrData(_T("BP_수입구분_코드"), strAcqCode);
		if ( strAcqCode == _T("1") )
		{
			( (CButton*) GetDlgItem(IDC_chkDONATOR1) )->SetCheck(FALSE);
			m_bDonator = FALSE;
			ESLAfxMessageBox(_T("수입구분이 구입으로 변경될 경우 기증자를 변경할 수 없습니다."));
			return;
		}
	}

	if( m_bDonator == TRUE ) 
	{		
		GetDlgItem(IDC_edtDONATOR1		)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnSEARCH_DONATOR1)->EnableWindow(TRUE);
		( (CButton*) GetDlgItem(IDC_chkPROVIDER) )->SetCheck(FALSE);			
		( (CComboBox*) GetDlgItem (IDC_cmbPC_PROVIDER) )->EnableWindow(FALSE);	
		ESLAfxMessageBox(_T("기증자료만 기증자 변경이 적용 됩니다."));
	}
	else
	{
		GetDlgItem(IDC_edtDONATOR1		)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnSEARCH_DONATOR1)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);	
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 기증자 에디트박스 KillFocus 할때마다 기증자정보를 검사한다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CAcqInfoChange::OnKillfocusedtDONATOR() 
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
// - 기증자를 검색하여 선택한다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CAcqInfoChange::OnbtnSEARCHDONATOR() 
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
// - 기증자이름을 변경한다면 기증자정보키를 지운다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CAcqInfoChange::OnChangeedtDONATOR() 
{
	CString strDonator = _T("");
	GetDlgItemText(IDC_edtDONATOR1, strDonator);
	if( strDonator != m_strDonatorName )	m_strDonatorKey.Empty();
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
INT CAcqInfoChange::CheckDonatorInfo()
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
			if	( cnt == 0 )
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
// - DB에서 기증자정보필수입력사항여부를 체크한다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CAcqInfoChange::SetDonatorInfo()
{
	CString strValue;

	GetManageValue(_T("수서"), _T("공통"), _T("기증자정보필수입력사항여부"), _T(""), strValue);
	
	if( strValue == _T("Y") )	m_bDonatorInfo = TRUE;
	else						m_bDonatorInfo = FALSE;
}

void CAcqInfoChange::OnSelchangecmbBSDIVNOACQCODE() 
{
	// TODO: Add your control notification handler code here
	CString strAcqCode;
	m_pCM->GetControlMgrData(_T("BP_수입구분_코드"), strAcqCode);
	
	if(strAcqCode == _T("1")) SetAcqType(PURCHASE);
	else if(strAcqCode == _T("2")) SetAcqType(DONATE);
	else if(strAcqCode.IsEmpty()) SetAcqType(ACQ_TYPE_INVALID);
	else SetAcqType(OTHER_ACQ_TYPE);
	
	
	CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)m_pCM->FindControl(_T("BP_수입구분_코드"));
	if(pCMB) pCMB->ShowDropDown(FALSE);		

	SetCtrlByAcqCode();		
}

VOID CAcqInfoChange::SetCtrlByAcqCode()
{
	switch( GetAcqType() )
	{
	case PURCHASE:
		{
			GetDlgItem(IDC_strBSFORM_GROUP_NO	)->SetWindowText(_T("차수번호"));
			GetDlgItem(IDC_strBSFORM_WORK_NO	)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_cmbBSDIVNO_WORK_NO	)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_btnBSDIVNO_CREATE	)->SetWindowText(_T("차수번호생성"));
			//( ( CComboBox*) GetDlgItem (IDC_cWORK_CODE) )->EnableWindow(TRUE);
			( ( CComboBox*) GetDlgItem (IDC_cWORK_CODE) )->ShowWindow(SW_SHOW);
			( ( CComboBox*) GetDlgItem (IDC_stcWORK_CODE) )->ShowWindow(SW_SHOW);
			// 기증자 변경을 해제
			( (CButton*) GetDlgItem(IDC_chkDONATOR1) )->SetCheck(FALSE);
			OnchkDONATOR();
			
		}
		break;
	case DONATE:
		{
			GetDlgItem(IDC_strBSFORM_GROUP_NO	)->SetWindowText(_T("접수번호"));
			GetDlgItem(IDC_strBSFORM_WORK_NO	)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_cmbBSDIVNO_WORK_NO	)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_btnBSDIVNO_CREATE	)->SetWindowText(_T("접수번호생성"));
			//( ( CComboBox*) GetDlgItem (IDC_cWORK_CODE) )->EnableWindow(FALSE);
			( ( CComboBox*) GetDlgItem (IDC_cWORK_CODE) )->ShowWindow(SW_HIDE);
			( ( CComboBox*) GetDlgItem (IDC_stcWORK_CODE) )->ShowWindow(SW_HIDE);
			//===============================================
			//20080618 ADD BY PJW : 수입구분에따라서 업무구분 셋팅한다.
			m_pCM->SetControlMgrData(_T("BS_업무구분"), _T("    "));
			//===============================================

			// 구입처 변경을 해제			
			( (CButton*) GetDlgItem(IDC_chkPROVIDER) )->SetCheck(FALSE);
			OnchkPROVIDER();
		}
		break;
	default:
		//===================================================
		//2009.10.20 ADD BY PJW : 
		{
			GetDlgItem(IDC_strBSFORM_GROUP_NO	)->SetWindowText(_T("그룹번호"));
			GetDlgItem(IDC_strBSFORM_WORK_NO	)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_cmbBSDIVNO_WORK_NO	)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_btnBSDIVNO_CREATE	)->SetWindowText(_T("그룹번호생성"));
			( ( CComboBox*) GetDlgItem (IDC_cWORK_CODE) )->ShowWindow(SW_HIDE);
			( ( CComboBox*) GetDlgItem (IDC_stcWORK_CODE) )->ShowWindow(SW_HIDE);
			m_pCM->SetControlMgrData(_T("BS_업무구분"), _T("    "));		
			( (CButton*) GetDlgItem(IDC_chkPROVIDER) )->SetCheck(FALSE);
			OnchkPROVIDER();
		}
		//===================================================
		break;
	}
	

	CString strAcqYear;
	CString strWorkCode;
	m_pCM->GetControlMgrData(_T("BP_수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("BS_업무구분"), strWorkCode);

	INT ids;
	ids = MakeGroupInfo(strAcqYear, strWorkCode);
	//=====================================================
	//20080612 ADD BY PJW : ids가 0이하로 떨어지면 업무구분을 다시 설정한다. 
	if( 0 > ids )
	{
		m_pCM->SetControlMgrData(_T("BS_업무구분"), _T("1"));
		AfxMessageBox(_T("작업번호를 구축할수 없습니다."));		
	}
	//=====================================================
}

VOID CAcqInfoChange::SetAcqType(INT nAcqType)
{
	CString strAcqCode;
	
	m_pCM->GetControlMgrData(_T("BP_수입구분_코드"), strAcqCode);		
	if ( strAcqCode == _T("") ) 
	{
		// 수입구분이 적용안함일 경우 오류임.
		// 자동 수정하여줌
		m_pCM->SetControlMgrData(_T("BP_수입구분_코드"), _T("1") );	
		//===================================================
		//2009.05.20 ADD BY PJW : 수입구분이 변경하면 업무구분도 변경되어야 한다.
		m_pCM->SetControlMgrData(_T("BS_업무구분"), _T("1"));
		//===================================================		
		strAcqCode = _T("1");
		AfxMessageBox(_T("수입구분을 적용안함으로 변경할 수 없습니다."));		
	}
	m_nAcqType = _ttoi(strAcqCode);	
}

INT CAcqInfoChange::GetAcqType()
{
	return m_nAcqType;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ] 인자로 그룹년도, 그룹번호, 업무구분으로 그룹정보를 구성한다.
// [ 미니스펙 ]
//////////////////////////////////////////////////////////////////////
INT CAcqInfoChange::MakeGroupInfo(CString strAcqYear, CString strWorkCode)
{
	// -- 수입년도와 수입구분에 따라서 차수/접수번호/그룹정보 추출한다.
	
	//================================================================
	// 변수정의
	//================================================================
	INT ids = -1;
	
	//================================================================
	// 수입년도가 없다면 에러 리턴한다.
	//================================================================
	if( strAcqYear.IsEmpty() == TRUE )
	{
		ESLAfxMessageBox(_T("수입년도가 올바르지 않습니다."));
		return -1;
	}
	
	//================================================================
	// 2. 수서 타입에 따라 그룹번호를 구성한다.
	//================================================================
	//=============================================
	//20080612 UPDATE BY PJW : ids가 -1이 떨어져도 ids로 리턴 못함 -> 가능하도록 수정
	//switch( m_nAcqType )
	//{
	//case PURCHASE:			ids = MakeDivNoInfo(strAcqYear, strWorkCode);
	//	break;
	//case DONATE:			ids = MakeReceiptNoInfo(strAcqYear);
	//	break;	
	//	if( ids < 0 )	return ids ;
	//return 0;
	//}
	switch( m_nAcqType )
	{
	case PURCHASE:			
		ids = MakeDivNoInfo(strAcqYear, strWorkCode);
		if( ids < 0 )
		{
			return ids ;
		}
		break;
	case DONATE:			
		ids = MakeReceiptNoInfo(strAcqYear);
		if( ids < 0 )
		{
			return ids ;
		}	
		break;
	default:
		ids = MakeGroupNoInfo( strAcqYear );
		if( ids < 0 )
		{
			return ids ;
		}	
	}
	if( ids < 0 )	return ids ;
	//=============================================


	return 0;
}


INT CAcqInfoChange::MakeDivNoInfo(CString strAcqYear, CString strWorkCode)
{
	CStringArray strArrayNum;
	CString strWhere;
	CString strWorkNoUseYN;
	CString strNum;
	CESL_DataMgr* pDM = FindDM(_T("DM_구입차수관리"));

	// 그룹번호 설정	
	if ( strWorkCode == _T("") ) strWhere.Format(_T("ACQ_YEAR='%s' AND WORKING_STATUS='2' AND WORK_CODE IS NULL"), strAcqYear);	
	else strWhere.Format(_T("ACQ_YEAR='%s' AND WORKING_STATUS='2' AND WORK_CODE='%s'"), strAcqYear, strWorkCode );		
	
	pDM ->RefreshDataManager(strWhere);	

	strArrayNum.RemoveAll();
	INT nRowCnt;
	nRowCnt = pDM->GetRowCount();
	//==============================================
	//20080612 ADD BY PJW : DM의 ROW가 0일경우 메세지 띄우고 빠져나간다.
	if( 0 >= nRowCnt )
	{
		return -1 ;
	}
	//==============================================
	
	for ( int i=0 ; i < nRowCnt ; i++ )
	{
		pDM->GetCellData(_T("차수번호"), i, strNum);
		strArrayNum.Add(strNum);
	}
	
	SetSingleComboBoxStrData(_T("CM_수서정보변경"), _T("0"), &strArrayNum, _T("BP_그룹번호"));

	SetWorkNo();

	return 0;
}

INT CAcqInfoChange::SetWorkNo()
{
	// 작업번호 설정
	CESL_DataMgr tmpDM;
	CStringArray strArrayNum;
	CString strAcqYear;
	CString strWorkCode;
	CString strWorkNoUseYN;
	CString strQuery;
	CString strNum;
	CString strWorkNo;
	INT		i;

	m_pCM->GetControlMgrData(_T("BP_그룹번호"), strWorkNo );	
	m_pCM->GetControlMgrData(_T("BP_수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("BS_업무구분"), strWorkCode);

	if(m_pInfo->CONNECTION_INFO.IsEmpty()) return -1;
	
	tmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);	
	if(strWorkCode.IsEmpty())
		strQuery.Format(
			_T("SELECT WORK_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE IS NULL) ORDER BY WORK_NO"),
//DEL 			_T("SELECT WORK_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE IS NULL) ORDER BY WORK_NO"),
			strAcqYear,
			strWorkNo);
	else
		strQuery.Format(
			_T("SELECT WORK_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE='%s') ORDER BY WORK_NO"),
//DEL 			_T("SELECT WORK_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE='%s') ORDER BY WORK_NO"),
			strAcqYear,
			strWorkNo, 
			strWorkCode);
	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;

	strArrayNum.RemoveAll();
	INT cnt = tmpDM.GetRowCount();	
	for( i=0 ; i<cnt ; i++)
	{
		tmpDM.GetCellData(i, 0, strNum);
		strArrayNum.Add(strNum);
	}	

	if(strWorkCode.IsEmpty())
		strQuery.Format(_T("SELECT WORK_NO_USE_YN FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE IS NULL AND MANAGE_CODE = UDF_MANAGE_CODE"), strAcqYear, strNum);
//DEL 		strQuery.Format(_T("SELECT WORK_NO_USE_YN FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE IS NULL"), strAcqYear, strNum);
	else 
//DEL 		strQuery.Format(_T("SELECT WORK_NO_USE_YN FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE = '%s'"), strAcqYear, strNum, strWorkCode);
		strQuery.Format(_T("SELECT WORK_NO_USE_YN FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strAcqYear, strNum, strWorkCode);
	ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;
	strWorkNoUseYN = tmpDM.GetCellData(0, 0);

	if(strWorkNoUseYN == _T("Y")) 
	{
		// 새로 생성 추가
		//strArrayNum.Add(_T("새로생성"));
	}
	
	SetSingleComboBoxStrData(_T("CM_수서정보변경"), _T("0"), &strArrayNum, _T("BP_작업번호"));	
	
	return 0;
}


INT CAcqInfoChange::MakeReceiptNoInfo(CString strAcqYear)
{
	CStringArray strArrayNum;
	CString strWhere;
	CString strNum;
	CESL_DataMgr* pDM = FindDM(_T("DM_접수번호관리"));
	
	strWhere.Format(_T("ACQ_YEAR='%s'"), strAcqYear );
	pDM ->RefreshDataManager(strWhere);	

	strArrayNum.RemoveAll();
	for ( int i=0 ; i<pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData(_T("접수번호"), i, strNum);
		strArrayNum.Add(strNum);
	}

	SetSingleComboBoxStrData(_T("CM_수서정보변경"), _T("0"), &strArrayNum, _T("BP_그룹번호"));			  

	// // SELECT WORK_NO FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '2007' AND SEQ_NO = 247 AND WORK_CODE='1') ORDER BY WORK_NO;

	return 0;
}

//===================================================
//2009.10.20 ADD BY PJW : 
INT CAcqInfoChange::MakeGroupNoInfo(CString strAcqYear)
{
	CStringArray strArrayNum;
	CString strWhere;
	CString strNum;
	CString strAcqCode;
	CESL_DataMgr* pDM = FindDM(_T("DM_그룹번호관리"));
	
	m_pCM->GetControlMgrData(_T("BP_수입구분_코드"), strAcqCode);
	strWhere.Format(_T("CREATE_YEAR='%s' AND ACQ_CODE = '%s'"), strAcqYear, strAcqCode );
	
	
	
	pDM ->RefreshDataManager(strWhere);	

	strArrayNum.RemoveAll();
	for ( int i=0 ; i<pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData(_T("그룹번호"), i, strNum);
		strArrayNum.Add(strNum);
	}

	SetSingleComboBoxStrData(_T("CM_수서정보변경"), _T("0"), &strArrayNum, _T("BP_그룹번호"));			  
	return 0;
}
//===================================================


INT CAcqInfoChange::SetSingleComboBoxStrData(CString strCMAlias, CString strInitVal, CStringArray * parrGroupNo, CString strCtrlAlias)
{
	CESL_ControlMgr *pCM = FindCM(strCMAlias);
	CESL_ControlComboBox *pCMB = (CESL_ControlComboBox *)pCM->FindControl(strCtrlAlias);
	pCMB->ResetContent();
	
	INT cnt = parrGroupNo->GetSize();
	CString strTmpData;
	INT nInitVal = 0;
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = parrGroupNo->GetAt(i);
		if(strInitVal == strTmpData) nInitVal = i;
		pCMB->AddString(strTmpData);
	}
	pCMB->SetCurSel(nInitVal);
	return 0;
}



void CAcqInfoChange::OnSelchangecWORKCODE() 
{
	SetCtrlByAcqCode();	
}



void CAcqInfoChange::OnSelchangecmbBSDIVNOGROUPNO() 
{
	CESL_ControlMultiComboBox* pCmb = (CESL_ControlMultiComboBox*) GetDlgItem(IDC_cmbBSDIVNO_ACQ_CODE);
 	INT nCurSel;
 	CString strCulData;
 	nCurSel = pCmb->GetCurSel();
	pCmb->GetData(strCulData, nCurSel, 0);
 	if( strCulData == _T("1") )
 	{
 		SetWorkNo();
 	}
	
}


void CAcqInfoChange::OnchkPROVIDER() 
{
	BOOL bCheck;
	bCheck = ( (CButton*) GetDlgItem(IDC_chkPROVIDER) )->GetCheck();

	BOOL bCheck2;
	bCheck2 = ( (CButton*) GetDlgItem(IDC_chkAcqCode) )->GetCheck();
	if ( bCheck2 && bCheck )
	{
		// 수입구분을 기증으로 변경할 경우 구입처를 변경하면 안된다.
		CString strAcqCode;
		m_pCM->GetControlMgrData(_T("BP_수입구분_코드"), strAcqCode);
		if ( strAcqCode == _T("2") )
		{
			( (CButton*) GetDlgItem(IDC_chkPROVIDER) )->SetCheck(FALSE);
			ESLAfxMessageBox(_T("수입구분이 기증으로 변경될 경우 구입처를 변경할 수 없습니다."));
			return;
		}
	}
	
	if ( bCheck )
	{
		( (CComboBox*) GetDlgItem (IDC_cmbPC_PROVIDER) )->EnableWindow(TRUE);	
		( (CButton*) GetDlgItem (IDC_chkDONATOR1))->SetCheck(FALSE);
		GetDlgItem(IDC_edtDONATOR1		)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnSEARCH_DONATOR1)->EnableWindow(FALSE);
		ESLAfxMessageBox(_T("구입자료만 구입처 변경이 적용 됩니다."));
	}
	else
	{
		( (CComboBox*) GetDlgItem (IDC_cmbPC_PROVIDER) )->EnableWindow(FALSE);	
	}
	
}

void CAcqInfoChange::OnchkAcqCode() 
{
	//=====================================================
	//20080618 UPDATE BY PJW : 체크박스에 따라서 컨트롤 비활성화시킨다.
	//SetCtrlByAcqCode();
	BOOL bCheck;
	bCheck = ( (CButton*) GetDlgItem(IDC_chkAcqCode) )->GetCheck();

	if(bCheck)
	{
		//===================================================
		//2009.03.28 UPDATE BY LKS : 체크박스 클릭시 작업번호 구축안되어있으므로 제외
		//SetCtrlByAcqCode();
		//===================================================
		GetDlgItem(IDC_cmbBSDIVNO_ACQ_CODE	)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnBSDIVNO_CREATE	)->EnableWindow(TRUE);
		GetDlgItem(IDC_cmbBSDIVNO_GROUP_NO	)->EnableWindow(TRUE);
		GetDlgItem(IDC_cmbBSDIVNO_WORK_NO	)->EnableWindow(TRUE);
		GetDlgItem(IDC_cWORK_CODE			)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_cmbBSDIVNO_ACQ_CODE	)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnBSDIVNO_CREATE	)->EnableWindow(FALSE);
		GetDlgItem(IDC_cmbBSDIVNO_GROUP_NO	)->EnableWindow(FALSE);
		GetDlgItem(IDC_cmbBSDIVNO_WORK_NO	)->EnableWindow(FALSE);
		GetDlgItem(IDC_cWORK_CODE			)->EnableWindow(FALSE);
	}
	//=====================================================

	//===================================================
	//2009.09.15 UPDATE BY LKS : Default로 1번째꺼 선정
	( (CComboBox*) GetDlgItem (IDC_cmbBSDIVNO_ACQ_CODE) )->SetCurSel(0);
	//===================================================
	OnSelchangecmbBSDIVNOACQCODE();
	//===================================================
				
}

HBRUSH CAcqInfoChange::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
//=================================================
//20080612 ADD BY PJW : 킬포커스 일어 날때도 차수유효성을 검사한다.
void CAcqInfoChange::OnKillfocuscWORKCODE() 
{
	SetCtrlByAcqCode();	
}
//=================================================
