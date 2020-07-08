// BO_CAT_LAST_NUMBER_1000.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_LAST_NUMBER_1000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BO_CAT_LAST_NUMBER_2000.h"

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_LAST_NUMBER_1000 dialog


CBO_CAT_LAST_NUMBER_1000::CBO_CAT_LAST_NUMBER_1000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_LAST_NUMBER_1000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//BUSINESS_MODE = _T("LIGHT");
	BUSINESS_MODE = _T("UP");

	m_nDatas = 0;

	m_strGroup1[m_nDatas] = _T("단행");
	m_strGroup2[m_nDatas] = _T("기증접수번호");
	m_strGroup3[m_nDatas] = _T("MO");
	m_strGroup4[m_nDatas] = _T("DON_NO");
	m_strGroup5[m_nDatas] = _T("기증접수번호");
	m_nDatas++;

	m_strGroup1[m_nDatas] = _T("단행");
	m_strGroup2[m_nDatas] = _T("그룹번호");
	m_strGroup3[m_nDatas] = _T("MO");
	m_strGroup4[m_nDatas] = _T("GRP_NO");
	m_strGroup5[m_nDatas] = _T("그룹번호");
	m_nDatas++;

	m_strGroup1[m_nDatas] = _T("단행");
	m_strGroup2[m_nDatas] = _T("차수번호");
	m_strGroup3[m_nDatas] = _T("MO");
	m_strGroup4[m_nDatas] = _T("PUR_NO");
	m_strGroup5[m_nDatas] = _T("차수번호");
	m_nDatas++;

	m_strGroup1[m_nDatas] = _T("연속");
	m_strGroup2[m_nDatas] = _T("차수번호");
	m_strGroup3[m_nDatas] = _T("SE");
	m_strGroup4[m_nDatas] = _T("PUR_NO");
	m_strGroup5[m_nDatas] = _T("차수번호");
	m_nDatas++;

	m_strGroup1[m_nDatas] = _T("연속");
	m_strGroup2[m_nDatas] = _T("구입접수번호");
	m_strGroup3[m_nDatas] = _T("SE");
	m_strGroup4[m_nDatas] = _T("CK1_NO");
	m_strGroup5[m_nDatas] = _T("구입접수번호");
	m_nDatas++;

	m_strGroup1[m_nDatas] = _T("연속");
	m_strGroup2[m_nDatas] = _T("기증접수번호");
	m_strGroup3[m_nDatas] = _T("SE");
	m_strGroup4[m_nDatas] = _T("CK2_NO");
	m_strGroup5[m_nDatas] = _T("기증접수번호");
	m_nDatas++;

	m_strGroup1[m_nDatas] = _T("연속");
	m_strGroup2[m_nDatas] = _T("제본차수번호");
	m_strGroup3[m_nDatas] = _T("SE");
	m_strGroup4[m_nDatas] = _T("BIN_NO");
	m_strGroup5[m_nDatas] = _T("제본차수번호");
	m_nDatas++;

	m_strGroup1[m_nDatas] = _T("공통");
	m_strGroup2[m_nDatas] = _T("등록번호");
	m_strGroup3[m_nDatas] = _T("CO");
	m_strGroup4[m_nDatas] = _T("REG_NO");
	m_strGroup5[m_nDatas] = _T("등록번호");
	m_nDatas++;

	m_strGroup1[m_nDatas] = _T("공통");
	//2018년 공공도서관 책이음서비스 확대구축 및 시스템개선		
	//KOL.RED.019-004 JY 연속간행물 기능 안정화
	//BEFORE===============================================================
	//m_strGroup2[m_nDatas] = _T("가원부번호");
	//AFTER================================================================
	m_strGroup2[m_nDatas] = _T("가등록번호");
	//=====================================================================
	m_strGroup3[m_nDatas] = _T("CO");
	m_strGroup4[m_nDatas] = _T("TRG_NO");
	m_strGroup5[m_nDatas] = _T("가원부번호");
	m_nDatas++;

	m_strGroup1[m_nDatas] = _T("공통");
	m_strGroup2[m_nDatas] = _T("이용자번호");
	m_strGroup3[m_nDatas] = _T("CO");
	m_strGroup4[m_nDatas] = _T("USR_NO");
	m_strGroup5[m_nDatas] = _T("이용자번호");
	m_nDatas++;

	m_strGroup1[m_nDatas] = _T("공통");
	m_strGroup2[m_nDatas] = _T("기증자번호");
	m_strGroup3[m_nDatas] = _T("CO");
	m_strGroup4[m_nDatas] = _T("DON_ID");
	m_strGroup5[m_nDatas] = _T("기증자번호");
	m_nDatas++;

	m_strGroup1[m_nDatas] = _T("공통");
	m_strGroup2[m_nDatas] = _T("제어번호");
	m_strGroup3[m_nDatas] = _T("CO");
	m_strGroup4[m_nDatas] = _T("CON_NO");
	m_strGroup5[m_nDatas] = _T("제어번호");
	m_nDatas++;

	m_strGroup1[m_nDatas] = _T("공통");
	m_strGroup2[m_nDatas] = _T("원부번호");
	m_strGroup3[m_nDatas] = _T("CO");
	m_strGroup4[m_nDatas] = _T("ACC_NO");
	m_strGroup5[m_nDatas] = _T("원부번호");
	m_nDatas++;

	m_nYearColumnWidth = 100 * 15;
}

CBO_CAT_LAST_NUMBER_1000::~CBO_CAT_LAST_NUMBER_1000()
{
}

BOOL CBO_CAT_LAST_NUMBER_1000::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CBO_CAT_LAST_NUMBER_1000::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_LAST_NUMBER_1000)
	DDX_Control(pDX, IDC_COMBO2, m_cGroup2);
	DDX_Control(pDX, IDC_COMBO1, m_cGroup1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_LAST_NUMBER_1000, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_LAST_NUMBER_1000)
	ON_BN_CLICKED(IDC_bRefresh, OnbRefresh)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_LAST_NUMBER_1000 message handlers

BOOL CBO_CAT_LAST_NUMBER_1000::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( InitESL_Mgr( _T("BO_CAT_LAST_NUMBER_1000") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : BO_CAT_LAST_NUMBER_1000") );
		return false;
	}
	
	InitComboBox();
	CESL_Grid *pGrid = (CESL_Grid *)FindControl(_T("CM_BO_CAT_LAST_NUMBER_1000"), _T("목록"));
	if (pGrid) {
		m_nYearColumnWidth = pGrid->GetColWidth(2, 0);
	}

	SetTimer(0, 100, NULL);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CBO_CAT_LAST_NUMBER_1000::InitComboBox(VOID)
{
	m_cGroup1.ResetContent();
	m_cGroup1.AddString(_T("단행"));
	m_cGroup1.AddString(_T("연속"));
	m_cGroup1.AddString(_T("공통"));

	m_cGroup1.SetCurSel(0);

	UpdateData(FALSE);

	InitComboBox2(_T("단행"));
	return 0;
}

INT CBO_CAT_LAST_NUMBER_1000::InitComboBox2(CString strGroup1)
{
	m_cGroup2.ResetContent();
	if (strGroup1 == _T("단행")) {
		m_cGroup2.AddString(_T("기증접수번호"));
		m_cGroup2.AddString(_T("그룹번호"));
		m_cGroup2.AddString(_T("차수번호"));
	}
	if (strGroup1 == _T("연속")) {
		m_cGroup2.AddString(_T("차수번호"));
		m_cGroup2.AddString(_T("구입접수번호"));
		m_cGroup2.AddString(_T("기증접수번호"));
		m_cGroup2.AddString(_T("제본차수번호"));
	}
	if (strGroup1 == _T("공통")) {
		m_cGroup2.AddString(_T("등록번호"));
		//2018년 공공도서관 책이음서비스 확대구축 및 시스템개선		
		//KOL.RED.019-004 JY 연속간행물 기능 안정화
		//BEFORE===============================================================
		//m_cGroup2.AddString(_T("가원부번호"));
		//AFTER================================================================
		m_cGroup2.AddString(_T("가등록번호"));
		//=====================================================================
		m_cGroup2.AddString(_T("이용자번호"));
		m_cGroup2.AddString(_T("기증자번호"));
		m_cGroup2.AddString(_T("제어번호"));
		m_cGroup2.AddString(_T("원부번호"));
	}
	m_cGroup2.SetCurSel(0);

	UpdateData(FALSE);
	return 0;
}

INT CBO_CAT_LAST_NUMBER_1000::RefreshData(VOID)
{
	CString strGroup1, strGroup2;
	UpdateData(TRUE);
	m_cGroup1.GetWindowText(strGroup1);
	m_cGroup2.GetWindowText(strGroup2);

	strGroup1.TrimLeft();			strGroup1.TrimRight();
	strGroup2.TrimLeft();			strGroup2.TrimRight();
	INT nCurSel = GetGroupIndex(strGroup1, strGroup2);

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_LAST_NUMBER_1000"));
	if (pDM == NULL) return -1;

	BOOL bDIVISION = FALSE;
	if (m_strGroup3[m_nCurSel] == _T("MO") && m_strGroup4[m_nCurSel] == _T("PUR_NO")) {
		bDIVISION = TRUE;
	}
	if (m_strGroup3[m_nCurSel] == _T("CO") && m_strGroup4[m_nCurSel] == _T("DON_ID")) {
		bDIVISION = TRUE;
	}
	if (m_strGroup3[m_nCurSel] == _T("CO") && m_strGroup4[m_nCurSel] == _T("CON_NO")) {
		bDIVISION = TRUE;
	}
	if (m_strGroup3[m_nCurSel] == _T("CO") && m_strGroup4[m_nCurSel] == _T("REG_NO")) {		
		bDIVISION = TRUE;
	}
	if (m_strGroup3[m_nCurSel] == _T("CO") && m_strGroup4[m_nCurSel] == _T("USR_NO")) {		
		bDIVISION = TRUE;
	}
	if (bDIVISION == TRUE) {
		INT nRows = pDM->GetRowCount();
		CString strTmp;
		for (INT i = 0; i < nRows; i++) {
			strTmp = pDM->GetCellData(_T("DIVISION_VALUE"), i);
			pDM->SetCellData(_T("기타"), strTmp, i);
		}
	}

	CString strOption;
	strOption.Format(_T("PUBLISH_FORM_CODE='%s' AND KIND_CODE='%s'"), m_strGroup3[m_nCurSel], GetKIND_CODE());
	pDM->RefreshDataManager(strOption);

	GridDisplay();
	CESL_Grid *pGrid = (CESL_Grid *)FindControl(_T("CM_BO_CAT_LAST_NUMBER_1000"), _T("목록"));
/* 수정 사항 : 공통(CO)->등록번호(REG_NO) 일 경우는 연도 컬럼을 보여주지 않는다.
 * 수정 날짜 : 2003.08.27
 * 수정 사람 : 최원종
 */
	if (m_strGroup3[m_nCurSel] == _T("CO") && ( m_strGroup4[m_nCurSel] == _T("REG_NO") || m_strGroup4[m_nCurSel] == _T("USR_NO")) ) {
		// 공통(CO)->등록번호(REG_NO) 일 경우는 연도 컬럼을 보여주지 않는다.
		if (pGrid) pGrid->SetColWidth(2, 0, 0);
	} else {
		if (pGrid) pGrid->SetColWidth(2, 0, m_nYearColumnWidth);
	}

	if (bDIVISION == TRUE) {
		INT nRows2 = pDM->GetRowCount();
		CString strTmp;
		for (INT i = 0; i < nRows2; i++) {
			strTmp = pDM->GetCellData(_T("DIVISION_VALUE"), i);
/* 수정 사항 : 마지막 번호 관리에서 기타에 Alias를 넣지 말고 Code로 나타내기 위해 주석 처리 +++
 * 수정 날짜 : 2003.08.20
 * 수정 사람 : 김홍수 
 */
			if (m_strGroup3[m_nCurSel] == _T("MO") && m_strGroup4[m_nCurSel] == _T("PUR_NO")) {
				m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("업무구분"), strTmp, strTmp);
			}
			/*
			if (m_strGroup3[m_nCurSel] == _T("CO") && m_strGroup4[m_nCurSel] == _T("REG_NO")) {
				m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("등록구분"), strTmp, strTmp);
			}
			if (m_strGroup3[m_nCurSel] == _T("CO") && m_strGroup4[m_nCurSel] == _T("CON_NO")) {
				m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("제어자료구분"), strTmp, strTmp);
			}
			*/
// 수정 사항 : 마지막 번호 관리에서 기타에 Alias를 넣지 말고 Code로 나타내기 위해 주석 처리 ---

			//=====================================================
			//2008.12.04 UPDATE BY PJW : 그리드에 직접데이터넣지않고 DM에 저장후 그리드 DISPLAY 한다.
// 			pGrid->SetTextMatrix(i+1, 4, strTmp);
			pDM->SetCellData(_T("기타"), strTmp,i);
			pGrid->Display();
			//=====================================================
		}
	}

	return 0;
}

INT CBO_CAT_LAST_NUMBER_1000::GridDisplay(VOID)
{
	CESL_Grid *pGrid = (CESL_Grid *)FindControl(_T("CM_BO_CAT_LAST_NUMBER_1000"), _T("목록"));
	if (pGrid) pGrid->Display();
	return 0;
}

INT CBO_CAT_LAST_NUMBER_1000::GetGroupIndex(CString strGroup1, CString strGroup2)
{
	for (INT i = 0; i < m_nDatas; i++) {
		if (m_strGroup1[i] == strGroup1 && m_strGroup2[i] == strGroup2) {
			m_nCurSel = i;
			return i;
		}
	}
	return -1;
}

VOID CBO_CAT_LAST_NUMBER_1000::OnbRefresh() 
{
	RefreshData();	
}

INT CBO_CAT_LAST_NUMBER_1000::OnInsert()
{
	CBO_CAT_LAST_NUMBER_2000 dlg(this);
	dlg.BUSINESS_MODE = BUSINESS_MODE;
	dlg.MODE = _T("입력");
	dlg.GROUP3 = m_strGroup3[m_nCurSel];
	dlg.GROUP4 = m_strGroup4[m_nCurSel];
	dlg.GROUP5 = m_strGroup5[m_nCurSel];
	if (dlg.DoModal() != IDOK) return 0;
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_LAST_NUMBER_1000"));
	if (pDM == NULL) return -1;
	CString strRecKey;
	pDM->MakeRecKey(strRecKey);

	INT ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}


	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strRecKey);
	pDM->AddDBFieldData(_T("PUBLISH_FORM_CODE"), _T("STRING"), m_strGroup3[m_nCurSel]);
	pDM->AddDBFieldData(_T("KIND_CODE"), _T("STRING"), GetKIND_CODE());
	pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE"));
	if (m_strGroup3[m_nCurSel] == _T("CO") && m_strGroup4[m_nCurSel] == _T("REG_NO")) {
		INT t = 0;
	} 
	// 2005.10.24 UPD BY PDW(Jr) //////////////////////////////////////////////
	// 이용자구분은 MAKE_YEAR를 사용하지 않는다 .
	///////////////////////////////////////////////////////////////////////////
	else if (m_strGroup3[m_nCurSel] == _T("CO") && m_strGroup4[m_nCurSel] == _T("USR_NO"));
	else {
		pDM->AddDBFieldData(_T("MAKE_YEAR"), _T("NUMERIC"), dlg.YEAR);
	}
	pDM->AddDBFieldData(_T("LAST_NUMBER"), _T("NUMERIC"), dlg.LAST_NUMBER);

	CString strQuery;

	strQuery.Format(_T("SELECT COUNT(*) FROM CO_LAST_NUMBER_TBL WHERE PUBLISH_FORM_CODE='%s' AND KIND_CODE='%s' AND MAKE_YEAR=%s AND MANAGE_CODE = UDF_MANAGE_CODE"), m_strGroup3[m_nCurSel], GetKIND_CODE(), dlg.YEAR);

	if (m_strGroup3[m_nCurSel] == _T("MO") && m_strGroup4[m_nCurSel] == _T("PUR_NO")) {
		pDM->AddDBFieldData(_T("DIVISION_VALUE"), _T("STRING"), dlg.DIVISION_VALUE);
		strQuery.Format(_T("SELECT COUNT(*) FROM CO_LAST_NUMBER_TBL WHERE PUBLISH_FORM_CODE='%s' AND KIND_CODE='%s' AND MAKE_YEAR=%s AND DIVISION_VALUE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), m_strGroup3[m_nCurSel], GetKIND_CODE(), dlg.YEAR, dlg.DIVISION_VALUE);
	}
	if (m_strGroup3[m_nCurSel] == _T("CO") && m_strGroup4[m_nCurSel] == _T("DON_ID")) {
		pDM->AddDBFieldData(_T("DIVISION_VALUE"), _T("STRING"), dlg.DIVISION_VALUE);
		strQuery.Format(_T("SELECT COUNT(*) FROM CO_LAST_NUMBER_TBL WHERE PUBLISH_FORM_CODE='%s' AND KIND_CODE='%s' AND MAKE_YEAR=%s AND DIVISION_VALUE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), m_strGroup3[m_nCurSel], GetKIND_CODE(), dlg.YEAR, dlg.DIVISION_VALUE);
	}
	if (m_strGroup3[m_nCurSel] == _T("CO") && m_strGroup4[m_nCurSel] == _T("CON_NO")) {
		pDM->AddDBFieldData(_T("DIVISION_VALUE"), _T("STRING"), dlg.DIVISION_VALUE);
		strQuery.Format(_T("SELECT COUNT(*) FROM CO_LAST_NUMBER_TBL WHERE PUBLISH_FORM_CODE='%s' AND KIND_CODE='%s' AND MAKE_YEAR=%s AND DIVISION_VALUE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), m_strGroup3[m_nCurSel], GetKIND_CODE(), dlg.YEAR, dlg.DIVISION_VALUE);
	}
	if (m_strGroup3[m_nCurSel] == _T("CO") && m_strGroup4[m_nCurSel] == _T("REG_NO")) {
		pDM->AddDBFieldData(_T("DIVISION_VALUE"), _T("STRING"), dlg.DIVISION_VALUE);
		strQuery.Format(_T("SELECT COUNT(*) FROM CO_LAST_NUMBER_TBL WHERE PUBLISH_FORM_CODE='%s' AND KIND_CODE='%s' AND DIVISION_VALUE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), m_strGroup3[m_nCurSel], GetKIND_CODE(), dlg.DIVISION_VALUE);
	}
	// 2005.10.24 UPD BY PDW(Jr) //////////////////////////////////////////////
	// 이용자구분추가.
	///////////////////////////////////////////////////////////////////////////
	if (m_strGroup3[m_nCurSel] == _T("CO") && m_strGroup4[m_nCurSel] == _T("USR_NO")) {
		pDM->AddDBFieldData(_T("DIVISION_VALUE"), _T("STRING"), dlg.YEAR);
		strQuery.Format(_T("SELECT COUNT(*) FROM CO_LAST_NUMBER_TBL WHERE PUBLISH_FORM_CODE='%s' AND KIND_CODE='%s' AND DIVISION_VALUE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), m_strGroup3[m_nCurSel], GetKIND_CODE(), dlg.YEAR);
	}

	CString result;
	ids = pDM->GetOneValueQuery(strQuery, result);	

	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("MakeInsertFrame를 확인하세요"));
		return -1;
	}

	result.TrimLeft();			result.TrimRight();
	if (result.GetLength() > 0) {
		if (_ttoi(result) > 0) {
			AfxMessageBox(_T("이미 있는 자료입니다. 중복 등록 할 수 없습니다."));
			pDM->InitDBFieldData();
			return 0;
		}
	}

	ids = pDM->MakeInsertFrame(_T("CO_LAST_NUMBER_TBL"));

	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("MakeInsertFrame를 확인하세요"));
		return -1;
	}
	
	ids = pDM->SendFrame();
	ids = pDM->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame를 확인하세요"));
		return -1;
	}

	if (ids < 0) {
		AfxMessageBox(_T("입력중 에러가 발생하였습니다."));
	} else {
		RefreshData();
		AfxMessageBox(_T("입력되었습니다."));
	}

	return 0;
}

INT CBO_CAT_LAST_NUMBER_1000::OnModify()
{
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_LAST_NUMBER_1000"));
	if (pDM == NULL) return -1;
	if (pDM->GetRowCount() == 0) {
		AfxMessageBox(_T("데이타가 없습니다."));
		return -1;
	}

	CESL_Grid *pGrid = (CESL_Grid *)FindControl(_T("CM_BO_CAT_LAST_NUMBER_1000"), _T("목록"));
	if (pGrid == NULL) return -1;

	CBO_CAT_LAST_NUMBER_2000 dlg(this);
	dlg.BUSINESS_MODE = BUSINESS_MODE;
	dlg.m_nStartRowIdx = pGrid->GetRow() -1;
	dlg.MODE = _T("수정");
	dlg.GROUP3 = m_strGroup3[m_nCurSel];
	dlg.GROUP4 = m_strGroup4[m_nCurSel];
	dlg.GROUP5 = m_strGroup5[m_nCurSel];

	dlg.DoModal();

	RefreshData();
	return 0;
}

INT CBO_CAT_LAST_NUMBER_1000::OnDelete()
{
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_LAST_NUMBER_1000"));
	if (pDM == NULL) return -1;
	if (pDM->GetRowCount() == 0) return -1;

	CESL_Grid *pGrid = (CESL_Grid *)FindControl(_T("CM_BO_CAT_LAST_NUMBER_1000"), _T("목록"));
	if (pGrid == NULL) return -1;
	CString str;
	INT nRows = pDM->GetRowCount();
	INT i;
	INT nCount = 0;
	for (i = 0; i < nRows; i++) {
		str = pGrid->GetTextMatrix(i+1, 1);
		if (str.GetLength() > 0) nCount++;
	}
	if (nCount == 0) {
		AfxMessageBox(_T("먼저 삭제하실 자료를 선정하여 주십시오."));
		return 0;
	}
	str.Format(_T("선정하신 %d건의 자료를 정말 삭제하시겠습니까?"), nCount);
	if (AfxMessageBox(str , MB_YESNO) != IDYES) return 0;

	INT ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}

	CString strRecKey, strQuery;
	for (i = 0; i < nRows; i++) {
		str = pGrid->GetTextMatrix(i+1, 1);
		if (str.GetLength() == 0) continue;
		strRecKey = pDM->GetCellData(_T("REC_KEY"), i);
		strQuery.Format(_T("DELETE FROM CO_LAST_NUMBER_TBL WHERE REC_KEY=%s;"), strRecKey);
		pDM->AddFrame(strQuery);
	}
	ids = pDM->SendFrame();
	pDM->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("삭제중 에러가 발생하였습니다."));
	} else {
		RefreshData();
		AfxMessageBox(_T("삭제되었습니다."));
	}
	return 0;
}

VOID CBO_CAT_LAST_NUMBER_1000::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0) {
		KillTimer(0);
		RefreshData();
	}
	CDialog::OnTimer(nIDEvent);
}

VOID CBO_CAT_LAST_NUMBER_1000::OnSelchangeCombo1() 
{
	UpdateData(TRUE);
	CString str;
	m_cGroup1.GetWindowText(str);
	str.TrimLeft();			str.TrimRight();
	InitComboBox2(str);

	RefreshData();
}

VOID CBO_CAT_LAST_NUMBER_1000::RearrangeControl(VOID)
{
	CRect rDlg, rControl;
	CWnd *pWnd;

	if (GetSafeHwnd() == NULL) return;
	GetClientRect(rDlg);

	pWnd = GetDlgItem(IDC_gLIST);
	if (pWnd) {
		rControl = rDlg;
		rControl.top = rControl.top + 35;
		rControl.bottom = rControl.bottom-2;
		rControl.left = rControl.left + 2;
		rControl.right = rControl.right;

		pWnd->MoveWindow(rControl);
	}
	return;
}

VOID CBO_CAT_LAST_NUMBER_1000::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

	RearrangeControl();
	
}

VOID CBO_CAT_LAST_NUMBER_1000::OnSelchangeCombo2() 
{
	RefreshData();
}


BEGIN_EVENTSINK_MAP(CBO_CAT_LAST_NUMBER_1000, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_LAST_NUMBER_1000)
	ON_EVENT(CBO_CAT_LAST_NUMBER_1000, IDC_gLIST, -601 /* DblClick */, OnDblClickgLIST, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_CAT_LAST_NUMBER_1000::OnDblClickgLIST() 
{
	//=====================================================
	//2008.12.04 ADD BY PJW : 그리드의 헤더를 더블클릭하였다면 리턴한다.
	CESL_Grid *pGrid = (CESL_Grid *)FindControl(_T("CM_BO_CAT_LAST_NUMBER_1000"), _T("목록"));
	if(pGrid->GetMouseRow() == 0) 
	{
		return ;
	}
	//=====================================================

	OnModify();
}

CString CBO_CAT_LAST_NUMBER_1000::GetKIND_CODE()
{
EFS_BEGIN
	if (BUSINESS_MODE == _T("UP")) {		// KOLAS II UP 인경우
		return m_strGroup4[m_nCurSel];
	} else {				// KOLAS II LIGHT 인경우
		return m_strGroup5[m_nCurSel];
	}
EFS_END
return _T("");
}
HBRUSH CBO_CAT_LAST_NUMBER_1000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
