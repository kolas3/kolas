#include "stdafx.h"
#include "GroupLibListDlg.h"
#include "FrameManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CGroupLibListDlg::CGroupLibListDlg(CESL_Mgr* pParent, CBL_LOC_LOAN_PROC *pLoan)
	: CESL_Mgr(CGroupLibListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroupLibListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pGrid = NULL;
	m_pLoan = pLoan;
	m_strLibCode = _T("");
}
CGroupLibListDlg::~CGroupLibListDlg()
{
	if(NULL != m_pDM)
	{
		m_pDM->FreeData();
	}
}

void CGroupLibListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupLibListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroupLibListDlg, CDialog)
	//{{AFX_MSG_MAP(CGroupLibListDlg)
	ON_BN_CLICKED(IDC_btnLibListSearch, OnbtnLibListSearch)
	ON_BN_CLICKED(IDC_btnLibListSelect, OnbtnLibListSelect)
	ON_BN_CLICKED(IDC_btnLibListClose, OnbtnLibListClose)	
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupLibListDlg message handlers

BOOL CGroupLibListDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
 	{		
		if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_edtLibListSearch)->m_hWnd == pMsg->hwnd )
			{
				OnbtnLibListSearch();
			}
			if(GetDlgItem(IDC_btnLibListSearch)->m_hWnd == pMsg->hwnd )
			{
				OnbtnLibListSearch();
			}
			if(GetDlgItem(IDC_btnLibListSelect)->m_hWnd == pMsg->hwnd )
			{
				OnbtnLibListSelect();
			}
			if(GetDlgItem(IDC_btnLibListClose)->m_hWnd == pMsg->hwnd )
			{
				OnbtnLibListClose();
			}
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CGroupLibListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	if(InitESL_Mgr(_T("SM_GROUP_LIB_LIST_DLG")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	m_pDM = FindDM(_T("DM_GROUP_LIB_LIST_DLG"));
	if(NULL == m_pDM)
	{
		AfxMessageBox(_T("DM_GROUP_LIB_LIST_DLG"));
		return FALSE;
	}

	((CButton*)GetDlgItem(IDC_radLibListUnity))->SetCheck(TRUE);
	
	m_pGrid = (CESL_Grid*)FindControl(_T("CM_GROUP_LIB_LIST_DLG"), _T("MainGrid"));
	if(NULL == m_pGrid)
	{
		AfxMessageBox(_T("CM_GROUP_LIB_LIST_DLG-MainGrid"));
		return FALSE;
	}	 

	OnbtnLibListSearch();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CGroupLibListDlg::DisplayGrid()
{
	AllControlDisplay(_T("CM_GROUP_LIB_LIST_DLG"));
}

void CGroupLibListDlg::OnbtnLibListSearch() 
{
	CString strWhere;
	CString strLibInfo;
	GetDlgItemText(IDC_edtLibListSearch, strLibInfo);
	strLibInfo.Replace(_T("#"), _T(""));
	strLibInfo.TrimLeft(); strLibInfo.TrimRight();

	if(!strLibInfo.IsEmpty())
	{
		strWhere.Format(_T(" LIB_NAME like '%%%s%%' "), strLibInfo);;
		INT nMode = 0;
		if(6 == strLibInfo.GetLength())
		{
			if(TRUE == m_pLoan->IsNumericText(strLibInfo))
			{
				strWhere.Format(_T(" LIB_CODE = '%s' "), strLibInfo);
			}
		}
	}
	
	m_pDM->RefreshDataManager(strWhere);
	INT	nDMRowCnt	=	m_pDM->GetRowCount();
	if( 1 > nDMRowCnt )
	{
		AfxMessageBox( _T("도서관 정보가 없습니다.") , MB_ICONINFORMATION );
	}
	DisplayGrid();

}

void CGroupLibListDlg::OnbtnLibListSelect() 
{
 	if(NULL == m_pDM) return;
 	if(1 > m_pDM->GetRowCount()) return;
 	if(NULL == m_pGrid) return;
 	INT nIdx = m_pGrid->GetIdx();
	if(0 > nIdx) return;
 	
 	CString strLibcode, strLibName;
 	m_pDM->GetCellData(_T("도서관부호"), nIdx, strLibcode);
	m_pDM->GetCellData(_T("도서관명"), nIdx, strLibName);
 	if(!strLibcode.IsEmpty() && !strLibName.IsEmpty())
	{
		m_strLibCode = strLibcode;
		m_strLibName = strLibName;
		OnOK();
	}
}

void CGroupLibListDlg::OnbtnLibListClose() 
{

	OnCancel();
}


HBRUSH CGroupLibListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_stcLibListString == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(0,70,213));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}

void CGroupLibListDlg::OnClose() 
{
	
	CDialog::OnClose();
}


BEGIN_EVENTSINK_MAP(CGroupLibListDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CLibListDlg)
	ON_EVENT(CGroupLibListDlg, IDC_gridLibListGrid, -601 /* DblClick */, OnDblClickgridLibListGrid, VTS_NONE)
	ON_EVENT(CGroupLibListDlg, IDC_gridLibListGrid, -604 /* KeyUp */, OnKeyUpgridLibListGrid, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CGroupLibListDlg::OnDblClickgridLibListGrid() 
{
	if(1 > m_pDM->GetRowCount()) return;
	if(1 > m_pGrid->GetMouseRow()) return;
	OnbtnLibListSelect();
}

void CGroupLibListDlg::OnKeyUpgridLibListGrid(short FAR* KeyCode, short Shift) 
{
	if(VK_ESCAPE == *KeyCode)
	{
		PostMessage(WM_KEYDOWN, VK_ESCAPE, 0);
	}
	else if(VK_RETURN == *KeyCode)
	{
		PostMessage(WM_KEYDOWN, VK_RETURN, 0);
	}
}
