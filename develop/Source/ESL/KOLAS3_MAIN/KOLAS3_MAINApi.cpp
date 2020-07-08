// KOLAS3_MAINApi.cpp: implementation of the CKOLAS3_MAINApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "KOLAS3_MAINApi.h"

#include <afxpriv.h>

#include "MToolBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "SetAcceleratorDlg.h"

#define BMP_SIZE 20
#define BMP_COUNT 32
#define USER_BSIZE_WIDE 55
#define USER_BSIZE_HIGHT 39

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKOLAS3_MAINApi::CKOLAS3_MAINApi(CWnd *pParentWnd /*=NULL*/)
{
	m_pParentWnd = pParentWnd;

	// default bitmap
	m_strBitmapNormalPath	= _T("..\\bmp\\normal.bmp");
	m_strBitmapHotPath		= _T("..\\bmp\\hot.bmp");
	m_strBitmapDisabledPath	= _T("..\\bmp\\disabled.bmp");	
}

CKOLAS3_MAINApi::~CKOLAS3_MAINApi()
{

}

//////////////////////////////////////////////////////////////////////
// CKOLAS3_MAINApi Operations
CToolBar *CKOLAS3_MAINApi::CreateToolBar(CFrameWnd *pFrame, UINT nDockBarID /*=0*/)
{
	if (pFrame == NULL) return NULL;
	CMToolBar * pToolBar = NULL;
	pToolBar = new CMToolBar();
	
	//if (!pToolBar->CreateEx(pFrame, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP |
	//	CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	if (!pToolBar->CreateEx(pFrame, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_LEFT |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))		
	{
		delete pToolBar;
		return NULL;
	}	
	
	pToolBar->EnableDocking(CBRS_ALIGN_ANY);
	//pFrame->DockControlBar(pToolBar, nDockBarID);

	return pToolBar;
}

VOID CKOLAS3_MAINApi::ReplaceBackgroundColor (CBitmap& ioBM, COLORREF rgbBack)
{
	if(!ioBM.GetSafeHandle() ||NULL == ioBM.m_hObject)
	{
		return;
	}
	// figure out how many pixels there are in the bitmap
	BITMAP		bmInfo;
	INT nIds = ioBM.GetBitmap(&bmInfo);
	if(0 == nIds)
	{
		return;
	}
	
	 // add support for additional bit depths here if you choose
	if(bmInfo.bmBitsPixel != 24)
	{
		return;
	}
	if(bmInfo.bmWidthBytes != (bmInfo.bmWidth * 3))
	{
		return;
	}
// 	VERIFY (bmInfo.bmBitsPixel == 24);
// 	VERIFY (bmInfo.bmWidthBytes == (bmInfo.bmWidth * 3));	

	const UINT		numPixels (bmInfo.bmHeight * bmInfo.bmWidth);	

	 // get a pointer to the pixels
    DIBSECTION  ds;
	VERIFY (ioBM.GetObject (sizeof (DIBSECTION), &ds) == sizeof (DIBSECTION));
	
	RGBTRIPLE*		pixels = reinterpret_cast<RGBTRIPLE*>(ds.dsBm.bmBits);
	if(NULL == pixels)
	{
		return;
	}
// 	VERIFY (pixels != NULL);	

	 // get the user's preferred button color from the _tsystem
	//const COLORREF		buttonColor (::GetSysColor (COLOR_BTNFACE));
	const COLORREF		buttonColor (rgbBack);
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
}

VOID CKOLAS3_MAINApi::ReplaceBackgroundColor2 (CBitmap& ioBM)
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
	const COLORREF		buttonColor  RGB(0,0,0);
	const COLORREF		buttonColor2  RGB(255,255,255);
	const RGBTRIPLE		userBackgroundColor = {
		GetBValue (buttonColor), GetGValue (buttonColor), GetRValue (buttonColor)};
	const RGBTRIPLE		userBackgroundColor2 = {
		GetBValue (buttonColor2), GetGValue (buttonColor2), GetRValue (buttonColor2)};

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
		else
		{
			pixels [i] = userBackgroundColor2;
		}
	}
}

BOOL CKOLAS3_MAINApi::CheckFileExists(CString strFilePath)
{
	CFile file;
	if (!file.Open(strFilePath, CFile::modeRead | CFile::typeBinary))
		return FALSE;
	else
		file.Close();
	return TRUE;
}

VOID CKOLAS3_MAINApi::SetToolBar(CFrameWnd *pFrame, CString strMenuAlias, CString strToolGroupAlias, CToolBar *pToolBar, CObList *pListToolBarItem, CObList *pListToolTip, CObList *pListImage)
{
	if (pFrame == NULL || strMenuAlias.IsEmpty() || pToolBar == NULL ||	pListToolBarItem == NULL || pListToolTip == NULL ||	pListImage == NULL)
		return;

	// Bitmap
	CBitmap bmpNormal;
	CBitmap bmpHot;
	CBitmap bmpDisabled;

	// Create ToolTip
	CToolTipCtrl *pToolTipCtrl = new CToolTipCtrl;
	pToolTipCtrl->Create(m_pParentWnd, TTS_ALWAYSTIP);
	pListToolTip->AddTail((CObject*)pToolTipCtrl);

	// Create ImageList
	CImageList * pImgListNormal = new CImageList;
	pImgListNormal->Create(BMP_SIZE, BMP_SIZE, ILC_COLOR24, 0, 4);
	
	CImageList * pImgListHot = new CImageList;
	pImgListHot->Create(BMP_SIZE, BMP_SIZE, ILC_COLOR24, 0, 4);
		
	CImageList * pImgListDisabled = new CImageList;
	pImgListDisabled->Create(BMP_SIZE, BMP_SIZE, ILC_COLOR24, 0, 4);
	
	pListImage->AddTail((CObject *)pImgListNormal);
	pListImage->AddTail((CObject *)pImgListHot);
	pListImage->AddTail((CObject *)pImgListDisabled);
	
	// Init ToolBar
	CToolBarCtrl &rpToolBarCtrl = pToolBar->GetToolBarCtrl();
	if (!::IsWindow(rpToolBarCtrl.GetSafeHwnd())) return;
	while (rpToolBarCtrl.GetButtonCount() > 0)
		rpToolBarCtrl.DeleteButton(0);

	// Button Witdh
	INT nButtonWidth = GetButtonWidth(pListToolBarItem, strMenuAlias);	
	nButtonWidth = nButtonWidth + USER_BSIZE_WIDE ;
	rpToolBarCtrl.SetButtonSize(CSize(nButtonWidth, USER_BSIZE_HIGHT));

	// set toolbar
	INT nIndex = 0;
	TCHAR szBuf[128];
	POSITION pos = pListToolBarItem->GetHeadPosition();
	while (pos)
	{
		CToolBarItem *pToolBarItem = (CToolBarItem*)pListToolBarItem->GetNext(pos);
		//if (pToolBarItem && pToolBarItem->m_strToolGroupAlias == strToolGroupAlias && pToolBarItem->m_strMenuAlias == strMenuAlias)
		if (pToolBarItem && pToolBarItem->m_strMenuID == strMenuAlias)
		{
			// 2008.03.14 ADD BY PDJ
			// 구분선일 경우 아이템이 추가되지 않는문제 생김
			if ( pToolBarItem->m_strCaption == _T("") ) continue;
			TBBUTTON TBButton;

			TBButton.iBitmap	= nIndex;
			TBButton.idCommand	= pToolBarItem->m_nID;
			TBButton.fsState	= TBSTATE_ENABLED;
			TBButton.fsStyle	= TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE ;			
			_stprintf(szBuf, _T("%s\0"), pToolBarItem->m_strCaption);
			TBButton.iString	= rpToolBarCtrl.AddStrings(szBuf);
			TBButton.dwData		= (DWORD)pToolBarItem;

			if (pToolBarItem->m_strStyle == _T("S"))
				TBButton.fsStyle = TBSTYLE_SEP;
			else if (pToolBarItem->m_strType == _T("R"))
				TBButton.fsState = TBSTATE_ENABLED;
			else if (pToolBarItem->m_strType == _T("I"))
				TBButton.fsState = TBSTATE_HIDDEN;
			else if (pToolBarItem->m_strType == _T("D"))
				TBButton.fsState = TBSTATE_INDETERMINATE;
			
			// Normal bitmap file check
			if (!CheckFileExists(pToolBarItem->m_strBitmapNormal))
			{
				// Default Check
				if (!CheckFileExists(m_strBitmapNormalPath))
				{
					AfxMessageBox(_T("Default Bitmap이 없습니다."));
					return;
				}
				else
					pToolBarItem->m_strBitmapNormal = m_strBitmapNormalPath;
			}

			// Hot bitmap file check
			if (!CheckFileExists(pToolBarItem->m_strBitmapHot))
			{
				// Default Check
				if (!CheckFileExists(m_strBitmapHotPath))
				{
					AfxMessageBox(_T("Default Bitmap이 없습니다."));
					return;
				}
				else
					pToolBarItem->m_strBitmapHot = m_strBitmapHotPath;
			}

			// Disabled bitmap file check
			if (!CheckFileExists(pToolBarItem->m_strBitmapDisabled))
			{
				// Default Check
				if (!CheckFileExists(m_strBitmapDisabledPath))
				{
					AfxMessageBox(_T("Default Bitmap이 없습니다."));
					return;
				}
				else
					pToolBarItem->m_strBitmapDisabled = m_strBitmapDisabledPath;
			}
			
			// attach bitmap
			CBitmap bmpNormalMask;
			bmpNormal.Attach(::LoadImage(NULL, pToolBarItem->m_strBitmapNormal, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
			bmpNormalMask.Attach(::LoadImage(NULL, pToolBarItem->m_strBitmapNormal, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
			bmpHot.Attach(::LoadImage(NULL, pToolBarItem->m_strBitmapHot, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
			bmpDisabled.Attach(::LoadImage(NULL, pToolBarItem->m_strBitmapDisabled, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED));
			
			ReplaceBackgroundColor(bmpNormal,  RGB(229, 234, 245));
			ReplaceBackgroundColor(bmpHot, RGB(245, 245, 240));
			ReplaceBackgroundColor(bmpDisabled,  RGB(227, 227, 220));
			
			/*
			ReplaceBackgroundColor2(bmpNormalMask);
			CDC *pDC = m_pParentWnd->GetDC();
			CDC maskDC;
			CDC SrcDC;

			maskDC.CreateCompatibleDC(pDC);
			SrcDC.CreateCompatibleDC(pDC);
			
			SrcDC.SelectObject(&bmpNormal);
			maskDC.SelectObject(&bmpNormalMask);
			SrcDC.SetBkColor( RGB(192, 192, 192) );
			SrcDC.BitBlt(0,0,20,20,&maskDC, 0,0,SRCCOPY );			

			
			*/

			pImgListNormal->Add(&bmpNormal, RGB(0, 0, 0));
			pImgListHot->Add(&bmpHot, RGB(0, 0, 0));
			pImgListDisabled->Add(&bmpDisabled, RGB(0, 0, 0));

			bmpNormal.DeleteObject();
			bmpNormal.Detach();
			bmpHot.DeleteObject();
			bmpHot.Detach();
			bmpDisabled.DeleteObject();
			bmpDisabled.Detach();
			
			rpToolBarCtrl.InsertButton(nIndex, &TBButton);
// 			((CMToolBar*)pToolBar)->m_listToolBarItem.AddTail(pToolBarItem);

			INT nBtnCnt = rpToolBarCtrl.GetButtonCount();

			if (pToolBarItem->m_strCaption != _T("도구모음"))
				pToolBar->SetButtonText(nIndex, pToolBarItem->m_strCaption);
			
			// tooltip..
			CRect rect;
			rpToolBarCtrl.GetItemRect(nIndex++, rect);
			pToolTipCtrl->AddTool(pToolBar, pToolBarItem->m_ToolTip, rect, pToolBarItem->m_nID);
		}
	}

	rpToolBarCtrl.SetImageList(pImgListNormal);
	rpToolBarCtrl.SetHotImageList(pImgListHot);
	rpToolBarCtrl.SetDisabledImageList(pImgListDisabled);

	rpToolBarCtrl.SetToolTips(pToolTipCtrl);

	// display
	if (rpToolBarCtrl.GetButtonCount() > 0)
	{
		pFrame->ShowControlBar(pToolBar, TRUE, NULL);
	}
	else
	{
		pFrame->ShowControlBar(pToolBar, FALSE, NULL);
	}

 	((CMToolBar*)pToolBar)->InitToolbar();

	rpToolBarCtrl.AutoSize();
// 	((CMToolBar*)pToolBar)->ToolBarSizing();

	pFrame->RecalcLayout();
}

INT CKOLAS3_MAINApi::GetButtonWidth(CObList *pListToolBarItem, CString strMenuAlias)
{
	CString TmpStr;
	INT Count=0,CountSize=0,TmpSize,ButtonSize=0;
	POSITION posButten = pListToolBarItem->GetHeadPosition();
	while (posButten)
	{
		CToolBarItem *pToolBarItem = (CToolBarItem*)pListToolBarItem->GetNext(posButten);
		if (pToolBarItem && pToolBarItem->m_strToolGroupAlias == strMenuAlias)
		{
			TmpStr = pToolBarItem->m_strCaption;
			
			TmpStr.TrimLeft();
			TmpStr.TrimRight();
			if(TmpStr.GetLength() > 10)
			{
				TmpSize = TmpStr.GetLength() - 10;
				CountSize = (TmpSize*5);
				if (CountSize >ButtonSize)
				{
					ButtonSize = (TmpSize*5) +2;
				}
			}
		}
	}

	return ButtonSize;
}

INT CKOLAS3_MAINApi::HideShowToolBarButton(CString strToolBarButtonAlias, CString strMenuAlias, CObList *pListToolBarGroup, CObList *pListToolBarItem, CObList *pListToolBar, BOOL bHide /*= TRUE*/)
{
	if (strToolBarButtonAlias.IsEmpty() || pListToolBarGroup == NULL || pListToolBarItem == NULL || pListToolBar == NULL) return -1;

	INT nID = -1;

	CToolBarItem *pToolBarItem = NULL;
	CToolBarGroup *pToolBarGroup = NULL;

	POSITION pos = NULL;
	
	CStringArray StringArray;

	pos = pListToolBarGroup->GetHeadPosition();
	while (pos)
	{
		pToolBarGroup = (CToolBarGroup*)pListToolBarGroup->GetNext(pos);
		if (pToolBarGroup == NULL) continue;

		if (pToolBarGroup->m_strMenuAlias == strMenuAlias)
		{
			StringArray.Add(pToolBarGroup->m_strAlias);
		}
	}

	if (StringArray.GetSize() < 1) return -1;

	pos = pListToolBarItem->GetHeadPosition();
	while (pos)
	{
		pToolBarItem = (CToolBarItem*)pListToolBarItem->GetNext(pos);
		if (pToolBarItem == NULL) continue;
		
		if (!IsInStrArr(&StringArray, pToolBarItem->m_strToolGroupAlias)) continue;

		if (pToolBarItem->m_strAlias == strToolBarButtonAlias)
		{
			nID = pToolBarItem->m_nID;
			break;
		}
	}
	
	if (nID == -1) return -1;

	UINT nState = 0;//TBSTATE_WRAP;

	pToolBarItem->m_bIsVisible = !bHide;

	if (bHide)
		nState |= TBSTATE_HIDDEN ;
	else
		nState |= TBSTATE_ENABLED;


	CToolBar *pToolBar = NULL;
	pos = pListToolBar->GetHeadPosition();
	while (pos)
	{
		pToolBar = (CToolBar*)pListToolBar->GetNext(pos);
		if (pToolBar == NULL) continue;

		CToolBarCtrl &pToolBarCtrl = pToolBar->GetToolBarCtrl();

		pToolBarCtrl.SetState(nID, TBSTATE_WRAP);
		pToolBarCtrl.SetState(nID, nState);

		CFrameWnd *pFrame = pToolBar->GetDockingFrame();
		pFrame->RecalcLayout();
	}

	

	return 0;
}

CString CKOLAS3_MAINApi::GetAcceToolBarButton(CString strAcceKey, CString strMenuAlias, CObList *pListToolBarGroup, CObList *pListToolBarItem, CObList *pListToolBar)
{
	if (strAcceKey.IsEmpty() || pListToolBarGroup == NULL || pListToolBarItem == NULL || pListToolBar == NULL) return _T("");
	
	INT nDatas = pListToolBarItem->GetCount();
	POSITION pos = pListToolBarItem->GetHeadPosition();
	CToolBarItem *pToolBarItem = NULL;
	for (INT i = 0; i < nDatas; i++) {
		pToolBarItem = (CToolBarItem*)pListToolBarItem->GetNext(pos);
		if (pToolBarItem == NULL) continue;

		if (pToolBarItem->m_strMenuAlias == strMenuAlias) {
			if (pToolBarItem->m_strAcceclerator == strAcceKey) return pToolBarItem->m_strAlias;
		}
	}

	return _T("");
}

INT CKOLAS3_MAINApi::ExecuteToolBarButton(CString strToolBarButtonAlias, CString strMenuAlias, CObList *pListToolBarGroup, CObList *pListToolBarItem, CObList *pListToolBar)
{
	if (strToolBarButtonAlias.IsEmpty() || pListToolBarGroup == NULL || pListToolBarItem == NULL || pListToolBar == NULL) return -1;

	INT nID = -1;

	CToolBarItem *pToolBarItem = NULL;
	CToolBarGroup *pToolBarGroup = NULL;

	POSITION pos = NULL;
	
	CStringArray StringArray;

	pos = pListToolBarGroup->GetHeadPosition();
	while (pos)
	{
		pToolBarGroup = (CToolBarGroup*)pListToolBarGroup->GetNext(pos);
		if (pToolBarGroup == NULL) continue;

		if (pToolBarGroup->m_strMenuAlias == strMenuAlias)
		{
			StringArray.Add(pToolBarGroup->m_strAlias);
		}
	}

	if (StringArray.GetSize() < 1) return -1;

	pos = pListToolBarItem->GetHeadPosition();
	while (pos)
	{
		pToolBarItem = (CToolBarItem*)pListToolBarItem->GetNext(pos);
		if (pToolBarItem == NULL) continue;
		
		if (!IsInStrArr(&StringArray, pToolBarItem->m_strToolGroupAlias)) continue;

		if (pToolBarItem->m_strAlias == strToolBarButtonAlias)
		{
			nID = pToolBarItem->m_nID;
			break;
		}
	}
	
	if (nID == -1) return -1;

	INT nState = -1;
	CToolBar *pToolBar = NULL;
	pos = pListToolBar->GetHeadPosition();
	while (pos)
	{
		pToolBar = (CToolBar*)pListToolBar->GetNext(pos);
		if (pToolBar == NULL) continue;

		CToolBarCtrl &pToolBarCtrl = pToolBar->GetToolBarCtrl();

		nState = pToolBarCtrl.GetState(nID);
		if (nState < 0) continue;

		if ((nState & TBSTATE_HIDDEN) != TBSTATE_HIDDEN &&
			(nState & TBSTATE_ENABLED) == TBSTATE_ENABLED &&
			(nState & TBSTATE_INDETERMINATE) != TBSTATE_INDETERMINATE)
		{
			m_pParentWnd->PostMessage(WM_COMMAND, nID, 0);
			return 0;
		}
	}	

	return 0;
}

BOOL CKOLAS3_MAINApi::IsInStrArr(CStringArray *pStringArray, CString str)
{
	INT nCount = pStringArray->GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		if (str == pStringArray->GetAt(idx))
			return TRUE;
	}

	return FALSE;
}

VOID CKOLAS3_MAINApi::DrawTransparent(CDC*pDC, INT x, INT y, CBitmap *pBmpOrg, COLORREF crColor)
{
    BITMAP bm;
    pBmpOrg->GetObject(sizeof(BITMAP), &bm);
    CPoint size(bm.bmWidth, bm.bmHeight);
    pDC->DPtoLP(&size);
    CPoint org(0, 0);
    pDC->DPtoLP(&org);

	pDC->SetBkMode(TRANSPARENT);

    //메모리 DC(dcImage)를 생성하고, 거기에 비트맵을 선택한다.
    CDC dcImage;
    dcImage.CreateCompatibleDC(pDC);
    CBitmap* pOldBitmapImage = dcImage.SelectObject(pBmpOrg);
    dcImage.SetMapMode(pDC->GetMapMode());

    //두번째 메모리 DC(dcAn)를 생성하고, 그 안에 AND 마스크를 만든다
    CDC dcAnd;
    dcAnd.CreateCompatibleDC(pDC);
    dcAnd.SetMapMode(pDC->GetMapMode());

    CBitmap bitmapAnd;
    bitmapAnd.CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
    CBitmap* pOldBitmapAnd = dcAnd.SelectObject(&bitmapAnd);

    dcImage.SetBkColor(crColor);
    dcAnd.BitBlt(org.x, org.y, size.x, size.y, &dcImage, org.x, org.y, SRCCOPY);

    //세번째 메모리 DC(dcXor)를 생성하고, 그 안에 XOR 마스크를 만든다
    CDC dcXor;
    dcXor.CreateCompatibleDC(pDC);
    dcXor.SetMapMode(pDC->GetMapMode());

    CBitmap bitmapXor;
    bitmapXor.CreateCompatibleBitmap(&dcImage, bm.bmWidth, bm.bmHeight);
    CBitmap* pOldBitmapXor = dcXor.SelectObject(&bitmapXor);

    dcXor.BitBlt(org.x, org.y, size.x, size.y, &dcImage, org.x, org.y, SRCCOPY);

    dcXor.BitBlt(org.x, org.y, size.x, size.y, &dcAnd, org.x, org.y, 0x220326);
    //0x220326 dcXor = (NOT dcAnd) AND dcXor

    //목적 사각형 안의 픽실을 임시 메모리 DC(dcTemp)로 복사한다.
    CDC dcTemp;
    dcTemp.CreateCompatibleDC(pDC);
    dcTemp.SetMapMode(pDC->GetMapMode());
	dcTemp.SetBkMode(TRANSPARENT);
	dcTemp.SetBkColor(crColor);

    CBitmap bitmapTemp;
    bitmapTemp.CreateCompatibleBitmap(&dcImage, bm.bmWidth, bm.bmHeight);
    CBitmap* pOldBitmapTemp = dcTemp.SelectObject(&bitmapTemp);

    dcTemp.BitBlt(org.x, org.y, size.x, size.y, pDC, x, y, SRCCOPY);

    //AND와 XOR 마스크를 적용한 마지막 이미지를 임시 메모리 DC안의 이미지로 생성한다.
    dcTemp.BitBlt(org.x, org.y, size.x, size.y, &dcAnd, org.x, org.y, SRCAND);

    dcTemp.BitBlt(org.x, org.y, size.x, size.y, &dcXor, org.x, org.y, SRCINVERT);

    //화면에 결과 이미지를 블릿한다.
    pDC->BitBlt(x, y, size.x, size.y, &dcTemp, org.x, org.y, SRCCOPY);

    //기본 비트맵을 복원한다.
    dcTemp.SelectObject(pOldBitmapTemp);
    dcXor.SelectObject(pOldBitmapXor);
    dcAnd.SelectObject(pOldBitmapAnd);
    dcImage.SelectObject(pOldBitmapImage);
}

VOID CKOLAS3_MAINApi::DrawTransparent(CDC * pDC, INT x, INT y, COLORREF crColour, CBitmap *pBitmap)
{
	COLORREF crOldBack = pDC->SetBkColor(RGB(255, 255, 255));
	COLORREF crOldText = pDC->SetTextColor(RGB(0,0,0));
	CDC dcImage, dcTrans;

	// Create two memory dcs for the image and the mask
	dcImage.CreateCompatibleDC(pDC);
	dcTrans.CreateCompatibleDC(pDC);

	// Select the image into the appropriate dc
	CBitmap* pOldBitmapImage = (CBitmap*)dcImage.SelectObject(this);

	if(!pBitmap || !pBitmap->GetSafeHandle() || !pBitmap->m_hObject)
	{
		return;
	}

	// Create the mask bitmap
	CBitmap bitmapTrans;
	BITMAP bm;
	pBitmap->GetBitmap(&bm);
	INT nWidth = bm.bmWidth;
	INT nHeight = bm.bmHeight;
	bitmapTrans.CreateBitmap(nWidth, nHeight, 1, 1, NULL);

	// Select the mask bitmap into the appropriate dc
	CBitmap* pOldBitmapTrans = dcTrans.SelectObject(&bitmapTrans);
	//CBitmap* pOldBitmapTrans = dcTrans.SelectObject(pBitmap);

	// Build mask based on transparent colour
	dcImage.SetBkColor(crColour);
	dcTrans.BitBlt(0, 0, nWidth, nHeight, &dcImage, 0, 0, SRCCOPY);
	pDC->BitBlt(x, y, nWidth, nHeight, &dcTrans, 0, 0, SRCAND);
	pDC->BitBlt(x, y, nWidth, nHeight, &dcTrans, 0, 0, SRCPAINT);
	
	// Do the work - True Mask method - cool if not actual display
	//pDC->BitBlt(x, y, nWidth, nHeight, &dcImage, 0, 0, SRCINVERT);
	//pDC->BitBlt(x, y, nWidth, nHeight, &dcTrans, 0, 0, SRCAND);
	//pDC->BitBlt(x, y, nWidth, nHeight, &dcImage, 0, 0, SRCINVERT);

	// Restore settings
	dcImage.SelectObject(pOldBitmapImage);
	dcTrans.SelectObject(pOldBitmapTrans);
	pDC->SetBkColor(crOldBack);
	pDC->SetTextColor(crOldText);
}

// TransparentBlt	- Copies a bitmap transparently onto the destination DC
// hdcDest		- Handle to destination device context 
// nXDest		- x-coordinate of destination rectangle's upper-left corner 
// nYDest		- y-coordinate of destination rectangle's upper-left corner 
// nWidth		- Width of destination rectangle 
// nHeight		- height of destination rectangle 
// hBitmap		- Handle of the source bitmap
// nXSrc		- x-coordinate of source rectangle's upper-left corner 
// nYSrc		- y-coordinate of source rectangle's upper-left corner 
// colorTransparent	- The transparent color
// hPal			- Logical palette to be used with bitmap. Can be NULL
VOID CKOLAS3_MAINApi::TransparentBlt( HDC hdcDest, INT nXDest, INT nYDest, INT nWidth, INT nHeight, HBITMAP hBitmap, INT nXSrc, INT nYSrc, COLORREF colorTransparent, HPALETTE hPal )
{
	nWidth += 10;
	nHeight += 10;
	CDC dc, memDC, maskDC, tempDC;
	dc.Attach( hdcDest );
	maskDC.CreateCompatibleDC(&dc);
	CBitmap maskBitmap;
	
	//add these to store return of SelectObject() calls
	CBitmap* pOldMemBmp = NULL;
	CBitmap* pOldMaskBmp = NULL;
	HBITMAP hOldTempBmp = NULL;
	
	memDC.CreateCompatibleDC(&dc);
	tempDC.CreateCompatibleDC(&dc);
	CBitmap bmpImage;
	bmpImage.CreateCompatibleBitmap( &dc, nWidth, nHeight);
	pOldMemBmp = memDC.SelectObject( &bmpImage );
	
	// Select and realize the palette
	if( dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE && hPal )
	{
		::SelectPalette( dc, hPal, FALSE );
		dc.RealizePalette();
		
		::SelectPalette( memDC, hPal, FALSE );
	}
	
	hOldTempBmp = (HBITMAP) ::SelectObject( tempDC.m_hDC, hBitmap );
	
	memDC.BitBlt( 0,0,nWidth, nHeight, &tempDC, nXSrc, nYSrc, SRCCOPY );
	
	// Create monochrome bitmap for the mask
	maskBitmap.CreateBitmap( nWidth, nHeight, 1, 1, NULL );
	pOldMaskBmp = maskDC.SelectObject( &maskBitmap );
	memDC.SetBkColor( colorTransparent );
	
	// Create the mask from the memory DC
	maskDC.BitBlt( 0, 0, nWidth, nHeight, &memDC, 0, 0, SRCCOPY );
	
	// Set the background in memDC to black. Using SRCPAINT with black 
	// and any other color results in the other color, thus making 
	// black the transparent color
	memDC.SetBkColor(RGB(0,0,0));
	memDC.SetTextColor(RGB(255,255,255));
	memDC.BitBlt(0, 0, nWidth, nHeight, &maskDC, 0, 0, SRCAND);
	
	// Set the foreground to black. See comment above.
	dc.SetBkColor(RGB(255,255,255));
	dc.SetTextColor(RGB(0,0,0));
	dc.BitBlt(nXDest, nYDest, nWidth, nHeight, &maskDC, 0, 0, SRCAND);
	
	// Combine the foreground with the background
	dc.BitBlt(nXDest, nYDest, nWidth, nHeight, &memDC, 0, 0, SRCPAINT);	
	
	if (hOldTempBmp)
		::SelectObject( tempDC.m_hDC, hOldTempBmp);
	if (pOldMaskBmp)
		maskDC.SelectObject( pOldMaskBmp );
	if (pOldMemBmp)
		memDC.SelectObject( pOldMemBmp );
	
	dc.Detach();
}

INT CKOLAS3_MAINApi::OnSetAcceleratorDialog(CString strMenuAlias, CObList *pListToolBarGroup, CObList *pListToolBarItem, CObList *pListToolBar)
{
	CSetAcceleratorDlg dlg;

	dlg.CONNECTION_INFO = m_strCONNECTION_INFO;
	dlg.m_strMenuAlias = strMenuAlias;
	dlg.m_pListToolBarGroup = pListToolBarGroup;
	dlg.m_pListToolBarItem = pListToolBarItem;
	dlg.m_pListToolBar = pListToolBar;

	dlg.DoModal();
	return 0;
}

