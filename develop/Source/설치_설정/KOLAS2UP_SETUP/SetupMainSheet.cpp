// SetupMainSheet.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS2UP_SETUP.h"
#include "SetupMainSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupMainSheet

IMPLEMENT_DYNAMIC(CSetupMainSheet, CPropertySheet)

CSetupMainSheet::CSetupMainSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_nIDCaption = nIDCaption;
	m_GridSetupPage.m_psp.dwFlags &= ~PSP_HASHELP;
	m_TnsSetupPage.m_psp.dwFlags &= ~PSP_HASHELP;
	m_ConnectSetupPage.m_psp.dwFlags &= ~PSP_HASHELP;
	m_EtcSetupPage.m_psp.dwFlags &= ~PSP_HASHELP;
    
	//2003.12.16 이학중 추가 
	m_VerInfoPage.m_psp.dwFlags &=  ~PSP_HASHELP;

	m_UserInfoPage.m_psp.dwFlags &=  ~PSP_HASHELP;
	
	m_DaemonSetupPage.m_psp.dwFlags &= ~PSP_HASHELP;

	AddPage(&m_TnsSetupPage);
	AddPage(&m_ConnectSetupPage);

	if (m_nIDCaption == IDS_SETUP)
	{
		AddPage(&m_GridSetupPage);
	}
	AddPage(&m_EtcSetupPage);

	//2003.12.16 이학중 추가 
    AddPage(&m_VerInfoPage) ;


//  2012.11.09 DEL BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2차
//	사용자 정보변경 기능 제거
//	AddPage(&m_UserInfoPage);

	CFileFind ff;
	if(ff.FindFile(_T("..\\cfg\\_openbase")) || ff.FindFile(_T("..\\cfg\\_KalendarD")))
	{
		AddPage(&m_DaemonSetupPage);
	}	
}

CSetupMainSheet::CSetupMainSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CSetupMainSheet::~CSetupMainSheet()
{
}


BEGIN_MESSAGE_MAP(CSetupMainSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CSetupMainSheet)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_COMMAND(1002, OnInitVersionInfo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupMainSheet Operations
INT CSetupMainSheet::OnInitVersionInfo()
{
	// 초기화 --> 버전 파일을 지운다.

	LPCTSTR lpFileName = NULL;

	// 메인 레퍼런스 버전 초기화
	lpFileName = _T("..\\CFG\\Reference\\Version.cfg");
	FILE *fd = _tfopen(lpFileName, _T("wb"));
	
		/*UNCHANGE*/fputc(0xFF, fd);
		/*UNCHANGE*/fputc(0xFE, fd);
	// KOL.UDF.022
	if (&fd != NULL) {
		_ftprintf(fd, _T("ESL_REFERENCE=0\r\n"));
	}
	
	// 마크 정보 버전 초기화
	lpFileName = _T("..\\cfg\\MarcRelation\\Version.cfg");
	DeleteFile(lpFileName);
	
	// 캐릭터셋 버전 초기화
	lpFileName = _T("..\\cfg\\CharSetVersion.cfg");
	DeleteFile(lpFileName);	

	// 관리 버전 초기화
	lpFileName = _T("..\\cfg\\ManagementVersion.cfg");
	DeleteFile(lpFileName);	


	MessageBox(_T("초기화 완료"), NULL, MB_ICONINFORMATION);

	fclose(fd);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CSetupMainSheet message handlers
INT CSetupMainSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

 	CRect rcClient;
 	GetClientRect(rcClient);
 	rcClient.bottom += 135;
 	rcClient.top = rcClient.bottom - 25;
 	rcClient.left += 15;
 	rcClient.right = rcClient.left + 90;
 	m_ButtonInitVersion.Create(_T("버전 초기화"), /*WS_VISIBLE | */WS_CHILD, rcClient, this, 1002);

//	if (m_nIDCaption == IDS_PRE_SETUP) m_ButtonInitVersion.ShowWindow(SW_HIDE);
		
	return 0;
}

BOOL CSetupMainSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	SetIcon(::AfxGetApp()->LoadIcon(IDR_MAINFRAME), FALSE);

	m_ButtonInitVersion.SetFont(GetFont());

	CString strVersion, strTitle;
	strVersion = _T("[v1.1]");
	strTitle.Format(_T("KOLAS III 환경설정 %s"), strVersion);
	SetWindowText(strTitle);

	CRect rRect1;
	CRect rRect2;
	GetDlgItem(IDCANCEL)->GetWindowRect(&rRect1);
	GetDlgItem(ID_APPLY_NOW)->GetWindowRect(&rRect2);
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);

	ScreenToClient(rRect1);
	ScreenToClient(rRect2);
	GetDlgItem(IDOK)->MoveWindow(rRect2);
	GetDlgItem(ID_APPLY_NOW)->MoveWindow(rRect1);
	GetDlgItem(IDOK)->SetWindowText(_T("닫기"));


	return bResult;
}

BOOL CSetupMainSheet::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->lParam == VK_RETURN && pMsg->message == WM_KEYDOWN )
	{		
		return TRUE;
	}
	
	return CPropertySheet::PreTranslateMessage(pMsg);
}
