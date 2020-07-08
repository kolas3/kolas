// BO_ACQ_PrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_PrintDlg.h"
#include "ExcelExporter.h"
#include "SubTotalPrintDataOptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_PrintDlg dialog


CBO_ACQ_PrintDlg::CBO_ACQ_PrintDlg(SPECIES_TYPE nSpeciesType, ACQ_TYPE nAcqType, CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_PrintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_PrintDlg)
	m_bDataSortYN = FALSE;
	//}}AFX_DATA_INIT
	m_pPrintMgr = NULL;
	m_arrData.RemoveAll();
	m_arrReportFileName.RemoveAll();
	m_arrReportType.RemoveAll();
	m_nSpeciesType = nSpeciesType;
	m_nAcqType = nAcqType;
	m_bCreate = FALSE;
	m_strReportFileName.Empty();
	m_nReportType = -1;
	m_pPrintDM = NULL;
	m_pGroupCollection = NULL;
	m_bSpecies = TRUE;
	m_bSeriesPaper = FALSE;
	m_bExcelExport = FALSE;
	m_bKOLAS2Format = FALSE;
	m_nPrintDataType = -1;

	m_strGroupingName = _T("");
	// m_pPrintDM의 분류별로 나뉘는 알리아스명
	m_strDMGroupingAlias = _T("");

	//2019.07.17 JOB.2019.0059 ADD BY KYJ : 로그 기록시 사유 추가
	PrivacyPrintLogYN = FALSE;
}

CBO_ACQ_PrintDlg::~CBO_ACQ_PrintDlg()
{
	if(m_pPrintMgr)
	{
		delete m_pPrintMgr;
		m_pPrintMgr = NULL;
	}
}

VOID CBO_ACQ_PrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_PrintDlg)
	DDX_Control(pDX, IDC_cmbCPNT_PRINT_LIST, m_ComboBox);
	DDX_Check(pDX, IDC_chkPRINT_DATA_SORT_YN, m_bDataSortYN);
	// 16/03/29 김혜영 : 한셀 적용하기
/// ADD -------------------------------------------------------------------------------------
	// 18.10.30 JMJ KOL.RED.2018.021
	/*
	CLSID clsExcelApp;
	if (!FAILED(CLSIDFromProgID(_T("HCell.Application"), &clsExcelApp))) 
	{
		DDX_Text(pDX, IDC_btnEXCEL_EXPORT, CString(_T("한셀출력")));	// 16/04/05 김혜영 : 상호대차 배포를 위해 임시주석처리 -> // 16/04/19 김혜영 : 주석풀음
	}
	*/
/// END -------------------------------------------------------------------------------------
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_PrintDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_PrintDlg)
	ON_CBN_SELCHANGE(IDC_cmbCPNT_PRINT_LIST, OnSelchangecmbPRINTLIST)
	ON_BN_CLICKED(IDC_btnSERIES_PAPER, OnbtnSERIESPAPER)
	ON_BN_CLICKED(IDC_btnEXCEL_EXPORT, OnbtnEXCELEXPORT)
	ON_BN_CLICKED(IDC_btnKOLAS2_FORMAT, OnbtnKOLAS2FORMAT)
	ON_BN_CLICKED(IDC_chkPRINT_DATA_SORT_YN, OnchkPRINTDATASORTYN)
	ON_BN_CLICKED(IDC_btnSET_SORT_CONDITION, OnbtnSETSORTCONDITION)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_PrintDlg message handlers

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ] 
// - 다이알로그창 초기화 (콤보박스 및 SIReport 파일명과 타입설정)
//
// [ 미니스펙 ]
// 1. SIReport 출력을 위하여 CBasicPrintMgr 클래스를 멤버변수로 할당한다.
// 2. 콤보박스를 초기화 한다.
// 3. 종타입을 판단하여 콤보박스 및 SIReport 파일명/타입을 설정하여
//	  Dialog 창을 초기화 한다.
//
/////////////////////////////////////////////////////////////////////////////
BOOL CBO_ACQ_PrintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//=======================================================================
	// 1. SIReport 출력을 위하여 CBasicPrintMgr 클래스를 멤버변수로 할당한다.
	//=======================================================================
	m_pPrintMgr = new CBasicPrintMgr(this);

	
	//=======================================================================
	// 2. 콤보박스를 초기화 한다.
	//=======================================================================
	m_ComboBox.ResetContent();
	
	
	//=======================================================================
	// 3. 종타입을 판단하여 콤보박스 및 SIReport 파일명/타입을 설정하여
	//	  Dialog 창을 초기화 한다.
	//=======================================================================
	if( Init() == FALSE )
	{
		ESLAfxMessageBox(_T("출력 리스트 선택 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 해당 종타입의 따라 콤보박스 및 SiReport 파일명/타입을 설정하여 
//	 Dialog 창을 초기화 한다.
//	
// [ 미니스펙 ]
// 1. ESL_MANAGE_TBL로 부터 사용 DB가 5163이나 육사용인지 체크하기위해 
//	  값을 받아온다.
// 2. 종타입에따라 내용을 추가한다.
//	  콤보박스 및 SIReport 파일명, SIReport 타입을 설정한다.
// 3. 종타입에 맞게 정의된 콤보박스 내용을 추가한다.
// 4. 콤보박스의 기본값은 첫번째 값으로 한다.
// 5. 선택된 콤보박스에 따라 SIReport 타입을 적용하고 
//	  버튼 및 체크박스의 Visible을 판단한다.
//
/////////////////////////////////////////////////////////////////////////////
BOOL CBO_ACQ_PrintDlg::Init()
{

	//=======================================================================
	// 1. ESL_MANAGE_TBL로 부터 사용 DB가 5163이나 육사용인지 체크하기위해 
	//	  값을 받아온다.
	//=======================================================================
	INT nCnt = 0;
	CString strValue, str64, strTemp;
	CStringArray astrHeadTitle, astrTailTitle;
	GetManageValue( _T("기타") , _T("공통") , _T("5163") , _T("5163") , strValue );
	GetManageValue( _T("기타") , _T("공통") , _T("육사") , _T("육사") , str64 );
	GetManageValue( _T("기타") , _T("공통") , _T("국방대"), _T("국방대") , m_strIs90 );
	

	astrHeadTitle.RemoveAll();		astrTailTitle.RemoveAll();
	//=======================================================================
	// 2. 종타입에따라 내용을 추가한다.
	//	  콤보박스 및 SIReport 파일명, SIReport 타입을 설정한다.
	//=======================================================================
	switch ( m_nSpeciesType )
	{
	// 정의되지 않은 종타입
	case SPECIES_TYPE_INVALID:	return FALSE;
		break;

	// 기초수서자료
	case BO_BASIC:
	{
		SetWindowText(_T("기초자료 출력물 리스트"));
		m_arrData.Add(_T("기초자료목록"));				m_arrReportFileName.Add(_T("K2UP_기초자료목록"));				m_arrReportType.Add(BO_ACQ_BASIC_DATA);

	}
		break;

	// 반입자료
	case BO_IMPORT:
	{
		SetWindowText(_T("반입자료 출력물 리스트"));
		m_arrData.Add(_T("반입자료목록"));				m_arrReportFileName.Add(_T("K2UP_반입자료목록"));				m_arrReportType.Add(BO_ACQ_IMPORT_DATA);
	}
		break;

	// 재활용자료
	case BO_RECYCLE:
	{
		SetWindowText(_T("재활용자료 출력물 리스트"));
		m_arrData.Add(_T("재활용자료목록"));			m_arrReportFileName.Add(_T("K2UP_재활용자료목록"));				m_arrReportType.Add(BO_ACQ_RECYCLE_DATA);
	}
		break;

	// 구입(비치)희망 자료
	case BO_REQUEST:
	{
			if( str64 == _T("Y") )	strTemp = _T("구입");
			else				strTemp = _T("비치");
		
			SetWindowText(strTemp+_T("희망자료 출력물 리스트"));
			m_arrData.Add(strTemp+_T("희망자료목록"));			m_arrReportFileName.Add(_T("K2UP_비치희망자료목록"));			m_arrReportType.Add(BO_ACQ_REQUEST_DATA);
		//JOB.2019.0073 : 충돌 License 제거
//		m_arrData.Add(_T("메일발송목록"));				m_arrReportFileName.Add(_T("K2UP_비치희망메일발송목록"));		m_arrReportType.Add(BO_ACQ_REQUEST_MAIL_SEND_LIST);
	}	
		break;

	// 구입(대상)자료
	case BO_PURCHASE:
	{
		// 2005-01-18 5163일 경우 구입대상자료목록(신청부서포함) 이라는 RPT 파일을 추가한다.
		
		SetWindowText(_T("구입대상자료 출력물 리스트"));
		m_arrData.Add(_T("구입대상자료목록"));				m_arrReportFileName.Add(_T("K2UP_구입대상자료목록"));				m_arrReportType.Add(BO_ACQ_PURCHASE_DATA);

		// 2005-01-18
		if( strValue == _T("Y") && str64 != _T("Y") )
		{
			m_arrData.Add(_T("구입대상자료목록(신청부서포함)"));	m_arrReportFileName.Add(_T("K2UP_구입대상자료목록(신청부서포함)"));	m_arrReportType.Add(BO_ACQ_PURCHASE_DATA_5163);
		}
			
		// 육사 전용으로 콤보박스에 해당 내용 추가 (64)
		// 2005-12-02 국방대에서도 사용 요청
		// 도서분류코드1별
		// 2005-09-05 강봉수
		//
		// 학처별
		// 2005-09-26 강봉수
		//
		if( str64 ==_T("Y") )
		{
			m_arrData.Add(_T("구입대상자료목록(학처별)"));			m_arrReportFileName.Add(_T("K2UP_구입대상자료목록_학처별"));	m_arrReportType.Add(BO_ACQ_PURCHASE_DATA_DEPT);
		}

		m_arrData.Add(_T("구입대상자료목록(도서분류코드별)"));	m_arrReportFileName.Add(_T("K2UP_구입대상자료목록_도서분류1별"));	m_arrReportType.Add(BO_ACQ_PURCHASE_DATA_USERDEFINECODE1);
		m_arrData.Add(_T("구입대상자료목록(등록구분별)")	);	m_arrReportFileName.Add(_T("K2UP_구입대상자료목록_등록구분별"));	m_arrReportType.Add(BO_ACQ_PURCHASE_DATA_REGCODE);
		m_arrData.Add(_T("심의대상자료목록(차수별)")		);	m_arrReportFileName.Add(_T("K2UP_심의대상자료목록"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO);
		m_arrData.Add(_T("심의대상자료목록(구입처별)")		);	m_arrReportFileName.Add(_T("K2UP_심의대상자료목록"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER);
		m_arrData.Add(_T("심의대상자료목록(주제구분별)")	);	m_arrReportFileName.Add(_T("K2UP_심의대상자료목록"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE);
		m_arrData.Add(_T("심의결과자료목록(차수별)")		);	m_arrReportFileName.Add(_T("K2UP_심의결과자료목록"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO);
		m_arrData.Add(_T("심의결과자료목록(구입처별)")		);	m_arrReportFileName.Add(_T("K2UP_심의결과자료목록"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER);
		m_arrData.Add(_T("심의결과자료목록(주제구분별)")	);	m_arrReportFileName.Add(_T("K2UP_심의결과자료목록"));				m_arrReportType.Add(BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE);
		m_arrData.Add(_T("주문자료목록(차수별)")			);	m_arrReportFileName.Add(_T("K2UP_주문자료목록_차수별"));			m_arrReportType.Add(BO_ACQ_ORDER_DATA_DIVNO);
		m_arrData.Add(_T("주문자료목록(구입처별)")			);	m_arrReportFileName.Add(_T("K2UP_주문자료목록_구입처별"));			m_arrReportType.Add(BO_ACQ_ORDER_DATA_PROVIDER);
		m_arrData.Add(_T("주문자료목록(주제구분별)")		);	m_arrReportFileName.Add(_T("K2UP_주문자료목록_주제구분별"));		m_arrReportType.Add(BO_ACQ_ORDER_DATA_SUBJECTCODE);
		// 2005-01-26
		if( strValue == _T("Y") && str64 != _T("Y") )
		{
			CString strName;
			strName = _T("주문자료목록(도서분류코드별)") ;
			m_arrData.Add(strName);	m_arrReportFileName.Add(_T("K2UP_주문자료목록_도서분류코드1별"));	m_arrReportType.Add(BO_ACQ_ORDER_DATA_MEDIACODE);
		}
	}
		break;

	// 검수대상자료
	case BO_ORDER:
	{
		SetWindowText(_T("검수대상자료 출력물 리스트"));
		m_arrData.Add(_T("검수대상자료목록"));			m_arrReportFileName.Add(_T("K2UP_검수대상자료목록"));			m_arrReportType.Add(BO_ACQ_CHECKINING_DATA);
		m_arrData.Add(_T("주문자료목록(차수별)"));		m_arrReportFileName.Add(_T("K2UP_주문자료목록_차수별"));		m_arrReportType.Add(BO_ACQ_ORDER_DATA_DIVNO);
		m_arrData.Add(_T("주문자료목록(구입처별)"));	m_arrReportFileName.Add(_T("K2UP_주문자료목록_구입처별"));		m_arrReportType.Add(BO_ACQ_ORDER_DATA_PROVIDER);
		m_arrData.Add(_T("주문자료목록(주제구분별)"));	m_arrReportFileName.Add(_T("K2UP_주문자료목록_주제구분별"));	m_arrReportType.Add(BO_ACQ_ORDER_DATA_SUBJECTCODE);
		m_arrData.Add(_T("검수자료목록"));				m_arrReportFileName.Add(_T("K2UP_검수자료목록"));				m_arrReportType.Add(BO_ACQ_CHECKIN_DATA);
		m_arrData.Add(_T("미검수자료목록"));			m_arrReportFileName.Add(_T("K2UP_미검수자료목록"));				m_arrReportType.Add(BO_ACQ_NONCHECKIN_DATA);
		m_arrData.Add(_T("구입신청자료도착목록"));		m_arrReportFileName.Add(_T("K2UP_구입신청자료도착목록"));		m_arrReportType.Add(BO_ACQ_CHECKIN_REQUEST_DATA);
		m_arrData.Add(_T("검수자료통계"));				m_arrReportFileName.Add(_T("K2UP_검수자료통계"));				m_arrReportType.Add(BO_ACQ_CHECKIN_STATISTICS);
		m_arrData.Add(_T("미검수자료통계"));			m_arrReportFileName.Add(_T("K2UP_미검수자료통계"));				m_arrReportType.Add(BO_ACQ_NONCHECKIN_STATISTICS);
		if(strValue == _T("Y"))
		{
			m_arrData.Add(_T("발주자료목록"));				m_arrReportFileName.Add(_T("K2UP_발주자료목록"));				m_arrReportType.Add(BO_ACQ_ORDER_DATA_ORDER);
		}
	}
		break;

	// 미검수자료, 미납자료
	case BO_MISSING:
	{
		SetWindowText(_T("미납자료 출력물 리스트"));
		m_arrData.Add(_T("미납자료목록"));				m_arrReportFileName.Add(_T("K2UP_미납자료목록"));				m_arrReportType.Add(BO_ACQ_MISSING_DATA);
		m_arrData.Add(_T("미검수자료통계"));			m_arrReportFileName.Add(_T("K2UP_미검수자료통계"));				m_arrReportType.Add(BO_ACQ_NONCHECKIN_STATISTICS);
	}
		break;

	// 검수완료자료, 등록인계자료
	case BO_REGOBJ:
		{
			if( m_nAcqType == PURCHASE )
	{
		SetWindowText(_T("구입등록인계대상자료 출력물 리스트"));
		m_arrData.Add(_T("구입등록인계대상자료목록"));	m_arrReportFileName.Add(_T("K2UP_구입등록인계대상자료목록"));	m_arrReportType.Add(BO_ACQ_PURCHASE_REGOBJ_DATA);
		m_arrData.Add(_T("검수완료자료목록"));			m_arrReportFileName.Add(_T("K2UP_검수완료자료목록"));			m_arrReportType.Add(BO_ACQ_CHECKIN_COMPLETE_DATA);
		m_arrData.Add(_T("검수자료통계"));				m_arrReportFileName.Add(_T("K2UP_검수자료통계"));				m_arrReportType.Add(BO_ACQ_CHECKIN_STATISTICS);
	}
			else if( m_nAcqType == DONATE )
			{
				SetWindowText(_T("기증등록인계대상자료 출력물 리스트"));
				m_arrData.Add(_T("기증등록인계대상자료목록"));	m_arrReportFileName.Add(_T("K2UP_기증등록인계대상자료목록"));	m_arrReportType.Add(BO_ACQ_DONATE_REGOBJ_DATA);
			}
		}
		break;

	// 기증접수자료
	case BO_DONATE:
	{
		SetWindowText(_T("기증접수자료 출력물 리스트"));
		m_arrData.Add(_T("기증접수자료목록"));					m_arrReportFileName.Add(_T("K2UP_기증접수자료목록"));			m_arrReportType.Add(BO_ACQ_DONATE_DATA);
		m_arrData.Add(_T("심의대상자료목록(접수별)")		);	m_arrReportFileName.Add(_T("K2UP_심의대상자료목록_기증"));		m_arrReportType.Add(BO_ACQ_DONATE_CHECKING_DATA_DIVNO);
		m_arrData.Add(_T("심의대상자료목록(기증자별)")		);	m_arrReportFileName.Add(_T("K2UP_심의대상자료목록_기증"));		m_arrReportType.Add(BO_ACQ_DONATE_CHECKING_DATA_PROVIDER);
		m_arrData.Add(_T("심의대상자료목록(주제구분별)")	);	m_arrReportFileName.Add(_T("K2UP_심의대상자료목록_기증"));		m_arrReportType.Add(BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE);
		m_arrData.Add(_T("심의결과자료목록(접수별)")		);	m_arrReportFileName.Add(_T("K2UP_심의결과자료목록_기증"));		m_arrReportType.Add(BO_ACQ_DONATE_CHECKED_DATA_DIVNO);
		m_arrData.Add(_T("심의결과자료목록(기증자별)")		);	m_arrReportFileName.Add(_T("K2UP_심의결과자료목록_기증"));		m_arrReportType.Add(BO_ACQ_DONATE_CHECKED_DATA_PROVIDER);
		m_arrData.Add(_T("심의결과자료목록(주제구분별)")	);	m_arrReportFileName.Add(_T("K2UP_심의결과자료목록_기증"));		m_arrReportType.Add(BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE);
		m_arrData.Add(_T("기증자라벨출력"));					m_arrReportFileName.Add(_T("K2UP_기증자라벨출력"));				m_arrReportType.Add(DONATOR_LABEL);
	}
		break;

	// 기증 미등록자료
	case BO_NONDONATE:
	{
		SetWindowText(_T("기증미등록자료 출력물 리스트"));
		m_arrData.Add(_T("기증미등록자료목록"));		m_arrReportFileName.Add(_T("K2UP_기증미등록자료목록"));			m_arrReportType.Add(BO_ACQ_NONDONATE_DATA);
	}
		break;

	//구입자료
	case BO_PURCHASE_DATA_PRINT:
	{
		SetWindowText(_T("구입자료 출력물 리스트"));
		m_arrData.Add(_T("구입자료목록"));	m_arrReportFileName.Add(_T("K2UP_구입자료목록"));	m_arrReportType.Add(BO_PURCHASE_DATA);
	}
		break;

	// 기증자료
	case BO_DONATE_DATA_PRINT:
	{
		SetWindowText(_T("기증자료 출력물 리스트"));
		m_arrData.Add(_T("기증자료목록"));	m_arrReportFileName.Add(_T("K2UP_기증자료목록"));	m_arrReportType.Add(BO_DONATE_DATA);
		m_arrData.Add(_T("기증자목록"));	m_arrReportFileName.Add(_T("K2UP_기증자목록"));		m_arrReportType.Add(BO_ACQ_DEFAULT);      
	}
		break;

	default:	return FALSE;
		break;
	}


	//=======================================================================
	// 3. 종타입에 맞게 정의된 콤보박스 내용을 추가한다.
	//=======================================================================
	nCnt = m_arrData.GetSize();
	for( INT i = 0 ; i < nCnt ; i++ ) 
		m_ComboBox.InsertString(i, (m_arrData.GetAt(i)).GetBuffer(0));
	

	//=======================================================================
	// 4. 콤보박스의 기본값은 첫번째 값으로 한다.
	//=======================================================================
	m_ComboBox.SetCurSel(0);


	//=======================================================================
	// 5. 선택된 콤보박스에 따라 SIReport 타입을 적용하고 
	//	  버튼 및 체크박스의 Visible을 판단한다.
	//=======================================================================
	OnSelchangecmbPRINTLIST();

	return TRUE;
}

BOOL CBO_ACQ_PrintDlg::Create(CWnd * pParentWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pParentWnd);
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 옵션창을 열어 설정값에 대한 멤버변수를 설정하고, 출력할 방식을 결정한 후에
//	 자신을 호출한 클래스에게 DM을 구축하라는 메시지를 보내준다.(리턴)
//
// [ 미니스펙 ]
// 1. SIReport 타입이 설정되지 않았다면 리턴한다.
// 2. _T("KOLAS2형식") 버튼을 눌렀다면 nExtraInfo=1000를 설정한다.
// 3. SIReport 타입이 아래에 해당할때에는 출력 옵션창에서 _T("출력데이터 재검색")을 보이지 않게 한다.
//		- i.	주문자료목록일괄(종/책)
//		- ii.	주문자료구입처별(종/책)
//		- iii.	주문자료목록주제별(종/책)
//		- iv.	심의대상자료목록_차수별
//		- v.	심의대상자료목록_구입처별
//		- vi.	심의대상자료목록_주제구분별
//		- vii.	심의결과자료목록_차수별
//		- viii.	심의결과자료목록_구입처별
//		- ix.	심의결과자료목록_주제구분별
//		- x.	주문자료목록매체구분별_5163(종/책)
// 4. SIReport 타입이 아래에 해당되지 않을때에는 출력 옵션창에서 선택된 옵션값을 받아와 멤버변수로 저장한다.
//		- i.	메일발송현황리스트
//		- ii.	디폴트
//		- iii.	검수자료통계
//		- iv.	미검수자료통계
// 5. DM을 구축하기 위한 메시지를 자신을 호출한 클래스에게 보낸다.
//
/////////////////////////////////////////////////////////////////////////////
VOID CBO_ACQ_PrintDlg::OnOK() 
{
	INT nExtraInfo;
	CSubTotalPrintDataOptionDlg dlg(this);


	//=======================================================================
	// 1. SIReport 타입이 설정되지 않았다면 리턴한다.
	//=======================================================================
	if(m_nReportType < 0) return ;
	

	//=======================================================================
	// 2. _T("KOLAS2형식") 버튼을 눌렀다면 nExtraInfo=1000를 설정한다.
	//=======================================================================
	if(m_bKOLAS2Format) nExtraInfo = 1000;
	else nExtraInfo = 0;


	//=======================================================================
	// 3. SIReport 타입이 아래에 해당할때에는 출력 옵션창에서 _T("출력데이터 재검색")을 보이지 않게 한다.
	//		- i.	주문자료목록일괄(종/책)
	//		- ii.	주문자료구입처별(종/책)
	//		- iii.	주문자료목록주제별(종/책)
	//		- iv.	심의대상자료목록_차수별
	//		- v.	심의대상자료목록_구입처별
	//		- vi.	심의대상자료목록_주제구분별
	//		- vii.	심의결과자료목록_차수별
	//		- viii.	심의결과자료목록_구입처별
	//		- ix.	심의결과자료목록_주제구분별
	//		- x.	주문자료목록매체구분별_5163(종/책)
	//=======================================================================
	if( m_nReportType == BO_ACQ_ORDER_DATA_DIVNO	||
		m_nReportType == BO_ACQ_ORDER_DATA_PROVIDER ||
		m_nReportType == BO_ACQ_ORDER_DATA_SUBJECTCODE || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO ||
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE	||
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER ||
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE ||  
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_DIVNO ||  
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_PROVIDER ||  
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE ||  
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_DIVNO ||  
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_PROVIDER ||  
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE ||  
		m_nReportType == BO_ACQ_ORDER_DATA_MEDIACODE || //2005-01-29 5163용 추가.
		m_nReportType == BO_ACQ_ORDER_DATA_ORDER )  //2006-01-23 5163용 추가.
	{
		dlg.SetSubTotalPrintYN(TRUE);
	}
	else
	{
		dlg.SetSubTotalPrintYN(FALSE);
	}


	//=======================================================================
	// 4. SIReport 타입이 아래에 해당되지 않을때에는 출력 옵션창에서 선택된 옵션값을 받아와 멤버변수로 저장한다.
	//		- i.	메일발송현황리스트
	//		- ii.	디폴트
	//		- iii.	검수자료통계
	//		- iv.	미검수자료통계
	//=======================================================================
	if( m_nReportType != BO_ACQ_REQUEST_MAIL_SEND_LIST && 
		m_nReportType != BO_ACQ_DEFAULT &&
		m_nReportType != BO_ACQ_CHECKIN_STATISTICS && 
		m_nReportType != BO_ACQ_NONCHECKIN_STATISTICS )
	{
		if( dlg.DoModal() == IDOK )		m_nPrintDataType = dlg.GetPrintDataType();
		else
		{
			if(	m_bExcelExport )
				m_bExcelExport = FALSE;
			return ;
		}
	}

	//2019.07.17 JOB.2019.0059 ADD BY KYJ : 로그 기록시 사유 추가
	CString strLogName;
	GetDlgItem(IDC_cmbCPNT_PRINT_LIST)->GetWindowText(strLogName);
	
	if(PrivacyPrintLogYN) {		
		m_pPrintMgr->SetPrivacyPrintLogParm( TRUE, m_pInfo->USER_PK, m_pInfo->USER_ID, strLogName, m_pInfo->MANAGE_CODE, strLogName,0);
	}
	//

	//=======================================================================
	// 5. DM을 구축하기 위한 메시지를 자신을 호출한 클래스에게 보낸다.
	//=======================================================================
	::PostMessage(pParentMgr->GetSafeHwnd(), MAKE_PRINT_DM, m_nReportType, nExtraInfo);
}

BOOL CBO_ACQ_PrintDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

INT CBO_ACQ_PrintDlg::Print()
{
	INT ids;

	if( m_bExcelExport == TRUE )	ids = PrintExcel();
	else							ids = PrintPaper();
	
	m_bExcelExport = m_bKOLAS2Format = m_bSeriesPaper = FALSE;
	
	return ids;
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 선택된 콤보박스에 따라 SIReport 타입을 적용하고 버튼 및 체크박스의 Visible을 판단한다.
//
// [ 미니스펙 ]
// 1. 현재 선택된 콤보박스의 인덱스 번호를 가져온다.
// 2. 현재 선택된 콤보박스의 내용을 가져온다.
// 3. 콤보인덱스 번호로 해당 콤보에 해당하는 SIReport 파일명을 멤버변수로 설정한다.
// 4. 해당 콤보에 해당하는 SIReport 타입을 멤버변수로 설정한다.
// 5. 엑셀출력버튼을 화면에 보이게 한다.
// 5-1. 국방대 출력물 형식에 의하여 분류명과 분류할 DM알리아스를 설정한다.
// 6. SIReport 타입이 아래에 해당할때에는 _T("연속용지") 버튼을 보이게 한다.
//		- i.	심의대상자료목록_차수별
//		- ii.	심의대상자료목록_구입처별
//		- iii.	심의대상자료목록_주제구분별
//		- iv.	심의결과자료목록_차수별
//		- v.	심의결과자료목록_구입처별
//		- vi.	심의결과자료목록_주제구분별
// 7. SIReport 타입이 아래에 해당할때에는 _T("KOLAS2형식") 버튼을 보이게 한다.
//		- i.	심의대상자료목록_차수별
//		- ii.	심의대상자료목록_구입처별
//		- iii.	심의대상자료목록_주제구분별
//		- iv.	심의결과자료목록_차수별
//		- v.	심의결과자료목록_구입처별
//		- vi.	심의결과자료목록_주제구분별
//		- vii.	주문자료목록일괄(종/책)
//		- viii.	주문자료목록구입처별(종/책)
//		- ix.	주문자료목록주제별(종/책)
//		- x.	주문자료목록매체구분별_5163(종/책)
// 8. SIReport 타입이 아래에 해당할때에는 출력시 정렬을 하지 못하도록
//	  정렬조건설정버튼 및 자료정렬 체크박스를 Disable 시킨다.
//		- i.	메일발송현황리스트
//		- ii.	미검수자료통계
//		- iii.	검수자료통계
//		- iv.	디폴트 (??)
// 9. SIReport 타입이 아래에 해당할때에는 _T("엑셀출력") 버튼을 안보이게 한다.	
//		- i.	검수자료통계
//		- ii.	미검수자료통계	
//
/////////////////////////////////////////////////////////////////////////////
VOID CBO_ACQ_PrintDlg::OnSelchangecmbPRINTLIST() 
{
	INT nCurSel ;
	CString str;
	
	//=======================================================================
	// 1. 현재 선택된 콤보박스의 인덱스 번호를 가져온다.
	//=======================================================================
	nCurSel = m_ComboBox.GetCurSel();
	
	//=======================================================================
	// 2. 현재 선택된 콤보박스의 내용을 가져온다.
	//=======================================================================
	m_ComboBox.GetWindowText(str);

	
	//=======================================================================
	// 3. 콤보인덱스 번호로 해당 콤보에 해당하는 SIReport 파일명을 멤버변수로 설정한다.
	//=======================================================================
	m_strReportFileName = m_arrReportFileName.GetAt(nCurSel);


	//=======================================================================
	// 4. 해당 콤보에 해당하는 SIReport 타입을 멤버변수로 설정한다.
	//=======================================================================
	m_nReportType = m_arrReportType.GetAt(nCurSel);

	
	//=======================================================================
	// 5. 엑셀출력버튼을 화면에 보이게 한다.
	//=======================================================================
	// 18.10.30 JMJ KOL.RED.2018.021
	// 출력기능 통합
	//GetDlgItem(IDC_btnEXCEL_EXPORT)->ShowWindow(SW_SHOW);


	//=======================================================================
	// 5-1. 국방대 출력물 형식에 의하여 분류명과 분류할 DM알리아스를 설정한다.
	// 2006.03.15 KBS - 국방대 출력 형식물 수정
	//=======================================================================
	switch( m_nReportType )
	{
	case BO_ACQ_PURCHASE_DATA_USERDEFINECODE1:
		{
			m_strGroupingName	 = _T("도서분류");
			m_strDMGroupingAlias = _T("UDF_사용자정의코드1");
		}
		break;
	
	case BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO:
	case BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO:
	case BO_ACQ_ORDER_DATA_DIVNO:
	case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
	case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
		{
			m_strGroupingName	 = _T("차수번호");
			m_strDMGroupingAlias = _T("BP_그룹번호");
		}
		break;

	case BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER:
	case BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER:
	case BO_ACQ_ORDER_DATA_PROVIDER:
		{
			m_strGroupingName	 = _T("구입처");
			m_strDMGroupingAlias = _T("UDF_구입처부호설명");
		}
		break;
	case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
	case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
		{
			m_strGroupingName	 = _T("기증자");
			m_strDMGroupingAlias = _T("BP_기증자명");
		}
		break;
	case BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE:
	case BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE:
	case BO_ACQ_ORDER_DATA_SUBJECTCODE:
	case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:
	case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:
		{
			m_strGroupingName	 = _T("주제구분");
			m_strDMGroupingAlias = _T("UDF_주제구분설명");
		}
		break;
	default:
		break;
	}

	//=======================================================================
	// 6. SIReport 타입이 아래에 해당할때에는 _T("연속용지") 버튼을 보이게 한다.
	//		- i.	심의대상자료목록_차수별
	//		- ii.	심의대상자료목록_구입처별
	//		- iii.	심의대상자료목록_주제구분별
	//		- iv.	심의결과자료목록_차수별
	//		- v.	심의결과자료목록_구입처별
	//		- vi.	심의결과자료목록_주제구분별
	//=======================================================================
	if( m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER ||
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE ||
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_DIVNO || 
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_PROVIDER ||
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE || 
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_DIVNO || 
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_PROVIDER || 
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE		)
	{
		GetDlgItem(IDC_btnSERIES_PAPER)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_btnSERIES_PAPER)->ShowWindow(SW_HIDE);
	}


	//=======================================================================
	// 7. SIReport 타입이 아래에 해당할때에는 _T("KOLAS2형식") 버튼을 보이게 한다.
	//		- i.	심의대상자료목록_차수별
	//		- ii.	심의대상자료목록_구입처별
	//		- iii.	심의대상자료목록_주제구분별
	//		- iv.	심의결과자료목록_차수별
	//		- v.	심의결과자료목록_구입처별
	//		- vi.	심의결과자료목록_주제구분별
	//		- vii.	주문자료목록일괄(종/책)
	//		- viii.	주문자료목록구입처별(종/책)
	//		- ix.	주문자료목록주제별(종/책)
	//		- x.	주문자료목록매체구분별_5163(종/책)
	//=======================================================================
	if( m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER ||
		m_nReportType == BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER || 
		m_nReportType == BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE ||
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_DIVNO || 
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_PROVIDER ||
		m_nReportType == BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE || 
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_DIVNO || 
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_PROVIDER || 
		m_nReportType == BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE ||
		m_nReportType == BO_ACQ_ORDER_DATA_DIVNO || 
		m_nReportType == BO_ACQ_ORDER_DATA_SUBJECTCODE ||
		m_nReportType == BO_ACQ_ORDER_DATA_PROVIDER || 
		m_nReportType == BO_ACQ_ORDER_DATA_MEDIACODE || //2005-01-29 5163용 추가.
		m_nReportType == BO_ACQ_ORDER_DATA_ORDER )  //2006-01-23 5163용 추가.
	{
		GetDlgItem(IDC_btnKOLAS2_FORMAT)->ShowWindow(SW_SHOW);
	}
	else
		GetDlgItem(IDC_btnKOLAS2_FORMAT)->ShowWindow(SW_HIDE);



	//=======================================================================
	// 8. SIReport 타입이 아래에 해당할때에는 출력시 정렬을 하지 못하도록
	//	  정렬조건설정버튼 및 자료정렬 체크박스를 Disable 시킨다.
	//		- i.	메일발송현황리스트
	//		- ii.	미검수자료통계
	//		- iii.	검수자료통계
	//		- iv.	디폴트 (??)
	//=======================================================================
	if( m_nReportType == BO_ACQ_REQUEST_MAIL_SEND_LIST || 
		m_nReportType == BO_ACQ_NONCHECKIN_STATISTICS ||
		m_nReportType == BO_ACQ_CHECKIN_STATISTICS || 		
		m_nReportType == BO_ACQ_DEFAULT ||
		m_nReportType == DONATOR_LABEL)
	{
		GetDlgItem(IDC_chkPRINT_DATA_SORT_YN)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnSET_SORT_CONDITION)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_chkPRINT_DATA_SORT_YN)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnSET_SORT_CONDITION)->EnableWindow(TRUE);
	}


	//=======================================================================
	// 9. SIReport 타입이 아래에 해당할때에는 _T("엑셀출력") 버튼을 안보이게 한다.	
	//		- i.	검수자료통계
	//		- ii.	미검수자료통계	
	//=======================================================================
	if( m_nReportType == BO_ACQ_CHECKIN_STATISTICS || 
		m_nReportType == BO_ACQ_NONCHECKIN_STATISTICS ||
		m_nReportType == DONATOR_LABEL)
	{
		GetDlgItem(IDC_btnEXCEL_EXPORT)->ShowWindow(SW_HIDE);
	}

	CString strTmp;
	m_ComboBox.GetWindowText(strTmp);
	//20070521 ADD PJW : 주제구분일경우 무조건정렬
	if ( strTmp.Find(_T("주제구분")) >= 0 )
	{
		( (CButton*) GetDlgItem(IDC_chkPRINT_DATA_SORT_YN) )->SetCheck(1);
		m_bDataSortYN = TRUE;
	}
}

VOID CBO_ACQ_PrintDlg::OnbtnSERIESPAPER() 
{
	// TODO: Add your control notification handler code here
	m_bSeriesPaper = TRUE;
	PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 엑셀출력버튼 이벤트
// 
// [ 미니스펙 ]
// 1. 멤버변수에 엑셀출력이라고 설정한다.
// 2. OnOK()함수를 호출한다. (출력버튼호출)
// 
/////////////////////////////////////////////////////////////////////////////
VOID CBO_ACQ_PrintDlg::OnbtnEXCELEXPORT() 
{
	//=======================================================================
	// 1. 멤버변수에 엑셀출력이라고 설정한다.
	//=======================================================================
	m_bExcelExport = TRUE;

	//=======================================================================
	// 2. OnOK()함수를 호출한다. (출력버튼호출)
	//=======================================================================
	PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - CBasicPrintMgr클래스에 출력할 정보를 설정하고 출력한다.
// [ 미니스펙 ]
// 1. 출력용 DM이 구축되지 않거나 데이터가 없으면 에러리턴한다.
// 2. 매핑된 멤버변수를 초기화 한다.
// 3. 그룹정보클래스에 수입년도와 그룹정보의 내용을 입수타입에 따라 멤버변수에 매핑한다.
// 4. 출력용 DM을 설정한다.
// 5. 연속용지 출력이라면 출력타입과 SIReport파일이름을 설정한다.
// 6. KOLAS2형식 출력이라면 출력타입과 SIReport파일이름을 설정한다.
// 7. 그외 출력이라면 심의대상자료인지 체크하여 출력타입과 SIReport파일이름을 설정한다.
// 8. 설정된 정보를 출력한다.
//
/////////////////////////////////////////////////////////////////////////////
INT CBO_ACQ_PrintDlg::PrintPaper()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT i, j, ids;
	CString strAcqYear, strGroupInfo, strTmpData, strPurchaseGroupInfo;


	//=======================================================================
	// 1. 출력용 DM이 구축되지 않거나 데이터가 없으면 에러리턴한다.
	//=======================================================================
	if( m_pPrintDM == NULL )	return -1;
	if( m_pPrintDM->GetRowCount() == 0 )
	{
		ESLAfxMessageBox(_T("출력 데이터가 없습니다."));
		return -1;
	}
	
	//=======================================================================
	// 2. 매핑된 멤버변수를 초기화 한다.
	//=======================================================================
	m_pPrintMgr->InitPrintData();

	//=======================================================================
	// 3. 그룹정보클래스에 수입년도와 그룹정보의 내용을 입수타입에 따라 멤버변수에 매핑한다.
	//=======================================================================
	if( m_pGroupCollection != NULL && m_pGroupCollection->GetGroupCount() > 0 )
	{
		strAcqYear = m_pGroupCollection->GetAcqYear();
		m_pGroupCollection->GetGroupInfo(strGroupInfo);
		if( strAcqYear.IsEmpty()	== TRUE	)	strAcqYear	 = _T("적용안함");
		if( strGroupInfo.IsEmpty()  == TRUE	)	strGroupInfo = _T("적용안함");
		
		// Key:수입년도, Value:strAcqYear
		m_pPrintMgr->AddPrintData(_T("수입년도"), strAcqYear);

		switch( m_nAcqType )
		{
		// Key:그룹이름, Value:strGroupInfo
		case ACQ_TYPE_INVALID:	m_pPrintMgr->AddPrintData(_T("그룹이름"), strGroupInfo);
			break;

		// Key:접수번호, Value:strGroupInfo
		case DONATE:			m_pPrintMgr->AddPrintData(_T("접수번호"), strGroupInfo);
			break;
		
		// Key:차수번호, Value:업무구분 : m_pGroupCollection->GetWorkCode()		차수번호 : strGroupInfo
		case PURCHASE:
		{
			strTmpData = m_pGroupCollection->GetWorkCode();
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("업무구분"), strTmpData, strTmpData);
				if( strTmpData.IsEmpty() == TRUE )		strTmpData = _T("적용안함");
			strPurchaseGroupInfo.Format(_T("업무구분 : %s     차수번호 : %s"), strTmpData, strGroupInfo);
			m_pPrintMgr->AddPrintData(_T("차수번호"), strPurchaseGroupInfo);
		}
			break;
		}
	}


	//=======================================================================
	// 4. 출력용 DM을 설정한다.
	//=======================================================================
	m_pPrintMgr->SetPrintDM(m_pPrintDM);

	//=======================================================================
	// 5. 연속용지 출력이라면 출력타입과 SIReport파일이름을 설정한다.
	//=======================================================================
	if( m_bSeriesPaper == TRUE )
	{
		switch( m_nReportType )
	{
		case BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE:		strTmpData = _T("K2UP_심의대상자료목록_연속용지");
			break;

		case BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE:		strTmpData = _T("K2UP_심의결과자료목록_연속용지");
			break;

		case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:		strTmpData = _T("K2UP_심의대상자료목록_기증_연속용지");
			break;

		case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:		strTmpData = _T("K2UP_심의결과자료목록_기증_연속용지");
			break;
		
		default:
		{
				ESLAfxMessageBox(_T("연속용지 출력을 할 수 없습니다."));
				return -1;
		}
			break;
		}

		m_pPrintMgr->SetPrintType(SERIES_PAPER);
		m_pPrintMgr->SetSIReportFilename(strTmpData);
	}
	//=======================================================================
	// 6. KOLAS2형식 출력이라면 출력타입과 SIReport파일이름을 설정한다.
	//=======================================================================
	else if( m_bKOLAS2Format == TRUE )
	{
		switch( m_nReportType )
		{
		case BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE:		strTmpData = _T("K2_심의대상자료목록");
			break;

		case BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE:		strTmpData = _T("K2_심의결과자료목록");
			break;

		case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:		strTmpData = _T("K2_심의대상자료목록_기증");
			break;

		case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:		strTmpData = _T("K2_심의결과자료목록_기증");
			break;
			
		case BO_ACQ_ORDER_DATA_MEDIACODE:	// 2005-01-29 5163용 추가.
		case BO_ACQ_ORDER_DATA_DIVNO:
		case BO_ACQ_ORDER_DATA_PROVIDER:
		case BO_ACQ_ORDER_DATA_SUBJECTCODE:					strTmpData = _T("K2_주문자료목록");	
			break;

		default:
		{
				ESLAfxMessageBox(_T("KOLAS2형식으로 출력을 할 수 없습니다."));
				return -1;
		}
			break;
		}
		
		m_pPrintMgr->SetPrintType(m_nReportType);
		m_pPrintMgr->SetSIReportFilename(strTmpData);
	}
	//=======================================================================
	// 7. 그외 출력이라면 심의대상자료인지 체크하여 출력타입과 SIReport파일이름을 설정한다.
	//=======================================================================
	else
	{
		CString strReportFileName;
		
		if( m_bSpecies == FALSE && m_nReportType == BO_ACQ_CHECKINING_DATA ) 
		{
			strReportFileName = m_strReportFileName + _T("_권");
		}
		else
		{
			strReportFileName = m_strReportFileName;
		}

		m_pPrintMgr->SetPrintType(m_nReportType);
		m_pPrintMgr->SetSIReportFilename(strReportFileName);
	}


	//=======================================================================
	// 8. 설정된 정보를 출력한다.
	// - 2006.03.14 KBS - 국방대의 경우 분류별을 페이지단위로 출력한다.
	//=======================================================================	
	if( _T("Y") == m_strIs90 )
	{
		switch( m_nReportType )
		{
		case BO_ACQ_PURCHASE_DATA_USERDEFINECODE1:
		case BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE:
		case BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE:
		case BO_ACQ_ORDER_DATA_DIVNO:
		case BO_ACQ_ORDER_DATA_PROVIDER:
		case BO_ACQ_ORDER_DATA_SUBJECTCODE:
			{
				INT nStart = 0;
				CString strData, strGrouping;
				POSITION pos;
				CESL_DataMgr TempDM ;
				CESL_DataMgr::reference* pRef = NULL;
				
				m_pPrintMgr->DeleteTempPages();
				TempDM.SetCONNECTION_INFO(m_pPrintDM->CONNECTION_INFO);
				TempDM.InitDMPKFromFile(m_pPrintDM->DB_MGR_ALIAS);
				TempDM.FreeData();
				strGrouping.Empty();
				for( i=0; i<m_pPrintDM->GetRowCount(); i++ )
				{
					if( m_pPrintDM->GetCellData(_T("UDF_일련번호"),i) == _T("소계") )
					{
						if( i < m_pPrintDM->GetRowCount()-1 )
						{
							if( m_pPrintDM->GetCellData(i+1,0) == _T("총계") )	i++;
						}
						
						strPurchaseGroupInfo =_T("차수정보 : "); 
						strPurchaseGroupInfo += strAcqYear +_T(" - ")+ strTmpData +_T(" - ")+ m_pPrintDM->GetCellData(_T("BP_그룹번호"), nStart);
						m_pPrintMgr->AddPrintData( _T("차수번호"), strPurchaseGroupInfo );

						strGrouping = _T("< ") + m_pPrintDM->GetCellData(m_strDMGroupingAlias,nStart)+_T(" >");
						m_pPrintMgr->AddPrintData( _T("분류"), strGrouping);
						/*
						if( m_strGroupingName.IsEmpty() == FALSE )
						{
							strGrouping = _T("< ") + m_strGroupingName + _T(" : ") + m_pPrintDM->GetCellData(m_strDMGroupingAlias,nStart)+_T(" >");
							m_pPrintMgr->AddPrintData( _T("분류"), strGrouping);
						}
						*/

						for( j=nStart; j<=i; j++ )
						{				
							TempDM.InsertRow(-1);
							pos = m_pPrintDM->RefList.GetHeadPosition();
							while( pos )
							{
								pRef = (CESL_DataMgr::reference*)m_pPrintDM->RefList.GetNext(pos);
								if( pRef == NULL ) continue;
								ids = m_pPrintDM->GetCellData(pRef->FIELD_ALIAS, j, strData);
								if( ids < 0 )	continue;
								ids = TempDM.SetCellData(pRef->FIELD_ALIAS, strData, TempDM.GetRowCount()-1);
								if( ids < 0 )	continue;
							}
						}
						m_pPrintMgr->SetPrintDM(&TempDM);
						m_pPrintMgr->MakeXMLPageFile();
						TempDM.FreeData();
						nStart = i+1;
					}		
				}
				ids = m_pPrintMgr->Print2();
			}
			break;

		default:	
			{
				strPurchaseGroupInfo =_T("차수정보 : "); 
				strPurchaseGroupInfo += strAcqYear +_T(" - ")+ strTmpData +_T(" - ")+ strGroupInfo;
				m_pPrintMgr->AddPrintData( _T("차수번호"), strPurchaseGroupInfo );
	ids = m_pPrintMgr->Print();
			}
			break;
		}
	}
	else	ids = m_pPrintMgr->Print();

	m_bSeriesPaper = FALSE;
	return ids;
}

INT CBO_ACQ_PrintDlg::PrintExcel()
{
	if(m_pPrintDM == NULL) return -1;
	if(m_pPrintDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("출력 데이터가 없습니다."));
		return -1;
	}
	CString strTmpData = m_strReportFileName;

	
	//JOB.2019.0017 ADD BY KYJ
	//개인정보 엑셀 반출,로그 적용
	//--------------------------------------------------------------------------------------------------------------------------
	//2019.07.17 JOB.2019.0059 ADD BY KYJ : 로그 기록시 사유 추가
	if( PrivacyPrintLogRecord(m_pPrintDM, _T("엑셀출력"), _T("E") ,0 ) < 0) return -1;
	//--------------------------------------------------------------------------------------------------------------------------
	

	if( m_bSeriesPaper == TRUE )
	{
		switch( m_nReportType )
		{
		case BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE:
			{
				strTmpData = _T("K2UP_심의대상자료목록_연속용지");			
			}
			break;

		case BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE:
			{
				strTmpData = _T("K2UP_심의결과자료목록_연속용지");	
			}
			break;

		case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:
			{
				strTmpData = _T("K2UP_심의대상자료목록_기증_연속용지");			
			}
			break;

		case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:
			{
				strTmpData = _T("K2UP_심의결과자료목록_기증_연속용지");	
			}
			break;
		}
	}
	else if( m_bKOLAS2Format == TRUE )
	{
		switch( m_nReportType )
		{
		case BO_ACQ_PURCHASE_CHECKING_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKING_DATA_SUBJECTCODE:
			{
				strTmpData = _T("K2_심의대상자료목록");			
			}
			break;

		case BO_ACQ_PURCHASE_CHECKED_DATA_DIVNO:
		case BO_ACQ_PURCHASE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_PURCHASE_CHECKED_DATA_SUBJECTCODE:
			{
				strTmpData = _T("K2_심의결과자료목록");	
			}
			break;

		case BO_ACQ_DONATE_CHECKING_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKING_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKING_DATA_SUBJECTCODE:
			{
				strTmpData = _T("K2_심의대상자료목록_기증");			
			}
			break;

		case BO_ACQ_DONATE_CHECKED_DATA_DIVNO:
		case BO_ACQ_DONATE_CHECKED_DATA_PROVIDER:
		case BO_ACQ_DONATE_CHECKED_DATA_SUBJECTCODE:
			{
				strTmpData = _T("K2_심의결과자료목록_기증");	
			}
			break;

		case BO_ACQ_ORDER_DATA_DIVNO:
		case BO_ACQ_ORDER_DATA_PROVIDER:
		case BO_ACQ_ORDER_DATA_SUBJECTCODE:
		case BO_ACQ_ORDER_DATA_MEDIACODE:
		case BO_ACQ_ORDER_DATA_ORDER:  // 2005-01-29 5163용 추가.
			{
				strTmpData = _T("K2_주문자료목록");	
			}
			break;
		}
	}
	else
	{
		if( !m_bSpecies && m_nReportType == BO_ACQ_CHECKINING_DATA )
		{
			strTmpData = m_strReportFileName + _T("_권");
		}
	}
	
	CSIReportManager * pSIReportManager = m_pPrintMgr->GetSIReportManager();

	CString sValue;
	for( INT i = 0; i< m_pPrintDM->GetRowCount()-1 ; i++ )
	{
		m_pPrintDM->GetCellData(_T("IBS_ISBN"),i,sValue );
		sValue = _T("'") + sValue;
		m_pPrintDM->SetCellData(_T("IBS_ISBN"),sValue,i);
	}

	if(pSIReportManager == NULL) return -1;
	CExcelExporter ExcelExporter(this, pSIReportManager, m_pPrintDM, strTmpData);
	INT ids = ExcelExporter.InitExcelExporter();
	if(ids < 0) return ids;

	ids = ExcelExporter.ExeclExport();
	if(ids < 0) return ids;

	return 0;
}

INT CBO_ACQ_PrintDlg::CreatePrintMgr()
{
	if(m_pPrintMgr) return  0;
	else
	{
		m_pPrintMgr = new CBasicPrintMgr(this);
	}
	return 0;
}

VOID CBO_ACQ_PrintDlg::OnbtnKOLAS2FORMAT() 
{
	m_bKOLAS2Format = TRUE;
	PostMessage(WM_COMMAND, IDOK, BN_CLICKED);	
}

VOID CBO_ACQ_PrintDlg::OnchkPRINTDATASORTYN() 
{
	CString strTmp;
	m_ComboBox.GetWindowText(strTmp);
	//20070521 ADD PJW : 주제구분일경우 무조건 정렬
	if ( strTmp.Find(_T("주제구분")) >= 0 )
	{
		( (CButton*) GetDlgItem(IDC_chkPRINT_DATA_SORT_YN) )->SetCheck(1);
		m_bDataSortYN = TRUE;
		AfxMessageBox(_T("주제구분별 출력물은 기본 정렬입니다."));
		return;
	}

	m_bDataSortYN = !m_bDataSortYN;


}

VOID CBO_ACQ_PrintDlg::OnbtnSETSORTCONDITION() 
{
	::PostMessage(pParentMgr->GetSafeHwnd(), SET_PRINT_DATA_ORDER_CONDITION, 0, 0);
}

HBRUSH CBO_ACQ_PrintDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
