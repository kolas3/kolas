// RPTUpdaterChildDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RPTUpdaterChild.h"
#include "RPTUpdaterChildDlg.h"

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
	virtual VOID DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
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

VOID CAboutDlg::DoDataExchange(CDataExchange* pDX)
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
// CRPTUpdaterChildDlg dialog

CRPTUpdaterChildDlg::CRPTUpdaterChildDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRPTUpdaterChildDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRPTUpdaterChildDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	m_strCommandLine = _T("");
	m_strSEQ_NO = _T("");
	m_strTRIAL_CNT = _T("");
	m_strLIB_PK = _T("");
	m_strSID = _T("");
	m_strLibName = _T("");
	m_nParentIdx = -1;
	m_strFIREWALL_YN = _T("N");
	m_bFirewall = FALSE;
	m_LogMsg = RPT_CONNECT_FAIL; //default
	m_arrayRptAlias.RemoveAll();


	
	//Thread
	m_pThreadMain = NULL;
	m_bIsThreadRunning = FALSE;
	m_hThread= ::CreateEvent(NULL, FALSE, FALSE, _T("ThreadEvent"));

	// KOL.UDF.022 시큐어코딩 보완
	m_nSucess = 0;
}

CRPTUpdaterChildDlg::~CRPTUpdaterChildDlg()
{
	// Thread Check
	if (m_bIsThreadRunning)
	{
		m_bIsThreadRunning = FALSE;

		MSG msg;
		while (TRUE)
		{
			if (::PeekMessage(&msg,GetSafeHwnd(), 1, 1, PM_NOREMOVE))
				AfxGetApp()->PumpMessage();

			if (WaitForSingleObject(m_hThread, 0) == WAIT_OBJECT_0)
			break;
		}
	}
}

VOID CRPTUpdaterChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRPTUpdaterChildDlg)
	DDX_Control(pDX, IDC_prgUPDATE, m_ctrlPrg);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRPTUpdaterChildDlg, CDialog)
	//{{AFX_MSG_MAP(CRPTUpdaterChildDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_MESSAGE(THREAD_END,OnThreadEnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRPTUpdaterChildDlg message handlers

BOOL CRPTUpdaterChildDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add _T("About...") menu item to _tsystem menu.

	// IDM_ABOUTBOX must be in the _tsystem command range.
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
	//  when the application's _tmain window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	

	if(ParseCommndLine()<0) {
		EndDialog(0);
	}
	
	
	if(ReadListFile()<0) {
		EndDialog(0);
	}
	
	
	//set window position
	CWnd* pWnd = NULL;
	pWnd = FindWindow(NULL,_T("RPTUpdater"));

	if(pWnd)
	{
		CRect rectParent;
		pWnd->GetWindowRect(rectParent);
	
		INT nBottom = rectParent.bottom-70;

		if(m_nParentIdx>0)
		{
			nBottom = rectParent.bottom-(70*(m_nParentIdx%MAX_CHILD+1));
		}
		
		this->MoveWindow(rectParent.right,nBottom,350,70);
		
	}

	if(m_nParentIdx>=MAX_CHILD)
	{
		m_nParentIdx = m_nParentIdx%MAX_CHILD;
	}

	
	SetTimer(0,1000,NULL);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CRPTUpdaterChildDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CRPTUpdaterChildDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		INT cxIcon = GetSystemMetrics(SM_CXICON);
		INT cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		INT x = (rect.Width() - cxIcon + 1) / 2;
		INT y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The _tsystem calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRPTUpdaterChildDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CRPTUpdaterChildDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(0 == nIDEvent)
	{
		KillTimer(0);
		

		//mainthread
		m_pThreadMain = AfxBeginThread(ThreadFunc_MainProc, this);
	}

	CDialog::OnTimer(nIDEvent);
}

INT CRPTUpdaterChildDlg::ParseCommndLine()
{
	
	//RPTUpdaterChild.exe [SeqNo/TrialCnt/LibraryPK/libraryName/ParentIdx/SID/FirewallYn]
	
	if(m_strCommandLine.IsEmpty()) return -1;
	
	m_strCommandLine.TrimLeft();
	m_strCommandLine.TrimRight();

	CString strOptionLine;
	INT nPos = m_strCommandLine.Find(_T("["));
	if(nPos<0) return -1;

	INT nLength = m_strCommandLine.GetLength();
	
	strOptionLine = m_strCommandLine.Mid(nPos+1,nLength-nPos-2);
	
	strOptionLine.TrimLeft();
	strOptionLine.TrimRight();

	if(strOptionLine.IsEmpty()) return -1;
	

	INT lastidx = strOptionLine.GetLength();
	INT startidx = 0;
	
	//차수정보 
	nPos = strOptionLine.Find(_T("/"));
	if(nPos<0) return -1;
	
	m_strSEQ_NO = strOptionLine.Left(nPos+1);
	m_strSEQ_NO.Remove('/');
	m_strSEQ_NO.TrimLeft();
	m_strSEQ_NO.TrimRight();
	startidx = nPos + 1;

	strOptionLine = strOptionLine.Right(lastidx - startidx);
	lastidx = strOptionLine.GetLength();
	
	//실행 횟수
	nPos = strOptionLine.Find(_T("/"));
	if(nPos<0) return -1;
	
	m_strTRIAL_CNT = strOptionLine.Left(nPos+1);
	m_strTRIAL_CNT.Remove('/');
	m_strTRIAL_CNT.TrimLeft();
	m_strTRIAL_CNT.TrimRight();
	startidx = nPos + 1;

	strOptionLine = strOptionLine.Right(lastidx - startidx);
	lastidx = strOptionLine.GetLength();


	//도서관 정보 PK 
	nPos = strOptionLine.Find(_T("/"));
	if(nPos<0) return -1;
	
	m_strLIB_PK = strOptionLine.Left(nPos+1);
	m_strLIB_PK.Remove('/');
	m_strLIB_PK.TrimLeft();
	m_strLIB_PK.TrimRight();
	startidx = nPos + 1;

	strOptionLine = strOptionLine.Right(lastidx - startidx);
	lastidx = strOptionLine.GetLength();


	//도서관 이름
	nPos = strOptionLine.Find(_T("/"));
	if(nPos<0) return -1;
	
	m_strLibName = strOptionLine.Left(nPos+1);
	m_strLibName.Remove('/');
	m_strLibName.TrimLeft();
	m_strLibName.TrimRight();
	startidx = nPos + 1;
	
	strOptionLine = strOptionLine.Right(lastidx - startidx);
	lastidx = strOptionLine.GetLength();
	
	
	//ParentIdx
	nPos = strOptionLine.Find(_T("/"));
	if(nPos<0) return -1;
	
	CString strTmp;
	strTmp = strOptionLine.Left(nPos+1);
	strTmp.Remove('/');
	strTmp.TrimLeft();
	strTmp.TrimRight();
	if(!strTmp.IsEmpty()) m_nParentIdx = _ttoi(strTmp.GetBuffer(0));

	startidx = nPos + 1;

	strOptionLine = strOptionLine.Right(lastidx - startidx);
	lastidx = strOptionLine.GetLength();


	//SID
	nPos = strOptionLine.Find(_T("/"));
	if(nPos<0) return -1;
	
	m_strSID = strOptionLine.Left(nPos+1);
	m_strSID.Remove('/');
	m_strSID.TrimLeft();
	m_strSID.TrimRight();
	startidx = nPos + 1;


	//FirewallYn
	m_strFIREWALL_YN = strOptionLine.Right(lastidx - startidx);
	m_strFIREWALL_YN.TrimLeft();
	m_strFIREWALL_YN.TrimRight();

	if(_T("Y") == m_strFIREWALL_YN) m_bFirewall = TRUE;
	
	if(m_strSEQ_NO.IsEmpty() || m_strLIB_PK.IsEmpty() || m_strLibName.IsEmpty() || m_strSID.IsEmpty() || m_strFIREWALL_YN.IsEmpty())
	{
		return -1;
	}

	SetWindowText(m_strLibName);

	return 0;
}


INT CRPTUpdaterChildDlg::ConnectTrial()
{
	
	CString strUserName,strOraclePassword;
	strUserName.Format(_T("kolas2@%s"), m_strSID);
	strOraclePassword = _T("kolas2#");

	INT nRequestID;
	nRequestID = GetSessionRequestID();
	GetDlgItem(IDC_staticNOTIFY)->SetWindowText(_T("connect to DB..."));
	if (ConnectDB(nRequestID, strUserName.GetBuffer(0), strOraclePassword.GetBuffer(0)) < 0)
	{
		return -1;
	}
	
	return 0;
}


INT CRPTUpdaterChildDlg::MainProcedure()
{
	
	if(ConnectTrial()<0) {
		m_LogMsg = RPT_CONNECT_FAIL;
		FileLog(_T("접속에 실패 하였습니다!"));	
		EndProcedure();
	}
			
	
	CString strConnectionInfo;
	strConnectionInfo.Format(_T("kolas2@%s/kolas2#[]"),m_strSID);

	m_DM.SetCONNECTION_INFO(strConnectionInfo);
	
	INT nSize = m_arrayRptAlias.GetSize();
	m_ctrlPrg.SetRange(0,nSize);
	m_ctrlPrg.SetPos(0);
	CString strNotify;

	CString strQuery;
	CString strAlias;
	CString strPK;
	INT ids = -1;
	
	InitFileLog();

//GetOneValueQuery
// -4004 : 접속되지않음.
// -4005 : 쿼리실행시 에러발생. 
// -4006 : 결과가 너무 많음.
// -4007 : 검색 결과 없음
// -4008 : 검색 필드가 결과 필드보다 많음
// -4999 : 아직 Oracle를 지원하지 않음

	for(INT i=0;i<nSize;i++)
	{
		//INSERT 인지 UPDATE인지 여부 판단(동시에 접속 여부 검사)
		
		strAlias = m_arrayRptAlias.GetAt(i);
		strNotify.Format(_T("%s 자료 처리중..."),strAlias);
		GetDlgItem(IDC_staticNOTIFY)->SetWindowText(strNotify);

		strQuery.Format(_T("SELECT PK FROM ESL_SIREPORT_TBL WHERE RPTALIAS='%s'"),strAlias);
		ids = m_DM.GetOneValueQuery(strQuery,strPK);
		
		if(-4004 == ids)
		{
			m_LogMsg = RPT_CONNECT_FAIL;
			FileLog(_T("접속에 실패 하였습니다!"));	
			EndProcedure();
		}
		
		if(ids<0)
		{
			
			if(-4004 == ids)
			{
				m_LogMsg = RPT_CONNECT_FAIL;
				FileLog(_T("접속에 실패 하였습니다!"));	
				EndProcedure();

			}
			else if(-4005 == ids || -4006 == ids)
			{
				m_LogMsg = PRT_PROCEDURE_FAIL;
				FileLog(_T("해당 자료의 SELECT에 실패 하였습니다!"));	
				EndProcedure();
			}
		}
	
		strPK.TrimLeft();
		strPK.TrimRight();
		
		if(strPK.IsEmpty()) //INSERT
		{
			if(InsertProcedure(strAlias)<0) 
			{
				m_LogMsg = PRT_PROCEDURE_FAIL;
				EndProcedure();
			}
			
		}
		else //UPDATE 
		{
			if(UpdateProcedure(strAlias,strPK)<0) 
			{
				m_LogMsg = PRT_PROCEDURE_FAIL;
				EndProcedure();
			}
		}
				
		m_ctrlPrg.SetPos(i+1);
	}

	m_LogMsg = PRT_SUCCESS;
	EndProcedure();

	return 0;
}


INT CRPTUpdaterChildDlg::InsertProcedure(CString strAlias)
{
	//해당 alias의 info file을 읽어 
	
	CString strPK;
	INT ids = -1;
	ids = m_DM.MakeRecKey(strPK, SEQ_NAME);

	if (ids<0) {
		FileLog(_T("REC_KEY 생성에 실패 하였습니다!"));	
		return -1; 
	}
	
	
/*	
0-FLAG
1-RPTGROUP
2-ENGINEER
3-DESCRIP
4-RPTSIZE
5-BITMAP_ALIAS
6-FIRST_DATE
7-LAST_DATE
*/

	CString strPath;
	CString msg;
	strPath.Format(_T("%s\\FileData\\%s.data"),CFG_DIRECTORY,strAlias);

	msg.Format(_T("------[%s] INSERT"),strAlias);
	FileLog(msg);

	CFile file;
	if (!file.Open(strPath, CFile::modeRead | CFile::typeBinary)) {
		msg.Format(_T("%s File을 읽을 수 없습니다"),strPath);
		FileLog(msg);
		return -1;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	INT rptsize = 0;
	rptsize = file.GetLength();
	BYTE * pBuffer = (BYTE *)malloc(rptsize + 1024);
	file.ReadHuge(pBuffer, rptsize);
	file.Close();
	
	CString strInfo[8];
	ids = GetInfoData(strAlias,strInfo);
	if (ids<0) {
		FileLog(_T("info file을 읽는데 실패 했습니다!"));	
		return -1; 
	}


	CString sCurrentTime;
	CTime t = CTime::GetCurrentTime();
	sCurrentTime.Format(_T("%04d%02d%02d%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

	CString sQuery;
	sQuery.Format(_T("INSERT INTO ESL_SIREPORT_TBL (PK, FLAG, RPTALIAS, RPTGROUP, ENGINEER, DESCRIP, RPTSIZE, FIRST_DATE, LAST_DATE) ")
				 _T("VALUES (%s, '%s', '%s', '%s', '%s', '%s', %d, '%s', '%s');"), 
				 strPK, strInfo[0], strAlias, strInfo[1] , strInfo[2], strInfo[3], rptsize, sCurrentTime, sCurrentTime);



	// XML LOB File Update
	ids = m_DM.ExecuteQuery(sQuery, 1);
	if (ids < 0) {
		msg = _T("[FAIL]") + sQuery;
		FileLog(msg);
		LogDB(strAlias,_T("I"),1);
		free (pBuffer);
		return -1;
	}

	ids = m_DM.LOBUpdate(_T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), pBuffer, rptsize);
	if (ids < 0) {
		msg = _T("[FAIL]Lobdata Update");
		FileLog(msg);	
		LogDB(strAlias,_T("I"),2);
		free (pBuffer);
		return -1;
	}

	free(pBuffer);

	msg = _T("[SUCCESS]") + sQuery;
	FileLog(msg);
	LogDB(strAlias,_T("I"),3);

	return 0;
}

INT CRPTUpdaterChildDlg::UpdateProcedure(CString strAlias,CString strPK)
{

/*	
0-FLAG
1-RPTGROUP
2-ENGINEER
3-DESCRIP
4-RPTSIZE
5-BITMAP_ALIAS
6-FIRST_DATE
7-LAST_DATE
*/
	
	INT ids = -1;

	CString strPath;
	CString msg;
	strPath.Format(_T("%s\\FileData\\%s.data"),CFG_DIRECTORY,strAlias);

	msg.Format(_T("------[%s] UPDATE"),strAlias);
	FileLog(msg);

	CFile file;
	if (!file.Open(strPath, CFile::modeRead | CFile::typeBinary)) {
		msg.Format(_T("%s File을 읽을 수 없습니다"),strPath);
		FileLog(msg);
		return -1;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	INT rptsize = 0;
	rptsize = file.GetLength();
	BYTE * pBuffer = (BYTE *)malloc(rptsize + 1024);
	file.ReadHuge(pBuffer, rptsize);
	file.Close();
	
	CString strInfo[8];
	ids = GetInfoData(strAlias,strInfo);
	if (ids<0) {
		FileLog(_T("info file을 읽는데 실패 했습니다!"));	
		return -1; 
	}

	CString sCurrentTime;
	CTime t = CTime::GetCurrentTime();
	sCurrentTime.Format(_T("%04d%02d%02d%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

	
	CString sQuery;
	sQuery.Format(_T("UPDATE ESL_SIREPORT_TBL SET FLAG='%s', RPTGROUP='%s', "
					 _T("ENGINEER ='%s', DESCRIP='%s', RPTSIZE = %d, BITMAP_ALIAS ='%s', LAST_DATE='%s' WHERE PK=%s;"))
				    ,strInfo[0], strInfo[1], strInfo[2], strInfo[3], rptsize,  strInfo[5], sCurrentTime, strPK);


	// XML LOB File Update
	ids = m_DM.ExecuteQuery(sQuery, 1);
	if (ids < 0) {
		msg = _T("[FAIL]") + sQuery;
		FileLog(msg);
		LogDB(strAlias,_T("U"),1);
		free (pBuffer);
		return -1;
	}

	ids = m_DM.LOBUpdate(_T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), pBuffer, rptsize);
	if (ids < 0) {
		msg = _T("[FAIL]Lobdata Update");
		FileLog(msg);
		LogDB(strAlias,_T("U"),2);
		free (pBuffer);
		return -1;
	}

	free(pBuffer);

	msg = _T("[SUCCESS]") + sQuery;
	FileLog(msg);
	LogDB(strAlias,_T("U"),3);

	return 0;
}


INT CRPTUpdaterChildDlg::GetInfoData(CString strAlias, CString strInfo[])
{

/*	
0-FLAG
1-RPTGROUP
2-ENGINEER
3-DESCRIP
4-RPTSIZE
5-BITMAP_ALIAS
6-FIRST_DATE
7-LAST_DATE
*/

	CString strLine;
	CString strPath;
	CStdioFile fileinfo;
	CString msg;
	CString strTitle,strData;
	INT nPos = -1;

	strPath.Format(_T("%s\\FileData\\%s.info"),CFG_DIRECTORY,strAlias);
	if (!fileinfo.Open(strPath, CFile::modeRead | CFile::typeBinary))
	{
		msg.Format(_T("%s File을 읽을 수 없습니다"),strPath);
		FileLog(msg);
		return -1;
	}
	TCHAR cUni;
	fileinfo.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fileinfo.SeekToBegin();
	}
	
	while(fileinfo.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();
		if(strLine.IsEmpty()) continue;

		nPos = strLine.Find(_T(":"));
		if(nPos<0) continue;
	
		strTitle = _T("");
		strData = _T("");

		strTitle = strLine.Left(nPos);
		strData = strLine.Mid(nPos+1);
		
		strTitle.TrimLeft();
		strTitle.TrimRight();
		strData.TrimLeft();
		strData.TrimRight();

		
		if(_T("FLAG") == strTitle)
		{
			strInfo[0] = strData;
		}
		else if(_T("RPTGROUP") == strTitle)
		{
			strInfo[1] = strData;
		}
		else if(_T("ENGINEER") == strTitle)
		{
			strInfo[2] = strData;
		}			
		else if(_T("DESCRIP") == strTitle)
		{
			strInfo[3] = strData;
		}		
		else if(_T("RPTSIZE") == strTitle)
		{
			strInfo[4] = strData;
		}
		else if(_T("BITMAP_ALIAS") == strTitle)
		{
			strInfo[5] = strData;
		}
		else if(_T("FIRST_DATE") == strTitle)
		{
			strInfo[6] = strData;
		}
		else if(_T("LAST_DATE") == strTitle)
		{
			strInfo[7] = strData;
		}
	}
	
	fileinfo.Close();

	return 0;
}


INT CRPTUpdaterChildDlg::EndProcedure()
{
	
	/*
	CString strNotify;

	if(m_LogMsg == RPT_CONNECT_FAIL)
	{
		strNotify.Format(_T("접속에 실패하였습니다!"));
	}
	else if(m_LogMsg == PRT_PROCEDURE_FAIL)
	{
		strNotify.Format(_T("작업중 문제가 발생하였습니다!"));
	}
	else if(m_LogMsg == PRT_SUCCESS)
	{
		strNotify.Format(_T("모든 작업을 성공적으로 수행하였습니다!"));
	}
	

	//개별 도서관 log를 남긴다
	*/

	EndFileLog();	

	//parent에 종료 정보를 보낸다 
	CWnd* pWnd = NULL;
	pWnd = FindWindow(NULL,_T("RPTUpdater"));

	if(pWnd)
	{
		::SendMessage(pWnd->GetSafeHwnd(),CHILD_PROCESS_END,m_nParentIdx,NULL);
	}

	EndDialog(0);

	return 0;
}

INT CRPTUpdaterChildDlg::ReadListFile()
{
	CString strLine;
	CStdioFile fileList;
	CString strListFile;

	strListFile.Format(_T("%s\\FileData\\list.tmp"),CFG_DIRECTORY);
	if (!fileList.Open(strListFile, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("List File을 읽을 수 없습니다"));
		return -1;
	}
	TCHAR cUni;
	fileList.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fileList.SeekToBegin();
	}
	
	while(fileList.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();
		if(strLine.IsEmpty()) continue;

		m_arrayRptAlias.Add(strLine);
	}
	
 
	CStdioFile fileLogCfg;
	CString strPath;

	strPath.Format(_T("%sLog.cfg"),CFG_DIRECTORY);
	

	if (!fileLogCfg.Open(strPath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("Log.cfg file을 찾을 수 없습니다!"));
		return -1;
	}
//	TCHAR cUni;
	fileLogCfg.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fileLogCfg.SeekToBegin();
	}
	
	CString strConnectInfo;

	while(fileLogCfg.ReadString(strLine))
	{
		strLine.TrimRight();
		strLine.TrimRight();
		if(strLine.IsEmpty()) continue;
	
		strConnectInfo = strLine;
	}
	
	if(strConnectInfo.IsEmpty()) return -1;
	m_DM_MYSQL.SetCONNECTION_INFO(strConnectInfo);


	return 0;
}


INT CRPTUpdaterChildDlg::LogDB(CString strRptAlias,
							   CString strQueryType,
							   INT nResult)
{

	CString strRecKey;
	m_DM_MYSQL.MySQL_MakeRecKey(strRecKey,_T("RPT_LOG_SEQ"));

	CString msg;
	strRecKey.TrimLeft();
	strRecKey.TrimRight();
	if(strRecKey.IsEmpty()) 
	{
		msg.Format(_T("log reckey를 생성할 수 없습니다!"));
		FileLog(msg);
		return 0;
	}
	

	CString sQuery;
	sQuery.Format(_T("INSERT INTO RPT_UPDATER_LOG_TBL ") 
				  _T("(PK,SEQ_NUM,TRIAL_CNT,LIB_KEY,RPT_ALIAS,QUERY_TYPE,DATE,TIME,RESULT) VALUES ")
				  _T("(%s,%s,%s,%s,'%s','%s',CURDATE(),CURTIME(),'%d')")
				  ,strRecKey,m_strSEQ_NO,m_strTRIAL_CNT,m_strLIB_PK,strRptAlias,strQueryType,nResult);
					
	INT ids = m_DM_MYSQL.ExecuteQuery(sQuery,1);


	if(ids<0) {
		msg.Format(_T("[log FAIL] %s"),sQuery);
		FileLog(msg);
	}
	else 
	{
		msg.Format(_T("[log SUCCESS] %s"),sQuery);
		FileLog(msg);

	}




	return 0;
}

INT CRPTUpdaterChildDlg::InitFileLog()
{
	CTime t = CTime::GetCurrentTime();
	CString msg;
	msg.Format(_T("[            START(%s차 - %s회)              ]"),m_strSEQ_NO,m_strTRIAL_CNT);
	FileLog(msg);

	return 0;
}

INT CRPTUpdaterChildDlg::EndFileLog()
{
	CTime t = CTime::GetCurrentTime();
	CString msg;
	msg.Format(_T("[            END(%s차 - %s회)                ]\n"),m_strSEQ_NO,m_strTRIAL_CNT);
	FileLog(msg);

	return 0;
}

VOID CRPTUpdaterChildDlg::FileLog(CString strLine)
{
	CStdioFile fileLog;

	CString strFilePath;
	strFilePath.Format(_T("%sLibraryLog\\%s.log"),CFG_DIRECTORY,m_strLibName);

	FILE * f = _tfopen(strFilePath, _T("a+b"));
	if (!f) return;
	

		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE 수정작업
		// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
		fseek(f, 0, SEEK_END);		
		if( 0 == ftell(f) )
		{
			/*UNCHANGE*/fputc(0xFF, f);
			/*UNCHANGE*/fputc(0xFE, f);
		}
		// ------------------------------------------------------------------------------
	
	CTime t = CTime::GetCurrentTime();
	CString msg;
	msg.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d] %s"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(),strLine);
	_ftprintf(f, _T("%s\r\n"), msg);

	fclose(f);

}

VOID CRPTUpdaterChildDlg::OnThreadEnd(WPARAM w,LPARAM l)
{
	EndProcedure();
}

UINT ThreadFunc_MainProc(LPVOID pParam)
{
	
	CRPTUpdaterChildDlg* pDlg = (CRPTUpdaterChildDlg*)pParam;
	// KOL.UDF.022 시큐어코딩 보완
//	if (pDlg == NULL)
	if (&pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}

	CString strQuery;
	CString strAlias;
	CString strPK;
	CString strNotify;
	INT ids = -1;
	INT i=0;
	CString strConnectionInfo;
	INT nSize;

	pDlg->InitFileLog();

	if(!pDlg->m_bFirewall)
	{
		if(pDlg->ConnectTrial()<0) {
			pDlg->m_LogMsg = RPT_CONNECT_FAIL;
			pDlg->FileLog(_T("접속에 실패 하였습니다!"));
			pDlg->LogDB(_T(""),_T("T"),0);
			
			goto END;
		}
		else
		{
			pDlg->LogDB(_T(""),_T("T"),3);
		}
	}
	
	
	strConnectionInfo.Format(_T("kolas2@%s/kolas2#[]"),pDlg->m_strSID);

	pDlg->m_DM.SetCONNECTION_INFO(strConnectionInfo);
	
	nSize = pDlg->m_arrayRptAlias.GetSize();
	pDlg->m_ctrlPrg.SetRange(0,nSize);
	pDlg->m_ctrlPrg.SetPos(0);




//GetOneValueQuery
// -4004 : 접속되지않음.
// -4005 : 쿼리실행시 에러발생. 
// -4006 : 결과가 너무 많음.
// -4007 : 검색 결과 없음
// -4008 : 검색 필드가 결과 필드보다 많음
// -4999 : 아직 Oracle를 지원하지 않음

	for(i=0;i<nSize;i++)
	{
		//INSERT 인지 UPDATE인지 여부 판단(동시에 접속 여부 검사)
		
		strAlias = pDlg->m_arrayRptAlias.GetAt(i);
		strNotify.Format(_T("%s 자료 처리중..."),strAlias);
		pDlg->GetDlgItem(IDC_staticNOTIFY)->SetWindowText(strNotify);
		
		if(pDlg->m_bFirewall)
		{
			pDlg->LogDB(strAlias,_T("F"),4);
			pDlg->FileLog(_T("방화벽으로 인해 로그로만 남깁니다!"));
			pDlg->m_ctrlPrg.SetPos(i+1);
			continue;
		}


		strQuery.Format(_T("SELECT PK FROM ESL_SIREPORT_TBL WHERE RPTALIAS='%s' AND FLAG != 'U'"),strAlias);
		ids = pDlg->m_DM.GetOneValueQuery(strQuery,strPK);
		
		
		if(ids<0)
		{
			
			if(-4004 == ids)
			{
				pDlg->m_LogMsg = RPT_CONNECT_FAIL;
				pDlg->FileLog(_T("접속에 실패 하였습니다!"));
				pDlg->LogDB(strAlias,_T("T"),0);
				
				goto END;

			}
			else if(-4005 == ids || -4006 == ids)
			{
				pDlg->m_LogMsg = PRT_PROCEDURE_FAIL;
				pDlg->FileLog(_T("해당 자료의 SELECT에 실패 하였습니다!"));	
				pDlg->LogDB(strAlias,_T("T"),1);
				
				goto END;
			}
		}
	
		strPK.TrimLeft();
		strPK.TrimRight();
		
		if(strPK.IsEmpty()) //INSERT
		{
			if(pDlg->InsertProcedure(strAlias)<0) 
			{
				goto END;
			}
			
		}
		else //UPDATE 
		{
			if(pDlg->UpdateProcedure(strAlias,strPK)<0) 
			{
				goto END;
	
			}
		}
				
		pDlg->m_ctrlPrg.SetPos(i+1);
	}
	

	goto END;
	

END:
	pDlg->m_bIsThreadRunning = FALSE;
	::SendMessage(pDlg->GetSafeHwnd(),THREAD_END,NULL,NULL);
	return 0;


}