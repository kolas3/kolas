// DivisionNoInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DivisionNoInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDivisionNoInfoDlg dialog

CDivisionNoInfoDlg::CDivisionNoInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CInfoDlg(pParent, CDivisionNoInfoDlg::IDD)
{
	//{{AFX_DATA_INIT(CDivisionNoInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pGroup = NULL;
}

CDivisionNoInfoDlg::~CDivisionNoInfoDlg()
{
	
}

VOID CDivisionNoInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDivisionNoInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDivisionNoInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CDivisionNoInfoDlg)
	ON_CBN_SELCHANGE(IDC_cmbBSDIVNO_ACQ_CODE, OnSelchangecmbBSFORMACQCODE)
	ON_EN_KILLFOCUS(IDC_edtBSDIVNO_ACQ_YEAR, OnKillfocusedtBSFORMACQYEAR)
	ON_CBN_SELCHANGE(IDC_cmbBSDIVNO_GROUP_NO, OnSelchangecmbBSFORMGROUPNO)
	ON_CBN_SELCHANGE(IDC_cmbBSDIVNO_WORK_NO, OnSelchangecmbBSFORMWORKNO)
	ON_BN_CLICKED(IDC_btnBSDIVNO_CREATE, OnbtnBSFORMGROUPNOCREATE)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(CHANGE_WORK_CODE, OnChangeWorkCode)
	ON_MESSAGE(SET_TAB_ORDER, OnSetTabOrder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDivisionNoInfoDlg message handlers

BOOL CDivisionNoInfoDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}	

BOOL CDivisionNoInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("K2UPGRADE_단행_종_차접수정보")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	//CM/DM설정
	m_pCM = FindCM(_T("CM_단행_종_차접수정보"));	
	if( m_pCM == NULL )
	{
		ESLAfxMessageBox(_T("[CM_단행_종_차접수정보]을 설정할 수 없습니다."));
		return FALSE;
	}

	//DB연결정보 설정
	this->SetCONNECTION_INFO(GetDM()->CONNECTION_INFO);
	
	m_pCM->AllControlDisplay();

	if( GetDM()->GetRowCount() == 0 )
	{
		GetDM()->InsertRow(-1);
		GetDM()->SetCellData(_T("BP_차수업무구분"), _T("1"), 0);
	}

	if( Init() == FALSE )
	{
		ESLAfxMessageBox(_T("차수/접수 정보 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	CString strValue = GetDM()->GetCellData(_T("BP_수입년도"), 0);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;
}

BOOL CDivisionNoInfoDlg::Init()
{
	CString strTmpData;

	switch( GetAcqType() )
	{
	case ACQ_TYPE_INVALID:		strTmpData = _T("");
		break;
	case OTHER_ACQ_TYPE:		strTmpData = GetGroupCollection()->GetAcqCode();
		break;
	default:					strTmpData.Format(_T("%d"), GetAcqType());
		break;
	}
	m_pCM->SetControlMgrData(_T("BP_수입구분_코드"), strTmpData);
	

	if( GetOpType() != SINSERT || (GetSpeciesType() != BO_REG_INVALID && GetSpeciesType() != BO_REG_PURCHASE && GetSpeciesType() != BO_REG_DONATE) )
	{
		CESL_ControlComboBox * pElement = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_수입구분_코드"));
		pElement->EnableWindow(FALSE);
	}
	
	if( GetOpType() == SINSERT )
	{
		if( GetGroupCollection() )
		{
			strTmpData = GetGroupCollection()->GetAcqYear();
			if( strTmpData.IsEmpty() == TRUE )		strTmpData = GetTodayYear();

			m_pCM->SetControlMgrData(_T("BP_수입년도"), strTmpData);
		}
	}
	//차수/접수번호 정보 컨트롤 설정
	SetCtrlByAcqCode();
	//차수/접수번호 정보 구축 및 초기 설정값 지정
	if( GetGroupCollection()->GetHaveGroupInfo() )
	{
		((CEdit*)GetDlgItem(IDC_edtBSDIVNO_ACQ_YEAR))->SetReadOnly(TRUE);
	}
	
	INT ids = MakeGroupNoList();
	if(ids < 0) return FALSE;

	strTmpData = GetDM()->GetCellData(_T("BP_수입년도"), 0);
	//차수업무구분을 설정한다. 저장시에 사용자가 다른 차수를 선택했는지를 알아내기 위해서 이다.
	//InsertWorkNo메소드 참조
	if( m_pGroup != NULL && GetOpType() != SINSERT )
	{
		GetDM()->SetCellData(_T("BP_차수업무구분"), m_pGroup->GetWorkCode(), 0);
	}
	
	return TRUE;
}

INT CDivisionNoInfoDlg::MakeGroupNoList()
{
	INT ids;
	CString strAcqYear, strGroupNo, strGroupName, strWorkNo, strWorkCode;
	
	GetDM()->GetCellData(_T("BP_수입년도"), 0, strAcqYear);
	GetDM()->GetCellData(_T("BP_그룹번호"), 0, strGroupNo);
	GetDM()->GetCellData(_T("BP_작업번호"), 0, strWorkNo);
	GetDM()->GetCellData(_T("BP_차수업무구분"), 0, strWorkCode);
	
	if( strAcqYear.IsEmpty() == TRUE )		strAcqYear = GetTodayYear();
	
	if( GetGroupCollection()->GetGroupCount() == 0 )
	{
		ids = GetGroupCollection()->MakeGroupInfo(strAcqYear, strGroupNo, strWorkCode);
		if( ids < 0 )	return -1;
	}
	
	m_pGroup = NULL;

	if( GetGroupCollection()->GetGroupCount() > 0 )
	{
		GetGroupCollection()->SetCurSelGroupInfo(strAcqYear, strGroupNo, strWorkNo, strWorkCode);
		m_pGroup = GetGroupCollection()->GetCurSelGroupInfo();
		CStringArray tmpArray;
		CString strCurSelInfo;
		GetGroupCollection()->GetGroupInfo(&tmpArray, strCurSelInfo);
		SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetGroupInfo(), &tmpArray, _T("BP_그룹번호"));
		if( GetAcqType() == PURCHASE )
		{
			CStringArray * pArray = m_pGroup->GetArrayWorkNo();
			SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetWorkNo(-1), pArray, _T("BP_작업번호"));
			SendPurchaseSeqNoChangeMessage();
		}
	}
	else
	{
		CStringArray tmpArray;		tmpArray.RemoveAll();
		SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &tmpArray, _T("BP_그룹번호"));
		if( GetAcqType() == PURCHASE )
		{
			SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &tmpArray, _T("BP_작업번호"));
		}
	}

	return 0;
}

VOID CDivisionNoInfoDlg::OnSelchangecmbBSFORMACQCODE() 
{
	// TODO: Add your control notification handler code here
	CString strAcqCode;
	m_pCM->GetControlMgrData(_T("BP_수입구분_코드"), strAcqCode);
	if(strAcqCode == _T("1")) SetAcqType(PURCHASE);
	else if(strAcqCode == _T("2")) SetAcqType(DONATE);
	else if(strAcqCode.IsEmpty()) SetAcqType(ACQ_TYPE_INVALID);
	else SetAcqType(OTHER_ACQ_TYPE);
	
	CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)m_pCM->FindControl(_T("BP_수입구분_코드"));
	if(pCMB) pCMB->ShowDropDown(FALSE);

	::PostMessage(pParentMgr->GetSafeHwnd(), ACQ_TYPE_CHANGE, _ttoi(strAcqCode.GetBuffer(0)), 0);	
}

VOID CDivisionNoInfoDlg::OnKillfocusedtBSFORMACQYEAR() 
{
	// TODO: Add your control notification handler code here
	if(GetDlgItem(IDC_edtBSDIVNO_ACQ_YEAR)->GetStyle() & ES_READONLY) return ;

	CString strAcqYear, strSrcAcqYear;
	m_pCM->GetControlMgrData(_T("BP_수입년도"), strAcqYear);
	if(strAcqYear.IsEmpty()) 
	{
		strAcqYear = GetDM()->GetCellData(_T("BP_수입년도"), 0);
		m_pCM->SetControlMgrData(_T("BP_수입년도"), strAcqYear);
		if(strAcqYear.IsEmpty())
		{
			ESLAfxMessageBox(_T("수입년도 정보를 설정해 주십시요."));
			return ;
		}
	}
	GetDM()->GetCellData(_T("BP_수입년도"), 0, strSrcAcqYear);
	if(strSrcAcqYear != strAcqYear)
	{
		GetDM()->SetCellData(_T("BP_그룹번호"), _T(""), 0);
		GetDM()->SetCellData(_T("BP_수입년도"), strAcqYear, 0);
		GetGroupCollection()->RemoveAll();
		INT ids = MakeGroupNoList();
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("그룹정보를 구축할 수 없습니다."));
			return;
		}		
	}
}

VOID CDivisionNoInfoDlg::OnSelchangecmbBSFORMGROUPNO() 
{
	// TODO: Add your control notification handler code here
	CString strWorkCode;
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_그룹번호"));
	INT nCurSel = pCmb->GetCurSel();
	if(nCurSel < 0) return ;
	GetGroupCollection()->SetCurSel(nCurSel);
	m_pGroup = GetGroupCollection()->GetCurSelGroupInfo();
	if(GetAcqType() == PURCHASE && m_pGroup)
	{
		CStringArray * pArray = m_pGroup->GetArrayWorkNo();
		SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetWorkNo(-1), pArray, _T("BP_작업번호"));
		strWorkCode = m_pGroup->GetWorkCode();
		SendPurchaseSeqNoChangeMessage();
	}
}

VOID CDivisionNoInfoDlg::OnSelchangecmbBSFORMWORKNO() 
{
	CESL_ControlComboBox* pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("BP_작업번호"));
	INT nCurSel = pCmb->GetCurSel();
	GetGroupCollection()->SetCurSelWorkNo(nCurSel);
}

VOID CDivisionNoInfoDlg::OnbtnBSFORMGROUPNOCREATE() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear, strAddGroupNo, strAddWorkNo, strAddLastNo, strWorkCode, strAddAcqCode;
	GetDM()->GetCellData(_T("BP_수입년도"), 0, strAcqYear);
	if(strAcqYear.IsEmpty()) strAcqYear = GetTodayYear();
	if(GetAcqType() == PURCHASE)
	{
		CDivisionNo_Insert_Dlg dlg(this);
		dlg.SetOpenMode(DINSERT);
		dlg.SetAcqYear(strAcqYear);
		dlg.SetCONNECTION_INFO(GetDM()->CONNECTION_INFO);
		dlg.SetWorkCode(GetGroupCollection()->GetWorkCode());
		if(GetSpeciesType() == BO_PURCHASE) dlg.SetWorkStatus(_T("구입대상"));
		else if(GetSpeciesType() == BO_ORDER) dlg.SetWorkStatus(_T("주문"));
		else if(GetSpeciesType() == BO_REGOBJ || GetSpeciesType() == BO_REG_PURCHASE || GetSpeciesType() == BO_MISSING) 
			dlg.SetWorkStatus(_T("검수완료"));
		if(IDOK == dlg.DoModal())
		{
			dlg.GetInsertInfo(strAddGroupNo, strAddWorkNo, strAddLastNo, strWorkCode);
			GetGroupCollection()->AddGroupInfo(strAcqYear, strAddGroupNo, strWorkCode);
			m_pGroup = GetGroupCollection()->GetCurSelGroupInfo();
			if(m_pGroup) m_pGroup->SetLastSerialNo(_ttoi(strAddLastNo.GetBuffer(0)));
			CStringArray tmpArray;
			GetGroupCollection()->GetGroupInfo(&tmpArray, strAddLastNo);
			SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strAddGroupNo, &tmpArray, _T("BP_그룹번호"));
			GetGroupCollection()->GetWorkNoInfo(&tmpArray);
			SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strAddWorkNo, &tmpArray, _T("BP_작업번호"));
			SendPurchaseSeqNoChangeMessage();
		}
	}
	else if(GetAcqType() == DONATE)
	{
		CRequisitionNo_Insert_Dlg dlg(this);
		dlg.SetOpenMode(DINSERT);
		dlg.SetAcqYear(strAcqYear);
		if(IDOK == dlg.DoModal())
		{
			dlg.GetInsertInfo(strAddGroupNo);
			GetGroupCollection()->AddGroupInfo(strAcqYear, strAddGroupNo);
			m_pGroup = GetGroupCollection()->GetCurSelGroupInfo();
			CStringArray tmpArray;
			GetGroupCollection()->GetGroupInfo(&tmpArray, strAddGroupNo);
			SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strAddGroupNo, &tmpArray, _T("BP_그룹번호"));
		}
	}
	else if(GetAcqType() == OTHER_ACQ_TYPE)
	{
		COtherGroupNo_Insert_Dlg dlg(this);
		dlg.SetOpenMode(DINSERT);
		dlg.SetAcqCode(GetGroupCollection()->GetAcqCode());
		dlg.SetAcqYear(strAcqYear);
		if(IDOK == dlg.DoModal())
		{
			dlg.GetInsertInfo(strAddGroupNo, strAddAcqCode);
			GetGroupCollection()->AddGroupInfo(strAcqYear, strAddGroupNo, strAddAcqCode);
			m_pGroup = GetGroupCollection()->GetCurSelGroupInfo();
			CStringArray tmpArray;
			GetGroupCollection()->GetGroupInfo(&tmpArray, strAddGroupNo);
			SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strAddGroupNo, &tmpArray, _T("BP_그룹번호"));
		}
	}
}

VOID CDivisionNoInfoDlg::SetCtrlByAcqCode()
{
	switch( GetAcqType() )
	{
	case PURCHASE:
	{
			GetDlgItem(IDC_strBSFORM_GROUP_NO	)->SetWindowText(_T("차수번호"));
			GetDlgItem(IDC_strBSFORM_WORK_NO	)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_cmbBSDIVNO_WORK_NO	)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_btnBSDIVNO_CREATE	)->SetWindowText(_T("차수번호생성"));
	}
		break;
	case DONATE:
	{
			GetDlgItem(IDC_strBSFORM_GROUP_NO	)->SetWindowText(_T("접수번호"));
			GetDlgItem(IDC_strBSFORM_WORK_NO	)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_cmbBSDIVNO_WORK_NO	)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_btnBSDIVNO_CREATE	)->SetWindowText(_T("접수번호생성"));
	}
		break;
	case OTHER_ACQ_TYPE:
	{
			GetDlgItem(IDC_strBSFORM_GROUP_NO	)->SetWindowText(_T("그룹번호"));
			GetDlgItem(IDC_strBSFORM_WORK_NO	)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_cmbBSDIVNO_WORK_NO	)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_btnBSDIVNO_CREATE	)->SetWindowText(_T("그룹번호생성"));
		}
		break;
	}
}

INT CDivisionNoInfoDlg::ValidateData()
{
	// 2008.03.10 REM BY PDJ
	// 차수상태에 따른 제한을 두지 않는다.
	/*
	BOOL bError = FALSE;

	*******************************************************************
	 *******************************************************************
	//20080204 UPDATE BY PJW : 수입구분에따라서 메세지 박스 보이도록 수정
	**
	if( m_pGroup == NULL )
	{
		ESLAfxMessageBox(_T("그룹정보가 선정되지 않았습니다."));
		return -1;
	}
	**
	if( m_pGroup == NULL )
	{
		if( GetAcqType() == PURCHASE )
		{
			ESLAfxMessageBox(_T("차수번호가 선정되지 않았습니다."));
			return -1;		
		}
		else if( GetAcqType() == DONATE )
		{
			ESLAfxMessageBox(_T("접수번호가 선정되지 않았습니다."));
			return -1;
		}
		else
		{
			ESLAfxMessageBox(_T("그룹정보가 선정되지 않았습니다."));
			return -1;
		}
	}
	*******************************************************************
	 *******************************************************************
	
	// 현재 선정된 차수의 상태가 변경되었는지 검사해야 한다.
	if( GetAcqType() == PURCHASE )
	{
		CString strQuery, strValue, strMsg;
		strQuery.Format(
			_T("SELECT MAX(WORKING_STATUS) FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE='%s'"),
			m_pGroup->GetAcqYear(), m_pGroup->GetGroupInfo(), m_pGroup->GetWorkCode());
		GetDM()->GetOneValueQuery(strQuery, strValue);

		if( GetSpeciesType() == BO_PURCHASE && strValue != _T("0") && strValue.IsEmpty() == FALSE ) 
		{
			bError = TRUE;
			strMsg.Format(_T("선정된 차수 [%s]의 차수 상태가 변경되었습니다.\n차수정보를 재설정 해 주십시요."), m_pGroup->GetGroupInfo());
			ESLAfxMessageBox(strMsg);
			GetGroupCollection()->RemoveAt(-1);
		}
		else if( GetSpeciesType() == BO_ORDER && strValue != _T("1") && strValue.IsEmpty() == FALSE ) 
		{
			bError = TRUE;
			strMsg.Format(_T("선정된 차수 [%s]의 차수 상태가 변경되었습니다.\n차수정보를 재설정 해 주십시요."), m_pGroup->GetGroupInfo());
			ESLAfxMessageBox(strMsg);
			GetGroupCollection()->RemoveAt(-1);
		}
		else if((GetSpeciesType() == BO_REGOBJ || GetSpeciesType() == BO_REG_PURCHASE || GetSpeciesType() == BO_MISSING) 
			&& strValue != _T("2") && strValue.IsEmpty() == FALSE )
		{
			bError = TRUE;
			strMsg.Format(_T("선정된 차수 [%s]의 차수 상태가 변경되었습니다.\n차수정보를 재설정 해 주십시요."), m_pGroup->GetGroupInfo());
			ESLAfxMessageBox(strMsg);
			GetGroupCollection()->RemoveAt(-1);
		}

		if( bError == TRUE )
		{
			CStringArray tmpArray;
			m_pGroup = GetGroupCollection()->GetCurSelGroupInfo();
			GetGroupCollection()->GetGroupInfo(&tmpArray, strMsg);
			SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetGroupInfo(), &tmpArray, _T("BP_그룹번호"));
			if( GetAcqType() == PURCHASE )
			{
				CStringArray* pArray = m_pGroup->GetArrayWorkNo();
				SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetWorkNo(-1), pArray, _T("BP_작업번호"));
				SendPurchaseSeqNoChangeMessage();
			}
		}
	}

	if( bError == TRUE )	return -1;
	*/

	return 0;
	
}

INT CDivisionNoInfoDlg::ApplyData()
{
	CString strWorkNo, strMsg;
	if( m_pGroup == NULL )
	{
		if(GetAcqType() == ACQ_TYPE_INVALID)
			strMsg = _T("선정된 그룹정보가 없습니다.");
		else if(GetAcqType() == PURCHASE)
			strMsg = _T("선정된 차수정보가 없습니다.");
		else if(GetAcqType() == DONATE)
			strMsg = _T("선정된 접수번호정보가 없습니다.");
		else 
			strMsg = _T("선정된 그룹번호정보가 없습니다.");
		ESLAfxMessageBox(strMsg);
		return -1;
	}

	GetDM()->SetCellData(_T("BP_수입년도"), m_pGroup->GetAcqYear(), 0);
	GetDM()->SetCellData(_T("BP_그룹번호"), m_pGroup->GetGroupInfo(), 0);

	return 0;
}

INT CDivisionNoInfoDlg::Display(INT idx)
{
	CString strAcqYear, strGroupNo, strGroupName, strWorkNo, strWorkCode, strCurSelInfo;
	CStringArray tmpArray;

	m_pCM->AllControlDisplay(idx);
	
	GetDM()->GetCellData(_T("BP_수입년도"), 0, strAcqYear);
	GetDM()->GetCellData(_T("BP_그룹번호"), 0, strGroupNo);
	GetDM()->GetCellData(_T("BP_작업번호"), 0, strWorkNo);
	GetDM()->GetCellData(_T("BP_차수업무구분"), 0, strWorkCode);
	if( strAcqYear.IsEmpty() == TRUE )		strAcqYear = GetTodayYear();

	//수입년도설정
	m_pCM->SetControlMgrData(_T("BP_수입년도"), strAcqYear);

	if( GetGroupCollection()->GetGroupCount() > 0 )
	{
		GetGroupCollection()->SetCurSelGroupInfo(strAcqYear, strGroupNo, strWorkNo, strWorkCode);
		m_pGroup = GetGroupCollection()->GetCurSelGroupInfo();
		if( m_pGroup )
		{
			GetGroupCollection()->GetGroupInfo(&tmpArray, strCurSelInfo);
			SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetGroupInfo(), &tmpArray, _T("BP_그룹번호"));
			if( GetAcqType() == PURCHASE )
			{
				CStringArray* pArray = m_pGroup->GetArrayWorkNo();
				SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetWorkNo(-1), pArray, _T("BP_작업번호"));
				SendPurchaseSeqNoChangeMessage();
			}
		}
	}
	return 0;
}

INT CDivisionNoInfoDlg::InsertWorkNoInfo(CESL_DataMgr * pDM_OUT)
{
	if		( GetAcqType() == DONATE )			return UpdateWorkSerialNo(pDM_OUT);
	else if	( GetAcqType() == OTHER_ACQ_TYPE )	return UpdateOtherGroupSerialNo(pDM_OUT);

	INT nPNum = -1;
	INT nBNum = -1;
	BOOL bDivNoChange = FALSE;
	BOOL bWorkNoChange = FALSE;
	CString strTmpData, strAcqYear, strDivNo, strWorkNo, strWorkCode;

	GetDM()->GetCellData(_T("BP_수입년도"), 0, strAcqYear);
	GetDM()->GetCellData(_T("BP_차수업무구분"), 0, strWorkCode);
	GetDM()->GetCellData(_T("BP_그룹번호"), 0, strDivNo);
	GetDM()->GetCellData(_T("BP_작업번호"), 0, strWorkNo);
	//if(strWorkNo == _T("0")) strWorkNo = _T("사용안함");

	if( GetOpType() == SINSERT )	bDivNoChange = TRUE;

	//if(strAcqYear != m_pGroup->GetAcqYear())
	if(strAcqYear != GetBookDM()->GetCellData(_T("BB_수입년도"), 0))
	{
		GetDM()->SetCellData(_T("BP_수입년도"), m_pGroup->GetAcqYear(), 0);
		bDivNoChange = TRUE;
		bWorkNoChange = TRUE;
	}
	if(strWorkCode != m_pGroup->GetWorkCode())
	{
		bDivNoChange = TRUE;
		bWorkNoChange = TRUE;
	}
	//if(strDivNo != m_pGroup->GetGroupInfo())
	if(strDivNo != GetBookDM()->GetCellData(_T("BB_그룹번호"), 0))
	{
		GetDM()->SetCellData(_T("BP_그룹번호"), m_pGroup->GetGroupInfo(), 0);
		bDivNoChange = TRUE;
		bWorkNoChange = TRUE;
	}
	if(GetAcqType() == PURCHASE && ( strWorkNo != m_pGroup->GetWorkNo() || GetOpType() == SINSERT))
	{
		if(m_pGroup->GetWorkNo() == _T("새로생성"))
		{
			GetGroupCollection()->InsertWorkNo(pDM_OUT);
			GetDM()->SetCellData(_T("BP_작업번호"), m_pGroup->GetWorkNo(), 0);
			nPNum = m_pGroup->GetLastSerialNo((INT)GetAcqType(), GetDM());
			strTmpData.Format(_T("%d"), ++nPNum);
			GetDM()->SetCellData(_T("BP_발주일련번호"), strTmpData, 0);
			bWorkNoChange = TRUE;
		}
		else 
		{
			GetDM()->SetCellData(_T("BP_작업번호"), m_pGroup->GetWorkNo(), 0);
			nPNum = m_pGroup->GetLastSerialNo((INT)GetAcqType(), GetDM());
			strTmpData.Format(_T("%d"), ++nPNum);
			GetDM()->SetCellData(_T("BP_발주일련번호"), strTmpData, 0);
			bWorkNoChange = TRUE;
		}
	}
	
	if(bWorkNoChange && GetAcqType() == PURCHASE)
	{
		INT ids = GetGroupCollection()->UpDateLastSerialNo(pDM_OUT, nPNum);
		if(ids < 0) return ids;
		m_pGroup->SetLastSerialNo(nPNum);
		strTmpData = pDM_OUT->m_lastmsg;
	}
	
	if((GetSpeciesType() == BO_ORDER || GetSpeciesType() == BO_REG_PURCHASE || GetSpeciesType() == BO_MISSING 
		|| (GetAcqType() == PURCHASE && GetSpeciesType() == BO_REGOBJ)) && bDivNoChange)
	{
		CString strQuery, strWorkCode, strSubQuery;
		strWorkCode = m_pGroup->GetWorkCode();
		if(strWorkCode.IsEmpty()) strSubQuery = _T("");
		else strSubQuery.Format(_T(" AND I.WORK_CODE = '%s' "), strWorkCode);
		strQuery.Format(
			_T("SELECT MAX(P.SERIAL_NO) FROM IDX_BO_TBL I, BO_PURCHASEINFO_TBL P WHERE I.REC_KEY=P.SPECIES_KEY %s AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO=%s AND I.MANAGE_CODE = UDF_MANAGE_CODE"),
//DEL 			_T("SELECT MAX(P.SERIAL_NO) FROM IDX_BO_TBL I, BO_PURCHASEINFO_TBL P WHERE I.REC_KEY=P.SPECIES_KEY %s AND P.ORDER_YEAR='%s' AND P.ORDER_SEQ_NO=%s"),
			strSubQuery,
			GetDM()->GetCellData(_T("BP_수입년도"), 0),
			GetDM()->GetCellData(_T("BP_그룹번호"), 0)
			);
		GetBookDM()->GetOneValueQuery(strQuery, strTmpData);
		nPNum = _ttoi(strTmpData.GetBuffer(0));
		strTmpData.Format(_T("%d"), ++nPNum);
		GetDM()->SetCellData(_T("BP_일련번호"), strTmpData, 0);
	}

	CString strAcqType;
	if(GetAcqType() == PURCHASE) strAcqType = _T("1");
	else if(GetAcqType() == DONATE) strAcqType = _T("2");

	if(GetSpeciesType() == BO_REG_PURCHASE || GetSpeciesType() == BO_REGOBJ || GetSpeciesType() == BO_MISSING)
	{
		CString strQuery, strWorkCode, strSubQuery;
		strWorkCode = m_pGroup->GetWorkCode();
		if(strWorkCode.IsEmpty()) strSubQuery = _T(" AND I.WORK_CODE IS NULL");
		else strSubQuery.Format(_T(" AND I.WORK_CODE='%s' "), strWorkCode);
		strQuery.Format(
			_T("SELECT MAX(B.SERIAL_NO) FROM IDX_BO_TBL I, BO_BOOK_TBL B WHERE I.REC_KEY = B.SPECIES_KEY %s AND B.ACQ_YEAR = '%s' AND B.SEQ_NO = %s AND I.MANAGE_CODE = UDF_MANAGE_CODE"), 
//DEL 			_T("SELECT MAX(B.SERIAL_NO) FROM IDX_BO_TBL I, BO_BOOK_TBL B WHERE I.REC_KEY = B.SPECIES_KEY %s AND B.ACQ_YEAR = '%s' AND B.SEQ_NO = %s"), 
			strSubQuery,
			GetDM()->GetCellData(_T("BP_수입년도"), 0),
			GetDM()->GetCellData(_T("BP_그룹번호"), 0)
			);
		GetBookDM()->GetOneValueQuery(strQuery, strTmpData);
		nBNum = _ttoi(strTmpData.GetBuffer(0));
	}
	
	//책DM을 권차-복본기호 순으로 Sort해야 한다.
	INT nCondColumns = 0;
	CString strAlias[2];
	INT nCond[2];
	strAlias[nCondColumns] = _T("BB_권일련번호");	nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_복본기호");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	GetBookDM()->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	
	INT nRowCnt = GetBookDM()->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = GetBookDM()->GetCellData(_T("BB_DB_FLAG"), i);
		if(strTmpData == _T("D")) continue;

		if(GetOpType() == SINSERT)
		{
			if(strTmpData != _T("I")) GetBookDM()->SetCellData(_T("BB_DB_FLAG"), _T("I"), i);
			GetBookDM()->SetCellData(_T("BB_수입구분_코드"), strAcqType, i);
			GetBookDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
			GetBookDM()->SetCellData(_T("BB_그룹번호"), m_pGroup->GetGroupInfo(), i);
			if(GetSpeciesType() == BO_REG_PURCHASE || GetSpeciesType() == BO_REGOBJ || GetSpeciesType() == BO_MISSING)
			{
				strTmpData.Format(_T("%d"), ++nBNum);
				GetBookDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
			}
			else
			{
				strTmpData.Format(_T("%d"), nPNum);
				GetBookDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
			}
		}
		else if(GetOpType() != SINSERT)
		{
			if(strTmpData == _T("I"))
			{
				GetBookDM()->SetCellData(_T("BB_수입구분_코드"), strAcqType, i);
				GetBookDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
				GetBookDM()->SetCellData(_T("BB_그룹번호"), m_pGroup->GetGroupInfo(), i);
				if(GetSpeciesType() == BO_REG_PURCHASE || GetSpeciesType() == BO_REGOBJ || GetSpeciesType() == BO_MISSING)
				{
					strTmpData.Format(_T("%d"), ++nBNum);
					GetBookDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
				}				
				else
				{
					strTmpData.Format(_T("%d"), nPNum);
					GetBookDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
				}
			}
			else if(strTmpData == _T("U"))
			{
				GetBookDM()->SetCellData(_T("BB_수입구분_코드"), strAcqType, i);
				GetBookDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
				GetBookDM()->SetCellData(_T("BB_그룹번호"), m_pGroup->GetGroupInfo(), i);
				if((GetSpeciesType() == BO_REG_PURCHASE || GetSpeciesType() == BO_REGOBJ || GetSpeciesType() == BO_MISSING) && bDivNoChange)
				{
					strTmpData.Format(_T("%d"), ++nBNum);
					GetBookDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
				}
			}
		}
	}

	GetAppendixDM()->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	nRowCnt = GetAppendixDM()->GetRowCount();
	for(i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = GetAppendixDM()->GetCellData(_T("BB_DB_FLAG"), i);
		if(strTmpData == _T("D")) continue;
		
		if(GetOpType() == SINSERT)
		{
			if(strTmpData != _T("I")) GetAppendixDM()->SetCellData(_T("BB_DB_FLAG"), _T("I"), i);
			GetAppendixDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
			GetAppendixDM()->SetCellData(_T("BB_그룹번호"), m_pGroup->GetGroupInfo(), i);
			if(GetSpeciesType() == BO_REG_PURCHASE || GetSpeciesType() == BO_REGOBJ || GetSpeciesType() == BO_MISSING)
			{
				strTmpData.Format(_T("%d"), ++nBNum);
				GetAppendixDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
			}
			else
			{
				strTmpData.Format(_T("%d"), nPNum);
				GetAppendixDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
			}
		}
		else if(GetOpType() != SINSERT)
		{
			if(strTmpData == _T("I"))
			{
				GetAppendixDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
				GetAppendixDM()->SetCellData(_T("BB_그룹번호"), m_pGroup->GetGroupInfo(), i);
				if(GetSpeciesType() == BO_REG_PURCHASE || GetSpeciesType() == BO_REGOBJ || GetSpeciesType() == BO_MISSING)
				{
					strTmpData.Format(_T("%d"), ++nBNum);
					GetAppendixDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
				}				
				else
				{
					strTmpData.Format(_T("%d"), nPNum);
					GetAppendixDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
				}
			}
			else if(strTmpData == _T("U"))
			{
				GetAppendixDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
				GetAppendixDM()->SetCellData(_T("BB_그룹번호"), m_pGroup->GetGroupInfo(), i);
				if((GetSpeciesType() == BO_REG_PURCHASE || GetSpeciesType() == BO_REGOBJ || GetSpeciesType() == BO_MISSING) && bDivNoChange)
				{
					strTmpData.Format(_T("%d"), ++nBNum);
					GetAppendixDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
				}
			}
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// [ 미니스펙 ]
/////////////////////////////////////////////////////////////////////////////
INT CDivisionNoInfoDlg::UpdateWorkSerialNo(CESL_DataMgr * pDM_OUT)
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT nNUM, nRowCnt;
	BOOL bDivNoChange = FALSE;
	CString strAcqYear, strDivNo, strWorkNo, strLastSerialNo, strTmpData, strDBFLAG, strAcqType;

	//=======================================================================
	// 1. DM에서 수입년도, 그룹번호, 발주일련번호를 가져온다.
	//=======================================================================
	GetDM()->GetCellData(_T("BP_수입년도"), 0, strAcqYear);
	GetDM()->GetCellData(_T("BP_그룹번호"), 0, strDivNo);
	GetDM()->GetCellData(_T("BP_발주일련번호"), 0, strLastSerialNo);

	//rowCnt = GetBookDM()->GetRowCount();
	//=======================================================================
	// 2. 그룹정보에서 마지막일련번호값을 가져온다.(그룹정보가 없다면 0으로 설정)
	//=======================================================================
	if( m_pGroup )	nNUM = m_pGroup->GetLastSerialNo();
	else			nNUM = 0;
	
	//=======================================================================
	// 3. 수서타입은 2로 설정 (기증)
	//=======================================================================
	strAcqType = _T("2");
	//if(strAcqYear != m_pGroup->GetAcqYear())
	if( strAcqYear != GetBookDM()->GetCellData(_T("BB_수입년도"), 0) )
	{
		GetDM()->SetCellData(_T("BP_수입년도"), m_pGroup->GetAcqYear(), 0);
		bDivNoChange = TRUE;
	}
	//if(strDivNo != m_pGroup->GetGroupInfo())
	if(strDivNo != GetBookDM()->GetCellData(_T("BB_그룹번호"), 0))
	{
		GetDM()->SetCellData(_T("BP_그룹번호"), m_pGroup->GetGroupInfo(), 0);
		bDivNoChange = TRUE;
	}
	
	//책DM을 권차-복본기호 순으로 Sort해야 한다.
	INT nCondColumns = 0;
	CString strAlias[2];
	INT nCond[2];
	strAlias[nCondColumns] = _T("BB_권일련번호");					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_복본기호");						nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	GetBookDM()->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);

	nRowCnt = GetBookDM()->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = GetBookDM()->GetCellData(_T("BB_DB_FLAG"), i);
		if(strTmpData == _T("D")) continue;

		if(GetOpType() == SINSERT)
		{
			if(strTmpData != _T("I")) GetBookDM()->SetCellData(_T("BB_DB_FLAG"), _T("I"), i);
			GetBookDM()->SetCellData(_T("BB_수입구분_코드"), strAcqType, i);
			GetBookDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
			GetBookDM()->SetCellData(_T("BB_그룹번호"), m_pGroup->GetGroupInfo(), i);

			strTmpData.Format(_T("%d"), ++nNUM);
			GetBookDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
		}
		else if(GetOpType() != SINSERT)
		{
			if(strTmpData == _T("I"))
			{
				GetBookDM()->SetCellData(_T("BB_수입구분_코드"), strAcqType, i);
				GetBookDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
				GetBookDM()->SetCellData(_T("BB_그룹번호"), m_pGroup->GetGroupInfo(), i);
				strTmpData.Format(_T("%d"), ++nNUM);
				GetBookDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
			}
			else if(strTmpData == _T("U") && bDivNoChange)
			{
				GetBookDM()->SetCellData(_T("BB_수입구분_코드"), strAcqType, i);
				GetBookDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
				GetBookDM()->SetCellData(_T("BB_그룹번호"), m_pGroup->GetGroupInfo(), i);
				strTmpData.Format(_T("%d"), ++nNUM);
				GetBookDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
			}
		}
	}

	GetAppendixDM()->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	nRowCnt = GetAppendixDM()->GetRowCount();
	for(i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = GetAppendixDM()->GetCellData(_T("BB_DB_FLAG"), i);
		if(strTmpData == _T("D")) continue;
		
		if(GetOpType() == SINSERT)
		{
			if(strTmpData != _T("I")) GetAppendixDM()->SetCellData(_T("BB_DB_FLAG"), _T("I"), i);
			GetAppendixDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
			GetAppendixDM()->SetCellData(_T("BB_그룹번호"), m_pGroup->GetGroupInfo(), i);
			
			strTmpData.Format(_T("%d"), ++nNUM);
			GetAppendixDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
		}
		else if(GetOpType() != SINSERT)
		{
			if(strTmpData == _T("I"))
			{
				GetAppendixDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
				GetAppendixDM()->SetCellData(_T("BB_그룹번호"), m_pGroup->GetGroupInfo(), i);
				strTmpData.Format(_T("%d"), ++nNUM);
				GetAppendixDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
			}
			else if(strTmpData == _T("U") && bDivNoChange)
			{
				GetAppendixDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
				GetAppendixDM()->SetCellData(_T("BB_그룹번호"), m_pGroup->GetGroupInfo(), i);
				strTmpData.Format(_T("%d"), ++nNUM);
				GetAppendixDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
			}
		}
	}

	if(m_pGroup)
	{
		m_pGroup->SetLastSerialNo(nNUM);
		m_pGroup->UpdateLastSerialNo(strAcqType, this, pDM_OUT);
	}

	return 0;
}

HBRUSH CDivisionNoInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;

}

VOID CDivisionNoInfoDlg::SendPurchaseSeqNoChangeMessage()
{
	CString strWorkCode;

	if( m_pGroup == NULL )	return;
	
	strWorkCode = m_pGroup->GetWorkCode();
	::PostMessage(pParentMgr->GetSafeHwnd(), PURCHASE_SEQ_NO_CHANGE, _ttoi(strWorkCode.GetBuffer(0)), 0);
}

VOID CDivisionNoInfoDlg::OnChangeWorkCode(LPARAM lParam, WPARAM wParam)
{
	GetGroupCollection()->RemoveAll();
	CString strWorkCode;
	strWorkCode.Format(_T("%d"), lParam);
	if( strWorkCode == _T("0") )	strWorkCode = _T("");

	GetDM()->SetCellData(_T("BP_차수업무구분"), strWorkCode, 0);
	GetDM()->SetCellData(_T("BP_그룹번호"), _T(""), 0);
	MakeGroupNoList();
}


BOOL CDivisionNoInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			return TRUE;
		else if(pMsg->wParam == VK_TAB)
		{
			if(pMsg->hwnd == GetDlgItem(IDC_btnBSDIVNO_CREATE)->GetSafeHwnd())
			{
				pParentMgr->PostMessage(SET_TAB_ORDER, 0, 0);
				return TRUE;
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_cmbBSDIVNO_ACQ_CODE)->GetSafeHwnd() && !GetDlgItem(IDC_btnBSDIVNO_CREATE)->IsWindowVisible())
			{
				pParentMgr->PostMessage(SET_TAB_ORDER, 0, 0);
				return TRUE;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

INT CDivisionNoInfoDlg::UpdateOtherGroupSerialNo(CESL_DataMgr *pDM_OUT)
{
	if(pDM_OUT == NULL) return -1;

	CString strAcqYear, strDivNo, strWorkNo, strLastSerialNo;
	GetDM()->GetCellData(_T("BP_수입년도"), 0, strAcqYear);
	GetDM()->GetCellData(_T("BP_그룹번호"), 0, strDivNo);
	
	INT cnt = 0;
	INT rowCnt = GetBookDM()->GetRowCount();
	INT nNUM = 0;
	if(m_pGroup) nNUM = m_pGroup->GetLastSerialNo();

	CString strTmpData, strDBFLAG, strAcqType;
	strAcqType = GetGroupCollection()->GetAcqCode();
	BOOL bDivNoChange = FALSE;
	
	//if(strAcqYear != m_pGroup->GetAcqYear())
	if(strAcqYear != GetBookDM()->GetCellData(_T("BB_수입년도"), 0))
	{
		GetDM()->SetCellData(_T("BP_수입년도"), m_pGroup->GetAcqYear(), 0);
		bDivNoChange = TRUE;
	}
	//if(strDivNo != m_pGroup->GetGroupInfo())
	if(strDivNo != GetBookDM()->GetCellData(_T("BB_그룹번호"), 0))
	{
		GetDM()->SetCellData(_T("BP_그룹번호"), m_pGroup->GetGroupInfo(), 0);
		bDivNoChange = TRUE;
	}

	strDivNo = m_pGroup->GetGroupInfo();
	if(strDivNo == _T("적용안함")) strDivNo.Empty();
	
	//책DM을 권차-복본기호 순으로 Sort해야 한다.
	INT nCondColumns = 0;
	CString strAlias[2];
	INT nCond[2];
	strAlias[nCondColumns] = _T("BB_권일련번호");					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_복본기호");						nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	GetBookDM()->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);

	INT nRowCnt = GetBookDM()->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = GetBookDM()->GetCellData(_T("BB_DB_FLAG"), i);
		if(strTmpData == _T("D")) continue;

		if(GetOpType() == SINSERT)
		{
			if(strTmpData != _T("I")) GetBookDM()->SetCellData(_T("BB_DB_FLAG"), _T("I"), i);
			GetBookDM()->SetCellData(_T("BB_수입구분_코드"), strAcqType, i);
			GetBookDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
			GetBookDM()->SetCellData(_T("BB_그룹번호"), strDivNo, i);

			strTmpData.Format(_T("%d"), ++nNUM);
			GetBookDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
		}
		else if(GetOpType() != SINSERT)
		{
			if(strTmpData == _T("I"))
			{
				GetBookDM()->SetCellData(_T("BB_수입구분_코드"), strAcqType, i);
				GetBookDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
				GetBookDM()->SetCellData(_T("BB_그룹번호"), strDivNo, i);
				strTmpData.Format(_T("%d"), ++nNUM);
				GetBookDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
			}
			else if(strTmpData == _T("U") && bDivNoChange)
			{
				GetBookDM()->SetCellData(_T("BB_수입구분_코드"), strAcqType, i);
				GetBookDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
				GetBookDM()->SetCellData(_T("BB_그룹번호"), strDivNo, i);
				strTmpData.Format(_T("%d"), ++nNUM);
				GetBookDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
			}
		}
	}

	GetAppendixDM()->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	nRowCnt = GetAppendixDM()->GetRowCount();
	for(i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = GetAppendixDM()->GetCellData(_T("BB_DB_FLAG"), i);
		if(strTmpData == _T("D")) continue;
		
		if(GetOpType() == SINSERT)
		{
			if(strTmpData != _T("I")) GetAppendixDM()->SetCellData(_T("BB_DB_FLAG"), _T("I"), i);
			GetAppendixDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
			GetAppendixDM()->SetCellData(_T("BB_그룹번호"), strDivNo, i);
			
			strTmpData.Format(_T("%d"), ++nNUM);
			GetAppendixDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
		}
		else if(GetOpType() != SINSERT)
		{
			if(strTmpData == _T("I"))
			{
				GetAppendixDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
				GetAppendixDM()->SetCellData(_T("BB_그룹번호"), strDivNo, i);
				strTmpData.Format(_T("%d"), ++nNUM);
				GetAppendixDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
			}
			else if(strTmpData == _T("U") && bDivNoChange)
			{
				GetAppendixDM()->SetCellData(_T("BB_수입년도"), m_pGroup->GetAcqYear(), i);
				GetAppendixDM()->SetCellData(_T("BB_그룹번호"), strDivNo, i);
				strTmpData.Format(_T("%d"), ++nNUM);
				GetAppendixDM()->SetCellData(_T("BB_일련번호"), strTmpData, i);
			}
		}
	}

	if(m_pGroup)
	{
		m_pGroup->SetLastSerialNo(nNUM);
		m_pGroup->UpdateLastSerialNo(strAcqType, this, pDM_OUT);
	}

	return 0;
}

CGroupInfo * CDivisionNoInfoDlg::GetGroupInfo()
{
	return m_pGroup;
}

UINT CDivisionNoInfoDlg::OnSetTabOrder(LPARAM lParam, WPARAM wParam)
{
	if(GetDlgItem(IDC_cmbBSDIVNO_ACQ_CODE)->IsWindowEnabled())
		GetDlgItem(IDC_cmbBSDIVNO_ACQ_CODE)->SetFocus();
	else
		GetDlgItem(IDC_edtBSDIVNO_ACQ_YEAR)->SetFocus();
	
	return 0;
}

