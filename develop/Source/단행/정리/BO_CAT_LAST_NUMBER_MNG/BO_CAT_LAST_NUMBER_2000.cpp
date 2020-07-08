// BO_CAT_LAST_NUMBER_2000.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_LAST_NUMBER_2000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_LAST_NUMBER_2000 dialog


CBO_CAT_LAST_NUMBER_2000::CBO_CAT_LAST_NUMBER_2000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_LAST_NUMBER_2000)
	YEAR = _T("");
	LAST_NUMBER = _T("");
	//}}AFX_DATA_INIT

	MODE = _T("입력");
	GROUP3 = _T("");
	GROUP4 = _T("");
	m_nStartRowIdx = -1;
	m_bGoFirst = FALSE;
}

CBO_CAT_LAST_NUMBER_2000::~CBO_CAT_LAST_NUMBER_2000()
{
}

VOID CBO_CAT_LAST_NUMBER_2000::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_LAST_NUMBER_2000)
	DDX_Control(pDX, IDC_COMBO1, m_cCombo1);
	DDX_Text(pDX, IDC_eYEAR, YEAR);
	DDX_Text(pDX, IDC_eNUMBER, LAST_NUMBER);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_LAST_NUMBER_2000, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_LAST_NUMBER_2000)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_LAST_NUMBER_2000 message handlers

BOOL CBO_CAT_LAST_NUMBER_2000::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( InitESL_Mgr( _T("BO_CAT_LAST_NUMBER_2000") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : BO_CAT_LAST_NUMBER_2000") );
		return false;
	}

	CWnd *pWndIDOK = GetDlgItem(IDOK);
	if (MODE == _T("입력")) {
		SetWindowText(_T("입력"));
		if (pWndIDOK) pWndIDOK->SetWindowText(_T("입력"));
		CWnd *pWnd1 = GetDlgItem(IDC_bPREV);
		if (pWnd1) pWnd1->ShowWindow(FALSE);
		CWnd *pWnd2 = GetDlgItem(IDC_bNEXT);
		if (pWnd2) pWnd2->ShowWindow(FALSE);
	} else {
		SetWindowText(_T("수정"));
		if (pWndIDOK) pWndIDOK->SetWindowText(_T("저장"));
	}
	
	InitComboBox();
	ShowControls();
	if (MODE == _T("수정")) {
		DisplayModifyData(0);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CBO_CAT_LAST_NUMBER_2000::InitComboBox(VOID)
{
	m_cCombo1.ResetContent();
	m_cCombo1.AddString(_T("GR"));
	m_cCombo1.AddString(_T("PN"));
	m_cCombo1.SetCurSel(0);

	UpdateData(FALSE);
	return 0;
}

VOID CBO_CAT_LAST_NUMBER_2000::OnOK() 
{	
	UpdateData(TRUE);

	DIVISION_VALUE = _T("");
	if (GROUP3 == _T("CO") && GROUP4 == _T("DON_ID")) {
		m_cCombo1.GetWindowText(DIVISION_VALUE);
	}
	if (GROUP3 == _T("MO") && GROUP4 == _T("PUR_NO")) {
		CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)FindControl(_T("CM_BO_CAT_LAST_NUMBER_2000"), _T("업무구분"));
		if (pCMB) pCMB->GetData(DIVISION_VALUE);
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("CON_NO")) {
		CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)FindControl(_T("CM_BO_CAT_LAST_NUMBER_2000"), _T("제어자료구분"));
		if (pCMB) pCMB->GetData(DIVISION_VALUE);
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("REG_NO")) {
		CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)FindControl(_T("CM_BO_CAT_LAST_NUMBER_2000"), _T("등록구분"));
		if (pCMB) pCMB->GetData(DIVISION_VALUE);
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("USR_NO")) {
		CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)FindControl(_T("CM_BO_CAT_LAST_NUMBER_2000"), _T("이용자번호구분"));
		if (pCMB) pCMB->GetData(DIVISION_VALUE);
	}
	YEAR.TrimLeft();			YEAR.TrimRight();
	if (GROUP3 == _T("CO") && GROUP4 == _T("REG_NO")) {
		;
	} else {
		if (YEAR.GetLength() == 0) {
			AfxMessageBox(_T("연도를 입력하여 주십시오."));
			return;
		}
	}
	LAST_NUMBER.TrimLeft();		LAST_NUMBER.TrimRight();
	if (LAST_NUMBER.GetLength() == 0) {
		AfxMessageBox(_T("마지막 번호를 입력하여 주십시오."));
		return;
	}
	if (MODE == _T("입력")) {
		CDialog::OnOK();
	} else {
		SAVE(m_index);
	}
}

VOID CBO_CAT_LAST_NUMBER_2000::OnCancel() 
{
	CDialog::OnCancel();
}

VOID CBO_CAT_LAST_NUMBER_2000::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
}

VOID CBO_CAT_LAST_NUMBER_2000::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
}

VOID CBO_CAT_LAST_NUMBER_2000::RearrangeControls(VOID)
{
	CWnd *pWnd = NULL;

	CRect rect;
	rect.top = 100;
	rect.bottom = rect.top + 30;
	rect.left = 200;
	rect.right = rect.left + 50;

	if (GROUP3 == _T("MO") && GROUP4 == _T("PUR_NO")) {
		pWnd = GetDlgItem(IDC_COMBO4);
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("DON_ID")) {
		pWnd = GetDlgItem(IDC_COMBO1);
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("CON_NO")) {
		pWnd = GetDlgItem(IDC_COMBO2);
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("REG_NO")) {
		pWnd = GetDlgItem(IDC_COMBO3);
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("USR_NO")) {
		pWnd = GetDlgItem(IDC_COMBO5);
	}

	if (pWnd) pWnd->MoveWindow(&rect);

	return;
}

VOID CBO_CAT_LAST_NUMBER_2000::SAVE(INT nRow)
{
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_LAST_NUMBER_1000"));
	if (pDM == NULL) return;

	CString strRecKey = pDM->GetCellData(_T("REC_KEY"), nRow);

	INT ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return;
	}


	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strRecKey);
	pDM->AddDBFieldData(_T("PUBLISH_FORM_CODE"), _T("STRING"), GROUP3);
	if (BUSINESS_MODE == _T("UP")) {			// KOLAS II UP 인경우
		pDM->AddDBFieldData(_T("KIND_CODE"), _T("STRING"), GROUP4);
	} else {								// KOLAS II LIGHT 인경우
		pDM->AddDBFieldData(_T("KIND_CODE"), _T("STRING"), GROUP5);
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("REG_NO")) {
		INT t = 0;
	} else {
		// 2005.10.24 UPD BY PDW(Jr) //////////////////////////////////////
		// 이용자구분번호 예외처리
		// 이용자구분번호는 MAKE_YEAR를 사용하지 않는다 
		///////////////////////////////////////////////////////////////////
		/* 수정전
		pDM->AddDBFieldData(_T("MAKE_YEAR"), _T("NUMERIC"), YEAR);
		*/
		if (GROUP3 == _T("CO") && GROUP4 == _T("USR_NO"))
			pDM->AddDBFieldData(_T("MAKE_YEAR"), _T("NUMERIC"), _T(""));
		else
			pDM->AddDBFieldData(_T("MAKE_YEAR"), _T("NUMERIC"), YEAR);
		///////////////////////////////////////////////////////////////////
	}
	pDM->AddDBFieldData(_T("LAST_NUMBER"), _T("NUMERIC"), LAST_NUMBER);

	if (GROUP3 == _T("MO") && GROUP4 == _T("PUR_NO")) {
		pDM->AddDBFieldData(_T("DIVISION_VALUE"), _T("STRING"), DIVISION_VALUE);
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("DON_ID")) {
		pDM->AddDBFieldData(_T("DIVISION_VALUE"), _T("STRING"), DIVISION_VALUE);
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("CON_NO")) {
		pDM->AddDBFieldData(_T("DIVISION_VALUE"), _T("STRING"), DIVISION_VALUE);
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("REG_NO")) {
		pDM->AddDBFieldData(_T("DIVISION_VALUE"), _T("STRING"), DIVISION_VALUE);
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("USR_NO")) {
		// 2005.10.24 UPD BY PDW(Jr) //////////////////////////////////////
		// 이용자 구분번호 수정
		// 입력을 에디트 박스로 변경
		///////////////////////////////////////////////////////////////////
		/*												DIVISION_VALUE*/	
		pDM->AddDBFieldData(_T("DIVISION_VALUE"), _T("STRING"), YEAR);
	}
	pDM->MakeUpdateFrame(_T("CO_LAST_NUMBER_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey);
	
	ids = pDM->SendFrame();
	pDM->EndFrame();
	if (ids < 0) {
		AfxMessageBox(_T("수정중 에러가 발생하였습니다."));
	} else {
		AfxMessageBox(_T("수정되었습니다."));
	}
	return;
}

INT CBO_CAT_LAST_NUMBER_2000::ShowControls(VOID)
{
	CWnd *pWnd = NULL;
	if (GROUP3 == _T("MO") && GROUP4 == _T("PUR_NO")) {
		pWnd = GetDlgItem(IDC_COMBO4);
		if (pWnd) {
			pWnd->ShowWindow(TRUE);
		}
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("DON_ID")) {
		CWnd *pWnd1 = GetDlgItem(IDC_COMBO1);
		if (pWnd1) pWnd1->ShowWindow(TRUE);
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("CON_NO")) {
		CWnd *pWnd2 = GetDlgItem(IDC_COMBO2);
		if (pWnd2) pWnd2->ShowWindow(TRUE);
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("REG_NO")) {
		CWnd *pWnd3 = GetDlgItem(IDC_COMBO3);
		if (pWnd3) pWnd3->ShowWindow(TRUE);

		CWnd *pWnd;
		pWnd = GetDlgItem(IDC_STATIC_YEAR);
		if (pWnd) pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_eYEAR);
		if (pWnd) pWnd->ShowWindow(FALSE);
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("USR_NO")) {
		CWnd *pWnd;
		pWnd = GetDlgItem(IDC_STATIC_YEAR);
		if (pWnd) 
		{
			pWnd->ShowWindow(TRUE);
			pWnd->SetWindowText(_T("코드:"));		// 2005.10.24 ADD BY PDW(Jr)
		}
		CWnd *pWnd2 = GetDlgItem(IDC_COMBO5);
		if (pWnd2) pWnd2->ShowWindow(FALSE);
	}
	return 0;
}

INT CBO_CAT_LAST_NUMBER_2000::ShowData(INT nRowIdx)
{
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_LAST_NUMBER_1000"));
	if (pDM == NULL) return -1;
	DIVISION_VALUE = pDM->GetCellData(_T("DIVISION_VALUE"), nRowIdx);
	YEAR = pDM->GetCellData(_T("MAKE_YEAR"), nRowIdx);
	// 2005.10.24 ADD BY PDW(Jr) //////////////////////////////////////
	// 이용자구분번호 예외처리 
	///////////////////////////////////////////////////////////////////
	if (GROUP3 == _T("CO") && GROUP4 == _T("USR_NO"))
		YEAR = DIVISION_VALUE;
	///////////////////////////////////////////////////////////////////

	LAST_NUMBER = pDM->GetCellData(_T("LAST_NUMBER"), nRowIdx);
	
	CWnd *pWnd = NULL;
	if (GROUP3 == _T("MO") && GROUP4 == _T("PUR_NO")) {
		CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)FindControl(_T("CM_BO_CAT_LAST_NUMBER_2000"), _T("업무구분"));
		if (pCMB) {
			pCMB->SelectItem(DIVISION_VALUE);
		}
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("DON_ID")) {
		m_cCombo1.SelectString(0, DIVISION_VALUE);
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("CON_NO")) {
		CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)FindControl(_T("CM_BO_CAT_LAST_NUMBER_2000"), _T("제어자료구분"));
		if (pCMB) {
			pCMB->SelectItem(DIVISION_VALUE);
		}
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("REG_NO")) {
		CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)FindControl(_T("CM_BO_CAT_LAST_NUMBER_2000"), _T("등록구분"));
		if (pCMB) {
			pCMB->SelectItem(DIVISION_VALUE);
		}
	}
	if (GROUP3 == _T("CO") && GROUP4 == _T("USR_NO")) {
		// 2005.10.24 REM BY PDW(Jr) //////////////////////////////////////////////////
		// 이용자구분번호는 EDIT BOX 입력을 받는다.
		///////////////////////////////////////////////////////////////////////////////
		/* 
		CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)FindControl(_T("CM_BO_CAT_LAST_NUMBER_2000"), _T("이용자번호구분"));
		if (pCMB) {
			pCMB->SelectItem(DIVISION_VALUE);
		}
		*/
	}

	UpdateData(FALSE);
	return 0;
}

VOID CBO_CAT_LAST_NUMBER_2000::OnbPREV() 
{
	DisplayModifyData(-1);
}

VOID CBO_CAT_LAST_NUMBER_2000::OnbNEXT() 
{
	DisplayModifyData(1);
}

VOID CBO_CAT_LAST_NUMBER_2000::DisplayModifyData( INT nFlag )
{
	CESL_Grid *pGrid = (CESL_Grid *)FindControl(_T("CM_BO_CAT_LAST_NUMBER_1000"), _T("목록"));
	if (pGrid == NULL) return;
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_LAST_NUMBER_1000"));
	if (pDM == NULL) return;
	INT nSelectedCount;
	INT i, nRows;

	if (nFlag == 0) {
		SelectMakeList( _T("CM_BO_CAT_LAST_NUMBER_1000"), _T("목록") );
		nSelectedCount = pGrid->m_arraySelect.GetCount();
		if (nSelectedCount == 0) {
			nRows = pDM->GetRowCount();
			for (i = 0; i < nRows; i++) pGrid->m_arraySelect.AddTail(i);
			pGrid->m_arrayPos = pGrid->m_arraySelect.GetHeadPosition();
			nSelectedCount = nRows;
		}
		m_index = SelectGetHeadPosition( _T("CM_BO_CAT_LAST_NUMBER_1000"), _T("목록") );
		if (m_nStartRowIdx >= 0) {
			m_bGoFirst = TRUE;
			CString str;
			str = pGrid->GetTextMatrix(m_nStartRowIdx+1, 1);
			if (str.GetLength() > 0) {
				if (m_index == m_nStartRowIdx) {
					m_index = m_nStartRowIdx;
					m_bGoFirst = FALSE;
				} else {
					for (INT i = 0; i < nSelectedCount; i++) {
						m_index = SelectGetNext( _T("CM_BO_CAT_LAST_NUMBER_1000"), _T("목록") );
						if (m_index == m_nStartRowIdx) break;
					}
				}
			} else {
				m_index = m_nStartRowIdx;
				if (nSelectedCount == 0) m_bGoFirst = FALSE;
			}
		}
	}
	if (nFlag == -1) {
		m_index = SelectGetPrev( _T("CM_BO_CAT_LAST_NUMBER_1000"), _T("목록") );

		if( -1 == m_index )
		{
			m_index = SelectGetHeadPosition( _T("CM_BO_CAT_LAST_NUMBER_1000"), _T("목록") );
			AfxMessageBox( _T(" 맨 처음 데이타입니다.") );
			return;
		}
	}

	if (nFlag == 1) {
		if (m_bGoFirst == TRUE) {
			m_index = SelectGetHeadPosition( _T("CM_BO_CAT_LAST_NUMBER_1000"), _T("목록") );
			m_bGoFirst = FALSE;
		} else {
			m_index = SelectGetNext( _T("CM_BO_CAT_LAST_NUMBER_1000"), _T("목록") );
		}
		if( -1 == m_index )
		{
			m_index = SelectGetTailPosition( _T("CM_BO_CAT_LAST_NUMBER_1000"), _T("목록") );
			AfxMessageBox( _T("마지막 데이타입니다.") );
			return;
		}
	}
	
	ShowData( m_index );
	if (m_index >= 0) {
		pGrid->SetReverse(m_index);
	}
}

HBRUSH CBO_CAT_LAST_NUMBER_2000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
