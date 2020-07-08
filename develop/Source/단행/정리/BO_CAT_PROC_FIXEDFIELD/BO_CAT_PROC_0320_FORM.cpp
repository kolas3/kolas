// BO_CAT_PROC_0320_FORM.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0320_FORM.h"
#include "BO_CAT_PROC_0320_FORM_01.h"
#include "BO_CAT_PROC_0320_FORM_02.h"
#include "BO_CAT_PROC_0320_FORM_03.h"
#include "BO_CAT_PROC_0320_FORM_04.h"
#include "BO_CAT_PROC_0320_FORM_05.h"
#include "BO_CAT_PROC_0320_FORM_06.h"
#include "BO_CAT_PROC_0320_FORM_07.h"
#include "BO_CAT_PROC_0320_FORM_08.h"
#include "BO_CAT_PROC_0320_FORM_09.h"
#include "BO_CAT_PROC_0320_FORM_10.h"
#include "BO_CAT_PROC_0320_FORM_11.h"
#include "BO_CAT_PROC_0320_FORM_12.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320_FORM dialog


CBO_CAT_PROC_0320_FORM::CBO_CAT_PROC_0320_FORM(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0320_FORM)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	/*m_pPage1 = NULL;
	m_pPage2 = NULL;
	m_pPage3 = NULL;
	m_pPage4 = NULL;
	m_pPage5 = NULL;
	m_pPage6 = NULL;
	m_pPage7 = NULL;
	m_pPage8 = NULL;
	m_pPage9 = NULL;
	m_pPage10 = NULL;
	m_pPage11 = NULL;
	m_pPage12 = NULL;
	*/

	m_pForm = NULL;

}

CBO_CAT_PROC_0320_FORM::~CBO_CAT_PROC_0320_FORM()
{
}

BOOL CBO_CAT_PROC_0320_FORM::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0320_FORM::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0320_FORM)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0320_FORM, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0320_FORM)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_cMAT_CATEGORY, OnSelchangecMATCATEGORY)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320_FORM message handlers
BOOL CBO_CAT_PROC_0320_FORM::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cMAT_CATEGORY);
	if (pComboBox)
	{
		if ( m_sType == _T("BK") ) {
			pComboBox->AddString(_T("마이크로형태자료"));
			pComboBox->AddString(_T("인쇄자료"));
			pComboBox->AddString(_T("형태를구분할수없는자료"));
		}
		else {
			pComboBox->AddString(_T("지도자료"));
			pComboBox->AddString(_T("구체"));
			pComboBox->AddString(_T("평면영사자료"));
			pComboBox->AddString(_T("마이크로형태자료"));
			pComboBox->AddString(_T("평면비영사자료"));
			pComboBox->AddString(_T("영화"));
			pComboBox->AddString(_T("녹음자료"));
			pComboBox->AddString(_T("인쇄자료"));
			pComboBox->AddString(_T("비디오녹화자료"));
			pComboBox->AddString(_T("형태를구분할수없는자료"));
			pComboBox->AddString(_T("컴퓨터파일"));
		}

		pComboBox->SetCurSel(0);
	}

	CString sCategory = m_sReceive007.Left(1);
	TCHAR cCategory;
	if ( sCategory == _T("") ) {
		AfxMessageBox(_T("일치하는 자료범주가 없습니다.") );
		//cCategory = a;
	}
	else {
		cCategory = sCategory.GetAt(0);
	}
	

	if ( m_sType == _T("BK") ) {
		if ( sCategory == _T("") ) {
			CreateScreen('h');
			return TRUE;
		}
		else if ( sCategory == _T("t") ) {
			pComboBox->SetCurSel(1);
			//CreateScreen( cCategory );	
		}
		else if ( sCategory == _T("z") ) {
			pComboBox->SetCurSel(2);
			//CreateScreen( cCategory );
		}
		CreateScreen( cCategory );
	}
	else {		//TX가 아닐경우
		if ( sCategory == _T("") ) {
			//AfxMessageBox( _T("일치하는 자료범주가 없습니다.") );
			CreateScreen( 'a' );
			return TRUE;
		}
		else if ( sCategory == _T("d") ) {
			pComboBox->SetCurSel(1);
			//CreateScreen( cCategory );
		}
		else if ( sCategory == _T("g") ) {
			pComboBox->SetCurSel(2);
			//CreateScreen( cCategory );
		}
		else if ( sCategory == _T("h") ) {
			pComboBox->SetCurSel(3);		
		}
		else if ( sCategory == _T("k") ) {
			pComboBox->SetCurSel(4);		
		}
		else if ( sCategory == _T("m") ) {
			pComboBox->SetCurSel(5);		
		}
		else if ( sCategory == _T("s") ) {
			pComboBox->SetCurSel(6);		
		}
		else if ( sCategory == _T("t") ) {
			pComboBox->SetCurSel(7);		
		}
		else if ( sCategory == _T("v") ) {
			pComboBox->SetCurSel(8);		
		}
		else if ( sCategory == _T("z") ) {
			pComboBox->SetCurSel(9);		
		}
		else if ( sCategory == _T("c") ) {
			pComboBox->SetCurSel(10);		
		}
		CreateScreen( cCategory );
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0320_FORM::OnOK() 
{
EFS_BEGIN

	if (m_pForm == NULL) return;
	CESL_ControlMgr *pCM;
	INT iCh;
	pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_01"));
	iCh = 1;
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_02"));
		iCh = 2;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_03"));
		iCh = 3;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_04"));
		iCh = 4;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_05"));
		iCh = 5;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_06"));
		iCh = 6;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_07"));
		iCh = 7;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_08"));
		iCh = 8;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_09"));
		iCh = 9;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_10"));
		iCh = 10;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_11"));
		iCh = 11;
	}
	if (pCM == NULL) {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_12"));
		iCh = 12;
	}

	CString sMessage;	//필수항목 메세지
	s007 = _T("");
	CString sItemData;
	if ( iCh == 1 ) {		
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_01"), _T("자료범주표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", 자료범주표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_01"), _T("자료식별표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 자료식별표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_01"), _T("원본상태"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_01"), _T("색채"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 색채");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_01"), _T("물리적매체"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 물리적매체");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_01"), _T("복제형태"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 복제형태");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_01"), _T("복제내용"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 복제내용");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_01"), _T("감광상태"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 감광상태");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
	}
	else if ( iCh == 2 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_02"), _T("자료범주표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", 자료범주표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_02"), _T("자료식별표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 자료식별표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_02"), _T("원본상태"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_02"), _T("색채"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 색채");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_02"), _T("물리적매체"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 물리적매체");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_02"), _T("복제형태"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 복제형태");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
	}
	else if ( iCh == 3 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("자료범주표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", 자료범주표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("자료식별표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 자료식별표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("원본상태"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("색채"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 색채");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("감광유제"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 감광유제");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("매체의음향수록"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 매체의음향수록");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("음향수록매체"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 음향수록매체");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("필름의폭/크기"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 필름의폭/크기");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_03"), _T("틀의재질"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 틀의재질");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;		
	}
	else if ( iCh == 4 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("자료범주표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", 자료범주표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("자료식별표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 자료식별표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("원본상태"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("감광상태"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 감광상태");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("규격"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 규격");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("축소비율범위"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 축소비율범위");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("축소비율"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 축소비율");
		}
		if ( sItemData == _T("") ) sItemData = _T("   ");
		if ( sItemData.GetLength() < 3 ) {
			while ( sItemData.GetLength() < 3 ) {
				sItemData = sItemData + _T(" ");
			}
		}
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("색채"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 색채");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("감광유제"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 감광유제");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("생성과정"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 생성과정");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_04"), _T("필름의기본재료"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 필름의기본재료");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
	}
	else if ( iCh == 5 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_05"), _T("자료범주표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", 자료범주표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_05"), _T("자료식별표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 자료식별표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_05"), _T("원본상태"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_05"), _T("색채"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 색채");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_05"), _T("기본재료"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 기본재료");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_05"), _T("틀의재질"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 틀의재질");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;		
	}
	else if ( iCh == 6 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("자료범주표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", 자료범주표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("자료식별표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 자료식별표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("원본상태"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("색채"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 색채");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("스크린형태"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 스크린형태");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("매체의음향수록"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 매체의음향수록");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("음향수록매체"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 음향수록매체");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("필름의폭"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 필름의폭");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("음향의재생상태"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 음향의재생상태");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("제작요소"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("감광상태"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("제작상태"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("필름의기본재료"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("색깔의범주"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("천연색인화유형"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("변질상태"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("완전성"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), _T("필름의검사일자"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T("      ");
		s007 = s007 + sItemData;
	}
	else if ( iCh == 7 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("자료범주표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", 자료범주표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("자료식별표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 자료식별표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("원본상태"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("속도"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 속도");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("녹음재생형태"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 녹음재생형태");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("음구의폭/높이"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 음구의폭/높이");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("크기"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 크기");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("테이프의폭"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 테이프의폭");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("테이프의구성"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 테이프의구성");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("녹음기법"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("재질"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("편집유형"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("특수녹음방법"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), _T("테이프의종류"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;		
	}
	else if ( iCh == 8 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_08"), _T("자료범주표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", 자료범주표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_08"), _T("자료식별표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 자료식별표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;		
	}
	else if ( iCh == 9 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("자료범주표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", 자료범주표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("자료식별표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 자료식별표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("원본상태"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("색채"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 색채");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("비디오녹화형식"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 비디오녹화형식");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("매체의음향수록"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 매체의음향수록");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("음향수록매체"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 음향수록매체");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("크기"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 크기");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_09"), _T("음향의재생상태"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 음향의재생상태");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
	}
	else if ( iCh == 10 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_10"), _T("자료범주표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", 자료범주표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_10"), _T("자료식별표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 자료식별표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;		
	}
	else if ( iCh == 11 ) {
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("자료범주표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = _T(", 자료범주표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("자료식별표시"), sItemData );
		if ( sItemData == _T("") ) {
			sMessage = sMessage + _T(", 자료식별표시");
		}
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("색채"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("규격"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("음향"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("해상도"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T("   ");
		if ( sItemData.GetLength() < 3 ) {
			while ( sItemData.GetLength() < 3 ) {
				sItemData = sItemData + _T(" ");
			}
		}
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("파일포맷"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("품질보증"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("이전의정보원"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("압축수준"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;

		sItemData = _T("");
		m_pForm->GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), _T("재포맷의품질"), sItemData );
		if ( sItemData == _T("") ) sItemData = _T(" ");
		s007 = s007 + sItemData;
	}

	//메세지 보여주기
	if ( sMessage.GetLength() > 3 ) {
		sMessage = sMessage.Mid(2) + _T("을(를) 입력해야 합니다.");
		AfxMessageBox( sMessage );
		return;
	}

	CDialog::OnOK();

EFS_END
}

INT CBO_CAT_PROC_0320_FORM::CreateScreen(TCHAR tchCatagory)
{
EFS_BEGIN

	if (tchCatagory == 0) return -1;

	CRect rect;
	GetClientRect(rect);

	rect.top	+= 50; 
	rect.left	+= 5;
	rect.bottom -= 50; 
	rect.right	-= 5;


	if (m_pForm != NULL)
	{
		delete m_pForm;
		m_pForm = NULL;
	}

	switch(tchCatagory)
	{
		case 'a':
			m_pForm = new CBO_CAT_PROC_0320_FORM_01(this, m_sReceive007);
			//((CBO_CAT_PROC_0320_FORM_01*)m_pForm)->
			((CBO_CAT_PROC_0320_FORM_01*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'd':
            m_pForm = new CBO_CAT_PROC_0320_FORM_02(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_02*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
            break;
        case 'g':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_03(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_03*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
            break;
        case 'h':
			m_pForm = new CBO_CAT_PROC_0320_FORM_04(this, m_sReceive007);
	        //m_pForm = new CBO_CAT_PROC_0320_FORM_04(this);
            ((CBO_CAT_PROC_0320_FORM_04*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'k':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_05(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_05*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'm':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_06(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_06*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 's':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_07(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_07*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 't':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_08(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_08*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'v':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_09(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_09*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'z':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_10(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_10*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'c':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_11(this, m_sReceive007);
            ((CBO_CAT_PROC_0320_FORM_11*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;

		default:
			m_pForm = new CBO_CAT_PROC_0320_FORM_12(this, m_sReceive007);
			((CBO_CAT_PROC_0320_FORM_12*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();

			break;
	}

	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_PROC_0320_FORM::OnSelchangecMATCATEGORY() 
{
EFS_BEGIN

	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cMAT_CATEGORY);
	if (pComboBox)
	{
		INT nSel = pComboBox->GetCurSel();
		if (nSel != CB_ERR)
		{
			CString strBuffer;
			pComboBox->GetLBText(nSel, strBuffer);
			if (!strBuffer.IsEmpty())
			{
				TCHAR cForm;
				if ( strBuffer == _T("지도자료") ) {
					cForm = 'a';
				}
				else if ( strBuffer == _T("구체") ) {
					cForm = 'd';
				}
				else if ( strBuffer == _T("평면영사자료") ) {
					cForm = 'g';
				}		
				else if ( strBuffer == _T("마이크로형태자료") ) {
					cForm = 'h';
				}
				else if ( strBuffer == _T("평면비영사자료") ) {
					cForm = 'k';
				}
				else if ( strBuffer == _T("영화") ) {
					cForm = 'm';
				}
				else if ( strBuffer == _T("녹음자료") ) {
					cForm = 's';
				}
				else if ( strBuffer == _T("인쇄자료") ) {
					cForm = 't';
				}
				else if ( strBuffer == _T("비디오녹화자료") ) {
					cForm = 'v';
				}
				else if ( strBuffer == _T("형태를구분할수없는자료") ) {
					cForm = 'z';
				}
				else if ( strBuffer == _T("컴퓨터파일") ) {
					cForm = 'c';
				}
				else if ( strBuffer == _T("고서") ) {
					cForm = 'o';
				}
				CreateScreen(cForm);
			}
		}
	}

EFS_END
}

VOID CBO_CAT_PROC_0320_FORM::OnDestroy() 
{
EFS_BEGIN

	CDialog::OnDestroy();
	
	if (m_pForm != NULL)
	{
		delete m_pForm;
		m_pForm = NULL;
	}
	

EFS_END
}

INT CBO_CAT_PROC_0320_FORM::SetType007(CString sType, CString s007)
{
EFS_BEGIN

	m_sReceive007 = s007;
	m_sType = sType;
	return 0;

EFS_END
return -1;

}


HBRUSH CBO_CAT_PROC_0320_FORM::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}