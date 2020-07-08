// AppendixDataMgr.cpp: implementation of the CAppendixDataMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppendixDataMgr.h"
#include "../INCLUDE/BO_ACQ.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("단행종API"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAppendixDataMgr::CAppendixDataMgr(CESL_Mgr* pParent /*=NULL*/)
{
	m_pParentMgr = pParent;
	m_pAppendixDM = NULL;
	m_bDMCreate = FALSE;
}

CAppendixDataMgr::~CAppendixDataMgr()
{
	if (m_pAppendixDM && m_bDMCreate) 
	{
		delete m_pAppendixDM;
		m_pAppendixDM = NULL;
	}
}

INT CAppendixDataMgr::SetDataMgr(CESL_DataMgr *pDM)
{
	if(m_pParentMgr == NULL) return -1;
	if(pDM == NULL)
	{
		m_pAppendixDM = new CESL_DataMgr;
		if(m_pAppendixDM == NULL) return -1;
		m_pAppendixDM->SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
		m_pAppendixDM->InitDMPKFromFile(_T("DMUP_단행_종_색인정보"));
		m_pAppendixDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
		m_bDMCreate = TRUE;
	}
	else
	{
		m_pAppendixDM = pDM;
	}
	return 0;
}

INT CAppendixDataMgr::MakeBookQueryFrame(CString strSpecies_key, CString strPurchaseinfo_key, CESL_DataMgr *pDM_OUT, INT nSendCount/*=-1*/)
{
	INT ids, i;
	CString strQuery, strDBFLAG, strBook_key;
	INT nRowCount = m_pAppendixDM->GetRowCount();

	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();

	// 이제 I/U/D에 따라서 Insert/Update/Delete를 구성한다.
	CString IsREPRESENT_BOOK_YN;
	for (i = 0; i < nRowCount; i++) {
		strDBFLAG = _T("");
		ids = m_pAppendixDM->GetCellData(_T("BB_DB_FLAG"), i, strDBFLAG);
		if (strDBFLAG == _T("I")) {
			pDM_OUT->MakeRecKey(strBook_key); 
			m_pAppendixDM->SetCellData(_T("BB_책KEY"), strBook_key, i);
		}
		IsREPRESENT_BOOK_YN = _T("");
		ids = m_pAppendixDM->GetCellData(_T("BB_대표책여부"), i, IsREPRESENT_BOOK_YN);
		IsREPRESENT_BOOK_YN.TrimLeft();		IsREPRESENT_BOOK_YN.TrimRight();
		IsREPRESENT_BOOK_YN.MakeUpper();
		if (IsREPRESENT_BOOK_YN != _T("Y") && IsREPRESENT_BOOK_YN != _T("N")) m_pAppendixDM->SetCellData(_T("BB_대표책여부"), _T("N"), i);
	}

	//편권차색인을 만들기 위해서 그리고 ISBN입력시 특수기호를 제거하기 위하여
	CString volIndex, strEAISBN, strBookAppendixFlag;
	//여기까지
	for (i = 0; i < nRowCount; i++) {
		RemoveAliasList.RemoveAll();
		strDBFLAG = _T("");
		ids = m_pAppendixDM->GetCellData(_T("BB_DB_FLAG"), i, strDBFLAG);
		if (strDBFLAG == _T("I")) {
			RemoveAliasList.Add(_T("BB_원본등록번호"));
			m_pAppendixDM->SetCellData(_T("BB_수서정보KEY"), strPurchaseinfo_key, i);
			m_pAppendixDM->SetCellData(_T("BB_종KEY"), strSpecies_key, i);
			m_pAppendixDM->SetCellData(_T("BB_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
			m_pAppendixDM->SetCellData(_T("BB_최초작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
			if (m_pAppendixDM->GetCellData(_T("BB_가격"), i).GetLength() == 0) m_pAppendixDM->SetCellData(_T("BB_가격"), _T("0"), i);
			//편권차색인을 만듬 과 낱권ISBN의 특수기호 제거함
			strEAISBN = m_pAppendixDM->GetCellData(_T("BB_낱권ISBN"), i); 
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strEAISBN);
			m_pAppendixDM->SetCellData(_T("BB_낱권ISBN"), strEAISBN, i);
			volIndex = m_pAppendixDM->GetCellData(_T("BB_편제명부록명"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(volIndex);
				m_pAppendixDM->SetCellData(_T("BB_편제명부록명색인"), volIndex, i);
			}
			volIndex = m_pAppendixDM->GetCellData(_T("BB_편권차"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->ConvertHanjaToHangul(volIndex);
				m_pAppendixDM->SetCellData(_T("BB_편권차색인"), volIndex, i);
			}
			strBook_key = m_pAppendixDM->GetCellData(_T("BB_책KEY"), i);
			pDM_OUT->MakeQueryByDM(RemoveAliasList, m_pAppendixDM, _T("BO_BOOK_TBL"), i, pDM_OUT);
		}
		if (strDBFLAG == _T("U")) {
			RemoveAliasList.Add(_T("BB_책KEY"));
			RemoveAliasList.Add(_T("BB_원본등록번호"));
			ids = m_pAppendixDM->GetCellData(_T("BB_책KEY"), i, strBook_key);
			m_pAppendixDM->SetCellData(_T("BB_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
			//편권차색인을 만듬 과 낱권ISBN의 특수기호 제거함
			strEAISBN = m_pAppendixDM->GetCellData(_T("BB_낱권ISBN"), i); 
			((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strEAISBN);
			m_pAppendixDM->SetCellData(_T("BB_낱권ISBN"), strEAISBN, i);			
			volIndex = m_pAppendixDM->GetCellData(_T("BB_편제명부록명"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				m_pAppendixDM->SetCellData(_T("BB_편제명부록명색인"), volIndex, i);
			}
			volIndex = m_pAppendixDM->GetCellData(_T("BB_편권차"), i);
			if(volIndex.GetLength() != 0)
			{
				((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(volIndex);
				m_pAppendixDM->SetCellData(_T("BB_편권차색인"), volIndex, i);
			}
			pDM_OUT->MakeQueryByDM(RemoveAliasList, m_pAppendixDM, _T("BO_BOOK_TBL"), i, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), strBook_key);
			strBook_key = m_pAppendixDM->GetCellData(_T("BB_책KEY"), i);
		}
		if (strDBFLAG == _T("D")) {
			ids = m_pAppendixDM->GetCellData(_T("BB_책KEY"), i, strBook_key);
			if(strBook_key.IsEmpty()) strBook_key = _T("-1");
			strQuery.Format(_T("DELETE FROM BO_BOOK_TBL WHERE REC_KEY=%s;"), strBook_key);
			pDM_OUT->AddFrame(strQuery);
			//책의 삭제쿼리를 생성할때 등록번호가 부여된 자료에 대하여 부여되어진 등록번호를 누락등록번호에 추가한다.
			CString strRecNo = m_pAppendixDM->GetCellData(_T("BB_등록번호"), i);
			if(!strRecNo.IsEmpty()) InsertToDropAccessionNoTbl(strRecNo, pDM_OUT, TRUE);
		}
	}

	return 0;
}

//부록에 Default값을 적용시킨다.
INT CAppendixDataMgr::SetDefaultValue(CESL_DataMgr * pDM_ACCU, INT nRowIdx, INT nSpeciesType) 
{
	if(!m_pAppendixDM) return -1;
	/*
	책 정보 
	- 수입구분 : BB_수입구분_코드 : 수입구분(1-구입,2-기증) 기본으로 1임
	- 입력일 : BB_입력일 : 입력일
	*/
	SPECIES_TYPE sType = (SPECIES_TYPE)nSpeciesType;

	CString INPUT_DATE, strTmpData;
	INPUT_DATE = GetTodayDate();

	INT nLastBookSerialNo = -1;
	CString strLastSerialNo;

	INT nRow = m_pAppendixDM->GetRowCount();
	for (INT i = 0; i < nRow; i++) 
	{
		if(m_pAppendixDM->GetCellData(_T("BB_DB_FLAG"), i) == _T("I") )
		{
			m_pAppendixDM->SetCellData(_T("BB_레이블출력구분"), _T("1"), i);
			m_pAppendixDM->SetCellData(_T("BB_책부록플래그"), _T("B"), i);
			m_pAppendixDM->SetCellData(_T("BB_입력일"), INPUT_DATE, i);
			strTmpData = m_pAppendixDM->GetCellData(_T("BB_자료상태"), i);
			if(strTmpData != _T("BOA113O")) m_pAppendixDM->SetCellData(_T("BB_납품여부"), _T("Y"), i);
			else m_pAppendixDM->SetCellData(_T("BB_납품여부"), _T("N"), i);
			if(sType == BO_REG_PURCHASE || sType == BO_REG_DONATE || sType == BO_DONATE || sType == BO_REG_INVALID || sType == BO_REGOBJ)
				m_pAppendixDM->SetCellData(_T("BB_검수일"), GetTodayDate(), i);
			if(sType == BO_REG_PURCHASE || sType == BO_REG_DONATE || sType == BO_REG_INVALID || sType == BO_REGOBJ)
				m_pAppendixDM->SetCellData(_T("BB_등록인계일자"), GetTodayDate(), i);
		}
	}
	return 0;
}

CString CAppendixDataMgr::GetTodayDate(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

INT CAppendixDataMgr::InsertToDropAccessionNoTbl(CString strRegNo, CESL_DataMgr * pDM_Out, BOOL IsCheckInsert)
{
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
// 	CString strCode = strRegNo.Left(2);
// 	INT nNumber = _ttoi((strRegNo.Right(10)).GetBuffer(0));
	CString strCode = strRegNo.Left(nRegCodeLength);
	INT nNumber = _ttoi((strRegNo.Right(12-nRegCodeLength)).GetBuffer(0));
	//===================================================================
	CString query, strTmpData;
	if(IsCheckInsert)	
	{
		query.Format(_T("SELECT REC_KEY FROM CO_DROP_ACCESSION_NO_TBL WHERE REG_CODE = '%s' AND REG_NO = %d MANAGE_CODE = UDF_MANAGE_CODE"), strCode, nNumber);
//DEL 		query.Format(_T("SELECT REC_KEY FROM CO_DROP_ACCESSION_NO_TBL WHERE REG_CODE = '%s' AND REG_NO = %d"), strCode, nNumber);
		pDM_Out->GetOneValueQuery(query, strTmpData);
		if(strTmpData.GetLength() != 0) return 0;
	}
	CString strRecKey;
	pDM_Out->MakeRecKey(strRecKey);
	query.Format(
		_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL (REC_KEY, PUBLISH_FORM_CODE, REG_CODE, REG_NO, MANAGE_CODE) VALUES(%s, 'MO', '%s', %d, UDF_MANAGE_CODE);"),
//DEL 		_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL (REC_KEY, PUBLISH_FORM_CODE, REG_CODE, REG_NO) VALUES(%s, 'MO', '%s', %d);"),
			strRecKey,
			strCode,
			nNumber);
	pDM_Out->AddFrame(query);
	return 0;
}
