// DlgProperty.cpp : implementation file
//

#include "stdafx.h"
#include "SIReportEditor.h"
#include "DlgProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgProperty dialog


CDlgProperty::CDlgProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProperty::IDD, pParent)
{
	// Init property font
	LOGFONT logFont;
	ZeroMemory(&logFont,sizeof(logFont));
	_tcscpy(logFont.lfFaceName,_T("ü"));
	logFont.lfHeight = 12;
	logFont.lfCharSet = DEFAULT_CHARSET;
	
	m_fontListBox.CreateFontIndirect(&logFont);
	m_fontProperty.CreateFontIndirect(&logFont);

	//{{AFX_DATA_INIT(CDlgProperty)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// KOL.UDF.022 시큐어코딩 보완
	m_pParentWnd = pParent;
}

VOID CDlgProperty::Init()
{
EFS_BEGIN

	FreePropertyListCtrlList();
	m_tabCtrl.DeleteAllItems();

EFS_END
}

CDlgProperty::~CDlgProperty()
{
	FreePropertyListCtrlList();
}

VOID CDlgProperty::FreePropertyListCtrlList()
{
EFS_BEGIN

	INT nCount = m_listPropertyListCtrl.GetCount();
	if (nCount == 0) return;

	for (INT i = 0; i < nCount ; i++)
	{
		CPropertyListCtrl* pCtrl = (CPropertyListCtrl*)m_listPropertyListCtrl.RemoveHead();
		if (pCtrl != NULL)
		{
			delete pCtrl;
			pCtrl =  NULL;
		}
	}
	
	m_listPropertyListCtrl.RemoveAll();

EFS_END
}


VOID CDlgProperty::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProperty)
	DDX_Control(pDX, IDC_TAB_PROPERTY, m_tabCtrl);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CDlgProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgProperty)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PROPERTY, OnSelchangeTab)
	//}}AFX_MSG_MAP
	ON_MESSAGE(ID_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProperty Operations

VOID CDlgProperty::AddTabItem(CString strItemName)
{
EFS_BEGIN

	INT nItem = m_tabCtrl.GetItemCount();
	
	TC_ITEM tcItem;
	ZeroMemory(&tcItem,sizeof(tcItem));

	tcItem.mask = TCIF_IMAGE | TCIF_TEXT;
	tcItem.iImage = nItem;
	tcItem.pszText = strItemName.GetBuffer(0);
	
	m_tabCtrl.InsertItem(nItem, &tcItem);
	m_tabCtrl.SetItem(nItem, &tcItem);

	AddPropertyListCtrl(strItemName);

	m_tabCtrl.SetCurSel(-1);

EFS_END
}

VOID CDlgProperty::AddPropertyListCtrl(CString strAlias)
{
EFS_BEGIN

	CRect rect;
	m_tabCtrl.GetClientRect(rect);

	rect.top += 40;
	rect.left += 5;
	rect.right -= 5;
	rect.bottom -= 5;
	
	CPropertyListCtrl *pPropertyListCtrl = new CPropertyListCtrl;
	pPropertyListCtrl->SetAlias(strAlias);

	pPropertyListCtrl->Create(rect, this, WM_USER + 1000);
	pPropertyListCtrl->SetFont(&m_fontListBox);
	pPropertyListCtrl->SetBkColor(RGB(255,255,255));
	pPropertyListCtrl->SetTextColor(RGB(74,109,132));
	pPropertyListCtrl->SetTextHighlightColor(RGB(80,80,80));
	pPropertyListCtrl->SetHighlightColor(RGB(246,246,220));
	pPropertyListCtrl->SetPropertyBkColor(RGB(255,255,255));
	pPropertyListCtrl->SetPropertyTextColor(RGB(0,0,192));
	pPropertyListCtrl->SetBoldSelection(TRUE);
	pPropertyListCtrl->SetLineStyle(RGB(74,109,132), PS_SOLID);
	pPropertyListCtrl->ShowWindow(SW_HIDE);
	pPropertyListCtrl->SetFocus();

	m_listPropertyListCtrl.AddTail((CObject*)pPropertyListCtrl);

EFS_END
}

VOID CDlgProperty::AddProperty(CString strAlias, CString strText, CString strItemAlias, INT nType, CString strData, INT nSelected /*=0*/, INT nAlignment /* = DT_LEFT */)
{
EFS_BEGIN

	if (nType < 0) nType = 0;
	if (nSelected < 0) nSelected = 0;

	POSITION pos = m_listPropertyListCtrl.GetHeadPosition();
	while (pos != NULL)
	{
		CPropertyListCtrl* pCtrl = (CPropertyListCtrl*)m_listPropertyListCtrl.GetNext(pos);
		if (pCtrl != NULL && pCtrl->GetAlias() == strAlias)
		{
			pCtrl->AddString(strText, strItemAlias, nType, strData, nSelected, nAlignment);
		}
	}

EFS_END
}

VOID CDlgProperty::AddProperty(CString strAlias, CString strText, CString strItemAlias, COLORREF color, INT nAlignment /* = DT_LEFT */)
{
EFS_BEGIN

	POSITION pos = m_listPropertyListCtrl.GetHeadPosition();
	while (pos != NULL)
	{
		CPropertyListCtrl* pCtrl = (CPropertyListCtrl*)m_listPropertyListCtrl.GetNext(pos);
		if (pCtrl != NULL && pCtrl->GetAlias() == strAlias)
		{
			pCtrl->AddString(strText, strItemAlias, color, nAlignment);
		}
	}

EFS_END
}

VOID CDlgProperty::AddProperty(CString strAlias, CString strText, CString strItemAlias, LOGFONT logfont, COLORREF color, INT nAlignment /* = DT_LEFT */)
{
EFS_BEGIN

	POSITION pos = m_listPropertyListCtrl.GetHeadPosition();
	while (pos != NULL)
	{
		CPropertyListCtrl* pCtrl = (CPropertyListCtrl*)m_listPropertyListCtrl.GetNext(pos);
		if (pCtrl != NULL && pCtrl->GetAlias() == strAlias)
		{
			CFont font;
			font.CreateFontIndirect(&logfont);
			pCtrl->AddString(strText, strItemAlias, &font, color, nAlignment);
		}
	}

EFS_END
}

CPropertyListCtrl* CDlgProperty::GetCurrentPropertyListCtrl()
{
EFS_BEGIN

	CString strItemName;
	TCITEM tcItem;
	tcItem.mask = TCIF_IMAGE | TCIF_TEXT;
	tcItem.cchTextMax = MAX_PATH;
	tcItem.pszText = strItemName.GetBuffer(MAX_PATH);
	
	INT t = m_tabCtrl.GetCurSel();
	m_tabCtrl.GetItem(m_tabCtrl.GetCurSel(), &tcItem);
	strItemName.ReleaseBuffer();

	POSITION pos = m_listPropertyListCtrl.GetHeadPosition();
	while (pos != NULL)
	{
		CPropertyListCtrl* pCtrl = (CPropertyListCtrl*)m_listPropertyListCtrl.GetNext(pos);
		if (pCtrl != NULL && pCtrl->GetAlias() == strItemName)
			return pCtrl;
	}

	return NULL;

EFS_END
return NULL;

}

VOID CDlgProperty::SetTabFocus(INT nItem)
{
EFS_BEGIN

	m_tabCtrl.SetCurFocus(nItem);
	m_tabCtrl.SetCurSel(nItem);

EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// CDlgProperty message handlers

BOOL CDlgProperty::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	// Initialize tab ctrl
	m_imageListTab.Create(IDB_CONTROL, 24, 1, RGB(255,255,255));
			
	m_tabCtrl.SetImageList(&m_imageListTab);

	Init();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

BOOL CDlgProperty::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	m_pParentWnd = pParentWnd;
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CDlgProperty::OnClose() 
{
EFS_BEGIN

	ShowWindow(SW_HIDE);

EFS_END
}

VOID CDlgProperty::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	if (m_tabCtrl.GetSafeHwnd() == NULL) return;
	
	CRect rect;
	GetClientRect(rect);
	m_tabCtrl.MoveWindow(rect);

	rect.top += 40;
	rect.left += 5;
	rect.right -= 5;
	rect.bottom -= 5;
		
	POSITION pos = m_listPropertyListCtrl.GetHeadPosition();
	while (pos != NULL)
	{
		CPropertyListCtrl* pCtrl = (CPropertyListCtrl*)m_listPropertyListCtrl.GetNext(pos);
		if (pCtrl == NULL) continue;

		pCtrl->MoveWindow(rect);
	}

EFS_END
}

VOID CDlgProperty::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString strItemName;
	TCITEM tcItem;
	tcItem.mask = TCIF_IMAGE | TCIF_TEXT;
	tcItem.cchTextMax = MAX_PATH;
	tcItem.pszText = strItemName.GetBuffer(MAX_PATH);
	
	INT t = m_tabCtrl.GetCurSel();
	m_tabCtrl.GetItem(m_tabCtrl.GetCurSel(), &tcItem);
	strItemName.ReleaseBuffer();
		
	POSITION pos = m_listPropertyListCtrl.GetHeadPosition();
	while (pos != NULL)
	{
		CPropertyListCtrl* pCtrl = (CPropertyListCtrl*)m_listPropertyListCtrl.GetNext(pos);
		if (pCtrl != NULL && pCtrl->GetAlias() == strItemName)
		{
			pCtrl->ShowWindow(SW_SHOW);
			pCtrl->SetFocus();
		}
		else
		{
			pCtrl->ShowWindow(SW_HIDE);
		}
	}
		
	*pResult = 0;

EFS_END
}

LONG CDlgProperty::OnPropertyChanged(UINT wItemChanged, LONG lPropertyType)
{
EFS_BEGIN

	m_pParentWnd->PostMessage(ID_PROPERTY_CHANGED, wItemChanged, lPropertyType);
	return 0;

EFS_END
return -1;

}

BOOL CDlgProperty::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			if (GetFocus()->GetSafeHwnd() != GetSafeHwnd())
			{
				CPropertyListCtrl* pCtrl = GetCurrentPropertyListCtrl();
				
				PROPERTYITEM *pItem = pCtrl->GetCurItem();
				if (pItem != NULL && pItem->nType == ID_PROPERTY_TEXT_MULTI)
				{
					if (pMsg->wParam == VK_ESCAPE)
					{
						if (pCtrl->m_pMultiEditWnd != NULL)
							pCtrl->m_pMultiEditWnd->ShowWindow(SW_HIDE);
						return TRUE;
					}
				}
				else
				{
					if (pItem != NULL && pItem->nType == ID_PROPERTY_TEXT)
					{
						if (pCtrl->m_pEditWnd != NULL)
							pCtrl->m_pEditWnd->ShowWindow(SW_HIDE);
					}
					return TRUE;
				}

			}
		}
	}	
	
	return CDialog::PreTranslateMessage(pMsg);

EFS_END
return FALSE;

}
