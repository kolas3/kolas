// AccessControlView.cpp : implementation of the CAccessControlView class
//

#include "stdafx.h"
#include "AccessControl.h"

#include "AccessControlDoc.h"
#include "AccessControlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MainFrm.h"
#include "AccessControlNewMenuGroupDlg.h"
#include "AccessControlWorkView.h"
#include "AccessControlSmartIDMenuListDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAccessControlView

IMPLEMENT_DYNCREATE(CAccessControlView, CFormView)

BEGIN_MESSAGE_MAP(CAccessControlView, CFormView)
	//{{AFX_MSG_MAP(CAccessControlView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_COMMAND(ID_MENU_GROUP_NEW, COMMAND_NewMenuGroup)
	ON_COMMAND(ID_MENU_GROUP_DELETE, COMMAND_DeleteMenuGroup)
	ON_COMMAND(ID_MENU_GROUP_MODIFY, COMMAND_ModifyMenuGroup)
	ON_COMMAND(ID_SMARTID_MENU_LIST, COMMAND_SmartIDMenuList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessControlView construction/destruction

CAccessControlView::CAccessControlView()
	: CFormView(CAccessControlView::IDD)
{
	//{{AFX_DATA_INIT(CAccessControlView)
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_pMenuGroupDM = new CESL_DataMgr;
	// KOL.UDF.022
	m_pMenuGrid = NULL;
	USER_FLAG = ' ';
}

CAccessControlView::~CAccessControlView()
{
	if (m_pMenuGroupDM) {
		delete m_pMenuGroupDM;
		m_pMenuGroupDM = NULL;
	}
	if (m_pMenuGrid) {
		delete m_pMenuGrid;
		m_pMenuGrid = NULL;
	}
}

VOID CAccessControlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessControlView)
	DDX_Control(pDX, IDC_tMENU, m_tab);
	//}}AFX_DATA_MAP
}

BOOL CAccessControlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

VOID CAccessControlView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	
	ReadCfgFile.ReadCfgFile(_T(""));
	m_pMenuGroupDM->SetDB(ReadCfgFile.m_sDBKIND);
	m_pMenuGroupDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
	
	// Grid 초기화
	m_pMenuGrid = new CESL_Grid(NULL);
	m_pMenuGrid->SubclassDlgItem(IDC_gMENU, this);
//	InitGrid();

	m_tab.InsertItem(0, _T("MENU GROUP"));
}

/////////////////////////////////////////////////////////////////////////////
// CAccessControlView printing

BOOL CAccessControlView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

VOID CAccessControlView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

VOID CAccessControlView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

VOID CAccessControlView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CAccessControlView diagnostics

#ifdef _DEBUG
VOID CAccessControlView::AssertValid() const
{
	CFormView::AssertValid();
}

VOID CAccessControlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CAccessControlDoc* CAccessControlView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAccessControlDoc)));
	return (CAccessControlDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAccessControlView message handlers

VOID CAccessControlView::OnSize(UINT nType, INT cx, INT cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	RECT r;
	GetClientRect(&r);
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_tMENU);
	if (pWnd) {
		pWnd->MoveWindow(&r, true);
		CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_tMENU);
		pWnd->GetClientRect(&r);
	}
	pWnd = NULL;
	pWnd = GetDlgItem(IDC_gMENU);
	if (pWnd) {
		r.top += 25;
		r.left += 5;
		r.right -= 5;
		r.bottom -= 5;
		pWnd->MoveWindow(&r, true);
	}
}

VOID CAccessControlView::ShowMenuGroupList(VOID)
{
	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	CString query;
	
	if(USER_FLAG == 'S')
	{
		//                   0     1     2       3        4       5           6               7
		query.Format(_T("SELECT PK, ALIAS, IDX, GROUPTYPE ,TARGET, CAPTION, SPF_USE_FLAG ,PROGRAME_USE_FLAG FROM ESL_MENUGROUP_TBL ORDER BY ALIAS"));	
	}
	else
	{
		//                   0     1     2       3        4       5           6               7
		query.Format(_T("SELECT PK, ALIAS, IDX, GROUPTYPE, TARGET, CAPTION, SPF_USE_FLAG, PROGRAME_USE_FLAG FROM ESL_MENUGROUP_TBL WHERE SPF_USE_FLAG = 'y' and PROGRAME_USE_FLAG = 'y' ORDER BY ALIAS"));
	}
	m_pMenuGroupDM->RestructDataManager(query);

	INT nRow = m_pMenuGroupDM->GetRowCount();
	INT gridrow;
	if (nRow == 0) gridrow = 2;
		else gridrow = nRow+1;
	m_pMenuGrid->RemoveAll();
	m_pMenuGrid->SetRows(gridrow);
	InitGrid();
	
	CString msg;
	CString strData;
	for (INT i = 0; i < nRow; i ++) {
		strData.Format(_T("%d"), i);
		m_pMenuGrid->SetTextMatrix(i+1, 0, strData);
		m_pMenuGrid->SetTextMatrix(i+1, 1, m_pMenuGroupDM->GetCellData(i, 3));
		m_pMenuGrid->SetTextMatrix(i+1, 2, m_pMenuGroupDM->GetCellData(i, 1));
		if (m_pMenuGroupDM->GetCellData(i, 3) == _T("Y")) msg = m_pMenuGroupDM->GetCellData(i, 2);
			else msg = _T("");
		m_pMenuGrid->SetTextMatrix(i+1, 3, msg);
		if(USER_FLAG == 'S')
		{
			m_pMenuGrid->SetTextMatrix(i+1, 4, m_pMenuGroupDM->GetCellData(i, 5));
			m_pMenuGrid->SetTextMatrix(i+1, 5, m_pMenuGroupDM->GetCellData(i, 6));
			m_pMenuGrid->SetTextMatrix(i+1, 6, m_pMenuGroupDM->GetCellData(i, 7));
		} else {
			m_pMenuGrid->SetTextMatrix(i+1, 4, m_pMenuGroupDM->GetCellData(i, 5));
			m_pMenuGrid->SetTextMatrix(i+1, 5, m_pMenuGroupDM->GetCellData(i, 6));
			m_pMenuGrid->SetTextMatrix(i+1, 6, m_pMenuGroupDM->GetCellData(i, 7));
		}
	}
	return;
}

VOID CAccessControlView::InitGrid(VOID)
{
	if(USER_FLAG == 'S')
	{
		m_pMenuGrid->SetCols(0,7);
		m_pMenuGrid->SetColumnInfo(0-1, _T(""),			30,			4);
		m_pMenuGrid->SetColumnInfo(1-1, _T("Type"),		30,			4);
		m_pMenuGrid->SetColumnInfo(2-1, _T("Alias"),	120,		1);
		m_pMenuGrid->SetColumnInfo(3-1, _T("Idx"),		0,			4);
		m_pMenuGrid->SetColumnInfo(4-1, _T("Caption"),	150,		1);
		m_pMenuGrid->SetColumnInfo(5-1, _T("SPF"),		30,			4);
		m_pMenuGrid->SetColumnInfo(6-1, _T("PRO"),		30,			4);
	}
	else
	{
		m_pMenuGrid->SetCols(0,7);
		m_pMenuGrid->SetColumnInfo(0-1, _T(""),			30,			4);
		m_pMenuGrid->SetColumnInfo(1-1, _T("Type"),		30,			4);
		m_pMenuGrid->SetColumnInfo(2-1, _T("Alias"),	150,		1);
		m_pMenuGrid->SetColumnInfo(3-1, _T("Idx"),		0,			4);
		m_pMenuGrid->SetColumnInfo(4-1, _T("Caption"),	150,		1);
		m_pMenuGrid->SetColumnInfo(5-1, _T("SPF"),		30,			4);
		m_pMenuGrid->SetColumnInfo(6-1, _T("PRO"),		30,			4);
	}
	return;
}

VOID CAccessControlView::COMMAND_NewMenuGroup(VOID)
{
	if (USER_FLAG != 'S') {
		AfxMessageBox(_T("Administrtor만 입력가 가능합니다."));
		return;
	}

	CAccessControlNewMenuGroupDlg dlg;
	dlg.MODE = 1;			// 입력모드
	dlg.m_cUSERFLAG = USER_FLAG;
	if (dlg.DoModal() != IDOK) return;

	CString query;
	query.Format(_T("SELECT COUNT(*) FROM ESL_MENUGROUP_TBL WHERE ALIAS = '%s'"), dlg.m_sName);
	CString strResult = _T("");
	m_pMenuGroupDM->GetOneValueQuery(query, strResult);
	strResult.TrimLeft();		strResult.TrimRight();
	if (strResult != _T("0")) {
		AfxMessageBox(_T("Menu group의 alias는 중복 될 수 없습니다. 다른 alias로 다시 시도 하십시오"));
		return;
	}

	CString strPK;
	if (m_pMenuGroupDM->m_dbkind == 2) m_pMenuGroupDM->MakeRecKey(strPK, ReadCfgFile.m_sSequence);
	if (m_pMenuGroupDM->m_dbkind == 1) strPK.Format(_T("%s.NEXTVAL"), ReadCfgFile.m_sSequence);
	query.Format(_T("INSERT INTO ESL_MENUGROUP_TBL(PK, ALIAS, IDX, GROUPTYPE, CAPTION) VALUES (%s, '%s', %d, '%s', '%s');"), 
					strPK, dlg.m_sName, dlg.m_idx, dlg.m_strGroupType, dlg.m_strCaption
				);

	INT ids;
	ids = m_pMenuGroupDM->StartFrame();
	ids = m_pMenuGroupDM->AddFrame(query);
	ids = m_pMenuGroupDM->SendFrame();
	m_pMenuGroupDM->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("Menu group를 추가하던중 에러가 발생하였습니다."));
	} else {
		AfxMessageBox(_T("입력되었습니다."));
	}
	ShowMenuGroupList();
}

VOID CAccessControlView::COMMAND_ModifyMenuGroup(VOID)
{
	//if (USER_FLAG != 'S') {
	//	AfxMessageBox(_T("Administrtor만 수정이 가능합니다."));
	//	return;
	//}
	INT nRow = m_pMenuGroupDM->GetRowCount();
	if (nRow == 0) return;
	INT row = m_pMenuGrid->GetRow();
	if (row == 0) return;

	CString pk, name, idx, strGroupType, strTarget;
	pk = m_pMenuGroupDM->GetCellData(row-1, 0);
	name = m_pMenuGroupDM->GetCellData(row-1, 1);
	idx = m_pMenuGroupDM->GetCellData(row-1, 2);
	strGroupType = m_pMenuGroupDM->GetCellData(row-1, 3);
	if (pk.GetLength() == 0) {
		AfxMessageBox(_T("손상된 데이타 입니다."));
		return;
	}

	CAccessControlNewMenuGroupDlg dlg;
	dlg.MODE = -1;			// 수정모드
	dlg.m_sName = name;
	dlg.m_strGroupType = strGroupType;
	dlg.m_strCaption = m_pMenuGroupDM->GetCellData(row-1, 5);
	dlg.m_cUSERFLAG = USER_FLAG;
	if (dlg.DoModal() != IDOK) return;
	if (USER_FLAG != 'S') {
		return;
	}
	INT ids;
	ids = m_pMenuGroupDM->StartFrame();

	CString query;
	query.Format(_T("UPDATE ESL_MENUGROUP_TBL SET ALIAS='%s', IDX='%d', GROUPTYPE='%s', CAPTION='%s' WHERE PK = %s;"), 
					dlg.m_sName, dlg.m_idx, dlg.m_strGroupType, dlg.m_strCaption, pk
				);
	ids = m_pMenuGroupDM->AddFrame(query);

	query.Format(_T("UPDATE ESL_MENU_TBL SET GROUP_ALIAS='%s' WHERE GROUP_ALIAS='%s';"), dlg.m_sName, name);
	ids = m_pMenuGroupDM->AddFrame(query);

	if (AfxMessageBox(_T("메뉴에서 사용하고 있는 Alias까지 모두 적용하시겠습니까?"), MB_YESNO) == IDYES) {
		query.Format(_T("UPDATE ESL_MENU_TBL SET CAPTION='%s' WHERE ALIAS='%s';"), dlg.m_strCaption, dlg.m_sName);
		ids = m_pMenuGroupDM->AddFrame(query);
	}
	ids = m_pMenuGroupDM->SendFrame();
	m_pMenuGroupDM->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("Menu group를 수정하던중 에러가 발생하였습니다."));
	} else {
		AfxMessageBox(_T("수정되었습니다."));
	}
	ShowMenuGroupList();


	return;
}

BEGIN_EVENTSINK_MAP(CAccessControlView, CFormView)
    //{{AFX_EVENTSINK_MAP(CAccessControlView)
	ON_EVENT(CAccessControlView, IDC_gMENU, -601 /* DblClick */, OnDblClickgMENU, VTS_NONE)
	ON_EVENT(CAccessControlView, IDC_gMENU, -605 /* MouseDown */, OnMouseDowngMENU, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CAccessControlView::OnDblClickgMENU() 
{
	INT nRow = m_pMenuGroupDM->GetRowCount();
	if (nRow == 0) return;
	INT row = m_pMenuGrid->GetRow();
	if (row == 0) return;

	CMainFrame * pframe = (CMainFrame*)GetParentFrame();
	CAccessControlWorkView *pWorkView = (CAccessControlWorkView *)pframe->GetSecondView();

	pWorkView->MenuGroupAlias = m_pMenuGroupDM->GetCellData(row-1, 1);
	pWorkView->ShowMenuList();
	CString strGroupType = m_pMenuGroupDM->GetCellData(row-1, 3);
	pWorkView->SetReadOnly(FALSE);
	if (strGroupType == _T("T")) pWorkView->m_cTOPMENUFLAG = 'T';
		else pWorkView->m_cTOPMENUFLAG = ' ';
}

VOID CAccessControlView::OnMouseDowngMENU(short Button, short Shift, long x, long y) 
{
	INT row = m_pMenuGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		//m_pMenuGrid->SetReverse(row-1);
	}
}

VOID CAccessControlView::COMMAND_DeleteMenuGroup(VOID)
{
	if (USER_FLAG != 'S') {
		AfxMessageBox(_T("Administrtor만 삭제가 가능합니다."));
		return;
	}
	INT nRow = m_pMenuGroupDM->GetRowCount();
	if (nRow == 0) return;
	INT row = m_pMenuGrid->GetRow();
	if (row == 0) return;

	CString pk, name;
	pk = m_pMenuGroupDM->GetCellData(row-1, 0);
	name = m_pMenuGroupDM->GetCellData(row-1, 1);
	if (pk.GetLength() == 0) {
		AfxMessageBox(_T("손상된 데이타 입니다."));
		return;
	}
	CString msg;
	msg.Format(_T("[%s]를 정말 삭제하시겠습니까? 하위 툴바와 메뉴가 모두 삭제됩니다."), name);
	if (AfxMessageBox(msg, MB_YESNO) != IDYES) return;

	CESL_DataMgr *pDM = new CESL_DataMgr;
	if (!pDM) return;
	pDM->SetDB(ReadCfgFile.m_sDBKIND);
	pDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);

	// TOOL Bar Button 지우기
	CString query;
	query.Format(_T("SELECT ALIAS FROM ESL_MENU_TBL WHERE GROUP_ALIAS='%s'"), name);
	pDM->RestructDataManager(query);
	INT tmpRow = pDM->GetRowCount();
	INT i, ids;
	pDM->StartFrame();
	for (i = 0; i < tmpRow; i++) {
		query.Format(_T("DELETE FROM ESL_TOOLGROUP_TBL WHERE MENU_ALIAS='%s';"), pDM->GetCellData(i, 0));
		pDM->AddFrame(query);
		query.Format(_T("DELETE FROM ESL_TOOLBAR_TBL WHERE MENU_ALIAS='%s';"), pDM->GetCellData(i, 0));
		pDM->AddFrame(query);
	}
	ids = 0;
	if (tmpRow > 0) ids = pDM->SendFrame();
	pDM->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("Tool Bar Button들을 지우던중 에러가 발생하였습니다."));
	}

	pDM->StartFrame();
	// ESL_MENU_TBL 지우기
	query.Format(_T("DELETE FROM ESL_MENU_TBL WHERE GROUP_ALIAS='%s';"), name);
	pDM->AddFrame(query);

	query.Format(_T("DELETE FROM ESL_MENUGROUP_TBL WHERE ALIAS='%s';"), name);
	pDM->AddFrame(query);
	ids = pDM->SendFrame();
	pDM->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("MENU를 지우던중 에러가 발생하였습니다."));
	}
	
	if (pDM) {
		delete pDM;
		pDM = NULL;
	}
	ShowMenuGroupList();
	AfxMessageBox(_T("삭제 작업을 마쳤습니다."));
	return;
}

VOID CAccessControlView::COMMAND_SmartIDMenuList(VOID)
{
	CMainFrame * pFrame = (CMainFrame*)GetParentFrame();
	if (pFrame == NULL) return;
	CAccessControlSmartIDMenuListDlg dlg;
	dlg.CONNECTION_INFO = ReadCfgFile.m_sConnection;
	dlg.FLAG = pFrame->FLAG;
	dlg.USER_ID = pFrame->NAME;
	dlg.DoModal();

	return;
}