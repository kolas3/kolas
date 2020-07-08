// SeExpectAllDateForm.cpp : implementation file
//

#include "stdafx.h"
#include "SeExpectAllDateForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeExpectAllDateForm dialog


CSeExpectAllDateForm::CSeExpectAllDateForm(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeExpectAllDateForm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeExpectAllDateForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDayType1 = NULL;
	m_pWeekType1 = NULL;
	m_pMonthType1 = NULL;
	m_pExtType1 = NULL;
	m_pDayType2 = NULL;
	m_pWeekType2 = NULL;
	m_pMonthType2 = NULL;
	m_pExtType2 = NULL;
	
	m_bIsFirstCreated = TRUE;
	m_bIsVerified = FALSE;
	
	m_charPUB_FREQ = 'm'; //default 
	m_pCM = NULL;
	m_nOpenMode = 1;
	m_nPubCurSel = 0;
	m_nAcqCurSel = 0;
	
	m_strPUB_EXP_KEY = _T("");
	m_strACQ_EXP_KEY = _T("");
	m_strPUB_EXP_TYPE = _T("");
	m_strACQ_EXP_TYPE = _T("");

	m_bIsParentHaveData = FALSE;

}

CSeExpectAllDateForm::~CSeExpectAllDateForm()
{
	FreeChildForm();
}

VOID CSeExpectAllDateForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeExpectAllDateForm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeExpectAllDateForm, CDialog)
	//{{AFX_MSG_MAP(CSeExpectAllDateForm)
	ON_CBN_SELCHANGE(IDC_cmbEXP_PUB_EXP_TYPE, OnSelchangecmbEXPPUBEXPTYPE)
	ON_CBN_SELCHANGE(IDC_cmbEXP_ACQ_EXP_TYPE, OnSelchangecmbEXPACQEXPTYPE)
	ON_BN_CLICKED(IDC_btnEXP_COPY_DATE, OnbtnEXPCOPYDATE)
	ON_BN_CLICKED(IDC_btnEXP_CLEAR_DATE, OnbtnEXPCLEARDATE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeExpectAllDateForm message handlers
BOOL CSeExpectAllDateForm::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}

BOOL CSeExpectAllDateForm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("연속_수서_예측_일자"))<0){
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_예측_일자") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_연속_예측_일자"));
	if(!m_pCM) {
		AfxMessageBox( _T("CM Error : CM_연속_예측_일자") );
		return false;
	}
	
	CreateChildForm();
	InitComboBox();

	
	if(m_nOpenMode>0)
	{
		//데이터가 검증 되었는지에 따른 데이터 설정 
		if(m_bIsVerified) {

			// 검증된 경우는 TYPE을 확인한 후 DM에서 가지와서  보여준다.	
			SetAllDataByDM();

		} else { 
		
			// 그렇지 않은 경우는 간행 빈도에 따라 콘트롤을 보여준다
			SetChildFormByPubFreq();
		}
		
	}
	else if(-1 == m_nOpenMode) //종화면 수정
	{
	
		if(m_bIsParentHaveData)
		{
	
			SetAllDataByDM();
		}
		else
		{
		
			SetAllData();			
			
		}
		

		
	}
	else if(-2 == m_nOpenMode) //일반 수정
	{
		//수정시에는 기존의 데이터를 보여줌
		SetAllData();


	}
		
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT	 CSeExpectAllDateForm::InitOpenMode()
{

	return 0;
}

VOID CSeExpectAllDateForm::CreateChildForm()
{
	m_pDayType1 = new CSeDayType(this);
	m_pWeekType1 = new CSeWeekType(this);
	m_pMonthType1 = new CSeMonthType(this);
	m_pExtType1 = new CSeExtType(this);
	
	
	m_pDayType2 = new CSeDayType(this);
	m_pWeekType2 = new CSeWeekType(this);
	m_pMonthType2 = new CSeMonthType(this);
	m_pExtType2 = new CSeExtType(this);

 
	m_pMonthType1->m_charPUB_FREQ = m_charPUB_FREQ;
	m_pMonthType2->m_charPUB_FREQ = m_charPUB_FREQ;	


	m_pDayType1->Create(this);
	m_pWeekType1->Create(this);
	m_pMonthType1->Create(this);
	m_pExtType1->Create(this);
	m_pDayType2->Create(this);
	m_pWeekType2->Create(this);
	m_pMonthType2->Create(this);
	m_pExtType2->Create(this);

	CRect rect1,rect2;
	this->GetClientRect(rect1);
	
	rect2 = rect1;

	rect1.top += 80;
	rect1.bottom = 210;
	rect1.left += 120;
	rect1.right -= 80;

	rect2.bottom -= 30;
	rect2.top = rect2.bottom - 130;
	rect2.left += 120;
	rect2.right -= 80;

	
	m_pDayType1->MoveWindow(rect1);
	m_pWeekType1->MoveWindow(rect1);
	m_pMonthType1->MoveWindow(rect1);
	m_pExtType1->MoveWindow(rect1);
	
	m_pDayType2->MoveWindow(rect2);
	m_pWeekType2->MoveWindow(rect2);
	m_pMonthType2->MoveWindow(rect2);
	m_pExtType2->MoveWindow(rect2);

	
	m_pDayType1->ShowWindow(SW_HIDE);
	m_pWeekType1->ShowWindow(SW_HIDE);
	m_pMonthType1->ShowWindow(SW_SHOW);
	m_pExtType1->ShowWindow(SW_HIDE);

	m_pDayType2->ShowWindow(SW_HIDE);
	m_pWeekType2->ShowWindow(SW_HIDE);
	m_pMonthType2->ShowWindow(SW_SHOW);
	m_pExtType2->ShowWindow(SW_HIDE);
}

VOID CSeExpectAllDateForm::FreeChildForm()
{
	if(m_pDayType1) delete m_pDayType1;
	m_pDayType1 = NULL;

	if(m_pWeekType1) delete m_pWeekType1;
	m_pWeekType1 = NULL;

	if(m_pMonthType1) delete m_pMonthType1;
	m_pMonthType1 = NULL;
	
	if(m_pExtType1) delete m_pExtType1;
	m_pExtType1 = NULL;

	if(m_pDayType2) delete m_pDayType2;
	m_pDayType2 = NULL;

	if(m_pWeekType2) delete m_pWeekType2;
	m_pWeekType2 = NULL;

	if(m_pMonthType2) delete m_pMonthType2;
	m_pMonthType2 = NULL;
	
	if(m_pExtType2) delete m_pExtType2;
	m_pExtType2 = NULL;

}

VOID CSeExpectAllDateForm::InitComboBox()
{
	CComboBox *pCombo;

	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbEXP_PUB_EXP_TYPE );
	pCombo->InsertString( 0, _T("월  단위") );
	pCombo->InsertString( 1, _T("주  단위") );
	pCombo->InsertString( 2, _T("일  단위") );
	pCombo->InsertString( 3, _T("기타단위") );

		
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbEXP_ACQ_EXP_TYPE );
	pCombo->InsertString( 0, _T("월  단위") );
	pCombo->InsertString( 1, _T("주  단위") );
	pCombo->InsertString( 2, _T("일  단위") );
	pCombo->InsertString( 3, _T("기타단위") );
		

}

VOID CSeExpectAllDateForm::SetChildFormByPubFreq()
{

	CString strPUB_FREQ;
	strPUB_FREQ.Format(_T("%c"),m_charPUB_FREQ);
	strPUB_FREQ.TrimLeft();
	strPUB_FREQ.TrimRight();
	if(strPUB_FREQ.IsEmpty()) return;
	
	m_pCM->SetControlMgrData(_T("간행빈도"),strPUB_FREQ);


	CComboBox *pCombo1;
	CComboBox *pCombo2;
	pCombo1 = ( CComboBox * )GetDlgItem( IDC_cmbEXP_PUB_EXP_TYPE );
	pCombo2 = ( CComboBox * )GetDlgItem( IDC_cmbEXP_ACQ_EXP_TYPE );
		

	switch( m_charPUB_FREQ )
	{
		case 'a' :
		case 'b' :
		case 'f' :
		case 'g' :
		case 'h' :
		case 'm' :
		case 't' :
		case 'q' :
			pCombo1->SetCurSel( 0 );
			pCombo2->SetCurSel( 0 );
			break;
		case 'c' :
		case 'd' :
		case 'e' :
		case 'i' :
		case 'w' :
			pCombo1->SetCurSel( 1 );
			pCombo2->SetCurSel( 1 );
			break;
		case 'j' :
		case 's' :
			pCombo1->SetCurSel( 2 );
			pCombo2->SetCurSel( 2 );
			break;
		case 'u' :
		case 'z' :
		default :
			pCombo1->SetCurSel( 3 );
			pCombo2->SetCurSel( 3 );
			break;
	}	
	
	OnSelchangecmbEXPPUBEXPTYPE(); 
	OnSelchangecmbEXPACQEXPTYPE();

}

VOID CSeExpectAllDateForm::OnSelchangecmbEXPPUBEXPTYPE() 
{
	// TODO: Add your control notification handler code here
	CComboBox *pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbEXP_PUB_EXP_TYPE );	
	INT nCurSel = pCombo->GetCurSel();

	ShowChildForm(nCurSel,1);

}

VOID CSeExpectAllDateForm::OnSelchangecmbEXPACQEXPTYPE() 
{
	// TODO: Add your control notification handler code here
	CComboBox *pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbEXP_ACQ_EXP_TYPE );	
	INT nCurSel = pCombo->GetCurSel();

	ShowChildForm(nCurSel,2);
}


VOID CSeExpectAllDateForm::ShowChildForm(INT nCurSel,INT nTYPE)
{
	if(1 == nTYPE)
	{
		switch(nCurSel)
		{
		case 0:		
			m_pDayType1->ShowWindow(SW_HIDE);
			m_pWeekType1->ShowWindow(SW_HIDE);
			m_pMonthType1->ShowWindow(SW_SHOW);
			m_pExtType1->ShowWindow(SW_HIDE);
			break;
		case 1:		
			m_pDayType1->ShowWindow(SW_HIDE);
			m_pWeekType1->ShowWindow(SW_SHOW);
			m_pMonthType1->ShowWindow(SW_HIDE);
			m_pExtType1->ShowWindow(SW_HIDE);
			break;
		case 2:		
			m_pDayType1->ShowWindow(SW_SHOW);
			m_pWeekType1->ShowWindow(SW_HIDE);
			m_pMonthType1->ShowWindow(SW_HIDE);
			m_pExtType1->ShowWindow(SW_HIDE);
			break;
		case 3:		
			m_pDayType1->ShowWindow(SW_HIDE);
			m_pWeekType1->ShowWindow(SW_HIDE);
			m_pMonthType1->ShowWindow(SW_HIDE);
			m_pExtType1->ShowWindow(SW_SHOW);
			break;
		}	
	}
	else if(2 == nTYPE)
	{
		switch(nCurSel)
		{
		case 0:		
			m_pDayType2->ShowWindow(SW_HIDE);
			m_pWeekType2->ShowWindow(SW_HIDE);
			m_pMonthType2->ShowWindow(SW_SHOW);
			m_pExtType2->ShowWindow(SW_HIDE);
			break;
		case 1:		
			m_pDayType2->ShowWindow(SW_HIDE);
			m_pWeekType2->ShowWindow(SW_SHOW);
			m_pMonthType2->ShowWindow(SW_HIDE);
			m_pExtType2->ShowWindow(SW_HIDE);
			break;
		case 2:		
			m_pDayType2->ShowWindow(SW_SHOW);
			m_pWeekType2->ShowWindow(SW_HIDE);
			m_pMonthType2->ShowWindow(SW_HIDE);
			m_pExtType2->ShowWindow(SW_HIDE);
			break;
		case 3:		
			m_pDayType2->ShowWindow(SW_HIDE);
			m_pWeekType2->ShowWindow(SW_HIDE);
			m_pMonthType2->ShowWindow(SW_HIDE);
			m_pExtType2->ShowWindow(SW_SHOW);
			break;
		}	
	}
}



INT CSeExpectAllDateForm::SaveAllDataToDM(CString strPUB_EXP_KEY, CString strACQ_EXP_KEY)
{
	//현재의 예측 유형에서 콘트롤 값들을 가져온다. 
	CComboBox *pCombo1;
	CComboBox *pCombo2;
	pCombo1 = ( CComboBox * )GetDlgItem( IDC_cmbEXP_PUB_EXP_TYPE );
	pCombo2 = ( CComboBox * )GetDlgItem( IDC_cmbEXP_ACQ_EXP_TYPE );

	INT nCurSel1 = pCombo1->GetCurSel();
	INT nCurSel2 = pCombo2->GetCurSel();
	
	//현재의 예측 유형을 저장한다.
	m_nPubCurSel = nCurSel1;
	m_nAcqCurSel = nCurSel2;
	
	//모든 DM의 데이터를 지운다
	const INT cnt = 4;
	CString strAlias[cnt] = {_T("DM_월간예측"),_T("DM_주간예측"),_T("DM_일간예측"),_T("DM_기타예측")};
	CESL_DataMgr* pDM;
	for(INT i=0;i<cnt;i++)
	{
		pDM = FindDM(strAlias[i]);
		pDM->FreeData();
	}
	
	//발행일 
	switch(nCurSel1)
	{
		case 0:
			SaveMonthTypeDataToDM(strPUB_EXP_KEY,_T("1"));
			break;
		case 1:
			SaveWeekTypeDataToDM(strPUB_EXP_KEY,_T("1"));
			break;
		case 2:
			SaveDayTypeDataToDM(strPUB_EXP_KEY,_T("1"));
			break;
		case 3:
		default:
			SaveExtTypeDataToDM(strPUB_EXP_KEY,_T("1"));
			break;
	}
	
	//입수일
	switch(nCurSel2)
	{
		case 0:
			SaveMonthTypeDataToDM(strACQ_EXP_KEY,_T("0"));
			break;
		case 1:
			SaveWeekTypeDataToDM(strACQ_EXP_KEY,_T("0"));
			break;
		case 2:
			SaveDayTypeDataToDM(strACQ_EXP_KEY,_T("0"));
			break;
		case 3:
		default:
			SaveExtTypeDataToDM(strACQ_EXP_KEY,_T("0"));
			break;
	}
	

	return 0;
}

VOID CSeExpectAllDateForm::SaveMonthTypeDataToDM(CString strKEY, CString strTYPE)
{
	CESL_DataMgr* pDM = FindDM(_T("DM_월간예측"));
	if(!pDM) return;
	
	pDM->InsertRow(-1);
	INT nRow = pDM->GetRowCount()-1;

	/*	
	_T("REC_KEY"),				
	_T("EXP_CLASS"),			
	_T("EXP_START_DATE"),		
	_T("LAST_ACQ_EXP_COUNT"),	
	_T("FIRST_WORK"),			
	_T("LAST_WORK") };			
	*/
	
	const INT cnt = 24;
	CString alias[ cnt ];
	GetMonthAlias(alias);


	CString strEXP_START_DATE = _T("");
	CString strLAST_EXP_COUNT = _T("");
	CString data[ cnt ];

	if(_T("1") == strTYPE) //발행일 
	{	
		m_pMonthType1->GetAllData(data);
		m_pCM->GetControlMgrData(_T("발행일예측시작일"),strEXP_START_DATE);
		m_pCM->GetControlMgrData(_T("발행예측갯수"),strLAST_EXP_COUNT);
		
	}
	else if(_T("0") == strTYPE) //입수일
	{
		m_pMonthType2->GetAllData(data);
		m_pCM->GetControlMgrData(_T("입수일예측시작일"),strEXP_START_DATE);
		m_pCM->GetControlMgrData(_T("입수예측갯수"),strLAST_EXP_COUNT);
	}
	
	pDM->SetCellData(alias,cnt,data,nRow); 
	pDM->SetCellData(_T("REC_KEY"),strKEY,nRow);
	pDM->SetCellData(_T("EXP_CLASS"),strTYPE,nRow);
	pDM->SetCellData(_T("EXP_START_DATE"),strEXP_START_DATE,nRow);
	pDM->SetCellData(_T("LAST_WORK"),GetWorkLogMsg(WORK_LOG_EXP1),nRow);
	pDM->SetCellData(_T("LAST_ACQ_EXP_COUNT"),strLAST_EXP_COUNT,nRow);
}

VOID CSeExpectAllDateForm::SaveWeekTypeDataToDM(CString strKEY, CString strTYPE)
{
	CESL_DataMgr* pDM = FindDM(_T("DM_주간예측"));
	if(!pDM) return;
	
	pDM->InsertRow(-1);
	INT nRow = pDM->GetRowCount()-1;

	/*
	_T("REC_KEY"),				
	_T("EXP_CLASS"),			
	_T("EXP_START_DATE"),		
	_T("LAST_ACQ_EXP_COUNT"),	
	_T("LAST_WORK") };			
	*/
		
	const INT cnt = 7;
	CString alias[ cnt ];
	GetWeekAlias(alias);
	

	CString strEXP_START_DATE = _T("");
	CString strLAST_EXP_COUNT = _T("");
	CString data[ cnt ];


	if(_T("1") == strTYPE) //발행일 
	{	
		m_pWeekType1->GetAllData(data);
		m_pCM->GetControlMgrData(_T("발행일예측시작일"),strEXP_START_DATE);
		m_pCM->GetControlMgrData(_T("발행예측갯수"),strLAST_EXP_COUNT);
	}
	else if(_T("0") == strTYPE) //입수일
	{
		m_pWeekType2->GetAllData(data);
		m_pCM->GetControlMgrData(_T("입수일예측시작일"),strEXP_START_DATE);
		m_pCM->GetControlMgrData(_T("입수예측갯수"),strLAST_EXP_COUNT);
	}
	
	pDM->SetCellData(alias,cnt,data,nRow); 
	pDM->SetCellData(_T("REC_KEY"),strKEY,nRow);
	pDM->SetCellData(_T("EXP_CLASS"),strTYPE,nRow);
	pDM->SetCellData(_T("EXP_START_DATE"),strEXP_START_DATE,nRow);
	pDM->SetCellData(_T("LAST_WORK"),GetWorkLogMsg(WORK_LOG_EXP1),nRow);
	pDM->SetCellData(_T("LAST_ACQ_EXP_COUNT"),strLAST_EXP_COUNT,nRow);

}

VOID CSeExpectAllDateForm::SaveDayTypeDataToDM(CString strKEY, CString strTYPE)
{
	CESL_DataMgr* pDM = FindDM(_T("DM_일간예측"));
	if(!pDM) return;
	
	pDM->InsertRow(-1);
	INT nRow = pDM->GetRowCount()-1;

	/*
		_T("REC_KEY"),				
		_T("EXP_CLASS"),			
		_T("EXP_START_DATE"),		
		_T("LAST_ACQ_EXP_COUNT"),	
		_T("LAST_WORK") };			
	*/

	const INT cnt = 12;
	CString alias[ cnt ];
	GetDayAlias(alias);

	CString strEXP_START_DATE = _T("");
	CString strLAST_EXP_COUNT = _T("");
	CString data[ cnt ];

	
	if(_T("1") == strTYPE) //발행일 
	{
		m_pDayType1->GetAllData(data);
		m_pCM->GetControlMgrData(_T("발행일예측시작일"),strEXP_START_DATE);
		m_pCM->GetControlMgrData(_T("발행예측갯수"),strLAST_EXP_COUNT);
	} 
	else if(_T("0") == strTYPE) //입수일
	{
		m_pDayType2->GetAllData(data);
		m_pCM->GetControlMgrData(_T("입수일예측시작일"),strEXP_START_DATE);
		m_pCM->GetControlMgrData(_T("입수예측갯수"),strLAST_EXP_COUNT);
	}
	
	pDM->SetCellData(alias,cnt,data,nRow); 
	pDM->SetCellData(_T("REC_KEY"),strKEY,nRow);
	pDM->SetCellData(_T("EXP_CLASS"),strTYPE,nRow);
	pDM->SetCellData(_T("EXP_START_DATE"),strEXP_START_DATE,nRow);
	pDM->SetCellData(_T("LAST_WORK"),GetWorkLogMsg(WORK_LOG_EXP1),nRow);
	pDM->SetCellData(_T("LAST_ACQ_EXP_COUNT"),strLAST_EXP_COUNT,nRow);
}

VOID CSeExpectAllDateForm::SaveExtTypeDataToDM(CString strKEY, CString strTYPE)
{
	CESL_DataMgr* pDM = FindDM(_T("DM_기타예측"));
	if(!pDM) return;
	
	pDM->InsertRow(-1);
	INT nRow = pDM->GetRowCount()-1;

	/*
		_T("REC_KEY"),				
		_T("EXP_CLASS"),			
		_T("EXP_START_DATE"),		
		_T("LAST_ACQ_EXP_COUNT"),		
		_T("LAST_WORK")
	*/

	const INT cnt = 4;
	CString alias[ cnt ];
	GetEtcAlias(alias);
	
	CString strEXP_START_DATE = _T("");
	CString strLAST_EXP_COUNT = _T("");
	CString data[ cnt ];

	if(_T("1") == strTYPE) //발행일 
	{
		m_pExtType1->GetAllData(data);
		m_pCM->GetControlMgrData(_T("발행일예측시작일"),strEXP_START_DATE);
		m_pCM->GetControlMgrData(_T("발행예측갯수"),strLAST_EXP_COUNT);
	} 
	else if(_T("0") == strTYPE) //입수일
	{
		m_pExtType2->GetAllData(data);
		m_pCM->GetControlMgrData(_T("입수일예측시작일"),strEXP_START_DATE);
		m_pCM->GetControlMgrData(_T("입수예측갯수"),strLAST_EXP_COUNT);
	}
	
	pDM->SetCellData(alias,cnt,data,nRow); 
	pDM->SetCellData(_T("REC_KEY"),strKEY,nRow);
	pDM->SetCellData(_T("EXP_CLASS"),strTYPE,nRow);
	pDM->SetCellData(_T("EXP_START_DATE"),strEXP_START_DATE,nRow);
	pDM->SetCellData(_T("LAST_WORK"),GetWorkLogMsg(WORK_LOG_EXP1),nRow);
	pDM->SetCellData(_T("LAST_ACQ_EXP_COUNT"),strLAST_EXP_COUNT,nRow);
}

INT CSeExpectAllDateForm::SaveAllData(CESL_DataMgr* pDM_OUT,INT nPubCurSel,INT nAcqCurSel)
{
	//DB에 보낼 쿼리를 구성해 주는 함수 
	//수정 모드에서는 하위 다이얼 로그에서 기존 데이터를 삭제한 후 INSERT한다
	
	
	//test
	/*
	//현재의 예측 유형에서 콘트롤 값들을 가져온다. 
	CComboBox *pCombo1;
	CComboBox *pCombo2;
	pCombo1 = ( CComboBox * )GetDlgItem( IDC_cmbEXP_PUB_EXP_TYPE );
	pCombo2 = ( CComboBox * )GetDlgItem( IDC_cmbEXP_ACQ_EXP_TYPE );

	INT nCurSel1 = pCombo1->GetCurSel();
	INT nCurSel2 = pCombo2->GetCurSel();
	
	//현재의 예측 유형을 저장한다.
	m_nPubCurSel = nCurSel1;
	m_nAcqCurSel = nCurSel2;
	*/
	
	//test
	if(nPubCurSel>=0)
	{
		m_nPubCurSel = nPubCurSel;
	}

	if(nAcqCurSel>=0)
	{
		m_nAcqCurSel = nAcqCurSel;
	}
	
	CString strTBL_NAME = _T("");
	CESL_DataMgr* pDM = NULL;
	INT nOption = 0;

	if(m_nPubCurSel == m_nAcqCurSel) nOption = 1; //발행일 예측과 입수일 예측이 같은 경우 한DM에 2ROW INSERT

	switch(m_nPubCurSel)
	{
	case 0: 
		strTBL_NAME = _T("SE_MONTH_EXP_TBL");
		pDM = FindDM(_T("DM_월간예측"));
		break;
	case 1: 
		strTBL_NAME = _T("SE_WEEK_EXP_TBL");
		pDM = FindDM(_T("DM_주간예측"));
		break;
	case 2: 
		strTBL_NAME = _T("SE_DAY_EXP_TBL");
		pDM = FindDM(_T("DM_일간예측"));
		break;
	case 3: 
	default:
		strTBL_NAME = _T("SE_ETC_EXP_TBL");
		pDM = FindDM(_T("DM_기타예측"));
		break;		
	}


	CArray<CString,CString> RemoveAliasList;

	for(INT i=0;i<pDM->GetRowCount();i++)
	{
		RemoveAliasList.RemoveAll();
		pDM_OUT->InitDBFieldData();
		pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM , strTBL_NAME, i , pDM_OUT);
	}
	
	//발행일과 입수일 예측 유형이 틀리면 추가로 입수일을 INSERT한다.
	if(0 == nOption)
	{
		switch(m_nAcqCurSel)
		{
		case 0: 
			strTBL_NAME = _T("SE_MONTH_EXP_TBL");
			pDM = FindDM(_T("DM_월간예측"));
			break;
		case 1: 
			strTBL_NAME = _T("SE_WEEK_EXP_TBL");
			pDM = FindDM(_T("DM_주간예측"));
			break;
		case 2: 
			strTBL_NAME = _T("SE_DAY_EXP_TBL");
			pDM = FindDM(_T("DM_일간예측"));
			break;
		case 3: 
		default:
			strTBL_NAME = _T("SE_ETC_EXP_TBL");
			pDM = FindDM(_T("DM_기타예측"));
			break;		
		}

		RemoveAliasList.RemoveAll();
		pDM_OUT->MakeQueryByDM(RemoveAliasList, pDM , strTBL_NAME, 0 , pDM_OUT);
	}
	
	return 0;
}

INT CSeExpectAllDateForm::DeleteOldData(CESL_DataMgr* pDM_OUT,CString strREC_KEY, CString strTYPE, CString strEXP_CLASS)
{
	//기존 정보 삭제를 위한 퀴리를 생성해 준다.
	if(strREC_KEY.IsEmpty())
	{
		AfxMessageBox(_T("일자 예측정보 삭제를 위한 REC_KEY를 찾지 못했습니다!"));
		return -1;
	}
	
	CString strTBL_NAME;

	switch(strTYPE.GetAt(0))
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
	}
		
	CString strQuery;
	//strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s AND EXP_CLASS = '%s';"),strTBL_NAME,strREC_KEY,strEXP_CLASS);
	strQuery.Format(_T("DELETE FROM %s WHERE REC_KEY = %s ;"),strTBL_NAME,strREC_KEY);
	pDM_OUT->AddFrame(strQuery);

	return 0;
}

VOID CSeExpectAllDateForm::SetAllDataByDM()
{
	//종화면에서 copy된 DM을 통해 기존 데이터를 보여준다 
	CString strPUB_FREQ;
	strPUB_FREQ.Format(_T("%c"),m_charPUB_FREQ);
	strPUB_FREQ.TrimLeft();
	strPUB_FREQ.TrimRight();
	if(strPUB_FREQ.IsEmpty()) return;
	
	m_pCM->SetControlMgrData(_T("간행빈도"),strPUB_FREQ);

	CComboBox *pCombo1;
	CComboBox *pCombo2;
	pCombo1 = ( CComboBox * )GetDlgItem( IDC_cmbEXP_PUB_EXP_TYPE );
	pCombo2 = ( CComboBox * )GetDlgItem( IDC_cmbEXP_ACQ_EXP_TYPE );
	
	CESL_DataMgr* pDM = NULL;
	
	const INT cnt = 4;
	CString strDM[cnt] = {_T("DM_월간예측"),_T("DM_주간예측"),_T("DM_일간예측"),_T("DM_기타예측")};
	CString strEXP_CLASS = _T("");

	for(INT i=0;i<cnt;i++)
	{
		pDM = FindDM(strDM[i]);
		if(!pDM) continue;
	
		for(INT j=0;j<pDM->GetRowCount();j++)
		{
			strEXP_CLASS = 	pDM->GetCellData(_T("EXP_CLASS"),j);
				
			if(_T("1") == strEXP_CLASS)
			{
				switch(i)
				{
				case 0: 
						SetMonthData(pDM,1,j);			
						pCombo1->SetCurSel(0);
						break;
				case 1: 
						SetWeekData(pDM,1,j); 
						pCombo1->SetCurSel(1);
						break;
				case 2: 
						SetDayData(pDM,1,j); 
						pCombo1->SetCurSel(2);
						break;
				case 3: 
						SetEtcData(pDM,1,j);
						pCombo1->SetCurSel(3);
						break;	
				}
			}
			else if(_T("0") == strEXP_CLASS)
			{

				switch(i)
				{
				case 0: 
						SetMonthData(pDM,0,j);			
						pCombo2->SetCurSel(0);
						break;
				case 1: 
						SetWeekData(pDM,0,j); 
						pCombo2->SetCurSel(1);
						break;
				case 2: 
						SetDayData(pDM,0,j); 
						pCombo2->SetCurSel(2);
						break;
				case 3: 
						SetEtcData(pDM,0,j);
						pCombo2->SetCurSel(3);
						break;	
				}
		
			}
		}
	}

	
	OnSelchangecmbEXPPUBEXPTYPE(); 
	OnSelchangecmbEXPACQEXPTYPE();
}

VOID CSeExpectAllDateForm::SetAllData()
{
	//종화면 간행빈도를 보여준다 
	CString strPUB_FREQ;
	strPUB_FREQ.Format(_T("%c"),m_charPUB_FREQ);
	strPUB_FREQ.TrimLeft();
	strPUB_FREQ.TrimRight();
	if(strPUB_FREQ.IsEmpty()) return;
	
	m_pCM->SetControlMgrData(_T("간행빈도"),strPUB_FREQ);


	
	//DB의 기존데이터를 보여준다
	CComboBox *pCombo1;
	CComboBox *pCombo2;
	pCombo1 = ( CComboBox * )GetDlgItem( IDC_cmbEXP_PUB_EXP_TYPE );
	pCombo2 = ( CComboBox * )GetDlgItem( IDC_cmbEXP_ACQ_EXP_TYPE );
	
	CESL_DataMgr* pDM = NULL;
	CString strWhere = _T("");

	//입수일 
	if(!m_strACQ_EXP_TYPE.IsEmpty())
	{
		
		strWhere.Format(_T("REC_KEY = %s"),m_strACQ_EXP_KEY);

		switch(m_strACQ_EXP_TYPE.GetAt(0))
		{
		case 'M': 
			pDM = FindDM(_T("DM_월간예측"));
			pDM->RefreshDataManager(strWhere);
			if(pDM->GetRowCount()) SetMonthData(pDM,0);			
			pCombo2->SetCurSel(0);
			break;
		case 'W': 
			pDM = FindDM(_T("DM_주간예측"));
			pDM->RefreshDataManager(strWhere);
			if(pDM->GetRowCount()) SetWeekData(pDM,0); 
			pCombo2->SetCurSel(1);
			break;
		case 'D': 
			pDM = FindDM(_T("DM_일간예측"));
			pDM->RefreshDataManager(strWhere);
			if(pDM->GetRowCount()) SetDayData(pDM,0); 
			pCombo2->SetCurSel(2);
			break;
		case 'E': 
			pDM = FindDM(_T("DM_기타예측"));
			pDM->RefreshDataManager(strWhere);
			if(pDM->GetRowCount()) SetEtcData(pDM,0);
			pCombo2->SetCurSel(3);
			break;		
		default:
			goto DEFAULT_TYPE_SET;
		}
	
	}

	//발행일 
	if(!m_strPUB_EXP_TYPE.IsEmpty())
	{
		
		strWhere.Format(_T("REC_KEY = %s"),m_strPUB_EXP_KEY);

		switch(m_strPUB_EXP_TYPE.GetAt(0))
		{
		case 'M': 
			pDM = FindDM(_T("DM_월간예측"));
			pDM->RefreshDataManager(strWhere);
			if(pDM->GetRowCount()) SetMonthData(pDM,1);
			pCombo1->SetCurSel(0);
			break;
		case 'W': 
			pDM = FindDM(_T("DM_주간예측"));
			pDM->RefreshDataManager(strWhere);
			if(pDM->GetRowCount()) SetWeekData(pDM,1); 
			pCombo1->SetCurSel(1);
			break;
		case 'D': 
			pDM = FindDM(_T("DM_일간예측"));
			pDM->RefreshDataManager(strWhere);
			if(pDM->GetRowCount()) SetDayData(pDM,1); 
			pCombo1->SetCurSel(2);
			break;
		case 'E': 
			pDM = FindDM(_T("DM_기타예측"));
			pDM->RefreshDataManager(strWhere);
			if(pDM->GetRowCount()) SetEtcData(pDM,1);
			pCombo1->SetCurSel(3);
			break;	
		default:
			goto DEFAULT_TYPE_SET;
		}
	
	}

	//test
	if(m_strACQ_EXP_TYPE.IsEmpty() && m_strPUB_EXP_TYPE.IsEmpty())
	{
		SetChildFormByPubFreq();
	}


	OnSelchangecmbEXPPUBEXPTYPE(); 
	OnSelchangecmbEXPACQEXPTYPE();
	return;

	
DEFAULT_TYPE_SET:
	SetChildFormByPubFreq();
	return;

}

VOID CSeExpectAllDateForm::SetMonthData(CESL_DataMgr* pDM, INT nType ,INT nRow)
{

	const INT cnt = 24;
	CString alias[ cnt ];
	CString data[ cnt ];
	GetMonthAlias(alias);
	
	pDM->GetCellData(alias,24,data,nRow);
	
	CString strEXP_START_DATE = _T("");
	strEXP_START_DATE = pDM->GetCellData(_T("EXP_START_DATE"),nRow);
	
	CString strLAST_EXP_COUNT = _T("");
	strLAST_EXP_COUNT = pDM->GetCellData(_T("LAST_ACQ_EXP_COUNT"),nRow);

	if(1 == nType) //발행일
	{
		m_pMonthType1->SetAllData(data);
		m_pCM->SetControlMgrData(_T("발행일예측시작일"),strEXP_START_DATE);
		m_pCM->SetControlMgrData(_T("발행예측갯수"),strLAST_EXP_COUNT);
	}
	else //입수일 
	{	
		m_pMonthType2->SetAllData(data);
		m_pCM->SetControlMgrData(_T("입수일예측시작일"),strEXP_START_DATE);
		m_pCM->SetControlMgrData(_T("입수예측갯수"),strLAST_EXP_COUNT);
	}
}

VOID CSeExpectAllDateForm::SetWeekData(CESL_DataMgr* pDM, INT nType,INT nRow)
{
	const INT cnt = 7;
	CString alias[ cnt ];
	CString data[ cnt ];
	GetWeekAlias(alias);
	
	pDM->GetCellData(alias,7,data,0);
	
	CString strEXP_START_DATE = _T("");
	strEXP_START_DATE = pDM->GetCellData(_T("EXP_START_DATE"),nRow);

	CString strLAST_EXP_COUNT = _T("");
	strLAST_EXP_COUNT = pDM->GetCellData(_T("LAST_ACQ_EXP_COUNT"),nRow);

	if(1 == nType) //발행일
	{
		m_pWeekType1->SetAllData(data);
		m_pCM->SetControlMgrData(_T("발행일예측시작일"),strEXP_START_DATE);
		m_pCM->SetControlMgrData(_T("발행예측갯수"),strLAST_EXP_COUNT);
	}
	else //입수일 
	{
		m_pWeekType2->SetAllData(data);
		m_pCM->SetControlMgrData(_T("입수일예측시작일"),strEXP_START_DATE);
		m_pCM->SetControlMgrData(_T("입수예측갯수"),strLAST_EXP_COUNT);
	}
}

VOID CSeExpectAllDateForm::SetDayData(CESL_DataMgr* pDM, INT nType,INT nRow)
{
	const INT cnt = 12;
	CString alias[ cnt ];
	CString data[ cnt ];
	GetDayAlias(alias);
	
	pDM->GetCellData(alias,12,data,nRow);
	
	CString strEXP_START_DATE = _T("");
	strEXP_START_DATE = pDM->GetCellData(_T("EXP_START_DATE"),nRow);

	CString strLAST_EXP_COUNT = _T("");
	strLAST_EXP_COUNT = pDM->GetCellData(_T("LAST_ACQ_EXP_COUNT"),nRow);

	if(1 == nType) //발행일
	{
		m_pDayType1->SetAllData(data);
		m_pCM->SetControlMgrData(_T("발행일예측시작일"),strEXP_START_DATE);
		m_pCM->SetControlMgrData(_T("발행예측갯수"),strLAST_EXP_COUNT);
	}
	else //입수일 
	{
		m_pDayType2->SetAllData(data);
		m_pCM->SetControlMgrData(_T("입수일예측시작일"),strEXP_START_DATE);
		m_pCM->SetControlMgrData(_T("입수예측갯수"),strLAST_EXP_COUNT);
	}
}

VOID CSeExpectAllDateForm::SetEtcData(CESL_DataMgr* pDM, INT nType, INT nRow)
{
	const INT cnt = 4;
	CString alias[ cnt ];
	CString data[ cnt ];
	GetEtcAlias(alias);
	
	pDM->GetCellData(alias,cnt,data,nRow);

	CString strEXP_START_DATE = _T("");
	strEXP_START_DATE = pDM->GetCellData(_T("EXP_START_DATE"),nRow);

	CString strLAST_EXP_COUNT = _T("");
	strLAST_EXP_COUNT = pDM->GetCellData(_T("LAST_ACQ_EXP_COUNT"),nRow);

	if(1 == nType) //발행일
	{
		m_pExtType1->SetAllData(data);
		m_pCM->SetControlMgrData(_T("발행일예측시작일"),strEXP_START_DATE);
		m_pCM->SetControlMgrData(_T("발행예측갯수"),strLAST_EXP_COUNT);
	}
	else //입수일 
	{
		m_pExtType2->SetAllData(data);
		m_pCM->SetControlMgrData(_T("입수일예측시작일"),strEXP_START_DATE);
		m_pCM->SetControlMgrData(_T("입수예측갯수"),strLAST_EXP_COUNT);
	}
}

VOID CSeExpectAllDateForm::GetMonthAlias(CString data[])
{
	
	data[0] = _T("EXP_TERM_DAY1");		//	0
	data[1] = _T("EXP_TERM_DAY2");		//  1
	data[2] = _T("EXP_TERM_DAY3");		//  2
	data[3] = _T("EXP_TERM_DAY4");		//	3	  
	data[4] = _T("EXP_TERM_DAY5");		//	4
	data[5] = _T("EXP_TERM_DAY6");		//	5
	data[6] = _T("EXP_TERM_DAY7");		//	6
	data[7] = _T("EXP_TERM_DAY8");		//	7
	data[8] = _T("EXP_TERM_DAY9");		//	8
	data[9] = _T("EXP_TERM_DAY10");		//	9
	data[10] = _T("EXP_TERM_DAY11");	//	10
	data[11] = _T("EXP_TERM_DAY12");	//	11
	data[12] = _T("EXP_BOOK_CNT1");		//	12
	data[13] = _T("EXP_BOOK_CNT2");		//	13
	data[14] = _T("EXP_BOOK_CNT3");		//	14
	data[15] = _T("EXP_BOOK_CNT4");		//	15
	data[16] = _T("EXP_BOOK_CNT5");		//	16
	data[17] = _T("EXP_BOOK_CNT6");		//	17
	data[18] = _T("EXP_BOOK_CNT7");		//	18
	data[19] = _T("EXP_BOOK_CNT8");		//	19
	data[20] = _T("EXP_BOOK_CNT9");		//	20
	data[21] = _T("EXP_BOOK_CNT10");	//	21
	data[22] = _T("EXP_BOOK_CNT11");	//	22
	data[23] = _T("EXP_BOOK_CNT12");	//	23

}

VOID CSeExpectAllDateForm::GetWeekAlias(CString data[])
{
	data[0] = _T("MON_CNT");				//	0
	data[1]	= _T("TUE_CNT");				//	1
	data[2]	= _T("WED_CNT");				//	2
	data[3]	= _T("THR_CNT");				//	3
	data[4]	= _T("FRI_CNT");				//	4
	data[5]	= _T("SAT_CNT");				//	5
	data[6]	= _T("SUN_CNT");				//  6
	
}

VOID CSeExpectAllDateForm::GetDayAlias(CString data[])
{
	data[0] = _T("EXP_TERM_DATE1");		//	0
	data[1] = _T("EXP_TERM_DATE2");		//	1
	data[2] = _T("EXP_TERM_DATE3");		//	2
	data[3] = _T("EXP_TERM_DATE4");		//	3
	data[4] = _T("EXP_TERM_DATE5");		//	4
	data[5] = _T("EXP_TERM_DATE6");		//	5
	data[6] = _T("EXP_BOOK_CNT1");		//	6
	data[7] = _T("EXP_BOOK_CNT2");		//	7
	data[8] = _T("EXP_BOOK_CNT3");		//	8
	data[9] = _T("EXP_BOOK_CNT4");		//	9
	data[10] = _T("EXP_BOOK_CNT5");		//	10
	data[11] = _T("EXP_BOOK_CNT6");		//	11

}

VOID CSeExpectAllDateForm::GetEtcAlias(CString data[])
{
	data[0] = _T("EXP_TERM_YEAR");		//	0
	data[1] = _T("EXP_TERM_MONTH");		//	1
	data[2] = _T("EXP_TERM_DAY");		//	2
	data[3] = _T("EXP_BOOK_CNT");		//	3
}

VOID CSeExpectAllDateForm::OnbtnEXPCOPYDATE() 
{
	// TODO: Add your control notification handler code here
	//모든 발행일예측정보를 입수일 예측 정보에 복사한다.
	

	//유형
	CComboBox *pCombo1;
	CComboBox *pCombo2;
	pCombo1 = ( CComboBox * )GetDlgItem( IDC_cmbEXP_PUB_EXP_TYPE );
	pCombo2 = ( CComboBox * )GetDlgItem( IDC_cmbEXP_ACQ_EXP_TYPE );

	INT nCurSel = pCombo1->GetCurSel();
	pCombo2->SetCurSel(nCurSel);
	OnSelchangecmbEXPACQEXPTYPE();
	

	//예측시작일
	CString strEXP_START_DATE = _T("");
	m_pCM->GetControlMgrData(_T("발행일예측시작일"),strEXP_START_DATE);
	m_pCM->SetControlMgrData(_T("입수일예측시작일"),strEXP_START_DATE);
	

	//예측된 갯수 
	CString strLAST_EXP_COUNT = _T("");
	m_pCM->GetControlMgrData(_T("발행예측갯수"),strLAST_EXP_COUNT);
	m_pCM->SetControlMgrData(_T("입수예측갯수"),strLAST_EXP_COUNT);
	

	//max size
	CString data[24];

	//data
	switch(nCurSel)
	{
	case 0:
		m_pMonthType1->GetAllData(data);
		m_pMonthType2->SetAllData(data);
		break;
	case 1:
		m_pWeekType1->GetAllData(data);
		m_pWeekType2->SetAllData(data);
		break;
	case 2:
		m_pDayType1->GetAllData(data);
		m_pDayType2->SetAllData(data);
		break;
	case 3:
		m_pExtType1->GetAllData(data);
		m_pExtType2->SetAllData(data);
		break;
	}

	
}

VOID CSeExpectAllDateForm::OnbtnEXPCLEARDATE() 
{
	// TODO: Add your control notification handler code here
	
	// 화면 지우기
	
	//max size
	CString data[24];
	
	m_pCM->SetControlMgrData(_T("발행일예측시작일"),_T(""));
	m_pCM->SetControlMgrData(_T("입수일예측시작일"),_T(""));
	
	m_pCM->SetControlMgrData(_T("입수예측갯수"),_T("0"));
	m_pCM->SetControlMgrData(_T("발행예측갯수"),_T("0"));

	m_pMonthType1->ClearAllData();
	m_pMonthType2->ClearAllData();
	m_pWeekType1->ClearAllData();
	m_pWeekType2->ClearAllData();
	m_pDayType1->ClearAllData();
	m_pDayType2->ClearAllData();
	m_pExtType1->ClearAllData();
	m_pExtType2->ClearAllData();

}


BOOL CSeExpectAllDateForm::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_ESCAPE )
		{
			return TRUE;
		}
		//2018년 공공도서관 책이음서비스 확대구축 및 시스템개선
		//KOL.RED.019-010 JY 연속간행물 기능 안정화
		if( pMsg->wParam == VK_RETURN )
		{
			return FALSE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CSeExpectAllDateForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
