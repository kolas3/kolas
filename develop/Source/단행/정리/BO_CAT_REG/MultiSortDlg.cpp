// MultiSortDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultiSortDlg.h"
#include "ESL_DataMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiSortDlg dialog


CMultiSortDlg::CMultiSortDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMultiSortDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiSortDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nSortCount = 0;
}
CMultiSortDlg::~CMultiSortDlg()
{
}

VOID CMultiSortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultiSortDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMultiSortDlg, CDialog)
	//{{AFX_MSG_MAP(CMultiSortDlg)
	ON_BN_CLICKED(IDC_bSORT_MULTI, OnbSORTMULTI)
	
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiSortDlg Operations

INT CMultiSortDlg::Init()
{
	m_nSortColumeID[0] = IDC_cSORT_COLUMN_1;
	m_nSortColumeID[1] = IDC_cSORT_COLUMN_2;
	m_nSortColumeID[2] = IDC_cSORT_COLUMN_3;
	m_nSortColumeID[3] = IDC_cSORT_COLUMN_4;
	m_nSortColumeID[4] = IDC_cSORT_COLUMN_5;
		
	m_nSortCondID[0] = IDC_cSORT_DIR_1;
	m_nSortCondID[1] = IDC_cSORT_DIR_2;
	m_nSortCondID[2] = IDC_cSORT_DIR_3;
	m_nSortCondID[3] = IDC_cSORT_DIR_4;
	m_nSortCondID[4] = IDC_cSORT_DIR_5;

	for (INT idx = 0; idx < 5; idx++)
	{
		m_arrStrSortAlias[idx].Empty();
		m_arrIntSortCond[idx] = ESL_DATAMGR_SORT_ASC;
	}

	// 설정된 값을 돌면서 콤보에 값을 넣어준다.
	INT nCount = m_arrStrSortCols.GetSize();
	CString strCol;
	CComboBox *pComboBox = NULL;
	for (idx = 0; idx < nCount; idx++)
	{
		strCol = m_arrStrSortCols.GetAt(idx);
		for (INT j = 0; j < 5; j++)
		{
			pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortColumeID[j]));
			if (pComboBox == NULL) continue;

			pComboBox->AddString(strCol);
		}
	}

	INT nCurSel = -1;
	for (idx = 0; idx < 5; idx++)
	{
		pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortColumeID[idx]));
		if (pComboBox == NULL) continue;
		nCurSel = pComboBox->AddString(_T("사용안함"));
		if (idx < nCurSel)
			pComboBox->SetCurSel(idx);
		else
			pComboBox->SetCurSel(nCurSel);


		pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortCondID[idx]));
		if (pComboBox == NULL) continue;

		pComboBox->AddString(_T("오름차순"));
		pComboBox->AddString(_T("내림차순"));
		pComboBox->SetCurSel(0);
	}

	if (nCount < 5)
	{
		for (idx = 0; idx < 5 - nCount; idx++)
		{
			pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortColumeID[4 - idx]));
			if (pComboBox == NULL) continue;
			pComboBox->EnableWindow(FALSE);

			pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortCondID[4 - idx]));
			if (pComboBox == NULL) continue;
			pComboBox->EnableWindow(FALSE);
		}
	}

	return 0;
}

INT CMultiSortDlg::AddSortInfo(CString strDMFieldAlias, CString strDesc)
{
	m_arrStrSortCols.Add(strDesc);
	m_mapDMAlias_Desc.SetAt(strDesc, strDMFieldAlias);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMultiSortDlg message handlers

BOOL CMultiSortDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	Init();

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMultiSortDlg::OnbSORTMULTI() 
{
	INT nSel = -1;
	INT nCond = -1;
	CString strCol;
	CComboBox *pComboBox = NULL;
	INT n = 0;
	for (INT idx = 0; idx < 5; idx++)
	{
		pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortColumeID[idx]));
		if (!pComboBox) continue;
		nSel = pComboBox->GetCurSel();
		pComboBox->GetLBText(nSel, strCol);
		
		if (strCol == _T("사용안함")) continue;

		m_mapDMAlias_Desc.Lookup(strCol, strCol);
		if (strCol.IsEmpty()) continue;

		m_arrStrSortAlias[n] = strCol;


		pComboBox = static_cast<CComboBox*>(GetDlgItem(m_nSortCondID[idx]));
		if (!pComboBox) continue;

		nSel = pComboBox->GetCurSel();
		if (nSel == 0)
			nCond = ESL_DATAMGR_SORT_ASC;
		else
			nCond = ESL_DATAMGR_SORT_DESC;		

		m_arrIntSortCond[n] = nCond;

		n++;
	}

	m_nSortCount = n;

	EndDialog(IDOK);
}



HBRUSH CMultiSortDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

