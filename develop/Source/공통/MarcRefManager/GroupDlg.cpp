// GroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GroupDlg.h"
#include "GourpInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroupDlg dialog


CGroupDlg::CGroupDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_strGroupCode = _T("");
}


VOID CGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupDlg)
	DDX_Control(pDX, IDC_listGourp, m_listGroup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CGroupDlg)
	ON_LBN_DBLCLK(IDC_listGourp, OnDblclklistGourp)
	ON_BN_CLICKED(IDC_btnGroupModify, OnbtnGroupModify)
	ON_BN_CLICKED(IDC_btnGroupInsert, OnbtnGroupInsert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupDlg message handlers

BOOL CGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("마크레프_그룹") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 마크레프_그룹") );
		return FALSE;
	}

	m_pDM = FindDM(_T("DM_마크레프_그룹"));
	if(!m_pDM)
	{
		AfxMessageBox( _T("reference Error : DM_마크레프_그룹") );
		return FALSE;		
	}

	ShowGroupList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CGroupDlg::ShowGroupList()
{
	m_pDM->RefreshDataManager(_T(""));

	// List Box초기화 
	m_listGroup.ResetContent();

	INT nRow;
	nRow = m_pDM->GetRowCount();
	if (nRow == 0) {
		AfxMessageBox(_T("그룹 정보가 없습니다."));
		return;
	}

	// 데이타를 출력
	CString groupcode;
	for(INT i=0;i<nRow;i++) {
	    groupcode.Format(_T("▷ %s"), m_pDM->GetCellData(_T("MARC_GROUP_CODE"),i));
		m_listGroup.InsertString(i,groupcode);  
	}
}



VOID CGroupDlg::OnOK() 
{
	// TODO: Add extra validation here
	// 그룹 선택

	UpdateData(TRUE);
	INT nRow = m_pDM->GetRowCount();
	INT SelectedRow = m_listGroup.GetCurSel();
	if (nRow <= SelectedRow-1) {
		AfxMessageBox(_T("선택이 잘못되었습니다."));
		return;
	}
  
	m_strGroupCode = m_pDM->GetCellData(_T("MARC_GROUP_CODE"),SelectedRow);
	
	if(m_strGroupCode.IsEmpty())
	{
		AfxMessageBox(_T("선택이 잘못되었습니다."));
		return;
	}

	CDialog::OnOK();
}

VOID CGroupDlg::OnDblclklistGourp() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}

VOID CGroupDlg::OnbtnGroupInsert() 
{
	// TODO: Add your control notification handler code here
	
	//그룹 입력
	CGourpInfoDlg dlg;
	dlg.m_nOpenMode = 1;
	if(IDOK == dlg.DoModal()) ShowGroupList();

}

VOID CGroupDlg::OnbtnGroupModify() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	INT nRow = m_pDM->GetRowCount();
	INT SelectedRow = m_listGroup.GetCurSel();
	if (nRow <= SelectedRow-1) {
		AfxMessageBox(_T("선택이 잘못되었습니다."));
		return;
	}

	CString strPK = m_pDM->GetCellData(_T("PK"),SelectedRow);

	//그룹 수정
	CGourpInfoDlg dlg;
	dlg.m_nOpenMode = -1;
	dlg.m_strPK = strPK;
	if(IDOK == dlg.DoModal()) ShowGroupList();

}


BOOL CGroupDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DELETE)
	{
        DeleteMarcGroup();
	}

	return CDialog::PreTranslateMessage(pMsg);
}

INT CGroupDlg::DeleteMarcGroup()
{
    
	INT nRow = m_pDM->GetRowCount();
	INT SelectedRow = m_listGroup.GetCurSel();
	if (nRow <= SelectedRow-1) {
		AfxMessageBox(_T("선택이 잘못되었습니다."));
		return 0;
	}

    if(nRow == 0) return 0;
   
	CString tmpGroupCode,tmpPK;
	tmpPK = m_pDM->GetCellData(_T("PK"),SelectedRow);
	tmpGroupCode = m_pDM->GetCellData(_T("MARC_GROUP_CODE"),SelectedRow);
  
	CString msg;
	msg.Format(_T("그룹 삭제시 모든 하위 태그와 관계도 삭제됩니다. '%s' 그룹을 정말로 삭제하시겠습니까?"),tmpGroupCode);
    if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;
  
	
	CString query;

	m_pDM->StartFrame();
	
	query.Format(_T("DELETE FROM MARC_FIRST_INDICATOR_TBL WHERE MARC_TAG_ALIAS IN ")
				 _T("(SELECT ALIAS FROM MARC_TAG_TBL WHERE MARC_GROUP_CODE = '%s');"),tmpGroupCode);
	m_pDM->AddFrame(query);


	query.Format(_T("DELETE FROM MARC_SECOND_INDICATOR_TBL WHERE MARC_TAG_ALIAS IN ")
				 _T("(SELECT ALIAS FROM MARC_TAG_TBL WHERE MARC_GROUP_CODE = '%s');"),tmpGroupCode);
	m_pDM->AddFrame(query);

	query.Format(_T("DELETE FROM MARC_SUBFIELD_TBL WHERE MARC_TAG_ALIAS IN ")
				 _T("(SELECT ALIAS FROM MARC_TAG_TBL WHERE MARC_GROUP_CODE = '%s');"),tmpGroupCode);
	m_pDM->AddFrame(query);
 

	query.Format(_T("DELETE FROM MARC_TAG_TBL WHERE MARC_GROUP_CODE = '%s';"), tmpGroupCode);
    m_pDM->AddFrame(query);
      

    query.Format(_T("DELETE FROM MARC_RELATION_TBL WHERE MARC_GROUP_CODE = '%s';"), tmpGroupCode);
    m_pDM->AddFrame(query);
      
 
	query.Format(_T("DELETE FROM MARC_GROUP_TBL WHERE PK = %s;"), tmpPK);
    m_pDM->AddFrame(query);
	
	INT ids = m_pDM->SendFrame();
    m_pDM->EndFrame();
	if(ids<0) AfxMessageBox(_T("삭제하던 중 에러가 발생하였습니다"));
	else AfxMessageBox(_T("삭제하였습니다"));
	
	ShowGroupList(); //refresh


    return 0;
}
