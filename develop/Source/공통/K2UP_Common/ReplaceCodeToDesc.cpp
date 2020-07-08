// ReplaceCodeToDesc.cpp: implementation of the CReplaceCodeToDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReplaceCodeToDesc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReplaceCodeToDesc::CReplaceCodeToDesc()
{

}

CReplaceCodeToDesc::~CReplaceCodeToDesc()
{

}

INT CReplaceCodeToDesc::ReplaceWorkingStatusCodeToDesc(CESL_DataMgr *pDM, CESL_Grid *pGrid, INT index)
{
	if(!pDM || !pGrid) return -1000;

	INT rowCnt = pDM->GetRowCount();
	CString workingstatus, tmpstr;
	for(INT i=0 ; i<rowCnt ; i++)
	{
		pDM->GetCellData(_T("I_자료상태"), i, workingstatus);
		if(workingstatus == _T("BOA111N"))
			tmpstr = _T("구입대상자료");
		else if(workingstatus == _T("BOA113N"))
			tmpstr = _T("검수대상자료");
		else if(workingstatus == _T("BOA114O"))
			tmpstr = _T("미납품자료");
		else if(workingstatus == _T("BOA141N"))
			tmpstr = _T("검수완료자료");
		pGrid->SetTextMatrix(i+1, index, tmpstr.GetBuffer(0));
	}
	return 0;	
}	

INT CReplaceCodeToDesc::ReplaceInputTypeCodeToDesc(CESL_DataMgr *pDM, CESL_Grid *pGrid, INT index)
{
	if(!pDM || !pGrid) return -1000;

	INT rowCnt = pDM->GetRowCount();
	CString inputtype, tmpstr;
	for(INT i=0 ; i<rowCnt ; i++)
	{
		pDM->GetCellData(_T("BP_입력구분"), i, inputtype);
		if(inputtype == _T("0"))
			tmpstr = _T("화면입력");
		else if(inputtype == _T("1"))
			tmpstr = _T("재활용반입");
		else if(inputtype == _T("2"))
			tmpstr = _T("비치희망반입");
		else if(inputtype == _T("3"))
			tmpstr = _T("파일반입");
		pGrid->SetTextMatrix(i+1, index, tmpstr.GetBuffer(0));
	}
	return 0;	
}

INT CReplaceCodeToDesc::ReplaceFormCodeToDesc(CESL_Mgr * pMgr, CESL_DataMgr *pDM, CESL_Grid *pGrid, INT index)
{
	if(!pMgr || !pDM || !pGrid) return -1000;

	INT rowCnt = pDM->GetRowCount();
	CString formcode, tmpstr;
	for(INT i=0 ; i<rowCnt ; i++)
	{
		pDM->GetCellData(_T("I_형식구분"), i, formcode);
		pMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("형식구분"), formcode, tmpstr);
		pGrid->SetTextMatrix(i+1, index, tmpstr.GetBuffer(0));
	}
	return 0;	
}

INT CReplaceCodeToDesc::ReplaceUseObjCodeToDesc(CESL_Mgr *pMgr, CESL_DataMgr *pDM, CESL_Grid *pGrid, INT index)
{
	if(!pMgr || !pDM || !pGrid) return -1000;

	INT rowCnt = pDM->GetRowCount();
	CString useobjcode, tmpstr;
	for(INT i=0 ; i<rowCnt ; i++)
	{
		pDM->GetCellData(_T("I_이용대상구분"), i, useobjcode);
		pMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("이용대상구분"), useobjcode, tmpstr);
		pGrid->SetTextMatrix(i+1, index, tmpstr.GetBuffer(0));
	}
	return 0;	
}

INT CReplaceCodeToDesc::ReplaceProviderCodeToDesc(CESL_Mgr *pMgr, CESL_DataMgr *pDM, CESL_Grid *pGrid, INT index)
{
	if(!pMgr || !pDM || !pGrid) return -1000;

	INT rowCnt = pDM->GetRowCount();
	CString providercode, tmpstr;
	for(INT i=0 ; i<rowCnt ; i++)
	{
		pDM->GetCellData(_T("BP_구입처부호"), i, providercode);
		pMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("구입처부호"), providercode, tmpstr);
		pGrid->SetTextMatrix(i+1, index, tmpstr.GetBuffer(0));
	}
	return 0;	
}

INT CReplaceCodeToDesc::ReplaceCodeToDesc(CESL_Mgr *pMgr, 
										  CESL_DataMgr *pDM, 
										  INT index, 
										  CString SourceFieldAlias, 
										  CString TargetFieldAlias, 
										  CString codeClassName)
{
	if(!pMgr || !pDM) return -1000;
	
	CString code, tmpstr;
	pDM->GetCellData(SourceFieldAlias, index, code);
	pMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(codeClassName, code, tmpstr);
	pDM->SetCellData(TargetFieldAlias, tmpstr, index);
	return 0;	
}

INT CReplaceCodeToDesc::ReplaceMainGridCodeToDesc(CESL_Mgr *pMgr, CESL_DataMgr *pDM)
{
	if(!pMgr || !pDM) return -1000;
	INT ids;
	INT rowCnt = pDM->GetRowCount();
	CString tmpstr, tmpstr1, tmpstr2;
	for(INT i=0 ; i<rowCnt ; i++)
	{
/*		ids = pDM->GetCellData(_T("IBS_형식구분"), i, tmpstr);
		if(ids >= 0)
		{
			pMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("형식구분"), tmpstr, tmpstr1);
			pDM->SetCellData(_T("UDF_형식구분설명"), tmpstr1, i);
		}
		ids = pDM->GetCellData(_T("IBS_이용대상구분"), i, tmpstr);
		if(ids >= 0)
		{
			pMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("이용대상구분"), tmpstr, tmpstr1);
			pDM->SetCellData(_T("UDF_이용대상구분설명"), tmpstr1, i);
		}
		ids = pDM->GetCellData(_T("BP_구입처부호"), i, tmpstr);
		if(ids >= 0)
		{		
			pMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("구입처관리"), tmpstr, tmpstr1);
			pDM->SetCellData(_T("UDF_구입처부호설명"), tmpstr1, i);
		}
*/		ids = pDM->GetCellData(_T("BP_입력구분"), i, tmpstr);
		if(ids >= 0)
		{
			if(tmpstr == _T("0"))
				tmpstr1 = _T("화면입력");
			else if(tmpstr == _T("1"))
				tmpstr1 = _T("재활용반입");
			else if(tmpstr == _T("2"))
				tmpstr1 = _T("비치희망반입");
			else if(tmpstr == _T("3"))
				tmpstr1 = _T("파일반입");
			pDM->SetCellData(_T("UDF_입력구분설명"), tmpstr1, i);
		}
		ids = pDM->GetCellData(_T("BB_자료상태"), i, tmpstr);
		if(ids >= 0)
		{
			if(tmpstr == _T("BOA111N"))
				tmpstr1 = _T("구입대상자료");
			else if(tmpstr == _T("BOA113N"))
				tmpstr1 = _T("검수대상자료");
			else if(tmpstr == _T("BOA114O"))
				tmpstr1 = _T("미납품자료");
			else if(tmpstr == _T("BOA141N"))
				tmpstr1 = _T("검수완료자료");
			pDM->SetCellData(_T("UDF_자료상태설명"), tmpstr1, i);
		}
		pDM->GetCellData(_T("BP_주제구분"), i, tmpstr);
		if(tmpstr.IsEmpty()) pDM->SetCellData(_T("UDF_주제구분설명"), _T("적용안함"), i);
		else
		{
			pDM->GetCellData(_T("IBS_분류표구분"), i, tmpstr1);
			if(tmpstr1.IsEmpty()) return 0;
			else if(tmpstr1 == _T("1")) tmpstr1 = _T("KDC대주제부호관리");
			else if(tmpstr1 == _T("2")) tmpstr1 = _T("DDC대주제부호관리");
			ReplaceCodeToDesc(pMgr, pDM, i, _T("BP_주제구분"), _T("UDF_주제구분설명"), tmpstr1);
		}
	}
	return 0;
}

INT CReplaceCodeToDesc::EvaluateDiscountRate(
											 CESL_DataMgr * pDM, 
											 CString priceAlias, 
											 CString drateAlias, 
											 CString dpriceAlias)
{
	if(!pDM) return -1000;
	INT rowCnt = pDM->GetRowCount();
	CString tmpprice, tmpdrate, tmpstr;
	DOUBLE price, drate, dprice;
	for(INT i=0 ; i<rowCnt ; i++)
	{
		pDM->GetCellData(priceAlias, i, tmpprice);
		pDM->GetCellData(drateAlias, i, tmpdrate);
		//price = atof(tmpprice.GetBuffer(0));
		//drate = atof(tmpdrate.GetBuffer(0));
		TCHAR* pEndPtr = NULL;
		price = _tcstod(tmpprice.GetBuffer(0), &pEndPtr);
		drate = _tcstod(tmpdrate.GetBuffer(0), &pEndPtr);

		dprice = price - (price*(drate/100.0));
		tmpstr.Format(_T("%10.2f"), dprice);
		pDM->SetCellData(dpriceAlias, tmpstr, i);
	}
	return 0;
}

VOID CReplaceCodeToDesc::InitGirdColumnHeading(CESL_Mgr * pMgr, CString CMAlias, CString GridAlias)
{
	CESL_ControlMgr * pCM = pMgr->FindCM(CMAlias);
	CESL_Grid * pGrid = (CESL_Grid*)pCM->FindControl(GridAlias);
	INT colCnt = pGrid->GetColCount();
	INT width, alignment;
	CString tmpstr;
	for(INT i=0 ; i<colCnt ; i++)
	{
		pGrid->GetColumnInfo(i, tmpstr, width, alignment);
		m_GridColumnHeading.Add(tmpstr);
	}
	
	CString LINK_DATA = pGrid->ELEMENT->LINK_DATA;
	LINK_DATA.TrimLeft();				LINK_DATA.TrimRight();
	if (LINK_DATA.GetLength() == 0) return ;

	CString strColumn, strTmpDMAlias, strTmpDMFieldAlias, szTmp;
	for (i = 0; true; i++) {
		strColumn = _T("");
		strColumn = LINK_DATA.SpanExcluding(_T(","));
		strColumn.TrimLeft();		strColumn.TrimRight();
		if (strColumn.GetLength() > 0) {
			if (pGrid->ParseMgrAndColumn(strColumn, strTmpDMAlias, strTmpDMFieldAlias) >= 0) {
				m_GridColumnLinkAlias.Add(strTmpDMFieldAlias);
			}
		}
		else
			m_GridColumnLinkAlias.Add(_T(""));
		if (strColumn == LINK_DATA) break;
		szTmp = LINK_DATA.Mid(strColumn.GetLength()+1);
		if (szTmp.GetLength() == 0) break;
		LINK_DATA = szTmp;
	}
}

INT CReplaceCodeToDesc::GetColumnIndexByHeadingName(CString colName)
{
	INT size = m_GridColumnHeading.GetSize();
	for(INT i=0 ; i<size ; i++)
	{
		if(colName == m_GridColumnHeading.GetAt(i))
			return i;
	}
	return -1;
}

INT CReplaceCodeToDesc::SetGridCellData(
										CESL_Mgr * pMgr, 
										CString CMAlias, 
										CString GridAlias, 
										CString DMAlias, 
										INT index, 
										BOOL isSpecies, 
										CStringArray *GridCellList)
{
	CESL_ControlMgr * pCM = pMgr->FindCM(CMAlias);
	CESL_DataMgr * pDM = pMgr->FindDM(DMAlias);
	CESL_Grid * pGrid = (CESL_Grid*)pCM->FindControl(GridAlias);

	INT cnt = GridCellList->GetSize();
	INT col;
	CString DMFieldAlias;
	for(INT i=0 ; i<cnt ; i++)
	{
		col = GetColumnIndexByHeadingName(GridCellList->GetAt(i));
		if(col < 0) continue;
		DMFieldAlias = m_GridColumnLinkAlias.GetAt(col);
		if(col >= 0) pGrid->SetAt(index, col, pDM->GetCellData(DMFieldAlias, index));
	}
	return 0;
}

INT CReplaceCodeToDesc::ReplaceFurnishStatusToDesc(CESL_Mgr *pMgr, CESL_DataMgr *pDM)
{
	INT cnt = pDM->GetRowCount();
	if(cnt == 0 ) return 0;
	CString tmpstr;
	for(INT i=0 ; i<cnt ; i++)
	{
		pDM->GetCellData(_T("비치상태"), i, tmpstr);
		if(tmpstr == _T("1")) pDM->SetCellData(_T("UDF_비치상태설명"), _T("신청중"), i);
		else if(tmpstr == _T("2")) pDM->SetCellData(_T("UDF_비치상태설명"), _T("구입중"), i);
		else if(tmpstr == _T("3")) pDM->SetCellData(_T("UDF_비치상태설명"), _T("기증접수중"), i);
		else if(tmpstr == _T("4")) pDM->SetCellData(_T("UDF_비치상태설명"), _T("정리중"), i);
		else if(tmpstr == _T("5")) pDM->SetCellData(_T("UDF_비치상태설명"), _T("소장중"), i);
		else if(tmpstr == _T("6")) pDM->SetCellData(_T("UDF_비치상태설명"), _T("취소됨"), i);
	}
	return 0;
}
