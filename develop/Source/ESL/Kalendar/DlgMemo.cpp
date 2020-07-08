// DlgMemo.cpp : implementation file
//

#include "stdafx.h"
#include "Kalendar.h"
#include "DlgMemo.h"
#include "DlgDay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FORENOON 0
#define AFTERNOON 1
/////////////////////////////////////////////////////////////////////////////
// CDlgMemo dialog


CDlgMemo::CDlgMemo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMemo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMemo)
	m_uAlarmNoon = FORENOON;	// 오전
	m_uAlarmHour = 1;
	m_uAlarmMinute = 0;
	m_bUseAlarm = FALSE;
//MACRODEL	m_pWndDay = NULL;
	//}}AFX_DATA_INIT
	// KOL.UDF.022
	m_uDay = 0;
	m_uMonth = 0;
	m_uYear = 0;
	m_stText = _T("");
	m_stTitle = _T("");
}


void CDlgMemo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMemo)
	DDX_Control(pDX, ID_BUTTON_MEMO_OK, m_btnMemoOK);
	DDX_Control(pDX, IDC_BUTTON_ALARM, m_btnUseAlarm);
	DDX_Control(pDX, IDC_BUTTON_HOUR, m_btnHour);
	DDX_Control(pDX, IDC_BUTTON_MIN, m_btnMinute);
	DDX_Control(pDX, IDC_BUTTON_NOON, m_btnNoon);
	DDX_Control(pDX, IDC_RICHEDIT_MEMO_TEXT, m_RichEditText);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_StaticTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMemo, CDialog)
	//{{AFX_MSG_MAP(CDlgMemo)
	ON_BN_CLICKED(IDC_BUTTON_NOON, OnButtonNoon)
	ON_BN_CLICKED(IDC_BUTTON_HOUR, OnButtonHour)
	ON_BN_CLICKED(IDC_BUTTON_MIN, OnButtonMin)
	ON_COMMAND(ID_HOUR_1, OnHour1)
	ON_COMMAND(ID_HOUR_10, OnHour10)
	ON_COMMAND(ID_HOUR_11, OnHour11)
	ON_COMMAND(ID_HOUR_12, OnHour12)
	ON_COMMAND(ID_HOUR_2, OnHour2)
	ON_COMMAND(ID_HOUR_3, OnHour3)
	ON_COMMAND(ID_HOUR_4, OnHour4)
	ON_COMMAND(ID_HOUR_5, OnHour5)
	ON_COMMAND(ID_HOUR_6, OnHour6)
	ON_COMMAND(ID_HOUR_7, OnHour7)
	ON_COMMAND(ID_HOUR_8, OnHour8)
	ON_COMMAND(ID_HOUR_9, OnHour9)
	ON_COMMAND(ID_MIN_15, OnMin15)
	ON_COMMAND(ID_MIN_30, OnMin30)
	ON_COMMAND(ID_MIN_45, OnMin45)
	ON_COMMAND(ID_MIN_0, OnMin0)
	ON_COMMAND(ID_NOON_FORE, OnNoonFore)
	ON_COMMAND(ID_NOON_AFTER, OnNoonAfter)
	ON_BN_CLICKED(IDC_BUTTON_ALARM, OnButtonAlarm)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_AFTER_10_MIN, OnAfter10Min)
	ON_COMMAND(ID_AFTER_30_MIN, OnAfter30Min)
	ON_COMMAND(ID_AFTER_5_MIN, OnAfter5Min)
	ON_COMMAND(ID_RICHEDIT_COPY, OnRicheditCopy)
	ON_COMMAND(ID_RICHEDIT_PASTE, OnRicheditPaste)
	ON_COMMAND(ID_RICHEDIT_CLEAR, OnRicheditClear)
	ON_COMMAND(ID_RICHEDIT_SELECTALL, OnRicheditSelectall)
	ON_BN_CLICKED(ID_BUTTON_MEMO_OK, OnButtonMemoOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMemo message handlers

BOOL CDlgMemo::OnInitDialog() 
{
	LOG(_T("   : CDlgMemo::OnInitDialog"));
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here	
	InitBG();

	//////////////////////////////////////////////////////////////////////////
	{// 여기서부터 이미지 버튼 설정
	m_btnMemoOK.LoadBitmaps(IDB_BITMAP_BUTTON_CLOSE);

	m_btnUseAlarm.LoadBitmaps(IDB_BITMAP_BUTTON_XP_STYLE);
	m_btnUseAlarm.SetDrawText(TRUE);

	m_btnNoon.LoadBitmaps(IDB_BITMAP_BUTTON_XP_STYLE);
	m_btnHour.SetDrawText(TRUE);

	m_btnHour.LoadBitmaps(IDB_BITMAP_BUTTON_XP_STYLE);
	m_btnHour.SetDrawText(TRUE);

	m_btnMinute.LoadBitmaps(IDB_BITMAP_BUTTON_XP_STYLE);
	m_btnMinute.SetDrawText(TRUE);

	}// 여기까지 이미지 버튼 설정
	//////////////////////////////////////////////////////////////////////////


	LONG cxScreen = GetSystemMetrics(SM_CXSCREEN);
	LONG cyScreen = GetSystemMetrics(SM_CYSCREEN);
	CRect rect;
	GetWindowRect(&rect);
	int x = (cxScreen-rect.right)/2;
	int y = (cyScreen-rect.bottom)/2;
	MoveWindow(x,y, MEMOWIDTH, MEMOHEIGHT);	

	

	CRect rDlg;
	GetWindowRect(rDlg);
	ScreenToClient(rDlg);

	LPPOINT lpPoint = new CPoint[8];

	lpPoint[0] = CPoint(0, 2);
 	lpPoint[1] = CPoint(2, 0);
 	lpPoint[2] = CPoint(rDlg.right -2, 0);
 	lpPoint[3] = CPoint(rDlg.right, 2);
 	lpPoint[4] = CPoint(rDlg.right, rDlg.bottom -3);
 	lpPoint[5] = CPoint(rDlg.right-3, rDlg.bottom);
 	lpPoint[6] = CPoint(3, rDlg.bottom);
 	lpPoint[7] = CPoint(0, rDlg.bottom-3);

	CRgn Rgn;
 	Rgn.CreatePolygonRgn(lpPoint, 8, WINDING);
	delete [] lpPoint;

	SetWindowRgn(Rgn, TRUE);
	Rgn.DeleteObject();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMemo::OnCancel() 
{	
	CDialog::OnCancel();
}
void CDlgMemo::OnButtonNoon() 
{
	// TODO: Add your control notification handler code here
	CMenu  *sMenu,*pMenu;
	pMenu = new CMenu;
	pMenu->LoadMenu(IDR_MENU_NOON);	
	sMenu = pMenu->GetSubMenu(0);
	
	// 메뉴를 버튼왼쪽아래에 띄우기
	CRect rect;
	GetDlgItem(IDC_BUTTON_NOON)->GetWindowRect(&rect);
	sMenu->TrackPopupMenu(
		TPM_RIGHTBUTTON|TPM_LEFTALIGN, rect.left, rect.bottom, this);
	delete pMenu;
}

void CDlgMemo::OnButtonHour() 
{
	// TODO: Add your control notification handler code here
	CMenu  *sMenu,*pMenu;
	pMenu = new CMenu;
	pMenu->LoadMenu(IDR_MENU_HOUR);	
	sMenu = pMenu->GetSubMenu(0);
	
	// 메뉴를 버튼왼쪽아래에 띄우기
	CRect rect;
	GetDlgItem(IDC_BUTTON_HOUR)->GetWindowRect(&rect);
	sMenu->TrackPopupMenu(
		TPM_RIGHTBUTTON|TPM_LEFTALIGN, rect.left, rect.bottom, this);
	delete pMenu;
}

void CDlgMemo::OnButtonMin() 
{
	// TODO: Add your control notification handler code here
	CMenu  *sMenu,*pMenu;
	pMenu = new CMenu;
	pMenu->LoadMenu(IDR_MENU_MINUTE);	
	sMenu = pMenu->GetSubMenu(0);
	
	// 메뉴를 버튼왼쪽아래에 띄우기
	CRect rect;
	GetDlgItem(IDC_BUTTON_MIN)->GetWindowRect(&rect);
	sMenu->TrackPopupMenu(
		TPM_RIGHTBUTTON|TPM_LEFTALIGN, rect.left, rect.bottom, this);
	delete pMenu;
}

void CDlgMemo::OnHour1() 
{
	// TODO: Add your command handler code here
	m_uAlarmHour = (m_uAlarmNoon * 12) + 1;		
	SetCtrlText();
}

void CDlgMemo::OnHour10() 
{
	// TODO: Add your command handler code here
	m_uAlarmHour = (m_uAlarmNoon * 12) + 10;	
	SetCtrlText();
}

void CDlgMemo::OnHour11() 
{
	// TODO: Add your command handler code here
	m_uAlarmHour = (m_uAlarmNoon * 12) + 11;	
	SetCtrlText();
}

void CDlgMemo::OnHour12() 
{
	// TODO: Add your command handler code here
	m_uAlarmHour = (m_uAlarmNoon * 12) + 12;
	SetCtrlText();
//	m_btnHour.SetWindowText("12시");
}

void CDlgMemo::OnHour2() 
{
	// TODO: Add your command handler code here
	m_uAlarmHour = (m_uAlarmNoon * 12) + 2;	
	SetCtrlText();
}

void CDlgMemo::OnHour3() 
{
	// TODO: Add your command handler code here
	m_uAlarmHour = (m_uAlarmNoon * 12) + 3;	
	SetCtrlText();
}

void CDlgMemo::OnHour4() 
{
	// TODO: Add your command handler code here
	m_uAlarmHour = (m_uAlarmNoon * 12) + 4;	
	SetCtrlText();
}

void CDlgMemo::OnHour5() 
{
	// TODO: Add your command handler code here
	m_uAlarmHour = (m_uAlarmNoon * 12) + 5;	
	SetCtrlText();
}

void CDlgMemo::OnHour6() 
{
	// TODO: Add your command handler code here
	m_uAlarmHour = (m_uAlarmNoon * 12) + 6;	
	SetCtrlText();
}

void CDlgMemo::OnHour7() 
{
	// TODO: Add your command handler code here
	m_uAlarmHour = (m_uAlarmNoon * 12) + 7;	
	SetCtrlText();
}

void CDlgMemo::OnHour8() 
{
	// TODO: Add your command handler code here
	m_uAlarmHour = (m_uAlarmNoon * 12) + 8;	
	SetCtrlText();
}

void CDlgMemo::OnHour9() 
{
	// TODO: Add your command handler code here
	m_uAlarmHour = (m_uAlarmNoon * 12) + 9;	
	SetCtrlText();
}

void CDlgMemo::OnMin15() 
{
	// TODO: Add your command handler code here
	m_uAlarmMinute = 15;	
	SetCtrlText();
}

void CDlgMemo::OnMin30() 
{
	// TODO: Add your command handler code here
	m_uAlarmMinute = 30;	
	SetCtrlText();
}

void CDlgMemo::OnMin45() 
{
	// TODO: Add your command handler code here
	m_uAlarmMinute = 45;		
	SetCtrlText();
}

void CDlgMemo::OnMin0() 
{
	// TODO: Add your command handler code here
	m_uAlarmMinute = 0;		
	SetCtrlText();
}

void CDlgMemo::OnNoonFore() 
{
	// TODO: Add your command handler code here
	m_uAlarmHour = m_uAlarmHour%12;
	m_uAlarmNoon = FORENOON;	
	SetCtrlText();
}

void CDlgMemo::OnNoonAfter() 
{
	// TODO: Add your command handler code here
	m_uAlarmHour = (m_uAlarmHour % 12) + 12;
	m_uAlarmNoon = AFTERNOON;	
	SetCtrlText();
}


void CDlgMemo::OnButtonAlarm() 
{
	// TODO: Add your control notification handler code here
	m_bUseAlarm = !m_bUseAlarm;
	
	m_btnMinute.EnableWindow(m_bUseAlarm);
	m_btnHour.EnableWindow(m_bUseAlarm);
	m_btnNoon.EnableWindow(m_bUseAlarm);	
}

void CDlgMemo::SetData(UINT uYear, UINT uMonth, UINT uDay, CString stText)
{
	CString strTemp;
	strTemp.Format(_T("   : CDlgMemo::SetData[%d/%d/%d/%s] "), uYear, uMonth, uDay, stText);
	LOG(strTemp);
	m_uYear = uYear;
	m_uMonth = uMonth;
	m_uDay = uDay;
	SetTitle();
	SetText(stText);
}

void CDlgMemo::SetText(CString stText)
{
	m_RichEditText.SetWindowText(stText);
}

// 타이틀 설정
void CDlgMemo::SetTitle()
{
	// 스태틱부분 다시 그려주기
	// 글자겹치는 현상 없애기 위해서
	CRect rec;
	m_StaticTitle.GetWindowRect(&rec);
	ScreenToClient(rec);
	InvalidateRect(rec, TRUE);

	CString st;
	st.Format(_T("%d년 %d월 %d일"), m_uYear, m_uMonth, m_uDay);	

	m_StaticTitle.SetWindowText(st);
//MACRODEL	m_StaticTitle.Invalidate(TRUE);	
}

// 알람시각버튼의 텍스트 설정
void CDlgMemo::SetCtrlText()
{
	CString stTemp;
	if(FORENOON==m_uAlarmNoon)
		m_btnNoon.SetWindowText(_T("오전"));
	else
		m_btnNoon.SetWindowText(_T("오후"));

	UINT uShowHour;
	if(0==(m_uAlarmHour%12))
		uShowHour = 12;
	else
		uShowHour = m_uAlarmHour%12;
	stTemp.Format(_T("%d시"), uShowHour);
	m_btnHour.SetWindowText(stTemp);

	stTemp.Format(_T("%d분"), m_uAlarmMinute);
	m_btnMinute.SetWindowText(stTemp);
}

void CDlgMemo::SetEnableCtrl(BOOL bEnable)
{
//MACRODEL	m_btnUseAlarm.EnableWindow(bEnable);
	m_btnNoon.EnableWindow(bEnable);
	m_btnHour.EnableWindow(bEnable);
	m_btnMinute.EnableWindow(bEnable);
}

// 알람관련버튼의 Visible설정
// 현재날짜 이전에는 알람관련버튼을 보이지 않기위해
void CDlgMemo::SetVisibleCtrl(BOOL bVisible)
{
	m_btnUseAlarm.ShowWindow(bVisible);
	m_btnNoon.ShowWindow(bVisible);
	m_btnHour.ShowWindow(bVisible);
	m_btnMinute.ShowWindow(bVisible);
}

// 배경이미지 그리기
BOOL CDlgMemo::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default	
	CRect rect; 
	GetClientRect( &rect );	
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_BGDC, 0, 0, SRCCOPY);				
	return TRUE;
}

// 스태틱배경 브러쉬 바꾸기
HBRUSH CDlgMemo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	// TODO: Change any attributes of the DC here
	if( nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(255,255,255));
		SetBkMode(pDC->m_hDC, TRANSPARENT);
		return (HBRUSH)::GetStockObject (NULL_BRUSH);
	}
	if(nCtlColor == CTLCOLOR_BTN)
	{
		return (HBRUSH)::GetStockObject (NULL_BRUSH);
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgMemo::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
		//point...
	if( nFlags & MK_LBUTTON ) // L 마우스 버튼이 눌러져 있구나..
	{
		this->PostMessage(
			WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
		return;				
	}	
	CDialog::OnMouseMove(nFlags, point);
}

BOOL CDlgMemo::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message && VK_TAB == pMsg->wParam) 
	{
		m_RichEditText.ReplaceSel(_T("\t"));
		return TRUE;
	}

	// 리치에디트에서 오른쪽버튼 눌렀을 때	
	if(WM_RBUTTONDOWN == pMsg->message)
	{
		CWnd * pWnd = (CWnd*) GetDlgItem(IDC_RICHEDIT_MEMO_TEXT); 
		POINT	point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		
		CRect	recRich;
		m_RichEditText.GetClientRect(&recRich);
		if(pWnd ==GetFocus()&&recRich.PtInRect(point))	
		{
			// 아래함수에서 편집메뉴를 띄운다
			// (복사하기/붙여넣기/지우기/전체선택)
			RightButtonOnRichEdit();
		}
	}			
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgMemo::ResetDay()
{

	SetData(0,0,0,"");
	SetTitle();
	m_uAlarmHour = 0;
	m_uAlarmMinute = 0;
	m_uAlarmNoon = 0;
	m_bUseAlarm = false;
//MACRODEL	m_pWndDay = NULL;

	m_btnUseAlarm.EnableWindow(TRUE);
	m_btnNoon.EnableWindow(FALSE);
	m_btnHour.EnableWindow(FALSE);
	m_btnMinute.EnableWindow(FALSE);
}

// 알람시각을 현재시각보다 10분뒤로 설정
void CDlgMemo::OnAfter10Min() 
{
	// TODO: Add your command handler code here
	CTime t = CTime::GetCurrentTime();
	UINT uAlarmMinute = t.GetMinute();
	UINT uAlarmHour = t.GetHour();
	if(59 < (uAlarmMinute+10) )
	{
		if(23<=uAlarmHour)
		{
//			AfxMessageBox(_T("다음날입니다."));				
			return;
		}
		else
		{
			uAlarmMinute = (uAlarmMinute+10) -  60;
			uAlarmHour++;
		}
	}
	else
		uAlarmMinute += 10;
	m_uAlarmMinute = uAlarmMinute;
	m_uAlarmHour = uAlarmHour;
	SetCtrlText();
}

// 알람시각을 현재시각보다 30분뒤로 설정
void CDlgMemo::OnAfter30Min() 
{
	// TODO: Add your command handler code here
	CTime t = CTime::GetCurrentTime();
	UINT uAlarmMinute = t.GetMinute();
	UINT uAlarmHour = t.GetHour();
	if(59 < (uAlarmMinute+30) )
	{
		if(23<=uAlarmHour)
		{
//			AfxMessageBox(_T("다음날입니다."));			
			return;
		}
		else
		{
			uAlarmMinute = (uAlarmMinute+30) -  60;
			uAlarmHour++;
		}
	}
	else
		uAlarmMinute += 30;
	m_uAlarmMinute = uAlarmMinute;
	m_uAlarmHour = uAlarmHour;
	SetCtrlText();
}

// 알람시각을 현재시각보다 5분뒤로 설정
void CDlgMemo::OnAfter5Min() 
{
	// TODO: Add your command handler code here	
	CTime t = CTime::GetCurrentTime();
	UINT uAlarmMinute = t.GetMinute();
	UINT uAlarmHour = t.GetHour();
	if(59 < (uAlarmMinute+5) )
	{
		if(23<=uAlarmHour)
		{
//			AfxMessageBox(_T("다음날입니다."));			
			return;
		}
		else
		{
			uAlarmMinute = (uAlarmMinute+5) -  60;
			uAlarmHour++;
		}
	}
	else
		uAlarmMinute += 5;
	m_uAlarmMinute = uAlarmMinute;
	m_uAlarmHour = uAlarmHour;
	SetCtrlText();
}

// 리치에디트위에서 오른쪽버튼을 눌렀을 때
// 편집메뉴(복사하기/붙여넣기/지우기/모두선택)을 띄운다.
void CDlgMemo::RightButtonOnRichEdit()
{
	POINT	point;
	CMenu  *sMenu,*pMenu;
	pMenu = new CMenu;
	pMenu ->LoadMenu(IDR_MENU_RICHEDIT);	
	sMenu = pMenu->GetSubMenu(0);
	GetCursorPos(&point);
	sMenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN, point.x, point.y, this, 
		CRect(point.x-100,point.y-100,point.x,point.y));
	delete	pMenu;
}

// 리치에디트에서 오른쪽버튼 클릭시 나오는메뉴 중
// 복사하기 클릭시
void CDlgMemo::OnRicheditCopy() 
{
	// TODO: Add your command handler code here
	m_RichEditText.Copy();
}

// 리치에디트에서 오른쪽버튼 클릭시 나오는메뉴 중
// 붙여넣기 클릭시
void CDlgMemo::OnRicheditPaste() 
{
	// TODO: Add your command handler code here
	m_RichEditText.Paste();
}

// 리치에디트에서 오른쪽버튼 클릭시 나오는메뉴 중
// 지우기 클릭시
void CDlgMemo::OnRicheditClear() 
{
	// TODO: Add your command handler code here
	m_RichEditText.SetSel(0, -1);	
	m_RichEditText.Clear();
}

// 리치에디트에서 오른쪽버튼 클릭시 나오는메뉴 중
// 전체선택 선택시
void CDlgMemo::OnRicheditSelectall() 
{
	// TODO: Add your command handler code here
	m_RichEditText.SetSel(0, -1);
}

void CDlgMemo::OnButtonMemoOk() 
{
	// TODO: Add your control notification handler code here
	SendData();
	ShowWindow(SW_HIDE);
}

void CDlgMemo::InitBG()
{
	CDC* pDC = GetDC();
	m_BGDC.CreateCompatibleDC(pDC);		
	m_BGBitmap.LoadBitmap(IDB_BITMAP_MEMO_BACKIMAGE);
	m_BGDC.SelectObject(&m_BGBitmap);
	ReleaseDC(pDC);
}

void CDlgMemo::SetData(CData *pData)
{
	pData->GetDate(&m_uYear, &m_uMonth, &m_uDay);
	SetTitle();
	SetText(pData->GetText());
}

void CDlgMemo::GetData(CData* pData)
{	
	pData->SetData(m_uYear, m_uMonth, m_uDay, GetText());	
	pData->SetUseAlarm(m_bUseAlarm);
}

CString CDlgMemo::GetText()
{
	CString strReturn = _T("");
	m_RichEditText.GetWindowText(strReturn);
	return strReturn;
}

void CDlgMemo::MoveMemo()
{
	
	/* 여기서부터 메모가 화면밖으로 안나가게 설정*/	
	LONG cxScreen, cyScreen;
	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);
	
	CPoint Pos;
	GetCursorPos(&Pos);
	if(cxScreen < Pos.x + MEMOWIDTH + 15)
		Pos.x = cxScreen - MEMOWIDTH - 15;
	if(cyScreen < Pos.y + MEMOHEIGHT + 15)
		Pos.y = cyScreen - MEMOHEIGHT - 15;
	/* 여기까지가 메모가 화면밖으로 안나가게 설정하는 부분*/

	MoveWindow(Pos.x + 15, Pos.y + 15, MEMOWIDTH, MEMOHEIGHT);
}

BOOL CDlgMemo::SetMemo(CData *pData)
{
	LOG(_T("In : CDlgMemo::SetDay"));
//MACRODEL	m_pWndDay = NULL;
///////////////////////////////////////////////////////////
// 여기서부터 받아온 데이터의 오류체크
		
	// 날짜가 올바르지 않은 경우 리턴
	UINT uYear    = 0;
	UINT uMonth = 0;
	UINT uDay     = 0;
	pData->GetDate(&uYear, &uMonth, &uDay);
	BOOL Chk = ChkDay(uYear, uMonth, uDay);
	if(FALSE==Chk)
	{
		CString strTemp;
		strTemp.Format(_T("Out: ERROR [CDlgMemo::SetMemo] 올바르지 않은 날짜(%d/%d/%d)"), uYear, uMonth, uDay);
		LOG(strTemp);
		return FALSE;
	}
// 여기까지가 오류체크
///////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
// 여기서부터 데이터 설정

	// 날짜값 설정
	m_uYear    = uYear    ;
	m_uMonth = uMonth ;
	m_uDay     = uDay     ;
	m_bUseAlarm = pData->GetUseAlarm();
	SetTitle();
	SetText(pData->GetText());
// 여기까지가 데이터 설정
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////	
//여기서부터 현재시각이전에는 알람관련버튼 숨기기	
	CTime t1 = CTime::GetCurrentTime();
	CTime t2 = CTime(m_uYear, m_uMonth, m_uDay, t1.GetHour(), t1.GetMinute(), 0);
	
	bool bVisible = false;
	if(t1.GetYear() < t2.GetYear())
		bVisible = true;
	else if(t1.GetYear()==t2.GetYear())	
	{
		if(t1.GetMonth() < t2.GetMonth())
			bVisible = true;
		else if(t1.GetMonth()==t2.GetMonth())				
			if(t1.GetDay()<=t2.GetDay())						
				bVisible = true;					
	}
	
	SetVisibleCtrl(bVisible);
//여기까지가 알람관련버튼 숨기기설정
///////////////////////////////////////////////////////////
	
//MACRODEL	// 현재 메모날짜에 해당하는 알람을 불러온다.
//MACRODEL	LoadAlarmTime(m_uYear, m_uMonth, m_uDay);
	CString strTemp;
	strTemp.Format(
		_T("Out: CDlgMemo::SetDay[%d/%d/%d]"), 
		m_uYear, m_uMonth, m_uDay);
	LOG(strTemp);
	return TRUE;
}

void CDlgMemo::SendData()
{
	CData* pData = new CData;
	pData->SetDate(m_uYear, m_uMonth, m_uDay);
	CString strTemp = GetText();
	
	pData->SetText(strTemp);	
	strTemp.Format(_T("   : CDlgMemo::SaveMemo -> CKalendarDlg::OnNotify [%d/%d/%d/%s]"), m_uYear, m_uMonth, m_uDay, strTemp);
	LOG(strTemp);	
	GetParent()->SendMessage(WM_NOTIFY, IDD, (LPARAM)pData);
	delete pData;
}

void CDlgMemo::SetAlarmTime(CTime AlarmTime)
{
	m_uAlarmNoon = AlarmTime.GetHour()/12;
	m_uAlarmHour = AlarmTime.GetHour();
	m_uAlarmMinute = AlarmTime.GetMinute();
	m_bUseAlarm = TRUE;	
	SetCtrlText();
	SetEnableCtrl(m_bUseAlarm);
}

void CDlgMemo::ResetAlarmTime()
{	
	CTime AlarmTime = CTime::GetCurrentTime();
	m_uAlarmNoon = AlarmTime.GetHour()/12;
	m_uAlarmHour = AlarmTime.GetHour();
	m_uAlarmMinute = AlarmTime.GetMinute();
	m_bUseAlarm = FALSE;
	SetCtrlText();
	SetEnableCtrl(m_bUseAlarm);
}

void CDlgMemo::GetAlarmTime(UINT *uAlarmHour, UINT *uAlarmMinute)
{
	*uAlarmHour = m_uAlarmHour;
	*uAlarmMinute = m_uAlarmMinute;
}

void CDlgMemo::GetDate(UINT *uYear, UINT *uMonth, UINT *uDay)
{
	*uYear = m_uYear;
	*uMonth = m_uMonth;
	*uDay = m_uDay;
}
