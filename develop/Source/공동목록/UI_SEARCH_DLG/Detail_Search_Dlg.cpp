// Detail_Search_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Detail_Search_Dlg.h"
#include "Search_Main_Dlg.h"
#include "..\..\include\공동목록\Query.h"
#include "..\..\관리\KOMARC\KOMarcGetter\KOMarcGetterDlg.h"
#include "Lib_Code_Search_Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetail_Search_Dlg dialog

CString pSM[2] = { _T("자료찾기_찾기"),_T("분류지원_찾기") };
CString pstrCM[2] = { _T("CM_자료찾기_찾기"),_T("CM_분류지원_찾기")};


CESL_ControlMgr *pCM;
CString pCM_Mgr ;



CDetail_Search_Dlg::CDetail_Search_Dlg( BOOL DialogType , CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)  
{
	//{{AFX_DATA_INIT(CCmd_Search_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pParentWindow = (CSearch_Main_Dlg *)pParent;

	m_strDefTitle = _T("");
	m_strTitle = _T("");
	m_DialogType = FALSE;
	memset(&m_szDispAlias,0,sizeof(TCHAR) * 128);
}

CDetail_Search_Dlg::~CDetail_Search_Dlg()
{
	
}


VOID CDetail_Search_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetail_Search_Dlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetail_Search_Dlg, CDialog)
	//{{AFX_MSG_MAP(CDetail_Search_Dlg)
	ON_BN_CLICKED(IDC_bUNIVERCITY_CODE, OnbUNIVERCITYCODE)
	ON_BN_CLICKED(IDC_bGOVERNMENT, OnbGOVERNMENT)
	ON_BN_CLICKED(IDC_bPUBLICATE_COUNTRY, OnbPUBLICATECOUNTRY)
	ON_BN_CLICKED(IDC_bTEXT_LANGUAGE, OnbTEXTLANGUAGE)
	ON_BN_CLICKED(IDC_bSUMMARY_LANGUAGE, OnbSUMMARYLANGUAGE)
	ON_BN_CLICKED(IDC_bLIB, OnbLIB)
	ON_EN_SETFOCUS(IDC_eTITLE, OnSetfocuseTITLE)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_cDATA, OnSelchangecDATA)
	ON_BN_CLICKED(IDC_chkALL, OnchkALL)
	ON_BN_CLICKED(IDC_chkMAGAZINE, OnchkMAGAZINE)
	ON_BN_CLICKED(IDC_chkCOMMON, OnchkCOMMON)
	ON_BN_CLICKED(IDC_chkGRADE, OnchkGRADE)
	ON_BN_CLICKED(IDC_chkEXERCISE, OnchkEXERCISE)
	ON_BN_CLICKED(IDC_chkCOMIC, OnchkCOMIC)
	ON_BN_CLICKED(IDC_chkNEWSPAPER, OnchkNEWSPAPER)
	ON_BN_CLICKED(IDC_chkNONBOOK, OnchkNONBOOK)
	ON_BN_CLICKED(IDC_chkPOINT, OnchkPOINT)
	ON_BN_CLICKED(IDC_chkTEXTBOOK, OnchkTEXTBOOK)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetail_Search_Dlg message handlers

BOOL CDetail_Search_Dlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CDetail_Search_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	CString pFilePath ;
    
    m_DialogType = _WORK_SEARCH ;

    if ( m_DialogType == _WORK_SEARCH ) 
    {
        pFilePath = pSM[0]; 
        pCM_Mgr = pstrCM[0] ;
    } 
    else if ( m_DialogType == _CLASS_SEARCH ) 
    {
        pFilePath = pSM[1];
	    pCM_Mgr = pstrCM[1] ;
    }

	if (InitESL_Mgr(pFilePath) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	if(pFilePath == _T("자료찾기_찾기"))
	    pCM = FindCM(_T("CM_자료찾기_찾기")) ;
	else
	   pCM = FindCM(_T("CM_분류지원_찾기")) ;
   
	INT ids ;
    ids = pCM->AllControlDisplay (-1) ;
    if(ids){
		return -1 ;
	}

    CEdit *mtemp = ((CEdit*)GetDlgItem(IDC_eTITLE)) ;
    mtemp->SetFocus () ;
    

    ((CComboBox *)GetDlgItem(IDC_cSORT_TITLE))->SetCurSel(0);
    ((CComboBox *)GetDlgItem(IDC_cSORT_PUBLISHER))->SetCurSel(0);
	((CComboBox *)GetDlgItem(IDC_cSORT_PUBYEAR))->SetCurSel(2);

	((CButton*)GetDlgItem(IDC_chkALL))->SetCheck(TRUE);
	OnchkALL();	
	SetTimer( 1 , 100 , NULL );
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CDetail_Search_Dlg::MakeQuery( INT *lpCollectionType , INT *lpQueryRang , TCHAR *lpBoQuery , TCHAR *lpSeQuery , TCHAR *lpChQuery,  TCHAR *lpSortInfo  , TCHAR *lpCollection , CKolisNetSearchParam *pKolisNetSearchParam)
{
    INT     ids;
    CQuery  Query;
    TCHAR    szQuery[2048] ;
    TCHAR    Temp[32] ;
    _tcscpy( Temp , pCM_Mgr ) ;

    Query.SetDialogPtr ( this );
    Query.SetQueryType ( 0 );
    Query.SetCtrlAlias ( Temp );

    szQuery   [0] = '\0';
    lpSortInfo[0] = '\0';

    if ( 0 != Query.CheckValue( ) ) return -1;

    ids = Query.CheckAlways( );
    if ( 0 > ids ) {
        ::MessageBox ( GetSafeHwnd() , _T("주검색 항목을 1개 이상 입력하십시오") , _T("자료찾기"), MB_ICONWARNING );
        return -2;
    }
    if ( ids == 100 ) return -3;
	
    Query.GetCollectionType ( lpCollectionType );
    Query.GetQueryRang      ( lpQueryRang );
	Query.SetMatCode( GetMatCode() );
	ids = Query.MakeQuery         ( lpBoQuery , _DAN_ , pKolisNetSearchParam );
	if ( ids != 0 ) return ids;
    ids = Query.MakeQuery         ( lpSeQuery , _YON_ , pKolisNetSearchParam );
	if ( ids != 0 ) return ids;	
	ids = Query.MakeQuery         ( lpChQuery , _CHULPAN_ , pKolisNetSearchParam );
	if ( ids != 0 ) return ids;
	

    Query.GetCollection     ( lpCollection );
	
    Query.GetSortInfo( lpSortInfo  );

	return 0;
}

INT CDetail_Search_Dlg::SetSearchCondition(CString strTitle, CString strAuthor, CString strPublisher, CString strPubYear)
{
	( (CEdit*)GetDlgItem(IDC_eTITLE) ) -> SetWindowText( strTitle );
	( (CEdit*)GetDlgItem(IDC_eAUTHOR) ) -> SetWindowText( strAuthor );
	( (CEdit*)GetDlgItem(IDC_ePUBLISHER) ) -> SetWindowText( strPublisher );
	( (CEdit*)GetDlgItem(IDC_ePUBLICATE_YEAR_START) ) -> SetWindowText( strPubYear );

    return 0;
}

INT CDetail_Search_Dlg::Clear( )
{
	INT    ids;

    CString temp_szInformationType=_T("") ;
	CString temp_szInformation=_T("") ;
    
    ids = pCM->GetControlMgrData(_T("자료"),temp_szInformation,-1) ;
  
    if ( ids ) {
	
        return -1;
    }

   
    ids = pCM->AllControlClear ();
    if ( ids ) {
   
        return -1;
    }
	

	ids = pCM->SetControlMgrData (_T("자료"),_T("temp_szInformation"),-1) ;
  
    if ( ids ) {
	
        return -1;
    }
 
	GetDlgItem(IDC_eTITLE)->SetFocus();
   
	return 0;
}

CString CDetail_Search_Dlg::GetMatCode()
{
	CString strSelect = _T("");
	
	if( ((CButton * )GetDlgItem(IDC_chkALL))->GetCheck() )
	{
		strSelect = _T("전체"); return strSelect;
	}
	if( ((CButton * )GetDlgItem(IDC_chkCOMMON))->GetCheck() )
	{
		if( strSelect.GetLength() == 0 )
			strSelect = _T("일반도서");
	}
	if( ((CButton * )GetDlgItem(IDC_chkGRADE))->GetCheck() )
	{
		if( strSelect.GetLength() == 0 )
			strSelect = _T("학위논문");
		else
			strSelect = strSelect + _T(",") + _T("학위논문");
	}
	if( ((CButton * )GetDlgItem(IDC_chkEXERCISE))->GetCheck() )
	{
		if( strSelect.GetLength() == 0 )
			strSelect = _T("학습서");
		else
			strSelect = strSelect + _T(",") + _T("학습서");
	}
	if( ((CButton * )GetDlgItem(IDC_chkCOMIC))->GetCheck() )
	{
		if( strSelect.GetLength() == 0 )
			strSelect = _T("만화");
		else
			strSelect = strSelect + _T(",") + _T("만화");
	}
	if( ((CButton * )GetDlgItem(IDC_chkMAGAZINE))->GetCheck() )
	{
		if( strSelect.GetLength() == 0 )
			strSelect = _T("잡지");
		else
			strSelect = strSelect + _T(",") + _T("잡지");
	}
	if( ((CButton * )GetDlgItem(IDC_chkNEWSPAPER))->GetCheck() )
	{
		if( strSelect.GetLength() == 0 )
			strSelect = _T("신문");
		else
			strSelect = strSelect + _T(",") + _T("신문");
	}
	if( ((CButton * )GetDlgItem(IDC_chkNONBOOK))->GetCheck() )
	{
		if( strSelect.GetLength() == 0 )
			strSelect = _T("비도서");
		else
			strSelect = strSelect + _T(",") + _T("비도서");
	}
	if( ((CButton * )GetDlgItem(IDC_chkPOINT))->GetCheck() )
	{
		if( strSelect.GetLength() == 0 )
			strSelect = _T("점자자료");
		else
			strSelect = strSelect + _T(",") + _T("점자자료");
	}
	if( ((CButton * )GetDlgItem(IDC_chkTEXTBOOK))->GetCheck() )
	{
		if( strSelect.GetLength() == 0 )
			strSelect = _T("교과서");
		else
			strSelect = strSelect + _T(",") + _T("교과서");
	}
	
	if( strSelect == _T("") )
		return _T("전체");

	if( strSelect.Find(_T(",")) >= 0 ) strSelect += _T(",");
	return strSelect;
}

VOID CDetail_Search_Dlg::SetDispAlias( TCHAR * lpDispAlias )
{
    _tcscpy ( m_szDispAlias , lpDispAlias );
}


VOID CDetail_Search_Dlg::ControlToggle( INT CollectionType )
{
	switch( CollectionType ) {
	case _DAN_ :
		GetDlgItem(IDC_eDEG_CODE)->EnableWindow( TRUE );
		GetDlgItem(IDC_eDEG_PUBLISH)->EnableWindow( TRUE );
		GetDlgItem(IDC_ePUBLICATE_YEAR_START)->EnableWindow( TRUE );
		GetDlgItem(IDC_ePUBLICATE_YEAR_START)->EnableWindow( TRUE );
		GetDlgItem(IDC_ePUBLICATE_YEAR_END	)->EnableWindow( TRUE );
		GetDlgItem(IDC_cTHESIS	)->EnableWindow( TRUE );
		GetDlgItem(IDC_cPRINT_COUNT	)->EnableWindow( FALSE );
		break;
	case _YON_ :
		GetDlgItem(IDC_eDEG_CODE)->EnableWindow( FALSE );
		GetDlgItem(IDC_eDEG_PUBLISH)->EnableWindow( FALSE );
		GetDlgItem(IDC_ePUBLICATE_YEAR_START)->EnableWindow( FALSE );
		GetDlgItem(IDC_ePUBLICATE_YEAR_START)->EnableWindow( FALSE );
		GetDlgItem(IDC_ePUBLICATE_YEAR_END	)->EnableWindow( FALSE );
		GetDlgItem(IDC_cTHESIS	)->EnableWindow( FALSE );
		GetDlgItem(IDC_cPRINT_COUNT	)->EnableWindow( TRUE );
		break;
	case _ALL_ :
		GetDlgItem(IDC_eDEG_CODE)->EnableWindow( TRUE );
		GetDlgItem(IDC_eDEG_PUBLISH)->EnableWindow( TRUE );
		GetDlgItem(IDC_ePUBLICATE_YEAR_START)->EnableWindow( TRUE );
		GetDlgItem(IDC_ePUBLICATE_YEAR_START)->EnableWindow( TRUE );
		GetDlgItem(IDC_ePUBLICATE_YEAR_END	)->EnableWindow( TRUE );
		GetDlgItem(IDC_cTHESIS	)->EnableWindow( TRUE );
		GetDlgItem(IDC_cPRINT_COUNT	)->EnableWindow( TRUE );
		break;
	}
}

VOID CDetail_Search_Dlg::OnbUNIVERCITYCODE() 
{
	// TODO: Add your control notification handler code here
	CKOMarcGetterDlg dlg( this );

	dlg.InitSetProperty( 1, _T("") );
	if(IDOK == dlg.DoModal())
	{
	  CString UNIVERCITYCODE = dlg.m_code;	
      GetDlgItem(IDC_eUNIVERCITY_CODE)->SetWindowText(UNIVERCITYCODE) ;
	}
}

VOID CDetail_Search_Dlg::OnbGOVERNMENT() 
{
	// TODO: Add your control notification handler code here
	CKOMarcGetterDlg dlg( this );

	dlg.InitSetProperty( 3, _T("") );
	if(IDOK == dlg.DoModal())
	{
		CString GOVERNMENTCODE = dlg.m_code;
		GetDlgItem(IDC_eGOVERNMENT)->SetWindowText(GOVERNMENTCODE) ;	
	}
}

VOID CDetail_Search_Dlg::OnbPUBLICATECOUNTRY() 
{
	// TODO: Add your control notification handler code here
	CKOMarcGetterDlg dlg( this );

	dlg.InitSetProperty( 0, _T("") );
	if(IDOK == dlg.DoModal())
	{
		CString PUBLICATECOUNTRYCODE = dlg.m_code;	
        GetDlgItem(IDC_ePUBLICATE_COUNTRY)->SetWindowText(PUBLICATECOUNTRYCODE) ;
	}
}

VOID CDetail_Search_Dlg::OnbTEXTLANGUAGE() 
{
	// TODO: Add your control notification handler code here
	CKOMarcGetterDlg dlg( this );

	dlg.InitSetProperty( 2, _T("") );
	if(IDOK == dlg.DoModal())
	{
		CString TEXTLANGUAGECODE = dlg.m_code;
		GetDlgItem(IDC_eTEXT_LANGUAGE)->SetWindowText(TEXTLANGUAGECODE) ;
	}
}

VOID CDetail_Search_Dlg::OnbSUMMARYLANGUAGE() 
{
	// TODO: Add your control notification handler code here
	CKOMarcGetterDlg dlg( this );

	dlg.InitSetProperty( 2, _T("") );
	if(IDOK == dlg.DoModal())
	{
		CString TEXTLANGUAGECODE = dlg.m_code;
		GetDlgItem(IDC_eSUMMARY_LANGUAGE)->SetWindowText(TEXTLANGUAGECODE) ;
	}
}

VOID CDetail_Search_Dlg::OnbLIB() 
{
	// TODO: Add your control notification handler code here

	CLib_Code_Search_Dlg dlg(this);
	if(dlg.DoModal () == IDCANCEL) 
	{
		CString LIBCODE = dlg.m_code ;
		GetDlgItem(IDC_eLIB_CODE)->SetWindowText(LIBCODE) ;
	}

}

VOID CDetail_Search_Dlg::OnSetfocuseTITLE() 
{
	EFS_BEGIN	
	
	CEdit *pEdit1 = (CEdit *) GetDlgItem (IDC_eTITLE);
	HIMC hIME;
	DWORD dwConversion, dwSentence;
	
	hIME = ::ImmGetContext(pEdit1->m_hWnd);
	if(hIME) 
	{
		ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;
		
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
		
		ImmReleaseContext(pEdit1->m_hWnd,hIME); 
	} 	
	
	EFS_END		
}

VOID CDetail_Search_Dlg::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == 1 )
	{
		KillTimer(1);
		((CEdit*) GetDlgItem (IDC_eTITLE))->SetFocus();	 
	}
	
	CDialog::OnTimer(nIDEvent);
}

VOID CDetail_Search_Dlg::OnSelchangecDATA() 
{
	// TODO: Add your control notification handler code here
	INT     CollectionType = 2;
    TCHAR    szData[128];
	
	CString temp_szData ;
    
    pCM->GetControlMgrData (_T("자료"),temp_szData,-1) ;

    _tcscpy(szData,temp_szData) ;


	if ( _tcsicmp ( szData , _T("전체") ) == 0 ) {
		CollectionType = 2;
	} 
	else

	if ( _tcsicmp ( szData , _T("KOLIS-NET") ) == 0 )  	
	{
		CollectionType = 0;
	} 
	else
	if ( _tcsicmp ( szData , _T("출판유통진흥원") ) == 0 ) {
		CollectionType = 3;
	} 
	else
	if ( _tcsicmp ( szData , _T("연속") ) == 0 ) {
		CollectionType = 1;
	}

	ControlToggle( CollectionType );
}


VOID CDetail_Search_Dlg::OnchkALL() 
{
	INT nCheck = ((CButton * )GetDlgItem(IDC_chkALL))->GetCheck();
	if( nCheck == 0 )
	{
		((CButton * )GetDlgItem(IDC_chkCOMMON))->SetCheck(FALSE);
		((CButton * )GetDlgItem(IDC_chkGRADE))->SetCheck(FALSE);
		((CButton * )GetDlgItem(IDC_chkEXERCISE))->SetCheck(FALSE);
		((CButton * )GetDlgItem(IDC_chkCOMIC))->SetCheck(FALSE);
		((CButton * )GetDlgItem(IDC_chkMAGAZINE))->SetCheck(FALSE);
		((CButton * )GetDlgItem(IDC_chkNEWSPAPER))->SetCheck(FALSE);
		((CButton * )GetDlgItem(IDC_chkNONBOOK))->SetCheck(FALSE);
		((CButton * )GetDlgItem(IDC_chkPOINT))->SetCheck(FALSE);
		((CButton * )GetDlgItem(IDC_chkTEXTBOOK))->SetCheck(FALSE);	
	}	
	else
	{
		((CButton * )GetDlgItem(IDC_chkCOMMON))->SetCheck(TRUE);
		((CButton * )GetDlgItem(IDC_chkGRADE))->SetCheck(TRUE);
		((CButton * )GetDlgItem(IDC_chkEXERCISE))->SetCheck(TRUE);
		((CButton * )GetDlgItem(IDC_chkCOMIC))->SetCheck(TRUE);
		((CButton * )GetDlgItem(IDC_chkMAGAZINE))->SetCheck(TRUE);
		((CButton * )GetDlgItem(IDC_chkNEWSPAPER))->SetCheck(TRUE);
		((CButton * )GetDlgItem(IDC_chkNONBOOK))->SetCheck(TRUE);
		((CButton * )GetDlgItem(IDC_chkPOINT))->SetCheck(TRUE);
		((CButton * )GetDlgItem(IDC_chkTEXTBOOK))->SetCheck(TRUE);	
	}	
}

VOID CDetail_Search_Dlg::OnchkMAGAZINE() 
{
	if( !((CButton*)GetDlgItem(IDC_chkMAGAZINE))->GetCheck() )
		((CButton*)GetDlgItem(IDC_chkALL))->SetCheck(FALSE);	
}

VOID CDetail_Search_Dlg::OnchkCOMMON() 
{
	if( !((CButton*)GetDlgItem(IDC_chkCOMMON))->GetCheck() )
		((CButton*)GetDlgItem(IDC_chkALL))->SetCheck(FALSE);
}

VOID CDetail_Search_Dlg::OnchkGRADE() 
{
	if( !((CButton*)GetDlgItem(IDC_chkGRADE))->GetCheck() )
		((CButton*)GetDlgItem(IDC_chkALL))->SetCheck(FALSE);	
}

VOID CDetail_Search_Dlg::OnchkEXERCISE() 
{
	if( !((CButton*)GetDlgItem(IDC_chkEXERCISE))->GetCheck() )
		((CButton*)GetDlgItem(IDC_chkALL))->SetCheck(FALSE);
}

VOID CDetail_Search_Dlg::OnchkCOMIC() 
{
	if( !((CButton*)GetDlgItem(IDC_chkCOMIC))->GetCheck() )
		((CButton*)GetDlgItem(IDC_chkALL))->SetCheck(FALSE);
}

VOID CDetail_Search_Dlg::OnchkNEWSPAPER() 
{
	if( !((CButton*)GetDlgItem(IDC_chkNEWSPAPER))->GetCheck() )
		((CButton*)GetDlgItem(IDC_chkALL))->SetCheck(FALSE);
}

VOID CDetail_Search_Dlg::OnchkNONBOOK() 
{
	if( !((CButton*)GetDlgItem(IDC_chkNONBOOK))->GetCheck() )
		((CButton*)GetDlgItem(IDC_chkALL))->SetCheck(FALSE);
}

VOID CDetail_Search_Dlg::OnchkPOINT() 
{
	if( !((CButton*)GetDlgItem(IDC_chkPOINT))->GetCheck() )
		((CButton*)GetDlgItem(IDC_chkALL))->SetCheck(FALSE);
}

VOID CDetail_Search_Dlg::OnchkTEXTBOOK() 
{
	if( !((CButton*)GetDlgItem(IDC_chkTEXTBOOK))->GetCheck() )
		((CButton*)GetDlgItem(IDC_chkALL))->SetCheck(FALSE);
}

BOOL CDetail_Search_Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		pParentWindow->Search() ;
		return TRUE;
	}
	else if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
	{
		return FALSE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CDetail_Search_Dlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting	
}

VOID CDetail_Search_Dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{	
	CDialog::OnShowWindow(bShow, nStatus);	
}

HBRUSH CDetail_Search_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
