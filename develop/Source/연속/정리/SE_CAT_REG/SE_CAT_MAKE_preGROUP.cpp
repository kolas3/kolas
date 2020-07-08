// SE_CAT_MAKE_preGROUP.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_MAKE_preGROUP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_MAKE_preGROUP dialog


CSE_CAT_MAKE_preGROUP::CSE_CAT_MAKE_preGROUP(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CAT_MAKE_preGROUP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CSE_CAT_MAKE_preGROUP::~CSE_CAT_MAKE_preGROUP()
{
}

BOOL CSE_CAT_MAKE_preGROUP::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);

	
}

VOID CSE_CAT_MAKE_preGROUP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_MAKE_preGROUP)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_MAKE_preGROUP, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_MAKE_preGROUP)
	ON_BN_CLICKED(IDC_CREAT_preGROUPNO, OnCREATpreGROUPNO)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_MAKE_preGROUP message handlers

BOOL CSE_CAT_MAKE_preGROUP::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_연속_정리_가원부생성")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_연속_정리_가원부생성"));

	CString m_Year;
	// 040304_HSS 수정 LocalTime으로 Select +++++++++++++++
	//CTime mTime = pDM->GetDBTime();
	CTime mTime = CTime::GetCurrentTime();
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Year.Format(_T("%d"), mTime.GetYear());

	SetControlData(_T("CM_연속_정리_가원부생성"), _T("입수년도"), m_Year);

	CString ACCESSION_REC_NO,tmp;
	pDM->GetOneValueQuery(_T("Select max(ACCESSION_REC_NO) from CO_TEMP_ACCESSION_REC_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE"), ACCESSION_REC_NO);

	tmp.Format(_T("%d"), _ttoi(ACCESSION_REC_NO.GetBuffer(0)) +1 );
	SetControlData(_T("CM_연속_정리_가원부생성"), _T("가원부번호"), tmp);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_CAT_MAKE_preGROUP::OnCREATpreGROUPNO() 
{
	// TODO: Add your control notification handler code here

	CString	strGroupNo,aquCode,acqYear;

	GetControlData(_T("CM_연속_정리_가원부생성"), _T("가원부번호"), strGroupNo);
	GetControlData(_T("CM_연속_정리_가원부생성"), _T("수입구분"), aquCode);
	GetControlData(_T("CM_연속_정리_가원부생성"), _T("입수년도"), acqYear);
	
	if( strGroupNo == _T("") )
	{
		AfxMessageBox(_T("가원부 번호를 입력해 주십시요 !"));
		return;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_연속_정리_가원부생성"));

	CString tmp,Qry;
	Qry.Format(_T("Select COUNT(*) from CO_TEMP_ACCESSION_REC_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACCESSION_REC_MAKE_YEAR='%s' AND ACCESSION_REC_NO = %s "),acqYear,strGroupNo);
	pDM->GetOneValueQuery(Qry, tmp);
	
	if ( tmp == _T("1"))
	{
		AfxMessageBox(_T("이미 생성된 가원부 입니다 !"));
		return;
	}
	

	pDM->StartFrame();
	pDM->InitDBFieldData();

	CString strREC_KEY;
	pDM->MakeRecKey(strREC_KEY);
	
	pDM->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),strREC_KEY);
	pDM->AddDBFieldData(_T("PUBLISH_FORM_CODE"),_T("STRING"),_T("MO")); 
	pDM->AddDBFieldData(_T("ACCESSION_REC_MAKE_YEAR"),_T("STRING"),acqYear); 
	pDM->AddDBFieldData(_T("ACQUISIT_CODE"),_T("STRING"),aquCode); 
	pDM->AddDBFieldData(_T("ACCESSION_REC_NO"),_T("NUMERIC"),strGroupNo);

	pDM->AddDBFieldData(_T("FIRST_WORK"),_T("STRING"),GetWorkLogMsg(WORK_LOG));
	pDM->AddDBFieldData(_T("LAST_WORK"),_T("STRING"),GetWorkLogMsg(WORK_LOG));
	pDM->AddDBFieldData(_T("MANAGE_CODE"),_T("STRING"),_T("UDF_MANAGE_CODE"));


	pDM->MakeInsertFrame(_T("CO_TEMP_ACCESSION_REC_TBL"));
	pDM->SendFrame();

	pDM->EndFrame();

	EndDialog(0);

}

HBRUSH CSE_CAT_MAKE_preGROUP::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
