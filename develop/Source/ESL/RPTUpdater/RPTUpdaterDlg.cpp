// RPTUpdaterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RPTUpdater.h"
#include "RPTUpdaterDlg.h"
#include "RptUpdateResultViewer.h"
#include "RptUpdateLogViewer.h"

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
// CRPTUpdaterDlg dialog

CRPTUpdaterDlg::CRPTUpdaterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRPTUpdaterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRPTUpdaterDlg)
	m_strSearchData = _T("");
	m_strSeqNo = _T("실행시부여");
	m_strTrialCnt = _T("");
	m_strSearchLibName = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pGridLibList = NULL;
	m_pGridRptList = NULL;

	m_nCurrentTnsIdx = 0;
	m_nChildCnt = 0;

	m_bNewSeqNo = TRUE;

	// KOL.UDF.022 시큐어코딩 보완
	INT i = 0;
	for(i = 0 ; i < MAX_CHILD ; i++){	
		m_nTnsIdx[i] = 0;
		m_bUsePosition[i] = FALSE;
	}
	
}

CRPTUpdaterDlg::~CRPTUpdaterDlg()
{
	FreeAll();
}


VOID CRPTUpdaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRPTUpdaterDlg)
	DDX_Control(pDX, IDC_cmbSEARCH, m_ctrlCmbSearch);
	DDX_Text(pDX, IDC_edtSEARCH, m_strSearchData);
	DDX_Text(pDX, IDC_edtSEQ_NUM, m_strSeqNo);
	DDX_Text(pDX, IDC_edtTrialCntMain, m_strTrialCnt);
	DDX_Text(pDX, IDC_edtLIB_NAME, m_strSearchLibName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRPTUpdaterDlg, CDialog)
	//{{AFX_MSG_MAP(CRPTUpdaterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnSEARCH, OnbtnSEARCH)
	ON_BN_CLICKED(IDC_btnSTART, OnbtnSTART)
	ON_BN_CLICKED(IDC_btnBringSeqNo, OnbtnBringSeqNo)
	ON_MESSAGE(CHILD_PROCESS_END,OnChildProcessEnd)
	ON_BN_CLICKED(IDC_btnLIB_SEARCH, OnbtnLIBSEARCH)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRPTUpdaterDlg message handlers

BOOL CRPTUpdaterDlg::OnInitDialog()
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

	//test
	CRect rect;
	GetWindowRect(rect);
	this->MoveWindow(rect.left+100,rect.top+200,rect.Width(),rect.Height());


	if(m_Api.InitRptUpdater()<0) 
	{
		return FALSE;	
	}
	
	InitGrid();
	SetGrid();
	

	//combobox init
	m_ctrlCmbSearch.InsertString(0,_T("제목"));
	m_ctrlCmbSearch.InsertString(1,_T("업무단계"));
	m_ctrlCmbSearch.SetCurSel(0);

	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CRPTUpdaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

VOID CRPTUpdaterDlg::OnPaint() 
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
HCURSOR CRPTUpdaterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


VOID CRPTUpdaterDlg::InitGrid()
{
	
	// Grid 초기화
	m_pGridLibList = new CESL_Grid(NULL);
	m_pGridLibList->SubclassDlgItem(IDC_gridLIB_LIST, this);

	m_pGridRptList = new CESL_Grid(NULL);
	m_pGridRptList->SubclassDlgItem(IDC_gridRPT_LIST, this);

}

VOID CRPTUpdaterDlg::SetGrid()
{
	
	m_pGridLibList->SetCols(0,5);
    m_pGridLibList->SetColumnInfo(0-1, _T(""),					0,				1);
	m_pGridLibList->SetColumnInfo(1-1, _T("도서관명"),			200,			1);
	m_pGridLibList->SetColumnInfo(2-1, _T("DB IP"),				100,  			1);
	m_pGridLibList->SetColumnInfo(3-1, _T("방화벽"),			50,				4);
	m_pGridLibList->SetColumnInfo(4-1, _T("선정"),				40,				1);

	m_pGridRptList->SetCols(0,4);
	m_pGridRptList->SetColumnInfo(0-1, _T(""),					0,				1);
    m_pGridRptList->SetColumnInfo(1-1, _T("GROUP ALIAS"),		110,			1);
	m_pGridRptList->SetColumnInfo(2-1, _T("ALIAS"),				190,  			1);
	m_pGridRptList->SetColumnInfo(3-1, _T("선정"),				40,				1);	


	m_pGridLibList->m_bCheck = TRUE;
	m_pGridLibList->m_bMenu = TRUE;
	m_pGridLibList->SetSelectionInfo('S',_T("V"),3);


	m_pGridRptList->m_bCheck = TRUE;
	m_pGridRptList->m_bMenu = TRUE;
	m_pGridRptList->SetSelectionInfo('S',_T("V"),2);


	ShowLibList();
	ShowRptList();

}


VOID CRPTUpdaterDlg::ShowLibList()
{
	// DM column 2 : strLibName
	// DM column 3 : strIp 
	// DM column 4 : strFirewallYn 

	INT nRow = m_Api.m_DM_MYSQL.GetRowCount();

	
	if(nRow>0)
	{
		m_pGridLibList->SetRows(nRow+1);
	}
	else
	{
		m_pGridLibList->SetRows(2);
		m_pGridLibList->SetRow(1);
		
		for(INT i=0;i<5;i++)
		{
			m_pGridLibList->SetCol(i);
			m_pGridLibList->SetText(_T(""));
		
		}
	}

	CString strTmp;
	CString strLibName;
	CString strIp;
	CString strFirewallYn;
	
	

	for(INT i=0;i<nRow;i++)
	{		
		
		m_pGridLibList->SetRow(i+1);

		strLibName = m_Api.m_DM_MYSQL.GetCellData(i,1);
		strIp = m_Api.m_DM_MYSQL.GetCellData(i,2);
		strFirewallYn = m_Api.m_DM_MYSQL.GetCellData(i,3);
		
		m_pGridLibList->SetCol(1);
		m_pGridLibList->SetText(strLibName);

		m_pGridLibList->SetCol(2);
		m_pGridLibList->SetText(strIp);

		m_pGridLibList->SetCol(3);
		m_pGridLibList->SetText(strFirewallYn);

	}	


}

VOID CRPTUpdaterDlg::ShowRptList()
{
	
	// DM column 2 : group
	// DM column 3 : alias 

	INT nRow = m_Api.m_DM.GetRowCount();

	if(nRow>0)
	{
		m_pGridRptList->SetRows(nRow+1);
	}
	else
	{
		m_pGridRptList->SetRows(2);
		m_pGridRptList->SetRow(1);
		
		for(INT i=0;i<4;i++)
		{
			m_pGridRptList->SetCol(i);
			m_pGridRptList->SetText(_T(""));
		}
	}


	CString strTmp;
	CString strGroup;
	CString strAlias;
	INT nPos = -1;	

	for(INT i=0;i<nRow;i++)
	{		
		
		m_pGridRptList->SetRow(i+1);

		strGroup = m_Api.m_DM.GetCellData(i,2);
		strAlias = m_Api.m_DM.GetCellData(i,3);
		
		m_pGridRptList->SetCol(1);
		m_pGridRptList->SetText(strGroup);
	

		m_pGridRptList->SetCol(2);
		m_pGridRptList->SetText(strAlias);

	}	


}



VOID CRPTUpdaterDlg::FreeAll()
{
	if(m_pGridLibList)
	{
		delete m_pGridLibList;
		m_pGridLibList = NULL;
	}
	
	
	if(m_pGridRptList)
	{
		delete m_pGridRptList;
		m_pGridRptList = NULL;
	}

}

VOID CRPTUpdaterDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	return;
	CDialog::OnOK();
}

VOID CRPTUpdaterDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	// UpdateProcess를 수행하는 Child를 띄운다

	if(0 == nIDEvent)
	{
		CreateChild();
	}

	CDialog::OnTimer(nIDEvent);
}

INT CRPTUpdaterDlg::CreateChild()
{
	
	if(m_nChildCnt == MAX_CHILD) return 0;
	
	INT nTotalProcess;
	nTotalProcess = m_Api.m_listConnectInfo.GetCount()-1;
	
	//모든 process완료
	if(m_nCurrentTnsIdx>nTotalProcess) {
		
		KillTimer(0);

		return 0;
	}
	
	PROCESS_INFORMATION pi;
	STARTUPINFO si;

	CString strPK;
	CString strLibName;
	CString strSID;
	CString strFirewallYn;

	CTns* pTns = NULL;
	POSITION pos = m_Api.m_listConnectInfo.FindIndex(m_nCurrentTnsIdx);
	pTns = (CTns*)m_Api.m_listConnectInfo.GetAt(pos);
	
	strPK = pTns->m_strLIB_PK;
	strLibName = pTns->m_strLIBNAME;
	strSID = pTns->m_strKEY_VALUE;
	strFirewallYn = pTns->m_strFIREWALL_YN;


	si.cb = sizeof(STARTUPINFO);
	ZeroMemory( &si, sizeof(si) );
	ZeroMemory( &pi, sizeof(pi) );
	
	INT nPosition = m_nCurrentTnsIdx;

	for(INT i=0;i<MAX_CHILD;i++)
	{
		if(m_bUsePosition[i] == FALSE) {
			nPosition = i;
			break;
		}
	}

	if(nPosition>=MAX_CHILD) 
	{
		nPosition = nPosition%MAX_CHILD;
	}

	CString strParentIdx;
	strParentIdx.Format(_T("%d"),nPosition);

	
	m_bUsePosition[nPosition] = TRUE;
	
	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);
	CString strCommand;
	strCommand.Format(_T("%s\\RPTUpdaterChild.exe [%s/%s/%s/%s/%s/%s/%s]"),
						szBuffer,m_strSeqNo,m_strTrialCnt,strPK,strLibName,strParentIdx,strSID,strFirewallYn);
	

	BOOL bSuccess = FALSE;
	bSuccess = CreateProcess(NULL,
							strCommand.GetBuffer(0),
							NULL,
							NULL, 
							FALSE, 
							0, 
							NULL, 
							NULL, 
							&si, 
							&pi);
	
			
		
	if(!bSuccess)
	{
		AfxMessageBox(_T("CREATEPROCESS FAILED!"));
		return -1;
	}
	

	//현재 진행 정도를 나타내는 idx;
	m_nCurrentTnsIdx++;
	
	//현재 child 갯수
	m_nChildCnt++;


	return 0;
}


VOID CRPTUpdaterDlg::OnChildProcessEnd(WPARAM w,LPARAM l)
{
	
	m_nChildCnt--;

	INT nIdx = -1;
	nIdx = (INT)w;
	m_bUsePosition[nIdx] = FALSE;



	if(0 == m_nChildCnt)
	{
		AfxMessageBox(_T("모든 작업이 완료 되었습니다"));

		
		CRptUpdateResultViewer view;
		view.m_strSeqNo = m_strSeqNo;
		view.DoModal();
		
	}

}


LRESULT CRPTUpdaterDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	

	return CDialog::DefWindowProc(message, wParam, lParam);
}


INT CRPTUpdaterDlg::LogMacro(INT nTnsIdx,INT nSuccess)
{
	


	return 0;
}


VOID CRPTUpdaterDlg::OnbtnSEARCH() 
{
	// TODO: Add your control notification handler code here
	//RPT정보를 검색한다
		
	UpdateData(TRUE);
	
	if(m_strSearchData.IsEmpty())
	{
		AfxMessageBox(_T("검색 조건이 입력되지 않았습니다!"));
		return;
	}
	
	INT nCurSel = m_ctrlCmbSearch.GetCurSel();
	m_Api.SearchPptInfo(m_strSearchData, nCurSel);

	ShowRptList();
}

VOID CRPTUpdaterDlg::OnbtnSTART() 
{
	// TODO: Add your control notification handler code here
	//선정 정보를 Api에 넘겨주고 , api의 procedure를 실행한다
	
	m_pGridLibList->SelectMakeList();
	m_pGridRptList->SelectMakeList();
	
	INT nLibCnt = m_pGridLibList->SelectGetCount();
	INT nRptCnt = m_pGridRptList->SelectGetCount();

	if(0 == nLibCnt)
	{
		AfxMessageBox(_T("선정된 도서관이 없습니다!"));
		return;
	}
	
	if(0 == nRptCnt)
	{
		AfxMessageBox(_T("선정된 출력 자료가 없습니다!"));
		return;
	}
	

	//선택된 도서관 중에 DB IP가 없는 도서관이 있는지를 검사한다 
	//선정정보 copy
	INT idx;
	CString strIP;

	m_Api.m_arraySelectedLib.RemoveAll();
	m_Api.m_arraySelectedDMIdx.RemoveAll();

	idx = m_pGridLibList->SelectGetHeadPosition();
	
	while(idx>=0)
	{
		strIP = m_Api.m_DM_MYSQL.GetCellData(idx,2);
		if(strIP.IsEmpty())
		{
			AfxMessageBox(_T("선정된 도서관 중에 DB SERVER IP가 없는 도서관이 있습니다!"));
			return;
		}

		
		m_Api.m_arraySelectedLib.Add(idx);
		idx = m_pGridLibList->SelectGetNext();
	}

	idx = m_pGridRptList->SelectGetHeadPosition();
	
	while(idx>=0)
	{
		m_Api.m_arraySelectedDMIdx.Add(idx);
		idx = m_pGridRptList->SelectGetNext();
	}
	
	//차수정보 설정
	if(m_bNewSeqNo) //새 차수 
	{
		CString strSeqNo;
		if(m_Api.GetNewSeqNo(strSeqNo)<0) 
		{
			AfxMessageBox(_T("새차수를 얻지 못하였습니다!"));
			return;
		}
		else 
		{
			m_strSeqNo = strSeqNo;
			m_strTrialCnt = _T("1");
			UpdateData(FALSE);
		}
	}
	else //기존 차수 
	{
		
		CString strTrialCnt;
		if(m_Api.GetTrialCnt(m_strSeqNo,strTrialCnt)<0) 
		{
			AfxMessageBox(_T("새로운 실행 횟수를 얻지 못하였습니다!"));
			return;
		}
		else
		{
			m_strTrialCnt = strTrialCnt;
			UpdateData(FALSE);
		}
	}
	
	
	//_tmain procedure
	BeginWaitCursor();

	if(m_Api.SetUpdateRptData()<0) 
	{
		AfxMessageBox(_T("업데이트 정보 설정에 실패 했습니다!"));
		return;
	}

	EndWaitCursor();

	//index initialize
	m_nCurrentTnsIdx = 0;


	//child position initialize
	for(INT i=0;i<MAX_CHILD;i++)
	{
		m_bUsePosition[i] = FALSE;
	}
	
	SetTimer(0,1000,NULL);
}

VOID CRPTUpdaterDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	// tnsname을 원래대로 복귀한다
	m_Api.ClearTmpData();

	CDialog::OnCancel();
}

VOID CRPTUpdaterDlg::OnbtnBringSeqNo() 
{
	// TODO: Add your control notification handler code here
	CRptUpdateResultViewer view;
	INT ids = 0;
	ids = view.DoModal();
	if(11 == ids)
	{
		
		CString strSeqNo;
		CString strRptAliasList;
		CString strLibPKList;
		CString strFailLibPKList;
		
		strSeqNo = view.m_strSeqNo;
		strRptAliasList = view.m_strCurRptAliasList;
		strLibPKList = view.m_strCurLibPKList;
		strFailLibPKList = view.m_strCurFailLibPKList;

		CArray<INT,INT> arrayFailLibIdx;

		ids = m_Api.InitRetrial(strSeqNo, 
								strRptAliasList,
								strLibPKList,
								strFailLibPKList,
								arrayFailLibIdx);
		


		if(ids<0)
		{
			AfxMessageBox(_T("선택된 정보를 가져오는데 실패 했습니다!"));
			return;
		}


		SetGrid();
		
		m_strTrialCnt = view.m_strCurTrialCnt;
		m_strSeqNo = strSeqNo;
		m_bNewSeqNo = FALSE;
		
		//모든 레포트 알리아스를 선정하고,도서관은 실패한 도서관에만 선정한다 
		SetGridCheck(arrayFailLibIdx);
		
		//그리드와 검색버튼을 비활성화 시킨다
		EnableControls(FALSE);

		UpdateData(FALSE);
	}
}

VOID CRPTUpdaterDlg::SetGridCheck(CArray<INT,INT>& arrayFailLibIdx)
{
	
	//RPT ALIAS
	INT nRow = m_Api.m_DM.GetRowCount();

	for(INT i=0;i<nRow;i++)
	{		
		m_pGridRptList->SetRow(i+1);
		m_pGridRptList->SetCol(3);
		m_pGridRptList->SetText(_T("V"));
	}	

	

	//도서관 
	//먼저 모든 check된것을 해지
	nRow = m_Api.m_DM_MYSQL.GetRowCount();
	
	for(i=0;i<nRow;i++)
	{		
		m_pGridLibList->SetRow(i+1);
		m_pGridLibList->SetCol(4);
		m_pGridLibList->SetText(_T(""));
	}	


	INT nSize = arrayFailLibIdx.GetSize();
	INT idx = -1;

	for(i=0;i<nSize;i++)
	{		
		idx = arrayFailLibIdx.GetAt(i);
		m_pGridLibList->SetRow(idx+1);
		m_pGridLibList->SetCol(4);
		m_pGridLibList->SetText(_T("V"));
	}	

}

VOID CRPTUpdaterDlg::EnableControls(BOOL bActive)
{
	GetDlgItem(IDC_btnSEARCH)->EnableWindow(bActive);
	m_pGridRptList->EnableWindow(bActive);
	m_pGridLibList->EnableWindow(bActive);
}
BEGIN_EVENTSINK_MAP(CRPTUpdaterDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CRPTUpdaterDlg)
	ON_EVENT(CRPTUpdaterDlg, IDC_gridRPT_LIST, -601 /* DblClick */, OnDblClickgridRPTLIST, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CRPTUpdaterDlg::OnDblClickgridRPTLIST() 
{
	// TODO: Add your control notification handler code here
	//레포트이 ALIAS을 더블클릭시 해당 레포트에 대한 LOG를 보여준다
	
	//alias column : 3 
	INT nCnt = m_Api.m_DM.GetRowCount();
	if(0 == nCnt) return;
		
	INT nRow = m_pGridRptList->GetRow();
	
	CString strAlias;

	strAlias = m_Api.m_DM.GetCellData(nRow-1,3);
	if(strAlias.IsEmpty()) return;


	CRptUpdateLogViewer viewer;
	viewer.m_strRptAlias = strAlias;
	viewer.DoModal();

}

VOID CRPTUpdaterDlg::OnbtnLIBSEARCH() 
{
	// TODO: Add your control notification handler code here
	// 도서관명 검색
	UpdateData(TRUE);
	
	if(m_Api.SearchLibInfo(m_strSearchLibName)<0)
	{
		AfxMessageBox(_T("다시 검색해 주십시요!"));
	}

	ShowLibList();
}
