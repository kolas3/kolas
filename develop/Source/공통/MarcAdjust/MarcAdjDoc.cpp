// MarcAdjDoc.cpp : implementation file
//

#include "stdafx.h"
#include "MarcAdjDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjDoc

IMPLEMENT_DYNCREATE(CMarcAdjDoc, CDocument)

CMarcAdjDoc::CMarcAdjDoc()
{
}

BOOL CMarcAdjDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CMarcAdjDoc::~CMarcAdjDoc()
{
}


BEGIN_MESSAGE_MAP(CMarcAdjDoc, CDocument)
	//{{AFX_MSG_MAP(CMarcAdjDoc)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjDoc diagnostics

#ifdef _DEBUG
VOID CMarcAdjDoc::AssertValid() const
{
	CDocument::AssertValid();
}

VOID CMarcAdjDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjDoc serialization

VOID CMarcAdjDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjDoc commands
