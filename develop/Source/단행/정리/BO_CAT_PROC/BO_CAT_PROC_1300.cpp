// BO_CAT_PROC_1300.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_1300.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_1300 dialog


CBO_CAT_PROC_1300::CBO_CAT_PROC_1300(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_1300)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInitDlg = TRUE;
}

CBO_CAT_PROC_1300::~CBO_CAT_PROC_1300()
{
}

BOOL CBO_CAT_PROC_1300::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_1300::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_1300)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_1300, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_1300)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bCatControlCodeMODIFY, OnbCatControlCodeMODIFY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_1300 message handlers

BOOL CBO_CAT_PROC_1300::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_1300")) < 0)
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

	INT idx;
	SelectMakeList(m_strParentCMAlias, m_strParentGridAlias);
	idx = SelectGetHeadPosition(m_strParentCMAlias, m_strParentGridAlias);
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		EndDialog(IDCANCEL);
		m_bInitDlg = FALSE;
		return TRUE;
		
	}
	
	CESL_DataMgr *pDM;
	pDM = FindDM(m_strParentDMAlias);
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return FALSE;
	}

	sRecKey = pDM->GetCellData( _T("BS_종KEY") , idx );

	if ( sRecKey == _T("") ) {
		AfxMessageBox (_T("선정된 자료가 없습니다."));

		return FALSE;
	}

	INT iRowCount = 1;

	idx = SelectGetNext(m_strParentCMAlias, m_strParentGridAlias);
	CString sRecKeySub;
	while ( idx != -1 ) {
		CString sKey;
		pDM->GetCellData( _T("BS_종KEY") , idx , sKey );	
		sRecKey = sRecKey + _T(" , ") + sKey;
		idx = SelectGetNext(m_strParentCMAlias, m_strParentGridAlias);

		iRowCount ++ ;
	} 

	CESL_DataMgr *pDM_1300;
	pDM_1300 = FindDM(_T("DM_BO_CAT_PROC_1300"));
	
	CString strWhere = _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY IN (") + sRecKey + _T(")");
	if (pDM_1300->RefreshDataManager(strWhere) < 0)	return FALSE;

	INT iDataCount = pDM_1300->GetRowCount();
	if ( pDM_1300->GetRowCount() < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}

	pDM_1300->SORT(_T("BB_등록번호"));

	for ( INT i = 1 ; i <= iDataCount ; i ++ ) {
		CString sMarc = pDM_1300->GetCellData( _T("BS_MARC") , i-1 );

		CMarc m_marc;
		CString strTagCode, strErrMsg, strHelpMsg;		
		if(m_pInfo->pMarcMgr->Decoding(sMarc, &m_marc))	{
			POSITION pos;
			pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
			while (pos)	{
				m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);	
				UpdateData(FALSE);
			}	
		}

		TCHAR szLeader[25];
		m_marc.GetLeader(szLeader);

		CString sLeader;
		sLeader.Format ( _T("[%s]") , szLeader );
		pDM_1300->SetCellData( _T("리더") , sLeader , i-1 );

		CString s007,s008;
		m_pInfo->pMarcMgr->GetItem( &m_marc, _T("007*"), s007 );
		if ( s007 != _T("") ) {
			s007 = _T("[") + s007 + _T("]");
			pDM_1300->SetCellData( _T("007"), s007, i-1 );
		}
		m_pInfo->pMarcMgr->GetItem( &m_marc, _T("008*"), s008 );
		if ( s008 != _T("") ) {
			s008 = _T("[") + s008 + _T("]");
			pDM_1300->SetCellData( _T("008"), s008, i-1 );
		}

	}	

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_1300"));
	pCM->AllControlDisplay();

	CESL_Grid * pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM->FindControl(_T("GRD_1300_ACC"));
	INT iSelectCount = pGrid_sub->GetRows();
	for ( i = 1 ; i < iSelectCount ; i ++ ) {
		pGrid_sub->SetTextMatrix( i, 1, _T("V") );
	}

	pCM = FindCM(m_strParentCMAlias);
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_PROC_0000 error") );
		return FALSE;
	}
	pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM->FindControl(m_strParentGridAlias);
	iSelectCount = pGrid_sub->GetRows();
	for ( i = 1 ; i < iSelectCount ; i ++ ) {
		CString sCheck = pGrid_sub->GetTextMatrix( i, 1 );
		if ( sCheck == _T("V") ) {
			m_aCheckRow.Add(i);
		}
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}


VOID CBO_CAT_PROC_1300::OnbCatControlCodeMODIFY() 
{
EFS_BEGIN

	CString sControlCode;
	GetControlData( _T("CM_BO_CAT_PROC_1300"), _T("BS_제어자료구분"), sControlCode);

	INT idx;
	SelectMakeList(_T("CM_BO_CAT_PROC_1300"), _T("GRD_1300_ACC"));
	idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_1300"), _T("GRD_1300_ACC"));
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		
		return;
		
	}

	AfxMessageBox(_T("제어번호가 없는 자료만 변경됩니다."));
	
	INT nCnt;
	CESL_ControlMgr *pCM;
	CESL_Grid * pGrid_sub = NULL;
	pCM = FindCM( m_strParentCMAlias );
	pGrid_sub = (CESL_Grid*)pCM->FindControl( m_strParentGridAlias );
	pGrid_sub->SelectMakeList();
	nCnt = pGrid_sub->SelectGetCount();

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_1300"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_1300 error") );
		return;
	}
	
	CString sSpeciesKey;
	CString str001;
	while ( idx != -1 ) {
		int ids = pDM->GetCellData( _T("제어번호") , idx , str001 );	
		if( _T("") != str001 )
		{
			idx = SelectGetNext(_T("CM_BO_CAT_PROC_1300"), _T("GRD_1300_ACC"));		
			continue;
		}
		pDM->SetCellData( _T("제어자료구분"), sControlCode , idx );
		CString sKey;
		pDM->GetCellData( _T("BS_종KEY") , idx , sKey );	
		sSpeciesKey = sSpeciesKey + _T(" , ") + sKey;
		idx = SelectGetNext(_T("CM_BO_CAT_PROC_1300"), _T("GRD_1300_ACC"));		

	} 

	if( sSpeciesKey.IsEmpty() )
	{
		return;
	}
	sSpeciesKey = sSpeciesKey.Mid(3);

	INT iCh = pDM->StartFrame();
	if ( 0 > iCh ) return;
	CString sQuery;
	sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET CONTROL_MAT_CODE = '%s', LAST_WORK='%s' WHERE REC_KEY IN( %s );") , sControlCode, GetWorkLogMsg(_T("자료일괄확인"),WORK_LOG), sSpeciesKey );
	iCh = pDM->AddFrame( sQuery );
	if ( 0 > iCh ) return;

	iCh = pDM->SendFrame();	
	if ( iCh < 0 ) {
		AfxMessageBox( _T("db저장 확인") );
		return;
	}
	iCh = pDM->EndFrame();
	if ( iCh < 0 ) 
	{
		AfxMessageBox(_T("EndFrame 를 확인하세요"));
		return;
	}

	AllControlDisplay( _T("CM_BO_CAT_PROC_1300") );

	pDM = FindDM(m_strParentDMAlias);
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}

	SelectMakeList(m_strParentCMAlias, m_strParentGridAlias);
	idx = SelectGetHeadPosition(m_strParentCMAlias, m_strParentGridAlias);
	
	while ( idx != -1 ) {
		CString sKey;
		pDM->SetCellData( _T("IBS_제어자료구분_코드"), sControlCode , idx );
		
		pDM->SetCellData( _T("BS_제어자료구분_코드"), sControlCode , idx );
		
		idx = SelectGetNext(m_strParentCMAlias, m_strParentGridAlias);		
	} 

	AllControlDisplay( m_strParentCMAlias );
	INT nChk;
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		nChk = m_aCheckRow.GetAt(i);
		pGrid_sub->SetTextMatrix( nChk, 1,_T("V"));
	}

EFS_END
}

HBRUSH CBO_CAT_PROC_1300::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
