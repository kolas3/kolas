// SePurchaseForm.cpp : implementation file
//

#include "stdafx.h"
#include "SePurchaseForm.h"
#include "SeSubHistoryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseForm dialog


CSePurchaseForm::CSePurchaseForm(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSePurchaseForm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSePurchaseForm)
	m_bCLAIM_YN = FALSE;
	m_bREAD_FIRST_YN = FALSE;
	//}}AFX_DATA_INIT
	
	m_pParentMgr = pParent;
	m_pDM = NULL;
	m_pDM_SUB_HISTORY = NULL;
	m_pCM = NULL;
	m_nOpenMode = 1; //default
	m_bIsLastCtrlFocused = FALSE;

	m_bIsLightVersion = FALSE;
	m_bIsDepartMngUse = FALSE;

	m_pDepartmentOrder = NULL;
	m_bIsDepartMngFirstShow = TRUE;

}

CSePurchaseForm::~CSePurchaseForm()
{
	// 17/09/06 김혜영 : 연속>구입>체크인관리에서 메모리누수 현상 발생
//*/ ADD ---------------------------------------------------------------------------
	if(m_pDepartmentOrder) 
	{
		delete m_pDepartmentOrder;
		m_pDepartmentOrder = NULL;
	}
//*/ END ---------------------------------------------------------------------------
}

VOID CSePurchaseForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSePurchaseForm)
	DDX_Check(pDX, IDC_chkPUR_CLAIM_YN, m_bCLAIM_YN);
	DDX_Check(pDX, IDC_chkPUR_READ_FIRST_YN, m_bREAD_FIRST_YN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSePurchaseForm, CDialog)
	//{{AFX_MSG_MAP(CSePurchaseForm)
	ON_BN_CLICKED(IDC_btnPUR_SUB_HISTORY, OnbtnPURSUBHISTORY)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_edtPUR_CLAIM_INTERVAL, OnKillfocusedtPURCLAIMINTERVAL)
	ON_EN_SETFOCUS(IDC_edtPUR_CLAIM_INTERVAL, OnSetfocusedtPURCLAIMINTERVAL)
	ON_BN_CLICKED(IDC_btnPUR_DEPARTMNG, OnbtnPURDEPARTMNG)
	ON_MESSAGE(BOOKCNTCHANGE, OnDepartmentBookCntChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseForm message handlers
BOOL CSePurchaseForm::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

BOOL CSePurchaseForm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
		
	if( InitESL_Mgr( _T("연속_수서_구입정보") ) < 0 ) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_구입정보") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_연속_구입정보"));
	m_pDM = FindDM(_T("DM_연속_구입정보"));
	m_pDM_SUB_HISTORY =  FindDM(_T("DM_연속_구독이력정보"));


	if(!m_pDM || !m_pCM || !m_pDM_SUB_HISTORY)  {
		AfxMessageBox( _T("DM,CM Error : 연속_수서_구입정보") );
		return FALSE;
	}
	
	if(m_nOpenMode>0)
		GetDlgItem(IDC_btnPUR_SUB_HISTORY)->ShowWindow(SW_HIDE);

	
	//control color
	AddColorControl(IDC_edtPUR_ACQ_BOOK_CNT, _T(""), _T(""), 'E');

	//라이트 버젼 settting
	if(m_bIsLightVersion)
	{
		SetLightVersion();
	}

	//부서 지정 기능 관련 
	if(!m_bIsDepartMngUse)
	{
		SetDepartMngNoUse();
	}
	else
	{
		CreateDepartMngDlg();
	}


	//육사일경우에만 display
	CString str64;
	GetManageValue( _T("기타"), _T("공통"), _T("육사"), _T("육사"), str64 );
	if ( str64 == _T("Y") ) 
	{
		GetDlgItem(IDC_sYookSa1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbDept)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_sYookSa2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbCource)->ShowWindow(SW_SHOW);		
	}
	else
	{
		GetDlgItem(IDC_sYookSa1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbDept)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_sYookSa2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbCource)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_UNIV)->ShowWindow(SW_HIDE);		
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CSePurchaseForm::SetLightVersion()
{

	//라이트 버젼의 경우 열람우선여부, 등록책수 관련 콘트롤없애고 입수책수 관련 콘트롤의 위치를 재배치 한다 
	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_chkPUR_READ_FIRST_YN);
	if(pWnd) pWnd->ShowWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_STATIC_PUR_REG_BOOK_CNT);
	if(pWnd) pWnd->ShowWindow(FALSE);

	pWnd = GetDlgItem(IDC_edtPUR_REG_BOOK_CNT);
	if(pWnd) pWnd->ShowWindow(FALSE);

	CRect rect;
	
	pWnd = GetDlgItem(IDC_STATIC_PUR_ACQ_BOOK_CNT);
	if(pWnd)
	{
		pWnd->GetClientRect(rect);
	
		rect.left = 12;
		rect.right = 61;
		rect.top = 39;
		rect.bottom = 53;

		pWnd->MoveWindow(rect);
	}
	
	pWnd = GetDlgItem(IDC_edtPUR_ACQ_BOOK_CNT);
	if(pWnd)
	{
		pWnd->GetClientRect(rect);
		
		rect.left = 63;
		rect.right = 180;
		rect.top = 37;
		rect.bottom = 57;		
		
		pWnd->MoveWindow(rect);
	}

	//라이트 버젼의 경우는 무조건 열람우선이다
	m_bREAD_FIRST_YN = TRUE;

	UpdateData(FALSE);
}

VOID CSePurchaseForm::SaveAllDataToDM()
{
	
	UpdateData(TRUE);
	//구입 정보들을 DM에 저장한다
	//DM으로 직접가지고올 data들
	const INT cnt = 10;
	CString alias[ cnt ] = {
		_T("SP_구독가격"),			//	0
		_T("SP_구독만기일"),		//	1
		_T("SP_구독시작일"),		//	2
		_T("SP_구입처_코드"),		//	3
		_T("SP_독촉간격"),			//	4
		_T("SP_등록책수"),			//	5
		_T("SP_입수책수"),			//	6
		_T("SP_발행부수"),			//  7
		_T("SP_신청학부"),			//  8
		_T("SP_신청학과")};			//	9
		
	CString tmp;

	for(INT i=0;i<cnt;i++)
	{
		tmp = _T("");	
		m_pCM->GetControlMgrData(alias[i],tmp);
		m_pDM->SetCellData(alias[i],tmp,0);
	}

	//독촉여부
	if(m_bCLAIM_YN)
		m_pDM->SetCellData(_T("SP_독촉여부"),_T("Y"),0);
	else 
	    m_pDM->SetCellData(_T("SP_독촉여부"),_T("N"),0);
	
		//열람우선여부
	if(m_bREAD_FIRST_YN)
		m_pDM->SetCellData(_T("SP_열람우선여부"),_T("Y"),0);
	else 
	    m_pDM->SetCellData(_T("SP_열람우선여부"),_T("N"),0);

}


INT CSePurchaseForm::RefreshAll(CString strSpeciesKey)
{
	CString strWhere;
	strWhere.Format(_T("SPECIES_KEY = %s"),strSpeciesKey);
	m_pDM->RefreshDataManager(strWhere);
	m_pCM->AllControlDisplay(0);
	
	m_pDM_SUB_HISTORY->FreeData();

	//구입관리 데이터가 없는 경우는 기증 데이터를 구입으로 입력하는 경우이다
	INT nRowCnt = m_pDM->GetRowCount();

	if(nRowCnt>0)
	{
		CString strREC_KEY;
		strREC_KEY = m_pDM->GetCellData(_T("SP_구독이력정보KEY"),0);
		strWhere.Format(_T("REC_KEY = %s"),strREC_KEY);
		if(!strREC_KEY.IsEmpty()) {
			m_pDM_SUB_HISTORY->RefreshDataManager(strWhere);
		}
	}

	
	INT nRow = m_pDM_SUB_HISTORY->GetRowCount();

	CString tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SP_독촉여부"),0);
	if(_T("Y") == tmp)
		m_bCLAIM_YN = TRUE;
	else 
		m_bCLAIM_YN = FALSE;

	tmp = _T("");
	

	tmp = m_pDM->GetCellData(_T("SP_열람우선여부"),0);
	if(_T("Y") == tmp)
		m_bREAD_FIRST_YN = TRUE;
	else 
		m_bREAD_FIRST_YN = FALSE;
	
	//2005.02.16 수정 +++
	if ( m_pDepartmentOrder == NULL ) 
		CreateDepartMngDlg();
	//2005.02.16 수정 ---

	if(0 == nRowCnt)
	{
		m_pDepartmentOrder->m_nOpenMode = 1;	
	}
	else
	{
		m_pDepartmentOrder->m_nOpenMode = -1;

		//부서지정자료
		if(m_pDepartmentOrder)
		{
			
			if(m_nOpenMode<0)
			{
				//부서별 자료지정 
				CString strPURCHASE_SPECIES_KEY;
				CString stPURCHASE_MANAGE_KEY;
				stPURCHASE_MANAGE_KEY = m_pDM->GetCellData(_T("SP_구입관리KEY"),0);

				//m_pDepartmentOrder->m_strPURCHASE_SPECIES_KEY = strPURCHASE_SPECIES_KEY;
				m_pDepartmentOrder->m_strPURCHASE_MANAGE_KEY = stPURCHASE_MANAGE_KEY;

			}

			//수정시일 경우는 부서별 입수 책수가 존재할 경우 
			//입수 책수를 수정 하지 못하도록 한다
			CString strCnt;
			CEdit* pEdit = (CEdit*)m_pCM->FindControl(_T("SP_입수책수"));
			pEdit->EnableWindow(TRUE);
			INT nAcqExpBookCnt = 0;
			if(m_nOpenMode<0)
			{
				m_pDepartmentOrder->RefreshAll();
				nAcqExpBookCnt = m_pDepartmentOrder->GetTotalAcqExpBookCnt();
				if(nAcqExpBookCnt>0)
				{
					pEdit->EnableWindow(FALSE);
					
				}
			}
		}

	
	}
	
	UpdateData(FALSE);
	
	return 0;
}

VOID CSePurchaseForm::ClearAll()
{

	m_pCM->AllControlClear();
	m_bCLAIM_YN = FALSE;
	m_bREAD_FIRST_YN = FALSE;
	UpdateData(FALSE);

}

VOID CSePurchaseForm::OnbtnPURSUBHISTORY() 
{
	// TODO: Add your control notification handler code here
	INT nRowCnt = m_pDM->GetRowCount();
	if(0 == nRowCnt) 
	{
		AfxMessageBox(_T("아직 구독이력이 존재하지 않습니다!"));
		return;
	}

	CString strREC_KEY;
	strREC_KEY = m_pDM->GetCellData(_T("SP_구입관리KEY"),0);

	CString strCurrentSubHistoryKey;
	strCurrentSubHistoryKey = m_pDM_SUB_HISTORY->GetCellData(_T("구독이력KEY"),0);

	CSeSubHistoryDlg dlg(this);
	dlg.m_strCurrentKey = strCurrentSubHistoryKey;
	dlg.m_strManageKey = strREC_KEY;
	if(dlg.DoModal() != IDOK) return;

	CString strNewKey;
	strNewKey = dlg.m_strNewKey;
	CString strWhere;
	strWhere.Format(_T("REC_KEY = %s"),strNewKey);
	m_pDM_SUB_HISTORY->RefreshDataManager(strWhere);

	if(0 == m_pDM_SUB_HISTORY->GetRowCount()) return;

	
	//새 구독이력 정보를 적용해준다
	
	//구독이력정보KEY
	CString strtmp = _T("");
	strtmp = m_pDM_SUB_HISTORY->GetCellData(_T("구독이력KEY"),0);
	m_pDM->SetCellData(_T("SP_구독이력정보KEY"),strtmp,0);
	strtmp = _T("");


	//구입처_코드 
	strtmp = m_pDM_SUB_HISTORY->GetCellData(_T("구입처_코드"),0);
	m_pDM->SetCellData(_T("SP_구입처_코드"),strtmp,0);
	strtmp = _T("");


	//구독가격 
	strtmp = m_pDM_SUB_HISTORY->GetCellData(_T("구독가격"),0);
	m_pDM->SetCellData(_T("SP_구독가격"),strtmp,0);
	strtmp = _T("");


	//구독시작일
	strtmp = m_pDM_SUB_HISTORY->GetCellData(_T("구독시작일"),0);
	m_pDM->SetCellData(_T("SP_구독시작일"),strtmp,0);
	strtmp = _T("");


	//구독만기일 
	strtmp = m_pDM_SUB_HISTORY->GetCellData(_T("구독만기일"),0);
	m_pDM->SetCellData(_T("SP_구독만기일"),strtmp,0);
	strtmp = _T("");

	//발행부수 
	strtmp = m_pDM_SUB_HISTORY->GetCellData(_T("발행부수"),0);
	m_pDM->SetCellData(_T("SP_발행부수"),strtmp,0);
	strtmp = _T("");

	m_pCM->AllControlDisplay(0);

}

HBRUSH CSePurchaseForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (IsColorControl(pWnd->GetDlgCtrlID())) {
		return SetColor(pDC);
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

VOID CSePurchaseForm::SetFocusOnFirstCtrl()
{
	GetDlgItem(IDC_cmbPUR_PROVIDER_CODE)->SetFocus();
}

VOID CSePurchaseForm::OnKillfocusedtPURCLAIMINTERVAL() 
{
	// TODO: Add your control notification handler code here
	
	m_bIsLastCtrlFocused = FALSE;
}

BOOL CSePurchaseForm::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_TAB)
		{
			if(m_bIsLastCtrlFocused)
			{
				// 마지막 콘트롤이 killfocus하면 parent에게 메시지를 보낸다
				:: SendMessage(	m_pParentMgr->GetSafeHwnd(), KILLFOCUS_LAST_CTRL , 0, 0);

				return TRUE;
			}
		}
		else if ( pMsg->wParam == VK_ESCAPE )
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CSePurchaseForm::OnSetfocusedtPURCLAIMINTERVAL() 
{
	// TODO: Add your control notification handler code here
	m_bIsLastCtrlFocused = TRUE;
}

VOID CSePurchaseForm::SetDepartMngNoUse()
{
	//부서 지정기능을 사용하지 않는 경우 발행부수와 부서지정 버튼을 감춘다 
	CWnd* pWnd = NULL;

	pWnd = GetDlgItem(IDC_stcPUR_ISSUE);
	if(pWnd) pWnd->ShowWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_edtPUR_ISSUE);
	if(pWnd) pWnd->ShowWindow(FALSE);	

	pWnd = GetDlgItem(IDC_btnPUR_DEPARTMNG);
	if(pWnd) pWnd->ShowWindow(FALSE);

}

VOID CSePurchaseForm::CreateDepartMngDlg()
{
	m_pDepartmentOrder = new CSeDepartmentOrder(this);
	if(!m_pDepartmentOrder) return;

	m_pDepartmentOrder->m_nOpenMode = m_nOpenMode;
	m_pDepartmentOrder->m_nOpenLockMode = -1;
	m_pDepartmentOrder->Create((CESL_Mgr*)this);
	m_pDepartmentOrder->CenterWindow();
	m_pDepartmentOrder->ShowWindow(FALSE);



	/*
	if(m_nOpenMode<0)
	{
		//부서별 자료지정 
		CString strPURCHASE_SPECIES_KEY;
		CString stPURCHASE_MANAGE_KEY;
		stPURCHASE_MANAGE_KEY = m_pDM->GetCellData(_T("SP_구입관리KEY"),0);

		//m_pDepartmentOrder->m_strPURCHASE_SPECIES_KEY = strPURCHASE_SPECIES_KEY;
		m_pDepartmentOrder->m_strPURCHASE_MANAGE_KEY = stPURCHASE_MANAGE_KEY;

	}

	m_pDepartmentOrder->Create((CESL_Mgr*)this);
	m_pDepartmentOrder->CenterWindow();
	m_pDepartmentOrder->ShowWindow(FALSE);

	//수정시일 경우는 부서별 입수 책수가 존재할 경우 
	//입수 책수를 수정 하지 못하도록 한다
	if(m_nOpenMode<0)
	{
		m_pDepartmentOrder->GetTotalAcqExpBookCnt();
		if(m_pDepartmentOrder->m_nTotalAcqExpBookCnt>0)
		{
			CString strCnt;
			CEdit* pEdit = (CEdit*)m_pCM->FindControl(_T("SP_입수책수"));
			pEdit->EnableWindow(FALSE);
			
		}
	}
	*/


}

VOID CSePurchaseForm::OnbtnPURDEPARTMNG() 
{
	// TODO: Add your control notification handler code here
	if(!m_pDepartmentOrder) return;
	

	//부서별 자료지정 
	CString strPURCHASE_SPECIES_KEY;
	CString stPURCHASE_MANAGE_KEY;
	stPURCHASE_MANAGE_KEY = m_pDM->GetCellData(_T("SP_구입관리KEY"),0);

	m_pDepartmentOrder->m_strPURCHASE_SPECIES_KEY = strPURCHASE_SPECIES_KEY;
	m_pDepartmentOrder->m_strPURCHASE_MANAGE_KEY = stPURCHASE_MANAGE_KEY;
	

	if(m_bIsDepartMngFirstShow) 
	{
		m_pDepartmentOrder->RefreshAll();
		m_bIsDepartMngFirstShow = FALSE;

	}


	INT nAcqExpBookCnt = m_pDepartmentOrder->GetTotalAcqExpBookCnt();
	m_pDepartmentOrder->ShowWindow(TRUE);
}

VOID CSePurchaseForm::SaveDepartmentOrderInfo()
{
	if(!m_pDepartmentOrder) return;
	
	//부서별 자료지정 
	CString strPURCHASE_SPECIES_KEY;
	CString stPURCHASE_MANAGE_KEY;
	stPURCHASE_MANAGE_KEY = m_pDM->GetCellData(_T("SP_구입관리KEY"),0);

	m_pDepartmentOrder->m_strPURCHASE_SPECIES_KEY = strPURCHASE_SPECIES_KEY;
	m_pDepartmentOrder->m_strPURCHASE_MANAGE_KEY = stPURCHASE_MANAGE_KEY;


	if(m_pDepartmentOrder->m_nOpenMode>0)
	{
		INT nAcqExpBookCnt = m_pDepartmentOrder->GetTotalAcqExpBookCnt(FALSE);
		if(0 == nAcqExpBookCnt)
		{
			INT nACQ_BOOK_CNT = 1;	
			CString strACQ_BOOK_CNT;
			m_pCM->GetControlMgrData(_T("SP_입수책수"),strACQ_BOOK_CNT);
			nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));

			m_pDepartmentOrder->InsertDefaultAcqBookCnt(nACQ_BOOK_CNT);
		}
	}


	INT ids = m_pDepartmentOrder->SaveAllData();

	//기증에서 구입으로 입력하는 경우
	if(m_nOpenMode != m_pDepartmentOrder->m_nOpenMode)
	{
		m_pDepartmentOrder->m_nOpenMode = m_nOpenMode;
	}

	
	m_pDepartmentOrder->RefreshAll();


	if(ids<0) AfxMessageBox(_T("부서지정주문정보 저장에 실패 했습니다!"));
}

VOID CSePurchaseForm::OnDepartmentBookCntChange(WPARAM w,LPARAM l)
{
	if(!m_pDepartmentOrder) return;

	INT nAcqExpBookCnt = m_pDepartmentOrder->GetTotalAcqExpBookCnt();
	CString strCnt;
	CEdit* pEdit = (CEdit*)m_pCM->FindControl(_T("SP_입수책수"));
	

	if(0 == nAcqExpBookCnt)
	{
		pEdit->EnableWindow(TRUE);
	}
	else
	{
		strCnt.Format(_T("%d"),nAcqExpBookCnt);
		m_pCM->SetControlMgrData(_T("SP_입수책수"),strCnt);
		pEdit->EnableWindow(FALSE);
	}

}
