// SeSpeciesForm.cpp : implementation file
//

#include "stdafx.h"
#include "SeSpeciesForm.h"
#include "SeMarcEdit.h"
#include "SeMemoEdit.h"
#include "SeTitleChangeDlg.h"
#include "SePublisherChangeDlg.h"
#include "SeMediaChangeDlg.h"
#include "SePubfreqChangeDlg.h"

#include "..\SE_HISTORY\SE_HISTORY_MAIN.h"
#include "..\SE_API\SeApi.h"
#include "..\SE_API\SeMarcImport.h"
#include "..\SE_API\SeDuplicatedTitleDlg.h"
#include "..\..\..\공동목록\Light_GatherDataSearch\UC_MARC_IMPORT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeSpeciesForm dialog


CSeSpeciesForm::CSeSpeciesForm(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeSpeciesForm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeSpeciesForm)
	m_bBINDING_YN = FALSE;
	//}}AFX_DATA_INIT
	m_nACQ_TYPE = 1; //default
	m_strSpeciesKey = _T("");
	m_pPurchaseDlg = NULL;
	m_pDonateDlg = NULL;
	m_pExpectDlg = NULL;
	m_nOpenMode = 1; //default 입력
	m_pCM = NULL;
	m_pDM = NULL;
	m_pDM_IDX = NULL;
	m_pDM_SUB_HISTORY = NULL;
	m_pDM_SPE_HISTORY = NULL;
	m_bIsExpCreated = FALSE;
	m_bIsExpVerified = FALSE;

	m_nPubExpType = -1;  
	m_nAcqExpType = -1;  

	m_pParentGrid = NULL;
	m_pParentDM = NULL;
	
	m_Marc.Init();
	m_strMEMO = _T("");
	m_nCurrentIdx = -1;


	m_strPURCHASE_YN = _T("");
	m_strDONATE_YN = _T("");
	
	m_pDiscontinueDlg = NULL;
	m_bIsDiscontinued = FALSE;
	m_bIsTitleChanged = FALSE;
	m_bIsPublisherChanged = FALSE;

	m_nMoveOption = 1; //default
	m_bIsLastCtrlFocused = FALSE;

	m_bIsLightVersion = FALSE;
	m_bIsDepartMngUse = FALSE;
	AfxInitRichEdit();

}

CSeSpeciesForm::~CSeSpeciesForm()
{
	FreeChildDialog();
}

VOID CSeSpeciesForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeSpeciesForm)
	DDX_Check(pDX, IDC_chkSPE_BINDING_YN, m_bBINDING_YN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeSpeciesForm, CDialog)
	//{{AFX_MSG_MAP(CSeSpeciesForm)
	ON_BN_CLICKED(IDC_btnMARC_EDIT, OnbtnMARCEDIT)
	ON_BN_CLICKED(IDC_btnMEMO, OnbtnMEMO)
	ON_BN_CLICKED(IDC_btnEXPECT, OnbtnEXPECT)
	ON_BN_CLICKED(IDC_btnHISTORY, OnbtnHISTORY)
	ON_BN_CLICKED(IDC_btnMARC_IMPORT, OnbtnMARCIMPORT)
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnALL_CLEAR, OnbtnALLCLEAR)
	ON_BN_CLICKED(IDC_btnSPE_TITLE_CHANGE, OnbtnSPETITLECHANGE)
	ON_BN_CLICKED(IDC_btnSPE_MEDIA_CODE_CHANGE, OnbtnSPEMEDIACODECHANGE)
	ON_BN_CLICKED(IDC_btnSPE_PUBLISHER_CHANGE, OnbtnSPEPUBLISHERCHANGE)
	ON_BN_CLICKED(IDC_btnSPE_PUB_FREQ_CHANGE, OnbtnSPEPUBFREQCHANGE)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnCOMMON_LIST, OnbtnCOMMONLIST)
	ON_CBN_KILLFOCUS(IDC_cmbSPE_SHELP_LOC_CODE, OnKillfocuscmbSPESHELPLOCCODE)
	ON_BN_CLICKED(IDC_btnSPE_TITLE_DUP_INSPECT, OnbtnSPETITLEDUPINSPECT)
	ON_MESSAGE(KILLFOCUS_LAST_CTRL, OnChildLastKillFocus)
	ON_CBN_SETFOCUS(IDC_cmbSPE_SHELP_LOC_CODE, OnSetfocuscmbSPESHELPLOCCODE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeSpeciesForm message handlers

BOOL CSeSpeciesForm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if( InitESL_Mgr( _T("연속_수서_종") ) < 0 ) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_종") );
		return FALSE;
	}


	m_pCM = FindCM(_T("CM_연속_종"));
	m_pDM = FindDM(_T("DM_연속_종"));
	m_pDM_IDX =  FindDM(_T("DM_연속_색인"));
	m_pDM_SUB_HISTORY =  FindDM(_T("DM_연속_구독이력정보"));
	m_pDM_SPE_HISTORY =  FindDM(_T("DM_연속_종_이력정보"));


	if(!m_pCM || !m_pDM || !m_pDM_IDX || !m_pDM_SUB_HISTORY || !m_pDM_SPE_HISTORY) {
		AfxMessageBox( _T("CM,DM Error : 연속_종,연속_색인") );
		return FALSE;
	}
	

	//라이트 버젼 setting 
	if(m_bIsLightVersion)
	{	
		SetLightVersion();
	}


	//구입 또는 기증 다이얼로그 생성
	CreateChildDialog();

	//콘트롤 설정
	InitControls();

	//입력 또는 수정 모드 설정 
	if(InitOpenMode()<0) {
		//수정
		EndDialog(0);
		return FALSE;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CSeSpeciesForm::SetLightVersion()
{
	//라이트 버젼의 경우 제본대상을 콘트롤없애고 가격 관련 콘트롤의 위치를 재배치 한다 	
	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_chkSPE_BINDING_YN);
	if(pWnd) pWnd->ShowWindow(FALSE);
		
	CRect rect;
	
	pWnd = GetDlgItem(IDC_STATIC_SPE_PRICE);
	if(pWnd)
	{
		pWnd->GetWindowRect(rect);
		ScreenToClient(&rect);
		rect.left = rect.left-43;
		rect.right = rect.right-43;
		
		/*
		rect.left = 12;
		rect.right = 45;
		rect.top = 265;
		rect.bottom = 280;
		*/

		pWnd->MoveWindow(rect);
	}
		
	pWnd = GetDlgItem(IDC_edtSPE_PRICE);
	if(pWnd)
	{
		pWnd->GetWindowRect(rect);
		ScreenToClient(&rect);

		rect.left = rect.left-43;
		rect.right = rect.right-33;
		
		/*
		rect.left = 63;
		rect.right = 144;
		rect.top = 263;
		rect.bottom = 283;		
		*/
			
		pWnd->MoveWindow(rect);
	}

}


VOID CSeSpeciesForm::InitControls()
{

	ShowUserDefineCode();

	//가격성격 설정 
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbSPE_PRICE_PERSONALITY );
	pCombo->InsertString( 0, _T("정가") );
	pCombo->InsertString( 1, _T("비매품") );
	pCombo->InsertString( 2, _T("가격불명") );
	pCombo->SetCurSel( 0 );
	

	//color
	AddColorControl(IDC_edtSPE_PUBLISHER, _T(""), _T(""), 'E');
	AddColorControl(IDC_cmbSPE_CONTROL_TYPE, _T("CM_연속_종"), _T("SS_제어자료구분_코드"), 'M');
	AddColorControl(IDC_cmbSPE_PUB_FREQ, _T("CM_연속_종"), _T("간행빈도"), 'M');
	AddColorControl(IDC_cmbSPE_PUB_STATUS, _T("CM_연속_종"), _T("간행상태"), 'M');
	AddColorControl(IDC_cmbSPE_SHELP_LOC_CODE, _T("CM_연속_종"), _T("SS_배가위치_코드"), 'M');

	
	//입력 모드 
	if(m_nOpenMode>0) 
	{
		
		//서명 color
		AddColorControl(IDC_edtSPE_TITLE1, _T(""), _T(""), 'E');
		
		

		//제어번호 
		m_pCM->SetControlMgrData(_T("SS_제어번호"),_T("<<자동 부여>>")); 
		
		//변경 버튼을 감춘다.
		GetDlgItem(IDC_btnSPE_TITLE_CHANGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnSPE_PUBLISHER_CHANGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnSPE_PUB_FREQ_CHANGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnSPE_MEDIA_CODE_CHANGE)->ShowWindow(SW_HIDE);
		

		//이전 다음을 감춘다
		//GetDlgItem(IDC_btnPREV)->ShowWindow(SW_HIDE); 
		
		//이전 버튼은 입력시 이전내용 불러오기 기능으로
		GetDlgItem(IDC_btnPREV)->SetWindowText(_T("이전내용\n불러오기"));
		GetDlgItem(IDC_btnNEXT)->ShowWindow(SW_HIDE);
		
		//종 이력 버튼을 감춘다
		GetDlgItem(IDC_btnHISTORY)->ShowWindow(SW_HIDE);

	}
	else
	{
		
		CEdit* pEdit;
		pEdit = (CEdit*)GetDlgItem(IDC_edtSPE_TITLE1);
		if(pEdit) pEdit->SetReadOnly();
		
		GetDlgItem(IDC_btnSPE_TITLE_DUP_INSPECT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnMARC_IMPORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnCOMMON_LIST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnALL_CLEAR)->ShowWindow(SW_HIDE);

	}

	ShowUserDefineCode();
}

INT CSeSpeciesForm::InitOpenMode()
{
	
	if(m_nOpenMode>0) //입력 
	{
	

	
	}
	else //수정
	{
		//부모 그리드에서 선정정보를 얻어온다
		if(!m_pParentGrid) {
			AfxMessageBox(_T("부모 그리드를 찾을 수 없습니다!"));
			return -1;
		}

		if(!m_pParentDM) {
			AfxMessageBox(_T("부모 DM을 찾을 수 없습니다!"));
			return -2;
		}
		
	
		if(m_nCurrentIdx<0) {
			AfxMessageBox(_T("선택된 항목이 없습니다!"));
			return -3;
		}
	
		m_strSpeciesKey = m_pParentDM->GetCellData(_T("ISS_종KEY"),m_nCurrentIdx);
		
		if(!m_strSpeciesKey.IsEmpty())
			RefreshAll(m_strSpeciesKey);
		
	}

	return 0;

}

INT CSeSpeciesForm::RefreshAll(CString strSpeciesKey)
{
	//종키를 가지고 종,색인 DM을 refresh 한다
	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"),strSpeciesKey);
	m_pDM->RefreshDataManager(strWhere);
	m_pDM_IDX->RefreshDataManager(strWhere);
	

	//마크구조체를 decoding 한다.
	m_Marc.Init();
	CString strStreamMarc;
	strStreamMarc = m_pDM->GetCellData(_T("MARC"),0);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_Marc);	


	//기본 control정보를 보여준다
	m_pCM->AllControlDisplay(0);
	
	
	//창간년, 폐간년 -- 빈칸의 경우는 trim해서 콘트롤에 저장
	CString strFoundationYear,strDiscontinueYear;
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@7-10"),strFoundationYear);
	strFoundationYear.TrimRight();
	strFoundationYear.TrimLeft();
	m_pCM->SetControlMgrData(_T("창간년"),strFoundationYear);
	
	
	//폐간년 
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@11-14"),strDiscontinueYear);
	strDiscontinueYear.TrimRight();
	strDiscontinueYear.TrimLeft();
	m_pCM->SetControlMgrData(_T("폐간년"),strDiscontinueYear);		



	//발행자2(550 tag 의 맨마지막 데이터)
	CArray<CString,CString&> arrayPublisher;
	CString strPublisher550 = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("550$a"),strPublisher550,&arrayPublisher);
	
	INT nSize = arrayPublisher.GetSize();
	if(nSize>0)
	{
		strPublisher550 = arrayPublisher.GetAt(nSize-1);
	}

	if(!strPublisher550.IsEmpty())
	{
		m_pCM->SetControlMgrData(_T("발행자2"),strPublisher550);
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
	

	//가격성격
	CString tmp;
	tmp = m_pDM->GetCellData(_T("SS_가격성격"),0);
	
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbSPE_PRICE_PERSONALITY );
	CString strCombo;

	if ( tmp == _T("") ) tmp = _T("정가");

	for(INT i=0;i<pCombo->GetCount();i++)
	{
		strCombo = _T("");
		pCombo->SetCurSel(i);
		pCombo->GetWindowText(strCombo);
		if(tmp == strCombo) break;		
	}


	//제본여부
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SS_제본여부"),0);
	if(_T("Y") == tmp)
		m_bBINDING_YN = TRUE;
	else 
		m_bBINDING_YN = FALSE;

	tmp = _T("");
	

	//ISSN
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
	// 2008.05.15 ADD BY PDJ
	// 없을경우 처리
	else
	{
		m_pCM->SetControlMgrData(_T("ISSN1"),_T(""));
		m_pCM->SetControlMgrData(_T("ISSN2"),_T(""));
	}

	//메모
	m_strMEMO = m_pDM->GetCellData(_T("SS_메모"),0);


	//현재 종의 상태
	m_strPURCHASE_YN = m_pDM->GetCellData(_T("SS_구입여부"),0);
	m_strDONATE_YN = m_pDM->GetCellData(_T("SS_기증여부"),0);
	
	//신청학과
	//m_pCM->SetControlMgrData(_T(""),strISSN1);

	//신청학부
	//m_pCM->SetControlMgrData(_T("ISSN1"),strISSN1);



	if(1 == m_nACQ_TYPE)  //구입 
	{

		if(_T("Y") == m_strPURCHASE_YN)
		{

			//기존 예측정보를 지운다 
			if(m_pExpectDlg) delete m_pExpectDlg;
			m_pExpectDlg = NULL;
			m_bIsExpCreated = FALSE;
			m_bIsExpVerified = FALSE;
			m_nPubExpType = -1;  
			m_nAcqExpType = -1;  

			
		}
		else if(_T("N") == m_strPURCHASE_YN)
		{
			//기증정보를 확인해보아서 예측정보가 있으면 가지고 온다
			BringExpData(strSpeciesKey,2);

		}
		
	}
	else if(2 == m_nACQ_TYPE) //기증 
	{

		if(_T("Y") == m_strDONATE_YN)
		{
			
			//기존 예측정보를 지운다 
			if(m_pExpectDlg) delete m_pExpectDlg;
			m_pExpectDlg = NULL;
			m_bIsExpCreated = FALSE;
			m_bIsExpVerified = FALSE;
			m_nPubExpType = -1;  
			m_nAcqExpType = -1;  

		}
		else if(_T("N") == m_strDONATE_YN)
		{
			//구입정보를 확인해보아서 예측정보가 있으면 가지고 온다
			BringExpData(strSpeciesKey,1);

		}
	}

	
	//종이력DM free 
	m_pDM_SPE_HISTORY->FreeData();


	//폐간여부
	if(m_pDiscontinueDlg)
	{
		delete m_pDiscontinueDlg;
		m_pDiscontinueDlg = NULL;
	}
	m_bIsDiscontinued = FALSE;

	
	//종변경 여부 
	m_bIsTitleChanged = FALSE;

	
	//발행자 변경 여부
	m_bIsPublisherChanged = FALSE;


	//child 
	if(1 == m_nACQ_TYPE)
	{
		m_pPurchaseDlg->RefreshAll(strSpeciesKey);
	}
	else if(2 == m_nACQ_TYPE)
	{
		m_pDonateDlg->RefreshAll(strSpeciesKey);
	}

	UpdateData(FALSE);
	

	return 0;
}


VOID CSeSpeciesForm::BringPrevData()
{
	//입력모드에서의 이전내용 불러오기
	INT nParentRowCnt = m_pParentDM->GetRowCount();
	if(0 == nParentRowCnt) return;
	
	//마지막 행 정보를 불러온다
	INT nIdx = nParentRowCnt -1;
	CString strSpeciesKey = m_pParentDM->GetCellData(_T("ISS_종KEY"),nIdx);

	//종키를 가지고 종,색인 DM을 refresh 한다
	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"),strSpeciesKey);
	m_pDM->RefreshDataManager(strWhere);
	m_pDM_IDX->RefreshDataManager(strWhere);
	
	
	//기본 control정보를 보여준다
	m_pCM->AllControlDisplay(0);
	

	//제어번호는 비운다
	m_pCM->SetControlMgrData(_T("SS_제어번호"),_T(""));

	//2005-10-04 : 마크정보를 얻어온다
	//마크구조체를 decoding 한다.
	CMarc tempMarc;
	tempMarc.Init();
	CString strStreamMarc;
	strStreamMarc = m_pDM->GetCellData(_T("MARC"),0);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &tempMarc);
	
	
	//간행빈도가 빈칸일 경우는 부정기간(_)로 표시한다.
	CString strPUB_FREQ = _T("");
	

	//간행빈도(부정기간처리)
	m_pInfo->pMarcMgr->GetItem(&tempMarc,_T("008@18"),strPUB_FREQ);
	
	strPUB_FREQ.TrimLeft();
	strPUB_FREQ.TrimRight();

	if(strPUB_FREQ.IsEmpty())
	{
		m_pCM->SetControlMgrData(_T("간행빈도"),_T("_"));
	}
	

	//가격성격
	CString tmp;
	tmp = m_pDM->GetCellData(_T("SS_가격성격"),0);
	
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbSPE_PRICE_PERSONALITY );
	CString strCombo;

	for(INT i=0;i<pCombo->GetCount();i++)
	{
		strCombo = _T("");
		pCombo->SetCurSel(i);
		pCombo->GetWindowText(strCombo);
		if(tmp == strCombo) break;
	}


	//제본여부
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SS_제본여부"),0);
	if(_T("Y") == tmp)
		m_bBINDING_YN = TRUE;
	else 
		m_bBINDING_YN = FALSE;

	tmp = _T("");
	
	//ISSN
	m_pInfo->pMarcMgr->GetItem(&tempMarc,_T("022$a"),tmp);
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
	
		
	/*
	//현재 종의 상태
	m_strPURCHASE_YN = m_pDM->GetCellData(_T("SS_구입여부"),0);
	m_strDONATE_YN = m_pDM->GetCellData(_T("SS_기증여부"),0);
	
	
	if(1 == m_nACQ_TYPE)  //구입 
	{

		if(_T("Y") == m_strPURCHASE_YN)
		{

			//기존 예측정보를 지운다 
			if(m_pExpectDlg) delete m_pExpectDlg;
			m_pExpectDlg = NULL;
			m_bIsExpCreated = FALSE;
			m_bIsExpVerified = FALSE;
			m_nPubExpType = -1;  
			m_nAcqExpType = -1;  

			
		}
		else if(_T("N") == m_strPURCHASE_YN)
		{
			//기증정보를 확인해보아서 예측정보가 있으면 가지고 온다
			BringExpData(strSpeciesKey,2);

		}
		
	}
	else if(2 == m_nACQ_TYPE) //기증 
	{

		if(_T("Y") == m_strDONATE_YN)
		{
			
			//기존 예측정보를 지운다 
			if(m_pExpectDlg) delete m_pExpectDlg;
			m_pExpectDlg = NULL;
			m_bIsExpCreated = FALSE;
			m_bIsExpVerified = FALSE;
			m_nPubExpType = -1;  
			m_nAcqExpType = -1;  

		}
		else if(_T("N") == m_strDONATE_YN)
		{
			//구입정보를 확인해보아서 예측정보가 있으면 가지고 온다
			BringExpData(strSpeciesKey,1);

		}
	}
	*/
	

	//데이터를 지운다
	m_pDM->FreeData();
	m_pDM_IDX->FreeData();

	//child 에 콘트롤에 데이터를 뿌리고, 데이터를 지운다
	if(1 == m_nACQ_TYPE)
	{
		m_pPurchaseDlg->RefreshAll(strSpeciesKey);
		m_pPurchaseDlg->m_pDM->FreeData();
		m_pPurchaseDlg->m_pDM_SUB_HISTORY->FreeData();
	}
	else if(2 == m_nACQ_TYPE)
	{
		m_pDonateDlg->RefreshAll(strSpeciesKey);
		m_pDonateDlg->m_pDM->FreeData();
	}

	UpdateData(FALSE);
	
	
	return;
}


VOID CSeSpeciesForm::BringExpData(CString strSPECIES_KEY,INT nACQ_TYPE)
{
	//수입 구분을 확인해서 해당 수입구분의 예측 데이터를 가지고 온다.
	CESL_DataMgr* pDM_MONTH = FindDM(_T("DM_종_월간예측"));
	CESL_DataMgr* pDM_WEEK = FindDM(_T("DM_종_주간예측"));
	CESL_DataMgr* pDM_DAY = FindDM(_T("DM_종_일간예측"));
	CESL_DataMgr* pDM_ETC = FindDM(_T("DM_종_기타예측"));
	CESL_DataMgr* pDM_VOL = FindDM(_T("DM_연속_종_예측_권호"));
	CESL_DataMgr* pDM_OTHER_VOL = FindDM(_T("DM_연속_종_예측_부차적권호"));
	CESL_DataMgr* pDM_APPENDIX = FindDM(_T("DM_연속_종_예측_부록"));


	if(!pDM_MONTH || !pDM_WEEK || !pDM_DAY || !pDM_ETC || !pDM_VOL || !pDM_OTHER_VOL || !pDM_APPENDIX)
	{
		AfxMessageBox(_T("예측정보를 가져올 DM을 찾지 못했습니다!")); 	
		return;
	}

	pDM_MONTH->FreeData();
	pDM_WEEK->FreeData();
	pDM_DAY->FreeData();	
	pDM_ETC->FreeData();	
	pDM_VOL->FreeData();
	pDM_OTHER_VOL->FreeData();	
	pDM_APPENDIX->FreeData();		

	
	CString strQuery;
	const INT cnt = 5;
	CString strResult[cnt];
	
	if(2 == nACQ_TYPE)
	{
		strQuery.Format(_T("SELECT REC_KEY,ACQ_EXP_KEY,PUB_EXP_KEY,ACQ_EXP_TYPE,PUB_EXP_TYPE FROM SE_DONATE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	}
	else if(1 == nACQ_TYPE)
	{
		strQuery.Format(_T("SELECT REC_KEY,ACQ_EXP_KEY,PUB_EXP_KEY,ACQ_EXP_TYPE,PUB_EXP_TYPE FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	}

	m_pDM->GetValuesQuery(strQuery,strResult,5);

	if(strResult[0].IsEmpty()) return;
	
	CString strACQ_KEY;
	CString strACQ_EXP_KEY;
	CString strPUB_EXP_KEY;
	CString strACQ_EXP_TYPE;
	CString strPUB_EXP_TYPE;
	
	strACQ_KEY = strResult[0];
	strACQ_EXP_KEY = strResult[1];
	strPUB_EXP_KEY =  strResult[2];
	strACQ_EXP_TYPE =  strResult[3];
	strPUB_EXP_TYPE =  strResult[4];


	if(strACQ_EXP_TYPE.IsEmpty() || strPUB_EXP_TYPE.IsEmpty()) {

		//기존 예측정보를 지운다 
		if(m_pExpectDlg) delete m_pExpectDlg;
		m_pExpectDlg = NULL;
		m_bIsExpCreated = FALSE;
		m_bIsExpVerified = FALSE;
		m_nPubExpType = -1;  
		m_nAcqExpType = -1;  
	
		return;
	}

	//권호 
	strQuery.Format(_T("ACQ_KEY = %s AND FORMAT_TYPE = '1'"),strACQ_KEY);
	pDM_VOL->RefreshDataManager(strQuery);


	//부차적권호 
	strQuery.Format(_T("ACQ_KEY = %s AND FORMAT_TYPE = '2'"),strACQ_KEY);
	pDM_OTHER_VOL->RefreshDataManager(strQuery);


	//부록
	strQuery.Format(_T("ACQ_KEY = %s"),strACQ_KEY);
	pDM_APPENDIX->RefreshDataManager(strQuery);

	
	if(strACQ_EXP_TYPE == strPUB_EXP_TYPE) //발행 예측과 입수 예측이 동일 할 경우
	{
		
		CESL_DataMgr* pDM_MONTH_SUB = FindDM(_T("DM_종_월간예측_보조"));
		CESL_DataMgr* pDM_WEEK_SUB = FindDM(_T("DM_종_주간예측_보조"));
		CESL_DataMgr* pDM_DAY_SUB = FindDM(_T("DM_종_일간예측_보조"));
		CESL_DataMgr* pDM_ETC_SUB = FindDM(_T("DM_종_기타예측_보조"));
		
		if(!pDM_MONTH_SUB || !pDM_WEEK_SUB || !pDM_DAY_SUB || !pDM_ETC_SUB)
		{
			AfxMessageBox(_T("예측정보를 가져올 보조 DM을 찾지 못했습니다!")); 	
			return;
		}

		strQuery.Format(_T("REC_KEY = %s"),strPUB_EXP_KEY); //발행일
	

		switch(strPUB_EXP_TYPE.GetAt(0))
		{
			case 'M':
				pDM_MONTH->RefreshDataManager(strQuery);
				break;
			case 'W':
				pDM_WEEK->RefreshDataManager(strQuery);
				break;
			case 'D':
				pDM_DAY->RefreshDataManager(strQuery);
				break;	
			case 'E':
				pDM_ETC->RefreshDataManager(strQuery);
				break;
			default:
				break;
		}

		strQuery.Format(_T("REC_KEY = %s"),strACQ_EXP_KEY); //입수일
		
		switch(strACQ_EXP_TYPE.GetAt(0))
		{
			case 'M':
				pDM_MONTH_SUB->RefreshDataManager(strQuery);
				break;
			case 'W':
				pDM_WEEK_SUB->RefreshDataManager(strQuery);
				break;
			case 'D':
				pDM_DAY_SUB->RefreshDataManager(strQuery);
				break;	
			case 'E':
				pDM_ETC_SUB->RefreshDataManager(strQuery);
				break;
			default:
				break;
			
		}

		//DATA copy
		switch(strPUB_EXP_TYPE.GetAt(0))
		{
			case 'M':
				pDM_MONTH->InsertRow(-1);
				CopyDMToDMByAlias(pDM_MONTH_SUB,0,pDM_MONTH,1);
				break;
			case 'W':
				pDM_WEEK->InsertRow(-1);
				CopyDMToDMByAlias(pDM_WEEK_SUB,0,pDM_WEEK,1);
				break;
			case 'D':
				pDM_DAY->InsertRow(-1);
				CopyDMToDMByAlias(pDM_DAY_SUB,0,pDM_DAY,1);
				break;	
			case 'E':
				pDM_ETC->InsertRow(-1);
				CopyDMToDMByAlias(pDM_ETC_SUB,0,pDM_ETC,1);
			default:
				break;

		}


	}
	else //발행 예측과 입수 예측이 다를 경우
	{

		strQuery.Format(_T("REC_KEY = %s"),strPUB_EXP_KEY); //발행일
		
		switch(strPUB_EXP_TYPE.GetAt(0))
		{
			case 'M':
				pDM_MONTH->RefreshDataManager(strQuery);
				break;
			case 'W':
				pDM_WEEK->RefreshDataManager(strQuery);
				break;
			case 'D':
				pDM_DAY->RefreshDataManager(strQuery);
				break;	
			case 'E':
				pDM_ETC->RefreshDataManager(strQuery);
				break;
			default:
				break;

		}

		strQuery.Format(_T("REC_KEY = %s"),strACQ_EXP_KEY); //입수일
		
		switch(strACQ_EXP_TYPE.GetAt(0))
		{
			case 'M':
				pDM_MONTH->RefreshDataManager(strQuery);
				break;
			case 'W':
				pDM_WEEK->RefreshDataManager(strQuery);
				break;
			case 'D':
				pDM_DAY->RefreshDataManager(strQuery);
				break;	
			case 'E':
				pDM_ETC->RefreshDataManager(strQuery);
				break;
			default:
				break;

		}
	}

	if(m_pExpectDlg) delete m_pExpectDlg;
	m_pExpectDlg = NULL;

	m_bIsExpVerified = TRUE;
	m_bIsExpCreated = FALSE;
	m_nPubExpType = GetIntExpType(strPUB_EXP_TYPE);  
	m_nAcqExpType = GetIntExpType(strACQ_EXP_TYPE);   


}

INT CSeSpeciesForm::DeleteExpData(CString strSPECIES_KEY,INT nACQ_TYPE)
{
	//수입 구분을 확인해서 해당 수입구분의 예측 데이터를 지운다.
	CString strQuery;
	const INT cnt = 5;
	CString strResult[cnt];
	
	if(2 == nACQ_TYPE)
	{
		strQuery.Format(_T("SELECT REC_KEY,ACQ_EXP_KEY,PUB_EXP_KEY,ACQ_EXP_TYPE,PUB_EXP_TYPE FROM SE_DONATE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	}
	else if(1 == nACQ_TYPE)
	{
		strQuery.Format(_T("SELECT REC_KEY,ACQ_EXP_KEY,PUB_EXP_KEY,ACQ_EXP_TYPE,PUB_EXP_TYPE FROM SE_PURCHASE_MANAGE_TBL WHERE SPECIES_KEY = %s"),strSPECIES_KEY);
	}

	m_pDM->GetValuesQuery(strQuery,strResult,5);

	if(strResult[0].IsEmpty()) return -1;
	
	CString strACQ_KEY;
	CString strACQ_EXP_KEY;
	CString strPUB_EXP_KEY;
	CString strACQ_EXP_TYPE;
	CString strPUB_EXP_TYPE;
	
	strACQ_KEY = strResult[0];
	strACQ_EXP_KEY = strResult[1];
	strPUB_EXP_KEY =  strResult[2];
	strACQ_EXP_TYPE =  strResult[3];
	strPUB_EXP_TYPE =  strResult[4];
	

	m_pDM->StartFrame();

	CString strTblName;
	
	//입수일 
	if(!strACQ_EXP_TYPE.IsEmpty() && !strACQ_EXP_KEY.IsEmpty())
	{
		
		strTblName = GetExpTable(strACQ_EXP_TYPE);
		strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s;"),strTblName,strACQ_EXP_KEY);
		m_pDM->InitDBFieldData();
		m_pDM->AddFrame(strQuery);
	
	}
	
	//발행일 
	if(!strPUB_EXP_TYPE.IsEmpty() && !strPUB_EXP_KEY.IsEmpty())
	{

		strTblName = GetExpTable(strPUB_EXP_TYPE);
		strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s;"),strTblName,strPUB_EXP_KEY);
		m_pDM->InitDBFieldData();
		m_pDM->AddFrame(strQuery);

	}
		
	//권호 , 부차적권호
	strQuery.Format(_T("DELETE FROM SE_VOL_EXP_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
	m_pDM->InitDBFieldData();
	m_pDM->AddFrame(strQuery);
	
	
	//부록
	strQuery.Format(_T("DELETE FROM SE_APPENDIX_EXP_TBL WHERE ACQ_KEY = %s;"),strACQ_KEY);
	m_pDM->InitDBFieldData();
	m_pDM->AddFrame(strQuery);
	
	
	//구입, 기증 정보 update
	/*
	if(2 == nACQ_TYPE)
	{
		strQuery.Format(_T("UPDATE SE_DONATE_MANAGE_TBL SET ACQ_EXP_KEY = NULL,PUB_EXP_KEY = NULL")
						_T(",ACQ_EXP_TYPE = NULL,PUB_EXP_TYPE = NULL WHERE REC_KEY = %s;"),strACQ_KEY);
	}
	else if(1 == nACQ_TYPE)
	{
		strQuery.Format(_T("UPDATE SE_PURCHASE_MANAGE_TBL SET ACQ_EXP_KEY = NULL,PUB_EXP_KEY = NULL")
						_T(",ACQ_EXP_TYPE = NULL,PUB_EXP_TYPE = NULL WHERE REC_KEY = %s;"),strACQ_KEY);
	}
	*/

	//수정 2004.2.20 -- 예측 타입만 지우도록...
	if(2 == nACQ_TYPE)
	{
		strQuery.Format(_T("UPDATE SE_DONATE_MANAGE_TBL SET ACQ_EXP_TYPE = NULL,PUB_EXP_TYPE = NULL")
						_T(" WHERE REC_KEY = %s;"),strACQ_KEY);
	}
	else if(1 == nACQ_TYPE)
	{
		strQuery.Format(_T("UPDATE SE_PURCHASE_MANAGE_TBL SET ACQ_EXP_TYPE = NULL,PUB_EXP_TYPE = NULL")
						_T(" WHERE REC_KEY = %s;"),strACQ_KEY);
	}
	
	m_pDM->InitDBFieldData();
	m_pDM->AddFrame(strQuery);


	INT ids = m_pDM->SendFrame();
	m_pDM->EndFrame();

	return ids;
}

CString CSeSpeciesForm::GetExpTable(CString strType)
{

	CString strTBL_NAME = _T("");

	switch(strType.GetAt(0))
	{
	case 'M':
		strTBL_NAME = _T("SE_MONTH_EXP_TBL");
		break;
	case 'W':
		strTBL_NAME = _T("SE_WEEK_EXP_TBL");
		break;
	case 'D':
		strTBL_NAME = _T("SE_DAY_EXP_TBL");
		break;
	case 'E':
		strTBL_NAME = _T("SE_ETC_EXP_TBL");
		break;
	default:
		break;
	}

	return strTBL_NAME;
}

VOID CSeSpeciesForm::RefreshParent()
{
	//부모 DM에 현재의 DM을 copy해주고 grid를 display한다
	INT idx;
	
	if(m_nOpenMode>0) //입력 
	{
		m_pParentDM->InsertRow(-1);
		idx = m_pParentDM->GetRowCount()-1;
	}	
	else
	{
		
		idx = m_nCurrentIdx;
		CopyDMToDMByAlias(m_pDM_IDX,0,m_pParentDM,idx);

	}
	

	CString strDesc;
	CopyDMToDMByAlias(m_pDM_IDX,0,m_pParentDM,idx);
	

	//간행빈도
	CString strPUB_FREQ;
	strPUB_FREQ = m_pDM_IDX->GetCellData(_T("ISS_간행빈도"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("간행빈도코드"),strPUB_FREQ,strDesc);
	m_pParentDM->SetCellData(_T("UDF_간행빈도"),strDesc,idx);


	//간행상태
	CString strPUB_STATUS;
	strPUB_STATUS = m_pDM_IDX->GetCellData(_T("ISS_간행상태"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("간행상태코드"),strPUB_STATUS,strDesc);
	m_pParentDM->SetCellData(_T("UDF_간행상태"),strDesc,idx);


	//제본여부
	CString strBINDING_YN = _T("");
	strBINDING_YN = m_pDM->GetCellData(_T("SS_제본여부"),0);
	m_pParentDM->SetCellData(_T("SS_제본여부"),strBINDING_YN,idx);

	
	//등록구분
	CString strREG_CODE = _T("");
	strREG_CODE = m_pDM->GetCellData(_T("SS_등록구분_코드"),0);
	m_pParentDM->SetCellData(_T("SS_등록구분_코드"),strREG_CODE,idx);


	//이용제한구분
	CString strUSE_LIMIT_CODE = _T("");
	strUSE_LIMIT_CODE = m_pDM->GetCellData(_T("SS_이용제한구분_코드"),0);
	m_pParentDM->SetCellData(_T("SS_이용제한구분_코드"),strUSE_LIMIT_CODE,idx);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("이용제한구분"),strUSE_LIMIT_CODE,strDesc);
	m_pParentDM->SetCellData(_T("UDF_이용제한구분"),strDesc,idx);
	

	
	//관리구분
	CString strMANAGE_CODE = _T("");
	strMANAGE_CODE = m_pDM->GetCellData(_T("SS_관리구분_코드"),0);
	m_pParentDM->SetCellData(_T("SS_관리구분_코드"),strMANAGE_CODE,idx);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("관리구분"),strMANAGE_CODE,strDesc);
	m_pParentDM->SetCellData(_T("UDF_관리구분"),strDesc,idx);
	
	

	//자료구분
	CString strMAT_CODE = _T("");
	strMAT_CODE = m_pDM->GetCellData(_T("SS_자료구분_코드"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("자료구분"),strMAT_CODE,strDesc);
	m_pParentDM->SetCellData(_T("UDF_자료구분"),strDesc,idx);



	//배가위치코드
	CString strSHELF_LOC_CODE = _T("");
	strSHELF_LOC_CODE = m_pDM->GetCellData(_T("SS_배가위치_코드"),0);
	m_pParentDM->SetCellData(_T("SS_배가위치_코드"),strSHELF_LOC_CODE,idx);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("자료실구분"),strSHELF_LOC_CODE,strDesc);
	m_pParentDM->SetCellData(_T("UDF_자료실"),strDesc,idx);



	//분류기호구분
	CString strCLASS_NO_TYPE = _T("");
	strCLASS_NO_TYPE = m_pDM->GetCellData(_T("SS_분류기호구분_코드"),0);
	m_pParentDM->SetCellData(_T("SS_분류기호구분_코드"),strCLASS_NO_TYPE,idx);
	

	//화폐구분
	CString strCURRENCY_CODE = _T("");
	strCURRENCY_CODE = m_pDM->GetCellData(_T("SS_화폐구분_코드"),0);
	m_pParentDM->SetCellData(_T("SS_화폐구분_코드"),strCURRENCY_CODE,idx);

	
	//가격
	CString strPRICE = _T("");
	strPRICE = m_pDM->GetCellData(_T("SS_가격"),0);
	m_pParentDM->SetCellData(_T("SS_가격"),strPRICE,idx);
	

	CString strPURCHASE_YN;
	CString strDONATE_YN;
	strPURCHASE_YN = m_pDM->GetCellData(_T("SS_구입여부"),0);
	strDONATE_YN = m_pDM->GetCellData(_T("SS_기증여부"),0);
		
	if(_T("Y") == strPURCHASE_YN &&  _T("Y") == strDONATE_YN)
	{
		m_pParentDM->SetCellData(_T("UDF_수입구분"),_T("구입/기증"),idx);
	}
	else if(_T("N") == strPURCHASE_YN &&  _T("Y") == strDONATE_YN)
	{
		m_pParentDM->SetCellData(_T("UDF_수입구분"),_T("기증"),idx);
	}
	else if(_T("Y") == strPURCHASE_YN &&  _T("N") == strDONATE_YN)
	{
		m_pParentDM->SetCellData(_T("UDF_수입구분"),_T("구입"),idx);
	}		


	// 학부, 학처명 추가
	// 2005.09.15 강봉수
	//
	CString strDept = _T("");
	CString strCource = _T("");
	if(m_nACQ_TYPE == 1)	
	{
		strDept = m_pPurchaseDlg->m_pDM->GetCellData(_T("SP_신청학부"),0);
		strCource = m_pPurchaseDlg->m_pDM->GetCellData(_T("SP_신청학과"),0);
	}
	else if (m_nACQ_TYPE == 2)
	{
		strDept = m_pDonateDlg->m_pDM->GetCellData(_T("SD_신청학부"),0);
		strCource = m_pDonateDlg->m_pDM->GetCellData(_T("SD_신청학과"),0);
	}

	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("학부"),strDept,strDesc);
	m_pParentDM->SetCellData(_T("UDF_학처명"),strDesc,idx);	
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("학과"),strCource,strDesc);
	m_pParentDM->SetCellData(_T("UDF_학과명"),strDesc,idx);
	
	
	m_pParentGrid->DisplayLine(idx);

}

INT CSeSpeciesForm::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
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

VOID CSeSpeciesForm::CreateChildDialog()
{
	CRect rect;
	this->GetClientRect(rect);
	
	rect.top = 320;
	rect.bottom = 465;

	if(1 == m_nACQ_TYPE)
	{
		m_pPurchaseDlg = new CSePurchaseForm(this);
		m_pPurchaseDlg->m_nOpenMode = m_nOpenMode;
		m_pPurchaseDlg->m_bIsLightVersion = m_bIsLightVersion;
		m_pPurchaseDlg->m_bIsDepartMngUse = m_bIsDepartMngUse;
		m_pPurchaseDlg->Create((CESL_Mgr*)this);
		m_pPurchaseDlg->ShowWindow(SW_SHOW);
		m_pPurchaseDlg->MoveWindow( rect );
	}
	else if(2 == m_nACQ_TYPE)
	{
		m_pDonateDlg = new CSeDonateForm(this);
		m_pDonateDlg->m_bIsLightVersion = m_bIsLightVersion;
		m_pDonateDlg->Create((CESL_Mgr*)this);
		m_pDonateDlg->ShowWindow(SW_SHOW);
		m_pDonateDlg->MoveWindow( rect );

	}

}

VOID CSeSpeciesForm::FreeChildDialog()
{
	if(1 == m_nACQ_TYPE)
	{
		if(m_pPurchaseDlg) delete m_pPurchaseDlg; 
		m_pPurchaseDlg = NULL;
	}
	else if(2 == m_nACQ_TYPE)
	{	
		if(m_pDonateDlg) delete m_pDonateDlg; 
		m_pDonateDlg = NULL;
	}

	if(m_pExpectDlg) delete m_pExpectDlg;
	m_pExpectDlg = NULL;

}	

VOID CSeSpeciesForm::SaveAllDataToDM()
{
	if(m_nOpenMode>0) //입력모드
	{
		//구입 혹은 기증 정보를 저장한다 		
		if(1 == m_nACQ_TYPE) {
		
			m_pPurchaseDlg->m_pDM->FreeData();
			m_pPurchaseDlg->m_pDM->InsertRow(-1);
			m_pPurchaseDlg->SaveAllDataToDM();
		
		} else {

			m_pDonateDlg->m_pDM->FreeData();
			m_pDonateDlg->m_pDM->InsertRow(-1);
			m_pDonateDlg->SaveAllDataToDM();
		}

		//마크구조체에 정보를 담아 온다
		SetMarcByControlData();


		//그외의 data를 종DM에 넣는다.
		m_pDM->FreeData();
		m_pDM->InsertRow(-1);
		SaveSpeciesDataToDM();

	}
	else   //수정모드
	{
		
		//수입구분이 변화(구입->기증, 기증->구입)되거나 기타의 구입구분일 경우 child는 INSERT이다 
		if(m_strPURCHASE_YN.IsEmpty() || m_strDONATE_YN.IsEmpty()) {
			AfxMessageBox(_T("종의 수입 구분이 정확하지 않습니다!"));
			return;
		}
		

		if(1 == m_nACQ_TYPE)  //구입 
		{

			if(_T("Y") == m_strPURCHASE_YN)
			{
				m_pPurchaseDlg->SaveAllDataToDM();
			
			}
			else if(_T("N") == m_strPURCHASE_YN)
			{
				m_pPurchaseDlg->m_pDM->FreeData();
				m_pPurchaseDlg->m_pDM->InsertRow(-1);
				m_pPurchaseDlg->SaveAllDataToDM();

			}
		
		}
		else if(2 == m_nACQ_TYPE) //기증 
		{

			if(_T("Y") == m_strDONATE_YN)
			{

				m_pDonateDlg->SaveAllDataToDM();

			}
			else if(_T("N") == m_strDONATE_YN)
			{
				m_pDonateDlg->m_pDM->FreeData();
				m_pDonateDlg->m_pDM->InsertRow(-1);
				m_pDonateDlg->SaveAllDataToDM();

			}
		}
		

		//마크구조체에 정보를 담아 온다
		SetMarcByControlData();

		//그외의 data를 종DM에 넣는다.
		SaveSpeciesDataToDM();
	}


}

INT CSeSpeciesForm::SetMarcByControlData()
{
	//입력과 수정모드에 대한 설정을 자세히 할것!
	//현재는 기종 데이터를 지운후 다시 입력한다
	
	//연속 마크 그룹 설정
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	CString strtmp;
	
	// 연속 마크 leader 설정
	CSeApi api(this);
	api.SetSerialMarcReader(&m_Marc);

	//기존데이터를 지우고 입력
	const INT cnt = 14;
	CString strtag[cnt] = {
		_T("245$x"),	//1
		_T("440$a"),	//2
		_T("245$d"),	//3
		_T("260$a"),	//4	
		_T("260$b"),	//5	
		_T("030$a"),	//6
		_T("300$c"),	//7	
		_T("300$a"),	//8	
		_T("022$a"),	//9	
		_T("008@6"),	//10
		_T("008@7-10"),	//11
		_T("008@11-14"),//12
		_T("008@18"),	//13
		_T("008@19")};	//14
	

	for(INT i=0;i<cnt;i++)
	{
		m_pInfo->pMarcMgr->DeleteItem(&m_Marc,strtag[i]);
	}
	

	if(m_nOpenMode>0 || m_bIsDiscontinued || m_bIsTitleChanged) //서명은 수정 모드에서는 특별 관리
	{
		
		m_pInfo->pMarcMgr->DeleteItem(&m_Marc,_T("245$a"));
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
	}
	
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
	

	//발행자가 변경 되었을 경우 저장시에 550tag에 추가한다.
	if(m_bIsPublisherChanged)  
	{
		m_pCM->GetControlMgrData(_T("발행자2"),strtmp);
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("550$a"),strtmp);
		m_bIsPublisherChanged = FALSE;
		strtmp = _T("");
	}


	m_pCM->GetControlMgrData(_T("CODEN"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("030$a"),strtmp);
	strtmp = _T("");
	
	m_pCM->GetControlMgrData(_T("크기"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("300$c"),strtmp);
	strtmp = _T("");
	
	m_pCM->GetControlMgrData(_T("면장수"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("300$a"),strtmp);
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
	
	//입력일자, 길이설정은  입력모드에서만 설정 
	if(m_nOpenMode>0) 
	{
		//길이 설정 (40자리)
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@39"),_T(" "));
		
		CTime t = CTime::GetCurrentTime();
		CString tmp,inputdate;
		tmp.Format(_T("%04d%02d%02d"),t.GetYear(),t.GetMonth(),t.GetDay());
		inputdate = tmp.Mid(2);
		m_pInfo->pMarcMgr->DeleteItem(&m_Marc,_T("008@0-5"));
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@0-5"),inputdate);
	}
	
	//발행년유형(간행상태)에 따라 폐간년도에 대한 설정을 해준다 
	CString strPubType;
	m_pCM->GetControlMgrData(_T("간행상태"),strPubType);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@6"),strPubType);
	
	
	//창간년 
	m_pCM->GetControlMgrData(_T("창간년"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@7-10"),strtmp);
	strtmp = _T("");

	
	//폐간년 
	if(strPubType == _T("d")) //폐간된 경우 
	{
		m_pCM->GetControlMgrData(_T("폐간년"),strtmp);
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@11-14"),strtmp);
		strtmp = _T("");
	}
	else 
	{
		m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@11-14"),_T("9999"));
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


	//정규성
	m_pCM->GetControlMgrData(_T("정규성"),strtmp);
	m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("008@19"),strtmp);
	strtmp = _T("");
	
	
	m_pInfo->pMarcMgr->DeleteEmptySubfield(&m_Marc);
	m_pInfo->pMarcMgr->DeleteEmptyField(&m_Marc);
	m_pInfo->pMarcMgr->CheckSimpleSyntax(&m_Marc);

	return 0;
}

INT CSeSpeciesForm::SetControlDataByMarc()
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

	
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("030$a"),strtmp);
	m_pCM->SetControlMgrData(_T("CODEN"),strtmp);
	strtmp = _T("");
	
	
	//발행자2(550 tag 의 맨마지막 데이터)
	CArray<CString,CString&> arrayPublisher;
	CString strPublisher550 = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("550$a"),strPublisher550,&arrayPublisher);
	
	INT nSize = arrayPublisher.GetSize();
	if(nSize>0)
	{
		strPublisher550 = arrayPublisher.GetAt(nSize-1);
	}

	if(!strPublisher550.IsEmpty())
	{
		m_pCM->SetControlMgrData(_T("발행자2"),strPublisher550);
	}

	
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("300$c"),strtmp);
	m_pCM->SetControlMgrData(_T("크기"),strtmp);
	strtmp = _T("");

	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("300$a"),strtmp);
	m_pCM->SetControlMgrData(_T("면장수"),strtmp);
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

	
	
	//간행빈도(310$a)처리: 설명으로 입력하는가?


	//008 TAG 처리
	////////////////////////////////////////////////////////////////////////////
	
	//발행년유형(간행상태)에 따라 폐간년도에 대한 설정을 해준다 
	CString strPubType = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@6"),strPubType);
	m_pCM->SetControlMgrData(_T("간행상태"),strPubType);
	
	
	//창간년 -- 빈칸의 경우는 trim해서 콘트롤에 저장
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@7-10"),strtmp);
	strtmp.TrimRight();
	strtmp.TrimLeft();
	m_pCM->SetControlMgrData(_T("창간년"),strtmp);
	strtmp = _T("");

	
	//폐간년 
	if(strPubType == _T("d")) //폐간된 경우만 폐간년 적용   -- 빈칸의 경우는 trim해서 콘트롤에 저장
	{
		m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@11-14"),strtmp);
		strtmp.TrimRight();
		strtmp.TrimLeft();
		m_pCM->SetControlMgrData(_T("폐간년"),strtmp);		
		strtmp = _T("");
	}


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


	//정규성
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@19"),strtmp);
	m_pCM->SetControlMgrData(_T("정규성"),strtmp);
	strtmp = _T("");
	
	

	return 0;
}


INT CSeSpeciesForm::SaveSpeciesDataToHistoryDM(CString strType, CString strOldData, CString strNewData, CString strReason, CString strDate)
{
	CString strSPECIES_KEY;
	strSPECIES_KEY = m_pDM->GetCellData(_T("SS_종KEY"),0);
	
	CString strREC_KEY;
	m_pDM->MakeRecKey(strREC_KEY);
	
	// 2008.05.16 ADD BY PDJ
	// 지우지 않으면 이전 데이터가 중복으로 저장되면서 에러.
	m_pDM_SPE_HISTORY->FreeData();
	//
	m_pDM_SPE_HISTORY->InsertRow(-1);
	INT nRow = m_pDM_SPE_HISTORY->GetRowCount()-1;
	
	m_pDM_SPE_HISTORY->SetCellData(_T("REC_KEY"),strREC_KEY,nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("종KEY"),strSPECIES_KEY,nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("타입"),strType,nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("선행정보"),strOldData,nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("현재정보"),strNewData,nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("변경일자"),strDate,nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("작업자"),m_pInfo->USER_ID,nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("변경사유"),strReason,nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("최초작업"),GetWorkLogMsg(WORK_LOG_SESPECIES),nRow);
	m_pDM_SPE_HISTORY->SetCellData(_T("마지막작업"),GetWorkLogMsg(WORK_LOG_SESPECIES),nRow);


	return 0;
}

INT CSeSpeciesForm::SaveSpeciesDataToDM()
{
	//DM으로 직접가지고올 data들
	const INT cnt = 19;
	CString alias[ cnt ] = {
		_T("SS_가격"),					//	0
		_T("SS_간행물등록번호"),		//	1
		_T("SS_간행빈도_코드"),			//	2
		_T("SS_간행상태_코드"),			//	3
		_T("SS_관리구분_코드"),			//	4
		_T("SS_등록구분_코드"),			//	5
		_T("SS_매체구분_코드"),			//	6
		_T("SS_배가위치_코드"),			//	7
		_T("SS_보조등록구분_코드"),		//	8
		_T("SS_이용대상구분_코드"),		//	9
		_T("SS_이용제한구분_코드"),		// 10
		_T("SS_자료구분_코드"),			// 11
		_T("SS_제어번호"),				// 12
		_T("SS_제어자료구분_코드"),		// 13
		_T("SS_형식구분_코드"),			// 14	
		_T("SS_화폐구분_코드"),			// 15
		_T("SS_연속분류코드1"),			// 16
		_T("SS_연속분류코드2"),			// 17
		_T("SS_연속분류코드3")};		// 18
	


	CString tmp;

	for(INT i=0;i<cnt;i++)
	{
		tmp = _T("");	
		m_pCM->GetControlMgrData(alias[i],tmp);
		m_pDM->SetCellData(alias[i],tmp,0);
	}
	
	UpdateData(TRUE);

	//가격성격
	tmp = _T("");
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbSPE_PRICE_PERSONALITY );
	pCombo->GetWindowText(tmp);
	m_pDM->SetCellData(_T("SS_가격성격"),tmp,0);

	
	//제본여부 
	if(m_bBINDING_YN)
		m_pDM->SetCellData(_T("SS_제본여부"),_T("Y"),0);
	else 
		m_pDM->SetCellData(_T("SS_제본여부"),_T("N"),0);

	//메모 
	m_pDM->SetCellData(_T("SS_메모"),m_strMEMO,0);

	return 0;
}

INT CSeSpeciesForm::SaveAllData()
{
	//API 함수들을 부르고 입력시 예측정보가 있는경우 예측정보를 저장한다
	INT ids = 0;
	CSeApi api(this);	
	
	if(m_nOpenMode>0) 
	{

		//입력 모드에서는 rec_key생성 
		CString strSPECIES_KEY,strACQ_KEY,strPUB_EXP_KEY,strACQ_EXP_KEY;
		m_pDM->MakeRecKey(strSPECIES_KEY);
		m_pDM->MakeRecKey(strACQ_KEY);
		m_pDM->MakeRecKey(strPUB_EXP_KEY);
		m_pDM->MakeRecKey(strACQ_EXP_KEY);

		if(strSPECIES_KEY.IsEmpty() || strACQ_KEY.IsEmpty() || strPUB_EXP_KEY.IsEmpty() || strACQ_EXP_KEY.IsEmpty())
		{
			AfxMessageBox(_T("데이터 키값 설정에 실패하였습니다. 다시 저장해 주십시요!")); 
			return -1;
		}
		
		m_pDM->SetCellData(_T("SS_종KEY"),strSPECIES_KEY,0);

		if(1 == m_nACQ_TYPE) { //구입입력
			
			
			m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_종KEY"),strSPECIES_KEY,0);
			m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_구입관리KEY"),strACQ_KEY,0);
			m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_발행예측정보KEY"),strPUB_EXP_KEY,0);
			m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_입수예측정보KEY"),strACQ_EXP_KEY,0);
			
			//일자 예측이 되었을 경우만 입력
			if(m_nAcqExpType>=0) m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_입수예측구분"),GetExpType(m_nAcqExpType),0);
			if(m_nPubExpType>=0) m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_발행예측구분"),GetExpType(m_nPubExpType),0);

			ids = api.SPECIES_PURCHASE_INSERT(m_pDM,m_pDM_IDX,m_pPurchaseDlg->m_pDM,m_pDM_SUB_HISTORY,&m_Marc);
	
		
		} else if(2 == m_nACQ_TYPE) { //기증입력
			
			m_pDonateDlg->m_pDM->SetCellData(_T("SD_종KEY"),strSPECIES_KEY,0);
			m_pDonateDlg->m_pDM->SetCellData(_T("SD_기증관리KEY"),strACQ_KEY,0);
			m_pDonateDlg->m_pDM->SetCellData(_T("SD_발행예측정보KEY"),strPUB_EXP_KEY,0);
			m_pDonateDlg->m_pDM->SetCellData(_T("SD_입수예측정보KEY"),strACQ_EXP_KEY,0);
			
			//일자 예측이 되었을 경우만 입력(예측정보 입력 에서 UPDATE문으로 바꾸는 것 고려)
			if(m_nAcqExpType>=0) m_pDonateDlg->m_pDM->SetCellData(_T("SD_입수예측구분"),GetExpType(m_nAcqExpType),0);
			if(m_nPubExpType>=0) m_pDonateDlg->m_pDM->SetCellData(_T("SD_발행예측구분"),GetExpType(m_nPubExpType),0);

			ids = api.SPECIES_DONATE_INSERT(m_pDM,m_pDM_IDX,m_pDonateDlg->m_pDM,&m_Marc);
			
		}
		

		//성공시에만 예측 정보 입력
		if(ids<0) return ids;

		if(m_bIsExpVerified) //예측정보가 검증 되었을 경우
		{
			INT ids2;
			CString strACQ_TYPE;
			strACQ_TYPE.Format(_T("%d"),m_nACQ_TYPE);

			ids2 = SaveExpectData(strPUB_EXP_KEY,strACQ_EXP_KEY,strACQ_KEY,strACQ_TYPE);
			if(ids2<0)
			{
				AfxMessageBox(_T("예측정보 초기 입력에 실패 하였습니다!"));
				return -2;
			}
		}

	}


	if(m_nOpenMode<0) //수정모드 
	{
		
 
		if(m_bIsDiscontinued) //폐간처리 되었을 경우 
		{
			ids = SaveDiscontinueData();
			return ids;
		}



		if(1 == m_nACQ_TYPE)  //구입 
		{

			if(_T("Y") == m_strPURCHASE_YN) //구입 수정 
			{
				//일자 예측이 되었을 경우만 입력
				if(m_nAcqExpType>=0) m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_입수예측구분"),GetExpType(m_nAcqExpType),0);
				if(m_nPubExpType>=0) m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_발행예측구분"),GetExpType(m_nPubExpType),0);
			
				ids = api.SPECIES_PURCHASE_UPDATE(m_pDM,
												  m_pDM_IDX,
												  m_pPurchaseDlg->m_pDM,
												  m_pDM_SUB_HISTORY,
												  m_pDM_SPE_HISTORY,
												  &m_Marc);

				if(ids<0) return ids;
		
				//예측 정보가 수정 되었을 경우는 예측 정보를 입력한다.
				if(m_pExpectDlg)
				{
					if(m_bIsExpVerified) 
					{
						//2004-12-24
						//modifield by loveisverb
						//ids = m_pExpectDlg->SaveAllData();
						ids = m_pExpectDlg->SaveAllData(m_nPubExpType,m_nAcqExpType);
					}
				}
				
				if(ids<0)
				{
					AfxMessageBox(_T("예측정보 수정에 실패 하였습니다!"));
					return -2;
				}
			
			}
			else if(_T("N") == m_strPURCHASE_YN) //구입 정보와 예측 정보만 입력
			{
				
				m_pDM->SetCellData(_T("SS_구입여부"),_T("Y"),0);
				CString strSPECIES_KEY,strACQ_KEY,strPUB_EXP_KEY,strACQ_EXP_KEY;
				strSPECIES_KEY = m_pDM->GetCellData(_T("SS_종KEY"),0);
				m_pDM->MakeRecKey(strACQ_KEY);
				m_pDM->MakeRecKey(strPUB_EXP_KEY);
				m_pDM->MakeRecKey(strACQ_EXP_KEY);
				

				m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_종KEY"),strSPECIES_KEY,0);
				m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_구입관리KEY"),strACQ_KEY,0);
				m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_발행예측정보KEY"),strPUB_EXP_KEY,0);
				m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_입수예측정보KEY"),strACQ_EXP_KEY,0);
			

				//일자 예측이 되었을 경우만 입력
				if(m_nAcqExpType>=0) m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_입수예측구분"),GetExpType(m_nAcqExpType),0);
				if(m_nPubExpType>=0) m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_발행예측구분"),GetExpType(m_nPubExpType),0);

				ids = api.SPECIES_PURCHASE_UPDATE_ACQ_CHANGE(m_pDM,
															 m_pDM_IDX,
															 m_pPurchaseDlg->m_pDM,
															 m_pDM_SUB_HISTORY,
															 m_pDM_SPE_HISTORY,
															 &m_Marc);
				
				if(ids<0) return ids;
	
				if(m_bIsExpVerified) //예측정보가 검증 되었을 경우
				{
					INT ids2;
					CString strACQ_TYPE;
					strACQ_TYPE.Format(_T("%d"),m_nACQ_TYPE);

					ids2 = SaveExpectData(strPUB_EXP_KEY,strACQ_EXP_KEY,strACQ_KEY,strACQ_TYPE);
					if(ids2<0)
					{
						AfxMessageBox(_T("예측정보 초기 입력에 실패 하였습니다!"));
						return -2;
					}
				}

			}
		
		}
		else if(2 == m_nACQ_TYPE) //기증 
		{

			if(_T("Y") == m_strDONATE_YN) //기증 수정 
			{
				//일자 예측이 새로이 되었을 경우만 입력(예측정보 입력 에서 UPDATE문으로 바꾸는 것 고려)
				if(m_nAcqExpType>=0) m_pDonateDlg->m_pDM->SetCellData(_T("SD_입수예측구분"),GetExpType(m_nAcqExpType),0);
				if(m_nPubExpType>=0) m_pDonateDlg->m_pDM->SetCellData(_T("SD_발행예측구분"),GetExpType(m_nPubExpType),0);
					
				ids = api.SPECIES_DONATE_UPDATE(m_pDM,
												m_pDM_IDX,
												m_pDonateDlg->m_pDM,
												m_pDM_SPE_HISTORY,
												&m_Marc);
				
				if(ids<0) return ids;

				//예측 정보가 수정 되었을 경우는 예측 정보를 입력한다.
				if(m_pExpectDlg)
				{
					//2004-12-24
					//modifield by loveisverb
					//ids = m_pExpectDlg->SaveAllData();
					ids = m_pExpectDlg->SaveAllData(m_nPubExpType,m_nAcqExpType);
				}
				
				if(ids<0)
				{
					AfxMessageBox(_T("예측정보 수정에 실패 하였습니다!"));
					return -2;
				}

			}
			else if(_T("N") == m_strDONATE_YN) //기증 정보와 예측 정보만 입력
			{
				
				m_pDM->SetCellData(_T("SS_기증여부"),_T("Y"),0);
				CString strSPECIES_KEY,strACQ_KEY,strPUB_EXP_KEY,strACQ_EXP_KEY;
				strSPECIES_KEY = m_pDM->GetCellData(_T("SS_종KEY"),0);
				m_pDM->MakeRecKey(strACQ_KEY);
				m_pDM->MakeRecKey(strPUB_EXP_KEY);
				m_pDM->MakeRecKey(strACQ_EXP_KEY);
				
				m_pDonateDlg->m_pDM->SetCellData(_T("SD_종KEY"),strSPECIES_KEY,0);
				m_pDonateDlg->m_pDM->SetCellData(_T("SD_기증관리KEY"),strACQ_KEY,0);
				m_pDonateDlg->m_pDM->SetCellData(_T("SD_발행예측정보KEY"),strPUB_EXP_KEY,0);
				m_pDonateDlg->m_pDM->SetCellData(_T("SD_입수예측정보KEY"),strACQ_EXP_KEY,0);
				
				//일자 예측이 되었을 경우만 입력(예측정보 입력 에서 UPDATE문으로 바꾸는 것 고려)
				if(m_nAcqExpType>=0) m_pDonateDlg->m_pDM->SetCellData(_T("SD_입수예측구분"),GetExpType(m_nAcqExpType),0);
				if(m_nPubExpType>=0) m_pDonateDlg->m_pDM->SetCellData(_T("SD_발행예측구분"),GetExpType(m_nPubExpType),0);

				ids = api.SPECIES_DONATE_UPDATE_ACQ_CHANGE(m_pDM,
														   m_pDM_IDX,
														   m_pDonateDlg->m_pDM,
														   m_pDM_SPE_HISTORY,
														   &m_Marc);
				
				if(ids<0) return ids;

				if(m_bIsExpVerified) //예측정보가 검증 되었을 경우
				{
					INT ids2;
					CString strACQ_TYPE;
					strACQ_TYPE.Format(_T("%d"),m_nACQ_TYPE);

					ids2 = SaveExpectData(strPUB_EXP_KEY,strACQ_EXP_KEY,strACQ_KEY,strACQ_TYPE);
					if(ids2<0)
					{
						AfxMessageBox(_T("예측정보 초기 입력에 실패 하였습니다!"));
						return -2;
					}
				}

			}
		}
	}
	

	//부서별 주문자료 저장
	if(1 == m_nACQ_TYPE)  
	{
		m_pPurchaseDlg->SaveDepartmentOrderInfo();
	}

	return ids;
}


INT CSeSpeciesForm::SaveDiscontinueData()
{

	if(!m_pDiscontinueDlg) {
		AfxMessageBox(_T("폐간정보를 찾을 수 없습니다!"));	
		return -1;
	}

	INT ids = 0;
	CSeApi api(this);

	CString strSPECIES_KEY,strACQ_KEY,strPUB_EXP_KEY,strACQ_EXP_KEY;
	m_pDM->MakeRecKey(strSPECIES_KEY);
	m_pDM->MakeRecKey(strACQ_KEY);
	m_pDM->MakeRecKey(strPUB_EXP_KEY);
	m_pDM->MakeRecKey(strACQ_EXP_KEY);
		
	m_pDM->SetCellData(_T("SS_종KEY"),strSPECIES_KEY,0);

	if(1 == m_nACQ_TYPE) //구입입력
	{ 
			
		m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_종KEY"),strSPECIES_KEY,0);
		m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_구입관리KEY"),strACQ_KEY,0);
		m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_발행예측정보KEY"),strPUB_EXP_KEY,0);
		m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_입수예측정보KEY"),strACQ_EXP_KEY,0);
				
		//일자 예측이 되었을 경우만 입력
		if(m_nAcqExpType>=0) m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_입수예측구분"),GetExpType(m_nAcqExpType),0);
		if(m_nPubExpType>=0) m_pPurchaseDlg->m_pDM->SetCellData(_T("SP_발행예측구분"),GetExpType(m_nPubExpType),0);

		ids = api.SPECIES_PURCHASE_INSERT_DISCONTINUE(m_pDM,
													  m_pDM_IDX,
													  m_pPurchaseDlg->m_pDM,
													  m_pDM_SUB_HISTORY,
													  &m_Marc,
													  &m_pDiscontinueDlg->m_OldMarc,
													  m_pDiscontinueDlg->m_strSPECIES_KEY,
													  m_pDiscontinueDlg->m_strDiscontinueReason);
		
		
	} 
	else if(2 == m_nACQ_TYPE) //기증입력
	{ 
			
		m_pDonateDlg->m_pDM->SetCellData(_T("SD_종KEY"),strSPECIES_KEY,0);
		m_pDonateDlg->m_pDM->SetCellData(_T("SD_기증관리KEY"),strACQ_KEY,0);
		m_pDonateDlg->m_pDM->SetCellData(_T("SD_발행예측정보KEY"),strPUB_EXP_KEY,0);
		m_pDonateDlg->m_pDM->SetCellData(_T("SD_입수예측정보KEY"),strACQ_EXP_KEY,0);
				
		//일자 예측이 되었을 경우만 입력(예측정보 입력 에서 UPDATE문으로 바꾸는 것 고려)
		if(m_nAcqExpType>=0) m_pDonateDlg->m_pDM->SetCellData(_T("SD_입수예측구분"),GetExpType(m_nAcqExpType),0);
		if(m_nPubExpType>=0) m_pDonateDlg->m_pDM->SetCellData(_T("SD_발행예측구분"),GetExpType(m_nPubExpType),0);
		
		
		ids = api.SPECIES_DONATE_INSERT_DISCONTINUE(m_pDM,
													m_pDM_IDX,
													m_pDonateDlg->m_pDM,
													&m_Marc,
													&m_pDiscontinueDlg->m_OldMarc,
													m_pDiscontinueDlg->m_strSPECIES_KEY,
													m_pDiscontinueDlg->m_strDiscontinueReason);											
			
	
	}
	
	//성공시에만 예측 정보 입력
	if(ids<0) return ids;

	if(m_bIsExpVerified) //예측정보가 검증 되었을 경우
	{
		INT ids2;
		CString strACQ_TYPE;
		strACQ_TYPE.Format(_T("%d"),m_nACQ_TYPE);

		ids2 = SaveExpectData(strPUB_EXP_KEY,strACQ_EXP_KEY,strACQ_KEY,strACQ_TYPE);
		if(ids2<0)
		{
			AfxMessageBox(_T("예측정보 초기 입력에 실패 하였습니다!"));
			return -2;
		}
	}


	return 0;
}

CString CSeSpeciesForm::GetExpType(INT ExpType)
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

INT CSeSpeciesForm::GetIntExpType(CString strExpType)
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

CString CSeSpeciesForm::GetTodayDate()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

INT CSeSpeciesForm::SaveExpectData(CString strPUB_EXP_KEY, CString strACQ_EXP_KEY, CString strACQ_KEY,CString strACQ_TYPE)
{
	//입력모드에서만 사용하는 함수로 
	//예측정보들을 일괄 입력 한다 
	//이미 데이터의 검증은 예측 모듈에서 끝났다는 전제이다
	CESL_DataMgr* pDM_DATE = NULL;
	CString strTBL_NAME = _T("");

	switch(m_nAcqExpType)
	{
	case 0:
		pDM_DATE = FindDM(_T("DM_종_월간예측"));
		strTBL_NAME = _T("SE_MONTH_EXP_TBL");
		break;
	case 1:
		pDM_DATE = FindDM(_T("DM_종_주간예측"));
		strTBL_NAME = _T("SE_WEEK_EXP_TBL");
		break;
	case 2:
		pDM_DATE = FindDM(_T("DM_종_일간예측"));
		strTBL_NAME = _T("SE_DAY_EXP_TBL");
		break;
	case 3:
		pDM_DATE = FindDM(_T("DM_종_기타예측"));
		strTBL_NAME = _T("SE_ETC_EXP_TBL");
		break;
	}

	
	CESL_DataMgr* pDM_VOL = FindDM(_T("DM_연속_종_예측_권호"));
	CESL_DataMgr* pDM_VOL_SUPPORT = FindDM(_T("DM_연속_종_예측_부차적권호"));
	CESL_DataMgr* pDM_APPENDIX = FindDM(_T("DM_연속_종_예측_부록"));

	if(!pDM_DATE || !pDM_VOL || !pDM_VOL_SUPPORT || !pDM_APPENDIX) {
		AfxMessageBox(_T("종 예측 DM 들을 찾을 수 없습니다"));
		return -1;
	}

	pDM_DATE->StartFrame();
	CArray<CString,CString> RemoveAliasList;
	CString strEXP_CLASS = _T("");

	for(INT i=0;i<pDM_DATE->GetRowCount();i++)
	{
		RemoveAliasList.RemoveAll();
		pDM_DATE->InitDBFieldData();
		strEXP_CLASS = pDM_DATE->GetCellData(_T("EXP_CLASS"),i);
	
		//해당되는 KEY를 입력 한다.
		if(_T("0") == strEXP_CLASS) pDM_DATE->SetCellData(_T("REC_KEY"),strACQ_EXP_KEY,i);
		else if(_T("1") == strEXP_CLASS) pDM_DATE->SetCellData(_T("REC_KEY"),strPUB_EXP_KEY,i);

		pDM_DATE->MakeQueryByDM(RemoveAliasList, pDM_DATE , strTBL_NAME , i, pDM_DATE);
		
	}		
	
	CString strREC_KEY = _T("");
	INT nEXP_LEVEL_NO = 0;
	CString strEXP_LEVEL_NO;
	

	for(i=0;i<pDM_VOL->GetRowCount();i++)
	{
		RemoveAliasList.RemoveAll();
		pDM_DATE->InitDBFieldData();
		pDM_DATE->MakeRecKey(strREC_KEY);			
		strEXP_LEVEL_NO.Format(_T("%d"),i+1);

		//수입구분과, 수입구분KEY, 권호예측KEY(REC_KEY),단계번호를 넣어준다
		pDM_VOL->SetCellData(_T("단계번호"),strEXP_LEVEL_NO,i);
		pDM_VOL->SetCellData(_T("수입구분"),strACQ_TYPE,i);
		pDM_VOL->SetCellData(_T("수입구분KEY"),strACQ_KEY,i);
		pDM_VOL->SetCellData(_T("권호예측KEY"),strREC_KEY,i);
		pDM_DATE->MakeQueryByDM(RemoveAliasList, pDM_VOL , _T("SE_VOL_EXP_TBL") , i, pDM_DATE);
		
		strREC_KEY = _T("");	
		nEXP_LEVEL_NO++;
	}		
	
	nEXP_LEVEL_NO = 0;

	for(i=0;i<pDM_VOL_SUPPORT->GetRowCount();i++)
	{
		RemoveAliasList.RemoveAll();
		pDM_DATE->InitDBFieldData();
		pDM_DATE->MakeRecKey(strREC_KEY);
		strEXP_LEVEL_NO.Format(_T("%d"),i+1);
	
		//수입구분과 수입구분KEY,권호예측KEY(REC_KEY),단계번호를 넣어준다
		pDM_VOL_SUPPORT->SetCellData(_T("단계번호"),strEXP_LEVEL_NO,i);
		pDM_VOL_SUPPORT->SetCellData(_T("수입구분"),strACQ_TYPE,i);
		pDM_VOL_SUPPORT->SetCellData(_T("수입구분KEY"),strACQ_KEY,i);
		pDM_VOL_SUPPORT->SetCellData(_T("권호예측KEY"),strREC_KEY,i);
		pDM_DATE->MakeQueryByDM(RemoveAliasList, pDM_VOL_SUPPORT , _T("SE_VOL_EXP_TBL") , i, pDM_DATE);

		strREC_KEY = _T("");
		nEXP_LEVEL_NO++;
	}	
	
	for(i=0;i<pDM_APPENDIX->GetRowCount();i++)
	{
		RemoveAliasList.RemoveAll();
		pDM_DATE->InitDBFieldData();
		pDM_DATE->MakeRecKey(strREC_KEY);	
	
		//수입구분과 수입구분KEY,부록예측KEY(REC_KEY)를 넣어준다
		pDM_APPENDIX->SetCellData(_T("수입구분"),strACQ_TYPE,i);
		pDM_APPENDIX->SetCellData(_T("수입구분KEY"),strACQ_KEY,i);
		pDM_APPENDIX->SetCellData(_T("부록예측KEY"),strREC_KEY,i);
		
		pDM_DATE->MakeQueryByDM(RemoveAliasList, pDM_APPENDIX , _T("SE_APPENDIX_EXP_TBL") , i, pDM_DATE);
		strREC_KEY = _T("");
	}	
	
	

	//발행예측과 입수예측이 일치하지 않는 경우 발행일예측추가입력
	if(m_nAcqExpType != m_nPubExpType) 
	{
		switch(m_nPubExpType)
		{
		case 0:
			pDM_DATE = FindDM(_T("DM_종_월간예측"));
			strTBL_NAME = _T("SE_MONTH_EXP_TBL");
			break;
		case 1:
			pDM_DATE = FindDM(_T("DM_종_주간예측"));
			strTBL_NAME = _T("SE_WEEK_EXP_TBL");
			break;
		case 2:
			pDM_DATE = FindDM(_T("DM_종_일간예측"));
			strTBL_NAME = _T("SE_DAY_EXP_TBL");
			break;
		case 3:
			pDM_DATE = FindDM(_T("DM_종_기타예측"));
			strTBL_NAME = _T("SE_ETC_EXP_TBL");
			break;
		}

		strEXP_CLASS = _T("");

		for(i=0;i<pDM_DATE->GetRowCount();i++)
		{
			RemoveAliasList.RemoveAll();
			pDM_DATE->InitDBFieldData();
			strEXP_CLASS = pDM_DATE->GetCellData(_T("EXP_CLASS"),i);
		
			//해당되는 KEY를 입력 한다.
			if(_T("0") == strEXP_CLASS) pDM_DATE->SetCellData(_T("REC_KEY"),strACQ_EXP_KEY,i);
			else if(_T("1") == strEXP_CLASS) pDM_DATE->SetCellData(_T("REC_KEY"),strPUB_EXP_KEY,i);

			pDM_DATE->MakeQueryByDM(RemoveAliasList, pDM_DATE , strTBL_NAME , i, pDM_DATE);
		
		}		
	}
	
	INT ids;
	ids = pDM_DATE->SendFrame();
	pDM_DATE->EndFrame();

	return ids;
}

VOID CSeSpeciesForm::OnbtnMARCEDIT() 
{
	// TODO: Add your control notification handler code here

	SetMarcByControlData();
	CSeMarcEdit dlg(this);
	dlg.m_pMarc = &m_Marc;
	if(dlg.DoModal() == IDOK) SetControlDataByMarc();
	
}

VOID CSeSpeciesForm::OnbtnMEMO() 
{
	// TODO: Add your control notification handler code here
	
	CSeMemoEdit dlg(this);
	dlg.m_strMEMO = m_strMEMO;
	if(dlg.DoModal() == IDOK) m_strMEMO = dlg.m_strMEMO;

}

VOID CSeSpeciesForm::OnbtnEXPECT() 
{
	// TODO: Add your control notification handler code here
	CString strPUB_FREQ = _T("");
	CString strTITLE = _T("");
	m_pCM->GetControlMgrData(_T("간행빈도"),strPUB_FREQ);
	m_pCM->GetControlMgrData(_T("서명"),strTITLE);
	INT nOpenMode;
	BOOL bCopyType = FALSE;

	if(m_nOpenMode<0)
	{
		if(1 == m_nACQ_TYPE)  //구입 
		{
			if(_T("Y") == m_strPURCHASE_YN) nOpenMode = m_nOpenMode;
			else if(_T("N") == m_strPURCHASE_YN) 
			{ 
				nOpenMode = 1;
				bCopyType = TRUE;
			}
		}
		else if(2 == m_nACQ_TYPE) //기증 
		{
			if(_T("Y") == m_strDONATE_YN) nOpenMode = m_nOpenMode;
			else if(_T("N") == m_strDONATE_YN)
			{ 
				nOpenMode = 1;
				bCopyType = TRUE;
			}
		}
	}
	else 
	{
		nOpenMode = m_nOpenMode;
	}
	

	if(false == m_bIsExpCreated) {
		
		m_pExpectDlg = new CSeExpectForm(this);
		m_pExpectDlg->m_nOpenMode = nOpenMode;
		m_pExpectDlg->m_nACQ_TYPE = m_nACQ_TYPE;
		m_pExpectDlg->m_strSPECIES_KEY = m_strSpeciesKey;
		m_pExpectDlg->m_strPUB_FREQ = strPUB_FREQ;
		m_pExpectDlg->m_strTITLE = strTITLE;
		m_pExpectDlg->m_bIsVerified = m_bIsExpVerified; 

		if(bCopyType) {
			m_pExpectDlg->m_nAcqType = m_nAcqExpType;
			m_pExpectDlg->m_nPubType = m_nPubExpType;
		}

		if(m_pExpectDlg->DoModal() == IDOK)
		{
			//일자 예측정보 유형을 가져온다
			m_nPubExpType = m_pExpectDlg->m_nPubType;
			m_nAcqExpType = m_pExpectDlg->m_nAcqType;
			m_bIsExpVerified = TRUE;
		}
		
		m_bIsExpCreated = true;
	}
	else  
	{
		m_pExpectDlg->m_bIsFirstCreated = false;
		m_pExpectDlg->m_strPUB_FREQ = strPUB_FREQ;
		m_pExpectDlg->m_strTITLE = strTITLE;
		m_pExpectDlg->m_strSPECIES_KEY = m_strSpeciesKey;
		m_pExpectDlg->m_bIsVerified = m_bIsExpVerified; //검증 여부 

		if(m_pExpectDlg->DoModal() == IDOK)
		{
			//일자 예측정보 유형을 가져온다
			m_nPubExpType = m_pExpectDlg->m_nPubType;
			m_nAcqExpType = m_pExpectDlg->m_nAcqType;
			m_bIsExpVerified = TRUE;
		}
	}
	
}

VOID CSeSpeciesForm::OnbtnHISTORY() 
{
	// TODO: Add your control notification handler code here
	CSE_HISTORY_MAIN dlg(this);
	dlg.sCurrentKey = m_pDM_IDX->GetCellData(_T("ISS_종KEY"),0);
	dlg.sCurrentTitle = m_pDM_IDX->GetCellData(_T("ISS_표제사항"),0);
	dlg.DoModal();

}

VOID CSeSpeciesForm::OnbtnMARCIMPORT() 
{
	// TODO: Add your control notification handler code here
	if(m_nOpenMode<0) return;

	CSeMarcImport dlg(this);
	if(dlg.DoModal() != IDOK) return;
	
	
	INT nType = dlg.m_nImportOption;
	
	if(0 == nType) //기본정보 반입 
	{

		m_Marc = dlg.m_Marc;
		SetControlDataByMarc();
		m_Marc.Init();
		SetMarcByControlData();

	}
	else if(1 == nType) //전체 정보 반입
	{

		m_Marc = dlg.m_Marc;
		SetControlDataByMarc();

	}

}

VOID CSeSpeciesForm::OnOK() 
{
	// 2007.04.13 ADD BY PDJ
	// 기증자 검증
	/*
	CString strDonator;
	CString strQuery;
	CString Result;
	INT		nCount;
	m_pDonateDlg->m_pCM->GetControlMgrData(_T("SD_기증자명"),strDonator);
	strDonator.Replace(_T(" "),_T(""));
	if ( !strDonator.IsEmpty() )
	{
		strQuery.Format(_T("SELECT COUNT(1) FROM CO_DONATOR_TBL WHERE DONATOR = '%s'"), strDonator);
		m_pDM->GetOneValueQuery(strQuery, Result);

		nCount = _ttoi(Result.GetBuffer(0));
		if ( nCount == 0 )
		{
			AfxMessageBox(_T("관리되지 않는 기증자 정보는 저장할 수 없습니다."));
			return;
		}
		else if ( nCount > 1 )
		{
			AfxMessageBox(_T("기증자이름이 중복됩니다. \n 검색하여 선정해 주십시오"));
			return;
		}
	}
	*/
	
	//필수 입력 검증

	//서명
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

	
	//자료실
	CString strSHELF_LOC_CODE;
	m_pCM->GetControlMgrData(_T("SS_배가위치_코드"),strSHELF_LOC_CODE);

	if(strSHELF_LOC_CODE.IsEmpty()) 
	{
		AfxMessageBox(_T("자료실이 입력되지 않았습니다!"));
		return;
	}
	
	//간행빈도 
	CString strPUB_FREQ;
	m_pCM->GetControlMgrData(_T("간행빈도"),strPUB_FREQ);
	strPUB_FREQ.TrimLeft();
	strPUB_FREQ.TrimRight();

	if(strPUB_FREQ.IsEmpty())
	{
		AfxMessageBox(_T("간행빈도가 입력되지 않았습니다!"));
		return;
	}
	
	//간행상태 
	CString strPUB_STATUS;
	m_pCM->GetControlMgrData(_T("간행상태"),strPUB_STATUS);
	strPUB_STATUS.TrimLeft();
	strPUB_STATUS.TrimRight();

	if(strPUB_STATUS.IsEmpty())
	{
		AfxMessageBox(_T("간행상태가 입력되지 않았습니다!"));
		return;
	}

	//창간년,폐간년 (존재할경우 4자리 check)
	CString strFoundationYear, strDiscontinueYear;
	m_pCM->GetControlMgrData(_T("창간년"),strFoundationYear);
	m_pCM->GetControlMgrData(_T("폐간년"),strDiscontinueYear);
	
	strFoundationYear.TrimLeft();
	strFoundationYear.TrimRight();
	strDiscontinueYear.TrimLeft();
	strDiscontinueYear.TrimRight();
	INT nLength = 0;

	if(!strFoundationYear.IsEmpty())
	{
		nLength = strFoundationYear.GetLength();
		if(4 != nLength) 
		{
			AfxMessageBox(_T("창간년에 4자리의 값을 넣어 주십시요!"));
			return;
		}
	}

	if(!strDiscontinueYear.IsEmpty())
	{
		nLength = strDiscontinueYear.GetLength();
		if(4 != nLength) 
		{
			AfxMessageBox(_T("폐간년에 4자리의 값을 넣어 주십시요!"));
			return;
		}
	}


	//입수책수
	CString strACQ_BOOK_CNT;
	if(1 == m_nACQ_TYPE) m_pPurchaseDlg->m_pCM->GetControlMgrData(_T("SP_입수책수"),strACQ_BOOK_CNT);
	else if(2 == m_nACQ_TYPE) m_pDonateDlg->m_pCM->GetControlMgrData(_T("SD_입수책수"),strACQ_BOOK_CNT);

	strACQ_BOOK_CNT.TrimLeft();
	strACQ_BOOK_CNT.TrimRight();

	if(strACQ_BOOK_CNT.IsEmpty())
	{
		AfxMessageBox(_T("입수책수가 입력되지 않았습니다!"));
		return;
	}
	else 
	{
		INT nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
		if(nACQ_BOOK_CNT<1)
		{
			AfxMessageBox(_T("입수책수는 1 이상이 되야 합니다!"));
			return;
		}
	}

	// 2008.05.15 ADD BY PDJ
	// 구독기간의 날짜가 정상적으로 입력되도록함.
	if ( m_pPurchaseDlg != NULL )
	{
		CString strStartDate;
		CString strEndDate;
		m_pPurchaseDlg->m_pCM->GetControlMgrData(_T("SP_구독시작일"),strStartDate);
		m_pPurchaseDlg->m_pCM->GetControlMgrData(_T("SP_구독만기일"),strEndDate);
		if ( strStartDate > strEndDate )
		{
			AfxMessageBox(_T("구독기간이 잘못되었습니다."));
			return;
		}
	}

	//DM과 마크 구조체에 데이터를 넣는다.
	SaveAllDataToDM();

	//API를 통해 DB operation을 한다	
	if(SaveAllData()<0) 
	{
		AfxMessageBox(_T("저장 실패!"));
	}
	else 
	{
		AfxMessageBox(_T("저장되었습니다"));	
		//부모화면 refresh
		RefreshParent();
	}
	
	if(m_nOpenMode>0) //입력
	{
		//모든 정보를 초기화 한다. 
		
		//화면을 지운다
		OnbtnALLCLEAR();

		//마크를 지운다.
		m_Marc.Init();
		
		//메모를 지운다 
		m_strMEMO = _T("");

		//제어번호 
		m_pCM->SetControlMgrData(_T("SS_제어번호"),_T("<<자동 부여>>")); 

		//예측정보를 지운다.
		if(m_pExpectDlg) delete m_pExpectDlg;
		m_pExpectDlg = NULL;
		m_bIsExpCreated = FALSE;
		m_bIsExpVerified = FALSE;
		m_nPubExpType = -1;  
		m_nAcqExpType = -1;  
		
		//DM 을 모두 지운다 
		FreeAllDM();

		return;
	}
	else //수정 
	{
		//폐간여부
		if(m_pDiscontinueDlg)
		{
			delete m_pDiscontinueDlg;
			m_pDiscontinueDlg = NULL;
		}

		m_bIsDiscontinued = FALSE;
	
		//종변경 여부 
		m_bIsTitleChanged = FALSE;

		//발행자 변경 여부
		m_bIsPublisherChanged = FALSE;

		
		//loveisverb
		//예측정보를 지운다.
		if(m_pExpectDlg) delete m_pExpectDlg;
		m_pExpectDlg = NULL;
		m_bIsExpCreated = FALSE;
		m_bIsExpVerified = FALSE;
		m_nPubExpType = -1;  
		m_nAcqExpType = -1; 
	
		//예측관련 DM 을 모두 지운다 
		FreeAllDM();

		//현재 종의 상태
		m_strPURCHASE_YN = m_pDM->GetCellData(_T("SS_구입여부"),0);
		m_strDONATE_YN = m_pDM->GetCellData(_T("SS_기증여부"),0);

		return;
	}

	CDialog::OnOK();
}

VOID CSeSpeciesForm::OnbtnPREV() 
{
	// TODO: Add your control notification handler code here
	
	
	//입력모드일 경우는 이전내용 불러오기
	if(m_nOpenMode>0)
	{
		BringPrevData();
		return;
	}
	
	
	
	if(1 == m_nMoveOption)
	{
		m_nCurrentIdx--;

		if(m_nCurrentIdx<0)
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

	m_strSpeciesKey = m_pParentDM->GetCellData(_T("ISS_종KEY"),m_nCurrentIdx);
	if(!m_strSpeciesKey.IsEmpty())
			RefreshAll(m_strSpeciesKey);

	m_pParentGrid->SetReverse(m_nCurrentIdx);
	
}

VOID CSeSpeciesForm::OnbtnNEXT() 
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


	m_strSpeciesKey = m_pParentDM->GetCellData(_T("ISS_종KEY"),m_nCurrentIdx);
	if(!m_strSpeciesKey.IsEmpty())
			RefreshAll(m_strSpeciesKey);

	m_pParentGrid->SetReverse(m_nCurrentIdx);

}

VOID CSeSpeciesForm::OnbtnALLCLEAR() 
{
	// TODO: Add your control notification handler code here
	m_pCM->AllControlClear();
	m_bBINDING_YN = FALSE;
	UpdateData(FALSE);

	
	if(1 == m_nACQ_TYPE) m_pPurchaseDlg->ClearAll();
	else if(2 == m_nACQ_TYPE) m_pDonateDlg->ClearAll();

}

VOID CSeSpeciesForm::OnbtnSPETITLECHANGE() 
{
	// TODO: Add your control notification handler code here
	// 서명 수정 
	// 종의 폐간 여부를 묻는다 
	
	CString msg;
	msg.Format(_T("현재 편집중인 종을 폐간하시겠습니까?"));
	
	INT nReturn = AfxMessageBox(msg,MB_YESNOCANCEL);

	//마크 그룹 설정
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	if(IDYES == nReturn) //폐간 처리 
	{
		
		//간행상태를 파악해 이미 폐간 처리 된 종인지를 확인한다
		CString tmp;
		m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@6"),tmp); 
		if(_T("d") == tmp) {
			AfxMessageBox(_T("이미 폐간된 종입니다!"));
			return;
		}

		m_pDiscontinueDlg = new CSeTitleChangeDiscontinueDlg(this);

		//DM
		m_pDiscontinueDlg->m_pDM = m_pDM;

		CString strOldTitle = _T("");
		CString strOldISSN1 = _T("");
		CString strOldISSN2 = _T("");
		CString strNewTitle = _T("");
		CString strNewISSN2 = _T("");
		CString strNewISSN1 = _T("");
		CString strISSN = _T("");
		
		//서명 
		m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("245$a"),strOldTitle);
		m_pDiscontinueDlg->m_strOldTitle = strOldTitle;
		
		//ISSN
		m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("022$a"),strISSN);
		INT npos = strISSN.Find(_T("-"));
		if(npos>0)
		{
			strOldISSN1 = strISSN.Left(npos);
			strOldISSN2 = strISSN.Mid(npos+1);

			strOldISSN1.TrimLeft();	strOldISSN1.TrimRight();
			strOldISSN2.TrimLeft();	strOldISSN2.TrimRight();
			
			m_pDiscontinueDlg->m_strOldISSN1 = strOldISSN1;
			m_pDiscontinueDlg->m_strOldISSN2 = strOldISSN2;
		}

		//마크 
		m_pDiscontinueDlg->m_OldMarc = m_Marc;
		
		//마크 포인터 
		m_pDiscontinueDlg->m_pNewMarc = &m_Marc;

		//기타 정보.
		m_pDiscontinueDlg->m_nACQ_TYPE = m_nACQ_TYPE;

		if(m_pDiscontinueDlg->DoModal() != IDOK) return;
		
		//기존 정보 갱신 
		m_pCM->SetControlMgrData(_T("서명"),m_pDiscontinueDlg->m_strNewTitle);
		m_pCM->SetControlMgrData(_T("ISSN1"),m_pDiscontinueDlg->m_strNewISSN1);
		m_pCM->SetControlMgrData(_T("ISSN2"),m_pDiscontinueDlg->m_strNewISSN2);

		//폐간 처리 한다
		m_bIsDiscontinued = TRUE;

		//기존 예측정보를 가지고 온다 
		BringExpData(m_pDiscontinueDlg->m_strSPECIES_KEY,m_nACQ_TYPE);
		
		AfxMessageBox(_T("새로운 종 정보를 입력해 주십시오!"));
		
	}
	else if(IDNO == nReturn) //일반 서명 수정 
	{

		CSeTitleChangeDlg dlg(this);
		dlg.m_pDM = m_pDM;

		CString strOldData = _T("");
		CString strNewData = _T("");

		//기존 데이터 
		m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("245$a"),strOldData);
		dlg.m_strOldData = strOldData;

		if(dlg.DoModal() != IDOK) return;
		
		//현재 데이터
		strNewData = dlg.m_strNewData;
		
		strNewData.TrimLeft();
		strNewData.TrimRight();

		//서명 변경 옵션을 파악한다 
		INT nOption = dlg.m_nRadioOption;
		
		if(0 == nOption) //245$a에 덮어씀 
		{
			m_pCM->SetControlMgrData(_T("서명"),strNewData);
			m_pInfo->pMarcMgr->DeleteItem(&m_Marc,_T("245$a"));	
			
			if(strNewData.GetAt(0) == '(') //관칭이 있을 경우에는 지시기호 20
			{
				m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$a!20"),strNewData);
			}
			else
			{
				m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$a"),strNewData);
			}
			
			m_bIsTitleChanged = TRUE;
		}
		else if(1 == nOption) //247$a로 기존 서명 이동
		{
			m_pCM->SetControlMgrData(_T("서명"),strNewData);
			m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("247$a"),strOldData);
			m_pInfo->pMarcMgr->DeleteItem(&m_Marc,_T("245$a"));	

			if(strNewData.GetAt(0) == '(') //관칭이 있을 경우에는 지시기호 20
			{
				m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$a!20"),strNewData);
			}
			else
			{
				m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("245$a"),strNewData);
			}

			SaveSpeciesDataToHistoryDM(_T("1"),strOldData,strNewData,dlg.m_strReason,dlg.m_strDate);
			m_bIsTitleChanged = TRUE;
		}
		else if(2 == nOption) //246$a에 변경 표제 기록
		{
			m_pInfo->pMarcMgr->SetItem(&m_Marc,_T("246$a"),strNewData);	
			SaveSpeciesDataToHistoryDM(_T("1"),strOldData,strNewData,dlg.m_strReason,dlg.m_strDate);
		}

	}


}

VOID CSeSpeciesForm::OnbtnSPEMEDIACODECHANGE() 
{
	// TODO: Add your control notification handler code here
	// 매체 구분 수정 	
	CSeMediaChangeDlg dlg(this);
	dlg.m_pDM = m_pDM;

	CString strOldData = _T("");
	CString strNewData = _T("");

	//기존 데이터 
	strOldData = m_pDM->GetCellData(_T("SS_매체구분_코드"),0);
	dlg.m_strOldData = strOldData;

	if(dlg.DoModal() != IDOK) return;
	
	//현재 데이터
	strNewData = dlg.m_strNewData;
	m_pCM->SetControlMgrData(_T("SS_매체구분_코드"),strNewData);

	SaveSpeciesDataToHistoryDM(_T("3"),strOldData,strNewData,dlg.m_strReason,dlg.m_strDate);


}


VOID CSeSpeciesForm::OnbtnSPEPUBLISHERCHANGE() 
{
	// TODO: Add your control notification handler code here
	// 발행자 수정 
	CSePublisherChangeDlg dlg(this);
	dlg.m_pDM = m_pDM;

	CString strOldData = _T("");
	CString strNewData = _T("");
	

	//기존 데이터 (550이 있을 경우는 550 아니면 260) 
	//550 tag 의 맨마지막 데이터
	CArray<CString,CString&> arrayPublisher;
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("550$a"),strOldData,&arrayPublisher);
	
	INT nSize = arrayPublisher.GetSize();
	
	if(nSize>0)
	{
		strOldData = arrayPublisher.GetAt(nSize-1);
	}
	else 
	{
		m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("260$b"),strOldData);
	}
		
	dlg.m_strOldData = strOldData;

	if(dlg.DoModal() != IDOK) return;
	
	//현재 데이터
	strNewData = dlg.m_strNewData;
	m_pCM->SetControlMgrData(_T("발행자2"),strNewData);
	m_bIsPublisherChanged = TRUE;

	SaveSpeciesDataToHistoryDM(_T("2"),strOldData,strNewData,dlg.m_strReason,dlg.m_strDate);

}


VOID CSeSpeciesForm::OnbtnSPEPUBFREQCHANGE() 
{
	// TODO: Add your control notification handler code here
	// 간행빈도 수정 	
	CSePubfreqChangeDlg dlg(this);
	dlg.m_pDM = m_pDM;

	CString strOldData = _T("");
	CString strNewData = _T("");

	//기존 데이터 
	m_pInfo->pMarcMgr->GetItem(&m_Marc,_T("008@18"),strOldData);
	dlg.m_strOldData = strOldData;

	if(dlg.DoModal() != IDOK) return;
	
	//현재 데이터
	strNewData = dlg.m_strNewData;
	m_pCM->SetControlMgrData(_T("간행빈도"),strNewData);

	SaveSpeciesDataToHistoryDM(_T("4"),strOldData,strNewData,dlg.m_strReason,dlg.m_strDate);
		

	//기존예측정보를 지운다
	DeleteExpData(m_strSpeciesKey,m_nACQ_TYPE);

	//modified by loveisverb
	//2004-12-27
	//예측정보를 지운다.
	if(m_pExpectDlg) delete m_pExpectDlg;
	m_pExpectDlg = NULL;
	m_bIsExpCreated = FALSE;
	m_bIsExpVerified = FALSE;
	m_nPubExpType = -1;  
	m_nAcqExpType = -1; 
	
	//예측관련 DM 을 모두 지운다 
	FreeAllDM();


	AfxMessageBox(_T("간행빈도의 변경으로 인해 \n 기존 예측정보는 삭제 되었습니다!"));

}

VOID CSeSpeciesForm::FreeAllDM()
{
	//입력관련 모든 DM을 찾아서 지운다 

	//loveisverb
	if(m_nOpenMode>0)
	{
		m_pDM->FreeData();
		m_pDM_IDX->FreeData();

		if(1 == m_nACQ_TYPE) m_pPurchaseDlg->m_pDM->FreeData();
		else if(2 == m_nACQ_TYPE) m_pDonateDlg->m_pDM->FreeData();
	}

	CESL_DataMgr* pDM_MONTH = FindDM(_T("DM_종_월간예측"));
	CESL_DataMgr* pDM_WEEK = FindDM(_T("DM_종_주간예측"));
	CESL_DataMgr* pDM_DAY = FindDM(_T("DM_종_일간예측"));
	CESL_DataMgr* pDM_ETC = FindDM(_T("DM_종_기타예측"));
	CESL_DataMgr* pDM_VOL = FindDM(_T("DM_연속_종_예측_권호"));
	CESL_DataMgr* pDM_OTHER_VOL = FindDM(_T("DM_연속_종_예측_부차적권호"));
	CESL_DataMgr* pDM_APPENDIX = FindDM(_T("DM_연속_종_예측_부록"));


	if(!pDM_MONTH || !pDM_WEEK || !pDM_DAY || !pDM_ETC || !pDM_VOL || !pDM_OTHER_VOL || !pDM_APPENDIX)
	{
		AfxMessageBox(_T("예측정보를 가져올 DM을 찾지 못했습니다!")); 	
		return;
	}

	pDM_MONTH->FreeData();
	pDM_WEEK->FreeData();
	pDM_DAY->FreeData();	
	pDM_ETC->FreeData();	
	pDM_VOL->FreeData();
	pDM_OTHER_VOL->FreeData();	
	pDM_APPENDIX->FreeData();
}

HBRUSH CSeSpeciesForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO: Change any attributes of the DC here	

	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	INT nID = pWnd->GetDlgCtrlID();
	if (IsColorControl(nID)) 
	{
		return SetColor(pDC);
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

VOID CSeSpeciesForm::OnbtnCOMMONLIST() 
{
	// TODO: Add your control notification handler code here
	
	// 공동목록 반입 
	CUC_MARC_IMPORT dlg(this);
	if (dlg.DoModal() != IDOK) return;
	
	CMarc tmpmarc;
	if(dlg.sMarc.IsEmpty()) return;

	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	INT ids = m_pInfo->pMarcMgr->Decoding(dlg.sMarc, &tmpmarc);
	
	if (ids < 0) {
		AfxMessageBox(_T("반입에 실패하였습니다!"));
		return;
	}
	
	//반입에 불필요한 마크 태그 값들을 삭제해준다
	const INT cnt = 5;
	CString strfiled[cnt] = {
		_T("001"),	//0
		_T("049"),	//1	
		_T("090"),	//2
		_T("980"),	//3	
		_T("052")	//4	
	};	
	
	for(INT i=0;i<cnt;i++)
	{
		m_pInfo->pMarcMgr->DeleteField(&tmpmarc,strfiled[i]);
	}

	m_Marc.Init();
	m_Marc = tmpmarc;
	SetControlDataByMarc();

}


VOID CSeSpeciesForm::OnKillfocuscmbSPESHELPLOCCODE() 
{
	// TODO: Add your control notification handler code here

	m_bIsLastCtrlFocused = FALSE;

}

VOID CSeSpeciesForm::OnChildLastKillFocus(WPARAM w,LPARAM l)
{
	//child의 마지막 콘트롤이 killfocus되면 첫번째 콘트롤에 setfocus 
	GetDlgItem(IDC_cmbSPE_CONTROL_TYPE)->SetFocus();

}

VOID CSeSpeciesForm::OnbtnSPETITLEDUPINSPECT() 
{
	// TODO: Add your control notification handler code here
	// 입력시 사용되는 서명 복본조사
	CString strTitle;
	m_pCM->GetControlMgrData(_T("서명"),strTitle);
	
	if(strTitle.IsEmpty())
	{
		AfxMessageBox(_T("서명이 입력되지 않았습니다!"));
		return;
	}

	CSeDuplicatedTitleDlg dlg(this);
	dlg.m_strCurrentTitle =  strTitle;
	dlg.DoModal();
}

VOID CSeSpeciesForm::OnSetfocuscmbSPESHELPLOCCODE() 
{
	// TODO: Add your control notification handler code here
	m_bIsLastCtrlFocused = TRUE;	
}

BOOL CSeSpeciesForm::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_TAB)
		{
			if(m_bIsLastCtrlFocused)
			{
				//자료실에서 killfocus 되면 구입 혹은 기증자료의 첫번째 콘트롤에 setfocus한다
				if(1 == m_nACQ_TYPE) m_pPurchaseDlg->SetFocusOnFirstCtrl();
				else if(2 == m_nACQ_TYPE) m_pDonateDlg->SetFocusOnFirstCtrl();

				return TRUE;
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CSeSpeciesForm::ShowUserDefineCode()
{
	CString strYN, strAlias;

	CWnd *pWnd = NULL;

	CESL_DataMgr * TmpDM;
	TmpDM = FindDM(_T("DM_연속_종"));

	// 사용자 정의 코드1 사용 여부.
	strYN.Empty();
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 103"), strYN);
	if(strYN == _T("Y"))
	{
		// 2005.09.14코드명도 바꿔준다.
		TmpDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 103"), strAlias);
		//=====================================================
		//2009.01.29 UPDATE BY PJW : strAlias가 비어있으면 추가하지 않는다.
// 		pWnd = GetDlgItem(IDC_STATIC_SPE13);        pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
// 		pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE1);		pWnd->ShowWindow(SW_SHOW);
		if(!strAlias.IsEmpty())
		{
			pWnd = GetDlgItem(IDC_STATIC_SPE13);        pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE1);		pWnd->ShowWindow(SW_SHOW);
		}
		else
		{
			pWnd = GetDlgItem(IDC_STATIC_SPE13);		pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE1);		pWnd->ShowWindow(SW_HIDE);
		}
		//=====================================================
		
	}
	else
	{
		pWnd = GetDlgItem(IDC_STATIC_SPE13);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE1);		pWnd->ShowWindow(SW_HIDE);
	}

	// 사용자 정의 코드2 사용 여부.
	strYN.Empty();
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 101"), strYN);
	if(strYN == _T("Y"))
	{
		TmpDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 101"), strAlias);
		//=====================================================
		//2009.01.29 UPDATE BY PJW : strAlias가 비어있으면 추가하지 않는다.
// 		pWnd = GetDlgItem(IDC_STATIC_SPE11);		pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
// 		pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE2);		pWnd->ShowWindow(SW_SHOW);
		if(!strAlias.IsEmpty())
		{
			pWnd = GetDlgItem(IDC_STATIC_SPE11);		pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE2);		pWnd->ShowWindow(SW_SHOW);
		}
		else
		{
			pWnd = GetDlgItem(IDC_STATIC_SPE11);		pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE2);		pWnd->ShowWindow(SW_HIDE);
		}
		//=====================================================
		
	}
	else
	{
		pWnd = GetDlgItem(IDC_STATIC_SPE11);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE2);		pWnd->ShowWindow(SW_HIDE);
	}

	// 사용자 정의 코드3 사용 여부.
	strYN.Empty();
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 102"), strYN);
	if(strYN == _T("Y"))
	{
		TmpDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 102"), strAlias);
		//=====================================================
		//2009.01.29 UPDATE BY PJW : strAlias가 비어있으면 추가하지 않는다.
// 		pWnd = GetDlgItem(IDC_STATIC_SPE12);		pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
// 		pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE3);		pWnd->ShowWindow(SW_SHOW);
		if(!strAlias.IsEmpty())
		{
			pWnd = GetDlgItem(IDC_STATIC_SPE12);		pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE3);		pWnd->ShowWindow(SW_SHOW);
		}
		else
		{
			pWnd = GetDlgItem(IDC_STATIC_SPE12);		pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE3);		pWnd->ShowWindow(SW_HIDE);
		}
		//=====================================================
		
	}
	else
	{
		pWnd = GetDlgItem(IDC_STATIC_SPE12);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbSPE_SERIES_CODE3);		pWnd->ShowWindow(SW_HIDE);
	}
}
