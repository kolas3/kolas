// Cat008FixedField_AC.cpp : implementation file
//

#include "stdafx.h"
#include "Cat008FixedField_AC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedField_AC dialog


CCat008FixedField_AC::CCat008FixedField_AC(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat008FixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCat008FixedField_AC)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}

CCat008FixedField_AC::~CCat008FixedField_AC()
{
}

VOID CCat008FixedField_AC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCat008FixedField_AC)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCat008FixedField_AC, CDialog)
	//{{AFX_MSG_MAP(CCat008FixedField_AC)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedField_AC message handlers

BOOL CCat008FixedField_AC::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_PROC_0320_FORM_13")) < 0)
	{
		m_strLastErrMsg = _T("InitESL_Mgr Error");
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_BO_CAT_PROC_0320_FORM_13"));
	if (!m_pCM) return FALSE;
	
	//2006-01-09 황정영
	if (Init() < 0) return FALSE;
	if (InitData() < 0) return FALSE;
	
	//m_pCM->
	SetWindowText(_T("전거통제 고정길이"));
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CCat008FixedField_AC::InitData()
{
EFS_BEGIN

	//전거 통제
	m_strGroupName = _T("[A]");

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Alias 세팅
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@0-5"),_T("입력날짜"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@6"),_T("지리구분"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@7"),_T("로마자번자표"));
	//8 빈칸
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@9"),_T("레코드종류"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@10"),_T("목록기술형식"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@11"),_T("주제명표목시스템"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@12"),_T("총서형태"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@13"),_T("총서번호유무"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@14"),_T("표목사용-주표목"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@15"),_T("주제부출표목"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@16"),_T("표목사용-총서명"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@17"),_T("주제세목형태"));
	//18~27 빈칸
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@28"),_T("레코드수정"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@29"),_T("참조평가"));
	//30 빈칸
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@31"),_T("레코드갱신"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@32"),_T("목록작성기관부호"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@33"),_T("채택표목수준"));
	//34~38 빈칸
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@39"),_T("동명이인"));

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Control 세팅
	m_mapControlAlias_ItemName.SetAt(_T("지리구분"), _T("(지리구분)"));
	m_mapControlAlias_ItemName.SetAt(_T("로마자번자표"), _T("(로마자번자표)"));
	
	m_mapControlAlias_ItemName.SetAt(_T("레코드종류"), _T("(레코드종류)"));
	m_mapControlAlias_ItemName.SetAt(_T("목록기술형식"), _T("(목록기술형식)"));
	m_mapControlAlias_ItemName.SetAt(_T("주제명표목시스템"), _T("(주제명표목시스템)"));
	m_mapControlAlias_ItemName.SetAt(_T("총서형태"), _T("(총서형태)"));
	m_mapControlAlias_ItemName.SetAt(_T("총서번호유무"), _T("(총서번호유무)"));
	m_mapControlAlias_ItemName.SetAt(_T("표목사용-주표목"), _T("(표목사용-주표목)"));
	m_mapControlAlias_ItemName.SetAt(_T("주제부출표목"), _T("(주제부출표목)"));
	m_mapControlAlias_ItemName.SetAt(_T("표목사용-총서명"), _T("(표목사용-총서명)"));
	m_mapControlAlias_ItemName.SetAt(_T("주제세목형태"), _T("(주제세목형태)"));
	
	m_mapControlAlias_ItemName.SetAt(_T("레코드수정"), _T("(레코드수정)"));
	m_mapControlAlias_ItemName.SetAt(_T("참조평가"), _T("(참조평가)"));

	m_mapControlAlias_ItemName.SetAt(_T("레코드갱신"), _T("(레코드갱신)"));
	m_mapControlAlias_ItemName.SetAt(_T("목록작성기관부호"), _T("(목록작성기관부호)"));
	m_mapControlAlias_ItemName.SetAt(_T("채택표목수준"), _T("(채택표목수준)"));
	m_mapControlAlias_ItemName.SetAt(_T("동명이인"), _T("(동명이인)"));

	m_mapControlAlias_ItemName.SetAt(_T("레코드상태"), _T("(레코드상태)"));
	m_mapControlAlias_ItemName.SetAt(_T("레코드형태"), _T("(레코드형태)"));
	m_mapControlAlias_ItemName.SetAt(_T("입력수준"), _T("(입력수준)"));

/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Extra 설정
	INT ids = -1;
	CString strItemData = _T("");

	// 1. 입력날자가 없으면 현재 날자를 넣는다.
	ids = m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("008@0-5"), strItemData);
	strItemData.TrimLeft();
	strItemData.TrimRight();
	if (ids < 0 || strItemData.IsEmpty())
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@0-5"), CTime::GetCurrentTime().Format(_T("%y%m%d")));

	//GetDlgItem(IDC_eINPUT_DATE)->SetWindowText(CTime::GetCurrentTime().Format(_T("%y%m%d")));	


	// 3. 발행년은 marc의 260$c의 년도를 넣는다. 없으면 현재 년도를 넣는다.
	ids = m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("260$c"), strItemData);
	
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

	return 0;


EFS_END
return FALSE;
}

VOID CCat008FixedField_AC::OnOk() 
{
EFS_BEGIN

	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("입력날짜"));

	if ( ValidateControlData(&arrStrControlAlias) < 0 ) return;
	
	ApplyControlData();
	
	// 리더 입력하기
	CString strLeader;
	m_pMarc->GetLeader(strLeader);
	
	CString strStatus;
	CString strForm;
	CString strSeoji;
	CString strInputLevel;
	
	m_pCM->GetControlMgrData(_T("레코드상태"), strStatus);
	m_pCM->GetControlMgrData(_T("레코드형태"), strForm );
	//m_pCM->GetControlMgrData(_T("서지수준"), strSeoji );
	m_pCM->GetControlMgrData(_T("입력수준"), strInputLevel );
	
	CString sTmp;
	sTmp.Format(_T("%5s%1s%1s %s%1s%s"), 
		strLeader.Left(5), 
		strStatus, 
		strForm, 
		//strSeoji, 
		strLeader.Mid(8, 9), 
		strInputLevel, 
		strLeader.Right(6));
	
	m_pMarc->SetLeader( sTmp.GetBuffer(0) );
	
	CDialog::OnOK();	
	
EFS_END
}

VOID CCat008FixedField_AC::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CCat008FixedField_AC::PreTranslateMessage(MSG* pMsg) 
{
	return CCat008FixedFieldMgr::PreTranslateMessage(pMsg);
}

HBRUSH CCat008FixedField_AC::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
