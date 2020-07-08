// BOISBNAdditionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BOISBNAdditionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBOISBNAdditionDlg dialog


CBOISBNAdditionDlg::CBOISBNAdditionDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBOISBNAdditionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pFormApi = NULL;
	m_pFormApi = new CBOFormApi(this);

	m_pGrid = NULL;
	m_pGrid = new CESL_Grid(NULL);
}

VOID CBOISBNAdditionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOISBNAdditionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBOISBNAdditionDlg, CDialog)
	//{{AFX_MSG_MAP(CBOISBNAdditionDlg)
	ON_BN_CLICKED(IDC_bISBN_CHECK, OnbISBNCHECK)
	ON_BN_CLICKED(IDC_bINPUT_ISBN, OnbINPUTISBN)
	ON_BN_CLICKED(IDC_bMODIFY_ISBN, OnbMODIFYISBN)
	ON_BN_CLICKED(IDC_bDELETE_ISBN, OnbDELETEISBN)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bCLOSE_ISBN, OnbCLOSEISBN)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBOISBNAdditionDlg Operations

INT CBOISBNAdditionDlg::ValidateData()
{
	// 낱권 ISBN 체크
	CString strItem;
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_eEA_ISBN);
	if (pEdit)
	{
		pEdit->GetWindowText(strItem);
		if (strItem.IsEmpty())
		{
			AfxMessageBox(_T("낱권 ISBN항목을 입력하세요"));
			pEdit->SetFocus();
			return -1;
		}
	}
	
	return 0;
}

INT CBOISBNAdditionDlg::Display()
{
	if (m_pGrid == NULL) return -1;

	m_pGrid->Clear();
	m_pGrid->SetCols(0, 4);
	m_pGrid->SetColumnInfo(0, _T("낱권ISBN"), 80, 4);
	m_pGrid->SetColumnInfo(1, _T("낱권ISBN부가기호"), 150, 4);
	m_pGrid->SetColumnInfo(2, _T("오류ISBN"), 80, 4);

	INT nRow = m_arrISBN.GetSize();
	if (nRow <= 0) return -1;

	m_pGrid->SetRows(nRow + 1);
	CISBN isbn;
	CString strIdx;
	for (INT idx = 0; idx < nRow; idx++)
	{
		isbn = m_arrISBN.GetAt(idx);
		strIdx.Format(_T("%d"), idx + 1);

		m_pGrid->SetTextMatrix(idx + 1, 0, strIdx);
		m_pGrid->SetTextMatrix(idx + 1, 1, isbn.m_strISBN1);
		m_pGrid->SetTextMatrix(idx + 1, 2, isbn.m_strISBN2);
		m_pGrid->SetTextMatrix(idx + 1, 3, isbn.m_strISBN3);
	}

	return 0;
}

INT CBOISBNAdditionDlg::EncodeAdditionISBN()
{
	INT nRow = m_arrISBN.GetSize();
	CISBN isbn;
	m_strAdditionISBN.Empty();

	for (INT idx = 0; idx < nRow; idx ++)
	{
		isbn.Init();
		isbn = m_arrISBN.GetAt(idx);
		
		m_strAdditionISBN += isbn.m_strISBN1;
		m_strAdditionISBN += _T("|");
		m_strAdditionISBN += isbn.m_strISBN2;
		m_strAdditionISBN += _T("|");
		m_strAdditionISBN += isbn.m_strISBN3;
		m_strAdditionISBN += _T(";");		
	}

	return 0;
}

INT CBOISBNAdditionDlg::DecodeAdditionISBN()
{
	if (m_strAdditionISBN.IsEmpty())
		return 0;
	
	// 각 필드의 구분자 : '|'
	// 각 ISBN의 구분자 : ';'

	INT ids = -1;
	CISBN isbn;

	INT nFind = -1;

	CString strAdditionISBN = m_strAdditionISBN;
	CString strISBN;

	while (TRUE)
	{
		nFind = strAdditionISBN.Find(';');
		if (nFind < 0)
		{
			nFind = strAdditionISBN.GetLength() - 1;
			if (nFind < 0) break;

			strISBN  = strAdditionISBN;
		}
		else
		{
			strISBN = strAdditionISBN.Mid(0, nFind);
		}

		strAdditionISBN = strAdditionISBN.Mid(nFind + 1);
		
		ids = ParseISBN(strISBN, isbn);
		if (ids < 0) continue;

		// 어레이에 추가한다.
		m_arrISBN.Add(isbn);

		if (strAdditionISBN.IsEmpty()) break;
	}

	return 0;
}

INT CBOISBNAdditionDlg::ParseISBN(CString strISBN, CISBN &isbn)
{
	if (strISBN.IsEmpty()) return -1;

	CString strItem;
	INT n = 0;
	INT nFind = -1;
	while (TRUE)
	{
		nFind = strISBN.Find('|');
		if (nFind < 0)
		{
			nFind = strISBN.GetLength() - 1;
			if (nFind < 0) break;

			strItem = strISBN;
		}
		else
		{
			if (nFind == 0)
			{
				strItem = _T("");
			}
			else
			{
				strItem = strISBN.Mid(0, nFind);
			}
		}

		strISBN = strISBN.Mid(nFind + 1);

		if (n == 0)
			isbn.m_strISBN1 = strItem;
		else if (n == 1)
			isbn.m_strISBN2 = strItem;
		else if (n == 2)
			isbn.m_strISBN3 = strItem;

		n++;

		if (strISBN.IsEmpty()) break;		
	}


	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CBOISBNAdditionDlg message handlers

BOOL CBOISBNAdditionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Subclass Grid
	if (!m_pGrid->SubclassDlgItem(IDC_gISBN, this))
	{
		AfxMessageBox(_T("Subclass Grid Failed !"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	// Decode Addition ISBN String
	if (DecodeAdditionISBN() < 0)
	{		
		AfxMessageBox(_T("Decode Addition ISBN String Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}	

	// Display
	Display();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBOISBNAdditionDlg::OnbISBNCHECK() 
{
	// 낱권 체크
	CString strEaISBNFrom;
	CString strEaISBNTo;
	GetDlgItem(IDC_eEA_ISBN)->GetWindowText(strEaISBNFrom);
	if (strEaISBNFrom.IsEmpty()) return;

	INT ids = m_pFormApi->CheckISBN(strEaISBNFrom, strEaISBNTo);
	if (ids < 0) return;
	
	GetDlgItem(IDC_eEA_ISBN)->SetWindowText(strEaISBNTo);
}

VOID CBOISBNAdditionDlg::OnbINPUTISBN() 
{
	if (ValidateData() < 0) return;

	CISBN isbn;
	
	GetDlgItem(IDC_eEA_ISBN)->GetWindowText(isbn.m_strISBN1);
	GetDlgItem(IDC_eEA_ISBN_ADD)->GetWindowText(isbn.m_strISBN2);
	GetDlgItem(IDC_eEA_ISBN_MISTAKE)->GetWindowText(isbn.m_strISBN3);

	m_arrISBN.Add(isbn);

	Display();

	INT nRow = m_pGrid->GetRows() - 1;

	m_pGrid->SetTopRow(nRow);
	m_pGrid->SetReverse(nRow - 1);

}

VOID CBOISBNAdditionDlg::OnbMODIFYISBN() 
{
	if (m_arrISBN.GetSize() <= 0) return;
	if (ValidateData() < 0) return;

	CISBN isbn;
	
	GetDlgItem(IDC_eEA_ISBN)->GetWindowText(isbn.m_strISBN1);
	GetDlgItem(IDC_eEA_ISBN_ADD)->GetWindowText(isbn.m_strISBN2);
	GetDlgItem(IDC_eEA_ISBN_MISTAKE)->GetWindowText(isbn.m_strISBN3);

	INT idx = m_pGrid->GetRow() - 1;

	m_arrISBN.SetAt(idx, isbn);

	Display();
	m_pGrid->SetTopRow(idx + 1);
	m_pGrid->SetReverse(idx);
}

VOID CBOISBNAdditionDlg::OnbDELETEISBN() 
{
	if (m_arrISBN.GetSize() <= 0) return;

	INT idx = m_pGrid->GetRow() - 1;
	m_arrISBN.RemoveAt(idx);

	Display();
	
	if (idx == 0)
		idx = 1;
	m_pGrid->SetTopRow(idx);
	m_pGrid->SetReverse(idx - 1);
}

VOID CBOISBNAdditionDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pFormApi != NULL)
	{
		delete m_pFormApi;
		m_pFormApi = NULL;
	}

	if (m_pGrid != NULL)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}
}

BEGIN_EVENTSINK_MAP(CBOISBNAdditionDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CBOISBNAdditionDlg)
	ON_EVENT(CBOISBNAdditionDlg, IDC_gISBN, -605 /* MouseDown */, OnMouseDowngISBN, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBOISBNAdditionDlg::OnMouseDowngISBN(short Button, short Shift, long x, long y) 
{
	INT nRow = m_pGrid->GetRow() - 1;
	
	if (m_arrISBN.GetSize() <= 0) return;
	CISBN isbn = m_arrISBN.GetAt(nRow);

	GetDlgItem(IDC_eEA_ISBN)->SetWindowText(isbn.m_strISBN1);
	GetDlgItem(IDC_eEA_ISBN_ADD)->SetWindowText(isbn.m_strISBN2);
	GetDlgItem(IDC_eEA_ISBN_MISTAKE)->SetWindowText(isbn.m_strISBN3);

 
	m_pGrid->SetReverse(nRow);

}

VOID CBOISBNAdditionDlg::OnbCLOSEISBN() 
{
	EncodeAdditionISBN();
	CDialog::OnOK();
}
