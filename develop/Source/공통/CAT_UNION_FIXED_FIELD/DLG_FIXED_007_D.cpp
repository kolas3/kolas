// DLG_FIXED_007_D.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_FIXED_007_D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_D dialog


CDLG_FIXED_007_D::CDLG_FIXED_007_D(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat007UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_FIXED_007_D)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}


void CDLG_FIXED_007_D::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_FIXED_007_D)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_FIXED_007_D, CDialog)
	//{{AFX_MSG_MAP(CDLG_FIXED_007_D)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_D message handlers

INT CDLG_FIXED_007_D::InitData()
{
	m_strSubGroupName = _T("<구체(d)>");

	m_pCM->SetControlMgrData(_T("자료범주표시"), _T("d"));
	
	// Alias 세팅
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@0"),_T("자료범주표시")	);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@1"),_T("특정자료종별")	);	
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@3"),_T("색채")			);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@4"),_T("물리적매체")		);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@5"),_T("복제유형")		);

	// Control 세팅	
	m_mapControlAlias_ItemName.SetAt(_T("특정자료종별")	, _T("(특정자료종별)")	);
	m_mapControlAlias_ItemName.SetAt(_T("색채")			, _T("(색채)")			);
	m_mapControlAlias_ItemName.SetAt(_T("물리적매체")	, _T("(물리적매체)")	);
	m_mapControlAlias_ItemName.SetAt(_T("복제유형")		, _T("(복제유형)")		);

	// Control에 보여주자
	Display();

	return 0;
}

INT CDLG_FIXED_007_D::Get007()
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("특정자료종별"));
	arrStrControlAlias.Add(_T("색채"));
	arrStrControlAlias.Add(_T("물리적매체"));
	arrStrControlAlias.Add(_T("복제유형"));	
	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// 기존의 007 지워준다.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("007"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_D message handlers

BOOL CDLG_FIXED_007_D::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_007_D")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_007_D"));
	if (!m_pCM) goto ERR;

	if (Init() < 0) goto ERR;
	if (InitData() < 0) goto ERR;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);
	EndDialog(IDCANCEL);
	return FALSE;
}

BOOL CDLG_FIXED_007_D::PreTranslateMessage(MSG* pMsg) 
{
	return CCat007UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_FIXED_007_D::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}