// DLG_FIXED_007_R.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_FIXED_007_R.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_R dialog


CDLG_FIXED_007_R::CDLG_FIXED_007_R(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat007UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_FIXED_007_D)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}
CDLG_FIXED_007_R::~CDLG_FIXED_007_R()
{
}

void CDLG_FIXED_007_R::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_FIXED_007_R)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_FIXED_007_R, CDialog)
	//{{AFX_MSG_MAP(CDLG_FIXED_007_R)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_R message handlers

INT CDLG_FIXED_007_R::InitData()
{
	m_strSubGroupName = _T("<원격탐사도(r)>");
	m_pCM->SetControlMgrData(_T("자료범주표시"), _T("r"));

	// Alias 세팅
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@0"),_T("특정자료종별"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@2"),_T("탐사기고도"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@3"),_T("탐사기각도"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@4"),_T("구름비율"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@5"),_T("탑재기유형"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@6"),_T("탑재기"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@7"),_T("탐사기유형"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@8"),_T("데이터유형"));

	// Control 세팅
	m_mapControlAlias_ItemName.SetAt(_T("특정자료종별")	, _T("(특정자료종별)"));
	m_mapControlAlias_ItemName.SetAt(_T("탐사기고도")	, _T("(탐사기 고도)"));
	m_mapControlAlias_ItemName.SetAt(_T("탐사기각도")	, _T("(탐사기 각도)"));
	m_mapControlAlias_ItemName.SetAt(_T("구름비율")		, _T("(구름비율)"));
	m_mapControlAlias_ItemName.SetAt(_T("탑재기유형")	, _T("(탑재기 유형)"));
	m_mapControlAlias_ItemName.SetAt(_T("탑재기")		, _T("(탑재기)"));
	m_mapControlAlias_ItemName.SetAt(_T("탐사기유형")	, _T("(탐사기 유형)"));
	m_mapControlAlias_ItemName.SetAt(_T("데이터유형")	, _T("(데이터 유형)"));

	// Control에 보여주자
	Display();

	return 0;
}

INT CDLG_FIXED_007_R::Get007()
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("(특정자료종별)"));
	arrStrControlAlias.Add(_T("(탐사기고도)"));
	arrStrControlAlias.Add(_T("(탐사기각도)"));
	arrStrControlAlias.Add(_T("(구름비율)"));
	arrStrControlAlias.Add(_T("(탑재기유형)"));
	arrStrControlAlias.Add(_T("(탑재기)"));
	arrStrControlAlias.Add(_T("(탐사기유형)"));
	arrStrControlAlias.Add(_T("(데이터유형)"));

	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// 기존의 007 지워준다.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("007"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_R message handlers

BOOL CDLG_FIXED_007_R::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_007_R")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_007_R"));
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

BOOL CDLG_FIXED_007_R::PreTranslateMessage(MSG* pMsg) 
{
	return CCat007UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_FIXED_007_R::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

HBRUSH CDLG_FIXED_007_R::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
