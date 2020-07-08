// EditBoxDlg2.cpp : implementation file
//

#include "stdafx.h"
#include "EditBoxDlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditBoxDlg2 dialog


CEditBoxDlg2::CEditBoxDlg2(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CEditBoxDlg2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditBoxDlg2)
	m_nValue = 0;
	m_nValue2 = 0;
	// KOL.UDF.022
	m_nCloseMode = 0;
	//}}AFX_DATA_INIT
}
// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CEditBoxDlg2::~CEditBoxDlg2()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CEditBoxDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditBoxDlg2)
	DDX_Text(pDX, IDC_edtVALUE, m_nValue);
	DDV_MinMaxInt(pDX, m_nValue, 0, 10000);
	DDX_Text(pDX, IDC_edtVALUE2, m_nValue2);
	DDV_MinMaxInt(pDX, m_nValue2, 0, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditBoxDlg2, CDialog)
	//{{AFX_MSG_MAP(CEditBoxDlg2)
	ON_BN_CLICKED(IDC_btnAPPLY, OnbtnAPPLY)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditBoxDlg2 message handlers

BOOL CEditBoxDlg2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	m_nCloseMode = -1;
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CEditBoxDlg2::OnbtnAPPLY() 
{
	INT ids;
	ids = UpdateData(TRUE);
	if( !ids ) return;

	m_nCloseMode = 0;

	OnCancel();
}



VOID CEditBoxDlg2::SetValue( CString sValue , CString sValue2 )
{
	m_nValue = _ttoi(sValue);
	m_nValue2 = _ttoi(sValue2);
}


VOID CEditBoxDlg2::GetValue( CString &sValue , CString &sValue2 )
{
	sValue.Format( _T("%d") , m_nValue );
	sValue2.Format( _T("%d") , m_nValue2 );
}

INT CEditBoxDlg2::IsEdit()
{
	return m_nCloseMode;
}

BOOL CEditBoxDlg2::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->wParam == VK_RETURN )
	{
		OnbtnAPPLY();
		return TRUE;
	}
	else if( pMsg->wParam == VK_ESCAPE )
	{
		OnCancel();
		return TRUE;
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CEditBoxDlg2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
