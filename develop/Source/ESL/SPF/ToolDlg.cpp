// ToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SPF.h"
#include "ToolDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolDlg dialog


CToolDlg::CToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolDlg::IDD, pParent)
{
	Init_Flag = false;

	//{{AFX_DATA_INIT(CToolDlg)
   
	//}}AFX_DATA_INIT
	
	// KOL.UDF.022 시큐어코딩 보완
	///MainFrame
	pMainFrame = NULL;
	m_nSelectIndex = 0;
    m_nOleFormat = 0;
	CurTabIndex = 0;
	TaskDM = NULL;
 	GroupDM = NULL;    
	m_ProjectName = _T("");
	m_Load_Type->Format(_T(""));
	m_Sql->Format(_T(""));
	m_TASK->Format(_T(""));
    m_TASKDESC->Format(_T(""));
	m_GroupList.RemoveAll();
}
CToolDlg::~CToolDlg()
{

	
}


VOID CToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolDlg)
	DDX_Control(pDX, IDC_TREE_TASK, m_Tree);
	DDX_Control(pDX, IDC_TAB_TOOL, m_Tab);
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolDlg, CDialog)
	//{{AFX_MSG_MAP(CToolDlg)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST1, OnBegindragList1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TOOL, OnSelchangeTabTool)
	ON_NOTIFY(NM_CLICK, IDC_TREE_TASK, OnClickTreeTask)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_TASK, OnDblclkTreeTask)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_TASK, OnSelchangedTreeTask)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolDlg message handlers

BOOL CToolDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//INIT LIST ITEM 
	SetListItem();
  
	///Drag and Drop Setting
    
    DragAcceptFiles(TRUE);
    
	m_nOleFormat = RegisterClipboardFormat(_T("SPF_TOOL"));
    
	BOOL bSuccess = m_oleDropTarget.Register(this);
	
	if(!bSuccess) AfxMessageBox(_T("Drop target Registering failed!"));  
	
	m_nSelectIndex = 0;

	
	//Init Tab
	m_imageListTab.Create(IDB_TAB, 24, 1, RGB(255,255,255));
	
	m_Tab.SetImageList(&m_imageListTab);
	
	TC_ITEM tcItem;
	tcItem.mask = TCIF_IMAGE | TCIF_TEXT;

	tcItem.iImage = 0;
	tcItem.pszText = _T("TASK");
	m_Tab.InsertItem(0, &tcItem);

	tcItem.iImage = 1;
	tcItem.pszText = _T("TOOL");
	m_Tab.InsertItem(2, &tcItem);

   	m_List.ShowWindow(false);
	m_Tree.ShowWindow(true);
	
	CurTabIndex = 0;	
   
	Init_Flag = true ;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CToolDlg::OnBegindragList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	//ole (clipboard copy)
	COleDataSource oleDataSrc;
	

	//선택된 리스트
	POSITION pos = m_List.GetFirstSelectedItemPosition();

	m_nSelectIndex = (INT)pos-1; 

	HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(SelectedIndex));
	if(hGlobal == NULL)
	{
		AfxMessageBox(TEXT(_T("Can not allocate enough memory\t")),MB_OK|MB_ICONSTOP);
		return;
	}
    	
	SelectedIndex *pData = (SelectedIndex*)::GlobalLock(hGlobal);
	

    CString TmpData = m_List.GetItemText(m_nSelectIndex,0); 
    
	_tcscpy(pData->index,TmpData.GetBuffer(0));

    ::GlobalUnlock(hGlobal);

	oleDataSrc.CacheGlobalData(m_nOleFormat, hGlobal);
	oleDataSrc.DoDragDrop(DROPEFFECT_MOVE);

	::GlobalFree(hGlobal);

	*pResult = 0;
}


VOID CToolDlg::SetListItem()
{
    //ImageList등록 관련 Init
	m_bmpList.Create(IDB_STATE,32,1,RGB(0,128,128));
    
	m_List.SetImageList(&m_bmpList,LVSIL_NORMAL);

	CRect rect;
	m_List.GetClientRect(&rect);
	
	
	//LV_COLUMN
	LV_COLUMN lvcolumn;
    lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcolumn.fmt  = LVCFMT_CENTER;
    lvcolumn.iSubItem = 1;
    lvcolumn.cx = 100;
	lvcolumn.pszText = _T("이름");
	m_List.InsertColumn(0,&lvcolumn);
    
    
	
	TCHAR *NAME[] = {_T("group"),_T("state"),_T("path")};
    
	for(INT i=0;i<3;i++)
	{
		//ITEM SETTING
		LV_ITEM lvitem;
		lvitem.iItem = m_List.GetItemCount();
		lvitem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
		lvitem.iSubItem =0;
		lvitem.iImage = i;
		lvitem.stateMask = LVIS_STATEIMAGEMASK;
		lvitem.state = INDEXTOSTATEIMAGEMASK(1);
 		lvitem.pszText = (LPCTSTR)(LPCTSTR)NAME[i];
    
		m_List.InsertItem(&lvitem);
	}


}


INT CToolDlg::Init_Tree()
{

	m_Tree.DeleteAllItems();

	INT Count=0; 
	Count = TaskDM->GetRowCount();
	

	if( m_ProjectName.GetLength() == 0)
		return 0;
	
	//insert project
	TV_INSERTSTRUCT TreeStruct;

	TreeStruct.hParent = NULL;
	TreeStruct.hInsertAfter = TVI_LAST;
	TreeStruct.item.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
	TreeStruct.item.state =0 ;
	TreeStruct.item.stateMask =0;
	TreeStruct.item.cchTextMax = 60;
	TreeStruct.item.iSelectedImage = 1;
	TreeStruct.item.cChildren = 0;


	///ROOT
	TreeStruct.item.pszText = m_ProjectName.GetBuffer(0);
	TreeStruct.item.iImage = 0;
	HTREEITEM hRoot = m_Tree.InsertItem(&TreeStruct);


	///TASK
	for ( INT i =0 ; i < Count ; i++)
	{
		HTREEITEM TaskRoot;

		CString tmp = TaskDM->GetCellData(i, 1);
		TreeStruct.hParent = hRoot;

		TreeStruct.item.pszText = tmp.GetBuffer(0);
		TaskRoot = m_Tree.InsertItem(&TreeStruct);

		for(INT j =0 ; j < GroupDM->GetRowCount() ; j++)
		{

			if( tmp == GroupDM->GetCellData(j, 2) )
			{
				CString tmp1 = GroupDM->GetCellData(j, 1);
				TreeStruct.hParent = TaskRoot;

				TreeStruct.item.pszText = tmp1.GetBuffer(0);
				m_Tree.InsertItem(&TreeStruct);

			}
		}
	}

//	TaskDM->GetCellData(i, 1)
	m_Tree.Expand( m_Tree.GetRootItem( ), TVE_TOGGLE  );

	return 0;
}

INT CToolDlg::DeleteGroupDMWithPK(CString PK)
{
     	
	for(INT i=0; i < GroupDM->GetRowCount();i++)
	{
		if(GroupDM->GetCellData(i,0) == PK)
		{
			GroupDM->DeleteRow(i,GroupDM->GetColCount());
		}
	}

	return 0;
}


VOID CToolDlg::OnSelchangeTabTool(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	INT index = m_Tab.GetCurSel();
	
	if(0==index)
	{
   		m_List.ShowWindow(false);
		m_Tree.ShowWindow(true);
	}
    else
	{
   		m_List.ShowWindow(true);
		m_Tree.ShowWindow(false);
	}
   
	*pResult = 0;
}

VOID CToolDlg::OnClickTreeTask(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM TreeItem = m_Tree.GetSelectedItem( );
	
	if( m_Tree.GetParentItem(TreeItem) )
	{
		 HTREEITEM TreeItemPar = m_Tree.GetParentItem(TreeItem);

		if( m_Tree.GetParentItem(TreeItemPar) )
		{
			///GROUP
			CString mTASK = m_Tree.GetItemText(TreeItemPar);
			SetTaskName(mTASK);


			return;
		}
		else
		{
			///TASK
			CString mTASK = m_Tree.GetItemText(TreeItem);
			SetTaskName(mTASK);
		}

	}
	else
	{

			///PROJECT
			SetTaskName(_T(""));
			return;

	}




	*pResult = 0;
}

VOID CToolDlg::OnDblclkTreeTask(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	
	*pResult = 0;
}

INT CToolDlg::Make_TaskSQL(CString mTASK)
{
	m_GroupList->RemoveAll();

	CString TmpQry,Tmp,Tmp2;
	INT Count = GroupDM->GetRowCount();

	TmpQry.Format(_T(" TASK_ALIAS = '%s' "), mTASK );

	m_Sql->Format(_T("%s"),TmpQry);

	return 0;
}

INT CToolDlg::Make_GroupSQL(CString mGROUP)
{
	m_GroupList->RemoveAll();
	INT dFlag = 1;
	CString TmpQry,Tmp,Tmp2;
	INT Count = GroupDM->GetRowCount();

	m_GroupList->AddHead(mGROUP);

	TmpQry.Format(_T(" in ( '%s' "), mGROUP);
	for ( INT i =0 ; i < Count ; i++)
	{
		if( mGROUP == GroupDM->GetCellData(i, 3) )
		{
			if (dFlag)
				TmpQry += _T(",");

			Tmp.Format(_T(" '%s' "),GroupDM->GetCellData(i, 1));
			Tmp2 += Tmp;

			m_GroupList->AddHead(GroupDM->GetCellData(i, 1));
		}
	}


	TmpQry += Tmp2;

	TmpQry += _T(" )");
	m_Sql->Format(_T("%s"),TmpQry);

	return 0;
}

INT CToolDlg::Init_View(CString Load_Type)
{

	m_Load_Type->Format(_T("%s"),Load_Type);
	::SendMessage(pMainFrame->GetSafeHwnd(),MSG_INIT_SQL,NULL,NULL);

	return 0;
}


INT CToolDlg::SetTaskName(CString TaskName)
{
	m_TASK->Format(_T("%s"),TaskName);


	for(INT i=0; i < TaskDM->GetRowCount() ; i++)
	{		
		if( TaskDM->GetCellData(i, 1) ==  TaskName)
		{
			m_TASKDESC->Format(_T("%s"),TaskDM->GetCellData(i, 3));
			return 0;	
		}

	}
	return 0;	
}

VOID CToolDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (!Init_Flag) return; 
	//Init SIZE
	CRect Rect,tmp;
	GetClientRect(&Rect);

	m_Tab.MoveWindow(Rect,true);

	tmp.top = Rect.top+30;
	tmp.bottom = Rect.bottom-2;
	tmp.left = Rect.left+2;
	tmp.right = Rect.right-2;

	m_List.MoveWindow(tmp);
	m_Tree.MoveWindow(tmp);


	// TODO: Add your message handler code here
	
}

VOID CToolDlg::OnSelchangedTreeTask(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM TreeItem = m_Tree.GetSelectedItem( );
	
	if( m_Tree.GetParentItem(TreeItem) )
	{
		 HTREEITEM TreeItemPar = m_Tree.GetParentItem(TreeItem);

		if( m_Tree.GetParentItem(TreeItemPar) )
		{
			///GROUP
			CString mTASK = m_Tree.GetItemText(TreeItemPar);
			SetTaskName(mTASK);

			CString mGROUP = m_Tree.GetItemText(TreeItem);
			Make_GroupSQL(mGROUP);

			Init_View(_T("GROUP"));

			return;
		}
		else
		{
			///TASK
			CString mTASK = m_Tree.GetItemText(TreeItem);
			Make_TaskSQL(mTASK);
			SetTaskName(mTASK);
			Init_View(_T("TASK"));
		}

	}
	else
	{

			///PROJECT
			SetTaskName(_T(""));
			return;

	}


	
	*pResult = 0;
}
