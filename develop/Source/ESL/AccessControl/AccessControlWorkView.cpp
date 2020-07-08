// AccessControlWorkView.cpp : implementation file
//

#include "stdafx.h"
#include "AccessControl.h"
#include "AccessControlWorkView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MainFrm.h"
#include "AccessControlMenuInsertDlg.h"
#include "AccessControlToolBarDlg.h"
#include "AccessControlSelectionListDlg.h"
#include "AccessControlSelectionMenuListDlg.h"
#include "AccessControlSelectionMenuModifyDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAccessControlWorkView

IMPLEMENT_DYNCREATE(CAccessControlWorkView, CFormView)

CAccessControlWorkView::CAccessControlWorkView()
	: CFormView(CAccessControlWorkView::IDD)
{
	//{{AFX_DATA_INIT(CAccessControlWorkView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGrid = new CESL_Grid(NULL);

	m_pMenuDM = new CESL_DataMgr;

	pUserList = NULL;
	m_cTOPMENUFLAG = ' ';
}

CAccessControlWorkView::~CAccessControlWorkView()
{
	if (m_pGrid) {
		delete m_pGrid;
		m_pGrid = NULL;
	}

	if (m_pMenuDM) {
		delete m_pMenuDM;
		m_pMenuDM = NULL;
	}

	FreeMenuList();
}

VOID CAccessControlWorkView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlWorkView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessControlWorkView, CFormView)
	//{{AFX_MSG_MAP(CAccessControlWorkView)
	ON_BN_CLICKED(IDC_bINSERT, OnbINSERT)
	ON_BN_CLICKED(IDC_bMODIFY, OnbMODIFY)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_BN_CLICKED(IDC_bTOOLBAR, OnbTOOLBAR)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bPREVIEW, OnbPREVIEW)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlWorkView diagnostics

#ifdef _DEBUG
VOID CAccessControlWorkView::AssertValid() const
{
	CFormView::AssertValid();
}

VOID CAccessControlWorkView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAccessControlWorkView message handlers

VOID CAccessControlWorkView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_pGrid->SubclassDlgItem(IDC_MSHFLEXGRID1, this);

	InitGrid();

	ReadCfgFile.ReadCfgFile(_T(""));
	m_pMenuDM->SetDB(ReadCfgFile.m_sDBKIND);
	m_pMenuDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
}

VOID CAccessControlWorkView::InitGrid(VOID)
{
	CMainFrame * pframe = (CMainFrame*)GetParentFrame();

	m_pGrid->RemoveAll();

	if(pframe->FLAG == 'S')
	{
		m_pGrid->SetCols(0,11);
		m_pGrid->SetColumnInfo(0-1, _T("")			, 30		,		7);
		m_pGrid->SetColumnInfo(1-1, _T("ALIAS")		, 100		,		1);
		m_pGrid->SetColumnInfo(2-1, _T("Idx")		, 30		,		7);
		m_pGrid->SetColumnInfo(3-1, _T("Caption")	, 120		,		1);
		m_pGrid->SetColumnInfo(4-1, _T("Visible")	, 50		,		4);
		m_pGrid->SetColumnInfo(5-1, _T("Style")		, 50		,		4);
		m_pGrid->SetColumnInfo(6-1, _T("Action")	, 50		,		4);
		m_pGrid->SetColumnInfo(7-1, _T("Target")	, 50		,		1);
		m_pGrid->SetColumnInfo(8-1, _T("SPF")		, 30		,		4);
		m_pGrid->SetColumnInfo(9-1, _T("PRO")		, 30		,		4);
		m_pGrid->SetColumnInfo(10-1, _T("TITLE")		, 50		,		1);

	}else
	{
		m_pGrid->SetCols(0,11);
		m_pGrid->SetColumnInfo(0-1, _T("")			, 30		,		7);
		m_pGrid->SetColumnInfo(1-1, _T("ALIAS")		, 100		,		1);
		m_pGrid->SetColumnInfo(2-1, _T("Idx")		, 30		,		7);
		m_pGrid->SetColumnInfo(3-1, _T("Caption")	, 120		,		1);
		m_pGrid->SetColumnInfo(4-1, _T("Visible")	, 50		,		4);
		m_pGrid->SetColumnInfo(5-1, _T("Style")		, 50		,		4);
		m_pGrid->SetColumnInfo(6-1, _T("Action")	, 50		,		4);
		m_pGrid->SetColumnInfo(7-1, _T("Target")	, 50		,		1);
		m_pGrid->SetColumnInfo(8-1, _T("SPF")		, 30		,		4);
		m_pGrid->SetColumnInfo(9-1, _T("PRO")		, 30		,		4);
		m_pGrid->SetColumnInfo(10-1, _T("TITLE")		, 50		,		1);
	}

}

VOID CAccessControlWorkView::ShowMenuList(VOID)
{
	CMainFrame * pframe = (CMainFrame*)GetParentFrame();

	GetDlgItem(IDC_STATIC1)->SetWindowText(MenuGroupAlias);
	CString query;

	if(pframe->FLAG == 'S')
	{
		//                    0    1         2        3      4        5       6       7      8          9                10           11     
		query.Format(_T("SELECT PK, ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET ,SPF_USE_FLAG ,PROGRAME_USE_FLAG, TITLE_DESC FROM ESL_MENU_TBL WHERE GROUP_ALIAS='%s' AND USER_ID='Administrator'  ORDER BY IDX"), MenuGroupAlias);	
	}
	else
	{
		//                    0    1         2        3      4        5       6       7      8          9                10           11
		query.Format(_T("SELECT PK, ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET, SPF_USE_FLAG, PROGRAME_USE_FLAG, TITLE_DESC FROM ESL_MENU_TBL WHERE GROUP_ALIAS='%s' AND USER_ID='%s' AND FLAG='%c' AND SPF_USE_FLAG = 'y' AND PROGRAME_USE_FLAG ='y'  ORDER BY IDX"), MenuGroupAlias, pframe->NAME, pframe->FLAG);
	}

	m_pMenuDM->RestructDataManager(query);

	INT nRow = m_pMenuDM->GetRowCount();
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
		m_pGrid->SetTextMatrix(i+1, 1, m_pMenuDM->GetCellData(i, 1));
		m_pGrid->SetTextMatrix(i+1, 2, m_pMenuDM->GetCellData(i, 3));
		m_pGrid->SetTextMatrix(i+1, 3, m_pMenuDM->GetCellData(i, 4));
		m_pGrid->SetTextMatrix(i+1, 4, m_pMenuDM->GetCellData(i, 5));
		m_pGrid->SetTextMatrix(i+1, 5, m_pMenuDM->GetCellData(i, 6));
		m_pGrid->SetTextMatrix(i+1, 6, m_pMenuDM->GetCellData(i, 7));
		m_pGrid->SetTextMatrix(i+1, 7, m_pMenuDM->GetCellData(i, 8));
		m_pGrid->SetTextMatrix(i+1, 8, m_pMenuDM->GetCellData(i, 9));
		m_pGrid->SetTextMatrix(i+1, 9, m_pMenuDM->GetCellData(i, 10));
		m_pGrid->SetTextMatrix(i+1, 10, m_pMenuDM->GetCellData(i, 11));
	}
	return;
}

VOID CAccessControlWorkView::OnbINSERT() 
{
	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	if (m_cTOPMENUFLAG != 'T') {
		if (pframe->FLAG == 'S') {			// Admin 일경우는 입력을 받아서 입력을 해준다.
			INSERT_Administrator();
		} else {							// 메소드나 유져 모드인 경우
			INSERT_UserOrMethod();
		}
	} else {
		MENU_INSERT(pframe->FLAG);
	}
	ShowMenuList();	
}

INT CAccessControlWorkView::INSERT_Administrator(VOID)
{
	// Admin 일경우는 입력을 받아서 입력을 해준다.
	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	CAccessControlMenuInsertDlg dlg(this);
	dlg.NAME = pframe->NAME;
	dlg.FLAG = pframe->FLAG;
	dlg.MODE = 1;		// 입력 모드
	if (dlg.DoModal() != IDOK) return 0;
	CString query;
	INT ids;

	ids = m_pMenuDM->StartFrame();

	// 자기자신의 중복 데이타 삭제하고
	query.Format(_T("DELETE FROM ESL_MENU_TBL WHERE ALIAS='%s' AND GROUP_ALIAS='%s' AND USER_ID='%s' AND FLAG='%c';"), dlg.m_sAlias, MenuGroupAlias, pframe->NAME, pframe->FLAG);
	ids = m_pMenuDM->AddFrame(query);	

	/* Mysql 인 경우는 \ 를 \\로 바꿔준다. */
	if (m_pMenuDM->m_dbkind == 2) dlg.m_sTarget.Replace(_T("\\"), _T("\\\\"));

	CString strPK;
	if (m_pMenuDM->m_dbkind == 2) m_pMenuDM->MakeRecKey(strPK, ReadCfgFile.m_sSequence);
	if (m_pMenuDM->m_dbkind == 1) strPK.Format(_T("%s.NEXTVAL"), ReadCfgFile.m_sSequence);
	query.Format(_T("INSERT INTO ESL_MENU_TBL (PK, FLAG, USER_ID, ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET, TITLE_DESC) ")
				 _T("VALUES (%s, '%c', '%s', '%s', '%s', %d, '%s', '%c', '%c', '%c', '%s', '%s');"),
				 strPK, pframe->FLAG, pframe->NAME, dlg.m_sAlias, MenuGroupAlias, dlg.m_iIdx, dlg.m_sCaption, dlg.VISIBLE, dlg.STYLE, dlg.ACTION, dlg.m_sTarget, dlg.m_sTitle);
	ids = m_pMenuDM->AddFrame(query);
	ids = m_pMenuDM->SendFrame();
	if (ids < 0) AfxMessageBox(_T("Menu를 입력하던중 에러가 발생하였습니다."));
		else AfxMessageBox(_T("입력되었습니다."));

	ids = m_pMenuDM->EndFrame();

	return 0;
}

INT CAccessControlWorkView::INSERT_UserOrMethod(VOID)
{
	CAccessControlSelectionListDlg SelectDlg;
	SelectDlg.MODE = 1;		// Menu Select Mode

	CString query;

	CESL_DataMgr *pDM = new CESL_DataMgr;
	if (!pDM) return -2;
	pDM->SetDB(ReadCfgFile.m_sDBKIND);
	pDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	//                      0    1      2        3       4       5      6         7               8              9
	query.Format(_T("SELECT ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET, SPF_USE_FLAG, PROGRAME_USE_FLAG, TITLE_DESC FROM ESL_MENU_TBL WHERE GROUP_ALIAS='%s' AND USER_ID='Administrator' AND FLAG='S' ORDER BY IDX"), MenuGroupAlias);
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
	CString sVisible = pDM->GetCellData(SelectDlg.nSelectedRow, 3);
	sVisible.TrimLeft();		sVisible.TrimRight();
	CString sStyle = pDM->GetCellData(SelectDlg.nSelectedRow, 4);
	sStyle.TrimLeft();			sStyle.TrimRight();
	CString sAction = pDM->GetCellData(SelectDlg.nSelectedRow, 5);
	sAction.TrimLeft();			sAction.TrimRight();
	CString sTarget = pDM->GetCellData(SelectDlg.nSelectedRow, 6);
	sTarget.TrimLeft();			sTarget.TrimRight();
	CString sSPF_USE_FLAG = pDM->GetCellData(SelectDlg.nSelectedRow, 7);
	sSPF_USE_FLAG.TrimLeft();	sSPF_USE_FLAG.TrimRight();
	CString sPROGRAME_USE_FLAG = pDM->GetCellData(SelectDlg.nSelectedRow, 8);
	sPROGRAME_USE_FLAG.TrimLeft();			sPROGRAME_USE_FLAG.TrimRight();
	CString sTitle = pDM->GetCellData(SelectDlg.nSelectedRow, 9);
	sTitle.TrimLeft();			sTitle.TrimRight();

	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	// 입력된 데이타를 처리한다.
	CString user_id;
	INT ids;


	ids = m_pMenuDM->StartFrame();
	// 자기자신(메소드, 유져)의 중복 데이타 삭제하고
	query.Format(_T("DELETE FROM ESL_MENU_TBL WHERE ALIAS='%s' AND GROUP_ALIAS='%s' AND USER_ID='%s' AND FLAG='%c';"), sAlias, MenuGroupAlias, pframe->NAME, pframe->FLAG);
	ids = m_pMenuDM->AddFrame(query);

	// 메소드인경우 유져중에서 중복된 데이타를 미리 모두 삭제한다.
	if (pframe->FLAG == 'M') {
		// 해당 메소드를 사용하는 유져중에서 중복 데이타 삭제하고
		nRow = pUserList->GetCount();
		for (INT i = 0; i < nRow; i++) {
			user_id = pUserList->GetAt(i);
			if (user_id.GetLength() == 0) continue;
			query.Format(_T("DELETE FROM ESL_MENU_TBL WHERE ALIAS='%s' AND GROUP_ALIAS='%s' AND USER_ID='%s' AND FLAG='U';"), sAlias, MenuGroupAlias, user_id);
			ids = m_pMenuDM->AddFrame(query);
		}
	}

	// 해당 메소드(유져) 정보를 입력해주고
	CString strPK;
	if (m_pMenuDM->m_dbkind == 2) m_pMenuDM->MakeRecKey(strPK, ReadCfgFile.m_sSequence);
	if (m_pMenuDM->m_dbkind == 1) strPK.Format(_T("%s.NEXTVAL"), ReadCfgFile.m_sSequence);

	/* Mysql 인 경우는 \ 를 \\로 바꿔준다. */
	if (m_pMenuDM->m_dbkind == 2) sTarget.Replace(_T("\\"), _T("\\\\"));

	query.Format(_T("INSERT INTO ESL_MENU_TBL (PK, FLAG, USER_ID, ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET, SPF_USE_FLAG, PROGRAME_USE_FLAG, TITLE_DESC) ")
				 _T("VALUES (%s, '%c', '%s', '%s', '%s', %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');"),
				 strPK, pframe->FLAG, pframe->NAME, sAlias, MenuGroupAlias, _ttoi(sIdx), sCaption, sVisible, sStyle, sAction, sTarget, sSPF_USE_FLAG, sPROGRAME_USE_FLAG, sTitle);
	ids = m_pMenuDM->AddFrame(query);
	
	// 메소드일경우는 해당 메소드를 사용하는 모든 유져에도 입력해준다.
	if (pframe->FLAG == 'M' || pframe->FLAG == 'S') {
		nRow = pUserList->GetCount();
		for (INT i = 0; i < nRow; i++) {
			user_id = pUserList->GetAt(i);
			if (user_id.GetLength() == 0) continue;

			if (m_pMenuDM->m_dbkind == 2) m_pMenuDM->MakeRecKey(strPK, ReadCfgFile.m_sSequence);
			if (m_pMenuDM->m_dbkind == 1) strPK.Format(_T("%s.NEXTVAL"), ReadCfgFile.m_sSequence);

			/* Mysql 인 경우는 \ 를 \\로 바꿔준다. */
			if (m_pMenuDM->m_dbkind == 2) sTarget.Replace(_T("\\"), _T("\\\\"));

			query.Format(_T("INSERT INTO ESL_MENU_TBL (PK, FLAG, USER_ID, ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET, SPF_USE_FLAG, PROGRAME_USE_FLAG, TITLE_DESC) ")
						 _T("VALUES (%s, '%c', '%s', '%s', '%s', %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');"),
						 strPK, 'U', user_id, sAlias, MenuGroupAlias, _ttoi(sIdx), sCaption, sVisible, sStyle, sAction, sTarget, sSPF_USE_FLAG, sPROGRAME_USE_FLAG, sTitle);
			ids = m_pMenuDM->AddFrame(query);
		}
	}

	if (pDM) {
		delete pDM;
		pDM = NULL;
	}
	ids = m_pMenuDM->SendFrame();
	m_pMenuDM->EndFrame();
	if (ids < 0) AfxMessageBox(_T("Menu를 입력하던중 에러가 발생하였습니다."));
		else AfxMessageBox(_T("입력되었습니다."));
	return 0;
}

VOID CAccessControlWorkView::OnbMODIFY() 
{
	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	if (m_cTOPMENUFLAG == 'T') {
		MENU_MODIFY(pframe->FLAG);
		ShowMenuList();	
		return;
	}
	INT nRow = m_pMenuDM->GetRowCount();
	if (nRow == 0) return;
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	CString pk = m_pMenuDM->GetCellData(row-1, 0);				
	if (pk.GetLength() == 0) {
		AfxMessageBox(_T("손상된 데이타 입니다."));
		return;
	}
	CAccessControlMenuInsertDlg dlg(this);
	dlg.NAME = pframe->NAME;
	dlg.FLAG = pframe->FLAG;
	dlg.MODE = -1;		// 수정 모드
	// 수정전 사항 입력
	dlg.m_sAlias	=	m_pMenuDM->GetCellData(row-1, 1);				
	dlg.m_iIdx		=	_ttoi(m_pMenuDM->GetCellData(row-1, 3));
	dlg.m_sCaption	=	m_pMenuDM->GetCellData(row-1, 4);
	dlg.VISIBLE		=	m_pMenuDM->GetCellData(row-1, 5).GetAt(0);
	dlg.STYLE		=	m_pMenuDM->GetCellData(row-1, 6).GetAt(0);
	dlg.ACTION		=	m_pMenuDM->GetCellData(row-1, 7).GetAt(0);
	dlg.m_sTarget	=	m_pMenuDM->GetCellData(row-1, 8);
	dlg.m_sTitle	=	m_pMenuDM->GetCellData(row-1, 11);

	if (dlg.DoModal() != IDOK) return;
	CString visible, style, action, idx;
	visible.Format(_T("%c"), dlg.VISIBLE);
	style.Format(_T("%c"), dlg.STYLE);
	action.Format(_T("%c"), dlg.ACTION);
	idx.Format(_T("%d"), dlg.m_iIdx);

	if (m_pMenuDM->m_dbkind == 2) dlg.m_sTarget.Replace(_T("\\"), _T("\\\\"));

	INT ids,i;
	CString user_id;

	ids = m_pMenuDM->StartFrame();

	// 해당 메소드(유져)의 정보를 수정해주고~
	m_pMenuDM->InitDBFieldData();
	ids = m_pMenuDM->AddDBFieldData(_T("IDX")		, _T("NUMERIC")	, idx						, 1);
	ids = m_pMenuDM->AddDBFieldData(_T("CAPTION")	, _T("STRING")	, dlg.m_sCaption			, 1);
	ids = m_pMenuDM->AddDBFieldData(_T("VISIBLE")	, _T("STRING")	, visible					, 1);
	ids = m_pMenuDM->AddDBFieldData(_T("STYLE")		, _T("STRING")	, style						, 1);
	ids = m_pMenuDM->AddDBFieldData(_T("ACTION")	, _T("STRING")	, action					, 1);
	ids = m_pMenuDM->AddDBFieldData(_T("TARGET")	, _T("STRING")	, dlg.m_sTarget				, 1);
	ids = m_pMenuDM->AddDBFieldData(_T("TITLE_DESC")		, _T("STRING")	, dlg.m_sTitle				, 1);
	ids = m_pMenuDM->MakeUpdateFrame(_T("ESL_MENU_TBL"), _T("PK"), _T("NUMERIC"), pk);

	CString query;
	// 메소드일경우는 해당 메소드를 사용하는 모든 유져에도 수정해준다.
	if (pframe->FLAG == 'S') {
		query.Format(_T("UPDATE ESL_MENU_TBL SET ")
					 _T("IDX=%s, ")
					 _T("CAPTION='%s', ")
					 _T("VISIBLE='%s', ")
					 _T("STYLE='%s', ")
					 _T("ACTION='%s', ")
					 _T("TARGET='%s' ,")
					 _T("TITLE_DESC='%s' ")
					 _T("WHERE ALIAS='%s' AND GROUP_ALIAS='%s';"),
					 idx,
					 dlg.m_sCaption,
					 visible,
					 style,
					 action,
					 dlg.m_sTarget,
					 dlg.m_sTitle,
					 dlg.m_sAlias,
					 MenuGroupAlias
		);
		m_pMenuDM->AddFrame(query);
	}

	if (pframe->FLAG == 'M') {
		nRow = pUserList->GetCount();
		for ( i = 0; i < nRow; i++) {
			user_id = pUserList->GetAt(i);
			if (user_id.GetLength() == 0) continue;
			query.Format(_T("UPDATE ESL_MENU_TBL SET ")
						 _T("IDX=%s, ")
						 _T("CAPTION='%s', ")
						 _T("VISIBLE='%s', ")
						 _T("STYLE='%s', ")
						 _T("ACTION='%s', ")
						 _T("TARGET='%s', ")
						 _T("TITLE_DESC='%s' ")
						 _T("WHERE FLAG='U' AND USER_ID='%s' AND ALIAS='%s' AND GROUP_ALIAS='%s';"),
						 idx,
						 dlg.m_sCaption,
						 visible,
						 style,
						 action,
						 dlg.m_sTarget,
						 dlg.m_sTitle,
						 user_id, dlg.m_sAlias,
						 MenuGroupAlias
			);
			m_pMenuDM->AddFrame(query);
		}
	}

	ids = m_pMenuDM->SendFrame();
	m_pMenuDM->EndFrame();
	if (ids < 0) AfxMessageBox(_T("Menu를 수정하던중 에러가 발생하였습니다."));
		else AfxMessageBox(_T("수정되었습니다."));

	ShowMenuList();		
}

VOID CAccessControlWorkView::OnbDELETE() 
{
	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	if (m_cTOPMENUFLAG == 'T') {
		MENU_DELETE(pframe->FLAG);
		ShowMenuList();	
		return;
	}
	INT nRow = m_pMenuDM->GetRowCount();
	if (nRow == 0) return;
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	CString pk = m_pMenuDM->GetCellData(row-1, 0);				
	if (pk.GetLength() == 0) {
		AfxMessageBox(_T("손상된 데이타 입니다."));
		return;
	}
	CString alias = m_pMenuDM->GetCellData(row-1, 1);				
	if (AfxMessageBox(_T("정말 지우시겠습니까?"), MB_YESNO) != IDYES) return;

	INT ids;
	ids = m_pMenuDM->StartFrame();
	
	// 해당 메소드(유져)의 정보를 삭제해주고~
	ids = m_pMenuDM->MakeDeleteFrame(_T("ESL_MENU_TBL"), _T("PK"), _T("NUMERIC"), pk);
	CString query;
	// 메소드일경우는 해당 메소드를 사용하는 모든 유져에도 삭제해준다.
	if (pframe->FLAG == 'S') {
		query.Format(_T("DELETE FROM ESL_MENU_TBL WHERE ALIAS='%s' AND GROUP_ALIAS='%s';"), alias, MenuGroupAlias);
		m_pMenuDM->AddFrame(query);
	}
	if (pframe->FLAG == 'M') {
		INT nRow = pUserList->GetCount();
		CString user_id;
		for (INT i = 0; i < nRow; i++) {
			user_id = pUserList->GetAt(i);
			if (user_id.GetLength() == 0) continue;
			query.Format(_T("DELETE FROM ESL_MENU_TBL WHERE FLAG='U' AND USER_ID='%s' AND ALIAS='%s' AND GROUP_ALIAD='%s';"), user_id, alias, MenuGroupAlias);
			m_pMenuDM->AddFrame(query);
		}
	}

	ids = m_pMenuDM->SendFrame();
	m_pMenuDM->EndFrame();
	if (ids < 0) AfxMessageBox(_T("Menu를 삭제하던중 에러가 발생하였습니다."));
		else AfxMessageBox(_T("삭제되었습니다."));

	ShowMenuList();		
}

VOID CAccessControlWorkView::OnbTOOLBAR() 
{
	if (m_cTOPMENUFLAG == 'T') {
		AfxMessageBox(_T("TOP MENU의 항목은 TOOLBAR를 가지지 않습니다."));
		return;
	}
	INT nRow = m_pMenuDM->GetRowCount();
	if (nRow == 0) return;
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	CString action = m_pMenuDM->GetCellData(row-1, 7);				 
	if (action != _T("D")) {
		AfxMessageBox(_T("Action 이 Load DLL인 Menu만이 ToolBar를 가질 수 있습니다."));
		return;
	}
	CString alias = m_pMenuDM->GetCellData(row-1, 1);				
	if (alias.GetLength() == 0) {
		AfxMessageBox(_T("손상된 데이타 입니다."));
		return;
	}

	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	CAccessControlToolBarDlg dlg;
	dlg.pUserList = pUserList;
	if (!dlg.pUserList) return;
	dlg.MENU_ALIAS = alias;
	dlg.FLAG = pframe->FLAG;
	dlg.USER_ID = pframe->NAME;
	dlg.SEQUENCE = ReadCfgFile.m_sSequence;
	dlg.DoModal();
}

BEGIN_EVENTSINK_MAP(CAccessControlWorkView, CFormView)
    //{{AFX_EVENTSINK_MAP(CAccessControlWorkView)
	ON_EVENT(CAccessControlWorkView, IDC_MSHFLEXGRID1, -605 /* MouseDown */, OnMouseDownMshflexgrid1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CAccessControlWorkView::OnMouseDownMshflexgrid1(short Button, short Shift, long x, long y) 
{
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		m_pGrid->SetReverse(row-1);
	}
}

VOID CAccessControlWorkView::OnSize(UINT nType, INT cx, INT cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	RECT rect, r;
	GetClientRect(&rect);
	CWnd *pWnd = NULL;

	pWnd = GetDlgItem(IDC_STATIC_BACK);
	r = rect;
	if (pWnd) pWnd->MoveWindow(&r, true);

	INT nButtonHeight = 25;
	INT nButtonWidth = 80;

	pWnd = GetDlgItem(IDC_STATIC1);
	r = rect;
	r.left += 5;
	r.right -= 5;
	r.top = r.top + 5;
	r.bottom = r.top + 15;
	if (pWnd) pWnd->MoveWindow(&r, true);

	pWnd = GetDlgItem(IDC_MSHFLEXGRID1);
	r = rect;
	r.left += 5;
	r.right -= 5;
	r.top = r.top + 20;
	r.bottom = r.bottom - nButtonHeight-10;
	if (pWnd) pWnd->MoveWindow(&r, true);

	pWnd = GetDlgItem(IDC_bINSERT);
	r = rect;
	r.left += 5;
	r.right = r.left + nButtonWidth;
	r.top = r.bottom - nButtonHeight-5;
	r.bottom = r.top + nButtonHeight;
	if (pWnd) pWnd->MoveWindow(&r, true);

	pWnd = GetDlgItem(IDC_bMODIFY);
	r.left = r.right + 10;
	r.right = r.left + nButtonWidth;
	if (pWnd) pWnd->MoveWindow(&r, true);

	pWnd = GetDlgItem(IDC_bDELETE);
	r.left = r.right + 10;
	r.right = r.left + nButtonWidth;
	if (pWnd) pWnd->MoveWindow(&r, true);

	pWnd = GetDlgItem(IDC_bTOOLBAR);
	r.left = r.right + 10;
	r.right = r.left + nButtonWidth;
	if (pWnd) pWnd->MoveWindow(&r, true);

	pWnd = GetDlgItem(IDC_bPREVIEW);
	r.left = r.right + 10;
	r.right = r.left + nButtonWidth;
	if (pWnd) pWnd->MoveWindow(&r, true);
}

VOID CAccessControlWorkView::OnbPREVIEW() 
{
	if (m_cTOPMENUFLAG == 'T') {
		AfxMessageBox(_T("TOP MENU의 항목은 미리보기를 할 수 없습니다."));
		return;
	}
	CESL_DataMgr *pDM = new CESL_DataMgr;
	pDM->SetDB(ReadCfgFile.m_sDBKIND);
	pDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	CMainFrame * pframe = (CMainFrame*)GetParentFrame();

	INT ids;
	CString query;
	query.Format(_T("SELECT PK, ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET FROM ESL_MENU_TBL WHERE USER_ID='%s' AND FLAG='%c' ORDER BY IDX"), pframe->NAME, pframe->FLAG);
	ids = pDM->RestructDataManager(query);
	if (ids < 0) {
		AfxMessageBox(_T("DB에서 Menu 데이타를 읽던중 에러가 발생하였습니다."));
		return;
	}
	FreeMenuList();
	CMenu *pMenu = MakePopupMenu(pDM, MenuGroupAlias);
	if (!pMenu) {
		AfxMessageBox(_T("메뉴를 구성하던중 에러가 발생하였습니다."));
		return;
	}
	if (pDM) {
		delete pDM;
		pDM = NULL;
	}
	POINT p;
	GetCursorPos(&p);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN,p.x, p.y,this);
}

CMenu * CAccessControlWorkView::MakePopupMenu(CESL_DataMgr *pDM, CString group_alias)
{
	CMenu *pMenu = new CMenu;
	if (!pMenu) return NULL;
	pMenu->CreatePopupMenu();
	INT nRow = pDM->GetRowCount();

	CString alias;
	CMenu *pSubMenu;
	INT UNIQUEID;
	for (INT i = 0; i < nRow; i++) {
		pSubMenu = NULL;
		if (pDM->GetCellData(i, 2) != group_alias) continue;
		alias = pDM->GetCellData(i, 1);
		if (pDM->GetCellData(i, 5) == _T("F")) continue;		// visible가 false일 경우 걍 넘어감
		if (pDM->GetCellData(i, 6) == _T("P")) {
			pMenu->AppendMenu(MF_SEPARATOR);
			continue;
		}
		if (pDM->GetCellData(i, 7) == _T("P")) {
			pSubMenu = MakePopupMenu(pDM, pDM->GetCellData(i, 8));
			pMenu->AppendMenu(MF_POPUP, (UINT)pSubMenu->m_hMenu, pDM->GetCellData(i, 4));
		} else {
			UNIQUEID = UniqueMsgList.AddMsg(pDM->GetCellData(i, 8));
			pMenu->AppendMenu(MF_STRING, UNIQUEID, pDM->GetCellData(i, 4));
		}
		
	}
	MenuList.AddTail((CObject*)pMenu);
	return pMenu;
}

INT CAccessControlWorkView::FreeMenuList(VOID)
{
	INT count = MenuList.GetCount();
	if (count == 0) return 0;
	CMenu *pData;
	for (INT i = 0; i < count; i++) {
		pData = NULL;
		pData = (CMenu*)MenuList.RemoveHead();
		if (pData) delete pData;
	}
	MenuList.RemoveAll();

	UniqueMsgList.FreeData();
	return 0;
}

BOOL CAccessControlWorkView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_COMMAND ) {
		INT msg_id = pMsg->wParam;
		if (msg_id >= 32101 && msg_id <= 32398) {
			AfxMessageBox(UniqueMsgList.GetMsg(msg_id), MB_ICONINFORMATION);
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}

INT CAccessControlWorkView::SetReadOnly(BOOL bFlag)
{
	GetDlgItem(IDC_MSHFLEXGRID1)->EnableWindow(!bFlag);
	GetDlgItem(IDC_bINSERT)->EnableWindow(!bFlag);
	GetDlgItem(IDC_bMODIFY)->EnableWindow(!bFlag);
	GetDlgItem(IDC_bDELETE)->EnableWindow(!bFlag);
	GetDlgItem(IDC_bPREVIEW)->EnableWindow(!bFlag);
	GetDlgItem(IDC_bTOOLBAR)->EnableWindow(!bFlag);
	return 0;
}

INT CAccessControlWorkView::MENU_INSERT(TCHAR cUSERFLAG)
{
	CAccessControlSelectionMenuListDlg dlg(this);
	dlg.m_strTOPMENUALIAS = MenuGroupAlias;
	dlg.m_cUSERFLAG = cUSERFLAG;
	if (dlg.DoModal() != IDOK) return 0;
	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	
	INT ids, nIdx;
	CString strQuery;
	ids = m_pMenuDM->StartFrame();

	CString strPK;
	if (m_pMenuDM->m_dbkind == 2) m_pMenuDM->MakeRecKey(strPK, ReadCfgFile.m_sSequence);
	if (m_pMenuDM->m_dbkind == 1) strPK.Format(_T("%s.NEXTVAL"), ReadCfgFile.m_sSequence);

	nIdx = GetMaxIdx();
	strQuery.Format(_T("INSERT INTO ESL_MENU_TBL (PK, FLAG, USER_ID, ALIAS, GROUP_ALIAS, IDX, CAPTION) ")
				 _T("VALUES (%s, '%c', '%s', '%s', '%s', %d, '%s');"),
				 strPK, pframe->FLAG, pframe->NAME, dlg.m_strALIAS, MenuGroupAlias, nIdx, dlg.m_strCAPTION);
	ids = m_pMenuDM->AddFrame(strQuery);
	ids = m_pMenuDM->SendFrame();
	if (ids < 0) AfxMessageBox(_T("Menu를 입력하던중 에러가 발생하였습니다."));
		else AfxMessageBox(_T("입력되었습니다."));
	ids = m_pMenuDM->EndFrame();
	return 0;
}

INT CAccessControlWorkView::GetMaxIdx(VOID)
{
	INT nRows = m_pMenuDM->GetRowCount();
	if (nRows == 0) return 0;

	INT nMaxIdx = -9999;
	INT nTmpIdx;
	CString strTmp;
	for (INT i = 0; i < nRows; i++) {
		nTmpIdx = -9999;
		strTmp = _T("");
		strTmp = m_pMenuDM->GetCellData(i, 3);
		if (strTmp.GetLength() == 0) nTmpIdx = -9999;
			else nTmpIdx = _ttoi(strTmp);

		if (nMaxIdx < nTmpIdx) nMaxIdx = nTmpIdx;
	}

	if (nMaxIdx < 0) nMaxIdx = 0;
		else nMaxIdx++;
	return nMaxIdx;
}

INT CAccessControlWorkView::MENU_MODIFY(TCHAR cUSERFLAG)
{
	INT ids;

	INT nRows = m_pMenuDM->GetRowCount();
	if (nRows == 0) return 0;

	INT nRow = m_pGrid->GetRow()-1;
	CAccessControlSelectionMenuModifyDlg dlg(this);
	INT nIdx = 0;
	CString strTmp, strPK;
	strTmp = m_pMenuDM->GetCellData(nRow, 3);
	strTmp.TrimLeft();		strTmp.TrimRight();
	if (strTmp.GetLength() == 0) nIdx = 0;
		else nIdx = _ttoi(strTmp);
	dlg.m_nIdx = nIdx;
	dlg.m_strCAPTION = m_pMenuDM->GetCellData(nRow, 4);
	strPK = m_pMenuDM->GetCellData(nRow, 0);
	strPK.TrimLeft();		strPK.TrimRight();
	if (dlg.DoModal() != IDOK) return 0;

	ids = m_pMenuDM->StartFrame();

	m_pMenuDM->InitDBFieldData();
	strTmp.Format(_T("%d"), dlg.m_nIdx);
	ids = m_pMenuDM->AddDBFieldData(_T("IDX")		, _T("NUMERIC")	, strTmp					, 1);
	ids = m_pMenuDM->AddDBFieldData(_T("CAPTION")	, _T("STRING")	, dlg.m_strCAPTION			, 1);
	ids = m_pMenuDM->MakeUpdateFrame(_T("ESL_MENU_TBL"), _T("PK"), _T("NUMERIC"), strPK);

	ids = m_pMenuDM->SendFrame();
	if (ids < 0) AfxMessageBox(_T("Menu를 수정하던중 에러가 발생하였습니다."));
		else AfxMessageBox(_T("수정되었습니다."));
	ids = m_pMenuDM->EndFrame();
	
	return 0;
}

INT CAccessControlWorkView::MENU_DELETE(TCHAR cUSERFLAG)
{
	INT ids;

	INT nRows = m_pMenuDM->GetRowCount();
	if (nRows == 0) return 0;

	INT nRow = m_pGrid->GetRow()-1;
	CString strPK, strTmp;
	strPK = m_pMenuDM->GetCellData(nRow, 0);
	strPK.TrimLeft();		strPK.TrimRight();

	strTmp.Format(_T("[%s]를 정말 삭제하시겠습니까?"), m_pMenuDM->GetCellData(nRow, 1));
	if (AfxMessageBox(strTmp, MB_YESNO) != IDYES) return 0;
	INT nIdx;
	strTmp = m_pMenuDM->GetCellData(nRow, 3);
	strTmp.TrimLeft();		strTmp.TrimRight();
	if (strTmp.GetLength() == 0) nIdx = 0;
		else nIdx = _ttoi(strTmp);

	ids = m_pMenuDM->StartFrame();

	m_pMenuDM->InitDBFieldData();
	ids = m_pMenuDM->MakeDeleteFrame(_T("ESL_MENU_TBL"), _T("PK"), _T("NUMERIC"), strPK);

	if (AfxMessageBox(_T("삭제한후 메뉴를 재정렬 하시겠습니까?"), MB_YESNO) == IDYES) {
		CString strQuery, strTmpPK, strTmpIdx;
		INT nIdx = 0;
		for (INT i = 0; i < nRows; i++) {
			strTmpPK = m_pMenuDM->GetCellData(i, 0);
			strTmpPK.TrimLeft();		strTmpPK.TrimRight();
			if (strTmpPK == strPK) continue;
			strTmpIdx = m_pMenuDM->GetCellData(i, 3);
			strTmpIdx.TrimLeft();		strTmpIdx.TrimRight();
			if (strTmpIdx.GetLength() > 0) {
				if (_ttoi(strTmpIdx) < 0) continue;
			}
			
			strQuery.Format(_T("UPDATE ESL_MENU_TBL SET IDX=%d WHERE PK=%s;"), nIdx, strTmpPK);
			m_pMenuDM->AddFrame(strQuery);
			nIdx++;
		}
	}

	m_pMenuDM->SendFrame();
	m_pMenuDM->EndFrame();
	if (ids < 0) AfxMessageBox(_T("Menu를 삭제하던중 에러가 발생하였습니다."));
		else AfxMessageBox(_T("삭제되었습니다."));
	
	return 0;
}
