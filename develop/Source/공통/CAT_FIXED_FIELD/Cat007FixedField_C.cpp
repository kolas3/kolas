// Cat007FixedField_C.cpp : implementation file
//

#include "stdafx.h"
#include "Cat007FixedField_C.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCat007FixedField_C dialog


CCat007FixedField_C::CCat007FixedField_C(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat007FixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCat007FixedField_C)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}
CCat007FixedField_C::~CCat007FixedField_C()
{
}

VOID CCat007FixedField_C::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCat007FixedField_C)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CCat007FixedField_C::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BEGIN_MESSAGE_MAP(CCat007FixedField_C, CDialog)
	//{{AFX_MSG_MAP(CCat007FixedField_C)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat007FixedField_C Operations
INT CCat007FixedField_C::InitData()
{
	m_strSubGroupName = _T("<컴퓨터파일(c)>");
	m_pCM->SetControlMgrData(_T("자료범주표시"), _T("c"));

	// Alias 세팅
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@0"),_T("자료범주표시"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@1"),_T("자료식별표시"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@2"),_T("색채"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@3"),_T("규격"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@4"),_T("음향"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@5"),_T("해상도"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@8"),_T("파일포맷"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@9"),_T("품질보증"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@10"),_T("이전의정보원"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@11"),_T("압축수준"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@12"),_T("재포맷의품질"));

	// Control 세팅
	m_mapControlAlias_ItemName.SetAt(_T("자료식별표시"), _T("(자료식별표시)"));
	m_mapControlAlias_ItemName.SetAt(_T("색채"), _T("(색채)"));
	m_mapControlAlias_ItemName.SetAt(_T("규격"), _T("(규격)"));
	m_mapControlAlias_ItemName.SetAt(_T("음향"), _T("(음향)"));
	m_mapControlAlias_ItemName.SetAt(_T("해상도"), _T("(해상도)"));
	m_mapControlAlias_ItemName.SetAt(_T("파일포맷"), _T("(파일포맷)"));
	m_mapControlAlias_ItemName.SetAt(_T("품질보증"), _T("(품질보증)"));
	m_mapControlAlias_ItemName.SetAt(_T("이전의정보원"), _T("(이전의정보원)"));
	m_mapControlAlias_ItemName.SetAt(_T("압축수준"), _T("(압축수준)"));
	m_mapControlAlias_ItemName.SetAt(_T("재포맷의품질"), _T("(재포맷의품질)"));

	// Control에 보여주자
	Display();

	return 0;
}

INT CCat007FixedField_C::Get007()
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("자료식별표시"));
	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// 기존의 007 지워준다.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("007"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CCat007FixedField_C message handlers

BOOL CCat007FixedField_C::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_PROC_0320_FORM_11")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_BO_CAT_PROC_0320_FORM_11"));
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

BOOL CCat007FixedField_C::PreTranslateMessage(MSG* pMsg) 
{
	return CCat007FixedFieldMgr::PreTranslateMessage(pMsg);
}

HBRUSH CCat007FixedField_C::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
