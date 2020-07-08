// PriceSum.cpp: implementation of the CPriceSum class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PriceSum.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPriceSum::CPriceSum(CESL_Mgr *pWnd):CSPF_Work(pWnd)
{
	m_pDM = pWnd->FindDM(_T("DM_공통가격합산"));
	m_pCM = pWnd->FindCM(_T("CM_공통가격합산"));
	m_pParentBookDM = NULL;

	m_bApplyDiscRate = FALSE;
	m_strSubQuery	 = _T("");
	m_strOrderPrice_Unit = _T("");

	// KOL.UDF.022 시큐어코딩 보완
	m_TotalPrice = 0;
	m_TotalBookCnt = 0;
	m_TotalSpeciesCnt = 0;
	m_strWorkingStatusWhereStmt = _T("");	
}

CPriceSum::~CPriceSum()
{
	DeteleRecKeyArray();
}

INT CPriceSum::InitPriceSum(CString CMAlias, INT tabCount, BOOL isSpecies, BOOL isMissingData)
{
	CString DMAlias = GetDMAlias(tabCount, isSpecies);
	CString GridAlias = GetGridAlias(tabCount, isSpecies);

	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);

	INT ids = m_pParentMgr->SelectMakeList(CMAlias, GridAlias);
	if( ids < 0 )		return -1000;
	
	INT firstPos = m_pParentMgr->SelectGetHeadPosition(CMAlias, GridAlias);
	if( firstPos < 0 )	return -1000;
	
	INT i = firstPos;
	CString rec_key = _T("");

	while(i >= 0)
	{
		if(isSpecies)	pDM->GetCellData(_T("BP_구입정보KEY"), i, rec_key);
		else			pDM->GetCellData(_T("BB_책KEY"), i, rec_key);
		m_ReckeyList.AddTail(rec_key);
		i = m_pParentMgr->SelectGetNext(CMAlias, GridAlias);
	}

	if(tabCount == 0)							m_strWorkingStatusWhereStmt = _T(" AND B.WORKING_STATUS < 'BOC'");
	else if(tabCount == 1)						m_strWorkingStatusWhereStmt = _T(" AND B.WORKING_STATUS = 'BOA111N'");
	else if(tabCount == 2 && isMissingData)		m_strWorkingStatusWhereStmt = _T(" AND B.WORKING_STATUS = 'BOA114O'");
	else if(tabCount == 2 && !isMissingData)	m_strWorkingStatusWhereStmt = _T(" AND B.WORKING_STATUS = 'BOA113N'");
	else if(tabCount == 3)						m_strWorkingStatusWhereStmt = _T(" AND B.WORKING_STATUS = 'BOA141N'");

	if( isSpecies == FALSE )	ids = InitTotalValue(FALSE, GRID_SELECT);
	else						ids = InitTotalValue(TRUE, GRID_SELECT);
	if( ids < 0 )		return -1001;

	ids = MakePriceDM(tabCount, isSpecies, _T("KDC주제구분"), GRID_SELECT);
	if( ids < 0 )		return -1002;

	return 0;
}

INT CPriceSum::InitPriceSum(CESL_DataMgr *pDM, CESL_Grid *pGrid, SPECIES_TYPE nSpeciesType, BOOL bSpeciesYN)
{
	INT ids, firstPos, nCnt, nMaxCnt, i;
	CString strPreckey, strBsortno, strTmpData;
	CStringArray* pRecKeyArray = new CStringArray;

	switch( nSpeciesType )
	{
	case BO_PURCHASE:	m_strWorkingStatusWhereStmt = _T(" AND B.WORKING_STATUS = 'BOA111N'");
		break;
	case BO_ORDER:		m_strWorkingStatusWhereStmt = _T(" AND B.WORKING_STATUS = 'BOA112N'");
		break;
	case BO_MISSING:	m_strWorkingStatusWhereStmt = _T(" AND B.WORKING_STATUS = 'BOA113O'");
		break;
	case BO_REGOBJ:		m_strWorkingStatusWhereStmt = _T(" AND B.WORKING_STATUS = 'BOA114O'");
		break;
	default:
		break;
	}

	ids	= pGrid->SelectMakeList();
	if(ids < 0)	return -1000;
	
	firstPos = pGrid->SelectGetHeadPosition();
	if( firstPos < 0 ) return -1000;	
	
	if( bSpeciesYN == TRUE )	nMaxCnt = SPECIES_DATA_LIMIT;
	else nMaxCnt = VOLUME_DATA_LIMIT;
	
	nCnt = 1;
	i = firstPos;
	while( i >= 0 )
	{
		if( nCnt > nMaxCnt && bSpeciesYN == TRUE )
		{
			m_ReckeyListArray.Add(pRecKeyArray);
			pRecKeyArray = new CStringArray;
			nCnt = 1;
		}

		if( bSpeciesYN == TRUE )
		{
			pDM->GetCellData(_T("BP_수서정보KEY"), i, strPreckey);
			pRecKeyArray->Add(strPreckey);
			nCnt++;
		}
		else
		{
			pDM->GetCellData(_T("BP_수서정보KEY"), i, strPreckey);
			pDM->GetCellData(_T("BB_권일련번호"), i, strBsortno);
			if( strPreckey.IsEmpty() == FALSE )
			{
				strTmpData.Format(_T("%s/%s"), strPreckey, strBsortno);
				m_MapVolInfo.SetAt(strTmpData, _T(""));
			}
		}
		i = pGrid->SelectGetNext();
	}
	if( bSpeciesYN == TRUE )	m_ReckeyListArray.Add(pRecKeyArray);
	
	if( bSpeciesYN == FALSE )	MakeRecKey();

	ids = InitTotalValue(bSpeciesYN, GRID_SELECT);
	if(ids < 0)		return -1001;
	
	ids = MakePriceDM(0, bSpeciesYN, _T("KDC주제구분"), GRID_SELECT);
	if(ids < 0)		return -1002;

	return 0;
}

CString CPriceSum::GetDMAlias(INT tabCount, BOOL isSpecies)
{
	if( isSpecies == FALSE )		return _T("DM_LIGHT_단행_수서_책");
	return _T("DM_LIGHT_단행_수서_종");
}

CString CPriceSum::GetGridAlias(INT tabCount, BOOL isSpecies)
{
	CString strAlias = _T("");
	switch( tabCount )
	{
	case 0:
		{
			if( isSpecies == TRUE )		strAlias = _T("MainGrid_All");
			else						strAlias = _T("MainGrid_All_Book");
		}
		break;
	case 1:
		{
			if( isSpecies == TRUE )		strAlias = _T("MainGrid_Purchasing");
			else						strAlias = _T("MainGrid_Purchasing_Book");
		}
		break;
	case 2:
		{
			if( isSpecies == TRUE )		strAlias = _T("MainGrid_CheckIn_Species");
			else						strAlias = _T("MainGrid_CheckIn_Book");
		}
		break;
	case 3:
		{
			if( isSpecies == TRUE )		strAlias = _T("MainGrid_CheckIn_Complete");
			else						strAlias = _T("MainGrid_CheckIn_Complete_Book");
		}
		break;
	default:							strAlias.Empty();
		break;
	}
	return strAlias;
}

INT CPriceSum::MakePriceDM(INT tabCount, BOOL isSpecies, CString tabName, PRICE_SUM_TYPE type)
{
	INT ids;
	
	if( type == GRID_SELECT )	return(MakePriceDMGridSelect(isSpecies, tabName));
	
	if( isSpecies == TRUE )
	{
		if		(tabName == _T("KDC주제구분"))		ids = MakeSpeciesPriceDM_KDC_Subject(m_pDM->DB_MGR_ALIAS);
		else if (tabName == _T("LDC주제구분"))		ids = MakeSpeciesPriceDM_LDC_Subject(m_pDM->DB_MGR_ALIAS);
		else if (tabName == _T("등록구분"))			ids = MakeSpeciesPriceDM_REG_Code(m_pDM->DB_MGR_ALIAS);
		else if (tabName == _T("별치구분"))			ids = MakeSpeciesPriceDM_SEP_Code(m_pDM->DB_MGR_ALIAS);
		else if (tabName == _T("형식구분"))			ids = MakeSpeciesPriceDM_FORM_Code(m_pDM->DB_MGR_ALIAS);
		else if (tabName == _T("매체구분"))			ids = MakeSpeciesPriceDM_MEDIA_Code(m_pDM->DB_MGR_ALIAS);
		else if (tabName == _T("보조등록구분"))		ids = MakeSpeciesPriceDM_SUB_REG_Code(m_pDM->DB_MGR_ALIAS);
		else if (tabName == _T("도서분류코드1"))	ids = MakeSpeciesPriceDM_User_Define_Code1(m_pDM->DB_MGR_ALIAS);
		else if (tabName == _T("도서분류코드2"))	ids = MakeSpeciesPriceDM_User_Define_Code2(m_pDM->DB_MGR_ALIAS);
		else if (tabName == _T("도서분류코드3"))	ids = MakeSpeciesPriceDM_User_Define_Code3(m_pDM->DB_MGR_ALIAS);
		else									return -1;
	}
	else
	{
		if		(tabName == _T("KDC주제구분"))		ids = MakeBookPriceDM_KDC_Subject(m_pDM->DB_MGR_ALIAS, type);
		else if (tabName == _T("LDC주제구분"))		ids = MakeBookPriceDM_LDC_Subject(m_pDM->DB_MGR_ALIAS, type);
		else if (tabName == _T("등록구분"))			ids = MakeBookPriceDM_REG_Code(m_pDM->DB_MGR_ALIAS, type);
		else if (tabName == _T("별치구분"))			ids = MakeBookPriceDM_SEP_Code(m_pDM->DB_MGR_ALIAS, type);
		else if (tabName == _T("형식구분"))			ids = MakeBookPriceDM_FORM_Code(m_pDM->DB_MGR_ALIAS, type);
		else if (tabName == _T("매체구분"))			ids = MakeBookPriceDM_MEDIA_Code(m_pDM->DB_MGR_ALIAS, type);
		else if(tabName == _T("보조등록구분"))	ids = MakeBookPriceDM_SUB_REG_Code(m_pDM->DB_MGR_ALIAS, type);
		else if (tabName == _T("도서분류코드1"))	ids = MakeBookPriceDM_User_Define_Code1(m_pDM->DB_MGR_ALIAS, type);
		else if (tabName == _T("도서분류코드2"))	ids = MakeBookPriceDM_User_Define_Code2(m_pDM->DB_MGR_ALIAS, type);
		else if (tabName == _T("도서분류코드3"))	ids = MakeBookPriceDM_User_Define_Code3(m_pDM->DB_MGR_ALIAS, type);
		else									return -1;
	}

	if(ids < 0)		return -1;

	return 0;
}

INT CPriceSum::MakeSpeciesPriceDM_KDC_Subject(CString DMAlias)
{
	INT ids, startrow, codeCnt, i ;
	FLOAT bookrate, pricerate ;
	CString headQuery, midQuery, tailQuery, tmpstr, tmpQuery;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];	
	CESL_DataMgr tmpDM;
	CESL_DataMgr* pDM;

	pDM = m_pParentMgr->FindDM(DMAlias);
	if( pDM == NULL )	return -1000;
	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT p.SUBJECT_CODE, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
		_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b, BO_PURCHASEINFO_TBL p ")
		_T("WHERE s.REC_KEY = p.SPECIES_KEY AND b.SPECIES_KEY = s.REC_KEY AND b.ACQ_KEY = p.REC_KEY ")
		_T("AND p.MANAGE_CODE = UDF_MANAGE_CODE ")
		_T("AND b.BOOK_APPENDIX_FLAG = 'B' AND s.CLASS_NO_TYPE = '1' ")
		, strPriceWon);

	headQuery += m_strWorkingStatusWhereStmt + _T(" ");
		
	tmpstr = MakeRecKeyQuery();
	if( tmpstr.IsEmpty() == TRUE )		return -1001;
	else								midQuery.Format(_T("AND p.REC_KEY IN %s "), tmpstr);

	tailQuery = _T("GROUP BY p.SUBJECT_CODE");		
		
	tmpQuery = headQuery + midQuery + tailQuery;
	ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0)			return -1002;

	startrow = 0;
	bookrate = pricerate = 0.0;	
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("KDC대주제부호관리"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("KDC대주제부호관리"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);	
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;
		
		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);

		TCHAR* pEndPtr = NULL;

		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}

		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)	pricerate = 0;
		else					pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery.Empty();
		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeSpeciesPriceDM_LDC_Subject(CString DMAlias)
{
	INT ids, startrow, codeCnt, i;
	FLOAT bookrate, pricerate ;
	CString headQuery, midQuery, tailQuery, tmpstr, tmpQuery;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];	
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	CESL_DataMgr tmpDM;


	if( pDM == NULL )		return -1000;
	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
			_T("SELECT p.SUBJECT_CODE, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
			_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
			_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b, BO_PURCHASEINFO_TBL p ")
			_T("WHERE s.REC_KEY = p.SPECIES_KEY AND b.SPECIES_KEY = s.REC_KEY AND b.ACQ_KEY = p.REC_KEY ")
			_T("AND p.MANAGE_CODE = UDF_MANAGE_CODE")
			_T("AND b.BOOK_APPENDIX_FLAG = 'B' AND s.CLASS_NO_TYPE = '2' ")
			, strPriceWon);

	headQuery += m_strWorkingStatusWhereStmt + _T(" ");
	
	tmpstr = MakeRecKeyQuery();
	if( tmpstr.IsEmpty() == TRUE )		return -1001;
	else								midQuery.Format(_T("AND p.REC_KEY IN %s "), tmpstr);

	tailQuery = _T("GROUP BY p.SUBJECT_CODE");
		
	tmpQuery = headQuery + midQuery + tailQuery;
	ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0)			return -1002;

	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("DDC대주제부호관리"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("DDC대주제부호관리"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);	
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;
		
		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		TCHAR* pEndPtr = NULL;
		
		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}
		
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)		pricerate = 0;
		else						pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);

		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);
		
		tmpQuery.Empty();
		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeSpeciesPriceDM_REG_Code(CString DMAlias)
{
	INT ids, startrow, codeCnt, i;
	FLOAT bookrate, pricerate ;
	CString headQuery, tailQuery, tmpQuery, midQuery, tmpstr;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];
	CESL_DataMgr tmpDM;
	CESL_DataMgr* pDM = m_pParentMgr->FindDM(DMAlias);

	if( pDM == NULL )		return -1000;
	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT b.REG_CODE, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
		_T("FROM BO_BOOK_TBL b WHERE b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' ")
		, strPriceWon);

	headQuery += m_strWorkingStatusWhereStmt + _T(" ");
	
	tmpstr = MakeRecKeyQuery();
	if( tmpstr.IsEmpty() == TRUE )		return -1001;
	else								midQuery.Format(_T("b.ACQ_KEY IN %s "), tmpstr);

	tailQuery.Format(_T("GROUP BY b.REG_CODE"));
	
	tmpQuery = headQuery + midQuery + tailQuery;
	ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;

	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("등록구분"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("등록구분"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);

		TCHAR* pEndPtr = NULL;

		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)	pricerate = 0;
		else					pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery.Empty();
		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeSpeciesPriceDM_MEDIA_Code(CString DMAlias)
{
	INT ids, startrow, codeCnt, i;
	FLOAT bookrate, pricerate;
	CString headQuery, tailQuery, tmpQuery, midQuery, tmpstr;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];
	CESL_DataMgr tmpDM;
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);

	if( pDM == NULL )		return -1000;
	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT b.MEDIA_CODE, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
		_T("FROM BO_BOOK_TBL b WHERE b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' ")
		, strPriceWon);
	headQuery += m_strWorkingStatusWhereStmt + _T(" ");
		
	tmpstr = MakeRecKeyQuery();
	if( tmpstr.IsEmpty() == TRUE )		return -1001;
	else								midQuery.Format(_T("b.ACQ_KEY IN %s "), tmpstr);
	
	tailQuery.Format(_T("GROUP BY b.MEDIA_CODE"));
	tmpQuery = headQuery + midQuery + tailQuery;
	ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0)			return -1002;


	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("매체구분"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("매체구분"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		TCHAR* pEndPtr = NULL;
		
		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if( m_TotalPrice == 0 ) 	pricerate = 0;
		else						pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);

		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery.Empty();
		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeSpeciesPriceDM_SEP_Code(CString DMAlias)
{
	INT ids, startrow, codeCnt, i;
	FLOAT bookrate, pricerate;
	CString headQuery, tailQuery, tmpQuery, midQuery, tmpstr;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];
	CESL_DataMgr tmpDM;
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);

	if( pDM == NULL )	return -1000;
	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT b.SEPARATE_SHELF_CODE, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
		_T("FROM BO_BOOK_TBL b WHERE b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' ")
		, strPriceWon);

	headQuery += m_strWorkingStatusWhereStmt + _T(" ");


	tmpstr = MakeRecKeyQuery();
	if( tmpstr.IsEmpty() == TRUE )	return -1001;
	else							midQuery.Format(_T("b.ACQ_KEY IN %s "), tmpstr);
	
	tailQuery.Format(_T("GROUP BY b.SEPARATE_SHELF_CODE"));
	
	tmpQuery = headQuery + midQuery + tailQuery;
	ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;


	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("별치기호"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("별치기호"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		TCHAR* pEndPtr = NULL;
		
		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)	pricerate = 0;
		else					pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);

		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery.Empty();
		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeSpeciesPriceDM_FORM_Code(CString DMAlias)
{

	INT ids, startrow, codeCnt, i;
	FLOAT bookrate, pricerate;
	CString headQuery, tailQuery, tmpQuery, midQuery, tmpstr;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];
	CESL_DataMgr tmpDM;
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);


	if( pDM == NULL )	 return -1000;
	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	
	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT s.FORM_CODE, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
		_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b ")
		_T("WHERE s.REC_KEY = b.SPECIES_KEY AND b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' ")
		, strPriceWon);

	headQuery += m_strWorkingStatusWhereStmt + _T(" ");


	tmpstr = MakeRecKeyQuery();
	if( tmpstr.IsEmpty() == TRUE )		return -1001;
	else								midQuery.Format(_T("b.ACQ_KEY IN %s "), tmpstr);
		
	tailQuery.Format(_T("GROUP BY s.FORM_CODE"));
	
	tmpQuery = headQuery + midQuery + tailQuery;
	ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;

	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("형식구분"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("형식구분"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		TCHAR* pEndPtr = NULL;

		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)	pricerate = 0;
		else					pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);
		
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery.Empty();
		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeSpeciesPriceDM_SUB_REG_Code(CString DMAlias)
{
	INT ids, startrow, codeCnt, i ;
	FLOAT bookrate, pricerate ;
	CString headQuery, midQuery, tailQuery, tmpQuery, tmpstr;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];
	CESL_DataMgr* pDM = m_pParentMgr->FindDM(DMAlias);
	CESL_DataMgr tmpDM;


	if( pDM == NULL ) return -1000;
	pDM->FreeData();	
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);	

	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT s.SUB_REG_CODE, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
		_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b ")
		_T("WHERE s.REC_KEY = b.SPECIES_KEY and b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' ")
		, strPriceWon);

	headQuery += m_strWorkingStatusWhereStmt + _T(" ");
	
	tmpstr = MakeRecKeyQuery();
	if( tmpstr.IsEmpty() == TRUE )		return -1001;
	else								midQuery.Format(_T("b.ACQ_KEY IN %s "), tmpstr);
	
	tailQuery.Format(_T("GROUP BY s.SUB_REG_CODE"));

	tmpQuery = headQuery + midQuery + tailQuery;
	ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0)			return -1002;

	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("보조등록구분"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("보조등록구분"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();

		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		TCHAR* pEndPtr = NULL;
		
		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)		pricerate = 0;
		else						pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);

		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery.Empty();
		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeSpeciesPriceDM_User_Define_Code1(CString DMAlias)
{
	INT ids, startrow, codeCnt, i;
	FLOAT bookrate, pricerate;

	CString headQuery, tailQuery, tmpQuery, midQuery, tmpstr;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];
	CESL_DataMgr tmpDM;
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);

	if( pDM == NULL )	 return -1000;
	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT s.USER_DEFINE_CODE1, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)) ,2) ")
		_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b ")
		_T("WHERE s.REC_KEY = b.SPECIES_KEY AND b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' ")
		, strPriceWon);

	headQuery += m_strWorkingStatusWhereStmt + _T(" ");

		
	tmpstr = MakeRecKeyQuery();
	if( tmpstr.IsEmpty() == TRUE )	return -1001;
	else							midQuery.Format(_T("b.ACQ_KEY IN %s "), tmpstr);
	
	tailQuery.Format(_T("GROUP BY s.USER_DEFINE_CODE1"));
	tmpQuery = headQuery + midQuery + tailQuery;
	ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;


	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("도서분류코드1"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("도서분류코드1"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);

		TCHAR* pEndPtr = NULL;

		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)	pricerate = 0;
		else					pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);

		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery.Empty();
		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeSpeciesPriceDM_User_Define_Code2(CString DMAlias)
{
	INT ids, startrow, codeCnt, i;
	FLOAT bookrate, pricerate;
	CString headQuery, tailQuery, tmpQuery, midQuery, tmpstr;	
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];
	CESL_DataMgr tmpDM;
	CESL_DataMgr* pDM = m_pParentMgr->FindDM(DMAlias);

	if( pDM == NULL )	return -1000;
	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT s.USER_DEFINE_CODE2, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
		_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b ")
		_T("WHERE s.REC_KEY = b.SPECIES_KEY ")
		_T("AND b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' ")
		, strPriceWon);

	headQuery += m_strWorkingStatusWhereStmt + _T(" ");


	tmpstr = MakeRecKeyQuery();
	if( tmpstr.IsEmpty() == TRUE )	return -1001;
	else							midQuery.Format(_T("b.ACQ_KEY IN %s "), tmpstr);
	
	tailQuery.Format(_T("GROUP BY s.USER_DEFINE_CODE2"));
	tmpQuery = headQuery + midQuery + tailQuery;
	ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;

	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("도서분류코드2"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("도서분류코드2"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		TCHAR* pEndPtr = NULL;

		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)	pricerate = 0;
		else					pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);

		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery.Empty();
		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeSpeciesPriceDM_User_Define_Code3(CString DMAlias)
{
	INT ids, startrow, codeCnt, i;
	FLOAT bookrate, pricerate;
	CString headQuery, tailQuery, tmpQuery, midQuery, tmpstr;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];
	CESL_DataMgr tmpDM;
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);

	if( pDM == NULL )	return -1000;
	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT s.USER_DEFINE_CODE3, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
		_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b ")
		_T("WHERE s.REC_KEY = b.SPECIES_KEY ")
		_T("AND b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' ")
		, strPriceWon);

	headQuery += m_strWorkingStatusWhereStmt + _T(" ");
		
	tmpstr = MakeRecKeyQuery();
	if(tmpstr.IsEmpty()) return -1001;
	else						midQuery.Format(_T("b.ACQ_KEY IN %s "), tmpstr);

	tailQuery.Format(_T("GROUP BY s.USER_DEFINE_CODE3"));
	
	tmpQuery = headQuery + midQuery + tailQuery;
	ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;


	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("도서분류코드3"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("도서분류코드3"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		TCHAR* pEndPtr = NULL;

		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}

		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)	pricerate = 0;
		else					pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery.Empty();
		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeBookPriceDM_KDC_Subject(CString DMAlias, PRICE_SUM_TYPE type)
{
	INT ids, startrow, codeCnt, i;
	FLOAT bookrate, pricerate;
	CString headQuery, tailQuery, tmpQuery, midQuery, tmpstr;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];
	CESL_DataMgr tmpDM;
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);

	
	if( pDM == NULL )	return -1000;
	if( m_strSubQuery.IsEmpty() == TRUE ) return 0;
	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT p.SUBJECT_CODE, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
		_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b, BO_PURCHASEINFO_TBL p ")
		_T("WHERE b.SPECIES_KEY = s.REC_KEY AND s.REC_KEY = b.SPECIES_KEY AND b.ACQ_KEY = p.REC_KEY ")
		_T("AND p.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' AND s.CLASS_NO_TYPE = '1' ")
		, strPriceWon);

	tmpstr = _T("(") + m_strSubQuery + _T(")");
	if( tmpstr.IsEmpty() == TRUE )		return -1001;
	else								midQuery.Format(_T("AND B.REC_KEY IN %s "), tmpstr);

	tailQuery = _T(" GROUP BY p.SUBJECT_CODE");

	tmpQuery = headQuery + midQuery + tailQuery;
	
	ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;

	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("KDC대주제부호관리"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("KDC대주제부호관리"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();

		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;
		
		pDM->SetCellData(_T("설명"), desc, nRow);		
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);

		TCHAR* pEndPtr = NULL;

		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}

		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)	pricerate = 0;
		else					pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery.Empty();
		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeBookPriceDM_LDC_Subject(CString DMAlias, PRICE_SUM_TYPE type)
{
	
	INT ids, startrow, codeCnt, i;
	FLOAT bookrate, pricerate;
	CString headQuery, tailQuery, tmpQuery, midQuery, tmpstr;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];
	CESL_DataMgr tmpDM;
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);

	
	if( pDM == NULL )		return -1000;
	if( m_strSubQuery.IsEmpty()  == TRUE )		return 0;
	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT p.SUBJECT_CODE, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
		_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b, BO_PURCHASEINFO_TBL p ")
		_T("WHERE b.SPECIES_KEY = s.REC_KEY AND s.REC_KEY = b.SPECIES_KEY AND b.ACQ_KEY = p.REC_KEY ")
		_T("AND p.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' AND s.CLASS_NO_TYPE = '2' ")
		, strPriceWon);
	
	tmpstr = _T("(") + m_strSubQuery + _T(")");
	if( tmpstr.IsEmpty() == TRUE )		return -1001;
	else 								midQuery.Format(_T("AND B.REC_KEY IN %s "), tmpstr);	
	
	tailQuery = _T("GROUP BY p.SUBJECT_CODE");

	tmpQuery = headQuery + midQuery + tailQuery;
	ids = tmpDM.RestructDataManager(tmpQuery);
	if( ids < 0 )	 return -1002;


	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("DDC대주제부호관리"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("DDC대주제부호관리"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();

		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;
		
		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		TCHAR* pEndPtr = NULL;

		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}

		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)	pricerate = 0;
		else					pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);

		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery.Empty();
		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeBookPriceDM_REG_Code(CString DMAlias, PRICE_SUM_TYPE type)
{	
	INT nRow, codeCnt, i, startrow = 0;
	FLOAT bookrate, pricerate ;
	CString headQuery, tailQuery, tmpQuery, midQuery, tmpstr;
	CString code, desc, bookcnt, pricesum, speciescnt, codes[6];	
	CESL_DataMgr* pDM = m_pParentMgr->FindDM(DMAlias);
	CESL_DataMgr tmpDM;


	if( pDM == NULL )	return -1000;
	if( m_strSubQuery.IsEmpty() == TRUE )	return 0;

	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);


	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT b.REG_CODE, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
		_T("FROM BO_BOOK_TBL b WHERE b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' ")
		, strPriceWon);

	tmpstr = _T("(") + m_strSubQuery + _T(")");
	if( tmpstr.IsEmpty() == TRUE )	return -1001;	
	else							midQuery.Format(_T("AND b.REC_KEY IN %s "), tmpstr);

	tailQuery.Format(_T("GROUP BY b.REG_CODE"));
	
	tmpQuery = headQuery + midQuery + tailQuery;
	INT ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;

	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("등록구분"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("등록구분"), _T("코드"), code, startrow);
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("등록구분"), _T("설명"), desc, startrow);

		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);	
		if(ids < 0) return -1003;

		pDM->InsertRow(-1);
		nRow = pDM->GetRowCount() - 1;
		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);

		TCHAR* pEndPtr = NULL;
		
		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}

		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);		
		if(m_TotalPrice == 0)		pricerate = 0;
		else					pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);

		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeBookPriceDM_SEP_Code(CString DMAlias, PRICE_SUM_TYPE type)
{
	INT ids, startrow, codeCnt, i;
	FLOAT bookrate, pricerate;
	CString headQuery, tailQuery, tmpQuery, midQuery, tmpstr;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];
	CESL_DataMgr tmpDM;
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	
	
	if( pDM == NULL )	return -1000;
	if( m_strSubQuery.IsEmpty() == TRUE )	return 0;

	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT b.SEPARATE_SHELF_CODE, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
		_T("FROM BO_BOOK_TBL b WHERE b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' ")
		, strPriceWon);
		
	tmpstr = _T("(") + m_strSubQuery + _T(")");
	if( tmpstr.IsEmpty() == TRUE )	return -1001;
	else							midQuery.Format(_T("AND b.REC_KEY IN %s "), tmpstr);
	
	tailQuery.Format(_T("GROUP BY b.SEPARATE_SHELF_CODE"));
	tmpQuery = headQuery + midQuery + tailQuery;
	ids = tmpDM.RestructDataManager(tmpQuery);
	if( ids < 0 )	 return -1002;

	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("별치기호"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("별치기호"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		TCHAR* pEndPtr = NULL;

		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)	pricerate = 0;
		else					pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);

		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery.Empty();
		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeBookPriceDM_MEDIA_Code(CString DMAlias, PRICE_SUM_TYPE type)
{
	INT ids, startrow, codeCnt, i;
	FLOAT bookrate, pricerate;
	CString headQuery, tailQuery, tmpQuery, midQuery, tmpstr;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];
	CESL_DataMgr tmpDM;
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);


	if( pDM == NULL )	 return -1000;
	if(m_strSubQuery.IsEmpty()) return 0;
	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT b.MEDIA_CODE, COUNT(b.rec_key), COUNT(DISTINCT b.species_key), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
		_T("FROM BO_BOOK_TBL b WHERE b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' ")
		, strPriceWon);

	tmpstr = _T("(") + m_strSubQuery + _T(")");
	if( tmpstr.IsEmpty() == TRUE )		return -1001;
	else								midQuery.Format(_T("AND b.REC_KEY IN %s "), tmpstr);
	
	tailQuery.Format(_T("GROUP BY b.MEDIA_CODE"));
	tmpQuery = headQuery + midQuery + tailQuery;
	ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;

	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("매체구분"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("매체구분"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		TCHAR* pEndPtr = NULL;

		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)	pricerate = 0;		
		else					pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery.Empty();
		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeBookPriceDM_FORM_Code(CString DMAlias, PRICE_SUM_TYPE type)
{
	INT ids, startrow, codeCnt, i;
	FLOAT bookrate, pricerate;
	CString headQuery, tailQuery, tmpQuery, midQuery, tmpstr;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];
	CESL_DataMgr tmpDM;
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	

	if(!pDM) return -1000;
	if(m_strSubQuery.IsEmpty()) return 0;
	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT s.FORM_CODE, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
		_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b ")
		_T("WHERE s.REC_KEY = b.SPECIES_KEY AND b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' ")
		, strPriceWon);

	tmpstr = _T("(") + m_strSubQuery + _T(")");
	if( tmpstr.IsEmpty() == TRUE )		return -1001;
	else								midQuery.Format(_T("AND b.REC_KEY IN %s "), tmpstr);
	
	tailQuery.Format(_T("GROUP BY s.FORM_CODE"));
	tmpQuery = headQuery + midQuery + tailQuery;
	ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;

	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("형식구분"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("형식구분"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		TCHAR* pEndPtr = NULL;
		
		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}

		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)	pricerate = 0;
		else					pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery.Empty();
		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeBookPriceDM_SUB_REG_Code(CString DMAlias, PRICE_SUM_TYPE type)
{
	INT nRow, codeCnt, i, startrow = 0;
	FLOAT bookrate, pricerate ;
	CString headQuery, tailQuery, tmpQuery, midQuery, tmpstr;
	CString code, desc, bookcnt, pricesum, speciescnt, codes[6];	
	CESL_DataMgr* pDM = m_pParentMgr->FindDM(DMAlias);
	CESL_DataMgr tmpDM;

	if( pDM == NULL )	return -1000;
	if( m_strSubQuery.IsEmpty() == TRUE )	return 0;

	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT s.SUB_REG_CODE, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
		_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b ")
		_T("WHERE s.REC_KEY = b.SPECIES_KEY AND b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' ")
		, strPriceWon);
	
	tmpstr = _T("(") + m_strSubQuery + _T(")");
	if( tmpstr.IsEmpty() == TRUE )		return -1001;	
	else								midQuery.Format(_T("AND b.REC_KEY IN %s "), tmpstr);

	tailQuery.Format(_T("GROUP BY s.SUB_REG_CODE"));
	
	tmpQuery = headQuery + midQuery + tailQuery;
	INT ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;

	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("보조등록구분"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		// 5. 
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("보조등록구분"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);	
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		nRow = pDM->GetRowCount() - 1;
		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);

		TCHAR* pEndPtr = NULL;

		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}

		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);		
		if(m_TotalPrice == 0)		pricerate = 0;
		else					pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery=_T("");
		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeBookPriceDM_User_Define_Code1(CString DMAlias, PRICE_SUM_TYPE type)
{
	INT ids, startrow, codeCnt, i;
	FLOAT bookrate, pricerate;
	CString headQuery, tailQuery, tmpQuery, midQuery, tmpstr;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];
	CESL_DataMgr tmpDM;
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	
	
	if( pDM == NULL	)	 return -1000;
	if( m_strSubQuery.IsEmpty() == TRUE ) return 0;
	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	
	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT s.USER_DEFINE_CODE1, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s'), UDF_MANAGE_CODE), 2) ")
		_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b ")
		_T("WHERE s.REC_KEY = b.SPECIES_KEY AND b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' ")
		, strPriceWon);

	tmpstr = _T("(") + m_strSubQuery + _T(")");
	if( tmpstr.IsEmpty() == TRUE )	return -1001;
	else							midQuery.Format(_T("AND b.REC_KEY IN %s "), tmpstr);

	tailQuery.Format(_T("GROUP BY s.USER_DEFINE_CODE1"));

	tmpQuery = headQuery + midQuery + tailQuery;
	ids = tmpDM.RestructDataManager(tmpQuery);
	if( ids < 0 )	 return -1002;
	
	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("도서분류코드1"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("도서분류코드1"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();

		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		TCHAR* pEndPtr = NULL;
		
		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);		
		if( m_TotalPrice == 0 )		pricerate = 0;
		else					pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery.Empty();
		startrow++;
	}
return 0;
}

INT CPriceSum::MakeBookPriceDM_User_Define_Code2(CString DMAlias, PRICE_SUM_TYPE type)
{
	INT ids, startrow, codeCnt, i;
	FLOAT bookrate, pricerate;
	CString headQuery, tailQuery, tmpQuery, midQuery, tmpstr;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];
	CESL_DataMgr tmpDM;
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);

	
	if( pDM == NULL )	return -1000;
	if(m_strSubQuery.IsEmpty()) return 0;

	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		
	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT s.USER_DEFINE_CODE2, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
		_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b ")
		_T("WHERE s.REC_KEY = b.SPECIES_KEY AND b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' ")
		, strPriceWon);

	tmpstr = _T("(") + m_strSubQuery + _T(")");
	if( tmpstr.IsEmpty() == TRUE )		return -1001;
	else								midQuery.Format(_T("AND b.REC_KEY IN %s "), tmpstr);
	
	tailQuery.Format(_T("GROUP BY s.USER_DEFINE_CODE2"));
	tmpQuery = headQuery + midQuery + tailQuery;
	ids = tmpDM.RestructDataManager(tmpQuery);
	if( ids < 0 )	 return -1002;

	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("도서분류코드2"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("도서분류코드2"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		TCHAR* pEndPtr = NULL;

		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}

		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if( m_TotalPrice == 0 )		pricerate = 0;
		else					pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery.Empty();
		startrow++;
	}

	return 0;
}

INT CPriceSum::MakeBookPriceDM_User_Define_Code3(CString DMAlias, PRICE_SUM_TYPE type)
{
	INT ids, startrow, codeCnt, i;
	FLOAT bookrate, pricerate;
	CString headQuery, tailQuery, tmpQuery, midQuery, tmpstr;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];
	CESL_DataMgr tmpDM;
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);

	
	if( pDM == NULL )	return -1000;
	if( m_strSubQuery.IsEmpty() == TRUE )	 return 0;
	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	headQuery.Format(
		_T("SELECT s.USER_DEFINE_CODE3, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
		_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
		_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b ")
		_T("WHERE s.REC_KEY = b.SPECIES_KEY AND b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' ")
		, strPriceWon);

	tmpstr = _T("(") + m_strSubQuery + _T(")");
	if( tmpstr.IsEmpty() == TRUE )		return -1001;
	else								midQuery.Format(_T("AND b.REC_KEY IN %s "), tmpstr);
	
	tailQuery.Format(_T("GROUP BY s.USER_DEFINE_CODE3"));
	
	tmpQuery = headQuery + midQuery + tailQuery;
	ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;


	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("도서분류코드3"));
	for( i=0 ; i<codeCnt ; i++ )
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("도서분류코드3"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))	ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else									ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		TCHAR* pEndPtr = NULL;

		tmpstr.Format(_T("0.00%%"));
		if( m_TotalBookCnt > 0 )
		{
			bookrate = (FLOAT)(_tcstod(bookcnt.GetBuffer(0), &pEndPtr)/m_TotalBookCnt * 100);
			tmpstr.Format(_T("%5.2f%%"), bookrate);
		}

		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);		
		if( m_TotalPrice == 0 )		pricerate = 0;
		else					pricerate = (FLOAT)(_tcstod(pricesum.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);

		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery.Empty();
		startrow++;
	}

	return 0;
}


CString CPriceSum::MakeRecKeyQuery()
{
	if( m_ReckeyList.IsEmpty() == TRUE )	return _T("");

	CString query, subquery, tmpquery, rec_key ;
	POSITION pos = m_ReckeyList.GetHeadPosition();
	
	rec_key = m_ReckeyList.GetNext(pos);
	query.Format(_T("%s"), rec_key);
	while( pos != NULL )
	{
		rec_key = m_ReckeyList.GetNext(pos);
		subquery.Format(_T(", %s"), rec_key);
		query += subquery;
	}
	tmpquery.Format(_T("( %s )"), query);

	return tmpquery;
}

INT CPriceSum::InitTotalValue(BOOL isSpecies, PRICE_SUM_TYPE type)
{
	INT ids = 0;	
	CString query, result;
	CESL_DataMgr tmpDM;

	if( m_pDM == NULL )		return -1;
	tmpDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	if( type == GRID_SELECT ) return InitTotalValueGridSelect(isSpecies);

	if( isSpecies  == TRUE )
	{
		m_TotalSpeciesCnt = m_ReckeyList.GetCount();
		query = _T("SELECT COUNT(REC_KEY), COUNT(DISTINCT b.SPECIES_KEY) FROM BO_BOOK_TBL B WHERE B.ACQ_KEY IN ");
		query += MakeRecKeyQuery() + m_strWorkingStatusWhereStmt;

		tmpDM.RestructDataManager(query);
		tmpDM.GetCellData(0, 0, result);
		m_TotalBookCnt = _ttoi(result.GetBuffer(0));
		tmpDM.GetCellData(0, 1, result);
		m_TotalSpeciesCnt = _ttoi(result.GetBuffer(0));
		
		query.Empty();
		CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
		query.Format(
			_T("SELECT GET_ORDERPRICE(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), UDF_MANAGE_CODE) ")
			_T("FROM BO_BOOK_TBL B, BO_PURCHASEINFO_TBL P ")
			_T("WHERE B.ACQ_KEY=P.REC_KEY AND P.MANAGE_CODE = UDF_MANAGE_CODE AND P.REC_KEY IN ")
			, strPriceWon);

		query += MakeRecKeyQuery() + m_strWorkingStatusWhereStmt;
		tmpDM.RestructDataManager(query);
		result.Empty();
		tmpDM.GetCellData(0, 0, result);
		if( result.IsEmpty() == TRUE )	result = _T("0");

		TCHAR* pEndPtr = NULL;
		m_TotalPrice = _tcstod(result.GetBuffer(0), &pEndPtr);
	}
	else if( isSpecies == FALSE && type == GRID_SELECT )
	{
		query = _T("SELECT COUNT(REC_KEY), COUNT(DISTINCT b.SPECIES_KEY) FROM BO_BOOK_TBL WHERE REC_KEY IN ");
		query += _T("(") + m_strSubQuery + _T(")");
		tmpDM.RestructDataManager(query);
		tmpDM.GetCellData(0, 0, result);
		m_TotalBookCnt = _ttoi(result.GetBuffer(0));
		tmpDM.GetCellData(0, 1, result);
		m_TotalSpeciesCnt = _ttoi(result.GetBuffer(0));
		
		query.Empty();		
		CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
		query.Format(
			_T("SELECT GET_ORDERPRICE(SUM(GetPriceWon(ACQ_KEY, PRICE, '%s', UDF_MANAGE_CODE)), UDF_MANAGE_CODE) ")
			_T("FROM BO_BOOK_TBL WHERE BO_BOOK_TBL.MANAGE_CODE = UDF_MANAGE_CODE AND REC_KEY IN ")
			, strPriceWon);

		query += _T("(") + m_strSubQuery + _T(")");
		tmpDM.RestructDataManager(query);
		result.Empty();
		tmpDM.GetCellData(0, 0, result);
		if( result.IsEmpty() == TRUE )	result = _T("0");
		
		TCHAR* pEndPtr = NULL;
		m_TotalPrice = _tcstod(result.GetBuffer(0), &pEndPtr);

	}
	else if( isSpecies == FALSE && type != GRID_SELECT )
	{
		query = _T("SELECT COUNT(REC_KEY), COUNT(DISTINCT(SPECIES_KEY)) FROM BO_BOOK_TBL WHERE REC_KEY IN ");
		query += _T("(") + m_strSubQuery + _T(")");
		
		tmpDM.RestructDataManager(query);
		tmpDM.GetCellData(0, 0, result);
		m_TotalBookCnt = _ttoi(result.GetBuffer(0));
		tmpDM.GetCellData(0, 1, result);
		m_TotalSpeciesCnt = _ttoi(result.GetBuffer(0));

		query.Empty();
		CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
		query.Format(
			_T("SELECT GET_ORDERPRICE(SUM(GetPriceWon(acq_key, price, '%s', UDF_MANAGE_CODE)), UDF_MANAGE_CODE) ")
			_T("FROM BO_BOOK_TBL WHERE BO_BOOK_TBL.MANAGE_CODE = UDF_MANAGE_CODE AND REC_KEY IN ")
			, strPriceWon);

		query += _T("(") + m_strSubQuery + _T(")");
		result.Empty();
		ids = m_pDM->GetOneValueQuery(query, result);
		if( result.IsEmpty() == TRUE )	result = _T("0");
			
		TCHAR* pEndPtr = NULL;
		m_TotalPrice = _tcstod(result.GetBuffer(0), &pEndPtr);
	}

	if(ids < 0)		return -1;
	return 0;
}

INT CPriceSum::GetBookCntPriceSum(CESL_DataMgr *pDM, CString &bookcnt, CString &speciescnt, CString &pricesum, CString code)
{
	INT ids, i, rowCnt;	
	CString tmpcode, tmpbcnt, tmpscnt, tmppsum;	
	tmpbcnt = tmpscnt = tmppsum = _T("0");
	
	if( pDM == NULL ) return -1000;

	rowCnt = pDM->GetRowCount();	
	for( i=0 ; i<rowCnt ; i++ )
	{
		ids = pDM->GetCellData(i, 0, tmpcode);
		if(ids < 0) return -1001;

		if(tmpcode == code)
		{
			pDM->GetCellData(i, 1, tmpbcnt);
			pDM->GetCellData(i, 2, tmpscnt);
			pDM->GetCellData(i, 3, tmppsum);
			break;
		}
	}

	bookcnt = tmpbcnt;	speciescnt = tmpscnt;	pricesum = tmppsum;

	return 0;
}

INT CPriceSum::SetReckeyList(CString CMAlias, CString tabName)
{
	INT ids = -1;
	CESL_ControlMgr * pCM = m_pParentMgr->FindCM(CMAlias);
	CString priceSumClass;
	pCM->GetControlMgrData(_T("가격합산구분"), priceSumClass);
	if(priceSumClass == _T("0"))
	{
		ids = SetMonthlyReckeyList(CMAlias);
		if(ids < 0) return -1000;
		else if( ids > 0) return ids;
	}
	else if(priceSumClass == _T("1"))
	{
		ids = SetDivisionNoReckeyList(CMAlias);
		if(ids < 0) return -1000;	
	}
	
	ids = InitTotalValue(FALSE, MONTHLY);
	if(ids < 0)
		return -1001;

	ids = MakePriceDM(-1, FALSE, tabName, MONTHLY);
	if(ids < 0) return -1002;

	return 0;
}

INT CPriceSum::SetMonthlyReckeyList(CString CMAlias)
{
	CString dateType, date1, date2;
	CESL_ControlMgr * pCM = m_pParentMgr->FindCM(CMAlias);
	pCM->GetControlMgrData(_T("가격합산날짜구분"), dateType);
	pCM->GetControlMgrData(_T("날짜1"), date1);
	pCM->GetControlMgrData(_T("날짜2"), date2);
	CString tmpquery;
	if(dateType == _T("1"))
		tmpquery.Format(_T("SELECT B.REC_KEY FROM BO_PURCHASEINFO_TBL P, BO_BOOK_TBL B WHERE P.MANAGE_CODE = UDF_MANAGE_CODE AND P.REC_KEY = B.ACCU_KEY AND B.BOOK_APPENDIX_FLAG = 'B'"));
	else
		tmpquery.Format(_T("SELECT B.REC_KEY FROM BO_BOOK_TBL B WHERE B.MANAGE_CODE = UDF_MANAGE_CODE AND B.BOOK_APPENDIX_FLAG = 'B'"));

	CString strOption = _T("");
	INT ids = MakeDateQuery(dateType, date1, date2, strOption);
	if(ids > 0) return ids;
	if(!strOption.IsEmpty())
		tmpquery += _T(" AND ") + strOption;
	m_strSubQuery = tmpquery;
	return 0;
}

INT CPriceSum::SetDivisionNoReckeyList(CString CMAlias)
{
	return 0;
}

INT CPriceSum::MakeDateQuery(CString dateType, CString date1, CString date2, CString &strOption)
{
	CString fieldName = _T("");

	if( dateType == _T("0") )			fieldName = _T("B.INPUT_DATE");
	else if( dateType == _T("1") )		fieldName = _T("P.INPUT_DATE");
	else if( dateType == _T("2") )		fieldName = _T("B.CHECKIN_DATE");
	else								return -1 ;

	if( date1.IsEmpty() == FALSE && date2.IsEmpty() == FALSE && date1 == date2 )
	{
		strOption.Format(_T("%s = '%s'"), fieldName, date1);
	}
	else if( date1.IsEmpty() == FALSE && date2.IsEmpty() == FALSE && date1 != date2 )
	{
		strOption.Format(_T("%s >= '%s' AND %s <= '%s'"), fieldName, date1, fieldName, date2);	
	}
	else if( date1.IsEmpty() == TRUE && date2.IsEmpty() == FALSE )
	{
		m_pParentMgr->ESLAfxMessageBox(_T("검색 시작일을 입력해 주세요."));
		return 1000;
	}
	else if( date1.IsEmpty() == FALSE && date2.IsEmpty() == TRUE )
	{
		strOption.Format(_T("%s = '%s'"), fieldName, date1);
	}
	else	strOption.Empty();

	return 0;
}

INT CPriceSum::MakeRecKey()
{
	INT nCnt, nDMRowCnt, i, ids;
	CStringArray ArrBookKey;
	CString strTmpData, strValue, strAcqKey, strVolSortNo;
	CStringArray* pArray = new CStringArray;
	
	if( m_pParentBookDM == NULL ) return -1;
	
	nCnt = m_MapVolInfo.GetCount();
	if(nCnt == 0) return 0;

	nDMRowCnt = m_pParentBookDM->GetRowCount();
	if(nDMRowCnt == 0) return 0;
	
	for( i=0 ; i<nDMRowCnt ; i++ )
	{
		strTmpData.Format(_T("%s/%s"),
			m_pParentBookDM->GetCellData(_T("BP_수서정보KEY"), i), 
			m_pParentBookDM->GetCellData(_T("BB_권일련번호"), i));

		if( m_MapVolInfo.Lookup(strTmpData, strValue) )
		{
			ArrBookKey.Add(m_pParentBookDM->GetCellData(_T("BB_책KEY"), i));
		}
	}

	ids = DeteleRecKeyArray();
	
	nCnt = ArrBookKey.GetSize();
	for( i=0 ; i<nCnt ; i++ )
	{
		if( i == SPECIES_DATA_LIMIT )
		{
			m_ReckeyListArray.Add(pArray);
			pArray = new CStringArray;
		}
		pArray->Add(ArrBookKey.GetAt(i));
	}
	m_ReckeyListArray.Add(pArray);

	return 0;
}

INT CPriceSum::SetReckeyList(CESL_ControlMgr *pCM, CString strSelectedTab, PRICE_SUM_TYPE nPSumType)
{	
	INT ids = -1;

	switch( nPSumType )
	{
	case MONTHLY:		
		ids = SetMonthlyReckeyList(pCM);
		break;
	case DIVISION_NO:	
		ids = SetDivisionNoReckeyList(pCM);
		break;
	default:			
		return -1;
		break;
	}
	
	if ( ids < 0 ) 
	{
		return -1000;
	}
	else if( ids > 0 )
	{
		return ids;
	}	
	
	ids = InitTotalValue(FALSE, nPSumType);
	if(ids < 0)	return -1001;

	ids = MakePriceDM(-1, FALSE, strSelectedTab, nPSumType);
	if(ids < 0) return -1002;

	return 0;
}

INT CPriceSum::SetMonthlyReckeyList(CESL_ControlMgr *pCM)
{
	INT ids;
	CString dateType, date1, date2, tmpquery, strOption = _T("");
	
	pCM->GetControlMgrData(_T("가격합산날짜구분"), dateType);
	pCM->GetControlMgrData(_T("날짜1"), date1);
	pCM->GetControlMgrData(_T("날짜2"), date2);
	
	if( dateType == _T("1") )
	{
		tmpquery.Format(_T("SELECT B.REC_KEY FROM BO_PURCHASEINFO_TBL P, BO_BOOK_TBL B ")
				_T("WHERE P.REC_KEY=B.ACQ_KEY AND P.MANAGE_CODE = UDF_MANAGE_CODE AND B.BOOK_APPENDIX_FLAG = 'B'"));
	}
	else
	{
		tmpquery.Format(_T("SELECT B.REC_KEY FROM BO_BOOK_TBL B WHERE B.MANAGE_CODE = UDF_MANAGE_CODE AND B.BOOK_APPENDIX_FLAG = 'B'"));
	}

	ids = MakeDateQuery(dateType, date1, date2, strOption);
	if( ids < 0 )
	{
		if( IDNO == AfxMessageBox( _T("가격합산구분에 따른 검색조건으로 가격합산을 할 수가 없습니다.\n전체 날짜에 대하여 검색할까요?"), MB_YESNO | MB_ICONQUESTION ) )
		{
			return -1;
		}
	}
	else if( ids > 0 )	return ids;

	if( strOption.IsEmpty() == FALSE )		tmpquery += _T(" AND ") + strOption;

	m_strSubQuery = tmpquery;
	
	return 0;
}

INT CPriceSum::SetDivisionNoReckeyList(CESL_ControlMgr *pCM)
{
	CString strAcqYear, strDivNoList, strWorkCode;

	pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	pCM->GetControlMgrData(_T("차수번호"), strDivNoList);
	pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	
	if( strWorkCode.IsEmpty() == TRUE )
	{
		m_strSubQuery.Format(
			_T("SELECT B.REC_KEY FROM IDX_BO_TBL I, BO_BOOK_TBL B WHERE I.REC_KEY = B.SPECIES_KEY AND ")
			_T("I.MANAGE_CODE = UDF_MANAGE_CODE AND I.WORK_CODE IS NULL AND BOOK_APPENDIX_FLAG = 'B' AND ACQ_YEAR = '%s' AND ACQ_CODE = '1' ")
			_T("AND SEQ_NO IN (%s)"),
			strAcqYear, strDivNoList);
	}
	else
	{
		m_strSubQuery.Format(
			_T("SELECT B.REC_KEY FROM IDX_BO_TBL I, BO_BOOK_TBL B WHERE I.REC_KEY = B.SPECIES_KEY AND ")
			_T("I.MANAGE_CODE = UDF_MANAGE_CODE AND B.BOOK_APPENDIX_FLAG = 'B' AND B.ACQ_YEAR = '%s' AND B.ACQ_CODE = '1' AND ")
			_T("B.SEQ_NO IN (%s) AND I.WORK_CODE = '%s'"),
			strAcqYear, strDivNoList, strWorkCode);
	}

	return 0;
}

INT CPriceSum::MakePriceDMGridSelect(BOOL bSpeciesYN, CString strCodeName)
{
	INT cnt, ids, i ;
	CString strDMPK;
	CESL_DataMgr* pMergeDM = NULL;
	CESL_DataMgr * pDM = NULL;
	CArray <CESL_DataMgr*, CESL_DataMgr*> m_DataArray;
	CGroupMgr mgr(m_pParentMgr);
	
	cnt = m_ReckeyListArray.GetSize();
	for( i=0 ; i<cnt ; i++ )
	{
		pDM = new CESL_DataMgr;
		pDM->SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
		pDM->InitDMPKFromFile(m_pDM->DB_MGR_ALIAS);
		pDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
		ids = MakePriceDMGridSelect(pDM, strCodeName, bSpeciesYN, m_ReckeyListArray.GetAt(i));
		if( ids < 0 ) continue;
		
		m_DataArray.Add(pDM);
	}
	
	ids = MergePriceSumDM(&m_DataArray, pMergeDM);
	if(ids < 0) 
	{
		cnt = m_DataArray.GetSize();
		for(INT i=0 ; i<cnt ; i++)
		{
			delete m_DataArray.GetAt(i);
		}
		m_DataArray.RemoveAll();
		return ids;
	}
	
	m_pDM->FreeData();
	pMergeDM = m_DataArray.GetAt(0);
	ids = CopyPriceSumDM(m_pDM, pMergeDM);

	cnt = m_DataArray.GetSize();
	for(i=0 ; i<cnt ; i++)
	{
		delete m_DataArray.GetAt(i);
	}
	m_DataArray.RemoveAll();
	if(ids < 0) return ids;
	
	return 0;
}

INT CPriceSum::CopyPriceSumDM(CESL_DataMgr *pTargetDM, CESL_DataMgr *pMergeDM)
{
	INT nRowCnt, nCurRow,i;
	CString strData[6];
	CString strAlias[6] = {	_T("설명"), 
							_T("종수"), 
							_T("책수"), 
							_T("금액"), 
							_T("금액비율"), 
							_T("책비율") };
	
	
	if(pMergeDM == NULL || pTargetDM == NULL) return -1;

	nRowCnt = pMergeDM->GetRowCount();	
	for( i=0 ; i<nRowCnt ; i++ )
	{
		pMergeDM->GetCellData(strAlias, 6, strData, i);
		pTargetDM->InsertRow(-1);
		nCurRow = pTargetDM->GetRowCount() - 1;
		pTargetDM->SetCellData(strAlias, 6, strData, nCurRow);
	}

	return 0;
}

INT CPriceSum::MergePriceSumDM(CArray <CESL_DataMgr *, CESL_DataMgr *> * pDataArray, CESL_DataMgr * pMergeDM)
{
	INT cnt, ids, i ;
	CESL_DataMgr* pDM = NULL;
	
	if(pDataArray == NULL) return -1;
	
	cnt = pDataArray->GetSize();
	if( cnt == 0 ) return -1;
	else if( cnt == 1 )
	{
		pMergeDM = pDataArray->GetAt(0);
		ids = SetRateInfo(pMergeDM);	
	}
	else
	{
	pMergeDM = pDataArray->GetAt(0);
		for( i=1 ; i<cnt ; i++ )
	{
		pDM = pDataArray->GetAt(i);
		ids = MergePriceSumDM(pMergeDM, pDM);
		if(ids < 0)	continue;
	}
		// 4.
	ids = SetRateInfo(pMergeDM);
	}
	return 0;
}

INT CPriceSum::SetRateInfo(CESL_DataMgr *pMergeDM)
{
	INT nRowCnt, i ;
	FLOAT fData;
	CString strTmpData;	
	
	if(pMergeDM == NULL) return -1;
	
	nRowCnt = pMergeDM->GetRowCount();	
	for( i=0 ; i<nRowCnt ; i++ )
	{
		strTmpData = pMergeDM->GetCellData(_T("책수"), i);
		TCHAR* pEndPtr = NULL;
		fData = (FLOAT)(_tcstod(strTmpData.GetBuffer(0), &pEndPtr))/m_TotalBookCnt * 100;

		strTmpData.Format(_T("%5.2f%%"), fData);
		pMergeDM->SetCellData(_T("책비율"), strTmpData, i);

		strTmpData = pMergeDM->GetCellData(_T("금액"), i);
		if(m_TotalPrice == 0)		fData = 0;
		else					fData = (FLOAT)(_tcstod(strTmpData.GetBuffer(0), &pEndPtr)/m_TotalPrice * 100);
		strTmpData.Format(_T("%5.2f%%"), fData);
		pMergeDM->SetCellData(_T("금액비율"), strTmpData, i);
	}
	return 0;
}

INT CPriceSum::MergePriceSumDM(CESL_DataMgr *pMergeDM, CESL_DataMgr *pDM)
{
	INT nRowCnt, nBookCnt1, nBookCnt2, nBookCnt, i;
	DOUBLE dPrice1, dPrice2, dPrice;
	CString strTmpData;

	if(pMergeDM == NULL || pDM == NULL) return -1;
	
	nRowCnt = pMergeDM->GetRowCount();	
	for( i=0 ; i<nRowCnt ; i++ )
	{
		strTmpData = pMergeDM->GetCellData(_T("종수"), i);
		nBookCnt1 = _ttoi(strTmpData.GetBuffer(0));
		strTmpData = pDM->GetCellData(_T("종수"), i);
		nBookCnt2 = _ttoi(strTmpData.GetBuffer(0));
		nBookCnt = nBookCnt1 + nBookCnt2;
		strTmpData.Format(_T("%d"), nBookCnt);
		pMergeDM->SetCellData(_T("종수"), strTmpData, i);

		strTmpData = pMergeDM->GetCellData(_T("책수"), i);
		nBookCnt1 = _ttoi(strTmpData.GetBuffer(0));
		strTmpData = pDM->GetCellData(_T("책수"), i);
		nBookCnt2 = _ttoi(strTmpData.GetBuffer(0));
		nBookCnt = nBookCnt1 + nBookCnt2;
		strTmpData.Format(_T("%d"), nBookCnt);
		pMergeDM->SetCellData(_T("책수"), strTmpData, i);

		strTmpData = pMergeDM->GetCellData(_T("금액"), i);
		strTmpData.Replace(_T(","), _T(""));
		TCHAR* pEndPtr = NULL;
		dPrice1 = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		strTmpData = pDM->GetCellData(_T("금액"), i);
		dPrice2 = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		dPrice = dPrice1 + dPrice2;
		strTmpData.Format(_T("%.0f"), dPrice);
		pMergeDM->SetCellData(_T("금액"), strTmpData, i);
	}

	return 0;
}

INT CPriceSum::MakePriceDMGridSelect(CESL_DataMgr *pDM, CString strCodeName, BOOL bSpeciesYN, CStringArray * pReckeyArray)
{	
	INT ids, startrow, codeCnt, i, nRow ;
	CString strQeury, strGroupByCode, strCodeClassName, strHeadQuery, strMidQuery, strTailQuery, strTmpData;
	CString code, desc, bookcnt, pricesum, speciescnt;
	CString codes[6];
	FLOAT bookrate, pricerate;	
	CESL_DataMgr tmpDM;
	
	pDM->FreeData();
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	GetCodeNameRelativeInfo(strCodeName, strGroupByCode, strCodeClassName, strHeadQuery);

	ids = MakeReckeyArrayQuery(bSpeciesYN, pReckeyArray, strMidQuery);
	if(ids < 0) return ids;
	if( strMidQuery.IsEmpty() == TRUE ) return -1;
		
	strTailQuery.Format(_T(" GROUP BY %s"), strGroupByCode);

	strQeury = strHeadQuery + strMidQuery + strTailQuery;
	
	ids = tmpDM.RestructDataManager(strQeury);
	if(ids < 0) return -1002;

	startrow = 0;
	bookrate = pricerate = 0.0;
	codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(strCodeClassName);
	for( i=0 ; i<codeCnt ; i++ )
	{
		m_pParentMgr->m_pInfo->pCodeMgr->GetCode(strCodeClassName, _T("코드"), code, startrow);
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(strCodeClassName, _T("설명"), desc, startrow);

		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, _T(""));
		else
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, speciescnt, pricesum, code);
		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		nRow = pDM->GetRowCount() - 1;
		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("종수"), speciescnt, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		
		startrow++;
	}

	return 0;
}

VOID CPriceSum::GetCodeNameRelativeInfo(CString strCodeName, CString &strGroupByCode, CString &strCodeClassName, CString &strHeadQuery)
{
	if( strCodeName == _T("KDC주제구분") )
	{
		strGroupByCode		= _T("p.SUBJECT_CODE");
		strCodeClassName = _T("KDC대주제부호관리");
		CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
			strHeadQuery.Format(
				_T("SELECT %s, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
				_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
				_T("FROM IDX_BO_TBL i, BO_BOOK_TBL b, BO_PURCHASEINFO_TBL p ")
				_T("WHERE i.REC_KEY = p.SPECIES_KEY AND i.MANAGE_CODE = UDF_MANAGE_CODE AND p.REC_KEY = b.ACQ_KEY AND b.BOOK_APPENDIX_FLAG = 'B' ")
				_T("AND i.CLASS_NO_TYPE = '1' AND ")
				, strGroupByCode, strPriceWon);

	}
	else if( strCodeName == _T("LDC주제구분") )
	{
		strGroupByCode = _T("p.SUBJECT_CODE");
		strCodeClassName = _T("DDC대주제부호관리");
		CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
		strHeadQuery.Format(
			_T("SELECT %s, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
			_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
			_T("FROM IDX_BO_TBL i, BO_BOOK_TBL b, BO_PURCHASEINFO_TBL p ")
			_T("WHERE i.REC_KEY = p.SPECIES_KEY AND p.REC_KEY = b.ACQ_KEY AND i.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' ")
			_T("AND i.CLASS_NO_TYPE = '2' AND ")
			, strGroupByCode, strPriceWon);

	}	
	else if( strCodeName == _T("등록구분") )
	{
		strGroupByCode = _T("b.REG_CODE");
		strCodeClassName = _T("등록구분");
		CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
		strHeadQuery.Format(
			_T("SELECT %s, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
			_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
			_T("FROM BO_BOOK_TBL b WHERE b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' AND ")
			, strGroupByCode, strPriceWon);
		
	}
	else if( strCodeName == _T("별치구분") )
	{
		strGroupByCode = _T("b.SEPARATE_SHELF_CODE");
		strCodeClassName = _T("별치기호");
		CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
		strHeadQuery.Format(
			_T("SELECT %s, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
			_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
			_T("FROM BO_BOOK_TBL b WHERE b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' AND ")
			,strGroupByCode, strPriceWon);

	}
	else if( strCodeName == _T("형식구분") )
	{
		strGroupByCode = _T("s.FORM_CODE");
		strCodeClassName = _T("형식구분");
		CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
		strHeadQuery.Format(
			_T("SELECT %s, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
			_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
			_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b ")
			_T("WHERE s.REC_KEY = b.SPECIES_KEY AND b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' AND ")
			, strGroupByCode, strPriceWon);
	}
	else if( strCodeName == _T("매체구분") )
	{
		strGroupByCode = _T("b.MEDIA_CODE");
		strCodeClassName = _T("매체구분");
		CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
		strHeadQuery.Format(
			_T("SELECT %s, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
			_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
			_T("FROM BO_BOOK_TBL b WHERE b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' AND ")
			, strGroupByCode, strPriceWon);

	}
	else if( strCodeName == _T("보조등록구분") )
	{
		strGroupByCode = _T("s.SUB_REG_CODE");
		strCodeClassName = _T("보조등록구분");
		CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
		strHeadQuery.Format(
			_T("SELECT %s, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
			_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
			_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b ")
			_T("WHERE s.REC_KEY = b.SPECIES_KEY AND b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' AND ")
			,strGroupByCode, strPriceWon);
		
	}
	else if( strCodeName == _T("도서분류코드1") )
	{
		strGroupByCode = _T("s.USER_DEFINE_CODE1");
		strCodeClassName = _T("도서분류코드1");
		CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
		strHeadQuery.Format(
			_T("SELECT %s, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
			_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
			_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b ")
			_T("WHERE s.REC_KEY = b.SPECIES_KEY AND b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' AND ")
			,strGroupByCode, strPriceWon);

	}
	else if( strCodeName == _T("도서분류코드2") ) 
	{
		strGroupByCode = _T("s.USER_DEFINE_CODE2");
		strCodeClassName = _T("도서분류코드2");
		CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
		strHeadQuery.Format(
			_T("SELECT %s, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
			_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
			_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b ")
			_T("WHERE s.REC_KEY = b.SPECIES_KEY AND b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' AND ")
			,strGroupByCode, strPriceWon);
		
	}
	else if( strCodeName == _T("도서분류코드3") )
	{
		strGroupByCode = _T("s.USER_DEFINE_CODE3");
		strCodeClassName = _T("도서분류코드3");
		CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
		strHeadQuery.Format(
			_T("SELECT %s, COUNT(b.REC_KEY), COUNT(DISTINCT b.SPECIES_KEY), ")
			_T("ROUND(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), 2) ")
			_T("FROM BO_SPECIES_TBL s, BO_BOOK_TBL b ")
			_T("WHERE s.REC_KEY = b.SPECIES_KEY AND b.MANAGE_CODE = UDF_MANAGE_CODE AND b.BOOK_APPENDIX_FLAG = 'B' AND ")
			,strGroupByCode, strPriceWon);

	}
}

INT CPriceSum::MakeReckeyArrayQuery(BOOL bSpeciesYN, CStringArray *pReckeyArray, CString &strQuery)
{
	INT cnt, i ;
	CString strTmpData, strTmpQuery;

	if(pReckeyArray == NULL) return -1;
	
	strQuery.Empty();
	strTmpQuery.Empty();
	cnt = pReckeyArray->GetSize();
	for( i=0 ; i<cnt ; i++ )
	{
		strTmpData = pReckeyArray->GetAt(i);
		if( strTmpQuery.IsEmpty() == TRUE )		strTmpQuery = strTmpData;
		else strTmpQuery += _T(", ") + strTmpData;
	}

	if( bSpeciesYN == TRUE )
	{
		strQuery.Format(_T("b.ACQ_KEY IN (%s) %s"), strTmpQuery, m_strWorkingStatusWhereStmt);
	}
	else
	{
		strQuery.Format(_T("b.REC_KEY IN (%s) "), strTmpQuery);
	}

	return 0;
}

INT CPriceSum::InitTotalValueGridSelect(BOOL bSpecies)
{
	INT ids, nBookCnt, nSpeciesCnt, cnt, i ;
	DOUBLE dPrice;
	CString strQuery, strBookCntQuery, strPriceQuery, strSubQuery, strTmpData;
	CStringArray* pReckeyList = NULL;
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	
	cnt = m_ReckeyListArray.GetSize();	
	strBookCntQuery = _T("SELECT COUNT(B.REC_KEY), COUNT(DISTINCT(B.SPECIES_KEY)) FROM BO_BOOK_TBL B WHERE ");
	
	CString strPriceWon = (m_bApplyDiscRate) ? _T("Y") : _T("N");
	strPriceQuery.Format(_T("SELECT GET_ORDERPRICE(SUM(GetPriceWon(b.ACQ_KEY, b.PRICE, '%s', UDF_MANAGE_CODE)), UDF_MANAGE_CODE) ")
		_T("FROM BO_BOOK_TBL B WHERE B.MANAGE_CODE = UDF_MANAGE_CODE AND ")
		, strPriceWon);

	nBookCnt = nSpeciesCnt = 0;	
	dPrice = 0.0;
	for( i=0 ; i<cnt ; i++ )
	{
		pReckeyList = m_ReckeyListArray.GetAt(i);
		ids = MakeReckeyArrayQuery(bSpecies, pReckeyList, strSubQuery);
		if(ids < 0) continue;
		if( strSubQuery.IsEmpty() == TRUE )  continue;
		strQuery = strBookCntQuery + strSubQuery;

		ids = tmpDM.RestructDataManager(strQuery);
		if(ids < 0) return -1;

		strTmpData = tmpDM.GetCellData(0, 0);
		nBookCnt += _ttoi(strTmpData.GetBuffer(0));

		strTmpData = tmpDM.GetCellData(0, 1);
		nSpeciesCnt += _ttoi(strTmpData.GetBuffer(0));

		strQuery = strPriceQuery + strSubQuery;
		ids = m_pDM->GetOneValueQuery(strQuery, strTmpData);
		if(ids < 0) return -1;

		TCHAR* pEndPtr = NULL;
		dPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

	}

	m_TotalSpeciesCnt = nSpeciesCnt;
	m_TotalBookCnt = nBookCnt;
	m_TotalPrice = (DOUBLE)dPrice;

	return 0;
}

INT CPriceSum::DeteleRecKeyArray()
{
	INT cnt = m_ReckeyListArray.GetSize();

	for(INT i=0 ; i<cnt ; i++)
	{
		delete m_ReckeyListArray.GetAt(i);
	}
	m_ReckeyListArray.RemoveAll();

	return 0;
}
