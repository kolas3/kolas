// DlgSymbolColor.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSymbolColor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSymbolColor dialog


CDlgSymbolColor::CDlgSymbolColor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSymbolColor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSymbolColor)
	m_bBold = FALSE;
	//}}AFX_DATA_INIT

	pParentEditCtrl = (CMarcEditCtrl*)pParent;
	
	// KOL.UDF.022
	m_crColor = NULL;
}


VOID CDlgSymbolColor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSymbolColor)
	DDX_Control(pDX, IDC_bCOLOR, m_btnColor);
	DDX_Check(pDX, IDC_ckBOLD, m_bBold);
	//}}AFX_DATA_MAP
	DDX_ColorPicker(pDX, IDC_bCOLOR, m_crColor);
}


BEGIN_MESSAGE_MAP(CDlgSymbolColor, CDialog)
	//{{AFX_MSG_MAP(CDlgSymbolColor)
	ON_LBN_SELCHANGE(IDC_lSYMBOL, OnSelchangelSYMBOL)
	ON_BN_CLICKED(IDC_ckBOLD, OnckBOLD)
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_SELENDOK, OnSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSymbolColor message handlers

BOOL CDlgSymbolColor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Items
	CListBox *pListBox = (CListBox*)GetDlgItem(IDC_lSYMBOL);

	pListBox->AddString(_T("텍스트"));
	pListBox->AddString(_T("숫자"));
	pListBox->AddString(_T("테그코드"));
	pListBox->AddString(_T("지시기호"));
	pListBox->AddString(_T("서브필드코드"));
	pListBox->AddString(_T("필드종단기호"));
	pListBox->AddString(_T("레코드종단기호"));
	pListBox->AddString(_T("에러문자"));
	pListBox->AddString(_T("배경색"));

	ButtonStatus();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDlgSymbolColor::ButtonStatus()
{
	CListBox *pListBox = (CListBox*)GetDlgItem(IDC_lSYMBOL);
	INT i = pListBox->GetCurSel();
	if (i == LB_ERR)
	{
		m_btnColor.EnableWindow(FALSE);
		GetDlgItem(IDC_ckBOLD)->ShowWindow(SW_HIDE);
	}
	else if (i == 8)
	{
		GetDlgItem(IDC_ckBOLD)->ShowWindow(SW_HIDE);
		m_btnColor.EnableWindow(TRUE);
	}
	else
	{
		m_btnColor.EnableWindow(TRUE);
		GetDlgItem(IDC_ckBOLD)->ShowWindow(SW_SHOW);
	}
}

VOID CDlgSymbolColor::OnSelchangelSYMBOL() 
{
	CListBox *pListBox = (CListBox*)GetDlgItem(IDC_lSYMBOL);
	INT i = pListBox->GetCurSel();
	
	if (i == 0)
	{
		pParentEditCtrl->GetTextColor(m_crColor, m_bBold);
	}
	else if (i == 1)
	{
		pParentEditCtrl->GetNumberColor(m_crColor, m_bBold);
	}
	else if (i == 2)
	{
		pParentEditCtrl->GetTagCodeColor(m_crColor, m_bBold);
	}
	else if (i == 3)
	{
		pParentEditCtrl->GetIndicatorCodeColor(m_crColor, m_bBold);
	}
	else if (i == 4)
	{
		pParentEditCtrl->GetSubfieldCodeColor(m_crColor, m_bBold);
	}
	else if (i == 5)
	{
		pParentEditCtrl->GetFieldTerminatorColor(m_crColor, m_bBold);
	}
	else if (i == 6)
	{
		pParentEditCtrl->GetRecordTerminatorColor(m_crColor, m_bBold);
	}
	else if (i == 7)
	{
		pParentEditCtrl->GetErrorColor(m_crColor, m_bBold);
	}
	else if (i == 8)
	{
		pParentEditCtrl->GetBkColor(m_crColor, m_bBold);
	}

	UpdateData(FALSE);

	ButtonStatus();	
}

LRESULT CDlgSymbolColor::OnSelChange(WPARAM wParam, LPARAM lParam) 
{
	UpdateData();

	CListBox *pListBox = (CListBox*)GetDlgItem(IDC_lSYMBOL);
	INT i = pListBox->GetCurSel();
	if (i == LB_ERR)
		return 0L;

	if (i == 0)
	{
		pParentEditCtrl->SetTextColor(m_crColor, m_bBold);
	}
	else if (i == 1)
	{
		pParentEditCtrl->SetNumberColor(m_crColor, m_bBold);
	}
	else if (i == 2)
	{
		pParentEditCtrl->SetTagCodeColor(m_crColor, m_bBold);
	}
	else if (i == 3)
	{
		pParentEditCtrl->SetIndicatorCodeColor(m_crColor, m_bBold);
	}
	else if (i == 4)
	{
		pParentEditCtrl->SetSubfieldCodeColor(m_crColor, m_bBold);
	}
	else if (i == 5)
	{
		pParentEditCtrl->SetFieldTerminatorColor(m_crColor, m_bBold);
	}
	else if (i == 6)
	{
		pParentEditCtrl->SetRecordTerminatorColor(m_crColor, m_bBold);
	}
	else if (i == 7)
	{
		pParentEditCtrl->SetErrorColor(m_crColor, m_bBold);
	}
	else if (i == 8)
	{
		pParentEditCtrl->SetBkColor(m_crColor, m_bBold);
	}

	pParentEditCtrl->FormatAll();

	
	return 0L;
}

VOID CDlgSymbolColor::OnckBOLD() 
{
	UpdateData();

	CListBox *pListBox = (CListBox*)GetDlgItem(IDC_lSYMBOL);
	INT i = pListBox->GetCurSel();
	if (i == LB_ERR)
		return;

	if (i == 0)
	{
		pParentEditCtrl->SetTextColor(m_crColor, m_bBold);
	}
	else if (i == 1)
	{
		pParentEditCtrl->SetNumberColor(m_crColor, m_bBold);
	}
	else if (i == 2)
	{
		pParentEditCtrl->SetTagCodeColor(m_crColor, m_bBold);
	}
	else if (i == 3)
	{
		pParentEditCtrl->SetIndicatorCodeColor(m_crColor, m_bBold);
	}
	else if (i == 4)
	{
		pParentEditCtrl->SetSubfieldCodeColor(m_crColor, m_bBold);
	}
	else if (i == 5)
	{
		pParentEditCtrl->SetFieldTerminatorColor(m_crColor, m_bBold);
	}
	else if (i == 6)
	{
		pParentEditCtrl->SetRecordTerminatorColor(m_crColor, m_bBold);
	}
	else if (i == 7)
	{
		pParentEditCtrl->SetErrorColor(m_crColor, m_bBold);
	}
	else if (i == 8)
	{
		pParentEditCtrl->SetTagCodeColor(m_crColor, m_bBold);
	}
	else if (i == 9)
	{
		pParentEditCtrl->SetBkColor(m_crColor, m_bBold);
	}

	pParentEditCtrl->FormatAll();

	
}
