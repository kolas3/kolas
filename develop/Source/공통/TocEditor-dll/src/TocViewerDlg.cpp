// TocViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TocViewerDlg.h"
#include "TocEditorDlg.h"

#include <AfxPriv.h>

#include "TocEditorDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTocViewerDlg dialog


CTocViewerDlg::CTocViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTocViewerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTocViewerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent= pParent ;
	m_arrChorokLine.SetSize(0,1) ;
//	m_pTocView= NULL ;
	m_bIsMokcha= TRUE ;
	m_bIsDan= TRUE ;
	m_sTitle.Empty() ;
	m_bMove= FALSE ;
	m_bPrev= m_bNext= FALSE ;
	m_nLine= -1; 

	m_DlgBrush.CreateSolidBrush(RGB(245, 245, 245));
}

CTocViewerDlg::~CTocViewerDlg()
{
	m_arrChorokLine.RemoveAll() ;
	m_arrChorokLine.FreeExtra() ;
	m_DlgBrush.DeleteObject();
}

VOID CTocViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTocViewerDlg)
	DDX_Control(pDX, IDC_TOCVIEWER, m_TocView);
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTocViewerDlg, CDialog)
	//{{AFX_MSG_MAP(CTocViewerDlg)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(IDC_BUTTON_PREV, OnPrevButton)
	ON_COMMAND(IDC_BUTTON_NEXT, OnNextButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTocViewerDlg message handlers

BOOL CTocViewerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_TocView.SetReadOnly(TRUE);
	m_TocView.bViewer = TRUE;
/*
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	
	cf.dwMask = CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_SIZE;
	cf.yHeight = 250;
//	cf.dwEffects = CFE_UNDERLINE ;
//	lstrcpyA(cf.szFaceName, T2A((LPTSTR)(LPCTSTR)GetFaceName()));
	cf.crTextColor = RGB(255,20,30);

	m_TocView.SetWordCharFormat(cf);
*/
//	m_TocView.SetOptions(ECOOP_OR, ECO_AUTOVSCROLL | ECO_AUTOHSCROLL);
	// TODO: Add extra initialization here
	// ... standard code
	
/*	// create the View
	CCreateContext cc;
	cc.m_pNewViewClass = RUNTIME_CLASS(CTocView);
	cc.m_pCurrentDoc = NULL;
	cc.m_pNewDocTemplate = NULL;
	cc.m_pLastView = NULL;
	cc.m_pCurrentFrame = NULL;

	m_pTocView = (CTocView*)CreateNewView(&cc, this, CRect(0, 0, 0, 0), 0);
	if (m_pTocView == NULL)
		EndDialog(IDCANCEL);

	// place the View in the view rectangle
	CRect rect;
	GetDlgItem(IDC_MY_VIEW_RECT)->GetWindowRect(&rect);
	ScreenToClient(rect);
	m_pTocView->MoveWindow(&rect);
*/
//	cfg 파일 경로설정.
/*	TCHAR m_exeDir[256];
	GetCurrentDirectory( 256 , m_exeDir ) ;
	m_sWorkDir = m_exeDir ;
	m_sWorkDir.TrimRight(_T("\\")) ;
	INT idx= m_sWorkDir.ReverseFind( '\\' ) ;
	if ( idx<=0 ) {
		AfxMessageBox( _T("cfg 파일의 경로를 얻을수가 없습니다.\r\nViewer 를 설정하지 못했습니다!") , MB_ICONERROR | MB_OK ) ;
		return FALSE ;
	}
	m_sWorkDir= m_sWorkDir.Mid(0,idx) ;
//	m_sWorkDir += _T("\\cfg") ;
	m_sWorkDir += PATH_CFG ;
*/
	
	
	
//	m_pTocView->ShowScrollBar(SB_BOTH) ;
//	CSize sizeTotal;
//	sizeTotal.cx = sizeTotal.cy = 100;
//	m_pTocView->SetScrollSizes(MM_TEXT, sizeTotal) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CTocViewerDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here
	m_TocView.bViewer = TRUE;
	m_TocView.OnInitialUpdate();
	//m_TocView.GetWindowText(m_sText);
	m_TocView.SetWindowText(m_sText);

	CRect rc ;
	GetWindowRect(&rc) ;
	if ( m_bMove )
		SetWindowPos(NULL, rc.left+30, rc.top+30, rc.Width(), rc.Height(), SWP_SHOWWINDOW) ;

	if ( ! m_sTitle.IsEmpty() )
		SetWindowText( _T("뷰어 - ") + m_sTitle ) ;

	GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(m_bPrev);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(m_bNext);

}

CWnd* CTocViewerDlg::CreateNewView(CCreateContext *pContext, CWnd *pParent, CRect &rect, INT wID)
{
	CWnd* pWnd = NULL;

	if (pContext != NULL)
	{
		if (pContext->m_pNewViewClass != NULL)
		{
			pWnd = (CWnd*)pContext->m_pNewViewClass->CreateObject();
			if (pWnd == NULL)
			{
				TRACE1("Error: Dynamic create of view %Fs failed\n", pContext->m_pNewViewClass->m_lpszClassName);
				return NULL;
			}
			ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CWnd)));

			if (!pWnd->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, rect, pParent, wID, pContext))
			{
				TRACE0("Error: couldn't create view \n");
				return NULL;
			}
			// send initial notification message
			pWnd->SendMessage(WM_INITIALUPDATE);
		}
	}
	return pWnd;
}

VOID CTocViewerDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	CDialog::OnDestroy();
//	if (m_pTocView)
//		m_pTocView->DestroyWindow();
		// PostNcDestroy() delete the pointer
}

VOID CTocViewerDlg::SetTocViewer(CString& sText, BOOL bIsMokcha/*=TRUE*/, BOOL bIsDan/*=TRUE*/, CString sDelimiter/*=_T("\r\n")*/)
{
	m_sText= sText ;
	m_sDelimiter= sDelimiter ;
	m_bIsMokcha= bIsMokcha ;
	m_bIsDan= bIsDan ;
}

VOID CTocViewerDlg::SetChorokLine(UINT nLine)
{
	m_arrChorokLine.Add(nLine) ;
}

INT CTocViewerDlg::GetChorokFromLine(UINT nLine, CString &sChorok)
{
	sChorok.Empty() ;
	if( m_pParent==NULL )	return -1 ;

	return ((CTocEditorDlg*)m_pParent)->GetChorokFromLine(nLine, sChorok) ;
}

VOID CTocViewerDlg::SetViewer(CString sTitle, UINT nLine/*=0*/, BOOL bMove/*=FALSE*/)
{
	m_sTitle= sTitle ;
	m_nLine= nLine ;
	m_bMove= bMove ;
}

VOID CTocViewerDlg::OnPrevButton()
{
/*	(m_pTocView->m_arrChorokLine).RemoveAll() ;

	if ( m_bIsMokcha )	// 목차뷰 일 경우
	{
		((CTocEditorDlg*)m_pParent)->GetMoveRecord(m_bIsMokcha, FALSE, m_sText, &m_bPrev, &m_bNext) ;
		if ( !m_bIsDan )		// 연간물 목차
		{
			((CTocEditorDlg*)m_pParent)->GetChorokLine(&m_arrChorokLine) ;
		}
	}
	else				// 초록뷰 일 경우
	{
		if ( m_bIsDan )		// 단행본 초록
		{
			((CTocEditorDlg*)m_pParent)->GetMoveRecord(m_bIsMokcha, FALSE, m_sText, &m_bPrev, &m_bNext) ;
		}
		else				// 연간물 초록
		{
			CString sKisaNo, sKisa ;
			INT nLine= ((CTocEditorDlg*)m_pParent)->GetChoroknKisa(m_sText, sKisaNo, sKisa, m_nLine, TRUE, TRUE) ;
			if ( nLine<0 )	return ;
			m_nLine= nLine ;
		}
	}

	m_pTocView->SetTextOutString(m_sText, m_sDelimiter, m_sWorkDir, m_bIsMokcha, m_bIsDan) ;
	(m_pTocView->m_arrChorokLine).Copy(m_arrChorokLine);
	m_arrChorokLine.RemoveAll() ;
*/
	// 이전 다음 버튼 업데이트
	UpdateEnablePrevNext() ;
}

VOID CTocViewerDlg::OnNextButton()
{
/*	(m_pTocView->m_arrChorokLine).RemoveAll() ;

	if ( m_bIsMokcha )	// 목차뷰 일 경우
	{
		((CTocEditorDlg*)m_pParent)->GetMoveRecord(m_bIsMokcha, TRUE, m_sText, &m_bPrev, &m_bNext) ;
		if ( !m_bIsDan )		// 연간물 목차
		{
			((CTocEditorDlg*)m_pParent)->GetChorokLine(&m_arrChorokLine) ;
		}
	}
	else				// 초록뷰 일 경우
	{
		if ( m_bIsDan )		// 단행본 초록
		{
			((CTocEditorDlg*)m_pParent)->GetMoveRecord(m_bIsMokcha, TRUE, m_sText, &m_bPrev, &m_bNext) ;
		}
		else				// 연간물 초록
		{
			CString sKisaNo, sKisa ;
			INT nLine= ((CTocEditorDlg*)m_pParent)->GetChoroknKisa(m_sText, sKisaNo, sKisa, m_nLine, FALSE, TRUE) ;
			if ( nLine<0 )	return ;
			m_nLine= nLine ;
		}
	}

	m_pTocView->SetTextOutString(m_sText, m_sDelimiter, m_sWorkDir, m_bIsMokcha, m_bIsDan) ;
	(m_pTocView->m_arrChorokLine).Copy(m_arrChorokLine);
	m_arrChorokLine.RemoveAll() ;
*/
	// 이전 다음 버튼 업데이트
	UpdateEnablePrevNext() ;
}

VOID CTocViewerDlg::EnablePrevNext(BOOL bPrev, BOOL bNext)
{
	m_bPrev= bPrev ;
	m_bNext= bNext ;
}

VOID CTocViewerDlg::UpdateEnablePrevNext()
{
	if ( !m_bIsDan )
	{
		if ( m_bIsMokcha )	// 연간물 목차
		{
		}
		else				// 연간물 초록
		{
			m_bPrev= ((CTocEditorDlg*)m_pParent)->KisaExistPrevnNext(m_nLine, TRUE, TRUE) ;
			m_bNext= ((CTocEditorDlg*)m_pParent)->KisaExistPrevnNext(m_nLine, FALSE, TRUE) ;

		}
	}

	GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(m_bPrev);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(m_bNext);
}

HBRUSH CTocViewerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG) 
	{
		pDC->SetTextColor(RGB(0, 0, 0));		
		pDC->SetBkMode(OPAQUE);		
		pDC->SetBkColor(RGB(245, 245, 245));		
		
		return m_DlgBrush;		
	}

	return hbr;
}