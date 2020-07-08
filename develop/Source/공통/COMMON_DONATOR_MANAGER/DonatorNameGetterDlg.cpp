// DonatorNameGetterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DonatorNameGetterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDonatorNameGetterDlg dialog


CDonatorNameGetterDlg::CDonatorNameGetterDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDonatorNameGetterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonatorNameGetterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;
}

CDonatorNameGetterDlg::~CDonatorNameGetterDlg()
{
	
}


VOID CDonatorNameGetterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonatorNameGetterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonatorNameGetterDlg, CDialog)
	//{{AFX_MSG_MAP(CDonatorNameGetterDlg)
	ON_BN_CLICKED(IDC_btnDONATOR_INPUT, OnbtnDONATORINPUT)
	ON_BN_CLICKED(IDC_btnDONATOR_MODIFY, OnbtnDONATORMODIFY)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonatorNameGetterDlg message handlers

BOOL CDonatorNameGetterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	
	// TODO: Add extra initialization here
	if( InitESL_Mgr( _T("K2UPGRADE_기증자검색결과") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error") );
		return false;
	}
	GetWindowRect(&m_rcInitial);
	m_pCM = FindCM(_T("CM_기증자검색결과"));
	m_pDM = FindDM(_T("DM_기증자관리"));	
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));

	
	//2019.07.17 JOB.2019.0059 ADD BY KYJ : 로그 기록시 사유 추가
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("검색 기증자목록");
	}

	m_pGrid->Display();
	
	PostMessage(WM_SIZE);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDonatorNameGetterDlg::OnOK() 
{
	// TODO: Add extra validation here
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));

	INT ids = pGrid->SelectMakeList();
	INT i = pGrid->SelectGetHeadPosition();
	if(ids < 0 || i < 0)  
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	
	if(pGrid->SelectGetCount() > 1)
		ESLAfxMessageBox(_T("처음 선정된 자료만 반영됩니다."));
	
	m_pDM->GetCellData(_T("기증자정보KEY"), i, m_strDonatorKey);
	m_pDM->GetCellData(_T("기증자"), i, m_strDonatorName);

	CDialog::OnOK();
}

BEGIN_EVENTSINK_MAP(CDonatorNameGetterDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDonatorNameGetterDlg)
	ON_EVENT(CDonatorNameGetterDlg, IDC_gridDONATOR, -601 /* DblClick */, OnDblClickgridDONATOR, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CDonatorNameGetterDlg::OnDblClickgridDONATOR() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	if(pGrid->GetMouseRow() == 0) return ;

	if(m_pDM->GetRowCount() == 0) return ;

	pGrid->SetAt(pGrid->GetRow()-1, 0, _T("V"));
	OnOK();
}

VOID CDonatorNameGetterDlg::OnbtnDONATORINPUT() 
{
	ShowDonatorMgrDlg(SINSERT);
}

VOID CDonatorNameGetterDlg::OnbtnDONATORMODIFY() 
{
	ShowDonatorMgrDlg(SUPDATE);
}

VOID CDonatorNameGetterDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	
	CRect rcWnd;
	GetWindowRect(rcWnd);
	
	if (rcWnd.Width() < m_rcInitial.Width())
	{
		rcWnd.right = rcWnd.left + m_rcInitial.Width();
		if (rcWnd.Height() < m_rcInitial.Height())
			rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		
		MoveWindow(rcWnd);
		return;
	}
	
	if (rcWnd.Height() < m_rcInitial.Height())
	{
		rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		if (rcWnd.Width() < m_rcInitial.Width())
			rcWnd.right = rcWnd.left + m_rcInitial.Width();			
		
		MoveWindow(rcWnd);
		return;
	}
	
	if(m_pGrid->GetSafeHwnd() == NULL) return;
	
	CRect rect;
	GetClientRect(rect);
	rect.top += 50;
	rect.bottom -= 4;
	rect.left += 4;
	rect.right -= 4;
	
	m_pGrid->MoveWindow(rect);	
}

VOID CDonatorNameGetterDlg::ShowDonatorMgrDlg(OPERATION_TYPE nOpType)
{
	INT nRowCnt;
	CDonatorMgr dlg(this);
	
	nRowCnt = m_pDM->GetRowCount();
	if( nRowCnt == 0 && nOpType != SINSERT )	return;
	
	dlg.SetOpenMode(nOpType);
	dlg.SetMainCMAlias(m_pCM->CM_ALIAS);
	dlg.DoModal();

	if(nOpType == SINSERT)
	{
		INT nCurCnt = m_pDM->GetRowCount();
		if(nCurCnt > nRowCnt)
		{
			m_pGrid->DisplayNumber();
			m_pGrid->SetTopRow(nRowCnt+1);
		}
	}
}



HBRUSH CDonatorNameGetterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;

}
