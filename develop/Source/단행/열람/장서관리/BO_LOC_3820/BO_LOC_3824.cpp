// BO_LOC_3824.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3824.h"
#include "BO_LOC_38201.h"
#include "webbrowser2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3824 dialog


CBO_LOC_3824::CBO_LOC_3824(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3824)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CBO_LOC_3824::~CBO_LOC_3824()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CBO_LOC_3824::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3824)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3824, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3824)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3824 message handlers

BOOL CBO_LOC_3824::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_3824::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids;

	ids = InitESL_Mgr(_T("SM_BO_LOC_3824"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitESL_Mgr") );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_3824::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(LoadLobData()) return ;

	ShowContents();
}

VOID CBO_LOC_3824::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
}

VOID CBO_LOC_3824::OnbPREV() 
{
	// TODO: Add your control notification handler code here
	
}

VOID CBO_LOC_3824::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

INT CBO_LOC_3824::ShowContents()
{
EFS_BEGIN
	CWebBrowser2 *m_pBrowser = (CWebBrowser2  *)GetDlgItem (IDC_expPoll);
	COleVariant noArg;
	
	CString strPath ;
	//strPath = _T("cfg\\webresearch\\research.htm") ;
	strPath = _T("cfg\\webresearch\\TempPage.htm") ;

	CString  strCurrentDir ;

	TCHAR sDir[1000];
	GetCurrentDirectory (1000,sDir);
	strCurrentDir = sDir;

    //strPath = _T("G:\\KOLAS2UP작업\\Kolas2up\\Cfg\\webresearch\\research.htm") ;
	strCurrentDir.Replace (_T("bin"),strPath.GetBuffer (0)) ;

	m_pBrowser->Navigate(strCurrentDir,&noArg,&noArg,&noArg,&noArg);

	return 0;
EFS_END
	return -1;
}

INT CBO_LOC_3824::LoadLobData()
{
EFS_BEGIN
	INT ids =0 , idx = 0;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3820_POLL") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3824::LoadLobData() := FindDM()") );


	CESL_Grid *pGrid = ( CESL_Grid * )FindControl( _T("CM_BO_LOC_3820"), _T("PollGrid") );

	idx = pGrid->GetMouseRow();
	if (idx < 1) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3824::LoadLobData() := GetMouseRow()") );


	CString strFileName  = _T("..\\cfg\\webresearch\\TempPage.htm") ;
	CFile file;
	INT     nFileSize ;
	CString sTemp;
	CString sRecKey;

	ids = pDM->GetCellData( _T("REC_KEY"), idx-1 , sRecKey );
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3824::LoadLobData() := GetCellData()") );

	ids = pDM->GetCellData( _T("파일크기"), idx-1 , sTemp );
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3824::LoadLobData() := GetCellData()") );

	if(sRecKey.IsEmpty()) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3824::LoadLobData() := IsEmpty()") );

	nFileSize = _ttoi(sTemp);

	if(nFileSize < 1) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBO_LOC_3824::LoadLobData() := nFileSize < 1") );

	INT nBufferSize = sizeof(BYTE) * nFileSize + 1024;
	BYTE *pBuffer = (BYTE*)malloc(nBufferSize);
	ZeroMemory(pBuffer, sizeof(BYTE) * nBufferSize);
	// KOL.UDF.022
	//if (pBuffer == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3824::LoadLobData() := malloc()") );
	if (&pBuffer == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("CBO_LOC_3824::LoadLobData() := malloc()") );

	ids = pDM->LOBGet(_T("RESEARCH_INFO_TBL"),_T("REC_KEY"),_T("NUMERIC"),sRecKey,_T("FILE_DATA"),pBuffer,&nBufferSize) ;
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CBO_LOC_3824::LoadLobData() := LOBGet()") );

	if (!file.Open(strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CBO_LOC_3824::LoadLobData() := Open()") );
	}
	
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	CString zz;
	zz = pBuffer;
	zz.Replace(_T(" type = submit "), _T(" type = hidden "));

	file.WriteHuge(zz.GetBuffer(0), nBufferSize);
	file.Close();	
	free(pBuffer);

	return 0;
EFS_END
	return -1;
}

HBRUSH CBO_LOC_3824::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
