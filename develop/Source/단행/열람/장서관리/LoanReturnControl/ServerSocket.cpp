// ServerSocket.cpp : implementation file
//

#include "stdafx.h"
#include "LoanReturnControl.h"
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
	m_pClientSocket = NULL;
	m_hParentWnd = NULL;
}

CServerSocket::~CServerSocket()
{
	if(m_pClientSocket)
	{
		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerSocket, CSocket)
	//{{AFX_MSG_MAP(CServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerSocket member functions

void CServerSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CSocket::OnAccept(nErrorCode);

	if(m_pClientSocket)
	{
		m_pClientSocket->Close();
		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}
	m_pClientSocket = new CClientSocket;
	m_pClientSocket->SetParentHandle(m_hParentWnd);

	Accept(*((CAsyncSocket*)m_pClientSocket));
}

void CServerSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnClose(nErrorCode);
}

void CServerSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnConnect(nErrorCode);
}

void CServerSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnReceive(nErrorCode);
}

void CServerSocket::OnSend(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnSend(nErrorCode);
}

int CServerSocket::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CSocket::Receive(lpBuf, nBufLen, nFlags);
}

int CServerSocket::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	return m_pClientSocket->Send(lpBuf, nBufLen, nFlags);
// 	return CSocket::Send(lpBuf, nBufLen, nFlags);
}

void CServerSocket::SetParentHandle(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
}
