// MNG_USER_MANAGER_04.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_USER_MANAGER_04.h"
#include "..\MNG_ACCESS_CONTROL\MNG_ACCESS_CONTROL_API.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_MANAGER_04 dialog


CMNG_USER_MANAGER_04::CMNG_USER_MANAGER_04(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_USER_MANAGER_04)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CMNG_USER_MANAGER_04::~CMNG_USER_MANAGER_04()
{
	
}

VOID CMNG_USER_MANAGER_04::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_USER_MANAGER_04)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMNG_USER_MANAGER_04, CDialog)
	//{{AFX_MSG_MAP(CMNG_USER_MANAGER_04)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()    
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_MANAGER_04 message handlers


BOOL CMNG_USER_MANAGER_04::Create(CWnd* pParentWnd)
{	
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd); 

	EFS_END
	return FALSE;

}	

VOID CMNG_USER_MANAGER_04::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	if( GetSafeHwnd() == NULL ) return;

	CWnd *pWnd = GetDlgItem(IDC_treeGROUP);

	if( pWnd->GetSafeHwnd() == NULL ) return ;
	
	pWnd->MoveWindow( 0 , 0 , cx , cy );

	EFS_END
}


INT CMNG_USER_MANAGER_04::InitMenuTreeCtrl()
{
	EFS_BEGIN

	m_imageList.Create( IDB_TREE, 8, 1, RGB( 0, 128, 128 ) );
	CTreeCtrl *pTreeCtrl = ( CTreeCtrl * )GetDlgItem( IDC_treeGROUP );
	if( pTreeCtrl == NULL) 
		return -1;
	pTreeCtrl->SetImageList(&m_imageList, TVSIL_NORMAL);

	return 0;

	EFS_END
	return -1;

}



VOID CMNG_USER_MANAGER_04::ClearMenuTree()
{
	EFS_BEGIN

	m_pDM_Menu->FreeData();
	DisplayMenuTree();	

	EFS_END
}



INT CMNG_USER_MANAGER_04::DisplayMenuTree()
{
	EFS_BEGIN

	CTreeCtrl *pTreeCtrl = (CTreeCtrl*)GetDlgItem(IDC_treeGROUP);
	if (pTreeCtrl == NULL) return -1;

	pTreeCtrl->DeleteAllItems();

	INT nRowCount = m_pDM_MenuGroup->GetRowCount();
	if (nRowCount <= 0) return -1;

	HTREEITEM hItem;
	HTREEITEM hRoot = pTreeCtrl->GetRootItem();
	TVINSERTSTRUCT tvInsert;

	CString strAlias;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (m_pDM_MenuGroup->GetCellData(_T("GROUPTYPE"), idx) != _T("T")) continue;

		CString strAlias = m_pDM_MenuGroup->GetCellData(_T("CAPTION"), idx);

		tvInsert.hParent = NULL;
		tvInsert.hInsertAfter = NULL;
		tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
		tvInsert.item.lParam = 0;
		tvInsert.item.pszText = strAlias.GetBuffer(0);
		tvInsert.item.iImage = 0;
		tvInsert.item.iSelectedImage = 0;
		
		
		hItem = pTreeCtrl->InsertItem(&tvInsert);
		pTreeCtrl->SetItemData(hItem, 100000);

		DisplayMenuTree(pTreeCtrl, hItem, m_pDM_MenuGroup->GetCellData(_T("TARGET"), idx));
		
	}

	return 0;

	EFS_END
	return -1;

}

INT CMNG_USER_MANAGER_04::DisplayMenuTree(CTreeCtrl *pTreeCtrl, HTREEITEM hParentItem, CString strAlias, INT nLevel/*=1*/)
{
	EFS_BEGIN

	if (pTreeCtrl == NULL) return -1;

	TVINSERTSTRUCT tvInsert;
	HTREEITEM hItem = NULL;

	CString sMenuAlias;

	if (nLevel == 1)
	{
		// Parse MenuGroups
		TCHAR *lpszToken = _tcstok(strAlias.GetBuffer(0), _T(","));
		while (lpszToken)
		{
			// MENU GROUP --> Level 2
			CString strMenuGroupAlias;
			strMenuGroupAlias.Format(_T("%s"), lpszToken);
			strMenuGroupAlias.TrimLeft();
			strMenuGroupAlias.TrimRight();

			for( INT i = 0 ; i < m_pDM_MenuGroup->GetRowCount() ; i++ )
			{
				m_pDM_MenuGroup->GetCellData( _T("ALIAS") , i , sMenuAlias );

				if( sMenuAlias.Compare(strMenuGroupAlias) == 0 )
					m_pDM_MenuGroup->GetCellData( _T("CAPTION") , i , strMenuGroupAlias );

				if( sMenuAlias.Compare(strMenuGroupAlias) == 0 ) break;
			}

			tvInsert.hParent = hParentItem;
			tvInsert.hInsertAfter = hItem;
			tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
			tvInsert.item.lParam = 0;
			tvInsert.item.pszText = strMenuGroupAlias.GetBuffer(0);
			tvInsert.item.iImage = 1;
			tvInsert.item.iSelectedImage = 1;
			
			
			hItem = pTreeCtrl->InsertItem(&tvInsert);
			pTreeCtrl->SetItemData(hItem, 100000);
			
			// MENU -- > Level 3
			DisplayMenuTree(pTreeCtrl, hItem, strMenuGroupAlias, 2);
			

			lpszToken = _tcstok(NULL, _T(","));
		}
	}
	else if (nLevel == 2)
	{
		// MENU -- > Level 3
		INT nRowCount = m_pDM_Menu->GetRowCount();
		for (INT idx = 0; idx < nRowCount; idx++)
		{
			if (m_pDM_Menu->GetCellData(_T("GROUP_ALIAS"), idx) != strAlias) continue;

			CString strCaption;
			strCaption = m_pDM_Menu->GetCellData(_T("CAPTION"), idx);

			tvInsert.hParent = hParentItem;
			tvInsert.hInsertAfter = hItem;
			tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
			tvInsert.item.pszText = strCaption.GetBuffer(0);

			if (m_pDM_Menu->GetCellData(_T("VISIBLE"), idx) == _T("T"))
			{
				tvInsert.item.iImage = 2;
				tvInsert.item.iSelectedImage = 2;
			}
			else
			{
				tvInsert.item.iImage = 3;
				tvInsert.item.iSelectedImage = 3;
			}

			
			hItem = pTreeCtrl->InsertItem(&tvInsert);
			pTreeCtrl->SetItemData(hItem, idx);
		}
	}
	
	return 0;

	EFS_END
	return -1;

}

BOOL CMNG_USER_MANAGER_04::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	m_pDM_Menu = FindDM( _T("DM_MNG_USER_P_MENU"));
	m_pDM_MenuGroup = FindDM( _T("DM_MNG_USER_METHOD_GROUP"));


	CRect WndPos;
	CRect ParentWndPos;
	pParentMgr->GetWindowRect( ParentWndPos );
	this->GetWindowRect( WndPos );
	this->MoveWindow(WndPos.top , WndPos.left , WndPos.Width()-1 , ParentWndPos.Height() );
	
//	InitMenuTreeCtrl();


	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CMNG_USER_MANAGER_04::RefreshMenuData( CString sID , INT nMode )
{
	EFS_BEGIN

	CString strOption;
	
	if( nMode == 0 )
		strOption.Format( _T("FLAG = 'U' AND USER_ID = '%s'"), sID );
	else
		strOption.Format( _T("FLAG = 'M' AND USER_ID = '%s'"), sID );

	CLocCommonAPI::RefreshDataManager( this , m_pDM_Menu, strOption );


	EFS_END
}

INT CMNG_USER_MANAGER_04::ChangeGroup( CString sID , INT nMode )
{
	EFS_BEGIN

	CTreeCtrl *pTreeCtrl = (CTreeCtrl*)GetDlgItem(IDC_treeGROUP);

	CString sFlag;

	if( nMode == 0 )
		sFlag = _T("U");
	else
		sFlag = _T("M");

	CMNG_ACCESS_CONTROL_API AccessApi;
	AccessApi.ShowMenuTree( m_pDM_Menu , pTreeCtrl , sFlag.GetAt(0) , sID );
	
	return 0;

	EFS_END
	return -1;

}


HBRUSH CMNG_USER_MANAGER_04::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
