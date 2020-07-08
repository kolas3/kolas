// ErrorMessage.cpp: implementation of the CErrorMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ErrorMessage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CErrorMessage::CErrorMessage()
{
	m_iLine	= 0;
	m_sErrorMessage.Empty();
	m_sSolution.Empty();
}


CErrorMessage::CErrorMessage(INT iLine, CString sErrorMessage, CString sSolution)
{
	m_iLine			= iLine;
	m_sErrorMessage = sErrorMessage;
	m_sSolution		= sSolution;
}

CErrorMessage::~CErrorMessage()
{

}

VOID CErrorMessage::SetMessageData(INT iLine, CString sErrorMessage, CString sSolution)
{
	m_iLine			= iLine;
	m_sErrorMessage = sErrorMessage;
	m_sSolution		= sSolution;
}

INT CErrorMessage::GetLine()
{
	return m_iLine;
}

CString CErrorMessage::GetErrorMessage()
{
	return m_sErrorMessage;
}

CString CErrorMessage::GetSolution()
{
	return m_sSolution;
}
