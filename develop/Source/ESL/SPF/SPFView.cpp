// SPFView.cpp : implementation of the CSPFView class
//

#include "stdafx.h"
#include "SPF.h"

#include "SPFDoc.h"
#include "SPFView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSPFView

IMPLEMENT_DYNCREATE(CSPFView, CScrollView)

BEGIN_MESSAGE_MAP(CSPFView, CScrollView)
	//{{AFX_MSG_MAP(CSPFView)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_SAVE_TASK, OnSaveTask)
	ON_COMMAND(ID_DELETE_OBJECT, OnDeleteObject)
	ON_COMMAND(ID_FRONT_ALIGN, OnFrontAlign)
	ON_COMMAND(ID_END_ALIGN, OnEndAlign)
	ON_COMMAND(ID_TOP_ALIGN, OnTopAlign)
	ON_COMMAND(ID_WIDE_SIZE, OnWideSize)
	ON_COMMAND(ID_HEIGHT_SIZE, OnHeightSize)
	ON_COMMAND(ID_DOWN_ALIGN, OnDownAlign)
	ON_COMMAND(ID_ALL_SIZE, OnAllSize)
	//}}AFX_MSG_MAP
	ON_MESSAGE(MSG_INIT_SPF,Init_SPF)

	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSPFView construction/destruction

CSPFView::CSPFView()
{
	// TODO: add construction code here
	m_CurObject =NULL;
	m_MoveFlag = 0;
	

	HeaderRect.left = 20;
	HeaderRect.top  = 10;
	HeaderRect.right = 900;
	HeaderRect.bottom = 40;

	///	CFG
	ReadCfgFile.ReadCfgFile(_T(""));
	GroupDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	StateDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	PathDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	//SAVE COUNT
	m_SAVE_COUNT=0;

	// KOL.UDF.022 시큐어코딩 보완
	m_pFrameWnd = NULL;
	m_List.RemoveAll(); 
	m_SelectList.RemoveAll(); 
	pToolDlg = NULL;
	m_strObjectType = _T("");
	m_bISMOUSEDRAG = FALSE;
    m_ptDropPoint = NULL;
	m_ResizeFlag = 0;
	m_ResizeIndex = 0; 
	m_point = NULL;
	
    m_GroupSQL = _T("");
	m_StateSQL = _T("");
	m_PathSQL = _T("");
}

CSPFView::~CSPFView()
{
  
    FreeObjectList();
 
}

BOOL CSPFView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSPFView drawing

VOID CSPFView::OnDraw(CDC* pDC)
{
	CSPFDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

    ShowObject(pDC);	
	

}

VOID CSPFView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = 980;
	sizeTotal.cy = 693;
	SetScrollSizes(MM_TEXT, sizeTotal);
	
    	


	///drop target에 등록
  	DragAcceptFiles(TRUE);
   	UINT i = RegisterClipboardFormat(_T("SPF_TOOL"));    
    m_dropTarget.Register(this);
    

	//ImageList등록 관련 Init
    m_bmpDrag.Create(IDB_STATE_VIEW,64,1,RGB(0,128,128));

    //FRAME Pointer 
	m_pFrameWnd = (CMainFrame*)AfxGetMainWnd();

	//SPF DATA 초기화    
//	Init_SPF();
	
    //네개의 DM포인터를 statedlg에 넘겨준다. 
    m_pFrameWnd->m_StateDlg.GroupDM = &GroupDM; 
	m_pFrameWnd->m_StateDlg.StateDM = &StateDM;
    m_pFrameWnd->m_StateDlg.PathDM = &PathDM;
    
	
	m_pFrameWnd->m_StateDlg.m_pList = &m_List;
	m_pFrameWnd->m_StateDlg.m_CurObject = m_CurObject;
	m_pFrameWnd->m_StateDlg.m_pView=this;

}

/////////////////////////////////////////////////////////////////////////////
// CSPFView printing

BOOL CSPFView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // default preparation
	return DoPreparePrinting(pInfo);
}

VOID CSPFView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

VOID CSPFView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSPFView diagnostics

#ifdef _DEBUG
VOID CSPFView::AssertValid() const
{
	CScrollView::AssertValid();
}

VOID CSPFView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CSPFDoc* CSPFView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSPFDoc)));
	return (CSPFDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSPFView message handlers



DROPEFFECT CSPFView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	// TODO: Add your specialized code here and/or call the base class

	 
 	 
	 //m_bmpDrag.BeginDrag(0,point);
     //m_bmpDrag.DragEnter(this,point);
    
	 return DROPEFFECT_MOVE;
	// return CScrollView::OnDragEnter(pDataObject, dwKeyState, point);
}


DROPEFFECT CSPFView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	 //m_bmpDrag.DragMove(point);
    
	 return DROPEFFECT_MOVE;
	//return CScrollView::OnDragOver(pDataObject, dwKeyState, point);
}

VOID CSPFView::OnDragLeave() 
{
	// TODO: Add your specialized code here and/or call the base class
	
     //m_bmpDrag.DragLeave(this);

	 m_bISMOUSEDRAG = FALSE;  

	CScrollView::OnDragLeave();
}

VOID CSPFView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CPoint Tmp;
	Tmp.x =  point.x - m_point.x;
	Tmp.y =  point.y - m_point.y;
	CClientDC dc(this);


	if( m_SelectList.GetCount() != 0 )
	{
	
		if(m_MoveFlag==1)
		{
			for(INT i =0 ; i < m_SelectList.GetCount() ; i++)
			{
				CSObject * tmp = (CSObject*)m_SelectList.GetAt(m_SelectList.FindIndex(i));

   				tmp->UnDrawObject(&dc);
				tmp->MoveObject(Tmp);
//				tmp->CheckAllEnv(point);

				ShowObject(&dc);
			}

			m_point = point;	
		}

	}
	else
	if( m_CurObject != NULL)
	{

		if( m_MoveFlag == 1 )//&& m_CurObject->m_ObjectType != 'P') //현재로서는 PATH는 MOVE를 허용하지 않음
		{

    		m_CurObject->UnDrawObject(&dc);
			m_CurObject->MoveObject(Tmp);
			m_CurObject->CheckAllEnv(point);


			ShowObject(&dc);
		}

		if(m_ResizeFlag == 1) //pickpoint를 움직이는 경우 
		{

			m_CurObject->UnDrawObject(&dc);
			m_CurObject->ReSizeObject(point,m_ResizeIndex);                 				
	        if(m_CurObject->m_ObjectType == 'P')
			{
              m_CurObject->CheckAllEnv(point);
			}


			ShowObject(&dc);
		}
	
		m_point = point;	
	}
  
	CScrollView::OnMouseMove(nFlags, point);
}



BOOL CSPFView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	// TODO: Add your specialized code here and/or call the base class
	
    m_ptDropPoint = point;
     
	HANDLE hData = pDataObject->GetGlobalData(RegisterClipboardFormat(_T("SPF_TOOL")));
	if (hData)
	{
		SelectedIndex *pData = (SelectedIndex*)::GlobalLock(hData);
		m_strObjectType = pData->index;
		::GlobalUnlock(hData);
	}
        
	CDC* pDC = GetDC();
	
	if(m_strObjectType == _T("group"))
	{
        Insert_Group_Object();
        ShowObject(pDC);
	}
    else if(m_strObjectType == _T("state"))
	{      
	
	   Insert_State_Object();
	   ShowObject(pDC);
	}
	else if(m_strObjectType == _T("path"))
	{      
	    Insert_Path_Object();
		ShowObject(pDC);
	}
    
	
    ReleaseDC(pDC);
    return CScrollView::OnDrop(pDataObject, dropEffect, point);
}


INT CSPFView::FreeObjectList()
{
    INT count = m_List.GetCount();
	if (count == 0) return 0;
	
	CSObject *pSObject;
	for (INT i = 0; i < count; i++) {
		pSObject = (CSObject*)m_List.RemoveHead();
		if (pSObject)
		{
			delete pSObject;
			pSObject = NULL;
		}
	}
	m_List.RemoveAll();
	return(count);
}




INT CSPFView::InitDM()
{
    CRect rect;
    CString Top,Bottom,Left,Right;
    CString Qry,TmpGroupSql,TmpStateSql,TmpPathSql;
		
	//Init 
	m_CurObject = NULL;
	
	//GroupDM setting 
	
	TmpGroupSql = _T("");
	if(m_pFrameWnd->m_Load_Type == _T("GROUP"))
		TmpGroupSql.Format(_T(" ALIAS "));

	TmpGroupSql += m_pFrameWnd->m_Sql;
    Qry.Format(_T("select PK,ALIAS,TASK_ALIAS,PARENT_GROUP_ALIAS,DESCRIP,TOP,BOTTOM,LEFT,RIGHT,IDX,GROUP_TYPE ") 
		_T(" from SPF_GROUP_TBL WHERE %s ORDER BY IDX"),TmpGroupSql);
	GroupDM.RestructDataManager(Qry);

	//StateDM setting
	TmpStateSql = _T("");
	if(m_pFrameWnd->m_Load_Type == _T("GROUP"))
		TmpStateSql.Format(_T(" GROUP_ALIAS "));

	TmpStateSql += m_pFrameWnd->m_Sql;
    Qry.Format(_T("select PK,ALIAS,GROUP_ALIAS,DESCRIP,TOP,BOTTOM,LEFT,RIGHT,IDX ")
		_T("from SPF_STATE_TBL WHERE %s ORDER BY IDX"),TmpStateSql);
	StateDM.RestructDataManager(Qry);
  

	//PathDM setting
	TmpPathSql=_T("");
	TmpPathSql += m_pFrameWnd->m_Sql;
	if(m_pFrameWnd->m_Load_Type == _T("GROUP"))
	TmpPathSql = Make_PathSql();
	
    Qry.Format(_T("select PK,ALIAS,PATH_TYPE,START_STATE_ALIAS,END_STATE_ALIAS,ISACTIVATED,SOURCE_FILE_NAME,SOURCE_FUNCTION_NAME,DESCRIP,PT1_X,PT1_Y,PT2_X,PT2_Y,PT3_X,PT3_Y,DEVELOPER,DVM_ALIAS,IDX,TASK_ALIAS") 
		_T(" from SPF_PATH_TBL WHERE  %s  ORDER BY IDX") ,TmpPathSql);
	PathDM.RestructDataManager(Qry);
        
	//GROUP
	if(m_pFrameWnd->m_Load_Type == _T("GROUP"))
	{

		CStringList s_List;
		
		for( INT i =0 ; i< PathDM.GetRowCount() ; i++)
		{
			  CString m_START_STATE_ALIAS = PathDM.GetCellData(i,3);
	          CString m_END_STATE_ALIAS = PathDM.GetCellData(i,4);
			  CString m_TASK_ALIAS = PathDM.GetCellData(i,18);
			if( m_pFrameWnd->m_TASK ==  m_TASK_ALIAS)
			{

				if( !s_List.Find( m_START_STATE_ALIAS ) )
				{
					s_List.AddHead(m_START_STATE_ALIAS);
				}
				if( !s_List.Find( m_END_STATE_ALIAS ) )
				{
					s_List.AddHead(m_END_STATE_ALIAS);
				}

			}
		}

		CString StateQry=_T(" AND ALIAS IN ( ");
		for(i = 0 ; i < s_List.GetCount() ; i++)
		{
			CString TmpQry;
			if( i == 0)   
				TmpQry.Format(_T(" '%s' "),s_List.GetAt(s_List.FindIndex(i)));
			else
				TmpQry.Format(_T(" ,'%s' "),s_List.GetAt(s_List.FindIndex(i)));

			StateQry += TmpQry;
		}

		StateQry += _T(" ) ");

//		TmpStateSql.Format(_T(" GROUP_ALIAS "));
//		TmpStateSql += m_pFrameWnd->m_Sql;
		
		//STATE Query
		TmpStateSql += StateQry;
		 Qry.Format(_T("select PK,ALIAS,GROUP_ALIAS,DESCRIP,TOP,BOTTOM,LEFT,RIGHT,IDX ")
			_T("from SPF_STATE_TBL WHERE TASK_ALIAS = '%s'  %s  ORDER BY IDX"),m_pFrameWnd->m_TASK,StateQry);
		StateDM.RestructDataManager(Qry);

		//PATH Query


	}


	//Group Object
	INT rowcount = GroupDM.GetRowCount();
	for(INT i =0; i < rowcount ; i++)
	{
		
		  CSGroupObject * pGroupObject = new CSGroupObject;
		  
		  pGroupObject->m_ObjectType = _T("G");
		  pGroupObject->m_PK = GroupDM.GetCellData(i,0);
		  pGroupObject->m_ALIAS = GroupDM.GetCellData(i,1);
          pGroupObject->m_TASK = GroupDM.GetCellData(i,2);
		  pGroupObject->m_PARENT_GROUP_ALIAS = GroupDM.GetCellData(i,3);
		  pGroupObject->m_DESCRIP = GroupDM.GetCellData(i,4);
		                    		  
		  Top = GroupDM.GetCellData(i,5);
          Bottom = GroupDM.GetCellData(i,6);
          Left = GroupDM.GetCellData(i,7);
          Right = GroupDM.GetCellData(i,8);

		  pGroupObject->IDX = _ttoi(GroupDM.GetCellData(i,9).GetBuffer(0));
		
		  pGroupObject->m_GROUP_TYPE = GroupDM.GetCellData(i,10);

          rect.top = _ttoi(Top);
          rect.bottom = _ttoi(Bottom);
		  rect.left = _ttoi(Left);
		  rect.right = _ttoi(Right);

          pGroupObject->m_ObjectRect = rect;
          pGroupObject->m_List = &m_List;
		  pGroupObject->pDM = &GroupDM;

	      m_List.AddTail((CObject*)pGroupObject);
	      pGroupObject = NULL;
	}
	


    //Stats Object
	rowcount = StateDM.GetRowCount();
	for( i =0; i < rowcount ; i++)
	{
		
		  CSStateObject * pStateObject = new CSStateObject;
		  

		  pStateObject->m_ObjectType = _T("S");
		  pStateObject->m_PK = StateDM.GetCellData(i,0);
		  pStateObject->m_ALIAS = StateDM.GetCellData(i,1);
		  pStateObject->m_GROUP_ALIAS = StateDM.GetCellData(i,2);
		  pStateObject->m_DESCRIP = StateDM.GetCellData(i,3);

	      if(m_pFrameWnd->m_Load_Type == _T("GROUP"))
		  {
  			  if( !m_pFrameWnd->m_GroupList.Find(pStateObject->m_GROUP_ALIAS) )
			  {
					pStateObject->OtherGroupFlag = 1;
			  }

		  }



		  Top = StateDM.GetCellData(i,4);
          Bottom = StateDM.GetCellData(i,5);
          Left = StateDM.GetCellData(i,6);
          Right = StateDM.GetCellData(i,7);

  		  pStateObject->IDX = _ttoi(StateDM.GetCellData(i,8).GetBuffer(0));

          rect.top = _ttoi(Top);
          rect.bottom = _ttoi(Bottom);
		  rect.left = _ttoi(Left);
		  rect.right = _ttoi(Right);

          pStateObject->m_ObjectRect = rect;
          pStateObject->m_List = &m_List;
          pStateObject->pDM = &StateDM;
		  
	      m_List.AddTail((CObject*)pStateObject);
	      pStateObject = NULL;
	}
    
    
    //PATH
	rowcount = PathDM.GetRowCount();
	for(i =0; i < rowcount ; i++)
	{
		  CString m_TASK_ALIAS = PathDM.GetCellData(i,18);
          if( m_TASK_ALIAS.Compare(m_pFrameWnd->m_TASK) != 0 ) continue;


		  CSPathObject * pPathObject = new CSPathObject;		  
		  pPathObject->m_ObjectType  = _T("P");	
		  pPathObject->m_PK = PathDM.GetCellData(i,0);
		  pPathObject->m_ALIAS = PathDM.GetCellData(i,1);
          pPathObject->m_PATH_TYPE = PathDM.GetCellData(i,2);

		  if(pPathObject->m_PATH_TYPE == _T("B"))
		  {	
			  CSPathObject * ptmpObject; 			
			  for(INT j =0 ; j<m_List.GetCount() ; j++)
			  {
				  ptmpObject= (CSPathObject *)m_List.GetAt(m_List.FindIndex(j));
				  if( ptmpObject->m_PK == pPathObject->m_PK)
				  {
					 ptmpObject->pBackPath = pPathObject;
					 break;
				  }
			  }
		  }

		  if(pPathObject->m_PATH_TYPE == _T("D"))
		  {	
			  CSPathObject * ptmpObject; 			
			  for(INT j =0 ; j<m_List.GetCount() ; j++)
			  {
				  ptmpObject= (CSPathObject *)m_List.GetAt(m_List.FindIndex(j));
				  if( ptmpObject->m_PK == pPathObject->m_PK)
				  {
					 pPathObject->pBackPath = ptmpObject;
					 break;
				  }
			  }
		  }

		  
		  pPathObject->m_START_STATE_ALIAS = PathDM.GetCellData(i,3);
          pPathObject->m_END_STATE_ALIAS = PathDM.GetCellData(i,4);
		  pPathObject->m_Activated =  PathDM.GetCellData(i,5);
		  pPathObject->m_SOURCE_FILE_NAME =  PathDM.GetCellData(i,6);
          pPathObject->m_SOURCE_FUNCTION_NAME = PathDM.GetCellData(i,7);
		  pPathObject->m_DESCRIP = PathDM.GetCellData(i,8);
          
		  pPathObject->m_pt1.x = _ttoi(PathDM.GetCellData(i,9));
          pPathObject->m_pt1.y = _ttoi(PathDM.GetCellData(i,10));
          pPathObject->m_pt2.x = _ttoi(PathDM.GetCellData(i,11));
          pPathObject->m_pt2.y = _ttoi(PathDM.GetCellData(i,12));
          pPathObject->m_pt3.x = _ttoi(PathDM.GetCellData(i,13));            
          pPathObject->m_pt3.y = _ttoi(PathDM.GetCellData(i,14));

          pPathObject->m_Developer = PathDM.GetCellData(i,15);
          pPathObject->m_DvmAlias  = PathDM.GetCellData(i,16);
		  
		  pPathObject->IDX = _ttoi(PathDM.GetCellData(i,17).GetBuffer(0));

	      
		  pPathObject->m_List = &m_List;
          pPathObject->pDM = &PathDM;

		  m_List.AddTail((CObject*)pPathObject);
	      pPathObject = NULL;
	}
 
	



	return 0;
}

CString CSPFView::Make_PathSql()
{
	INT Count=0;
	CString TmpSql,Tmp,Tmp2,TmpSql2;
	Count = StateDM.GetRowCount();

	if( Count == 0)
	{
		return _T("START_STATE_ALIAS IN ('ahehehehehehehehe') ");

	}

	///START
	TmpSql = _T(" START_STATE_ALIAS IN ( ");

	for( INT i=0 ; i < Count ; i++)
	{
		Tmp.Format(_T(" '%s' "),StateDM.GetCellData(i, 1));

		if( i != Count-1)
			Tmp += _T(",");

		Tmp2 += Tmp;
	}

	TmpSql += Tmp2;
	TmpSql += _T(")");

	TmpSql += _T(" OR ");

	////END
	TmpSql2 = _T(" END_STATE_ALIAS IN ( ");
	Tmp2 = _T("");
	for( i=0 ; i < Count ; i++)
	{
		Tmp.Format(_T(" '%s' "),StateDM.GetCellData(i, 1));

		if( i != Count-1)
			Tmp += _T(",");

		Tmp2 += Tmp;
	}

	TmpSql2 += Tmp2;
	TmpSql2 += _T(")");


	TmpSql += TmpSql2;

	return TmpSql;
}


INT CSPFView::Insert_Group_Object()
{
	CGroupInsert dlg(this);

	CString Tmp = m_pFrameWnd->m_TASK;
	if( Tmp.GetLength() == 0)
	{
		AfxMessageBox(_T("TASK 선택 되지 않았습니다."));
		return 0;
	}


	if( dlg.DoModal() == IDOK )
	{
        CSGroupObject * pGroupObject = new CSGroupObject;
						
	    CRect rect;
        rect.top = m_ptDropPoint.y+GROUPOBJECT_SIZE;
	    rect.bottom = m_ptDropPoint.y-GROUPOBJECT_SIZE;
		rect.left = m_ptDropPoint.x-GROUPOBJECT_SIZE;
		rect.right = m_ptDropPoint.x+GROUPOBJECT_SIZE;
             
		CString TOP,BOTTOM,LEFT,RIGHT;
		GroupDM.MakeRecKey(pGroupObject->m_PK);
            
		TOP.Format(_T("%d"),rect.top);
		BOTTOM.Format(_T("%d"),rect.bottom);
		LEFT.Format(_T("%d"),rect.left);
		RIGHT.Format(_T("%d"),rect.right);
                                  
        //object에 정보저장 후 COblist에 연결 
		pGroupObject->m_ObjectType = 'G';
		pGroupObject->m_ALIAS = dlg.m_NAME;	
		pGroupObject->m_DESCRIP = dlg.m_DESC;

		if( dlg.m_Type )
			pGroupObject->m_GROUP_TYPE = _T("Y");
		else
			pGroupObject->m_GROUP_TYPE = _T("");
			

        pGroupObject->m_ObjectRect = rect;
		pGroupObject->m_TASK = m_pFrameWnd->m_TASK;

		///IDX 가져 오기 
		if(GroupDM.GetRowCount() == 0 ) 
			pGroupObject->IDX = 3000;
		else
		{
			INT tmpIDX=3000,mIDX;
			for(INT i = 0 ; i <  GroupDM.GetRowCount() ; i++)
			{
				mIDX = _ttoi(GroupDM.GetCellData(i,9).GetBuffer(0));
				if( tmpIDX < mIDX )
					tmpIDX = mIDX;
			}
			pGroupObject->IDX = tmpIDX+1;
		
		}


		//LIST 
		pGroupObject->m_List = &m_List;
		//DM
		pGroupObject->pDM = &GroupDM;

            
        m_List.AddTail((CObject*)pGroupObject);
			     
		
		pGroupObject->InsertToDM();
		
		////////////////////////////////////////tooldlg groupDM insert
	
		CESL_DataMgr *pDM;
		
		pDM = m_pFrameWnd->m_dlg.GroupDM;
        CString tmp_idx;
        tmp_idx.Format(_T("%d"),pGroupObject->IDX);
        
		
		pDM->InsertRow(-1);
		pDM->SetCellData(pDM->GetRowCount()-1,0,pGroupObject->m_PK);
		pDM->SetCellData(pDM->GetRowCount()-1,1,pGroupObject->m_ALIAS);
		pDM->SetCellData(pDM->GetRowCount()-1,2,pGroupObject->m_TASK);
		
		pDM->SetCellData(pDM->GetRowCount()-1,3,pGroupObject->m_DESCRIP);
		pDM->SetCellData(pDM->GetRowCount()-1,4,TOP);
		pDM->SetCellData(pDM->GetRowCount()-1,5,BOTTOM);
		pDM->SetCellData(pDM->GetRowCount()-1,6,LEFT); 
		pDM->SetCellData(pDM->GetRowCount()-1,7,RIGHT);
		//pDM->SetCellData(pDM->GetRowCount()-1,9,tmp_idx);

		//pDM->SetCellData(pDM->GetRowCount()-1,10,pGroupObject->m_GROUP_TYPE);
		
		m_pFrameWnd->m_dlg.Init_Tree();    //maybe another function will be needed
        ///////////////////////////////////////////////////////////////
	
		
		return 0;

   }

	return 0;
}
INT CSPFView::Insert_State_Object()
{
	CStateInsert dlg(this);

  	CString Tmp = m_pFrameWnd->m_TASK;
	if( Tmp.GetLength() == 0)
	{
		AfxMessageBox(_T("TASK 선택 되지 않았습니다."));
		return 0;
	}
  
	if( dlg.DoModal() == IDOK )
	{
          
	    CSStateObject * pStateObject = new CSStateObject;
						
	    CRect rect;
        rect.top = m_ptDropPoint.y+STATEOBJECT_SIZE;
	    rect.bottom = m_ptDropPoint.y-STATEOBJECT_SIZE;
		rect.left = m_ptDropPoint.x-STATEOBJECT_SIZE;
		rect.right = m_ptDropPoint.x+STATEOBJECT_SIZE;
             
		CString TOP,BOTTOM,LEFT,RIGHT;
		StateDM.MakeRecKey(pStateObject->m_PK);
            
		TOP.Format(_T("%d"),rect.top);
		BOTTOM.Format(_T("%d"),rect.bottom);
		LEFT.Format(_T("%d"),rect.left);
		RIGHT.Format(_T("%d"),rect.right);
                                  
        //object에 정보저장 후 COblist에 연결 
		pStateObject->m_ObjectType = 'S';
		pStateObject->m_ALIAS = dlg.m_NAME;	
		pStateObject->m_DESCRIP = dlg.m_DESC;
        pStateObject->m_ObjectRect = rect;

		pStateObject->m_TASK = m_pFrameWnd->m_TASK; 

		///IDX 가져 오기 
		if(StateDM.GetRowCount() == 0 ) 
			pStateObject->IDX = 4000;
		else
		{
			INT tmpIDX=4000,mIDX;
			for(INT i = 0 ; i <  StateDM.GetRowCount() ; i++)
			{
				mIDX = _ttoi(StateDM.GetCellData(i,8).GetBuffer(0));
				if( tmpIDX < mIDX )
					tmpIDX = mIDX;
			}
			pStateObject->IDX = tmpIDX+1;
		
		}
		
   		//LIST 
		pStateObject->m_List = &m_List;
		//DM
		pStateObject->pDM = &StateDM;

        m_List.AddTail((CObject*)pStateObject);
			     

    	pStateObject->InsertToDM();


		return 0;

	}
   	
      return 0;
}

INT CSPFView::Insert_Path_Object()
{
	
    CPathInsert dlg(this);

	CString Tmp = m_pFrameWnd->m_TASK;
	if( Tmp.GetLength() == 0)
	{
		AfxMessageBox(_T("TASK 선택 되지 않았습니다."));
		return 0;
	}
	  
	  if( dlg.DoModal() == IDOK )
	  {
            
		    if(!dlg.m_use_back) ///////////////////////SINGLE PATH INSERT
			{
				CSPathObject * pPathObject = new CSPathObject;
											
				PathDM.MakeRecKey(pPathObject->m_PK);
                          
				//object에 정보저장 후 COblist에 연결 
				pPathObject->m_ObjectType = 'P';
				pPathObject->m_ALIAS = dlg.m_NAME;	
				pPathObject->m_DESCRIP = dlg.m_DESC;
				pPathObject->m_PATH_TYPE = _T("S");
				pPathObject->m_SOURCE_FILE_NAME = dlg.m_FILE;
				pPathObject->m_SOURCE_FUNCTION_NAME = dlg.m_FUNCTION;
				pPathObject->m_Developer = dlg.m_Developer;
				pPathObject->m_DvmAlias = dlg.m_DvmAlias;
		
				pPathObject->m_TASK = m_pFrameWnd->m_TASK; 


				if( dlg.m_IsActivated )
					pPathObject->m_Activated = _T("T");
				else
					pPathObject->m_Activated = _T("F");

                pPathObject->m_pt1.x = m_ptDropPoint.x-PATHOBJECT_WIDTH;
                pPathObject->m_pt1.y = m_ptDropPoint.y;
                pPathObject->m_pt2.x = m_ptDropPoint.x;
                pPathObject->m_pt2.y = m_ptDropPoint.y-PATHOBJECT_HEIGHT;
                pPathObject->m_pt3.x = m_ptDropPoint.x+PATHOBJECT_WIDTH;           
                pPathObject->m_pt3.y = m_ptDropPoint.y;    

				///IDX 가져 오기 
				if(PathDM.GetRowCount() == 0 ) 
					pPathObject->IDX = 5000;
				else
				{
					INT tmpIDX=5000,mIDX;
					for(INT i = 0 ; i <  PathDM.GetRowCount() ; i++)
					{
						mIDX = _ttoi(PathDM.GetCellData(i,8).GetBuffer(0));
						if( tmpIDX < mIDX )
							tmpIDX = mIDX;
					}
					pPathObject->IDX = tmpIDX+1;				
				}

				
   				//LIST 
				pPathObject->m_List = &m_List;
				//DM
				pPathObject->pDM = &PathDM;

				m_List.AddTail((CObject*)pPathObject);
				
                pPathObject->InsertToDM();

                
			}
			else /////////////////////////////DOUBLE PATH INSERT
			{
                CSPathObject * pPathObject = new CSPathObject;
			
             
				CString PK;
				PathDM.MakeRecKey(PK);
            				
				pPathObject->m_PK = PK;
				pPathObject->m_ObjectType = 'P';
				pPathObject->m_ALIAS = dlg.m_NAME;	
				pPathObject->m_DESCRIP = dlg.m_DESC;
				pPathObject->m_PATH_TYPE = _T("D");
				pPathObject->m_SOURCE_FILE_NAME = dlg.m_FILE;
				pPathObject->m_SOURCE_FUNCTION_NAME = dlg.m_FUNCTION;
				pPathObject->m_Developer = dlg.m_Developer;
				pPathObject->m_DvmAlias = dlg.m_DvmAlias;
		
				pPathObject->m_TASK = m_pFrameWnd->m_TASK; 

				if( dlg.m_IsActivated )
					pPathObject->m_Activated = _T("T");
				else
					pPathObject->m_Activated = _T("F");

                pPathObject->m_pt1.x = m_ptDropPoint.x-PATHOBJECT_WIDTH;
                pPathObject->m_pt1.y = m_ptDropPoint.y;
                pPathObject->m_pt2.x = m_ptDropPoint.x;
                pPathObject->m_pt2.y = m_ptDropPoint.y-PATHOBJECT_HEIGHT;
                pPathObject->m_pt3.x = m_ptDropPoint.x+PATHOBJECT_WIDTH;           
                pPathObject->m_pt3.y = m_ptDropPoint.y;    
				///IDX 가져 오기 
				if(PathDM.GetRowCount() == 0 ) 
					pPathObject->IDX = 5000;
				else
				{
					INT tmpIDX=5000,mIDX;
					for(INT i = 0 ; i <  PathDM.GetRowCount() ; i++)
					{
						mIDX = _ttoi(PathDM.GetCellData(i,8).GetBuffer(0));
						if( tmpIDX < mIDX )
							tmpIDX = mIDX;
					}
					pPathObject->IDX = tmpIDX+1;								
				}

   				//LIST 
				pPathObject->m_List = &m_List;
				//DM
				pPathObject->pDM = &PathDM;

				m_List.AddTail((CObject*)pPathObject);
				
                pPathObject->InsertToDM();
                
				//////////////////////////////BACK PATH
                CSPathObject *pPathObject1 = new CSPathObject;
                
                pPathObject1->m_PK = PK;
				pPathObject1->m_ObjectType = 'P';
				pPathObject1->m_ALIAS = dlg.m_BACKNAME;	
				pPathObject1->m_DESCRIP = dlg.m_BACKDESC;
				pPathObject1->m_PATH_TYPE = _T("B");
				pPathObject1->m_SOURCE_FILE_NAME = dlg.m_BACKFILE;
				pPathObject1->m_SOURCE_FUNCTION_NAME = dlg.m_BACKFUNCTION;
				pPathObject1->m_Developer = dlg.m_Backdeveloper;
				pPathObject1->m_DvmAlias = dlg.m_BackDvmAlias;
		
				pPathObject1->m_TASK = m_pFrameWnd->m_TASK; 

				if( dlg.m_IsActivated )
					pPathObject1->m_Activated = _T("T");
				else
					pPathObject1->m_Activated = _T("F");

                pPathObject1->m_pt1.x = m_ptDropPoint.x-PATHOBJECT_WIDTH;
                pPathObject1->m_pt1.y = m_ptDropPoint.y;
                pPathObject1->m_pt2.x = m_ptDropPoint.x;
                pPathObject1->m_pt2.y = m_ptDropPoint.y-PATHOBJECT_HEIGHT;
                pPathObject1->m_pt3.x = m_ptDropPoint.x+PATHOBJECT_WIDTH;           
                pPathObject1->m_pt3.y = m_ptDropPoint.y;  
				///IDX 가져 오기 
				if(PathDM.GetRowCount() == 0 ) 
					pPathObject->IDX = 5000;
				else
				{
					INT tmpIDX=5000,mIDX;
					for(INT i = 0 ; i <  PathDM.GetRowCount() ; i++)
					{
						mIDX = _ttoi(PathDM.GetCellData(i,8).GetBuffer(0));
						if( tmpIDX < mIDX )
							tmpIDX = mIDX;
					}
					pPathObject->IDX = tmpIDX+1;								
				}

				//BACK PATH 연결 
				pPathObject->pBackPath = pPathObject1;

   				//LIST 
				pPathObject1->m_List = &m_List;
				//DM
				pPathObject1->pDM = &PathDM;

				m_List.AddTail((CObject*)pPathObject1);

                pPathObject1->InsertToDM();
 			}



	  }
	  
	  return 0;
}


INT CSPFView::ShowObject(CDC* pDC)
{
	CSObject * tmp;

	DrawTaskTitle(pDC);

	for( INT i =0 ; i < m_List.GetCount() ; i++)
	{

		tmp =(CSObject *)m_List.GetAt(m_List.FindIndex(i));
		tmp->DrawObject(pDC);
	}


	return 0;

}

INT CSPFView::UnShowObject(CDC* pDC)
{
	CSObject * tmp;

	UnDrawTaskTitle(pDC);

	for( INT i =0 ; i < m_List.GetCount() ; i++)
	{


		tmp =(CSObject *)m_List.GetAt(m_List.FindIndex(i));
		tmp->UnDrawObject(pDC);
	}


	return 0;

}


VOID CSPFView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_MoveFlag = 1;
	m_point = point;
	CClientDC dc(this);



	if( m_CurObject != NULL)
	{
		if(m_CurObject->IsPickPoint(point) != 0)
		{
			m_ResizeFlag = 1;
			m_MoveFlag = 0;
			m_ResizeIndex = m_CurObject->IsPickPoint(point);
			return;
		}
	}

	///여러개 선택 할 경우
	if (nFlags & MK_SHIFT)
	{
		if( m_CurObject != NULL)
		m_SelectList.AddTail( (CObject*)m_CurObject );

		if(FindObject(point))
		{			
			for(INT  i =0 ; i < m_SelectList.GetCount() ; i++)
			{
				CSObject * tmp = (CSObject*)m_SelectList.GetAt(m_SelectList.FindIndex(i));
				if (tmp->m_ALIAS == m_CurObject->m_ALIAS)
				{
					m_CurObject->UnDrawObject(&dc);
					m_CurObject->m_Selected = 0;
					m_SelectList.RemoveAt(m_SelectList.FindIndex(i));

				    m_pFrameWnd->m_StateDlg.DeleteAllGrid();
					m_CurObject->DrawObject(&dc);
					//ShowObject(&dc);
					return;
				}
			}

			m_SelectList.AddTail( (CObject*)m_CurObject );
		    m_pFrameWnd->m_StateDlg.DeleteAllGrid();

			m_CurObject->DrawObject(&dc);
		    //ShowObject(&dc);
			return;
		}

		return;
	}

	///OBJECT 정상으로 되돌리기 
	for(INT i =0 ; i < m_SelectList.GetCount() ; i++)
	{
		CSObject * tmp = (CSObject*)m_SelectList.GetAt(m_SelectList.FindIndex(i));
		tmp->UnDrawObject(&dc);
		tmp->m_Selected = 0;
	}
	m_SelectList.RemoveAll();	


	if( m_CurObject != NULL)
	{
	  	m_CurObject->UnDrawObject(&dc);
		m_CurObject->m_Selected = 0;
		m_CurObject=NULL;
	}

	if(FindObject(point))
	{
		ShowGrid();			
	}
    else 
	{
        m_pFrameWnd->m_StateDlg.DeleteAllGrid();
		m_MoveFlag = 0;
	}


    ShowObject(&dc);
	
	CScrollView::OnLButtonDown(nFlags, point);
}


INT CSPFView::FindObject(CPoint point)
{

	CSObject * tmp;

	INT Count = m_List.GetCount();
	for( INT i = Count - 1 ; i > -1 ; i--)
	{
		tmp =(CSObject *)m_List.GetAt(m_List.FindIndex(i));
		if( tmp->IsSeleted(point) )
		{
			m_CurObject = tmp;
			tmp->m_Selected = 1;
			return 1;
		}
	}

	m_CurObject = NULL; 

	return 0;

}



INT CSPFView::IsPickPoint(CPoint point)
{
    
    if(m_CurObject->IsPickPoint(point) != 0)
	{
		return 1;
	}


	return 0;
}


VOID CSPFView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_SelectList.GetCount() != 0 )
	{
	
		m_SAVE_COUNT++ ;

		for(INT i =0 ; i < m_SelectList.GetCount() ; i++)
		{
			CSObject * tmp = (CSObject*)m_SelectList.GetAt(m_SelectList.FindIndex(i));

		}

		m_point = point;	

	}
	else
	if(m_CurObject != NULL)
	{

		m_SAVE_COUNT++ ;

		if(m_MoveFlag)
		{
			m_CurObject->SetAllEnv(point);
		}
	    
		if(m_ResizeFlag == 1 && m_CurObject->m_ObjectType == 'P')
		{
            m_CurObject->SetAllEnv(point);
		}

    }
		
	m_MoveFlag = 0;
    m_ResizeFlag = 0;

	if( m_SAVE_COUNT == 100)
	{
		if(AfxMessageBox(_T("100회정도 OBJECT변경이 이루어 졌습니다.  저장 하시겠습니까?"), MB_YESNO) == IDYES )
		{
			SaveToDB();
		}
	}

	CScrollView::OnLButtonUp(nFlags, point);
}

INT CSPFView::ShowGrid()
{    
	//statsdlg에다가 현재의 object의 PK와 type을 준다 
	m_pFrameWnd->m_StateDlg.m_CurObject = m_CurObject;

    //statedlg grid의 tab의 index를 바꾸어 준다  
	m_pFrameWnd->m_StateDlg.ChangeTabText(); 
    
	//data를 state dlg grid에 보여준다  
    m_pFrameWnd->m_StateDlg.ShowDataOnGrid();

	return 0;
}

INT CSPFView::SaveToDB()
{
	
	GroupDM.SendFrame(TRUE);
	GroupDM.EndFrame();

	StateDM.SendFrame(TRUE);  
	StateDM.EndFrame();

    PathDM.SendFrame(TRUE);  
    PathDM.EndFrame();  
	
	CSObject * tmp;

	for( INT i =0 ; i < m_List.GetCount() ; i++)
	{
		tmp =(CSObject *)m_List.GetAt(m_List.FindIndex(i));
		tmp->SaveToDM();
	}
    
	GroupDM.SendFrame(TRUE);
	GroupDM.EndFrame();

	StateDM.SendFrame(TRUE);  
	StateDM.EndFrame();

    PathDM.SendFrame(TRUE);  
    PathDM.EndFrame();  
    
	//save count 초기화
	m_SAVE_COUNT = 0;

	AfxMessageBox(_T("저장이 완료 되었습니다 ."));
	return 0;
}


VOID CSPFView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if( m_CurObject != NULL)
	{
		ModifyObject();
	}

	CScrollView::OnLButtonDblClk(nFlags, point);
}

INT CSPFView::ModifyObject()
{
    if(m_CurObject->m_ObjectType == _T("G"))
    {
       ModifyGroupObject();
	}
	else if(m_CurObject->m_ObjectType == _T("S"))
	{
       ModifyStateObject();
	}
    else if(m_CurObject->m_ObjectType == _T("P"))
	{
       ModifyPathObject();
	}

	return 0;
}

INT CSPFView::ModifyGroupObject()
{
    
	CString mGroupType;
	CGroupInsert dlg(this);


	dlg.m_NAME=m_CurObject->m_ALIAS;	
	dlg.m_DESC=m_CurObject->m_DESCRIP;

	if(m_CurObject->m_GROUP_TYPE.GetLength())
		dlg.m_Type = true;
	else
		dlg.m_Type = false;

	if( dlg.DoModal() == IDOK )
	{
            
                                  
        //object에 정보저장 후 COblist에 연결 
		m_CurObject->m_ALIAS = dlg.m_NAME;	
		m_CurObject->m_DESCRIP = dlg.m_DESC;
		m_CurObject->m_TASK = m_pFrameWnd->m_TASK;
		
		if(dlg.m_Type)
			m_CurObject->m_GROUP_TYPE = _T("Y");
		else
			m_CurObject->m_GROUP_TYPE = _T("");
		
		m_CurObject->SaveToDM();
		
		return 0;
	

   }

    return 0;
}

INT CSPFView::ModifyStateObject()
{
	CStateInsert dlg(this);

	dlg.m_NAME = m_CurObject->m_ALIAS;	
	dlg.m_DESC = m_CurObject->m_DESCRIP;

	if( dlg.DoModal() == IDOK )
	{
            
        //object에 정보저장 후 COblist에 연결 
		m_CurObject->m_ALIAS = dlg.m_NAME;	
		m_CurObject->m_DESCRIP = dlg.m_DESC;

    	m_CurObject->SaveToDM();


		return 0;

	}


    return 0;
}

INT CSPFView::ModifyPathObject()
{
    CPathInsert dlg(this);
	CSPathObject * m_path,*tmp;


	dlg.ModifyFlag = 1;

	dlg.m_NAME=m_CurObject->m_ALIAS;	
	dlg.m_DESC=m_CurObject->m_DESCRIP;
	dlg.m_FILE=m_CurObject->m_SOURCE_FILE_NAME;
	dlg.m_FUNCTION=m_CurObject->m_SOURCE_FUNCTION_NAME;

	dlg.m_Developer=m_CurObject->m_Developer;
	dlg.m_DvmAlias=m_CurObject->m_DvmAlias;

	if(	m_CurObject->m_Activated == _T("T"))
		dlg.m_IsActivated = true;
	else
		dlg.m_IsActivated = false;
		
	if(m_CurObject->m_PATH_TYPE == _T("D"))
	{
		dlg.ModifyFlag = 2;

		tmp = (CSPathObject *)m_CurObject;
		m_path = (CSPathObject *)tmp->GetBackPath();


		dlg.m_use_back = 1;
		dlg.m_BACKNAME=m_path->m_ALIAS;	
		dlg.m_BACKDESC=m_path->m_DESCRIP;
		dlg.m_BACKFILE=m_path->m_SOURCE_FILE_NAME;
		dlg.m_BACKFUNCTION=m_path->m_SOURCE_FUNCTION_NAME;
		dlg.m_Backdeveloper=m_path->m_Developer;
		dlg.m_BackDvmAlias=m_path->m_DvmAlias;

	}
	  
    if( dlg.DoModal() == IDOK )
	{

		m_CurObject->m_ALIAS = dlg.m_NAME;	
		m_CurObject->m_DESCRIP = dlg.m_DESC;
		m_CurObject->m_SOURCE_FILE_NAME = dlg.m_FILE;
		m_CurObject->m_SOURCE_FUNCTION_NAME = dlg.m_FUNCTION;
		m_CurObject->m_Developer = dlg.m_Developer;
		m_CurObject->m_DvmAlias = dlg.m_DvmAlias;


		if(	dlg.m_IsActivated)
			m_CurObject->m_Activated = _T("T");
		else
			m_CurObject->m_Activated = _T("F");


		if(m_CurObject->m_PATH_TYPE == _T("D"))
		{
			m_path->m_ALIAS = dlg.m_BACKNAME;	
			m_path->m_DESCRIP = dlg.m_BACKDESC;
			m_path->m_SOURCE_FILE_NAME = dlg.m_BACKFILE;
			m_path->m_SOURCE_FUNCTION_NAME = dlg.m_BACKFUNCTION;
			m_path->m_Developer = dlg.m_Backdeveloper;
			m_path->m_DvmAlias = dlg.m_BackDvmAlias;
			m_path->m_Activated = m_CurObject->m_Activated;

			m_path->SaveToDM();
	
		}

		m_CurObject->SaveToDM();

	  }
	  

    return 0;
}

VOID CSPFView::Init_SPF( LPARAM l ,WPARAM w)
{


	CClientDC dc(this);

	
	ScrollToPosition(CPoint(0,0));

	UnShowObject(&dc);

	FreeObjectList();


	InitDM();
	ShowObject(&dc);

	
}

VOID CSPFView::OnSaveTask() 
{
	// TODO: Add your command handler code here
	SaveToDB();
}

VOID CSPFView::OnDeleteObject() 
{
	// TODO: Add your command handler code here
	if( m_CurObject != NULL)
	{

		if(AfxMessageBox(_T("해당 OBJECT를 삭제 하시겠습니까?"), MB_YESNO) == IDYES )
		{
			CClientDC dc(this);
			m_CurObject->UnDrawObject(&dc);
			m_CurObject->DeleteToDM();
			
			if(m_CurObject->m_ObjectType == 'G')
			{
               //tree를 refresh한다.
               
			   
			   m_pFrameWnd->m_dlg.DeleteGroupDMWithPK(m_CurObject->m_PK);
			   
			   m_pFrameWnd->m_dlg.Init_Tree();

			}
			
			delete m_CurObject;
			m_CurObject = NULL;
		}


	}

}

VOID CSPFView::DrawTaskTitle(CDC *pDC)
{
    ///DrawTextVariables
	COLORREF m_colorText;
    LOGFONT m_logFont;

	m_colorText = RGB(0,0,0);

	m_logFont.lfHeight = 15;
    m_logFont.lfWidth = 0;
	m_logFont.lfEscapement =0;
	m_logFont.lfOrientation = 0;
	m_logFont.lfWeight = FW_BOLD;
	m_logFont.lfItalic = FALSE;
	m_logFont.lfUnderline = TRUE;
	m_logFont.lfStrikeOut = FALSE;
	m_logFont.lfCharSet = DEFAULT_CHARSET;
	m_logFont.lfOutPrecision = OUT_CHARACTER_PRECIS;
	m_logFont.lfClipPrecision = OUT_CHARACTER_PRECIS;
    m_logFont.lfQuality = DEFAULT_QUALITY;
	m_logFont.lfPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE;
	

 
    //pDC->Rectangle(HeaderRect);
	
	pDC->SetTextColor(m_colorText);
	pDC->SetBkMode(OPAQUE);
//	DrawTextWithFont(pDC, m_pFrameWnd->m_TASK, HeaderRect, m_logFont, DT_VCENTER|DT_CENTER |DT_SINGLELINE);

	CFont *pOldFont = NULL;
	INT height = -1;

	INT fontsize = m_logFont.lfHeight;
	CFont font;

	m_logFont.lfHeight = -MulDiv(fontsize, GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), 72);
	font.CreateFontIndirect(&m_logFont);

	pOldFont = pDC->SelectObject(&font);

		
	height = pDC->DrawText(m_pFrameWnd->m_TASK, HeaderRect, DT_VCENTER|DT_CENTER |DT_SINGLELINE);
//	pDC->MoveTo(10,35);
//	pDC->LineTo(300,35);
	pDC->SelectObject(pOldFont);
}


VOID CSPFView::UnDrawTaskTitle(CDC *pDC)
{
	CPen m_pen;
	CPen *pOldPen;
	m_pen.CreatePen( PS_SOLID , 3 , RGB(255,255,255));
	pOldPen = pDC->SelectObject(&m_pen);

	pDC->Rectangle(HeaderRect);

}

VOID CSPFView::OnFrontAlign() 
{
	// TODO: Add your command handler code here
	AlignObject(1);

}

VOID CSPFView::OnEndAlign() 
{
	// TODO: Add your command handler code here
	AlignObject(2);

}

VOID CSPFView::OnTopAlign() 
{
	// TODO: Add your command handler code here
	AlignObject(3);

}

VOID CSPFView::OnDownAlign() 
{
	// TODO: Add your command handler code here
	AlignObject(4);
}


INT CSPFView::AlignObject(INT index)
{
    INT COUNT =  m_SelectList.GetCount();
	
	if(COUNT >1) 
	{		
		CSObject * mObject = (CSObject*)m_SelectList.GetAt(m_SelectList.FindIndex(0));
        CClientDC dc(this);

		for(INT i =1 ; i < m_SelectList.GetCount() ; i++)
		{
			CSObject * tmp = (CSObject*)m_SelectList.GetAt(m_SelectList.FindIndex(i));

            ///비교 정렬
			if(tmp->m_ObjectType != 'P')
			{
			    
				tmp->UnDrawObject(&dc);
								
				CPoint TmpPoint;
                
				switch(index)
				{
					case 1: //front
						TmpPoint.x = mObject->m_ObjectRect.left - tmp->m_ObjectRect.left;
						TmpPoint.y = 0; 
						break;
					case 2:  //end
						TmpPoint.x = mObject->m_ObjectRect.right - tmp->m_ObjectRect.right;
						TmpPoint.y = 0; 
						break;
					case 3:  //top
						TmpPoint.x = 0;
						TmpPoint.y = mObject->m_ObjectRect.bottom - tmp->m_ObjectRect.bottom;  
						break;
					case 4:  //down
						TmpPoint.x = 0;
						TmpPoint.y = mObject->m_ObjectRect.top - tmp->m_ObjectRect.top; 
						break;
				}
              
				tmp->MoveObject(TmpPoint); 
			
			    ShowObject(&dc); 
			   
			}
		}
	}
	
	return index; 
}


VOID CSPFView::OnWideSize() 
{
	// TODO: Add your command handler code here
    SetObjectSize(1);
}


VOID CSPFView::OnHeightSize() 
{
	// TODO: Add your command handler code here
	SetObjectSize(2);

}

VOID CSPFView::OnAllSize() 
{
	// TODO: Add your command handler code here
	SetObjectSize(3);

}

INT CSPFView::SetObjectSize(INT index)
{
    INT COUNT =  m_SelectList.GetCount();
	
	if(COUNT >1) 
	{		
		CSObject * mObject = (CSObject*)m_SelectList.GetAt(m_SelectList.FindIndex(0));
        CClientDC dc(this);
        
		if(mObject->m_ObjectType != 'G') return -1;

		for(INT i =1 ; i < m_SelectList.GetCount() ; i++)
		{
			CSObject * tmp = (CSObject*)m_SelectList.GetAt(m_SelectList.FindIndex(i));

            ///비교 정렬
			if(tmp->m_ObjectType == 'G')
			{
			    
				tmp->UnDrawObject(&dc);
				INT tmpwidth,tmpheight;
                          
				switch(index)
				{
				case 1: //width
				    tmpwidth = mObject->m_ObjectRect.right - mObject->m_ObjectRect.left;
					tmp->m_ObjectRect.right = tmp->m_ObjectRect.left + tmpwidth;
              		break;
				
				case 2: //height
				    tmpheight = mObject->m_ObjectRect.top - mObject->m_ObjectRect.bottom;
					tmp->m_ObjectRect.top = tmp->m_ObjectRect.bottom + tmpheight;
              		break;
				
				case 3: //allsize
				    tmpwidth = mObject->m_ObjectRect.right - mObject->m_ObjectRect.left;
					tmpheight = mObject->m_ObjectRect.top - mObject->m_ObjectRect.bottom;
					
					tmp->m_ObjectRect.top = tmp->m_ObjectRect.bottom + tmpheight;
					tmp->m_ObjectRect.right = tmp->m_ObjectRect.left + tmpwidth;
              		break;
				}  
		
				ShowObject(&dc); 
			}
		}
	}

	return index;
}
