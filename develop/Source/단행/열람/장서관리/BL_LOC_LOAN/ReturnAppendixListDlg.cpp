// ReturnAppendixListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReturnAppendixListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReturnAppendixListDlg dialog


CReturnAppendixListDlg::CReturnAppendixListDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD , pParent)
{
	//{{AFX_DATA_INIT(CReturnAppendixListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCloseMode = 1; 
}

CReturnAppendixListDlg::~CReturnAppendixListDlg()
{
}
VOID CReturnAppendixListDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReturnAppendixListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CReturnAppendixListDlg, CDialog)
	//{{AFX_MSG_MAP(CReturnAppendixListDlg)
	ON_BN_CLICKED(IDC_btnSELECT, OnbtnSELECT)
	ON_BN_CLICKED(IDC_btnLOAN_CANCEL, OnbtnCANCEL)
	ON_BN_CLICKED(IDC_btnSELECT2, OnbtnSELECT2)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReturnAppendixListDlg message handlers

BOOL CReturnAppendixListDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	if(InitESL_Mgr(_T("SM_APPENDIX_RETURN_INFO_LIST")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	ViewGrid();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

INT CReturnAppendixListDlg::ViewGrid()
{
	EFS_BEGIN

	CString sDMAlias = _T("DM_BO_LOC_3100_RETURN_APPENDIX_INFO");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -4 , _T("ViewGrid()") );	

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("자료실구분") , _T("자료실") );


	INT ids = AllControlDisplay( _T("CM_APPENDIX_RETURN_INFO_LIST") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("ViewGrid()") );

	OnClickAppendixList();

	return 0;

	EFS_END
	return -1;

}

// 반납
VOID CReturnAppendixListDlg::OnbtnSELECT() 
{
	EFS_BEGIN

	m_nCloseMode = 0;
	OnCancel();

	EFS_END
}

INT CReturnAppendixListDlg::GetCloseMode()
{
	EFS_BEGIN

	return m_nCloseMode;

	EFS_END
	return -1;

}

// 취소
VOID CReturnAppendixListDlg::OnbtnCANCEL() 
{
	EFS_BEGIN

	m_nCloseMode = 1;
	OnCancel();

	EFS_END
}

BEGIN_EVENTSINK_MAP(CReturnAppendixListDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CReturnAppendixListDlg)
	ON_EVENT(CReturnAppendixListDlg, IDC_APPENDIX_LIST, -600 /* Click */, OnClickAppendixList, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CReturnAppendixListDlg::OnClickAppendixList() 
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_APPENDIX_RETURN_INFO_LIST");
	CString sGridAlias = _T("AppendixGrid");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2(  -1 , _T("OnClickAppendixList") );	

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2(  -2 , _T("OnClickAppendixList") );	

	INT nIndex = pGrid->GetIdx();

	pGrid->SetReverse(nIndex);

	INT ids = AllControlDisplay(_T("CM_APPENDIX_RETURN_INFO") , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2(  -3 , _T("OnClickAppendixList") );	

	CString sDMAlias = _T("DM_BO_LOC_3100_RETURN_APPENDIX_INFO");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_VOID2(  -4 , _T("OnClickAppendixList") );	

	CString sReturnState;
	ids = pDM->GetCellData( _T("부록상태") , nIndex , sReturnState );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2(  -5 , _T("OnClickAppendixList") );	

	((CButton*)GetDlgItem(IDC_radAPPENDIX_RETURN1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_radAPPENDIX_RETURN2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_radAPPENDIX_RETURN3))->SetCheck(0);
	if( sReturnState.Compare(_T("0"))==0 )
	{
		((CButton*)GetDlgItem(IDC_radAPPENDIX_RETURN1))->SetCheck(1);
	}
	else if( sReturnState.Compare(_T("1"))==0 )
	{
		((CButton*)GetDlgItem(IDC_radAPPENDIX_RETURN2))->SetCheck(1);
	}
	else if( sReturnState.Compare(_T("2"))==0 )
	{
		((CButton*)GetDlgItem(IDC_radAPPENDIX_RETURN3))->SetCheck(1);
	}

	EFS_END
}

VOID CReturnAppendixListDlg::OnbtnSELECT2() 
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_APPENDIX_RETURN_INFO");
	CString sDMAlias = _T("DM_BO_LOC_3100_RETURN_APPENDIX_INFO");

	INT nIndex = GetCurrentGridIndex();

	const INT nCnt = 2;
	TCHAR* sFieldAlias[nCnt] =
	{
		_T("연체료") , _T("배상비")
	};

	CString sData[nCnt];
	for( INT i = 0 ;i < nCnt ; i++ )
	{
		ids = GetControlData( sCMAlias , sFieldAlias[i] , sData[i] );
		if( ids < 0 )ERROR_MSG_RETURN_VOID2(  -1 , _T("OnbtnSELECT2") );	

		ids = SetDataMgrData( sDMAlias , sFieldAlias[i] , sData[i] , nIndex );
		if( ids < 0 )ERROR_MSG_RETURN_VOID2(  -2 , _T("OnbtnSELECT2") );	
	}

	CString sMode;
	GetReturnType( sMode );
	ids = SetDataMgrData( sDMAlias , _T("부록상태") , sMode , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2(  -3 , _T("OnbtnSELECT2") );	

	ViewGrid();

	EFS_END
}


INT CReturnAppendixListDlg::GetCurrentGridIndex()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_APPENDIX_RETURN_INFO_LIST");
	CString sGridAlias = _T("AppendixGrid");

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("GetCurrentGridIndex") );	

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("GetCurrentGridIndex") );	

	INT nIndex = pGrid->GetIdx();

	return nIndex;

	EFS_END
	return -1;

}

VOID CReturnAppendixListDlg::GetReturnType( CString &sMode )
{
	EFS_BEGIN

	if( ((CButton*)GetDlgItem(IDC_radAPPENDIX_RETURN1))->GetCheck() == 1 )
		sMode = _T("0");
	if( ((CButton*)GetDlgItem(IDC_radAPPENDIX_RETURN2))->GetCheck() == 1 )
		sMode = _T("1");
	if( ((CButton*)GetDlgItem(IDC_radAPPENDIX_RETURN3))->GetCheck() == 1 )
		sMode = _T("2");

	EFS_END
}

HBRUSH CReturnAppendixListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}
