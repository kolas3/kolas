// MarcManagerGroupManager.cpp : implementation file
//

#include "stdafx.h"
#include "MarcManager.h"
#include "MarcManagerGroupManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerGroupManager dialog


CMarcManagerGroupManager::CMarcManagerGroupManager(CWnd* pParent /*=NULL*/)
	: CDialog(CMarcManagerGroupManager::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcManagerGroupManager)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// KOL.UDF.022
	m_pGroupDM = NULL;
    m_pTagDM = NULL;
}


VOID CMarcManagerGroupManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcManagerGroupManager)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarcManagerGroupManager, CDialog)
	//{{AFX_MSG_MAP(CMarcManagerGroupManager)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_GROUP_INSERT, OnGroupInsert)
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerGroupManager message handlers

BOOL CMarcManagerGroupManager::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pGroupDM = NULL;	
	m_pGroupDM = new CESL_DataMgr;
    
	m_pTagDM = NULL;
	m_pTagDM = new CESL_DataMgr;

	ShowGroupList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMarcManagerGroupManager::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(m_pGroupDM) 
	{
       delete m_pGroupDM;
       m_pGroupDM = NULL;
	}
    
	if(m_pTagDM) 
	{
       delete m_pTagDM;
       m_pTagDM = NULL;
	}

	// TODO: Add your message handler code here
	
}

VOID CMarcManagerGroupManager::ShowGroupList(VOID)
{
	ReadCfgFile.ReadCfgFile(_T(""));
	if (!m_pGroupDM) return;

	INT ids;
	m_pGroupDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	
	ids = m_pGroupDM->RestructDataManager(_T("SELECT PK, MARC_GROUP_CODE, DESCRIP FROM MARC_GROUP_TBL ORDER BY MARC_GROUP_CODE"));
	
	if (ids < 0) {
		AfxMessageBox(_T("그룹 정보를 읽지 못하였습니다."));
		return;
	}
	
	// List Box초기화 
	m_list.ResetContent();

	INT nRow;
	nRow = m_pGroupDM->GetRowCount();

	// 데이타를 출력
	CString groupcode;
	for ( INT i = 0 ; i < nRow ; i++ ) {
	    groupcode.Format(_T("▷ %s"), m_pGroupDM->GetCellData(i, 1));
		m_list.InsertString(i,groupcode);  
	}

	UpdateData(false);

	return;
}



VOID CMarcManagerGroupManager::OnGroupInsert() 
{
	// TODO: Add your control notification handler code here
	CMarcManagerGroupInsert dlg;
    if(dlg.DoModal() != IDOK) return;
    UpdateData(true);
  
	//insert query making
    CString query,tmpDescrip,tmpTime;

	CTime time = m_pGroupDM->GetDBTime();
    tmpTime.Format(_T("%d/%d/%d"),time.GetYear(),time.GetMonth(),time.GetDay()); //DB TIME SETTING 
    tmpDescrip = IsNullData(dlg.m_sGroupDescrip);
    tmpTime = IsNullData(tmpTime);

    m_pGroupDM->StartFrame();

	query.Format(_T("INSERT INTO MARC_GROUP_TBL (PK,MARC_GROUP_CODE,DESCRIP,CREATE_DATE)")
	_T(" VALUES (%s.NEXTVAL, '%s', %s , %s);"), ReadCfgFile.m_sSequence ,dlg.m_sGroupCode,tmpDescrip,tmpTime);
	
	m_pGroupDM->AddFrame(query);
    
	INT ids = m_pGroupDM->SendFrame();
    if(ids < 0) AfxMessageBox(_T("수정과정에서 에러가 발생하였습니다!")); 
	m_pGroupDM->EndFrame();
    
	ShowGroupList(); //refresh

}


VOID CMarcManagerGroupManager::OnDblclkList1() 
{
	// TODO: Add your control notification handler code here
	    
	INT nRow = m_pGroupDM->GetRowCount();
	INT SelectedRow = m_list.GetCurSel();
	if (nRow <= SelectedRow-1) {
		AfxMessageBox(_T("선택이 잘못되었습니다."));
		return;
	}
    
	CMarcManagerGroupInsert dlg;

	dlg.m_sGroupCode = m_pGroupDM->GetCellData(SelectedRow, 1);
    dlg.m_sGroupDescrip = m_pGroupDM->GetCellData(SelectedRow, 2);
    dlg.m_bModifyFlag = TRUE;

    UpdateData(false);
    if(dlg.DoModal() != IDOK) return;
    UpdateData(true);

	//modify query making
    CString tmpPK,query,tmpDescrip,tmpTime;
    
	tmpPK = m_pGroupDM->GetCellData(SelectedRow, 0);
	CTime time = m_pGroupDM->GetDBTime();
    tmpTime.Format(_T("%d/%d/%d"),time.GetYear(),time.GetMonth(),time.GetDay()); //DB TIME SETTING 
    tmpDescrip = IsNullData(dlg.m_sGroupDescrip);
    tmpTime = IsNullData(tmpTime);

    m_pGroupDM->StartFrame();

    query.Format(_T("UPDATE MARC_GROUP_TBL SET DESCRIP  = %s, MODIFY_DATE = %s WHERE PK = %s;"), tmpDescrip , tmpTime, tmpPK);

	m_pGroupDM->AddFrame(query);
    
	INT ids = m_pGroupDM->SendFrame();
    if(ids < 0) AfxMessageBox(_T("수정과정에서 에러가 발생하였습니다!")); 
	m_pGroupDM->EndFrame();
    
	ShowGroupList(); //refresh

}


CString CMarcManagerGroupManager::IsNullData(CString data)
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

INT CMarcManagerGroupManager::DeleteMarcGroup()
{
    INT nRow = m_pGroupDM->GetRowCount();
	INT SelectedRow = m_list.GetCurSel();
	if (nRow <= SelectedRow-1) {
		AfxMessageBox(_T("선택이 잘못되었습니다."));
		return 0;
	}

    if(nRow == 0) return 0;
   
	CString tmpGroupCode,tmpPK;
	tmpPK = m_pGroupDM->GetCellData(SelectedRow, 0);
	tmpGroupCode = m_pGroupDM->GetCellData(SelectedRow, 1);
  
	CString msg;
	msg.Format(_T("그룹 삭제시 모든 하위 태그와 관계도 삭제됩니다. '%s' 그룹을 정말로 삭제하시겠습니까?"),tmpGroupCode);
    if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;
  
	if (!m_pTagDM) return -1;

	INT ids;
	m_pTagDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	
	CString query;
    query.Format(_T("SELECT ALIAS FROM MARC_TAG_TBL WHERE MARC_GROUP_CODE = '%s'"),tmpGroupCode);

	ids = m_pTagDM->RestructDataManager(query);
	
	if (ids < 0) {
		AfxMessageBox(_T("하위 태그 정보를 읽지 못하였습니다."));
		return -2;
	}
     
    m_pTagDM->StartFrame();
	
	//먼저 모든 하위 태그의 하위 정보들을 삭제시킨다
    for(INT i=0; i<m_pTagDM->GetRowCount();i++)
	{
       query.Format(_T("DELETE FROM MARC_FIRST_INDICATOR_TBL WHERE MARC_TAG_ALIAS = '%s';"), m_pTagDM->GetCellData(i, 0));
       m_pTagDM->AddFrame(query);
       
	   query.Format(_T("DELETE FROM MARC_SECOND_INDICATOR_TBL WHERE MARC_TAG_ALIAS = '%s';"), m_pTagDM->GetCellData(i, 0));
       m_pTagDM->AddFrame(query);

	   query.Format(_T("DELETE FROM MARC_SUBFIELD_TBL WHERE MARC_TAG_ALIAS = '%s';"), m_pTagDM->GetCellData(i, 0));
       m_pTagDM->AddFrame(query);
	}

	//그리고 하위태그들을 삭제한다 
	query.Format(_T("DELETE FROM MARC_TAG_TBL WHERE MARC_GROUP_CODE = '%s';"), tmpGroupCode);
    m_pTagDM->AddFrame(query);
      
	//그리고 하위관계들을 삭제한다
    query.Format(_T("DELETE FROM MARC_RELATION_TBL WHERE MARC_GROUP_CODE = '%s';"), tmpGroupCode);
    m_pTagDM->AddFrame(query);
      
    //그리고 해당 그룹을 삭제한다 
    query.Format(_T("DELETE FROM MARC_GROUP_TBL WHERE PK = %s;"), tmpPK);
    m_pTagDM->AddFrame(query);
	
	ids = m_pTagDM->SendFrame();
    m_pTagDM->EndFrame();
	if(ids<0) AfxMessageBox(_T("삭제하던 중 에러가 발생하였습니다"));
	
	ShowGroupList(); //refresh

    return 0;
}

BOOL CMarcManagerGroupManager::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DELETE)
	{
        DeleteMarcGroup();
	}

	return CDialog::PreTranslateMessage(pMsg);
}
