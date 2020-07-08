// Light_UC_NOTIFY_03.cpp : implementation file
//

#include "stdafx.h"
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include "Light_UC_NOTIFY_03.h"

// 2017/09/22 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
// KOLIS-NET OpenAPI
/*//NEW------------------------------------------------------------------------*/
#include "KolisNetService.h"
/*//END------------------------------------------------------------------------*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLight_UC_NOTIFY_03 dialog


CLight_UC_NOTIFY_03::CLight_UC_NOTIFY_03(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_RowIndex = 0;
}

CLight_UC_NOTIFY_03::~CLight_UC_NOTIFY_03()
{
	
}

BOOL CLight_UC_NOTIFY_03::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CLight_UC_NOTIFY_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLight_UC_NOTIFY_03)
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLight_UC_NOTIFY_03, CDialog)
	//{{AFX_MSG_MAP(CLight_UC_NOTIFY_03)
	ON_BN_CLICKED(IDC_bFOLDER_SELECT, OnbFOLDERSELECT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLight_UC_NOTIFY_03 message handlers

BOOL CLight_UC_NOTIFY_03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
//	CESL_Grid * m_Grid;
	CString     RecKey;
	INT         GridRowIndex, ids;
	
	//////////////////////////////////////////////////////////////////////////////////
	// 2. 현재 선택된 그리드의 RowIndex를 구한다.
	//////////////////////////////////////////////////////////////////////////////////
	GridRowIndex = m_RowIndex ;

	//////////////////////////////////////////////////////////////////////////////////
	// 3. 현재 선택된 그리드의 RowIndex의 RecKey를 구한다.
	//////////////////////////////////////////////////////////////////////////////////
	
	ids = GetDataMgrData(_T("DM_UC_NOTIFY_MAIN"),_T("REC_KEY"),RecKey,GridRowIndex );

	//////////////////////////////////////////////////////////////////////////////////
	// 4. RecKey에 해당하는 파일정보를 구한다.
	//////////////////////////////////////////////////////////////////////////////////
	ids = GetFileInfo( RecKey );
	if ( ids<0 ) {
		AfxMessageBox (_T("GetFileInfo(REC_KEY) Error"), MB_ICONEXCLAMATION);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CLight_UC_NOTIFY_03::GetRowIndex(INT index)
{
	m_RowIndex = index ;
	return 0 ;
}

INT CLight_UC_NOTIFY_03::GetFileInfo(CString pParentRecKey)
{
	CESL_DataMgr * m_DataMgr;
	CString        SqlBuf, FileName, FileSize;
	INT            ids, RecCnt, i, filelen;

	CString sFileName ;

	//////////////////////////////////////////////////////////////////////////////////
	// 1. 파일정보를 구한다.
	//////////////////////////////////////////////////////////////////////////////////
	m_DataMgr = FindDM(_T("DM_UC_NOTIFY_FILEINFO"));
	if(m_DataMgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return -1 ;
	}

	SqlBuf.Format ( _T("PARENT_KEY = %s"), pParentRecKey );
	ids = m_DataMgr->RefreshDataManager (SqlBuf);
	if ( ids<0 ) return -2;
 
	RecCnt = m_DataMgr ->GetRowCount ();    
	m_List.DeleteAllItems ();
	
	//////////////////////////////////////////////////////////////////////////////////
	// 2. 파일정보를 리스트에 추가한다.
	//////////////////////////////////////////////////////////////////////////////////
	if ( RecCnt > 0 ) {
		for ( i = 0; i < RecCnt; i++ ) {
		
			ids = GetDataMgrData (_T("DM_UC_NOTIFY_FILEINFO"),_T("파일이름"),FileName,i);            
			ids = GetDataMgrData (_T("DM_UC_NOTIFY_FILEINFO"),_T("파일사이즈"),FileSize,i);

			filelen = FileSize.GetLength ();
		
			if ( filelen < 4 ) {
				 sFileName.Format ( _T("%s(1KB)"), FileName );
			} else if ( filelen == 4 ) {
				 sFileName.Format ( _T("%s(%sKB)"), FileName, FileSize.Mid (1,1) );
			} else if ( filelen == 5 ) {
				 sFileName.Format ( _T("%s(%s%sKB)"), FileName, FileSize.Mid (1,1), FileSize.Mid (2,1) );
			} else if ( filelen == 6 ) {
				 sFileName.Format ( _T("%s(%s%s%sKB)"), FileName, FileSize.Mid (1,1), FileSize.Mid (2,1), FileSize.Mid (3,1) );
			} else if ( filelen == 7 ) {
				sFileName.Format ( _T("%s(%s.%s%s%sKB)"), FileName, FileSize.Mid (1,1), FileSize.Mid (2,1), FileSize.Mid (3,1), FileSize.Mid (4,1) );
			} else if ( filelen == 8 ) {
				sFileName.Format ( _T("%s(%s%s.%s%s%sKB)"), FileName, FileSize.Mid (1,1), FileSize.Mid (2,1), FileSize.Mid (3,1), FileSize.Mid (4,1),FileSize.Mid (5,1) );
			} else if ( filelen == 9 ) {
				sFileName.Format ( _T("%s(%s%s%s.%s%s%sKB)"), FileName, FileSize.Mid (1,1), FileSize.Mid (2,1), FileSize.Mid (3,1), FileSize.Mid (4,1),FileSize.Mid (5,1),FileSize.Mid (6,1) );
			}
			m_List.InsertColumn (0,_T(""));
			m_List.InsertItem (0, sFileName);
		}
	}

	return 0;
}

VOID CLight_UC_NOTIFY_03::OnbFOLDERSELECT() 
{
	
	//사용자가 선택한 폴더 경로 얻어오는 소스.
	ITEMIDLIST        *pidlBrowse;
    TCHAR            pszPathname[MAX_PATH];
 	TCHAR    szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);

	BROWSEINFO BrInfo;
    BrInfo.pidlRoot = NULL;

    memset( &BrInfo, 0, sizeof(BrInfo) );
    BrInfo.pszDisplayName = pszPathname;
    BrInfo.lpszTitle = _T("파일을 저장할 위치를 선택하세요");
    BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

    // 다이얼로그를 띄우기
    pidlBrowse = ::SHBrowseForFolder(&BrInfo);    

    if( pidlBrowse != NULL)
    {
        // 패스를 얻어옴
        ::SHGetPathFromIDList(pidlBrowse, pszPathname);    		
    }
	else return ;

	_tchdir(szCurrentDir);   
    GetDlgItem( IDC_eSHELF_LOC )->SetWindowText( pszPathname );
}

VOID CLight_UC_NOTIFY_03::OnOK() 
{
	CESL_DataMgr     * m_pDataMgr;
	CString            strFilePath;
	CString            FileSize, RecKey;
	CString            FileName, FullpathName;
	INT                ids, RecCnt, i;

	//////////////////////////////////////////////////////////////////////////////////
	// 1. 파일저장경로 설정을 체크한다.
	//////////////////////////////////////////////////////////////////////////////////
	GetDlgItem( IDC_eSHELF_LOC )->GetWindowText( strFilePath );
	if ( 0 == strFilePath.GetLength() ) {
		MessageBox( _T("파일저장경로를 설정하십시오."), _T("파일저장"), MB_OK );
		return;
	}


	m_pDataMgr = FindDM(_T("DM_UC_NOTIFY_FILEINFO"));
	if(m_pDataMgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return ;
	}

	//////////////////////////////////////////////////////////////////////////////////
	// 2. 파일을 SERVER에서 가져온다.
	//////////////////////////////////////////////////////////////////////////////////     
	RecCnt = m_pDataMgr->GetRowCount () ;


	CString result;
	CKolisNetSender sender;
	for (i=0; i<RecCnt; i++)
	{
		CString strFileKey;
		m_pDataMgr->GetCellData(_T("REC_KEY"), i, strFileKey);
		m_pDataMgr->GetCellData(_T("PARENT_KEY"), i, RecKey);
		m_pDataMgr->GetCellData(_T("파일이름"), i, FileName);
		m_pDataMgr->GetCellData(_T("파일사이즈"), i, FileSize);
		if (!strFilePath.IsEmpty() && _T("\\") != strFilePath.Right(1))
		{
			strFilePath += _T("\\");
		}
		FullpathName.Format(_T("%s%s"), strFilePath, FileName);

		if (!CKolisNetService::DownloadNotifyFile(this, _T("UC"), RecKey, strFileKey, FileName, FullpathName, result))
		{
			MessageBox(result, _T("파일저장"), MB_OK);
			OnCancel();
			return;
		}
	}

	CDialog::OnOK();
}

HBRUSH CLight_UC_NOTIFY_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
