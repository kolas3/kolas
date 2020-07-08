// BO_CAT_REG_0510.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_REG_0510.h"
//#include _T("BO_CAT_REG_0510_01.h")
#include "..\..\..\단행\정리\BO_CAT_LAST_NUMBER\BO_CAT_LAST_NUMBER_0000.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0510 dialog


CBO_CAT_REG_0510::CBO_CAT_REG_0510(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_CAT_REG_0510::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_0510)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CBO_CAT_REG_0510::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_0510)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_0510, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_0510)
	ON_BN_CLICKED(IDC_bLastNumber, OnbLastNumber)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0510 message handlers

BOOL CBO_CAT_REG_0510::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_REG_0510::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
	if (!pDM) return;

	CString sACCESSION_REC_MAKE_YEAR, sACCESSION_REC_NO, sACQUISIT_CODE, sDIVISION_NAME, sCLASS_NAME, sPROVIDER;
	CString sREGISTRANT, sREG_DATE, sCLASS_WORKER, sCLASS_DATE, sCATALOG_WORKER, sCATALOG_DATE;
	CString sLABELING_WORKER, sLABELING_DATE, sPROCESSING_WORKER, sPROCESSING_COMPLETE_DATE;
	CString sYBK_TRANSFER_DATE, sSHELF_WORKER, sSHELF_DATE;

	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("수입년도"), sACCESSION_REC_MAKE_YEAR);
	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("원부번호"), sACCESSION_REC_NO);
	if ( sACCESSION_REC_NO == _T("") ) {
		AfxMessageBox ( _T("원부번호를 입력하세요.") );
		return;
	}
	INT iAccNoUpdate = _ttoi ( sACCESSION_REC_NO.GetBuffer(0) );
	if ( iAccNoUpdate > iAccNum ) {
		AfxMessageBox( _T("원부번호는 마지막원부번호 보다 작아야 합니다.") );
		return;
	}

	//원부번호 체크	
	CString sQueryCount = _T("SELECT COUNT(REC_KEY) FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_NO = '") + sACCESSION_REC_NO + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE AND ACCESSION_REC_MAKE_YEAR = '") + sACCESSION_REC_MAKE_YEAR + _T("'");
	CString sCount;	//시작 등록번호
	pDM->GetOneValueQuery( sQueryCount , sCount );
	if ( sCount != _T("0") ) {
		AfxMessageBox (_T("원부번호 부여된 자료가 있습니다."));
		//m_bInitDlg = FALSE;
		return;
	}

	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("수입구분"), sACQUISIT_CODE, -1, 0);
	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("과명"), sDIVISION_NAME);
	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("직명"), sCLASS_NAME);
	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("공급자"), sPROVIDER);
	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("등록자"), sREGISTRANT);
	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("등록일자"), sREG_DATE);
	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("분류자"), sCLASS_WORKER);
	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("분류작업일자"), sCLASS_DATE);
	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("목록자"), sCATALOG_WORKER);
	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("목록일자"), sCATALOG_DATE);
	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("장비자"), sLABELING_WORKER);
	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("장비일자"), sLABELING_DATE);
	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("정리완료자"), sPROCESSING_WORKER);
	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("정리완료일자"), sPROCESSING_COMPLETE_DATE);
	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("인계일자"), sYBK_TRANSFER_DATE);
	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("배가자"), sSHELF_WORKER);
	GetControlData(_T("CM_BO_CAT_REG_0510"), _T("배가일자"), sSHELF_DATE);
	
	//rec_key 만들기
	CString sRecKey;
	//pDM->MakeRecKey( sRecKey, table_name=_T(""), _T("CO_ACCESSION_REC_TBL"), _T("REC_KEY"));
	pDM->MakeRecKey( sRecKey );
	pDM->StartFrame();

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sRecKey);

	//pDM->AddDBFieldData(_T("PUBLISH_FORM_CODE"), _T("STRING"), _T("MO") );
	//pDM->AddDBFieldData(_T("ACCESSION_REC_MAKE_YEAR"), _T("STRING"), sREG_DATE.Left(4) );
	
	pDM->AddDBFieldData(_T("ACCESSION_REC_MAKE_YEAR"), _T("STRING"), sACCESSION_REC_MAKE_YEAR);
	pDM->AddDBFieldData(_T("ACCESSION_REC_NO"), _T("STRING"), sACCESSION_REC_NO);
	pDM->AddDBFieldData(_T("PUBLISH_FORM_CODE"), _T("STRING"), _T("MO"));
	pDM->AddDBFieldData(_T("ACQUISIT_CODE"), _T("STRING"), sACQUISIT_CODE);
	pDM->AddDBFieldData(_T("DIVISION_NAME"), _T("STRING"), sDIVISION_NAME);
	pDM->AddDBFieldData(_T("CLASS_NAME"), _T("STRING"), sCLASS_NAME);
	pDM->AddDBFieldData(_T("PROVIDER"), _T("STRING"), sPROVIDER);
	pDM->AddDBFieldData(_T("REGISTRANT"), _T("STRING"), sREGISTRANT);
	pDM->AddDBFieldData(_T("REG_DATE"), _T("STRING"), sREG_DATE);
	pDM->AddDBFieldData(_T("CLASS_WORKER"), _T("STRING"), sCLASS_WORKER);
	pDM->AddDBFieldData(_T("CLASS_DATE"), _T("STRING"), sCLASS_DATE);
	pDM->AddDBFieldData(_T("CATALOG_WORKER"), _T("STRING"), sCATALOG_WORKER);
	pDM->AddDBFieldData(_T("CATALOG_DATE"), _T("STRING"), sCATALOG_DATE);
	pDM->AddDBFieldData(_T("LABELING_WORKER"), _T("STRING"), sLABELING_WORKER);
	pDM->AddDBFieldData(_T("LABELING_DATE"), _T("STRING"), sLABELING_DATE);
	pDM->AddDBFieldData(_T("PROCESSING_WORKER"), _T("STRING"), sPROCESSING_WORKER);
	pDM->AddDBFieldData(_T("PROCESSING_COMPLETE_DATE"), _T("STRING"), sPROCESSING_COMPLETE_DATE);
	pDM->AddDBFieldData(_T("YBK_TRANSFER_DATE"), _T("STRING"), sYBK_TRANSFER_DATE);
	pDM->AddDBFieldData(_T("SHELF_WORKER"), _T("STRING"), sSHELF_WORKER);
	pDM->AddDBFieldData(_T("SHELF_DATE"), _T("STRING"), sSHELF_DATE);
	pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE"));

	pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(WORK_LOG));
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(WORK_LOG));

	/*
	GetControlData(_T("CM_MAIN_SAMPLE_INSERT"), _T("PASSWORD"), strPassword);
	GetControlData(_T("CM_MAIN_SAMPLE_INSERT"), _T("NAME"), strName);
	GetControlData(_T("CM_MAIN_SAMPLE_INSERT"), _T("TEL"), strTel);
	GetControlData(_T("CM_MAIN_SAMPLE_INSERT"), _T("BIRTHDAY"), srBirthday);
	GetControlData(_T("CM_MAIN_SAMPLE_INSERT"), _T("GRADE"), strGrade, -1, 0);
	GetControlData(_T("CM_MAIN_SAMPLE_MODIFY"), _T("ADDRESS"), strAddress);
	*/
/*
	CString strQuery, strResult;
	strQuery.Format(_T("select count(*) from esl_test where id = '%s'"), strID);
	pDM->GetOneValueQuery(strQuery, strResult);
	if (strResult != _T("0")) 
	{
		AfxMessageBox(_T("존재하는 아이디 입니당"));
		return;
	}
*/
	/*
	pDM->AddDBFieldData(_T("BIRTHDAY"), _T("STRING"), srBirthday);
	pDM->AddDBFieldData(_T("GRADE"), _T("NUMERIC"), strGrade);
	pDM->AddDBFieldData(_T("ADDRESS"), _T("STRING"), strAddress);
	*/

	pDM->MakeInsertFrame(_T("CO_ACCESSION_REC_TBL"));

	//마지막번호 UPDATE
	CString sQuery;
	sQuery.Format( _T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = '%s' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND KIND_CODE = '원부번호';") , sACCESSION_REC_NO );
	INT iCh = pDM->AddFrame( sQuery );


	INT ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return;
	}
	pDM->EndFrame();
	//AfxMessageBox(_T("Insert Success"));	

	//그리드 반영
	CString sWhere = _T("REC_KEY = '") + sRecKey + _T("'");

	if (pDM->RefreshDataManager(sWhere) < 0)	return;

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_REG_0500"));
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_BO_CAT_REG_0500 error") );
		return;
	}
	pCM->AllControlDisplay();

	AfxMessageBox(_T("저장완료"));
	//AllControlDisplay(_T("CM_MAIN_SAMPLE"));

	/*
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_REG_0500"));
	pCM->AllControlDisplay(1);	
	*/
	/*
	CESL_Mgr * pCESLMgr;
	//CESL_ControlMgr *pCM = pCESLMgr->FindCM(_T("CM_BO_CAT_REG_0500"));
	CESL_ControlMgr *pCM1 = pCESLMgr->FindCM(_T("CM_BO_CAT_REG_0500"));
	pCESLMgr->AllControlDisplay(_T("DM_BO_CAT_REG_0500"));
	*/
	//AllControlDisplay(_T("CM_BO_CAT_REG_0500"));
	//OnbSEARCH();
	
	//CDialog::OnOK();

EFS_END
}

BOOL CBO_CAT_REG_0510::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_REG_0510")) < 0)
	//if (InitESL_Mgr(_T("BO_CAT_REG")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	//등록자넣기
	CString sUserName = m_pInfo->USER_NAME;
	if ( sUserName == _T("") ) {
		sUserName = m_pInfo->USER_ID;
	}
	SetControlData( _T("CM_BO_CAT_REG_0510"), _T("등록자"), sUserName );
		
	//마지막 원부번호 검색	
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
	if (!pDM) return FALSE;

	CTime time = CTime::GetCurrentTime();
	INT iYear = time.GetYear();
	CString sYear;
	sYear.Format( _T("%d"), iYear );

	CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND KIND_CODE = '원부번호' AND MAKE_YEAR = '") + sYear + _T("'");
	CString sResult;
	pDM->GetOneValueQuery( sQuery , sResult );
	//INT iAccNum;
	iAccNum = _ttoi(sResult.GetBuffer(0));
	iAccNum ++;
	sResult.Format( _T("%d") , iAccNum );
	SetControlData( _T("CM_BO_CAT_REG_0510") , _T("원부번호") , sResult);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}


VOID CBO_CAT_REG_0510::OnbLastNumber() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	//CBO_CAT_REG_0510_01 dlg(this);
	//dlg.DoModal();
	BO_CAT_LAST_NUMBER_0000 dlg(0,this);
	dlg.DoModal();	

EFS_END
}
