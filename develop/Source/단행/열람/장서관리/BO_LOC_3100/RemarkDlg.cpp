// RemarkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RemarkDlg.h"
#include "BO_LOC_31001.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemarkDlg dialog


CRemarkDlg::CRemarkDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRemarkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRemarkDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentMgr = pParent;
	m_ctlSlider = NULL;
}

CRemarkDlg::~CRemarkDlg()
{
	if( m_ctlSlider != NULL ) delete m_ctlSlider;
}

void CRemarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemarkDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRemarkDlg, CDialog)
	//{{AFX_MSG_MAP(CRemarkDlg)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnRemarkUpdate, OnbtnRemarkUpdate)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 862, OnMyMethod)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemarkDlg message handlers

BOOL CRemarkDlg::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

void CRemarkDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	//++2008.06.12 UPDATE BY PWR {{++
	CWnd* pNoteWnd = GetDlgItem( IDC_EDT_REMARKDLG_NOTE );
	CWnd* pLineWnd = GetDlgItem( IDC_STA_REMARKDLG_LINE );
	CWnd* pRemarkUpdateWnd = GetDlgItem( IDC_btnRemarkUpdate );
	if( NULL==pNoteWnd || NULL==pLineWnd || NULL==pRemarkUpdateWnd ) return;

	CRect rRemarkUpdate;
	pRemarkUpdateWnd->GetWindowRect(&rRemarkUpdate);

	HDWP hdwp = BeginDeferWindowPos(3);
	DeferWindowPos( hdwp, pNoteWnd->GetSafeHwnd(), HWND_TOP,
					3, 15, cx-41, cy-17, SWP_NOZORDER | SWP_SHOWWINDOW );
	DeferWindowPos( hdwp, pLineWnd->GetSafeHwnd(), HWND_TOP,
					2, 14, cx-39, cy-15, SWP_NOZORDER | SWP_SHOWWINDOW );
	DeferWindowPos( hdwp, pRemarkUpdateWnd->GetSafeHwnd(), HWND_TOP,
					cx-36, 14, rRemarkUpdate.Width(), rRemarkUpdate.Height(), SWP_NOZORDER | SWP_SHOWWINDOW );
	EndDeferWindowPos(hdwp);
	
	Invalidate();
	UpdateWindow();
}

BOOL CRemarkDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(IsWindowVisible())
	{
		if( WM_KEYDOWN == pMsg->message )
		{
			if( VK_ESCAPE == pMsg->wParam )
			{
				return TRUE;
			}
			else if(VK_RETURN == pMsg->wParam)
			{
				if( GetDlgItem(IDC_EDT_REMARKDLG_NOTE)->GetSafeHwnd() == pMsg->hwnd)
				{
				}
				else
				{
					return TRUE;
				}
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CRemarkDlg::OnClose() 
{
	((CBO_LOC_3100_USER_INFO_SIMPLE*)pParentMgr)->SetRemarkControl(TRUE);
	CDialog::OnClose();
}

CString CRemarkDlg::GetRemarkText()
{
	CString strText;
	GetDlgItemText(IDC_EDT_REMARKDLG_NOTE, strText);
	return strText;
}

VOID CRemarkDlg::SetRemarkText(CString strRemark)
{
	SetDlgItemText(IDC_EDT_REMARKDLG_NOTE, strRemark);
}

HBRUSH CRemarkDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
	
BOOL CRemarkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	m_ctlSlider = new CSlider;
	m_ctlSlider->Create( this, 2, 1, _T("Remark") );

	((CEdit*)GetDlgItem(IDC_EDT_REMARKDLG_NOTE))->SetLimitText(300);

	return TRUE;
}

VOID CRemarkDlg::SetTransparency(INT percent)
{
	SLWA pSetLayeredWindowAttributes = NULL; 
	HINSTANCE hmodUSER32 = LoadLibrary(_T("USER32.DLL")); 
	pSetLayeredWindowAttributes=(SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");

	HWND hwnd = this->m_hWnd;
	SetWindowLong(hwnd, GWL_EXSTYLE,GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	pSetLayeredWindowAttributes(hwnd, 0, (255 * percent) / 100, LWA_ALPHA);
}

LONG CRemarkDlg::OnMyMethod(WPARAM wParam,LPARAM lParam)
{
	INT nPercent = (INT)lParam;
	SetTransparency(nPercent);
	
	return 0;
}

void CRemarkDlg::OnbtnRemarkUpdate() 
{
	m_pParentMgr->SendMessage(WM_REMARK_UPDATE, NULL, NULL);	
}
