// GroupCollection.cpp: implementation of the CGroupCollection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GroupCollection.h"
#include "GroupMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("그룹콜렉션"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGroupCollection::CGroupCollection(CESL_Mgr * pParentMgr)
{
	m_pMgr = pParentMgr;
	Init();
}

CGroupCollection::~CGroupCollection()
{
	RemoveAll();
}

VOID CGroupCollection::Init()
{
	m_bHaveGroupInfo = FALSE;
	RemoveAll();
	m_nIdx = -1;
	m_nAcqType = ACQ_TYPE_INVALID;
	m_bAllGroupInfo = FALSE;
}

INT CGroupCollection::ParsingStringApplyDuplicate(CString DelimeterStr, CString strSource, CStringArray *pTargetArray)
{
	if(strSource.GetLength() == 0) return 0;
	INT pos = strSource.Find(DelimeterStr, 0);
	INT nDelimeterCnt = DelimeterStr.GetLength();
	CString strTmpData;
	while(pos > 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+nDelimeterCnt);
		strTmpData.TrimLeft();	strTmpData.TrimRight();
		pTargetArray->Add(strTmpData);
		pos = strSource.Find(DelimeterStr, 0);
	}
	strSource.TrimLeft();	strSource.TrimRight();
	pTargetArray->Add(strSource);
	return 0;
}

INT CGroupCollection::ParsingString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray)
{
	if(strSource.GetLength() == 0) return 0;
	INT pos = strSource.Find(DelimeterStr, 0);
	CString strTmpData;
	while(pos > 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+1);
		((CIndexTool*)m_pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
		if(!CheckIsHave(pTargetArray, strTmpData))
			pTargetArray->Add(strTmpData);
		pos = strSource.Find(DelimeterStr, 0);
	}
	((CIndexTool*)m_pMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strSource);
	if(!CheckIsHave(pTargetArray, strSource))
		pTargetArray->Add(strSource);
	return 0;
}

BOOL CGroupCollection::CheckIsHave(CStringArray *codeValue, CString code)
{
	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
	return FALSE;
}

VOID CGroupCollection::AddGroupInfo(CGroupInfo * pGroupInfo)
{
	CGroupInfo * pGroup = new CGroupInfo;
	pGroupInfo->CopyData(pGroup);
	m_arrGroupInfo.Add(pGroup);
}

INT CGroupCollection::GetGroupInfo(CStringArray * pGroupInfo, CString &strCurSelInfo)
{
	INT cnt = m_arrGroupInfo.GetSize();
	pGroupInfo->RemoveAll();
	CString strTmpData;
	
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = m_arrGroupInfo.GetAt(i)->GetGroupInfo();
		pGroupInfo->Add(strTmpData);
	}

	if(m_nIdx < 0) return 0;
	if(pGroupInfo->GetSize() == 0) return 0;
	strCurSelInfo = pGroupInfo->GetAt(m_nIdx);
	return 0;
}

INT CGroupCollection::GetGroupInfo(CString &strGroupInfo)
{
	if(m_nAcqType == ACQ_TYPE_INVALID && m_bAllGroupInfo)
	{
		strGroupInfo = _T("적용안함");
		return 0;
	}
	else if(m_bAllGroupInfo)
	{
		strGroupInfo = _T("전체자료");
		return 0;
	}

	INT cnt = m_arrGroupInfo.GetSize();
	strGroupInfo.Empty();
	CString strTmpData;
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = m_arrGroupInfo.GetAt(i)->GetGroupInfo();
		if(strTmpData == _T("적용안함")) continue;
		if(strGroupInfo.IsEmpty()) strGroupInfo = strTmpData;
		else strGroupInfo += _T(", ") + strTmpData;
	}
	return 0;
}

INT CGroupCollection::SetCurSel(INT idx)
{
	if(idx >= m_arrGroupInfo.GetSize())	
		return -1;
	m_nIdx = idx;

	if(m_nAcqType == PURCHASE)
		MakeWorkNoInfo();
	else
		MakeSerialNo(m_nIdx);

	return 0;
}

INT CGroupCollection::ValidateGroupInfo(INT idx)
{
	INT cnt = m_arrGroupInfo.GetSize();
	if(idx == -1)	
	{
		for(INT i=0 ; i<cnt ; i++)
		{
			CheckValidate(i);
		}
		return 0;
	}
	else if(cnt <= idx)
	{
		m_pMgr->ESLAfxMessageBox(_T("인덱스 정보가 올바르지 않습니다."));
		return -1;
	}
	CheckValidate(idx);
	return 0;
}

INT CGroupCollection::CheckValidate(CString strGroupInfo, CString strAcqYear, CString strWorkCode/*=_T("")*/)
{
	CGroupMgr m_GroupMgr(m_pMgr);
	m_GroupMgr.InitTempData();
	if(m_nAcqType == OTHER_ACQ_TYPE)
	{
		if(m_strAcqCode.IsEmpty()) return -1;
		m_GroupMgr.SetTempData(_T("수입구분"), _ttoi(m_strAcqCode.GetBuffer(0)));
	}
	else m_GroupMgr.SetTempData(_T("수입구분"), (INT)m_nAcqType);
	m_GroupMgr.SetTempData(_T("수입년도"), strAcqYear.GetBuffer(0));
	m_GroupMgr.SetTempData(_T("업무구분"), strWorkCode.GetBuffer(0));
	if(m_nAcqType == PURCHASE)
		m_GroupMgr.SetTempData(_T("작업상태"), m_strWorkStatus.GetBuffer(0));
	else 
		m_GroupMgr.SetTempData(_T("작업상태"), _T(""));
	INT ids = -1;

	m_GroupMgr.SetTempData(_T("그룹번호"), strGroupInfo.GetBuffer(0));
	ids = m_GroupMgr.SPFExecutePath(_T("그룹번호유효성검사"));
	return ids;
}

INT CGroupCollection::CheckValidate(INT idx)
{
	CString strMsg;
	CGroupInfo * info = m_arrGroupInfo.GetAt(idx);

	CGroupMgr m_GroupMgr(m_pMgr);
	m_GroupMgr.InitTempData();
	m_GroupMgr.SetTempData(_T("수입구분"), m_nAcqType);
	m_GroupMgr.SetTempData(_T("수입년도"), info->GetAcqYear().GetBuffer(0));
	if(m_nAcqType == PURCHASE)
		m_GroupMgr.SetTempData(_T("작업상태"), m_strWorkStatus.GetBuffer(0));
	else 
		m_GroupMgr.SetTempData(_T("작업상태"), _T(""));
	INT ids = -1;

	m_GroupMgr.SetTempData(_T("그룹번호"), info->GetGroupNo().GetBuffer(0));
	ids = m_GroupMgr.SPFExecutePath(_T("그룹번호유효성검사"));
	if(ids < 0)
	{
		if(m_nAcqType == PURCHASE)
			strMsg.Format(_T("입력하신 차수번호 [%s]는 사용할 수 없습니다."), info->GetGroupNo());
		else if(m_nAcqType == DONATE)
			strMsg.Format(_T("입력하신 접수번호 [%s]는 사용할 수 없습니다."), info->GetGroupNo());
		else if(m_nAcqType == ACQ_TYPE_INVALID)
			strMsg.Format(_T("입력하신 그룹명 [%s]는 사용할 수 없습니다."), info->GetGroupNo());
		else if(m_nAcqType == OTHER_ACQ_TYPE)
			strMsg.Format(_T("입력하신 그룹번호 [%s]는 사용할 수 없습니다."), info->GetGroupNo());

		m_pMgr->ESLAfxMessageBox(strMsg);
		m_arrGroupInfo.RemoveAt(idx);
	}
	
	return 0;
}

CGroupInfo * CGroupCollection::GetCurSelGroupInfo()
{
	if(m_nIdx < 0) return NULL;
	if(m_arrGroupInfo.GetSize() == 0) return NULL;

	return m_arrGroupInfo.GetAt(m_nIdx);
}

INT CGroupCollection::Init(CString strAcqYear)
{
	RemoveAll();
	INT ids = MakeGroupInfo(strAcqYear);
	if(ids < 0) return -1;
	m_nIdx = 0;
	return 0;
}

INT CGroupCollection::MakeGroupInfo(CString strAcqYear, CString strGroupInfo, CString strWorkCode)
{
	INT ids = -1;

	if( strAcqYear.IsEmpty() == TRUE )
	{
		m_pMgr->ESLAfxMessageBox(_T("수입년도가 올바르지 않습니다."));
		return -1;
	}

	RemoveAll();
	
	switch( m_nAcqType )
	{
	case PURCHASE:			ids = MakeDivNoInfo(m_strWorkStatus, strAcqYear, strGroupInfo, strWorkCode);
		break;
	case DONATE:			ids = MakeReceiptNoInfo(strAcqYear, strGroupInfo);
		break;
	case ACQ_TYPE_INVALID:	ids = MakeGroupNameInfo(strAcqYear, strGroupInfo);
		break;
	case OTHER_ACQ_TYPE:
	{
		if(m_strAcqCode.IsEmpty()) return -1;
		ids = MakeOtherGroupNoInfo(m_strAcqCode, strAcqYear, strGroupInfo);
	}
		break;
	}
	if( ids < 0 )	return ids ;

	m_nIdx = 0;
	
	if( m_nAcqType == PURCHASE )	MakeWorkNoInfo();
	else if(m_nAcqType == ACQ_TYPE_INVALID)
	{
		CGroupInfo* pGroup = new CGroupInfo();
		pGroup->SetAcqYear(strAcqYear);
		pGroup->SetGroupInfo(_T("적용안함"));
		pGroup->SetGroupNo(_T("0"));
		pGroup->SetLastSerialNo(0);
		m_arrGroupInfo.InsertAt(0, pGroup);
		MakeSerialNo(0);
	}
	else if(m_nAcqType == OTHER_ACQ_TYPE)
	{
		CGroupInfo * pGroup = new CGroupInfo();
		pGroup->SetAcqYear(strAcqYear);
		pGroup->SetGroupInfo(_T("적용안함"));
		pGroup->SetLastSerialNo(0);
		m_arrGroupInfo.InsertAt(0, pGroup);
		MakeSerialNo(0);
	}

	m_bHaveGroupInfo = TRUE;

	if( strGroupInfo.IsEmpty() == TRUE )	m_bAllGroupInfo = TRUE;
	else m_bAllGroupInfo = FALSE;

	return 0;
}

INT CGroupCollection::SetDM(CESL_DataMgr * pDM, CString strDMAlias)
{
	CGroupMgr GroupMgr(m_pMgr);
	pDM = new CESL_DataMgr;
	pDM->SetCONNECTION_INFO(m_pMgr->CONNECTION_INFO);
	pDM->InitDMPKFromFile(strDMAlias);
	pDM->SetMarcMgr(m_pMgr->m_pInfo->pMarcMgr);
	return 0;
}

INT CGroupCollection::MakeDivNoInfo(CString strWorkingStatus, CString strAcqYear, CString strGroupInfo, CString strWorkCode/*=_T("")*/)
{
	INT i, ids, col, nCols;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE, strQuery, strOption;	
	CString strGroupInfoAlias[4] = {	_T("차수번호"),
										_T("수입년도"),
										_T("마지막작업번호"),
										_T("업무구분")	};
	CESL_DataMgr DM;
	CGroupMgr GroupMgr(m_pMgr);

	DM.SetCONNECTION_INFO(m_pMgr->CONNECTION_INFO);
	DM.InitDMPKFromFile(_T("DM_구입차수관리"));
	DM.SetMarcMgr(m_pMgr->m_pInfo->pMarcMgr);

	nCols = DM.m_nCols;
	if( nCols < 0 )		nCols = DM.GetRefColumnCount();
	for( col=0; col<nCols; col++ )
	{
		if( DM.GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0 )
		{
			continue;
		}
		if( FIELD_NAME.Find(_T("(")) >= 0 )
		{
			DM.SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, _T("udf"));
		}
	}

	if		( strWorkingStatus == _T("구입대상") )	strWorkingStatus = _T("0");
	else if	( strWorkingStatus == _T("주문")	 )	strWorkingStatus = _T("1");
	else if	( strWorkingStatus == _T("검수완료") )	strWorkingStatus = _T("2");
	
	if( strWorkCode.IsEmpty() )	
	{
		strQuery.Format(_T("ACQ_YEAR='%s' AND WORK_CODE IS NULL"), strAcqYear);
	}
	else						
	{
		strQuery.Format(_T("ACQ_YEAR='%s' AND WORK_CODE='%s'"), strAcqYear, strWorkCode);
	}

	if( strGroupInfo.IsEmpty() == FALSE )
	{
		strOption.Format(_T(" AND SEQ_NO IN (%s)"), strGroupInfo);
		strQuery += strOption;
	}
	
	ids = DM.RefreshDataManager(strQuery);
	if( ids < 0 )	return -1;
	
	CString strGroupInfoData[4];
	CGroupInfo* pGroup = NULL;
	for( i=0 ; i<DM.GetRowCount(); i++ )
	{
		DM.GetCellData(strGroupInfoAlias, 4, strGroupInfoData, i);
		
		pGroup = new CGroupInfo();
		pGroup->SetGroupInfo(strGroupInfoData[0]);
		pGroup->SetAcqYear(strGroupInfoData[1]);
		pGroup->SetLastWorkNo(strGroupInfoData[2]);
		pGroup->SetWorkCode(strGroupInfoData[3]);
		m_arrGroupInfo.Add(pGroup);
	}
	return 0;
}

INT CGroupCollection::MakeReceiptNoInfo(CString strAcqYear, CString strGroupInfo)
{
	CESL_DataMgr DM;
	CGroupMgr GroupMgr(m_pMgr);
	DM.SetCONNECTION_INFO(m_pMgr->CONNECTION_INFO);
	DM.InitDMPKFromFile(_T("DM_접수번호관리"));
	DM.SetMarcMgr(m_pMgr->m_pInfo->pMarcMgr);
	
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	INT col, nCols;
	nCols = DM.m_nCols;
	if(nCols < 0) nCols = DM.GetRefColumnCount();
	for (col = 0; col < nCols; col++) {
		if (DM.GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0) continue;
		if(FIELD_NAME.Find(_T("(")) >= 0)
		{
			DM.SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, _T("udf"));
		}
	}

	CString strQuery, strOption;
	
	CString strGroupInfoAlias[3];
	strGroupInfoAlias[0] = _T("접수번호");
	strGroupInfoAlias[1] = _T("수입년도");
	strGroupInfoAlias[2] = _T("마지막일련번호");
	strQuery.Format(_T("ACQ_YEAR='%s'"), strAcqYear);
	if(!strGroupInfo.IsEmpty())
	{
		strOption.Format(_T(" AND RECEIPT_NO IN (%s)"), strGroupInfo);
		strQuery += strOption;
	}
	
	INT ids = DM.RefreshDataManager(strQuery);
	if(ids < 0) return -1;
	
	CString strGroupInfoData[3];

	INT nRowCnt = DM.GetRowCount();

	INT j = -1;
	CGroupInfo * pGroup = NULL;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		DM.GetCellData(strGroupInfoAlias, 3, strGroupInfoData, i);
		
		pGroup = new CGroupInfo();
		pGroup->SetGroupInfo(strGroupInfoData[0]);
		pGroup->SetAcqYear(strGroupInfoData[1]);
		pGroup->SetLastSerialNo(_ttoi(strGroupInfoData[2].GetBuffer(0)));
		m_arrGroupInfo.Add(pGroup);
	}
	return 0;
}

INT CGroupCollection::MakeGroupNameInfo(CString strAcqYear, CString strGroupInfo)
{
	CESL_DataMgr DM ;
	CGroupMgr GroupMgr(m_pMgr);
	DM.SetCONNECTION_INFO(m_pMgr->CONNECTION_INFO);
	DM.InitDMPKFromFile(_T("DM_그룹관리"));
	DM.SetMarcMgr(m_pMgr->m_pInfo->pMarcMgr);
	
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	INT col, nCols;
	nCols = DM.m_nCols;
	if(nCols < 0) nCols = DM.GetRefColumnCount();
	for (col = 0; col < nCols; col++) {
		if (DM.GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0) continue;
		if(FIELD_NAME.Find(_T("(")) >= 0)
		{
			DM.SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, _T("udf"));
		}
	}

	CString strQuery, strOption;
	
	CString strGroupInfoAlias[4];
	
	strGroupInfoAlias[0] = _T("그룹이름");
	strGroupInfoAlias[1] = _T("그룹생성년도");
	strGroupInfoAlias[2] = _T("그룹번호");
	strGroupInfoAlias[3] = _T("마지막일련번호");
	
	strQuery.Format(_T("CREATE_YEAR='%s'"), strAcqYear);
	if(!strGroupInfo.IsEmpty())
	{
 		CString strTempGroupInfo;
		strTempGroupInfo = strGroupInfo;
		strTempGroupInfo.Replace(_T("'"),_T(""));

		strOption.Format(_T(" AND GROUP_NAME IN ('%s')"), strTempGroupInfo);
		
		strQuery += strOption;
	}

	INT ids = DM.RefreshDataManager(strQuery);
	if(ids < 0) return -1;
	
	CString strGroupInfoData[4];

	INT nRowCnt = DM.GetRowCount();

	INT j = -1;
	CGroupInfo * pGroup = NULL;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		DM.GetCellData(strGroupInfoAlias, 4, strGroupInfoData, i);
		
		pGroup = new CGroupInfo();
		pGroup->SetGroupInfo(strGroupInfoData[0]);
		pGroup->SetAcqYear(strGroupInfoData[1]);
		pGroup->SetGroupNo(strGroupInfoData[2]);
		pGroup->SetLastSerialNo(_ttoi(strGroupInfoData[3].GetBuffer(0)));

		m_arrGroupInfo.Add(pGroup);
	}

	return 0;
}

INT CGroupCollection::MakeGroupInfo(CString strAcqYear)
{
	if(strAcqYear.IsEmpty())
	{
		m_pMgr->ESLAfxMessageBox(_T("수입년도가 올바르지 않습니다."));
		return -1;
	}
	RemoveAll();
	
	INT ids = -1;
	if(m_nAcqType == PURCHASE) ids = MakeDivNoInfo(m_strWorkStatus, strAcqYear, _T(""));
	else if(m_nAcqType == DONATE) ids = MakeReceiptNoInfo(strAcqYear, _T(""));
	else if(m_nAcqType == ACQ_TYPE_INVALID) ids = MakeGroupNameInfo(strAcqYear, _T(""));
	if(ids < 0) return ids ;

	m_nIdx = 0;
	if(m_nAcqType == PURCHASE)
		MakeWorkNoInfo();
	else if(m_nAcqType == ACQ_TYPE_INVALID)
	{
		CGroupInfo * pGroup = new CGroupInfo();
		pGroup->SetAcqYear(strAcqYear);
		pGroup->SetGroupInfo(_T("적용안함"));
		pGroup->SetGroupNo(_T("0"));
		pGroup->SetLastSerialNo(0);
		m_arrGroupInfo.InsertAt(0, pGroup);
		MakeSerialNo(0);
	}

	if(m_arrGroupInfo.GetSize() != 0)
	{
		m_bHaveGroupInfo = TRUE;
		m_bAllGroupInfo = TRUE;
	}

	return 0;
}

INT CGroupCollection::GetWorkNoInfo(CStringArray * pWorkNo)
{
	pWorkNo->RemoveAll();
	CGroupInfo *Group = m_arrGroupInfo.GetAt(m_nIdx);
	pWorkNo = Group->GetArrayWorkNo();
	return 0;
}

INT CGroupCollection::AddWorkNoInfo(CString strWorkNo)
{
	CGroupInfo *Group = m_arrGroupInfo.GetAt(m_nIdx);
	CStringArray * pWorkNo = Group->GetArrayWorkNo();
	INT cnt = pWorkNo->GetSize();
	CString strTmpData = pWorkNo->GetAt(cnt-1);
	pWorkNo->SetAt(cnt-1, strWorkNo);
	pWorkNo->Add(strTmpData);
	SetCurSelWorkNo(cnt-1);
	return 0;
}

INT CGroupCollection::GetLastSerailNo(INT idx)
{
	CGroupInfo *Group = m_arrGroupInfo.GetAt(m_nIdx);
	return Group->GetLastSerialNo();
}

INT CGroupCollection::MakeWorkNoInfo()
{
	if(m_arrGroupInfo.GetSize() == 0) return 0;

	CGroupInfo * pGroup = m_arrGroupInfo.GetAt(m_nIdx);
	if(!pGroup) return -1;
	if(pGroup->GetHaveWorkNo()) return 0;
	if(pGroup->GetGroupInfo() == _T("적용안함")) return 0;
	
	CGroupMgr m_GroupMgr(m_pMgr);
	m_GroupMgr.InitTempData();
	m_GroupMgr.SetTempData(_T("ACQ_YEAR"), pGroup->GetAcqYear().GetBuffer(0));
	m_GroupMgr.SetTempData(_T("Group_No"), pGroup->GetGroupInfo().GetBuffer(0));
	m_GroupMgr.SetTempData(_T("WORK_CODE"), pGroup->GetWorkCode().GetBuffer(0));
	m_GroupMgr.SetTempPtr(_T("Work_No"), (DWORD*)pGroup->GetArrayWorkNo());
	INT ids = m_GroupMgr.SPFExecutePath(_T("작업번호정보추출"));
	if(ids < 0) return ids;
	else if(ids == 1000)
	{
		pGroup->GetArrayWorkNo()->Add(_T("새로생성"));
	}
	
	pGroup->SetHaveWorkNo(TRUE);
	pGroup->SetIndex(0);
	INT cnt = pGroup->GetArrayWorkNo()->GetSize();
	pGroup->SetLastWorkNo();

	ids = MakeSerialNo(0);
	
	return ids;
}

INT CGroupCollection::MakeSerialNo(INT idx)
{
	CString strWorkNo, strLastNo;
	CGroupInfo* pGroup = m_arrGroupInfo.GetAt(m_nIdx);
	if( pGroup == NULL )	return -1;

	if( m_nAcqType == PURCHASE )
	{
		strWorkNo = pGroup->GetWorkNo(idx);
		if( strWorkNo == _T("새로생성") )
		{
			pGroup->SetLastSerialNo(1);
			return 0;
		}
		else if( strWorkNo == _T("사용안함") )	
		{
			strWorkNo = _T("0");
	}
	}

	CGroupMgr m_GroupMgr(m_pMgr);
	if( m_nAcqType == OTHER_ACQ_TYPE )
	{
		m_GroupMgr.InitTempData();
		m_GroupMgr.SetTempData(_T("수입구분"), m_strAcqCode.GetBuffer(0));
	}

	m_GroupMgr.GetLastSerialNumber((INT)m_nAcqType, pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strWorkNo, strLastNo, pGroup->GetWorkCode());
	if(strLastNo.IsEmpty()) return 0;
	pGroup->SetLastSerialNo(_ttoi(strLastNo.GetBuffer(0)));
	pGroup->SetHaveLastSerialNo(TRUE);
	return 0;
}

INT CGroupCollection::AddGroupInfo(CString strAcqYear, CString strGroupInfo, CString strGroupNo/*=_T("")*/)
{
	CGroupInfo * pGroup = new CGroupInfo();
	pGroup->SetAcqYear(strAcqYear);
	pGroup->SetGroupInfo(strGroupInfo);
	if(m_nAcqType == PURCHASE) 
	{
		if( GetWorkCode() == strGroupNo || GetGroupCount() == 0)
			pGroup->SetWorkCode(strGroupNo);
		else														return 0;
	}
	else if(m_nAcqType == ACQ_TYPE_INVALID || m_nAcqType == OTHER_ACQ_TYPE)
		pGroup->SetGroupNo(strGroupNo);
	
	m_arrGroupInfo.Add(pGroup);
	m_nIdx = m_arrGroupInfo.GetSize() - 1;

	if(m_nAcqType == PURCHASE && strGroupInfo != _T("적용안함"))
		MakeWorkNoInfo();
	return 0;
}

INT CGroupCollection::RemoveAt(INT idx)
{
	INT cnt = m_arrGroupInfo.GetSize();
	if(idx < 0) idx = m_nIdx;
	if(m_nIdx < 0 || idx >= cnt) return -1;
	
	CGroupInfo * pGroup = m_arrGroupInfo.GetAt(idx);
	if(pGroup)
	{
		delete pGroup;
		pGroup = NULL;
	}
	
	m_arrGroupInfo.RemoveAt(idx);
	if(m_arrGroupInfo.GetSize() <= m_nIdx)
		m_nIdx = m_arrGroupInfo.GetSize()-1;

	if(m_arrGroupInfo.GetSize() == 0)
	{
		m_nIdx = -1;
		m_bHaveGroupInfo = FALSE;
	}
	
	return 0;
}

INT CGroupCollection::RemoveAll()
{
	INT i = 0;
	while( i < m_arrGroupInfo.GetSize() )
	{
		delete m_arrGroupInfo.GetAt( i++ );
	}

	m_arrGroupInfo.RemoveAll();
	m_nIdx = -1;
	m_bHaveGroupInfo = FALSE;

	return 0;
}

INT CGroupCollection::SetCurSelGroupInfo(CString strAcqYear, CString strGroupInfo, CString strWorkNo/*=_T("")*/, CString strWorkCode/*=_T("")*/)
{
	INT cnt = m_arrGroupInfo.GetSize();
	if(cnt == 0) return -1;
	for(INT i=0 ; i<cnt ; i++)
	{	
		if(m_nAcqType == ACQ_TYPE_INVALID)
		{
			if(strAcqYear == m_arrGroupInfo.GetAt(i)->GetAcqYear() && 
				strGroupInfo == m_arrGroupInfo.GetAt(i)->GetGroupNo())
				break;
		}
		else if(m_nAcqType == DONATE)
		{
			if(strAcqYear == m_arrGroupInfo.GetAt(i)->GetAcqYear() && 
				strGroupInfo == m_arrGroupInfo.GetAt(i)->GetGroupInfo())
				break;
		}
		else if(m_nAcqType == PURCHASE)
		{
			if(strAcqYear == m_arrGroupInfo.GetAt(i)->GetAcqYear() &&
				strWorkCode == m_arrGroupInfo.GetAt(i)->GetWorkCode() && 
				strGroupInfo == m_arrGroupInfo.GetAt(i)->GetGroupInfo())
				break;
		}
		else if(m_nAcqType == OTHER_ACQ_TYPE)
		{
			if(strAcqYear == m_arrGroupInfo.GetAt(i)->GetAcqYear() &&
				strGroupInfo == m_arrGroupInfo.GetAt(i)->GetGroupInfo())
				break;
		}
	}	
	if(i == cnt) i = 0;
	
	SetCurSel(i);
	if(m_nAcqType == PURCHASE)
		m_arrGroupInfo.GetAt(i)->SetCurSelWorkNo(strWorkNo);

	return 0;
}

INT CGroupCollection::SetCurSelWorkNo(INT idx)
{
	CGroupInfo * pGroup = GetCurSelGroupInfo();
	pGroup->SetIndex(idx);
	MakeSerialNo(idx);
	return 0;
}

CString CGroupCollection::GetAcqYear()
{
	if(m_arrGroupInfo.GetSize() == 0) return _T("");
	if(m_nIdx < 0) m_nIdx = 0;
	return m_arrGroupInfo.GetAt(m_nIdx)->GetAcqYear();
}

INT CGroupCollection::UpDateLastSerialNo(CESL_DataMgr *pDM_OUT, INT nNum)
{
	CString strQuery;

	switch( m_nAcqType )
	{
	case PURCHASE:
	{
		CString strTmpData = m_arrGroupInfo.GetAt(m_nIdx)->GetWorkNo();
			CString strWorkCode = m_arrGroupInfo.GetAt(m_nIdx)->GetWorkCode();
			
			if( strTmpData.IsEmpty() == TRUE )	return -1;			
		
			if( strWorkCode.IsEmpty() )
			{
				strQuery.Format(_T("UPDATE BO_PURCHASE_WORK_NO_TBL SET ")
								_T("LAST_SERIAL_NO = %d, ")
								_T("LAST_WORK = '%s' ")
								_T("WHERE WORK_NO = %s ")
								_T("AND PURCHASE_SEQ_NO_KEY IN ")
								_T("(SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE IS NULL);"),
				nNum, 
				m_pMgr->GetWorkLogMsg(WORK_LOG),
				strTmpData,
				m_arrGroupInfo.GetAt(m_nIdx)->GetAcqYear(),
								m_arrGroupInfo.GetAt(m_nIdx)->GetGroupInfo()	);
			}
		else
			{
				strQuery.Format(_T("UPDATE BO_PURCHASE_WORK_NO_TBL SET ")
								_T("LAST_SERIAL_NO = %d, ")
								_T("LAST_WORK = '%s' ")
								_T("WHERE WORK_NO = %s ")
								_T("AND PURCHASE_SEQ_NO_KEY IN ")
								_T("(SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR = '%s' AND SEQ_NO = %s AND WORK_CODE = '%s');"),
				nNum, 
				m_pMgr->GetWorkLogMsg(WORK_LOG),
				strTmpData,
				m_arrGroupInfo.GetAt(m_nIdx)->GetAcqYear(),
				m_arrGroupInfo.GetAt(m_nIdx)->GetGroupInfo(),
								m_arrGroupInfo.GetAt(m_nIdx)->GetWorkCode()			);
	}
		}
		break;

	case ACQ_TYPE_INVALID:
	{
			strQuery.Format(_T("UPDATE BO_ACQ_GROUP_TBL SET ")
							_T("GROUP_LAST_SERIAL_NO = %d, ")
							_T("LAST_WORK = '%s' ")
							_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_SEQ = %s AND CREATE_YEAR = '%s';"),

			nNum, 
			m_pMgr->GetWorkLogMsg(WORK_LOG),
			m_arrGroupInfo.GetAt(m_nIdx)->GetGroupNo(),
							m_arrGroupInfo.GetAt(m_nIdx)->GetAcqYear()		);		
	}
		break;
	case DONATE:
	{
			strQuery.Format(_T("UPDATE BO_DON_RECEIPT_TBL SET ")
							_T("LAST_SERIAL_NO = %d, ")
							_T("LAST_WORK = '%s' ")
							_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND RECEIPT_NO = %s AND ACQ_YEAR = '%s';"), 
			nNum, 
			m_pMgr->GetWorkLogMsg(WORK_LOG),
			m_arrGroupInfo.GetAt(m_nIdx)->GetGroupInfo(),
							m_arrGroupInfo.GetAt(m_nIdx)->GetAcqYear()		);
		}
		break;
	}

	pDM_OUT->AddFrame(strQuery);
	return 0;
}

INT CGroupCollection::InsertWorkNo(CESL_DataMgr * pDM_OUT)
{
	CGroupMgr m_GroupMgr(m_pMgr);
	m_GroupMgr.InitTempData();
	m_GroupMgr.SetTempData(_T("수입년도"), m_arrGroupInfo.GetAt(m_nIdx)->GetAcqYear().GetBuffer(0));
	m_GroupMgr.SetTempData(_T("차수번호"), m_arrGroupInfo.GetAt(m_nIdx)->GetGroupInfo().GetBuffer(0));
	m_GroupMgr.SetTempData(_T("업무구분"), m_arrGroupInfo.GetAt(m_nIdx)->GetWorkCode().GetBuffer(0));
	m_GroupMgr.SetTempPtr(_T("OUT_DM"), (DWORD*)pDM_OUT);
	INT ids = m_GroupMgr.SPFExecutePath(_T("작업번호생성"));
	if(ids < 0) return -1;
	CString strTmpData;
	strTmpData.Format(_T("%d"), ids);
	AddWorkNoInfo(strTmpData);
	return ids;
}

BOOL CGroupCollection::IsEmpty()
{
	if(m_arrGroupInfo.GetSize() == 0) 
		return TRUE;
	return FALSE;
}

INT CGroupCollection::GetGroupCount()
{
	return m_arrGroupInfo.GetSize();
}

INT CGroupCollection::GetDetailGroupInfo(CStringArray * pGroupInfo)
{
	INT cnt = m_arrGroupInfo.GetSize();
	CString strTmpData;
	CGroupInfo * pGroup = NULL;
	pGroupInfo->RemoveAll();
	for(INT i=0 ; i<cnt ; i++)
	{
		pGroup = m_arrGroupInfo.GetAt(i);
		strTmpData.Format(_T("%s년-%s"), pGroup->GetAcqYear(), pGroup->GetGroupInfo());
		pGroupInfo->Add(strTmpData);
	}
	return 0;
}

INT CGroupCollection::GetGroupDataCount(BOOL bSpecies, CESL_DataMgr * pDM)
{
	CGroupMgr m_GroupMgr(m_pMgr);
	CGroupInfo * pGroup = m_arrGroupInfo.GetAt(m_nIdx);
	m_GroupMgr.InitTempData();
	m_GroupMgr.SetTempData(_T("ACQ_YEAR"), pGroup->GetAcqYear().GetBuffer(0));
	m_GroupMgr.SetTempData(_T("GROUP_NO"), pGroup->GetGroupInfo().GetBuffer(0));
	m_GroupMgr.SetTempData(_T("SPECIES_YN"), bSpecies);
	m_GroupMgr.SetTempData(_T("ACQ_TYPE"), (INT)m_nAcqType);
	m_GroupMgr.SetTempPtr(_T("DM"), (DWORD*)pDM);
	INT ids = m_GroupMgr.SPFExecutePath(_T("그룹자료수구하기"));
	return ids;
}

INT CGroupCollection::UpdateDupSearchCompleteYN(BOOL bCheckYN, CESL_DataMgr * pDM)
{
	INT cnt = m_arrGroupInfo.GetSize();
	CGroupInfo * pGroup = NULL;
	CString strQuery, strWorkCode;
	CString strData;
	if(bCheckYN) strData = _T("Y");
	else strData = _T("N");

	pDM->StartFrame();
	for(INT i=0 ; i<cnt ; i++)
	{
		pGroup = m_arrGroupInfo.GetAt(i);
		strWorkCode = pGroup->GetWorkCode();
		if(strWorkCode.IsEmpty())
			strQuery.Format(
				_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET DUP_CHECK_YN='%s', LAST_WORK='%s' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE IS NULL;"),
				strData, m_pMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetAcqYear(), pGroup->GetGroupInfo());
		else
			strQuery.Format(
				_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET DUP_CHECK_YN='%s', LAST_WORK='%s' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO=%s AND WORK_CODE='%s';"),
				strData, m_pMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetAcqYear(), pGroup->GetGroupInfo(), strWorkCode);
		pDM->AddFrame(strQuery);
		strQuery.Empty();
	}
	INT ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;

	return ids;
}

INT CGroupCollection::UpdateCopySearchCompleteYN(BOOL bCheckYN, CESL_DataMgr * pDM)
{
	INT cnt = m_arrGroupInfo.GetSize();
	CGroupInfo * pGroup = NULL;
	CString strQuery;
	CString strData;
	if(bCheckYN) strData = _T("Y");
	else strData = _T("N");

	pDM->StartFrame();
	for(INT i=0 ; i<cnt ; i++)
	{
		pGroup = m_arrGroupInfo.GetAt(i);
		strQuery.Format(
			_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET COPY_CHECK_YN='%s', LAST_WORK='%s' WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_YEAR='%s' AND SEQ_NO=%s;"),
			strData, m_pMgr->GetWorkLogMsg(WORK_LOG), pGroup->GetAcqYear(), pGroup->GetGroupInfo());
		pDM->AddFrame(strQuery);
		strQuery.Empty();
	}
	INT ids = pDM->SendFrame();
	if(ids < 0) return -1;
	ids = pDM->EndFrame();
	if(ids < 0) return -1;

	return ids;
}

INT CGroupCollection::CopyData(CGroupCollection *pGroupCollection)
{
	if(pGroupCollection == NULL) return -1;

	pGroupCollection->RemoveAll();
	pGroupCollection->SetWorkingStatus(m_strWorkStatus);
	pGroupCollection->SetAcqType(m_nAcqType);
	pGroupCollection->SetHaveGroupInfo(m_bHaveGroupInfo);
	INT cnt = m_arrGroupInfo.GetSize();
	CGroupInfo * pGroup = NULL;
	
	for(INT i=0 ; i<cnt ; i++)
	{
		pGroup = m_arrGroupInfo.GetAt(i);
		CGroupInfo pTargetGroup;
		pGroup->CopyData(&pTargetGroup);
		pGroupCollection->AddGroupInfo(&pTargetGroup);
	}
	if(m_nIdx >= 0)
		pGroupCollection->SetCurSel(m_nIdx);
	else pGroupCollection->SetCurSel(0);
	return 0;
}

CGroupInfo * CGroupCollection::GetGroupInfo(INT idx)
{
	return m_arrGroupInfo.GetAt(idx);
}

BOOL CGroupCollection::IsHaveGroupInfo(CGroupInfo *pGroup)
{
	INT cnt = m_arrGroupInfo.GetSize();
	CString strAcqYear = pGroup->GetAcqYear();
	CString strGroupNo, strTmpAcqYear, strTmpGroupNo;
	if(m_nAcqType == ACQ_TYPE_INVALID) strGroupNo = pGroup->GetGroupNo();
	else strGroupNo = pGroup->GetGroupInfo();

	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpAcqYear = m_arrGroupInfo.GetAt(i)->GetAcqYear();
		if(m_nAcqType == ACQ_TYPE_INVALID) strTmpGroupNo = m_arrGroupInfo.GetAt(i)->GetGroupNo();
		else strTmpGroupNo = m_arrGroupInfo.GetAt(i)->GetGroupInfo();
		if(strTmpAcqYear == strAcqYear && strTmpGroupNo == strGroupNo)
			return TRUE;
	}
	return FALSE;
}

INT CGroupCollection::DeleteThisGroupInfo(CGroupCollection *pGroupCollection)
{
	INT ids = -1;
	INT cnt = pGroupCollection->GetGroupCount();
	INT nSourceCnt = m_arrGroupInfo.GetSize();
	if(cnt == 0) return 0 ;
	CGroupInfo * pGroupInfo = NULL;
	for(INT i=0 ; i<cnt ; i++)
	{
		pGroupInfo = pGroupCollection->GetGroupInfo(i);
		for(INT j=0 ; j<nSourceCnt ; j++)
		{
			if(m_arrGroupInfo.GetAt(j)->IsEqual(pGroupInfo)) 
			{
				RemoveAt(j);
				break;
			}
		}
		nSourceCnt = m_arrGroupInfo.GetSize();
	}

	if(m_arrGroupInfo.GetSize() == 0) m_nIdx = -1;
	else m_nIdx = 0;
	
	return 0;
}

BOOL CGroupCollection::IsIncludeGroupInfo(CString strAcqYear, CString strGroupInfo, CString strWorkCode/*=_T("")*/)
{
	CGroupInfo * pGroup = NULL;
	INT cnt = m_arrGroupInfo.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		pGroup = m_arrGroupInfo.GetAt(i);
		if(pGroup->IsEqual(strAcqYear, strGroupInfo, strWorkCode))
			return TRUE;
	}
	return FALSE;
}

ACQ_TYPE CGroupCollection::GetAcqType()
{
	return m_nAcqType;
}

CString CGroupCollection::CheckValidate(CString strGroupInfo, CString strAcqYear, ACQ_TYPE nAcqType, CString strWorkCode/*=_T("")*/)
{
	INT nInitCnt = -1;

	CStringArray arrDivNo;		arrDivNo.RemoveAll();
	ParsingString(_T(","), strGroupInfo, &arrDivNo);
	
	INT cnt = arrDivNo.GetSize();
	nInitCnt = cnt;

	INT ids = -1;

	CString strMsg;
	for(INT i=0 ; i<cnt && cnt>0; i++)
	{
		ids = CheckValidate(arrDivNo.GetAt(i), strAcqYear, strWorkCode);
		if(ids < 0)
		{
			if(nAcqType == PURCHASE)
				strMsg.Format(_T("입력하신 차수번호 [%s]는 사용할 수 없습니다."), arrDivNo.GetAt(i));
			else if(nAcqType == DONATE)
				strMsg.Format(_T("입력하신 접수번호 [%s]는 사용할 수 없습니다."), arrDivNo.GetAt(i));
			else if(nAcqType == ACQ_TYPE_INVALID)
				strMsg.Format(_T("입력하신 그룹명 [%s]는 사용할 수 없습니다."), arrDivNo.GetAt(i));

			m_pMgr->ESLAfxMessageBox(strMsg);
			arrDivNo.RemoveAt(i);
			i--;
			cnt--;
		}
	}
	if(nInitCnt != cnt)
	{
		CString strTmpData;		strTmpData.Empty();
		for(i=0 ; i<cnt ; i++)
		{
			if(strTmpData.IsEmpty()) strTmpData = arrDivNo.GetAt(i);
			else strTmpData += _T(", ") + arrDivNo.GetAt(i);
		}
		return strTmpData;
	}

	return strGroupInfo;
}

INT CGroupCollection::DeleteThisGroupInfo(CGroupInfo *pGroup)
{
	INT ids = -1;
	INT nCnt = m_arrGroupInfo.GetSize();
	if(nCnt == 0) return 0 ;
	for(INT i=0 ; i<nCnt ; i++)
	{
		if(m_arrGroupInfo.GetAt(i)->IsEqual(pGroup)) 
		{
			RemoveAt(i);
			break;
		}
	}
	
	if(m_arrGroupInfo.GetSize() == 0) m_nIdx = -1;
	else m_nIdx = 0;
	
	return 0;
}

BOOL CGroupCollection::IsSameWorkCode(CString strWorkCode)
{
	INT cnt = m_arrGroupInfo.GetSize();
	if(cnt == 0) return TRUE;
	CGroupInfo * pGroup = NULL;
	for(INT i=0 ; i<cnt ; i++)
	{
		pGroup = m_arrGroupInfo.GetAt(i);
		if(pGroup->GetWorkCode() != strWorkCode)
			return FALSE;
	}
	return TRUE;
}

CString CGroupCollection::GetWorkCode()
{
	INT cnt = m_arrGroupInfo.GetSize();
	if(cnt == 0) return _T("");
	return ((CGroupInfo*)m_arrGroupInfo.GetAt(0))->GetWorkCode();
}

INT CGroupCollection::GetBookTotalInfo(CESL_DataMgr *pDM, CString &strTotalPrice, CString &strTotalBookCnt)
{
	INT i, cnt, ids;
	CString strQuery, strWhere, strWorkingStatus;	
	CGroupInfo* pGroup = NULL;
	
	cnt = m_arrGroupInfo.GetSize();
	if( cnt == 0 )
	{
		strTotalPrice = _T("0");
		strTotalBookCnt = _T("0");
		return 0;
	}

	CString strRecKeyQuery;
	strRecKeyQuery = m_strSelectBookKey;
	ConvertRecKeyToSubQuery(strRecKeyQuery, _T("BO_BOOK_TBL"), _T("REC_KEY"));
	CString strQueryWhere;
	strQueryWhere = _T("B.REC_KEY IN( ") + strRecKeyQuery +_T(" )");

	strQueryWhere.TrimLeft();
	strQueryWhere.TrimRight();
	if(0 > strQueryWhere.Find(_T("MANAGE_CODE = UDF_MANAGE_CODE")))
	{
		strQueryWhere = _T("MANAGE_CODE = UDF_MANAGE_CODE AND ") + strQueryWhere;
	}

	switch( m_nAcqType )
	{
	case ACQ_TYPE_INVALID:
		{
			strQuery.Format(_T("SELECT COUNT(*), GET_ORDERPRICE(SUM(GetPriceWon(B.ACQ_KEY, B.PRICE, 'Y', UDF_MANAGE_CODE)), UDF_MANAGE_CODE) ")
					_T("FROM BO_BOOK_TBL B ")
					_T("WHERE B.%s AND "), strQueryWhere );
		}
		break;
	case PURCHASE:
		{
			strQuery.Format(_T("SELECT COUNT(B.REC_KEY), GET_ORDERPRICE(SUM(GetPriceWon(B.ACQ_KEY, B.PRICE, 'Y', UDF_MANAGE_CODE)), UDF_MANAGE_CODE) ")
					_T("FROM IDX_BO_TBL I, BO_BOOK_TBL B ")
					_T("WHERE I.REC_KEY=B.SPECIES_KEY ")
					_T("AND I.%s AND "), strQueryWhere );
		}
		break;
	case DONATE:
		{
			strQuery.Format(_T("SELECT COUNT(*), GET_ORDERPRICE(SUM(GetPriceWon(B.ACQ_KEY, B.PRICE, 'Y', UDF_MANAGE_CODE)), UDF_MANAGE_CODE) ")
					_T("FROM BO_BOOK_TBL B ")
					_T("WHERE B.%s AND "), strQueryWhere );
		}
		break;
	default:
		break;
	}

	pGroup = m_arrGroupInfo.GetAt(0);
	if( pGroup != NULL )	strWhere = pGroup->GetBookGroupQuery((INT)m_nAcqType);
	for( i=1 ; i<cnt ; i++ )
	{
		pGroup = m_arrGroupInfo.GetAt(i);
		if( pGroup == NULL )	continue;
		strWhere += _T(" OR ") + pGroup->GetBookGroupQuery((INT)m_nAcqType);
	}
	strQuery += _T("( ") + strWhere + _T(" )");

	if( m_strWorkStatus.IsEmpty() == FALSE )
	{
		if		( m_strWorkStatus == _T("구입대상") )	strQuery += _T(" AND B.WORKING_STATUS = 'BOA111N' ");
		else if	( m_strWorkStatus == _T("주문")		)	strQuery += _T(" AND B.WORKING_STATUS = 'BOA112N' ");
	}

	ids = pDM->RestructDataManager(strQuery);
	if(ids < 0) return ids;

	strTotalBookCnt = pDM->GetCellData(0, 0);
	strTotalPrice = pDM->GetCellData(0, 1);

	return 0;
}

INT CGroupCollection::GetDeliveryRelativeBookCnt(CESL_DataMgr *pDM, CString &strDeliveryBookCnt, CString &strNonDeliveryBookCnt)
{
	INT cnt = m_arrGroupInfo.GetSize();
	if(cnt == 0) 
	{
		strDeliveryBookCnt = _T("0");
		strNonDeliveryBookCnt = _T("0");
	}
	CStringArray arrBookGroupQuery;
	arrBookGroupQuery.RemoveAll();
	CGroupInfo * pGroup = NULL;
	for(INT i=0 ; i<cnt ; i++)
	{
		pGroup = m_arrGroupInfo.GetAt(i);
		if(pGroup == NULL) continue;
		arrBookGroupQuery.Add(pGroup->GetBookGroupQuery((INT)m_nAcqType));
	}

	CString strQuery, strWhere;

	CString strRecKeyQuery;
	strRecKeyQuery = m_strSelectBookKey;
	ConvertRecKeyToSubQuery(strRecKeyQuery, _T("BO_BOOK_TBL"), _T("REC_KEY"));

	if(m_nAcqType == ACQ_TYPE_INVALID)
		strQuery.Format(_T("SELECT COUNT(*) FROM BO_BOOK_TBL B WHERE DELIVERY_YN='Y' AND REC_KEY IN (%s) AND "), strRecKeyQuery );
	else if(m_nAcqType == PURCHASE)
		strQuery.Format(_T("SELECT COUNT(B.REC_KEY) FROM IDX_BO_TBL I, BO_BOOK_TBL B WHERE I.REC_KEY=B.SPECIES_KEY AND B.DELIVERY_YN='Y' AND B.REC_KEY IN (%s) AND "), strRecKeyQuery);
	else if(m_nAcqType == DONATE)
		strQuery.Format(_T("SELECT COUNT(*) FROM BO_BOOK_TBL B WHERE DELIVERY_YN='Y' AND B.REC_KEY IN (%s) AND"), strRecKeyQuery);
	
	cnt = arrBookGroupQuery.GetSize();
	for(i=0 ; i<cnt ; i++)
	{
		if(strWhere.IsEmpty()) strWhere = arrBookGroupQuery.GetAt(i);
		else strWhere += _T(" OR ") + arrBookGroupQuery.GetAt(i);
	}
	strQuery += _T("( ") + strWhere + _T(" )");
	INT ids = pDM->RestructDataManager(strQuery);	
	if(ids < 0) return ids;

	strDeliveryBookCnt = pDM->GetCellData(0, 0);

	if(m_nAcqType == ACQ_TYPE_INVALID)
		strQuery.Format(_T("SELECT COUNT(*) FROM BO_BOOK_TBL B WHERE DELIVERY_YN='N' AND B.REC_KEY IN (%s) AND"), strRecKeyQuery);
	else if(m_nAcqType == PURCHASE)
		strQuery.Format(_T("SELECT COUNT(B.REC_KEY) FROM IDX_BO_TBL I, BO_BOOK_TBL B WHERE I.REC_KEY=B.SPECIES_KEY AND B.DELIVERY_YN='N' AND B.REC_KEY IN (%s) AND"), strRecKeyQuery);
	else if(m_nAcqType == DONATE)
		strQuery.Format(_T("SELECT COUNT(*) FROM BO_BOOK_TBL B WHERE DELIVERY_YN='N' AND B.REC_KEY IN (%s) AND"), strRecKeyQuery);

	strQuery += _T("( ") + strWhere + _T(" )");
	ids = pDM->RestructDataManager(strQuery);	
	if(ids < 0) return ids;

	strNonDeliveryBookCnt = pDM->GetCellData(0, 0);

	return 0;
}	

INT CGroupCollection::MakeGroupInfoByRFID(CString strRFIDSerial, CString strConnectionInfo/*=_T("")*/)
{
	CString strAcqYear, strGroupInfo, strWorkCode, strQuery;
	if(strRFIDSerial.IsEmpty())
	{
		m_pMgr->ESLAfxMessageBox(_T("검색할 RFID정보가 올바르지 않습니다."));
		return -1;
	}
	RemoveAll();
	
	CESL_DataMgr tmpDM;
	if(strConnectionInfo.IsEmpty()) tmpDM.SetCONNECTION_INFO(m_pMgr->CONNECTION_INFO);
	else tmpDM.SetCONNECTION_INFO(strConnectionInfo);


	INT ids = -1;
	if(m_nAcqType == PURCHASE)
	{
		strQuery.Format(
			_T("SELECT P.ORDER_YEAR, P.ORDER_SEQ_NO, I.WORK_CODE FROM IDX_BO_TBL I, BO_PURCHASEINFO_TBL P WHERE I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY IN (SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE WORKNO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE)"),
			strRFIDSerial);
		tmpDM.RestructDataManager(strQuery);
		strAcqYear = tmpDM.GetCellData(0, 0);
		strGroupInfo = tmpDM.GetCellData(0, 1);
		strWorkCode = tmpDM.GetCellData(0, 2);
		ids = MakeDivNoInfo(m_strWorkStatus, strAcqYear, strGroupInfo, strWorkCode);
	}
	else if(m_nAcqType == DONATE)
	{
		strQuery.Format(
			_T("SELECT ACQ_YEAR, RECEIPT_NO FROM BO_DONINFO_TBL WHERE REC_KEY IN (SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE WORKNO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE)"),
			strRFIDSerial);
		tmpDM.RestructDataManager(strQuery);
		strAcqYear = tmpDM.GetCellData(0, 0);
		strGroupInfo = tmpDM.GetCellData(0, 1);
		ids = MakeReceiptNoInfo(strAcqYear, strGroupInfo);
	}
	if(ids < 0) return ids ;

	m_nIdx = 0;
	if(m_nAcqType == PURCHASE)
		MakeWorkNoInfo();

	if(m_arrGroupInfo.GetSize() != 0)
	{
		m_bHaveGroupInfo = TRUE;
	}
	return 0;
}

INT CGroupCollection::MakeOtherGroupNoInfo(CString strAcqCode, CString strAcqYear, CString strGroupInfo)
{
	CESL_DataMgr DM;
	CGroupMgr GroupMgr(m_pMgr);
	DM.SetCONNECTION_INFO(m_pMgr->CONNECTION_INFO);
	DM.InitDMPKFromFile(_T("DM_수서그룹번호관리"));
	DM.SetMarcMgr(m_pMgr->m_pInfo->pMarcMgr);
	
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	INT col, nCols;
	nCols = DM.m_nCols;
	if(nCols < 0) nCols = DM.GetRefColumnCount();
	for(col = 0; col < nCols; col++)
	{
		if (DM.GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0) continue;
		if(FIELD_NAME.Find(_T("(")) >= 0)
		{
			DM.SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, _T("udf"));
		}
	}
	
	CString strQuery, strOption;
	
	CString strGroupInfoAlias[3];
	strGroupInfoAlias[0] = _T("그룹번호");
	strGroupInfoAlias[1] = _T("수입년도");
	strGroupInfoAlias[2] = _T("마지막일련번호");

	strQuery.Format(_T("CREATE_YEAR='%s' AND ACQ_CODE='%s'"), strAcqYear, strAcqCode);
	if(!strGroupInfo.IsEmpty() && strGroupInfo != _T("적용안함"))
	{
		strOption.Format(_T(" AND GROUP_SEQ IN (%s)"), strGroupInfo);
		strQuery += strOption;
	}
	
	INT ids = DM.RefreshDataManager(strQuery);
	if(ids < 0) return -1;
	
	CString strGroupInfoData[3];
	
	INT nRowCnt = DM.GetRowCount();
	
	INT j = -1;
	CGroupInfo * pGroup = NULL;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		DM.GetCellData(strGroupInfoAlias, 3, strGroupInfoData, i);
		
		pGroup = new CGroupInfo();
		pGroup->SetGroupInfo(strGroupInfoData[0]);
		pGroup->SetAcqYear(strGroupInfoData[1]);
		pGroup->SetLastSerialNo(_ttoi(strGroupInfoData[2].GetBuffer(0)));
		m_arrGroupInfo.Add(pGroup);
	}
	return 0;
}

INT CGroupCollection::CheckAcqNo(CString strAcqYear, CString strAcqCode, CString strWorkCode)
{
	CString strQuery;
	CString strWhere;
	CString strTemp;
	CString strRecKey;
	CESL_DataMgr tmpDM;
	CString strConnectionInfo;
	if(strConnectionInfo.IsEmpty()) tmpDM.SetCONNECTION_INFO(m_pMgr->CONNECTION_INFO);
	else tmpDM.SetCONNECTION_INFO(strConnectionInfo);

	strWhere.Format( _T("") );
	if ( !strAcqYear.IsEmpty() )
	{
		strWhere.Format(_T("ACQ_YEAR = '%s'"),strAcqYear);
	}

	if ( !strAcqCode.IsEmpty() && !strWhere.IsEmpty() )
	{
		strTemp = strWhere;
		strWhere.Format( _T("%s AND SEQ_NO IN( %s )"), strTemp, strAcqCode );
	}
	else if( !strAcqCode.IsEmpty() )
	{
		strWhere.Format(_T("SEQ_NO IN( %s )"),strAcqCode);
	}
	
	if ( !strWorkCode.IsEmpty() && !strWhere.IsEmpty() )
	{
		strTemp = strWhere;
		strWhere.Format(_T("%s AND WORK_CODE = '%s'"), strTemp,strWorkCode);
	}
	else if ( !strWorkCode.IsEmpty() )
	{
		strWhere.Format(_T("WORK_CODE = '%s'"),strAcqCode);
	}

	if( !strWhere.IsEmpty() )
	{
		strWhere = _T("MANAGE_CODE = UDF_MANAGE_CODE AND ") + strWhere;
		strQuery.Format(_T("SELECT REC_KEY FROM BO_PURCHASE_SEQ_NO_TBL WHERE %s"),strWhere);
	}
	else
	{
		return 0;
	}
	tmpDM.GetOneValueQuery( strQuery, strRecKey );
	
	if( strRecKey.IsEmpty()  )
	{
		return -1;
	}
	return 0;
}

INT CGroupCollection::ConvertRecKeyToSubQuery(CString &strRecKeyList, CString strTableName, CString strKeyName)
{
	
	CString strTemp;
 	CString strRecKey;
 	CString strBookKeyTempList;
 	CString strQueryWhere;
 	INT nCnt;
 	INT nLocation;
 	INT nStartLocation;
	INT i ; 
	strBookKeyTempList = strRecKeyList;
	strBookKeyTempList.Replace(_T("'"), _T(""));
	nCnt = 0;
	nLocation = 0;
	strQueryWhere.Format(_T(""));
	while( 1 )
	{
		nLocation = strBookKeyTempList.Find(_T(","), nLocation+1);
		++nCnt;
		if( 0 > nLocation )
		{
			break;			
		}
	}
	if ( (nCnt % 1000) == 0 )
	{
		nCnt = (nCnt/1000);
	}
	else
	{
		nCnt = (nCnt/1000)+1;
	}
	nStartLocation = strBookKeyTempList.GetLength()/nCnt;

	for( i = 0 ; i < nCnt ; i++)
	{
		if( i == (nCnt -1) )
		{
			strRecKey = strBookKeyTempList;
		}
		else
		{
			nLocation = strBookKeyTempList.Find(_T(","), nStartLocation);
			strRecKey = strBookKeyTempList.Mid(0, nLocation );
			strTemp.Format(_T("%s,"), strRecKey);
			strBookKeyTempList.Replace( strTemp, _T("") );
		}
	
		if( strQueryWhere.IsEmpty() )
		{
			strQueryWhere.Format(_T("%s IN (%s)"), strKeyName, strRecKey);
		}
		else
		{
			strTemp.Format(_T("%s OR %s IN( %s )"), strQueryWhere,strKeyName, strRecKey );
			strQueryWhere = strTemp;
		}
	}
	strRecKeyList.Format(_T("SELECT %s FROM %s WHERE %s"), strKeyName, strTableName,strQueryWhere);

	return 0;
}
