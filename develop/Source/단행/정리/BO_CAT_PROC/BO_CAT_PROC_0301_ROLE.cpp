// BO_CAT_PROC_0301_ROLE.cpp : implementation file
//

#include "stdafx.h"

#include "BO_CAT_PROC_0301_ROLE.h"
#include "BO_CAT_PROC_0301_ROLE_01.h"
#include "BO_CAT_PROC_0301_ROLE_02.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0301_ROLE dialog


CBO_CAT_PROC_0301_ROLE::CBO_CAT_PROC_0301_ROLE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0301_ROLE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0301_ROLE::~CBO_CAT_PROC_0301_ROLE()
{
}

VOID CBO_CAT_PROC_0301_ROLE::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0301_ROLE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0301_ROLE, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0301_ROLE)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bINPUT, OnbINPUT)
	ON_BN_CLICKED(IDC_bCat0301DELETE, OnbCat0301DELETE)
	ON_BN_CLICKED(IDC_bUPDATE, OnbUPDATE)
	ON_BN_CLICKED(IDC_0301CANCEL, On0301cancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0301_ROLE message handlers

BOOL CBO_CAT_PROC_0301_ROLE::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0301_ROLE::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_MNG_ROLE_POPUP")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0301_ROLE::OnbSEARCH() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sWord;
	GetControlData( _T("CM_BO_CAT_MNG_ROLE_POPUP"), _T("역할어"), sWord );
	if ( sWord.IsEmpty() ){
		AfxMessageBox( _T("역할어를 입력하세요.") );
		return;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM( _T("DM_BO_CAT_MNG_ROLE") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_MNG_ROLE error") );
		return;
	}

	CString strWhere;
	strWhere = _T("ROLE_WORD >= '") + sWord + _T("'");
	if (pDM->RefreshDataManager(strWhere) < 0)	return;
	if ( pDM->GetRowCount() < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_MNG_ROLE_POPUP"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_MNG_ROLE error") );
		return;
	}
	pCM->AllControlDisplay();

EFS_END
}

VOID CBO_CAT_PROC_0301_ROLE::OnbINPUT() 
{
EFS_BEGIN

	CBO_CAT_PROC_0301_ROLE_01 dlg(this);
	if ( dlg.DoModal() == IDOK ) {
		m_bModifyFlag = true;
	}

EFS_END
}

VOID CBO_CAT_PROC_0301_ROLE::OnbCat0301DELETE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_ControlMgr *pCM;
	pCM = FindCM( _T("CM_BO_CAT_MNG_ROLE_POPUP") );
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_MNG_ROLE ERROR") );
		return ;
	}

	SelectMakeList(_T("CM_BO_CAT_MNG_ROLE_POPUP"), _T("GRD_ROLE"));
	INT idx = SelectGetTailPosition(_T("CM_BO_CAT_MNG_ROLE_POPUP"), _T("GRD_ROLE"));
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}

	if (MessageBox(_T("선정한 자료를 삭제하시겠습니까?"), _T("삭제 확인"), MB_YESNO) == IDYES)
	{
		CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_MNG_ROLE"));
		INT ids = pDM->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return;
		}


		CString sAccRecKey, sAccRecKeyTmp;
		do
		{
			CString strCondition;
			pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

			sAccRecKeyTmp += _T(",") + strCondition;

			pDM->MakeDeleteFrame(_T("CO_ROLE_WORD_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
			pDM->DeleteRow(idx);

			idx = SelectGetPrev(_T("CM_BO_CAT_MNG_ROLE_POPUP"), _T("GRD_AUXILIARY"));			
		}		
		while (idx > 0);

		sAccRecKey = sAccRecKeyTmp.Mid(1);
		CString sQuery;
		sQuery.Format( _T("DELETE FROM CO_ROLE_WORD_TBL WHERE REC_KEY IN (%s);") , sAccRecKey );
		INT iCh = pDM->AddFrame(sQuery);

		ids = pDM->SendFrame();
		if (ids < 0) {
			AfxMessageBox(_T("삭제사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
			return ;
		}
		ids = pDM->EndFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("EndFrame 를 확인하세요"));
			return;
		}

		m_bModifyFlag = true;
	}
	AllControlDisplay(_T("CM_BO_CAT_MNG_ROLE_POPUP"));

EFS_END
}

VOID CBO_CAT_PROC_0301_ROLE::OnbUPDATE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CBO_CAT_PROC_0301_ROLE_02 dlg(this);
	if ( dlg.DoModal() == IDOK ) {
		m_bModifyFlag = true;
	}

EFS_END
}

VOID CBO_CAT_PROC_0301_ROLE::On0301cancel() 
{
EFS_BEGIN

	if ( m_bModifyFlag > 0 ) {
		CESL_DataMgr *pDM;
		pDM = FindDM( _T("DM_BO_CAT_MNG_ROLE") );
		if ( pDM == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_MNG_ROLE error") );
			return;
		}

		CESL_DataMgr *m_pDM_Manage;
		m_pDM_Manage = FindDM( _T("DM_BO_CAT_MNG_ROLE") );
		if ( m_pDM_Manage == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_MNG_ROLE error") );
			return;
		}
	
		CESL_DataMgr DmCharMng;
		DmCharMng.SetCONNECTION_INFO(pDM->CONNECTION_INFO);	
		CString sQuery = _T("SELECT REC_KEY, VERSION FROM CO_CHARSET_MANAGEMENT_TBL WHERE CHARSET_NAME = '역할어'");
		DmCharMng.RestructDataManager(sQuery);	
		CString strRecKey = DmCharMng.GetCellData( 0, 0 );
		CString strVersion = DmCharMng.GetCellData( 0, 1 );
		INT iVersion = _ttoi( strVersion );
		iVersion++;
		strVersion.Format( _T("%d"), iVersion );
		
		CString strWhere = _T("");
		if (pDM->RefreshDataManager(strWhere) < 0)	return;
	
		INT curdrive;	
		curdrive = _getdrive();
		CString sPath;
		if(curdrive==3){
			sPath=_T("C:\\KOLAS2UP\\CFG\\CO_ROLE_WORD_TBL.txt");
		}
		if(curdrive==4){
			sPath=_T("D:\\KOLAS2UP\\CFG\\CO_ROLE_WORD_TBL.txt");
		}
		if(curdrive==5){
			sPath=_T("E:\\KOLAS2UP\\CFG\\CO_ROLE_WORD_TBL.txt");
		}

		sPath = _T("..\\CFG\\CO_ROLE_WORD_TBL.txt");

		FILE *fp;
		fp = _tfopen( sPath , _T("wb") );
		if ( fp == NULL ) {
			AfxMessageBox( _T("CO_ROLE_WORD_TBL.txt 파일이 없습니다.") );
			return;
		}		
		
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
					
		INT iRowCount = pDM->GetRowCount();
		_ftprintf( fp, _T(".FILE_VERSION =  \r\n") );
		for ( INT i = 0 ; i < iRowCount ; i++ ) {
			CString sRoleWord = pDM->GetCellData( _T("역할어"), i );
			CString sLine = sRoleWord + _T("                          |Y\r\n");
			_ftprintf( fp, sLine );
		}
		fclose( fp );

		CStdioFile file;
		INT nFileSize = 0;
		if (!file.Open(sPath, CFile::modeRead | CFile::typeBinary))
		{
			MessageBox(_T("파일을 여는데 실패했습니다."));
			return;
		}
		
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		
		nFileSize = file.GetLength();
		BYTE *pBuffer = (BYTE*)malloc(sizeof(BYTE) * (nFileSize) + 1024);
		ZeroMemory(pBuffer, nFileSize);
		nFileSize = file.ReadHuge(pBuffer, nFileSize);
		file.Close();

		CString sTmp = m_pDM_Manage->TBL_NAME;
		
		INT ids = m_pDM_Manage->LOBUpdate( _T("CO_CHARSET_MANAGEMENT_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey, _T("FILE_DATA"), pBuffer, nFileSize);
		if (ids < 0)
		{
			free(pBuffer);
			MessageBox(_T("변경적용 실패"));
			return;
		}
		
		CString strLen;
		strLen.Format(_T("%d"), nFileSize);

		ids = m_pDM_Manage->StartFrame();	
		
		m_pDM_Manage->InitDBFieldData();
		m_pDM_Manage->AddDBFieldData(_T("VERSION"), _T("NUMERIC"), strVersion);
		m_pDM_Manage->AddDBFieldData(_T("FILE_LEN"), _T("NUMERIC"), strLen);
		m_pDM_Manage->MakeUpdateFrame( _T("CO_CHARSET_MANAGEMENT_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey );
		
		ids = m_pDM_Manage->SendFrame();
		if (ids < 0) 
		{
			free(pBuffer);
			MessageBox(_T("변경적용 실패했습니다."));
			return;
		}
		ids = m_pDM_Manage->EndFrame();
		if (ids < 0) 
		{
			free(pBuffer);
			MessageBox(_T("변경적용 실패했습니다."));
			return;
		}
	}


	EndDialog(IDCANCEL);

EFS_END
}

BOOL CBO_CAT_PROC_0301_ROLE::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)	
	{
		OnbSEARCH();
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
HBRUSH CBO_CAT_PROC_0301_ROLE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
