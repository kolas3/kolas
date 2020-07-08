// FileLoanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileLoanDlg.h"
#include "StdioFileEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileLoanDlg dialog


CFileLoanDlg::CFileLoanDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileLoanDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pLoan = NULL;
	m_pLoanMangeValue = NULL;
	m_pLoanShareManager = NULL;
}

CFileLoanDlg::~CFileLoanDlg()
{

}

void CFileLoanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileLoanDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileLoanDlg, CDialog)
	//{{AFX_MSG_MAP(CFileLoanDlg)
	ON_BN_CLICKED(IDC_btnFILE_LOAD, OnbtnFILELOAD)
	ON_BN_CLICKED(IDC_btnFILE_LOAN, OnbtnFILELOAN)
	ON_BN_CLICKED(IDC_btnFILE_SAVE1, OnbtnFILESAVE1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileLoanDlg message handlers

BOOL CFileLoanDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd());

	InitESL_Mgr(_T("SM_FILE_LOAN"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFileLoanDlg::OnbtnFILELOAD() 
{
	// TODO: Add your control notification handler code here
	TCHAR szPath[2000] = {0}; 
	GetCurrentDirectory(2000, szPath);

	CTime t = CTime::GetCurrentTime();
	CString filename, msg;
	
	CFileDialog FileDlg(
		TRUE,
		_T("txt"),
		_T(""),
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("텍스트 파일(*.txt)|*.txt|모든 파일(*.*)|*.*||")
		);

	if(IDOK != FileDlg.DoModal())
	{
		return;
	}

	SetCurrentDirectory(szPath);				
	SetDlgItemText(IDC_EDIT1, FileDlg.GetPathName());
	CString strFileName = FileDlg.GetPathName();

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_FILE_LOAN"), _T("그리드"));
	if(NULL == pGrid)
	{
		return;
	}
	
	CStdioFileEx File;
	CFileException ex;	
	if(!File.Open(strFileName, CFile::modeRead|CFile::typeText, &ex))
	{
		TCHAR szError[2000] = {0};
		ex.GetErrorMessage(szError, 2000);
		AfxMessageBox(szError);
		return;
	}	

	pGrid->Display();
	pGrid->SetRedraw(FALSE);	

	INT nRow = 2;
	CString strLine;
	while(File.ReadString(strLine))
	{		
		pGrid->SetRows(nRow);
		pGrid->SetTextMatrix(nRow-1, 1, strLine);
		nRow++;
	}
	File.Close();

	pGrid->SetRedraw(TRUE);
	pGrid->Invalidate();

	GetDlgItem(IDC_btnFILE_LOAN)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnFILE_SAVE1)->EnableWindow(FALSE);
}

void CFileLoanDlg::OnbtnFILELOAN() 
{
	// TODO: Add your control notification handler code here	
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_FILE_LOAN"), _T("그리드"));
	if(NULL == pGrid)
	{
		return;
	}

	INT ids = 0;
	CString strLine;
	for(INT nRow = 1; nRow < pGrid->GetRows(); nRow++)
	{
		strLine = pGrid->GetTextMatrix(nRow, 1);		
		SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), strLine);
		ids = m_pLoan->SPFExcutePath(_T("대출"));

		if(0 == ids)
		{
			pGrid->SetTextMatrix(nRow, 2, _T("성공"));
		}
		else if(1 == ids)
		{
			CBO_LOC_3192 Dlg(this, m_pLoanShareManager, m_pLoanMangeValue, 1);
			Dlg.DoModal();

			pGrid->SetTextMatrix(nRow, 2, _T("성공"));
		}
		else
		{
			pGrid->SetTextMatrix(nRow, 2, _T("실패"));
		}
	}

	GetDlgItem(IDC_btnFILE_LOAN)->EnableWindow(FALSE);
	GetDlgItem(IDC_btnFILE_SAVE1)->EnableWindow(TRUE);	
}

void CFileLoanDlg::OnbtnFILESAVE1() 
{ 
	TCHAR szPath[2000] = {0}; 
	GetCurrentDirectory(2000, szPath);
	CString strFile;
  
	CFileDialog FileDlg(
		FALSE,
		_T("txt"),
		_T(""),
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("텍스트 파일(*.txt)|*.txt|모든 파일(*.*)|*.*||")
		);

	if(IDOK == FileDlg.DoModal())
	{
		SetCurrentDirectory(szPath);
		strFile = FileDlg.GetPathName();
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_FILE_LOAN"), _T("그리드"));
	if(NULL == pGrid)
	{
		return;
	}

	CStdioFileEx File;
	CFileException ex;	
	if(!File.Open(strFile, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode, &ex))
	{
		TCHAR szError[2000] = {0};
		ex.GetErrorMessage(szError, 2000);
		AfxMessageBox(szError);
		return;
	}	

	INT ids = 0;
	CString strLine, strResult;
	for(INT nRow = 1; nRow < pGrid->GetRows(); nRow++)
	{
		strResult = pGrid->GetTextMatrix(nRow, 2);
		if(_T("실패") == strResult)
		{
			strResult.Format(_T("%s\r\n"), pGrid->GetTextMatrix(nRow, 1));
			File.WriteString(strResult);
		}
	}
	File.Close();
}
