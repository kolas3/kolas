// SetUpRegNoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SetUpRegNoDlg.h"
#include "DropRegNoApplyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetUpRegNoDlg dialog


CSetUpRegNoDlg::CSetUpRegNoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSetUpRegNoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetUpRegNoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pDM_INDEX = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pApi = NULL;
}
CSetUpRegNoDlg::~CSetUpRegNoDlg()
{
	
}


VOID CSetUpRegNoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetUpRegNoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetUpRegNoDlg, CDialog)
	//{{AFX_MSG_MAP(CSetUpRegNoDlg)
	ON_BN_CLICKED(IDC_btnSET_REG_NO_LOAN_INFO, OnbtnSETREGNOLOANINFO)
	ON_BN_CLICKED(IDC_btnSET_REG_NO_DROP_REG_NO, OnbtnSETREGNODROPREGNO)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetUpRegNoDlg message handlers

BOOL CSetUpRegNoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_등록번호설정")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_등록번호설정"));	
	m_pGrid = (CESL_Grid *)m_pCM->FindControl(_T("MainGrid"));

	if(!Init())
	{
		ESLAfxMessageBox(_T("등록번호설정 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSetUpRegNoDlg::Init()
{
	if(m_pDM == NULL) return FALSE;
	if(m_pDM_INDEX == NULL) return FALSE;
	m_pGrid->Display();
	return TRUE;
}

BOOL CSetUpRegNoDlg::CheckSelectValid()
{
	if(m_pDM->GetRowCount() == 0) return FALSE;
	
	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return FALSE;
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return FALSE;
	
	return TRUE;
}

VOID CSetUpRegNoDlg::OnbtnSETREGNOLOANINFO() 
{
	// TODO: Add your control notification handler code here
	if(m_pApi == NULL)	 return;
	
	m_pApi->InitTempData();	m_pApi->InitTempPtr();
	m_pApi->SetTempPtr(_T("색인DM"), (DWORD*)m_pDM_INDEX);
	m_pApi->SetTempPtr(_T("책DM"), (DWORD*)m_pDM);
	INT ids = m_pApi->SPFExecutePath(_T("등록번호설정IN대출정보"));
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("등록번호를 설정할 수 없습니다."));
		return;
	}
	ESLAfxMessageBox(_T("등록번호를 설정 하였습니다."));
	m_pGrid->Display();
}

VOID CSetUpRegNoDlg::OnbtnSETREGNODROPREGNO() 
{
	// TODO: Add your control notification handler code here
	if(!CheckSelectValid())
	{
		ESLAfxMessageBox(_T("선정된 정보가 없습니다."));
		return;
	}

	CDropRegNoApplyDlg dlg(this);
	dlg.SetDataClass(_T("MO"));
	dlg.SetRegCode(m_pDM->GetCellData(_T("BB_등록구분_코드"), 0));
	if (IDCANCEL == dlg.DoModal()) return;

	CString strRegCode, strRegNo;
	CArray<INT, INT> * pArrRegNo = dlg.GetSelectRegNo();
	INT i, j;	i=j=0;
	INT ids = m_pGrid->SelectMakeList();
	i = m_pGrid->SelectGetHeadPosition();
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	while(i >= 0)
	{
		strRegCode = m_pDM->GetCellData(_T("BB_등록구분_코드"), i);
		if(strRegCode != dlg.GetRegCode()) 
		{
			i = m_pGrid->SelectGetNext();
			continue;
		}
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		strRegNo.Format(_T("%s%010d"), strRegCode, pArrRegNo->GetAt(j++));
		CString strTemp;
		strTemp.Format(_T("%d"), pArrRegNo->GetAt(j++));
		strRegNo = pParentMgr->m_pInfo->MakeRegNo(strRegCode, strTemp);
		//===================================================================
		m_pDM->SetCellData(_T("BB_등록번호"), strRegNo, i);
		if(j >= pArrRegNo->GetSize()) break;
		
		i = m_pGrid->SelectGetNext();
	}

	m_pGrid->Display();
}

VOID CSetUpRegNoDlg::OnOK() 
{
	// TODO: Add extra validation here
	::PostMessage(pParentMgr->GetSafeHwnd(), WM_COMMAND, IDC_btnMARC_UPDATE, BN_CLICKED);	
	
	CDialog::OnOK();
}

HBRUSH CSetUpRegNoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}







