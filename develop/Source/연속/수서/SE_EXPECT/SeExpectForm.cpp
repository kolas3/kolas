// SeExpectForm.cpp : implementation file
//

#include "stdafx.h"
#include "SeExpectForm.h"
#include "..\SE_API\SeExpectVerifier.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeExpectForm dialog


CSeExpectForm::CSeExpectForm(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeExpectForm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeExpectForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDateForm = NULL;
	m_pVolTitleForm1 = NULL;
	m_pVolTitleForm2 = NULL;
	m_pAppendixForm = NULL;

	m_strSPECIES_KEY = _T("");
	m_strACQ_KEY = _T("");
	m_nACQ_TYPE = 1; //default
	m_nOpenMode = 1; //종화면 입력 default
	
	//loveisverb
	//m_bIsFirstCreated = FALSE;
	m_bIsFirstCreated = TRUE;
	m_bIsParentHaveData = FALSE;

	m_bIsVerified = FALSE;
	m_strTITLE = _T("");
	
	m_nPubType = -1; //default
	m_nAcqType = -1; //default

	m_strPUB_FREQ = _T("");
	
	m_strPUB_EXP_TYPE = _T("");
	m_strACQ_EXP_TYPE = _T("");
	m_strPUB_EXP_KEY = _T("");
	m_strACQ_EXP_KEY = _T("");
}


CSeExpectForm::~CSeExpectForm()
{
	FreeChildForm();
}

VOID CSeExpectForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeExpectForm)
	DDX_Control(pDX, IDC_tabEXPECT, m_ctrlTAB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeExpectForm, CDialog)
	//{{AFX_MSG_MAP(CSeExpectForm)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabEXPECT, OnSelchangetabEXPECT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeExpectForm message handlers
BOOL CSeExpectForm::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}

BOOL CSeExpectForm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("연속_수서_예측") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_예측") );
		return false;
	}

	
	CString stralias;
	if(1 == m_nACQ_TYPE) stralias = _T("DM_연속_예측_구입정보");
	else stralias = _T("DM_연속_예측_기증정보");
	
	m_pDM = FindDM(stralias);
	if(!m_pDM) {
		AfxMessageBox( _T("DM Error : DM_연속_예측_구입정보 or DM_연속_예측_기증정보") );
		return false;
	}
	
	if(InitOpenMode()<0) {
		EndDialog(0);
		return FALSE;
	}

	CreateChildForm();
	
	m_ctrlTAB.InsertItem(0,_T("발행일/입수일 예측"));
	m_ctrlTAB.InsertItem(1,_T("권호 예측"));
	m_ctrlTAB.InsertItem(2,_T("부차적권호 예측"));
	m_ctrlTAB.InsertItem(3,_T("부록 예측"));
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CSeExpectForm::InitOpenMode()
{
	
	if(1 == m_nOpenMode) //종화면 입력 모드  
	{		
		//본서명을 보여 준다 
		if(!m_strTITLE.IsEmpty()) 
			GetDlgItem(IDC_edtEXP_TITILE)->SetWindowText(m_strTITLE);
		
		/*
		if(false == m_bIsFirstCreated)
		{
			//종화면에서 처음 뜨는 것이 아닐 경우 
			//종화면의 모든 예측 정보를 복사해 온다
			ExchangeDM(2);
		}
		*/

		//test
		if(m_bIsVerified) ExchangeDM(2);


	}
	else if(m_nOpenMode<0) //수정 모드
	{
		
		//loveisverb
		//본서명을 보여 준다 
		if(!m_strTITLE.IsEmpty()) 
			GetDlgItem(IDC_edtEXP_TITILE)->SetWindowText(m_strTITLE);
		
		//종키와 수입구분을 받아서 구입 또는 기증 정보를 얻어온다.
		if(m_strSPECIES_KEY.IsEmpty()) {
			AfxMessageBox(_T("종KEY가 입력되지 않았습니다!"));
			return -1;
		}
		
		CString strDM,strWhere;
		if(1 == m_nACQ_TYPE)
				strDM = _T("DM_연속_예측_구입정보");
		if(2 == m_nACQ_TYPE)
				strDM = _T("DM_연속_예측_기증정보");
	
		strWhere.Format(_T("SPECIES_KEY = %s"),m_strSPECIES_KEY);
		CESL_DataMgr* pDM = FindDM(strDM);
		pDM->RefreshDataManager(strWhere);
		

		if(1 == m_nACQ_TYPE) {
			m_strACQ_KEY =  pDM->GetCellData(_T("SP_구입관리KEY"),0);	
			m_strPUB_EXP_KEY = pDM->GetCellData(_T("SP_발행예측정보KEY"),0);
			m_strACQ_EXP_KEY = pDM->GetCellData(_T("SP_입수예측정보KEY"),0);
			m_strPUB_EXP_TYPE = pDM->GetCellData(_T("SP_발행예측구분"),0);
			m_strACQ_EXP_TYPE = pDM->GetCellData(_T("SP_입수예측구분"),0);

		} else if(2 == m_nACQ_TYPE) {
			m_strACQ_KEY =  pDM->GetCellData(_T("SD_기증관리KEY"),0);
			m_strPUB_EXP_KEY = pDM->GetCellData(_T("SD_발행예측정보KEY"),0);
			m_strACQ_EXP_KEY = pDM->GetCellData(_T("SD_입수예측정보KEY"),0);
			m_strPUB_EXP_TYPE = pDM->GetCellData(_T("SD_발행예측구분"),0);
			m_strACQ_EXP_TYPE = pDM->GetCellData(_T("SD_입수예측구분"),0);
		}

		
		if(-1 == m_nOpenMode) //종화면 수정
		{
			if(false == m_bIsFirstCreated && m_bIsParentHaveData)
			{
				//종화면에서 처음 뜨는 것이 아닐 경우 (종화면에 데이터가 있는 경우)
				//종화면의 모든 예측 정보를 복사해 온다
				ExchangeDM(2);
			}

		}
		else if(-2 == m_nOpenMode) //일반수정
		{
			
		}
		
		
		
		/*
		//본서명을 보여 준다 
		if(!m_strTITLE.IsEmpty()) 
			GetDlgItem(IDC_edtEXP_TITILE)->SetWindowText(m_strTITLE);
		
		//종키와 수입구분을 받아서 구입 또는 기증 정보를 얻어온다.
		if(m_strSPECIES_KEY.IsEmpty()) {
			AfxMessageBox(_T("종KEY가 입력되지 않았습니다!"));
			return -1;
		}
		
		CString strDM,strWhere;
		if(1 == m_nACQ_TYPE)
				strDM = _T("DM_연속_예측_구입정보");
		if(2 == m_nACQ_TYPE)
				strDM = _T("DM_연속_예측_기증정보");
	
		strWhere.Format(_T("SPECIES_KEY = %s"),m_strSPECIES_KEY);
		CESL_DataMgr* pDM = FindDM(strDM);
		pDM->RefreshDataManager(strWhere);
		

		if(1 == m_nACQ_TYPE) {
			m_strACQ_KEY =  pDM->GetCellData(_T("SP_구입관리KEY"),0);	
			m_strPUB_EXP_KEY = pDM->GetCellData(_T("SP_발행예측정보KEY"),0);
			m_strACQ_EXP_KEY = pDM->GetCellData(_T("SP_입수예측정보KEY"),0);
			m_strPUB_EXP_TYPE = pDM->GetCellData(_T("SP_발행예측구분"),0);
			m_strACQ_EXP_TYPE = pDM->GetCellData(_T("SP_입수예측구분"),0);

		} else if(2 == m_nACQ_TYPE) {
			m_strACQ_KEY =  pDM->GetCellData(_T("SD_기증관리KEY"),0);
			m_strPUB_EXP_KEY = pDM->GetCellData(_T("SD_발행예측정보KEY"),0);
			m_strACQ_EXP_KEY = pDM->GetCellData(_T("SD_입수예측정보KEY"),0);
			m_strPUB_EXP_TYPE = pDM->GetCellData(_T("SD_발행예측구분"),0);
			m_strACQ_EXP_TYPE = pDM->GetCellData(_T("SD_입수예측구분"),0);
		}


		if(-2 == m_nOpenMode) GetDlgItem(IDOK)->SetWindowText(_T("저장"));
		*/
	
	}


	return 0;
}

INT CSeExpectForm::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{

	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}

VOID CSeExpectForm::CreateChildForm()
{
	m_pDateForm = new CSeExpectAllDateForm(this);
	m_pVolTitleForm1 = new CSeExpectVoltileForm(this);
	m_pVolTitleForm2 = new CSeExpectVoltileForm(this);
	m_pAppendixForm = new CSeExpectAppendixForm(this);

	
	//일자 예측에 간행빈도를 setting한다.
	if(!m_strPUB_FREQ.IsEmpty())
		m_pDateForm->m_charPUB_FREQ = m_strPUB_FREQ.GetAt(0);
	
	m_pVolTitleForm1->m_strFORMAT_TYPE = _T("1"); //권호 
	m_pVolTitleForm2->m_strFORMAT_TYPE = _T("2"); //부차적 권호
	

	//set open mode
	m_pDateForm->m_nOpenMode = m_nOpenMode;
	m_pVolTitleForm1->m_nOpenMode = m_nOpenMode;
	m_pVolTitleForm2->m_nOpenMode = m_nOpenMode;
	m_pAppendixForm->m_nOpenMode = m_nOpenMode;

	
	//수정시 개별 화면에 필요한 정보들 
	if(m_nOpenMode<0)
	{
		m_pDateForm->m_strPUB_EXP_KEY = m_strPUB_EXP_KEY;
		m_pDateForm->m_strACQ_EXP_KEY = m_strACQ_EXP_KEY;
		m_pDateForm->m_strPUB_EXP_TYPE = m_strPUB_EXP_TYPE;
		m_pDateForm->m_strACQ_EXP_TYPE = m_strACQ_EXP_TYPE;
		
		//수입구분KEY
		m_pVolTitleForm1->m_strACQ_KEY = m_strACQ_KEY;
		m_pVolTitleForm2->m_strACQ_KEY = m_strACQ_KEY;
		m_pAppendixForm->m_strACQ_KEY = m_strACQ_KEY;

	}

	m_pDateForm->m_bIsFirstCreated = m_bIsFirstCreated; //초기화 여부 
	m_pDateForm->m_bIsVerified = m_bIsVerified; // 검증 여부


	//loveisverb
	m_pDateForm->m_bIsParentHaveData = m_bIsParentHaveData; //부모 변수 가져감
	m_pVolTitleForm1->m_bIsFirstCreated = m_bIsFirstCreated; //초기화 여부 
	m_pVolTitleForm2->m_bIsFirstCreated = m_bIsFirstCreated; //초기화 여부 
	m_pAppendixForm->m_bIsFirstCreated = m_bIsFirstCreated; //초기화 여부 


	m_pDateForm->Create(this);
	m_pVolTitleForm1->Create(this);
	m_pVolTitleForm2->Create(this);
	m_pAppendixForm->Create(this);
	
	CRect rect;
	this->GetClientRect(rect);
	
	rect.top += 60;
	rect.bottom -= 40;
	rect.left += 10;
	rect.right -= 10;

	m_pDateForm->MoveWindow(rect);
	m_pVolTitleForm1->MoveWindow(rect);
	m_pVolTitleForm2->MoveWindow(rect);
	m_pAppendixForm->MoveWindow(rect);
	
	m_pDateForm->ShowWindow(SW_SHOW);
	m_pVolTitleForm1->ShowWindow(SW_HIDE);
	m_pVolTitleForm2->ShowWindow(SW_HIDE);
	m_pAppendixForm->ShowWindow(SW_HIDE);

}


VOID CSeExpectForm::FreeChildForm()
{
	if(m_pDateForm) delete m_pDateForm;
	m_pDateForm = NULL;
	
	if(m_pVolTitleForm1) delete m_pVolTitleForm1;
	m_pVolTitleForm1 = NULL;

	if(m_pVolTitleForm2) delete m_pVolTitleForm2;
	m_pVolTitleForm2 = NULL;

	if(m_pAppendixForm) delete m_pAppendixForm;
	m_pAppendixForm = NULL;

}

VOID CSeExpectForm::OnSelchangetabEXPECT(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	INT nCurSel = m_ctrlTAB.GetCurSel();
	switch (nCurSel)
	{

	case 0:
			m_pDateForm->ShowWindow(SW_SHOW);
			m_pVolTitleForm1->ShowWindow(SW_HIDE);
			m_pVolTitleForm2->ShowWindow(SW_HIDE);
			m_pAppendixForm->ShowWindow(SW_HIDE);
			break;

	case 1:
			m_pDateForm->ShowWindow(SW_HIDE);
			m_pVolTitleForm1->ShowWindow(SW_SHOW);
			m_pVolTitleForm2->ShowWindow(SW_HIDE);
			m_pAppendixForm->ShowWindow(SW_HIDE);
			break;

	case 2:
			m_pDateForm->ShowWindow(SW_HIDE);
			m_pVolTitleForm1->ShowWindow(SW_HIDE);
			m_pVolTitleForm2->ShowWindow(SW_SHOW);
			m_pAppendixForm->ShowWindow(SW_HIDE);
			break;

	case 3:
			m_pDateForm->ShowWindow(SW_HIDE);
			m_pVolTitleForm1->ShowWindow(SW_HIDE);
			m_pVolTitleForm2->ShowWindow(SW_HIDE);
			m_pAppendixForm->ShowWindow(SW_SHOW);
			break;
	}	
	

	*pResult = 0;
}

VOID CSeExpectForm::OnOK() 
{
	// TODO: Add extra validation here
	INT ids = 0;

	if(1 == m_nOpenMode ) //종화면 입력 
	{
		//DM에 일자 예측 정보를 키를 넣지 않고 저장한 후    
		m_pDateForm->SaveAllDataToDM(_T(""),_T(""));

		//데이 터를 검증 한후 
		if(VerifyAllData(m_pDateForm->m_nAcqCurSel,m_pDateForm->m_nPubCurSel)<0) {
			return;
		}

		//일자 예측의 기본 정보를 저장한다 
		m_nPubType = m_pDateForm->m_nPubCurSel;
		m_nAcqType = m_pDateForm->m_nAcqCurSel;

		//현재의 데이터를 종화면에 복사해준다 
		ExchangeDM(1);

	}
	else if(-1 == m_nOpenMode) //종화면 수정모드 
	{
	
		//DM에 일자 예측정보를 키를 넣고 저장한 후 
		m_pDateForm->SaveAllDataToDM(m_strPUB_EXP_KEY,m_strACQ_EXP_KEY);
		
		//데이터를 검증한다 
		if(VerifyAllData(m_pDateForm->m_nAcqCurSel,m_pDateForm->m_nPubCurSel)<0) {
			return; 
		}
		
		//일자 예측의 기본 정보를 저장한다 
		m_nPubType = m_pDateForm->m_nPubCurSel;
		m_nAcqType = m_pDateForm->m_nAcqCurSel;

		//loveisverb
		//현재의 데이터를 종화면에 복사해준다 
		ExchangeDM(1);
	
		m_bIsParentHaveData = TRUE;
		if(m_pDateForm) m_pDateForm->m_bIsParentHaveData = m_bIsParentHaveData;

	}
	else if(-2 == m_nOpenMode) //일반 수정모드 
	{
	
		ids = SaveAllDataForGeneralType();
		if(-1 == ids) return;
	}


	CDialog::OnOK();
}

INT CSeExpectForm::SaveAllData(INT nPubExpType, INT nAcqExpType)
{
	m_pDM->StartFrame();
	m_pDM->InitDBFieldData();
	


	//수정시에는 먼저 입수 예측유형에 따라 delete 한다.
	if(!m_strPUB_EXP_TYPE.IsEmpty())
		m_pDateForm->DeleteOldData(m_pDM,m_strPUB_EXP_KEY,m_strPUB_EXP_TYPE,_T("1"));
	
	if(!m_strACQ_EXP_TYPE.IsEmpty())
		m_pDateForm->DeleteOldData(m_pDM,m_strACQ_EXP_KEY,m_strACQ_EXP_TYPE,_T("0"));


	CString strACQ_TYPE;
	if(1 == m_nACQ_TYPE) strACQ_TYPE = _T("1");
	if(2 == m_nACQ_TYPE) strACQ_TYPE = _T("2");

	//test
	m_pDateForm->SaveAllData(m_pDM,nPubExpType,nAcqExpType);
	m_pVolTitleForm1->SaveAllData(m_pDM,strACQ_TYPE, m_strACQ_KEY);
	m_pVolTitleForm2->SaveAllData(m_pDM,strACQ_TYPE, m_strACQ_KEY);
	m_pAppendixForm->SaveAllData(m_pDM,strACQ_TYPE, m_strACQ_KEY);
	
	INT ids = 0;
	ids = m_pDM->SendFrame();
	m_pDM->EndFrame();

	return ids;
}

INT CSeExpectForm::SaveAllDataForGeneralType()
{
	m_pDM->StartFrame();
	m_pDM->InitDBFieldData();
	
	
	//수정시에는 먼저 입수 예측유형에 따라 delete 한다.
	if(!m_strPUB_EXP_TYPE.IsEmpty())
		m_pDateForm->DeleteOldData(m_pDM,m_strPUB_EXP_KEY,m_strPUB_EXP_TYPE,_T("1"));
	
	if(!m_strACQ_EXP_TYPE.IsEmpty())
		m_pDateForm->DeleteOldData(m_pDM,m_strACQ_EXP_KEY,m_strACQ_EXP_TYPE,_T("0"));

	m_pDateForm->SaveAllDataToDM(m_strPUB_EXP_KEY,m_strACQ_EXP_KEY);
	

	//데이터를 검증한다 
	if(VerifyAllData(m_pDateForm->m_nAcqCurSel,m_pDateForm->m_nPubCurSel)<0) {
		m_pDM->EndFrame();
		return -1; //검증 실패 
	}

	CString strACQ_TYPE;
	if(1 == m_nACQ_TYPE) strACQ_TYPE = _T("1");
	if(2 == m_nACQ_TYPE) strACQ_TYPE = _T("2");

	
	m_pDateForm->SaveAllData(m_pDM);
	m_pVolTitleForm1->SaveAllData(m_pDM,strACQ_TYPE, m_strACQ_KEY);
	m_pVolTitleForm2->SaveAllData(m_pDM,strACQ_TYPE, m_strACQ_KEY);
	m_pAppendixForm->SaveAllData(m_pDM,strACQ_TYPE, m_strACQ_KEY);
		
				
	//일자 예측 TYPE 갱신(해당 수입 구분 정보에 UPDATE를 날린다)
	CString strQuery;
	m_strPUB_EXP_TYPE = GetExpType(m_pDateForm->m_nPubCurSel); 
	m_strACQ_EXP_TYPE = GetExpType(m_pDateForm->m_nAcqCurSel); 

	if(1 == m_nACQ_TYPE)
	{
		strQuery.Format(_T("UPDATE SE_PURCHASE_MANAGE_TBL SET PUB_EXP_TYPE = '%s', ACQ_EXP_TYPE = '%s' WHERE REC_KEY = %s;")
		,m_strPUB_EXP_TYPE,m_strACQ_EXP_TYPE,m_strACQ_KEY);
	}
	else if(2 == m_nACQ_TYPE)
	{
		strQuery.Format(_T("UPDATE SE_DONATE_MANAGE_TBL SET PUB_EXP_TYPE = '%s', ACQ_EXP_TYPE = '%s' WHERE REC_KEY = %s;")
		,m_strPUB_EXP_TYPE,m_strACQ_EXP_TYPE,m_strACQ_KEY);
	}
	
	m_pDM->AddFrame(strQuery);

	INT ids = 0;
	ids = m_pDM->SendFrame();
	m_pDM->EndFrame();


	if(ids>=0)			
	{
		//종화면에서의 UPDATE시를 위한 예측 TYPE 갱신
		m_nPubType = m_pDateForm->m_nPubCurSel; 
		m_nAcqType = m_pDateForm->m_nAcqCurSel; 
		AfxMessageBox(_T("저장하였습니다!"));
		return 0;
	}

	AfxMessageBox(_T("저장에 실패 하였습니다!"));
	return -2; //쿼리 실패
}

CString CSeExpectForm::GetExpType(INT ExpType)
{
	CString strType = _T("");
	
	switch(ExpType)
	{
	case 0:
		strType = _T("M");
		break;
	case 1:
		strType = _T("W");
		break;
	case 2:
		strType = _T("D");
		break;
	case 3:
		strType = _T("E");
		break;
	default:
		break;
	}

	return strType;
}

INT CSeExpectForm::GetIntExpType(CString strExpType)
{
	
	switch(strExpType.GetAt(0))
	{
	case 'M':
		return 0;
		break;
	case 'W':
		return 1;
		break;
	case 'D':
		return 2;
		break;
	case 'E':
		return 3;
		break;
	default:
		break;
	}

	return -1;
}

INT CSeExpectForm::VerifyAllData(INT nAcqExpType, INT nPubExpType)
{
	//모든 child Form의 data들을 검증한다 
	CESL_DataMgr* pDM_DATE1;
	CESL_DataMgr* pDM_DATE2;
	
	INT nOption = 1; //입수일예측과 발행일 예측이 틀린 경우 
	if(nAcqExpType == nPubExpType) nOption = 0; //같은 경우


	switch(nAcqExpType)
	{
	case 0: 
		pDM_DATE1 = FindDM(_T("DM_월간예측"));
		break;
	case 1: 
		pDM_DATE1 = FindDM(_T("DM_주간예측"));
		break;
	case 2: 
		pDM_DATE1 = FindDM(_T("DM_일간예측"));
		break;
	case 3: 
		pDM_DATE1 = FindDM(_T("DM_기타예측"));
		break;		
	}
	
	switch(nPubExpType)
	{
	case 0: 
		pDM_DATE2 = FindDM(_T("DM_월간예측"));
		break;
	case 1: 
		pDM_DATE2 = FindDM(_T("DM_주간예측"));
		break;
	case 2: 
		pDM_DATE2 = FindDM(_T("DM_일간예측"));
		break;
	case 3: 
		pDM_DATE2 = FindDM(_T("DM_기타예측"));
		break;		
	}

	
	CSeExpectVerifier api;
	INT ids1 = 0; 
	INT ids2 = 0;
	INT ids3 = 0;
	INT ids4 = 0;
	INT ids5 = 0;	

	if(nOption) {
		
		ids1 = api.VerifyDateExpData(pDM_DATE1,nAcqExpType,0); 
		ids2 = api.VerifyDateExpData(pDM_DATE2,nPubExpType,0); 

	} else {

		ids1 = api.VerifyDateExpData(pDM_DATE1,nAcqExpType,1); 
		ids2 = api.VerifyDateExpData(pDM_DATE1,nAcqExpType,0); 
	
	}
	
	ids3 = api.VerifyAppendixExpData(m_pAppendixForm->m_pDM);
	ids4 = api.VerifyVolExpData(m_pVolTitleForm1->m_pDM);
	ids5 = api.VerifyVolExpData(m_pVolTitleForm2->m_pDM);
	

	if(ids1<0 || ids2<0 || ids3<0 || ids4<0 || ids5<0) return -1;

	return 0;
}

VOID CSeExpectForm::CopyDM(CString strSource,CString strTarget)
{
	CESL_DataMgr* pDM_TARGET = FindDM(strTarget);
	CESL_DataMgr* pDM_SOURCE = FindDM(strSource);

	if(!pDM_TARGET || !pDM_SOURCE) {
		CString msg;
		msg.Format(_T(" [%s] or [%s] DM 을 찾을 수 없습니다!"),strTarget,strSource);
		AfxMessageBox(msg);
	}

	pDM_TARGET->FreeData();
	
	for(INT i=0;i<pDM_SOURCE->GetRowCount();i++)
	{
		pDM_TARGET->InsertRow(-1);
		CopyDMToDMByAlias(pDM_SOURCE,i,pDM_TARGET,i);
	}
}
	
		
VOID CSeExpectForm::ExchangeDM(INT nOption)
{
	CESL_DataMgr* pDM_TARGET = NULL;
	CESL_DataMgr* pDM_SOURCE = NULL;
	
	
	//권호, 부차적 권호, 부록 예측정보
	const INT cnt = 3;
	CString strDM1[cnt] = {_T("DM_연속_예측_권호"),_T("DM_연속_예측_부차적권호"),_T("DM_연속_예측_부록")};
	CString strDM2[cnt] = {_T("DM_연속_종_예측_권호"),_T("DM_연속_종_예측_부차적권호"),_T("DM_연속_종_예측_부록")};
	
	if(2 == nOption) { //데이터 받기 
	
		for(INT i=0;i<cnt;i++) 	{
			CopyDM(strDM2[i],strDM1[i]);
		}
	
	} else if(1 == nOption) { //데이터 주기
		
		for(INT i=0;i<cnt;i++) 	{
			CopyDM(strDM1[i],strDM2[i]);
		}
	}

	//일자예측정보 
	CString strDMSpecies = _T("");
	CString strDMExpect = _T("");
	
	if(m_nPubType<0) return; //예측유형 정보가 없으면 return

	switch(m_nPubType)
	{
	case 0:
		strDMExpect = _T("DM_월간예측");
		strDMSpecies = _T("DM_종_월간예측");
		break;
	case 1:
		strDMExpect = _T("DM_주간예측");
		strDMSpecies = _T("DM_종_주간예측");
		break;
	case 2:
		strDMExpect = _T("DM_일간예측");
		strDMSpecies = _T("DM_종_일간예측");
		break;	
	case 3:
		strDMExpect = _T("DM_기타예측");
		strDMSpecies = _T("DM_종_기타예측");
		break;
	default:
		break;
	}

	if(2 == nOption)  //데이터 받기
		CopyDM(strDMSpecies,strDMExpect);
	else  // 데이터 주기 
		CopyDM(strDMExpect,strDMSpecies);
	
	

	//발행일 예측 유형과 입수일 예측 유형이 일치하지 않을 경우 추가 데이터 교환 
	if(m_nPubType != m_nAcqType)
	{
		switch(m_nAcqType)
		{
		case 0:
			strDMExpect = _T("DM_월간예측");
			strDMSpecies = _T("DM_종_월간예측");
			break;
		case 1:
			strDMExpect = _T("DM_주간예측");
			strDMSpecies = _T("DM_종_주간예측");
			break;
		case 2:
			strDMExpect = _T("DM_일간예측");
			strDMSpecies = _T("DM_종_일간예측");
			break;	
		case 3:
			strDMExpect = _T("DM_기타예측");
			strDMSpecies = _T("DM_종_기타예측");
			break;
		default: 
			break;
		}

		if(2 == nOption)  //데이터 받기
			CopyDM(strDMSpecies,strDMExpect);
		else  // 데이터 주기 
			CopyDM(strDMExpect,strDMSpecies);
		
	}
	
}

HBRUSH CSeExpectForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
