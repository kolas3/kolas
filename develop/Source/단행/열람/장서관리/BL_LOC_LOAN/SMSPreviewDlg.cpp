// SMSPreviewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SMSPreviewDlg.h"
#include "Imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString g_strKakaoUsed;

/////////////////////////////////////////////////////////////////////////////
// CSMSPreviewDlg dialog


CSMSPreviewDlg::CSMSPreviewDlg(CESL_Mgr* pParent /*=NULL*/ , BOOL IsEditMode /*=FALSE*/)
	: CESL_Mgr(CSMSPreviewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMSPreviewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bIsEditMode	=	IsEditMode;
	m_nDividCnt		=	0;
}


void CSMSPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMSPreviewDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMSPreviewDlg, CDialog)
	//{{AFX_MSG_MAP(CSMSPreviewDlg)
	ON_BN_CLICKED(IDC_BTN_SMS_SEND, OnBtnSmsSend)
	ON_BN_CLICKED(IDC_BTN_SMS_CLOSE, OnBtnSmsClose)
	ON_EN_CHANGE(IDC_EDIT_SMS_MSG, OnChangeEditSmsMsg)
	ON_EN_SETFOCUS(IDC_EDIT_SMS_MSG, OnSetfocusEditSmsMsg)
	ON_EN_CHANGE(IDC_EDIT_SMS_SENDPHONE, OnChangeEditSmsSendphone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMSPreviewDlg message handlers

void CSMSPreviewDlg::OnBtnSmsSend() 
{
	INT ids;

	CString strMsg , strTemp;
	( (CEdit*)GetDlgItem( IDC_EDIT_SMS_MSG ) )->GetWindowText( strMsg );
	strTemp		=	strMsg;
	strTemp.TrimLeft();		strTemp.TrimRight();
	strTemp.Replace( _T(" ") , _T("") );

	INT		nLength		=	strTemp.GetLength();
	if( 1 > nLength )
	{
		AfxMessageBox( _T("보내실 내용을 입력하세요.") , MB_ICONINFORMATION );
		return;
	}

	CString strSendTel;
	( (CEdit*)GetDlgItem( IDC_EDIT_SMS_SENDPHONE ) )->GetWindowText( strSendTel );
	strSendTel.TrimLeft();		strSendTel.TrimRight();
	strSendTel.Replace( _T(" ") , _T("") );

	nLength		=	strSendTel.GetLength();
	if( 1 > nLength )
	{
		AfxMessageBox( _T("보내는 사람 전화번호를 입력하세요.") , MB_ICONINFORMATION );
		return;
	}
	
	
	ids = SendMsg();
	if( 0 > ids ) 
	{
		AfxMessageBox( _T("SMS보내기에 실패하였습니다.") );
		return;
	}
	else
	{
		nLength = GetSMSLength( strMsg.GetBuffer(0) );
		if( 80 < nLength )
		{
			if( _T("Y") == m_strSMSDivideYN )
			{
				m_nDividCnt =	( nLength/80 ) + 1;
				EndDialog( 1010 );
			}
			else
			{
				m_nDividCnt	=	1;
				EndDialog( 1020 );
			}
		}
		else
		{
			m_nDividCnt	=	1;
			EndDialog( 0 );
		}
	}
}

void CSMSPreviewDlg::OnBtnSmsClose() 
{
	OnCancel();
}

BOOL CSMSPreviewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd());

	INT	ids = MakeMsg();
	if( 0 > ids ) 
	{
		return FALSE;
	}
	else if( 1030 == ids )
	{
		EndDialog( 1030 );
	}

	((CEdit*)GetDlgItem( IDC_EDIT_SMS_RECEIVEPHONE ) )->SetWindowText(m_sRecvTel);
	((CEdit*)GetDlgItem( IDC_EDIT_SMS_SENDPHONE ) )->SetWindowText(m_sSendTel);
	if( TRUE == m_bIsEditMode )
	{
		m_sMsg	=	_T("");
		((CEdit*)GetDlgItem( IDC_EDIT_SMS_MSG ))->SetReadOnly( FALSE );		
		((CEdit*)GetDlgItem( IDC_EDIT_SMS_MSG ) )->SetWindowText( m_sMsg );
		OnChangeEditSmsMsg();

		((CEdit*)GetDlgItem( IDC_EDIT_SMS_MSG ))->SetFocus();
	}
	else
	{
		((CEdit*)GetDlgItem( IDC_EDIT_SMS_MSG ))->SetReadOnly( TRUE );
		((CEdit*)GetDlgItem( IDC_EDIT_SMS_MSG ))->SetWindowText(m_sMsg);
		OnChangeEditSmsMsg();

		((CButton*)GetDlgItem( IDC_BTN_SMS_SEND ))->SetFocus();
	}

	return TRUE;
}

void CSMSPreviewDlg::OnChangeEditSmsMsg() 
{
	UpdateData( TRUE );

	( (CEdit*)GetDlgItem( IDC_EDIT_SMS_MSG ) )->GetWindowText( m_sMsg );
	
	CString sLen;
	sLen.Format(_T("%d byte"), GetSMSLength(m_sMsg.GetBuffer(0)) );
	( (CEdit*)GetDlgItem(IDC_EDIT_SMS_LENGTH) )->SetWindowText(sLen);
	UpdateData( FALSE );
}

INT CSMSPreviewDlg::SetInfo(CString sUserKey, CString sLoanKey, CString sUserName, CString sTitle, 
							CString sDate, CString sRecvTel, CString sSMSType , CString strSMSDivideYN )
{
	m_sUserKey = sUserKey;
	if( TRUE == m_sUserKey.IsEmpty() ) 
	{
		m_sUserKey = _T("0");
	}

	m_sLoanKey = sLoanKey;
	if( TRUE == m_sLoanKey.IsEmpty() ) 
	{
		m_sLoanKey = _T("0");
	}

	m_sUserName = sUserName;
	m_sTitle = sTitle;
	m_sDate = sDate; 
	m_sRecvTel = sRecvTel;
	m_sSMSType = sSMSType;

	m_strSMSDivideYN	=	strSMSDivideYN;

	return 0;
}

INT CSMSPreviewDlg::MakeMsg()
{
	INT ids;

	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	
	SMS_CONNECTION_INFO SMS_ConnectionInfo;
	SMS_MSG_INFO SMS_MsgInfo;	
	CString sSMSType = m_sSMSType;
	CSMS_MakeMsg MakeSMS(this);
	CSMS_Env_Info SMSEnvInfo(this);
	SMSEnvInfo.MakeSMSEnv();
	SMSEnvInfo.GetSMSEnv( sSMSType, &SMS_ConnectionInfo, &SMS_MsgInfo );


	m_sSendTel = SMS_MsgInfo.sCallBackNumber;
	m_sRecvTel.Replace( _T("-"), _T("") );
	m_sSendTel.Replace( _T("-"), _T("") );
	if( _T("") == m_sRecvTel ) 
	{
		return 1030;
	}	

	CString sFirst_Work;
	sFirst_Work.Format( _T("%s"), GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ) );

	ids = MakeSMS.MakeSMSMsg( sSMSType, &SMS_MsgInfo, m_sUserName, m_sDate, m_sTitle, 1, m_sMsg );
	if(ids<0)
	{
		AfxMessageBox(_T("문자메시지 구성에 실패했습니다."));
		return -2;
	}

	return 0;
}

INT CSMSPreviewDlg::SendMsg()
{
	INT ids;
	
	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
 	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);

	CString sFirst_Work;
	sFirst_Work.Format( _T("%s"), GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ) );

	CString sMsg = m_sMsg;
	sMsg.Replace( _T("'"), _T("''") );


	CString strQuery;

	if (g_strKakaoUsed.IsEmpty())
	{
		strQuery.Format( 
			_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='KAKAO플러스친구시스템정보' AND value_name='KAKAO플러스친구사용유무' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		TmpDM.GetOneValueQuery(strQuery, g_strKakaoUsed);
		
		if (g_strKakaoUsed.IsEmpty())
		{
			g_strKakaoUsed = _T("N");
		}
	}
	if (g_strKakaoUsed.CompareNoCase(_T("Y")) == 0)
	{
		strQuery.Format( _T("KAKAOPLUS_TOT_SEND_PROC(%s, %s, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', NULL, '%s', '%s');")
								, m_sUserKey, m_sLoanKey, m_sSendTel, m_sRecvTel, sMsg	// 1,2,3,4,5
								, _T(""), m_pInfo->MANAGE_CODE, m_sSMSType				// 6,7,8
								, m_pInfo->USER_NAME, sFirst_Work, 					    // 9,10
								m_pInfo->LIB_NAME , sMsg );
	}
	else
	{
		strQuery.Format( _T("SMS_TOT_SEND_PROC(%s,%s,'%s','%s','%s','%s','%s','%s','%s','%s');")
					, m_sUserKey, m_sLoanKey, m_sSendTel, m_sRecvTel, sMsg	// 1,2,3,4,5
					, _T(""), m_pInfo->MANAGE_CODE, m_sSMSType				// 6,7,8
					, m_pInfo->USER_NAME, sFirst_Work );					// 9,10
	}
	ids = TmpDM.ExecuteQuery(strQuery, TRUE);
	if( 0 > ids ) 
	{
		return -1;
	}
	
	return 0;
}

INT	CSMSPreviewDlg::GetSMSLength(_TCHAR *fi_pSourceData)
{
	INT		nWCharSource;
	INT		nBytesNeeded;

	nWCharSource   = wcslen(fi_pSourceData);

	nBytesNeeded = WideCharToMultiByte( CP_ACP 	, 
										0				,
										(LPWSTR)fi_pSourceData	, 
										nWCharSource			,
										NULL					, 
										0						,
										NULL, 
										NULL );
	return nBytesNeeded;
}

CString CSMSPreviewDlg::GetDivideMsg( CString &msg )
{
	CString sendMsg;
	INT length;	

	CHAR		*szMsg;		
	CHAR		szCutMsg[81];
	INT			nLen;

	this->UnicodeToMultiByte(msg.GetBuffer(0), &szMsg, &nLen );	

	if( IsCanCut( szMsg, 80 ) )
		length = 80;
	else
		length = 80 - 1;

	strncpy(szCutMsg , szMsg, length);
	szCutMsg[length] = '\0';


	TCHAR *szUnicodeMessage;
	INT		nUnicodeLen;

	this->MultiByteToUnicode(szCutMsg, &szUnicodeMessage, &nUnicodeLen );

	sendMsg.Format(_T("%s"), szUnicodeMessage );
	
	msg = msg.Mid( sendMsg.GetLength() );

	return sendMsg;
}

BOOL CSMSPreviewDlg::IsCanCut( CString str, INT pos )
{
	INT i, nLastPos = -1;
	if( str.GetLength() <= pos )
		return TRUE;
	
	TCHAR ch = str.GetAt( pos );
	
	if( pos + 1 == str.GetLength() || ( ch & ( 0x80 ) ) != 128 )
		return TRUE;
	
	if( !pos )
		if( ( ch & ( 0x80 ) ) != 128 )
			return TRUE;
		else
			return FALSE;

	for( i = 0; i < pos; i++ )
	{
		ch = str.GetAt( i );
		
		if( ( ch & ( 0x80 ) ) != 128 )
			nLastPos = i;
	}
	
	if( nLastPos == -1 && pos % 2 == 1 )
		return TRUE;
	
	if( nLastPos == -1 && pos % 2 == 0 )
		return TRUE;
	
	BOOL result = !( ( pos - nLastPos ) % 2 );
	return result;
}

INT	CSMSPreviewDlg::UnicodeToMultiByte(_TCHAR *fi_pSourceData, CHAR **fo_pDestinationData, INT *fo_nByteDes )
{

	CHAR	*pszDestinationData;
	INT		nWCharSource;
	INT		nBytesNeeded;

	
	nWCharSource   = wcslen(fi_pSourceData);

	if ( nWCharSource <= 0 ) {
		*fo_nByteDes = 0;
		return -100;
	}
	
	nBytesNeeded = WideCharToMultiByte( CP_ACP 	, 
										0				,
										(LPWSTR)fi_pSourceData	, 
										nWCharSource			,
										NULL					, 
										0						,
										NULL, 
										NULL );
	
	if (nBytesNeeded == 0) 
	{
		*fo_nByteDes = 0;
		DWORD dw = GetLastError();
		return -100 ;
	}
	
	pszDestinationData = (CHAR*) malloc(sizeof(CHAR) * nBytesNeeded + sizeof(CHAR));
	
	nBytesNeeded = WideCharToMultiByte     (CP_ACP		, 
											0					,
											(LPWSTR)fi_pSourceData		, 
											nWCharSource				,
											(CHAR*)pszDestinationData	, 
											nBytesNeeded				, 
											NULL			    , 
										    NULL );
	if (nBytesNeeded == 0) 
	{
		*fo_nByteDes = 0;
		free(pszDestinationData);
		return -100 ;
	}

	*(LPSTR)((LPSTR)pszDestinationData + nBytesNeeded) = '\0';	

	*fo_pDestinationData = pszDestinationData;
	*fo_nByteDes         = nBytesNeeded;

	return 0;
}

INT	CSMSPreviewDlg::MultiByteToUnicode(CHAR *fi_pSourceData, _TCHAR **fo_pDestinationData, INT *fo_nByteDes )
{

	_TCHAR	*pszDestinationData;
	INT		nWCharNeeded;
	INT		nSouceDataByte;
	

	nSouceDataByte = strlen(fi_pSourceData);
	
	/* Query the number of WChar required to store the Dest string */
	nWCharNeeded = MultiByteToWideChar( CP_ACP , 
										(UINT)0  , 
										fi_pSourceData   , 
										nSouceDataByte   , 
										NULL             , 
										0 );
	if (nWCharNeeded == 0) 
	{
		*fo_nByteDes = 0;
		return -100 ;
	}
	

	/* Allocate the required amount of space */	
	/* We need more 2 bytes for '\0' */
	pszDestinationData = (_TCHAR*) malloc (sizeof(_TCHAR)*nWCharNeeded + sizeof(_TCHAR));	
		
	/* Do the conversion */
	nWCharNeeded = MultiByteToWideChar(	CP_ACP				, 
										0						,
										fi_pSourceData					, 
										nSouceDataByte			   		,
										pszDestinationData          	, 
										nWCharNeeded);
	if (nWCharNeeded == 0) 
	{
		*fo_nByteDes = 0;
		free(pszDestinationData);
		return -100 ;
	}	

	*(LPWSTR)((LPWSTR)pszDestinationData + nWCharNeeded) = L'\0';
	
	/* MultiByteToWideChar returns # WCHAR, so multiply by 2 */
	*fo_nByteDes = 2*nWCharNeeded ;	

	*fo_pDestinationData = pszDestinationData;


	return 0;
}

void CSMSPreviewDlg::OnSetfocusEditSmsMsg() 
{
EFS_BEGIN

	CEdit *pEditSMSMsg;
	pEditSMSMsg = (CEdit*)GetDlgItem(IDC_EDIT_SMS_MSG);
		
	HIMC hIME;
	DWORD dwConversion, dwSentence;
	
	hIME = ::ImmGetContext( pEditSMSMsg->m_hWnd );
	if(hIME) 
	{
		ImmGetConversionStatus( hIME , &dwConversion , &dwSentence );
		
		dwConversion=IME_CMODE_HANGUL;
		
		ImmSetConversionStatus( hIME , dwConversion , dwSentence );
		
		ImmReleaseContext( pEditSMSMsg->m_hWnd , hIME ); 
	} 

EFS_END	
}

void CSMSPreviewDlg::OnChangeEditSmsSendphone() 
{
	CString strSendTel;
	( (CEdit*)GetDlgItem( IDC_EDIT_SMS_SENDPHONE ) )->GetWindowText( strSendTel );

	m_sSendTel	=	strSendTel;		
}

BOOL CSMSPreviewDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( VK_RETURN == pMsg->wParam )
	{
		if( WM_KEYDOWN == pMsg->message || WM_CHAR == pMsg->message )
		{
			// 보내기
			if( GetDlgItem(IDC_BTN_SMS_SEND)->GetSafeHwnd() == pMsg->hwnd)
			{
				// SEND
				OnBtnSmsSend();
			}
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
