// SMTPLOGDLG.cpp : implementation file
//

#include "stdafx.h"
#include "SMTPLOGDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMTPLOGDLG dialog


CSMTPLOGDLG::CSMTPLOGDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CSMTPLOGDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMTPLOGDLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSMTPLOGDLG::~CSMTPLOGDLG()
{
	m_Dlg_brush.DeleteObject();
}

VOID CSMTPLOGDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMTPLOGDLG)
	DDX_Control(pDX, IDC_eLOG_VIEW, m_ctlLogView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMTPLOGDLG, CDialog)
	//{{AFX_MSG_MAP(CSMTPLOGDLG)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMTPLOGDLG message handlers

BOOL CSMTPLOGDLG::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CSMTPLOGDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CStdioFile	logfile;	
	CString		sCurDate;
	CString		strLine;
	CString		strLog;

	CTime tTime = CTime::GetCurrentTime();
	sCurDate.Format( _T("..\\tunning\\(%04d%02d%02d)SmtpLog.txt"), tTime.GetYear( ), tTime.GetMonth( ), tTime.GetDay( ) );
	
	if ( !logfile.Open(sCurDate, CFile::modeRead | CFile::typeBinary ) )
	{
		strLog = _T("No log...");
	}
	else
	{
		TCHAR cUni;
		logfile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			logfile.SeekToBegin();
		}
		
		strLog = _T("");
		strLine = _T("");
		while( logfile.ReadString( strLine ) )
		{			
			strLog += strLine + _T("\r\n");	
		}		
	}
	if ( logfile.m_pStream != NULL ) logfile.Close();
	m_ctlLogView.SetWindowText(strLog);
	m_ctlLogView.SendMessage(WM_VSCROLL, SB_BOTTOM, 0 );

	m_Dlg_brush.CreateSolidBrush(RGB(239, 239, 239)); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CSMTPLOGDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = NULL;
	if(pWnd->m_hWnd == this->m_hWnd) hbr = HBRUSH(m_Dlg_brush);
	else hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(RGB(239, 239, 239));

		return m_Dlg_brush;
	}

	return hbr;
}
