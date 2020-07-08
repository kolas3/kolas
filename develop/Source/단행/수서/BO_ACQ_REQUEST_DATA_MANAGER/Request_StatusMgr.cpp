// Request_StatusMgr.cpp : implementation file
//

#include "stdafx.h"
#include "Request_StatusMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRequest_StatusMgr dialog
CRequest_StatusMgr::CRequest_StatusMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRequest_StatusMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRequest_StatusMgr)
	m_bRemarkCode = TRUE;
	m_bFurnishStatus = TRUE;
	m_strReason = _T("");
	m_bRemarkCode2 = FALSE;
	m_strReason2 = _T("");
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM = NULL;
	m_pMainCM = NULL;
}
CRequest_StatusMgr::~CRequest_StatusMgr()
{
}

VOID CRequest_StatusMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequest_StatusMgr)
	DDX_Check(pDX, IDC_chkREMARK, m_bRemarkCode);
	DDX_Check(pDX, IDC_chkFURNISH_STATUS, m_bFurnishStatus);
	DDX_Check(pDX, IDC_chkREMARK2, m_bRemarkCode2);
	DDX_Text(pDX, IDC_edtCANCEL_REASON, m_strReason2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRequest_StatusMgr, CDialog)
	//{{AFX_MSG_MAP(CRequest_StatusMgr)
	ON_CBN_SELCHANGE(IDC_cmbFURNISH_STATUS, OnSelchangecmbFURNISHSTATUS)
	ON_BN_CLICKED(IDC_chkREMARK, OnchkREMARK)
	ON_BN_CLICKED(IDC_chkFURNISH_STATUS, OnchkFURNISHSTATUS)
	ON_CBN_SELCHANGE(IDC_CMB_CANCELREASON, OnSelchangeCmbCancelreason)
	ON_BN_CLICKED(IDC_chkREMARK2, OnchkREMARK2)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRequest_StatusMgr message handlers
BOOL CRequest_StatusMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids, count;
	CString strcnt;
	CESL_Grid* pGrid = NULL;

	if( InitESL_Mgr(_T("K2UPGRADE_비치상태관리")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_비치상태관리"));
	m_pDM = FindDM(_T("DM_비치희망자료관리"));
	m_pMainCM = FindCM(_T("CM_비치희망자료관리"));
	pGrid		= (CESL_Grid*)m_pMainCM->FindControl(_T("MainGrid"));
	if( m_pCM == NULL )
	{
		ESLAfxMessageBox(_T("[CM_비치상태관리]를 설정할 수 없습니다."));
		return FALSE;
	}
	if( m_pDM == NULL || m_pMainCM == NULL || pGrid == NULL )
	{
		ESLAfxMessageBox(_T("[CM/DM_비치희망자료관리]를 설정할 수 없습니다."));
		return FALSE;
	}

	ids = pGrid->SelectMakeList();
	count = pGrid->SelectGetCount();
	if( ids < 0 || count <= 0 )
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		this->EndDialog(IDCANCEL);
	}
	
	strcnt.Format(_T("%d"), count);
	m_pCM->SetControlMgrData(_T("선택개수"), strcnt);

	CString furnish_status[4][2] = {_T("1"), _T("신청중"),
									_T("2"), _T("처리중"),
									_T("3"), _T("소장중"),
									_T("4"), _T("취소됨")};
	
	CMulitComboBoxManager mgr(this);
	mgr.SetMulTiComboBoxStrData(_T("CM_비치상태관리"), _T("소장중"), furnish_status, _T("비치상태"), 4);	
	( (CEdit*) GetDlgItem(IDC_edtCANCEL_REASON) )->SetLimitText(200);
	
	OnchkREMARK();

	OnchkFURNISHSTATUS();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;
}

VOID CRequest_StatusMgr::OnSelchangecmbFURNISHSTATUS() 
{
	CString status;
	
	CEdit* pEdt		= (CEdit*)GetDlgItem(IDC_edtCANCEL_REASON);
	CEdit* pDate	= (CEdit*)GetDlgItem(IDC_edtFURNISH_DATE);
	CButton* pStc	= (CButton*)GetDlgItem(IDC_stcFURNISH_DATE);
	
	m_pCM->GetControlMgrData(_T("비치상태"), status);

	if( status == _T("3") )
	{
		pStc->ShowWindow(SW_SHOW);
		pDate->ShowWindow(SW_SHOW);
	}
	else
	{
		pStc->ShowWindow(SW_HIDE);
		pDate->ShowWindow(SW_HIDE);
	}		
}

VOID CRequest_StatusMgr::OnOK() 
{
	INT ids;
	CString status, strFurnishDate, errstr;
	CRequestManager Mgr(this);

	if( m_bFurnishStatus == FALSE && m_bRemarkCode == FALSE && m_bRemarkCode2 == FALSE )
	{
		ESLAfxMessageBox(_T("변경사항을 선택해 주십시오."));
		return;
	}
	
	if ( m_bRemarkCode ) m_pCM->GetControlMgrData( _T("비고"), m_strReason, -1, 1 );
	if ( m_bRemarkCode2 ) ( (CEdit*) GetDlgItem (IDC_edtCANCEL_REASON) )->GetWindowText(m_strReason2);
	
	m_pCM->GetControlMgrData(_T("비치상태"), status);
	m_pCM->GetControlMgrData(_T("비치일자"), strFurnishDate);

	Mgr.InitTempData();
	Mgr.SetTempData(_T("CMAlias"),			m_pCM->CM_ALIAS.GetBuffer(0));
	Mgr.SetTempData(_T("DMAlias"),			m_pDM->DB_MGR_ALIAS.GetBuffer(0));
	Mgr.SetTempData(_T("GridAlias"),		_T("MainGrid"));
	Mgr.SetTempData(_T("MainCMAlias"),		m_pMainCM->CM_ALIAS.GetBuffer(0));
	Mgr.SetTempData(_T("Status"),			status.GetBuffer(0));
	
	if ( m_bRemarkCode == TRUE )
	{
		Mgr.SetTempData(_T("Cancel_Reason"),	m_strReason.GetBuffer(0));
	}
	else if ( m_bRemarkCode2 == TRUE )
	{
		Mgr.SetTempData(_T("Cancel_Reason"),	m_strReason2.GetBuffer(0));
	}
	else
	{
		Mgr.SetTempData(_T("Cancel_Reason"),	_T(""));
	}

	Mgr.SetTempData(_T("Furnish_Date"),		strFurnishDate.GetBuffer(0));	

	if ( m_bRemarkCode || m_bRemarkCode2 ) Mgr.m_bRemarkCode = TRUE;
	else Mgr.m_bRemarkCode = FALSE;
	
	Mgr.m_bFurnishStatus = m_bFurnishStatus;

	ids = Mgr.SPFExecutePath(_T("비치상태변경"));
	if ( ids < 0 )
	{
		switch( ids )
		{
		case -1000:		errstr = _T("비치상태 변경을 위한 DM을 찾을 수 없습니다.");
			break;
		case -1001:		errstr = _T("비치희망자료관리의 메인 그리드의 선택 리스트를 구성할 수 없습니다.");
			break;
		case -1002:		errstr = _T("비치상태 변경을 위한 쿼리를 수행할 수 없습니다.");
			break;
		case -1003:		errstr = _T("비치상태 변경에 대한 사용자 공지를 할 수 없습니다.");
			break;
		default:		errstr = _T("알 수 없는 예외발생으로 비치상태 변경을 할 수 없습니다.");
			break;
		}
		ESLAfxMessageBox(errstr.GetBuffer(0));
		return;
	}

	if ( m_bFurnishStatus == 1  )
	{
		errstr = _T("비치희망자료의 상태를 변경하였습니다.");
		ESLAfxMessageBox(errstr.GetBuffer(0));
	}
	else if ( m_bRemarkCode == 1 || m_bRemarkCode2 == 1 )
	{
		errstr = _T("비치희망자료의 비고를 변경하였습니다.");
		ESLAfxMessageBox(errstr.GetBuffer(0));		
	}
		
	CDialog::OnOK();
}

VOID CRequest_StatusMgr::OnchkREMARK() 
{
	m_bRemarkCode = !m_bRemarkCode;
	if( m_bRemarkCode == TRUE )		GetDlgItem(IDC_CMB_CANCELREASON)->EnableWindow(TRUE);
	else							GetDlgItem(IDC_CMB_CANCELREASON)->EnableWindow(FALSE);		

	( (CButton*) GetDlgItem(IDC_chkREMARK2) )->SetCheck(FALSE);
	GetDlgItem(IDC_edtCANCEL_REASON)->EnableWindow(FALSE);

	m_bRemarkCode2 = FALSE;

	UpdateData(FALSE);	
}

VOID CRequest_StatusMgr::OnchkREMARK2() 
{
	m_bRemarkCode2 = !m_bRemarkCode2;
	if( m_bRemarkCode2 == TRUE )	GetDlgItem(IDC_edtCANCEL_REASON)->EnableWindow(TRUE);
	else							GetDlgItem(IDC_edtCANCEL_REASON)->EnableWindow(FALSE);		

	( (CButton*) GetDlgItem(IDC_chkREMARK) )->SetCheck(FALSE);
	GetDlgItem(IDC_CMB_CANCELREASON)->EnableWindow(FALSE);	

	m_bRemarkCode = FALSE;

	UpdateData(FALSE);	
}

VOID CRequest_StatusMgr::OnchkFURNISHSTATUS() 
{
	m_bFurnishStatus = !m_bFurnishStatus;
	if( m_bFurnishStatus == TRUE )	
	{
		GetDlgItem(IDC_cmbFURNISH_STATUS)->EnableWindow(TRUE);
		GetDlgItem(IDC_edtFURNISH_DATE	)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_cmbFURNISH_STATUS)->EnableWindow(FALSE);
		GetDlgItem(IDC_edtFURNISH_DATE	)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);	
}

VOID CRequest_StatusMgr::OnSelchangeCmbCancelreason() 
{
	m_pCM->GetControlMgrData( _T("비고"), m_strReason, -1, 1 );
	m_pCM->SetControlMgrData( _T("취소사유"), m_strReason );
	UpdateData(FALSE);
}

HBRUSH CRequest_StatusMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
