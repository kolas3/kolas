// ErrorQueue.cpp: implementation of the CErrorQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ErrorQueue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CErrorQueue::CErrorQueue()
{

}

CErrorQueue::~CErrorQueue()
{
	CErrorMessage* pErrorMessage = NULL;

	INT iMaxCount = m_arrErrorMessage.GetSize();
	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		pErrorMessage = m_arrErrorMessage.GetAt(i);
		delete pErrorMessage;
	}

	m_arrErrorMessage.RemoveAll();
	m_arrErrorMessage.FreeExtra();
}

INT CErrorQueue::AddErrorMessage(INT iLine, CString sErrorMessage, CString sSolution)
{
	CErrorMessage* pErrorMessage = new CErrorMessage(iLine,sErrorMessage,sSolution);
	if(pErrorMessage == NULL) return -1;

	for (INT i=0; i< m_arrErrorMessage.GetSize(); i++)
	{
		CErrorMessage *pMsg= m_arrErrorMessage.GetAt(i);
		if (iLine< pMsg->GetLine())
		{
			m_arrErrorMessage.InsertAt(i, pErrorMessage);
			return 0;
		}
	}
	m_arrErrorMessage.Add(pErrorMessage);
	return 0;
}


INT CErrorQueue::GetErrorCount()
{
	return m_arrErrorMessage.GetSize();
}

CString CErrorQueue::GetLine(INT iIdx)
{
	INT iLine = m_arrErrorMessage.GetAt(iIdx)->GetLine();
	CString sLine;
	sLine.Format(_T("%i"),iLine);
	return sLine;
}

CString CErrorQueue::GetErrorMessage(INT iIdx)
{
	return m_arrErrorMessage.GetAt(iIdx)->GetErrorMessage();
}

CString CErrorQueue::GetSolution(INT iIdx)
{
	return m_arrErrorMessage.GetAt(iIdx)->GetSolution();
}

VOID CErrorQueue::ResetContent()
{
	CErrorMessage* pErrorMessage = NULL;

	INT iMaxCount = m_arrErrorMessage.GetSize();
	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		pErrorMessage = m_arrErrorMessage.GetAt(i);
		delete pErrorMessage;
	}

	m_arrErrorMessage.RemoveAll();
	m_arrErrorMessage.FreeExtra();
}


