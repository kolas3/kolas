// StatisticsConditionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StatisticsConditionDlg.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/DivisionNo_List_Dlg.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/RequisitionNo_List_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticsConditionDlg dialog


CStatisticsConditionDlg::CStatisticsConditionDlg(CESL_Mgr* pParent /*=NULL*/)
	: CBasicStatistics(CStatisticsConditionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatisticsConditionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arrDataClass.RemoveAll();
	m_arrDateClass.RemoveAll();
	m_pComboMgr = NULL;
	m_nAcqType = ACQ_TYPE_INVALID;
	m_arrHorizontalData.RemoveAll();
	m_arrHorizontalDesc.RemoveAll();
	m_arrVerticalData.RemoveAll();
	m_arrVerticalDesc.RemoveAll();
	m_pThread = NULL;
	m_bCreate = FALSE;
}

CStatisticsConditionDlg::~CStatisticsConditionDlg()
{
	ThreadTerminate();
}

VOID CStatisticsConditionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsConditionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsConditionDlg, CDialog)
	//{{AFX_MSG_MAP(CStatisticsConditionDlg)
	ON_BN_CLICKED(IDC_btnSTAT_GROUP_VIEW, OnbtnSTATGROUPVIEW)
	ON_CBN_SELCHANGE(IDC_cmbSTAT_DATA_CLASS, OnSelchangecmbSTATDATACLASS)
	ON_EN_KILLFOCUS(IDC_edtSTAT_GROUPINFO, OnKillfocusedtSTATGROUPINFO)
	ON_EN_KILLFOCUS(IDC_edtSTAT_ACQ_YEAR, OnKillfocusedtSTATACQYEAR)
	ON_CBN_SELCHANGE(IDC_cmbSTAT_ACQ_CODE, OnSelchangecmbSTATACQCODE)
	ON_CBN_SELCHANGE(IDC_cmbSTAT_BASIC_CLASS, OnSelchangecmbSTATBASICCLASS)
	ON_MESSAGE(JOB_CANCEL_MSG, OnJobCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticsConditionDlg message handlers

BOOL CStatisticsConditionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_통계조건설정")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_통계조건설정"));
	m_pDM = FindDM(_T("DM_통계자료"));
	
	m_pComboMgr =  new CMulitComboBoxManager(this);

	if(!Init())
	{
		ESLAfxMessageBox(_T("통계 조건설정 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CStatisticsConditionDlg::Init()
{
	m_arrHorizontalData.Add(_T("전체자료"));
	m_arrHorizontalData.Add(_T("KDC주제구분"));
	m_arrHorizontalData.Add(_T("DDC주제구분"));
	m_arrHorizontalDesc.Add(_T("전체자료"));
	m_arrHorizontalDesc.Add(_T("KDC대주제부호관리"));
	m_arrHorizontalDesc.Add(_T("DDC대주제부호관리"));
	m_arrVerticalData.Add(_T("업무구분별통계"));
	m_arrVerticalData.Add(_T("자료구분별통계"));
	m_arrVerticalData.Add(_T("등록구분별통계"));
	m_arrVerticalData.Add(_T("별치구분별통계"));
	m_arrVerticalDesc.Add(_T("업무구분"));
	m_arrVerticalDesc.Add(_T("자료구분"));
	m_arrVerticalDesc.Add(_T("등록구분"));
	m_arrVerticalDesc.Add(_T("별치기호"));
	
 	if(m_nAcqType != ACQ_TYPE_INVALID) GetDlgItem(IDC_cmbSTAT_ACQ_CODE)->EnableWindow(FALSE);
	
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("KDC주제구분"), &m_arrHorizontalData, _T("자료기준"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &m_arrVerticalData, _T("통계물목록"));
	CString strAcqCode;
	strAcqCode.Format(_T("%d"), m_nAcqType);
	m_pCM->SetControlMgrData(_T("수입구분"), strAcqCode);
	m_pGroupCollection = new CGroupCollection(this);
	m_pGroupCollection->SetAcqType(m_nAcqType);
	
	SetDataClassCtrl();
	
	return TRUE;
}

VOID CStatisticsConditionDlg::SetDataClassCtrl()
{
	CString strAcqCode;
	m_pCM->GetControlMgrData(_T("수입구분"), strAcqCode);
	m_nAcqType = (ACQ_TYPE)_ttoi(strAcqCode.GetBuffer(0));
	m_pGroupCollection->RemoveAll();
	m_pGroupCollection->SetAcqType(m_nAcqType);
	
	if(m_nAcqType == ACQ_TYPE_INVALID || m_nAcqType == EXCHANGE)
	{
		m_arrDataClass.Add(_T("기간별통계구축"));
		m_arrDateClass.Add(_T("입력일"));
		m_arrDateClass.Add(_T("등록일"));
	}
	else if(m_nAcqType == PURCHASE)
	{
		m_arrDataClass.Add(_T("기간별통계구축"));
		m_arrDataClass.Add(_T("차수별통계구축"));
		m_arrDateClass.Add(_T("입력일"));
		m_arrDateClass.Add(_T("주문일"));
		m_arrDateClass.Add(_T("검수일"));
		m_arrDateClass.Add(_T("등록일"));
		GetDlgItem(IDC_btnSTAT_GROUP_VIEW)->SetWindowText(_T("차수번호보기"));
		GetDlgItem(IDC_stcSTAT_GROUPINFO)->SetWindowText(_T("차수번호"));
	}
	else if(m_nAcqType == DONATE)
	{
		m_arrDataClass.Add(_T("기간별통계구축"));
		m_arrDataClass.Add(_T("접수번호별통계구축"));
		m_arrDateClass.Add(_T("입력일"));
		m_arrDateClass.Add(_T("등록일"));
		GetDlgItem(IDC_btnSTAT_GROUP_VIEW)->SetWindowText(_T("접수번호보기"));
		GetDlgItem(IDC_stcSTAT_GROUPINFO)->SetWindowText(_T("접수번호"));
		GetDlgItem(IDC_cmbSTAT_WORK_CODE)->EnableWindow(FALSE);
	}
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrDataClass.GetAt(0), &m_arrDataClass, _T("자료구분"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrDateClass.GetAt(0), &m_arrDateClass, _T("자료구분일"));
	SetCtrlByDataClass();
}

VOID CStatisticsConditionDlg::SetCtrlByDataClass()
{
	INT nCount1 = 0;
	INT nArrCodeResourceID1[10];
	nArrCodeResourceID1[nCount1++] = IDC_stcSTAT_DATE_CLASS;
	nArrCodeResourceID1[nCount1++] = IDC_cmbSTAT_DATE_CLASS;
	nArrCodeResourceID1[nCount1++] = IDC_edtSTAT_DATE1;
	nArrCodeResourceID1[nCount1++] = IDC_stcSTAT_DATE_RANGE;
	nArrCodeResourceID1[nCount1++] = IDC_edtSTAT_DATE2;
	INT nCount2 = 0;
	INT nArrCodeResourceID2[10];
	nArrCodeResourceID2[nCount2++] = IDC_edtSTAT_GROUPINFO;
	nArrCodeResourceID2[nCount2++] = IDC_btnSTAT_GROUP_VIEW;
	nArrCodeResourceID2[nCount2++] = IDC_edtSTAT_ACQ_YEAR;
	nArrCodeResourceID2[nCount2++] = IDC_stcSTAT_ACQ_YEAR;
	nArrCodeResourceID2[nCount2++] = IDC_stcSTAT_GROUPINFO;
	nArrCodeResourceID2[nCount2++] = IDC_stcSTAT_WORK_CODE;
	nArrCodeResourceID2[nCount2++] = IDC_cmbSTAT_WORK_CODE;

	for(INT i=0 ; i<nCount1 ; i++)
	{
		GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_HIDE);
	}
	for(i=0 ; i<nCount2 ; i++)
	{
		GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_HIDE);
	}
	CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("자료구분"));
	INT nCurSel = pCMB->GetCurSel();

	if(nCurSel == 0)
	{
		for(i=0 ; i<nCount1 ; i++)
		{
			GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_SHOW);
		}
	}
	else if(nCurSel == 1)
	{
		for(i=0 ; i<nCount2 ; i++)
		{
			GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_SHOW);
		}
	}

}

VOID CStatisticsConditionDlg::OnbtnSTATGROUPVIEW() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear, strSelDivNo, strWorkCode;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	
	if(strAcqYear.IsEmpty()) 
	{
		ESLAfxMessageBox(_T("수입년도 정보를 입력해 주십시요"));
		return;
	}

	if(m_nAcqType == PURCHASE)
	{
		m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
		CDivisionNo_List_Dlg dlg(this);
		dlg.SetWorkStatus(_T("전체자료"));
		dlg.SetAcquisitYear(strAcqYear);
		dlg.SetWorkCode(strWorkCode);
		dlg.SetGroupCollection(m_pGroupCollection);
		if(IDOK == dlg.DoModal())
		{
			strSelDivNo = dlg.GetSelDivNo();
			m_pCM->SetControlMgrData(_T("그룹번호"), strSelDivNo);
			m_pCM->SetControlMgrData(_T("업무구분"), m_pGroupCollection->GetWorkCode());
			m_pCM->SetControlMgrData(_T("수입년도"), m_pGroupCollection->GetAcqYear());
//			OnOK();
		}
	}	
	else if(m_nAcqType == DONATE)
	{
		CRequisitionNo_List_Dlg dlg(this);
		dlg.SetSpeciesType(SPECIES_TYPE_INVALID);
		dlg.SetAcqYear(strAcqYear);
		dlg.SetGroupCollection(m_pGroupCollection);
		if(IDOK == dlg.DoModal())
		{
			strSelDivNo = dlg.GetSelInfo();
			m_pCM->SetControlMgrData(_T("그룹번호"), strSelDivNo);
			m_pCM->SetControlMgrData(_T("수입년도"), m_pGroupCollection->GetAcqYear());
			OnOK();
		}
	}	
}

VOID CStatisticsConditionDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_nStartRow = 0;
	INT ids = CheckValidateInputValue();
	if(ids < 0) return ;
	m_pStatCondition->SetAcqType(m_nAcqType);
	CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("자료기준"));
	CESL_ControlComboBox * pCMB1 = (CESL_ControlComboBox*)m_pCM->FindControl(_T("통계물목록"));
	INT nCurSel = pCMB->GetCurSel();
	m_pStatCondition->SetClassNoType(m_arrHorizontalDesc.GetAt(nCurSel));
	nCurSel = pCMB1->GetCurSel();
	CString strFieldName, strVerticalDesc;
	strVerticalDesc = m_arrVerticalDesc.GetAt(nCurSel);
	if(strVerticalDesc == _T("업무구분")) strFieldName = _T("I.WORK_CODE");
	if(strVerticalDesc == _T("등록구분")) strFieldName = _T("B.REG_CODE");
	if(strVerticalDesc == _T("별치기호")) strFieldName = _T("B.SEPARATE_SHELF_CODE");
	if(strVerticalDesc == _T("자료구분")) strFieldName = _T("I.MAT_CODE");
	m_pStatCondition->SetVerticalInfo(strVerticalDesc, strFieldName);
	m_pStatCondition->SetWhereStmt();
	MakeStatistics();
}

VOID CStatisticsConditionDlg::OnSelchangecmbSTATDATACLASS() 
{
	// TODO: Add your control notification handler code here
	SetCtrlByDataClass();
}

BOOL CStatisticsConditionDlg::PreTranslateMessage(MSG* pMsg) 
{
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		GetDlgItem(IDOK)->SetFocus();
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CStatisticsConditionDlg::OnKillfocusedtSTATGROUPINFO() 
{
	// TODO: Add your control notification handler code here
	if(!GetDlgItem(IDC_edtSTAT_GROUPINFO)->IsWindowVisible()) return ;

	CString strAcqYear, strGroupInfo, strWorkCode;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("그룹번호"), strGroupInfo);
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	strGroupInfo = CheckValidateGroupNo(strGroupInfo, strAcqYear, strWorkCode);
	
	m_pCM->SetControlMgrData(_T("그룹번호"), strGroupInfo);
}

VOID CStatisticsConditionDlg::OnKillfocusedtSTATACQYEAR() 
{
	// TODO: Add your control notification handler code here
	if(!GetDlgItem(IDC_edtSTAT_ACQ_YEAR)->IsWindowVisible()) return ;
	OnKillfocusedtSTATGROUPINFO();
}

VOID CStatisticsConditionDlg::OnSelchangecmbSTATACQCODE() 
{
	// TODO: Add your control notification handler code here
	SetDataClassCtrl();
}

INT CStatisticsConditionDlg::CheckValidateInputValue()
{
	CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("자료구분"));
	INT nCurSel = pCMB->GetCurSel();
	INT ids = -1;
	if(nCurSel == 0) ids = CheckValidateDateInputValue();
	else if(nCurSel == 1) ids = CheckValidateGroupInputValue();
	return ids;
}

INT CStatisticsConditionDlg::CheckValidateDateInputValue()
{
	CString strDate1, strDate2;
	m_pCM->GetControlMgrData(_T("날짜1"), strDate1);
	m_pCM->GetControlMgrData(_T("날짜2"), strDate2);
	if(strDate1.IsEmpty() && strDate2.IsEmpty())
	{
		ESLAfxMessageBox(_T("검색일 조건을 입력해 주십시요"));
		return -1;
	}
	if(strDate1.IsEmpty() && !strDate2.IsEmpty())
	{
		ESLAfxMessageBox(_T("검색 시작일을 입력해 주세요"));
		return -1;
	}
	CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("자료구분일"));
	INT nCurSel = pCMB->GetCurSel();
	CString strDateClass = m_arrDateClass.GetAt(nCurSel);
	
	m_pStatCondition->InitDateInfo();
	m_pStatCondition->SetStatType(STATISTICS_BY_DATE);
	m_pStatCondition->AddDateInfo(_T("날짜1"), strDate1);
	m_pStatCondition->AddDateInfo(_T("날짜2"), strDate2);
	if(strDateClass == _T("입력일"))
	{
		m_pStatCondition->AddDateInfo(_T("DATE_FIELD"), _T("B.INPUT_DATE"));
	}
	else if(strDateClass == _T("등록일"))
	{
		m_pStatCondition->AddDateInfo(_T("DATE_FIELD"), _T("B.REG_DATE"));
	}
	else if(strDateClass == _T("주문일"))
	{
		m_pStatCondition->AddDateInfo(_T("DATE_FIELD"), _T("P.ORDER_DATE"));
	}
	else if(strDateClass == _T("검수일"))
	{
		m_pStatCondition->AddDateInfo(_T("DATE_FIELD"), _T("P.CHECKIN_DATE"));
	}
	return 0;
}

INT CStatisticsConditionDlg::CheckValidateGroupInputValue()
{
	CString strAcqYear, strGroupInfo, strWorkCode;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("그룹번호"), strGroupInfo);
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	
	strGroupInfo = CheckValidateGroupNo(strGroupInfo, strAcqYear, strWorkCode);

	if(strGroupInfo.IsEmpty())
	{
		if(m_nAcqType == PURCHASE)
			ESLAfxMessageBox(_T("통계자료를 구축할 차수정보가 없습니다."));
		else if(m_nAcqType == DONATE)
			ESLAfxMessageBox(_T("통계자료를 구축할 접수번호정보가 없습니다."));
		return -1;
	}

	m_pGroupCollection->MakeGroupInfo(strAcqYear, strGroupInfo, strWorkCode);
	m_pStatCondition->SetStatType(STATISTICS_BY_DIVNO);
	m_pStatCondition->SetGroupCollection(m_pGroupCollection);
	return 0;
}

INT CStatisticsConditionDlg::MakeStatistics()
{
	m_pDM->FreeData();
	ShowDataProcessingDlg();
	m_bRunning = TRUE;
	//m_pThread = AfxBeginThread(GetStatistics, this);
	GetAcqStatistics();
	return 0;
}

UINT CStatisticsConditionDlg::GetStatistics(LPVOID pParam )
{
    CStatisticsConditionDlg* pObject = (CStatisticsConditionDlg*)pParam;
	//pObject->GetStatistics();
	pObject->GetAcqStatistics();
	return 0;   // thread completed successfully
}

VOID CStatisticsConditionDlg::ThreadTerminate()
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

BOOL CStatisticsConditionDlg::Create(CWnd *pWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(IDD, pWnd);
}

VOID CStatisticsConditionDlg::OnSelchangecmbSTATBASICCLASS() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("자료기준"));
	INT nCurSel = pCMB->GetCurSel();
	m_pStatCondition->SetClassNoType(m_arrHorizontalDesc.GetAt(nCurSel));
	
	::SendMessage(pParentMgr->GetSafeHwnd(), CHANGE_CLASS_NO_TYPE, 0, 0);
}

VOID CStatisticsConditionDlg::OnJobCancel(WPARAM w,LPARAM l)
{
	ThreadTerminate();
}