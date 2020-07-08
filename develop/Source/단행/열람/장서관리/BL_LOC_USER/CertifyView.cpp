#include "Stdafx.h"
#include "CertifyView.h"

#include "CertifyMainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CERTIFY_INFO_GET_FUNC(_CLASS, _NAME, _TYPE)	\
	_TYPE _CLASS::Get##_NAME()	\
	{\
		return g_pMainDlg->Get##_NAME();\
	}
#define CERTIFY_INFO_SET_FUNC(_CLASS, _NAME, _TYPE)	\
	VOID  _CLASS::Set##_NAME(_TYPE _NAME) \
	{\
		g_pMainDlg->Set##_NAME(_NAME);\
	}

CCertifyMainDlg* g_pMainDlg;

/////////////////////////////////////////////////////////////////////////////
// CCertifyView dialog

CCertifyView::CCertifyView(UINT IDD, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	g_pMainDlg = (CCertifyMainDlg*)pParent;
}

CCertifyView::~CCertifyView()
{
}

BEGIN_MESSAGE_MAP(CCertifyView, CDialog)
	//{{AFX_MSG_MAP(CCertifyView)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCertifyView message handlers

BOOL CCertifyView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			return GetParent()->PreTranslateMessage(pMsg);
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CCertifyView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if ( IsColorControl(pWnd->GetDlgCtrlID()) )
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(RGB(m_bkR, m_bkG, m_bkB));
		return m_BkBrush;
	}

	return hbr;
}

//{{
CERTIFY_INFO_GET_FUNC(CCertifyView, UserMode, INT)
CERTIFY_INFO_SET_FUNC(CCertifyView, UserMode, INT)

CERTIFY_INFO_GET_FUNC(CCertifyView, UserKey, CString)
CERTIFY_INFO_SET_FUNC(CCertifyView, UserKey, CString)

CERTIFY_INFO_GET_FUNC(CCertifyView, UserNo, CString)
CERTIFY_INFO_SET_FUNC(CCertifyView, UserNo, CString)

CERTIFY_INFO_GET_FUNC(CCertifyView, UserName, CString)
CERTIFY_INFO_SET_FUNC(CCertifyView, UserName, CString)

CERTIFY_INFO_GET_FUNC(CCertifyView, BirthDay, CString)
CERTIFY_INFO_SET_FUNC(CCertifyView, BirthDay, CString)

CERTIFY_INFO_GET_FUNC(CCertifyView, HandPhone, CString)
CERTIFY_INFO_SET_FUNC(CCertifyView, HandPhone, CString)

CERTIFY_INFO_GET_FUNC(CCertifyView, KLMemberYN, BOOL)
CERTIFY_INFO_SET_FUNC(CCertifyView, KLMemberYN, BOOL)
	
CERTIFY_INFO_GET_FUNC(CCertifyView, CertifyCI, CString)
CERTIFY_INFO_SET_FUNC(CCertifyView, CertifyCI, CString)

CERTIFY_INFO_GET_FUNC(CCertifyView, CertifyGender, CString)
CERTIFY_INFO_SET_FUNC(CCertifyView, CertifyGender, CString)

CERTIFY_INFO_GET_FUNC(CCertifyView, CertifyFailCode, CString)
CERTIFY_INFO_SET_FUNC(CCertifyView, CertifyFailCode, CString)

CERTIFY_INFO_GET_FUNC(CCertifyView, CertifyDate, CString)
CERTIFY_INFO_SET_FUNC(CCertifyView, CertifyDate, CString)

CERTIFY_INFO_GET_FUNC(CCertifyView, CertifyWorker, CString)
CERTIFY_INFO_SET_FUNC(CCertifyView, CertifyWorker, CString)
//}}
