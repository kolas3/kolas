// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "LoanReturnControl.h"
#include "ClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket()
{
	// KOL.UDF.022
	m_hParentWnd = NULL;
}

CClientSocket::~CClientSocket()
{

}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

void CClientSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnAccept(nErrorCode);
}

void CClientSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnConnect(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnClose(nErrorCode);
}

void CClientSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class	
	CSocket::OnReceive(nErrorCode);

	CHAR szBuff[10000];
	memset(szBuff, 0x00, sizeof(szBuff));
	Receive(szBuff, sizeof(szBuff));
	SendMessage(m_hParentWnd, WM_MSG_RECEIVE, m_hSocket, (LPARAM)szBuff);
}

void CClientSocket::OnSend(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnSend(nErrorCode);
}

void CClientSocket::SetParentHandle(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
}
