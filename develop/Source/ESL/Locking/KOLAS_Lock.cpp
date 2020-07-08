// KOLAS_Lock.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS_Lock.h"
#include "ESL_DataMgr.h"

// 2012.12.03 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 6회
// 비밀번호 암호화
#include "..\\..\\단행\\열람\\장서관리\\LOC_CommonAPI\\LocCommonAPI.h"
// << 2015년 공공도서관 시스템 개선 2015/11/11 _LCM : 자동로그아웃기능
#include "..\\..\\공통\\GPKI\\GPKILoginDlg.h"
// >> 2015/11/11 --ADD
	
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKOLAS_Lock dialog


CKOLAS_Lock::CKOLAS_Lock(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKOLAS_Lock::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKOLAS_Lock)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bmpLogin.LoadBMP(_T("..\\bmp\\Main_LOGIN.bmp"));
	CBitmap bitmap;	
	bitmap.LoadBitmap(IDB_LOCK_BG);	
	m_brush.CreatePatternBrush(&bitmap);
	
	// << 2015년 공공도서관 시스템 개선 2015/11/27 _LCM : GPKI 로그인 선택
	m_bGPKI = FALSE;
	// KOL.UDF.022
	m_loginX = 0;
	m_loginy = 0;
}

BOOL CKOLAS_Lock::Create(CWnd* pParentWnd)
{	
	
	return CDialog::Create(IDD, pParentWnd); 
}	

CKOLAS_Lock::~CKOLAS_Lock()
{
	DeleteObject(m_brush);		
}

void CKOLAS_Lock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKOLAS_Lock)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKOLAS_Lock, CDialog)
	//{{AFX_MSG_MAP(CKOLAS_Lock)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()	
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
// << 2015년 공공도서관 시스템 개선 2015/11/11 _LCM : 자동로그아웃기능
	ON_BN_CLICKED(IDC_BUTTON_GPKI_LOGIN, OnButtonGpkiLogin)
// >> 2015/11/11 --ADD
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKOLAS_Lock message handlers

void CKOLAS_Lock::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);			
}


BOOL CKOLAS_Lock::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect;
	GetClientRect(&rect);	
	
	pDC->FillRect(&rect, &m_brush);

	return TRUE;
	
	
	//return CDialog::OnEraseBkgnd(pDC);
}

void CKOLAS_Lock::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	
	m_loginX = (rect.right-rect.left)/2 - 157;
	m_loginy = (rect.bottom-rect.top)/2 - 109;
	dc.SetBkMode(TRANSPARENT);
	m_bmpLogin.DrawDIB(&dc, m_loginX, m_loginy );
	ResizeControl();
}


VOID CKOLAS_Lock::ResizeControl()
{
	CEdit *pEdit;
	pEdit = (CEdit*) GetDlgItem(IDC_eID);
	if ( pEdit->GetSafeHwnd() == NULL ) return;

	CRect rect;
	pEdit->GetClientRect(rect);
	ClientToScreen(rect);
	rect.left = m_loginX+140;
	rect.top = m_loginy+ 87;
	rect.right = m_loginX +252;
	rect.bottom = m_loginy + 108;			
	pEdit->MoveWindow(rect);
	pEdit = (CEdit*) GetDlgItem(IDC_ePASSWORD);	
	rect.top += 40;
	rect.bottom += 40; 
	pEdit->MoveWindow(rect);

	// << 2015년 공공도서관 시스템 개선 2015/11/11 _LCM : GPKI 로그인 이미지 변경	
	CRect rectTmp;
	CButton* pBtn = (CButton*)GetDlgItem(IDC_BUTTON_GPKI_LOGIN);
	pBtn->GetClientRect(rectTmp);
	rect.top -= 100;
	rect.left += 70;
	//rect.bottom -= 100;
	rect.right = rect.left + rectTmp.Width();
	rect.bottom = rect.top + rectTmp.Height();
	pBtn->MoveWindow(rect);

	HBITMAP hBmp = 0;
	BITMAP bitmap;
	hBmp = (HBITMAP)LoadImage(0, _T("..\\bmp\\btn_gpki1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(hBmp, sizeof(BITMAP), &bitmap);
	pBtn->SetBitmap(hBmp);
	// >> 2015/11/11 _LCM --MOD

	CStatic* pRSC = (CStatic*) GetDlgItem (IDC_IMGRSC);	
	pRSC->GetClientRect(rect);
	ClientToScreen(rect);
	rect.right = m_loginX+rect.Width()+5;
	rect.bottom = m_loginy+rect.Height()+50;
	rect.left = m_loginX+5;
	rect.top = m_loginy+50;	
	pRSC->MoveWindow(rect);
	return;
}

// << 2015년 공공도서관 시스템 개선 2015/11/27 _LCM : GPKI 활성화 비활성화
void CKOLAS_Lock::OnVisibleGPKI()
{
	CESL_DataMgr dataMgr;
	CString strQuery;	
	CString strValue;
		
	dataMgr.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	
	
	strQuery.Format(_T("SELECT GPKI_USE FROM MN_USER_TBL WHERE ID='%s'"), m_pInfo->USER_ID);
	dataMgr.GetOneValueQuery(strQuery, strValue);

	if(strValue.Compare(_T("Y"))==0)
	{
		((CButton*)GetDlgItem(IDC_ePASSWORD))->EnableWindow(FALSE);		
	}	
	else
	{
		((CButton*)GetDlgItem(IDC_BUTTON_GPKI_LOGIN))->ShowWindow(SW_HIDE);
	}
}

// >> 2015/11/27 _LCM --ADD

BOOL CKOLAS_Lock::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	GetDlgItem(IDC_eID)->SetWindowText(m_pInfo->USER_ID);
	GetDlgItem(IDC_ePASSWORD)->SetWindowText(_T(""));

	this->ShowWindow(SW_HIDE);

	OnVisibleGPKI();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKOLAS_Lock::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_SHOWWINDOW )
	{
		( (CEdit*) GetDlgItem(IDC_ePASSWORD) )->SetFocus();
	}
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_ESCAPE ) return TRUE;

		if ( pMsg->wParam == VK_RETURN )
		{
			m_bGPKI = FALSE;
			UnLock();				
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CKOLAS_Lock::UnLock()
{
	CESL_DataMgr m_DataMgr;
	CString strQuery;
	CString strPassword;
	CString strValue;
	
	GetDlgItem(IDC_ePASSWORD)->GetWindowText(strPassword);	
	m_DataMgr.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		
	// 2012.12.03 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 6회
	// 잠금화면 비밀번호 암호화 적용
	if(m_pInfo->m_bUserPwEncryption && !m_bGPKI)
	{
		CString strCryptoPassword;
		strCryptoPassword = CLocCommonAPI::StringToSHA256(strPassword);
		strPassword = strCryptoPassword;
	}
	
	strQuery.Format(_T("SELECT COUNT(1) FROM MN_USER_TBL WHERE ID='%s' AND PASSWD='%s'"), m_pInfo->USER_ID, strPassword);
	m_DataMgr.GetOneValueQuery(strQuery, strValue);

	if ( _ttoi(strValue) > 0 )
	{	
		GetDlgItem(IDC_ePASSWORD)->SetWindowText(_T(""));		
		GetParent()->SendMessage(WM_USER + 99, NULL, NULL);		
		return;
	}

	AfxMessageBox(_T("아이디 또는 암호가 잘못되었습니다. 아이디와 암호를 확인하십시오."));

}

void CKOLAS_Lock::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	( (CEdit*) GetDlgItem(IDC_ePASSWORD) )->SetFocus();
}

// << 2015년 공공도서관 시스템 개선 2015/11/11 _LCM : GPKI 인증서 로그인
void CKOLAS_Lock::OnButtonGpkiLogin() 
{
	// TODO: Add your control notification handler code here

	m_bGPKI = TRUE;

	CGPKILoginDlg dlg;

	if(dlg.DoModal() == IDOK)
	{				
		CString strUser = _T("");
		GetDlgItem(IDC_eID)->GetWindowText(strUser);

		if(strUser.Compare(dlg.m_strLoginInfo[0]) == 0)
		{
			GetDlgItem(IDC_ePASSWORD)->SetWindowText(dlg.m_strLoginInfo[1]);
			UnLock();
		}
		else
		{
			AfxMessageBox(_T("현재 로그인 사용자와 다른 사용자입니다."),MB_OK);
		}
	}	
}
// >> 2015/11/11 --ADD
