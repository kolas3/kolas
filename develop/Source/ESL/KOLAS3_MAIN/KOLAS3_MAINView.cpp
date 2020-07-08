// KOLAS3_MAINView.cpp : implementation of the CKOLAS3_MAINView class
//
#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "KOLAS3_MAINDoc.h"
#include "KOLAS3_MAINView.h"
#include "mainfrm.h"
#include "LoginDlg.h"
#include "..\ESL\ESL_UnicodeFileIO.h"
#include "..\ESL\ESL_MBCSFileIO.h"
#include "EFS.h"
#include "..\..\공통\IndexToolKit\IndexTool.h"
#include "KIISystemInfo.h"
#include "..\..\공동목록\UC_NOTIFY_POPUP\UC_NotifyPopupDlg.h"
#include "UC_KOLISNetUseChecker.h"
#include "..\\..\\공동목록\\KOLISNetWebSearch\\KOLISNetWebSearchMgr.h"
#include "MToolBar.h"
#include "ECO_FILE_API.h"
#include "ECO_FileIO.h"
#include "BalloonHelp.h"
#include "NetworkInfoMgr.h"
#include "RegistryEx.h"
#include "FWDES.h" // DES 암복호화
#include "..\\..\\단행\\열람\\장서관리\\Loc_macro\\errormacro.h"

// 2012.10.05 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2회
// 비밀번호 갱신기간 만료
#include "RENEWAL_PASSWORD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BMP_SIZE 20
#define BMP_COUNT 32
#define USER_BSIZE_WIDE 55
#define USER_BSIZE_HIGHT 39
#define USER_SHOW_MENU 31000 

// << 2015년 공공도서관 시스템 개선 2015/11/13 _LCM : 버전
#define VERSION_MAX_SIZE 512
#pragma comment(lib,"KOLASIIIVer.lib")
extern "C" __declspec(dllimport) bool GetKOLASIIIVer(wchar_t*);
extern "C" __declspec(dllimport) bool GetKOLASIIIBuildDate(wchar_t*);
// >> 2015/11/13 --ADD

CKOLISNetWebSearchMgr *m_KolisNetWebSearch;
/////////////////////////////////////////////////////////////////////////////
// CKOLAS3_MAINView
IMPLEMENT_DYNCREATE(CKOLAS3_MAINView, CFormView)

BEGIN_MESSAGE_MAP(CKOLAS3_MAINView, CFormView)
//{{AFX_MSG_MAP(CKOLAS3_MAINView)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_WM_DESTROY()	
	ON_NOTIFY(TCN_SELCHANGE, IDC_ctlFormTab, OnSelchangectlFormTab)
	ON_WM_MOVE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_NCACTIVATE()	
//}}AFX_MSG_MAP
// Standard printing commands
//ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
//ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
//ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
ON_MESSAGE(WM_DESTROY_MODELESS, OnDestroyModelessDlg)
//ON_MESSAGE(RC_SHOW, OnRCShowRCFrame)
ON_MESSAGE(RC_GET_CUR_MENU, OnRCGetCurMenu)
ON_MESSAGE(USER_TOOLBAR_HIDE_SHOW, OnShowHideToolBarButton)
ON_MESSAGE(WM_TOOLBAR_EXE, OnExecuteToolBarButton)
ON_MESSAGE(WM_SET_ACCELERATOR, OnSetAcceleratorDialog)
ON_MESSAGE(WM_SHOW_RC_DLG, OnShowRCDlg)
ON_MESSAGE(WM_USER + 1009, OnChangeTab)
ON_MESSAGE(WM_USER + 1010, OnInternet)
ON_MESSAGE(WM_USER + 1011, OnCloseForm)	
ON_MESSAGE(WM_USER + 1014, OnKolisnetSearch)	
ON_MESSAGE(WM_USER + 1015, OnWebTitle)	
ON_MESSAGE(WM_USER + 1016, OnKolisnetSearchStart)	
ON_MESSAGE(WM_USER + 1017, OnChangeURL)		
ON_MESSAGE(WM_USER + 1018, OnKolisnetSearchFind)		
ON_MESSAGE(WM_USER + 1100, OnExplorerPrev)	
ON_MESSAGE(WM_USER + 1101, OnExplorerNext)	
ON_MESSAGE(WM_USER + 1102, OnExplorerReflash)	
ON_MESSAGE(WM_USER + 1103, OnExplorerStop)	
ON_MESSAGE(WM_USER + 1020, OnKolisnetWebSearchShowHide)	
//ON_MESSAGE(WM_USER + 1021, OnKolisnetWebSearchHide)	
ON_MESSAGE(WM_USER + 1024, OnKolisnetWebSearchStart)
ON_MESSAGE(WM_USER + 1025, OnKolisnetWebSearchCount)	
ON_MESSAGE(WM_USER + 1026, OnFurnishWebSearch)	
ON_MESSAGE(WM_USER + 1028, OnAddModelessDlgList)
ON_MESSAGE(WM_USER + 1029, OnChangeUnityLoanServiceStatus)
ON_MESSAGE(WM_USER + 87  , OnExecuteItem)
ON_MESSAGE(WM_USER + 89  , OnExecuteWebMoveItem)
ON_MESSAGE(WM_USER + 90  , OnExecuteWebItem)
ON_MESSAGE(WM_USER + 99  , KOLAS_LOCK)
ON_MESSAGE(WM_USER + 4002, OnGetESLMGR)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKOLAS3_MAINView construction/destruction

// << 2015년 공공도서관 시스템 개선 2015/11/11 _LCM : 자동로그아웃기능
typedef struct tagKBDLLHOOKSTRUCT {
    DWORD     vkCode;
    DWORD     scanCode;
    DWORD     flags;
    DWORD     time;
//    ULONG_PTR dwExtraInfo;
} KBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;

#define WH_KEYBOARD_LL	13
#define WH_MOUSE_LL		14
extern CKOLAS3_MAINApp theApp;
LRESULT CALLBACK GetMsgProcKey(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GetMsgProcMouse(int nCode, WPARAM wParam, LPARAM lParam);
HHOOK g_hookKey = NULL;
HHOOK g_hookMouse = NULL;
// >> 2015/11/11 --ADD

CKOLAS3_MAINView::CKOLAS3_MAINView()
: CFormView(CKOLAS3_MAINView::IDD)
{	
	//	SetClassLong(this->GetSafeHwnd(), 0 , (LONG)this);
	m_pInfo = new CESL_Information;
	
	// IndexToolKit을 붙여준다.
	CIndexTool *pIndexTool = new CIndexTool;
	m_pInfo->pIndexTool = (CObject*)pIndexTool;
	
	m_nInitID = 32001;
	m_LoadTool = false;	
	
	m_pMainApi = new CKOLAS3_MAINApi(this);
	m_ScreenSize.cx = -1;
	m_ScreenSize.cy = -1;
		
	//2004-07-07 이학중 
	m_bDownLoader = FALSE ;
	
	// 
	m_nTabIndex = 0;
	m_nMenuFormCount = 0;
	m_nExplorerCount = 0;
	m_nToolBarCount = 0;
	
	INT i;
	for ( i=0 ; i<100 ; i++ )
	{
		m_pForm[i] = NULL;
	}
	
	for ( i=0 ; i<100 ; i++ )
	{
		m_Explorer[i] = NULL;
	}
	
	m_TabList.RemoveAll();	
	m_KolisNetWebSearch = NULL;
	m_bKolisnetWebSearchFirstShow = TRUE;
	
	m_nMenuItemCount = 0;	
	m_bLock = FALSE;
	m_pLockForm = NULL;
	m_strFurnishISBN = _T("");
	m_sGroupName = _T("10");
	
	m_bUserLogin = FALSE;

	m_bIsRunModal = FALSE;
	m_bIsRunTestUnityLoanServiceThread = FALSE;
	m_pThread = NULL;
	// << 2015년 공공도서관 시스템 개선 2015/11/11 _LCM : 자동로그아웃기능
	m_pTimeOut = NULL;
	m_nTimeOut = 0;
	m_nTimeOutMax = 0;
	m_bTimeOut = FALSE;
	// >> 2015/11/11 --ADD
	// 18.11.07 JMJ KOL.RED.2018.020
	m_MainExplorer = NULL;

}

CKOLAS3_MAINView::~CKOLAS3_MAINView()
{	
	if (m_pInfo) 
	{
		if (m_pInfo->pIndexTool) {
			CIndexTool *pIndexTool = (CIndexTool*)m_pInfo->pIndexTool;
			delete pIndexTool;
			m_pInfo->pIndexTool = NULL;
		}
		delete m_pInfo;
		m_pInfo = NULL;
	}
	
	if  ( m_KolisNetWebSearch != NULL )
	{
		delete m_KolisNetWebSearch;
	}
	
	if ( m_pLockForm != NULL ) 
	{
		delete m_pLockForm;	
		m_pLockForm = NULL;
	}
	
	if (m_pMainApi)
	{
		delete m_pMainApi;
		m_pMainApi = NULL;
	}
	
	INT nCount = m_TabList.GetCount();
	TAB_ITEMS_STRU *tabitem_stru;
	POSITION pos;
	for ( int i=nCount-1 ; i >= 0 ; i-- )
	{
		tabitem_stru = NULL;
		pos = m_TabList.FindIndex(i);
		if ( pos == NULL ) continue;
		
		tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
		if ( tabitem_stru->nItemType == _MENU_FORM )
		{
			FreeModelessDlg(tabitem_stru->nFormIndex);
			if(tabitem_stru->pEslMgr!=NULL)
			{
				tabitem_stru->pEslMgr = NULL;
			}
			delete m_pForm[tabitem_stru->nFormIndex];
			m_pForm[tabitem_stru->nFormIndex] = NULL;
		}
		else if ( tabitem_stru->nItemType == _INET_FORM )
		{
			delete m_Explorer[tabitem_stru->nInternetIndex];
			m_Explorer[tabitem_stru->nInternetIndex] = NULL;
		}
		// 18.11.07 JMJ KOL.RED.2018.020
		else if ( tabitem_stru->nItemType == _MAIN_FORM )
		{
			delete m_MainExplorer;
			m_MainExplorer = NULL;
		}
		if ( tabitem_stru !=NULL ) 
		{
			m_TabList.RemoveAt(pos);
			delete tabitem_stru;
		}
	}
	
	m_TabList.RemoveAll();
	
	FreeSubMenuList();
	FreeImageList();
	FreeToolTipList();
	FreeMenuItem();	
	FreeToolBarList();	
	FreeToolBarItem();		
	//FreeModelessDlg();	
	FreeToolBarGroup();
	
	// 2009.03.11 ADD BY CJY : 여러개의 관리구분으로 로그인 할 경우를 위해 PID로 구분한다.
	DWORD dwPID = ::GetCurrentProcessId();
	CString strFileName;
	strFileName.Format(_T("..\\CFG\\EFS\\user_info(%u).log"), dwPID);	
	::DeleteFile(strFileName);

	CFileFind ff;
	BOOL bFind = ff.FindFile(_T("../대출증/이용자사진/*.*"));
	while (bFind)
	{		
		bFind = ff.FindNextFile();
		
		CString strFileName = ff.GetFileName();
		if (strFileName.CompareNoCase(_T("."))  == 0 ||
			strFileName.CompareNoCase(_T("..")) == 0 )
			continue;

		CString strFilePath = ff.GetFilePath();
		::DeleteFile(strFilePath);
	}

// << 2015년 공공도서관 시스템 개선 2015/11/11 _LCM : 자동로그아웃기능
	if(m_pTimeOut != NULL)
	{
		WaitForSingleObject(m_pTimeOut->m_hThread, 0);	

		delete m_pTimeOut;
		m_pTimeOut = NULL;
	}

	UnhookWindowsHookEx(g_hookKey);
	UnhookWindowsHookEx(g_hookMouse);
// >> 2015/11/11 --ADD

	CKIISystemInfo::Free();
}

void CKOLAS3_MAINView::FreeImageList()
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

void CKOLAS3_MAINView::FreeToolTipList()
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

void CKOLAS3_MAINView::FreeSubMenuList()
{
	INT nCount = m_listSubMenu.GetCount();
	if (nCount == 0) return;
	CMenu *pMenu;
	for (INT i = 0; i < nCount; i++) 
	{
		pMenu = NULL;
		pMenu = (CMenu*)m_listSubMenu.RemoveHead();
		if (pMenu) delete pMenu;
	}
	m_listSubMenu.RemoveAll();
	return;
}

void CKOLAS3_MAINView::FreeToolBarList()
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

VOID CKOLAS3_MAINView::FreeModelessDlg(INT nFormIndex)
{
	INT nCount = m_listModelessDlg[nFormIndex].GetCount();
	if (nCount == 0) return;
	CModelessDlg *pModelessDlg;
	for (INT i = 0; i < nCount; i++) 
	{
		pModelessDlg = NULL;
		pModelessDlg = (CModelessDlg*)m_listModelessDlg[nFormIndex].RemoveHead();
		if (pModelessDlg) 
		{				
			delete pModelessDlg;
			pModelessDlg = NULL;
		}
	}
	m_listModelessDlg[nFormIndex].RemoveAll();
	return;
}

void CKOLAS3_MAINView::FreeMenuItem()
{
	INT nCount = m_listMenuItem.GetCount();
	if (nCount == 0) return;
	CMenuItem *pMenuItem;
	for (INT i = 0; i < nCount; i++) 
	{
		pMenuItem = NULL;
		pMenuItem = (CMenuItem*)m_listMenuItem.RemoveHead();
		if (pMenuItem) delete pMenuItem;
	}
	m_listMenuItem.RemoveAll();
	return;
}

void CKOLAS3_MAINView::FreeToolBarItem()
{
	INT nCount = m_listToolBarItem.GetCount();
	if (nCount == 0) return;
	CToolBarItem *pToolBarItem;
	for (INT i = 0; i < nCount; i++) 
	{
		pToolBarItem = NULL;
		pToolBarItem = (CToolBarItem*)m_listToolBarItem.RemoveHead();
		if (pToolBarItem) delete pToolBarItem;
	}
	m_listToolBarItem.RemoveAll();
	return;
}

void CKOLAS3_MAINView::FreeToolBarGroup()
{
	INT nCount = m_listToolBarGroup.GetCount();
	if (nCount == 0) return;
	CToolBarGroup *pToolBarGruop;
	for (INT i = 0; i < nCount; i++) 
	{
		pToolBarGruop = NULL;
		pToolBarGruop = (CToolBarGroup*)m_listToolBarGroup.RemoveHead();
		if (pToolBarGruop) delete pToolBarGruop;
	}
	m_listToolBarGroup.RemoveAll();
	return;
}

VOID CKOLAS3_MAINView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKOLAS3_MAINView)
	DDX_Control(pDX, IDC_ctlFormTab, m_ctlFormTab);
	// 18.11.07 JMJ KOL.RED.2018.020
	//DDX_Control(pDX, IDC_FLASH_MAIN, m_ctrlMainFlash);
	//}}AFX_DATA_MAP
}

BOOL CKOLAS3_MAINView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CKOLAS3_MAINView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

#ifdef _KOLASIII_TL
	m_pInfo->MODE = 10000;
#endif
	m_pInfo->m_pMain = this;

	// 2010.11.26 ADD BY KSJ : 통합도서서비스 사용유무에 따라 Main이 달라짐
	// 16.09.29 JMJ KOL.UDF.020
	//CString strValue, strIsUnityLoanService;
	CString strValue;	
	
	// DB 정보를 읽온다.
	INT ids;
	ids = GetReferenceDBInfo();
	if (ids < 0) 
	{
		MessageBox(_T("DB Reference 정보를 읽지 못하였습니다. 프로그램을 종료합니다."));
		GetParentFrame()->PostMessage(WM_CLOSE, 0, 0);
	}
	else 
	{	
		m_pInfo->CONNECTION_INFO = CONNECTION_INFO;

		INT ids;
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		CString strQuery;
		
		// 2010.11.26 ADD BY KSJ : 통합도서서비스 사용유무에 따라 Main이 달라짐
		strQuery.Format(_T("SELECT MIN(VALUE_1) FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='통합도서서비스' AND VALUE_NAME='통합도서서비스'"));
		// 16.09.29 JMJ KOL.UDF.020
		//ids = dm.GetOneValueQuery(strQuery,strIsUnityLoanService);	
		ids = dm.GetOneValueQuery(strQuery,m_strIsUnityLoanService);	
		if ( ids < 0) 
		{
			AfxMessageBox(_T("접속정보를 확인해 주세요.\r\n프로그램을 종료합니다."));
			GetParentFrame()->PostMessage(WM_CLOSE, 0, 0);
			return;
		}
		
		// 2012.09.26 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2회
		// 비밀번호 암호화 관리값 체크
		CString strIsUserPwEncryption;
		CString strIsVisitorPwEncryption;
		CString strIsCardPwEncryption;
		strQuery.Format(_T("SELECT NVL((SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USERPWENCRYPTION' AND ROWNUM = 1), 'N'), NVL((SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'VISITORPWENCRYPTION' AND ROWNUM = 1), 'N'), NVL((SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'CARDPWCRYPTION' AND ROWNUM = 1), 'N') FROM DUAL"));
		dm.RestructDataManager(strQuery);
		strIsUserPwEncryption = dm.GetCellData(0, 0);
		strIsVisitorPwEncryption = dm.GetCellData(0, 1);
		strIsCardPwEncryption = dm.GetCellData(0, 2);	
		if(_T("Y") == strIsUserPwEncryption)
		{	//사용자 비밀번호 암호화여부
			m_pInfo->m_bUserPwEncryption = TRUE;
		}
		if(_T("Y") == strIsVisitorPwEncryption)
		{	//이용자 비밀번호 암호화여부
			m_pInfo->m_bVisitorPwEncryption = TRUE;
		}
		if(_T("Y") == strIsCardPwEncryption)
		{	//회원증 비밀번호 암호화여부
			m_pInfo->m_bCardPwEncryption = TRUE;
		}

		m_pInfo->CONNECTION_INFO = CONNECTION_INFO;
		// 로그인 다이알로그를 띄우는 타이머
		SetTimer(1, 100, NULL);	
	} 
	
	// Tab Control 초기화
	m_ctlFormTab.InitImageList(IDB_TABIMAGES);	// only necessary to call if tabs have images	
	TabExtended(TRUE);	
	
	CRect rect;	
	GetClientRect(rect);			
	m_ctlFormTab.MoveWindow(0,_TAB_TOP,rect.Width(),rect.Height()-_TAB_TOP );
	this->m_ctlFormTab.InsertItem(0, _T("메인"), 0);
	m_ctlFormTab.SetParent(this);

	TCHAR		currentpath[512];
	CString		strMainPath;
	
	m_ctlFormTab.GetClientRect(rect);
	// 18.11.07 JMJ KOL.RED.2018.020
	/*
	rect.left = 3;
	rect.top = _FLASH_TOP;	
	rect.right -= 6;
	m_ctrlMainFlash.MoveWindow(rect);	
	*/
	_tgetcwd( currentpath, 512 );
	
#ifdef _KOLASIII
		//16/03/10 김혜영 : 책이음서비스 사용유무에 따른 차이를 둘 필요가 없어짐
/*// BEFORE ----------------------------------------------------------------------------------
		// 2010.11.26 ADD BY KSJ : 통합도서서비스 사용유무에 따라 Main이 달라짐
		//DEL strMainPath.Format(_T("%s\\KolasMain.kmf"), currentpath );
		if(_T("Y")==strIsUnityLoanService)
		{
			strMainPath.Format(_T("%s\\kolas3KL.kmf"), currentpath );
		}
		else
		{
			strMainPath.Format(_T("%s\\KolasMain.kmf"), currentpath );
		}
*/// AFTER -----------------------------------------------------------------------------------
	//	strMainPath.Format(_T("%s\\Kolas3KL.kmf"), currentpath );
//*/ END -------------------------------------------------------------------------------------
#endif

#ifdef _KOLASIII_TL
//	strMainPath.Format(_T("%s\\kolas3_이동도서관.kmf"), currentpath );
#endif

	// 18.11.07 JMJ KOL.RED.2018.020
//	m_ctrlMainFlash.SetMovie(strMainPath);
	
	TAB_ITEMS_STRU *tabitem_stru;
	tabitem_stru = NULL;
	tabitem_stru = new TAB_ITEMS_STRU;
	tabitem_stru->nItemType = _MAIN_FORM;
	tabitem_stru->nItemIndex = 0;
	tabitem_stru->nFormIndex = -1;
	tabitem_stru->nInternetIndex = -1;
	tabitem_stru->MenuID = _T("");
	tabitem_stru->nToolBarID = -1;
	tabitem_stru->ExplorerStatusText = _T("");
	tabitem_stru->WorkingTime = _T("");
	tabitem_stru->SearchedRow = _T("");
	tabitem_stru->pEslMgr = NULL;

	////////////////
	// 18.11.07 JMJ KOL.RED.2018.020

	GetClientRect(rect);	
	rect.top = _INET_TOP;
	rect.right += 5;
	//rect.bottom -= 52;	
	
	CString strBody;
	CString strTitle;
	m_MainExplorer = new CExplorer();

	strMainPath.Format(_T("file:\\%s\\mainweb\\main.html"), currentpath );
	m_MainExplorer->SetURL(strMainPath);
	m_MainExplorer->Create(NULL, _T("HtmlView"), WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN, rect, this, AFX_IDW_PANE_FIRST);
	m_MainExplorer->m_HtmlView->m_pParent = this;
	m_MainExplorer->MoveWindow(rect);	
	m_MainExplorer->m_nInternetIndex = 0;
	
	///////////////
	
	m_TabList.AddTail((CObject*)tabitem_stru);	
	SetFormScree();
	
	EnableThemeDialogTexture(GetSafeHwnd());	
	
	// 외부 연동용 핸들 기록
	FileSaveHwnd();	
	
	// 외부 연동용 메뉴 로드
	MenuItemLoad();		
	
	/*
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CStatusBar *pBar = pFrame->GetStatusBar();
	///pBar->SetPaneInfo(2, ID_STATUS_SLIDER, SBPS_NOBORDERS, 500);
	pBar->SetPaneText(2,_T("2"));
	pBar->SetPaneText(3,_T("3"));
	pBar->SetPaneText(4,_T("4"));
	*/
}

/////////////////////////////////////////////////////////////////////////////
// CKOLAS3_MAINView diagnostics

#ifdef _DEBUG
void CKOLAS3_MAINView::AssertValid() const
{
	CFormView::AssertValid();
}

void CKOLAS3_MAINView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CKOLAS3_MAINDoc* CKOLAS3_MAINView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKOLAS3_MAINDoc)));
	return (CKOLAS3_MAINDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKOLAS3_MAINView message handlers

INT CKOLAS3_MAINView::GetReferenceDBInfo(void)
{
	// 2008.09.26 UPDATE BY CJY : 암호화된 접속정보를 복호화한 후 설정
	CString info = _T("");
	{
		CString strDMInfo;
		CFWDES DES;		// 암복호화 클래스
		DES.Read_DES_File(	// 접속정보를 복호화하여 읽어온다.
			_T("..\\CFG\\UseDMInfoChange.cfg"),	// 접속정보 파일
			_T("k^Ty"),	// 암호키
			strDMInfo	// 복호화된 문자
			);
		
		CString strMode = _T("");
		CString strDBInfoBefore = _T("");
		CString strDBInfoAfter = _T("");
		
		CString strAlias = _T("");
		CString strLine = _T("");
		
		for(int nCnt=0; nCnt < 3; nCnt++)
		{
			AfxExtractSubString(strLine, strDMInfo, nCnt, '\n');
			AfxExtractSubString(strAlias, strLine, 0, '=');
			
			strAlias.TrimLeft();
			strAlias.TrimRight();
			if(_T("MODE") == strAlias)
			{
				AfxExtractSubString(strMode, strLine, 1, '=');
				strMode.TrimLeft();
				strMode.TrimRight();
			}
			else if(_T("BEFORE") == strAlias)
			{
				AfxExtractSubString(strDBInfoBefore, strLine, 1, '=');
				strDBInfoBefore.TrimLeft();
				strDBInfoBefore.TrimRight();
			}
			else if(_T("AFTER") == strAlias)
			{
				AfxExtractSubString(strDBInfoAfter, strLine, 1, '=');
				strDBInfoAfter.TrimLeft();
				strDBInfoAfter.TrimRight();
			}			
		}
		if(_T("TRUE") == strMode)
		{
			info = strDBInfoAfter;
		}
		else if(_T("FALSE") == strMode)
		{
			info = strDBInfoBefore;
		}
	}
	CONNECTION_INFO = info;
	DBKIND = _T("ORACLE");

// 이전소스
//DEL 	// 접속정보를 UseDMInfoChange.cfg에서 가져오도록 변경	
//DEL 	BOOL bDBInfoMode = FALSE;
//DEL 	CStdioFile fd;
//DEL 	CString info = _T(""), strDBInfoBefore = _T(""), strDBInfoAfter = _T("");
//DEL 	if (fd.Open(_T("..\\CFG\\UseDMInfoChange.cfg"), CStdioFile::modeRead|CFile::typeBinary) == TRUE) {
//DEL 		TCHAR cUni;
//DEL 		fd.Read( &cUni, sizeof(TCHAR) );
//DEL 		if( 0xFEFF != cUni )
//DEL 		{
//DEL 			fd.SeekToBegin();
//DEL 		}
//DEL 		CString strLine, strTag, strData;
//DEL 		while(TRUE) {
//DEL 			if (fd.ReadString(strLine) == FALSE) break;
//DEL 			strLine.TrimLeft();				strLine.TrimRight();
//DEL 			if (strLine.GetLength() == 0) continue;
//DEL 			
//DEL 			INT nPos = strLine.Find('=', 0);
//DEL 			if (nPos == -1) continue;
//DEL 			
//DEL 			strTag = strLine.Left(nPos);
//DEL 			strTag.MakeUpper();			strTag.TrimLeft();		strTag.TrimRight();
//DEL 			strData = strLine.Mid(nPos+1);
//DEL 			strData.TrimLeft();			strData.TrimRight();
//DEL 			
//DEL 			if (strTag == _T("MODE")) {
//DEL 				strData.MakeUpper();
//DEL 				if (strData == _T("TRUE")) bDBInfoMode = TRUE;
//DEL 				else bDBInfoMode = FALSE;
//DEL 			}
//DEL 			if (strTag == _T("BEFORE")) strDBInfoBefore = strData;
//DEL 			if (strTag == _T("AFTER")) strDBInfoAfter = strData;
//DEL 		}
//DEL 		fd.Close();
//DEL 		if (strDBInfoBefore.GetLength() == 0) bDBInfoMode = FALSE;
//DEL 		if (strDBInfoAfter.GetLength() == 0) bDBInfoMode = FALSE;
//DEL 	}
//DEL 	if (bDBInfoMode == TRUE) 
//DEL 	{
//DEL 		strDBInfoAfter.TrimLeft();		strDBInfoAfter.TrimRight();
//DEL 		strDBInfoAfter.MakeLower();
//DEL 		info = strDBInfoAfter;
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		strDBInfoBefore.TrimLeft();		strDBInfoBefore.TrimRight();
//DEL 		strDBInfoBefore.MakeLower();
//DEL 		info = strDBInfoBefore;
//DEL 	}
//DEL 	CONNECTION_INFO = info;
//DEL 	DBKIND = _T("ORACLE");
//DEL 	m_sSequence = _T("ESL_SEQ");
//DEL 	m_sUserID = _T("이씨오");
	return 0;
}

void CKOLAS3_MAINView::OnTimer(UINT nIDEvent) 
{
	// 로그인 화면을 띄운다.
	if (nIDEvent == 1) 
	{
		KillTimer(1);
		CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame == NULL) return;		
		
		CString strCmdLine = AfxGetApp()->m_lpCmdLine;
		
		CLoginDlg dlg(this);
		dlg.m_strUser = strCmdLine;
		dlg.CONNECTION_INFO = CONNECTION_INFO;		
		
		if ( TRUE ) {
			//=================================================
			//2004-07-07 이학중 
			Get_ECOPatchDownLoader_Info();
			
			//=================================================
			dlg.DBKIND = DBKIND;
			dlg.m_pInfo = m_pInfo;
			if (dlg.DoModal() != IDOK) 
			{
				// 로그인 실패
				if(m_bDownLoader == FALSE) 
				GetParentFrame()->PostMessage(WM_CLOSE, 0, 0);
				return;
			} 
			else 
			{
				// 로그인 성공
				// Title에 로그인 시간, 로그인 아이디를 보여준다.
				CString strWindowText;
				CTime t = CTime::GetCurrentTime();				
				
//				HANDLE file = CreateFile( _T("KOLASIII.exe"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
// 				CString strTime;  				
// 				SYSTEMTIME time;				
// 				FILETIME create_time, access_time, write_time;				
// 				GetFileTime(file, &create_time, &access_time, &write_time);				
// 				FileTimeToSystemTime( &write_time, &time );				
// 				strTime.Format( _T("%04d-%02d-%02d "), time.wYear, time.wMonth, time.wDay);

				//=============================================
				//2008.08.19 UPDATE BY PJW : 패치버전을 제외한다.
				//				strWindowText.Format(_T("KOLAS III [%s][%s, %d-%d-%d %d:%d:%d] - Patch Version %s"), m_pInfo->TITLE, m_pInfo->USER_ID,t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), strTime);

				// << 2015년 공공도서관 시스템 개선 2015/11/13 _LCM : 버전 관리 
				CString sLastPatchVersion;

				TCHAR swVersion [VERSION_MAX_SIZE] = {0,};
				TCHAR swDate [VERSION_MAX_SIZE] = {0,};
				
				if(!GetKOLASIIIVer(swVersion) || !GetKOLASIIIBuildDate(swDate) )
					sLastPatchVersion.Format(_T("UnInstall"));
				else					
					sLastPatchVersion.Format(_T("%s-%s"), swDate, swVersion);				
					
				// 16.09.29 JMJ KOL.UDF.020
				if(m_strIsUnityLoanService == _T("Y")){
					CFrameWriter Writer(m_pInfo);
					Writer.setCommand(_T("IL_K71_LOC_B01_SERVICE"));
					Writer.addRecord();
					Writer.addElement(_T("도서관부호"), m_pInfo->LIB_CODE);
					Writer.addElement(_T("자료관리시스템명"), _T("KOLAS III"));
					Writer.addElement(_T("자료관리시스템버전"), swVersion);
					Writer.addElement(_T("클라이언트아이피"), m_pInfo->LOCAL_IP);
					Writer.addElement(_T("물리적주소"), m_pInfo->MAC_ADDRESS);


					CFrameSender Sender(m_pInfo);	
					CFrameReader Reader; 
					Sender.CommMiddleWareDaemon(&Writer, &Reader);
				}
				/*				
				if( KIISystemInfoInst.GetSetupBuildVersion() == _T(""))
				{

					sLastPatchVersion.Format(_T("UnInstall"));
				}
				else
				{
					sLastPatchVersion = KIISystemInfoInst.GetSetupBuildVersion().Mid(2) + _T("-") + KIISystemInfoInst.GetLastPatchVersion();
				}
				*/

				// >> 2015/11/13 --MOD

				strWindowText.Format(_T("KOLAS III [%s][%s, %d-%d-%d %d:%d:%d] - Ver. %s"), m_pInfo->TITLE, m_pInfo->USER_ID,t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), sLastPatchVersion);
				


				//=============================================
				
				GetParentFrame()->SetWindowText(strWindowText);		

				// 2009.10.15 ADD BY PDJ : 로그인 정보 기록
				CString strQuery;
				// << 2015년 공공도서관 시스템 개선 2015/11/13 _LCM : 버전 관리 
				/*
				strQuery.Format(_T("UPDATE MN_USER_TBL SET USER_IP='%s', INSTALL_PG_VER='%s', PATCH_PG_VER='%s' WHERE REC_KEY=%s;"), 
					GetLocalIP(), sLastPatchVersion.Left(sLastPatchVersion.Find(_T("-"))), KIISystemInfoInst.GetLastPatchVersion(), m_pInfo->USER_PK );
				*/
				strQuery.Format(_T("UPDATE MN_USER_TBL SET USER_IP='%s', INSTALL_PG_VER='%s', PATCH_PG_VER='%s' WHERE REC_KEY=%s;"), 
					GetLocalIP(), sLastPatchVersion.Left(sLastPatchVersion.Find(_T("-"))), swVersion, m_pInfo->USER_PK );
				// >> 2015/11/13 --MOD

				CESL_DataMgr dm;
				dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
				dm.StartFrame();
				dm.AddFrame(strQuery);
				dm.SendFrame(TRUE);
				dm.EndFrame();


				// << 2015년 공공도서관 시스템 개선 2015/10/13/ _LCM : 미사용 시 로그아웃 기능 추가
				CString strTemp = _T("");
				strQuery.Format(_T("SELECT SECU_TIMEOUT FROM MN_USER_TBL WHERE REC_KEY = '%s'"), m_pInfo->USER_PK);
				dm.GetOneValueQuery(strQuery, strTemp);				
				m_nTimeOutMax = _ttoi(strTemp);
				if(m_nTimeOutMax > 0)
				{
					m_bTimeOut = TRUE;					
				}
				
				if(m_bTimeOut && m_pTimeOut == NULL)
				{					
					g_hookKey = SetWindowsHookEx(WH_KEYBOARD_LL, GetMsgProcKey, (HINSTANCE)theApp.m_hInstance, NULL);
					g_hookMouse = SetWindowsHookEx(WH_MOUSE_LL, GetMsgProcMouse, (HINSTANCE)theApp.m_hInstance, NULL);
					m_pTimeOut = AfxBeginThread(TimeOutServiceThread, this);
				}				
				else if(m_bTimeOut && m_pTimeOut != NULL)
				{					
					m_pTimeOut->ResumeThread();									
				}
				// >> 2015/10/13 --MOD
				
				// 19.04.08 JMJ 책두레보완조치
				//--------------------------------------------------------------------------------------------------------------------------------------------
				CString strValue;
				INT ids = 0;
			
				strQuery.Format(_T("SELECT MIN(VALUE_1) FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='책두레시스템' AND VALUE_NAME='책두레사용여부' AND MANAGE_CODE = '%s'"), m_pInfo->MANAGE_CODE);
				ids = dm.GetOneValueQuery(strQuery,strValue);
				
				if ( ids < 0) 
				{
					AfxMessageBox(_T("접속정보를 확인해 주세요.\r\n프로그램을 종료합니다."));
					GetParentFrame()->PostMessage(WM_CLOSE, 0, 0);
					return;
				}

				if(strValue.Compare(_T("Y")) != 0){
					HideMenuAndToolbar(_T("책두레"));
				}
				//--------------------------------------------------------------------------------------------------------------------------------------------
				// 18.12.27 JMJ KOL.RED.2018.020.001 추가보완
				// 최종병합본에 누락된 코드 - 19.04.08
				//--------------------------------------------------------------------------------------------------------------------------------------------
				if(strValue == _T("Y"))
				{
					// 자바스크립트 AddKolasMenu('3700', 'event:3720', '책두레');
					m_MainExplorer->m_HtmlView->SetOption(_T("3700"), _T("event:3720"), _T("책두레"));
				}
				
				//--------------------------------------------------------------------------------------------------------------------------------------------
			}	
		

			CFileFind ff;
			if ( !ff.FindFile(_T("..\\cfg\\_NOTICE")) )
			{	
				CESL_DataMgr dm;
				dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
				
				CString strQuery;
				CString strUC, strES;
				strQuery =	_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
					_T("WHERE GROUP_2='기타' AND GROUP_3='공통' AND CLASS_ALIAS='공동목록공지사항' ")
					_T("AND MANAGE_CODE=UDF_MANAGE_CODE");
				dm.GetOneValueQuery(strQuery, strUC);
				if ( strUC == _T("Y") )
				{			
				
					CUC_NotifyPopupDlg pNotifyDlg;
					pNotifyDlg.m_pInfo = m_pInfo;
					pNotifyDlg.DoModal();	
				}

	
			}
			//2010.04.14 UPDATE BY CJY : Kalendar가 실행되는 동안 kolas가 대기하지 않도록 수정
			CString strCmd;
			strCmd.Format(_T("%s %s"), m_pInfo->USER_ID, m_pInfo->CONNECTION_INFO ); 
			::ShellExecute(NULL, _T("open"), _T("Kalendar.exe"), strCmd, _T("."), SW_SHOWNA);


			// MenuTreeBar Setting
			if ( m_listMenuItem.GetCount() > 0 )
			{
				
				CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
				pFrame->m_MenuBar.SetParent(this);
				pFrame->m_MenuBar.CreateMenuTreeBar(&m_listMenuItem);				
			}
			
			// Internet Explorer Navi Parent Setting
			pFrame->m_Navigator.SetParentView(this);	

			// 자동 메뉴 호출						
			if ( !dlg.m_AutoLoadMenu.IsEmpty() )
			{	
				CString strMenu = dlg.m_AutoLoadMenu;		
				CString strItem;				
				
				pFrame->LockWindowUpdate();			
				
				INT offset = 0;
				INT nIndex = 0;
				INT	nCount = 0;				
				while ( 1 )
				{
					nIndex = strMenu.Find(_T(" "), offset );
					if ( nIndex < 0 )
					{
						strItem = strMenu.Mid(offset);						
					}
					else
					{
						strItem = strMenu.Mid(offset, nIndex-offset);
					}
					
					if ( strItem.Find(_T("@")) == 0 )
					{	
						pFrame->m_Navigator.m_strURL = strItem.Mid(1); 
						LoadInternet();	
					}
					else
					{
						NewForm(strItem.Mid(1));
						// 2010.08.23 ADD BY PWR : 메뉴가 정상적으로 로드될때까지 잠시 대기
  						MSG msg;
  						DWORD dwStart;
  						dwStart = GetTickCount();
  						while(GetTickCount() - dwStart < 100)
  						{
  							while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  							{
  								TranslateMessage(&msg);
  								DispatchMessage(&msg);
  							}
  						}
					}
					nCount++;
					
					offset = nIndex + 1;
					if ( nIndex < 0 ) break;
				}
				
				// 첫번째 탭으로 이동한다.				
				if ( nCount > 1 )
				{
					m_ctlFormTab.SetCurSel(1);
					OnChangeTab();
				}
				
				pFrame->UnlockWindowUpdate();
			}

			// 2012.10.05 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2회
			// 사용자 비밀번호 갱신기간
			CESL_DataMgr dm;
			dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
			CString strQuery;
			CString strRenewalDate;
			CString strGetData;
			BOOL bRenewal = FALSE;
			strQuery.Format(_T("SELECT PW_RENEWAL_DATE FROM MN_USER_TBL WHERE REC_KEY = %s"), m_pInfo->USER_PK);
			dm.GetOneValueQuery(strQuery, strRenewalDate);
			CTime tCurrentTime = CTime::GetCurrentTime();	
			if(10 == strRenewalDate.GetLength())
			{
				CTime tRenewalTime(_ttoi(strRenewalDate.Left(4)),_ttoi(strRenewalDate.Mid(5, 2)),_ttoi(strRenewalDate.Right(2)),0,0,0);
				CTimeSpan tSpan = tCurrentTime - tRenewalTime;
				INT nElapseDay = (INT)tSpan.GetDays();
				if(180 < nElapseDay)
				{	//갱신기간 만료(6개월)
					bRenewal = TRUE;
				}
			}
			else
			{
				bRenewal = TRUE;
			}
			if(bRenewal)
			{
				CRENEWAL_PASSWORD dlg((CESL_Mgr*)this);
				dlg.m_pInfo = m_pInfo;
				if(IDOK == dlg.DoModal())
				{
					AfxMessageBox(_T("비밀번호 변경이 완료되었습니다."));
				}
			}
			// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


			//2018.02.01 JOB.2018.0005 ADD BY KYJ : KOLAS 버전 업로드 기능 추가 
			//1. 버전업로드를 할것인지 판단한다.
			//2. 현재코라스의 버전과 db에저장된 버전을 가져온다.
			//3. db에 저장된값이 없으면 insert 있으면 현재 코라스의 버전과 비교하여 update 한다.
			//start-------------------------------------------------------------------------------------------------------------------			
			CFileFind filefind;
			//1. 버전업로드를 할것인지 판단한다.
			if (!filefind.FindFile(_T("..\\cfg\\_dontsendmyversion"))) {
				//2. 현재코라스의 버전과 db에저장된 버전을 가져온다.
				TCHAR swVersion [VERSION_MAX_SIZE] = {0,};
				GetKOLASIIIVer(swVersion);
				
				CString strVesrion = swVersion;
				
				CESL_DataMgr dm;
				dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
				dm.SetDB(DBKIND);
				CString strTemp = _T("");
				CString strQuery = _T("");
				
				strQuery.Format(_T("SELECT MAX(SWVERSION) FROM MN_VER_TBL WHERE MAC_ADDRESS = '%s' AND LIB_CODE = '%s'"), m_pInfo->MAC_ADDRESS, m_pInfo->LIB_CODE);
				int ids = dm.GetOneValueQuery(strQuery, strTemp);

				//3. db에 저장된값이 없으면 insert 있으면 현재 코라스의 버전과 비교하여 update 한다.
				//가져온 버전의 유효값 검사는 length가 11인지만 판단한다.
				if(strTemp == _T("") && strVesrion.GetLength() == 11){
					strQuery.Format( _T("INSERT INTO MN_VER_TBL")
						_T("(REC_KEY, LIB_CODE, MAC_ADDRESS, SWVERSION, RENEWAL_DATE) ")
						_T("VALUES(ESL_SEQ.NEXTVAL, '%s', '%s', '%s', SYSDATE);")
						, m_pInfo->LIB_CODE, m_pInfo->MAC_ADDRESS, swVersion);
					dm.ExecuteQuery(strQuery, TRUE);
					
					SENDMYVERSION(m_pInfo->LIB_CODE,m_pInfo->MAC_ADDRESS,swVersion);
				}
				else if(_ttoi(strTemp) < _ttoi(swVersion) && strVesrion.GetLength() == 11 && strTemp.GetLength() ==11  )
				{
					strQuery.Format( _T("UPDATE MN_VER_TBL SET ")
						_T("SWVERSION = '%s', RENEWAL_DATE=SYSDATE WHERE MAC_ADDRESS = '%s' AND LIB_CODE = '%s';"),swVersion, m_pInfo->MAC_ADDRESS, m_pInfo->LIB_CODE);
					dm.ExecuteQuery(strQuery, TRUE);
					
					SENDMYVERSION(m_pInfo->LIB_CODE,m_pInfo->MAC_ADDRESS,swVersion);
				}
				
				
			}
			
			//end---------------------------------------------------------------------------------------------------------
			
		} else {
			// 업데이트할 파일이 발견 되었으니 스마트 업데이터를 실행하고 MAIN은 종료함
			PROCESS_INFORMATION pinfo1;
			STARTUPINFO Startupinfo = {0};
			Startupinfo.cb = sizeof(STARTUPINFO);
			::CreateProcess(NULL, _T("AutoExtract.exe"), NULL, NULL, FALSE, 0, NULL, NULL, &Startupinfo, &pinfo1);
			GetParentFrame()->PostMessage(WM_CLOSE, 0, 0);
			return;
		}
	}
	else if(3 == nIDEvent)
	{
		KillTimer(3);
	
		if(!m_pInfo->IsConnectUnityLoanService())
		{
			if(!m_bIsRunTestUnityLoanServiceThread)
			{
				m_pThread = AfxBeginThread(TestUnityLoanServiceThread, this);
			}
		}		
	}

	
	CFormView::OnTimer(nIDEvent);
}
// 19.04.08 JMJ 책두레보완조치
//--------------------------------------------------------------------------------------------------------------------------------------------
VOID CKOLAS3_MAINView::HideMenuAndToolbar(CString strGroupName)
{
	// 메뉴 제거
	if(strGroupName.IsEmpty() == FALSE){
		POSITION pos1,	pos2;
		CMenuItem *pMenuItem = NULL;
		CToolBarItem *newToolBarItem = NULL;
		
		for(pos1 = m_listMenuItem.GetHeadPosition(); (pos2 = pos1) != NULL ;){
			pMenuItem = NULL;
			pMenuItem = (CMenuItem *)m_listMenuItem.GetNext(pos1);
			if(GroupMenuCheck(strGroupName, _T("메뉴"), pMenuItem->m_strAlias, pMenuItem->m_strGroupAlias) == TRUE)
			{
				m_listMenuItem.GetAt(pos2);
				m_listMenuItem.RemoveAt(pos2);
			}
		}
		// 툴바 제거
		for(pos1 = m_listToolBarItem.GetHeadPosition(); (pos2 = pos1) != NULL ;){
			newToolBarItem = NULL;
			newToolBarItem = (CToolBarItem *)m_listToolBarItem.GetNext(pos1);
			if(GroupMenuCheck(strGroupName, _T("툴바"), newToolBarItem->m_strAlias, newToolBarItem->m_strToolGroupAlias) == TRUE)
			{
				m_listToolBarItem.GetAt(pos2);
				m_listToolBarItem.RemoveAt(pos2);
			}
		}
	}
}

BOOL CKOLAS3_MAINView::GroupMenuCheck(CString strGroupName, CString strType, CString strAlias, CString strGroupAlias)
{
	if(strGroupName.Compare(_T("책두레")) == 0){
		if(strType.Compare(_T("메뉴")) == 0){
			if(strAlias.Compare(_T("책두레")) == 0 && strGroupAlias.Compare(_T("CO7_책두레")) == 0){
				return TRUE;
			}
		}
		// 책두레는 31001에서 툴바 제한됨
	}
	return FALSE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------


VOID CKOLAS3_MAINView::LoadMenuRef()
{
	DWORD START, FINISH;
	START = GetTickCount();
	CESL_DataMgr dm;
	CMenuItem *newMenuItem;
	dm.SetDB(DBKIND);
	dm.SetCONNECTION_INFO(CONNECTION_INFO);
	CString strQuery,strQuery1,strQuery2,strQuery3,strQuery4,strQuery5;
	
// Get MenuItem
#ifndef _KOLASIII_TL
// 2008.08.21 UPDATE BY LKS : 모든 메뉴에 권한 부여하는 방식->권한 여부를 검사 후 모든 권한을 부여하거나 권한이 부여된 메뉴만을 LOAD
	strQuery.Format(
		_T("SELECT AU_TOTAL FROM MN_USER_TBL WHERE AU_TOTAL='T' AND ID='%s'"), m_pInfo->USER_ID);
	dm.RestructDataManager(strQuery);
	
	//모든권한이 부여되어 있다면(AU_TOTAL='T')
	if(dm.GetRowCount()!=0)
	{
		//기존에 사용하던 모든 메뉴에 권한 부여하는 쿼리
		strQuery.Format(
			_T("SELECT ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET, TITLE_DESC, MENU_ID, ILEVEL, JLEVEL, KLEVEL, ICON_IDX ")			
			// 16/10/25 김혜영 : KOLIS-NET 웹페이지 연동 메뉴 추가
			// 국가자료공동목록(CO6_%%)의 MENU_ID가 3670 보다 작은 경우에는 메뉴구성을 하지 않도록 함
/*// BEFORE -----------------------------------------------------------------------------------------
			_T("FROM ESL_NEWMENU_TBL WHERE SPF_USE_FLAG ='y' ORDER BY MENU_ID"));		
*/// AFTER ------------------------------------------------------------------------------------------
			_T("FROM ESL_NEWMENU_TBL WHERE SPF_USE_FLAG ='y' AND MENU_ID NOT IN (SELECT MENU_ID FROM ESL_NEWMENU_TBL WHERE GROUP_ALIAS like 'CO6_%%' AND MENU_ID < 3670) ORDER BY MENU_ID"));
//*/ END ---------------------------------------------------------------------------------------------
		dm.RestructDataManager(strQuery);
	}
	//모든권한이 아닌 특정권한인 경우(AU_B='T' OR USER_AU_A='T')
	else
	{
		strQuery1.Format(_T("SELECT ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET, TITLE_DESC, MENU_ID, ILevel, JLevel, KLevel, ICON_IDX FROM ESL_NEWMENU_TBL WHERE SPF_USE_FLAG ='y' AND MENU_ID IN (SELECT MENU_ID FROM ESL_NEWMENU_TBL ENT, MN_USER_TBL MUT WHERE SPF_USE_FLAG ='y' AND ( ( MUT.AU_A = ENT.AU_A AND MUT.AU_A = 'T' ) OR ( MUT.AU_B = ENT.AU_B AND MUT.AU_B = 'T' ) OR ( MUT.AU_C = ENT.AU_C AND MUT.AU_C = 'T' ) OR ( MUT.AU_D = ENT.AU_D AND MUT.AU_D = 'T' ) OR ( MUT.USER_AU_A = ENT.USER_AU_A AND MUT.USER_AU_A = 'T' ) OR ( MUT.USER_AU_B = ENT.USER_AU_B AND MUT.USER_AU_B = 'T' ) OR ( MUT.USER_AU_C = ENT.USER_AU_C AND MUT.USER_AU_C = 'T' ) OR ( MUT.USER_AU_D = ENT.USER_AU_D AND MUT.USER_AU_D = 'T' ) OR ( MUT.USER_AU_E = ENT.USER_AU_E AND MUT.USER_AU_E = 'T' ) OR ( MUT.USER_AU_F = ENT.USER_AU_F AND MUT.USER_AU_F = 'T' ) ")
			_T("OR ( MUT.USER_AU_G = ENT.USER_AU_G AND MUT.USER_AU_G = 'T' ) OR ( MUT.USER_AU_H = ENT.USER_AU_H AND MUT.USER_AU_H = 'T' ) OR ( MUT.USER_AU_I = ENT.USER_AU_I AND MUT.USER_AU_I = 'T' ) OR ( MUT.USER_AU_J = ENT.USER_AU_J AND MUT.USER_AU_J = 'T' ) OR ( MUT.USER_AU_K = ENT.USER_AU_K AND MUT.USER_AU_K = 'T' ) OR ( MUT.USER_AU_L = ENT.USER_AU_L AND MUT.USER_AU_L = 'T' ) OR ( MUT.USER_AU_M = ENT.USER_AU_M AND MUT.USER_AU_M = 'T' ) OR ( MUT.USER_AU_N = ENT.USER_AU_N AND MUT.USER_AU_N = 'T' ) OR ( MUT.USER_AU_O = ENT.USER_AU_O AND MUT.USER_AU_O = 'T' ) OR ( MUT.USER_AU_P = ENT.USER_AU_P AND MUT.USER_AU_P = 'T' ) OR ( MUT.USER_AU_Q = ENT.USER_AU_Q AND MUT.USER_AU_E = 'T' ) OR ( MUT.USER_AU_R = ENT.USER_AU_R AND MUT.USER_AU_R = 'T' ) OR ( MUT.USER_AU_S = ENT.USER_AU_S AND MUT.USER_AU_S = 'T' ) OR ( MUT.USER_AU_T = ENT.USER_AU_T AND MUT.USER_AU_T = 'T' ) ) AND MUT.ID='%s')"),m_pInfo->USER_ID);
		strQuery2.Format(_T("UNION SELECT ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET, TITLE_DESC, MENU_ID, ILevel, JLevel, KLevel, ICON_IDX FROM ESL_NEWMENU_TBL WHERE SPF_USE_FLAG ='y' AND MENU_ID IN (SELECT SUBSTR(MENU_ID, 1, 3) FROM ESL_NEWMENU_TBL ENT, MN_USER_TBL MUT WHERE SPF_USE_FLAG ='y' AND ( ( MUT.AU_A = ENT.AU_A AND MUT.AU_A = 'T' ) OR ( MUT.AU_B = ENT.AU_B AND MUT.AU_B = 'T' ) OR ( MUT.AU_C = ENT.AU_C AND MUT.AU_C = 'T' ) OR ( MUT.AU_D = ENT.AU_D AND MUT.AU_D = 'T' ) OR ( MUT.USER_AU_A = ENT.USER_AU_A AND MUT.USER_AU_A = 'T' ) OR ( MUT.USER_AU_B = ENT.USER_AU_B AND MUT.USER_AU_B = 'T' ) OR ( MUT.USER_AU_C = ENT.USER_AU_C AND MUT.USER_AU_C = 'T' ) OR ( MUT.USER_AU_D = ENT.USER_AU_D AND MUT.USER_AU_D = 'T' ) OR ( MUT.USER_AU_E = ENT.USER_AU_E AND MUT.USER_AU_E = 'T' )  OR ( MUT.USER_AU_F = ENT.USER_AU_F AND MUT.USER_AU_F = 'T' ) ")
			_T("OR ( MUT.USER_AU_G = ENT.USER_AU_G AND MUT.USER_AU_G = 'T' ) OR ( MUT.USER_AU_H = ENT.USER_AU_H AND MUT.USER_AU_H = 'T' ) OR ( MUT.USER_AU_I = ENT.USER_AU_I AND MUT.USER_AU_I = 'T' ) OR ( MUT.USER_AU_J = ENT.USER_AU_J AND MUT.USER_AU_J = 'T' ) OR ( MUT.USER_AU_K = ENT.USER_AU_K AND MUT.USER_AU_K = 'T' ) OR ( MUT.USER_AU_L = ENT.USER_AU_L AND MUT.USER_AU_L = 'T' ) OR ( MUT.USER_AU_M = ENT.USER_AU_M AND MUT.USER_AU_M = 'T' ) OR ( MUT.USER_AU_N = ENT.USER_AU_N AND MUT.USER_AU_N = 'T' ) OR ( MUT.USER_AU_O = ENT.USER_AU_O AND MUT.USER_AU_O = 'T' ) OR ( MUT.USER_AU_P = ENT.USER_AU_P AND MUT.USER_AU_P = 'T' ) OR ( MUT.USER_AU_Q = ENT.USER_AU_Q AND MUT.USER_AU_E = 'T' ) OR ( MUT.USER_AU_R = ENT.USER_AU_R AND MUT.USER_AU_R = 'T' ) OR ( MUT.USER_AU_S = ENT.USER_AU_S AND MUT.USER_AU_S = 'T' ) OR ( MUT.USER_AU_T = ENT.USER_AU_T AND MUT.USER_AU_T = 'T' ) ) AND MUT.ID='%s' )"),m_pInfo->USER_ID);
		strQuery3.Format(_T("UNION SELECT ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET, TITLE_DESC, MENU_ID, ILevel, JLevel, KLevel, ICON_IDX FROM ESL_NEWMENU_TBL WHERE SPF_USE_FLAG ='y' AND MENU_ID IN (SELECT MENU_ID FROM ESL_NEWMENU_TBL ENT, MN_USER_TBL MUT WHERE GROUP_ALIAS IN (SELECT GROUP_ALIAS FROM ESL_NEWMENU_TBL ENT WHERE MENU_ID IN "));
		strQuery4.Format(_T("(SELECT SUBSTR(MENU_ID, 1, 3) FROM ESL_NEWMENU_TBL ENT, MN_USER_TBL MUT WHERE SPF_USE_FLAG ='y' AND ( ( MUT.AU_A = ENT.AU_A AND MUT.AU_A = 'T' ) OR ( MUT.AU_B = ENT.AU_B AND MUT.AU_B = 'T' ) OR ( MUT.AU_C = ENT.AU_C AND MUT.AU_C = 'T' ) OR ( MUT.AU_D = ENT.AU_D AND MUT.AU_D = 'T' ) OR ( MUT.USER_AU_A = ENT.USER_AU_A AND MUT.USER_AU_A = 'T' ) OR ( MUT.USER_AU_B = ENT.USER_AU_B AND MUT.USER_AU_B = 'T' ) OR ( MUT.USER_AU_C = ENT.USER_AU_C AND MUT.USER_AU_C = 'T' ) OR ( MUT.USER_AU_D = ENT.USER_AU_D AND MUT.USER_AU_D = 'T' ) OR ( MUT.USER_AU_E = ENT.USER_AU_E AND MUT.USER_AU_E = 'T' ) OR ( MUT.USER_AU_F = ENT.USER_AU_F AND MUT.USER_AU_F = 'T' ) OR ( MUT.USER_AU_G = ENT.USER_AU_G AND MUT.USER_AU_G = 'T' ) OR ( MUT.USER_AU_H = ENT.USER_AU_H AND MUT.USER_AU_H = 'T' ) OR ( MUT.USER_AU_I = ENT.USER_AU_I AND MUT.USER_AU_I = 'T' ) ")
			_T("OR ( MUT.USER_AU_J = ENT.USER_AU_J AND MUT.USER_AU_J = 'T' ) OR ( MUT.USER_AU_K = ENT.USER_AU_K AND MUT.USER_AU_K = 'T' ) OR ( MUT.USER_AU_L = ENT.USER_AU_L AND MUT.USER_AU_L = 'T' ) OR ( MUT.USER_AU_M = ENT.USER_AU_M AND MUT.USER_AU_M = 'T' ) OR ( MUT.USER_AU_N = ENT.USER_AU_N AND MUT.USER_AU_N = 'T' ) OR ( MUT.USER_AU_O = ENT.USER_AU_O AND MUT.USER_AU_O = 'T' ) OR ( MUT.USER_AU_P = ENT.USER_AU_P AND MUT.USER_AU_P = 'T' ) OR ( MUT.USER_AU_Q = ENT.USER_AU_Q AND MUT.USER_AU_E = 'T' ) OR ( MUT.USER_AU_R = ENT.USER_AU_R AND MUT.USER_AU_R = 'T' ) OR ( MUT.USER_AU_S = ENT.USER_AU_S AND MUT.USER_AU_S = 'T' ) OR ( MUT.USER_AU_T = ENT.USER_AU_T AND MUT.USER_AU_T = 'T' ) ) AND MUT.ID='%s') ) AND IDX=1 )"),m_pInfo->USER_ID);
		strQuery5.Format(_T("ORDER BY MENU_ID "));

		// 16/10/25 김혜영 : KOLIS-NET 웹페이지 연동 메뉴 추가
/*// BEFORE ----------------------------------------------------------------------------------
		dm.RestructDataManager(strQuery1+strQuery2+strQuery3+strQuery4+strQuery5);
*/// AFTER ------------------------------------------------------------------------------------------
		strQuery.Format(_T("AND MENU_ID NOT IN (SELECT MENU_ID FROM ESL_NEWMENU_TBL WHERE GROUP_ALIAS like 'CO6_%%' AND MENU_ID < 3670) "));
		dm.RestructDataManager(strQuery1+strQuery2+strQuery3+strQuery4+strQuery+strQuery5);
//*/ END ---------------------------------------------------------------------------------------------

		INT count = dm.GetRowCount();
		
		//부모가 같은 메뉴중 ILEVEL이 3인 경우 권한이 부여된 메뉴중 처음 메뉴 하나는 반드시 IDX가 1이어야 
		//트리에 올바르게 정렬되어 들어간다(ILEVEL이 3이고 IDX가 1인 메뉴의 GROUP_ALIAS를 이용해 트리상의 링크를 생성해준다.)
		for(INT j=0;j<count;j++)
		{
			if(dm.GetCellData(j,10)==_T("3"))
			{
				if(j!=0) 
				{
					if(dm.GetCellData(j,11)!=dm.GetCellData(j-1,11))					
					{
						// 2008.11.19 UPDATE BY CJY : 같은 JLEVEL상에 반드시 하나는 IDX가 1이어야 한다.						
						// ILEVEL중간에 ILEVEL이 4인 경우의 처리루틴 추가
						// MENU_ID ILEVEL JLEVEL IDX
						// 3111 3 31 1
						// 3112 3 31 2
						// 31121 4 37 1						
						// 3113 3 31 3
						// 
						// 위의 경우 3111에 이미 IDX가 1이 설정되어 있기 때문에 
						// 3113에서 IDX를 설정해주면 안되는 데(3111과 3113은 같은 JLEVEL을 가지고 있다)
						// 기존 소스에서는 바로 전(31121)의 JLEVEL값이 현재(3113) JLEVEL과 다르다고 IDX를 1로 처리하여 문제가 발생
						// 이를 수정하였다.
						CString strFuncName = _T("CKOLAS3_MAINView::LoadMenuRef()");

						int bIsInsertAlready = FALSE;
						
						int ids = -1;
						const ILEVEL = 10;
						const JLEVEL = 11;
						CString rResult;
						
						ids = dm.GetCellData(j-1, ILEVEL, rResult);
						if(0 > ids) ERROR_MSG_RETURN_VOID2( -1 , strFuncName);
						if(_T("4") == rResult)
						{						
							for(int nReverseCnt = j-1; nReverseCnt >= 0; nReverseCnt--)
							{
								ids = dm.GetCellData(nReverseCnt, ILEVEL, rResult);
								if(0 > ids) ERROR_MSG_RETURN_VOID2( -2 , strFuncName);
								if(_T("3") == rResult)
								{
									CString rJLEVEL_1;
									CString rJLEVEL_2;

									ids = dm.GetCellData(j, JLEVEL, rJLEVEL_1);
									if(0 > ids) ERROR_MSG_RETURN_VOID2( -3 , strFuncName);
									ids = dm.GetCellData(nReverseCnt, JLEVEL, rJLEVEL_2);
									if(0 > ids) ERROR_MSG_RETURN_VOID2( -4 , strFuncName);
									
									// 현재 JLEVEL과 같은 값이 이전 ROW에 있다면 해당 JLEVEL에는
									// 이미 IDX가 설정이 되어있다.
									if(rJLEVEL_1 == rJLEVEL_2) 
									{
										bIsInsertAlready = TRUE;	
									}
									break;
								}
							}
						}
						
						if(FALSE == bIsInsertAlready)
						{
							ids = dm.SetCellData(j,2,_T("1"));						
							if(0 > ids) ERROR_MSG_RETURN_VOID2( -5, strFuncName);
						}						
					}					
				}
			}
		}
	}
#endif

#ifdef _KOLASIII_TL
	strQuery.Format(
		_T("SELECT ALIAS, GROUP_ALIAS, IDX, CAPTION, VISIBLE, STYLE, ACTION, TARGET, TITLE_DESC, MENU_ID, ILEVEL, JLEVEL, KLEVEL, ICON_IDX ")
		// 16/10/25 김혜영 : KOLIS-NET 웹페이지 연동 메뉴 추가
/*// BEFORE -----------------------------------------------------------------------------------------
		_T("FROM ESL_NEWMENU_TBL WHERE SPF_USE_FLAG ='y' ORDER BY MENU_ID"));
*/// AFTER ------------------------------------------------------------------------------------------
		_T("FROM ESL_NEWMENU_TBL WHERE SPF_USE_FLAG ='y' AND MENU_ID NOT IN (SELECT MENU_ID FROM ESL_NEWMENU_TBL WHERE GROUP_ALIAS like 'CO6_%%' AND MENU_ID < 3670) ORDER BY MENU_ID"));
//*/ END ---------------------------------------------------------------------------------------------
		dm.RestructDataManager(strQuery);
#endif
	
	for (INT i = 0; i < dm.GetRowCount(); i++)
	{
		CString strBuffer;
		newMenuItem = new CMenuItem;
		
		// ID
		newMenuItem->m_nID = m_nInitID++;
		
		// Alias
		dm.GetCellData(i, 0, strBuffer);
		newMenuItem->m_strAlias = strBuffer;

		// Group alias
		dm.GetCellData(i, 1, strBuffer);
		newMenuItem->m_strGroupAlias = strBuffer;

		// IDX
		dm.GetCellData(i, 2, strBuffer);
		newMenuItem->m_nIndex = _ttoi(strBuffer);
		
		// Caption
		dm.GetCellData(i, 3, strBuffer);
		newMenuItem->m_strCaption = strBuffer;

		// Visible
		dm.GetCellData(i, 4, strBuffer);
		newMenuItem->m_strVisible = strBuffer;

		// Style
		dm.GetCellData(i, 5, strBuffer);
		newMenuItem->m_strStyle = strBuffer;

		// Actioin
		dm.GetCellData(i, 6, strBuffer);
		newMenuItem->m_strAction = strBuffer;

		// Target
		dm.GetCellData(i, 7, strBuffer);
		newMenuItem->m_strTarget = strBuffer;

		// Title
		dm.GetCellData(i, 8, strBuffer);
		newMenuItem->m_strTitle = strBuffer;
		
		// Menu ID
		dm.GetCellData(i, 9, strBuffer);
		newMenuItem->m_strMenuID = strBuffer;
		
		// MenuTreeBar
		dm.GetCellData(i, 10, strBuffer);
		newMenuItem->m_nILevel = _ttoi(strBuffer);
		
		dm.GetCellData(i, 11, strBuffer);
		newMenuItem->m_nJLevel = _ttoi(strBuffer);
		
		dm.GetCellData(i, 12, strBuffer);
		newMenuItem->m_nKLevel = _ttoi(strBuffer);
		
		dm.GetCellData(i, 13, strBuffer);
		newMenuItem->m_nIconIdx= _ttoi(strBuffer);
		
		m_listMenuItem.AddTail((CObject*)newMenuItem);
	}
	
	FINISH = GetTickCount();
	TRACE(_T("(Finish) - LoadMenuRef [%d.%03d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);
}

void CKOLAS3_MAINView::LoadToolGroupRef()
{
	DWORD START, FINISH;
	START = GetTickCount();
	CESL_DataMgr dm;
	CToolBarGroup *newToolGroup;
	dm.SetDB(DBKIND);
	dm.SetCONNECTION_INFO(CONNECTION_INFO);
	CString strQuery;

	// Get MenuItem
	strQuery.Format(_T("SELECT ALIAS, MENU_ALIAS, MENU_ID, TOOLGROUP_ALIGN FROM ESL_NEWTOOLGROUP_TBL ORDER BY MENU_ID") );
	dm.RestructDataManager(strQuery);

	for (INT i = 0; i < dm.GetRowCount(); i++)
	{
		CString strBuffer;
		newToolGroup = new CToolBarGroup;
		
		// Alias
		dm.GetCellData(i, 0, strBuffer);
		newToolGroup->m_strAlias = strBuffer;

		// Group alias
		dm.GetCellData(i, 1, strBuffer);
		newToolGroup->m_strMenuAlias = strBuffer;
		
		dm.GetCellData(i, 2, strBuffer);
		newToolGroup->m_strMenuID = strBuffer;
		
		m_listToolBarGroup.AddTail((CObject*)newToolGroup);

		// Align
		dm.GetCellData(i, 3, strBuffer);
		if (strBuffer == _T("T"))
			newToolGroup->m_nDockBarID = AFX_IDW_DOCKBAR_TOP;
		else if (strBuffer == _T("B"))
			newToolGroup->m_nDockBarID = AFX_IDW_DOCKBAR_BOTTOM;
		else if (strBuffer == _T("L"))
			newToolGroup->m_nDockBarID = AFX_IDW_DOCKBAR_LEFT;
		else if (strBuffer == _T("R"))
			newToolGroup->m_nDockBarID = AFX_IDW_DOCKBAR_RIGHT;		
	}
	FINISH = GetTickCount();
	TRACE(_T("(Finish) - LoadToolGroupRef [%d.%03d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);
}

void CKOLAS3_MAINView::LoadToolbarRef()
{
	DWORD START, FINISH;
	START = GetTickCount();
	CESL_DataMgr dm;
	CToolBarItem *newToolBarItem;
	dm.SetDB(DBKIND);
	dm.SetCONNECTION_INFO(CONNECTION_INFO);
	CString strQuery;

	// Get Toolbar item
	strQuery.Format(//0         1         2     3      4    5   6       7            8           9             10       11          12      13   14
		_T("SELECT ALIAS,TOOLGROUP_ALIAS,IDX,CAPTION,STYLE,TYPE,SM,BITMAP_NORMAL,BITMAP_HOT,BITMAP_DISABLED,TOOLTIP,MENU_ALIAS,ACCECLERATOR,PK,MENU_ID ")
		_T("FROM ESL_NEWTOOLBAR_TBL WHERE SPF_USE_FLAG ='y' ORDER BY IDX"));
	dm.RestructDataManager(strQuery);

	for (INT i = 0; i < dm.GetRowCount(); i++)
	{
		CString strBuffer;
		newToolBarItem = new CToolBarItem;
		
		newToolBarItem->m_nID = m_nInitID ++;
		
		// ALIAS
		dm.GetCellData(i, 0, strBuffer);
		newToolBarItem->m_strAlias = strBuffer;
		
		// TOOLGROUP_ALIAS
		dm.GetCellData(i, 1, strBuffer);
		newToolBarItem->m_strToolGroupAlias = strBuffer;
		
		// IDX
		dm.GetCellData(i, 2, strBuffer);
		newToolBarItem->m_nIndex = _ttoi(strBuffer);
		
		// CAPTION
		dm.GetCellData(i, 3, strBuffer);
		newToolBarItem->m_strCaption = strBuffer;
		
		// STYLE
		dm.GetCellData(i, 4, strBuffer);
		newToolBarItem->m_strStyle = strBuffer;
		
		// TYPE
		dm.GetCellData(i, 5, strBuffer);
		newToolBarItem->m_strType = strBuffer;
		
		//SM
		dm.GetCellData(i, 6, strBuffer);
		newToolBarItem->m_strSM = strBuffer;
		
		// BITMAP_NORMAL
		dm.GetCellData(i, 7, strBuffer);
		newToolBarItem->m_strBitmapNormal = strBuffer;
		
		//BITMAP_HOT
		dm.GetCellData(i, 8, strBuffer);
		newToolBarItem->m_strBitmapHot = strBuffer;

		//BITMAP_DIABLED
		dm.GetCellData(i, 9, strBuffer);
		newToolBarItem->m_strBitmapDisabled = strBuffer;

		//TOOLTIP
		dm.GetCellData(i, 10, strBuffer);
		newToolBarItem->m_ToolTip = strBuffer;

		//MENU ALIAS
		dm.GetCellData(i, 11, strBuffer);
		newToolBarItem->m_strMenuAlias = strBuffer;

		// ACCECLERATOR
		dm.GetCellData(i, 12, strBuffer);
		newToolBarItem->m_strAcceclerator = strBuffer;

		// PK
		dm.GetCellData(i, 13, strBuffer);
		newToolBarItem->m_strPK = strBuffer;
		
		//MENU ID		
		dm.GetCellData(i, 14, strBuffer);
		newToolBarItem->m_strMenuID = strBuffer;
		
		m_listToolBarItem.AddTail((CObject*)newToolBarItem);
	}
	FINISH = GetTickCount();
	TRACE(_T("(Finish) - LoadToolbarRef [%d.%03d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);
}

void CKOLAS3_MAINView::SetMenu(CString strTopGroup, BOOL bUserDefaultLoad/*=TRUE*/, CString strMenuItemGroupAlias/*=""*/, CString strMenuItemAlias/*=""*/, INT *pnMenuItemID/*=NULL*/)
{
	if (pnMenuItemID) *pnMenuItemID = -1;
	// Get menu group
	CMenu *pSystemMenu = new CMenu;
	pSystemMenu->CreateMenu();
	
	INT i, nMenuCount = 0;

	CMenuItem *pMenuItem;

	INT nRows = m_listMenuItem.GetCount();
	POSITION pos;
	pos = m_listMenuItem.GetHeadPosition();
	CString strUserDefaultLoad = _T("");
	UINT nUserDefaultLoad = 0;
	for (i = 0; i < nRows; i++) {
		pMenuItem = NULL;
		pMenuItem = (CMenuItem *)m_listMenuItem.GetNext(pos);
		if (pMenuItem->m_strVisible == _T("F") || pMenuItem->m_strVisible == _T("f")) continue;
		
		if (pMenuItem->m_strGroupAlias != strTopGroup) continue;
		if (pMenuItem->m_strAction == _T("U")) {
			strUserDefaultLoad = pMenuItem->m_strTarget;
			continue;
		}
		nMenuCount++;
	}

	if (nMenuCount == 0) 
	{
		delete pSystemMenu;
		AfxMessageBox(_T("선택하신 메뉴는 사용 권한이 없는 메뉴 입니다. 사용을 원하시면 [관리자]에게 문의 하십시오."));
		return;
	}

	// 2006.2.7 ADD BY PDJ
	// 공동목록 사용유무 체크
	if ( strTopGroup == _T("UP기타_공동목록메뉴") )
	{
		/// K2UPUNI +++++++++++++++++++++++++++++++++++++++++++++++++++
		//		UC_KOLISNetUseChecker KolisnetUseChk;
		//		KolisnetUseChk.SetUserId(m_pInfo->USER_ID);
		//		if ( KolisnetUseChk.GetUseFlag() == FALSE )
		//		{
		//			return;
		//		}
		/// K2UPUNI ---------------------------------------------------
	}	
	
	RedrawWindow();
	
	INT nAutoLoadCmdMsgID;
	
	CString strMenuGroupAlias;
	CString strMenuCaption;

	pos = m_listMenuItem.GetHeadPosition();
	for (i = 0; i < nRows; i++) {
		pMenuItem = NULL;
		pMenuItem = (CMenuItem *)m_listMenuItem.GetNext(pos);
		if (pMenuItem == NULL) continue;
		if (pMenuItem->m_strVisible == _T("F") || pMenuItem->m_strVisible == _T("f")) continue;
		
		if (bUserDefaultLoad == FALSE) {
			if (pMenuItem->m_strAlias == strMenuItemAlias) {
				if (pMenuItem->m_strGroupAlias == strMenuItemGroupAlias && pMenuItem->m_strAction == _T("D") && pMenuItem->m_strTarget.Find('.', 0) >= 0) {
					if (pnMenuItemID) *pnMenuItemID = pMenuItem->m_nID;
				}
			}
		}
		if (strUserDefaultLoad.GetLength() > 0) {
			if (pMenuItem->m_strAlias == strUserDefaultLoad) {
				if (pMenuItem->m_strAction == _T("D") && pMenuItem->m_strTarget.Find('.', 0) >= 0) {
					nUserDefaultLoad = pMenuItem->m_nID;
				}
			}
		}
		if (pMenuItem->m_strGroupAlias != strTopGroup) continue;
		if (pMenuItem->m_strAction == _T("U")) continue;
		
		strMenuGroupAlias = pMenuItem->m_strAlias;
		strMenuGroupAlias.TrimLeft();			strMenuGroupAlias.TrimRight();
		if (!strMenuGroupAlias.IsEmpty())
		{
			nAutoLoadCmdMsgID = -1;
			strMenuCaption = _T("");
			nAutoLoadCmdMsgID = IsAutoLoadMenu(strMenuGroupAlias, strMenuCaption);
			strMenuCaption = pMenuItem->m_strCaption;
			if (nAutoLoadCmdMsgID == -1)	
			{
				// Sub Menu가 있는 경우, 혹은 메뉴가 아예 없는 경우
				// 서브 메뉴를 생성해서 붙인다.
				// Append Submenu
				CMenu *pMenu = AppendSubMenu(strMenuGroupAlias);
				//pSystemMenu->AppendMenu(MF_POPUP, (UINT)pMenu->m_hMenu, strMenuGroupAlias);
				pSystemMenu->AppendMenu(MF_POPUP, (UINT)pMenu->m_hMenu, strMenuCaption);
				pMenu->Detach();
				m_listSubMenu.AddTail((CObject*)pMenu);
			} else {
				//pSystemMenu->AppendMenu(MF_STRING | MF_ENABLED, nAutoLoadCmdMsgID, strMenuCaption);
				pSystemMenu->AppendMenu(MF_STRING | MF_ENABLED, nAutoLoadCmdMsgID, strMenuCaption);
			}
		}
	}
	
	GetParentFrame()->SetMenu(pSystemMenu);
	
	//GetParentFrame()->DrawMenuBar();
	m_listSubMenu.AddTail((CObject*)pSystemMenu);
	
	//	SetScrollSizes(MM_TEXT,CSize(1,1));
	
	// Default 작업 화면이 있는 경우 그 작업 화면을 띄워준다.
	if (bUserDefaultLoad == TRUE && nUserDefaultLoad > 0) {
		PostMessage(WM_COMMAND, nUserDefaultLoad, NULL);
	}
}

// Return Values
// -1		:	AutoLoad Menu 가 아님
INT CKOLAS3_MAINView::IsAutoLoadMenu(CString strMenuGroupAlias, CString &strMenuCaption)
{
	INT nAutoLoadCmdMsgID	  = -1;
	strMenuCaption = strMenuGroupAlias;

	POSITION pos = m_listMenuItem.GetHeadPosition();
	while (pos)
	{
		CMenuItem *pMenuItem = (CMenuItem*)m_listMenuItem.GetNext(pos);
		if (pMenuItem && pMenuItem->m_strGroupAlias == strMenuGroupAlias)
		{
			if (pMenuItem->m_strVisible == _T("F")) continue;		// 안보이게 설정된 메뉴인경우
			
			if (pMenuItem->m_strStyle == _T("P"))					// Separator 인경우
			{
				return nAutoLoadCmdMsgID;						// return -1;
			}
			else if (pMenuItem->m_strStyle == _T("S"))				// String 인 경우
			{
				return nAutoLoadCmdMsgID;						// return -1;
			} 
			else if (pMenuItem->m_strStyle == _T("A"))				// AutoLoadMenu인경우
			{
				nAutoLoadCmdMsgID = pMenuItem->m_nID;
				strMenuCaption = pMenuItem->m_strCaption;
			}
		}
	}
	return nAutoLoadCmdMsgID;
}


CMenu *CKOLAS3_MAINView::AppendSubMenu(CString strGroupAlias)
{
	CMenu *pMenu = new CMenu;
	pMenu->CreateMenu();

	POSITION pos = m_listMenuItem.GetHeadPosition();
	while (pos)
	{
		CMenuItem *pMenuItem = (CMenuItem*)m_listMenuItem.GetNext(pos);
		if (pMenuItem && pMenuItem->m_strGroupAlias == strGroupAlias)
		{
			if (pMenuItem->m_strVisible == _T("F")) continue;
			
			if (pMenuItem->m_strStyle == _T("P"))
			{
				pMenu->AppendMenu(MF_SEPARATOR);
			}
			else if (pMenuItem->m_strStyle == _T("S"))
			{
				if (pMenuItem->m_strAction == _T("D"))
				{
					pMenu->AppendMenu(MF_STRING | MF_ENABLED, pMenuItem->m_nID, pMenuItem->m_strCaption);
				}
				if (pMenuItem->m_strAction == _T("P"))
				{
					CMenu *pSubMenu = AppendSubMenu(pMenuItem->m_strTarget);
					pMenu->AppendMenu(MF_POPUP, (UINT)pSubMenu->m_hMenu, pMenuItem->m_strCaption);
					pSubMenu->Detach();
					m_listSubMenu.AddTail((CObject*)pSubMenu);
				}	
			}
		}
	}
	return pMenu;
}

//CToolBar *CKOLAS3_MAINView::CreateToolBar(UINT nDockBarID /*=0*/)
/*
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame == NULL) return NULL;

	CToolBar * pToolBar = NULL;
	pToolBar = new CToolBar();
	
	if (!pToolBar->CreateEx(pFrame, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP |
		CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY ))
	{
		delete pToolBar;
		return NULL;
	}
	
	pToolBar->EnableDocking(CBRS_ALIGN_ANY);
	pFrame->DockControlBar(pToolBar, nDockBarID);	

	return pToolBar;
}
*/

BOOL CKOLAS3_MAINView::SetToolBarGroup(CString strMenuAlias)
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame == NULL) return FALSE;
	
	//ONSIZE 
	m_LoadTool = true;
	
	// 툴바 관련 초기화
	//FreeImageList();
	//FreeToolBarList();
	
	// 그룹에 맞게 툴바 생성
	CToolBarGroup *pToolBarGroup = NULL;
	CToolBar *pToolBar = NULL;

	POSITION pos = m_listToolBarGroup.GetHeadPosition();
	while (pos)
	{
		pToolBarGroup = (CToolBarGroup*)m_listToolBarGroup.GetNext(pos);
		if (pToolBarGroup == NULL) continue;
		if (pToolBarGroup->m_strMenuID != strMenuAlias) continue;
		
		//pToolBar = m_pMainApi->CreateToolBar(pFrame, pToolBarGroup->m_nDockBarID);
		//pToolBar = m_pMainApi->CreateToolBar(pFrame, IDC_ctlFormTab);
		pToolBar = CreateToolBar(IDC_ctlFormTab);		
		//pToolBar = m_ctlFormTab.CreateToolBar(IDC_ctlFormTab);		
		if (pToolBar == NULL) continue;
		
		pToolBar->SetWindowText(pToolBarGroup->m_strAlias);
		
		m_listToolBar.AddTail((CObject *)pToolBar);
		
		CRect rDlg;
		GetClientRect(rDlg);
		pToolBar->MoveWindow(0, _TOOLBAR_TOP, rDlg.right, 43);
		SetToolBar(strMenuAlias, pToolBarGroup->m_strAlias, pToolBar);					
		pToolBar->ShowWindow(SW_SHOW);			
	}
	
	//ReSize Tool Bar
	CRect rect;
	GetClientRect(rect);
	//ReSizeTool(rect.Width());

	// m_pInfo 에 세팅
	m_pInfo->m_pListToolBar = &m_listToolBar;
	if ( pToolBar == NULL ) return FALSE;	
	
	return TRUE;
}

void CKOLAS3_MAINView::SetToolBar(CString strMenuAlias, CString strToolGroupAlias, CToolBar *pToolBar)
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();

	m_pMainApi->SetToolBar(pFrame, strMenuAlias, strToolGroupAlias, pToolBar, &m_listToolBarItem, &m_listToolTip, &m_listImage);

	return;
}

BOOL CKOLAS3_MAINView::OnCmdMsg(UINT nID, INT nCode, VOID* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// check menu id
	POSITION pos = m_listMenuItem.GetHeadPosition();
	while (pos)
	{
		CMenuItem *pMenuItem = (CMenuItem*)m_listMenuItem.GetNext(pos);
		if (pMenuItem && nID == pMenuItem->m_nID)
			return TRUE;
	}

	// check toolbar id
	pos = m_listToolBarItem.GetHeadPosition();
	while (pos)
	{
		CToolBarItem *pToolBarItem = (CToolBarItem*)m_listToolBarItem.GetNext(pos);
		if (pToolBarItem && nID == pToolBarItem->m_nID && pToolBarItem->m_strType == _T("D"))
			return FALSE;
		if (pToolBarItem && nID == pToolBarItem->m_nID)
			return TRUE;
	}

	return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CKOLAS3_MAINView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if ( m_nMenuFormCount >= 20 ) 
	{
		AfxMessageBox(_T("사용되지 않는 업무 화면을 닫고 선택하세요."));
		return -1;
	}
	
	POSITION pos;
	
	// menu
	pos = m_listMenuItem.GetHeadPosition();
	while (pos)
	{
		CMenuItem *pMenuItem = (CMenuItem*)m_listMenuItem.GetNext(pos);
		if (pMenuItem && (UINT)wParam == pMenuItem->m_nID)
		{
			// EFS를 위해 메뉴를 저장한다.
			SetProgramPath(_T("MENU"), pMenuItem->m_strAlias);
			SetStatusText(_T(""));
			
			//FreeModelessDlg();
			
			if (pMenuItem->m_strTarget == _T("HOME"))
			{				
				CMenu *pMenu = GetParentFrame()->GetMenu();
				if (pMenu)
				{
					INT nCount = pMenu->GetMenuItemCount();
					for (INT i = nCount - 1; i >= 0; i--)
					{
						pMenu->DeleteMenu(i, MF_BYPOSITION);
					}
				}

				CString strTitle;
				GetParentFrame()->GetWindowText(strTitle);
				INT nFind = strTitle.Find(_T(">>"));
				if (nFind > 0)
				{
					strTitle = strTitle.Mid(0, nFind);
				}
				GetParentFrame()->SetWindowText(strTitle);

				FreeImageList();
				CMainFrame *pFrame = (CMainFrame*)GetParentFrame();
				FreeToolBarList();
				pFrame->RecalcLayout();
				return TRUE;
			}

			// 타이틀을 변경한다.
			CString strTitle;
			GetParentFrame()->GetWindowText(strTitle);
			GetParentFrame()->SetMessageText(pMenuItem->m_strCaption);

			INT nFind = strTitle.Find(_T(">>"));
			if (nFind > 0)
			{
				strTitle = strTitle.Mid(0, nFind);
			}
			//CString str1 = _T("");
			//str1.Format(_T("%s ▶ %s"), GetBusinessClass(pMenuItem->m_strGroupAlias), pMenuItem->m_strCaption);
			
			strTitle += _T(">>") + pMenuItem->m_strTitle;
			GetParentFrame()->SetWindowText(strTitle);

			// Save Cur Menu Alias
			m_strCurMenuAlias = pMenuItem->m_strAlias;
			m_strCurMenuCaption = pMenuItem->m_strCaption;
			m_strCurMenuTitle = pMenuItem->m_strTitle;
			
			BOOL bShow = SetToolBarGroup(pMenuItem->m_strMenuID);
			INT nResult = LoadDll(pMenuItem, bShow);

			TAB_ITEMS_STRU* tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
			
			if( -1 != tabitem_stru->nToolBarID )
			{
				POSITION posToolBarIndex = m_listToolBar.FindIndex(tabitem_stru->nToolBarID);
				if( NULL == posToolBarIndex ) return 0;
				
				CMToolBar *pToolBar = (CMToolBar*)m_listToolBar.GetAt(posToolBarIndex);
				pToolBar->ToolBarSizing(-1);
			}
				
			if (nResult < 0)
			{
				return TRUE;
			}
		
			GetParentFrame()->RecalcLayout();

			return TRUE;
		}
	}
	
	pos = m_listToolBarItem.GetHeadPosition();
	while (pos)
	{
		CToolBarItem *pToolBarItem = (CToolBarItem*)m_listToolBarItem.GetNext(pos);
		if(0 != (UINT)wParam)
		{
			if (pToolBarItem && (UINT)wParam == pToolBarItem->m_nID)
			{
				if( pToolBarItem->m_strAlias==_T("도구모음"))
				{
					SetStatusText(_T(""));
					ShowMenu(pToolBarItem->m_nID);
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

INT CKOLAS3_MAINView::LoadDll(CMenuItem *pMenuItem, BOOL bShow)
{
	ShowHideModeless(m_nTabIndex, SW_HIDE);	
	if (pMenuItem == NULL) return -1;
	
	CFileFind fd;
	if (fd.FindFile(pMenuItem->m_strTarget) == FALSE) {
		CString msg;
		//=============================================
		//2008.08.19 UPDATE BY PJW : 메세지변경
		//msg.Format(_T("지정된 매뉴를 로드하지 못하였습니다."), pMenuItem->m_strTarget);
		msg.Format(_T("향후 지원예정입니다."), pMenuItem->m_strTarget);
		//=============================================
		
		MessageBox(msg);
		return -1;
	}

	m_strCurMenuAlias = pMenuItem->m_strAlias;
	
	INT nCount = m_TabList.GetCount();
	TAB_ITEMS_STRU *tabitem_stru;
	tabitem_stru = NULL;
	tabitem_stru = new TAB_ITEMS_STRU;
	tabitem_stru->nItemType = _MENU_FORM;
	tabitem_stru->nItemIndex = nCount;
	tabitem_stru->nFormIndex = m_nMenuFormCount;
	tabitem_stru->nInternetIndex = -1;	
	tabitem_stru->MenuAlias = pMenuItem->m_strAlias;
	tabitem_stru->ExplorerStatusText = _T("");
	tabitem_stru->WorkingTime = _T("");
	tabitem_stru->SearchedRow= _T("");
	tabitem_stru->MenuID = pMenuItem->m_strMenuID;
	tabitem_stru->pEslMgr = NULL;
	
	// TabIndex 이동
	m_nTabIndex = nCount;
	
	if ( bShow == TRUE )
	{
		tabitem_stru->nToolBarID = m_nToolBarCount;
		m_nToolBarCount ++;
	}
	else
	{
		// 툴바가 없는 폼
		tabitem_stru->nToolBarID = -1; 
	}		
	
	tabitem_stru->MenuCatagory = pMenuItem->m_strTitle;	
	m_TabList.AddTail((CObject*)tabitem_stru);
	
	// 새로운 폼을 생성한다.
	m_pForm[m_nMenuFormCount] = new CForm;
	if (!m_pForm[m_nMenuFormCount]->CreateForm(this, pMenuItem->m_strTarget.GetBuffer(0), m_pInfo, 'M', pMenuItem->m_strMenuID)) return -1;	
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();	

	CRect rect;
	GetClientRect(rect);
	
	if ( bShow == TRUE ) rect.top = _FORM_TOP;	
	else rect.top = _INET_TOP;		
	
	rect.right = rect.right + 2;
	
	m_pForm[m_nMenuFormCount]->ResizeForm(rect);
		
	m_nMenuFormCount++;		
	
	SetFormScree();
	return 0;
}

void CKOLAS3_MAINView::CreateModelessDlg(CToolBarItem *pToolBarItem)
{
	// 2008.09.25 ADD BY PDJ : Toolbar hit log 
	CESL_DataMgr dm ;
	dm.SetDB(DBKIND);
	dm.SetCONNECTION_INFO(CONNECTION_INFO);	
	
	CString strQuery ;
	CString strValue;
	strQuery.Format( _T("SELECT REC_KEY FROM ESL_TOOLBAR_STATISTICS_TBL ")
					 _T("WHERE MENU_ID='%s' AND ALIAS='%s' AND MANAGE_CODE=UDF_MANAGE_CODE")
					 , pToolBarItem->m_strMenuID, pToolBarItem->m_strAlias );
// 	strQuery.Format(_T("SELECT REC_KEY FROM ESL_TOOLBAR_STATISTICS_TBL WHERE MENU_ID='%s' AND ALIAS='%s'"), pToolBarItem->m_strMenuID, pToolBarItem->m_strAlias);
	dm.GetOneValueQuery(strQuery, strValue);
	
	if ( strValue == _T("") )
	{
		// Insert
		strQuery.Format( _T("INSERT INTO ESL_TOOLBAR_STATISTICS_TBL ")
						 _T("(REC_KEY, ALIAS, MENU_ALIAS, MENU_ID, HIT_COUNT, LAST_HIT_DATE, MANAGE_CODE) ")
						 _T("VALUES(ESL_SEQ.NEXTVAL, '%s', '%s', '%s', 1, SYSDATE, UDF_MANAGE_CODE);")
						 , pToolBarItem->m_strAlias, pToolBarItem->m_strMenuAlias, pToolBarItem->m_strMenuID );
//		strQuery.Format( _T("INSERT INTO ESL_TOOLBAR_STATISTICS_TBL (REC_KEY,ALIAS,MENU_ALIAS,MENU_ID,HIT_COUNT,LAST_HIT_DATE ) VALUES ")
//			_T("(ESL_SEQ.NEXTVAL, '%s', '%s', '%s', 1, SYSDATE);"), pToolBarItem->m_strAlias, pToolBarItem->m_strMenuAlias, pToolBarItem->m_strMenuID );
		dm.ExecuteQuery(strQuery, TRUE);
	}
	else
	{
		// +1
		strQuery.Format( _T("UPDATE ESL_TOOLBAR_STATISTICS_TBL ")
						 _T("SET HIT_COUNT=HIT_COUNT+1, LAST_HIT_DATE=SYSDATE WHERE REC_KEY=%s;"), strValue);
// 		strQuery.Format( _T("UPDATE ESL_TOOLBAR_STATISTICS_TBL SET  HIT_COUNT = HIT_COUNT+1, LAST_HIT_DATE=SYSDATE WHERE REC_KEY=%s;"), strValue);
		dm.ExecuteQuery(strQuery, TRUE);
	}

	// ==================

	TAB_ITEMS_STRU *tabitem_stru;	
	POSITION pos;
	tabitem_stru = NULL;
	pos = m_TabList.FindIndex(m_nTabIndex);
	if ( pos == NULL ) return;
	tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
	
	if ( tabitem_stru->nItemType != _MENU_FORM ) return;
	
	if ( m_pForm[tabitem_stru->nFormIndex] == NULL ) return;	
	
	// check dialog
	pos = m_listModelessDlg[tabitem_stru->nFormIndex].GetHeadPosition();
	while (pos)
	{
		CModelessDlg *pModelessDlg = (CModelessDlg*)m_listModelessDlg[tabitem_stru->nFormIndex].GetNext(pos);
		// 2010.03.30 UPDATE BY PWR : SHOW->HIDE MODE
		if(pToolBarItem->m_strSM == pModelessDlg->m_strSM && pToolBarItem->m_strAlias == pModelessDlg->m_strButtonAlias)
		{
			if(pModelessDlg->m_pESL_Mgr->IsWindowVisible() == TRUE)
 			{
 				pModelessDlg->m_pESL_Mgr->ShowWindow(SW_HIDE);
				pModelessDlg->m_bShow = FALSE;
 			}
 			else
 			{
				pModelessDlg->m_pESL_Mgr->ShowWindow(SW_SHOW);			
				pModelessDlg->m_pESL_Mgr->SetFocus();
				pModelessDlg->m_bShow = TRUE;
			}
			return;
		}
// 		if (pToolBarItem->m_strSM == pModelessDlg->m_strSM && pToolBarItem->m_strAlias == pModelessDlg->m_strButtonAlias)
// 		{
// 			pModelessDlg->m_pESL_Mgr->ShowWindow(SW_SHOW);			
// 			pModelessDlg->m_pESL_Mgr->SetFocus();
// 			return;
// 		}
	}
	
	m_bIsRunModal = TRUE;
	//CESL_Mgr *pESL_Mgr = m_pForm[m_nMenuFormCount-1]->CreateModelessDlg(pToolBarItem->m_strSM, pToolBarItem->m_strAlias, pToolBarItem->m_strType == _T("R") ? TRUE : FALSE);
	CESL_Mgr *pESL_Mgr = m_pForm[tabitem_stru->nFormIndex]->CreateModelessDlg(pToolBarItem->m_strSM, pToolBarItem->m_strAlias, pToolBarItem->m_strType == _T("R") ? TRUE : FALSE);
	m_bIsRunModal = FALSE;

	if (pESL_Mgr != NULL)
	{
		CModelessDlg *newModelessDlg = new CModelessDlg;
		newModelessDlg->m_pESL_Mgr = pESL_Mgr;
		newModelessDlg->m_strSM = pToolBarItem->m_strSM;
		newModelessDlg->m_strButtonAlias = pToolBarItem->m_strAlias;		
		m_listModelessDlg[tabitem_stru->nFormIndex].AddTail((CObject*)newModelessDlg);
	}
}

UINT CKOLAS3_MAINView::OnDestroyModelessDlg(WPARAM wParam, LPARAM lParam)
{
/*
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
	*/
	return 0;
}

INT CKOLAS3_MAINView::GetMenuInfo(CString strMenuAlias, CString &strMenuTarget, CString &strMenuCaption, CString &strMenuGroup, CString &strMenuTitle)
{
	POSITION pos = m_listMenuItem.GetHeadPosition();
	while (pos)
	{
		CMenuItem *pMenuItem = (CMenuItem*)m_listMenuItem.GetNext(pos);
		if (pMenuItem == NULL) continue;
		if (pMenuItem->m_strAlias != strMenuAlias) continue;
		if (pMenuItem->m_strAction != _T("D")) continue;
		
		strMenuTarget = pMenuItem->m_strTarget;
		strMenuCaption = pMenuItem->m_strCaption;
		strMenuGroup = pMenuItem->m_strGroupAlias;
		strMenuTitle = pMenuItem->m_strTitle;

		return 0;
	}

	return -1;
}

UINT CKOLAS3_MAINView::OnRCGetCurMenu(WPARAM wParam, LPARAM lParam)
{
	CString *pStrMenuAlias = (CString*)wParam;
	CString *pStrMenuTitle = (CString*)lParam;

	if (pStrMenuAlias == NULL || pStrMenuTitle == NULL) return -1;

	//2004-02-16 [이학중] ========================================
	//즐겨찾기 추가시 화면이 없거나 기본메뉴가 없을 경우 즐겨찾기 추가 안 되도록 수정. 
	TAB_ITEMS_STRU *tabitem_stru;	
	tabitem_stru = NULL;
	POSITION pos;
	pos = m_TabList.FindIndex(m_nTabIndex);
	if ( pos == NULL ) return 0;
	tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
	
	if ( tabitem_stru->nItemType != _MENU_FORM ) return -1;	
	//2003-02-16 [이학중] ----------------------------------------
	pStrMenuAlias->Format(_T("%s"), m_strCurMenuAlias);
	pStrMenuTitle->Format(_T("%s"), m_strCurMenuTitle);
	
	return 0;
}

//
//UINT CKOLAS3_MAINView::OnRCShowRCFrame(WPARAM wParam, LPARAM lParam)
//{
//	// 메뉴 Alias 가져오기
//	CString *pStrMenuAlias = (CString*)lParam;
//	if (pStrMenuAlias == NULL) return -1;
//	if (pStrMenuAlias->GetLength() == 0) return -1;
//	
//	CString strMenuAlias;
//	CString strMenuTarget;
//	CString strMenuCaption;
//	CString strMenuGroup;
//	CString strMenuTitle;
//	strMenuAlias.Format(_T("%s"), pStrMenuAlias->GetBuffer(0));
//	if (GetMenuInfo(strMenuAlias, strMenuTarget, strMenuCaption, strMenuGroup, strMenuTitle) < 0) return -1;
//	
//	CRect rcFrame;
//	GetParentFrame()->GetWindowRect(rcFrame);
//	rcFrame.left += 100;
//	rcFrame.right += 100;
//	
//	rcFrame.top += 100;
//	rcFrame.bottom += 100;
//	
//	INT nPos1 = -1;
//	INT	nPos2 = -1;
//	
//	CString strWindowText;
//	GetParentFrame()->GetWindowText(strWindowText);
//	
//	CString sLibraryName;
//	nPos1 = strWindowText.Find(_T('['));
//	nPos2 = strWindowText.Find(_T(']'));
//	sLibraryName = strWindowText.Mid(nPos1, nPos2-nPos1+1);
//	
//	CString sTopMenu;
//	nPos1 = strMenuTitle.Find(_T('['));
//	nPos2 = strMenuTitle.Find(_T(']'));
//	sTopMenu = strMenuTitle.Mid(nPos1, nPos2-nPos1+1);
//	
//	strMenuTitle = strMenuTitle.Mid(nPos2+2);
//	
//	//화면메뉴명 [도서관명] : [Top메뉴명] ▶ 디렉토리명 ▶ 디렉토리명
//	//화면메뉴명 [도서관명] : [Top메뉴명]
//	CString sTitleText;	
//	nPos1 = nPos2 = -1;
//	while (TRUE) 
//	{
//		nPos2 = strMenuTitle.Find(_T("▶"), nPos1+1);
//		if (nPos2 == -1) break;
//		nPos1 = nPos2;
//	}
//	
//	if (-1 == nPos1)
//	{
//		sTitleText = strMenuTitle + _T(" ") + sLibraryName + _T(" : ") + sTopMenu;
//	}
//	else
//	{
//		sTitleText = strMenuTitle.Mid(nPos1+2) + _T(" ") + sLibraryName + _T(" : ") + sTopMenu + _T(" ") + strMenuTitle.Left(nPos1-1);
//	}
//	
//	// 쓰레드 생성
//	CRCThread *pRCThread;
//	CRuntimeClass *pRuntimeClass = RUNTIME_CLASS(CRCThread);
//	pRCThread = (CRCThread*)pRuntimeClass->CreateObject();
//	pRCThread->StartThread(	sTitleText, 
//		rcFrame,	
//		strMenuAlias, 
//		strMenuCaption, 
//		strMenuTarget, 
//		&m_listToolBarGroup, 
//		&m_listToolBarItem, 
//		m_pInfo, 
//		m_pRCDlg);
//	
//	m_RCThreadList.AddTail((CObject*)pRCThread);
//		  
//	return 0;
//}
						  

UINT CKOLAS3_MAINView::OnShowHideToolBarButton(WPARAM wParam, LPARAM lParam)
{
	if ((CString*) wParam == NULL || lParam < 0) return -1;
	CString strToolBarButtonAlias;
	strToolBarButtonAlias.Format(_T("%s"), ((CString*)wParam)->GetBuffer(0));
	BOOL bHide = (BOOL)lParam;
	
	// 2008.06.17 UPDATE BY PDW {{++
	// 탭단위 복수형태로 지원된 툴바들중
	// 현재 인덱스에서 지울수 있게 변경한다 
	// 현재 툴바를 가져온다	
	POSITION pos = m_TabList.FindIndex(m_nTabIndex);
	if( NULL == pos ) return 0;
	
	TAB_ITEMS_STRU* tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
	
	if( -1 != tabitem_stru->nToolBarID )
	{
		POSITION posToolBarIndex = m_listToolBar.FindIndex(tabitem_stru->nToolBarID);
		if( NULL == posToolBarIndex ) return 0;
		
		CToolBar *pToolBar = (CToolBar*)m_listToolBar.GetAt(posToolBarIndex);
		CToolBarCtrl &pToolBarCtrl = pToolBar->GetToolBarCtrl();
		
		// 보여주거나 감출 툴바아이템의 ID를 가져온다
		UINT nID = -1;
		CToolBarItem *pToolBarItem;
		POSITION positem = m_listToolBarItem.GetHeadPosition();
		while(positem)
		{
			pToolBarItem = (CToolBarItem*)m_listToolBarItem.GetNext(positem);
			// 13/03/27 박대우 : 2012년도 도서관 통합서비스 확대구축 사업 2차 수정작업
			//                   메뉴체크 조건 변경
/*//BEFORE-----------------------------------------------------------------------
			if ( pToolBarItem->m_strMenuAlias == tabitem_stru->MenuAlias )
*///AFTER------------------------------------------------------------------------
			if (pToolBarItem->m_strMenuID.CompareNoCase(tabitem_stru->MenuID) == 0)
/*//END------------------------------------------------------------------------*/			
			{
				if (pToolBarItem->m_strAlias == strToolBarButtonAlias)
				{
					nID = pToolBarItem->m_nID;
					break;
				}
			}
		}
		
		if (nID == -1) return -1;
		
		UINT nState = 0;//TBSTATE_WRAP;
		pToolBarItem->m_bIsVisible = !bHide;
		
		if (bHide)
		{
			nState |= TBSTATE_HIDDEN ;
		}
		else
		{
			nState |= TBSTATE_ENABLED;
		}
		
		pToolBarCtrl.SetState(nID, TBSTATE_WRAP);
		pToolBarCtrl.SetState(nID, nState);		
	}
	//	m_pMainApi->HideShowToolBarButton(strToolBarButtonAlias, m_strCurMenuAlias, &m_listToolBarGroup, &m_listToolBarItem, &m_listToolBar, bHide);
	// 2008.06.17 UPDATE BY PDW --}}
	
	GetParentFrame()->RecalcLayout();

	return 0;
}

/*
UINT CKOLAS3_MAINView::OnExecuteToolBarButton(WPARAM wParam, LPARAM lParam)
{
	if ((CString*)wParam == NULL) return -1;
	CString strToolBarButtonAlias;
	strToolBarButtonAlias.Format(_T("%s"), ((CString*)wParam)->GetBuffer(0));

	m_pMainApi->ExecuteToolBarButton(strToolBarButtonAlias, m_strCurMenuAlias, &m_listToolBarGroup, &m_listToolBarItem, &m_listToolBar);
	GetParentFrame()->RecalcLayout();

	return 0;
}
*/

UINT CKOLAS3_MAINView::OnExecuteToolBarButton(WPARAM wParam, LPARAM lParam)
{
	if ((TCHAR*)wParam == NULL) return -1;
	/// 2008.01.21 add by pdw
	/// 바코드 업무 지원을 위한 수정
	if(	NULL != (TCHAR*)lParam )
	{
		CString strFlag;
		strFlag.Format(_T("%s"), (TCHAR*)lParam);
		
		if( _T("BARCODE_SM") == strFlag )
		{
			CString strToolBarButtonAlias;
			strToolBarButtonAlias.Format(_T("%s"), (TCHAR*)wParam);
			
			POSITION pos = m_TabList.FindIndex(m_nTabIndex);
			if( NULL == pos ) return 0;
			
			TAB_ITEMS_STRU* tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
			
			if( -1 != tabitem_stru->nToolBarID )
			{
				POSITION posToolBarIndex = m_listToolBar.FindIndex(tabitem_stru->nToolBarID);
				if( NULL == posToolBarIndex ) return 0;
				
				CToolBar *pToolBar = (CToolBar*)m_listToolBar.GetAt(posToolBarIndex);
				CToolBarCtrl &pToolBarCtrl = pToolBar->GetToolBarCtrl();
				
				UINT nID = -1;
				CToolBarItem *pToolBarItem;
				POSITION positem = m_listToolBarItem.GetHeadPosition();
				while(positem)
				{
					pToolBarItem = (CToolBarItem*)m_listToolBarItem.GetNext(positem);
					if ( pToolBarItem->m_strMenuAlias == tabitem_stru->MenuAlias )
					{
						if (pToolBarItem->m_strAlias == strToolBarButtonAlias)
						{
							nID = pToolBarItem->m_nID;
							break;
						}
					}
				}
				
				if (nID == -1) return -1;
				PostMessage(WM_COMMAND, nID, 0);
			}
			
		}		
	}
	
	CString strToolBarButtonAcce;
	strToolBarButtonAcce.Format(_T("%s"), (TCHAR*)wParam);

	CString strToolBarButtonAlias = m_pMainApi->GetAcceToolBarButton(strToolBarButtonAcce, m_strCurMenuAlias, &m_listToolBarGroup, &m_listToolBarItem, &m_listToolBar);
	if (strToolBarButtonAlias.GetLength() == 0) return 0;
	m_pMainApi->ExecuteToolBarButton(strToolBarButtonAlias, m_strCurMenuAlias, &m_listToolBarGroup, &m_listToolBarItem, &m_listToolBar);
	GetParentFrame()->RecalcLayout();

	return 0;
}

UINT CKOLAS3_MAINView::OnSetAcceleratorDialog(WPARAM wParam, LPARAM lParam)
{
	m_pMainApi->m_strCONNECTION_INFO = m_pInfo->CONNECTION_INFO;
	INT ids = m_pMainApi->OnSetAcceleratorDialog(m_strCurMenuAlias, &m_listToolBarGroup, &m_listToolBarItem, &m_listToolBar);

	return 0;
}

VOID CKOLAS3_MAINView::OnSize(UINT nType, INT cx, INT cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	CRect rect;
	
	INT nCount = m_TabList.GetCount();
	if ( nCount <= 0 ) return;
	
	
	TAB_ITEMS_STRU *tabitem_stru;
	CToolBar *pToolBar;
	
	tabitem_stru = NULL;
		
	if ( m_ctlFormTab.GetSafeHwnd() != NULL ) 
	{		
		m_ctlFormTab.MoveWindow(0,_TAB_TOP,cx+5,30);						
#ifdef _KOLASIII
		CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
		pFrame->m_MenuBar.MoveWindow(0,_MENU_TOP,cx,25);		
#endif
	}

	POSITION pos;
	for ( int i=0 ; i<nCount ; i++ )
	{
		pos = m_TabList.FindIndex(i);
		if ( pos == NULL ) continue;
		tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
		switch ( tabitem_stru->nItemType )
		{
		case _MAIN_FORM :
			// 18.11.07 JMJ KOL.RED.2018.020
			/*
			m_ctlFormTab.GetClientRect(rect);				
			rect.top = _FLASH_TOP;		
			rect.bottom = cy;			
			if ( m_nTabIndex == i && m_bLock == FALSE ) m_ctrlMainFlash.MoveWindow(rect);		
			else m_ctrlMainFlash.MoveWindow(0,0,0,0);		
			*/
			m_ctlFormTab.GetClientRect(rect);		
			rect.top = _INET_TOP;	
			rect.bottom = cy;		
			m_MainExplorer->MoveWindow(rect);
			break;
		case _MENU_FORM :		
			m_ctlFormTab.GetClientRect(rect);		
			rect.top = _FORM_TOP;	
			rect.bottom = cy;
			pToolBar = NULL;
			pos = m_listToolBar.FindIndex(tabitem_stru->nFormIndex);			
			if ( pos != NULL )
			{
				pToolBar = (CToolBar*)m_listToolBar.GetAt(pos);		
				pToolBar->MoveWindow(0,_TOOLBAR_TOP,rect.right, 43);			
				if (m_pForm[tabitem_stru->nFormIndex]->m_pCreateFun == NULL) return;
				if (m_pForm[tabitem_stru->nFormIndex]->m_pResizeFun == NULL) return;
				rect.right = rect.right - 3;
				m_pForm[tabitem_stru->nFormIndex]->ResizeForm(rect);		
			}
			else
			{
				rect.top = _INET_TOP;	
				m_pForm[tabitem_stru->nFormIndex]->ResizeForm(rect);
			}
			break;
		case _INET_FORM :
			m_ctlFormTab.GetClientRect(rect);		
			rect.top = _INET_TOP;	
			rect.bottom = cy;		
			m_Explorer[tabitem_stru->nInternetIndex]->MoveWindow(rect);
			break;
		default:
			break;
		}
	}
	
	// Locking중이면	
	if ( m_bLock == TRUE ) 
	{		
		rect.left = 0;
		rect.top = 0;
		rect.right = cx;
		rect.bottom = cy;
		m_pLockForm->ResizeForm(rect);
	}
}

VOID CKOLAS3_MAINView::OnPaint() 
{	
	CPaintDC dc(this); // device context for painting	
}
void CKOLAS3_MAINView::ReSizeTool(int cx)
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	INT t,tmp,tmp2,tmp3,ToolCount=0;
	
	UINT nID;
	UINT nStyle;
	INT iImage;
	CString rString;
	INT output=0;
	TBBUTTONINFO* ptbbi=NULL;
	POSITION pos;
	
	for( INT i = 0; i < m_listToolBar.GetCount() ; i++) 
	{	 
		pos = m_listToolBar.FindIndex(i);
		if ( pos == NULL ) continue;
		CToolBar * pToolBar	= (CToolBar *)m_listToolBar.GetAt(pos);
		CToolBarCtrl &rpToolBarCtrl = pToolBar->GetToolBarCtrl();
		
		t = rpToolBarCtrl.GetButtonCount();
		ToolCount = t;
		
		tmp2 = t * 70; //tool Bar Button 전체의 길이 
		tmp = cx + 70; // 화면 길이 
		
		//TRACE(_T("Tmp : %d \n"),tmp);
		
		if (tmp2 > tmp)
		{
			
			if( (tmp2 - tmp) > 70 )
			{
				tmp3 = tmp2 - tmp;
				tmp = tmp3 / 70;
				t = t - tmp; 
			}
			
			//TRACE(_T("Tool Count: %d  \n"),ToolCount);
			tmp3 = t -2;
			
			for ( INT h = 0 ; h < ToolCount ; h++)
			{
				if( h < tmp3 )
				{
					pToolBar->GetButtonInfo(h, nID, nStyle, iImage);
					output=rpToolBarCtrl.HideButton(nID,FALSE);
					//TRACE(_T("Tool Index : %d Change V : %d Return : %d\n"),tmp3,h,output);
				}
				else
				{
					if ( h == ToolCount-1)
					{

						pToolBar->GetButtonInfo(ToolCount-1, nID, nStyle, iImage);
						rpToolBarCtrl.HideButton(nID,FALSE);
						pToolBar->GetButtonInfo(ToolCount-2, nID, nStyle, iImage);
						rpToolBarCtrl.HideButton(nID,TRUE);
					
					}
					else
					{

						pToolBar->GetButtonInfo(h, nID, nStyle, iImage);
						rpToolBarCtrl.HideButton(nID,TRUE);
						//TRACE(_T("Tool Index : %d Change INV : %d Return : %d\n"),tmp3,h,output);					
					}

				}

			}





		}
		else
		{			
		/*
		pToolBar->GetButtonInfo(ToolCount-2, nID, nStyle, iImage);
		rpToolBarCtrl.HideButton(nID,FALSE);			
		pToolBar->GetButtonInfo(ToolCount-1, nID, nStyle, iImage);
		rpToolBarCtrl.HideButton(nID,TRUE);
			*/
		}
	}	
}

VOID CKOLAS3_MAINView::ShowMenu(INT m_ID)
{
	INT tmp;
	UINT nID;
	UINT nStyle;
	INT iImage;
	CString rString;

	CBitmap bmpNormal;
	CBitmap bmpHot;

	CToolBar *TmpTool;
	CToolBarItem * TmpItem;
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();

	m_Menu.CreatePopupMenu();
	m_Menu.SetIconSize(48,48);
	
	POSITION pos;
	for(INT i =0 ; i< m_listToolBar.GetCount() ; i++)
	{
		pos = m_listToolBar.FindIndex(i);
		if ( pos == NULL ) continue;
		TmpTool = (CToolBar *)m_listToolBar.GetAt(pos);
		tmp = TmpTool->CommandToIndex(m_ID);
		if(tmp != -1) break;
	}


	CToolBarCtrl &rpToolBarCtrl = TmpTool->GetToolBarCtrl();


	for ( i=0; i < rpToolBarCtrl.GetButtonCount() ; i++)
	{
		TmpTool->GetButtonInfo(i, nID, nStyle, iImage);
		if( rpToolBarCtrl.IsButtonHidden(nID) )
		{
			for ( INT j =0 ; j < m_listToolBarItem.GetCount(); j++)
			{
				pos = m_listToolBarItem.FindIndex(j);
				if ( pos == NULL ) continue;
				TmpItem = (CToolBarItem*)m_listToolBarItem.GetAt(pos);
				
				if ( TmpItem->m_nID == nID)
				{
					if (TmpItem->m_strType == _T("D") || TmpItem->m_strStyle ==_T("S")) continue;
					
					rString = TmpItem->m_strCaption;

					CFile file;
					if (!file.Open(TmpItem->m_strBitmapNormal, CFile::modeRead | CFile::typeBinary))
					{
						m_Menu.AppendMenu(MF_STRING | MF_ENABLED, nID, (LPTSTR)(LPCTSTR) rString);
					}
					else
					{
						file.Close();
						
						if ( !file.Open(TmpItem->m_strBitmapHot, CFile::modeRead | CFile::typeBinary) )
						{
							m_Menu.AppendMenu(MF_STRING | MF_ENABLED, nID, (LPTSTR)(LPCTSTR)rString);
							
						}
						else
						{
							file.Close();

							bmpNormal.Attach(::LoadImage(NULL, TmpItem->m_strBitmapNormal, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
							bmpHot.Attach(::LoadImage(NULL, TmpItem->m_strBitmapHot, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
							
							m_Menu.AppendMenuA(MF_STRING | MF_ENABLED , nID, (LPSTR)(LPCTSTR)rString, &bmpNormal);
							bmpNormal.Detach();
							bmpHot.Detach();



						}
					}
				}
			}
		}
	}


	tagPOINT tmp1;
	CPoint point;
	GetCursorPos(&tmp1);

				
	point.x = tmp1.x;
	point.y = tmp1.y;



	m_Menu.TrackPopupMenu(TPM_LEFTALIGN,  point.x, point.y, this);

	m_Menu.DestroyMenu();
}

void CKOLAS3_MAINView::ShowRCDlg()
{
	/*
	if (m_pRCDlg->GetSafeHwnd())
	{
		m_pRCDlg->ShowWindow(SW_SHOW);
		m_pRCDlg->BringWindowToTop();
	}
	*/
}

VOID CKOLAS3_MAINView::ParseString(CString source, CString &head, CString &tail)
{
	INT cut = source.Find('=');
	if (cut == -1) return;
	
	head = source.Mid(0, cut);
	head.TrimLeft();		head.TrimRight();
	tail = source.Mid(cut+1);
	tail.TrimLeft();		tail.TrimRight();

	head.MakeUpper();
	if (head != _T("CAPTION")) tail.MakeUpper();
}

void CKOLAS3_MAINView::DisplayMainObject(CDC *pDC)
{
	INT count = m_listMainScreenObject.GetCount();
	GetClientRect(m_ScreenRect);
	if (m_ScreenSize.cx != -1 && m_ScreenSize.cy != -1) {
		m_nMarginX = (m_ScreenRect.Width() - m_ScreenSize.cx) / 2;
		m_nMarginY = (m_ScreenRect.Height() - m_ScreenSize.cy) / 2;
	}
	CMainObject * pMO = NULL;
	POSITION pos;
	pos = m_listMainScreenObject.GetHeadPosition();
	for(INT i = 0; i < count; i++) {
		pMO = NULL;
		pMO = (CMainObject*)m_listMainScreenObject.GetNext(pos);
		if (!pMO) continue;
		if (pMO->TYPE == _T("BITMAP")) DisplayMainObject_Bitmap(pMO, pDC);
		if (pMO->TYPE == _T("BITMAPPATTERN")) DisplayMainObject_BitmapPattern(pMO, pDC);
		if (pMO->TYPE == _T("BITMAPBUTTON")) DisplayMainObject_BitmapButton(pMO);
		if (pMO->TYPE == _T("BUTTON")) DisplayMainObject_Button(pMO);
	}	
}

void CKOLAS3_MAINView::DisplayMainObject_Bitmap(CMainObject *pMO, CDC *pDC)
{
	if (!pMO) return;
	if (!pDC) return;
	
	// 종은 수정
	CLoadBmp *pBmp = NULL;
	if (!pMO->CONTROL)
	{	
		CString filename;
		filename.Format(_T("..\\BMP\\%s"), pMO->IMAGE);
		pBmp = new CLoadBmp;
		if (!pBmp) return;
		if (pBmp->LoadBMP(filename) == false) return;
		pMO->CONTROL = (CObject*)pBmp;
	}
	else
	{
		pBmp = (CLoadBmp*)pMO->CONTROL;
	}
	INT sx=0, sy=0, cx = 0, cy = 0;
	ParseImageSize(pMO->SIZE, &sx, &sy, &cx, &cy);
	if (m_ScreenSize.cx != -1 && m_ScreenSize.cy != -1) {
		if (m_nMarginX > 0) sx = sx + m_nMarginX;
		if (m_nMarginY > 0) sy = sy + m_nMarginY;
	}
	pBmp->DrawDIB(pDC, sx, sy, cx, cy);

}

void CKOLAS3_MAINView::DisplayMainObject_BitmapPattern(CMainObject *pMO, CDC *pDC)
{
	if (!pMO) return;
	if (!pDC) return;
	
	// 종은 수정
	CBitmap *pBmp = NULL;

	if (!pMO->CONTROL)
	{
		CString fileName;
		fileName.Format(_T("..\\BMP\\%s"), pMO->IMAGE);
		pBmp = new CBitmap;
		if (!pBmp) return;

		pBmp->Attach(::LoadImage(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
		pMO->CONTROL = (CObject*)pBmp;
	}
	else
	{
		pBmp = (CBitmap*)pMO->CONTROL;
	}

	CBrush brush;
	brush.CreatePatternBrush(pBmp);

	CRect rect;
	GetClientRect(rect);

	pDC->FillRect(rect, &brush);

}

void CKOLAS3_MAINView::DisplayMainObject_BitmapButton(CMainObject *pMO)
{
	if (!pMO) return;
	INT sx=0, sy=0, cx=0, cy=0;
	ParseImageSize(pMO->SIZE, &sx, &sy, &cx, &cy);
	if (m_ScreenSize.cx != -1 && m_ScreenSize.cy != -1) {
		if (m_nMarginX > 0) sx = sx + m_nMarginX;
		if (m_nMarginY > 0) sy = sy + m_nMarginY;
		if (pMO->CONTROL) {
			CRect r(sx, sy, sx+cx, sy+cy);
			CWnd *pWnd = (CWnd*)pMO->CONTROL;
			pWnd->MoveWindow(r, TRUE);
			return;
		}
	}
	if (pMO->CONTROL) return;
	CNewButton *pButton = new CNewButton;
	pButton->Create(_T("My button"), BS_BITMAP|BS_OWNERDRAW|WS_CHILD|WS_VISIBLE, CRect(sx,sy,cx,cy), this, _ttoi(pMO->OBJECTID));
	CString up, down, focus;
	ParseButtonBitmap(pMO->IMAGE, up, down, focus);
	pButton->LoadBitmap(up, down, focus);
	pMO->CONTROL = (CObject*)pButton;
}

void CKOLAS3_MAINView::DisplayMainObject_Button(CMainObject *pMO)
{
	if (!pMO) return;
	if (pMO->CONTROL) return;
	CButton *pButton = new CButton;
	INT sx=0, sy=0, cx=0, cy=0;
	ParseImageSize(pMO->SIZE, &sx, &sy, &cx, &cy);
	pButton->Create(pMO->CAPTION, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(sx,sy,sx+cx,sy+cy), this, _ttoi(pMO->OBJECTID));
	pMO->CONTROL = (CObject*)pButton;
}

void CKOLAS3_MAINView::ParseButtonBitmap(CString source, CString &up, CString &down, CString &focus)
{
	INT cut, prev;
	CString str;
	cut = source.Find(',', 0);
	str = source.Mid(0, cut);
	str.TrimLeft();		str.TrimRight();
	up.Format(_T("..\\BMP\\%s"), str);
	prev = cut+1;
	
	cut = source.Find(',', prev);
	str = source.Mid(prev, cut-prev);
	str.TrimLeft();		str.TrimRight();
	down.Format(_T("..\\BMP\\%s"), str);
	prev = cut+1;

	str = source.Mid(prev);
	str.TrimLeft();		str.TrimRight();
	focus.Format(_T("..\\BMP\\%s"), str);
}

void CKOLAS3_MAINView::ParseImageSize(CString source, int *sx, int *sy, int *cx, int *cy)
{
	INT cut, prev;
	CString str;
	cut = source.Find(',', 0);
	str = source.Mid(0, cut);
	str.TrimLeft();		str.TrimRight();
	if (str.GetLength() == 0) *sx = 0;
		else *sx = _ttoi(str);
	prev = cut+1;
	
	cut = source.Find(',', prev);
	str = source.Mid(prev, cut-prev);
	str.TrimLeft();		str.TrimRight();
	if (str.GetLength() == 0) *sy = 0;
		else *sy = _ttoi(str);
	prev = cut+1;

	cut = source.Find(',', prev);
	str = source.Mid(prev, cut-prev);
	str.TrimLeft();		str.TrimRight();
	if (str.GetLength() == 0) *cx = 0;
		else *cx = _ttoi(str);
	prev = cut+1;

	str = source.Mid(prev);
	str.TrimLeft();		str.TrimRight();
	if (str.GetLength() == 0) *cy = 0;
		else *cy = _ttoi(str);
	return;
}

VOID CKOLAS3_MAINView::ParseBitmapImageSize(CString source, INT *sx, INT *sy)
{
	INT cut, prev;
	CString str;
	cut = source.Find(',', 0);
	str = source.Mid(0, cut);
	str.TrimLeft();		str.TrimRight();
	if (str.GetLength() == 0) *sx = 0;
		else *sx = _ttoi(str);
	prev = cut+1;
	
	str = source.Mid(prev);
	str.TrimLeft();		str.TrimRight();
	if (str.GetLength() == 0) *sy = 0;
		else *sy = _ttoi(str);
	return;
}

void CKOLAS3_MAINView::OnFileSave() 
{
	//ShowRCDlg();
}

INT CKOLAS3_MAINView::GetPatchCount(VOID)
{
	FILE *fd = _tfopen(_T("..\\CFG\\_DEBUG"), _T("rb"));
	if (fd) {
		fclose(fd);
		return 0;
	}
	
	CString strErrorFileName = _T("..\\Cfg\\SmartUpdater_err.cfg");
	FILE *fd_err = _tfopen(strErrorFileName, _T("rb"));
	if (fd_err) {
		fclose(fd_err);
		DeleteFile(strErrorFileName);
		return 0;
	}
	
	INT ids;
	
	//	INT nPatch_Count;
	//	INT nLastPatch_pk = 0;
	
	// file load -> insert to nLastPatch_pk
	
	//  HKEY    hKey;
	//	DWORD   dwType, n;
	//
	//	TCHAR    szValue[80];
	//
	//	#ifdef _ECOLAS_S
	//		ids = RegOpenKey( HKEY_LOCAL_MACHINE, _T("Software\\ECO\\ECOLAS-S"), &hKey );
	//	#endif
	//	#ifdef _ECOLAS
	//		ids = RegOpenKey( HKEY_LOCAL_MACHINE, _T("Software\\ECO\\ECOLAS"), &hKey );
	//	#endif
	//	#ifdef _UP
	//		ids = RegOpenKey( HKEY_LOCAL_MACHINE, _T("Software\\ECO\\KOLAS II Upgrade"), &hKey );
	//	#endif
	//	#ifdef _LIGHT
	//		ids = RegOpenKey( HKEY_LOCAL_MACHINE, _T("Software\\ECO\\KOLAS II Light"), &hKey );
	//	#endif
	//    if ( ids != ERROR_SUCCESS ) {
	//        return -1;
	//    }
	//
	//    n = 80;
	//    ids = RegQueryValueEx ( hKey,    // handle key
	//                            _T("LAST_PATCH_PK"),
	//                            NULL,
	//                            &dwType,
	//                            (UCHAR*)&szValue[0],
	//                            &n );
	//    if ( ids != ERROR_SUCCESS ) {
	//        return -2;
	//    }
	//
	//    nLastPatch_pk = _ttoi(szValue);
	
	CString sLastPatchVersion = KIISystemInfoInst.GetLastPatchVersion();
	//###################################################################################
	//if (sLastPatchVersion.IsEmpty()) sLastPatchVersion = _T("0");
	//if (sLastPatchVersion.Compare(_T("1.1"))) sLastPatchVersion = _T("030101");
	//	if (sLastPatchVersion.IsEmpty() || !sLastPatchVersion.Compare(_T("1.1"))) 
	//	{
	//		sLastPatchVersion = DEFAULT_LAST_VERSION;
	//	}
	//### >>> 처리 로직 CKIISystemInfo 클래스로 옮겨감
	//###################################################################################
	
	CString strResult = _T("0");
	INT nPatch_Count = 0;
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(CONNECTION_INFO);
	CString sQuery;
	//	sQuery.Format(_T("select count(pk) ")
	//				    _T("from PATCH_UPLOAD_TBL ")
	//				   _T("where pk>%d ")
	//				     _T("and length(FILE_NAME) > 14 "), nLastPatch_pk);
	
	sQuery.Format(_T("select count(pk) ")
		_T("from PATCH_UPLOAD_TBL ")
		_T("where length(FILE_NAME) > 18 ")
		_T("AND CAN_PATCH = 'Y' ")
		_T("AND substr(FILE_NAME,6,1)='0' ")
		_T("AND substr(FILE_NAME,6,13)>'%s' "),
		//_T("AND pk>%d"), 
		sLastPatchVersion);	//, _ttoi(KIISystemInfoInst.GetLastPatchKey()));
	
	//AfxMessageBox(sQuery);
	ids = dm.GetOneValueQuery(sQuery, strResult);
	if (strResult.GetLength() > 0) nPatch_Count = _ttoi(strResult.GetBuffer(0));
	
	return nPatch_Count;
}

/*
VOID CKOLAS3_MAINView::FreeRCThreadList(VOID)
{
	INT nCount = m_RCThreadList.GetCount();
	if (nCount == 0) return;
	CRCThread *pRCThread;
	for (INT i = 0; i < nCount; i++) 
	{
		pRCThread = NULL;
		pRCThread = (CRCThread*)m_RCThreadList.RemoveHead();
		if (pRCThread) {
			pRCThread->EndThread();
			Sleep(50);
			try {
				::WaitForSingleObject(pRCThread->m_hThread,INFINITE);
			} catch(...) {
				Sleep(500);
			}
			pRCThread = NULL;
		}
	}
	m_RCThreadList.RemoveAll();
	return;
}
*/

VOID CKOLAS3_MAINView::OnDestroy() 
{	
	CFormView::OnDestroy();
}

CString CKOLAS3_MAINView::GetBusinessClass(CString strMenuGroupAlias)
{
	if (strMenuGroupAlias.GetLength() < 2) return _T("기타");
	CString strHeader = strMenuGroupAlias.Left(2);
	strHeader.MakeUpper();
	
	if (strHeader == _T("BO")) return _T("단행");
	if (strHeader == _T("SE")) return _T("연속");
	if (strHeader == _T("CO")) return _T("공통");
	
	return _T("기타");
}

INT CKOLAS3_MAINView::SetStatusText(CString str)
{
	m_strSTATUS_TEXT = str;
	if ( GetParentFrame()->GetSafeHwnd() != NULL ) GetParentFrame()->SendMessage(WM_SET_STATUS_TEXT, NULL, (long)m_strSTATUS_TEXT.GetBuffer(0));
	return 0;
}

BOOL CKOLAS3_MAINView::IsDebugMode(VOID)
{
	EFS_BEGIN
		FILE *fd = _tfopen(_T("..\\CFG\\_DEBUG"), _T("rb"));
	if (fd) 
	{
		fclose(fd);
		return TRUE;
	}
	return FALSE;
	EFS_END
		return FALSE;
}

UINT CKOLAS3_MAINView::OnShowRCDlg(WPARAM wParam, LPARAM lParam)
{
	ShowRCDlg();
	return 0;
}

BOOL CKOLAS3_MAINView::PreTranslateMessage(MSG* pMsg) 
{	
	// 잠금기능 
	// 메시지 잠금
	if ( m_bLock == TRUE ) 
	{
		if ( pMsg->message != WM_PAINT )
		{
			return TRUE;
		}
	}
	
	if ( pMsg->message == WM_KEYDOWN && GetKeyState(VK_CONTROL) < 0 && pMsg->wParam == VK_F3 ) {
		PostMessage(WM_USER+2006);
		return TRUE;		
	}	
	
	if ( pMsg->message == WM_COMMAND || pMsg->message == 1111)
	{
		CString strTemp;
		strTemp.Format(_T("%d, %d"), pMsg->wParam, pMsg->lParam );
	}	
	
	return CFormView::PreTranslateMessage(pMsg);
}

BOOL CKOLAS3_MAINView::FindMenu(CString strMenuItemGroupAlias, CString strMenuItemAlias)
{
	INT nRows = m_listMenuItem.GetCount();
	POSITION pos = m_listMenuItem.GetHeadPosition();
	CMenuItem *pMenuItem;
	for (INT i = 0; i < nRows; i++) {
		pMenuItem = (CMenuItem *)m_listMenuItem.GetNext(pos);
		if (pMenuItem == NULL) return NULL;
		if (pMenuItem->m_strGroupAlias == strMenuItemGroupAlias && pMenuItem->m_strAlias == strMenuItemAlias && pMenuItem->m_strAction == _T("D")) return TRUE;
	}
	return FALSE;
}

INT CKOLAS3_MAINView::GetAutoExtractPatch(VOID)
{
	if (IsDebugMode() == TRUE) return 0;

	CRect rect, DlgRect;
	GetClientRect(&rect);
	DlgRect = rect;
	INT nWidth = DlgRect.Width();
	INT nHeight = DlgRect.Height();
	DlgRect.top = (nHeight - 30) / 2;
	DlgRect.bottom = DlgRect.top + 30;
	DlgRect.left = (nWidth - 331) / 2;
	DlgRect.right = DlgRect.left + 331;
	
	INT ids;
	
	//	INT nLastPatch_pk;
	//
	//	nLastPatch_pk = 0;
	//
	//	// file load -> insert to nLastPatch_pk
	//    HKEY    hKey;
	//    DWORD   dwType, n;
	//
	//    TCHAR    szValue[80];
	//
	//	#ifdef _ECOLAS_S
	//		ids = RegOpenKey( HKEY_LOCAL_MACHINE, _T("Software\\ECO\\ECOLAS-S"), &hKey );
	//	#endif
	//	#ifdef _ECOLAS
	//			ids = RegOpenKey( HKEY_LOCAL_MACHINE, _T("Software\\ECO\\ECOLAS"), &hKey );
	//	#endif
	//	#ifdef _UP
	//		ids = RegOpenKey( HKEY_LOCAL_MACHINE, _T("Software\\ECO\\KOLAS II Upgrade"), &hKey );
	//	#endif
	//	#ifdef _LIGHT
	//		ids = RegOpenKey( HKEY_LOCAL_MACHINE, _T("Software\\ECO\\KOLAS II Light"), &hKey );
	//	#endif
	//    if ( ids != ERROR_SUCCESS ) {
	//        return -1;
	//    }
	//
	//    n = 80;
	//    ids = RegQueryValueEx ( hKey,    // handle key
	//                            _T("AUTOEXTRACT_LAST_PATCH_PK"),
	//                            NULL,
	//                            &dwType,
	//                            (UCHAR*)&szValue[0],
	//                            &n );
	//    if ( ids != ERROR_SUCCESS ) {
	//		_stprintf(szValue, _T("-1\0"));
	//		RegSetValueEx( hKey, _T("AUTOEXTRACT_LAST_PATCH_PK"), NULL, REG_SZ, (_TUCHAR*)szValue, _tcsclen(szValue));
	//    }
	//	RegCloseKey(hKey);
	//
	//    nLastPatch_pk = _ttoi(szValue);
	
	CString strResult = _T("0");
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(CONNECTION_INFO);
	CString sQuery;
	//sQuery.Format(_T("select count(pk) from PATCH_BIN_UPLOAD_TBL where pk>%d"), nLastPatch_pk);
	sQuery.Format(_T("select count(pk) from PATCH_BIN_UPLOAD_TBL where pk>%d"), _ttoi(KIISystemInfoInst.GetLastAutoExtractPatchKey()));
	
	//AfxMessageBox(sQuery);
	
	ids = dm.GetOneValueQuery(sQuery, strResult);
	strResult.TrimLeft();		
	strResult.TrimRight();
	if (strResult.GetLength() > 0) 
	{
		if (_ttoi(strResult) > 0) 
		{
			GetNewAutoExtract(&dm);
		}
	}


	//SMART UPDATE 생성 
	//2004-06-28 이학중 
	//Get_ECOPatchDownLoader_Info();
	return 0;
}

INT CKOLAS3_MAINView::GetNewAutoExtract(CESL_DataMgr *pDM)
{
	if (pDM == NULL) return -1;
	
	CString strLocalPath = _T("AutoExtract.EXE");
	
	CString strMaxPK, strQuery, strResult[8], strServerPatchFileUrl;
	INT nResult = 6;
	INT ids;

	// ESL_MANAGE_TBL에서 PATCH_URL을 읽어온다.
	strQuery.Format( _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
					 _T("WHERE GROUP_1='L' AND GROUP_2='기타' AND GROUP_3='공통' ")
					 _T("AND CLASS_ALIAS='SERVER_PATCH_FILE_URL' AND MANAGE_CODE=UDF_MANAGE_CODE") );
	ids = pDM->GetOneValueQuery(strQuery, strServerPatchFileUrl);
	strServerPatchFileUrl.TrimLeft();			strServerPatchFileUrl.TrimRight();
	if (strServerPatchFileUrl.GetLength() == 0) return -2;
	if (ids < 0) return -2;

	// PATCH_BIN_UPLOAD_TBL에서 MAX(PK)를 읽어온다.
	strQuery.Format(_T("SELECT MAX(PK) FROM PATCH_BIN_UPLOAD_TBL"));
	ids = pDM->GetOneValueQuery(strQuery, strMaxPK);
	strMaxPK.TrimLeft();			strMaxPK.TrimRight();
	if (strMaxPK.GetLength() == 0) return -3;
	if (ids < 0) return -3;

	// MAX(PK)를 기준으로 PATCH 정보를 읽어온다.
	//                      0       1              2           3          4            5 
	strQuery.Format(_T("SELECT PK, PATCH_BIN_KEY, LIBRARY_PK, FILE_PATH, FILE_SIZE, SERVER_FILE_NAME FROM PATCH_BIN_UPLOAD_TBL WHERE PK=%s"), strMaxPK);
	ids = pDM->GetValuesQuery(strQuery, strResult, nResult);
	if (ids < 0) return -4;

	CString strGetUrl;
	if (strServerPatchFileUrl.GetAt(strServerPatchFileUrl.GetLength()-1) == '/') {
		strGetUrl.Format(_T("%s%s"), strServerPatchFileUrl, strResult[5]);
	} else {
		strGetUrl.Format(_T("%s/%s"), strServerPatchFileUrl, strResult[5]);
	}

	if (URLDownloadToFile(NULL, strGetUrl, strLocalPath, 0, NULL) != S_OK) 
	{	
		// AutoExtract.EXE 패치에 실패한 경우
		AfxMessageBox(_T(" 패치파일 다운로드에 실패하였습니다. "));
	} else {
		// AutoExtract.EXE 패치에 성공한 경우
		//ids = SetNewAutoExtractPkToRegistry(strMaxPK);
		ids = KIISystemInfoInst.SetLastAutoExtractPatchKey(strMaxPK);
		if (ids < 0)
		{
			AfxMessageBox(_T("AutoExtract.EXE의 업데이트 로그를 남기는데 실패하였습니다."));
		}
		else if (ids == 0) 
		{
			// DB 에 로그를 남긴다.
			
			/*INSERT INTO PATCH_BIN_CLIENT_LOG_TBL SET PK=(알아서),LIB_PATCH_KEY=(2의PK),
			PATCH_BIN_KEY=(2의PATCH_BIN_KEY),
			LIBRARY_PK=(2의LIBRARY_PK),
			CLIENT_NAME='(알아서)',CLIENT_PATCH_DATE=SYSDATE*/
			
			TCHAR szBufComputerName[257];
			DWORD nBufComputerName = 256;
			GetComputerName(szBufComputerName, &nBufComputerName);
			szBufComputerName[nBufComputerName] = '\0';

			CString strTmp;
			pDM->StartFrame();
			pDM->InitDBFieldData();
			strTmp.Format(_T("%s.NEXTVAL"), pDM->GetDefaultSEQName());
			pDM->AddDBFieldData(_T("PK"), _T("NUMERIC"), strTmp);
			pDM->AddDBFieldData(_T("LIB_PATCH_KEY"), _T("NUMERIC"), strResult[0]);
			pDM->AddDBFieldData(_T("PATCH_BIN_KEY"), _T("NUMERIC"), strResult[1]);
			pDM->AddDBFieldData(_T("LIBRARY_PK"), _T("NUMERIC"), strResult[2]);
			strTmp.Format(_T("%s"), szBufComputerName);
			pDM->AddDBFieldData(_T("CLIENT_NAME"), _T("STRING"), strTmp);
			pDM->AddDBFieldData(_T("CLIENT_PATCH_DATE"), _T("NUMERIC"), _T("SYSDATE"));
			pDM->MakeInsertFrame(_T("PATCH_BIN_CLIENT_LOG_TBL"));
			
			ids = pDM->SendFrame();
			pDM->EndFrame();
			if (ids < 0) 
			{
				AfxMessageBox(_T("DB에 AutoExtract.EXE 업데이트 로그를 남기는데 실패하였습니다."));
			}
		}
	}

	return 0;
}

//INT CKOLAS3_MAINView::SetNewAutoExtractPkToRegistry(CString strMaxPK)
//{
//    HKEY    hKey;
//	INT ids;
//	#ifdef _ECOLAS_S
//		ids = RegOpenKey( HKEY_LOCAL_MACHINE, _T("Software\\ECO\\ECOLAS-S"), &hKey );
//	#endif
//	#ifdef _ECOLAS
//		ids = RegOpenKey( HKEY_LOCAL_MACHINE, _T("Software\\ECO\\ECOLAS"), &hKey );
//	#endif
//	#ifdef _UP
//		ids = RegOpenKey( HKEY_LOCAL_MACHINE, _T("Software\\ECO\\KOLAS II Upgrade"), &hKey );
//	#endif
//	#ifdef _LIGHT
//		ids = RegOpenKey( HKEY_LOCAL_MACHINE, _T("Software\\ECO\\KOLAS II Light"), &hKey );
//	#endif
//    if ( ids != ERROR_SUCCESS ) {
//        return -1;
//    }
//
//    ids = RegSetValueEx( hKey, _T("AUTOEXTRACT_LAST_PATCH_PK"), NULL, REG_SZ, (_TUCHAR*)strMaxPK.GetBuffer(0), strMaxPK.GetLength());
//	RegCloseKey(hKey);
//    if ( ids != ERROR_SUCCESS ) {
//		AfxMessageBox(_T("AutoExtract.EXE의 업데이트 로그를 남기는데 실패하였습니다."));
//		return -1;
//    } else {
//		return 0;
//	}
//	return 0;
//}
//


VOID CKOLAS3_MAINView::LoadMenuRef_2()
{
	
	INT ids ;
	DWORD START, FINISH;
	START = GetTickCount();
	
	CMenuItem *newMenuItem ;
	
	INT nCount = 0 ;
	
	CStdioFile file ;
	CString strLine ;
	CString strTag ;
	CString strValue ;
	if(file.Open (_T("..\\cfg\\menutoolbar\\menu.cfg"),CFile::modeRead|CFile::typeBinary) )
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		newMenuItem = NULL ;
		while (file.ReadString (strLine))
		{	
			strLine.TrimLeft ();
			strLine.TrimRight () ;
			strLine.MakeUpper ();
			
			if(strLine.GetLength () < 2) continue ;
			if(strLine.GetAt (0) == '/') continue ;
			if(strLine.GetAt (0) == '#') continue ;
			
			if(strLine.GetAt (0) == '[') 
			{
				if(newMenuItem)
				{
					m_listMenuItem.AddTail((CObject*)newMenuItem);
					newMenuItem = NULL ;
				}
				if(!newMenuItem)
				{
					newMenuItem = new CMenuItem ;
		            newMenuItem->m_nID = m_nInitID++;	
					nCount ++ ;
				}
			}
			
			INT nPos = strLine.Find (_T("="))  ;
			if(nPos > 0 )
			{
				strTag = strLine.Mid (0,nPos-1) ;
				strTag.TrimLeft ();
				strTag.TrimRight () ;
				strTag.MakeUpper () ;
				
				strValue = strLine.Mid (nPos+1) ;
				strValue.TrimLeft ();
				strValue.TrimRight () ;
				strValue.MakeUpper () ;
				
				if(strTag == _T("ALIAS")) newMenuItem->m_strAlias = strValue ;
				// Group alias
				if(strTag == _T("GROUP_ALIAS")) newMenuItem->m_strGroupAlias = strValue;
				// IDX
				if(strTag == _T("IDX")) newMenuItem->m_nIndex = _ttoi(strValue);
				// Caption	
				if(strTag == _T("CAPTION")) newMenuItem->m_strCaption = strValue;
				// Visible
				if(strTag == _T("VISIBLE")) newMenuItem->m_strVisible = strValue;
				// Style
				if(strTag == _T("STYLE")) newMenuItem->m_strStyle = strValue;
				// Actioin
				if(strTag == _T("ACTION")) newMenuItem->m_strAction = strValue;
				// Target
				if(strTag == _T("TARGET")) newMenuItem->m_strTarget = strValue;
				// Title
				if(strTag == _T("TITLE_DESC")) newMenuItem->m_strTitle = strValue;
				
			}
		}
		if(newMenuItem)
		{
			m_listMenuItem.AddTail((CObject*)newMenuItem);
			newMenuItem = NULL ;
		}
	}
	else
	{
		AfxMessageBox(_T("Menu 파일을 Load 하지 못했습니다.")) ;
		return ;
	}

	file.Close () ;
       
	// 사용자의 디폴트 작업 화면을 가진 TOP 메뉴 항목을 읽어온다.

	CESL_DataMgr dm ;
	dm.SetDB(DBKIND);
	dm.SetCONNECTION_INFO(CONNECTION_INFO);
	
	m_strDEFAULT_LOAD_TOPMENU_ALIAS = _T("");
	CString strQuery ;
	strQuery.Format(_T("SELECT DEFAULT_LOAD_TOPMENU_ALIAS "
		_T("FROM ESL_MENU_METHOD_TBL ")
		_T("WHERE FLAG = 'U' AND USER_ID ='%s'")), m_pInfo->USER_ID);
	ids = dm.GetOneValueQuery(strQuery, m_strDEFAULT_LOAD_TOPMENU_ALIAS);
	
	FINISH = GetTickCount();
	TRACE(_T("(Finish) - LoadMenuRef [%d.%03d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);
}

void CKOLAS3_MAINView::LoadToolbarRef_2()
{
	DWORD START, FINISH;
	START = GetTickCount();
	
	CToolBarItem *newToolBarItem;
   
	INT nCount = 0;

	CStdioFile file ;
	CString strLine ;
	CString strTag ;
    CString strValue ;
	if(file.Open (_T("..\\cfg\\menutoolbar\\Toolbar.cfg"),CFile::modeRead|CFile::typeBinary) )
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
        newToolBarItem = NULL ;
		while (file.ReadString (strLine))
		{	
			strLine.TrimLeft ();
			strLine.TrimRight () ;
			/// 2004년 05월 13일
			/// 조병걸 수정
			/// 이거 소문자인게 있어서 모두 대문자이면 안됨
			//strLine.MakeUpper ();
               
			if(strLine.GetLength () < 2) continue ;
			if(strLine.GetAt (0) == '/') continue ;
			if(strLine.GetAt (0) == '#') continue ;

			if(strLine.GetAt (0) == '[') 
			{
				if(newToolBarItem)
				{
					m_listToolBarItem.AddTail((CObject*)newToolBarItem);
					newToolBarItem = NULL ;
				}
				if(!newToolBarItem)
				{
					newToolBarItem = new CToolBarItem ;
					// ID
					newToolBarItem->m_nID = m_nInitID++;
					
					nCount++ ;
				}
			}
			
			INT nPos = strLine.Find (_T("="))  ;
            if(nPos > 0 )
			{
				strTag = strLine.Mid (0,nPos-1) ;
				strTag.TrimLeft ();
				strTag.TrimRight () ;
				strTag.MakeUpper () ;
                
                strValue = strLine.Mid (nPos+1) ;
				strValue.TrimLeft ();
				strValue.TrimRight () ;
				/// 2004년 05월 13일
				/// 조병걸 수정
				/// 이거 소문자인게 있어서 모두 대문자이면 안됨
				//strValue.MakeUpper () ;
				
				if(strTag == _T("ALIAS")) newToolBarItem->m_strAlias = strValue ;
				
				if(strTag == _T("TOOLGROUP_ALIAS")) newToolBarItem->m_strToolGroupAlias = strValue;
				
				if(strTag == _T("IDX")) newToolBarItem->m_nIndex = _ttoi(strValue);
				
				if(strTag == _T("CAPTION")) newToolBarItem->m_strCaption = strValue;
				
				if(strTag == _T("STYLE")) newToolBarItem->m_strStyle = strValue;
				
				if(strTag == _T("TYPE")) newToolBarItem->m_strType = strValue;
				
				if(strTag == _T("SM")) newToolBarItem->m_strSM = strValue;
				
				if(strTag == _T("BITMAP_NORMAL"))newToolBarItem->m_strBitmapNormal= strValue;
				
				if(strTag == _T("BITMAP_HOT")) newToolBarItem->m_strBitmapHot = strValue;
				
				if(strTag == _T("BITMAP_DIABLED")) newToolBarItem->m_strBitmapDisabled = strValue;
				
				if(strTag == _T("TOOLTIP")) newToolBarItem->m_ToolTip = strValue;
				
				if(strTag == _T("MENU_ALIAS")) newToolBarItem->m_strMenuAlias = strValue;
				
				if(strTag == _T("ACCECLERATOR")) newToolBarItem->m_strAcceclerator = strValue;
				
				if(strTag == _T("PK")) newToolBarItem->m_strPK = strValue;
			}
		}
		if(newToolBarItem)
		{
			m_listToolBarItem.AddTail((CObject*)newToolBarItem);
			newToolBarItem = NULL ;
		}

	}
	else
	{
		AfxMessageBox(_T("Menu 파일을 Load 하지 못했습니다.")) ;
		return ;
	}

	file.Close () ;
	
	
	
	FINISH = GetTickCount();
	TRACE(_T("(Finish) - LoadToolbarRef [%d.%03d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);
}

void CKOLAS3_MAINView::LoadToolGroupRef_2()
{
	DWORD START, FINISH;
	START = GetTickCount();
	
	
	CToolBarGroup *newToolGroup;
	
	INT nCount = 0 ;
	
	CStdioFile file ;
	CString strLine ;
	CString strTag ;
    CString strValue ;
	if(file.Open (_T("..\\cfg\\menutoolbar\\ToolbarGroup.cfg"),CFile::modeRead|CFile::typeBinary) )
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
        newToolGroup = NULL ;
		while (file.ReadString (strLine))
		{	
			strLine.TrimLeft ();
			strLine.TrimRight () ;
			strLine.MakeUpper ();
               
			if(strLine.GetLength () < 2) continue ;
			if(strLine.GetAt (0) == '/') continue ;
			if(strLine.GetAt (0) == '#') continue ;

			if(strLine.GetAt (0) == '[') 
			{
				if(newToolGroup)
				{
					m_listToolBarGroup.AddTail((CObject*)newToolGroup);
					newToolGroup = NULL ;
			
				}
				if(!newToolGroup)
				{
					newToolGroup = new CToolBarGroup ;   
					nCount++ ;
				}
			}
			
			INT nPos = strLine.Find (_T("="))  ;
            if(nPos > 0 )
			{
				strTag = strLine.Mid (0,nPos-1) ;
				strTag.TrimLeft ();
				strTag.TrimRight () ;
				strTag.MakeUpper () ;
                
                strValue = strLine.Mid (nPos+1) ;
				strValue.TrimLeft ();
				strValue.TrimRight () ;
				strValue.MakeUpper () ;
				
				if(strTag == _T("ALIAS")) newToolGroup->m_strAlias = strValue ;
				
				if(strTag == _T("MENU_ALIAS")) newToolGroup->m_strMenuAlias = strValue;
				
                if(strTag == _T("ALIGN"))
				{
					if (strValue == _T("T"))
						newToolGroup->m_nDockBarID = AFX_IDW_DOCKBAR_TOP;
					else if (strValue == _T("B"))
						newToolGroup->m_nDockBarID = AFX_IDW_DOCKBAR_BOTTOM;
					else if (strValue == _T("L"))
						newToolGroup->m_nDockBarID = AFX_IDW_DOCKBAR_LEFT;
					else if (strValue == _T("R"))
						newToolGroup->m_nDockBarID = AFX_IDW_DOCKBAR_RIGHT;	
				}

			}
		}
		if(newToolGroup)
		{
			m_listToolBarGroup.AddTail((CObject*)newToolGroup);
			newToolGroup = NULL ;
		}
	}
	else
	{
		AfxMessageBox(_T("Menu 파일을 Load 하지 못했습니다.")) ;
		return ;
	}

	file.Close () ;
	
	
	
	
	FINISH = GetTickCount();
	TRACE(_T("(Finish) - LoadToolGroupRef [%d.%03d sec]\n"), (FINISH-START)/1000, (FINISH-START)%1000);
}


VOID CKOLAS3_MAINView::Get_ECOPatchDownLoader_Info()
{
	HKEY hKey ;
	INT ids ;
	CString strSetupPath ;
    //HKEY_LOCAL_MACHINE\SOFTWARE\ECO\ECOPatchDownloader
	ids = RegOpenKey( HKEY_LOCAL_MACHINE, _T("Software\\ECO\\ECOPatchDownloader"), &hKey );
    //NOT EXIST ECOPatchDownLoader
	if( ids != ERROR_SUCCESS)
	{
		//C:\\Program Files\\ECOPatchDownloader\\	
		
        //strSetupPath.Format (_T("..\\SM_SETUP\\setup.exe")) ;
        strSetupPath.Format (_T("..\\setup.exe")) ;
		
		CFileFind find ;
		if(find.FindFile (strSetupPath ) == FALSE)
		{
			return ;
		}

		CString strMsg ;
		strMsg.Format (_T("새로운 스마트 업데이트가 배포 되었습니다.\r\n")
			_T("스마트 업데이트를 설치하셔야 패치를 다운 받으실 수 있습니다.\r\n")
			_T("[확인]버튼을 누르시면 스마트 업데이트의 [SETUP.EXE]가 실행 됩니다.")) ;		   
		//AfxMessageBox(_T("새로운 스마트 업데이트가 배포 되었습니다.\r\n스마트 업데이트를 설치하셔야 패치를 다운 받으실 수 있습니다.")) ;
        AfxMessageBox(strMsg) ;
		
		m_bDownLoader = TRUE ;
		
		// 업데이트할 파일이 발견 되었으니 스마트 업데이터를 실행하고 MAIN은 종료함
		PROCESS_INFORMATION pinfo1;
		STARTUPINFO Startupinfo = {0};
		Startupinfo.cb = sizeof(STARTUPINFO);
		//::CreateProcess(NULL, _T("setup.exe"), NULL, NULL, FALSE, 0, NULL, NULL, &Startupinfo, &pinfo1);
		::CreateProcess(NULL, strSetupPath.GetBuffer (0), NULL, NULL, FALSE, 0, NULL, NULL, &Startupinfo, &pinfo1);
		GetParentFrame()->PostMessage(WM_CLOSE, 0, 0);
	}
	else
	{
		//파일 삭제 하기 
		//setup.exe
        //Autorun.inf
		//Data1.cab
		//ECOPatchDownloader.msi
		//instmsia.exe
		//instmsiw.exe
		//
		CString  strFileName = _T("");
		
		strSetupPath.Format (_T("..\\setup.exe")) ;
		
		CFileFind find ;
		if(find.FindFile (strSetupPath ) == TRUE)
		{
			CStdioFile file ;
			file.Remove (strSetupPath.GetBuffer (0)) ;
		}
        
        strFileName.Format (_T("..\\Autorun.inf")) ; 
		if(find.FindFile (strFileName ) == TRUE)
		{
			CStdioFile file ;
			file.Remove (strFileName.GetBuffer (0)) ;
		}
		strFileName.Format (_T("..\\Data1.cab")) ; 
		if(find.FindFile (strFileName ) == TRUE)
		{
			CStdioFile file ;
			file.Remove (strFileName.GetBuffer (0)) ;
		}
		strFileName.Format (_T("..\\ECOPatchDownloader.msi")) ; 
		if(find.FindFile (strFileName ) == TRUE)
		{
			CStdioFile file ;
			file.Remove (strFileName.GetBuffer (0)) ;
		}
		
		strFileName.Format (_T("..\\instmsia.exe")) ; 
		if(find.FindFile (strFileName ) == TRUE)
		{
			CStdioFile file ;
			file.Remove (strFileName.GetBuffer (0)) ;
		}
		
		strFileName.Format (_T("..\\instmsiw.exe")) ; 
		if(find.FindFile (strFileName ) == TRUE)
		{
			CStdioFile file ;
			file.Remove (strFileName.GetBuffer (0)) ;
		}		
	}         
	return ;
}


void CKOLAS3_MAINView::TabExtended(BOOL bTabExtended)
{
	m_ctlFormTab.SetExtended(bTabExtended);
	//LPCTSTR eTabOrientation  =GetProfileInt(_T("XPTabDlg"), _T("TabOrientation"),e_tabBottom);
	//TabOrientation((ETabOrientation)eTabOrientation);	// update orientation
	RedrawTab();
}
/////////////////////////////////////////////////////////////////////////////
// change tab control orientation
void CKOLAS3_MAINView::TabOrientation(ETabOrientation eTabOrientation) 
{
	m_ctlFormTab.ModifyStyle(TCS_VERTICAL|TCS_BOTTOM|TCS_MULTILINE,(eTabOrientation&1?TCS_BOTTOM:0) | 
		(eTabOrientation>e_tabBottom?(TCS_VERTICAL|(TRUE && IsThemeActiveXP()?0:TCS_MULTILINE)):0));
	CWnd* pWnd=GetDlgItem(IDC_ctlFormTab);
	if(pWnd) pWnd->EnableWindow(IsThemeActiveXP()?TRUE:FALSE);
}
/////////////////////////////////////////////////////////////////////////////
VOID CKOLAS3_MAINView::RedrawTab()
{
	CWnd* pWnd=GetDlgItem(IDC_ctlFormTab);
	if(pWnd)
	{	CRect rcInv;
	pWnd->GetWindowRect(rcInv); ScreenToClient(rcInv);
	InvalidateRect(rcInv);
	}
	else Invalidate();
}

#define IDS_UTIL_UXTHEME        _T("UxTheme.dll")
#define IDS_UTIL_THEMETEXTURE   "EnableThemeDialogTexture"
/////////////////////////////////////////////////////////////////////////////
void EnableThemeDialogTexture(HWND hwndDlg)
{
	HINSTANCE hDll=LoadLibrary(IDS_UTIL_UXTHEME);							// 'UxTheme.dll'
	if(hDll==NULL)
		return;	// the DLL won't be available on anything except Windows XP
	ULONG (PASCAL *lpfnEnableTheme)(HWND, DWORD);
	(FARPROC&)lpfnEnableTheme=GetProcAddress(hDll,IDS_UTIL_THEMETEXTURE);	// 'EnableThemeDialogTexture'
	if(lpfnEnableTheme)
		lpfnEnableTheme(hwndDlg, 6);		// ETDT_ENABLETAB = (ETDT_ENABLE | ETDT_USETABTEXTURE) = (2|4) = 6
	else ASSERT(FALSE);
	FreeLibrary(hDll);
}

BEGIN_EVENTSINK_MAP(CKOLAS3_MAINView, CFormView)
//{{AFX_EVENTSINK_MAP(CKOLAS3_MAINView)
//ON_EVENT(CKOLAS3_MAINView, IDC_FLASH_MAIN, 150 /* FSCommand */, OnFSCommandFlashMain, VTS_BSTR VTS_BSTR)
//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKOLAS3_MAINView::OnFSCommandFlashMain(LPCTSTR command, LPCTSTR args) 
{
	NewForm(args); 		
}

BOOL CKOLAS3_MAINView::ReLoadFormCheck(CString strMenuID)
{
	INT nCount = m_TabList.GetCount();
	
	TAB_ITEMS_STRU *tabitem_stru;
	POSITION pos;
	for ( int i=0 ; i<nCount ; i++ )
	{
		tabitem_stru = NULL;
		pos = m_TabList.FindIndex(i);
		if ( pos == NULL ) continue;
		tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
		
		if ( tabitem_stru->nItemType == _MENU_FORM )
		{			
			if ( tabitem_stru->MenuID == strMenuID )
			{
				m_ctlFormTab.SetCurSel(i);
				OnChangeTab();
				return TRUE;
				break;
			}
		}
	}
	
	return FALSE;
}

INT CKOLAS3_MAINView::NewForm(CString strMenuAlias)
{
	if ( ReLoadFormCheck(strMenuAlias) )
	{
		return 0;
	}
	// ======================================
	// Menu Item 찾기
	//	
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame == NULL) return -1;
	
	if ( strMenuAlias == _T("200") )
	{
#ifdef _KOLASIII
		pFrame->m_Navigator.m_strURL = _T("http://www.kolas3.net");
		LoadInternet();
#endif
		return 0;
	}
	else if ( strMenuAlias == _T("300") )
	{
		return 0;
	}
	// 2010.12.06 ADD BY KSJ : 통합도서서비스 홈페이지 추가
	else if ( strMenuAlias == _T("400") )
	{
#ifdef _KOLASIII
		pFrame->m_Navigator.m_strURL = _T("http://book.nl.go.kr");
		LoadInternet();
#endif
		return 0;
	}
	
	if ( strMenuAlias == "false" ) return 0;
	// 2008.07.03 ADD BY PWR : 메인(FLASH)의 GROUPNAME기억
	if( ( 9 < _ttoi(strMenuAlias) ) && ( 42 > _ttoi(strMenuAlias) ) ) 
	{
		if( strMenuAlias == _T("10") )
			pFrame->m_MenuBar.SetMenuDir(_T("1000"));
		else if( strMenuAlias == _T("20") )
			pFrame->m_MenuBar.SetMenuDir(_T("2000"));
		else if( strMenuAlias == _T("30") )
			pFrame->m_MenuBar.SetMenuDir(_T("3000"));
		else
			pFrame->m_MenuBar.SetMenuDir(_T("1000"));
		m_sGroupName = strMenuAlias;
		return 0;
	}
	
	INT nRows = m_listMenuItem.GetCount();
	
	POSITION pos;
	CMenuItem *pMenuItem;
	INT		nMenuCount = 0;
	
	pos = m_listMenuItem.GetHeadPosition();
	CString strUserDefaultLoad = _T("");
	UINT nUserDefaultLoad = 0;
	
	// 메뉴찾기
	CString strTopGroupAlias;
	for ( int i = 0; i < nRows; i++) {
		pMenuItem = NULL;
		pMenuItem = (CMenuItem *)m_listMenuItem.GetNext(pos);		
		if (pMenuItem->m_strMenuID == strMenuAlias ) 
		{
			nMenuCount++;
			break;
		}		
		
		/*
		if ( pMenuItem->m_strCaption == _T("") )
		{
		strTopGroupAlias = pMenuItem->m_strGroupAlias; 
		}
		*/
	}
	// 2008.09.02 ADD BY CJY : strMenuID에서 찾지 못한 경우는 m_nID로 다시 한 번 검색한다.
	// 이렇게 하는 이유는 MENU_ID값이 5자리인 경우
	// INT CMenuBarDlg::InputDate에서 ID값이 변경되기 때문이다.
	// 이때 ID값은 항상 5000이상이다.
	if(0 == nMenuCount && 5000 <= _ttoi(strMenuAlias))
	{
		pos = m_listMenuItem.GetHeadPosition();
		for ( int i = 0; i < nRows; i++)
		{		
			pMenuItem = NULL;
			pMenuItem =(CMenuItem *)m_listMenuItem.GetNext(pos);	
			UINT nAlias = _ttoi(strMenuAlias);
			if(pMenuItem->m_nID == nAlias) 
			{
		 		pMenuItem->m_nID = _ttoi(strMenuAlias);
				// 중복실행을 체크한다
				if(TRUE == ReLoadFormCheck(pMenuItem->m_strMenuID))
				{
					return 0;
				}
				nMenuCount++;
				break;
			}		
		}
	}
	if ( nMenuCount > 0 )
	{
		INT nCount = m_TabList.GetCount();	
		CString strMenu;		
		strMenu.Format(_T("%s     "), pMenuItem->m_strCaption);				
		m_ctlFormTab.InsertItem(nCount+1, strMenu, pMenuItem->m_nIconIdx );
		BOOL bShow = SetToolBarGroup(pMenuItem->m_strMenuID);
		if (LoadDll(pMenuItem, bShow) < 0) 
		{	
			// 16/06/15 김혜영 : 특정메뉴를 로딩하지 못하고 이후에 다른메뉴를 로딩하는 경우에 이전메뉴의 툴바가 표현되는 오류
//*/ ADD ----------------------------------------------------------------------------------------------------------
			CToolBar *pToolBar = NULL;	
			pToolBar = (CToolBar*)m_listToolBar.RemoveTail();
			if ( pToolBar )	delete pToolBar;
//*/ END ----------------------------------------------------------------------------------------------------------
			m_ctlFormTab.DeleteItem(nCount);
			return 0;		
		}
	}
	// 2008.08.21 ADD BY LKS : 사용권한이 없는 메뉴를 선택했을 경우 메시지 출력
	else 
	{	
		//트리바를 통해 메뉴를 선택할 경우 else로 빠지는데 (strMenuAlias가 큐브를 통해 접근한 경우와 다르다!!) 이 경우는 skip해야함
		//큐브를 통해 접근할 경우 길이가 4이므로 메세지를 출력하고
		//트리바를 통해 접근할 경우 skip해야 하므로 길이가 4인지 검사
		if(strMenuAlias.GetLength()==4)
		{
			AfxMessageBox(_T("선택하신 메뉴의 사용 권한이 없습니다. ")); 
		}
		return 0;
	}
#ifdef _KOLASIII
	if ( pFrame->m_MenuBar.GetSafeHwnd() != NULL ) pFrame->m_MenuBar.SetMenuDir(strMenuAlias);	
#endif

	//StatusBar 초기화
	pFrame->ClearStatusBarLog();
	pFrame->SetStatusBarMode_MenuForm();
	
	return 0;
}

VOID CKOLAS3_MAINView::SetFormScree()
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame == NULL) return;
	
	INT nCount = m_TabList.GetCount();
	
	TAB_ITEMS_STRU *tabitem_stru;
	CToolBar *pToolBar;
	POSITION posToolBarIndex;
	POSITION pos;
	for ( int i=0 ; i<nCount ; i++ )
	{
		tabitem_stru = NULL;
		pos = m_TabList.FindIndex(i);
		if ( pos == NULL ) continue;
		tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
		
		switch ( tabitem_stru->nItemType )
		{
		case _MAIN_FORM :
			// 18.11.07 JMJ KOL.RED.2018.020
			/*
			if ( m_nTabIndex == tabitem_stru->nItemIndex ) 
			{
				CRect rect;
				GetClientRect(rect);
				rect.top = _FLASH_TOP;
				m_ctrlMainFlash.MoveWindow(rect);					
				m_ctrlMainFlash.ShowWindow(SW_SHOW);
				// 2008.07.02 ADD BY PWR : 저장된GROUPNAME으로 메인(FLASH)을 보여줌
				if( m_sGroupName == _T("10") )
					pFrame->m_MenuBar.SetMenuDir(_T("1000"));
				else if( m_sGroupName == _T("20") )
					pFrame->m_MenuBar.SetMenuDir(_T("2000"));
				else if( m_sGroupName == _T("30") )
					pFrame->m_MenuBar.SetMenuDir(_T("3000"));
				else
					pFrame->m_MenuBar.SetMenuDir(_T("1000"));
			}
			else 
			{
				m_ctrlMainFlash.MoveWindow(0,0,0,0);
			}
			pFrame->m_Navigator.SetURL(_T(""));				
			*/
			if ( m_nTabIndex == tabitem_stru->nItemIndex ) 
			{					
				m_MainExplorer->ShowWindow(SW_SHOW);
				pFrame->m_Navigator.SetURL(m_MainExplorer->GetURL() );
				if( m_sGroupName == _T("10") )
					pFrame->m_MenuBar.SetMenuDir(_T("1000"));
				else if( m_sGroupName == _T("20") )
					pFrame->m_MenuBar.SetMenuDir(_T("2000"));
				else if( m_sGroupName == _T("30") )
					pFrame->m_MenuBar.SetMenuDir(_T("3000"));
				else
					pFrame->m_MenuBar.SetMenuDir(_T("1000"));
			}
			else m_MainExplorer->ShowWindow(SW_HIDE);
			break;
		case _MENU_FORM :
			pToolBar = NULL;
			posToolBarIndex = NULL;
			if ( tabitem_stru->nToolBarID != -1 )
			{
				posToolBarIndex = m_listToolBar.FindIndex(tabitem_stru->nToolBarID);				
			}
			
			if ( posToolBarIndex != NULL ) pToolBar = (CToolBar*)m_listToolBar.GetAt(posToolBarIndex);
			if ( m_nTabIndex == tabitem_stru->nItemIndex ) 
			{
				m_pForm[tabitem_stru->nFormIndex]->Show(TRUE);					
				if ( pToolBar != NULL ) pToolBar->ShowWindow(SW_SHOW);															
				m_strCurMenuAlias = tabitem_stru->MenuAlias;
			}					
			else 
			{
				m_pForm[tabitem_stru->nFormIndex]->Show(FALSE);
				if ( pToolBar != NULL ) pToolBar->ShowWindow(SW_HIDE);	
			}
			
			if ( m_nTabIndex == tabitem_stru->nItemIndex ) 
			{
				m_pForm[tabitem_stru->nFormIndex]->Show(TRUE);					
				if ( pToolBar != NULL )
				{	
					CRect rRect;
					m_ctlFormTab.GetClientRect(rRect);
					pToolBar->ShowWindow(SW_SHOW);	
					pToolBar->UpdateWindow();
					((CMToolBar*)pToolBar)->ToolBarSizing(rRect.Width());
				}
				m_strCurMenuAlias = tabitem_stru->MenuAlias;
				// 2008.07.03 ADD BY PWR : 해당 메뉴를 보여줌
				pFrame->m_MenuBar.SetMenuDir(tabitem_stru->MenuID);
			}					
			else 
			{
				m_pForm[tabitem_stru->nFormIndex]->Show(FALSE);
				if ( pToolBar != NULL )
				{
					pToolBar->ShowWindow(SW_HIDE);	
				}
			}
			pFrame->m_Navigator.SetURL(_T(""));

			break;
		case _INET_FORM :
			if ( m_nTabIndex == tabitem_stru->nItemIndex ) 
			{					
				m_Explorer[tabitem_stru->nInternetIndex]->ShowWindow(SW_SHOW);
				pFrame->m_Navigator.SetURL(m_Explorer[tabitem_stru->nInternetIndex]->GetURL() );
				// 2008.07.03 ADD BY PWR : 기본메뉴를 보여줌
				pFrame->m_MenuBar.SetMenuDir(_T("1000"));
			}
			else m_Explorer[tabitem_stru->nInternetIndex]->ShowWindow(SW_HIDE);
			break;
		default:
			break;
		}
	}
	m_ctlFormTab.SetCurSel(m_nTabIndex);
	//m_ctlFormTab.Invalidate(TRUE);
	//Invalidate(TRUE);
	/*
	for ( int i=0 ; i<=m_nFormCount ; i++ )
	{
	if ( i == 0 )
	{
	m_ctrlMainFlash.ShowWindow(FALSE);			
	}
	else if ( m_nInternetIndex == i )
	{
	m_Explorer->ShowWindow(FALSE);
	}
	else
	{
	m_pForm[i]->Show(FALSE);
	}
	}
	
	  if ( m_nFormIndex == 0 )
	  {
	  m_ctrlMainFlash.ShowWindow(TRUE);			
	  }
	  else if ( m_nFormIndex == m_nInternetIndex  )
	  {
	  m_Explorer->ShowWindow(TRUE);
	  }
	  else
	  {
	  m_pForm[m_nFormIndex]->Show(TRUE);
	  }
	  
		
		  //m_listToolBar.Get.AddTail((CObject *)pToolBar);	
		  INT nCount = m_listToolBar.GetCount();	
		  if ( nCount > 0 && m_nFormIndex > 0 )
		  {
		  CToolBar *pToolBar;
		  for (INT i = 0; i < nCount; i++) 
		  {
		  pToolBar = NULL;
		  pToolBar = (CToolBar*)m_listToolBar.GetAt(m_listToolBar.FindIndex(i));
		  pToolBar->ShowWindow(SW_HIDE);		
		  }	
		  pToolBar = (CToolBar*)m_listToolBar.GetAt(m_listToolBar.FindIndex(m_nFormIndex-1));
		  pToolBar->ShowWindow(SW_SHOW);
		  }
		  
			
			  m_ctlFormTab.SetCurSel(m_nFormIndex);		
	*/
}

void CKOLAS3_MAINView::OnSelchangectlFormTab(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	*pResult = 0;
}

VOID CKOLAS3_MAINView::OnChangeTab()
{
	TAB_ITEMS_STRU *tabitem_stru;
	POSITION pos;
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();

	// 17/11/29 김혜영 : 메뉴간 이동시에도 자동로그아웃 되는 현상 수정
//*/ ADD ---------------------------------------------------------------------------
	m_nTimeOut = 0;
//*/ END ---------------------------------------------------------------------------

	// Tab 이동전에 현재탭의 띄어져 있는 모달리스 대화상자를 숨긴다.
	ShowHideModeless(m_nTabIndex, SW_HIDE);	

	m_nTabIndex = m_ctlFormTab.GetCurSel();
	SetFormScree();
	
	// Tab 이동후 현재 Tab의 보여졌었던 탭을 활성화 시킨다.
	ShowHideModeless(m_nTabIndex, SW_SHOW);
	
	// Menu Tree Bar를 선택된 곳으로 이동시킨다.	
	tabitem_stru = NULL;
	pos = m_TabList.FindIndex(m_nTabIndex);
	if ( pos == NULL ) return;
	tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
	
	if ( tabitem_stru->nItemType == _MENU_FORM )
	{
		// 2008.07.02 ADD BY LKS : Tab이 변경될 때마다 변경되는 탭이 갖고 있던 WorkingTime과 검색된 Row갯수를 상태바에 출력한다.
		pos = m_TabList.FindIndex(m_nTabIndex);
		tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
		pFrame->ClearStatusBarLog();
		pFrame->SetStatusBarMode_MenuForm();
		pFrame->GetStatusBar()->SetPaneText(3, tabitem_stru->SearchedRow );
		pFrame->GetStatusBar()->SetPaneText(4, tabitem_stru->WorkingTime );
		
		CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
#ifdef _KOLASIII
		if ( pFrame->m_MenuBar.GetSafeHwnd() != NULL ) pFrame->m_MenuBar.SetMenuDir(tabitem_stru->MenuID);	
#endif
	}
	
	else if(tabitem_stru->nItemType == _MAIN_FORM || tabitem_stru->nItemType == _INET_FORM)
	{
		//StatusBar 초기화
		pFrame->ClearStatusBarLog();
		pFrame->GetStatusBar()->SetPaneText(5, _T(""));
		pFrame->GetStatusBar()->SetPaneText(6, _T(""));
		pFrame->SetStatusBarMode_InternetForm();
	}
	
	else
	{
#ifdef _KOLASIII
		CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
		if ( pFrame->m_MenuBar.GetSafeHwnd() != NULL ) 
		{
			CString sClass;
			sClass = tabitem_stru->MenuID;
			if( sClass == _T("단행") )
			{
				pFrame->m_MenuBar.SetMenuDir(_T("1000"));	
			}
			else if( sClass == _T("연속") )
			{
				pFrame->m_MenuBar.SetMenuDir(_T("2000"));	
			}
			else if( sClass == _T("공통") )
			{
				pFrame->m_MenuBar.SetMenuDir(_T("3000"));	
			}
			else
			{
				pFrame->m_MenuBar.SetMenuDir(_T("1000"));	
			}
		}
#endif
	}
#ifdef _KOLASIII
	pFrame->m_MenuBar.Invalidate();
#endif
}

VOID CKOLAS3_MAINView::ShowHideModeless(INT nTabIndex, INT nShow)
{
	TAB_ITEMS_STRU *tabitem_stru;	
	POSITION pos;
	tabitem_stru = NULL;
	pos = m_TabList.FindIndex(nTabIndex);
	if ( pos == NULL ) return;
	tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
	
	if ( tabitem_stru->nItemType != _MENU_FORM ) return;
	
	if ( m_pForm[tabitem_stru->nFormIndex] == NULL ) return;	
	
	// check dialog
	pos = m_listModelessDlg[tabitem_stru->nFormIndex].GetHeadPosition();
	while (pos)
	{
		CModelessDlg *pModelessDlg = (CModelessDlg*)m_listModelessDlg[tabitem_stru->nFormIndex].GetNext(pos);		
		if(nShow == SW_SHOW)
		{
			if(pModelessDlg->m_bShow == TRUE)
			{
				pModelessDlg->m_pESL_Mgr->ShowWindow(SW_SHOWNOACTIVATE);
			}		
		}
		else
		{	
			if ( (pModelessDlg->m_pESL_Mgr->GetStyle() & WS_VISIBLE) != 0 )
			{
				pModelessDlg->m_pESL_Mgr->ShowWindow(SW_HIDE);
				pModelessDlg->m_bShow = TRUE;
			}
			else
			{
				pModelessDlg->m_bShow = FALSE;
			}
		}
	}	
}

VOID CKOLAS3_MAINView::OnInternet()
{
	CreateKolisnetWebSearchManager();
	LoadInternet();	
}

INT CKOLAS3_MAINView::LoadInternet()
{	
#ifndef _KOLASIII_TL
	if ( m_nExplorerCount >= 20 )
	{
		AfxMessageBox(_T("사용되지 않는 인터넷 화면을 닫고 선택하세요."));
		return -1;
	}
	
	ShowHideModeless(m_nTabIndex, SW_HIDE);	
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame == NULL) return -1;
	// 새로운 폼을 생성한다.	
	INT nCount = m_TabList.GetCount();
	// 없으면 빈페이지로 뜨게한다.
	pFrame->m_Navigator.m_strURL.TrimLeft();
	pFrame->m_Navigator.m_strURL.TrimRight();
	if(pFrame->m_Navigator.m_strURL.IsEmpty())
	{
		pFrame->m_Navigator.m_strURL = _T("about:blank");
	}
	m_ctlFormTab.InsertItem(nCount, pFrame->m_Navigator.m_strURL+_T("     "), 1);
	
	
	CRect rect;
	GetClientRect(rect);	
	rect.top = _INET_TOP;
	rect.right += 5;
	//rect.bottom -= 52;	
	
	CString strBody;
	CString strTitle;
	m_Explorer[m_nExplorerCount] = new CExplorer();
	m_Explorer[m_nExplorerCount]->SetURL(pFrame->m_Navigator.GetURL());
	m_Explorer[m_nExplorerCount]->Create(NULL, _T("HtmlView"), WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN, rect, this, AFX_IDW_PANE_FIRST);
	m_Explorer[m_nExplorerCount]->MoveWindow(rect);	
	m_Explorer[m_nExplorerCount]->m_nInternetIndex = m_nExplorerCount;
	
	//m_Explorer[m_nExplorerCount]->m_HtmlView->GetHtmlDocument(strBody);
	//m_Explorer[m_nExplorerCount]->m_HtmlView->GetFullName(strBody);
	
	TAB_ITEMS_STRU *tabitem_stru;
	tabitem_stru = NULL;
	tabitem_stru = new TAB_ITEMS_STRU;
	tabitem_stru->nItemType = _INET_FORM;
	tabitem_stru->nItemIndex = nCount;
	tabitem_stru->nFormIndex = -1;
	tabitem_stru->nInternetIndex = m_nExplorerCount;
	tabitem_stru->ExplorerStatusText = _T("");
	tabitem_stru->WorkingTime = _T("");
	tabitem_stru->SearchedRow = _T("");
	tabitem_stru->pEslMgr = NULL;
	m_TabList.AddTail((CObject*)tabitem_stru);
	m_nTabIndex = nCount;
	
	m_nExplorerCount++;
	
	SetFormScree();
	
	//++2008.07.07 ADD BY  {{++
	//탭의 상태에 따라 상태바의 구성을 달리 한다.
	//StatusBar 초기화
	pFrame->ClearStatusBarLog();
	pFrame->SetStatusBarMode_InternetForm();
	//--2008.07.07 ADD BY LKS --}}
#endif

	return 0;
}

VOID CKOLAS3_MAINView::OnCloseForm()
{	
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	INT nCount = m_TabList.GetCount();
	INT nCloseType =-1;
	
	TAB_ITEMS_STRU *tabitem_stru;
	CToolBar *pToolBar;
	
	tabitem_stru = NULL;
	POSITION pos, pos2,posTemp;
	pos = m_TabList.FindIndex(m_nTabIndex);

	BOOL bIsMinusIndex = TRUE;
	
	//++2008.07.07 ADD BY LKS {{++
	//Tab을 닫는 경우 현재 마지막 Tab이라면 이전 Tab으로 이동하므로 이전 탭의 정보 출력,
	//마지막 탭이 아니라면 다음 Tab으로 이동하므로 다음 Tab의 정보 출력.	
	INT m_nCurSel = m_ctlFormTab.GetCurSel();
	m_nCurSel+=1;
	if(m_nCurSel == nCount) //마지막 Tab이라면
	{
		posTemp = m_TabList.FindIndex(m_nTabIndex-1);
	}
	else //다음 Tab이 존재한다면
	{
		posTemp = m_TabList.FindIndex(m_nTabIndex+1);
	}
	if(NULL == posTemp) return;

	tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(posTemp);
	
	if(tabitem_stru->nItemType==_MENU_FORM) //탭의 타입에 따라 상태바 구성을 달리 한다.
	{
		pFrame->SetStatusBarMode_MenuForm();
	}
	else if(tabitem_stru->nItemType==_MAIN_FORM || tabitem_stru->nItemType==_INET_FORM)
	{
		pFrame->GetStatusBar()->SetPaneText(5, _T(""));
		pFrame->GetStatusBar()->SetPaneText(6, _T(""));
		pFrame->SetStatusBarMode_InternetForm();
	}
	pFrame->GetStatusBar()->SetPaneText(0, _T(""));//인터넷 폼에서 출력한 링크 정보 -> " "
	pFrame->GetStatusBar()->SetPaneText(3, tabitem_stru->SearchedRow );
	pFrame->GetStatusBar()->Invalidate();
	pFrame->GetStatusBar()->SetPaneText(4, tabitem_stru->WorkingTime );
	
	//--2008.07.02 ADD BY LKS --}}
	

	if ( pos == NULL ) return;
	tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
	
	switch ( tabitem_stru->nItemType )
	{
	case _MAIN_FORM :				
		break;
	case _MENU_FORM :
		pToolBar = NULL;
		if ( tabitem_stru->nToolBarID != -1 ) 
		{
			pos = m_listToolBar.FindIndex(tabitem_stru->nToolBarID);
			if ( pos != NULL )
			{
				pToolBar = (CToolBar*)m_listToolBar.GetAt(pos);
				pToolBar->ShowWindow(SW_HIDE);
				delete pToolBar;
				pToolBar = NULL;
				m_listToolBar.RemoveAt(pos);
				m_nToolBarCount--;
			}
		}			
		FreeModelessDlg(tabitem_stru->nFormIndex);
		
		if(tabitem_stru->pEslMgr!=NULL)
		{
			tabitem_stru->pEslMgr = NULL;
		}
		delete m_pForm[tabitem_stru->nFormIndex];	
		
		m_pForm[tabitem_stru->nFormIndex] = NULL;
		m_nMenuFormCount--;
		nCloseType = _MENU_FORM;
		if(-1 == tabitem_stru->nToolBarID)
		{
			bIsMinusIndex = FALSE;
		}
		break;
	case _INET_FORM :					
		delete m_Explorer[tabitem_stru->nInternetIndex];
		m_Explorer[tabitem_stru->nInternetIndex] = NULL;
		m_nExplorerCount--; 
		nCloseType = _INET_FORM;
		break;
	default:
		break;
	}
	
	// 2009.04.02 ADD BY CJY : 메모리누수 제거
	if(NULL != tabitem_stru)
	{
		delete tabitem_stru;
		tabitem_stru = NULL;
	}

	INT nDeleteIndex;
	nDeleteIndex = m_nTabIndex;
	pos = m_TabList.FindIndex(nDeleteIndex);
	if ( pos != NULL ) m_TabList.RemoveAt(pos);
	if ( m_nTabIndex == nCount-1 )
	{
		m_nTabIndex--;		
	}
	else
	{
		for ( int i=m_nTabIndex ; i<nCount-1 ; i++ )
		{
			tabitem_stru = NULL;
			pos = m_TabList.FindIndex(i);
			if ( pos == NULL ) continue;
			tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
			// Tab Index 재부여
			tabitem_stru->nItemIndex -= 1;		
			// Form또는 Internet이 닫혔을 경우 해당 Index 재정렬
			switch ( tabitem_stru->nItemType )
			{			
			case _MENU_FORM :
				if ( nCloseType == _MENU_FORM )
				{
					// Form 이동
					m_pForm[tabitem_stru->nFormIndex-1] = m_pForm[tabitem_stru->nFormIndex];		
					
					// modeless이동
					pos2 = m_listModelessDlg[tabitem_stru->nFormIndex].GetHeadPosition();
					while (pos2)
					{
						m_listModelessDlg[tabitem_stru->nFormIndex-1].AddTail((CObject*)m_listModelessDlg[tabitem_stru->nFormIndex].GetNext(pos2));							
						if ( pos2 == NULL ) break;
					}
					m_listModelessDlg[tabitem_stru->nFormIndex].RemoveAll();
					
					// Index 이동
					tabitem_stru->nFormIndex = tabitem_stru->nFormIndex-1;
					
					if ( tabitem_stru->nToolBarID != -1 )
					{
						if(bIsMinusIndex)
						{
							tabitem_stru->nToolBarID--;
						}
					}
					
					
				}
				break;
			case _INET_FORM :		
				if ( nCloseType == _INET_FORM )
				{
					m_Explorer[tabitem_stru->nInternetIndex-1] = m_Explorer[tabitem_stru->nInternetIndex];
					tabitem_stru->nInternetIndex = tabitem_stru->nInternetIndex-1; 
				}
				break;
			default:
				break;
			}	
			pos = m_TabList.FindIndex(i);
			if ( pos != NULL ) m_TabList.SetAt(pos, (CObject*)tabitem_stru);
		}	
	}
	
	m_ctlFormTab.SetCurSel(m_nTabIndex);	
	ShowHideModeless(m_nTabIndex, SW_SHOW);
	m_ctlFormTab.DeleteItem(nDeleteIndex);	
	SetFormScree();				
	m_ctlFormTab.Invalidate(FALSE);
}



CToolBar *CKOLAS3_MAINView::CreateToolBar(UINT nDockBarID /*=0*/)
{	
	CMToolBar * pToolBar = NULL;
	pToolBar = new CMToolBar();
	
	//if (!pToolBar->CreateEx(pFrame, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP |
	//	CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	if (!pToolBar->CreateEx(this, 0, WS_CHILD | WS_VISIBLE | CBRS_TOP |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ^ WS_BORDER))		
	{
		delete pToolBar;
		return NULL;
	}
	
	//pToolBar->EnableDocking(CBRS_ALIGN_ANY);
	//pFrame->DockControlBar(pToolBar, nDockBarID);
	//HBITMAP m_hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T("G:\\Kolas2up_uni\\bmp\\kolisnetwebsearchmiddle.bmp"),
	//	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	//pToolBar->SetBitmap(m_hBitmap );
	
	return pToolBar;
}

VOID CKOLAS3_MAINView::OnWebTitle(WPARAM wParam,LPARAM lParam)
{	
	TAB_ITEMS_STRU *tabitem_stru;
	tabitem_stru = NULL;	
	
	INT nExplorerIndex = (INT)wParam;
	INT nTabIndex = GetTabIndex(nExplorerIndex);
	
	// tab이 변경될수도 있음 m_nTabIndex가 아니라 Internet Index로 탭을 찾는다.	
	POSITION pos;
	pos = m_TabList.FindIndex(nTabIndex);
	if ( pos == NULL ) return;
	tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);	
	
	//if (m_nTabIndex == 0 ) return;	
	
	CString strTitle;
	strTitle = m_Explorer[tabitem_stru->nInternetIndex]->GetTitle() + _T("     ");
	m_ctlFormTab.SetSelText(nTabIndex, strTitle ); 	
}

VOID CKOLAS3_MAINView::OnKolisnetSearchStart()
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
#ifdef _KOLASIII
	pFrame->m_MenuBar.SetCount(_T("  S"));
#endif
}

VOID CKOLAS3_MAINView::OnKolisnetSearchFind(WPARAM wParam,LPARAM lParam)
{
	// 웹검색 메니져에게 ISBN이 선택되었음을 통지.
	CString *strISBN = (CString*)lParam;	
	
	m_KolisNetWebSearch->FindISBN(*strISBN);
}

VOID CKOLAS3_MAINView::OnChangeURL(WPARAM wParam,LPARAM lParam)
{
#ifdef _KOLASIII
	TAB_ITEMS_STRU *tabitem_stru;
	tabitem_stru = NULL;	
	
	INT nExplorerIndex = (INT)wParam;
	INT nTabIndex = GetTabIndex(nExplorerIndex);	
	
	POSITION pos;
	pos = m_TabList.FindIndex(nTabIndex);
	if ( pos == NULL ) return;	
	tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);	
	
	if ( tabitem_stru->nItemType != _INET_FORM ) return;
	
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CString strURL = m_Explorer[tabitem_stru->nInternetIndex]->GetURL();
	if ( strURL == _T("http:///") ) return;
	m_Explorer[tabitem_stru->nInternetIndex]->SetURL(strURL);
	
	CString strCurrentURL = pFrame->m_Navigator.GetURL();
	
	if ( strCurrentURL != strURL ) 
	{
		if ( m_nTabIndex != nTabIndex) return;
		pFrame->m_Navigator.SetURL(strURL, TRUE);
	}
#endif
}
VOID CKOLAS3_MAINView::OnKolisnetSearch(WPARAM wParam,LPARAM lParam)
{		
	TAB_ITEMS_STRU *tabitem_stru;
	tabitem_stru = NULL;	
	
	INT nExplorerIndex = (INT)wParam;
	INT nTabIndex = GetTabIndex(nExplorerIndex);
	
	POSITION pos;
	pos = m_TabList.FindIndex(nTabIndex);
	if ( pos == NULL ) return;	
	tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);	
	
	if ( tabitem_stru->nItemType != _INET_FORM ) return;
	
	CString strISBNList;
	strISBNList = _T("");
	
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	INT nCount = m_Explorer[tabitem_stru->nInternetIndex]->m_pISBNList.GetSize();	
#ifdef _KOLASIII
	if ( nCount == 0 )
	{		
		pFrame->m_MenuBar.SetCount(_T("  X"));				
	}
	else
	{
		if ( m_KolisNetWebSearch != NULL ) pFrame->m_MenuBar.SetCount(m_KolisNetWebSearch->GetResultCount());
	}
#endif

	if ( m_KolisNetWebSearch != NULL ) m_KolisNetWebSearch->Search(m_Explorer[tabitem_stru->nInternetIndex]->m_pISBNList);	
}


VOID CKOLAS3_MAINView::OnExplorerPrev()
{	
	TAB_ITEMS_STRU *tabitem_stru;	
	POSITION pos;
	
	tabitem_stru = NULL;
	pos = m_TabList.FindIndex(m_nTabIndex);
	if ( pos == NULL ) return;
	tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
	if ( tabitem_stru->nItemType == _INET_FORM ) m_Explorer[tabitem_stru->nInternetIndex]->m_HtmlView->GoBack();
	
}
VOID CKOLAS3_MAINView::OnExplorerNext()
{
	TAB_ITEMS_STRU *tabitem_stru;	
	POSITION pos;
	
	tabitem_stru = NULL;
	pos = m_TabList.FindIndex(m_nTabIndex);
	if ( pos == NULL ) return;
	tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
	if ( tabitem_stru->nItemType == _INET_FORM ) m_Explorer[tabitem_stru->nInternetIndex]->m_HtmlView->GoForward();
}
VOID CKOLAS3_MAINView::OnExplorerReflash()
{
	TAB_ITEMS_STRU *tabitem_stru;	
	POSITION pos;
	
	tabitem_stru = NULL;
	pos = m_TabList.FindIndex(m_nTabIndex);
	if ( pos == NULL ) return;
	tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
	if ( tabitem_stru->nItemType == _INET_FORM ) m_Explorer[tabitem_stru->nInternetIndex]->m_HtmlView->Refresh();
}
VOID CKOLAS3_MAINView::OnExplorerStop()
{
	TAB_ITEMS_STRU *tabitem_stru;	
	POSITION pos;
	
	tabitem_stru = NULL;
	pos = m_TabList.FindIndex(m_nTabIndex);
	if ( pos == NULL ) return;
	tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
	if ( tabitem_stru->nItemType == _INET_FORM ) m_Explorer[tabitem_stru->nInternetIndex]->m_HtmlView->Stop();
}

VOID CKOLAS3_MAINView::CreateKolisnetWebSearchManager()
{
	if ( m_KolisNetWebSearch == NULL )
	{
		CRect inetRect;
		CRect ksRect;
		this->GetClientRect(inetRect);
		ClientToScreen(inetRect);
		m_KolisNetWebSearch = new CKOLISNetWebSearchMgr();
		m_KolisNetWebSearch->SetParentInfo(m_pInfo);
		m_KolisNetWebSearch->Create(_T(""), _T(""), WS_CHILD|WS_VISIBLE, inetRect, this, 0 );				
		m_KolisNetWebSearch->GetClientRect(ksRect);
		m_KolisNetWebSearch->MoveWindow(inetRect.right - ksRect.Width()-16, inetRect.top+0, ksRect.Width(), ksRect.Height() );
	}	
}

VOID CKOLAS3_MAINView::OnKolisnetWebSearchShowHide()
{
	CreateKolisnetWebSearchManager();	
	
	if ( m_KolisNetWebSearch != NULL )
	{	
		if ( ( m_KolisNetWebSearch->GetStyle() & WS_VISIBLE) != 0 )
		{
			m_KolisNetWebSearch->ShowWindow(SW_HIDE);
		}
		else
		{
			if ( m_bKolisnetWebSearchFirstShow )
			{
				CRect inetRect;
				CRect ksRect;
				this->GetClientRect(inetRect);
				ClientToScreen(inetRect);
				m_KolisNetWebSearch->GetClientRect(ksRect);
				m_KolisNetWebSearch->MoveWindow(inetRect.right - ksRect.Width()-16, inetRect.top+0, ksRect.Width(), ksRect.Height() );
				
				m_bKolisnetWebSearchFirstShow = FALSE;
			}
			m_KolisNetWebSearch->ShowWindow(SW_SHOWNOACTIVATE);
		}
	}
}

VOID CKOLAS3_MAINView::OnKolisnetWebSearchCount()
{
	if ( m_KolisNetWebSearch != NULL ) 
	{
		CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
#ifdef _KOLASIII
		pFrame->m_MenuBar.SetCount(m_KolisNetWebSearch->GetResultCount());
#endif
	}
}

VOID CKOLAS3_MAINView::OnFurnishWebSearch(WPARAM wParam,LPARAM lParam)
{	
	CString *pStrFurnishInfo = (CString*)wParam;
	CString strMsg;
	
	
	if ( m_strFurnishISBN == *pStrFurnishInfo ) 
	{
		m_strFurnishISBN = _T("");
		return;
	}
	
	strMsg.Format(_T(" %s일 신청되었습니다"), *pStrFurnishInfo);
	
	UINT uFlagOption = CBalloonHelp::unCLOSE_ON_KEYPRESS|
		CBalloonHelp::unDISABLE_XP_SHADOW|
		CBalloonHelp::unSHOW_CLOSE_BUTTON;
	
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	
	CBalloonHelp::LaunchBalloon(_T("비치희망 신청 정보안내"),               // 타이틀
		strMsg,  // 텍스트
		point,                                             // 화면 좌표
		IDI_INFORMATION,                              // 아이콘 종류
		uFlagOption,                                  // 플래그 옵션
		this,                                               // 부모 윈도우
		_T(""),                                            // 하이퍼링크(풍선도움말 클릭시 이동되는 곳)
		5000                                                   // 도움말이 얼마동안 보여질 것인지 시간설정
		);
	
	m_strFurnishISBN.Format(_T("%s"), *pStrFurnishInfo );
}

VOID CKOLAS3_MAINView::OnKolisnetWebSearchStart()
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
#ifdef _KOLASIII
	pFrame->m_MenuBar.SetCount(-1);
#endif
}

VOID CKOLAS3_MAINView::FileSaveHwnd()
{	
	CStdioFile sf;
	CString strHandle;
	sf.Open(_T("Khandle.dat"), CFile::modeCreate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite);	
	strHandle.Format(_T("%d"), GetSafeHwnd());
	sf.WriteString(strHandle);	
	sf.Close();	
}

VOID CKOLAS3_MAINView::OnExecuteItem(WPARAM wParam,LPARAM lParam)
{	
	if ( m_nMenuFormCount >= 20 ) 
	{
		AfxMessageBox(_T("사용되지 않는 업무 화면을 닫고 선택하세요."));
		return;
	}
	
	if ( m_bLock == TRUE ) return;
	
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CString str = (LPCTSTR)(lParam);
	pFrame->LockWindowUpdate();
	NewForm(str);
	pFrame->UnlockWindowUpdate();
	// 18.12.14 JMJ KOL.RED.2018.020 보완
	// 메뉴비활성화 수정
	pFrame->SetActiveView(this);
}

// 18.11.07 JMJ KOL.RED.2018.020
VOID CKOLAS3_MAINView::OnExecuteWebItem(WPARAM wParam,LPARAM lParam)
{	
	if ( m_nMenuFormCount >= 20 ) 
	{
		AfxMessageBox(_T("사용되지 않는 업무 화면을 닫고 선택하세요."));
		return;
	}
	
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CString str = (LPCTSTR)(lParam);
	NewForm(str);

	//pFrame->UnlockWindowUpdate();
}
// 18.11.07 JMJ KOL.RED.2018.020
VOID CKOLAS3_MAINView::OnExecuteWebMoveItem(WPARAM wParam,LPARAM lParam)
{	
	if ( m_nMenuFormCount >= 20 ) 
	{
		AfxMessageBox(_T("사용되지 않는 업무 화면을 닫고 선택하세요."));
		return;
	}

	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame == NULL) return ;
		
	pFrame->m_Navigator.m_strURL = (LPCTSTR)(lParam);
	LoadInternet();
}

LRESULT CKOLAS3_MAINView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{	
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame != NULL && m_bLock == FALSE ) 
	{
		
		CString strItem;
		strItem.Format(_T("%d"), lParam);
		if ( message == 1111 )
		{
			NewForm(strItem);
		}
		else if (message == 1112)
		{		
			CString strFAddress;
			INT		ids;
			ids = GetCurIAddress(strFAddress);
			if ( !ids ) 
			{
#ifdef _KOLASIII
				if ( pFrame->m_Navigator.GetSafeHwnd() != NULL ) pFrame->m_Navigator.m_strURL = strFAddress;
				LoadInternet();
#endif
			}
		}
		else if(message == 1113)
		{		
			KalendarAutoSearch();			
		}
		
	}

	return CFormView::WindowProc(message, wParam, lParam);
}

INT CKOLAS3_MAINView::GetCurIAddress(CString &Faddress)
{
	CStdioFile sf;
	if ( sf.Open(_T("IAddress.dat"), CFile::modeRead | CFile::typeBinary ) ==0 ) return -1;
	
	TCHAR cUni;
	sf.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		sf.SeekToBegin();
	}
	
	CString strLine;
	sf.ReadString(strLine);
	strLine.TrimRight();	
	Faddress = strLine;
	
	sf.Close();
	
	
	return 0;	
}

void CKOLAS3_MAINView::OnMove(int x, int y) 
{	
	CFormView::OnMove(x, y);	
}

void CKOLAS3_MAINView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnMouseMove(nFlags, point);
}

void CKOLAS3_MAINView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
// << 2015년 공공도서관 시스템 개선 2015/11/11 _LCM : 자동로그아웃기능
	m_nTimeOut = 0;
// >> 2015/11/11 --ADD
	
	CFormView::OnLButtonUp(nFlags, point);
}

INT	CKOLAS3_MAINView::MenuItemLoad()
{
/*	
FILE	*fp;
CString strTemp;
_TCHAR	szFileName[64];	
_TCHAR	szTemp[1024];
_TCHAR	*pToken;
INT		nIndex;
INT		line_length;

  _stprintf(szFileName, _T("..\\Cfg\\MenuList.dat") );
  fp = FdkFopen( szFileName, _T("r") );
  if ( fp == NULL ) {return FALSE;}
  
	line_length = 0;		
	
	  while ( 1 ) 
	  {
	  if (CECO_FileIO::next_line(fp, szTemp, &line_length) == -1) { break; }
	  if ( szTemp == NULL ) break;
	  if ( szTemp[0] == _T('\n') || szTemp[0] == _T('.') ) continue;
	  CECO_FileIO::str_rtrim(szTemp, szTemp, '\n');
	  
		pToken = _tcstok(szTemp, _T(","));
		if ( pToken == NULL ) return -100;
		m_nMenuItemCount ++;
		if ( m_nMenuItemCount == 1 )
		{
		m_pMENU_ITEM_STRU = (MENU_ITEM_STRU*) malloc ( sizeof(MENU_ITEM_STRU) * m_nMenuItemCount );
		if ( m_pMENU_ITEM_STRU == NULL ) return -100;
		}
		else
		{
		m_pMENU_ITEM_STRU = (MENU_ITEM_STRU*) realloc ( m_pMENU_ITEM_STRU, sizeof(MENU_ITEM_STRU) * m_nMenuItemCount );
		if ( m_pMENU_ITEM_STRU == NULL ) return -100;
		}			
		
		  
			_tcscpy(m_pMENU_ITEM_STRU[m_nMenuItemCount-1].szPublishFormCode, pToken);
			nIndex = 0;
			while ( 1 ) 
			{
			pToken = _tcstok(NULL, _T(","));
			if ( pToken == NULL ) break;			
			
			  switch ( nIndex )
			  {
			  case 0:
			  _tcscpy(m_pMENU_ITEM_STRU[m_nMenuItemCount-1].szGroup	, pToken );
			  break;
			  case 1:
			  _tcscpy(m_pMENU_ITEM_STRU[m_nMenuItemCount-1].szCaption	, pToken );
			  break;
			  case 2:
			  _tcscpy(m_pMENU_ITEM_STRU[m_nMenuItemCount-1].szAlias	, pToken );
			  break;
			  case 3:
			  _tcscpy(m_pMENU_ITEM_STRU[m_nMenuItemCount-1].szItemNo	, pToken );
			  break;
			  case 4:
			  _tcscpy(m_pMENU_ITEM_STRU[m_nMenuItemCount-1].szItemAlias, pToken );
			  break;
			  default:
			  break;
			  }
			  
				nIndex ++;			
				}	
				}
				
				  fclose(fp);
	*/
	return 0;
	
}

INT CKOLAS3_MAINView::GetTabIndex(INT nInternetIndex)
{
	INT nCount = m_TabList.GetCount();
	TAB_ITEMS_STRU *tabitem_stru;
	POSITION pos;
	for ( int i=0 ; i<nCount ; i++ )
	{
		tabitem_stru = NULL;
		pos = m_TabList.FindIndex(i);
		if ( pos == NULL ) continue;
		
		tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
		
		if ( tabitem_stru->nItemType != _INET_FORM ) continue;
		
		if ( tabitem_stru->nInternetIndex == nInternetIndex ) 
		{
			return i;
		}
	}
	
	return -1;
}

VOID CKOLAS3_MAINView::KOLAS_LOCK()
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	if ( m_bLock == TRUE ) 
	{	
		// Flash 잠금
		// 18.11.07 JMJ KOL.RED.2018.020
		//if ( m_nTabIndex == 0 ) m_ctrlMainFlash.ShowWindow(SW_SHOW);
		if ( m_nTabIndex == 0 ) m_MainExplorer->ShowWindow(SW_SHOW);
		
		pFrame->LockWindowUpdate();
		// 1. 메인 프레임 잠금 해제
		m_bLock = FALSE;
		// 2. 탭 컨트롤 잠금 해제
		m_ctlFormTab.ShowWindow(SW_SHOW);		
		// 3. 컨트롤바 잠금 해제
		pFrame->ShowControlBar(&pFrame->m_wndDialogBar, TRUE, FALSE);
		// 4. 인터넷 네비게이터 잠금 해제
		pFrame->m_Navigator.m_bLock = FALSE;
		// 5. 메뉴바 잠금 해제	
		pFrame->ShowControlBar(&pFrame->m_wndDialogBar2, TRUE, FALSE);
		// 6. 상태바 잠금
		pFrame->ShowControlBar(pFrame->GetStatusBar(), TRUE, FALSE);
		// 7. Lock 폼 해제
		m_pLockForm->Show(FALSE);
		// 8. 잠금모드 해제
		ShowHideCurrentForm(TRUE);
		pFrame->UnlockWindowUpdate();	
// << 2015년 공공도서관 시스템 개선 2015/11/11 _LCM : 자동로그아웃기능
		if(m_pTimeOut)
			m_pTimeOut->ResumeThread();	
		ShowHideModeless(m_nTabIndex, SW_SHOW);
// >> 2015/11/11 --ADD

	}
	else
	{	
		pFrame->LockWindowUpdate();
		// 1. 실행중인 폼 잠금
		ShowHideCurrentForm(FALSE);
		// 2. 탭 컨트롤 잠금 
		m_ctlFormTab.ShowWindow(SW_HIDE);
		// 3. 컨트롤 바 잠금
		pFrame->ShowControlBar(&pFrame->m_wndDialogBar, FALSE, FALSE);
		// 4. 네비게이터 잠금
		pFrame->m_Navigator.m_bLock = TRUE;		
		// 5. 메뉴바 잠금
		pFrame->ShowControlBar(&pFrame->m_wndDialogBar2, FALSE, FALSE);
		// 6. 상태바 잠금
		pFrame->ShowControlBar(pFrame->GetStatusBar(), FALSE, FALSE);		
		
		// 7. 잠금모드 설정
		if ( m_pLockForm == NULL )
		{
			m_pLockForm = new CForm;			
			if (!m_pLockForm->CreateForm(this, _T("..\\dll\\common\\Locking.dll"), m_pInfo, 'M', _T("LOCK"))) return ;
			m_pLockForm->Show(FALSE);
		}		
		CRect rect;
		GetClientRect(rect);		
		m_pLockForm->ResizeForm(rect);
		m_pLockForm->Show(TRUE);
		// 메인 프레임 잠금 설정
		m_bLock = TRUE;
		pFrame->UnlockWindowUpdate();
		// Flash 잠금
		
		// 18.11.07 JMJ KOL.RED.2018.020
		//if ( m_nTabIndex == 0 ) m_ctrlMainFlash.MoveWindow(0,0,0,0);
		if ( m_nTabIndex == 0 ) m_MainExplorer->ShowWindow(SW_HIDE);
// << 2015년 공공도서관 시스템 개선 2015/11/11 _LCM : 자동로그아웃기능
		ShowHideModeless(m_nTabIndex, SW_HIDE);
// >> 2015/11/11 --ADD
	}	
}

VOID CKOLAS3_MAINView::ShowHideCurrentForm(BOOL bShow)
{
	TAB_ITEMS_STRU *tabitem_stru;
	CToolBar *pToolBar;
	POSITION posToolBarIndex;
	POSITION pos;
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	
	tabitem_stru = NULL;
	pos = m_TabList.FindIndex(m_nTabIndex);
	if ( pos == NULL ) return;
	tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
	
	switch ( tabitem_stru->nItemType )
	{
	case _MAIN_FORM :
		// 18.11.07 JMJ KOL.RED.2018.020
		/*
		if ( bShow ) m_ctrlMainFlash.ShowWindow(SW_SHOW);	
		else m_ctrlMainFlash.ShowWindow(SW_HIDE);			
		*/
		if ( bShow ) 
		{					
			m_MainExplorer->ShowWindow(SW_SHOW);
			pFrame->m_Navigator.SetURL(m_MainExplorer->GetURL() );			
		}
		else m_MainExplorer->ShowWindow(SW_HIDE);
		break;
	case _MENU_FORM :
		pToolBar = NULL;
		posToolBarIndex = NULL;
		
		if ( tabitem_stru->nToolBarID != -1 ) posToolBarIndex = m_listToolBar.FindIndex(tabitem_stru->nFormIndex);
		
		if ( posToolBarIndex != NULL ) pToolBar = (CToolBar*)m_listToolBar.GetAt(posToolBarIndex);
		if ( bShow ) 
		{
			m_pForm[tabitem_stru->nFormIndex]->Show(TRUE);					
			if ( pToolBar != NULL ) pToolBar->ShowWindow(SW_SHOW);													
		}					
		else 
		{
			m_pForm[tabitem_stru->nFormIndex]->Show(FALSE);
			if ( pToolBar != NULL ) pToolBar->ShowWindow(SW_HIDE);	
		}
		
#ifdef _KOLASIII
		pFrame->m_Navigator.SetURL(_T(""));
#endif
		break;
	case _INET_FORM :
#ifdef _KOLASIII
		if ( bShow ) 
		{					
			m_Explorer[tabitem_stru->nInternetIndex]->ShowWindow(SW_SHOW);
			pFrame->m_Navigator.SetURL(m_Explorer[tabitem_stru->nInternetIndex]->GetURL() );			
		}
		else m_Explorer[tabitem_stru->nInternetIndex]->ShowWindow(SW_HIDE);
#endif
		break;
	default:
		break;
	}
}

// 2008.07.02 ADD BY LKS : mainframe에서 각 탭의 WorkingTime과 검색된 Row갯수를 기록하기 위해 호출
VOID CKOLAS3_MAINView::UpdateTabInfo(CString strRow , CString strTime)
{
	TAB_ITEMS_STRU *tabitem_stru;
	POSITION pos;
	
	int currentTabIndex = m_ctlFormTab.GetCurSel();
	//if ( m_TabList.GetCount() <= 0 ) return;
	if ( currentTabIndex == 0 ) return;
	
	INT nCount = m_TabList.GetCount();
	
	if ( m_nTabIndex <= 0 ) return;
	pos = m_TabList.FindIndex(m_nTabIndex);
	
	tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
	
	tabitem_stru->SearchedRow  = strRow;
	tabitem_stru->WorkingTime = strTime;	
}

INT	CKOLAS3_MAINView::KalendarAutoSearch()
{
	// KALENDAR와의 파일매핑
	HANDLE phFileMap = OpenFileMapping(
		FILE_MAP_READ,
		FALSE,
		(LPCTSTR)_T("KALENDAR_SELECT_DIVISION_NO")
		);
	if(INVALID_HANDLE_VALUE == phFileMap) return NULL;
	
	// 오픈한 맵의 내용을 읽어온다.
	LPVOID lpMapView = MapViewOfFile(
		phFileMap,
		FILE_MAP_READ,
		0,
		0,
		0
		);
	if(NULL == lpMapView)	return NULL;
	
	// 읽어온 내용을 기록
	CString strData = (TCHAR*)lpMapView;
	
	
	// 데이터 절단
	// 들어오는 데이터의 형식은
	// 수입년도_차수상태:작업코드/차수번호
	// ex>>
	// 2008_구입:1/127
	CString strTmp = _T("");
	
	CString strAcqYear = _T("");	// 수입년도
	CString strDivStatus = _T("");	// 차수상태
	CString strWorkCode = _T("");	// 차수코드
	CString strDivNo = _T("");	// 차수번호	
	
	AfxExtractSubString(strTmp, strData, 0, ':');
	
	AfxExtractSubString(strAcqYear, strTmp, 0, '_');
	AfxExtractSubString(strDivStatus, strTmp, 1, '_');
	
	
	AfxExtractSubString(strTmp, strData, 1, ':');
	AfxExtractSubString(strWorkCode, strTmp, 0, '/');
	AfxExtractSubString(strDivNo, strTmp, 1, '/');
	
	// 수입년도
	// strACQYear;
	// 차수상태(구입, 검수, 미납, 등록)
	// strDivStatus;
	// 차수코드(동양서, 서양서..)
	// strWorkCode;
	// 차수번호
	// strDivNo;
	
	strTmp.Format(_T("수입년도 = %s 차수상태 = %s 차수코드 = %s 차수번호 = %s\r\n"),
		strAcqYear,
		strDivStatus,
		strWorkCode,
		strDivNo
		);
	TRACE(strTmp);
	
	UnmapViewOfFile(lpMapView);
	CloseHandle(phFileMap);
	
	HWND hWnd = ::FindWindow(NULL, _T("Kalendar"));
	if(NULL != hWnd)
	{
		CWnd* pWnd = new CWnd;
		pWnd->Attach(hWnd);
		pWnd->PostMessage(1113, 0, 0);
		pWnd->Detach();
		delete pWnd;
		TRACE(_T("Receive %s. Please Next Data\r\n"), strData);
	}
	
	// 관련 화면 찾기 & 띄우기
	if ( strDivStatus == _T("구입") )
	{
		NewForm(_T("1210"));
	}
	else if ( strDivStatus == _T("검수") || strDivStatus == _T("미납") )
	{
		NewForm(_T("1220"));
	}
	else if ( strDivStatus == _T("등록") )
	{
		NewForm(_T("1410"));
	}
	else
	{
		return 0;
	}
	
	POSITION pos;
	TAB_ITEMS_STRU *tabitem_stru;
	pos = m_TabList.FindIndex(m_nTabIndex);
	if ( pos != NULL ) 
	{
		tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
		m_pForm[tabitem_stru->nFormIndex]->KalendarSearch(strDivStatus, _T("1"), strAcqYear,strWorkCode,strDivNo);
	}
	
	return 0;
}

BOOL CKOLAS3_MAINView::OnNcActivate(BOOL bActive) 
{
	if(TRUE == bActive)
	{
		FileSaveHwnd();
		CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame == NULL) return FALSE;
	}

	return CFormView::OnNcActivate(bActive);
}

/*
@Create
 - 2010.03.26 BY PWR
@Description
 - m_listModelessDlg에 추가
@Parameter
 - 
@Return Value
 - 
@Mini Spec
 1. 
*/
LRESULT CKOLAS3_MAINView::OnAddModelessDlgList(WPARAM wParam, LPARAM lParam)
{
	if((CESL_Mgr*)wParam != NULL)
	{
		TAB_ITEMS_STRU *tabitem_stru;	
		POSITION pos;
		tabitem_stru = NULL;
		pos = m_TabList.FindIndex(m_nTabIndex);
		if(pos == NULL) return 0;
		tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);

		CModelessDlg *newModelessDlg = new CModelessDlg;
		newModelessDlg->m_pESL_Mgr = (CESL_Mgr*)wParam;
		m_listModelessDlg[tabitem_stru->nFormIndex].AddTail((CObject*)newModelessDlg);
	}

	return 0;
}

VOID CKOLAS3_MAINView::OnGetESLMGR(WPARAM wParam,LPARAM lParam)
{
	TAB_ITEMS_STRU *tabitem_stru;
	POSITION pos;

	pos = m_TabList.FindIndex(m_nTabIndex);
	if ( pos == NULL ) return;
	tabitem_stru = (TAB_ITEMS_STRU*)m_TabList.GetAt(pos);
	
	if(tabitem_stru->pEslMgr == NULL)
	{	
		tabitem_stru->pEslMgr = (CESL_Mgr*)wParam;
	}
}

UINT CKOLAS3_MAINView::TestUnityLoanServiceThread(LPVOID pParam)
{
	CKOLAS3_MAINView* pDlg = (CKOLAS3_MAINView*)pParam;

	pDlg->m_bIsRunTestUnityLoanServiceThread = TRUE;
	INT ids = pDlg->OnTestLCTLoanService();
// 	if(0 == ids)
// 	{
// 		ids = pDlg->OnTestTCTLoanService();
// 	}
	pDlg->m_bIsRunTestUnityLoanServiceThread = FALSE;

	if(0 > ids)
	{
		pDlg->SetTimer(3, 10000, NULL);
	}
	else
	{
		pDlg->m_pInfo->SetConnectUnityLoanService(TRUE);
	}

	return ids;
}
LRESULT CKOLAS3_MAINView::OnChangeUnityLoanServiceStatus(WPARAM wParam, LPARAM lParam)
{	
	if(!m_pInfo->IsConnectUnityLoanService())
	{
		if(!m_bIsRunTestUnityLoanServiceThread)
		{
			SetTimer(3, 2000, NULL);
		}
	}
	
	return 0;
}

INT CKOLAS3_MAINView::OnTestLCTLoanService()
{
	CFrameWriter Writer(m_pInfo);
		
	Writer.setCommand(_T("IL_K26_LOC_B01_SERVICE"));		
	Writer.addRecord();			
	Writer.addElement(_T("서비스명"), _T("LCT"));
	Writer.addElement(_T("쿼리"), _T("SELECT 'TEST' FROM DUAL"));
	Writer.addElement(_T("컬럼갯수"), _T("1"));
	Writer.addElement(_T("코드"), _T("0"));		

	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;

	Sender.SetTestMode(TRUE);
	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids)
	{
		return ids;
	}

	CString strTemp = Reader.getCommand();
	if(_T("ERROR")==strTemp)
	{
		return -1000;
	}
	else
	{
		strTemp = Reader.getElement(0, 0);
		if(_T("TEST") != strTemp)
		{
			return -1010;
		}
	}

	return 0;
}

INT CKOLAS3_MAINView::OnTestTCTLoanService()
{
	CFrameWriter Writer(m_pInfo);
		
	Writer.setCommand(_T("IL_K26_LOC_B01_SERVICE"));		
	Writer.addRecord();			
	Writer.addElement(_T("서비스명"), _T("TCT"));
	Writer.addElement(_T("쿼리"), _T("SELECT 'TEST' FROM DUAL"));
	Writer.addElement(_T("컬럼갯수"), _T("1"));
	Writer.addElement(_T("코드"), _T("0"));		

	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;

	Sender.SetTestMode(TRUE);
	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids)
	{
		return ids;
	}

	CString strTemp = Reader.getCommand();
	if(_T("ERROR")==strTemp)
	{
		return -1000;
	}
	else
	{
		strTemp = Reader.getElement(0, 0);
		if(_T("TEST") != strTemp)
		{
			return -1010;
		}
	}

	return 0;
}

// << 2015년 공공도서관 시스템 개선 2015/09/30 _LCM : KOLAS Lock
void CKOLAS3_MAINView::LockKOLAS3()
{	
	SendMessage(WM_USER+99, NULL, NULL);	
}
// >> 2015/09/30 --ADD

// 2015년 공공도서관 시스템 개선 2015/09/30 _LCM : TimeOut Count
UINT CKOLAS3_MAINView::TimeOutServiceThread(LPVOID pParam)
{
	CKOLAS3_MAINView* pDlg = (CKOLAS3_MAINView*)pParam;

	INT ids  = 0;
						
	while(true)
	{
		Sleep(1000);
		pDlg->m_nTimeOut++;

		//TRACE1("TimeOut : %d\n", pDlg->m_nTimeOut);	
		//if( pDlg->m_nTimeOut > 10 )
		if( pDlg->m_nTimeOut > (pDlg->m_nTimeOutMax * 60 ) )
		{
			pDlg->m_nTimeOut = 0;
			
			pDlg->LockKOLAS3();			

			pDlg->m_pTimeOut->SuspendThread();
		}
	}

	return ids;
}
// >> 2015/09/30 --ADD

// 2015년 공공도서관 시스템 개선 2015/09/30 _LCM : Key Hook
LRESULT CALLBACK GetMsgProcKey(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode >= 0)
	{
		int nKey = (int)wParam;
		
		if(nKey == WM_KEYDOWN)
		{			
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();			
			CKOLAS3_MAINView* pView = (CKOLAS3_MAINView*)pFrame->GetActiveView();
			pView->m_nTimeOut = 0;						
		}
	}

	return CallNextHookEx(g_hookKey, nCode, wParam, lParam);
}
// >> 2015/09/30 --ADD

// 2015년 공공도서관 시스템 개선 2015/09/30 _LCM : Mouse Hook
LRESULT CALLBACK GetMsgProcMouse(int nCode, WPARAM wParam, LPARAM lParam)
{	
	if(nCode >= 0)
	{
		switch(wParam)
		{
			case WM_RBUTTONUP:
			case WM_LBUTTONUP:
				CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();			
				CKOLAS3_MAINView* pView = (CKOLAS3_MAINView*)pFrame->GetActiveView();
				pView->m_nTimeOut = 0;			
				break;
		}	
	}
	return CallNextHookEx(g_hookMouse, nCode, wParam, lParam);
}
// >> 2015/09/30 --ADD

//2018.02.01 JOB.2018.0005 ADD BY KYJ : KOLAS 버전 업로드 기능 추가 
BOOL CKOLAS3_MAINView::SENDMYVERSION(CString strLIB_CODE , CString strMAC, CString strSWVER){
		
	if(strLIB_CODE.IsEmpty() || strMAC.IsEmpty() || strSWVER.IsEmpty()) {
		CString result_msg = _T("");
		result_msg.Format(_T("PARAMETER ERORR! LIB_CODE ='%s', MAC_ADDRESS = '%s', SWVER ='%s'"), strLIB_CODE, strMAC, strSWVER);
		writeLog(result_msg, _T("VERSION_UPLOAD"));
		return false;
	}

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	dm.SetDB(DBKIND);
	CString strTemp = _T("");
	CString strQuery = _T("");
	CString URL = _T("");
	
	strQuery.Format(_T("SELECT value_1 from esl_manage_tbl where class_alias='기술정보센터' and value_name = 'OPEN_API_URL' and manage_code = UDF_MANAGE_CODE"));
	dm.GetOneValueQuery(strQuery, strTemp);

	URL.Format(_T("%s?lib_code=%s&mac_address=%s&version=%s"),strTemp,strLIB_CODE,strMAC,strSWVER);

	strQuery.Format(_T("SELECT value_2 from esl_manage_tbl where class_alias='기술정보센터' and value_name = 'OPEN_API_URL' and manage_code = UDF_MANAGE_CODE"));
	dm.GetOneValueQuery(strQuery, strTemp);
	
	CString options[3];

	AfxExtractSubString(options[0],strTemp,0,',');
	AfxExtractSubString(options[1],strTemp,1,',');
	AfxExtractSubString(options[2],strTemp,2,',');

	CString strServer, strObject;
	DWORD dwSearviceType;
	INTERNET_PORT nPort;
	CInternetSession Session;
	CHttpConnection* pServer = NULL;
	CHttpFile *pFile = NULL;
	AfxParseURL(URL, dwSearviceType, strServer, strObject, nPort);
	try {
		// timeout : 5 sec
		Session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, _ttoi(options[0]));
		Session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF, _ttoi(options[1]));
		Session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, _ttoi(options[2]));
		pServer = Session.GetHttpConnection(strServer, nPort);
		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject);
		CString strHeader = _T("Content-Type: application/x-www-form-urlencoded\r\n");
		pFile->SendRequest(strHeader);
		writeLog(URL, _T("VERSION_UPLOAD"));
		
	} catch (CInternetException* e) {
		TCHAR szError[255];
		e->GetErrorMessage(szError, 255);
		e->Delete();
		pFile->Close();
		pServer->Close();
		Session.Close();
		delete pFile;
		delete pServer;
		
		writeLog(_T("ACCESS ERORR!"), _T("VERSION_UPLOAD"));
		
		return FALSE;
		
	} 
	
	pFile->Close();
	pServer->Close();
	Session.Close();
	delete pFile;
	delete pServer;
	
	return true;
}

//2018.02.01 JOB.2018.0005 ADD BY KYJ : KOLAS 버전 업로드 기능 추가 
VOID CKOLAS3_MAINView::writeLog(CString strlog, CString strmode) {
	if (strlog.IsEmpty()) return;
	SYSTEMTIME time;
	GetLocalTime(&time);
	CString filename, msg;
	filename.Format(_T("..\\tunning\\ESL_DM_%04d%02d%02d.txt"), time.wYear, time.wMonth, time.wDay);
	msg.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d.%03d][%s]\t :: %s"), time.wYear, time.wMonth, time.wDay,time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, strmode, strlog);
	CFileFind filefind;
	if (filefind.FindFile(_T("..\\cfg\\_log"))) {
		FILE* fp;
		fp = _tfopen(filename, _T("a+b"));
		if (fp == NULL) return;
		fseek(fp, 0, SEEK_END);
		if (0 == ftell(fp)) {
			fputc(0xFF, fp);
			fputc(0xFE, fp);
		}
		msg.TrimLeft(); msg.TrimRight();
		_ftprintf(fp, _T("%s\r\n"), msg);
		fclose(fp);
	}
}