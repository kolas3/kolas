// MarcManagerGroupSelect.cpp : implementation file
//

#include "stdafx.h"
#include "MarcManager.h"
#include "MarcManagerGroupSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerGroupSelect dialog


CMarcManagerGroupSelect::CMarcManagerGroupSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CMarcManagerGroupSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcManagerGroupSelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// KOL.UDF.022
	m_pGroupDM = NULL;
	m_sGroupCode = _T("");

}


VOID CMarcManagerGroupSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcManagerGroupSelect)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarcManagerGroupSelect, CDialog)
	//{{AFX_MSG_MAP(CMarcManagerGroupSelect)
	ON_WM_DESTROY()
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerGroupSelect message handlers

VOID CMarcManagerGroupSelect::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_pGroupDM) 
	{
       delete m_pGroupDM;
       m_pGroupDM = NULL;
	}

}

BOOL CMarcManagerGroupSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	
	m_pGroupDM = NULL;	
	m_pGroupDM = new CESL_DataMgr;
    
	SetTimer(0,100,NULL);
    

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMarcManagerGroupSelect::ShowGroupList(VOID)
{
	if (ReadCfgFile.ReadCfgFile(_T("")) < 0) return;
	if (!m_pGroupDM) return;

	INT ids;
	m_pGroupDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	
	ids = m_pGroupDM->RestructDataManager(_T("SELECT PK, MARC_GROUP_CODE FROM MARC_GROUP_TBL ORDER BY MARC_GROUP_CODE"));
	
	if (ids < 0) {
		AfxMessageBox(_T("그룹 정보를 읽지 못하였습니다."));
		return;
	}
	
	// List Box초기화 
	m_list.ResetContent();

	INT nRow;
	nRow = m_pGroupDM->GetRowCount();
	if (nRow == 0) {
		AfxMessageBox(_T("그룹 정보가 없습니다."));
		return;
	}

	// 데이타를 출력
	CString groupcode;
	for ( INT i = 0 ; i < nRow ; i++ ) {
	    groupcode.Format(_T("▷ %s"), m_pGroupDM->GetCellData(i, 1));
		m_list.InsertString(i,groupcode);  
	}

	UpdateData(false);

	return;
}

VOID CMarcManagerGroupSelect::OnDblclkList1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	INT nRow = m_pGroupDM->GetRowCount();
	INT SelectedRow = m_list.GetCurSel();
	if (nRow <= SelectedRow-1) {
		AfxMessageBox(_T("선택이 잘못되었습니다."));
		return;
	}
    
	m_sGroupCode = m_pGroupDM->GetCellData(SelectedRow, 1);

	CDialog::OnOK();
}


VOID CMarcManagerGroupSelect::OnOK() 
{
	// TODO: Add extra validation here
	OnDblclkList1(); 

}

VOID CMarcManagerGroupSelect::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 0)
	{
	    KillTimer(0);
		ShowGroupList();
    }

	CDialog::OnTimer(nIDEvent);
}
