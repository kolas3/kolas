// TestShare.cpp : implementation file
//

#include "stdafx.h"
#include "TestShare.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestShare dialog


CTestShare::CTestShare(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestShare)
	m_sNTYPE = _T("");
	m_sCOMMAND = _T("");
	m_sMASTERCHANGEYN = _T("");
	m_sSTYPE = _T("");
	m_sUPDATEFIELD = _T("");
	m_sUPDATEDLIBCODE = _T("");
	m_sUPDATETABLERECKEY = _T("");
	m_edtUPDATESETQUERY = _T("");
	m_sUPDATETABLENAME = _T("");
	m_sDUPDATETABLERECKEY = _T("");
	//}}AFX_DATA_INIT


	
}

CTestShare::~CTestShare()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
	delete m_pShareManager;
}


VOID CTestShare::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestShare)
	DDX_Text(pDX, IDC_edtNTYPE, m_sNTYPE);
	DDX_Text(pDX, IDC_edtSCOMMAND, m_sCOMMAND);
	DDX_Text(pDX, IDC_edtSMASTERUSERINFOCHANGEYN, m_sMASTERCHANGEYN);
	DDX_Text(pDX, IDC_edtSTYPE, m_sSTYPE);
	DDX_Text(pDX, IDC_edtSUPDATEDFIELDS, m_sUPDATEFIELD);
	DDX_Text(pDX, IDC_edtSUPDATELIBCODE, m_sUPDATEDLIBCODE);
	DDX_Text(pDX, IDC_edtSUPDATETABLERECKEY, m_sUPDATETABLERECKEY);
	DDX_Text(pDX, IDC_edtUPDATESETQUERY, m_edtUPDATESETQUERY);
	DDX_Text(pDX, IDC_edtUPDATETABLENAME, m_sUPDATETABLENAME);
	DDX_Text(pDX, IDC_UPDATEDTABLERECKEY, m_sDUPDATETABLERECKEY);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CTestShare, CDialog)
	//{{AFX_MSG_MAP(CTestShare)
	ON_BN_CLICKED(IDC_btnTEST2, OnbtnTEST2)
	ON_BN_CLICKED(IDC_btnTEST3, OnbtnTEST3)
	ON_BN_CLICKED(IDC_btnTESTO, OnbtnTESTO)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestShare message handlers
BOOL CTestShare::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

		
	return CDialog::Create(IDD, pParentWnd);
	

	EFS_END
	return FALSE;

}

BOOL CTestShare::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	INT ids =  InitESL_Mgr(_T("SM_TEST_LOAN_SHARE"));
	if( ids < 0 ) AfxMessageBox(_T("InitERROR"));

/*
//	-- 대출자
	m_sNTYPE = _T("0");
	m_sSTYPE = _T("I");
	m_sCOMMAND = _T("11");
	m_sMASTERCHANGEYN = _T("Y");
	m_sUPDATETABLENAME=_T("CO_LOAN_USER_TBL");
	m_sUPDATETABLERECKEY=_T("60134881");
	m_sUPDATEFIELD = _T("");
	
	m_sUPDATEDLIBCODE = _T("");
	m_sDUPDATETABLERECKEY = _T("");
	m_edtUPDATESETQUERY = _T("");
*/

//	-- 직급별 반납연기
	m_sNTYPE = _T("0");
	m_sSTYPE = _T("I");
	m_sCOMMAND = _T("61");
	m_sMASTERCHANGEYN = _T("");
	m_sUPDATETABLENAME=_T("CD_CODE_TBL");
	m_sUPDATETABLERECKEY=_T("50291787");
	m_sUPDATEFIELD = _T("");
	
	m_sUPDATEDLIBCODE = _T("");
	m_sDUPDATETABLERECKEY = _T("");
	m_edtUPDATESETQUERY = _T("");

/*
//	-- 직급별 등록구분별 반납연기
	m_sNTYPE = _T("0");
	m_sSTYPE = _T("I");
	m_sCOMMAND = _T("62");
	m_sMASTERCHANGEYN = _T("");
	m_sUPDATETABLENAME=_T("MN_LOAN_ENV_POSREG_TBL");
	m_sUPDATETABLERECKEY=_T("61317360");
	m_sUPDATEFIELD = _T("");
	
	m_sUPDATEDLIBCODE = _T("");
	m_sDUPDATETABLERECKEY = _T("");
	m_edtUPDATESETQUERY = _T("");
*/

/*
//	-- 휴관일 정보
	m_sNTYPE = _T("0");
	m_sSTYPE = _T("I");
	m_sCOMMAND = _T("63");
	m_sMASTERCHANGEYN = _T("");
	m_sUPDATETABLENAME=_T("MN_LIB_HOLIDAY_TBL");
	m_sUPDATETABLERECKEY=_T("61044108");
	m_sUPDATEFIELD = _T("");
	
	m_sUPDATEDLIBCODE = _T("");
	m_sDUPDATETABLERECKEY = _T("");
	m_edtUPDATESETQUERY = _T("");
*/
/*
//	-- 공통적인 반납연기 정보
	m_sNTYPE = _T("0");
	m_sSTYPE = _T("I");
	m_sCOMMAND = _T("64");
	m_sMASTERCHANGEYN = _T("");
	m_sUPDATETABLENAME=_T("ESL_MANAGE_TBL");
	m_sUPDATETABLERECKEY=_T("60140454");
	m_sUPDATEFIELD = _T("");
	
	m_sUPDATEDLIBCODE = _T("");
	m_sDUPDATETABLERECKEY = _T("");
	m_edtUPDATESETQUERY = _T("");
*/	
	
	UpdateData(FALSE);

	m_pShareManager = new CLoanShareManager(this);
	m_pShareManager->Create(this);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}


INT CTestShare::Test()
{
	EFS_BEGIN

	UpdateData(TRUE);

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = _ttoi(m_sNTYPE);

	UploadShareInfo.UploadShareLoanInfo.sType = m_sSTYPE;
	UploadShareInfo.UploadShareLoanInfo.sCommand = m_sCOMMAND;
	UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = m_sMASTERCHANGEYN;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = m_sUPDATETABLENAME;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = m_sUPDATETABLERECKEY;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = m_sUPDATEFIELD;
	
	UploadShareInfo.UploadShareDelayInfo.sUpdateLibCode = m_sUPDATEDLIBCODE;
	UploadShareInfo.UploadShareDelayInfo.sUpdatedTableRecKey = m_sDUPDATETABLERECKEY;
	UploadShareInfo.UploadShareDelayInfo.sUpdatedSetQuery = m_edtUPDATESETQUERY;
	

	m_pShareManager->SendLoanInfoUpload( UploadShareInfo );
		

	return 0;

	

	EFS_END
	return -1;

}


VOID CTestShare::OnbtnTEST2() 
{
	AllUserSend();	
}

INT CTestShare::AllUserSend()
{
	EFS_BEGIN

	CESL_DataMgr* pDM = FindDM( _T("DM_TEST_LOAN_SHARE_USER") );

	pDM->RefreshDataManager(_T("REC_KEY NOT IN ( 60058609 , 60089942 , 60147035 )"));

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	
	UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
	UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");

	CString sRecKey;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("REC_KEY") , i , sRecKey );

		if( sRecKey.IsEmpty() ) continue;
			
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		m_pShareManager->SendLoanInfoUpload( UploadShareInfo );
	}



	return 0;

	EFS_END
	return -1;
}

VOID CTestShare::OnbtnTEST3() 
{
	AllLoanSend();
	
}


INT CTestShare::AllLoanSend()
{
	EFS_BEGIN
		
	CESL_DataMgr* pDM = FindDM( _T("DM_TEST_LOAN_SHARE_INFO") );
	
	pDM->RefreshDataManager(_T(""));
	
	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	
	UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
//	UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	
	CString sRecKey;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("REC_KEY") , i , sRecKey );
		
		if( sRecKey.IsEmpty() ) continue;

		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		m_pShareManager->SendLoanInfoUpload( UploadShareInfo );
	}
	
	
	
	return 0;
	
	EFS_END
	return -1;
}
VOID CTestShare::OnbtnTESTO() 
{
	EFS_BEGIN
		
	Test();
	
	EFS_END
		
}

HBRUSH CTestShare::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// 여기까지 추가

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
