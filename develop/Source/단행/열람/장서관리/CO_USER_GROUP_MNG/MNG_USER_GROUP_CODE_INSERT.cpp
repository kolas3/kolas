// MNG_USER_GROUP_CODE_INSERT.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_USER_GROUP_CODE_INSERT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_GROUP_CODE_INSERT dialog


CMNG_USER_GROUP_CODE_INSERT::CMNG_USER_GROUP_CODE_INSERT(CESL_Mgr* pParent,CString sUserKey)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_USER_GROUP_CODE_INSERT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sUserKey = sUserKey;
}
// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CMNG_USER_GROUP_CODE_INSERT::~CMNG_USER_GROUP_CODE_INSERT()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CMNG_USER_GROUP_CODE_INSERT::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_USER_GROUP_CODE_INSERT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CMNG_USER_GROUP_CODE_INSERT, CDialog)
	//{{AFX_MSG_MAP(CMNG_USER_GROUP_CODE_INSERT)
	ON_BN_CLICKED(IDC_btnMUGCI_INSERT, OnbtnMUGCIINSERT)
	ON_BN_CLICKED(IDC_btnMUGCI_CLOSE, OnbtnMUGCICLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_GROUP_CODE_INSERT message handlers

BOOL CMNG_USER_GROUP_CODE_INSERT::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	INT ids = InitESL_Mgr( _T("MNG_USER_GROUP_CODE_INSERT") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	InitControl();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CMNG_USER_GROUP_CODE_INSERT::OnbtnMUGCIINSERT() 
{
EFS_BEGIN

	AfxGetApp()->DoWaitCursor(-1);

	InsertGroup();

	AfxGetApp()->DoWaitCursor(1);
	

EFS_END
}

VOID CMNG_USER_GROUP_CODE_INSERT::OnbtnMUGCICLOSE() 
{
EFS_BEGIN

	OnCancel();	

EFS_END
}



INT CMNG_USER_GROUP_CODE_INSERT::InitControl()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_USER_GROUP_CODE_INFO"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitControl") );

	CESL_ControlMgr* pCM = FindCM(_T("CM_MNG_USER_CODE_INSERT"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitControl") );

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("그룹")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("InitControl") );

	CString sInsertCode[2];
	
	pCMCB->InitMultiComboBox(2);

	sInsertCode[0] = _T("   ");
	sInsertCode[1] = _T("적용안함");
	ids = pCMCB->SetData( sInsertCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InitControl") );

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("코드") , i , sInsertCode[0] );
		pDM->GetCellData( _T("이름") , i , sInsertCode[1] );

		sInsertCode[0].TrimLeft(); sInsertCode[0].TrimRight();
		if( sInsertCode[0].IsEmpty() ) continue;
			
		ids = pCMCB->SetData( sInsertCode , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InitControl") );
	}
	
	
	pCMCB->SetCurSel(0);



	return 0;



EFS_END
return -1;

}


INT CMNG_USER_GROUP_CODE_INSERT::InsertGroup()
{
EFS_BEGIN


	INT ids;

	CString sGroupCode;
	ids = GetControlData( _T("CM_MNG_USER_CODE_INSERT") , _T("그룹") , sGroupCode , -1 , 0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertGroup") ); 

	if( sGroupCode.IsEmpty() )
	{
		AfxMessageBox(_T("입력할 그룹을 지정해주십시오."));
		return 3;
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_USER_GROUP_CODE_INFO"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("InsertGroup") );


	CString sDeleteQuery;

	pDM->StartFrame();


	pDM->InitDBFieldData();
	sDeleteQuery.Format( _T("DELETE FROM MN_LOAN_USER_GROUP_LIST_TBL WHERE USER_KEY=%s AND GROUP_CODE='%s';") , m_sUserKey , sGroupCode );
	pDM->AddFrame( sDeleteQuery );

	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , _T("ESL_SEQ.NextVal") ); 
	pDM->AddDBFieldData( _T("GROUP_CODE") , _T("STRING") , sGroupCode );
	pDM->AddDBFieldData( _T("USER_KEY") , _T("NUMERIC") , m_sUserKey  );
	ids = pDM->MakeInsertFrame( _T("MN_LOAN_USER_GROUP_LIST_TBL") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InsertGroup") );

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("InsertGroup") );

	pDM->EndFrame();

	pParentMgr->SendMessage( WM_ECO_CHANGE_GROUP );


	AfxMessageBox( _T("변경되었습니다. ") );

	return 0;



EFS_END
return -1;

}

HBRUSH CMNG_USER_GROUP_CODE_INSERT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
