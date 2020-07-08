// SE_BINDING_ORDER_03.cpp : implementation file
//

#include "stdafx.h"
#include "SE_BINDING_ORDER_03.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_ORDER_03 dialog


CSE_BINDING_ORDER_03::CSE_BINDING_ORDER_03(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_nHeadPosition = m_nTailPosition = 0;
}

CSE_BINDING_ORDER_03::~CSE_BINDING_ORDER_03()
{
}

VOID CSE_BINDING_ORDER_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_BINDING_ORDER_03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_BINDING_ORDER_03, CDialog)
	//{{AFX_MSG_MAP(CSE_BINDING_ORDER_03)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_ORDER_03 message handlers

BOOL CSE_BINDING_ORDER_03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_SE_BINDING_ORDER_03")) < 0 ) {

		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}
	
	INT ids = MoveIndex( _CURRENT );
	if( ids ) {
		AfxMessageBox (_T("MoveIndex() Error~!"));
		return FALSE ;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_BINDING_ORDER_03::OnbSAVE() 
{
	INT ids;

	CString sSql, sBindingPrice, sRecKey;

	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_ORDER_01"));
	if( pDataMgr == NULL ) return;

	ids = GetControlData( _T("CM_SE_BINDING_ORDER_MOD"), _T("BINDING_PRICE"), sBindingPrice, -1 );
	if( ids < 0 ) return;

	ids = SetDataMgrData( _T("DM_SE_BINDING_ORDER_01"), _T("BINDING_PRICE"), sBindingPrice, m_nIdx );
	if( ids < 0 ) return;

	ids = GetDataMgrData( _T("DM_SE_BINDING_ORDER_01"), _T("REC_KEY"), sRecKey, m_nIdx );
	if( ids < 0 ) return;
	
	pDataMgr->StartFrame();
	sSql.Format( _T("UPDATE SE_BINDING_BOOK_TBL SET BINDING_PRICE = '%s' WHERE BOOK_KEY = %s;"), 
				 sBindingPrice, sRecKey );
	
	pDataMgr->AddFrame(sSql);
    ids = pDataMgr->SendFrame();
	pDataMgr->EndFrame();
	if( ids < 0 ) 
	{
		AfxMessageBox(_T("저장에 실패 하였습니다!"));	
		return;
	}

	Display();
}

VOID CSE_BINDING_ORDER_03::Display()
{
	INT ids, nRowCnt, i;
	INT nTotPrice, nPrice;
	
	CString sSpeciesKey, sBindVolCnt;
	CString sBindingPrice, sTotBindingPrice;

	nPrice = nTotPrice = 0;

	CESL_DataMgr* pDataMgr = FindDM(_T("DM_SE_BINDING_ORDER_01"));
	if( pDataMgr == NULL ) return;

	nRowCnt = pDataMgr->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ ) {
		
		ids = GetDataMgrData( _T("DM_SE_BINDING_ORDER_01"), _T("BINDING_PRICE"), sBindingPrice, i );
		if( ids < 0 ) return;

		nPrice = _ttoi( (TCHAR*)LPCTSTR(sBindingPrice) );

		nTotPrice += nPrice;
	}
	
	for( i = 0 ; i < nRowCnt ; i++ ) {
		sTotBindingPrice.Format( _T("%d"), nTotPrice );
		ids = SetDataMgrData( _T("DM_SE_BINDING_ORDER_01"), _T("TOT_BINDING_PRICE"), sTotBindingPrice, i );
	}

	ids = AllControlDisplay( _T("CM_SE_BINDING_ORDER_01"), -1 );
	if( ids < 0 ) return;

	ids = AllControlDisplay( _T("CM_SE_BINDING_ORDER_MOD"), m_nIdx );
	if( ids < 0 ) return;
}

VOID CSE_BINDING_ORDER_03::OnbNEXT() 
{
	INT ids;

	ids = MoveIndex(_NEXT);
	if( ids ) return;
}

VOID CSE_BINDING_ORDER_03::OnbPREV() 
{
	INT ids;

	ids = MoveIndex(_PREV);
	if( ids ) return;	
}

INT CSE_BINDING_ORDER_03::MoveIndex(TCHAR to)
{
    INT nRowIndex, ids;

	CESL_ControlMgr *pCM		= FindCM( _T("CM_SE_BINDING_ORDER_01") );
	CESL_Grid		*pGridMgr	= (CESL_Grid*) pCM->FindControl ( _T("MainGrid") ) ;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1. 기본화면의 index에 대한 처리를 한다.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    switch( to ) {
        case _NOMOVE:
            nRowIndex = pGridMgr->SelectGetIdx( );
            if ( nRowIndex < 0 ) return -2;

			if( m_nHeadPosition == nRowIndex ) GetDlgItem(IDC_bPREV)->EnableWindow(FALSE);
			else							   GetDlgItem(IDC_bPREV)->EnableWindow(TRUE);	
			if( m_nTailPosition == nRowIndex ) GetDlgItem(IDC_bNEXT)->EnableWindow(FALSE);
			else							   GetDlgItem(IDC_bNEXT)->EnableWindow(TRUE);	
            break;
        case _CURRENT:
			ids = pGridMgr->SelectMakeList();
            if ( ids < 0 ) return -3;
		
			m_nTailPosition = pGridMgr->SelectGetTailPosition();
			m_nHeadPosition = pGridMgr->SelectGetHeadPosition();

			if( m_nHeadPosition == m_nTailPosition )GetDlgItem(IDC_bNEXT)->EnableWindow(FALSE);
			GetDlgItem(IDC_bPREV)->EnableWindow(FALSE);
			break;
        case _PREV:
            nRowIndex = pGridMgr->SelectGetPrev( );
            if ( nRowIndex < 0 ) return -4;

			if( m_nHeadPosition == nRowIndex ) GetDlgItem(IDC_bPREV)->EnableWindow(FALSE);
			else							   GetDlgItem(IDC_bPREV)->EnableWindow(TRUE);	
			if( m_nTailPosition == nRowIndex ) GetDlgItem(IDC_bNEXT)->EnableWindow(FALSE);
			else							   GetDlgItem(IDC_bNEXT)->EnableWindow(TRUE);	
            break;
        case _NEXT:
            nRowIndex = pGridMgr->SelectGetNext( );
            if ( nRowIndex < 0 ) return -4;

			if( m_nHeadPosition == nRowIndex ) GetDlgItem(IDC_bPREV)->EnableWindow(FALSE);
			else							   GetDlgItem(IDC_bPREV)->EnableWindow(TRUE);	
			if( m_nTailPosition == nRowIndex ) GetDlgItem(IDC_bNEXT)->EnableWindow(FALSE);
			else					  		   GetDlgItem(IDC_bNEXT)->EnableWindow(TRUE);	
            break;
        default:
            return -6;
            break;
    }

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2. 선택정보를 표시한다.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //pGridMgr->SetListIndexSelection( );

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 3. 이전,다음 버튼의 Enable/Diable
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Prev_Next_ButtonStatus( _T("DISP_DICIDE"), m_pParCtrlAlias, m_pParGridAlias, IDC_bPREV, IDC_bNEXT);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 4. 이동된 레코드 인덱스를 구한다.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_nIdx = pGridMgr->SelectGetIdx();
    if ( m_nIdx < 0 ) return -8;

	ids = AllControlDisplay( _T("CM_SE_BINDING_ORDER_MOD"), m_nIdx );
	if( ids < 0 ) return -1;

    return 0;
}


HBRUSH CSE_BINDING_ORDER_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
