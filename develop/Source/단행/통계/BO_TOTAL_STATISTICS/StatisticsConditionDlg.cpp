// StatisticsConditionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StatisticsConditionDlg.h"
#include "StatisticsCondition.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/DivisionNo_List_Dlg.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/RequisitionNo_List_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticsConditionDlg dialog

// 생성자
CStatisticsConditionDlg::CStatisticsConditionDlg(CESL_Mgr* pParent /*=NULL*/)
	: CBasicStatistics(CStatisticsConditionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatisticsConditionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strIs90 = _T("");
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
	m_strUserCodeAlias.RemoveAll();

	
}

// 소멸자
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
	ON_WM_CTLCOLOR() 
	ON_MESSAGE(JOB_CANCEL_MSG, OnJobCancel)
	ON_EN_KILLFOCUS(IDC_edtEND_REG_NO, OnKillfocusedtENDREGNO)
	ON_EN_KILLFOCUS(IDC_edtSTART_REG_NO, OnKillfocusedtSTARTREGNO)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticsConditionDlg message handlers
// 초기화
BOOL CStatisticsConditionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// SM을 설정
	if( InitESL_Mgr(_T("K2UPGRADE_통계조건설정")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	GetManageValue(_T("기타"), _T("공통"), _T("국방대"), _T(""), m_strIs90);

	// CM/DM을 설정
	m_pCM = FindCM(_T("CM_통계조건설정"));

	// LC별 통계 보류
	/*
	if( _T("Y") == m_strIs90 )	m_pDM = FindDM(_T("DM_자산통계자료(국방대)"));
	else					m_pDM = FindDM(_T("DM_자산통계자료"));
	*/
	m_pDM = FindDM(_T("DM_자산통계자료"));

	if( m_pCM == NULL || m_pDM == NULL )
	{
		ESLAfxMessageBox(_T("통계 조건설정 화면을 초기화 할 수 없습니다."));
		return FALSE;
	}
	
	// 사용자정의코드 사용여부를 설정
	m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USERDEFINECODE'"), m_strUseCodeYN);

	// 멀티 콤보박스를 생성
	m_pComboMgr =  new CMulitComboBoxManager(this);

	// 콤보박스의 내용을 초기화
	if( Init() == FALSE )
	{
		ESLAfxMessageBox(_T("통계 조건설정 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//OnSelchangecmbSTATBASICCLASS();	


	
	//===================================================
	//2010.05.02 ADD BY PJW : 통합 관리자에 경우에만 관리구분을 선택가능
	if(NULL != m_pInfo && _T("S") != m_pInfo->USER_PRIVILEGE)
	{
		GetDlgItem(IDC_stcManageCode)->ShowWindow(SW_HIDE);
 		GetDlgItem(IDC_cmbManageCode)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_stcManageCode)->ShowWindow(SW_SHOW);
 		GetDlgItem(IDC_cmbManageCode)->ShowWindow(SW_SHOW);
	}
	//===================================================


	// KOL.RED.2018.220 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//시작등록번호|E|11023|||||||
	//끝등록번호|E|11024|||||||
	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_통계조건설정"), _T("시작등록번호"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_통계조건설정"), _T("끝등록번호"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}
	// KOL.RED.2018.220 ---------------------------------------------------------------------------	

	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  
}

BOOL CStatisticsConditionDlg::Init()
{
	INT nUseCodeCnt = 0;
	CString strAcqCode, strTmp ;



	// 가로줄 조건설정값.
	//m_arrHorizontalData.Add(_T("전체자료"));
	//m_arrHorizontalData.Add(_T("KDC주제구분"));
	//m_arrHorizontalData.Add(_T("DDC주제구분"));
	m_arrHorizontalData.Add(_T("수입구분"));

	
	//m_arrHorizontalDesc.Add(_T("전체자료"));
	//m_arrHorizontalDesc.Add(_T("KDC대주제부호관리"));
	//m_arrHorizontalDesc.Add(_T("DDC대주제부호관리"));
	m_arrHorizontalDesc.Add(_T("수입구분"));


	// 세로줄 조건설정값.	
	m_arrVerticalData.Add(_T("업무구분별통계"));
	m_arrVerticalData.Add(_T("자료구분별통계"));
	m_arrVerticalData.Add(_T("등록구분별통계"));
	m_arrVerticalData.Add(_T("별치구분별통계"));
	m_arrVerticalData.Add(_T("보조등록구분별통계"));
	
	m_arrVerticalDesc.Add(_T("업무구분"));
	m_arrVerticalDesc.Add(_T("자료구분"));
	m_arrVerticalDesc.Add(_T("등록구분"));
	m_arrVerticalDesc.Add(_T("별치기호"));
	m_arrVerticalDesc.Add(_T("보조등록구분"));

	if( m_strUseCodeYN == _T("Y") )
	{
		CString strYN, strName;
		m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 91"), strYN);
		if( strYN == _T("Y") ) 
		{
			m_pDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 91"), strName);
			strTmp.Format(_T("%s별통계"), strName);
			m_arrVerticalData.Add(strTmp);
			m_arrVerticalDesc.Add(_T("도서분류코드1"));
			m_strUserCodeAlias.Add(_T("도서분류코드1"));
		}
		m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 92"), strYN);
		if( strYN == _T("Y") ) 
		{
			m_pDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS from MN_USER_DEFINE_TBL WHERE CLASS_CODE = 92"), strName);
			strTmp.Format(_T("%s별통계"), strName);
			m_arrVerticalData.Add(strTmp);
			m_arrVerticalDesc.Add(_T("도서분류코드2"));
			m_strUserCodeAlias.Add(_T("도서분류코드2"));
		}
		m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 93"), strYN);
		if( strYN == _T("Y") ) 
		{
			m_pDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 93"), strName);
			strTmp.Format(_T("%s별통계"), strName);
			m_arrVerticalData.Add(strTmp);
			m_arrVerticalDesc.Add(_T("도서분류코드3"));
			m_strUserCodeAlias.Add(_T("도서분류코드3"));
		}
	}
	// END:

	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("KDC주제구분"), &m_arrHorizontalData, _T("자료기준"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &m_arrVerticalData, _T("통계물목록"));
	
	


	
	

	// 그룹정보 클래스를 생성하고 수입구분상태를 설정한다.
	m_pGroupCollection = new CGroupCollection(this);
	m_pGroupCollection->SetAcqType(m_nAcqType);

	// 자료구분일/자료구분 콤보박스를 초기화 및 등록한다.
	SetDataClassCtrl();
	
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 자료구분/자료구분일의 콤보박스를 초기화 및 설정한다. 
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CStatisticsConditionDlg::SetDataClassCtrl()
{
	CString strAcqCode;
	
	m_nAcqType = ACQ_TYPE_INVALID;

	// 그룹정보클래스를 초기화한다.
	m_pGroupCollection->RemoveAll();
	m_pGroupCollection->SetAcqType(m_nAcqType);
	
	// 수입구분 상태에 따라 자료구분 및 자료구분일 콤보박스를 설정한다.
	m_arrDataClass.RemoveAll();
	m_arrDateClass.RemoveAll();

	
	m_arrDataClass.Add(_T("기간별통계구축"));
	m_arrDateClass.Add(_T("배가일"));
	m_arrDateClass.Add(_T("입력일"));
	m_arrDateClass.Add(_T("등록일"));
	
	
	/*
	else if( m_nAcqType == PURCHASE )
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
	else if( m_nAcqType == DONATE )
	{
		m_arrDataClass.Add(_T("기간별통계구축"));
		m_arrDataClass.Add(_T("접수번호별통계구축"));

		m_arrDateClass.Add(_T("입력일"));
		m_arrDateClass.Add(_T("등록일"));
		GetDlgItem(IDC_btnSTAT_GROUP_VIEW)->SetWindowText(_T("접수번호보기"));
		GetDlgItem(IDC_stcSTAT_GROUPINFO)->SetWindowText(_T("접수번호"));
		GetDlgItem(IDC_cmbSTAT_WORK_CODE)->EnableWindow(FALSE);
	}	
	*/
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrDataClass.GetAt(0), &m_arrDataClass, _T("자료구분"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_arrDateClass.GetAt(0), &m_arrDateClass, _T("자료구분일"));

	


	// 리소스들을 등록한다.
	SetCtrlByDataClass();
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 선택되어진 자료구분값에 따라 리소스들의 표시를 설정한다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CStatisticsConditionDlg::SetCtrlByDataClass()
{
	INT nCount1, nCount2, i, nCurSel;
	INT nArrCodeResourceID1[10], nArrCodeResourceID2[10];
	CESL_ControlComboBox* pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("자료구분"));

	nCount1 = nCount2 = 0;

	// 기간별 통계에 나타내어질 리소스들
	nArrCodeResourceID1[nCount1++] = IDC_stcSTAT_DATE_CLASS;
	nArrCodeResourceID1[nCount1++] = IDC_cmbSTAT_DATE_CLASS;
	nArrCodeResourceID1[nCount1++] = IDC_edtSTAT_DATE1;
	nArrCodeResourceID1[nCount1++] = IDC_stcSTAT_DATE_RANGE;
	nArrCodeResourceID1[nCount1++] = IDC_edtSTAT_DATE2;
	
	// 그룹정보(차수/접수번호) 통계에 나타내어질 리소스들
	nArrCodeResourceID2[nCount2++] = IDC_edtSTAT_GROUPINFO;
	nArrCodeResourceID2[nCount2++] = IDC_btnSTAT_GROUP_VIEW;
	nArrCodeResourceID2[nCount2++] = IDC_edtSTAT_ACQ_YEAR;
	nArrCodeResourceID2[nCount2++] = IDC_stcSTAT_ACQ_YEAR;
	nArrCodeResourceID2[nCount2++] = IDC_stcSTAT_GROUPINFO;
	nArrCodeResourceID2[nCount2++] = IDC_stcSTAT_WORK_CODE;
	nArrCodeResourceID2[nCount2++] = IDC_cmbSTAT_WORK_CODE;


	// 수입구분 콤보의 설정에 따라 리소스들의 표시를 설정한다.

	nCurSel = ( (CComboBox*) GetDlgItem(IDC_cmbSTAT_ACQ_CODE) )->GetCurSel();

	//nCurSel = pCMB->GetCurSel();
	switch( nCurSel )
	{
	case 0:
		{
			for( i=0 ; i<nCount1 ; i++ )
			{
				GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_SHOW);
			}
			for( i=0 ; i<nCount2 ; i++ )	
			{
				GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_HIDE);
			}
		}
	case 1:
		{
			for( i=0 ; i<nCount1 ; i++ )	
			{
				GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_SHOW);
			}
			for( i=0 ; i<nCount2 ; i++ )	
			{
				GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_HIDE);
			}
		}
		break;

	case 2:
		{
			for( i=0 ; i<nCount1 ; i++ )	
			{
				//===================================================
				//2009.03.28 UPDATE BY PJW : 구입이외의 업무구분일 경우도 구입과 컨트롤을 맞춘다.
// 				GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_HIDE);
				GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_SHOW);
				//===================================================
				
			}
			for( i=0 ; i<nCount2 ; i++ )	
			{
				//===================================================
				//2009.03.28 UPDATE BY PJW : 구입이외의 업무구분일 경우도 구입과 컨트롤을 맞춘다.
// 				GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_SHOW);
				GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_HIDE);
				//===================================================
				
			}
		}
		break;

	default:
		{
			for( i=0 ; i<nCount1 ; i++ )	
			{
				//===================================================
				//2009.03.28 UPDATE BY PJW : 구입이외의 업무구분일 경우도 구입과 컨트롤을 맞춘다.
// 				GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_HIDE);
				GetDlgItem(nArrCodeResourceID1[i])->ShowWindow(SW_SHOW);
				//===================================================
				
			}
			for( i=0 ; i<nCount2 ; i++ )	
			{
				//===================================================
				//2009.03.28 UPDATE BY PJW : 구입이외의 업무구분일 경우도 구입과 컨트롤을 맞춘다.
// 				GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_HIDE);
				GetDlgItem(nArrCodeResourceID2[i])->ShowWindow(SW_HIDE);
				//===================================================
				
			}
		}
		break;
	}

}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 자료구분이 차수/접수번호별일때 리스트창을 열고 통계자료를 구축한다.
//
// [ 미니스펙 ]
// 1. 수입년도를 체크한다.(필수항목)
// 2. 수입구분에따라 차수/접수 번호 리스트 창을 연다.
// 3. 통계자료를 구축한다.( 하지않는다.) -> 다른조건설정을 할 수 있으므로
//
/////////////////////////////////////////////////////////////////////////////
VOID CStatisticsConditionDlg::OnbtnSTATGROUPVIEW() 
{
	/*
	CString strAcqYear, strSelDivNo, strWorkCode;

	//=======================================================================
	// 1. 수입년도를 체크한다.(필수항목)
	//=======================================================================
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	if( strAcqYear.IsEmpty() == TRUE ) 
	{
		ESLAfxMessageBox(_T("수입년도 정보를 입력해 주십시요"));
		return;
	}

	//=======================================================================
	// 2. 수입구분에따라 차수/접수 번호 리스트 창을 연다.
	//=======================================================================
	switch( m_nAcqType )
	{
	case PURCHASE:
	{
		m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);

		CDivisionNo_List_Dlg dlg(this);
		dlg.SetWorkStatus(_T("전체자료"));
		dlg.SetAcquisitYear(strAcqYear);
		dlg.SetWorkCode(strWorkCode);
		dlg.SetGroupCollection(m_pGroupCollection);

			if( IDOK == dlg.DoModal() )
		{
			strSelDivNo = dlg.GetSelDivNo();
			m_pCM->SetControlMgrData(_T("그룹번호"), strSelDivNo);
			m_pCM->SetControlMgrData(_T("업무구분"), m_pGroupCollection->GetWorkCode());
			m_pCM->SetControlMgrData(_T("수입년도"), m_pGroupCollection->GetAcqYear());
		}
	}	
		break;
	case DONATE:
	{
		CRequisitionNo_List_Dlg dlg(this);
		dlg.SetSpeciesType(SPECIES_TYPE_INVALID);
		dlg.SetAcqYear(strAcqYear);
		dlg.SetGroupCollection(m_pGroupCollection);

			if( IDOK == dlg.DoModal() )
		{
			strSelDivNo = dlg.GetSelInfo();
			m_pCM->SetControlMgrData(_T("그룹번호"), strSelDivNo);
			m_pCM->SetControlMgrData(_T("수입년도"), m_pGroupCollection->GetAcqYear());
		}
	}	
		break;
	default:	return;
		break;
	}

	//=======================================================================
	// 3. 통계자료를 구축한다.
	//=======================================================================
	//OnOK();
	*/
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 조건설정클래스에 조건을 설정하여 쿼리문을 생성하고 통계자료를 구축한다.
//
// [ 미니스펙 ]
// 1. 자료구분에 해당하는 조건들이 유효한지 판단하고 조건을 설정한다.
// 2. 수입구분을 조건설정클래스에 설정한다.
// 3. 가로열의 조건을 설명값으로 조건설정클래스에 설정한다.
// 4. 세로열의 조건을 설명값과 Field값으로 조건설정클래스에 설정한다.
// 5. 조건설정클래스에 설정된 값으로 쿼리문을 구축한다.
// 6. 통계자료를 구축한다.
//
/////////////////////////////////////////////////////////////////////////////
VOID CStatisticsConditionDlg::OnOK()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT ids, nCurSel, i;
	CString strFieldName, strVerticalDesc;
	CESL_ControlComboBox* pCMB	= (CESL_ControlComboBox*)m_pCM->FindControl(_T("자료기준"));
	CESL_ControlComboBox* pCMB1 = (CESL_ControlComboBox*)m_pCM->FindControl(_T("통계물목록"));
		
	m_nStartRow = 0;
	m_bDonatorFlag = FALSE;

	//=======================================================================
	// 1. 자료구분에 해당하는 조건들이 유효한지 판단하고 조건을 설정한다.
	//=======================================================================
	ids = CheckValidateInputValue();
	if( ids < 0 )	 return ;

	//=======================================================================
	// 2. 수입구분을 조건설정클래스에 설정한다.
	//=======================================================================
	m_pStatCondition->SetAcqType(m_nAcqType);
	
	//=======================================================================
	// 3. 가로열의 조건을 설명값으로 조건설정클래스에 설정한다.
	//=======================================================================
	nCurSel = pCMB->GetCurSel();
	m_pStatCondition->SetClassNoType(m_arrHorizontalDesc.GetAt(nCurSel));

	//=======================================================================
	// 4. 세로열의 조건을 설명값과 Field값으로 조건설정클래스에 설정한다.
	//=======================================================================
	nCurSel = pCMB1->GetCurSel();
	strVerticalDesc = m_arrVerticalDesc.GetAt(nCurSel);	
	if	( strVerticalDesc == _T("업무구분") )		strFieldName = _T("I.WORK_CODE");
	else if	( strVerticalDesc == _T("등록구분") )		strFieldName = _T("B.REG_CODE");
	else if	( strVerticalDesc == _T("별치기호") )		strFieldName = _T("B.SEPARATE_SHELF_CODE");
	else if	( strVerticalDesc == _T("자료구분") )		strFieldName = _T("I.MAT_CODE");
	else if	( strVerticalDesc == _T("보조등록구분") )	strFieldName = _T("I.SUB_REG_CODE");
	else if	( strVerticalDesc == _T("직원부서") )		strFieldName = _T("P.CLASS_CODE");	
	else if	( strVerticalDesc == _T("기증자별")	)		
	{
		m_bDonatorFlag = TRUE;
		strFieldName = _T("P.DONATOR_KEY");
	}
	else strFieldName.Empty();

	// 2004.11.06 사용자 정의코드 사용.
	// BEGIN:
	for( i=0; i<m_strUserCodeAlias.GetSize(); i++ )
	{
		if( strVerticalDesc == m_strUserCodeAlias[i] ) 
			strFieldName.Format(_T("I.USER_DEFINE_CODE%d"), i+1);
	}
	// END:
	m_pStatCondition->SetVerticalInfo(strVerticalDesc, strFieldName);
	m_pStatCondition->SetDonatorFlag(m_bDonatorFlag);


	//===================================================
	//2009.04.09 ADD BY PJW : 등록번호를 조건에 추가함.
	CString strStartRegNo;
	CString strEndRegNo;
	m_pCM->GetControlMgrData(_T("시작등록번호"), strStartRegNo);
	m_pCM->GetControlMgrData(_T("끝등록번호"), strEndRegNo);

	m_pStatCondition->SetStartRegNo(strStartRegNo);
	m_pStatCondition->SetEndRegNo(strEndRegNo);
	//===================================================

	//=======================================================================
	// 5. 조건설정클래스에 설정된 값으로 쿼리문을 구축한다.
	//=======================================================================
	//===================================================
	//2010.05.02 ADD BY PJW : 통합대출자일경우 선택한 관리구분이 들어가도록 수정함
	if(NULL != m_pInfo && _T("S") == m_pInfo->USER_PRIVILEGE)
	{
		CString strManageCode = _T("");
		CESL_ControlMultiComboBox* pcmbManageCode = (CESL_ControlMultiComboBox*)GetDlgItem(IDC_cmbManageCode);
		INT nCur = pcmbManageCode->GetCurSel();
		pcmbManageCode->GetData(strManageCode, nCur, 0);
		strManageCode.TrimLeft(); strManageCode.TrimRight();
		m_pStatCondition->SetManageCode(strManageCode);

	}
	//====================================================

	//====================================================
	// 2012.10.18 ADD BY BJS : 2012년도 도서관 통합서비스 확대구축 사업 2차 (배가상태에 따른 자료실 조건의 추가)
	CString strReference = _T("");
	if( ((CButton*)GetDlgItem(IDC_ReferenceCheck))->GetCheck() == BST_CHECKED )
	{
		if(IDCANCEL==AfxMessageBox(_T("배가상태를 구분하지 않으면 모든 자료에 대한 통계가 산출됩니다! 계속하시겠습니까?"), MB_OKCANCEL))
		{
			return;
		}
		strReference = _T("전체");
		strReference.TrimLeft(); strReference.TrimRight();
		m_pStatCondition->SetReference(strReference);
		m_pStatCondition->AddDateInfo(_T(""), _T(""));
	}
	else
	{		
		
		CESL_ControlMultiComboBox* pcmbBTS_Reference = (CESL_ControlMultiComboBox*)GetDlgItem(IDC_cmbBTS_Reference);
		INT nCur2 = pcmbBTS_Reference->GetCurSel();
		pcmbBTS_Reference->GetData(strReference, nCur2, 0);
		strReference.TrimLeft(); strReference.TrimRight();
		m_pStatCondition->SetReference(strReference);
		
	}
	
	//===================================================

	

	ids = m_pStatCondition->SetWhereStmt();
	if( ids < 0 )	
	{
		ESLAfxMessageBox(_T("통계자료를 구축할 수 없습니다."));
		return;
	}

	//=======================================================================
	// 6. 통계자료를 구축한다.
	//=======================================================================
	MakeStatistics();
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 자료구분의 콤보변경시 해당리소스의 표시를 설정한다.
/////////////////////////////////////////////////////////////////////////////
VOID CStatisticsConditionDlg::OnSelchangecmbSTATDATACLASS() 
{
	SetCtrlByDataClass();
}

BOOL CStatisticsConditionDlg::PreTranslateMessage(MSG* pMsg) 
{
 	if( pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		GetDlgItem(IDOK)->SetFocus();
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CStatisticsConditionDlg::OnKillfocusedtSTATGROUPINFO() 
{
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
	if(!GetDlgItem(IDC_edtSTAT_ACQ_YEAR)->IsWindowVisible()) return ;
	OnKillfocusedtSTATGROUPINFO();
}

VOID CStatisticsConditionDlg::OnSelchangecmbSTATACQCODE() 
{
	SetDataClassCtrl();
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 자료구분값에 따라 유효한지 검사한다. 
//	(기간별 : CheckValidateDateInputValue(), 차수/접수번호별 : CheckValidateGroupInputValue() )
// [ 미니스펙 ]
/////////////////////////////////////////////////////////////////////////////
INT CStatisticsConditionDlg::CheckValidateInputValue()
{
	INT ids, nCurSel;
	CESL_ControlComboBox* pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("자료구분"));
	
	nCurSel = pCMB->GetCurSel();
	switch( nCurSel )
	{
	case 0:		ids = CheckValidateDateInputValue();
		break;
	case 1:		ids = CheckValidateGroupInputValue();
		break;
	default:	ids = -1;
		break;
	}

	return ids;
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 자료구분이 기간별통계구축일 경우 조건의 유효성을 판단하고 조건설정 클래스를 설정한다.
//
// [ 미니스펙 ]
// 1. 에디트박스에서 날짜를 가져온다.
// 2. CStaticsCondition 클래스를 설정한다.
// 3. 자료구분일의 값에 따라 검색할 DATE_FIELD를 설정한다.( CStaticsCondition )
//
/////////////////////////////////////////////////////////////////////////////
INT CStatisticsConditionDlg::CheckValidateDateInputValue()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT nCurSel;
	CString strDate1, strDate2, strDateClass;
	CESL_ControlComboBox* pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("자료구분일"));

	//=======================================================================
	// 1. 에디트박스에서 날짜를 가져온다.
	//=======================================================================
	m_pCM->GetControlMgrData(_T("날짜1"), strDate1);
	m_pCM->GetControlMgrData(_T("날짜2"), strDate2);
	if( strDate1.IsEmpty() == TRUE && strDate2.IsEmpty() == TRUE )
	{
		ESLAfxMessageBox(_T("검색일 조건을 입력해 주십시오."));
		return -1;
	}
	else if( strDate1.IsEmpty() == TRUE && strDate2.IsEmpty() == FALSE )
	{
		ESLAfxMessageBox(_T("검색 시작일을 입력해 주십시오."));
		return -1;
	}
	
	
	//=======================================================================
	// 2. CStaticsCondition 클래스를 설정한다.
	//=======================================================================
	m_pStatCondition->InitDateInfo();
	m_pStatCondition->SetStatType(STATISTICS_BY_DATE);
	m_pStatCondition->AddDateInfo(_T("날짜1"), strDate1);
	m_pStatCondition->AddDateInfo(_T("날짜2"), strDate2);

	//=======================================================================
	// 3. 자료구분일의 값에 따라 검색할 DATE_FIELD를 설정한다.( CStaticsCondition )
	//=======================================================================
	nCurSel = pCMB->GetCurSel();
	strDateClass = m_arrDateClass.GetAt(nCurSel);

	if( ((CButton*)GetDlgItem(IDC_ReferenceCheck))->GetCheck() == BST_UNCHECKED )
	{
		if		( strDateClass == _T("입력일") )
		{
			m_pStatCondition->AddDateInfo(_T("DATE_FIELD"), _T("B.INPUT_DATE"));
		}
		else if	( strDateClass == _T("등록일") )
		{
			m_pStatCondition->AddDateInfo(_T("DATE_FIELD"), _T("B.REG_DATE"));
		}
		else if	( strDateClass == _T("주문일") )
		{
			m_pStatCondition->AddDateInfo(_T("DATE_FIELD"), _T("P.ORDER_DATE"));
		}
		else if	( strDateClass == _T("검수일") )
		{
			m_pStatCondition->AddDateInfo(_T("DATE_FIELD"), _T("P.CHECKIN_DATE"));
		}
		else if	( strDateClass == _T("배가일") )
		{
			m_pStatCondition->AddDateInfo(_T("DATE_FIELD"), _T("B.SHELF_DATE"));
		}
		else	
		{
			ESLAfxMessageBox(_T("자료구분일을 설정하여 주십시오."));
			return -1;
		}
	}
	
	

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 자료구분이 차수/접수번호별통계구축일 경우 조건의 유효성을 판단하고 조건설정클래스를 설정한다.
//
// [ 미니스펙 ]
// 1. 화면에서 수입년도/그룹번호/업무구분 값을 가져온다.
// 2. 그룹번호(차수/접수번호)의 유효성을 판단한다.
// 3. 접수/차수번호가 없다면 에러리턴한다.
// 4. CStaticsCondition 클래스(조건설정클래스)에 그룹정보를 설정한다.
//
/////////////////////////////////////////////////////////////////////////////
INT CStatisticsConditionDlg::CheckValidateGroupInputValue()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	CString strAcqYear, strGroupInfo, strWorkCode;

	//=======================================================================
	// 1. 화면에서 수입년도/그룹번호/업무구분 값을 가져온다.
	//=======================================================================
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("그룹번호"), strGroupInfo);
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	
	//=======================================================================
	// 2. 그룹번호(차수/접수번호)의 유효성을 판단한다.
	//=======================================================================
	strGroupInfo = CheckValidateGroupNo(strGroupInfo, strAcqYear, strWorkCode);

	//=======================================================================
	// 3. 접수/차수번호가 없다면 에러리턴한다.
	//=======================================================================
	if( strGroupInfo.IsEmpty() == TRUE )
	{
		switch( m_nAcqType )
	{
		case PURCHASE:	ESLAfxMessageBox(_T("통계자료를 구축할 차수정보가 없습니다."));
			break;
		case DONATE:	ESLAfxMessageBox(_T("통계자료를 구축할 접수번호정보가 없습니다."));
			break;
		default:		ESLAfxMessageBox(_T("통계자료를 구축할 정보가 없습니다."));
			break;
		}
		return -1;
	}

	//=======================================================================
	// 4. CStaticsCondition 클래스(조건설정클래스)에 그룹정보를 설정한다.
	//=======================================================================
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
	/*
	CESL_ControlComboBox* pCMB = (CESL_ControlComboBox*)m_pCM->FindControl(_T("자료기준"));

	INT nCurSel = pCMB->GetCurSel();
	m_pStatCondition->SetClassNoType(m_arrHorizontalDesc.GetAt(nCurSel));

	::SendMessage(pParentMgr->GetSafeHwnd(), CHANGE_CLASS_NO_TYPE, 0, 0);
	*/
}

VOID CStatisticsConditionDlg::OnJobCancel(WPARAM w,LPARAM l)
{
	ThreadTerminate();
}


HBRUSH CStatisticsConditionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CStatisticsConditionDlg::OnKillfocusedtENDREGNO() 
{	
	CString strRegNo;
	INT nIds;

	CEdit* pEdt = (CEdit*)GetDlgItem(IDC_edtEND_REG_NO);

	nIds = 0;
	strRegNo = _T("");

	m_pCM->GetControlMgrData( _T("끝등록번호"), strRegNo);	

	nIds = MakeRegNo( strRegNo );

	if( nIds == -1001 )
	{
		AfxMessageBox(_T("등록번호 자릿수가 맞지 않습니다."));
		m_pCM->SetControlMgrData(_T("끝등록번호"), _T(""));	
		pEdt->SetFocus();
		return;
	}
	else if( nIds == -1002 )
	{
		AfxMessageBox(_T("등록번호 12자리 변환을 실패 했습니다."));
		m_pCM->SetControlMgrData(_T("끝등록번호"), _T(""));	
		pEdt->SetFocus();
		return;
	}

	m_pCM->SetControlMgrData(_T("끝등록번호"), strRegNo);	
}	

void CStatisticsConditionDlg::OnKillfocusedtSTARTREGNO() 
{
	CString strRegNo;
	INT nIds;

	CEdit* pEdt = (CEdit*)GetDlgItem(IDC_edtSTART_REG_NO);

	nIds = 0;
	strRegNo = _T("");

	m_pCM->GetControlMgrData( _T("시작등록번호"), strRegNo);	

	nIds = MakeRegNo( strRegNo );

	if( nIds == -1001 )
	{
		AfxMessageBox(_T("등록번호 자릿수가 맞지 않습니다."));
		m_pCM->SetControlMgrData(_T("시작등록번호"), _T(""));	
		pEdt->SetFocus();
		return;
	}
	else if( nIds == -1002 )
	{
		AfxMessageBox(_T("등록번호 자릿수 변환에 실패 했습니다."));
		m_pCM->SetControlMgrData(_T("시작등록번호"), _T(""));	
		pEdt->SetFocus();
		return;
	}

	m_pCM->SetControlMgrData(_T("시작등록번호"), strRegNo);	
}

//===================================================
//2009.04.09 ADD BY PJW : 등록번호를 12자리로 변경한다.
INT CStatisticsConditionDlg::MakeRegNo(CString &strRegNo)
{
	INT nLength;
	INT nRegCodeLength;
	CString strRegCode;
	CString strRegFigure;
	CString strTempRegNo;

	strRegNo.Replace(_T(" "),_T(""));
	strRegNo.MakeUpper();
	if(!strRegNo.IsEmpty())
	{
		nLength = strRegNo.GetLength();
		nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
		if( nRegCodeLength < nLength  && 12 >= nLength )
		{
			
			strRegCode = strRegNo.Left(nRegCodeLength);
			strRegFigure = strRegNo.Mid(nRegCodeLength);
			strTempRegNo = m_pInfo->MakeRegNo( strRegCode, strRegFigure );
			if(strTempRegNo == _T("error"))
			{
				return -1002;
			}

			strRegNo = strTempRegNo;
		}
		else
		{
			return -1001;
		}
	}	

	return 0;
}
//===================================================