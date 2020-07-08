// ServerSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ServerMgr.h"
#include "ServerSocket.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSocket

CServerSocket::CServerSocket()
{
	CAsyncSocket::CAsyncSocket();
	// KOL.UDF.022 시큐어코딩 보완
	m_pHwnd = NULL;
}

CServerSocket::~CServerSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerSocket member functions



/////////////////////////////////////////////////////////////////////////////
//*************************메시지를 전달 할 윈도우*************************//
void CServerSocket::SetWnd(HWND hwnd)
{
	m_pHwnd=hwnd;
}

////////////////////////////////////////////////////////////////////////////
//******************클라이언트의 접속요청을 받는 함수*********************//
void CServerSocket::OnAccept(int nErrorCode) 
{
	//클라이언트가 접속 했을 경우 처리 함수 호출
	::SendMessage(m_pHwnd,WM_ACCEPT_CLIENT,0,0);
}