#include "stdafx.h"
#include "BO_LOC_3180.h"
#include "BO_LOC_3183.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBO_LOC_3183::~CBO_LOC_3183()
{
	
}

CBO_LOC_3183::CBO_LOC_3183(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
		
}


VOID CBO_LOC_3183::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3183)
	//}}AFX_DATA_MAP	
	
}


BEGIN_MESSAGE_MAP(CBO_LOC_3183, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3183)
	ON_BN_CLICKED(IDC_bLoad, OnbLoad)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CBO_LOC_3183::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	
	return CDialog::Create(IDD, pParentWnd);
}

INT CBO_LOC_3183::DoModal() 
{
	
	return CDialog::DoModal();
}

BOOL CBO_LOC_3183::PreTranslateMessage(MSG* pMsg) 
{	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CBO_LOC_3183::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	if(InitESL_Mgr(_T("SM_BO_LOC_3183")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	if(LoadData()){
		AfxMessageBox(_T("ERROR CBO_LOC_3183::LoadData()"));
		return FALSE;
	}

	return TRUE;  
	              
}

INT CBO_LOC_3183::LoadData()
{
	EFS_BEGIN

	CString strFuncName = _T("CBO_LOC_3183::LoadData()");
	INT ids = 0;

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3183"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , strFuncName );

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_3183") );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , strFuncName );


	pDM->FreeData();
	ids = pDM->RefreshDataManager(_T(""));
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -7 , strFuncName );
	
	ids = pCM->AllControlDisplay();
	if(ids)  ERROR_MSG_RETURN_ERROR_CODE2( -9 , strFuncName );
	
	return 0;

	EFS_END

	return -1;
}

VOID CBO_LOC_3183::OnbLoad() 
{
	EFS_BEGIN

	INT ids=0;

	ids = ShowData();
	if(ids){
	}

	return ;

	EFS_END

	return ;	
}

INT CBO_LOC_3183::ShowData()
{
	EFS_BEGIN

	INT ids=0;

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_3183") );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3183::ShowData() := FindCM()") );
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(_T("ReloadGrid")));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3183::ShowData() := FindControl()") );

	INT nTmpIndex;
	nTmpIndex = pGrid->GetIdx();

	CString sWorkStartTime;
	CString sWorker;
	sWorkStartTime = pGrid->GetTextMatrix(nTmpIndex+1, 1);
	sWorker = pGrid->GetTextMatrix(nTmpIndex+1, 2);

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3180"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3183::ShowData() := FindDM()") );

	CESL_ControlMgr *pCMMain = FindCM( _T("CM_BO_LOC_3180") );
	if( pCMMain == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3183::ShowData() := FindCM()") );

	CString sWhere;
	sWhere.Format(_T(" to_char(WORK_START_TIME , 'YYYY/MM/DD:HH24-MI-SS')='%s' and WORKER = '%s' "),
		sWorkStartTime, sWorker);
	pDM->FreeData();
	ids = pDM->RefreshDataManager(sWhere);
	if(ids){
	}

	ids = pCMMain->AllControlDisplay();
	if(ids){
	}

	OnOK();

	return 0;
	EFS_END
	return -1;
}

VOID CBO_LOC_3183::OnOK() 
{	
	CDialog::OnOK();
}

BEGIN_EVENTSINK_MAP(CBO_LOC_3183, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_3183)
	ON_EVENT(CBO_LOC_3183, IDC_gridRELOAD, -601 /* DblClick */, OnDblClickgridRELOAD, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_3183::OnDblClickgridRELOAD() 
{
	EFS_BEGIN

	INT ids=0;

	ids = ShowData();
	if(ids){
	}

	return ;
	EFS_END
	return ;	
}

HBRUSH CBO_LOC_3183::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
