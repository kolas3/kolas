// HoldInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HoldInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHoldInfoDlg dialog


CHoldInfoDlg::CHoldInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CHoldInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_nDMRowIdx = -1;
}

CHoldInfoDlg::~CHoldInfoDlg()
{
	
}

VOID CHoldInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHoldInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHoldInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CHoldInfoDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHoldInfoDlg message handlers
INT CHoldInfoDlg::Init()
{
	if (m_strHoldInfoFieldAlias.IsEmpty() || !m_pDM) return -1;
	
	CString strStreamMarc = m_pDM->GetCellData(m_strHoldInfoFieldAlias, m_nDMRowIdx);

	if (!m_MarcEditor.SubclassDlgItem(IDC_eHOLD_INFO_MA, -1, this)) return -1;

	if ( strStreamMarc.GetLength() <= 9999 )
	{
		INT ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_Marc);
		if (ids < 0) return ids;

		m_MarcEditor.Init(m_pInfo->pMarcMgr, &m_Marc, m_pInfo);
		m_MarcEditor.Display();
	}
	else
	{
		CString str049;			
		INT offset, i;
		offset = 0;

		for ( i=0 ; i<strStreamMarc.GetLength() ; i++ )
		{
			if ( strStreamMarc.GetAt(i) == 30 )
			{
				offset = i;
				break;
			}

		}		
		str049.Format(_T("%cl"), 31);
		strStreamMarc.Replace(str049, _T("\n     ") + str049 );		
		str049 = _T("049") + strStreamMarc.Mid(offset+1);		
		str049.Replace(_T("0491 \n     "),  _T("0491 ") );
		

		( (CEdit*) GetDlgItem(IDC_eHOLD_INFO_MA) )->SetWindowText(str049);
	}	
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CHoldInfoDlg message handlers

BOOL CHoldInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (Init() < 0)
	{
		AfxMessageBox(_T("다이얼로그 초기화 실패"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	PostMessage(WM_SIZE);	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CHoldInfoDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CWnd *pWnd = GetDlgItem(IDC_eHOLD_INFO_MA);
	if (!pWnd || !pWnd->GetSafeHwnd()) return;

	CRect rcClient;
	GetClientRect(rcClient);

	rcClient.DeflateRect(5, 5, 5, 30);
	pWnd->MoveWindow(rcClient);

	pWnd = GetDlgItem(IDOK);
	if (!pWnd) return;

	CRect rcWnd;
	pWnd->GetWindowRect(rcWnd);
	ScreenToClient(rcWnd);
	pWnd->SetWindowPos(this, rcClient.right - rcWnd.Width(), rcClient.bottom + 5, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

}


HBRUSH CHoldInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
