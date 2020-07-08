// FeeInsertLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FeeInsertLogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeInsertLogDlg dialog


CFeeInsertLogDlg::CFeeInsertLogDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CFeeInsertLogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeInsertLogDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFeeInsertLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeInsertLogDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeInsertLogDlg, CDialog)
	//{{AFX_MSG_MAP(CFeeInsertLogDlg)
	ON_BN_CLICKED(IDC_btnInsertLogOK, OnbtnInsertLogOK)
	ON_BN_CLICKED(IDC_btnInsertLogCancel, OnbtnInsertLogCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeInsertLogDlg message handlers

BOOL CFeeInsertLogDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// XP THEME
	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	if( InitESL_Mgr( _T("FEE_INSERT_LOG") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : FEE_INSERT_LOG") );
		return FALSE;
	}

	INT ids;
	ids = SetDM();
	if(0 > ids)
	{
		return FALSE;
	}

	ids = AllControlDisplay( _T("CM_FEE_INSERT_LOG") );
	if(0 > ids) 
	{
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFeeInsertLogDlg::OnbtnInsertLogOK() 
{
	// INSERT
	INT ids;
	CString strQuery;
	CString sFirstWork = GetWorkLogMsg( _T("수수료소급입력"), __WFILE__, __LINE__ );

	m_strMoneyPaid.Replace( _T(","), _T("") );
	strQuery.Format(_T("INSERT INTO CO_FEE_PAYMENT_INFO_TBL")
					_T("(REC_KEY,CLASS_CODE,LOAN_INFO_KEY,PAYMENT_DATE,MONEY_PAID,STATUS,")
					_T("CREATE_DATE,USER_ID,RECEIPT_KEY,RECEIPT_PRINT_YN,MANAGE_CODE,RETRO_YN,FIRST_WORK) ")
					_T("VALUES(ESL_SEQ.NEXTVAL,'%s',%s,%s,%s,'Y',SYSDATE,'%s',0,'N',UDF_MANAGE_CODE,'Y','%s');")
					, m_strFeeClass, m_strUserKey, m_strPaymentDate, m_strMoneyPaid, m_pInfo->USER_ID, sFirstWork);
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	dm.StartFrame();
	dm.InitDBFieldData();
	dm.AddFrame(strQuery);
	ids = dm.SendFrame(TRUE);	
	if(0 > ids)
	{
		AfxMessageBox(_T("수수료 입력에 실패하였습니다."));
		return;
	}
	dm.EndFrame();

	OnOK();
}

INT CFeeInsertLogDlg::SetDM()
{
	INT ids;
	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_INSERT_LOG") );
	if( pDM == NULL ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1000, _T("SetDM") );
	}

	// 대상 저장
	const int nLogCnt = 5;
	CString sLogAliasList[nLogCnt] = 
	{
		_T("대출자번호"), _T("대출자이름"), _T("수수료종류"), _T("납부일자"), _T("납부금액")
	};
	CString sLogValueList[nLogCnt];
	// 대출자번호(설명)
	sLogValueList[0] = m_strUserNoDesc;	
	// 대출자이름
	sLogValueList[1] = m_strUserName;	
	// 수수료종류
	if(_T("0") == m_strFeeClass)
	{
		sLogValueList[2] = _T("연체료");
	}
	else if(_T("1") == m_strFeeClass)
	{
		sLogValueList[2] = _T("회원증재발급");
	}
	else if(_T("2") == m_strFeeClass)
	{
		sLogValueList[2] = _T("대출정지일해제");
	}
	else
	{
		sLogValueList[2] = _T("");
	}	
	// 납부일자
	sLogValueList[3] = m_strPaymentDate.Mid(9, 10);
	// 납부금액
	sLogValueList[4] = m_strMoneyPaid + _T("원");	

	// DM INSERT
	for(INT i = 0; i < nLogCnt; i++)
	{
		pDM->InsertRow(-1);
		ids = pDM->SetCellData( _T("정보"), sLogAliasList[i], i );
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1010, _T("SetDM") );
		}
		ids = pDM->SetCellData( _T("내용"), sLogValueList[i], i );
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1040, _T("SetDM") );
		}

	}

	return 0;
}

void CFeeInsertLogDlg::OnbtnInsertLogCancel() 
{
	OnCancel();	
}

BOOL CFeeInsertLogDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( VK_RETURN == pMsg->wParam )
	{
		if( WM_KEYDOWN == pMsg->message || WM_CHAR == pMsg->message )
		{
			// 입력
			if( GetDlgItem(IDC_btnInsertLogOK)->GetSafeHwnd() == pMsg->hwnd)
			{
				// 입력진행
				OnbtnInsertLogOK();
			}
			// 취소
			else if( GetDlgItem(IDC_btnInsertLogCancel)->GetSafeHwnd() == pMsg->hwnd)
			{
				// 취소진행
				OnbtnInsertLogCancel();
			}
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
