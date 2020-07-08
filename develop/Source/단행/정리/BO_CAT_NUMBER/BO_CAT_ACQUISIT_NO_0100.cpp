// BO_CAT_ACQUISIT_NO_0100.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_ACQUISIT_NO_0100.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_ACQUISIT_NO_0100 dialog


CBO_CAT_ACQUISIT_NO_0100::CBO_CAT_ACQUISIT_NO_0100(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_ACQUISIT_NO_0100)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_ACQUISIT_NO_0100::~CBO_CAT_ACQUISIT_NO_0100()
{
}

VOID CBO_CAT_ACQUISIT_NO_0100::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_ACQUISIT_NO_0100)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_ACQUISIT_NO_0100, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_ACQUISIT_NO_0100)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_ACQUISIT_NO_0100 message handlers

BOOL CBO_CAT_ACQUISIT_NO_0100::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_ACQUISIT_NO_0100")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_ACQUISIT_NO_0100::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here
	CString sWorkTbl, sClassCodeType, sShelfCode;	
	GetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("작업구분"), sWorkTbl );
	GetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("분류기호구분"), sClassCodeType );
	GetControlData( _T("CM_BO_CAT_ACQUISIT_NO"), _T("별치기호"), sShelfCode );

	CString sClassCode, sAcqNo;
	GetControlData( _T("CM_BO_CAT_ACQUISIT_NO_0100"), _T("분류기호"), sClassCode );
	GetControlData( _T("CM_BO_CAT_ACQUISIT_NO_0100"), _T("수입순번호"), sAcqNo );
	
	if ( sClassCode.IsEmpty() || sAcqNo.IsEmpty() ) {
		AfxMessageBox( _T("항목을 모두 입력하십시요.") );
		return;
	}
	
	CESL_DataMgr *pDM;
	if ( sWorkTbl == _T("마지막수입순번호관리") ) {
		pDM = FindDM(_T("DM_BO_CAT_ACQUISIT_NO"));
	}
	else if ( sWorkTbl == _T("누락수입순번호관리") ) {
		pDM = FindDM(_T("DM_BO_CAT_OMIT_NO"));
	}
	if (!pDM) return;

	INT m_idx;
	m_idx = pDM->GetRowCount();

	//rec_key 만들기
	CString sRecKey;
	//pDM->MakeRecKey( sRecKey, table_name=_T(""), _T("CO_ACCESSION_REC_TBL"), _T("REC_KEY"));
	pDM->MakeRecKey( sRecKey );
	INT ids = pDM->StartFrame();

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sRecKey);
	pDM->AddDBFieldData(_T("CLASS_NO_TYPE"), _T("STRING"), sClassCodeType );
	pDM->AddDBFieldData(_T("CLASS_NO"), _T("STRING"), sClassCode );
	pDM->AddDBFieldData(_T("ACQUISIT_NO"), _T("STRING"), sAcqNo );
	pDM->AddDBFieldData(_T("SEPARATE_SHELF_CODE"), _T("STRING"), sShelfCode );	
	pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );

	if ( sWorkTbl == _T("마지막수입순번호관리") ) {
		ids = pDM->MakeInsertFrame(_T("CO_LAST_BOOK_ACQUISITNO_TBL"));

		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("MakeInsertFrame를 확인하세요"));
			return;
		}

	}
	else if ( sWorkTbl == _T("누락수입순번호관리") ) {
		ids = pDM->MakeInsertFrame(_T("CO_OMIT_BOOK_ACQUISITNO_TBL"));
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("MakeInsertFrame를 확인하세요"));
			return;
		}
	}

	ids = pDM->SendFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("SendFrame를 확인하세요"));
		return;
	}

	if (ids < 0) {
		AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return;
	}
	ids = pDM->EndFrame();	
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame 을 확인하세요"));
		return;
	}

	AfxMessageBox(_T("저장완료"));

	pDM->InsertRow(-1);
	pDM->SetCellData( _T("수입순번호"), sAcqNo, m_idx );
	pDM->SetCellData( _T("분류기호"), sClassCode, m_idx );
	/*
	CESL_ControlMgr *pCM1 = FindCM(_T("CM_BO_CAT_UNLESS_WORD_0000"));
	AllControlDisplay(_T("DM_BO_CAT_UNLESS_WORD_0000"));	
	pCM1->AllControlDisplay(m_idx);
	*/
	/*
	CString sTmp = pDM->GetCellData( _T("단어"), m_idx );
	sTmp = pDM->GetCellData( _T("조사"), m_idx+1 );
	pDM->SetCellData( _T("조사"), sWord, m_idx+1 );
	*/

	//AllControlDisplay(_T("CM_BO_CAT_AUXILIARY"));

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_ACQUISIT_NO"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_ACQUISIT_NO error") );
		return;
	}
	pCM->AllControlDisplay();

	CDialog::OnOK();

EFS_END
}

HBRUSH CBO_CAT_ACQUISIT_NO_0100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
