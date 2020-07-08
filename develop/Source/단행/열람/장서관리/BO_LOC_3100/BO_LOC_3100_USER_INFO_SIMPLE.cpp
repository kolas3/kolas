// BO_LOC_3100_USER_INFO_SIMPLE.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_31001.h"
#include "BO_LOC_3100_USER_INFO_SIMPLE.h"
#include "Imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBO_LOC_3100_USER_INFO_SIMPLE::CBO_LOC_3100_USER_INFO_SIMPLE(CESL_Mgr* pParent ,CBL_LOC_LOAN_PROC* pLoan)
	: CESL_Mgr(CBO_LOC_3100_USER_INFO_SIMPLE::IDD, pParent)
{

	m_pRemarkDlg	=	NULL;
	m_pLoan			=	pLoan;
	m_pParent		=	pParent;
}

CBO_LOC_3100_USER_INFO_SIMPLE::~CBO_LOC_3100_USER_INFO_SIMPLE()
{
	m_Brush.DeleteObject();
}

void CBO_LOC_3100_USER_INFO_SIMPLE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBO_LOC_3100_USER_INFO_SIMPLE, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3100_USER_INFO_SIMPLE)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_NOTE, OnSetfocusEditNote)
	ON_EN_CHANGE(IDC_EDIT_NOTE, OnChangeEditNote)
	ON_BN_CLICKED(IDC_BTN_NOTE_MODIFY, OnBtnNoteModify)
	ON_BN_CLICKED(IDC_BTN_LONE_STOP_CANCEL, OnBtnLoneStopCancel)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_REMARK_UPDATE, OnBtnNoteModify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3100_USER_INFO_SIMPLE message handlers

BOOL CBO_LOC_3100_USER_INFO_SIMPLE::OnInitDialog() 
{
	CDialog::OnInitDialog();

	INT	ids	=	InitESL_Mgr(_T("BO_LOC_3100_USER_INFO_SIMPLE"));
	if( ids < 0 )
	{
		AfxMessageBox( _T("Init Refrence Error") );
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	m_Brush.CreateSolidBrush(RGB(255,0,0));

	m_pRemarkDlg = new CRemarkDlg(this);
	m_pRemarkDlg->Create(this);
	m_pRemarkDlg->pMain = m_pParent->pMain;
	m_pRemarkDlg->pMain->SendMessage(WM_USER+1028, (WPARAM)m_pRemarkDlg, 0);

	((CEdit*)GetDlgItem(IDC_EDIT_NOTE))->SetLimitText(200);

	if( m_pLoan->m_pManageValue->m_sFeeMngSys == _T("Y") )
	{
		GetDlgItem(IDC_BTN_LONE_STOP_CANCEL)->ShowWindow(SW_SHOW);
	}

	if(m_pLoan->m_pManageValue->m_sUnityLoanCntYN == _T("Y"))
	{
		GetDlgItem(IDC_STATIC_ILL_LONE_COUNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ILL_LOAN_COUNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_UNITY_LONE_COUNT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_UNITY_LOAN_COUNT)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_STATIC_ILL_LONE_COUNT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_ILL_LOAN_COUNT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_UNITY_LONE_COUNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_UNITY_LOAN_COUNT)->ShowWindow(SW_HIDE);
	}
	
	return TRUE;
}

HBRUSH CBO_LOC_3100_USER_INFO_SIMPLE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if(nCtlColor == CTLCOLOR_STATIC && IDC_EDIT_USER_STATE == pWnd->GetDlgCtrlID())
	{
		CString strText;
		pWnd->GetWindowText(strText);
		if(_T("대출정지") == strText)
		{
			pDC->SetTextColor(RGB(255,255,255));
			pDC->SetBkColor(RGB(255,0,0));
			return (HBRUSH) m_Brush;
		}
	}
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}

void CBO_LOC_3100_USER_INFO_SIMPLE::OnSetfocusEditNote() 
{
EFS_BEGIN

	CEdit *pEdit1;
	pEdit1 = ( CEdit*) GetDlgItem (IDC_EDIT_NOTE);
		
	HIMC hIME;
	DWORD dwConversion, dwSentence;
	
	hIME = ::ImmGetContext(pEdit1->m_hWnd);
	if(hIME) 
	{
		ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;
		
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
		
		ImmReleaseContext(pEdit1->m_hWnd,hIME); 
	} 

EFS_END		
}
void CBO_LOC_3100_USER_INFO_SIMPLE::OnChangeEditNote() 
{
	UpdateData(TRUE);
	CString strText;
	GetDlgItemText( IDC_EDIT_NOTE, strText );
	m_pRemarkDlg->SetRemarkText( strText );	
	UpdateData(FALSE);
}

BOOL CBO_LOC_3100_USER_INFO_SIMPLE::PreTranslateMessage(MSG* pMsg) 
{
	if( WM_LBUTTONDBLCLK == pMsg->message )
	{
		if( GetDlgItem( IDC_EDIT_NOTE )->GetSafeHwnd() == pMsg->hwnd )
		{	
			m_pRemarkDlg->ShowWindow(SW_SHOWNOACTIVATE);
			SetRemarkControl(FALSE);
			return TRUE;
		}			
	}

	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_ESCAPE )
		{
			GetParent()->PostMessage(WM_KEYDOWN, VK_ESCAPE);
			return TRUE;
		}
		else if ( pMsg->wParam == VK_RETURN )
		{
			CWnd* pWnd = GetDlgItem( IDC_EDIT_NOTE );
			if(NULL == pWnd || pWnd->GetSafeHwnd() != pMsg->hwnd)
			{
				return TRUE;
			}
		}
	}	

	return CDialog::PreTranslateMessage(pMsg);
}

INT CBO_LOC_3100_USER_INFO_SIMPLE::SetRemarkControl(BOOL bShow)
{
EFS_BEGIN

	const int nCtrlCount = 3;
	INT	nId[nCtrlCount] = {IDC_STATIC_REMARK, IDC_EDIT_NOTE, IDC_BTN_NOTE_MODIFY};		
			
	for(INT i=0;i<nCtrlCount;i++ )
	{
		if(bShow)
		{
			GetDlgItem(nId[i])->ShowWindow( SW_SHOWNOACTIVATE );
		}
		else
		{
			GetDlgItem(nId[i])->ShowWindow( bShow );
		}
	}
	
	if( bShow )
	{
		SetDlgItemText(IDC_EDIT_NOTE, m_pRemarkDlg->GetRemarkText());
	}
	else
	{
		CString strText;
		GetDlgItemText(IDC_EDIT_NOTE,strText);
		m_pRemarkDlg->SetRemarkText(strText);
	}

	return 0;
	
EFS_END
	return -1;
}

void CBO_LOC_3100_USER_INFO_SIMPLE::OnBtnNoteModify() 
{
EFS_BEGIN

	CString strNote;

	if( m_pRemarkDlg->IsWindowVisible() )
	{
		strNote = m_pRemarkDlg->GetRemarkText();
		SetDlgItemText(IDC_EDIT_NOTE, strNote);
		SetControlData(_T("CM_BO_LOC_3100_MEMBER_INFO"), _T("비고"), strNote);		
	}
	else
	{
		INT	nError	=	GetDlgItemText(IDC_EDIT_NOTE,strNote);
		if( 0	<=	nError )
		{ 
			SetControlData(_T("CM_BO_LOC_3100_MEMBER_INFO"), _T("비고"), strNote);
		}
	}


	INT ids;
	ids = m_pLoan->SPFExcutePath( _T("이용자_비고수정") );
	
	if(	NULL !=	pParentMgr)
	{
		CWnd* pWnd = (pParentMgr->GetDlgItem(IDC_edtNUMBER));			

		if(NULL != pWnd && NULL != pWnd->GetSafeHwnd())
			pWnd->SetFocus();
	}

EFS_END
}

void CBO_LOC_3100_USER_INFO_SIMPLE::OnBtnLoneStopCancel() 
{
EFS_BEGIN

	INT ids;
	ids = m_pLoan->SPFExcutePath( _T("대출정지일해제") );

EFS_END	
}

void CBO_LOC_3100_USER_INFO_SIMPLE::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);

}

void CBO_LOC_3100_USER_INFO_SIMPLE::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);	
	
}
