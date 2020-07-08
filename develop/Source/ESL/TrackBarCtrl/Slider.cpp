// Slider.cpp : implementation file
//

#include "stdafx.h"
#include "Slider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSlider

CSlider::CSlider()
{
	//슬라이더 바 위에 올라갈 버튼의 위치 조정
	m_nPosPercent = 0;
	nButtonPosX = 1;
	nButtonPosY = 1;
	// KOL.UDF.022 시큐어코딩 보완
	m_pParent = NULL;
	m_pButton = NULL;
	m_nLeftBound = 0;
	m_nRightBound = 0;
	m_rectWndPos = NULL;
	bgRect = NULL;
	mainDlgRect = NULL;
	INT buttonWidth;
	
	CString m_strAlias;

}

CSlider::~CSlider()
{
	Save();
	if( m_pButton != NULL )
	{
		delete m_pButton;
		m_pButton = NULL;
	}
}

BEGIN_MESSAGE_MAP(CSlider, CButton)
	//{{AFX_MSG_MAP(CSlider)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(UM_SLIDERBUTTONMOVED,OnButtonMoved)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSlider message handlers

void CSlider::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	
}

BOOL CSlider::Create(CWnd *pParent,INT nWndPosX,INT nWndPosY,CString alias) 
{
	// 부모 윈도의 포인터 잡는다
	m_pParent		= pParent;
	
	// 배경을 로드한다.
	m_picBg.Load(_T("..\\BMP\\slider_bg.bmp"));
	m_rectWndPos.left	= nWndPosX;
	m_rectWndPos.top	= nWndPosY;
	m_rectWndPos.right	= nWndPosX + m_picBg.m_Width;
	m_rectWndPos.bottom	= nWndPosY + m_picBg.m_Height;
	
	
	// 퍼센트 계산 하기 위해 바운더리를 계산
	// picTemp 는 버튼의 폭이 얼마나 되는지 알기위해 임시로 사용 (추후 수정?)
	
	CPicture	picTemp;
	picTemp.Load(_T("..\\BMP\\slider_buttonon.bmp"));
	buttonWidth = picTemp.m_Width;
	
	//왼쪽과 오른쪽 경계 설정
	m_nLeftBound	= nButtonPosX;
	m_nRightBound	= m_nLeftBound+(m_picBg.m_Width-(m_nLeftBound*2));
	
	// 우선 배경이 되는 부분의 윈도를 생성
	CButton::Create(_T("slider"),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,m_rectWndPos,m_pParent,20001);
	
	// 슬라이더 버튼 생성
	m_pButton = new CSliderButton;
	m_pButton->Create(this,nButtonPosX,nButtonPosY,m_nRightBound);
	
	////I/O//////////////////////////////////////////////////////////////////////////////
	
	if ( alias == _T("") )
	{
		m_strAlias = _T("common");
	}
	else
	{
		m_strAlias = alias;
	}
	
	
	CString filename;		
	filename.Format(_T("..\\cfg\\Transparency\\%s.cfg"), m_strAlias);				
	
	FILE *fr;
	fr = _tfopen(filename.GetBuffer(0), _T("rb"));
	if(fr != NULL)
	{
		// KOL.UDF.022 시큐어코딩 보완
		//if (!fr) //해당 파일이 없다면
		if (&fr == NULL) //해당 파일이 없다면
		{
			FILE *fw;
			fw = _tfopen(filename.GetBuffer(0), _T("wb"));
			if (fw != NULL) 
			{
				fclose(fw);
			}				
		}
		else //해당파일이 있다면
		{
			CString strContents,temp;
			int nPos;
			TCHAR szBuf[128];
			_fgetts(szBuf, 127, fr);
			
			strContents.Format(_T("%s"), szBuf);
			nPos=strContents.Find(':',0);
			temp=strContents.Mid(nPos+1,3);
			CString getStr = temp.GetAt(2);
			
			if(getStr == ']')// 100이 아닌값
			{
				// 100이 아닌값
				temp = strContents.Mid(nPos+1,2);
				SetPos(_ttoi(temp));
				
			}
			else if(getStr == '0')// 100이면
			{
				// 100
				temp = strContents.Mid(nPos+1,3);
				SetPos(_ttoi(temp));
			}
			
		}
		fclose(fr);
	}
	else
	{
		SetMaxPos();
	}
	
	//버튼 포지션이 최소값일 때 OnbuttonMoved 함수가 불리지 않아 
	//메시지를 부모 다이얼로그로 보낼 수 없으므로 추가
	m_pParent->SendMessage(UM_SLIDERMOVED,(WPARAM)0,(LPARAM)m_nPosPercent+20);
	
	return TRUE;
}

BOOL CSlider::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	// 배경 그려주고.
	m_picBg.Show(pDC,CRect(0,0,m_picBg.m_Width,m_picBg.m_Height));

	return TRUE;
}

LONG CSlider::OnButtonMoved(WPARAM wParam, LPARAM lParam)
{
	INT nButtonPos = (INT)wParam;
	
	// 버튼이 움직인 거리 / 슬라이더 바의 길이 * 80
	// 다이얼로그가 완전히 사라지는것을 막기 위해 80단위로변환
	float a,b,c;
	a = (float)(nButtonPos-m_nLeftBound);
	b = (float)(m_nRightBound-m_nLeftBound-buttonWidth);
	c = ( a / b * 80);
	m_nPosPercent = (INT)c;
	//	TRACE(_T("%d\n"),m_nPosPercent);
	
	if(m_nPosPercent<0)
	{
		m_nPosPercent = 0;
	}
	else if(m_nPosPercent>80)
	{
		m_nPosPercent = 80;
	}
	
	Invalidate();
	UpdateWindow();
		
	m_pParent->SendMessage(UM_SLIDERMOVED,(WPARAM)0,(LPARAM)m_nPosPercent+20);
	
	
	return 0;
}

void CSlider::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//m_pButton
	
	CPoint n;
	GetCursorPos(&n);
	ScreenToClient(&n);
	
	// 슬라이더 바 경계값 사이에 클릭시 해당 마우스 포인터를 중심으로 버튼 이동
	if(n.x>=m_nLeftBound+(buttonWidth/2) && n.x<=m_nRightBound-(buttonWidth/2))
	{
		m_pButton->m_rectButtonPos.left = n.x-(buttonWidth/2);
		m_pButton->m_rectButtonPos.right = n.x + (buttonWidth/2);
		m_pButton->MoveWindow(&(m_pButton->m_rectButtonPos));
		
		Invalidate();
		UpdateWindow();
	}
	
	// 그렇지 않으면( 경계값을 넘으면 ) 가장 끝 위치로 버튼 이동
	else
	{
		if(n.x<m_nLeftBound+(buttonWidth/2))
		{
			m_pButton->m_rectButtonPos.left = m_nLeftBound;
			m_pButton->m_rectButtonPos.right=m_nLeftBound+buttonWidth;
			m_pButton->MoveWindow(&(m_pButton->m_rectButtonPos));
		}
		else if(n.x>m_nRightBound-(buttonWidth/2))
		{
			m_pButton->m_rectButtonPos.left=m_nRightBound-buttonWidth;
			m_pButton->m_rectButtonPos.right=m_nRightBound;
			m_pButton->MoveWindow(&(m_pButton->m_rectButtonPos));
		}
	}
	
	CButton::OnLButtonDown(nFlags, point);
}

void CSlider::SetMaxPos()
{
	m_pButton->m_rectButtonPos.left=m_nRightBound-buttonWidth;
	m_pButton->m_rectButtonPos.right=m_nRightBound;
	m_pButton->MoveWindow(&(m_pButton->m_rectButtonPos));	
}

void CSlider::SetPos(int nPercent)
{
	//percentage값을 버튼 왼쪽위치의 값으로 변환
	int ButtonLeft = 0;
	int temp = m_nLeftBound;
	
	ButtonLeft = (((float)(nPercent)*float(m_nRightBound-m_nLeftBound-buttonWidth))/80)-17;
	
	m_pButton->m_rectButtonPos.left = ButtonLeft;
	m_pButton->m_rectButtonPos.right= m_pButton->m_rectButtonPos.left+buttonWidth;
	m_pButton->MoveWindow(&(m_pButton->m_rectButtonPos));
}

int CSlider::GetPosition()
{
	return this->m_nPosPercent;
}

void CSlider::SetPosition(int percent)
{
	//0~100 % 단위를 0~80% 단위로 변환
	int calcPercent = (80*percent / 100)+20;

	int ButtonLeft = 0;
	ButtonLeft = (((float)(calcPercent)*float(m_nRightBound-m_nLeftBound-buttonWidth))/80)-17;
	
	m_pButton->m_rectButtonPos.left = ButtonLeft;
	m_pButton->m_rectButtonPos.right= m_pButton->m_rectButtonPos.left+buttonWidth;
	m_pButton->MoveWindow(&(m_pButton->m_rectButtonPos));
}


///////////////////////////////////////////////////////////////////////////////////////////
// CSliderButton
///////////////////////////////////////////////////////////////////////////////////////////
CSliderButton::CSliderButton()
{
	// KOL.UDF.022 시큐어코딩 보완
	m_pParent = NULL;
	m_ptOld = NULL;
	m_ptNew = NULL;
	m_bLButtonDown = FALSE;
	m_bCursorOnWindow = FALSE;
	m_nLeftBound = 0;
	m_nRightBound = 0;
	m_rectButtonPos = NULL;
	m_rectButtonImage = NULL;
	minX = 0;
	maxX = 0;
	buttonWidth = 0;
	
}

CSliderButton::~CSliderButton()
{
}


BEGIN_MESSAGE_MAP(CSliderButton, CButton)
//{{AFX_MSG_MAP(CSliderButton)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_PAINT()
ON_WM_WINDOWPOSCHANGED()
ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSliderButton message handlers

// 버튼의 OWNERDRAW 속성을 위해서 있어야 하는 함수
void CSliderButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	
}

BOOL CSliderButton::Create(CWnd *pParent,INT x,INT y,INT nMoveWidth) 
{
	m_pParent			= pParent;
	m_bCursorOnWindow	= FALSE;
	m_nLeftBound		= x;
	m_nRightBound		= nMoveWidth;
	
	// 슬라이더 버튼에 올라갈 이미지 로딩
	m_picButton1.Load("..\\BMP\\slider_buttonon.bmp");
	m_picButton2.Load("..\\BMP\\slider_button.bmp");
	m_picButtonBuffer1.Load("..\\BMP\\slider_buttonon.bmp");
	m_picButtonBuffer2.Load("..\\BMP\\slider_button.bmp");
	
	buttonWidth = m_picButton1.m_Width;
	
	// 슬라이더 버튼의 위치
	m_rectButtonPos.left	= x;
	m_rectButtonPos.top		= y;
	m_rectButtonPos.right	= x + m_picButton1.m_Width;
	m_rectButtonPos.bottom	= y + m_picButton1.m_Height;
	
	// 슬라이더 버튼 위에 올라가는 이미지 크기
	m_rectButtonImage.left		= 0;
	m_rectButtonImage.top		= 0;
	m_rectButtonImage.right		= m_picButton1.m_Width;
	m_rectButtonImage.bottom	= m_picButton1.m_Height;
	
	CButton::Create(_T("sliderbutton"),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,m_rectButtonPos,m_pParent,20000);
	
	return TRUE;
}


void CSliderButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CRect bgWnd;
	GetParent()->GetWindowRect(bgWnd);
	
	// 마우스가 이동할 경계값 설정
	minX = bgWnd.left + m_nLeftBound + (buttonWidth/2);
	maxX = bgWnd.left + m_nRightBound - (buttonWidth/2)-1;
	
	
	SetCapture();
	m_bLButtonDown = TRUE;
	
	if( m_bCursorOnWindow == FALSE )
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bCursorOnWindow = _TrackMouseEvent(&tme);
	}
	
	// ~☆~ 윈도 위치가 바뀐 후 커서의 현재 위치를 다시 세팅(m_ptNew와 m_ptOld를 동일하게함)
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	
	m_ptNew.x = pt.x;
	m_ptNew.y = pt.y;
	
	m_ptOld = m_ptNew;	
	
	CButton::OnLButtonDown(nFlags, point);
}

void CSliderButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	m_bLButtonDown = FALSE;
	m_bCursorOnWindow=FALSE;
	ReleaseCapture();
	
	Invalidate();
	UpdateWindow();
	
	CButton::OnLButtonUp(nFlags, point);
}

void CSliderButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CPoint mousePoint;
	GetCursorPos(&mousePoint);
	
	//TRACE(_T("%d\n"),mousePoint.x);
	
	CPoint ptDiffer;
	m_ptNew = mousePoint;
	ptDiffer = m_ptNew - m_ptOld;
	//	m_ptOld = m_ptNew;
	
	// 왼쪽 버튼이 눌려 졌을 때
	if( m_bLButtonDown == TRUE )//&& m_bCursorOnWindow == TRUE )
	{
		// 마우스 이동 경계값 사이라면 이동거리를 계산하여 버튼 이동
		if(mousePoint.x>=minX && mousePoint.x<=maxX )
		{
			
			ScreenToClient(&ptDiffer);
			
			m_rectButtonPos.left		= m_rectButtonPos.left + ptDiffer.x ;
			m_rectButtonPos.right		= m_rectButtonPos.right + ptDiffer.x ;
			MoveWindow(&m_rectButtonPos);
		}
		
		// 마우스 이동이 경계값을 벗어난다면 버튼을 끝 위치로 이동(멈춘것 처럼 보임)
		else
		{
			if(mousePoint.x<minX)
			{
				m_rectButtonPos.left=m_nLeftBound;
				m_rectButtonPos.right=m_nLeftBound+buttonWidth;
				MoveWindow(&m_rectButtonPos);
			}
			else if(mousePoint.x>=maxX)
			{
				m_rectButtonPos.left=m_nRightBound-buttonWidth;
				m_rectButtonPos.right=m_nRightBound;
				MoveWindow(&m_rectButtonPos);
			}
		}
		
	}	
	
	CButton::OnMouseMove(nFlags, point);
}

void CSliderButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// 슬라이더 버튼 그린다.
	if( m_bCursorOnWindow == TRUE )
	{
		m_picButton2.Show(&dc,m_rectButtonImage);
	}	
	else if( m_bCursorOnWindow == FALSE )
	{
		m_picButton1.Show(&dc,m_rectButtonImage);
	}
}

LONG CSliderButton::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{	
	// 마우스가 버튼 위로 들어오면 처음에 OnMouseMove→
	// OnMouseMove→TrackMouseEvent를 실행→WM_MOUSEHOVER
	Invalidate();
	UpdateWindow();
	
	return 0;
}

LONG CSliderButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	// 커서가 윈도우를 벗어날 때 : FALSE
	m_bCursorOnWindow = FALSE;	
	// 버튼 그림 Change
	Invalidate();
	UpdateWindow();
	
	return 0;
}

void CSliderButton::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CButton::OnWindowPosChanged(lpwndpos);
	
	// Button moving -> SendMessage
	m_pParent->SendMessage(UM_SLIDERBUTTONMOVED,(WPARAM)m_rectButtonPos.left,0);	
}
///////////////////////////////////////////////////////////////////////////////////////////

void CSlider::Save()
{
//파일에 퍼센티지값 저장
	CString filename;
	filename.Format(_T("..\\cfg\\Transparency\\%s.cfg"), m_strAlias);
	FILE *fd;
	fd = _tfopen(filename.GetBuffer(0), _T("wb"));
	if(fd != NULL)
	{
		CString str;
		str.Format(_T("[POSITION:%d]"),m_nPosPercent+20);
		_ftprintf(fd, _T("%s\0"), str);
		
		fclose(fd);
	}
}
