// MISSING_DATA_SUMPRICE.cpp : implementation file
//

#include "stdafx.h"
#include "MISSING_DATA_SUMPRICE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMISSING_DATA_SUMPRICE dialog


CMISSING_DATA_SUMPRICE::CMISSING_DATA_SUMPRICE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMISSING_DATA_SUMPRICE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParentDM = NULL;
	m_pParentGrid = NULL;

	m_pDM = NULL;
	m_pCM = NULL;

}

CMISSING_DATA_SUMPRICE::~CMISSING_DATA_SUMPRICE()
{
}



VOID CMISSING_DATA_SUMPRICE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMISSING_DATA_SUMPRICE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMISSING_DATA_SUMPRICE, CDialog)
	//{{AFX_MSG_MAP(CMISSING_DATA_SUMPRICE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMISSING_DATA_SUMPRICE message handlers

BOOL CMISSING_DATA_SUMPRICE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr(_T("SM_SE_MISSING_SUM_PRICE"))<0)
	{
		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}	

	m_pCM = FindCM ( _T("CM_SE_MISSING_SUM_PRICE") ) ;
	if ( m_pCM == NULL ) 
	{
		AfxMessageBox ( _T("OPEN CM ERROR") );
		return FALSE;
	}

	m_pDM = FindDM ( _T("DM_SE_MISSING_SUM_PRICE") );
	if ( m_pDM == NULL ) 
	{
		AfxMessageBox ( _T("OPEN DM ERROR") );
		return FALSE;
	}

	if ( !Init() ) return FALSE;

	m_pCM->AllControlDisplay();

	GetDlgItem(IDC_sSUMPRICE)->SetWindowText(m_strSumPrice);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMISSING_DATA_SUMPRICE::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

VOID CMISSING_DATA_SUMPRICE::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CMISSING_DATA_SUMPRICE::SetPrice( CString strSumPrice)
{
	m_strSumPrice = strSumPrice;
}

VOID CMISSING_DATA_SUMPRICE::SetParentInfo(CESL_DataMgr *pDM, CESL_Grid *pGrid)
{
	m_pParentDM = pDM;
	m_pParentGrid = pGrid;
}

BOOL CMISSING_DATA_SUMPRICE::Init()
{
	// 부모 그리드에 있는 정보 복사
	if ( m_pParentDM == NULL || m_pParentGrid == NULL ) return FALSE;

	if ( m_pDM == NULL || m_pCM == NULL ) return FALSE;


	INT nChildIdx = 0;
	INT nIdx = -1;
	nIdx = m_pParentGrid->SelectGetHeadPosition();
	while ( nIdx > 0 ) 
	{
		m_pDM->InsertRow(-1);

		m_pDM->SetCellData( _T("TITLE_INFO"), m_pParentDM->GetCellData( _T("TITLE_INFO"), nIdx ), nChildIdx );
		m_pDM->SetCellData( _T("PUBLISHER"), m_pParentDM->GetCellData( _T("PUBLISHER"), nIdx ), nChildIdx );
		m_pDM->SetCellData( _T("VOL_TITLE"), m_pParentDM->GetCellData( _T("VOL_TITLE"), nIdx ), nChildIdx );
		m_pDM->SetCellData( _T("UDF_구입처"), m_pParentDM->GetCellData( _T("UDF_구입처"), nIdx ), nChildIdx );
		m_pDM->SetCellData( _T("구입가격"), m_pParentDM->GetCellData( _T("구입가격"), nIdx ), nChildIdx );

		nChildIdx++;
		nIdx = m_pParentGrid->SelectGetNext();
	}

	return TRUE;
}

HBRUSH CMISSING_DATA_SUMPRICE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
