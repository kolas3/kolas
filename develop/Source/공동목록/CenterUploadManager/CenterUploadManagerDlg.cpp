// CenterUploadManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CenterUploadManager.h"
#include "CenterUploadManagerDlg.h"
#include "..\..\include\공동목록\CU_ComApi.h"
#include "..\..\공동목록\check_dup_api\rs_sql.h"
#include "..\CenterUpload_api\CenterUpload.h"
#include "..\..\공통\IndexToolKit\IndexTool.h"

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
// CCenterUploadManagerDlg dialog

CCenterUploadManagerDlg::CCenterUploadManagerDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCenterUploadManagerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_pBaseDataMgr = NULL;
	m_pDupDataMgr = NULL;
	m_DupDBAlias = _T("");   
	m_pDBAlias = _T("");    
	m_strUploadFileName = _T("");
	m_UserID = _T("");
	m_ManageCode = _T("");
}

CCenterUploadManagerDlg::~CCenterUploadManagerDlg()
{
}

void CCenterUploadManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCenterUploadManagerDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCenterUploadManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CCenterUploadManagerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bStart, OnbStart)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCenterUploadManagerDlg message handlers

BOOL CCenterUploadManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	CRect rcDesktop;
	INT	nDesktopWidth, nDesktopHeight;

	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	::SystemParametersInfo(SPI_GETWORKAREA,0,&rcDesktop,0);
	nDesktopWidth=rcDesktop.right-rcDesktop.left;
	nDesktopHeight=rcDesktop.bottom-rcDesktop.top;	

	BOOL bTaskbarOnRight=nDesktopWidth<cx && rcDesktop.left==0;
	BOOL bTaskbarOnLeft=nDesktopWidth<cx && rcDesktop.left!=0;
	BOOL bTaskBarOnTop=nDesktopHeight<cy && rcDesktop.top!=0;
	BOOL bTaskbarOnBottom=nDesktopHeight<cy && rcDesktop.top==0;
	
	CRect clientRect;
	GetWindowRect(clientRect);
	//GetClientRect(clientRect);
	INT nScreenX, nScreenY;
	if ( bTaskbarOnRight )
	{
		nScreenX = nDesktopWidth - clientRect.Width();		
	}
	else
	{
		nScreenX = cx - clientRect.Width();
	}
	if ( bTaskbarOnBottom )
	{
		nScreenY = nDesktopHeight - clientRect.Height();
	}
	else
	{
		nScreenY = cy-clientRect.Height();
	}

	MoveWindow(nScreenX, nScreenY, clientRect.Width(), clientRect.Height() );


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	CDialog::SetIcon(m_hIcon, TRUE);			// Set big icon
	CDialog::SetIcon(m_hIcon, FALSE);		// Set small icon	

	InitESL();	

	OnbStart();

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCenterUploadManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCenterUploadManagerDlg::OnPaint() 
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
HCURSOR CCenterUploadManagerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCenterUploadManagerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CCenterUploadManagerDlg::InitESL()
{
	DWORD dwPID = ::GetCurrentProcessId();

	CStdioFile fd;
	CString strFileName;
	CString strLine;

  	strFileName.Format(_T("..\\CFG\\EFS\\user_info(%u).log"), dwPID);
	
	
	if (!fd.Open(strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) 
	{
		return -1;
	}

	if( 0 == fd.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fd.Write( &cUni, sizeof(TCHAR));
	}
	
	
 	strLine.Format(_T("%s\r\n"), m_ManageCode );
	fd.WriteString(strLine);

	fd.Close();

	if (InitESL_Mgr(_T("SM_CENTERUPLOAD_DAEMON")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	m_pInfo = new CESL_Information();
	CIndexTool *pIndexTool = new CIndexTool;
	m_pInfo->pIndexTool = (CObject*)pIndexTool;

	m_pInfo->USER_ID = m_UserID;
	m_pInfo->MANAGE_CODE = m_ManageCode;
	
	m_pInfo->InitMarcMgr();

	m_pDBAlias = _T("DM_CENTERUPLOAD_DAEMON_대상자료") ;
	m_DupDBAlias = _T("DM_CENTERUPLOAD_DAEMON_복본자료");   

	m_pBaseDataMgr = FindDM(m_pDBAlias);
	if(m_pBaseDataMgr == NULL) AfxMessageBox (_T(" FindDM() Error")) ;

	m_pDupDataMgr  = FindDM(m_DupDBAlias);
	if(m_pDupDataMgr == NULL) AfxMessageBox (_T(" FindDM() Error")) ;    

	return TRUE;
	
}

UINT BeginUpload_Thread(LPVOID pParam)
{
	CCenterUploadManagerDlg *pMain = (CCenterUploadManagerDlg*)pParam;
	if (pMain == NULL) return -1;

	CString lib_code;
	TCHAR sExtraInfo[100];
	INT nRowCount;
	INT	ids;
	INT i;	
	INT iEtcErrorCnt;
	INT iErrorCnt;
	INT iFinishedCnt;	
	INT	iSyntaxErrorCnt;
	INT	iSpeciesCnt;
	INT	nTempUploadCnt;

	nRowCount = 0 ;
    iSpeciesCnt = 0;
	nTempUploadCnt = 0;

	ids = pMain->MakeUploadData(pMain->m_strUploadFileName);
	if ( ids ) 
	{
		pMain->SendMessage(WM_CLOSE, NULL, NULL );
		return FALSE;
	}
	
	nRowCount = pMain->m_pBaseDataMgr->GetRowCount();

    pMain->ShowStatus( nRowCount, 0, 0, 0, 0 );

	CRS_SQL m_RS_SQL(pMain) ;
    ids  = m_RS_SQL.GetLibraryInfo (lib_code) ;
	if ( ids ) 
	{
		pMain->SendMessage(WM_CLOSE, NULL, NULL );
		return -1;
	}	
	
	pMain->m_pInfo->LIB_CODE = lib_code;	
	pMain->m_pInfo->LOCAL_IP = pMain->m_strLocalIP;

	CCenterUpload Check_Upload_api(pMain) ;
	CString str_DIRECT_UPLOAD_YN ;
	ids = Check_Upload_api.Check_Direct_Upload (str_DIRECT_UPLOAD_YN) ;	
	if ( ids ) 
	{
		pMain->SendMessage(WM_CLOSE, NULL, NULL );
		return -1;
	}	

	iEtcErrorCnt	= 0;
	iErrorCnt		= 0;
	iFinishedCnt	= 0;	
	iSyntaxErrorCnt = 0;

	CString strRegNos , strUseLimitCodes , strSpeciesLimitCode;

	CString strSpeciesKey ;
	CString strMarc ;

	CString strControlNo;

	for ( i=0 ; i<nRowCount ; i++ ) 
	{
		if(str_DIRECT_UPLOAD_YN == _T("N"))
		{   
			pMain->m_pBaseDataMgr->GetCellData (_T("REC_KEY"),i,strSpeciesKey) ;
			pMain->m_pBaseDataMgr->GetCellData (_T("MARC"),i,strMarc) ;		
			
			pMain->m_pBaseDataMgr->GetCellData (_T("제어번호"), i, strControlNo);

			pMain->m_pBaseDataMgr->GetCellData (_T("이용제한구분"), i, strSpeciesLimitCode);
			pMain->MakeStringData( strControlNo, strRegNos , strUseLimitCodes );
			
			ids = Check_Upload_api.Input_Temp_Mater_tbl (strSpeciesKey, strMarc, _T(""), strControlNo, strRegNos , strUseLimitCodes ,strSpeciesLimitCode);

			if(ids < 0 )
			{
				ids = UpdateUploadStatus( pMain, pMain->m_pDBAlias, i, _T("4") );
				if (ids < 0) 				
				{
					if ( AfxMessageBox(_T("업로드를 실패하였습니다.\n계속 진행하시겠습니까?"), MB_YESNO, MB_ICONQUESTION  ) == IDYES )
					{
						iEtcErrorCnt ++ ;
						iFinishedCnt ++ ;				
						pMain->ShowStatus( nRowCount, iFinishedCnt, nTempUploadCnt, iSyntaxErrorCnt, iEtcErrorCnt );
						continue;
					}
					else
					{			
						pMain->SendMessage(WM_CLOSE, NULL, NULL );
						return -1;
					}
				}
				
				iEtcErrorCnt ++ ;
				iFinishedCnt ++ ;
				pMain->ShowStatus( nRowCount, iFinishedCnt, nTempUploadCnt, iSyntaxErrorCnt, iEtcErrorCnt );				
				continue;
			}			
			ids = UpdateUploadStatus( pMain, pMain->m_pDBAlias, i, _T("5") );		
			if (ids<0) AfxMessageBox (_T("UpdateUploadStatus(5) Error!!!"), FALSE);
			
			nTempUploadCnt ++;
			iFinishedCnt ++ ;			
			pMain->ShowStatus( nRowCount, iFinishedCnt, nTempUploadCnt, iSyntaxErrorCnt, iEtcErrorCnt );			
			continue ;
		}
		else
		{

		}
	}

	if( nTempUploadCnt > 0 && str_DIRECT_UPLOAD_YN == _T("N") )
	{
		ids = Check_Upload_api.Update_Temp_Lib_Statistics_Tbl(nTempUploadCnt,_T("U")) ;        
	}


	pMain->SendMessage(WM_CLOSE, NULL, NULL );

	return TRUE;
}

void CCenterUploadManagerDlg::ShowStatus( INT nRowCount, INT iFinishedCnt, INT iDupMatCnt, INT iSyntaxErrorCnt, INT iEtcErrorCnt )
{
	CString frm;

    if ( iFinishedCnt == 0 ) {
        m_ProgressCtrl.SetRange( 0, nRowCount );
        m_ProgressCtrl.SetStep( 1 );
        m_ProgressCtrl.SetPos( 0 );
    } 
	else 
	{
		frm.Format( _T("%d / %d"), iFinishedCnt, nRowCount );
		m_ProgressCtrl.SetWindowText(frm);
        m_ProgressCtrl.StepIt();		
    }    
    
    frm.Format( _T("%d 건"), iDupMatCnt );
    GetDlgItem(IDC_STATIC_2)->SetWindowText( frm );
    frm.Format( _T("%d 건"), iSyntaxErrorCnt );
    GetDlgItem(IDC_STATIC_3)->SetWindowText( frm );
    frm.Format( _T("%d 건"), iEtcErrorCnt );
    GetDlgItem(IDC_STATIC_4)->SetWindowText( frm );
	
}

INT CCenterUploadManagerDlg::MakeUploadData(CString strFileName)
{
	CString strWhere;
	CStdioFile file;
	CString strFullFileName;
	CString strLine;	
	CString strControlList;	
	CStringArray strArrayControlList;

	strFullFileName.Format(_T("%s"), strFileName);
	if ( file.Open( strFullFileName, CFile::modeRead | CFile::typeBinary ) ) 
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}

		strArrayControlList.RemoveAll();
		while ( file.ReadString( strLine ) ) 
		{
			strLine.Replace(_T("\r"), _T(""));
			strLine.Replace(_T("\n"), _T(""));
			strLine.TrimLeft(_T(" "));
			strLine.TrimRight(_T(" "));
			if ( strLine != _T("" )) strArrayControlList.Add(strLine);						        
		}
	}
	else
	{
		return -100;
	}
	file.Close();

	CString strControlNo;
	INT		nCount;	
	nCount = strArrayControlList.GetSize();
	strControlList = _T("");
	for ( int i=0 ; i<nCount ; i++ )
	{
		strControlNo = strArrayControlList.GetAt(i);
		if ( strControlList.Find(strControlNo) < 0 )
		{
			if ( i>0 ) strControlList += _T(",");
			strControlList += _T("'")+strControlNo+_T("'");
		}
	}

	if ( nCount == 0 )
	{
		CDialog::OnClose();
		return 0;
	}

	strWhere.Format(_T(" S.CONTROL_NO IN ( %s ) AND I.MANAGE_CODE = UDF_MANAGE_CODE "), strControlList );

	m_pBaseDataMgr->RefreshDataManager(strWhere);	
	return 0;
}

VOID CCenterUploadManagerDlg::SetUploadFile(CString strFileName)
{
	INT offset;

	offset = strFileName.Find(_T(" "),0);
	m_UserID = strFileName.Left(offset);

	strFileName = strFileName.Mid(offset);
	strFileName.TrimLeft();	strFileName.TrimRight();
	offset = strFileName.Find(_T(" "),0);
	m_ManageCode = strFileName.Left(offset);

	strFileName = strFileName.Mid(offset);
	strFileName.TrimLeft();	strFileName.TrimRight();
	offset = strFileName.Find(_T(" "),0);
	m_strUploadFileName = strFileName.Left(offset);

	m_strLocalIP = strFileName.Mid(offset+1);
}

void CCenterUploadManagerDlg::OnbStart() 
{
	AfxBeginThread(	BeginUpload_Thread, this);		
}

HBRUSH CCenterUploadManagerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;

}

VOID CCenterUploadManagerDlg::MakeStringData( CString ControlNo , CString &Reg_nos , CString &UseLimitCodes )
{
	if(ControlNo.IsEmpty()) return;
	
	Reg_nos = _T("");
	UseLimitCodes = _T("");
	
	CESL_DataMgr pDM;
	CString strQuery;
	int ids;
	
	strQuery.Format(
		_T("SELECT BB.REG_NO , BB.USE_LIMIT_CODE FROM BO_BOOK_TBL BB ")
		_T("WHERE BB.SPECIES_KEY = ( SELECT MAX(BS.REC_KEY) FROM BO_SPECIES_TBL BS WHERE BS.CONTROL_NO = '%s' AND BS.MANAGE_CODE = UDF_MANAGE_CODE ) ")
		,ControlNo
		);
	
	pDM.SetCONNECTION_INFO(m_pBaseDataMgr->CONNECTION_INFO);
	
	ids = pDM.RestructDataManager(strQuery);
	
	if (ids < 0 ) return;
	if ( pDM.GetRowCount() <= 0 ) return;
	
	CString Reg_No = _T(""), UseLimitCode = _T("");
	
	for ( int i=0 ; i < pDM.GetRowCount()  ; i++ )
	{
		Reg_No = _T("");
		UseLimitCode = _T("");
		
		pDM.GetCellData(  i , 0 , Reg_No );		
		
		if ( Reg_nos.IsEmpty() ) { Reg_nos += Reg_No; }
		else 
		{
			if (!Reg_No.IsEmpty()) Reg_nos += _T(",") + Reg_No;
		}
		
		pDM.GetCellData( i , 1 , UseLimitCode );
		
		if ( UseLimitCodes.IsEmpty() ){ UseLimitCodes += UseLimitCode; }
		else
		{
			if (!UseLimitCode.IsEmpty())  UseLimitCodes += _T(",") + UseLimitCode;
		}
		
	}
	
}
