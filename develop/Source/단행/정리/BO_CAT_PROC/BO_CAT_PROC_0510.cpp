// BO_CAT_PROC_0510.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0510.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0510 dialog


CBO_CAT_PROC_0510::CBO_CAT_PROC_0510(CESL_Mgr* pParent , CString sRecKey /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0510)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	sSpeciesKey = sRecKey;
	m_pBOSpeciesApi = NULL;
	m_nDividType = -1;
	m_bMultiVol = FALSE;
}

CBO_CAT_PROC_0510::~CBO_CAT_PROC_0510()
{
	if (m_pBOSpeciesApi != NULL)
	{
		delete m_pBOSpeciesApi;
		m_pBOSpeciesApi = NULL;
	}	
}


VOID CBO_CAT_PROC_0510::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0510)
	DDX_Control(pDX, IDC_TAB1, m_ctlTAB);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0510, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0510)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bCat0510INSERT, OnbCat0510INSERT)
	ON_BN_CLICKED(IDC_bObjectINSERT, OnbObjectINSERT)
	ON_BN_CLICKED(IDC_bNewObjectINSERT, OnbNewObjectINSERT)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_bNewObjectINSERT2, OnbNewObjectINSERT2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0510 message handlers

BOOL CBO_CAT_PROC_0510::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0510::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0510")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0510"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return FALSE;
	}
	INT iMainCount = pDM->GetRowCount();

	m_ctlTAB.InsertItem(0, _T("책분리"));
	m_ctlTAB.InsertItem(1, _T("권분리"));

	m_nOldDividType = m_nDividType;	

	if ( m_nDividType == -1 || m_nDividType == 0 ) 
	{		
		m_ctlTAB.SetCurSel(0);
		GetDlgItem(IDC_GRD_OBJECT2)->ShowWindow(FALSE);
	}
	else
	{
		m_ctlTAB.SetCurSel(1);
		GetDlgItem(IDC_GRD_OBJECT)->ShowWindow(FALSE);
	}

	CString sYn;
	pDM->GetCellData( _T("BB_책KEY"), 0 , sYn );
	if ( pDM->GetRowCount() == 0 ) {
		CString strWhere;
		strWhere.Format(_T("BOOK_APPENDIX_FLAG = 'B' AND SPECIES_KEY = %s ORDER BY VOL_SORT_NO"), sSpeciesKey);	
		if (pDM->RefreshDataManager(strWhere) < 0)	return FALSE;
	}
	
	INT iRowCount = pDM->GetRowCount();
	iMainCount = iRowCount;
	if ( pDM->GetRowCount() < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}	

	CESL_DataMgr *pDM_0510_OBJECT;
	pDM_0510_OBJECT = FindDM(_T("DM_BO_CAT_PROC_0510_OBJECT"));
	INT iObjCount = pDM_0510_OBJECT->GetRowCount();
	
	if ( m_nDividType == 0 || m_nDividType == -1) 
	{
		if ( iObjCount > 0 ) 
		{
			GetDlgItem(IDC_bCat0510INSERT)->EnableWindow(true);
			GetDlgItem (IDC_bNewObjectINSERT)->EnableWindow(FALSE);
		}
		else 
		{		
			GetDlgItem(IDC_bCat0510INSERT)->EnableWindow(false);			
		}
		GetDlgItem(IDOK)->EnableWindow(true);
		
	}
	else 
	{
		GetDlgItem(IDC_bCat0510INSERT)->EnableWindow(false);
		GetDlgItem(IDC_bObjectINSERT)->EnableWindow(false);

		CESL_DataMgr *pDM_VolumeHold = FindDM(_T("DMUP_단행_권_권정보"));		
		INT nRowCount = pDM_VolumeHold->GetRowCount();

		if ( nRowCount > 0 )
		{
			GetDlgItem(IDC_bNewObjectINSERT)->EnableWindow(FALSE);
			GetDlgItem(IDC_bNewObjectINSERT2)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_bNewObjectINSERT)->EnableWindow(TRUE);
			GetDlgItem(IDC_bNewObjectINSERT2)->EnableWindow(FALSE);			
		}
		

		GetDlgItem(IDOK)->EnableWindow(false);
		if ( pDM->GetRowCount() < 2 ) {
			GetDlgItem(IDC_bObjectINSERT)->EnableWindow(false);
		}
	}
	
	if ( m_nDividType == 0 )
	{
		CESL_DataMgr *pDM_VolumeHold = FindDM(_T("DMUP_단행_권_권정보"));		
		pDM_VolumeHold->FreeData();
		m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(pDM, pDM_VolumeHold);
		if ( pDM_VolumeHold->GetRowCount() > 1 )
		{
			m_bMultiVol = TRUE;		
		}
		else
		{
			( (CButton*) GetDlgItem (IDC_bNewObjectINSERT) )->EnableWindow(FALSE);
		}
	}

	iObjectCount = iObjCount;

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0510"));
	pCM->AllControlDisplay();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0510::OnbCat0510INSERT() 
{
EFS_BEGIN
	
	INT m_idx;
	CESL_DataMgr *pDM_0510;
	pDM_0510 = FindDM(_T("DM_BO_CAT_PROC_0510"));
	iMainCount = pDM_0510->GetRowCount();

	CESL_DataMgr *pDM_0510_OBJECT;
	pDM_0510_OBJECT = FindDM(_T("DM_BO_CAT_PROC_0510_OBJECT"));

	SelectMakeList(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_OBJECT"));
	m_idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_OBJECT"));

	if ( m_idx == -1 ) return;
	
	if ( m_nOldDividType == -1 )
	{
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);	
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);	
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);	
	}
	

	CArray < INT, INT > aIdx;
	while ( m_idx != -1 ) {
		CString sItem;
		pDM_0510_OBJECT->GetCellData( _T("BB_등록번호") , m_idx , sItem );					
		pDM_0510->InsertRow(-1);	
		iMainCount ++ ;
		pDM_0510->SetCellData( _T("BB_등록번호") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_편권차") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_편권차") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_편제명부록명") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_편제명부록명") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_매체구분_코드") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_매체구분_코드") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_형식구분_코드") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_형식구분_코드") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_발행년") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_발행년") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_낱권ISBN") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_낱권ISBN") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_낱권ISBN부가기호") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_낱권ISBN부가기호") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_면장수") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_면장수") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_크기") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_크기") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_가격성격") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_가격성격") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_가격") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_가격") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_가격잡정보") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_가격잡정보") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_딸림자료") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_딸림자료") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_물리적특성") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_물리적특성") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_책KEY") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_책KEY") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_대표책여부") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_대표책여부") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_권일련번호") , m_idx , sItem );							
		pDM_0510->SetCellData( _T("BB_권일련번호") , sItem, iMainCount-1 );
		
		aIdx.Add( m_idx );
		iObjectCount -- ;

		m_idx = SelectGetNext(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_OBJECT"));
	} 
	
	for( INT i = aIdx.GetSize()-1 ; i >=0 ; i-- ) {
		pDM_0510_OBJECT->DeleteRow( aIdx.GetAt(i) );
	}
	if ( pDM_0510_OBJECT->GetRowCount() == 0 ) 
	{		
		GetDlgItem(IDC_bNewObjectINSERT)->EnableWindow(TRUE);		
		GetDlgItem(IDC_bCat0510INSERT)->EnableWindow(FALSE);
		if ( m_bMultiVol == TRUE ) GetDlgItem(IDC_bNewObjectINSERT)->EnableWindow(TRUE);
		m_nDividType = -1;		
	}
	else if ( pDM_0510->GetRowCount() > 0 ) {
		GetDlgItem(IDOK)->EnableWindow(true);
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0510"));
	pCM->AllControlDisplay();

EFS_END
}

VOID CBO_CAT_PROC_0510::OnbObjectINSERT() 
{
EFS_BEGIN

	m_ctlTAB.SetCurSel(0);
	GetDlgItem(IDC_GRD_OBJECT)->ShowWindow(TRUE);
	GetDlgItem(IDC_GRD_OBJECT2)->ShowWindow(FALSE);	

	INT m_idx;
	CESL_DataMgr *pDM_0510;
	pDM_0510 = FindDM(_T("DM_BO_CAT_PROC_0510"));

	m_ctlTAB.SetCurSel(0);

	CESL_DataMgr *pDM_0510_OBJECT;
	pDM_0510_OBJECT = FindDM(_T("DM_BO_CAT_PROC_0510_OBJECT"));

	SelectMakeList(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_MAIN"));
	m_idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_MAIN"));	

	if ( m_idx == -1 ) return;
	
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);	

	while ( m_idx != -1 ) {
		CString sItem;
		pDM_0510->GetCellData( _T("BB_등록번호") , m_idx , sItem );					
		pDM_0510_OBJECT->InsertRow(-1);	
		iObjectCount ++ ;
		pDM_0510_OBJECT->SetCellData( _T("BB_등록번호") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_편권차") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_편권차") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_편제명부록명") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_편제명부록명") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_매체구분_코드") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_매체구분_코드") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_형식구분_코드") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_형식구분_코드") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_발행년") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_발행년") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_낱권ISBN") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_낱권ISBN") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_낱권ISBN부가기호") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_낱권ISBN부가기호") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_면장수") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_면장수") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_크기") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_크기") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_가격성격") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_가격성격") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_가격") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_가격") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_가격잡정보") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_가격잡정보") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_딸림자료") , m_idx , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_딸림자료") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_물리적특성") , m_idx , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_물리적특성") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_책KEY") , m_idx , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_책KEY") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_대표책여부") , m_idx , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_대표책여부") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_이용제한구분_코드") , m_idx , sItem );	
		pDM_0510_OBJECT->SetCellData( _T("BB_이용제한구분_코드") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_등록구분_코드") , m_idx , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_등록구분_코드") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_관리구분_코드") , m_idx , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_관리구분_코드") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_자료상태") , m_idx , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_자료상태") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_별치기호_코드") , m_idx , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_별치기호_코드") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_권일련번호") , m_idx , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_권일련번호") , sItem, iObjectCount-1 );		

		m_idx = SelectGetNext(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_MAIN"));
	} 

	SelectMakeList(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_MAIN"));
	m_idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_MAIN"));

	CArray < INT, INT > aIdx;
	while ( m_idx != -1 ) {
		aIdx.Add( m_idx );
		iMainCount -- ;
		m_idx = SelectGetNext(_T("CM_BO_CAT_PROC_0510"), _T("GRD_0510_MAIN"));
	}
	for( INT i = aIdx.GetSize()-1 ; i >=0 ; i-- ) {
		pDM_0510->DeleteRow( aIdx.GetAt(i) );
	}
	
	if ( pDM_0510_OBJECT->GetRowCount() > 0 ) {
		GetDlgItem(IDC_bCat0510INSERT)->EnableWindow(true);
		GetDlgItem(IDOK)->EnableWindow(true);
		GetDlgItem(IDC_bNewObjectINSERT)->EnableWindow(false);
		GetDlgItem(IDC_bNewObjectINSERT2)->EnableWindow(false);
		m_nDividType = 0;
	}

	if ( pDM_0510->GetRowCount() < 1 ) {
		GetDlgItem(IDOK)->EnableWindow(false);
		GetDlgItem(IDC_bNewObjectINSERT)->EnableWindow(false);
		GetDlgItem(IDC_bNewObjectINSERT2)->EnableWindow(false);
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0510"));
	pCM->AllControlDisplay();

EFS_END
}

VOID CBO_CAT_PROC_0510::OnOK() 
{
EFS_BEGIN

	CDialog::OnOK();

EFS_END
}

void CBO_CAT_PROC_0510::OnbNewObjectINSERT() 
{
	m_ctlTAB.SetCurSel(1);
	GetDlgItem(IDC_GRD_OBJECT)->ShowWindow(FALSE);
	GetDlgItem(IDC_GRD_OBJECT2)->ShowWindow(TRUE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);	

	CESL_DataMgr *pDM_0510 = FindDM(_T("DM_BO_CAT_PROC_0510"));
	CESL_DataMgr *pDM_VolumeHold = FindDM(_T("DMUP_단행_권_권정보"));		
	pDM_VolumeHold->FreeData();
	m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(pDM_0510, pDM_VolumeHold);

	if ( pDM_VolumeHold->GetRowCount() > 0 )
	{
		( (CButton*) GetDlgItem ( IDC_bObjectINSERT) )->EnableWindow(FALSE);
		( (CButton*) GetDlgItem ( IDC_bNewObjectINSERT) )->EnableWindow(FALSE);		
		( (CButton*) GetDlgItem ( IDC_bNewObjectINSERT2) )->EnableWindow(TRUE);
		( (CButton*) GetDlgItem ( IDOK) )->EnableWindow(TRUE);
		m_nDividType = 1;
	}
	else
	{
		( (CButton*) GetDlgItem ( IDC_bObjectINSERT) )->EnableWindow(TRUE);
	}

	PasteMultiVol();
	
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0510"));
	pCM->AllControlDisplay();
}

void CBO_CAT_PROC_0510::OnbNewObjectINSERT2() 
{
	if ( m_nOldDividType == -1 )
	{
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);	
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);	
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);	
	}
	

	CESL_DataMgr *pDM_VolumeHold = FindDM(_T("DMUP_단행_권_권정보"));
	pDM_VolumeHold->FreeData();		
	iObjectCount = 0;

	RecoveryBook();

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0510"));
	pCM->AllControlDisplay();	

	( (CButton*) GetDlgItem ( IDC_bObjectINSERT) )->EnableWindow(TRUE);
	( (CButton*) GetDlgItem ( IDC_bNewObjectINSERT) )->EnableWindow(TRUE);		
	( (CButton*) GetDlgItem ( IDC_bNewObjectINSERT2) )->EnableWindow(FALSE);	
	m_nDividType = -1;
}

void CBO_CAT_PROC_0510::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	INT nCurSel = m_ctlTAB.GetCurSel();

	if ( nCurSel == 0)
	{
		GetDlgItem(IDC_GRD_OBJECT)->ShowWindow(TRUE);
		GetDlgItem(IDC_GRD_OBJECT2)->ShowWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_GRD_OBJECT)->ShowWindow(FALSE);
		GetDlgItem(IDC_GRD_OBJECT2)->ShowWindow(TRUE);
	}
		
	*pResult = 0;
}

INT CBO_CAT_PROC_0510::PasteMultiVol()
{	
	CESL_DataMgr *pDM_0510;
	pDM_0510 = FindDM(_T("DM_BO_CAT_PROC_0510"));

	m_ctlTAB.SetCurSel(1);

	CESL_DataMgr *pDM_0510_OBJECT;
	pDM_0510_OBJECT = FindDM(_T("DM_BO_CAT_PROC_0510_OBJECT"));	

	INT nRowCount;
	CString sItem;
	CString sVolSortNo;
	nRowCount = pDM_0510->GetRowCount();
	pDM_0510->GetCellData( _T("BB_권일련번호") , 0 , sVolSortNo );
	for ( int i=0 ; i<nRowCount ; i++ )
	{
		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_권일련번호") , i , sItem );
		if ( sItem == sVolSortNo ) continue;

		pDM_0510_OBJECT->InsertRow(-1);
		iObjectCount ++ ;
		pDM_0510_OBJECT->SetCellData( _T("BB_권일련번호") , sItem, iObjectCount-1 );
		
		pDM_0510->GetCellData( _T("BB_등록번호") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_등록번호") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_편권차") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_편권차") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_편제명부록명") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_편제명부록명") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_매체구분_코드") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_매체구분_코드") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_형식구분_코드") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_형식구분_코드") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_발행년") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_발행년") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_낱권ISBN") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_낱권ISBN") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_낱권ISBN부가기호") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_낱권ISBN부가기호") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_면장수") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_면장수") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_크기") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_크기") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_가격성격") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_가격성격") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_가격") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_가격") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_가격잡정보") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_가격잡정보") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_딸림자료") , i , sItem );							
		pDM_0510_OBJECT->SetCellData( _T("BB_딸림자료") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_물리적특성") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_물리적특성") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_책KEY") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_책KEY") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_대표책여부") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_대표책여부") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_이용제한구분_코드") , i , sItem );	
		pDM_0510_OBJECT->SetCellData( _T("BB_이용제한구분_코드") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_등록구분_코드") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_등록구분_코드") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_관리구분_코드") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_관리구분_코드") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_자료상태") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_자료상태") , sItem, iObjectCount-1 );

		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_별치기호_코드") , i , sItem );
		pDM_0510_OBJECT->SetCellData( _T("BB_별치기호_코드") , sItem, iObjectCount-1 );	
	} 
	
	for ( i=0 ; i<nRowCount ; i++ )
	{
		sItem = _T("");
		pDM_0510->GetCellData( _T("BB_권일련번호") , nRowCount - i - 1 , sItem );
		if ( sItem == sVolSortNo ) continue;

		pDM_0510->DeleteRow(nRowCount - i - 1);
	}

	CESL_DataMgr *pDM_VolumeHold = FindDM(_T("DMUP_단행_권_권정보"));		
	nRowCount = pDM_VolumeHold->GetRowCount();
	for ( i=0 ; i<nRowCount ; i++ )
	{
		sItem = _T("");
		pDM_VolumeHold->GetCellData( _T("BB_권일련번호") , nRowCount - i - 1 , sItem );
		if ( sItem == sVolSortNo ) pDM_VolumeHold->DeleteRow(nRowCount - i - 1);				
	}

	return 0;	
}

INT CBO_CAT_PROC_0510::RecoveryBook()
{
	INT iMainCount;
	CESL_DataMgr *pDM_0510;
	pDM_0510 = FindDM(_T("DM_BO_CAT_PROC_0510"));	
	iMainCount = pDM_0510->GetRowCount();

	CESL_DataMgr *pDM_0510_OBJECT;
	pDM_0510_OBJECT = FindDM(_T("DM_BO_CAT_PROC_0510_OBJECT"));
	
	CString sItem;
	for ( int i=0 ; i<pDM_0510_OBJECT->GetRowCount() ; i++ )	
	{		
		pDM_0510_OBJECT->GetCellData( _T("BB_등록번호") , i , sItem );					
		pDM_0510->InsertRow(-1);	
		iMainCount ++ ;
		pDM_0510->SetCellData( _T("BB_등록번호") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_편권차") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_편권차") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_편제명부록명") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_편제명부록명") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_매체구분_코드") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_매체구분_코드") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_형식구분_코드") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_형식구분_코드") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_발행년") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_발행년") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_낱권ISBN") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_낱권ISBN") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_낱권ISBN부가기호") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_낱권ISBN부가기호") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_면장수") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_면장수") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_크기") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_크기") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_가격성격") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_가격성격") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_가격") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_가격") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_가격잡정보") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_가격잡정보") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_딸림자료") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_딸림자료") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_물리적특성") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_물리적특성") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_책KEY") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_책KEY") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_대표책여부") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_대표책여부") , sItem, iMainCount-1 );

		sItem = _T("");
		pDM_0510_OBJECT->GetCellData( _T("BB_권일련번호") , i , sItem );							
		pDM_0510->SetCellData( _T("BB_권일련번호") , sItem, iMainCount-1 );
	} 	

	pDM_0510_OBJECT->FreeData();

	return 0;
}

void CBO_CAT_PROC_0510::OnCancel() 
{
	m_nDividType = m_nOldDividType;	
	CDialog::OnCancel();
}

HBRUSH CBO_CAT_PROC_0510::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
