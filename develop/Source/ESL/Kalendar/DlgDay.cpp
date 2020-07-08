// DlgDay.cpp : implementation file
//

#include "stdafx.h"
#include "Kalendar.h"
#include "DlgDay.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULTBACKCOLOR RGB(255,255,255)//RGB(255,255,255)
#define TODAYBACKCOLOR RGB(128,248,128)
/////////////////////////////////////////////////////////////////////////////
// CDlgDay dialog


CDlgDay::CDlgDay(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDay)
	//}}AFX_DATA_INIT
//DEL  	m_bIsToday = FALSE;
	m_uDayOfTheWeek = WEEKDAY;
//	m_stText = _T("");
	m_Brush.CreateSolidBrush(RGB(255,255,255));

	m_bIsHoliday = FALSE;
	m_bIsToday = FALSE;

	// KOL.UDF.022
	m_bUseAlarm = FALSE;	
	m_uDayOfTheWeek = 0;
	m_uDay = 0; 
	m_uMonth = 0;
	m_uYear = 0; 
}

CDlgDay::~CDlgDay()
{

}
void CDlgDay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDay)
	DDX_Control(pDX, IDC_RICHEDIT_TEXT, m_RichEditText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDay, CDialog)
	//{{AFX_MSG_MAP(CDlgDay)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()	
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDay message handlers

// 날짜설정
// 현재 날짜를 리셋하고, 새 날짜로 설정
// 새 날짜의 파일이 있으면 불러오기
void CDlgDay::SetDay(UINT uYear, UINT uMonth, UINT uDay)
{	
	// 현재데이터 저장하고, 리셋	
	SaveDay();	
	ResetDay();

	if(FALSE==ChkDay(uYear, uMonth, uDay))
	{
		CString strTemp;
		strTemp.Format(_T("ERROR : [CDlgDay::SetDay] 올바르지 않은 날짜(%d/%d/%d)"), uYear, uMonth, uDay);
		LOG(strTemp);
		return;
	}
		// 메모날짜의 변경
		m_uYear = uYear;
		m_uMonth = uMonth;
		m_uDay = uDay;	
	
	// 해당날짜의 파일이 있으면	
	if(MEMOVIEW==g_ViewMode)
	{
		if(CCalFileMgr::IsFile(GetFileName()))	
		{
			ReadMemoFile();
		}
	}
	Invalidate();
}

// 다이어로그 초기화
BOOL CDlgDay::OnInitDialog() 
{
	CDialog::OnInitDialog();		
	m_bUseAlarm = FALSE;
	/* 리치에디트 마우스이벤트 잡아내기*/
	CRichEditCtrl *pWndRichEditCtrl = &m_RichEditText;
	//(CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT1);
	 pWndRichEditCtrl->SetEventMask(
		 pWndRichEditCtrl->GetEventMask() | ENM_MOUSEEVENTS);
	 // | ENM_SCROLLEVENTS | ENM_KEYEVENTS);
	 /* 리치에디트 마우스이벤트 잡아내기의 끝*/	
	// TODO: Add extra initialization here

	 InitBG();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 날짜 리셋
void CDlgDay::ResetDay()
{
	// 초기화
	m_RichEditText.SetWindowText(_T(""));

// 	GetDlgItem(IDC_STATIC_TODAY)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STATIC_HOLIDAY)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STATIC_TODAY_HOLIDAY)->ShowWindow(SW_HIDE);
	m_bUseAlarm = FALSE;
	m_uDay = 0;
	m_uMonth = 0;
	m_uYear = 0;
	m_bIsHoliday = FALSE;
	m_bIsToday = FALSE;

	Invalidate();
}

HBRUSH CDlgDay::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CDlgDay::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//	CDialog::OnClose();
}

void CDlgDay::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CDlgDay::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}

// 휴관일 설정
void CDlgDay::SetHoliday()
{
	if(!IsToday())
	{
// 		GetDlgItem(IDC_STATIC_HOLIDAY)->ShowWindow(SW_SHOW);
	}
	else
	{
// 		GetDlgItem(IDC_STATIC_TODAY)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_TODAY_HOLIDAY)->ShowWindow(SW_SHOW);
	}
	m_bIsHoliday = TRUE;
}

// 텍스트 설정
void CDlgDay::SetText(CString st)
{
	m_RichEditText.SetWindowText(st);	
	WriteMemoFile();
}

// 텍스트컬러 설정
void CDlgDay::SetTextColor(COLORREF cr)
{
	CHARFORMAT cf;
	m_RichEditText.GetDefaultCharFormat( cf );
	cf.crTextColor = cr;
	cf.dwMask |= CFM_COLOR;
	cf.dwEffects &= (~CFE_AUTOCOLOR);
	m_RichEditText.SetDefaultCharFormat(cf);		
}

// 현재 마우스커서가 리치에디트텍스트위에 있는지 리턴
BOOL CDlgDay::IsMousePointInRichEditText() const
{
	CRect rec;
	CPoint point;
	m_RichEditText.GetClientRect(&rec);	

	GetCursorPos(&point);
	ScreenToClient(&point);

	return rec.PtInRect(point);
}

void CDlgDay::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default			
	CDialog::OnLButtonDown(nFlags, point);
	SendData();
}

BOOL CDlgDay::IsDay() const
{	
	if(0>=m_uDay) 
		return FALSE;	
	return TRUE;
}

void CDlgDay::MoveWindow(CRect rec)
{
	CDialog::MoveWindow(rec);		
	GetClientRect(&rec);
	m_RichEditText.MoveWindow(2,16,rec.right, rec.bottom);	
}

// 백그라운드 컬러설정
void CDlgDay::SetBackgroundColor(COLORREF cr)
{
	m_RichEditText.SetBackgroundColor(FALSE, cr);
}

void CDlgDay::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(IsDay()&&!IsDataEmpty())
	{		
		WriteMemoFile();
	}
}

// 데이터가 있는지
BOOL CDlgDay::IsDataEmpty() const
{
	CString st;
	m_RichEditText.GetWindowText(st);
	if(_T("")==st)
		return TRUE;
	return FALSE;
}

// 달력의 날짜가 오늘인지
BOOL CDlgDay::IsToday() const
{
	CTime t = CTime::GetCurrentTime();
	if((UINT)t.GetYear() != m_uYear)
		return FALSE;
	if((UINT)t.GetMonth() != m_uMonth)
		return FALSE;
	if((UINT)t.GetDay() != m_uDay)
		return FALSE;
	return TRUE;	
}

// 텍스트의 리턴
CString CDlgDay::GetText() const
{	
	CString st;
	m_RichEditText.GetWindowText(st);
	return st;
}

void CDlgDay::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnMouseMove(nFlags, point);
}

BOOL CDlgDay::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgDay::SaveDay()
{	
	// 현재 날짜의 데이터기록
	CString st = GetFileName();	
	// 날짜가 올바르고, 데이터가 있으면 저장
	if(IsDay()&&!IsDataEmpty())
	{
		WriteMemoFile();				
	}
	// 파일이 있고, 현재 메모의 데이터가 없으면 메모삭제
	if(IsDataEmpty()&&CCalFileMgr::IsFile(st))		
	{		
		::DeleteFile(st);
	}
}

UINT CDlgDay::GetDay()
{
	return m_uDay;
}

UINT CDlgDay::GetYear()
{
	return m_uYear;
}


UINT CDlgDay::GetMonth()
{
	return m_uMonth;
}

void CDlgDay::GetDate(UINT* pYear, UINT* pMonth, UINT* pDay)
{
	*pYear = m_uYear;
	*pMonth = m_uMonth;
	*pDay = m_uDay;
}

BOOL CDlgDay::WriteMemoFile()
{
	CString stFileName = GetFileName();
	CString stText = _T("");
	if(_T("")!= stFileName)
	{
		stText = GetText();
		CCalFileMgr::WriteMemoFile(stFileName, stText);
		return TRUE;
	}
	return FALSE;
}

BOOL CDlgDay::ReadMemoFile()
{
	CString stFileName = GetFileName();
	CString stText;
	if(_T("")!=stFileName)
	{
		CCalFileMgr::ReadMemoFile(stFileName, &stText);	
		SetText(stText);
		return TRUE;
	}
	return FALSE;
}

void CDlgDay::SetWeekDay()
{
	m_uDayOfTheWeek = WEEKDAY;
}

void CDlgDay::SetSunday()
{
	m_uDayOfTheWeek = SUNDAY;
}

void CDlgDay::SetSaturday()
{
	m_uDayOfTheWeek = SATURDAY;
}

void CDlgDay::SetToday()
{
// 	GetDlgItem(IDC_STATIC_TODAY)->ShowWindow(SW_SHOW);
//DEL 	m_bIsToday = TRUE;	
	m_bIsToday = TRUE;
}

CString CDlgDay::GetFileName()
{
	return CCalFileMgr::GetMemoFileName(m_uYear, m_uMonth, m_uDay);
}

void CDlgDay::GetData(CData *pData)
{
	pData->SetData(m_uYear, m_uMonth, m_uDay, GetText());
}

void CDlgDay::SetDate(CData *pData)
{
	UINT uYear = 0;
	UINT uMonth = 0;
	UINT uDay = 0;
	pData->GetDate(&uYear,&uMonth,&uDay);
	SetDay(uYear,uMonth,uDay);
}

void CDlgDay::SendData()
{
	CData* pData = new CData;
	GetData(pData);
	GetParent()->SendMessage(WM_NOTIFY, IDD, (LPARAM)pData);
	delete pData;
}

BOOL CDlgDay::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Brush.DeleteObject();
	return CDialog::DestroyWindow();
}

BOOL CDlgDay::OnEraseBkgnd(CDC* pDC) 
{
/*
	CRect rRect;
	GetClientRect(&rRect);

	CBitmap MemBitmap, Bitmap[2];
	CDC DC[2];
	
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	MemBitmap.CreateCompatibleBitmap(pDC, rRect.Width(), rRect.Height());
	MemDC.SelectObject(&MemBitmap);

	UINT uNum[2] = {0,0};
	UINT uTmp = m_uDay;
	UINT BitmapDay = IDB_BITMAP_DAY_BLACK_0;
	if(SATURDAY == m_uDayOfTheWeek)
		BitmapDay = IDB_BITMAP_DAY_BLUE_0;
	if(SUNDAY == m_uDayOfTheWeek)
		BitmapDay = IDB_BITMAP_DAY_RED_0;

	for(UINT i=0;i<2;i++)
	{
		uNum[1-i] = uTmp % 10;
		uTmp = uTmp / 10;
	}		

	if(0==uNum[0] && 0==uNum[1])
	{
		for(i=0;i<2;i++)
		{
			DC[i].CreateCompatibleDC(pDC);	
			Bitmap[i].LoadBitmap(IDB_BITMAP_DAY_NULL);		
			DC[i].SelectObject(&Bitmap[i]);
		}
	}
	else
	{
		for(i=0;i<2;i++)
		{
			DC[i].CreateCompatibleDC(pDC);				
			Bitmap[i].LoadBitmap(BitmapDay + uNum[i]);		
			DC[i].SelectObject(&Bitmap[i]);
		}
	}

	MemDC.FillSolidRect(0, 0, rRect.Width(), rRect.Height(), RGB(255,255,255));
	for(i=0;i<2;i++)
	{
		MemDC.BitBlt(i*10,0,10, 17, &DC[i],0,0,SRCCOPY);
	}	
	pDC->FillSolidRect(0, 0, 100, 100, RGB(255,0,0));
 	pDC->BitBlt(0, 0, 20, 15, &MemDC, rRect.Width(), rRect.Height(), SRCCOPY);
*/

	UINT uNum[2] = {0,0};
	UINT uTmp = m_uDay;
	INT nY = 0;
	switch(m_uDayOfTheWeek)
	{
	case SATURDAY:
		nY = 15;
		break;
	case SUNDAY:
		nY = 30;
		break;
	default:
		nY = 0;
		break;
	}

	for(UINT i=0;i<2;i++)
	{
		uNum[1-i] = uTmp % 10;
		uTmp = uTmp / 10;
	}		

	m_MemDC.FillSolidRect(0, 0, DAYWIDTH, DAYHEIGHT, RGB(255,255,255));
	if(0 != uNum[0] || 0 != uNum[1])
	{
		m_MemDC.BitBlt(0, 0, 10, 15, &m_DC[0], uNum[0] * 10, nY, SRCCOPY);	
		m_MemDC.BitBlt(10, 0, 10, 15, &m_DC[0], uNum[1] * 10, nY, SRCCOPY);	
	}

	if(m_bIsHoliday && m_bIsToday)
	{
		m_MemDC.BitBlt(20, 0, 83, 15, &m_DC[3], 0, 0, SRCCOPY);	
	}
	else if(m_bIsHoliday)
	{
		m_MemDC.BitBlt(20, 0, 49, 15, &m_DC[1], 0, 0, SRCCOPY);	
	}
	else if(m_bIsToday)
	{
		m_MemDC.BitBlt(20, 0, 36, 14, &m_DC[2], 0, 0, SRCCOPY);	
	}

	pDC->BitBlt(0, 0, DAYWIDTH, DAYHEIGHT, &m_MemDC, 0, 0, SRCCOPY);

 	return TRUE;
}

INT CDlgDay::InitBG()
{
	CDC* pDC = GetDC();

	m_MemDC.CreateCompatibleDC(pDC);
	m_MemBitmap.CreateCompatibleBitmap(pDC, DAYWIDTH, DAYHEIGHT);
	m_MemDC.SelectObject(&m_MemBitmap);

	m_DC[0].CreateCompatibleDC(pDC);
	m_Bitmap[0].LoadBitmap(IDB_DAY);
	m_DC[0].SelectObject(&m_Bitmap[0]);

	m_DC[1].CreateCompatibleDC(pDC);
	m_Bitmap[1].LoadBitmap(IDB_BITMAP_HOLIDAY);
	m_DC[1].SelectObject(&m_Bitmap[1]);

	m_DC[2].CreateCompatibleDC(pDC);
	m_Bitmap[2].LoadBitmap(IDB_BITMAP_TODAY);
	m_DC[2].SelectObject(&m_Bitmap[2]);

	m_DC[3].CreateCompatibleDC(pDC);
	m_Bitmap[3].LoadBitmap(IDB_BITMAP_TODAY_HOLIDAY);
	m_DC[3].SelectObject(&m_Bitmap[3]);

	ReleaseDC(pDC);

	return 0;
}