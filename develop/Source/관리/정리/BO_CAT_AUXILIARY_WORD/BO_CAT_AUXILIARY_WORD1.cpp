// BO_CAT_AUXILIARY_WORD1.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_AUXILIARY_WORD1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_AUXILIARY_WORD dialog


CBO_CAT_AUXILIARY_WORD::CBO_CAT_AUXILIARY_WORD(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_AUXILIARY_WORD)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_AUXILIARY_WORD::~CBO_CAT_AUXILIARY_WORD()
{
}

BOOL CBO_CAT_AUXILIARY_WORD::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}


VOID CBO_CAT_AUXILIARY_WORD::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_AUXILIARY_WORD)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_AUXILIARY_WORD, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_AUXILIARY_WORD)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_AUXILIARY_WORD message handlers

BOOL CBO_CAT_AUXILIARY_WORD::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_AUXILIARY")) < 0)
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

VOID CBO_CAT_AUXILIARY_WORD::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gAuxiliaryWord);
	if (pGrid->GetSafeHwnd() == NULL) return;

	pGrid->MoveWindow(0, 0, cx, cy);

EFS_END
}

INT CBO_CAT_AUXILIARY_WORD::Delete()
{
EFS_BEGIN

	CESL_ControlMgr *pCM;
	pCM = FindCM( _T("CM_BO_CAT_AUXILIARY") );
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_AUXILIARY ERROR") );
		return -1;
	}

	SelectMakeList(_T("CM_BO_CAT_AUXILIARY"), _T("GRD_AUXILIARY"));
	INT idx = SelectGetTailPosition(_T("CM_BO_CAT_AUXILIARY"), _T("GRD_AUXILIARY"));
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;
	}

	if (MessageBox(_T("선정한 자료를 삭제하시겠습니까?"), _T("삭제 확인"), MB_YESNO) == IDYES)
	{
		CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_AUXILIARY"));
		pDM->StartFrame();

		CString sAccRecKey, sAccRecKeyTmp;
		do
		{
			CString strCondition;
			pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

			sAccRecKeyTmp += _T(",") + strCondition;

			pDM->MakeDeleteFrame(_T("CO_AUXILIARY_WORD_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
			pDM->DeleteRow(idx);

			idx = SelectGetPrev(_T("CM_BO_CAT_AUXILIARY"), _T("GRD_AUXILIARY"));			
		}		
		while (idx >= 0);

		INT ids = pDM->SendFrame();
		if (ids < 0) {
			AfxMessageBox(_T("삭제사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
			return -1;
		}
		pDM->EndFrame();
	}
	AllControlDisplay(_T("CM_BO_CAT_AUXILIARY"));

	CreateFile();

	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_AUXILIARY_WORD::CreateFile()
{
	EFS_BEGIN

	CESL_DataMgr *pDM;
	pDM = FindDM( _T("DM_BO_CAT_AUXILIARY_FILE") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_AUXILIARY_FILE error") );
		return;
	}

	CESL_DataMgr *m_pDM_Manage;
	m_pDM_Manage = FindDM( _T("DM_BO_CAT_AUXILIARY_FILE") );
	if ( m_pDM_Manage == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_AUXILIARY_FILE error") );
		return;
	}

	CESL_DataMgr DmCharMng;
	DmCharMng.SetCONNECTION_INFO(pDM->CONNECTION_INFO);	
	CString sQuery = _T("SELECT REC_KEY, VERSION FROM CO_CHARSET_MANAGEMENT_TBL WHERE CHARSET_NAME = '조사'");
	DmCharMng.RestructDataManager(sQuery);	
	CString strRecKey = DmCharMng.GetCellData( 0, 0 );
	CString strVersion = DmCharMng.GetCellData( 0, 1 );
	INT iVersion = _ttoi( strVersion );
	iVersion++;
	strVersion.Format( _T("%d"), iVersion );
	
	CString strWhere = _T("");
	if (pDM->RefreshDataManager(strWhere) < 0)	return;

	CString sPath = _T("..\\CFG\\CO_AUXILIARY_WORD_TBL.txt");

	FILE *fp;

	fp = _tfopen( sPath , _T("wb") );
	if ( fp == NULL ) {
		AfxMessageBox( _T("CO_AUXILIARY_WORD_TBL.txt 파일이 없습니다.") );
		return;
	}	
	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}		
	
	INT iRowCount = pDM->GetRowCount();
	_ftprintf( fp, _T(".FILE_VERSION =  \r\n") );
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sRoleWord = pDM->GetCellData( _T("조사"), i );
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

	EFS_END
}

HBRUSH CBO_CAT_AUXILIARY_WORD::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
