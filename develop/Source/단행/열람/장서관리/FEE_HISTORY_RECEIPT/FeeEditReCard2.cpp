// FeeEditReCard2.cpp : implementation file
//

#include "stdafx.h"
#include "FeeEditReCard2.h"
#include "..\\LOC_CommonAPI\\LocCommonAPI.h"
#include "..\..\..\..\관리\도서관관리\FileManager\LibFileManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeEditReCard2 dialog


CFeeEditReCard2::CFeeEditReCard2(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CFeeEditReCard2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeEditReCard2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bSuccess = FALSE;
	// KOL.UDF.022
	m_nIndex = 0;
	m_strEdtFee = _T("");
}
CFeeEditReCard2::~CFeeEditReCard2()
{
}

void CFeeEditReCard2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeEditReCard2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_EDT_RECARD2_PAYMENT, m_edtFee);
	DDX_Text(pDX, IDC_EDT_RECARD2_PAYMENT, m_strEdtFee);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeEditReCard2, CDialog)
	//{{AFX_MSG_MAP(CFeeEditReCard2)
	ON_EN_CHANGE(IDC_EDT_RECARD2_PAYMENT, OnChangeEdtReCard2Payment)
	ON_BN_CLICKED(IDC_BTN_RECARD2_EDIT, OnBtnReCard2Edit)
	ON_BN_CLICKED(IDC_BTN_RECARD2_CANCEL, OnBtnReCard2Cancel)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeEditReCard2 message handlers

BOOL CFeeEditReCard2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitControl();

	EnableThemeDialogTexture(GetSafeHwnd()); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFeeEditReCard2::OnChangeEdtReCard2Payment() 
{
	UpdateData(TRUE);

	m_strEdtFee.Replace( _T(","), _T("") );

	CString strBuf = m_strEdtFee;
	strBuf.Replace( _T(","), _T("") );

	INT nSize = (strBuf.GetLength() * 3) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 0 };

	::GetNumberFormat(NULL, NULL, strBuf, &nFmt, pszFormattedNumber, nSize-1);

	m_strEdtFee = pszFormattedNumber;

	if (pszFormattedNumber)
	delete [] pszFormattedNumber;

	UpdateData(FALSE);

	INT nLen = m_strEdtFee.GetLength();
	m_edtFee.SetSel(nLen, -1);
}

// **컨트롤의 초기화
VOID CFeeEditReCard2::InitControl()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_HISTORY") );
	if( !pDM ) 
	{
		AfxMessageBox(	_T("CFeeEditArrear::InitControl()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY"), MB_ICONSTOP );
		return;
	}

	CString sPayment;
	pDM->GetCellData( _T("납부금액"), m_nIndex, sPayment );
	sPayment.Replace( _T(","), _T("") );
	((CEdit*)GetDlgItem(IDC_EDT_RECARD2_PAYMENT))->SetWindowText( sPayment );
	((CEdit*)GetDlgItem( IDC_EDT_RECARD2_PAYMENT ))->SetFocus();
	//납부금액에 블럭잡기
	INT nLen = m_strEdtFee.GetLength();
	m_edtFee.SetSel(0, nLen);
}

// **수정
VOID CFeeEditReCard2::OnBtnReCard2Edit() 
{
	INT ids;
	m_bSuccess = FALSE;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_HISTORY") );
	if( !pDM ) 
	{
		AfxMessageBox(	_T("CFeeEditReCard::InitControl()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY"), MB_ICONSTOP );
		return;
	}

	// 자료수집
	CString sPaymentKey, sLoanKey, sPayment, sMoneyPaid, sEditor, sEditContent;
	pDM->GetCellData( _T("PAYMENT_KEY"), m_nIndex, sPaymentKey );
	pDM->GetCellData( _T("LOAN_INFO_KEY"), m_nIndex, sLoanKey );
	pDM->GetCellData( _T("납부금액"), m_nIndex, sPayment );
	((CEdit*)GetDlgItem(IDC_EDT_RECARD2_PAYMENT))->GetWindowText( sMoneyPaid );
	sEditor.Format( _T("%s::%s"), m_pInfo->USER_ID, m_pInfo->LOCAL_IP );
	sEditContent.Format( _T("납부금액:%s원->%s원")
						, sPayment, sMoneyPaid );

	sPayment.Replace( _T(","), _T("") );
	sMoneyPaid.Replace( _T(","), _T("") );

	CString sLastWork = GetWorkLogMsg( _T("회원증재발급수수료수정"), __WFILE__, __LINE__ );

	// 쿼리생성
	CString sQuery;
	sQuery.Format(	_T("UPDATE CO_FEE_PAYMENT_INFO_TBL ")
					_T("SET MONEY_PAID=%s, EDIT_DATE=SYSDATE, ")
					_T("EDITOR='%s', EDIT_CONTENT='%s', LAST_WORK='%s' ")
					_T("WHERE REC_KEY=%s;")
					, sMoneyPaid, sEditor, sEditContent, sLastWork, sPaymentKey );

	pDM->StartFrame();
	pDM->InitDBFieldData();	
	pDM->AddFrame(sQuery);
	ids = pDM->SendFrame(TRUE);	
	pDM->EndFrame();
	if( ids < 0 )
	{
		CString sErrorMsg;
		sErrorMsg = _T("SendFrame Error::회원증재발급수정");
		if( ids == -4004 )
			sErrorMsg = _T("SendFrame Error::회원증재발급수정\r\n-접속되지않음");
		else if( ids == -4005 )
			sErrorMsg = _T("SendFrame Error::회원증재발급수정\r\n-쿼리실행시 에러발생");
		else if( ids == -4999 )
			sErrorMsg = _T("SendFrame Error::회원증재발급수정\r\n-아직 Oracle 를 지원하지 않음");
		AfxMessageBox( sErrorMsg, MB_ICONSTOP );
		// UPDATE 실패
		m_bSuccess = FALSE;
		return;
	}

	// UPDATE 성공
	m_bSuccess = TRUE;
	OnCancel();
}

VOID CFeeEditReCard2::OnBtnReCard2Cancel() 
{
	OnCancel();	
}

BOOL CFeeEditReCard2::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			HWND MsgH = pMsg->hwnd;
			if( GetDlgItem(IDC_EDT_RECARD2_PAYMENT)->m_hWnd == MsgH )
			{
				GetDlgItem(IDC_BTN_RECARD2_EDIT)->SetFocus();
			}
			else if( GetDlgItem(IDC_BTN_RECARD2_EDIT)->m_hWnd == MsgH )
			{
				OnBtnReCard2Edit();
			}
			else if( GetDlgItem(IDC_BTN_RECARD2_CANCEL)->m_hWnd == MsgH )
			{
				OnCancel();
			}
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CFeeEditReCard2::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}