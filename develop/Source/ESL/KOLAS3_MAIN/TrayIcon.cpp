// TrayIcon.cpp: implementation of the CTrayIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "TrayIcon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrayIcon::CTrayIcon()
{
	m_bHidden = FALSE;
}

CTrayIcon::~CTrayIcon()
{

}

//////////////////////////////////////////////////////////////////////
// CTrayIcon Operations
BOOL CTrayIcon::Create(CWnd *pWnd, UINT uCallBackMessage, LPCTSTR szToolTip, HICON icon, UINT uID)
{
	m_tnd.cbSize = sizeof(NOTIFYICONDATA);
	m_tnd.hWnd = pWnd->GetSafeHwnd();
	m_tnd.uID = uID;
	m_tnd.hIcon = icon;
	m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_tnd.uCallbackMessage = uCallBackMessage;
	_tcscpy(m_tnd.szTip, szToolTip);

	return Shell_NotifyIcon(NIM_ADD, &m_tnd);
}

BOOL CTrayIcon::SetIcon(HICON hIcon)
{
	m_tnd.uFlags = NIF_ICON;
	m_tnd.hIcon = hIcon;

	return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

BOOL CTrayIcon::SetIcon(LPCTSTR lpszIconName)
{
	HICON hIcon = AfxGetApp()->LoadIcon(lpszIconName);
	
	return SetIcon(hIcon);
}

BOOL CTrayIcon::SetIcon(UINT nIDResource)
{
	HICON hIcon = AfxGetApp()->LoadIcon(nIDResource);

	return SetIcon(hIcon);
}

BOOL CTrayIcon::SetToolTipText(LPCTSTR pszToolTipText)
{
	m_tnd.uFlags = NIF_TIP;
	_tcscpy(m_tnd.szTip, pszToolTipText);

	return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

VOID CTrayIcon::HideIcon()
{
    if (!m_bHidden) 
	{
        m_tnd.uFlags = NIF_ICON;
        Shell_NotifyIcon (NIM_DELETE, &m_tnd);
        m_bHidden = TRUE;
    }
}

VOID CTrayIcon::ShowIcon()
{
    if (m_bHidden) 
	{
        m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
        Shell_NotifyIcon(NIM_ADD, &m_tnd);
        m_bHidden = FALSE;
    }
}

VOID CTrayIcon::RemoveIcon()
{
    m_tnd.uFlags = 0;
    Shell_NotifyIcon(NIM_DELETE, &m_tnd);
}
