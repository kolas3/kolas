// MarcManagerView.cpp : implementation of the CMarcManagerView class
//

#include "stdafx.h"
#include "MarcManager.h"

#include "MarcManagerDoc.h"
#include "MarcManagerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "MarcManagerWorkView.h"
#include "MarcManagerTagInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerView

IMPLEMENT_DYNCREATE(CMarcManagerView, CFormView)

BEGIN_MESSAGE_MAP(CMarcManagerView, CFormView)
	//{{AFX_MSG_MAP(CMarcManagerView)
	ON_BN_CLICKED(IDC_bTagSearch, OnbTagSearch)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tTagSearch, OnSelchangetTagSearch)
	ON_COMMAND(ID_GROUP_SELECT, OnGroupSelect)
	ON_COMMAND(ID_TAG_INSERT, OnTagInsert)
	ON_COMMAND(ID_GROUP_MANAGE, OnGroupManage)
	ON_COMMAND(ID_RELATION_INSERT, OnRelationInsert)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerView construction/destruction

CMarcManagerView::CMarcManagerView()
	: CFormView(CMarcManagerView::IDD)
{
	//{{AFX_DATA_INIT(CMarcManagerView)
	m_sTagCode = _T("");
	m_sTagName = _T("");
	m_sGroupCodeOnView = _T("");
	//}}AFX_DATA_INIT
	// TODO: add construction code here
    m_pTagSearchDM = new CESL_DataMgr;
    m_pRelationSearchDM = new CESL_DataMgr;

	m_sMarcGroup = _T("");
    m_nCurrentGridRow = 0;

	// KOL.UDF.022
	m_pMainFrame = NULL;
	m_pTagSearchGrid = NULL;	
}


CMarcManagerView::~CMarcManagerView()
{
    if (m_pTagSearchDM) {
		delete m_pTagSearchDM;
		m_pTagSearchDM = NULL;
	}
 
	
	if (m_pRelationSearchDM) {
		delete m_pRelationSearchDM;
		m_pRelationSearchDM =NULL;
	}
	
	
	if (m_pTagSearchGrid) {
		delete m_pTagSearchGrid;
		m_pTagSearchGrid = NULL;
	}

}

VOID CMarcManagerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcManagerView)
	DDX_Control(pDX, IDC_tTagSearch, m_tab);
	DDX_Text(pDX, IDC_eTagCode, m_sTagCode);
	DDX_Text(pDX, IDC_eTagName, m_sTagName);
	DDX_Text(pDX, IDC_eGroupCode, m_sGroupCodeOnView);
	//}}AFX_DATA_MAP
}

BOOL CMarcManagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

VOID CMarcManagerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

    //connection setting
	ReadCfgFile.ReadCfgFile(_T(""));
	m_pTagSearchDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	m_pRelationSearchDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);


	// Grid 초기화
	m_pTagSearchGrid = new CESL_Grid(NULL);
	m_pTagSearchGrid->SubclassDlgItem(IDC_gTagSearch, this);
    InitTagGrid();


	//tab 초기화 
    m_tab.InsertItem(0, _T("태그"));
    m_tab.InsertItem(1, _T("관계"));
    
    //FRAME Pointer 
	m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
    
    GetDlgItem(IDC_eGroupCode)->EnableWindow(FALSE);

}

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerView printing

BOOL CMarcManagerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

VOID CMarcManagerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

VOID CMarcManagerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

VOID CMarcManagerView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerView diagnostics

#ifdef _DEBUG
VOID CMarcManagerView::AssertValid() const
{
	CFormView::AssertValid();
}

VOID CMarcManagerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMarcManagerDoc* CMarcManagerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMarcManagerDoc)));
	return (CMarcManagerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerView message handlers

VOID CMarcManagerView::InitTagGrid()
{
    m_pTagSearchGrid->SetCols(0,3);
	m_pTagSearchGrid->SetColumnInfo(0-1, _T(""),	     0,	        4);
	m_pTagSearchGrid->SetColumnInfo(1-1, _T("코드"),	50, 		4);
    m_pTagSearchGrid->SetColumnInfo(2-1, _T("이름"),   175, 		4);

}
VOID CMarcManagerView::InitRelationGrid()
{
    m_pTagSearchGrid->SetCols(0,3);
	m_pTagSearchGrid->SetColumnInfo(0-1, _T(""),			 0,	        4);
	m_pTagSearchGrid->SetColumnInfo(1-1, _T("연산기호"),	70, 		4);
    m_pTagSearchGrid->SetColumnInfo(2-1, _T("기준인자"),   155, 		4);

}

INT CMarcManagerView::TagSearch()
{
       
	ShowTagSearchData();

    return 0;
}

INT CMarcManagerView::ShowTagSearchData()
{
    UpdateData(TRUE);
	
	CString query,tmpquery;
	
	query.Format(_T("SELECT PK, ALIAS, TAG_CODE, TAG_NAME FROM MARC_TAG_TBL"));
       
	tmpquery.Format(_T(" WHERE MARC_GROUP_CODE = '%s'"),m_sMarcGroup);
    query += tmpquery;
	
	
	if(!m_sTagCode.IsEmpty())
	{
		tmpquery.Format(_T(" AND TAG_CODE like '%%%s%%' "),m_sTagCode);
		query += tmpquery;
	}
	
	if(!m_sTagName.IsEmpty() )
	{
		tmpquery.Format(_T("AND TAG_NAME like '%%%s%%' "),m_sTagName);
		query += tmpquery;
	}    
	
	tmpquery = _T(" ORDER BY TAG_CODE ");
    query += tmpquery;

	///////////////////현재는 1000개까지만 검색이됨
    //tmpquery = _T(" AND ROWNUM < 1001 ORDER BY TAG_CODE ");
    //query += tmpquery;
    
	m_pTagSearchDM->RestructDataManager(query);
    //////////////////////////////////////////////////////////////////
	
	
	INT nRow = m_pTagSearchDM->GetRowCount();
	INT gridrow;
	if (nRow == 0) gridrow = 2;
		else gridrow = nRow+1;
	m_pTagSearchGrid->RemoveAll();
	m_pTagSearchGrid->SetRows(gridrow);
	InitTagGrid();
	
	
	for (INT i = 0; i < nRow; i ++) {
		m_pTagSearchGrid->SetTextMatrix(i+1, 1, m_pTagSearchDM->GetCellData(i, 2));
		m_pTagSearchGrid->SetTextMatrix(i+1, 2, m_pTagSearchDM->GetCellData(i, 3));
	} 

	return 0;
}

INT CMarcManagerView::RelationSearch()
{
    UpdateData(TRUE);
	
	CString query,tmpquery;
	
	query.Format(_T("SELECT PK, OPERATOR, BASIC_PARAM FROM MARC_RELATION_TBL"));
       
	tmpquery.Format(_T(" WHERE MARC_GROUP_CODE = '%s'"),m_sMarcGroup);
    query += tmpquery;
	
	
	if(!m_sTagCode.IsEmpty())
	{
		tmpquery.Format(_T(" AND OPERATOR like '%%%s%%' "),m_sTagCode);
		query += tmpquery;
	}
	
	if(!m_sTagName.IsEmpty() )
	{
		tmpquery.Format(_T("AND BASIC_PARAM like '%%%s%%' "),m_sTagName);
		query += tmpquery;
	}    
	
	//////현재로서는 1000개 까지 출력 
    tmpquery = _T("  AND ROWNUM < 1001 ORDER BY OPERATOR");
    query += tmpquery;
    
	m_pRelationSearchDM->RestructDataManager(query);
    //////////////////////////////////////////////////////////////////
	
	
	INT nRow = m_pRelationSearchDM->GetRowCount();
	INT gridrow;
	if (nRow == 0) gridrow = 2;
		else gridrow = nRow+1;
	m_pTagSearchGrid->RemoveAll();
	m_pTagSearchGrid->SetRows(gridrow);
	InitRelationGrid();

	
	for (INT i = 0; i < nRow; i ++) {
		m_pTagSearchGrid->SetTextMatrix(i+1, 1, m_pRelationSearchDM->GetCellData(i, 1));
		m_pTagSearchGrid->SetTextMatrix(i+1, 2, m_pRelationSearchDM->GetCellData(i, 2));
	} 


    return 0;
}

VOID CMarcManagerView::OnbTagSearch() 
{
	// TODO: Add your control notification handler code here
	////////////////////////////////////////////////////////////////
	/////tag search query 생성부분 

	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	CMarcManagerWorkView *pWorkView = (CMarcManagerWorkView *)pframe->GetSecondView();
    
	pWorkView->ClearAllTagGrid();
	pWorkView->ClearAllRelationGrid();
    pWorkView->m_sTagAlias = _T("");
	
	if(m_tab.GetCurSel() == 0)
	{
       TagSearch();
	}
	else
	{
       RelationSearch();
	}

}


INT CMarcManagerView::DeleteTagWithRow(INT currow)
{
    CString tmpPK,tmpAlias,tmpCode,tmpName;
   
	tmpPK = m_pTagSearchDM->GetCellData(currow-1,0);
	tmpAlias = m_pTagSearchDM->GetCellData(currow-1,1);
	tmpCode = m_pTagSearchDM->GetCellData(currow-1,2);
    tmpName = m_pTagSearchDM->GetCellData(currow-1,3);
	
	CString msg;
	msg.Format(_T("이름이 '%s' 인  %s 코드를  '%s' 그룹에서 삭제하시겠습니까?"),tmpName,tmpCode,m_sMarcGroup);
    if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;

	//delete query
	CString query;
    m_pTagSearchDM->StartFrame();
    
	query.Format(_T("DELETE FROM MARC_FIRST_INDICATOR_TBL WHERE MARC_TAG_ALIAS = '%s';"),tmpAlias);
    m_pTagSearchDM->AddFrame(query);
    
	query.Format(_T("DELETE FROM MARC_SECOND_INDICATOR_TBL WHERE MARC_TAG_ALIAS = '%s';"),tmpAlias);
    m_pTagSearchDM->AddFrame(query);
	
	query.Format(_T("DELETE FROM MARC_SUBFIELD_TBL WHERE MARC_TAG_ALIAS = '%s';"),tmpAlias);
    m_pTagSearchDM->AddFrame(query);
	
	query.Format(_T("DELETE FROM MARC_TAG_TBL WHERE PK = %s;"),tmpPK);
    m_pTagSearchDM->AddFrame(query);
    
	INT ids = m_pTagSearchDM->SendFrame();
    m_pTagSearchDM->EndFrame();

	if(ids<0) AfxMessageBox(_T("삭제하던 중 에러가 발생하였습니다"));
	
	ShowTagSearchData(); //refresh
     
	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	CMarcManagerWorkView *pWorkView = (CMarcManagerWorkView *)pframe->GetSecondView();

	pWorkView->ClearAllTagGrid();

	return 0;
}

INT CMarcManagerView::DeleteRelationWithRow(INT currow)
{
    CString tmpPK,tmpopertor,tmpbasicparam;
   
	tmpPK = m_pRelationSearchDM->GetCellData(currow-1,0);
    
	tmpopertor = m_pRelationSearchDM->GetCellData(currow-1,1);
    tmpbasicparam = m_pRelationSearchDM->GetCellData(currow-1,2);
	
	CString msg;
	msg.Format(_T("연산자가 %s 이고 기준인자가  %s 인 관계를  %s 그룹에서 삭제하시겠습니까?"),tmpopertor,tmpbasicparam,m_sMarcGroup);
    if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;

	//delete query
	CString query;
    m_pRelationSearchDM->StartFrame();

    m_pRelationSearchDM->AddFrame(query);
	
	query.Format(_T("DELETE FROM MARC_RELATION_TBL WHERE PK = %s;"),tmpPK);
    m_pRelationSearchDM->AddFrame(query);
    
	INT ids = m_pRelationSearchDM->SendFrame();
    m_pRelationSearchDM->EndFrame();

	if(ids<0) AfxMessageBox(_T("삭제하던 중 에러가 발생하였습니다"));
	
	RelationSearch(); //refresh
     
	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	CMarcManagerWorkView *pWorkView = (CMarcManagerWorkView *)pframe->GetSecondView();

	pWorkView->ClearAllRelationGrid();
 


    return 0;
}


CString CMarcManagerView::IsNullData(CString data)
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


INT CMarcManagerView::ShowTagDataOnWorkView()
{
    INT nRow = m_pTagSearchDM->GetRowCount();
	
	if (nRow == 0) return 0;
	INT row = m_pTagSearchGrid->GetRow();
	if (row == 0) return 0;

	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	CMarcManagerWorkView *pWorkView = (CMarcManagerWorkView *)pframe->GetSecondView();

	
	pWorkView->m_sTagAlias = m_pTagSearchDM->GetCellData(row-1, 1);
	pWorkView->ShowTagData();

    return 0;
}

INT CMarcManagerView::ShowRelationDataOnWorkView()
{
    INT nRow = m_pRelationSearchDM->GetRowCount();
	
	if (nRow == 0) return 0;
	INT row = m_pTagSearchGrid->GetRow();
	if (row == 0) return 0;

	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	CMarcManagerWorkView *pWorkView = (CMarcManagerWorkView *)pframe->GetSecondView();

	
	pWorkView->m_sRelationPK = m_pRelationSearchDM->GetCellData(row-1, 0);
	pWorkView->ShowRelationData();

    return 0;
}

BEGIN_EVENTSINK_MAP(CMarcManagerView, CFormView)
    //{{AFX_EVENTSINK_MAP(CMarcManagerView)
	ON_EVENT(CMarcManagerView, IDC_gTagSearch, -601 /* DblClick */, OnDblClickgTagSearch, VTS_NONE)
	ON_EVENT(CMarcManagerView, IDC_gTagSearch, -605 /* MouseDown */, OnMouseDowngTagSearch, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


VOID CMarcManagerView::OnDblClickgTagSearch() 
{
	// TODO: Add your control notification handler code here
    if(m_tab.GetCurSel() == 0)
	{
      ShowTagDataOnWorkView();
	}
	else
	{
	  ShowRelationDataOnWorkView();
	}

}

VOID CMarcManagerView::OnMouseDowngTagSearch(short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	INT row = m_pTagSearchGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
      m_pTagSearchGrid->SetReverse(row-1);
	  m_nCurrentGridRow = row;
	}
     
    OnDblClickgTagSearch();

}

VOID CMarcManagerView::OnSize(UINT nType, INT cx, INT cy) 
{
	CFormView::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
		RECT rect, r;
	GetClientRect(&rect);
	CWnd *pWnd = NULL;
//////////////////////////////////////////////////////	
///////Tag front page onsize
	pWnd = GetDlgItem(IDC_tTagSearch);
    r = rect;
	if (pWnd) pWnd->MoveWindow(&r, true);
	
	pWnd = GetDlgItem(IDC_gTagSearch);
	r = rect;
	r.left += 5;
	r.right -= 5;
	r.top = r.top + 120;
	r.bottom = r.bottom - 5;
	if (pWnd) pWnd->MoveWindow(&r, true);


}

VOID CMarcManagerView::OnSelchangetTagSearch(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	INT index = m_tab.GetCurSel();
	
	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	CMarcManagerWorkView *pWorkView = (CMarcManagerWorkView *)pframe->GetSecondView();
    
	pWorkView->ClearAllTagGrid();
	pWorkView->ClearAllRelationGrid();
    
    m_sTagCode = _T("");
	m_sTagName = _T("");

	if(0 == index)
	{
	    GetDlgItem(IDC_STATIC_VIEW1)->SetWindowText(_T("코드"));
        GetDlgItem(IDC_STATIC_VIEW2)->SetWindowText(_T("이름"));
		
		m_pTagSearchGrid->RemoveAll();
		InitTagGrid();
		pWorkView->TagTabSelected();
	}
	else
	{
		GetDlgItem(IDC_STATIC_VIEW1)->SetWindowText(_T("기호"));
        GetDlgItem(IDC_STATIC_VIEW2)->SetWindowText(_T("인자"));
		
		m_pTagSearchGrid->RemoveAll();
		InitRelationGrid();
		pWorkView->RelationTabSelected();
	}
    
	UpdateData(FALSE);

	*pResult = 0;
}

VOID CMarcManagerView::OnGroupSelect() 
{
	// TODO: Add your command handler code here

	CMarcManagerGroupSelect dlg(this);
	if(dlg.DoModal() != IDOK) return;
	
	if(m_sMarcGroup == dlg.m_sGroupCode) return;

	m_sMarcGroup = dlg.m_sGroupCode;
    m_sGroupCodeOnView = m_sMarcGroup;
    

	//초기화 작업
	m_pTagSearchDM->FreeData();
	m_pTagSearchGrid->RemoveAll();
	m_pTagSearchGrid->SetRows(2);
	InitTagGrid();
	
	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	CMarcManagerWorkView *pWorkView = (CMarcManagerWorkView *)pframe->GetSecondView();
    
	pWorkView->ClearAllTagGrid();
	pWorkView->ClearAllRelationGrid();
    pWorkView->m_sTagAlias = _T("");


	UpdateData(FALSE);
	

	/*
	CMarcManagerGroupSelect dlg(this);
	if(dlg.DoModal() == IDOK) m_sMarcGroup = dlg.m_sGroupCode;
    m_sGroupCodeOnView = m_sMarcGroup;
    UpdateData(FALSE);
	*/

}

VOID CMarcManagerView::OnTagInsert() 
{
	// TODO: Add your command handler code here
    CMarcManagerTagInfo dlg(this);
    
	dlg.m_sTagGroupCode = m_sMarcGroup;
    //default option
	dlg.m_nRadioRepeat = 0;
	dlg.m_nRadioEssential = 3;
	
	UpdateData(false);
    if(dlg.DoModal() != IDOK) return;
    UpdateData(true);
    
    CString tmpalias,tmpdescrip,tmprepeat,tmpessential,tmpexample,tmptime;
    CTime time = m_pTagSearchDM->GetDBTime();
    tmptime.Format(_T("%d/%d/%d"),time.GetYear(),time.GetMonth(),time.GetDay()); //DB TIME SETTING 
    
	if(dlg.m_nRadioRepeat == 0)
	{
        tmprepeat = _T("'Y'");
	}
	else
	{
        tmprepeat = _T("'N'");
	}

    if(dlg.m_nRadioEssential == 0)
	{
        tmpessential = _T("'M'");
	}
    else if(dlg.m_nRadioEssential == 1)
	{
        tmpessential = _T("'A'");
	}
	else if(dlg.m_nRadioEssential == 2)
	{
        tmpessential = _T("'O'");
	}
	else
	{
        tmpessential = _T("'B'");
	}
		
	//tmpdescrip = IsNullData(dlg.m_sTagDescrip);
	tmpdescrip = dlg.m_sTagDescrip;
	tmpexample = IsNullData(dlg.m_sTagExample);
	tmptime = IsNullData(tmptime);
	tmpalias.Format(_T("'%s_%s'"),dlg.m_sTagGroupCode,dlg.m_sTagCode);

	//insert query
	CString query,tmpquery;
    m_pTagSearchDM->StartFrame();

	CString strPK;
	m_pTagSearchDM->MakeRecKey(strPK);
	

	query.Format(_T("INSERT INTO MARC_TAG_TBL (PK,ALIAS,MARC_GROUP_CODE,TAG_CODE,TAG_NAME,REPEAT_TYPE,ESSENTIAL_TYPE,EXAMPLE,CREATE_DATE)")
	
	_T(" VALUES (%s, %s,'%s', '%s' , '%s', %s, %s, %s, %s );"),strPK,tmpalias,dlg.m_sTagGroupCode, dlg.m_sTagCode, dlg.m_sTagName ,tmprepeat,tmpessential,tmpexample,tmptime);
   
	m_pTagSearchDM->AddFrame(query);
    INT ids = m_pTagSearchDM->SendFrame();
    m_pTagSearchDM->EndFrame();
	
	if(ids < 0) {
		AfxMessageBox(_T("입력과정에서 에러가 발생하였습니다!")); 
		return;
	}
	
	ids = UpdateTagDescripData(m_pTagSearchDM, strPK, tmpdescrip);
	if(ids < 0) AfxMessageBox(_T("입력과정에서 에러가 발생하였습니다!")); 

	if(m_tab.GetCurSel() == 0) ShowTagSearchData(); //refresh

}

INT CMarcManagerView::UpdateTagDescripData(CESL_DataMgr *pDM, CString strPK, CString strDescrip)
{
	if (!pDM) return-1;
	pDM->StartFrame();
	
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("DESCRIP"), _T("STRING"), strDescrip, -1);
	pDM->ORACLE_OCIMgr_MakeUpdateFrame(_T("MARC_TAG_TBL"), _T("PK"), _T("NUMERIC"), strPK);
   
	INT ids = pDM->SendFrame();
	pDM->EndFrame();
	return ids;
}


BOOL CMarcManagerView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DELETE)
	{
		if(m_tab.GetCurSel() == 0)
		{
		    if(m_pTagSearchDM->GetRowCount()>0)
			{
               DeleteTagWithRow(m_nCurrentGridRow);                  
			}
		}
        else
		{
            if(m_pRelationSearchDM->GetRowCount()>0)
			{
               DeleteRelationWithRow(m_nCurrentGridRow);                  
			}

		}
	}
   
	return CFormView::PreTranslateMessage(pMsg);
}

VOID CMarcManagerView::OnGroupManage() 
{
	// TODO: Add your command handler code here
	CMarcManagerGroupManager dlg;
	dlg.DoModal();
    
}

VOID CMarcManagerView::OnRelationInsert() 
{
	// TODO: Add your command handler code here
	CMarcManagerRelation dlg(this);
    if(m_sMarcGroup == _T("")) m_sMarcGroup = m_pMainFrame->m_sMarcGroup;
    
	dlg.m_sRelationGroupCode = m_sMarcGroup;
   	
	UpdateData(false);
    if(dlg.DoModal() != IDOK) return;
    UpdateData(true);
    
    CString tmpparam,tmpdescrip,tmptime;
    CTime time = m_pTagSearchDM->GetDBTime();
    tmptime.Format(_T("%d/%d/%d"),time.GetYear(),time.GetMonth(),time.GetDay()); //DB TIME SETTING 
    
	tmpdescrip = IsNullData(dlg.m_sRelationDescrip);
   	tmpparam = IsNullData(dlg.m_sRelationParam);
	tmptime = IsNullData(tmptime);
	
	//insert query
	CString query,tmpquery;
    m_pRelationSearchDM->StartFrame();

	query.Format(_T("INSERT INTO MARC_RELATION_TBL (PK,MARC_GROUP_CODE,OPERATOR,BASIC_PARAM,PARAM,DESCRIP,CREATE_DATE)")
	_T(" VALUES (%s.NEXTVAL, '%s','%s', '%s' , %s, %s, %s);"),ReadCfgFile.m_sSequence,dlg.m_sRelationGroupCode, dlg.m_sRelationOperator, dlg.m_sRelationBasicParam ,tmpparam,tmpdescrip,tmptime);
   

	m_pRelationSearchDM->AddFrame(query);
    INT ids = m_pRelationSearchDM->SendFrame();
    if(ids < 0) AfxMessageBox(_T("입력과정에서 에러가 발생하였습니다!")); 
    m_pRelationSearchDM->EndFrame();
    
    if(m_tab.GetCurSel() == 1) RelationSearch(); //refresh

}
