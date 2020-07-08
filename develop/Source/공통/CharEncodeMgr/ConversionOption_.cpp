// ConversionOption.cpp : implementation file
//

#include "stdafx.h"
#include "ConversionOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConversionOption dialog


CConversionOption::CConversionOption(CWnd* pParent /*=NULL*/)
	: CharConvert(CConversionOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConversionOption)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConversionOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConversionOption)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConversionOption, CDialog)
	//{{AFX_MSG_MAP(CConversionOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConversionOption message handlers

BOOL CConversionOption::OnInitDialog() 
{
ECO_EXCEPTION_BEGIN
	CDialog::OnInitDialog();
	CString		sBuffer;


   if (m_MBFlags & MB_PRECOMPOSED)
	   ((CButton*)GetDlgItem(IDC_Precomposed))->SetCheck(1);
   
   if (m_MBFlags & MB_COMPOSITE)
	   ((CButton*)GetDlgItem(IDC_Composite))->SetCheck(1);
   
   if (m_MBFlags & MB_USEGLYPHCHARS)
	   ((CButton*)GetDlgItem(IDC_Useglyph))->SetCheck(1);
   
      
   if (m_CFlags & WC_COMPOSITECHECK)
	   ((CButton*)GetDlgItem(IDC_CompositeCheck))->SetCheck(1);
   
   if (m_CFlags & WC_DISCARDNS)
	   ((CButton*)GetDlgItem(IDC_Discardns))->SetCheck(1);
   
   if (m_CFlags & WC_SEPCHARS)
	   ((CButton*)GetDlgItem(IDC_Sepchars))->SetCheck(1);
   
   if (m_CFlags & WC_DEFAULTCHAR)
	   ((CButton*)GetDlgItem(IDC_Defaultchar))->SetCheck(1);
   

   // Default Char
   sBuffer = m_DefaultChar;
   GetDlgItem(IDC_DefaultCharacter)->SetWindowText(sBuffer);
	   
	if (m_UsedDefaultChar)
		((CButton*)GetDlgItem(IDC_UsedDefaultChar))->SetCheck(1);


ECO_EXCEPTION_END
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CConversionOption::OnOK() 
{
ECO_EXCEPTION_BEGIN
	CString		sBuffer;


	if (((CButton*)GetDlgItem(IDC_Precomposed))->GetCheck())
		m_MBFlags |= MB_PRECOMPOSED;
	else
		m_MBFlags &= ~MB_PRECOMPOSED;
	
	if (((CButton*)GetDlgItem(IDC_Composite))->GetCheck())
		m_MBFlags |= MB_COMPOSITE;
	else
		m_MBFlags &= ~MB_COMPOSITE;
	
	if (((CButton*)GetDlgItem(IDC_Useglyph))->GetCheck())
		m_MBFlags |= MB_USEGLYPHCHARS;
	else
		m_MBFlags &= ~MB_USEGLYPHCHARS;
	


	if (((CButton*)GetDlgItem(IDC_Discardns))->GetCheck())
		m_CFlags |= WC_DISCARDNS;
	else
		m_CFlags &= ~WC_DISCARDNS;
	
	if (((CButton*)GetDlgItem(IDC_CompositeCheck))->GetCheck())
		m_CFlags |= WC_COMPOSITECHECK;
	else
		m_CFlags &= ~WC_COMPOSITECHECK;
	
	if (((CButton*)GetDlgItem(IDC_Sepchars))->GetCheck())
		m_CFlags |= WC_SEPCHARS;
	else
		m_CFlags &= ~WC_SEPCHARS;
	
	if (((CButton*)GetDlgItem(IDC_Defaultchar))->GetCheck())
		m_CFlags |= WC_DEFAULTCHAR;
	else
		m_CFlags &= ~WC_DEFAULTCHAR;


	if (((CButton*)GetDlgItem(IDC_UsedDefaultChar))->GetCheck())
		m_UsedDefaultChar = TRUE;
	else m_UsedDefaultChar = FALSE;


	GetDlgItem(IDC_DefaultCharacter)->GetWindowText(sBuffer);
	if ( sBuffer.IsEmpty() )
		strcpy(m_DefaultChar, "");
	else strcpy(m_DefaultChar, (CHAR*)(sBuffer.GetBuffer(0)));


	//SetConversionOption(m_MBFlags, m_CFlags, m_DefChar, m_UsedDefChar);

	CDialog::OnOK();
ECO_EXCEPTION_END
}
