// SeDepartmentOrder.cpp : implementation file
//

#include "stdafx.h"
#include "SeDepartmentOrder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeDepartmentOrder dialog


CSeDepartmentOrder::CSeDepartmentOrder(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeDepartmentOrder)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;

	m_nOpenMode = 1; 
	m_nOpenLockMode = 1; 

	m_strPURCHASE_SPECIES_KEY = _T("");
	m_strPURCHASE_MANAGE_KEY = _T("");

	m_nCurrentIdx = -1;
	m_nTotalAcqExpBookCnt = 0;

	m_pParentMgr = pParent;

	m_bIsModifiable = TRUE;
}


VOID CSeDepartmentOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeDepartmentOrder)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeDepartmentOrder, CDialog)
	//{{AFX_MSG_MAP(CSeDepartmentOrder)
	ON_BN_CLICKED(IDC_btnDepOrderInsert, OnbtnDepOrderInsert)
	ON_BN_CLICKED(IDC_btnDepOrderUpdate, OnbtnDepOrderUpdate)
	ON_BN_CLICKED(IDC_btnDepOrderDelete, OnbtnDepOrderDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeDepartmentOrder message handlers

BOOL CSeDepartmentOrder::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("연속_수서_부서지정자료"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_부서지정자료") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_연속_수서_부서지정자료"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	m_pDM = FindDM(_T("DM_연속_수서_부서지정자료"));
	

	if(!m_pDM || !m_pCM || !m_pGrid) {
		AfxMessageBox( _T("DM,CM,grid Error : 연속_수서_부서지정자료") );
		return FALSE;
	}	

	/*
	if(m_nOpenMode<0)
	{
		RefreshAll();
		GetTotalAcqExpBookCnt();
	}
	*/

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSeDepartmentOrder::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}

VOID CSeDepartmentOrder::RefreshAll()
{
	
	if(m_nOpenMode>0) return;

	CString strQuery = _T("");

	if(1 == m_nOpenLockMode)
	{
		strQuery.Format(_T("PURCHASE_SPECIES_KEY = %s"),m_strPURCHASE_SPECIES_KEY);
	}
	else if(-1 == m_nOpenLockMode)
	{
		strQuery.Format(_T("PURCHASE_MANAGE_KEY = %s"),m_strPURCHASE_MANAGE_KEY);
	}
	
	m_pDM->RefreshDataManager(strQuery);

	INT nRowCnt = m_pDM->GetRowCount();

	if(nRowCnt>0) {
		m_nCurrentIdx = 0;
		m_pGrid->SetReverse(m_nCurrentIdx);
		ShowAllData();
	}

	if(!m_bIsModifiable)
	{
		HideOperationControls();
	}

}

VOID CSeDepartmentOrder::OnbtnDepOrderInsert() 
{
	// TODO: Add your control notification handler code here
	
	
	if(VerifyData()<0) return;

	m_pDM->InsertRow(-1);
	INT row = m_pDM->GetRowCount()-1;
	SetAllData(row, _T("I")); //insert

}

VOID CSeDepartmentOrder::OnbtnDepOrderUpdate() 
{
	// TODO: Add your control notification handler code here
	
	if( 0 == m_pDM->GetRowCount() || m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return;
	}

	if(VerifyData(FALSE)<0) return;

	CString strUDF;
	strUDF = m_pDM->GetCellData(_T("UDF_상태"),m_nCurrentIdx);

	if(_T("I") == strUDF)
	{
		SetAllData(m_nCurrentIdx, _T("I")); //insert
	}
	else 
	{
		SetAllData(m_nCurrentIdx, _T("U")); //update
	}


}

VOID CSeDepartmentOrder::OnbtnDepOrderDelete() 
{
	// TODO: Add your control notification handler code here
	m_pDM->SetCellData(_T("UDF_삭제여부"),_T("Y"),m_nCurrentIdx);

	// color refresh
	RefreshGridColor();

}

VOID CSeDepartmentOrder::SetAllData(INT row, CString strUDF)
{
	
	const INT cnt = 3;
	CString alias[cnt];
	CString data[cnt];


	alias[0] = _T("부서코드");
	alias[1] = _T("입수예정책수");
	alias[2] = _T("우선순위");


	for(INT i=0;i<cnt;i++)
	{
		m_pCM->GetControlMgrData(alias[i],data[i]);
	}

	m_pDM->SetCellData(alias,3,data,row);
	

	//UDF처리 
	m_pDM->SetCellData(_T("UDF_부서코드"),data[0],row);
	m_pDM->SetCellData(_T("UDF_상태"),strUDF,row);

	m_nCurrentIdx =	row;
	ShowAllData();
	
}


INT CSeDepartmentOrder::ShowAllData()
{
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("자료실구분"),_T("UDF_부서코드"));
	m_pCM->AllControlDisplay(m_nCurrentIdx);
	m_pGrid->SetReverse(m_nCurrentIdx);

	return 0;
}

BEGIN_EVENTSINK_MAP(CSeDepartmentOrder, CDialog)
    //{{AFX_EVENTSINK_MAP(CSeDepartmentOrder)
	ON_EVENT(CSeDepartmentOrder, IDC_gridDepOrderList, -600 /* Click */, OnClickgridDepOrderList, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSeDepartmentOrder::OnClickgridDepOrderList() 
{
	// TODO: Add your control notification handler code here
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		if(m_pDM->GetRowCount()>0)
		{
			m_nCurrentIdx = row-1;
			ShowAllData();

			// color refresh
			RefreshGridColor();

		}
	
	}
}

VOID CSeDepartmentOrder::RefreshGridColor()
{
	INT nRowCnt = m_pDM->GetRowCount();
	
	CString strDeleteYn;
	
	for(INT i=0;i<nRowCnt;i++)
	{
		strDeleteYn = _T("");
		strDeleteYn = m_pDM->GetCellData(_T("UDF_삭제여부"),i);
		
		if(_T("Y") == strDeleteYn)
		{
			SetGridRowUniqueColor(i);
		}

	}

}

VOID CSeDepartmentOrder::SetGridRowUniqueColor(INT nRow)
{
	COLORREF red, white, black;
	
	red = RGB(251,196,234);
	white = RGB(255,255,255);
	black = RGB(0,0,0);

	INT nCol = m_pGrid->GetCol();
	INT nCols = m_pGrid->GetCols(0);
	
	if (nRow >= 0)
	{
		m_pGrid->SetRow(nRow + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			m_pGrid->SetCol(i);
			m_pGrid->SetCellBackColor(red);
			m_pGrid->SetCellForeColor(black);
		}
	}
	
	m_pGrid->SetCol(nCol);
}

INT CSeDepartmentOrder::SaveAllData()
{
	// 일괄저장함수
	CString strUDF;
	CString strDeleteYN;
	CString strRecKey;
	CString strQuery;
	CArray<CString,CString> RemoveAliasList;

	INT nRowCnt = m_pDM->GetRowCount();
	
	if(0 == nRowCnt) return 0;

	m_pDM->StartFrame();


	for(INT i=0;i<nRowCnt;i++)
	{

		strUDF = m_pDM->GetCellData(_T("UDF_상태"),i);
		strDeleteYN = m_pDM->GetCellData(_T("UDF_삭제여부"),i);
		strRecKey = m_pDM->GetCellData(_T("부서별주문KEY"),i);		

		if(_T("Y") == strDeleteYN) //삭제 
		{
			if(_T("I") == strUDF) continue;
			
			strQuery.Format(_T("DELETE FROM SE_DEPARTMENT_ORDER_TBL WHERE REC_KEY = %s;"),strRecKey);

			m_pDM->InitDBFieldData();
			m_pDM->AddFrame(strQuery);
			
		}
		


		if(_T("I") == strUDF) //입력
		{
			
			m_pDM->MakeRecKey(strRecKey);
			m_pDM->SetCellData(_T("부서별주문KEY"),strRecKey,i);
			m_pDM->SetCellData(_T("구입종KEY"),m_strPURCHASE_SPECIES_KEY,i);
			m_pDM->SetCellData(_T("구입정보KEY"),m_strPURCHASE_MANAGE_KEY,i);

	
			RemoveAliasList.RemoveAll();
			RemoveAliasList.Add(_T("UDF_부서코드"));
			m_pDM->InitDBFieldData();
			m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("SE_DEPARTMENT_ORDER_TBL"), i , m_pDM);

		}
		else //수정
		{
			RemoveAliasList.RemoveAll();
			RemoveAliasList.Add(_T("UDF_부서코드"));

			m_pDM->InitDBFieldData();
			m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("SE_DEPARTMENT_ORDER_TBL"), i , m_pDM, _T("REC_KEY"), _T("NUMERIC"), strRecKey);

		}

	}

	INT ids = m_pDM->SendFrame();
	m_pDM->EndFrame();
	
	if(ids>=0) RefreshAll();

	return ids;
	
	/*
	if(ids<0)
	{
		AfxMessageBox(_T("저장에 실패하였습니다!"));
	}
	else
	{
		AfxMessageBox(_T("저장하였습니다!"));
		RefreshAll();
	}
	*/
}

VOID CSeDepartmentOrder::OnCancel() 
{
	// TODO: Add extra cleanup here

	:: SendMessage(	m_pParentMgr->GetSafeHwnd(), BOOKCNTCHANGE , 0, 0);


	CDialog::OnCancel();
}

INT CSeDepartmentOrder::GetTotalAcqExpBookCnt(BOOL bAllCnt)
{

	//총 입수책수 계산
	m_nTotalAcqExpBookCnt = 0;
	CString strAcqExpBookCnt;
	CString strPurchaseSpeciesKey;
	INT nACqExpBookCnt = 0;
	
	INT nRowCnt = m_pDM->GetRowCount();
	CString strDeleteYN;

	if(nRowCnt>0)
	{
		for(INT i=0;i<nRowCnt;i++)
		{
			strDeleteYN = m_pDM->GetCellData(_T("UDF_삭제여부"),i);
			strPurchaseSpeciesKey = m_pDM->GetCellData(_T("구입종KEY"),i);

			if(_T("Y") == strDeleteYN) continue;
			
			if(m_nOpenMode<0)
			{
				if(!bAllCnt)
				{
					if(strPurchaseSpeciesKey.IsEmpty()) continue;

				}
			}

			strAcqExpBookCnt = m_pDM->GetCellData(_T("입수예정책수"),i);
			nACqExpBookCnt = _ttoi(strAcqExpBookCnt.GetBuffer(0));
			m_nTotalAcqExpBookCnt += nACqExpBookCnt;
		}
	
	}

	return m_nTotalAcqExpBookCnt;
}

INT CSeDepartmentOrder::VerifyData(BOOL bInsert)
{
	
	CString strDepartCode;
	m_pCM->GetControlMgrData(_T("부서코드"),strDepartCode);

	
	INT nRow = m_pDM->GetRowCount();
	CString strExistingCode;

	for(INT i=0;i<nRow;i++)
	{
		strExistingCode = m_pDM->GetCellData(_T("부서코드"),i);

		if(strExistingCode == strDepartCode)
		{
			if(!bInsert) 
			{
				if(m_nCurrentIdx == i) continue;
			}

			AfxMessageBox(_T("동일한 부서에 대한 정보가 입력되어 있습니다!")); 
			return -1;
		}
	}

	CString strAcqExpBookCnt;
	m_pCM->GetControlMgrData(_T("입수예정책수"),strAcqExpBookCnt);
	
	if(strAcqExpBookCnt.IsEmpty())
	{
		AfxMessageBox(_T("입수예정책수가 입력되지 않았습니다!")); 
		return -1;		

	}
	else
	{
		INT nCount = 0;
		nCount = _ttoi(strAcqExpBookCnt.GetBuffer(0));
		if(0 == nCount) 
		{
			AfxMessageBox(_T("입수예정책수는 0 이 될 수 없습니다!")); 
			return -1;
		}

	}

	CString strPriority;
	m_pCM->GetControlMgrData(_T("우선순위"),strPriority);
	
	if(strPriority.IsEmpty())
	{
		AfxMessageBox(_T("우선순위가 입력되지 않았습니다!")); 
		return -1;		

	}


	return 0;
}

VOID CSeDepartmentOrder::HideOperationControls()
{
	
	CWnd* pWnd = NULL;

	pWnd = GetDlgItem(IDC_btnDepOrderInsert);
	if(pWnd) pWnd->ShowWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_btnDepOrderUpdate);
	if(pWnd) pWnd->ShowWindow(FALSE);	

	pWnd = GetDlgItem(IDC_btnDepOrderDelete);
	if(pWnd) pWnd->ShowWindow(FALSE);

}