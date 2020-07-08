// TL_BO_LOC_LOAN_HIS.cpp : implementation file
//

#include "stdafx.h"
#include "TL_BO_LOC_LOAN_HIS.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TL_BO_LOC_LOAN_HIS dialog


TL_BO_LOC_LOAN_HIS::TL_BO_LOC_LOAN_HIS(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(TL_BO_LOC_LOAN_HIS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
}

TL_BO_LOC_LOAN_HIS::~TL_BO_LOC_LOAN_HIS()
{
}

void TL_BO_LOC_LOAN_HIS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TL_BO_LOC_LOAN_HIS)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TL_BO_LOC_LOAN_HIS, CDialog)
	//{{AFX_MSG_MAP(TL_BO_LOC_LOAN_HIS)
	ON_BN_CLICKED(IDC_btnDATE_SEARCH, OnbtnDATESEARCH)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TL_BO_LOC_LOAN_HIS message handlers

BOOL TL_BO_LOC_LOAN_HIS::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd());

	if( 0 > InitESL_Mgr(_T("TL_BO_LOC_LOAN_HIS")) )
	{
		return -1;
	}

	CTime tTo = CTime::GetCurrentTime();
	CTime tFrom( tTo.GetYear()-1, tTo.GetMonth(), tTo.GetDay(),0,0,0 );
	CDateTimeCtrl *pDateFrom	 = (CDateTimeCtrl*)GetDlgItem( IDC_datDATE1 );
	pDateFrom->SetTime( &tFrom );
	CDateTimeCtrl *pDateTo = (CDateTimeCtrl*)GetDlgItem( IDC_datDATE2 );
	pDateTo->SetTime( &tTo );

	SetDateFromTo();

	ShowLoanHistory();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT TL_BO_LOC_LOAN_HIS::ShowLoanHistory()
{
EFS_BEGIN
	
	INT		ids;

	ids = GetLoanHistory(m_strUserKey);
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("TL_BO_LOC_LOAN_HIS::ShowLoanHistory()") );

	ids = AllControlDisplay(_T("TL_CM_BO_LOC_LOAN_HIS"));
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("TL_BO_LOC_LOAN_HIS::ShowLoanHistory()") );

	return 0;
EFS_END
return -1;
}

INT TL_BO_LOC_LOAN_HIS::GetLoanHistory(CString strUserKey)
{
EFS_BEGIN
	
	INT		ids;

	if( strUserKey.IsEmpty() )	
	{
		return -1001;
	}

	CESL_DataMgr* pDM = FindDM(_T("TL_DM_BO_LOC_LOAN_HIS"));
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("TL_BO_LOC_LOAN_HIS::GetLoanHistory") );

	CString sWhere;;
	sWhere.Format(_T("CL.USER_KEY=%s"), strUserKey );

	CString sLoanDateWhere = _T("");

	if( !m_sDateTo.IsEmpty() && !m_sDateFrom.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND (CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS'))")
								, m_sDateFrom,m_sDateTo);
	}
	else if( !m_sDateFrom.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND (CL.LOAN_DATE BETWEEN TO_DATE('%s 00:00:00', 'YYYY/MM/DD HH24:MI:SS')")
								_T(" AND SYSDATE)")
								, m_sDateFrom);	
	}
	else if( !m_sDateTo.IsEmpty() )
	{
		sLoanDateWhere.Format(  _T(" AND (CL.LOAN_DATE <= TO_DATE('%s 23:59:59', 'YYYY/MM/DD HH24:MI:SS'))")
								, m_sDateTo);	
	}

	sWhere += sLoanDateWhere;

	ids = pDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("TL_BO_LOC_LOAN_HIS::GetLoanHistory") );
	

	return 0;
EFS_END
return -1;
}

VOID TL_BO_LOC_LOAN_HIS::SetDateFromTo()
{
	SYSTEMTIME tTime;
	GetLocalTime(&tTime);

	m_sDateTo.Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	
	m_sDateFrom.Format( _T("%04d/%02d/%02d"), tTime.wYear-1 , tTime.wMonth , tTime.wDay  );
}

VOID TL_BO_LOC_LOAN_HIS::SetDateFromToByCondition()
{
	SYSTEMTIME	tTime;
	DWORD		dwResult;

	CDateTimeCtrl *pDateFrom	 = (CDateTimeCtrl*)GetDlgItem( IDC_datDATE1 );
	dwResult = pDateFrom->GetTime( &tTime );
	if (dwResult != GDT_VALID )
	{
		m_sDateFrom = _T("");
	}
	else
	{
		m_sDateFrom.Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	}
	
	CDateTimeCtrl *pDateTo = (CDateTimeCtrl*)GetDlgItem( IDC_datDATE2 );
	dwResult = pDateTo->GetTime( &tTime );
	if (dwResult != GDT_VALID ) 
	{
		m_sDateTo = _T("");
	}
	else
	{
		m_sDateTo.Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	}
}

void TL_BO_LOC_LOAN_HIS::OnbtnDATESEARCH() 
{
	// TODO: Add your control notification handler code here
	SetDateFromToByCondition();
	ShowLoanHistory();
}

void TL_BO_LOC_LOAN_HIS::OnTimer(UINT nIDEvent) 
{
	if(1 == nIDEvent)
	{
		KillTimer(1);
		EndDialog(IDCANCEL);		
	}
	CDialog::OnTimer(nIDEvent);
}