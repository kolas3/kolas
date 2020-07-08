// AccessControlToolBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccessControl.h"
#include "AccessControlToolBarDlg.h"
#include "AccessControlToolGroupInsertDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "AccessControlToolbarInsertDlg.h"
#include "AccessControlSelectionListDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAccessControlToolBarDlg dialog


CAccessControlToolBarDlg::CAccessControlToolBarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessControlToolBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessControlToolBarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pGrid = new CESL_Grid(NULL);
	m_pDM = new CESL_DataMgr;
	pUserList = NULL;

	m_pGrid_Tool = new CESL_Grid(NULL);
	m_pToolDM = new CESL_DataMgr;

	m_pToolbar = NULL;
	pNormalList = NULL;
	pHotList = NULL;
	pDisabledList = NULL;


	// KOL.UDF.022
	FLAG = ' ';
	MENU_ALIAS = _T("");
	USER_ID = _T("");
	SEQUENCE = _T("");
	GROUP_ALIAS = _T("");	
}


VOID CAccessControlToolBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlToolBarDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlToolBarDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessControlToolBarDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bINSERT, OnbINSERT)
	ON_BN_CLICKED(IDC_bMODIFY, OnbMODIFY)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_BN_CLICKED(IDC_bGropuINSERT, OnbGropuINSERT)
	ON_BN_CLICKED(IDC_bGroupMODIFY, OnbGroupMODIFY)
	ON_BN_CLICKED(IDC_bGroupDELETE, OnbGroupDELETE)
	ON_BN_CLICKED(IDC_bREARRANGE, OnbREARRANGE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlToolBarDlg message handlers

BOOL CAccessControlToolBarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ReadCfgFile.ReadCfgFile(_T(""));
	m_pDM->SetDB(ReadCfgFile.m_sDBKIND);
	m_pDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	m_pToolDM->SetDB(ReadCfgFile.m_sDBKIND);
	m_pToolDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	m_pGrid->SubclassDlgItem(IDC_MSHFLEXGRID1, this);
	m_pGrid_Tool->SubclassDlgItem(IDC_MSHFLEXGRID2, this);


	InitGrid();
	InitToolGrid();
	ShowToolBarGroup();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CAccessControlToolBarDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_pGrid) {
		delete m_pGrid;
		m_pGrid = NULL;
	}
	if (m_pDM) {
		delete m_pDM;
		m_pDM = NULL;
	}

	if (m_pGrid_Tool) {
		delete m_pGrid_Tool;
		m_pGrid_Tool = NULL;
	}
	if (m_pToolDM) {
		delete m_pToolDM;
		m_pToolDM = NULL;
	}
	FreeToolBar();
}

VOID CAccessControlToolBarDlg::ShowToolBarList(VOID)
{
	CString query;
	//                   0     1        2        3      4       5      6    7        8             9           10            11         12
	query.Format(_T("SELECT PK, ALIAS, MENU_ALIAS, IDX, CAPTION, STYLE, TYPE, SM, BITMAP_NORMAL, BITMAP_HOT, BITMAP_DISABLED,TOOLTIP, ACCECLERATOR ")
				 _T("FROM ESL_TOOLBAR_TBL WHERE USER_ID='%s' AND MENU_ALIAS='%s' AND TOOLGROUP_ALIAS='%s' ORDER BY IDX"),
				 USER_ID, MENU_ALIAS, GROUP_ALIAS);
	m_pDM->RestructDataManager(query);

	INT nRow = m_pDM->GetRowCount();
	INT gridrow;
	if (nRow == 0) gridrow = 2;
		else gridrow = nRow+1;
	m_pGrid->RemoveAll();
	InitGrid();
	m_pGrid->SetRows(gridrow);
	
	CString msg;
	for (INT i = 0; i < nRow; i ++) {
		msg.Format(_T("%d"), i);
		m_pGrid->SetTextMatrix(i+1, 0, msg);
		m_pGrid->SetTextMatrix(i+1, 1, m_pDM->GetCellData(i, 1));
		m_pGrid->SetTextMatrix(i+1, 2, m_pDM->GetCellData(i, 3));
		m_pGrid->SetTextMatrix(i+1, 3, m_pDM->GetCellData(i, 4));
		m_pGrid->SetTextMatrix(i+1, 4, m_pDM->GetCellData(i, 5));
		m_pGrid->SetTextMatrix(i+1, 5, m_pDM->GetCellData(i, 6));
		m_pGrid->SetTextMatrix(i+1, 6, m_pDM->GetCellData(i, 7));
		m_pGrid->SetTextMatrix(i+1, 7, m_pDM->GetCellData(i, 8));
		m_pGrid->SetTextMatrix(i+1, 8, m_pDM->GetCellData(i, 9));
		m_pGrid->SetTextMatrix(i+1, 9, m_pDM->GetCellData(i, 10));
		m_pGrid->SetTextMatrix(i+1, 10, m_pDM->GetCellData(i, 12));
	}
	return;
}

VOID CAccessControlToolBarDlg::ShowToolBarGroup(VOID)
{
	CString query;
	query.Format(_T("SELECT PK, ALIAS, MENU_ALIAS, DESCRIPT, TOOLGROUP_ALIGN ")
				 _T("FROM ESL_TOOLGROUP_TBL WHERE MENU_ALIAS='%s' "), MENU_ALIAS);

	m_pToolDM->RestructDataManager(query);

	INT nRow = m_pToolDM->GetRowCount();
	INT gridrow;
	if (nRow == 0) gridrow = 2;
		else gridrow = nRow+1;
	m_pGrid_Tool->RemoveAll();
	InitToolGrid();
	m_pGrid_Tool->SetRows(gridrow);
	
	CString msg;
	for (INT i = 0; i < nRow; i ++) {
		msg.Format(_T("%d"), i);
		m_pGrid_Tool->SetTextMatrix(i+1, 0, msg);
		msg = m_pToolDM->GetCellData(i, 4);
		if (msg == _T("T")) msg = _T("위쪽(Top)");
		if (msg == _T("B")) msg = _T("아랫쪽(Bottom)");
		if (msg == _T("L")) msg = _T("왼쪽(Left)");
		if (msg == _T("R")) msg = _T("오른쪽(Right)");
		m_pGrid_Tool->SetTextMatrix(i+1, 1, msg);
		m_pGrid_Tool->SetTextMatrix(i+1, 2, m_pToolDM->GetCellData(i, 1));
		m_pGrid_Tool->SetTextMatrix(i+1, 3, m_pToolDM->GetCellData(i, 2));
		m_pGrid_Tool->SetTextMatrix(i+1, 4, m_pToolDM->GetCellData(i, 3));	
	}
	return;
}
VOID CAccessControlToolBarDlg::OnbCLOSE() 
{
	CDialog::OnOK();	
}

VOID CAccessControlToolBarDlg::InitGrid(VOID)
{
	m_pGrid->RemoveAll();
	m_pGrid->SetColumnInfo(0-1, _T("")			, 30		,		7);
	m_pGrid->SetColumnInfo(1-1, _T("ALIAS")		, 100		,		1);
	m_pGrid->SetColumnInfo(2-1, _T("Idx")		, 30		,		7);
	m_pGrid->SetColumnInfo(3-1, _T("Caption")	, 100		,		1);
	m_pGrid->SetColumnInfo(4-1, _T("Style")		, 40		,		4);
	m_pGrid->SetColumnInfo(5-1, _T("Type")		, 40		,		4);
	m_pGrid->SetColumnInfo(6-1, _T("SM")		, 105		,		1);
	m_pGrid->SetColumnInfo(7-1, _T("Normal")	, 90		,		1);
	m_pGrid->SetColumnInfo(8-1, _T("Hot")		, 90		,		1);
	m_pGrid->SetColumnInfo(9-1, _T("Disabled")	, 90		,		1);
	m_pGrid->SetColumnInfo(10-1, _T("Accelerator")	, 80	,		4);

}
VOID CAccessControlToolBarDlg::InitToolGrid(VOID)
{

	m_pGrid_Tool->RemoveAll();
	m_pGrid_Tool->SetColumnInfo(0-1, _T("")					, 30		,		7);
	m_pGrid_Tool->SetColumnInfo(1-1, _T("ALIGN")			, 80		,		4);
	m_pGrid_Tool->SetColumnInfo(2-1, _T("ALIAS")			, 100		,		1);
	m_pGrid_Tool->SetColumnInfo(3-1, _T("MENU ALIAS")		, 100		,		1);
	m_pGrid_Tool->SetColumnInfo(4-1, _T("DESCRPTION")		, 260		,		1);
}


VOID CAccessControlToolBarDlg::OnbINSERT() 
{
	if( GROUP_ALIAS.GetLength() == 0 ) return;

	if (FLAG == 'S') {
		// Admin 일경우는 입력을 받아서 입력을 해준다.
		INSERT_Administrator();
	} else {
		// 메소드나 유져 모드인 경우
		INSERT_UserOrMethod();
	}
	ShowToolBarList();
}

INT CAccessControlToolBarDlg::INSERT_Administrator(VOID)
{
	CAccessControlToolBarInsertDlg dlg;
	dlg.MODE = 1;			// 입력모드
	if (dlg.DoModal() != IDOK) return 0;

	CString query;
	INT ids;
	ids = m_pDM->StartFrame();
	query.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE ALIAS='%s' AND MENU_ALIAS='%s' AND TOOLGROUP_ALIAS='%s' AND USER_ID='%s' AND FLAG='%c';"), dlg.m_sAlias, MENU_ALIAS, GROUP_ALIAS, USER_ID, FLAG);
	m_pDM->AddFrame(query);

	if (m_pDM->m_dbkind == 2) {
		dlg.m_sNormal.Replace(_T("'"), _T("''"));
		dlg.m_sNormal.Replace(_T("\\"), _T("\\\\"));
		dlg.m_sHot.Replace(_T("'"), _T("''"));
		dlg.m_sHot.Replace(_T("\\"), _T("\\\\"));
		dlg.m_sDisabled.Replace(_T("'"), _T("''"));
		dlg.m_sDisabled.Replace(_T("\\"), _T("\\\\"));
	}
	CString strPK;
	if (m_pDM->m_dbkind == 2) m_pDM->MakeRecKey(strPK, SEQUENCE);
	if (m_pDM->m_dbkind == 1) strPK.Format(_T("%s.NEXTVAL"), SEQUENCE);
	query.Format(_T("INSERT INTO ESL_TOOLBAR_TBL (PK, FLAG, USER_ID, ALIAS, MENU_ALIAS, TOOLGROUP_ALIAS, IDX, CAPTION, STYLE, TYPE, SM, BITMAP_NORMAL, BITMAP_HOT, BITMAP_DISABLED , TOOLTIP, ACCECLERATOR) ")
				 _T(" VALUES (%s, '%c', '%s', '%s', '%s', '%s', %d, '%s', '%c', '%c', '%s', '%s', '%s', '%s' ,'%s', '%s');"),
				 strPK, FLAG, USER_ID, dlg.m_sAlias, MENU_ALIAS, GROUP_ALIAS, dlg.m_iIdx, dlg.m_sCaption, dlg.m_cSTYLE, dlg.m_cTYPE, dlg.m_sSM, dlg.m_sNormal, dlg.m_sHot, dlg.m_sDisabled, dlg.m_ToolTip, dlg.m_strACC);
	ids = m_pDM->AddFrame(query);
	ids = m_pDM->SendFrame();
	m_pDM->EndFrame();
	if (ids < 0) AfxMessageBox(_T("ToolBar Button을 입력하던중 에러가 발생하였습니다."));
		else AfxMessageBox(_T("입력되었습니다."));
	
	return 0;
}

INT CAccessControlToolBarDlg::INSERT_UserOrMethod(VOID)
{
	CAccessControlSelectionListDlg SelectDlg;
	SelectDlg.MODE = 2;		// Toolbar Select Mode

	CString query;

	CESL_DataMgr *pDM = new CESL_DataMgr;
	if (!pDM) return -2;
	pDM->SetDB(ReadCfgFile.m_sDBKIND);
	pDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	//                     0     1      2       3      4   5         6            7           8               9          10            11                   12
	query.Format(_T("SELECT ALIAS, IDX, CAPTION, STYLE, TYPE, SM, BITMAP_NORMAL, BITMAP_HOT, BITMAP_DISABLED, TOOLTIP, SPF_USE_FLAG, PROGRAME_USE_FLAG, ACCECLERATOR FROM ESL_TOOLBAR_TBL WHERE MENU_ALIAS='%s' AND TOOLGROUP_ALIAS='%s' AND USER_ID='Administrator' AND FLAG='S' ORDER BY IDX"), MENU_ALIAS, GROUP_ALIAS);
	pDM->RestructDataManager(query);
	if (pDM->GetRowCount() == 0) {
		AfxMessageBox(_T("Administrator에게 관련 정보가 없습니다. Administrator에 먼저 작업을 해주십시오."));
		return -1;
	}

	INT nRow = pDM->GetRowCount();
	SelectDlg.m_pDM = pDM; 
	if (SelectDlg.DoModal() != IDOK) return 0;
	if (SelectDlg.nSelectedRow < 0 || SelectDlg.nSelectedRow >= nRow) return -3;

	// 선택된 데이타의 처리
	CString sAlias = pDM->GetCellData(SelectDlg.nSelectedRow, 0);
	sAlias.TrimLeft();			sAlias.TrimRight();
	CString sIdx = pDM->GetCellData(SelectDlg.nSelectedRow, 1);
	sIdx.TrimLeft();			sIdx.TrimRight();
	CString sCaption = pDM->GetCellData(SelectDlg.nSelectedRow, 2);
	sCaption.TrimLeft();		sCaption.TrimRight();
	CString sStyle = pDM->GetCellData(SelectDlg.nSelectedRow, 3);
	sStyle.TrimLeft();			sStyle.TrimRight();
	CString sType = pDM->GetCellData(SelectDlg.nSelectedRow, 4);
	sType.TrimLeft();			sType.TrimRight();
	CString sSM = pDM->GetCellData(SelectDlg.nSelectedRow, 5);
	sSM.TrimLeft();				sSM.TrimRight();
	CString sBITMAP_NORMAL = pDM->GetCellData(SelectDlg.nSelectedRow, 6);
	if (pDM->m_dbkind == 2) sBITMAP_NORMAL.Replace(_T("\\"), _T("\\\\"));
	sBITMAP_NORMAL.TrimLeft();	sBITMAP_NORMAL.TrimRight();
	CString sBITMAP_HOT = pDM->GetCellData(SelectDlg.nSelectedRow, 7);
	if (pDM->m_dbkind == 2) sBITMAP_HOT.Replace(_T("\\"), _T("\\\\"));
	sBITMAP_HOT.TrimLeft();	sBITMAP_HOT.TrimRight();
	CString sBITMAP_DISABLED = pDM->GetCellData(SelectDlg.nSelectedRow, 8);
	if (pDM->m_dbkind == 2) sBITMAP_DISABLED.Replace(_T("\\"), _T("\\\\"));
	sBITMAP_DISABLED.TrimLeft();	sBITMAP_DISABLED.TrimRight();
	CString sTOOLTIP = pDM->GetCellData(SelectDlg.nSelectedRow, 9);
	sTOOLTIP.TrimLeft();		sTOOLTIP.TrimRight();
	CString sSPF_USE_FLAG = pDM->GetCellData(SelectDlg.nSelectedRow, 10);
	sSPF_USE_FLAG.TrimLeft();		sSPF_USE_FLAG.TrimRight();
	CString sPROGRAME_USE_FLAG = pDM->GetCellData(SelectDlg.nSelectedRow, 11);
	sPROGRAME_USE_FLAG.TrimLeft();		sPROGRAME_USE_FLAG.TrimRight();
	CString sACCECLERATOR = pDM->GetCellData(SelectDlg.nSelectedRow, 12);
	sACCECLERATOR.TrimLeft();		sACCECLERATOR.TrimRight();

	// 입력된 데이타를 처리한다.
	CString user_id;
	INT ids;


	ids = m_pDM->StartFrame();
	// 자기자신(메소드, 유져)의 중복 데이타 삭제하고
	query.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE ALIAS='%s' AND MENU_ALIAS='%s' AND TOOLGROUP_ALIAS='%s' AND USER_ID='%s' AND FLAG='%c';"), sAlias, MENU_ALIAS, GROUP_ALIAS, USER_ID, FLAG);
	ids = m_pDM->AddFrame(query);

	// 메소드인경우 유져중에서 중복된 데이타를 미리 모두 삭제한다.
	if (FLAG == 'M') {
		// 해당 메소드를 사용하는 유져중에서 중복 데이타 삭제하고
		nRow = pUserList->GetCount();
		for (INT i = 0; i < nRow; i++) {
			user_id = pUserList->GetAt(i);
			if (user_id.GetLength() == 0) continue;
			query.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE ALIAS='%s' AND MENU_ALIAS='%s' AND TOOLGROUP_ALIAS = '%s' AND USER_ID='%s' AND FLAG='U';"), sAlias, MENU_ALIAS, GROUP_ALIAS, user_id);
			ids = m_pDM->AddFrame(query);
		}
	}

	// 해당 메소드(유져) 정보를 입력해주고
	CString strPK;
	if (m_pDM->m_dbkind == 2) m_pDM->MakeRecKey(strPK, SEQUENCE);
	if (m_pDM->m_dbkind == 1) strPK.Format(_T("%s.NEXTVAL"), SEQUENCE);
	query.Format(_T("INSERT INTO ESL_TOOLBAR_TBL (PK, FLAG, USER_ID, ALIAS, MENU_ALIAS, TOOLGROUP_ALIAS, IDX, CAPTION, STYLE, TYPE, SM, BITMAP_NORMAL, BITMAP_HOT, BITMAP_DISABLED , TOOLTIP, SPF_USE_FLAG, PROGRAME_USE_FLAG, ACCECLERATOR) ")
				 _T(" VALUES (%s, '%c', '%s', '%s', '%s', '%s', %s, '%s', '%s', '%s', '%s', '%s', '%s', '%s' ,'%s', '%s', '%s', '%s');"),
				 strPK, FLAG, USER_ID, sAlias, MENU_ALIAS, GROUP_ALIAS, sIdx, sCaption, sStyle, sType, sSM, sBITMAP_NORMAL, sBITMAP_HOT, sBITMAP_DISABLED, sTOOLTIP, sSPF_USE_FLAG, sPROGRAME_USE_FLAG, sACCECLERATOR);
	ids = m_pDM->AddFrame(query);
	
	// 메소드일경우는 해당 메소드를 사용하는 모든 유져에도 입력해준다.
	if (FLAG == 'M') {
		nRow = pUserList->GetCount();
		for (INT i = 0; i < nRow; i++) {
			user_id = pUserList->GetAt(i);
			if (user_id.GetLength() == 0) continue;
			if (m_pDM->m_dbkind == 2) m_pDM->MakeRecKey(strPK, SEQUENCE);
			if (m_pDM->m_dbkind == 1) strPK.Format(_T("%s.NEXTVAL"), SEQUENCE);
			query.Format(_T("INSERT INTO ESL_TOOLBAR_TBL (PK, FLAG, USER_ID, ALIAS, MENU_ALIAS, TOOLGROUP_ALIAS, IDX, CAPTION, STYLE, TYPE, SM, BITMAP_NORMAL, BITMAP_HOT, BITMAP_DISABLED , TOOLTIP, SPF_USE_FLAG, PROGRAME_USE_FLAG, ACCECLERATOR) ")
						 _T(" VALUES (%s, '%c', '%s', '%s', '%s', '%s', %s, '%s', '%s', '%s', '%s', '%s', '%s', '%s' ,'%s', '%s', '%s', '%s');"),
						 strPK, 'U', user_id, sAlias, MENU_ALIAS, GROUP_ALIAS, sIdx, sCaption, sStyle, sType, sSM, sBITMAP_NORMAL, sBITMAP_HOT, sBITMAP_DISABLED, sTOOLTIP, sSPF_USE_FLAG, sPROGRAME_USE_FLAG, sACCECLERATOR);
			ids = m_pDM->AddFrame(query);
		}
	}

	// KOL.UDF.022
	//if (pDM) {
	if (&pDM != NULL) {
		delete pDM;
		pDM = NULL;
	}
	ids = m_pDM->SendFrame();
	m_pDM->EndFrame();
	if (ids < 0) AfxMessageBox(_T("Menu를 입력하던중 에러가 발생하였습니다."));
		else AfxMessageBox(_T("입력되었습니다."));
	return 0;
}

VOID CAccessControlToolBarDlg::OnbMODIFY() 
{
	INT nRow = m_pDM->GetRowCount();
	if (nRow == 0) return;
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	CString pk = m_pDM->GetCellData(row-1, 0);				
	if (pk.GetLength() == 0) {
		AfxMessageBox(_T("손상된 데이타 입니다."));
		return;
	}

	CAccessControlToolBarInsertDlg dlg;
	dlg.MODE = -1;			// 수정모드
	dlg.m_sAlias = m_pDM->GetCellData(row-1, 1);
	dlg.m_iIdx = _ttoi(m_pDM->GetCellData(row-1, 3));
	dlg.m_sCaption = m_pDM->GetCellData(row-1, 4);
	if (m_pDM->GetCellData(row-1, 5).GetLength() > 0) {
		dlg.m_cSTYLE = m_pDM->GetCellData(row-1, 5).GetAt(0);
	} else {
		dlg.m_cSTYLE = ' ';
	}
	if (m_pDM->GetCellData(row-1, 6).GetLength() > 0) {
		dlg.m_cTYPE = m_pDM->GetCellData(row-1, 6).GetAt(0);
	} else {
		dlg.m_cTYPE = ' ';
	}
	dlg.m_sSM = m_pDM->GetCellData(row-1, 7);
	dlg.m_sNormal = m_pDM->GetCellData(row-1, 8);
	dlg.m_sHot = m_pDM->GetCellData(row-1, 9);
	dlg.m_sDisabled = m_pDM->GetCellData(row-1, 10);
	dlg.m_ToolTip = m_pDM->GetCellData(row-1, 11);
	dlg.m_strACC = m_pDM->GetCellData(row-1, 12);
	
	if (dlg.DoModal() != IDOK) return;
	CString idx, style, type;
	style.Format(_T("%c"), dlg.m_cSTYLE);
	type.Format(_T("%c"), dlg.m_cTYPE);
	idx.Format(_T("%d"), dlg.m_iIdx);

	INT ids;
	ids = m_pDM->StartFrame();

	if (m_pDM->m_dbkind == 2) {
		dlg.m_sNormal.Replace(_T("'"), _T("''"));
		dlg.m_sNormal.Replace(_T("\\"), _T("\\\\"));
		dlg.m_sHot.Replace(_T("'"), _T("''"));
		dlg.m_sHot.Replace(_T("\\"), _T("\\\\"));
		dlg.m_sDisabled.Replace(_T("'"), _T("''"));
		dlg.m_sDisabled.Replace(_T("\\"), _T("\\\\"));
	}
	m_pDM->InitDBFieldData();
	ids = m_pDM->AddDBFieldData(_T("IDX")				, _T("NUMERIC")	, idx						, 1);
	ids = m_pDM->AddDBFieldData(_T("CAPTION")			, _T("STRING")	, dlg.m_sCaption			, 1);
	ids = m_pDM->AddDBFieldData(_T("STYLE")				, _T("STRING")	, style						, 1);
	ids = m_pDM->AddDBFieldData(_T("TYPE")				, _T("STRING")	, type						, 1);
	ids = m_pDM->AddDBFieldData(_T("SM")				, _T("STRING")	, dlg.m_sSM					, 1);
	ids = m_pDM->AddDBFieldData(_T("BITMAP_NORMAL")		, _T("STRING")	, dlg.m_sNormal				, 1);
	ids = m_pDM->AddDBFieldData(_T("BITMAP_HOT")		, _T("STRING")	, dlg.m_sHot				, 1);
	ids = m_pDM->AddDBFieldData(_T("BITMAP_DISABLED")	, _T("STRING")	, dlg.m_sDisabled			, 1);
	ids = m_pDM->AddDBFieldData(_T("TOOLTIP")			, _T("STRING")	, dlg.m_ToolTip				, 1);
	ids = m_pDM->AddDBFieldData(_T("ACCECLERATOR")		, _T("STRING")	, dlg.m_strACC				, 1);
	ids = m_pDM->MakeUpdateFrame(_T("ESL_TOOLBAR_TBL"), _T("PK"), _T("NUMERIC"), pk);

	// Method일 경우는 해당 메소드를 사용하는 모든 사용자에게도 적용해준다.
	CString query;
	if (FLAG == 'S') {
		query.Format(_T("UPDATE ESL_TOOLBAR_TBL SET ")
					 _T("IDX=%d, ")
					 _T("CAPTION='%s', STYLE='%s', ")
					 _T("TYPE='%s', SM='%s', ")
					 _T("BITMAP_NORMAL='%s', BITMAP_HOT='%s', ")
					 _T("BITMAP_DISABLED='%s' ,")
					 _T("TOOLTIP='%s' ,")
					 _T("ACCECLERATOR='%s' ")
					 _T("WHERE MENU_ALIAS='%s' AND TOOLGROUP_ALIAS='%s' AND ALIAS='%s';"),
					 dlg.m_iIdx,
					 dlg.m_sCaption, style,
					 type, dlg.m_sSM,
					 dlg.m_sNormal, dlg.m_sHot,
					 dlg.m_sDisabled,dlg.m_ToolTip,
					 dlg.m_strACC,
					 MENU_ALIAS, GROUP_ALIAS, dlg.m_sAlias 
		);			 			 
		m_pDM->AddFrame(query);
	}
	if (FLAG == 'M') {
		INT nRow = pUserList->GetCount();
		CString user_id;
		for (INT i = 0; i < nRow; i++) {
			user_id = pUserList->GetAt(i);
			if (user_id.GetLength() == 0) continue;
			query.Format(_T("UPDATE ESL_TOOLBAR_TBL SET ")
						 _T("IDX=%d, ")
						 _T("CAPTION='%s', STYLE='%s', ")
						 _T("TYPE='%s', SM='%s', ")
						 _T("BITMAP_NORMAL='%s', BITMAP_HOT='%s', ")
						 _T("BITMAP_DISABLED='%s' ,")
						 _T("TOOLTIP='%s' ")
						 _T("ACCECLERATOR='%s' ,")
						 _T("WHERE FLAG='U' AND USER_ID='%s' AND ALIAS='%s' AND MENU_ALIAS='%s' AND TOOLGROUP_ALIAS='%s';"),
						 dlg.m_iIdx,
						 dlg.m_sCaption, style,
						 type, dlg.m_sSM,
						 dlg.m_sNormal, dlg.m_sHot,
						 dlg.m_sDisabled,dlg.m_ToolTip,
						 dlg.m_strACC,
						 user_id, dlg.m_sAlias, MENU_ALIAS, GROUP_ALIAS 
			);			 			 
			m_pDM->AddFrame(query);
		}
	}

	ids = m_pDM->SendFrame();
	m_pDM->EndFrame();
	if (ids < 0) AfxMessageBox(_T("ToolBar Button을 수정하던중 에러가 발생하였습니다."));
		else AfxMessageBox(_T("수정되었습니다."));

	ShowToolBarList();
}

VOID CAccessControlToolBarDlg::OnbDELETE() 
{
	INT nRow = m_pDM->GetRowCount();
	if (nRow == 0) return;
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	CString pk = m_pDM->GetCellData(row-1, 0);				
	if (pk.GetLength() == 0) {
		AfxMessageBox(_T("손상된 데이타 입니다."));
		return;
	}
	CString alias = m_pDM->GetCellData(row-1, 1);
	if (AfxMessageBox(_T("정말 지우시겠습니까?"), MB_YESNO) != IDYES) return;
	INT ids;
	ids = m_pDM->StartFrame();

	m_pDM->InitDBFieldData();
	ids = m_pDM->MakeDeleteFrame(_T("ESL_TOOLBAR_TBL"), _T("PK"), _T("NUMERIC"), pk);
	// Method일 경우는 해당 메소드를 사용하는 모든 사용자에게도 적용해준다.
	CString query;
	if (FLAG == 'S') {
		query.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE ALIAS='%s' AND MENU_ALIAS='%s' AND TOOLGROUP_ALIAS='%s';"), alias, MENU_ALIAS, GROUP_ALIAS);
		m_pDM->AddFrame(query);
	}

	if (FLAG == 'M') {
		INT nRow = pUserList->GetCount();
		CString user_id;
		for (INT i = 0; i < nRow; i++) {
			user_id = pUserList->GetAt(i);
			if (user_id.GetLength() == 0) continue;
			query.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE FLAG='U' AND USER_ID='%s' AND ALIAS='%s' AND MENU_ALIAS='%s' AND TOOLGROUP_ALIAS='%s';"), user_id, alias, MENU_ALIAS, GROUP_ALIAS);
			m_pDM->AddFrame(query);
		}
	}

	ids = m_pDM->SendFrame();
	m_pDM->EndFrame();
	if (ids < 0) AfxMessageBox(_T("ToolBar Button을 삭제하던중 에러가 발생하였습니다."));
		else AfxMessageBox(_T("삭제되었습니다."));

	ShowToolBarList();	
}

VOID CAccessControlToolBarDlg::OnbPREVIEW() 
{
	FreeToolBar();
	m_pToolbar = new CToolBarCtrl;

	RECT r;
	GetClientRect(&r);
	r.bottom = r.top + 64;
	r.left = 10;
	r.right -= 10;
	m_pToolbar->Create(WS_CHILD | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC , r, this, 30000);

	// Toolbar 구성하기
	CSize s;
	s.cx = 48;
	s.cy = 48;
	m_pToolbar->SetButtonSize(s);

	pNormalList = new CImageList;
	pNormalList->Create(48, 48, ILC_COLOR24, 0, 4);
	pHotList = new CImageList;
	pHotList->Create(48, 48, ILC_COLOR24, 0, 4);
	pDisabledList = new CImageList;
	pDisabledList->Create(48, 48, ILC_COLOR24, 0, 4);

	INT nRow = m_pDM->GetRowCount();
	if (nRow == 0) return;
	CString NormalPath, HotPath, DisabledPath;
	TBBUTTON button;
	INT count = 0;
	for (INT i = 0; i < nRow; i ++) {
		button.iBitmap = NULL;
		NormalPath = _T("");
		HotPath = _T("");
		DisabledPath = _T("");
		if (m_pDM->GetCellData(i, 5) == _T("S")) {
			button.idCommand = 0;
			button.fsStyle = TBSTYLE_SEP;
			button.fsState = TBSTATE_ENABLED;
			button.iBitmap = NULL;
		} else {
			button.idCommand = WM_USER+32101+i;
			button.fsStyle = TBSTYLE_BUTTON;
			button.fsState = TBSTATE_ENABLED;
			button.iBitmap = count;
			// Normal bitmap
			if (m_pDM->GetCellData(i, 8).GetLength() > 0) {
				bmNormal[count].DeleteObject();
				NormalPath.Format(_T("..\\Bmp\\Toolbar\\%s"), m_pDM->GetCellData(i, 8));
				bmNormal[count].Attach(::LoadImage(NULL,NormalPath, IMAGE_BITMAP,0,0,LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
			} else {
				bmNormal[count].DeleteObject();
			}
			// Hot bitmap
			if (m_pDM->GetCellData(i, 9).GetLength() > 0) {
				bmHot[count].DeleteObject();
				HotPath.Format(_T("..\\Bmp\\Toolbar\\%s"), m_pDM->GetCellData(i, 9));
				bmHot[count].Attach(::LoadImage(NULL,HotPath, IMAGE_BITMAP,0,0,LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
			} else {
				bmHot[count].DeleteObject();
			}
			// Disabled bitmap
			if (m_pDM->GetCellData(i, 10).GetLength() > 0) {
				bmDisabled[count].DeleteObject();
				DisabledPath.Format(_T("..\\Bmp\\Toolbar\\%s"), m_pDM->GetCellData(i, 10));
				bmDisabled[count].Attach(::LoadImage(NULL,DisabledPath, IMAGE_BITMAP,0,0,LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
			} else {
				bmDisabled[count].DeleteObject();
			}
			
			pNormalList->Add(&bmNormal[count], RGB(0, 0, 0));
			pHotList->Add(&bmHot[count], RGB(0, 0, 0));
			pDisabledList->Add(&bmDisabled[count], RGB(0, 0, 0));

			count += 1;
		}
		m_pToolbar->AddButtons(1, &button);
	}
	m_pToolbar->SetImageList(pNormalList);
	m_pToolbar->SetHotImageList(pHotList);
	m_pToolbar->SetDisabledImageList(pDisabledList);
	m_pToolbar->ShowWindow(true);
	
}

BEGIN_EVENTSINK_MAP(CAccessControlToolBarDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAccessControlToolBarDlg)
	ON_EVENT(CAccessControlToolBarDlg, IDC_MSHFLEXGRID1, -605 /* MouseDown */, OnMouseDownMshflexgrid1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CAccessControlToolBarDlg, IDC_MSHFLEXGRID2, -605 /* MouseDown */, OnMouseDownMshflexgrid2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CAccessControlToolBarDlg, IDC_MSHFLEXGRID2, -601 /* DblClick */, OnDblClickMshflexgrid2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CAccessControlToolBarDlg::OnMouseDownMshflexgrid1(short Button, short Shift, long x, long y) 
{
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		//m_pGrid->SetReverse(row-1);
	}
}

INT CAccessControlToolBarDlg::FreeToolBar(VOID)
{
	if (m_pToolbar) {
		delete m_pToolbar;
		m_pToolbar = NULL;
	}
	if (pHotList) {
		delete pHotList;
		pHotList = NULL;
	}
	if (pNormalList) {
		delete pNormalList;
		pNormalList = NULL;
	}
	if (pDisabledList) {
		delete pDisabledList;
		pDisabledList = NULL;
	}

	return 0;
}

VOID CAccessControlToolBarDlg::OnbGropuINSERT() 
{
	// TODO: Add your control notification handler code here
	CAccessControlToolGroupInsertDlg dlg;
	dlg.MODE = 1;			// 입력모드

	if (dlg.DoModal() != IDOK) return;


	CString query;
	INT ids;
	ids = m_pToolDM->StartFrame();
	CString strPK;
	if (m_pToolDM->m_dbkind == 2) m_pToolDM->MakeRecKey(strPK, SEQUENCE);
	if (m_pToolDM->m_dbkind == 1) strPK.Format(_T("%s.NEXTVAL"), SEQUENCE);
	query.Format(_T("INSERT INTO ESL_TOOLGROUP_TBL (PK, ALIAS, MENU_ALIAS, DESCRIPT, TOOLGROUP_ALIGN) ")
				 _T(" VALUES (%s, '%s', '%s', '%s', '%c');"),
				 strPK, dlg.m_GroupAlias, MENU_ALIAS, dlg.m_GroupDesc, dlg.m_eGroupAlign);
	ids = m_pToolDM->AddFrame(query);


	ids = m_pToolDM->SendFrame();
	m_pToolDM->EndFrame();	
	if (ids < 0) AfxMessageBox(_T("Tool Group을 입력하던중 에러가 발생하였습니다."));
		else AfxMessageBox(_T("입력되었습니다."));
	ShowToolBarGroup();

}

VOID CAccessControlToolBarDlg::OnMouseDownMshflexgrid2(short Button, short Shift, long x, long y) 
{
	// TODO: Add your control notification handler code here
	INT row = m_pGrid_Tool->GetRow();
	if (row == 0) return;
	if (row > 0) {
		//m_pGrid_Tool->SetReverse(row-1);
	}

}

VOID CAccessControlToolBarDlg::OnbGroupMODIFY() 
{
	// TODO: Add your control notification handler code here
	if( m_pToolDM->GetRowCount() == 0 ) return;

	INT row = m_pGrid_Tool->GetRow();
	if(row == 0 ) return;
	
	CString pk = m_pToolDM->GetCellData(row-1, 0);				
	
	if (pk.GetLength() == 0) {
		AfxMessageBox(_T("손상된 데이타 입니다."));
		return;
	}

	CAccessControlToolGroupInsertDlg dlg;

	dlg.m_GroupAlias = m_pToolDM->GetCellData(row-1, 1); 
	dlg.m_GroupDesc = m_pToolDM->GetCellData(row-1, 3);
	CString strData = m_pToolDM->GetCellData(row-1, 4);
	strData.TrimLeft();		strData.TrimRight();		strData.MakeUpper();
	if (strData.GetLength() == 0) strData = _T("T");
	dlg.m_eGroupAlign = strData.GetAt(0);
	if (dlg.DoModal() != IDOK) return;

	INT ids;
	ids = m_pToolDM->StartFrame();

	m_pToolDM->InitDBFieldData();
	ids = m_pToolDM->AddDBFieldData(_T("ALIAS")				, _T("STRING")	, dlg.m_GroupAlias			, 1);
	ids = m_pToolDM->AddDBFieldData(_T("DESCRIPT")			, _T("STRING")	, dlg.m_GroupDesc			, 1);
	ids = m_pToolDM->AddDBFieldData(_T("TOOLGROUP_ALIGN")	, _T("STRING")	, dlg.m_eGroupAlign			, 1);
	ids = m_pToolDM->MakeUpdateFrame(_T("ESL_TOOLGROUP_TBL"), _T("PK")		, _T("NUMERIC")					, pk);

	// Method일 경우는 해당 메소드를 사용하는 모든 사용자에게도 적용해준다.
	CString query;

	query.Format(_T("update ESL_TOOLBAR_TBL set MENU_ALIAS = '%s' where MENU_ALIAS = '%s';"),dlg.m_GroupAlias,m_pToolDM->GetCellData(row-1, 1));

	m_pToolDM->AddFrame(query);


	ids = m_pToolDM->SendFrame();
	m_pToolDM->EndFrame();
	if (ids < 0) AfxMessageBox(_T("ToolBar Group을 수정하던중 에러가 발생하였습니다."));
		else AfxMessageBox(_T("수정되었습니다."));


	ShowToolBarGroup();

	GROUP_ALIAS = m_pToolDM->GetCellData(row-1, 1);
	ShowToolBarList();


}

VOID CAccessControlToolBarDlg::OnbGroupDELETE() 
{
	INT nRow = m_pToolDM->GetRowCount();
	if (nRow == 0) return;
	INT row = m_pGrid_Tool->GetRow();
	if (row == 0) return;
	CString pk = m_pToolDM->GetCellData(row-1, 0);				
	if (pk.GetLength() == 0) {
		AfxMessageBox(_T("손상된 데이타 입니다."));
		return;
	}

	CString alias = m_pToolDM->GetCellData(row-1, 1);
	if (AfxMessageBox(_T("정말 지우시겠습니까?"), MB_YESNO) != IDYES) return;
	INT ids;
	ids = m_pToolDM->StartFrame();

	m_pToolDM->InitDBFieldData();
	ids = m_pToolDM->MakeDeleteFrame(_T("ESL_TOOLGROUP_TBL"), _T("PK"), _T("NUMERIC"), pk);
	// Method일 경우는 해당 메소드를 사용하는 모든 사용자에게도 적용해준다.
	CString query;

	query.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE MENU_ALIAS = '%s';"), alias);
	m_pToolDM->AddFrame(query);
	
	ids = m_pToolDM->SendFrame();
	m_pToolDM->EndFrame();
	if (ids < 0) AfxMessageBox(_T("ToolBar Group을 삭제하던중 에러가 발생하였습니다."));
		else AfxMessageBox(_T("삭제되었습니다."));


	ShowToolBarGroup();
	InitGrid();	
}

VOID CAccessControlToolBarDlg::OnDblClickMshflexgrid2() 
{

	if( m_pToolDM->GetRowCount() == 0 ) return;

	INT row = m_pGrid_Tool->GetRow();

	GROUP_ALIAS = m_pToolDM->GetCellData(row-1, 1);
	ShowToolBarList();
}

VOID CAccessControlToolBarDlg::OnbREARRANGE() 
{
	INT nRows = m_pDM->GetRowCount();
	if (nRows == 0) return;
	if (AfxMessageBox(_T("정말 재정렬 하시겠습니까?"), MB_YESNO) != IDYES) return;
	INT ids;
	ids = m_pDM->StartFrame();
	CString strQuery;
	for (INT i = 0; i < nRows; i++) {
		CString strPK = _T("");
		strPK = m_pDM->GetCellData(i, 0);
		strPK.TrimLeft();			strPK.TrimRight();
		if (strPK.GetLength() == 0) continue;

		strQuery.Format(_T("UPDATE ESL_TOOLBAR_TBL SET IDX=%d WHERE PK=%s;"), i, strPK);
		m_pDM->AddFrame(strQuery);
	}
	ids = m_pDM->SendFrame();
	m_pDM->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("재정렬 도중 에러가 발생하였습니다."));
	} else {
		ShowToolBarList();
		AfxMessageBox(_T("재정렬 하였습니다."));
	}
}
