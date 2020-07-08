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

BOOL CGroupInfo::IsEqual(CGroupInfo *pGroupInfo)
{
	CString strAcqYear, strGroupNo, strGroupInfo, strWorkCode;

	strAcqYear = pGroupInfo->GetAcqYear();
	strGroupInfo = pGroupInfo->GetGroupInfo();
	strGroupNo = pGroupInfo->GetGroupNo();
	strWorkCode = pGroupInfo->GetWorkCode();

	if( strAcqYear == m_strAcqYear && strGroupInfo == m_strGroupInfo && strGroupNo == m_strGroupNo )
	{
		if( m_strWorkCode.IsEmpty() == TRUE )	return TRUE;
		else
			if( strWorkCode == m_strWorkCode )	return TRUE;
	}

	return FALSE;
}

BOOL CGroupInfo::IsEqual(CString strAcqYear, CString strGroupInfo, CString strWorkCode/*=_T("")*/)
{

	if( m_strAcqYear == strAcqYear && m_strGroupInfo == strGroupInfo )
		{
		if( m_strWorkCode.IsEmpty() == TRUE )	return TRUE;
		else
			if( m_strWorkCode == strWorkCode )	return TRUE;
	}

	return FALSE;
}

INT CGroupInfo::UpDateBookLastSerialNo(CString strAcqCode, CESL_Mgr * pMgr, CESL_DataMgr *pDM_OUT)
{
	CString strQuery;

	if( strAcqCode.IsEmpty() == TRUE || strAcqCode == _T("0") )
	{
		strQuery.Format(
			_T("UPDATE BO_ACQ_GROUP_TBL SET GROUP_LAST_SERIAL_NO = %d, LAST_WORK = '%s' ")
			_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND CREATE_YEAR = '%s' AND GROUP_SEQ = %s;"),
			m_nBookLastSerialNo,
			pMgr->GetWorkLogMsg(WORK_LOG),
			m_strAcqYear,
			m_strGroupNo
		);
	}
	else if( strAcqCode == _T("2") )
	{
		strQuery.Format(
			_T("UPDATE BO_DON_RECEIPT_TBL SET LAST_SERIAL_NO = %d, LAST_WORK = '%s' ")
			_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND RECEIPT_NO = %s;"),
			m_nBookLastSerialNo,
			pMgr->GetWorkLogMsg(WORK_LOG),
			m_strAcqYear,
			m_strGroupInfo
		);
	}
	else
	{
		strQuery.Format(
			_T("UPDATE BO_ACQ_SEQ_NO_TBL SET GROUP_LAST_SERIAL_NO = %d, LAST_WORK = '%s' ")
			_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_CODE='%s' AND CREATE_YEAR = '%s' AND GROUP_SEQ = %s;"),
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

INT CGroupInfo::UpdateLastWorNo(CESL_Mgr *pMgr, CESL_DataMgr *pDM)
{
	CString strQuery;

	strQuery.Format(
		_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET LAST_WORK='%s', LAST_WORK_NO=%s ")
		_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE='%s';"),
		pMgr->GetWorkLogMsg(WORK_LOG), m_strLastWorkNo, m_strAcqYear, m_strGroupInfo, m_strWorkCode);

	pDM->AddFrame(strQuery);
	return 0;
}

INT CGroupInfo::UpDateLastSerialNo(CESL_Mgr * pMgr, CESL_DataMgr *pDM_OUT)
{
	CString strWorkNo, strQuery;

	if( m_nIdx >= 0 )					strWorkNo = m_arrWorkNo.GetAt(m_nIdx);
	if( strWorkNo == _T("사용안함") )	strWorkNo = _T("0");

	strQuery.Format(
		_T("UPDATE BO_PURCHASE_WORK_NO_TBL SET LAST_SERIAL_NO = %d, LAST_WORK = '%s' ")
		_T("WHERE WORK_NO = %s AND PURCHASE_SEQ_NO_KEY IN(")
		_T("SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s);"),
		m_nLastSerialNo, 
		pMgr->GetWorkLogMsg(WORK_LOG),
		strWorkNo,
		m_strAcqYear,
		m_strGroupInfo
	);

	pDM_OUT->AddFrame(strQuery);

	return 0;
}

BOOL CGroupInfo::CheckValidationOrderProcess(CString strHoldDupSearchYN, CString strWorkDupSearchYN, CString strReviewYN, CString &strNextProcess, CESL_DataMgr * pDM)
{
	CString strQuery, strDupCheckYN, strCopyCheckYN, strReviewCheckYN;
	CESL_DataMgr tmpDM;
	
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	
	if( m_strWorkCode.IsEmpty() == TRUE )
	{
		strQuery.Format(
			_T("SELECT DUP_CHECK_YN, COPY_CHECK_YN, REVIEW_CHECK_YN ")
			_T("FROM BO_PURCHASE_SEQ_NO_TBL ")
			_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE IS NULL"),
			m_strAcqYear, m_strGroupInfo);
	}
	else 
	{
		strQuery.Format(
			_T("SELECT DUP_CHECK_YN, COPY_CHECK_YN, REVIEW_CHECK_YN ")
			_T("FROM BO_PURCHASE_SEQ_NO_TBL ")
			_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE='%s'"),	
			m_strAcqYear, m_strGroupInfo, m_strWorkCode);
	}

	tmpDM.RestructDataManager(strQuery);
	if( tmpDM.GetRowCount() ==  0 )
	{
		strNextProcess = _T("복본조사");
		return FALSE;
	}

	strDupCheckYN = tmpDM.GetCellData(0, 0);
	strCopyCheckYN = tmpDM.GetCellData(0, 1);
	strReviewCheckYN = tmpDM.GetCellData(0, 2);

	if( strHoldDupSearchYN == _T("Y") )
	{
		if( strDupCheckYN == _T("N") )
		{
			strNextProcess = _T("복본조사");
			return FALSE;
		}
	}
	if( strWorkDupSearchYN == _T("Y") )
	{
		if( strCopyCheckYN == _T("N") )
		{
			strNextProcess = _T("중복조사");
			return FALSE;
		}
	}
	if( strReviewYN == _T("Y") )
	{
		if( strReviewCheckYN == _T("N") )
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

	if( strAcqCode == _T("1") )
	{
		if( m_nIdx >= 0 )	strWorkNo = m_arrWorkNo.GetAt(m_nIdx);
		else return -1;

		if( strWorkNo.IsEmpty() == TRUE )	return -1;
		if( strWorkNo == _T("사용안함") )	strWorkNo = _T("0");
		
		if( m_strAcqYear.IsEmpty() || m_strGroupInfo.IsEmpty() )	return 0;

		if( m_strWorkCode.IsEmpty() == TRUE )
		{
			strQuery.Format(_T("UPDATE BO_PURCHASE_WORK_NO_TBL SET ")
							_T("LAST_SERIAL_NO = %d, ")
							_T("LAST_WORK = '%s' ")
							_T("WHERE WORK_NO = %s ")
							_T("AND PURCHASE_SEQ_NO_KEY = ")
							_T("(SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE IS NULL);"),
				m_nLastSerialNo, 
				pMgr->GetWorkLogMsg(WORK_LOG),
				strWorkNo,
				m_strAcqYear,
							m_strGroupInfo	);
		}
		else
		{
			strQuery.Format(_T("UPDATE BO_PURCHASE_WORK_NO_TBL SET ")
							_T("LAST_SERIAL_NO = %d, ")
							_T("LAST_WORK = '%s' ")
							_T("WHERE WORK_NO = %s ")
							_T("AND PURCHASE_SEQ_NO_KEY = ")
							_T("(SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE = '%s');"),
				m_nLastSerialNo, 
				pMgr->GetWorkLogMsg(WORK_LOG),
				strWorkNo,
				m_strAcqYear,
				m_strGroupInfo,
							m_strWorkCode	);
		}
	}
	else if(strAcqCode == _T("2"))
	{
		if( m_strAcqYear.IsEmpty() || m_strGroupInfo.IsEmpty() )	return 0;
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
			_T("UPDATE BO_ACQ_GROUP_TBL SET GROUP_LAST_SERIAL_NO = %d, LAST_WORK = '%s' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND CREATE_YEAR = '%s' AND GROUP_SEQ = %s;"),
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
				_T("UPDATE BO_ACQ_SEQ_NO_TBL SET GROUP_LAST_SERIAL_NO=%d, LAST_WORK='%s' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_CODE='%s' AND CREATE_YEAR='%s' AND GROUP_SEQ=%s;"),
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

CString CGroupInfo::GetRecKey()
{
	return m_strRecKey;
}

CString CGroupInfo::GetWorkNo(INT idx)
{
	INT cnt = m_arrWorkNo.GetSize();
	
	if( cnt == 0 )		return _T("");
	if( idx == -1 ) 
	{
		if( m_nIdx < 0 )	m_nIdx = 0;
		return m_arrWorkNo.GetAt(m_nIdx);
	}

	return m_arrWorkNo.GetAt(idx);
}

CString CGroupInfo::GetBookGroupQuery(INT nAcqType)
{
	CString strQuery=_T("");

	switch( nAcqType )
	{
	case 0:	strQuery.Format(_T("((B.ACQ_CODE='0' OR B.ACQ_CODE IS NULL) AND B.ACQ_YEAR='%s' AND B.SEQ_NO=%s)"), m_strAcqYear, m_strGroupNo );
		break;

	case 1:
		{
			if( m_strWorkCode.IsEmpty() == TRUE )
			{
				strQuery.Format(_T("(B.ACQ_CODE='1' AND B.ACQ_YEAR='%s' AND B.SEQ_NO=%s AND I.WORK_CODE IS NULL)"), 
								m_strAcqYear, m_strGroupInfo );
			}
			else
			{
				strQuery.Format(_T("(B.ACQ_CODE='1' AND B.ACQ_YEAR='%s' AND B.SEQ_NO=%s AND I.WORK_CODE='%s')"), 
								m_strAcqYear, m_strGroupInfo, m_strWorkCode );
			}
		}
		break;

	case 2:	strQuery.Format(_T("(B.ACQ_CODE='2' AND B.ACQ_YEAR='%s' AND B.SEQ_NO=%s)"), m_strAcqYear, m_strGroupNo );
		break;

	default:	strQuery.Empty();
		break;
	}
	
	return strQuery;
}

INT CGroupInfo::SetRecKey(CString strAcqCode, CESL_DataMgr * pDM, CString strMode /* = _T("MO") */)
{
	CString strResult, strQuery = _T("");
	
	if( strAcqCode == _T("0") )
	{		
		if( strMode == _T("SE") )
		{
			strQuery.Format(
				_T("SELECT REC_KEY FROM SE_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO =%s"),
				m_strAcqYear, m_strGroupInfo);
		}
		else
		{
			strQuery.Format(
				_T("SELECT REC_KEY FROM BO_ACQ_GROUP_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND CREATE_YEAR='%s' AND GROUP_SEQ=%s"),
				m_strAcqYear, m_strGroupNo);
		}
	}
	else if( strAcqCode == _T("1") )
	{		
		if( strMode == _T("SE") )
		{
			strQuery.Format(
				_T("SELECT REC_KEY FROM SE_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO =%s"),
				m_strAcqYear, m_strGroupInfo);
		}
		else
		{
			if( m_strWorkCode.IsEmpty() == TRUE )
			{
				strQuery.Format(_T("SELECT REC_KEY ")
								_T("FROM BO_PURCHASE_SEQ_NO_TBL ")
								_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE IS NULL"),	m_strAcqYear, m_strGroupInfo);
			}
			else
			{
				strQuery.Format(_T("SELECT REC_KEY ")
								_T("FROM BO_PURCHASE_SEQ_NO_TBL ")
								_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE='%s'"),	m_strAcqYear, m_strGroupInfo, m_strWorkCode);
			}
		}
	}
	else if( strAcqCode == _T("2") )
	{		
		if( strMode == _T("SE") )
	{
			strQuery.Format(
				_T("SELECT REC_KEY FROM SE_RECEIPT_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND RECEIPT_NO=%s"),
				m_strAcqYear, m_strGroupInfo);
		}
		else
		{
			strQuery.Format(
				_T("SELECT REC_KEY FROM BO_DON_RECEIPT_TBL WHERE  MANAGE_CODE = UDF_MANAGE_CODE AND  ACQ_YEAR='%s' AND RECEIPT_NO=%s"),
				m_strAcqYear, m_strGroupInfo);
		}
	}
	else 
	{
		if( m_strGroupInfo == _T("적용안함") )
		{
			m_strRecKey = _T("-1");
			return 0;
		}
		
		if( strMode == _T("SE") )
	{
			if( m_strWorkCode.IsEmpty() == TRUE )
			{
				strQuery.Format(_T("SELECT REC_KEY FROM SE_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND SEQ_NO IS NULL"));
			}
		else
			{
				strQuery.Format(_T("SELECT REC_KEY FROM SE_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND SEQ_NO =%s"), m_strWorkCode);
			}
	}
		else
	{
			strQuery.Format(
				_T("SELECT REC_KEY FROM BO_ACQ_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_CODE='%s' AND CREATE_YEAR ='%s' AND GROUP_SEQ=%s"),
				strAcqCode, m_strAcqYear, m_strGroupInfo);
		}
	}
	pDM->GetOneValueQuery(strQuery, strResult);
	m_strRecKey = strResult;
	
	return 0;
}

VOID CGroupInfo::SetLastWorkNo()
{
	INT cnt, nLastWorkNo, nTmpData, i;

	nLastWorkNo = nTmpData = -1;
	cnt = m_arrWorkNo.GetSize();	
	for( i=0 ; i<cnt ; i++ )
	{
		nTmpData = _ttoi(m_arrWorkNo.GetAt(i).GetBuffer(0));
		if( nLastWorkNo < nTmpData )	 nLastWorkNo = nTmpData;
	}

	m_strLastWorkNo.Format(_T("%d"), nLastWorkNo);
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

VOID CGroupInfo::SetArrayWorkNo(CStringArray *pArray)
{
	m_arrWorkNo.RemoveAll();
	INT cnt = pArray->GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		m_arrWorkNo.Add(pArray->GetAt(i));
	}
}

INT CGroupInfo::GetLastSerialNo(INT nAcqType/*=-1*/, CESL_DataMgr* pDM/*==NULL*/)
{
	INT ids = -1;
		
	switch( nAcqType )
	{	
	case 0:		ids = GetGroupLastSerialNum(pDM);
		break;
	case 1:		ids = GetDivNoLastSerialNum(pDM);
		break;
	case 2:		ids = GetReceiptNoLastSerialNum(pDM);
		break;
	
	default:
		break;
	}
	
	return m_nLastSerialNo;
}

INT CGroupInfo::GetGroupLastSerialNum(CESL_DataMgr* pDM)
{
	INT ids, nLastSerialNum;
	CString strQuery, strResult;
	CESL_DataMgr tmpDM;
	
	if( pDM == NULL ) return -1;

	if( m_strAcqYear.IsEmpty() || m_strGroupInfo == _T("적용안함") )
	{
		nLastSerialNum = 0;
	}
	else
	{
		tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);	
		strQuery.Format(_T("SELECT GROUP_LAST_SERIAL_NO ")
						_T("FROM BO_ACQ_GROUP_TBL ")
						_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND CREATE_YEAR = '%s' AND GROUP_NAME = '%s'"), m_strAcqYear, m_strGroupInfo);
		ids = tmpDM.RestructDataManager(strQuery);
		if( ids < 0 )  return -1;
		
		strResult = tmpDM.GetCellData(0,0);
		nLastSerialNum = _ttoi(strResult.GetBuffer(0));
	}

	if( m_nLastSerialNo != nLastSerialNum )	m_nLastSerialNo = nLastSerialNum;
	
	return 0;
}

INT CGroupInfo::GetDivNoLastSerialNum(CESL_DataMgr* pDM)
{
	INT ids, nLastNum;
	CString strQuery, strResult, strWorkNo;
	CESL_DataMgr tmpDM;

	if( pDM == NULL ) return -1;

	if( m_nIdx >= 0 )	strWorkNo = m_arrWorkNo.GetAt(m_nIdx);
	else				return -1;

	if( strWorkNo == _T("사용안함") )	strWorkNo = _T("0");

	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	if( m_strWorkCode.IsEmpty() == TRUE )
	{
		strQuery.Format(_T("SELECT LAST_SERIAL_NO ")
						_T("FROM BO_PURCHASE_WORK_NO_TBL ")
						_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND WORK_NO = %s ")
						_T("AND PURCHASE_SEQ_NO_KEY = ")
						_T("(SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE IS NULL)"),
						strWorkNo, m_strAcqYear, m_strGroupInfo);
	}
	else
	{
		strQuery.Format(_T("SELECT LAST_SERIAL_NO ")
						_T("FROM BO_PURCHASE_WORK_NO_TBL ")
						_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND WORK_NO = %s ")
						_T("AND PURCHASE_SEQ_NO_KEY = ")
						_T("(SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE = %s)"),
						strWorkNo, m_strAcqYear, m_strGroupInfo, m_strWorkCode);
	}

	ids = tmpDM.RestructDataManager(strQuery);
	if( ids < 0 )	return -1;

	strResult	= tmpDM.GetCellData(0,0);
	nLastNum	= _ttoi(strResult.GetBuffer(0));	

	if( nLastNum != m_nLastSerialNo )	m_nLastSerialNo = nLastNum;
	return 0;
}

INT CGroupInfo::GetReceiptNoLastSerialNum(CESL_DataMgr* pDM)
{
	INT ids, nLastNum;
	CString strQuery, strResult;
	CESL_DataMgr tmpDM;	

	if( pDM == NULL ) return -1;
	if( m_strAcqYear.IsEmpty() || m_strGroupInfo.IsEmpty() || m_strGroupInfo == _T("적용안함") )
	{
		return -1;
	}	
	
	tmpDM.SetCONNECTION_INFO(pDM->CONNECTION_INFO);	
	strQuery.Format(_T("SELECT LAST_SERIAL_NO ")
					_T("FROM BO_DON_RECEIPT_TBL ")
					_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND RECEIPT_NO = %s"), m_strAcqYear, m_strGroupInfo);
	ids = tmpDM.RestructDataManager(strQuery);
	if( ids < 0 ) return -1;

	strResult = tmpDM.GetCellData(0,0);
	nLastNum = _ttoi(strResult.GetBuffer(0));

	if( nLastNum != m_nLastSerialNo )	m_nLastSerialNo = nLastNum;
	return 0;
}
