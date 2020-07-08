// Kolas2up_ImageEditorResizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Kolas2up_ImageEditorResizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKolas2up_ImageEditorResizeDlg dialog


CKolas2up_ImageEditorResizeDlg::CKolas2up_ImageEditorResizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKolas2up_ImageEditorResizeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKolas2up_ImageEditorResizeDlg)
	m_nWidth = 0;
	m_nHeight = 0;
	m_strRatio = _T("");
	//}}AFX_DATA_INIT

	m_fRatio = 1.0;
}


VOID CKolas2up_ImageEditorResizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKolas2up_ImageEditorResizeDlg)
	DDX_Text(pDX, IDC_eWIDTH, m_nWidth);
	DDX_Text(pDX, IDC_eHEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_STATIC_RATIO, m_strRatio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKolas2up_ImageEditorResizeDlg, CDialog)
	//{{AFX_MSG_MAP(CKolas2up_ImageEditorResizeDlg)
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	ON_EN_CHANGE(IDC_eWIDTH, OnChangeeWIDTH)
	ON_EN_CHANGE(IDC_eHEIGHT, OnChangeeHEIGHT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKolas2up_ImageEditorResizeDlg message handlers

VOID CKolas2up_ImageEditorResizeDlg::OnbOK() 
{
	UpdateData(TRUE);

	CDialog::OnOK();
}

VOID CKolas2up_ImageEditorResizeDlg::OnbCANCEL() 
{
	CDialog::OnCancel();	
}

VOID CKolas2up_ImageEditorResizeDlg::OnChangeeWIDTH() 
{
	UpdateData(TRUE);
	m_nHeight = GetHeight(m_nWidth);
	UpdateData(FALSE);
}

VOID CKolas2up_ImageEditorResizeDlg::OnChangeeHEIGHT() 
{
	UpdateData(TRUE);
	m_nWidth = GetWidth(m_nHeight);	
	UpdateData(FALSE);
}

INT CKolas2up_ImageEditorResizeDlg::GetWidth(INT cy)
{
	return ((INT)((double)cy / m_fRatio));
}

INT CKolas2up_ImageEditorResizeDlg::GetHeight(INT cx)
{
	return ((INT)((double)cx * m_fRatio));
}
