// SMS_5130.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_5130.h"

#include "../../../SMS/CO_SMS/SMS_MakeMsg.h"
#include "../../../SMS/CO_SMS/SMS_Env_Info.h"

#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_5130 dialog


CSMS_5130::CSMS_5130(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_5130)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSMS_5130::~CSMS_5130()
{
}

VOID CSMS_5130::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_5130)
	DDX_Control(pDX, IDC_expSEARCH, m_WebBrowser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_5130, CDialog)
	//{{AFX_MSG_MAP(CSMS_5130)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_radOPTION1, OnradOPTION1)
	ON_BN_CLICKED(IDC_radOPTION2, OnradOPTION2)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_5130 message handlers

BOOL CSMS_5130::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CSMS_5100* pSMS = (CSMS_5100*)this->pParentMgr;
	
	ChangeScreen( pSMS->m_strSMSType );

	InitScreen();
	
	ShowHistory(0);
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CSMS_5130::ShowHistory( INT nMode )
{
	EFS_BEGIN

	CString sNURL =	_T("history.html");
	SMS_CONNECTION_INFO SMS_ConnectionInfo;
	SMS_MSG_INFO SMS_MsgInfo;
	
	CSMS_Env_Info SMSEnvInfo(this);
	SMSEnvInfo.MakeSMSEnv();
	SMSEnvInfo.GetSMSEnv( _T("SMS01") , &SMS_ConnectionInfo , &SMS_MsgInfo );
	
	CString sHtmlData;
	CString shtml;
	CSMS_5100* pSMS = (CSMS_5100*)this->pParentMgr;

	if( _T("0") == pSMS->m_strSMSType )
	{
		CString sURL[2] =
		{
			 _T("http://pro.xroshot.com/eco/htm/charge/default.jsp") , 
			 _T("http://pro.xroshot.com/eco/htm/result/default.jsp")
		};
		
		shtml = _T("")
				_T("<html>")
				_T("<body onload=login_form.submit()>")
				_T("<form name=login_form method=post action='%s'>")
				_T("<input type=hidden name=code value='%s'>")
				_T("<input type=hidden name=userid value='%s'>")
				_T("<input type=hidden name=domain value='%s'>")
				_T("</body>")
				_T("</form>")
				_T("</html>");
		
		sHtmlData.Format( shtml , sURL[nMode] , 
			SMS_ConnectionInfo.sUserPhoneId , 
			SMS_ConnectionInfo.sUserId , 
			SMS_ConnectionInfo.sUserDomain );
	}
	else if( _T("5") == pSMS->m_strSMSType )
	{
		shtml = 
			_T("")
			_T("<html>")
			_T("<SCRIPT SRC=\"http://rpt.messageplus.co.kr/js/md5.js\" TYPE=\"text/javascript\"></SCRIPT>")
			_T("<script language=\"JavaScript\" type=\"text/JavaScript\">")
			_T("function fn_sendIt(){")
			_T("FmLogin.USER_PWD.value = hex_md5(FmLogin.USER_PWD.value);")
			_T("FmLogin.submit();")
			_T("}")
			_T("</script>")
			_T("<body onload=\"fn_sendIt();\">")
			_T("<form name=\"FmLogin\" action=\"http://rpt.messageplus.co.kr/pages/login_Check.jsp\" method=\"post\">")
			_T("<input type=\"hidden\" value=\"rpt\" name=\"p_company_code\">")
			_T("<input type=\"hidden\" name=\"l_type\" value=\"10\">")
			_T("<input type=hidden name=USER_ID value=\"%s\">")
			_T("<input type=hidden name=USER_PWD value=\"%s\">")
			_T("</body>")
			_T("</form>")
			_T("</html>");

		sHtmlData.Format( shtml, SMS_ConnectionInfo.sBindId, SMS_ConnectionInfo.sBindPwd );
	}
	
	CStdioFile sfFile;
	INT ids = sfFile.Open( sNURL , CFile::modeCreate | CFile::modeWrite | CFile::typeBinary );
	if( !ids ) 
	{
		AfxMessageBox( _T("파일을 만드는데 실패했습니다.") );
		
		return 0;
	}
	
	if( 0 == sfFile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		sfFile.Write( &cUni, sizeof(TCHAR));
	}
	
	CString historyFilePath = sfFile.GetFilePath();
	
	sfFile.WriteString( sHtmlData );
	sfFile.Close();

	m_WebBrowser.Navigate( historyFilePath , NULL , NULL , NULL , NULL  );
	
	return 0;

	EFS_END
	
	return -1;
}

VOID CSMS_5130::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if( GetSafeHwnd() == NULL )	 return;


	if( GetDlgItem(IDC_expSEARCH)->GetSafeHwnd() == NULL ) return;

	GetDlgItem(IDC_expSEARCH)->MoveWindow( 10 , 40 , cx-20 , cy - 50 );

	CSMS_5100* pSMS = (CSMS_5100*)this->pParentMgr;	
	ChangeScreen( pSMS->m_strSMSType );

}

INT CSMS_5130::InitScreen()
{
	EFS_BEGIN


	((CButton*)GetDlgItem(IDC_radOPTION1))->SetCheck(TRUE);


	return 0;

	EFS_END

	return -1;

}

VOID CSMS_5130::OnradOPTION1() 
{
	ShowHistory(0);	
}

VOID CSMS_5130::OnradOPTION2() 
{
	ShowHistory(1);	
}

VOID CSMS_5130::OnbtnCLOSE() 
{
	OnCancel();	
}

INT CSMS_5130::ChangeScreen(CString strSMSType)
{
	if( _T("5") == strSMSType )
	{
		GetDlgItem(IDC_STA_DESC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_radOPTION1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_radOPTION2)->ShowWindow(SW_HIDE);

		CRect	 rc, rcBtn;		
		
		CWnd* pBtn = GetDlgItem(IDC_btnCLOSE);

		this->GetClientRect( rc );
		pBtn->GetClientRect( rcBtn );

		pBtn->MoveWindow( (rc.right-rcBtn.Width())-8, rc.top+7, rcBtn.Width(), rcBtn.Height() );
	}
	return 0;
}

HBRUSH CSMS_5130::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}