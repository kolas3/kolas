// DLG_FIXED_007_M.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_FIXED_007_M.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_M dialog


CDLG_FIXED_007_M::CDLG_FIXED_007_M(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat007UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_FIXED_007_D)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}
CDLG_FIXED_007_M::~CDLG_FIXED_007_M()
{
}

void CDLG_FIXED_007_M::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_FIXED_007_M)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_FIXED_007_M, CDialog)
	//{{AFX_MSG_MAP(CDLG_FIXED_007_M)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_M message handlers

INT CDLG_FIXED_007_M::InitData()
{
	m_strSubGroupName = _T("<영화(m)>");
	m_pCM->SetControlMgrData(_T("자료범주표시"), _T("m"));

	// Alias 세팅
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@0")	,_T("자료범주표시")		);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@1")	,_T("특정자료종별")		);	
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@3")	,_T("색채")				);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@4")	,_T("스크린형태")		);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@5")	,_T("음향유무")			);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@6")	,_T("음향수록매체")		);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@7")	,_T("필름의폭")			);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@8")	,_T("음향의재생상태")		);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@9")	,_T("제작요소")			);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@10")	,_T("감광상태")			);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@11")	,_T("복제단계")			);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@12")	,_T("필름의기본재료")	);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@13")	,_T("색채의범주")		);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@14")	,_T("다채색인화유형")	);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@15")	,_T("변질상태")			);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@16")	,_T("필름의완전성")		);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@17-22")	,_T("필름검사일자")		);

	// Control 세팅	
	m_mapControlAlias_ItemName.SetAt(_T("특정자료종별")		,_T("(특정자료종별)")	);
	m_mapControlAlias_ItemName.SetAt(_T("색채")				,_T("(색채)")			);
	m_mapControlAlias_ItemName.SetAt(_T("스크린형태")		,_T("(스크린 형태)")	);
	m_mapControlAlias_ItemName.SetAt(_T("음향유무")			,_T("(음향 유무)")		);
	m_mapControlAlias_ItemName.SetAt(_T("음향수록매체")		,_T("(음향수록 매체)")	);
	m_mapControlAlias_ItemName.SetAt(_T("필름의폭")			,_T("(필름의 폭)")		);
	m_mapControlAlias_ItemName.SetAt(_T("음향의재생상태")		,_T("(음향의 재생상태)")	);
	m_mapControlAlias_ItemName.SetAt(_T("제작요소")			,_T("(제작요소)")		);
	m_mapControlAlias_ItemName.SetAt(_T("감광상태")			,_T("(감광상태)")		);
	m_mapControlAlias_ItemName.SetAt(_T("복제단계")			,_T("(복제단계)")		);
	m_mapControlAlias_ItemName.SetAt(_T("필름의기본재료")	,_T("(필름의 기본재료)"));
	m_mapControlAlias_ItemName.SetAt(_T("색채의범주")		,_T("(색채의 범주)")	);
	m_mapControlAlias_ItemName.SetAt(_T("다채색인화유형")	,_T("(다채색 인화유형)"));
	m_mapControlAlias_ItemName.SetAt(_T("변질상태")			,_T("(변질상태)")		);
	m_mapControlAlias_ItemName.SetAt(_T("필름의완전성")		,_T("(필름의 완전성)")	);
	m_mapControlAlias_ItemName.SetAt(_T("필름검사일자")		,_T("(필름검사일자)")	);

	// Control에 보여주자
	Display();

	return 0;
}

INT CDLG_FIXED_007_M::Get007()
{
	CStringArray arrStrControlAlias;	
	
	arrStrControlAlias.Add(_T("특정자료종별")	);
	arrStrControlAlias.Add(_T("색채")			);
	arrStrControlAlias.Add(_T("스크린형태")		);
	arrStrControlAlias.Add(_T("음향유무")		);
	arrStrControlAlias.Add(_T("음향수록매체")	);
	arrStrControlAlias.Add(_T("필름의폭")		);
	arrStrControlAlias.Add(_T("음향의재생상태")		);
	arrStrControlAlias.Add(_T("제작요소")		);
	arrStrControlAlias.Add(_T("감광상태")		);
	arrStrControlAlias.Add(_T("복제단계")		);
	arrStrControlAlias.Add(_T("필름의기본재료")	);
	arrStrControlAlias.Add(_T("색채의범주")		);
	arrStrControlAlias.Add(_T("다채색인화유형")	);
	arrStrControlAlias.Add(_T("변질상태")		);
	arrStrControlAlias.Add(_T("필름의완전성")	);
	arrStrControlAlias.Add(_T("필름검사일자")	);

	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// 기존의 007 지워준다.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("007"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_M message handlers

BOOL CDLG_FIXED_007_M::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_007_M")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_007_M"));
	if (!m_pCM) goto ERR;

	if (Init() < 0) goto ERR;
	if (InitData() < 0) goto ERR;

	( (CEdit*) GetDlgItem (IDC_eFIXED008_16) )->SetLimitText(6);
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);
	EndDialog(IDCANCEL);
	return FALSE;
}

BOOL CDLG_FIXED_007_M::PreTranslateMessage(MSG* pMsg) 
{
	return CCat007UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_FIXED_007_M::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

HBRUSH CDLG_FIXED_007_M::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
