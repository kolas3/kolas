// BudgetListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetListDlg.h"
#include "BudgetApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBudgetListDlg dialog


CBudgetListDlg::CBudgetListDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBudgetListDlg)
	m_bIsDelayedBudget = FALSE;
	//}}AFX_DATA_INIT

	m_nOpenMode = 1; //default
	m_pChildMat = NULL;
	m_pChildUsage = NULL;
	m_pChildExecute = NULL;

	m_arraySubDeleteKey.RemoveAll();
	m_arrayExeDeleteKey.RemoveAll();

	m_pCM = NULL;
	m_pDM = NULL;
	m_pDM_EXE_SUB = NULL;

	m_pParentDM = NULL;	
	m_pParentGrid = NULL;
	m_nCurrentIdx = -1;
	m_nMoveOption = 1;

	m_strBUDGET_COMPILATIONKEY = _T("");
	m_bSubTypeLimited = TRUE;
}

CBudgetListDlg::~CBudgetListDlg()
{
	FreeChild();
}


VOID CBudgetListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBudgetListDlg)
	DDX_Control(pDX, IDC_tabBUDGET_LIST, m_ctrlTab);
	DDX_Check(pDX, IDC_chkBUDGET_LIST_DELAYED_YN, m_bIsDelayedBudget);
	//}}AFX_DATA_MAP
}

BOOL CBudgetListDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BEGIN_MESSAGE_MAP(CBudgetListDlg, CDialog)
	//{{AFX_MSG_MAP(CBudgetListDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabBUDGET_LIST, OnSelchangetabBUDGETLIST)
	ON_BN_CLICKED(IDC_chkBUDGET_LIST_DELAYED_YN, OnchkBUDGETLISTDELAYEDYN)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_PREV, OnbtnBUDGETLISTPREV)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_NEXT, OnbtnBUDGETLISTNEXT)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_EXECUTION_INFO, OnbtnBUDGETLISTEXECUTIONINFO)
	ON_MESSAGE(IDS_REFRESHALL,OnRefreshAll)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBudgetListDlg message handlers

BOOL CBudgetListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	if(InitESL_Mgr( _T("예산관리_예산편성") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 예산관리_예산편성") );
		return false;
	}
		
	m_pCM = FindCM(_T("CM_예산편성"));
	m_pDM = FindDM(_T("DM_예산편성"));
	m_pDM_EXE_SUB = FindDM(_T("DM_예산집행보조"));

	if(!m_pCM || !m_pDM || !m_pDM_EXE_SUB) {
		AfxMessageBox( _T("CM,DM Error : CM_예산편성,DM_예산편성,DM_예산집행보조") );
		return FALSE;
	}

	//modified by loveisverb 
	//2004-12-22
	//ECOLAS는 K2UP과 달리 예산관리 보조 유형을 사용하지 않는다.
	//m_bSubTypeLimited = IsBudgetSubTypeLimited(m_pDM);
	m_bSubTypeLimited = FALSE;

	INT codeCnt = m_pInfo->pCodeMgr->GetCodeCount(_T("예산산출구분"));

	CRect rect;
	this->GetClientRect(rect);
	
	rect.top += 265;
	rect.bottom -= 62;
	rect.left += 35;
	rect.right -= 35;

//////////////////////////////////////////////////////////////////////
		m_pChildForm = new CBudgetListForm(this);
		m_pChildForm->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildForm->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildForm->Create(this);
		m_pChildForm->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////

		m_pChildMat = new CBudgetListMat(this);
		m_pChildMat->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildMat->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildMat->Create(this);
		m_pChildMat->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////

		m_pChildMedia = new CBudgetListMedia(this);
		m_pChildMedia->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildMedia->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildMedia->Create(this);
		m_pChildMedia->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////

		m_pChildSubReg = new CBudgetListSubReg(this);
		m_pChildSubReg->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildSubReg->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildSubReg->Create(this);
		m_pChildSubReg->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////

		m_pChildUseObj = new CBudgetListUseObj(this);
		m_pChildUseObj->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildUseObj->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildUseObj->Create(this);
		m_pChildUseObj->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////

		m_pChildManage = new CBudgetListManage(this);
		m_pChildManage->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildManage->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildManage->Create(this);
		m_pChildManage->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////

		m_pChildUsage = new CBudgetListUsage(this);
		m_pChildUsage->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildUsage->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildUsage->Create(this);
		m_pChildUsage->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////

		m_pChildCource = new CBudgetListCource(this);
		m_pChildCource->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildCource->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildCource->Create(this);
		m_pChildCource->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////


		m_pChildDept = new CBudgetListDept(this);
		m_pChildDept->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildDept->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildDept->Create(this);
		m_pChildDept->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////

		m_pChildExecute = new CBudgetExecutionMgr(this);
		m_pChildExecute->m_nOpenMode = m_nOpenMode;

		if(m_nOpenMode<0)
			m_pChildExecute->m_pArrayDeleteKey = &m_arraySubDeleteKey;

		m_pChildExecute->Create(this);
		//m_pChildExecute->MoveWindow(rect);

//////////////////////////////////////////////////////////////////////
	CString strTmp;
	INT startrow = 0;
	INT nDeptYN = 0;
	
	//육사에서 학처탭을 최우선으로 요구.
	for ( INT nIdx = 0 ; nIdx < codeCnt ; nIdx++ )
	{
		startrow = m_pInfo->pCodeMgr->GetCode(_T("예산산출구분"), _T("코드"), strTmp, startrow);
		startrow++;
		if ( strTmp == _T("학부별") )
		{		
			m_ctrlTab.InsertItem(0, _T("학처별") );
			m_pChildDept->ShowWindow(SW_SHOW);
			nDeptYN = 1;
		}
	}
	
	startrow = 0;

	for ( nIdx = 0 ; nIdx < codeCnt ; nIdx++ )
	{
		startrow = m_pInfo->pCodeMgr->GetCode(_T("예산산출구분"), _T("코드"), strTmp, startrow);
		startrow++;

		INT nRow = m_ctrlTab.GetRowCount();

		if( nDeptYN == 1 && strTmp == _T("학부별") )
			continue;

		if ( strTmp == _T("학부별") )
		{		
			if ( nRow == 0 )
				m_pChildDept->ShowWindow(SW_SHOW);
			else 
				m_pChildDept->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nRow, _T("학처별") );
		}
		else if ( strTmp == _T("형식구분") )
		{

			if ( nRow == 0 )
				m_pChildForm->ShowWindow(SW_SHOW);
			else
				m_pChildForm->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nRow, strTmp );
		}
		else if ( strTmp == _T("자료구분") )
		{
			if ( nRow == 0 )
				m_pChildMat->ShowWindow(SW_SHOW);
			else 
				m_pChildMat->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nRow, strTmp );
		}
		else if ( strTmp == _T("매체구분") )
		{
			if ( nRow == 0 )
				m_pChildMedia->ShowWindow(SW_SHOW);
			else 
				m_pChildMedia->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nRow, strTmp );
		}
		else if ( strTmp == _T("보조등록구분") )
		{
			if ( nRow == 0 )
				m_pChildSubReg->ShowWindow(SW_SHOW);
			else 
				m_pChildSubReg->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nRow, strTmp );
		}
		else if ( strTmp == _T("이용대상구분") )
		{
			if ( nRow == 0 )
				m_pChildUseObj->ShowWindow(SW_SHOW);
			else 
				m_pChildUseObj->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nRow, strTmp );
		}
		else if ( strTmp == _T("관리구분") )
		{		
			if ( nRow == 0 )
				m_pChildManage->ShowWindow(SW_SHOW);
			else 
				m_pChildManage->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nRow, strTmp );
		}
		else if ( strTmp == _T("용도구분") )
		{		
			if ( nRow == 0 )
				m_pChildUsage->ShowWindow(SW_SHOW);
			else 
				m_pChildUsage->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nRow, strTmp );
		}
		else if ( strTmp == _T("학과별") )
		{		
			if ( nRow == 0 )
				m_pChildCource->ShowWindow(SW_SHOW);
			else 
				m_pChildCource->ShowWindow(SW_HIDE);

			m_ctrlTab.InsertItem(nIdx, strTmp );
		}
	}

	if(InitOpenMode()<0)
	{
		EndDialog(0);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBudgetListDlg::OnRefreshAll()
{
	if(!m_strBUDGET_COMPILATIONKEY.IsEmpty())
		RefreshAll(m_strBUDGET_COMPILATIONKEY);
}

INT CBudgetListDlg::InitOpenMode()
{
	
	if(m_nOpenMode>0) //입력 
	{
	
		GetDlgItem(IDC_btnBUDGET_LIST_PREV)->ShowWindow(FALSE);
		GetDlgItem(IDC_btnBUDGET_LIST_NEXT)->ShowWindow(FALSE);

		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtBUDGET_LIST_CF_BUDGET);
		if(pEdit) pEdit->SetReadOnly(TRUE);
	}
	else //수정
	{
		//부모 그리드에서 선정정보를 얻어온다
		if(!m_pParentGrid) {
			AfxMessageBox(_T("부모 그리드를 찾을 수 없습니다!"));
			return -1;
		}

		if(!m_pParentDM) {
			AfxMessageBox(_T("부모 DM을 찾을 수 없습니다!"));
			return -2;
		}
		
	
		if(m_nCurrentIdx<0) {
			AfxMessageBox(_T("선택된 항목이 없습니다!"));
			return -3;
		}
	
		m_strBUDGET_COMPILATIONKEY = m_pParentDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);

		if(m_strBUDGET_COMPILATIONKEY.IsEmpty())
		{
			AfxMessageBox(_T("해당 KEY를 찾을 수 없습니다!"));
			return -4;
		}
		
		if(!m_strBUDGET_COMPILATIONKEY.IsEmpty())
			RefreshAll(m_strBUDGET_COMPILATIONKEY);		
	}

	return 0;

}

VOID CBudgetListDlg::FreeChild()
{
	if(m_pChildMat) delete m_pChildMat;
	m_pChildMat = NULL;
	
	if(m_pChildUsage) delete m_pChildUsage;
	m_pChildUsage = NULL;

	if(m_pChildForm) delete m_pChildForm;
	m_pChildForm = NULL;

	if(m_pChildMedia) delete m_pChildMedia;
	m_pChildMedia = NULL;

	if(m_pChildSubReg) delete m_pChildSubReg;
	m_pChildSubReg = NULL;

	if(m_pChildUseObj) delete m_pChildUseObj;
	m_pChildUseObj = NULL;

	if(m_pChildManage) delete m_pChildManage;
	m_pChildManage = NULL;

	if(m_pChildCource) delete m_pChildCource;
	m_pChildCource = NULL;

	if(m_pChildDept) delete m_pChildDept;
	m_pChildDept = NULL;

	if(m_pChildExecute) delete m_pChildExecute;
	m_pChildExecute = NULL;

}

VOID CBudgetListDlg::OnSelchangetabBUDGETLIST(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	INT nCurSel = m_ctrlTab.GetCurSel();

	TC_ITEM tcm;
	CString strTabMenu;

	TCHAR	szBuff[64];
	tcm.mask = TCIF_TEXT;
	tcm.pszText = szBuff;
	tcm.cchTextMax = sizeof(szBuff);
	m_ctrlTab.GetItem(nCurSel, &tcm );
	strTabMenu.Format(_T("%s"), tcm.pszText );

	if ( strTabMenu == _T("형식구분") )
	{
		m_pChildForm->ShowWindow(SW_SHOW);
		m_pChildMat->ShowWindow(SW_HIDE);
		m_pChildMedia->ShowWindow(SW_HIDE);
		m_pChildSubReg->ShowWindow(SW_HIDE);

		m_pChildUsage->ShowWindow(SW_HIDE);
		m_pChildUseObj->ShowWindow(SW_HIDE);
		m_pChildManage->ShowWindow(SW_HIDE);
		m_pChildCource->ShowWindow(SW_HIDE);
		m_pChildDept->ShowWindow(SW_HIDE);

	}
	else if ( strTabMenu == _T("자료구분") )
	{
		m_pChildForm->ShowWindow(SW_HIDE);
		m_pChildMat->ShowWindow(SW_SHOW);
		m_pChildMedia->ShowWindow(SW_HIDE);
		m_pChildSubReg->ShowWindow(SW_HIDE);

		m_pChildUsage->ShowWindow(SW_HIDE);
		m_pChildUseObj->ShowWindow(SW_HIDE);
		m_pChildManage->ShowWindow(SW_HIDE);
		m_pChildCource->ShowWindow(SW_HIDE);
		m_pChildDept->ShowWindow(SW_HIDE);
	}
	else if ( strTabMenu == _T("매체구분") )
	{
		m_pChildForm->ShowWindow(SW_HIDE);
		m_pChildMat->ShowWindow(SW_HIDE);
		m_pChildMedia->ShowWindow(SW_SHOW);
		m_pChildSubReg->ShowWindow(SW_HIDE);

		m_pChildUsage->ShowWindow(SW_HIDE);
		m_pChildUseObj->ShowWindow(SW_HIDE);
		m_pChildManage->ShowWindow(SW_HIDE);
		m_pChildCource->ShowWindow(SW_HIDE);
		m_pChildDept->ShowWindow(SW_HIDE);
	}
	else if ( strTabMenu == _T("보조등록구분") )
	{
		m_pChildForm->ShowWindow(SW_HIDE);
		m_pChildMat->ShowWindow(SW_HIDE);
		m_pChildMedia->ShowWindow(SW_HIDE);
		m_pChildSubReg->ShowWindow(SW_SHOW);

		m_pChildUsage->ShowWindow(SW_HIDE);
		m_pChildUseObj->ShowWindow(SW_HIDE);
		m_pChildManage->ShowWindow(SW_HIDE);
		m_pChildCource->ShowWindow(SW_HIDE);
		m_pChildDept->ShowWindow(SW_HIDE);
	}
	else if ( strTabMenu == _T("용도구분") )
	{
		m_pChildForm->ShowWindow(SW_HIDE);
		m_pChildMat->ShowWindow(SW_HIDE);
		m_pChildMedia->ShowWindow(SW_HIDE);
		m_pChildSubReg->ShowWindow(SW_HIDE);

		m_pChildUsage->ShowWindow(SW_SHOW);
		m_pChildUseObj->ShowWindow(SW_HIDE);
		m_pChildManage->ShowWindow(SW_HIDE);
		m_pChildCource->ShowWindow(SW_HIDE);
		m_pChildDept->ShowWindow(SW_HIDE);
	}
	else if ( strTabMenu == _T("이용대상구분") )
	{
		m_pChildForm->ShowWindow(SW_HIDE);
		m_pChildMat->ShowWindow(SW_HIDE);
		m_pChildMedia->ShowWindow(SW_HIDE);
		m_pChildSubReg->ShowWindow(SW_HIDE);

		m_pChildUsage->ShowWindow(SW_HIDE);
		m_pChildUseObj->ShowWindow(SW_SHOW);
		m_pChildManage->ShowWindow(SW_HIDE);
		m_pChildCource->ShowWindow(SW_HIDE);
		m_pChildDept->ShowWindow(SW_HIDE);
	}
	else if ( strTabMenu == _T("관리구분") )
	{
		m_pChildForm->ShowWindow(SW_HIDE);
		m_pChildMat->ShowWindow(SW_HIDE);
		m_pChildMedia->ShowWindow(SW_HIDE);
		m_pChildSubReg->ShowWindow(SW_HIDE);

		m_pChildUsage->ShowWindow(SW_HIDE);
		m_pChildUseObj->ShowWindow(SW_HIDE);
		m_pChildManage->ShowWindow(SW_SHOW);
		m_pChildCource->ShowWindow(SW_HIDE);
		m_pChildDept->ShowWindow(SW_HIDE);
	}
	else if ( strTabMenu == _T("학과별") )
	{
		m_pChildForm->ShowWindow(SW_HIDE);
		m_pChildMat->ShowWindow(SW_HIDE);
		m_pChildMedia->ShowWindow(SW_HIDE);
		m_pChildSubReg->ShowWindow(SW_HIDE);

		m_pChildUsage->ShowWindow(SW_HIDE);
		m_pChildUseObj->ShowWindow(SW_HIDE);
		m_pChildManage->ShowWindow(SW_HIDE);
		m_pChildCource->ShowWindow(SW_SHOW);
		m_pChildDept->ShowWindow(SW_HIDE);
	}
	else if ( strTabMenu == _T("학처별") )
	{
		m_pChildForm->ShowWindow(SW_HIDE);
		m_pChildMat->ShowWindow(SW_HIDE);
		m_pChildMedia->ShowWindow(SW_HIDE);
		m_pChildSubReg->ShowWindow(SW_HIDE);

		m_pChildUsage->ShowWindow(SW_HIDE);
		m_pChildUseObj->ShowWindow(SW_HIDE);
		m_pChildManage->ShowWindow(SW_HIDE);
		m_pChildCource->ShowWindow(SW_HIDE);
		m_pChildDept->ShowWindow(SW_SHOW);
	}

	*pResult = 0;
}

INT CBudgetListDlg::SaveControlDataToDM()
{
	
	if(m_nOpenMode>0) m_pDM->InsertRow(-1);


	//DM으로 직접가지고올 data들
	const INT cnt = 9;
	CString alias[ cnt ] = {
		_T("예산편성년도"),				//	0
		_T("예산편성구분"),				//	1
		_T("예산편성상태"),				//	2
		_T("예산편성명"),				//	3
		_T("예산편성자"),				//	4
		_T("이월된예산액"),				//	5
		_T("이월할예산액"),				//	6
		_T("예산편성액"),				//	7
		_T("예산편성보조구분")			//	8
		};
	
	CString tmp;

	for(INT i=0;i<cnt;i++)
	{
		tmp = _T("");	
		m_pCM->GetControlMgrData(alias[i],tmp);
		m_pDM->SetCellData(alias[i],tmp,0);
	}
	
	UpdateData(TRUE);
	
	//이월처리여부
	if(m_bIsDelayedBudget)
		m_pDM->SetCellData(_T("이월처리여부"),_T("Y"),0);
	else 
		m_pDM->SetCellData(_T("이월처리여부"),_T("N"),0);

	return 0;
}

VOID CBudgetListDlg::OnOK() 
{
	// TODO: Add extra validation here
	//저장함수

	CString strBUDGET_YEAR;
	m_pCM->GetControlMgrData(_T("예산편성년도"),strBUDGET_YEAR);

	if(strBUDGET_YEAR.IsEmpty()) 
	{
		AfxMessageBox(_T("예산편성년도가 입력되지 않았습니다!"));
		return;
	}

	CString strBUDGET_CLASS;
	m_pCM->GetControlMgrData(_T("예산편성구분"),strBUDGET_CLASS);

	if(strBUDGET_CLASS.IsEmpty()) 
	{
		AfxMessageBox(_T("예산편성구분이 입력되지 않았습니다!"));
		return;
	}

	SaveControlDataToDM();
	
	CBudgetApi *pApi = NULL;
	pApi = new CBudgetApi (this);
	INT ids = -1;

	if(m_nOpenMode>0)
	{

		ids = pApi->InsertBudgetList(m_pDM,
								   m_pChildMat->m_pDM, 
								   m_pChildUsage->m_pDM,
								   m_pChildForm->m_pDM,
								   m_pChildMedia->m_pDM,
								   m_pChildSubReg->m_pDM,
								   m_pChildUseObj->m_pDM,
								   m_pChildManage->m_pDM,
								   m_pChildCource->m_pDM,
								   m_pChildDept->m_pDM,
								   m_pChildExecute->m_pDM
								   );
	}
	else
	{

		ids = pApi->UpdateBudgetList(m_pDM,
								   m_pChildMat->m_pDM, 
								   m_pChildUsage->m_pDM,
								   m_pChildForm->m_pDM,
								   m_pChildMedia->m_pDM,
								   m_pChildSubReg->m_pDM,
								   m_pChildUseObj->m_pDM,
								   m_pChildManage->m_pDM,
								   m_pChildCource->m_pDM,
								   m_pChildDept->m_pDM,
								   m_pChildExecute->m_pDM,
								   m_arraySubDeleteKey,
	                               m_arrayExeDeleteKey);
	}
	

	if(ids<0)
	{
		AfxMessageBox(_T("저장에 실패하였습니다!"));
		return;
	}
	else
	{
		if(m_nOpenMode>0) 
		{
			AfxMessageBox(_T("입력하였습니다!"));
			m_strBUDGET_COMPILATIONKEY = m_pDM->GetCellData(_T("REC_KEY"),0);
		}
		else 
		{
			AfxMessageBox(_T("수정하였습니다!"));
		}

		RefreshParent();
		RefreshAll(m_strBUDGET_COMPILATIONKEY);		
		return;
	}


	CDialog::OnOK();
}

VOID CBudgetListDlg::OnchkBUDGETLISTDELAYEDYN() 
{
	// TODO: Add your control notification handler code here
	//이월 되었을 경우 - 이월할 급액란을 활성화 

	UpdateData(TRUE);
	
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtBUDGET_LIST_CF_BUDGET);

	//이월처리여부
	if(m_bIsDelayedBudget)
	{
		if(pEdit) pEdit->SetReadOnly(FALSE);
	}
	else
	{
		if(pEdit) pEdit->SetReadOnly(TRUE);
	}

}

INT CBudgetListDlg::RefreshAll(CString strREC_KEY)
{
	
	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"),strREC_KEY);


	m_pDM->RefreshDataManager(strWhere);
	m_pCM->AllControlDisplay(0);
	

	//예산집행보조 
	strWhere.Format(_T("BUDGET_EXECUTION_KEY IN (SELECT REC_KEY FROM CO_BUDGET_EXECUTION_TBL WHERE ")
					_T("BUDGET_COMPILATION_KEY = %s)"),strREC_KEY);
	m_pDM_EXE_SUB->RefreshDataManager(strWhere);



	//delete key refresh
	m_arraySubDeleteKey.RemoveAll();
	m_arrayExeDeleteKey.RemoveAll();


	//child refresh
	m_pChildMat->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildUsage->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildForm->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildMedia->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildSubReg->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildUseObj->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildManage->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildCource->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildDept->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	m_pChildExecute->m_strBUDGET_COMPILATION_KEY = strREC_KEY;
	
	m_pChildMat->RefreshAll();
	m_pChildUsage->RefreshAll();
	m_pChildForm->RefreshAll();
	m_pChildMedia->RefreshAll();
	m_pChildSubReg->RefreshAll();
	m_pChildUseObj->RefreshAll();
	m_pChildManage->RefreshAll();
	m_pChildCource->RefreshAll();
	m_pChildDept->RefreshAll();
	m_pChildExecute->RefreshAll();

	return 0;
}

VOID CBudgetListDlg::OnbtnBUDGETLISTPREV() 
{
	// TODO: Add your control notification handler code here
	if(1 == m_nMoveOption)
	{
		m_nCurrentIdx--;

		if(m_nCurrentIdx<0)
		{
			AfxMessageBox(_T("처음 항목입니다!"));
			m_nCurrentIdx++;
			return;
		}
	}
	else if(2 == m_nMoveOption)
	{
		INT nIdx = m_pParentGrid->SelectGetPrev();

		if(nIdx<0) 
		{
			INT nHeadIdx = m_pParentGrid->SelectGetHeadPosition();
			
			if(m_nCurrentIdx == nHeadIdx)
			{
				AfxMessageBox(_T("처음 항목입니다!"));
				m_pParentGrid->SelectGetHeadPosition();
				return;
			}
			else //이전 선정 정보가 없을 경우 맨 앞의 선정정보를 택한다 
			{
				m_nCurrentIdx = nHeadIdx;
			}
		
		}
		else //선정 정보를 기준으로 움직인다
		{
			m_nCurrentIdx = nIdx;
		}

	}

	m_strBUDGET_COMPILATIONKEY = m_pParentDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);
		
	if(!m_strBUDGET_COMPILATIONKEY.IsEmpty())
			RefreshAll(m_strBUDGET_COMPILATIONKEY);

	m_pParentGrid->SetReverse(m_nCurrentIdx);
}

VOID CBudgetListDlg::OnbtnBUDGETLISTNEXT() 
{
	// TODO: Add your control notification handler code here
		if(1 == m_nMoveOption)
	{
		m_nCurrentIdx++;
		INT nMaxIdx = m_pParentDM->GetRowCount()-1;
		if(m_nCurrentIdx > nMaxIdx)
		{
			AfxMessageBox(_T("마지막 항목입니다!"));
			m_nCurrentIdx--;
			return;
		}
	}
	else if(2 == m_nMoveOption)
	{
		INT nIdx = m_pParentGrid->SelectGetNext();

		if(nIdx<0) 
		{
			INT nTailIdx = m_pParentGrid->SelectGetTailPosition();
			
			if(m_nCurrentIdx == nTailIdx)
			{
				AfxMessageBox(_T("마지막 항목입니다!"));
				m_pParentGrid->SelectGetTailPosition();
				return;
			}
			else //이전 선정 정보가 없을 경우 맨뒤의 선정정보를 택한다 
			{
				m_nCurrentIdx = nTailIdx;
			}
		}
		else //선정 정보를 기준으로 움직인다
		{
			m_nCurrentIdx = nIdx;
		}
	}


	m_strBUDGET_COMPILATIONKEY = m_pParentDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);
		
	if(!m_strBUDGET_COMPILATIONKEY.IsEmpty())
		RefreshAll(m_strBUDGET_COMPILATIONKEY);

	m_pParentGrid->SetReverse(m_nCurrentIdx);
}

VOID CBudgetListDlg::OnbtnBUDGETLISTEXECUTIONINFO() 
{
	// TODO: Add your control notification handler code here
	
	if(m_pChildExecute)
	{
		m_pChildExecute->CenterWindow();
		m_pChildExecute->ShowWindow(SW_SHOW);	
	}

}
VOID CBudgetListDlg::RefreshParent()
{
	//부모 DM에 현재의 DM을 copy해주고 grid를 display한다
	INT idx;
	
	if(m_nOpenMode>0) //입력 
	{
		m_pParentDM->InsertRow(-1);
		idx = m_pParentDM->GetRowCount()-1;
	}	
	else
	{
		
		idx = m_nCurrentIdx;
	}


	CopyDMToDMByAlias(m_pDM,0,m_pParentDM,idx);
	

	CString strTmp;	
	CString strDesc;

	//UDF_예산편성구분
	strTmp = m_pDM->GetCellData(_T("예산편성구분"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("예산편성구분"),strTmp,strDesc);
	m_pParentDM->SetCellData(_T("UDF_예산편성구분"),strDesc,idx);


	//UDF_예산편성상태
	strTmp = m_pDM->GetCellData(_T("예산편성상태"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("예산편성상태"),strTmp,strDesc);
	m_pParentDM->SetCellData(_T("UDF_예산편성상태"),strDesc,idx);
	

	m_pParentGrid->DisplayLine(idx);

}

INT CBudgetListDlg::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}

BOOL CBudgetListDlg::IsBudgetSubTypeLimited(CESL_DataMgr* pDM)
{
	CString strQuery;
	CString strResult;

	strQuery.Format(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'BUDGETMNGLIMIT'"));

	pDM->GetOneValueQuery(strQuery,strResult);

	strResult.TrimLeft();
	strResult.TrimRight();

	if(strResult.IsEmpty() || _T("N") == strResult) return FALSE;

	return TRUE;
}


HBRUSH CBudgetListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
