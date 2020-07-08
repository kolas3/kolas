// BudgetListWork.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetListWork.h"
#include "BudgetApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBudgetListWork dialog


CBudgetListWork::CBudgetListWork(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBudgetListWork)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nOpenMode = 1; //default
	m_pCM = NULL;
	m_pDM = NULL;
	m_pDM_EXE_SUB = NULL;
	m_pGrid = NULL;
	m_strBUDGET_COMPILATION_KEY = _T("");
	m_nCurrentIdx = -1;
	m_pArrayDeleteKey = NULL;
	m_sBudgetTotal = _T("");
}
CBudgetListWork::~CBudgetListWork()
{
}

VOID CBudgetListWork::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBudgetListWork)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBudgetListWork, CDialog)
	//{{AFX_MSG_MAP(CBudgetListWork)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_MAT_INSERT, OnbtnBUDGETLISTMATINSERT)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_MAT_MODIFY, OnbtnBUDGETLISTMATMODIFY)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_MAT_DELETE, OnbtnBUDGETLISTMATDELETE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBudgetListWork message handlers

BEGIN_EVENTSINK_MAP(CBudgetListWork, CDialog)
    //{{AFX_EVENTSINK_MAP(CBudgetListWork)
	ON_EVENT(CBudgetListWork, IDC_gridBUDGET_LIST_MAT, -600 /* Click */, OnClickgridBUDGETLISTMAT, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBudgetListWork::OnClickgridBUDGETLISTMAT() 
{
	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		
		if(m_pDM->GetRowCount()>0)
		{
			m_nCurrentIdx = row-1;
			ShowAllData();
		}
	
	}
}

BOOL CBudgetListWork::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr( _T("예산관리_예산편성_자료별") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 예산관리_예산편성_자료별") );
		return false;
	}
	
	
	m_pCM = FindCM(_T("CM_예산편성_자료별"));
	m_pDM = FindDM(_T("DM_예산편성보조_자료별"));
	m_pDM_EXE_SUB = FindDM(_T("DM_예산집행보조"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));

	if(!m_pCM || !m_pDM || !m_pGrid || !m_pDM_EXE_SUB) {
		AfxMessageBox( _T("CM,DM,Grid Error : CM_예산편성_자료별,DM_예산편성보조_자료별,그리드,DM_예산집행보조") );
		return FALSE;
	}
	
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	if(InitOpenMode()<0)
	{
		AfxMessageBox(_T("InitOpenMode FAIL!"));
		return FALSE;
	}


	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CBudgetListWork::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CBudgetApi api(this);
	CString strTmpData1, strTmpData2;
	
	if(strDMFieldAlias == _T("남은할당액"))
	{	
		strTmpData1 = m_pDM->GetCellData(_T("UDF_남은할당액"),nRow);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData1, strTmpData2);
		m_pDM->SetCellData(_T("남은할당액"), strTmpData2, nRow);
		
	}
	else if(strDMFieldAlias == _T("현재사용액"))
	{
		strTmpData1 = m_pDM->GetCellData(_T("UDF_현재사용액"),nRow);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData1, strTmpData2);
		m_pDM->SetCellData(_T("현재사용액"), strTmpData2, nRow);		
	}
	else if(strDMFieldAlias == _T("UDF_예산할당액"))
	{	
		strTmpData1 = m_pDM->GetCellData(_T("예산할당액"),nRow);
		m_PriceConvertMgr.ConversionPriceInfoToDesc(strTmpData1, strTmpData2);
		m_pDM->SetCellData(_T("UDF_예산할당액"), strTmpData2, nRow);
	}

	return 0;
}

VOID CBudgetListWork::OnbtnBUDGETLISTMATINSERT() 
{
	//추가 함수 
	//DM에 UDF_상태를 _T("I")로 해서 추가한다
	INT ids = -1;

	CBudgetApi api(this);
	CString strAcount, strCode;
	m_pCM->GetControlMgrData(_T("예산할당액"),strAcount);
	if( m_nOpenMode != 1 )
	{
		ids = api.CheckCompilationAcount(1,m_pDM,strAcount,m_strBUDGET_COMPILATION_KEY,-1);
		if(ids < 0 )
			return;
	}
	m_pCM->GetControlMgrData(_T("자료구분"),strCode,-1);
	ids = api.CheckDupCode(m_pDM, strCode);
	if(ids < 0 )
		return;
	
	m_pDM->InsertRow(-1);
	INT row = m_pDM->GetRowCount()-1;
	SetAllData(row, _T("I")); //insert	
}

VOID CBudgetListWork::OnbtnBUDGETLISTMATMODIFY() 
{
	// TODO: Add your control notification handler code here
	// 2005-02-17 검색된 자료가 없으면 리턴한다.
	INT nCnt = m_pDM->GetRowCount();
	if(nCnt < 1)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return ;
	}

	CBudgetApi api(this);
	CString strAcount;
	m_pCM->GetControlMgrData(_T("예산할당액"),strAcount);
	if( m_nOpenMode != 1 )
	{
		INT ids = api.CheckCompilationAcount(0,m_pDM,strAcount,m_strBUDGET_COMPILATION_KEY, m_nCurrentIdx);
		if(ids < 0 )
			return;
	}	
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

VOID CBudgetListWork::OnbtnBUDGETLISTMATDELETE() 
{
	// TODO: Add your control notification handler code here
	// 2005-02-17 검색된 자료가 없으면 리턴한다.
	INT nCnt = m_pDM->GetRowCount();
	if(nCnt < 1)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return ;
	}


	CString strTmp;
	strTmp = m_pDM->GetCellData(_T("UDF_자료구분"),m_nCurrentIdx);

	CString msg;
	msg.Format(_T("자료구분이 [%s]인 예산할당을 삭제하겠습니까?"), strTmp);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return;


	CString strUDF;
	strUDF = m_pDM->GetCellData(_T("UDF_상태"),m_nCurrentIdx);

	if(_T("I") != strUDF) 
	{
		CString strREC_KEY;
		strREC_KEY = m_pDM->GetCellData(_T("REC_KEY"),m_nCurrentIdx);
		m_pArrayDeleteKey->Add(strREC_KEY); //update
	}
	
	m_pDM->DeleteRow(m_nCurrentIdx);

	// 2005-02-17 맨 마지막 데이터를 삭제한 경우에는 그 데이터가 없어졌으므로 그 윗줄을 Set한다.
	if( (m_nCurrentIdx != 0) && (nCnt == (m_nCurrentIdx+1)) )
		m_nCurrentIdx--;

	ShowAllData();
	
	
}


//////////

VOID CBudgetListWork::SetAllData(INT row, CString strUDF)
{
	const INT cnt = 2;
	CString alias[cnt];
	CString data[cnt];

	alias[0] = _T("이용대상구분");
	alias[1] = _T("예산할당액");


	for(INT i=0;i<cnt;i++)
	{
		m_pCM->GetControlMgrData(alias[i],data[i]);
	}

	m_pDM->SetCellData(_T("코드"),data[0],row);
	m_pDM->SetCellData(_T("UDF_이용대상구분"),data[0],row);
	m_pDM->SetCellData(_T("예산할당액"),data[1],row);
	m_pDM->SetCellData(_T("UDF_상태"),strUDF,row);
	m_pDM->SetCellData(_T("보조유형"),_T("W"),row);

	m_nCurrentIdx =	row;
	ShowAllData();
}

INT CBudgetListWork::RefreshAll()
{
	if(m_strBUDGET_COMPILATION_KEY.IsEmpty())
	{
		AfxMessageBox(_T("예산편성KEY를 찾을 수 없습니다!"));
		return -1;
	}


	CString strWhere;
	strWhere.Format(_T("BUDGET_COMPILATION_KEY = %s AND CLASS_KIND = 'USE' AND SUB_TYPE = 'W'"),m_strBUDGET_COMPILATION_KEY);
	m_pDM->RefreshDataManager(strWhere);

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("이용대상구분"),_T("UDF_이용대상구분"));

	if(m_pDM->GetRowCount()>0) m_nCurrentIdx = 0; 


	//예산집행 보조정보 적용 
	CBudgetApi api(this);
	api.ApplySubExecutionInfo(m_pDM,m_pDM_EXE_SUB,_T("M"));

	ShowAllData();

	return 0;

}

INT CBudgetListWork::InitOpenMode()
{

	
	if(m_nOpenMode<0)
	{

		if(!m_pArrayDeleteKey)
		{
			AfxMessageBox(_T("삭제KEY ARRAY를 찾을 수 없습니다!"));
			return -1;
		}
	}

	return 0;
}

INT CBudgetListWork::ShowAllData()
{
	CString sTemp;
	INT ii = 0;
	INT iTotal = 0;

	if(m_nCurrentIdx<0) return 0;
	
	for ( INT nIdx = 0 ; nIdx < m_pDM->GetRowCount() ; nIdx++ )
	{
		m_pDM->SetCellData( _T("UDF_이용대상구분"), m_pDM->GetCellData( _T("코드"), nIdx ) , nIdx );
		sTemp = m_pDM->GetCellData( _T("예산할당액"), nIdx);
		if(!sTemp.IsEmpty())
		{	
			ii = _ttoi(sTemp);
			iTotal += ii;
		}
	}
	if(iTotal != 0)
	{
		m_sBudgetTotal.Format(_T("%d"),iTotal);
		for( INT i=m_sBudgetTotal.GetLength()-1,j=0 ; i>=0 ; i--,j++ )
		{
			if( j != 0 && (j%3) == 0 ) 
			{ 
				m_sBudgetTotal.Insert( i+1,_T(",") ); i--;j++;
			}
		}
		SetDlgItemText(IDC_edtBUDGET_LIST_WORK_ACCOUNT2,m_sBudgetTotal);
	}
	else
		SetDlgItemText(IDC_edtBUDGET_LIST_WORK_ACCOUNT2,_T(""));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("이용대상구분"),_T("UDF_이용대상구분"));
	

	m_pCM->AllControlDisplay();
	m_pGrid->Display();
	m_pGrid->SetReverse(m_nCurrentIdx);

	return 0;
}

HBRUSH CBudgetListWork::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
