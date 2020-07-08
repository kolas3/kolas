// LoanFromExcelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoanFromExcel.h"
#include "LoanFromExcelDlg.h"
#include <direct.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LOAN_SUCCESS_LOG _T("대출성공")
#define RETURN_SUCCESS_LOG _T("반납성공")

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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanFromExcelDlg dialog

CLoanFromExcelDlg::CLoanFromExcelDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoanFromExcelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CLoanFromExcelDlg::~CLoanFromExcelDlg()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CLoanFromExcelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanFromExcelDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoanFromExcelDlg, CDialog)
	//{{AFX_MSG_MAP(CLoanFromExcelDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btnLOAN_FILE_SEARCH, OnbtnLOANFILESEARCH)
	ON_BN_CLICKED(IDC_btnRETURN_FILE_SEARCH, OnbtnRETURNFILESEARCH)
	ON_BN_CLICKED(IDC_btnINIT, OnbtnINIT)
	ON_BN_CLICKED(IDC_btnRUN, OnbtnRUN)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabRESULT, OnSelchangetabRESULT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanFromExcelDlg message handlers

BOOL CLoanFromExcelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 
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
	InitLoanReturnDM();
	InitTabCtrl();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CLoanFromExcelDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

VOID CLoanFromExcelDlg::OnPaint() 
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
HCURSOR CLoanFromExcelDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CLoanFromExcelDlg::OnbtnLOANFILESEARCH() 
{
	SelectFile(0);
}

VOID CLoanFromExcelDlg::OnbtnRETURNFILESEARCH() 
{
	SelectFile(1);
}


/// 파일 선택
/// nType = 0 -> 대출한 파일
/// nType = 1 -> 반납한 파일
INT CLoanFromExcelDlg::SelectFile( INT nType )
{
	CString sFileName;
	TCHAR    sCurrentDirectory[300];
	
	//  현 디렉토리를 저장한다.
    _tgetcwd( sCurrentDirectory, 300 ) ;
		
    CFileDialog dlg(TRUE, _T("*.xls"), _T("*.xls"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("파일형식XLS Files(*.xls)|*.xls||") );
    if ( IDOK != dlg.DoModal() ) return 0;
	
	// 원래 디렉토리로 복귀
	_tchdir( sCurrentDirectory ) ;
	
    // 파일명이 선택되지않고 '확인'을 click했을 경우.
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return -1;

    sFileName = dlg.GetPathName();	

	if( nType == 0 )
	{
		GetDlgItem(IDC_edtLOAN_FILE_NAME)->SetWindowText(sFileName);
	}
	else
	{
		GetDlgItem(IDC_edtRETURN_FILE_NAME)->SetWindowText(sFileName);		
	}

	return 0;
}

VOID CLoanFromExcelDlg::OnbtnINIT() 
{
	InitLoanReturnDM();
	InitCtrl();
	ViewGrid();
}

INT CLoanFromExcelDlg::InitCtrl()
{
	GetDlgItem(IDC_edtLOAN_FILE_NAME)->SetWindowText(_T(""));
	GetDlgItem(IDC_edtRETURN_FILE_NAME)->SetWindowText(_T(""));
	GetDlgItem(IDC_edtPROC_DATE)->SetWindowText(_T(""));

	return 0;
}

VOID CLoanFromExcelDlg::OnbtnRUN() 
{
	AfxGetApp()->DoWaitCursor(1);

	
	INT ids = CheckRun();	
	if( ids != 0 )
		return ;


	ids = RunProc();
	if( ids != 0 )
		return ;

	ViewGrid();

	ExportExcelFile();

	AfxMessageBox(_T("처리되었습니다."));


	AfxGetApp()->DoWaitCursor(-1);

}


INT CLoanFromExcelDlg::CheckRun()
{
	INT ids;

	/// 1. 파일 이름이 있는지 검사
	CString sLoanFileName , sReturnFileName;
	GetDlgItem(IDC_edtLOAN_FILE_NAME)->GetWindowText(sLoanFileName);
	GetDlgItem(IDC_edtRETURN_FILE_NAME)->GetWindowText(sReturnFileName);
	
	if( sLoanFileName.IsEmpty() && sReturnFileName.IsEmpty() )
	{
		AfxMessageBox(_T("파일을 선택해주십시오"));
		return 1;
	}

	/// 2. 내용 구축
	InitLoanReturnDM();
	if( !sLoanFileName.IsEmpty() )
		ids = MakeLoanDMFromExcelFile(sLoanFileName);
	if( !sReturnFileName.IsEmpty() )
		ids = MakeReturnDMFromExcelFile(sReturnFileName);

	if( ids != 0 ) return 3;

	/// 3. 내용이 존재하는지 검사
	if( m_LoanDM.GetRowCount() == 0 && m_ReturnDM.GetRowCount() == 0 ) 
	{
		AfxMessageBox( _T("파일에 처리할 자료가 존재하지 않습니다.") );
		return 2;
	}
	

	return 0;
}

INT CLoanFromExcelDlg::RunProc()
{
	CString sProcDate,sLoanPeriod;
	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )GetDlgItem( IDC_edtPROC_DATE );
	CTime tTime;
	DWORD dwResult = pDate->GetTime( tTime );		
	sProcDate.Format( _T("%04d/%02d/%02d"), tTime.GetYear(), tTime.GetMonth(), tTime.GetDay() );
	
	GetDlgItem(IDC_edtLOANPERIOD)->GetWindowText(sLoanPeriod);
	
	CLoanReturnMgr LoanReturnMgr(this , sProcDate , sLoanPeriod );
	INT ids = LoanReturnMgr.InitInfo();
	if( ids != 0 ) return 1;

	CESL_DataMgr OracleDM;
	OracleDM.SetCONNECTION_INFO(_T("kolas2@k2up/kolas2#[]"));

	/// 대출 /반납을 동시에 처리하기
	/// 대출부터 반납까지...

	CProgressCtrl* pPrg = (CProgressCtrl*)GetDlgItem(IDC_prgLOANRETURN);
	pPrg->SetRange(0,m_LoanDM.GetRowCount()+m_ReturnDM.GetRowCount() );

	CString sProcStatus,sRegNo;
	INT nPos = 1;
	for( INT i = 0 ; i < m_LoanDM.GetRowCount() ; i++ )
	{
		sProcStatus.Empty();
		m_LoanDM.GetCellData(_T("처리상태"),i,sProcStatus);
		m_LoanDM.GetCellData(_T("등록번호"),i,sRegNo);

		pPrg->SetPos(nPos);
		nPos++;
		if( !sProcStatus.IsEmpty() ) continue;

		ids = LoanReturnBook( sRegNo , &OracleDM ,  &LoanReturnMgr );
		if( ids == 4007 ) 
			m_LoanDM.SetCellData( _T("처리상태") , _T("DB에 자료가 존재하지 않습니다.") , i );
		

	}

	for( i = 0 ; i < m_ReturnDM.GetRowCount() ; i++ )
	{
		sProcStatus.Empty();
		m_ReturnDM.GetCellData(_T("처리상태"),i,sProcStatus);
		m_ReturnDM.GetCellData(_T("등록번호"),i,sRegNo);
		
		pPrg->SetPos(nPos);
		nPos++;
		if( !sProcStatus.IsEmpty() ) continue;
		
		ids = LoanReturnBook( sRegNo , &OracleDM , &LoanReturnMgr );
		if( ids == 4007 ) 
			m_ReturnDM.SetCellData( _T("처리상태") , _T("DB에 자료가 존재하지 않습니다.") , i );
		
	}



	return 0;
}

INT CLoanFromExcelDlg::InitLoanReturnDM()
{
	m_LoanDM.FreeData();
	m_ReturnDM.FreeData();

	m_LoanDM.InitDataMgrRef(3);
	m_LoanDM.SetDataMgrRef( 0 , _T("등록번호") , _T("") , _T("") , _T("") , _T("") );
	m_LoanDM.SetDataMgrRef( 1 , _T("이용자번호") , _T("") , _T("") , _T("") , _T("") );
	m_LoanDM.SetDataMgrRef( 2 , _T("처리상태") , _T("") , _T("") , _T("") , _T("") );
	
	m_ReturnDM.InitDataMgrRef(3);
	m_ReturnDM.SetDataMgrRef( 0 , _T("등록번호") , _T("") , _T("") , _T("") , _T("") );
	m_ReturnDM.SetDataMgrRef( 1 , _T("이용자번호") , _T("") , _T("") , _T("") , _T("") );
	m_ReturnDM.SetDataMgrRef( 2 , _T("처리상태") , _T("") , _T("") , _T("") , _T("") );
	
	return 0;
}

INT CLoanFromExcelDlg::MakeLoanDMFromExcelFile( CString sFileName )
{
	INT ids;
	ids = MakeDMFromExcelFile( sFileName , &m_LoanDM );
	return ids;
}


INT CLoanFromExcelDlg::MakeReturnDMFromExcelFile( CString sFileName )
{
	INT ids;
	
	ids = MakeDMFromExcelFile( sFileName , &m_ReturnDM );
	return ids;
}

INT CLoanFromExcelDlg::MakeDMFromExcelFile(CString sFileName , CESL_DataMgr *pDM)
{
	INT ids;
	// 1. 파일을 오픈한다.
	C_ExcelManControl ExcelCtrl;
	CRect rect;
	GetWindowRect(rect);
	
	
	CString strLastError;
	ExcelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rect, this, WM_USER + 10000);
	if (ExcelCtrl.m_hWnd == NULL) {
		strLastError = _T("Excelmanager.ocx가 설치되어 있지 않습니다. 설치하여 주십시오.");
		AfxMessageBox( strLastError);
		return 1;
	}
    CExcelManager ExcelMgr;
	ExcelMgr.SetManager(&ExcelCtrl);
	
    if(ExcelMgr.OpenManager() == FALSE) 
	{
        strLastError = _T("NOT Installed Excelmanager.ocx or No Excel!!!");
		AfxMessageBox( strLastError);
		return 2;
	}
	
	ExcelMgr.SetUseTemplate(TRUE, sFileName);
	if (ExcelMgr.OpenManager() == FALSE)
	{
		strLastError = _T("Open Failed");
		AfxMessageBox( strLastError);
		return 3;
	}
	
	// 3. 한라인씩 읽어가며 DM에 INSERT한다.	
	/// 0번째 컬럼은 등록번호 , 1번째 컬럼은 이용자번호로 정의한다.
	INT nColCount = 2;
	CString strItem;
	BOOL bStop = FALSE;
	BOOL bRowEmpty;	// 행전체가 비어 있는지 검사
	// 엑셀 파일의 끝은 행 전체가 비어 있거나 #EOF가 존재하는 행이다.
	while(TRUE)
	{	
		pDM->InsertRow(-1);
		bRowEmpty = TRUE;
		for( INT i = 0 ; i < nColCount ; i++ )
		{
			strItem = ExcelMgr.GetTextMatrix(pDM->GetRowCount() , i + 1);
			strItem.TrimLeft(); strItem.TrimRight();
			if( strItem.Compare(_T("#_TEOF"))==0)
			{
				bStop = TRUE;
				i = nColCount;
			}
			
			
			if( !bStop )
			{
				if( !strItem.IsEmpty() )
					bRowEmpty = FALSE;

				if( i == 0 && strItem.GetLength() > 3 )
				{
					ConvertRegNo(strItem);
				}
				
				ids = pDM->SetCellData( pDM->GetRowCount()-1 , i  , strItem  );
				if( ids < 0 ) return -1;
			}
		}
		if( bRowEmpty )
			bStop = TRUE;
		if( bStop ) break;
	}
	pDM->DeleteRow( pDM->GetRowCount()-1 );
	
	ExcelMgr.CloseManager();
	
	return 0;
}

INT CLoanFromExcelDlg::ConvertRegNo( CString &strItem )
{
	CString sPreRegNo = strItem;
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	strItem.Format( _T("%s%010d"),sPreRegNo.Mid(0,2) , _ttoi(sPreRegNo.Mid(2)) );

	strItem = m_pInfo->MakeRegNo(sPreRegNo.Left(nRegCodeLength) , _ttoi(sPreRegNo.Mid(nRegCodeLength)) );
	
	//=====================================================
	

	strItem.MakeUpper();

	return 0;
}

INT CLoanFromExcelDlg::LoanReturnBook( CString sRegNo , CESL_DataMgr* pOracleDM , CLoanReturnMgr* pLoanReturnMgr )
{
	INT ids;

	CString sGetOneValueQuery,sWorkingStatus;
	CString sProcStatus;
	/// 자료 상태를 알아보기
	sGetOneValueQuery.Format(_T("SELECT WORKING_STATUS FROM BO_BOOK_TBL WHERE REG_NO='%s'"),sRegNo );
	
	ids = pOracleDM->GetOneValueQuery(sGetOneValueQuery,sWorkingStatus);
	if( ids < 0 && ids != -4007 ) return 1;

	if( ids == -4007 ) return 4007;

	CString sCRegNo,sUserNo;
	INT nType = CanLoan(sWorkingStatus);
	CString sErrorMsg;
	if(  nType == 0 )
	{
		for( INT i = 0 ; i < m_LoanDM.GetRowCount() ; i++ )
		{
			sProcStatus.Empty();
			m_LoanDM.GetCellData(_T("처리상태"),i,sProcStatus);
			m_LoanDM.GetCellData(_T("등록번호"),i,sCRegNo);
			m_LoanDM.GetCellData(_T("이용자번호"),i,sUserNo);
			
			if( !sProcStatus.IsEmpty() ) continue;
			if( sRegNo.Compare(sCRegNo) != 0 ) continue;

			ids = pLoanReturnMgr->LoanBook( sUserNo , sRegNo , sErrorMsg);
			if( ids == 0 )
			{
				m_LoanDM.SetCellData( _T("처리상태") , LOAN_SUCCESS_LOG , i );
			}
			else
				m_LoanDM.SetCellData( _T("처리상태") , sErrorMsg , i );

			ids = LoanReturnBook( sRegNo , pOracleDM , pLoanReturnMgr);
			if( ids == 4007 ) 
				m_LoanDM.SetCellData( _T("처리상태") , _T("DB에 자료가 존재하지 않습니다.") , i );
			break;
		}
	}
	else if( nType == 1 )
	{
		for( INT i = 0 ; i < m_ReturnDM.GetRowCount() ; i++ )
		{
			sProcStatus.Empty();
			m_ReturnDM.GetCellData(_T("처리상태"),i,sProcStatus);
			m_ReturnDM.GetCellData(_T("등록번호"),i,sCRegNo);
			
			if( !sProcStatus.IsEmpty() ) continue;
			if( sRegNo.Compare(sCRegNo) != 0 ) continue;
			
			ids = pLoanReturnMgr->ReturnBook( sRegNo , sErrorMsg );
			if( ids == 0 )
			{
				m_ReturnDM.SetCellData( _T("처리상태") , RETURN_SUCCESS_LOG , i );
			}
			else
				m_ReturnDM.SetCellData( _T("처리상태") , sErrorMsg , i );
			ids = LoanReturnBook( sRegNo , pOracleDM , pLoanReturnMgr);
			if( ids == 4007 ) 
				m_ReturnDM.SetCellData( _T("처리상태") , _T("DB에 자료가 존재하지 않습니다.") , i );
			break;
		}

	}
	else
	{
		for( INT i = 0 ; i < m_LoanDM.GetRowCount() ; i++ )
		{
			sProcStatus.Empty();
			m_LoanDM.GetCellData(_T("처리상태"),i,sProcStatus);
			m_LoanDM.GetCellData(_T("등록번호"),i,sCRegNo);
			m_LoanDM.GetCellData(_T("이용자번호"),i,sUserNo);
			if( !sProcStatus.IsEmpty() ) continue;
			if( sRegNo.Compare(sCRegNo) != 0 ) continue;
			m_LoanDM.SetCellData( _T("처리상태") , _T("DB에 존재하지 않는 자료입니다") , i );
		}
		for( i = 0 ; i < m_ReturnDM.GetRowCount() ; i++ )
		{
			sProcStatus.Empty();
			m_LoanDM.GetCellData(_T("처리상태"),i,sProcStatus);
			m_LoanDM.GetCellData(_T("등록번호"),i,sCRegNo);
			m_LoanDM.GetCellData(_T("이용자번호"),i,sUserNo);
			if( !sProcStatus.IsEmpty() ) continue;
			if( sRegNo.Compare(sCRegNo) != 0 ) continue;
			m_ReturnDM.SetCellData( _T("처리상태") , _T("DB에 존재하지 않는 자료입니다") , i );
		}
	}

	return 0;
}

INT CLoanFromExcelDlg::CanLoan( CString sWorkingStatus )
{
	if( sWorkingStatus.Compare(_T("BOL112N")) == 0 )
		return 0;
	else if( sWorkingStatus.Compare(_T("BOL211O")) == 0 || 
		//=====================================================
		//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
		// 18.09.27 JMJ KOL.RED.2018.008
		sWorkingStatus.Compare(_T("BOL411O")) == 0 || 
		sWorkingStatus.Compare(_T("BOL511O")) == 0 || 
		// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
		sWorkingStatus.Compare(_T("BOL611O")) == 0 || 
		//=====================================================
		sWorkingStatus.Compare(_T("BOL212O")) == 0 || 
		sWorkingStatus.Compare(_T("BOL213O")) == 0 || 
		sWorkingStatus.Compare(_T("BOL214O")) == 0 || 
		sWorkingStatus.Compare(_T("BOL215O")) == 0 
		)
		return 1;

	return 2;
}

INT CLoanFromExcelDlg::InitTabCtrl()
{
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabRESULT);


	pTab->InsertItem( 0 , _T("대출") );
	pTab->InsertItem( 1 , _T("반납") );
	pTab->InsertItem( 2 , _T("대출오류로그") );
	pTab->InsertItem( 3 , _T("반납오류로그") );
	
	return 0;
}



INT CLoanFromExcelDlg::ViewGrid()
{
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabRESULT);

	INT nIndex = pTab->GetCurSel();

	CESL_DataMgr* pDM = NULL;
	if( nIndex == 0 || nIndex == 2 )
		pDM = &m_LoanDM;
	else if( nIndex == 1 || nIndex == 3)
		pDM = &m_ReturnDM;

	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN);

	INT nCnt = pDM->GetRowCount()+1;
	if( pDM->GetRowCount() == 0 )
		nCnt = 2;
	pGrid->Clear();
	pGrid->SetRows(nCnt);

	CString sValue;
	INT nInsertIndex = 0;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		sValue = pDM->GetCellData( i , 2 );

		if( nIndex > 1 && ( sValue.Compare(LOAN_SUCCESS_LOG)==0 || sValue.Compare(RETURN_SUCCESS_LOG ) == 0 ) ) continue;
		
		for( INT j = 0 ; j < 4 ; j++ )
		{
			if( j == 0 )
				sValue.Format( _T("%d") , nInsertIndex+1 );
			else
				sValue = pDM->GetCellData( i , j-1 );

			pGrid->SetTextMatrix( nInsertIndex+1 , j , sValue );
		}

		nInsertIndex++;
	}

	pGrid->SetTextMatrix( 0 , 1 , _T("등록번호") );
	pGrid->SetTextMatrix( 0 , 2 , _T("이용자번호") );
	pGrid->SetTextMatrix( 0 , 3 , _T("처리상태") );

	return 0;
}

VOID CLoanFromExcelDlg::OnSelchangetabRESULT(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ViewGrid();
	
	*pResult = 0;
}


INT CLoanFromExcelDlg::ExportExcelFile()
{
	CString sLoanFailFileName;
	CString sReturnFailFileName;

	sLoanFailFileName.Format( _T("[%s]대출실패.xls") , CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S")) );
	sReturnFailFileName.Format( _T("[%s]반납실패.xls") , CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S")) );
	
	MakeFile( 0 , sLoanFailFileName );
	MakeFile( 1 , sReturnFailFileName );
	
	return 0;
}

INT CLoanFromExcelDlg::MakeFile( INT nType , CString sFileName )
{
	
	TCHAR TmpDir[500];
	GetCurrentDirectory(500,TmpDir);
	CRect rect;
	GetClientRect(&rect);
	
	// 파일 오픈
    C_ExcelManControl ExcelCtrl;
	ExcelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rect, this, 8805);
	if (ExcelCtrl.m_hWnd == NULL) {
		MessageBox(_T("Excelmanager.ocx가 설치되어 있지 않습니다. 설치하여 주십시오."), _T("Warnning!"), MB_ICONEXCLAMATION);
		return 1;
	}
    CExcelManager ExcelManager(&ExcelCtrl);
    if(ExcelManager.OpenManager() == FALSE) {
        MessageBox(_T("NOT Installed Excelmanager.ocx or No Excel!!!"), _T("Warnning!"), MB_ICONEXCLAMATION);
		return 2;
	}
	
	// 파일 입력
	CESL_DataMgr* pDM = NULL;
	if( nType == 0 )
		pDM = &m_LoanDM;
	else if( nType == 1)
		pDM = &m_ReturnDM;
		
	CString sValue;
	INT nInsertIndex = 0;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		sValue = pDM->GetCellData( i , 2 );
		
		if( sValue.Compare(LOAN_SUCCESS_LOG)==0 || sValue.Compare(RETURN_SUCCESS_LOG ) == 0 ) continue;
		
		for( INT j = 0 ; j < 3 ; j++ )
		{
			sValue = pDM->GetCellData( i , j );
			
			ExcelManager.SetTextMatrix( nInsertIndex+1 , j+1 , sValue );		
		}
		
		nInsertIndex++;
	}

	CString sFullPath;
	sFullPath.Format(_T("%s\\%s"),TmpDir,sFileName);
	
	if( nInsertIndex > 0 )
		ExcelManager.SaveAs(sFullPath);
	
	// 사용한 엑셀 객체를 반환한다.
    ExcelManager.CloseManager();
	
	SetCurrentDirectory(TmpDir);

	if( nInsertIndex != 0 )
	{
		CString sMsg;
		if( nType == 0 )
			sMsg.Format(_T("대출시 실패한 로그가 '%s'파일에 저장되었습니다.") , sFileName );
		else
			sMsg.Format(_T("반납시 실패한 로그가 '%s'파일에 저장되었습니다.") , sFileName );
		
		AfxMessageBox(sMsg);

	}

	
	return 0;
}

HBRUSH CLoanFromExcelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// 여기까지 추가

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
