#include "stdafx.h"
#include "FeeReCard.h"
#include "..\\BL_LOC_LOAN\\BO_LOC_RECEIPT_PRINT_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFeeReCard::CFeeReCard(CESL_Mgr* pParent /*=NULL*/, CLonaManageValue* pManageValue /*NULL*/)
	: CESL_Mgr(CFeeReCard::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeReCard)
	m_pManageValue = pManageValue;
	//}}AFX_DATA_INIT
}
CFeeReCard::~CFeeReCard()
{
	if(m_pManageValue) m_pManageValue = NULL;
}

void CFeeReCard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeReCard)
	DDX_Control(pDX, IDC_edtMONEY_PAID, m_edtFee);
	DDX_Text(pDX, IDC_edtMONEY_PAID, m_strEdtFee);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeReCard, CDialog)
	//{{AFX_MSG_MAP(CFeeReCard)
	ON_BN_CLICKED(IDC_radCLASS1, OnradCLASS1)
	ON_BN_CLICKED(IDC_radCLASS2, OnradCLASS2)
	ON_BN_CLICKED(IDC_radCLASS3, OnradCLASS3)
	ON_BN_CLICKED(IDC_radCARD1, OnradCARD1)
	ON_BN_CLICKED(IDC_radCARD2, OnradCARD2)
	ON_BN_CLICKED(IDC_radCARD3, OnradCARD3)
	ON_EN_CHANGE(IDC_edtMONEY_PAID, OnChangeedtFEE)
	ON_BN_CLICKED(IDC_btnPRINT_RECEIPT1, OnbtnPRINTRECEIPT)
	ON_BN_CLICKED(IDC_btnPREVIEW1, OnbtnPREVIEW)
	ON_BN_CLICKED(IDC_btnRECARD_CLOSE1, OnbtnRECARDCLOSE)
	ON_BN_CLICKED(IDC_BTN_PRINT_CANCEL1, OnbtnPRINTCANCEL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeReCard message handlers

BOOL CFeeReCard::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CFeeReCard::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_ESCAPE )
		{
			return TRUE;
		}		
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CFeeReCard::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(InitESL_Mgr( _T("FEE_RECARD_RECEIPT") ) < 0){
		AfxMessageBox( _T("InitESL_Mgr Error!") );
		return FALSE;
	}

	((CButton*)GetDlgItem(IDC_radCLASS1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_radCARD1))->SetCheck(TRUE);
	((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("0") );
	OnradCLASS1();
	OnradCARD1();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CFeeReCard::SetUserKey( CString sUserKey, CString strUserNo, CString strName )
{
	m_sRecKey = sUserKey;
	m_sUserNo = strUserNo;
	m_sName = strName;

	return 0;
}

void CFeeReCard::OnradCLASS1() 
{
	m_sClass = _T("성인");

	((CButton*)GetDlgItem(IDC_radCLASS1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_radCLASS2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_radCLASS3))->SetCheck(FALSE);
	
	if( ( ((CButton*)GetDlgItem(IDC_radCARD1))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("0") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_radCARD2))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("1000") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_radCARD3))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("2000") );
	}
}

void CFeeReCard::OnradCLASS2() 
{
	m_sClass = _T("중,고등학생");

	((CButton*)GetDlgItem(IDC_radCLASS1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_radCLASS2))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_radCLASS3))->SetCheck(FALSE);

	if( ( ((CButton*)GetDlgItem(IDC_radCARD1))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("0") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_radCARD2))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("800") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_radCARD3))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("1500") );
	}
}

void CFeeReCard::OnradCLASS3() 
{
	m_sClass = _T("초등학생이하");

	((CButton*)GetDlgItem(IDC_radCLASS1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_radCLASS2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_radCLASS3))->SetCheck(TRUE);

	if( ( ((CButton*)GetDlgItem(IDC_radCARD1))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("0") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_radCARD2))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("500") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_radCARD3))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("1000") );
	}
}

void CFeeReCard::OnradCARD1() 
{
	m_sCard = _T("일반카드형");

	((CButton*)GetDlgItem(IDC_radCARD1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_radCARD2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_radCARD3))->SetCheck(FALSE);

	if( ( ((CButton*)GetDlgItem(IDC_radCLASS1))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("0") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_radCLASS2))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("0") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_radCLASS3))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("0") );
	}
}

void CFeeReCard::OnradCARD2() 
{
	m_sCard = _T("플라스틱카드형");
	((CButton*)GetDlgItem(IDC_radCARD1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_radCARD2))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_radCARD3))->SetCheck(FALSE);

	if( ( ((CButton*)GetDlgItem(IDC_radCLASS1))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("1000") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_radCLASS2))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("800") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_radCLASS3))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("500") );
	}
}

void CFeeReCard::OnradCARD3() 
{
	m_sCard = _T("RF카드형");
	((CButton*)GetDlgItem(IDC_radCARD1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_radCARD2))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_radCARD3))->SetCheck(TRUE);

	if( ( ((CButton*)GetDlgItem(IDC_radCLASS1))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("2000") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_radCLASS2))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("1500") );
	}
	else if( ( ((CButton*)GetDlgItem(IDC_radCLASS3))->GetCheck() ) == TRUE )
	{
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->SetWindowText( _T("1000") );
	}
}

void CFeeReCard::OnChangeedtFEE() 
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
	
	CString sPayment;
	((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->GetWindowText(sPayment);
	if( sPayment == _T("0") )
	{
		((CEdit*)GetDlgItem(IDC_btnPREVIEW1))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_BTN_PRINT_CANCEL1))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_btnPRINT_RECEIPT1))->SetWindowText( _T("신청(&P)") );
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_btnPREVIEW1))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_BTN_PRINT_CANCEL1))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_btnPRINT_RECEIPT1))->SetWindowText( _T("출력(&P)") );
	}

}

void CFeeReCard::OnbtnPRINTRECEIPT() 
{
	int ids = OnReCardReceipt(FALSE);
	if( ids != 100 ) OnCancel();	
}

void CFeeReCard::OnbtnPREVIEW() 
{
	int ids = OnReCardReceipt(TRUE);
	if( ids != 100 ) OnCancel();
}

void CFeeReCard::OnbtnRECARDCLOSE() 
{
	OnCancel();
}

INT CFeeReCard::OnReCardReceipt(BOOL bShow)
{
	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_RECARD_RECEIPT") );
	CString sQuery, sNumber, sRecKey, sUserNo, sName, sClass, sCard, sPayment;
	CString sInsertRecKey, sSysDate;
	INT ids;
	sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='RCT_NO' ")
			 _T("AND MANAGE_CODE=UDF_MANAGE_CODE");
	ids = pDM->GetOneValueQuery( sQuery, sNumber );
	sQuery.Format(  _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d ")
					_T("WHERE KIND_CODE='RCT_NO' AND MANAGE_CODE=UDF_MANAGE_CODE;"), _ttoi(sNumber)+1 );		
	pDM->StartFrame();
	pDM->InitDBFieldData();	
	pDM->AddFrame(sQuery);

	sNumber.Format( _T("%d"), _ttoi(sNumber)+1);
	sRecKey = m_sRecKey;
	sUserNo = m_sUserNo;
	sName = m_sName;
	sClass = m_sClass;
	sCard = m_sCard;
	((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->GetWindowText(sPayment);

	pDM->InsertRow(-1);
	pDM->SetCellData( _T("일련번호")	, sNumber	, 0);
	pDM->SetCellData( _T("회원번호")	, sUserNo	, 0);
	pDM->SetCellData( _T("성명")		, sName		, 0);
	pDM->SetCellData( _T("회원등급")	, sClass	, 0);
	pDM->SetCellData( _T("카드종류")	, sCard		, 0);
	pDM->SetCellData( _T("납부금액")	, sPayment	, 0);
	pDM->SetCellData( _T("REC_KEY")		, sRecKey	, 0);

	CBO_LOC_RECEIPT_PRINT_DLG* ReceiptDlg;
	ReceiptDlg = NULL;
	
	ReceiptDlg = new CBO_LOC_RECEIPT_PRINT_DLG( 3, this, m_pManageValue );	
	ReceiptDlg->pMain = this->pMain;
	ReceiptDlg->pParentMgr = (CESL_Mgr*)this;
	ReceiptDlg->m_pInfo = this->m_pInfo;
	
	INT nPrintStatus=0;

	if( sPayment == _T("0") )
	{
		nPrintStatus = ReceiptDlg->PrintFeeReCardInfo(bShow, FALSE);
	}
	else
	{
		nPrintStatus = ReceiptDlg->PrintFeeReCardInfo(bShow, TRUE);
	}


	if ( nPrintStatus )
	{
		if( nPrintStatus == 100)
		{
			if( &ReceiptDlg != NULL )
				delete ReceiptDlg;
			ReceiptDlg = NULL;

			return 100;
		}
		else
		{
			if ( AfxMessageBox( _T("출력실패! 재시도 하시게습니까?"), MB_YESNO ) == IDYES )
			{
				OnReCardReceipt(FALSE);

				if( &ReceiptDlg != NULL )
					delete ReceiptDlg;
				ReceiptDlg = NULL;
				
				return 100;
			}
			else
			{
				if( &ReceiptDlg != NULL )
					delete ReceiptDlg;
				ReceiptDlg = NULL;
				
				return 0;
			}
		}
	}
	sPayment.Replace( _T(","), _T("") );

	sQuery.Format( _T("SELECT ESL_SEQ.NEXTVAL FROM DUAL") );
	pDM->GetOneValueQuery(sQuery, sInsertRecKey);

	sSysDate = _T("SYSDATE");				
	
	CString sUserID = m_pInfo->USER_ID;
	CString sFirstWork = GetWorkLogMsg( _T("회원증재발급(출력)"), __WFILE__, __LINE__ );

	if( sPayment == _T("0") )
	{
		sQuery.Format(	_T("INSERT INTO CO_FEE_PAYMENT_INFO_TBL(REC_KEY, CLASS_CODE, LOAN_INFO_KEY, ")
						_T("PAYMENT_DATE, MONEY_PAID, STATUS, CREATE_DATE, RECEIPT_KEY, RECEIPT_PRINT_YN, CARD_CLASS, USER_ID, MANAGE_CODE, FIRST_WORK ) ")
						_T("VALUES( ESL_SEQ.NEXTVAL, '1', '%s', %s, '%s', 'Y', %s, %s, 'N', '%s', '%s', UDF_MANAGE_CODE, '%s' );")
						, sRecKey, sSysDate, sPayment, sSysDate, sInsertRecKey, sCard, sUserID, sFirstWork );
		pDM->AddFrame( sQuery );
	}
	else
	{
		sQuery.Format(	_T("INSERT INTO CO_FEE_PAYMENT_INFO_TBL(REC_KEY, CLASS_CODE, LOAN_INFO_KEY, PAYMENT_DATE, ")
						_T("MONEY_PAID, STATUS, CREATE_DATE, RECEIPT_KEY, RECEIPT_PRINT_YN, CARD_CLASS, USER_ID, MANAGE_CODE, FIRST_WORK ) ")
						_T("VALUES( ESL_SEQ.NEXTVAL, '1', '%s', %s, '%s', 'Y', %s, %s, 'Y', '%s', '%s', UDF_MANAGE_CODE, '%s' );")
						,sRecKey, sSysDate, sPayment, sSysDate, sInsertRecKey, sCard, sUserID, sFirstWork );
		pDM->AddFrame( sQuery );
	}
	
	sQuery.Format(	_T("INSERT INTO CO_FEE_RECEIPT_TBL ( REC_KEY, PRINT_COUNT, PRINT_DATE, RECEIPT_XML, FIRST_WORK, MANAGE_CODE )")
					_T("VALUES ( '%s', '1', SYSDATE, '%s', '회원증재발급수수료 납부_'||SYSDATE, UDF_MANAGE_CODE);")
					,sInsertRecKey, ReceiptDlg->m_strRCT);
	pDM->AddFrame( sQuery );
	pDM->SendFrame(TRUE);	
	pDM->EndFrame();
	
	pDM->FreeData();

	AfxMessageBox( _T("회원증카드 재발급 신청되었습니다.") );

	if( &ReceiptDlg != NULL )
		delete ReceiptDlg;
	ReceiptDlg = NULL;
	
	return 0;
}

void CFeeReCard::OnbtnPRINTCANCEL() 
{
	if ( AfxMessageBox( _T("수수료를 받으셨습니까?\n(영수증 출력을 생략하고 재발급 신청됩니다.)\n")
						_T("취소된 자료는 수수료관리에서 재출력할 수 있습니다."), MB_YESNO) == IDYES )
	{
		CESL_DataMgr *pDM = FindDM( _T("DM_FEE_RECARD_RECEIPT") );
		CString sQuery, sNumber, sRecKey, sUserNo, sName, sClass, sCard, sPayment;
		CString sInsertRecKey, sSysDate;
		INT ids;
		sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
				 _T("WHERE KIND_CODE='RCT_NO' AND MANAGE_CODE=UDF_MANAGE_CODE");
		ids = pDM->GetOneValueQuery( sQuery, sNumber );
		sQuery.Format(	_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d ")
						_T("WHERE KIND_CODE='RCT_NO' AND MANAGE_CODE=UDF_MANAGE_CODE;"), _ttoi(sNumber)+1 );		
		pDM->StartFrame();
		pDM->InitDBFieldData();	
		pDM->AddFrame(sQuery);

		sNumber.Format( _T("%d"), _ttoi(sNumber)+1 );
		sRecKey = m_sRecKey;
		sUserNo = m_sUserNo;
		sName = m_sName;
		sClass = m_sClass;
		sCard = m_sCard;
		((CEdit*)GetDlgItem(IDC_edtMONEY_PAID))->GetWindowText(sPayment);

		pDM->InsertRow(-1);
		pDM->SetCellData( _T("일련번호")	, sNumber	, 0);
		pDM->SetCellData( _T("회원번호")	, sUserNo	, 0);
		pDM->SetCellData( _T("성명")		, sName		, 0);
		pDM->SetCellData( _T("회원등급")	, sClass	, 0);
		pDM->SetCellData( _T("카드종류")	, sCard		, 0);
		pDM->SetCellData( _T("납부금액")	, sPayment	, 0);
		pDM->SetCellData( _T("REC_KEY")		, sRecKey	, 0);
	
		CBO_LOC_RECEIPT_PRINT_DLG* ReceiptDlg;
		ReceiptDlg = NULL;
		
		ReceiptDlg = new CBO_LOC_RECEIPT_PRINT_DLG( 3, this, m_pManageValue );
		ReceiptDlg->pMain = this->pMain;
		ReceiptDlg->pParentMgr = (CESL_Mgr*)this;
		ReceiptDlg->m_pInfo = this->m_pInfo;
		
		INT nPrintStatus=0;

		nPrintStatus = ReceiptDlg->PrintFeeReCardInfo(FALSE, FALSE);

		if ( nPrintStatus )
		{
			AfxMessageBox( _T("XML생성에 실패 하였습니다.") );

			if( &ReceiptDlg != NULL )
				delete ReceiptDlg;
			ReceiptDlg = NULL;

			return;
		}

		sQuery.Format( _T("SELECT ESL_SEQ.NEXTVAL FROM DUAL") );
		pDM->GetOneValueQuery(sQuery, sInsertRecKey);
		
		sPayment.Replace( _T(","), _T("") );

		sSysDate = _T("SYSDATE");			
		CString sFirstWork = GetWorkLogMsg( _T("회원증재발급(출력취소)"), __WFILE__, __LINE__ );
		
		CString sUserID = m_pInfo->USER_ID;
		sQuery.Format(	_T("INSERT INTO CO_FEE_PAYMENT_INFO_TBL(REC_KEY, CLASS_CODE, LOAN_INFO_KEY, PAYMENT_DATE, ")
						_T("MONEY_PAID, STATUS, CREATE_DATE, RECEIPT_KEY, RECEIPT_PRINT_YN, CARD_CLASS, USER_ID, MANAGE_CODE, FIRST_WORK ) ")
						_T("VALUES( ESL_SEQ.NEXTVAL, '1', '%s', %s, '%s', 'Y', %s, %s, 'N', '%s', '%s', UDF_MANAGE_CODE, '%s' );")
						, sRecKey, sSysDate, sPayment, sSysDate, sInsertRecKey, sCard, sUserID, sFirstWork );
		pDM->AddFrame( sQuery );
		
		sQuery.Format(	_T("INSERT INTO CO_FEE_RECEIPT_TBL ( REC_KEY, PRINT_COUNT, PRINT_DATE, RECEIPT_XML, FIRST_WORK, MANAGE_CODE )")
						_T("VALUES ( '%s', '0', SYSDATE, '%s', '회원증재발급수수료 납부_'||SYSDATE, UDF_MANAGE_CODE);")
						, sInsertRecKey, ReceiptDlg->m_strRCT );
		pDM->AddFrame( sQuery );
		pDM->SendFrame(TRUE);	
		pDM->EndFrame();
		
		pDM->FreeData();

		AfxMessageBox( _T("회원증카드 재발급 신청되었습니다.") );

		if( &ReceiptDlg != NULL )
			delete ReceiptDlg;
		ReceiptDlg = NULL;
		
		OnCancel();
	}
}

HBRUSH CFeeReCard::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
