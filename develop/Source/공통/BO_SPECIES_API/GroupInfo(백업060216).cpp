// GroupInfo.cpp: implementation of the CGroupInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GroupInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("그룹정보"), _T(__FILE__), __LINE__
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//생성자
CGroupInfo::CGroupInfo()
{
	m_strGroupInfo = _T("");
	m_strAcqYear = _T("");
	m_strGroupNo = _T("");
	m_strLastWorkNo = _T("");
	m_arrWorkNo.RemoveAll();
	m_nLastSerialNo = -1;
	m_nIdx = -1;
	m_bHaveWorkNo = FALSE;
	m_bHaveLastSerialNo = FALSE;
	m_nBookLastSerialNo = -1;
	m_nOrderLastSerialNo = - 1;
	m_strRecKey = _T("");
	m_bWorkNoUseYN = FALSE;
}

CGroupInfo::~CGroupInfo()
{

}

CString CGroupInfo::GetWorkNo(INT idx)
{
	INT cnt = m_arrWorkNo.GetSize();
	
	if(cnt == 0) return _T("");
	if(idx == -1) 
	{
		if(m_nIdx < 0) m_nIdx = 0;
		return m_arrWorkNo.GetAt(m_nIdx);
	}
	return m_arrWorkNo.GetAt(idx);
}

INT CGroupInfo::SetCurSelWorkNo(CString strWorkNo)
{
	INT cnt = m_arrWorkNo.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strWorkNo == m_arrWorkNo.GetAt(i))
			m_nIdx = i;
	}

	return 0;
}


INT CGroupInfo::UpDateLastSerialNo(CESL_Mgr * pMgr, CESL_DataMgr *pDM_OUT)
{
	CString strWorkNo;
	if(m_nIdx >= 0) strWorkNo = m_arrWorkNo.GetAt(m_nIdx);
	if(strWorkNo == _T("사용안함")) strWorkNo = _T("0");
	CString strQuery;
	strQuery.Format(
		_T("UPDATE BO_PURCHASE_WORK_NO_TBL SET LAST_SERIAL_NO = %d, LAST_WORK = '%s' WHERE WORK_NO = %s AND PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s);"),
		m_nLastSerialNo, 
		pMgr->GetWorkLogMsg(WORK_LOG),
		strWorkNo,
		m_strAcqYear,
		m_strGroupInfo
	);
	pDM_OUT->AddFrame(strQuery);
	return 0;
}

INT CGroupInfo::CopyData(CGroupInfo *pGroup)
{
	pGroup->SetLastSerialNo(m_nLastSerialNo);
	pGroup->SetAcqYear(m_strAcqYear);
	pGroup->SetGroupInfo(m_strGroupInfo);
	pGroup->SetArrayWorkNo(&m_arrWorkNo);
	pGroup->SetHaveLastSerialNo(m_bHaveLastSerialNo);
	pGroup->SetHaveWorkNo(m_bHaveWorkNo);
	pGroup->SetGroupNo(m_strGroupNo);
	pGroup->SetLastWorkNo(m_strLastWorkNo);
	pGroup->SetIndex(m_nIdx);
	pGroup->SetWorkCode(m_strWorkCode);
	pGroup->SetWorkNoUseYN(m_bWorkNoUseYN);

	return 0;
}

VOID CGroupInfo::SetArrayWorkNo(CStringArray *pArray)
{
	m_arrWorkNo.RemoveAll();
	INT cnt = pArray->GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		m_arrWorkNo.Add(pArray->GetAt(i));
	}
}

BOOL CGroupInfo::IsEqual(CGroupInfo *pGroupInfo)
{
	CString strAcqYear, strGroupNo, strGroupInfo, strWorkCode;

	strAcqYear = pGroupInfo->GetAcqYear();
	strGroupInfo = pGroupInfo->GetGroupInfo();
	strGroupNo = pGroupInfo->GetGroupNo();
	strWorkCode = pGroupInfo->GetWorkCode();

	if(strAcqYear == m_strAcqYear && strGroupInfo == m_strGroupInfo && strGroupNo == m_strGroupNo)
	{
		if(m_strWorkCode.IsEmpty()) return TRUE;
		else
		{
			if(strWorkCode == m_strWorkCode) return TRUE;
		}
	}
	return FALSE;
}

INT CGroupInfo::UpDateBookLastSerialNo(CString strAcqCode, CESL_Mgr * pMgr, CESL_DataMgr *pDM_OUT)
{
	CString strQuery;
	if(strAcqCode.IsEmpty() || strAcqCode == _T("0"))
	{
		strQuery.Format(
			_T("UPDATE BO_ACQ_GROUP_TBL SET GROUP_LAST_SERIAL_NO = %d, LAST_WORK = '%s' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND CREATE_YEAR = '%s' AND GROUP_SEQ = %s;"),
			m_nBookLastSerialNo,
			pMgr->GetWorkLogMsg(WORK_LOG),
			m_strAcqYear,
			m_strGroupNo
		);
	}
	else if(strAcqCode == _T("2"))
	{
		strQuery.Format(
			_T("UPDATE BO_DON_RECEIPT_TBL SET LAST_SERIAL_NO = %d, LAST_WORK = '%s' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND RECEIPT_NO = %s;"),
			m_nBookLastSerialNo,
			pMgr->GetWorkLogMsg(WORK_LOG),
			m_strAcqYear,
			m_strGroupInfo
		);
	}
	else
	{
		strQuery.Format(
			_T("UPDATE BO_ACQ_SEQ_NO_TBL SET GROUP_LAST_SERIAL_NO = %d, LAST_WORK = '%s' WHERE ACQ_CODE='%s' AND CREATE_YEAR = '%s' AND GROUP_SEQ = %s;"),
			m_nBookLastSerialNo,
			pMgr->GetWorkLogMsg(WORK_LOG),
			m_strGroupNo,
			m_strAcqYear,
			m_strGroupInfo
			);
	}
	pDM_OUT->AddFrame(strQuery);
	return 0;
}

INT CGroupInfo::SetRecKey(CString strAcqCode, CESL_DataMgr * pDM, CString strMode /* = _T("MO") */)
{
	CString strQuery;
	if(strAcqCode == _T("0"))
	{
		if ( strMode == _T("SE") )
		{
			strQuery.Format(
				_T("SELECT REC_KEY FROM SE_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO =%s"),
				m_strAcqYear, m_strGroupInfo);
		}
		else// ( strMode == _T("MO") ) 
		{
			strQuery.Format(
				_T("SELECT REC_KEY FROM BO_ACQ_GROUP_TBL WHERE CREATE_YEAR='%s' AND GROUP_SEQ=%s"),
				m_strAcqYear, m_strGroupNo);
		}
	}
	else if(strAcqCode == _T("1"))
	{		
		if ( strMode == _T("SE") )
		{
			strQuery.Format(
				_T("SELECT REC_KEY FROM SE_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO =%s"),
				m_strAcqYear, m_strGroupInfo);
		}
		else// ( strMode == _T("MO") ) 
		{
			if( m_strWorkCode.IsEmpty() == TRUE )
			{
				strQuery.Format(
					_T("SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE IS NULL"),
					m_strAcqYear, m_strGroupInfo);
			}
			else
			{
				strQuery.Format(
					_T("SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE='%s'"),
					m_strAcqYear, m_strGroupInfo, m_strWorkCode);
			}
		}
	}
	else if(strAcqCode == _T("2"))
	{
		if ( strMode == _T("SE") )
		{
			strQuery.Format(
				_T("SELECT REC_KEY FROM SE_RECEIPT_TBL WHERE ACQ_YEAR='%s' AND RECEIPT_NO=%s"),
				m_strAcqYear, m_strGroupInfo);
		}
		else// ( strMode == _T("MO") )
		{
			strQuery.Format(
				_T("SELECT REC_KEY FROM BO_DON_RECEIPT_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND RECEIPT_NO=%s"),
				m_strAcqYear, m_strGroupInfo);
		}
	}
	else 
	{
		if(m_strGroupInfo == _T("적용안함"))
		{
			m_strRecKey = _T("-1");
			return 0;
		}
		
		if ( strMode == _T("SE") )
		{
			strQuery.Format(
				_T("SELECT REC_KEY FROM SE_PURCHASE_SEQ_NO_TBL WHERE SEQ_NO =%s"),
				m_strWorkCode);
		}
		else// ( strMode == _T("MO") )
		{
			strQuery.Format(
				_T("SELECT REC_KEY FROM BO_ACQ_SEQ_NO_TBL WHERE ACQ_CODE='%s' AND CREATE_YEAR ='%s' AND GROUP_SEQ=%s"),
				strAcqCode, m_strAcqYear, m_strGroupInfo);
		}
	}
	CString strResult;
	pDM->GetOneValueQuery(strQuery, strResult);
	m_strRecKey = strResult;
	return 0;
}

CString CGroupInfo::GetRecKey()
{
	return m_strRecKey;
}

INT CGroupInfo::UpdateLastWorNo(CESL_Mgr *pMgr, CESL_DataMgr *pDM)
{
	CString strQuery;
	strQuery.Format(
		_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET LAST_WORK='%s', LAST_WORK_NO=%s WHERE ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE='%s';"),
		pMgr->GetWorkLogMsg(WORK_LOG), m_strLastWorkNo, m_strAcqYear, m_strGroupInfo, m_strWorkCode);
	pDM->AddFrame(strQuery);
	return 0;
}

BOOL CGroupInfo::IsEqual(CString strAcqYear, CString strGroupInfo, CString strWorkCode/*=_T("")*/)
{

	if(m_strAcqYear == strAcqYear && m_strGroupInfo == strGroupInfo)
	{
		if(m_strWorkCode.IsEmpty())	return TRUE;
		else
		{
			if(m_strWorkCode == strWorkCode) return TRUE;
		}
	}
	return FALSE;
}

BOOL CGroupInfo::CheckValidationOrderProcess(CString strHoldDupSearchYN, CString strWorkDupSearchYN, CString strReviewYN, CString &strNextProcess, CESL_DataMgr * pDM)
{
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString strQuery;
	if(m_strWorkCode.IsEmpty())
		strQuery.Format(_T("SELECT DUP_CHECK_YN, COPY_CHECK_YN, REVIEW_CHECK_YN FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE IS NULL"),
			m_strAcqYear, m_strGroupInfo);
	else 
		strQuery.Format(_T("SELECT DUP_CHECK_YN, COPY_CHECK_YN, REVIEW_CHECK_YN FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE='%s'"),
			m_strAcqYear, m_strGroupInfo, m_strWorkCode);
	tmpDM.RestructDataManager(strQuery);
	if(tmpDM.GetRowCount() ==  0)
	{
		strNextProcess = _T("복본조사");
		return FALSE;
	}

	CString strDupCheckYN, strCopyCheckYN, strReviewCheckYN;
	strDupCheckYN = tmpDM.GetCellData(0, 0);
	strCopyCheckYN = tmpDM.GetCellData(0, 1);
	strReviewCheckYN = tmpDM.GetCellData(0, 2);

	if(strHoldDupSearchYN == _T("Y"))
	{
		if(strDupCheckYN == _T("N"))
		{
			strNextProcess = _T("복본조사");
			return FALSE;
		}
	}
	if(strWorkDupSearchYN == _T("Y"))
	{
		if(strCopyCheckYN == _T("N"))
		{
			strNextProcess = _T("중복조사");
			return FALSE;
		}
	}
	if(strReviewYN == _T("Y"))
	{
		if(strReviewCheckYN == _T("N"))
		{
			strNextProcess = _T("심의처리");
			return FALSE;
		}
	}
	return TRUE;
}

INT CGroupInfo::UpdateLastSerialNo(CString strAcqCode, CESL_Mgr *pMgr, CESL_DataMgr *pDM_OUT)
{
	CString strWorkNo, strQuery;
	if(strAcqCode == _T("1"))
	{
		if(m_nIdx >= 0) strWorkNo = m_arrWorkNo.GetAt(m_nIdx);
		else return -1;
		if(strWorkNo.IsEmpty()) return -1;

		if(strWorkNo == _T("사용안함")) strWorkNo = _T("0");
		if(m_strAcqYear.IsEmpty() || m_strGroupInfo.IsEmpty()) return 0;

		if(m_strWorkCode.IsEmpty())
			strQuery.Format(
				_T("UPDATE BO_PURCHASE_WORK_NO_TBL SET LAST_SERIAL_NO = %d, LAST_WORK = '%s' WHERE WORK_NO = %s AND PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE IS NULL);"),
				m_nLastSerialNo, 
				pMgr->GetWorkLogMsg(WORK_LOG),
				strWorkNo,
				m_strAcqYear,
				m_strGroupInfo
			);
		else
			strQuery.Format(
				_T("UPDATE BO_PURCHASE_WORK_NO_TBL SET LAST_SERIAL_NO = %d, LAST_WORK = '%s' WHERE WORK_NO = %s AND PURCHASE_SEQ_NO_KEY IN (SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE = '%s');"),
				m_nLastSerialNo, 
				pMgr->GetWorkLogMsg(WORK_LOG),
				strWorkNo,
				m_strAcqYear,
				m_strGroupInfo,
				m_strWorkCode
			);
	}
	else if(strAcqCode == _T("2"))
	{
		if(m_strAcqYear.IsEmpty() || m_strGroupInfo.IsEmpty()) return 0;
		strQuery.Format(
			_T("UPDATE BO_DON_RECEIPT_TBL SET LAST_SERIAL_NO = %d, LAST_WORK = '%s' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND RECEIPT_NO = %s;"),
			m_nLastSerialNo,
			pMgr->GetWorkLogMsg(WORK_LOG),
			m_strAcqYear,
			m_strGroupInfo
		);
	}
	else if(strAcqCode.IsEmpty() || strAcqCode == _T("0"))
	{
		if(m_strAcqYear.IsEmpty() || m_strGroupNo.IsEmpty()) return 0;
		strQuery.Format(
			_T("UPDATE BO_ACQ_GROUP_TBL SET GROUP_LAST_SERIAL_NO = %d, LAST_WORK = '%s' WHERE CREATE_YEAR = '%s' AND GROUP_SEQ = %s;"),
			m_nLastSerialNo,
			pMgr->GetWorkLogMsg(WORK_LOG),
			m_strAcqYear,
			m_strGroupNo
		);
	}
	else
	{
		if(m_strGroupInfo != _T("적용안함"))
		{
			if(m_strAcqYear.IsEmpty() || m_strGroupInfo.IsEmpty()) return 0;
			strQuery.Format(
				_T("UPDATE BO_ACQ_SEQ_NO_TBL SET GROUP_LAST_SERIAL_NO=%d, LAST_WORK='%s' WHERE ACQ_CODE='%s' AND CREATE_YEAR='%s' AND GROUP_SEQ=%s;"),
				m_nLastSerialNo,
				pMgr->GetWorkLogMsg(WORK_LOG),
				strAcqCode,
				m_strAcqYear,
				m_strGroupInfo
			);
		}
		else strQuery.Empty();
	}
	
	if(!strQuery.IsEmpty()) pDM_OUT->AddFrame(strQuery);
	return 0;
}

CString CGroupInfo::GetBookGroupQuery(INT nAcqType)
{
	CString strQuery;
	if(nAcqType == 0)
	{
		strQuery.Format(_T("((B.ACQ_CODE='0' OR B.ACQ_CODE IS NULL) AND B.ACQ_YEAR='%s' AND B.SEQ_NO=%s)"), m_strAcqYear, m_strGroupNo);
	}
	else if(nAcqType == 1)
	{
		if(m_strWorkCode.IsEmpty())
			strQuery.Format(_T("(B.ACQ_CODE='1' AND B.ACQ_YEAR='%s' AND B.SEQ_NO=%s AND I.WORK_CODE IS NULL)"), m_strAcqYear, m_strGroupInfo);
		else
			strQuery.Format(_T("(B.ACQ_CODE='1' AND B.ACQ_YEAR='%s' AND B.SEQ_NO=%s AND I.WORK_CODE='%s')"), m_strAcqYear, m_strGroupInfo, m_strWorkCode);
	}
	else if(nAcqType == 2)
	{
		strQuery.Format(_T("(B.ACQ_CODE='2' AND B.ACQ_YEAR='%s' AND B.SEQ_NO=%s)"), m_strAcqYear, m_strGroupNo);
	}
	return strQuery;
}

VOID CGroupInfo::SetLastWorkNo()
{
	INT cnt = m_arrWorkNo.GetSize();
	INT nLastWorkNo, nTmpData;
	nLastWorkNo = nTmpData = -1;
	for(INT i=0 ; i<cnt ; i++)
	{
		nTmpData = _ttoi(m_arrWorkNo.GetAt(i).GetBuffer(0));
		if(nLastWorkNo < nTmpData) nLastWorkNo = nTmpData;
	}
	m_strLastWorkNo.Format(_T("%d"), nLastWorkNo);
}
