// ErrorAdjustDoc.cpp : implementation file
//

#include "stdafx.h"
#include "ErrorAdjustDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CErrorAdjustDoc

IMPLEMENT_DYNCREATE(CErrorAdjustDoc, CDocument)

CErrorAdjustDoc::CErrorAdjustDoc()
{
}

BOOL CErrorAdjustDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CErrorAdjustDoc::~CErrorAdjustDoc()
{
}


BEGIN_MESSAGE_MAP(CErrorAdjustDoc, CDocument)
	//{{AFX_MSG_MAP(CErrorAdjustDoc)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CErrorAdjustDoc diagnostics

#ifdef _DEBUG
VOID CErrorAdjustDoc::AssertValid() const
{
	CDocument::AssertValid();
}

VOID CErrorAdjustDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CErrorAdjustDoc serialization

VOID CErrorAdjustDoc::Serialize(CArchive& ar)
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
// CErrorAdjustDoc commands
