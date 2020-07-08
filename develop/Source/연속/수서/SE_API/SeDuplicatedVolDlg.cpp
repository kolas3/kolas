// SeDuplicatedVolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeDuplicatedVolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeDuplicatedVolDlg dialog


CSeDuplicatedVolDlg::CSeDuplicatedVolDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeDuplicatedVolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeDuplicatedVolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pCM = NULL;
	m_nCurrentIdx = -1;
	m_strVOL_TITLE = _T("");

}

CSeDuplicatedVolDlg::~CSeDuplicatedVolDlg()
{
}

VOID CSeDuplicatedVolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeDuplicatedVolDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeDuplicatedVolDlg, CDialog)
	//{{AFX_MSG_MAP(CSeDuplicatedVolDlg)
	ON_BN_CLICKED(IDC_btnDUP_NEWVOL, OnbtnDUPNEWVOL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeDuplicatedVolDlg message handlers

BOOL CSeDuplicatedVolDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("연속_수서_체크인_권호복본조사"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_체크인_권호복본조사") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_연속_권호복본조사"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	m_pDM = FindDM(_T("DM_체크인_권"));
	

	if(!m_pDM || !m_pCM || !m_pGrid) {
		AfxMessageBox( _T("DM,CM,grid Error : 연속_수서_체크인_권호복본조사") );
		return false;
	}	
	
	
	m_pCM->SetControlMgrData(_T("권호명"),m_strVOL_TITLE);

	INT nRowCnt = m_pDM->GetRowCount();
	CString msg;
	msg.Format(_T("총 [ %d ] 건의 권호 복본조사결과가 있습니다."),nRowCnt);
	GetDlgItem(IDC_stcDUP_INSPECT_RESULT)->SetWindowText(msg);
	m_pGrid->Display();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CSeDuplicatedVolDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSeDuplicatedVolDlg)
	ON_EVENT(CSeDuplicatedVolDlg, IDC_gridDUP, -600 /* Click */, OnClickgridDUP, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSeDuplicatedVolDlg::OnClickgridDUP() 
{
	// TODO: Add your control notification handler code here
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		m_nCurrentIdx = row-1;
	
	}
}

VOID CSeDuplicatedVolDlg::OnOK() 
{
	// TODO: Add extra validation here
	// 기존 권호 선택 	
	if(m_nCurrentIdx<0) {
		AfxMessageBox(_T("선택된 권호가 없습니다!"));
		return;
	}
	

	CDialog::OnOK();
}

VOID CSeDuplicatedVolDlg::OnbtnDUPNEWVOL() 
{
	// TODO: Add your control notification handler code here
	// 새권호로 입력 
	EndDialog(11);

}

VOID CSeDuplicatedVolDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	// 작업중지 
	CDialog::OnCancel();
}

HBRUSH CSeDuplicatedVolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
