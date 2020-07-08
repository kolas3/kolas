// SIReportTestView.cpp : implementation of the CSIReportTestView class
//

#include "stdafx.h"
#include "SIReportTest.h"

#include "SIReportTestDoc.h"
#include "SIReportTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSIReportTestView

IMPLEMENT_DYNCREATE(CSIReportTestView, CFormView)

BEGIN_MESSAGE_MAP(CSIReportTestView, CFormView)
	//{{AFX_MSG_MAP(CSIReportTestView)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_NEW, OnFind)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIReportTestView construction/destruction

CSIReportTestView::CSIReportTestView()
	: CFormView(CSIReportTestView::IDD)
{
	//{{AFX_DATA_INIT(CSIReportTestView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_pos = 0;

	// DataMgr 초기화
	m_pDM = new CESL_DataMgr;
	m_pDM->SetCONNECTION_INFO(_T("kolas2@kolas2/kolas2#[]"));
	m_pDM->DB_MGR_ALIAS = _T("DM_테스트");
	m_pDM->TBL_NAME = _T("ESL_TEST");

	m_pDM->InitDataMgrRef(7);
	m_pDM->SetDataMgrRef(0, _T("아이디"),	_T("ID"),			_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(1, _T("암호"),		_T("PASSWORD"),		_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(2, _T("이름"),		_T("NAME"),			_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(3, _T("주소"),		_T("ADDRESS"),		_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(4, _T("전화번호"), _T("TEL"),			_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(5, _T("생일"),		_T("BIRTHDAY"),		_T("str"), _T(""), _T(""));
	m_pDM->SetDataMgrRef(6, _T("직급"),		_T("GRADE"),		_T("str"), _T(""), _T(""));

	m_strRptName = _T("");
}

CSIReportTestView::~CSIReportTestView()
{
	if (m_pDM) {
		delete m_pDM;
		m_pDM = NULL;
	}
}

VOID CSIReportTestView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSIReportTestView)
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDC_MSHFLEXGRID1, m_Grid);
	DDX_Control(pDX, IDC_EXPLORER1, m_Browser);
	//}}AFX_DATA_MAP
}

BOOL CSIReportTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

VOID CSIReportTestView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	InitTabControl();

	m_Grid.Clear();
	m_Grid.SetRows(2);
	m_Grid.SetCols(0, 8);

	SetColumnInfo(0, _T(""), 40, 4);
	SetColumnInfo(1, _T("아이디"),		 70, 4);
	SetColumnInfo(2, _T("암호"),		 70, 4);
	SetColumnInfo(3, _T("이름"),		70, 4);
	SetColumnInfo(4, _T("주소"),		200, 4);
	SetColumnInfo(5, _T("전화번호"),	100, 4);
	SetColumnInfo(6, _T("생일"),		 90, 4);
	SetColumnInfo(7, _T("직급"),		 50, 4);
	UpdateData(false);
}

/////////////////////////////////////////////////////////////////////////////
// CSIReportTestView printing

BOOL CSIReportTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

VOID CSIReportTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

VOID CSIReportTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

VOID CSIReportTestView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CSIReportTestView diagnostics

#ifdef _DEBUG
VOID CSIReportTestView::AssertValid() const
{
	CFormView::AssertValid();
}

VOID CSIReportTestView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSIReportTestDoc* CSIReportTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSIReportTestDoc)));
	return (CSIReportTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSIReportTestView message handlers

INT CSIReportTestView::InitTabControl(VOID)
{
	TC_ITEM item;

	item.mask = TCIF_IMAGE | TCIF_TEXT;
	item.iImage = 0;

	item.pszText = _T("Data");
	m_tab.InsertItem(0, &item);

	item.pszText = _T("Xml");
	m_tab.InsertItem(1, &item);
	UpdateData(false);

	return 0;
}

VOID CSIReportTestView::OnSize(UINT nType, INT cx, INT cy) 
{
	CFormView::OnSize(nType, cx, cy);
	INT pos = 0;

	RECT r;
	GetClientRect(&r);
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_TAB1);
	if (pWnd) {
		pWnd->MoveWindow(&r, true);
		CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB1);
		pWnd->GetClientRect(&r);
	}
	r.top += 25;
	r.left += 5;
	r.right -= 5;
	r.bottom -= 5;

	CWnd *pGrid = GetDlgItem(IDC_MSHFLEXGRID1);
	CWnd *pBrowser = GetDlgItem(IDC_EXPLORER1);
	if (m_pos == 0) {
		if (pGrid) pGrid->MoveWindow(&r, true);
		if (pBrowser) pBrowser->MoveWindow(0, 0, 0, 0, true);
	} else {
		if (pBrowser) pBrowser->MoveWindow(&r, true);
		if (pGrid) pGrid->MoveWindow(0, 0, 0, 0, true);
	}
}

VOID CSIReportTestView::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(true);
	m_pos = m_tab.GetCurSel();

	CWnd *pGrid = GetDlgItem(IDC_MSHFLEXGRID1);
	CWnd *pBrowser = GetDlgItem(IDC_EXPLORER1);

	RECT r;
	GetClientRect(&r);
	r.top += 25;
	r.left += 5;
	r.right -= 5;
	r.bottom -= 5;

	if (!pGrid) return;
	if (!pBrowser) return;
	if (m_pos == 0) {
		pGrid->MoveWindow(&r, true);
		pBrowser->MoveWindow(0, 0, 0, 0, true);
	} else {
		pBrowser->MoveWindow(&r, true);
		pGrid->MoveWindow(0, 0, 0, 0, true);
	}
	if (pResult) *pResult = 0;
}

VOID CSIReportTestView::OnFind(VOID)
{
	INT ids = m_pDM->RefreshDataManager(_T("ID IS NOT NULL ORDER BY ID"));
	INT i;
	CString strGrade;
	for (i = 0; i < m_pDM->GetRowCount(); i++) {
		strGrade = ConvertCodeToDesc_Grade(m_pDM->GetCellData(_T("직급"), i));
		m_pDM->SetCellData(_T("직급"), strGrade, i);
	}


	INT nRow = m_pDM->GetRowCount();
	m_Grid.Clear();
	m_Grid.SetRows(2);
	m_Grid.SetCols(0, 8);

	SetColumnInfo(0, _T(""), 40, 4);
	SetColumnInfo(1, _T("아이디"),		 70, 4);
	SetColumnInfo(2, _T("암호"),		 70, 4);
	SetColumnInfo(3, _T("이름"),		70, 4);
	SetColumnInfo(4, _T("주소"),		200, 4);
	SetColumnInfo(5, _T("전화번호"),	100, 4);
	SetColumnInfo(6, _T("생일"),		 90, 4);
	SetColumnInfo(7, _T("직급"),		 50, 4);
	UpdateData(false);
	if (nRow == 0) {
		AfxMessageBox(_T("검색된 자료가 없습니다."));
		return;
	}
	
	m_Grid.SetRows(nRow+1);
	CString strMsg;
	for (i = 0; i < nRow; i++) {
		strMsg.Format(_T("%d"), i+1);
		m_Grid.SetTextMatrix(i+1, 0, strMsg);
		m_Grid.SetTextMatrix(i+1, 1, m_pDM->GetCellData(_T("아이디"), i));
		m_Grid.SetTextMatrix(i+1, 2, m_pDM->GetCellData(_T("암호"), i));
		m_Grid.SetTextMatrix(i+1, 3, m_pDM->GetCellData(_T("이름"), i));
		m_Grid.SetTextMatrix(i+1, 4, m_pDM->GetCellData(_T("주소"), i));
		m_Grid.SetTextMatrix(i+1, 5, m_pDM->GetCellData(_T("전화번호"), i));
		m_Grid.SetTextMatrix(i+1, 6, m_pDM->GetCellData(_T("생일"), i));
		m_Grid.SetTextMatrix(i+1, 7, m_pDM->GetCellData(_T("직급"), i));
	}
	UpdateData(false);
}

INT CSIReportTestView::SetColumnInfo(INT col, CString str, INT width, INT alignment)
{
	m_Grid.SetColAlignmentFixed(col, 4);
	m_Grid.SetTextMatrix(0, col, str);
	m_Grid.SetColWidth(col, 0, width*15);
	m_Grid.SetColAlignment(col, alignment);
	
	return 0;
}

VOID CSIReportTestView::OnFileOpen(VOID)
{
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");

	wmemset(&OFN, 0, sizeof(OPENFILENAME));

	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFilter = _T("RPT파일(*.rpt)\0*.rpt\0모든파일(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("RPT 파일 열기...");
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = MAX_PATH;
	OFN.lpstrDefExt = _T("rpt");
	OFN.Flags = OFN_OVERWRITEPROMPT;

	if (::GetOpenFileName(&OFN) != 0)
	{
		CString strFileName;
		strFileName = OFN.lpstrFile;
		m_strRptName = strFileName;

		m_Browser.Navigate(strFileName, NULL, NULL, NULL, NULL);
		m_tab.SetCurSel(1);
		UpdateData(false);
		OnSelchangeTab1(NULL, NULL);
	}
}


VOID CSIReportTestView::OnFileSave(VOID)
{
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");

	wmemset(&OFN, 0, sizeof(OPENFILENAME));

	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFilter = _T("RPT파일(*.rpt)\0*.rpt\0모든파일(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("RPT 파일 DB로 저장");
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = MAX_PATH;
	OFN.lpstrDefExt = _T("rpt");
	OFN.Flags = OFN_OVERWRITEPROMPT;

	if (::GetOpenFileName(&OFN) == 0) return;
	CString strFileName = OFN.lpstrFile;

	if (AfxMessageBox(_T("파일을 DB에 Upload 하는 작업은 약간 오래걸릴수 있습니다. 계속 하시겠습니까?"), MB_YESNO) != IDYES) return;
	CTime t = CTime::GetCurrentTime();
	CString cur_date;
	cur_date.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	CFile fd;
	fd.Open(strFileName, CFile::modeRead | CFile::typeBinary);
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	CString query;
	CString PK;
	m_pDM->MakeRecKey(PK, _T("esl_seq"));
	query.Format(_T("INSERT INTO ESL_SIREPORT_TBL (PK, FLAG, RPTALIAS, ENGINEER, DESCRIP, RPTSIZE, FIRST_DATE, LAST_DATE) VALUES ")
		         _T("(%s, 'X', 'SI REPORT TEST', 'wjchoi', 'wjchoi@eco.co.kr', %d, '%s', '%s');"),
				 PK, fd.GetLength(), cur_date, cur_date);

	m_pDM->ExecuteQuery(query, true);
	
	BYTE *pBuffer = (BYTE *)malloc(sizeof(BYTE) * fd.GetLength() + 64);
	if (pBuffer == NULL) {
		AfxMessageBox(_T("메모리 할당 에러"));
		return;
	}
	INT BufferSize = sizeof(BYTE) * fd.GetLength() + 64;
	fd.ReadHuge(pBuffer, fd.GetLength());

	INT ids = m_pDM->LOBUpdate(_T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), PK, _T("RPTDATA"), pBuffer, fd.GetLength());
	if (ids < 0) {
		AfxMessageBox(_T("파일을 DB에 올리던중 에러가 발생하였습니다."));
	} else {
		AfxMessageBox(_T("파일을 DB에 성공적으로 Upload 하였습니다."));
	}
	if (pBuffer) {
		free(pBuffer);
		pBuffer = NULL;
	}
}

VOID CSIReportTestView::OnFilePrint(VOID)
{
	if (m_strRptName.GetLength() == 0) m_strRptName = _T("sireporttest2.rpt");

	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return;
	}
	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("SI REPORT TEST"));
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return;
	}
	
	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	pSIReportManager->SetDataMgr(m_pDM);
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
}

INT CSIReportTestView::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	CString msg;
	msg.Format(_T("- %d/%d -"), EventInfo->m_nPage, EventInfo->m_nTotalPage);


	//INT horz_align;							// 정렬(왼쪽:ALIGN_LEFT, 중앙:ALIGN_HCENTER, 오른쪽:ALIGN_RIGHT)
	//INT vert_align;							// 정렬(위:ALIGN_TOP, 가운데:ALIGN_VCENTER, 아래:ALIGN_BOTTOM)
	//pSIReportManager->SetTableCellText(_T("페이지번호"), 0, 0, msg, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	pSIReportManager->SetTableCellText(_T("페이지번호"), 0, 0, msg, ALIGN_VCENTER, ALIGN_HCENTER);
	if (EventInfo->m_nPage == 1) pSIReportManager->SetBarcodeCaption(_T("바코드"), _T("123"));
	if (EventInfo->m_nPage == 2) pSIReportManager->SetBarcodeCaption(_T("바코드"), _T("123456"));
	if (EventInfo->m_nPage == 3) pSIReportManager->SetBarcodeCaption(_T("바코드"), _T("123456789"));
	return 0;
}

CString CSIReportTestView::ConvertCodeToDesc_Grade(CString code)
{
	code.TrimLeft();		code.TrimRight();
	if (code == _T("1")) return _T("Ⅰ급");
	if (code == _T("2")) return _T("Ⅱ급");
	if (code == _T("3")) return _T("Ⅲ급");
	if (code == _T("4")) return _T("Ⅳ급");
	if (code == _T("5")) return _T("Ⅴ급");
	return _T("적용안함");
}

INT CSIReportTestView::GetSIReportFileFromDB(CString RptName, CString tmpFilename, CString &PK)
{
	CESL_DataMgr *pDM = new CESL_DataMgr;
	if (!pDM) return -1000;
	pDM->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	INT i, ids;
	CString szBuf, sQuery;
	CString strPK = _T(""), strRPTSize;

	sQuery.Format(_T("SELECT PK, RPTSIZE, FLAG, BITMAP_ALIAS FROM ESL_SIREPORT_TBL WHERE RPTALIAS='%s'"), RptName);
	pDM->RestructDataManager(sQuery);
	INT nRow = pDM->GetRowCount();

	// 사용자 수정 형식이 있는지 찾아본다. 있으면 이것이 원본 형식보다 우선한다.
	for (i = 0; i < nRow; i++) {
		pDM->GetCellData(i, 2, szBuf);
		szBuf.TrimLeft();			szBuf.TrimRight();
		szBuf.MakeUpper();
		if (szBuf == _T("U")) {
			pDM->GetCellData(i, 0, strPK);
			strPK.TrimLeft();		strPK.TrimRight();
			pDM->GetCellData(i, 1, strRPTSize);
			strRPTSize.TrimLeft();	strRPTSize.TrimRight();
			if (strRPTSize.GetLength() == 0) strRPTSize = _T("0");
			break;
		}
	}
	// 사용자 수정 형식이 없을때는 원본 형식을 찾는다.
	if (strPK.GetLength() == 0) {
		for (i = 0; i < nRow; i++) {
			pDM->GetCellData(i, 2, szBuf);
			szBuf.TrimLeft();			szBuf.TrimRight();
			szBuf.MakeUpper();
			if (szBuf == _T("X")) {
				pDM->GetCellData(i, 0, strPK);
				strPK.TrimLeft();		strPK.TrimRight();
				pDM->GetCellData(i, 1, strRPTSize);
				strRPTSize.TrimLeft();	strRPTSize.TrimRight();
				if (strRPTSize.GetLength() == 0) strRPTSize = _T("0");
				break;
			}
		}
	}
	if (strPK.GetLength() == 0) return -1022;		// 해당 Report 가 없습니다.
	PK = strPK;

	// Report Data File을 가져온다.
	ids = GetLOBToFile(pDM, _T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), _ttoi(strRPTSize), tmpFilename);
	if (ids < 0) return -1025;		// Lob을 File로 다운 받는데 실패

	CString strTmp, strALIAS;
	for (i = 0; i < nRow; i++) {
		pDM->GetCellData(i, 2, szBuf);
		szBuf.TrimLeft();			szBuf.TrimRight();
		szBuf.MakeUpper();
		if (szBuf == _T("B")) {
			pDM->GetCellData(i, 0, strPK);
			strPK.TrimLeft();		strPK.TrimRight();
			pDM->GetCellData(i, 1, strRPTSize);
			strRPTSize.TrimLeft();	strRPTSize.TrimRight();
			if (strRPTSize.GetLength() == 0) strRPTSize = _T("0");
			pDM->GetCellData(i, 3, strALIAS);
			strALIAS.TrimLeft();	strALIAS.TrimRight();
			if (strALIAS.GetLength() == 0) continue;
			strTmp.Format(_T("rpt_tmp\\%s.bmp"), strALIAS);
			ids = GetLOBToFile(pDM, _T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), _ttoi(strRPTSize), strTmp);
		}
	}
	
	return 0;
}

INT CSIReportTestView::GetLOBToFile(CESL_DataMgr *pDM, CString table_name, CString PKFieldname, CString PKFieldtype, CString PKData, CString FieldName, INT Size, CString FileName)
{
	BYTE *pBuffer;
	INT nBufferSize = Size + 1024;
	INT ids;

	pBuffer = (BYTE*)malloc(sizeof(BYTE) * nBufferSize);
	if (!pBuffer) {
		AfxMessageBox(_T("Malloc Memory Fail!"));
		return -1024;
	}
	
	ids = pDM->LOBGet(table_name, PKFieldname, PKFieldtype, PKData, FieldName, pBuffer, &nBufferSize);
	if (ids < 0) {
		AfxMessageBox(_T("Get Lob Data Fail!"));
		free(pBuffer);
		return -1024;
	}
	if (nBufferSize <= 0) {
		AfxMessageBox(_T("Empty Lob Data!"));
		return -1024;
	}
	CFile f;
    if( !f.Open(FileName, CFile::modeWrite|CFile::modeCreate | CFile::typeBinary) ) {
		free(pBuffer);
		AfxMessageBox(_T("Create Lob File Fail"));
		return - 1025;
	}
	
	if( 0 == f.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		f.Write( &cUni, sizeof(TCHAR));
	}
		
	f.WriteHuge(pBuffer, nBufferSize);
	f.Close();
	free (pBuffer);

	return 0;
}