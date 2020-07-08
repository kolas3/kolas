// DataProcessingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_REG_EPC_PROGRESS_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_REG_EPC_PROGRESS_DLG dialog


CBO_REG_EPC_PROGRESS_DLG::CBO_REG_EPC_PROGRESS_DLG(CESL_Mgr * pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_REG_EPC_PROGRESS_DLG)
	m_strMessage = _T("");
	//}}AFX_DATA_INIT
	m_bCancel = FALSE;
	m_bCreate = FALSE;
	m_pParentWnd = pParent;		
	mCtrl_pGrid = NULL;
}


VOID CBO_REG_EPC_PROGRESS_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_REG_EPC_PROGRESS_DLG)
	DDX_Control(pDX, IDC_prgDATA_PROCESSING, m_prgDataProcessing);
	DDX_Text(pDX, IDC_stcDATA_PROCESSING, m_strMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_REG_EPC_PROGRESS_DLG, CDialog)
	//{{AFX_MSG_MAP(CBO_REG_EPC_PROGRESS_DLG)
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_CREATE()	
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_REG_EPC_PROGRESS_DLG message handlers

BOOL CBO_REG_EPC_PROGRESS_DLG::Create(CWnd *pParentWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_REG_EPC_PROGRESS_DLG::SetMessage(CString newMsg)
{
	m_strMessage = newMsg;
	CButton * pBtn = (CButton*)GetDlgItem(IDC_stcDATA_PROCESSING);

	//++2008.09.30 ADD BY CJY {{++
	// 투명 스태틱 잔상 제거
	pBtn->SetWindowText(_T(""));
 
 	CRect rBtn; 
 	pBtn->GetWindowRect(rBtn);
 	ScreenToClient(rBtn);
 	InvalidateRect(rBtn);
 	UpdateWindow();
	//--2008.09.30 ADD BY CJY --}}

	pBtn->SetWindowText(m_strMessage);	
	//dateData(FALSE);
}

CButton * CBO_REG_EPC_PROGRESS_DLG::GetDataProcessingMsgCtrl()
{
	return (CButton*)GetDlgItem(IDC_stcDATA_PROCESSING);
}

VOID CBO_REG_EPC_PROGRESS_DLG::OnCancel() 
{
	// TODO: Add extra cleanup here
	SendMsgToParent();
	CDialog::OnCancel();
}

VOID CBO_REG_EPC_PROGRESS_DLG::SendMsgToParent()
{
	m_pParentWnd->SendMessage(JOB_CANCEL_MSG, 0, 0);
}

VOID CBO_REG_EPC_PROGRESS_DLG::InitPos()
{
	m_prgDataProcessing.SetPos(0);

	m_prgDataProcessing.SetBkColour(RGB(255, 255, 255));
}

VOID CBO_REG_EPC_PROGRESS_DLG::ShowHideCancelButton(BOOL bShowHide/*=TRUE*/)
{
	if(bShowHide) GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);
	else GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
}

BOOL CBO_REG_EPC_PROGRESS_DLG::OnEraseBkgnd(CDC* pDC) 
{	/*
	if ( m_BGDC == NULL ) InitBG();
	CRect rect; 
	GetClientRect( &rect );	
	if ( pDC == NULL ) return TRUE;
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_BGDC, 0, 0, SRCCOPY);					
	return TRUE;
	*/
	return CDialog::OnEraseBkgnd(pDC);
}


void CBO_REG_EPC_PROGRESS_DLG::InitBG()
{
/*	CDC* pDC = GetDC();
	m_BGDC.CreateCompatibleDC(pDC);		
	m_BGBitmap.LoadBitmap(IDB_WAIT);
	m_BGDC.SelectObject(&m_BGBitmap);
	ReleaseDC(pDC);	

	//++2009.01.06 ADD BY CJY {{++
	// 이미지 크기에 맞춰 다이얼로그 크기조정
	BITMAP bm;
	m_BGBitmap.GetObject(sizeof(BITMAP), &bm);

	CRect rWnd, rClient;
	GetWindowRect(&rWnd);
	GetClientRect(&rClient);

	const int HEIGHT_OFFSET = rWnd.Height() - rClient.Height();
	const int WIDTH_OFFSET = rWnd.Width() - rClient.Width();

	SetWindowPos(NULL, 0, 0, bm.bmWidth+WIDTH_OFFSET, bm.bmHeight+HEIGHT_OFFSET, SWP_NOMOVE);	
*/	//--2009.01.06 ADD BY CJY --}}
}

void CBO_REG_EPC_PROGRESS_DLG::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);	
		
}

int CBO_REG_EPC_PROGRESS_DLG::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	InitBG();
	
	return 0;
}


// 스태틱배경 브러쉬 바꾸기
//DEL HBRUSH CBO_REG_EPC_PROGRESS_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
//DEL {
//DEL 	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
//DEL 	// TODO: Change any attributes of the DC here
//DEL 	if( nCtlColor == CTLCOLOR_STATIC)
//DEL 	{
//DEL //DEL 		pDC->SetTextColor(RGB(255,255,255));
//DEL 		SetBkMode(pDC->m_hDC, TRANSPARENT);
//DEL 		return (HBRUSH)::GetStockObject (NULL_BRUSH);
//DEL 	}
//DEL 	// TODO: Return a different brush if the default is not desired
//DEL 	return hbr;
//DEL }

BOOL CBO_REG_EPC_PROGRESS_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	mCtrl_pGrid = new CESL_Grid(NULL);
	mCtrl_pGrid->SubclassDlgItem(IDC_grid_ERROR_VIEW, this);
	mCtrl_pGrid->InitToolTip();

	mCtrl_pGrid->SetTextMatrix(0, 0, _T("에러코드"));
	mCtrl_pGrid->SetTextMatrix(0, 1, _T("서명"));
	mCtrl_pGrid->SetTextMatrix(0, 2, _T("내용"));

	mCtrl_pGrid->SetCols(0, 3);
 	mCtrl_pGrid->SetColWidth(0, 0, 840);
	mCtrl_pGrid->SetColWidth(1, 0, 3000);	
	mCtrl_pGrid->SetColWidth(2, 0, 4750);

	mCtrl_pGrid->SetRow(0);
	mCtrl_pGrid->SetCol(0);
	mCtrl_pGrid->SetCellAlignment(4);
	mCtrl_pGrid->SetCol(1);
	mCtrl_pGrid->SetCellAlignment(4);
	mCtrl_pGrid->SetCol(2);
	mCtrl_pGrid->SetCellAlignment(4);

	mCtrl_pGrid->SetRow(1);
	mCtrl_pGrid->SetFixedCols(0);
	mCtrl_pGrid->m_bFroceReverse = FALSE;
	
	EnableThemeDialogTexture(GetSafeHwnd());


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CBO_REG_EPC_PROGRESS_DLG::AddErrorList(int nCode, CString strTitle, CString strMessage)
{
//DEL 	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_grid_ERROR_VIEW);
	if(NULL == mCtrl_pGrid) return -1;

	int nRow = -1;
	if(2 == mCtrl_pGrid->GetRows() && TRUE == mCtrl_pGrid->GetTextMatrix(1, 0).IsEmpty())
	{
		nRow = 1;
	}
	else 
	{
		nRow =  mCtrl_pGrid->GetRows();	
		mCtrl_pGrid->SetRows(nRow+1);
	}
	
	CString strCode;
	strCode.Format(_T("%d"), nCode);
	mCtrl_pGrid->SetTextMatrix(nRow, 0, strCode);
	mCtrl_pGrid->SetTextMatrix(nRow, 1, strTitle);
	mCtrl_pGrid->SetTextMatrix(nRow, 2, strMessage);

	mCtrl_pGrid->SendMessage(WM_VSCROLL, SB_BOTTOM, 0 );	

	return 0;
}

int CBO_REG_EPC_PROGRESS_DLG::ResetGrid()
{
	if(NULL == mCtrl_pGrid) return -1;
	mCtrl_pGrid->SetRows(2);
	mCtrl_pGrid->SetTextMatrix(1, 0, _T(""));
	mCtrl_pGrid->SetTextMatrix(1, 1, _T(""));
	mCtrl_pGrid->SetTextMatrix(1, 2, _T(""));

	return 0;
}

int CBO_REG_EPC_PROGRESS_DLG::TogleOKCancelBtn(UINT uEanbleBtnID)
{
	if(IDOK != uEanbleBtnID && IDCANCEL != uEanbleBtnID) return -1;

	UINT uDisableBtnID = (IDOK == uEanbleBtnID) ? IDCANCEL : IDOK;
	
	CWnd* pEnableWnd = GetDlgItem(uEanbleBtnID);
	CWnd* pDisableWnd = GetDlgItem(uDisableBtnID);

	if(NULL != pEnableWnd) pEnableWnd->EnableWindow(TRUE);
	if(NULL != pDisableWnd) pDisableWnd->EnableWindow(FALSE);

	return 0;
}

void CBO_REG_EPC_PROGRESS_DLG::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( nFlags & MK_LBUTTON ) // L 마우스 버튼이 눌러져 있구나..
	{
		this->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
		return;
	}

	CDialog::OnMouseMove(nFlags, point);
}
