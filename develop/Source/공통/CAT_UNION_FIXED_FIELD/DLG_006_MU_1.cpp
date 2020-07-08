// DLG_006_MU_1.cpp : implementation file
// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
// 통합마크 개정 적용

#include "stdafx.h"
#include "DLG_006_MU_1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_006_MU_1 dialog
CDLG_006_MU_1::CDLG_006_MU_1(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat006UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_006_MU_1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}
CDLG_006_MU_1::~CDLG_006_MU_1()
{
}

void CDLG_006_MU_1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_006_MU_1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_006_MU_1, CDialog)
	//{{AFX_MSG_MAP(CDLG_006_MU_1)
		// NOTE: the ClassWizard will add message map macros here
		ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_006_MU_1 message handlers
INT CDLG_006_MU_1::InitData()
{
	m_strSubGroupName = _T("<지도자료(a)>");
	m_pCM->SetControlMgrData(_T("자료범주표시"), _T("a"));

	// Alias 세팅
	// 자료별	
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@1-2")	,_T("작곡형식"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@3")	,_T("악보형식"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@4")	,_T("음악파트보"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@5")	,_T("이용대상자수준"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@6")	,_T("개별자료형태"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@7")	,_T("딸림자료1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@8")	,_T("딸림자료2"));			
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@10")	,_T("녹음자료의내용1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@11")	,_T("녹음자료의내용2"));		
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@12")	,_T("조옮김및편곡"));

	// Control 세팅		
	m_mapControlAlias_ItemName.SetAt(_T("작곡형식")			, _T("(작곡형식)"));	
	m_mapControlAlias_ItemName.SetAt(_T("악보형식")			, _T("(악보형식)"));	
	m_mapControlAlias_ItemName.SetAt(_T("음악파트보")		, _T("(음악 파트보)"));
	m_mapControlAlias_ItemName.SetAt(_T("이용대상자수준")	, _T("(이용대상자 수준)"));
	m_mapControlAlias_ItemName.SetAt(_T("개별자료형태")		, _T("(개별자료 형태)"));
	m_mapControlAlias_ItemName.SetAt(_T("딸림자료1")		, _T("(딸림자료)"));
	m_mapControlAlias_ItemName.SetAt(_T("딸림자료2")		, _T("(딸림자료)"));	
	m_mapControlAlias_ItemName.SetAt(_T("녹음자료의내용1")	, _T("(녹음자료의 내용)"));
	m_mapControlAlias_ItemName.SetAt(_T("녹음자료의내용2")	, _T("(녹음자료의 내용)"));	
	m_mapControlAlias_ItemName.SetAt(_T("조옮김및편곡")		, _T("(조 옮김 및 편곡)"));	

	// Control에 보여주자
	Display();

	return 0;
}

INT CDLG_006_MU_1::Get006()
{
	CStringArray arrStrControlAlias;	
	
	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// 기존의 007 지워준다.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("006"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_006_MU_1 message handlers

BOOL CDLG_006_MU_1::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_006_MU_1")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_006_MU_1"));
	if (!m_pCM) goto ERR;

	if (Init() < 0) goto ERR;
	if (InitData() < 0) goto ERR;
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);
	EndDialog(IDCANCEL);
	return FALSE;
}

BOOL CDLG_006_MU_1::PreTranslateMessage(MSG* pMsg) 
{
	return CCat006UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_006_MU_1::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}
HBRUSH CDLG_006_MU_1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}