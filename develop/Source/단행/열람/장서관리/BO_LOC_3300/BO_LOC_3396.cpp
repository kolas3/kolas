// BO_LOC_3396.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3396.h"
#include "imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3396 dialog


CBO_LOC_3396::CBO_LOC_3396(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3396)
	m_sHomePage = _T("");
	m_sLibName = _T("");
	m_sPhoneNumber = _T("");
	m_nStartPos = 0;
	//}}AFX_DATA_INIT
}

CBO_LOC_3396::~CBO_LOC_3396()
{
}

VOID CBO_LOC_3396::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3396)
	DDX_Text(pDX, IDC_edtLIB_HOMEPAGE, m_sHomePage);
	DDX_Text(pDX, IDC_edt3300_LIB_NAME, m_sLibName);
	DDX_Text(pDX, IDC_edtPHONE_NUMBER, m_sPhoneNumber);
	DDX_Text(pDX, IDC_edtStartPos, m_nStartPos);
	DDV_MinMaxInt(pDX, m_nStartPos, 1, 4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3396, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3396)
	ON_EN_SETFOCUS(IDC_edtLIB_HOMEPAGE, OnSetfocusedtLIBHOMEPAGE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3396 message handlers

BOOL CBO_LOC_3396::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	SetLibInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



BOOL CBO_LOC_3396::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN
		
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
	
}

INT CBO_LOC_3396::GetLibInfo( CString &sHomePage , CString &sLibName , CString &sPhoneNumber )
{
	EFS_BEGIN

	if( !UpdateData(TRUE) )
		return 1;

	sHomePage = m_sHomePage;
	sLibName = m_sLibName;
	sPhoneNumber = m_sPhoneNumber;

	return 0;
	EFS_END

	return -1;
}

INT CBO_LOC_3396::SetLibInfo()
{
	EFS_BEGIN
	
	CString sLibInfo[3] = {
		_T("LIB_NAME") , _T("LIB_URL") , _T("LIB_TEL") 
	};
	CLocCommonAPI::GetLibInfo( sLibInfo , sLibInfo , 3 , this );

	m_sHomePage = sLibInfo[1];
	m_sLibName = sLibInfo[0];
	m_sPhoneNumber = sLibInfo[2];
	m_nStartPos = 1;
	
	UpdateData(FALSE);

	return 0;
	EFS_END
		
	return -1;
}

INT CBO_LOC_3396::GetStartPos(INT &nStartPos)
{
	EFS_BEGIN

	if( !UpdateData(TRUE) )
		return 1;
		
	nStartPos = m_nStartPos;

	return 0;

	EFS_END

	return -1;

}

VOID CBO_LOC_3396::OnSetfocusedtLIBHOMEPAGE() 
{
	// TODO: Add your control notification handler code here
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtLIB_HOMEPAGE)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_ALPHANUMERIC;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtLIB_HOMEPAGE)->m_hWnd,hIME); 
    }
}

HBRUSH CBO_LOC_3396::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
