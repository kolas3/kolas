// BO_Purchase_Statistics.cpp : implementation file
//

#include "stdafx.h"
#include "BO_Purchase_Statistics.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/DivisionNo_List_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_Purchase_Statistics dialog

CBO_Purchase_Statistics::CBO_Purchase_Statistics(CESL_Mgr* pParent /*=NULL*/)
	: CBasicStatistics(CBO_Purchase_Statistics::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_Purchase_Statistics)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arrDataClass.RemoveAll();
	m_arrDateClass.RemoveAll();
	m_pComboMgr = NULL;
	m_arrHorizontalData.RemoveAll();
	m_arrHorizontalDesc.RemoveAll();
	m_arrVerticalData.RemoveAll();
	m_arrVerticalDesc.RemoveAll();
	m_pParentGroupCollection = NULL;
	m_pThread = NULL;
	m_nOpenType = 0;
	m_pPrintMgr = NULL;
	m_nSpeciesType = SPECIES_TYPE_INVALID;
}

CBO_Purchase_Statistics::~CBO_Purchase_Statistics()
{
	if(m_pStatCondition)
	{
		delete m_pStatCondition;
		m_pStatCondition = NULL;
	}
	if(m_pComboMgr)
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}
	if(m_pPrintMgr)
	{
		delete m_pPrintMgr;
		m_pPrintMgr = NULL;
	}
	ThreadTerminate();
}

VOID CBO_Purchase_Statistics::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_Purchase_Statistics)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_Purchase_Statistics, CDialog)
	//{{AFX_MSG_MAP(CBO_Purchase_Statistics)
	ON_BN_CLICKED(IDC_btnPUR_STAT_VIEW_DIVNO, OnbtnPURSTATVIEWDIVNO)
	ON_EN_KILLFOCUS(IDC_edtPURSTAT_DIVNO, OnKillfocusedtPURSTATDIVNO)
	ON_CBN_SELCHANGE(IDC_cmbPURSTAT_WORK_CODE, OnSelchangecmbPURSTATWORKCODE)
	ON_EN_KILLFOCUS(IDC_edtPURSTAT_ACQ_YEAR, OnKillfocusedtPURSTATACQYEAR)
	ON_CBN_SELCHANGE(IDC_cmbPURSTAT_BASIC_CLASS, OnSelchangecmbPURSTATBASICCLASS)
	ON_BN_CLICKED(IDC_btnPURSTAT_CONTRUCT, OnbtnPURSTATCONTRUCT)
	ON_MESSAGE(JOB_CANCEL_MSG, OnJobCancel)
	ON_MESSAGE(QUERY_CREATE, OnQueryCreate)
	ON_BN_CLICKED(IDC_btnPURSTAT_PRINT, OnbtnPURSTATPRINT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_Purchase_Statistics message handlers
// 초기화
BOOL CBO_Purchase_Statistics::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("K2UPGRADE_수서구입통계")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_수서구입통계"));
	m_pDM = FindDM(_T("DM_통계자료"));
	m_pGrid = (CESL_Grid *)m_pCM->FindControl(_T("MainGrid"));
	if( m_pCM == NULL || m_pGrid == NULL )
	{
		ESLAfxMessageBox(_T("[CM_수서구입통계]를 설정할 수 없습니다."));
		return FALSE;
	}	
	if( m_pDM == NULL )
	{
		ESLAfxMessageBox(_T("[DM_통계자료]를 설정할 수 없습니다."));
		return FALSE;
	}
	m_pGrid->SetSort(FALSE);
	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	
	if( Init() == FALSE )
	{
		ESLAfxMessageBox(_T("검수통계화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	return TRUE;
}

VOID CBO_Purchase_Statistics::ThreadTerminate()
{
	if(!m_pThread) return ;
	if(AfxIsValidAddress(m_pThread, sizeof(CWinThread), 0))
	{
		m_pThread->m_bAutoDelete = FALSE;
		m_bRunning = FALSE;
		WaitForSingleObject(m_pThread->m_hThread, INFINITE);
		m_pThread = NULL;
	}
}

BOOL CBO_Purchase_Statistics::Init()
{
	CString strGroupInfo;

		
	m_arrHorizontalData.Add(_T("전체자료")			);
	m_arrHorizontalData.Add(_T("KDC주제구분")		);
	m_arrHorizontalData.Add(_T("DDC주제구분")		);
	m_arrHorizontalDesc.Add(_T("전체자료")			);
	m_arrHorizontalDesc.Add(_T("KDC대주제부호관리")	);
	m_arrHorizontalDesc.Add(_T("DDC대주제부호관리")	);

	m_arrVerticalData.Add(_T("자료구분별통계")		);
	m_arrVerticalData.Add(_T("등록구분별통계")		);
	m_arrVerticalData.Add(_T("별치구분별통계")		);
	m_arrVerticalDesc.Add(_T("자료구분")			);
	m_arrVerticalDesc.Add(_T("등록구분")			);
	m_arrVerticalDesc.Add(_T("별치기호")			);
		
	if( m_pParentGroupCollection == NULL )		return FALSE;

	m_pGroupCollection = new CGroupCollection(this);
	m_pGroupCollection->SetAcqType(PURCHASE);
	m_pParentGroupCollection->CopyData(m_pGroupCollection);
	
	m_pGroupCollection->GetGroupInfo(strGroupInfo);
	m_pCM->SetControlMgrData(_T("차수번호"), strGroupInfo						);
	m_pCM->SetControlMgrData(_T("업무구분"), m_pGroupCollection->GetWorkCode()	);
	m_pCM->SetControlMgrData(_T("수입년도"), m_pGroupCollection->GetAcqYear()	);
	
	m_pStatCondition = new CStatisticsCondition(this);
	m_pStatCondition->SetAcqType(PURCHASE);
	m_pStatCondition->SetGroupCollection(m_pGroupCollection);
	if( m_nOpenType > 0 )	m_pStatCondition->SetStatType(STATISTICS_BY_DIVNO_DELIVERY);// 검수통계
	else					m_pStatCondition->SetStatType(STATISTICS_BY_DIVNO_MISSING);	// 미납통계
	
	m_pComboMgr = new CMulitComboBoxManager(this);
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("KDC주제구분"), &m_arrHorizontalData, _T("자료기준"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &m_arrVerticalData, _T("통계물목록"));

	m_pPrintMgr = new CBasicPrintMgr(this);
	
	
	return (SetCtrlByOpenType());
}

BOOL CBO_Purchase_Statistics::SetCtrlByOpenType()
{
	if		( m_nOpenType > 0 )		SetWindowText(_T("검수자료통계")	);
	else if	( m_nOpenType < 0 )		SetWindowText(_T("미검수자료통계")	);
	else							return FALSE ;

	return TRUE;
}

VOID CBO_Purchase_Statistics::OnbtnPURSTATVIEWDIVNO() 
{
	CString strAcqYear, strWorkCode, strSelDivNo;

	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear	);
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);

	if( strAcqYear.IsEmpty() == TRUE )
	{
		ESLAfxMessageBox(_T("수입년도 정보를 입력해 주십시요"));
		return;
	}

	CDivisionNo_List_Dlg dlg(this);
	dlg.SetWorkStatus(m_strDivNoWorkStatus);
	dlg.SetAcquisitYear(strAcqYear);
	dlg.SetWorkCode(strWorkCode);
	dlg.SetSpeciesType(m_nSpeciesType);
	dlg.SetGroupCollection(m_pGroupCollection);
	
	if( IDOK == dlg.DoModal() )
	{
		strSelDivNo = dlg.GetSelDivNo();
		m_pCM->SetControlMgrData(_T("차수번호"), strSelDivNo						);
		m_pCM->SetControlMgrData(_T("업무구분"), m_pGroupCollection->GetWorkCode()	);
		m_pCM->SetControlMgrData(_T("수입년도"), m_pGroupCollection->GetAcqYear()	);
	}
}	


VOID CBO_Purchase_Statistics::OnKillfocusedtPURSTATDIVNO() 
{
	CString strAcqYear, strGroupInfo, strWorkCode;

	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("차수번호"), strGroupInfo);
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	
	strGroupInfo = CheckValidateGroupNo(strGroupInfo, strAcqYear, strWorkCode);
	
	m_pCM->SetControlMgrData(_T("차수번호"), strGroupInfo);	
}


VOID CBO_Purchase_Statistics::OnSelchangecmbPURSTATWORKCODE() 
{
	OnKillfocusedtPURSTATDIVNO();
}

VOID CBO_Purchase_Statistics::OnKillfocusedtPURSTATACQYEAR() 
{
	OnKillfocusedtPURSTATDIVNO();
}

BOOL CBO_Purchase_Statistics::PreTranslateMessage(MSG* pMsg) 
{
 	if( pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam )
	{
		GetDlgItem(IDC_btnPURSTAT_CONTRUCT)->SetFocus();
		PostMessage(WM_COMMAND, IDC_btnPURSTAT_CONTRUCT, BN_CLICKED);
		return TRUE;
	}	

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_Purchase_Statistics::OnSelchangecmbPURSTATBASICCLASS() 
{
	INT nCurSel ;
	CESL_ControlComboBox* pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("자료기준"));
	
	nCurSel = pCMB->GetCurSel();
	m_pStatCondition->SetClassNoType(m_arrHorizontalDesc.GetAt(nCurSel));
	
	ChangeColumnHeaderOfSubjectCode();
}

VOID CBO_Purchase_Statistics::ChangeColumnHeaderOfSubjectCode()
{
	CString tmpstr, codeClassName;
	codeClassName = m_pStatCondition->GetClassNoType();
	for(INT i=0 ; i<10 ; i++)
	{
		tmpstr.Format(_T("%d"), i);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(codeClassName, tmpstr, tmpstr);
		if(tmpstr.IsEmpty()) tmpstr.Format(_T("%d"), i);
		m_pGrid->SetTextMatrix(0, i+3, tmpstr);
	}
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 통계구축 버튼
/////////////////////////////////////////////////////////////////////////////
VOID CBO_Purchase_Statistics::OnbtnPURSTATCONTRUCT() 
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT nCurSel;
	CString strAcqYear, strGroupInfo, strWorkCode, strFieldName, strVerticalDesc;
	
	m_nStartRow = 0;
	
	//=======================================================================
	// 1. 수입년도, 차수번호, 업무구분에 해당하는 차수정보를 가져온다.
	//=======================================================================
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("차수번호"), strGroupInfo);
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	strGroupInfo = CheckValidateGroupNo(strGroupInfo, strAcqYear, strWorkCode);
	if( strGroupInfo.IsEmpty() == TRUE )		return ;
	
	//=======================================================================
	// 2. 작업상태, 수입년도, 차수/접수번호, 차수업무구분을 이용하여 
	//	 DM을 구축하여 Group클래스를 생성한다.
	//=======================================================================
	m_pGroupCollection->MakeGroupInfo(strAcqYear, strGroupInfo, strWorkCode);

	CESL_ControlComboBox* pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("자료기준"));
	CESL_ControlComboBox* pCMB1 = (CESL_ControlComboBox*)m_pCM->FindControl(_T("통계물목록"));
	nCurSel = pCMB->GetCurSel();
	m_pStatCondition->SetClassNoType(m_arrHorizontalDesc.GetAt(nCurSel));
	nCurSel = pCMB1->GetCurSel();
	
	strVerticalDesc = m_arrVerticalDesc.GetAt(nCurSel);
	if( strVerticalDesc == _T("등록구분") )		strFieldName = _T("B.REG_CODE");
	if( strVerticalDesc == _T("별치기호") )		strFieldName = _T("B.SEPARATE_SHELF_CODE");
	if( strVerticalDesc == _T("자료구분") )		strFieldName = _T("I.MAT_CODE");
	m_pStatCondition->SetVerticalInfo(strVerticalDesc, strFieldName);
	m_pStatCondition->SetWhereStmt();
	
	MakeStatistics();	
}

INT CBO_Purchase_Statistics::MakeStatistics()
{
	m_pDM->FreeData();
	ShowDataProcessingDlg();
	m_bRunning = TRUE;
	m_pThread = AfxBeginThread(GetStatistics, this);
	return 0;
}

UINT CBO_Purchase_Statistics::GetStatistics(LPVOID pParam )
{
    CBO_Purchase_Statistics* pObject = (CBO_Purchase_Statistics*)pParam;
	pObject->GetAcqStatistics();
	return 0;   // thread completed successfully
}

VOID CBO_Purchase_Statistics::OnJobCancel(WPARAM w,LPARAM l)
{
	ThreadTerminate();
}

VOID CBO_Purchase_Statistics::OnQueryCreate(WPARAM w,LPARAM l)
{
	m_pGrid->Display();
}

VOID CBO_Purchase_Statistics::OnbtnPURSTATPRINT() 
{
	CString strClassNoType = m_pStatCondition->GetClassNoType();

	if( strClassNoType != _T("KDC") && strClassNoType != _T("DDC")) strClassNoType = _T("전체자료");

	m_pPrintMgr->InitPrintData();
	m_pPrintMgr->AddPrintData(_T("제목"), m_pStatCondition->GetStatType());
	m_pPrintMgr->AddPrintData(_T("자료구분"), m_pStatCondition->GetVerticalDesc());
	m_pPrintMgr->AddPrintData(_T("조건"), m_pStatCondition->GetCondition());
	m_pPrintMgr->AddPrintData(_T("분류표구분"), strClassNoType);
	m_pPrintMgr->SetPrintDM(m_pDM);
	
	if		( strClassNoType.Left(3) == _T("KDC") )
	{
		m_pPrintMgr->SetSIReportFilename(_T("K2UP_통계자료_KDC"));
	}
	else if ( strClassNoType.Left(3) == _T("DDC") )
	{
		m_pPrintMgr->SetSIReportFilename(_T("K2UP_통계자료_DDC"));
	}
	else if ( strClassNoType.Left(2) == _T("LC") )
	{
		m_pPrintMgr->SetSIReportFilename(_T("K2UP_통계자료_LC"));
	}
	else	
	{
	m_pPrintMgr->SetSIReportFilename(_T("K2UP_통계자료"));
	}
	
	m_pPrintMgr->Print();	
}
