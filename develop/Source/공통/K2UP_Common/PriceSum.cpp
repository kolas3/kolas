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
	m_strSubQuery = _T("");

	// KOL.UDF.022 시큐어코딩 보완
	m_TotalPrice = 0;
	m_TotalBookCnt = 0;
	m_TotalSpeciesCnt = 0;
}

CPriceSum::~CPriceSum()
{

}

//return값 검사
// -1000 : 선택목록을 구성할 수 없습니다.
// -1001 : 총책수/총종소 등의 값을 얻어올 수 없습니다.
// -1002 : 주제별/등록구분별/형식구분별/별치구분별 가격합산DM을 작성할 수 없습니다.
INT CPriceSum::InitPriceSum(CString CMAlias, INT tabCount, BOOL isSpecies, BOOL isMissingData)
{
	CString DMAlias = GetDMAlias(tabCount, isSpecies);
	CString GridAlias = GetGridAlias(tabCount, isSpecies);

	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);

	INT ids = m_pParentMgr->SelectMakeList(CMAlias, GridAlias);
	if(ids < 0)
		return -1000;
	
	INT firstPos = m_pParentMgr->SelectGetHeadPosition(CMAlias, GridAlias);
	if(firstPos < 0)
		return -1000;
	
	INT i = firstPos;
	CString rec_key = _T("");

	while(i >= 0)
	{
		if(isSpecies)
			pDM->GetCellData(_T("BP_구입정보KEY"), i, rec_key);
		else
			pDM->GetCellData(_T("BB_책KEY"), i, rec_key);
		m_ReckeyList.AddTail(rec_key);
		i = m_pParentMgr->SelectGetNext(CMAlias, GridAlias);
	}

		if(tabCount == 0)
		m_strWorkingStatusWhereStmt = _T(" AND B.WORKING_STATUS < 'BOC'");
	else if(tabCount == 1)
		m_strWorkingStatusWhereStmt = _T(" AND B.WORKING_STATUS = 'BOA111N'");
	else if(tabCount == 2 && isMissingData)
		m_strWorkingStatusWhereStmt = _T(" AND B.WORKING_STATUS = 'BOA114O'");
	else if(tabCount == 2 && !isMissingData)
		m_strWorkingStatusWhereStmt = _T(" AND B.WORKING_STATUS = 'BOA113N'");
	else if(tabCount == 3)
		m_strWorkingStatusWhereStmt = _T(" AND B.WORKING_STATUS = 'BOA141N'");

	if(!isSpecies)
		ids = InitTotalValue(FALSE, 1);
	else 
		ids = InitTotalValue(TRUE, 1);
	if(ids < 0)
		return -1001;

	ids = MakePriceDM(tabCount, isSpecies, _T("KDC주제구분"), 1);
	if(ids < 0)
		return -1002;

	return 0;
}

CString CPriceSum::GetDMAlias(INT tabCount, BOOL isSpecies)
{
	if(!isSpecies)
		return _T("DM_LIGHT_단행_수서_책");
	return _T("DM_LIGHT_단행_수서_종");
}

CString CPriceSum::GetGridAlias(INT tabCount, BOOL isSpecies)
{
	switch(tabCount)
	{
	case 0:
		if(isSpecies)
			return _T("MainGrid_All");
		else
			return _T("MainGrid_All_Book");
	case 1:
		if(isSpecies)
			return _T("MainGrid_Purchasing");
		else
			return _T("MainGrid_Purchasing_Book");
	case 2:
		if(isSpecies)
			return _T("MainGrid_CheckIn_Species");
		return _T("MainGrid_CheckIn_Book");
	case 3:
		if(isSpecies)
			return _T("MainGrid_CheckIn_Complete");
		else
			return _T("MainGrid_CheckIn_Complete_Book");
	}
	return _T("");
}

INT CPriceSum::MakePriceDM(INT tabCount, BOOL isSpecies, CString tabName, INT type)
{
	// KOL.UDF.022 시큐어코딩 보완
	INT ids = -1;

	CString DMAlias = _T("DM_가격합산");

	if(!isSpecies)
	{
		if(tabName == _T("KDC주제구분"))
			ids = MakeBookPriceDM_KDC_Subject(DMAlias, type);
		else if(tabName == _T("LDC주제구분"))
			ids = MakeBookPriceDM_LDC_Subject(DMAlias, type);
		else if(tabName == _T("등록구분"))
			ids = MakeBookPriceDM_REG_Code(DMAlias, type);
		else if(tabName == _T("별치구분"))
			ids = MakeBookPriceDM_SEP_Code(DMAlias, type);
		else if(tabName == _T("형식구분"))
			ids = MakeBookPriceDM_FORM_Code(DMAlias, type);
	}
	else
	{
		if(tabName == _T("KDC주제구분"))
			ids = MakeSpeciesPriceDM_KDC_Subject(DMAlias);
		else if(tabName == _T("LDC주제구분"))
			ids = MakeSpeciesPriceDM_LDC_Subject(DMAlias);
		else if(tabName == _T("등록구분"))
			ids = MakeSpeciesPriceDM_REG_Code(DMAlias);
		else if(tabName == _T("별치구분"))
			ids = MakeSpeciesPriceDM_SEP_Code(DMAlias);
		else if(tabName == _T("형식구분"))
			ids = MakeSpeciesPriceDM_FORM_Code(DMAlias);
	}

	if(ids < 0)
		return -1;
	return 0;
}

//return값 검사
// -1000 : 가격합산 DM을 구찾을 수 업습니다.
// -1001 : 가격합산 리스트를 구축할 수 없습니다.
// -1002 : 가격합산을 위한 임시DM을 구축할 수 없습니다.
// -1003 : 가격합산을 위한 임시DM이 NULL이거나 값을 찾을 수 없습니다.
INT CPriceSum::MakeSpeciesPriceDM_KDC_Subject(CString DMAlias)
{
	
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(!pDM) return -1000;

	pDM->FreeData();

	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString headQuery = 
		_T("SELECT p.subject_code, COUNT (b.rec_key), SUM (GetPriceWon(b.accu_key, b.price)) FROM bo_species_tbl s, bo_book_tbl b, bo_purchaseinfo_tbl p WHERE s.rec_key = p.species_key AND b.species_key = s.rec_key and b.accu_key = p.rec_key and b.book_appendix_flag='B' AND s.CLASS_NO_TYPE = '1' ") + m_strWorkingStatusWhereStmt + _T(" ");
		
	CString midQuery, tailQuery, tmpstr, tmpQuery;
	//tmpstr = MakeSpeciesQuery();
	tmpstr = MakeRecKeyQuery();
	if(tmpstr.IsEmpty())
		return -1001;
	else
		midQuery.Format(_T(" AND p.REC_KEY IN %s"), tmpstr);

	tailQuery = _T(" GROUP BY p.subject_code");
		
	tmpQuery = headQuery + midQuery + tailQuery;

	INT ids = tmpDM.RestructDataManager(tmpQuery);

	if(ids < 0)
		return -1002;

	CString codes[6];
	CString code, desc, bookcnt, pricesum;
	INT startrow = 0;
	float bookrate = 0.0;
	float pricerate = 0.0;
	INT codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("KDC대주제부호관리"));
	for(INT i=0 ; i<codeCnt ; i++)
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("KDC대주제부호관리"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		if(code == _T("") || desc == _T("적용안함"))
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, _T(""));
		else
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, code);	

		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;
		
		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		bookrate = (float)atof(bookcnt)/m_TotalBookCnt * 100;
		tmpstr.Format(_T("%5.2f%%"), bookrate);
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)
			pricerate = 0;
		else
			pricerate = (float)atof(pricesum)/m_TotalPrice * 100;
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery=_T("");
		startrow++;
	}

	return 0;
}

//return값 검사
// -1000 : 가격합산 DM을 구찾을 수 업습니다.
// -1001 : 가격합산 리스트를 구축할 수 없습니다.
// -1002 : 가격합산을 위한 임시DM을 구축할 수 없습니다.
// -1003 : 가격합산을 위한 임시DM이 NULL이거나 값을 찾을 수 없습니다.
INT CPriceSum::MakeSpeciesPriceDM_LDC_Subject(CString DMAlias)
{
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(!pDM) return -1000;

	pDM->FreeData();

	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);

	CString headQuery = 
		_T("SELECT p.subject_code, COUNT (b.rec_key), SUM (GetPriceWon(b.accu_key, b.price)) FROM bo_species_tbl s, bo_book_tbl b, bo_purchaseinfo_tbl p WHERE s.rec_key = p.species_key AND b.species_key = s.rec_key and b.accu_key = p.rec_key and b.book_appendix_flag='B' AND s.CLASS_NO_TYPE = '2' ")  + m_strWorkingStatusWhereStmt + _T(" ");;
		
	CString midQuery, tailQuery, tmpstr, tmpQuery;
	tmpstr = MakeRecKeyQuery();
	if(tmpstr.IsEmpty())
		return -1001;
	else
		midQuery.Format(_T(" AND p.REC_KEY IN %s"), tmpstr);

	tailQuery = _T(" GROUP BY p.subject_code");
		
	tmpQuery = headQuery + midQuery + tailQuery;

	INT ids = tmpDM.RestructDataManager(tmpQuery);

	if(ids < 0)
		return -1002;
	
	CString codes[6];
	CString code, desc, bookcnt, pricesum;
	INT startrow = 0;
	float bookrate = 0.0;
	float pricerate = 0.0;
	INT codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("DDC대주제부호관리"));
	for(INT i=0 ; i<codeCnt ; i++)
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("DDC대주제부호관리"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		if(code == _T("") || desc == _T("적용안함"))
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, _T(""));
		else
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, code);	

		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;
		
		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		bookrate = (float)atof(bookcnt)/m_TotalBookCnt * 100;
		tmpstr.Format(_T("%5.2f%%"), bookrate);
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)
			pricerate = 0;
		else
			pricerate = (float)atof(pricesum)/m_TotalPrice * 100;
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);
		
		tmpQuery = _T("");
		startrow++;
	}

	return 0;
}

//return값 검사
// -1000 : 가격합산 DM을 구찾을 수 업습니다.
// -1001 : 가격합산 리스트를 구축할 수 없습니다.
// -1002 : 가격합산을 위한 임시DM을 구축할 수 없습니다.
// -1003 : 가격합산을 위한 임시DM이 NULL이거나 값을 찾을 수 없습니다.
INT CPriceSum::MakeSpeciesPriceDM_REG_Code(CString DMAlias)
{
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(!pDM) return -1000;

	pDM->FreeData();

	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString headQuery =
		_T("SELECT b.reg_code, COUNT (b.rec_key), SUM (GetPriceWon(b.accu_key, b.price)) FROM bo_book_tbl b WHERE b.book_appendix_flag='B' ") + m_strWorkingStatusWhereStmt + _T(" AND ");
		
	CString tailQuery, tmpQuery, midQuery, tmpstr;
	//tmpstr = MakeBookQuery();
	tmpstr = MakeRecKeyQuery();
	if(tmpstr.IsEmpty()) return -1001;
	
	midQuery.Format(_T("b.accu_key IN %s"), tmpstr);
	tailQuery.Format(_T(" GROUP BY b.REG_CODE"));
	tmpQuery = headQuery + midQuery + tailQuery;

	INT ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;

	CString codes[6];
	CString code, desc, bookcnt, pricesum;
	INT startrow = 0;
	float bookrate = 0.0;
	float pricerate = 0.0;
	INT codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("등록구분"));
	for(INT i=0 ; i<codeCnt ; i++)
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("등록구분"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, _T(""));
		else
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, code);	

		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		bookrate = (float)atof(bookcnt)/m_TotalBookCnt * 100;
		tmpstr.Format(_T("%5.2f%%"), bookrate);
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)
			pricerate = 0;
		else
			pricerate = (float)atof(pricesum)/m_TotalPrice * 100;
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery=_T("");
		startrow++;
	}

	return 0;
}

//return값 검사
// -1000 : 가격합산 DM을 구찾을 수 업습니다.
// -1001 : 가격합산 리스트를 구축할 수 없습니다.
// -1002 : 가격합산을 위한 임시DM을 구축할 수 없습니다.
// -1003 : 가격합산을 위한 임시DM이 NULL이거나 값을 찾을 수 없습니다.
INT CPriceSum::MakeSpeciesPriceDM_SEP_Code(CString DMAlias)
{
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(!pDM) return -1000;

	pDM->FreeData();

	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString headQuery =
		_T("SELECT b.separate_shelf_code, COUNT (b.rec_key), SUM (GetPriceWon(b.accu_key, b.price)) FROM bo_book_tbl b WHERE b.book_appendix_flag='B' ") + m_strWorkingStatusWhereStmt + _T(" AND ");
		
	CString tailQuery, tmpQuery, midQuery, tmpstr;
	//tmpstr = MakeBookQuery();
	tmpstr = MakeRecKeyQuery();
	if(tmpstr.IsEmpty()) return -1001;
	
	midQuery.Format(_T("b.accu_key IN %s"), tmpstr);
	tailQuery.Format(_T(" GROUP BY b.separate_shelf_code"));
	tmpQuery = headQuery + midQuery + tailQuery;

	INT ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;

	CString codes[6];
	CString code, desc, bookcnt, pricesum;
	INT startrow = 0;
	float bookrate = 0.0;
	float pricerate = 0.0;
	INT codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("별치기호"));
	for(INT i=0 ; i<codeCnt ; i++)
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("별치기호"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, _T(""));
		else
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, code);	

		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		bookrate = (float)atof(bookcnt)/m_TotalBookCnt * 100;
		tmpstr.Format(_T("%5.2f%%"), bookrate);
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)
			pricerate = 0;
		else
			pricerate = (float)atof(pricesum)/m_TotalPrice * 100;
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery=_T("");
		startrow++;
	}

	return 0;
}

//return값 검사
// -1000 : 가격합산 DM을 구찾을 수 업습니다.
// -1001 : 가격합산 리스트를 구축할 수 없습니다.
// -1002 : 가격합산을 위한 임시DM을 구축할 수 없습니다.
// -1003 : 가격합산을 위한 임시DM이 NULL이거나 값을 찾을 수 없습니다.
INT CPriceSum::MakeSpeciesPriceDM_FORM_Code(CString DMAlias)
{
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(!pDM) return -1000;

	pDM->FreeData();

	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString headQuery =
		_T("SELECT s.form_code, COUNT (b.rec_key), SUM (GetPriceWon(b.accu_key, b.price)) FROM bo_species_tbl s, bo_book_tbl b WHERE s.rec_key = b.species_key and b.book_appendix_flag='B' ") + m_strWorkingStatusWhereStmt + _T(" AND ");
		
	CString tailQuery, tmpQuery, midQuery, tmpstr;
	//tmpstr = MakeBookQuery();
	tmpstr = MakeRecKeyQuery();
	if(tmpstr.IsEmpty()) return -1001;
	
	midQuery.Format(_T("b.accu_key IN %s"), tmpstr);
	tailQuery.Format(_T(" GROUP BY s.FORM_CODE"));
	tmpQuery = headQuery + midQuery + tailQuery;

	INT ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;

	CString codes[6];
	CString code, desc, bookcnt, pricesum;
	INT startrow = 0;
	float bookrate = 0.0;
	float pricerate = 0.0;
	INT codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("형식구분"));
	for(INT i=0 ; i<codeCnt ; i++)
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("형식구분"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, _T(""));
		else
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, code);	

		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		bookrate = (float)atof(bookcnt)/m_TotalBookCnt * 100;
		tmpstr.Format(_T("%5.2f%%"), bookrate);
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)
			pricerate = 0;
		else
			pricerate = (float)atof(pricesum)/m_TotalPrice * 100;
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery=_T("");
		startrow++;
	}

	return 0;
}

//return값 검사
// -1000 : 가격합산 DM을 구찾을 수 업습니다.
// -1001 : 가격합산 리스트를 구축할 수 없습니다.
// -1002 : 가격합산을 위한 임시DM을 구축할 수 없습니다.
// -1003 : 가격합산을 위한 임시DM이 NULL이거나 값을 찾을 수 없습니다.
INT CPriceSum::MakeBookPriceDM_KDC_Subject(CString DMAlias, INT type)
{
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(!pDM) return -1000;

	pDM->FreeData();

	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString headQuery =
		_T("SELECT p.subject_code, COUNT (b.rec_key), SUM (GetPriceWon(b.accu_key, b.price)) FROM bo_species_tbl s, bo_book_tbl b, bo_purchaseinfo_tbl p WHERE b.species_key = s.rec_key AND s.rec_key = b.species_key AND b.accu_key = p.rec_key and b.book_appendix_flag='B' AND s.CLASS_NO_TYPE = '1'");
			
	CString tailQuery, tmpQuery, midQuery, tmpstr;
	if(type > 0)
		tmpstr = MakeRecKeyQuery();
		//tmpstr = MakeBookReckeyQuery();
	else
		tmpstr = _T("(") + m_strSubQuery + _T(")");
	if(tmpstr.IsEmpty()) return -1001;
	midQuery.Format(_T(" AND B.REC_KEY IN %s"), tmpstr);
	tailQuery = _T(" GROUP BY p.subject_code");

	tmpQuery = headQuery + midQuery + tailQuery;
	INT ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;

	CString codes[6];
	CString code, desc, bookcnt, pricesum;
	INT startrow = 0;
	float bookrate = 0.0;
	float pricerate = 0.0;
	INT codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("KDC대주제부호관리"));
	for(INT i=0 ; i<codeCnt ; i++)
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("KDC대주제부호관리"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();

		if(code == _T("") || desc == _T("적용안함"))
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, _T(""));
		else
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, code);	

		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;
		
		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		bookrate = (float)atof(bookcnt)/m_TotalBookCnt * 100;
		tmpstr.Format(_T("%5.2f%%"), bookrate);
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)
			pricerate = 0;
		else
			pricerate = (float)atof(pricesum)/m_TotalPrice * 100;
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery=_T("");
		startrow++;
	}

	return 0;
}

//return값 검사
// -1000 : 가격합산 DM을 구찾을 수 업습니다.
// -1001 : 가격합산 리스트를 구축할 수 없습니다.
// -1002 : 가격합산을 위한 임시DM을 구축할 수 없습니다.
// -1003 : 가격합산을 위한 임시DM이 NULL이거나 값을 찾을 수 없습니다.
INT CPriceSum::MakeBookPriceDM_LDC_Subject(CString DMAlias, INT type)
{
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(!pDM) return -1000;

	pDM->FreeData();

	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString headQuery =
		_T("SELECT p.subject_code, COUNT (b.rec_key), SUM (GetPriceWon(b.accu_key, b.price)) FROM bo_species_tbl s, bo_book_tbl b, bo_purchaseinfo_tbl p WHERE b.species_key = s.rec_key AND s.rec_key = b.species_key AND b.accu_key = p.rec_key and b.book_appendix_flag='B' AND s.CLASS_NO_TYPE = '2'");
			
	CString tailQuery, tmpQuery, midQuery, tmpstr;
	if(type > 0) tmpstr = MakeRecKeyQuery();
	else tmpstr = _T("(") + m_strSubQuery + _T(")");
	if(tmpstr.IsEmpty()) return -1001;
	midQuery.Format(_T(" AND B.REC_KEY IN %s"), tmpstr);
	tailQuery = _T(" GROUP BY p.subject_code");

	tmpQuery = headQuery + midQuery + tailQuery;
	INT ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;

	CString codes[6];
	CString code, desc, bookcnt, pricesum;
	INT startrow = 0;
	float bookrate = 0.0;
	float pricerate = 0.0;
	INT codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("DDC대주제부호관리"));
	for(INT i=0 ; i<codeCnt ; i++)
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("DDC대주제부호관리"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		if(code == _T("") || desc == _T("적용안함"))
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, _T(""));
		else
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, code);	

		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;
		
		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		bookrate = (float)atof(bookcnt)/m_TotalBookCnt * 100;
		tmpstr.Format(_T("%5.2f%%"), bookrate);
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)
			pricerate = 0;
		else
			pricerate = (float)atof(pricesum)/m_TotalPrice * 100;
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery=_T("");
		startrow++;
	}

	return 0;
}

//return값 검사
// -1000 : 가격합산 DM을 구찾을 수 업습니다.
// -1001 : 가격합산 리스트를 구축할 수 없습니다.
// -1002 : 가격합산을 위한 임시DM을 구축할 수 없습니다.
// -1003 : 가격합산을 위한 임시DM이 NULL이거나 값을 찾을 수 없습니다.
INT CPriceSum::MakeBookPriceDM_REG_Code(CString DMAlias, INT type)
{
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(!pDM) return -1000;

	pDM->FreeData();

	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString headQuery =
		_T("SELECT b.reg_code, COUNT (b.rec_key), SUM (GetPriceWon(b.accu_key, b.price)) FROM bo_book_tbl b WHERE b.book_appendix_flag='B' AND ");
		
	CString tailQuery, tmpQuery, midQuery, tmpstr;
	if(type > 0) tmpstr = MakeRecKeyQuery(); //tmpstr = MakeBookQuery();
	else tmpstr = _T("(") + m_strSubQuery + _T(")");
	if(tmpstr.IsEmpty()) return -1001;
	
	midQuery.Format(_T("b.REC_KEY IN %s"), tmpstr);
	tailQuery.Format(_T(" GROUP BY b.REG_CODE"));
	tmpQuery = headQuery + midQuery + tailQuery;

	INT ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;

	CString codes[6];
	CString code, desc, bookcnt, pricesum;
	INT startrow = 0;
	float bookrate = 0.0;
	float pricerate = 0.0;
	INT codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("등록구분"));
	for(INT i=0 ; i<codeCnt ; i++)
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("등록구분"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, _T(""));
		else
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, code);	

		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		bookrate = (float)atof(bookcnt)/m_TotalBookCnt * 100;
		tmpstr.Format(_T("%5.2f%%"), bookrate);
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)
			pricerate = 0;
		else
			pricerate = (float)atof(pricesum)/m_TotalPrice * 100;
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery=_T("");
		startrow++;
	}

	return 0;
}

//return값 검사
// -1000 : 가격합산 DM을 구찾을 수 업습니다.
// -1001 : 가격합산 리스트를 구축할 수 없습니다.
// -1002 : 가격합산을 위한 임시DM을 구축할 수 없습니다.
// -1003 : 가격합산을 위한 임시DM이 NULL이거나 값을 찾을 수 없습니다.
INT CPriceSum::MakeBookPriceDM_SEP_Code(CString DMAlias, INT type)
{
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(!pDM) return -1000;

	pDM->FreeData();

	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString headQuery =
		_T("SELECT b.separate_shelf_code, COUNT (b.rec_key), SUM (GetPriceWon(b.accu_key, b.price)) FROM bo_book_tbl b WHERE b.book_appendix_flag='B' AND ");
		
	CString tailQuery, tmpQuery, midQuery, tmpstr;
	if(type > 0) tmpstr = MakeRecKeyQuery(); //tmpstr = MakeBookQuery();
	else tmpstr = _T("(") + m_strSubQuery + _T(")");
	if(tmpstr.IsEmpty()) return -1001;
	
	midQuery.Format(_T("b.REC_KEY IN %s"), tmpstr);
	tailQuery.Format(_T(" GROUP BY b.separate_shelf_code"));
	tmpQuery = headQuery + midQuery + tailQuery;

	INT ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;

	CString codes[6];
	CString code, desc, bookcnt, pricesum;
	INT startrow = 0;
	float bookrate = 0.0;
	float pricerate = 0.0;
	INT codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("별치기호"));
	for(INT i=0 ; i<codeCnt ; i++)
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("별치기호"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, _T(""));
		else
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, code);	

		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		bookrate = (float)atof(bookcnt)/m_TotalBookCnt * 100;
		tmpstr.Format(_T("%5.2f%%"), bookrate);
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)
			pricerate = 0;
		else
			pricerate = (float)atof(pricesum)/m_TotalPrice * 100;
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery=_T("");
		startrow++;
	}

	return 0;
}

//return값 검사
// -1000 : 가격합산 DM을 구찾을 수 업습니다.
// -1001 : 가격합산 리스트를 구축할 수 없습니다.
// -1002 : 가격합산을 위한 임시DM을 구축할 수 없습니다.
// -1003 : 가격합산을 위한 임시DM이 NULL이거나 값을 찾을 수 없습니다.
INT CPriceSum::MakeBookPriceDM_FORM_Code(CString DMAlias, INT type)
{
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(!pDM) return -1000;

	pDM->FreeData();

	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString headQuery =
		_T("SELECT s.form_code, COUNT (b.rec_key), SUM (GetPriceWon(b.accu_key, b.price)) FROM bo_species_tbl s, bo_book_tbl b WHERE s.rec_key = b.species_key AND b.book_appendix_flag='B' AND ");
		
	CString tailQuery, tmpQuery, midQuery, tmpstr;
	if(type > 0) tmpstr = MakeRecKeyQuery(); //tmpstr = MakeBookQuery();
	else tmpstr = _T("(") + m_strSubQuery + _T(")");
	if(tmpstr.IsEmpty()) return -1001;
	
	midQuery.Format(_T("b.REC_KEY IN %s"), tmpstr);
	tailQuery.Format(_T(" GROUP BY s.FORM_CODE"));
	tmpQuery = headQuery + midQuery + tailQuery;

	INT ids = tmpDM.RestructDataManager(tmpQuery);
	if(ids < 0) return -1002;

	CString codes[6];
	CString code, desc, bookcnt, pricesum;
	INT startrow = 0;
	float bookrate = 0.0;
	float pricerate = 0.0;
	INT codeCnt = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("형식구분"));
	for(INT i=0 ; i<codeCnt ; i++)
	{
		startrow = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(_T("형식구분"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		if(code == _T("") || desc == _T("적용안함"))
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, _T(""));
		else
			ids = GetBookCntPriceSum(&tmpDM, bookcnt, pricesum, code);	

		if(ids < 0) return -1003;
		
		pDM->InsertRow(-1);
		INT nRow = pDM->GetRowCount() - 1;

		pDM->SetCellData(_T("설명"), desc, nRow);
		pDM->SetCellData(_T("책수"), bookcnt, nRow);
		bookrate = (float)atof(bookcnt)/m_TotalBookCnt * 100;
		tmpstr.Format(_T("%5.2f%%"), bookrate);
		pDM->SetCellData(_T("책비율"), tmpstr, nRow);
		pDM->SetCellData(_T("금액"), pricesum, nRow);
		if(m_TotalPrice == 0)
			pricerate = 0;
		else
			pricerate = (float)atof(pricesum)/m_TotalPrice * 100;
		tmpstr.Format(_T("%5.2f%%"), pricerate);
		pDM->SetCellData(_T("금액비율"), tmpstr, nRow);

		tmpQuery=_T("");
		startrow++;
	}
return 0;
}

CString CPriceSum::MakeRecKeyQuery()
{
	if(m_ReckeyList.IsEmpty())
		return _T("");

	CString query, subquery;
	POSITION pos = m_ReckeyList.GetHeadPosition();
	CString rec_key = m_ReckeyList.GetNext(pos);
	
	query.Format(_T("%s"), rec_key);

	while(pos != NULL)
	{
		rec_key = m_ReckeyList.GetNext(pos);
		subquery.Format(_T(", %s"), rec_key);
		query += subquery;
	}
	CString tmpquery ;
	tmpquery.Format(_T("( %s )"), query);

	return tmpquery;
}

/*
CString CPriceSum::MakeSpeciesQuery()
{
	if(m_ReckeyList.IsEmpty())
		return _T("");

	CString query, subquery;
	POSITION pos = m_ReckeyList.GetHeadPosition();
	CString rec_key = m_ReckeyList.GetNext(pos);
	
	query.Format(_T("%s"), rec_key);

	while(pos != NULL)
	{
		rec_key = m_ReckeyList.GetNext(pos);
		subquery.Format(_T(", %s"), rec_key);
		query += subquery;
	}
	CString tmpquery ;
	tmpquery.Format(_T("( %s )"), query);

	return tmpquery;
}
*/

INT CPriceSum::InitTotalValue(BOOL isSpecies, INT type)
{
	INT ids = 0;
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(_T("DM_가격합산"));
	if(!pDM)
		return -1;
	CString query, result;
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	if(isSpecies)
	{
		m_TotalSpeciesCnt = m_ReckeyList.GetCount();
		query = _T("SELECT COUNT(REC_KEY) FROM BO_BOOK_TBL B WHERE B.ACCU_KEY IN ");

		query += MakeRecKeyQuery() + m_strWorkingStatusWhereStmt;
		tmpDM.RestructDataManager(query);
		tmpDM.GetCellData(0, 0, result);
		m_TotalBookCnt = _ttoi(result.GetBuffer(0));
		
		query = _T("SELECT SUM(getpricewon(b.accu_key, b.price)) FROM BO_BOOK_TBL B, BO_PURCHASEINFO_TBL P WHERE B.ACCU_KEY=P.REC_KEY AND P.REC_KEY IN ");
		query += MakeRecKeyQuery() + m_strWorkingStatusWhereStmt;
		tmpDM.RestructDataManager(query);
		tmpDM.GetCellData(0, 0, result);
		m_TotalPrice = (float)atof(result.GetBuffer(0));
	}
	else if(!isSpecies && type > 0)
	{
		query = _T("SELECT COUNT(DISTINCT ACCU_KEY) FROM BO_BOOK_TBL WHERE REC_KEY IN ");
		query += MakeRecKeyQuery();
		tmpDM.RestructDataManager(query);
		tmpDM.GetCellData(0, 0, result);
		m_TotalSpeciesCnt = _ttoi(result.GetBuffer(0));
		m_TotalBookCnt = m_ReckeyList.GetCount();
		query = _T("SELECT SUM(getpricewon (accu_key, price)) FROM BO_BOOK_TBL WHERE REC_KEY IN ");
		query += MakeRecKeyQuery();
		tmpDM.RestructDataManager(query);
		tmpDM.GetCellData(0, 0, result);
		m_TotalPrice = (float)atof(result.GetBuffer(0));
	}
	else if(!isSpecies && type < 0)
	{
		query = _T("SELECT COUNT(DISTINCT ACCU_KEY) FROM BO_BOOK_TBL WHERE REC_KEY IN ");
		query += _T("(") + m_strSubQuery + _T(")");
		ids = pDM->GetOneValueQuery(query, result);
		m_TotalSpeciesCnt = _ttoi(result.GetBuffer(0));
		query = _T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE REC_KEY IN ");
		query += _T("(") + m_strSubQuery + _T(")");
		ids = pDM->GetOneValueQuery(query, result);
		m_TotalBookCnt = _ttoi(result.GetBuffer(0));
		query = _T("SELECT SUM(getpricewon (accu_key, price)) FROM BO_BOOK_TBL WHERE REC_KEY IN ");
		query += _T("(") + m_strSubQuery + _T(")");
		ids = pDM->GetOneValueQuery(query, result);
		m_TotalPrice = (float)atof(result.GetBuffer(0));		
	}

	if(ids < 0)
		return -1;
	return 0;
}
/*
CString CPriceSum::MakeBookQuery()
{
	if(m_ReckeyList.IsEmpty())
		return _T("");

	CString query, subquery;
	POSITION pos = m_ReckeyList.GetHeadPosition();
	CString rec_key = m_ReckeyList.GetNext(pos);
	
	query.Format(_T("%s"), rec_key);

	while(pos != NULL)
	{
		rec_key = m_ReckeyList.GetNext(pos);
		subquery.Format(_T(", %s"), rec_key);
		query += subquery;
	}

	CString tmpstr ;
	tmpstr.Format(_T("( %s )"), query);

	return tmpstr;	
}

CString CPriceSum::MakeBookReckeyQuery()
{
	if(m_ReckeyList.IsEmpty())
		return _T("");

	CString query, subquery;
	POSITION pos = m_ReckeyList.GetHeadPosition();
	CString rec_key = m_ReckeyList.GetNext(pos);
	
	query.Format(_T("%s"), rec_key);

	while(pos != NULL)
	{
		rec_key = m_ReckeyList.GetNext(pos);
		subquery.Format(_T(", %s"), rec_key);
		query += subquery;
	}

	CString tmpstr;
	tmpstr.Format(_T("( %s )"), query);

	return tmpstr;
}
*/
INT CPriceSum::GetBookCntPriceSum(CESL_DataMgr *pDM, CString &bookcnt, CString &pricesum, CString code)
{
	if(!pDM) return -1000;
	INT ids;
	INT rowCnt = pDM->GetRowCount();
	if(rowCnt == 0)
	{
		bookcnt = _T("0");
		pricesum = _T("0");
		return 0;
	}
	INT itmpbcnt, bcnt;
	FLOAT psum, ftmppsum;
	bcnt = 0; 
	psum = 0.0;
	CString tmpcode, tmpbcnt, tmppsum;
	for(INT i=0 ; i<rowCnt ; i++)
	{
		itmpbcnt = 0;
		ftmppsum = 0.0;
		ids = pDM->GetCellData(i, 0, tmpcode);
		if(ids < 0) return -1001;
		if(tmpcode == code)
		{
			ids = pDM->GetCellData(i, 1, tmpbcnt);
			if(ids < 0) return -1001;
			ids = pDM->GetCellData(i, 2, tmppsum);
			if(ids < 0) return -1001;	
			itmpbcnt = _ttoi(tmpbcnt.GetBuffer(0));
			ftmppsum = (FLOAT)atof(tmppsum.GetBuffer(0));
		}

		bcnt += itmpbcnt;
		psum += ftmppsum;
	}

	bookcnt.Format(_T("%d"), bcnt);
	pricesum.Format(_T("%.2f"), psum);
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
	
	ids = InitTotalValue(FALSE, -1);
	if(ids < 0)
		return -1001;

	ids = MakePriceDM(-1, FALSE, tabName, -1);
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
		tmpquery.Format(_T("SELECT B.REC_KEY FROM BO_PURCHASEINFO_TBL P, BO_BOOK_TBL B WHERE P.REC_KEY=B.ACCU_KEY AND B.BOOK_APPENDIX_FLAG='B'"));// AND _T(");
	else
		tmpquery.Format(_T("SELECT B.REC_KEY FROM BO_BOOK_TBL B WHERE B.BOOK_APPENDIX_FLAG='B'"));// AND _T(");

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
	if(dateType == _T("0"))
		fieldName = _T("B.INPUT_DATE");
	else if(dateType == _T("1"))
		fieldName = _T("P.INPUT_DATE");
	else if(dateType == _T("2"))
		fieldName = _T("B.CHECKIN_DATE");

	if(date1.IsEmpty() && !date2.IsEmpty())
	{
		m_pParentMgr->ESLAfxMessageBox(_T("검색 시작일을 입력해 주세요."));
		return 1000;
	}
		//strOption.Format(_T("%s <= '%s'"), fieldName, date2);
	else if(!date1.IsEmpty() && date2.IsEmpty())
		strOption.Format(_T("%s = '%s'"), fieldName, date1);
	else if(!date1.IsEmpty() && !date2.IsEmpty() && date1 == date2)
		strOption.Format(_T("%s = '%s'"), fieldName, date1);
	else if(!date1.IsEmpty() && !date2.IsEmpty() && date1 != date2)
		strOption.Format(_T("%s >= '%s' AND %s <= '%s'"), fieldName, date1, fieldName, date2);	
	return 0;
}
