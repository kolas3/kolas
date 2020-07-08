// TitleString.cpp: implementation of the CTitleString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TitleString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTitleString::CTitleString( CWnd *lpWnd,CString sMsg)
{
	m_pWnd = lpWnd ;
    m_pWnd->GetWindowText (m_Caption );
	m_pWnd->SetWindowText (sMsg);
    
}

CTitleString::~CTitleString()
{
	m_pWnd->SetWindowText(m_Caption);

}
