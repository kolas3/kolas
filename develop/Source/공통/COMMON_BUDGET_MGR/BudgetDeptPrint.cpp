// BudgetDeptPrint.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetListDept.h"
#include "BudgetDeptPrint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBudgetDeptPrint dialog


CBudgetDeptPrint::CBudgetDeptPrint(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBudgetDeptPrint)
		// NOTE: the ClassWizard will add member initialization here

	m_pDM		= NULL;
	m_pParentDM = NULL;
	m_pCM		= NULL;
	m_nSPECIES1	= 0;
	m_nSPECIES2 = 0;
	m_nBOOK1	= 0;
	m_nBOOK2	= 0;
	m_nPRICE1	= 0;
	m_nPRICE2	= 0;
	m_nCodeCnt  = 0;

	//}}AFX_DATA_INIT
	
	// KOL.UDF.022 시큐어코딩 보완
	m_pGrid = NULL;
}

CBudgetDeptPrint::~CBudgetDeptPrint()
{

}

VOID CBudgetDeptPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBudgetDeptPrint)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBudgetDeptPrint, CDialog)
	//{{AFX_MSG_MAP(CBudgetDeptPrint)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBudgetDeptPrint message handlers


INT CBudgetDeptPrint::InitControl()
{
	this->SetWindowText(_T("분기별 선정도서"));
	m_pGrid->SetTextMatrix(0,0,_T("   학  처"));
	m_pGrid->SetColWidth(0,0,1600);
		
	return 0;
}

VOID CBudgetDeptPrint::SetParentInfo(CESL_DataMgr * pDM)
{
	m_pParentDM = pDM;
}

//====================================================================
// [함수설명]
//====================================================================
VOID CBudgetDeptPrint::ShowAllData()
{
	if(!m_pParentDM)
	{
		AfxMessageBox(_T(" 부모 DM이 없습니다.!!"));
		return;
	}
	
	INT idx = m_pParentDM->GetRowCount();
	if(idx < 1)
	{
		AfxMessageBox(_T("해당 편성에 집행된 내역이 없습니다.!!"));
		return;
	}
/*	
	CString data[COUNT] = {
	_T("  적용  안함"),
	_T("  인문과학처"),
	_T("  사회과학처"),
	_T("  이  학  처"),
	_T("  공  학  처"),
	_T("  체육  학처"),
	_T("  군사  학처"),
	_T("  도  서  관"),
	_T("  정기간행물"),
	_T("  군사  도서"),
	_T("  합      계")};
*/

	INT nCode = 256;
	INT nDefaultValue = 0;
	CString strCode[256], strDesc[256];
	nDefaultValue = 0;

	
	m_pInfo->pCodeMgr->GetCode(_T("학부"), strCode, strDesc, nCode, nDefaultValue);	
	m_nCodeCnt = m_pInfo->pCodeMgr->GetCodeCount(_T("학부"));

	strDesc[m_nCodeCnt] = _T("합계");
	m_nCodeCnt += 1;

	
	idx = 0;

	for(INT i=1;i<m_nCodeCnt+1;i++)
	{
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData(_T("UDF_도서구분"),_T("일반"),i*2-2);
		m_pDM->SetCellData(_T("UDF_단위"),_T("\\"),i*2-2);
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData(_T("UDF_도서구분"),_T("군사"),i*2-1);
		m_pDM->SetCellData(_T("UDF_단위"),_T("\\"),i*2-1);	
	}
/*	//==========================================================================
	// 학처별 일반/군사의 도서구분, 단위정보를 DM에 구축해 놓는다.
	//==========================================================================
	for(INT i = 1; i<COUNT+1; i++)
	{
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData(_T("UDF_도서구분"),_T("일반"),i*2-2);
		m_pDM->SetCellData(_T("UDF_단위"),_T("\\"),i*2-2);
		m_pDM->InsertRow(-1);
		m_pDM->SetCellData(_T("UDF_도서구분"),_T("군사"),i*2-1);
		m_pDM->SetCellData(_T("UDF_단위"),_T("\\"),i*2-1);
	}
*/	
	//==========================================================================
	// 집행차수정보를 구해서 DM의 나머지 값들을 세팅한다.
	//==========================================================================
	if(SetDMBySeqNo() < 1) // 차수 정보를 얻어온다.
	{
		AfxMessageBox(_T("집행한 차수정보가 없습니다."));
	}

	m_pCM->AllControlDisplay();
	
	//==========================================================================
	// 제 0 번째 컬럼을 학처로 세팅
	//==========================================================================
	for(i = 1; i<m_nCodeCnt+1; i = i++)
	{
		if(i == 1){
			m_pGrid->SetTextMatrix(i,0,strDesc[i-1]);
			m_pGrid->SetTextMatrix(i+1,0,strDesc[i-1]);
		}
		else{
			m_pGrid->SetTextMatrix(i*2-1,0,strDesc[i-1]);
			m_pGrid->SetTextMatrix(i*2,0,strDesc[i-1]);
		}
	}

	//==========================================================================
	// 학처별로 그리드 머지
	//==========================================================================

	i = 0;
	for(INT j=1 ; j<m_nCodeCnt ; j+=2)
	{
		m_pGrid->SetMergeCells(2);
		m_pGrid->SetMergeCol(0,TRUE);
		m_pGrid->SetTextMatrix(j,0,strDesc[i]); 
		m_pGrid->SetTextMatrix(j+1,0,strDesc[i]); 
		
		i++;
	}
}
//==========================================================================
// [함수설명]
//  차수의 학처별로 2 ROW(일반/군사)씩  DM에 값을 Update한다.
//  - argument 1. 학처에 매칭되는 번호
// [MINI SPEC]
//  1.일반/군사 별로 종수를 Update한다.
//  2.일반/군사 별로 권수를 Update한다.
//  3.일반/군사 별로 가격을 Update한다.
//  4.해당 맴버변수는 초기화 한다. 
//==========================================================================
INT CBudgetDeptPrint::SaveDM(INT nDept)
{
	CString sSPECIES1, sSPECIES2, sBOOK1, sBOOK2, sPRICE1, sPRICE2;
	CString sTemp;
	sSPECIES1.Format(_T("%d"),m_nSPECIES1);
	sSPECIES2.Format(_T("%d"),m_nSPECIES2);
	sBOOK1.Format(_T("%d"),m_nBOOK1);
	sBOOK2.Format(_T("%d"),m_nBOOK2);
	sPRICE1.Format(_T("%d"),m_nPRICE1);
	sPRICE2.Format(_T("%d"),m_nPRICE2);
	
	//----------------------------------------------------------------------
	// 1.일반/군사 별로 종수를 Update한다.
	//----------------------------------------------------------------------
	sTemp = m_pDM->GetCellData(_T("UDF_종수"),nDept*2);
	m_pDM->SetCellData(_T("UDF_종수"),ModifyCell(sTemp, sSPECIES1),nDept*2   );
	sTemp = m_pDM->GetCellData(_T("UDF_종수"),nDept*2+1);
	m_pDM->SetCellData(_T("UDF_종수"),ModifyCell(sTemp, sSPECIES2),nDept*2+1 );

	//----------------------------------------------------------------------
	// 2.일반/군사 별로 권수를 Update한다.
	//----------------------------------------------------------------------
	sTemp = m_pDM->GetCellData(_T("UDF_권수"),nDept*2);
	m_pDM->SetCellData(_T("UDF_권수"),ModifyCell(sTemp, sBOOK1),nDept*2   );
	sTemp = m_pDM->GetCellData(_T("UDF_권수"),nDept*2+1);
	m_pDM->SetCellData(_T("UDF_권수"),ModifyCell(sTemp, sBOOK2),nDept*2+1 );
	
	//----------------------------------------------------------------------
	// 3.일반/군사 별로 가격을 Update한다.
	//----------------------------------------------------------------------
	sTemp = m_pDM->GetCellData(_T("UDF_가격"),nDept*2);
	m_pDM->SetCellData(_T("UDF_가격"),ModifyCell(sTemp, sPRICE1),nDept*2   );
	sTemp = m_pDM->GetCellData(_T("UDF_가격"),nDept*2+1);
	m_pDM->SetCellData(_T("UDF_가격"),ModifyCell(sTemp, sPRICE2),nDept*2+1 );
	
	//----------------------------------------------------------------------
	// 4.해당 맴버변수는 초기화 한다. 
	//----------------------------------------------------------------------
	m_nSPECIES1 = 0;
	m_nSPECIES2 = 0;
	m_nBOOK1	= 0;
	m_nBOOK2	= 0;
	m_nPRICE1	= 0;
	m_nPRICE2	= 0;
	
	return 0;		
}

//==========================================================================
// [함수설명]
//  기존 셀의 정보와 추가되는 정보를 더한 값을 리턴한다.
//  - argument 
//		1. 이전 셀정보
//		2. 추가 셀정보  
//  - return
//      1. Update될 셀 정보
//==========================================================================
CString CBudgetDeptPrint::ModifyCell(CString str1, CString str2)
{
	if(str1.IsEmpty() == TRUE)
		str1 = _T("0");
	if(str2.IsEmpty() == TRUE)
		str2 = _T("0");
	CString sResult;
	INT number;
	number = _ttoi(str1) + _ttoi(str2);
	sResult.Format(_T("%d"), number);

	return sResult;
}

INT CBudgetDeptPrint::SetDMBySeqNo()
{
	INT nCnt;
	CString sQuery, result, sREC_KEY;
	CESL_DataMgr tempDM;
	tempDM.SetCONNECTION_INFO(m_pParentDM->CONNECTION_INFO);
	
	// 편성키를 가져온다.
	m_pParentDM->GetCellData(_T("예산편성KEY"),0,sREC_KEY);

	// 집행한 차수의 개수를 가져온다.
	sQuery.Format(_T("SELECT EXECUTE_GROUP_KEY FROM ")
		_T("CO_BUDGET_EXECUTION_TBL WHERE ")
		_T("BUDGET_COMPILATION_KEY IN ")
		_T("(SELECT REC_KEY ")
		_T("FROM CO_BUDGET_COMPILATION_TBL ")
		_T("WHERE REC_KEY = %s)"), sREC_KEY);

	tempDM.RestructDataManager(sQuery);
	nCnt = tempDM.GetRowCount();

	if(nCnt < 1)
		return -1;

	
	for(INT i = 0; i< nCnt ; i++)
	{
		sREC_KEY.Empty();
		sREC_KEY = tempDM.GetCellData(i,0);
		if(sREC_KEY.IsEmpty() == FALSE)
			GetSpeciesCnt(sREC_KEY);
	}
	
	SaveDMSUM();
	
	return nCnt;
}

INT CBudgetDeptPrint::SaveDMSUM()
{
	CString sTemp;
	INT nTotalSpeciesCnt1, nTotalBookCnt1, nTotalPrice1;
	INT nTotalSpeciesCnt2, nTotalBookCnt2, nTotalPrice2;
	
	nTotalSpeciesCnt1 = 0;
	nTotalBookCnt1    = 0;
	nTotalPrice1      = 0;
	nTotalSpeciesCnt2  = 0;
	nTotalBookCnt2    = 0;
	nTotalPrice2      = 0;

	sTemp.Empty();
	for(INT i = 0; i<m_nCodeCnt-1 ; i++)
	{
		// 일반
		m_pDM->GetCellData(_T("UDF_종수"),i*2,sTemp);
		nTotalSpeciesCnt1 += _ttoi(sTemp);
		m_pDM->GetCellData(_T("UDF_권수"),i*2,sTemp);
		nTotalBookCnt1 += _ttoi(sTemp);
		m_pDM->GetCellData(_T("UDF_가격"),i*2,sTemp);
		nTotalPrice1 += _ttoi(sTemp);
		
		// 군사
		m_pDM->GetCellData(_T("UDF_종수"),i*2+1,sTemp);
		nTotalSpeciesCnt2 += _ttoi(sTemp);
		m_pDM->GetCellData(_T("UDF_권수"),i*2+1,sTemp);
		nTotalBookCnt2 += _ttoi(sTemp);
		m_pDM->GetCellData(_T("UDF_가격"),i*2+1,sTemp);
		nTotalPrice2 += _ttoi(sTemp);
	}
	
	sTemp.Format(_T("%d"),nTotalSpeciesCnt1);
	m_pDM->SetCellData(_T("UDF_종수"),sTemp,m_nCodeCnt*2-2);
	sTemp.Format(_T("%d"),nTotalBookCnt1);
	m_pDM->SetCellData(_T("UDF_권수"),sTemp,m_nCodeCnt*2-2);
	sTemp.Format(_T("%d"),nTotalPrice1);
	m_pDM->SetCellData(_T("UDF_가격"),sTemp,m_nCodeCnt*2-2);
	sTemp.Format(_T("%d"),nTotalSpeciesCnt2);
	m_pDM->SetCellData(_T("UDF_종수"),sTemp,m_nCodeCnt*2-1);
	sTemp.Format(_T("%d"),nTotalBookCnt2);
	m_pDM->SetCellData(_T("UDF_권수"),sTemp,m_nCodeCnt*2-1);
	sTemp.Format(_T("%d"),nTotalPrice2);
	m_pDM->SetCellData(_T("UDF_가격"),sTemp,m_nCodeCnt*2-1);

	return 0;
}

INT CBudgetDeptPrint::GetSpeciesCnt(CString data)
{
	if(data.IsEmpty() == TRUE)
	{
		return -1;
	}

	//차수번호를 구한다.
	CString sQuery, sSeq_No, sAcq_Year, sPrice;
	INT nSpeciesCnt;
	CESL_DataMgr tempDM;
	tempDM.SetCONNECTION_INFO(m_pParentDM->CONNECTION_INFO);

	
	sQuery.Format(_T("SELECT SEQ_NO FROM BO_PURCHASE_SEQ_NO_TBL WHERE REC_KEY = %s"),data);
	m_pDM->GetOneValueQuery(sQuery,sSeq_No);

	sQuery.Format(_T("SELECT ACQ_YEAR FROM BO_PURCHASE_SEQ_NO_TBL WHERE REC_KEY = %s"),data);
	m_pDM->GetOneValueQuery(sQuery,sAcq_Year);


	CString sCondition;
	CString sSpecies_key;

	// 학처별로...
	for(INT k = 0 ; k<m_nCodeCnt-1 ; k++)
	{
		sCondition = GetCondition(k);
		//차수에 대해 도서구분별 종수와 가격을 구한다.
		//도서구분이 일반일때..
		sQuery.Format(_T("SELECT SPECIES_KEY, ROUND(ORDER_PRICE,-1) ")
			_T("FROM BO_PURCHASEINFO_TBL ")
			_T("WHERE %s ORDER_SEQ_NO = '%s' ")
			_T("AND ORDER_YEAR = '%s' ")
			_T("AND SPECIES_KEY IN ")
			_T("(SELECT REC_KEY FROM BO_SPECIES_TBL WHERE USER_DEFINE_CODE1 = '01' AND MANAGE_CODE = UDF_MANAGE_CODE)")
//DEL 			_T("(SELECT REC_KEY FROM BO_SPECIES_TBL WHERE USER_DEFINE_CODE1 = '01')")
			,sCondition, sSeq_No, sAcq_Year);

		tempDM.RestructDataManager(sQuery);
		nSpeciesCnt = tempDM.GetRowCount();
		
		if(nSpeciesCnt > 0)
		{
			m_nSPECIES1 += nSpeciesCnt;
			for( INT i = 0; i<nSpeciesCnt; i++)
			{
				sPrice = tempDM.GetCellData(i,1);
				if(sPrice.IsEmpty())
				{
					sPrice = _T("0");
				}
				m_nPRICE1 += _ttoi(sPrice);
				sSpecies_key = tempDM.GetCellData(i,0);
				GetBookCnt(sSpecies_key,0);
			}
		}
		
		tempDM.FreeData();
		sPrice.Empty();
		
		//도서구분이 군사일때...
		sQuery.Format(_T("SELECT SPECIES_KEY, ROUND(ORDER_PRICE,-1) ")
			_T("FROM BO_PURCHASEINFO_TBL ")
			_T("WHERE %s ORDER_SEQ_NO = '%s' ")
			_T("AND ORDER_YEAR = '%s' ")
			_T("AND SPECIES_KEY IN ")
			_T("(SELECT REC_KEY FROM BO_SPECIES_TBL WHERE USER_DEFINE_CODE1 = '02' AND MANAGE_CODE = UDF_MANAGE_CODE)")
//DEL 			_T("(SELECT REC_KEY FROM BO_SPECIES_TBL WHERE USER_DEFINE_CODE1 = '02')")
			,sCondition, sSeq_No, sAcq_Year);

		tempDM.RestructDataManager(sQuery);
		nSpeciesCnt = tempDM.GetRowCount();
		
		if(nSpeciesCnt > 0)
		{
			m_nSPECIES2 += nSpeciesCnt;
			for( INT i = 0; i<nSpeciesCnt; i++)
			{
				sPrice = tempDM.GetCellData(i,1);
				if(sPrice.IsEmpty())
				{
					sPrice = _T("0");
				}
				m_nPRICE2 += _ttoi(sPrice);
				sSpecies_key = tempDM.GetCellData(i,0);
				GetBookCnt(sSpecies_key,1);
			}
		}

		SaveDM(k);
		tempDM.FreeData();
		sPrice.Empty();
		

	}
	return 0;
}

INT CBudgetDeptPrint::GetBookCnt(CString data, INT mode)
{
	if( data.IsEmpty() == TRUE)
	{
		return -1;
	}
	CString sQuery, sResult;
	
	sQuery.Format(_T("SELECT COUNT(REC_KEY) FROM BO_BOOK_TBL WHERE SPECIES_KEY = %s"),data);
	m_pDM->GetOneValueQuery(sQuery,sResult);
	
	if(sResult.IsEmpty() == TRUE)
	{
		sResult = _T("0");
	}
	
	//=============================================================
	// 2. 도서구분별로 종의 책수를 구한다.
	//=============================================================
	if(mode == 0)
	{
		m_nBOOK1 += _ttoi(sResult);
	}
	else
	{
		m_nBOOK2 += _ttoi(sResult);
	}

	return 0;
}

CString CBudgetDeptPrint::GetCondition(INT number)
{
	CString cTemp;
	switch(number)
	{
	case 0:
		cTemp = _T("DEPT_CODE IS NULL AND");
		break;
	case 1: 
		cTemp = _T("DEPT_CODE = '01' AND");
		break;
	case 2: 
		cTemp = _T("DEPT_CODE = '02' AND");
		break;
	case 3: 
		cTemp = _T("DEPT_CODE = '03' AND");
		break;
	case 4: 
		cTemp = _T("DEPT_CODE = '04' AND");
		break;
	case 5: 
		cTemp = _T("DEPT_CODE = '05' AND");
		break;
	case 6: 
		cTemp = _T("DEPT_CODE = '06' AND");
		break;
	case 7: 
		cTemp = _T("DEPT_CODE = '07' AND");
		break;
	case 8: 
		cTemp = _T("DEPT_CODE = '08' AND");
		break;
	case 9: 
		cTemp = _T("DEPT_CODE = '09' AND");
		break;
	default:
		cTemp.Empty();
	}

	return cTemp;
}

BOOL CBudgetDeptPrint::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	if(InitESL_Mgr( _T("예산관리_분기별_선정도서") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 예산관리_분기별_선정도서") );
		return false;
	}
	
	
	m_pCM = FindCM(_T("CM_예산관리_분기별_선정도서"));
	m_pDM = FindDM(_T("DM_예산관리_분기별_선정도서"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));

	if(!m_pCM || !m_pDM || !m_pGrid) {
		ESLAfxMessageBox( _T("CM,DM,Grid Error : CM_예산관리_분기별_선정도서,DM_예산관리_분기별_선정도서,그리드") );
		return FALSE;
	}
	
	if(InitControl()<0)
	{
		AfxMessageBox( _T(" 컨트롤 초기화 실패!! "));
		return FALSE;
	}
	
	ShowAllData();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CBudgetDeptPrint::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
