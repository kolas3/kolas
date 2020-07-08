// ECOMathView.cpp : implementation of the CECOMathView class
//

#include "stdafx.h"
#include "ECOMath.h"

#include "ECOMathDoc.h"
#include "ECOMathView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CECOMathView

IMPLEMENT_DYNCREATE(CECOMathView, CView)

BEGIN_MESSAGE_MAP(CECOMathView, CView)
	//{{AFX_MSG_MAP(CECOMathView)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECOMathView construction/destruction

CECOMathView::CECOMathView()
{
	// TODO: add construction code here
}

CECOMathView::~CECOMathView()
{
}

BOOL CECOMathView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~FWS_ADDTOTITLE;
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CECOMathView drawing

VOID CECOMathView::OnDraw(CDC* pDC)
{
	static INT x = 50, y = 50;
	CECOMathDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	
	struct _Cell *top;

	top = (pDoc ->m_Math).GetFirstCell();
	RECT rect;

	rect.bottom = 0;
	rect.left = 0;
	rect.right = 0;
	rect.top = 0;

	INT alpha = 0;
	POINT caretPoint;
	while(top != NULL) {
		INT max_x_pos = 0;
		CFenceObject *pFence = ((CFenceObject *)(top ->cell));
		CFenceObject *pFence_ReDraw = ((CFenceObject *)(top ->cell));
		if(((top ->cell) -> GetObjectType() == FENCE_OBJECT) && (pFence ->m_iType == 8)) {
			x += alpha + 20;
			INT max_height = 0;
			INT start_x = x;
			pFence = ((CFenceObject *)(top ->cell)) ->m_pRightFence;
			(top ->cell)->DrawObject(pDC, start_x, y - 20);
			(top ->cell)->GetSize(rect);
			alpha += rect.right;
			top = top -> next;
			alpha = 0;
			if(top == NULL) {
				AfxMessageBox(_T("top == NULL"));
				return;
			}
			while(top -> cell != pFence) {
				(top ->cell)->DrawObject(pDC, x + alpha, y);
				if(top == (pDoc ->m_Math).GetCurrentCell()) {
					HideCaret();
					caretPoint.x = x + alpha;
					caretPoint.y = y;
					SetCaretPos(caretPoint);
					ShowCaret();
				}
				(top ->cell)->GetSize(rect);
				max_height = (max_height > rect.top)? max_height : rect.top;
				alpha += rect.right;
				top = top -> next;
				if(top == NULL) break;
			} 
			
			max_x_pos = x + alpha;

			CBlankObject *rbrace = ((CFenceObject *)(top ->cell)) ->m_pRightBrace;
			alpha = 0;
			while(top -> cell != rbrace) {
				(top ->cell)->DrawObject(pDC, x + alpha, y - max_height - 30);
				if(top == (pDoc ->m_Math).GetCurrentCell()) {
					HideCaret();
					caretPoint.x = x + alpha;
					caretPoint.y = y - max_height - 30;
					SetCaretPos(caretPoint);
					ShowCaret();
				}
				(top ->cell)->GetSize(rect);
				alpha += rect.right;
				top = top -> next;
				if(top == NULL) break;
			} 

			if(max_x_pos < (x + alpha)) max_x_pos = x + alpha; 

			//////////////////////////////////////////////////////////////
			// Brace를 다시 그린다...................

			pFence_ReDraw ->SetSize((max_x_pos - x)/2, (max_x_pos - x)/2);
			pFence_ReDraw ->DrawObject(pDC, start_x, y - 20);

			x = max_x_pos + 10;
			alpha = 0;
		}
		if(((top ->cell) -> GetObjectType() == FENCE_OBJECT) && (pFence ->m_iType == 9)) {
			x += alpha + 20;
			INT max_height = 0;
			INT start_x = x;
			pFence = ((CFenceObject *)(top ->cell)) ->m_pRightFence;
			(top ->cell)->DrawObject(pDC, start_x, y + 30);
			(top ->cell)->GetSize(rect);
			alpha += rect.right;
			top = top -> next;
			alpha = 0;
			if(top == NULL) {
				AfxMessageBox(_T("top == NULL"));
				return;
			}
			while(top -> cell != pFence) {
				(top ->cell)->DrawObject(pDC, x + alpha, y);
				if(top == (pDoc ->m_Math).GetCurrentCell()) {
					HideCaret();
					caretPoint.x = x + alpha;
					caretPoint.y = y;
					SetCaretPos(caretPoint);
					ShowCaret();
				}
				(top ->cell)->GetSize(rect);
				max_height = (max_height > rect.top)? max_height : rect.top;
				alpha += rect.right;
				top = top -> next;
				if(top == NULL) break;
			} 
			
			max_x_pos = x + alpha;

			CBlankObject *rbrace = ((CFenceObject *)(top ->cell)) ->m_pRightBrace;
			alpha = 0;
			while(top -> cell != rbrace) {
				(top ->cell)->DrawObject(pDC, x + alpha, y - max_height + 60);
				if(top == (pDoc ->m_Math).GetCurrentCell()) {
					HideCaret();
					caretPoint.x = x + alpha;
					caretPoint.y = y - max_height + 60;
					SetCaretPos(caretPoint);
					ShowCaret();
				}
				(top ->cell)->GetSize(rect);
				alpha += rect.right;
				top = top -> next;
				if(top == NULL) break;
			} 

			if(max_x_pos < (x + alpha)) max_x_pos = x + alpha; 

			//////////////////////////////////////////////////////////////
			// Brace를 다시 그린다...................

			pFence_ReDraw ->SetSize((max_x_pos - x)/2, (max_x_pos - x)/2);
			pFence_ReDraw ->DrawObject(pDC, start_x, y + 30);

			x = max_x_pos + 10;
			alpha = 0;
		}
		else if(((top ->cell) -> GetObjectType() == FRACTION_OBJECT)) {
			x += alpha;
			INT max_height = 0;
			INT start_x = x + alpha;

			alpha = 0;
			CFractionObject *fraction = (CFractionObject *)(top ->cell);
			
			top = top ->next;
			while((top ->cell) ->GetObjectType() != FRACTION_OBJECT) {
				(top ->cell)->DrawObject(pDC, x + alpha, y - 10);
				if(top == (pDoc ->m_Math).GetCurrentCell()) {
					HideCaret();
					caretPoint.x = x + alpha;
					caretPoint.y = y - 10;
					SetCaretPos(caretPoint);
					ShowCaret();
				}
				(top ->cell)->GetSize(rect);
				max_height = (max_height > rect.top)? max_height : rect.top;
				alpha += rect.right;
				top = top -> next;
			}

			max_x_pos = x + alpha;

			CFractionObject *div = (CFractionObject *)(top ->cell);
			if(div ->m_bMiddle == true) {
				if(div ->m_iType == 0) {
					div -> DrawObject(pDC, x, y + 15);
					alpha = 0;
				}
				else if(div ->m_iType == 1) {
					div -> DrawObject(pDC, x + alpha, y + 15);
				}
				else if(div ->m_iType == 2) {
					div -> DrawObject(pDC, x + alpha, y + 15);
				}
			}

			top = top ->next;
			
			while((top ->cell) ->GetObjectType() != FRACTION_OBJECT) {
				(top ->cell)->DrawObject(pDC, x + alpha, y + 30);
				if(top == (pDoc ->m_Math).GetCurrentCell()) {
					HideCaret();
					caretPoint.x = x + alpha;
					caretPoint.y = y + 30;
					SetCaretPos(caretPoint);
					ShowCaret();
				}
				(top ->cell)->GetSize(rect);
				max_height = (max_height > rect.top)? max_height : rect.top;
				alpha += rect.right;
				top = top -> next;
			}

			if(max_x_pos < (x + alpha)) max_x_pos = x + alpha; 

			div ->SetSize((max_x_pos - x), (max_x_pos - x));

			if(div ->m_bMiddle == true) {
				if(div ->m_iType == 0) {
					div -> DrawObject(pDC, x, y + 15);
					alpha = 0;
				}
				else if(div ->m_iType == 1) {
					div -> DrawObject(pDC, max_x_pos, y + 15);
				}
				else if(div ->m_iType == 2) {
					div -> DrawObject(pDC, max_x_pos, y + 15);
				}
			}

			top = top -> next;

			x = max_x_pos;
			alpha = 0;
		}
		else if(((top ->cell) -> GetObjectType() == RADICAL_OBJECT)) {
			CRadicalObject *radical = (CRadicalObject *)(top ->cell);
			if(radical ->m_iType == 0) {
				x += alpha;
				INT max_height = 0;
				INT start_x = x + alpha;

				alpha = 20;

				radical = (CRadicalObject *)(top ->cell);

				radical ->DrawObject(pDC, start_x, y);
				x = start_x;

				top = top ->next;
				while((top ->cell) ->GetObjectType() != RADICAL_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha, y);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha;
						caretPoint.y = y;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					max_height = (max_height > rect.top)? max_height : rect.top;
					alpha += rect.right;
					top = top -> next;
				}

				max_x_pos = x + alpha;

				radical ->SetSize((max_x_pos - x - 20), (max_x_pos - x - 20));
				radical ->DrawObject(pDC, start_x, y);

				x = max_x_pos;
			}
			else {
				x += alpha;
				INT max_height = 0;
				INT start_x = x + alpha;

				alpha = 20;
			
				x = start_x;

				top = top ->next;
				while((top ->cell) ->GetObjectType() != RADICAL_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha, y - 10);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha;
						caretPoint.y = y - 10;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					max_height = (max_height > rect.top)? max_height : rect.top;
					alpha += rect.right;
					top = top -> next;
				}

				x = x + alpha;

				radical = (CRadicalObject *)(top ->cell);

				radical ->DrawObject(pDC, x, y);

				alpha = 20;

				top = top ->next;

				start_x = x;

				while((top ->cell) ->GetObjectType() != RADICAL_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha, y);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha;
						caretPoint.y = y;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					max_height = (max_height > rect.top)? max_height : rect.top;
					alpha += rect.right;
					top = top -> next;
				}

				max_x_pos = x + alpha;

				radical ->SetSize((max_x_pos - start_x - 20), (max_x_pos - start_x - 20));
				radical ->DrawObject(pDC, x, y);
			}

			top = top ->next;
			x = max_x_pos;
		}
		else if(((top ->cell) -> GetObjectType() == SCRIPT_OBJECT)) {
			CScriptObject *script = (CScriptObject *)(top ->cell);
			x += alpha;
			alpha = 20;
			if(script ->m_iType == 0) {
				top = top ->next;
				while((top ->cell) ->GetObjectType() != SCRIPT_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha, y - 10);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha;
						caretPoint.y = y - 10;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}

				//x = x + alpha;
			}
			else if(script ->m_iType == 1) {
				top = top ->next;
				alpha = 20;
				while((top ->cell) ->GetObjectType() != SCRIPT_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha, y + 10);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha;
						caretPoint.y = y + 10;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				//x = x + alpha;
			}
			else if(script ->m_iType == 2) {
				INT max_alpha;
				alpha = 20;
				top = top ->next;
				while((top ->cell) ->GetObjectType() != SCRIPT_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha, y - 10);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha;
						caretPoint.y = y - 10;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}

				max_alpha = alpha;
				alpha = 20;

				top = top -> next;

				while((top ->cell) ->GetObjectType() != SCRIPT_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha, y + 10);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha;
						caretPoint.y = y + 10;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}

				max_alpha = (max_alpha < alpha)? alpha:max_alpha;

				alpha = max_alpha;
			}

			top = top ->next;
			x = alpha + x;
		}
		else if(((top ->cell) -> GetObjectType() == SUM_OBJECT)) {
			INT start_x = x + alpha;

			CSumObject *sum = (CSumObject *)(top ->cell);

			x += alpha;
	
			if(sum ->m_iType == 0) {
				(top ->cell)->DrawObject(pDC, start_x, y);

				top = top -> next;

				alpha = 40;

				while((top ->cell) ->GetObjectType() != SUM_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha, y);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha;
						caretPoint.y = y;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}

				(top ->cell)->DrawObject(pDC, start_x, y);
				(top ->cell)->GetSize(rect);
				top = top -> next;
				x += alpha - 30;
				alpha = 0;
				
			}
			else if(sum ->m_iType == 1) {
				(top ->cell)->DrawObject(pDC, start_x, y);

				top = top ->next;
				while((top ->cell) ->GetObjectType() != SUM_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha + 10, y + 20);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha + 10;
						caretPoint.y = y + 20;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				top = top -> next;

				max_x_pos = x + alpha;
				alpha = 50;
				while((top ->cell) ->GetObjectType() != SUM_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha + 10, y);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha + 10;
						caretPoint.y = y;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				top = top -> next;

				x = (max_x_pos > (x + alpha)) ? max_x_pos : x + alpha;
			}
			else if(sum ->m_iType == 2) {
				(top ->cell)->DrawObject(pDC, x + alpha, y);

				top = top ->next;
				while((top ->cell) ->GetObjectType() != SUM_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha + 10, y + 20);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha + 10;
						caretPoint.y = y + 20;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				top = top -> next;

				max_x_pos = x + alpha;

				alpha = 20;
				while((top ->cell) ->GetObjectType() != SUM_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha + 10, y - 20);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha + 10;
						caretPoint.y = y - 20;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				top = top -> next;

				//x = (max_x_pos > (x + alpha)) ? max_x_pos : x + alpha;

				alpha = 20;
				while((top ->cell) ->GetObjectType() != SUM_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha + 10, y);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha + 10;
						caretPoint.y = y;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				top = top -> next;
				x = (max_x_pos > (x + alpha)) ? max_x_pos : x + alpha;
			}
			else if(sum ->m_iType == 3) {
				(top ->cell)->DrawObject(pDC, x + alpha, y);

				top = top ->next;
				while((top ->cell) ->GetObjectType() != SUM_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha + 20, y + 20);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha + 20;
						caretPoint.y = y + 20;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				top = top -> next;
				
				alpha = 20;
				while((top ->cell) ->GetObjectType() != SUM_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha + 40, y);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha + 40;
						caretPoint.y = y;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				top = top -> next;

				x = x + alpha;
				alpha = 0;
			}
			else if(sum ->m_iType == 4) {
				(top ->cell)->DrawObject(pDC, start_x, y);

				top = top ->next;
				alpha = 20;
				while((top ->cell) ->GetObjectType() != SUM_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha + 10, y + 20);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha + 10;
						caretPoint.y = y + 20;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				top = top -> next;

				max_x_pos = x + alpha;

				alpha = 20;
				while((top ->cell) ->GetObjectType() != SUM_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha + 10, y - 20);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha + 10;
						caretPoint.y = y - 20;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				top = top -> next;

				//x = (max_x_pos > (x + alpha)) ? max_x_pos : x + alpha;

				alpha = 20;
				while((top ->cell) ->GetObjectType() != SUM_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha + 40, y);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha + 40;
						caretPoint.y = y;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				top = top -> next;

				x = (max_x_pos > (x + alpha)) ? max_x_pos : x + alpha;
			}
		}
		else if(((top ->cell) -> GetObjectType() == INTEGRAL_OBJECT)) {
			CIntegralObject *integral = (CIntegralObject *)(top ->cell);

			if(integral -> m_iType == 0) {
				INT max_height = 0;
				INT start_x = x + alpha;

				(top ->cell)->DrawObject(pDC, start_x, y);

				alpha = 50;
				top = top ->next;
				while((top ->cell) ->GetObjectType() != INTEGRAL_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha, y);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha;
						caretPoint.y = y;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				top = top -> next;

				x = x + alpha - 20;
				alpha = 0;
			}
			else if(integral -> m_iType == 1) {
				INT max_height = 0;
				INT start_x = x + alpha;
				x += alpha;

				(top ->cell)->DrawObject(pDC, start_x, y);

				alpha = 20;
				top = top ->next;
				while((top ->cell) ->GetObjectType() != INTEGRAL_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha, y + 30);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha;
						caretPoint.y = y + 30;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				alpha = 20;
				top = top -> next;

				while((top ->cell) ->GetObjectType() != INTEGRAL_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha, y - 25);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha;
						caretPoint.y = y - 20;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				alpha = 50;
				top = top -> next;

				while((top ->cell) ->GetObjectType() != INTEGRAL_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha, y);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha;
						caretPoint.y = y;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				alpha += rect.right;
				top = top -> next;

				alpha -= 100;

				x = x + alpha;
			}
			else if(integral -> m_iType == 2) {
				INT max_height = 0;
				INT start_x = x + alpha;

				x += alpha;
				(top ->cell)->DrawObject(pDC, start_x, y);

				alpha = 20;
				top = top ->next;
				while((top ->cell) ->GetObjectType() != INTEGRAL_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha, y + 30);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha;
						caretPoint.y = y + 30;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				alpha = 20;
				top = top -> next;

				while((top ->cell) ->GetObjectType() != INTEGRAL_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha, y - 25);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha;
						caretPoint.y = y - 25;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				alpha = 50;
				top = top -> next;

				while((top ->cell) ->GetObjectType() != INTEGRAL_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha, y);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha;
						caretPoint.y = y;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				alpha += rect.right;
				top = top -> next;

				alpha -= 100;
				x = x + alpha;
			}			
		}
		else if(((top ->cell) -> GetObjectType() == BAR_OBJECT)) {
			CBarObject *bar = (CBarObject *)(top ->cell);

			if(bar -> m_iType == 0) {
				//x += alpha;
				INT max_height = 0;
				INT start_x = x + alpha;

				alpha = 0;

				(top ->cell)->DrawObject(pDC, start_x, y - 40);

				top = top ->next;
				alpha = 25;
				while((top ->cell) ->GetObjectType() != BAR_OBJECT) {
					(top ->cell)->DrawObject(pDC, start_x + alpha, y);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = start_x + alpha;
						caretPoint.y = y;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				top = top -> next;

				if(max_x_pos < (x + alpha)) max_x_pos = x + alpha; 

				max_x_pos = x + alpha;

				bar ->SetSize(alpha, alpha);
				bar ->DrawObject(pDC, start_x, y - 40);

				x = max_x_pos; // + 25;
				alpha = 0;
			}
			else if(bar -> m_iType == 1) {
				x += alpha;
				INT max_height = 0;
				INT start_x = x + alpha;

				(top ->cell)->DrawObject(pDC, x + alpha - 10, y);

				top = top ->next;
				while((top ->cell) ->GetObjectType() != BAR_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha - 5, y);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha - 10;
						caretPoint.y = y;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				top = top -> next;

				if(max_x_pos < (x + alpha)) max_x_pos = x + alpha; 

				//////////////////////////////////////////////////////////////
				// Brace를 다시 그린다...................

				bar ->SetSize((max_x_pos - x), (max_x_pos - x));
				bar ->DrawObject(pDC, start_x - 5, y);

				x = max_x_pos - 20;
				alpha = 0;
			}
			else if(bar -> m_iType == 2) {
				x += alpha;
				INT max_height = 0;
				INT start_x = x + alpha;

				(top ->cell)->DrawObject(pDC, x + alpha - 10, y - 20);

				top = top ->next;
				while((top ->cell) ->GetObjectType() != BAR_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha - 5, y);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha - 10;
						caretPoint.y = y;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				top = top -> next;

				if(max_x_pos < (x + alpha)) max_x_pos = x + alpha; 

				//////////////////////////////////////////////////////////////
				// Brace를 다시 그린다...................

				bar ->SetSize((max_x_pos - x), (max_x_pos - x));
				bar ->DrawObject(pDC, start_x - 5, y - 20);

				x = max_x_pos - 20;
				alpha = 0;
			}
			else if(bar -> m_iType == 3) {
				x += alpha;
				INT max_height = 0;
				INT start_x = x + alpha;

				(top ->cell)->DrawObject(pDC, start_x, y - 20);

				top = top ->next;
				while((top ->cell) ->GetObjectType() != BAR_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha, y);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha;
						caretPoint.y = y;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				top = top -> next;

				if(max_x_pos < (x + alpha)) max_x_pos = x + alpha; 

				//////////////////////////////////////////////////////////////
				// Brace를 다시 그린다...................

				bar ->SetSize((max_x_pos - x), (max_x_pos - x));
				bar ->DrawObject(pDC, start_x, y - 20);

				x = max_x_pos - 20;
				alpha = 0;
			}
			else if(bar -> m_iType == 4) {
				x += alpha;
				INT max_height = 0;
				INT start_x = x + alpha;

				(top ->cell)->DrawObject(pDC, x + alpha - 10, y + 20);

				top = top ->next;
				while((top ->cell) ->GetObjectType() != BAR_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha - 5, y);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha - 10;
						caretPoint.y = y;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				top = top -> next;

				if(max_x_pos < (x + alpha)) max_x_pos = x + alpha; 

				//////////////////////////////////////////////////////////////
				// Brace를 다시 그린다...................

				bar ->SetSize((max_x_pos - x), (max_x_pos - x));
				bar ->DrawObject(pDC, start_x - 5, y + 20);

				x = max_x_pos - 20;
				alpha = 0;
			}
			else if(bar -> m_iType == 5) {
				x += alpha;
				INT max_height = 0;
				INT start_x = x + alpha;

				(top ->cell)->DrawObject(pDC, start_x, y + 20);

				top = top ->next;
				while((top ->cell) ->GetObjectType() != BAR_OBJECT) {
					(top ->cell)->DrawObject(pDC, x + alpha, y);
					if(top == (pDoc ->m_Math).GetCurrentCell()) {
						HideCaret();
						caretPoint.x = x + alpha;
						caretPoint.y = y;
						SetCaretPos(caretPoint);
						ShowCaret();
					}
					(top ->cell)->GetSize(rect);
					alpha += rect.right;
					top = top -> next;
				}
				(top ->cell)->GetSize(rect);
				top = top -> next;

				if(max_x_pos < (x + alpha)) max_x_pos = x + alpha; 

				//////////////////////////////////////////////////////////////
				// Brace를 다시 그린다...................

				bar ->SetSize((max_x_pos - x), (max_x_pos - x));
				bar ->DrawObject(pDC, start_x, y + 20);

				x = max_x_pos - 20;
				alpha = 0;
			}
		}
		else {
			alpha = rect.right;
			x += alpha;

			(top ->cell)->DrawObject(pDC, x, y);
			if(top == (pDoc ->m_Math).GetCurrentCell()) {
				HideCaret();
				caretPoint.x = x;
				caretPoint.y = y;
				SetCaretPos(caretPoint);
				ShowCaret();
			}
			(top ->cell)->GetSize(rect);
			top = top -> next;
		}
	}

	x = 50;
	y = 50;
}

/////////////////////////////////////////////////////////////////////////////
// CECOMathView diagnostics

#ifdef _DEBUG
VOID CECOMathView::AssertValid() const
{
	CView::AssertValid();
}

VOID CECOMathView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CECOMathDoc* CECOMathView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CECOMathDoc)));
	return (CECOMathDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CECOMathView message handlers

INT CECOMathView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CBitmap* pBitmap = new CBitmap;
	pBitmap->LoadBitmap(IDB_CARET);
	CreateCaret(pBitmap);
	ShowCaret();

	return 0;
}

VOID CECOMathView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	if(nChar >= 'a' && nChar <= 'z') {
		//AfxMessageBox(_T("ascii"));
		((CECOMathDoc *)GetDocument()) ->ClickASCII(nChar);
	}
	else if(nChar >= 'A' && nChar <= 'Z') {
		//AfxMessageBox(_T("ASCII"));
		((CECOMathDoc *)GetDocument()) ->ClickASCII(nChar);
	}
	else if(nChar >= '0' && nChar <= '9') {
		//AfxMessageBox(_T("Number"));
		((CECOMathDoc *)GetDocument()) ->ClickASCII(nChar);
	}

	CView::OnChar(nChar, nRepCnt, nFlags);
}

VOID CECOMathView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	DWORD fdwConversion;
	DWORD fdwNewConversion;
	DWORD fdwSentence;
	
	switch(nChar) {
		case VK_UP :
			((CECOMathDoc *)GetDocument()) -> UpButtonProcess();
			break;
		case VK_DOWN :
			((CECOMathDoc *)GetDocument()) -> DownButtonProcess();
			break;
		case VK_LEFT :
			((CECOMathDoc *)GetDocument()) -> LeftButtonProcess();
			break;
		case VK_RIGHT :
			((CECOMathDoc *)GetDocument()) -> RightButtonProcess();
			break;
		case VK_HOME :
			((CECOMathDoc *)GetDocument()) -> HomeButtonProcess();
			break;
		case VK_END :
			((CECOMathDoc *)GetDocument()) -> EndButtonProcess();
			break;
		case VK_PRIOR :	// PageUp
			AfxMessageBox(_T("PGUP"));
			((CECOMathDoc *)GetDocument()) -> HomeButtonProcess();
			break;
		case VK_NEXT : // PageDonw
			AfxMessageBox(_T("PGDN"));
			((CECOMathDoc *)GetDocument()) -> EndButtonProcess();
			break;
		case VK_BACK :
			((CECOMathDoc *)GetDocument()) -> BackButtonProcess();
			break;
		case VK_DELETE :
			((CECOMathDoc *)GetDocument()) -> DeleteButtonProcess();
			break;
	}

	m_hIMC = ImmGetContext(::GetForegroundWindow());

	ImmGetConversionStatus(m_hIMC, &fdwConversion, &fdwSentence);
	fdwNewConversion = IME_CMODE_CHARCODE;
	ImmSetConversionStatus(m_hIMC, fdwNewConversion, fdwSentence);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

VOID CECOMathView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(bActivate) {
		DWORD fdwConversion;
		DWORD fdwNewConversion;
		DWORD fdwSentence;

		m_hIMC = ImmGetContext(::GetForegroundWindow());

		ImmGetConversionStatus(m_hIMC, &fdwConversion, &fdwSentence);
		fdwNewConversion = IME_CMODE_CHARCODE;
		ImmSetConversionStatus(m_hIMC, fdwNewConversion, fdwSentence);
	}
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
