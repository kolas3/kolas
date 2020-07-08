// DLG_008_MX_1.cpp : implementation file
// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
// 통합마크 개정 적용

#include "stdafx.h"
#include "resource.h"
#include "DLG_008_MX_1.h"
#include "DLG_FIXED_007.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_008_MX_1 dialog


CDLG_008_MX_1::CDLG_008_MX_1(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat008UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_008_MX_1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}

CDLG_008_MX_1::~CDLG_008_MX_1()
{
}
void CDLG_008_MX_1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_008_MX_1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_008_MX_1, CDialog)
	//{{AFX_MSG_MAP(CDLG_008_MX_1)
	ON_BN_CLICKED(IDC_b007EDIT, Onb007EDIT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_008_MX_1 message handlers

BOOL CDLG_008_MX_1::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

INT CDLG_008_MX_1::InitData()
{
	m_strGroupName = _T("[MX]");

	// Alias 세팅
	// 공통
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@0-5"),_T("입력일자"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@6"),_T("발행년유형"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@7-10"),_T("발행년1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@11-14"),_T("발행년2"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@15-17"),_T("발행국부호"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@26-27"),_T("한국대학부호"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@28"),_T("수정레코드"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@32"),_T("목록전거"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@35-37"),_T("언어"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@38-39"),_T("정부기관부호"));

	// 자료별	
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@23"),_T("개별자료형태"));				

	// Control 세팅
	// 공통
	m_mapControlAlias_ItemName.SetAt(_T("발행년유형"), _T("(발행년유형)"));
	m_mapControlAlias_ItemName.SetAt(_T("발행국부호"), _T("발행국부호"));
	m_mapControlAlias_ItemName.SetAt(_T("한국대학부호"), _T("한국대학부호"));
	m_mapControlAlias_ItemName.SetAt(_T("수정레코드"), _T("(수정레코드)"));
	m_mapControlAlias_ItemName.SetAt(_T("목록전거"), _T("(목록전거)"));
	m_mapControlAlias_ItemName.SetAt(_T("언어"), _T("언어"));	
	m_mapControlAlias_ItemName.SetAt(_T("정부기관부호"), _T("정부기관부호"));

	// 자료별
	m_mapControlAlias_ItemName.SetAt(_T("개별자료형태"), _T("(개별자료 형태)"));	

	// 리더
	m_mapControlAlias_ItemName.SetAt(_T("리더_레코드상태"), _T("(레코드상태)"));
	m_mapControlAlias_ItemName.SetAt(_T("리더_레코드유형"), _T("(레코드유형)"));
	m_mapControlAlias_ItemName.SetAt(_T("리더_서지수준"), _T("(서지수준)"));
	m_mapControlAlias_ItemName.SetAt(_T("리더_제어유형"), _T("(제어유형)"));
	m_mapControlAlias_ItemName.SetAt(_T("리더_문자부호체계"), _T("(문자부호체계)"));
	m_mapControlAlias_ItemName.SetAt(_T("리더_입력수준"), _T("(입력수준)"));
	m_mapControlAlias_ItemName.SetAt(_T("리더_목록기술형식"), _T("(목록기술형식)"));
	m_mapControlAlias_ItemName.SetAt(_T("리더_다권본자원의레코드수준"), _T("(다권본 자원의 레코드 수준)"));

	// Extra 설정
	INT ids = -1;
	CString strItemData;

	// 1. 입력날자가 없으면 현재 날자를 넣는다.
	ids = m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("008@0-5"), strItemData);
	strItemData.TrimLeft();
	strItemData.TrimRight();
	if (ids < 0 || strItemData.IsEmpty())
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@0-5"), CTime::GetCurrentTime().Format(_T("%y%m%d")));
	// 2. 발행년 유형이 없으면 s를 넣는다.
	ids = m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("008@6"), strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		strItemData = _T("s");
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@6"), strItemData);
	}

	// s일 경우
	if (strItemData == _T("s"))
	{
		// 3. 발행년은 marc의 260$c의 년도를 넣는다. 없으면 현재 년도를 넣는다.
		ids = m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("260$c"), strItemData);

		// 년도 숫자로
		CString strYear;
		INT nLength = strItemData.GetLength();
		TCHAR tch = 0;
		for (INT idx = 0; idx < nLength; idx++)
		{
			tch = strItemData.GetAt(idx);
			if (_istdigit(tch))
				strYear += tch;
		}

		if (ids < 0 || strYear.IsEmpty())
			strYear = CTime::GetCurrentTime().Format(_T("%Y"));
		
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@7-10"), strYear);
	}
	// 008 자리수를 40으로 만든다.
	CString s008;
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("008*"), s008);
	if ( s008.GetLength() < 40 ) {
		while ( s008.GetLength() < 40 ) {
			s008 = s008 + _T(" ");
		}
	}
	m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008*"), s008 );

	// Control에 보여주자
	Display();		

	// 리더
	CString strLeader;
	m_pMarc->GetLeader(strLeader);

	CString strStatus;
	CString strForm;
	CString strSeoji;
	CString strControlType;
	CString strCharset;
	CString strInputLevel;
	CString strMok;
	CString strRelation;

	strStatus = strLeader.Mid(5,1);
	strForm = strLeader.Mid(6,1);
	strSeoji = strLeader.Mid(7,1);
	strControlType = strLeader.Mid(8,1);
	strCharset = strLeader.Mid(9,1);
	strInputLevel = strLeader.Mid(17,1);
	strMok = strLeader.Mid(18, 1);
	strRelation = strLeader.Mid(19, 1);

	if (strStatus != _T(" "))
		m_pCM->SetControlMgrData(_T("리더_레코드상태"), strStatus);	// 상태->레코드상태 (소정수정 2004.10.08)

	if (strForm != _T(" "))
		m_pCM->SetControlMgrData(_T("리더_레코드유형"), strForm);
	
	if (strSeoji != _T(" "))
		m_pCM->SetControlMgrData(_T("리더_서지수준"), strSeoji);
	
	if (strControlType != _T(" "))
		m_pCM->SetControlMgrData(_T("리더_제어유형"), strControlType);

	if (strCharset != _T(" "))
		m_pCM->SetControlMgrData(_T("리더_문자부호체계"), strCharset);
	
	if (strMok != _T(" "))
		m_pCM->SetControlMgrData(_T("리더_목록기술형식"), strMok);

	if (strRelation != _T(" "))
		m_pCM->SetControlMgrData(_T("리더_다권본자원의레코드수준"), strRelation);
	

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_008_MX_1 message handlers

BOOL CDLG_008_MX_1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_FIXED_008_MX_1")) < 0)
	{
		m_strLastErrMsg = _T("InitESL_Mgr Error");
		goto ERR;
	}
	
	m_pCM = FindCM(_T("CM_FIXED_008_MX_1"));
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

VOID CDLG_008_MX_1::OnOK() 
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("입력일자"));
	arrStrControlAlias.Add(_T("발행년1"));
	arrStrControlAlias.Add(_T("발행년유형"));
	INT ids = ValidateControlData(&arrStrControlAlias);
	if (ids < 0) return;
	
	ApplyControlData();

	// 리더 입력하기
	CString strLeader;
	m_pMarc->GetLeader(strLeader);

	CString strStatus;
	CString strForm;
	CString strSeoji;
	CString strControlType;
	CString strCharset;
	CString strInputLevel;
	CString strMok;
	CString strRelation;

	m_pCM->GetControlMgrData(_T("리더_레코드상태"), strStatus);
	m_pCM->GetControlMgrData(_T("리더_레코드유형"), strForm);
	m_pCM->GetControlMgrData(_T("리더_서지수준"), strSeoji);
	m_pCM->GetControlMgrData(_T("리더_제어유형"), strControlType);
	m_pCM->GetControlMgrData(_T("리더_문자부호체계"), strCharset);
	m_pCM->GetControlMgrData(_T("리더_입력수준"), strInputLevel);
	m_pCM->GetControlMgrData(_T("리더_목록기술형식"), strMok);
	m_pCM->GetControlMgrData(_T("리더_다권본자원의레코드수준"), strRelation);


	CString sTmp;
	sTmp.Format(_T("%5s%1s%1s%1s%1s%1s%s%1s%1s%1s%s"), 
				strLeader.Left(5), 
				strStatus, 
				strForm, 
				strSeoji, 
				strControlType,
				strCharset,
				strLeader.Mid(10, 7), 
				strInputLevel, 
				strMok,
				strRelation,
				strLeader.Right(4));

	m_pMarc->SetLeader( sTmp.GetBuffer(0) );
	
	CDialog::OnOK();
}

BOOL CDLG_008_MX_1::PreTranslateMessage(MSG* pMsg) 
{
	return CCat008UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

void CDLG_008_MX_1::Onb007EDIT() 
{
	CDLG_FIXED_007 dlg(this);
	dlg.m_pMarc = m_pMarc;
	dlg.m_strGroupName = m_strGroupName;
	dlg.DoModal();		
}
HBRUSH CDLG_008_MX_1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}