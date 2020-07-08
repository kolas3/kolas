// BO_CAT_ROLE_WORD_0300.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_ROLE_WORD_0300.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_ROLE_WORD_0300 dialog


CBO_CAT_ROLE_WORD_0300::CBO_CAT_ROLE_WORD_0300(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_ROLE_WORD_0300)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_ROLE_WORD_0300::~CBO_CAT_ROLE_WORD_0300()
{
}

BOOL CBO_CAT_ROLE_WORD_0300::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_ROLE_WORD_0300::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_ROLE_WORD_0300)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_ROLE_WORD_0300, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_ROLE_WORD_0300)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_ROLE_WORD_0300 message handlers

BOOL CBO_CAT_ROLE_WORD_0300::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_MNG_ROLE_0300")) < 0)	
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	Display(0);		

	//CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_MNG_ROLE"));
	if ( pDM == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_MNG_ROLE ERROR") );
		m_bInitDlg = FALSE;
		return FALSE;
	}
	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );

	CString sWord;
	sWord = pDM->GetCellData( _T("역할어"), m_idx );
	SetControlData( _T("CM_BO_CAT_MNG_ROLE_0300"), _T("역할어"), sWord );

	if ( sRecKey.IsEmpty() ) {
		m_bInitDlg = FALSE;
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_ROLE_WORD_0300::Display(INT nDir)
{
EFS_BEGIN

	INT idx;
	switch(nDir)
	{
	case 0:
		SelectMakeList(_T("CM_BO_CAT_MNG_ROLE"), _T("GRD_ROLE"));
		idx = SelectGetHeadPosition(_T("CM_BO_CAT_MNG_ROLE"), _T("GRD_ROLE"));
		if (idx < 0)
		{
			AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
			EndDialog(IDCANCEL);
			return;
		}
		break;
	case -1:
		idx = SelectGetPrev(_T("CM_BO_CAT_MNG_ROLE"), _T("GRD_ROLE"));
		if (idx == -1) {
			idx = SelectGetHeadPosition(_T("CM_BO_CAT_MNG_ROLE"), _T("GRD_ROLE"));
			AfxMessageBox(_T("처음 자료입니다."));
			return;
		}
		break;
	case 1:
		idx = SelectGetNext(_T("CM_BO_CAT_MNG_ROLE"), _T("GRD_ROLE"));
		if (idx == -1) {
			idx = SelectGetTailPosition(_T("CM_BO_CAT_MNG_ROLE"), _T("GRD_ROLE"));
			AfxMessageBox(_T("마지막 자료입니다."));
			return;
		}
		break;
	}
	
	m_idx = idx; 
	AllControlDisplay(_T("CM_BO_CAT_MNG_ROLE"), m_idx);
	SetReverse(_T("CM_BO_CAT_MNG_ROLE"), _T("GRD_ROLE"), m_idx);

EFS_END
}

VOID CBO_CAT_ROLE_WORD_0300::OnbPREV() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	Display(-1);		
	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );

	CString sWord;
	sWord = pDM->GetCellData( _T("역할어"), m_idx );
	SetControlData( _T("CM_BO_CAT_MNG_ROLE_0300"), _T("역할어"), sWord );

EFS_END
}

VOID CBO_CAT_ROLE_WORD_0300::OnbNEXT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	Display(1);
	sRecKey = pDM->GetCellData( _T("REC_KEY") , m_idx );

	CString sWord;
	sWord = pDM->GetCellData( _T("역할어"), m_idx );
	SetControlData( _T("CM_BO_CAT_MNG_ROLE_0300"), _T("역할어"), sWord );

EFS_END
}

VOID CBO_CAT_ROLE_WORD_0300::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here
	INT ids;
    CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_MNG_ROLE"));	
	if (!pDM) return;
	
	//역할어 중복검사
	CString sWord;
	GetControlData( _T("CM_BO_CAT_MNG_ROLE_0300"), _T("역할어"), sWord );
	INT iRowCountRole = pDM->GetRowCount();		
	for ( INT i = 0 ; i < iRowCountRole ; i++ ) {
		CString sDmWord = pDM->GetCellData( _T("역할어"), i );
		if ( sDmWord == sWord ) {
			AfxMessageBox( _T("이미 입력한 역할어입니다.") );
			return;
		}
	}

	if ( sWord == _T("") ) {
		AfxMessageBox( _T("역할어를 입력하세요.") );
		return;
	}

	///수정 작업 시작 
	CString strID;
	//pCESLMgr->GetControlData(_T("CM_BO_CAT_REG_0520"), _T("등록작업자"), strID);
	strID = pDM->GetCellData ( _T("REC_KEY") ,m_idx );

	pDM->StartFrame();
	
	///수정 작업 시작  
	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_CAT_MNG_ROLE_0300"));
	ids = pDM->MakeControlMgrFiledData((CObject*)pCM);
	ids = pDM->MakeControlMgrUpdateFrame(_T("REC_KEY"), _T("STRING"), strID, m_idx);

	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("기증자관리"), __WFILE__, __LINE__), TRUE);

	//ids = pDM->MakeControlMgrUpdateFrame(_T("REGISTRANT"), _T("STRING"), strID, idx);
	//ids = pDM->MakeControlMgrUpdateFrame(_T("등록작업자"), _T("STRING"), strID);
   	///수정 작업 완료 


	ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("수정사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return;
	}
  
	/*
	CBO_CAT_REG_0520 * pREG_0520;
	INT tmp = pREG_0520->m
	*/
	
	//pCESLMgr->AllControlDisplay(_T("DM_BO_CAT_REG_0500"));
	CESL_ControlMgr *pCM1 = FindCM(_T("CM_BO_CAT_MNG_ROLE"));
	AllControlDisplay(_T("DM_BO_CAT_MNG_ROLE"));	
	pCM1->AllControlDisplay(m_idx);
 
	//파일에 쓰기
	//CESL_DataMgr *pDM;
	pDM = FindDM( _T("DM_BO_CAT_MNG_ROLE_FILE") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_MNG_ROLE error") );
		return;
	}
	//strWhere = _T("ROLE_WORD LIKE '") + sWord + _T("%'");
	CString strWhere = _T("");
	if (pDM->RefreshDataManager(strWhere) < 0)	return;

	//드라이브 구해오기
	/*
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
*/
	CString sPath = _T("..\\CFG\\CO_ROLE_WORD_TBL.txt");
	//파일읽어오기
	FILE *fp;
	//TCHAR cLine[256];
	fp = _tfopen( sPath , _T("wb") );
	if ( fp == NULL ) {
		AfxMessageBox( _T("CO_ROLE_WORD_TBL.txt 파일이 없습니다.") );
		return;
	}		
	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}

	INT iRowCount = pDM->GetRowCount();
	_ftprintf( fp, _T(".FILE_VERSION = 1 \n\r") );
	for ( i = 0 ; i < iRowCount ; i++ ) {
		CString sRoleWord = pDM->GetCellData( _T("역할어"), i );
		CString sLine = sRoleWord + _T("                          |Y\n\r");
		_ftprintf( fp, sLine );
	}
	fclose( fp );

	//DestroyWindow();
	CDialog::OnOK();

EFS_END
}

HBRUSH CBO_CAT_ROLE_WORD_0300::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
