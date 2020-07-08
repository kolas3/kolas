#include "stdafx.h"
#include "UserCardHistoryDlg.h"
#include "Kolas2up_MobileApi.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUserCardHistoryDlg::CUserCardHistoryDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_nMode = -1;
	m_strCivilNoHideYN = _T("N");
	m_sIsUnityLoanService = _T("N");
}

CUserCardHistoryDlg::~CUserCardHistoryDlg()
{
}

void CUserCardHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CUserCardHistoryDlg)
	//}}AFX_DATA_MAP	
	
}


BEGIN_MESSAGE_MAP(CUserCardHistoryDlg, CDialog)
	//{{AFX_MSG_MAP(CUserCardHistoryDlg)
	ON_BN_CLICKED(IDC_btnRegister, OnbtnRegister)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CUserCardHistoryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd());

	if(0>InitESL_Mgr(_T("SM_USER_CARD_HISTORY")))
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	if (m_nMode == 0 || 
		m_nMode == 2 || 
		m_nMode == 4 || 
		m_nMode == 5 || 
		m_nMode == 6 )  
	{
		GetDlgItem(IDC_btnRegister)->ShowWindow(FALSE);
	}

	ViewCardHistory();
		
	return TRUE;  
	              
}

VOID CUserCardHistoryDlg::ViewCardHistory()
{	
	CString strText = m_strUserName + _T("님의 회원증 발급이력입니다");
	SetDlgItemText(IDC_staUserCardHistory, strText);

	CString strWhere;
	strWhere.Format(_T(" USER_KEY=%s ") , m_strUserKey);
	CESL_DataMgr* pDM = FindDM(_T("DM_USER_CARD_HISTORY"));
	pDM->RefreshDataManager(strWhere);

	
	CString strGetData = _T("");	
	for(INT i = 0; i < pDM->GetRowCount(); i++)
	{
		pDM->GetCellData(_T("대출자번호"), i, strGetData);
		if(_T("Y") != m_strCivilNoHideYN)
		{
			CLocCommonAPI::ChangeSecuriyUserNo(this, strGetData, m_sIsUnityLoanService);
		}
		pDM->SetCellData(_T("대출자번호_VIEW"), strGetData, i);
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_USER_CARD_HISTORY"), _T("grid"));	
	pGrid->Display();
}

void CUserCardHistoryDlg::OnbtnRegister() 
{
	CardHistoryRegister();
}

VOID CUserCardHistoryDlg::CardHistoryRegister()
{
	
	CString strTemp;
	CESL_DataMgr *pDM = FindDM(_T("DM_USER_CARD_HISTORY"));
	INT nRowCnt = pDM->GetRowCount();
	for(INT i=0; i<nRowCnt; i++)
	{
		strTemp = pDM->GetCellData(_T("대출자번호"),i);
		if(m_strUserNo==strTemp)
		{
			AfxMessageBox(_T("이미 등록하신 회원증입니다."));
			return;
		}
	}

	
	CString strLibCode, strLibName, strReissueDate, strWorkerID, strWorkerName, strFirstWork;
	strLibCode = this->m_pInfo->LIB_CODE;
	strLibName = this->m_pInfo->LIB_NAME;

	CTime t = CLocCommonAPI::GetCurrentDateTime(this, strReissueDate);
	
	strWorkerID = this->m_pInfo->USER_ID;
	strWorkerName = this->m_pInfo->USER_NAME;
	strFirstWork = this->GetWorkLogMsg(_T("회원증등록"), __WFILE__, __LINE__);

	
	pDM->InsertRow(-1);
	pDM->SetCellData(_T("대출자번호"), m_strUserNo, nRowCnt);
	pDM->SetCellData(_T("도서관부호"), strLibCode, nRowCnt);
	pDM->SetCellData(_T("도서관명"), strLibName, nRowCnt);
	pDM->SetCellData(_T("발급일"), strReissueDate.Left(16), nRowCnt);
	pDM->SetCellData(_T("작업자ID"), strWorkerID, nRowCnt);
	pDM->SetCellData(_T("작업자명"), strWorkerName, nRowCnt);
	pDM->SetCellData(_T("최초작업"), strFirstWork, nRowCnt);
	CString strGetData = m_strUserNo;
	if(_T("Y") != m_strCivilNoHideYN)
	{
		CLocCommonAPI::ChangeSecuriyUserNo(this, strGetData, m_sIsUnityLoanService);
	}
	pDM->SetCellData(_T("대출자번호_VIEW"), strGetData, i);

	strTemp = strReissueDate;
	strReissueDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), strTemp);

	
	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( this->m_pInfo->MODE );
	MobileApi.SetDM( pDM );
	MobileApi.StartLog();
	MobileApi.StartFrame();
	CString strRecKey;
	INT ids = MobileApi.MakeRecKey(strRecKey);
	if(0>ids) 
	{
		pDM->DeleteRow(nRowCnt);
		return;
	}

	MobileApi.InitDBFieldData();
	MobileApi.AddDBFieldData(_T("USER_KEY"),_T("NUMERIC"),m_strUserKey,TRUE);
	MobileApi.AddDBFieldData(_T("USER_NO"),_T("STRING"),m_strUserNo,TRUE);
	MobileApi.AddDBFieldData(_T("REISSUE_DATE"),_T("NUMERIC"),strReissueDate,TRUE);
	MobileApi.AddDBFieldData(_T("REISSUE_LIB_CODE"),_T("STRING"),this->m_pInfo->LIB_CODE,TRUE);
	MobileApi.AddDBFieldData(_T("REISSUE_LIB_NAME"),_T("STRING"),this->m_pInfo->LIB_NAME,TRUE);
	MobileApi.AddDBFieldData(_T("WORKER_ID"),_T("STRING"),this->m_pInfo->USER_ID,TRUE);
	MobileApi.AddDBFieldData(_T("WORKER_NAME"),_T("STRING"),this->m_pInfo->USER_NAME,TRUE);
	MobileApi.AddDBFieldData(_T("FIRST_WORK"),_T("STRING"),this->GetWorkLogMsg(_T(""), __WFILE__, __LINE__),TRUE);
	MobileApi.MakeInsertFrame(_T("CO_MEMBER_CARD_ISSUE_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey);
	MobileApi.SendFrame();
	MobileApi.EndFrame();
	MobileApi.EndLog();

	
	CESL_Grid *pGrid = (CESL_Grid*)FindControl(_T("CM_USER_CARD_HISTORY"), _T("grid"));
	pGrid->DisplayLine(nRowCnt);
}
