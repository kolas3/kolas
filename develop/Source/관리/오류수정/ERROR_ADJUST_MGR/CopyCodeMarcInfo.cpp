// CopyCodeMarcInfo.cpp: implementation of the CCopyCodeMarcInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CopyCodeMarcInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCopyCodeMarcInfo::CCopyCodeMarcInfo(CESL_Mgr * pParentMgr)
	:CMarcInfo(pParentMgr)
{
	m_pDM_BOOK = NULL;
	m_bRegCodeYN = FALSE;
	m_bVolYN = FALSE;
	m_bSeShelfCodeYN = FALSE;
}

CCopyCodeMarcInfo::~CCopyCodeMarcInfo()
{
	if(m_pDM_BOOK)
	{
		delete m_pDM_BOOK;
		m_pDM_BOOK = NULL;
	}
}

INT CCopyCodeMarcInfo::Update(CESL_DataMgr *pDM, CMarc *pMarc)
{	
	INT ids;
	ids = SetBookDM(pDM->CONNECTION_INFO);
	if(ids < 0) return ids;
	else if(ids == 1000) return 0;

	ids = Init(pDM->CONNECTION_INFO);
	if(ids < 0) return ids;
	INT nLastCopyCode, nCurRow;
	nLastCopyCode = nCurRow = -1;
	CString strTmpData, strQuery;
	for(INT i=0 ; i<m_pDM_BOOK->GetRowCount() ; i++)
	{
		nCurRow = i;
		m_strRegCode = m_pDM_BOOK->GetCellData(_T("BB_등록구분_코드"), i);
		m_strSeShelfCode = m_pDM_BOOK->GetCellData(_T("BB_별치기호_코드"), i);
		m_strVol = m_pDM_BOOK->GetCellData(_T("BB_편권차"), i);

		nLastCopyCode = GetLastDupCodeNumber(nCurRow, nCurRow+1);
		if(nLastCopyCode == 0) strTmpData = _T("");
		else strTmpData.Format(_T("%d"), ++nLastCopyCode);
		m_pDM_BOOK->SetCellData(_T("BB_복본기호"), strTmpData, i);
		strQuery.Format(_T("UPDATE BO_BOOK_TBL SET COPY_CODE='%s' WHERE REC_KEY=%s;"),
			strTmpData, m_pDM_BOOK->GetCellData(_T("BB_책KEY"), i));
		pDM->AddFrame(strQuery);
	}

	ids = Restruct049MarcTag(pMarc);
	if(ids < 0) return ids;

	return 0;
}

INT CCopyCodeMarcInfo::SetBookDM(CString strConnectionInfo)
{
	if(m_strSpeciesKey.IsEmpty()) return -1;

	m_pDM_BOOK = new CESL_DataMgr;
	if(m_pDM_BOOK == NULL) return -1;
	m_pDM_BOOK->SetCONNECTION_INFO(strConnectionInfo);
	m_pDM_BOOK->InitDMPKFromFile(_T("DMUP_단행_종_책정보"));
	m_pDM_BOOK->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	CString strQuery;
	strQuery.Format(_T("SPECIES_KEY=%s"), m_strSpeciesKey);

	INT ids = m_pDM_BOOK->RefreshDataManager(strQuery);
	if(ids < 0) return ids;
	
	if(m_pDM_BOOK->GetRowCount() == 0) return 1000;

	INT nCondColumns = 0;
	CString strAlias[32];
	INT nCond[32];

	strAlias[nCondColumns] = _T("BB_권일련번호");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	strAlias[nCondColumns] = _T("BB_복본기호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	strAlias[nCondColumns] = _T("BB_등록번호");			nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	ids = m_pDM_BOOK->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	if(ids < 0) return ids;
	return 0;
}

INT CCopyCodeMarcInfo::Init(CString strConnectionInfo)
{
	if(strConnectionInfo.IsEmpty()) return -1;
	CString strQuery, strTmpData;
	//++2008.10.22 UPDATE BY PWR {{++
	// 통합시스템 적용
	strQuery.Format( _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
					 _T("WHERE CLASS_ALIAS='복본기호부여방식' AND MANAGE_CODE=UDF_MANAGE_CODE") );
//	strQuery.Format(
//		_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='복본기호부여방식'"));
	//--2008.10.22 UPDATE BY PWR --}}
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(strConnectionInfo);
	tmpDM.RestructDataManager(strQuery);
	INT nRowCnt = tmpDM.GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = tmpDM.GetCellData(i, 0);
		if(strTmpData == _T("등록구분별")) m_bRegCodeYN = TRUE;
		else if(strTmpData == _T("권차별")) m_bVolYN = TRUE;
		else if(strTmpData == _T("별치기호별")) m_bSeShelfCodeYN = TRUE;
	}
	return 0;
}

BOOL CCopyCodeMarcInfo::IsCopy(CESL_DataMgr *pDM_BOOK, INT idx)
{
	if(pDM_BOOK == NULL) return FALSE;
	CString strAlias[3] = {
		_T("BB_편권차"), _T("BB_등록구분_코드"), _T("BB_별치기호_코드")
	};
	CString strData[3];
	pDM_BOOK->GetCellData(strAlias, 3, strData, idx);
	if(m_bVolYN && m_strVol != strData[0]) return FALSE;
	if(m_bRegCodeYN && m_strRegCode != strData[1]) return FALSE;
	if(m_bSeShelfCodeYN && m_strSeShelfCode != strData[2]) return FALSE;
	return TRUE;
}

INT CCopyCodeMarcInfo::GetLastDupCodeNumber(INT nCurIdx, INT nStartIdx, INT nEndIdx, CString strRegNo)
{
	if (m_pDM_BOOK == NULL) return -1;
	
	if(strRegNo.IsEmpty()) return 0;
	
	INT nDupNumber = 0;
	
	for (INT idx = nStartIdx; idx <= nEndIdx; idx++)
	{
		CString strTemp;
		INT nNumber = 0;
		
		// 복본인지 검사한다.
		if (!IsCopy(m_pDM_BOOK, idx)) continue;
		//자기 자신이면 검사하지 않는다.
		if(idx == nCurIdx) continue;
		
		// 복본기호를 가져온다.
		if(strRegNo != _T("-1"))
		{
			strTemp = m_pDM_BOOK->GetCellData(_T("BB_등록번호"), idx);
			if(strTemp.IsEmpty()) continue;
		}
		
		strTemp = m_pDM_BOOK->GetCellData(_T("BB_복본기호"), idx);
		if (strTemp.IsEmpty()) strTemp = _T("1");
		
		nNumber = _ttoi(strTemp.GetBuffer(0));	
		if (nNumber > nDupNumber)
			nDupNumber = nNumber;
	}
	
	return nDupNumber;
}

INT CCopyCodeMarcInfo::GetLastDupCodeNumber(INT nCurIdx, INT nEndIdx/*=-1*/, CString strRegNo/*=_T("-1")*/)
{
	if (m_pDM_BOOK == NULL) return -1;
	
	INT nRowCount;
	if(nEndIdx > -1) 
		nRowCount = nEndIdx;
	else
		nRowCount = m_pDM_BOOK->GetRowCount();
	
	return GetLastDupCodeNumber(nCurIdx, 0, nRowCount-1, strRegNo);
}

BOOL CCopyCodeMarcInfo::IsSameSeShelfCode()
{
	if(m_pDM_BOOK == NULL) return FALSE;
	INT nRowCnt = m_pDM_BOOK->GetRowCount();
	if(nRowCnt < 2) return TRUE;
	
	CString strSeShelfCode, strTmpData;
	strSeShelfCode = m_pDM_BOOK->GetCellData(_T("BB_별치기호_코드"), 0);
	strSeShelfCode.TrimLeft();	strSeShelfCode.TrimRight();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = m_pDM_BOOK->GetCellData(_T("BB_별치기호_코드"), i);
		strTmpData.TrimLeft();	strTmpData.TrimRight();
		if(strSeShelfCode != strTmpData) return FALSE;
	}
	return TRUE;
}

INT CCopyCodeMarcInfo::Restruct049MarcTag(CMarc *pMarc)
{
	BOOL bSameSeShelfCode = FALSE;
	bSameSeShelfCode = IsSameSeShelfCode();
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(pMarc, _T("049"));
	m_pDM_BOOK->SORT(_T("BB_등록번호"));
	CString strRegNo, strVol, strSeShelfCode, strCopyCode, str049;
	INT nRowCnt = m_pDM_BOOK->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strCopyCode = m_pDM_BOOK->GetCellData(_T("BB_복본기호"), i);
		strRegNo = m_pDM_BOOK->GetCellData(_T("BB_등록번호"), i);
		strVol = m_pDM_BOOK->GetCellData(_T("BB_편권차"), i);
		strSeShelfCode = m_pDM_BOOK->GetCellData(_T("BB_별치기호_코드"), i);
		if(strRegNo.IsEmpty()) continue;
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$l"), strRegNo);
		if(!strVol.IsEmpty())
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$v"), strVol);
		if(!bSameSeShelfCode && !strSeShelfCode.IsEmpty())
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$f"), strSeShelfCode);
		if(!strCopyCode.IsEmpty())
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$c"), strCopyCode);
	}
	
	if(bSameSeShelfCode)
	{
		m_pParentMgr->m_pInfo->pMarcMgr->GetField( pMarc, _T("049"), str049 );
		if (!str049.IsEmpty())
		{
			strSeShelfCode = m_pDM_BOOK->GetCellData(_T("BB_별치기호_코드"), 0);
			if(!strSeShelfCode.IsEmpty())
				m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("049$f"), strSeShelfCode);
			str049 = _T("0") + str049.Mid(1);
			m_pParentMgr->m_pInfo->pMarcMgr->DeleteField( pMarc, _T("049") );
			m_pParentMgr->m_pInfo->pMarcMgr->InsertField( pMarc, _T("049") + str049);
		}		
	}
	else
	{
		m_pParentMgr->m_pInfo->pMarcMgr->GetField( pMarc, _T("049"), str049 );
		if (!str049.IsEmpty()) 
		{
			str049 = _T("1") + str049.Mid(1);
			m_pParentMgr->m_pInfo->pMarcMgr->DeleteField( pMarc, _T("049") );
			m_pParentMgr->m_pInfo->pMarcMgr->InsertField( pMarc, _T("049") + str049);
		}
	}
	
	return 0;	
}