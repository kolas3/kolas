// DLG_FIXED_007_S.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_FIXED_007_S.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_S dialog


CDLG_FIXED_007_S::CDLG_FIXED_007_S(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat007UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_FIXED_007_D)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}
CDLG_FIXED_007_S::~CDLG_FIXED_007_S()
{
}

void CDLG_FIXED_007_S::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_FIXED_007_S)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_FIXED_007_S, CDialog)
	//{{AFX_MSG_MAP(CDLG_FIXED_007_S)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_S message handlers

INT CDLG_FIXED_007_S::InitData()
{
	m_strSubGroupName = _T("<녹음자료(s)>");
	m_pCM->SetControlMgrData(_T("자료범주표시"), _T("s"));

	// Alias 세팅
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@0"),_T("자료범주표시")	);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@1"),_T("특정자료종별")	);	
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@3"),_T("속도")			);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@4"),_T("녹음재생형태")	);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@5"),_T("음구의폭/높이")	);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@6"),_T("크기")			);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@7"),_T("테이프폭")		);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@8"),_T("테이프트랙의수")	);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@9"),_T("음반,실린더,테이프의종류"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@10"),_T("재질")			);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@11"),_T("음구의깍임종류")	);
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@12"),_T("특수재생장치특성"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("007@13"),_T("녹음/저장기법")	);

	// Control 세팅	
	m_mapControlAlias_ItemName.SetAt(_T("특정자료종별")		,_T("(특정자료종별)"));	
	m_mapControlAlias_ItemName.SetAt(_T("속도")				,_T("(속도)"));			
	m_mapControlAlias_ItemName.SetAt(_T("녹음재생형태")		,_T("(녹음재생 형태)"));	
	m_mapControlAlias_ItemName.SetAt(_T("음구의폭/높이")	,_T("(음구의 폭/높이)"));	
	m_mapControlAlias_ItemName.SetAt(_T("크기")				,_T("(크기)"));			
	m_mapControlAlias_ItemName.SetAt(_T("테이프폭")			,_T("(테이프 폭)"));		
	m_mapControlAlias_ItemName.SetAt(_T("테이프트랙의수")	,_T("(테이프 트랙의 수)"));	
	m_mapControlAlias_ItemName.SetAt(_T("음반,실린더,테이프의종류"), _T("(음반, 실린더, 테이프의 종류)"));
	m_mapControlAlias_ItemName.SetAt(_T("재질")				,_T("(재질)"));
	m_mapControlAlias_ItemName.SetAt(_T("음구의깍임종류")	,_T("(음구의 깍임 종류)"));
	m_mapControlAlias_ItemName.SetAt(_T("특수재생장치특성")	,_T("(특수재생장치특성)"));
	m_mapControlAlias_ItemName.SetAt(_T("녹음/저장기법")	,_T("(녹음/저장 기법)"));	


	// Control에 보여주자
	Display();

	return 0;
}

INT CDLG_FIXED_007_S::Get007()
{
	CStringArray arrStrControlAlias;	
	arrStrControlAlias.Add(_T("특정자료종별")	);	
	arrStrControlAlias.Add(_T("속도")			);
	arrStrControlAlias.Add(_T("녹음재생형태")	);
	arrStrControlAlias.Add(_T("음구의폭/높이")	);
	arrStrControlAlias.Add(_T("크기")			);
	arrStrControlAlias.Add(_T("테이프폭")		);
	arrStrControlAlias.Add(_T("테이프트랙의수")	);
	arrStrControlAlias.Add(_T("음반,실린더,테이프의종류"));
	arrStrControlAlias.Add(_T("재질")			);
	arrStrControlAlias.Add(_T("음구의깍임종류")	);
	arrStrControlAlias.Add(_T("특수재생장치특성"));
	arrStrControlAlias.Add(_T("녹음/저장기법")	);
	
	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return ids;

	// 기존의 007 지워준다.
	m_pInfo->pMarcMgr->DeleteField(m_pMarc, _T("007"));
	
	ids = ApplyControlData();
	if (ids < 0) return ids;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007_S message handlers

BOOL CDLG_FIXED_007_S::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_007_S")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_007_S"));
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

BOOL CDLG_FIXED_007_S::PreTranslateMessage(MSG* pMsg) 
{
	return CCat007UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

BOOL CDLG_FIXED_007_S::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

HBRUSH CDLG_FIXED_007_S::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
