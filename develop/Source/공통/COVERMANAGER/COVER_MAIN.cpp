// COVER_MAIN.cpp : implementation file
//

#include "stdafx.h"
#include "COVER_MAIN.h"

#include "RS_Acc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCOVER_MAIN dialog


CCOVER_MAIN::CCOVER_MAIN(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCOVER_MAIN::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCOVER_MAIN)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDataMgr = NULL;
	m_szBasicName[0] = '\0';
	m_szLocalPath[0] = '\0';
	m_szSpeciesKey[0] = '\0';
	m_nMOSE = _MONO_;
	m_iRowIndex = -1;
	m_DeleteKeyList.RemoveAll();
	m_SAOrderList.RemoveAll();

	m_nUnity = 0;
	m_sSrcFileName = NULL;
	m_sDescFileName = NULL;

	// 최초 수정으로 선택한다.
	m_InsertMode = FALSE;
}

CCOVER_MAIN::~CCOVER_MAIN()
{
	if ( m_nUnity > 0 ) {
		for ( INT i = 0 ; i < m_nUnity ; i++ ) {
			if ( NULL != m_sSrcFileName[i] )
				free (m_sSrcFileName[i]);
			if ( NULL != m_sDescFileName[i] )
				free ( m_sDescFileName[i] );
		}
		free (m_sSrcFileName);
		m_sSrcFileName = NULL;
		free (m_sDescFileName);
		m_sDescFileName = NULL;
	}
}

VOID CCOVER_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCOVER_MAIN)
	DDX_Control(pDX, IDC_LIST_CTRL2, m_ListDesc);
	DDX_Control(pDX, IDC_LIST_CTRL, m_ListSource);
	DDX_Control(pDX, IDC_EXPLORER1, m_Web);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCOVER_MAIN, CDialog)
	//{{AFX_MSG_MAP(CCOVER_MAIN)
	ON_BN_CLICKED(IDC_bINSERT, OnbINSERT)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bCANCEL_001, OnbCANCEL)
	ON_BN_CLICKED(IDC_bSAVE_001, OnbSAVE)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CTRL, OnClickListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CTRL, OnDblclkListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CTRL2, OnDblclkListCtrl2)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCOVER_MAIN message handlers

BOOL CCOVER_MAIN::OnInitDialog() 
{
	CDialog::OnInitDialog();

	INT		ids;
	INT		iIdx;
	TCHAR	sPathName[_MAX_PATH] = {NULL};
	TCHAR	sTotalPathName[_MAX_PATH] = {NULL};
	CString szTmpPathName;
	CString	strKey;
	::GetCurrentDirectory( _MAX_PATH , sPathName );


	if (InitESL_Mgr(_T("공통_표지관리")) < 0) {
		AfxMessageBox(_T("InitESL_Mgr() Error!!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_pDataMgr = FindDM( _T("DM_표지") );

	if ( TRUE == m_InsertMode ) {
		if ( _tcsclen( m_szSpeciesKey ) <= 0 ) {
			ids = m_pDataMgr->MakeRecKey(strKey);
			if ( ids ) {
				AfxMessageBox ( _T("기준명이 설정되지 않았습니다.") );
				EndDialog(IDCANCEL);
				return FALSE;
			}
			_tcscpy ( m_szSpeciesKey , strKey.GetBuffer(0) );
		}
		if ( _tcsclen ( m_szBasicName ) <= 0 ) {
			_tcscpy ( m_szBasicName , strKey.GetBuffer(0) );
		}
	}
	if ( _tcsclen ( m_szBasicName ) <= 0 ) {
		AfxMessageBox ( _T("기준명이 설정되지 않았습니다.") );
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if ( _tcsclen ( m_szLocalPath ) <= 0 ) {
		AfxMessageBox ( _T("로컬 폴더의 경로가 설정되지 않았습니다.") );
		EndDialog(IDCANCEL);
		return FALSE;
	}

	// local path 는 드라이브명을 포함한 전체여야 한다.
	if ( m_szLocalPath[1] != ':' ) {
		_tcscpy ( sTotalPathName , m_szLocalPath );
		szTmpPathName.Format(_T("%s") , sPathName);
		iIdx = szTmpPathName.ReverseFind('\\');
		if ( iIdx < 0 ) {
			AfxMessageBox( _T("유효한 로컬폴더가 아닙니다.\n확인하세요!!") );
			EndDialog(IDCANCEL);
			return FALSE;
		} else {
			_stprintf ( m_szLocalPath , _T("%s%s") , 
				szTmpPathName.Left(iIdx) , sTotalPathName );
		}
	}

	if ( FALSE == ::SetCurrentDirectory(m_szLocalPath) ) {
		::SetCurrentDirectory(sPathName);
		AfxMessageBox( _T("유효한 로컬폴더가 아닙니다.\n확인하세요!!") );
		EndDialog(IDCANCEL);
		return FALSE;
	}

	::SetCurrentDirectory(sPathName);

	if ( _tcsclen ( m_szSpeciesKey ) <= 0 ) {
		AfxMessageBox ( _T("종키가 설정되지 않았습니다.") );
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_ListSource.InsertColumn( 0 , _T("번  호") , LVCFMT_CENTER , 80 );
	m_ListSource.InsertColumn( 1 , _T("파일 이름") , LVCFMT_CENTER , 250 );
	m_ListSource.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	m_ListDesc.InsertColumn( 0 , _T("번  호") , LVCFMT_CENTER , 80 );
	m_ListDesc.InsertColumn( 1 , _T("파일 이름") , LVCFMT_CENTER , 250 );
	m_ListDesc.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	ids = Init();
	if ( ids ) {
		AfxMessageBox(_T("Init() Error!!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CCOVER_MAIN::SetLocalPath(TCHAR *inPath)
{
	INT		i, idx = 0;
	TCHAR	szTmpPath[_MAX_PATH] = {NULL};
	for ( i = 0 ; i < (INT)_tcsclen ( inPath ) ; i++ ) {
		if ( inPath[i] == '.' )
			idx++;
		else
			break;
	}	
	_tcscpy ( szTmpPath , &inPath[idx] );
	
	if ( szTmpPath[_tcsclen(szTmpPath)-1] == '\\' )
		_tcscpy ( m_szLocalPath , szTmpPath );
	else
		_stprintf ( m_szLocalPath , _T("%s\\") , szTmpPath );
}

VOID CCOVER_MAIN::SetBasicName(TCHAR *inName)
{
	_tcscpy ( m_szBasicName , inName );
}

VOID CCOVER_MAIN::OnbINSERT() 
{
	INT		ids , nSerialNo;

	CString	strPathName , strExt , strFileName , CurDate , DescPath;
	TCHAR	szPathName[_MAX_PATH] = {NULL};
	TCHAR	sTemp[64];
	COleDateTime datetime = COleDateTime::GetCurrentTime();

	GetCurrentDirectory( _MAX_PATH , szPathName );
	CFileDialog dlg(TRUE,_T("JPG") , _T("*.JPG") , OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Jpeg File(*.jpg;*.jpeg)|*.JPG;*.JPEG|BitMap File(*.bmp)|*.bmp|All File(*.*)|*.*||"));
	if ( IDOK != dlg.DoModal() ) {
		::SetCurrentDirectory(szPathName);
		return;
	}
	::SetCurrentDirectory(szPathName);
	strPathName = dlg.GetPathName();
	strExt = dlg.GetFileExt();
	
	ids = GetMaxSerialNo(&nSerialNo);
	if ( ids ) {
		AfxMessageBox( _T("GetMaxSerialNo() Error!!"));
		return;
	}

	strFileName.Format(_T("%s_%d.%s"),m_szBasicName , nSerialNo , strExt );
	DescPath.Format(_T("%s%s") , m_szLocalPath , strFileName );

	if ( !CopyFile( strPathName , DescPath , FALSE ) ) {
		CString strMsg;
		strMsg.Format( _T("CopyFile() Error!! ErrorCode : %d") , GetLastError() );
		AfxMessageBox(strMsg);
		return;
	}

	ids = m_pDataMgr->SetCellData( _T("FILE_NAME") , strFileName , -1 );
	if ( ids <= 0 ) {
		AfxMessageBox( _T("SetCellData() Error!!"));
		return;
	}

	_stprintf ( sTemp , _T("%d") , nSerialNo );
	ids = m_pDataMgr->SetCellData( _T("SERIAL_NO") , sTemp , m_pDataMgr->GetRowCount()-1 );
	if ( ids <= 0 ) {
		AfxMessageBox( _T("SetCellData() Error!!"));
		return;
	}

	ids = m_pDataMgr->SetCellData( _T("IUD_FLAG") , _T("I") , m_pDataMgr->GetRowCount()-1 );
	if ( ids <= 0 ) {
		AfxMessageBox( _T("SetCellData() Error!!"));
		return;
	}

	ids = m_pDataMgr->SetCellData( _T("SPECIES_KEY") , m_szSpeciesKey , m_pDataMgr->GetRowCount()-1 );
	if ( ids <= 0 ) {
		AfxMessageBox( _T("SetCellData() Error!!"));
		return;
	}

	if ( m_nMOSE == _MONO_ ) {
		ids = m_pDataMgr->SetCellData( _T("PUB_TYPE") , _T("MO") , m_pDataMgr->GetRowCount()-1 );
	} else {
		ids = m_pDataMgr->SetCellData( _T("PUB_TYPE") , _T("SE") , m_pDataMgr->GetRowCount()-1 );
	}
	if ( ids <= 0 ) {
		AfxMessageBox( _T("SetCellData() Error!!"));
		return;
	}

	CurDate.Format(_T("%04d/%02d/%02d") , datetime.GetYear() , datetime.GetMonth() , datetime.GetDay() );
	ids = m_pDataMgr->SetCellData( _T("CREATE_DATE") , CurDate , m_pDataMgr->GetRowCount()-1 );
	if ( ids <= 0 ) {
		AfxMessageBox( _T("SetCellData() Error!!"));
		return;
	}

	ids = ShowDataToGrid();
	if ( ids ) {
		AfxMessageBox(_T("ShowDataToGrid() Error!!"));
		return;
	}

	ids = ShowCover( m_pDataMgr->GetRowCount()-1 );
	if ( ids ) {
		AfxMessageBox(_T("ShowCover() Error!!"));
		return;
	}
}

INT CCOVER_MAIN::SetPubType(INT nType)
{
	if ( nType == _MONO_ ) {
		m_nMOSE = _MONO_;
	} else if ( nType == _SERIAL_ ) {
		m_nMOSE = _SERIAL_;
	} else {
		return -10;
	}
	return 0;
}

// SERIAL_NO 를 구해서 준다.
INT CCOVER_MAIN::GetMaxSerialNo(INT *nMaxNo)
{
	INT		ids , nRowCnt;
	INT		nMax = 0, i;
	nRowCnt = m_pDataMgr->GetRowCount();
	TCHAR*	pTemp = NULL;
	
	for ( i = 0 ; i < nRowCnt ; i++ ) {
		ids = m_pDataMgr->GetBaseFieldData( i , _T("SERIAL_NO") , &pTemp );
		if ( ids ) {
			return -10;
		}
		if ( nMax < _ttoi (pTemp) )
			nMax = _ttoi(pTemp);
		FreeOnePointer( (VOID**)&pTemp );
	}
	(*nMaxNo) = nMax+1;
	return 0;
}

VOID CCOVER_MAIN::FreeOnePointer(VOID **pPt)
{
	if ( NULL != (*pPt) ) {
		free ( (*pPt) );
		(*pPt) = NULL;
	}
}

INT CCOVER_MAIN::ShowCover(INT iRowIdx)
{	
	INT		ids , nCount;
	TCHAR *	FileName = NULL;
	CString strPath;
	FILE *fp;

	if ( iRowIdx < 0 ) {
		// 자료를 모두 삭제하여 없는 경우
		SetDlgItemInt ( IDC_ePAGE1 , 0 );
		SetDlgItemInt ( IDC_ePAGE2 , 0 );
		PrevNextEnable();
		m_iRowIndex = -1;
		SetDlgItemText ( IDC_eFILENAME , _T("") );
		strPath = _T("about:blank");
	} else {
		nCount = m_pDataMgr->GetRowCount();
		if ( nCount <= 0 ) {
			ShowCover(-1);
			return 0;
		}
		
		m_iRowIndex = iRowIdx;

		ids = m_pDataMgr->GetBaseFieldData( iRowIdx , _T("FILE_NAME") , &FileName );
		if ( ids ) {
			return -10;
		}

		SetDlgItemText ( IDC_eFILENAME , FileName );
		strPath.Format( _T("%s%s") , m_szLocalPath , FileName );
		FreeOnePointer ( (VOID**)&FileName );
		
		if ( (fp = _tfopen( strPath.GetBuffer(0) , _T("rb"))) == NULL )
			strPath = _T("about:blank");
		else
			fclose ( fp );

		SetDlgItemInt ( IDC_ePAGE1 , iRowIdx+1 );
		SetDlgItemInt ( IDC_ePAGE2 , nCount );

		PrevNextEnable();
		m_ListSource.SetItemState(m_iRowIndex , LVIS_FOCUSED | LVIS_SELECTED , 0x000F );
	}

	// strPath의 파일을 보여준다.
	LPCTSTR lpszURL = strPath;
	DWORD   dwFlags = FALSE;
	LPCTSTR lpszTargetFrameName = NULL;
	LPCTSTR lpszHeaders = NULL;
	LPVOID lpvPostData  = NULL;
	DWORD dwPostDataLen = 0;

	COleSafeArray vPostData;
	if (lpvPostData != NULL) {
		if (dwPostDataLen == 0) {
			dwPostDataLen = lstrlen((LPCTSTR) lpvPostData);
		}
		vPostData.CreateOneDim(VT_UI1, dwPostDataLen, lpvPostData);
	}
	COleVariant vURL(lpszURL , VT_BSTR);
	COleVariant vHeaders(lpszHeaders, VT_BSTR);
	COleVariant vTargetFrameName(lpszTargetFrameName, VT_BSTR);
	COleVariant vFlags((long) dwFlags, VT_I4);

	m_Web.SetStatusBar( TRUE );
	m_Web.Navigate2( vURL, vFlags, vTargetFrameName, vPostData, vHeaders );

	return 0;
}

VOID CCOVER_MAIN::PrevNextEnable()
{
	INT nIdx , nTotalCnt;
	nIdx = GetDlgItemInt ( IDC_ePAGE1 );
	nTotalCnt = GetDlgItemInt ( IDC_ePAGE2 );
	// 이전 버튼
	if ( nIdx > 1 )
		GetDlgItem ( IDC_bPREV )->EnableWindow();
	else 
		GetDlgItem ( IDC_bPREV )->EnableWindow(FALSE);
	// 다음 버튼
	if ( nIdx < nTotalCnt && nTotalCnt > 1 )
		GetDlgItem ( IDC_bNEXT )->EnableWindow();
	else
		GetDlgItem ( IDC_bNEXT )->EnableWindow(FALSE);
	// 삭제 버튼
	if ( nIdx == 0 && nTotalCnt == 0 )
		GetDlgItem ( IDC_bDELETE )->EnableWindow(FALSE);
	else
		GetDlgItem ( IDC_bDELETE )->EnableWindow();
}

INT CCOVER_MAIN::Init()
{
	INT		nRowCnt, ids , nFileSize;
	TCHAR	sWhere[512] = {NULL};
	CString	strPath , strFileSize;
	CFileFind find;
	BOOL	bWorking;
	TCHAR	sPathName[_MAX_PATH] = {NULL};
	HANDLE	hFile;
	BYTE *	pByte = NULL;

	INT		i , FileSize;
	DWORD	WritedByte;
	TCHAR *	sRecKey = NULL;
	TCHAR *	FileName = NULL;

	m_DeleteKeyList.RemoveAll();
	m_SAOrderList.RemoveAll();

	m_ListSource.DeleteAllItems();
	m_ListDesc.DeleteAllItems();

	::GetCurrentDirectory( _MAX_PATH , sPathName );
	::SetCurrentDirectory( m_szLocalPath );

	bWorking = find.FindFile( _T("*.*") );
	while(bWorking) {
		bWorking = find.FindNextFile();
		if ( find.IsDots() ) continue;
		if ( find.IsDirectory() ) continue;

		strPath = find.GetFilePath();
		if ( !DeleteFile ( strPath ) ) {
			strPath.Format(_T("파일 삭제를 실패하였습니다. ErrorCode:%d") , GetLastError() );
			return -5;
		}
	}
	::SetCurrentDirectory( sPathName );

	COleVariant * vURL = new COleVariant(_T("about:blank"));//, VT_BSTR);
	COleVariant * pvarEmpty = new COleVariant;
	m_Web.Navigate2( vURL, pvarEmpty, pvarEmpty, pvarEmpty, pvarEmpty );
	delete vURL;
	delete pvarEmpty;

	_stprintf ( sWhere , _T("SPECIES_KEY = %s ORDER BY SERIAL_NO ") , m_szSpeciesKey );
	ids = m_pDataMgr->RefreshDataManager( sWhere );
	if ( ids ) {
		return -10;
	}
	nRowCnt = m_pDataMgr->GetRowCount();
	if ( nRowCnt <= 0 ) {
		ids = ShowCover(-1);
		if ( ids ) {
			return -20;
		}
		m_ListSource.DeleteAllItems();
		return 0;
	}

	for ( i = 0 ; i < nRowCnt ; i++ ) {
		ids = m_pDataMgr->GetBaseFieldData( i , _T("REC_KEY") , &sRecKey );
		if ( ids ) {
			return -30;
		}

		_stprintf ( sWhere , _T("SELECT FILE_SIZE FROM CO_COVER_INFO_TBL WHERE REC_KEY = %s") , sRecKey );
		m_pDataMgr->GetOneValueQuery((LPCTSTR)sWhere , strFileSize , FALSE );
		nFileSize = _ttoi ( strFileSize.GetBuffer(0) );

		ids = m_pDataMgr->GetBaseFieldData( i , _T("FILE_NAME") , &FileName );
		if ( ids ) {
			FreeOnePointer ( (VOID**)&sRecKey );
			return -40;
		}
		if ( _tcsclen ( FileName ) <= 0 ) {
			FreeOnePointer ( (VOID**)&sRecKey );
			FreeOnePointer ( (VOID**)&FileName );
			return 100;
		}
		strPath.Format(_T("%s%s") , m_szLocalPath , FileName );
		FreeOnePointer ( (VOID**)&FileName );

		pByte = (BYTE*) malloc ( sizeof(BYTE) * nFileSize +sizeof(BYTE) );
		ids = m_pDataMgr->LOBGet( _T("CO_COVER_INFO_TBL") , _T("REC_KEY") , _T("NUMERIC") , 
			sRecKey , _T("IMAGE_FILE") , pByte , &FileSize );
		FreeOnePointer ( (VOID**)&sRecKey );
		if ( ids ) {
			free(pByte);
			return -50;
		}
		hFile = CreateFile( strPath			, 
                      GENERIC_WRITE			,
                      0						,
                      NULL					,
                      CREATE_ALWAYS			,
                      FILE_ATTRIBUTE_NORMAL	,
                      NULL);
		if ( hFile == INVALID_HANDLE_VALUE ) {
			free(pByte);
			return -60;
		}
		if ( !WriteFile( hFile , pByte , FileSize , &WritedByte , NULL ) ) {
			free(pByte);
			CloseHandle ( hFile );
			return -70;
		}
		CloseHandle(hFile);
	}

	ids = ShowDataToGrid();
	if ( ids ) {
		return -25;
	}

	ids = ShowCover( 0 );
	if ( ids ) {
		return -30;
	}

	return 0;
}

INT CCOVER_MAIN::ShowGridText(INT nType, INT iRow, INT iCol, TCHAR *sData)
{
	LVITEM lv;
	lv.mask = LVIF_TEXT;
	lv.iItem = iRow;
	lv.iSubItem = iCol;
	lv.pszText = sData;

	if ( nType == 0 ) {
		//첫번째 그리드에 쓴다.
		if ( iCol == 0 ) {
			if ( 0 > m_ListSource.InsertItem(&lv) ) {
				return -10;
			}
		} else {
			if ( !m_ListSource.SetItem(&lv) )
				return -20;
		}
	} else if ( nType == 1 ) {
		//첫번째 그리드에 쓴다.
		if ( iCol == 0 ) {
			if ( 0 > m_ListDesc.InsertItem(&lv) ) {
				return -10;
			}
		} else {
			if ( !m_ListDesc.SetItem(&lv) )
				return -20;
		}
	}
	else
		return -30;

	return 0;
}

VOID CCOVER_MAIN::OnbDELETE() 
{
	INT		ids;
	TCHAR *	sTemp = NULL;
	TCHAR *	sRecKey = NULL;
	CString	strFileName;
	INT		nRowCnt = m_pDataMgr->GetRowCount();
	if ( m_iRowIndex < 0 || nRowCnt <= 0 || nRowCnt <= m_iRowIndex ) {
		AfxMessageBox( _T("삭제할 데이터가 없거나 잘못된 인덱스 입니다") );
		GetDlgItem(IDC_bDELETE)->EnableWindow(FALSE);
		return;
	}
	ids = m_pDataMgr->GetBaseFieldData( m_iRowIndex , _T("IUD_FLAG") , &sTemp );
	if ( ids ) {
		AfxMessageBox( _T("GetBaseFieldData() Error!!") );
		return;
	}
	if ( _tcsicmp( sTemp , _T("I") ) != 0 ) {
		ids = m_pDataMgr->GetBaseFieldData( m_iRowIndex , _T("REC_KEY") , &sRecKey );
		if ( ids ) {
			FreeOnePointer ( (VOID**)&sTemp );
			AfxMessageBox( _T("GetBaseFieldData() Error!!") );
			return;
		}
		strFileName = m_pDataMgr->GetCellData(_T("FILE_NAME") , m_iRowIndex);
		m_DeleteKeyList.Add(sRecKey);
		FreeOnePointer ( (VOID**)&sRecKey );
/*
		if ( m_nMOSE == _MONO_ )
			ids = RS_RemoveFile( strFileName.GetBuffer(0) , _T("CO_COVER_INFO_TBL\\MONO") , LS );
		else
			ids = RS_RemoveFile( strFileName.GetBuffer(0) , _T("CO_COVER_INFO_TBL\\SERIAL") , LS );
*/
		if ( ids ) {
			FreeOnePointer ( (VOID**)&sTemp );
			AfxMessageBox( _T("RS_RemoveFile() Error!!") );
			return;
		}
	}
	FreeOnePointer ( (VOID**)&sTemp );

	strFileName = m_pDataMgr->GetCellData(_T("FILE_NAME") , m_iRowIndex);
	ids = DeleteDescGrid( strFileName );
	if ( ids ) {
		AfxMessageBox( _T("DeleteDescGrid() Error!!") );
		return;
	}

	ids = m_pDataMgr->DeleteRow(m_iRowIndex);
	if ( ids ) {
		AfxMessageBox( _T("DeleteRow() Error!!") );
		return;
	}

	ids = ShowDataToGrid();
	if ( ids ) {
		AfxMessageBox( _T("ShowDataToGrid() Error!!") );
		return;
	}

	ids = ShowCover ( 0 );
	if ( ids ) {
		AfxMessageBox( _T("ShowCover() Error!!") );
		return;
	}
}

INT CCOVER_MAIN::ShowDataToGrid()
{
	INT		ids , i , j , nRowCnt;
	TCHAR	sRow[8] = {NULL};
	TCHAR*	sFileName = NULL;

	m_ListSource.DeleteAllItems();
	nRowCnt = m_pDataMgr->GetRowCount();
	if ( nRowCnt <= 0 ) {
		return 0;
	}

	for ( i = 0 ; i < nRowCnt ; i++ ) {
		for ( j = 0 ; j < 2 ; j++ ) {
			_stprintf ( sRow , _T("%d") , i+1 );
			if ( j == 0 ) {
				ids = ShowGridText( 0 , i , j , sRow );
				if ( ids ) {
					return -10;
				}
			} else {
				ids = m_pDataMgr->GetBaseFieldData( i , _T("FILE_NAME") , &sFileName);
				if ( ids ) {
					return -20;
				}
				ids = ShowGridText ( 0 , i , j , sFileName );
				FreeOnePointer ( (VOID**)&sFileName );
				if ( ids ) {
					return -30;
				}
			}
		}
	}
	return 0;
}


VOID CCOVER_MAIN::OnbPREV() 
{
	INT		ids , nRowCnt;

	nRowCnt = m_pDataMgr->GetRowCount();
	if ( nRowCnt <= 0 ) {
		GetDlgItem(IDC_bPREV)->EnableWindow(FALSE);
		return;
	}
	if ( m_iRowIndex > 0 && m_iRowIndex < nRowCnt ) {
		ids = ShowCover( m_iRowIndex-1 );
		if ( ids ) {
			AfxMessageBox ( _T("ShowCover() Error!!") );
			return;
		}
	}
}

VOID CCOVER_MAIN::OnbNEXT() 
{
	INT		ids , nRowCnt;

	nRowCnt = m_pDataMgr->GetRowCount();
	if ( nRowCnt <= 0 ) {
		GetDlgItem(IDC_bNEXT)->EnableWindow(FALSE);
		return;
	}
	if ( m_iRowIndex >= 0 && m_iRowIndex <= nRowCnt-2 ) {
		ids = ShowCover( m_iRowIndex+1 );
		if ( ids ) {
			AfxMessageBox ( _T("ShowCover() Error!!") );
			return;
		}
	}
}

VOID CCOVER_MAIN::OnbCANCEL() 
{
	INT		ids;
	ids = Init();
	if ( ids ) {
		AfxMessageBox ( _T("Init() Error!!") );
		return;
	}
}

VOID CCOVER_MAIN::OnbSAVE() 
{
	INT		ids , nOrderCnt , iWorkIndex;
	BOOL	bInsert;
	TCHAR*	pIUD = NULL;
	CString	sRecKey , sFileName , sFilePath , strIUD , strSrcFileName , strSrcFilePath;
	CString sDeleteKey , Where , strFileSize;
	HANDLE						hFile;
	BY_HANDLE_FILE_INFORMATION	bhfi;
	INT							nBytesSource;
	DWORD						ReadByte;

	INT nRowCount = m_pDataMgr->GetRowCount();

	if ( m_DeleteKeyList.GetSize() <= 0 && nRowCount <= 0 ) {
		AfxMessageBox ( _T("적용할 표지 데이터가 없습니다.") );
		return;
	}

	nOrderCnt = m_ListDesc.GetItemCount();
	if ( nOrderCnt != 0 && nOrderCnt != nRowCount ) {
		AfxMessageBox ( _T("순서를 새로 정의 하려면 모두 함께 적용해야 합니다.\n왼쪽 그리드와 오른쪽 그리드의 개수를 일치하세요!! "));
		return;
	}

	for(INT i=0; i < nRowCount; i++) {
		if ( nOrderCnt > 0 ) {
			ids = GetWorkIndex( m_SAOrderList.GetAt(i) , &iWorkIndex );
			if ( ids ) {
				AfxMessageBox ( _T("새로 정렬하려는 자료중 \n원본에 존재하지 않는 자료가 있습니다.") );
				return;
			}
		} else {
			iWorkIndex = i;
		}
		ids = m_pDataMgr->GetCellData( _T("IUD_FLAG") , iWorkIndex , strIUD );
		if ( ids ) {
			AfxMessageBox ( _T("GetBaseFieldData() Error!!") );
			return;
		}
		if ( strIUD.CompareNoCase( _T("I") ) == 0 ) {
			bInsert = TRUE;
			m_pDataMgr->MakeRecKey(sRecKey);
		} else {
			bInsert = FALSE;
			m_pDataMgr->GetCellData(_T("REC_KEY"), iWorkIndex, sRecKey);
		}

		sFileName = m_pDataMgr->GetCellData( _T("FILE_NAME") , iWorkIndex );
		strSrcFileName.Format( _T("%s") , sFileName );
		sFilePath.Format(_T("%s%s") , m_szLocalPath , sFileName );
		strSrcFilePath.Format( _T("%s") , sFilePath );

		// 파일의 크기와 사이즈를 구한다.
		hFile = CreateFile(sFilePath		,
                       GENERIC_READ			,
                       0					,
                       NULL					,
                       OPEN_EXISTING		,
                       FILE_ATTRIBUTE_NORMAL,
                       NULL);
		if (hFile == INVALID_HANDLE_VALUE) 
		{
			AfxMessageBox ( _T("CreateFile() Error!!") );
			return;
		}	
		
		GetFileInformationByHandle (hFile, &bhfi);
		if (bhfi.nFileSizeHigh != 0) 
		{
			CloseHandle (hFile);
			AfxMessageBox ( _T("GetFileInformationByHandle() Error!!") );
			return;
		}
		nBytesSource = bhfi.nFileSizeLow;
		BYTE * pByte = (BYTE*) malloc ( sizeof(BYTE) * nBytesSource +sizeof(BYTE) );
		pByte[0] = '\0';

		if ( !ReadFile ( hFile , pByte , nBytesSource , &ReadByte , NULL ) ) {
			free ( pByte );
			CloseHandle ( hFile );
			AfxMessageBox ( _T("ReadFile() Error!!") );
			return;
		}

		CloseHandle ( hFile );
		strFileSize.Format(_T("%d") , ReadByte);

		m_pDataMgr->StartFrame();
		m_pDataMgr->InitDBFieldData();
		m_pDataMgr->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), sRecKey);
		m_pDataMgr->AddDBFieldData(_T("SPECIES_KEY"), _T("NUMERIC"), m_pDataMgr->GetCellData(_T("SPECIES_KEY"), iWorkIndex));
		if ( nOrderCnt > 0 ) {
			CString sTemp;
			sTemp.Format(_T("%d") , i+1);
			m_pDataMgr->AddDBFieldData(_T("SERIAL_NO"), _T("NUMERIC"), sTemp );
			GetNewFileName(sFileName , m_szBasicName , i+1);
		} else {
			m_pDataMgr->AddDBFieldData(_T("SERIAL_NO"), _T("NUMERIC"), m_pDataMgr->GetCellData(_T("SERIAL_NO"), i));
		}
		if ( bInsert ) 
			m_pDataMgr->AddDBFieldData(_T("CREATE_DATE"), _T("STRING"), m_pDataMgr->GetCellData(_T("CREATE_DATE"), i));
		m_pDataMgr->AddDBFieldData(_T("PUB_TYPE"), _T("STRING"), m_pDataMgr->GetCellData(_T("PUB_TYPE"), i));
		m_pDataMgr->AddDBFieldData(_T("FILE_NAME"), _T("STRING"), sFileName );
		m_pDataMgr->AddDBFieldData(_T("FILE_SIZE"), _T("NUMERIC"), strFileSize );
/*
		if ( m_nMOSE == _MONO_ )
			ids = RS_UpLoadFile ( strSrcFileName.GetBuffer(0) , strSrcFilePath.GetBuffer(0) , sFileName.GetBuffer(0) ,
								_T("CO_COVER_INFO_TBL\\MONO") , LS );
		else {
			ids = RS_UpLoadFile ( strSrcFileName.GetBuffer(0) , strSrcFilePath.GetBuffer(0) , sFileName.GetBuffer(0) ,
								_T("CO_COVER_INFO_TBL\\SERIAL") , LS );
		}
*/
		if ( ids ) {
			AfxMessageBox ( _T("파일을 서버로 이동중 오류가 발생하였습니다.") );
			return;
		}

		if ( bInsert ) 
			ids = m_pDataMgr->MakeInsertFrame(_T("CO_COVER_INFO_TBL"));
		else
			ids = m_pDataMgr->MakeUpdateFrame(_T("CO_COVER_INFO_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		if ( ids < 0 ) {
			AfxMessageBox ( _T("MakeUpdateFrame() Error!!") );
			return;
		}

		ids = m_pDataMgr->SendFrame(FALSE);
		if ( ids ) {
			AfxMessageBox ( _T("SendFrame() Error!!") );
			return;
		}
		m_pDataMgr->EndFrame();

		if ( bInsert ) {
			ids = m_pDataMgr->LOBUpdate(_T("CO_COVER_INFO_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey ,
				_T("IMAGE_FILE") , pByte , ReadByte );
			if ( ids ) {
				free ( pByte );
				AfxMessageBox ( _T("LOBUpdate() Error!!") );
				return;
			}
		}
		free ( pByte );
	}

	// 삭세한다.
	for ( i = 0 ; i < m_DeleteKeyList.GetSize() ; i++ ) {
		m_pDataMgr->StartFrame();
		sDeleteKey = m_DeleteKeyList.GetAt(i);
		if ( FALSE == sDeleteKey.IsEmpty() )
			m_pDataMgr->MakeDeleteFrame( _T("CO_COVER_INFO_TBL") , _T("REC_KEY") , _T("NUMERIC") , sDeleteKey );
		m_pDataMgr->SendFrame();
		m_pDataMgr->EndFrame();
	}

	AfxMessageBox ( _T("표지적용 완료") );
	CDialog::OnOK();
}

VOID CCOVER_MAIN::SetSpeciesKey(TCHAR *inSzKey)
{
	_tcscpy ( m_szSpeciesKey , inSzKey );
}



VOID CCOVER_MAIN::OnClickListCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	INT		ids;
	NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
	if ( pNMListView->iItem < 0 ) {
		return ;
	} else {
		ids = ShowCover( pNMListView->iItem );
		if ( ids ) {
			AfxMessageBox ( _T("ChowCover() Error!!") );
			return ;
		}
	}
	*pResult = 0;
}

VOID CCOVER_MAIN::OnDblclkListCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	INT		i , ids , nCnt;
	INT		iRowIdx;
	CString strFileName;

	NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;

	iRowIdx = pNMListView->iItem;
	if ( iRowIdx < 0 ) {
		return ;
	} else {
		nCnt = m_SAOrderList.GetSize();
		strFileName = m_ListSource.GetItemText ( iRowIdx , 1 );
		if ( strFileName.IsEmpty() )
			return;
		for ( i = 0 ; i < nCnt ; i++ ) {
			if ( m_SAOrderList.GetAt(i).CompareNoCase(strFileName) == 0 ) {
				return;
			}
		}
		m_SAOrderList.Add(strFileName);
		ids = ShowDescDataToGrid();
		if ( ids ) {
			AfxMessageBox ( _T("ShowDescDataToGrid() Error!!") );
			return;
		}
	}
	*pResult = 0;
}

VOID CCOVER_MAIN::SetInsertMode( BOOL	bMode ) 
{
	m_InsertMode = bMode;
}

VOID CCOVER_MAIN::GetSpeciesKey( CString &strKey )
{
	strKey.Format(_T("%s") , m_szSpeciesKey);
}

INT CCOVER_MAIN::ShowDescDataToGrid()
{
	INT		ids , nCnt;
	CString sTemp;

	m_ListDesc.DeleteAllItems();
	nCnt = m_SAOrderList.GetSize();

	for ( INT i = 0 ; i < nCnt ; i++ ) {
		for ( INT j = 0 ; j < 2 ; j++ ) {
			if ( j == 0 ) {
				sTemp.Format( _T("%d") , i+1 );
				ids = ShowGridText( 1 , i , j , sTemp.GetBuffer(0) );
				if ( ids ) return ids;
			} else {
				ids = ShowGridText( 1 , i , j , m_SAOrderList.GetAt(i).GetBuffer(0) );
				if ( ids ) return ids;
			}
		}
	}
	return 0;
}

VOID CCOVER_MAIN::OnDblclkListCtrl2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	INT		ids;
	INT		iRowIdx;
	CString strFileName;

	NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;

	iRowIdx = pNMListView->iItem;
	if ( iRowIdx < 0 ) {
		return ;
	} else {
		strFileName = m_ListDesc.GetItemText( iRowIdx , 1 );
		ids = DeleteDescGrid(strFileName);
		if ( ids ) {
			AfxMessageBox ( _T("DeleteDescGrid() Error!!") );
			return;
		}
	}
	*pResult = 0;
}

INT CCOVER_MAIN::DeleteDescGrid(CString strFileName)
{
	INT		i , ids , nCnt;
	BOOL	bDelete = FALSE;

	if ( strFileName.IsEmpty() ) 
		return 0;

	nCnt = m_SAOrderList.GetSize();
	for ( i = nCnt-1 ; i >= 0 ; i-- ) {
		if ( m_SAOrderList.GetAt(i).CompareNoCase(strFileName) == 0 ) {
			m_SAOrderList.RemoveAt(i);
			bDelete = TRUE;
		}
	}
	if ( bDelete ) {
		ids = ShowDescDataToGrid();
		if ( ids ) {
			return ids;
		}
	}
	return 0;
}

INT CCOVER_MAIN::GetWorkIndex(CString strFileName, INT *iIdx)
{
	INT		i;
	INT		nCnt = m_pDataMgr->GetRowCount();
	for ( i = 0 ; i < nCnt ; i++ ) {
		if ( m_pDataMgr->GetCellData( _T("FILE_NAME") , i ).CompareNoCase(strFileName) == 0 ) {
			(*iIdx) = i;
			return 0;
		}
	}
	return -10;
}

VOID CCOVER_MAIN::GetNewFileName(CString &strName, TCHAR * sBasicName , INT nSerialNo)
{
	CString strExt;
	INT		idx = strName.ReverseFind('.');
	strExt = strName.Right(strName.GetLength()-idx-1);

	strName.Format(_T("%s_%d.%s") , sBasicName , nSerialNo , strExt );
}

// 종통합을 수행한다. 화면 작업을 하지 않는다. 화면을 만들지도 않는다.
// 
INT CCOVER_MAIN::UnitySpecies(CESL_DataMgr * pDM , TCHAR * szBasicName , TCHAR * szSrcKey , TCHAR ** pszDecKey , INT nCount )
{
	INT		ids , nSrcCnt , nDescCnt;
	INT		nSerialNo , i;
	TCHAR	szWhere[1024] = {NULL};

	CString	strOldName , strNewName , strUpPath;
	CString strRecKey , strMoSe;

	m_nUnity = 0;
	m_sSrcFileName = NULL;
	m_sDescFileName = NULL;

	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM 이 설정되지 않았습니다.") );
		return 100;
	}
	if ( szBasicName == NULL || szBasicName[0] == '\0' ) {
		AfxMessageBox ( _T("파일명이 입력되지 않았습니다.") );
		return 100;
	}

	if ( szSrcKey == NULL || szSrcKey[0] == '\0' ) {
		AfxMessageBox ( _T("원본의 종키가 입력되지 않았습니다.") );
		return 100;
	}
	if ( nCount <= 0 ) {
		AfxMessageBox ( _T("통합할 대상이 없습니다.") );
		return 100;
	}
	for ( i = 0 ; i < nCount ; i++ ) {
		if ( pszDecKey[i] == NULL || pszDecKey[i][0] == '\0' ) {
			AfxMessageBox ( _T("통합할 대상의 종키가 입력되지 않았습니다.") );
			return 100;
		}
	}
	if ( _tcsclen ( m_szLocalPath ) <= 0 ) {
		AfxMessageBox ( _T("로컬 폴더의 경로가 설정되지 않았습니다.") );
		return 100;
	}

	if( m_pDataMgr == NULL ) 
	{
		m_pDataMgr = FindDM( _T("DM_표지") );
	}

	_stprintf ( szWhere , _T("SPECIES_KEY = %s ORDER BY SERIAL_NO") , szSrcKey );
	ids = m_pDataMgr->RefreshDataManager( (LPCTSTR)szWhere );
	if ( ids ) {
		return -10;
	}

	nSrcCnt = m_pDataMgr->GetRowCount();
	nSerialNo = nSrcCnt+1;

	// 대상 표지를 검색한다.
	_stprintf ( szWhere , _T("SPECIES_KEY IN ( ") );
	for ( i = 0 ; i < nCount ; i++ ) {
		if ( i == 0 ) {
			_stprintf ( &szWhere[_tcsclen(szWhere)] , _T("%s ") , pszDecKey[i] );
		} else {
			_stprintf ( &szWhere[_tcsclen(szWhere)] , _T(" , %s ") , pszDecKey[i] );
		}
	}
	_stprintf ( &szWhere[_tcsclen(szWhere)] , _T(") ORDER BY SPECIES_KEY , SERIAL_NO ") );
	ids = m_pDataMgr->RefreshDataManager( (LPCTSTR)szWhere );
	if ( ids ) {
		return -30;
	}

	// 수정 시작
	nDescCnt = m_pDataMgr->GetRowCount();
	for ( i = 0 ; i < nDescCnt ; i++ ) {
		strOldName = m_pDataMgr->GetCellData( _T("FILE_NAME") , i );
		strRecKey = m_pDataMgr->GetCellData( _T("REC_KEY") , i );
		strMoSe = m_pDataMgr->GetCellData( _T("PUB_TYPE") , i );
		GetNewFileName(strNewName , szBasicName , nSerialNo );
		if ( strMoSe.CompareNoCase(_T("MO")) == 0 ) {
			m_nMOSE = _MONO_;
		} else {
			m_nMOSE = _SERIAL_;
		}

		if ( strOldName.CompareNoCase(strNewName) != 0 ) {
			_stprintf ( szWhere , _T("UPDATE CO_COVER_INFO_TBL SET ")
				_T("SERIAL_NO = %d , FILE_NAME = '%s' , SPECIES_KEY = '%s' WHERE REC_KEY = %s ;") ,
				nSerialNo , strNewName , szSrcKey , strRecKey );
			ids = pDM->AddFrame((LPCTSTR)szWhere);
			if ( ids < 0 ) {
				return -40;
			}
			// 해당하는 표지파일을 다운받는다.
			strUpPath.Format(_T("%s%s") , m_szLocalPath , strOldName);
			ids = MakeCoverFile ( m_pDataMgr , i );
			if ( ids ) {
				return -50;
			}
/*
			if ( _MONO_ == m_nMOSE ) {
				ids = RS_RemoveFile( strOldName.GetBuffer(0) , _T("CO_COVER_INFO_TBL\\MONO") , LS );
			} else {
				ids = RS_RemoveFile( strOldName.GetBuffer(0) , _T("CO_COVER_INFO_TBL\\SERIAL") , LS );
			}
*/
			if ( ids ) {
				return -60;
			}
/*
			if ( _MONO_ == m_nMOSE ) {
				ids = RS_UpLoadFile ( strOldName.GetBuffer(0) , strUpPath.GetBuffer(0) , 
						strNewName.GetBuffer(0) , _T("CO_COVER_INFO_TBL\\MONO") , LS );
			} else {
				ids = RS_UpLoadFile ( strOldName.GetBuffer(0) , strUpPath.GetBuffer(0) , 
						strNewName.GetBuffer(0) , _T("CO_COVER_INFO_TBL\\SERIAL") , LS );
			}
*/
			if ( ids ) {
				return -70;
			}
			m_nUnity++;
			if ( m_nUnity == 1 ) {
				// 원래 있던 파일의 이름을 기억한다.
				m_sSrcFileName = (TCHAR**) malloc ( sizeof(TCHAR*) * m_nUnity );
				m_sSrcFileName[m_nUnity-1] = (TCHAR*)malloc(sizeof(TCHAR*)*MAX_PATH);
				m_sSrcFileName[m_nUnity-1][0] = '\0';
				_tcscpy ( m_sSrcFileName[m_nUnity-1] , strOldName.GetBuffer(0) );

				// 새로 있던 파일의 이름을 기억한다.
				m_sDescFileName = (TCHAR**) malloc ( sizeof(TCHAR*) * m_nUnity );
				m_sDescFileName[m_nUnity-1] = (TCHAR*)malloc(sizeof(TCHAR*)*MAX_PATH);
				m_sDescFileName[m_nUnity-1][0] = '\0';
				_tcscpy ( m_sDescFileName[m_nUnity-1] , strNewName.GetBuffer(0) );
			} else {
				// 원래 있던 파일의 이름을 기억한다.
				realloc ( m_sSrcFileName , sizeof(TCHAR*) * m_nUnity );
				m_sSrcFileName[m_nUnity-1] = (TCHAR*)malloc(sizeof(TCHAR*)*MAX_PATH);
				m_sSrcFileName[m_nUnity-1][0] = '\0';
				_tcscpy ( m_sSrcFileName[m_nUnity-1] , strOldName.GetBuffer(0) );
				
				// 새로 있던 파일의 이름을 기억한다.
				realloc ( m_sDescFileName , sizeof(TCHAR*) * m_nUnity );
				m_sDescFileName[m_nUnity-1] = (TCHAR*)malloc(sizeof(TCHAR*)*MAX_PATH);
				m_sDescFileName[m_nUnity-1][0] = '\0';
				_tcscpy ( m_sDescFileName[m_nUnity-1] , strNewName.GetBuffer(0) );
			}
		}
		nSerialNo++;
	}

	_stprintf ( szWhere , _T("SPECIES_KEY = %s ORDER BY SERIAL_NO") , szSrcKey );
	ids = m_pDataMgr->RefreshDataManager( (LPCTSTR)szWhere );
	if ( ids ) {
		return -10;
	}

	nSrcCnt = m_pDataMgr->GetRowCount();
	nSerialNo = 1;
	
	for ( i = 0 ; i < nSrcCnt ; i++ ) {
		strOldName = m_pDataMgr->GetCellData( _T("FILE_NAME") , i );
		strRecKey = m_pDataMgr->GetCellData( _T("REC_KEY") , i );
		strMoSe = m_pDataMgr->GetCellData( _T("PUB_TYPE") , i );
		GetNewFileName(strNewName , szBasicName , nSerialNo );

		if ( strOldName.CompareNoCase(strNewName) != 0 ) {
			_stprintf ( szWhere , _T("UPDATE CO_COVER_INFO_TBL SET ")
				_T("SERIAL_NO = %d , FILE_NAME = '%s' , SPECIES_KEY = '%s WHERE REC_KEY = %s ;") ,
				nSerialNo , strNewName , szSrcKey , strRecKey );
			ids = pDM->AddFrame((LPCTSTR)szWhere);
			if ( ids < 0 ) {
				return -20;
			}
			// 해당하는 표지파일을 다운받는다.
			strUpPath.Format(_T("%s%s") , m_szLocalPath , strOldName);
			ids = MakeCoverFile ( m_pDataMgr , i );
			if ( ids ) {
				return -50;
			}
/*
			if ( _MONO_ == m_nMOSE ) {
				ids = RS_RemoveFile( strOldName.GetBuffer(0) , _T("CO_COVER_INFO_TBL\\MONO") , LS );
			} else {
				ids = RS_RemoveFile( strOldName.GetBuffer(0) , _T("CO_COVER_INFO_TBL\\SERIAL") , LS );
			}
*/
			if ( ids ) {
				return -60;
			}
/*
			if ( _MONO_ == m_nMOSE ) {
				ids = RS_UpLoadFile ( strOldName.GetBuffer(0) , strUpPath.GetBuffer(0) , 
						strNewName.GetBuffer(0) , _T("CO_COVER_INFO_TBL\\MONO") , LS );
			} else {
				ids = RS_UpLoadFile ( strOldName.GetBuffer(0) , strUpPath.GetBuffer(0) , 
						strNewName.GetBuffer(0) , _T("CO_COVER_INFO_TBL\\SERIAL") , LS );
			}
*/
			if ( ids ) {
				return -70;
			}
			m_nUnity++;
			if ( m_nUnity == 1 ) {
				// 원래 있던 파일의 이름을 기억한다.
				m_sSrcFileName = (TCHAR**) malloc ( sizeof(TCHAR*) * m_nUnity );
				m_sSrcFileName[m_nUnity-1] = (TCHAR*)malloc(sizeof(TCHAR*)*MAX_PATH);
				m_sSrcFileName[m_nUnity-1][0] = '\0';
				_tcscpy ( m_sSrcFileName[m_nUnity-1] , strOldName.GetBuffer(0) );

				// 새로 있던 파일의 이름을 기억한다.
				m_sDescFileName = (TCHAR**) malloc ( sizeof(TCHAR*) * m_nUnity );
				m_sDescFileName[m_nUnity-1] = (TCHAR*)malloc(sizeof(TCHAR*)*MAX_PATH);
				m_sDescFileName[m_nUnity-1][0] = '\0';
				_tcscpy ( m_sDescFileName[m_nUnity-1] , strNewName.GetBuffer(0) );
			} else {
				// 원래 있던 파일의 이름을 기억한다.
				realloc ( m_sSrcFileName , sizeof(TCHAR*) * m_nUnity );
				m_sSrcFileName[m_nUnity-1] = (TCHAR*)malloc(sizeof(TCHAR*)*MAX_PATH);
				m_sSrcFileName[m_nUnity-1][0] = '\0';
				_tcscpy ( m_sSrcFileName[m_nUnity-1] , strOldName.GetBuffer(0) );
				
				// 새로 있던 파일의 이름을 기억한다.
				realloc ( m_sDescFileName , sizeof(TCHAR*) * m_nUnity );
				m_sDescFileName[m_nUnity-1] = (TCHAR*)malloc(sizeof(TCHAR*)*MAX_PATH);
				m_sDescFileName[m_nUnity-1][0] = '\0';
				_tcscpy ( m_sDescFileName[m_nUnity-1] , strNewName.GetBuffer(0) );
			}
		}
		nSerialNo++;
	}

	if ( nSrcCnt > 0 || nDescCnt > 0 ) {
		strRecKey.Format(_T("원본 종의 표지데이터 : %d\n")
						 _T("대상 종의 표지데이터 : %d\n")
						 _T("이 각각 통합 되었었습니다. 표지화면에서 확인 하세요") , nSrcCnt , nDescCnt );
		AfxMessageBox ( strRecKey );
		return 0;
	}

	return 0;
}

INT CCOVER_MAIN::MakeCoverFile(CESL_DataMgr * pDM , INT	iRowIdx)
{
	INT		ids;
	CString	strFileName , strDownPath , strMoSe;

	if ( NULL == pDM || iRowIdx <= 0 ) {
		return -10;
	}
	
	strFileName = pDM->GetCellData( _T("FILE_NAME") , iRowIdx );
	strMoSe = pDM->GetCellData( _T("PUB_TYPE") , iRowIdx );

	strDownPath.Format(_T("%s%s") , m_szLocalPath , strFileName );
/*
	if ( _MONO_ == m_nMOSE ) {
		ids = RS_DownLoadFile ( strFileName.GetBuffer(0) , strDownPath.GetBuffer(0) , 
			strFileName.GetBuffer(0) , _T("CO_COVER_INFO_TBL\\MONO") , LS );
	} else {
		ids = RS_DownLoadFile ( strFileName.GetBuffer(0) , strDownPath.GetBuffer(0) , 
			strFileName.GetBuffer(0) , _T("CO_COVER_INFO_TBL\\SERIAL") , LS );
	}
*/
	if ( ids ) {
		return -20;
	}
	return 0;
}

INT CCOVER_MAIN::CoverFileRollBack()
{
	INT		ids , i;
	CString strUpPath;
	if ( 0 >= m_nUnity ) {
		return 0;
	}

	// 새로 올라간 파일들을 지운다.
	for ( i = 0 ; i < m_nUnity ; i++ ) {
/*
		if ( _MONO_ == m_nMOSE ) {
			ids = RS_RemoveFile( m_sDescFileName[i] , _T("CO_COVER_INFO_TBL\\MONO") , LS );
		} else {
			ids = RS_RemoveFile( m_sDescFileName[i] , _T("CO_COVER_INFO_TBL\\SERIAL") , LS );
		}
*/
		if ( ids ) {
			return -10;
		}
	}

	// 원래 있던 파일로 다시 올린다.
	for ( i = 0 ; i < m_nUnity ; i++ ) {
		strUpPath.Format(_T("%s%s") , m_szLocalPath , m_sSrcFileName[i]);
/*
		if ( _MONO_ == m_nMOSE ) {
			ids = RS_UpLoadFile ( m_sSrcFileName[i] , strUpPath.GetBuffer(0) , 
				m_sSrcFileName[i] , _T("CO_COVER_INFO_TBL\\MONO") , LS );
		} else {
			ids = RS_UpLoadFile ( m_sSrcFileName[i] , strUpPath.GetBuffer(0) , 
				m_sSrcFileName[i] , _T("CO_COVER_INFO_TBL\\SERIAL") , LS );
		}
*/
		if ( ids ) {
			return -20;
		}
	}
	return 0;
}

INT CCOVER_MAIN::DeleteCoverInfo(TCHAR *pszSpeciesKey)
{
	INT		ids , nRowCnt;
	TCHAR	sWhere[512];
	CString strMose, strFileName , sRecKey;
	if ( NULL == pszSpeciesKey || pszSpeciesKey[0] == '\0' ) {
		return -10;
	}

	if (InitESL_Mgr(_T("공통_표지관리")) < 0) {
		return -20;
	}

	m_pDataMgr = FindDM( _T("DM_표지") );

	if ( NULL == m_pDataMgr ) {
		return -30;
	}

	_stprintf ( sWhere , _T("SPECIES_KEY = %s ORDER BY SERIAL_NO ") , pszSpeciesKey );
	ids = m_pDataMgr->RefreshDataManager( sWhere );
	if ( ids ) {
		return -40;
	}

	nRowCnt = m_pDataMgr->GetRowCount();
	for ( INT i = 0 ; i < nRowCnt ; i++ ) {
		strMose = m_pDataMgr->GetCellData( _T("PUB_TYPE") , i );
		strFileName = m_pDataMgr->GetCellData( _T("FILE_NAME") , i );
		sRecKey = m_pDataMgr->GetCellData( _T("REC_KEY") , i );
/*
		if ( strMose.CompareNoCase(_T("MO")) == 0 ) {
			ids = RS_RemoveFile( strFileName.GetBuffer(0) , _T("CO_COVER_INFO_TBL\\MONO") , LS );
		} else {
			ids = RS_RemoveFile( strFileName.GetBuffer(0) , _T("CO_COVER_INFO_TBL\\SERIAL") , LS );
		}
*/
		if ( ids ) {
			return -50;
		}
		m_pDataMgr->StartFrame();
		m_pDataMgr->MakeDeleteFrame( _T("CO_COVER_INFO_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		m_pDataMgr->SendFrame();
		m_pDataMgr->EndFrame();
	}

	return 0;
}

HBRUSH CCOVER_MAIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
