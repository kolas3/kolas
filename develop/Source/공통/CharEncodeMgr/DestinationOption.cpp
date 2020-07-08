// DestinationOption.cpp : implementation file
//

#include "stdafx.h"
#include "DestinationOption.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDestinationOption dialog


CDestinationOption::CDestinationOption(INT	IsUnicode, CWnd* pParent /*=NULL*/)
	: CharConvert(CDestinationOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDestinationOption)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_IsUnicode = IsUnicode;
}


CDestinationOption::~CDestinationOption()
{

}

void CDestinationOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDestinationOption)
	DDX_Control(pDX, IDC_CodePage, m_CodePage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDestinationOption, CDialog)
	//{{AFX_MSG_MAP(CDestinationOption)
	ON_BN_CLICKED(IDC_ROther, OnROther)
	ON_BN_CLICKED(IDC_ROEM, OnRoem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDestinationOption message handlers

BOOL CDestinationOption::OnInitDialog() 
{

	CDialog::OnInitDialog();


	CString				sBuffer, sComboData;
	_TCHAR				*pszCodePageDesc;
	INT					nCurCodePage, nIdx, ids;

	
	// Get Current Code page Initialize
	GetCurrCodePage	(&nCurCodePage);

	// If Source is Unicode, then Destination cannot set Unicode. So Unicode Radio Button was Disable
	if ( m_IsUnicode == ISUNICODE || m_IsUnicode == ISUTF8 )
	{
		nIdx = 0;				
		// Get All Code Pages
		GetAllCodePage();
		m_CodePage.FormatList( 2 , DT_LEFT , FALSE , EDGE , 0 );

		// Insert Combo Box
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
		m_CodePage.SetCurSel(20);


		if ( nCurCodePage == m_DestinationCodePage )
		{
			((CButton*)GetDlgItem(IDC_RUTF8 ))->SetCheck(1);
			//((CButton*)GetDlgItem(IDC_RAnsi ))->SetCheck(1);
			GetDlgItem(IDC_CodePage)->EnableWindow(FALSE);	
		}
		else if ( m_DestinationCodePage == ISUNICODE )
			((CButton*)GetDlgItem(IDC_RUnicode))->SetCheck(1);
		else if ( m_DestinationCodePage == ISUTF8 )
			((CButton*)GetDlgItem(IDC_RUTF8))->SetCheck(1);
		else
		{
			sBuffer.Format(_T("%d"), m_DestinationCodePage );
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
/*
		if ( m_IsUnicode == ISUNICODE )			
			GetDlgItem(IDC_RUnicode)->EnableWindow(FALSE);
		else
			GetDlgItem(IDC_RUTF8)->EnableWindow(FALSE);
			*/
	}	
	// If Source is MultiByteCode, then Destination only can set Unicode. 
	else
	{
		if ( m_DestinationCodePage == ISUNICODE )
			((CButton*)GetDlgItem(IDC_RUnicode))->SetCheck(1);
		else if ( m_DestinationCodePage == ISUTF8 )
			((CButton*)GetDlgItem(IDC_RUTF8))->SetCheck(1);
		else 
			((CButton*)GetDlgItem(IDC_RUnicode))->SetCheck(1);

	//	GetDlgItem(IDC_RAnsi   )->EnableWindow(FALSE);
		GetDlgItem(IDC_ROEM    )->EnableWindow(FALSE);
		GetDlgItem(IDC_ROther  )->EnableWindow(FALSE);
		GetDlgItem(IDC_CodePage)->EnableWindow(FALSE);
		GetDlgItem(IDC_CodePage)->EnableWindow(FALSE);
	}

	FreeCodePages(m_pszCodePage, m_iCodePageCnt);



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/***************************************************************************

//	Get Destination CodePage
//  If Unicode, Then No Change

***************************************************************************/
void CDestinationOption::OnOK() 
{

	CString		sBuffer;
	_TCHAR		*pszCodePageDesc;
	INT			nIdx;
	INT			nCurCodePage;
	

	if ( ((CButton*)GetDlgItem(IDC_RUnicode))->GetCheck() )
	{
		_tcscpy(m_CodePageDesc, _T("유니코드(UTF-16)"));
		m_DestinationCodePage = ISUNICODE;
	}
	else if ( ((CButton*)GetDlgItem(IDC_RUTF8))->GetCheck() )
	{
		_tcscpy(m_CodePageDesc, _T("유니코드(UTF-8)"));
		m_DestinationCodePage = ISUTF8;
	}

	/*
	else if ( ((CButton*)GetDlgItem(IDC_RAnsi))->GetCheck() || 
		      ((CButton*)GetDlgItem(IDC_ROEM))->GetCheck() )
	{
		GetCurrCodePage(&nCurCodePage);
		m_DestinationCodePage = nCurCodePage;
		ChangeCodepageToDesc(m_DestinationCodePage, &pszCodePageDesc);
		_stprintf(m_CodePageDesc, _T("코드페이지 : %s"), pszCodePageDesc);
		free(pszCodePageDesc);
	}
	*/
	else if ( ((CButton*)GetDlgItem(IDC_ROther))->GetCheck())
	{
		nIdx = m_CodePage.GetCurSel();
		m_CodePage.GetLBText(nIdx, sBuffer);
		m_DestinationCodePage = _ttoi(sBuffer.GetBuffer(0));

		m_CodePage.GetLBText(nIdx, sBuffer, 1);
		_stprintf(m_CodePageDesc, _T("코드페이지 : %s"), sBuffer.GetBuffer(0));
	}

	CDialog::OnOK();

}

/***************************************************************************

//	If Radion Button(Other Codepage) is Check, then Combo Box is Enabled

***************************************************************************/
void CDestinationOption::OnROther() 
{

	GetDlgItem(IDC_CodePage)->EnableWindow(TRUE);

}

/***************************************************************************

//	If Radion Button(Ansi Codepage) is Check, then Combo Box is Disabled

***************************************************************************/
void CDestinationOption::OnRAnsi() 
{

	GetDlgItem(IDC_CodePage)->EnableWindow(FALSE);	

}

/***************************************************************************

//	If Radion Button(OEM Codepage) is Check, then Combo Box is Disabled

***************************************************************************/
void CDestinationOption::OnRoem() 
{

	GetDlgItem(IDC_CodePage)->EnableWindow(FALSE);	

}

int CDestinationOption::GetDestinationCodePage()
{

	return m_DestinationCodePage;

}

void CDestinationOption::OnCancel() 
{
	CDialog::OnCancel();
}
