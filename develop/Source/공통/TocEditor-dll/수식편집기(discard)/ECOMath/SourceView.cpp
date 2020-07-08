// SourceView.cpp : implementation file
//

#include "stdafx.h"
#include "ECOMath.h"
#include "SourceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSourceView

IMPLEMENT_DYNCREATE(CSourceView, CEditView)

CSourceView::CSourceView()
{
}

CSourceView::~CSourceView()
{
}


BEGIN_MESSAGE_MAP(CSourceView, CEditView)
	//{{AFX_MSG_MAP(CSourceView)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSourceView diagnostics

#ifdef _DEBUG
VOID CSourceView::AssertValid() const
{
	CView::AssertValid();
}

VOID CSourceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CECOMathDoc* CSourceView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CECOMathDoc)));
	return (CECOMathDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSourceView message handlers

VOID CSourceView::UpdateView()
{
	CECOMathDoc* pDoc = GetDocument();
	struct _Cell *top, *current;
	CString msg = _T("$");
	CString blank;

	ASSERT_VALID(pDoc);

	top = (pDoc ->m_Math).GetFirstCell();
	current = top;

	while(current != NULL) {
		if(current -> cell != NULL) {
			switch((current ->cell)->GetObjectType()) {
				case BLANK_OBJECT :
					blank = (current ->cell) -> GetObject();
					if(blank.GetLength() > 0) {
						msg += _T(" ") + blank;
					}
					break;
				case ASCII_OBJECT :
					msg += _T(" ") + (current ->cell) -> GetObject();
					break;
				case GREEK_OBJECT :
					msg += _T(" ") + (current ->cell) -> GetObject();
					break;
				case RELATION_OBJECT :
					msg += _T(" ") + (current ->cell) -> GetObject();
					break;
				case BIOP_OBJECT :
					msg += _T(" ") + (current ->cell) -> GetObject();
					break;	
				case ARROW_OBJECT :
					msg += _T(" ") + (current ->cell) -> GetObject();
					break;
				case LOGICAL_OBJECT :
					msg += _T(" ") + (current ->cell) -> GetObject();
					break;
				case SET_OBJECT :
					msg += _T(" ") + (current ->cell) -> GetObject();
					break;
				case MISC_OBJECT :
					msg += _T(" ") + (current ->cell) -> GetObject();
					break;

				/////////////////////////////////////////////
				case FENCE_OBJECT :
					msg += _T(" ") + (current ->cell) -> GetObject();
					break;
				case FRACTION_OBJECT :
					msg += _T(" ") + (current ->cell) -> GetObject();
					break;
				case RADICAL_OBJECT :
					msg += _T(" ") + (current ->cell) -> GetObject();
					break;
				case SCRIPT_OBJECT :
					msg += _T(" ") + (current ->cell) -> GetObject();
					break;
				case SUM_OBJECT :
					msg += _T(" ") + (current ->cell) -> GetObject();
					break;
				case INTEGRAL_OBJECT :
					msg += _T(" ") + (current ->cell) -> GetObject();
					break;
				case BAR_OBJECT :
					msg += _T(" ") + (current ->cell) -> GetObject();
					break;
			}
		}

		current = current -> next;
	}

	msg += _T(" $");	

	((CView *)this) -> SetWindowText(msg);
}

BOOL CSourceView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style &= ~FWS_ADDTOTITLE;
	return CEditView::PreCreateWindow(cs);
}
