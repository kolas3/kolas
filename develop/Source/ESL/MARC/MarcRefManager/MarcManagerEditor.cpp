// MarcManagerEditor.cpp : implementation file
//

#include "stdafx.h"
#include "MarcManager.h"
#include "MarcManagerEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerEditor dialog


CMarcManagerEditor::CMarcManagerEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CMarcManagerEditor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcManagerEditor)
	m_sEditorAlias = _T("");
	//}}AFX_DATA_INIT
	// KOL.UDF.022
	m_pEditorDM = NULL;
	m_pEditorGrid = NULL;
	m_nCurrentGridRow = 0;
}


VOID CMarcManagerEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcManagerEditor)
	DDX_Text(pDX, IDC_eEditorAlias, m_sEditorAlias);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarcManagerEditor, CDialog)
	//{{AFX_MSG_MAP(CMarcManagerEditor)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bEditorSearch, OnbEditorSearch)
	ON_BN_CLICKED(IDC_bEditorInsert, OnbEditorInsert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerEditor message handlers

BOOL CMarcManagerEditor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pEditorDM = NULL;	
	m_pEditorDM = new CESL_DataMgr;
 
	//connection setting
	ReadCfgFile.ReadCfgFile(_T(""));
	m_pEditorDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	// Grid 초기화
	m_pEditorGrid = new CESL_Grid(NULL);
	m_pEditorGrid->SubclassDlgItem(IDC_gEditorGrid, this);
    
	InitEditorGrid();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMarcManagerEditor::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_pEditorDM) 
	{
       delete m_pEditorDM;
       m_pEditorDM = NULL;
	}
     
	if(m_pEditorGrid) 
	{
       delete m_pEditorGrid;
       m_pEditorGrid = NULL;
	}

}

VOID CMarcManagerEditor::InitEditorGrid()
{
    m_pEditorGrid->SetCols(0,5);
    m_pEditorGrid->SetColumnInfo(0-1, _T("ALIAS"),			200,			4);
	m_pEditorGrid->SetColumnInfo(1-1, _T("TYPE"),		    40,	  			4);
	m_pEditorGrid->SetColumnInfo(2-1, _T("태그코드"),	    60,				4);
	m_pEditorGrid->SetColumnInfo(3-1, _T("DATA"),			340,			4);
	m_pEditorGrid->SetColumnInfo(4-1, _T("INDEX"),	        40,				4);
  
}

VOID CMarcManagerEditor::OnbEditorSearch() 
{
 	// TODO: Add your control notification handler code here
    ShowEditorSearchData();
}

INT CMarcManagerEditor::ShowEditorSearchData()
{
    UpdateData(TRUE);
	
	CString query,tmpquery;
	
	query.Format(_T("SELECT PK, ALIAS, ALIAS_TYPE, TAG_CODE, DATA, INDEX_FLAG FROM MARC_ALIAS_TBL"));
  	
	if(!m_sEditorAlias.IsEmpty())
	{
		tmpquery.Format(_T(" WHERE ALIAS like '%%%s%%' "),m_sEditorAlias);
		query += tmpquery;
	}
	
    tmpquery = _T(" ORDER BY ALIAS");
    query += tmpquery;
    
	m_pEditorDM->RestructDataManager(query);
    //////////////////////////////////////////////////////////////////
	
	
	INT nRow = m_pEditorDM->GetRowCount();
	INT gridrow;
	if (nRow == 0) gridrow = 2;
		else gridrow = nRow+1;
	m_pEditorGrid->RemoveAll();
	m_pEditorGrid->SetRows(gridrow);
	InitEditorGrid();
	
	
	for (INT i = 0; i < nRow; i ++) {
		m_pEditorGrid->SetTextMatrix(i+1, 0, m_pEditorDM->GetCellData(i, 1));
		m_pEditorGrid->SetTextMatrix(i+1, 1, m_pEditorDM->GetCellData(i, 2));
	    m_pEditorGrid->SetTextMatrix(i+1, 2, m_pEditorDM->GetCellData(i, 3));
		m_pEditorGrid->SetTextMatrix(i+1, 3, m_pEditorDM->GetCellData(i, 4));
	    m_pEditorGrid->SetTextMatrix(i+1, 4, m_pEditorDM->GetCellData(i, 5));
	
	}     
	


	return 0;
}

CString CMarcManagerEditor::IsNullData(CString data)
{
    if(data.IsEmpty())
	{
		data = _T("NULL");
	}
	else
	{
		CString tmpdata;
		tmpdata.Format(_T("'%s'"),data);
		data = tmpdata;
	}

	return data;
}

INT CMarcManagerEditor::DeleteEditorData()
{
    if(m_pEditorDM->GetRowCount == 0) return 0;
	
	CString tmpPK,tmpalias;
    tmpPK = m_pEditorDM->GetCellData(m_nCurrentGridRow-1,0);
    tmpalias = m_pEditorDM->GetCellData(m_nCurrentGridRow-1,1);
    
	CString msg;
	msg.Format(_T("ALIAS가  '%s' 인 항목을 삭제하시겠습니까?"),tmpalias);
    if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;
     
    CString query;
	query.Format(_T("DELETE FROM MARC_ALIAS_TBL WHERE PK = %s;"),tmpPK);
    
	m_pEditorDM->AddFrame(query);
    INT ids = m_pEditorDM->SendFrame();
    if(ids < 0) AfxMessageBox(_T("삭제과정에서 에러가 발생하였습니다!")); 
    m_pEditorDM->EndFrame();
    
	ShowEditorSearchData(); //refresh

	return 0;
}

BEGIN_EVENTSINK_MAP(CMarcManagerEditor, CDialog)
    //{{AFX_EVENTSINK_MAP(CMarcManagerEditor)
	ON_EVENT(CMarcManagerEditor, IDC_gEditorGrid, -605 /* MouseDown */, OnMouseDowngEditorGrid, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMarcManagerEditor, IDC_gEditorGrid, -601 /* DblClick */, OnDblClickgEditorGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMarcManagerEditor::OnMouseDowngEditorGrid(short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	INT row = m_pEditorGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
      m_pEditorGrid->SetReverse(row-1);
	  m_nCurrentGridRow = row;
	}


}

VOID CMarcManagerEditor::OnDblClickgEditorGrid() 
{
	// TODO: Add your control notification handler code here
	if(m_pEditorDM->GetRowCount() == 0) return;
    if(m_nCurrentGridRow <0) return;

	CMarcManagerEditorInsert dlg(this);
    	   
	dlg.m_sEditorAlias = m_pEditorGrid->GetTextMatrix(m_nCurrentGridRow,0);
	dlg.m_sEditorTagCode = m_pEditorGrid->GetTextMatrix(m_nCurrentGridRow,2);
	dlg.m_sEditorData = m_pEditorGrid->GetTextMatrix(m_nCurrentGridRow,3);
	
    if(m_pEditorGrid->GetTextMatrix(m_nCurrentGridRow,1) == _T("$"))
	{
       dlg.m_nEditorAliasType = 0;
	}
    else if(m_pEditorGrid->GetTextMatrix(m_nCurrentGridRow,1) == _T("*"))
	{
	   dlg.m_nEditorAliasType = 1;	
	}
	else
	{
       dlg.m_nEditorAliasType = 2;	
    }

    if(m_pEditorGrid->GetTextMatrix(m_nCurrentGridRow,4) == _T("Y"))
	{
       dlg.m_nEditorIdex= 0;
	}
    else 
	{
       dlg.m_nEditorIdex= 1;
	}

	UpdateData(false);
	if(dlg.DoModal() != IDOK) return;
    UpdateData(true);
	 
	
/*
	CString tmptime;
    CTime time = m_pEditorDM->GetDBTime();
    tmptime.Format(_T("%d/%d/%d"),time.GetYear(),time.GetMonth(),time.GetDay()); //DB TIME SETTING 
    tmptime = IsNullData(tmptime);
*/ 
   
    CString tmpdata,tmpcode,tmpPK,tmptype,tmpindex;
    tmpdata = IsNullData(dlg.m_sEditorData);
    tmpcode = IsNullData(dlg.m_sEditorTagCode);
    tmpPK = m_pEditorDM->GetCellData(m_nCurrentGridRow-1,0); 

	if(dlg.m_nEditorAliasType == 0)
	{
        tmptype = _T("'$'");
	}
	else if(dlg.m_nEditorAliasType == 1)
	{
        tmptype = _T("'*'");
	}
	else
	{
		tmptype = _T("'@'");
	}

	
	if(dlg.m_nEditorIdex == 0)
    {
	    tmpindex = _T("'Y'");
	}
	else 
	{
		tmpindex = _T("'N'");
	}
	
	//update query
	CString query,tmpquery;
    m_pEditorDM->StartFrame();
    
	query.Format(_T("UPDATE MARC_ALIAS_TBL SET ALIAS = ")
	_T("'%s', ALIAS_TYPE = %s ,TAG_CODE = %s ,DATA = %s ,INDEX_FLAG = %s"),dlg.m_sEditorAlias,tmptype,tmpcode,tmpdata,tmpindex);
    
    tmpquery.Format(_T(" WHERE PK = %s;"),tmpPK);
    
	query += tmpquery;

	m_pEditorDM->AddFrame(query);
    INT ids = m_pEditorDM->SendFrame();
    if(ids < 0) AfxMessageBox(_T("수정과정에서 에러가 발생하였습니다!")); 
    m_pEditorDM->EndFrame();
    
	ShowEditorSearchData(); //refresh

}

VOID CMarcManagerEditor::OnbEditorInsert() 
{
	// TODO: Add your control notification handler code here
	CMarcManagerEditorInsert dlg(this);
	
	if(dlg.DoModal() != IDOK) return;
    UpdateData(true);
     
	CString tmpdata,tmpcode,tmptype,tmpindex;
    tmpdata = IsNullData(dlg.m_sEditorData);
    tmpcode = IsNullData(dlg.m_sEditorTagCode);
    

	if(dlg.m_nEditorAliasType == 0)
	{
        tmptype = _T("'$'");
	}
	else if(dlg.m_nEditorAliasType == 1)
	{
        tmptype = _T("'*'");
	}
	else
	{
		tmptype = _T("'@'");
	}

	
	if(dlg.m_nEditorIdex == 0)
    {
	    tmpindex = _T("'Y'");
	}
	else 
	{
		tmpindex = _T("'N'");
	}
	
	//update query
	CString query,tmpquery;
    m_pEditorDM->StartFrame();
   
	query.Format(_T("INSERT INTO MARC_ALIAS_TBL (PK,ALIAS,ALIAS_TYPE,TAG_CODE,DATA,INDEX_FLAG)")
	_T(" VALUES (%s.NEXTVAL, '%s', %s , %s ,%s, %s);"), ReadCfgFile.m_sSequence ,dlg.m_sEditorAlias,tmptype,tmpcode,tmpdata,tmpindex);

	m_pEditorDM->AddFrame(query);
    INT ids = m_pEditorDM->SendFrame();
    if(ids < 0) AfxMessageBox(_T("수정과정에서 에러가 발생하였습니다!")); 
    m_pEditorDM->EndFrame();
    
	ShowEditorSearchData(); //refresh


}

BOOL CMarcManagerEditor::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DELETE)
	{
        DeleteEditorData();
	}

	return CDialog::PreTranslateMessage(pMsg);
}
