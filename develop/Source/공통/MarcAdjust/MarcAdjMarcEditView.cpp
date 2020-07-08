// MarcAdjMarcEditView.cpp : implementation file
//

#include "stdafx.h"
#include "MarcAdjMarcEditView.h"
#include "MarcAdjFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjMarcEditView

IMPLEMENT_DYNCREATE(CMarcAdjMarcEditView, CFormView)

CMarcAdjMarcEditView::CMarcAdjMarcEditView()
	: CFormView(CMarcAdjMarcEditView::IDD)
{
	//{{AFX_DATA_INIT(CMarcAdjMarcEditView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarcAdjMgr = NULL;
	m_cRange.cpMax = 0;
	m_cRange.cpMin = 0;
	m_bAutoComplete = FALSE;

	m_crLink = NULL;
}

CMarcAdjMarcEditView::~CMarcAdjMarcEditView()
{
	CWinApp *pApp = AfxGetApp();	
	if ( m_bAutoComplete == TRUE ) pApp->WriteProfileString(_T("Settings"), _T("CatalogingAutoComplete"),_T("TRUE"));	
	else pApp->WriteProfileString(_T("Settings"), _T("CatalogingAutoComplete"),_T("FALSE"));	
}

VOID CMarcAdjMarcEditView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcAdjMarcEditView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarcAdjMarcEditView, CFormView)
	//{{AFX_MSG_MAP(CMarcAdjMarcEditView)
	ON_WM_SIZE()	
	ON_WM_MOVE()
	ON_NOTIFY( EN_LINK, IDC_eMARC_ADJ, OnURLLink )
	ON_BN_CLICKED(IDC_chkAUTOCOMPLETE, OnchkAUTOCOMPLETE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjMarcEditView diagnostics

#ifdef _DEBUG
VOID CMarcAdjMarcEditView::AssertValid() const
{
	CFormView::AssertValid();
}

VOID CMarcAdjMarcEditView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjMarcEditView Operations

VOID CMarcAdjMarcEditView::InitMarcEditCtrl()
{
	if (m_pMarcAdjMgr == NULL) return;
	CMarcAdjResourceMgr *pResourceMgr = m_pMarcAdjMgr->GetResourceMgr();
	pResourceMgr->m_MarcEditor.SubclassDlgItem(IDC_eMARC_ADJ, -1, this, GetParentFrame());
	pResourceMgr->m_MarcEditor.GetEditCtrl()->ModifyStyleEx(NULL, WS_VSCROLL);
	pResourceMgr->m_MarcEditor.SetLeaderEditBox(GetDlgItem(IDC_eLEADER_ADJ)->GetSafeHwnd());

	m_pMarcAdjMgr->InitMarcEditor();

	pResourceMgr->m_MarcEditor.Display();
	m_pMarcAdjMgr->BackupOldMarc();
	pResourceMgr->m_MarcEditor.GetEditCtrl()->SetEventMask( pResourceMgr->m_MarcEditor.GetEditCtrl()->GetEventMask() | ENM_LINK ) ; 

	CWinApp *pApp = AfxGetApp();
	CString strbAutoComplete;
	
	CMarcEditCtrl *pMarcEditCtrl = m_pMarcAdjMgr->GetResourceMgr()->m_MarcEditor.GetEditCtrl();
	strbAutoComplete = pApp->GetProfileString(_T("Settings"), _T("CatalogingAutoComplete"));		
	if ( strbAutoComplete == _T("") || strbAutoComplete == _T("TRUE")) 
	{
		pApp->WriteProfileString(_T("Settings"), _T("CatalogingAutoComplete"),_T("TRUE"));
		( (CButton*) GetDlgItem(IDC_chkAUTOCOMPLETE) )->SetCheck(TRUE);		
		m_bAutoComplete = TRUE;
		pMarcEditCtrl->m_bAutoComplete = m_bAutoComplete;
	}
	else
	{
		( (CButton*) GetDlgItem(IDC_chkAUTOCOMPLETE) )->SetCheck(FALSE);		
		m_bAutoComplete = FALSE;
		pMarcEditCtrl->m_bAutoComplete = m_bAutoComplete;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjMarcEditView message handlers

VOID CMarcAdjMarcEditView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT,CSize(1,1));

	m_pMarcAdjMgr = static_cast<CMarcAdjFrame*>(GetParentFrame())->GetMarcAdjMgr();

	InitMarcEditCtrl();	
}

VOID CMarcAdjMarcEditView::OnSize(UINT nType, INT cx, INT cy) 
{
	CFormView::OnSize(nType, cx, cy);

	CRect rcWnd;
	CWnd *pWnd = GetDlgItem(IDC_eLEADER_ADJ);
	CWnd *pWnd2 = GetDlgItem(IDC_chkAUTOCOMPLETE);
	if (pWnd->GetSafeHwnd())
	{
		pWnd->GetClientRect(rcWnd);
		pWnd->MoveWindow(0, 0, cx-80, rcWnd.Height());
		pWnd2->MoveWindow(cx-80, 0, cx, rcWnd.Height());
	}
	
	pWnd = GetDlgItem(IDC_eMARC_ADJ);
	if (pWnd->GetSafeHwnd())
	{
		pWnd->MoveWindow(0, rcWnd.Height(), cx, cy - rcWnd.Height());
	}	
}

LRESULT CMarcAdjMarcEditView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{	
	if (message == WM_COMMAND)
	{
		if (EN_KILLFOCUS == HIWORD(wParam))
		{
	
			if (IDC_eMARC_ADJ == LOWORD(wParam))
			{
				CMarcEditCtrl *pMarcEditCtrl = m_pMarcAdjMgr->GetResourceMgr()->m_MarcEditor.GetEditCtrl();
				pMarcEditCtrl->GetSel(m_cRange);
				
				if (pMarcEditCtrl->GetModify()) 
					pMarcEditCtrl->ApplyEdit();
			}	
		}
		if (EN_SETFOCUS == HIWORD(wParam))
		{
			if (IDC_eMARC_ADJ == LOWORD(wParam))
			{
				CMarcEditCtrl *pMarcEditCtrl = m_pMarcAdjMgr->GetResourceMgr()->m_MarcEditor.GetEditCtrl();
				pMarcEditCtrl->SetModify(FALSE);

				pMarcEditCtrl->SetSel(m_cRange);
			}
		}
	}	
	
	return CFormView::WindowProc(message, wParam, lParam);
}

void CMarcAdjMarcEditView::OnURLLink( 
			NMHDR*   in_pNotifyHeader, 
			LRESULT* out_pResult )
{
	ENLINK* l_pENLink = ( ENLINK* )in_pNotifyHeader ;

	*out_pResult = 0 ;

	switch( l_pENLink->msg )
	{
		default:
		{
		}
		break ;

		case WM_SETCURSOR:
		{
			if( m_crLink != NULL )
			{
			    ::SetCursor( m_crLink ) ;

				*out_pResult = 1 ;
			}
		}
		break ;

		case WM_LBUTTONDOWN:
		{
			CString l_URL ;
			CHARRANGE l_CharRange ;

			if (m_pMarcAdjMgr == NULL) return;
			CMarcAdjResourceMgr *pResourceMgr = m_pMarcAdjMgr->GetResourceMgr();			
			
			pResourceMgr->m_MarcEditor.GetEditCtrl()->GetSel( l_CharRange );
			pResourceMgr->m_MarcEditor.GetEditCtrl()->SetSel( l_pENLink->chrg );					
			pResourceMgr->m_MarcEditor.GetEditCtrl()->GetExSelText() ;
			pResourceMgr->m_MarcEditor.GetEditCtrl()->SetSel( l_CharRange ) ;

			CWaitCursor l_WaitCursor ;

			MessageBox( l_URL, _T("링크 클릭") );

			*out_pResult = 1 ;
		}
		break ;

		case WM_LBUTTONUP:
		{
			*out_pResult = 1 ;
		}
		break ;
	}
}

void CMarcAdjMarcEditView::OnMove(int x, int y) 
{
	CFormView::OnMove(x, y);
	
	// TODO: Add your message handler code here
	
}

void CMarcAdjMarcEditView::OnchkAUTOCOMPLETE() 
{
	m_bAutoComplete = ((CButton*)GetDlgItem(IDC_chkAUTOCOMPLETE))->GetCheck();		
	CMarcEditCtrl *pMarcEditCtrl = m_pMarcAdjMgr->GetResourceMgr()->m_MarcEditor.GetEditCtrl();
	pMarcEditCtrl->m_bAutoComplete = m_bAutoComplete;
}
