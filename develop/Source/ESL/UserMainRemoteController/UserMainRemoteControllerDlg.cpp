// UserMainRemoteControllerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserMainRemoteController.h"
#include "UserMainRemoteControllerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserMainRemoteControllerDlg dialog

CUserMainRemoteControllerDlg::CUserMainRemoteControllerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserMainRemoteControllerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserMainRemoteControllerDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bShow = FALSE;
}

void CUserMainRemoteControllerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserMainRemoteControllerDlg)
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_EXITALL, m_btnExitAll);
	DDX_Control(pDX, IDC_BTN_QUITSELECT, m_btnQuitSelect);
	DDX_Control(pDX, IDC_BTN_QUITALL, m_btnQuitAll);
	DDX_Control(pDX, IDC_BTN_EXITSELECT, m_btnExitSelect);
	DDX_Control(pDX, IDC_LIST_PC_LIST, m_lstClientList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUserMainRemoteControllerDlg, CDialog)
	//{{AFX_MSG_MAP(CUserMainRemoteControllerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDR_MENU_EXIT, OnMenuExit)
	ON_WM_WINDOWPOSCHANGING()
	ON_BN_CLICKED(IDC_BTN_EXITALL, OnBtnExitAll)
	ON_BN_CLICKED(IDC_BTN_QUITALL, OnBtnQuitall)
	ON_BN_CLICKED(IDC_BTN_QUITSELECT, OnBtnQuitselect)
	ON_BN_CLICKED(IDC_BTN_EXITSELECT, OnBtnExitselect)
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	ON_MESSAGE(TRAYICON_MESSAGE, OnTrayMessage)
	ON_MESSAGE(WM_ACCEPT_CLIENT, OnAcceptClient)
	ON_MESSAGE(WM_RECEIVE_MSG, OnReceiveMsg)
	ON_MESSAGE(WM_CLOSE_SERVER, OnCloseServer)
	ON_MESSAGE(WM_TYPE_INFO, OnTypeInfo)
	ON_MESSAGE(WM_PRINT_CLIENT_LIST, PrintClientList)
	ON_MESSAGE(WM_DELETE_CLOSE_CLIENT_LIST, DeleteCloseClientList)
	ON_WM_TIMER()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserMainRemoteControllerDlg message handlers

BOOL CUserMainRemoteControllerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
//DEL 	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	SetWindowText("User Search System Manager");

	if(TRUE == CheckRun())
	{
		PostMessage(WM_QUIT);
	}
	else
	{
		m_hTrayIcon.Create(
			this,
			TRAYICON_MESSAGE,
			_T("User Search System Manager"),
			m_hIcon,
			IDR_MAINFRAME
			);
		
		m_ServerMgr.SetWnd(GetSafeHwnd());
		INT nResult = m_ServerMgr.CreateServer();
		if( 0	!=	nResult )
		{
			AfxMessageBox("Server Create Failed.\r\nError Code : %d",nResult,MB_ICONSTOP);
			return	FALSE;
		}
	}
	
	InitButtonImage();
	InitButtonPosition();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CUserMainRemoteControllerDlg::InitButtonImage()
{
	m_btnQuitSelect.LoadBitmaps(IDB_BITMAP_QUIT_SELECT_OFF,IDB_BITMAP_QUIT_SELECT_ON
								,IDB_BITMAP_QUIT_SELECT_OFF,IDB_BITMAP_QUIT_SELECT_ON);
	m_btnQuitSelect.SizeToContent();

	m_btnQuitAll.LoadBitmaps(IDB_BITMAP_QUIT_ALL_OFF,IDB_BITMAP_QUIT_ALL_ON
							,IDB_BITMAP_QUIT_ALL_OFF, IDB_BITMAP_QUIT_ALL_ON);
	m_btnQuitAll.SizeToContent();

	m_btnExitSelect.LoadBitmaps(IDB_BITMAP_EXIT_SELECT_OFF,IDB_BITMAP_EXIT_SELECT_ON
								,IDB_BITMAP_EXIT_SELECT_OFF,IDB_BITMAP_EXIT_SELECT_ON);
	m_btnExitSelect.SizeToContent();

	m_btnExitAll.LoadBitmaps(IDB_BITMAP_EXIT_ALL_OFF,IDB_BITMAP_EXIT_ALL_ON
							,IDB_BITMAP_EXIT_ALL_OFF, IDB_BITMAP_EXIT_ALL_ON);
	m_btnExitAll.SizeToContent();

	m_btnCancel.LoadBitmaps(IDB_BITMAP_ADMIN_BUTTON_X);
	m_btnCancel.SizeToContent();
}

VOID CUserMainRemoteControllerDlg::InitButtonPosition()
{
	CButton		*pBtnExitAll;
	CButton		*pBtnExitSelect;
	CButton		*pBtnQuitAll;
	CButton		*pBtnQuitSelect;
	CRect		rect1;
	CRect		rect2;
	CRect		rect3;
	CRect		rect4;

	pBtnExitAll =		(( CButton* ) GetDlgItem (IDC_BTN_EXITALL	) );
	pBtnExitSelect =	(( CButton* ) GetDlgItem (IDC_BTN_EXITSELECT	) );
	pBtnQuitAll =		(( CButton* ) GetDlgItem (IDC_BTN_QUITALL	) );
	pBtnQuitSelect =	(( CButton* ) GetDlgItem (IDC_BTN_QUITSELECT	) );

	pBtnExitAll->GetWindowRect(rect1);	
	ScreenToClient(rect1);	
	rect1.left -= 1; rect1.right -= 1; rect1.top += 2; rect1.bottom += 2;	
	pBtnExitAll->MoveWindow(rect1);

	pBtnExitSelect->GetWindowRect(rect2);	
	ScreenToClient(rect2);	
	rect2.left -= 1; rect2.right -= 1; rect2.top += 1; rect2.bottom += 1;	
	pBtnExitSelect->MoveWindow(rect2);

	pBtnQuitAll->GetWindowRect(rect3);	
	ScreenToClient(rect3);	
	rect3.left -= 1; rect3.right -= 1; rect3.top += 1; rect3.bottom += 1;	
	pBtnQuitAll->MoveWindow(rect3);

	pBtnQuitSelect->GetWindowRect(rect4);	
	ScreenToClient(rect4);	
	rect4.left -= 1; rect4.right -= 1; rect4.top += 1; rect4.bottom += 1;	
	pBtnQuitSelect->MoveWindow(rect4);
}

void CUserMainRemoteControllerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		// 캡션바에 X버튼 눌렀을 경우
		// 프로그램을 종료하지 않고 숨기기
		if(nID==SC_CLOSE)			
		{			
			ShowWindow(SW_HIDE);			
			return;
		}
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUserMainRemoteControllerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUserMainRemoteControllerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


//트레이아이콘 메시지를 받았을 때의 처리
void CUserMainRemoteControllerDlg::OnTrayMessage(WPARAM wParam, LPARAM lParam)
{
	if(WM_LBUTTONDBLCLK == lParam)
	{
		// 왼쪽버튼을 눌렀을 경우 윈도우 보이기/숨기기 토글
		BOOL bWindowShow = IsWindowVisible();
		if(TRUE == bWindowShow)
		{
			ShowWindow(SW_HIDE);
		}
		else
		{
			ShowWindow(SW_SHOW);
		}
	}
	else if(WM_RBUTTONDOWN == lParam)
	{
		// 오른쪽버튼 눌렀을 경우 팝업메뉴 띄우기
		POINT	point;
		CMenu  *sMenu,*pMenu;	

		pMenu = new CMenu;
		pMenu ->LoadMenu(IDR_MENU_TRAY);	
		sMenu = pMenu->GetSubMenu(0);	
		
		GetCursorPos(&point);
		sMenu->TrackPopupMenu(
			TPM_RIGHTBUTTON|TPM_LEFTALIGN,
			point.x,
			point.y,
			this, 
			CRect(point.x-100, point.y-100, point.x, point.y)
			);
		delete pMenu;
		
	}
}

void CUserMainRemoteControllerDlg::OnMenuExit() 
{
	// TODO: Add your command handler code here
	

	UINT uResult = AfxMessageBox(
		_T("검색대 프로그램과 연결이 끊어집니다.\r\n")
		_T("종료 하시겠습니까?\r\n")
		,MB_YESNO
		);
	if(IDYES == uResult) 
	{
		INT nResult;
		INT nClientCnt	=	m_ServerMgr.GetClientCnt();
		if( 1	>	nClientCnt )
		{
			nResult		=	m_ServerMgr.OnCloseServer();
			if( 1	!=	nResult )
			{
				AfxMessageBox("Server 종료 중 Error가 발생하였습니다.\r\nError Code : %d", nResult);
				return;
			}
		}
		else if( 1	<=	nClientCnt )
		{
			nResult		=	m_ServerMgr.SendMsg(TYPE_CLOSE_SERVER);
			if( 1	!=	nResult )
			{
				AfxMessageBox("Server 종료 Message 전송 중 Error가 발생하였습니다.\r\nError Code : %d", nResult);
				return;
			}

			nResult		=	m_ServerMgr.OnCloseServer();
			if( 1	!=	nResult )
			{
				AfxMessageBox("Server 종료 중 Error가 발생하였습니다.\r\nError Code : %d", nResult);
				return;
			}
		}

		m_lstClientList.ResetContent();
		m_arrayClientData.RemoveAll();
		PostMessage(WM_QUIT, NULL, NULL);
	}
}

void CUserMainRemoteControllerDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);

	// TODO: Add your message handler code here

	// 맨 처음 다이얼로그 시작시 화면에 미표기하기 위해 처기
	if(FALSE == m_bShow)
	{
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
	}
	else
	{
		lpwndpos->flags |= SWP_SHOWWINDOW;
	}
}

BOOL CUserMainRemoteControllerDlg::ShowWindow(int nCmdShow)
{
	if(SW_HIDE != nCmdShow) m_bShow = TRUE;
	return CDialog::ShowWindow(nCmdShow);
}


BOOL CUserMainRemoteControllerDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{			
			ShowWindow(SW_HIDE);
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//**********************************************************//
//**********************************************************//
//**********************************************************//
void CUserMainRemoteControllerDlg::OnBtnExitAll() 
{
	// TODO: Add your control notification handler code here
	INT nListCnt	=	m_arrayClientData.GetSize();
	if( 1	<=	nListCnt )
	{
		INT nResult;

		nResult		=	m_ServerMgr.SendMsg(TYPE_EXIT);
		if( 1	!=	nResult )
		{
			AfxMessageBox("PC 종료 Message 전송에 실패하였습니다.\r\nError Code : %d" , nResult);
			return;
		}
		nResult		=	m_ServerMgr.CloseAllClient();
		if(	1	!=	nResult)
		{
			AfxMessageBox("Client 종료에 실패하였습니다.\r\nError Code : %d" , nResult);
			return;
		}
		m_lstClientList.ResetContent();
		m_arrayClientData.RemoveAll();
	}
	Currently_ConnectUserMain_Count();
}

void CUserMainRemoteControllerDlg::OnBtnQuitall() 
{
	// TODO: Add your control notification handler code here
	INT nListCnt	=	m_arrayClientData.GetSize();
	if( 1	<=	nListCnt )
	{
		INT nResult;

		nResult		=	m_ServerMgr.SendMsg(TYPE_QUIT);
		if( 1	!=	nResult )
		{
			AfxMessageBox("PC 종료 Message 전송에 실패하였습니다.\r\nError Code : %d" , nResult);
			return;
		}

		nResult		=	m_ServerMgr.CloseAllClient();
		if(	1	!=	nResult)
		{
			AfxMessageBox("Client 종료에 실패하였습니다.\r\nError Code : %d" , nResult);
			return;
		}
		m_lstClientList.ResetContent();
		m_arrayClientData.RemoveAll();
	}
	Currently_ConnectUserMain_Count();
}

void CUserMainRemoteControllerDlg::OnBtnExitselect() 
{
	// TODO: Add your control notification handler code here
	ExitSelect();
}

void CUserMainRemoteControllerDlg::OnBtnQuitselect() 
{
	// TODO: Add your control notification handler code here
	QuitSelect();
}

///////////////////////////////////////////////
//List에서 선택한 Client 관리

//PC 종료
void CUserMainRemoteControllerDlg::ExitSelect()
{
	INT nResult;

	INT nListCnt	=	m_arrayClientData.GetSize();
	if( 1	>	nListCnt )
	{
		return;
	}
	for(INT nListIndex=nListCnt-1 ; nListIndex>=0 ; nListIndex--)
	{
		if( 0	!=	m_lstClientList.GetSel(nListIndex) )
		{
			CString		strInfo	=	m_arrayClientData.GetAt(nListIndex);
			CString		strIp	=	"";
			AfxExtractSubString( strIp , strInfo , 1 , ':' );

			nResult		=	m_ServerMgr.SendToSelectClient(TYPE_EXIT,strIp); 
			if( 1		!=	nResult )
			{
				AfxMessageBox("Message 전송에 실패했습니다.\r\nError Code : %d" , nResult);
				return;
			}
			if( FALSE	==	m_ServerMgr.CloseSelectClient(nListIndex) )
			{
				AfxMessageBox("선택한 PC에 대한 정보가 없습니다.");
				return;
			}
			m_arrayClientData.RemoveAt(nListIndex);
		}
	}
	Changed_Client_List();
	Currently_ConnectUserMain_Count();
}

//Program 종료
void CUserMainRemoteControllerDlg::QuitSelect()
{	
	INT nResult;

	INT nListCnt	=	 m_arrayClientData.GetSize();
	if( 1	>	nListCnt )
	{
		return;
	}
	for(INT nListIndex=nListCnt-1 ; nListIndex>=0 ; nListIndex--)
	{
		if( 0	!=	m_lstClientList.GetSel(nListIndex) )
		{
			
			CString		strInfo =	m_arrayClientData.GetAt(nListIndex);
			CString		strIp	=	"";
			AfxExtractSubString( strIp , strInfo , 1 , ':' );
			
			nResult		=	m_ServerMgr.SendToSelectClient(TYPE_QUIT,strIp);
			if(	1		!=		nResult )
			{
				AfxMessageBox("Message 전송에 실패하였습니다.\r\nError Code : %d" , nResult );
				return;
			}
			if( FALSE	==	m_ServerMgr.CloseSelectClient(nListIndex) )
			{
				AfxMessageBox("선택한 PC에 대한 정보가 없습니다.");
				return;
			}
			m_arrayClientData.RemoveAt(nListIndex);
		}
	}
	Changed_Client_List();
	Currently_ConnectUserMain_Count();
}

void CUserMainRemoteControllerDlg::OnBtnCancel() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
}
//**********************************************************//
//**********************************************************//
//**********************************************************//


/////////////////////////////////////////////////////////////////////////////
//************접속한 Client의 갯수를 화면에 출력 하는 함수*****************//
void CUserMainRemoteControllerDlg::Currently_ConnectUserMain_Count()
{
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_MSG);
	if(NULL != pStatic) 
	{
		CString strText = _T("");
		strText.Format(
			_T("연결 PC : %d"),
			m_ServerMgr.GetClientCnt()
			);
		CRect rStatic= CRect(0, 0, 0, 0);
		pStatic ->GetWindowRect(rStatic);
		ScreenToClient(rStatic);
		InvalidateRect(rStatic);
		pStatic->SetWindowText(strText);
	}
}

/*
void CUserMainRemoteControllerDlg::Changed_ConnectUserMain_Count(INT nNewConnect_UserMain_Count)
{
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_MSG);
	if(NULL != pStatic) 
	{
		CString strText = _T("");
		strText.Format(
			_T("연결 PC : %d"),
			nNewConnect_UserMain_Count
			);
		CRect rStatic= CRect(0, 0, 0, 0);
		pStatic ->GetWindowRect(rStatic);
		ScreenToClient(rStatic);
		InvalidateRect(rStatic);
		pStatic->SetWindowText(strText);
	}
}
*/



/////////////////////////////////////////////////////////////////////////////
//************접속 요청을 Server Manage에 넘겨 주는 함수*******************//
LONG CUserMainRemoteControllerDlg::OnAcceptClient(UINT wParam, LONG lParam)
{
	INT nResult = m_ServerMgr.OnAcceptClient(wParam, lParam);
	if( 1	==	nResult )
	{		
		Currently_ConnectUserMain_Count();
		return	nResult;
	}
	else
	{
		AfxMessageBox("Accept 처리 중 Error가 발생하였습니다.\r\nError Code : %d" , nResult);
		Currently_ConnectUserMain_Count();
		return	nResult;
	}
}


/////////////////////////////////////////////////////////////////////////////
//*************받은 메시지를 Server Manage에 넘겨 주는 함수****************//
LONG CUserMainRemoteControllerDlg::OnReceiveMsg(UINT wParam, LONG lParam)
{
	INT	nResult	= m_ServerMgr.OnReceiveMsg(wParam, lParam);
	if( 1	==	nResult )
	{
		Currently_ConnectUserMain_Count();
		return	nResult;
	}
	else
	{
		AfxMessageBox("Receive 처리 중 Error가 발생하였습니다.\r\nError Code : %d" , nResult);
		Currently_ConnectUserMain_Count();
		return	nResult;
	}
}

/////////////////////////////////////////////////////////////////////////////
//******************받은 메시지의 타입을 알아내는 함수*********************//
INT CUserMainRemoteControllerDlg::OnTypeInfo(UINT wParam, LONG lParam)
{
	if( TYPE_CONNECT == lParam )
	{
		Currently_ConnectUserMain_Count();
		return TRUE;
	}
	else if( TYPE_CONNECT_CHECK == lParam )
	{
		Currently_ConnectUserMain_Count();
		return TRUE;
	}
	else if( TYPE_CLOSE_CLIENT == lParam )
	{
		Currently_ConnectUserMain_Count();
		return TRUE;
	}
	else if( TYPE_CLOSE_SERVER == lParam )
	{
		::SendMessage( GetSafeHwnd() , WM_CLOSE_SERVER , 0 , 0);
		Currently_ConnectUserMain_Count();
		return TRUE;
	}
	return FALSE;
}

//Server 종료 메시지를 받고, Server Manage에 있는 함수 호출 하여 종료 처리
LONG CUserMainRemoteControllerDlg::OnCloseServer(UINT wParam, LONG lParam)
{
	if( 1 == m_ServerMgr.OnCloseServer() )
	{
		m_lstClientList.ResetContent();
		m_arrayClientData.RemoveAll();
		PostMessage(WM_QUIT, NULL, NULL);
		return TRUE;
	}
	else
	{
		AfxMessageBox(GetLastError());
		return FALSE;
	}
}

#include <tlhelp32.h>
BOOL CUserMainRemoteControllerDlg::CheckRun()
{
	
	HANDLE         hProcessSnap = NULL; 
	BOOL           bRet      = FALSE; 
	PROCESSENTRY32 pe32      = {0}; 
	BOOL bFind = FALSE;
	
	//  Take a snapshot of all processes in the system. 
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
	
	if (hProcessSnap	!=	INVALID_HANDLE_VALUE) 
	{
		
		//  Fill in the size of the structure before using it. 
		pe32.dwSize = sizeof(PROCESSENTRY32); 
		
		//  Walk the snapshot of the processes, and for each process, 
		//  display information.     
		
		INT		nCount;
		nCount	=	0;
		if(Process32First(hProcessSnap, &pe32)) 
		{ 
			do 
			{ 					
				if(_tcsicmp(pe32.szExeFile, _T("UserMainRemoteController.exe"))==0)
				{
					nCount++;
					if ( nCount > 1 )
					{
						bFind = TRUE;
						HANDLE hProcess = NULL;
						break;
					}
				} 				
			} 
			while (Process32Next(hProcessSnap, &pe32)); 
		}
		
		// Do not forget to clean up the snapshot object. 
		CloseHandle (hProcessSnap); 
	}
	
	return bFind;
}

//////////////////////////////////////////////////////////////////////////
//출력 관련 함수
void CUserMainRemoteControllerDlg::PrintClientList(UINT wParam, LONG lParam)
{
	if( NULL == lParam )
	{
		AfxMessageBox(_T("Client의 정보가 없습니다.\r\n"));
		return;
	}

	LPCTSTR		strInfo;
	strInfo	 =	(LPCTSTR)lParam;

	CString	strName	= "";
	AfxExtractSubString( strName , strInfo , 0 , ':' );

	m_arrayClientData.Add(strInfo);
	m_lstClientList.AddString(strName);
}

//Client 종료시 Client List에서 삭제
void CUserMainRemoteControllerDlg::DeleteCloseClientList(WPARAM wParam, LPARAM lParam)
{
	if( NULL == lParam )
	{
		AfxMessageBox(_T("Client의 정보가 없습니다.\r\n"));
		return;
	}

	LPCTSTR		strIp	=	(LPCTSTR)lParam;

	INT		nListCnt	=	m_arrayClientData.GetSize();
	for( INT nListIndex=nListCnt-1 ; nListIndex>=0 ; nListIndex--)
	{
		CString		strListInfo	=	m_arrayClientData.GetAt(nListIndex);
		CString		strListIp	=	"";
		AfxExtractSubString( strListIp , strListInfo , 1 , ':' );

		if( strIp == strListIp )
		{	
			m_arrayClientData.RemoveAt(nListIndex);
			Changed_Client_List();
			Currently_ConnectUserMain_Count();
			return;
		}
	}
}


void CUserMainRemoteControllerDlg::Changed_Client_List()
{
	m_lstClientList.ResetContent();

	INT		nListCnt		=	m_arrayClientData.GetSize();
	for(INT nListIndex=0 ; nListIndex<nListCnt ; nListIndex++)
	{
		CString		strInfo =	m_arrayClientData.GetAt(nListIndex);

		CString		strName	=	"";
		AfxExtractSubString( strName , strInfo , 0 , ':' );

		m_lstClientList.AddString(strName);
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//Static 설정 하는 함수(글자색, 배경 투명)
HBRUSH CUserMainRemoteControllerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if( nCtlColor == CTLCOLOR_STATIC )
	{
		pDC->SetTextColor(RGB(106,90,205));
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject( NULL_BRUSH );
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

//////////////////////////////////////////////////////////////////////////
//프로그램을 마우스로 움직일 수 있게 하는 함수
void CUserMainRemoteControllerDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( nFlags & MK_LBUTTON ) // L 마우스 버튼이 눌러져 있구나..
	{
		CRect	rDlg;
		GetClientRect(rDlg);
		CRect	cap		=	CRect(0, 0, rDlg.right, 38);
		
		if(cap.PtInRect(point))
		{
			this->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
			return;
		}		
	}
	CDialog::OnMouseMove(nFlags, point);
}
/*
void CUserMainRemoteControllerDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CRect rDlg;
	GetClientRect(rDlg);
	CRect	cap = CRect(0, 0, rDlg.right, 38);
	
	if(cap.PtInRect(point))
	{
		this->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
		return;
	}
	CDialog::OnLButtonDown(nFlags, point);
}
*/
/*
void CUserMainRemoteControllerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(	TIMER_CONNECT_CHECK	==	nIDEvent )
	{
		INT nResult		=	m_ServerMgr.SendMsg( TYPE_CONNECT_CHECK );
	}
	
	CDialog::OnTimer(nIDEvent);
}
*/

void CUserMainRemoteControllerDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	// TODO: Add your message handler code here
	CString str;
	str.Format(_T("X : %d , Y : %d \r\n"),x,y);
	TRACE(str);
	
}
