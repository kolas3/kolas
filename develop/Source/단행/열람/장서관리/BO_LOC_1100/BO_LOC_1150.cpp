// BO_LOC_1150.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_1150.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1150 dialog


CBO_LOC_1150::CBO_LOC_1150(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_1150)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bGabPrintOpt = false;
	
	m_paperType = 0;
	m_sPassDate = _T("");
	m_strCloseChK_YN = _T("");
}

CBO_LOC_1150::~CBO_LOC_1150()
{

}

VOID CBO_LOC_1150::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_1150)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_1150, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_1150)
	ON_BN_CLICKED(IDC_btnPRINT, OnbtnPRINT)
	ON_BN_CLICKED(IDC_btnClose, OnbtnClose)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_rCPAPER, OnrCPAPER)
	ON_BN_CLICKED(IDC_rA4, OnrA4)
	ON_BN_CLICKED(IDC_cGABPrintOpt, OncGABPrintOpt)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_1150 message handlers

BOOL CBO_LOC_1150::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	if(InitESL_Mgr(_T("BO_LOC_1150")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_1150::OnbtnPRINT() 
{
	GetControlData( _T("CM_BO_LOC_1150") , _T("인계일자") , m_sPassDate );
	OnCancel();
}

VOID CBO_LOC_1150::OnbtnClose() 
{
	m_strCloseChK_YN = _T("Y");

	OnCancel();	
}

VOID CBO_LOC_1150::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	if( bShow )
		SetPaperType();
}

VOID CBO_LOC_1150::SetPaperType()
{
	m_paperType = GetInitPaperType();

	const INT ctrlCnt = 2;
	UINT id[ ctrlCnt ] = {
		IDC_rCPAPER, IDC_rA4 };
	bool flag[ ctrlCnt ] = {
		false, false };
	flag[ m_paperType ] = true;

	CButton *pBtn = NULL;
	for( INT i = 0; i < ctrlCnt; i++ )
	{
		pBtn = ( CButton * )GetDlgItem( id[ i ] );
		pBtn->SetCheck( flag[ i ] );
	}	
}

INT CBO_LOC_1150::GetInitPaperType()
{
	CStdioFile fp;
	if( !fp.Open( PAPER_TYPE_INFO_FILEPATH, CFile::modeRead | CFile::typeBinary ) )
		return 1;
	
		TCHAR cUni;
		fp.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fp.SeekToBegin();
		}
	
	CString type;
	fp.ReadString( type );
	fp.Close();	

	return _ttoi( type );
}

VOID CBO_LOC_1150::OnrCPAPER() 
{
	CButton *pBtn = ( CButton * )GetDlgItem( IDC_cGABPrintOpt );
	if( pBtn->GetCheck() )
	{
		m_bGabPrintOpt = true;
		CButton *pBtn1 = ( CButton * )GetDlgItem( IDC_rCPAPER );
		pBtn1->SetCheck(0);
		CButton *pBtn2 = ( CButton * )GetDlgItem( IDC_rA4 );
		pBtn2->SetCheck(1);		
	}

	RememberPaperType();
}

VOID CBO_LOC_1150::OnrA4() 
{
	// TODO: Add your control notification handler code here
	
	RememberPaperType();
}

VOID CBO_LOC_1150::RememberPaperType()
{
	m_paperType = GetPaperType();
	
	FILE *fp = _tfopen( PAPER_TYPE_INFO_FILEPATH, _T("w+b") );
	
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
		
	_ftprintf( fp, _T("%d"), m_paperType );
	fclose( fp );	
}

INT CBO_LOC_1150::GetPaperType()
{
	const INT ctrlCnt = 2;
	UINT id[ ctrlCnt ] = {
		IDC_rCPAPER, IDC_rA4 };	

	for( INT i = 0; i < ctrlCnt; i++ )
	{
		CButton *pBtn = ( CButton * )GetDlgItem( id[ i ] );
		if( pBtn->GetCheck() )
			return i;
	}

	return 1;
}

VOID CBO_LOC_1150::OncGABPrintOpt() 
{
	// TODO: Add your control notification handler code here
	
	CButton *pBtn = ( CButton * )GetDlgItem( IDC_cGABPrintOpt );
	if( pBtn->GetCheck() )
	{
		m_bGabPrintOpt = true;

		CButton *pBtn1 = ( CButton * )GetDlgItem( IDC_rCPAPER );
		pBtn1->SetCheck(0);
		CButton *pBtn2 = ( CButton * )GetDlgItem( IDC_rA4 );
		pBtn2->SetCheck(1);
		RememberPaperType();
	}
	else
	{
		m_bGabPrintOpt = false;
	}
}

HBRUSH CBO_LOC_1150::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
