// BO_CAT_PROC_0400.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0400.h"
#include "..\..\..\공통\BO_VOLUME_FORM\BOVolumeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0400 dialog


CBO_CAT_PROC_0400::CBO_CAT_PROC_0400(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0400)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0400::~CBO_CAT_PROC_0400()
{
}

BOOL CBO_CAT_PROC_0400::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0400::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0400)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0400, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0400)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bACCESSIONNO_GIVE, OnbACCESSIONNOGIVE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0400 message handlers

BOOL CBO_CAT_PROC_0400::OnInitDialog() 
{
EFS_BEGIN

	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0400_삭제")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	INT idx;
	SelectMakeList(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
	idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));

	if ( idx < 0 ) {
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		m_bInitDlg = FALSE;		
		return FALSE;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return FALSE;
	}

	CString sRecKey;
	sRecKey = pDM->GetCellData( _T("BS_종KEY") , idx );

	if ( sRecKey == _T("") ) {
		AfxMessageBox (_T("선정된 자료가 없습니다."));
		return FALSE;
	}

	RecKeyListArray.Add( sRecKey );
	m_IdxArray.Add(idx);
	INT iRowCount = 1;

	idx = SelectGetNext(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
	CString sRecKeySub;
	while ( idx != -1 ) {
		CString sKey;
		pDM->GetCellData( _T("BS_종KEY") , idx , sKey );			
		RecKeyListArray.Add( sKey );
		m_IdxArray.Add(idx);
		idx = SelectGetNext(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
		iRowCount ++ ;
	} 
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;


EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0400::OnbACCESSIONNOGIVE() 
{
EFS_BEGIN

	CBO_SPECIES api(this);
	api.SetOpenMode(-7000);
	api.InitDataMgr();

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}
	CString strSpeciesKey;

	CString sForm = _T("PURCHASE");
	CString sMode = _T("RECYCLE");

	INT ids;

	CString sMessage;
	sMessage.Format( _T("%d건의 자료를 삭제하시겠습니까?"), RecKeyListArray.GetSize() );
	
	if (MessageBox( sMessage, _T("삭제확인"), MB_YESNO) != IDYES) {
		return;
	}
	for ( INT i = 0 ; i < RecKeyListArray.GetSize() ; i++ ) {
		strSpeciesKey = RecKeyListArray.GetAt(i);
		ids = api.BO_SPECIES_DELETE_OR_RECYCLE(sForm, sMode, strSpeciesKey);
	}
	for ( i = m_IdxArray.GetSize()-1 ; i >=0 ; i-- ) {
		pDM->DeleteRow(m_IdxArray.GetAt(i));
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0000"));
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_BO_CAT_PROC_0000 error") );
		return;
	}
	pCM->AllControlDisplay();

	sMessage.Format( _T("%d건의 자료를 삭제하였습니다."), RecKeyListArray.GetSize() );
	AfxMessageBox( sMessage );

EFS_END
}

VOID CBO_CAT_PROC_0400::OnCancel() 
{
EFS_BEGIN

	DestroyWindow();	

EFS_END
}

HBRUSH CBO_CAT_PROC_0400::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
