// SE_HISTORY_VIEW.cpp : implementation file
//

#include "stdafx.h"
#include "SE_HISTORY_VIEW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_HISTORY_VIEW

IMPLEMENT_DYNCREATE(CSE_HISTORY_VIEW, CView)

CSE_HISTORY_VIEW::CSE_HISTORY_VIEW()
{
	m_CurObject = NULL;
	sOriginalKey = _T("");

}

CSE_HISTORY_VIEW::~CSE_HISTORY_VIEW()
{
	FreeData();	
}


BEGIN_MESSAGE_MAP(CSE_HISTORY_VIEW, CView)
	//{{AFX_MSG_MAP(CSE_HISTORY_VIEW)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEACTIVATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_HISTORY_VIEW drawing

VOID CSE_HISTORY_VIEW::OnInitialUpdate()
{

	sCurrentKey = sParentKey;

	LoadViewData(sParentKey);

	CView::OnInitialUpdate();

}

VOID CSE_HISTORY_VIEW::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	DrawData();
}

/////////////////////////////////////////////////////////////////////////////
// CSE_HISTORY_VIEW diagnostics

#ifdef _DEBUG
VOID CSE_HISTORY_VIEW::AssertValid() const
{
	CView::AssertValid();
}

VOID CSE_HISTORY_VIEW::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSE_HISTORY_VIEW message handlers


BOOL CSE_HISTORY_VIEW::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CView::PreTranslateMessage(pMsg);
}

INT CSE_HISTORY_VIEW::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return 0;
}

VOID CSE_HISTORY_VIEW::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnTimer(nIDEvent);
}

VOID CSE_HISTORY_VIEW::DrawData()
{
	CClientDC dc(this);

	if ( sCurrentKey != sParentKey)
	{
		sParentKey = sCurrentKey;
		LoadViewData(sParentKey);
	}

	INT i;
	INT nCount = m_DrawObject.GetCount();
	

	for (i = 0 ; i < nCount ; i++)
	{
		CSObject * DrawObject = (CSObject *)m_DrawObject.GetAt(m_DrawObject.FindIndex(i));
		DrawObject->DrawObject(&dc);
	}
		
	

}

VOID CSE_HISTORY_VIEW::UnDrawData()
{
	CClientDC dc(this);
	INT nCount = m_DrawObject.GetCount();

	for (INT i = 0 ; i < nCount ; i++)
	{
		CSObject * DrawObject = (CSObject *)m_DrawObject.GetAt(m_DrawObject.FindIndex(i));
		DrawObject->UnDrawObject(&dc);
	}

}




VOID CSE_HISTORY_VIEW::LoadViewData(CString REC_KEY)
{
	FreeData();

	CString strQry1,strQry2;

	strQry1 = _T(" P.SPECIES_KEY = ") + REC_KEY;
	strQry2 = _T(" N.SPECIES_KEY = ") + REC_KEY;

	pPrevDM->RefreshDataManager(strQry1);
	pNextDM->RefreshDataManager(strQry2);

	//그룹을 그린다. 후속 저록 - 현재 저록 - 선행 저록 
	MakeGroup();


	//각 그룹에 들어 있는 저록을 표시한다.
	MakeTitleState();


	//저록 과 저록 을 연결 시킨다. 
	Makeline();
	
}

VOID CSE_HISTORY_VIEW::LoadGridData()
{
	pParentWnd->PostMessage( MSG_VIEW_GRID );
}

VOID CSE_HISTORY_VIEW::FreeData()
{
	INT nCount = m_DrawObject.GetCount();

	for (INT i = 0 ; i < nCount ; i++)
	{
		CSObject * DrawObject = (CSObject *)m_DrawObject.GetAt(m_DrawObject.FindIndex(i));
		delete DrawObject;
	}
	
	m_DrawObject.RemoveAll();
}

VOID CSE_HISTORY_VIEW::MakeGroup()
{
	INT nX,nY;

	mGroup1 = new CSE_HISTORY_GROUP();
	mGroup2 = new CSE_HISTORY_GROUP();
	mGroup3 = new CSE_HISTORY_GROUP();
	
	//선행저록
	nX = GROUP_sX;
	nY = GROUP_sY;
	CRect rect1(nX,nY,nX+GROUP_WIDTH,nY+GROUP_HEIGHT);
	mGroup1->m_ObjectRect =rect1 ;
	mGroup1->m_ALIAS = _T("선행저록");

	//현재저록 
	nX += GROUP_GAP;
	CRect rect2(nX,nY,nX+GROUP_WIDTH,nY+GROUP_HEIGHT);
	mGroup2->m_ObjectRect =rect2 ;
	mGroup2->m_ALIAS = _T("현재저록");

	//후속저록 
	nX += GROUP_GAP;
	CRect rect3(nX,nY,nX+GROUP_WIDTH,nY+GROUP_HEIGHT);
	mGroup3->m_ObjectRect =rect3 ;
	mGroup3->m_ALIAS = _T("후속저록");

	m_DrawObject.AddTail((CObject*)mGroup1);
	m_DrawObject.AddTail((CObject*)mGroup2);
	m_DrawObject.AddTail((CObject*)mGroup3);

}

VOID CSE_HISTORY_VIEW::MakeTitleState()
{
	//현재 값 설정 
	INT nIdx = 0 ;
	
	CSStateObject * pStateObject = new CSStateObject;
		  
	
	pStateObject->m_ObjectType = _T("S");
	pStateObject->m_PK = sCurrentKey;
	pStateObject->m_ALIAS = sCurrentTitle;
	pStateObject->m_GROUP_ALIAS = _T("");
	pStateObject->m_DESCRIP = _T("C");
	
	pStateObject->IDX = nIdx;

	if(sOriginalKey == pStateObject->m_PK)
		pStateObject->m_bIsCenter = TRUE;
	
	CRect rect;

	INT nX = mGroup2->m_ObjectRect.left + (GROUP_WIDTH/2);
	INT nY = mGroup2->m_ObjectRect.top + (GROUP_HEIGHT/2);

    rect.top = nY+STATEOBJECT_SIZE;
    rect.bottom = nY-STATEOBJECT_SIZE;
	rect.left = nX-STATEOBJECT_SIZE;
	rect.right = nX+STATEOBJECT_SIZE;
	
	pStateObject->m_ObjectRect = rect;

	pStateObject->m_List = &m_DrawObject;
		
	m_DrawObject.AddTail((CObject*)pStateObject);
    pStateObject = NULL;	  
	nIdx ++;

	//이전 값 설정 
	INT nCount = pPrevDM->GetRowCount();
	
	for ( INT i = 0 ;  i < nCount ; i++)
	{
		CSStateObject * pStateObject = new CSStateObject;
			  
		
		pStateObject->m_ObjectType = _T("S");
		pStateObject->m_PK = pPrevDM->GetCellData(_T("종키"),i);
		pStateObject->m_ALIAS = pPrevDM->GetCellData(_T("서명"),i);;
		pStateObject->m_GROUP_ALIAS = _T("");
		pStateObject->m_DESCRIP = _T("P");

		if(sOriginalKey == pStateObject->m_PK)
			pStateObject->m_bIsCenter = TRUE;
		
		pStateObject->IDX = nIdx;
		
		CRect rect;

		INT nX = mGroup1->m_ObjectRect.left + (GROUP_WIDTH/2);
		INT nY = mGroup1->m_ObjectRect.top + ( (GROUP_HEIGHT/(nCount+1))*(i+1) );

		rect.top = nY+STATEOBJECT_SIZE;
		rect.bottom = nY-STATEOBJECT_SIZE;
		rect.left = nX-STATEOBJECT_SIZE;
		rect.right = nX+STATEOBJECT_SIZE;
		
		pStateObject->m_ObjectRect = rect;

		pStateObject->m_List = &m_DrawObject;
			
		m_DrawObject.AddTail((CObject*)pStateObject);
		pStateObject = NULL;	  
		nIdx ++	; 
	}


	//다음 값 설정
	nCount = pNextDM->GetRowCount();
	
	for ( i = 0 ;  i < nCount ; i++)
	{
		CSStateObject * pStateObject = new CSStateObject;
			  
		
		pStateObject->m_ObjectType = _T("S");
		pStateObject->m_PK = pNextDM->GetCellData(_T("종키"),i);
		pStateObject->m_ALIAS = pNextDM->GetCellData(_T("서명"),i);;
		pStateObject->m_GROUP_ALIAS = _T("");
		pStateObject->m_DESCRIP = _T("N");

		if(sOriginalKey == pStateObject->m_PK)
			pStateObject->m_bIsCenter = TRUE;
		
		pStateObject->IDX = nIdx;
		
		CRect rect;

		INT nX = mGroup3->m_ObjectRect.left + (GROUP_WIDTH/2);
		INT nY = mGroup3->m_ObjectRect.top + ( (GROUP_HEIGHT/(nCount+1))*(i+1) );

		rect.top = nY+STATEOBJECT_SIZE;
		rect.bottom = nY-STATEOBJECT_SIZE;
		rect.left = nX-STATEOBJECT_SIZE;
		rect.right = nX+STATEOBJECT_SIZE;
		
		pStateObject->m_ObjectRect = rect;

		pStateObject->m_List = &m_DrawObject;
			
		m_DrawObject.AddTail((CObject*)pStateObject);
		pStateObject = NULL;	  
		nIdx ++	; 
	}

}

VOID CSE_HISTORY_VIEW::Makeline()
{

	//현재 값을 구한다. 
	INT nPrevX=0,nPrevY=0;
	INT nNextX=0,nNextY=0;

	INT nCount = m_DrawObject.GetCount();

	for (INT i = 0 ; i < nCount ; i++)
	{
		CSObject * DrawObject = (CSObject *)m_DrawObject.GetAt(m_DrawObject.FindIndex(i));
		if(DrawObject->m_ObjectType == _T("S") && DrawObject->m_DESCRIP==_T("C"))
		{
			//PREV
			nPrevX = DrawObject->m_ObjectRect.left ;
			nPrevY = DrawObject->m_ObjectRect.top + ( DrawObject->m_ObjectRect.Height()/2);
		    //NEXT
			nNextX = DrawObject->m_ObjectRect.right ;
			nNextY = DrawObject->m_ObjectRect.top + ( DrawObject->m_ObjectRect.Height()/2) ;

			break;
		}
	}

	//이전 값을 현재 값에 연결 시킨다   
	INT nStartX=0,nStartY=0;

	for ( i = 0 ; i < nCount ; i++)
	{
		CSObject * DrawObject = (CSObject *)m_DrawObject.GetAt(m_DrawObject.FindIndex(i));
		if(DrawObject->m_ObjectType == _T("S") && DrawObject->m_DESCRIP==_T("P"))
		{
		    nStartX = DrawObject->m_ObjectRect.right ;
			nStartY = DrawObject->m_ObjectRect.top + ( DrawObject->m_ObjectRect.Height()/2) ;

			CSPathObject * pPathObject = new CSPathObject;		  
			pPathObject->m_ObjectType  = _T("P");	
			pPathObject->m_PATH_TYPE = _T("");
			
				  
			pPathObject->m_START_STATE_ALIAS = _T("");
			pPathObject->m_END_STATE_ALIAS = _T("");
			pPathObject->m_Activated =  _T("F");
			pPathObject->m_DESCRIP = _T("");
				  
			pPathObject->m_pt1.x = nStartX;
			pPathObject->m_pt1.y = nStartY;
			pPathObject->m_pt2.x = ((nPrevX+nStartX)/2);
			pPathObject->m_pt2.y = ((nPrevY+nStartY-30)/2);
			pPathObject->m_pt3.x = nPrevX;            
			pPathObject->m_pt3.y = nPrevY;
		  				  
  
			m_DrawObject.AddTail((CObject*)pPathObject);
			pPathObject = NULL;
		
		}
	}

	
		  
	//다음 값을 현재 값에 연결 시킨다   
	nStartX=0,nStartY=0;

	for ( i = 0 ; i < nCount ; i++)
	{
		CSObject * DrawObject = (CSObject *)m_DrawObject.GetAt(m_DrawObject.FindIndex(i));
		if(DrawObject->m_ObjectType == _T("S") && DrawObject->m_DESCRIP==_T("N"))
		{
			nStartX = DrawObject->m_ObjectRect.left ;
			nStartY = DrawObject->m_ObjectRect.top + ( DrawObject->m_ObjectRect.Height()/2);

			CSPathObject * pPathObject = new CSPathObject;		  
			pPathObject->m_ObjectType  = _T("P");	
			pPathObject->m_PATH_TYPE = _T("");
			
				  
			pPathObject->m_START_STATE_ALIAS = _T("");
			pPathObject->m_END_STATE_ALIAS = _T("");

			pPathObject->m_Activated =  _T("F");
			pPathObject->m_DESCRIP = _T("");
				  
			pPathObject->m_pt1.x = nStartX;
			pPathObject->m_pt1.y = nStartY;
			pPathObject->m_pt2.x = ((nPrevX+nStartX)/2);
			pPathObject->m_pt2.y = ((nPrevY+nStartY-30)/2);
			pPathObject->m_pt3.x = nNextX;            
			pPathObject->m_pt3.y = nNextY;
		  				  
  
			m_DrawObject.AddTail((CObject*)pPathObject);
			pPathObject = NULL;
		
		}
	}
	




}

VOID CSE_HISTORY_VIEW::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);

	INT nCount = m_DrawObject.GetCount();

	for (INT i = 0 ; i < nCount ; i++)
	{
		CSObject * DrawObject = (CSObject *)m_DrawObject.GetAt(m_DrawObject.FindIndex(i));
		if(DrawObject->m_ObjectType ==_T("S") )
		{

			INT m_Selected = DrawObject->IsSeleted(point);
			if( m_Selected == 1)
			{
				if ( m_CurObject == DrawObject) return;

				if( m_CurObject != NULL && m_CurObject !=DrawObject )
				{
					m_CurObject->m_Selected = 0;
					m_CurObject->m_Activated = _T("F");
					m_CurObject->UnDrawObject(&dc);
					m_CurObject->DrawObject(&dc);

				}

				DrawObject->UnDrawObject(&dc);
				DrawObject->m_Activated = _T("T");
				DrawObject->DrawObject(&dc);
					
				m_CurObject=DrawObject;

				sCurrentKey =  m_CurObject->m_PK;
				sCurrentTitle = m_CurObject->m_ALIAS;

				LoadGridData();

			}
		}

	}

	for (i = 0 ; i < nCount ; i++)
	{
		CSObject * DrawObject = (CSObject *)m_DrawObject.GetAt(m_DrawObject.FindIndex(i));
		if(DrawObject->m_ObjectType ==_T("P") )
		{
				
			DrawObject->DrawObject(&dc);				
		}

	}
	
	CView::OnLButtonDown(nFlags, point);
}

