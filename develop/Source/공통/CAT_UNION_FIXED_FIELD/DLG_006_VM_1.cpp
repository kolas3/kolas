// DLG_006_VM_1.cpp : implementation file
// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
// 통합마크 개정 적용

#include "stdafx.h"
#include "DLG_006_VM_1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_006_VM_1 dialog

CDLG_006_VM_1::CDLG_006_VM_1(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat006UnionFixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_006_VM_1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMarc = pMarc;
}

CDLG_006_VM_1::~CDLG_006_VM_1()
{
}

void CDLG_006_VM_1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_006_VM_1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_006_VM_1, CDialog)
	//{{AFX_MSG_MAP(CDLG_006_VM_1)
		// NOTE: the ClassWizard will add message map macros here
		ON_EN_KILLFOCUS(IDC_eFIXED008_11, OnKillfocuseFIXED00811)
		ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_006_VM_1 message handlers
INT CDLG_006_VM_1::InitData()
{
	m_strSubGroupName = _T("<지도자료(a)>");
	m_pCM->SetControlMgrData(_T("자료범주표시"), _T("a"));

	// Alias 세팅
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@1-3"),_T("상영시간"));	
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@5"),_T("이용대상자수준"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@9"),_T("개별자료형태"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@12"),_T("시청각자료의유형"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("006@13"),_T("기술수준"));

	// Control 세팅		
	m_mapControlAlias_ItemName.SetAt(_T("상영시간")				, _T("(영화나 비디오 녹화자료의 상영시간)"));
	m_mapControlAlias_ItemName.SetAt(_T("이용대상자수준")		, _T("(이용대상자 수준)"));
	m_mapControlAlias_ItemName.SetAt(_T("개별자료형태")			, _T("(개별자료 형태)"));
	m_mapControlAlias_ItemName.SetAt(_T("시청각자료의유형")		, _T("(시청각자료의 유형)"));	
	m_mapControlAlias_ItemName.SetAt(_T("기술수준")				, _T("(기술수준)"));	

	// Control에 보여주자
	Display();

	return 0;
}

INT CDLG_006_VM_1::Get006()
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
// CDLG_006_VM_1 message handlers

BOOL CDLG_006_VM_1::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_FIXED_006_VM_1")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_FIXED_006_VM_1"));
	if (!m_pCM) goto ERR;

	if (Init() < 0) goto ERR;
	if (InitData() < 0) goto ERR;

	( (CEdit*) GetDlgItem (IDC_eFIXED008_11) )->SetLimitText(3);
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);
	EndDialog(IDCANCEL);
	return FALSE;
}

BOOL CDLG_006_VM_1::PreTranslateMessage(MSG* pMsg) 
{
	return CCat006UnionFixedFieldMgr::PreTranslateMessage(pMsg);
}

void CDLG_006_VM_1::OnKillfocuseFIXED00811() 
{
	// 상영시간이 숫자로 되어 있고 3자리가 아닌경우 0으로 채워주기
	CString strText;
	CString strNum;
	INT nNum;

	( (CEdit*) GetDlgItem (IDC_eFIXED008_11) )->GetWindowText(strText);

	nNum = _ttoi(strText.GetBuffer(0));
	if ( nNum > 0 && strText.GetLength() < 3 )
	{
		strNum.Format(_T("%03d"), nNum );
		( (CEdit*) GetDlgItem (IDC_eFIXED008_11) )->SetWindowText(strNum);

	}		
}


BOOL CDLG_006_VM_1::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}
HBRUSH CDLG_006_VM_1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}