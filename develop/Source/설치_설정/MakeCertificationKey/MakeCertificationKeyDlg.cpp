// MakeCertificationKeyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MakeCertificationKey.h"
#include "MakeCertificationKeyDlg.h"

#include "..\\..\\ESL\\CHECK_CERTIFICATION\\CERTIFICATION_Mgr.h"
#include "MAKEKEY_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual VOID DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

VOID CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakeCertificationKeyDlg dialog

CMakeCertificationKeyDlg::CMakeCertificationKeyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMakeCertificationKeyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMakeCertificationKeyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pDM = NULL ;
	m_pGrid = NULL ;


}
CMakeCertificationKeyDlg::~CMakeCertificationKeyDlg()
{
	if(m_pDM )
	{
		delete m_pDM ;
		m_pDM = NULL ;
	}

	if(m_pGrid )
	{
		delete m_pGrid ;
		m_pGrid = NULL ;
	}

}



VOID CMakeCertificationKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMakeCertificationKeyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMakeCertificationKeyDlg, CDialog)
	//{{AFX_MSG_MAP(CMakeCertificationKeyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bMAKEKEY, OnbMAKEKEY)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_BN_CLICKED(IDC_bGETDATA, OnbGETDATA)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakeCertificationKeyDlg message handlers

BOOL CMakeCertificationKeyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add _T("About...") menu item to _tsystem menu.

	// IDM_ABOUTBOX must be in the _tsystem command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's _tmain window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	m_pDM = new CESL_DataMgr ;
	m_pDM->m_dbkind = 1 ;
	m_pDM->SetCONNECTION_INFO (_T("SMARTU@SMARTU/SMARTU#[]")) ;


	InitGrid();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CMakeCertificationKeyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CMakeCertificationKeyDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		INT cxIcon = GetSystemMetrics(SM_CXICON);
		INT cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		INT x = (rect.Width() - cxIcon + 1) / 2;
		INT y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The _tsystem calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMakeCertificationKeyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CMakeCertificationKeyDlg::OnbMAKEKEY() 
{
	// TODO: Add your control notification handler code here
	CMAKEKEY_DLG dlg ;
	dlg.DoModal () ;
}

VOID CMakeCertificationKeyDlg::OnbSEARCH() 
{
	// TODO: Add your control notification handler code here
	


	CString strQuery ;
	strQuery = _T(" SELECT REC_KEY,LIB_CODE ,COMPUTER_ID,CERTIFICATION_KEY,REGISTER ,FIRST_DATE,LAST_DATE ")
		       _T(" FROM K2L_CERTIFICATION_TBL ORDER BY LAST_DATE DESC")  ;

	m_pDM->RestructDataManager (strQuery) ;
	
	INT nRowCnt = m_pDM->GetRowCount () ;

	if(nRowCnt == 0)
	{
		AfxMessageBox (_T("검색 데이타가 없습니다.")) ;
		return ;
	}


	m_pGrid->SetRows(nRowCnt +1) ;


	CString strTemp = _T("") ;
	for(INT i = 0 ; i < nRowCnt ; i++)
	{
		for(INT j = 0 ; j < 6 ; j++)
		{
			strTemp = m_pDM->GetCellData (i,j+1) ;
			m_pGrid->SetTextMatrix (i+1,j+1,strTemp) ;
		}
	}
	//도서관부호
	//컴퓨터정보
	//인증키 
	//설치자
	//최초작업일
	//마지막작업일

	//AfxMessageBox (m_pDM->GetCellData (1,1)) ;

}

VOID CMakeCertificationKeyDlg::OnbDELETE() 
{
	// TODO: Add your control notification handler code here
	
	CString strRecKey ;
	INT nDMIndex ;

	nDMIndex = m_pGrid->GetIdx () ;

	if(m_pDM->GetRowCount () == 0)
	{
		return  ;
	}

	strRecKey = m_pDM->GetCellData (nDMIndex ,0) ;

	CString strQuery ;
	strQuery.Format (_T("DELETE FROM  K2L_CERTIFICATION_TBL WHERE REC_KEY = %s ; "),strRecKey ) ;

    m_pDM->StartFrame () ;
	m_pDM->AddFrame (strQuery) ;
	m_pDM->SendFrame () ;
	m_pDM->EndFrame () ;

    
	SearchFunc () ;


}

VOID CMakeCertificationKeyDlg::OnbGETDATA() 
{
	// TODO: Add your control notification handler code here
	INT ids = ::WinExec(_T("GetComputerID.exe"), SW_SHOW);	
}
VOID CMakeCertificationKeyDlg::InitGrid (VOID)
{

	m_pGrid = new CESL_Grid(NULL) ;
	m_pGrid->SubclassDlgItem (IDC_MAIN_GRID,this) ;


	m_pGrid->RemoveAll();
	m_pGrid->SetFixedRows(1);
	m_pGrid->SetFixedCols(1);
	m_pGrid->SetRows(2);
	m_pGrid->SetCols(0, 7);
	m_pGrid->SetColWidth(0, 0, 30*15);
	
	m_pGrid->SetColWidth(0, 0, 30*30);
	for(INT i = 1; i < 7 ; i++)
	{
		if(i == 3 )
		{
			m_pGrid->SetColWidth(i, 0, 30*120);
			continue ;
		}
		m_pGrid->SetColWidth(i, 0, 30*50);

	}
	
	m_pGrid->SetTextMatrix(0, 1, _T("도서관부호"));
	m_pGrid->SetTextMatrix(0, 2, _T("컴퓨터정보"));
	m_pGrid->SetTextMatrix(0, 3, _T("인증키"));
    m_pGrid->SetTextMatrix(0, 4, _T("설치자"));

	m_pGrid->SetTextMatrix(0, 5, _T("최초작업일"));
	m_pGrid->SetTextMatrix(0, 6, _T("마지막작업일"));
	
   
	m_pGrid->SetColAlignmentBand (0,1,1) ;

	return;
}

BEGIN_EVENTSINK_MAP(CMakeCertificationKeyDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMakeCertificationKeyDlg)
	ON_EVENT(CMakeCertificationKeyDlg, IDC_MAIN_GRID, -601 /* DblClick */, OnDblClickMainGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMakeCertificationKeyDlg::OnDblClickMainGrid() 
{
	// TODO: Add your control notification handler code here
	
	//AfxMessageBox (_T("db click")) ;
	
	CString strRecKey ;
	INT nDMIndex ;

	nDMIndex = m_pGrid->GetIdx () ;
	
	strRecKey = m_pDM->GetCellData (nDMIndex ,0) ;


	CMAKEKEY_DLG dlg ;
	dlg.SetRecKey (strRecKey) ;
	dlg.SetModifyScreen (false) ;
	dlg.DoModal () ;



}

VOID CMakeCertificationKeyDlg::SearchFunc()
{
	// TODO: Add your control notification handler code here
	

	CString strQuery ;
	strQuery = _T(" SELECT REC_KEY,LIB_CODE ,COMPUTER_ID,CERTIFICATION_KEY,REGISTER ,FIRST_DATE,LAST_DATE ")
		       _T(" FROM K2L_CERTIFICATION_TBL ORDER BY LAST_DATE DESC")  ;

	m_pDM->RestructDataManager (strQuery) ;
	
	INT nRowCnt = m_pDM->GetRowCount () ;

	if(nRowCnt == 0)
	{
		AfxMessageBox (_T("검색 데이타가 없습니다.")) ;
		return ;
	}


	m_pGrid->SetRows(nRowCnt +1) ;


	CString strTemp = _T("") ;
	for(INT i = 0 ; i < nRowCnt ; i++)
	{
		for(INT j = 0 ; j < 6 ; j++)
		{
			strTemp = m_pDM->GetCellData (i,j+1) ;
			m_pGrid->SetTextMatrix (i+1,j+1,strTemp) ;
		}
	}
	//도서관부호
	//컴퓨터정보
	//인증키 
	//설치자
	//최초작업일
	//마지막작업일

	//AfxMessageBox (m_pDM->GetCellData (1,1)) ;

}
