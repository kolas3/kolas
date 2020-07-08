// BudgetExecutionInfo.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetExecutionInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBudgetExecutionInfo dialog


CBudgetExecutionInfo::CBudgetExecutionInfo(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBudgetExecutionInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pCM = NULL;
	m_pParentDM = NULL;	
	m_pParentGrid = NULL;
	m_nCurrentIdx = -1;

	m_nOpenMode = 1; //default
}

CBudgetExecutionInfo::~CBudgetExecutionInfo()
{
}

VOID CBudgetExecutionInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBudgetExecutionInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBudgetExecutionInfo, CDialog)
	//{{AFX_MSG_MAP(CBudgetExecutionInfo)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBudgetExecutionInfo message handlers

BOOL CBudgetExecutionInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("예산관리_예산편성_예산집행_개별정보") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 예산관리_예산편성_예산집행_개별정보") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_예산집행_개별정보"));

	if(!m_pCM || !m_pParentDM || !m_pParentGrid) {
		AfxMessageBox( _T("CM,DM,Grid Error : CM_예산집행_개별정보,DM_예산집행,그리드") );
		return FALSE;
	}

	if(m_nOpenMode<0)
	{
		
		if(m_nCurrentIdx<0)
		{
			AfxMessageBox( _T("선택된 자료가 없습니다!") );
			EndDialog(0);
			return FALSE;

		}
		
		m_pCM->AllControlDisplay(m_nCurrentIdx);
	
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBudgetExecutionInfo::OnOK() 
{
	// TODO: Add extra validation here
	
	CString strBudget;
	//strBudget = m_pCM->GetControlMgrData(_T("집행예산"),strBudget);
	strBudget.Format(_T("%d"),m_pCM->GetControlMgrData(_T("집행예산"),strBudget));

	if(strBudget.IsEmpty())
	{
		AfxMessageBox( _T("집행예산을 입력해 주십시요!") );
		return;
	}

	CString strDate;
	//strDate = m_pCM->GetControlMgrData(_T("집행일"),strDate);
	strDate.Format(_T("%d"), m_pCM->GetControlMgrData(_T("집행일"),strDate));

	if(strDate.IsEmpty())
	{
		AfxMessageBox( _T("집행일을 입력해 주십시요!") );
		return;
	}
	

	if(m_nOpenMode>0)
	{
		m_pParentDM->InsertRow(-1);
		m_nCurrentIdx = m_pParentDM->GetRowCount()-1;
	}


	const INT cnt = 6;
	CString alias[cnt];
	CString data[cnt];

	alias[0] = _T("집행예산");
	alias[1] = _T("집행상태");
	alias[2] = _T("집행일");
	alias[3] = _T("집행타입");
	alias[4] = _T("집행작업자");
	alias[5] = _T("비고");	

	for(INT i=0;i<cnt;i++)
	{
		m_pCM->GetControlMgrData(alias[i],data[i]);
	}

	m_pParentDM->SetCellData(_T("집행예산"),data[0],m_nCurrentIdx);
	m_pParentDM->SetCellData(_T("집행상태"),data[1],m_nCurrentIdx);
	m_pParentDM->SetCellData(_T("UDF_집행상태"),data[1],m_nCurrentIdx);
	m_pParentDM->SetCellData(_T("집행일"),data[2],m_nCurrentIdx);
	m_pParentDM->SetCellData(_T("집행타입"),data[3],m_nCurrentIdx);
	m_pParentDM->SetCellData(_T("UDF_집행타입"),data[3],m_nCurrentIdx);
	m_pParentDM->SetCellData(_T("집행작업자"),data[4],m_nCurrentIdx);
	m_pParentDM->SetCellData(_T("비고"),data[5],m_nCurrentIdx);

	if(m_nOpenMode>0)
	{
		m_pParentDM->SetCellData(_T("UDF_상태"),_T("I"),m_nCurrentIdx);
	}
	else
	{
		CString strUDF = m_pParentDM->GetCellData(_T("UDF_상태"),m_nCurrentIdx);
		if(_T("I") != strUDF) m_pParentDM->SetCellData(_T("UDF_상태"),_T("U"),m_nCurrentIdx);
	}


	CDialog::OnOK();
}

HBRUSH CBudgetExecutionInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
