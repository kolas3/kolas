// BO_CAT_PROC_0203.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_PROC_0203.h"
#include "BO_CAT_PROC_0203_01.h"
#include "BO_CAT_PROC_0203_02.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0203 dialog


CBO_CAT_PROC_0203::CBO_CAT_PROC_0203(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0203)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0203::~CBO_CAT_PROC_0203()
{
}

VOID CBO_CAT_PROC_0203::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0203)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0203, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0203)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bPRINT, OnbPRINT)
	ON_BN_CLICKED(IDC_bPRINTFORM_CHANGE, Onb0203PRINTFORMCHANGE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0203 message handlers

BOOL CBO_CAT_PROC_0203::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0203::OnbPRINT() 
{
EFS_BEGIN
	CBO_CAT_PROC_0203_01 dlg(this);
	dlg.DoModal();	

EFS_END
}

BOOL CBO_CAT_PROC_0203::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0203")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	if (m_strParentCMAlias.IsEmpty())
		m_strParentCMAlias = _T("CM_BO_CAT_PROC_0000");
	if (m_strParentDMAlias.IsEmpty())
		m_strParentDMAlias = _T("DM_BO_CAT_PROC_0000");
	if (m_strParentGridAlias.IsEmpty())
		m_strParentGridAlias = _T("ProcGrid");

	INT ids = SelectMakeList(m_strParentCMAlias, m_strParentGridAlias);
	if ( 0 != ids ) {
		AfxMessageBox ( _T("SelectMakeList error") );
	}
	INT idx = SelectGetHeadPosition(m_strParentCMAlias, m_strParentGridAlias);
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		EndDialog(IDCANCEL);
		return -1;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(m_strParentDMAlias);
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return FALSE;
	}

	CString sRecKey = pDM->GetCellData( _T("BS_종KEY") , idx );
	while ( idx != -1 ) {
		CString sKey;
		pDM->GetCellData( _T("BS_종KEY") , idx , sKey );	
		sRecKey = sRecKey + _T(" , ") + sKey;
		idx = SelectGetNext(m_strParentCMAlias, m_strParentGridAlias);
	}


	CESL_DataMgr *pDM_0203;
	pDM_0203 = FindDM( _T("DM_BO_CAT_PROC_0203") );
	if ( NULL == pDM_0203 ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0203 error") );
		return FALSE;
	}
	
	CString strWhere = _T("B.SPECIES_KEY = I.REC_KEY");

	strWhere = strWhere + _T(" AND I.REC_KEY IN(") + sRecKey + _T(")");
	if (pDM_0203->RefreshDataManager(strWhere) < 0)	return FALSE;
	if ( pDM_0203->GetRowCount() < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}

	INT iRowCount = pDM_0203->GetRowCount();
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sCallNo;
		CString sShelfCode = pDM_0203->GetCellData( _T("BB_별치기호_코드"), i );
		sCallNo = sShelfCode;
		CString sClassNo = pDM_0203->GetCellData( _T("BB_분류기호"), i );
		if ( sClassNo != _T("") ) {
			if ( sCallNo != _T("") ) {
				sCallNo = sCallNo + _T(" ") + sClassNo;
			}
			else {
				sCallNo = sClassNo;
			}
		}
		CString sBookCode = pDM_0203->GetCellData( _T("BB_도서기호"), i );
		if ( sBookCode != _T("") ) {
			if ( sCallNo != _T("") ) {
				sCallNo = sCallNo + _T("-") + sBookCode;
			}
			else {
				sCallNo = sBookCode;
			}
		}
		CString sVolCode = pDM_0203->GetCellData( _T("BB_권책기호"), i );
		if ( sVolCode != _T("") ) {
			if ( sCallNo != _T("") ) {
				sCallNo = sCallNo + _T("-") + sVolCode;
			}
			else{
				sCallNo =sBookCode;
			}
		}
		CString sCopyCode = pDM_0203->GetCellData( _T("BB_복본기호"), i );
		if ( sCopyCode != _T("") ) {
			if ( sCallNo != _T("") ) {
				sCallNo = sCallNo + _T("=") + sCopyCode;
			}
			else {
				sCallNo = sCopyCode;
			}
		}
		pDM_0203->SetCellData( _T("BB_청구기호"), sCallNo, i );

		CString sPrintForm;
		pDM_0203->GetCellData( _T("BB_레이블출력구분"), i, sPrintForm);
		if ( sPrintForm == _T("") ) {
			pDM_0203->SetCellData( _T("BB_레이블출력구분"), _T("줄바꿔찍기") , i );
		}
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0203"));
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_BO_CAT_PROC_0203 error") );
		return FALSE;
	}
	pCM->AllControlDisplay();

	CESL_Grid *pGrid0203 = ( CESL_Grid * )FindControl(_T("CM_BO_CAT_PROC_0203"), _T("그리드_레이블출력"));
	pGrid0203->SetProtectSelectColumn(TRUE);

	INT iGrdRowCount = pGrid0203->GetRows();
	for ( i = 1 ; i < iGrdRowCount ; i++ ) {
		pGrid0203->SetTextMatrix( i, 1, _T("V") );
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0203::Onb0203PRINTFORMCHANGE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	INT ids = SelectMakeList(_T("CM_BO_CAT_PROC_0203"), _T("그리드_레이블출력"));
	if ( 0 != ids ) {
		AfxMessageBox ( _T("SelectMakeList error") );
	}
	INT idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0203"), _T("그리드_레이블출력"));
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));

		return;
	}

	CBO_CAT_PROC_0203_02 dlg(this);
	dlg.DoModal();

EFS_END
}
HBRUSH CBO_CAT_PROC_0203::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
