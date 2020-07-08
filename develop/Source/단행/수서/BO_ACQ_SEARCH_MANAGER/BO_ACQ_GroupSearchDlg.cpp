// BO_ACQ_GroupSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_GroupSearchDlg.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/Group_List_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_GroupSearchDlg dialog


CBO_ACQ_GroupSearchDlg::CBO_ACQ_GroupSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CBO_SearchDlg(CBO_ACQ_GroupSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_GroupSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
}
CBO_ACQ_GroupSearchDlg::~CBO_ACQ_GroupSearchDlg()
{
}

VOID CBO_ACQ_GroupSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_GroupSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_GroupSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_GroupSearchDlg)
	ON_BN_CLICKED(IDC_btnGRPSEARCH_VIEW, OnbtnGRPSEARCHVIEW)
	ON_EN_KILLFOCUS(IDC_edtGRPSEARCH_NO, OnKillfocusedtGRPSEARCHNO)
	ON_BN_CLICKED(IDC_btnGRPSEARCH_CLEAR, OnbtnGRPSEARCHCLEAR)
	ON_BN_CLICKED(IDC_btnSET_SORT_CONDITION, OnbtnSETSORTCONDITION)
	ON_BN_CLICKED(IDC_chkSORT_YN, OnchkSORTYN)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_edtGRPSEARCH_ACQYEAR, OnKillfocusedtGRPSEARCHACQYEAR)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_GroupSearchDlg message handlers

BOOL CBO_ACQ_GroupSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_그룹검색")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM설정
	m_pCM = FindCM(_T("CM_그룹검색"));		

	//============================================
	//20080603 ADD BY PJW : 수입년을 숫자 4자리 이상 넣지않도록 수정
//	CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_edtGRPSEARCH_ACQYEAR);
//	pAcqYear->SetLimitText(4);
	//============================================

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_GroupSearchDlg::OnbtnGRPSEARCHVIEW() 
{
	// TODO: Add your control notification handler code here
	CString strCreateYear;
	m_pCM->GetControlMgrData(_T("수입년도"), strCreateYear);
	CGroup_List_Dlg dlg(this);
	dlg.SetSpeciesType(BO_DATA_IMPORT);
	dlg.SetAcqYear(strCreateYear);
	dlg.SetGroupCollection(m_pGroups);
	dlg.DoModal();
	
	m_strGroupName = dlg.GetSelInfo();
	m_pCM->SetControlMgrData(_T("그룹명"), m_strGroupName);	
	m_pCM->SetControlMgrData(_T("수입년도"), m_pGroups->GetAcqYear());
	if(!m_strGroupName.IsEmpty()) OnOK();
}

VOID CBO_ACQ_GroupSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strAcqYear, strGroupName, strOption;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("그룹명"), strGroupName);
	//입력된 그룹정보 검사
	//strGroupName = CheckValidateGroupNo(m_pCM, strGroupName, strAcqYear);

	CString strWhere;
	strWhere.Format(_T("I.REC_KEY = S.REC_KEY AND S.REC_KEY = B.SPECIES_KEY ")
		_T("AND B.SEQ_NO = G.GROUP_SEQ(+) AND I.SPECIES_CLASS IN ('0', '1') ")
		_T("AND (B.ACQ_CODE='0' OR B.ACQ_CODE IS NULL) AND B.BOOK_APPENDIX_FLAG='B' ")
		_T("AND B.WORKING_STATUS IN ('BOT111O', 'BOT112O')"));

	if(!strAcqYear.IsEmpty())
	{
		strOption.Format(_T(" AND G.CREATE_YEAR = '%s'"), strAcqYear);
		strWhere += strOption;
	}
	if(!strGroupName.IsEmpty())
	{
		strOption = _T(" AND (")+MakeGroupInfoOrQuery(m_strGroupName)+_T(")");
		strWhere += strOption;
	}

	if(strGroupName.IsEmpty())
		strWhere += _T(" AND G.GROUP_SEQ=0 ");

	SetWhereStmt(strWhere);

	::PostMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
}

VOID CBO_ACQ_GroupSearchDlg::OnKillfocusedtGRPSEARCHNO() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("그룹명"), m_strGroupName);
	
	m_strGroupName = CheckValidateGroupNo(m_pCM, m_strGroupName, strAcqYear);		
	m_pCM->SetControlMgrData(_T("그룹명"), m_strGroupName);
}

BOOL CBO_ACQ_GroupSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		GetDlgItem(IDOK)->SetFocus();
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_ACQ_GroupSearchDlg::ShowGroupInfo()
{
	CString strAcqYear;
	strAcqYear = m_pGroups->GetAcqYear();
	if(strAcqYear.IsEmpty()) strAcqYear = GetTodayYear();
	m_pGroups->GetGroupInfo(m_strGroupName);
	m_pCM->SetControlMgrData(_T("그룹명"), m_strGroupName);	
	m_pCM->SetControlMgrData(_T("수입년도"), strAcqYear);
	GetDlgItem(IDC_edtGRPSEARCH_NO)->SetFocus();
}

VOID CBO_ACQ_GroupSearchDlg::OnbtnGRPSEARCHCLEAR() 
{
	// TODO: Add your control notification handler code here
	ClearScreen(m_pCM);
	m_pGroups->RemoveAll();
	ShowGroupInfo();
}

VOID CBO_ACQ_GroupSearchDlg::OnbtnSETSORTCONDITION() 
{
	// TODO: Add your control notification handler code here
	ShowDataSortDlg();	
}

VOID CBO_ACQ_GroupSearchDlg::OnchkSORTYN() 
{
	// TODO: Add your control notification handler code here
	CButton * pBtn = (CButton*)GetDlgItem(IDC_chkSORT_YN);
	if(pBtn && pBtn->GetCheck()) m_bDataSortYN = TRUE;
	else m_bDataSortYN = FALSE;	
}

HBRUSH CBO_ACQ_GroupSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

void CBO_ACQ_GroupSearchDlg::OnKillfocusedtGRPSEARCHACQYEAR() 
{
	//============================================
	//20080603 ADD BY PJW : 수입년을 숫자 4자리를 넣지않았을 경우 4자리 숫자를 맞춰준다 
//	CString strAcqYear;
//	INT nYear;
//	CEdit * pAcqYear = (CEdit*)GetDlgItem(IDC_edtGRPSEARCH_ACQYEAR);
//	pAcqYear->GetWindowText(strAcqYear);
//	if( strAcqYear.IsEmpty() )
//	{
//		return;
//	}
//	else
//	{
//		nYear = _ttoi(strAcqYear);
//	}
//	if( nYear < 1000 || nYear > 9999 )
//	{
//		ESLAfxMessageBox(_T("1000 ~ 9999 사이의 값을 입력해주세요"));
//		pAcqYear->SetWindowText(_T(""));
//	}
	//============================================	
}
