// BO_DataManager.cpp: implementation of the CBO_DataManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_DataManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_DataManager::CBO_DataManager(CESL_Mgr * pParentMgr)
{
	m_pParentMgr = pParentMgr;
	m_pDM = NULL;
}

CBO_DataManager::~CBO_DataManager()
{
	if( m_pDM != NULL )
	{
		delete m_pDM;
		m_pDM = NULL;
	}
}

INT CBO_DataManager::InitDataManager(CString strConnectionInfo, CString strDMAlias/*=_T("")*/)
{
	if(m_pParentMgr == NULL) return -1;
	if(strConnectionInfo.IsEmpty()) strConnectionInfo = m_pParentMgr->CONNECTION_INFO;

	if(strConnectionInfo.IsEmpty()) return -1;
	if(!strDMAlias.IsEmpty())  m_strDMAlias = strDMAlias;
	if(m_strDMAlias.IsEmpty()) return -1;
	
	if(m_pDM)
	{
		delete m_pDM;
		m_pDM = NULL;
	}

	m_pDM = new CESL_DataMgr;
	m_pDM->SetCONNECTION_INFO(strConnectionInfo);
	m_pDM->InitDMPKFromFile(strDMAlias);
	m_pDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

	return 0;
}

CString CBO_DataManager::GetTodayDate(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

CString CBO_DataManager::GetTodayYear(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;
}

INT CBO_DataManager::SetDM(CString strRecKey)
{
	CString strQuery;
	if(strRecKey.IsEmpty()) return -1;
	if(m_pDM == NULL) return -1;
	strQuery.Format(_T("REC_KEY = %s"), strRecKey);
	INT ids = m_pDM->RefreshDataManager(strQuery);
	if(ids < 0) return -1;
	return 0;
}

INT CBO_DataManager::SetDM(CESL_DataMgr *pDM_SOURCE, INT nSourceRowIdx, INT nTargetRowIdx/*=0*/)
{
	if (!m_pDM || !pDM_SOURCE) return -1;
	if(m_pDM->GetRowCount() == 0) m_pDM->InsertRow(-1);
	if(nTargetRowIdx < 0) return -1;

	INT nSourceColumnCount = m_pDM->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) 
	{
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, nSourceRowIdx, strData);
		ids = m_pDM->SetCellData(field_alias, strData, nTargetRowIdx);
	}
	return 0;
}

INT CBO_DataManager::SetDM(CESL_ControlMgr * pCM_SOURCE)
{
	if (m_pDM == NULL || pCM_SOURCE == NULL) return -1;
	
	INT i = 0;
	INT ids = -1;
	
	if(m_pDM->GetRowCount() == 0) m_pDM->InsertRow(-1);
	
	CString strAlias;
	CString strData;
	CESL_Control_Element *pCE = NULL;
	INT startpos = -1;
	POSITION pos = pCM_SOURCE->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)pCM_SOURCE->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		
		strAlias = pCE->GetAlias();
		if (strAlias.IsEmpty()) continue;
		ids = pCM_SOURCE->GetControlMgrData(strAlias, strData);
		if (ids < 0) continue;		
		m_pDM->SetCellData(strAlias, strData, 0);
	}
	return 0;
}

INT CBO_DataManager::ParsingString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray, BOOL bIndexing/*=TRUE*/)
{
	if(m_pParentMgr == NULL) return -1;
	if(strSource.GetLength() == 0) return 0;
	INT pos = strSource.Find(DelimeterStr, 0);
	CString strTmpData;
	while(pos >= 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+DelimeterStr.GetLength());
		if(bIndexing) ((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		if(!bIndexing || !CheckIsHave(pTargetArray, strTmpData))
			pTargetArray->Add(strTmpData);
		pos = strSource.Find(DelimeterStr, 0);
	}
	if(bIndexing) ((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strSource);
	if(!bIndexing || !CheckIsHave(pTargetArray, strSource))
		pTargetArray->Add(strSource);
	return 0;
}

BOOL CBO_DataManager::CheckIsHave(CStringArray *codeValue, CString code)
{
	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
	return FALSE;
}

INT CBO_DataManager::ParsingParenthesis(CString &strSource, CStringArray *pArrData, BOOL bIndexing/*=TRUE*/)
{
	if(m_pParentMgr == NULL) return -1;
	pArrData->RemoveAll();
	CString strData;
	INT nStratPos = strSource.Find(_T("("));
	INT nEndPos = strSource.Find(_T(")"));
	while(nStratPos >= 0 && nEndPos > 0)
	{
		strData = strSource.Mid(nStratPos, nEndPos-nStratPos+1);
		if(!strData.IsEmpty())
		{
			strSource.Replace(_T("(")+strData+_T(")"), _T(""));
			if(bIndexing) ((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strData);
			pArrData->Add(strData);
		}
		nStratPos = strSource.Find(_T("("));
		nEndPos = strSource.Find(_T(")"));
	}
	strSource.Replace(_T("("), _T(""));
	strSource.Replace(_T(")"), _T(""));
	return 0;
}

INT CBO_DataManager::GetCellData(CString strAlias, CString &strData)
{
	if(m_pDM == NULL) return -1;
	if(strAlias.IsEmpty()) return -1;
	if(m_pDM->GetRowCount() == 0) return -1;

	m_pDM->GetCellData(strAlias, 0, strData);
	return 0;
}

INT CBO_DataManager::SetCellData(CString strAlias, CString strData)
{
	if(m_pDM == NULL) return -1;
	if(strAlias.IsEmpty()) return -1;
	if(m_pDM->GetRowCount() == 0) return -1;

	return m_pDM->SetCellData(strAlias, strData, 0);
}

INT CBO_DataManager::GetCellData(CStringArray * pArrAlias, CStringArray * pArrData)
{
	if(pArrAlias == NULL || pArrData == NULL) return -1;

	INT ids;
	INT nCnt = pArrAlias->GetSize();
	if(nCnt == 0) return 0;
	
	CString strTmpData;
	for(INT i=0 ; i<nCnt ; i++)
	{
		ids = GetCellData(pArrAlias->GetAt(i), strTmpData);
		if(ids < 0) return -1;
		pArrData->Add(strTmpData);
	}

	return 0;
}

INT CBO_DataManager::SetCellData(CStringArray * pArrAlias, CStringArray * pArrData)
{
	if(pArrAlias == NULL || pArrData == NULL) return -1;
	
	INT ids;
	INT nCnt = pArrAlias->GetSize();
	if(nCnt == 0) return 0;
	
	CString strTmpData;
	for(INT i=0 ; i<nCnt ; i++)
	{
		ids = SetCellData(pArrAlias->GetAt(i), pArrData->GetAt(i));
		if(ids < 0) return -1;
	}
	
	return 0;
}

INT CBO_DataManager::InsertData()
{
	if(m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return -1;

	//RecKey 필드가 설정되었는지 검사한다.
	INT ids = CheckRecKeyInfo();
	if(ids < 0) return -1;
	
	CArray<CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	
	m_pDM->StartFrame();
	ids = m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, m_pDM);
	if(ids < 0) return -1;
	ids = m_pDM->SendFrame();
	if(ids < 0) return -1;
	m_pDM->EndFrame();
	return 0;
}

INT CBO_DataManager::InsertData(CESL_DataMgr * pDM_OUT, CArray<CString, CString> &RemoveAliasList, CString strRecKeyAlias/*=_T("")*/)
{
	if(pDM_OUT == NULL) return -1;
	if(m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return -1;

	//RecKey 필드가 설정되었는지 검사한다.
	INT ids = CheckRecKeyInfo(strRecKeyAlias);
	if(ids < 0) return -1;

	ids = m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, pDM_OUT);
	if(ids < 0) return -1;
	
	return 0;
}

INT CBO_DataManager::UpdateData(CESL_DataMgr * pDM_OUT, CArray<CString, CString> &RemoveAliasList, CString strRecKeyAlias/*=_T("")*/)
{
	if(pDM_OUT == NULL) return -1;
	if(m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return -1;

	if(!strRecKeyAlias.IsEmpty()) m_strRecKeyAlias = strRecKeyAlias;
	if(m_strRecKeyAlias.IsEmpty()) return -1;

	//RecKey 필드가 설정되었는지 검사한다.
	INT ids = CheckRecKeyInfo(m_strRecKeyAlias);
	if(ids < 0) return -1;

	ids = m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, pDM_OUT, _T("REC_KEY"), _T("NUMERIC"), m_pDM->GetCellData(m_strRecKeyAlias, 0));
	if(ids < 0) return -1;
	return 0;
}

INT CBO_DataManager::UpdateData()
{
	if(m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return -1;
	if(m_strRecKeyAlias.IsEmpty()) return -1;
	//RecKey 필드가 설정되었는지 검사한다.
	INT ids = CheckRecKeyInfo(m_strRecKeyAlias);
	if(ids < 0) return -1;
	
	CArray<CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	
	m_pDM->StartFrame();
	ids = m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, m_pDM, _T("REC_KEY"), _T("NUMERIC"), m_pDM->GetCellData(m_strRecKeyAlias, 0));
	if(ids < 0) return -1;
	ids = m_pDM->SendFrame();
	if(ids < 0) return -1;
	m_pDM->EndFrame();
	return 0;
}

INT CBO_DataManager::DeleteData()
{
	if(m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return -1;
	if(m_strRecKeyAlias.IsEmpty()) return -1;
	//RecKey 필드가 설정되었는지 검사한다.
	INT ids = CheckRecKeyInfo(m_strRecKeyAlias);
	if(ids < 0) return -1;

	m_pDM->StartFrame();
	ids = m_pDM->MakeDeleteFrame(m_pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), m_pDM->GetCellData(m_strRecKeyAlias, 0));
	if(ids < 0) return -1;
	ids = m_pDM->SendFrame();
	if(ids < 0) return -1;
	m_pDM->EndFrame();

	return 0;
}

INT CBO_DataManager::DeleteData(CESL_DataMgr * pDM_OUT, CString strRecKeyAlias/*=_T("")*/)
{
	if(pDM_OUT == NULL) return -1;
	if(m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return -1;
	if(m_strRecKeyAlias.IsEmpty()) return -1;
	//RecKey 필드가 설정되었는지 검사한다.
	INT ids = CheckRecKeyInfo(m_strRecKeyAlias);
	if(ids < 0) return -1;
	
	ids = pDM_OUT->MakeDeleteFrame(m_pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), m_pDM->GetCellData(m_strRecKeyAlias, 0));
	if(ids < 0) return -1;
		
	return 0;
}

INT CBO_DataManager::DeleteDataByRecKey(CString strRecKey)
{
	if(m_pDM == NULL) return -1;
	if(strRecKey.IsEmpty()) return -1;
	
	m_pDM->StartFrame();
	INT ids = m_pDM->MakeDeleteFrame(m_pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey);
	if(ids < 0) return -1;
	ids = m_pDM->SendFrame();
	if(ids < 0) return -1;
	m_pDM->EndFrame();
	return 0;
}

INT CBO_DataManager::DeleteDataByRecKey(CESL_DataMgr * pDM_OUT, CString strRecKey)
{
	if(pDM_OUT == NULL) return -1;
	if(m_pDM == NULL) return -1;
	if(strRecKey.IsEmpty()) return -1;
	
	INT ids = pDM_OUT->MakeDeleteFrame(m_pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey);
	if(ids < 0) return -1;
	return 0;
}

INT CBO_DataManager::DeleteDataByRecKeyArray(CStringArray * pArrReckey)
{
	if(m_pDM == NULL) return -1;
	if(pArrReckey == NULL) return -1;
	INT nCnt = pArrReckey->GetSize();
	if(nCnt == 0) return 0;
	
	INT ids;
	CString strRecKey;
	m_pDM->StartFrame();
	for(INT i=0 ; i<nCnt ; i++)
	{
		strRecKey = pArrReckey->GetAt(i);
		ids = m_pDM->MakeDeleteFrame(m_pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey);
		if(ids < 0) return -1;
	}
	ids = m_pDM->SendFrame();
	if(ids < 0) return -1;
	m_pDM->EndFrame();
	return 0;
}

INT CBO_DataManager::DeleteDataByRecKeyArray(CESL_DataMgr * pDM_OUT, CStringArray * pArrReckey)
{
	if(pDM_OUT == NULL) return -1;
	if(m_pDM == NULL) return -1;
	if(pArrReckey == NULL) return -1;
	INT nCnt = pArrReckey->GetSize();
	if(nCnt == 0) return 0;
	
	INT ids;
	CString strRecKey;
	for(INT i=0 ; i<nCnt ; i++)
	{
		strRecKey = pArrReckey->GetAt(i);
		ids = pDM_OUT->MakeDeleteFrame(m_pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey);
		if(ids < 0) return -1;
	}
	return 0;
}

INT CBO_DataManager::CheckRecKeyInfo(CString strRecKeyAlias/*=_T("")*/)
{
	if( strRecKeyAlias.IsEmpty() == FALSE ) 	m_strRecKeyAlias = strRecKeyAlias;
	if( m_strRecKeyAlias.IsEmpty() == TRUE )	return 0;
	if( m_pDM->GetCellData(m_strRecKeyAlias, 0).IsEmpty() == TRUE )		return -1;

	return 0;
}

INT CBO_DataManager::FreeDM()
{
	INT ids;

	if( m_pDM == NULL )		return -1;

	ids = m_pDM->FreeData();
	if( ids < 0 )			return -1;

	return 0;
}

INT CBO_DataManager::CopyDataManager(CBO_DataManager * pData)
{
	if(pData == NULL) return -1;
	if(pData->GetDM() == NULL) return -1;
	if(pData->GetDM()->GetRowCount() == 0) return -1;

	INT ids = SetDM(pData->GetDM());
	if(ids < 0) return -1;
	return 0;
}

INT CBO_DataManager::SetDM(CESL_DataMgr * pDM)
{
	if(pDM == NULL) return -1;

	INT ids = InitDataManager(pDM->CONNECTION_INFO, pDM->DB_MGR_ALIAS);
	if(ids < 0) return -1;
	if(pDM->GetRowCount() > 0)
	{
		ids = SetDM(pDM, 0);
		if(ids < 0) return -1;
	}
	return 0;
}

INT CBO_DataManager::SortDM(CString strAlias, BOOL bDesc)
{
	if(m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return 0;

	INT nSortDirection;
	if(bDesc) nSortDirection = ESL_DATAMGR_SORT_DESC;
	else nSortDirection = ESL_DATAMGR_SORT_ASC;
	INT ids = m_pDM->SORT(strAlias, nSortDirection);
	if(ids < 0) return -1;
	return 0;
}

INT CBO_DataManager::SortDM(CString strAlias, BOOL bDesc, INT nStartIdx, INT nEndIdx)
{
	if(m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return 0;
	
	INT nSortDirection;
	if(bDesc) nSortDirection = ESL_DATAMGR_SORT_DESC;
	else nSortDirection = ESL_DATAMGR_SORT_ASC;
	INT ids = m_pDM->SORT(strAlias, nStartIdx, nEndIdx, nSortDirection);
	if(ids < 0) return -1;
	return 0;
}

INT CBO_DataManager::SortDM(INT nCondColCnt, CString * strAlias, INT * nCond, BOOL bDesc)
{
	if(m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return 0;
	
	INT nSortDirection;
	if(bDesc) nSortDirection = ESL_DATAMGR_SORT_DESC;
	else nSortDirection = ESL_DATAMGR_SORT_ASC;
	INT ids = m_pDM->SORT(nCondColCnt, strAlias, nCond, nSortDirection);
	if(ids < 0) return -1;
	return 0;
}

INT CBO_DataManager::SortDM(INT nCondColCnt, CString * strAlias, INT * nCond, BOOL bDesc, INT nStartIdx, INT nEndIdx)
{
	if(m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return 0;
	
	INT nSortDirection;
	if(bDesc) nSortDirection = ESL_DATAMGR_SORT_DESC;
	else nSortDirection = ESL_DATAMGR_SORT_ASC;
	INT ids = m_pDM->SORT(nCondColCnt, strAlias, nCond, nSortDirection, nStartIdx, nEndIdx);
	if(ids < 0) return -1;
	return 0;
}
