// UploadMarcExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UploadMarcExportDlg.h"
#include <direct.h>

// DES 암복호화
#include "FWDES.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CUploadMarcExportDlg dialog

CUploadMarcExportDlg::CUploadMarcExportDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUploadMarcExportDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

	m_pParent = pParent;	
	m_pDM = NULL;
	m_strPath = _T("");
	m_CONNECTION_INFO = _T("");
	m_hIcon = NULL;

	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

VOID CUploadMarcExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUploadMarcExportDlg)	
	DDX_Control(pDX, IDC_prog, m_ProgressBar);
	//}}AFX_DATA_MAP
}

CUploadMarcExportDlg::~CUploadMarcExportDlg()
{
	if ( m_pDM ) 
	{
		delete m_pDM;
		m_pDM = NULL;
	}
}
BEGIN_MESSAGE_MAP(CUploadMarcExportDlg, CDialog)
	//{{AFX_MSG_MAP(CUploadMarcExportDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bSearch, OnbSearch)
	ON_BN_CLICKED(IDC_bPath, OnbPath)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUploadMarcExportDlg message handlers

BOOL CUploadMarcExportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	///////////////////////////////////////////////////////////////////////////////////
	// 여기부터 접속 정보 설정
		//++2008.09.29 UPDATE BY CJY {{++
	// 암호화된 접속정보를 복호화한 후 설정
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

	if (InitESL_Mgr(_T("SM_UploadMarcExport")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	SetLibGrid();
	SetControl();	
	
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_pDM = new CESL_DataMgr;
	m_pDM->SetCONNECTION_INFO(m_CONNECTION_INFO);	

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CUploadMarcExportDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

VOID CUploadMarcExportDlg::OnPaint() 
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

HCURSOR CUploadMarcExportDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CUploadMarcExportDlg::OnCancel() 
{
	CDialog::OnCancel();
}

VOID CUploadMarcExportDlg::OnOK() 
{
	//반출
	if ( !DoExport() )
	{
		return;
	}


	//CDialog::OnOK();
}

BOOL CUploadMarcExportDlg::DoExport()
{
	if ( !DoMakeFile() )	return FALSE;

	return TRUE;
}

BOOL CUploadMarcExportDlg::DoMakeDMFromDB( CString strManageCode )
{
	//입력된 Control 값 가져오기	
	if ( m_pDM == NULL ) return FALSE;
	m_pDM->FreeData();

	INT ids = -1;
	CString strManageCodeQuery;
	if ( strManageCode.IsEmpty() ) 
		strManageCodeQuery = _T("");
	else 
		strManageCodeQuery.Format( _T("AND MANAGE_CODE = '%s'") , strManageCode );

	// 2006-03-15 UPDATE BY PDW(JR) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 제어번호 조건을 코드와 번호부분으로 분리 
	CString strControlMatCode;
	CESL_ControlMgr *pCM = FindCM(_T("CM_UploadMarcExport"));
	pCM->GetControlMgrData(_T("제어자료구분"), strControlMatCode);
	strControlMatCode.TrimLeft();strControlMatCode.TrimRight();
	if ( strControlMatCode.IsEmpty() )
	{
		AfxMessageBox(_T("제어자료구분 조건이 선택되지 않았습니다."));
		return FALSE;
	}

	CString strControlNoStart;
	CString strControlNoEnd;
	GetDlgItem(IDC_eControlNoStart)->GetWindowText(strControlNoStart);
	GetDlgItem(IDC_eControlNoEnd)->GetWindowText(strControlNoEnd);

	if ( strControlNoStart.IsEmpty() && strControlNoStart.IsEmpty() )		 
	{
		AfxMessageBox(_T("제어번호 조건이 입력되지 않았습니다."));
		return FALSE;
	}
	
	// 제어자료구분 조건과 제어번호 조건을 합친다.
	if ( !strControlNoStart.IsEmpty() ) strControlNoStart = strControlMatCode + strControlNoStart;
	if ( !strControlNoEnd.IsEmpty() )   strControlNoEnd = strControlMatCode + strControlNoEnd;
	// -----------------------------------------------------------------------------------------------

	if ( (strControlNoStart.GetLength() > 1 && strControlNoStart.GetLength() < 12) ||
		 (strControlNoEnd.GetLength() > 1 && strControlNoEnd.GetLength() < 12) 	 )
	{
		AfxMessageBox(_T("제어번호 자릿수가 잘못되었습니다."));
		return FALSE;
	}

	// 2006-03-24 UPDT BY PDW(Jr) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 경로명 변경과 관련된 수정사항 조건절을 따로 분리했다.
	// 센터업로드 상태 조건 추가 됨 ( 반출후 국중에 제공된 자료 )
	CString strQuery = _T("select marc, rec_key from bo_species_tbl where "); 
	CString strCondition;
	if ( !strControlNoStart.IsEmpty() && !strControlNoEnd.IsEmpty() )
	{
		strCondition.Format( _T("( CENTER_UPLOAD_STATUS is NULL or ( CENTER_UPLOAD_STATUS != '0' and CENTER_UPLOAD_STATUS !=5 ) )")
							_T(" AND LOC_FLAG ='0' ")
							_T(" %s AND CONTROL_NO between '%s' AND  '%s' ")
							, strManageCodeQuery, strControlNoStart, strControlNoEnd );
	}
	else if (!strControlNoStart.IsEmpty())
	{
		strCondition.Format( _T("( CENTER_UPLOAD_STATUS is NULL or ( CENTER_UPLOAD_STATUS != '0' and CENTER_UPLOAD_STATUS !=5 ) )")
						     _T(" AND LOC_FLAG ='0' ")
							 _T(" %s AND CONTROL_NO between '%s' AND  '%s99999' ")
							, strManageCodeQuery, strControlNoStart, strControlNoStart.Left(7));
	}
	else
	{
		strCondition.Format( _T("( CENTER_UPLOAD_STATUS is NULL or ( CENTER_UPLOAD_STATUS != '0' and CENTER_UPLOAD_STATUS !=5 ) )")
							_T(" AND LOC_FLAG ='0' ")
							_T(" %s AND CONTROL_NO ='%s' ")
							, strManageCodeQuery, strControlNoEnd);
	}	

	strQuery += strCondition;

	CStdioFile file;
	if (!file.Open(_T("Query.txt"), CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) 
	{
		AfxMessageBox(_T("파일 생성에 실패했습니다."));
		return FALSE;
	}
		
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	file.WriteString(strQuery);
	file.Close();

	ids = m_pDM->RestructDataManager( strQuery );
	if ( ids < 0 ) return FALSE;
	
	SetExportPathFilename( strCondition, TRUE );

	return TRUE;

}

BOOL CUploadMarcExportDlg::DoMakeFile( )
{
	//반출 파일 생성
	CString strLibCode;
	CString strLibName;
	
	( (CEdit*) GetDlgItem(IDC_eLibcode) )->GetWindowText(strLibCode);
	( (CEdit*) GetDlgItem(IDC_eLibname) )->GetWindowText(strLibName);	

	CString strFileName;
	CString strFileNameExp;
	INT nFileNameExp = 1;
	CString strStreamMarc;
	INT nCheckCnt = 0;
	CStdioFile file;

	CString strPath;

	( (CEdit*) GetDlgItem(IDC_ePATH) )->GetWindowText(strPath);

	// 2006-03-22 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 파일 중복 체크
	TCHAR* cPath = new TCHAR[strPath.GetLength() + 1];
	_stprintf( cPath, _T("%s"), strPath);
	FILE* fp = _tfopen( cPath, _T("rb"));	
	if ( fp )
	{
		fclose( fp );
		if ( IDNO == AfxMessageBox( _T("    중복된 파일이 존재합니다.\r\n"
			                           _T("    기존 파일을 바꾸시겠습니까?\r\n")
			                           _T("(예 : 덮어쓰기 , 아니오 : 작업취소)")), MB_YESNO ) )
		{				
			delete [] cPath;
			return FALSE;
		}		
	}	
	
	delete [] cPath;
	// ----------------------------------------------------------------------------------------------------------

	if (!file.Open(strPath , CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) 
	{
		AfxMessageBox(_T("파일 생성에 실패했습니다."));
		return FALSE;
	}
	
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	if ( m_pDM->GetRowCount() < 1 ) return FALSE;

	//프로그래스바
	SetProgressBar ();

	CString strCount;
	CString strQuery;


	for ( INT nIdx = 0 ; nIdx < m_pDM->GetRowCount() ; nIdx++ )
	{
		nCheckCnt++;
		m_ProgressBar.StepIt();

		
		INT nCount = nIdx;
		strCount.Format(_T("%d"), nIdx );
		GetDlgItem(IDC_sCount)->SetWindowText( strCount );

		m_pDM->GetCellData( nIdx , 0, strStreamMarc );
		strStreamMarc += _T("\r\n");
		file.WriteString(strStreamMarc);
		
		if ( nCheckCnt == 10000 ) 		
		{
			nCheckCnt = 0 ;
			file.Close();

			nFileNameExp++;
			strFileNameExp.Format(_T(".%03d"), nFileNameExp );
			strFileName = strPath.Left(strPath.GetLength()-4)+strFileNameExp+ _T(".txt");

			if (!file.Open(strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) 
			{
				AfxMessageBox(_T("파일 생성에 실패했습니다."));
				return FALSE;
			}
			
			if( 0 == file.GetLength() )
			{			
				TCHAR cUni = 0xFEFF;
				file.Write( &cUni, sizeof(TCHAR));
			}

		}
	}

	strCount.Format(_T("%d"), m_pDM->GetRowCount() );
	GetDlgItem(IDC_sCount)->SetWindowText( strCount );
	file.Close();

	BOOL bCheck;

	bCheck = ( (CButton*) GetDlgItem(IDC_chkChangeStatus) )->GetCheck();

	if ( bCheck )
	{
		m_ProgressBar.SetPos(0);
		for ( nIdx = 0 ; nIdx < m_pDM->GetRowCount() ; nIdx++ )
		{		
			strCount.Format(_T("%d"), nIdx );
			GetDlgItem(IDC_sCount)->SetWindowText( strCount );
			
			m_ProgressBar.StepIt();
			strQuery.Format(_T(" UPDATE BO_SPECIES_TBL SET CENTER_UPLOAD_STATUS = '6' WHERE REC_KEY = '%s'; "), m_pDM->GetCellData( nIdx , 1 ) );
			m_pDM->ExecuteQuery( strQuery, 1 );
		}
	}

	// 반출 성공 path 저장
	CWinApp *pApp = AfxGetApp();
	pApp->WriteProfileString(_T("UploadExportMarcFilePath"), _T("PATH"), m_strPath);

	// 성공 메시지
	CString strOutputMsg;
	strOutputMsg.Format(_T("%s \r\n폴더에 정상적으로 반출되었습니다."), strPath);

	AfxMessageBox(strOutputMsg);
	return TRUE;
}

VOID CUploadMarcExportDlg::SetControl()
{

	((CEdit*)GetDlgItem(IDC_eControlNoStart))->SetLimitText(9);
	((CEdit*)GetDlgItem(IDC_eControlNoEnd))->SetLimitText(9);

	( (CButton*) GetDlgItem(IDC_chkChangeStatus) )->SetCheck(TRUE);	

	// 반출 Path	
	CWinApp *pApp = AfxGetApp();
	
	m_strPath = pApp->GetProfileString(_T("UploadExportMarcFilePath"), _T("PATH"));	
	TCHAR sCurrentDirectory[512];
	if ( m_strPath.IsEmpty() )
	{		
		_tgetcwd( sCurrentDirectory, _MAX_PATH ) ;
		m_strPath.Format(_T("%s\\"), sCurrentDirectory);
	}

	((CEdit*) GetDlgItem(IDC_ePATH))->SetWindowText(m_strPath);
	
}

VOID CUploadMarcExportDlg::SetLibGrid()
{
	// 이용자 정보를 가져온다.
	CString strUserID;
	CString strQuery;
	CString strManageCode;
	CString strDate;

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_CONNECTION_INFO);

	strQuery.Format(_T("SELECT TO_CHAR(SYSDATE,'YYYY') FROM DUAL"));
	dm.GetOneValueQuery(strQuery, strManageCode);

	strUserID = m_pParent->GetUserID();
	strQuery.Format(_T("SELECT MANAGE_CODE FROM MN_USER_TBL WHERE ID='%s'"), strUserID );
	dm.GetOneValueQuery(strQuery, strManageCode);

	if ( strManageCode.IsEmpty() )
	{
		strQuery.Format(_T("WHERE MANAGE_CODE IS NULL"));
	}
	else
	{
		strQuery.Format(_T("WHERE MANAGE_CODE='%s'"), strManageCode);
	}	

	strQuery.Format(_T("select MANAGE_CODE, LIB_CODE, LIB_NAME from MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s'"), strManageCode);
	dm.RestructDataManager( strQuery );	
	
	if ( !dm.GetRowCount() )	return;
	// ----------------------------------------------------------------------------------------------------------------------

	( (CEdit*) GetDlgItem (IDC_eManageCode ) )->SetWindowText(dm.GetCellData( 0, 0 ));
	( (CEdit*) GetDlgItem (IDC_eLibcode ) )->SetWindowText(dm.GetCellData( 0, 1 ));
	( (CEdit*) GetDlgItem (IDC_eLibname ) )->SetWindowText(dm.GetCellData( 0, 2 ));	
}

BEGIN_EVENTSINK_MAP(CUploadMarcExportDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CUploadMarcExportDlg)	
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CUploadMarcExportDlg::OnbSearch() 
{
	if ( !DoSearch () ) return ;
}

BOOL CUploadMarcExportDlg::DoSearch()
{	
	CString strManageCode;

	( (CEdit*) GetDlgItem(IDC_eManageCode) )->GetWindowText(strManageCode);

	//DM 만들기
	if ( !DoMakeDMFromDB( strManageCode ) )	return FALSE;

	//총 갯수 셋팅
	CString strTmp;
	strTmp.Format(_T("%d"), m_pDM->GetRowCount() );
	GetDlgItem(IDC_sTotalCount)->SetWindowText(strTmp);
	GetDlgItem(IDC_sCount)->SetWindowText(_T("0"));

	CString strPath = _T("");

	if ( m_pDM->GetRowCount() < 1) 
	{
		AfxMessageBox(_T("검색된 결과가 없습니다."));
		GetDlgItem(IDOK)->EnableWindow(FALSE);		
	}
	else
	{
		CString strMessage;
		strMessage.Format(_T("전체  [  %d  ]  건의 자료가 검색되었습니다.. "), m_pDM->GetRowCount() );
		AfxMessageBox ( strMessage ) ;
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
		
	return TRUE;
}

INT CUploadMarcExportDlg::SetProgressBar()
{
	// 상태바 설정
	m_ProgressBar.SetRange(0, m_pDM->GetRowCount() );
	m_ProgressBar.SetPos(0);
	m_ProgressBar.SetStep(1);

	return 0;
}



VOID CUploadMarcExportDlg::OnbPath() 
{
	TCHAR sCurrentDirectory[512];

	CFileFind finder;
	

	_tgetcwd( sCurrentDirectory, _MAX_PATH ) ;

	CFileDialog pDlg( FALSE, _T("txt"), _T("*.txt") ,
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						_T("파일형식(*.txt)||") );
	if ( pDlg.DoModal() == IDOK )
	{
		m_strPath = pDlg.GetPathName();
		((CEdit*) GetDlgItem(IDC_ePATH))->SetWindowText(m_strPath);
		m_strPath = m_strPath.Left(m_strPath.GetLength()-pDlg.GetFileName().GetLength());	
	}
	_tchdir( sCurrentDirectory ) ;
	
}

INT CUploadMarcExportDlg::GetExportPathFilename(CString &strGetPathFilename)
{	

	if ( m_strPath.IsEmpty() )
	{		
		CWinApp *pApp = AfxGetApp();
	
		m_strPath = pApp->GetProfileString(_T("UploadExportMarcFilePath"), _T("PATH"));	
		TCHAR sCurrentDirectory[512];
		if ( m_strPath.IsEmpty() )
		{		
			_tgetcwd( sCurrentDirectory, _MAX_PATH ) ;
			m_strPath.Format(_T("%s\\"), sCurrentDirectory);
		}
	}


	strGetPathFilename = m_strPath;

	return 0;
}


INT CUploadMarcExportDlg::SetExportPathFilename(CString strCondition, BOOL bSearch)
{
	// ==========================================================================================================
	// 1. 경로명을 구한다.
	// ==========================================================================================================
	CString strPath;
	GetExportPathFilename( strPath );

	// ==========================================================================================================
	// 2. DM에 로우가 존재한다면 파일명이 생성된다.
	// ==========================================================================================================
	if ( m_pDM && bSearch )
	{
		if ( m_pDM->GetRowCount() )
		{
			CString strQuery;
			strQuery.Format( _T("Select min(control_no)||max(control_no) from bo_species_tbl where %s"), strCondition );

			CString strResult;
			if ( m_pDM->GetOneValueQuery( strQuery, strResult ) < 0 )	return -1;
			
			CString strLibCode, strLibName;
			((CEdit*) GetDlgItem (IDC_eLibcode))->GetWindowText(strLibCode);
			((CEdit*) GetDlgItem (IDC_eLibname))->GetWindowText(strLibName);	

			strPath.Format( _T("%s%s_%s(%s~%s %d건).TXT"), strPath, strLibCode, strLibName, 
				strResult.Left(12), strResult.Right(12), m_pDM->GetRowCount() );
		}
	}
	
	// ==========================================================================================================
	// 3. 컨트롤에 입력한다.
	// ==========================================================================================================
	((CEdit*) GetDlgItem(IDC_ePATH))->SetWindowText(strPath);

	return 0;
}

HBRUSH CUploadMarcExportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;

}
