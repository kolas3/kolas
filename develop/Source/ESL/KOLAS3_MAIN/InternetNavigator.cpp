// InternetNavigator.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "InternetNavigator.h"
// 웹사이트의 소스파일에서 아이콘경로를 읽어 유니코드로 변환하는 데 사용
#include "CharConvert.h"	
#include "BalloonHelp.h"

// 17/02/06 김혜영 : KOLAS3.net Search를 통해 검색시 404 오류 발생
//*/ ADD ---------------------------------------------------------------------------
#include "..\\..\\..\\단행\\열람\\장서관리\\LOC_CommonAPI\\LocCommonAPI.h"
//*/ END ---------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInternetNavigator dialog


CInternetNavigator::CInternetNavigator(CWnd* pParent /*=NULL*/)
	: CDialog(CInternetNavigator::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInternetNavigator)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pView = NULL;
	m_bLock = FALSE;

	//++2008.07.03 ADD BY CJY {{++
	m_bIsKeyDown = FALSE;
	m_hDefaultIcon = NULL;

//DEL 	m_bDrawIcon = FALSE;
	//--2008.07.03 ADD BY CJY --}}
}


CInternetNavigator::~CInternetNavigator()
{
	m_Dlg_brush.DeleteObject();

	CString strData[MAX_URL_COUNT];
	const INT MAX = m_aURLHistory.GetSize();
	for(INT nCount=0; nCount < MAX; nCount++)
	{
		strData[nCount] = m_aURLHistory.GetAt(nCount);
	}

	Set_URLHistoryToRegstry(strData, MAX_URL_COUNT, 1);
}

void CInternetNavigator::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInternetNavigator)	
	DDX_Control(pDX, IDC_cmbURL, m_cmbURL);
	DDX_Control(pDX, IDC_edtHELP, m_edtHELP);	
	DDX_Control(pDX, IDC_btnNEXT	, m_btnNext);
	DDX_Control(pDX, IDC_btnPREV	, m_btnPrev);
	DDX_Control(pDX, IDC_btnREFLASH	, m_btnReflash);
	DDX_Control(pDX, IDC_btnSTOP	, m_btnStop);	
	DDX_Control(pDX, IDC_btnHELP	, m_btnHelp);
	//}}AFX_DATA_MAP
	
}

BEGIN_MESSAGE_MAP(CInternetNavigator, CDialog)
	//{{AFX_MSG_MAP(CInternetNavigator)
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnREFLASH, OnbtnREFLASH)
	ON_BN_CLICKED(IDC_btnSTOP, OnbtnSTOP)
	ON_BN_CLICKED(IDC_btnHELP, OnbtnHELP)
	ON_BN_CLICKED(IDC_btnLOCK, OnbtnLOCK)
	ON_CBN_SELENDOK(IDC_cmbURL, OnSelendokcmbURL)
	ON_CBN_EDITCHANGE(IDC_cmbURL, OnEditchangecmbURL)
	ON_WM_MOVE()
	ON_CBN_DROPDOWN(IDC_cmbURL, OnDropdowncmbURL)
	ON_EN_KILLFOCUS(IDC_edtHELP, OnKillfocusedtHELP)
	ON_EN_SETFOCUS(IDC_edtHELP, OnSetfocusedtHELP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInternetNavigator message handlers

BOOL CInternetNavigator::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	m_pParent = pParentWnd;	
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CInternetNavigator::PreTranslateMessage(MSG* pMsg)
{
	// 잠금기능
	// 메시지 잠금	
 	if ( m_bLock == TRUE ) 
 	{
 		if ( pMsg->message != WM_PAINT )
 		{
 			return TRUE;
 		}
 	}

	//++2008.06.30 UPDATE BY CJY {{++

	// 버튼을 눌렀을 경우
	// ESC키 -> 자동완성 리스트 숨김
	// 엔터키 -> 주소표시줄의 사이트로 이동
	// 아래/위 화살표/페이지업/페이지다운 -> 자동완성 리스트의 선택된 리스트 변경
	// 탭키 -> 자동완성 리스트에 선택된 텍스트를 콤보박스 에디트에 설정
	if(pMsg->message == WM_KEYDOWN )
	{	
		
		switch(pMsg->wParam)
		{
 		case VK_ESCAPE:			
	 		m_List.ShowWindow(SW_HIDE);
 			return TRUE;
 			break;
		case VK_RETURN:
			OnNavi();
			return TRUE;
			break;
			
		case VK_DOWN:			
			if(TRUE == m_cmbURL.GetDroppedState())
			{
				INT nResult = m_cmbURL.GetCurSel();
				INT nMax = m_cmbURL.GetCount();
				if(nResult+1 < nMax) m_cmbURL.SetCurSel(nResult+1);
			}
			else if(TRUE == m_List.IsWindowVisible())
			{
				m_List.Set_NextCurSel();
			}  			
			return TRUE;
			break;
		case VK_UP:		
			if(TRUE == m_cmbURL.GetDroppedState())
			{
				INT nResult = m_cmbURL.GetCurSel();
				if(0 <= nResult-1) m_cmbURL.SetCurSel(nResult-1);
			}
			else if(TRUE == m_List.IsWindowVisible())
			{
				m_List.Set_PrevCurSel();
			} 				
			return TRUE;			
			break;	
 		case VK_NEXT:
			if(TRUE == m_cmbURL.GetDroppedState())
			{	// 9는 한 화면에 표시되는 콤보박스 목록의 개수
				INT nResult = m_cmbURL.GetCurSel();
				INT nMax = m_cmbURL.GetCount();
				if(nResult+9 < nMax) m_cmbURL.SetCurSel(nResult+9);
				else m_cmbURL.SetCurSel(nMax-1);
			}
			else if(TRUE == m_List.IsWindowVisible())
			{
				m_List.Set_TailCurSel();
			} 			
 			return TRUE;
 			break;		
 		case VK_PRIOR:
			if(TRUE == m_cmbURL.GetDroppedState())
			{	// 9는 한 화면에 표시되는 콤보박스 목록의 개수
				INT nResult = m_cmbURL.GetCurSel();
				if(0 <= nResult-9) m_cmbURL.SetCurSel(nResult-9);
				else m_cmbURL.SetCurSel(0);
			}
			else if(TRUE == m_List.IsWindowVisible())
			{
				m_List.Set_HeadCurSel();
			} 	 			
 			return TRUE;
 			break;
		case VK_TAB:
			//	탭키를 눌렀을 경우 자동완성리스트에 내용을 에디트상자에 보여주기
			{
				CString strText = m_List.Get_CurSelText();				
				if(_T("") != strText)
				{
					m_cmbURL.SetWindowText(strText);
					m_List.ShowWindow(SW_HIDE);
					
					CEdit* pEdit = m_cmbURL.GetEditCtrl();
					pEdit->SetSel(-1);
				}
				return TRUE;
			}			
			break;
		default:
			m_bIsKeyDown = TRUE;
			break;
		}
		
	}
	else if(pMsg->message == WM_KEYUP)
	{
		m_bIsKeyDown = FALSE;		
	}
	
	//DEL 이전소스
//DEL 	if ( pMsg->message == WM_KEYDOWN )
//DEL 	{
//DEL 		if ( pMsg->wParam == VK_ESCAPE ) 
//DEL 		{
//DEL 			return TRUE;
//DEL 		}
//DEL 		else if ( pMsg->wParam == VK_RETURN )
//DEL 		{
//DEL 			//m_pView = m_pFrameWnd->GetActiveView();
//DEL 			( (CEdit*) GetDlgItem (IDC_edtURL) )->GetWindowText(m_strURL);
//DEL 			if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+1010, NULL, NULL);
//DEL 			return TRUE;
//DEL 		}
//DEL 	}
	//--2008.06.30 UPDATE BY CJY --}}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CInternetNavigator::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnPrev.LoadBitmaps("..\\BMP\\IePrev.bmp"		, RGB(0,0,0));
	m_btnNext.LoadBitmaps("..\\BMP\\IeNext.bmp"		, RGB(0,0,0));
	m_btnReflash.LoadBitmaps("..\\BMP\\Iereflash.bmp"	, RGB(0,0,0));
	m_btnStop.LoadBitmaps("..\\BMP\\Iestop.bmp"		, RGB(0,0,0));
	m_btnHelp.LoadBitmaps("..\\BMP\\IeHelp.bmp"		, RGB(0,0,0));	

	m_Dlg_brush.CreateSolidBrush(RGB(230,230,230));
	
	//++2008.07.01 ADD BY CJY {{++
	// 해당사이트에 아이콘파일이 없을 때 불러올 파일 설정
//DEL 	m_hDefaultIcon = (HICON)::LoadImage(
//DEL 		NULL,
//DEL 		(LPCTSTR)ICON_FILE_PATH _T("\\default.ico"),
//DEL 		IMAGE_ICON,
//DEL 		0,
//DEL 		0,
//DEL 		LR_LOADFROMFILE
//DEL 		); 	
	//DEL m_hDefaultIcon = ExtractIcon(NULL, _T("url.dll"), 1);
	ExtractIconEx(_T("url.dll"), 0, NULL, &m_hDefaultIcon, 1);

	// 디렉토리 없을시 만들기(가장 하위디렉토리만 적용됨)
	CFileFind file;
	CString strDir = ICON_FILE_PATH _T("\\");	
	BOOL bResult = file.FindFile(strDir + _T("*.*"));
	if(TRUE != bResult)
	{ 
		CreateDirectory(strDir, NULL);
	}
	else	// 기존 아이콘 삭제
	{	// 기존의 아이콘과 현재 방문한 웹사이트 순서가 일치한다는 보장이 없음
		if(TRUE == file.FindFile(strDir + _T("*.ico")))
		{
			while(TRUE == file.FindNextFile())
			{
				::DeleteFile(strDir + file.GetFileName());
			}
			::DeleteFile(strDir + file.GetFileName());
		}
	}
	

	// 히스토리 설정
 	CString strURL[MAX_URL_COUNT];
 	Get_URLHistoryFromRegstry(strURL, MAX_URL_COUNT, 1);

	// 콤보박스와 연결될 이미지 리스트의 초기화	
 	m_aImageList.Create(16, 16, ILC_COLOR32|ILC_MASK, 0, 25);
 
 	for(INT nCount=0; nCount<MAX_URL_COUNT; nCount++)
 	{
 		if(_T("") == strURL[nCount]) break;
 		m_aURLHistory.Add(strURL[nCount]);		
 	}
	
 	m_List.Create(IDD_AUTO_COMPLETE, this);	
	m_List.m_pParent = this;
 	m_List.Add_AutoCompleteData(&m_aURLHistory);

	m_cmbURL.SetImageList(&m_aImageList);
	m_cmbURL.Set_DefaultIcon(m_hDefaultIcon);
 	m_cmbURL.Set_DrawIconToEdit(TRUE);

 	Add_AllItemFromHistroyArray();
 	Add_AllIconToImageList();
	//--2008.07.01 ADD BY CJY  --}}	

	//++2008.07.10 ADD BY CJY {{++
	// 초기화면시 about:blank 보여주기	
	// 웹 아이콘을 보여주기 위해선 아이템을 추가하여 선택하여야 한다.
	// 콤보박스에 아이템을 추가해도 사용자가 콤보박스를 Dropdown할 시
	// m_aURLHistory에 있는 내용으로 아이템이 갱신되기 때문에 상관없다
//DEL 	m_bDrawIcon = TRUE;
 	m_cmbURL.SetWindowText(_T("about:blank"));
	// 기본 아이콘을 불러온뒤에
 //DEL 	m_aImageList.Add(m_hDefaultIcon);
//DEL 
//DEL 	// 아이템 설정
//DEL 	COMBOBOXEXITEM Item;
//DEL 	Item.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE;
//DEL 	Item.iItem = 0;	
//DEL 	Item.iImage = 0;
//DEL 	Item.iSelectedImage = 0;
//DEL 	Item.pszText = (TCHAR*)(LPCTSTR)_T("about:blank");
//DEL 	
//DEL 	// 콤보박스에 아이템을 추가한후
//DEL 	m_cmbURL.InsertItem(&Item);
//DEL 	// 방금 추가한 아이템을 선택한다
//DEL  	m_cmbURL.SetCurSel(0);

	// 이후 이미지 리스트에서 추가한 이미지 삭제
 //DEL 	m_aImageList.Remove(m_aImageList.GetImageCount()-1);
	
	//--2008.07.10 ADD BY CJY  --}}	

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CInternetNavigator::GetURL()
{	
	return m_strURL;
}

VOID CInternetNavigator::SetURL(CString strURL, BOOL bReDownload_IconFile)
{
	m_List.ShowWindow(SW_HIDE);
//++2008.07.01 UPDATE BY CJY {{++
	
 	if(_T("") == strURL)
	{	
 		m_cmbURL.SetCurSel(-1);
		m_cmbURL.SetWindowText(_T("about:blank"));	
	}
 	else
	{
		strURL = Make_URL(strURL);

		INT nIndex = Get_IndexFromHistoryArray(strURL);
		
		// 2009.05.11 REM BY PDJ : 자꾸 다운됨.
//		Put_Item(strURL, &nIndex, bReDownload_IconFile);
// 		if(0 <= nIndex)
//		{
//			m_cmbURL.SetCurSel(nIndex);
//		}
	}	
	m_strURL = strURL;

//DEL	이전소스
//DEL 	m_strURL = strURL;
//DEL 	( (CEdit*) GetDlgItem (IDC_edtURL) )->SetWindowText(m_strURL);	
//--2008.07.01 UPDATE BY CJY --}}
}
void CInternetNavigator::OnSetfocusedtURL() 
{
	//++2008.06.30 UPDATE BY CJY {{++
	m_cmbURL.GetWindowText(m_strURL);
	CEdit* pEdit = m_cmbURL.GetEditCtrl();
	pEdit->SetSel(0, m_strURL.GetLength());
//DEL	이전소스
//DEL 	CString strURL;	
//DEL 	( (CEdit*) GetDlgItem (IDC_edtURL) )->GetWindowText(m_strURL);		
//DEL 	( (CEdit*) GetDlgItem (IDC_edtURL) )->SetSel(0,m_strURL.GetLength());
	
	//--2008.06.30 UPDATE BY CJY --}}
}

void CInternetNavigator::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDown(nFlags, point);
}

void CInternetNavigator::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);	

	//++2008.07.02 UPDATE BY CJY {{++
	// 에디트박스->콤보박스로 변경
	// 화면깜밖임 개선
	CWnd *pWnd = ( (CWnd*) GetDlgItem (IDC_cmbURL) );
	if ( pWnd->GetSafeHwnd() )
	{
		if ( cx < 450 ) cx=450;
		// 3은 크기가 조정될 컨트롤의 개수
		HDWP hdwp = ::BeginDeferWindowPos(4);
 		// 콤보박스 크기조정
 		CRect rect = CRect(71, 5, cx-256, 27);
 		::DeferWindowPos(
 			hdwp, 
 			pWnd->GetSafeHwnd(),
 			HWND_TOP,                
 			rect.left,
 			rect.top,
 			rect.Width(),
 			rect.Height(),
 			SWP_NOZORDER|SWP_SHOWWINDOW
 			);
 		// 자동완성리스트 크기조정
  		rect.top = rect.bottom;//rect.bottom;
		rect.left = rect.left+25;
		rect.right = rect.right-220;
   		rect.bottom = rect.top+100;//rect.top + 400; 		
 		ClientToScreen(rect);
 		m_List.MoveWindow(rect); // m_List는 다이어로그
		// 정지 버튼 크기조정
		pWnd = ( (CWnd*) GetDlgItem (IDC_btnSTOP) );
		pWnd->GetClientRect(rect);		
		ClientToScreen(rect);
 		rect.top = 5;
 		rect.bottom = 27;
 		rect.left = cx - ( rect.right - rect.left)-204;
 		rect.right = cx - 204;
 		::DeferWindowPos(
 			hdwp, 
 			pWnd->GetSafeHwnd(),
 			HWND_TOP,                
 			rect.left,
 			rect.top,
 			rect.Width(),
 			rect.Height(),
 			SWP_NOZORDER|SWP_SHOWWINDOW
 			);
		// 새로고침 버튼 크기조정
		pWnd = ( (CWnd*) GetDlgItem (IDC_btnREFLASH) );
		pWnd->GetClientRect(rect);		
		ClientToScreen(rect);
 		rect.top = 5;
 		rect.bottom = 27;
 		rect.left = cx - ( rect.right - rect.left)- 230;
 		rect.right = cx - 230;
		::DeferWindowPos(
			hdwp, 
			pWnd->GetSafeHwnd(),
			HWND_TOP,                
			rect.left,
			rect.top,
			rect.Width(),
			rect.Height(),
			SWP_NOZORDER|SWP_SHOWWINDOW
			);
		// 도움말 버튼 크기조정
		pWnd = ( (CWnd*) GetDlgItem (IDC_btnHELP) );
		pWnd->GetClientRect(rect);		
		ClientToScreen(rect);
 		rect.top = 5;
 		rect.bottom = 27;
 		rect.left = cx - ( rect.right - rect.left)- 4;
 		rect.right = cx - 4;
		::DeferWindowPos(
			hdwp, 
			pWnd->GetSafeHwnd(),
			HWND_TOP,                
			rect.left,
			rect.top,
			rect.Width(),
			rect.Height(),
			SWP_NOZORDER|SWP_SHOWWINDOW
			);
		// 도움말 에디트 크기조정
  		//rect.top = rect.bottom;
		rect.left = rect.left-170;
		rect.right = rect.right-27;
   		//rect.bottom = rect.top+100;
 		//ClientToScreen(rect);
 		m_edtHELP.MoveWindow(rect); 		
		::EndDeferWindowPos (hdwp);
		CStatic *m_HelpMessage;		
		m_HelpMessage = (CStatic*)GetDlgItem(IDC_HELP_MESSAGE);
		m_HelpMessage->GetClientRect(rect);		
		ClientToScreen(rect);
		rect.top = 8;
 		rect.bottom = 24;
 		rect.left = cx - ( rect.right - rect.left)- 55;
 		rect.right = cx - 55;
		m_HelpMessage->MoveWindow(rect); 
	}
}

HBRUSH CInternetNavigator::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=NULL;
	if(pWnd->m_hWnd==this->m_hWnd) hbr= HBRUSH(m_Dlg_brush);	
	else hbr =CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

void CInternetNavigator::OnbtnPREV() 
{	
	if ( m_pView == NULL ) return;
	if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+1100, NULL, NULL);		
}

void CInternetNavigator::OnbtnNEXT() 
{	
	if ( m_pView == NULL ) return;
	if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+1101, NULL, NULL);			
}

void CInternetNavigator::OnbtnREFLASH() 
{	
	if ( m_pView == NULL ) return;
	if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+1102, NULL, NULL);			
	
}

void CInternetNavigator::OnbtnSTOP() 
{	
	if ( m_pView == NULL ) return;
	if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+1103, NULL, NULL);					
}

void CInternetNavigator::OnbtnHELP() 
{	
	if ( m_pView == NULL ) return;
	if ( m_pView->GetSafeHwnd() != NULL ) OnNavi();
}

void CInternetNavigator::OnbtnLOCK() 
{	
	if ( m_pView == NULL ) return;
	if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+99, NULL, NULL);	
}

VOID CInternetNavigator::SetFrameWnd(CFrameWnd* pFrameWnd)
{
	m_pFrameWnd = pFrameWnd;	
}

VOID CInternetNavigator::SetParentView(CView *pView)
{
	m_pView = pView;
}


//++2008.06.30 ADD BY CJY {{++
// 추가된 사항
// 1. 주소표시줄 에디트박스에서 확장콤보박스로 변환
// 2. 레지스트리에서 방문한 웹사이트 목록을 받아와서 콤보박스에 추가
// 3. 콤보박스 아이템 선택시 해당 사이트로 이동
// 4. 아이콘 추가
// 5. 사용자가 엔터키로 사이트 방문시 레지스트리에 방문한 웹사이트 기록
// 6. 콤보박스에 있는 내용 자동완성 기능

// 함수이름: CInternetNavigator::GetURLHistoryFromRegstry
// 함수설명: 레지스트리로부터 url목록을 읽어온다.
// 반환  형: INT 
// 인자[CString *strResult[]] : 받아올 값의 배열
// 인자[INT nGetURLCount] : 받아올 값의 개수
// 인자[INT nStart] : 레지스트리에 있는 url1부터 url25번까지의 값 중 몇 번째값부터 받아올 것인지 설정
INT CInternetNavigator::Get_URLHistoryFromRegstry(CString* strResult, INT nGetURLCount, INT nStart)
{
	// 범위가 잘못된 경우 리턴
	if(NULL == strResult) return -1;
	if(1 > nGetURLCount || nGetURLCount > MAX_URL_COUNT) return -2;
	if(0 > nStart || nStart > MAX_URL_COUNT) return -3;
	if(MAX_URL_COUNT+1 < nGetURLCount + nStart) return -4;

	HKEY hKey = NULL; 
	// Key를 Open하고 key값을 받아낸다. 
	LONG lResult = RegOpenKeyExW(
		HKEY_CURRENT_USER, 
		_T("Software\\Microsoft\\Internet Explorer\\TypedURLs"),
		0,
		KEY_QUERY_VALUE,
		&hKey
		); 

	// Open이 성공이 아닌 경우
	if(ERROR_SUCCESS != lResult)
	{
		RegCloseKey(hKey);
		return -5;
	}

	CString strValueName = _T("");// 얻어올 값의 이름	
	DWORD dwType = REG_SZ;  // 찾는값의 TYPE 
	DWORD dwCount =0;//_MAX_PATH;  // 반환될 값의 길이 	
	for(INT nCount=0; nCount<nGetURLCount; nCount++)
	{ 
		strValueName.Format(_T("url%d"), nStart+nCount);
				
		// dwCount에 데이터를 받아오는 데, 필요한 크기가 들어온다.
		TCHAR* pszValue = NULL;
		lResult = RegQueryValueEx(
			hKey,
			strValueName,
			NULL,
			&dwType,
			(LPBYTE)pszValue,
			&dwCount
			); 

		// 필요한 크기만큼 동적할당 해준후
		pszValue = new TCHAR[dwCount];
		// 다시 데이터를 받아온다.
		lResult = RegQueryValueEx(
			hKey,
			strValueName,
			NULL,
			&dwType,
			(LPBYTE)pszValue,
			&dwCount
			); 
		
		if(ERROR_SUCCESS != lResult)
		{
			RegCloseKey(hKey);
			delete[] pszValue;
			return -6;
		}
		else
		{			
			strResult[nCount] = pszValue;
			delete[] pszValue;
		}
	}

	RegCloseKey(hKey);

	return 0;
}

//--2008.06.30 ADD BY CJY --}}
//++2008.07.01 ADD BY CJY {{++

// CInternetFile/CInternetSession에 사용
#include <afxinet.h>

// 함수이름: GetIconPath
// 함수설명: 해당 웹사이트의 소스를 열어 아이콘의 위치경로를 얻어 반환한다.
// 반환  형: INT 
// 인자[CString strURL] : 
// 인자[CString* strIconPath] : 
INT CInternetNavigator::Get_IconPathFromURLSourceFile(CString strURL, CString* strIconPath, CString* strIconFileName)
{	
	if(NULL == strIconPath || NULL == strIconFileName) return -1;

	strURL.TrimLeft();
	if(0 != strURL.Find(_T("http://"))) return -2;

	// 2009.02.25 ADD BY PDJ : 임시로 처리함. 교보문고 상세보기시 문제가 있음.
	CString strMainUrl;
	INT		nOffset;
	nOffset = strURL.Find(_T("/"), 7);
	strMainUrl = strURL.Left(nOffset);
	strMainUrl.MakeUpper();
	//if ( ( strMainUrl.Find(_T("KYOBOBOOK")) >= 0 || strMainUrl.Find(_T("INTERPARK")) >= 0) && strURL.GetLength() > 40 ) return 0;	
	//

	
	TCHAR* szUTmp = NULL;	
	char szData[20000];
	// 2009.05.11 REM BY PDJ : 세션 무한루프 및 메모리 릭
	
	// URL의 소스파일을 읽어와서 숏 컷 아이콘정보를 얻어온다.
//  	try
//	{
//		CInternetSession session;
//		CInternetFile *file = (CInternetFile*)session.OpenURL(strURL);
//		
//		file->SetReadBufferSize(20000);		
//		file->ReadHuge(szData, 20000);		
//	}
//	catch(...)
//	{
//		return GetLastError();
//	}
	
	
	try
	{
		CInternetSession session;
		CInternetFile *file = (CInternetFile*)session.OpenURL(strURL);

		DWORD dwSize=file->GetLength();// 파일의 크기
		
		file->SetReadBufferSize(dwSize);		
		file->Read(szData, dwSize);		
	}
	catch(...)
	{
		return GetLastError();
	}

	CharConvert cv;	
	CString strData;
	INT len;
	while(TRUE)
	{
	// ansi로 읽어오기 때문에 유니코드로 변환한다.
		if(0 == cv.MultiByteToUnicode(szData,&szUTmp,&len))
		{			
			strData = szUTmp;				
			delete [] szUTmp;
		}
		
		INT nIndex = strData.Find(_T("shortcut icon"));
		if(0 > nIndex) break;
		
		nIndex = strData.Find(_T("href"), nIndex+1);
		if(0 > nIndex) break;
		
		// icon의 주소가 시작되는 지점
		nIndex = strData.Find(_T("\""), nIndex+1);
		if(0 > nIndex) break;
		INT nFirst = nIndex+1;

		// icon의 주소가 끝나는 지점
		nIndex = strData.Find(_T("\""), nIndex+1);
		if(0 > nIndex) break;
		INT nEnd = nIndex;

		// icon의 주소
		strData = strData.Mid(nFirst, nEnd-nFirst);		
		
		nIndex = strData.ReverseFind('/');
		if(0 > nIndex) break;

		*strIconPath = strData.Left(nIndex);
		*strIconFileName = strData.Mid(nIndex+1);

		return 0;
	}

	INT nIndex = strURL.Find(_T("//"));
	nIndex = strURL.Find(_T("/"), nIndex+2);

	if(0 <= nIndex)
	{
		*strIconPath = strURL.Left(nIndex);
		*strIconFileName = _T("favicon.ico");
	}
	else return -1;
	return 0;
}


// 함수이름: CInternetNavigator::OnSelendokcmbURL
// 함수설명: 사용자가 콤보박스목록에서 아이템을 클릭하였을 경우
// 반환  형: void 
void CInternetNavigator::OnSelendokcmbURL()
{
	// TODO: Add your control notification handler code here	
 	INT nIndex = m_cmbURL.GetCurSel();
 	if(0>nIndex) return;
	
 	CString strText = _T("");
 	m_cmbURL.GetLBText(nIndex, strText);
  	m_cmbURL.SetCurSel(nIndex);

 	SetURL(strText);

//DEL 	CString strFilePath = ICON_FILE_PATH _T("//") + Get_IconFileName(nIndex);
//DEL 	CFileFind file;
//DEL 	if(FALSE == file.FindFile(strFilePath))
	if(FALSE == Is_IconFile(nIndex))
	{
		// 2009.05.11 REM BY PDJ : 안되는 싸이트 있음
		//Download_IconFile(strText, ICON_FILE_PATH, Get_IconFileName(nIndex));
		//Modify_ImageToImageList(nIndex);
	}

  	if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+1010, NULL, NULL);
}


// 함수이름: CInternetNavigator::OnEditchangecmbURL
// 함수설명: 콤보박스의 에디트박스의 글자가 변경되었을 경우
// 반환  형: void 
void CInternetNavigator::OnEditchangecmbURL() 
{
	// TODO: Add your control notification handler code here

//DEL  	에디트박스가 변경될 경우는 사용자가 주소표시줄에 문자를 입력하거나
//DEL  	콤보박스의 아이템을 선택하였을 때인 데,
//DEL  	사용자가 문자를 입력하였을때만 자동완성기능을 보여주게 한다.
 	if(TRUE == m_bIsKeyDown) // m_bIsKeyDown은 Pretranslate에서 설정한다.
 	{		
 		CString strEditText = _T("");
 		m_cmbURL.GetWindowText(strEditText);
 		m_List.ShowAutoCompleteList(strEditText); 
 	}
}

//--2008.07.01 ADD BY CJY --}}

//++2008.07.07 ADD BY CJY {{++


// 함수이름: CInternetNavigator::Download_IconFile
// 함수설명: url경로에서 아이콘파일 경로를 찾아 다운받는다.
// 반환  형: INT 
// 인자[CString strURL] : 아이콘을 받아올 url경로
// 인자[CString strFilePath] : 아이콘파일을 저장할 파일경로
// 인자[CString strFileName] : 아이콘파일 이름
INT CInternetNavigator::Download_IconFile(CString strURL, CString strFilePath, CString strFileName)
{
	::DeleteFile(strFilePath + _T("\\") + strFileName);
	
	CString strGetIconPath = _T("");
	CString strGetIconName = _T("favicon.ico");
	Get_IconPathFromURLSourceFile(strURL, &strGetIconPath, &strGetIconName);
	
	if(_T("") == strGetIconPath) strGetIconPath = strURL;
	
	// 경로가 드라이브인경우'c:\', 'd:\', 
	if(0 <= strGetIconPath.Find(_T(":\\"))) return -1;

	// 저장 경로 설정
	if(_T("") == strFilePath)
	{
		TCHAR tmp[256];
		ZeroMemory(tmp, 256);
		::GetCurrentDirectory(256, tmp);
		strFilePath = tmp;
	}    

	strURL = strGetIconPath + _T("/") + strGetIconName;
	CString strFile = strFilePath + _T("/") + strFileName;
	
	try
	{
		HRESULT hr = URLDownloadToFile(
			NULL,      // ptr to ActiveX container
			strURL,      // URL to get
			strFile,     // file to store data in
			0,         // reserved
			NULL //&callback  // ptr to IBindStatusCallback
			);
		if(!SUCCEEDED(hr)) return -1;

		return 0;
	}
	catch(...)
	{
		return GetLastError();
	}	

	return -1;
}


// 함수이름: CInternetNavigator::Add_ItemFromHistroyArray
// 함수설명: 히스토리 문자열에서 문자열을 가져와 콤보박스에 추가한다.
// 반환  형: INT 
// 인자[INT nIndex] : 
INT CInternetNavigator::Add_ItemFromHistroyArray(INT nIndex)
{
	if(0 > nIndex || nIndex >= m_aURLHistory.GetSize()) return -1;
	if(_T("") == m_aURLHistory.GetAt(nIndex)) return -2;

	COMBOBOXEXITEM Item;
	Item.mask = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE;
	Item.iItem = nIndex;
	Item.iImage = nIndex;
	Item.iSelectedImage = nIndex;
	Item.pszText = (TCHAR*)(LPCTSTR)m_aURLHistory.GetAt(nIndex);
	
	m_cmbURL.InsertItem(&Item);	
	
	return 0;
}


// 함수이름: CInternetNavigator::Add_AllItemFromHistroyArray
// 함수설명: 이전 방문한사이트 배열에 있는 모든 내용을 콤보박스에 아이템으로 추가한다.
// 반환  형: INT 
INT CInternetNavigator::Add_AllItemFromHistroyArray()
{
	for(INT nCount=0; nCount < MAX_URL_COUNT; nCount++)
	{
		Add_ItemFromHistroyArray(nCount);
	}

	return 0;
}


// 함수이름: CInternetNavigator::Reset_ItemList
// 함수설명: 콤보박스의 아이템과 이미지리스트를 초기화한다
// 반환  형: INT 
INT CInternetNavigator::Reset_ItemList()
{
	m_cmbURL.ResetContent();
	const INT MAX = m_aImageList.GetImageCount();
	for(INT nCount=0; nCount < MAX; nCount++)
		m_aImageList.Remove(0);

	return 0;
}


// 함수이름: CInternetNavigator::Set_URLHistoryToRegstry
// 함수설명: 
// 반환  형: INT 
// 인자[CString *strPutData] : 문자배열
// 인자[INT nSetURLCount] : 저장할 개수
// 인자[INT nStart] : 시작번호 0번부터 시작
INT CInternetNavigator::Set_URLHistoryToRegstry(CString *strPutData, INT nSetURLCount, INT nStart)
{
	if(NULL == strPutData) return -1;
	if(1 > nSetURLCount || nSetURLCount > MAX_URL_COUNT) return -2;
	if(0 > nStart || nStart > MAX_URL_COUNT) return -3;
	if(MAX_URL_COUNT+1 < nSetURLCount + nStart) return -4;
		
	HKEY hKey = NULL; 
	// Key를 Open하고 key값을 받아낸다. 
	LONG lResult = RegOpenKeyExW(HKEY_CURRENT_USER, 
		_T("Software\\Microsoft\\Internet Explorer\\TypedURLs"), 0, KEY_ALL_ACCESS, &hKey); 
	// Open이 성공이 아닌 경우
	if(ERROR_SUCCESS != lResult) return -5;

	CString strValueName = _T("");// 얻어올 값의 이름		
	DWORD dwCount =0;// 값의 길이 	
	for(INT nCount=0; nCount<nSetURLCount; nCount++)
	{ 
		strValueName.Format(_T("url%d"), nStart+nCount);
		if(_T("") == strPutData[nCount]) break;

		// 저장될 크기
		DWORD dwCount = ( sizeof(TCHAR)*(strPutData[nCount].GetLength()+1) );
		TCHAR* pszValue = strPutData[nCount].GetBuffer(strPutData[nCount].GetLength());

		lResult = RegSetValueEx(
			hKey,
			strValueName,	// 값 이름
			NULL,
			REG_SZ,		// 값의 형식
			(LPBYTE)pszValue,	// 저장될 값
			dwCount			// 저장될 값의 길이
			);
 
		if(ERROR_SUCCESS != lResult) return -6;
	}
	RegCloseKey(hKey);
	return 0;
}

// 함수이름: CInternetNavigator::Is_AlreadyInsertHistoryArray
// 함수설명: 해당문자열이 히스토리 배열에 저장되어 있는지 여부 판단
// 반환  형: BOOL 
// 인자[CString strData] : 
BOOL CInternetNavigator::Is_AlreadyInsertHistoryArray(CString strData)
{
	if(0<= Get_IndexFromHistoryArray(strData)) return TRUE;
	return FALSE;
}


// 함수이름: CInternetNavigator::OnDropdowncmbURL
// 함수설명: 사용자가 콤보박스의 목록을 열었을 경우
// 반환  형: void 
void CInternetNavigator::OnDropdowncmbURL() 
{
	// TODO: Add your control notification handler code here
	// 자동완성 리스트박스를 숨기고
	m_List.ShowWindow(SW_HIDE);
	// 콤보박스의 모든 아이템 초기화
   	Reset_ItemList();
	// 이미지리스트에 아이콘을 설정
 	Add_AllIconToImageList();
	// 아이템을 추가
	Add_AllItemFromHistroyArray();

	m_cmbURL.Set_DrawIconToEdit(TRUE);
}


// 함수이름: CInternetNavigator::Get_IconFileName
// 함수설명: 특정 인덱스의 아이콘파일이름을 반환한다
// 반환  형: CString 
// 인자[INT nIndex] : 
CString CInternetNavigator::Get_IconFileName(INT nIndex)
{
	CString strIndex = _T("");
	strIndex.Format(_T("url%d.ico"), nIndex+1);
	return strIndex;
}


// 함수이름: CInternetNavigator::Add_IconToImageList
// 함수설명: 아이콘 파일 경로에서 해당아이콘파일을 읽어와 리스트에 추가한다.
// 반환  형: INT 
// 인자[CString strIconName] : 
INT CInternetNavigator::Add_IconToImageList(CString strIconName)
{
	CString strFile = ICON_FILE_PATH;
	strFile += _T("\\") + strIconName;
	
	HICON hIcon = (HICON)::LoadImage(
		NULL,
		(LPCTSTR)strFile,
		IMAGE_ICON,
		0,
		0,
		LR_LOADFROMFILE
		);
	
	INT nCount = m_aImageList.GetImageCount();
	if(0>m_aImageList.Add(hIcon))
	{			 
		m_aImageList.Add(m_hDefaultIcon);
	}
	nCount = m_aImageList.GetImageCount();
	return 0;
	
}


// 함수이름: CInternetNavigator::Add_AllIconToImageList
// 함수설명: 아이콘파일경로에서 모든 아이콘파일을 불러와 리스트에 추가한다.
// 반환  형: INT 
INT CInternetNavigator::Add_AllIconToImageList()
{
	CString strIconName = _T("");
	for(INT nCount=0; nCount < m_aURLHistory.GetSize(); nCount++)
	{
		strIconName = Get_IconFileName(nCount);
		Add_IconToImageList(strIconName);
	}
	return 0;
}


// 함수이름: CInternetNavigator::Modify_ImageToImageList
// 함수설명: 이미지리스트의 특정 인덱스의 이미지를 다시 불러온다.
// 반환  형: INT 
// 인자[INT nIndex] : 
INT CInternetNavigator::Modify_ImageToImageList(INT nIndex)
{
	CString strIconName = Get_IconFileName(nIndex);
	
	CString strFile = ICON_FILE_PATH;
	strFile += _T("\\") + strIconName;
	
	HICON hIcon = (HICON)::LoadImage(
		NULL,
		(LPCTSTR)strFile,
		IMAGE_ICON,
		0,
		0,
		LR_LOADFROMFILE
		);
	if(NULL != hIcon)
	{
		HICON hPrevIcon = m_aImageList.ExtractIcon(nIndex);
		HICON hNewIcon = hIcon;
		CString strURL = m_aURLHistory.GetAt(nIndex);

		TRACE(_T("REPLACE ICON : %d -> %d (Default = %d) %s\r\n"),
			hPrevIcon, hNewIcon, m_hDefaultIcon, strURL
			);

		m_aImageList.Replace(nIndex, hIcon);
	}
	return 0;
}


// 함수이름: CInternetNavigator::Get_IndexFromHistoryArray
// 함수설명: 이전 방문한웹사이트 배열에서 특정 문자열이 포함된 인덱스를 반환한다
// 반환  형: INT 
// 인자[CString strData] : 
INT CInternetNavigator::Get_IndexFromHistoryArray(CString strData)
{
	const INT MAX = m_aURLHistory.GetSize();
	for(INT nCount=0; nCount<MAX; nCount++)
	{
		if(strData == m_aURLHistory.GetAt(nCount)) return nCount;
	}

	return -1;
}



// 함수이름: CInternetNavigator::OnMove
// 함수설명: 
// 윈도우 이동시 자동완성리스트의 위치도 같이 이동
// 메인프레임의 OnMove함수에서 이 곳으로 SendMessage를 보낸다.
// 반환  형: void 
// 인자[int x] : 
// 인자[int y] : 
void CInternetNavigator::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	
	// 콤보박스 크기조정
	if(NULL != m_cmbURL.GetSafeHwnd())
	{
		CRect rect;
		m_cmbURL.GetWindowRect(&rect);
		// 자동완성리스트 크기조정
		rect.top = rect.bottom;//rect.bottom;	
		rect.left = rect.left+25;
		rect.right = rect.right-20;
		rect.bottom = rect.top+100;//rect.top + 400;
		m_List.MoveWindow(rect); // m_List는 다이어로그		
	}
}



// 함수이름: CInternetNavigator::OnNotify
// 함수설명: 자동완성 리스트에서 아이템이 선택되었을 경우 SendMessage로 여기로 보내진다.
// 반환  형: BOOL 
// 인자[WPARAM wParam] : 보낸 곳의 id값
// 인자[LPARAM lParam] : 
// 인자[LRESULT* pResult] : 
BOOL CInternetNavigator::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class

	// 자동완성 리스트에서 사용자가 아이템을 선택하였을 경우
	if(CAutoCompleteList::IDD == wParam)
	{
		NMHDR* pHdr = (NMHDR*)lParam;
		if(LBN_SELCHANGE == pHdr->code)
		{			
			CString strData = m_List.Get_CurSelText();
			m_cmbURL.SetWindowText(strData);
			INT nLength = strData.GetLength();

			CEdit* pEdit = m_cmbURL.GetEditCtrl();
			pEdit->SetSel(-1,-1);
		}
		else if(VK_RETURN == pHdr->code)
		{
			PostMessage(WM_KEYDOWN, VK_RETURN);
		}
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}


// 함수이름: CInternetNavigator::Is_IconFile
// 함수설명: 하드디스크상에서 특정 인덱스의 아이콘파일의 존재여부 판단
// 반환  형: BOOL 
// 인자[INT nIndex] : 
BOOL CInternetNavigator::Is_IconFile(INT nIndex)
{
	CString strFilePath = ICON_FILE_PATH _T("//") + Get_IconFileName(nIndex);
	CFileFind file;
	return file.FindFile(strFilePath);
}

//--2008.07.07 ADD BY CJY --}}

//++2008.07.08 ADD BY CJY {{++
// 함수이름: CInternetNavigator::Put_Item
// 함수설명: 인자로 받은 텍스트가 이미 콤보박스 리스트에 있으면 해당 리스트의 인덱스를 반환하고,
//		없으면 콤보박스에 추가시킨다. 
// 반환  형: INT 
// 인자[CString strText] : 
// 인자[INT *pGetIndex] : 
INT CInternetNavigator::Put_Item(CString strText, INT *pGetIndex, BOOL bReDownload_IconFile)
{
	// 목록에 없는 내용이면 추가한다.
	INT nTextIndex = Get_IndexFromHistoryArray(strText);
	if(0 > nTextIndex)
	{
		
		const INT nSize = m_aURLHistory.GetSize();
		if(MAX_URL_COUNT == nSize)
		{ 
			//DEL 		// 목록의 개수가 꽉찼으면 url 2부터 url max번까지를
			//DEL 		// url1부터 url max-1번까지 옮긴 후
			//DEL 		// 새 내용을 url max에 저장
			//DEL 		CString strData[MAX_URL_COUNT];
			//DEL 		Get_URLHistoryFromRegstry(strData, MAX_URL_COUNT-1, 2);
			//DEL 		strData[MAX_URL_COUNT-1] = strText;
			//DEL 		Set_URLHistoryToRegstry(strData, MAX_URL_COUNT, 1);
			
			// url목록을 갱신
			m_aURLHistory.RemoveAt(0);
			m_aURLHistory.Add(strText);
			
 			// icon파일 갱신 및 현재 접속한 사이트 아이콘 다운로드
 			CString strFile = ICON_FILE_PATH;
 			strFile += _T("\\");
 			strFile += Get_IconFileName(0);
			
 			::DeleteFile(strFile);
			
 			for(INT nCount=1; nCount <= MAX_URL_COUNT; nCount++)
 			{ 	
 				CString strFilePath = ICON_FILE_PATH _T("\\");
 				CString strOldFilePath = strFilePath + Get_IconFileName(nCount);
 				CString strNewFilePath = strFilePath + Get_IconFileName(nCount-1);
 				CFileFind filefind;
 				CFile file;
 				if(TRUE == filefind.FindFile(strOldFilePath))
 				{
 					file.Rename(strOldFilePath, strNewFilePath);
 				}			
 				
 			}
			// 현재 접속한 사이트의 아이콘을 다운받아
			// 이미지 리스트에 추가한뒤
			// 콤보박스 리스트에도 추가한다.
			Download_IconFile(strText, ICON_FILE_PATH, Get_IconFileName(MAX_URL_COUNT-1));
			m_aImageList.Remove(0);
			Add_IconToImageList(Get_IconFileName(MAX_URL_COUNT-1));
			
			m_cmbURL.DeleteItem(0);
			Add_ItemFromHistroyArray(MAX_URL_COUNT-1);
			
			if(NULL != pGetIndex) *pGetIndex = MAX_URL_COUNT-1;
		}				
		// 목록의 개수가 남아있으면
		// url nCount+1번에 저장
		else if(MAX_URL_COUNT > nSize)
		{
			//DEL 		Set_URLHistoryToRegstry(&strText, 1, nSize+1);
			m_aURLHistory.Add(strText);
			Download_IconFile(strText, ICON_FILE_PATH, Get_IconFileName(nSize));
			Add_IconToImageList(Get_IconFileName(nSize));
			Add_ItemFromHistroyArray(nSize);
			
			if(NULL != pGetIndex) *pGetIndex = nSize;
		}
		else return -1;

		m_List.Add_AutoCompleteData(strText);
	}
	else // 이미 목록에 있는 사이트일 경우
	{		
		// 아이콘파일이 없는 경우 새로 다운받는다.				
		if(FALSE == Is_IconFile(nTextIndex))
		{
			if(TRUE == bReDownload_IconFile)
			{
				Download_IconFile(strText, ICON_FILE_PATH, Get_IconFileName(nTextIndex));
				Modify_ImageToImageList(nTextIndex);
			}

			if(NULL != pGetIndex) *pGetIndex = nTextIndex;
		}
	}	
	
	return 0;
}


// 함수이름: CInternetNavigator::Make_URL
// 함수설명: 
// 반환  형: CString 
// 인자[CString strURL] : 
CString CInternetNavigator::Make_URL(CString strURL) const
{
	strURL.TrimLeft();
	if(-1 == strURL.Find(_T("://")) && _T("") != strURL)
	{
		strURL = _T("http://") + strURL;
	}
	// 17/02/06 김혜영 : KOLAS3.net Search를 통해 검색시 404 오류 발생
/*//REM--------------------------------------------------------------------------
	if(strURL.GetLength()-1 != strURL.ReverseFind('/') && _T("") != strURL)
	{
		strURL += _T("/");
	}
*///END--------------------------------------------------------------------------	

	return strURL;
}
//--2008.07.08 ADD BY CJY --}}
//DEL LRESULT CInternetNavigator::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
//DEL {
//DEL 	// TODO: Add your specialized code here and/or call the base class
//DEL 	if(TRUE == m_bDrawIcon && NULL != m_cmbURL.GetSafeHwnd())
//DEL 	{		
//DEL 		CDC* pDC = m_cmbURL.GetDC();
//DEL 		DrawIconEx(
//DEL 			pDC->m_hDC,
//DEL 			3,
//DEL 			3,
//DEL 			m_hDefaultIcon,
//DEL 			16,
//DEL 			16,
//DEL 			NULL,
//DEL 			(HBRUSH)RGB(255, 255, 255),
//DEL 			DI_NORMAL
//DEL 			);
//DEL 		ReleaseDC(pDC);
//DEL 	}	
//DEL 	return CDialog::WindowProc(message, wParam, lParam);
//DEL }

// 2008.11.10 ADD BY PWR
// 다이얼로그 XP테마적용
VOID CInternetNavigator::EnableThemeDialogTexture(HWND hwndDlg)
{
	HINSTANCE hDll=LoadLibrary(_T("UxTheme.dll"));	// 'UxTheme.dll'
	if(hDll==NULL)
		return;	// the DLL won't be available on anything except Windows XP
	ULONG (PASCAL *lpfnEnableTheme)(HWND, DWORD);
	(FARPROC&)lpfnEnableTheme=GetProcAddress(hDll,"EnableThemeDialogTexture");	// 'EnableThemeDialogTexture'
	if(lpfnEnableTheme)
		lpfnEnableTheme(hwndDlg, 6);	// ETDT_ENABLETAB = (ETDT_ENABLE | ETDT_USETABTEXTURE) = (2|4) = 6
	else ASSERT(FALSE);
	FreeLibrary(hDll);
}


VOID CInternetNavigator::OnNavi()
{
	CWnd* CHelpEdit = (CEdit*) GetDlgItem(IDC_edtHELP);
	CWnd* pHelpBtn = (CButton*) GetDlgItem(IDC_btnHELP);
	CWnd* pWnd = GetFocus();
	
	if ( pWnd == CHelpEdit || pWnd == pHelpBtn)
	{
		CString strHelpURL;
		CString strHelpText;		
		((CEdit*) GetDlgItem (IDC_edtHELP))->GetWindowText(strHelpText);
		strHelpText.Replace(_T(" "), _T("+"));		
		// 17/02/06 김혜영 : KOLAS3.net Search를 통해 검색시 404 오류 발생
/*//BEFORE-----------------------------------------------------------------------------
		strHelpURL.Format( _T("http://www.kolas3.net/bbs/board.php?bo_table=faq&sfl=wr_subject%%7C%%7Cwr_content&stx=%s&sop=and"), strHelpText);		
*///AFTER------------------------------------------------------------------------------
		CString strTmp;	//URL 인코딩
		strTmp = CLocCommonAPI::UrlEncode((LPCTSTR)strHelpText);
		strHelpURL.Format( _T("http://kolas.nl.go.kr/nltech/plib/faq/faqList.do?bbCategory=1&searchCondition=bb_content&searchKeyword=%s"), strTmp);
/*//END-------------------------------------------------------------------------------*/
		SetURL(strHelpURL);
		m_cmbURL.SetWindowText(strHelpURL);					
	}
	
	m_List.ShowWindow(SW_HIDE);				
	CString strText = _T("");
	m_cmbURL.GetWindowText(strText);
	
	SetURL(strText, TRUE);
	//DEL	
	//DEL 				// 목록에 없는 내용이면 레지스트리에 추가한다.
	//DEL 				INT nTextIndex = Get_IndexFromHistoryArray(strText);
	//DEL 				if(0 > nTextIndex)
	//DEL 				{		
	//DEL 					const INT nSize = m_aURLHistory.GetSize();
	//DEL 					if(MAX_URL_COUNT == nSize)
	//DEL 					{ 
	//DEL 						// 목록의 개수가 꽉찼으면 url 2부터 url max번까지를
	//DEL 						// url1부터 url max-1번까지 옮긴 후
	//DEL 						// 새 내용을 url max에 저장
	//DEL  						CString strData[MAX_URL_COUNT];
	//DEL 						Get_URLHistoryFromRegstry(strData, MAX_URL_COUNT-1, 2);
	//DEL 						strData[MAX_URL_COUNT-1] = strText;
	//DEL 						Set_URLHistoryToRegstry(strData, MAX_URL_COUNT, 1);
	//DEL 						
	//DEL 						// url목록을 갱신
	//DEL 						m_aURLHistory.RemoveAt(0);
	//DEL 						m_aURLHistory.Add(strText);
	//DEL 						
	//DEL 						// icon파일 갱신 및 현재 접속한 사이트 아이콘 다운로드
	//DEL 						CString strFile = ICON_FILE_PATH;
	//DEL 						strFile += _T("\\");
	//DEL 						strFile += Get_IconFileName(0);
	//DEL 						
	//DEL 						::DeleteFile(strFile);
	//DEL 						
	//DEL 						for(INT nCount=1; nCount <= MAX_URL_COUNT; nCount++)
	//DEL 						{ 	
	//DEL 							CString strFilePath = ICON_FILE_PATH _T("\\");
	//DEL 							CString strOldFilePath = strFilePath + Get_IconFileName(nCount);
	//DEL 							CString strNewFilePath = strFilePath + Get_IconFileName(nCount-1);
	//DEL 							CFileFind filefind;
	//DEL 							CFile file;
	//DEL 							if(TRUE == filefind.FindFile(strOldFilePath))
	//DEL 							{
	//DEL 								file.Rename(strOldFilePath, strNewFilePath);
	//DEL 							}			
	//DEL 							
	//DEL 						}
	//DEL 						// 현재 접속한 사이트의 아이콘을 다운받아
	//DEL 						// 이미지 리스트에 추가한뒤
	//DEL 						// 콤보박스 리스트에도 추가한다.
	//DEL 						Download_IconFile(strText, ICON_FILE_PATH, Get_IconFileName(MAX_URL_COUNT-1)); 
	//DEL 						m_aImageList.Remove(0);
	//DEL 						Add_IconToImageList(Get_IconFileName(MAX_URL_COUNT-1));
	//DEL 						
	//DEL 						m_cmbURL.DeleteItem(0);
	//DEL 						Add_ItemFromHistroyArray(MAX_URL_COUNT-1);//MAX_URL_COUNT-1);
	//DEL 
	//DEL 
	//DEL 					//DEL 	m_cmbURL.SetCurSel(MAX_URL_COUNT-1);
	//DEL 					}				
	//DEL 					// 목록의 개수가 남아있으면
	//DEL 					// url nCount+1번에 저장
	//DEL 					else if(MAX_URL_COUNT > nSize)
	//DEL 					{
	//DEL 						Set_URLHistoryToRegstry(&strText, 1, nSize+1);
	//DEL 						m_aURLHistory.Add(strText);
	//DEL 						Download_IconFile(strText, ICON_FILE_PATH, Get_IconFileName(nSize));
	//DEL 						Add_IconToImageList(Get_IconFileName(nSize));
	//DEL 						Add_ItemFromHistroyArray(nSize);
	//DEL 
	//DEL 					//DEL 	m_cmbURL.SetCurSel(nSize);
	//DEL 					}
	//DEL 					else return TRUE;
	//DEL 				}
	//DEL 				else // 이미 목록에 있는 사이트일 경우
	//DEL 				{					
	//DEL 					
	//DEL 					// 아이콘파일이 없는 경우 새로 다운받는다.				
	//DEL 					if(FALSE == Is_IconFile(nTextIndex))
	//DEL 					{
	//DEL 						Download_IconFile(strText, ICON_FILE_PATH, Get_IconFileName(nTextIndex));
	//DEL 					 	Modify_ImageToImageList(nTextIndex);
	//DEL 					}
	//DEL 				}
	m_List.Set_Visible(TRUE); 							
	
	if ( m_pView->GetSafeHwnd() != NULL ) m_pView->SendMessage(WM_USER+1010, NULL, NULL);				
	
 			
}
void CInternetNavigator::OnKillfocusedtHELP() 
{

	CString strText;
	CStatic *m_HelpMessage;		
	m_HelpMessage = (CStatic*)GetDlgItem(IDC_HELP_MESSAGE);

	( (CEdit*) GetDlgItem(IDC_edtHELP) )->GetWindowText(strText);

	if ( strText.IsEmpty() )
	{
		m_HelpMessage->ShowWindow(SW_SHOW);
	}
	else
	{
		m_HelpMessage->ShowWindow(SW_HIDE);
	}		
}

void CInternetNavigator::OnSetfocusedtHELP() 
{	
	CStatic *m_HelpMessage;		
	m_HelpMessage = (CStatic*)GetDlgItem(IDC_HELP_MESSAGE);
	m_HelpMessage->ShowWindow(SW_HIDE);
}
