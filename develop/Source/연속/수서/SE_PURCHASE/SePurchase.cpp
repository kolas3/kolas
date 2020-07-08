// SePurchase.cpp : implementation file
//

#include "stdafx.h"
#include "SePurchase.h"
#include "..\SE_API\SeApi.h"
#include "..\SE_API\SePurchaseNoDlg.h"
#include "SeMarcImport.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSePurchase dialog


CSePurchase::CSePurchase(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSePurchase::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSePurchase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pDM = NULL;
	m_pParentDM = NULL;	
	m_pDM_SEQ_NO = NULL;	
	m_pDM_WORK_NO = NULL;	
	m_pParentGrid = NULL;
	m_pCM = NULL;
	m_nOpenMode = 1; //default
	m_Marc.Init();
	m_nCurrentIdx = -1;
	m_bIsImported = FALSE;
	m_bIsWorkNoChanged = FALSE;
	m_bIsNewWorkNo = FALSE;
	m_nMoveOption = 1; //default

	m_bIsLightVersion = FALSE;
	m_bIsDepartMngUse = FALSE;

	m_pDepartmentOrder = NULL;

}


CSePurchase::~CSePurchase()
{
	if(m_pDepartmentOrder)
	{
		delete m_pDepartmentOrder;
		m_pDepartmentOrder = NULL;
	}
}

VOID CSePurchase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSePurchase)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSePurchase, CDialog)
	//{{AFX_MSG_MAP(CSePurchase)
	ON_CBN_SELCHANGE(IDC_cmbSPE_KDCDDDC, OnSelchangecmbSPEKDCDDDC)
	ON_BN_CLICKED(IDC_btnMARC_IMPORT, OnbtnMARCIMPORT)
	ON_BN_CLICKED(IDC_btnALL_CLEAR, OnbtnALLCLEAR)
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnSPE_CREATE, OnbtnSPECREATE)
	ON_EN_KILLFOCUS(IDC_edtSPE_ORDER_NO, OnKillfocusedtSPEORDERNO)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnDEPARTMNG, OnbtnDEPARTMNG)
	ON_MESSAGE(BOOKCNTCHANGE, OnDepartmentBookCntChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSePurchase message handlers

BOOL CSePurchase::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr( _T("연속_수서_구입종") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_구입종") );
		return false;
	}
	
	m_pDM = FindDM(_T("DM_연속_구입종"));
	m_pDM_SEQ_NO = FindDM(_T("DM_연속_구입종_차수번호")); 
	m_pDM_WORK_NO = FindDM(_T("DM_연속_구입종_작업번호"));
	m_pCM = FindCM(_T("CM_연속_구입종"));
	

	if(!m_pDM || !m_pCM || !m_pDM_SEQ_NO || !m_pDM_WORK_NO) {
		AfxMessageBox( _T("DM,CM Error : 연속_수서_구입종") );
		return false;
	}
	
	InitControls();
	
	if(InitOpenMode()<0) {
		EndDialog(0);
		return FALSE;
	}

	if(m_bIsLightVersion)
	{
		SetLightVersion();
	}

	if(!m_bIsDepartMngUse)
	{
		SetDepartMngNoUse();
	}
	else
	{
		CreateDepartMngDlg();
	
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSePurchase::SetLightVersion()
{
	//라이트 버젼의 경우 등록 책수 콘트롤을 없앤다 
	CWnd* pWnd = NULL;

	pWnd = GetDlgItem(IDC_stcPUR_REG_BOOK_CNT);
	if(pWnd) pWnd->ShowWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_edtPUR_REG_BOOK_CNT);
	if(pWnd) pWnd->ShowWindow(FALSE);
}

VOID CSePurchase::SetDepartMngNoUse()
{
	//부서 지정기능을 사용하지 않는 경우 발행부수와 부서지정 버튼을 감춘다 
	CWnd* pWnd = NULL;

	pWnd = GetDlgItem(IDC_stcPUR_ISSUE);
	if(pWnd) pWnd->ShowWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_edtPUR_ISSUE);
	if(pWnd) pWnd->ShowWindow(FALSE);	

	pWnd = GetDlgItem(IDC_btnDEPARTMNG);
	if(pWnd) pWnd->ShowWindow(FALSE);

}

VOID CSePurchase::CreateDepartMngDlg()
{
	m_pDepartmentOrder = new CSeDepartmentOrder(this);
	if(!m_pDepartmentOrder) return;

	m_pDepartmentOrder->m_nOpenMode = m_nOpenMode;
	m_pDepartmentOrder->m_nOpenLockMode = 1;

	if(m_nOpenMode<0)
	{
		//부서별 자료지정 
		CString strPURCHASE_SPECIES_KEY;
		CString stPURCHASE_MANAGE_KEY;
		strPURCHASE_SPECIES_KEY = m_pDM->GetCellData(_T("REC_KEY"),0);

		
		CString strSPECIES_KEY;	
		CString strQuery;
		strSPECIES_KEY = m_pDM->GetCellData(_T("SPECIES_KEY"),0);


		if(!strSPECIES_KEY.IsEmpty())
		{
			strQuery.Format(_T("SELECT REC_KEY FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);

			m_pDM->GetOneValueQuery(strQuery,stPURCHASE_MANAGE_KEY);
		}


		m_pDepartmentOrder->m_strPURCHASE_SPECIES_KEY = strPURCHASE_SPECIES_KEY;
		m_pDepartmentOrder->m_strPURCHASE_MANAGE_KEY = stPURCHASE_MANAGE_KEY;

	}

	m_pDepartmentOrder->Create((CESL_Mgr*)this);
	m_pDepartmentOrder->CenterWindow();
	m_pDepartmentOrder->ShowWindow(FALSE);

	//수정시일 경우는 부서별 입수 책수가 존재할 경우 
	//입수 책수를 수정 하지 못하도록 한다
	INT nAcqExpBookCnt = 0;
	if(m_nOpenMode<0)
	{
		m_pDepartmentOrder->RefreshAll();
		nAcqExpBookCnt = m_pDepartmentOrder->GetTotalAcqExpBookCnt(FALSE);
		if(nAcqExpBookCnt>0)
		{
			CString strCnt;
			CEdit* pEdit = (CEdit*)m_pCM->FindControl(_T("입수책수"));
			pEdit->EnableWindow(FALSE);
			

		}
	}
}


VOID CSePurchase::InitControls()
{
	
	//control color
	AddColorControl(IDC_edtSPE_ORDER_NO, _T(""), _T(""), 'E');		
	AddColorControl(IDC_edtSPE_TITLE1, _T(""), _T(""), 'E');
	AddColorControl(IDC_edtSPE_PUBLISHER, _T(""), _T(""), 'E');
	AddColorControl(IDC_edtPUR_ACQ_BOOK_CNT, _T(""), _T(""), 'E');
	
	/*
	AddColorControl(IDC_edtPUR_START_DATE, _T("CM_연속_구입종"), _T("구독시작일"), 'D');
	AddColorControl(IDC_edtPUR_END_DATE, _T("CM_연속_구입종"), _T("구독만기일"), 'D');
	*/

	AddColorControl(IDC_cmbSPE_CONTROL_TYPE, _T("CM_연속_구입종"), _T("제어자료구분_코드"), 'M');
	AddColorControl(IDC_cmbSPE_PUB_FREQ, _T("CM_연속_구입종"), _T("간행빈도"), 'M');

	//-------------------------------------------------------------------
	// 2005-10-11 황정영: 학처 학과는 육사가 아니면 숨긴다.
	//-------------------------------------------------------------------
	CString str64;
	GetManageValue( _T("기타"), _T("공통"), _T("육사"), _T("육사"), str64 );
	if ( str64 == _T("Y") ) 
	{
		GetDlgItem(IDC_STATIC_DEPT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbDept)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_COURSE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbCource)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_STATIC_DEPT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbDept)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_COURSE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbCource)->ShowWindow(SW_HIDE);
	}


	if(m_nOpenMode>0) //입력 
	{
		//다음 버튼을 감춘다 
		//GetDlgItem(IDC_btnPREV)->ShowWindow(SW_HIDE);
		//2005-10-13 황정여 추가
		GetDlgItem(IDC_btnPREV)->SetWindowText(_T("이전 정보"));
		GetDlgItem(IDC_btnNEXT)->ShowWindow(SW_HIDE);


		//입력 분류기호 처리
		OnSelchangecmbSPEKDCDDDC();

		//2005-09-27 황정영: 입력시 제일 높은 차수번호로 초기화한다.
		CString strSeq_No, strSeq_YEAR, strQuery;
		strSeq_No.Empty();
		strSeq_YEAR.Empty();

		GetDlgItem(IDC_edtSPE_ORDER_YEAR)->GetWindowText(strSeq_YEAR);
		strQuery.Format(_T("SELECT MAX(SEQ_NO) from SE_PURCHASE_SEQ_NO_TBL WHERE working_status = '0' and acq_year = '%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), strSeq_YEAR);
		m_pDM_SEQ_NO->GetOneValueQuery(strQuery,strSeq_No);
		GetDlgItem(IDC_edtSPE_ORDER_NO)->SetWindowText(strSeq_No);

	}
	else
	{
		//차수 입력을 차수 수정 버튼으로 만든다 
		GetDlgItem(IDC_btnSPE_CREATE)->SetWindowText(_T("차수 수정"));
		
		//차수 항목을 read only화 한다  
		CEdit* pEdit;
		pEdit = (CEdit*)GetDlgItem(IDC_edtSPE_ORDER_NO);
		pEdit->SetReadOnly();

		
		//자료반입,화면지우기  버튼을 감춘다
		GetDlgItem(IDC_btnMARC_IMPORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnALL_CLEAR)->ShowWindow(SW_HIDE);

	}

	AddColorControl(IDC_edtSPE_ORDER_NO, _T(""), _T(""), 'E');		
	AddColorControl(IDC_edtSPE_TITLE1, _T(""), _T(""), 'E');
	AddColorControl(IDC_edtSPE_PUBLISHER, _T(""), _T(""), 'E');
	AddColorControl(IDC_edtPUR_ACQ_BOOK_CNT, _T(""), _T(""), 'E');
	

}

INT CSePurchase::InitOpenMode()
{

	
	if(m_nOpenMode>0) //입력 
	{
		
			
	
	}
	else //수정 
	{
	
		if(!m_pParentDM) {
			AfxMessageBox(_T("부모 DM을 찾을 수 없습니다!"));
			return -1;
		}
		
		if(m_nCurrentIdx<0) {
			AfxMessageBox(_T("선정된 정보가 없습니다!"));
			return -2;
		}

		/*
		//부모 그리드에서 선정정보를 얻어온다
		if(!m_pParentGrid) {
			AfxMessageBox(_T("부모 그리드를 찾을 수 없습니다!"));
			return -1;
		}

		
		m_pParentGrid->SelectMakeList();
		m_nCurrentIdx = m_pParentGrid->SelectGetHeadPosition();
		if(m_nCurrentIdx<0) {
			AfxMessageBox(_T("선정된 정보가 없습니다!"));
			return -3;
		}
		*/

		CString strREC_KEY;
		strREC_KEY = m_pParentDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);
		
		if(!strREC_KEY.IsEmpty())
			RefreshAll(strREC_KEY);

	}



	return 0;
}
//======================================================================
// [함수설명]
//  입력된 구입정보를 저장한다.
// 
// [MINI SPEC] 
// 1. 서명/발행자/차수번호/간행빈도/입수책수를 콘트롤에서 가져온다.
// 2. 데이터를 DM에 저장한다. 
//======================================================================

VOID CSePurchase::OnOK() 
{
	//==================================================================
	// 1. 서명/발행자/차수번호/간행빈도/입수책수를 콘트롤에서 가져온다.
	//==================================================================
	CString strTitle;
	m_pCM->GetControlMgrData(_T("서명"),strTitle);

	if(strTitle.IsEmpty()) 
	{
		AfxMessageBox(_T("서명이 입력되지 않았습니다!"));
		return;
	}

	//발행자(260)
	CString strPublisher;
	m_pCM->GetControlMgrData(_T("발행자"),strPublisher);

	if(strPublisher.IsEmpty()) 
	{
		AfxMessageBox(_T("발행자가 입력되지 않았습니다!"));
		return;
	}
	
	CString strSEQ_NO;
	m_pCM->GetControlMgrData(_T("차수번호"),strSEQ_NO);

	if(	strSEQ_NO.IsEmpty())
	{	
		AfxMessageBox(_T("차수번호가 입력되지 않았습니다!"));
		return;
	}
	
	CString strPUB_FREQ;
	m_pCM->GetControlMgrData(_T("간행빈도"),strPUB_FREQ);
	strPUB_FREQ.TrimLeft();
	strPUB_FREQ.TrimRight();

	if(strPUB_FREQ.IsEmpty())
	{
		AfxMessageBox(_T("간행빈도가 입력되지 않았습니다!"));
		return;
	}
	
	CString strACQ_BOOK_CNT;
	INT nACQ_BOOK_CNT = 0;
	m_pCM->GetControlMgrData(_T("입수책수"),strACQ_BOOK_CNT);
	if(strACQ_BOOK_CNT.IsEmpty())
	{
		AfxMessageBox(_T("입수책수가 입력되지 않았습니다!"));
		return;
	}

	nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
	if(nACQ_BOOK_CNT<1)
	{
		AfxMessageBox(_T("입수책수는 1 이상이어야 합니다!"));
		return;
	}
	
	//==================================================================
	// 2. 데이터를 DM에 저장한다.
	//==================================================================
	SaveAllDataToDM();

	INT ids = 0;
	CSeApi api(this);

	if(m_nOpenMode>0)
	{	
		ids = api.SPECIES_PUR_OBJ_INSERT(m_pDM,
										 &m_Marc,
										 m_pDM_SEQ_NO,
										 m_pDM_WORK_NO,
										 m_bIsNewWorkNo,
										 0,
										 m_bIsDepartMngUse);
	}
	else 
	{
		
		ids = api.SPECIES_PUR_OBJ_UPDATE(m_pDM,
										 &m_Marc,
										 m_pDM_SEQ_NO,
										 m_pDM_WORK_NO,
										 m_bIsNewWorkNo,
										 m_bIsWorkNoChanged,
										 0,
										 m_bIsDepartMngUse);

	}
	

	if(m_bIsDepartMngUse)
	{
		SaveDepartmentOrderInfo();
	}

	RefreshParent();


	if(ids<0)
	{
		AfxMessageBox(_T("저장에 실패하였습니다!"));
	}
	else 
	{
		AfxMessageBox(_T("저장하였습니다!"));
		
		//member variables initialize
		m_bIsWorkNoChanged = FALSE;
		m_bIsNewWorkNo = FALSE;
	
	}
	
	if(m_nOpenMode>0) //입력  

	{
		m_pDM->FreeData(); 
		m_pCM->AllControlClear();
		
		//2005-09-27
		//입력차수번호는 그대로 유지한다.
		GetDlgItem(IDC_edtSPE_ORDER_NO)->SetWindowText(strSEQ_NO);

		m_Marc.Init();
		m_bIsImported = FALSE;
		m_bIsWorkNoChanged = FALSE;
		m_bIsNewWorkNo = FALSE;
		
		CString str;
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSPE_WORK_NO);
		if(!pCombo) return;
		
		pCombo->GetWindowText(str);
		if( str == _T("새번호") )
		{
			INT i = pCombo->GetCurSel();
			if( i > 0)
				pCombo->SetCurSel(i-1);
		}
	//	pCombo->ResetContent();

		return;
	}
	else //수정
	{
		
		return;
	}

	CDialog::OnOK();
}

VOID CSePurchase::RefreshParent()
{
	//부모 DM에 현재의 DM과 마크를 copy해주고 grid를 display한다
	INT idx = -1;
	CString strPROVIDER_CODE;
	CString strMANAGE_CODE;
	CString strMAT_CDOE;
	CString strDesc;
	CString strStreamMarc;

	if(m_nOpenMode>0) //입력 
	{
		m_pParentDM->InsertRow(-1);
		idx = m_pParentDM->GetRowCount()-1;

		//test
		m_pParentDM->SetCellData(_T("UDF_차수작업상태"),_T("구입대상"),idx);
	}	
	else
	{
		idx = m_nCurrentIdx;
	}
	
	
	CopyDMToDMByAlias(m_pDM,0,m_pParentDM,idx);
	m_pInfo->pMarcMgr->Encoding(&m_Marc, strStreamMarc);
	m_pParentDM->SetCellData(_T("MARC"),strStreamMarc,idx);

	
	//구입처
	strPROVIDER_CODE = m_pDM->GetCellData(_T("구입처_코드"),0);
	m_pParentDM->SetCellData(_T("구입처_코드"),strPROVIDER_CODE,idx);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("구입처관리"),strPROVIDER_CODE,strDesc);
	m_pParentDM->SetCellData(_T("UDF_구입처"),strDesc,idx);


	//관리구분
	strMANAGE_CODE = m_pDM->GetCellData(_T("관리구분_코드"),0);
	m_pParentDM->SetCellData(_T("관리구분_코드"),strMANAGE_CODE,idx);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("관리구분"),strMANAGE_CODE,strDesc);
	m_pParentDM->SetCellData(_T("UDF_관리구분"),strDesc,idx);


	//자료구분
	strMAT_CDOE = m_pDM->GetCellData(_T("자료구분_코드"),0);
	m_pParentDM->SetCellData(_T("자료구분_코드"),strMAT_CDOE,idx);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("자료구분"),strMAT_CDOE,strDesc);
	m_pParentDM->SetCellData(_T("UDF_자료구분"),strDesc,idx);

	m_pParentGrid->SetProtectSelectColumn(TRUE);
	//m_pParentGrid->DisplayLine(idx);
	m_pParentGrid->Display();
	m_pParentGrid->SetReverse(idx);
}

INT CSePurchase::RefreshAll(CString strREC_KEY)
{
	
	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"),strREC_KEY);

	m_pDM->RefreshDataManager(strWhere);
	m_pCM->AllControlDisplay(0);
	

	//마크구조체를 decoding 한다.
	m_Marc.Init();
	CString strStreamMarc;
	strStreamMarc = m_pDM->GetCellData(_T("MARC"),0);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_Marc);	


	//ISSN
	CString tmp = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("022$a"),tmp);
	INT npos = tmp.Find(_T("-"));
	if(npos>0) {
		CString strISSN1,strISSN2;
		strISSN1 = tmp.Left(npos);
		strISSN2 = tmp.Mid(npos+1);
		
		strISSN1.TrimLeft(); 	strISSN1.TrimRight();
		strISSN2.TrimLeft(); 	strISSN2.TrimRight();
		
		m_pCM->SetControlMgrData(_T("ISSN1"),strISSN1);
		m_pCM->SetControlMgrData(_T("ISSN2"),strISSN2);
	}
	
	//간행빈도가 빈칸일 경우는 부정기간(_)로 표시한다.
	CString strPUB_FREQ = _T("");
	

	//간행빈도(부정기간처리)
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@18"),strPUB_FREQ);
	
	strPUB_FREQ.TrimLeft();
	strPUB_FREQ.TrimRight();

	if(strPUB_FREQ.IsEmpty())
	{
		m_pCM->SetControlMgrData(_T("간행빈도"),_T("_"));
	}

	//분류 기호 처리 
	m_pCM->SetControlMgrData(_T("분류기호구분_코드"),m_pDM->GetCellData(_T("분류기호구분_코드"), 0));
	m_pCM->SetControlMgrData(_T("주제부호_코드"),m_pDM->GetCellData(_T("주제부호_코드"), 0));
	OnSelchangecmbSPEKDCDDDC();

	
	//차수 및 작업번호 처리
	m_bIsWorkNoChanged = FALSE;
	m_bIsNewWorkNo = FALSE;


	CString strSEQ_NO = _T("");
	strSEQ_NO = m_pDM->GetCellData(_T("차수번호"),0);
	if(strSEQ_NO.IsEmpty()) return 0;

	CString strYear = _T("");
	strYear = m_pDM->GetCellData(_T("차수년도"),0);
	if(strYear.IsEmpty()) return 0;


	CString strWorkNo = _T("");
	strWorkNo = m_pDM->GetCellData(_T("차수내작업번호"),0);
	
	CString strQuery;
	strQuery.Format(_T("ACQ_YEAR = '%s' AND SEQ_NO = %s"),strYear,strSEQ_NO);
	m_pDM_SEQ_NO->RefreshDataManager(strQuery);
	if(0 == m_pDM_SEQ_NO->GetRowCount()) return 0;

	CString strSEQ_NO_KEY = _T("");
	strSEQ_NO_KEY = m_pDM_SEQ_NO->GetCellData(_T("REC_KEY"),0);

	if(_T("0") == strWorkNo || strWorkNo.IsEmpty() ) //작업번호를 사용하지 않는 경우 
	{
		SetWorkNo(_T(""),_T(""),FALSE);
	}
	else //사용하는 경우
	{
		SetWorkNo(strSEQ_NO_KEY,strWorkNo);
	}

	return 0;
}

//======================================================================
// [함수설명]
//  데이터를 DM에 저장한다.
// 
// [MINI SPEC] 
// 1. 입력구분을 체크하여 DM을 FREE하고 마지막에 추가한다.
// 2. DM 저장함수를 호출한다. 
// 3. MARC 저장함를 호출한다.
//======================================================================
INT CSePurchase::SaveAllDataToDM()
{
	//==================================================================
	// 1. 입력구분을 체크하여 DM을 FREE하고 마지막에 추가한다.
	//==================================================================
	if(m_nOpenMode>0 && !m_bIsImported)
	{
		m_pDM->FreeData();
		m_pDM->InsertRow(-1);
	}
	
	//==================================================================
	// 2. DM 저장함수를 호출한다. 
	//==================================================================
	SaveSpecieaData();

	//==================================================================
	// 3. MARC 저장함를 호출한다.
	//==================================================================
	SetMarcByControlData();


	return 0;
}

//======================================================================
// [함수설명]
//  종정보를 DM에 저장한다.
// 
// [MINI SPEC] 
// 1. DM으로 직접 가져올 데이터를 초기화
// 2. 컨트롤로부터 DM으로 데이터를 옮긴다.
//======================================================================
INT CSePurchase::SaveSpecieaData()
{
	
	//==================================================================
	// 1. DM으로 직접 가져올 데이터를 초기화
	//==================================================================
	const INT cnt = 20;
	CString alias[ cnt ] = {
		_T("구독가격"),					//	0
		_T("구입처_코드"),			    //	1
		_T("구독시작일"),				//	2
		_T("구독만기일"),				//	3
		_T("입수책수"),					//	4
		_T("등록책수"),					//	5
		_T("관리구분_코드"),			//	6
		_T("등록구분_코드"),			//	7
		_T("매체구분_코드"),			//	8
		_T("보조등록구분_코드"),		//	9
		_T("이용대상구분_코드"),		// 10
		_T("이용제한구분_코드"),		// 11
		_T("자료구분_코드"),			// 12
		_T("제어자료구분_코드"),		// 13
		_T("형식구분_코드"),            // 14
		_T("주제부호_코드"),            // 15
		_T("분류기호구분_코드"),		// 16	
		_T("발행부수"),					// 17	
		_T("신청학과"),					// 18	
		_T("신청학부"),					// 19	
	};				

	//==================================================================
	// 2. 컨트롤로부터 DM으로 데이터를 옮긴다.
	//==================================================================
	CString tmp;
	for(INT i=0;i<cnt;i++)
	{
		tmp = _T("");	
		m_pCM->GetControlMgrData(alias[i],tmp);
		m_pDM->SetCellData(alias[i],tmp,0);
	}
	
	//==================================================================
	// 3. 입력 구분을 처리한다.
	//==================================================================
	if(m_nOpenMode>0)
	{
		if(m_bIsImported) m_pDM->SetCellData(_T("입력구분"),_T("1"),0); //주문중 
		else m_pDM->SetCellData(_T("입력구분"),_T("0"),0); //화면 입력 
	}
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSPE_WORK_NO);
	if(!pCombo) return 0;
	tmp	= _T("");
	pCombo->GetWindowText(tmp);	
	CString strYear = _T("");
	CString strNo = _T("");

	//차수년도, 차수번호, 작업번호 check
	if(m_nOpenMode>0)
	{
		
		//차수년도 
		m_pCM->GetControlMgrData(_T("차수년도"),strYear);
		m_pDM->SetCellData(_T("차수년도"),strYear,0);
		
		//차수번호 
		m_pCM->GetControlMgrData(_T("차수번호"),strNo);
		m_pDM->SetCellData(_T("차수번호"),strNo,0);


		//입력시에는 새 작업 번호를 선택했는가 여부를 판단한다.
		if(_T("새번호") == tmp) 
		{
			m_bIsNewWorkNo = TRUE;
		}
		else if(_T("사용안함") == tmp)
		{
			m_pDM->SetCellData(_T("차수내작업번호"),_T("0"),0);
		}
		else
		{
			m_pDM->SetCellData(_T("차수내작업번호"),tmp,0);

		}



	}
	else
	{
		//수정시에는 차수년도 혹은 차수번호에 변경사항이 있는지를 check해 작업번호에 반영한다 
		CString strExistingYear;
		CString strExistingSeqNo;
		CString strNewYear;
		CString strNewSeqNo;
		strExistingYear = m_pDM->GetCellData(_T("차수년도"),0);
		strExistingSeqNo = m_pDM->GetCellData(_T("차수번호"),0);
		m_pCM->GetControlMgrData(_T("차수년도"),strNewYear);
		m_pCM->GetControlMgrData(_T("차수번호"),strNewSeqNo);
		
		
		//차수년도 
		m_pDM->SetCellData(_T("차수년도"),strNewYear,0);
		
		//차수번호 
		m_pDM->SetCellData(_T("차수번호"),strNewSeqNo,0);
		

		if(strExistingYear != strNewYear || strExistingSeqNo != strNewSeqNo)
		{
			if(_T("새번호") == tmp) //새 차수번호 중 새번호
			{
				m_bIsNewWorkNo = TRUE;
				return 0;
			}
			else if(_T("사용안함") == tmp) //새 차수번호 중 작업번호 0 
			{
				m_pDM->SetCellData(_T("차수내작업번호"),_T("0"),0);
				m_bIsWorkNoChanged = TRUE;
				return 0;
			}
			else //새 차수번호 중 기존 번호
			{
				m_pDM->SetCellData(_T("차수내작업번호"),tmp,0);
				m_bIsWorkNoChanged = TRUE;
				return 0;
			}

		}
		
		
		
		//수정 시에는 차수내 작업 번호의 변경사항을 check해서 적용해 주어야 한다.
		if(0 == m_pDM_WORK_NO->GetRowCount()) return 0;  //작업번호 사용안함
		
	
		if(_T("새번호") == tmp) 
		{
			m_bIsNewWorkNo = TRUE;
			return 0;
		}
		else
		{
			CString strExistingWorkNo = _T("");
			strExistingWorkNo = m_pDM->GetCellData(_T("차수내작업번호"),0); //기존 작업번호
			
			if(tmp == strExistingWorkNo) //변화 없음 
			{
				return 0; 
			}
			else //변화 있음 
			{
				m_bIsWorkNoChanged = TRUE;
				m_pDM->SetCellData(_T("차수내작업번호"),tmp,0);
				return 0; 
			}
			
			
		}
	}

	return 0;
}

INT CSePurchase::SetMarcByControlData()
{
	//입력과 수정모드에 대한 설정을 자세히 할것!
	//현재는 기종 데이터를 지운후 다시 입력한다
	
	//연속 마크 그룹 설정
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	CString strtmp;
	
	if(m_nOpenMode>0) 
	{
		// 연속 마크 leader 설정
		CSeApi api(this);
		api.SetSerialMarcReader(&m_Marc);
	}

	//기존데이터를 지우고 입력
	const INT cnt = 8;
	CString strtag[cnt] = {
		_T("245$a"),	//0
		_T("245$x"),	//1
		_T("440$a"),	//2
		_T("245$d"),	//3
		_T("260$a"),	//4	
		_T("260$b"),	//5	
		_T("022$a"), 	//6
		_T("008@18")};  //7
	

	for(INT i=0;i<cnt;i++)
	{
		m_pInfo->pMarcMgr->DeleteItem(&m_Marc,strtag[i]);
	}
	
	/*
	m_pCM->GetControlMgrData(_T("서명"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$a"),strtmp);
	strtmp = _T("");
	*/

	m_pCM->GetControlMgrData(_T("서명"),strtmp);
		
	strtmp.TrimLeft();
	strtmp.TrimRight();
		
	if(!strtmp.IsEmpty())
	{
		if(strtmp.GetAt(0) == '(') //관칭이 있을 경우에는 지시기호 20
		{
			m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$a!20"),strtmp);
		}
		else
		{
			m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$a"),strtmp);
		}
	}

	strtmp = _T("");


	m_pCM->GetControlMgrData(_T("대등서명"),strtmp);

	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$x"),strtmp);
	strtmp = _T("");
	
	m_pCM->GetControlMgrData(_T("총서명"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("440$a"),strtmp);
	strtmp = _T("");
	
	m_pCM->GetControlMgrData(_T("저자"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$d"),strtmp);
	strtmp = _T("");

	m_pCM->GetControlMgrData(_T("발행지"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("260$a"),strtmp);
	strtmp = _T("");

	m_pCM->GetControlMgrData(_T("발행자"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("260$b"),strtmp);
	strtmp = _T("");


	//ISSN처리
	CString strISSN,strISSN1,strISSN2;
	m_pCM->GetControlMgrData(_T("ISSN1"),strISSN1);
	m_pCM->GetControlMgrData(_T("ISSN2"),strISSN2);

	if(!strISSN1.IsEmpty() && !strISSN2.IsEmpty())
	{
		strISSN.Format(_T("%s-%s"),strISSN1,strISSN2);
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("022$a"),strISSN);
		strtmp = _T("");
	}
	
	//008 TAG 처리
	////////////////////////////////////////////////////////////////////////////
	
	//입력일자, 길이설정은  입력모드(반입이 아닌 경우)에서만 설정 
	if(m_nOpenMode>0 && !m_bIsImported) 
	{
		//길이 설정 (40자리)
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@39"),_T(" "));
		
		CTime t = CTime::GetCurrentTime();
		CString tmp,inputdate;
		tmp.Format(_T("%04d%02d%02d"),t.GetYear(),t.GetMonth(),t.GetDay());
		inputdate = tmp.Mid(2);
		m_pInfo->pMarcMgr->DeleteItem(&m_Marc,_T("008@0-5"));
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@0-5"),inputdate);
	
		//간행상태(간행중 c)로 표기 
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@6"),_T("c"));

		//정규성(정규 r)로 표기
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@19"),_T("r"));

	}
	
	
	//간행빈도
	m_pCM->GetControlMgrData(_T("간행빈도"),strtmp);

	
	if(_T("_") == strtmp)
	{
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@18"),_T(" "));
	}
	else
	{
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@18"),strtmp);	
	}
	
	//간행빈도(310$a)처리: 설명으로 입력 (008 tag을 따라간다)
	CString strDesc;
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("간행빈도코드"),strtmp,strDesc);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("310$a"),strDesc);

	strtmp = _T("");


	m_pInfo->pMarcMgr->DeleteEmptySubfield(&m_Marc);
	m_pInfo->pMarcMgr->DeleteEmptyField(&m_Marc);
	m_pInfo->pMarcMgr->CheckSimpleSyntax(&m_Marc);

	return 0;
}

INT CSePurchase::SetControlDataByMarc()
{
	//마크반입과 마크 편집기능에서 사용될 함수로 
	//마크에서 콘트롤 데이터를 추출한다 
	

	//연속 마크 그룹 설정
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	CString strtmp;
	
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("245$a"),strtmp);
	m_pCM->SetControlMgrData(_T("서명"),strtmp);
	strtmp = _T("");
	
	
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("245$x"),strtmp);
	m_pCM->SetControlMgrData(_T("대등서명"),strtmp);
	strtmp = _T("");

	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("440$a"),strtmp);
	m_pCM->SetControlMgrData(_T("총서명"),strtmp);
	strtmp = _T("");
	
	
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("245$d"),strtmp);
	m_pCM->SetControlMgrData(_T("저자"),strtmp);
	strtmp = _T("");

	
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("260$a"),strtmp);
	m_pCM->SetControlMgrData(_T("발행지"),strtmp);
	strtmp = _T("");

	
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("260$b"),strtmp);
	m_pCM->SetControlMgrData(_T("발행자"),strtmp);
	strtmp = _T("");

	
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("300$c"),strtmp);
	m_pCM->SetControlMgrData(_T("크기"),strtmp);
	strtmp = _T("");


	//간행빈도
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@18"),strtmp);
	
	strtmp.TrimLeft();
	strtmp.TrimRight();

	if(strtmp.IsEmpty())
	{
		m_pCM->SetControlMgrData(_T("간행빈도"),_T("_"));
	}
	else
	{
		m_pCM->SetControlMgrData(_T("간행빈도"),strtmp);
	}

	strtmp = _T("");


	//ISSN처리
	CString strISSN,strISSN1,strISSN2;
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("022$a"),strISSN);
	INT npos = strISSN.Find(_T("-"));
	if(npos>0)
	{
		strISSN1 = strISSN.Left(npos);
		strISSN2 = strISSN.Mid(npos+1);

		strISSN1.TrimLeft();	strISSN1.TrimRight();
		strISSN2.TrimLeft();	strISSN2.TrimRight();
		
		m_pCM->SetControlMgrData(_T("ISSN1"),strISSN1);
		m_pCM->SetControlMgrData(_T("ISSN2"),strISSN2);

	}


	return 0;
}


INT CSePurchase::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
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

VOID CSePurchase::OnSelchangecmbSPEKDCDDDC() 
{
	// TODO: Add your control notification handler code here
	
	CESL_ControlMultiComboBox *pMultiCombo_Code = (CESL_ControlMultiComboBox *)m_pCM->FindControl(_T("분류기호구분_코드"));
	if (!pMultiCombo_Code) return;
    CESL_ControlMultiComboBox *pMultiCombo_Subject = (CESL_ControlMultiComboBox *)m_pCM->FindControl(_T("주제부호_코드"));
	if (!pMultiCombo_Subject) return;
  
	CString class_name = _T("");
	CString strWhichCode;
	pMultiCombo_Code->GetData(strWhichCode,pMultiCombo_Code->GetCurSel(),1);
	strWhichCode.TrimLeft();		strWhichCode.TrimRight();
	strWhichCode.MakeUpper();
	CString str90;
	GetManageValue(_T("기타"), _T("공통"), _T("국방대"), _T(""), str90);

	if (strWhichCode == _T("KDC")) class_name = _T("KDC대주제부호관리");
	if (strWhichCode == _T("DDC")) class_name = _T("DDC대주제부호관리");
	//2005-11-23 황정영 LC
	if (strWhichCode == _T("LC") && str90 == _T("Y")) class_name = _T("LC주제부호");	

	pMultiCombo_Subject->FreeComboData();
	pMultiCombo_Subject->ResetContent();
	pMultiCombo_Subject->InitMultiComboBox(2);
	if (class_name.GetLength() == 0) return ;
	
	
	INT nCode = 256;
	INT nDefaultValue = 0;
	CString strCode[256], strDesc[256];
	nDefaultValue = 0;
	m_pInfo->pCodeMgr->GetCode(class_name, strCode, strDesc, nCode, nDefaultValue);	
	if (nCode == 0) return;
	

	CString strDefaultValue;
	strDefaultValue	= m_pDM->GetCellData(_T("주제부호_코드"), 0);
	
	for (INT i = 0; i < nCode; i++) {
		if (strCode[i].GetLength() == 0) strCode[i] = _T("  ");
		
		if (strDefaultValue == strCode[i]) nDefaultValue = i;

		
		pMultiCombo_Subject->AddItem(strCode[i], 0, i);
		pMultiCombo_Subject->AddItem(strDesc[i], 1, i);
	}
	
	pMultiCombo_Subject->SetCurSel(nDefaultValue);	

}

VOID CSePurchase::OnbtnMARCIMPORT() 
{
	// TODO: Add your control notification handler code here
	
	//자료 반입 
	CSeMarcImport dlg(this);
	if(dlg.DoModal() != IDOK) return;
	
	m_pDM->FreeData();
	m_pDM->InsertRow(-1);
	
	CopyDMToDMByAlias(dlg.m_pDM,dlg.m_nCurrentIdx,m_pDM,0);
	m_Marc = dlg.m_Marc;
	
	SetControlDataByMarc();
	
	//차수년도,차수번호 
	CString stYear;
	CString strPurchaseNo;

	m_pCM->GetControlMgrData(_T("차수년도"),stYear);
	m_pCM->GetControlMgrData(_T("차수번호"),strPurchaseNo);

	m_pCM->AllControlDisplay(0);

	m_pCM->SetControlMgrData(_T("차수년도"),stYear);
	m_pCM->SetControlMgrData(_T("차수번호"),strPurchaseNo);

	m_bIsImported = TRUE;
	
}
//=====================================================================
// [함수설명]
//  화면지우기 기능을 수행한다.
// [MINI SPEC]
//  1. 차수번호를 임시 저장한다. [2005-10-04] 황정영 수정
//  2. 컨트롤을 clear한다.
//  3. 차수번호를 복구하고 작업번호는 유지한다. [2005-10-04] 황정영 수정
//  
//=====================================================================
VOID CSePurchase::OnbtnALLCLEAR() 
{
	//2005-09-30 차수번호는 남겨놓는다.
	CString strPurchaseNo;
	m_pCM->GetControlMgrData(_T("차수번호"),strPurchaseNo);

	// TODO: Add your control notification handler code here
	m_pCM->AllControlClear();

	m_pCM->SetControlMgrData(_T("차수번호"),strPurchaseNo);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSPE_WORK_NO);
	if(!pCombo) return;
	// pCombo->ResetContent();
}


VOID CSePurchase::BringPrevData()
{
	INT nRow = m_pParentDM->GetRowCount();
	if(nRow<1) return; 
	
	INT nIdx = nRow - 1;

	CString strREC_KEY;
	strREC_KEY = m_pParentDM->GetCellData(_T("REC_KEY"),nIdx);
	
	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"),strREC_KEY);

	m_pDM->RefreshDataManager(strWhere);
	m_pCM->AllControlDisplay(0);
	

	//마크구조체를 decoding 한다.
	m_Marc.Init();
	CString strStreamMarc;
	strStreamMarc = m_pDM->GetCellData(_T("MARC"),0);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_Marc);	


	//ISSN
	CString tmp = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("022$a"),tmp);
	INT npos = tmp.Find(_T("-"));
	if(npos>0) {
		CString strISSN1,strISSN2;
		strISSN1 = tmp.Left(npos);
		strISSN2 = tmp.Mid(npos+1);
		
		strISSN1.TrimLeft(); 	strISSN1.TrimRight();
		strISSN2.TrimLeft(); 	strISSN2.TrimRight();
		
		m_pCM->SetControlMgrData(_T("ISSN1"),strISSN1);
		m_pCM->SetControlMgrData(_T("ISSN2"),strISSN2);
	}


	//분류 기호 처리 
	m_pCM->SetControlMgrData(_T("분류기호구분_코드"),m_pDM->GetCellData(_T("분류기호구분_코드"), 0));
	m_pCM->SetControlMgrData(_T("주제부호_코드"),m_pDM->GetCellData(_T("주제부호_코드"), 0));
	OnSelchangecmbSPEKDCDDDC();

	//데이터를 지운다
	m_pDM->FreeData(); 
	m_Marc.Init();
	m_bIsImported = FALSE;
	m_bIsWorkNoChanged = FALSE;
	m_bIsNewWorkNo = FALSE;

	//차수번호와 작업번호를 지운다
	m_pCM->SetControlMgrData(_T("차수번호"),_T(""));

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSPE_WORK_NO);
	if(!pCombo) return;
	//2005-10-13 황정영 추가
//	pCombo->ResetContent();
}


VOID CSePurchase::OnbtnPREV() 
{
	// TODO: Add your control notification handler code here
	
	//입력모드일 경우는 이전내용 불러오기
	if(m_nOpenMode>0)
	{
		BringPrevData();
		
		//2005-09-27 황정영: 입력시 제일 높은 차수번호로 초기화한다.
		CString strSeq_No, strSeq_YEAR, strQuery;
		strSeq_No.Empty();
		strSeq_YEAR.Empty();
		strQuery.Empty();

		GetDlgItem(IDC_edtSPE_ORDER_YEAR)->GetWindowText(strSeq_YEAR);
		strQuery.Format(_T("SELECT MAX(SEQ_NO) from SE_PURCHASE_SEQ_NO_TBL WHERE working_status = '0' and acq_year = '%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), strSeq_YEAR);
		m_pDM_SEQ_NO->GetOneValueQuery(strQuery,strSeq_No);

		GetDlgItem(IDC_edtSPE_ORDER_NO)->SetWindowText(strSeq_No);

		return;
	}

	
	
	if(1 == m_nMoveOption)
	{
		m_nCurrentIdx--;

		if(m_nCurrentIdx < 0)
		{
			AfxMessageBox(_T("처음 항목입니다!"));
			m_nCurrentIdx++;
			return;
		}
		
	}
	else if(2 == m_nMoveOption)
	{
		INT nIdx = m_pParentGrid->SelectGetPrev();

		if(nIdx<0) 
		{
			INT nHeadIdx = m_pParentGrid->SelectGetHeadPosition();
			
			if(m_nCurrentIdx == nHeadIdx)
			{
				AfxMessageBox(_T("처음 항목입니다!"));
				m_pParentGrid->SelectGetHeadPosition();
				return;
			}
			else //이전 선정 정보가 없을 경우 맨 앞의 선정정보를 택한다 
			{
				m_nCurrentIdx = nHeadIdx;
			}
		
		}
		else //선정 정보를 기준으로 움직인다
		{
			m_nCurrentIdx = nIdx;
		}

	}
	
	CString strREC_KEY;
	strREC_KEY = m_pParentDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);
	RefreshAll(strREC_KEY);
	m_pParentGrid->SetReverse(m_nCurrentIdx);
	
}

VOID CSePurchase::OnbtnNEXT() 
{
	// TODO: Add your control notification handler code here
	
	if(1 == m_nMoveOption)
	{
		m_nCurrentIdx++;
		INT nMaxIdx = m_pParentDM->GetRowCount()-1;
		if(m_nCurrentIdx > nMaxIdx)
		{
			AfxMessageBox(_T("마지막 항목입니다!"));
			m_nCurrentIdx--;
			return;
		}
	}
	else if(2 == m_nMoveOption)
	{
		INT nIdx = m_pParentGrid->SelectGetNext();

		if(nIdx<0) 
		{
			INT nTailIdx = m_pParentGrid->SelectGetTailPosition();
			
			if(m_nCurrentIdx == nTailIdx)
			{
				AfxMessageBox(_T("마지막 항목입니다!"));
				m_pParentGrid->SelectGetTailPosition();
				return;
			}
			else //이전 선정 정보가 없을 경우 맨뒤의 선정정보를 택한다 
			{
				m_nCurrentIdx = nTailIdx;
			}
		}
		else //선정 정보를 기준으로 움직인다
		{
			m_nCurrentIdx = nIdx;
		}
	}

	CString strREC_KEY;
	strREC_KEY = m_pParentDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);
	RefreshAll(strREC_KEY);
	m_pParentGrid->SetReverse(m_nCurrentIdx);

}

VOID CSePurchase::OnbtnSPECREATE() 
{
	// TODO: Add your control notification handler code here
	// 차수 입력
	CSePurchaseNoDlg dlg(this);
	if(dlg.DoModal() != IDOK) return;
	
	
	//수정 모드에서는 현재 차수가 주문처리중인가를 파악해 주문처리중 차수는 수정 불가 
	if(m_nOpenMode<0)
	{
		if(m_pDM_SEQ_NO->GetRowCount()>0) 
		{
			CString strWORKING_STATUS = _T("");
			strWORKING_STATUS = m_pDM_SEQ_NO->GetCellData(_T("작업상태"),0);
			if(_T("1") == strWORKING_STATUS)
			{
				AfxMessageBox(_T("현재 자료는 주문 중이므로 차수를 변경할 수 없습니다!"));
				return;
			}
		}
	}

	

	m_pCM->SetControlMgrData(_T("차수년도"),dlg.m_strYear);
	m_pCM->SetControlMgrData(_T("차수번호"),dlg.m_strPurchaseNo);


	SetPurchaseNo();
}

VOID CSePurchase::OnKillfocusedtSPEORDERNO() 
{
	// TODO: Add your control notification handler code here
	// 입력시 차수번호 kill focus: 해당 수입년도의 해당 차수를 가져와 해당 작업번호를 뿌려준다 
	if(m_nOpenMode<0) return;
	
	SetPurchaseNo();
}

VOID CSePurchase::SetPurchaseNo()
{

	CString strSEQ_NO = _T("");
	m_pCM->GetControlMgrData(_T("차수번호"),strSEQ_NO);
	if(strSEQ_NO.IsEmpty()) return;

	CString strYear;
	m_pCM->GetControlMgrData(_T("차수년도"),strYear);
	if(strYear.IsEmpty()) return;

	CString strQuery;
	strQuery.Format(_T("ACQ_YEAR = '%s' AND SEQ_NO = %s"),strYear,strSEQ_NO);
	m_pDM_SEQ_NO->RefreshDataManager(strQuery);
	
	INT nRowCnt = m_pDM_SEQ_NO->GetRowCount();
	if(0 == nRowCnt) {
		AfxMessageBox(_T("해당 차수년도에 존재하지 않는 차수번호입니다!"));
		//==========================================
		//20080606 UPDATE BY PJW : 차수가올바르지 않을경우 NULL로 셋팅한다.
		//GetDlgItem(IDC_edtSPE_ORDER_NO)->SetFocus();
		CEdit * pOrderNo = (CEdit *)GetDlgItem(IDC_edtSPE_ORDER_NO); 
		pOrderNo->SetWindowText(_T(""));
		pOrderNo->SetFocus();
		//=========================================
		return;
	}


	if(m_nOpenMode) //입력시에는 구입대상에 해당하는 차수만 입력 할 수 있게 한다
	{
		CString strWorking_status;
		strWorking_status = m_pDM_SEQ_NO->GetCellData(_T("작업상태"),0);
		if(_T("0") != strWorking_status) //구입대상이 아니면 
		{
			AfxMessageBox(_T("해당 차수는 주문중입니다. 구입대상 차수를 선택해 주세요!"));
			GetDlgItem(IDC_edtSPE_ORDER_NO)->SetFocus();
			return;
		}
	}
	
	
	CString strWorkNo = _T("");
	strWorkNo = m_pDM_SEQ_NO->GetCellData(_T("마지막작업번호"),0);
	
	CString strREC_KEY = _T("");
	strREC_KEY = m_pDM_SEQ_NO->GetCellData(_T("REC_KEY"),0);


	if(_T("0") == strWorkNo) //작업번호를 사용하지 않는 경우 
	{
		SetWorkNo(_T(""),_T(""),FALSE);
	}
	else //사용하는 경우
	{
		SetWorkNo(strREC_KEY,strWorkNo); //마지막 번호로 setting 
	}

}

VOID CSePurchase::SetWorkNo(CString strSEQ_NO_KEY, CString strCurrentWorkNo , BOOL bUseWorkNo)
{
	//작업번호 설정 함수 
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSPE_WORK_NO);
	if(!pCombo) return;
	
	if(!bUseWorkNo) //사용안함
	{
		pCombo->ResetContent();
		pCombo->InsertString( 0, _T("사용안함") );
		pCombo->SetCurSel( 0 );
		return;
	}

	if(strSEQ_NO_KEY.IsEmpty()) return;
	
	CString strQuery;
	strQuery.Format(_T("PURCHASE_SEQ_NO_KEY = %s"),strSEQ_NO_KEY);
	m_pDM_WORK_NO->RefreshDataManager(strQuery);
	
	pCombo->ResetContent();
	INT nRowCnt = m_pDM_WORK_NO->GetRowCount();
	CString strWorkNo;

	for(INT i=0;i<nRowCnt;i++)
	{
		strWorkNo = _T("");
		strWorkNo = m_pDM_WORK_NO->GetCellData(_T("작업번호"),i);
		pCombo->InsertString( i, strWorkNo);
	}
	
	pCombo->InsertString( nRowCnt , _T("새번호") );
		
	
	if(strCurrentWorkNo.IsEmpty())
	{
		pCombo->SetCurSel( 0 );
	}
	else
	{
		INT nIndex = pCombo->FindStringExact(0,strCurrentWorkNo);
	
		if(nIndex>=0) pCombo->SetCurSel( nIndex );
		else pCombo->SetCurSel( 0 );

	}

}
HBRUSH CSePurchase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	INT nID = pWnd->GetDlgCtrlID();
	if (IsColorControl(nID)) 
	{
		return SetColor(pDC);
	}

	return hbr;
}

VOID CSePurchase::OnbtnDEPARTMNG() 
{
	// TODO: Add your control notification handler code here
	if(!m_pDepartmentOrder) return;
	

	//부서별 자료지정 
	CString strPURCHASE_SPECIES_KEY;
	CString stPURCHASE_MANAGE_KEY;
	strPURCHASE_SPECIES_KEY = m_pDM->GetCellData(_T("REC_KEY"),0);

	
	if(m_nOpenMode<0)
	{
		CString strSPECIES_KEY;	
		CString strQuery;
		strSPECIES_KEY = m_pDM->GetCellData(_T("SPECIES_KEY"),0);


		if(!strSPECIES_KEY.IsEmpty())
		{
			strQuery.Format(_T("SELECT REC_KEY FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);

			m_pDM->GetOneValueQuery(strQuery,stPURCHASE_MANAGE_KEY);
		}
	}

	m_pDepartmentOrder->m_strPURCHASE_SPECIES_KEY = strPURCHASE_SPECIES_KEY;
	m_pDepartmentOrder->m_strPURCHASE_MANAGE_KEY = stPURCHASE_MANAGE_KEY;
	
	//test
	//m_pDepartmentOrder->RefreshAll();
	INT nAcqExpBookCnt = m_pDepartmentOrder->GetTotalAcqExpBookCnt(FALSE);
	m_pDepartmentOrder->ShowWindow(TRUE);

}

VOID CSePurchase::OnDepartmentBookCntChange(WPARAM w,LPARAM l)
{
	if(!m_pDepartmentOrder) return;

	INT nAcqExpBookCnt = m_pDepartmentOrder->GetTotalAcqExpBookCnt(FALSE);
	CString strCnt;
	CEdit* pEdit = (CEdit*)m_pCM->FindControl(_T("입수책수"));
	

	if(0 == nAcqExpBookCnt)
	{
		pEdit->EnableWindow(TRUE);
	}
	else
	{
		strCnt.Format(_T("%d"),nAcqExpBookCnt);
		m_pCM->SetControlMgrData(_T("입수책수"),strCnt);
		pEdit->EnableWindow(FALSE);
	}

}


VOID CSePurchase::SaveDepartmentOrderInfo()
{
	if(!m_pDepartmentOrder) return;
	

	//부서별 자료지정 
	CString strPURCHASE_SPECIES_KEY;
	CString stPURCHASE_MANAGE_KEY;
	strPURCHASE_SPECIES_KEY = m_pDM->GetCellData(_T("REC_KEY"),0);

	
	if(m_nOpenMode<0)
	{
		CString strSPECIES_KEY;	
		CString strQuery;
		strSPECIES_KEY = m_pDM->GetCellData(_T("SPECIES_KEY"),0);


		if(!strSPECIES_KEY.IsEmpty())
		{
			strQuery.Format(_T("SELECT REC_KEY FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);

			m_pDM->GetOneValueQuery(strQuery,stPURCHASE_MANAGE_KEY);
		}
	}

	m_pDepartmentOrder->m_strPURCHASE_SPECIES_KEY = strPURCHASE_SPECIES_KEY;
	m_pDepartmentOrder->m_strPURCHASE_MANAGE_KEY = stPURCHASE_MANAGE_KEY;

	//test
	if(m_nOpenMode>0)
	{
		INT nAcqExpBookCnt = m_pDepartmentOrder->GetTotalAcqExpBookCnt(FALSE);
		if(0 == nAcqExpBookCnt)
		{
			INT nACQ_BOOK_CNT = 1;	
			CString strACQ_BOOK_CNT;
			m_pCM->GetControlMgrData(_T("입수책수"),strACQ_BOOK_CNT);
			nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));

			m_pDepartmentOrder->InsertDefaultAcqBookCnt(nACQ_BOOK_CNT);
		}
	}
	
	INT ids = m_pDepartmentOrder->SaveAllData();

	if(ids<0) AfxMessageBox(_T("부서지정주문정보 저장에 실패 했습니다!"));


}
