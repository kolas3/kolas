// TocEditor_TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TocEditor_Test.h"
#include "TocEditor_TestDlg.h"

#include "ChorokTestDlg.h"
#include "YonChorokSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual VOID DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

VOID CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTocEditor_TestDlg dialog

CTocEditor_TestDlg::CTocEditor_TestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTocEditor_TestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTocEditor_TestDlg)
	m_nline = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nKindof= 0 ;
//	mKIND = _DAN_SINGLE;
	mKIND = _DAN_MULTI;
//	mKIND = _YON_;
//	mKIND = _KISA_;

	// KOL.UDF.022 시큐어코딩 보완
	m_nMokchaLine = 0;
}

VOID CTocEditor_TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTocEditor_TestDlg)
	DDX_Text(pDX, IDC_EDIT1, m_nline);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTocEditor_TestDlg, CDialog)
	//{{AFX_MSG_MAP(CTocEditor_TestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_VIEWER_BUTTON, OnViewerButton)
	ON_BN_CLICKED(IDC_DELETE_CHOROK_BUTTON, OnDeleteChorokButton)
	ON_BN_CLICKED(IDC_UPDATE_CHOROK_BUTTON, OnUpdateChorokButton)
	ON_BN_CLICKED(IDC_EDIT_CHOROK_BUTTON, OnEditChorokButton)
	ON_BN_CLICKED(IDC_VERIFY_CHOROK_BUTTON, OnVerifyChorokButton)
	ON_BN_CLICKED(IDC_SOOSIKEDITOR_BUTTON, OnSoosikEditorButton)
	ON_BN_CLICKED(IDC_SETKISANO2ALLKISA_BUTTON, OnSetKisaNo2allkisaButton)
	ON_BN_CLICKED(IDC_NEWMOKCHA_BUTTON, OnNewmokchaButton)
	ON_MESSAGE(MSG_EDIT_DAN_CHOROK, OnEditDanChorok)
	ON_MESSAGE(MSG_EDIT_YON_CHOROK, OnEditYonChorok)
	ON_MESSAGE(MSG_EDIT_KISA_CHOROK, OnEditKisaChorok)
	ON_MESSAGE(MSG_SHOW_VIEWER, OnShowViewer)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTocEditor_TestDlg message handlers

BOOL CTocEditor_TestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add _T("About...") menu item to _tsystem menu.

	// IDM_ABOUTBOX must be in the _tsystem command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's _tmain window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	TCHAR *sMokcha;
	//CString sMokcha ;
	CFile cf;
	if(!cf.Open(_T("c:\\toctmp.toc"), CFile::modeRead | CFile::typeBinary)) return FALSE;
	TCHAR cUni;
	cf.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		cf.SeekToBegin();
	}
	
	DWORD dlength = cf.GetLength();
	sMokcha = (TCHAR *)malloc(sizeof(TCHAR) * (dlength+1));
	cf.Read(sMokcha, dlength);
	cf.Close();
	sMokcha[dlength] = '\0';



//	===================================================
//	다음은 TocEditor 다이얼로그를 사용하는 예제입니다.
//	함수 호출하는 순서에 주의하여 사용하여 주십시오.
//	===================================================
// 단행본 , 연간물 상관없이 설정 - 뷰어에서 이전 다음 보기를 위해
	m_TocEditorDlg.SetMoveRecordFunc( (INT (CWnd::*)(BOOL, BOOL, CString&, BOOL*, BOOL*))MoveRecord ) ;

	if(mKIND == _DAN_SINGLE) {

		// 1. 단행본일 경우
			m_nKindof= _DAN_SINGLE;
			// 단행-단일, 단행-다권, 연속간행물 설정	_DAN_SINGLE, _DAN_MULTI, _YON_
			// 반드시 설정해주도록 한다. 연간물일 경우에는 목차키를 반드시 설정해 준다.
			if ( m_TocEditorDlg.MyCreate(this, CRect(10,10,500,500), m_nKindof ) )
				AfxMessageBox( _T("목차 편집기를 띄우지 못했습니다!") ) ;
			
		
		//	sMokcha= _T("<id>KMO199700001\r\n<call>01$a021.7$b국26e$c4\r\n<count>\r\n<date>1997\r\n<pub>국립중앙도서관\r\n<sertitle>\r\n<title>도서관정보화\r\n<body>목차\r\n표제지 = 0\r\n발간사 = i\r\n목차 = ii\r\n제1장 서론 = 1\r\n 제1절 현황 = 2\r\n 제2절 문제점 = 5\r\n제2장 본론 = 20\r\n 제1절 해결방안 = 20\r\n 제2절 향후계획 = 30\r\n제3장 결론 = 55\r\n참고문헌 = 155\r\n</body>\r\n<table>표목차\r\n<표1>한국사서교사 배치현황 = 15\r\n<표2>일본의 학교도서관 사서교사 배치기준 = 22\r\n</table>\r\n<figure>그림목차\r\n[그림1]사서교사의 직능별 학력 = 8\r\n[모형1]학교 도서관 = 12\r\n</figure>\r\n<body>Contents\r\ntitle page = 0\r\nReward = i\r\ncontents = ii\r\nchapter 1 preface = 1\r\n section 1 introduction = 2\r\n section 2 problem = 5\r\n</body>") ;
//			sMokcha= _T("<id>KMO2222\r\n<call>01  $$A001$$  B13$$C1-2$$\r\n<date>\r\n<pub>\r\n<sertitle>\r\n<title>\r\n<vol>1\r\n<count>\r\n<body>목차\r\n1 다가오는 미국의 = 11\r\n2 변화의 양상 = 34\r\n</body>\r\n</vol>\r\n<vol>2\r\n<count>\r\n<body>목차\r\n8 자신하기 시작한 = 261\r\n9 변화의 진전 = 291\r\n10 새 세대의 저쪽 = 358\r\n</body>\r\n</vol>");
			m_TocEditorDlg.SetTocFromString(sMokcha) ;

	}
	else if(mKIND == _YON_) {
			// 2. 연간물일 경우
				m_nKindof= _YON_ ;
				if ( m_TocEditorDlg.MyCreate(this, CRect(0,0,500,400), m_nKindof ) ) {
					return FALSE ;	// 실패!
				}

				// Primary 키를 DB로부터 가져오는 함수 설정 - CS 로 맞춰주어야 한다.
				m_TocEditorDlg.SetYonKeyFunc( (INT (CWnd::*)(TCHAR*))MakePrimaryKey ) ;

			

//				sMokcha= _T("<id>KSE000011632\r\n<vol>2호(II)\r\n<body>목차\r\n卷頭言 / 白雅悳 = 2[!--10000000--]\r\n이재영 수식예제$$wow~~good~~$$ \r\n<count>\r\n<date>\r\n<pub>\r\n<sertitle>\r\n<issue>\r\<body> Wow!!\r\n</body>\r\n<body>목차\r\n卷頭言 / 白雅悳 = 2[!--10000000--]\r\n創刊辭 수식예제 $$\\sum { aa }$$** !$$!!$$!金明植\r\n!$$%%%$$= 10[!--20000000--]\r\n婦人問題의一考察 : 自由思想과賢母良妻主義 / 辛日鎔 = 14[!--30000000--]\r\n</body>\r\n") ;
			//	sMokcha= _T("<id>KSE000011632\r\n<key>17638754\r\n<vol>창간호\r\n<call>\r\n<count>\r\n<date>\r\n<pub>\r\n<sertitle>\r\n<issue>\r\n<body>목차\r\n卷頭言 / 白雅悳 = 2[!--17806227--]\r\n創刊辭 / 金明植 = 4[!--17806228--]\r\n新生活新紀元의第一年第一日의나의所望 / 姜邁 = 10[!--17806229--]\r\n婦人問題의一考察 : 自由思想과賢母良妻主義 / 辛日鎔 = 14[!--17806230--]\r\n重生하라 / 金元璧 = 19[!--17806231--]\r\n新生活誕生에對하야 / 李承駿 = 22[!--17806232--]\r\n生活의不安 / 李星泰 = 24[!--17806233--]\r\n民衆精神의一考察 / 鄭栢 = 29[!--17806234--]\r\n社會運動의先驅者의出來를促하노라 / 申伯雨 = 34[!--17806235--]\r\n覺醒하라新思想을 / 革菴生 = 39[!--17806236--]\r\n婦人問題種種 / 槿園 = 42[!--17806237--]\r\n漢詩 = 46[!--17806238--]\r\n社會葬은何? / 金明植 = 47[!--17806239--]\r\n正義의疇範 / 赤笑生 = 55[!--17806240--]\r\n金剛山遊記 / 春園 = 57[!--17806241--]\r\n新生活의理想 / 公民 = 64[!--17806242--]\r\n趣旨書及組織[!--17806243--]\r\n編輯을맛치고[!--17806244--]\r\n</body>\r\n</vol>\r\n");

				// 스트링으로 목차를 넣어준다.
				m_TocEditorDlg.SetTocFromString(sMokcha) ;

				// 초록 설정.
				m_TocEditorDlg.SetExistYonChorok(_T("<ID=KDM000000001>\r\n첫번째 초록") , _T("10000000")) ;
				m_TocEditorDlg.SetExistYonChorok(_T("<ID=KDM000000002>\r\n두번째 초록") , _T("20000000")) ;
				m_TocEditorDlg.SetExistYonChorok(_T("<ID=KDM000000003>\r\n세번째 초록") , _T("30000000")) ;

				// 기사중에서 원문 구축된 기사는 삭제하지 못하도록 원문 구축된 기사제어번호를 세팅해준다.
				// added by jungjoo 2002.4.12.
				if( m_TocEditorDlg.SetWonmunExistKisaNo(_T("30000000")) < 0 )
				{
					EndDialog(FALSE) ;
				}
				if( m_TocEditorDlg.SetWonmunExistKisaNo(_T("20000000")) < 0 )
				{
					EndDialog(FALSE) ;
				}

			/*
				sMokcha= _T("<id>KSE199501211\n<key>1021005640428\n<vol>통권4\n<call>\n<count>\n<date>\n<pub>\n<sertitle>\n<issue>\n<body>목차\n읽는 설교 : 믿음의 초점 / 박보범 = 8[!--10191568--]\n시[!--10191569--]\n 새벽기도·구름 / 김보림 = 12[!--10191570--]\n 메꽃·하얀그림자 / 문창길 = 14[!--10191571--]\n 강물은 흐르면서·딴뜻 / 박재화 = 17[!--10191572--]\n 사랑의 볕·감사기도 2 / 이구재 = 19[!--10191573--]\n 꿈꾸는 나무가 되어·공책 / 이상묵 = 21[!--10191574--]\n 가을의 노래·영산강 풀잎 / 최규창 = 24[!--10191575--]\n특집 : 한국기독교문학의 방향[!--10191576--]\n 기독교 문학의 가능성 / 원형갑 = 28[!--10191577--]\n 감동과 각성의 시 / 박이도 = 40[!--10191578--]\n 한국 소설과 기독교의 만남 / 이건숙 = 45[!--10191579--]\n 기독교 아동문학의 오늘과 내일 / 김철수 = 51[!--10191580--]\n수필[!--10191581--]\n 견공과 애완견 그리고 잡종개 / 김부희 = 56[!--10191582--]\n 장인 어른 예찬 / 송길원 = 61[!--10191583--]\n동화[!--10191584--]\n 진돌이의 눈물 / 김철수 = 66[!--10191585--]\n 공부 백점 사람 마음 백점 사람 / 이태선 = 72[!--10191586--]\n여행기 : 빠리日記(3) / 박강월 = 81[!--10191587--]\n다시봄 : 양반과 허세의 심리 / 김형석 = 90[!--10191588--]\n간증문학 : 장애인이라고 결혼 못하나요 / 송명희 = 93[!--10191589--]\n</body>\n</vol>\n");
				m_TocEditorDlg.SetTocFromString(sMokcha) ;
				m_TocEditorDlg.SetExistYonChorok(_T("<ID=KDM000000001>\r\n첫번째 초록") , _T("10191568")) ;
				m_TocEditorDlg.SetExistYonChorok(_T("<ID=KDM000000002>\r\n두번째 초록") , _T("10191570")) ;
			*/
	}
	else if(mKIND == _KISA_) {

			// 3. 기사물 일때..

			m_nKindof= _KISA_ ;
			if ( m_TocEditorDlg.MyCreate(this, CRect(0,0,500,400), m_nKindof ) ) {
				return FALSE ;	// 실패!
			}

		
//			sMokcha= _T("<id>KMO2222\n<call>瓦貨窒01$$A001$$B13$$C1-2$$\n<date>\n<pub>\n<sertitle>\n<title>\n<vol>1\n<count>\n<body>목차\n1 다가오는 미국의 = 11\n2 변화의 양상 = 34\n</body>\n</vol>\n<vol>2\n<count>\n<body>목차\n8 자신하기 시작한 = 261\n9 변화의 진전 = 291\n10 새 세대의 저쪽 = 358\n</body>\n</vol>");
//			sMokcha= _T("0123456789$$<?xml version='1.0' encoding='euc-kr'?>\r\n<m:math xmlns:m='http://www.w3.org/1998/Math/MathML'>\r\n <m:mrow>r\n  <m:mfrac>\r\n   <m:mrow>\r\n    <m:mi>&#x0031;</m:mi>\r\n    <m:mi>&#x0032;</m:mi>\r\n    <m:mi>&#x0033;</m:mi>\r\n   </m:mrow>\r\n   <m:mrow>\r\n    <m:mi>&#x0031;</m:mi>\r\n    <m:mi>&#x0032;</m:mi>\r\n   </m:mrow>\r\n  </m:mfrac>\r\n  <m:mo>&#x002b;</m:mo>\r\n  <m:msub>\r\n   <m:mrow>\r\n    <m:mo>&#x222b;</m:mo>\r\n   </m:mrow>\r\n</m:msub>\r\n  <m:mrow>\r\n   <m:mi>&#x0066;</m:mi>\r\n   <m:mi>&#x0077;</m:mi>\r\n   <m:mi>&#x0065;</m:mi>\r\n   <m:mi>&#x0077;</m:mi>\r\n  </m:mrow>\r\n </m:mrow>\r\n</m:math >\r\n \r\n$$0123456789012$$<?xml version='1.0' encoding='euc-kr'?>\r\n<m:math xmlns:m='http://www.w3.org/1998/Math/MathML'>\r\n <m:mrow>\r\n  <m:munder>\r\n   <m:mrow>\r\n    <m:mo>&#x2211;</m:mo>\r\n    <m:mo>&#x0028;</m:mo>\r\n     <m:mrow>\r\n      <m:mi>&#x006e;</m:mi>\r\n      <m:mo>&#x002b;</m:mo>\r\n      <m:mi>&#x0031;</m:mi>\r\n     </m:mrow>\r\n    <m:mo>&#x0029;</m:mo>\r\n   </m:mrow>\r\n   <m:mrow>\r\n    <m:mi>&#x006b;</m:mi>\r\n    <m:mo>&#x003d;</m:mo>\r\n    <m:mi>&#x0031;</m:mi>\r\n   </m:mrow>\r\n</m:munder>\r\n </m:mrow>\r\n</m:math >\r\n$$3456789012345678901234567890") ;

			// 스트링으로 목차를 넣어준다.
			m_TocEditorDlg.SetTocFromString(sMokcha) ;
	}

/*
	// 3. 반출 포맷 가져오기 테스트
	CString sResult;
	CString sMokcha1= _T("<id>KSE000011632\r\n<key>17638754\r\n<vol>창간호\r\n<call>\r\n<count>\r\n<date>\r\n<pub>\r\n<sertitle>\r\n<issue>\r\n<body>목차\r\n卷頭言 / 白雅悳 = 2[!--17806227--]\r\n創刊辭 / 金明植 = 4[!--17806228--]\r\n新生活新紀元의第一年第一日의나의所望 / 姜邁 = 10[!--17806229--]\r\n婦人問題의一考察 : 自由思想과賢母良妻主義 / 辛日鎔 = 14[!--17806230--]\r\n重生하라 / 金元璧 = 19[!--17806231--]\r\n新生活誕生에對하야 / 李承駿 = 22[!--17806232--]\r\n生活의不安 / 李星泰 = 24[!--17806233--]\r\n民衆精神의一考察 / 鄭栢 = 29[!--17806234--]\r\n社會運動의先驅者의出來를促하노라 / 申伯雨 = 34[!--17806235--]\r\n覺醒하라新思想을 / 革菴生 = 39[!--17806236--]\r\n婦人問題種種 / 槿園 = 42[!--17806237--]\r\n漢詩 = 46[!--17806238--]\r\n社會葬은何? / 金明植 = 47[!--17806239--]\r\n正義의疇範 / 赤笑生 = 55[!--17806240--]\r\n金剛山遊記 / 春園 = 57[!--17806241--]\r\n新生活의理想 / 公民 = 64[!--17806242--]\r\n趣旨書及組織[!--17806243--]\r\n編輯을맛치고[!--17806244--]\r\n</body>\r\n</vol>\r\n");
	m_TocEditorDlg.GetBanchoolFormat(sMokcha1, sResult );
	AfxMessageBox(sResult);
*/





	if( m_TocEditorDlg.m_hWnd!= NULL)
		m_TocEditorDlg.SetFocus() ;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CTocEditor_TestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CTocEditor_TestDlg::OnPaint() 
{
//	m_nline = m_TocEditorDlg.m_TocEdit.GetFirstVisibleLine();;
//	UpdateData(FALSE);
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		INT cxIcon = GetSystemMetrics(SM_CXICON);
		INT cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		INT x = (rect.Width() - cxIcon + 1) / 2;
		INT y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The _tsystem calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTocEditor_TestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CTocEditor_TestDlg::OnViewerButton() 
{
	// TODO: Add your control notification handler code here
	OnShowViewer() ;
}

VOID CTocEditor_TestDlg::OnDeleteChorokButton() 
{
	// TODO: Add your control notification handler code here
	// 현재 커서가 위치한 라인 값을 이용해서 기사제어번호를 찾는다.
	m_nMokchaLine= m_TocEditorDlg.GetCurrentLine() ;
	CString sChorokText, sKisaNo ;
	sKisaNo= m_TocEditorDlg.GetYonKisaNo(m_nMokchaLine, false) ;

	if ( sKisaNo.IsEmpty()			// 라인에 초록이 존재 하지 않는 경우를 걸러낸다.
		|| sKisaNo==_CHOROK_ERROR_STRING_ )
		return ;

	CString sMsg ;
	sMsg= _T("기사제어번호 ") + sKisaNo + _T(" 에 대한 초록을 삭제 하겠습니까?");
	if ( IDYES == AfxMessageBox( sMsg , MB_YESNO ) ) 
	{
		// 초록 구조체에서 삭제 해준다.(삭제 플래그)
//		if ( m_TocEditorDlg.DeleteYonKisaNo(sKisaNo) < 0 ) {
//			;	// 초록 삭제 실패
//		}
		if ( m_TocEditorDlg.ReflectYonChorok2Stru(_T(""),sKisaNo) < 0 ) {
			// 실패
		}
	}

}

VOID CTocEditor_TestDlg::OnUpdateChorokButton() 
{
	// TODO: Add your control notification handler code here

	CYonChorokSet rs ;
	m_TocEditorDlg.GetResultYonChorokSet(rs) ;
	while ( rs.Next() ) {
		CString sKey= rs.GetKisaNo() ;
		CString sText= rs.GetChorokText() ;
		CString sKisaStatus= rs.GetKisaStatus() ;		
		CString sChorokStatus= rs.GetChorokStatus() ;
		CString sKisa= rs.GetKisa() ;		// 기사추가!

		CString sMsg ;
		if ( sChorokStatus==_CHOROK_STATUS_NEW_ )		// 초록이 새로 작성된 경우
		{
			sMsg = _T("초록 새로작성!\r\n") ;
			sMsg += _T("키 : ") ;
			sMsg += sKey ;
			sMsg += _T("\r\n") ;
			sMsg += _T("내용 : ") ;
			sMsg += sText ;
			sMsg += _T("\r\n") ;
			sMsg += _T("기사 : ") ;
			sMsg += sKisa ;
			AfxMessageBox(sMsg) ;
			// DB 에 업데이트 시켜준다.
		}
		else if ( sChorokStatus==_CHOROK_STATUS_MODIFY_ )	// 초록이 변경된 경우
		{
			sMsg = _T("초록 변경!\r\n") ;
			sMsg += _T("키 : ") ;
			sMsg += sKey ;
			sMsg += _T("\r\n") ;
			sMsg += _T("내용 : ") ;
			sMsg += sText ;
			sMsg += _T("\r\n") ;
			sMsg += _T("기사 : ") ;
			sMsg += sKisa ;
			AfxMessageBox(sMsg) ;
			// DB 에 업데이트 시켜준다.
		}
		else if ( sChorokStatus==_CHOROK_STATUS_DELETE_ )	// 초록이 삭제된 경우
		{
			sMsg = _T("초록 삭제!\r\n") ;
			sMsg += _T("키 : ") ;
			sMsg += sKey ;
			sMsg += _T("\r\n") ;
			sMsg += _T("내용 : ") ;
			sMsg += sText ;
			sMsg += _T("\r\n") ;
			sMsg += _T("기사 : ") ;
			sMsg += sKisa ;
			AfxMessageBox(sMsg) ;
			// DB 에 업데이트 시켜준다.
		}

		if ( sKisaStatus==_CHOROK_STATUS_NEW_ )		// 기사제어번호가 새로 작성된 경우
		{
			sMsg = _T("기사제어번호 새로작성!\r\n") ;
			sMsg += _T("키 : ") ;
			sMsg += sKey ;
			sMsg += _T("\r\n") ;
			sMsg += _T("내용 : ") ;
			sMsg += sText ;
			sMsg += _T("\r\n") ;
			sMsg += _T("기사 : ") ;
			sMsg += sKisa ;
			AfxMessageBox(sMsg) ;
		}
		else if ( sKisaStatus==_CHOROK_STATUS_DELETE_ )		// 기사제어번호가 삭제된 경우
		{
			sMsg = _T("기사제어번호 삭제!\r\n") ;
			sMsg += _T("키 : ") ;
			sMsg += sKey ;
			sMsg += _T("\r\n") ;
			sMsg += _T("내용 : ") ;
			sMsg += sText ;
			sMsg += _T("\r\n") ;
			sMsg += _T("기사 : ") ;
			sMsg += sKisa ;
			AfxMessageBox(sMsg) ;
		}
	}
	rs.Free();

}

VOID CTocEditor_TestDlg::OnEditChorokButton() 
{
	// TODO: Add your control notification handler code here
	if (m_nKindof==_YON_)// 연간물일 경우
	{
		OnEditYonChorok();
	}
	else if (m_nKindof == _KISA_)
	{
		OnEditKisaChorok();
	}		
	else	// 단행본일 경우
	{
		OnEditDanChorok();
	}
}

// 목차를 업데이트 하기 전에
// 목차에 옆의 기사제어번호가 정확한 위치에 들어갔는지 확인해 볼 수 있는 함수
// 목차에서 기사제어번호가 제대로 삽입되었는지 검사해주자.
VOID CTocEditor_TestDlg::OnVerifyChorokButton() 
{
	// TODO: Add your control notification handler code here
	CString sMokcha ;
	sMokcha= m_TocEditorDlg.GetFinalToc() ;

	INT nCnt= m_TocEditorDlg.CheckYonKisaNo(sMokcha) ;
	if ( nCnt ) {	// nCnt 는 목차에서 에러 건수를 나타내준다.
		CString sError ;
		sError.Format(_T("목차에서 기사제어번호 오류 건수가 %d 건 검색되었습니다."), nCnt) ;
		AfxMessageBox(sError) ;
	}
	else {
		AfxMessageBox(_T("이상없음!")) ;
	}
}

VOID CTocEditor_TestDlg::OnSoosikEditorButton() 
{
	// TODO: Add your control notification handler code here
	m_TocEditorDlg.ExecuteSoosikEditor() ;
}

VOID CTocEditor_TestDlg::OnSetKisaNo2allkisaButton() 
{
	// TODO: Add your control notification handler code here
	m_TocEditorDlg.SetKisaNo2AllKisa() ;
}

VOID CTocEditor_TestDlg::OnNewmokchaButton() 
{
	// TODO: Add your control notification handler code here
	m_TocEditorDlg.TocNewEdit() ;
}

// 오른쪽 버튼 팝업메뉴의 초록 편집(단행본)
VOID CTocEditor_TestDlg::OnEditDanChorok()
{
//	AfxMessageBox( _T("이곳에서 단행본 초록 편집 실행하십시오!") ) ;
	ASSERT(m_TocEditorDlg) ;
	CChorokTestDlg dlg(_T("<ID=KDM000000000>\r\n테스트 단행초록"), &m_TocEditorDlg) ;
	dlg.DoModal();
}

// 오른쪽 버튼 팝업메뉴의 초록 편집(연간물)
VOID CTocEditor_TestDlg::OnEditYonChorok()
{
//	AfxMessageBox( _T("이곳에서 연간물 초록 편집 실행하십시오!") ) ;
	ASSERT(m_TocEditorDlg);
	CChorokTestDlg dlg(_T("<id=KDM000000000>\r\n<kabs>테스트 연간초록\r\n\r\n</kabs>\r\n<fabs>abstract\r\n\r\n</fabs>"), &m_TocEditorDlg) ;
	dlg.DoModal();
}

VOID CTocEditor_TestDlg::OnEditKisaChorok()
{
	ASSERT(m_TocEditorDlg);
	CChorokTestDlg dlg(_T("<ID=KDM000000000>\r\n테스트 기사초록"), &m_TocEditorDlg) ;
	dlg.DoModal();
}

// 오른쪽 버튼 팝업메뉴의 목차 뷰어 보기()
VOID CTocEditor_TestDlg::OnShowViewer()
{
	m_TocEditorDlg.ShowViewer(TRUE, FALSE) ;
}

// 임시 MakePrimaryKey 함수
INT CTocEditor_TestDlg::MakePrimaryKey(TCHAR* pKey)
{
	CString strKey;
	CStdioFile file;
	if ( file.Open(_T("key.txt"),CFile::modeRead | CFile::typeBinary)==NULL )
		return -1 ;
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
		
	file.ReadString(strKey);
	INT nID=_ttoi(strKey);
	file.Close();
	nID++ ;

	TCHAR szID[64] ;
	wmemset( szID, '\0', sizeof(szID) ) ;
	_itot(nID, szID, 10) ;
	_tcscpy( pKey, szID );
	
	if (file.Open(_T("key.txt"),CFile::modeCreate|CFile::modeWrite | CFile::typeBinary)==NULL)
		return -2 ;
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	strKey.Format(_T("%d"),nID);
	file.WriteString(strKey);
	file.Close();
	return 0 ;
}

// 임시 MoveRecord 함수 - 뷰어에서 이전 다음 보여줄 때 필요하다.
INT CTocEditor_TestDlg::MoveRecord( BOOL bMokcha , BOOL bNext , CString& strNewString , BOOL *pbPrevEnable , BOOL *pbNextEnable )
{
	*pbPrevEnable = FALSE ;
	*pbNextEnable = TRUE ;

	if ( bMokcha )
		strNewString = _T("<id>MoveRecord함수 테스트\r\n<key>\r\n<call>test\r\n<count>123\r\n<date>2001/10\r\n<pub>eco\r\n<sertitle>\r\n<issue>\r\n<body>본문\r\nPrev, Next Test!!!! = 1[!--10000000--]\r\n</body>\r\n<table>\r\n</table>\r\n<figure>\r\n</figure>\r\n<body>\r\n첫번째 기사 - 이 부분은 Prev, Next Test $$ \\sum { aa } $$ 랍니다. = 2[!--20000000--]\r\n</body>\r\n<body>\r\nPrev, Next Test = 3[!--30000000--]\r\n</body>") ;
	else
		strNewString = _T("<ID=KDM999999999>\r\n바뀐 초록!") ;

	return 0 ;
}

VOID CTocEditor_TestDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CStringArray arrString ;
	m_TocEditorDlg.GetYonAllKisaNo(arrString) ;

	for(INT i=0 ; i<arrString.GetSize() ; i++ )
	{
		AfxMessageBox(arrString.GetAt(i) );
	}
}
