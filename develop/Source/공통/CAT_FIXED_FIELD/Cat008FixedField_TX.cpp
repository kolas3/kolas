// Cat008FixedField_TX.cpp : implementation file
//

#include "stdafx.h"
#include "Cat008FixedField_TX.h"
#include "Cat007FieldContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedField_TX dialog


CCat008FixedField_TX::CCat008FixedField_TX(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat008FixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCat008FixedField_TX)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}
CCat008FixedField_TX::~CCat008FixedField_TX()
{
}

VOID CCat008FixedField_TX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCat008FixedField_TX)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCat008FixedField_TX, CDialog)
	//{{AFX_MSG_MAP(CCat008FixedField_TX)
	ON_BN_CLICKED(IDC_b007EDIT, Onb007EDIT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedField_TX Operations

INT CCat008FixedField_TX::InitData()
{
	m_strGroupName = _T("[TX]");

	// Alias 세팅
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@0-5"),_T("입력날짜"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@6"),_T("발행년유형"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@7-10"),_T("발행년1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@11-14"),_T("발행년2"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@15-17"),_T("발행국부호"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@18"),_T("삽도표시1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@19"),_T("삽도표시2"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@20"),_T("삽도표시3"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@21"),_T("삽도표시4"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@22"),_T("이용수준"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@23"),_T("자료식별"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@24"),_T("내용형식1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@25"),_T("내용형식2"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@26-27"),_T("한국대학부호"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@28"),_T("수정"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@29"),_T("회의"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@30"),_T("기념논문"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@31"),_T("색인"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@32"),_T("목록전거"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@33"),_T("문학형식"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@34"),_T("전기"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@35-37"),_T("언어구분"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@38-39"),_T("정부기관부호"));

	// Control 세팅
	m_mapControlAlias_ItemName.SetAt(_T("발행년유형"), _T("(발행년유형)"));
	m_mapControlAlias_ItemName.SetAt(_T("삽도표시1"), _T("(삽도표시)"));
	m_mapControlAlias_ItemName.SetAt(_T("삽도표시2"), _T("(삽도표시)"));
	m_mapControlAlias_ItemName.SetAt(_T("삽도표시3"), _T("(삽도표시)"));
	m_mapControlAlias_ItemName.SetAt(_T("삽도표시4"), _T("(삽도표시)"));
	m_mapControlAlias_ItemName.SetAt(_T("이용수준"), _T("(이용수준)"));
	m_mapControlAlias_ItemName.SetAt(_T("자료식별"), _T("(자료식별)"));
	m_mapControlAlias_ItemName.SetAt(_T("내용형식1"), _T("(내용형식)"));
	m_mapControlAlias_ItemName.SetAt(_T("내용형식2"), _T("(내용형식)"));
	m_mapControlAlias_ItemName.SetAt(_T("수정"), _T("(수정)"));
	m_mapControlAlias_ItemName.SetAt(_T("회의"), _T("(회의)"));
	m_mapControlAlias_ItemName.SetAt(_T("기념논문"), _T("(기념논문)"));
	m_mapControlAlias_ItemName.SetAt(_T("색인"), _T("(색인)"));
	m_mapControlAlias_ItemName.SetAt(_T("목록전거"), _T("(목록전거)"));
	m_mapControlAlias_ItemName.SetAt(_T("문학형식"), _T("(문학형식)"));
	m_mapControlAlias_ItemName.SetAt(_T("전기"), _T("(전기)"));
	
	m_mapControlAlias_ItemName.SetAt(_T("레코드상태"), _T("(레코드상태)"));
	m_mapControlAlias_ItemName.SetAt(_T("레코드형태"), _T("(레코드형태)"));
	m_mapControlAlias_ItemName.SetAt(_T("서지수준"), _T("(서지수준)단행"));
	m_mapControlAlias_ItemName.SetAt(_T("입력수준"), _T("(입력수준)"));
	m_mapControlAlias_ItemName.SetAt(_T("목록기술형식"), _T("(목록기술형식)"));
	m_mapControlAlias_ItemName.SetAt(_T("연관레코드조건"), _T("(연관레코드조건)"));

	m_mapControlAlias_ItemName.SetAt(_T("발행국부호"), _T("발행국부호"));
	m_mapControlAlias_ItemName.SetAt(_T("한국대학부호"), _T("한국대학부호"));
	m_mapControlAlias_ItemName.SetAt(_T("언어구분"), _T("언어"));
	m_mapControlAlias_ItemName.SetAt(_T("정부기관부호"), _T("정부기관부호"));


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
	CString strInputLevel;
	CString strMok;
	CString strRelation;

	strStatus = strLeader.Mid(5,1);
	strForm = strLeader.Mid(6,1);
	strSeoji = strLeader.Mid(7,1);
	strInputLevel = strLeader.Mid(17,1);
	strMok = strLeader.Mid(18, 1);
	strRelation = strLeader.Mid(19, 1);

	if (strStatus != _T(" "))
		m_pCM->SetControlMgrData(_T("레코드상태"), strStatus);	// 상태->레코드상태 (소정수정 2004.10.08)

	if (strForm != _T(" "))
		m_pCM->SetControlMgrData(_T("레코드형태"), strForm);
	
	if (strSeoji != _T(" "))
		m_pCM->SetControlMgrData(_T("서지수준"), strSeoji);
	
	if (strInputLevel != _T(" "))
		m_pCM->SetControlMgrData(_T("입력수준"), strInputLevel);
	
	if (strMok != _T(" "))
		m_pCM->SetControlMgrData(_T("목록기술형식"), strMok);

	if (strRelation != _T(" "))
		m_pCM->SetControlMgrData(_T("연관레코드조건"), strRelation);

	// 서지수준이 없을때 연속이면 s 아니면 m을 넣어준다.
	if (strSeoji == _T(" "))
	{
		if (m_pInfo->pMarcMgr->GetMarcGroup() == _T("연속"))
			strSeoji = _T("s");
		else
			strSeoji = _T("m");
		
		m_pCM->SetControlMgrData(_T("서지수준"), strSeoji);
	}


	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedField_TX message handlers

BOOL CCat008FixedField_TX::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_PROC_0320_01")) < 0)
	{
		m_strLastErrMsg = _T("InitESL_Mgr Error");
		goto ERR;
	}
	
	m_pCM = FindCM(_T("CM_BO_CAT_PROC_0320_01"));
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

VOID CCat008FixedField_TX::OnOK() 
{
	CStringArray arrStrControlAlias;
	arrStrControlAlias.Add(_T("입력날짜"));
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
	CString strInputLevel;
	CString strMok;
	CString strRelation;

	m_pCM->GetControlMgrData(_T("레코드상태"), strStatus);
	m_pCM->GetControlMgrData(_T("레코드형태"), strForm);
	m_pCM->GetControlMgrData(_T("서지수준"), strSeoji);
	m_pCM->GetControlMgrData(_T("입력수준"), strInputLevel);
	m_pCM->GetControlMgrData(_T("목록기술형식"), strMok);
	m_pCM->GetControlMgrData(_T("연관레코드조건"), strRelation);


	CString sTmp;
	sTmp.Format(_T("%5s%1s%1s%1s%s%1s%1s%1s%s"), 
				strLeader.Left(5), 
				strStatus, 
				strForm, 
				strSeoji, 
				strLeader.Mid(8, 9), 
				strInputLevel, 
				strMok,
				strRelation,
				strLeader.Right(4));

	m_pMarc->SetLeader( sTmp.GetBuffer(0) );
	
	CDialog::OnOK();
}

BOOL CCat008FixedField_TX::PreTranslateMessage(MSG* pMsg) 
{
	return CCat008FixedFieldMgr::PreTranslateMessage(pMsg);
}

VOID CCat008FixedField_TX::Onb007EDIT() 
{
	CCat007FieldContainer dlg(this);
	dlg.m_pMarc = m_pMarc;
	dlg.m_strGroupName = m_strGroupName;
	dlg.DoModal();
}

HBRUSH CCat008FixedField_TX::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
