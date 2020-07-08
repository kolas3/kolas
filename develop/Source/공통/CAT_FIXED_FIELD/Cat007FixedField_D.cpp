// Cat007FixedField_D.cpp : implementation file
//

#include "stdafx.h"
#include "Cat007FixedField_D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCat007FixedField_D dialog


CCat007FixedField_D::CCat007FixedField_D(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat007FixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCat007FixedField_D)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}
CCat007FixedField_D::~CCat007FixedField_D()
{
}
BOOL CCat007FixedField_D::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CCat007FixedField_D::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCat007FixedField_D)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCat007FixedField_D, CDialog)
	//{{AFX_MSG_MAP(CCat007FixedField_D)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat007FixedField_D Operations
INT CCat007FixedField_D::InitData()
{
	m_strSubGroupName = _T("<구체(d)>");

	m_pCM->SetControlMgrData(_T("자료범주표시"), _T("d"));
	
	// Alias 세팅
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@0"),_T("자료범주표시"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@1"),_T("자료식별표시"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@2"),_T("원본상태"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@3"),_T("색채"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@4"),_T("물리적매체"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@5"),_T("복제형태"));

	// Control 세팅
	m_mapControlAlias_ItemName.SetAt(_T("자료식별표시"), _T("(자료식별표시)"));
	m_mapControlAlias_ItemName.SetAt(_T("원본상태"), _T("(원본상태)"));
	m_mapControlAlias_ItemName.SetAt(_T("색채"), _T("(색채)"));
	m_mapControlAlias_ItemName.SetAt(_T("물리적매체"), _T("(물리적매체)"));
	m_mapControlAlias_ItemName.SetAt(_T("복제형태"), _T("(복제형태)"));

	// Control에 보여주자
	Display();

	return 0;
}

INT CCat007FixedField_D::Get007()
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("자료식별표시"));
	arrStrControlAlias.Add(_T("색채"));
	arrStrControlAlias.Add(_T("물리적매체"));
	arrStrControlAlias.Add(_T("복제형태"));
	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// 기존의 007 지워준다.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("007"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CCat007FixedField_D message handlers

BOOL CCat007FixedField_D::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_PROC_0320_FORM_02")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_BO_CAT_PROC_0320_FORM_02"));
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

BOOL CCat007FixedField_D::PreTranslateMessage(MSG* pMsg) 
{
	return CCat007FixedFieldMgr::PreTranslateMessage(pMsg);
}

HBRUSH CCat007FixedField_D::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
