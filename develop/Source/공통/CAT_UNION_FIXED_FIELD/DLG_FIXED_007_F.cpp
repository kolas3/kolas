// DLG_FIXED_007_F.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_FIXED_007_F.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_F dialog


CDLG_FIXED_007_F::CDLG_FIXED_007_F(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat007UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_FIXED_007_D)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}

CDLG_FIXED_007_F::~CDLG_FIXED_007_F()
{
}

void CDLG_FIXED_007_F::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_FIXED_007_F)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_FIXED_007_F, CDialog)
	//{{AFX_MSG_MAP(CDLG_FIXED_007_F)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_F message handlers

INT CDLG_FIXED_007_F::InitData()
{
	m_strSubGroupName = _T("<촉감자료(f)>");
	m_pCM->SetControlMgrData(_T("자료범주표시"), _T("f"));

	// Alias 세팅
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@0"),_T("자료범주표시"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@1"),_T("특정자료종별"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@3"),_T("점자체계의종류1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@4"),_T("점자체계의종류2"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@5"),_T("축약수준"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@6"),_T("점자악보형식1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@7"),_T("점자악보형식2"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@8"),_T("점자악보형식3"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@9"),_T("형태적특징"));	

	// Control 세팅	
	m_mapControlAlias_ItemName.SetAt(_T("특정자료종별"), _T("(특정자료종별)"));
	m_mapControlAlias_ItemName.SetAt(_T("점자체계의종류1"), _T("(점자체계의 종류)"));
	m_mapControlAlias_ItemName.SetAt(_T("점자체계의종류2"), _T("(점자체계의 종류)"));
	m_mapControlAlias_ItemName.SetAt(_T("축약수준"), _T("(축약수준)"));
	m_mapControlAlias_ItemName.SetAt(_T("점자악보형식"), _T("(점자악보형식)"));	
	m_mapControlAlias_ItemName.SetAt(_T("점자악보형식2"), _T("(점자악보형식)"));	
	m_mapControlAlias_ItemName.SetAt(_T("점자악보형식3"), _T("(점자악보형식)"));	
	m_mapControlAlias_ItemName.SetAt(_T("형태적특징"), _T("(형태적특징)"));

	// Control에 보여주자
	Display();

	return 0;
}

INT CDLG_FIXED_007_F::Get007()
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("특정자료종별"));
	arrStrControlAlias.Add(_T("점자체계의종류1"));
	arrStrControlAlias.Add(_T("점자체계의종류2"));
	arrStrControlAlias.Add(_T("축약수준"));
	arrStrControlAlias.Add(_T("점자악보형식"));
	arrStrControlAlias.Add(_T("점자악보형식2"));
	arrStrControlAlias.Add(_T("점자악보형식3"));
	arrStrControlAlias.Add(_T("형태적특징"));
	
	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// 기존의 007 지워준다.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("007"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_F message handlers

BOOL CDLG_FIXED_007_F::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_007_F")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_007_F"));
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

BOOL CDLG_FIXED_007_F::PreTranslateMessage(MSG* pMsg) 
{
	return CCat007UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_FIXED_007_F::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

HBRUSH CDLG_FIXED_007_F::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
