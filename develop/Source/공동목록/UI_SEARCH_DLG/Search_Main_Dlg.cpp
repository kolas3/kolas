// Search_Main_Dlg.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "Search_Main_Dlg.h"
#include "Detail_Search_Dlg.h"
#include "Cmd_Search_Dlg.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearch_Main_Dlg dialog

const UINT	idBottomBack[1] = { IDC_sBAR };
const INT	BottomCtlCnt = 5;
const UINT	idBottom[BottomCtlCnt] = 
{
	IDC_bSEARCH , IDC_bCMD_SEARCH , IDC_bCLEAR,	IDOK , IDCANCEL
};


CSearch_Main_Dlg::CSearch_Main_Dlg(CKolisNetSearchParam* pKolisNetSearchParam, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmd_Search_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

    m_DialogType       = _WORK_SEARCH;
    m_bIsDetail        = FALSE;
    m_bIsCmd           = FALSE;

    m_pDetailDlg = NULL;
	m_pCmdDlg    = NULL;

    m_CollectionType = -1;
    m_QueryRang      = -1;
    m_szBoQuery[0]   = '\0';
    m_szSeQuery[0]   = '\0';
	m_szChQuery[0]   = '\0';
	m_szSortInfo[0]  = '\0';

	m_sTitle = _T("");
	m_sAuthor = _T("");
	m_sPublisher = _T("");
	m_sPubYear = _T("");
	
	m_pKolisNetSearchParam = pKolisNetSearchParam;
}

CSearch_Main_Dlg::~CSearch_Main_Dlg()
{
	if ( NULL != m_pDetailDlg ) {
	delete m_pDetailDlg;
	m_pDetailDlg = NULL;
	}
	if ( NULL != m_pCmdDlg ) {
	delete m_pCmdDlg;
	m_pCmdDlg = NULL;
	}
}


BOOL CSearch_Main_Dlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CSearch_Main_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearch_Main_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearch_Main_Dlg, CDialog)
	//{{AFX_MSG_MAP(CSearch_Main_Dlg)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bCMD_SEARCH, OnbCMDSEARCH)
	ON_BN_CLICKED(IDC_bCLEAR, OnbCLEAR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearch_Main_Dlg message handlers

BOOL CSearch_Main_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	
    INT    ids;
    TCHAR   szDialogType[32];
	CString data ;
    
    CRect rChild, rTmp;
	GetDlgItem(IDC_REC01)->GetWindowRect(&rTmp);

	ScreenToClient(&rTmp);
	rChild.left = rTmp.left;
	rChild.top = rTmp.top;

	m_pDetailDlg = new CDetail_Search_Dlg( m_DialogType , this );
    m_pDetailDlg->Create(NULL, NULL, WS_CHILD, rChild, this, IDD_UC_WORK_SEARCH, NULL);
	m_pDetailDlg->SetSearchCondition( m_sTitle, m_sAuthor, m_sPublisher, m_sPubYear);
	m_pDetailDlg->UpdateWindow();
   
	m_pCmdDlg = new CCmd_Search_Dlg( this);
   	m_pCmdDlg->Create(NULL, NULL, WS_CHILD, rChild, this, IDD_UC_CMD_SEARCH, NULL);
	m_pCmdDlg->ShowWindow(SW_HIDE);
	m_pCmdDlg->UpdateWindow();

    CESL_DataMgr *pDM = FindDM(_T("DM_찾기"));
	
    ids = GetDataMgrData(_T("DM_찾기"),_T("DialogType"), data, -1);
    
    if ( ids ) {
    ids = SetDataMgrData(_T("DM_찾기"),_T("DialogType"), _T("1"), -1);
        
        _tcscpy ( szDialogType , _T("0") );
    }
  
    switch ( szDialogType[0] ) 
    {
    case '0' :
        Toggle ( _SIMPLE );
        break;
    case '1' :
        Toggle ( _DETAIL );
        break;
    case '2' :
        Toggle ( _COMMAND );
        break;
    }
    
    (CButton *)GetDlgItem(IDC_bCMD_SEARCH)->ShowWindow(FALSE) ;	 

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



VOID CSearch_Main_Dlg::Toggle( INT Status )
{

    CRect	rChild, rBottom, rStatic;
	INT		nHeight , i;
    CString Title,tmpTitle;

    switch ( Status ) 
    {
    case _DETAIL  :
	    GetDlgItem(IDC_REC01)->GetWindowRect(&rStatic);
	    ScreenToClient(&rStatic);

	    m_pDetailDlg ->MoveWindow(rStatic, TRUE);

        GetDlgItem(IDC_bCMD_SEARCH)->SetWindowText(_T("명령어검색"));
	    GetDlgItem(IDC_bCMD_SEARCH)->ShowWindow(SW_HIDE); 

        GetDlgItem(IDC_bSEARCH)->SetWindowText(_T("간략검색"));  
        GetDlgItem(IDC_bSEARCH)->ShowWindow(SW_SHOW);

        m_pDetailDlg->GetWindowRect(&rChild);
	    ScreenToClient(&rChild);

        m_pCmdDlg ->ShowWindow(SW_HIDE);
        m_pDetailDlg ->ShowWindow(SW_SHOW);
        
        Title = m_Title;
        Title += _T(" - 상세검색");
        SetWindowText(Title);

        m_bIsDetail = TRUE;
        m_bIsCmd    = FALSE;
        SetDataMgrData(_T("DM_찾기"),_T("DialogType"),_T("1"),-1);
 		
		break;
    case _SIMPLE  : 
	    GetDlgItem(IDC_REC02)->GetWindowRect(&rStatic);
	    ScreenToClient(&rStatic);

        m_pDetailDlg->MoveWindow (rStatic,TRUE);
        GetDlgItem(IDC_bCMD_SEARCH)->SetWindowText(_T("명령어검색")); 
	    GetDlgItem(IDC_bCMD_SEARCH)->ShowWindow(SW_HIDE); 

        GetDlgItem(IDC_bSEARCH)->SetWindowText(_T("상세검색")); 
        GetDlgItem(IDC_bSEARCH)->ShowWindow(SW_SHOW);

        m_pDetailDlg->GetWindowRect(&rChild);
	    ScreenToClient(&rChild);

        m_pCmdDlg ->ShowWindow(SW_HIDE);
        m_pDetailDlg ->ShowWindow(SW_SHOW);

        Title = m_Title;
        Title += _T(" - 간략검색");
        SetWindowText(Title);

        m_bIsDetail = FALSE;
        m_bIsCmd    = FALSE;
      
        SetDataMgrData(_T("DM_찾기"),_T("DialogType"),_T("0"),-1);

        break;
    case _CLASS_SEARCH : 
	    GetDlgItem(IDC_REC04)->GetWindowRect(&rStatic);
	    ScreenToClient(&rStatic);

	    m_pDetailDlg ->MoveWindow(rStatic, TRUE);

        GetDlgItem(IDC_bCMD_SEARCH)->SetWindowText(_T("명령어검색"));
	    GetDlgItem(IDC_bCMD_SEARCH)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_bSEARCH)->ShowWindow(SW_HIDE);

        m_pDetailDlg->GetWindowRect(&rChild);
	    ScreenToClient(&rChild);

        m_pCmdDlg ->ShowWindow(SW_HIDE);
        m_pDetailDlg ->ShowWindow(SW_SHOW);

        SetWindowText(m_Title);

        m_bIsDetail = FALSE;
        m_bIsCmd    = FALSE;
        break;
    case _COMMAND : 
	    GetDlgItem(IDC_REC03)->GetWindowRect(&rStatic);
	    ScreenToClient(&rStatic);

	    m_pCmdDlg->MoveWindow(rStatic, TRUE);

        GetDlgItem(IDC_bCMD_SEARCH)->SetWindowText(_T("빈칸채우기검색"));
	    GetDlgItem(IDC_bCMD_SEARCH)->ShowWindow(SW_SHOW); 
        GetDlgItem(IDC_bSEARCH)->ShowWindow(SW_HIDE);

	    m_pCmdDlg->GetWindowRect(&rChild);			              
	    ScreenToClient(&rChild);

        m_pDetailDlg ->ShowWindow(SW_HIDE);
        m_pCmdDlg ->ShowWindow(SW_SHOW);

        Title = m_Title;
        Title += _T(" - 명령어검색");
        SetWindowText(Title);

        m_bIsCmd = TRUE;

        SetDataMgrData(_T("DM_찾기"),_T("DialogType"),_T("2"),-1);

        break;
    }

	GetDlgItem(idBottomBack[0])->GetWindowRect(&rStatic);
	ScreenToClient(&rStatic);
	nHeight = rStatic.bottom - rStatic.top;
	rStatic.top		= rChild.bottom +2 ;
	rStatic.bottom	= rStatic.top + nHeight;
	GetDlgItem(idBottomBack[0])->MoveWindow(rStatic, TRUE);

	for( i = 0; i < BottomCtlCnt; i++ ) {
		GetDlgItem(idBottom[i])->GetWindowRect(&rBottom);
		ScreenToClient(&rBottom);
		nHeight = rBottom.bottom - rBottom.top;
		rBottom.top		= rStatic.top + 6;
		rBottom.bottom	= rBottom.top + nHeight;
		GetDlgItem(idBottom[i])->MoveWindow(rBottom, TRUE);
	}

	CRect rMain;
	GetWindowRect(&rMain);
	ScreenToClient(&rMain);

	rMain.bottom = rStatic.bottom + 5;
	ClientToScreen(&rMain);
	MoveWindow(&rMain, TRUE);
    CenterWindow();

}

VOID CSearch_Main_Dlg::OnbSEARCH() 
{
	// TODO: Add your control notification handler code here
	 if ( m_bIsDetail == TRUE ) {
        Toggle ( _SIMPLE );
    } else {
        Toggle ( _DETAIL );
    }
}

VOID CSearch_Main_Dlg::OnbCMDSEARCH() 
{
	// TODO: Add your control notification handler code here
	 if ( _CLASS_SEARCH == m_DialogType ) {
        if ( m_bIsCmd == FALSE ) {
            Toggle ( _COMMAND );
        } else {
            Toggle ( _CLASS_SEARCH );
        }
    } else {
        if ( m_bIsCmd == FALSE ) {
            Toggle ( _COMMAND );
        } else {
            if ( m_bIsDetail == TRUE ) {
                Toggle ( _DETAIL );
            } else {
                Toggle ( _SIMPLE );
            }
        }
    }
}

VOID CSearch_Main_Dlg::OnOK() 
{	
	Search();
}

VOID CSearch_Main_Dlg::Search()
{
	INT      ids = 0;
    CString  sMsg;
    TCHAR     szBoQuery[4096];
    TCHAR     szSeQuery[4096];
	TCHAR	 szChQuery[4096];
	
	if ( m_bIsCmd == TRUE )
	{     
    } 
	else
	{
		ids = m_pDetailDlg->MakeQuery( &m_CollectionType , &m_QueryRang , szBoQuery , szSeQuery , szChQuery, m_szSortInfo  , m_szCollection , m_pKolisNetSearchParam );       
    }
    if ( ids != 0 )
	{
        return;
    }	
	
	CDialog::OnOK();
}

VOID   CSearch_Main_Dlg::SetDispAlias  ( TCHAR * lpDispAlias )
{
  _tcscpy ( m_szDispAlias , lpDispAlias );
}

INT CSearch_Main_Dlg::SetDefaultTitle(CString strTitle)
{
    m_strTitle = strTitle;
    return 0;
}

VOID CSearch_Main_Dlg::GetCollectionType( INT *lpCollectionType )
{
    *lpCollectionType = m_CollectionType ;
}

VOID CSearch_Main_Dlg::GetCollection( TCHAR *lpCollection )
{
    _tcscpy ( lpCollection , m_szCollection );
}

VOID CSearch_Main_Dlg::GetQueryRang( INT *lpQueryRang )
{
    *lpQueryRang = m_QueryRang;
}

VOID CSearch_Main_Dlg::GetQuery( TCHAR *lpQuery  , INT CollectionType )
{
	if ( CollectionType == _DAN_ ) {
		_tcscpy ( lpQuery , m_szBoQuery ); 
	} else
	if ( CollectionType == _YON_ ) {
		_tcscpy ( lpQuery , m_szSeQuery ); 
	}	
	if ( CollectionType == _CHULPAN_ ) {
		_tcscpy ( lpQuery , m_szChQuery ); 
	}	
}

VOID CSearch_Main_Dlg::GetSortInfo( TCHAR *lpSortInfo )
{
    _tcscpy ( lpSortInfo , m_szSortInfo ); 
}


VOID CSearch_Main_Dlg::OnbCLEAR() 
{
	// TODO: Add your control notification handler code here

    if ( m_bIsCmd == TRUE ) {
	 //   m_pCmdDlg->Clear( );
    } else {
        m_pDetailDlg->Clear( );
    }
}

BOOL CSearch_Main_Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
	    OnOK() ;
		return TRUE;
	}
	else if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
	{
		OnCancel();
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CSearch_Main_Dlg::SetSearchCondition( CString strTitle/*=NULL*/, CString strAuthor/*=NULL*/, CString strPublisher/*=NULL*/, CString strPubYear/*=NULL*/ )
{
	m_sTitle	= strTitle;
	m_sAuthor	= strAuthor;
	m_sPublisher= strPublisher;
	m_sPubYear  = strPubYear;
}
HBRUSH CSearch_Main_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
