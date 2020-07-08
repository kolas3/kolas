// Light_UC_NOTIFY_03.cpp : implementation file
//

#include "stdafx.h"
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include "UC_NotifyPopup_FileDlg.h"


#include "KolisNetService.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUC_NotifyPopup_FileDlg dialog


CUC_NotifyPopup_FileDlg::CUC_NotifyPopup_FileDlg(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// KOL.UDF.022 시큐어코딩 보완
	m_RowIndex = 0;
	m_nSelectIndex = 0;
}

CUC_NotifyPopup_FileDlg::~CUC_NotifyPopup_FileDlg()
{
	
}

BOOL CUC_NotifyPopup_FileDlg::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CUC_NotifyPopup_FileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUC_NotifyPopup_FileDlg)
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUC_NotifyPopup_FileDlg, CDialog)
	//{{AFX_MSG_MAP(CUC_NotifyPopup_FileDlg)
	ON_BN_CLICKED(IDC_bFOLDER_SELECT, OnbFOLDERSELECT)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUC_NotifyPopup_FileDlg message handlers

BOOL CUC_NotifyPopup_FileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString     RecKey;
	INT         GridRowIndex, ids;
	
	GridRowIndex = m_RowIndex ;
	
	ids = GetDataMgrData(_T("DM_UC_NOTIFY"),_T("REC_KEY"),RecKey,GridRowIndex );

	ids = GetFileInfo( RecKey );
	if ( ids<0 ) {
		AfxMessageBox (_T("GetFileInfo(REC_KEY) Error"), MB_ICONEXCLAMATION);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CUC_NotifyPopup_FileDlg::GetRowIndex(INT index)
{
	m_RowIndex = index ;
	return 0 ;
}

INT CUC_NotifyPopup_FileDlg::GetFileInfo(CString pParentRecKey)
{

	CESL_DataMgr* pDM = FindDM(_T("DM_UC_NOTIFY_FILE"));
	if (pDM == NULL) return -3;

	CKolisNetDataMapper Mapper;
	//Mapper.AddData(KOLISNET_PARAM_UNF_KEY, _T("REC_KEY"));
	Mapper.AddData(KOLISNET_PARAM_UN_KEY, _T("PARENT_KEY"));
	Mapper.AddData(KOLISNET_PARAM_FILE_NAME, _T("파일이름"));
	Mapper.AddData(KOLISNET_PARAM_FILE_SIZE, _T("파일사이즈"));
	Mapper.AddData(KOLISNET_PARAM_FILE_URL, _T("URL"));

	if (!CKolisNetService::GetKolisNetNotifyFileInfo(this, pParentRecKey, pDM, &Mapper))
	{
		return -1;
	}

	INT ids, RecCnt, filelen;
	RecCnt = pDM->GetRowCount();
	CString FileName, FileSize;
    
	INT nCntList = m_List.GetItemCount();
	m_List.DeleteAllItems ();
	
	m_List.InsertColumn (0, _T(""));

	
	if ( RecCnt > 0 ) {

	
		for ( INT i = 0; i < RecCnt; i++ ) {
			ids = GetDataMgrData (_T("DM_UC_NOTIFY_FILE"),_T("파일이름"),FileName,i);
            
			ids = GetDataMgrData (_T("DM_UC_NOTIFY_FILE"),_T("파일사이즈"),FileSize,i);

			if (!FileSize.IsEmpty()) {
				filelen = FileSize.GetLength ();
				CString sFileNameTmp = FileName;
				if ( filelen < 4 ) 
					 FileName.Format ( _T("%s(1KB)"), sFileNameTmp );
				else
					FileName.Format ( _T("%s(%sKB)"), sFileNameTmp, FileSize );

			}
		
			m_List.InsertItem (i, FileName);
		}
	}

	ListView_SetItemState (m_List.GetSafeHwnd(), 0, LVIS_FOCUSED|LVIS_SELECTED, 0x000F);
	
	return 0;
}



VOID CUC_NotifyPopup_FileDlg::OnbFOLDERSELECT() 
{

	m_List.SetFocus();

	POSITION pos = m_List.GetFirstSelectedItemPosition();
	int nIdx = m_List.GetNextSelectedItem(pos);

	if (0 > nIdx) {
		MessageBox( _T("다운로드할 파일을 선택하세요."), _T("다운로드"), MB_OK );
		return;
	}

	CString strURL;
	GetDataMgrData (_T("DM_UC_NOTIFY_FILE"), _T("URL"), strURL, nIdx);
	if (strURL.IsEmpty()) {
		return;
	}

	ShellExecute(0, _T("open"), strURL, 0, 0, 1);
}

CString CUC_NotifyPopup_FileDlg::GetFilePath()
{
	//사용자가 선택한 폴더 경로 얻어오는 소스.
	ITEMIDLIST        *pidlBrowse;
    TCHAR            pszPathname[MAX_PATH];
 	TCHAR    szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);

	BROWSEINFO BrInfo;
 //   BrInfo.hwndOwner = GetSafeHwnd();
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
	else return _T("");

	_tchdir(szCurrentDir);   

	//m_strExportFilePath = pszPathname;

	return pszPathname;

}

VOID CUC_NotifyPopup_FileDlg::OnOK() 
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

	m_pDataMgr = FindDM(_T("DM_UC_NOTIFY_FILE"));
	if(m_pDataMgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return ;
	}

	//////////////////////////////////////////////////////////////////////////////////
	// 2. 파일을 SERVER에서 가져온다.
	//////////////////////////////////////////////////////////////////////////////////
     
	RecCnt = m_pDataMgr->GetRowCount () ;


	CString strResult;
	for (i=0; i<RecCnt; i++)
	{		
		CString strFileKey;
		ids = GetDataMgrData (_T("DM_UC_NOTIFY_FILE"), _T("REC_KEY"), strFileKey,i);
		ids = GetDataMgrData (_T("DM_UC_NOTIFY_FILE"), _T("PARENT_KEY"), RecKey,i);
		ids = GetDataMgrData (_T("DM_UC_NOTIFY_FILE"), _T("파일이름"), FileName,i);
		ids = GetDataMgrData (_T("DM_UC_NOTIFY_FILE"), _T("파일사이즈"), FileSize,i);
		ids = GetDataMgrData (_T("DM_UC_NOTIFY_FILE"), _T("URL"), FileSize,i);
	
		FullpathName.Format ( _T("%s\\%s"), strFilePath, FileName );

		if (!CKolisNetService::DownloadNotifyFile(this, _T("UC"), RecKey, strFileKey, FileName, FullpathName, strResult))
		{
			AfxMessageBox(strResult);
			continue;
		}
	}
	AfxMessageBox(_T("저장하였습니다."));

	CDialog::OnOK();
}

HBRUSH CUC_NotifyPopup_FileDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CUC_NotifyPopup_FileDlg::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	// 여기에 선택된 파일의 URL표시 
	
	*pResult = 0;
}
