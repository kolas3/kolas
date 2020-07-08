// DlgAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "Kalendar.h"
#include "DlgAlarm.h"
#include "DlgDay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarm dialog


CDlgAlarm::CDlgAlarm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAlarm)
	//}}AFX_DATA_INIT
//	m_bIsDoModal = FALSE;
	// KOL.UDF.022
	m_uAlarmDay = 0;
	m_uAlarmMonth = 0;
	m_uAlarmYear = 0;
	m_uAlarmHour = 0;
	m_uAlarmMinute = 0;
}


void CDlgAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAlarm)
	DDX_Control(pDX, ID_BUTTON_ALARM_OK, m_btnAlarmOK);
	DDX_Control(pDX, IDC_STATIC_ALARM_TITLE, m_StaticAlarmTitle);
	DDX_Control(pDX, IDC_RICHEDIT_ALARM_TEXT, m_RichEditAlarmText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAlarm, CDialog)
	//{{AFX_MSG_MAP(CDlgAlarm)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_BUTTON_ALARM_OK, OnButtonAlarmOk)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarm message handlers

void CDlgAlarm::OnOK() 
{
	// TODO: Add extra validation here
//	m_bIsDoModal = FALSE;
	CDialog::OnOK();
}

BOOL CDlgAlarm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitBG();
	m_btnAlarmOK.LoadBitmaps(IDB_BITMAP_BUTTON_CLOSE_RED);
	SetText(_T(""));
	SetTitle();
	MoveWindow(0,0,MEMOWIDTH,MEMOHEIGHT);

	CRect rDlg;
	GetWindowRect(rDlg);
	ScreenToClient(rDlg);

	LPPOINT lpPoint = new CPoint[8];
//DEL 	{ 
	lpPoint[0] = CPoint(0, 2);
 	lpPoint[1] = CPoint(2, 0);
 	lpPoint[2] = CPoint(rDlg.right -2, 0);
 	lpPoint[3] = CPoint(rDlg.right, 2);
 	lpPoint[4] = CPoint(rDlg.right, rDlg.bottom -3);
 	lpPoint[5] = CPoint(rDlg.right-3, rDlg.bottom);
 	lpPoint[6] = CPoint(3, rDlg.bottom);
 	lpPoint[7] = CPoint(0, rDlg.bottom-3);
//DEL 	};
	CRgn Rgn;
 	Rgn.CreatePolygonRgn(lpPoint, 8, WINDING);
	delete [] lpPoint;

	SetWindowRgn(Rgn, TRUE);
	Rgn.DeleteObject();

	// TODO: Add extra initialization here	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgAlarm::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default	
	CRect rect; 
	GetClientRect( &rect );	
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_BGDC, 0, 0, SRCCOPY);				
	return TRUE;
}

HBRUSH CDlgAlarm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	// TODO: Change any attributes of the DC here
	if( nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(255,255,255));
		SetBkMode(pDC->m_hDC, TRANSPARENT);
		return (HBRUSH)::GetStockObject (NULL_BRUSH);
	}	// TODO: Return a different brush if the default is not desired

	return hbr;
}

void CDlgAlarm::OnButtonAlarmOk() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlgAlarm::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( nFlags & MK_LBUTTON ) // L 마우스 버튼이 눌러져 있구나..
	{
		this->PostMessage(
			WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
		return;				
	}
	CDialog::OnMouseMove(nFlags, point);
}


// 함수이름: CDlgAlarm::InitBG
// 함수설명: 배경DC 및 비트맵 초기화
// 반환  형: void 
void CDlgAlarm::InitBG()
{
	CDC* pDC = GetDC();
	m_BGDC.CreateCompatibleDC(pDC);		
	m_BGBitmap.LoadBitmap(IDB_BITMAP_ALARM_BACKIMAGE);
	m_BGDC.SelectObject(&m_BGBitmap);
	ReleaseDC(pDC);
}

void CDlgAlarm::SetTitle()
{
	CString stTitle;
	stTitle.Format(_T("알람 : %d시 %d분"), m_uAlarmHour, m_uAlarmMinute);
	m_StaticAlarmTitle.SetWindowText(stTitle);
}


// 함수이름: CDlgAlarm::SetData
// 함수설명: 
// 반환  형: void 
// 인자[CTime AlarmTime] : 
// 인자[CString stText] : 
void CDlgAlarm::SetData(CTime AlarmTime, CString stText)
{
	m_uAlarmYear	= AlarmTime.GetYear();
	m_uAlarmMonth = AlarmTime.GetMonth();
	m_uAlarmDay     = AlarmTime.GetDay();
	m_uAlarmHour   = AlarmTime.GetHour();
	m_uAlarmMinute= AlarmTime.GetMinute();

	SetTitle();
	SetText(stText);
	SendData();
}


// 함수이름: CDlgAlarm::SetText
// 함수설명: 
// 반환  형: void 
// 인자[CString stText] : 
void CDlgAlarm::SetText(CString stText)
{
	m_RichEditAlarmText.SetWindowText(stText);
}

void CDlgAlarm::GetData(CData *pData)
{
	pData->SetDate(m_uAlarmYear,m_uAlarmMonth, m_uAlarmDay);
	pData->SetText(_T(""));
}

void CDlgAlarm::SendData()
{
	CData* pData = new CData;
	GetData(pData);
	GetParent()->SendMessage(WM_NOTIFY, IDD, (LPARAM)pData);
	delete pData;
}
