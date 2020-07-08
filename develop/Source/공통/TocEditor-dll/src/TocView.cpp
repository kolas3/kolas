// TocView.cpp : implementation file
//

#include "stdafx.h"
#include "TocView.h"

#include "TocParse.h"
#include "TocEditorDef.h"

#include "TocViewerDlg.h"
#include "EcoMathViewer.h"

#include "TocEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ONE_WIDTH	6		// 한문자당 width pixel
//#define MARGIN_X		3
#define MARGIN_Y		5
#define	LINE_PIXEL_X	3
#define	LINE_PIXEL_Y	20		// 16 도 괜찮음
//#define MAX_TOC			4096
#define CONVERT_HEAD_TAG	_T("[")
#define CONVERT_END_TAG		_T("]")
#define RGB_NORMAL_COLOR	RGB(0,0,0)		// 검정색
#define RGB_TAG_COLOR		RGB(0,127,0)	// 청록색
#define RGB_CHOROK_COLOR	RGB(255,0,0)	// 빨강색

#define	SOOSIK_START_TAG	_T("$$")	// added by jungjoo	2002/4/4
#define	SOOSIK_END_TAG		_T("$$")	// added by jungjoo	2002/4/4
/////////////////////////////////////////////////////////////////////////////
// CTocView

IMPLEMENT_DYNCREATE(CTocView, CScrollView)

CTocView::CTocView()
{
//	m_sDelimiter = _T("\r\n") ;
	m_saLineText.RemoveAll();
	m_saConverted.RemoveAll();
	m_Rect= CRect(0,0,0,0) ;
	m_nLine= 0 ;

	m_arrChorokLine.SetSize(0,1) ;
	m_pParent= NULL ;
	m_bIsDan= TRUE ;
	m_bSetCursor= FALSE ;
//	ZeroMemory(&m_lf, sizeof(LOGFONT)) ;
}

CTocView::~CTocView()
{
	m_saLineText.RemoveAll();
	m_saLineText.FreeExtra();
	m_saConverted.RemoveAll();
	m_saConverted.FreeExtra();

	m_arrChorokLine.RemoveAll() ;
	m_arrChorokLine.FreeExtra() ;
}


BEGIN_MESSAGE_MAP(CTocView, CScrollView)
	//{{AFX_MSG_MAP(CTocView)
    ON_WM_MOUSEACTIVATE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTocView drawing

VOID CTocView::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	theFont.DeleteObject() ;
	VERIFY(theFont.CreatePointFont(95,_T("굴림체"),pDC)) ;
//	theFont.GetLogFont(&m_lf);
//	m_lf.lfUnderline= TRUE ;
//	theFont.DeleteObject() ;
//	theFont.CreatePointFontIndirect(&m_lf, pDC) ;
	if ( pDC->SelectObject(&theFont)==NULL ) {
		AfxMessageBox( _T("SelectObject fn Error!") , MB_ICONERROR ) ;
		return ;
	}

	CString sOneLine ;
	for ( INT i=0 ; i<m_saLineText.GetSize() ; i++ )
	{
		sOneLine= m_saLineText.GetAt(i) ;
		if ( sOneLine.Mid(0,1)!=CONVERT_HEAD_TAG )
		{
			pDC->SetTextColor(RGB_NORMAL_COLOR) ;
			SetTextOut( pDC, LINE_PIXEL_X, (i*LINE_PIXEL_Y), sOneLine) ;
//			pDC->TextOut(LINE_PIXEL_X, (i*LINE_PIXEL_Y), sOneLine) ;
		}
		else
		{
			INT nEndIdx= sOneLine.Find(CONVERT_END_TAG) ;
			if ( nEndIdx<0 )
			{
				pDC->SetTextColor(RGB_NORMAL_COLOR) ;
				SetTextOut( pDC, LINE_PIXEL_X, (i*LINE_PIXEL_Y), sOneLine) ;
//				pDC->TextOut(LINE_PIXEL_X, (i*LINE_PIXEL_Y), sOneLine) ;
				continue ;
			}
			
			INT nFind= 0 ;
			for ( INT nIdx=0 ; nIdx<m_saConverted.GetSize() ; nIdx++ )
			{
				nFind= sOneLine.Find( m_saConverted.GetAt(nIdx) ) ;
				if ( nFind>0 )
				{
					pDC->SetTextColor(RGB_TAG_COLOR) ;		// 태그라인
					SetTextOut( pDC, LINE_PIXEL_X, (i*LINE_PIXEL_Y), sOneLine) ;
//					pDC->TextOut(LINE_PIXEL_X, (i*LINE_PIXEL_Y), sOneLine) ;
					break ;
				}
			}
			
			if ( nFind<=0 )
			{
				pDC->SetTextColor(RGB_NORMAL_COLOR) ;
				SetTextOut( pDC, LINE_PIXEL_X, (i*LINE_PIXEL_Y), sOneLine) ;
//				pDC->TextOut(LINE_PIXEL_X, (i*LINE_PIXEL_Y), sOneLine) ;
			}
		}

	}

	m_nLine= 0 ;
}

/////////////////////////////////////////////////////////////////////////////
// CTocView diagnostics

#ifdef _DEBUG
VOID CTocView::AssertValid() const
{
	CScrollView::AssertValid();
}

VOID CTocView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTocView message handlers

VOID CTocView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal) ;	// 형식적으로라도 해줘야 한다.

	m_pParent= GetParent() ;
}

INT CTocView::SetTextOutString(CString sText, CString sDelimiter, CString sExeDir, BOOL bIsMokcha, BOOL bIsDan)
{
	// 리턴값으로 \n 만 있는 경우 이를 모조리 \r\n 으로 바꿔준다.
	INT nIdx= sText.Find('\n') ;
	if ( nIdx>=1 )
	{
		if ( sText.GetAt(nIdx-1)!='\r' )
			sText.Replace( _T("\n"), _T("\r\n") ) ;
	}
	else if ( nIdx==0 )
	{
		sText.Replace( _T("\n"), _T("\r\n") ) ;
	}

	m_saLineText.RemoveAll();
	m_bIsDan= bIsDan ;

	InitScroll() ;

	sExeDir.TrimLeft();
	sExeDir.TrimRight(_T("\\"));
	sExeDir+=_T("\\");	
	CString metafile= sExeDir+sViewerFileName;

	CFileException ex;
	if( ! m_ViewerCfgFile.Open(metafile, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary , &ex) ) {
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		AfxMessageBox(szError) ;
		return -1 ;
	}
	TCHAR cUni;
	m_ViewerCfgFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		m_ViewerCfgFile.SeekToBegin();
	}

	if( Convert2ViewerTag(sText, sDelimiter, bIsMokcha, bIsDan) )
	{
		m_ViewerCfgFile.Close();
		return -2 ;
	}
	m_ViewerCfgFile.Close();
	return 0 ;
}

INT CTocView::Convert2ViewerTag(CString &sResult, CString sDelimiter, BOOL bIsMokcha, BOOL bIsDan)
{
	sResult = sDelimiter + sResult ;

	CString sHeadTag = _T("") ;
	CString sTailTag = _T("") ;
	CString sConv = _T("") ;
	CString sRecord;
	CString sToken;
	TCHAR cUni;
	m_ViewerCfgFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		m_ViewerCfgFile.SeekToBegin();
	}

	m_saConverted.RemoveAll() ;

	if ( bIsMokcha )		// 목차일 경우에만 적용시키자.
	{
		while (m_ViewerCfgFile.ReadString(sRecord))	// 태그를 치환해 주자.
		{
			sRecord.TrimLeft();
			sRecord.TrimRight();

			if(sRecord.IsEmpty()) continue;

			// =================================================================================================
			// 주석 제거
			// =================================================================================================
			INT iCurIdx = sRecord.Find(_T(".."));
			if(iCurIdx >= 0) {
				sRecord = sRecord.Left(iCurIdx);
				sRecord.TrimLeft();
				sRecord.TrimRight();

				if(sRecord.IsEmpty()) {
					continue;
				}
			}

			// =================================================================================================
			// 레코드 형식 [TAG] | [태그이름]
			// =================================================================================================
			CParse tokenParse(sRecord.GetBuffer(0));
			tokenParse.SetDelimeter(_T("|"));

			const INT iElementCnt = 2;
			CString sElement[iElementCnt];

			for(INT i = 0 ; tokenParse.HasMoreToken() && i < iElementCnt ; i++)
			{
				sElement[i] = tokenParse.NextToken();
				sElement[i].TrimLeft();
				sElement[i].TrimRight();
			}

			sHeadTag= _T("<") + sElement[0] + _T(">") ;
			sTailTag= _T("</") + sElement[0] + _T(">") ;
			sConv= CONVERT_HEAD_TAG ;	// _T("[") 
			sConv+=	sElement[1] ;
			sConv+= CONVERT_END_TAG ;	// _T("] ")
			sConv+= _T(" ") ;
			m_saConverted.Add( sElement[1] ) ;

//			sHeadTag.MakeUpper() ;	sTailTag.MakeUpper() ;
//			sResult.Replace( sDelimiter+sHeadTag, sDelimiter+sConv );
//			sResult.Replace( sDelimiter+sTailTag, sDelimiter+_T("") );
			sHeadTag.MakeLower() ;	sTailTag.MakeLower() ;
			sResult.Replace( sDelimiter+sHeadTag, sDelimiter+sConv );
			sResult.Replace( sDelimiter+sTailTag, sDelimiter+_T("") );
		}

	}

	INT nLength= sDelimiter.GetLength() ;
	sResult.Delete(0, nLength) ;	// 위에서 강제로 붙였던 sDelimiter 을 제거하기 위해

	m_saLineText.RemoveAll();

	// 우선 기사제어번호 태그부터 없애준다.

	CString sEndTag ;
	INT nFindStart= -1 ;
	INT nFindEnd= -1 ;
	sEndTag= _END_CHOROK_KEY_ ;
	INT nLen= sEndTag.GetLength() ;

	CString token ;
	INT nLine= sResult.Replace(sDelimiter, sDelimiter) ;
	INT nStart=0 ;
	INT nFind=-1 ;
	for ( INT i=0 ; i<=nLine ; i++ )
	{
		nFind= sResult.Find( sDelimiter, nStart ) ;

		if ( nFind<0 )	// 마지막 라인
			token= sResult.Mid(nStart) ;
		else
			token= sResult.Mid( nStart, nFind-nStart ) ;

		if( !bIsDan ) {
			// 기사제어번호는 없애주자.
			nFindStart= token.Find(_START_CHOROK_KEY_) ;
			if ( nFindStart>=0 )
			{
				nFindEnd= token.Find(sEndTag, nFindStart) ;
				if ( nFindEnd>=0 )
				{
					token.Delete( nFindStart, nFindEnd+nLen-nFindStart) ;
				}
			}
		}

		m_saLineText.Add(token) ;
		nStart= nFind+ nLength ;
	}

	UpdateScrollSize() ;

	Invalidate() ;

	return 0 ;
}

VOID CTocView::SetTextOut(CDC *pDC, INT x, INT y, CString str)
{
	m_nLine++ ;
	INT nSize= m_arrChorokLine.GetSize() ;
	for( INT i=0 ; i<nSize ; i++ )
	{
		if ( m_nLine==m_arrChorokLine.GetAt(i) )
		{
			pDC->SetTextColor(RGB_CHOROK_COLOR) ;		// 초록이 있는 기사에 색을 칠해준다.
			break ;
		}
	}

	// 수식을 보여주기 위해 추가한 부분
	// added by jungjoo 2002.4.17
	INT nIdx, nIdx1, nLeftIdx, nRightIdx ;
	nIdx= nIdx1= 0 ;
	nLeftIdx= nRightIdx= -1 ;
	INT nFindIdx=-1 ;
	UINT X_coordinates= x;

	CString sStartTag, sEndTag ;
	sStartTag= SOOSIK_START_TAG ;
	sEndTag= SOOSIK_END_TAG ;

	CString sFront ;
	CString sSoosik ;

	while(TRUE)
	{
		nLeftIdx= str.Find(sStartTag, nIdx) ;
		if ( nLeftIdx<0 )	break ;

		nIdx1= nLeftIdx + sStartTag.GetLength() + 1;	// 수식태그 사이에 빈칸이 하나라도 있어야 하기 때문에 +1을 해주었다.
		nRightIdx= str.Find(sEndTag, nIdx1) ;
		if ( nRightIdx<0 )	break ;

		// 여기서 수식 앞 부분과 수식을 그려준다.
		// 수식 앞부분
		sFront= str.Mid(nIdx, nLeftIdx-nIdx) ;
		pDC->TextOut(X_coordinates, y+MARGIN_Y, sFront) ;

		X_coordinates+= sFront.GetLength()*ONE_WIDTH ; // 한글자당 픽셀을 계산해 주면 된다. 굴림체이므로..

		// 태그까지 포함한 수식 부분
		sSoosik= str.Mid(nLeftIdx, nRightIdx-nLeftIdx+sEndTag.GetLength()) ;

		// 수식을 그리다가 실패했을 경우에는 실패한 메시지를 띄우고,
		// 수식 태그 그대로 보여주어야 한다.
		RECT Rect ;
		CEcoMathViewer * pViewer= NULL;

		try {
			POINT Position ;
			Position.x = X_coordinates ;
			Position.y = y;

			CreateMathViewerObject(&pViewer);
			pViewer->SetMathStatement(sSoosik);
			if(!pViewer->parse())	// 실패
			{
				pDC->TextOut(X_coordinates, y+MARGIN_Y, sSoosik) ;
				X_coordinates += sSoosik.GetLength()*ONE_WIDTH;
			}
			else		// 성공
			{
				pDC->SetBkMode(TRANSPARENT);
				pViewer->Cal_Size(Rect,0,pDC);
				pViewer->Display(pDC,Position,20, Rect);

//				pDC->Rectangle(30,0,Rect.right + 30 , Rect.bottom);

//				ReleaseDC(pDC);
				X_coordinates += Rect.right ;
			}

		} catch(CString & err) {
			AfxMessageBox(err);
		}

		delete pViewer ;

		// 다음 수식을 찾기 위해...
		nIdx= nRightIdx + sEndTag.GetLength() ;
	}

	// 뒷부분을 그려준다.
	pDC->TextOut(X_coordinates, y+MARGIN_Y, str.Mid(nIdx)) ;

}

VOID CTocView::UpdateScrollSize()
{
	INT nCnt= m_saLineText.GetSize() ;		// y 좌표
	UINT nLength= 0 ;						// x 좌표
	CString sOneLine ;
	for ( INT idx=0 ; idx<nCnt ; idx++ )
	{
		sOneLine= m_saLineText.GetAt(idx) ;
		if ( _tcsclen(sOneLine)>nLength )
			nLength= _tcsclen(sOneLine) ;
	}

	CSize sizeTotal ;
	sizeTotal.cx= nLength * ONE_WIDTH + LINE_PIXEL_X ;
	sizeTotal.cy= nCnt * LINE_PIXEL_Y + MARGIN_Y ;

	SetScrollSizes(MM_TEXT, sizeTotal) ;
}

VOID CTocView::InitScroll()
{
	GetClientRect(m_Rect) ;
	if ( m_Rect.IsRectEmpty() )	return ;

	CSize sizeTotal ;
	sizeTotal.cx= m_Rect.Width() ;
	sizeTotal.cy= m_Rect.Height() ;

	SetScrollSizes(MM_TEXT, sizeTotal) ;
}

INT CTocView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return 0 ;		// kolis 에 붙이면 에러 나는 현상을 방지하기 위해
}

VOID CTocView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);

	CPoint point ;
	point= GetScrollPosition() ;
	switch ( nSBCode ) {
	case SB_BOTTOM :		// Scroll to bottom.
		break ;
	case SB_TOP :			// Scroll to top. 
		break ;
	case SB_ENDSCROLL :		// End scroll.
		return ;
	case SB_LINEDOWN :		// Scroll one line down.
		point.y+= LINE_PIXEL_Y+1 ;
		break ;
	case SB_LINEUP :		// Scroll one line up.
		point.y-= LINE_PIXEL_Y+1 ;
		break ;
	case SB_PAGEDOWN :		// Scroll one page down.
		point.y+= (LINE_PIXEL_Y*10)+1 ;
		break ;
	case SB_PAGEUP :		// Scroll one page up.
		point.y-= (LINE_PIXEL_Y*10)+1 ;
		break ;
	case SB_THUMBPOSITION :	// Scroll to the absolute position. The current position is provided in nPos.
		return ;
	case SB_THUMBTRACK :	// Drag scroll box to specified position. The current position is provided in nPos.
		point.y= nPos ;
		break ;
	}

	ScrollToPosition(point) ;
}

VOID CTocView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);

	CPoint point ;
	point= GetScrollPosition() ;
	switch ( nSBCode ) {
	case SB_BOTTOM :		// Scroll to bottom.
		break ;
	case SB_TOP :			// Scroll to top. 
		break ;
	case SB_ENDSCROLL :		// End scroll.
		return ;
	case SB_LINEDOWN :		// Scroll one line down.
		point.x+= LINE_PIXEL_Y ;
		break ;
	case SB_LINEUP :		// Scroll one line up.
		point.x-= LINE_PIXEL_Y ;
		break ;
	case SB_PAGEDOWN :		// Scroll one page down.
		point.x+= (LINE_PIXEL_Y)*10 ;
		break ;
	case SB_PAGEUP :		// Scroll one page up.
		point.x-= (LINE_PIXEL_Y)*10 ;
		break ;
	case SB_THUMBPOSITION :	// Scroll to the absolute position. The current position is provided in nPos.
		return ;
	case SB_THUMBTRACK :	// Drag scroll box to specified position. The current position is provided in nPos.
		point.x= nPos ;
		break ;
	}

	ScrollToPosition(point) ;
}

VOID CTocView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( m_bIsDan )	return ;
	if ( m_arrChorokLine.GetSize()<=0 )	return ;

	CPoint pt ;
	pt= GetScrollPosition() ;

//	long x= point.x ;
	long y= point.y + pt.y - 3 ;	// 오차 때문에 -3 를 해준다.

	UINT nLine=0 ;
	nLine= y / LINE_PIXEL_Y ;
	nLine++ ;

//	CTocViewerDlg* pParent= (CTocViewerDlg*)GetParent() ;
//	if ( pParent==NULL )	return ;

	CString sText ;
	((CTocViewerDlg*)m_pParent)->GetChorokFromLine(nLine, sText) ;

	if ( sText.IsEmpty() )	return ;

//	CTocViewerDlg dlg(m_pParent) ;	// 초록뷰어 또한 CTocViewerDlg 를 부모로 하자. added by jungjoo 2002.5.2
	CTocViewerDlg dlg(((CTocViewerDlg*)m_pParent)->m_pParent) ;
//	dlg.SetWindowText(_T("TEST")) ;		// 안된다.
	dlg.SetTocViewer( sText, FALSE, m_bIsDan ) ;
	dlg.SetViewer(_T("초록"), nLine, TRUE) ;

	BOOL bPrev, bNext ;
	bPrev= bNext= FALSE ;
	bPrev= ((CTocEditorDlg*)((CTocViewerDlg*)m_pParent)->m_pParent)->KisaExistPrevnNext(nLine, TRUE, TRUE) ;
	bNext= ((CTocEditorDlg*)((CTocViewerDlg*)m_pParent)->m_pParent)->KisaExistPrevnNext(nLine, FALSE, TRUE) ;
	dlg.EnablePrevNext(bPrev, bNext) ;
	dlg.DoModal() ;

	CScrollView::OnLButtonDown(nFlags, point);
}

VOID CTocView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( m_bIsDan )	return ;
	if ( m_arrChorokLine.GetSize()<=0 )	return ;

	// 마우스 포인터를 바꿔주기 위해
//	HCURSOR hCursor= LoadCursor( NULL, MAKEINTRESOURCE(IDC_MYCURSOR) ) ;

	CPoint pt ;
	pt= GetScrollPosition() ;

//	long x= point.x ;
	long y= point.y + pt.y - 3 ;	// 오차 때문에 -3 를 해준다.

	INT nLine=0 ;
	nLine= y / LINE_PIXEL_Y ;
	nLine++ ;

//	CTocViewerDlg* pParent= (CTocViewerDlg*)GetParent() ;
//	if ( pParent==NULL )
//	{
//		m_bSetCursor=FALSE ;
//		return ;
//	}

	CString sText ;
	((CTocViewerDlg*)m_pParent)->GetChorokFromLine(nLine, sText) ;

	if ( sText.IsEmpty() )
	{
		m_bSetCursor=FALSE ;
		return ;
	}

	m_bSetCursor=TRUE ;

	CScrollView::OnMouseMove(nFlags, point);
}

BOOL CTocView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if ( m_bSetCursor )
	{
		SetCursor (AfxGetApp()->LoadCursor (IDC_MYCURSOR));
		return 1 ;		// 반드시...
	}

	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}
