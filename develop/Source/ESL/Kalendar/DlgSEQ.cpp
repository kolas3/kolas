// DlgSEQ.cpp : implementation file
//

#include "stdafx.h"
#include "kalendar.h"
#include "DlgSEQ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSEQ dialog


CDlgSEQ::CDlgSEQ(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSEQ::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSEQ)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nArrayIndex_Of_SelectPopupStatic = -1;
	m_uDayOfTheWeek = WEEKDAY;
	m_bIsInitDataStatic[TITLE_STATIC] = FALSE;
	m_bIsInitDataStatic[POPUP_STATIC] = FALSE;
	m_bIsInitDataStatic[2] = FALSE;
	m_bIsInitDataStatic[3] = FALSE;

	m_pOldFont = NULL;
	m_pNewFont = NULL;

	// KOL.UDF.022
	//DATA_STATIC* m_pPrev_Data_Static = NULL;
	m_pPrev_Data_Static = NULL;

	m_strToolTipText_For_Mouse_In_the_PopupMenu = _T("");

	m_bIsHoliday = FALSE;
	m_bIsToday = FALSE;
	// KOL.UDF.022
	m_pSelect_Data_Static = NULL;
	m_nArrayIndex_Of_SelectPopupStatic = 0;
	m_uDayOfTheWeek = 0;
	m_uYear = 0;
	m_uMonth = 0;
	m_uDay = 0;	
}

CDlgSEQ::~CDlgSEQ()
{
	RemoveAll_DataStatic();
	if(NULL != m_pNewFont) delete m_pNewFont;
	if(NULL != m_pOldFont) delete m_pOldFont;

	m_Brush.DeleteObject();	

}
void CDlgSEQ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSEQ)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSEQ, CDialog)
	//{{AFX_MSG_MAP(CDlgSEQ)
	ON_COMMAND_RANGE(ID_DIVISION_NO1, ID_DIVISION_NO100, OnButtonPopupMenu)
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()	
	ON_WM_TIMER()	
	ON_WM_MENUSELECT()
	ON_WM_MOUSEMOVE()
	ON_WM_ENTERIDLE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSEQ message handlers

BOOL CDlgSEQ::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Brush.CreateSolidBrush(RGB(255,255,255));

	////////////////////////////////////////////////////////////////////////
	// 여기서부터 마우스가 올려져있는 스태틱에 적용할 변수들 초기화
	CFont* pFont = GetFont();

	LOGFONT LogFont;
	pFont->GetLogFont(&LogFont);

	m_pOldFont = new CFont;
	m_pOldFont->CreateFontIndirect(&LogFont);
	
	LogFont.lfUnderline = TRUE;
	
	m_pNewFont = new CFont;
	m_pNewFont->CreateFontIndirect(&LogFont);

	m_pPrev_Data_Static = NULL;
	m_pSelect_Data_Static = NULL;
	// 여기까지 마우스가 올려져있는 스태틱에 적용할 변수들 초기화
	////////////////////////////////////////////////////////////////////////
	
	m_ToolTip.Create(this);
	m_ToolTip.SetDelay(300);
	// 마우스의 위치에서 실제 출력될 툴팁상자 위치의 보정치
	m_ToolTip.SetOffset(20, 20);	// 마우스의 위치에서 10, 10을 추가한 곳에서 툴팁 보여주기
	m_ToolTip.On(TRUE);

	Init_DataStatic();

	InitBG();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSEQ::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//DEL  	INT nArrayIndex = 0, nIndex = 0;
//DEL  	if(0 <= Find_StaticIndex_InMouse(&nArrayIndex, &nIndex))
//DEL  	{
//DEL  		if(POPUP_STATIC == nIndex)
//DEL  		{
//DEL  			DATA_STATIC_EX *pDATA_STATIC = (DATA_STATIC_EX*)m_aStaticArray[nArrayIndex].GetAt(POPUP_STATIC);
//DEL  			OnButtonPopupStatic(pDATA_STATIC, nArrayIndex, nIndex);			
//DEL  		}
//DEL  		else
//DEL  		{		
//DEL  			m_ToolTip.Hide();
//DEL  			m_nArrayIndex_Of_SelectPopupStatic = -1;
//DEL  
//DEL  
//DEL  			// 선택된 데이터 전송
//DEL  			CData* pData = new CData;
//DEL  			Get_SelectData(pData);
//DEL  			GetParent()->SendMessage(WM_NOTIFY, IDD, (LPARAM)pData);
//DEL  			delete pData;
//DEL  		}
//DEL  	}

	CDialog::OnLButtonDown(nFlags, point);
}

HBRUSH CDlgSEQ::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//DEL 	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(nCtlColor==CTLCOLOR_DLG)
	{
	//DEL  	pDC->SetBkMode(TRANSPARENT);		
		//(HBRUSH)::CreateSolidBrush(RGB(255, 255, 255));
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	else if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(128, 128, 128));

		if(NULL != m_pSelect_Data_Static)
		{
			if(pWnd->GetSafeHwnd() ==
				m_pSelect_Data_Static->mCtrl_Static.GetSafeHwnd()
				)
			{
				pDC->SelectObject(m_pNewFont);
				pDC->SetTextColor(RGB(200, 50, 50));
			}
		}
	}
	// TODO: Return a different brush if the default is not desired
	return m_Brush;

	// TODO: Return a different brush if the default is not desired
}


// 함수이름: CDlgSEQ::OnSetCursor
// 함수설명: 커서 설정
// 마우스가 스태틱위에 올려져 있는지 판단후 커서설정
// 이전, 현재 선택한 스태틱 설정(새폰트 적용 및 해제)
// 반환  형: BOOL 
// 인자[CWnd* pWnd] : 
// 인자[UINT nHitTest] : 
// 인자[UINT message] : 
BOOL CDlgSEQ::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default	
 	KillTimer(ID_TIMER_FOR_MOUSE_WITHOUT_DLGSEQ_CLIENT);
	DATA_STATIC* pDATA_STATIC = NULL;

	// 마우스가 스태틱위에 올려져있을 경우
	if(0 <= Get_StaticInMouse(&pDATA_STATIC))
	{	
		CString strMsg = _T("AUTO_HIDE:FALSE");
		GetParent()->SendMessage(WM_REQUEST_KALENDARDLG, IDD, (LPARAM)&strMsg);

		SetCursor(LoadCursor(NULL, IDC_HAND));	   	
		m_pSelect_Data_Static = pDATA_STATIC;
		pDATA_STATIC->mCtrl_Static.Invalidate();
		
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		if(_T("") != pDATA_STATIC->m_strToolTipText)
		{		 	
			m_ToolTip.Set(point, pDATA_STATIC->m_strToolTipText);
		}
 		SetTimer(ID_TIMER_FOR_MOUSE_WITHOUT_DLGSEQ_CLIENT, 100, NULL);
	}
	else
	{
		CString strMsg = _T("AUTO_HIDE:TRUE");
		GetParent()->SendMessage(WM_REQUEST_KALENDARDLG, IDD, (LPARAM)&strMsg);

		SetCursor(LoadCursor(NULL, IDC_ARROW));
		m_pSelect_Data_Static = NULL;
		m_ToolTip.Hide();
	}
	
 	if(pDATA_STATIC != m_pPrev_Data_Static)
 	{	
 		if(NULL != m_pPrev_Data_Static)	  
 			m_pPrev_Data_Static->mCtrl_Static.Invalidate();
 		m_pPrev_Data_Static = pDATA_STATIC;
 	}
	return TRUE;
}

INT CDlgSEQ::AddData(CString strStatus, CString strData, CString strCaption, CString strToolTipText)
{
	INT nMode = -1;
	if(_T("구입") == strStatus)
	{
		nMode = ACQ;
	}
	else if(_T("검수") == strStatus)
	{
		nMode = CHECKIN;
	}
	else if(_T("미납") == strStatus)
	{
		nMode = MISSING;
	}
	else if(_T("등록") == strStatus)
	{
		nMode = REG;
	}
	else return -1;

	if(FALSE == m_bIsInitDataStatic[nMode]) return -1;
	
	if(_T("") == strCaption) strCaption = strData;	



	CArray<DATA_STATIC*, DATA_STATIC*>* pArray = &m_aStaticArray[nMode];

	// 타이틀 스태틱에 데이터 추가
	DATA_STATIC* pDATA_STATIC = (DATA_STATIC*)pArray->GetAt(TITLE_STATIC);
	pDATA_STATIC->m_strData += _T(", ") + strData;
	pDATA_STATIC->m_strData.TrimLeft(_T(", "));
	if(_T("") != strToolTipText)
		pDATA_STATIC->m_strToolTipText = 
			pDATA_STATIC->m_strToolTipText +
			_T("\n") +
			strToolTipText;
	pDATA_STATIC->m_strToolTipText.TrimLeft(_T("\n"));
	pDATA_STATIC->mCtrl_Static.ShowWindow(SW_SHOWNA);


	// 팝업스태틱이 사용중이면(일반 스태틱에 더이상 데이터를 추가 못시키는 경우)
  //	DATA_STATIC* pPopup_Static = (DATA_STATIC*)pArray->GetAt(POPUP_STATIC);
	DATA_STATIC* pNo_3_Static = (DATA_STATIC*)pArray->GetAt(NO_3_STATIC);
 	if(/*TRUE == pPopup_Static->m_bUse || */TRUE == pNo_3_Static->m_bUse)
	{	

/////////////////////////////////////////////////////////////////////////////////
{// 여기서부터 	팝업 스태틱에 맨 처음 데이터가 들어왔을 경우의 처리		
		// 기존에 숨겨져있던 팝업 스태틱을 보여주어야 하는 데,
		// 기존의 차수번호 스태틱이 팝업 스태틱과 겹칠경우를 위해 처리
		// 이 경우 기존의 스태틱을 숨기고, 기존의 스태틱에 있던
		// 데이터를 팝업 스태틱에 추가시킨다.

		DATA_STATIC_EX* pPopup_Static = 
			(DATA_STATIC_EX*)(DATA_STATIC*)	pArray->GetAt(POPUP_STATIC);
		CRect rPopup = pPopup_Static->m_rStaticRect;
		
		for(INT nIndex = NO_1_STATIC; nIndex <= NO_3_STATIC; nIndex++)
		{
			DATA_STATIC* pNo_Static = pArray->GetAt(nIndex);
			CRect rNo = pNo_Static->m_rStaticRect;
			// 기존 차수번호 스태틱의 위치가 팝업 스태티기과 겹칠 때
			if(rNo.right >= rPopup.left - 3)	// -3은 스태틱간의 간격
			{
				// 번호 스태틱을 숨겨버린다
				pNo_Static->mCtrl_Static.MoveWindow(CRect(0, 0, 0, 0));
				pNo_Static->m_rStaticRect = CRect(0, 0, 0, 0);
				
				// 기존 스태틱의 데이터를 팝업스태틱에 추가시킨뒤			
				//DEL 	pPopup_Static->m_strData += _T(", ") + pNo_3_Static->m_strData;
				//DEL 	pPopup_Static->m_strData.TrimLeft(_T(", "));
				pPopup_Static->m_aData.Add(pNo_Static->m_strData);
				pPopup_Static->m_aCaption.Add(pNo_Static->m_strCaption);
				pPopup_Static->m_aToolTipText.Add(pNo_Static->m_strToolTipText);

				pPopup_Static->m_strToolTipText = 
					pPopup_Static->m_strToolTipText + 
					_T("\n") + 
					pNo_Static->m_strToolTipText;
				pPopup_Static->m_strToolTipText.TrimLeft(_T("\n"));
				
				pNo_Static->m_strData = _T("");
				pNo_Static->m_strCaption = _T("");
				pNo_Static->m_strToolTipText = _T("");
				
			}
		}
		// 팝업스태틱을 보여준다
		pPopup_Static->mCtrl_Static.ShowWindow(SW_SHOW);
		pPopup_Static->m_bUse = TRUE;

}//  여기까지 팝업 스태틱에 맨 처음 데이터가 들어왔을 경우의 처리
/////////////////////////////////////////////////////////////////////////////////
		DATA_STATIC_EX* pPopup_Static = (DATA_STATIC_EX*)pArray->GetAt(POPUP_STATIC);
		// 팝업스태틱에 데이터 추가
//DEL 		pPopup_Static->m_strData += _T(", ") + strData;
//DEL 		pPopup_Static->m_strData.TrimLeft(_T(", "));
		pPopup_Static->m_aData.Add(strData);
		pPopup_Static->m_aCaption.Add(strCaption);
		pPopup_Static->m_aToolTipText.Add(strToolTipText);
		if(_T("") != strToolTipText)
			pPopup_Static->m_strToolTipText = 
				pPopup_Static->m_strToolTipText + 
				_T("\n") + 
				strToolTipText;
		pPopup_Static->m_strToolTipText.TrimLeft(_T("\n"));
	}
	// 스태틱에 데이터설정 가능시
	else
	{
		// 스태틱의 위치를 설정하기 위해 이전 스태틱의 위치를 얻는다.		
		for(INT nIndex = NO_1_STATIC; nIndex <= NO_3_STATIC; nIndex++)
		{
			DATA_STATIC* pDATA_STATIC = (DATA_STATIC*)pArray->GetAt(nIndex);
			if(FALSE == pDATA_STATIC->m_bUse) break;
		}
		if(NO_3_STATIC < nIndex) return -1;

		DATA_STATIC* pDATA_STATIC = (DATA_STATIC*)pArray->GetAt(nIndex-1);
		INT nLeft = pDATA_STATIC->m_rStaticRect.right + 5;// 5는 스태틱간의 간격
		INT nTop = pDATA_STATIC->m_rStaticRect.top;		
		INT nBottom = pDATA_STATIC->m_rStaticRect.bottom;
		
		// 설정할 텍스트의 크기를 얻는다(화면 출력용)
		CRect rStatic = CalculateTextSize(strCaption, CRect(nLeft, nTop, nLeft+10, nBottom));

//DEL 		DATA_STATIC_EX* pPopup_Static = 
//DEL 			(DATA_STATIC_EX*)pArray->GetAt(POPUP_STATIC);
//DEL 		// 세번째 차수번호 스태틱의 위치가 팝업 스태티기과 겹칠 때
//DEL 		if(rStatic.right >= pPopup_Static->rStaticRect.left - 3)	//
		// 넓이를 벗어나는지 체크
		if(rStatic.right >= RIGHT)
		{
			// 벗어날 경우 팝업스태틱을 활성화
			DATA_STATIC_EX* pPopup_Static = 
				(DATA_STATIC_EX*)pArray->GetAt(POPUP_STATIC);
			pPopup_Static->m_aCaption.Add(strCaption);
			pPopup_Static->m_aData.Add(strData);
			pPopup_Static->m_aToolTipText.Add(strToolTipText);
			pPopup_Static->mCtrl_Static.ShowWindow(SW_SHOWNA);
 			pPopup_Static->m_bUse = TRUE;			
			if(_T("") != strToolTipText)
				pPopup_Static->m_strToolTipText = 
					pPopup_Static->m_strToolTipText + 
					_T("\n") + 
					strToolTipText;
			pPopup_Static->m_strToolTipText.TrimLeft(_T("\n"));
//DEL 			// 이 이후부터는 팝업스태틱에 추가를 하기위해 
//DEL 			// 숫자 스태틱의 마지막인 no_3_static을 사용중으로 처리한다
//DEL 			pDATA_STATIC = (DATA_STATIC*)pArray->GetAt(NO_3_STATIC);
//DEL 			pDATA_STATIC->m_bUse = TRUE;
		}

		// 벗어나지 않으면 데이터를 설정한다.
		else
		{
			pDATA_STATIC = (DATA_STATIC*)pArray->GetAt(nIndex);
			pDATA_STATIC->mCtrl_Static.SetWindowText(strCaption);
			pDATA_STATIC->mCtrl_Static.MoveWindow(rStatic);
			pDATA_STATIC->m_rStaticRect = rStatic;
			pDATA_STATIC->m_strCaption = strCaption;
			pDATA_STATIC->m_strData = strData;
			pDATA_STATIC->m_strToolTipText = strToolTipText;
			pDATA_STATIC->m_bUse = TRUE;
			pDATA_STATIC->m_nDataSize = CSize(rStatic.Width(), rStatic.Height());

		}
		DATA_STATIC_EX* pPopup_Static = (DATA_STATIC_EX*)pArray->GetAt(POPUP_STATIC);
 		// 세번째 차수번호 스태틱의 위치가 팝업 스태티기과 겹칠 때
 		if(rStatic.right >= pPopup_Static->m_rStaticRect.left - 3)
		{
			// 이 이후부터는 팝업스태틱에 추가를 하기위해 
			// 숫자 스태틱의 마지막인 no_3_static을 사용중으로 처리한다
			pDATA_STATIC = (DATA_STATIC*)pArray->GetAt(NO_3_STATIC);
			pDATA_STATIC->m_bUse = TRUE;
		}
	}
	
	return 0;
}

CRect CDlgSEQ::CalculateTextSize(const CString strText, const CRect rTextRect, CFont* pFont)
{	
	pFont = (NULL == pFont)? GetFont() : pFont;
	// 폰트설정 			
 	CDC* pDC = GetDC(); 	// DC설정	
	CFont* OldFont = pDC->SelectObject(pFont);//DEL (CFont*)pDC->SelectStockObject(DEFAULT_GUI_FONT);
		
	// 계산
	CRect rText = rTextRect;
	pDC->DrawText(strText, &rText, DT_EDITCONTROL | DT_CALCRECT);
	
	pDC->SelectObject(OldFont);
	ReleaseDC(pDC);
	
	return rText;
}

void CDlgSEQ::SetWeekDay()
{
	m_uDayOfTheWeek = WEEKDAY;
}

void CDlgSEQ::SetSunday()
{
	m_uDayOfTheWeek = SUNDAY;
}

void CDlgSEQ::SetSaturday()
{
	m_uDayOfTheWeek = SATURDAY;
}

// void CDlgSEQ::OnPaint() 
// {
//  	CPaintDC dc(this); // device context for painting
//  	
//  	// TODO: Add your message handler code here
//  	CBitmap MemBitmap, Bitmap[2];
//  	CDC DC[2];
//  	
//  	CDC MemDC;
//  	MemDC.CreateCompatibleDC(&dc);
//  	MemBitmap.CreateCompatibleBitmap(&dc, 20, 15);
//  	MemDC.SelectObject(&MemBitmap);
//  
//  	UINT uNum[2] = {0,0};
//  	UINT uTmp = m_uDay;
//  	UINT BitmapDay = IDB_BITMAP_DAY_BLACK_0;
//  	if(SATURDAY == m_uDayOfTheWeek)
//  		BitmapDay = IDB_BITMAP_DAY_BLUE_0;
//  	if(SUNDAY == m_uDayOfTheWeek)
//  		BitmapDay = IDB_BITMAP_DAY_RED_0;
//  
//  	for(UINT i=0;i<2;i++)
//  	{
//  		uNum[1-i] = uTmp % 10;
//  		uTmp = uTmp / 10;
//  	}		
//  
//  	if(0==uNum[0] && 0==uNum[1])
//  	{
//  		for(i=0;i<2;i++)
//  		{
//  			DC[i].CreateCompatibleDC(&dc);	
//  			Bitmap[i].LoadBitmap(IDB_BITMAP_DAY_NULL);		
//  			DC[i].SelectObject(&Bitmap[i]);
//  		}
//  	}
//  	else
//  	{
//  		for(i=0;i<2;i++)
//  		{
//  			DC[i].CreateCompatibleDC(&dc);				
//  			Bitmap[i].LoadBitmap(BitmapDay + uNum[i]);		
//  			DC[i].SelectObject(&Bitmap[i]);
//  		}
//  	}
//  
//  	for(i=0;i<2;i++)
//  	{
//  		MemDC.BitBlt(i*10,0,10, 17, &DC[i],0,0,SRCCOPY);
//  	}	
//  	
//   	CDC* pDC = GetDC();
//  	pDC->BitBlt(0,0,20,15,&MemDC,0,0,SRCCOPY);
//   	ReleaseDC(pDC);
// 	
// 	// Do not call CDialog::OnPaint() for painting messages
// }

// 날짜설정
// 현재 날짜를 리셋하고, 새 날짜로 설정
void CDlgSEQ::SetDay(UINT uYear, UINT uMonth, UINT uDay)
{	

	if(FALSE==ChkDay(uYear, uMonth, uDay))
	{
		CString strTemp;
		strTemp.Format(_T("ERROR : [CDlgSEQ::SetDay] 올바르지 않은 날짜(%d/%d/%d)"), uYear, uMonth, uDay);
		LOG(strTemp);
		return;
	}
		// 날짜의 변경
		m_uYear = uYear;
		m_uMonth = uMonth;
		m_uDay = uDay;	
	
	//  삭제예정
/*	
	INT nMax = rand()%5;
	for(INT nCount=0; nCount < nMax; nCount++)
	{		
		INT nNum = rand()% ( ( ((rand()%2)*100) + ((rand()%10)*10) ) + 1);
		CString strTemp;
		strTemp.Format(_T("%d"), nNum);
		AddData(_T("구입"), strTemp);
	}
	nMax = rand()%5;
	for(nCount=0; nCount < nMax; nCount++)
	{
		INT nNum = rand()% ( ( ((rand()%2)*100) + ((rand()%10)*10) ) + 1);
		CString strTemp;
		strTemp.Format(_T("%d"), nNum);
		AddData(_T("검수"), strTemp);
	}
	nMax = rand()%5;
	for(nCount=0; nCount < nMax; nCount++)
	{
		INT nNum = rand()% ( ( ((rand()%2)*100) + ((rand()%10)*10) ) + 1);
		CString strTemp;
		strTemp.Format(_T("%d"), nNum);
		AddData(_T("미납"), strTemp);
	}
	nMax = rand()%5;
	for(nCount=0; nCount < nMax; nCount++)
	{
		INT nNum = rand()% ( ( ((rand()%2)*100) + ((rand()%10)*10) ) + 1);
		CString strTemp;
		strTemp.Format(_T("%d"), nNum);
		AddData(_T("등록"), strTemp);
	}
*/	//
// 	OnPaint();
	Invalidate();
}


// 함수이름: CDlgSEQ::ResetDay
// 함수설명: 데이터 초기화
// 반환  형: void 
void CDlgSEQ::ResetDay()
{
// 	GetDlgItem(IDC_STATIC_TODAY)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STATIC_HOLIDAY)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STATIC_TODAY_HOLIDAY)->ShowWindow(SW_HIDE);

	m_bIsToday = FALSE;
	m_bIsHoliday = FALSE;

	m_uDay = 0;
	m_uMonth = 0;
	m_uYear = 0;
	m_nArrayIndex_Of_SelectPopupStatic = -1;

	DATA_STATIC* pDATA_STATIC = NULL;
	DATA_STATIC_EX* pDATA_STATIC_EX = NULL;
	for(INT nArrayIndex=0; nArrayIndex < ARRAY_COUNT; nArrayIndex++)
	{
		pDATA_STATIC = (DATA_STATIC*)m_aStaticArray[nArrayIndex].GetAt(TITLE_STATIC);
		pDATA_STATIC->m_strData = _T("");
		pDATA_STATIC->m_strToolTipText = _T("");
		pDATA_STATIC->m_nDataSize = CSize(0, 0);
		pDATA_STATIC->mCtrl_Static.SetFont(m_pOldFont);
		pDATA_STATIC->mCtrl_Static.ShowWindow(SW_HIDE);

		pDATA_STATIC_EX = 
			(DATA_STATIC_EX*)m_aStaticArray[nArrayIndex].GetAt(POPUP_STATIC);
		pDATA_STATIC_EX->m_strData = _T("");
		pDATA_STATIC_EX->m_nDataSize = CSize(0, 0);
		pDATA_STATIC_EX->m_strToolTipText = _T("");		
		pDATA_STATIC_EX->mCtrl_Static.ShowWindow(SW_HIDE);
		pDATA_STATIC_EX->mCtrl_Static.SetFont(m_pOldFont);
		pDATA_STATIC_EX->m_bUse = FALSE;

		pDATA_STATIC_EX->m_aCaption.RemoveAll();
		pDATA_STATIC_EX->m_aData.RemoveAll();
		pDATA_STATIC_EX->m_aToolTipText.RemoveAll();

		for(INT nIndex=NO_1_STATIC; nIndex <= NO_3_STATIC; nIndex++)
		{
			pDATA_STATIC = (DATA_STATIC*)m_aStaticArray[nArrayIndex].GetAt(nIndex);
			pDATA_STATIC->m_bUse = FALSE;
			pDATA_STATIC->m_nDataSize = CSize(0, 0);			
			pDATA_STATIC->m_rStaticRect = CRect(0, 0, 0, 0);
			pDATA_STATIC->m_strCaption = _T("");
			pDATA_STATIC->m_strData = _T("");
			pDATA_STATIC->m_strToolTipText = _T("");
			
			pDATA_STATIC->mCtrl_Static.MoveWindow(CRect(0, 0, 0, 0));
			pDATA_STATIC->mCtrl_Static.SetWindowText(_T(""));
			pDATA_STATIC->mCtrl_Static.SetFont(m_pOldFont);
		}
	}
	m_pSelect_Data_Static = NULL;
	m_pPrev_Data_Static = NULL;

	Invalidate();
}

void CDlgSEQ::SetToday()
{
// 	GetDlgItem(IDC_STATIC_TODAY)->ShowWindow(SW_SHOW);	
	m_bIsToday = TRUE;
}


void CDlgSEQ::SetHoliday()
{
// 	if(!IsToday())
// 	{
// 		GetDlgItem(IDC_STATIC_HOLIDAY)->ShowWindow(SW_SHOW);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_STATIC_TODAY)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_TODAY_HOLIDAY)->ShowWindow(SW_SHOW);
// 	}
	m_bIsHoliday = TRUE;		
}

BOOL CDlgSEQ::IsToday() const
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

INT CDlgSEQ::Get_StaticInMouse(DATA_STATIC** pGet_Data)
{
	if(NULL == pGet_Data) return -1;
	DATA_STATIC* pDATA_STATIC = Get_StaticInMouse();
	if(NULL == pDATA_STATIC) return -2;
	*pGet_Data = pDATA_STATIC;

	return 0;
}

CDlgSEQ::DATA_STATIC* CDlgSEQ::Get_StaticInMouse()
{
	INT nArrayIndex = 0, nIndex = 0;
	INT nResult = Find_StaticIndex_InMouse(&nArrayIndex, &nIndex);
	if(0 > nResult) return NULL;

	DATA_STATIC* pDATA_STATIC =
		(DATA_STATIC*)m_aStaticArray[nArrayIndex].GetAt(nIndex);
	
	return pDATA_STATIC;
}


INT CDlgSEQ::Init_DataStatic()
{
	{
	CRect rStaticTitle = CRect(3, ACQ_TOP, 10, ACQ_BOTTOM);	
	rStaticTitle = CalculateTextSize(_T("구입:"), rStaticTitle);
		
	CRect rStaticPopup = CRect(3, ACQ_TOP, 10, ACQ_BOTTOM);
	rStaticPopup = CalculateTextSize(_T("▶"), rStaticPopup);
	const INT WIDTH = rStaticPopup.Width();
	rStaticPopup.right = RIGHT;
	rStaticPopup.left = RIGHT - WIDTH;
	
	INT nResult = Init_DataStatic(ACQ, _T("구입:"), rStaticTitle, rStaticPopup);
	if(0 <= nResult) { m_bIsInitDataStatic[ACQ] = TRUE; }
	}

	{
	CRect rStaticTitle = CRect(3, CHECKIN_TOP, 10, CHECKIN_BOTTOM);	
	rStaticTitle = CalculateTextSize(_T("검수:"), rStaticTitle);
		
	CRect rStaticPopup = CRect(3, CHECKIN_TOP, 10, CHECKIN_BOTTOM);
	rStaticPopup = CalculateTextSize(_T("▶"), rStaticPopup);
	const INT WIDTH = rStaticPopup.Width();
	rStaticPopup.right = RIGHT;
	rStaticPopup.left = RIGHT - WIDTH;
	
	INT nResult = Init_DataStatic(CHECKIN, _T("검수:"), rStaticTitle, rStaticPopup);
	if(0 <= nResult) { m_bIsInitDataStatic[CHECKIN] = TRUE; }
	}

	{
	CRect rStaticTitle = CRect(3, MISSING_TOP, 10, MISSING_TOP);	
	rStaticTitle = CalculateTextSize(_T("미납:"), rStaticTitle);
		
	CRect rStaticPopup = CRect(3, MISSING_TOP, 10, MISSING_TOP);
	rStaticPopup = CalculateTextSize(_T("▶"), rStaticPopup);
	const INT WIDTH = rStaticPopup.Width();
	rStaticPopup.right = RIGHT;
	rStaticPopup.left = RIGHT - WIDTH;
	
	INT nResult = Init_DataStatic(MISSING, _T("미납:"), rStaticTitle, rStaticPopup);
	if(0 <= nResult) { m_bIsInitDataStatic[MISSING] = TRUE; }
	}

	
	{
	CRect rStaticTitle = CRect(3, REG_TOP, 10, REG_BOTTOM);	
	rStaticTitle = CalculateTextSize(_T("등록:"), rStaticTitle);
		
	CRect rStaticPopup = CRect(3, REG_TOP, 10, REG_BOTTOM);
	rStaticPopup = CalculateTextSize(_T("▶"), rStaticPopup);
	const INT WIDTH = rStaticPopup.Width();
	rStaticPopup.right = RIGHT;
	rStaticPopup.left = RIGHT - WIDTH;
	
	INT nResult = Init_DataStatic(REG, _T("등록:"), rStaticTitle, rStaticPopup);
	if(0 <= nResult) { m_bIsInitDataStatic[REG] = TRUE; }
	}

	return 0;
}



// 함수이름: CDlgSEQ::Init_DataStatic
// 함수설명: 스태틱을 동적으로 생성한다
// 예>
//		[타이틀][차수1][차수2][차수3][팝업]
//		[구입: ][ 12  ][ 15  ][ 20   ][▶  ]
// 반환  형: INT 
// 인자[INT nArrayIndex] : 인덱스의 종류(구입ACQ)
// 인자[CString strCaption] : 화면에 보여질 텍스트
// 인자[CRect rStaticTitle] : 타이틀스태틱의 크기
// 인자[CRect rStaticPopup] : 팝업스태틱의 크기
INT CDlgSEQ::Init_DataStatic(INT nArrayIndex, CString strCaption, CRect rStaticTitle, CRect rStaticPopup)
{
	if(ACQ != nArrayIndex &&
		CHECKIN != nArrayIndex &&
		MISSING != nArrayIndex &&
		REG !=	nArrayIndex
		) return -1;
	
	// 배열에 담을 크기는 5개
	m_aStaticArray[nArrayIndex].SetSize(5);

	// 타이틀 스태틱
	DATA_STATIC* pDATA_STATIC = New_DataStatic(strCaption, _T(""), rStaticTitle);
	pDATA_STATIC->m_nArrayIndex = nArrayIndex;	
	pDATA_STATIC->m_nDataIndex = TITLE_STATIC;
	m_aStaticArray[nArrayIndex].SetAt(TITLE_STATIC, pDATA_STATIC);
	
	// 팝업 스태틱
//DEL 	pDATA_STATIC = New_DataStatic_Ex(_T(">>"), _T(""), rStaticPopup);
	pDATA_STATIC = New_DataStatic_Ex(_T("▶"), _T(""), rStaticPopup);
	pDATA_STATIC->m_nArrayIndex = nArrayIndex;
	pDATA_STATIC->m_nDataIndex = POPUP_STATIC;
	pDATA_STATIC->mCtrl_Static.ShowWindow(SW_HIDE);
	m_aStaticArray[nArrayIndex].SetAt(POPUP_STATIC, pDATA_STATIC);
	

	// 3은 차수스태틱의 개수
	for(INT nIndex=NO_1_STATIC; nIndex <= NO_3_STATIC; nIndex++)
	{
		pDATA_STATIC = New_DataStatic(_T(""), _T(""), CRect(0,0,0,0));
		pDATA_STATIC->m_bUse = FALSE;
		pDATA_STATIC->m_nArrayIndex = nArrayIndex;
		pDATA_STATIC->m_nDataIndex = nIndex;
		m_aStaticArray[nArrayIndex].SetAt(nIndex, pDATA_STATIC);		
	}
	return 0;
}


// 함수이름: CDlgSEQ::New_DataStatic
// 함수설명: 데이터 스태틱 동적 생성
// 반환  형: CDlgSEQ::DATA_STATIC* 
// 인자[CString strCaption] : 
// 인자[CString strData] : 
// 인자[CRect rStatic] : 
// 인자[CFont *pFont] : 
CDlgSEQ::DATA_STATIC* CDlgSEQ::New_DataStatic(CString strCaption, CString strData, CRect rStatic, CFont *pFont)
{
	DATA_STATIC* pDATA_STATIC = new DATA_STATIC;
	
	pFont = (NULL == pFont)? GetFont() : pFont;

	pDATA_STATIC->m_strData = strData;
	pDATA_STATIC->mCtrl_Static.Create(strCaption, WS_VISIBLE, rStatic, this);
	pDATA_STATIC->mCtrl_Static.SetFont(pFont);
	pDATA_STATIC->m_nDataSize = CSize(rStatic.Width(), rStatic.Height());
	pDATA_STATIC->m_rStaticRect = rStatic;
	pDATA_STATIC->m_bUse = TRUE;

	return pDATA_STATIC;
}

CDlgSEQ::DATA_STATIC_EX* CDlgSEQ::New_DataStatic_Ex(CString strCaption, CString strData, CRect rStatic, CFont *pFont)
{
	DATA_STATIC_EX* pDATA_STATIC = new DATA_STATIC_EX;
	
	pFont = (NULL == pFont)? GetFont() : pFont;

	pDATA_STATIC->m_strData = strData;
	pDATA_STATIC->mCtrl_Static.Create(strCaption, WS_VISIBLE, rStatic, this);
	pDATA_STATIC->mCtrl_Static.SetFont(pFont);
	pDATA_STATIC->m_nDataSize = CSize(rStatic.Width(), rStatic.Height());
	pDATA_STATIC->m_rStaticRect = rStatic;
	pDATA_STATIC->m_bUse = TRUE;

	return pDATA_STATIC;	
}


// 함수이름: CDlgSEQ::RemoveAll_DataStatic
// 함수설명: 데이터스태틱 메모리 해제
// 반환  형: INT 
INT CDlgSEQ::RemoveAll_DataStatic()
{
	DATA_STATIC* pDATA_STATIC = NULL;	

	for(INT nArrayCount = 0; nArrayCount < ARRAY_COUNT; nArrayCount++)
	{
		for(INT nRemainCount = m_aStaticArray[nArrayCount].GetSize()-1; 0 <= nRemainCount ; nRemainCount--)
		{
			pDATA_STATIC = (DATA_STATIC*)m_aStaticArray[nArrayCount].GetAt(nRemainCount);
			if(NULL != pDATA_STATIC)
			{
				// 팝업스태틱은 확장 데이터 스태틱형식이다
				if(POPUP_STATIC == nRemainCount)
				{
					DATA_STATIC_EX* pDATA_STATIC_EX = 
						(DATA_STATIC_EX*)m_aStaticArray[nArrayCount].GetAt(nRemainCount);
					pDATA_STATIC_EX->m_aCaption.RemoveAll();
					pDATA_STATIC_EX->m_aData.RemoveAll();
					pDATA_STATIC_EX->m_aToolTipText.RemoveAll();

					delete pDATA_STATIC_EX;
				}
				else
				{
					delete pDATA_STATIC;
				}
			}		
		}
		m_aStaticArray[nArrayCount].RemoveAll();
	}

	return 0;
}

// 함수이름: CDlgSEQ::Find_StaticIndex_InMouse
// 함수설명: 현재 마우스가 가르키는 스태틱데이터의 인덱스를 얻어온다.
// 반환  형: INT 
// 인자[INT *nArrayIndex] : 
// 인자[INT *nIndex] : 
INT CDlgSEQ::Find_StaticIndex_InMouse(INT *nArrayIndex, INT *nIndex)
{	
	if(NULL == nArrayIndex)	return -1;
	if(NULL == nIndex)	return -2;

	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	
	// 현재 마우스의 위치가
	// 구입, 검수, 미납, 등록 스태틱영역중 어느 배열에 속해있는지 판단
	CRect r[ARRAY_COUNT];
	r[ACQ] =	CRect(LEFT, ACQ_TOP, RIGHT, ACQ_BOTTOM);
	r[CHECKIN] =	CRect(LEFT, CHECKIN_TOP, RIGHT, CHECKIN_BOTTOM);
	r[MISSING] =	CRect(LEFT, MISSING_TOP, RIGHT, MISSING_BOTTOM);
	r[REG] =	CRect(LEFT, REG_TOP, RIGHT, REG_BOTTOM);
	
	for(INT nCount = 0; nCount < ARRAY_COUNT; nCount++)
	{
		if(TRUE == r[nCount].PtInRect(point)) break;
	}
	if(ARRAY_COUNT <= nCount) return -1;
		
	INT nArray = nCount;

	DATA_STATIC* pDATA_STATIC = NULL;
	CRect rCompare;	
	
	INT nMAX = m_aStaticArray[nArray].GetSize();
	for(INT nDataCount=0; nDataCount < nMAX; nDataCount++)
	{
		pDATA_STATIC = (DATA_STATIC*)m_aStaticArray[nArray].GetAt(nDataCount);
		rCompare = pDATA_STATIC->m_rStaticRect;
		if(TRUE == rCompare.PtInRect(point))
		{	
			// 팝업스태틱/타이틀 스태틱의 경우 기본적으로 숨김 상태인 데,
			// 이때는 없는 것으로 간주한다
			if(TITLE_STATIC == nDataCount)
			{
				return -1;
			}
			else if(POPUP_STATIC == nDataCount)
			{
				if(SW_HIDE == pDATA_STATIC->mCtrl_Static.IsWindowVisible())
				{
					continue;
				}				
			}			
			*nArrayIndex	= nArray;
			*nIndex		= nDataCount;				
			return 0;
		}
	}

	*nArrayIndex	= -1;
	*nIndex		= -1;

	return -2;
}

void CDlgSEQ::OnButtonPopupStatic(DATA_STATIC_EX *pPopup_static, INT nArrayIndex, INT nDataIndex)
{
//DEL 	CStringArray aData;

	if(NULL == pPopup_static) return ;
	if(POPUP_STATIC != pPopup_static->m_nDataIndex) return ;
	m_nArrayIndex_Of_SelectPopupStatic = pPopup_static->m_nArrayIndex;

	const nMAX = pPopup_static->m_aCaption.GetSize();
	if(0 >= nMAX) return ;

	CString strMsg = _T("AUTO_HIDE:FALSE");
	GetParent()->SendMessage(WM_REQUEST_KALENDARDLG, IDD, (LPARAM)&strMsg);
	/////////////////////////////////////////////////////////////////////////
	{// 여기서부터 팝업메뉴 띄우기
	CMenu  *sMenu,*pMenu;
	pMenu = new CMenu;
	pMenu->LoadMenu(IDR_MENU_DIVISION_NO);		
  	sMenu = pMenu->GetSubMenu(0);	
 	sMenu->ModifyMenu(0, MF_BYPOSITION, ID_DIVISION_NO1, pPopup_static->m_aCaption.GetAt(0));
	for(INT nCount = 1, nID = ID_DIVISION_NO2; nCount < nMAX; nCount++, nID++)
	{		
		sMenu->InsertMenu(nCount, MF_BYPOSITION, nID, pPopup_static->m_aCaption.GetAt(nCount));
	}
	// 메뉴를 띄우기
	CRect rect = pPopup_static->m_rStaticRect;
	ClientToScreen(rect);

	// 팝업메뉴의 오른쪽끝이 화면을 벗어나는 경우를 방지하기 위해
	// 칼렌다가 오른쪽끝에 있을때의 팝업메뉴의 위치를 다르게 한다.
	CRect rDlg;
	GetWindowRect(rDlg);
	const LONG cxScreen = GetSystemMetrics(SM_CXSCREEN);
	
	if(cxScreen > rDlg.right +100)	// 100은 임시 숫자
	{				// 팝업크기를 얻을 방법을 모르겠음
		sMenu->TrackPopupMenu(
			TPM_RIGHTBUTTON|TPM_LEFTALIGN,
			rect.right,
			rect.top,
			this
			);
	}
	else
	{	// 칼렌다가 오른쪽끝에 있을 경우
		sMenu->TrackPopupMenu(
			TPM_RIGHTBUTTON|TPM_RIGHTALIGN,
			rect.left,
			rect.top,
			this
			);
	}
	delete pMenu;	
	}// 여기까지 팝업메뉴 띄우기
	/////////////////////////////////////////////////////////////////////////

}

INT CDlgSEQ::Reset_SelectStatic()
{
	if(NULL != m_pSelect_Data_Static)
		m_pSelect_Data_Static->mCtrl_Static.SetFont(m_pOldFont);

	return 0;
}

INT CDlgSEQ::Get_SelectData(CData *pData) const
{
//DEL 	if(NULL == m_pSelect_Data_Static) return -1;
//DEL 
//DEL 	CString strDataType = _T("");
//DEL 	switch(m_pSelect_Data_Static->m_nArrayIndex)
//DEL 	{
//DEL 	case ACQ:
//DEL 		strDataType = _T("구입:");
//DEL 		break;
//DEL 	case CHECKIN:
//DEL 		strDataType = _T("검수:");
//DEL 		break;
//DEL 	case MISSING:
//DEL 		strDataType = _T("미납:");
//DEL 		break;
//DEL 	case REG:
//DEL 		strDataType = _T("등록:");
//DEL 		break;	
//DEL 	}
//DEL 	pData->SetData(
//DEL 		m_uYear,
//DEL 		m_uMonth,
//DEL 		m_uDay,
//DEL 		strDataType + m_pSelect_Data_Static->m_strData
//DEL 		);
	return Get_Data(m_pSelect_Data_Static, pData);	
}

void CDlgSEQ::OnTimer(UINT nIDEvent) 
{	
	// TODO: Add your message handler code here and/or call default
	// 마우스가 현재 다이어로그영역을 벗어날 경우를 대비
	if(nIDEvent = ID_TIMER_FOR_MOUSE_WITHOUT_DLGSEQ_CLIENT)
	{
		if(NULL == Get_StaticInMouse())
		{
 			KillTimer(ID_TIMER_FOR_MOUSE_WITHOUT_DLGSEQ_CLIENT);

			SetCursor(LoadCursor(NULL, IDC_ARROW));
			if(NULL != m_pSelect_Data_Static)
			{
				m_pSelect_Data_Static->mCtrl_Static.SetFont(m_pOldFont);
				m_pSelect_Data_Static = NULL;
			}	
			if(NULL != m_pPrev_Data_Static)
			{	
				m_pPrev_Data_Static->mCtrl_Static.SetFont(m_pOldFont);
				m_pPrev_Data_Static = NULL;
			}
		}		
	}
	CDialog::OnTimer(nIDEvent);
}


BOOL CDlgSEQ::IsDay() const
{	
	if(0>=m_uDay) 
		return FALSE;	
	return TRUE;
}


// 함수이름: CDlgSEQ::OnButtonPopupMenu
// 함수설명: 사용자가 팝업스태틱의 메뉴를 선택한 경우 
// 사용자가 선택한 ID에 해당하는 차수번호를 팝업스태틱으로부터 알아낸다음
// 해당 차수번호가 선택되었음을 CKalendarDlg에 전달한다
// 반환  형: void 
// 인자[UINT nID] : 
void CDlgSEQ::OnButtonPopupMenu(UINT nID)
{
	m_ToolTip.Hide();

	// 1. 팝업스태틱이 어느 차수상태에 해당하는지 알아낸다음
	INT nArrayIndex = -1;
	switch(m_nArrayIndex_Of_SelectPopupStatic)
	{	
	case ACQ:
	case CHECKIN:
	case MISSING:
	case REG:
		nArrayIndex = m_nArrayIndex_Of_SelectPopupStatic;
		break;
	default:
		return;
		break;
	}

	// 2. 해당 팝업스태틱의 데이터를 얻어온다.
//DEL 	DATA_STATIC* pDATA_STATIC = m_aStaticArray[nArrayIndex].GetAt(POPUP_STATIC);

	// 3. 이후 사용자가 선택한 아이디에 해당하는 차수번호를 얻는다.	
//DEL 	CString strData = pDATA_STATIC->m_strData;
//DEL 	strData.TrimRight(_T(", "));
//DEL 	strData += _T(", ");
//DEL 
//DEL 	INT nIndex = 0;
//DEL 	INT nStart = 0;
//DEL 	CString strDivisionNo = _T("");

//DEL 	for(UINT nFindID = ID_DIVISION_NO1; nFindIndex <= nID; nFindIndex++)
//DEL 	{
//DEL  		nIndex = strData.Find(_T(", "), nStart);
//DEL  		if(nFindIndex == nID)
//DEL  			strDivisionNo = strData.Mid(nStart, nIndex - nStart);
//DEL  		nStart = nIndex+2;
//DEL  	}
	

//DEL 	// 4. KalendarDlg에 전달한다	
//DEL 	CString strBackup = pDATA_STATIC->m_strData;
//DEL 	pDATA_STATIC->m_strData = strDivisionNo;
	
	CData* pData = new CData;
	INT nIndex = nID - ID_DIVISION_NO1;
	DATA_STATIC_EX* pPopup_Static = 
		(DATA_STATIC_EX*)m_aStaticArray[nArrayIndex].GetAt(POPUP_STATIC);

	if(0 <= Get_Data_For_PopupStatic(pPopup_Static, pData, nIndex))
	{	
		GetParent()->SendMessage(WM_NOTIFY, IDD, (LPARAM)pData);
	}	
//DEL 	pDATA_STATIC->m_strData = strBackup;
	delete pData;
}

INT CDlgSEQ::Get_Data(const DATA_STATIC *pPDATA_STATIC, CData *pGetData) const
{	
	if(NULL == pPDATA_STATIC) return -1;
	
	CString strDataType = _T("");
	switch(pPDATA_STATIC->m_nArrayIndex)
	{
	case ACQ:
		strDataType = _T("구입:");
		break;
	case CHECKIN:
		strDataType = _T("검수:");
		break;
	case MISSING:
		strDataType = _T("미납:");
		break;
	case REG:
		strDataType = _T("등록:");
		break;	
	}
	if(_T("") == pPDATA_STATIC->m_strData) return -1;

	pGetData->SetData(
		m_uYear,
		m_uMonth,
		m_uDay,
		strDataType + pPDATA_STATIC->m_strData
		);

	return 0;
}

INT CDlgSEQ::Get_Data_For_PopupStatic(const DATA_STATIC_EX* pDATA_STATIC_EX, CData* pGetData, INT nIndex) const
{
	if(NULL == pDATA_STATIC_EX) return -1;
	
	CString strDataType = _T("");
	switch(pDATA_STATIC_EX->m_nArrayIndex)
	{
	case ACQ:
		strDataType = _T("구입:");
		break;
	case CHECKIN:
		strDataType = _T("검수:");
		break;
	case MISSING:
		strDataType = _T("미납:");
		break;
	case REG:
		strDataType = _T("등록:");
		break;	
	}

	pGetData->SetData(
		m_uYear,
		m_uMonth,
		m_uDay,
		strDataType + pDATA_STATIC_EX->m_aData.GetAt(nIndex)
		);

	return 0;
}

BOOL CDlgSEQ::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::DestroyWindow();
}

void CDlgSEQ::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu) 
{
 	CDialog::OnMenuSelect(nItemID, nFlags, hSysMenu);
 
 	INT nArrayIndex = m_nArrayIndex_Of_SelectPopupStatic;
 	if(0 > nArrayIndex) return;
 
 	DATA_STATIC_EX* pPopup_static = 
 		(DATA_STATIC_EX*)m_aStaticArray[nArrayIndex].GetAt(POPUP_STATIC);
 
	// ITEM ID는 ID_DIVISION_NO1부터 시작된다
 	INT nIndex = nItemID - ID_DIVISION_NO1;
	if(0 > nIndex) return;

 	m_strToolTipText_For_Mouse_In_the_PopupMenu = pPopup_static->m_aToolTipText.GetAt(nIndex);

	// TODO: Add your message handler code here
	
}

void CDlgSEQ::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnMouseMove(nFlags, point);
}

// 함수이름: CDlgSEQ::OnEnterIdle
// 함수설명: 모달 다이어로그나 메뉴가 팝업된 상태일시 들어오는 메시지
// 반환  형: void 
// 인자[UINT nWhy] : MSGF_DIALOGBOX 모달, MSGF_MENU 메뉴
// 인자[CWnd* pWho] : 해당 윈도우
void CDlgSEQ::OnEnterIdle(UINT nWhy, CWnd* pWho) 
{
	CDialog::OnEnterIdle(nWhy, pWho);
		
	// TODO: Add your message handler code here
	if(MSGF_MENU == nWhy)
	{		
		CRect r;
		pWho->GetWindowRect(r);
	
		CPoint point;
		GetCursorPos(&point);	

		if(TRUE == r.PtInRect(point))
		{		
 			if(_T("") != m_strToolTipText_For_Mouse_In_the_PopupMenu)
 			{				
 				m_ToolTip.Display(
 					point, 
 					CPoint(r.right+4, point.y), 
 					m_strToolTipText_For_Mouse_In_the_PopupMenu
 					); 				
//DEL 				pWho->SetWindowPos(
//DEL  					&m_ToolTip,
//DEL  					0,
//DEL  					0,
//DEL  					0,
//DEL  					0,
//DEL  					SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW
//DEL  					);
  			}
		}		
		else m_ToolTip.Hide();
	}
	
}

LRESULT CDlgSEQ::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_EXITMENULOOP == message)
	{
		CString strMsg = _T("AUTO_HIDE:TRUE");
		GetParent()->SendMessage(WM_REQUEST_KALENDARDLG, IDD, (LPARAM)&strMsg);		
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgSEQ::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
 	INT nArrayIndex = 0, nIndex = 0;
 	if(0 <= Find_StaticIndex_InMouse(&nArrayIndex, &nIndex))
 	{
 		if(POPUP_STATIC == nIndex)
 		{
 			DATA_STATIC_EX *pDATA_STATIC = (DATA_STATIC_EX*)m_aStaticArray[nArrayIndex].GetAt(POPUP_STATIC);
 			OnButtonPopupStatic(pDATA_STATIC, nArrayIndex, nIndex);			
 		}
 		else
 		{		
 			m_ToolTip.Hide();
 			m_nArrayIndex_Of_SelectPopupStatic = -1;
 
 
 			// 선택된 데이터 전송
 			CData* pData = new CData;
 			Get_SelectData(pData);
 			GetParent()->SendMessage(WM_NOTIFY, IDD, (LPARAM)pData);
 			delete pData;
 		}
 	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

BOOL CDlgSEQ::OnEraseBkgnd(CDC* pDC) 
{
// 	CDialog::OnEraseBkgnd(pDC);

	
// 	CRect rRect;
// 	GetClientRect(&rRect);
// 
// 	CBitmap MemBitmap, Bitmap[2];
// 	CDC DC[2];
// 	
// 	CDC MemDC;
// 	MemDC.CreateCompatibleDC(pDC);
// 	MemBitmap.CreateCompatibleBitmap(pDC, rRect.Width(), rRect.Height());
// 	MemDC.SelectObject(&MemBitmap);
// 
// 	UINT uNum[2] = {0,0};
// 	UINT uTmp = m_uDay;
// 	UINT BitmapDay = IDB_BITMAP_DAY_BLACK_0;
// 	if(SATURDAY == m_uDayOfTheWeek)
// 		BitmapDay = IDB_BITMAP_DAY_BLUE_0;
// 	if(SUNDAY == m_uDayOfTheWeek)
// 		BitmapDay = IDB_BITMAP_DAY_RED_0;
// 
// 	for(UINT i=0;i<2;i++)
// 	{
// 		uNum[1-i] = uTmp % 10;
// 		uTmp = uTmp / 10;
// 	}		
// 
// 	if(0==uNum[0] && 0==uNum[1])
// 	{
// 		for(i=0;i<2;i++)
// 		{
// 			DC[i].CreateCompatibleDC(pDC);	
// 			Bitmap[i].LoadBitmap(IDB_BITMAP_DAY_NULL);		
// 			DC[i].SelectObject(&Bitmap[i]);
// 		}
// 	}
// 	else
// 	{
// 		for(i=0;i<2;i++)
// 		{
// 			DC[i].CreateCompatibleDC(pDC);				
// 			Bitmap[i].LoadBitmap(BitmapDay + uNum[i]);		
// 			DC[i].SelectObject(&Bitmap[i]);
// 		}
// 	}
// 
// 	MemDC.FillSolidRect(0, 0, rRect.Width(), rRect.Height(), RGB(255,255,255));
// 	for(i=0;i<2;i++)
// 	{
// 		MemDC.BitBlt(i*10,0,10, 17, &DC[i],0,0,SRCCOPY);
// 	}	
//  	pDC->BitBlt(0, 0, 20, 15, &MemDC, rRect.Width(), rRect.Height(), SRCCOPY);

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

INT CDlgSEQ::InitBG()
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