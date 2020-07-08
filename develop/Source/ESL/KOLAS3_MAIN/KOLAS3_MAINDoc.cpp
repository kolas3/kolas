// KOLAS3_MAINDoc.cpp : implementation of the CKOLAS3_MAINDoc class
//

#include "stdafx.h"
#include "KOLAS3_MAIN.h"

#include "KOLAS3_MAINDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKOLAS3_MAINDoc

IMPLEMENT_DYNCREATE(CKOLAS3_MAINDoc, CDocument)

BEGIN_MESSAGE_MAP(CKOLAS3_MAINDoc, CDocument)
	//{{AFX_MSG_MAP(CKOLAS3_MAINDoc)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKOLAS3_MAINDoc construction/destruction

CKOLAS3_MAINDoc::CKOLAS3_MAINDoc()
{
	// TODO: add one-time construction code here

}

CKOLAS3_MAINDoc::~CKOLAS3_MAINDoc()
{
}

BOOL CKOLAS3_MAINDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CKOLAS3_MAINDoc serialization

VOID CKOLAS3_MAINDoc::Serialize(CArchive& ar)
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
// CKOLAS3_MAINDoc diagnostics

#ifdef _DEBUG
VOID CKOLAS3_MAINDoc::AssertValid() const
{
	CDocument::AssertValid();
}

VOID CKOLAS3_MAINDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKOLAS3_MAINDoc commands
