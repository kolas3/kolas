// ServerSocket.cpp : implementation file
//

#include "stdafx.h"
#include "KLRFIDService.h"
#include "KLServerSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKLServerSocket

CKLServerSocket::CKLServerSocket()
{
	m_pClientSocket = NULL;
	// KOL.UDF.022
	m_hParentWnd = NULL;
}

CKLServerSocket::~CKLServerSocket()
{
	if(m_pClientSocket)
	{
		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CKLServerSocket, CSocket)
	//{{AFX_MSG_MAP(CKLServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CKLServerSocket member functions

void CKLServerSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CSocket::OnAccept(nErrorCode);

	if(m_pClientSocket)
	{
		m_pClientSocket->Close();
		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}
	m_pClientSocket = new CKLClientSocket;
	m_pClientSocket->SetParentHandle(m_hParentWnd);

	Accept(*((CAsyncSocket*)m_pClientSocket));
}

void CKLServerSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnClose(nErrorCode);
}

void CKLServerSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnConnect(nErrorCode);
}

void CKLServerSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnReceive(nErrorCode);
}

void CKLServerSocket::OnSend(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnSend(nErrorCode);
}

int CKLServerSocket::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CSocket::Receive(lpBuf, nBufLen, nFlags);
}

int CKLServerSocket::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	return m_pClientSocket->Send(lpBuf, nBufLen, nFlags);
// 	return CSocket::Send(lpBuf, nBufLen, nFlags);
}

void CKLServerSocket::SetParentHandle(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
}
