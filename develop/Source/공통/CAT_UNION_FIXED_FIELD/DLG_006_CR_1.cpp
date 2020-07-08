// DLG_006_CR_1.cpp : implementation file
// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
// 통합마크 개정 적용

#include "stdafx.h"
#include "DLG_006_CR_1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_006_CR_1 dialog
CDLG_006_CR_1::CDLG_006_CR_1(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat006UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_006_CR_1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}

CDLG_006_CR_1::~CDLG_006_CR_1()
{
}

void CDLG_006_CR_1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_006_CR_1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_006_CR_1, CDialog)
	//{{AFX_MSG_MAP(CDLG_006_CR_1)
		// NOTE: the ClassWizard will add message map macros here
		ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_006_CR_1 message handlers
INT CDLG_006_CR_1::InitData()
{
	//m_strSubGroupName = _T("<지도자료(a)>");
	m_pCM->SetControlMgrData(_T("자료범주표시"), _T("a"));

	// Alias 세팅
	// 자료별
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@1"),_T("간행빈도"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@2"),_T("정규성"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@4"),_T("계속자료의유형"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@5"),_T("원자료의발간형식"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@6"),_T("개별자료형태"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@7"),_T("내용형식1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@8"),_T("내용형식2"));		
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@9"),_T("회의간행물"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@12"),_T("표제의언어"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@13"),_T("표제기본표목변경시레코드생성방식"));

	// Control 세팅		
	// 자료별		
	m_mapControlAlias_ItemName.SetAt(_T("간행빈도"), _T("(간행빈도)"));
	m_mapControlAlias_ItemName.SetAt(_T("정규성"), _T("(정규성)"));	
	m_mapControlAlias_ItemName.SetAt(_T("계속자료의유형"), _T("(계속자료의 유형)"));
	m_mapControlAlias_ItemName.SetAt(_T("원자료의발간형식"), _T("(원자료의 발간형식)"));
	m_mapControlAlias_ItemName.SetAt(_T("개별자료형태"), _T("(개별자료 형태)"));
	m_mapControlAlias_ItemName.SetAt(_T("내용형식1"), _T("(내용형식)"));	
	m_mapControlAlias_ItemName.SetAt(_T("내용형식2"), _T("(내용형식)"));
	m_mapControlAlias_ItemName.SetAt(_T("회의간행물"), _T("(회의간행물)"));
	m_mapControlAlias_ItemName.SetAt(_T("표제의언어"), _T("(표제의 언어)"));	
	m_mapControlAlias_ItemName.SetAt(_T("표제기본표목변경시레코드생성방식"), _T("(표제 기본표목 변경시 레코드생성방식)"));

	// Control에 보여주자
	Display();

	return 0;
}

INT CDLG_006_CR_1::Get006()
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
// CDLG_006_CR_1 message handlers

BOOL CDLG_006_CR_1::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_006_CR_1")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_006_CR_1"));
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

BOOL CDLG_006_CR_1::PreTranslateMessage(MSG* pMsg) 
{
	return CCat006UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_006_CR_1::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}
HBRUSH CDLG_006_CR_1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}