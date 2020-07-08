#include "stdafx.h"
#include "KL_STATEMENT_IMAGE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CKL_STATEMENT_IMAGE::CKL_STATEMENT_IMAGE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKL_STATEMENT_IMAGE::IDD, pParent)
{
	m_nCurIdx = -1;
	m_sImageFilePath = _T("");
	m_strFuncName = _T("");
	m_strDMAlias = _T("");
	m_pBmp = NULL;
	
	m_strTmpUserKey = _T("");
}

void CKL_STATEMENT_IMAGE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_STATEMENT_IMAGE) 
	//}}AFX_DATA_MAP
	
}
CKL_STATEMENT_IMAGE::~CKL_STATEMENT_IMAGE()
{
	if(NULL != m_pBmp)
	{
		delete m_pBmp;
		m_pBmp = NULL;
	}	
}

BEGIN_MESSAGE_MAP(CKL_STATEMENT_IMAGE, CDialog)
	//{{AFX_MSG_MAP(CKL_STATEMENT_IMAGE)
	ON_BN_CLICKED(IDC_UPLOAD_FILE, OnUploadFile)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CKL_STATEMENT_IMAGE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(0 > InitESL_Mgr("SM_SCANFILE"))
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}	

	m_strDMAlias = _T("DM_SCANFILE");
	
	if ( 0 > SearchFileList() ) {
		AfxMessageBox(_T("SearchFileList Error!"));
		return FALSE;
	}
	
	return TRUE;  
	              
}

void CKL_STATEMENT_IMAGE::SetUserKey(CString strUserKey)
{
	m_strUserKey = strUserKey;
}

INT CKL_STATEMENT_IMAGE::SearchFileList()
{
	CString strWhere = _T(""), strQuery = _T("");
	CESL_DataMgr* pDM = FindDM(m_strDMAlias);
	if(NULL==pDM) return -5;

	strWhere.Format( _T(" USER_KEY = %s ") , m_strUserKey );
	pDM->RefreshDataManager(strWhere);

	DisplayGrid();

	return 0;
}

void CKL_STATEMENT_IMAGE::OnUploadFile() 
{
EFS_BEGIN
	INT			ids = -1, nInsertedRow = -1, nKB = 0, nFileExtIdx = -1;	
	TCHAR		szCurrentDir[512];
	CString		sPath = _T(""), strReckey = _T(""), strFileName = _T(""), strCreateDate = _T(""), strMsg = _T(""), strFileExt = _T(""), strFirstWork = _T("");
	CStdioFile	Dlg;	
	
	
	_tgetcwd(szCurrentDir, 512);		
	CFileDialog fd(TRUE, NULL , NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ,_T("JPG File (*.jpg)|*.jpg|JPEG File (*.jpeg)|*.jpeg|||") );	
	if (fd.DoModal() != IDOK) return ;
	_tchdir(szCurrentDir);
	m_sImageFilePath = fd.GetPathName();

	
	m_sImageFilePath.MakeReverse();
	nFileExtIdx = m_sImageFilePath.Find(_T("."));

	if ( nFileExtIdx > 0 ) {
		strFileExt = m_sImageFilePath.Left(nFileExtIdx);
		strFileExt.MakeReverse();
	}
	m_sImageFilePath.MakeReverse();
	strFileExt.MakeUpper();
	if ( !(strFileExt == _T("JPG") || strFileExt == _T("JPEG")) ) {
		strMsg.Format(_T("선택한 파일은 확장자가 [ %s ] 입니다.\n 1024KB 미만의 jpg/jpeg 파일을 선택하십시오."), strFileExt);
		AfxMessageBox( strMsg );
		return;
	}

	
	HANDLE hFile = CreateFile( m_sImageFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL );
	int nFileSize = GetFileSize( hFile, NULL );
	CloseHandle( hFile );	
	
	if ( nFileSize > _MAX_FILE_SIZE ) {
		m_sImageFilePath = _T("");	
		nKB = nFileSize / 1000;
		strMsg.Format(_T("선택한 파일의 사이즈는 [ %d KB ]로 너무 큽니다.\n 1024KB 미만의 파일을 선택하십시오."), nKB);
		AfxMessageBox( strMsg );
		return;
	}	

	
	ids = UpdateScanFile(strReckey, strFileName, strCreateDate, strFirstWork);
	if ( ids < 0 ) {		
		ERROR_MSG_RETURN_VOID2( ids , _T("UpdateScanFile") );	
	}	
	
	
	ids = InsertDM(nInsertedRow, strReckey, strFileName, strCreateDate, strFirstWork);
	if ( ids < 0 ) {		
		ERROR_MSG_RETURN_VOID2( ids , _T("InsertDM") );	
	}

	
	AfxGetApp()->DoWaitCursor( 1 );
	ids = DisplayImage(nInsertedRow);
	if ( ids < 0 ) {		
		AfxGetApp()->DoWaitCursor( -1 );
		ERROR_MSG_RETURN_VOID2( ids , _T("DisplayImage") );	
	}
	AfxGetApp()->DoWaitCursor( -1 );

	
	AfxMessageBox(_T("파일이 업로드되었습니다."));
	
EFS_END
}

INT CKL_STATEMENT_IMAGE::UpdateScanFile(CString& strReckey, CString& strFileName, CString& strCreateDate, CString& strFirstWork)
{
	INT ids = -1, nFind = -1;
	CString strInsertDate = _T(""), strTmp = _T("");
	

	CESL_DataMgr* pDM = FindDM(m_strDMAlias);
	if(NULL==pDM) return -5;

	strReckey = _T("");
	ids = pDM->MakeRecKey(strReckey);

	strCreateDate = _T("");
	CTime t = CTime::GetCurrentTime();
	strInsertDate.Format(_T("TO_DATE('%04d/%02d/%02d %02d:%02d:%02d', 'YYYY/MM/DD HH24:MI:SS')"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	strCreateDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	strFileName = _T("");
	strTmp = m_sImageFilePath;
	strTmp.MakeReverse();
	nFind = strTmp.Find(_T("\\"));
	strFileName = strTmp.Mid(0, nFind);
	strFileName.MakeReverse();

	if (strFileName.GetLength() == 0 ) {
		AfxMessageBox(_T("선택된 파일 핸들링 오류!"));
		return -6;
	}

	
	strFirstWork = this->GetWorkLogMsg(_T("등록"), __WFILE__, __LINE__);
	pDM->StartFrame();
	pDM->InitDBFieldData();	
	ids = pDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strReckey, TRUE);
	if ( ids < 0 ) return -10;
	ids = pDM->AddDBFieldData(_T("USER_KEY"), _T("NUMERIC"), m_strUserKey, TRUE);
	if ( ids < 0 ) return -15;
	ids = pDM->AddDBFieldData(_T("FILENAME"), _T("STRING"), strFileName, TRUE);
	if ( ids < 0 ) return -20;
	ids = pDM->AddDBFieldData(_T("CREATE_DATE"), _T("NUMERIC"), strInsertDate, TRUE);	
	if ( ids < 0 ) return -25;
	ids = pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), strFirstWork, TRUE);
	if ( ids < 0 ) return -30;	
	ids = pDM->MakeInsertFrame(_T("CO_AGREEMENT_SCAN_TBL"));
	if ( ids < 0 ) return -35;
	ids = pDM->SendFrame(TRUE);
	if ( ids < 0 ) return -40;
	pDM->EndFrame();

	
	CFile file;
	ids = file.Open(m_sImageFilePath, CFile::modeRead | CFile::typeBinary);
	if( ids == 0 ) {
		file.Close();
		AfxMessageBox(_T("선택된 파일 읽기 오류!"));
		return -45;
	}

	TCHAR szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);	
	
	INT nFileSize = file.GetLength();
	BYTE* pBuffer = (BYTE*)malloc(nFileSize+ 1024);
	file.ReadHuge(pBuffer, nFileSize);
	file.Close();
	
	ids = pDM->LOBUpdate( _T("CO_AGREEMENT_SCAN_TBL") , _T("REC_KEY"), _T("NUMERIC") , strReckey, _T("IMAGE_DATA") , pBuffer, nFileSize);
	if (ids < 0) {
		AfxMessageBox(_T("파일 업로드 오류!"));
		free (pBuffer);
		return -50;
	}
	free(pBuffer);	

	return 0;
}

INT CKL_STATEMENT_IMAGE::InsertDM(INT& nInsertedRowIdx, CString strReckey, CString strFileName, CString strCreateDate, CString strFirstWork)
{
	INT ids = -1, nRowCnt = 0, i = 0;
	CString strMaxNum = _T(""), strCmpValue = _T(""), strBaseValue = _T("0"), strInsertRowVal = _T("");
	CESL_DataMgr* pDM = FindDM(m_strDMAlias);
	if(NULL==pDM) return -5;		

	
	nRowCnt = pDM->GetRowCount();	
	for ( i = 0; i < nRowCnt; i++ ) {
		if ( i == 0 ) {
			ids = pDM->GetCellData(_T("RNUM"), i, strBaseValue);
			if ( strBaseValue.GetLength() == 0 ) strBaseValue = _T("0");
			continue;
		}
		ids = pDM->GetCellData(_T("RNUM"), i, strCmpValue);
		if ( strCmpValue.GetLength() == 0 ) strCmpValue = _T("0");

		if ( _ttoi(strBaseValue) < _ttoi(strCmpValue) ) strBaseValue = strCmpValue;
	}

	strInsertRowVal.Format(_T("%d"), _ttoi(strBaseValue) + 1);

	
	ids = pDM->InsertRow(-1);
	nInsertedRowIdx = pDM->GetRowCount() - 1;	

	pDM->SetCellData(_T("RNUM")       , strInsertRowVal, nInsertedRowIdx );
	pDM->SetCellData(_T("REC_KEY")    , strReckey      , nInsertedRowIdx );
	pDM->SetCellData(_T("USER_KEY")   , m_strUserKey   , nInsertedRowIdx );
	pDM->SetCellData(_T("FILENAME")   , strFileName    , nInsertedRowIdx );
	pDM->SetCellData(_T("CREATE_DATE"), strCreateDate  , nInsertedRowIdx );	
	pDM->SetCellData(_T("FIRST_WORK") , strFirstWork   , nInsertedRowIdx );	

	DisplayGrid();
	
	return 0;
}

void CKL_STATEMENT_IMAGE::DisplayGrid()
{
	INT ids = AllControlDisplay(_T("CM_SCANFILE"));
}

INT CKL_STATEMENT_IMAGE::DisplayImage(INT idx)
{
	INT ids = -1 , nFileExtIdx = -1;
	CString strDisplayFilePath = _T(""), strRNum = _T(""), strFileExt = _T(""), strFileName = -1;

	
	CESL_DataMgr* pDM = FindDM(m_strDMAlias);
	if(NULL==pDM) return -5;	
	ids = pDM->GetCellData(_T("RNUM"), idx, strRNum);
	if ( ids < 0 ) return -10;
	ids = pDM->GetCellData(_T("FILENAME"), idx, strFileName);
	if ( ids < 0 ) return -11;

	strFileName.MakeReverse();
	nFileExtIdx = strFileName.Find(_T("."));	
	if ( nFileExtIdx > 0 ) {
		strFileExt = strFileName.Left(nFileExtIdx);
		strFileExt.MakeReverse();
	}
	strFileExt.MakeUpper();
	if ( !(strFileExt == _T("JPG") || strFileExt == _T("JPEG")) ) {
		AfxMessageBox(_T("선택된 이미지의 확장자가 이상합니다."));
		return 100;
	}
	strFileExt.MakeLower();
	
	strDisplayFilePath.Format( _T("%s%s_%s.%s"), _IMAGE_PATH, m_strUserKey, strRNum, strFileExt);	

	
	_tmkdir(_T("../대출증"));
	_tmkdir(_T("../대출증/이용자약관동의"));

	DeleteFile(strDisplayFilePath);

	
	ids = DownloadFile(idx, strDisplayFilePath);
	if ( ids < 0 ) return ids;

	
	CFile fFile;
	ids = fFile.Open( strDisplayFilePath , CFile::modeRead | CFile::typeBinary );
	if( 0 == ids ) {
		if( NULL != m_pBmp ) {
			delete m_pBmp;
		}
		
		m_pBmp = NULL;
		OnPaint();
		AfxMessageBox(_T("선택된 이미지를 불러올 수 없습니다."));
		return 100;
	}
	
	fFile.Close();
	
	if( NULL == m_pBmp ) {
		m_pBmp = new PLWinBmp;
	}
	
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;
	
	cv.UnicodeToMultiByte( strDisplayFilePath.GetBuffer(0), &szTmp, &len );
	if( 0 < len ) {
		m_Decoder.MakeBmpFromFile (szTmp, m_pBmp);	
		delete [] szTmp;
	}
	
	OnPaint();
	

	return 0;
}

INT CKL_STATEMENT_IMAGE::DownloadFile(INT idx, CString strDisplayFilePath)
{
	INT ids = -1;
	CString strFileName = _T(""), strRecKey = _T("");

	CESL_DataMgr* pDM = FindDM(m_strDMAlias);
	if(NULL==pDM) return -5;	
	ids = pDM->GetCellData(_T("REC_KEY"), idx, strRecKey);
	if ( ids < 0 ) return -10;
	ids = pDM->GetCellData(_T("FILENAME"), idx, strFileName);
	if ( ids < 0 ) return -15;

	if ( strFileName.GetLength() <= 0 ) return 0;

	BYTE *pBuffer = (BYTE *)malloc(_MAX_SCAN_BLOB_BUF);
	
	INT nSize;
	ids = pDM->LOBGet( _T("CO_AGREEMENT_SCAN_TBL") , _T("REC_KEY") , _T("NUMERIC") , strRecKey , _T("IMAGE_DATA") , pBuffer , &nSize );
	if( 0 > ids ) return -20;
	
	if( 0 >= nSize ) {
		if( NULL != m_pBmp ) {
			delete m_pBmp;
		}
		
		m_pBmp = NULL;
		OnPaint();
		free( pBuffer );
		AfxMessageBox( _T("등록된 파일정보는 있으나, 업로드 되지 않은 파일이 있습니다.\n 등록정보를 삭제하고 다시 등록하십시오.") );		
		return 0;
	}
	
	CFile fd;
	if( NULL == fd.Open( strDisplayFilePath , CFile::modeCreate|CFile::modeWrite | CFile::typeBinary) ) {
		AfxMessageBox( _T("파일 내려받기 오류!") );
		free( pBuffer );
		return -25;
	}
	
	fd.WriteHuge(pBuffer, nSize);
	free( pBuffer );
	
	fd.Close();

	return 0;
}

void CKL_STATEMENT_IMAGE::OnPaint() 
{
	CPaintDC dc(this);
	if (!GetSafeHwnd()) {
		return;
	}
	
	if (!GetDlgItem(IDC_PREVIEW) && !GetDlgItem(IDC_PREVIEW)->GetSafeHwnd()) {
		return;
	}	
		
	if ( m_pBmp == NULL ) {		
		m_pBmp = new PLWinBmp;
		m_Decoder.MakeBmpFromFile ("..\\BMP\\NoImage.jpg", m_pBmp);
	}
	
	CRect rc;
	GetDlgItem(IDC_PREVIEW)->GetWindowRect(rc);
	ClientToScreen(rc);
	PLFilterResizeBilinear Filter(rc.Width()-2, rc.Height()-2);
	Filter.ApplyInPlace(m_pBmp); 
	
	CDC* pDC = GetDlgItem(IDC_PREVIEW)->GetDC();
	m_pBmp->Draw(pDC->m_hDC, 1, 1);
	ReleaseDC(pDC);
}

BEGIN_EVENTSINK_MAP(CKL_STATEMENT_IMAGE, CDialog)
    //{{AFX_EVENTSINK_MAP(CKL_STATEMENT_IMAGE)
	ON_EVENT(CKL_STATEMENT_IMAGE, IDC_GRID_SCAN_FILE, -601 /* DblClick */, OnDblClickGridScanFile, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKL_STATEMENT_IMAGE::OnDblClickGridScanFile() 
{
	INT ids = -1, nIdx = -1;	
	CESL_ControlMgr *pCM = FindCM( _T("CM_SCANFILE") );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("CKL_STATEMENT_IMAGE::OnDblClickGridScanFile") );
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("FileListGrid") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2( -2 , _T("CKL_STATEMENT_IMAGE::OnDblClickGridScanFile") );
	
	if( pGrid->GetMouseRow() < 1 ) return ;

	nIdx = pGrid->GetIdx();
	if(0 > nIdx) {
		return;
	}

	AfxGetApp()->DoWaitCursor( 1 );
	ids = DisplayImage(nIdx);
	if ( ids < 0 ) {
		AfxGetApp()->DoWaitCursor( -1 );
		ERROR_MSG_RETURN_VOID2( ids , _T("CKL_STATEMENT_IMAGE::OnDblClickGridScanFile") );
	}	
	AfxGetApp()->DoWaitCursor( -1 );
}

void CKL_STATEMENT_IMAGE::OnDelete() 
{
	INT ids = -1, nSelection = 0;
	CString strMsg = _T("");

	
	ids = Check_Selection(nSelection);
	if ( ids < 0 ) {
		ERROR_MSG_RETURN_VOID2( ids , _T("CKL_STATEMENT_IMAGE::CheckSelection()") );
	}
	if ( nSelection <= 0 ) {
		AfxMessageBox(_T("삭제하려는 파일을 선정하십시오."));
		return;
	}

	
	strMsg.Format(_T("선정된 [%d]개의 파일을 삭제하시겠습니까?\n 삭제후에는 복구가 불가능합니다."), nSelection);
	if ( MessageBox(strMsg, _T("파일삭제"), MB_YESNO | MB_ICONQUESTION) == IDNO ) return;	

	
	ids = Delete_File();
	if ( ids < 0 ) {
		ERROR_MSG_RETURN_VOID2( ids , _T("CKL_STATEMENT_IMAGE::Delete_File()") );
	}
	
	
	DisplayGrid();

	
	if( NULL != m_pBmp ) {		
		delete m_pBmp;
		m_pBmp = NULL;
	}
	OnPaint();

	
	strMsg.Format(_T("[%d]개의 파일이 삭제되었습니다."), nSelection);
	AfxMessageBox(strMsg);
}

INT CKL_STATEMENT_IMAGE::Check_Selection(INT& nSelection)
{
	INT ids = -1, i = -1, nRowCnt = -1;
	CString strSelection = _T("");
	CESL_DataMgr* pDM = FindDM(m_strDMAlias);
	if ( pDM == NULL ) return -5;
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_SCANFILE"), _T("FileListGrid"));
	if ( pGrid == NULL ) return -10;

	nSelection = 0;
	nRowCnt = pDM->GetRowCount();
	for( i = 0 ; i < nRowCnt; i++ ) {
		ids = pGrid->GetAt(i, 0, strSelection);
		if ( strSelection != _T("V")) continue;
		nSelection++;
	}

	return 0;
return -1;
}

INT CKL_STATEMENT_IMAGE::Delete_File()
{
	INT ids = -1, i = -1, nRowCnt = 0;
	CString strSelection = _T(""), strDelKey = _T(""), strFileName = _T(""), strRecKey = _T("");
	CTime time = NULL;

	CESL_DataMgr* pDM = FindDM(m_strDMAlias);
	if ( pDM == NULL ) return -5;
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_SCANFILE"), _T("FileListGrid"));
	if ( pGrid == NULL ) return -10;
	
	
	nRowCnt = pDM->GetRowCount();	
	for( i = nRowCnt - 1 ; i >= 0; i-- ) {
		ids = pGrid->GetAt(i, 0, strSelection);
		if ( strSelection != _T("V")) continue;
	
		ids = pDM->MakeRecKey(strRecKey);
		ids = pDM->GetCellData(_T("FILENAME"), i, strFileName);
		ids = pDM->GetCellData(_T("REC_KEY"), i, strDelKey);
				
		pDM->StartFrame();
		pDM->InitDBFieldData();
		ids = pDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strRecKey, TRUE);
		if ( ids < 0 ) return -15;
		ids = pDM->AddDBFieldData(_T("USER_KEY"), _T("NUMERIC"), m_strUserKey, TRUE);
		if ( ids < 0 ) return -20;
		ids = pDM->AddDBFieldData(_T("FILENAME"), _T("STRING"), strFileName, TRUE);
		if ( ids < 0 ) return -25;
		ids = pDM->AddDBFieldData(_T("WORK_DATE"), _T("NUMERIC"), _T("SYSDATE"), TRUE);
		if ( ids < 0 ) return -30;
		ids = pDM->AddDBFieldData(_T("WORKER_ID"), _T("STRING"), this->m_pInfo->USER_ID, TRUE);
		if ( ids < 0 ) return -35;
		ids = pDM->AddDBFieldData(_T("WPRKER_IP"), _T("STRING"), this->m_pInfo->LOCAL_IP, TRUE);
		if ( ids < 0 ) return -40;
		ids = pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), this->GetWorkLogMsg(_T("스캔파일삭제"), __WFILE__, __LINE__), TRUE);
		if ( ids < 0 ) return -45;
		ids = pDM->MakeInsertFrame(_T("CO_AGREEMENT_SCAN_DEL_TBL"));
		if ( ids < 0 ) return -50;

		
		pDM->InitDBFieldData();
		ids = pDM->MakeDeleteFrame(_T("CO_AGREEMENT_SCAN_TBL"), _T("REC_KEY"), _T("NUMERIC"), strDelKey);
		if ( ids < 0 ) return -55;
		
		ids = pDM->SendFrame(TRUE);
		if ( ids < 0 ) return -60;
		pDM->EndFrame();

		
		ids = pDM->DeleteRow(i);
	}
	
	return 0;
}


INT CKL_STATEMENT_IMAGE::MakeTmpUserKey()
{
	INT i=0, ids = 0;
	CString strQuery = _T("");
	CESL_DataMgr* pDM = FindDM(m_strDMAlias);
	if(NULL==pDM) return -5;

	strQuery.Format(_T("SELECT ESL_SEQ.NEXTVAL FROM DUAL "));
	ids = pDM->GetOneValueQuery(strQuery, m_strTmpUserKey);

	if ( m_strUserKey.GetLength() <= 0 ) {
		SetUserKey(m_strTmpUserKey);
	}

	return 0;
}

void CKL_STATEMENT_IMAGE::GetTmpUserKey(CString& strKey)
{
	strKey = m_strTmpUserKey;
}
