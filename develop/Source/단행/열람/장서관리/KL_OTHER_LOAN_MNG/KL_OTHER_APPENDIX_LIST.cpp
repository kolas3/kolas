// KL_OTHER_APPENDIX_LIST.cpp : implementation file
//

#include "stdafx.h"
#include "KL_OTHER_APPENDIX_LIST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_APPENDIX_LIST dialog


CKL_OTHER_APPENDIX_LIST::CKL_OTHER_APPENDIX_LIST(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKL_OTHER_APPENDIX_LIST::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_OTHER_APPENDIX_LIST)
		// KOL.UDF.022
		m_pLoan = NULL;
	//}}AFX_DATA_INIT
}

CKL_OTHER_APPENDIX_LIST::~CKL_OTHER_APPENDIX_LIST()
{
}

void CKL_OTHER_APPENDIX_LIST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_OTHER_APPENDIX_LIST)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_OTHER_APPENDIX_LIST, CDialog)
	//{{AFX_MSG_MAP(CKL_OTHER_APPENDIX_LIST)
	ON_BN_CLICKED(IDC_btnSELECT, OnbtnSELECT)
	ON_BN_CLICKED(IDC_btnLOAN_CANCEL, OnbtnLOANCANCEL)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnSEND_CANCEL, OnbtnSENDCANCEL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_APPENDIX_LIST message handlers

BOOL CKL_OTHER_APPENDIX_LIST::PreTranslateMessage(MSG* pMsg) 
{
	if (WM_KEYDOWN == pMsg->message) {
		if (VK_ESCAPE == pMsg->wParam) {
			return TRUE;
		} else if (VK_RETURN == pMsg->wParam) {
			if (pMsg->hwnd == GetDlgItem( IDC_edtAPPENDIX_BARCODE )->m_hWnd) {
				// 선정
				CheckBarCodeRegNo();
			} else {
				// 발송
				OnbtnSELECT();
			}
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CKL_OTHER_APPENDIX_LIST::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	

	if(InitESL_Mgr(_T("SM_KL_OTHER_APPENDIX_LIST")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error : SM_KL_OTHER_APPENDIX_LIST"));
		return FALSE;
	}

	ViewGrid();

	GetDlgItem(IDC_edtAPPENDIX_BARCODE)->SetFocus();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CKL_OTHER_APPENDIX_LIST::ViewGrid()
{
	// display, 1건은 자동 선정
	CESL_DataMgr *pDM = FindDM(_T("DM_KL_OTHER_APPENDIX_LIST"));
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_KL_OTHER_APPENDIX_LIST"), _T("AppendixGrid"));
	pGrid->Display();
	if (NULL != pDM && NULL != pGrid) {
		if (1 == pDM->GetRowCount()) {
			pGrid->SetAt(0, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
		}
	}
	
	return 0;
}

void CKL_OTHER_APPENDIX_LIST::OnbtnSELECT() 
{
	INT ids = SelectCMToDM();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2(-1, _T("OnbtnSELECT"));
	if (0 == ids) {
		AfxMessageBox(_T("부록자료중 발송할 자료를 선택해주십시오."));
		return;
	} else if (1000 == ids) {
		return;
	}
	
	OnOK();
}

void CKL_OTHER_APPENDIX_LIST::OnbtnLOANCANCEL() 
{
	CESL_DataMgr *pDM = FindDM(_T("DM_KL_OTHER_APPENDIX_LIST"));
	pDM->FreeData();

	OnOK();
}

INT CKL_OTHER_APPENDIX_LIST::SelectCMToDM()
{
	INT ids;
	CESL_DataMgr *pDM = FindDM(_T("DM_KL_OTHER_APPENDIX_LIST"));
	if (NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("SelectCMToDM"));
	CESL_ControlMgr *pCM = FindCM(_T("CM_KL_OTHER_APPENDIX_LIST"));
	if (NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE2(-2, _T("SelectCMToDM"));
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(_T("AppendixGrid")));
	if (NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE2(-3, _T("SelectCMToDM"));

	pGrid->SelectMakeList();
	INT nRowCount = pDM->GetRowCount();

	// 대출가능여부 확인
	CString sCheck;
	CString sLoanYN;
	INT nCheck = 0;
	for (INT i = 0; i < pDM->GetRowCount(); i++) {
		if (-1 == pGrid->m_nSelectIdx) continue;
		
		pGrid->GetAt(i, pGrid->m_nSelectIdx, sCheck);
		if(sCheck.IsEmpty()) continue;

		nCheck++;
		pDM->GetCellData(_T("대출가능여부"), i, sLoanYN);
		if (_T("O") != sLoanYN) {
			AfxMessageBox( _T("대출가능한 자료만 선택해주십시오.") );
			return 1000;
		}
	}

	// 비선정 자료 제거
	if (0 < nCheck) {
		for (i = nRowCount-1; i > -1; i--) {
			if (-1 == pGrid->m_nSelectIdx) continue;
			
			pGrid->GetAt(i, pGrid->m_nSelectIdx, sCheck);
			if(!sCheck.IsEmpty()) continue;

			ids = pDM->DeleteRow(i);
			if (0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, _T("SelectCMToDM"));
		}
		
		return pDM->GetRowCount();
	}

	return nCheck;
}

INT CKL_OTHER_APPENDIX_LIST::CheckBarCodeRegNo()
{
	INT ids;
	CString sInputRegNo;
	GetDlgItem(IDC_edtAPPENDIX_BARCODE)->GetWindowText(sInputRegNo);
	sInputRegNo.TrimLeft(); sInputRegNo.TrimRight();
	CLocCommonAPI::MakeRegNoSpaceToZero(sInputRegNo);
	
	CESL_DataMgr *pDM = FindDM(_T("DM_KL_OTHER_APPENDIX_LIST"));
	if (NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("CheckBarCodeRegNo"));
	CESL_ControlMgr *pCM = FindCM(_T("CM_KL_OTHER_APPENDIX_LIST"));
	if (NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE2(-2, _T("CheckBarCodeRegNo"));
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(_T("AppendixGrid")));
	if (NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE2(-3, _T("CheckBarCodeRegNo"));
	
	CString sRegNo;
	for (INT i = 0; i < pDM->GetRowCount(); i++) {
		ids = pDM->GetCellData(_T("등록번호"), i, sRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-4, _T("CheckBarCodeRegNo"));
		
		if(0 != sRegNo.Compare(sInputRegNo)) continue;
		
		CString strCheck;
		pGrid->GetAt(i, pGrid->m_nSelectIdx, strCheck);
		if (pGrid->m_sSelectString != strCheck) {
			pGrid->SetAt(i, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
			pGrid->SetReverse(i);
			GetDlgItem(IDC_edtAPPENDIX_BARCODE)->SetWindowText(_T(""));
		}
		
		return 0;
	}
	
	AfxMessageBox( _T("부록자료가 존재하지 않습니다.") );

	return 0;
}

void CKL_OTHER_APPENDIX_LIST::OnbtnSENDCANCEL() 
{
	OnCancel();
}
