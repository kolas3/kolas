// SIReportEditorView.cpp : implementation of the CSIReportEditorView class
//

#include "stdafx.h"
#include "SIReportEditor.h"

#include "SIReportEditorDoc.h"
#include "SIReportEditorView.h"

#include "DlgRptFileManage.h"
#include "DlgRptSave.h"

#include "DlgPageSetup.h"

#include "DlgDefaultDMSetup.h"

#include "MainFrm.h"

#include "ESL_DataMgr.h"
#include "efs.h"

#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSIReportEditorView

IMPLEMENT_DYNCREATE(CSIReportEditorView, CScrollView)

BEGIN_MESSAGE_MAP(CSIReportEditorView, CScrollView)
	//{{AFX_MSG_MAP(CSIReportEditorView)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_LAYOUT_MAKE_SAME_SIZE, OnLayoutMakeSameSize)
	ON_COMMAND(ID_LAYOUT_MAKE_SAME_WIDTH, OnLayoutMakeSameWidth)
	ON_COMMAND(ID_LAYOUT_MAKE_SAME_HEIGHT, OnLayoutMakeSameHeight)
	ON_COMMAND(ID_LAYOUT_ORDER_BRING_TO_FRONT, OnLayoutOrderBringToFront)
	ON_COMMAND(ID_LAYOUT_ORDER_SEND_TO_BACK, OnLayoutOrderSendToBack)
	ON_COMMAND(ID_LAYOUT_ORDER_BRING_FORWARD, OnLayoutOrderBringForward)
	ON_COMMAND(ID_LAYOUT_ORDER_SEND_BACKWARD, OnLayoutOrderSendBackward)
	ON_COMMAND(ID_LAYOUT_ALIGN_LEFT, OnLayoutAlignLeft)
	ON_COMMAND(ID_LAYOUT_ALIGN_HORZ_CENTER, OnLayoutAlignHorzCenter)
	ON_COMMAND(ID_LAYOUT_ALIGN_RIGHT, OnLayoutAlignRight)
	ON_COMMAND(ID_LAYOUT_ALIGN_TOP, OnLayoutAlignTop)
	ON_COMMAND(ID_LAYOUT_ALIGN_VERT_CENTER, OnLayoutAlignVertCenter)
	ON_COMMAND(ID_LAYOUT_ALIGN_BOTTOM, OnLayoutAlignBottom)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_FILE_PAGE_SETUP, OnFilePageSetup)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_EDIT_ZOOM_IN, OnEditZoomIn)
	ON_COMMAND(ID_EDIT_ZOOM_OUT, OnEditZoomOut)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_WM_TIMER()
	ON_COMMAND(ID_FILE_SAVE_TO_FILE, OnFileSaveToFile)
	ON_COMMAND(ID_FILE_NEW_SHEET, OnFileNewSheet)
	ON_COMMAND(ID_TABLE_INSERT_ROW, OnTableInsertRow)
	ON_COMMAND(ID_TABLE_INSERT_COL, OnTableInsertCol)
	ON_COMMAND(ID_TABLE_DELETE_ROW, OnTableDeleteRow)
	ON_COMMAND(ID_TABLE_DELETE_COL, OnTableDeleteCol)
	ON_COMMAND(ID_TABLE_CHANGE_COL, OnTableChangeCol)
	ON_COMMAND(ID_TABLE_MERGE, OnTableMerge)
	ON_COMMAND(ID_TABLE_DEMERGE, OnTableDemerge)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW_SHEET, OnUpdateFileNewSheet)
	ON_COMMAND(ID_FILE_DEFAULT_DM_SETUP, OnFileDefaultDmSetup)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSIReportEditorView construction/destruction

CSIReportEditorView::CSIReportEditorView()
	:m_ioMgr(MASTER_IS_EDITOR), m_sheetDesigner(&m_ioMgr)
{
	m_pSIReportWorkForm = NULL;
	
	m_nSheetHeight = EDITOR_PAPER_A4_X;
	m_nSheetWidth = EDITOR_PAPER_A4_Y;

	m_pointSheet.x = 10;
	m_pointSheet.y = 10;

	m_strTempDir = TEMP_DIR;

	m_strReportPK = _T("");
	m_strOpenAlias = _T("");

	m_nEditorMode = USER_NORMAL;
}

CSIReportEditorView::~CSIReportEditorView()
{
	if (m_pSIReportWorkForm != NULL)
	{
		delete m_pSIReportWorkForm;
		m_pSIReportWorkForm = NULL;
	}

	DeleteTempBitmapFile(MASTER_IS_EDITOR);
}

INT CSIReportEditorView::DeleteTempBitmapFile(INT MODE) 
{
EFS_BEGIN

	CFileFind finder;
	CString strWildCard;
	BOOL bWorking;

	if (MODE == MASTER_IS_MANAGER) {
		strWildCard.Format(_T("%s\\%s\\*.bmp"), m_strTempDir, MANAGER_TEMP_DIR);
	} else {
		strWildCard.Format(_T("%s\\Editor\\*.bmp"), m_strTempDir);
	}

	CString strPath, strName;
	bWorking = finder.FindFile(strWildCard);	
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		strPath = finder.GetFilePath();
		strName = finder.GetFileName();

		DeleteFile(strPath);
	}
	
	return 0;

EFS_END
return -1;

}

BOOL CSIReportEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
EFS_BEGIN

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.lpszClass = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW, AfxGetApp()->LoadStandardCursor(IDC_ARROW), ( HBRUSH )::GetStockObject( GRAY_BRUSH ), 0 );
    
	return CScrollView::PreCreateWindow(cs);

EFS_END
return FALSE;

}

/////////////////////////////////////////////////////////////////////////////
// CSIReportEditorView Operations
INT CSIReportEditorView::GetBmpIdxOfWorkForm()
{
EFS_BEGIN

	if (m_pSIReportWorkForm == NULL) return 0;

	INT nBmpIdx = 0;

	POSITION pos = m_pSIReportWorkForm->WorkSheetList.GetHeadPosition();
	while (pos != NULL)
	{
		CSIReportWorkSheet* pSheet = (CSIReportWorkSheet*)m_pSIReportWorkForm->WorkSheetList.GetNext(pos);
		if (pSheet == NULL) continue;
		
		POSITION posObject = pSheet->ObjList.GetHeadPosition();
		while (posObject != NULL)
		{
			CSIReportObject* pObject = (CSIReportObject*)pSheet->ObjList.GetNext(posObject);
			if (pObject != NULL && pObject->GetObjType() == OBJECT_BITMAP)
				nBmpIdx++;
		}
	}

	return nBmpIdx;	

EFS_END
return -1;

}

VOID CSIReportEditorView::EditWorkSheet(CString strSheetAlias)
{
EFS_BEGIN

	if (strSheetAlias.IsEmpty()) 
		return;

	if (m_pSIReportWorkForm != NULL)
	{
		delete m_pSIReportWorkForm;
		m_pSIReportWorkForm = NULL;
	}

	m_pSIReportWorkForm = new CSIReportWorkForm(MASTER_IS_EDITOR);
	
	m_pSIReportWorkForm->LoadWorkForm(m_strTempFileName);
	ReCalcAllRect(m_pSIReportWorkForm, -1);

	POSITION pos = m_pSIReportWorkForm->WorkSheetList.GetHeadPosition();
	while (pos != NULL)
	{
		CSIReportWorkSheet* pSheet = (CSIReportWorkSheet*)m_pSIReportWorkForm->WorkSheetList.GetNext(pos);
		if (pSheet != NULL && pSheet->Alias == strSheetAlias)
		{
			if (m_ReadBroker.MODE == EDIT_DATA)
				m_sheetDesigner.InitMSheetDesinger(pSheet, TRUE);
			else
				m_sheetDesigner.InitMSheetDesinger(pSheet);
			return;
		}
	}

EFS_END
}

VOID CSIReportEditorView::EditPage(CString strPageFileName)
{
EFS_BEGIN

	strPageFileName.Format(_T("%s\\%s"), m_strTempDir, strPageFileName);

	if (m_pSIReportWorkForm != NULL)
	{
		delete m_pSIReportWorkForm;
		m_pSIReportWorkForm = NULL;
	}

	m_pSIReportWorkForm = new CSIReportWorkForm(MASTER_IS_EDITOR);
	m_pSIReportWorkForm->LoadWorkForm(strPageFileName);
	ReCalcAllRect(m_pSIReportWorkForm, -1);
	m_strCurrentOpenFileName = strPageFileName;
	
	POSITION pos = m_pSIReportWorkForm->WorkSheetList.FindIndex(0);
	if (pos != NULL)
	{
		CSIReportWorkSheet *pWorkSheet = (CSIReportWorkSheet*)m_pSIReportWorkForm->WorkSheetList.GetAt(pos);
		if (pWorkSheet == NULL) return;
		
		if (m_ReadBroker.MODE == EDIT_FORM)
			m_sheetDesigner.InitMSheetDesinger(pWorkSheet, TRUE);
		else
			m_sheetDesigner.InitMSheetDesinger(pWorkSheet);

		m_sheetDesigner.SetBitmapIndex(GetBmpIdxOfWorkForm());
	}

EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// CSIReportEditorView drawing
VOID CSIReportEditorView::OnInitialUpdate()
{
EFS_BEGIN

	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = m_pointSheet.x * 2 + m_nSheetWidth;
	sizeTotal.cy = m_pointSheet.y * 2 + m_nSheetHeight;

	SetScrollSizes(MM_TEXT, sizeTotal);
	
	// Get Current Directory
	TCHAR tchBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, (LPTSTR)tchBuffer);
	m_strTempDir.Format(_T("%s\\%s"), tchBuffer, TEMP_DIR);
	m_strTempFileName.Format(_T("%s\\tmp_ReportEditor.rpt"), m_strTempDir);

	// Make Temp directory
	_tmkdir(TEMP_DIR);
	_tmkdir(EDITOR_TEMP_DIR);

	// Determine Editor Mode --> Normal User, Developer
	CFile file;
	if (file.Open(_T("..\\CFG\\_DEBUG"), CFile::modeRead | CFile::typeBinary))
	{
		m_nEditorMode = USER_DEVELOPER;		
		file.Close();
	}
	else
	{
		m_nEditorMode = USER_NORMAL;
	}

	// Road Data Manager Config File
	m_ReadCfgFile.ReadCfgFile(_T(""));

	INT ids;
	ids = GetDataMgrList();

	// Test Code
	//m_strOpenAlias = _T("50005023");
	//m_ReadBroker.SetData(_T("50005023"), _T("BITMAP"), EDIT_FORM);
	//m_ReadBroker.Save();

	// Create Sheet Designer
	CRect rect;
	rect.top = m_pointSheet.x;
	rect.left = m_pointSheet.y;
	rect.right = rect.left + m_nSheetWidth;
	rect.bottom = rect.top + m_nSheetHeight;
	m_sheetDesigner.Create(NULL, _T("EditView"), WS_VISIBLE|WS_CHILD, rect, this, WM_USER + 100);
	m_sheetDesigner.ShowWindow(SW_HIDE);
		
	// When Call Open By Manager
	if (!m_strOpenAlias.IsEmpty())
		InitSIReportEditor();
	else
		OnFileNew();

EFS_END
}

VOID CSIReportEditorView::InitSIReportEditor()
{
EFS_BEGIN

	// Read Open Param
	m_ReadBroker.Load(_T(""));

	// Init Sheet View Tree Ctrl
	((CMainFrame*)GetParentFrame())->GetTabDialog()->m_dlgSheetView.InitTreeCtrl();
	
	// Set Open Alias
	m_strOpenAlias = m_ReadBroker.RPTALIAS;
	
	// Check Open Files
	CString strPageName;
	CString strFileName;
	
	for (INT i = 1; i <= 9999; i++)
	{
		strPageName.Format(_T("PAGE%04d"), i);
		strFileName.Format(_T("%s\\%s.xml"), m_strTempDir, strPageName, i);
		CFile file;
		if (!file.Open(strFileName, CFile::modeRead | CFile::typeBinary))
			break;
		
		// Add Page to Sheet View 
		((CMainFrame*)GetParentFrame())->GetTabDialog()->m_dlgSheetView.AddPage(strPageName, strFileName);
		
		file.Close();
	}
	
	// Auto Open...
	SetTimer(1, 300, NULL);

EFS_END
}

VOID CSIReportEditorView::InitSheetDesigner()
{
EFS_BEGIN

	if (m_pSIReportWorkForm == NULL) return;

	PageSetupProc(m_pSIReportWorkForm->paper);

	((CMainFrame*)GetParentFrame())->GetTabDialog()->m_dlgSheetView.InitSheet();

	POSITION pos = m_pSIReportWorkForm->WorkSheetList.GetHeadPosition();
	while (pos != NULL)
	{
		CSIReportWorkSheet* pSheet = (CSIReportWorkSheet*)m_pSIReportWorkForm->WorkSheetList.GetNext(pos);
		if (pSheet == NULL) continue;
		((CMainFrame*)GetParentFrame())->GetTabDialog()->m_dlgSheetView.AddSheet(pSheet->Alias);
	}
		
	pos = m_pSIReportWorkForm->WorkSheetList.FindIndex(0);
	if (pos != NULL)
	{
		CSIReportWorkSheet *pWorkSheet = (CSIReportWorkSheet*)m_pSIReportWorkForm->WorkSheetList.GetAt(pos);
		if (pWorkSheet == NULL) return;
		
		if (m_ReadBroker.MODE == EDIT_FORM)
			m_sheetDesigner.InitMSheetDesinger(pWorkSheet, TRUE);
		else
			m_sheetDesigner.InitMSheetDesinger(pWorkSheet);

		m_sheetDesigner.SetBitmapIndex(GetBmpIdxOfWorkForm());
		
		m_sheetDesigner.ShowWindow(SW_SHOW);
		m_sheetDesigner.Invalidate();
		m_sheetDesigner.UpdateWindow();
	}

EFS_END
}

VOID CSIReportEditorView::OnDraw(CDC* pDC)
{
EFS_BEGIN

	if (m_sheetDesigner.GetWorkSheet() != NULL)
	{
		CRect rect;
		m_sheetDesigner.GetClientRect(rect);

		CBrush bk_brush;
		bk_brush.CreateSolidBrush(RGB(0, 0, 0));

		CRect rectDraw;
		rectDraw.left = rect.right + m_pointSheet.x;
		rectDraw.top = rect.top + m_pointSheet.y + 5;
		rectDraw.right = rect.right + m_pointSheet.x + 5;
		rectDraw.bottom = rect.bottom + m_pointSheet.y + 5;

		pDC->FillRect(rectDraw, &bk_brush);

		rectDraw.left = rect.left + m_pointSheet.x + 5;
		rectDraw.top = rect.bottom + m_pointSheet.y;
		rectDraw.right = rect.right + m_pointSheet.x + 5;
		rectDraw.bottom = rect.bottom + m_pointSheet.y + 5;

		pDC->FillRect(rectDraw, &bk_brush);

		m_sheetDesigner.Invalidate();
	}

EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// CSIReportEditorView printing

BOOL CSIReportEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
EFS_BEGIN

	// default preparation
	return DoPreparePrinting(pInfo);

EFS_END
return FALSE;

}

VOID CSIReportEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
EFS_BEGIN

	// TODO: add extra initialization before printing

EFS_END
}

VOID CSIReportEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
EFS_BEGIN

	// TODO: add cleanup after printing

EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// CSIReportEditorView diagnostics

#ifdef _DEBUG
VOID CSIReportEditorView::AssertValid() const
{
EFS_BEGIN

	CScrollView::AssertValid();

EFS_END
}

VOID CSIReportEditorView::Dump(CDumpContext& dc) const
{
EFS_BEGIN

	CScrollView::Dump(dc);

EFS_END
}

CSIReportEditorDoc* CSIReportEditorView::GetDocument() // non-debug version is inline
{
EFS_BEGIN

	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSIReportEditorDoc)));
	return (CSIReportEditorDoc*)m_pDocument;

EFS_END
return NULL;

}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSIReportEditorView message handlers
VOID CSIReportEditorView::OnFileNew() 
{
EFS_BEGIN

	if (m_pSIReportWorkForm != NULL)
	{
		INT nID = MessageBox(_T("변경내용을 저장하시겠습니까?"), NULL, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (nID == IDYES)
			OnFileSave();
		else if (nID == IDCANCEL)
			return;
		
		delete m_pSIReportWorkForm;
		m_pSIReportWorkForm = NULL;
	}

	((CMainFrame*)GetParentFrame())->GetTabDialog()->m_dlgSheetView.InitTreeCtrl();

	m_pSIReportWorkForm = new CSIReportWorkForm(MASTER_IS_EDITOR);

	m_strCurrentOpenFileName.Empty();
	m_strReportPK.Empty();
	m_strTitle.Empty();
	m_strDescription.Empty();
	
	CDocument * pDocument;
	if ( NULL != ( pDocument = GetDocument() ) ) 
	{
		pDocument->SetTitle( _T("제목없음") );
	}
	
	// Add Default Sheet
	CSIReportWorkSheet *pWorkSheet = new CSIReportWorkSheet;
	pWorkSheet->Alias = _T("SHEET_01");
	pWorkSheet->Idx = 0;
	pWorkSheet->master_sheet = TRUE;

	m_pSIReportWorkForm->AddNewSheet(pWorkSheet);
	m_pSIReportWorkForm->IncludeList.AddTail((CObject*)pWorkSheet);

	InitSheetDesigner();

	ReCalcAllRect(m_pSIReportWorkForm, 1);
	m_ioMgr.SaveToXML(m_pSIReportWorkForm, m_strTempFileName);
	ReCalcAllRect(m_pSIReportWorkForm, -1);

EFS_END
}

VOID CSIReportEditorView::OnFileNewSheet() 
{
EFS_BEGIN

	if (m_pSIReportWorkForm == NULL) return;

	INT nIdx = m_pSIReportWorkForm->WorkSheetList.GetCount();

	CString strSheetAlias;
	strSheetAlias.Format(_T("SHEET_%02d"), nIdx + 1);
	
	CSIReportWorkSheet *pWorkSheet = new CSIReportWorkSheet;
	pWorkSheet->Alias = strSheetAlias;
	pWorkSheet->Idx = nIdx;
	pWorkSheet->master_sheet = TRUE;
	
	m_pSIReportWorkForm->AddNewSheet(pWorkSheet);
	m_pSIReportWorkForm->IncludeList.AddTail((CObject*)pWorkSheet);

	m_sheetDesigner.InitMSheetDesinger(pWorkSheet);
	((CMainFrame*)GetParentFrame())->GetTabDialog()->m_dlgSheetView.AddSheet(pWorkSheet->Alias);

	ReCalcAllRect(m_pSIReportWorkForm, 1);
	m_ioMgr.SaveToXML(m_pSIReportWorkForm, m_strTempFileName);
	ReCalcAllRect(m_pSIReportWorkForm, -1);

EFS_END
}

VOID CSIReportEditorView::OnFileOpen() 
{
EFS_BEGIN

	CDlgRptFileManage dlg(this);

	dlg.m_sConnection = m_ReadCfgFile.m_sConnection;
	dlg.m_sUserID = m_ReadCfgFile.m_sUserID;
	dlg.m_sSequence = m_ReadCfgFile.m_sSequence;
	
	dlg.m_strOpenAlias= m_strOpenAlias;
	dlg.m_strTempFileName = m_strTempFileName;
	if (dlg.DoModal() == IDOK)
	{
		m_strReportPK = dlg.m_strReportPK;
		m_strTitle = dlg.m_strTitle;
		m_strDescription = dlg.m_strDescription;
		m_strRptGroup = dlg.m_strRptGroup;
		
		// delete current work form and create new workform
		if (m_pSIReportWorkForm != NULL)
		{
			delete m_pSIReportWorkForm;
			m_pSIReportWorkForm = NULL;
		}
		m_pSIReportWorkForm = new CSIReportWorkForm(MASTER_IS_EDITOR);

		CDocument * pDocument;
		if ( NULL != ( pDocument = GetDocument() ) ) {
			pDocument->SetTitle( m_strTitle );
		}
		
		m_pSIReportWorkForm->LoadWorkForm(dlg.m_strOpenFileName);
		ReCalcAllRect(m_pSIReportWorkForm, -1);

		InitSheetDesigner();
	}

EFS_END
}

VOID CSIReportEditorView::OnFileSave() 
{
EFS_BEGIN

	// Hum... for Edit..T.T
	m_sheetDesigner.GetEditWnd()->ShowWindow(SW_HIDE);
	
	// Determine Save to file
	if (m_ReadBroker.MODE == EDIT_DATA)
	{
		SaveToFile();
		return;
	}

	// Save workform to tempfile
	ReCalcAllRect(m_pSIReportWorkForm, 1);
	m_ioMgr.SaveToXML(m_pSIReportWorkForm, m_strTempFileName);
	ReCalcAllRect(m_pSIReportWorkForm, -1);
	
	// show save dialog
	CDlgRptSave dlg;

	dlg.m_sConnection = m_ReadCfgFile.m_sConnection;
	dlg.m_sUserID = m_ReadCfgFile.m_sUserID;
	dlg.m_sSequence = m_ReadCfgFile.m_sSequence;

	dlg.m_strSaveFileName = m_strTempFileName;

	// bitmap
	POSITION posSheet = m_pSIReportWorkForm->WorkSheetList.GetHeadPosition();
	while (posSheet != NULL)
	{
		CSIReportWorkSheet* pSheet = (CSIReportWorkSheet*)m_pSIReportWorkForm->WorkSheetList.GetNext(posSheet);
		if (pSheet == NULL) continue;

		POSITION posObject = pSheet->ObjList.GetHeadPosition();
		while (posObject != NULL)
		{
			CSIReportObject* pObject = (CSIReportObject*)pSheet->ObjList.GetNext(posObject);
			if (pObject != NULL && pObject->GetObjType() == OBJECT_BITMAP)
			{
				CSIReportObject_Bitmap* pBitmap = (CSIReportObject_Bitmap*)pObject;

				CString strBmpFileName;
				strBmpFileName.Format(_T("%s\\%s.bmp"), EDITOR_TEMP_DIR, pBitmap->m_strName);

				CopyFile(pBitmap->file_name, strBmpFileName, FALSE);

				dlg.AddBitmapAlias(pBitmap->m_strName);
			}
		}
	}

	// Save 
	if (!m_strReportPK.IsEmpty()) 
	{
		dlg.m_nMode = MODE_SAVE;
		dlg.m_strReportPK = m_strReportPK;
		dlg.m_strRptGroup = m_strRptGroup;
	}
	// Save As
	else
	{
		dlg.m_strRptGroup = m_strRptGroup;
		dlg.m_nMode = MODE_SAVE_AS;
	}
	
	if (!m_strTitle.IsEmpty()) 
		dlg.m_strTitle = m_strTitle;
	if (!m_strDescription.IsEmpty()) 
		dlg.m_strDescription = m_strDescription;
	
	if (dlg.DoModal() == IDOK)
	{
		m_strReportPK = dlg.m_strReportPK;
		m_strTitle = dlg.m_strTitle;
		m_strDescription = dlg.m_strDescription;	

		CDocument * pDocument;
		if ( NULL != ( pDocument = GetDocument() ) ) 
		{
			pDocument->SetTitle( m_strTitle );
		}
	}

EFS_END
}

VOID CSIReportEditorView::OnFileSaveAs() 
{
EFS_BEGIN

	m_strReportPK.Empty();
	OnFileSave();

EFS_END
}

VOID CSIReportEditorView::OnFileSaveToFile() 
{
EFS_BEGIN

	SaveToFileAs();	

EFS_END
}

// Local File Save / Load
VOID CSIReportEditorView::OpenFromFile(CString strOpenFileName /*=_T("")*/)
{
EFS_BEGIN

	if (strOpenFileName.IsEmpty())
	{
		TCHAR szBuffer[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, szBuffer);

		OPENFILENAME OFN;
		TCHAR lpstrFile[MAX_PATH] = _T("");

		memset(&OFN, 0, sizeof(OPENFILENAME));

		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = m_hWnd;
		OFN.lpstrFilter = _T("RPT파일(*.rpt)\0*.rpt\0모든파일(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("RPT 파일 열기...");
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = MAX_PATH;
		OFN.lpstrDefExt = _T("rpt");
		OFN.Flags = OFN_OVERWRITEPROMPT;

		SetCurrentDirectory(szBuffer);

		if (::GetOpenFileName(&OFN) == 0)
		{
			return;
		}

		strOpenFileName = OFN.lpstrFile;
	}
	else
	{
		strOpenFileName.Format(_T("%s\\%s"), m_strTempDir, strOpenFileName);
	}
			
	if (!strOpenFileName.IsEmpty())
	{

		if (m_pSIReportWorkForm)
		{
			/*INT nID = MessageBox(_T("변경내용을 저장하시겠습니까?"), NULL, MB_YESNOCANCEL | MB_ICONQUESTION);
			/if (nID == IDYES)
			{
				OnFileSave();
			}
			else if (nID == IDCANCEL)
			{
				return;
			}
			*/
			delete m_pSIReportWorkForm;
			m_pSIReportWorkForm = NULL;
		}

		m_pSIReportWorkForm = new CSIReportWorkForm(MASTER_IS_EDITOR);
	
		m_pSIReportWorkForm->LoadWorkForm(strOpenFileName);
		ReCalcAllRect(m_pSIReportWorkForm, -1);

		m_strCurrentOpenFileName = strOpenFileName;

		InitSheetDesigner();
	}

EFS_END
}

VOID CSIReportEditorView::SaveToFile()
{
EFS_BEGIN

	if (m_strCurrentOpenFileName.IsEmpty())
		SaveToFileAs();
	else
	{
		ReCalcAllRect(m_pSIReportWorkForm, 1);
		m_ioMgr.SaveToXML(m_pSIReportWorkForm, m_strCurrentOpenFileName);
		ReCalcAllRect(m_pSIReportWorkForm, -1);
		/***********************************************
		 ***********************************************/
		//20080507 UPDATE BY PJW : 메세지 한글로 수정
		//AfxMessageBox(_T("Save Complete"));
		AfxMessageBox(_T("저장 완료"));
		/***********************************************
		 ***********************************************/
	}

EFS_END
}

VOID CSIReportEditorView::SaveToFileAs()
{
EFS_BEGIN

	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);

	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");

	memset(&OFN, 0, sizeof(OPENFILENAME));

	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = m_hWnd;
	OFN.lpstrFilter = _T("RPT파일(*.rpt)\0*.rpt\0모든파일(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("RPT 파일 저장...");
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = MAX_PATH;
	OFN.lpstrDefExt = _T("rpt");
	OFN.Flags = OFN_OVERWRITEPROMPT;
	
	if (::GetSaveFileName(&OFN) != 0)
	{
		m_strCurrentOpenFileName = OFN.lpstrFile;

		ReCalcAllRect(m_pSIReportWorkForm, 1);
		m_ioMgr.SaveToXML(m_pSIReportWorkForm, m_strCurrentOpenFileName);
		ReCalcAllRect(m_pSIReportWorkForm, -1);
		/***********************************************
		 ***********************************************/
		//20080507 UPDATE BY PJW : 메세지 한글로 수정
		//AfxMessageBox(_T("Save Complete"));
		AfxMessageBox(_T("저장 완료"));
		/***********************************************
		 ***********************************************/
	}

	SetCurrentDirectory(szBuffer);

EFS_END
}

VOID CSIReportEditorView::PageSetupProc(INT nOrientation)
{
EFS_BEGIN

	if (m_pSIReportWorkForm == NULL) return;

	CRect rect;
	m_pSIReportWorkForm->paper = nOrientation;
	m_sheetDesigner.GetClientRect(rect);
	rect.top = 0;			rect.left = 0;
	if (nOrientation == PAPER_A4_LANDSCAPE)
	{
		rect.bottom = EDITOR_PAPER_A4_X;		
		rect.right = EDITOR_PAPER_A4_Y;
	}
	if (nOrientation == PAPER_A4_PORTRAIT)
	{
		rect.bottom = EDITOR_PAPER_A4_Y;		
		rect.right = EDITOR_PAPER_A4_X;
	}
	if (nOrientation == PAPER_B4_LANDSCAPE)
	{
		rect.bottom = EDITOR_PAPER_B4_X;		
		rect.right = EDITOR_PAPER_B4_Y;
	}
	if (nOrientation == PAPER_B4_PORTRAIT)
	{
		rect.bottom = EDITOR_PAPER_B4_Y;		
		rect.right = EDITOR_PAPER_B4_X;
	}
	m_sheetDesigner.SetWindowPos(this, m_pointSheet.x, m_pointSheet.y, rect.Width(), rect.Height(), SWP_NOZORDER);
	RecalcViewSize();

EFS_END
}

VOID CSIReportEditorView::OnTimer(UINT nIDEvent) 
{
EFS_BEGIN

	if (nIDEvent == 1) {
		KillTimer(1);
		OnFileOpen();
		Invalidate();
		m_strOpenAlias.Empty();
		if (m_ReadBroker.MODE == EDIT_DATA)
			((CMainFrame*)GetParentFrame())->GetTabDialog()->m_dlgSheetView.ReadFirstPage();
		else if (m_ReadBroker.MODE == EDIT_FORM)
			((CMainFrame*)GetParentFrame())->GetTabDialog()->m_dlgSheetView.ReadFirstSheet();

		
	}
	
	CScrollView::OnTimer(nIDEvent);

EFS_END
}


/////////////////////////////////////////////////////////////////////////////
// CSIReportEditorView message handlers -- Commands

// Page Setup
VOID CSIReportEditorView::OnFilePageSetup() 
{
EFS_BEGIN

	if (m_pSIReportWorkForm == NULL) return;

	CDlgPageSetup dlg;
	INT nPaperSize = m_pSIReportWorkForm->paper;
	if (m_pSIReportWorkForm->paper == PAPER_A4_LANDSCAPE) {
		dlg.m_strPaper = _T("A4");
		dlg.m_nOrientation = 0;
	}
	if (m_pSIReportWorkForm->paper == PAPER_A4_PORTRAIT) {
		dlg.m_strPaper = _T("A4");
		dlg.m_nOrientation = 1;
	}
	if (m_pSIReportWorkForm->paper == PAPER_B4_LANDSCAPE) {
		dlg.m_strPaper = _T("B4");
		dlg.m_nOrientation = 0;
	}
	if (m_pSIReportWorkForm->paper == PAPER_B4_PORTRAIT) {
		dlg.m_strPaper = _T("B4");
		dlg.m_nOrientation = 1;
	}
	if (dlg.DoModal() != IDOK) return;
	dlg.m_strPaper.TrimLeft();				dlg.m_strPaper.TrimRight();
	dlg.m_strPaper.MakeUpper();
	INT nOrientation = -1;
	if (dlg.m_nOrientation == 0)
	{
		if (dlg.m_strPaper == _T("A4")) nOrientation = PAPER_A4_LANDSCAPE;
		if (dlg.m_strPaper == _T("B4")) nOrientation = PAPER_B4_LANDSCAPE;
	}
	if (dlg.m_nOrientation == 1)
	{
		if (dlg.m_strPaper == _T("A4")) nOrientation = PAPER_A4_PORTRAIT;
		if (dlg.m_strPaper == _T("B4")) nOrientation = PAPER_B4_PORTRAIT;		
	}
	if (nOrientation != nPaperSize) PageSetupProc(nOrientation);	

EFS_END
}

VOID CSIReportEditorView::OnFileDefaultDmSetup() 
{
EFS_BEGIN

	if (m_pSIReportWorkForm == NULL) return;
	
	if (m_sheetDesigner.GetWorkSheet() == NULL) return;

	
	INT nCount = m_arrDefaultDMAlias.GetSize();
	if (nCount <= 0) return;

	CDlgDefaultDMSetup dlg;
	dlg.m_strDefaultDM = m_sheetDesigner.GetWorkSheet()->m_strDefaultDMAlias;
	dlg.SetDefaultDMList(&m_arrDefaultDMAlias);
	
	if (dlg.DoModal() == IDOK)
	{
		m_sheetDesigner.GetWorkSheet()->m_strDefaultDMAlias = dlg.m_strDefaultDM;
	}


EFS_END
}


// Make Same Size
VOID CSIReportEditorView::OnLayoutMakeSameSize() 
{
EFS_BEGIN

	m_sheetDesigner.EditMakeSameSize(MAKE_SAME_SIZE);

EFS_END
}

VOID CSIReportEditorView::OnLayoutMakeSameWidth() 
{
EFS_BEGIN

	m_sheetDesigner.EditMakeSameSize(MAKE_SAME_WIDTH);

EFS_END
}

VOID CSIReportEditorView::OnLayoutMakeSameHeight() 
{
EFS_BEGIN

	m_sheetDesigner.EditMakeSameSize(MAKE_SAME_HEIGHT);

EFS_END
}

// Order
VOID CSIReportEditorView::OnLayoutOrderBringToFront() 
{
EFS_BEGIN

	m_sheetDesigner.EditBringToFront();

EFS_END
}

VOID CSIReportEditorView::OnLayoutOrderSendToBack() 
{
EFS_BEGIN

	m_sheetDesigner.EditSendToBack();

EFS_END
}

VOID CSIReportEditorView::OnLayoutOrderBringForward() 
{
EFS_BEGIN

	m_sheetDesigner.EditBringForward();	

EFS_END
}

VOID CSIReportEditorView::OnLayoutOrderSendBackward() 
{
EFS_BEGIN

	m_sheetDesigner.EditSendBackward();

EFS_END
}

// Align
VOID CSIReportEditorView::OnLayoutAlignLeft() 
{
EFS_BEGIN

	m_sheetDesigner.EditAlignObject(VK_LEFT);

EFS_END
}

VOID CSIReportEditorView::OnLayoutAlignHorzCenter() 
{
EFS_BEGIN

	m_sheetDesigner.EditCenterObject(VK_SHIFT);

EFS_END
}

VOID CSIReportEditorView::OnLayoutAlignRight() 
{
EFS_BEGIN

	m_sheetDesigner.EditAlignObject(VK_RIGHT);

EFS_END
}

VOID CSIReportEditorView::OnLayoutAlignTop() 
{
EFS_BEGIN

	m_sheetDesigner.EditAlignObject(VK_UP);

EFS_END
}

VOID CSIReportEditorView::OnLayoutAlignVertCenter() 
{
EFS_BEGIN

	m_sheetDesigner.EditCenterObject(VK_CONTROL);

EFS_END
}

VOID CSIReportEditorView::OnLayoutAlignBottom() 
{
EFS_BEGIN

	m_sheetDesigner.EditAlignObject(VK_DOWN);

EFS_END
}

// Edit

VOID CSIReportEditorView::OnEditUndo() 
{
EFS_BEGIN

	m_sheetDesigner.EditUndo();

EFS_END
}

VOID CSIReportEditorView::OnEditRedo() 
{
EFS_BEGIN

	m_sheetDesigner.EditRedo();

EFS_END
}

VOID CSIReportEditorView::OnEditCopy() 
{
EFS_BEGIN

	m_sheetDesigner.EditCopyObject();

EFS_END
}

VOID CSIReportEditorView::OnEditCut() 
{
EFS_BEGIN

	m_sheetDesigner.EditCutObject();

EFS_END
}

VOID CSIReportEditorView::OnEditPaste() 
{
EFS_BEGIN

	m_sheetDesigner.EditPastObject();

EFS_END
}

VOID CSIReportEditorView::OnEditSelectAll() 
{
EFS_BEGIN

	m_sheetDesigner.EditSelectAll();

EFS_END
}

VOID CSIReportEditorView::RecalcViewSize()
{
EFS_BEGIN

	CRect rect;
	m_sheetDesigner.GetClientRect(rect);
	
	CSize sizeTotal;
	sizeTotal.cx = m_pointSheet.x * 2 + rect.Width();
	sizeTotal.cy = m_pointSheet.y * 2 + rect.Height();
	SetScrollSizes(MM_TEXT, sizeTotal);
	Invalidate();

EFS_END
}

// Zoom
BOOL CSIReportEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
EFS_BEGIN

	if (m_sheetDesigner.GetWorkSheet() != NULL && GetAsyncKeyState(VK_CONTROL))
	{
		INT nRowsScrolled = 3 * zDelta / 120;
		if (nRowsScrolled > 0)
			OnEditZoomIn();
		else
			OnEditZoomOut();
		
		return TRUE;
	}

	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);

EFS_END
return FALSE;

}

VOID CSIReportEditorView::OnEditZoomIn() 
{
EFS_BEGIN

	if (m_sheetDesigner.GetWorkSheet() != NULL)
	{
		m_sheetDesigner.EditZoomIn();
		
		ScrollToPosition(CPoint(0, 0));
		CRect rect;
		m_sheetDesigner.GetClientRect(rect);
		m_sheetDesigner.SetWindowPos(this, m_pointSheet.x, m_pointSheet.y, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOSIZE);

		RecalcViewSize();
	}

EFS_END
}

VOID CSIReportEditorView::OnEditZoomOut() 
{
EFS_BEGIN

	if (m_sheetDesigner.GetWorkSheet() != NULL)
	{
		m_sheetDesigner.EditZoomOut();
		
		ScrollToPosition(CPoint(0, 0));

		CRect rect;
		m_sheetDesigner.GetClientRect(rect);
		m_sheetDesigner.SetWindowPos(this, m_pointSheet.x, m_pointSheet.y, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOSIZE);
		
		RecalcViewSize();	
	}	

EFS_END
}

VOID CSIReportEditorView::OnTableInsertRow() 
{
EFS_BEGIN

	m_sheetDesigner.EditTableInsertRow();

EFS_END
}

VOID CSIReportEditorView::OnTableInsertCol() 
{
EFS_BEGIN

	m_sheetDesigner.EditTableInsertColumn();

EFS_END
}

VOID CSIReportEditorView::OnTableDeleteRow() 
{
EFS_BEGIN

	m_sheetDesigner.EditTableDeleteRow();	

EFS_END
}

VOID CSIReportEditorView::OnTableDeleteCol() 
{
EFS_BEGIN

	m_sheetDesigner.EditTableDeleteColumn();	

EFS_END
}

VOID CSIReportEditorView::OnTableChangeCol() 
{
EFS_BEGIN

	m_sheetDesigner.EditTableChangeColumn();

EFS_END
}

VOID CSIReportEditorView::OnTableMerge() 
{
EFS_BEGIN

	m_sheetDesigner.EditTableMergeCells();	

EFS_END
}

VOID CSIReportEditorView::OnTableDemerge() 
{
EFS_BEGIN

	m_sheetDesigner.EditTableDeMergeCells();	

EFS_END
}

VOID CSIReportEditorView::OnUpdateFileNewSheet(CCmdUI* pCmdUI) 
{
EFS_BEGIN

	if (m_ReadBroker.MODE != 0 && m_nEditorMode == USER_NORMAL)
		pCmdUI->Enable(FALSE);

EFS_END
}

INT CSIReportEditorView::ReCalcAllRect(CSIReportWorkForm *pWorkForm, INT nMul)
{
	if (pWorkForm == NULL) return -1;
	if (m_pSIReportWorkForm->paper == PAPER_A4_LANDSCAPE) {
		pWorkForm->ReCalcAllRect(nMul*PAPER_A4_MARGIN_X, nMul*PAPER_A4_MARGIN_Y);
	}
	if (m_pSIReportWorkForm->paper == PAPER_A4_PORTRAIT) {
		pWorkForm->ReCalcAllRect(nMul*PAPER_A4_MARGIN_Y, nMul*PAPER_A4_MARGIN_X);
	}
	if (m_pSIReportWorkForm->paper == PAPER_B4_LANDSCAPE) {
		pWorkForm->ReCalcAllRect(nMul*PAPER_B4_MARGIN_X, nMul*PAPER_B4_MARGIN_Y);
	}
	if (m_pSIReportWorkForm->paper == PAPER_B4_PORTRAIT) {
		pWorkForm->ReCalcAllRect(nMul*PAPER_B4_MARGIN_Y, nMul*PAPER_B4_MARGIN_X);
	}
	
	return 0;
}

INT CSIReportEditorView::GetDataMgrList(VOID)
{
	m_arrDefaultDMAlias.RemoveAll();

	CFileFind finder;
	if (finder.FindFile(_T("..\\CFG\\REFERENCE\\DM\\*.UDM")) == FALSE) return -1;

	CString strFileName, strDMAlias;
	INT nPos;
	while(TRUE) {
		if (finder.FindNextFile() == FALSE) break;
	
		strFileName = finder.GetFileName();
		nPos = strFileName.Find(_T(".UDM"), 0);
		if (nPos == -1) continue;
		strDMAlias = strFileName.Mid(0, nPos);
		strDMAlias.TrimLeft();			strDMAlias.TrimRight();

		m_arrDefaultDMAlias.Add(strDMAlias);
	}
	return 0;
}