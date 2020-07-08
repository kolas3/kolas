// UserMainMarcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserMainMarcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserMainMarcDlg dialog


CUserMainMarcDlg::CUserMainMarcDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CUserMainMarcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserMainMarcDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
}


void CUserMainMarcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserMainMarcDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserMainMarcDlg, CDialog)
	//{{AFX_MSG_MAP(CUserMainMarcDlg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_MARC_btnOK, OnMARCbtnOK)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserMainMarcDlg message handlers

BOOL CUserMainMarcDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr(_T("SM_USERMAIN_MARC")) < 0)
 	{
 		AfxMessageBox(_T("InitESL_Mgr Error!"));
 		return FALSE;
 	}

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserMainMarcDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	m_marcEditor.SubclassDlgItem( IDC_MARC_richSPECIES_MARC, -1, this );
	m_marcEditor.SetMarcMgr( m_pInfo->pMarcMgr );

	ViewMarc();
}

BOOL CUserMainMarcDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CUserMainMarcDlg::ViewMarc()
{
	CString marc;
	CString sDMAlias;
	if(_T("MO") == m_sPublishFormCode)
	{
		sDMAlias = _T("DM_USERMAIN_DETAIL_BO");
	}
	else
	{
		sDMAlias = _T("DM_USERMAIN_DETAIL_SE");
	}
	CESL_DataMgr *pDM = FindDM(sDMAlias);
	pDM->GetCellData( _T("S_MARC"), 0, marc );

	if( m_pInfo->pMarcMgr->Decoding( marc, &m_marc ) < 0 )
	{
		AfxMessageBox( _T("Error : Marc Decoding") );
		return;
	}

	m_marcEditor.Init( m_pInfo->pMarcMgr, &m_marc );
	m_marcEditor.Display();
}

void CUserMainMarcDlg::OnMARCbtnOK() 
{
	OnOK();	
}

void CUserMainMarcDlg::OnClose() 
{
	
	CDialog::OnClose();
}
