// DLG_FIXED_007_C.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_FIXED_007_C.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_C dialog


CDLG_FIXED_007_C::CDLG_FIXED_007_C(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat007UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_FIXED_007_C)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}

CDLG_FIXED_007_C::~CDLG_FIXED_007_C()
{
}

void CDLG_FIXED_007_C::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_FIXED_007_C)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_FIXED_007_C, CDialog)
	//{{AFX_MSG_MAP(CDLG_FIXED_007_C)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_C message handlers

INT CDLG_FIXED_007_C::InitData()
{
	m_strSubGroupName = _T("<전자자료(c)>");
	m_pCM->SetControlMgrData(_T("자료범주표시"), _T("c"));

	// Alias 세팅
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@0"),_T("자료범주표시"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@1"),_T("특정자료종별"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@3"),_T("색채"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@4"),_T("규격"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@5"),_T("음향"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@6-8"),_T("이미지의비트깊이"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@9"),_T("파일형식의종수"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@10"),_T("품질보증지표"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@11"),_T("원시자료의정보원"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@12"),_T("압축수준"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@13"),_T("재포맷의품질"));

	// Control 세팅
	m_mapControlAlias_ItemName.SetAt(_T("특정자료종별")			, _T("(특정자료종별)")	);
	m_mapControlAlias_ItemName.SetAt(_T("색채")					, _T("(색채)")		);
	m_mapControlAlias_ItemName.SetAt(_T("규격")					, _T("(규격)"));
	m_mapControlAlias_ItemName.SetAt(_T("음향")					, _T("(음향)"));
	m_mapControlAlias_ItemName.SetAt(_T("이미지의비트깊이")		, _T("(이미지의 비트깊이)"));
	m_mapControlAlias_ItemName.SetAt(_T("파일형식의종수")		, _T("(파일형식의 종수)"));
	m_mapControlAlias_ItemName.SetAt(_T("품질보증지표")			, _T("(품질보증지표)"));
	m_mapControlAlias_ItemName.SetAt(_T("원시자료의정보원")		, _T("(원시자료의 정보원)"));
	m_mapControlAlias_ItemName.SetAt(_T("압축수준")				, _T("(압축수준)"));
	m_mapControlAlias_ItemName.SetAt(_T("재포맷의품질")			, _T("(재포맷의 품질)"));

	// Control에 보여주자
	Display();

	return 0;
}

INT CDLG_FIXED_007_C::Get007()
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("특정자료종별"));
	arrStrControlAlias.Add(_T("색채"));
	arrStrControlAlias.Add(_T("규격"));
	arrStrControlAlias.Add(_T("음향"));
	arrStrControlAlias.Add(_T("이미지의비트깊이"));
	arrStrControlAlias.Add(_T("파일형식의종수"));
	arrStrControlAlias.Add(_T("품질보증지표"));
	arrStrControlAlias.Add(_T("원시자료의정보원"));
	arrStrControlAlias.Add(_T("압축수준"));
	arrStrControlAlias.Add(_T("재포맷의품질"));
	arrStrControlAlias.Add(_T("필름검사일자"));
	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// 기존의 007 지워준다.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("007"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_C message handlers

BOOL CDLG_FIXED_007_C::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_007_C")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_007_C"));
	if (!m_pCM) goto ERR;

	if (Init() < 0) goto ERR;
	if (InitData() < 0) goto ERR;

	( (CEdit*) GetDlgItem (IDC_eFIXED008_5) )->SetLimitText(3);
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);
	EndDialog(IDCANCEL);
	return FALSE;
}

BOOL CDLG_FIXED_007_C::PreTranslateMessage(MSG* pMsg) 
{
	return CCat007UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_FIXED_007_C::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

HBRUSH CDLG_FIXED_007_C::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
