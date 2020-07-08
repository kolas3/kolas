// ReserveSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReserveSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReserveSearchDlg dialog


CReserveSearchDlg::CReserveSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CReserveSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReserveSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pLocProc = NULL;
	m_strCivilNoHideYN = _T("N");

	m_OneLineReserve = FALSE;
}
CReserveSearchDlg::~CReserveSearchDlg()
{
}

void CReserveSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReserveSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReserveSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CReserveSearchDlg)
	ON_BN_CLICKED(IDC_btnRS_Search, OnbtnRSSearch)
	ON_BN_CLICKED(IDC_btnRS_Close, OnbtnRSClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReserveSearchDlg message handlers

BOOL CReserveSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());
	INT ids;
	ids = InitESL_Mgr(_T("SM_RESERVE_SEARCH"));
	if(0 > ids) return FALSE;

	((CButton*)GetDlgItem(IDC_radRS_NotNull))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_radRS_Null))->SetCheck(FALSE);

	m_strWhere = _T("");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CReserveSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CReserveSearchDlg::OnbtnRSSearch() 
{
	SearchStart();
}

void CReserveSearchDlg::OnbtnRSClose() 
{
	OnCancel();
}

INT CReserveSearchDlg::SearchStart()
{
	CString strBefore, strAfter;
	GetControlData(_T("CM_RESERVE_SEARCH"), _T("조회기간1"), strBefore);
	GetControlData(_T("CM_RESERVE_SEARCH"), _T("조회기간2"), strAfter);
	if(10 != strBefore.GetLength()) 
	{
		strBefore = _T("");
		SetControlData(_T("CM_RESERVE_SEARCH"), _T("조회기간1"), strBefore);
	}
	if(10 != strAfter.GetLength())
	{
		strAfter = _T("");
		SetControlData(_T("CM_RESERVE_SEARCH"), _T("조회기간2"), strAfter);
	}

	CString strWhere;
	if(!strBefore.IsEmpty() && !strAfter.IsEmpty())
	{
		strWhere.Format(_T("CL.RESERVATION_DATE BETWEEN TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS') AND TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), strBefore, strAfter);
	}
	else if(!strBefore.IsEmpty() && strAfter.IsEmpty())
	{
		strWhere.Format(_T("CL.RESERVATION_DATE > TO_DATE('%s 00:00:00','YYYY/MM/DD HH24:MI:SS')"), strBefore);
	}
	else if(strBefore.IsEmpty() && !strAfter.IsEmpty())
	{
		strWhere.Format(_T("CL.RESERVATION_DATE < TO_DATE('%s 23:59:59','YYYY/MM/DD HH24:MI:SS')"), strAfter);
	}
	else
	{
		strWhere = _T("");
	}

	if(!strWhere.IsEmpty())
	{
		strWhere += _T(" AND ");
	}
	if(TRUE == ((CButton*)GetDlgItem(IDC_radRS_NotNull))->GetCheck())
	{
		strWhere += _T("CL.RESERVATION_EXPIRE_DATE IS NOT NULL");
	}
	else
	{
		strWhere += _T("CL.RESERVATION_EXPIRE_DATE IS NULL");
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_RESERVE"));
	if(NULL == pDM) return -1;
	pDM->RefreshDataManager(strWhere);
	CESL_DataMgr *pSeDM = FindDM(_T("DM_BO_LOC_RESERVE_SE"));
	if(NULL == pSeDM) return -1;
	pSeDM->RefreshDataManager(strWhere);
	pDM->AddDM(pSeDM);
	pDM->SORT(_T("예약일"));

	SetOverduePeriod(pDM);

	m_strWhere = strWhere;

	if(_T("Y") != m_strCivilNoHideYN)
	{
		CString strGetData = _T("");
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("대출자번호"), i, strGetData);
			m_pLocProc->m_pLocCommonAPI->ChangeSecuriyUserNo(this, strGetData, m_pLocProc->m_pManageValue->m_sIsUnityLoanService);
			pDM->SetCellData(_T("대출자번호"), strGetData, i);
		}
	}

	ControlDisplay(_T("CM_BO_LOC_RESERVE"), _T("MainGrid"));

	if(1 > pDM->GetRowCount())
	{
		AfxMessageBox(_T("검색결과가 없습니다."));
	}

	return 0;
}

INT CReserveSearchDlg::SetOverduePeriod( CESL_DataMgr* pDM ){
	
	if( pDM == NULL || pDM->GetRowCount() < 0 ) return -1;
	
	COleDateTime tCurrentTime = COleDateTime::GetCurrentTime();
	tCurrentTime = COleDateTime( tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 0, 0, 0 );
	
	COleDateTimeSpan tDiffTime;
	
	long lDiffDays;
	CString strQuery = _T(""),strData= _T(""),strTemp = _T(""),strReturnPlanDate = _T("");
	for (int i = 0 ; i < pDM->GetRowCount() ; i++)
	{
		
		pDM->GetCellData(_T("반납예정일"), i, strReturnPlanDate);
		
		if( strReturnPlanDate.GetLength() != 10 ) 
		{
			pDM->SetCellData(_T("연체일수"), _T("0"), i);
			continue;
		}
		
		if( m_OneLineReserve )
		{	
			pDM->GetCellData(_T("한줄서기비치중도서"), i, strData);
			if( strData != _T("") && strData.GetLength() > 0 ){
				
				if( _ttoi(strData) > 0 ) {
					pDM->SetCellData(_T("연체일수"), _T("0"), i);
					continue;
				}
			}		
		}
		
		COleDateTime tReturnPlanDate ( _ttoi(strReturnPlanDate.Mid(0,4)), _ttoi(strReturnPlanDate.Mid(5,2)) , _ttoi(strReturnPlanDate.Mid(8,2)), 0, 0, 0 );
		tDiffTime = tCurrentTime - tReturnPlanDate;
		lDiffDays = tDiffTime.GetDays();
		
		if(lDiffDays <= 0) {
			pDM->SetCellData(_T("연체일수"), _T("0"), i);
			continue;
		}
		strTemp.Format(_T("%d"),lDiffDays);
		pDM->SetCellData(_T("연체일수"), strTemp == _T("") ? _T("0") : strTemp , i);		
	}
	
	
	return 0;
}
