// MarcAdjResourceMgr.cpp: implementation of the CMarcAdjResourceMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MarcAdjResourceMgr.h"
#include "MarcToolBar.h"
// Program Mode - Univ Mode, Original Mode
#include "..\\MarcAdjustApi\\CatProgramMode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarcAdjResourceMgr::CMarcAdjResourceMgr(CESL_Mgr *pParentESLMgr /*= NULL */)
	:m_dlgCode(pParentESLMgr), m_dlgVolume(pParentESLMgr)
{
	m_pFrameWnd = NULL;
	m_pParentESLMgr = pParentESLMgr;

	m_nResourceID = 32001;

	m_strBitmapNormalPath	= _T("..\\bmp\\normal.bmp");
	m_strBitmapHotPath		= _T("..\\bmp\\hot.bmp");
	m_strBitmapDisabledPath	= _T("..\\bmp\\disabled.bmp");
}

CMarcAdjResourceMgr::~CMarcAdjResourceMgr()
{
	FreeResourceList();
}


//////////////////////////////////////////////////////////////////////
// CMarcAdjResourceMgr Operations

VOID CMarcAdjResourceMgr::FreeResourceList()
{
	EFS_BEGIN

	VOID *pClass = NULL;
	FreePtrList(&m_listMenu, static_cast<CMenu*>(pClass));

	FreePtrList(&m_listToolBar, static_cast<CToolBar*>(pClass));

	FreePtrList(&m_listMenuItem, static_cast<CMenuItem*>(pClass));

	FreePtrList(&m_listToolBarItem, static_cast<CToolBarItem*>(pClass));

	FreePtrList(&m_listImageList, static_cast<CImageList*>(pClass));

	FreePtrList(&m_listToolTip, static_cast<CToolTipCtrl*>(pClass));

	EFS_END
}

template <class T> VOID CMarcAdjResourceMgr::FreePtrList(CPtrList *pPtrList, T *pClass)
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

INT CMarcAdjResourceMgr::CreateResource(CFrameWnd *pFrameWnd)
{
	EFS_BEGIN

	m_pFrameWnd = pFrameWnd;

	InitMenu();
	InitToolBar();

	CreateMenu();
	CreateToolBar();
	CreateDialogBar();

	m_pFrameWnd->RecalcLayout();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjResourceMgr::InitMenu()
{
	EFS_BEGIN

	INT nIdx = 0;

	CreateMenuItem(nIdx, _T("보기(&V)"), _T("VIEW_IDD_CODE"), _T("코드바 보기"));
	CreateMenuItem(nIdx, _T("보기(&V)"), _T("VIEW_IDD_VOLINFO"), _T("권별정보 보기"));
	CreateMenuItem(nIdx, _T("보기(&V)"), _T("VIEW_TOOLBAR"), _T("마크정리도구모음 보기"));	

	CreateMenuItem(nIdx, _T("지원(&S)"), _T("SUPPORT_별치기호"), _T("별치기호 조회"));
	CreateMenuItem(nIdx, _T("지원(&S)"), _T("SUPPORT_수입순번호"), _T("수입순번호 관리"));
	CreateMenuItem(nIdx, _T("지원(&S)"), _T("SUPPORT_저자기호"), _T("저자기호 조회"));
	CreateMenuItem(nIdx, _T("지원(&S)"), _T("SUPPORT_역할어"), _T("역할어 관리"));
	CreateMenuItem(nIdx, _T("지원(&S)"), _T("SUPPORT_분류기호"), _T("분류기호 참조"));

	CreateMenuItem(nIdx, _T("반입(&I)"), _T("국가자료공동목록반입"), _T("국가자료공동목록반입"));
	CreateMenuItem(nIdx, _T("반입(&I)"), _T("MARC반입"), _T("MARC반입"));

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjResourceMgr::CreateMenuItem(INT &nIdx, LPCTSTR lpszGroupAlias, LPCTSTR lpszAlias, LPCTSTR lpszCaption)
{
	EFS_BEGIN

	CMenuItem *pMenuItem = new CMenuItem;
	m_listMenuItem.AddTail(pMenuItem);

	m_mapMenuGroup.SetAt(lpszGroupAlias, 1);

	pMenuItem->m_nID = ++m_nResourceID;
	pMenuItem->m_nIndex = nIdx++;
	pMenuItem->m_strGroupAlias = lpszGroupAlias;
	pMenuItem->m_strAlias = lpszAlias;
	pMenuItem->m_strCaption = lpszCaption;
	
	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjResourceMgr::InitToolBar()
{
	EFS_BEGIN

	INT nIdx = 0;

	CreateToolBarItem(nIdx, _T("권별정보"), _T("권별정보"), _T("권별정보를 보여줍니다."), _T(""), 
					 _T("..\\bmp\\334_on.bmp"), _T("..\\bmp\\334_on.bmp"), _T("..\\bmp\\334_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("부록정보"), _T("부록정보"), _T("부록정보를 보여줍니다."), _T(""), 
					 _T("..\\bmp\\273_on.bmp"), _T("..\\bmp\\273_on.bmp"), _T("..\\bmp\\273_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("청구기호생성"), _T("청구기호생성"), _T("청구기호를 생성합니다.."), _T(""),
					 _T("..\\bmp\\335_on.bmp"), _T("..\\bmp\\335_on.bmp"), _T("..\\bmp\\335_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("청구기호조사"), _T("청구기호조사"), _T("청구기호를 조사합니다.."), _T(""),
					 _T("..\\bmp\\336_on.bmp"), _T("..\\bmp\\336_on.bmp"), _T("..\\bmp\\336_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("고정길이(006)"), _T("고정길이"), _T("고정길이(006)을 편집합니다.."), _T(""),
					 _T("..\\bmp\\3371_on.bmp"), _T("..\\bmp\\3371_on.bmp"), _T("..\\bmp\\3371_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("고정길이(008)"), _T("고정길이"), _T("리더,고정길이(007, 008)을 편집합니다.."), _T(""),
					 _T("..\\bmp\\337_on.bmp"), _T("..\\bmp\\337_on.bmp"), _T("..\\bmp\\337_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("분류지원시스템"), _T("분류지원"), _T("분류지원 시스템.."), _T(""),
					 _T("..\\bmp\\338_on.bmp"), _T("..\\bmp\\338_on.bmp"), _T("..\\bmp\\338_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("SUPPORT_분류기호"), _T("분류기호참조"), _T("분류기호참조"), _T(""),
					 _T("..\\bmp\\338_on.bmp"), _T("..\\bmp\\338_on.bmp"), _T("..\\bmp\\338_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("부출지원"), _T("부출지원"), _T("부출지원.."), _T(""),
					 _T("..\\bmp\\339_on.bmp"), _T("..\\bmp\\339_on.bmp"), _T("..\\bmp\\339_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("기본값"), _T("기본값"), _T("기본값 설정.."), _T(""),
					 _T("..\\bmp\\340_on.bmp"), _T("..\\bmp\\340_on.bmp"), _T("..\\bmp\\340_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("키워드"), _T("키워드"), _T("키워드 생성.."), _T(""),
				     _T("..\\bmp\\341_on.bmp"), _T("..\\bmp\\341_on.bmp"), _T("..\\bmp\\341_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("복본조사"), _T("복본조사"), _T("복본조사.."), _T(""),
					 _T("..\\bmp\\342_on.bmp"), _T("..\\bmp\\342_on.bmp"), _T("..\\bmp\\342_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("자동부여"), _T("자동부여"), _T("자동 등록번호 부여.."), _T(""),
					 _T("..\\bmp\\063_on.bmp"), _T("..\\bmp\\063_on.bmp"), _T("..\\bmp\\063_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("누락부여"), _T("누락부여"), _T("누락 등록번호 부여.."), _T(""),
					 _T("..\\bmp\\204_on.bmp"), _T("..\\bmp\\204_on.bmp"), _T("..\\bmp\\204_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("목차초록관리"), _T("목차편집"), _T("목차초록관리.."), _T(""),
					 _T("..\\bmp\\360_on.bmp"), _T("..\\bmp\\360_on.bmp"), _T("..\\bmp\\360_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("소장정보"), _T("소장정보"), _T("소장정보의 049TAG을 보여줍니다.."), _T(""),
					 _T("..\\bmp\\342_on.bmp"), _T("..\\bmp\\342_on.bmp"), _T("..\\bmp\\342_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("메모장"), _T("메모장"), _T("메모장.."), _T(""),
					 _T("..\\bmp\\363_on.bmp"), _T("..\\bmp\\363_on.bmp"), _T("..\\bmp\\363_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("ebook보기"), _T("ebook보기"), _T("ebook보기입니다.."), _T(""),
					 _T("..\\bmp\\ebook_on.bmp"), _T("..\\bmp\\ebook_on.bmp"), _T("..\\bmp\\ebook_off.bmp"));
	CreateToolBarItem(nIdx, _T("표지정보"), _T("표지정보"), _T("표지정보입니다..."), _T(""),
					 _T("..\\bmp\\ebook_on.bmp"), _T("..\\bmp\\ebook_on.bmp"), _T("..\\bmp\\ebook_off.bmp"));
	CreateToolBarItem(nIdx, _T("저장"), _T("저장"), _T("저장합니다..."), _T(""),
					 _T("..\\bmp\\343_on.bmp"), _T("..\\bmp\\343_on.bmp"), _T("..\\bmp\\343_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("이전내용"), _T("이전내용"), _T("이전내용을 복사합니다.."), _T(""),
					 _T("..\\bmp\\345_on.bmp"), _T("..\\bmp\\345_on.bmp"), _T("..\\bmp\\345_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("이전"), _T("이전"), _T("이전 자료를 봅니다.."), _T(""),
					 _T("..\\bmp\\345_on.bmp"), _T("..\\bmp\\345_on.bmp"), _T("..\\bmp\\345_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("다음"), _T("다음"), _T("다음 자료를 봅니다.."), _T(""),
					 _T("..\\bmp\\346_on.bmp"), _T("..\\bmp\\346_on.bmp"), _T("..\\bmp\\346_dimd.bmp"));
	CreateToolBarItem(nIdx, _T("SYS_CLOSE"), _T("닫기"), _T("닫습니다..."), _T(""),
					 _T("..\\bmp\\347_on.bmp"), _T("..\\bmp\\347_on.bmp"), _T("..\\bmp\\347_dimd.bmp"));

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjResourceMgr::CreateToolBarItem(INT &nIdx, LPCTSTR lpszAlias, LPCTSTR lpszCaption, LPCTSTR lpszToolTip, LPCTSTR lpszStyle, LPCTSTR lpszBitmapNormal, LPCTSTR lpszBitmapHot, LPCTSTR lpszBitmapDisabled)
{
	EFS_BEGIN

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

	EFS_END
	return -1;
}

INT CMarcAdjResourceMgr::CreateMenu()
{
	EFS_BEGIN

	CMenu *pSystemMenu = new CMenu;
	pSystemMenu->CreateMenu();

	m_listMenu.AddTail(pSystemMenu);

	CString strKey;
	INT nValue;
	
	CMenuItem *pMenuItem = NULL;
	POSITION posMenuItem = NULL;
	POSITION posMenuGroup = m_mapMenuGroup.GetStartPosition();
	while (posMenuGroup)
	{
		m_mapMenuGroup.GetNextAssoc(posMenuGroup, strKey, nValue);
		if (strKey.IsEmpty()) continue;

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

		pSystemMenu->AppendMenu(MF_POPUP, (UINT)pMenu->m_hMenu, strKey);
		pMenu->Detach();
	}

	m_pFrameWnd->SetMenu(pSystemMenu);

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjResourceMgr::CreateToolBar()
{
	EFS_BEGIN

	CMarcToolBar *pToolBar = new CMarcToolBar;
	m_listToolBar.AddTail(pToolBar);
	pToolBar->m_listToolBarItem = &m_listToolBarItem;
	pToolBar->m_pToolBar = &pToolBar->GetToolBarCtrl();
	pToolBar->m_pParent = this;

	if (!pToolBar->CreateEx(m_pFrameWnd, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP |
		CBRS_TOOLTIPS | CBRS_FLYBY))
	{
		delete pToolBar;
		return NULL;
	}

	pToolBar->SetWindowText(_T("MARC정리 도구모음"));

	pToolBar->EnableDocking(CBRS_ALIGN_TOP);

	CBitmap bmpNormal;
	CBitmap bmpHot;
	CBitmap bmpDisabled;

	CToolTipCtrl *pToolTipCtrl = new CToolTipCtrl;
	pToolTipCtrl->Create(m_pFrameWnd, TTS_ALWAYSTIP);
	m_listToolTip.AddTail((CObject*)pToolTipCtrl);

	CImageList * pImgListNormal = new CImageList;
	pImgListNormal->Create(BMP_SIZE, BMP_SIZE, ILC_COLOR24, 0, 4);
	
	CImageList * pImgListHot = new CImageList;
	pImgListHot->Create(BMP_SIZE, BMP_SIZE, ILC_COLOR24, 0, 4);
		
	CImageList * pImgListDisabled = new CImageList;
	pImgListDisabled->Create(BMP_SIZE, BMP_SIZE, ILC_COLOR24, 0, 4);

	m_listImageList.AddTail(pImgListNormal);
	m_listImageList.AddTail(pImgListHot);
	m_listImageList.AddTail(pImgListDisabled);

	CToolBarCtrl &rpToolBarCtrl = pToolBar->GetToolBarCtrl();
	if (!::IsWindow(rpToolBarCtrl.GetSafeHwnd())) return -1;
	while (rpToolBarCtrl.GetButtonCount() > 0)
		rpToolBarCtrl.DeleteButton(0);

	INT nButtonWidth = GetButtonWidth();	
	nButtonWidth = nButtonWidth + USER_BSIZE_WIDTH;
	rpToolBarCtrl.SetButtonSize(CSize(nButtonWidth, USER_BSIZE_HIGHT));
		
	rpToolBarCtrl.SetMaxTextRows(2);
	rpToolBarCtrl.SetDrawTextFlags(DT_RIGHT, DT_WORDBREAK | DT_EDITCONTROL | DT_RIGHT);
	
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
		
		if (!IsFileExist(pToolBarItem->m_strBitmapNormal))
		{
			if (!IsFileExist(m_strBitmapNormalPath))
			{
				AfxMessageBox(_T("Default Bitmap이 없습니다."));
				return -1;
			}
			else
				pToolBarItem->m_strBitmapNormal = m_strBitmapNormalPath;
		}
		
		if (!IsFileExist(pToolBarItem->m_strBitmapHot))
		{
			if (!IsFileExist(m_strBitmapHotPath))
			{
				AfxMessageBox(_T("Default Bitmap이 없습니다."));
				return -1;
			}
			else
				pToolBarItem->m_strBitmapHot = m_strBitmapHotPath;
		}
		
		if (!IsFileExist(pToolBarItem->m_strBitmapDisabled))
		{
			if (!IsFileExist(m_strBitmapDisabledPath))
			{
				AfxMessageBox(_T("Default Bitmap이 없습니다."));
				return -1;
			}
			else
				pToolBarItem->m_strBitmapDisabled = m_strBitmapDisabledPath;
		}
			
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

	EFS_END
	return -1;
}

INT CMarcAdjResourceMgr::CreateDialogBar()
{
	EFS_BEGIN

	m_dlgCode.pMain = NULL;
	if (!m_wndCodeBar.Create(m_pFrameWnd, &m_dlgCode, CString(_T("코드")), 
#ifdef _CAT_PROGRAM_MODE_UNIV_
		IDD_CODE_UNIV, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY))
#else
		IDD_CODE, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY))
#endif
	{
		TRACE0("Failed to create m_wndCodeBar\n");
        return -1;      // fail to create
	}
	
	m_dlgVolume.pMain = NULL;
	if (!m_wndVolumeBar.Create(m_pFrameWnd, &m_dlgVolume, CString(_T("권별정보")), 
		IDD_VOLINFO, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	{
		TRACE0("Failed to create m_wndVolumeBar\n");
        return -1;      // fail to create
	}

	m_wndCodeBar.EnableDocking(CBRS_ALIGN_TOP);
	m_pFrameWnd->DockControlBar(&m_wndCodeBar);

	m_wndVolumeBar.EnableDocking(CBRS_ALIGN_BOTTOM);
	m_pFrameWnd->DockControlBar(&m_wndVolumeBar);

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjResourceMgr::HideToolBarButton(CString strToolBarButtonAlias, BOOL bHide /*= TRUE*/)
{
	EFS_BEGIN

	INT nID = -1;

	CStringArray StringArray;

	CToolBarItem *pToolBarItem = NULL;
	POSITION pos = m_listToolBarItem.GetHeadPosition();
	while (pos)
	{
		pToolBarItem = static_cast<CToolBarItem*>(m_listToolBarItem.GetNext(pos));
		if (pToolBarItem == NULL) continue;
		if (pToolBarItem->m_strAlias == strToolBarButtonAlias)
		{
			nID = pToolBarItem->m_nID;
			break;
		}
	}
	
	if (nID == -1) return -1;

	UINT nState = 0;

	if (bHide)
		nState |= TBSTATE_HIDDEN ;
	else
		nState |= TBSTATE_ENABLED;

	CToolBar *pToolBar = NULL;
	pos = m_listToolBar.GetHeadPosition();
	while (pos)
	{
		pToolBar = static_cast<CToolBar*>(m_listToolBar.GetNext(pos));
		if (pToolBar == NULL) continue;

		CToolBarCtrl &pToolBarCtrl = pToolBar->GetToolBarCtrl();

		pToolBarCtrl.SetState(nID, TBSTATE_WRAP);
		pToolBarCtrl.SetState(nID, nState);

		m_pFrameWnd->RecalcLayout();
	}

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjResourceMgr::SetToolBarButtonCatpion(CString strToolBarButtonAlias, CString strCaption)
{
	EFS_BEGIN

	INT nID = -1;

	CStringArray StringArray;

	CToolBarItem *pToolBarItem = NULL;
	POSITION pos = m_listToolBarItem.GetHeadPosition();
	while (pos)
	{
		pToolBarItem = static_cast<CToolBarItem*>(m_listToolBarItem.GetNext(pos));
		if (pToolBarItem == NULL) continue;
		if (pToolBarItem->m_strAlias == strToolBarButtonAlias)
		{
			nID = pToolBarItem->m_nID;
			break;
		}
	}
	
	if (nID == -1) return -1;

	CToolBar *pToolBar = NULL;
	pos = m_listToolBar.GetHeadPosition();
	while (pos)
	{
		pToolBar = static_cast<CToolBar*>(m_listToolBar.GetNext(pos));
		if (pToolBar == NULL) continue;

		pToolBar->SetButtonText(pToolBarItem->m_nIndex, strCaption);
		
		m_pFrameWnd->RecalcLayout();
	}
	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjResourceMgr::DeleteMenuItem(CString strMenuAlias)
{
	EFS_BEGIN

	INT nID = -1;
	CMenuItem *pMenuItem = NULL;
	POSITION pos = m_listMenuItem.GetHeadPosition();
	while (pos)
	{
		pMenuItem = static_cast<CMenuItem*>(m_listMenuItem.GetNext(pos));
		if (pMenuItem == NULL) continue;
		if (pMenuItem->m_strAlias == strMenuAlias)
		{
			nID = pMenuItem->m_nID;
			break;
		}
	}

	if (nID == -1) return -1;

	CMenu *pMenu = m_pFrameWnd->GetMenu();
	pMenu->DeleteMenu(nID, MF_BYCOMMAND);

	return 0;

	EFS_END
	return -1;
}


BOOL CMarcAdjResourceMgr::IsFileExist(LPCTSTR lpszFileName)
{
	EFS_BEGIN

	CFile file;
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary))
		return FALSE;
	else
		file.Close();
	return TRUE;

	EFS_END
	return FALSE;
}

VOID CMarcAdjResourceMgr::ReplaceBackgroundColor (CBitmap& ioBM)
{
	EFS_BEGIN

	// figure out how many pixels there are in the bitmap
	BITMAP		bmInfo;

	//if ( ioBM.GetSafeHandle() == NULL ) return;

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
	const RGBTRIPLE		userBackgroundColor = {
		GetBValue (buttonColor), GetGValue (buttonColor), GetRValue (buttonColor)};

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

	EFS_END
}

INT CMarcAdjResourceMgr::GetButtonWidth()
{
	EFS_BEGIN

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

	EFS_END
	return -1;
}

BOOL CMarcAdjResourceMgr::OnCmdMsg(UINT nID)
{
	EFS_BEGIN

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

	EFS_END
	return FALSE;
}

BOOL CMarcAdjResourceMgr::OnCommand(UINT nID, CString &strCommandAlias)
{
	EFS_BEGIN

	POSITION pos = NULL;
	// Menu
	CMenuItem *pMenuItem = NULL;
	pos = m_listMenuItem.GetHeadPosition();
	while (pos)
	{
		pMenuItem = static_cast<CMenuItem*>(m_listMenuItem.GetNext(pos));
		if (pMenuItem && pMenuItem->m_nID == nID)
		{
			if (!OnCommand(pMenuItem->m_strAlias))
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
			if (!OnCommand(pToolBarItem->m_strAlias))
				strCommandAlias = pToolBarItem->m_strAlias;

			return TRUE;
		}
	}

	return FALSE;

	EFS_END
	return FALSE;
}

BOOL CMarcAdjResourceMgr::OnCommand(CString strCommandAlias)
{
	EFS_BEGIN

	if (strCommandAlias == _T("VIEW_IDD_CODE"))
	{
		if (m_wndCodeBar.IsWindowVisible())
			m_wndCodeBar.ShowWindow(SW_HIDE);
		else
			m_wndCodeBar.ShowWindow(SW_SHOW);
		
		m_pFrameWnd->DockControlBar(&m_wndCodeBar, AFX_IDW_DOCKBAR_TOP);
	}
	else if (strCommandAlias == _T("VIEW_IDD_VOLINFO"))
	{
		if (m_wndVolumeBar.IsWindowVisible())
			m_wndVolumeBar.ShowWindow(SW_HIDE);
		else
			m_wndVolumeBar.ShowWindow(SW_SHOW);

		m_pFrameWnd->DockControlBar(&m_wndVolumeBar, AFX_IDW_DOCKBAR_BOTTOM);
	}
	else if (strCommandAlias == _T("VIEW_TOOLBAR"))
	{
		CMarcToolBar *pToolBar = NULL;
		POSITION pos = m_listToolBar.GetHeadPosition();
		while (pos)
		{
			pToolBar = static_cast<CMarcToolBar*>(m_listToolBar.GetNext(pos));
			if (pToolBar == NULL) continue;
			if (pToolBar->IsWindowVisible())
				pToolBar->ShowWindow(SW_HIDE);
			else
				pToolBar->ShowWindow(SW_SHOW);
		}
	}
	else return FALSE;
	
	m_pFrameWnd->RecalcLayout();

	return TRUE;

	EFS_END
	return FALSE;
}