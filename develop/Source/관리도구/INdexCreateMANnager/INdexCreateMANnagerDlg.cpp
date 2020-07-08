// INdexCreateMANnagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "INdexCreateMANnager.h"
#include "INdexCreateMANnagerDlg.h"
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
// CINdexCreateMANnagerDlg dialog

CINdexCreateMANnagerDlg::CINdexCreateMANnagerDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CINdexCreateMANnagerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	
}

CINdexCreateMANnagerDlg::~CINdexCreateMANnagerDlg()
{
}

void CINdexCreateMANnagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CINdexCreateMANnagerDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CINdexCreateMANnagerDlg, CDialog)
	//{{AFX_MSG_MAP(CINdexCreateMANnagerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bStart, OnbStart)
	ON_WM_CTLCOLOR()	
	ON_BN_CLICKED(IDC_btnCOUNT_CHECK, OnbtnCOUNTCHECK)	
	ON_BN_CLICKED(IDC_bConTest, OnbConTest)
	ON_BN_CLICKED(IDC_chkINDEX, OnchkINDEX)
	ON_BN_CLICKED(IDC_chkDETAIL, OnchkDETAIL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CINdexCreateMANnagerDlg message handlers

BOOL CINdexCreateMANnagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	EnableThemeDialogTexture(GetSafeHwnd()); 
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

	( (CButton*) GetDlgItem (IDC_chkINDEX))->SetCheck(TRUE);
	( (CButton*) GetDlgItem (IDC_chkDETAIL))->SetCheck(TRUE);
	m_bIndex = TRUE;
	m_bDetail = TRUE;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	CDialog::SetIcon(m_hIcon, TRUE);			// Set big icon
	CDialog::SetIcon(m_hIcon, FALSE);		// Set small icon	

	// default 접속정보 Set
	( (CEdit*) GetDlgItem (IDC_eID	) )->SetWindowText(_T("KOLAS"));
	( (CEdit*) GetDlgItem (IDC_ePASS) )->SetWindowText(_T("ECO_KOLAS3#"));
	( (CEdit*) GetDlgItem (IDC_eSID	) )->SetWindowText(_T("K3"));

	m_strConnectionInfo = _T("");	
	InitIndexMap();
	InitESL();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CINdexCreateMANnagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CINdexCreateMANnagerDlg::OnPaint() 
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
HCURSOR CINdexCreateMANnagerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CINdexCreateMANnagerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CINdexCreateMANnagerDlg::InitESL()
{
	if (InitESL_Mgr(_T("SM_INC_MAN")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	m_pInfo = new CESL_Information();
	CIndexTool *pIndexTool = new CIndexTool;
	m_pInfo->pIndexTool = (CObject*)pIndexTool;
	m_pIndexTool = pIndexTool;

	//m_pInfo->USER_ID = _T("ECO8");
	
	//===================================================
	//2009.06.11 ADD BY PJW : 현재 PROCESSID의 이름을 가지는 파일에 MANAGE_CODE를 저장한다.
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
	
	
 	strLine.Format(_T("%s\r\n"), m_pInfo->MANAGE_CODE );
	fd.WriteString(strLine);

	fd.Close();
	//===================================================
	
	m_pInfo->InitMarcMgr();
	m_pInfo->pMarcMgr->SetMarcGroup("통합마크");
	m_pMarcMgr = m_pInfo->pMarcMgr;
	
	m_pDM  = FindDM(_T("DM_INC_MAN"));
	if(m_pDM == NULL) AfxMessageBox (_T(" FindDM() Error"));

	m_pDM_Index  = FindDM(_T("DM_INC_MAN_INDEX"));
	if(m_pDM_Index == NULL) AfxMessageBox (_T(" FindDM() Error"));
	
	return TRUE;
	
}

UINT BeginUpload_Thread(LPVOID pParam)
{
	CINdexCreateMANnagerDlg *pMain = (CINdexCreateMANnagerDlg*)pParam;
	if (pMain == NULL) return -1;

	CString strStreamMarc;
	CString strQuery;
	CString strRecKey;
	CMarc	marc;
	INT		nRowCount;
	INT		nTotalCount;
	INT		ids;
	INT		nIndex;

	CString strTotalCount;
	CString strProgressText;

	CString strIndexItem;
	CString strIndexDMFieldAlias;
	CString strIndexToolKitAlias;
	CString strIdxTitle;
	CString strIdxAuthor;
	CString strIdxPublisher;
	CString strIdxKeyword;
	CString strIdxSubject;
	CString strManageCode;
	CString strCheckManageCode;
	
	CIndex *pIndex = pMain->m_pIndexTool->GetIndex();


	// 전체 개수 구해오기
	pMain->m_pDM->GetOneValueQuery(_T("SELECT COUNT(1) FROM BO_SPECIES_TBL"), strTotalCount);
	nTotalCount = _ttoi(strTotalCount);
	// Max Count 설정
    pMain->ShowStatus( nTotalCount, 0);	

	// log 파일이 있을 경우 이어서 하도록 함.
	CStdioFile st;	
	CString strFileName;
	CString strContinueCount;
	CString strTmp;
	INT		Offset;

	( (CEdit*) pMain->GetDlgItem (IDC_edtLIB_NAME) )->GetWindowText(strFileName);
	strFileName = _T("..\\tunning\\") + strFileName + _T(".config");

	CFileFind filefind;
	INT nContinue;

	BOOL bFind = filefind.FindFile(strFileName);	

	if( bFind ) 
	{
		st.Open( strFileName, CFile::modeRead | CFile::typeBinary );
		if ( st == NULL )
		{
			AfxMessageBox(_T("로그 파일 생성에 실패."));
			return 0;
		}
		st.Seek(2, CFile::begin);
		st.ReadString(strContinueCount);		
		
		if ( strContinueCount.GetLength() > 3 )
		{
			Offset = strContinueCount.Find(_T(","),0);			
			nContinue = _ttoi(strContinueCount.Left(Offset));
			if ( nContinue == nTotalCount ) 
			{
				AfxMessageBox(_T("완료된 상태입니다."));
				return 0;
			}
			strTmp.Format(_T("%d / %s 이어서 하시겠습니까?"), nContinue, strTotalCount );
			if ( AfxMessageBox(strTmp) != IDOK )
			{
				st.Close();
				return 0;
			}
			pMain->ShowStatus(nTotalCount, nContinue);
			pMain->m_ProgressCtrl.SetPos( nContinue );
			strRecKey = strContinueCount.Mid(Offset+1);
		}
		else
		{
			strRecKey = _T("0");
			nContinue = 0;
		}
		st.Close();
	}
	else
	{
		strRecKey = _T("0");
		nContinue = 0;
	}	
	
	nIndex = nContinue;
	pMain->m_pDM_Index->FreeData();

	// 쿼리 만들때 무시할 Alias Array
	CArray<CString, CString> arrRemoveAliasList;
	arrRemoveAliasList.RemoveAll();

	DWORD nSTART,nFINISH;
	nSTART = GetTickCount();
	// 현재시간 구하기
	COleDateTime tStartTime = COleDateTime::GetCurrentTime();
	( (CEdit*) pMain->GetDlgItem (IDC_eMANAGE_CODE) )->GetWindowText(strCheckManageCode);
	strCheckManageCode.TrimLeft();
	strCheckManageCode.TrimRight();

	while(1)
	{
		ids = pMain->m_pDM_Index->StartFrame();	
		strQuery.Format(_T("REC_KEY > %s AND ROWNUM < 6"),  strRecKey );
		//strQuery.Format(_T("REC_KEY = %d"),  0 );
		pMain->m_pDM->RefreshDataManager(strQuery);

		nRowCount = pMain->m_pDM->GetRowCount();
		if ( nRowCount <= 0 ) break;

		pMain->m_pDM_Index->FreeData();		
		for ( int i=0 ; i<nRowCount ; i++ )
		{				
			strStreamMarc = pMain->m_pDM->GetCellData( _T("MARC"), i);
			strRecKey = pMain->m_pDM->GetCellData( _T("REC_KEY"), i);
			strManageCode = pMain->m_pDM->GetCellData( _T("MANAGE_CODE"), i);
			if ( (strCheckManageCode !=_T("")) && (strManageCode != strCheckManageCode) ) 
			{				
				nIndex++;
				pMain->ShowStatus( nTotalCount, nIndex );
				continue;
			}

			ids = pMain->m_pMarcMgr->Decoding(strStreamMarc, &marc);
			if (ids < 0) return ids;
			
			//SELECT /*+ INDEX_ASC(BO_SPECIES_TBL BO_SPECIES_TBL_PK) */ REC_KEY,MARC FROM BO_SPECIES_TBL WHERE REC_KEY > 1 AND ROWNUM < 100
			
			// 색인 재생성
			if ( pMain->m_bIndex )
			{
				pMain->m_pIndexTool->Init(pMain->m_pMarcMgr, &marc);				
				// 정의된 Index ToolKit의 항목을 가져와 세팅한다.		
				pMain->m_pDM_Index->InsertRow(-1);
				POSITION pos = pMain->m_mapIndexDMtoIndexToolKit.GetStartPosition();
				while (pos)
				{
					strIndexItem.Empty();
					pMain->m_mapIndexDMtoIndexToolKit.GetNextAssoc(pos, strIndexDMFieldAlias, strIndexToolKitAlias);
					
					ids = pMain->m_pIndexTool->GetIndex(strIndexToolKitAlias, strIndexItem);
					if (ids < 0) continue;
					
					//pIndex->MakeIndex(strIndexItem);
					
					pMain->m_pDM_Index->SetCellData(strIndexDMFieldAlias, strIndexItem, i);
				}
				
				// IDX_ALL_ITEM필드 설정 - 검색용본표제, 검색용저자, 검색용 발행자, 검색용색인어
				ids = pMain->m_pDM_Index->GetCellData( _T("IBS_검색용본표제"), i, strIdxTitle);
				ids = pMain->m_pDM_Index->GetCellData( _T("IBS_검색용저작자"), i, strIdxAuthor);
				ids = pMain->m_pDM_Index->GetCellData( _T("IBS_검색용발행자"), i, strIdxPublisher);
				ids = pMain->m_pDM_Index->GetCellData( _T("IBS_검색용키워드"), i, strIdxKeyword);
				
				CString strIdxAllItem = strIdxTitle + _T(" ") + strIdxAuthor + _T(" ") + strIdxPublisher + _T(" ") + strIdxKeyword + _T(" ") ;
				ids = pMain->m_pDM_Index->SetCellData( _T("IDX_ALL_ITEM"), strIdxAllItem, i);
				
				// IDX_BO_TBL DB Update
				pMain->m_pDM_Index->InitDBFieldData();
				pMain->m_pDM_Index->MakeQueryByDM(arrRemoveAliasList, pMain->m_pDM_Index, _T("IDX_BO_TBL"), i, pMain->m_pDM_Index, _T("REC_KEY"), _T("NUMERIC"), strRecKey );			
			}

			if ( pMain->m_bDetail )
			{
				// BO_SPECIES_TBL MARC Update
				pMain->m_pDM_Index->InitDBFieldData();				
				pMain->m_pMarcMgr->Encoding(&marc, strStreamMarc);			
				pMain->m_pDM_Index->AddDBFieldData(_T("MARC"),  _T("STRING"), strStreamMarc);
				pMain->m_pDM_Index->MakeUpdateFrame(_T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey);			
			}
			nIndex++;
			pMain->ShowStatus( nTotalCount, nIndex );
		}	
		
		nFINISH = GetTickCount();

		pMain->RemainTimeDisplay(tStartTime, nFINISH-nSTART, nTotalCount, nIndex);
		pMain->WriteProcessInfo(strFileName, nIndex, strRecKey);
		ids = pMain->m_pDM_Index->SendFrame(TRUE);
		ids = pMain->m_pDM_Index->EndFrame();
		//break;
	}
	
	

	AfxMessageBox(_T("완료되었습니다"));

	//pMain->SendMessage(WM_CLOSE, NULL, NULL );

	return TRUE;
}

VOID CINdexCreateMANnagerDlg::WriteProcessInfo(CString strFileName, INT nCount, CString strRecKey)
{
	FILE* fp;		
	fp = _tfopen (strFileName, _T("wb"));
	if (fp == NULL) return ;		
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
	// KOLAS 2 UP UNICODE 수정작업
	// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}
	// ------------------------------------------------------------------------------
	
	_ftprintf (fp, _T("%d, %s"), nCount, strRecKey);
	
	fclose(fp);
}

VOID CINdexCreateMANnagerDlg::RemainTimeDisplay(COleDateTime tStartTime, DWORD dTime, INT nTotalCount, INT nIndex)
{
	CString strRemainTime;
	float nRemainTime;
	float nCountTime;
	nCountTime = ((float)dTime/1000);	

	// 남은시간 구하기
	nRemainTime = nCountTime*((float)nTotalCount/(float)nIndex);	
	COleDateTimeSpan tSpen(0,0,0,nRemainTime);


	// 현재시간에 남은시간 더하기
	COleDateTime tend = tStartTime + tSpen;
	
	/*
	nRemainTime = nCountTime*((float)nTotalCount/(float)nIndex);	
	CTime tProcessTime (0,0,0,0,0,nRemainTime);		

    //CTimeSpan span = COleDateTime::GetCurrentTime() - dtDDay;    // span에는 31일이라는 값이 들어갑니다.
	CTimeSpan span = CTime::GetCurrentTime() + nRemainTime;
	*/	
	
	//nRemainTime = nCountTime*((float)nTotalCount/(float)nIndex)/60;
	//strRemainTime.Format(_T("%02.02f 분  "), nRemainTime );
	strRemainTime.Format(_T("%d일 %02d:%02d:%02d  "), tend.GetDay(), tend.GetHour(), tend.GetMinute(), tend.GetSecond() );
	( (CStatic*) GetDlgItem (IDC_sRemainTime) )->SetWindowText(strRemainTime);
}

void CINdexCreateMANnagerDlg::ShowStatus( INT nRowCount, INT iFinishedCnt )
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
}


void CINdexCreateMANnagerDlg::OnbStart() 
{
	if ( m_strConnectionInfo==_T("") )
	{
		AfxMessageBox(_T("접속정보 TEST를 먼저 수행하세요"));
		return;
	}

	( (CButton*) GetDlgItem (IDC_chkDETAIL ) )->EnableWindow(FALSE);
	( (CButton*) GetDlgItem (IDC_chkINDEX ) )->EnableWindow(FALSE);


	AfxBeginThread(	BeginUpload_Thread, this);		
}

HBRUSH CINdexCreateMANnagerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

void CINdexCreateMANnagerDlg::OnbtnCOUNTCHECK() 
{
	if ( m_strConnectionInfo==_T("") )
	{
		AfxMessageBox(_T("접속정보 TEST를 먼저 수행하세요"));
		return;
	}
	
	CString strValue;
	m_pDM->GetOneValueQuery(_T("SELECT COUNT(1) FROM BO_SPECIES_TBL"), strValue);
	m_ProgressCtrl.SetWindowText(strValue + _T(" 건"));
	
}

void CINdexCreateMANnagerDlg::OnchkALL() 
{
		
}

VOID CINdexCreateMANnagerDlg::InitIndexMap()
{
	m_mapIndexDMtoIndexToolKit.RemoveAll();
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용저작자"), _T("저작자"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_범위검색용발행년도"), _T("범위검색용발행년도"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용범위검색발행자"), _T("범위검색용발행자"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용정렬용본표제"), _T("범위검색용본표제"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용키워드"), _T("키워드파일"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용발행자"), _T("발행자"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용본표제"), _T("본표제"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_표제사항"), _T("표제사항"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_저작자사항"), _T("저작자사항"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_발행자사항"), _T("발행자사항"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_발행년도사항"), _T("발행년도사항"));
}

void CINdexCreateMANnagerDlg::OnbConTest() 
{
	CString strID;
	CString strPASS;
	CString strSID;
	CString strValue;
	// 접속정보 Get
	( (CEdit*) GetDlgItem (IDC_eID	) )->GetWindowText(strID	);
	( (CEdit*) GetDlgItem (IDC_ePASS) )->GetWindowText(strPASS	);
	( (CEdit*) GetDlgItem (IDC_eSID	) )->GetWindowText(strSID	);

	m_strConnectionInfo.Format(_T("%s@%s/%s[]"),strID, strSID, strPASS);


	m_pDM->SetCONNECTION_INFO(m_strConnectionInfo);
	m_pDM_Index->SetCONNECTION_INFO(m_strConnectionInfo);

	INT ids;
	ids = m_pDM->GetOneValueQuery(_T("SELECT LIB_NAME FROM MN_LIBINFO2_TBL"), strValue );
	if ( ids > 0 )
	{		

		( (CEdit*) GetDlgItem(IDC_edtLIB_NAME) )->SetWindowText(strValue);
		CESL_DataMgr pDM;
		pDM.SetCONNECTION_INFO(m_strConnectionInfo);
		CComboBox *pManageCode;
		pManageCode = (CComboBox*) GetDlgItem (IDC_cmbMANAGE_CODE);
		
		pDM.RestructDataManager(_T("SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL ORDER BY MANAGE_CODE"));
		pDM.MANAGE_CODE = _T("");
		
		INT nCount;
		
		nCount = pDM.GetRowCount();
		for ( int i=0 ; i<nCount ; i++ )
		{
			pManageCode->AddString(pDM.GetCellData(i,0));		
		}	
		OnbtnCOUNTCHECK();
		AfxMessageBox(_T("SUCESS"));
	}
	else
	{
		AfxMessageBox(_T("FAIL"));
		m_strConnectionInfo = _T("");
	}
}

void CINdexCreateMANnagerDlg::OnchkINDEX() 
{
	m_bIndex = ( (CButton*) GetDlgItem (IDC_chkINDEX ) )->GetCheck();	
}

void CINdexCreateMANnagerDlg::OnchkDETAIL() 
{
	m_bDetail = ( (CButton*) GetDlgItem (IDC_chkDETAIL ) )->GetCheck();
	
}
