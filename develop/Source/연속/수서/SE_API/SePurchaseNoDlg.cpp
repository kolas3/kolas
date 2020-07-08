// SePurchaseNoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SePurchaseNoDlg.h"
#include "SePurchaseNoInsertDlg.h"
#include "..\..\..\공통\BO_ACQ_GROUP_MANAGER\GroupCollection.h"
#include "..\..\..\공통\BO_ACQ_GROUP_MANAGER\BO_BudgetMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseNoDlg dialog


CSePurchaseNoDlg::CSePurchaseNoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSePurchaseNoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSePurchaseNoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_strYear = _T("");
	m_strPurchaseNo = _T("");
	m_strWorking_status = _T("");
	m_nCurrentIdx = -1;
	m_nOpenMode = 1; //default
}
CSePurchaseNoDlg::~CSePurchaseNoDlg()
{
}


VOID CSePurchaseNoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSePurchaseNoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSePurchaseNoDlg, CDialog)
	//{{AFX_MSG_MAP(CSePurchaseNoDlg)
	ON_BN_CLICKED(IDC_btnPUR_NO_MGR_SEARCH, OnbtnPURNOMGRSEARCH)
	ON_BN_CLICKED(IDC_btnPUR_NO_MGR_INSERT, OnbtnPURNOMGRINSERT)
	ON_BN_CLICKED(IDC_btnPUR_NO_MGR_DELETE, OnbtnPURNOMGRDELETE)
	ON_BN_CLICKED(IDC_btnBUDGET_EXECUTION, OnbtnBUDGETEXECUTION)
	ON_BN_CLICKED(IDC_btnBUDGET_CANCEL, OnbtnBUDGETCANCEL)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSePurchaseNoDlg message handlers

BOOL CSePurchaseNoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("연속_수서_차수번호"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_차수번호") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_연속_차수번호"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	m_pDM = FindDM(_T("DM_연속_차수번호"));
	

	if(!m_pDM || !m_pCM || !m_pGrid) {
		AfxMessageBox( _T("DM,CM,grid Error : 연속_수서_차수번호") );
		return FALSE;
	}	
	
	//자료상태를 설정 해준다.
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_NO_MGR_STATUS);
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("구입대상") );
		pCombo->InsertString( 1, _T("주문중") );
		pCombo->InsertString( 2, _T("전체") );
		pCombo->SetCurSel( 0 );
	}

	//초기 정보를 보여 준다
	RefreshAll();
	


	//구입종 입력과 검색 외에는 _T("적용")버튼을 보이지 않게 한다
	//if(-1 == m_nOpenMode) GetDlgItem(IDOK)->ShowWindow(FALSE);
	//if(m_nOpenMode<0) GetDlgItem(IDOK)->ShowWindow(FALSE);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSePurchaseNoDlg::OnbtnPURNOMGRSEARCH() 
{
	// TODO: Add your control notification handler code here
	RefreshAll();

}

//=================================================================
// [함수설명]
//	기초[자료상태 0] 연속 차수정보를 보여준다.
// [MINI SPEC]
//  1. 컨트롤로 부터 차수년도를 가져와 쿼리 조건을 구성한다.
//  2. 컨트롤로 부터 작업상를 가져와 쿼리 조건을 구성한다.
//  3. DM을 구성하여 연속 차수정보를 보여준다. 
//  4. 예산관련 버튼 사용여부에 따라 버튼과 그리드를 숨긴다.
//=================================================================
VOID CSePurchaseNoDlg::RefreshAll()
{
	//-------------------------------------------------------------
	// 1. 컨트롤로 부터 차수년도를 가져와 쿼리 조건을 구성한다.
	//-------------------------------------------------------------
	CString strYEAR;
	m_pCM->GetControlMgrData(_T("차수년도"),strYEAR);
	
	if(strYEAR.IsEmpty())
	{
		AfxMessageBox(_T("차수년도가 입력되지 않았습니다!"));
		return;
	}	
	
	CString strQuery = _T("");
	CString strtmpquery = _T("");
	strtmpquery.Format(_T("N.ACQ_YEAR = '%s'"),strYEAR);
	strQuery += strtmpquery;

	//-------------------------------------------------------------
	// 2. 컨트롤로 부터 작업상를 가져와 쿼리 조건을 구성한다.
	//-------------------------------------------------------------
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbPUR_NO_MGR_STATUS);
	INT nCurSel = 0; 
	if(pCombo) nCurSel = pCombo->GetCurSel();
	
	if(nCurSel != 2) //전체 
	{
		strQuery += _T(" AND ");
		strtmpquery.Format(_T("N.WORKING_STATUS = '%d'"),nCurSel);
		strQuery += strtmpquery;
	}

	//-------------------------------------------------------------
	// 3. DM을 구성하여 연속 차수정보를 보여준다. 
	//-------------------------------------------------------------
	m_pDM->RefreshDataManager(strQuery);
	
	CString strWORKING_STATUS;
	
	for(INT i=0;i<m_pDM->GetRowCount();i++)
	{
		strWORKING_STATUS = _T("");
		strWORKING_STATUS = m_pDM->GetCellData(_T("작업상태"),i);
		if(_T("0") == strWORKING_STATUS) m_pDM->SetCellData(_T("UDF_작업상태"),_T("구입대상"),i);
		else if(_T("1") == strWORKING_STATUS)  m_pDM->SetCellData(_T("UDF_작업상태"),_T("주문중"),i);
	}

	m_pGrid->Display();

	//-------------------------------------------------------------------
	// 2005-10-11 황정영: 예산관련 버튼및 그리드컬럼을 사용여부에 따라 버튼을 숨긴다.
	//-------------------------------------------------------------------
	CString sBudgetYN;
	m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'BUDGETMNG'"), sBudgetYN);
	if(sBudgetYN == _T("N"))
	{
		GetDlgItem(IDC_btnBUDGET_EXECUTION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnBUDGET_CANCEL)->ShowWindow(SW_HIDE);
		//========================================
		//20080606 UPDATE BY PJW : 버튼위치수정
		GetDlgItem(IDC_btnPUR_NO_MGR_SEARCH)->MoveWindow(330,17,70,22);
		GetDlgItem(IDC_btnPUR_NO_MGR_INSERT)->MoveWindow(420,17,70,22);
		GetDlgItem(IDC_btnPUR_NO_MGR_DELETE)->MoveWindow(500,17,70,22);
		GetDlgItem(IDC_btnBUDGET_EXECUTION)->MoveWindow(420,47,70,22);
		GetDlgItem(IDC_btnBUDGET_CANCEL)->MoveWindow(500,47,70,22);
		//========================================


		m_pGrid->SetColWidth(1,0,600);
		m_pGrid->SetColWidth(4,0,2200);
		m_pGrid->SetColWidth(8,0,0);
	}
}

//=================================================================
// [함수설명]
//	입력버튼으로 차수입력화면을 부른다.
// [MINI SPEC]
//  1. 차수년도 검사
//  2. 차수입력 다이얼로그를 생성하고 해당 차수년도를 세팅한다.
//=================================================================
VOID CSePurchaseNoDlg::OnbtnPURNOMGRINSERT() 
{
	//-------------------------------------------------------------
	// 1. 차수년도 검사
	//-------------------------------------------------------------
	CString strYEAR;
	m_pCM->GetControlMgrData(_T("차수년도"),strYEAR);
	
	if(strYEAR.IsEmpty())
	{
		AfxMessageBox(_T("차수년도가 입력되지 않았습니다!"));
		return;
	}	
	
	//-------------------------------------------------------------
	// 2. 차수입력 다이얼로그를 생성하고 해당 차수년도를 세팅한다.
	//-------------------------------------------------------------
	CSePurchaseNoInsertDlg dlg(this);
	dlg.m_strYEAR = strYEAR;
	
	if(dlg.DoModal() != IDOK) return;

	RefreshAll();

}

//=================================================================
// [함수설명]
//	선택된 차수를 적용한다.
// [MINI SPEC]
//  1. 차수를 선택했는지 검사
//  2. 작업상태가 구입대상인 상태인지 검사한다.
//  3. 선택된 차수의 차수년도/차수번호/작업상태정보를 멤버에 저장
//=================================================================
VOID CSePurchaseNoDlg::OnOK() 
{
	// TODO: Add extra validation here

	//-------------------------------------------------------------
	// 1. 차수를 선택했는지 검사
	//-------------------------------------------------------------
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return;
	}


	//-------------------------------------------------------------
	// 2. 작업상태가 구입대상인 상태인지 검사한다.
	//-------------------------------------------------------------
	CString strWORKING_STATUS = _T("");
	strWORKING_STATUS = m_pDM->GetCellData(_T("작업상태"),m_nCurrentIdx);

	
	if(m_nOpenMode>0)
	{
		if(strWORKING_STATUS != _T("0"))
		{
			AfxMessageBox(_T("이미 주문중 상태인 차수는 적용할 수 없습니다!"));
			return;
		}
	}

	//-------------------------------------------------------------
	// 3. 선택된 차수의 차수년도/차수번호/작업상태정보를 멤버에 저장
	//-------------------------------------------------------------
	m_strYear = m_pDM->GetCellData(_T("차수년도"),m_nCurrentIdx);
	m_strPurchaseNo = m_pDM->GetCellData(_T("차수번호"),m_nCurrentIdx);
	m_strWorking_status = m_pDM->GetCellData(_T("작업상태"),m_nCurrentIdx);

	if(m_strYear.IsEmpty() || m_strPurchaseNo.IsEmpty())
	{
		AfxMessageBox(_T("차수 정보를 찾을 수 없습니다!"));
		m_strYear = _T("");
		m_strPurchaseNo = _T("");
		return;
	}


	CDialog::OnOK();
}

BEGIN_EVENTSINK_MAP(CSePurchaseNoDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSePurchaseNoDlg)
	ON_EVENT(CSePurchaseNoDlg, IDC_gridPUR_NO_MGR, -600 /* Click */, OnClickgridPURNOMGR, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSePurchaseNoDlg::OnClickgridPURNOMGR() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pDM->GetRowCount()) return;

	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		m_nCurrentIdx = row-1;
	}
}

//=================================================================
// [함수설명]
//  차수를 삭제한다.
// [MINI SPEC]
//  1. 선정된 차수정보를 구성한다.
//  2. 예산관리 사용여부를 구한다.
//  3. 선정된 차수들의 삭제는 아래쪽 부터 한다. (그래야 DM row와 매칭이 된다.)
//  4. 예산집행이 되어있는 차수라면 집행 취소가 선행되어야 한다.
//  5. 차수내 종이 있으면 삭제되지 않는다.
//=================================================================
VOID CSePurchaseNoDlg::OnbtnPURNOMGRDELETE() 
{
	
	/*// TODO: Add your control notification handler code here
	if(0 == m_pDM->GetRowCount() || m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return;
	}
	
	CString strSpeciesNo;
	INT nSpeciesNo = 0;
	strSpeciesNo = m_pDM->GetCellData(_T("UDF_종수"),m_nCurrentIdx);
	if(!strSpeciesNo.IsEmpty()) nSpeciesNo = _ttoi(strSpeciesNo.GetBuffer(0));

	if(nSpeciesNo != 0)
	{
		AfxMessageBox(_T("해당 차수에 존재하는 종이 있어 삭제 할 수 없습니다!"));
		return;
	}
	
	//차수번호
	CString strPurchaseNo;
	strPurchaseNo = m_pDM->GetCellData(_T("차수번호"),m_nCurrentIdx);
	

	//차수년도
	CString strYear;
	strYear = m_pDM->GetCellData(_T("차수년도"),m_nCurrentIdx);
	

	CString msg;
	msg.Format(_T("[ %s ] 년도의 [ %s ] 차수를 삭제하시겠습니까?"),strYear,strPurchaseNo);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return;


	CString strREC_KEY;
	strREC_KEY = m_pDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);
	
	CString strQuery;
	m_pDM->StartFrame();
	
	strQuery.Format(_T("DELETE FROM SE_PURCHASE_SEQ_NO_TBL WHERE REC_KEY = %s;"),strREC_KEY);
	m_pDM->AddFrame(strQuery);

	strQuery.Format(_T("DELETE FROM SE_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY = %s;"),strREC_KEY);
	m_pDM->AddFrame(strQuery);
	
	INT ids = m_pDM->SendFrame();
	m_pDM->EndFrame();
	
	if(ids<0)
	{
		AfxMessageBox(_T("삭제에 실패하였습니다!"));
	}
	else
	{
		AfxMessageBox(_T("삭제하였습니다!"));
		m_pDM->DeleteRow(m_nCurrentIdx);
		m_pGrid->Display();
	}
	*/
	//2005-10-20 황정영: 1. 차수를 선정해야 처리될 수 있도록 변경
	//                   2. 예산집행 한 차수에 대한 처리..

	//-------------------------------------------------------------
	// 1. 선정된 차수정보를 구성한다.
	//-------------------------------------------------------------
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	
	CString strSpeciesNo, strBudgetYN, msg;
	INT nSpeciesNo = 0;
	INT nCount = 0;

	//-------------------------------------------------------------
	// 2. 예산관리 사용여부를 구한다.
	//-------------------------------------------------------------
	m_pDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'BUDGETMNG'"),strBudgetYN);

	//-------------------------------------------------------------
	// 3. 선정된 차수들의 삭제는 아래쪽 부터 한다. (그래야 DM row와 매칭이 된다.)
	//-------------------------------------------------------------
	INT i = m_pGrid->SelectGetTailPosition();
	
	INT nRows = m_pGrid->SelectGetCount();
	for(INT j = 0; j<nRows ; j++)
	{
		//차수번호
		CString strPurchaseNo;
		strPurchaseNo = m_pDM->GetCellData(_T("차수번호"),i);
		
		//차수년도
		CString strYear;
		strYear = m_pDM->GetCellData(_T("차수년도"),i);
	
		CString strData;
		//-------------------------------------------------------------
		// 4. 예산집행이 되어있는 차수라면 집행 취소가 선행되어야 한다.
		//-------------------------------------------------------------
		if(strBudgetYN == _T("Y"))
		{
			m_pDM->GetCellData(_T("UDF_예산집행상태"),i,strData);
			if( strData == _T("집행"))
			{
				msg.Format(_T("[ %s ]년도 [ %s ]차수는 예산집행 취소 후 삭제가능합니다."), strYear, strPurchaseNo);
				ESLAfxMessageBox(msg);
				i = m_pGrid->SelectGetPrev();
				continue;
			}
		}

		//-------------------------------------------------------------
		// 5. 차수내 종이 있으면 삭제되지 않는다.
		//-------------------------------------------------------------
		strSpeciesNo = m_pDM->GetCellData(_T("UDF_종수"),i);

		if(!strSpeciesNo.IsEmpty()) nSpeciesNo = _ttoi(strSpeciesNo.GetBuffer(0));

		if(nSpeciesNo != 0)
		{
			msg.Format(_T("[ %s ]년도 [ %s ]차수는 존재하는 종이 있어 삭제할 수 없습니다."), strYear, strPurchaseNo);
			AfxMessageBox(msg);
			i = m_pGrid->SelectGetPrev();
			continue;
		}
		
		msg.Format(_T("[ %s ] 년도의 [ %s ] 차수를 삭제하시겠습니까?"),strYear,strPurchaseNo);
		if(AfxMessageBox(msg,MB_YESNO) != IDYES) 
		{
			i = m_pGrid->SelectGetPrev();
			continue;
		}
		
		CString strREC_KEY;
		strREC_KEY = m_pDM->GetCellData(_T("REC_KEY"),i);
		
		CString strQuery;
		m_pDM->StartFrame();
		
		strQuery.Format(_T("DELETE FROM SE_PURCHASE_SEQ_NO_TBL WHERE REC_KEY = %s;"),strREC_KEY);
		m_pDM->AddFrame(strQuery);

		strQuery.Format(_T("DELETE FROM SE_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY = %s;"),strREC_KEY);
		m_pDM->AddFrame(strQuery);
		
		ids = m_pDM->SendFrame();
		m_pDM->EndFrame();
		
		if(ids<0)
		{
			msg.Format(_T("[ %s ] 년도의 [ %s ] 차수를 삭제 실패했습니다."),strYear,strPurchaseNo);
			AfxMessageBox(msg);
		}
		else
		{
			m_pDM->DeleteRow(i);
			nCount++;
		}

		i = m_pGrid->SelectGetPrev();
	}
	
	if(nCount > 0)
	{
		msg.Format(_T("선정한 [ %d ]개 중에 [ %d ]개의 차수를 삭제하였습니다."), nRows, nCount);
		ESLAfxMessageBox(msg);
		m_pGrid->Display();
	}
}

//=================================================================
// [함수설명]
//  선택한 차수를 예산 집행한다.
// [MINI SPEC]
//  1.선정 정보를얻는다.
//  2. 예산집행상태를 얻어 집행 여부를 결정한다.
//=================================================================
VOID CSePurchaseNoDlg::OnbtnBUDGETEXECUTION() 
{
	//-------------------------------------------------------------
	// 1.선정 정보를얻는다.
	//-------------------------------------------------------------
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}

	if ( m_pGrid->SelectGetCount() > 1 ) 
	{
		AfxMessageBox ( _T("한개의 차수만 선정해 주세요..") );
		return;
	}

	// 예산집행 상태 받아옴
	CString strData;
	m_pCM->GetControlMgrData(_T("예산집행상태"), strData);

	CArray<INT, INT> ArrIdx;	ArrIdx.RemoveAll();
	CGroupCollection GroupCollection(this);	GroupCollection.RemoveAll();
	GroupCollection.SetAcqType(PURCHASE);
	CString strMsg;


	INT i = m_pGrid->SelectGetHeadPosition();

//	if ( m_pGrid->SelectGetCount() > 1 ) 
//	{
//		AfxMessageBox ( _T("한개의 차수만 선정해 주세요..") );
//		return;
//	}

	while(i >= 0) 
	{
		ids = GroupCollection.AddGroupInfo(
				m_pDM->GetCellData(_T("차수년도"), i),
				m_pDM->GetCellData(_T("차수번호"), i));
		if(ids < 0) return ;
		ArrIdx.Add(i);
		i = m_pGrid->SelectGetNext();
	}

	if(GroupCollection.GetGroupCount() == 0) return ;

	m_pDM->StartFrame();
	CBO_BudgetMgr BudgetMgr(this);
	BudgetMgr.SetDMOut(m_pDM);
	BudgetMgr.SetGroupCollection(&GroupCollection);
	BudgetMgr.SetExecuteType(_T("1"));
	BudgetMgr.SetMode(_T("SE"));


	BUDGET_ERROR_TYPE res;
	res = BudgetMgr.MakeBudgetExecutionInfo();	
	// 2005-01-06 BUDGET_IGNORE일 경우에는 예산집행 취소이므로 리턴한다.
	// if(res != BUDGET_SUCCESS && res != BUDGET_IGNORE)
	if(res != BUDGET_SUCCESS)
		return ;

	ids = m_pDM->SendFrame();
	if(ids < 0) return ;
	ids = m_pDM->EndFrame();
	if(ids < 0) return ;

	for(i=0 ; i<ArrIdx.GetSize() ; i++)
	{
		m_pDM->SetCellData(_T("예산집행정보KEY"), BudgetMgr.GetBudgetExeKey(), ArrIdx.GetAt(i));
		m_pDM->SetCellData(_T("예산집행상태"), strData, ArrIdx.GetAt(i));
		if ( BudgetMgr.GetExecuteType() == _T("1") )
			m_pDM->SetCellData(_T("UDF_예산집행상태"), _T("집행"), ArrIdx.GetAt(i));
		else
			m_pDM->SetCellData(_T("UDF_예산집행상태"), _T("미집행"), ArrIdx.GetAt(i));

		m_pGrid->DisplayLine(ArrIdx.GetAt(i++));
	}

	AfxMessageBox ( _T("완료되었습니다.") );
	return ;
	
}

//=================================================================
// [함수설명]
//  선정된 차수를 예산집행 취소한다.
// [MINI SPEC]
//  1.선정 정보를얻는다.
//=================================================================
VOID CSePurchaseNoDlg::OnbtnBUDGETCANCEL() 
{
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}

	CArray<INT, INT> ArrIdx;	ArrIdx.RemoveAll();
	CGroupCollection GroupCollection(this);	GroupCollection.RemoveAll();
	GroupCollection.SetAcqType(PURCHASE);
	CString strMsg;
	INT i = m_pGrid->SelectGetHeadPosition();
	while(i >= 0) 
	{
		if(!m_pDM->GetCellData(_T("예산집행정보KEY"), i).IsEmpty())
		{
			ids = GroupCollection.AddGroupInfo(
					m_pDM->GetCellData(_T("차수년도"), i),
					m_pDM->GetCellData(_T("차수번호"), i),
					m_pDM->GetCellData(_T("업무구분"), i));
			if(ids < 0) return ;
			ArrIdx.Add(i);
		}
		else
		{
			strMsg.Format(
				_T("[%s:%s]차수는 예산집행정보가 없습니다."),
				m_pDM->GetCellData(_T("차수년도"), i),
				m_pDM->GetCellData(_T("차수번호"), i));
			m_pGrid->SetReverse(i);
			ESLAfxMessageBox(strMsg);
		}
		i = m_pGrid->SelectGetNext();
	}

	if(GroupCollection.GetGroupCount() == 0) return ;

	m_pDM->StartFrame();
	CBO_BudgetMgr BudgetMgr(this);
	BudgetMgr.SetDMOut(m_pDM);
	BudgetMgr.SetMode(_T("SE"));
	BudgetMgr.SetGroupCollection(&GroupCollection);
	BUDGET_ERROR_TYPE res = BudgetMgr.DeleteBudgetExecutionInfo();
	if(res != BUDGET_SUCCESS && res != BUDGET_IGNORE)
		return ;
	
	ids = m_pDM->SendFrame();
	if(ids < 0) return ;
	ids = m_pDM->EndFrame();
	if(ids < 0) return ;

	for(i=0 ; i<ArrIdx.GetSize() ; i++)
	{
		m_pDM->SetCellData(_T("예산집행정보KEY"), BudgetMgr.GetBudgetExeKey(), ArrIdx.GetAt(i));
		m_pDM->SetCellData(_T("예산집행상태"), _T(""), ArrIdx.GetAt(i));
		m_pGrid->DisplayLine(ArrIdx.GetAt(i));
	}

	AfxMessageBox ( _T("정상적으로 예산집행이 취소되었습니다.") );
	RefreshAll();
	return ;
	
}

//=================================================================
// [함수설명]
//  선정된 차수들의 선정 정보를  위쪽부터 저장한다. 
//=================================================================
INT CSePurchaseNoDlg::CheckSelectDataValid()
{
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	if(m_pDM->GetRowCount() == 0) return -1;
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

HBRUSH CSePurchaseNoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
