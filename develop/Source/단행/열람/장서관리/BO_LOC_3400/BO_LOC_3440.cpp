// BO_LOC_3440.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3440.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3440 dialog

CBO_LOC_3440::CBO_LOC_3440(CESL_Mgr* pParent, CLonaManageValue* pLoanManageValue)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3440)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLoanManageValue = pLoanManageValue;
	m_pSpfWork = NULL;
	m_pLoanShareManager = NULL;

}

CBO_LOC_3440::~CBO_LOC_3440()
{

}
CBO_LOC_3440::CBO_LOC_3440(CESL_Mgr* pParent,CLoanShareManager* pLoanShareManager, CLonaManageValue* pLoanManageValue)
	: CESL_Mgr(IDD, pParent)
{
	m_pLoanShareManager = pLoanShareManager;
	m_pLoanManageValue = pLoanManageValue;
	m_pSpfWork = NULL;
}

VOID CBO_LOC_3440::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3440)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3440, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3440)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnDDELIVERY, OnbtnDDELIVERY)
	ON_BN_CLICKED(IDC_btn3400_DCLOSE, OnbtnDCLOSE)
	ON_BN_CLICKED(IDC_bSEDNMAIL, OnbtnbSEDNMAIL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3440 message handlers

VOID CBO_LOC_3440::OnClose() 
{
	if( m_pSpfWork != NULL )
		delete m_pSpfWork;

	m_pSpfWork = NULL;

	CDialog::OnClose();
}

BOOL CBO_LOC_3440::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("BO_LOC_3440")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	m_pSpfWork = new CBL_BO_LOC_34( this , m_pLoanManageValue );

	DisplaySelectCnt();

	CString strValue;
	m_pLoanManageValue->GetManageValue(_T("열람"), _T("공통"), _T("배달자료관리"), _T("기본 대출기한"), strValue);
	if(!strValue.IsEmpty())
	{
		SetDlgItemInt(IDC_edtDLOAN_PERIOD, _ttoi(strValue), FALSE);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CBO_LOC_3440::DisplaySelectCnt()
{
 
	INT nSelectCnt = m_pSpfWork->GetSelectMainGridCnt();

	CString sSelectCnt;
	sSelectCnt.Format( _T("%d") , nSelectCnt );

	GetDlgItem(IDC_edtREPAIR_SPECIES)->SetWindowText(sSelectCnt);

	return 0;
}


VOID CBO_LOC_3440::OnbtnDDELIVERY() 
{
	m_pSpfWork->SPFExcutePath( DELIVERY );	

	CSPF_Work* spfWork = m_pSpfWork;

	CString sLoanInfoType = _T("I") ;
	CString sUserInfoType = _T("U") ;

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;

	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	
	if( sLoanInfoType.Compare(_T("I")) == 0 )
	{
		CString sRecKey;
		spfWork->GetTempData( SHARE_CO_LOAN_TBL_INSERT_REC_KEY , sRecKey );
		
		
		UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		
	}
	else if( sLoanInfoType.Compare(_T("U")) == 0 )
	{
		CString sRecKey;
		CString sUpdateFields;
		spfWork->GetTempData( SHARE_CO_LOAN_TBL_UPDATE_REC_KEY  , sRecKey );
		spfWork->GetTempData( SHARE_CO_LOAN_TBL_UPDATE_FIELDS  , sUpdateFields );
		
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = sUpdateFields;				
	}
	else if( sLoanInfoType.Compare(_T("D")) == 0  )
	{
		CString sRecKey;
		spfWork->GetTempData( SHARE_CO_LOAN_TBL_DELETE_REC_KEY  , sRecKey );
		
		UploadShareInfo.UploadShareLoanInfo.sType = _T("D");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		
	}

	if( sLoanInfoType.Compare(_T("N")) != 0 )
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
	UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
	
	if( sUserInfoType.Compare(_T("I")) == 0 )
	{
		CString sRecKey;
		spfWork->GetTempData( SHARE_CO_LOAN_USER_TBL_INSERT_REC_KEY , sRecKey );

		UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		
	}
	else if( sUserInfoType.Compare(_T("U")) == 0 )
	{
		CString sRecKey;
		CString sUpdateFields;
		spfWork->GetTempData( SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY  , sRecKey );
		spfWork->GetTempData( SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS  , sUpdateFields );
				
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = sUpdateFields;		
	}
	else if( sUserInfoType.Compare(_T("D")) == 0 )
	{
		CString sRecKey;
		spfWork->GetTempData( SHARE_CO_LOAN_USER_TBL_DELETE_REC_KEY  , sRecKey );
		
		UploadShareInfo.UploadShareLoanInfo.sType = _T("D");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		
	}
	if( sUserInfoType.Compare(_T("N")) != 0 )
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );

}

VOID CBO_LOC_3440::OnbtnDCLOSE() 
{
	OnCancel();	
}


VOID CBO_LOC_3440::OnbtnbSEDNMAIL()
{

}
HBRUSH CBO_LOC_3440::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}