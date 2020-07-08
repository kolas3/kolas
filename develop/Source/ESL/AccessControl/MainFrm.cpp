// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "AccessControl.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "AccessControlDoc.h"
#include "AccessControlView.h"
#include "AccessControlWorkView.h"

#include "AccessControlUserSelectDlg.h"
#include "AccessControlNewMethodDlg.h"
#include "AccessControlDeleteMethodDlg.h"
#include "AccessControlDeleteUserDlg.h"
#include "AccessControlChangeMethodDlg.h"
#include "AccessControlUserDuplicate.h"

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_METHOD_NEW, COMMAND_MethodNew)
	ON_COMMAND(ID_METHOD_DELETE, COMMAND_MethodDelete)
	ON_COMMAND(ID_USER_NEW, COMMAND_UserNew)
	ON_COMMAND(ID_USER_DELETE, COMMAND_UserDelete)
	ON_COMMAND(ID_USER_METHOD, COMMAND_UserChangeMethod)
	ON_COMMAND(ID_LOGIN_NEW, COMMAND_NewLogin)
	ON_COMMAND(ID_USER_DUPLICATE, COMMAND_UserDuplicate)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	BUILD = _T("build. 1");
	// KOL.UDF.022
	FLAG = ' ';
	NAME = _T("");
}

CMainFrame::~CMainFrame()
{
}

INT CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	/*
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	*/
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE; 
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
VOID CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

VOID CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// CG: The following block was added by the Splitter Bar component.
	{
		// 화면을 분할 하고
		if (!m_wndSplitter.CreateStatic(this, 1, 2))
		{
			TRACE0("Failed to create splitter bar ");
			return FALSE;    // failed to create
		}

		m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CAccessControlView), CSize(270, 200), pContext);
		m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CAccessControlWorkView), CSize(200, 200), pContext);

		// 사용자 선택 화면을 띄운다.
		SetTimer(0, 300, NULL);

		return TRUE;
	}
}

VOID CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0) {
		KillTimer(0);
		CAccessControlUserSelectDlg dlg;
		if (dlg.DoModal() == IDOK) {
			if (dlg.NAME.GetLength() > 0) Login(dlg.NAME, dlg.FLAG);
		} else {

		}
	}
	CFrameWnd::OnTimer(nIDEvent);
}

CView * CMainFrame::GetFirstView(VOID)
{
	CDocument *pDoc = GetActiveDocument();
	if (!pDoc) return NULL;
	POSITION pd =  pDoc->GetFirstViewPosition();
	return pDoc->GetNextView(pd);
}

CView * CMainFrame::GetSecondView(VOID)
{
	CDocument *pDoc = GetActiveDocument();
	if (!pDoc) return NULL;
	POSITION pd =  pDoc->GetFirstViewPosition();
	CView * pView = pDoc->GetNextView(pd);
	if (!pView) return NULL;
	return pDoc->GetNextView(pd);
}

VOID CMainFrame::COMMAND_MethodNew(VOID)
{
	CAccessControlNewMethodDlg dlg;
	dlg.FLAG = 'M';
	if (dlg.DoModal() != IDOK) return;
	Login(dlg.m_sName, dlg.FLAG);

	return;
}

VOID CMainFrame::COMMAND_UserNew(VOID)
{
	CAccessControlNewMethodDlg dlg;
	dlg.FLAG = 'U';
	if (dlg.DoModal() != IDOK) return;

	Login(dlg.m_sName, dlg.FLAG);
	return;
}

VOID CMainFrame::Login(CString name, TCHAR flag)
{
	FLAG = flag;
	NAME = name;

	CString title;


	if (name == _T("Administrator")) title.Format(_T("[%s]AccessControl [Administrator] "), BUILD);
	else if (flag == 'M') title.Format(_T("[%s]AccessControl [METHOD] - %s"), BUILD, name);
		 else title.Format(_T("[%s]AccessControl [USER] - %s"), BUILD, name);

	SetWindowText(title);

	// 메소드와를 선택한경우는 해당 메소드를 사용하는 사용자의 리스트를 읽어온다.
	// 관리자일 경우 메소드 와 해당 사용 자를 모두 읽어 온다 
	if (flag == 'M' || flag == 'S') UserList.Refresh(name);

	// 이제 Menu Group List를 뿌린다.
	CDocument *pDoc = GetActiveDocument();
	CAccessControlView * pView;
	POSITION pd =  pDoc->GetFirstViewPosition();
	pView = (CAccessControlView *)pDoc->GetNextView(pd);
	pView->USER_FLAG = flag;
	pView->ShowMenuGroupList();

	CAccessControlWorkView *pWorkView;
	pWorkView = (CAccessControlWorkView *)pDoc->GetNextView(pd);
	pWorkView->pUserList = &UserList;
}

VOID CMainFrame::COMMAND_UserDelete(VOID)
{
	CAccessControlDeleteUserDlg dlg;
	dlg.DoModal();
	return;
}

VOID CMainFrame::COMMAND_MethodDelete(VOID)
{
	CAccessControlDeleteMethodDlg dlg;
	dlg.DoModal();
	return;
}

VOID CMainFrame::COMMAND_UserChangeMethod(VOID)
{
	if (FLAG != 'U') {
		AfxMessageBox(_T("User만이 Method를 변경할 수 있습니다. 먼저 변경할 유져로 로그인 해주세요."));
		return;
	}
	CAccessControlChangeMethodDlg dlg;
	dlg.m_sUSER_ID = NAME;
	dlg.DoModal();
	return;
}

VOID CMainFrame::COMMAND_NewLogin(VOID)
{
	CAccessControlUserSelectDlg dlg;
	if (dlg.DoModal() == IDOK) {
		if (dlg.NAME.GetLength() > 0) Login(dlg.NAME, dlg.FLAG);
	} else {

	}
	return;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CFrameWnd::PreTranslateMessage(pMsg);
}

VOID CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CESL_DataMgr dm1;
	dm1.ORACLE_DisconnectDB();
	CFrameWnd::OnClose();
}

VOID CMainFrame::COMMAND_UserDuplicate(VOID)
{
	CAccessControlUserDuplicate dlg;
	
	dlg.DoModal();

	return;
}