// DlgControlBox.cpp : implementation file
//

#include "stdafx.h"
#include "SIReportEditor.h"
#include "DlgControlBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgControlBox dialog


CDlgControlBox::CDlgControlBox(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgControlBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgControlBox)
	//}}AFX_DATA_INIT
}


VOID CDlgControlBox::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgControlBox)
	DDX_Control(pDX, IDC_LIST_CONTROL, m_listCtrl);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CDlgControlBox, CDialog)
	//{{AFX_MSG_MAP(CDlgControlBox)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgControlBox message handlers

BOOL CDlgControlBox::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	InitListCtrl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CDlgControlBox::InitListCtrl()
{
EFS_BEGIN

	// Init ListCtrl
	m_listCtrl.SetExtendedStyle(m_listCtrl.GetExtendedStyle() | LVS_EX_INFOTIP);

	// make image list
	m_imageList.Create(48, 48, ILC_COLOR24, 0, 4);
	CBitmap bmp;
	
	// Edit Tool Bar
	CString strFileName;
	for (INT i = 1; i <= 5; i++)
	{
		strFileName.Format(_T("..\\bmp\\SIReportEditor\\icon48_%02d.bmp"), i);
		if (!CheckFileExist(strFileName)) return;

		bmp.Attach(::LoadImage(NULL, strFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
		m_imageList.Add(&bmp, RGB(0, 0, 0));

		bmp.Detach();

	}
	m_listCtrl.SetImageList(&m_imageList, LVSIL_NORMAL);

	// init column
	m_listCtrl.InsertColumn(0, _T("Object Name"));
	
	// Add Item
	m_listCtrl.InsertItem(0, _T("테이블"), 0);
	m_listCtrl.InsertToolTip(0, CString(_T("테이블을 생성합니다.")));
	
	m_listCtrl.InsertItem(1, _T("텍스트상자"), 1);
	m_listCtrl.InsertToolTip(1, CString(_T("텍스트상자를 생성합니다.")));
	
	m_listCtrl.InsertItem(2, _T("선"), 2);
	m_listCtrl.InsertToolTip(2, CString(_T("선을 생성합니다.")));
	
	m_listCtrl.InsertItem(3, _T("바코드"), 3);
	m_listCtrl.InsertToolTip(3, CString(_T("바코드를 생성합니다.")));

	m_listCtrl.InsertItem(4, _T("비트맵"), 4);
	m_listCtrl.InsertToolTip(4, CString(_T("비트맵을 생성합니다.")));


EFS_END
}

BOOL CDlgControlBox::CheckFileExist(CString strFileName)
{
EFS_BEGIN

	CFile file;
	if (!file.Open(strFileName, CFile::modeRead | CFile::typeBinary))
	{
		return FALSE;
	}
	else 
		file.Close();

	return TRUE;

EFS_END
return FALSE;

}

VOID CDlgControlBox::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);

	if (!m_listCtrl.GetSafeHwnd()) return;
	
	CRect rect;
	GetClientRect(rect);
	m_listCtrl.MoveWindow(rect);
	m_listCtrl.Arrange(LVA_DEFAULT);

EFS_END
}

BOOL CDlgControlBox::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}	
	return CDialog::PreTranslateMessage(pMsg);

EFS_END
return FALSE;

}
