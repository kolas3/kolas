// BO_ACQ_MANAGE.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_MANAGE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_MANAGE dialog


CBO_ACQ_MANAGE::CBO_ACQ_MANAGE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_MANAGE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_MANAGE)
	m_MissingProc_UseYN = -1;
	m_OrderMail_AutoSendYN = -1;
	m_RequestMail_AutoSendYN = -1;
	m_Title_AutoSearchYN = -1;
	//}}AFX_DATA_INIT
}


VOID CBO_ACQ_MANAGE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_MANAGE)
//	DDX_Radio(pDX, IDC_radMISSPROC_USE, m_MissingProc_UseYN);
	DDX_Radio(pDX, IDC_radORDERMAIL_AUTO_SEND, m_OrderMail_AutoSendYN);
	DDX_Radio(pDX, IDC_radREQUESTMAIL_AUTO_SEND, m_RequestMail_AutoSendYN);
	DDX_Radio(pDX, IDC_radTITLE_AUTO_SEARCH, m_Title_AutoSearchYN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_MANAGE, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_MANAGE)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_radTITLE_AUTO_SEARCH, OnradTITLEAUTOSEARCH)
	ON_BN_CLICKED(IDC_radTITLE_HAND_SEARCH, OnradTITLEHANDSEARCH)
//	ON_BN_CLICKED(IDC_radMISSPROC_USE, OnradMISSPROCUSE)
//	ON_BN_CLICKED(IDC_radMISSPROC_NONUSE, OnradMISSPROCNONUSE)
	ON_BN_CLICKED(IDC_radORDERMAIL_AUTO_SEND, OnradORDERMAILAUTOSEND)
	ON_BN_CLICKED(IDC_radORDERMAIL_HAND_SEND, OnradORDERMAILHANDSEND)
	ON_BN_CLICKED(IDC_radREQUESTMAIL_AUTO_SEND, OnradREQUESTMAILAUTOSEND)
	ON_BN_CLICKED(IDC_radREQUESTMAIL_HAND_SEND, OnradREQUESTMAILHANDSEND)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_MANAGE message handlers

VOID CBO_ACQ_MANAGE::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CBO_ACQ_MANAGE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UP_인계확인")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	InitAcqManageValue();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CBO_ACQ_MANAGE::InitAcqManageValue()
{
	CString value;
	this->GetManageValue(_T("수서"), _T("단행"), _T("수서서명자동복본조사"), _T(""), value);
	value.MakeUpper();
	if(value == _T("Y")) m_Title_AutoSearchYN = 0;
	else m_Title_AutoSearchYN = 1;
	this->GetManageValue(_T("수서"), _T("단행"), _T("미납처리사용여부"), _T(""), value);
	value.MakeUpper();
	if(value == _T("Y")) m_MissingProc_UseYN = 0;
	else m_MissingProc_UseYN = 1;
	this->GetManageValue(_T("수서"), _T("단행"), _T("주문메일자동발송"), _T(""), value);
	value.MakeUpper();
	if(value == _T("Y")) m_OrderMail_AutoSendYN = 0;
	else m_OrderMail_AutoSendYN = 1;
	this->GetManageValue(_T("수서"), _T("단행"), _T("비치희망자료알림메일자동발송"), _T(""), value);
	value.MakeUpper();
	if(value == _T("Y")) m_RequestMail_AutoSendYN = 0;
	else m_RequestMail_AutoSendYN = 1;
}

VOID CBO_ACQ_MANAGE::OnradTITLEAUTOSEARCH() 
{
	// TODO: Add your control notification handler code here
	m_Title_AutoSearchYN = 0;
	UpdateData(FALSE);
}

VOID CBO_ACQ_MANAGE::OnradTITLEHANDSEARCH() 
{
	// TODO: Add your control notification handler code here
	m_Title_AutoSearchYN = 1;
	UpdateData(FALSE);
}

VOID CBO_ACQ_MANAGE::OnradMISSPROCUSE() 
{
	// TODO: Add your control notification handler code here
	m_MissingProc_UseYN = 0;
	UpdateData(FALSE);
}

VOID CBO_ACQ_MANAGE::OnradMISSPROCNONUSE() 
{
	// TODO: Add your control notification handler code here
	m_MissingProc_UseYN = 1;
	UpdateData(FALSE);
}

VOID CBO_ACQ_MANAGE::OnradORDERMAILAUTOSEND() 
{
	// TODO: Add your control notification handler code here
	m_OrderMail_AutoSendYN = 0;
	UpdateData(FALSE);
}

VOID CBO_ACQ_MANAGE::OnradORDERMAILHANDSEND() 
{
	// TODO: Add your control notification handler code here
	m_OrderMail_AutoSendYN = 1;
	UpdateData(FALSE);
}

VOID CBO_ACQ_MANAGE::OnradREQUESTMAILAUTOSEND() 
{
	// TODO: Add your control notification handler code here
	m_RequestMail_AutoSendYN = 0;
	UpdateData(FALSE);
}

VOID CBO_ACQ_MANAGE::OnradREQUESTMAILHANDSEND() 
{
	// TODO: Add your control notification handler code here
	m_RequestMail_AutoSendYN = 1;
	UpdateData(FALSE);
}

VOID CBO_ACQ_MANAGE::OnOK() 
{
	// TODO: Add extra validation here
	ApplyAcqManageValue();
	ESLAfxMessageBox(_T("설정사항을 저장하였습니다."));
	//CDialog::OnOK();
}

VOID CBO_ACQ_MANAGE::ApplyAcqManageValue()
{
	CString value;
	if(m_Title_AutoSearchYN == 0) value = _T("Y");
	else value = _T("N");
	this->SetManageValue(_T("수서"), _T("단행"), _T("수서서명자동복본조사"), _T(""), value);
	if(m_MissingProc_UseYN == 0) value = _T("Y");
	else value = _T("N");
	this->SetManageValue(_T("수서"), _T("단행"), _T("미납처리사용여부"), _T(""), value);
	if(m_OrderMail_AutoSendYN == 0) value = _T("Y");
	else value = _T("N");	
	this->SetManageValue(_T("수서"), _T("단행"), _T("주문메일자동발송"), _T(""), value);
	if(m_OrderMail_AutoSendYN == 0) value = _T("Y");
	else value = _T("N");
	this->SetManageValue(_T("수서"), _T("단행"), _T("비치희망자료알림메일자동발송"), _T(""), value);
}

VOID CBO_ACQ_MANAGE::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}

BOOL CBO_ACQ_MANAGE::Create(CWnd *pWnd)
{
	return CDialog::Create(IDD, pWnd);
}
