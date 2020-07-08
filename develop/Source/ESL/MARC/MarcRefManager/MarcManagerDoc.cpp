// MarcManagerDoc.cpp : implementation of the CMarcManagerDoc class
//

#include "stdafx.h"
#include "MarcManager.h"

#include "MarcManagerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerDoc

IMPLEMENT_DYNCREATE(CMarcManagerDoc, CDocument)

BEGIN_MESSAGE_MAP(CMarcManagerDoc, CDocument)
	//{{AFX_MSG_MAP(CMarcManagerDoc)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerDoc construction/destruction

CMarcManagerDoc::CMarcManagerDoc()
{
	// TODO: add one-time construction code here

}

CMarcManagerDoc::~CMarcManagerDoc()
{
}

BOOL CMarcManagerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMarcManagerDoc serialization

VOID CMarcManagerDoc::Serialize(CArchive& ar)
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
// CMarcManagerDoc diagnostics

#ifdef _DEBUG
VOID CMarcManagerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

VOID CMarcManagerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerDoc commands
