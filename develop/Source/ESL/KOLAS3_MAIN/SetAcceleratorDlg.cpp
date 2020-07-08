// SetAcceleratorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "SetAcceleratorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ToolBarItem.h"
#include "SetAccleratorDlg_Modify.h"
/////////////////////////////////////////////////////////////////////////////
// CSetAcceleratorDlg dialog


CSetAcceleratorDlg::CSetAcceleratorDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSetAcceleratorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetAcceleratorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pGrid = NULL;
	m_strMenuAlias = _T("");
	m_pListToolBarGroup = NULL;
	m_pListToolBarItem = NULL;
	m_pListToolBar = NULL;
}


VOID CSetAcceleratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetAcceleratorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetAcceleratorDlg, CDialog)
	//{{AFX_MSG_MAP(CSetAcceleratorDlg)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bMODIFY, OnbMODIFY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetAcceleratorDlg message handlers

BOOL CSetAcceleratorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pGrid = new CESL_Grid(NULL);
	m_pGrid->SubclassDlgItem(IDC_MSHFLEXGRID1, this);
	InitGrid();
	RefreshList();
	if (m_arrRowIdxList.GetSize() == 0) {
		AfxMessageBox(_T("툴바 버튼이 없습니다."));
		PostMessage(WM_CLOSE);
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSetAcceleratorDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pGrid) {
		delete m_pGrid;
		m_pGrid = NULL;
	}
	return CDialog::DestroyWindow();
}

VOID CSetAcceleratorDlg::OnbCLOSE() 
{
	CDialog::OnOK();
}

VOID CSetAcceleratorDlg::OnbMODIFY() 
{

	CSetAccleratorDlg_Modify dlg(this);	
	dlg.DoModal () ;
/*
	if (m_arrRowIdxList.GetSize() == 0) return;
	INT nRow = m_pGrid->GetRow()-1;
	INT nDataIdx = m_arrRowIdxList.GetAt(nRow);
	POSITION pos = m_pListToolBarItem->FindIndex(nDataIdx);
	if (pos == NULL) return;
	CToolBarItem *pToolBarItem = (CToolBarItem *)m_pListToolBarItem->GetAt(pos);;
	if (pToolBarItem == NULL) return;


	CSetAccleratorDlg_Modify dlg(this);	
	dlg.m_strACC = pToolBarItem->m_strAcceclerator;

	if (dlg.DoModal() != IDOK) return;
	
	pToolBarItem->m_strAcceclerator = dlg.m_strACC;
	
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(CONNECTION_INFO);
	
	CString strQuery;
	strQuery.Format(_T("UPDATE ESL_TOOLBAR_TBL SET ACCECLERATOR='%s' WHERE PK=%s;"), dlg.m_strACC, pToolBarItem->m_strPK);
	dm.StartFrame();
	dm.AddFrame(strQuery);
	INT ids = dm.SendFrame();
	dm.EndFrame();

	if (ids == 0) {
		pToolBarItem->m_strAcceclerator = dlg.m_strACC;
		m_pGrid->SetTextMatrix(nRow+1, 3, pToolBarItem->m_strAcceclerator);
		AfxMessageBox(_T("수정되었습니다."));
	} else {
		AfxMessageBox(_T("수정작업중 에러가 발생하였습니다."));
	}
	
*/	


	return;
}

INT CSetAcceleratorDlg::InitGrid(VOID)
{
	if (m_pGrid == NULL) return -1;
	m_pGrid->RemoveAll();
	m_pGrid->SetRows(2);
	m_pGrid->SetCols(0, 4);


    //=======================================================================
	//2004-03-23[이학중]
	//m_pGrid->SetColumnInfo(1-1, _T("그룹"),		125,	1);
	//m_pGrid->SetColumnInfo(2-1, _T("캡션"),		125,	1);
    m_pGrid->SetColumnInfo (-1,_T(""),30,4);
	m_pGrid->SetColumnInfo(1-1, _T("그룹"),		0,	1);
	m_pGrid->SetColumnInfo(2-1, _T("버튼이름"),		140,	1);	
	//=======================================================================    
	m_pGrid->SetColumnInfo(3-1, _T("단축키"),	100,	4);
	return 0;
}

INT CSetAcceleratorDlg::RefreshList(VOID)
{
	m_arrRowIdxList.RemoveAll();

	INT nTotalDatas = m_pListToolBarItem->GetCount();
	INT i, nDatas = 0;
	POSITION pos = m_pListToolBarItem->GetHeadPosition();
	CToolBarItem *pToolBarItem;
	for (i = 0; i < nTotalDatas; i++) {
		pToolBarItem = NULL;
		pToolBarItem = (CToolBarItem *)m_pListToolBarItem->GetNext(pos);
		if (pToolBarItem == NULL) continue;
		if (pToolBarItem->m_strStyle != _T("B")) continue;

		if (pToolBarItem->m_strMenuAlias == m_strMenuAlias) nDatas++;
	}
	
	if (nDatas == 0) return -1;
	m_pGrid->SetRows(nDatas+1);

	INT nRow = 0;
	CString str;
	pos = m_pListToolBarItem->GetHeadPosition();
	for (i = 0; i < nTotalDatas; i++) {
		pToolBarItem = NULL;
		pToolBarItem = (CToolBarItem *)m_pListToolBarItem->GetNext(pos);
		if (pToolBarItem == NULL) continue;
		if (pToolBarItem->m_strMenuAlias != m_strMenuAlias) continue;
		if (pToolBarItem->m_strStyle != _T("B")) continue;

		m_arrRowIdxList.Add(i);
		str.Format(_T("%d"), nRow);
		m_pGrid->SetTextMatrix(nRow+1, 0, str);
		m_pGrid->SetTextMatrix(nRow+1, 1, pToolBarItem->m_strToolGroupAlias);
		m_pGrid->SetTextMatrix(nRow+1, 2, pToolBarItem->m_strCaption);
		m_pGrid->SetTextMatrix(nRow+1, 3, pToolBarItem->m_strAcceclerator);

		nRow++;
	}

	return 0;
}
