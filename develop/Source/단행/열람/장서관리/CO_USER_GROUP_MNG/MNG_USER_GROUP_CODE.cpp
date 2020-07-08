// MNG_USER_GROUP_CODE.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_USER_GROUP_CODE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_GROUP_CODE dialog


CMNG_USER_GROUP_CODE::CMNG_USER_GROUP_CODE(CESL_Mgr* pParent,CString sUserKey)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_USER_GROUP_CODE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sUserKey = sUserKey;
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CMNG_USER_GROUP_CODE::~CMNG_USER_GROUP_CODE()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CMNG_USER_GROUP_CODE::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_USER_GROUP_CODE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CMNG_USER_GROUP_CODE, CDialog)
	//{{AFX_MSG_MAP(CMNG_USER_GROUP_CODE)
	ON_BN_CLICKED(IDC_btnMUGC_CLOSE, OnbtnMUGCCLOSE)
	ON_BN_CLICKED(IDC_btnMUGC_INSERT, OnbtnMUGCINSERT)
	ON_BN_CLICKED(IDC_btnMUGC_DELETE, OnbtnMUGCDELETE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ECO_CHANGE_GROUP, OnRefreshData)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_GROUP_CODE message handlers

BOOL CMNG_USER_GROUP_CODE::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr( _T("MNG_USER_GROUP_CODE") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );


	ids = InitCodeValue();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("OnInitDialog") );

	ids = RefreshData();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}


INT CMNG_USER_GROUP_CODE::RefreshData()
{
EFS_BEGIN

	INT ids;

	/// 검색
	CString sWhere;
	sWhere.Format( _T("USER_KEY = %s") , m_sUserKey );


	CESL_DataMgr* pDM = FindDM( _T("DM_MNG_USER_GROUP_CODE") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RefreshData") );

	ids = pDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RefreshData") );

	AllControlDisplay( _T("CM_MNG_USER_GROUP_CODE") );

	return 0;

EFS_END
return -1;

}


INT CMNG_USER_GROUP_CODE::InitCodeValue()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM( _T("DM_USER_GROUP_CODE_INFO") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitCodeValue") );

	ids = pDM->RefreshDataManager( _T("") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitCodeValue") );

	return 0;

EFS_END
return -1;

}

VOID CMNG_USER_GROUP_CODE::OnbtnMUGCCLOSE() 
{
EFS_BEGIN

	OnCancel();	

EFS_END
}




VOID CMNG_USER_GROUP_CODE::OnbtnMUGCINSERT() 
{
EFS_BEGIN

	CMNG_USER_GROUP_CODE_INSERT Dlg(this,m_sUserKey);
	Dlg.DoModal();

EFS_END
}


LRESULT CMNG_USER_GROUP_CODE::OnRefreshData(WPARAM wParam, LPARAM lParam)
{
	INT ids;

	ids = RefreshData();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnRefreshData") );

	return 0;
}

VOID CMNG_USER_GROUP_CODE::OnbtnMUGCDELETE() 
{
EFS_BEGIN

	AfxGetApp()->DoWaitCursor(-1);

	DeleteGroup();

	AfxGetApp()->DoWaitCursor(1);
	

EFS_END
}

INT CMNG_USER_GROUP_CODE::DeleteGroup()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_MNG_USER_GROUP_CODE"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("InsertGroup") );

	if( pDM->GetRowCount() < 1 ) 
	{
		AfxMessageBox( _T("삭제할 그룹이 존재하지 않습니다.") );
		return 1;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_USER_GROUP_CODE"),_T("그룹그리드"));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("DeleteGroup") );

	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt < 1 )
	{
		AfxMessageBox( _T("삭제할 그룹을 선정해주십시오.") );
		return 1;
	}

	pDM->StartFrame();

	INT nIndex;
	CString sRecKey;
	CString sDeleteQuery;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		ids = pDM->GetCellData( _T("그룹정보KEY") , nIndex , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertGroup") );

		pDM->InitDBFieldData();
		sDeleteQuery.Format( _T("DELETE FROM MN_LOAN_USER_GROUP_LIST_TBL WHERE REC_KEY=%s;") , sRecKey  );
		pDM->AddFrame( sDeleteQuery );

		pGrid->SelectGetNext();

	}

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("InsertGroup") );

	pDM->EndFrame();

	SendMessage( WM_ECO_CHANGE_GROUP );

	AfxMessageBox( _T("삭제되었습니다. ") );

	return 0;

EFS_END
return -1;

}

HBRUSH CMNG_USER_GROUP_CODE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
