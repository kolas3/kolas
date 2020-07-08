// NoticeMessage.cpp : implementation file
// 

#include "stdafx.h"
#include "NoticeMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define BLINK_TIME 100
#define END_TIME 5000

/////////////////////////////////////////////////////////////////////////////
// CNoticeMessage dialog


CNoticeMessage::CNoticeMessage(CWnd* pParent /*=NULL*/)
	: CDialog(CNoticeMessage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNoticeMessage)
	//}}AFX_DATA_INIT

	m_pParent = pParent;

	HINSTANCE m_hREInstance;     
	m_hREInstance = LoadLibrary(_T("RICHED32.DLL"));

	m_nDelayTime = 0;
	// KOL.UDF.022
	m_sMsg = _T("");
	m_nMode = 0;
	m_nChange = 0;
}


void CNoticeMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNoticeMessage)
	DDX_Control(pDX, IDC_redtMESSAGE, m_CtrlMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNoticeMessage, CDialog)
	//{{AFX_MSG_MAP(CNoticeMessage)
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoticeMessage message handlers

BOOL CNoticeMessage::OnInitDialog() 
{
	EFS_BEGIN		

	CDialog::OnInitDialog();

	
	
	m_nChange = 0;

	SetTimer( 0 , BLINK_TIME , NULL );

	// 2006-03-27 UPDATE BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 메세지 박스의 시간을 설정할수 있도록 변경
	// Default
	if ( !m_nDelayTime )	SetTimer( 1 , END_TIME , NULL );
	else                    SetTimer( 1 , m_nDelayTime , NULL );
	// ------------------------------------------------------------------------------------------------------
	SetTimer( 2 , 1 , NULL );
	SetTimer( 3 , 100 , NULL );
	
	CRect PrePos;
	GetDlgItem(IDC_redtMESSAGE)->GetWindowRect(PrePos);
	ScreenToClient(PrePos);
	if( m_nMode ==0 )
		GetDlgItem(IDC_redtMESSAGE)->MoveWindow(PrePos.left,15,PrePos.Width(),PrePos.Height());
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALS

	EFS_END
	return FALSE;
}

void CNoticeMessage::OnTimer(UINT nIDEvent) 
{
	EFS_BEGIN

	if( nIDEvent == 0 )
	{
		ShowMessage();
		KillTimer(0);
		return;
	}
	else if( nIDEvent == 1 )
	{
		KillTimer(1);
		KillTimer(0);
		KillTimer(2);
		KillTimer(3);
		OnClose();
		return;
	}
	else if( nIDEvent == 2 )
	{
		KillTimer(2);
		if( m_pParent->GetSafeHwnd() == NULL )
		{
			KillTimer(1);
			KillTimer(0);
			KillTimer(2);
			KillTimer(3);
			OnClose();
			return;
		}
		SetTimer(2,1,NULL );
		return;
	}
	else if( nIDEvent == 3 )
	{
		m_CtrlMessage.SetWindowText(m_sMsg);
		return;
	}

	CDialog::OnTimer(nIDEvent);

	EFS_END
}

void CNoticeMessage::OnLButtonUp(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	KillTimer(1);
	KillTimer(0);
	KillTimer(2);
	KillTimer(3);	
	OnClose();
	
	CDialog::OnLButtonUp(nFlags, point);

	EFS_END
}

BOOL CNoticeMessage::Create( CWnd *pParentWnd )
{
	EFS_BEGIN

	CDialog::Create(IDD , pParentWnd );

	return TRUE;

	EFS_END
	return FALSE;

}

void CNoticeMessage::SetMsg( CString sMsg , int nMode )
{
	EFS_BEGIN

//	ReSize( sMsg );
	m_sMsg = sMsg;

	m_nMode = nMode;

	EFS_END
}

void CNoticeMessage::ShowMessage()
{
	EFS_BEGIN

	if( m_nChange == 0 )
	{
		m_CtrlMessage.SetWindowText(m_sMsg);
		m_nChange = 0;
	}
	else
	{
		m_CtrlMessage.SetWindowText( _T("") );
		m_nChange = 0;
		return ;
	}

	
	CHARFORMAT cf;
	cf.crTextColor = RGB(100,100,50);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;
	cf.yHeight = 230;
	cf.dwEffects = CFE_PROTECTED | CFE_BOLD;
	_tcscpy( (LPTSTR)(LPCTSTR)cf.szFaceName , _T("굴림") );
	/*
	CHARFORMAT cf;
	cf.crTextColor = RGB(112,112,56);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;
	cf.yHeight = 200;
	cf.dwEffects = CFE_PROTECTED;
	strcpy( cf.szFaceName , "굴림" );
	*/
	
	m_CtrlMessage.SetDefaultCharFormat(cf);
	/*
	COLORREF BackColor;
	BackColor = RGB(0,0,0);
	m_CtrlMessage.SetBackgroundColor( false , BackColor );
	*/

	EFS_END
}

void CNoticeMessage::ReSize( CString sMsg )
{
	EFS_BEGIN

	int nCount = GetRetrunCount( sMsg );
	
	MoveWindow( 0 , 0 , 300 , (nCount+2)*20 );
	m_CtrlMessage.MoveWindow( 0 , 0 , 300 , (nCount+2)*20 );

	EFS_END
}

int CNoticeMessage::GetRetrunCount( CString sMsg )
{
	EFS_BEGIN

	int nIndex = sMsg.Find( _T("\r\n") , 0 );

	if( nIndex < 0 ) return 0;

	int nCount = 1;
	while( true )
	{
		nIndex = sMsg.Find( _T("\r\n") , 0 );
		if( nIndex > -1 )nCount++;
		else break;
	}

	return nCount;

	EFS_END
	return -1;

}

BOOL CNoticeMessage::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
	{
		KillTimer(1);
		KillTimer(0);
		KillTimer(2);
		KillTimer(3);
		OnClose();
		return TRUE;
	}	
	return CDialog::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;
}

BOOL CNoticeMessage::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}


void CNoticeMessage::OnClose() 
{
	EFS_BEGIN

	KillTimer(1);
	KillTimer(0);
	KillTimer(2);
	KillTimer(3);
	DestroyWindow();

	EFS_END
}
