// SeSubHistoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeSubHistoryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeSubHistoryDlg dialog


CSeSubHistoryDlg::CSeSubHistoryDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeSubHistoryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeSubHistoryDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pDM = NULL;
	m_strManageKey = _T("");
	m_strCurrentKey = _T("");
	m_strNewKey = _T("");
	m_nCurrentKeyRow = -1;
	m_nCurrentIdx = -1;
}

CSeSubHistoryDlg::~CSeSubHistoryDlg()
{
}

VOID CSeSubHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeSubHistoryDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeSubHistoryDlg, CDialog)
	//{{AFX_MSG_MAP(CSeSubHistoryDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeSubHistoryDlg message handlers

BOOL CSeSubHistoryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("연속_수서_구독이력관리"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_구독이력관리") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_연속_구독이력관리"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	m_pDM = FindDM(_T("DM_연속_구독이력관리"));
	

	if(!m_pDM || !m_pCM || !m_pGrid) {
		AfxMessageBox( _T("DM,CM,grid Error : 연속_수서_구독이력관리") );
		return FALSE;
	}	

	if(m_strManageKey.IsEmpty()) {
		AfxMessageBox(_T("구입관리KEY를 찾을 수 없습니다!"));
		return FALSE;
	}	
	
	if(m_strCurrentKey.IsEmpty()) {
		AfxMessageBox(_T("현재 구독이력KEY를 찾을 수 없습니다!"));
		return FALSE;
	}	

	CString strWhere;
	strWhere.Format(_T("PURCHASE_MANAGE_KEY = %s"),m_strManageKey);
	m_pDM->RefreshDataManager(strWhere);
	
	
	if(0 == m_pDM->GetRowCount()) {
		AfxMessageBox(_T("구독이력정보가 없습니다!"));
		return FALSE;
	}	
	
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM, _T("구입처관리"), _T("UDF_구입처"));
	m_pGrid->Display();

	//현재의 KEY를 표시해 준다
	SetCurrentKey();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CSeSubHistoryDlg::SetCurrentKey()
{
	CString strREC_KEY;
	INT nRow = m_pDM->GetRowCount();

	for(INT i=0;i<nRow;i++)
	{
		strREC_KEY = _T("");
		strREC_KEY = m_pDM->GetCellData(_T("구독이력KEY"),i);
		
		if(strREC_KEY == m_strCurrentKey) {
			m_nCurrentKeyRow = i;
			break;
		}

	}
	
	SetRedReverse();
	//SetGridRowRedBackColor(m_nCurrentKeyRow);
	

	return 0;
}

VOID CSeSubHistoryDlg::SetRedReverse()
{
	INT col;
    col = m_pGrid->GetCol();
    
	INT cols;
	cols = m_pGrid->GetCols(0);
	
	COLORREF red, white, black;
	red = RGB(251,196,234);
	white = RGB(255,255,255);	
	black = RGB(0,0,0);	

	INT nRow = m_pDM->GetRowCount();

	for( INT i=1;i<nRow+1;i++)
	{
		m_pGrid->SetRow(i);
		
		if(i == m_nCurrentKeyRow + 1)
		{
			for ( INT j = 1 ; j < cols ; j++ ) 
			{
				m_pGrid->SetCol(j);
				m_pGrid->SetCellBackColor(red);
				m_pGrid->SetCellForeColor(black);
			}
		}
		else
		{
			for ( INT j = 1 ; j < cols ; j++ ) 
			{
				m_pGrid->SetCol(j);
				m_pGrid->SetCellBackColor(white);
				m_pGrid->SetCellForeColor(black);
			}
		}
	}
}

VOID CSeSubHistoryDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_nCurrentIdx<0) {
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return;
	}

	
	CString strREC_KEY;
	strREC_KEY = m_pDM->GetCellData(_T("구독이력KEY"),m_nCurrentIdx);

	if(strREC_KEY == m_strCurrentKey) {

		AfxMessageBox(_T("변경사항이 없습니다"));
		return;
	}
	else
	{
	
		CString msg;
		msg.Format(_T("구독이력을 변경하시겠습니까?"));
		if(AfxMessageBox(msg,MB_YESNO) != IDYES) return;
		
		m_strNewKey = strREC_KEY;

	}

	CDialog::OnOK();
}

BEGIN_EVENTSINK_MAP(CSeSubHistoryDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSeSubHistoryDlg)
	ON_EVENT(CSeSubHistoryDlg, IDC_gridSUB_HISTORY, -600 /* Click */, OnClickgridSUBHISTORY, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSeSubHistoryDlg::OnClickgridSUBHISTORY() 
{
	// TODO: Add your control notification handler code here
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		m_nCurrentIdx = row-1;
		
		if(m_nCurrentIdx == m_nCurrentKeyRow)
		{
			SetRedReverse();
		}
		else
		{
			m_pGrid->SetReverse(row-1);
			SetGridRowRedBackColor(m_nCurrentKeyRow);
		}
	}

}

INT CSeSubHistoryDlg::SetGridRowRedBackColor(INT nRow)
{
	COLORREF red, white, black;
	
	red = RGB(251,196,234);
	white = RGB(255,255,255);
	black = RGB(0,0,0);

	INT nCol = m_pGrid->GetCol();
	INT nCols = m_pGrid->GetCols(0);
	
	if (nRow >= 0)
	{
		m_pGrid->SetRow(nRow + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			m_pGrid->SetCol(i);
			m_pGrid->SetCellBackColor(red);
			m_pGrid->SetCellForeColor(black);
		}
	}
	
	m_pGrid->SetCol(nCol);

	return 0;
}
HBRUSH CSeSubHistoryDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
