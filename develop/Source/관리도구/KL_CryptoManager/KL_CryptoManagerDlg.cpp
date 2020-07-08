// KL_CryptoManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KL_CryptoManager.h"
#include "KL_CryptoManagerDlg.h"

// DES 암복호화
// #include "FWDES.h"

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
// CKL_CryptoManagerDlg dialog

CKL_CryptoManagerDlg::CKL_CryptoManagerDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_CryptoManagerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	
}
CKL_CryptoManagerDlg::~CKL_CryptoManagerDlg()
{
	if ( m_pInfo != NULL) delete m_pInfo;
}

void CKL_CryptoManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_CryptoManagerDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_ProgressCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKL_CryptoManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_CryptoManagerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bConTest, OnbConTest)
	ON_BN_CLICKED(IDC_btnCOUNT_CHECK, OnbtnCOUNTCHECK)
	ON_BN_CLICKED(IDC_bStart, OnbStart)
	ON_BN_CLICKED(IDC_rENCODING, OnrENCODING)
	ON_BN_CLICKED(IDC_rDECODING, OnrDECODING)
	ON_BN_CLICKED(IDC_rSYNC, OnrSYNC)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_CryptoManagerDlg message handlers

BOOL CKL_CryptoManagerDlg::OnInitDialog()
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// default 접속정보 Set
	if(_T("K2UP") == m_strMode)
	{
		( (CEdit*) GetDlgItem (IDC_eID	) )->SetWindowText(_T("kolas2"));
		( (CEdit*) GetDlgItem (IDC_ePASS) )->SetWindowText(_T("kolas2#"));
		( (CEdit*) GetDlgItem (IDC_eSID	) )->SetWindowText(_T("k2up"));
	}
	else
	{
		( (CEdit*) GetDlgItem (IDC_eID	) )->SetWindowText(_T("KOLAS"));
		( (CEdit*) GetDlgItem (IDC_ePASS) )->SetWindowText(_T("ECO_KOLAS3#"));
		( (CEdit*) GetDlgItem (IDC_eSID	) )->SetWindowText(_T("K3"));
	}
	m_strConnectionInfo = _T("");
	InitESL();
	
	if(_T("K2UP") == m_strMode)
	{
		GetDlgItem(IDC_STATIC_MANAGE_CODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_eMANAGE_CODE)->ShowWindow(SW_HIDE);

		SetWindowText(_T("KL Crypto For KOLAS2UP (암호화)"));
	}

	( (CButton*) GetDlgItem (IDC_rENCODING) )->SetCheck(TRUE);
	m_bEncodingMode = 0;

	// TEST CODE
// 	CString strSetData = _T("D677ABCE24156E7D");
// 	CString strGetData;
// 	ESL_DES_Decryption(strSetData, strGetData);
// 	strSetData = strGetData;
// 	ESL_DES_Encryption(strSetData, strGetData);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKL_CryptoManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CKL_CryptoManagerDlg::OnPaint() 
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
HCURSOR CKL_CryptoManagerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}




BOOL CKL_CryptoManagerDlg::InitESL()
{
	if (InitESL_Mgr(_T("SM_KL_CRYPTO")) < 0)		                
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	m_pInfo = new CESL_Information();	
	if(_T("K2UP") == m_strMode)
	{
		m_pDM  = FindDM(_T("DM_KL_CRYPTO_K2UP"));
	}
	else
	{
		m_pDM  = FindDM(_T("DM_KL_CRYPTO"));
	}	
	if(m_pDM == NULL) AfxMessageBox (_T(" FindDM() Error"));

	// K3의 경우 로그인되면서 로그인이용자의 관리구분이 설정가능하지만 
	// 이 프로그램내에서는 사용불가이다.
	m_pDM->MANAGE_CODE = _T("없음");
	
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
	
	

	
	return TRUE;
	
}

void CKL_CryptoManagerDlg::OnbConTest()
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

	m_pDM->CONNECTION_INFO = m_strConnectionInfo;
	m_pDM->ParseCONNECTION_INFO();
	

	INT ids;
  	ids = m_pDM->GetOneValueQuery(_T("SELECT LIB_NAME FROM MN_LIBINFO2_TBL"), strValue );
	if ( ids > 0 )
	{		

		( (CEdit*) GetDlgItem(IDC_edtLIB_NAME) )->SetWindowText(strValue);
		CESL_DataMgr pDM;
		pDM.CONNECTION_INFO = m_strConnectionInfo;
		pDM.ParseCONNECTION_INFO();

// 		pDM.SetCONNECTION_INFO(m_strConnectionInfo);
		CComboBox *pManageCode;
		pManageCode = (CComboBox*) GetDlgItem (IDC_cmbMANAGE_CODE);
		
		ids = pDM.RestructDataManager(_T("SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL ORDER BY MANAGE_CODE"));
		pDM.MANAGE_CODE = _T("");
		
		INT nCount;
		
		nCount = pDM.GetRowCount();
		for ( int i=0 ; i<nCount ; i++ )
		{
			pManageCode->AddString(pDM.GetCellData(i,0));		
		}	
		OnbtnCOUNTCHECK();
		AfxMessageBox(_T("SUCESS"));

		GetDlgItem(IDC_bStart)->EnableWindow(TRUE);

		GetDlgItem(IDC_eSID)->EnableWindow(FALSE);
		GetDlgItem(IDC_eID)->EnableWindow(FALSE);		
		GetDlgItem(IDC_ePASS)->EnableWindow(FALSE);
		GetDlgItem(IDC_eMANAGE_CODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_edtLIB_NAME)->EnableWindow(FALSE);
	}
	else
	{
		AfxMessageBox(_T("FAIL"));
		GetDlgItem(IDC_bStart)->EnableWindow(FALSE);
		m_strConnectionInfo = _T("");
	}
}

void CKL_CryptoManagerDlg::OnbtnCOUNTCHECK() 
{
	if ( m_strConnectionInfo==_T("") )
	{
		AfxMessageBox(_T("접속정보 TEST를 먼저 수행하세요"));
		return;
	}
	
	if ( m_bEncodingMode == 0)
	{
		CString strQuery, strValue;
		strQuery.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE CRYPTO_CIVIL_NO IS NULL AND CIVIL_NO IS NOT NULL"));
		m_pDM->GetOneValueQuery(strQuery, strValue);
		m_ProgressCtrl.SetWindowText(_T("암호화대상 건수 : ") + strValue + _T(" 건"));
	}
	else if ( m_bEncodingMode == 1 )
	{
		CString strQuery, strValue;
		strQuery.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE CRYPTO_CIVIL_NO IS NOT NULL AND LENGTH(CIVIL_NO)=14 AND CIVIL_NO LIKE '%%*%%'"));
		m_pDM->GetOneValueQuery(strQuery, strValue);
		m_ProgressCtrl.SetWindowText(_T("복호화대상 건수 : ") + strValue + _T(" 건"));
	}
	else
	{
		CString strQuery, strValue;
		strQuery.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE CRYPTO_CIVIL_NO IS NOT NULL"));
		m_pDM->GetOneValueQuery(strQuery, strValue);
		m_ProgressCtrl.SetWindowText(_T("재생성대상 건수 : ") + strValue + _T(" 건"));
	}
}



void CKL_CryptoManagerDlg::OnbStart() 
{
// 	m_pDM->SetCONNECTION_INFO(m_strConnectionInfo);

	if ( m_bEncodingMode == 0 )
	{
		if ( AfxMessageBox(_T("정말 암호화 하시겠습니까?"), MB_YESNO) == IDNO )
		{
			return;
		}
	}
	else if ( m_bEncodingMode == 1 )
	{
		if ( AfxMessageBox(_T("정말 복호화 하시겠습니까?"), MB_YESNO) == IDNO )
		{
			return;
		}
	}
	else
	{
		if ( AfxMessageBox(_T("정말 재생성 하시겠습니까?"), MB_YESNO) == IDNO )
		{
			return;
		}
	}	
	m_pDM->CONNECTION_INFO = m_strConnectionInfo;
	m_pDM->ParseCONNECTION_INFO();

	GetDlgItem(IDC_bStart)->EnableWindow(FALSE);
	GetDlgItem(IDC_bConTest)->EnableWindow(FALSE);

	if ( m_bEncodingMode == 0 )
	{
		AfxBeginThread(	BeginUpload_Thread, this);			
	}
	else if ( m_bEncodingMode == 1 )
	{
		AfxBeginThread(	BeginUpload_Thread2, this);			
	}
	else
	{
		AfxBeginThread(	BeginUpload_SYNC, this);	
	}
}

UINT BeginUpload_Thread(LPVOID pParam)
{
	CKL_CryptoManagerDlg *pMain = (CKL_CryptoManagerDlg*)pParam;
	if (pMain == NULL) return -1;

	CString strQuery;
	CString strRecKey;
	CString strCheckManageCode;
	CString strCivilNo;
	CString strCryptoCivilNo;
	CString strManageCode;
	CString strCheckString;
	CString strCivilNoTemp;
	INT		nRowCount;
	INT		nTotalCount;
	INT		ids;
	INT		nIndex;
 	INT		nErrorCode = 0;

	CString strTotalCount;
	CString strProgressText;

	pMain->m_pDM->GetOneValueQuery(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE CRYPTO_CIVIL_NO IS NULL AND CIVIL_NO IS NOT NULL"), strTotalCount);
	// 전체 개수 구해오기

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
	bFind = FALSE;

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

	CString strQuery2;
	if(strCheckManageCode.IsEmpty())
	{
		strQuery2 = _T("SELECT MIN(VALUE_1) FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = '통합도서서비스' AND VALUE_NAME = '주민번호보안설정사용여부'");
	}
	else
	{
		strQuery2.Format(
			_T("SELECT MIN(VALUE_1) FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = '통합도서서비스' AND VALUE_NAME = '주민번호보안설정사용여부' AND MANAGE_CODE = '%s'"),
			strCheckManageCode);
	}	

	BOOL bIsUpdateCivilNo = TRUE;
	CString strValue2;
	pMain->m_pDM->GetOneValueQuery(strQuery2, strValue2);
	if(_T("N") == strValue2)
	{
		bIsUpdateCivilNo = FALSE;
	}
	else
	{
		bIsUpdateCivilNo = TRUE;
	}

// 	CFWDES DES;
	while(1)
	{
		ids = pMain->m_pDM->StartFrame();	

		if(_T("K2UP") == pMain->m_strMode)
		{
			strQuery.Format(
				_T("SELECT REC_KEY, CIVIL_NO, CRYPTO_CIVIL_NO, CIVIL_NO_TEMP, NULL ")
				_T("FROM ")
					_T("(SELECT CLU.REC_KEY, CLU.CIVIL_NO, CLU.CRYPTO_CIVIL_NO, CLU.CIVIL_NO_TEMP, NULL ")
					_T("FROM CO_LOAN_USER_TBL CLU WHERE CLU.REC_KEY > %s AND CLU.CRYPTO_CIVIL_NO IS NULL AND CLU.CIVIL_NO IS NOT NULL ORDER BY REC_KEY ASC) ")
				_T("WHERE ROWNUM < 300 "),
			strRecKey );
		}
		else
		{
			strQuery.Format(
				_T("SELECT REC_KEY, CIVIL_NO, CRYPTO_CIVIL_NO, CIVIL_NO_TEMP, MANAGE_CODE ")
				_T("FROM ")
					_T("(SELECT CLU.REC_KEY, CLU.CIVIL_NO, CLU.CRYPTO_CIVIL_NO, CLU.CIVIL_NO_TEMP, CLU.MANAGE_CODE ")
					_T("FROM CO_LOAN_USER_TBL CLU WHERE CLU.REC_KEY > %s AND CLU.CRYPTO_CIVIL_NO IS NULL AND CLU.CIVIL_NO IS NOT NULL ORDER BY REC_KEY ASC) ")
				_T("WHERE ROWNUM < 300 "),
			strRecKey );
		}
		//strQuery.Format(_T("REC_KEY = %d"),  0 );
// 		pMain->m_pDM->RefreshDataManager(strQuery);
		pMain->m_pDM->RestructDataManager(strQuery);

		nRowCount = pMain->m_pDM->GetRowCount();
		if ( nRowCount <= 0 ) break;

		//pMain->m_pDM->FreeData();		
		for ( int i=0 ; i<nRowCount ; i++ )
		{	
			strCivilNo.Empty();
			strCryptoCivilNo.Empty();
			strRecKey.Empty();
			strManageCode.Empty();
			strCivilNoTemp.Empty();

// 			strCivilNo = pMain->m_pDM->GetCellData( _T("CIVIL_NO"), i);			
// 			strCryptoCivilNo = pMain->m_pDM->GetCellData( _T("CRYPTO_CIVIL_NO"), i);			
// 			strRecKey = pMain->m_pDM->GetCellData( _T("REC_KEY"), i);
// 			strManageCode = pMain->m_pDM->GetCellData( _T("MANAGE_CODE"), i);

			strRecKey = pMain->m_pDM->GetCellData(i, 0);
			strCivilNo = pMain->m_pDM->GetCellData(i, 1);
			strCryptoCivilNo = pMain->m_pDM->GetCellData(i, 2);
			strCivilNoTemp = pMain->m_pDM->GetCellData(i, 3);
			strManageCode = pMain->m_pDM->GetCellData(i, 4);

			
// 			nErrorCode = DES.Simple_Decipher(strCryptoCivilNo, _ESL_DES_KEY_, strCheckString );				
// 			nErrorCode = pMain->ESL_DES_Decryption(strCryptoCivilNo, strCheckString);
			if(
				(!strCryptoCivilNo.IsEmpty() && nErrorCode >= 0) || 
				strCivilNo.IsEmpty() || 
				( (strCheckManageCode !=_T("")) && (strManageCode != strCheckManageCode) ) 
			 )
			{				
				nIndex++;
				pMain->ShowStatus( nTotalCount, nIndex );
				continue;
			}

			if ( strCivilNo.GetLength() == 14 )
			{
				strCheckString = strCivilNo.Left(9) + _T("*****");
			}
			else
			{
				strCheckString = strCivilNo;
			}
						
			// CRYPTO_CIVIL_NO, CIVIL_NO Update
			CString strEnc = pMain->KL_Encryption(strCivilNo);
			pMain->m_pDM->InitDBFieldData();							
			pMain->m_pDM->AddDBFieldData(_T("CRYPTO_CIVIL_NO"),  _T("STRING"), strEnc);
			if(bIsUpdateCivilNo)
			{
				pMain->m_pDM->AddDBFieldData(_T("CIVIL_NO"),  _T("STRING"), strCheckString);
			}
			if(strCivilNoTemp.IsEmpty())
			{
 				pMain->m_pDM->AddDBFieldData(_T("CIVIL_NO_TEMP"),  _T("STRING"), strCivilNo);
			}
			pMain->m_pDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey);

 			CString strLine;// = strCivilNo + _T(",") + strEnc;
			strLine.Format(_T("UPDATE CO_LOAN_USER_TBL SET CRYPTO_CIVIL_NO = NULL, CIVIL_NO = '%s' WHERE REC_KEY = %s"), strCivilNo, strRecKey);
 			pMain->ConvertLog(strLine.GetBuffer(0));			
 			strLine.ReleaseBuffer();
			nIndex++;
			pMain->ShowStatus( nTotalCount, nIndex );
		}	
		
		nFINISH = GetTickCount();

		pMain->RemainTimeDisplay(tStartTime, nFINISH-nSTART, nTotalCount, nIndex);
		pMain->WriteProcessInfo(strFileName, nIndex, strRecKey);
 		ids = pMain->m_pDM->SendFrame(TRUE);
		ids = pMain->m_pDM->EndFrame();	
	}	

	
	AfxMessageBox(_T("완료되었습니다"));	

	return TRUE;
}


UINT BeginUpload_Thread2(LPVOID pParam)
{
	CKL_CryptoManagerDlg *pMain = (CKL_CryptoManagerDlg*)pParam;
	if (pMain == NULL) return -1;

	CString strQuery;
	CString strRecKey;
	CString strCheckManageCode;
	CString strCivilNo;
	CString strCryptoCivilNo;
	CString strManageCode;
	CString strCheckString;
	CString strCivilNoTemp;
	INT		nRowCount;
	INT		nTotalCount;
	INT		ids;
	INT		nIndex;
 	INT		nErrorCode = 0;

	CString strTotalCount;
	CString strProgressText;

	pMain->m_pDM->GetOneValueQuery(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE CRYPTO_CIVIL_NO IS NOT NULL AND LENGTH(CIVIL_NO)=14 AND CIVIL_NO LIKE '%%*%%'"), strTotalCount);
	// 전체 개수 구해오기

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
	bFind = FALSE;

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

	CString strQuery2;
	if(strCheckManageCode.IsEmpty())
	{
		strQuery2 = _T("SELECT MIN(VALUE_1) FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = '통합도서서비스' AND VALUE_NAME = '주민번호보안설정사용여부'");
	}
	else
	{
		strQuery2.Format(
			_T("SELECT MIN(VALUE_1) FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = '통합도서서비스' AND VALUE_NAME = '주민번호보안설정사용여부' AND MANAGE_CODE = '%s'"),
			strCheckManageCode);
	}	

	BOOL bIsUpdateCivilNo = TRUE;
	CString strValue2;
	pMain->m_pDM->GetOneValueQuery(strQuery2, strValue2);
	if(_T("N") == strValue2)
	{
		bIsUpdateCivilNo = FALSE;
	}
	else
	{
		bIsUpdateCivilNo = TRUE;
	}

	CStringArray strArstCivilNo;
	strArstCivilNo.RemoveAll();
	BOOL bLoopEnd=FALSE;

// 	CFWDES DES;
	INT nUpdateCnt;
	INT nStarCnt;

	nStarCnt = 0;
	while(1)
	{
		if ( bLoopEnd == TRUE ) 
		{
			break;
		}		

		if(_T("K2UP") == pMain->m_strMode)
		{
			strQuery.Format(
				_T("SELECT REC_KEY, CIVIL_NO, CRYPTO_CIVIL_NO, CIVIL_NO_TEMP ")
				_T("FROM ")
				_T("(SELECT CLU.REC_KEY, CLU.CIVIL_NO, CLU.CRYPTO_CIVIL_NO, CLU.CIVIL_NO_TEMP ")
				_T("FROM CO_LOAN_USER_TBL CLU WHERE CLU.REC_KEY > 0 AND CRYPTO_CIVIL_NO IS NOT NULL AND LENGTH(CIVIL_NO)=14 AND CIVIL_NO LIKE '%%*%%'ORDER BY REC_KEY ASC) ")
				_T("WHERE ROWNUM < 300 ") );
		}
		else
		{
			strQuery.Format(
				_T("SELECT REC_KEY, CIVIL_NO, CRYPTO_CIVIL_NO, CIVIL_NO_TEMP, MANAGE_CODE ")
				_T("FROM ")
				_T("(SELECT CLU.REC_KEY, CLU.CIVIL_NO, CLU.CRYPTO_CIVIL_NO, CLU.CIVIL_NO_TEMP, CLU.MANAGE_CODE ")
				_T("FROM CO_LOAN_USER_TBL CLU WHERE CLU.REC_KEY > 0 AND CRYPTO_CIVIL_NO IS NOT NULL AND LENGTH(CIVIL_NO)=14 AND CIVIL_NO LIKE '%%*%%'ORDER BY REC_KEY ASC) ")
				_T("WHERE ROWNUM < 300 ") );
		}
		//strQuery.Format(_T("REC_KEY = %d"),  0 );
// 		pMain->m_pDM->RefreshDataManager(strQuery);
		pMain->m_pDM->RestructDataManager(strQuery);

		nRowCount = pMain->m_pDM->GetRowCount();
		if ( nRowCount <= 0 ) break;

		
		nUpdateCnt = 0;
		//pMain->m_pDM->FreeData();	
		ids = pMain->m_pDM->StartFrame();	
		for ( int i=0 ; i<nRowCount ; i++ )
		{	
			strCivilNo.Empty();
			strCryptoCivilNo.Empty();
			strRecKey.Empty();
			strManageCode.Empty();
			strCivilNoTemp.Empty();

			strRecKey = pMain->m_pDM->GetCellData(i, 0);
			strCivilNo = pMain->m_pDM->GetCellData(i, 1);
			strCryptoCivilNo = pMain->m_pDM->GetCellData(i, 2);
			strCivilNoTemp = pMain->m_pDM->GetCellData(i, 3);
			/*
			strManageCode = pMain->m_pDM->GetCellData(i, 4);
			
			if(				
				( (strCheckManageCode !=_T("")) && (strManageCode != strCheckManageCode) ) 
			 )
			{				
				nIndex++;
				pMain->ShowStatus( nTotalCount, nIndex );
				continue;
			}
			*/
						
			// CRYPTO_CIVIL_NO, CIVIL_NO Update
			CString strDec;
			pMain->ESL_DES_Decryption(strCryptoCivilNo, strDec);
			pMain->m_pDM->InitDBFieldData();										

			if ( (strDec.Find(_T("*"), 0) >= 0)  && 
				( strCivilNoTemp.Find(_T("*"), 0) >= 0 || strCivilNoTemp.GetLength() == 0 ) )
			{				
				// 기존에 나왔던 데이터인지 확인한 후 종료
				for ( int m=0 ; m<strArstCivilNo.GetSize() ; m++ )
				{
					if ( strArstCivilNo.GetAt(m) == strDec )
					{
						bLoopEnd = TRUE;
						break;
					}
				}
				if ( bLoopEnd == TRUE ) { break; }
				//CString strMsg;
				//strMsg.Format(_T("주민등록번호 원데이터에 * 문자가 포함되었습니다. 데이터를 확인하세요. \r\n %s"), strDec);
				//if ( AfxMessageBox(strMsg, MB_OKCANCEL) == IDCANCEL ) 
				//{					
				//	bLoopEnd = TRUE;
				//	break;
				//}					

				nStarCnt++;				
				strArstCivilNo.Add(strDec);
			}
			else
			{
				// civil_no_temp에 정상이면
				if (strCivilNoTemp.Find(_T("*"), 0) <0 && strCivilNoTemp.GetLength() == 14 )
				{
					strDec = strCivilNoTemp;
				}
			}
			if(!strDec.IsEmpty())
			{
				pMain->m_pDM->AddDBFieldData(_T("CIVIL_NO"),  _T("STRING"), strDec);
				pMain->m_pDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey);
				nUpdateCnt++;
			}						
 			
			nIndex++;
			pMain->ShowStatus( nTotalCount, nIndex );
		}	
		
		nFINISH = GetTickCount();

		pMain->RemainTimeDisplay(tStartTime, nFINISH-nSTART, nTotalCount, nIndex);
		pMain->WriteProcessInfo(strFileName, nIndex, strRecKey);
 		if ( nUpdateCnt > 0 ) ids = pMain->m_pDM->SendFrame(TRUE);
		ids = pMain->m_pDM->EndFrame();	
	}	

	
	CString strMsg;
	strMsg.Format(_T("%d건의 데이터에 * 문자가 포함되었습니다."), nStarCnt);
	AfxMessageBox(strMsg);

	return TRUE;
}

UINT BeginUpload_SYNC(LPVOID pParam)
{
	CKL_CryptoManagerDlg *pMain = (CKL_CryptoManagerDlg*)pParam;
	if (pMain == NULL) return -1;

	CString strQuery;
	CString strRecKey;
	CString strCheckManageCode;
	CString strCivilNo;
	CString strCryptoCivilNo;
	CString strManageCode;
	CString strCheckString;
	CString strCivilNoTemp;
	INT		nRowCount;
	INT		nTotalCount;
	INT		ids;
	INT		nIndex;
 	INT		nErrorCode = 0;
	INT		nReEncCnt = 0;

	CString strTotalCount;
	CString strProgressText;

	pMain->m_pDM->GetOneValueQuery(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE CRYPTO_CIVIL_NO IS NOT NULL"), strTotalCount);
	// 전체 개수 구해오기

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
	strFileName = _T("..\\tunning\\SYNC_") + strFileName + _T(".config");

	CFileFind filefind;
	INT nContinue;

	BOOL bFind = filefind.FindFile(strFileName);
	bFind = FALSE;

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

	CString strQuery2;
	if(strCheckManageCode.IsEmpty())
	{
		strQuery2 = _T("SELECT MIN(VALUE_1) FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = '통합도서서비스' AND VALUE_NAME = '주민번호보안설정사용여부'");
	}
	else
	{
		strQuery2.Format(
			_T("SELECT MIN(VALUE_1) FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = '통합도서서비스' AND VALUE_NAME = '주민번호보안설정사용여부' AND MANAGE_CODE = '%s'"),
			strCheckManageCode);
	}	

	BOOL bIsUpdateCivilNo = TRUE;
	CString strValue2;
	pMain->m_pDM->GetOneValueQuery(strQuery2, strValue2);
	if(_T("N") == strValue2)
	{
		bIsUpdateCivilNo = FALSE;
	}
	else
	{
		bIsUpdateCivilNo = TRUE;
	}

// 	CFWDES DES;
	CString strEnc;
	while(1)
	{
		ids = pMain->m_pDM->StartFrame();	

		if(_T("K2UP") == pMain->m_strMode)
		{
			strQuery.Format(
				_T("SELECT REC_KEY, CIVIL_NO, CRYPTO_CIVIL_NO, CIVIL_NO_TEMP, NULL ")
				_T("FROM ")
					_T("(SELECT CLU.REC_KEY, CLU.CIVIL_NO, CLU.CRYPTO_CIVIL_NO, CLU.CIVIL_NO_TEMP, NULL ")
					_T("FROM CO_LOAN_USER_TBL CLU WHERE CLU.REC_KEY > %s AND CLU.CRYPTO_CIVIL_NO IS NOT NULL ORDER BY REC_KEY ASC) ")
				_T("WHERE ROWNUM < 300 "),
			strRecKey );
		}
		else
		{
			strQuery.Format(
				_T("SELECT REC_KEY, CIVIL_NO, CRYPTO_CIVIL_NO, CIVIL_NO_TEMP, MANAGE_CODE ")
				_T("FROM ")
					_T("(SELECT CLU.REC_KEY, CLU.CIVIL_NO, CLU.CRYPTO_CIVIL_NO, CLU.CIVIL_NO_TEMP, CLU.MANAGE_CODE ")
					_T("FROM CO_LOAN_USER_TBL CLU WHERE CLU.REC_KEY > %s AND CLU.CRYPTO_CIVIL_NO IS NOT NULL ORDER BY REC_KEY ASC) ")
				_T("WHERE ROWNUM < 300 "),
			strRecKey );
		}
		//strQuery.Format(_T("REC_KEY = %d"),  0 );
// 		pMain->m_pDM->RefreshDataManager(strQuery);
		pMain->m_pDM->RestructDataManager(strQuery);

		nRowCount = pMain->m_pDM->GetRowCount();
		if ( nRowCount <= 0 ) break;

		//pMain->m_pDM->FreeData();		
		for ( int i=0 ; i<nRowCount ; i++ )
		{	
			strCivilNo.Empty();
			strCryptoCivilNo.Empty();
			strRecKey.Empty();
			strManageCode.Empty();
			strCivilNoTemp.Empty();

			strRecKey = pMain->m_pDM->GetCellData(i, 0);
			strCivilNo = pMain->m_pDM->GetCellData(i, 1);
			strCryptoCivilNo = pMain->m_pDM->GetCellData(i, 2);
			strCivilNoTemp = pMain->m_pDM->GetCellData(i, 3);
			strManageCode = pMain->m_pDM->GetCellData(i, 4);						
			
 			nErrorCode = pMain->ESL_DES_Decryption(strCryptoCivilNo, strCheckString);			

			if ( strCheckManageCode.GetLength() == 2 )
			{
				if (strManageCode != strCheckManageCode)
				{
					nIndex++;
					pMain->ShowStatus( nTotalCount, nIndex );
					continue;
				}
			}
				
			if ( (strCheckString.Find(_T("*"),0) < 0 || nErrorCode < 0) )
			{				
				nIndex++;
				pMain->ShowStatus( nTotalCount, nIndex );
				continue;
			}	

			if ( strCivilNo.GetLength() != 14 )
			{
				nIndex++;
				pMain->ShowStatus( nTotalCount, nIndex );
				continue;
			}			
			// CRYPTO_CIVIL_NO, CIVIL_NO Update
			// CIVIL_NO_TEMP 참조
			if ( strCivilNo.Find(_T("*"),0) < 0 )
			{
				nErrorCode = pMain->ESL_DES_Encryption(strCivilNo, strEnc);
				if ( nErrorCode< 0 )
				{				
					nIndex++;
					pMain->ShowStatus( nTotalCount, nIndex );
					continue;
				}				
			}
			else if ( strCivilNoTemp.Find(_T("*"),0) < 0 && strCivilNoTemp.GetLength() == 14) 
			{
				nErrorCode = pMain->ESL_DES_Encryption(strCivilNoTemp, strEnc);				
				if ( nErrorCode< 0 )
				{
					nIndex++;
					pMain->ShowStatus( nTotalCount, nIndex );
					continue;
				}
			}
			else
			{
				nIndex++;
				pMain->ShowStatus( nTotalCount, nIndex );
				continue;
			}
			
			pMain->m_pDM->InitDBFieldData();
			pMain->m_pDM->AddDBFieldData(_T("CRYPTO_CIVIL_NO"),  _T("STRING"), strEnc);	
			nReEncCnt++;
			if (strCivilNo.GetLength() < 14 || strCivilNo.Find(_T("*"), 0 ) >= 0)
			{
				if ( strCheckString.Find(_T("*"), 0 ) < 0 )
				{
 					pMain->m_pDM->AddDBFieldData(_T("CIVIL_NO"),  _T("STRING"), strCheckString);
				}
				else if ( strCivilNo.GetLength() < 14 || strCivilNoTemp.Find(_T("*"), 0 ))
				{
					pMain->m_pDM->AddDBFieldData(_T("CIVIL_NO"),  _T("STRING"), strCivilNoTemp);
				}
			}
		
			if(strCivilNoTemp.IsEmpty())
			{
 				pMain->m_pDM->AddDBFieldData(_T("CIVIL_NO_TEMP"),  _T("STRING"), strCheckString);
				pMain->m_pDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey);
				nIndex++;
			}			

 			CString strLine;// = strCivilNo + _T(",") + strEnc;
			strLine.Format(_T("UPDATE CO_LOAN_USER_TBL SET CRYPTO_CIVIL_NO = NULL WHERE REC_KEY = %s"), strEnc, strRecKey);
 			pMain->ConvertLog(strLine.GetBuffer(0));			
 			strLine.ReleaseBuffer();			
			pMain->ShowStatus( nTotalCount, nIndex );
		}	
		
		nFINISH = GetTickCount();

		pMain->RemainTimeDisplay(tStartTime, nFINISH-nSTART, nTotalCount, nIndex);
		pMain->WriteProcessInfo(strFileName, nIndex, strRecKey);
 		ids = pMain->m_pDM->SendFrame(TRUE);
		ids = pMain->m_pDM->EndFrame();	
	}	

	CString strMsg;
	strMsg.Format(_T("%d 건이 재변환 되었습니다."), nReEncCnt);
	AfxMessageBox(strMsg);	

	return TRUE;
}


VOID CKL_CryptoManagerDlg::RemainTimeDisplay(COleDateTime tStartTime, DWORD dTime, INT nTotalCount, INT nIndex)
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
	
	strRemainTime.Format(_T("%d일 %02d:%02d:%02d  "), tend.GetDay(), tend.GetHour(), tend.GetMinute(), tend.GetSecond() );
	( (CStatic*) GetDlgItem (IDC_sRemainTime) )->SetWindowText(strRemainTime);
}

void CKL_CryptoManagerDlg::ShowStatus( INT nRowCount, INT iFinishedCnt )
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


VOID CKL_CryptoManagerDlg::WriteProcessInfo(CString strFileName, INT nCount, CString strRecKey)
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

CString CKL_CryptoManagerDlg::KL_Encryption(CString strValue )
{	

	INT nErrorCode;
	CString strDesc;
	CFWDES DES;
// 	nErrorCode = m_ESL_DES.Simple_Cipher(strValue, _ESL_DES_KEY_, strDesc );	
	nErrorCode = ESL_DES_Encryption(strValue, strDesc);

	return strDesc;

//	CString strDst;
//	// Simple_Cipher
//	UCHAR* szSrc = NULL;
//	UINT nSrcLen = 0;
//
//	UCHAR* szDst = NULL;
//	UINT nDstLen = 0;
//
//	UCHAR* szKey = NULL;
//
//	// 평문을 8바이트의 배수로 맞춰준다.
//	while(0 != strValue.GetLength()% (8/sizeof(TCHAR)) )
//	{
//		strValue += _T(" ");
//	}
//
//	{		
//		// 소스 변환
//		UINT LEN = strValue.GetLength();
//
//		TCHAR* sz_Tmp = new TCHAR[LEN+1];
//		ZeroMemory(sz_Tmp, LEN);
//		_tcscpy((TCHAR*)sz_Tmp, (TCHAR*)strValue.GetBuffer(0));
//		strValue.ReleaseBuffer();
//		sz_Tmp[LEN] = '\0';
//
//		szSrc = new UCHAR[LEN*sizeof(TCHAR)+1];
//		ZeroMemory(szSrc, LEN*sizeof(TCHAR));
//		szSrc[LEN*sizeof(TCHAR)] = '\0';
//
//		// TCHAR를 바이트단위로 나누어 UCHAR에 설정한다.
//		for(UINT uCnt = 0; uCnt < LEN; uCnt++)
//		{
//			TCHAR Tmp = sz_Tmp[uCnt];
//			szSrc[(uCnt*2)] = (BYTE)(Tmp & 0x00FF);
//			Tmp = Tmp >> 8;
//			szSrc[(uCnt*2)+1] = (BYTE)(Tmp & 0x00FF);			
//		}		
//		nSrcLen = (LEN) * sizeof(TCHAR);
//
//		delete []sz_Tmp;
//	}
//	{
//		// 결과물 메모리 할당
//		UINT LEN = strValue.GetLength();
//		szDst = new UCHAR[LEN*sizeof(TCHAR)+1];
//		ZeroMemory(szDst, LEN*sizeof(TCHAR));
//		szDst[LEN*sizeof(TCHAR)] = '\0';		
//	}	
//	{
//		// 키 메모리 할당
//		const INT KEY_LENGTH = 8/sizeof(TCHAR); // 키는 8바이트
//		while(KEY_LENGTH > _ESL_DES_KEY_.GetLength())
//		{
//			strKey += _T(" ");
//		}
//		strKey = strKey.Left(KEY_LENGTH);
//
//		TCHAR* sz_Tmp = new TCHAR[KEY_LENGTH+1];
//		ZeroMemory(sz_Tmp, KEY_LENGTH);
//		_tcscpy((TCHAR*)sz_Tmp, (TCHAR*)strKey.GetBuffer(0));
//		strKey.ReleaseBuffer();
//		sz_Tmp[KEY_LENGTH] = '\0';
//
//		szKey = new UCHAR[KEY_LENGTH*sizeof(TCHAR)+1];
//		ZeroMemory(szKey, KEY_LENGTH*sizeof(TCHAR));
//		szKey[KEY_LENGTH] = '\0';
//		for(int uCnt = 0; uCnt < KEY_LENGTH; uCnt++)
//		{
//			TCHAR Tmp = sz_Tmp[uCnt];
//			szKey[(uCnt*2)] = (BYTE)(Tmp & 0x00FF);
//			Tmp = Tmp >> 8;
//			szKey[(uCnt*2)+1] = (BYTE)(Tmp & 0x00FF);			
//		}
//
//		delete []sz_Tmp;
//	}
//	
//	// 2. 암호화
//	CFWDES DES;
//	DES.Cipher(szKey, nSrcLen, szSrc, &nDstLen, szDst);
//	
//	// 3. CString 변환
//	{
//		UCHAR* sz_Tmp = new UCHAR[nDstLen+2];
//		ZeroMemory(sz_Tmp, nDstLen+1);
//		sz_Tmp[nDstLen+1] = '\0';
//
//		
//		for(UINT nCnt = 0; nCnt < nDstLen; nCnt++)
//		{			
//			sz_Tmp[nCnt] = szDst[nCnt];			
//		}		
//	 	strDst.Format(_T("%s"), (TCHAR*)sz_Tmp);
//		delete []sz_Tmp;
//	}
//
//	// 4. 메모리 해제
//	delete []szKey;
//	delete []szSrc;
//	delete []szDst;
//
//	return strDst;
}

void CKL_CryptoManagerDlg::ConvertLog(TCHAR * query)
{
EFS_BEGIN

	SYSTEMTIME t;
	GetLocalTime(&t);
	CString filename, msg;
	filename.Format(_T("..\\tunning\\데이터복구용로그_%04d%02d%02d"), t.wYear, t.wMonth, t.wDay);
	msg.Format(_T("/* %04d-%02d-%02d.%02d:%02d:%02d.%03d */ %s\r\n/"), t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds, query);
	
	{
		filename = filename + _T(".txt");

		// 이전소스	
		FILE* fp;
		
		fp = _tfopen (filename, _T("a+b"));
		if (fp == NULL)
		{
			return ;
		}
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE 수정작업
		// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
		fseek(fp, 0, SEEK_END);		
		if( 0 == ftell(fp) )
		{
			
			/*UNCHANGE*/fputc(0xFF, fp);
			/*UNCHANGE*/fputc(0xFE, fp);
			CString strComment = _T("/* 아래 쿼리들은 암호화작업이 잘못되었을 때 원래의 상태로 복구하기 위해 사용되는 쿼리입니다. */");
			_ftprintf (fp, _T("%s\r\n"), strComment);
		}
		// ------------------------------------------------------------------------------
		
		_ftprintf (fp, _T("%s\r\n"), msg);
		
		fclose(fp);
	}
//--2008.10.16 UPDATE BY CJY --}}
EFS_END
}

void CKL_CryptoManagerDlg::OnrENCODING() 
{
	BOOL bCheck;
	bCheck	= ( (CButton*) GetDlgItem(IDC_rENCODING) )->GetCheck();

	if ( bCheck ) 
	{
		m_bEncodingMode = 0;
	}
}

void CKL_CryptoManagerDlg::OnrDECODING() 
{
	BOOL bCheck;
	bCheck	= ( (CButton*) GetDlgItem(IDC_rDECODING) )->GetCheck();		

	if ( bCheck ) 
	{
		m_bEncodingMode = 1;
	}
}

void CKL_CryptoManagerDlg::OnrSYNC() 
{
	BOOL bCheck;
	bCheck	= ( (CButton*) GetDlgItem(IDC_rSYNC) )->GetCheck();		

	if ( bCheck ) 
	{
		m_bEncodingMode = 2;
	}
}
