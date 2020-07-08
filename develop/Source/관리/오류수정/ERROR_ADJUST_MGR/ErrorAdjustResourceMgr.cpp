// ErrorAdjustResourceMgr.cpp: implementation of the CErrorAdjustResourceMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ErrorAdjustResourceMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CErrorAdjustResourceMgr::CErrorAdjustResourceMgr(CESL_Mgr *pParentESLMgr /*= NULL */)
{
	m_pFrameWnd = NULL;
	m_pParentESLMgr = pParentESLMgr;
	
	m_nResourceID = 31001;
	
	// default bitmap
	m_strBitmapNormalPath	= _T("..\\bmp\\normal.bmp");
	m_strBitmapHotPath		= _T("..\\bmp\\hot.bmp");
	m_strBitmapDisabledPath	= _T("..\\bmp\\disabled.bmp");
}

CErrorAdjustResourceMgr::~CErrorAdjustResourceMgr()
{
	FreeResourceList();
}

VOID CErrorAdjustResourceMgr::FreeResourceList()
{
	VOID *pClass = NULL;
	
	// ToolBar
	FreePtrList(&m_listToolBar, static_cast<CToolBar*>(pClass));
	
	// ToolBarItemList
	FreePtrList(&m_listToolBarItem, static_cast<CToolBarItem*>(pClass));
	
	// ImageList
	FreePtrList(&m_listImageList, static_cast<CImageList*>(pClass));
	
	// ToolTipList
	FreePtrList(&m_listToolTip, static_cast<CToolTipCtrl*>(pClass));
	
	// Menu
	//VOID *pClass = NULL;
	FreePtrList(&m_listMenu, static_cast<CMenu*>(pClass));
	
	// MenuItemList
	FreePtrList(&m_listMenuItem, static_cast<CMenuItem*>(pClass));
	
}

VOID CErrorAdjustResourceMgr::FreeToolBarResourceList()
{
	VOID *pClass = NULL;
	
	// ToolBar
	FreePtrList(&m_listToolBar, static_cast<CToolBar*>(pClass));
	
	// ToolBarItemList
	FreePtrList(&m_listToolBarItem, static_cast<CToolBarItem*>(pClass));
	
	// ImageList
	FreePtrList(&m_listImageList, static_cast<CImageList*>(pClass));
	
	// ToolTipList
	FreePtrList(&m_listToolTip, static_cast<CToolTipCtrl*>(pClass));
	
}

template <class T> VOID CErrorAdjustResourceMgr::FreePtrList(CPtrList *pPtrList, T *pClass)
{
	if (pPtrList == NULL) return;
	
	pClass = NULL;
	POSITION pos = pPtrList->GetHeadPosition();
	while (pos)
	{
		pClass = static_cast<T*>(pPtrList->GetNext(pos));
		if (pClass)
		{
			delete pClass;
			pClass = NULL;
		}
	}
	
	pPtrList->RemoveAll();
}

INT CErrorAdjustResourceMgr::CreateResource(CFrameWnd *pFrameWnd, ERROR_ADJUST_TYPE nErrorAdjustType)
{
	m_pFrameWnd = pFrameWnd;
	
	InitMenu();
	InitToolBar(nErrorAdjustType);
	
	CreateMenu();
	CreateToolBar();
	CreateDialogBar();
	
	m_pFrameWnd->RecalcLayout();
	
	return 0;
}

INT CErrorAdjustResourceMgr::InitMenu()
{
	INT nIdx = 0;
		
	CreateMenuItem(nIdx, _T("구입정보오류수정(&P)"), _T("PURCHASEINFO_ERROR_ADJUST"), _T("구입정보오류수정"));
	CreateMenuItem(nIdx, _T("권호정보색인동기화(&S)"), _T("VOLINDEX_ERROR_ADJUST"), _T("권호정보색인동기화"));
	CreateMenuItem(nIdx, _T("색인재구축(&R)"), _T("INDEX_ERROR_ADJUST"), _T("색인재구축"));
	CreateMenuItem(nIdx, _T("쓰레기자료관리(&W)"), _T("WASTEDATA_ERROR_ADJUST"), _T("쓰레기자료관리"));
	CreateMenuItem(nIdx, _T("재활용자료복원(&Y)"), _T("RECYCLE_DATA_RESTORE"), _T("재활용자료복원"));
	CreateMenuItem(nIdx, _T("비치희망색인재구축(&Q)"), _T("REQUEST_ERROR_ADJUST"), _T("비치희망색인재구축"));
	CreateMenuItem(nIdx, _T("종정보오류수정(&Q)"), _T("SPECIESINFO_ERROR_ADJUST"), _T("종정보오류수정"));
	CreateMenuItem(nIdx, _T("관리번호오류수정(&Q)"), _T("MANAGENO_ERROR_ADJUST"), _T("관리번호오류수정"));
	CreateMenuItem(nIdx, _T("마크정보오류수정(&M)"), _T("MARCDATA_ERROR_ADJUST"), _T("마크정보오류수정"));
	
	return 0;
}

INT CErrorAdjustResourceMgr::InitToolBar(ERROR_ADJUST_TYPE nErrorAdjustType)
{
	INT nIdx = 0;
	
	FreeToolBarResourceList();
	
	INT ids;
	if(nErrorAdjustType == ERROR_ADJUST_TYPE_INVALID) return -1;
	else if(nErrorAdjustType == PURCHASEINFO_ERROR_ADJUST)
		ids = InitToolBarOfPurchaseError();
	else if(nErrorAdjustType == VOLINDEX_ERROR_ADJUST)
		ids = InitToolBarOfVolIndexError();
	else if(nErrorAdjustType == INDEX_ERROR_ADJUST)
		ids = InitToolBarOfIndexError();
	else if(nErrorAdjustType == WASTEDATA_ERROR_ADJUST)
		ids = InitToolBarOfWasteDataError();
	else if(nErrorAdjustType == RECYCLE_DATA_RESTORE)
		ids = InitToolBarOfRecycleDataRestore();
	else if(nErrorAdjustType == REQUEST_ERROR_ADJUST)
		ids = InitToolBarOfRequestError();
	else if(nErrorAdjustType == SPECIESINFO_ERROR_ADJUST)
		ids = InitToolBarOfSpeciesInfoError();
	else if(nErrorAdjustType == MANAGENO_ERROR_ADJUST)
		ids = InitToolBarOfManageNoError();
	else if(nErrorAdjustType == MARCDATA_ERROR_ADJUST)
		ids = InitToolBarOfMarcDataError();

	return 0;
}

INT CErrorAdjustResourceMgr::InitToolBarOfPurchaseError()
{
	INT nIdx = 0;
	
	CreateToolBarItem(nIdx, _T("검색"), _T("검색"), _T("오류자료를 검색합니다."),_T(""),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("구입정보생성"), _T("구입정보생성"), _T("구입정보를 생성합니다."),_T(""),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("새로고침"), _T("새로고침"), _T("오류 정보를 새로구성합니다."),_T(""),_T("..\\bmp\\354_on.bmp"),_T("..\\bmp\\354_on.bmp"),_T("..\\bmp\\354_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("닫기"), _T("닫기"), _T("오류수정 창을 닫습니다."),_T(""),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_dimd.bmp"));
	
	return 0;
}

INT CErrorAdjustResourceMgr::InitToolBarOfIndexError()
{
	INT nIdx = 0;
	
	CreateToolBarItem(nIdx, _T("검색"), _T("검색"), _T("오류자료를 검색합니다."),_T(""),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("색인재구축"), _T("색인재구축"), _T("색인정보를 재구축합니다."),_T(""),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("닫기"), _T("닫기"), _T("오류수정 창을 닫습니다."),_T(""),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_dimd.bmp"));
	
	return 0;
}

INT CErrorAdjustResourceMgr::InitToolBarOfVolIndexError()
{
	INT nIdx = 0;
	
	CreateToolBarItem(nIdx, _T("검색"), _T("검색"), _T("오류자료를 검색합니다."),_T(""),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("책색인동기화"), _T("책색인동기화"), _T("책정보와 색인정보를 동기화 시켜줍니다."),_T(""),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("책부록플래그설정"), _T("책부록플래그설정"), _T("책정보의 책/부록 플래그를 설정합니다."),_T(""),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("새로고침"), _T("새로고침"), _T("오류 정보를 새로구성합니다."),_T(""),_T("..\\bmp\\354_on.bmp"),_T("..\\bmp\\354_on.bmp"),_T("..\\bmp\\354_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("닫기"), _T("닫기"), _T("오류수정 창을 닫습니다."),_T(""),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_dimd.bmp"));
	
	return 0;
}

INT CErrorAdjustResourceMgr::InitToolBarOfWasteDataError()
{
	INT nIdx = 0;
	
	CreateToolBarItem(nIdx, _T("검색"), _T("검색"), _T("오류자료를 검색합니다."),_T(""),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("쓰레기자료삭제"), _T("쓰레기자료삭제"), _T("쓰레기자료를 삭제합니다."),_T(""),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("새로고침"), _T("새로고침"), _T("오류 정보를 새로구성합니다."),_T(""),_T("..\\bmp\\354_on.bmp"),_T("..\\bmp\\354_on.bmp"),_T("..\\bmp\\354_dimd.bmp"));
	//CreateToolBarItem(nIdx, _T("오류정보보기"), _T("오류정보보기"), _T("상세오류정보를 보여줍니다."),_T(""),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("닫기"), _T("닫기"), _T("오류수정 창을 닫습니다."),_T(""),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_dimd.bmp"));
	
	return 0;
}

INT CErrorAdjustResourceMgr::InitToolBarOfRecycleDataRestore()
{
	INT nIdx = 0;
	
	CreateToolBarItem(nIdx, _T("검색"), _T("검색"), _T("재활용자료를 검색합니다."),_T(""),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("재활용자료복원"), _T("재활용자료복원"), _T("재활용자료를 원래 상태로 복원 합니다."),_T(""),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("닫기"), _T("닫기"), _T("오류수정 창을 닫습니다."),_T(""),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_dimd.bmp"));
	
	return 0;
}

INT CErrorAdjustResourceMgr::InitToolBarOfRequestError()
{
	INT nIdx = 0;
	
	CreateToolBarItem(nIdx, _T("검색"), _T("검색"), _T("재활용자료를 검색합니다."),_T(""),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("비치희망색인재구축"), _T("비치희망색인재구축"), _T("비치희망자료의 색인 정보를 재구축한다."),_T(""),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("닫기"), _T("닫기"), _T("오류수정 창을 닫습니다."),_T(""),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_dimd.bmp"));
	
	return 0;	
}

INT CErrorAdjustResourceMgr::InitToolBarOfSpeciesInfoError()
{
	INT nIdx = 0;
	
	CreateToolBarItem(nIdx, _T("검색"), _T("검색"), _T("종정보오류 자료를 검색합니다."),_T(""),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("종정보오류수정"), _T("종정보오류수정"), _T("재활용시 삭제된 종정보를 복구한다."),_T(""),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("닫기"), _T("닫기"), _T("오류수정 창을 닫습니다."),_T(""),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_dimd.bmp"));
	
	return 0;	
}

INT CErrorAdjustResourceMgr::InitToolBarOfManageNoError()
{
	INT nIdx = 0;
	
	CreateToolBarItem(nIdx, _T("검색"), _T("검색"), _T("관리번호 오류자료를 검색합니다."),_T(""),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("관리번호오류수정"), _T("관리번호오류수정"), _T("데이터변환시 등록번호가 잘못 부여된 자료를 삭제한다."),_T(""),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("닫기"), _T("닫기"), _T("오류수정 창을 닫습니다."),_T(""),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_dimd.bmp"));
	
	return 0;	
}

INT CErrorAdjustResourceMgr::CreateMenu()
{
	CMenu *pSystemMenu = new CMenu;
	pSystemMenu->CreateMenu();
	
	m_listMenu.AddTail(pSystemMenu);
	
	//CString strKey;
	//INT nValue;
	
	CMenuItem *pMenuItem = NULL;
	POSITION posMenuItem = NULL;
	
	posMenuItem = m_listMenuItem.GetHeadPosition();

	while (posMenuItem)
	{
		pMenuItem = static_cast<CMenuItem*>(m_listMenuItem.GetNext(posMenuItem));
		if (pMenuItem == NULL) continue;
			
		pSystemMenu->AppendMenu(MF_STRING | MF_ENABLED, pMenuItem->m_nID, pMenuItem->m_strCaption);
	}

	m_pFrameWnd->SetMenu(pSystemMenu);
	
	/*
	POSITION posMenuGroup = m_mapMenuGroup.GetStartPosition();
	while (posMenuGroup)
	{
		m_mapMenuGroup.GetNextAssoc(posMenuGroup, strKey, nValue);
		if (strKey.IsEmpty()) continue;
		
		// Crete SubMenu
		CMenu *pMenu = new CMenu;
		pMenu->CreateMenu();
		m_listMenu.AddTail(pMenu);
		
		posMenuItem = m_listMenuItem.GetHeadPosition();
		while (posMenuItem)
		{
			pMenuItem = static_cast<CMenuItem*>(m_listMenuItem.GetNext(posMenuItem));
			if (pMenuItem == NULL) continue;
			if (strKey != pMenuItem->m_strGroupAlias) continue;
			
			
			pMenu->AppendMenu(MF_STRING | MF_ENABLED, pMenuItem->m_nID, pMenuItem->m_strCaption);
		}
		
		// Append SubMenu
		pSystemMenu->AppendMenu(MF_POPUP, (UINT)pMenu->m_hMenu, strKey);
		pMenu->Detach();
	}
	
	m_pFrameWnd->SetMenu(pSystemMenu);
	*/
	
	return 0;
}

INT CErrorAdjustResourceMgr::CreateMenuItem(INT &nIdx, LPCTSTR lpszGroupAlias, LPCTSTR lpszAlias, LPCTSTR lpszCaption)
{
	CMenuItem *pMenuItem = new CMenuItem;
	m_listMenuItem.AddTail(pMenuItem);
	
	m_mapMenuGroup.SetAt(lpszGroupAlias, 1);
	
	pMenuItem->m_nID = ++m_nResourceID;
	pMenuItem->m_nIndex = nIdx++;
	pMenuItem->m_strGroupAlias = lpszGroupAlias;
	pMenuItem->m_strAlias = lpszAlias;
	pMenuItem->m_strCaption = lpszCaption;
	
	return 0;
}

INT CErrorAdjustResourceMgr::CreateToolBar()
{
	CToolBar *pToolBar = new CToolBar;
	m_listToolBar.AddTail(pToolBar);

	//if (!pToolBar->CreateEx(m_pFrameWnd, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP |
		//CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))

	if (!pToolBar->CreateEx(m_pFrameWnd, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP |
		CBRS_TOOLTIPS | CBRS_FLYBY))
	{
		delete pToolBar;
		return NULL;
	}

	pToolBar->SetWindowText(_T("오류수정 도구모음"));

	pToolBar->EnableDocking(CBRS_ALIGN_TOP);
	//m_pFrameWnd->DockControlBar(pToolBar);

	// Bitmap
	CBitmap bmpNormal;
	CBitmap bmpHot;
	CBitmap bmpDisabled;

	// Create ToolTip
	CToolTipCtrl *pToolTipCtrl = new CToolTipCtrl;
	pToolTipCtrl->Create(m_pFrameWnd, TTS_ALWAYSTIP);
	m_listToolTip.AddTail((CObject*)pToolTipCtrl);

	// Create ImageList
	CImageList * pImgListNormal = new CImageList;
	pImgListNormal->Create(BMP_SIZE, BMP_SIZE, ILC_COLOR24, 0, 4);
	
	CImageList * pImgListHot = new CImageList;
	pImgListHot->Create(BMP_SIZE, BMP_SIZE, ILC_COLOR24, 0, 4);
		
	CImageList * pImgListDisabled = new CImageList;
	pImgListDisabled->Create(BMP_SIZE, BMP_SIZE, ILC_COLOR24, 0, 4);

	m_listImageList.AddTail(pImgListNormal);
	m_listImageList.AddTail(pImgListHot);
	m_listImageList.AddTail(pImgListDisabled);

	// Init ToolBar
	CToolBarCtrl &rpToolBarCtrl = pToolBar->GetToolBarCtrl();
	if (!::IsWindow(rpToolBarCtrl.GetSafeHwnd())) return -1;
	while (rpToolBarCtrl.GetButtonCount() > 0)
		rpToolBarCtrl.DeleteButton(0);

	// Button Size
	INT nButtonWidth = GetButtonWidth();	
	nButtonWidth = nButtonWidth + USER_BSIZE_WIDTH;
	rpToolBarCtrl.SetButtonSize(CSize(nButtonWidth, USER_BSIZE_HIGHT));
		
	rpToolBarCtrl.SetMaxTextRows(2);
	rpToolBarCtrl.SetDrawTextFlags(DT_RIGHT, DT_WORDBREAK | DT_EDITCONTROL | DT_RIGHT);
	
	// Set ToolBar
	TCHAR szBuf[128];
	CToolBarItem *pToolBarItem = NULL;
	POSITION pos = m_listToolBarItem.GetHeadPosition();
	while (pos)
	{
		pToolBarItem = static_cast<CToolBarItem*>(m_listToolBarItem.GetNext(pos));
		if (pToolBarItem == NULL) continue;
		
		TBBUTTON TBButton;
		
		TBButton.iBitmap	= pToolBarItem->m_nIndex;
		TBButton.idCommand	= pToolBarItem->m_nID;
		TBButton.fsState	= TBSTATE_ENABLED;
		TBButton.fsStyle	= TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;
		_stprintf(szBuf, _T("%s\0"), pToolBarItem->m_strCaption);
		TBButton.iString	= rpToolBarCtrl.AddStrings(szBuf);
		TBButton.dwData		= 0;
		
		if (pToolBarItem->m_strStyle == _T("S"))
			TBButton.fsStyle = TBSTYLE_SEP;
		else if (pToolBarItem->m_strType == _T("R"))
			TBButton.fsState = TBSTATE_ENABLED;
		else if (pToolBarItem->m_strType == _T("I"))
			TBButton.fsState = TBSTATE_HIDDEN;
		else if (pToolBarItem->m_strType == _T("D"))
			TBButton.fsState = TBSTATE_INDETERMINATE;
		
		// Normal bitmap file check
		if (!IsFileExist(pToolBarItem->m_strBitmapNormal))
		{
			// Default Check
			if (!IsFileExist(m_strBitmapNormalPath))
			{
				AfxMessageBox(_T("Default Bitmap이 없습니다."));
				return -1;
			}
			else
				pToolBarItem->m_strBitmapNormal = m_strBitmapNormalPath;
		}
		
		// Hot bitmap file check
		if (!IsFileExist(pToolBarItem->m_strBitmapHot))
		{
			// Default Check
			if (!IsFileExist(m_strBitmapHotPath))
			{
				AfxMessageBox(_T("Default Bitmap이 없습니다."));
				return -1;
			}
			else
				pToolBarItem->m_strBitmapHot = m_strBitmapHotPath;
		}
		
		// Disabled bitmap file check
		if (!IsFileExist(pToolBarItem->m_strBitmapDisabled))
		{
			// Default Check
			if (!IsFileExist(m_strBitmapDisabledPath))
			{
				AfxMessageBox(_T("Default Bitmap이 없습니다."));
				return -1;
			}
			else
				pToolBarItem->m_strBitmapDisabled = m_strBitmapDisabledPath;
		}
		
		
		// attach bitmap
		bmpNormal.Attach(::LoadImage(NULL, pToolBarItem->m_strBitmapNormal, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
		bmpHot.Attach(::LoadImage(NULL, pToolBarItem->m_strBitmapHot, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
		bmpDisabled.Attach(::LoadImage(NULL, pToolBarItem->m_strBitmapDisabled, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
		
		ReplaceBackgroundColor(bmpNormal);
		ReplaceBackgroundColor(bmpHot);
		ReplaceBackgroundColor(bmpDisabled);
		
		pImgListNormal->Add(&bmpNormal, RGB(0, 0, 0));
		pImgListHot->Add(&bmpHot, RGB(0, 0, 0));
		pImgListDisabled->Add(&bmpDisabled, RGB(0, 0, 0));
		
		bmpNormal.Detach();
		bmpHot.Detach();
		bmpDisabled.Detach();
		
		rpToolBarCtrl.InsertButton(pToolBarItem->m_nIndex, &TBButton);
		
		pToolBar->SetButtonText(pToolBarItem->m_nIndex, pToolBarItem->m_strCaption);
		
		// tooltip..
		CRect rect;
		rpToolBarCtrl.GetItemRect(pToolBarItem->m_nIndex, rect);
		pToolTipCtrl->AddTool(pToolBar, pToolBarItem->m_ToolTip, rect, pToolBarItem->m_nID);

	}

	rpToolBarCtrl.SetImageList(pImgListNormal);
	rpToolBarCtrl.SetHotImageList(pImgListHot);
	rpToolBarCtrl.SetDisabledImageList(pImgListDisabled);

	rpToolBarCtrl.SetToolTips(pToolTipCtrl);

	rpToolBarCtrl.AutoSize();
	
	return 0;
}

VOID CErrorAdjustResourceMgr::ReplaceBackgroundColor(CBitmap& ioBM)
{
	// figure out how many pixels there are in the bitmap
	BITMAP		bmInfo;
	
	VERIFY (ioBM.GetBitmap (&bmInfo));
	
	// add support for additional bit depths here if you choose
	if (bmInfo.bmBitsPixel != 24) return;
	VERIFY (bmInfo.bmBitsPixel == 24);
	VERIFY (bmInfo.bmWidthBytes == (bmInfo.bmWidth * 3));
	
	const UINT		numPixels (bmInfo.bmHeight * bmInfo.bmWidth);
	
	// get a pointer to the pixels
    DIBSECTION  ds;
	VERIFY (ioBM.GetObject (sizeof (DIBSECTION), &ds) == sizeof (DIBSECTION));
	
	RGBTRIPLE*		pixels = reinterpret_cast<RGBTRIPLE*>(ds.dsBm.bmBits);
	VERIFY (pixels != NULL);
	
	// get the user's preferred button color from the _tsystem
	const COLORREF		buttonColor (::GetSysColor (COLOR_BTNFACE));
	const RGBTRIPLE		userBackgroundColor = { GetBValue (buttonColor), GetGValue (buttonColor), GetRValue (buttonColor)};
		
	//RGBTRIPLE	kBackgroundColor = {192, 192, 192};
	RGBTRIPLE	kBackgroundColor = {200, 208, 212};
	// search through the pixels, substituting the user's button
	// color for any pixel that has the magic background color
	for (UINT i = 0; i < numPixels; ++i)
	{
		if (pixels [i].rgbtBlue == kBackgroundColor.rgbtBlue &&
			pixels [i].rgbtGreen == kBackgroundColor.rgbtGreen &&
			pixels [i].rgbtRed == kBackgroundColor.rgbtRed)
		{
			pixels [i] = userBackgroundColor;
		}
	}
}

INT CErrorAdjustResourceMgr::CreateToolBarItem(INT &nIdx, 
											 LPCTSTR lpszAlias, 
											 LPCTSTR lpszCaption, 
											 LPCTSTR lpszToolTip,
											 LPCTSTR lpszStyle, 
											 LPCTSTR lpszBitmapNormal,
											 LPCTSTR lpszBitmapHot,
											 LPCTSTR lpszBitmapDisabled
											 )
{
	
	CToolBarItem *pToolBarItem = new CToolBarItem;
	m_listToolBarItem.AddTail(pToolBarItem);
	pToolBarItem->m_nID = ++m_nResourceID;
	pToolBarItem->m_nIndex = nIdx++;
	pToolBarItem->m_strAlias = lpszAlias;
	pToolBarItem->m_strCaption = lpszCaption;
	pToolBarItem->m_ToolTip = lpszToolTip;
	pToolBarItem->m_strStyle = lpszStyle;
	
	
	pToolBarItem->m_strBitmapNormal = lpszBitmapNormal;
	pToolBarItem->m_strBitmapHot = lpszBitmapHot;
	pToolBarItem->m_strBitmapDisabled = lpszBitmapDisabled;
	
	
	return 0;
}

INT CErrorAdjustResourceMgr::CreateDialogBar()
{
/*
	m_dlgSpeciesInfo.pMain = NULL;
	if (!m_wndSpeciesInfo.Create(m_pFrameWnd, &m_dlgSpeciesInfo, CString(_T("종정보")), 
		IDD_CHECKIN_VOL_SPECIES, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	{
		TRACE0("Failed to create m_wndSpeciesInfo\n");
        return -1;      // fail to create
	}

	m_wndSpeciesInfo.EnableDocking(CBRS_ALIGN_BOTTOM);
	m_pFrameWnd->DockControlBar(&m_wndSpeciesInfo);

*/
	return 0;
}

BOOL CErrorAdjustResourceMgr::IsFileExist(LPCTSTR lpszFileName)
{
	CFile file;
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary))
		return FALSE;
	else
		file.Close();
	return TRUE;
}

BOOL CErrorAdjustResourceMgr::OnCmdMsg(UINT nID)
{
	POSITION pos = NULL;
		
	// Menu
	CMenuItem *pMenuItem = NULL;
	pos = m_listMenuItem.GetHeadPosition();
	while (pos)
	{
		pMenuItem = static_cast<CMenuItem*>(m_listMenuItem.GetNext(pos));
		if (pMenuItem && pMenuItem->m_nID == nID)
			return TRUE;
	}
	

	// ToolBar
	CToolBarItem *pToolBarItem = NULL;
	pos = m_listToolBarItem.GetHeadPosition();
	while (pos)
	{
		pToolBarItem = static_cast<CToolBarItem*>(m_listToolBarItem.GetNext(pos));
		if(pToolBarItem && pToolBarItem->m_nID == nID)
			return TRUE;
	}

	return FALSE;
}

BOOL CErrorAdjustResourceMgr::OnCommand(UINT nID, CString &strCommandAlias)
{
	POSITION pos = NULL;
	
	// Menu
	CMenuItem *pMenuItem = NULL;
	pos = m_listMenuItem.GetHeadPosition();
	while (pos)
	{
		pMenuItem = static_cast<CMenuItem*>(m_listMenuItem.GetNext(pos));
		if (pMenuItem && pMenuItem->m_nID == nID)
		{
			//if (!OnCommand(pMenuItem->m_strAlias))
				strCommandAlias = pMenuItem->m_strAlias;

			//::MessageBox(m_pFrameWnd->m_hWnd, pMenuItem->m_strAlias, _T("MARC 정리"), MB_OK);
			return TRUE;
		}
	}
	

	// ToolBar
	CToolBarItem *pToolBarItem = NULL;
	pos = m_listToolBarItem.GetHeadPosition();
	while (pos)
	{
		pToolBarItem = static_cast<CToolBarItem*>(m_listToolBarItem.GetNext(pos));
		if(pToolBarItem && pToolBarItem->m_nID == nID)
		{
			//if (!OnCommand(pToolBarItem->m_strAlias))
				strCommandAlias = pToolBarItem->m_strAlias;

			return TRUE;
		}
	}

	return FALSE;
}


BOOL CErrorAdjustResourceMgr::OnCommand(CString strCommandAlias)
{	
	if (strCommandAlias == _T("VIEW_IDD_CHECKIN_VOL_SPECIES"))
	{
		if (m_wndSpeciesInfo.IsWindowVisible())
			m_wndSpeciesInfo.ShowWindow(SW_HIDE);
		else
			m_wndSpeciesInfo.ShowWindow(SW_SHOW);

		m_pFrameWnd->DockControlBar(&m_wndSpeciesInfo, AFX_IDW_DOCKBAR_BOTTOM);
	}
	else return FALSE;

	m_pFrameWnd->RecalcLayout();

	return TRUE;
}

INT CErrorAdjustResourceMgr::GetButtonWidth()
{
	CString strTmp;
	INT nCount = 0;
	INT nCountSize = 0;
	INT nButtonSize = 0;
	INT nTmpSize = 0;
	
	
	CToolBarItem *pToolBarItem = NULL;
	POSITION pos = m_listToolBarItem.GetHeadPosition();
	while (pos)
	{
		pToolBarItem = static_cast<CToolBarItem*>(m_listToolBarItem.GetNext(pos));
		if (pToolBarItem == NULL) continue;
		
		strTmp = pToolBarItem->m_strCaption;
		
		strTmp.TrimLeft();
		strTmp.TrimRight();
		if(strTmp.GetLength() > 10)
		{
			nTmpSize = strTmp.GetLength() - 10;
			nCountSize = (nTmpSize * 5);
			if (nCountSize > nButtonSize)
			{
				nButtonSize = (nTmpSize * 5) + 2;
			}
		}
	}
	
	return nButtonSize;
}

INT CErrorAdjustResourceMgr::InitToolBarOfMarcDataError()
{
	INT nIdx = 0;
	
	CreateToolBarItem(nIdx, _T("검색"), _T("검색"), _T("오류자료를 검색합니다."),_T(""),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_on.bmp"),_T("..\\bmp\\348_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("마크수정"), _T("마크수정"), _T("마크정보를 수정합니다."),_T(""),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_on.bmp"),_T("..\\bmp\\351_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("닫기"), _T("닫기"), _T("오류수정 창을 닫습니다."),_T(""),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_dimd.bmp"));
	
	return 0;
}
