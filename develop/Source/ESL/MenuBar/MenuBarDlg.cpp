// MenuBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MenuBarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuBarDlg dialog

CMenuBarDlg::CMenuBarDlg(INT nAliasCount, CWnd* pParent /*=NULL*/)
	: CDialog(CMenuBarDlg::IDD, pParent)
{
	// 메뉴의 갯수를 멜록 
	ArrayMenu = (MIMenu*)malloc(sizeof(MIMenu) * 1);

	m_nBmpInfoCount = 0;
	m_IsBkPaint = FALSE;	

	// 총 메뉴들의 갯수 
	m_AliasCount = nAliasCount;

	// 버튼 리스트 
	m_pListIDButton= NULL;

	// 한줄로 생기는 것들의 ID 갯수
	m_nButtonCount = 0;


	// LeftMenu와 RightMenu의 경계 Index
	m_nBorderLine = 0;

	m_CurrentID = m_BeforeID = 0;

	m_Statichwnd = NULL;
	m_pStaticWnd = NULL;

	m_pParent = pParent;

	m_nAddMenuIndex = 0 ;
	m_XPosion = 0;

	m_AddFlag = FALSE ;

	// Font
	m_Font.CreateFont( 12, 0,0,0, FW_THIN ,0,0,0, DEFAULT_CHARSET,
			    OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_DONTCARE,_T("굴림체"));
	// - default
	m_Font.GetLogFont(&font);
	font.lfUnderline = TRUE;
    m_UnderLineFont.CreateFontIndirect(&font);
	// - Bold, UnderLine
	font.lfWeight = FW_BOLD;
	m_CurrentMenuFont.CreateFontIndirect(&font);
	// - Bold
	font.lfUnderline = FALSE;
	m_BoldFont.CreateFontIndirect(&font);

	draw_rect1.UpperLeft  = 0;
	draw_rect1.LowerRight = 1;	
}

CMenuBarDlg::~CMenuBarDlg()
{
	if( m_pListIDButton != NULL )
	{
		free(m_pListIDButton);
		m_pListIDButton = NULL;
	}
	if( ArrayMenu != NULL )
	{
		free(ArrayMenu);
		ArrayMenu = NULL;
	}

	for ( INT i = 0 ; i < 3 ; i ++ )
	{
		m_BkDC[i].DeleteDC();
		m_BkBmp[i].DeleteObject();
	}

	m_Font.DeleteObject();
	m_UnderLineFont.DeleteObject();
	m_BoldFont.DeleteObject();
}

void CMenuBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMenuBarDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMenuBarDlg, CDialog)
	//{{AFX_MSG_MAP(CMenuBarDlg)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuBarDlg message handlers

BOOL CMenuBarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 자료를 넣어 둔다. 인자 값을 받아서..
	//	PutDataInStruct();
	//	Init();
	//	MakeLineMenu(1110);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 라인 메뉴를 만드는 곳
// 아이디, 오른쪽에있는 시블링 메뉴인지 아닌지, 온싸이즈에서 온 것인지 
INT CMenuBarDlg::MakeLineMenu(INT n_ID, BOOL nSiblingFlag, BOOL bSubFlag)
{	
	// 온싸이즈에서 온것이 아닐때 그냥넘어갈수 있다.
	// 온싸이즈에서 온것이면 같은 ID라도 다르게 그려질수 있으므로
	if( !bSubFlag )
	{
		if( m_CurrentID == n_ID )
			return 0;
	}

	// BMP 그림들의 갯수 리셋 
	m_nBmpInfoCount = 0;

	m_AddFlag = FALSE;

	m_CurrentID = n_ID;
	TCHAR szTemp[128] = {NULL};

	INT *pnIDlist;

	pnIDlist = NULL;

	INT i, j, nSun, nCount, nTemp = 0, ids = 0;

	for( i = 0; i < 50; i++ )
	{
		m_Static[i].DestroyWindow();
		m_Static[i].m_hWnd = NULL;
	}
	m_pAddMenuStatic[0].DestroyWindow();
	m_pAddMenuStatic[0].m_hWnd = NULL;

	nCount = 0;
	
	for( i = 0; i < m_AliasCount; i++ )
	{		
		nSun = i;
		if ( ArrayMenu[nSun].nID == n_ID)
		{
			nCount = 1;
			pnIDlist = (INT*)malloc( sizeof(INT) * 1 );
			pnIDlist[0] = n_ID;
			
			for( j = 0; j < m_AliasCount; j++ )					
			{
				if(		( ArrayMenu[j].nLevel == ArrayMenu[nSun].nLevel - 1 ) 
					&&	( ArrayMenu[j].nIndex == ArrayMenu[nSun].nParentIndex ) )
				{					
					nSun = j;
					nCount ++;	
					pnIDlist = (INT*)realloc(pnIDlist, sizeof(INT) * nCount);
					pnIDlist[nCount-1] = ArrayMenu[j].nID;
					j=-1;
				}	
			}
		}		
	}
 	//++2008.08.27 ADD BY CJY {{++
	// nID에서 찾지 못한 경우는 nMenuID로 다시 한 번 검색한다.
	// 이렇게 하는 이유는 MENU_ID값이 5자리인 경우
	// INT CMenuBarDlg::InputDate에서 nID값이 변경되기 때문이다.
	// 이때 ID값은 항상 5000이상이다.
 	if(1 >= nCount && 5000 <= n_ID)
 	{
 		CString strID = _T("");
 		strID.Format(_T("%d"), n_ID);
 	for( i = 0; i < m_AliasCount; i++ )
 	{		
 		nSun = i;
 		if ( ArrayMenu[nSun].nMenuID == n_ID)
 		{
 			nCount = 1;
 			pnIDlist = (INT*)malloc( sizeof(INT) * 1 );
 			pnIDlist[0] =  n_ID;
 			
 			for( j = 0; j < m_AliasCount; j++ )					
 			{
 				if(		( ArrayMenu[j].nLevel == ArrayMenu[nSun].nLevel - 1 ) 
 					&&	( ArrayMenu[j].nIndex == ArrayMenu[nSun].nParentIndex ) )
 				{					
 					nSun = j;
 					nCount ++;	
 					pnIDlist = (INT*)realloc(pnIDlist, sizeof(INT) * nCount);
 					pnIDlist[nCount-1] = ArrayMenu[j].nID;
 					j=-1;
 				}	
 			}
 		}		
		if ( ArrayMenu[i].nMenuID == n_ID)
		{
			// 메뉴생성시에는 nMenuID가 아닌 nID를 설정해주어야 한다.
			pnIDlist[0] = ArrayMenu[i].nID;
		}
 	}	
 	}
 	//--2008.08.27 ADD BY CJY --}}

	ids = MakeLeftLineMenu(nCount, pnIDlist, nSiblingFlag);
	if( ids ) return -5;

	if( pnIDlist != NULL )	pnIDlist = NULL;
	if( !nSiblingFlag )	n_ID = m_BeforeID;
	
	// 같은 레벨 단위의 것들 나타내기
	nCount = 0;
	
	for( i = 0; i < m_AliasCount; i++ )
	{		
		if( ArrayMenu[i].nID == n_ID )
		{
			if( pnIDlist != NULL ) pnIDlist = NULL;
			pnIDlist = (INT*)malloc( sizeof(INT) * 1 );
			
			for( j = 0; j < m_AliasCount; j++ )
			{
				if(		( ArrayMenu[i].nLevel == ArrayMenu[j].nLevel ) 
					&&	( ArrayMenu[i].nParentIndex == ArrayMenu[j].nParentIndex) )
				{
					nCount++;
					pnIDlist = (INT*)realloc(pnIDlist, sizeof(INT) * nCount );
					pnIDlist[nCount-1] = ArrayMenu[j].nID;
				}				
			}
			break;
		}
	}
	//++2008.08.27 ADD BY CJY {{++
	// nID에서 찾지 못한 경우는 nMenuID로 다시 한 번 검색한다.
	// 이렇게 하는 이유는 MENU_ID값이 5자리인 경우
	// INT CMenuBarDlg::InputDate에서 nID값이 변경되기 때문이다.
	// 이때 ID값은 항상 5000이상이다.
	if(0 == nCount && 5000 <= n_ID)
	{
		for( i = 0; i < m_AliasCount; i++ )
		{		
			if( ArrayMenu[i].nMenuID == n_ID )
			{
				if( pnIDlist != NULL ) pnIDlist = NULL;
				pnIDlist = (INT*)malloc( sizeof(INT) * 1 );
				
				for( j = 0; j < m_AliasCount; j++ )
				{
					if(		( ArrayMenu[i].nLevel == ArrayMenu[j].nLevel ) 
						&&	( ArrayMenu[i].nParentIndex == ArrayMenu[j].nParentIndex) )
					{
						nCount++;
						pnIDlist = (INT*)realloc(pnIDlist, sizeof(INT) * nCount );
						pnIDlist[nCount-1] = ArrayMenu[j].nID;
					}				
				}
				break;
			}
		}
	}
	//--2008.08.27 ADD BY CJY --}}
	ids = MakeRightLineMenu(nCount, pnIDlist);
	if( ids ) return -10;

	if( pnIDlist != NULL )
	{
		free(pnIDlist);
		pnIDlist = NULL;
	}

	m_BeforeID = n_ID;

	return 0;
}

INT CMenuBarDlg::MakeLeftLineMenu(INT ButtonCount, INT *pIDButtonList, BOOL nSiblingFlag)
{
	INT ids, i, n_IdButton;
	CString strName, strID ;
	CRect MainRect, TempRect;

	GetParent()->GetClientRect(&TempRect);
	MainRect = TempRect;

	// 간격수정 
	// 위에서부터의 높이 
	MainRect.top = TempRect.top + 5 ;// + 11;
	// 왼쪽에서부터의 시작점
	MainRect.left = TempRect.left + 1 ;
	// 전체높이 bottom - top 
	MainRect.bottom = 25 ;//TempRect.bottom - 5 ;//MainRect.top + 11 ;

	if ( NULL != m_pListIDButton )
	{
		m_pListIDButton = NULL;
	}

	m_pListIDButton = (INT*)malloc(sizeof(INT)*ButtonCount);
	m_nBorderLine = m_nButtonCount = ButtonCount;

	for( i = 0 ; i < ButtonCount * 2 - 1 ; i++ )
	{			
		if( 0 ==  i % 2 )
		{
			strID.Format(_T("2%04d"),pIDButtonList[ButtonCount - i/2 -1]);
			
			n_IdButton = _ttoi(strID.GetBuffer(0));

			m_pListIDButton[i/2] = n_IdButton;

			INT nMaxLength = WideCharToMultiByte(CP_ACP, 0, GetNameBYID(pIDButtonList[ButtonCount - i/2-1]).GetBuffer(0), -1, NULL, 0, NULL,NULL);	
			// 간격수정
			// _GAPLEGTH 는 다음에 오는 것과의 거리 
			MainRect.right = MainRect.left + 7 * (nMaxLength-1) + _LEFTGAPLEGTH ;
			
			if( NULL != m_Static[i].m_hWnd )
			{
				m_Static[i].DestroyWindow();
			}

			ids = m_Static[i].Create(GetNameBYID(pIDButtonList[ButtonCount - i/2-1]).GetBuffer(0), NORMAL_STYLE , MainRect , this, n_IdButton);
			if( 0 == ids ) return -10;
			m_Static[i].SetWindowText(GetNameBYID(pIDButtonList[ButtonCount - i/2-1]).GetBuffer(0));
		}
		else
		{
			// 간격수정
			// left는 이전에 오는 것과의 거리 
			//MainRect.left += 3 ;
			// right는 현재 것의 크기 ( 그림으로 대체 된후 그림의 크기와 마춰야함 )
			MainRect.right = MainRect.left + 10;//+12;

			if( NULL != m_Static[i].m_hWnd )
			{
				m_Static[i].DestroyWindow();
			}

			m_nBmpInfoCount++;

			BmpInfoList[m_nBmpInfoCount-1].rcPosition = MainRect;
			BmpInfoList[m_nBmpInfoCount-1].rcPosition.left += 2;
			BmpInfoList[m_nBmpInfoCount-1].nKind = 0;
			BmpInfoList[m_nBmpInfoCount-1].nID = 0;

			ids = m_Static[i].Create(_T(""), NORMAL_STYLE , MainRect , this, IDB_bTEST1 /*+ ((i+1)/2)-1*/);
			if( 0 == ids ) return -10;
		}
		m_Static[i].GetClientRect(&TempRect);
		// 새로만든것의 오른쪽끝을 다음에 쓸 것의 맨왼쪽으로 설정 
		MainRect.left += TempRect.right - TempRect.left ;
	}

	// 가운데에 있는 걸 그려 준다. 왼쪽끝의 것과의 거리는 + 로 설정  
	m_nBmpInfoCount++;
	BmpInfoList[m_nBmpInfoCount-1].rcPosition = MainRect ;
	BmpInfoList[m_nBmpInfoCount-1].rcPosition.left = BmpInfoList[m_nBmpInfoCount-1].rcPosition.right + 7 ;
	BmpInfoList[m_nBmpInfoCount-1].nKind = 1;

	if( !nSiblingFlag )
	{
	}	
	else
	{
		m_LastMenuRect = MainRect;
	}

	return 0;
}

INT CMenuBarDlg::MakeRightLineMenu(INT ButtonCount, INT *pIDButtonList)
{
	INT ids, i, n_IdButton;
	CString strName, strID ;
	CRect MainRect, TempRect;

	MainRect = m_LastMenuRect;

	// 왼쪽 깊이메뉴와 오른쪽 Sibling 와의 거리 
	MainRect.left += 25;

	m_nButtonCount += ButtonCount;

	m_pListIDButton = (INT*)realloc( m_pListIDButton, sizeof(INT*)* m_nButtonCount);	

	GetClientRect(&m_ParentRect);

	for( i = 0 ; i < ButtonCount * 2 - 1 ; i++ )
	{		
		if( 0 ==  i % 2 )
		{
			// Real Size 구하기	
			INT nMaxLength = WideCharToMultiByte(CP_ACP, 0, GetNameBYID(pIDButtonList[i/2]).GetBuffer(0), -1, NULL, 0, NULL,NULL);	
			//MainRect.right = MainRect.left + 14 * GetNameBYID(pIDButtonList[i/2]).GetLength();
			MainRect.right = MainRect.left + 6 * ( nMaxLength -1) + _RIGHTGAPLEGTH ;

			strID.Format(_T("3%04d"),pIDButtonList[i/2]);
			
			n_IdButton = _ttoi(strID.GetBuffer(0));
			
			m_pListIDButton[i/2 + (m_nButtonCount- ButtonCount)] = n_IdButton;

			// 맨 왼쪽에서 부터의 일정거리를 떨어진 곳에서 부터
			// 오른쪽 끝으로부터 24떨어진 곳이 현재 생성중인 것을 가리게 될시 ADDMENU스태틱을 생성한다.
			if( ( m_AddFlag == FALSE ) && ( MainRect.left > m_ParentRect.left + 300 ) && ( m_ParentRect.right < MainRect.right + 24 /*MainRect.right > m_ParentRect.right - (MainRect.right - MainRect.left) - 60*/ ) )
			{
				TempRect = MainRect;
				// 간격수정
				// ADD메뉴의 스태틱 크기 조정 
				TempRect.right = TempRect.left + 12;
				
				if( NULL != m_pAddMenuStatic[0].m_hWnd )
				{
					m_pAddMenuStatic[0].DestroyWindow();
				}
				
				m_pAddMenuStatic[0].Create(_T("▶"), NORMAL_STYLE , TempRect , this, IDB_ADDMENU);
				
				m_nAddMenuIndex = i/2 + (m_nButtonCount- ButtonCount);

				// 표시와 겹치는 사이 표시 스태틱 삭제
				m_Static[i-1+ ((m_nButtonCount - ButtonCount) *2-1) ].DestroyWindow();	

				// ADDMENU가 생성되었다는 것을 나타내는 플래그 
				m_AddFlag = TRUE;
			}
			else
			{
				// 에드메뉴가 생기면 그림도 표시하지 않는다. 
				if ( m_AddFlag != TRUE )
				{
					m_nBmpInfoCount += 2;
					
					BmpInfoList[m_nBmpInfoCount-2].rcPosition = BmpInfoList[m_nBmpInfoCount-1].rcPosition = MainRect;
					BmpInfoList[m_nBmpInfoCount-2].rcPosition.left -= 5;
					BmpInfoList[m_nBmpInfoCount-1].rcPosition.left = MainRect.right ;
					
					BmpInfoList[m_nBmpInfoCount-2].nID = BmpInfoList[m_nBmpInfoCount-1].nID = n_IdButton;				
					
					if( m_CurrentID == (n_IdButton %10000) )
					{
						BmpInfoList[m_nBmpInfoCount-1].nKind = 5;
						BmpInfoList[m_nBmpInfoCount-2].nKind = 4;
					}
					else
					{
						BmpInfoList[m_nBmpInfoCount-1].nKind = 3;
						BmpInfoList[m_nBmpInfoCount-2].nKind = 2;
					}
				}
			}
			
			if( m_AddFlag != TRUE )
			{			
				if( NULL != m_Static[i].m_hWnd )
				{
					m_Static[i+((m_nButtonCount-ButtonCount)*2-1)].DestroyWindow();
				}
				ids = m_Static[i+((m_nButtonCount-ButtonCount)*2-1)].Create(GetNameBYID(pIDButtonList[i/2]), NORMAL_STYLE , MainRect , this, n_IdButton);
				if( 0 == ids ) return -10;
				m_Static[i+ ((m_nButtonCount - ButtonCount) *2-1) ].SetWindowText(GetNameBYID(pIDButtonList[i/2]));				
			}
		}
		else
		{
			if( m_AddFlag != TRUE )
			{
				//MainRect.left += 3 ;
				// 간격수정
				// 오른쪽에 있는 메뉴들의 틈사이 거리 
				MainRect.right = MainRect.left+12+8;
				
				if( NULL != m_Static[i].m_hWnd )
				{
					m_Static[i+ ((m_nButtonCount - ButtonCount) *2-1)].DestroyWindow();
				}

				ids = m_Static[i+ ((m_nButtonCount - ButtonCount) *2-1) ].Create(_T(""), NORMAL_STYLE , MainRect , this, IDB_bTEST2 /*+ ((i+1)/2)-1*/);
				if( 0 == ids ) return -10;
				//m_Static[i+ ((m_nButtonCount - ButtonCount) *2-1) ].SetWindowText(_T("-"));			
			}
		}	
		
		m_Static[ i+ ((m_nButtonCount - ButtonCount) *2-1) ].GetClientRect(&TempRect);
		
		MainRect.left += TempRect.right - TempRect.left ;
	}

	return 0;
}

CString CMenuBarDlg::GetNameBYID(INT n_ID)
{
	INT i;
	for (i = 0 ; i < m_AliasCount ; i ++ )
	{		
		if ( ArrayMenu[i].nID == n_ID)
		{
			return ArrayMenu[i].strAlias;
		}
			
	}
	//++2008.08.27 ADD BY CJY {{++
	// nID에서 찾지 못한 경우는 nMenuID로 다시 한 번 검색한다.
	// 이렇게 하는 이유는 MENU_ID값이 5자리인 경우
	// INT CMenuBarDlg::InputDate에서 nID값이 변경되기 때문이다.
	// 이때 ID값은 항상 5000이상이다.
	if(5000 <= n_ID)
	{
		for (i = 0 ; i < m_AliasCount ; i ++ )
		{		
			if ( ArrayMenu[i].nMenuID == n_ID)
			{
				return ArrayMenu[i].strAlias;
			}
			
		}
	}
	//--2008.08.27 ADD BY CJY --}}

	return _T("오류");
}

// 클릭했을 떄 메시지 들어오는곳 
BOOL CMenuBarDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	WORD wControlID = LOWORD(wParam);

	if ( HIWORD(wParam) == STN_CLICKED  )
	{
		INT ids;
		if( IDB_ADDMENU == wControlID )
		{
			ids = MakeAddMenu();
			if( ids ) return FALSE;
			return TRUE;
		}


		if ( (wControlID > 20000) && (wControlID < 44000) ) 
		{			
			for( INT i = 0 ; i < m_nButtonCount ; i ++ )
			{
				if ( m_pListIDButton[i] == wControlID  )
				{	
					HWND		hWnd;
					hWnd = GetDlgItem(wControlID)->GetSafeHwnd();

					m_Statichwnd = hWnd;

					ids = MakeMenuByID( wControlID , FALSE);
					if(ids) return FALSE;

					return TRUE;
				}
			}						
		}
		if ( (wControlID > 1000) && (wControlID < 6000) ) 
		{
			ids = SendIDToParent(wControlID);
			if( ids ) return FALSE;

			ids = MakeMenuByID( wControlID , TRUE);	
			if( ids ) return FALSE;
			
			return TRUE;
		}
    }
	return CDialog::OnCommand(wParam, lParam);
}

// 아이디를 받아서 그거에 대한 처리해주는곳 ( 클릭 했을 떄 ) 클릭이 모냐면 
INT CMenuBarDlg::MakeMenuByID(INT n_ID, BOOL fListflag)
{
	INT ids, nSunIndex;;
	INT nILevel, nJLevel, nKLevel;

	// ID 의 IJK값을 가져온다. 
	// ID가없으면 5를 리턴한다. 
	ids = TakeIJK(n_ID % 10000, &nILevel, &nJLevel, &nKLevel );
	if( ids ) return -15;

	// 오른쪽에 있는 sibling이면
	if( n_ID / 10000 == 3 )
	{
		// 자식이있는 경우 
		if( GetCheckSun( nILevel, nKLevel, &nSunIndex ) )
		{
			nILevel += 1;
			nJLevel = nKLevel ;
			nKLevel = nSunIndex ;			

			// 자식부터 시작하는 메뉴를 만든다. 
			// 상관없음 첫번째 인자 안씀 
			ids = AutoMakeMenu( n_ID , nILevel, nJLevel, nKLevel, !fListflag);
			if( ids ) return -20;
		}
		else
		{
			// 그외의 경우 
			ids = SendIDToParent(n_ID);
			if( ids ) return -5;
			
			ids = MakeLineMenu(n_ID % 10000, FALSE);
			if( ids ) return -10;
		}
		
		return 0;
	}
		
	if( fListflag )
	{
		// 메뉴안에서 고른ID 메시지 일 때 
		ids = MakeLineMenu(n_ID % 10000);
		if( ids ) return -10;
	}	

	// 자신부터 시작하는 메뉴를 만든다. 
	// 상관없음 첫번째 인자 안씀 
	ids = AutoMakeMenu(n_ID , nILevel, nJLevel, nKLevel, !fListflag);
	if( ids ) return -20;

	return 0;
}

INT CMenuBarDlg::SendIDToParent(INT n_ID)
{
	CString strMenuId;
	strMenuId.Format(_T("%d"), n_ID % 10000);
	if ( NULL != m_pParent )
	{
		m_pParent->SendMessage(WM_MANUBARRESULT , NULL, (LPARAM)(LPCTSTR)strMenuId);
	}
	return 0;
}

// ID 를주면 I,J,K를 리턴한다.
INT CMenuBarDlg::TakeIJK(INT n_ID, INT *nILevel, INT *nJLevel, INT *nKLevel)
{
	INT ids = 0, i ;

	for (i = 0 ; i < m_AliasCount ; i ++ )
	{		
		if ( ArrayMenu[i].nID == n_ID  )
		{
			*nILevel = ArrayMenu[i].nLevel;
			*nJLevel = ArrayMenu[i].nParentIndex;
			*nKLevel = ArrayMenu[i].nIndex;
			return 0;
		}
	}
	//++2008.08.27 ADD BY CJY {{++
	// nID에서 찾지 못한 경우는 nMenuID로 다시 한 번 검색한다.
	// 이렇게 하는 이유는 MENU_ID값이 5자리인 경우
	// INT CMenuBarDlg::InputDate에서 nID값이 변경되기 때문이다.
	// 이때 ID값은 항상 5000이상이다.
	if(5000 <= n_ID)
	{
		for (i = 0 ; i < m_AliasCount ; i ++ )
		{		
			if ( ArrayMenu[i].nMenuID == n_ID  )
			{
				*nILevel = ArrayMenu[i].nLevel;
				*nJLevel = ArrayMenu[i].nParentIndex;
				*nKLevel = ArrayMenu[i].nIndex;
				return 0;
			}
		}
	}
	//--2008.08.27 ADD BY CJY --}}
	return 5;
}

// CMenu 메뉴를 만드는 곳 
INT CMenuBarDlg::AutoMakeMenu(INT n_ID, INT niLevel, INT njLevel, INT nkLevel, BOOL bShowFlag, BOOL bAddMenu )
{
	INT ids, i, j, k;
	INT ILevel, JLevel, KLevel;
	CPoint point;
	CString strName;

	BOOL flag;

	GetCursorPos(&point);

	CMenu mainmenu;

	// 메인메뉴 생성
	ids = mainmenu.CreatePopupMenu();
	if( ids == 0 ) return -5;

	INT nItemList[16];
	INT nCount;

	// 같은 그룹의 아이디들을 가져오기
	ids = GetSameGroupID( niLevel, njLevel, &nCount, nItemList);
	if( ids ) return -5;

	Init();
	
	// 메인 메뉴들을 구성하는 부분 
	for( i = 0 ; i < nCount ; i ++ )
	{
		flag = TRUE;

		if( bAddMenu )
		{
			for( j = 0; j < m_nAddMenuIndex ; j ++ )
			{
				if( nItemList[i] == (m_pListIDButton[j]%10000) )
				{
					flag = FALSE;
					break;
				}
			}
		}

		if( flag == TRUE )
		{
			
			ids = GetIJK( nItemList[i], &ILevel, &JLevel, &KLevel, &strName);	
			if( ids ) return -10;
			
			if( nMaxDepth == 1 || FALSE == GetCheckSun( ILevel, KLevel) )
			{
				//INT nTemp = GetIDByIJK( ILevel, JLevel, KLevel );
				ids = mainmenu.AppendMenu(MF_STRING , nItemList[i] , strName);
				if( 0 == ids ) return -15;
			}
			else
			{
				ids = mainmenu.AppendMenu(MF_STRING | MF_POPUP, (UINT)hmenuPopup[ILevel]/*[JLevel]*/[KLevel], strName);
				if( 0 == ids ) return -15;
			}
		}
	}


	// 그외의 메뉴들을 구성 
	for( i = niLevel ; i < nMaxDepth ; i ++ )
	{/*
		for( j = 0 ; j <= nMaxItem ; j ++ )
		{*/
			for( k = 0 ; k <= nMaxItem ; k ++ )
			{				
				//strName = GetMenuName(i,j,k);
				strName = GetMenuName2(i,k);
				if( strName.GetLength() != 0 )
				{
					if( i != niLevel )
					{
						ids = GetParentIJK(i, 0, k, &ILevel, &JLevel, &KLevel);
						if( ids )return -20;
						
						if( GetCheckSun( i, k ) )
						{
							ids = AppendMenu(hmenuPopup[ILevel]/*[JLevel]*/[KLevel], MF_STRING| MF_POPUP, (UINT)hmenuPopup[i]/*[j]*/[k], strName);
							if( 0 == ids )	
							{
								DWORD temp ;
								temp = GetLastError();
								return temp;
							}
						}
						else
						{
							INT nTemp = GetIDByIJK2( i, k );
							ids = AppendMenu(hmenuPopup[ILevel]/*[JLevel]*/[KLevel], MF_STRING , nTemp, strName);
							if( 0 == ids ) 	
							{
								DWORD temp ;
								temp = GetLastError();
								return temp;
							}
						}
					}
				}
			}
		//}
	}

	// 화면에 구성한 메뉴들을 보여주는 부분 
	if( bShowFlag )
	{
		CRect rect;
		if( GetDlgItem(n_ID) !=NULL )
		{
			GetDlgItem(n_ID)->GetWindowRect(rect);
			
			ids = mainmenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN  , rect.left, rect.bottom+1 , this);		
			if ( 0 == ids ) 
			{
				DWORD temp ;
				temp = GetLastError();
				return temp;
			}
		}
	}

	Free();
	
	ids = mainmenu.DestroyMenu();
	if ( 0 == ids ) 
	{
		DWORD temp ;
		temp = GetLastError();
		return temp;
	}
	

	Invalidate(FALSE);
	
	return 0;
}

INT CMenuBarDlg::GetMaxDepth(INT niLevel, INT nkLevel)
{
	INT nCount, j;

	nCount = 0;

	for( j = 0 ; j < m_AliasCount ; j ++ )
	{
		if( ArrayMenu[j].nLevel > nCount )
		{
			nCount = ArrayMenu[j].nLevel;
		}
	} 

	return nCount + 1;
}

INT CMenuBarDlg::GetMaxItem(INT niLevel, INT nkLevel)
{
	INT nCount, j;

	nCount = 0;

	for( j = 0 ; j < m_AliasCount ; j ++ )
	{
		if( ArrayMenu[j].nIndex > nCount )
		{
			nCount = ArrayMenu[j].nIndex;
		}
	} 

	return nCount ;
}

INT CMenuBarDlg::GetSameGroupID(INT niLevel, INT njLevel, INT *Count, INT *nItemList)
{
	INT nCount, i;

	nCount = 0;

	for( i = 0 ; i < m_AliasCount ; i ++ )
	{		
		if( (ArrayMenu[i].nLevel == niLevel) && (ArrayMenu[i].nParentIndex == njLevel) )
		{
			nCount++;
			nItemList[nCount-1] = ArrayMenu[i].nID;
		}
	}
	*Count = nCount;

	return 0;
}

INT CMenuBarDlg::GetIJK(INT n_ID, INT *ILevel, INT *JLevel, INT *KLevel, CString *strName)
{
	INT nCount, i;

	nCount = 0;

	for( i = 0 ; i < m_AliasCount ; i ++ )
	{
		if( ArrayMenu[i].nID == n_ID )
		{
			*ILevel = ArrayMenu[i].nLevel;
			*JLevel = ArrayMenu[i].nParentIndex;
			*KLevel = ArrayMenu[i].nIndex;
			strName->Format(_T("%s"), ArrayMenu[i].strAlias );
		}
	}

	return 0;
}

BOOL CMenuBarDlg::GetCheckSun(INT niLevel, INT nkLevel, INT *nSunIndex)
{
	INT nCount, j;

	nCount = 0;

	for( j = 0 ; j < m_AliasCount ; j ++ )
	{		
		if( ArrayMenu[j].nLevel == niLevel+1 )
		{
			if( ArrayMenu[j].nParentIndex == nkLevel )
			{
				if( nSunIndex != NULL )
				{
					*nSunIndex = ArrayMenu[j].nIndex;
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}

INT CMenuBarDlg::GetIDByIJK(INT i, INT j, INT k)
{
	INT l;

	for ( l = 0 ; l < m_AliasCount ; l ++ )
	{
		if ( (ArrayMenu[l].nLevel == i) && (ArrayMenu[l].nParentIndex == j) && (ArrayMenu[l].nIndex == k))
		{
			return ArrayMenu[l].nID;
		}
	}
	return -5;
}

INT CMenuBarDlg::GetIDByIJK2(INT i, INT k)
{
	INT l;

	for (l = 0 ; l < m_AliasCount ; l ++ )
	{
		if ( (ArrayMenu[l].nLevel == i) && (ArrayMenu[l].nIndex == k))
		{
			return ArrayMenu[l].nID;
		}
	}
	return -5;
}

CString CMenuBarDlg::GetMenuName(INT niLevel, INT njLevel, INT nkLevel)
{
	INT i; 

	for (i = 0 ; i < m_AliasCount ; i ++ )
	{		
		if ( ArrayMenu[i].nLevel == niLevel )
		{
			if( ArrayMenu[i].nParentIndex == njLevel )
			{
				if( ArrayMenu[i].nIndex == nkLevel )
				{
					return (CString) ArrayMenu[i].strAlias;
				}
			}
		}
	}
	return _T("");
}

CString CMenuBarDlg::GetMenuName2(INT niLevel, INT nkLevel)
{
	INT i; 

	for (i = 0 ; i < m_AliasCount ; i ++ )
	{		
		if ( ArrayMenu[i].nLevel == niLevel )
		{
			if( ArrayMenu[i].nIndex == nkLevel )
			{
				return (CString) ArrayMenu[i].strAlias;
			}
		}
	}
	return _T("");
}

INT CMenuBarDlg::GetParentIJK(INT i, INT j, INT k, INT *ILevel, INT *JLevel, INT *KLevel)
{
	INT l;
	j = GetParentIndex( i, k );
	for (l = 0 ; l < m_AliasCount ; l ++ )
	{		
		if ( (ArrayMenu[l].nLevel == i-1) && (ArrayMenu[l].nIndex == j) )
		{
			*ILevel = ArrayMenu[l].nLevel ;
			*JLevel = ArrayMenu[l].nParentIndex ;
			*KLevel = ArrayMenu[l].nIndex ;
		}
	}

	return 0;
}

INT CMenuBarDlg::GetParentIndex(INT i, INT k)
{
	INT l;
	for (l = 0 ; l < m_AliasCount ; l ++ )
	{		
		if ( (ArrayMenu[l].nLevel == i) && (ArrayMenu[l].nIndex == k) )
		{
			return ArrayMenu[l].nParentIndex;
		}
	}

	return 0;
}


INT CMenuBarDlg::InputDate(CObList *listMenuItem)
{
	CMenuItem *pMenuItem;

	INT i;
	INT nPos;
	INT nIndex = 0;
	CString strTempName;

	INT nRows = listMenuItem->GetCount();

	POSITION pos;
	pos = listMenuItem->GetHeadPosition();

	m_AliasCount = nRows ;

	nIndex = 3;

	// 메뉴의 갯수를 멜록 
	ArrayMenu = (MIMenu *)realloc(ArrayMenu, sizeof(MIMenu) * nIndex);

	// 단행 연속 공통은 기본으로 생성 
	_tcscpy(ArrayMenu[0].strAlias, _T("단행"));
	_tcscpy(ArrayMenu[1].strAlias,  _T("연속"));
	_tcscpy(ArrayMenu[2].strAlias,  _T("공통"));
	ArrayMenu[0].nParentIndex = 0;	
	ArrayMenu[1].nParentIndex = 0;
	ArrayMenu[2].nParentIndex = 0;
	ArrayMenu[0].nIndex = 0;	
	ArrayMenu[1].nIndex = 1;
	ArrayMenu[2].nIndex = 2;
	ArrayMenu[0].nID = 1000;	
	ArrayMenu[1].nID = 2000;
	ArrayMenu[2].nID = 3000;
	ArrayMenu[0].nLevel = 0;
	ArrayMenu[1].nLevel = 0;
	ArrayMenu[2].nLevel = 0;



	// 쭉돌면서 리얼록을 계속 한다..

	// 스타일이 s인것이 나오면 1개를 더 생성하여 
	// 부모로 쓴다. 인덱스가 1일때만, 그리고 자신은 그것의 밑이됨 그건 디비인덱스 제어 

	for( i = 0 ; i < m_AliasCount ; i ++ )
	{		
		pMenuItem = NULL;
		pMenuItem = (CMenuItem *)listMenuItem->GetNext(pos);

		if( ( pMenuItem->m_nILevel == NULL ) && ( pMenuItem->m_nJLevel == NULL ) ) 
		{
			continue;
		}

		// 자신이 아닌 자신의 부모를 새로 생성하는 구역 
		// 스타일이 멀티 일때, 인덱스가 1일때 한번만, 3레벨일때 
		if( ( pMenuItem->m_strStyle == _T("S") ) && ( pMenuItem->m_nIndex == 1 ) && ( pMenuItem->m_nILevel == 3 ) )
		{	
			ArrayMenu = (MIMenu *)realloc(ArrayMenu, sizeof(MIMenu) * (nIndex + 1));
			
			ArrayMenu[nIndex].nLevel = pMenuItem->m_nILevel-1;

			strTempName = pMenuItem->m_strGroupAlias;
			nPos = pMenuItem->m_strGroupAlias.Find(_T("_"));

			strTempName.Delete(0, nPos+1);
			
			// 이름은 언더바까지 잘라낸다 ex) CO6_목록일관성관리 => 목록일관성관리
			_tcscpy(ArrayMenu[nIndex].strAlias,  strTempName );

			// 아이디가 4천이 넘으면 
			if( _ttoi(pMenuItem->m_strMenuID) > 4000 )
			{
				// 부모 인덱스를 21로 설정한다.
				ArrayMenu[nIndex].nParentIndex = 21;
			}
			else
			{
				// 다른 경우는 맨앞두자리로 00을 붙인다 ex) 3910 -> 3900
				for( INT j = 0 ; j < nIndex ; j ++ )
				{
					if( (_ttoi(pMenuItem->m_strMenuID) / 100 * 100 ) == ArrayMenu[j].nID )
						ArrayMenu[nIndex].nParentIndex = ArrayMenu[j].nIndex;
				}
			}
			// 인덱스는 그것의 부모라고 지정한 인덱스로 한다 ( DB제어 )
			ArrayMenu[nIndex].nIndex = pMenuItem->m_nJLevel;
			// ID는 5로 시작하는 유일한 숫자로 한다 ( 모든메뉴의 갯수가 999 이하일떄만 가능 ) 
			ArrayMenu[nIndex].nID = 5000 + nIndex;
			nIndex++;
		}

		// 자신을 저장하는 구역 이름과 ijk를 저장한다.
		// 현재 자신을 저장한다.
		// 메뉴의 갯수를 멜록 
		ArrayMenu = (MIMenu *)realloc(ArrayMenu, sizeof(MIMenu) * (nIndex + 1));		


		// 1레벨의 것들 처리 
		// i레벨이 1이고 자신의 인덱스가 1일때 
		if( ( pMenuItem->m_nILevel == 1 ) && ( pMenuItem->m_nIndex == 1 ) )
		{
			strTempName = pMenuItem->m_strGroupAlias;
			nPos = pMenuItem->m_strGroupAlias.Find(_T("."));

			strTempName.Delete(0, nPos+1);

			// 점 이후를 이름으로 쓴다 ( 단행.정리 => 정리 )
			_tcscpy(ArrayMenu[nIndex].strAlias,  strTempName );
			// 아이디는 151 => 1500 식으로 변형한다.
			ArrayMenu[nIndex].nID = _ttoi(pMenuItem->m_strMenuID) / 10 * 100;
		}
		else
		{
			// 그외의 경우는 이름은 캡션의 이름을 쓴다. ( 기증자관리 ) 
			_tcscpy(ArrayMenu[nIndex].strAlias,  pMenuItem->m_strCaption );

			if( _ttoi(pMenuItem->m_strMenuID) > 9999 )
			{
				//아이디가 9999보다 크면 즉, 5자리아이디이면 ( 4랩의 것들 ) 5000+로 유일한 값을 만들어 준다.
				ArrayMenu[nIndex].nID = 5000 + nIndex;
				//++2008.08.27 ADD BY CJY {{++
				// 코라스3에서 ID를 전달해줄 때는 nID가 아닌
				// 실제 MENU_ID값이 넘어오기 때문에 이 값을 비교하기 위해
				// 실제 MENU_ID값을 변수에 저장시킨다
				ArrayMenu[nIndex].nMenuID = _ttoi(pMenuItem->m_strMenuID);
				pMenuItem->m_nID = 5000 + nIndex;				
				//--2008.08.27 ADD BY CJY --}}
			}
			else
			{
				// 그외의 경우는 그냥 아이디를 쓴다. 
				ArrayMenu[nIndex].nID = _ttoi(pMenuItem->m_strMenuID);
			}
		}

		// IJK 들은 있는 그대로 저장한다. 
		ArrayMenu[nIndex].nLevel = pMenuItem->m_nILevel;
		ArrayMenu[nIndex].nParentIndex = pMenuItem->m_nJLevel;
		ArrayMenu[nIndex].nIndex = pMenuItem->m_nKLevel;

		nIndex++;
	}

	m_AliasCount = nIndex ;

	ModifyStyle(0, /*WS_CLIPCHILDREN |*/ WS_CLIPSIBLINGS);

	return 0;
}


INT CMenuBarDlg::PutDataInStruct()
{
	// 인풋을 보내오면 받는다.

// 예제 인풋 

	_tcscpy(ArrayMenu[0].strAlias, _T("단행"));
	_tcscpy(ArrayMenu[1].strAlias,  _T("연속"));

	_tcscpy(ArrayMenu[2].strAlias,  _T("수서대상"));
	_tcscpy(ArrayMenu[3].strAlias,  _T("희망자료관리"));
	_tcscpy(ArrayMenu[4].strAlias,  _T("반입자료관리"));

	_tcscpy(ArrayMenu[5].strAlias,  _T("등록"));
	_tcscpy(ArrayMenu[6].strAlias,  _T("등록관리"));
	_tcscpy(ArrayMenu[7].strAlias,  _T("등록자료관리"));
	_tcscpy(ArrayMenu[8].strAlias,  _T("EPC일괄부여"));

	ArrayMenu[0].nParentIndex = 0;	
	ArrayMenu[1].nParentIndex = 0;

	ArrayMenu[2].nParentIndex = 0;
	ArrayMenu[3].nParentIndex = 0;
	ArrayMenu[4].nParentIndex = 0;

	ArrayMenu[5].nParentIndex = 0;
	ArrayMenu[6].nParentIndex = 1;
	ArrayMenu[7].nParentIndex = 2;
	ArrayMenu[8].nParentIndex = 2;

	ArrayMenu[0].nIndex = 0;	
	ArrayMenu[1].nIndex = 1;

	ArrayMenu[2].nIndex = 0;
	ArrayMenu[3].nIndex = 0;
	ArrayMenu[4].nIndex = 1;

	ArrayMenu[5].nIndex = 1;
	ArrayMenu[6].nIndex = 2;
	ArrayMenu[7].nIndex = 0;
	ArrayMenu[8].nIndex = 1;


	ArrayMenu[0].nID = 1000;	
	ArrayMenu[1].nID = 4000;

	ArrayMenu[2].nID = 1100;
	ArrayMenu[3].nID = 1110;
	ArrayMenu[4].nID = 1120;

	ArrayMenu[5].nID = 1300;
	ArrayMenu[6].nID = 1310;
	ArrayMenu[7].nID = 1311;
	ArrayMenu[8].nID = 1312;
	
	ArrayMenu[0].nLevel = 0;	
	ArrayMenu[1].nLevel = 0;

	ArrayMenu[2].nLevel = 1;
	ArrayMenu[3].nLevel = 2;
	ArrayMenu[4].nLevel = 2;

	ArrayMenu[5].nLevel = 1;
	ArrayMenu[6].nLevel = 2;
	ArrayMenu[7].nLevel = 3;
	ArrayMenu[8].nLevel = 3;

	_tcscpy(ArrayMenu[9].strAlias,  _T("공통"));
	ArrayMenu[9].nParentIndex = 0;
	ArrayMenu[9].nIndex = 2;
	ArrayMenu[9].nID = 3000;
	ArrayMenu[9].nLevel = 0;


	return 0;
}

BOOL CMenuBarDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CDialog::Create(IDD, pParentWnd);
}

INT CMenuBarDlg::Init()
{
	INT i, k ;
	CString strName;

	nMaxDepth = GetMaxDepth(0, 0)+1;
	nMaxItem  = GetMaxItem(0, 0)+1;

	// 최대 깊이 멜록 ( 레벨의 깊이 )
	hmenuPopup = (HMENU**)malloc(sizeof(HMENU**) * nMaxDepth);
	for (i = 0 ; i < nMaxDepth ; i ++ )
	{
		hmenuPopup[i] = (HMENU*)malloc(sizeof(HMENU*) * nMaxItem);
		for ( k = 0 ; k <= nMaxItem ; k ++ )
		{
			strName = GetMenuName2(i,k);
 			if( strName.GetLength() != 0 )
			{
				hmenuPopup[i][k] = ::CreatePopupMenu();				
			}				
		}	
	}

	return 0;
}

INT CMenuBarDlg::Free()
{
	INT i, k ;
	CString strName;

	nMaxDepth = GetMaxDepth(0, 0)+1;
	nMaxItem  = GetMaxItem(0, 0)+1;

	// 최대 깊이 멜록 ( 레벨의 깊이 )
	for (i = 0 ; i < nMaxDepth ; i ++ )
	{
		for ( k = 0 ; k < nMaxItem ; k ++ )
		{
			strName = GetMenuName2(i,k);
			if( strName.GetLength() != 0 )
			{
				INT ids = DestroyMenu(hmenuPopup[i][k]);
				if( 0 == ids )	
				{
					DWORD temp ;
					temp = GetLastError();
					//return temp;
				}
			}				
		}
		free(hmenuPopup[i]);
	}
	free(hmenuPopup);

	return 0;
}

HBRUSH CMenuBarDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if( nCtlColor == CTLCOLOR_STATIC )
    {	
		// 배경투명 
		pDC->SetBkMode(TRANSPARENT);
		// 글씨색
		pDC->SetTextColor(TEXTCOLOR);
		// Add Menu Color (▶)
		if( pWnd->GetDlgCtrlID() == IDB_ADDMENU  )
		{
			pDC->SetTextColor(ADDMENUCOLOR);
			OldFont = pDC->SelectObject(&m_Font);
			return (HBRUSH)GetStockObject(NULL_BRUSH);
		}
		// Menu Color
		else
		{	
			for( INT i = 0; i < m_nButtonCount; i++ )
			{
				if(	( pWnd->GetDlgCtrlID() == m_pListIDButton[i] ) )
				{
					// 마우스가 위치한 Static
					if( m_Statichwnd == pWnd->GetSafeHwnd() )
					{
						// 글씨가 들어간 배경을 변경한다.
						pDC->SetTextColor(CHOOSE_TXTCOLOR);	
						OldFont = pDC->SelectObject(&m_UnderLineFont);
						
						// Left Menu 마지막 Index는 굵게
						if( m_nBorderLine-1 == i)
						{
							pDC->SetTextColor(CHOOSE_TXTCOLOR);
							OldFont = pDC->SelectObject(&m_CurrentMenuFont);
						}
					}
					// 마우스가 위치하지 않은 Static
					else
					{
						CRect rect;
						GetDlgItem(m_pListIDButton[i])->GetClientRect(rect);
						ReDrawBG(pDC, rect);
						OldFont = pDC->SelectObject(&m_Font);	
						
						// Left Menu 마지막 Index는 굵게
						if( m_nBorderLine-1 == i)
						{
							pDC->SetTextColor(TEXTCOLOR);
							OldFont = pDC->SelectObject(&m_BoldFont);
						}
					}
					return (HBRUSH)GetStockObject(NULL_BRUSH);
				}
			}
			OldFont = pDC->SelectObject(&m_Font);
			return (HBRUSH)GetStockObject(NULL_BRUSH);
		}
	}
	return hbr;
}


BOOL CMenuBarDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	HWND temp,temp2;
	temp = pWnd->GetSafeHwnd();	

	// 현재 커서가 있는 곳이랑 그전에 커서가 있던게 다를 때 
	if( m_Statichwnd != temp )
	{
		CRect rect;
		::GetClientRect(m_Statichwnd,&rect);

		temp2 = this->GetSafeHwnd();

		if( m_Statichwnd != temp2)
		 	InvalidateRect(rect,TRUE);		
		else
		 	Invalidate(FALSE);

		m_Statichwnd = temp;
	}
	else
	{
		return TRUE;
	}

	CPoint	pt;			// 마우스 커서 위치를 저장할 객체
	CRect	rc;			// 스태틱 컨트롤의 위치를 저장할 객체.
	GetCursorPos(&pt);	// 마우스 커서의 위치를 찾아온다.
	
	if( GetDlgItem(IDB_ADDMENU) != NULL )
	{
		GetDlgItem(IDB_ADDMENU)->GetWindowRect(rc);
		
		// 만약 마우스가 스태틱 컨트롤 위에 와있으면..
		if(rc.PtInRect(pt))
		{
			::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND));
			//GetDlgItem(m_pListIDButton[i])->Invalidate(FALSE);
			return TRUE;
		}
	}
	for( INT i = 0; i < m_nButtonCount; i++ )
	{		
		// 스태틱 컨트롤의 위치를 찾아온다.
		if( GetDlgItem(m_pListIDButton[i]) != NULL )
		{
			GetDlgItem(m_pListIDButton[i])->GetWindowRect(rc);
			
			// 만약 마우스가 스태틱 컨트롤 위에 와있으면..
			if(rc.PtInRect(pt))
			{
				::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND));
				//GetDlgItem(m_pListIDButton[i])->Invalidate(FALSE);
				return TRUE;
			}
		}
	}

	/*
	// ] 이 모양의 것들을 기준으로 [] 한쌍씩 비교 
	for( i = m_nBmpInfoCount-1 ; i >= 0 ; i-=2 )
	{
		// 2보다 클때 즉 선택이 되었는지 안되었는지의 [] 들일 경우에
		if( BmpInfoList[i].nKind >= 2 )
		{
			// 마우스 커서가 사이안에 위치하는지 확인
			GetDlgItem(BmpInfoList[i-1].nID)->GetWindowRect(rc);
			if(rc.PtInRect(pt))
			{
				// 안에 위치하고 있으면 양쪽의 것들을 선택으로 셋팅하고 
				BmpInfoList[i-1].nKind = 4;
				BmpInfoList[i].nKind = 5;

				for( INT j = 0 ; j < m_nBmpInfoCount ; j ++)
				{
					// 자신이거나 자신의전것 일때는 원상태로 돌리지 않음 ( 안선택된 상태로 )
					//                    I
					// EX)   [    ]  [    ]  [    ]
					//               J    J      이런 경우에는 스킵    
					if( (i != j) && (i != j+1) )
					{
						if( BmpInfoList[j].nKind == 5 )
							BmpInfoList[j].nKind = 3;
						if( BmpInfoList[j].nKind == 4 )
							BmpInfoList[j].nKind = 2;
					}
				}
				Invalidate();
			}			
		}
	}
	*/

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CMenuBarDlg::OnEraseBkgnd(CDC* pDC) 
{

	if(!m_IsBkPaint)
	{
		InitBMP(pDC);
		m_IsBkPaint = true;
	}

	// 배경색 다시 칠해주기 
	FillBackground(pDC);


	// 그림 넣어 주기 
	for( INT i =0 ; i < m_nBmpInfoCount ; i ++ )
	{
		// 위에서 아래로 높이 설정 
		pDC->BitBlt( BmpInfoList[i].rcPosition.left , 9 ,info[BmpInfoList[i].nKind].bmWidth,
			info[BmpInfoList[i].nKind].bmHeight,&m_BkDC[BmpInfoList[i].nKind], 0, 0, SRCCOPY);		
	}

	// 가려진 스태틱 그려주기 
	Invalidate(FALSE);

	return FALSE;
}

VOID CMenuBarDlg::FillBackground(CDC *pDC)
{
	if( pDC->GetSafeHdc() == NULL )
		return;

	CRect rect;
	GetClientRect(&rect);
	
	TRIVERTEX data1[2];

	data1[0].x = rect.left;	
	data1[0].y = rect.top+1;		
	data1[0].Red   = 252 * 255;
	data1[0].Green = 253 * 255;
	data1[0].Blue  = 253 * 255;
	data1[0].Alpha = 0x0000;

	data1[1].Red   = 231 * 255;
	data1[1].Green = 231 * 255;
	data1[1].Blue  = 231 * 255;
	data1[1].Alpha = 0x0000;
	data1[1].x  = rect.right;
	data1[1].y  = rect.bottom;
	
	::GradientFill(pDC->m_hDC, data1, 2, &draw_rect1, 1, GRADIENT_FILL_RECT_V);

}

INT CMenuBarDlg::MakeAddMenu()
{
//	CMenu mainmenu;

	INT ilevel, JLevel, KLevel;

	CString strName;

	GetIJK(m_pListIDButton[m_nAddMenuIndex]%10000, &ilevel, &JLevel, &KLevel, &strName);


	AutoMakeMenu( IDB_ADDMENU, ilevel, JLevel, KLevel, TRUE, TRUE);

	Invalidate(FALSE);

	return 0;

}

void CMenuBarDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if(  abs(m_XPosion - cx) > 5 )
	{
		KillTimer(1);
		SetTimer(1, 200, NULL);
		m_XPosion = cx;
	}
}

void CMenuBarDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if( nIDEvent == 1 )
	{
		MakeLineMenu(m_CurrentID % 10000, TRUE, TRUE);
		KillTimer(1);
	}

	CDialog::OnTimer(nIDEvent);
}


void CMenuBarDlg::ReDrawBG(CDC *pDC, CRect rect)
{	
	if( pDC->GetSafeHdc() == NULL )
		return;
		
	TRIVERTEX data1[2];
	
	data1[0].x = rect.left;	
	data1[0].y = rect.top;		
	data1[0].Red   = 250 * 255;
	data1[0].Green = 251 * 255;
	data1[0].Blue  = 251 * 255;
	data1[0].Alpha = 0x0000;
	
	data1[1].Red   = 233 * 255;
	data1[1].Green = 233 * 255;
	data1[1].Blue  = 233 * 255;
	data1[1].Alpha = 0x0000;
	data1[1].x  = rect.right;
	data1[1].y  = rect.bottom;
	
	::GradientFill(pDC->m_hDC, data1, 2, &draw_rect1, 1, GRADIENT_FILL_RECT_V);
}

void CMenuBarDlg::InitBMP(CDC *pDC)
{
	for( INT i = 0 ; i < 6 ; i ++ )
	{
		// prevent continuous loading.
		INT ids =  m_BkBmp[i].LoadBitmap(IDB_LEFTBMP + i) ;
		if( ids == 0)
			return ;
		m_BkBmp[i].GetObject(sizeof(BITMAP),&info[i]);
			
		m_BkDC[i].CreateCompatibleDC(pDC);
		m_BkDC[i].SelectObject(&m_BkBmp[i]);
	}
	return;	
}
