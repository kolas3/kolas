// DLG_006_BK_1.cpp : implementation file
// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
// 통합마크 개정 적용

#include "stdafx.h"
#include "DLG_006_BK_1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_006_BK_1 dialog
CDLG_006_BK_1::CDLG_006_BK_1(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat006UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_006_BK_1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}

CDLG_006_BK_1::~CDLG_006_BK_1()
{
}

void CDLG_006_BK_1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_006_BK_1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_006_BK_1, CDialog)
	//{{AFX_MSG_MAP(CDLG_006_BK_1)
	ON_BN_CLICKED(IDC_b007EDIT, Onb007EDIT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_006_BK_1 message handlers
INT CDLG_006_BK_1::InitData()
{
	//m_strSubGroupName = _T("<문자자료(a)>");
	m_pCM->SetControlMgrData(_T("자료범주표시"), _T("a"));

	// Alias 세팅
	// 자료별	
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@1"),_T("삽화표시1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@2"),_T("삽화표시2"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@3"),_T("삽화표시3"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@4"),_T("삽화표시4"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@5"),_T("이용대상자수준"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@6"),_T("개별자료형태"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@7"),_T("내용형식1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@8"),_T("내용형식2"));		
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@9"),_T("회의간행물"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@10"),_T("기념논문집"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@11"),_T("색인"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@12"),_T("문학형식"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@13"),_T("전기"));

	// Control 세팅		
	// 자료별
	m_mapControlAlias_ItemName.SetAt(_T("삽화표시1"), _T("(삽화표시)"));
	m_mapControlAlias_ItemName.SetAt(_T("삽화표시2"), _T("(삽화표시)"));
	m_mapControlAlias_ItemName.SetAt(_T("삽화표시3"), _T("(삽화표시)"));
	m_mapControlAlias_ItemName.SetAt(_T("삽화표시4"), _T("(삽화표시)"));
	m_mapControlAlias_ItemName.SetAt(_T("이용대상자수준"), _T("(이용대상자 수준)"));
	m_mapControlAlias_ItemName.SetAt(_T("개별자료형태"), _T("(개별자료 형태)"));
	m_mapControlAlias_ItemName.SetAt(_T("내용형식1"), _T("(내용형식)"));
	m_mapControlAlias_ItemName.SetAt(_T("내용형식2"), _T("(내용형식)"));	
	m_mapControlAlias_ItemName.SetAt(_T("회의간행물"), _T("(회의간행물)"));
	m_mapControlAlias_ItemName.SetAt(_T("기념논문집"), _T("(기념논문집)"));
	m_mapControlAlias_ItemName.SetAt(_T("색인"), _T("(색인)"));	
	m_mapControlAlias_ItemName.SetAt(_T("문학형식"), _T("(문학형식)"));
	m_mapControlAlias_ItemName.SetAt(_T("전기"), _T("(전기)"));	

	// Control에 보여주자
	Display();

	return 0;
}

INT CDLG_006_BK_1::Get006()
{
	CStringArray arrStrControlAlias;	
	// 필수 항목 체크
	/*
	arrStrControlAlias.Add(_T("삽화표시1"));
	arrStrControlAlias.Add(_T("삽화표시2"));
	arrStrControlAlias.Add(_T("삽화표시3"));
	arrStrControlAlias.Add(_T("삽화표시4"));	
	*/
	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// 기존의 007 지워준다.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("006"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_006_BK_1 message handlers

BOOL CDLG_006_BK_1::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_006_BK_1")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_006_BK_1"));
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

BOOL CDLG_006_BK_1::PreTranslateMessage(MSG* pMsg) 
{
	return CCat006UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_006_BK_1::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

void CDLG_006_BK_1::Onb007EDIT() 
{
	// TODO: Add your control notification handler code here
	
}

HBRUSH CDLG_006_BK_1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
