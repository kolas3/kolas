// Msg.cpp: implementation of the CFrameCtl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrameCtl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFrameCtl::CFrameCtl()
{
	Init();
}

CFrameCtl::~CFrameCtl()
{

}

void CFrameCtl::Init()
{
	m_strText = _T("");
}

/////////////////////////////////////////////////////////////////////////////
// CFrameCtl serialization

void CFrameCtl::Serialize(CArchive& ar)
{	
	if (ar.IsStoring())
	{
		ar << m_strText;
	}
	else
	{
		ar >> m_strText;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFrameCtl diagnostics

#ifdef _DEBUG
void CFrameCtl::AssertValid() const
{
	CObject::AssertValid();
}

void CFrameCtl::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG
