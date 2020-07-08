// BoLocReserveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BoLocReserveDlg.h"
#include "..\\LOC_CommonAPI\\LocCommonAPI.h"
#include "..\\BL_LOC_LOAN\\BO_LOC_3170.h"
#include "..\\BL_LOC_LOAN\\BO_LOC_3170_ReserveInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBoLocReserveDlg dialog


CBoLocReserveDlg::CBoLocReserveDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBoLocReserveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBoLocReserveDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pLoanShareManager = NULL;
	m_pLoanMangeValue = NULL;
	m_pPreRegNoMgr = NULL;
	m_pLocProc = NULL;
	m_strWhere = _T("");
	m_strCivilNoHideYN = _T("N");
}
CBoLocReserveDlg::~CBoLocReserveDlg()
{
	if(NULL != m_pLocProc)
	{
		delete m_pLocProc;
		m_pLocProc = NULL;
	}
	if(NULL != m_pPreRegNoMgr)
	{
		delete m_pPreRegNoMgr;
		m_pPreRegNoMgr = NULL;
	}
	if(NULL != m_pLoanMangeValue)
	{
		delete m_pLoanMangeValue;
		m_pLoanMangeValue = NULL;
	}
	if(NULL != m_pLoanShareManager)
	{
		delete m_pLoanShareManager;
		m_pLoanShareManager = NULL;
	}	
}

void CBoLocReserveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBoLocReserveDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBoLocReserveDlg, CDialog)
	//{{AFX_MSG_MAP(CBoLocReserveDlg)
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+1000, OnReSearch)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBoLocReserveDlg message handlers

BOOL CBoLocReserveDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	INT ids;
	ids = InitESL_Mgr(_T("SM_BO_LOC_RESERVE"));
	if(0 > ids) return FALSE;

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_RESERVE"));
	CESL_Grid* pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("예약자료관리");
	}

	SetTimer(1, 200, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBoLocReserveDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
	{		
		if(pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CBoLocReserveDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridBLR_MAIN);
	if(NULL == pGrid->GetSafeHwnd()) return;
	pGrid->MoveWindow(0, 1, cx, cy);
}

void CBoLocReserveDlg::OnTimer(UINT nIDEvent) 
{
	if(1 == nIDEvent)
	{
		KillTimer(1);
		Init();
	}
	
	CDialog::OnTimer(nIDEvent);
}

BOOL CBoLocReserveDlg::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

INT CBoLocReserveDlg::Init()
{
	m_pLoanShareManager = new CLoanShareManager(this);

	m_pLoanMangeValue = new CLonaManageValue(this);
	m_pLoanMangeValue->pMain = pMain;
	m_pLoanMangeValue->pParentMgr = (CESL_Mgr*)this;
	m_pLoanMangeValue->SM_STYLE = 'T';
	m_pLoanMangeValue->m_pInfo = this->m_pInfo;
	m_pLoanMangeValue->DoModal();

	m_pPreRegNoMgr = new CPreRegCodeMgr(this);

	m_pLocProc = new CBL_LOC_LOAN_PROC(this, m_pLoanShareManager, m_pLoanMangeValue, m_pPreRegNoMgr);

	m_pLocProc->m_pManageValue->GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), m_strCivilNoHideYN);
	m_strCivilNoHideYN.TrimLeft();	m_strCivilNoHideYN.TrimRight();	m_strCivilNoHideYN.MakeUpper();

	SetDMAlias();

	return 0;
}

INT CBoLocReserveDlg::SetDMAlias()
{
	INT ids;
	if(m_pLocProc->m_pManageValue->m_bVolReserve)
	{
		INT nIndex;
		CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_RESERVE"));
		if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("SetDMAlias"));
		CESL_DataMgr *pSeDM = FindDM(_T("DM_BO_LOC_RESERVE_SE"));
		if(NULL == pSeDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("SetDMAlias"));
		CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
		CString strFieldName = _T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2 WHERE CL2.SPECIES_KEY=BB.SPECIES_KEY AND CL2.VOL_INFO=BB.VOL_SORT_NO AND CL2.STATUS='3')");
		CString strFieldNameSE = _T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2 WHERE CL2.SPECIES_KEY=BB.SPECIES_KEY AND CL2.VOL_INFO=BB.VOL_KEY AND CL2.STATUS='3')");
		nIndex = pDM->FindColumn(_T("예약이용자수")); 
		if(-1 < nIndex)
		{
			ids = pDM->GetColumnRefInfo(nIndex, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("SetDMAlias"));
			ids = pDM->SetDataMgrRef(nIndex, FIELD_ALIAS, strFieldName, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("SetDMAlias"));
			ids = pSeDM->GetColumnRefInfo(nIndex, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("SetDMAlias"));
			ids = pSeDM->SetDataMgrRef(nIndex, FIELD_ALIAS, strFieldNameSE, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("SetDMAlias"));
		}
	}

	return 0;
}

INT CBoLocReserveDlg::ReservationStatus()
{
	CString strFuncName = _T("ReservationStatus");

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_RESERVE"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	CESL_ControlMgr	*pCM = FindCM(_T("CM_BO_LOC_RESERVE"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	
	pGrid->SelectMakeList();
	INT	nRowCount = pGrid->SelectGetCount();
	pGrid->SelectGetTailPosition();
	if(0 >= nRowCount)
	{
		m_pLocProc->SelfCloseMessageBox(_T("선정된 자료가 없습니다."));
		return 1;
	}
	if(1 < nRowCount)
	{
		m_pLocProc->SelfCloseMessageBox(_T("한 개의 자료만 선정하여 주십시오."));
		return 2;
	}
	INT nIndex;
	nIndex = pGrid->SelectGetIdx();
	if(0 > nIndex) return 0;
	TCHAR *sFieldAlias[5] = 
	{
		 _T("단행/연속구분"), _T("책KEY"), _T("예약이용자수"), _T("종KEY"), _T("권호정보KEY")
	};
	CString sGetData[5];
	for(INT i = 0; i < 5; i++)
	{
		GetDataMgrData(_T("DM_BO_LOC_RESERVE"), sFieldAlias[i], sGetData[i], nIndex);
	}

	if(!m_pLocProc->m_pManageValue->m_bVolReserve)
	{
		CBO_LOC_3170 Dlg(this, m_pLoanShareManager, m_pLoanMangeValue, 0);
		Dlg.pMain = this->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)this;
		Dlg.m_pInfo = this->m_pInfo;
		Dlg.SetInfo(sGetData[3], sGetData[4], sGetData[0], sGetData[1]);
		Dlg.DoModal();
	}
	else 
	{
		CBO_LOC_3170_ReserveInfo Dlg(this, m_pLoanShareManager, m_pLoanMangeValue, 0);
		Dlg.pMain = this->pMain;
		Dlg.pParentMgr = (CESL_Mgr*)this;
		Dlg.m_pInfo = this->m_pInfo;
		Dlg.SetInfo(sGetData[3], sGetData[4], sGetData[0], _T(""));
		Dlg.DoModal();
	}

	return 0;
}

LRESULT CBoLocReserveDlg::OnReSearch(WPARAM wParam, LPARAM lParam) 
{
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_RESERVE"));
	if(NULL == pDM) return -1;
	pDM->RefreshDataManager(m_strWhere);
	CESL_DataMgr *pSeDM = FindDM(_T("DM_BO_LOC_RESERVE_SE"));
	if(NULL == pSeDM) return -1;
	pSeDM->RefreshDataManager(m_strWhere);
	pDM->AddDM(pSeDM);
	pDM->SORT(_T("예약일"));

	if(_T("Y") != m_strCivilNoHideYN)
	{
		CString strGetData = _T("");
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("대출자번호"), i, strGetData);
			m_pLocProc->m_pLocCommonAPI->ChangeSecuriyUserNo(this, strGetData, m_pLoanMangeValue->m_sIsUnityLoanService);
			pDM->SetCellData(_T("대출자번호"), strGetData, i);
		}
	}

	ControlDisplay(_T("CM_BO_LOC_RESERVE"), _T("MainGrid"));

	return 0;
}
