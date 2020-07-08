#include "stdafx.h"
#include "LoanStopDateSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CLoanStopDateSet::CLoanStopDateSet(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CLoanStopDateSet::IDD, pParent)
{
	
		
	
}


void CLoanStopDateSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanStopDateSet) 
	//}}AFX_DATA_MAP	
}


BEGIN_MESSAGE_MAP(CLoanStopDateSet, CDialog)
	//{{AFX_MSG_MAP(CLoanStopDateSet)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_radPlus, OnradPlus)
	ON_BN_CLICKED(IDC_radMinus, OnradMinus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CLoanStopDateSet::OnOK() 
{
	CString strSetDate;
	CString strCurrentNextYearDate;
	CString strLoanStopSetDate;
	INT nYear;
	INT nMonth;
	INT nDay;
	INT nFirstPos;
	INT nLastPos;

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtSetDate);	
	pEdit->GetWindowText(strSetDate);
	if( strSetDate.IsEmpty() )
	{
		AfxMessageBox(_T("변경일을 설정해 주세요!"));
		pEdit->SetFocus();
		return;
	}
	else if( _ttoi(strSetDate) > 365 )
	{
		AfxMessageBox(_T("변경일은 365일 이하로 설정해 주세요!"));
		pEdit->SetWindowText(_T(""));
		pEdit->SetFocus();
		return;
	}

	
	CTime tCurrentTime = CTime::GetCurrentTime();
	nYear	= tCurrentTime.GetYear();
	nMonth	= tCurrentTime.GetMonth();
	nDay	= tCurrentTime.GetDay();

	CTimeSpan tSpan1( 365 , 0 , 0 , 0 );

	tCurrentTime += tSpan1;

	strCurrentNextYearDate = tCurrentTime.Format( _T("%Y/%m/%d") );

	
	if(m_strLoanStopDate.IsEmpty())
	{
 		strLoanStopSetDate.Format(_T("%d/%d/%d"), nYear, nMonth, nDay);
	}
	else
	{
		strLoanStopSetDate = m_strLoanStopDate;
	}
	
	nFirstPos = strLoanStopSetDate.Find(_T("/"), 0);
	nLastPos = strLoanStopSetDate.Find(_T("/"), nFirstPos+1);
	
	nYear	= _ttoi( strLoanStopSetDate.Left(nFirstPos) );
	nMonth	= _ttoi( strLoanStopSetDate.Mid(nFirstPos+1,nLastPos-nFirstPos-1) );
	nDay	= _ttoi( strLoanStopSetDate.Mid(nLastPos+1) );

	CTime t( nYear, nMonth, nDay,0,0,0 );

	CTimeSpan tSpan2( _ttoi(strSetDate) , 0 , 0 , 0 );
	if( ( ((CButton*)GetDlgItem(IDC_radPlus))->GetCheck() ) == TRUE )
	{
		t += tSpan2;
	}
	else
	{
		t -= tSpan2;
	}
	
	strLoanStopSetDate = t.Format( _T("%Y/%m/%d") );

	
	CString strDate;
	CString strNextDate;

	nFirstPos = strLoanStopSetDate.Find(_T("/"), 0);
	nLastPos = strLoanStopSetDate.Find(_T("/"), nFirstPos+1);

	nYear		= _ttoi( strLoanStopSetDate.Left(nFirstPos) );
	nMonth		= _ttoi( strLoanStopSetDate.Mid(nFirstPos+1,nLastPos-nFirstPos-1) );
	nDay		= _ttoi( strLoanStopSetDate.Mid(nLastPos+1) );
	strDate.Format(_T("%04d%02d%02d"), nYear, nMonth, nDay);
	
	nFirstPos = strCurrentNextYearDate.Find(_T("/"), 0);
	nLastPos = strCurrentNextYearDate.Find(_T("/"), nFirstPos+1);

	nYear	= _ttoi( strCurrentNextYearDate.Left(nFirstPos) );
	nMonth	= _ttoi( strCurrentNextYearDate.Mid(nFirstPos+1,nLastPos-nFirstPos-1) );
	nDay	= _ttoi( strCurrentNextYearDate.Mid(nLastPos+1) );
	strNextDate.Format(_T("%04d%02d%02d"), nYear, nMonth, nDay);
	if( _ttoi(strDate) <= _ttoi(strNextDate) )
	{
		m_strLoanStopDate = strLoanStopSetDate;
	}
	else
	{
		AfxMessageBox(_T("오늘로 부터 365일 이후 날짜로 설정합니다."));
 		m_strLoanStopDate = strCurrentNextYearDate;	
	}

	CDialog::OnOK();
}


void CLoanStopDateSet::OnCancel() 
{
	
	
	CDialog::OnCancel();
}

BOOL CLoanStopDateSet::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnOK();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CLoanStopDateSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	OnradPlus() ;

	EnableThemeDialogTexture(GetSafeHwnd()); 	

	return TRUE;
}

HBRUSH CLoanStopDateSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}

void CLoanStopDateSet::OnradPlus() 
{
	((CButton*)GetDlgItem(IDC_radPlus))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_radMinus))->SetCheck(FALSE);	
}

void CLoanStopDateSet::OnradMinus() 
{
	((CButton*)GetDlgItem(IDC_radPlus))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_radMinus))->SetCheck(TRUE);	
}
