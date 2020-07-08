// DivisionNo_Select_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "DivisionNo_Select_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_Select_Dlg dialog


CDivisionNo_Select_Dlg::CDivisionNo_Select_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDivisionNo_Select_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDivisionNo_Select_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pSourceDM = NULL;
	m_pSourceGrid = NULL;
	m_pGroupCollection = NULL;
	m_pGroup = NULL;
	m_strWorkStatus = _T("");
	m_strAcqYear = _T("");
	m_bCreateDM = FALSE;
	m_bAllowNULLGroup = FALSE;
}

CDivisionNo_Select_Dlg::~CDivisionNo_Select_Dlg()
{
	if(m_pGroupCollection)
	{
		delete m_pGroupCollection;
		m_pGroupCollection = NULL;
	}

	if(m_bCreateDM)
	{
		if(m_pDM)
		{
			delete m_pDM;
			m_pDM = NULL;
		}
	}
}

VOID CDivisionNo_Select_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDivisionNo_Select_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDivisionNo_Select_Dlg, CDialog)
	//{{AFX_MSG_MAP(CDivisionNo_Select_Dlg)
	ON_BN_CLICKED(IDC_btnDIVNO_SEARCH, OnbtnDIVNOSEARCH)
	ON_EN_KILLFOCUS(IDC_edtDIVNO, OnKillfocusedtDIVNO)
	ON_EN_KILLFOCUS(IDC_edtACQ_YEAR, OnKillfocusedtACQYEAR)
	ON_CBN_SELCHANGE(IDC_cmbDIVNO_WORKNO, OnSelchangecmbDIVNOWORKNO)
	ON_BN_CLICKED(IDC_btnDIVNO_CREATE, OnbtnDIVNOCREATE)
	ON_CBN_SELCHANGE(IDC_cmbDIVNO_WORK_CODE, OnSelchangecmbDIVNOWORKCODE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDivisionNo_Select_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CGroupMgr mgr(this);

	if( InitESL_Mgr(_T("K2UPGRADE_구입차수_선택")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_구입차수_선택"));
	m_pDM = FindDM(_T("DM_구입차수관리"));

	if( m_pCM == NULL )
	{
		ESLAfxMessageBox( _T("[CM_구입차수_선택]을 설정할 수 없습니다.") );
		return FALSE;
	}

	
	if( m_pDM == NULL )
	{
		m_pDM = new CESL_DataMgr;
		m_pDM->SetCONNECTION_INFO(this->CONNECTION_INFO);
		m_pDM->InitDMPKFromFile(_T("DM_구입차수관리"));
		m_pDM->SetMarcMgr(this->m_pInfo->pMarcMgr);	
		m_bCreateDM = TRUE;
	}
	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	if( m_strWorkStatus.IsEmpty() == TRUE )		m_strWorkStatus = _T("구입대상");	
	if( m_strAcqYear.IsEmpty() == TRUE )		m_strAcqYear = GetTodayYear();
	m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);

	m_pGroupCollection = new CGroupCollection(this);
	m_pGroupCollection->SetAcqType(PURCHASE);
	m_pGroupCollection->SetWorkingStatus(m_strWorkStatus);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  
}

VOID CDivisionNo_Select_Dlg::OnOK() 
{
	INT ids;
	CString strData;

	if( m_pGroup == NULL && m_bAllowNULLGroup == FALSE )
	{
		ESLAfxMessageBox(_T("선정하신 차수정보가 올바르지 않습니다. 다시 확인해 주십시요."));
		return;
	}

	if( m_pSourceDM != NULL	&& m_pSourceGrid != NULL )
	{
		ids = m_pSourceGrid->SelectMakeList();
		if( ids < 0 )	return ;
		ids = m_pSourceGrid->SelectGetHeadPosition();
		if( ids < 0 )	return ;

		while( ids >= 0 )
		{
			strData = m_pSourceDM->GetCellData(_T("IBS_업무구분"), ids);
			if( strData != m_pGroup->GetWorkCode() )
			{
				if( IDNO == ESLAfxMessageBox(_T("선정하신 자료의 업무구분값이 동일하지 않습니다.\n\n해당 차수번호의 업무구분값으로 일괄적용 하시겠습니까?"), MB_YESNO | MB_ICONQUESTION) )	
				{
					return;
				}

				break;
			}

			ids = m_pSourceGrid->SelectGetNext();
		}
	}

	/**/
	
	if( m_pGroup != NULL )
	{
		if(m_pGroup->GetWorkNo() == _T("사용안함")) m_pGroup->SetGroupNo(_T("0"));
		if(m_pGroup->GetWorkNo() == _T("새로생성"))
		{
			m_pDM->StartFrame();
			m_pGroupCollection->InsertWorkNo(m_pDM);
			INT ids = m_pDM->SendFrame();
			if(ids < 0) return ;
			ids = m_pDM->EndFrame();
			if(ids < 0) return ;
		}
	}

	CDialog::OnOK();
}

VOID CDivisionNo_Select_Dlg::OnbtnDIVNOSEARCH() 
{
	INT pos ;
	CString strGroupInfo, strWorkCode, strWorkNo ;
	
	CDivisionNo_List_Dlg dlg(this);

	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);

	dlg.SetAcquisitYear(m_strAcqYear);
	dlg.SetWorkStatus(m_strWorkStatus);
	dlg.SetWorkCode(strWorkCode);
	dlg.SetGroupCollection(m_pGroupCollection);

	if(IDOK == dlg.DoModal())		strGroupInfo = dlg.GetSelDivNo();
	else							return ;
	if( strGroupInfo.IsEmpty() == TRUE ) return;
	
	pos = strGroupInfo.Find(_T(","), 0);
	if(pos >= 0)
	{
		strGroupInfo = strGroupInfo.Mid(0, pos);
		strGroupInfo.TrimLeft();	strGroupInfo.TrimRight();
	}

	m_pCM->SetControlMgrData(_T("차수번호"), strGroupInfo);

	strWorkCode = m_pGroupCollection->GetWorkCode() ;

	m_pCM->SetControlMgrData( _T("업무구분"), strWorkCode ) ;
	
	m_pCM->GetControlMgrData( _T("작업번호"), strWorkNo );
	m_pGroupCollection->SetCurSelGroupInfo(m_strAcqYear, strGroupInfo, strWorkNo, strWorkCode );

	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
	SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), m_pGroup->GetArrayWorkNo(), _T("작업번호"));
}

CString CDivisionNo_Select_Dlg::GetTodayYear(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;
}

VOID CDivisionNo_Select_Dlg::OnKillfocusedtDIVNO() 
{
	INT pos;
	CString strGroupNo, strWorkCode;

	m_pCM->GetControlMgrData(_T("차수번호"), strGroupNo);
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	if( strGroupNo.IsEmpty() == TRUE )	return;

	pos = strGroupNo.Find(_T(","), 0);
	if( pos >= 0 )
	{
		strGroupNo = strGroupNo.Mid(0, pos);
		strGroupNo.TrimLeft();	strGroupNo.TrimRight();
	}
	
	INT ids;
	ids = m_pGroupCollection->CheckAcqNo(m_strAcqYear, strGroupNo, strWorkCode);
	if( ids < 0 )
	{
		m_pCM->SetControlMgrData(_T("차수번호"), _T(""));
		ESLAfxMessageBox(_T("입력하신 차수번호가 올바르지 않습니다."));
		return;
	}
	
	m_pGroupCollection->RemoveAll();
	m_pGroupCollection->AddGroupInfo(m_strAcqYear, strGroupNo, strWorkCode);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
	SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), m_pGroup->GetArrayWorkNo(), _T("작업번호"));
}

INT CDivisionNo_Select_Dlg::SetSingleComboBoxStrData(CString strCMAlias, CString strInitVal, CStringArray * parrGroupNo, CString strCtrlAlias)
{
	INT i, cnt, nInitVal = 0;
	CString strTmpData;
	CESL_ControlMgr *pCM = FindCM(strCMAlias);
	CESL_ControlComboBox *pCMB = (CESL_ControlComboBox *)pCM->FindControl(strCtrlAlias);
	pCMB->ResetContent();
	
	cnt = parrGroupNo->GetSize();	
	for( i=0 ; i<cnt ; i++ )
	{
		strTmpData = parrGroupNo->GetAt(i);
		if(strInitVal == strTmpData) nInitVal = i;

		pCMB->AddString(strTmpData);
	}
	pCMB->SetCurSel(nInitVal);

	return 0;
}

INT CDivisionNo_Select_Dlg::GetSingleComboBoxSelectData(CString strCMAlias, CString strCtrlAlias, CStringArray * parrGroupNo, CString &strSelData)
{
	CESL_ControlMgr *pCM = FindCM(strCMAlias);
	CESL_ControlComboBox *pCMB = (CESL_ControlComboBox *)pCM->FindControl(strCtrlAlias);
	INT nCurSel = pCMB->GetCurSel();
	if(nCurSel >= 0) strSelData = parrGroupNo->GetAt(nCurSel);
	return 0;
}

VOID CDivisionNo_Select_Dlg::OnKillfocusedtACQYEAR() 
{
	// TODO: Add your control notification handler code here
	m_pCM->GetControlMgrData(_T("수입년도"), m_strAcqYear);
	OnKillfocusedtDIVNO();
}

VOID CDivisionNo_Select_Dlg::OnSelchangecmbDIVNOWORKNO() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("작업번호"));
	INT nCurSel = pCmb->GetCurSel();
	m_pGroupCollection->SetCurSelWorkNo(nCurSel);
}

VOID CDivisionNo_Select_Dlg::OnbtnDIVNOCREATE() 
{
	// TODO: Add your control notification handler code here
	CString strAcqYear, strWorkStatus, strWorkCode;
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	if(m_strWorkStatus.IsEmpty()) m_strWorkStatus = _T("구입대상");
	CESL_DataMgr * pWorkNoDM = FindDM(_T("DM_작업번호관리"));
	m_pDM->FreeData();

	m_pDM->TBL_NAME = _T("BO_PURCHASE_SEQ_NO_TBL S");

	CDivisionNo_Insert_Dlg dlg(this);
	dlg.SetOpenMode(DINSERT);
	dlg.SetDivNoDMPtr(m_pDM);
	dlg.SetWorkNoDMPtr(pWorkNoDM);
	dlg.SetAcqYear(strAcqYear);
	dlg.SetWorkStatus(m_strWorkStatus);
	dlg.SetWorkCode(strWorkCode);
	dlg.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	if(IDOK == dlg.DoModal())
	{
		m_pDM->TBL_NAME = _T("BO_PURCHASE_SEQ_NO_TBL S, CO_BUDGET_EXECUTION_TBL B");
		m_pGroupCollection->RemoveAll();
		m_pGroupCollection->AddGroupInfo(m_pDM->GetCellData(_T("수입년도"), 0), 
			                             m_pDM->GetCellData(_T("차수번호"), 0), 
										 m_pDM->GetCellData(_T("업무구분"), 0));
		m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
		m_pCM->SetControlMgrData(_T("차수번호"), m_pGroup->GetGroupInfo());
		SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), m_pGroup->GetArrayWorkNo(), _T("작업번호"));
	}
	else
	{
		m_pDM->TBL_NAME = _T("BO_PURCHASE_SEQ_NO_TBL S, CO_BUDGET_EXECUTION_TBL B");
	}
}

CGroupInfo * CDivisionNo_Select_Dlg::GetSelGroup()
{
	return m_pGroupCollection->GetCurSelGroupInfo();
}

VOID CDivisionNo_Select_Dlg::OnSelchangecmbDIVNOWORKCODE() 
{
	m_pCM->GetControlMgrData(_T("수입년도"), m_strAcqYear);

	m_pCM->SetControlMgrData(_T("차수번호"), _T(""));
	
	OnKillfocusedtDIVNO();
	
}

BOOL CDivisionNo_Select_Dlg::PreTranslateMessage(MSG *pMsg)
{
	if (pMsg->message == WM_CHAR)
    {
		INT nVirtKey = (INT) pMsg->wParam;
		if ((nVirtKey < '0' || nVirtKey > '9') && nVirtKey != 8)
		{
			ESLAfxMessageBox(_T("숫자만 입력하실 수 있습니다.")); 
            return TRUE;
		}
    }
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CDivisionNo_Select_Dlg::OnCancel() 
{
	CDialog::OnCancel();
}

HBRUSH CDivisionNo_Select_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{ 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
