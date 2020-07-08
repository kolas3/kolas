// MarcAdjMgrMemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcAdjMgrMemoDlg.h"
#include "CatESL_Mgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjMgrMemoDlg dialog


CMarcAdjMgrMemoDlg::CMarcAdjMgrMemoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcAdjMgrMemoDlg)
	m_strMemo = _T("");
	//}}AFX_DATA_INIT

	m_pDM_Species = NULL;
}

CMarcAdjMgrMemoDlg::~CMarcAdjMgrMemoDlg()
{
	
}

VOID CMarcAdjMgrMemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcAdjMgrMemoDlg)
	DDX_Text(pDX, IDC_eMEMO_MA, m_strMemo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarcAdjMgrMemoDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcAdjMgrMemoDlg)
	ON_BN_CLICKED(IDC_bSAVE_MA, OnbSAVEMA)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjMgrMemoDlg Operations
INT CMarcAdjMgrMemoDlg::Init()
{
	m_strMemo = m_pDM_Species->GetCellData(m_strMemoDMFieldAlias, 0);
	UpdateData(FALSE);
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CMarcAdjMgrMemoDlg message handlers

BOOL CMarcAdjMgrMemoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (!m_pDM_Species) 
	{
		m_strLastError = _T("종 DM을 설정해 주세요");
		goto ERR;
	}

	if (m_strMemoDMFieldAlias.IsEmpty())
	{
		m_strLastError = _T("메모 필드 Alias를 설정해 주세요");
		goto ERR;
	}

	if (Init() < 0) goto ERR;

	PostMessage(WM_SIZE);

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	AfxMessageBox(m_strLastError);
	EndDialog(IDCANCEL);
	return FALSE;
}

VOID CMarcAdjMgrMemoDlg::OnbSAVEMA() 
{
	UpdateData();

	m_pDM_Species->SetCellData(m_strMemoDMFieldAlias, m_strMemo, 0);

	CCatApi *pCatApi = static_cast<CCatESL_Mgr*>(pParentMgr)->GetCatApi();
	INT nModeMONO_SERIAL = static_cast<CCatESL_Mgr*>(pParentMgr)->m_nModeMONO_SERIAL;
	INT nAppMode = static_cast<CCatESL_Mgr*>(pParentMgr)->GetAppMode();
	if (nModeMONO_SERIAL == MODE_SERIAL)
	{
		CString strQueryFlag = pCatApi->GetQueryFlag(MNG_MODIFY, nAppMode );
		m_pDM_Species->SetCellData(_T("SB_DB_FLAG"), strQueryFlag, 0);
	}
	else 
	{
		CString strQueryFlag = pCatApi->GetQueryFlag(MNG_MODIFY, nAppMode );
		m_pDM_Species->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, 0);
	}

	AfxMessageBox(_T("저장하였습니다."));

	EndDialog(IDOK);
}

VOID CMarcAdjMgrMemoDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (!GetSafeHwnd()) return;
	if (!GetDlgItem(IDC_eMEMO_MA)->GetSafeHwnd()) return;

	INT nHeight = 50;

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcWnd = rcClient;
	rcWnd.bottom -= nHeight;

	GetDlgItem(IDC_eMEMO_MA)->MoveWindow(rcWnd);

	rcWnd = rcClient;
	rcWnd.top = rcWnd.bottom - nHeight - 5;
	GetDlgItem(IDC_GROUP_MA)->MoveWindow(rcWnd);

	rcWnd = rcClient;
	rcWnd.top = rcWnd.bottom - nHeight;
	rcWnd.left = rcClient.right - 180;
	rcWnd.right = rcWnd.left + 80;
	rcWnd.DeflateRect(0, 5);
	GetDlgItem(IDC_bSAVE_MA)->MoveWindow(rcWnd);

	rcWnd = rcClient;
	rcWnd.top = rcWnd.bottom - nHeight;
	rcWnd.left = rcClient.right - 90;
	rcWnd.right = rcWnd.left + 80;
	rcWnd.DeflateRect(0, 5);
	GetDlgItem(IDCANCEL)->MoveWindow(rcWnd);
}

HBRUSH CMarcAdjMgrMemoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
