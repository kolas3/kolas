// MNG_CODE_MANAGER_02.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_CODE_MANAGER_02.h"

#include "MNG_CODE_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_02 dialog


CMNG_CODE_MANAGER_02::CMNG_CODE_MANAGER_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CODE_MANAGER_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CMNG_CODE_MANAGER_02::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	


VOID CMNG_CODE_MANAGER_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CODE_MANAGER_02)
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CODE_MANAGER_02, CDialog)
	//{{AFX_MSG_MAP(CMNG_CODE_MANAGER_02)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_02 message handlers

VOID CMNG_CODE_MANAGER_02::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

}

BOOL CMNG_CODE_MANAGER_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CODE_MANAGER_02") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CODE_MANAGER_02") );
		return false;
	}
	
	ViewCodeClass();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_CODE_MANAGER_02::ViewCodeClass()
{
	SelectCodeClass();
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_02") );
	
	m_combo.InsertString( 0, _T(" - ¼±ÅÃ - ") );
	
	CString className;
	INT rowCnt = pDM->GetRowCount();
	for( INT i = 0; i < rowCnt; i++ )
	{
		pDM->GetCellData( _T("CLASS_NAME"), i, className );
		m_combo.InsertString( i + 1, className );
	}

	m_combo.SetCurSel( 0 );
}

VOID CMNG_CODE_MANAGER_02::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

CString CMNG_CODE_MANAGER_02::MakeSelectQuery( INT codeClass )
{
	CString where;
	where.Format( _T(" C.CLASS = %d AND U.CODE_KEY = C.REC_KEY AND U.USER_KEY = %s"), codeClass, GetUserKey() );	

	return where;
}


// CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );

CString CMNG_CODE_MANAGER_02::GetUserKey()
{
	CString query, result;
	query.Format( _T(" SELECT REC_KEY FROM MN_USER_TBL WHERE ID = '%s' "), GetUserID() );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );
	pDM->GetOneValueQuery( query, result );
	
	return result;
}

VOID CMNG_CODE_MANAGER_02::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here

	if( !m_combo.GetCurSel() )
		return;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_01") );
	INT codeClass = GetCodeClassNum();
	pDM->RefreshDataManager( MakeSelectQuery( codeClass ) );
	
	CMNG_CODE_MANAGER_01 *pParent = ( CMNG_CODE_MANAGER_01 * )pParentMgr;
	pParent->m_mode = 1;
	pParent->m_codeClass = codeClass;
	pParent->ShowGrid();	
}

INT CMNG_CODE_MANAGER_02::SelectCodeClass()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_02") );
	CString where = _T(" USE_YN = 'Y'");
	pDM->RefreshDataManager( where );

	return 0;
}

INT CMNG_CODE_MANAGER_02::GetCodeClassNum()
{
	CString classNum;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_02") );
	pDM->GetCellData( _T("CLASS_CODE"), m_combo.GetCurSel() - 1, classNum );

	return _ttoi( classNum );
}
