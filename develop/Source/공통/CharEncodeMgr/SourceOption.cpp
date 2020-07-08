// SourceOption.cpp : implementation file
//

#include "stdafx.h"
#include "SourceOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSourceOption dialog


CSourceOption::CSourceOption(INT	IsUnicode, CWnd* pParent /*=NULL*/)
	: CharConvert(CSourceOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSourceOption)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_IsUnicode = IsUnicode;
}


void CSourceOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSourceOption)
	DDX_Control(pDX, IDC_CodePage, m_CodePage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSourceOption, CDialog)
	//{{AFX_MSG_MAP(CSourceOption)
	ON_BN_CLICKED(IDC_RAnsi, OnRAnsi)
	ON_BN_CLICKED(IDC_ROEM, OnRoem)
	ON_BN_CLICKED(IDC_ROther, OnROther)
	ON_BN_CLICKED(IDC_RUnicode, OnRUnicode)
	ON_BN_CLICKED(IDC_RUTF8, OnRutf8)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSourceOption message handlers

BOOL CSourceOption::OnInitDialog() 
{

	CDialog::OnInitDialog();

	CString				sBuffer, sComboData;
	_TCHAR				*pszCodePageDesc;
	INT					nCurCodePage, nIdx, ids;


	// Get Current Code page Initialize
	GetCurrCodePage	(&nCurCodePage);

	// If Source is Unicode, then Destination cannot set Unicode. So Unicode Radio Button was Disable
	if ( ( m_IsUnicode == ISUNICODE || m_IsUnicode == ISUTF8 ) && nCurCodePage == m_SourceCodePage )
	{
		GetDlgItem(IDC_RAnsi   )->EnableWindow(FALSE);
		GetDlgItem(IDC_ROEM    )->EnableWindow(FALSE);
		GetDlgItem(IDC_ROther  )->EnableWindow(FALSE);
		GetDlgItem(IDC_CodePage)->EnableWindow(FALSE);

		if ( m_IsUnicode == ISUNICODE )
		{
			GetDlgItem(IDC_RUTF8)->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_RUnicode))->SetCheck(1);		
		}
		else
		{
			GetDlgItem(IDC_RUnicode)->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_RUTF8))->SetCheck(1);		
		}
	}	
	// If Source is MultiByteCode, then Destination only can set Unicode. 
	else
	{
		nIdx = 0;
		// Get All Code Pages
		GetAllCodePage();		
		m_CodePage.FormatList( 2 , DT_LEFT , FALSE , EDGE , 0 );

		// Insert Combo Box
		nIdx = 0;
		for ( INT i = 0 ; i < m_iCodePageCnt ; i++ )
		{
			// Get CodePage Description
			ids = ChangeCodepageToDesc(_ttoi(m_pszCodePage[i]), &pszCodePageDesc);
			if ( ids == GENERAL_ERROR )
			{
				sBuffer = _T("ChangeCodepageToDesc Error!!!");
				AfxMessageBox(sBuffer);
			}
			else if ( ids == LOOP_CONTINUE )
				continue;

			m_CodePage.AddItem(m_pszCodePage[i],0,nIdx);
			m_CodePage.AddItem(pszCodePageDesc,1,nIdx++);

			free(pszCodePageDesc);
		}
		
		m_CodePage.SetCurSel(0);


		if ( nCurCodePage == m_SourceCodePage )
		{
			((CButton*)GetDlgItem(IDC_RAnsi ))->SetCheck(1);
			GetDlgItem(IDC_CodePage)->EnableWindow(FALSE);	
		}
		else
		{
			sBuffer.Format(_T("%d"), m_SourceCodePage);
			for ( i = 0 ; i < nIdx ; i++ )
			{
				m_CodePage.GetLBText(i, sComboData);
				if ( _tcscmp(sComboData.GetBuffer(0), sBuffer.GetBuffer(0)) == 0 )
				{
					((CButton*)GetDlgItem(IDC_ROther ))->SetCheck(1);
					m_CodePage.SetCurSel(i);
					GetDlgItem(IDC_CodePage)->EnableWindow(TRUE);	
					break;
				}
			}			
		}

		GetDlgItem(IDC_RUnicode)->EnableWindow(FALSE);
		GetDlgItem(IDC_RUTF8)->EnableWindow(FALSE);
	}
	
	FreeCodePages(m_pszCodePage, m_iCodePageCnt);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSourceOption::OnOK() 
{

	CString			sBuffer;
	_TCHAR		*pszCodePageDesc;
	INT				nIdx;
	INT				nCurCodePage;
	

	if ( ((CButton*)GetDlgItem(IDC_RUnicode))->GetCheck() )
	{
	}
	else if ( ((CButton*)GetDlgItem(IDC_RAnsi))->GetCheck() || 
		      ((CButton*)GetDlgItem(IDC_ROEM))->GetCheck() )
	{
		GetCurrCodePage(&nCurCodePage);
		m_SourceCodePage = nCurCodePage;
		ChangeCodepageToDesc(m_SourceCodePage, &pszCodePageDesc);
		wcscpy(m_CodePageDesc, pszCodePageDesc);
		free(pszCodePageDesc);
	}
	else if ( ((CButton*)GetDlgItem(IDC_ROther))->GetCheck())
	{
		nIdx = m_CodePage.GetCurSel();
		m_CodePage.GetLBText(nIdx, sBuffer);
		m_SourceCodePage = _ttoi(sBuffer.GetBuffer(0));

		m_CodePage.GetLBText(nIdx, sBuffer, 1);
		wcscpy(m_CodePageDesc, sBuffer.GetBuffer(0));
	}

	CDialog::OnOK();

}


/***************************************************************************

//	If Radion Button(Ansi Codepage) is Check, then Combo Box is Disabled

***************************************************************************/
void CSourceOption::OnRAnsi() 
{

	GetDlgItem(IDC_CodePage)->EnableWindow(FALSE);	

}


/***************************************************************************

//	If Radion Button(OEM Codepage) is Check, then Combo Box is Disabled

***************************************************************************/
void CSourceOption::OnRoem() 
{

	GetDlgItem(IDC_CodePage)->EnableWindow(FALSE);	

}


/***************************************************************************

//	If Radion Button(Other Codepage) is Check, then Combo Box is Enabled

***************************************************************************/
void CSourceOption::OnROther() 
{

	GetDlgItem(IDC_CodePage)->EnableWindow(TRUE);	

}

void CSourceOption::OnRUnicode() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_CodePage)->EnableWindow(FALSE);	
}

void CSourceOption::OnRutf8() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_CodePage)->EnableWindow(FALSE);		
}
