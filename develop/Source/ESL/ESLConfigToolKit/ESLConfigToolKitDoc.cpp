// ESLConfigToolKitDoc.cpp : implementation of the CESLConfigToolKitDoc class
//

#include "stdafx.h"
#include "ESLConfigToolKit.h"

#include "ESLConfigToolKitDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CESLConfigToolKitDoc

IMPLEMENT_DYNCREATE(CESLConfigToolKitDoc, CDocument)

BEGIN_MESSAGE_MAP(CESLConfigToolKitDoc, CDocument)
	//{{AFX_MSG_MAP(CESLConfigToolKitDoc)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESLConfigToolKitDoc construction/destruction

CESLConfigToolKitDoc::CESLConfigToolKitDoc()
{
	// TODO: add one-time construction code here

}

CESLConfigToolKitDoc::~CESLConfigToolKitDoc()
{
}

BOOL CESLConfigToolKitDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CESLConfigToolKitDoc serialization

VOID CESLConfigToolKitDoc::Serialize(CArchive& ar)
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
// CESLConfigToolKitDoc diagnostics

#ifdef _DEBUG
VOID CESLConfigToolKitDoc::AssertValid() const
{
	CDocument::AssertValid();
}

VOID CESLConfigToolKitDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CESLConfigToolKitDoc commands
