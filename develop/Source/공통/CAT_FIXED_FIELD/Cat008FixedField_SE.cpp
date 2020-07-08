// Cat008FixedField_SE.cpp : implementation file
//

#include "stdafx.h"
#include "Cat008FixedField_SE.h"
#include "Cat007FieldContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedField_SE dialog


CCat008FixedField_SE::CCat008FixedField_SE(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CCat008FixedFieldMgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCat008FixedField_SE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
}

CCat008FixedField_SE::~CCat008FixedField_SE()
{
}

VOID CCat008FixedField_SE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCat008FixedField_SE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCat008FixedField_SE, CDialog)
	//{{AFX_MSG_MAP(CCat008FixedField_SE)
	ON_BN_CLICKED(IDC_b007EDIT, Onb007EDIT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat008FixedField_SE Operations

INT CCat008FixedField_SE::InitData()
{
	m_strGroupName = _T("[S]");

	// Alias 세팅
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@0-5"),_T("입력날짜"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@6"),_T("발행년유형"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@7-10"),_T("발행년1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@11-14"),_T("발행년2"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@15-17"),_T("발행국부호"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@18"),_T("간행빈도"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@19"),_T("정규성"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@20"),_T("ISSN센터부호"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@21"),_T("연속간행물유형"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@22"),_T("원자료의발간형식"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@23"),_T("해당자료의발간형식"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@24"),_T("내용형식1"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@25"),_T("내용형식2"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@26-27"),_T("한국대학부호"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@28"),_T("수정"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@29"),_T("회의"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@32"),_T("목록전거"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@33"),_T("표제의언어"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@34"),_T("목록기술방식"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@35-37"),_T("언어구분"));
	m_mapMarcAlias_ControlAlias.SetAt(_T("008@38-39"),_T("정부기관부호"));

	// Control 세팅
	m_mapControlAlias_ItemName.SetAt(_T("발행년유형"), _T("(발행년유형)"));
	m_mapControlAlias_ItemName.SetAt(_T("간행빈도"), _T("(간행빈도)"));
	m_mapControlAlias_ItemName.SetAt(_T("정규성"), _T("(정규성)"));
	m_mapControlAlias_ItemName.SetAt(_T("ISSN센터부호"), _T("(ISSN센터부호)"));
	m_mapControlAlias_ItemName.SetAt(_T("연속간행물유형"), _T("(연간물유형)"));
	m_mapControlAlias_ItemName.SetAt(_T("원자료의발간형식"), _T("(원자료발간형식)"));
	m_mapControlAlias_ItemName.SetAt(_T("해당자료의발간형식"), _T("(해당자료발간형식)"));
	m_mapControlAlias_ItemName.SetAt(_T("내용형식1"), _T("(내용형식)"));
	m_mapControlAlias_ItemName.SetAt(_T("내용형식2"), _T("(내용형식)"));
	m_mapControlAlias_ItemName.SetAt(_T("수정"), _T("(수정레코드)"));
	m_mapControlAlias_ItemName.SetAt(_T("회의"), _T("(회의간행물)"));
	m_mapControlAlias_ItemName.SetAt(_T("목록전거"), _T("(목록전거)"));
	m_mapControlAlias_ItemName.SetAt(_T("표제의언어"), _T("(표제의언어)"));
	m_mapControlAlias_ItemName.SetAt(_T("목록기술방식"), _T("(목록기술방식)"));
	
	m_mapControlAlias_ItemName.SetAt(_T("레코드상태"), _T("(레코드상태)"));
	m_mapControlAlias_ItemName.SetAt(_T("레코드형태"), _T("(레코드형태)"));
	m_mapControlAlias_ItemName.SetAt(_T("서지수준"), _T("(서지수준)단행"));
	m_mapControlAlias_ItemName.SetAt(_T("입력수준"), _T("(입력수준)"));
	m_mapControlAlias_ItemName.SetAt(_T("목록기술형식"), _T("(목록기술형식)"));

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

	// 1) 발행사항 - 발행년 가져오기
	CString str260CData;
	ids = m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("260$c"), str260CData);

	// 2) 년도 숫자로
	CString strYear1,strYear2,strTmp;
	strYear1 = strYear2 = _T("");
	BOOL bIsFirst = TRUE;
	INT nLength = str260CData.GetLength();
	for (INT idx = 0; idx < nLength; idx++)
	{
		strTmp = str260CData.Mid(idx, 1);
		if ( !strTmp.Compare(_T("u")) || (strTmp >= '0' && strTmp <= '9') )
		{
			if (bIsFirst) strYear1 += strTmp;
			else strYear2 += strTmp;
		}
		else if ( strYear1.GetLength() >0 && !strTmp.Compare(_T("-")) ) bIsFirst = FALSE;
	}

	// 2. 발행년 유형이 없을때 폐간년이 없으면 c, 있으면 d를 넣는다.
	ids = m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("008@6"), strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		// 폐간년이 있으면 
		if ( strYear2.GetLength()>0) strItemData = _T("d");
		else strItemData = _T("c");
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@6"), strItemData);
	}

	// 3. 발행년은 marc의 260$c의 년도와 sync

	if (strItemData == _T("c") || strItemData == _T("u"))	// 간행중 간행물 / 미상
	{
		// 없으면 현재 년도를 넣는다.
		if (ids < 0 || strYear1.IsEmpty()) strYear1 = CTime::GetCurrentTime().Format(_T("%Y"));
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@7-10"), strYear1);

		// 계속간행/미상인 경우 발행년2는 항상 9999이당
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@11-14"), _T("9999"));
	}
	else if (strItemData == _T("d"))	// 폐간된 간행물
	{
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@7-10"), strYear1);
		m_pInfo->pMarcMgr->SetItem(m_pMarc, _T("008@11-14"), strYear2);
	}

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

	strStatus = strLeader.Mid(5,1);
	strForm = strLeader.Mid(6,1);
	strSeoji = strLeader.Mid(7,1);
	strInputLevel = strLeader.Mid(17,1);
	strMok = strLeader.Mid(18, 1);

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
// CCat008FixedField_SE message handlers

BOOL CCat008FixedField_SE::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("BO_CAT_PROC_0326")) < 0)
	{
		m_strLastErrMsg = _T("InitESL_Mgr Error");
		goto ERR;
	}

	m_pCM = FindCM(_T("CM_BO_CAT_PROC_0326"));
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

VOID CCat008FixedField_SE::OnOK() 
{
	// 040311_HSS 추가 -> 발행년1, 2에 숫자+u만 쓸 수 있도록 ex) 19uu, 198u ++++++++++++++
	CString strYear, strTmp;
	INT i;
	m_pCM->GetControlMgrData(_T("발행년1"), strYear);
	for (i=0; i<strYear.GetLength(); i++)
	{
		strTmp = strYear.Mid(i, 1);
		if (strTmp.Compare(_T("u")) && (strTmp < '0' || strTmp > '9') ){
			AfxMessageBox(_T("발행년1에는 숫자 및 'u'만 입력할 수 있습니다.\n예 : 1999, 19uu"));
			return;
		}
	}
	m_pCM->GetControlMgrData(_T("발행년2"), strYear);
	for (i=0; i<strYear.GetLength(); i++)
	{
		strTmp = strYear.Mid(i, 1);
		if (strTmp.Compare(_T("u")) && (strTmp < '0' || strTmp > '9') ){
			AfxMessageBox(_T("발행년2에는 숫자 및 'u'만 입력할 수 있습니다.\n예 : 1999, 19uu"));
			return;
		}
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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

	m_pCM->GetControlMgrData(_T("레코드상태"), strStatus);
	m_pCM->GetControlMgrData(_T("레코드형태"), strForm );
	m_pCM->GetControlMgrData(_T("서지수준"), strSeoji );
	m_pCM->GetControlMgrData(_T("입력수준"), strInputLevel );
	m_pCM->GetControlMgrData(_T("목록기술형식"), strMok );

	CString sTmp;
	sTmp.Format(_T("%5s%1s%1s%1s%s%1s%1s%s"), 
				strLeader.Left(5), 
				strStatus, 
				strForm, 
				strSeoji, 
				strLeader.Mid(8, 9), 
				strInputLevel,
				strMok,
				strLeader.Right(5));

	m_pMarc->SetLeader( sTmp.GetBuffer(0) );
	
	CDialog::OnOK();
}

BOOL CCat008FixedField_SE::PreTranslateMessage(MSG* pMsg) 
{
	return CCat008FixedFieldMgr::PreTranslateMessage(pMsg);
}

VOID CCat008FixedField_SE::Onb007EDIT() 
{
	CCat007FieldContainer dlg(this);
	dlg.m_pMarc = m_pMarc;
	dlg.m_strGroupName = m_strGroupName;
	dlg.DoModal();
}

HBRUSH CCat008FixedField_SE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
