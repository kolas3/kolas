// AcqGroupResourceMgr.cpp: implementation of the CAcqGroupResourceMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AcqGroupResourceMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAcqGroupResourceMgr::CAcqGroupResourceMgr(CESL_Mgr * pParentMgr/*=NULL*/)
{
	m_nGroupOpenType = GROUP_OPEN_TYPE_INVALID;
	m_bHaveOtherGroup = FALSE;
	m_pFrameWnd = NULL;
	m_pParentESLMgr = pParentMgr;
	
	m_nResourceID = 31001;
	
	// default bitmap
	m_strBitmapNormalPath	= _T("..\\bmp\\normal.bmp");
	m_strBitmapHotPath		= _T("..\\bmp\\hot.bmp");
	m_strBitmapDisabledPath	= _T("..\\bmp\\disabled.bmp");
	
	m_bHaveOtherGroup = CheckOtherAcqType();
}

CAcqGroupResourceMgr::~CAcqGroupResourceMgr()
{
	FreeResourceList();
}

VOID CAcqGroupResourceMgr::FreeResourceList()
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

VOID CAcqGroupResourceMgr::FreeToolBarResourceList()
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

template <class T> VOID CAcqGroupResourceMgr::FreePtrList(CPtrList *pPtrList, T *pClass)
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

INT CAcqGroupResourceMgr::CreateResource(CFrameWnd *pFrameWnd, ACQ_GROUP_TYPE nAcqGroupType)
{
	m_pFrameWnd = pFrameWnd;
	
	InitMenu();
	InitToolBar(nAcqGroupType);
	
	CreateMenu();
	CreateToolBar();
	CreateDialogBar();
	
	m_pFrameWnd->RecalcLayout();
	
	return 0;
}

INT CAcqGroupResourceMgr::InitMenu()
{
	INT nIdx = 0;
	
	if(m_nGroupOpenType == ACQ_GROUP_OPEN)
	{
		CreateMenuItem(nIdx, _T("수서구입차수관리(&P)"), _T("ACQ_PURCHASE_GROUP"), _T("수서구입차수관리"));
		CreateMenuItem(nIdx, _T("수서기증접수번호관리(&R)"), _T("ACQ_DONATE_GROUP"), _T("수서기증접수번호관리"));
		if(m_bHaveOtherGroup)
			CreateMenuItem(nIdx, _T("수서그룹관리(&E)"), _T("ACQ_OTHER_GROUP"), _T("수서그룹관리"));
	}
	else if(m_nGroupOpenType == PURCHASE_GROUP_OPEN)
	{
		CreateMenuItem(nIdx, _T("수서구입차수관리(&P)"), _T("ACQ_PURCHASE_GROUP"), _T("수서구입차수관리"));
	}
	else if(m_nGroupOpenType == DONATE_GROUP_OPEN)
	{
		CreateMenuItem(nIdx, _T("수서기증접수번호관리(&R)"), _T("ACQ_DONATE_GROUP"), _T("수서기증접수번호관리"));
	}
	else if(m_nGroupOpenType == ACQOBJ_GROUP_OPEN)
	{
		CreateMenuItem(nIdx, _T("수서대상그룹관리(&O)"), _T("ACQ_OBJ_GROUP"), _T("수서대상그룹관리"));
	}
	
	return 0;
}

INT CAcqGroupResourceMgr::InitToolBar(ACQ_GROUP_TYPE nAcqGroupType)
{
	INT nIdx = 0;
	
	FreeToolBarResourceList();
	
	INT ids;

	if(nAcqGroupType == ACQ_GROUP_INVALID)
		return -1;
	else if(nAcqGroupType == ACQ_OBJ_GROUP)
		ids = InitToolBarOfAcqObjGroup();
	else if(nAcqGroupType == ACQ_PURCHASE_GROUP)
		ids = InitToolBarOfPurchaseGroup();
	else if(nAcqGroupType == ACQ_DONATE_GROUP)
		ids = InitToolBarOfDonateGroup();
	else if(nAcqGroupType == ACQ_OTHER_GROUP)
		ids = InitToolBarOfAcqOtherGroup();
	
	return 0;
}

INT CAcqGroupResourceMgr::InitToolBarOfAcqObjGroup()
{
	INT nIdx = 0;
	
	CreateToolBarItem(nIdx, _T("검색"), _T("검색"), _T("수서대상그룹을 검색합니다."),_T(""),_T("..\\bmp\\003_on.bmp"),_T("..\\bmp\\003_on.bmp"),_T("..\\bmp\\003_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("입력"), _T("입력"), _T("수서대상그룹을 생성합니다."),_T(""),_T("..\\bmp\\116_on.bmp"),_T("..\\bmp\\116_on.bmp"),_T("..\\bmp\\116_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("수정"), _T("수정"), _T("수서대상그룹 정보를 수정합니다."),_T(""),_T("..\\bmp\\089_on.bmp"),_T("..\\bmp\\089_on.bmp"),_T("..\\bmp\\089_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("삭제"), _T("삭제"), _T("수서대상그룹을 삭제합니다."),_T(""),_T("..\\bmp\\081_on.bmp"),_T("..\\bmp\\081_on.bmp"),_T("..\\bmp\\081_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("그룹통합"), _T("그룹통합"), _T("수서대상그룹을 통합합니다."),_T(""),_T("..\\bmp\\163_on.bmp"),_T("..\\bmp\\163_on.bmp"),_T("..\\bmp\\163_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("그룹분리"), _T("그룹분리"), _T("수서대상그룹을 분리합니다."),_T(""),_T("..\\bmp\\076_on.bmp"),_T("..\\bmp\\076_on.bmp"),_T("..\\bmp\\076_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("그룹복사"), _T("그룹복사"), _T("수서대상그룹을 복사합니다."),_T(""),_T("..\\bmp\\042_on.bmp"),_T("..\\bmp\\042_on.bmp"),_T("..\\bmp\\042_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("닫기"), _T("닫기"), _T("그룹관리 창을 닫습니다."),_T(""),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_dimd.bmp"));
	
	return 0;
}

INT CAcqGroupResourceMgr::InitToolBarOfPurchaseGroup()
{
	INT nIdx = 0;
	
	CreateToolBarItem(nIdx, _T("검색"), _T("검색"), _T("수서구입차수를 검색합니다."),_T(""),_T("..\\bmp\\003_on.bmp"),_T("..\\bmp\\003_on.bmp"),_T("..\\bmp\\003_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("입력"), _T("입력"), _T("수서구입차수를 생성합니다."),_T(""),_T("..\\bmp\\116_on.bmp"),_T("..\\bmp\\116_on.bmp"),_T("..\\bmp\\116_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("수정"), _T("수정"), _T("수서구입차수 정보를 수정합니다."),_T(""),_T("..\\bmp\\089_on.bmp"),_T("..\\bmp\\089_on.bmp"),_T("..\\bmp\\089_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("삭제"), _T("삭제"), _T("수서구입차수를 삭제합니다."),_T(""),_T("..\\bmp\\081_on.bmp"),_T("..\\bmp\\081_on.bmp"),_T("..\\bmp\\081_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("차수정보보기"), _T("차수정보보기"), _T("수서구입차수내 자료를 자료상태별로 확인합니다."),_T(""),_T("..\\bmp\\084_on.bmp"),_T("..\\bmp\\084_on.bmp"),_T("..\\bmp\\084_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("차수통합"), _T("차수통합"), _T("수서구입차수를 통합합니다."),_T(""),_T("..\\bmp\\163_on.bmp"),_T("..\\bmp\\163_on.bmp"),_T("..\\bmp\\163_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("차수분리"), _T("차수분리"), _T("수서구입차수를 분리합니다."),_T(""),_T("..\\bmp\\076_on.bmp"),_T("..\\bmp\\076_on.bmp"),_T("..\\bmp\\076_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("차수복사"), _T("차수복사"), _T("수서구입차수를 복사합니다."),_T(""),_T("..\\bmp\\042_on.bmp"),_T("..\\bmp\\042_on.bmp"),_T("..\\bmp\\042_dimd.bmp"));	
	CreateToolBarItem(nIdx, _T("닫기"), _T("닫기"), _T("그룹관리 창을 닫습니다."),_T(""),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_dimd.bmp"));
	
	return 0;
}

INT CAcqGroupResourceMgr::InitToolBarOfDonateGroup()
{
	INT nIdx = 0;
	
	CreateToolBarItem(nIdx, _T("검색"), _T("검색"), _T("수서기증접수번호를 검색합니다."),_T(""),_T("..\\bmp\\003_on.bmp"),_T("..\\bmp\\003_on.bmp"),_T("..\\bmp\\003_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("입력"), _T("입력"), _T("수서기증접수번호를 생성합니다."),_T(""),_T("..\\bmp\\116_on.bmp"),_T("..\\bmp\\116_on.bmp"),_T("..\\bmp\\116_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("수정"), _T("수정"), _T("수서기증접수번호 정보를 수정합니다."),_T(""),_T("..\\bmp\\089_on.bmp"),_T("..\\bmp\\089_on.bmp"),_T("..\\bmp\\089_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("삭제"), _T("삭제"), _T("수서기증접수번호를 삭제합니다."),_T(""),_T("..\\bmp\\081_on.bmp"),_T("..\\bmp\\081_on.bmp"),_T("..\\bmp\\081_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("접수번호정보보기"), _T("접수번호정보보기"), _T("수서기증접수번호내 자료를 자료상태별로 확인합니다."),_T(""),_T("..\\bmp\\084_on.bmp"),_T("..\\bmp\\084_on.bmp"),_T("..\\bmp\\084_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("접수번호통합"), _T("접수번호통합"), _T("수서기증접수번호를 통합합니다."),_T(""),_T("..\\bmp\\163_on.bmp"),_T("..\\bmp\\163_on.bmp"),_T("..\\bmp\\163_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("접수번호분리"), _T("접수번호분리"), _T("수서기증접수번호를 분리합니다."),_T(""),_T("..\\bmp\\076_on.bmp"),_T("..\\bmp\\076_on.bmp"),_T("..\\bmp\\076_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("접수번호복사"), _T("접수번호복사"), _T("수서기증접수번호를 복사합니다."),_T(""),_T("..\\bmp\\042_on.bmp"),_T("..\\bmp\\042_on.bmp"),_T("..\\bmp\\042_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("닫기"), _T("닫기"), _T("그룹관리 창을 닫습니다."),_T(""),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_dimd.bmp"));
	
	return 0;
}

INT CAcqGroupResourceMgr::InitToolBarOfAcqOtherGroup()
{
	INT nIdx = 0;
	
	CreateToolBarItem(nIdx, _T("검색"), _T("검색"), _T("수서그룹번호를 검색합니다."),_T(""),_T("..\\bmp\\003_on.bmp"),_T("..\\bmp\\003_on.bmp"),_T("..\\bmp\\003_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("입력"), _T("입력"), _T("수서그룹번호를 생성합니다."),_T(""),_T("..\\bmp\\116_on.bmp"),_T("..\\bmp\\116_on.bmp"),_T("..\\bmp\\116_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("수정"), _T("수정"), _T("수서그룹번호 정보를 수정합니다."),_T(""),_T("..\\bmp\\089_on.bmp"),_T("..\\bmp\\089_on.bmp"),_T("..\\bmp\\089_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("삭제"), _T("삭제"), _T("수서그룹번호를 삭제합니다."),_T(""),_T("..\\bmp\\081_on.bmp"),_T("..\\bmp\\081_on.bmp"),_T("..\\bmp\\081_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("수서그룹정보보기"), _T("수서그룹정보보기"), _T("수서그룹번호내 자료를 자료상태별로 확인합니다."),_T(""),_T("..\\bmp\\084_on.bmp"),_T("..\\bmp\\084_on.bmp"),_T("..\\bmp\\084_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("수서그룹통합"), _T("수서그룹통합"), _T("수서그룹번호를 통합합니다."),_T(""),_T("..\\bmp\\163_on.bmp"),_T("..\\bmp\\163_on.bmp"),_T("..\\bmp\\163_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("수서그룹분리"), _T("수서그룹분리"), _T("수서그룹번호를 분리합니다."),_T(""),_T("..\\bmp\\076_on.bmp"),_T("..\\bmp\\076_on.bmp"),_T("..\\bmp\\076_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("수서그룹복사"), _T("수서그룹복사"), _T("수서그룹번호를 복사합니다."),_T(""),_T("..\\bmp\\042_on.bmp"),_T("..\\bmp\\042_on.bmp"),_T("..\\bmp\\042_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("닫기"), _T("닫기"), _T("그룹관리 창을 닫습니다."),_T(""),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_on.bmp"),_T("..\\bmp\\347_dimd.bmp"));
	
	return 0;
}

INT CAcqGroupResourceMgr::CreateMenu()
{
	CMenu *pSystemMenu = new CMenu;
	pSystemMenu->CreateMenu();
	
	m_listMenu.AddTail(pSystemMenu);	
	
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
	
	return 0;
}

INT CAcqGroupResourceMgr::CreateMenuItem(INT &nIdx, LPCTSTR lpszGroupAlias, LPCTSTR lpszAlias, LPCTSTR lpszCaption)
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

INT CAcqGroupResourceMgr::CreateToolBar()
{
	CToolBar *pToolBar = new CToolBar;
	m_listToolBar.AddTail(pToolBar);

	if (!pToolBar->CreateEx(m_pFrameWnd, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP |
		CBRS_TOOLTIPS | CBRS_FLYBY))
	{
		delete pToolBar;
		return NULL;
	}

	pToolBar->SetWindowText(_T("수서그룹관리 도구모음"));

	pToolBar->EnableDocking(CBRS_ALIGN_TOP);

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

VOID CAcqGroupResourceMgr::ReplaceBackgroundColor(CBitmap& ioBM)
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
		
	RGBTRIPLE	kBackgroundColor = {192, 192, 192};
	//RGBTRIPLE	kBackgroundColor = {200, 208, 212};
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

INT CAcqGroupResourceMgr::CreateToolBarItem(INT &nIdx, 
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

INT CAcqGroupResourceMgr::CreateDialogBar()
{
	return 0;
}

BOOL CAcqGroupResourceMgr::IsFileExist(LPCTSTR lpszFileName)
{
	CFile file;
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary))
		return FALSE;
	else
		file.Close();
	return TRUE;
}

BOOL CAcqGroupResourceMgr::OnCmdMsg(UINT nID)
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

BOOL CAcqGroupResourceMgr::OnCommand(UINT nID, CString &strCommandAlias)
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

INT CAcqGroupResourceMgr::GetButtonWidth()
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

BOOL CAcqGroupResourceMgr::CheckOtherAcqType()
{
	if(m_pParentESLMgr == NULL) return FALSE;

	CString codes[6];
	CString code, desc;
	INT startrow = 0;
	
	INT codeCnt = m_pParentESLMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("입수구분"));
	for(INT i=0 ; i<codeCnt ; i++)
	{
		startrow = m_pParentESLMgr->m_pInfo->pCodeMgr->GetCode(_T("입수구분"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();

		startrow++;

		if(code == _T("") || code == _T("1") || code == _T("2"))
			continue;
		else
			return TRUE;
	}
	
	return FALSE;
}
