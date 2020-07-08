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

BOOL CBO_CAT_PROC_0320_FORM::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_CAT_PROC_0320_FORM::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0320_FORM)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0320_FORM, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0320_FORM)
	ON_CBN_SELCHANGE(IDC_cMAT_CATEGORY, OnSelchangecMATCATEGORY)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320_FORM message handlers
BOOL CBO_CAT_PROC_0320_FORM::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cMAT_CATEGORY);
	if (pComboBox)
	{
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

		pComboBox->SetCurSel(0);
	}

	CreateScreen('a');
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_PROC_0320_FORM::OnOK() 
{
	// TODO: Add extra validation here
	//CString sItem;
	INT ids;
	//GetControlData(_T("CM_BO_CAT_PROC_0320_FORM_04"), _T("자료범주표시"), sItem);


	if (m_pForm == NULL) return;
	//CESL_ControlMgr *pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_04"));
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
	/*
	else if (pCM == NULL) {
		//pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_02"));
		//if (pCM != NULL) {
		//	iCh = 2;
		//}
	}
	else {
		pCM = m_pForm->FindCM(_T("CM_BO_CAT_PROC_0320_FORM_04"));
		if (pCM != NULL) {
			iCh = 4;
		}
	}
	//if (pCM == NULL) return;
	*/

	CESL_Control_Element *pCME = NULL;
	POSITION pos = pCM->Control_List.GetHeadPosition();
	CString strData;
	CString strAlias;
	while(pos)
	{
		pCME = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCME == NULL) continue;
		
		strAlias = pCME->GetAlias();
		ids = pCME->GetControlMgrData(strData);
		if (ids < 0) continue;

		// ALIAS = strAlias, DATA = strData
		if ( strData == _T("") ) {
			strData = _T(" ");
		}
		if ( strData == _T("") && strAlias == _T("축소비율")) {
			strData = _T("   ");
		}
		s007 = s007 + strData;

		//


	}

	
	CDialog::OnOK();
}

INT CBO_CAT_PROC_0320_FORM::CreateScreen(TCHAR tchCatagory)
{
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
			m_pForm = new CBO_CAT_PROC_0320_FORM_01(this);
			((CBO_CAT_PROC_0320_FORM_01*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'd':
            m_pForm = new CBO_CAT_PROC_0320_FORM_02(this);
            ((CBO_CAT_PROC_0320_FORM_02*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
            break;
        case 'g':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_03(this);
            ((CBO_CAT_PROC_0320_FORM_03*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
            break;
        case 'h':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_04(this);
            ((CBO_CAT_PROC_0320_FORM_04*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'k':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_05(this);
            ((CBO_CAT_PROC_0320_FORM_05*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'm':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_06(this);
            ((CBO_CAT_PROC_0320_FORM_06*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 's':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_07(this);
            ((CBO_CAT_PROC_0320_FORM_07*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 't':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_08(this);
            ((CBO_CAT_PROC_0320_FORM_08*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'v':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_09(this);
            ((CBO_CAT_PROC_0320_FORM_09*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'z':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_10(this);
            ((CBO_CAT_PROC_0320_FORM_10*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;
		case 'c':
	        m_pForm = new CBO_CAT_PROC_0320_FORM_11(this);
            ((CBO_CAT_PROC_0320_FORM_11*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();
			break;

		default:
			m_pForm = new CBO_CAT_PROC_0320_FORM_12(this);
			((CBO_CAT_PROC_0320_FORM_12*)m_pForm)->Create(this);
			m_pForm->MoveWindow(rect);
			m_pForm->ShowWindow(SW_SHOW);
			m_pForm->SetFocus();

			break;
	}

	return 0;
}

VOID CBO_CAT_PROC_0320_FORM::OnSelchangecMATCATEGORY() 
{
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
				//CreateScreen(strBuffer.GetAt(0));
				CreateScreen(cForm);
			}
		}
	}
}

VOID CBO_CAT_PROC_0320_FORM::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pForm != NULL)
	{
		delete m_pForm;
		m_pForm = NULL;
	}
	
}
