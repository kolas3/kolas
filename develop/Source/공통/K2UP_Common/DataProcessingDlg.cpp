// DataProcessingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataProcessingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataProcessingDlg dialog


CDataProcessingDlg::CDataProcessingDlg(CWnd * pParent /*=NULL*/)
{
	//{{AFX_DATA_INIT(CDataProcessingDlg)
	m_strMessage = _T("");
	//}}AFX_DATA_INIT
	m_bCancel = FALSE;
	m_bCreate = FALSE;
	m_pParentWnd = pParent;		
}


VOID CDataProcessingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataProcessingDlg)
	DDX_Control(pDX, IDC_prgDATA_PROCESSING, m_prgDataProcessing);
	DDX_Text(pDX, IDC_stcDATA_PROCESSING, m_strMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataProcessingDlg, CDialog)
	//{{AFX_MSG_MAP(CDataProcessingDlg)
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataProcessingDlg message handlers

BOOL CDataProcessingDlg::Create(CWnd *pParentWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pParentWnd);
}

VOID CDataProcessingDlg::SetMessage(CString newMsg)
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

CButton * CDataProcessingDlg::GetDataProcessingMsgCtrl()
{
	return (CButton*)GetDlgItem(IDC_stcDATA_PROCESSING);
}

VOID CDataProcessingDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	SendMsgToParent();
	CDialog::OnCancel();
}

VOID CDataProcessingDlg::SendMsgToParent()
{
	m_pParentWnd->SendMessage(JOB_CANCEL_MSG, 0, 0);
}

VOID CDataProcessingDlg::InitPos()
{
	m_prgDataProcessing.SetPos(0);
}

VOID CDataProcessingDlg::ShowHideCancelButton(BOOL bShowHide/*=TRUE*/)
{
	if(bShowHide) GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);
	else GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
}

BOOL CDataProcessingDlg::OnEraseBkgnd(CDC* pDC) 
{	
	if ( m_BGDC == NULL ) InitBG();
	CRect rect; 
	GetClientRect( &rect );	
	if ( pDC == NULL ) return TRUE;
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_BGDC, 0, 0, SRCCOPY);					
	return TRUE;
}


void CDataProcessingDlg::InitBG()
{
	CDC* pDC = GetDC();
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
	//--2009.01.06 ADD BY CJY --}}
}

void CDataProcessingDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);	
		
}

int CDataProcessingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	InitBG();
	
	return 0;
}


// 스태틱배경 브러쉬 바꾸기
HBRUSH CDataProcessingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	// TODO: Change any attributes of the DC here
	if( nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(255,255,255));
		SetBkMode(pDC->m_hDC, TRANSPARENT);
		return (HBRUSH)::GetStockObject (NULL_BRUSH);
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}