// BibilographyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BibilographyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBibilographyDlg dialog
#define	_DAN_SINGLE		0
#define	_DAN_MULTI		1
#define	_YON_			2
#define	_KISA_			3

#define BTN_WIDTH		75
#define BTN_HEIGHT		21
#define BTN_BLANK		10
#define BTN_LEFT		150
#define	BTN_TOP			237


CBibilographyDlg::CBibilographyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBibilographyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBibilographyDlg)
	m_nIsKind = _DAN_SINGLE;
	m_str1 = _T("");
	m_str2 = _T("");
	m_str3 = _T("");
	m_str4 = _T("");
	m_str5 = _T("");
	m_str6 = _T("");
	m_str7 = _T("");
	m_str8 = _T("");
	//}}AFX_DATA_INIT
}


VOID CBibilographyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBibilographyDlg)
	DDX_Text(pDX, IDC_EDIT_ID, m_str1);
	DDX_Text(pDX, IDC_EDIT_CALL, m_str2);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_str3);
	DDX_Text(pDX, IDC_EDIT_DATE, m_str4);	
	DDX_Text(pDX, IDC_EDIT_PUB, m_str5);
	DDX_Text(pDX, IDC_EDIT_SERTITLE, m_str6);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_str7);
	DDX_Text(pDX, IDC_EDIT_TITLE2, m_str8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBibilographyDlg, CDialog)
	//{{AFX_MSG_MAP(CBibilographyDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBibilographyDlg message handlers

VOID CBibilographyDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData() ;

	CreateMessage() ;

	CDialog::OnOK() ;
}

VOID CBibilographyDlg::CreateMessage()
{
	CString tmp ;
	INT i;

	m_strBibilography.Empty() ;

	switch (m_nIsKind) {
	case _KISA_:
		tmp.Format( _T("<id>%s\r\n") , m_str1 ) ;
		m_strBibilography += tmp ;

		tmp.Format( _T("<vol>%s\r\n") , m_str2 ) ;
		m_strBibilography += tmp ;
		break;

		
	case _YON_:
		for(i=0;i<8;i++) {
			switch(i) {
			case 0: tmp.Format( _T("<id>%s\r\n") , m_str1 ); break;
			case 1: tmp.Format( _T("<key>%s\r\n") , m_str2 ); break;
			case 2: tmp.Format( _T("<sertitle>%s\r\n") , m_str3 ); break;
			case 3: tmp.Format( _T("<title>%s\r\n") , m_str4 ); break;
			case 4: tmp.Format( _T("<issue>%s\r\n") , m_str5 ); break;
			case 5: tmp.Format( _T("<date>%s\r\n") , m_str6 ); break;
			case 6: tmp.Format( _T("<call>%s\r\n") , m_str7 ); break;
			case 7: tmp.Format( _T("<count>%s\r\n") , m_str8 ); break;
			}
			m_strBibilography += tmp ;
		}
		break;

	case _DAN_SINGLE:
		for(i=0;i<7;i++) {
			switch(i) {
			case 0: tmp.Format( _T("<id>%s\r\n") , m_str1 ); break;
			case 1: tmp.Format( _T("<call>%s\r\n") , m_str2 ); break;
			case 2:	tmp.Format( _T("<count>%s\r\n") , m_str3 ); break;
			case 3: tmp.Format( _T("<date>%s\r\n") , m_str4 ); break;
			case 4: tmp.Format( _T("<pub>%s\r\n") , m_str5 ); break;
			case 5: tmp.Format( _T("<sertitle>%s\r\n") , m_str6 ); break;
			case 6: tmp.Format( _T("<title>%s\r\n") , m_str7 ); break;
			}
			m_strBibilography += tmp ;
		}
		break;
	}

}

BOOL CBibilographyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString cstmp;
	INT m_nBtnSize = 0;
	INT i;

	switch (m_nIsKind) {
	case _DAN_SINGLE:
		SetWindowText(_T("서지사항 입력 - 단행물"));
		GetDlgItem(IDC_STATIC0 + 7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC0_DESC + 7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ID + 7)->ShowWindow(SW_HIDE);
		m_nBtnSize = 1;
		break;

	case _YON_:
		SetWindowText(_T("서지사항 입력 - 연속간행물"));
		for(i=0;i<8;i++) {
			switch(i) {
				case 0: cstmp.Format(_T("제어번호")); break;
				case 1: cstmp.Format(_T("권호 key")); break;
				case 2: cstmp.Format(_T("총 서 명")); break;
				case 3: cstmp.Format(_T("서    명")); break;
				case 4: cstmp.Format(_T("권 호 명")); break;
				case 5: cstmp.Format(_T("발 행 일")); break;
				case 6: cstmp.Format(_T("청구기호")); break;
				case 7: cstmp.Format(_T("총페이지")); break;
			}
			GetDlgItem(IDC_STATIC0 + i)->SetWindowText(cstmp);
		}
		for(i=0;i<8;i++) {
			switch(i) {
				case 0: cstmp.Format(_T("<id>")); break;
				case 1: cstmp.Format(_T("<key>")); break;
				case 2: cstmp.Format(_T("<sertitle>")); break;
				case 3: cstmp.Format(_T("<title>")); break;
				case 4: cstmp.Format(_T("<issue>")); break;
				case 5: cstmp.Format(_T("<date>")); break;
				case 6: cstmp.Format(_T("<call>")); break;
				case 7: cstmp.Format(_T("<count>")); break;
			}
			GetDlgItem(IDC_STATIC0_DESC + i)->SetWindowText(cstmp);
		}
		m_nBtnSize = 0;
		break;

	case _KISA_:
		SetWindowText(_T("서지사항 입력 - 기시물"));
		GetDlgItem(IDC_STATIC0)->SetWindowText(_T("기사제어번호"));
		GetDlgItem(IDC_STATIC0_DESC)->SetWindowText(_T("<id>"));
		GetDlgItem(IDC_STATIC1)->SetWindowText(_T("호수"));
		GetDlgItem(IDC_STATIC1_DESC)->SetWindowText(_T("<vol>"));
		for(INT i=0;i<6;i++) {
			GetDlgItem(IDC_STATIC2 + i)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC2_DESC + i)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_COUNT + i)->ShowWindow(SW_HIDE);
		}
		m_nBtnSize = 5;

		
		break;
	}

	RECT rect;
	GetDlgItem(IDC_STATIC_OUTLINE)->GetWindowRect(&rect);
	GetDlgItem(IDC_STATIC_OUTLINE)->SetWindowPos(NULL, 0,0, rect.right - rect.left, rect.bottom - rect.top - 30 * m_nBtnSize, SWP_NOMOVE);

	GetDlgItem(IDOK)->MoveWindow(BTN_LEFT, BTN_TOP - 30 * m_nBtnSize,
		BTN_WIDTH, BTN_HEIGHT , TRUE);

	GetDlgItem(IDCANCEL)->MoveWindow(BTN_LEFT + BTN_WIDTH + BTN_BLANK, BTN_TOP - 30 * m_nBtnSize,
		BTN_WIDTH, BTN_HEIGHT, TRUE);

	GetWindowRect(&rect);
	SetWindowPos(NULL, 0,0, rect.right - rect.left, rect.bottom - rect.top - 30 * m_nBtnSize, SWP_NOMOVE);




	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
