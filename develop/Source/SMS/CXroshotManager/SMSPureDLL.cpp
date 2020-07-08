#include "stdafx.h"
#include "SMSPureDLL.h"

SMSPureDLL::SMSPureDLL()
{
	// KOL.UDF.022 시큐어코딩 보완
	m_hPureDLL = NULL;
	m_lpSMSOpen = 0;
	m_lpSMSClose = 0;
	m_lpSMSSendText = 0;
	m_lpSMSBind = 0;
}

SMSPureDLL::~SMSPureDLL()
{
}

bool SMSPureDLL::Init()
{
	m_hPureDLL = LoadLibrary(_T("..\\DLL\\mono\\PureDLL.DLL"));

	if (m_hPureDLL)
	{
		m_lpSMSOpen = (LPSMSOPEN)GetProcAddress(m_hPureDLL, "SMSOpen");
		m_lpSMSClose = (LPSMSCLOSE)GetProcAddress(m_hPureDLL, "SMSClose");
		m_lpSMSBind = (LPSMSBIND)GetProcAddress(m_hPureDLL, "SMSBind");
		m_lpSMSSendText = (LPSMSSENDTEXT)GetProcAddress(m_hPureDLL, "SMSSendText");
	}
	return true;
}

bool SMSPureDLL::UnInit()
{
	if (NULL != m_hPureDLL)
		FreeLibrary(m_hPureDLL);

	return true;
}


INT SMSPureDLL::SMSOpen(TCHAR* pIPAddress, unsigned short nPort)
{
	if (NULL != m_lpSMSOpen)
		return m_lpSMSOpen(pIPAddress, nPort);
	else
		return -1;
}

INT SMSPureDLL::SMSClose()
{
	if (NULL != m_lpSMSClose)
		return m_lpSMSClose();
	else
		return -1;
}

INT SMSPureDLL::SMSBind(TCHAR* userid, TCHAR* userpwd)
{
	if (NULL != m_lpSMSBind)
	{
		m_lpSMSBind(userid, userpwd);
		return 0;
	}
	else
		return -1;
}

INT SMSPureDLL::SMSSendText(TCHAR* strCallNo, TCHAR* strCallBack, TCHAR* strCaller, TCHAR* strResvDate, TCHAR* strResvTime, TCHAR* strSerialNo, TCHAR* strMsg)
{
	if (NULL != m_lpSMSSendText)
		return m_lpSMSSendText(strCallNo, strCallBack, strCaller, strResvDate, strResvTime, strSerialNo, strMsg);
	else
		return -1;
}
