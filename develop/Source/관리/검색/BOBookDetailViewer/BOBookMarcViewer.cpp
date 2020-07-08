// BOBookMarcViewer.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BOBookMarcViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBOBookMarcViewer dialog


CBOBookMarcViewer::CBOBookMarcViewer(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBOBookMarcViewer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
}
CBOBookMarcViewer::~CBOBookMarcViewer()
{
	
}
BOOL CBOBookMarcViewer::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CBOBookMarcViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOBookMarcViewer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBOBookMarcViewer, CDialog)
	//{{AFX_MSG_MAP(CBOBookMarcViewer)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBOBookMarcViewer message handlers

BOOL CBOBookMarcViewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_BUSINESS_SEARCHER_04") ) < 0 )
	{
		AfxMessageBox( _T("Error : InitESL_Mgr - MNG_BUSINESS_SEARCHER_04") );
		return false;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBOBookMarcViewer::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	m_marcEditor.SubclassDlgItem( IDC_richSPECIES_MARC, -1, this );
	m_marcEditor.SetMarcMgr( m_pInfo->pMarcMgr );

	ViewMarc();
	
	// TODO: Add your message handler code here	
}

VOID CBOBookMarcViewer::ViewMarc()
{
	CString marc;
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_BUSINESS_SEARCHER_03_BO") );
	pDM->GetCellData( _T("S_MARC"), 0, marc );

	if( m_pInfo->pMarcMgr->Decoding( marc, &m_marc ) < 0 )
	{
		AfxMessageBox( _T("Error : Marc Decoding") );
		return;
	}

	m_marcEditor.Init( m_pInfo->pMarcMgr, &m_marc );
	m_marcEditor.Display();
}

HBRUSH CBOBookMarcViewer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
