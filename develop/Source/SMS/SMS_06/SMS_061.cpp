// SMS_061.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_061.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	SMS_LENGTH 80
/////////////////////////////////////////////////////////////////////////////
// CSMS_06 dialog


CSMS_06::CSMS_06(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr( IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_06)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pSMSList = NULL;
	m_strPhoneNumber = _T("");
}

CSMS_06::~CSMS_06()
{
	if( m_pSMSList )
	{
		delete m_pSMSList;
		m_pSMSList = NULL;
	}


}

VOID CSMS_06::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_06)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_06, CDialog)
	//{{AFX_MSG_MAP(CSMS_06)
	ON_BN_CLICKED(IDC_btnSEND, OnbtnSEND)
	ON_BN_CLICKED(IDC_btnClear, OnbtnClear)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_eMsg, OnChangeeMsg)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_06 message handlers

BOOL CSMS_06::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CSMS_06::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("SMS_DLG_06") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : SMS_DLG_01") );
		return false;
	}

	( (CEdit*) GetDlgItem ( IDC_eMsg ) )->SetLimitText(80);
	( (CEdit*) GetDlgItem ( IDC_eLen ) )->SetWindowText(_T("0 / 80 bytes"));

	( (CEdit*) GetDlgItem (IDC_eReceiveNum) )->SetWindowText(m_strPhoneNumber);

	m_pSMSList = new SMS_LIST;	
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSMS_06::OnbtnSEND() 
{	
	CESL_DataMgr *pDM;
	CString strMsgID;
	CString strMsg;
	CString strSender;
	CString strReceiveName;
	CString strReceiveNum;
	CString strCallBackNum;	
	CString strWhere;
	CString strSMSSystemInfo;
	INT		ids = 0;

	( (CEdit*) GetDlgItem (IDC_eMsg ) )->GetWindowText(strMsg);
	( (CEdit*) GetDlgItem (IDC_eReceiveNum ) )->GetWindowText(strReceiveNum);
	( (CEdit*) GetDlgItem (IDC_eSendNum ) )->GetWindowText(strSender);	

	pDM = FindDM( _T("DM_SMS_DLG_06") );

	strWhere=_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
			 _T("WHERE CLASS_ALIAS='SMS시스템정보' AND MANAGE_CODE=UDF_MANAGE_CODE");

	pDM->GetOneValueQuery(strWhere, strSMSSystemInfo);

	RemoveSMSList();

	CSMSSendDlg dlg( this );
	if ( strSMSSystemInfo == _T("0") || strSMSSystemInfo.IsEmpty() )
	{
		SMS_MSG_INFO SMS_MsgInfo;
		SMS_CONNECTION_INFO SMS_ConnectionInfo ;

		CSMS_Env_Info SMSEnvInfo( this );
		SMSEnvInfo.MakeSMSEnv();
		SMSEnvInfo.GetSMSEnv( _T("0"), &SMS_ConnectionInfo , &SMS_MsgInfo );

		dlg.m_SMS_ConnectionInfo.sBindId			= SMS_ConnectionInfo.sBindId;
		dlg.m_SMS_ConnectionInfo.sBindPwd			= SMS_ConnectionInfo.sBindPwd;
		dlg.m_SMS_ConnectionInfo.sUserDomain		= SMS_ConnectionInfo.sUserDomain;
		dlg.m_SMS_ConnectionInfo.sUserPhoneId		= SMS_ConnectionInfo.sUserPhoneId;
		dlg.m_SMS_ConnectionInfo.sUserAccessIp		= SMS_ConnectionInfo.sUserAccessIp;
		dlg.m_SMS_ConnectionInfo.sCfgConFilePath	= SMS_ConnectionInfo.sCfgConFilePath;
		dlg.m_callbackNum = strSender;
		dlg.m_pSMSList = NULL;
		dlg.m_pSMSSender = NULL;		
		dlg.m_SMSSending = false;
		dlg.m_bSMSStop = false;
		dlg.SMSInit(this);
		

		m_pSMSList->msg.AddTail(strMsg);
		m_pSMSList->pParent = this;
		m_pSMSList->pSMSSender = NULL;
		m_pSMSList->receiverName.AddTail(_T(""));
		m_pSMSList->receiverNum.AddTail(strReceiveNum);
		m_pSMSList->title = _T("");

		dlg.OneSendSMSMsg(m_pSMSList);

		
	}
	else if ( strSMSSystemInfo == _T("1") )
	{
		m_pSMSList->msg.AddTail(strMsg);
		m_pSMSList->pParent = this;
		m_pSMSList->pSMSSender = NULL;
		m_pSMSList->receiverName.AddTail(_T(""));
		m_pSMSList->receiverNum.AddTail(strReceiveNum);
		m_pSMSList->title = _T("");
		dlg.m_pSMSList = NULL;
		dlg.m_pSMSSender = NULL;		
		dlg.m_SMSSending = false;
		dlg.m_bSMSStop = false;
		dlg.SMSInit();


		dlg.OneSendSMSMsg(m_pSMSList);
	}
	else if ( strSMSSystemInfo == _T("2") )
	{
		m_pSMSList->msg.AddTail(strMsg);
		m_pSMSList->pParent = this;
		m_pSMSList->pSMSSender = NULL;
		m_pSMSList->receiverName.AddTail(_T(""));
		m_pSMSList->receiverNum.AddTail(strReceiveNum);
		m_pSMSList->title = _T("");
		dlg.m_pSMSList = NULL;
		dlg.m_pSMSSender = NULL;		
		dlg.m_SMSSending = false;
		dlg.m_bSMSStop = false;
		dlg.SMSInit();
		
		dlg.OneSendSMSMsg(m_pSMSList);
	}		
	else if ( strSMSSystemInfo == _T("3") )
	{
		m_pSMSList->msg.AddTail(strMsg);
		m_pSMSList->pParent = this;
		m_pSMSList->pSMSSender = NULL;
		m_pSMSList->receiverName.AddTail(_T(""));
		m_pSMSList->receiverNum.AddTail(strReceiveNum);
		m_pSMSList->title = _T("");
		dlg.m_pSMSList = NULL;
		dlg.m_pSMSSender = NULL;		
		dlg.m_SMSSending = false;
		dlg.m_bSMSStop = false;
		dlg.SMSInit();
		
		dlg.OneSendSMSMsg(m_pSMSList);
	}		
	else if ( strSMSSystemInfo == _T("4") )
	{
		m_pSMSList->msg.AddTail(strMsg);
		m_pSMSList->pParent = this;
		m_pSMSList->pSMSSender = NULL;
		m_pSMSList->receiverName.AddTail(_T(""));
		m_pSMSList->receiverNum.AddTail(strReceiveNum);
		m_pSMSList->title = _T("");
		dlg.m_pSMSList = NULL;
		dlg.m_pSMSSender = NULL;		
		dlg.m_SMSSending = false;
		dlg.m_bSMSStop = false;
		dlg.SMSInit();
		
		dlg.OneSendSMSMsg(m_pSMSList);
	}
	else
	{
		AfxMessageBox(_T("발송취소 되었습니다. 설정값을 확인하십시오."));
		return;
	}

	AfxMessageBox(_T("발송되었습니다."));
	
}

VOID CSMS_06::OnbtnClear() 
{
	( (CEdit*) GetDlgItem (IDC_eMsg ) )->SetWindowText(_T(""));	
	( (CEdit*) GetDlgItem (IDC_eMsg ) )->SetFocus();
	( (CEdit*) GetDlgItem ( IDC_eLen ) )->SetWindowText(_T("0 / 80 bytes"));
}

INT CSMS_06::SendMsgInDB(CESL_DataMgr *pDM, CString strMsgID, CString strMSGGB, CString strMSGDTTM, CString strSendPhoneNum, CString strReceivePhone, CString strCallBackNum, CString strMsg)
{
	pDM->InitDBFieldData();	
	pDM->AddDBFieldData( _T("CMP_MSG_ID")	, _T("STRING") , strMsgID );
	pDM->AddDBFieldData( _T("CMP_USR_ID")	, _T("STRING") , _T("00000") );
	pDM->AddDBFieldData( _T("ODR_FG")		, _T("STRING") , _T("1") );
	pDM->AddDBFieldData( _T("SMS_GB")		, _T("STRING") , _T("1") );
	pDM->AddDBFieldData( _T("USED_CD")		, _T("STRING") , _T("00") );		
	pDM->AddDBFieldData( _T("MSG_GB")		, _T("STRING") , strMSGGB );  //A :SMS, B:URL, C:해외SMS, D:해외URL
	pDM->AddDBFieldData( _T("SND_DTTM")		, _T("STRING") , strMsgID.Left(14) );
	pDM->AddDBFieldData( _T("SND_PHN_ID")	, _T("STRING") , strCallBackNum );		
	pDM->AddDBFieldData( _T("RCV_PHN_ID")	, _T("STRING") , strReceivePhone );
	pDM->AddDBFieldData( _T("CALLBACK")		, _T("STRING") , strCallBackNum );//callbackNum );
	pDM->AddDBFieldData( _T("SND_MSG")		, _T("STRING") , strMsg );	
	pDM->AddDBFieldData( _T("EXPIRE_VAL")	, _T("STRING") , _T("0") );
	pDM->AddDBFieldData( _T("SMS_ST")		, _T("STRING") , _T("0") );
	pDM->AddDBFieldData( _T("RSLT_VAL")		, _T("STRING") , _T("99") );
	pDM->MakeInsertFrame( _T("ARREO_SMS"));	

	return 0;
}

INT CSMS_06::MakeMsgID(CESL_DataMgr *pDM, CString &f_pMsgID)
{
	CString strDateTime;
	CString strSqeID;
	CString strSeqWhere;
	CString strTmp;
	CTime tCurrent;	

	pDM->GetDBTime();	
	tCurrent = pDM->GetDBTime();	
	strDateTime.Format(_T("%04d%02d%02d%02d%02d"),tCurrent.GetYear(),tCurrent.GetMonth(),tCurrent.GetDay(), tCurrent.GetHour(), tCurrent.GetMinute() );

	strSeqWhere.Format(_T("SELECT ARREO_SEQ.NEXTVAL FROM DUAL"));
	pDM->GetOneValueQuery(strSeqWhere, strTmp );

	strSqeID.Format(_T("%08s"), strTmp);	
	f_pMsgID = strDateTime+strSqeID;
	
	return 0;
}

VOID CSMS_06::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

VOID CSMS_06::OnChangeeMsg() 
{
	CString strText;
	CString strLen;

	( (CEdit*) GetDlgItem ( IDC_eMsg ) )->GetWindowText(strText);
	
	strLen.Format(_T("%d / 80 bytes"), strText.GetLength() );

	( (CEdit*) GetDlgItem ( IDC_eLen ) )->SetWindowText(strLen);	
}


VOID CSMS_06::RemoveSMSList()
{
	m_pSMSList->msg.RemoveAll();
	m_pSMSList->msg.RemoveAll();
	m_pSMSList->receiverName.RemoveAll();
	m_pSMSList->receiverNum.RemoveAll();
	m_pSMSList->pSMSSender = NULL;
	m_pSMSList->title.Empty();
}

VOID CSMS_06::SetHandphoneNumber(CString strNum)
{
	m_strPhoneNumber = strNum;	
}

VOID CSMS_06::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	( (CEdit*) GetDlgItem (IDC_eMsg) )->ShowWindow(SW_SHOW);
}

HBRUSH CSMS_06::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}