// FeeInsert.cpp : implementation file
//

#include "stdafx.h"
#include "FeeInsert.h"
#include "FeeInsertUserDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeInsert dialog


CFeeInsert::CFeeInsert(CESL_Mgr* pParent)
	: CESL_Mgr(CFeeInsert::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeInsert)
	m_strCtlMoneyPaid = _T("");
	//}}AFX_DATA_INIT
}
CFeeInsert::~CFeeInsert()
{
	
}


void CFeeInsert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeInsert)
	DDX_Control(pDX, IDC_edtInsertMoneyPaid, m_edtMoneyPaid);
	DDX_Text(pDX, IDC_edtInsertMoneyPaid, m_strCtlMoneyPaid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeInsert, CDialog)
	//{{AFX_MSG_MAP(CFeeInsert)
	ON_BN_CLICKED(IDC_btnInsertInput, OnbtnInsertInput)
	ON_BN_CLICKED(IDC_btnInsertCancel, OnbtnInsertCancel)
	ON_BN_CLICKED(IDC_btnInsertSelect, OnbtnInsertSelect)
	ON_EN_CHANGE(IDC_edtInsertMoneyPaid, OnChangeedtInsertMoneyPaid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeInsert message handlers

BOOL CFeeInsert::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// XP THEME
	EnableThemeDialogTexture(GetSafeHwnd()); 

	if( InitESL_Mgr( _T("FEE_INSERT") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : FEE_INSERT") );
		return FALSE;
	}

	// 콤보박스 초기화
	((CButton*)GetDlgItem(IDC_radInsertClass1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_radInsertClass2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_radInsertClass3))->SetCheck(FALSE);

	return TRUE;
}

void CFeeInsert::OnbtnInsertInput() 
{
	// 대출자선택확인
	if(m_strUserKey.IsEmpty())
	{
		AfxMessageBox(_T("대출자를 선택하십시오."));
		return;
	}

	// 납부금액 확인
	m_strMoneyPaid = m_strCtlMoneyPaid;
	m_strMoneyPaid.Replace( _T(","), _T("") );
	if(m_strMoneyPaid.IsEmpty() || _ttoi(m_strMoneyPaid) == 0) 
	{
		AfxMessageBox(_T("납부금액을 입력하십시오."));
		return;
	}
	m_strMoneyPaid = m_strCtlMoneyPaid;

	// 수수료 종류 확인
	if(1 == ((CButton*)GetDlgItem(IDC_radInsertClass1))->GetCheck())
	{
		m_strFeeClass = _T("0");
	}
	else if(1 == ((CButton*)GetDlgItem(IDC_radInsertClass2))->GetCheck())
	{
		m_strFeeClass = _T("1");
	}
	else if(1 == ((CButton*)GetDlgItem(IDC_radInsertClass3))->GetCheck())
	{
		m_strFeeClass = _T("2");
	}
	else
	{
		AfxMessageBox(_T("수수료 종류를 선택하십시오."));
		return;
	}

	// 납부일자 확인
	CDateTimeCtrl *pDate = (CDateTimeCtrl*)GetDlgItem( IDC_datInsertPaymentDate );

	SYSTEMTIME tTime;
	DWORD dwResult = pDate->GetTime( &tTime );
	if (dwResult != GDT_VALID ) 
	{
		ERROR_MSG_RETURN_VOID2( -1000, _T("OnbtnInsertInput") );
		return;
	}
	m_strPaymentDate.Format( _T("TO_DATE('%04d/%02d/%02d','YYYY/MM/DD')"), 
							 tTime.wYear, tTime.wMonth, tTime.wDay );

	// 다음
	OnOK();
}

void CFeeInsert::OnbtnInsertCancel() 
{
	OnCancel();
}

void CFeeInsert::OnbtnInsertSelect() 
{
	CFeeInsertUserDlg UserDlg(this);
	// 2010.09.08 ADD BY KSJ : 통합도서서비스 사용여부
	UserDlg.m_sIsUnityLoanService = m_sIsUnityLoanService;
	if( IDOK == UserDlg.DoModal() )
	{
		m_strUserKey = UserDlg.m_strUserKey;
		m_strUserName = UserDlg.m_strUserName;
		m_strUserNo = UserDlg.m_strUserNo;
		m_strUserNoDesc = UserDlg.m_strUserNoDesc;

		// 대출자번호 (설명을 저장:조건에 따라*표시 될수있으므로)
		((CEdit*)GetDlgItem( IDC_edtInsertUserNo ))->SetWindowText(m_strUserNoDesc);
		((CEdit*)GetDlgItem( IDC_edtInsertUserName ))->SetWindowText(m_strUserName);
		GetDlgItem(IDC_edtInsertMoneyPaid)->SetFocus();
	}

	return;
}

void CFeeInsert::OnChangeedtInsertMoneyPaid() 
{
	UpdateData(TRUE);

	m_strCtlMoneyPaid.Replace( _T(","), _T("") );

	CString strBuf = m_strCtlMoneyPaid;
	strBuf.Replace( _T(","), _T("") );

	INT nSize = (strBuf.GetLength() * 3) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 0 };

	::GetNumberFormat(NULL, NULL, strBuf, &nFmt, pszFormattedNumber, nSize-1);

	m_strCtlMoneyPaid = pszFormattedNumber;

	if (pszFormattedNumber)
		delete [] pszFormattedNumber;

	UpdateData(FALSE);

	INT nLen = m_strCtlMoneyPaid.GetLength();
	m_edtMoneyPaid.SetSel(nLen, -1);
}

BOOL CFeeInsert::PreTranslateMessage(MSG* pMsg) 
{
	if( VK_RETURN == pMsg->wParam )
	{
		if( WM_KEYDOWN == pMsg->message || WM_CHAR == pMsg->message )
		{
			// 납부금액
			if( GetDlgItem(IDC_edtInsertMoneyPaid)->GetSafeHwnd() == pMsg->hwnd)
			{
				// 다음진행
				OnbtnInsertInput();
			}
			// 납부일자
			else if( GetDlgItem(IDC_datInsertPaymentDate)->GetSafeHwnd() == pMsg->hwnd)
			{
				// 납부금액 입력으로 
				GetDlgItem(IDC_edtInsertMoneyPaid)->SetFocus();
			}
			// 대출자 선택
			else if( GetDlgItem(IDC_btnInsertSelect)->GetSafeHwnd() == pMsg->hwnd)
			{
				// 대출자선택진행
				OnbtnInsertSelect();
			}
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
