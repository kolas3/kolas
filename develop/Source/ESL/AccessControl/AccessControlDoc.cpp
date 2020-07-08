// AccessControlDoc.cpp : implementation of the CAccessControlDoc class
//

#include "stdafx.h"
#include "AccessControl.h"

#include "AccessControlDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessControlDoc

IMPLEMENT_DYNCREATE(CAccessControlDoc, CDocument)

BEGIN_MESSAGE_MAP(CAccessControlDoc, CDocument)
	//{{AFX_MSG_MAP(CAccessControlDoc)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlDoc construction/destruction

CAccessControlDoc::CAccessControlDoc()
{
	// TODO: add one-time construction code here

}

CAccessControlDoc::~CAccessControlDoc()
{
}

BOOL CAccessControlDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAccessControlDoc serialization

VOID CAccessControlDoc::Serialize(CArchive& ar)
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
// CAccessControlDoc diagnostics

#ifdef _DEBUG
VOID CAccessControlDoc::AssertValid() const
{
	CDocument::AssertValid();
}

VOID CAccessControlDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAccessControlDoc commands
