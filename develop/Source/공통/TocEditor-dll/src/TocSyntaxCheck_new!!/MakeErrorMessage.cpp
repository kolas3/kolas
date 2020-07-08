// MakeErrorMessage.cpp: implementation of the CMakeErrorMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MakeErrorMessage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakeErrorMessage::CMakeErrorMessage()
: m_MessageFile(m_sMessageFileName.GetBuffer(0),CFile::modeRead | CFile::shareDenyWrite)
{
	
}

CMakeErrorMessage::~CMakeErrorMessage()
{

}

CMakeErrorMessage* CMakeErrorMessage::GetErrorMessage(INT iErrorType, INT iLine, CString sTag)
{
	
}
