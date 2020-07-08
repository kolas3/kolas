// SE_CAT_INPUT_BOOK.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_INPUT_BOOK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_INPUT_BOOK dialog


CSE_CAT_INPUT_BOOK::CSE_CAT_INPUT_BOOK(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pParent = NULL;
	m_pParent = pParent;

	m_pCM = NULL;
	m_pDM_Volume = NULL;

	m_nSlectedVolIdx = 0;

//	m_pCatApi = NULL;
}

CSE_CAT_INPUT_BOOK::~CSE_CAT_INPUT_BOOK()
{
}

VOID CSE_CAT_INPUT_BOOK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_INPUT_BOOK)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_INPUT_BOOK, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_INPUT_BOOK)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_INPUT_BOOK message handlers

BOOL CSE_CAT_INPUT_BOOK::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_연속_정리_책입력")) < 0)
	{
		AfxMessageBox ( _T("Init SM_연속_정리_책입력 Error") );
		return FALSE;
	}
	m_pCM = FindCM(_T("CM_연속_정리_책"));
	if ( m_pCM == NULL ) 
	{
		AfxMessageBox ( _T("Init CM_연속_정리_책 Error") );
		return FALSE;
	}

	m_pDM_Volume = FindDM(_T("DM_연속_정리DM_권정보"));
	if ( m_pDM_Volume == NULL ) 
	{
		AfxMessageBox ( _T("Init DM_연속_정리DM_권정보 Error") );
		return FALSE;
	}

	//자료 상태를 설정 해준다.
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbBOOK_WORKING_STATUS);
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("정리대상자료") );
		pCombo->InsertString( 1, _T("배가자료(소장자료)") );
		pCombo->SetCurSel( 0 );
	}

	// 권호명 Setting
	m_pCM->SetControlMgrData(_T("SV_권호명"), m_pDM_Volume->GetCellData(_T("SV_권호명"), m_nSlectedVolIdx) );

	// 배가위치부호 default -> Link Data로 안잡히는 이유가 무엇인지,,
	CESL_DataMgr* pDM_Species = FindDM(_T("DM_연속_정리DM_종정보"));
	if ( pDM_Species == NULL ) 
	{
		AfxMessageBox ( _T("Init DM_연속_정리DM_권정보 Error") );
		return FALSE;
	}
	CString strShelfLocCode = pDM_Species->GetCellData(_T("SS_배가위치_코드"), 0);
	m_pCM->SetControlMgrData(_T("SB_배가위치부호"), strShelfLocCode );

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;

EFS_END
	return FALSE;
}

VOID CSE_CAT_INPUT_BOOK::OnOK() 
{
	// 책정보 입력
	INT ids = SaveDataToDM();
	if (ids<0) return;

	CDialog::OnOK();
}

INT CSE_CAT_INPUT_BOOK::SaveDataToDM()
{
EFS_BEGIN
	CComboBox * pCombo;
	CString strItemData;
	INT ids;
	INT nCurSel = 0;

	// 저장상태
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbBOOK_WORKING_STATUS );
	if(pCombo) nCurSel = pCombo->GetCurSel();

	CESL_DataMgr* pDM_Book;
	CString strWorkingStatus;
	if (nCurSel == 0) {
		strWorkingStatus.Format(_T("SEC111N"));	// 정리중
		pDM_Book = FindDM(_T("DM_연속_정리DM_책정보"));
		if ( pDM_Book == NULL ) 
		{
			AfxMessageBox ( _T("Init DM_연속_정리DM_책정보 Error") );
			return -1;
		}
	}
	if (nCurSel == 1) {
		strWorkingStatus.Format(_T("SEL212N"));	// 소장책
		pDM_Book = FindDM(_T("DM_연속_정리DM_소장책정보"));
		if ( pDM_Book == NULL ) 
		{
			AfxMessageBox ( _T("Init DM_연속_정리DM_소장책정보 Error") );
			return -1;
		}
	}

	// 종 DM
	CESL_DataMgr* pDM_Species = FindDM(_T("DM_연속_정리DM_종정보"));
	if ( pDM_Species == NULL ) 
	{
		AfxMessageBox ( _T("Init DM_연속_정리DM_종정보 Error") );
		return -1;
	}

	// Data
	CString strRegCode, strAcqDate, strShelfLocCode, strRemark;
	m_pCM->GetControlMgrData(_T("SB_등록구분_코드"), strRegCode);
	m_pCM->GetControlMgrData(_T("SB_입수일"), strAcqDate);
	m_pCM->GetControlMgrData(_T("SB_배가위치부호"), strShelfLocCode);
	m_pCM->GetControlMgrData(_T("SB_비고"), strRemark);

	// 책입력
	ids = ((CCatApi_SE*)m_pCatApi)->InsertBookToDM(pDM_Species, m_pDM_Volume, pDM_Book
			, m_nSlectedVolIdx, strWorkingStatus, 0, strRegCode, strAcqDate, strShelfLocCode, strRemark );
	if (ids<0) 
	{
		AfxMessageBox(_T("책 입력을 정상적으로 완료하지 못하였습니다."));
		return -1;
	}
	
	// 권의 총책수 + 1
	m_pDM_Volume->GetCellData(_T("SV_총자료수"), m_nSlectedVolIdx, strItemData);
	ids = _ttoi(strItemData) + 1;
	strItemData.Format(_T("%d"), ids);
	m_pDM_Volume->SetCellData(_T("SV_총자료수"), strItemData, m_nSlectedVolIdx);

	CDialog::OnOK();

EFS_END
return -1;
}

HBRUSH CSE_CAT_INPUT_BOOK::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
