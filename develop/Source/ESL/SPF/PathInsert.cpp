// PathInsert.cpp : implementation file
//

#include "stdafx.h"
#include "SPF.h"
#include "PathInsert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPathInsert dialog


CPathInsert::CPathInsert(CWnd* pParent /*=NULL*/)
	: CDialog(CPathInsert::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPathInsert)
	m_BACKNAME = _T("");
	m_BACKDESC = _T("");
	m_NAME = _T("");
	m_DESC = _T("");
	m_use_back = FALSE;
	m_FILE = _T("");
	m_FUNCTION = _T("");
	m_BACKFILE = _T("");
	m_BACKFUNCTION = _T("");
	m_Developer = _T("");
	m_IsActivated = FALSE;
	m_Backdeveloper = _T("");
	m_BackDvmAlias = _T("");
	m_DvmAlias = _T("");
	//}}AFX_DATA_INIT
    
	ModifyFlag = 0;
}


VOID CPathInsert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPathInsert)
	DDX_Control(pDX, IDC_DVM_ALIAS, m_DvmAliasCtrl);
	DDX_Control(pDX, IDC_BACK_DVM_ALIAS, m_BackDvmAliasCtrl);
	DDX_Control(pDX, IDC_DEVELOPER, m_DeveloperCtrl);
	DDX_Control(pDX, IDC_BACKDEVELOPER, m_BackDeveloperCtrl);
	DDX_Control(pDX, IDC_BACKPATH_FUNCTION, m_BACKFUNCTIONCTRL);
	DDX_Control(pDX, IDC_BACKPATH_FILE, m_BACKFILECTRL);
	DDX_Control(pDX, IDC_PATH_FUNCTION, m_FUNCTIONCTRL);
	DDX_Control(pDX, IDC_PATH_FILE, m_FILECTRL);
	DDX_Control(pDX, IDC_USE_BACK, m_Use_backCtrl);
	DDX_Control(pDX, IDC_PATH_NAME, m_NAMECTRL);
	DDX_Control(pDX, IDC_BACKPATH_DESC, m_BACKDESCCTRL);
	DDX_Control(pDX, IDC_BACKPATH_NAME, m_BACKNAMECTRL);
	DDX_Control(pDX, IDC_PATH_DESC, m_DESCCTRL);
	DDX_Control(pDX, IDC_tabPATH, m_ctrlTABPATH);
	DDX_Text(pDX, IDC_BACKPATH_NAME, m_BACKNAME);
	DDX_Text(pDX, IDC_BACKPATH_DESC, m_BACKDESC);
	DDX_Text(pDX, IDC_PATH_NAME, m_NAME);
	DDX_Text(pDX, IDC_PATH_DESC, m_DESC);
	DDX_Check(pDX, IDC_USE_BACK, m_use_back);
	DDX_Text(pDX, IDC_PATH_FILE, m_FILE);
	DDX_Text(pDX, IDC_PATH_FUNCTION, m_FUNCTION);
	DDX_Text(pDX, IDC_BACKPATH_FILE, m_BACKFILE);
	DDX_Text(pDX, IDC_BACKPATH_FUNCTION, m_BACKFUNCTION);
	DDX_Text(pDX, IDC_DEVELOPER, m_Developer);
	DDX_Check(pDX, IDC_ISACTIVATED, m_IsActivated);
	DDX_Text(pDX, IDC_BACKDEVELOPER, m_Backdeveloper);
	DDX_Text(pDX, IDC_BACK_DVM_ALIAS, m_BackDvmAlias);
	DDX_Text(pDX, IDC_DVM_ALIAS, m_DvmAlias);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPathInsert, CDialog)
	//{{AFX_MSG_MAP(CPathInsert)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabPATH, OnSelchangetabPATH)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPathInsert message handlers

BOOL CPathInsert::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    
	//tabø° ∫Ÿ¿œ index setting
	if( ModifyFlag )
	{
		m_Use_backCtrl.ShowWindow(SW_HIDE);
	}

	if( ModifyFlag == 1 )
	{

		TCHAR scTABINDEX[][7] = {_T("FORTH")};
		TC_ITEM item;
		item.mask = TCIF_TEXT;
    	item.pszText = scTABINDEX[0];
		m_ctrlTABPATH.InsertItem(0,&item);
	
	}
	else
	{

		TCHAR scTABINDEX[][7] = {_T("FORTH"), _T("BACK")};
		TC_ITEM item;
		item.mask = TCIF_TEXT;
    
		for(INT i=0; i<2; i++)
		{
			item.pszText = scTABINDEX[i];
			m_ctrlTABPATH.InsertItem(i,&item);
		}
	}

    m_NAMECTRL.ShowWindow(SW_SHOW);
    m_DESCCTRL.ShowWindow(SW_SHOW);
    m_FILECTRL.ShowWindow(SW_SHOW);
	m_FUNCTIONCTRL.ShowWindow(SW_SHOW);
	m_DeveloperCtrl.ShowWindow(SW_SHOW);
	m_DvmAliasCtrl.ShowWindow(SW_SHOW);
    
	
	m_BACKNAMECTRL.ShowWindow(SW_HIDE);
    m_BACKDESCCTRL.ShowWindow(SW_HIDE);
	m_BACKFILECTRL.ShowWindow(SW_HIDE);
	m_BACKFUNCTIONCTRL.ShowWindow(SW_HIDE);
	m_BackDeveloperCtrl.ShowWindow(SW_HIDE);
	m_BackDvmAliasCtrl.ShowWindow(SW_HIDE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CPathInsert::OnSelchangetabPATH(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	INT index = m_ctrlTABPATH.GetCurSel();

	if(0==index)
	{
		if(!ModifyFlag)
		m_Use_backCtrl.ShowWindow(SW_SHOW);
        
		m_NAMECTRL.ShowWindow(SW_SHOW);
        m_DESCCTRL.ShowWindow(SW_SHOW);
        m_FILECTRL.ShowWindow(SW_SHOW);
		m_FUNCTIONCTRL.ShowWindow(SW_SHOW);
		m_DeveloperCtrl.ShowWindow(SW_SHOW);
		m_DvmAliasCtrl.ShowWindow(SW_SHOW);

                
		m_BACKNAMECTRL.ShowWindow(SW_HIDE);
        m_BACKDESCCTRL.ShowWindow(SW_HIDE);
		m_BACKFILECTRL.ShowWindow(SW_HIDE);
		m_BACKFUNCTIONCTRL.ShowWindow(SW_HIDE);
		m_BackDeveloperCtrl.ShowWindow(SW_HIDE);
		m_BackDvmAliasCtrl.ShowWindow(SW_HIDE);

	
	}
    else if(1==index)
	{
		UpdateData(TRUE);
		if(!m_use_back) 
		{
			m_ctrlTABPATH.SetCurSel(0);
			return;
		}

		m_Use_backCtrl.ShowWindow(SW_HIDE);
        m_NAMECTRL.ShowWindow(SW_HIDE);
        m_DESCCTRL.ShowWindow(SW_HIDE);
        m_FILECTRL.ShowWindow(SW_HIDE);
		m_FUNCTIONCTRL.ShowWindow(SW_HIDE);
		m_DeveloperCtrl.ShowWindow(SW_HIDE);
		m_DvmAliasCtrl.ShowWindow(SW_HIDE);
		
				
		m_BACKNAMECTRL.ShowWindow(SW_SHOW);
        m_BACKDESCCTRL.ShowWindow(SW_SHOW);
        m_BACKFILECTRL.ShowWindow(SW_SHOW);
		m_BACKFUNCTIONCTRL.ShowWindow(SW_SHOW);
		m_BackDeveloperCtrl.ShowWindow(SW_SHOW);
		m_BackDvmAliasCtrl.ShowWindow(SW_SHOW);

	
	}
    
	
	*pResult = 0;
}

VOID CPathInsert::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	CDialog::OnOK();
}

VOID CPathInsert::OnCancel() 
{
	// TODO: Add extra cleanup here
	

	CDialog::OnCancel();
}
