// BO_CAT_REG_NUMBER_0100.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_REG_NUMBER_0100.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_NUMBER_0100 dialog


CBO_CAT_REG_NUMBER_0100::CBO_CAT_REG_NUMBER_0100(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_NUMBER_0100)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_REG_NUMBER_0100::~CBO_CAT_REG_NUMBER_0100()
{
}

VOID CBO_CAT_REG_NUMBER_0100::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_NUMBER_0100)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_NUMBER_0100, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_NUMBER_0100)
	ON_BN_CLICKED(IDC_bRegNumIDOK, OnbRegNumIDOK)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_NUMBER_0100 message handlers

BOOL CBO_CAT_REG_NUMBER_0100::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_REG_NUMBER_0100")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분코드를 6자리 이하로 제한
	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_eRegNumberCode);
	pEdit->SetLimitText(6);
	//=====================================================


	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

// return 값은 중복데이터 갯수
INT CBO_CAT_REG_NUMBER_0100::SearchKindData(CString strKindCode, CString strMakeYear, CString strDivisionValue) 
{
EFS_BEGIN
	INT ids;
	CString strQuery;
	CString strWhere = _T("");

	if (strKindCode.GetLength() > 0)
		strWhere.Format(_T("KIND_CODE = '%s'"), strKindCode);

	if (strMakeYear.GetLength() > 0){
		if (strWhere.GetLength() > 0) strWhere += _T(" and ");
		strWhere += _T("MAKE_YEAR = '") + strMakeYear + _T("'");
	}

	if (strDivisionValue.GetLength() > 0){
		if (strWhere.GetLength() > 0) strWhere += _T(" and ");
		strWhere += _T("DIVISION_VALUE = '") + strDivisionValue + _T("'");
	}

	if (strWhere.GetLength() <=0 )
		return -100;

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	strQuery.Format(_T("SELECT count(*) from CO_LAST_NUMBER_TBL where %s AND MANAGE_CODE = UDF_MANAGE_CODE"), strWhere);
	ids = dm.RestructDataManager(strQuery);
	if (ids < 0) return -200;

	if (dm.GetRowCount() == 0) return -300;
	else {
		ids = _ttoi(dm.GetCellData(0, 0));
		return ids;
	}

	return 0;
EFS_END
}

VOID CBO_CAT_REG_NUMBER_0100::OnbRegNumIDOK() 
{
EFS_BEGIN

	INT ids;
	// TODO: Add your control notification handler code here
	CString sDivision, sNumber;
	GetControlData( _T("CM_BO_CAT_REG_NUMBER_0100"), _T("등록구분"), sDivision );
	GetControlData( _T("CM_BO_CAT_REG_NUMBER_0100"), _T("일련번호"), sNumber );
	if ( sDivision.IsEmpty() || sNumber.IsEmpty() ) {
		AfxMessageBox( _T("모든항목을 입력하세요.") );
		return;
	}
	sDivision.MakeUpper();

	// 중복 Data 확인
	ids = SearchKindData(_T("REG_NO"), _T(""), sDivision);
	if (ids < 0){
		AfxMessageBox(_T("중복 데이터 확인에 실패하였습니다."));
		return;
	}else if (ids > 0) {
		AfxMessageBox(_T("같은 등록구분이 존재합니다. 다른 등록구분값을 입력하여 주십시오."));
		return;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_NUMBER"));
	if (!pDM) return;

	INT m_idx;
	m_idx = pDM->GetRowCount();

	//rec_key 만들기
	CString sRecKey;
	//pDM->MakeRecKey( sRecKey, table_name=_T(""), _T("CO_ACCESSION_REC_TBL"), _T("REC_KEY"));
	pDM->MakeRecKey( sRecKey );
	ids = pDM->StartFrame();

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("STRING"), sRecKey);
	//pDM->AddDBFieldData(_T("MAKE_YEAR"), _T("STRING"), sMakeYear );
	pDM->AddDBFieldData(_T("LAST_NUMBER"), _T("STRING"), sNumber );
	pDM->AddDBFieldData(_T("PUBLISH_FORM_CODE"), _T("STRING"), _T("CO") );
	pDM->AddDBFieldData(_T("KIND_CODE"), _T("STRING"), _T("REG_NO") );
	pDM->AddDBFieldData(_T("DIVISION_VALUE"), _T("STRING"), sDivision );
	pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );

	ids = pDM->MakeInsertFrame(_T("CO_LAST_NUMBER_TBL"));
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("MakeInsertFrame를 확인하세요"));
		return;
	}

	ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return;
	}
	ids = pDM->EndFrame();	
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame()를 확인하세요"));
		return;
	}

	AfxMessageBox(_T("저장하였습니다."));

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_LAST_NUMBER"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_LAST_NUMBER error") );
		return;
	}
	pCM->AllControlDisplay();

	CDialog::OnOK();

EFS_END
}

HBRUSH CBO_CAT_REG_NUMBER_0100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
