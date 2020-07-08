// BO_CAT_UNLESS_WORD_0000.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_UNLESS_WORD_0000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_UNLESS_WORD_0000 dialog


CBO_CAT_UNLESS_WORD_0000::CBO_CAT_UNLESS_WORD_0000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_CAT_UNLESS_WORD_0000::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_UNLESS_WORD_0000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_UNLESS_WORD_0000::~CBO_CAT_UNLESS_WORD_0000()
{
}
BOOL CBO_CAT_UNLESS_WORD_0000::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_UNLESS_WORD_0000::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_UNLESS_WORD_0000)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_UNLESS_WORD_0000, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_UNLESS_WORD_0000)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_UNLESS_WORD_0000 message handlers

BOOL CBO_CAT_UNLESS_WORD_0000::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_UNLESS_WORD_0000")) < 0)
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

VOID CBO_CAT_UNLESS_WORD_0000::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gUnlessWord);
	if (pGrid->GetSafeHwnd() == NULL) return;

//	CButton *pButton = (CButton*)GetDlgItem(IDC_bDELETE);
//	if (pButton->GetSafeHwnd() == NULL) return;
	
//	pButton->SetWindowPos(NULL, cx/2, 0, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	pGrid->MoveWindow(0, 0, cx, cy);

EFS_END
}

INT CBO_CAT_UNLESS_WORD_0000::Delete()
{
EFS_BEGIN

	//CESL_DataMgr *pCM;
	CESL_ControlMgr *pCM;
	pCM = FindCM( _T("CM_BO_CAT_UNLESS_WORD_0000") );
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_UNLESS_WORD_0000 ERROR") );
		return -1;
	}

	SelectMakeList(_T("CM_BO_CAT_UNLESS_WORD_0000"), _T("GRD_UNLESS_WORD"));
	INT idx = SelectGetTailPosition(_T("CM_BO_CAT_UNLESS_WORD_0000"), _T("GRD_UNLESS_WORD"));
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
		return -1;
	}

	if (MessageBox(_T("선정한 자료를 삭제하시겠습니까?"), _T("삭제 확인"), MB_YESNO) == IDYES)
	{
		CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_UNLESS_WORD_0000"));
		pDM->StartFrame();

		CString sAccRecKey, sAccRecKeyTmp;
		do
		{
			CString strCondition;
			pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

			sAccRecKeyTmp += _T(",") + strCondition;		//키 조합

			pDM->MakeDeleteFrame(_T("CO_UNLESS_WORD_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
			pDM->DeleteRow(idx);

			idx = SelectGetPrev(_T("CM_BO_CAT_UNLESS_WORD_0000"), _T("GRD_UNLESS_WORD"));			
		}		
		while (idx >= 0);
/*
		sAccRecKey = sAccRecKeyTmp.Mid(1);
		CString sQuery;
		sQuery.Format( _T("DELETE FROM CO_UNLESS_WORD_TBL WHERE REC_KEY IN (%s);") , sAccRecKey );
		INT iCh = pDM->AddFrame(sQuery);
*/
		INT ids = pDM->SendFrame();
		if (ids < 0) {
			AfxMessageBox(_T("삭제사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
			return -1;
		}
		pDM->EndFrame();
	}
	AllControlDisplay(_T("CM_BO_CAT_UNLESS_WORD_0000"));

	//파일생성
	CreateFile();

	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_UNLESS_WORD_0000::CreateFile()
{
	EFS_BEGIN

	CESL_DataMgr *pDM;
	pDM = FindDM( _T("DM_BO_CAT_UNLESS_WORD_0000_FILE") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_UNLESS_WORD_0000_FILE error") );
		return;
	}

	CESL_DataMgr *m_pDM_Manage;
	m_pDM_Manage = FindDM( _T("DM_BO_CAT_UNLESS_WORD_0000_FILE") );
	if ( m_pDM_Manage == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_UNLESS_WORD_0000_FILE error") );
		return;
	}

	CESL_DataMgr DmCharMng;
	DmCharMng.SetCONNECTION_INFO(pDM->CONNECTION_INFO);	
	CString sQuery = _T("SELECT REC_KEY, VERSION FROM CO_CHARSET_MANAGEMENT_TBL WHERE CHARSET_NAME = '불용어'");
	DmCharMng.RestructDataManager(sQuery);	
	CString strRecKey = DmCharMng.GetCellData( 0, 0 );
	CString strVersion = DmCharMng.GetCellData( 0, 1 );
	INT iVersion = _ttoi( strVersion );
	iVersion++;
	strVersion.Format( _T("%d"), iVersion );
		
	CString strWhere = _T("");
	if (pDM->RefreshDataManager(strWhere) < 0)	return;

	CString sPath = _T("..\\CFG\\CO_UNLESS_WORD_TBL.txt");
	//파일읽어오기
	FILE *fp;
	//TCHAR cLine[256];
	fp = _tfopen( sPath , _T("wb") );
	if ( fp == NULL ) {
		AfxMessageBox( _T("CO_UNLESS_WORD_TBL.txt 파일이 없습니다.") );
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
		CString sRoleWord = pDM->GetCellData( _T("불용어"), i );
		CString sLine = sRoleWord + _T("                          |Y\r\n");
		_ftprintf( fp, sLine );
	}
	fclose( fp );


	// 관리 테이블에 파일, 파일길이, 버전정보를 업데이트 한다.

	// 파일을 통채로 BLOB으로 DB에 올린다.
	CStdioFile file;
	INT nFileSize = 0;
	//if (!file.Open(strTempFileName, CFile::modeRead))
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
	// 파일
	//INT ids = m_pDM_Manage->LOBUpdate(m_pDM_Manage->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey, _T("FILE_DATA"), pBuffer, nFileSize);
	INT ids = m_pDM_Manage->LOBUpdate( _T("CO_CHARSET_MANAGEMENT_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey, _T("FILE_DATA"), pBuffer, nFileSize);
	if (ids < 0)
	{
		// 메모리 해제
		free(pBuffer);
		MessageBox(_T("변경적용 실패"));
		return;
	}
	
	// 파일길이, 버전정보
	CString strLen;
	strLen.Format(_T("%d"), nFileSize);

	ids = m_pDM_Manage->StartFrame();	
	
	m_pDM_Manage->InitDBFieldData();
	m_pDM_Manage->AddDBFieldData(_T("VERSION"), _T("NUMERIC"), strVersion);
	m_pDM_Manage->AddDBFieldData(_T("FILE_LEN"), _T("NUMERIC"), strLen);
	//m_pDM_Manage->AddDBFieldData(_T("FILE_NAME"), _T("STRING"), m_pDM_Manage->GetCellData(_T("TABLE_NAME"), nManagementDMRow) + _T(".TXT"));
	//m_pDM_Manage->MakeUpdateFrame(m_pDM_Manage->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey, nManagementDMRow);
	m_pDM_Manage->MakeUpdateFrame( _T("CO_CHARSET_MANAGEMENT_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey );
	//INSERT INTO CO_CHARSET_MANAGEMENT_TBL ('50364199', '조사', '1', '조사', '조사', 'CO_AUXILIARY_WORD_TBL', 'CO_AUXILIARY_WORD_TBL.TXT', 
	
	ids = m_pDM_Manage->SendFrame();
	if (ids < 0) 
	{
		// 메모리 해제
		free(pBuffer);
		MessageBox(_T("변경적용 실패했습니다."));
		return;
	}
	ids = m_pDM_Manage->EndFrame();
	if (ids < 0) 
	{
		// 메모리 해제
		free(pBuffer);
		MessageBox(_T("변경적용 실패했습니다."));
		return;
	}

	EFS_END
}
HBRUSH CBO_CAT_UNLESS_WORD_0000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
