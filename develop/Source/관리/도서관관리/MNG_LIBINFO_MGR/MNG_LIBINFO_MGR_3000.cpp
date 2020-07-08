// MNG_LIBINFO_MGR_3000.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_LIBINFO_MGR_3000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIBINFO_MGR_3000 dialog


CMNG_LIBINFO_MGR_3000::CMNG_LIBINFO_MGR_3000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_LIBINFO_MGR_3000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bModifyFlag = FALSE;
	m_nStartRowIdx = -1;

	m_cUseMANAGE_CODE = 'N';
}
CMNG_LIBINFO_MGR_3000::~CMNG_LIBINFO_MGR_3000()
{
	
}
VOID CMNG_LIBINFO_MGR_3000::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_LIBINFO_MGR_3000)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_LIBINFO_MGR_3000, CDialog)
	//{{AFX_MSG_MAP(CMNG_LIBINFO_MGR_3000)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_WM_CTLCOLOR()   
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_LIBINFO_MGR_3000 message handlers

BOOL CMNG_LIBINFO_MGR_3000::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("MNG_LIBINFO_MGR_3000") ) < 0 )
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_LIBINFO_MGR_3000") );
	
	if (m_nStartRowIdx >= 0) {
		POSITION pos = m_pGrid->m_arraySelect.GetHeadPosition();
		for (INT i = 0; i < m_pGrid->SelectGetCount(); i++) {
			pos = m_pGrid->m_arraySelect.FindIndex(i);
			if (pos == NULL) continue;
			if (m_pGrid->m_arraySelect.GetAt(pos) == m_nStartRowIdx) {
				m_pGrid->m_arrayPos = pos;
				break;
			}
		}

		m_nIndex = m_pGrid->SelectGetIdx();
	} else {
		m_nIndex = m_pGrid->SelectGetHeadPosition();
	}
	if (m_cUseMANAGE_CODE == 'N') {
		CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)FindControl(_T("CM_MNG_LIBINFO_MGR_3000"), _T("관리구분"));
		if (pCMB) pCMB->EnableWindow(FALSE);
	}
	m_pGrid->SetReverse(m_nIndex);
	AllControlDisplay(_T("CM_MNG_LIBINFO_MGR_3000"), m_nIndex);


	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_LIBINFO_MGR_3000::OnbPREV() 
{
	m_nIndex = m_pGrid->SelectGetPrev();
	if (m_nIndex == -1) {
		m_nIndex = m_pGrid->SelectGetHeadPosition();
		AfxMessageBox(_T("맨 처음 데이타 입니다."));
	} 
	m_pGrid->SetReverse(m_nIndex);
	AllControlDisplay(_T("CM_MNG_LIBINFO_MGR_3000"), m_nIndex);
}

VOID CMNG_LIBINFO_MGR_3000::OnbNEXT() 
{
	m_nIndex = m_pGrid->SelectGetNext();
	if (m_nIndex == -1) {
		m_nIndex = m_pGrid->SelectGetTailPosition();
		AfxMessageBox(_T("맨 마지막 데이타 입니다."));
	} 
	m_pGrid->SetReverse(m_nIndex);
	AllControlDisplay(_T("CM_MNG_LIBINFO_MGR_3000"), m_nIndex);
}

VOID CMNG_LIBINFO_MGR_3000::OnOK() 
{
	CESL_DataMgr *pDM = FindDM(_T("DM_MNG_LIBINFO_MGR_1000"));
	if (pDM == NULL) return;
	CESL_ControlMgr *pCM = FindCM(_T("CM_MNG_LIBINFO_MGR_3000"));
	if (pCM == NULL) return;

	CString strTmp, strPK;
	strPK = pDM->GetCellData(_T("REC_KEY"), m_nIndex);

	//++2008.10.15 UPDATE BY PWR {{++
	// 통합시스템 적용
	CString strQuery;
	strQuery.Format(_T("SELECT COUNT(*) FROM %s WHERE MANAGE_CODE=UDF_MANAGE_CODE AND REC_KEY <> %s"), pDM->TBL_NAME, strPK );
	CString strMANAGE_CODE, strLIB_CODE;
	pCM->GetControlMgrData(_T("관리구분"), strMANAGE_CODE);			strMANAGE_CODE.TrimLeft();			strMANAGE_CODE.TrimRight();
	if (m_cUseMANAGE_CODE == 'Y' && strMANAGE_CODE == _T("")) {
		AfxMessageBox(_T("관리구분별도서관설정이 'Y'인경우는 관리구분을 '적용안함'으로 할 수 없습니다."));
		return;
	}
	pCM->GetControlMgrData(_T("도서관부호"), strLIB_CODE);			strLIB_CODE.TrimLeft();			strLIB_CODE.TrimRight();
//	CString strQuery;
	if (strMANAGE_CODE.GetLength() == 0) {
		strQuery.Format(_T("SELECT COUNT(*) FROM %s WHERE MANAGE_CODE IS NULL AND REC_KEY <> %s"), pDM->TBL_NAME, strPK);
	} else {
		strQuery.Format(_T("SELECT COUNT(*) FROM %s WHERE MANAGE_CODE='%s' AND REC_KEY <> %s"), pDM->TBL_NAME, strMANAGE_CODE, strPK);
	}
	//--2008.10.15 UPDATE BY PWR --}}

	pDM->GetOneValueQuery(strQuery, strTmp);
	strTmp.TrimLeft();			strTmp.TrimRight();
	if (strTmp != _T("0")) {
		AfxMessageBox(_T("이미 해당 관리구분을 사용하는 데이터가 있습니다."));
		return;
	}

	pDM->StartFrame();
	pDM->InitDBFieldData();

	pCM->GetControlMgrData(_T("관리구분"), strTmp);			strTmp.TrimLeft();			strTmp.TrimRight();
	pDM->AddDBFieldData(_T("MANAGE_CODE"),		_T("STRING"),  strTmp, TRUE);
	pCM->GetControlMgrData(_T("도서관이름"), strTmp);			strTmp.TrimLeft();			strTmp.TrimRight();
	pDM->AddDBFieldData(_T("LIB_NAME"),			_T("STRING"),  strTmp, TRUE);
	pCM->GetControlMgrData(_T("도서관장이름"), strTmp);			strTmp.TrimLeft();			strTmp.TrimRight();
	pDM->AddDBFieldData(_T("LIB_DIRECTOR_NAME"),_T("STRING"),  strTmp, TRUE);
	pCM->GetControlMgrData(_T("도서관부호"), strTmp);			strTmp.TrimLeft();			strTmp.TrimRight();
	pDM->AddDBFieldData(_T("LIB_CODE"),			_T("STRING"),  strTmp, TRUE);
	pCM->GetControlMgrData(_T("도서관우편번호"), strTmp);			strTmp.TrimLeft();			strTmp.TrimRight();
	pDM->AddDBFieldData(_T("LIB_ZIP_CODE"),		_T("STRING"),  strTmp, TRUE);
	pCM->GetControlMgrData(_T("도서관주소"), strTmp);			strTmp.TrimLeft();			strTmp.TrimRight();
	pDM->AddDBFieldData(_T("LIB_ADDRESS"),		_T("STRING"),  strTmp, TRUE);
	pCM->GetControlMgrData(_T("도서관전화번호"), strTmp);			strTmp.TrimLeft();			strTmp.TrimRight();
	pDM->AddDBFieldData(_T("LIB_TEL"),			_T("STRING"),  strTmp, TRUE);
	pCM->GetControlMgrData(_T("도서관팩스"), strTmp);			strTmp.TrimLeft();			strTmp.TrimRight();
	pDM->AddDBFieldData(_T("LIB_FAX"),			_T("STRING"),  strTmp, TRUE);
	pCM->GetControlMgrData(_T("도서관홈페이지"), strTmp);			strTmp.TrimLeft();			strTmp.TrimRight();
	pDM->AddDBFieldData(_T("LIB_URL"),			_T("STRING"),  strTmp, TRUE);
	pCM->GetControlMgrData(_T("도서관대표EMAIL"), strTmp);			strTmp.TrimLeft();			strTmp.TrimRight();
	pDM->AddDBFieldData(_T("LIB_EMAIL"),		_T("STRING"),  strTmp, TRUE);
	pDM->AddDBFieldData(_T("LAST_WORK"),		_T("STRING"),  GetWorkLogMsg( _T("도서관관리"), __WFILE__, __LINE__ ), TRUE);
	pDM->MakeUpdateFrame(pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strPK, m_nIndex);
	INT ids = pDM->SendFrame();
	pDM->EndFrame();

	if (ids < 0) {
		AfxMessageBox(_T("수정도중 에러가 발생하였습니다."));
	} else {
		m_bModifyFlag = TRUE;
		AfxMessageBox(_T("수정되었습니다."));
	}
}

VOID CMNG_LIBINFO_MGR_3000::OnCancel() 
{
	CDialog::OnCancel();
}

HBRUSH CMNG_LIBINFO_MGR_3000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}