// PreViewCardPrint.cpp : implementation file
//

#include "stdafx.h"
#include "PreViewCardPrint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool bGfxUserAbort;

/////////////////////////////////////////////////////////////////////////////
// CPreViewCardPrint dialog
BOOL CALLBACK EXPORT _GfxAbortProc( HDC, INT )
{
	MSG msg;
	while (!bGfxUserAbort && ::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
	{
		if (!AfxGetThread()->PumpMessage()) return FALSE;
	}
	return !bGfxUserAbort;
}



CPreViewCardPrint::CPreViewCardPrint(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreViewCardPrint)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pUserDM = NULL;
	m_nLastPage = 0;
	m_nFirstPage = 0;
	m_nCurrentPage = 0;

	m_nResolution = 1440;

	m_pUserCard = NULL;
	m_pDC = NULL;

	m_pPreviewView = NULL;
	m_bIsPrint = FALSE;
	
	// 2012.10.29 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 4회
	m_bShowKLIconYN = FALSE;
}

CPreViewCardPrint::~CPreViewCardPrint()
{
	delete m_pUserCard;
	m_pUserCard = NULL;
	delete m_pDC;
	m_pDC = NULL;
}

void CPreViewCardPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreViewCardPrint)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreViewCardPrint, CDialog)
	//{{AFX_MSG_MAP(CPreViewCardPrint)
	ON_BN_CLICKED(IDC_PREVIEW_PRINT_CPAGE, OnPreviewPrintCpage)
	ON_BN_CLICKED(IDC_btnPREVIEW_PRINT_TPAGE, OnbtnPREVIEWPRINTTPAGE)
	ON_BN_CLICKED(IDC_btnPREVIEW_PREV, OnbtnPREVIEWPREV)
	ON_BN_CLICKED(IDC_btnPREVIEW_NEXT, OnbtnPREVIEWNEXT)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_btnPREVIEW_CLOSE, OnbtnPREVIEWCLOSE)
	ON_BN_CLICKED(IDC_btnPREVIEW_FIRST, OnbtnPREVIEWFIRST)
	ON_BN_CLICKED(IDC_btnPREVIEW_LAST, OnbtnPREVIEWLAST)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreViewCardPrint message handlers

BOOL CPreViewCardPrint::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	InitESL_Mgr( _T("SM_대출증출력_미리보기") );

	// 비트맵 버튼 초기화
	m_BitmapButtonList.Add(IDC_btnPREVIEW_PREV, this , _T("..\\BMP\\SIReportEditor\\mbtn05_np.bmp"), _T("..\\BMP\\SIReportEditor\\mbtn05_d.bmp"), _T("..\\BMP\\SIReportEditor\\mbtn05_f.bmp"));
	m_BitmapButtonList.Add(IDC_btnPREVIEW_NEXT, this, _T("..\\BMP\\SIReportEditor\\mbtn06_n.bmp"), _T("..\\BMP\\SIReportEditor\\mbtn06_d.bmp"), _T("..\\BMP\\SIReportEditor\\mbtn06_f.bmp"));
	m_BitmapButtonList.Add(IDC_PREVIEW_PRINT_CPAGE, this, _T("..\\BMP\\SIReportEditor\\mbtn10_n.bmp"), _T("..\\BMP\\SIReportEditor\\mbtn10_d.bmp"), _T("..\\BMP\\SIReportEditor\\mbtn10_f.bmp"));
	m_BitmapButtonList.Add(IDC_btnPREVIEW_PRINT_TPAGE, this, _T("..\\BMP\\SIReportEditor\\mbtn11_n.bmp"), _T("..\\BMP\\SIReportEditor\\mbtn11_d.bmp"), _T("..\\BMP\\SIReportEditor\\mbtn11_f.bmp"));
	m_BitmapButtonList.Add(IDC_btnPREVIEW_CLOSE, this, _T("..\\BMP\\SIReportEditor\\mbtn09_n.bmp"), _T("..\\BMP\\SIReportEditor\\mbtn09_d.bmp"), _T("..\\BMP\\SIReportEditor\\mbtn09_f.bmp"));
	m_BitmapButtonList.Add(IDC_btnPREVIEW_FIRST, this, _T("..\\BMP\\SIReportEditor\\mbtn04_n.bmp"), _T("..\\BMP\\SIReportEditor\\mbtn04_d.bmp"), _T("..\\BMP\\SIReportEditor\\mbtn04_f.bmp"));
	m_BitmapButtonList.Add(IDC_btnPREVIEW_LAST, this, _T("..\\BMP\\SIReportEditor\\mbtn07_n.bmp"), _T("..\\BMP\\SIReportEditor\\mbtn07_d.bmp"), _T("..\\BMP\\SIReportEditor\\mbtn07_f.bmp"));
	

	CRect WndPos;
	GetDlgItem(IDC_STATIC_PREVIEW_POS)->GetWindowRect(WndPos);
	ScreenToClient(WndPos);

	m_pPreviewView = new CPreview();
	m_pPreviewView->CreateEx( WS_EX_CLIENTEDGE, NULL, NULL, WS_CHILD | WS_VISIBLE, WndPos, this, 1, NULL );	

	m_pDC = new CDC();
	CDC *pDC = m_pPreviewView->GetDC();
	m_pDC->CreateCompatibleDC( pDC );
	ReleaseDC(pDC);

	m_pPreviewView->SetDC(m_pDC);

	m_pPreviewView->m_nHeight = WndPos.Height();
	m_pPreviewView->m_nWidth = WndPos.Width();

	CBitmap bmpBuffer;
	pDC = m_pPreviewView->GetDC();
    bmpBuffer.CreateCompatibleBitmap(pDC,WndPos.Width(),WndPos.Height());
	ReleaseDC(pDC);
	m_pOldBitmap1=(CBitmap*)m_pDC->SelectObject(&bmpBuffer);


	m_pDC->SetBkMode(TRANSPARENT);
	m_pDC->SetBkColor(RGB(255,255,255) );
	
	PreViewUserCard(MOVE_CURRENT);		
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CPreViewCardPrint::PreViewUserCard(INT nMoveMode)
{
	m_nLastPage = m_pUserDM->GetRowCount()-1;


	if( nMoveMode == MOVE_NEXT )
	{
		if( m_nCurrentPage + 1 <= m_nLastPage )
			m_nCurrentPage++;			
	}
	else if( nMoveMode == MOVE_PREV )
	{
		if( m_nCurrentPage - 1 >= m_nFirstPage )
			m_nCurrentPage--;			
	}
	else if( nMoveMode == MOVE_FIRST )
	{
		m_nCurrentPage = m_nFirstPage;
	}
	else if( nMoveMode == MOVE_LAST )
	{
		m_nCurrentPage = m_nLastPage;
	}
	
	SetButton();

	ShowUserCard();

	/// 페이지 정보 보여주기
	CString sPage;
	sPage.Format( _T("페이지 : %d/%d") , m_nCurrentPage+1 , m_nLastPage+1 );

	GetDlgItem(IDC_STATIC_PAGE_INFO)->SetWindowText(sPage);

	return 0;
}

INT CPreViewCardPrint::SetPrintList( CESL_DataMgr* pUserDM )
{
	m_pUserDM = pUserDM;

	return 0;
}

INT CPreViewCardPrint::SetButton()
{
	if( m_nFirstPage == m_nCurrentPage )
	{
		GetDlgItem(IDC_btnPREVIEW_PREV)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_btnPREVIEW_PREV)->EnableWindow(TRUE);
	}

	if( m_nLastPage == m_nCurrentPage )
	{
		GetDlgItem(IDC_btnPREVIEW_NEXT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_btnPREVIEW_NEXT)->EnableWindow(TRUE);
	}
	

	return 0;
}

INT CPreViewCardPrint::ShowUserCard()
{
	INT ids;
	
	CRect WndPos;
	GetDlgItem(IDC_STATIC_PREVIEW_POS)->GetWindowRect(WndPos);

	CBrush White(RGB(255,255,255));
	m_pDC->FillRect( CRect(0,0,WndPos.Width(),WndPos.Height()) , &White );

	ids = MakeInfo(m_nCurrentPage,m_pDC,CUC_MANAGER_MODE_PREVIEW );

	ids = PrintScreen();

	m_pPreviewView->ReDraw();

	return 0;
}

INT CPreViewCardPrint::MakeInfo(INT nIndex , CDC* pDC , INT nMode )
{
	INT ids;

	/// 1. 양식 정보 구성
	ids = MakeCardFormatInfo(nIndex);
	
	/// 2. 양식 정보에 실제 데이터 삽입
	ids = MakeRealData(nIndex);
	
	/// 3. DC 생성
	ids = MakeDC(pDC , nMode );

	return 0;
}

INT CPreViewCardPrint::MakeCardFormatInfo(INT nIndex)
{
	/// 양식 정보를 참조
	CString sFormKey;
	m_pUserDM->GetCellData( _T("양식정보KEY") , nIndex , sFormKey );
	 
	CString sWhere;
	sWhere.Format( _T("CUCF.REC_KEY = %s ") , sFormKey );
	
	/// DB에서 형식별 카드 출력 정보 가져오기
	CESL_DataMgr* pCardDM = FindDM(_T("DM_대출증카드출력정보"));
	pCardDM->RefreshDataManager(sWhere);

	return 0;
	
}

INT CPreViewCardPrint::MakeRealData(INT nIndex)
{
	/// DB에서 형식별 카드 출력 정보 가져오기
	CESL_DataMgr* pCardDM = FindDM(_T("DM_대출증카드출력정보"));

	CString sType;
	CString sAlias;
	CString sIsLink;
	CString sData;
	for( INT i = 0 ; i < pCardDM->GetRowCount() ; i++ )
	{
		pCardDM->GetCellData( _T("타입") , i , sType );

		if( sType.Compare(TYPE_LINE) == 0 ) 
		{
			
		}
		else if( sType.Compare(TYPE_TEXTBOX) == 0 )
		{
			pCardDM->GetCellData( _T("알리아스명") , i , sAlias );
			pCardDM->GetCellData( _T("링크여부") , i , sIsLink );

			if( sIsLink.Compare(_T("Y")) == 0 )
			{
				m_pUserDM->GetCellData( sAlias , nIndex , sData );
				if( sAlias.Compare(_T("출력일")) == 0  || sAlias.Compare(_T("등록일")) == 0)
				{
					if( sData.GetLength() > 9 )
					{
						CString sYear = sData.Mid(0,4);
						CString sMonth = sData.Mid(5,2);
						CString sDay = sData.Mid(8,2);
						
						sMonth.Format( _T("%d") , _ttoi(sMonth) );
						sDay.Format( _T("%d") , _ttoi(sDay) );
						
						sData.Format( _T("%s년 %s월 %s일") , sYear , sMonth , sDay );	
					}
				}

				pCardDM->SetCellData( _T("내용") , sData , i );				
			}
		}
		else if( sType.Compare(TYPE_BARCODE) == 0 ) 
		{
			pCardDM->GetCellData( _T("알리아스명") , i , sAlias );
			pCardDM->GetCellData( _T("링크여부") , i , sIsLink );
			
			if( sIsLink.Compare(_T("Y")) == 0 )
			{
				m_pUserDM->GetCellData( sAlias , nIndex , sData );
				pCardDM->SetCellData( _T("내용") , sData , i );				
			}
		}
		else if( sType.Compare(TYPE_BITMAP) == 0 ) 
		{
			pCardDM->GetCellData( _T("링크여부") , i , sIsLink );
			if( sIsLink.Compare(_T("Y")) == 0 )
			{
				pCardDM->GetCellData( _T("알리아스명") , i , sAlias );
				m_pUserDM->GetCellData( sAlias , nIndex , sData );
				pCardDM->SetCellData( _T("파일경로") , sData , i );	
			}
		}
		else if( sType.Compare(TYPE_BACKGROUND) == 0 ) 
		{
		}

		// 2012.10.29 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 4회
		if(!m_bShowKLIconYN && _T("통합도서표시") == pCardDM->GetCellData(_T("알리아스명"), i))
		{
			pCardDM->SetCellData( _T("파일경로") , _T("") , i );	
		}
	}
	
	return 0;
}

INT CPreViewCardPrint::MakeDC( CDC* pDC , INT nMode )
{
	/// 객체로 만들어 화면에 그려주기
	delete m_pUserCard;
	m_pUserCard = NULL;
	if( m_pUserCard == NULL )
		m_pUserCard = new CControlObjectMgr;
	
	CESL_DataMgr* pCardDM = FindDM(_T("DM_대출증카드출력정보"));

	m_pUserCard->MakeControlObjectList( pCardDM , nMode );
	m_pUserCard->DrawDC(pDC);

	return 0;
}

void CPreViewCardPrint::OnPreviewPrintCpage() 
{
	PrintCPage();
}

INT CPreViewCardPrint::PrintCPage()
{
	PrintUserCard(m_nCurrentPage);	

	return 0;
}

void CPreViewCardPrint::OnbtnPREVIEWPRINTTPAGE() 
{
	PrintUserCard();
}


INT CPreViewCardPrint::PrintUserCard(INT nPage)
{
	INT nStartPrintPage = m_nFirstPage;
	INT nEndPrintPage = m_nLastPage+1;

	if( nPage != -1 )
	{
		nStartPrintPage = nPage;
		nEndPrintPage = nPage+1;
	}
	
	CDC* pDC = new CDC();

	// 사용할 프린터를 결정한다
	CPrintDialog pd(FALSE,  PD_ALLPAGES | PD_NOSELECTION | PD_NOPAGENUMS );
	if (pd.DoModal() != IDOK) return -1;
	m_pd.hDevMode = pd.m_pd.hDevMode;
	m_pd.hDevNames = pd.m_pd.hDevNames;
	
	LPDEVMODE pDevMode = pd.GetDevMode();
	pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
	
	pd.m_pd.hDevMode = pDevMode;

	// PRINTER DEVICE를 얻어온다.
	LPDEVNAMES lpDevNames = (m_pd.hDevNames != NULL) ? 
		(LPDEVNAMES)::GlobalLock(m_pd.hDevNames) : NULL;
	if (lpDevNames == NULL) return -1;
	LPDEVMODE  lpDevMode = (m_pd.hDevMode != NULL) ?
		(LPDEVMODE)::GlobalLock(m_pd.hDevMode) : NULL;
	if (lpDevMode == NULL) return -1;
	Sleep(10);
	
	HDC hDC = ::CreateDC((LPCTSTR)lpDevNames + lpDevNames->wDriverOffset,
		(LPCTSTR)lpDevNames + lpDevNames->wDeviceOffset,
		(LPCTSTR)lpDevNames + lpDevNames->wOutputOffset,
		lpDevMode);	
	if (hDC == NULL) return -1;
	
	if (m_pd.hDevNames != NULL) ::GlobalUnlock(m_pd.hDevNames);
	if (m_pd.hDevMode != NULL) ::GlobalUnlock(m_pd.hDevMode);	


	//pDC->Attach(pd.GetPrinterDC());
	pDC->Attach(hDC);
	pDC->m_bPrinting = TRUE;

	CPrintInfo Info;
	Info.m_rectDraw.SetRect(0,0, pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));

	INT nSaveDC = pDC->SaveDC();

	pDC->SetBkMode(TRANSPARENT); // 투명하게 한다.

	DOCINFO di = {sizeof(DOCINFO), _T("회원증 출력"), NULL}; // 출력시 이름 설정
	pDC->StartDoc(&di);
	
	for( INT i = nStartPrintPage  ; i < nEndPrintPage ; i++ )
	{
		pDC->StartPage();

		pDC->SetMapMode(MM_ANISOTROPIC);
		pDC->SetWindowExt(m_nResolution, m_nResolution);
		pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
		
		MakeInfo(i,pDC,CUC_MANAGER_MODE_PRINT );
		
		if (pDC->EndPage() < 0 || !_GfxAbortProc(pDC->GetSafeHdc(), 0)) {
			break;
		}

		m_pUserDM->SetCellData(_T("회원증출력여부"), _T("Y"), i);		
	}
	
	pDC->EndDoc();
	
	pDC->RestoreDC(nSaveDC);	// 다시 DC를 RETURN한다.

	// print CDC객체 소멸
	pDC->DeleteDC();
	pDC->Detach();
	delete pDC;

	m_bIsPrint = TRUE;

	return 0;
}


INT CPreViewCardPrint::PrintScreen()
{
	return 0;
}

void CPreViewCardPrint::OnbtnPREVIEWPREV() 
{
	PreViewUserCard(MOVE_PREV);	
}

void CPreViewCardPrint::OnbtnPREVIEWNEXT() 
{
	PreViewUserCard(MOVE_NEXT);	
}

void CPreViewCardPrint::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

void CPreViewCardPrint::OnDestroy() 
{
	m_pDC->SelectObject(m_pOldBitmap1);

	delete m_pPreviewView;
	m_pPreviewView = NULL;

	CDialog::OnDestroy();
}

void CPreViewCardPrint::OnbtnPREVIEWCLOSE() 
{
	OnCancel();	
}



void CPreViewCardPrint::OnbtnPREVIEWFIRST() 
{
	PreViewUserCard(MOVE_FIRST);	
}

void CPreViewCardPrint::OnbtnPREVIEWLAST() 
{
	PreViewUserCard(MOVE_LAST);	
}

HBRUSH CPreViewCardPrint::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
