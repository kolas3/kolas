// SEBookMarcViewer.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "SEBookMarcViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSEBookMarcViewer dialog


CSEBookMarcViewer::CSEBookMarcViewer(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSEBookMarcViewer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CSEBookMarcViewer::~CSEBookMarcViewer()
{
	
}


BOOL CSEBookMarcViewer::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CSEBookMarcViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSEBookMarcViewer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSEBookMarcViewer, CDialog)
	//{{AFX_MSG_MAP(CSEBookMarcViewer)
	ON_WM_SHOWWINDOW()
	
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEBookMarcViewer message handlers

BOOL CSEBookMarcViewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_BUSINESS_SEARCHER_04_SE") ) < 0 )
	{
		AfxMessageBox( _T("Error : InitESL_Mgr - MNG_BUSINESS_SEARCHER_04") );
		return false;
	}

	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSEBookMarcViewer::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	m_marcEditor.SubclassDlgItem( IDC_RICHEDIT1, -1, this );
	m_marcEditor.SetMarcMgr( m_pInfo->pMarcMgr );

	ViewMarc();
	
	// TODO: Add your message handler code here	
}

VOID CSEBookMarcViewer::ViewMarc()
{
	CString marc;
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_BUSINESS_SEARCHER_03_SE") );
	pDM->GetCellData( _T("S_MARC"), 0, marc );

	if( m_pInfo->pMarcMgr->Decoding( marc, &m_marc ) < 0 )
	{
		AfxMessageBox( _T("Error : Marc Decoding") );
		return;
	}

	m_marcEditor.Init( m_pInfo->pMarcMgr, &m_marc );
	m_marcEditor.Display();
}


HBRUSH CSEBookMarcViewer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
