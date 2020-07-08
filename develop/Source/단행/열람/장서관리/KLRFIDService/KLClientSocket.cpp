// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "KLRFIDService.h"
#include "KLClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKLClientSocket

CKLClientSocket::CKLClientSocket()
{
	// KOL.UDF.022
	m_hParentWnd = NULL;
}

CKLClientSocket::~CKLClientSocket()
{

}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CKLClientSocket, CSocket)
	//{{AFX_MSG_MAP(CKLClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CKLClientSocket member functions

void CKLClientSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnAccept(nErrorCode);
}

void CKLClientSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnConnect(nErrorCode);
}

void CKLClientSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnClose(nErrorCode);
}

void CKLClientSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class	
	CSocket::OnReceive(nErrorCode);

	CHAR szBuff[10000];
	memset(szBuff, 0x00, sizeof(szBuff));
	Receive(szBuff, sizeof(szBuff));
	SendMessage(m_hParentWnd, WM_MSG_RECEIVE, m_hSocket, (LPARAM)szBuff);
}

void CKLClientSocket::OnSend(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnSend(nErrorCode);
}

void CKLClientSocket::SetParentHandle(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
}
