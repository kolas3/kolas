// Purchase_PriceChange.cpp : implementation file
//

#include "stdafx.h"
#include "SePurchaseProgress.h"
#include "Purchase_PriceChange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPurchase_PriceChange dialog


CPurchase_PriceChange::CPurchase_PriceChange(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CPurchase_PriceChange)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
}

CPurchase_PriceChange::~CPurchase_PriceChange()
{
}

VOID CPurchase_PriceChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPurchase_PriceChange
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPurchase_PriceChange, CDialog)
	//{{AFX_MSG_MAP(CPurchase_PriceChange)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPurchase_PriceChange message handlers

BOOL CPurchase_PriceChange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr( _T("연속_수서_구입자료관리_구입가격일괄변경") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_구입자료관리_구입가격일괄변경") );
		return false;
	}

	m_pCM = FindCM(_T("CM_연속_수서_구입자료관리_구입가격일괄변경"));
	if ( m_pCM == NULL ) 
	{
		AfxMessageBox ( _T(" CM 초기화 에러") ) ;
		return FALSE;
	}

	if ( m_pGrid == NULL || m_pDM == NULL ) 
		return FALSE;

	m_pGrid->SelectMakeList();
	INT idx = m_pGrid->SelectGetHeadPosition();
	if(idx<0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
	    return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CPurchase_PriceChange::OnOK() 
{
	if ( m_pGrid == NULL || m_pDM == NULL ) 
		return ;

	INT cnt = m_pGrid->SelectGetCount();
	
	CString strMessage;
	strMessage.Format(_T("선정돤 %d 건에 대해 구입 가격을 일괄 변경하시겠습니까? "), cnt );

	if(AfxMessageBox(strMessage,MB_YESNO) != IDYES) return; 

	CSePurchaseProgress* pProgressDlg;
	pProgressDlg = new CSePurchaseProgress(this);
	pProgressDlg->Create(this);

	pProgressDlg->m_ctrlProgress.SetRange(0, cnt );
	pProgressDlg->m_ctrlProgress.SetPos(0);
	pProgressDlg->ShowWindow(SW_SHOW);
	pProgressDlg->BringWindowToTop();
	pProgressDlg->CenterWindow();

	INT ids = m_pDM->StartFrame();
	if ( ids < 0 ) return;

	INT nIdx = m_pGrid->SelectGetHeadPosition();
	while(nIdx>=0)
	{
		CString afterPrice;
		CString strPercent;
		CString PrePrice = m_pDM->GetCellData ( _T("구독가격"), nIdx );

		m_pCM->GetControlMgrData(_T("가격") , strPercent );

		INT nafterPrice = 0;
		nafterPrice = _ttoi(PrePrice) * _ttoi(strPercent) / 100;
		afterPrice.Format(_T("%d"), nafterPrice );

		CString strQuery;
		if ( afterPrice == _T("") ) 
		{
			nIdx = m_pGrid->SelectGetNext();
			continue;
		}

		strQuery.Format(_T("UPDATE SE_PURCHASE_SPECIES_TBL SET PRICE = '%s' WHERE REC_KEY = %s ;"), afterPrice , m_pDM->GetCellData ( _T("REC_KEY"), nIdx ));
		ids = m_pDM->AddFrame(strQuery);
		if ( ids < 0 ) return;

		pProgressDlg->m_ctrlProgress.SetPos( nIdx + 1 );
		nIdx = m_pGrid->SelectGetNext();
	}

	pProgressDlg->ShowWindow(SW_HIDE);

	ids = m_pDM->SendFrame();
	if ( ids < 0 ) return;
	else AfxMessageBox ( _T("정상적으로 변경되었습니다.") );


	ids = m_pDM->EndFrame();
	if ( ids < 0 ) return;



	CDialog::OnOK();
}

VOID CPurchase_PriceChange::OnCancel() 
{
	CDialog::OnCancel();
}

VOID CPurchase_PriceChange::SetInfo(CESL_DataMgr *pDM, CESL_Grid *pGrid)
{
	m_pDM = pDM;
	m_pGrid = pGrid ;
	return;
}

HBRUSH CPurchase_PriceChange::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
