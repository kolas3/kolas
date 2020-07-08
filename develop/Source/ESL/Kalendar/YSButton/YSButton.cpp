// YSButton.cpp : implementation file
//

#include "stdafx.h"
#include "YSButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYSButton

CYSButton::CYSButton()
{
	m_bCursorOnButton = FALSE;
	m_bHover = FALSE;
	m_bDrawMask = FALSE;
	m_bDrawText = FALSE;

	m_logfont.lfHeight = 0;
	// KOL.UDF.022	
	m_MemDC.DeleteDC();
	m_bMaskFlag = FALSE;
	m_hCursor = NULL;
	m_hBitmap = NULL;	
	m_nMode = TRUE;
	m_nSliceWidth = 0;
	m_nHeight = 0;
	m_rectButtonPos = NULL;
	m_nMask = NULL;
	poldbgDC = NULL;
}

CYSButton::~CYSButton()
{
	if(m_bDrawMask == TRUE){
		bgDC.SelectObject(poldbgDC);
	}
}


BEGIN_MESSAGE_MAP(CYSButton, CButton)
	//{{AFX_MSG_MAP(CYSButton)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYSButton message handlers
void CYSButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if((int)lpDrawItemStruct->itemID < 0) return;
	
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcBtn(lpDrawItemStruct->rcItem);
	int nBtnID=0;
	
	if(lpDrawItemStruct->itemState & ODS_DISABLED)
		nBtnID = ID_BTNDISABLE;
	else if(lpDrawItemStruct->itemState & ODS_SELECTED)
		nBtnID = ID_BTNDOWN;
	else if(m_bHover)
		nBtnID = ID_BTNHOVER;
	else
		nBtnID = ID_BTNUP;

//	if(!m_Bitmap[nBtnID].m_hObject) nBtnID = ID_BTNUP;
//	if(!m_Bitmap[nBtnID].m_hObject) return;

	DrawBitmap(nBtnID, rcBtn, pDC);

	//++2008.08.11 ADD BY CJY {{++
	// 비트맵위에 텍스트그리기 기능 추가
	if(TRUE == IsDrawText())
	{
		CString strDrawText;
		GetWindowText(strDrawText);
		CSize TextSize = pDC->GetTextExtent(strDrawText);

		CPoint Offset;
		Offset.x = ( rcBtn.Width() - TextSize.cx ) / 2;
		Offset.y = ( rcBtn.Height() - TextSize.cy ) / 2;
		rcBtn.OffsetRect(Offset);

		INT nOldMode = pDC->SetBkMode(TRANSPARENT);
		if(FALSE == IsWindowEnabled())
		{
			COLORREF crOldTextColor = pDC->SetTextColor(RGB(161, 161, 146));
			pDC->DrawText(strDrawText, rcBtn, DT_EDITCONTROL);
			pDC->SetTextColor(crOldTextColor);
		}
		else
		{
			pDC->DrawText(strDrawText, rcBtn, DT_EDITCONTROL);
		}
	}
	//--2008.08.11 ADD BY CJY --}}
}

BOOL CYSButton::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
//DEL 	CWnd *pWnd = this->GetParent();
//DEL 	CDC *pParentDC = pWnd->GetDC();
//DEL 	
//DEL 	CDC srcDC;
//DEL 	CBitmap srcBmp;
//DEL 	CBitmap *oldsrcBmp;
//DEL 
//DEL 	srcDC.CreateCompatibleDC(pParentDC);
//DEL 	srcBmp.CreateCompatibleBitmap(pParentDC, m_nSliceWidth, m_nHeight);
//DEL 	oldsrcBmp = srcDC.SelectObject(&srcBmp);
//DEL 
//DEL 	srcDC.BitBlt(0, 0, m_nSliceWidth, m_nHeight, pParentDC,
//DEL 		m_rectButtonPos.left, m_rectButtonPos.top, SRCCOPY);
//DEL 
//DEL 	pParentDC->SelectObject(oldsrcBmp);
//DEL 	ReleaseDC(pParentDC);
//DEL 
//DEL 	pDC->BitBlt(0,0,m_nSliceWidth,m_nHeight, &srcDC, 0, 0, SRCCOPY);
//DEL 
//DEL  	return CButton::OnEraseBkgnd(pDC);
	CRect rBtn;
	GetClientRect(&rBtn);
	DrawBitmap(ID_BTNUP, rBtn, pDC);

 	return TRUE;
}

void CYSButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bCursorOnButton == FALSE){
		TRACKMOUSEEVENT tme;
		ZeroMemory(&tme,sizeof(TRACKMOUSEEVENT));
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bCursorOnButton = _TrackMouseEvent(&tme);
	}
	CButton::OnMouseMove(nFlags, point);
}

LRESULT CYSButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	m_bHover = TRUE;
	Invalidate();
	return FALSE;
}

LRESULT CYSButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bCursorOnButton = FALSE;
	m_bHover = FALSE;
	Invalidate();
	return FALSE;
}

void CYSButton::SetToolTipText(CString strText)
{
	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.AddTool(this, (LPCTSTR)strText);
}

BOOL CYSButton::PreTranslateMessage(MSG* pMsg) 
{//툴팁을 띄우기 위해 RelayEvent를 호출해준다.
	if(m_ToolTip.GetSafeHwnd() != NULL)
		m_ToolTip.RelayEvent(pMsg);
	return CButton::PreTranslateMessage(pMsg);
}

void CYSButton::SetCursor(HCURSOR hCursor)
{
	m_hCursor = hCursor;
}

BOOL CYSButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	::SetCursor(m_hCursor);	
	Invalidate(FALSE);

	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CYSButton::LoadBitmaps(UINT nIDUp, UINT nIDDown, UINT nIDHover, UINT nIDDisable)
{
	m_nMode = 2;
	m_bMaskFlag = FALSE;
	SetButtonStyle(BS_OWNERDRAW, FALSE);
	if(nIDUp > 0) m_Bitmap[ID_BTNUP].LoadBitmap(nIDUp);
	if(nIDDown > 0) m_Bitmap[ID_BTNDOWN].LoadBitmap(nIDDown);
	if(nIDHover > 0) m_Bitmap[ID_BTNHOVER].LoadBitmap(nIDHover);
	if(nIDDisable > 0) m_Bitmap[ID_BTNDISABLE].LoadBitmap(nIDDisable);

	if(nIDUp > 0){
		BITMAP bm;
		m_Bitmap[ID_BTNUP].GetObject(sizeof(BITMAP), &bm);
		m_nSliceWidth = bm.bmWidth;
		m_nHeight = bm.bmHeight;
		SetWindowPos(NULL, 0, 0, m_nSliceWidth, m_nHeight, SWP_NOMOVE);
	}
	GetWindowRect(&m_rectButtonPos);
	return TRUE;
}

BOOL CYSButton::LoadBitmaps(UINT nIDUp, UINT nIDDown, UINT nIDHover, UINT nIDDisable, UINT nMask)
{
	m_nMode = 2;
	m_nMask = nMask;
	m_bMaskFlag = TRUE;
	SetButtonStyle(BS_OWNERDRAW, FALSE);
	if(nIDUp > 0) m_Bitmap[ID_BTNUP].LoadBitmap(nIDUp);
	if(nIDDown > 0) m_Bitmap[ID_BTNDOWN].LoadBitmap(nIDDown);
	if(nIDHover > 0) m_Bitmap[ID_BTNHOVER].LoadBitmap(nIDHover);
	if(nIDDisable > 0) m_Bitmap[ID_BTNDISABLE].LoadBitmap(nIDDisable);

	if(nIDUp > 0){
		BITMAP bm;
		m_Bitmap[ID_BTNUP].GetObject(sizeof(BITMAP), &bm);
		m_nSliceWidth = bm.bmWidth;
		m_nHeight = bm.bmHeight;
		SetWindowPos(NULL, 0, 0, m_nSliceWidth, m_nHeight, SWP_NOMOVE);
	}
	GetWindowRect(&m_rectButtonPos);
	return TRUE;
}

BOOL CYSButton::LoadBitmaps(UINT nResourceID)
{
	m_nMode = 0;
	m_bMaskFlag = FALSE;
	m_Bitmapall.LoadBitmap(nResourceID);
	
	BITMAP bm;
	m_Bitmapall.GetObject(sizeof(BITMAP), &bm);
	m_nSliceWidth = (int)(bm.bmWidth/4);	
	m_nHeight = bm.bmHeight;
	SetWindowPos(NULL, 0, 0, m_nSliceWidth, m_nHeight, SWP_NOMOVE);

	GetWindowRect(&m_rectButtonPos);

	CDC* pDC = GetDC();
	m_MemDC.DeleteDC();
	m_MemBitmap.DeleteObject();
	m_MemDC.CreateCompatibleDC(pDC);
	m_MemBitmap.LoadBitmap(nResourceID);
	m_MemDC.SelectObject(&m_MemBitmap);
	ReleaseDC(pDC);

	return TRUE;
}

BOOL CYSButton::LoadBitmaps(UINT nResourceID, UINT nMask)
{
	m_nMode = 0;
	m_nMask = nMask;
	m_bMaskFlag = TRUE;
	m_Bitmapall.LoadBitmap(nResourceID);
	
	BITMAP bm;
	m_Bitmapall.GetObject(sizeof(BITMAP), &bm);
	m_nSliceWidth = (int)(bm.bmWidth/4);
	m_nHeight = bm.bmHeight;
	SetWindowPos(NULL, 0, 0, m_nSliceWidth, m_nHeight, SWP_NOMOVE);
	GetWindowRect(&m_rectButtonPos);

	CDC* pDC = GetDC();
	m_MemDC.DeleteDC();
	m_MemBitmap.DeleteObject();
	m_MemDC.CreateCompatibleDC(pDC);
	m_MemBitmap.LoadBitmap(nResourceID);
	m_MemDC.SelectObject(&m_MemBitmap);
	ReleaseDC(pDC);

	return TRUE;
}

BOOL CYSButton::LoadBitmaps(CString strFileName)
{
	m_nMode = 1;
	m_bMaskFlag = FALSE;
	m_hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), strFileName,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	BITMAP bm;
	(CBitmap::FromHandle(m_hBitmap))->GetBitmap(&bm);
	m_nSliceWidth = (int)(bm.bmWidth/4);
	m_nHeight = bm.bmHeight;
	SetWindowPos(NULL, 0, 0, m_nSliceWidth, m_nHeight, SWP_NOMOVE);
	GetWindowRect(&m_rectButtonPos);

	CDC* pDC = GetDC();
	m_MemDC.DeleteDC();
	m_MemBitmap.DeleteObject();
	m_MemDC.CreateCompatibleDC(pDC);
	m_MemBitmap.Attach(m_hBitmap);
	m_MemDC.SelectObject(&m_MemBitmap);
	ReleaseDC(pDC);

	return TRUE;
}

BOOL CYSButton::LoadBitmaps(CString strFileName, UINT nMask)
{
	m_nMode = 1;
	m_nMask = nMask;
	m_bMaskFlag = TRUE;
	m_hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), strFileName,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	BITMAP bm;
	(CBitmap::FromHandle(m_hBitmap))->GetBitmap(&bm);
	m_nSliceWidth = (int)(bm.bmWidth/4);
	m_nHeight = bm.bmHeight;
	SetWindowPos(NULL, 0, 0, m_nSliceWidth, m_nHeight, SWP_NOMOVE);
	GetWindowRect(&m_rectButtonPos);

	CDC* pDC = GetDC();
	m_MemDC.DeleteDC();
	m_MemBitmap.DeleteObject();
	m_MemDC.CreateCompatibleDC(pDC);
	m_MemBitmap.Attach(m_hBitmap);
	m_MemDC.SelectObject(&m_MemBitmap);
	ReleaseDC(pDC);

	return TRUE;
}

void CYSButton::DrawBitmap(UINT nID, CRect rcBtn, CDC *pDC)
{

	if(0 == m_nMode || 1 == m_nMode)
	{
		UINT INTERVAL = m_nSliceWidth * nID;
		if(FALSE == m_bMaskFlag)
		{
			pDC->BitBlt(0, 0, m_nSliceWidth, m_nHeight, &m_MemDC, INTERVAL, 0, SRCCOPY);
		}
	}
// 	if(m_nMode == 0 || m_nMode == 1){
// 		CDC			oriDC;
// 		UINT		INTERVAL = m_nSliceWidth * nID;
// 		oriDC.CreateCompatibleDC(pDC);
// 		CBitmap *poldoriDC = oriDC.SelectObject(m_nMode ? CBitmap::FromHandle(m_hBitmap) : &m_Bitmapall);
// 			if(m_bMaskFlag == FALSE){					
// 					pDC->BitBlt(0,0,m_nSliceWidth,m_nHeight,&oriDC,INTERVAL,0, SRCCOPY);
// 					oriDC.SelectObject(poldoriDC);
// 			}
// 			else{
// 					UINT MASK = m_nMask;
// 					CDC srcDC, maskDC, memDC;
// 					CBitmap srcBmp, maskBmp, memBmp;
// 					CBitmap *poldsrcDC, *poldmaskDC, *poldmemDC;
// 					//1. 원본이미지에 4개 버튼의 그림이 들어있는 oriDC에서 srcDC에 1/4을 복사해준다.
// 					srcDC.CreateCompatibleDC(pDC);
// 					srcBmp.CreateCompatibleBitmap(pDC, m_nSliceWidth, m_nHeight);
// 					poldsrcDC = srcDC.SelectObject(&srcBmp);
// 					srcDC.BitBlt(0,0,m_nSliceWidth, m_nHeight,&oriDC,INTERVAL,0,SRCCOPY);
// 					//2. 마스크를 가지는 DC(그림이 그려질부분은 BLACK, 배경은 WHITE)
// 					maskDC.CreateCompatibleDC(pDC);
// 					maskBmp.CreateBitmap(m_nSliceWidth, m_nHeight,1, 1, NULL);
// 					poldmaskDC = maskDC.SelectObject(&maskBmp);
// 					srcDC.SetBkColor(MASK);
// 					maskDC.BitBlt(0,0,m_nSliceWidth,m_nHeight,&srcDC,0,0,SRCCOPY);
// 					//3. 원본배경은 검은색 아닌것은 원본그대로
// 					srcDC.SetBkColor(RGB(0,0,0));
// 					srcDC.SetTextColor(RGB(255,255,255));
// 					srcDC.BitBlt(0,0,m_nSliceWidth,m_nHeight,&maskDC,0,0,SRCAND);
// 					//4. 배경을 저장
// 					if(m_bDrawMask == FALSE){
// 						CBitmap bgBmp;
// 						bgDC.CreateCompatibleDC(pDC);
// 						bgBmp.CreateCompatibleBitmap(pDC, m_nSliceWidth, m_nHeight);
// 						poldbgDC = bgDC.SelectObject(&bgBmp);
// 						bgDC.BitBlt(0,0,m_nSliceWidth,m_nHeight,pDC,0,0,SRCCOPY);
// 						m_bDrawMask = TRUE;
// 					}
// 					memDC.CreateCompatibleDC(pDC);
// 					memBmp.CreateCompatibleBitmap(&bgDC,m_nSliceWidth,m_nHeight);
// 					poldmemDC = memDC.SelectObject(&memBmp);
// 					memDC.BitBlt(0,0,m_nSliceWidth,m_nHeight,&bgDC,0,0,SRCCOPY);  
// 					//5. 배경과 합쳐서 출력한다.
// 					memDC.BitBlt(0,0,m_nSliceWidth,m_nHeight,&maskDC,0,0,SRCAND);
// 					memDC.BitBlt(0,0,m_nSliceWidth,m_nHeight,&srcDC,0,0,SRCPAINT);
// 
// 					pDC->BitBlt(0,0,m_nSliceWidth,m_nHeight,&memDC,0,0,SRCCOPY);
// 					
// 					oriDC.SelectObject(poldoriDC);
// 					srcDC.SelectObject(poldsrcDC);
// 					maskDC.SelectObject(poldmaskDC);
// 					memDC.SelectObject(poldmemDC);
// 			}
// 	}
	else {//m_nMode == 2
			CDC oriDC;
			oriDC.CreateCompatibleDC(pDC);
			CBitmap *poldoriDC = oriDC.SelectObject(&m_Bitmap[nID]);
			if(m_bMaskFlag == FALSE){
					pDC->BitBlt(0,0,m_nSliceWidth,m_nHeight,&oriDC,0,0,SRCCOPY);
					oriDC.SelectObject(poldoriDC);
			}
			else{
					UINT MASK = m_nMask;
					CDC srcDC, maskDC, memDC;
					CBitmap srcBmp, maskBmp, memBmp;
					CBitmap *poldsrcDC, *poldmaskDC, *poldmemDC;
					//1. 4개의 버튼 그림중에서 하나를 선택
					srcDC.CreateCompatibleDC(pDC);
					srcBmp.CreateCompatibleBitmap(pDC, m_nSliceWidth, m_nHeight);
					poldsrcDC = srcDC.SelectObject(&srcBmp);
					srcDC.BitBlt(0,0,m_nSliceWidth, m_nHeight,&oriDC,0,0,SRCCOPY);

			//		srcDC.CreateCompatibleDC(pDC);
			//		poldsrcDC = srcDC.SelectObject(&m_Bitmap[nID]);
			//		srcDC.BitBlt(0,0,m_nSliceWidth, m_nHeight,pDC,0,0,SRCCOPY);//?
					//2. 마스크를 가지는 DC(그림이 그려질부분은 BLACK, 배경은 WHITE)
					maskDC.CreateCompatibleDC(pDC);
					maskBmp.CreateBitmap(m_nSliceWidth, m_nHeight,1, 1, NULL);
					poldmaskDC = maskDC.SelectObject(&maskBmp);
					srcDC.SetBkColor(MASK);
					maskDC.BitBlt(0,0,m_nSliceWidth,m_nHeight,&srcDC,0,0,SRCCOPY);
					//3. 원본배경은 검은색 아닌것은 원본그대로
					srcDC.SetBkColor(RGB(0,0,0));
					srcDC.SetTextColor(RGB(255,255,255));
					srcDC.BitBlt(0,0,m_nSliceWidth,m_nHeight,&maskDC,0,0,SRCAND);
					//4. 배경을 저장
					if(m_bDrawMask == FALSE){
						CBitmap bgBmp;
						bgDC.CreateCompatibleDC(pDC);
						bgBmp.CreateCompatibleBitmap(pDC, m_nSliceWidth, m_nHeight);
						poldbgDC = bgDC.SelectObject(&bgBmp);
						bgDC.BitBlt(0,0,m_nSliceWidth,m_nHeight,pDC,0,0,SRCCOPY);
						m_bDrawMask = TRUE;
					}
					memDC.CreateCompatibleDC(pDC);
					memBmp.CreateCompatibleBitmap(&bgDC,m_nSliceWidth,m_nHeight);
					poldmemDC = memDC.SelectObject(&memBmp);
					memDC.BitBlt(0,0,m_nSliceWidth,m_nHeight,&bgDC,0,0,SRCCOPY);  
					//5. 배경과 합쳐서 출력한다.
					memDC.BitBlt(0,0,m_nSliceWidth,m_nHeight,&maskDC,0,0,SRCAND);
					memDC.BitBlt(0,0,m_nSliceWidth,m_nHeight,&srcDC,0,0,SRCPAINT);

					pDC->BitBlt(0,0,m_nSliceWidth,m_nHeight,&memDC,0,0,SRCCOPY);
					
					oriDC.SelectObject(poldoriDC);
					srcDC.SelectObject(poldsrcDC);
					maskDC.SelectObject(poldmaskDC);
					memDC.SelectObject(poldmemDC);	

					/*
					*/
			}
	}
}
//++2008.08.11 ADD BY CJY {{++
// 비트맵위에 텍스트그리기 기능 추가
void CYSButton::SetDrawText(BOOL bIsDraw)
{
	m_bDrawText = bIsDraw;
}

BOOL CYSButton::IsDrawText()
{
	return m_bDrawText;
}
//--2008.08.11 ADD BY CJY --}}
