// DLG_FIXED_007_H.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_FIXED_007_H.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_H dialog


CDLG_FIXED_007_H::CDLG_FIXED_007_H(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat007UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_FIXED_007_D)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}

CDLG_FIXED_007_H::~CDLG_FIXED_007_H()
{
}

void CDLG_FIXED_007_H::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_FIXED_007_H)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_FIXED_007_H, CDialog)
	//{{AFX_MSG_MAP(CDLG_FIXED_007_H)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_H message handlers

INT CDLG_FIXED_007_H::InitData()
{
	m_strSubGroupName = _T("<마이크로자료(h)>");
	m_pCM->SetControlMgrData(_T("자료범주표시"), _T("h"));

	// Alias 세팅
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@0"),_T("자료범주표시"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@1"),_T("특정자료종별"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@3"),_T("감광상태"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@4"),_T("규격"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@5"),_T("축소비율의범위"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@6-8"),_T("축소비율"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@9"),_T("색채"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@10"),_T("감광유제"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@11"),_T("복제단계"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@12"),_T("필름의기본재료"));

	// Control 세팅
	m_mapControlAlias_ItemName.SetAt(_T("특정자료종별")		, _T("(특정자료종별)"));
	m_mapControlAlias_ItemName.SetAt(_T("감광상태")			, _T("(감광상태)"));
	m_mapControlAlias_ItemName.SetAt(_T("규격")				, _T("(규격)"));
	m_mapControlAlias_ItemName.SetAt(_T("축소비율의범위")	, _T("(축소비율의 범위)"));
	m_mapControlAlias_ItemName.SetAt(_T("축소비율")			, _T("(축소비율)"));
	m_mapControlAlias_ItemName.SetAt(_T("색채")				, _T("(색채)"));
	m_mapControlAlias_ItemName.SetAt(_T("감광유제")			, _T("(감광유제)"));
	m_mapControlAlias_ItemName.SetAt(_T("복제단계")			, _T("(복제단계)"));
	m_mapControlAlias_ItemName.SetAt(_T("필름의기본재료")	, _T("(필름의 기본재료)"));	

	// Control에 보여주자
	Display();

	return 0;
}

INT CDLG_FIXED_007_H::Get007()
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("특정자료종별"))	;
	arrStrControlAlias.Add(_T("감광상태"))		;
	arrStrControlAlias.Add(_T("규격"))			;
	arrStrControlAlias.Add(_T("축소비율의범위"));
	arrStrControlAlias.Add(_T("축소비율"))		;
	arrStrControlAlias.Add(_T("색채"))			;
	arrStrControlAlias.Add(_T("감광유제"))		;
	arrStrControlAlias.Add(_T("복제단계"))		;
	arrStrControlAlias.Add(_T("필름의기본재료"));
	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// 기존의 007 지워준다.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("007"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_H message handlers

BOOL CDLG_FIXED_007_H::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_007_H")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_007_H"));
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

BOOL CDLG_FIXED_007_H::PreTranslateMessage(MSG* pMsg) 
{
	return CCat007UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_FIXED_007_H::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

HBRUSH CDLG_FIXED_007_H::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
