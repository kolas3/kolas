// MarcManagerWorkView.cpp : implementation file
//

#include "stdafx.h"
#include "MarcManager.h"
#include "MarcManagerWorkView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerWorkView

IMPLEMENT_DYNCREATE(CMarcManagerWorkView, CFormView)

CMarcManagerWorkView::CMarcManagerWorkView()
	: CFormView(CMarcManagerWorkView::IDD)
{
   
	//{{AFX_DATA_INIT(CMarcManagerWorkView)
	m_sTagDescrip = _T("");
	m_sTagExample = _T("");
	m_sRelationDescrip = _T("");
	//}}AFX_DATA_INIT
    
	m_pRelationDM  = new CESL_DataMgr;
	m_pTagInfoDM = new CESL_DataMgr;
    m_pTagFirstIndicatorDM = new CESL_DataMgr;
    m_pTagSecondIndicatorDM = new CESL_DataMgr;
    m_pTagSubfieldDM = new CESL_DataMgr;
    
    m_pRelationGrid =NULL;
    m_pTagInfoGrid = NULL;
    m_pTagIndicatorGrid = NULL;
    m_pTagSubfieldGrid = NULL;
    
	//그외 init
	m_nFirstToSecond = 0;
    m_nTagIndicatorCurRow = 0;
    m_bIsTagUsing = true;

	// KOL.UDF.022
	m_sTagAlias = _T("");
	m_sMarcGroup = _T("");
	m_sRelationPK = _T("");
    
	m_nWhichTagGrid = 0;
    m_nTagSubfieldCurRow = 0;
}

CMarcManagerWorkView::~CMarcManagerWorkView()
{
    if (m_pTagInfoDM) {
		delete m_pTagInfoDM;
		m_pTagInfoDM = NULL;
	}

    if (m_pTagFirstIndicatorDM) {
		delete m_pTagFirstIndicatorDM;
		m_pTagFirstIndicatorDM = NULL;
	}
    
	if (m_pTagSecondIndicatorDM) {
		delete m_pTagSecondIndicatorDM;
		m_pTagSecondIndicatorDM = NULL;
	}
    
    if (m_pTagSubfieldDM) {
		delete m_pTagSubfieldDM;
		m_pTagSubfieldDM = NULL;
	}
    
    if(m_pRelationDM) {
		delete m_pRelationDM;
		m_pRelationDM = NULL;
	}
	
	if (m_pTagInfoGrid) {
		delete m_pTagInfoGrid;
		m_pTagInfoGrid = NULL;
	}

    if (m_pTagIndicatorGrid) {
		delete m_pTagIndicatorGrid;
		m_pTagIndicatorGrid = NULL;
	}

	if (m_pTagSubfieldGrid) {
		delete m_pTagSubfieldGrid;
		m_pTagSubfieldGrid = NULL;
	}
    
	if(m_pRelationGrid) {
		delete m_pRelationGrid;
		m_pRelationGrid = NULL;
	}

}

VOID CMarcManagerWorkView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcManagerWorkView)
	DDX_Control(pDX, IDC_eTagExample, m_cTagExample);
	DDX_Control(pDX, IDC_eTagDescrip, m_cTagDescrip);
	DDX_Control(pDX, IDC_tTagInfo, m_tab);
	DDX_Text(pDX, IDC_eTagDescrip, m_sTagDescrip);
	DDX_Text(pDX, IDC_eTagExample, m_sTagExample);
	DDX_Text(pDX, IDC_eRelationDescrip, m_sRelationDescrip);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarcManagerWorkView, CFormView)
	//{{AFX_MSG_MAP(CMarcManagerWorkView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tTagInfo, OnSelchangetTagInfo)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bTagIndicator, OnbTagIndicator)
	ON_BN_CLICKED(IDC_bTagSubfield, OnbTagSubfield)
	ON_COMMAND(ID_GROUP_MANAGE, OnGroupManage)
	ON_COMMAND(ID_GROUP_SELECT, OnGroupSelect)
	ON_COMMAND(ID_RELATION_INSERT, OnRelationInsert)
	ON_COMMAND(ID_TAG_INSERT, OnTagInsert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerWorkView diagnostics

#ifdef _DEBUG
VOID CMarcManagerWorkView::AssertValid() const
{
	CFormView::AssertValid();
}

VOID CMarcManagerWorkView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerWorkView message handlers


INT CMarcManagerWorkView::ShowTagData()
{
    ShowTagInfoData();
    ShowTagIndicatorData();
	ShowTagSubfieldData();
	return 0;
}

INT CMarcManagerWorkView::ShowRelationData()
{
/////////////////////////////////////////////////////////////////
    ////////Relation data query
	CString query,tmpquery;
	
	query.Format(_T("SELECT PK,MARC_GROUP_CODE, OPERATOR, BASIC_PARAM, PARAM, DESCRIP, CREATE_DATE, MODIFY_DATE FROM MARC_RELATION_TBL"));
 	       
	tmpquery.Format(_T(" WHERE PK = %s"),m_sRelationPK);
  	
    query += tmpquery;

	m_pRelationDM->RestructDataManager(query);
    //////////////////////////////////////////////////////////////////

	INT nRow = m_pRelationDM->GetRowCount();
	INT gridrow;
	if (nRow == 0) gridrow = 2;
		else gridrow = nRow+1;
	m_pRelationGrid->RemoveAll();
	m_pRelationGrid->SetRows(gridrow);
	InitRelationGrid();
    
	CString tmpessential;

	for (INT i = 0; i < nRow; i ++) {
		m_pRelationGrid->SetTextMatrix(i+1, 0, m_pRelationDM->GetCellData(i, 1));
		m_pRelationGrid->SetTextMatrix(i+1, 1, m_pRelationDM->GetCellData(i, 2));
	    m_pRelationGrid->SetTextMatrix(i+1, 2, m_pRelationDM->GetCellData(i, 3));
	    m_pRelationGrid->SetTextMatrix(i+1, 3, m_pRelationDM->GetCellData(i, 4));
	    m_pRelationGrid->SetTextMatrix(i+1, 4, m_pRelationDM->GetCellData(i, 6));
	    m_pRelationGrid->SetTextMatrix(i+1, 5, m_pRelationDM->GetCellData(i, 7));
	}
 
	m_sRelationDescrip = m_pRelationDM->GetCellData(0, 5);

	UpdateData(FALSE);

	return 0;
}
INT CMarcManagerWorkView::ShowTagInfoData()
{
    /////////////////////////////////////////////////////////////////
    ////////TagInfoData query
	CString query,tmpquery;
	
	query.Format(_T("SELECT PK,MARC_GROUP_CODE, TAG_CODE, TAG_NAME ,REPEAT_TYPE,ESSENTIAL_TYPE,DESCRIP,CREATE_DATE,MODIFY_DATE,EXAMPLE FROM MARC_TAG_TBL"));
 	       
	tmpquery.Format(_T(" WHERE ALIAS = '%s'"),m_sTagAlias);
  	
    query += tmpquery;

	m_pTagInfoDM->RestructDataManager(query);
    //////////////////////////////////////////////////////////////////

	INT nRow = m_pTagInfoDM->GetRowCount();
	INT gridrow;
	if (nRow == 0) gridrow = 2;
		else gridrow = nRow+1;
	m_pTagInfoGrid->RemoveAll();
	m_pTagInfoGrid->SetRows(gridrow);
	InitTagInfoGrid();
    
	CString tmpessential;

	for (INT i = 0; i < nRow; i ++) {
		m_pTagInfoGrid->SetTextMatrix(i+1, 0, m_pTagInfoDM->GetCellData(i, 1));
		m_pTagInfoGrid->SetTextMatrix(i+1, 1, m_pTagInfoDM->GetCellData(i, 2));
	    m_pTagInfoGrid->SetTextMatrix(i+1, 2, m_pTagInfoDM->GetCellData(i, 3));
	    m_pTagInfoGrid->SetTextMatrix(i+1, 3, m_pTagInfoDM->GetCellData(i, 4));
		
		if(m_pTagInfoDM->GetCellData(i, 5) == _T("M"))
		{
            tmpessential = _T("필수");
		}
		else if(m_pTagInfoDM->GetCellData(i, 5) == _T("A"))
		{
            tmpessential = _T("해당시필수");
		}
		else if(m_pTagInfoDM->GetCellData(i, 5) == _T("O"))
		{
            tmpessential = _T("재량");
		}
		else if(m_pTagInfoDM->GetCellData(i, 5) == _T("B"))
		{
            tmpessential = _T("공백");
		}
		
		m_pTagInfoGrid->SetTextMatrix(i+1, 4, tmpessential);
		m_pTagInfoGrid->SetTextMatrix(i+1, 5, m_pTagInfoDM->GetCellData(i, 7));
	    m_pTagInfoGrid->SetTextMatrix(i+1, 6, m_pTagInfoDM->GetCellData(i, 8));
	}
 
	if(nRow != 0){
		m_sTagDescrip = m_pTagInfoDM->GetCellData(0, 6);
	    m_sTagExample = m_pTagInfoDM->GetCellData(0, 9);
	
	}

	UpdateData(FALSE);

	return 0;
}


INT CMarcManagerWorkView::ShowTagIndicatorData()
{
    /////////////////////////////////////////////////////////////////
    ////////TagFirstIndicator query
	CString query,tmpquery;
	
	query.Format(_T("SELECT PK, IND_CODE, DESCRIP, CREATE_DATE, MODIFY_DATE,DEFAULT_YN FROM MARC_FIRST_INDICATOR_TBL"));
 	       
	tmpquery.Format(_T(" WHERE MARC_TAG_ALIAS = '%s'"),m_sTagAlias);
  	
    query += tmpquery;

	m_pTagFirstIndicatorDM->RestructDataManager(query);
    //////////////////////////////////////////////////////////////////
    ////////TagSecondIndicator query
	query.Format(_T("SELECT PK, IND_CODE, DESCRIP, CREATE_DATE, MODIFY_DATE,DEFAULT_YN FROM MARC_SECOND_INDICATOR_TBL"));
 	       
	tmpquery.Format(_T(" WHERE MARC_TAG_ALIAS = '%s'"),m_sTagAlias);
  	
    query += tmpquery;

	m_pTagSecondIndicatorDM->RestructDataManager(query);
    //////////////////////////////////////////////////////////////////
 
	INT nRow = m_pTagFirstIndicatorDM->GetRowCount() + m_pTagSecondIndicatorDM->GetRowCount();
	INT gridrow;
	if (nRow == 0) gridrow = 2;
		else gridrow = nRow+1;
	m_pTagIndicatorGrid->RemoveAll();
	m_pTagIndicatorGrid->SetRows(gridrow);
	InitTagIndicatorGrid();
    
	for (INT i = 0; i < m_pTagFirstIndicatorDM->GetRowCount(); i ++) {
		m_pTagIndicatorGrid->SetTextMatrix(1, 0, _T("제1지시기호"));
		m_pTagIndicatorGrid->SetTextMatrix(i+1, 1, m_pTagFirstIndicatorDM->GetCellData(i, 1));
	    m_pTagIndicatorGrid->SetTextMatrix(i+1, 2, m_pTagFirstIndicatorDM->GetCellData(i, 2));
	    m_pTagIndicatorGrid->SetTextMatrix(i+1, 3, m_pTagFirstIndicatorDM->GetCellData(i, 3));
		m_pTagIndicatorGrid->SetTextMatrix(i+1, 4, m_pTagFirstIndicatorDM->GetCellData(i, 4));
		m_pTagIndicatorGrid->SetTextMatrix(i+1, 5, m_pTagFirstIndicatorDM->GetCellData(i, 5));
	}
    
	INT k = 0;
    
	m_nFirstToSecond = i;

    for (INT j = i ; j < nRow ; j ++) {
		m_pTagIndicatorGrid->SetTextMatrix(i+1, 0, _T("제2지시기호"));
		m_pTagIndicatorGrid->SetTextMatrix(j+1, 1, m_pTagSecondIndicatorDM->GetCellData(k, 1));
	    m_pTagIndicatorGrid->SetTextMatrix(j+1, 2, m_pTagSecondIndicatorDM->GetCellData(k, 2));
	    m_pTagIndicatorGrid->SetTextMatrix(j+1, 3, m_pTagSecondIndicatorDM->GetCellData(k, 3));
		m_pTagIndicatorGrid->SetTextMatrix(j+1, 4, m_pTagSecondIndicatorDM->GetCellData(k, 4));
	    m_pTagIndicatorGrid->SetTextMatrix(j+1, 5, m_pTagSecondIndicatorDM->GetCellData(k, 5));
		k++;
	}

	UpdateData(FALSE);

	return 0;
}

INT CMarcManagerWorkView::ShowTagSubfieldData()
{
    /////////////////////////////////////////////////////////////////
    ////////TagInfoData query
	CString query,tmpquery;
	
	query.Format(_T("SELECT PK,SUBFIELD_CODE,REPEAT_TYPE,ESSENTIAL_TYPE,PUNCTUATION_MARK_SET,DESCRIP,CREATE_DATE,MODIFY_DATE FROM MARC_SUBFIELD_TBL"));
 	       
	tmpquery.Format(_T(" WHERE MARC_TAG_ALIAS = '%s'"),m_sTagAlias);
  	
    query += tmpquery;

	m_pTagSubfieldDM->RestructDataManager(query);
    //////////////////////////////////////////////////////////////////

	INT nRow = m_pTagSubfieldDM->GetRowCount();
	INT gridrow;
	if (nRow == 0) gridrow = 2;
		else gridrow = nRow+1;
	m_pTagSubfieldGrid->RemoveAll();
	m_pTagSubfieldGrid->SetRows(gridrow);
	InitTagSubfieldGrid();
    
	CString tmpessential;
    
	for (INT i = 0; i < nRow; i ++) {
		m_pTagSubfieldGrid->SetTextMatrix(i+1, 0, m_pTagSubfieldDM->GetCellData(i, 1));
		m_pTagSubfieldGrid->SetTextMatrix(i+1, 1, m_pTagSubfieldDM->GetCellData(i, 2));
	    
		if(m_pTagSubfieldDM->GetCellData(i, 3) == _T("M"))
		{
            tmpessential = _T("필수");
		}
		else if(m_pTagSubfieldDM->GetCellData(i, 3) == _T("A"))
		{
            tmpessential = _T("해당시필수");
		}
		else if(m_pTagSubfieldDM->GetCellData(i, 3) == _T("O"))
		{
            tmpessential = _T("재량");
		}
		else if(m_pTagSubfieldDM->GetCellData(i, 3) == _T("B"))
		{
			tmpessential = _T("공백");
        }
		m_pTagSubfieldGrid->SetTextMatrix(i+1, 2, tmpessential);
	    m_pTagSubfieldGrid->SetTextMatrix(i+1, 3, m_pTagSubfieldDM->GetCellData(i, 4));
		m_pTagSubfieldGrid->SetTextMatrix(i+1, 4, m_pTagSubfieldDM->GetCellData(i, 5));
		m_pTagSubfieldGrid->SetTextMatrix(i+1, 5, m_pTagSubfieldDM->GetCellData(i, 6));
	    m_pTagSubfieldGrid->SetTextMatrix(i+1, 6, m_pTagSubfieldDM->GetCellData(i, 7));
	}
	return 0;
    
	UpdateData(FALSE);
}

VOID CMarcManagerWorkView::InitTagInfoGrid()
{
    m_pTagInfoGrid->SetCols(0,7);
    m_pTagInfoGrid->SetColumnInfo(0-1, _T("그룹코드"),		90,				4);
	m_pTagInfoGrid->SetColumnInfo(1-1, _T("코드"),	        60,	  			4);
	m_pTagInfoGrid->SetColumnInfo(2-1, _T("이름"),	        200,			4);
	m_pTagInfoGrid->SetColumnInfo(3-1, _T("반복여부"),		60,				4);
	m_pTagInfoGrid->SetColumnInfo(4-1, _T("필수여부"),		90,				4);
	m_pTagInfoGrid->SetColumnInfo(5-1, _T("생성일 "),	    70,				4);
    m_pTagInfoGrid->SetColumnInfo(6-1, _T("수정일 "),		70,				4);
}

VOID CMarcManagerWorkView::InitTagIndicatorGrid()
{
    m_pTagIndicatorGrid->SetCols(0,6);
    m_pTagIndicatorGrid->SetColumnInfo(0-1, _T("지시기호"), 	80,				4);
	m_pTagIndicatorGrid->SetColumnInfo(1-1, _T("코드"),		    50,				4);
	m_pTagIndicatorGrid->SetColumnInfo(2-1, _T("설명"),         200,			4);
	m_pTagIndicatorGrid->SetColumnInfo(3-1, _T("생성일"),		70,				4);
    m_pTagIndicatorGrid->SetColumnInfo(4-1, _T("수정일"),		70,				4);
	m_pTagIndicatorGrid->SetColumnInfo(5-1, _T("기본값"),		50,				4);
}

VOID CMarcManagerWorkView::InitTagSubfieldGrid()
{
    m_pTagSubfieldGrid->SetCols(0,7);
    m_pTagSubfieldGrid->SetColumnInfo(0-1, _T("식별기호"),		80,				4);
	m_pTagSubfieldGrid->SetColumnInfo(1-1, _T("반복여부"),	    60,	  			4);
	m_pTagSubfieldGrid->SetColumnInfo(2-1, _T("필수여부"),	    70,				4);
	m_pTagSubfieldGrid->SetColumnInfo(3-1, _T("구두점셋트"),	70,				4);
	m_pTagSubfieldGrid->SetColumnInfo(4-1, _T("설명"),	        230,			4);
	m_pTagSubfieldGrid->SetColumnInfo(5-1, _T("생성일 "),	    70,				4);
    m_pTagSubfieldGrid->SetColumnInfo(6-1, _T("수정일 "),		70,				4);
}

VOID CMarcManagerWorkView::InitRelationGrid()
{
    m_pRelationGrid->SetCols(0,6);
    m_pRelationGrid->SetColumnInfo(0-1, _T("그룹코드"),		90,				4);
	m_pRelationGrid->SetColumnInfo(1-1, _T("연산기호"),	    60,	  			4);
	m_pRelationGrid->SetColumnInfo(2-1, _T("기준인자"),	    90,				4);
	m_pRelationGrid->SetColumnInfo(3-1, _T("연산인자"), 	90,				4);
	m_pRelationGrid->SetColumnInfo(4-1, _T("생성일 "),	    70,				4);
    m_pRelationGrid->SetColumnInfo(5-1, _T("수정일 "),		70,				4);
}

INT CMarcManagerWorkView::TagTabSelected()
{
    m_bIsTagUsing = true;
	
	
	//tag관련 front page item
    m_pTagInfoGrid->ShowWindow(true);
    m_pTagIndicatorGrid->ShowWindow(true);
    m_tab.ShowWindow(true);
    GetDlgItem(IDC_eTagDescrip)->ShowWindow(true);
    GetDlgItem(IDC_bTagIndicator)->ShowWindow(true);
    GetDlgItem(IDC_bTagSubfield)->ShowWindow(true);
    
	//relation관련 item 
	GetDlgItem(IDC_gRelation)->ShowWindow(false);
    GetDlgItem(IDC_eRelationDescrip)->ShowWindow(false);
	
    return 0;
}

INT CMarcManagerWorkView::RelationTabSelected()
{
    m_bIsTagUsing = false;
	
	//relation관련 item 
	GetDlgItem(IDC_gRelation)->ShowWindow(true);
    GetDlgItem(IDC_eRelationDescrip)->ShowWindow(true);

	//tag관련 all item
    m_pTagInfoGrid->ShowWindow(false);
    m_pTagIndicatorGrid->ShowWindow(false);
    m_pTagSubfieldGrid->ShowWindow(false);
    m_tab.ShowWindow(false);
    GetDlgItem(IDC_eTagDescrip)->ShowWindow(false);
    GetDlgItem(IDC_eTagExample)->ShowWindow(false);
    GetDlgItem(IDC_Static_Example)->ShowWindow(false);
    GetDlgItem(IDC_bTagIndicator)->ShowWindow(false);
    GetDlgItem(IDC_bTagSubfield)->ShowWindow(false);

    return 0;
}

CString CMarcManagerWorkView::IsNullData(CString data)
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

INT CMarcManagerWorkView::DeleteIndicatorWithRow(INT currow)
{
    //properly make delete query
	CString tmpPK,tmpCode;
    INT mode;
    
	if(currow > m_nFirstToSecond) 
	{
		tmpPK = m_pTagSecondIndicatorDM->GetCellData(currow-m_pTagFirstIndicatorDM->GetRowCount()-1 ,0);
	    tmpCode = m_pTagSecondIndicatorDM->GetCellData(currow-m_pTagFirstIndicatorDM->GetRowCount()-1 ,1);
		mode = 1;
	}
    else
	{
	    tmpPK = m_pTagFirstIndicatorDM->GetCellData(currow-1,0);
	    tmpCode = m_pTagFirstIndicatorDM->GetCellData(currow-1,1);
	    mode = 0;
	}
	
	CString msg;
	msg.Format(_T("제%d지시기호에서  %s 코드를 삭제하시겠습니까?"),mode+1,tmpCode);
    if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;
	

	//delete query
	CString query,tmpquery;
    m_pTagFirstIndicatorDM->StartFrame();

	if(!mode) {
		query.Format(_T("DELETE FROM MARC_FIRST_INDICATOR_TBL"));
    }
	else {
       query.Format(_T("DELETE FROM MARC_SECOND_INDICATOR_TBL"));
	}  
    
	tmpquery.Format(_T(" WHERE PK = %s;"),tmpPK);
    
	query += tmpquery;

	m_pTagFirstIndicatorDM->AddFrame(query);
    INT ids = m_pTagFirstIndicatorDM->SendFrame();
    m_pTagFirstIndicatorDM->EndFrame();

	if(ids<0) AfxMessageBox(_T("삭제하던 중 에러가 발생하였습니다"));
	ShowTagIndicatorData(); //refresh

	return 0;
}


INT CMarcManagerWorkView::ClearAllTagGrid()
{
	m_pTagInfoGrid->RemoveAll(); 
    m_pTagIndicatorGrid->RemoveAll(); 
    m_pTagSubfieldGrid->RemoveAll(); 
    
    m_sTagDescrip = _T("");
	m_sTagExample = _T("");

    UpdateData(false);

	return 0;
}

INT CMarcManagerWorkView::ClearAllRelationGrid()
{
    m_pRelationGrid->RemoveAll(); 
    m_sRelationDescrip = _T("");

    UpdateData(false);

	return 0;
}


INT CMarcManagerWorkView::DeleteSubfieldWithRow(INT currow)
{
    //properly make delete query
	CString tmpPK,tmpCode;
   
	tmpPK = m_pTagSubfieldDM->GetCellData(currow-1,0);
	tmpCode = m_pTagSubfieldDM->GetCellData(currow-1,1);
		
	CString msg;
	msg.Format(_T("식별기호에서  %s 코드를 삭제하시겠습니까?"),tmpCode);
    if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;

	//delete query
	CString query;
    m_pTagSubfieldDM->StartFrame();

	query.Format(_T("DELETE FROM MARC_SUBFIELD_TBL WHERE PK = %s;"),tmpPK);

	m_pTagSubfieldDM->AddFrame(query);
    INT ids = m_pTagSubfieldDM->SendFrame();
    m_pTagSubfieldDM->EndFrame();

	if(ids<0) AfxMessageBox(_T("삭제하던 중 에러가 발생하였습니다"));
	ShowTagSubfieldData(); //refresh

	return 0;
}



VOID CMarcManagerWorkView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	ReadCfgFile.ReadCfgFile(_T(""));
	m_pTagInfoDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
    m_pTagFirstIndicatorDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	m_pTagSecondIndicatorDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
    m_pTagSubfieldDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
    m_pRelationDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	// Grid 초기화
	m_pTagInfoGrid = new CESL_Grid(NULL);
	m_pTagInfoGrid->SubclassDlgItem(IDC_gTagInfo, this);
   	m_pTagIndicatorGrid = new CESL_Grid(NULL);
	m_pTagIndicatorGrid->SubclassDlgItem(IDC_gTagIndicator, this);
	m_pTagSubfieldGrid = new CESL_Grid(NULL);
    m_pTagSubfieldGrid->SubclassDlgItem(IDC_gTagSubfield, this);
    m_pRelationGrid = new CESL_Grid(NULL);
    m_pRelationGrid->SubclassDlgItem(IDC_gRelation, this);


	//tab선택에 따른 객체 보여주기 초기화 
	///////////////////tag next page
	m_pTagSubfieldGrid->ShowWindow(false);
	GetDlgItem(IDC_eTagExample)->ShowWindow(false);
    GetDlgItem(IDC_Static_Example)->ShowWindow(false);
    ///////////////////relation page
	GetDlgItem(IDC_gRelation)->ShowWindow(false);
    GetDlgItem(IDC_eRelationDescrip)->ShowWindow(false);
	
	//tab 초기화 
    m_tab.InsertItem(0, _T("태그정보와 지시기호"));
    m_tab.InsertItem(1, _T("식별기호와 예시"));

	//임시 마크그룹은 연속으로 한다 
    m_sMarcGroup = _T("serials");

}

VOID CMarcManagerWorkView::OnSelchangetTagInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	INT index = m_tab.GetCurSel();
	
	if(0==index)
	{
   		m_pTagInfoGrid->ShowWindow(true);
		m_pTagIndicatorGrid->ShowWindow(true);
	    GetDlgItem(IDC_eTagDescrip)->ShowWindow(true);
        GetDlgItem(IDC_eTagExample)->ShowWindow(false);
		m_pTagSubfieldGrid->ShowWindow(false);
	    GetDlgItem(IDC_Static_Example)->ShowWindow(false);
	    
	}
    else
	{
   		m_pTagSubfieldGrid->ShowWindow(true);
		GetDlgItem(IDC_eTagExample)->ShowWindow(true);
		GetDlgItem(IDC_Static_Example)->ShowWindow(true);
        m_pTagInfoGrid->ShowWindow(false);
		m_pTagIndicatorGrid->ShowWindow(false);
		GetDlgItem(IDC_eTagDescrip)->ShowWindow(false);
		
	}
    
	UpdateData(false);

	*pResult = 0;
}

VOID CMarcManagerWorkView::OnSize(UINT nType, INT cx, INT cy) 
{
	CFormView::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	RECT rect, r;
	GetClientRect(&rect);
	CWnd *pWnd = NULL;
//////////////////////////////////////////////////////	
///////Tag front page onsize
	pWnd = GetDlgItem(IDC_tTagInfo);
    r = rect;
	if (pWnd) pWnd->MoveWindow(&r, true);
	
	pWnd = GetDlgItem(IDC_gTagInfo);
	r = rect;
	r.left += 5;
	r.right -= 5;
	r.top = r.top + 20;
	r.bottom = r.top + 60;
	if (pWnd) pWnd->MoveWindow(&r, true);
	
	pWnd = GetDlgItem(IDC_eTagDescrip);
	r = rect;
	r.left += 5;
	r.right -= 5;
	r.top = r.top + 90;
	r.bottom = r.bottom - 310;
	if (pWnd) pWnd->MoveWindow(&r, true);
	
	pWnd = GetDlgItem(IDC_gTagIndicator);
	r = rect;
	r.left += 5;
	r.right -= 5;
	r.top = r.bottom - 300;
	r.bottom = r.bottom - 100;
	if (pWnd) pWnd->MoveWindow(&r, true);

/////////////////////////////////////////////////
//////tag back page onsize
	pWnd = GetDlgItem(IDC_gTagSubfield);
	r = rect;
	r.left += 5;
	r.right -= 5;
	r.top = r.top + 20;
	r.bottom = r.bottom -150;
	if (pWnd) pWnd->MoveWindow(&r, true);

	pWnd = GetDlgItem(IDC_eTagExample);
	r = rect;
	r.left += 5;
	r.right -= 5;
	r.top = r.bottom - 120;
	r.bottom = r.bottom - 80;
	if (pWnd) pWnd->MoveWindow(&r, true);

    pWnd = GetDlgItem(IDC_Static_Example);
	r = rect;
	r.left += 5;
	r.right -= 5;
	r.top = r.bottom - 140;
	r.bottom = r.bottom - 125;
	if (pWnd) pWnd->MoveWindow(&r, true);
///////////////////////////////////////////////////
/////relation page onsize
	pWnd = GetDlgItem(IDC_gRelation);
	r = rect;
	r.left += 5;
	r.right -= 5;
	r.top = r.top;
	r.bottom = r.top + 60;
	if (pWnd) pWnd->MoveWindow(&r, true);
    	
	pWnd = GetDlgItem(IDC_eRelationDescrip);
	r = rect;
	r.left += 5;
	r.right -= 5;
	r.top = r.top + 90;
	r.bottom = r.top + 150;
	if (pWnd) pWnd->MoveWindow(&r, true);
///////////////////////////////////////////////////
///tag button onsize
	pWnd = GetDlgItem(IDC_bTagIndicator);
	r = rect;
	r.left += 5;
	r.right = r.left + 75;
	r.top = r.bottom - 75;
	r.bottom = r.top + 70;
	if (pWnd) pWnd->MoveWindow(&r, true);

	pWnd = GetDlgItem(IDC_bTagSubfield);
	r.left = r.right + 10;
	r.right = r.left + 75;
	if (pWnd) pWnd->MoveWindow(&r, true);
///////////////////////////////////////////////

}

BEGIN_EVENTSINK_MAP(CMarcManagerWorkView, CFormView)
    //{{AFX_EVENTSINK_MAP(CMarcManagerWorkView)
	ON_EVENT(CMarcManagerWorkView, IDC_gTagIndicator, -601 /* DblClick */, OnDblClickgTagIndicator, VTS_NONE)
	ON_EVENT(CMarcManagerWorkView, IDC_gTagIndicator, -605 /* MouseDown */, OnMouseDowngTagIndicator, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMarcManagerWorkView, IDC_gTagIndicator, -602 /* KeyDown */, OnKeyDowngTagIndicator, VTS_PI2 VTS_I2)
	ON_EVENT(CMarcManagerWorkView, IDC_gTagSubfield, -605 /* MouseDown */, OnMouseDowngTagSubfield, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMarcManagerWorkView, IDC_gTagSubfield, -601 /* DblClick */, OnDblClickgTagSubfield, VTS_NONE)
	ON_EVENT(CMarcManagerWorkView, IDC_gTagInfo, -601 /* DblClick */, OnDblClickgTagInfo, VTS_NONE)
	ON_EVENT(CMarcManagerWorkView, IDC_gRelation, -601 /* DblClick */, OnDblClickgRelation, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMarcManagerWorkView::OnDblClickgTagIndicator() 
{
	// TODO: Add your control notification handler code here
    if(m_sTagAlias.IsEmpty()) return;

	INT nRow = m_pTagFirstIndicatorDM->GetRowCount() + m_pTagSecondIndicatorDM->GetRowCount();
	if (nRow == 0) return;
	INT row = m_pTagIndicatorGrid->GetRow();
	if (row == 0) return;

	CMarcManagerIndicator dlg(this);
	CString tmpPK;
    INT mode;

	//properly make radiobuttons enable 
	if(row > m_nFirstToSecond)
	{
		dlg.m_iRadioButton = 1; 
	    tmpPK = m_pTagSecondIndicatorDM->GetCellData(row-m_pTagFirstIndicatorDM->GetRowCount()-1 ,0);
	    dlg.m_iModifyFlag = 2;
	    mode = 1;
	}
    else
	{
		dlg.m_iRadioButton = 0; 
	    tmpPK = m_pTagFirstIndicatorDM->GetCellData(row-1,0);
	    dlg.m_iModifyFlag = 1;
	    mode = 0;
	}
	
	dlg.m_sIndicatorCode =  m_pTagIndicatorGrid->GetTextMatrix(row,1);
	dlg.m_sIndicatorDescrip = m_pTagIndicatorGrid->GetTextMatrix(row,2);
	
	CString strtmp;
	strtmp = m_pTagIndicatorGrid->GetTextMatrix(row,5);
	strtmp.MakeUpper();
	if(strtmp.GetLength()>0) 
		dlg.m_bDefault = TRUE;
	else 
		dlg.m_bDefault = FALSE;

	UpdateData(false);
	if(dlg.DoModal() != IDOK) return;
    UpdateData(true);
	
	CString tmpdescrip,tmptime;
    CTime time = m_pTagFirstIndicatorDM->GetDBTime();
    tmptime.Format(_T("%d/%d/%d"),time.GetYear(),time.GetMonth(),time.GetDay()); //DB TIME SETTING 
    
	tmpdescrip = IsNullData(dlg.m_sIndicatorDescrip);
	tmptime = IsNullData(tmptime);
	
	//update query for default value
	CString query,tmpquery;
    m_pTagFirstIndicatorDM->StartFrame();
	m_pTagFirstIndicatorDM->InitDBFieldData();
	
	CString tmpdefault;	
	if(FALSE == dlg.m_bDefault) tmpdefault = _T("''");
	if(TRUE == dlg.m_bDefault) {
		tmpdefault = _T("'V'");
		if(!mode)
		{
			query.Format(_T("UPDATE MARC_FIRST_INDICATOR_TBL SET DEFAULT_YN = '' WHERE MARC_TAG_ALIAS = '%s';"),m_sTagAlias); 	
		}
		else
		{
			query.Format(_T("UPDATE MARC_SECOND_INDICATOR_TBL SET DEFAULT_YN = '' WHERE MARC_TAG_ALIAS = '%s';"),m_sTagAlias);
		}
		m_pTagFirstIndicatorDM->AddFrame(query);
	}
	
	m_pTagFirstIndicatorDM->InitDBFieldData();
	//update query
	if(!mode)
	{
		query.Format(_T("UPDATE MARC_FIRST_INDICATOR_TBL SET IND_CODE = '%s', DESCRIP = %s, MODIFY_DATE = %s, DEFAULT_YN = %s"),dlg.m_sIndicatorCode,tmpdescrip,tmptime,tmpdefault);
    }
	else
	{
       query.Format(_T("UPDATE MARC_SECOND_INDICATOR_TBL SET IND_CODE = '%s', DESCRIP = %s, MODIFY_DATE = %s, DEFAULT_YN = %s"),dlg.m_sIndicatorCode,tmpdescrip,tmptime,tmpdefault);
    }  
    
    tmpquery.Format(_T(" WHERE PK = %s;"),tmpPK);
    
	query += tmpquery;

	m_pTagFirstIndicatorDM->AddFrame(query);
    INT ids = m_pTagFirstIndicatorDM->SendFrame();
    if(ids < 0) AfxMessageBox(_T("수정과정에서 에러가 발생하였습니다!")); 
	m_pTagFirstIndicatorDM->EndFrame();
    
	ShowTagIndicatorData(); //refresh

}

VOID CMarcManagerWorkView::OnMouseDowngTagIndicator(short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	INT row = m_pTagIndicatorGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		m_pTagIndicatorGrid->SetReverse(row-1);
	    m_nTagIndicatorCurRow = row;
	    m_nWhichTagGrid = 0;
	}

}

VOID CMarcManagerWorkView::OnbTagIndicator() 
{
	// TODO: Add your control notification handler code here
    if(m_sTagAlias.IsEmpty()) return;
	
	CMarcManagerIndicator dlg(this);
	dlg.m_iRadioButton = 0; 
	UpdateData(false);
	if(dlg.DoModal() != IDOK) return;
		
	//insert query
	CString query,tmpquery;
    m_pTagFirstIndicatorDM->StartFrame();
    
    CString tmpdescrip,tmptime;
    CTime time = m_pTagFirstIndicatorDM->GetDBTime();
    tmptime.Format(_T("%d/%d/%d"),time.GetYear(),time.GetMonth(),time.GetDay()); //DB TIME SETTING 
    
	tmpdescrip = IsNullData(dlg.m_sIndicatorDescrip);
	tmptime = IsNullData(tmptime);
	
	m_pTagFirstIndicatorDM->InitDBFieldData();
	CString tmpdefault;	
	if(FALSE == dlg.m_bDefault) tmpdefault = _T("''");
	if(TRUE == dlg.m_bDefault) {
		tmpdefault = _T("'V'");
		if(!dlg.m_iRadioButton)
		{
			query.Format(_T("UPDATE MARC_FIRST_INDICATOR_TBL SET DEFAULT_YN = '' WHERE MARC_TAG_ALIAS = '%s';"),m_sTagAlias); 	
		}
		else
		{
			query.Format(_T("UPDATE MARC_SECOND_INDICATOR_TBL SET DEFAULT_YN = '' WHERE MARC_TAG_ALIAS = '%s';"),m_sTagAlias);
		}
		m_pTagFirstIndicatorDM->AddFrame(query);
	}

	m_pTagFirstIndicatorDM->InitDBFieldData();
	if(!dlg.m_iRadioButton)
	{
		query.Format(_T("INSERT INTO MARC_FIRST_INDICATOR_TBL (PK,MARC_TAG_ALIAS,IND_CODE,DESCRIP,CREATE_DATE,DEFAULT_YN)")
		_T(" VALUES (%s.NEXTVAL, '%s','%s', %s , %s, %s);"),ReadCfgFile.m_sSequence,m_sTagAlias,dlg.m_sIndicatorCode,tmpdescrip,tmptime,tmpdefault);
    }
	else
	{
       query.Format(_T("INSERT INTO MARC_SECOND_INDICATOR_TBL (PK,MARC_TAG_ALIAS,IND_CODE,DESCRIP,CREATE_DATE,DEFAULT_YN)")
		_T(" VALUES (%s.NEXTVAL, '%s','%s', %s , %s, %s);"),ReadCfgFile.m_sSequence,m_sTagAlias,dlg.m_sIndicatorCode,tmpdescrip,tmptime,tmpdefault);
    }  
   
	m_pTagFirstIndicatorDM->AddFrame(query);
    INT ids = m_pTagFirstIndicatorDM->SendFrame();
    m_pTagFirstIndicatorDM->EndFrame();
    
	ShowTagIndicatorData(); //refresh
	if(ids<0) AfxMessageBox(_T("입력과정에서 에러가 발생하였습니다"));
}

VOID CMarcManagerWorkView::OnKeyDowngTagIndicator(short FAR* KeyCode, short Shift) 
{
	// TODO: Add your control notification handler code here
//	INT *k = (INT*)VK_DELETE;
	
//	if((INT*)KeyCode == k) 
	
	
//	CString code;
//	code.Format(_T("%d"), KeyCode);
//	TRACE(_T("  %s  KEYDOWNED!!!\n"),code);

}

BOOL CMarcManagerWorkView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DELETE)
	{
		if(m_bIsTagUsing)
		{
		   if(m_nWhichTagGrid == 0)
		   {
    		   DeleteIndicatorWithRow(m_nTagIndicatorCurRow);
		   }
		   else if(m_nWhichTagGrid == 1)
		   {
               DeleteSubfieldWithRow(m_nTagSubfieldCurRow);
		
		   }
		}
    }

	return CFormView::PreTranslateMessage(pMsg);
}

VOID CMarcManagerWorkView::OnMouseDowngTagSubfield(short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	INT row = m_pTagSubfieldGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		m_pTagSubfieldGrid->SetReverse(row-1);
	    m_nTagSubfieldCurRow = row;
	    m_nWhichTagGrid = 1;
	}
}

VOID CMarcManagerWorkView::OnDblClickgTagSubfield() 
{
	// TODO: Add your control notification handler code here
	if(m_sTagAlias.IsEmpty()) return;
	
	INT nRow = m_pTagSubfieldDM->GetRowCount();
	if (nRow == 0) return;
	INT row = m_pTagSubfieldGrid->GetRow();
	if (row == 0) return;

	CMarcManagerSubfield dlg(this);
	CString tmpPK;
    
	tmpPK = m_pTagSubfieldDM->GetCellData(row-1,0);
    
	
	dlg.m_sSubfieldCode = m_pTagSubfieldGrid->GetTextMatrix(row,0);
    dlg.m_sSubfiledPunc = m_pTagSubfieldGrid->GetTextMatrix(row,3);
	dlg.m_sSubfieldDescrip = m_pTagSubfieldGrid->GetTextMatrix(row,4);
    
	if(m_pTagSubfieldGrid->GetTextMatrix(row,1) == _T("Y"))
	{
		dlg.m_nRadioRepeat = 0;
	}
	else if(m_pTagSubfieldGrid->GetTextMatrix(row,1) == _T("N"))
	{
        dlg.m_nRadioRepeat = 1;
	}
	
	if(m_pTagSubfieldGrid->GetTextMatrix(row,2) == _T("필수"))
	{
		dlg.m_nRadioEssential = 0;
	}
	else if(m_pTagSubfieldGrid->GetTextMatrix(row,2) == _T("해당시필수"))
	{
        dlg.m_nRadioEssential = 1;
	}
	else if(m_pTagSubfieldGrid->GetTextMatrix(row,2) == _T("재량"))
	{
        dlg.m_nRadioEssential = 2;
	}
	else if(m_pTagSubfieldGrid->GetTextMatrix(row,2) == _T("공백"))
	{
        dlg.m_nRadioEssential = 3;
	}


	UpdateData(false);
	if(dlg.DoModal() != IDOK) return;
    UpdateData(true);
        	
	CString tmpdescrip,tmppunc,tmprepeat,tmpessential,tmptime;
    CTime time = m_pTagSubfieldDM->GetDBTime();
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
	
	tmppunc = IsNullData(dlg.m_sSubfiledPunc);
	tmpdescrip = IsNullData(dlg.m_sSubfieldDescrip);
	tmptime = IsNullData(tmptime);

	//update query
	CString query,tmpquery;
    m_pTagSubfieldDM->StartFrame();


	query.Format(_T("UPDATE MARC_SUBFIELD_TBL SET SUBFIELD_CODE = ")
	_T("'%s', REPEAT_TYPE = %s, ESSENTIAL_TYPE = %s, PUNCTUATION_MARK_SET =%s, DESCRIP = %s, MODIFY_DATE = %s"),dlg.m_sSubfieldCode,tmprepeat,tmpessential,tmppunc,tmpdescrip,tmptime);
   
    
    tmpquery.Format(_T(" WHERE PK = %s;"),tmpPK);
    
	query += tmpquery;

	m_pTagSubfieldDM->AddFrame(query);
    INT ids = m_pTagSubfieldDM->SendFrame();
    if(ids < 0) AfxMessageBox(_T("수정과정에서 에러가 발생하였습니다!")); 
	m_pTagSubfieldDM->EndFrame();
    
	ShowTagSubfieldData(); //refresh

}

VOID CMarcManagerWorkView::OnbTagSubfield() 
{
	// TODO: Add your control notification handler code here
	if(m_sTagAlias.IsEmpty()) return;
	
	CMarcManagerSubfield dlg(this);
	
	//default option
	dlg.m_nRadioRepeat = 0;
	dlg.m_nRadioEssential = 3;
	UpdateData(false);
	if(dlg.DoModal() != IDOK) return;
    
	CString tmpdescrip,tmppunc,tmprepeat,tmpessential,tmptime;
    CTime time = m_pTagSubfieldDM->GetDBTime();
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
	
	tmppunc = IsNullData(dlg.m_sSubfiledPunc);
	tmpdescrip = IsNullData(dlg.m_sSubfieldDescrip);
	tmptime = IsNullData(tmptime);
    
	CString tmpSubfieldCode;
	tmpSubfieldCode = _T("$");
	tmpSubfieldCode += dlg.m_sSubfieldCode;
	
	//insert query
	CString query;
    m_pTagSubfieldDM->StartFrame();
    
	query.Format(_T("INSERT INTO MARC_SUBFIELD_TBL (PK,MARC_TAG_ALIAS,SUBFIELD_CODE,REPEAT_TYPE,ESSENTIAL_TYPE,PUNCTUATION_MARK_SET,DESCRIP,CREATE_DATE)")
	
	_T(" VALUES (%s.NEXTVAL, '%s','%s', %s , %s, %s, %s, %s);"),ReadCfgFile.m_sSequence,m_sTagAlias,tmpSubfieldCode,tmprepeat,tmpessential, tmppunc, tmpdescrip,tmptime);
    
	m_pTagSubfieldDM->AddFrame(query);
    INT ids = m_pTagSubfieldDM->SendFrame();
	if(ids < 0) AfxMessageBox(_T("입력과정에서 에러가 발생하였습니다!")); 
    m_pTagSubfieldDM->EndFrame();
    
	ShowTagSubfieldData(); //refresh

}

VOID CMarcManagerWorkView::OnDblClickgTagInfo() 
{
	// TODO: Add your control notification handler code here
	if(m_sTagAlias.IsEmpty()) return;
    if(m_pTagInfoDM->GetRowCount() == 0) return;

	CMarcManagerTagInfo dlg(this);

	CString tmpPK;
    
	tmpPK = m_pTagInfoDM->GetCellData(0,0);
    	   
	dlg.m_sTagGroupCode = m_pTagInfoGrid->GetTextMatrix(1,0);
	dlg.m_sTagCode = m_pTagInfoGrid->GetTextMatrix(1,1);
   	dlg.m_sTagName = m_pTagInfoGrid->GetTextMatrix(1,2);
	dlg.m_sTagDescrip = m_sTagDescrip;
    dlg.m_sTagExample = m_sTagExample;
	
	if(m_pTagInfoGrid->GetTextMatrix(1,3) == _T("Y"))
	{
		dlg.m_nRadioRepeat = 0;
	}
	else if(m_pTagInfoGrid->GetTextMatrix(1,3) == _T("N"))
	{
        dlg.m_nRadioRepeat = 1;
	}
	
	if(m_pTagInfoGrid->GetTextMatrix(1,4) == _T("필수"))
	{
		dlg.m_nRadioEssential = 0;
	}
	else if(m_pTagInfoGrid->GetTextMatrix(1,4) == _T("해당시필수"))
	{
        dlg.m_nRadioEssential = 1;
	}
	else if(m_pTagInfoGrid->GetTextMatrix(1,4) == _T("재량"))
	{
        dlg.m_nRadioEssential = 2;
	}
	else if(m_pTagInfoGrid->GetTextMatrix(1,4) == _T("공백"))
	{
        dlg.m_nRadioEssential = 3;
	}

	dlg.m_bModifyFlag = true;

	UpdateData(false);
	if(dlg.DoModal() != IDOK) return;
    UpdateData(true);
    
	CString tmpdescrip,tmprepeat,tmpessential,tmpexample,tmptime;
    CTime time = m_pTagInfoDM->GetDBTime();
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
	

	//update query
	
	/*
	CString query,tmpquery;
    m_pTagInfoDM->StartFrame();

	query.Format(_T("UPDATE MARC_TAG_TBL SET TAG_NAME = ")
	_T("'%s', REPEAT_TYPE = %s, ESSENTIAL_TYPE = %s, DESCRIP = %s, EXAMPLE = %s, MODIFY_DATE = %s"),dlg.m_sTagName,tmprepeat,tmpessential,tmpdescrip,tmpexample,tmptime);
    
    tmpquery.Format(_T(" WHERE PK = %s;"),tmpPK);
    
	query += tmpquery;

	m_pTagInfoDM->AddFrame(query);
    INT ids = m_pTagInfoDM->SendFrame();
    if(ids < 0) AfxMessageBox(_T("수정과정에서 에러가 발생하였습니다!")); 
    m_pTagInfoDM->EndFrame();
    */

	CString query,tmpquery;
    m_pTagInfoDM->StartFrame();

	query.Format(_T("UPDATE MARC_TAG_TBL SET TAG_NAME = ")
	_T("'%s', REPEAT_TYPE = %s, ESSENTIAL_TYPE = %s, EXAMPLE = %s, MODIFY_DATE = %s"),dlg.m_sTagName,tmprepeat,tmpessential,tmpexample,tmptime);
    
    tmpquery.Format(_T(" WHERE PK = %s;"),tmpPK);
    
	query += tmpquery;

	m_pTagInfoDM->AddFrame(query);
    INT ids = m_pTagInfoDM->SendFrame();
    m_pTagInfoDM->EndFrame();
	if(ids < 0) {
		AfxMessageBox(_T("수정과정에서 에러가 발생하였습니다!")); 
		return;
	}


	//descrip만 따로 update
	ids = UpdateTagDescripData(m_pTagInfoDM, tmpPK, tmpdescrip);
	if(ids < 0) AfxMessageBox(_T("수정과정에서 에러가 발생하였습니다!")); 

	ShowTagInfoData(); //refresh
}

INT CMarcManagerWorkView::UpdateTagDescripData(CESL_DataMgr *pDM, CString strPK, CString strDescrip)
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

VOID CMarcManagerWorkView::OnDblClickgRelation() 
{
	// TODO: Add your control notification handler code here
	if(m_sRelationPK.IsEmpty()) return;
    if(m_pRelationDM->GetRowCount() == 0) return;

	CMarcManagerRelation dlg(this);
    	   
	dlg.m_sRelationGroupCode = m_pRelationGrid->GetTextMatrix(1,0);
	dlg.m_sRelationOperator = m_pRelationGrid->GetTextMatrix(1,1);
   	dlg.m_sRelationBasicParam = m_pRelationGrid->GetTextMatrix(1,2);
	dlg.m_sRelationParam = m_pRelationGrid->GetTextMatrix(1,3);
	dlg.m_sRelationDescrip = m_sRelationDescrip;
   
	dlg.m_bModifyFlag = true;

	UpdateData(false);
	if(dlg.DoModal() != IDOK) return;
    UpdateData(true);
    
	CString tmpdescrip,tmpparam,tmptime;
    CTime time = m_pTagInfoDM->GetDBTime();
    tmptime.Format(_T("%d/%d/%d"),time.GetYear(),time.GetMonth(),time.GetDay()); //DB TIME SETTING 
   
	tmpparam = IsNullData(dlg.m_sRelationParam);	
	tmpdescrip = IsNullData(dlg.m_sRelationDescrip);
    tmptime = IsNullData(tmptime);


	//update query
	CString query,tmpquery;
    m_pRelationDM->StartFrame();

	query.Format(_T("UPDATE MARC_RELATION_TBL SET OPERATOR = ")
	_T("'%s', BASIC_PARAM = '%s', PARAM = %s, DESCRIP = %s, MODIFY_DATE = %s"),dlg.m_sRelationOperator,dlg.m_sRelationBasicParam,tmpparam,tmpdescrip,tmptime);
    
    tmpquery.Format(_T(" WHERE PK = %s;"),m_sRelationPK);
    
	query += tmpquery;

	m_pRelationDM->AddFrame(query);
    INT ids = m_pRelationDM->SendFrame();
    if(ids < 0) AfxMessageBox(_T("수정과정에서 에러가 발생하였습니다!")); 
    m_pRelationDM->EndFrame();
    
	ShowRelationData(); //refresh


}

VOID CMarcManagerWorkView::OnGroupManage() 
{
	// TODO: Add your command handler code here
	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	CMarcManagerView *pView = (CMarcManagerView *)pframe->GetFirstView();
    pView->OnGroupManage();
}

VOID CMarcManagerWorkView::OnGroupSelect() 
{
	// TODO: Add your command handler code here
	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	CMarcManagerView *pView = (CMarcManagerView *)pframe->GetFirstView();
    pView->OnGroupSelect();
}

VOID CMarcManagerWorkView::OnRelationInsert() 
{
	// TODO: Add your command handler code here
	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	CMarcManagerView *pView = (CMarcManagerView *)pframe->GetFirstView();
    pView->OnRelationInsert();
}

VOID CMarcManagerWorkView::OnTagInsert() 
{
	// TODO: Add your command handler code here
	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	CMarcManagerView *pView = (CMarcManagerView *)pframe->GetFirstView();
    pView->OnTagInsert();
}
