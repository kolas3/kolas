// SEVolumeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SEVolumeDlg.h"

// 040327_HSS 연속수서api
#include "..\..\수서\SE_API\SeApi.h"

// 권입력
#include "SE_CAT_INPUT_VOL.h"
// 책입력
#include "SE_CAT_INPUT_BOOK.h"
// 부록입력
#include "SE_CAT_INPUT_APPENDIX.h"

#include "..\..\..\공통\MarcAdjust\MarcAdjustConstant.h"

// 표지관리
#include "..\..\..\공통\COVERMANAGER\COVER_MAIN.h"
#include "webbrowser2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSEVolumeDlg dialog


CSEVolumeDlg::CSEVolumeDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSEVolumeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCatApi = NULL;

	m_strBookGroupName = _T("책정보");
	m_strVolumeGroupName = _T("권호정보");
	m_strHoldBookGroupName = _T("소장책정보");
	m_strAppendixGroupName = _T("부록정보");
	m_strHoldAppendixGroupName = _T("소장부록정보");
	m_strSelectedGroupName = _T("");

	m_strVolumeGrid = _T("GRID_VOLUME");
	m_strBookGrid = _T("GRID_BOOK");
	m_strHoldBookGrid = _T("GRID_HOLD_BOOK");
	m_strAppendixGrid = _T("GRID_APPENDIX");
	m_strHoldAppendixGrid = _T("GRID_HOLD_APPENDIX");
	m_strSelectedGrid = _T("");

	m_pGrid_Volume = NULL;
	m_pGrid_Book = NULL;
	m_pGrid_HoldBook = NULL;
	m_pGrid_Appendix = NULL;
	m_pGrid_HoldAppendix = NULL;
	m_pGrid_Selected = NULL;

	m_pDM_Volume = NULL;
	m_pDM_Book = NULL;
	m_pDM_HoldBook = NULL;
	m_pDM_Appendix = NULL;
	m_pDM_HoldAppendix = NULL;
	m_pDM_Selected = NULL;

	m_bGripperSizing = FALSE;
}

CSEVolumeDlg::~CSEVolumeDlg()
{
}

VOID CSEVolumeDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSEVolumeDlg)
	DDX_Control(pDX, IDC_STATIC_GRID_GUIDLINE, m_GroupBoxCtrl);
	DDX_Control(pDX, IDC_TAB_SVF, m_TabCtrl);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CSEVolumeDlg, CDialog)
	//{{AFX_MSG_MAP(CSEVolumeDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SVF, OnSelchangeTabSvf)
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_bCONTENT_MGR, OnbCONTENTMGR)
	ON_BN_CLICKED(IDC_bDEL_VOL_BOOK, OnbDELETE)
	ON_BN_CLICKED(IDC_bINPUT_APPENDIX, OnbINPUT_APPENDIX)
	ON_BN_CLICKED(IDC_bINPUT_BOOK, OnbINPUT_BOOK)
	ON_BN_CLICKED(IDC_bINPUT_VOLUME, OnbINPUT_VOLUME)
	ON_BN_CLICKED(IDC_bCOVER, OnbCOVER)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MPROPERTY_GRID_UPDATE, OnMPropertyGridUpdate)
	ON_MESSAGE(WM_MPROPERTY_GRID_GET_DATA, OnMPropertyGridGetData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEVolumeDlg Operations

INT CSEVolumeDlg::Init()
{
	EFS_BEGIN

	if (m_pCatApi == NULL) return -1;
	
	if (!m_pDM_Volume || !m_pDM_Book || !m_pDM_HoldBook || !m_pDM_Appendix || !m_pDM_HoldAppendix) return -1;

	m_pGrid_Volume = static_cast<CESL_Grid*>(FindControl(_T("CM_연속_권호정보"), m_strVolumeGrid));
	m_pGrid_Book = static_cast<CESL_Grid*>(FindControl(_T("CM_연속_권호정보"), m_strBookGrid));
	m_pGrid_HoldBook = static_cast<CESL_Grid*>(FindControl(_T("CM_연속_권호정보"), m_strHoldBookGrid));
	m_pGrid_Appendix = static_cast<CESL_Grid*>(FindControl(_T("CM_연속_권호정보"), m_strAppendixGrid));
	m_pGrid_HoldAppendix = static_cast<CESL_Grid*>(FindControl(_T("CM_연속_권호정보"), m_strHoldAppendixGrid));
	if (!m_pGrid_Volume || !m_pGrid_Book || !m_pGrid_HoldBook || !m_pGrid_Appendix || !m_pGrid_HoldAppendix ) return -1;

	m_TabCtrl.InsertItem(0, _T("권호정보"));
	m_TabCtrl.InsertItem(1, _T("책정보"));
	m_TabCtrl.InsertItem(2, _T("부록정보"));
	m_TabCtrl.InsertItem(3, _T("소장책정보"));
	m_TabCtrl.InsertItem(4, _T("소장부록정보"));

	m_pGrid_Volume->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pGrid_Book->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pGrid_HoldBook->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pGrid_Appendix->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pGrid_HoldAppendix->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	//m_strDMAlias_Volume = _T("DM_연속_정리DM_권정보");
	//m_strDMAlias_Book = _T("DM_연속_정리DM_책정보");

	// 040930_HSS 입력인 경우 목차초록관리 안보이게
	if (m_nAppMode == MNG_INSERT)
	{
		CWnd *pWndContent = GetDlgItem(IDC_bCONTENT_MGR);
		pWndContent->ShowWindow(SW_HIDE);
	}

	// 표지정보 사용 여부에 따라 버튼 숨기기 041129_소정
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO);

	CString strQuery, strReturnValue;
	strQuery = _T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USECOVERIMG'");
	dm.GetOneValueQuery(strQuery, strReturnValue);
	if ( strReturnValue != _T("Y") )
	{
		GetDlgItem(IDC_bCOVER)->ShowWindow(FALSE);
	}

	INT ids = -1;

	ids = InitGridData(0);

	AllControlDisplay(_T("CM_연속_권호정보"), 0);
	
	Display();

	return 0;

	EFS_END
	return -1;
}

// 우측 ROW 정보 상세 화면 Data 설정
INT CSEVolumeDlg::InitGridData(INT nFlag)
{
	EFS_BEGIN

	m_grid.Init();
	m_grid.SetFixedColCount(1);
	m_grid.SetFixedColWidth(100);
	m_grid.SetDataColWidth(100);

	// 권호정보 그룹
	
	//m_grid.AddItem(_T("본   표   제"), CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T(""), _T(""), _T(""));
	//m_grid.AddItem(_T("편        저"), CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, );
	//m_grid.AddItem(_T("대 등 표 제"), CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T(""));
	//m_grid.AddItem(_T("발   행   자"), CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("001"), _T("!자료실구분.코드,!자료실구분.설명"));
	
	m_grid.AddItem(_T("권호일련번호"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_권일련번호"));
	m_grid.AddItem(_T("형 태 구 분"), m_strVolumeGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SV_형식구분_코드"), _T("!형식구분.코드,!형식구분.설명"));
	m_grid.AddItem(_T("매 체 구 분"), m_strVolumeGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SV_매체구분_코드"), _T("!매체구분.코드,!매체구분.설명"));
	m_grid.AddItem(_T("권       호"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_권호명"));
	m_grid.AddItem(_T("부차적 권호"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_부차적권호명"));
	m_grid.AddItem(_T("발   행   일"), m_strVolumeGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SV_발행일자"));
	m_grid.AddItem(_T("가        격"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_가격"));
	m_grid.AddItem(_T("화 폐 구 분"), m_strVolumeGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SV_화폐구분"), _T("!화폐부호관리.코드,!화폐부호관리.설명"));
	m_grid.AddItem(_T("가 격 성 격"), m_strVolumeGroupName, CMPropertyItem::typeComboBox, CMPropertyItem::styleNormal, _T("SV_가격성격"), _T("정가,할인가"));
	m_grid.AddItem(_T("면   장   수"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_면장수"));
	m_grid.AddItem(_T("물리적 특성"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_물리적특성"));
	m_grid.AddItem(_T("크        기"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_책크기"));
	m_grid.AddItem(_T("딸 림 자 료"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_딸림자료"));
	m_grid.AddItem(_T("권호별 서명"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_권호별서명"));
	m_grid.AddItem(_T("비       고"), m_strVolumeGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SV_비고"));

	// 권 Color 설정
	m_grid.SetGroupColor(m_strVolumeGroupName, RGB(208, 230, 238));

	// 책정보 그룹
	if (nFlag==0)		// 책
	{
		m_grid.AddItem(_T("수 입 구 분"), m_strBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_수입구분_코드"), _T("!수입구분.코드,!수입구분.설명"));
		m_grid.AddItem(_T("입  수  일"), m_strBookGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SB_입수일"));
		m_grid.AddItem(_T("책  상  태"), m_strBookGroupName, CMPropertyItem::typeComboBox, CMPropertyItem::styleReadOnly, _T("SB_자료상태"));
		m_grid.AddItem(_T("자  료  실"), m_strBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleReadOnly, _T("SB_배가위치부호"), _T("!자료실구분.코드,!자료실구분.설명"));
		m_grid.AddItem(_T("비      고"), m_strBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_비고"));
		m_grid.AddItem(_T("권/연차기호"), m_strBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_권책기호"));
		m_grid.AddItem(_T("분 류 기 호"), m_strBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_분류기호"));
		m_grid.AddItem(_T("별 치 기 호"), m_strBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_별치기호_코드"), _T("!별치기호.코드,!별치기호.설명"));
		m_grid.AddItem(_T("관 리 구 분"), m_strBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_관리구분_코드"), _T("!관리구분.코드,!관리구분.설명"));
		m_grid.AddItem(_T("배 가 일 자"), m_strBookGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SB_최초배가일자"));
		m_grid.AddItem(_T("복 본 기 호"), m_strBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_복본기호"));
		m_grid.AddItem(_T("등 록 구 분"), m_strBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_등록구분_코드"), _T("!등록구분.코드,!등록구분.설명"));
		m_grid.AddItem(_T("등 록 번 호"), m_strBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleReadOnly, _T("SB_등록번호"));

		// 책 Color 설정
		m_grid.SetGroupColor(m_strBookGroupName, RGB(220, 238, 208));
	}
	else if (nFlag==1)	// 부록
	{
		m_grid.AddItem(_T("부  록  명"), m_strAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SA_부록명"));
		m_grid.AddItem(_T("부록형식구분"), m_strAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SA_형식구분"), _T("!형식구분.코드,!형식구분.설명"));
		m_grid.AddItem(_T("부록매체구분"), m_strAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SA_매체구분"), _T("!매체구분.코드,!매체구분.설명"));
		m_grid.AddItem(_T("수 입 구 분"), m_strAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_수입구분_코드"), _T("!수입구분.코드,!수입구분.설명"));
		m_grid.AddItem(_T("입  수  일"), m_strAppendixGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SB_입수일"));
		m_grid.AddItem(_T("책  상  태"), m_strAppendixGroupName, CMPropertyItem::typeComboBox, CMPropertyItem::styleReadOnly, _T("SB_자료상태"));
		m_grid.AddItem(_T("자  료  실"), m_strAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleReadOnly, _T("SB_배가위치부호"), _T("!자료실구분.코드,!자료실구분.설명"));
		m_grid.AddItem(_T("비      고"), m_strAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_비고"));
		m_grid.AddItem(_T("권/연차기호"), m_strAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_권책기호"));
		m_grid.AddItem(_T("분 류 기 호"), m_strAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_분류기호"));
		m_grid.AddItem(_T("별 치 기 호"), m_strAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_별치기호_코드"), _T("!별치기호.코드,!별치기호.설명"));
		m_grid.AddItem(_T("관 리 구 분"), m_strAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_관리구분_코드"), _T("!관리구분.코드,!관리구분.설명"));
		m_grid.AddItem(_T("배 가 일 자"), m_strAppendixGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SB_최초배가일자"));
		m_grid.AddItem(_T("복 본 기 호"), m_strAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_복본기호"));
		m_grid.AddItem(_T("등 록 구 분"), m_strBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_등록구분_코드"), _T("!등록구분.코드,!등록구분.설명"));
		m_grid.AddItem(_T("등 록 번 호"), m_strAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleReadOnly, _T("SB_등록번호"));

		// 부록 Color 설정
		m_grid.SetGroupColor(m_strAppendixGroupName, RGB(220, 238, 208));
	}
	else if (nFlag==3)		// 책
	{
		m_grid.AddItem(_T("수 입 구 분"), m_strHoldBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_수입구분_코드"), _T("!수입구분.코드,!수입구분.설명"));
		m_grid.AddItem(_T("입  수  일"), m_strHoldBookGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SB_입수일"));
		m_grid.AddItem(_T("책  상  태"), m_strHoldBookGroupName, CMPropertyItem::typeComboBox, CMPropertyItem::styleReadOnly, _T("SB_자료상태"));
		m_grid.AddItem(_T("자  료  실"), m_strHoldBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleReadOnly, _T("SB_배가위치부호"), _T("!자료실구분.코드,!자료실구분.설명"));
		m_grid.AddItem(_T("비      고"), m_strHoldBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_비고"));
		m_grid.AddItem(_T("권/연차기호"), m_strHoldBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_권책기호"));
		m_grid.AddItem(_T("분 류 기 호"), m_strHoldBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_분류기호"));
		m_grid.AddItem(_T("별 치 기 호"), m_strHoldBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_별치기호_코드"), _T("!별치기호.코드,!별치기호.설명"));
		m_grid.AddItem(_T("관 리 구 분"), m_strHoldBookGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_관리구분_코드"), _T("!관리구분.코드,!관리구분.설명"));
		m_grid.AddItem(_T("배 가 일 자"), m_strHoldBookGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SB_최초배가일자"));
		m_grid.AddItem(_T("복 본 기 호"), m_strHoldBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_복본기호"));
		m_grid.AddItem(_T("등 록 번 호"), m_strHoldBookGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleReadOnly, _T("SB_등록번호"));

		// 책 Color 설정
		m_grid.SetGroupColor(m_strHoldBookGroupName, RGB(220, 238, 208));
	}
	else if (nFlag==4)	// 부록
	{
		m_grid.AddItem(_T("부  록  명"), m_strHoldAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SA_부록명"));
		m_grid.AddItem(_T("부록형식구분"), m_strHoldAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SA_형식구분"), _T("!형식구분.코드,!형식구분.설명"));
		m_grid.AddItem(_T("부록매체구분"), m_strHoldAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SA_매체구분"), _T("!매체구분.코드,!매체구분.설명"));
		m_grid.AddItem(_T("수 입 구 분"), m_strHoldAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_수입구분_코드"), _T("!수입구분.코드,!수입구분.설명"));
		m_grid.AddItem(_T("입  수  일"), m_strHoldAppendixGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SB_입수일"));
		m_grid.AddItem(_T("책  상  태"), m_strHoldAppendixGroupName, CMPropertyItem::typeComboBox, CMPropertyItem::styleReadOnly, _T("SB_자료상태"));
		m_grid.AddItem(_T("자  료  실"), m_strHoldAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleReadOnly, _T("SB_배가위치부호"), _T("!자료실구분.코드,!자료실구분.설명"));
		m_grid.AddItem(_T("비      고"), m_strHoldAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_비고"));
		m_grid.AddItem(_T("권/연차기호"), m_strHoldAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_권책기호"));
		m_grid.AddItem(_T("분 류 기 호"), m_strHoldAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_분류기호"));
		m_grid.AddItem(_T("별 치 기 호"), m_strHoldAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_별치기호_코드"), _T("!별치기호.코드,!별치기호.설명"));
		m_grid.AddItem(_T("관 리 구 분"), m_strHoldAppendixGroupName, CMPropertyItem::typeMultiComboBox, CMPropertyItem::styleNormal, _T("SB_관리구분_코드"), _T("!관리구분.코드,!관리구분.설명"));
		m_grid.AddItem(_T("배 가 일 자"), m_strHoldAppendixGroupName, CMPropertyItem::typeDateEditCtrl, CMPropertyItem::styleNormal, _T("SB_최초배가일자"));
		m_grid.AddItem(_T("복 본 기 호"), m_strHoldAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleNormal, _T("SB_복본기호"));
		m_grid.AddItem(_T("등 록 번 호"), m_strHoldAppendixGroupName, CMPropertyItem::typeEditCtrl, CMPropertyItem::styleReadOnly, _T("SB_등록번호"));

		// 부록 Color 설정
		m_grid.SetGroupColor(m_strHoldAppendixGroupName, RGB(220, 238, 208));
	}

	// 그룹별 DM Pointer 설정
	m_grid.SetGroupDMPtr(m_strVolumeGroupName, m_pDM_Volume);
	m_grid.SetGroupDMPtr(m_strBookGroupName, m_pDM_Book);
	m_grid.SetGroupDMPtr(m_strHoldBookGroupName, m_pDM_HoldBook);
	m_grid.SetGroupDMPtr(m_strAppendixGroupName, m_pDM_Appendix);
	m_grid.SetGroupDMPtr(m_strHoldAppendixGroupName, m_pDM_HoldAppendix);

	return 0;

	EFS_END
	return -1;
}

// 상세정보 그리드 정보 뿌려주기 (Tab Change, Sell Select)
INT CSEVolumeDlg::Display()
{
EFS_BEGIN

	INT ids;
	
	INT nCurSel = m_TabCtrl.GetCurSel();
	if (nCurSel < 0) return -1;

	CString strGroup, strGrid;
	CESL_Grid *pGrid = NULL;
	CESL_DataMgr *pDM;
	
	if (nCurSel == 0)
	{
		strGroup = m_strVolumeGroupName;
		pGrid = m_pGrid_Volume;
		pDM = m_pDM_Volume;
		strGrid = m_strVolumeGrid;

		ids = InitGridData(0);
	}
	else if (nCurSel == 1)
	{
		strGroup = m_strBookGroupName;
		pGrid = m_pGrid_Book;
		pDM = m_pDM_Book;
		strGrid = m_strBookGrid;

		ids = InitGridData(0);
	}
	else if (nCurSel == 2)
	{
		strGroup = m_strAppendixGroupName;
		pGrid = m_pGrid_Appendix;
		pDM = m_pDM_Appendix;
		strGrid = m_strAppendixGrid;

		ids = InitGridData(1);
	}
	else if (nCurSel == 3)
	{
		strGroup = m_strHoldBookGroupName;
		pGrid = m_pGrid_HoldBook;
		pDM = m_pDM_HoldBook;
		strGrid = m_strHoldBookGrid;

		ids = InitGridData(3);
	}
	else if (nCurSel == 4)
	{
		strGroup = m_strHoldAppendixGroupName;
		pGrid = m_pGrid_HoldAppendix;
		pDM = m_pDM_HoldAppendix;
		strGrid = m_strHoldAppendixGrid;

		ids = InitGridData(4);
	}
	else
		return -1;

	INT nRow = pGrid->GetRow() - 1;
	if (nRow < 0) return -1;

	// 040322_HSS 선택된 정보 입력 ++++++++++++++
	m_pDM_Selected = pDM;
	m_pGrid_Selected = pGrid;
	m_strSelectedGroupName = strGroup;
	m_strSelectedGrid = strGrid;
	// ++++++++++++++++++++++++++++++++++++++++++

	m_grid.Display(nRow, strGroup);	

	// 040323_HSS DB_Flag가 D인 경우 Height=0 ++++++++++++++++++
	INT nRows = m_pDM_Selected->GetRowCount();
	for (INT idx = 0; idx < nRows; idx++)
	{
		if (m_strSelectedGrid == m_strVolumeGrid)	// 권인 경우
		{
			if (m_pDM_Selected->GetCellData(_T("SV_DB_FLAG"), idx) == _T("D"))
			{
				m_pGrid_Selected->SetRowHeight(idx + 1, 0);
			}
		}
		else	// 책,소장책,부록,소장부록인 경우
		{
			if (m_pDM_Selected->GetCellData(_T("SB_DB_FLAG"), idx) == _T("D"))
			{
				m_pGrid_Selected->SetRowHeight(idx + 1, 0);
			}
		}
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	return 0;

EFS_END
	return -1;
}

// Tap Control 변경시 보여주기 설정
INT CSEVolumeDlg::DisplayGrids()
{
	EFS_BEGIN

	INT nCurSel = m_TabCtrl.GetCurSel();
	if (nCurSel < 0) return -1;

	CRect rcClient;
	m_GroupBoxCtrl.GetWindowRect(rcClient);
	ScreenToClient(rcClient);

	if (nCurSel == 0)
	{
		m_pGrid_Volume->MoveWindow(rcClient);

		m_pGrid_Book->ShowWindow(SW_HIDE);
		m_pGrid_Volume->ShowWindow(SW_SHOW);
		m_pGrid_HoldBook->ShowWindow(SW_HIDE);
		m_pGrid_Appendix->ShowWindow(SW_HIDE);
		m_pGrid_HoldAppendix->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 1)
	{
		m_pGrid_Book->MoveWindow(rcClient);

		m_pGrid_Book->ShowWindow(SW_SHOW);
		m_pGrid_Volume->ShowWindow(SW_HIDE);
		m_pGrid_HoldBook->ShowWindow(SW_HIDE);
		m_pGrid_Appendix->ShowWindow(SW_HIDE);
		m_pGrid_HoldAppendix->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 3)
	{
		m_pGrid_HoldBook->MoveWindow(rcClient);

		m_pGrid_Book->ShowWindow(SW_HIDE);
		m_pGrid_Volume->ShowWindow(SW_HIDE);
		m_pGrid_HoldBook->ShowWindow(SW_SHOW);
		m_pGrid_Appendix->ShowWindow(SW_HIDE);
		m_pGrid_HoldAppendix->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 2)
	{
		m_pGrid_Appendix->MoveWindow(rcClient);

		m_pGrid_Book->ShowWindow(SW_HIDE);
		m_pGrid_Volume->ShowWindow(SW_HIDE);
		m_pGrid_HoldBook->ShowWindow(SW_HIDE);
		m_pGrid_Appendix->ShowWindow(SW_SHOW);
		m_pGrid_HoldAppendix->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 4)
	{
		m_pGrid_HoldAppendix->MoveWindow(rcClient);

		m_pGrid_Book->ShowWindow(SW_HIDE);
		m_pGrid_Volume->ShowWindow(SW_HIDE);
		m_pGrid_HoldBook->ShowWindow(SW_HIDE);
		m_pGrid_Appendix->ShowWindow(SW_HIDE);
		m_pGrid_HoldAppendix->ShowWindow(SW_SHOW);
	}

	return 0;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CSEVolumeDlg message handlers

BOOL CSEVolumeDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	if (InitESL_Mgr(_T("SM_연속_권호정보")) < 0)
	{
		m_strErrMsg = _T("Init ESL Mgr Error");
		goto ERR;
	}

	if (!m_grid.SubclassDlgItem(IDC_GRID_VALUES, this))
	{
		m_strErrMsg = _T("그리드 서브클래싱 실패");
		goto ERR;
	}

	if (Init() < 0)
	{
		m_strErrMsg = _T("다이얼로그 초기화 실패");
		goto ERR;
	}

	PostMessage(WM_SIZE);

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	if (!m_strErrMsg.IsEmpty())
		AfxMessageBox(m_strErrMsg);

	EndDialog(IDCANCEL);
	return FALSE;

	EFS_END
	return FALSE;
}

VOID CSEVolumeDlg::OnSelchangeTabSvf(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN

	DisplayGrids();
	Display();

	*pResult = 0;

	EFS_END
}

VOID CSEVolumeDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	if (!GetSafeHwnd()) return;

	if (m_pGrid_Book == NULL || m_pGrid_Volume == NULL) return;

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcGrid;
	m_grid.GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);

	INT nWidth = rcGrid.Width();

	rcGrid.right = rcClient.right - 5;
	rcGrid.left = rcGrid.right - nWidth;
	rcGrid.bottom = rcClient.bottom;
	m_grid.MoveWindow(rcGrid);
	
	rcClient.top = rcGrid.top + 38;
	rcClient.right = rcGrid.left - 15;

	GetDlgItem(IDC_SIZE_GRIPPER)->SetWindowPos(this, rcClient.right, rcGrid.top, 7, rcGrid.Height(), SWP_NOZORDER);

	rcClient.right -= 5;
	m_TabCtrl.MoveWindow(rcClient);
		
	CRect rcTab;
	m_TabCtrl.GetItemRect(0, rcTab);

	rcClient.top += rcTab.Height();

	rcClient.DeflateRect(10, 10);
	m_GroupBoxCtrl.MoveWindow(rcClient);

	DisplayGrids();

	EFS_END
}

VOID CSEVolumeDlg::OnbCONTENTMGR() 
{
	INT ids;
	ids = m_pDM_Volume->GetRowCount();
	if (ids<=0)
	{
		AfxMessageBox(_T("권정보가 없습니다."));
		return;
	}
	
	CCONTENTMGR_SE_EDIT dlg(this);
	dlg.Set_Mgr(_T(""), m_pDM_Volume->DB_MGR_ALIAS, _T(""), _T("SV_권KEY"), m_pGrid_Volume->GetRow() - 1);
	dlg.DoModal() ;
}

BEGIN_EVENTSINK_MAP(CSEVolumeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSEVolumeDlg)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_VOLUME_SVF, -607 /* MouseUp */, OnMouseUpGridVolumeSvf, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_BOOK_SVF, -607 /* MouseUp */, OnMouseUpGridBookSvf, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_HOLD_BOOK_SVF, -605 /* MouseDown */, OnMouseDownGridHoldBookSvf, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_APPENDIX_SVF, -607 /* MouseUp */, OnMouseUpGridAppendixSvf, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_HOLD_APPENDIX_SVF, -605 /* MouseDown */, OnMouseDownGridHoldAppendixSvf, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_BOOK_SVF, -604 /* KeyUp */, OnKeyUpGridBookSvf, VTS_PI2 VTS_I2)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_HOLD_BOOK_SVF, -604 /* KeyUp */, OnKeyUpGridHoldBookSvf, VTS_PI2 VTS_I2)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_HOLD_APPENDIX_SVF, -604 /* KeyUp */, OnKeyUpGridHoldAppendixSvf, VTS_PI2 VTS_I2)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_VOLUME_SVF, -604 /* KeyUp */, OnKeyUpGridVolumeSvf, VTS_PI2 VTS_I2)
	ON_EVENT(CSEVolumeDlg, IDC_GRID_APPENDIX_SVF, -604 /* KeyUp */, OnKeyUpGridAppendixSvf, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSEVolumeDlg::OnMouseUpGridVolumeSvf(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

VOID CSEVolumeDlg::OnMouseUpGridBookSvf(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

VOID CSEVolumeDlg::OnMouseDownGridHoldBookSvf(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

VOID CSEVolumeDlg::OnMouseUpGridAppendixSvf(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

VOID CSEVolumeDlg::OnMouseDownGridHoldAppendixSvf(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

// 상세정보 수정시 들어옴 - 권정보와 책정보의 싱크를 맞추어준다.
// ??? - 어떻게 수정된 내용이 해당 DM에 복사되는지 모르겠음 - ??
VOID CSEVolumeDlg::OnMPropertyGridUpdate(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	INT nCurSel = m_TabCtrl.GetCurSel();
	if (nCurSel < 0) return;

	// 040322_HSS 책/부록정보와 권정보의 Sync
	// 1) 권정보를 책/부록의 UDF정보에 Copy
	// 2) 권정보의 책/부록 수 다시 맞추기(삭제/입력 등으로 변동가능)
	m_pCatApi->SyncVolumeBookAppendixDM(m_pDM_Volume, m_pDM_Book, m_pDM_HoldBook, m_pDM_Appendix, m_pDM_HoldAppendix);
	//static_cast<CCatESL_Mgr_SE*>(pParentMgr)->SyncVolumeBookAppendixDM(m_pDM_Volume, m_pDM_Book, m_pDM_HoldBook, m_pDM_Appendix, m_pDM_HoldAppendix);

	// 040529_HSS 그리드 적용
	if (nCurSel == 0)			// 권
	{
		// 권일때 수정했음을 표시
		INT nIdx = wParam;
		if (m_pDM_Volume->GetCellData(_T("SV_DB_FLAG"), nIdx) != _T("I"))
		{
			m_pDM_Volume->SetCellData(_T("SV_DB_FLAG"), m_pCatApi->GetQueryFlag(MNG_MODIFY, m_nAppMode, m_pDM_Volume->GetCellData(_T("SV_DB_FLAG"), nIdx)), nIdx);
			//m_pDM_Volume->SetCellData(_T("SV_DB_FLAG"), GetQueryFlag(MNG_UPDATE, m_nAppMode, m_pDM_Volume->GetCellData(_T("SV_DB_FLAG"), nIdx)), nIdx);
		}

		m_pGrid_Volume->Display();
		m_pGrid_Book->Display();
		m_pGrid_Appendix->Display();
		m_pGrid_Volume->SetRow(wParam+1);
	}
	else if (nCurSel == 1)
	{
		m_pGrid_Book->Display();
		m_pGrid_Book->SetRow(wParam+1);
	}
	else if (nCurSel == 2)
	{
		m_pGrid_Appendix->Display();
		m_pGrid_Appendix->SetRow(wParam+1);
	}
	else if (nCurSel == 3)
	{
		// 소장책 수정
		INT nIdx = wParam;
		if (m_pDM_HoldBook->GetCellData(_T("SB_DB_FLAG"), nIdx) == _T(""))
		{
			m_pDM_HoldBook->SetCellData(_T("SB_DB_FLAG"), m_pCatApi->GetQueryFlag(MNG_MODIFY, m_nAppMode, m_pDM_HoldBook->GetCellData(_T("SB_DB_FLAG"), nIdx)), nIdx);
			//m_pDM_Volume->SetCellData(_T("SV_DB_FLAG"), GetQueryFlag(MNG_UPDATE, m_nAppMode, m_pDM_Volume->GetCellData(_T("SV_DB_FLAG"), nIdx)), nIdx);
		}

		m_pGrid_HoldBook->Display();
		m_pGrid_HoldBook->SetRow(wParam+1);
	}
	else if (nCurSel == 4)
	{
		// 소장책 수정
		INT nIdx = wParam;
		if (m_pDM_HoldAppendix->GetCellData(_T("SB_DB_FLAG"), nIdx) == _T(""))
		{
			m_pDM_HoldAppendix->SetCellData(_T("SB_DB_FLAG"), m_pCatApi->GetQueryFlag(MNG_MODIFY, m_nAppMode, m_pDM_HoldAppendix->GetCellData(_T("SB_DB_FLAG"), nIdx)), nIdx);
		}

		m_pGrid_HoldAppendix->Display();
		m_pGrid_HoldAppendix->SetRow(wParam+1);
	}

	Display();

	EFS_END
}

// 메인그리드 뿌려줄 때 동작
// 그리드 내용 변환 함수 - Descript으로 바꾸어 준다.
INT CSEVolumeDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	EFS_BEGIN

	strGridAlias.TrimLeft();		strGridAlias.TrimRight();
	
	if (strDMFieldAlias.Find(_T("자료상태")) >= 0)
		str = GetWorkingStatusDesc(str);
	

	// 040323_HSS 제본책여부, 권호유형 추가
	else if (strDMFieldAlias.Find(_T("SB_책종류")) >= 0)
	{
		if (str == _T("B")) str.Format(_T("제본책"));
		else if (str == _T("C")) str.Format(_T("비제본책"));
	}
	else if (strDMFieldAlias.Find(_T("SV_권호유형")) >= 0)
	{
		if (str == _T("V")) str.Format(_T("일반권호"));
		else if (str == _T("B")) str.Format(_T("제본권호"));
	}

	return 0;

	EFS_END
	return -1;
}

// 상세정보 관련 - 목록에서 선택 시 자료상태의 값을 Description으로 변경
UINT CSEVolumeDlg::OnMPropertyGridGetData(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	CString strAlias;
	strAlias.Format(_T("%s"), reinterpret_cast<CString*>(wParam)->GetBuffer(0));

	if (strAlias == _T("SB_자료상태"))
	{
		CString strWorkingStatusCode;
		strWorkingStatusCode.Format(_T("%s"), reinterpret_cast<CString*>(lParam)->GetBuffer(0));
		strWorkingStatusCode = GetWorkingStatusDesc(strWorkingStatusCode);

		reinterpret_cast<CString*>(lParam)->Format(_T("%s"), strWorkingStatusCode);

		return 0;
	}
	else return -1;

	EFS_END
	return -1;
}

VOID CSEVolumeDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	CWnd *pWnd = GetDlgItem(IDC_SIZE_GRIPPER);
	CRect rcWnd;
	pWnd->GetWindowRect(rcWnd);
	ScreenToClient(rcWnd);
	if (rcWnd.PtInRect(point))
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		SetCapture();
		m_bGripperSizing = TRUE;
	}
	
	CDialog::OnLButtonDown(nFlags, point);

	EFS_END
}


VOID CSEVolumeDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	if (m_bGripperSizing)
	{
		m_bGripperSizing = FALSE;

		// 그리퍼를 대상으로 그리드들을 옮긴다.
		CWnd *pWnd = GetDlgItem(IDC_SIZE_GRIPPER);
		CRect rcWnd;
		pWnd->GetWindowRect(rcWnd);
		ScreenToClient(rcWnd);

		CRect rcTabCtrl;
		m_TabCtrl.GetWindowRect(rcTabCtrl);
		ScreenToClient(rcTabCtrl);
		rcTabCtrl.right = rcWnd.left - 5;

		m_TabCtrl.MoveWindow(rcTabCtrl);
		
		CRect rcTabItem;
		m_TabCtrl.GetItemRect(0, rcTabItem);

		rcTabCtrl.top += rcTabItem.Height();

		rcTabCtrl.DeflateRect(10, 10);
		m_GroupBoxCtrl.MoveWindow(rcTabCtrl);

		DisplayGrids();

		CRect rcGrid;
		m_grid.GetWindowRect(rcGrid);
		ScreenToClient(rcGrid);
		rcGrid.left = rcWnd.right + 5;
		m_grid.MoveWindow(rcGrid);

	}

	ReleaseCapture();
	
	CDialog::OnLButtonUp(nFlags, point);

	EFS_END
}

VOID CSEVolumeDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	EFS_BEGIN

	if (m_bGripperSizing)
	{
		CRect rcClinet;
		GetClientRect(rcClinet);
		if (!rcClinet.PtInRect(point)) return;

		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));

		CWnd *pWnd = GetDlgItem(IDC_SIZE_GRIPPER);
		CRect rcWnd;
		pWnd->GetWindowRect(rcWnd);
		ScreenToClient(rcWnd);

		rcWnd.left = point.x;

		pWnd->SetWindowPos(this, rcWnd.left, rcWnd.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
	else
	{
		CWnd *pWnd = GetDlgItem(IDC_SIZE_GRIPPER);
		CRect rcWnd;
		pWnd->GetWindowRect(rcWnd);
		ScreenToClient(rcWnd);
		if (rcWnd.PtInRect(point))
		{
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		}
	}
		
	CDialog::OnMouseMove(nFlags, point);

	EFS_END
}


// 040322_HSS 삭제, 입력 추가 ++++++++++++++++++++++++++++
VOID CSEVolumeDlg::OnbDELETE() 
{
EFS_BEGIN
	CString strMsg;
	INT idx, ids;

	m_pGrid_Selected->SelectMakeList();

	idx = m_pGrid_Selected->SelectGetHeadPosition();
	if(idx<0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
	    return;
	}

	ids = m_pGrid_Selected->SelectGetCount();
	strMsg.Format(_T("선택하신 [ %d ]건의 %s를 삭제하시겠습니까?\n정리화면에서 저장시에 DB에서 완전히 삭제됩니다.")
		, ids, m_strSelectedGroupName);
	if(AfxMessageBox(strMsg,MB_YESNO) != IDYES) return;

	CSeApi api(this);

	// 그리드를 돌면서 SB_DB_FLAG를 _T("D")로 바꾼다.
	// 삭제가능인지 확인하고, 권인 경우 책,부록까지 바꾸어야 함
	while ( idx != -1 ) {
		if (m_strSelectedGrid == m_strVolumeGrid)	// 권인 경우
		{
			if (m_pDM_Selected->GetCellData(_T("SV_DB_FLAG"),idx) == _T("I"))
			{
				//m_pDM_Selected->SetCellData(_T("SV_DB_FLAG"), _T("D"), idx);
				m_pDM_Selected->DeleteRow(idx);
				idx = SelectGetNext(_T("CM_연속_권호정보"), m_strSelectedGrid);
				continue;
			}

			CString strSPECIES_KEY, strVOL_KEY, strVOL_TYPE, strVolTitle;
			CStringArray arrayChildBookKeys;

			strSPECIES_KEY = m_pDM_Selected->GetCellData(_T("SV_종KEY"),idx);
			strVOL_KEY = m_pDM_Selected->GetCellData(_T("SV_권KEY"),idx);
			strVOL_TYPE = m_pDM_Selected->GetCellData(_T("SV_권호유형"),idx);
			strVolTitle = m_pDM_Selected->GetCellData(_T("SV_권호명"),idx);

			// 표지삭제 041130_소정추가
			if ( !strVOL_KEY.IsEmpty() )
			{
				CCOVER_MAIN dlg(this);
				dlg.DeleteCoverInfo ( strVOL_KEY.GetBuffer(0) ) ;
			
			}

			ids = api.VOL_CAT_DELETE(m_pDM_Selected,
					   strSPECIES_KEY,
					   strVOL_KEY,
					   strVOL_TYPE,
					   strVolTitle,
					   arrayChildBookKeys,
					   TRUE, TRUE);
			if (ids < 0)
			{
				if(-1002 == ids) // verifying fail
				{
					//strMsg.Format(_T("[ %s ] 자료를 삭제할 수 없습니다!"), strVolTitle);
					//AfxMessageBox(strMsg);

					idx = SelectGetNext(_T("CM_연속_권호정보"), m_strSelectedGrid);
					continue;
				}
				else 
				{
					strMsg.Format(_T("[ %s ] 자료를 삭제하던 중 문제가 발생해 작업을 중단합니다!"), strVolTitle);
					AfxMessageBox(strMsg);
					return;
				}
			}
			else m_pDM_Selected->SetCellData(_T("SV_DB_FLAG"), _T("D"), idx);
		}
		else	// 책,소장책,부록,소장부록인 경우
		{
			if (m_pDM_Selected->GetCellData(_T("SB_DB_FLAG"),idx) == _T("I"))
			{
				//m_pDM_Selected->SetCellData(_T("SB_DB_FLAG"), _T("D"), idx);
				m_pDM_Selected->DeleteRow(idx);
				idx = SelectGetNext(_T("CM_연속_권호정보"), m_strSelectedGrid);
				continue;
			}

			CString strBOOK_KEY, strACQ_TYPE_VOL_KEY, strVOL_KEY;
			CString strSPECIES_KEY, strBOOK_TYPE, strBooktitle;
			
			strBOOK_KEY = m_pDM_Selected->GetCellData(_T("SB_책KEY"),idx);
			strACQ_TYPE_VOL_KEY = m_pDM_Selected->GetCellData(_T("SB_수입구분별권KEY"),idx);
			strVOL_KEY = m_pDM_Selected->GetCellData(_T("SB_권KEY"),idx);
			strSPECIES_KEY = m_pDM_Selected->GetCellData(_T("SB_종KEY"),idx);
			strBOOK_TYPE = m_pDM_Selected->GetCellData(_T("SB_책종류"),idx);

			if (m_pDM_Selected == m_pDM_Book || m_pDM_Selected == m_pDM_HoldBook )
				strBooktitle = m_pDM_Selected->GetCellData(_T("SV_권호명"),idx);
			if (m_pDM_Selected == m_pDM_Appendix || m_pDM_Selected == m_pDM_HoldAppendix )
				strBooktitle = m_pDM_Selected->GetCellData(_T("SA_부록명"),idx);

			
			// 마지막 책일 경우(041206_소정추가)
			BOOL fIsLastBook = IsLastBook(idx);
			if( fIsLastBook )
			{
				strMsg.Format(_T("%s 권의 마지막 책입니다. 삭제할 경우 권호정보도 함께 삭제됩니다.\n정말 삭제하시겠습니까?")
					, strBooktitle);
				if(AfxMessageBox(strMsg,MB_YESNO) != IDYES) 
				{
					idx = SelectGetNext(_T("CM_연속_권호정보"), m_strSelectedGrid);
					continue;
				}
			}
	
			ids = api.BOOK_CAT_DELETE(m_pDM_Selected,
							strBOOK_KEY,strACQ_TYPE_VOL_KEY,strVOL_KEY,
							strSPECIES_KEY,strBOOK_TYPE,strBooktitle,TRUE, FALSE);
			if (ids < 0)
			{
				if(-1002 == ids) // verifying fail
				{
					//strMsg.Format(_T("[ %s ] 자료를 삭제할 수 없습니다!"), strBooktitle);
					//AfxMessageBox(strMsg);

					idx = SelectGetNext(_T("CM_연속_권호정보"), m_strSelectedGrid);
					continue;
				}
				else 
				{
					strMsg.Format(_T("[ %s ] 자료를 삭제하던 중 문제가 발생해 작업을 중단합니다!"), strBooktitle);
					AfxMessageBox(strMsg);
					return;
				}
			}
			else {
				m_pDM_Selected->SetCellData(_T("SB_DB_FLAG"), _T("D"), idx);
				// 마지막 책이면 권에도 _T("D")
				if (fIsLastBook) SetVolumeDBFlag(strVOL_KEY, _T("D"));
			}
		}
		idx = SelectGetNext(_T("CM_연속_권호정보"), m_strSelectedGrid);
	}

	// 삭제정보 Sync
	m_pCatApi->SyncVolumeBookAppendixDM(m_pDM_Volume, m_pDM_Book, m_pDM_HoldBook, m_pDM_Appendix, m_pDM_HoldAppendix);

	m_pGrid_Volume->Display();
	m_pGrid_Book->Display();
	m_pGrid_Appendix->Display();
	DisplayGrids();
	Display();

	return;
EFS_END
}

VOID CSEVolumeDlg::OnbINPUT_VOLUME() 
{
	if (m_strSelectedGroupName != m_strVolumeGroupName)
	{
		m_TabCtrl.SetCurSel(0);
		DisplayGrids();
		Display();
	}
	CSE_CAT_INPUT_VOL dlg(this);
	dlg.SetCatApi(m_pCatApi);
	if(dlg.DoModal() == IDOK)
	{
		m_pCatApi->SyncVolumeBookAppendixDM(m_pDM_Volume, m_pDM_Book, m_pDM_HoldBook, m_pDM_Appendix, m_pDM_HoldAppendix);

		m_pGrid_Volume->Display();
		m_pGrid_Book->Display();
		m_pGrid_HoldBook->Display();
		DisplayGrids();
		Display();
	}

	return;
}

VOID CSEVolumeDlg::OnbINPUT_BOOK() 
{
	// 권호정보로 가기
	if (m_strSelectedGroupName != m_strVolumeGroupName)
	{
		AfxMessageBox(_T("권호정보에서 책을 추가할 권호를 선택하여 주십시요"));
		m_TabCtrl.SetCurSel(0);
		DisplayGrids();
		Display();
		return;
	}
	
// 권호정보 있는지 확인
	INT nVolCnt = m_pDM_Volume->GetRowCount();
	if ( nVolCnt <= 0 )
	{
		AfxMessageBox(_T("책을 추가할 권호정보를 먼저 입력하여 주십시오."));
		m_TabCtrl.SetCurSel(0);
		DisplayGrids();
		Display();
		return;
	}

	INT nIdx = -1;
	INT nSelectedCnt = 0;
	CString sCheck;
	//2018년 공공도서관 책이음서비스 확대구축 및 시스템개선
	//KOL.RED.019-006 JY 연속간행물 기능 안정화
	//BEFORE=======================================
	//for( INT i = 0 ; i< nVolCnt+1; i++ )
	//AFTER========================================
	for( INT i = 0 ; i< nVolCnt; i++ )
	{
		m_pGrid_Selected->GetAt(i,0,sCheck);
		if( sCheck == _T("V") )
		{
			nIdx = i;
			nSelectedCnt += 1;
			//2018년 공공도서관 책이음서비스 확대구축 및 시스템개선
			//KOL.RED.018-006 JY 연속간행물 기능 안정화
			//ADD===================================
			sCheck = _T("");
			//======================================
		}
	}

	if( nSelectedCnt <= 0 )
	{
		AfxMessageBox(_T("책을 추가할 권호를 선정해주십시오"));
		return;
	}
	else if( nSelectedCnt > 1 )
	{
		AfxMessageBox(_T("책을 추가할 권호를 한개만 선정해주십시오"));
		return;
	}

	CString sBookCnt;
	m_pDM_Volume->GetCellData(_T("SV_책수"),nIdx,sBookCnt);
	INT nCnt = _ttoi(sBookCnt);
	
	if( nCnt < 1 )//소장 권호로 간주한다..
	{
		AfxMessageBox(_T("소장권호에 책은 추가할 수 없습니다"));
		return;
	}
	
	CSE_CAT_INPUT_BOOK dlg(this);
	dlg.InitMemberData(m_pCatApi, nIdx);
	if(dlg.DoModal() == IDOK)
	{
		m_pCatApi->SyncVolumeBookAppendixDM(m_pDM_Volume, m_pDM_Book, m_pDM_HoldBook, m_pDM_Appendix, m_pDM_HoldAppendix);

		m_TabCtrl.SetCurSel(1);
		m_pGrid_Volume->Display();
		m_pGrid_Book->Display();
		m_pGrid_HoldBook->Display();
		DisplayGrids();
		Display();
	}

	return;
}

VOID CSEVolumeDlg::OnbINPUT_APPENDIX() 
{
	// 권호정보로 가기
	if (m_strSelectedGroupName == m_strVolumeGroupName ) //|| m_strHoldBookGroupName != m_strSelectedGroupName)
	{
		// 권호정보 있는지 확인
		INT nVolCnt = m_pDM_Volume->GetRowCount();
		if ( nVolCnt <= 0 )
		{
			AfxMessageBox(_T("부록을 추가할 권호정보를 먼저 입력하여 주십시오."));
			m_TabCtrl.SetCurSel(0);
			DisplayGrids();
			Display();
			return;
		}

		INT nIdx = -1;
		INT nSelectedCnt = 0;
		CString sCheck;
		// 17/11/27 김혜영 : 연속>정리>목록완성에서 부록입력시 한개이상 자료가 선택되었다는 오류 발생
/*//BEFORE-----------------------------------------------------------------------------
		for( INT i = 0 ; i< nVolCnt+1; i++ )
*///AFTER------------------------------------------------------------------------------
		for( INT i = 0 ; i< nVolCnt; i++ )	
		{
			sCheck = _T("");
/*//END-------------------------------------------------------------------------------*/		
			m_pGrid_Selected->GetAt(i,0,sCheck);
			if( sCheck == _T("V") )
			{
				nIdx = i;
				nSelectedCnt += 1;
			}
		}

		if( nSelectedCnt <= 0 )
		{
			AfxMessageBox(_T("부록을 추가할 권호를 선정해주십시오"));
			return;
		}
		else if( nSelectedCnt > 1 )
		{
			AfxMessageBox(_T("부록을 추가할 권호를 한개만 선정해주십시오"));
			return;
		}
		
		CString sHoldCnt;
		m_pDM_Volume->GetCellData(_T("SV_소장책수"),nIdx,sHoldCnt);
		INT nCnt = _ttoi(sHoldCnt);
		
		CSE_CAT_INPUT_APPENDIX dlg(this);

		if( nCnt > 0 )//소장 권호로 간주한다..
		{
			dlg.InitMemberData(m_pCatApi, nIdx, 1);	
		}
		else
		{
			dlg.InitMemberData(m_pCatApi, nIdx, 0);
		}
					
		if(dlg.DoModal() == IDOK)
		{
			m_pCatApi->SyncVolumeBookAppendixDM(m_pDM_Volume, m_pDM_Book, m_pDM_HoldBook, m_pDM_Appendix, m_pDM_HoldAppendix);

			m_TabCtrl.SetCurSel(2);
			m_pGrid_Volume->Display();
			m_pGrid_Appendix->Display();
			m_pGrid_HoldAppendix->Display();
			DisplayGrids();
			Display();
		}
	}
	else
	{
		AfxMessageBox(_T("권호정보에서 책을 추가할 권호를 선택하여 주십시요"));
		m_TabCtrl.SetCurSel(0);
		DisplayGrids();
		Display();
		return;
	}
	return;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

CString CSEVolumeDlg::GetWorkingStatusDesc(CString strWorkingStatusCode)
{
	EFS_BEGIN

	CString strDesc = _T("") ;

	FindWorkingStatusCodeDesc( strWorkingStatusCode, strDesc );
	
	if ( strDesc != _T("") )  return strDesc;
	else return _T("");


	EFS_END
	return _T("");
}

VOID CSEVolumeDlg::FindWorkingStatusCodeDesc(CString strKey, CString &strValue )
{
	// 책 상태 설명, 코드
	CMap<CString, LPCTSTR, CString, LPCTSTR> mapWorkingStatusCodeDesc;
	mapWorkingStatusCodeDesc.RemoveAll();
	mapWorkingStatusCodeDesc.SetAt(_T("SEA111N"),_T("구입입수자료(인계대상자료)"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEA112N"),_T("구입결호자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEA211N"),_T("기증입수자료(인계대상자료)"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEA212N"),_T("기증결호자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEB111N"),_T("제본인계자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEB112N"),_T("제본결정자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEB113N"),_T("제본발주자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEB114N"),_T("제본검수자료(등록인계대상자료)"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL111N"),_T("실시간인계자료(실시간배가대상자료)"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL112N"),_T("실시간배가자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL113O"),_T("실시간폐기자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SER111N"),_T("등록인계자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SER112N"),_T("등록자료(정리인계대상자료)"));
	mapWorkingStatusCodeDesc.SetAt(_T("SER113O"),_T("가원부번호부여자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEC111N"),_T("정리대상자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEC112O"),_T("열람인계대상자료(정리완료자료)"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL211O"),_T("열람인계자료(배가대상자료)"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL212N"),_T("배가자료(소장자료)"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL213O"),_T("수리제본자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL214O"),_T("파손자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL215O"),_T("제적자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL216O"),_T("분실자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL217O"),_T("재정리자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL311O"),_T("관외대출자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL312O"),_T("관내대출자료"));
	//=====================================================
	//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
	// 18.09.27 JMJ KOL.RED.2018.008
	mapWorkingStatusCodeDesc.SetAt(_T("SEL411O"),_T("연속책두레업무진행자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL511O"),_T("통합도서타관반납"));
	//=====================================================
	mapWorkingStatusCodeDesc.SetAt(_T("SEL313O"),_T("특별대출자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL314O"),_T("긴급대출자료"));
	mapWorkingStatusCodeDesc.SetAt(_T("SEL315O"),_T("배달자료"));

	mapWorkingStatusCodeDesc.Lookup(strKey, strValue);
}

// 해당 권의 마지막 책인지 확인( 마지막 책이면 return TRUE)
BOOL CSEVolumeDlg::IsLastBook(INT nIdx)
{
EFS_BEGIN
	if ( nIdx < 0 ) return FALSE;

	// 부록인 경우 check 안함
	if (m_strSelectedGrid == m_strAppendixGrid  || m_strSelectedGrid == m_strHoldAppendixGrid)	return FALSE;

	CString sVolKey = m_pDM_Selected->GetCellData(_T("SV_권KEY"),nIdx);
	if ( sVolKey == _T("") ) return FALSE;

	CString sTmpVolKey=_T("");
	INT nCountBook = 0;

	// 책 DM에서 해당 권에 속한 책 수 확인
	for ( INT i=0; i<m_pDM_Book->GetRowCount(); i++)
	{
		if ( m_pDM_Book->GetCellData(_T("SB_DB_FLAG"),i) != _T("D") ) // 삭제된 책이 아니면
		{
			sTmpVolKey = m_pDM_Book->GetCellData(_T("SV_권KEY"), i);
			
			if ( sTmpVolKey == sVolKey )  nCountBook++;
		}
	}

	// 소장 DM에서 책 해당 권에 속한 책 수  확인
	for ( i=0; i<m_pDM_HoldBook->GetRowCount(); i++)
	{
		if ( m_pDM_HoldBook->GetCellData(_T("SB_DB_FLAG"),i) != _T("D") ) 
		{
			sTmpVolKey = m_pDM_HoldBook->GetCellData(_T("SB_권KEY"), i);//SB_권KEY

			if ( sTmpVolKey == sVolKey )  nCountBook++;
		}
	}

	if ( nCountBook == 1 ) return TRUE;

	return FALSE;
EFS_END
	return FALSE;

}


INT CSEVolumeDlg::SetVolumeDBFlag(CString sVolKey, CString sFlag)
{
EFS_BEGIN
	
	if ( sVolKey == _T("") ) return -1;
	
	CString sTmpVolKey = _T("");
	for ( INT i=0; i<m_pDM_Volume->GetRowCount(); i++)
	{
		sTmpVolKey = m_pDM_Volume->GetCellData(_T("SV_권KEY"), i);
		if ( sTmpVolKey == sVolKey ) 
		{
			m_pDM_Volume->SetCellData(_T("SV_DB_FLAG"), sFlag, i);
			return 0;
		}
	}

	return 0;

EFS_END
	return -1;
}
// 표지관리 (소정 추가 2004.11.29)
VOID CSEVolumeDlg::OnbCOVER() 
{
	// 권호정보로 가기
	if (m_strSelectedGroupName != m_strVolumeGroupName)
	{
		AfxMessageBox(_T("권호정보에서 표지정보를 추가할 권호를 선택하여 주십시요"));
		m_TabCtrl.SetCurSel(0);
		DisplayGrids();
		Display();
		return;
	}
	// 선택된 그리드 Index
	INT nVolIdx = m_pGrid_Selected->GetRow() - 1;

	//5163 표지정보 수정사항
	CString strVolumeKey;	// 
	strVolumeKey.Empty();
	TCHAR szVolumeKey[32];
	szVolumeKey[0] = _T('\0');

	CCOVER_MAIN dlg(this);
	dlg.SetPubType(_SERIAL_);
	dlg.SetLocalPath ( _T("..\\bmp\\cover_info\\") );

//	if ( m_nManageMode == MNG_INSERT )
//	{
//		dlg.SetInsertMode(TRUE);
//		//if ( dlg.DoModal() == IDOK )
//		//{
//			dlg.SetBasicName ( szVolumeKey );
//			dlg.SetSpeciesKey ( szVolumeKey );
//			dlg.DoModal();
//			dlg.GetSpeciesKey ( strVolumeKey );
//			m_pCatESLMgr->m_pDM_Volume->SetCellData(_T("SV_권KEY"), strVolumeKey , 0 );
//		//}
//	}
//	else if ( m_nManageMode == MNG_MODIFY )
//	{
		CString strRecKey;
		strRecKey = m_pDM_Volume->GetCellData(_T("SV_권KEY"), nVolIdx );
		_tcscpy ( szVolumeKey , strRecKey.GetBuffer(0) );
		dlg.SetInsertMode(FALSE);
		dlg.SetBasicName ( szVolumeKey );
		dlg.SetSpeciesKey ( szVolumeKey );
		dlg.DoModal();
//	}

	
}

VOID CSEVolumeDlg::OnKeyUpGridBookSvf(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

VOID CSEVolumeDlg::OnKeyUpGridHoldBookSvf(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

VOID CSEVolumeDlg::OnKeyUpGridHoldAppendixSvf(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

VOID CSEVolumeDlg::OnKeyUpGridVolumeSvf(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	Display();

	EFS_END
}

VOID CSEVolumeDlg::OnKeyUpGridAppendixSvf(short FAR* KeyCode, short Shift) 
{
	EFS_BEGIN

	Display();

	EFS_END
}


HBRUSH CSEVolumeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
