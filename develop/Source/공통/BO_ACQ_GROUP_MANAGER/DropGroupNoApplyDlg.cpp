// DropGroupNoApplyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DropGroupNoApplyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDropGroupNoApplyDlg dialog


CDropGroupNoApplyDlg::CDropGroupNoApplyDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDropGroupNoApplyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDropGroupNoApplyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_nGroupType = DIVNO;
	m_arrIdx.RemoveAll();
	m_nSelectGroupNo = -1;

	m_nLastNum = -1;
}

CDropGroupNoApplyDlg::~CDropGroupNoApplyDlg()
{
}

VOID CDropGroupNoApplyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDropGroupNoApplyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDropGroupNoApplyDlg, CDialog)
	//{{AFX_MSG_MAP(CDropGroupNoApplyDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDropGroupNoApplyDlg message handlers

BOOL CDropGroupNoApplyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_누락그룹번호적용")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_누락그룹번호적용"));
	m_pDM = FindDM(_T("DM_누락그룹번호적용"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->SetSort(FALSE);
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("누락번호 적용 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	m_pGrid->Display();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDropGroupNoApplyDlg::OnOK() 
{
	// TODO: Add extra validation here
	INT ids = m_pGrid->SelectMakeList();
	if(ids<0)
	{
		ESLAfxMessageBox(_T("선정자료를 구축할 수 없습니다."));
		return ;
	}
	INT i = m_pGrid->SelectGetHeadPosition();
	if(i < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	m_nSelectGroupNo = m_arrIdx.GetAt(i);
	CDialog::OnOK();
}

BOOL CDropGroupNoApplyDlg::Init()
{
	if(m_nGroupType == RECEIPT_NO)
	{
		GetDlgItem(IDC_strDROP_WORK_CODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtDROP_WORK_CODE)->ShowWindow(SW_HIDE);
		m_pCM->SetControlMgrData(_T("종류"), _T("접수번호"));
		m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);
	}
	else if(m_nGroupType == DIVNO)
	{
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("업무구분"), m_strWorkCode, m_strWorkCode);
		m_pCM->SetControlMgrData(_T("종류"), _T("차수번호"));
		m_pCM->SetControlMgrData(_T("업무구분"), m_strWorkCode);
		m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);
	}
	else if(m_nGroupType == OTHER_GROUP_NO)
	{
		if(m_strAcqCode.IsEmpty()) return FALSE;
	
		CString strType, strAcqCodeDesc;
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("입수구분"), m_strAcqCode, strAcqCodeDesc);
		strType.Format(_T("그룹번호:%s"), strAcqCodeDesc);
		GetDlgItem(IDC_strDROP_WORK_CODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtDROP_WORK_CODE)->ShowWindow(SW_HIDE);
		m_pCM->SetControlMgrData(_T("종류"), strType);
		m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);		
	}
	
	INT ids = MakeDM();
	if(ids != 0) return FALSE;
	return TRUE;
}


INT CDropGroupNoApplyDlg::MakeDM()
{
	CString strQuery, strTmpData, strWorkCode;
	m_pInfo->pCodeMgr->ConvertDescToCode(_T("업무구분"), m_strWorkCode, strWorkCode);
	if(m_nGroupType == DIVNO)
	{
		if(strWorkCode.IsEmpty())
			strQuery.Format(
				_T("SELECT DISTINCT(SEQ_NO) FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND WORK_CODE IS NULL ORDER BY SEQ_NO DESC"),
				m_strAcqYear);
		else
			strQuery.Format(
				_T("SELECT DISTINCT(SEQ_NO) FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND WORK_CODE='%s' ORDER BY SEQ_NO DESC"),
				m_strAcqYear, strWorkCode);
	}
	else if(m_nGroupType == RECEIPT_NO)
	{
		strQuery.Format(
			_T("SELECT DISTINCT(RECEIPT_NO) FROM BO_DON_RECEIPT_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' ORDER BY RECEIPT_NO DESC"),
			m_strAcqYear);
	}
	else if(m_nGroupType == OTHER_GROUP_NO)
	{
		strQuery.Format(
			_T("SELECT DISTINCT(GROUP_SEQ) FROM BO_ACQ_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_CODE = '%s' AND CREATE_YEAR='%s' ORDER BY GROUP_SEQ DESC"),
			m_strAcqCode, m_strAcqYear);	
	}
	
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return ids;

	for(INT i=0 ; i<m_nLastNum-1 ; i++)
	{
		m_arrIdx.Add(i+1);
	}
	INT nTmpData;
	INT nRowCnt = tmpDM.GetRowCount();

	for(i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = tmpDM.GetCellData(i, 0);
		if(strTmpData.IsEmpty()) continue;
		nTmpData = _ttoi(strTmpData.GetBuffer(0));
		if(nTmpData >= 1 && nTmpData <= m_arrIdx.GetSize())
		{
			m_arrIdx.RemoveAt(nTmpData-1);
		}
	}

	m_pDM->FreeData();
	nRowCnt = m_arrIdx.GetSize();
	if(nRowCnt == 0)
	{
		ESLAfxMessageBox(_T("누락된 번호가 없습니다."));
		return 1000;
	}
	for(i=0 ; i<nRowCnt ; i++)
	{
		m_pDM->InsertRow(-1);
		nTmpData = m_pDM->GetRowCount()-1;
		strTmpData.Format(_T("%d"), m_arrIdx.GetAt(i));
		m_pDM->SetCellData(_T("누락번호"), strTmpData, nTmpData);
	}

	return 0;
}

HBRUSH CDropGroupNoApplyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
