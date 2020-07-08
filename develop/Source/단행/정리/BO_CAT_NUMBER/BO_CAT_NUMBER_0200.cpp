// BO_CAT_NUMBER_0200.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_NUMBER_0200.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_NUMBER_0200 dialog


CBO_CAT_NUMBER_0200::CBO_CAT_NUMBER_0200(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_NUMBER_0200)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_NUMBER_0200::~CBO_CAT_NUMBER_0200()
{
}

VOID CBO_CAT_NUMBER_0200::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_NUMBER_0200)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_NUMBER_0200, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_NUMBER_0200)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_NUMBER_0200 message handlers

BOOL CBO_CAT_NUMBER_0200::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_NUMBER_0200::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here
	CString sWorkTbl, sClassCodeType, sShelfCode;	
	GetControlData( _T("CM_BO_CAT_NUMBER_0200"), _T("작업구분"), sWorkTbl );
	GetControlData( _T("CM_BO_CAT_NUMBER_0200"), _T("분류기호구분"), sClassCodeType );
	GetControlData( _T("CM_BO_CAT_NUMBER_0200"), _T("별치기호"), sShelfCode );

	CString sClassCode, sAcqNo;
	GetControlData( _T("CM_BO_CAT_NUMBER_0200"), _T("분류기호"), sClassCode );
	GetControlData( _T("CM_BO_CAT_NUMBER_0200"), _T("수입순번호"), sAcqNo );
	
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
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return;
	}


	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sRecKey);
	pDM->AddDBFieldData(_T("CLASS_NO_TYPE"), _T("STRING"), sClassCodeType );
	pDM->AddDBFieldData(_T("CLASS_NO"), _T("STRING"), sClassCode );
	pDM->AddDBFieldData(_T("ACQUISIT_NO"), _T("STRING"), sAcqNo );
	pDM->AddDBFieldData(_T("SEPARATE_SHELF_CODE"), _T("STRING"), sShelfCode );	

	pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(_T("수입순번호입력"),WORK_LOG));
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("수입순번호입력"),WORK_LOG));

	//======================================                    
	//2008.10.14 ADD BY PJW : 임력시 관리구분 조건추가        
//	CString strManageCode ;                                   
//	strManageCode = m_pInfo->MANAGE_CODE;                     
//	pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), strManageCode );
	pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );
	//====================================== 

	if ( sWorkTbl == _T("마지막수입순번호관리") ) {
		 ids = pDM->MakeInsertFrame(_T("CO_LAST_BOOK_ACQUISITNO_TBL"));
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("MakeInsertFrame 를 확인하세요"));
			return;
		}
	}
	else if ( sWorkTbl == _T("누락수입순번호관리") ) {
		//=====================================================
		//2008.11.20 ADD BY PJW : 누락수입순번호를 부여하면 중복조사를 한다.
		CString strQuery;
		CString strCnt;
		if( sShelfCode.IsEmpty() )
		{
			strQuery.Format(_T("SELECT COUNT(1) FROM BO_BOOK_TBL WHERE  MANAGE_CODE = UDF_MANAGE_CODE AND CLASS_NO_TYPE = '%s' AND CLASS_NO = '%s' AND SEPARATE_SHELF_CODE IS NULL AND BOOK_CODE = '%s'")
						, sClassCodeType, sClassCode, sAcqNo );
		}
		else
		{
			strQuery.Format(_T("SELECT COUNT(1) FROM BO_BOOK_TBL WHERE  MANAGE_CODE = UDF_MANAGE_CODE AND CLASS_NO_TYPE = '%s' AND CLASS_NO = '%s' AND SEPARATE_SHELF_CODE = '%s' AND BOOK_CODE = '%s'")
						, sClassCodeType, sClassCode, sShelfCode, sAcqNo );
		}
		pDM->GetOneValueQuery(strQuery,strCnt);
		if( _ttoi( strCnt ) > 0 )
		{
			AfxMessageBox(_T("부여된 수입순번호가 존재합니다."));
			return;
		}

		if( sShelfCode.IsEmpty() )
		{
			strQuery.Format(_T("SELECT COUNT(1) FROM CO_OMIT_BOOK_ACQUISITNO_TBL WHERE  MANAGE_CODE = UDF_MANAGE_CODE AND CLASS_NO_TYPE = '%s' AND CLASS_NO = '%s' AND SEPARATE_SHELF_CODE IS NULL AND ACQUISIT_NO = %s")
						, sClassCodeType, sClassCode, sAcqNo );
		}
		else
		{
			strQuery.Format(_T("SELECT COUNT(1) FROM CO_OMIT_BOOK_ACQUISITNO_TBL WHERE  MANAGE_CODE = UDF_MANAGE_CODE AND CLASS_NO_TYPE = '%s' AND CLASS_NO = '%s' AND SEPARATE_SHELF_CODE = '%s' AND ACQUISIT_NO = %s")
						, sClassCodeType, sClassCode, sShelfCode, sAcqNo );
		}
		pDM->GetOneValueQuery(strQuery,strCnt);
		if( _ttoi( strCnt ) > 0 )
		{
			AfxMessageBox(_T("누락수입순번호가 존재합니다."));
			return;
		}
		//=====================================================
		ids = pDM->MakeInsertFrame(_T("CO_OMIT_BOOK_ACQUISITNO_TBL"));
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("MakeInsertFrame 를 확인하세요"));
			return;
		}
	}

	ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return;
	}
	ids = pDM->EndFrame();	
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("EndFrame 를 확인하세요"));
			return;
		}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //2004.06.03 김소정 추가  
	pDM->InsertRow(-1);
	pDM->SetCellData(_T("REC_KEY"), sRecKey, m_idx);
	pDM->SetCellData(_T("분류기호구분"), sClassCodeType, m_idx);
	pDM->SetCellData(_T("분류기호"), sClassCode, m_idx);
	pDM->SetCellData(_T("수입순번호"), sAcqNo, m_idx);
	pDM->SetCellData(_T("별치기호"), sShelfCode, m_idx);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	AfxMessageBox(_T("저장완료"));

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

BOOL CBO_CAT_NUMBER_0200::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_NUMBER_0200")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	// 040916_HSS 기본값 설정 +++++++++++++++++++++++++++++++++
	//GetControlData( _T("CM_BO_CAT_NUMBER_0200"), _T("작업구분"), sWorkTbl );
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_ACQUISIT_NO"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_ACQUISIT_NO error") );
		return FALSE;
	}
	CString strTmpData;
	pCM->GetControlMgrData(_T("작업구분"), strTmpData);
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cWORK_TYPE );
	//SetControlData( _T("CM_BO_CAT_NUMBER_0200"), _T("작업구분"), strTmpData );
	if (!strTmpData.Compare(_T("마지막수입순번호관리")))
		pCombo->SetCurSel( 0 );
	else if (!strTmpData.Compare(_T("누락수입순번호관리")))
		pCombo->SetCurSel( 1 );
//	pCM->GetControlMgrData(_T("분류기호구분"), strTmpData);
//	SetControlData( _T("CM_BO_CAT_NUMBER_0200"), _T("분류기호구분"), strTmpData );
//	pCM->GetControlMgrData(_T("분류기호부터"), strTmpData);
//	SetControlData( _T("CM_BO_CAT_NUMBER_0200"), _T("분류기호"), strTmpData );
//	pCM->GetControlMgrData(_T("별치기호"), strTmpData);
//	SetControlData( _T("CM_BO_CAT_NUMBER_0200"), _T("별치기호"), strTmpData );
	// --------------------------------------------------------

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

HBRUSH CBO_CAT_NUMBER_0200::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
