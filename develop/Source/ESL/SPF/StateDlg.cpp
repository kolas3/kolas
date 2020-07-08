// StateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SPF.h"
#include "StateDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStateDlg dialog


CStateDlg::CStateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStateDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pView=pParent;
	m_Index = 1;
	Init_Flag = false;

	// KOL.UDF.022 시큐어코딩 보완
	GroupDM = NULL;  
	StateDM = NULL;
	PathDM = NULL;
	m_pList = NULL;
	CurTabIndex = 0;
	m_CurObject = NULL;
	m_Qry = _T("");
}


VOID CStateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStateDlg)
	DDX_Control(pDX, IDC_gridBOTTOM, m_grid);
	DDX_Control(pDX, IDC_tabGRID, m_ctrlTAB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStateDlg, CDialog)
	//{{AFX_MSG_MAP(CStateDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabGRID, OnSelchangetabGRID)
	ON_BN_CLICKED(IDC_btnResINSERT, OnbtnResINSERT)
	ON_BN_CLICKED(IDC_btnResDELETE, OnbtnResDELETE)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStateDlg message handlers

BOOL CStateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	//tab에 붙일 index setting
	
	TCHAR scTABINDEX[][30] = {_T("GROUP"),_T("MENU RESOURCE"),_T("TOOLBUTTON RESOURCE")};
	TC_ITEM item;
	item.mask = TCIF_TEXT;
    
	for(INT i=0; i<3; i++)
	{
		item.pszText = scTABINDEX[i];
		m_ctrlTAB.InsertItem(i,&item);
	}

    CurTabIndex = 0;	

	ReadCfgFile.ReadCfgFile(_T(""));
	ResourceDM.SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	CRect Rect,tmp;
	GetClientRect(&Rect);

	m_ctrlTAB.MoveWindow(Rect,true);

	tmp.top = Rect.top+25;
	tmp.bottom = Rect.bottom-2;
	tmp.left = Rect.left+2;
	tmp.right = Rect.right-2;

	m_grid.MoveWindow(tmp);
	

	Init_Flag = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//화면에서 클릭한 object의 데이터를 보여준다
INT CStateDlg::ShowDataOnGrid()
{
    if(m_CurObject->m_ObjectType == 'G')
	{
		GetDlgItem(IDC_btnResINSERT)->ShowWindow(false);
		GetDlgItem(IDC_btnResDELETE)->ShowWindow(false);
		ShowGroupDataOnGrid();
	}
    else if(m_CurObject->m_ObjectType == 'S')
	{
		GetDlgItem(IDC_btnResINSERT)->ShowWindow(false);
		GetDlgItem(IDC_btnResDELETE)->ShowWindow(false);
		ShowStateDataOnGrid();
	}
	else if(m_CurObject->m_ObjectType == 'P')
	{
		GetDlgItem(IDC_btnResINSERT)->ShowWindow(true);
		GetDlgItem(IDC_btnResDELETE)->ShowWindow(true);
	    ShowPathDataOnGrid();
	}

	return 0;
}

INT CStateDlg::DeleteAllGrid()
{
    
    m_grid.SetCols(0,2);
	m_grid.SetRows(2);
    m_grid.SetTextMatrix(0,1,_T(""));
	m_grid.SetTextMatrix(1,1,_T(""));


	GetDlgItem(IDC_btnResINSERT)->ShowWindow(false);
	GetDlgItem(IDC_btnResDELETE)->ShowWindow(false);

	return 0;
}


INT CStateDlg::ShowGroupDataOnGrid()
{
    //해당하는 PK가 없으면 return
	if(m_CurObject->m_PK == _T("")) return 0;
	
	COLORREF white;
	white = RGB(255,255,255);
    
	m_ctrlTAB.SetCurSel(0);
    CurTabIndex = 0;

    m_grid.SetCols(0,6);
    m_grid.SetRows(2);
    m_grid.SetTextMatrix(0,1,_T("PK"));
	m_grid.SetTextMatrix(0,2,_T("ALIAS"));
	m_grid.SetColWidth(2, 0, 100*15);

    m_grid.SetTextMatrix(0,3,_T("TASK_ALIAS"));
	m_grid.SetColWidth(3, 0, 100*15);

	m_grid.SetTextMatrix(0,4,_T("PARENT_GROUP_ALIAS"));
	m_grid.SetColWidth(4, 0, 100*15);

	m_grid.SetTextMatrix(0,5,_T("DESCRIP"));
	m_grid.SetColWidth(5, 0, 400*15);

    
	//각 ObjectType에 해당하는 DM에서 정보들을 PK로 가져와 Grid에 보여준다 
    for(INT i=0; i<GroupDM->GetRowCount(); i++)
	{
		if(GroupDM->GetCellData(i,0) == m_CurObject->m_PK)
		{
             for(INT j=0; j<5;j++)
			 {
			   m_grid.SetTextMatrix(1,j+1,GroupDM->GetCellData(i,j));
			   m_grid.SetRow(1);
			   m_grid.SetCol(j+1);
			   m_grid.SetCellBackColor(white);
			 }
		}
	}
  	UpdateData(false);
	
	SPFInitRes();

	return 0;
}

INT CStateDlg::SPFInitRes()
{
	m_Qry = _T("");

	if(m_CurObject->m_ObjectType == 'G')
	{
		CString GroupAlias,StateAlias;
		CStringList TmpGroupList;
		CStringList TmpStateList;
		CStringList TmpPathList;

		TmpGroupList.AddTail(m_CurObject->m_ALIAS);
		
		//GROUP
		INT Count = GroupDM->GetRowCount();
		for ( INT i=0 ; i < Count ; i++)
		{
			GroupAlias=GroupDM->GetCellData(i,3);
			if ( m_CurObject->m_ALIAS == GroupAlias )
			{
				TmpGroupList.AddTail(GroupAlias);
			}

		}

		//STATE
		Count = StateDM->GetRowCount();
		for ( i=0 ; i < Count ; i++)
		{
			GroupAlias=StateDM->GetCellData(i,2);
			if (  TmpGroupList.Find(GroupAlias) != NULL )
			{
				TmpStateList.AddTail(StateDM->GetCellData(i,1));
			}

		}

		//PATH

		Count = PathDM->GetRowCount();
		for ( i=0 ; i < Count ; i++)
		{
			StateAlias=PathDM->GetCellData(i,3);
			if (  TmpStateList.Find(StateAlias) != NULL )
			{
				TmpPathList.AddTail(PathDM->GetCellData(i,1));
			}

		}

		m_Qry = SPFMakeResQry(&TmpPathList);
		
 
	}
    else if(m_CurObject->m_ObjectType == 'S')
	{
		//PATH
		INT Count;
		CStringList TmpPathList;
		CString StateAlias;

		Count = PathDM->GetRowCount();
		for (INT i=0 ; i < Count ; i++)
		{
			StateAlias=PathDM->GetCellData(i,3);
			if (  StateAlias == m_CurObject->m_ALIAS )
			{
				TmpPathList.AddTail(PathDM->GetCellData(i,1));
			}

		}

		m_Qry = SPFMakeResQry(&TmpPathList);

	}
	else if(m_CurObject->m_ObjectType == 'P')
	{
		//PATH
		CStringList TmpPathList;

		TmpPathList.AddTail(m_CurObject->m_ALIAS);

		m_Qry = SPFMakeResQry(&TmpPathList);

  
	}

	return 0 ;
}

CString CStateDlg::SPFMakeResQry(CStringList *m_List)
{

	CString tmpStr;
	for (INT i=0 ; i < m_List->GetCount() ; i++)
	{

		if (i!=0)
		tmpStr +=_T(" OR ");
		
		tmpStr += _T(" PATH_ALIAS = '");
		tmpStr += m_List->GetAt(m_List->FindIndex(i));
		tmpStr += _T("' ");

	}
	return tmpStr;
}


INT CStateDlg::ShowStateDataOnGrid()
{
    //해당하는 PK가 없으면 return
	if(m_CurObject->m_PK == _T("")) return 0;
	
	COLORREF white;
	white = RGB(255,255,255);
    
	m_ctrlTAB.SetCurSel(0);
    CurTabIndex = 0;

    m_grid.SetCols(0,5);
    m_grid.SetRows(2);
    m_grid.SetTextMatrix(0,1,_T("PK"));
	m_grid.SetTextMatrix(0,2,_T("ALIAS"));
	m_grid.SetColWidth(2, 0, 100*15);
    m_grid.SetTextMatrix(0,3,_T("GROUP_ALIAS"));
	m_grid.SetColWidth(3, 0, 150*15);
	m_grid.SetTextMatrix(0,4,_T("DESCRIP"));
	m_grid.SetColWidth(4, 0, 200*15);

    
	

	//각 ObjectType에 해당하는 DM에서 정보들을 PK로 가져와 Grid에 보여준다 
    for(INT i=0; i<StateDM->GetRowCount(); i++)
	{
		if(StateDM->GetCellData(i,0) == m_CurObject->m_PK)
		{
             for(INT j=0; j<4;j++)
			 {
			   m_grid.SetTextMatrix(1,j+1,StateDM->GetCellData(i,j));
			   m_grid.SetRow(1);
			   m_grid.SetCol(j+1);
			   m_grid.SetCellBackColor(white);
			 }
		}
	}
  
	UpdateData(false);

	SPFInitRes();

	return 0;
}

INT CStateDlg::ShowPathDataOnGrid()
{
   
	//해당하는 PK가 없으면 return
    if(m_CurObject->m_PK == _T("")) return 0;
	m_ctrlTAB.SetCurSel(0);
	CurTabIndex = 0;
    
	COLORREF white;
	white = RGB(255,255,255);
    
    m_grid.SetCols(0,10);
    m_grid.SetRows(2);
    m_grid.SetTextMatrix(0,1,_T("PK"));
	m_grid.SetTextMatrix(0,2,_T("ALIAS"));
	m_grid.SetColWidth(2, 0, 80*15);
    m_grid.SetTextMatrix(0,3,_T("PATH_TYPE"));
	m_grid.SetColWidth(3, 0, 50*15);
	m_grid.SetTextMatrix(0,4,_T("START_STATE_ALIAS"));
	m_grid.SetColWidth(4, 0, 100*15);

	m_grid.SetTextMatrix(0,5,_T("END_STATE_ALIAS"));
	m_grid.SetColWidth(5, 0, 100*15);

	m_grid.SetTextMatrix(0,6,_T("ISACTIVATED"));
	m_grid.SetColWidth(6, 0, 100*15);

	m_grid.SetTextMatrix(0,7,_T("SOURCE_FILE_NAME"));
	m_grid.SetColWidth(7, 0, 150*15);
    m_grid.SetTextMatrix(0,8,_T("SOURCE_FUNCTION_NAME"));
	m_grid.SetColWidth(8, 0, 150*15);
	m_grid.SetTextMatrix(0,9,_T("DESCRIP"));
	m_grid.SetColWidth(9, 0, 150*15);

	//각 ObjectType에 해당하는 DM에서 정보들을 PK로 가져와 Grid에 보여준다 
	for(INT i=0; i<PathDM->GetRowCount(); i++)
	{
		if(PathDM->GetCellData(i,0) == m_CurObject->m_PK)
		{
			if(PathDM->GetCellData(i,2) == _T("B"))//BACK PATH는 두번째 행에 data를 보여준다 
            {
				m_grid.SetRows(3);
				for(INT j=0; j<9;j++)
				{
				  m_grid.SetTextMatrix(2,j+1,PathDM->GetCellData(i,j));
				  m_grid.SetRow(2);
			      m_grid.SetCol(j+1);
			      m_grid.SetCellBackColor(white);
              	}
			}
			else
			{
                for(INT j=0; j<9;j++)
				{
				  m_grid.SetTextMatrix(1,j+1,PathDM->GetCellData(i,j));
				  m_grid.SetRow(1);
			      m_grid.SetCol(j+1);
			      m_grid.SetCellBackColor(white);
				}
			}
		
		}
	}
  
	UpdateData(false);

	SPFInitRes();

	return 0;
}

VOID CStateDlg::OnSelchangetabGRID(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	INT index = m_ctrlTAB.GetCurSel();
	
	if(0==index)
	{
       ShowDataOnGrid();
	}
    else
	{
       ShowRESDataOnGrid(); 
	}
   
	*pResult = 0;
}

INT CStateDlg::ShowRESDataOnGrid()
{

    //ResourceDM setting
	CString Qry;
	INT index = m_ctrlTAB.GetCurSel();

    Qry = _T("select PK,ALIAS,PATH_ALIAS,RESOURCE_TYPE from SPF_PATH_RESOURCE_TBL");
	
	m_Qry.TrimLeft();
	m_Qry.TrimRight();

	if( m_Qry.GetLength() != 0)
	{
		Qry += _T(" WHERE ");

		INT index = m_ctrlTAB.GetCurSel();
		Qry += m_Qry;

		ResourceDM.RestructDataManager(Qry);

	}
	else
	{
		ResourceDM.FreeData();
	}


	INT ResCount=0;

	if ( index == 1 ) 
	{	ResCount=GetCountRes(_T("M"));	}
	else if ( index == 2 )
	{	ResCount=GetCountRes(_T("B"));	}
	

	if ( ResCount )
	{

		m_grid.SetCols(0,4);


		m_grid.SetRows(ResCount+1);

		m_grid.SetTextMatrix(0,1,_T("PK"));
		m_grid.SetTextMatrix(0,2,_T("RESOURCE_ALIAS"));
		m_grid.SetTextMatrix(0,3,_T("PATH_TYPE"));
		INT MenuCount = 0 , ButtonCount = 0;

		for( INT i =0 ; i < ResourceDM.GetRowCount() ; i++)
		{
			if(  index == 1 )
			{
				if(ResourceDM.GetCellData(i,3) == _T("M"))
				{
				  m_grid.SetTextMatrix(MenuCount+1,1,ResourceDM.GetCellData(i,0));
				  m_grid.SetTextMatrix(MenuCount+1,2,ResourceDM.GetCellData(i,1));
				  m_grid.SetTextMatrix(MenuCount+1,3,ResourceDM.GetCellData(i,2));
				  MenuCount++;
				}
			}
			else
			if(  index == 2 )
			{
				if(ResourceDM.GetCellData(i,3) == _T("B"))
				{
				  m_grid.SetTextMatrix(ButtonCount+1,1,ResourceDM.GetCellData(i,0));
				  m_grid.SetTextMatrix(ButtonCount+1,2,ResourceDM.GetCellData(i,1));
				  m_grid.SetTextMatrix(ButtonCount+1,3,ResourceDM.GetCellData(i,2));
				  ButtonCount++;
				}
			}

		}
		

	}
	else
	{
		m_grid.SetCols(0,4);
		m_grid.SetRows(2);
		m_grid.SetTextMatrix(0,1,_T("PK"));
		m_grid.SetTextMatrix(0,2,_T("RESOURCE_ALIAS"));
		m_grid.SetTextMatrix(0,3,_T("PATH_TYPE"));

		m_grid.SetTextMatrix(1,1,_T(""));
		m_grid.SetTextMatrix(1,2,_T(""));
		m_grid.SetTextMatrix(1,3,_T(""));
	}


	return 0;
}


BEGIN_EVENTSINK_MAP(CStateDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CStateDlg)
	ON_EVENT(CStateDlg, IDC_gridBOTTOM, -600 /* Click */, OnClickgridBOTTOM, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()



VOID CStateDlg::OnClickgridBOTTOM() 
{
	// TODO: Add your control notification handler code here
	COLORREF blue,white;
	blue = RGB(228,243,251);
	white = RGB(255,255,255);

	INT ColCount = m_grid.GetCols(0);
	INT RowCount = m_grid.GetRows();
	if ( ColCount == 0 ) return; 
    INT row = m_grid.GetRow();
    

	for (INT i = 1 ; i < ColCount ;i++)
	{
		for(INT j = 1 ; j < RowCount ; j++)
		{
			m_grid.SetRow(j);
			m_grid.SetCol(i);
			m_grid.SetCellBackColor(white);
		}
	}

	m_grid.SetRow(row);
	for ( i = 1 ; i < ColCount ;i++)
	{
		m_grid.SetCol(i);
		m_grid.SetCellBackColor(blue);
	}

	m_Index = row;
}

INT CStateDlg::ChangeTabText()
{
    TC_ITEM item;
    m_ctrlTAB.GetItem(0,&item);
    item.mask = TCIF_TEXT;

	if(m_CurObject->m_ObjectType == 'G') 
	{    
        item.pszText = _T("GROUP");
        m_ctrlTAB.SetItem(0,&item);
	}
	else if(m_CurObject->m_ObjectType == 'S')
    {
		item.pszText = _T("STATE");
        m_ctrlTAB.SetItem(0,&item);
	}
	else if(m_CurObject->m_ObjectType == 'P')
	{
        item.pszText = _T("PATH");
        m_ctrlTAB.SetItem(0,&item);
	}

	return 0;
}

VOID CStateDlg::OnbtnResINSERT() 
{
	// TODO: Add your control notification handler code here
	
	INT ids ;

	INT index = m_ctrlTAB.GetCurSel();

	if( index == 0) return;

	if(index == 1)
	{

		CString Menu_Alias;
		CSPFMenuInsert Dlg;
		
		if( Dlg.DoModal() == IDOK )
		{		

				
			for( INT i =0 ; i < Dlg.m_Seleted.GetCount(); i++ )
			{
				
				Menu_Alias = Dlg.m_Seleted.GetAt(Dlg.m_Seleted.FindIndex(i));
				
				if( !FindAlias(Menu_Alias) )
					{
						
						ids=SPFInsertMenu( Menu_Alias ); 

					}
			
			}


		}

	}
	else if(index == 2)
	{

		CString Button_Alias;
		CSPFButtonInsert Dlg;
		
		for( INT i =0 ; i < ResourceDM.GetRowCount() ; i++)
		{
			if( ResourceDM.GetCellData(i,3) == _T("M"))
			{
				Dlg.m_MenuList.AddTail( ResourceDM.GetCellData(i,1) );
			}
		}

		
		if( Dlg.DoModal() == IDOK )
		{		

				
			for( i =0 ; i < Dlg.m_Seleted.GetCount(); i++ )
			{
				
				Button_Alias = Dlg.m_Seleted.GetAt(Dlg.m_Seleted.FindIndex(i));
				
				if( !FindAlias(Button_Alias) )
					{
						
						ids=SPFInsertButton( Button_Alias ); 

					}
			
			}


		}
	}

	ShowRESDataOnGrid();

}

INT CStateDlg::FindAlias(CString Menu_Alias)
{

	CString Tmp_Alias; 

	for ( INT i = 1 ; i < m_grid.GetRows() ;  i++)
	{
		
		Tmp_Alias=m_grid.GetTextMatrix( i , 2 );
		if( Menu_Alias == Tmp_Alias )
		{
			return 1;
		}
	}
	return 0;
}

INT CStateDlg::SPFInsertMenu(CString Menu_Alias)
{

	CString Qry;

	Qry.Format(_T("INSERT INTO SPF_PATH_RESOURCE_TBL VALUES ( %s.nextval , '%s' , '%s' ,'M'); "),ReadCfgFile.m_sSequence ,Menu_Alias ,m_CurObject->m_ALIAS);  

	ResourceDM.AddFrame(Qry);

	ResourceDM.SendFrame(TRUE);
	ResourceDM.EndFrame();

	
	return 0;
}

INT CStateDlg::SPFInsertButton(CString Button_Alias)
{

	CString Qry;

	Qry.Format(_T("INSERT INTO SPF_PATH_RESOURCE_TBL VALUES ( %s.nextval , '%s' , '%s' ,'B'); "),ReadCfgFile.m_sSequence ,Button_Alias ,m_CurObject->m_ALIAS);  

	ResourceDM.AddFrame(Qry);

	ResourceDM.SendFrame(TRUE);
	ResourceDM.EndFrame();

	
	return 0;
}

VOID CStateDlg::OnbtnResDELETE() 
{
	// TODO: Add your control notification handler code here
	// KOL.UDF.022 시큐어코딩 보완
	INT Count = 0;
	CString m_Qry;
	INT index = m_ctrlTAB.GetCurSel();

	if( index == 0) return;
	

	if(index == 1)
	{
		Count = GetCountRes (_T("M"));
	}
	else if(index == 2)
	{
		Count = GetCountRes (_T("B"));
	}
	
	if( Count == 0 ) return;

	index =	m_grid.GetRow();
	if( index == 0) return;

	if( AfxMessageBox(_T("정말 지우시겠습니까?"), MB_YESNO) == IDYES )
	{
		CString RES_PK =m_grid.GetTextMatrix(index,1);
			
		m_Qry.Format(_T("DELETE FROM SPF_PATH_RESOURCE_TBL WHERE PK = %s ;"), RES_PK);
		ResourceDM.AddFrame(m_Qry);
		ResourceDM.SendFrame(TRUE);
		ResourceDM.EndFrame();
	}

	ShowRESDataOnGrid();
}


INT CStateDlg::GetCountRes(CString Type)
{
	INT ResCount=0;

	if ( Type == _T("M") ) 
	{
		for( INT k =0 ; k <  ResourceDM.GetRowCount() ; k++)
		{
			if( ResourceDM.GetCellData(k,3) == _T("M"))
				ResCount++;
		}

	}
	else if ( Type == _T("B") )
	{
		for( INT k =0 ; k <  ResourceDM.GetRowCount() ; k++)
		{
			if( ResourceDM.GetCellData(k,3) == _T("B"))
				ResCount++;
		}
	}


	return ResCount;
}

VOID CStateDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	
	if(!Init_Flag) return ;

	CRect Rect,tmp;
	GetClientRect(&Rect);

	m_ctrlTAB.MoveWindow(Rect,true);

	tmp.top = Rect.top+25;
	tmp.bottom = Rect.bottom- 30;
	tmp.left = Rect.left+2;
	tmp.right = Rect.right-2;

	m_grid.MoveWindow(tmp);
	// TODO: Add your message handler code here
	
}
