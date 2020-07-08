// LoanShareWaitMessage.cpp : implementation file
//

#include "stdafx.h"
#include "LoanShareWaitMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoanShareWaitMessage dialog


CLoanShareWaitMessage::CLoanShareWaitMessage(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoanShareWaitMessage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CLoanShareWaitMessage::~CLoanShareWaitMessage()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CLoanShareWaitMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanShareWaitMessage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoanShareWaitMessage, CDialog)
	//{{AFX_MSG_MAP(CLoanShareWaitMessage)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanShareWaitMessage message handlers

BOOL CLoanShareWaitMessage::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN
		
		
		return CDialog::Create(IDD, pParentWnd);
	
	
	EFS_END
		return FALSE;
	
}
INT CLoanShareWaitMessage::SetRemainCnt( INT nRemainCnt )
{
	EFS_BEGIN

	GetDlgItem(IDC_staGEMESSAGE)->SetWindowText( _T("대출 공유에 관련된 작업이 진행중입니다. \r\n완료될때까지 잠시만 기다려주십시요.") );
	

	CString sMsg;
	sMsg.Format( _T("남은 작업 수 : %d개") ,  nRemainCnt );

	GetDlgItem(IDC_staREMAIN_COUNT)->SetWindowText( sMsg );


	return 0;

	EFS_END

	return -1;
}

BOOL CLoanShareWaitMessage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CLoanShareWaitMessage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// 여기까지 추가

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
