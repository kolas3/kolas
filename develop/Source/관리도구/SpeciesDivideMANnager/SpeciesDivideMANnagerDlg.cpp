// SpeciesDivideMANnagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpeciesDivideMANnager.h"
#include "SpeciesDivideMANnagerDlg.h"
#include "..\..\공통\IndexToolKit\IndexTool.h"
#include "..\\..\\공통\\MarcAdjustApi\\CatApi_BO.h"


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
// CSpeciesDivideMANnagerDlg dialog

CSpeciesDivideMANnagerDlg::CSpeciesDivideMANnagerDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpeciesDivideMANnagerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	
	
	m_bRunning = FALSE;
	m_bStop = FALSE;
}

CSpeciesDivideMANnagerDlg::~CSpeciesDivideMANnagerDlg()
{
}

void CSpeciesDivideMANnagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpeciesDivideMANnagerDlg)
	DDX_Control(pDX, IDC_PROGRESS2, m_ProgressCtrl2);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSpeciesDivideMANnagerDlg, CDialog)
	//{{AFX_MSG_MAP(CSpeciesDivideMANnagerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bStart, OnbStart)
	ON_WM_CTLCOLOR()	
	ON_BN_CLICKED(IDC_btnCOUNT_CHECK, OnbtnCOUNTCHECK)	
	ON_BN_CLICKED(IDC_bConTest, OnbConTest)	
	ON_BN_CLICKED(IDC_bClose, OnbClose)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpeciesDivideMANnagerDlg message handlers

BOOL CSpeciesDivideMANnagerDlg::OnInitDialog()
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

	m_pInfo->nREG_CODE_LENGTH = 2;

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSpeciesDivideMANnagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSpeciesDivideMANnagerDlg::OnPaint() 
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
HCURSOR CSpeciesDivideMANnagerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSpeciesDivideMANnagerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CSpeciesDivideMANnagerDlg::InitESL()
{
	if (InitESL_Mgr(_T("SM_SD_MAN")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	m_pInfo = new CESL_Information();
	CIndexTool *pIndexTool = new CIndexTool;
	m_pInfo->pIndexTool = (CObject*)pIndexTool;
	m_pIndexTool = pIndexTool;

	//m_pInfo->USER_ID = _T("ECO8");	
	
	m_pInfo->InitMarcMgr();
	m_pInfo->pMarcMgr->SetMarcGroup("통합마크");
	m_pMarcMgr = m_pInfo->pMarcMgr;
	
	m_pDM_INDEX  = FindDM(_T("DM_SD_MAN_INDEX"));
	if(m_pDM_INDEX == NULL) AfxMessageBox (_T(" FindDM() Error"));

	m_pDM_INDEX_SEARCH  = FindDM(_T("DM_SD_MAN_INDEX_SEARCH"));
	if(m_pDM_INDEX == NULL) AfxMessageBox (_T(" FindDM() Error"));

	m_pDM_SPECIES  = FindDM(_T("DM_SD_MAN_SPECIES"));
	if(m_pDM_SPECIES == NULL) AfxMessageBox (_T(" FindDM() Error"));

	m_pDM_SPECIES_TEMP  = FindDM(_T("DM_SD_MAN_SPECIES_TEMP"));
	if(m_pDM_SPECIES == NULL) AfxMessageBox (_T(" FindDM() Error"));

	m_pDM_BOOK  = FindDM(_T("DM_SD_MAN_BOOK"));
	if(m_pDM_BOOK == NULL) AfxMessageBox (_T(" FindDM() Error"));
	
	return TRUE;
	
}

UINT BeginUpload_Thread(LPVOID pParam)
{
	CSpeciesDivideMANnagerDlg *pMain = (CSpeciesDivideMANnagerDlg*)pParam;
	if (pMain == NULL) return -1;

	pMain->m_bRunning = TRUE;

	CCatApi_BO capApi(pMain);

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
	INT		nTotalTotalCount;
	
	CIndex *pIndex = pMain->m_pIndexTool->GetIndex();


	// 전체 개수 구해오기
	pMain->m_pDM_INDEX->GetOneValueQuery(_T("SELECT COUNT(1) FROM IDX_BO_TBL "), strTotalCount);	
	nTotalTotalCount = _ttoi(strTotalCount);
	pMain->ShowTotalStatus( nTotalTotalCount,0);

	// 전체 개수 구해오기
	pMain->m_pDM_INDEX->GetOneValueQuery(_T("SELECT /*+ INDEX_ASC(IDX_BO_TBL IDX_BO_TBL_PK) */ COUNT(1) FROM IDX_BO_TBL WHERE REC_KEY > 1 AND LENGTH(MANAGE_CODE) > 2"), strTotalCount);	
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
	pMain->m_pDM_INDEX->FreeData();

	// 쿼리 만들때 무시할 Alias Array
	CArray<CString, CString> arrRemoveAliasList;
	arrRemoveAliasList.RemoveAll();

	DWORD nSTART,nFINISH;
	nSTART = GetTickCount();
	// 현재시간 구하기
	COleDateTime tStartTime = COleDateTime::GetCurrentTime();
	
	INT i,j, y;
	CString speciesKeyList;
	CString strMakeReckeyQuery;
	INT		nScanCnt;
	CString strScanCount;

	nScanCnt = 0;
	while(1)
	{		
		//strQuery.Format(_T("REC_KEY > %s AND ROWNUM < 6"),  strRecKey );		
		strQuery.Format(_T("REC_KEY > 1 AND LENGTH(MANAGE_CODE) > 2 AND ROWNUM < 6") );		
		//strQuery.Format(_T("REC_KEY IN (20006577,20006583)") );		
		pMain->m_pDM_INDEX_SEARCH->RefreshDataManager(strQuery);
		nScanCnt += 5;
		pMain->ShowTotalStatus( nTotalTotalCount,nScanCnt);		
	
		speciesKeyList = _T("");
		for ( i=0 ; i<pMain->m_pDM_INDEX_SEARCH->GetRowCount() ; i++ )
		{
			if ( !speciesKeyList.IsEmpty() ) speciesKeyList += _T(",");
			speciesKeyList += pMain->m_pDM_INDEX_SEARCH->GetCellData(_T("REC_KEY"),i);
		}

		if ( speciesKeyList.IsEmpty() ) 
		{
			break;
		}
		strQuery.Format(_T("REC_KEY IN (%s) ORDER BY REC_KEY "), speciesKeyList );		
		pMain->m_pDM_INDEX->RefreshDataManager(strQuery);

		if ( pMain->m_pDM_INDEX->GetRowCount() <= 0 )
		{			
			continue;
		}

		// 마크가져오기
		strQuery.Format(_T("REC_KEY IN (%s) ORDER BY REC_KEY "), speciesKeyList );		
		pMain->m_pDM_SPECIES->RefreshDataManager(strQuery);

		nRowCount = pMain->m_pDM_SPECIES->GetRowCount();
		if ( nRowCount <= 0 ) break;		
		
		CString strManageCodeList;				
		CString strNewRecKey;		
		CString strManageCode;
		CString strControlMatCode;
		CString strOriControlNo;
		CString strControlNo;
		CStringArray strArraySpeciesKeyList;
					
		INT nManageCodeCount;

		if ( pMain->m_bStop == TRUE ) 
		{
			pMain->m_bStop = FALSE;
			return TRUE;
		}

		// 종 5개씩 분리작업		
		for ( i=0 ; i<nRowCount ; i++ )
		{					
			ids = pMain->m_pDM_INDEX->StartFrame();	
			pMain->m_pDM_INDEX->InitDBFieldData();

			strArraySpeciesKeyList.RemoveAll();			

			strManageCodeList = pMain->m_pDM_INDEX->GetCellData( _T("MANAGE_CODE"), i);
			strRecKey = pMain->m_pDM_INDEX->GetCellData( _T("REC_KEY"), i);						
			strOriControlNo = pMain->m_pDM_INDEX->GetCellData( _T("CONTROL_NO"), i);			
			strControlMatCode = strOriControlNo.Left(3);
			

			strArraySpeciesKeyList.Add(strRecKey);
			strQuery.Format(_T("UPDATE IDX_BO_TBL SET MANAGE_CODE='%s' WHERE REC_KEY =%s;"), strManageCodeList.Left(2), strRecKey );			
			pMain->m_pDM_INDEX->AddFrame(strQuery);
			strQuery.Format(_T("UPDATE BO_SPECIES_TBL SET MANAGE_CODE='%s' WHERE REC_KEY =%s;"), strManageCodeList.Left(2), strRecKey );
			pMain->m_pDM_INDEX->AddFrame(strQuery);

			// Manage_Code 개수만큼 색인, 종 생성			
			nManageCodeCount = (strManageCodeList.GetLength() + 1 ) /3;

			// 원본을 뺀 나머지 만큼 복사 : 개수-1 개 복사
			for ( y=1 ; y<nManageCodeCount ; y++ )
			{	
				strManageCode = strManageCodeList.Mid(y*3,2);

				ids = pMain->m_pDM_SPECIES->MakeRecKey(strNewRecKey);				
				strArraySpeciesKeyList.Add(strNewRecKey);

				strControlNo = pMain->GetLastControlNo(strControlMatCode, strManageCode);

				// 색인 복사
				strQuery.Format(_T("INSERT INTO IDX_BO_TBL (REC_KEY,MANAGE_CODE, WORK_CODE,REG_CODE,MAT_CODE,FORM_CODE,MEDIA_CODE,USE_OBJ_CODE,USE_LIMIT_CODE,SUB_REG_CODE,IDX_TITLE,IDX_KEYWORD,IDX_AUTHOR,IDX_PUBLISHER,IDX_ITITLE,IDX_IPUBLISHER,IDX_IPUB_YEAR,IDX_ALL_ITEM,IDX_VOL,IDX_VOL_TITLE,IDX_PLACE,IDX_HOLD_PLACE,IDX_SE_SHELF_CODE,TITLE_INFO,AUTHOR_INFO,PUB_INFO,PUB_YEAR_INFO,PLACE_INFO,MAIN_PLACE_INFO,EDIT_INFO,TITLE,AUTHOR,PUBLISHER,PUB_YEAR,ST_CODE,ST_ISSN,ST_STRN,ST_RNSTRN,ST_CBN,ST_CAN,SE_SHELF_CODE,CLASS_NO_TYPE,CLASS_NO,BOOK_CODE,VOL_CODE,VOL_CODE_DISP,CONTROL_NO,KDCP_CLASS,KDC_CLASS,DDC_CLASS,CEC_CLASS,CWC_CLASS,COC_CLASS,UDC_CLASS,NDC_CLASS,LC_CLASS,ETC_CLASS,UNI_CODE,GOV_CODE,DEG_CODE,EDIT,TEXT_LANG,SUM_LANG,SPECIES_CLASS,HOLD_DATE,DUP_YN,WORKING_STATUS,PUBLISH_COUNTRY_CODE,DELETE_DATE,CONTENTS_YN,ABSTRACTS_YN,WONMUN_YN,MANAGE_NO,IDX_SUBJECT,USER_DEFINE_CODE1,USER_DEFINE_CODE2,USER_DEFINE_CODE3,IDX_ICS,IDX_STANDARDNO,STANDARDNO_INFO, FIRST_WORK)")
					_T("SELECT %s,'%s',WORK_CODE,REG_CODE,MAT_CODE,FORM_CODE,MEDIA_CODE,USE_OBJ_CODE,USE_LIMIT_CODE,SUB_REG_CODE,IDX_TITLE,IDX_KEYWORD,IDX_AUTHOR,IDX_PUBLISHER,IDX_ITITLE,IDX_IPUBLISHER,IDX_IPUB_YEAR,IDX_ALL_ITEM,IDX_VOL,IDX_VOL_TITLE,IDX_PLACE,IDX_HOLD_PLACE,IDX_SE_SHELF_CODE,TITLE_INFO,AUTHOR_INFO,PUB_INFO,PUB_YEAR_INFO,PLACE_INFO,MAIN_PLACE_INFO,EDIT_INFO,TITLE,AUTHOR,PUBLISHER,PUB_YEAR,ST_CODE,ST_ISSN,ST_STRN,ST_RNSTRN,ST_CBN,ST_CAN,SE_SHELF_CODE,CLASS_NO_TYPE,CLASS_NO,BOOK_CODE,VOL_CODE,VOL_CODE_DISP,'%s',KDCP_CLASS,KDC_CLASS,DDC_CLASS,CEC_CLASS,CWC_CLASS,COC_CLASS,UDC_CLASS,NDC_CLASS,LC_CLASS,ETC_CLASS,UNI_CODE,GOV_CODE,DEG_CODE,EDIT,TEXT_LANG,SUM_LANG,SPECIES_CLASS,HOLD_DATE,DUP_YN,WORKING_STATUS,PUBLISH_COUNTRY_CODE,DELETE_DATE,CONTENTS_YN,ABSTRACTS_YN,WONMUN_YN,MANAGE_NO,IDX_SUBJECT,USER_DEFINE_CODE1,USER_DEFINE_CODE2,USER_DEFINE_CODE3,IDX_ICS,IDX_STANDARDNO,STANDARDNO_INFO, '종분리 일괄작업 20081204' FROM IDX_BO_TBL WHERE REC_KEY=%s;")
					, strNewRecKey, strManageCode, strControlNo, strRecKey );
				pMain->m_pDM_INDEX->AddFrame(strQuery);

				// 종 복사
				strQuery.Format(_T("INSERT INTO BO_SPECIES_TBL (REC_KEY,MANAGE_CODE,ACQ_YEAR,WORK_CODE,CONTROL_NO,CONTROL_MAT_CODE,MAT_CODE,FORM_CODE,SUB_REG_CODE,MEDIA_CODE,USE_OBJ_CODE,USE_LIMIT_CODE,LOC_FLAG,SUBJECT_CODE,CLASS_NO_TYPE,KDC_CLASS_NO,KDC_CLASS_NO_VER,DDC_CLASS_NO,DDC_CLASS_NO_VER,ECT_CLASS_NO,SHELF_LOC_CODE,TOC_COMMON_KEY,CENTER_UPLOAD_STATUS,REMARK,WORKER,INPUT_DATE,DUP_FLAG,APPLY_YN,CATALOG_STATUS,PRIORITY_YN, FIRST_WORK)")
					_T("SELECT %s,'%s',ACQ_YEAR,WORK_CODE,'%s',CONTROL_MAT_CODE,MAT_CODE,FORM_CODE,SUB_REG_CODE,MEDIA_CODE,USE_OBJ_CODE,USE_LIMIT_CODE,LOC_FLAG,SUBJECT_CODE,CLASS_NO_TYPE,KDC_CLASS_NO,KDC_CLASS_NO_VER,DDC_CLASS_NO,DDC_CLASS_NO_VER,ECT_CLASS_NO,SHELF_LOC_CODE,TOC_COMMON_KEY,CENTER_UPLOAD_STATUS,REMARK,WORKER,INPUT_DATE,DUP_FLAG,APPLY_YN,CATALOG_STATUS,PRIORITY_YN, '종분리 일괄작업 20081204' FROM BO_SPECIES_TBL WHERE REC_KEY=%s;")
					, strNewRecKey, strManageCode, strControlNo, strRecKey );
				pMain->m_pDM_INDEX->AddFrame(strQuery);

				// 책테이블에서 MANAGE_CODE별로 SPECIES_KEY변경
				strQuery.Format(_T("UPDATE BO_BOOK_TBL SET SPECIES_KEY=%s WHERE SPECIES_KEY=%s AND MANAGE_CODE='%s';")
					, strNewRecKey, strRecKey, strManageCode );
				pMain->m_pDM_INDEX->AddFrame(strQuery);

				// 대표책 설정
				strQuery.Format(_T("UPDATE BO_BOOK_TBL BB SET REPRESENT_BOOK_YN='Y' WHERE REG_NO = ( SELECT MIN(REG_NO) FROM BO_BOOK_TBL BB2 WHERE BB2.SPECIES_KEY = BB.SPECIES_KEY AND BB2.SPECIES_KEY=%s);")
					, strNewRecKey);
				pMain->m_pDM_INDEX->AddFrame(strQuery);				
			}		

			pMain->m_pDM_INDEX->SendFrame(TRUE);
			pMain->m_pDM_INDEX->EndFrame();
			CString strWhere;
			CString strCallNo;
			CString strSeparateShelfCode;
			CString strClassNo;
			CString strBookCode;
			CString strVolCode;
			CString strCopyCode;
			CString strVolSortNo;
			CString str090Field;
			CString strNewSpeciesList;
			CString strStreamMarc;
			CString strBookKey;
			CString str001Field;
			CString strCurRecKey;
			CMarc	marc;

			strNewSpeciesList = _T("");
			for ( y=0 ; y<strArraySpeciesKeyList.GetSize() ; y++ )
			{
				strCurRecKey = strArraySpeciesKeyList.GetAt(y);	
				if ( !strNewSpeciesList.IsEmpty() ) strNewSpeciesList += _T(",");
				strNewSpeciesList += strCurRecKey;
			}	
			
			// 원본과 분리된 종 모두 조회;
			strWhere.Format(_T("REC_KEY IN ( %s ) ORDER BY REC_KEY"), strNewSpeciesList);
			pMain->m_pDM_SPECIES_TEMP->RefreshDataManager(strWhere);

			CString strTmpSpeciesKey;

			strStreamMarc = pMain->m_pDM_SPECIES_TEMP->GetCellData( _T("MARC"), 0);				
			
			pMain->m_pDM_INDEX->StartFrame();	
			pMain->m_pDM_INDEX->InitDBFieldData();	

			for ( y=0 ; y<strArraySpeciesKeyList.GetSize() ; y++ )
			{
				strCurRecKey = strArraySpeciesKeyList.GetAt(y);												
				strTmpSpeciesKey = pMain->m_pDM_SPECIES_TEMP->GetCellData( _T("REC_KEY"), y);
				strControlNo = pMain->m_pDM_SPECIES_TEMP->GetCellData( _T("CONTROL_NO"), y);
				ids = pMain->m_pMarcMgr->Decoding(strStreamMarc, &marc);
				if ( strTmpSpeciesKey != strCurRecKey ) 
				{
					AfxMessageBox(_T("Key꼬임"));
					return -1;
				}

				// 001 재구성
				if ( strOriControlNo != strControlNo )
				{
					str001Field.Format(_T("001%s"), strControlNo);
					pMain->m_pMarcMgr->DeleteField(&marc, _T("001"));
					pMain->m_pMarcMgr->InsertField(&marc, str001Field);					
				}

				// 책정보 구성
				// 책정보 VOL_SORT_NO 재부여
				strWhere.Format(_T("SPECIES_KEY=%s ORDER BY VOL_CODE, COPY_CODE, REC_KEY"), strCurRecKey);
				pMain->m_pDM_BOOK->RefreshDataManager(strWhere);

				// 마크 업데이트용 090 만들기
				strClassNo = pMain->m_pDM_BOOK->GetCellData("CLASS_NO",0);
				strBookCode = pMain->m_pDM_BOOK->GetCellData("BOOK_CODE",0);
				str090Field.Format(_T("090  %ca%s%cb%s"), SUBFIELD_CODE, strClassNo, SUBFIELD_CODE, strBookCode);
				
				// 049 만들기
				capApi.ConvertBookTo049(&marc, pMain->m_pDM_BOOK);


				// 책업데이트
				// VOL_SORT_NO
				for ( j=0 ; j<pMain->m_pDM_BOOK->GetRowCount() ; j++ )
				{
					strBookKey = pMain->m_pDM_BOOK->GetCellData(_T("REC_KEY"), j);
					strVolSortNo = pMain->m_pDM_BOOK->GetCellData(_T("VOL_SORT_NO"), j);
					strQuery.Format(_T("UPDATE BO_BOOK_TBL SET VOL_SORT_NO=%s WHERE REC_KEY=%s;"),strVolSortNo, strBookKey);
					pMain->m_pDM_INDEX->AddFrame(strQuery);
				}

				// 색인항목 업데이트
				strQuery.Format(_T("UPDATE IDX_BO_TBL SET IDX_HOLD_PLACE = GET_BO_MAIN_SHELF_CODE(%s), MAIN_PLACE_INFO = GET_BO_MAIN_SHELF_CODE(%s), REG_CODE = GET_BO_BOOK_INFO(%s, 'REG_CODE'), PLACE_INFO = GET_BO_BOOK_INFO(%s, 'SHELF_LOC_CODE'), IDX_VOL_TITLE = GET_BO_BOOK_INFO(%s, 'VOL_TITLE_INDEX'), IDX_VOL = GET_BO_BOOK_INFO(%s, 'VOL_INDEX'), IDX_SE_SHELF_CODE = GET_BO_BOOK_INFO(%s, 'SEPARATE_SHELF_CODE'), MANAGE_CODE = GET_BO_BOOK_INFO(%s, 'MANAGE_CODE'), USE_LIMIT_CODE = GET_BO_BOOK_INFO(%s, 'USE_LIMIT_CODE') WHERE REC_KEY = %s;")
					,strCurRecKey, strCurRecKey, strCurRecKey, strCurRecKey, strCurRecKey, strCurRecKey, strCurRecKey, strCurRecKey, strCurRecKey, strCurRecKey );
				pMain->m_pDM_INDEX->AddFrame(strQuery);

				// 090
				pMain->m_pMarcMgr->DeleteField(&marc, _T("090"));
				pMain->m_pMarcMgr->InsertField(&marc, str090Field);
				
				// Streammarc 추출
				pMain->m_pMarcMgr->Encoding(&marc, strStreamMarc);							
				
				// 종업데이트				
				pMain->m_pDM_INDEX->AddDBFieldData(_T("MARC"),  _T("STRING"), strStreamMarc);
				pMain->m_pDM_INDEX->MakeUpdateFrame(_T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), strCurRecKey);				
			}
			
			nIndex++;
			pMain->ShowStatus( nTotalCount, nIndex );

			ids = pMain->m_pDM_INDEX->SendFrame(TRUE);
			ids = pMain->m_pDM_INDEX->EndFrame();

			if ( pMain->m_bStop == TRUE ) 
			{
				pMain->m_bStop = FALSE;
				return TRUE;
			}
		} // END FOR : SPECIES		

		
		nFINISH = GetTickCount();

		pMain->RemainTimeDisplay(tStartTime, nFINISH-nSTART, nTotalCount, nIndex);
		pMain->WriteProcessInfo(strFileName, nIndex, strRecKey);
	}	

	pMain->m_bRunning = FALSE;
	pMain->m_bStop = FALSE;

	( (CButton*) pMain->GetDlgItem ( IDC_bClose ) )->SetWindowText(_T("닫기"));
	( (CButton*) pMain->GetDlgItem ( IDC_bClose ) )->EnableWindow(TRUE);

	AfxMessageBox(_T("완료되었습니다"));
	return TRUE;
}

VOID CSpeciesDivideMANnagerDlg::WriteProcessInfo(CString strFileName, INT nCount, CString strRecKey)
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

VOID CSpeciesDivideMANnagerDlg::RemainTimeDisplay(COleDateTime tStartTime, DWORD dTime, INT nTotalCount, INT nIndex)
{
	CString strRemainTime;
	float nRemainTime;
	float nCountTime;
	nCountTime = ((float)dTime/1000);	

	// 남은시간 구하기
	nRemainTime = nCountTime*((float)nTotalCount/(float)nIndex);	
	COleDateTimeSpan tSpen(0,0,0,(INT)nRemainTime);


	// 현재시간에 남은시간 더하기
	COleDateTime tend = tStartTime + tSpen;
	
	strRemainTime.Format(_T("%d일 %02d:%02d:%02d  "), tend.GetDay(), tend.GetHour(), tend.GetMinute(), tend.GetSecond() );
	( (CStatic*) GetDlgItem (IDC_sRemainTime) )->SetWindowText(strRemainTime);
}

void CSpeciesDivideMANnagerDlg::ShowStatus( INT nRowCount, INT iFinishedCnt )
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

void CSpeciesDivideMANnagerDlg::ShowTotalStatus( INT nRowCount, INT iFinishedCnt )
{
	CString frm;

    if ( iFinishedCnt == 0 ) {
        m_ProgressCtrl2.SetRange( 0, nRowCount );
        m_ProgressCtrl2.SetStep( 1 );
        m_ProgressCtrl2.SetPos( 0 );
    } 
	else 
	{
		frm.Format( _T("%d / %d"), iFinishedCnt, nRowCount );
		m_ProgressCtrl2.SetWindowText(frm);
        m_ProgressCtrl2.StepIt();		
    }    
}


void CSpeciesDivideMANnagerDlg::OnbStart() 
{
	if ( m_strConnectionInfo==_T("") )
	{
		AfxMessageBox(_T("접속정보 TEST를 먼저 수행하세요"));
		return;
	}	

	if ( AfxMessageBox(_T("변환의 모든 책임을 지신다면 수락!~~"), MB_YESNO) != IDYES )
	{
		return;
	}

	( (CButton*) GetDlgItem ( IDC_bClose ) )->SetWindowText(_T("중지"));

	AfxBeginThread(	BeginUpload_Thread, this);		
}

HBRUSH CSpeciesDivideMANnagerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}

void CSpeciesDivideMANnagerDlg::OnbtnCOUNTCHECK() 
{
	if ( m_strConnectionInfo==_T("") )
	{
		AfxMessageBox(_T("접속정보 TEST를 먼저 수행하세요"));
		return;
	}
	
	CString strValue;
	m_pDM_SPECIES->GetOneValueQuery(_T("SELECT /*+ INDEX_ASC(IDX_BO_TBL IDX_BO_TBL_PK) */ COUNT(1) FROM IDX_BO_TBL WHERE REC_KEY > 1 AND LENGTH(MANAGE_CODE) > 2"), strValue);	
	m_ProgressCtrl.SetWindowText(strValue + _T(" 건"));
	
}


VOID CSpeciesDivideMANnagerDlg::InitIndexMap()
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

void CSpeciesDivideMANnagerDlg::OnbConTest() 
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


	m_pDM_SPECIES->SetCONNECTION_INFO(m_strConnectionInfo);
	m_pDM_INDEX->SetCONNECTION_INFO(m_strConnectionInfo);
	m_pDM_BOOK->SetCONNECTION_INFO(m_strConnectionInfo);

	INT ids;
	ids = m_pDM_SPECIES->GetOneValueQuery(_T("SELECT LIB_NAME FROM MN_LIBINFO2_TBL"), strValue );
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



CString CSpeciesDivideMANnagerDlg::GetLastControlNo(CString strControlMatCode, CString strManageCode)
{	
	CString strQuery;
	CString strReckey;
	CString strControlNo;
	CString strYear;
	CString strItem;
	CString strStreamMarc;
	CString strSpeciesKey;
	CString sControlNo;


	INT iControlNoCount = 0;
	INT	ids;
	
	// 마지막 제어번호 가져오기	
	CTime time = CTime::GetCurrentTime();	
	strYear.Format( _T("%04d"), time.GetYear() );
	strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
					_T("WHERE KIND_CODE='CON_NO' AND MAKE_YEAR='%s' AND DIVISION_VALUE='%s' AND MANAGE_CODE='%s' FOR UPDATE")
					, strYear, strControlMatCode, strManageCode );
	ids = m_pDM_INDEX->GetOneValueQuery(strQuery, strItem);
	if (ids < 0)
	{
		strControlNo = _T("1");
		// 제어번호 중복체크		
		sControlNo.Format( _T("%s%s%05s"), strControlMatCode, strYear, strControlNo );
		strQuery.Format(_T("SELECT CONTROL_NO FROM BO_SPECIES_TBL ")
						_T("WHERE CONTROL_NO='%s' AND MANAGE_CODE='%s'"), sControlNo, strManageCode);
		ids = m_pDM_INDEX->GetOneValueQuery(strQuery, strItem);
		if ( strItem != _T("") ) {
			AfxMessageBox( _T("제어번호가 중복됩니다. 마지막번호를 수정하고 다시 부여하세요.") );
			return -1;
		}

		// 없으면 Insert
		ids = m_pDM_INDEX->MakeRecKey(strReckey);
		if (ids < 0) return ids;		
		
		strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ")
						_T("(REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, ")
						_T("LAST_NUMBER, FIRST_WORK, LAST_WORK, MANAGE_CODE) ")
						_T("VALUES ('%s','CO','CON_NO','%s','%s','%s', '%s', '%s', '%s');")
						, strReckey, strControlMatCode, strYear, strControlNo
						, GetWorkLogMsg(_T("Marc입력"),WORK_LOG)
						, GetWorkLogMsg(_T("Marc입력"),WORK_LOG), strManageCode );			
	}
	else
	{
		// 있으면 Update
		if ( iControlNoCount < 0 ) {
			iControlNoCount = 0;
		}		

		strControlNo.Format(_T("%d"), _ttoi(strItem) + 1 + iControlNoCount);		
		// 제어번호 중복체크		
		sControlNo.Format( _T("%s%s%05s"), strControlMatCode, strYear, strControlNo );
		strQuery.Format(_T("SELECT CONTROL_NO FROM BO_SPECIES_TBL ")
						_T("WHERE CONTROL_NO='%s' AND MANAGE_CODE='%s'"), sControlNo, strManageCode);
		ids = m_pDM_INDEX->GetOneValueQuery(strQuery, strItem);
		if ( strItem != _T("") ) {
			AfxMessageBox( _T("제어번호가 중복됩니다. 마지막번호를 수정하고 다시 부여하세요.") );
			return -1;
		}
		
		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%s, LAST_WORK='%s' ")
						_T("WHERE KIND_CODE='CON_NO' AND DIVISION_VALUE='%s' AND MAKE_YEAR='%s' AND MANAGE_CODE='%s';"), 
			strControlNo, GetWorkLogMsg(_T("Marc수정"),WORK_LOG), strControlMatCode, strYear, strManageCode);

		iControlNoCount++;
	}
	ids = m_pDM_INDEX->AddFrame(strQuery);	
	return sControlNo;
}

void CSpeciesDivideMANnagerDlg::OnbClose() 
{
	if ( m_bRunning )
	{
		( (CButton*) GetDlgItem ( IDC_bClose ) )->EnableWindow(FALSE);
		SetTimer(1, 1000, NULL);
		return;
	}

	CDialog::OnCancel();
}

void CSpeciesDivideMANnagerDlg::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == 1 )
	{
		if ( m_bStop == FALSE )
		{
			m_bRunning = FALSE;
			KillTimer(1);
			( (CButton*) GetDlgItem ( IDC_bClose ) )->SetWindowText(_T("닫기"));
			( (CButton*) GetDlgItem ( IDC_bClose ) )->EnableWindow(TRUE);
			( (CButton*) GetDlgItem ( IDC_bStart ) )->EnableWindow(FALSE);			
		}
	}
		
	CDialog::OnTimer(nIDEvent);
}
