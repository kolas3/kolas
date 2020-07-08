// BudgetListCource.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetListCource.h"
#include "BudgetApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBudgetListCource dialog


CBudgetListCource::CBudgetListCource(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBudgetListCource)
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
CBudgetListCource::~CBudgetListCource()
{
}

VOID CBudgetListCource::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBudgetListCource)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CBudgetListCource::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


BEGIN_MESSAGE_MAP(CBudgetListCource, CDialog)
	//{{AFX_MSG_MAP(CBudgetListCource)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_COURCE_INSERT, OnbtnBUDGETLISTCOURCEINSERT)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_COURCE_MODIFY, OnbtnBUDGETLISTCOURCEMODIFY)
	ON_BN_CLICKED(IDC_btnBUDGET_LIST_COURCE_DELETE, OnbtnBUDGETLISTCOURCEDELETE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBudgetListCource message handlers

BOOL CBudgetListCource::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	
	if(InitESL_Mgr( _T("예산관리_예산편성_학과별") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 예산관리_예산편성_학과별") );
		return false;
	}
	
	
	m_pCM = FindCM(_T("CM_예산편성_학과별"));
	m_pDM = FindDM(_T("DM_예산편성보조_학과별"));
	m_pDM_EXE_SUB = FindDM(_T("DM_예산집행보조"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));

	if(!m_pCM || !m_pDM || !m_pGrid || !m_pDM_EXE_SUB) {
		AfxMessageBox( _T("CM,DM,Grid Error : CM_예산편성_학과별,DM_예산편성보조_학과별,그리드") );
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

INT CBudgetListCource::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
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

INT CBudgetListCource::InitOpenMode()
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

INT CBudgetListCource::ShowAllData()
{
	if(m_nCurrentIdx<0) return 0;
	
	CString sTemp = _T("");
	INT ii = 0;
	INT iTotal = 0;
	
	//모두 Display하기 위해... ㅠㅠ;;
	for ( INT nIdx = 0 ; nIdx < m_pDM->GetRowCount() ; nIdx++ )
	{
		m_pDM->SetCellData( _T("UDF_학과"), m_pDM->GetCellData( _T("코드"), nIdx ) , nIdx );
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
		SetDlgItemText(IDC_edtBUDGET_LIST_COURCE_ACCOUNT2,m_sBudgetTotal);
	}
	else 
		SetDlgItemText(IDC_edtBUDGET_LIST_COURCE_ACCOUNT2,_T(""));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("학과"),_T("UDF_학과"));
	

	m_pCM->AllControlDisplay();
	m_pGrid->Display();
	m_pGrid->SetReverse(m_nCurrentIdx);

	return 0;
}

VOID CBudgetListCource::SetAllData(INT row, CString strUDF)
{
	const INT cnt = 2;
	CString alias[cnt];
	CString data[cnt];

	alias[0] = _T("학과");
	alias[1] = _T("예산할당액");


	for(INT i=0;i<cnt;i++)
	{
		m_pCM->GetControlMgrData(alias[i],data[i]);
	}

	m_pDM->SetCellData(_T("코드"),data[0],row);
	m_pDM->SetCellData(_T("UDF_학과"),data[0],row);
	m_pDM->SetCellData(_T("예산할당액"),data[1],row);
	m_pDM->SetCellData(_T("UDF_상태"),strUDF,row);
	m_pDM->SetCellData(_T("보조유형"),_T("C"),row);

	m_nCurrentIdx =	row;
	ShowAllData();
}

VOID CBudgetListCource::OnbtnBUDGETLISTCOURCEINSERT() 
{
	// TODO: Add your control notification handler code here
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
	m_pCM->GetControlMgrData(_T("학과"),strCode,-1);
	ids = api.CheckDupCode(m_pDM, strCode);
	if(ids < 0 )
		return;

	m_pDM->InsertRow(-1);
	INT row = m_pDM->GetRowCount()-1;
	SetAllData(row, _T("I")); //insert
}

VOID CBudgetListCource::OnbtnBUDGETLISTCOURCEMODIFY() 
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

VOID CBudgetListCource::OnbtnBUDGETLISTCOURCEDELETE() 
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
	strTmp = m_pDM->GetCellData(_T("UDF_학과"),m_nCurrentIdx);

	CString msg;
	msg.Format(_T(" [%s] 학과의  예산할당을 삭제하겠습니까?"), strTmp);
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

INT CBudgetListCource::RefreshAll()
{
	if(m_strBUDGET_COMPILATION_KEY.IsEmpty())
	{
		AfxMessageBox(_T("예산편성KEY를 찾을 수 없습니다!"));
		return -1;
	}

	CString strWhere;
	strWhere.Format(_T("BUDGET_COMPILATION_KEY = %s AND CLASS_KIND = 'COU' AND SUB_TYPE = 'C'"),m_strBUDGET_COMPILATION_KEY);
	m_pDM->RefreshDataManager(strWhere);

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("학과"),_T("UDF_학과"));

	if(m_pDM->GetRowCount()>0) m_nCurrentIdx = 0; 

	//예산집행 보조정보 적용 
	CBudgetApi api(this);
	api.ApplySubExecutionInfo(m_pDM,m_pDM_EXE_SUB,_T("C"));

	ShowAllData();

	return 0;

}
BEGIN_EVENTSINK_MAP(CBudgetListCource, CDialog)
    //{{AFX_EVENTSINK_MAP(CBudgetListCource)
	ON_EVENT(CBudgetListCource, IDC_gridBUDGET_LIST_COURCE, -600 /* Click */, OnClickgridBUDGETLISTCOURCE, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBudgetListCource::OnClickgridBUDGETLISTCOURCE() 
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

HBRUSH CBudgetListCource::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
