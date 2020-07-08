// DlgClassCode.cpp : implementation file
//

#include "stdafx.h"
#include "DlgClassCode.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgClassCode dialog


CDlgClassCode::CDlgClassCode(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgClassCode::IDD, pParent), m_Grid(NULL)
{
	//{{AFX_DATA_INIT(CDlgClassCode)
	m_strKeyword = _T("");
	//}}AFX_DATA_INIT
}
CDlgClassCode::~CDlgClassCode()
{
	m_Dlg_brush.DeleteObject();
}

VOID CDlgClassCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgClassCode)
	DDX_Control(pDX, IDC_TAB_COM, m_ctrlTab);
	DDX_Text(pDX, IDC_eCLASS_CODE_KEYWORD, m_strKeyword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgClassCode, CDialog)
	//{{AFX_MSG_MAP(CDlgClassCode)
	ON_EN_CHANGE(IDC_eCLASS_CODE_KEYWORD, OnChangeeCLASSCODEKEYWORD)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgClassCode message handlers

BOOL CDlgClassCode::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (!m_Grid.SubclassDlgItem(IDC_MSHFLEXGRID_CODE, this))
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	m_ctrlTab.InsertItem(0, _T("부호"));
	m_ctrlTab.InsertItem(1, _T("설명"));

	Display();

	if (!m_strKeyword.IsEmpty())
	{
		FindRow();
	}

	m_Dlg_brush.CreateSolidBrush(RGB(239, 239, 239));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;
}

INT CDlgClassCode::Display()
{
	EFS_BEGIN

	m_Grid.Clear();
	m_Grid.SetRows(2);
	m_Grid.SetCols(0, 3);
	m_Grid.SetColWidth(0, 0, 0);
	m_Grid.SetColumnInfo(0, _T("부호"), 30, 4);
	m_Grid.SetColumnInfo(1, _T("설명"), 450, 1);

	m_Grid.SetScrollTrack(TRUE);

	INT nCount = m_mapClassCode.GetCount();
	if (nCount <= 0) return -1;

	INT idx = -1;
	
	CStringArray arrClassCode;
	
	CString strCode;
	CString strDesc;

	POSITION pos = m_mapClassCode.GetStartPosition();
	while (pos)
	{
		m_mapClassCode.GetNextAssoc(pos, strCode, strDesc);
		if (strCode.IsEmpty() || strDesc.IsEmpty()) continue;

		nCount = arrClassCode.GetSize();

		for (idx = 0; idx < nCount; idx++)
		{
			if (strCode < arrClassCode.GetAt(idx)) break;
		}

		arrClassCode.InsertAt(idx, strCode);
	}	

	m_Grid.SetRows(m_mapClassCode.GetCount() + 1);

	INT nRow = 0;
	CString strIdx;
	
	nCount = arrClassCode.GetSize();
	for (idx = 0; idx < nCount; idx++)
	{
		strCode = arrClassCode.GetAt(idx);
		if (!m_mapClassCode.Lookup(strCode, strDesc)) continue;

		strIdx.Format(_T("%d"), nRow + 1);

		m_Grid.SetTextMatrix(nRow + 1, 0, strIdx);
		m_Grid.SetTextMatrix(nRow + 1, 1, strCode);
		m_Grid.SetTextMatrix(nRow + 1, 2, strDesc);

		nRow++;
	}
	
	return 0;

	EFS_END
	return -1;
}

VOID CDlgClassCode::InitData()
{
	EFS_BEGIN

	m_mapClassCode.RemoveAll();

	EFS_END
}

VOID CDlgClassCode::AddData(CString strCode, CString strDesc)
{
	EFS_BEGIN

	if (strCode.IsEmpty() || strDesc.IsEmpty())
		return;

	m_mapClassCode.SetAt(strCode, strDesc);

	EFS_END
}

VOID CDlgClassCode::FindRow()
{
	EFS_BEGIN

	UpdateData();
	
	if (m_strKeyword.IsEmpty()) return;

	INT idx = m_ctrlTab.GetCurSel();
	if (idx == 0)
		idx = 1;
	else if (idx == 1)
		idx = 2;


	INT nRow = m_Grid.GetRows();
	for (INT i = 0; i < nRow; i++)
	{
		if (m_Grid.GetTextMatrix(i, idx).Find(m_strKeyword) >= 0)
		{
			m_Grid.SetTopRow(i);
			m_Grid.SetReverse(i - 1);
			return;
		}
	}

	EFS_END
}

VOID CDlgClassCode::OnOK() 
{
	EFS_BEGIN

	
	INT nRow = m_Grid.GetRow();
	m_strSelCode = m_Grid.GetTextMatrix(nRow, 1);
	m_strSelDesc = m_Grid.GetTextMatrix(nRow, 2);
	
	CDialog::OnOK();

	EFS_END
}

VOID CDlgClassCode::OnChangeeCLASSCODEKEYWORD() 
{
	EFS_BEGIN

	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
	if (!m_strKeyword.IsEmpty())
		FindRow();

	EFS_END	
}

BEGIN_EVENTSINK_MAP(CDlgClassCode, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlgClassCode)
	ON_EVENT(CDlgClassCode, IDC_MSHFLEXGRID_CODE, -605 /* MouseDown */, OnMouseDownMshflexgridCode, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgClassCode, IDC_MSHFLEXGRID_CODE, -601 /* DblClick */, OnDblClickMshflexgridCode, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CDlgClassCode::OnMouseDownMshflexgridCode(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	m_Grid.SetReverse(m_Grid.GetRow() - 1);	

	EFS_END
}

VOID CDlgClassCode::OnDblClickMshflexgridCode() 
{
	EFS_BEGIN

	OnOK();

	EFS_END
}

BOOL CDlgClassCode::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;
}

void CDlgClassCode::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_COM);
	if(pTab->GetSafeHwnd() == NULL) return;
	pTab->MoveWindow(3, 2, cx - 5, cy - 32);

	CMSHFlexGrid *pgrid = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID_CODE);
	if(pgrid->GetSafeHwnd() == NULL) return;
	pgrid->MoveWindow(8, 55, cx - 18, cy- 91);

	CButton *pbtnOk = (CButton*)GetDlgItem(IDOK);
	if(pbtnOk->GetSafeHwnd() == NULL) return;
	pbtnOk->MoveWindow(cx - 142, cy- 28, 60,23 );

	CButton *pbtnClose = (CButton*)GetDlgItem(IDCANCEL);
	if(pbtnClose->GetSafeHwnd() == NULL) return;
	pbtnClose->MoveWindow(cx - 72, cy- 28 , 60,23 );
	
}

HBRUSH CDlgClassCode::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=NULL;
	if(pWnd->m_hWnd==this->m_hWnd) hbr= HBRUSH(m_Dlg_brush);	
	else hbr =CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG) 
	{
		pDC->SetTextColor(RGB(0,0,0));		
		pDC->SetBkMode(OPAQUE);		
		pDC->SetBkColor(RGB(239,239,239));		
		
		return m_Dlg_brush;		
	}
	return hbr;
}
