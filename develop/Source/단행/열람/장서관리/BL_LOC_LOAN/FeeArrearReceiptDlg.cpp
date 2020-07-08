// FeeArrearReceiptDlg.cpp : implementation file
// MAKE BY PWR

#include "stdafx.h"
#include "FeeArrearReceiptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeArrearReceiptDlg dialog


CFeeArrearReceiptDlg::CFeeArrearReceiptDlg(CESL_Mgr* pParent, CLonaManageValue *pLMV)
	: CESL_Mgr(CFeeArrearReceiptDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeArrearReceiptDlg)
	m_strEdtMoneyPaid = _T("");
	//}}AFX_DATA_INIT
	m_ReceiptDlg = NULL;

	m_pLSM = pLMV;
}

CFeeArrearReceiptDlg::~CFeeArrearReceiptDlg()
{
	if( m_ReceiptDlg )
		delete m_ReceiptDlg;
	m_ReceiptDlg = NULL;

}


VOID CFeeArrearReceiptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeArrearReceiptDlg)
	DDX_Control(pDX, IDC_edtMONEYPAID, m_edtMoneyPaid);
	DDX_Text(pDX, IDC_edtMONEYPAID, m_strEdtMoneyPaid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeArrearReceiptDlg, CDialog)
	//{{AFX_MSG_MAP(CFeeArrearReceiptDlg)
	ON_BN_CLICKED(IDC_btnFEE_RECEIPT_PRINT, OnbtnPRINTRECEIPT)
	ON_BN_CLICKED(IDC_btnLocloanCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnFEE_PREVIEW, OnbtnPREVIEW)
	ON_EN_CHANGE(IDC_edtMONEYPAID, OnChangeedtMONEYPAID)
	ON_BN_CLICKED(IDC_btnPRINT_CANCEL, OnbtnPRINTCANCEL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeArrearReceiptDlg message handlers

BOOL CFeeArrearReceiptDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr( _T("FEE_ARREAR_RECEIPT") ) < 0){
		AfxMessageBox( _T("InitESL_Mgr Error!::FEE_ARREAR_RECEIPT") );
		return FALSE;
	}

	m_pUserDM = FindDM( _T("DM_FEE_ARREAR_RECEIPT_USER") );
	m_pReceiptBookDM  = FindDM( _T("DM_FEE_ARREAR_RECEIPT_LOAN") );

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;
}

BOOL CFeeArrearReceiptDlg::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CFeeArrearReceiptDlg::Clear()
{
	m_pUserDM->FreeData();
	m_pReceiptBookDM->FreeData();
	AllControlDisplay( _T("CM_FEE_ARREAR_RECEIPT"), -1 );	
	((CEdit*)GetDlgItem(IDC_edtMONEYPAID))->SetWindowText( _T("") );
	((CEdit*)GetDlgItem(IDC_edtBOOK))->SetWindowText( _T("") );
}

INT CFeeArrearReceiptDlg::ReLoad()
{
	INT nCnt = 0;
	nCnt = m_pUserDM->GetRowCount();

	if( nCnt < 1 )
	{
		this->ShowWindow(FALSE);
		return 0;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_FEE_ARREAR_RECEIPT") , _T("UserGrid") );
	INT nIndex = m_pUserDM->GetRowCount()-1;	
	ShowReceiptList(nIndex);
	pGrid->SetReverse(nIndex);
	return 0;
}

INT CFeeArrearReceiptDlg::AddUser(CString strRecKey, CString strUserNo)
{	
	CString strWhere;
	INT	nIndex;

	INT nCnt = m_pUserDM->GetRowCount();
	CString sGetData;
	BOOL bOverlap = FALSE;
	for(int i = 0; i < nCnt; i++)
	{
		m_pUserDM->GetCellData( _T("대출자KEY"), i, sGetData );
		if( sGetData == strRecKey )
		{
			bOverlap = TRUE;
			nIndex = i;
			break;
		}
	}

	CESL_DataMgr *pTempDM = FindDM( _T("DM_FEE_ARREAR_RECEIPT_USER_TEMP") );
	
	strWhere.Format( _T("CLU.REC_KEY = '%s'"), strRecKey );
	pTempDM->RefreshDataManager(strWhere);	
	
	CString strUserKey;
	CString strUserName;
	CString strDelayDay;
	CString strDelayMoney;
	CString strPaymentDate;
	CString strToTalDelayDay;
	CString strLoanStopDate;
	
	if ( pTempDM->GetRowCount() > 0 )
	{	
		pTempDM->GetCellData( _T("성명")		, 0, strUserName );
		pTempDM->GetCellData( _T("적용연체일")	, 0, strDelayDay );		
		pTempDM->GetCellData( _T("누적납부금")	, 0, strDelayMoney );
		pTempDM->GetCellData( _T("납부일")		, 0, strPaymentDate );
		pTempDM->GetCellData( _T("총연체일")	, 0, strToTalDelayDay );
		pTempDM->GetCellData( _T("대출정지일")	, 0, strLoanStopDate );
		pTempDM->GetCellData( _T("대출자KEY")	, 0, strUserKey );
		
		if(!bOverlap)
		{			
			m_pUserDM->InsertRow(-1);
			nIndex = m_pUserDM->GetRowCount() -1;	
		}
		
		INT ids;
		CString strValue;
		ids = GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("AddUser") );

		strValue.TrimLeft();		strValue.TrimRight();
		strValue.MakeUpper();
		if( _T("Y") != strValue )
		{
			CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_pLSM->m_sIsUnityLoanService );
		}
		
		m_pUserDM->SetCellData( _T("대출자번호"), strUserNo			, nIndex );
		m_pUserDM->SetCellData( _T("성명")		, strUserName		, nIndex );
		m_pUserDM->SetCellData( _T("적용연체일"), strDelayDay		, nIndex );		
		m_pUserDM->SetCellData( _T("누적납부금"), strDelayMoney		, nIndex );
		m_pUserDM->SetCellData( _T("납부일")	, strPaymentDate	, nIndex );
		m_pUserDM->SetCellData( _T("총연체일")	, strToTalDelayDay	, nIndex );
		m_pUserDM->SetCellData( _T("대출정지일"), strLoanStopDate	, nIndex );
		m_pUserDM->SetCellData( _T("대출자KEY")	, strUserKey		, nIndex );
	}
	
	ShowReceiptList(nIndex);

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_FEE_ARREAR_RECEIPT") , _T("UserGrid") );
	pGrid->SetReverse( nIndex );

	return 0;
}

INT CFeeArrearReceiptDlg::ShowReceiptList(INT nIndex)
{
	CString strWhere;
	CString strUserKey;
	INT ids;
	m_nIndex = nIndex;

	m_pUserDM->GetCellData( _T("대출자KEY"), nIndex, strUserKey );	
	strWhere.Format( _T("CLU.REC_KEY = '%s'"), strUserKey );
	m_pReceiptBookDM->RefreshDataManager(strWhere);

	CString sBasicArrear;
	m_pLSM->GetManageValue(_T("열람"), _T("공통"), _T("연체관리"), _T("연체료"), sBasicArrear);
	m_pReceiptBookDM->SetCellData(_T("기본연체료"), sBasicArrear, 0);

	m_sTotalBook.Format( _T("%d"), m_pReceiptBookDM->GetRowCount() );
	((CEdit*)GetDlgItem(IDC_edtBOOK))->SetWindowText( m_sTotalBook );

	m_pUserDM->GetCellData( _T("누적납부금"), nIndex, m_sTotalMoney );
	((CEdit*)GetDlgItem(IDC_edtMONEYPAID))->SetWindowText( m_sTotalMoney );

	ids = AllControlDisplay( _T("CM_FEE_ARREAR_RECEIPT"), -1 );
	if ( ids < 0 ) return ids;

	return 0;
}
INT CFeeArrearReceiptDlg::UpdateReceiptList(CString strRecKey)
{
	return 0;
}

BEGIN_EVENTSINK_MAP(CFeeArrearReceiptDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CFeeArrearReceiptDlg)
	ON_EVENT(CFeeArrearReceiptDlg, IDC_gridARREARUSERLIST, -600 /* Click */, OnClickgridARREARUSERLIST, VTS_NONE)
	ON_EVENT(CFeeArrearReceiptDlg, IDC_gridARREARUSERLIST, 69 /* SelChange */, OnSelChangegridARREARUSERLIST, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CFeeArrearReceiptDlg::OnClickgridARREARUSERLIST(){}

VOID CFeeArrearReceiptDlg::OnbtnPRINTRECEIPT() 
{
	if ( m_pReceiptBookDM->GetRowCount() <= 0 )
	{
		AfxMessageBox( _T("출력할 영수증이 없습니다.") );
		return;
	}
	
	INT nPrintStatus=0;
	if ( m_ReceiptDlg->GetSafeHwnd() == NULL )
	{
		m_ReceiptDlg = new CBO_LOC_RECEIPT_PRINT_DLG( 3, this, m_pLSM );
		m_ReceiptDlg->pMain = this->pMain;
		m_ReceiptDlg->pParentMgr = (CESL_Mgr*)this;
		m_ReceiptDlg->m_pInfo = this->m_pInfo;

		nPrintStatus = m_ReceiptDlg->PrintFeeInfo(FALSE, TRUE, m_nIndex);
	}

	if ( nPrintStatus )
	{
		if ( AfxMessageBox( _T("출력실패. 재시도 하시게습니까?"), MB_YESNO ) == IDYES )
		{
			OnbtnPRINTRECEIPT();
			return;
		}
		else
		{
			this->Clear();
			ShowWindow(SW_HIDE);
			return;
		}
	}

	CString strUpdateKey;
	CString strUpdateKeyList;
	CString strQuery;
	CString strRecKey;

	strUpdateKeyList = _T("");
	for( INT i = 0 ; i < m_pReceiptBookDM->GetRowCount() ; i++ )
	{
		m_pReceiptBookDM->GetCellData( _T("REC_KEY"), i, strUpdateKey );
		if ( strUpdateKeyList != _T("") ) strUpdateKeyList += _T(",");
		strUpdateKeyList += strUpdateKey;
	}

	strQuery.Format( _T("SELECT ESL_SEQ.NEXTVAL FROM DUAL") );
	m_pReceiptBookDM->GetOneValueQuery(strQuery, strRecKey);

	CString sLastWork = GetWorkLogMsg( _T("연체료납부처리(출력)"), __WFILE__, __LINE__ );
	strQuery.Format( _T("UPDATE CO_FEE_PAYMENT_INFO_TBL SET STATUS='Y', RECEIPT_KEY=%s, ")
					 _T("RECEIPT_PRINT_YN='Y', PAYMENT_DATE=SYSDATE, LAST_WORK='%s' WHERE REC_KEY IN (%s);")
					 , strRecKey, sLastWork, strUpdateKeyList );		
	m_pReceiptBookDM->StartFrame();
	m_pReceiptBookDM->InitDBFieldData();
	m_pReceiptBookDM->AddFrame(strQuery);
	
	CString sReceiptXML = m_ReceiptDlg->m_strRCT;
	sReceiptXML.Replace( _T("'"), _T("''") );
	
	strQuery.Format( _T("INSERT INTO CO_FEE_RECEIPT_TBL ( REC_KEY, PRINT_COUNT, PRINT_DATE, RECEIPT_XML, FIRST_WORK, MANAGE_CODE )")
				 _T("VALUES ( '%s', '1', SYSDATE, '%s', '연체료 납부_'||SYSDATE, '%s');"), strRecKey, sReceiptXML, m_pInfo->MANAGE_CODE);	

	m_pReceiptBookDM->AddFrame(strQuery);	
	m_pReceiptBookDM->SendFrame(TRUE);	
	m_pReceiptBookDM->EndFrame();

	m_pReceiptBookDM->FreeData();

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_FEE_ARREAR_RECEIPT") , _T("UserGrid") );
	INT nIndex = pGrid->GetIdx();
	m_pUserDM->DeleteRow( nIndex );
	ReLoad();
	m_ReceiptDlg = NULL;
}

VOID CFeeArrearReceiptDlg::OnSelChangegridARREARUSERLIST() 
{
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_FEE_ARREAR_RECEIPT") , "UserGrid" );
	INT nIndex = pGrid->GetIdx();
	ShowReceiptList(nIndex);

	pGrid->SetReverse( nIndex );
}

VOID CFeeArrearReceiptDlg::OnbtnCLOSE() 
{
	this->ShowWindow(FALSE);
}

VOID CFeeArrearReceiptDlg::OnbtnPREVIEW() 
{
	if ( m_pReceiptBookDM->GetRowCount() <= 0 )
	{
		AfxMessageBox( _T("출력할 영수증이 없습니다.") );
		return;
	}

	INT nPrintStatus=0;
	if ( m_ReceiptDlg->GetSafeHwnd() == NULL )
	{
		m_ReceiptDlg = new CBO_LOC_RECEIPT_PRINT_DLG( 3, this, m_pLSM );
		m_ReceiptDlg->pMain = this->pMain;
		m_ReceiptDlg->pParentMgr = (CESL_Mgr*)this;
		m_ReceiptDlg->m_pInfo = this->m_pInfo;

		nPrintStatus = m_ReceiptDlg->PrintFeeInfo(TRUE, TRUE, m_nIndex);
	}

	if ( nPrintStatus )
	{
		if( nPrintStatus == 100)
		{
			return;
		}
		else
		{
			if ( AfxMessageBox( _T("출력실패. 재시도 하시게습니까?"), MB_YESNO ) == IDYES )
			{
				OnbtnPRINTRECEIPT();
				return;
			}
			else
			{
				this->Clear();
				ShowWindow(SW_HIDE);
				return;
			}
		}
	}

	CString strUpdateKey;
	CString strUpdateKeyList;
	CString strQuery;
	CString strRecKey;

	strUpdateKeyList = _T("");
	for ( int i=0 ; i<m_pReceiptBookDM->GetRowCount() ; i++ )
	{
		m_pReceiptBookDM->GetCellData( _T("REC_KEY"), i, strUpdateKey );
		if ( strUpdateKeyList != _T("") ) strUpdateKeyList += _T(",");
		strUpdateKeyList += strUpdateKey;
	}

	strQuery.Format( _T("SELECT ESL_SEQ.NEXTVAL FROM DUAL") );
	m_pReceiptBookDM->GetOneValueQuery(strQuery, strRecKey);

	CString sLastWork = GetWorkLogMsg( _T("연체료납부처리(출력)"), __WFILE__, __LINE__ );

	strQuery.Format( _T("UPDATE CO_FEE_PAYMENT_INFO_TBL SET STATUS='Y', RECEIPT_KEY=%s, ")
					 _T("RECEIPT_PRINT_YN='Y', PAYMENT_DATE=SYSDATE, LAST_WORK='%s' WHERE REC_KEY IN (%s);")
					 , strRecKey, sLastWork, strUpdateKeyList );		
	m_pReceiptBookDM->StartFrame();
	m_pReceiptBookDM->InitDBFieldData();	
	m_pReceiptBookDM->AddFrame(strQuery);	
	
	CString sReceiptXML = m_ReceiptDlg->m_strRCT;
	sReceiptXML.Replace( _T("'"), _T("''") );
	
	strQuery.Format( _T("INSERT INTO CO_FEE_RECEIPT_TBL ( REC_KEY, PRINT_COUNT, PRINT_DATE, RECEIPT_XML, FIRST_WORK, MANAGE_CODE )")
					 _T("VALUES ( '%s', '1', SYSDATE, '%s', '연체료 납부_'||SYSDATE, UDF_MANAGE_CODE);")
					 , strRecKey, sReceiptXML);
	m_pReceiptBookDM->AddFrame(strQuery);	
	m_pReceiptBookDM->SendFrame(TRUE);	
	m_pReceiptBookDM->EndFrame();

	m_pReceiptBookDM->FreeData();
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_FEE_ARREAR_RECEIPT") , _T("UserGrid") );
	INT nIndex = pGrid->GetIdx();
	m_pUserDM->DeleteRow( nIndex );
	
	ReLoad();

	m_ReceiptDlg = NULL;
}

BOOL CFeeArrearReceiptDlg::PreTranslateMessage(MSG* pMsg) 
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

VOID CFeeArrearReceiptDlg::OnChangeedtMONEYPAID() 
{
	UpdateData(TRUE);

	m_strEdtMoneyPaid.Replace( _T(","), _T("") );

	CString strBuf = m_strEdtMoneyPaid;
	strBuf.Replace( _T(","), _T("") );

	INT nSize = (strBuf.GetLength() * 3) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 0 };

	::GetNumberFormat(NULL, NULL, strBuf, &nFmt, pszFormattedNumber, nSize-1);

	m_strEdtMoneyPaid = pszFormattedNumber;

	if (pszFormattedNumber)
		delete [] pszFormattedNumber;

	UpdateData(FALSE);

	INT nLen = m_strEdtMoneyPaid.GetLength();
	m_edtMoneyPaid.SetSel(nLen, -1);
}

VOID CFeeArrearReceiptDlg::OnbtnPRINTCANCEL() 
{
	if ( m_pReceiptBookDM->GetRowCount() <= 0 )
	{
		AfxMessageBox(_T("출력취소할 영수증이 없습니다."));
	}
	else
	{
		if ( AfxMessageBox(_T("수수료를 받으셨습니까?\n(영수증 출력을 생략합니다.)\n취소된 자료는 수수료관리에서 재출력할 수 있습니다."), MB_YESNO) == IDYES )
		{
			// 1.XML생성
			INT nPrintStatus=0;
			if ( m_ReceiptDlg->GetSafeHwnd() == NULL )
			{
				m_ReceiptDlg = new CBO_LOC_RECEIPT_PRINT_DLG( 3, this, m_pLSM );
				m_ReceiptDlg->pMain = this->pMain;
				m_ReceiptDlg->pParentMgr = (CESL_Mgr*)this;
				m_ReceiptDlg->m_pInfo = this->m_pInfo;
				
				nPrintStatus = m_ReceiptDlg->PrintFeeInfo(FALSE, FALSE, m_nIndex);
			}
			
			if ( nPrintStatus )
			{
				AfxMessageBox(_T("XML생성에 실패 하였습니다."));
				return;
			}
			
			CString strUpdateKey;
			CString strUpdateKeyList;
			CString strQuery;
			CString strRecKey;

			strUpdateKeyList = _T("");
			for( INT i=0 ; i<m_pReceiptBookDM->GetRowCount() ; i++ )
			{
				m_pReceiptBookDM->GetCellData( _T("REC_KEY"), i, strUpdateKey );
				if( strUpdateKeyList != _T("") ) strUpdateKeyList += _T(",");
				strUpdateKeyList += strUpdateKey;
			}
			
			strQuery.Format( _T("SELECT ESL_SEQ.NEXTVAL FROM DUAL") );
			m_pReceiptBookDM->GetOneValueQuery(strQuery, strRecKey);

			CString sLastWork = GetWorkLogMsg( _T("연체료납부처리(출력취소)"), __WFILE__, __LINE__ );

			strQuery.Format( _T("UPDATE CO_FEE_PAYMENT_INFO_TBL SET STATUS='Y', RECEIPT_KEY='%s', ")
							 _T("PAYMENT_DATE=SYSDATE, LAST_WORK='%s' WHERE REC_KEY IN (%s);")
							 , strRecKey, sLastWork, strUpdateKeyList );		
			m_pReceiptBookDM->StartFrame();
			m_pReceiptBookDM->InitDBFieldData();
			m_pReceiptBookDM->AddFrame(strQuery);
			
			CString sReceiptXML = m_ReceiptDlg->m_strRCT;
			sReceiptXML.Replace( _T("'"), _T("''") );
			
			strQuery.Format( _T("INSERT INTO CO_FEE_RECEIPT_TBL ( REC_KEY, PRINT_COUNT, PRINT_DATE, RECEIPT_XML, FIRST_WORK, MANAGE_CODE )")
				 _T("VALUES ( '%s', '0', SYSDATE, '%s', '연체료 납부_'||SYSDATE, UDF_MANAGE_CODE);")
							 , strRecKey, sReceiptXML);
			m_pReceiptBookDM->AddFrame(strQuery);
			m_pReceiptBookDM->SendFrame(TRUE);	
			m_pReceiptBookDM->EndFrame();
			
			m_pReceiptBookDM->FreeData();
			
			CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_FEE_ARREAR_RECEIPT") , _T("UserGrid") );
			INT nIndex = pGrid->GetIdx();
			m_pUserDM->DeleteRow( nIndex );
			
			ReLoad();
			
			m_ReceiptDlg = NULL;
		}
	}
}

HBRUSH CFeeArrearReceiptDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
