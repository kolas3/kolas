// RCView.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "KOLAS3_MAINDoc.h"
#include "RCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRCView

IMPLEMENT_DYNCREATE(CRCView, CFormView)

CRCView::CRCView()
	: CFormView(CRCView::IDD)
{
	//{{AFX_DATA_INIT(CRCView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bmpMain.LoadBMP(_T("..\\bmp\\logo.bmp"));

	m_pMainApi = new CKOLAS3_MAINApi(this);
	//m_pInfo = new CESL_Information;

	m_pForm = NULL;
	m_pListToolBarGroup = NULL;
	m_pListToolBarItem = NULL;

	m_pRCDlg = NULL;
}

CRCView::~CRCView()
{
	if (m_pMainApi)
	{
		delete m_pMainApi;
		m_pMainApi = NULL;
	}
	if (m_pForm)
	{
		delete m_pForm;
		m_pForm = NULL;
	}
	FreeImageList();
	FreeModelessDlg();
	FreeToolBarList();
	FreeToolTipList();
}

VOID CRCView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRCView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRCView, CFormView)
	//{{AFX_MSG_MAP(CRCView)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DESTROY_MODELESS, OnDestroyModelessDlg)
	ON_MESSAGE(USER_TOOLBAR_HIDE_SHOW, OnShowHideToolBarButton)
	ON_MESSAGE(WM_TOOLBAR_EXE, OnExecuteToolBarButton)
	ON_MESSAGE(WM_SET_ACCELERATOR, OnSetAcceleratorDialog)
	ON_MESSAGE(WM_SHOW_RC_DLG, OnShowRCDlg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRCView diagnostics

#ifdef _DEBUG
VOID CRCView::AssertValid() const
{
	CFormView::AssertValid();
}

VOID CRCView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CKOLAS3_MAINDoc* CRCView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKOLAS3_MAINDoc)));
	return (CKOLAS3_MAINDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRCView Operations

VOID CRCView::FreeImageList()
{
	INT nCount = m_listImage.GetCount();
	if (nCount == 0) return;
	CImageList *pImageList;
	for (INT i = 0; i < nCount; i++) 
	{
		pImageList = NULL;
		pImageList = (CImageList*)m_listImage.RemoveHead();
		if (pImageList) delete pImageList;
	}
	m_listImage.RemoveAll();
	return;
}

VOID CRCView::FreeToolTipList()
{
	INT nCount = m_listToolTip.GetCount();
	if (nCount == 0) return;
	CToolTipCtrl *pToolTipCtrl;
	for (INT i = 0; i < nCount; i++) 
	{
		pToolTipCtrl = NULL;
		pToolTipCtrl = (CToolTipCtrl*)m_listToolTip.RemoveHead();
		if (pToolTipCtrl) delete pToolTipCtrl;
	}
	m_listToolTip.RemoveAll();
	return;
}

VOID CRCView::FreeToolBarList()
{
	INT nCount = m_listToolBar.GetCount();
	if (nCount == 0) return;
	CToolBar *pToolBar;
	for (INT i = 0; i < nCount; i++) 
	{
		pToolBar = NULL;
		pToolBar = (CToolBar*)m_listToolBar.RemoveHead();
		if (pToolBar) delete pToolBar;
	}
	m_listToolBar.RemoveAll();
	return;
}

VOID CRCView::FreeModelessDlg()
{
	INT nCount = m_listModelessDlg.GetCount();
	if (nCount == 0) return;
	CModelessDlg *pModelessDlg;
	for (INT i = 0; i < nCount; i++) 
	{
		pModelessDlg = NULL;
		pModelessDlg = (CModelessDlg*)m_listModelessDlg.RemoveHead();
		if (pModelessDlg) delete pModelessDlg;
	}
	m_listModelessDlg.RemoveAll();
	return;
}

VOID CRCView::SetParentInfo(CString strMenuAlias, CString strTargetDll, CString strCaption, CObList *pListToolBarGroup, CObList *pListToolBarItem)
{
	m_strMenuAlias = strMenuAlias;
	m_strTargetDll = strTargetDll;
	m_strCaption = strCaption;

	m_pListToolBarGroup = pListToolBarGroup;
	m_pListToolBarItem = pListToolBarItem;
}

VOID CRCView::SetToolBarGroup(CString strMenuAlias)
{
	if (m_pListToolBarGroup == NULL) return;

	CFrameWnd *pFrame = GetParentFrame();
	if (pFrame == NULL) return;
	
	// 툴바 관련 초기화
	FreeImageList();
	FreeToolBarList();
	
	// 그룹에 맞게 툴바 생성
	CToolBarGroup *pToolBarGroup = NULL;
	CToolBar *pToolBar = NULL;

	POSITION pos = m_pListToolBarGroup->GetHeadPosition();
	while (pos)
	{
		pToolBarGroup = (CToolBarGroup*)m_pListToolBarGroup->GetNext(pos);
		if (pToolBarGroup == NULL) continue;
		if (pToolBarGroup->m_strMenuAlias != strMenuAlias) continue;

		pToolBar = m_pMainApi->CreateToolBar(pFrame, pToolBarGroup->m_nDockBarID);
		if (pToolBar == NULL) continue;

		m_listToolBar.AddTail((CObject *)pToolBar);


		m_pMainApi->SetToolBar(pFrame, strMenuAlias, pToolBarGroup->m_strAlias, pToolBar, m_pListToolBarItem, &m_listToolTip, &m_listImage);

	}
	
	// m_pInfo 에 세팅
	m_pInfo->m_pListToolBar = &m_listToolBar;
}

INT CRCView::LoadDll(CString strDllName)
{
	if (m_pForm)
	{
		delete m_pForm;
		m_pForm = NULL;
	}

/*
	CFile file;
	if (!file.Open(strDllName, CFile::modeRead)) 
	{
		CString msg;
		msg.Format(_T("지정된 경로에서 %s 파일을 찾을 수 없습니다."), strDllName);
		MessageBox(msg);
		return -1;
	}
	file.Close();
*/
	CFileFind fd;
	if (fd.FindFile(strDllName) == FALSE) {
		CString msg;
		msg.Format(_T("지정된 경로에서 %s 파일을 찾을 수 없습니다."), strDllName);
		MessageBox(msg);
		return -1;
	}
	
	m_pForm = new CForm;
	if (!m_pForm->CreateForm(this, strDllName.GetBuffer(0), m_pInfo, 'R', m_strMenuAlias)) return -1;
	
	CRect rect;
	GetClientRect(rect);
	m_pForm->ResizeForm(rect);

	return 0;
}

VOID CRCView::Init()
{
	if (m_strMenuAlias.IsEmpty() ||
		m_strTargetDll.IsEmpty() ||
		m_strCaption.IsEmpty() ||
		m_pListToolBarGroup == NULL ||
		m_pListToolBarItem == NULL
		)
		return;
		
	// Load Menu DLL
	FreeToolBarList();
	GetParentFrame()->RecalcLayout();
	FreeImageList();

	// Set ToolBar
	SetToolBarGroup(m_strMenuAlias);
	if (LoadDll(m_strTargetDll) < 0) return;
	GetParentFrame()->SetMessageText(m_strCaption);
	GetParentFrame()->RecalcLayout();
}

VOID CRCView::CreateModelessDlg(CToolBarItem *pToolBarItem)
{
	if (!m_pForm) return;

	// check dialog
	POSITION pos = m_listModelessDlg.GetHeadPosition();
	while (pos)
	{
		CModelessDlg *pModelessDlg = (CModelessDlg*)m_listModelessDlg.GetNext(pos);
		if (pToolBarItem->m_strSM == pModelessDlg->m_strSM && pToolBarItem->m_strAlias == pModelessDlg->m_strButtonAlias)
		{
			pModelessDlg->m_pESL_Mgr->ShowWindow(SW_SHOW);
			pModelessDlg->m_pESL_Mgr->SetFocus();
			return;
		}
	}
	
	CESL_Mgr *pESL_Mgr = m_pForm->CreateModelessDlg(pToolBarItem->m_strSM, pToolBarItem->m_strAlias, pToolBarItem->m_strType == _T("R") ? TRUE : FALSE);
	if (pESL_Mgr != NULL)
	{
		CModelessDlg *newModelessDlg = new CModelessDlg;
		newModelessDlg->m_pESL_Mgr = pESL_Mgr;
		newModelessDlg->m_strSM = pToolBarItem->m_strSM;
		newModelessDlg->m_strButtonAlias = pToolBarItem->m_strAlias;
		m_listModelessDlg.AddTail((CObject*)newModelessDlg);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CRCView message handlers

VOID CRCView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
//	SetScrollSizes(MM_TEXT,CSize(1,1));	
}

VOID CRCView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if (!m_pForm)
	{		
		RECT r;
		GetClientRect(&r);
		CRect img_rect = m_bmpMain.GetImageSize();
		INT x, y;
		y = r.bottom - r.top - img_rect.Height();
		y /= 2;
		x = r.right - r.left - img_rect.Width();
		x /= 2;
		m_bmpMain.DrawDIB(&dc, x, y);		
		//m_bmpMain.DrawDIB(&memDC, x, y);
	}
	
	// Do not call CFormView::OnPaint() for painting messages
}

UINT CRCView::OnDestroyModelessDlg(WPARAM wParam, LPARAM lParam)
{
	CESL_Mgr *pESL_Mgr;
	CModelessDlg *pModelessDlg;
	pESL_Mgr = (CESL_Mgr*)wParam;
	if (!pESL_Mgr) return 0;

	POSITION pos = m_listModelessDlg.GetHeadPosition();
	while (pos)
	{
		POSITION oldPos = pos;
		pModelessDlg = (CModelessDlg*)m_listModelessDlg.GetNext(pos);

		if (pESL_Mgr == pModelessDlg->m_pESL_Mgr)
		{
			delete pModelessDlg;
			pModelessDlg = NULL;
			m_listModelessDlg.RemoveAt(oldPos);
			return 0;
		}
	}
	return 0;
}

BOOL CRCView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// toolbar
	if (m_pListToolBarItem)
	{
		POSITION pos = m_pListToolBarItem->GetHeadPosition();
		while (pos)
		{
			CToolBarItem *pToolBarItem = (CToolBarItem*)m_pListToolBarItem->GetNext(pos);
			if (pToolBarItem && (UINT)wParam == pToolBarItem->m_nID)
			{
				if( pToolBarItem->m_strAlias==_T("도구모음"))
				{
					return TRUE;
				}
			
				//TRACE(_T("%d, %s\n"), pToolBarItem->m_nID, pToolBarItem->m_strCaption);

				CreateModelessDlg(pToolBarItem);
				return TRUE;
			}

		}
	}

	return CFormView::OnCommand(wParam, lParam);
}


BOOL CRCView::OnCmdMsg(UINT nID, INT nCode, VOID* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if (m_pListToolBarItem)
	{
		// check toolbar id
		POSITION pos = m_pListToolBarItem->GetHeadPosition();
		while (pos)
		{
			CToolBarItem *pToolBarItem = (CToolBarItem*)m_pListToolBarItem->GetNext(pos);
			if (pToolBarItem && nID == pToolBarItem->m_nID && pToolBarItem->m_strType == _T("D"))
				return FALSE;
			if (pToolBarItem && nID == pToolBarItem->m_nID)
				return TRUE;
		}
	}

	return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

VOID CRCView::OnSize(UINT nType, INT cx, INT cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	if (m_pForm)
	{
		CRect rect;
		GetClientRect(rect);
		
	
		if (m_pForm->m_pCreateFun == NULL) return;
		if (m_pForm->m_pResizeFun == NULL) return;

		m_pForm->ResizeForm(rect);
		
	
	}	
}

UINT CRCView::OnShowHideToolBarButton(WPARAM wParam, LPARAM lParam)
{
	if ((CString*) wParam == NULL || lParam < 0) return -1;
	CString strToolBarButtonAlias;
	strToolBarButtonAlias.Format(_T("%s"), ((CString*)wParam)->GetBuffer(0));
	BOOL bHide = (BOOL)lParam;

	m_pMainApi->HideShowToolBarButton(strToolBarButtonAlias, m_strMenuAlias, m_pListToolBarGroup, m_pListToolBarItem, &m_listToolBar, bHide);
	GetParentFrame()->RecalcLayout();

	return 0;
}

UINT CRCView::OnExecuteToolBarButton(WPARAM wParam, LPARAM lParam)
{
	if ((CString*)wParam == NULL) return -1;
	CString strToolBarButtonAcce;
	strToolBarButtonAcce.Format(_T("%s"), (TCHAR*)wParam);

	CString strToolBarButtonAlias = m_pMainApi->GetAcceToolBarButton(strToolBarButtonAcce, m_strMenuAlias, m_pListToolBarGroup, m_pListToolBarItem, &m_listToolBar);
	if (strToolBarButtonAlias.GetLength() == 0) return 0;
	m_pMainApi->ExecuteToolBarButton(strToolBarButtonAlias, m_strMenuAlias, m_pListToolBarGroup, m_pListToolBarItem, &m_listToolBar);
	GetParentFrame()->RecalcLayout();

	return 0;
}

UINT CRCView::OnSetAcceleratorDialog(WPARAM wParam, LPARAM lParam)
{
	m_pMainApi->m_strCONNECTION_INFO = m_pInfo->CONNECTION_INFO;
	INT ids = m_pMainApi->OnSetAcceleratorDialog(m_strMenuAlias, m_pListToolBarGroup, m_pListToolBarItem, &m_listToolBar);

	return 0;
}

VOID CRCView::ShowRCDlg()
{
	if (m_pRCDlg == NULL) return;
	if (m_pRCDlg->GetSafeHwnd())
	{
		m_pRCDlg->ShowWindow(SW_SHOW);
		m_pRCDlg->BringWindowToTop();
	}
}

UINT CRCView::OnShowRCDlg(WPARAM wParam, LPARAM lParam)
{
	if (m_pMainApi == NULL) return 0;
	ShowRCDlg();
	return 0;
}
