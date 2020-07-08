// ESLDlg_ExportType.cpp : implementation file
//

#include "stdafx.h"

#include "ESLDlg_ExportType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_ExportType dialog


CESLDlg_ExportType::CESLDlg_ExportType(CWnd* pParent /*=NULL*/)
	: CDialog(CESLDlg_ExportType::IDD, pParent)
{
	//{{AFX_DATA_INIT(CESLDlg_ExportType)
		m_bIsExcel			= FALSE;
		m_bIsHancell		= FALSE;
		m_bIsCSV			= FALSE;
		m_bExistsExcelStyle = FALSE;
		m_bExistsCSVStyle	= FALSE;
		m_bVisibleCSV		= FALSE;
		m_bVisibleExcel		= FALSE;
		m_bVisibleHancell	= FALSE;
		m_nTypeCount		= 0;
	//}}AFX_DATA_INIT
}


void CESLDlg_ExportType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CESLDlg_ExportType)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CESLDlg_ExportType, CDialog)
	//{{AFX_MSG_MAP(CESLDlg_ExportType)
	ON_BN_CLICKED(IDC_BTN_TYPE_SELECT, OnBtnTypeSelect)
	ON_BN_CLICKED(IDC_BTN_TYPE_CANCEL, OnBtnTypeCancel)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_ExportType message handlers

BOOL CESLDlg_ExportType::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CLSID clsExcelApp;

	BOOL	bIsHancell	= FALSE,
			bIsExcel	= FALSE,
			bIsCSV		= FALSE
			;
	INT i = -1;

	// 한셀 설치여부 확인
	if (!FAILED(CLSIDFromProgID(_T("HCell.Application"), &clsExcelApp))) 
	{
		bIsHancell = TRUE;
	}
	// 엑셀 설치여부 확인
	if (!FAILED(CLSIDFromProgID(_T("Excel.Application"), &clsExcelApp))) 
	{
		bIsExcel = TRUE;
	}
	// CSV는 기본
	if(TRUE)
	{
		bIsCSV = TRUE;
	}
	//->IsWindowVisible()
	
	// 스타일파일이 있을경우
	if(	m_bExistsExcelStyle == TRUE || m_bExistsCSVStyle == TRUE )
	{
		if(m_bExistsExcelStyle == FALSE)
		{
			GetDlgItem(IDC_RADIO_TYPE_HANCELL)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_RADIO_TYPE_EXCEL)->ShowWindow(SW_HIDE);
			bIsHancell = FALSE;
			bIsExcel = FALSE;
		}
		if(m_bExistsCSVStyle == FALSE)
		{
			GetDlgItem(IDC_RADIO_TYPE_CSV)->ShowWindow(SW_HIDE);
			bIsCSV = FALSE;
		}
	}

	if(bIsHancell == FALSE)
	{
		GetDlgItem(IDC_RADIO_TYPE_HANCELL)->ShowWindow(SW_HIDE);
	}
	else
	{
		m_bVisibleHancell = TRUE;
		m_nTypeCount++;
	}

	if(bIsExcel == FALSE)
	{
		GetDlgItem(IDC_RADIO_TYPE_EXCEL)->ShowWindow(SW_HIDE);
	}
	else
	{
		m_bVisibleExcel = TRUE;
		m_nTypeCount++;
	}

	if(bIsCSV == FALSE)
	{
		GetDlgItem(IDC_RADIO_TYPE_CSV)->ShowWindow(SW_HIDE);
	}
	else
	{
		m_bVisibleCSV = TRUE;
		m_nTypeCount++;
	}

	// 출력가능한 종류가 하나밖에 없으면 선택창없이 리턴
	if(m_nTypeCount == 1)
	{
		m_bIsHancell	= bIsHancell;
		m_bIsExcel		= bIsExcel;
		m_bIsCSV		= bIsCSV;
		CDialog::OnOK();
	}
	else if(m_nTypeCount == 0)
	{
		m_bIsHancell	= FALSE;
		m_bIsExcel		= FALSE;
		m_bIsCSV		= FALSE;
		CDialog::OnCancel();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CESLDlg_ExportType::OnBtnTypeSelect() 
{
	m_bIsHancell = ((CButton*)GetDlgItem(IDC_RADIO_TYPE_HANCELL))->GetCheck();
	m_bIsExcel = ((CButton*)GetDlgItem(IDC_RADIO_TYPE_EXCEL))->GetCheck();
	m_bIsCSV = ((CButton*)GetDlgItem(IDC_RADIO_TYPE_CSV))->GetCheck();
	
	CDialog::OnOK();
}

void CESLDlg_ExportType::OnBtnTypeCancel() 
{
	m_bIsHancell	= FALSE;
	m_bIsExcel		= FALSE;
	m_bIsCSV		= FALSE;
	CDialog::OnCancel();	
}

BOOL CESLDlg_ExportType::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CDialog::Create(IDD, pParentWnd);
}

void CESLDlg_ExportType::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
}

void CESLDlg_ExportType::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rRect;
	BOOL bVisible = FALSE;
	this->GetWindowRect(rRect);
	ScreenToClient(rRect);

	INT nDialogWidth = rRect.Width();
	INT nIndexPos = 0, nBeforeIndexPos = 0;
	INT i = -1, nIndexCount = 0;
	
	INT nRadioArray[] = 
	{
		IDC_RADIO_TYPE_EXCEL, IDC_RADIO_TYPE_HANCELL, IDC_RADIO_TYPE_CSV, 0
	};

	while(nRadioArray[++i] > 0)
	{
		bVisible = (GetDlgItem(nRadioArray[i])->IsWindowVisible());
		if(bVisible == TRUE)
		{
			++nIndexCount;

			if(nIndexCount == 1)
			{
				((CButton*)GetDlgItem(nRadioArray[i]))->SetCheck(TRUE);
			}

			GetDlgItem(nRadioArray[i])->GetWindowRect(rRect);
			ScreenToClient(rRect);

			// 왼쪽 8픽셀 오른쪽 2픽셀 = 10픽셀 여유분, 기본크기 74
			nIndexPos = (nDialogWidth / m_nTypeCount) * nIndexCount;

			rRect.left = (nIndexPos - nBeforeIndexPos) / 2 - 37 + nBeforeIndexPos + 4;
			rRect.right = rRect.left + 74;
			GetDlgItem(nRadioArray[i])->MoveWindow(rRect);

			nBeforeIndexPos = nIndexPos;
		}
	}
		//m_pMessageDlg[m_nMessageLoop]->MoveWindow(rect.left, rect.top + (rect.Height()*m_nMessageLoop), rect.Width(), rect.Height());
}
