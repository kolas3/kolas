// EditBoxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EditBoxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditBoxDlg dialog


CEditBoxDlg::CEditBoxDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CEditBoxDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditBoxDlg)
	m_nValue = 0;
	// KOL.UDF.022
	m_nCloseMode = 0;
	//}}AFX_DATA_INIT
}


// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CEditBoxDlg::~CEditBoxDlg()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CEditBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditBoxDlg)
	DDX_Text(pDX, IDC_edtVALUE, m_nValue);
	DDV_MinMaxInt(pDX, m_nValue, 1, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditBoxDlg, CDialog)
	//{{AFX_MSG_MAP(CEditBoxDlg)
	ON_BN_CLICKED(IDC_btnAPPLY, OnbtnAPPLY)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditBoxDlg message handlers

VOID CEditBoxDlg::OnbtnAPPLY() 
{
	INT ids;
	ids = UpdateData(TRUE);
	if( !ids ) return;

	m_nCloseMode = 0;

	OnCancel();
}

BOOL CEditBoxDlg::PreTranslateMessage(MSG* pMsg) 
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

BOOL CEditBoxDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	m_nCloseMode = -1;
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CEditBoxDlg::SetValue( CString sValue )
{
	m_nValue = _ttoi(sValue);
}


VOID CEditBoxDlg::GetValue( CString &sValue )
{
	sValue.Format( _T("%d") , m_nValue );
}

INT CEditBoxDlg::IsEdit()
{
	return m_nCloseMode;
}

HBRUSH CEditBoxDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
