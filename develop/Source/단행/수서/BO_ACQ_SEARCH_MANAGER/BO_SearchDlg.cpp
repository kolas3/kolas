// BO_SearchDlg.cpp: implementation of the CBO_SearchDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_SearchDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_SearchDlg::CBO_SearchDlg(UINT IDD, CESL_Mgr * pParentMgr):CESL_Mgr(IDD, pParentMgr)
{
	m_nDialogID = IDD;
	m_bCreate = FALSE;
	m_nOpenMode = SEARCH_TYPE_INVALID;
	m_nAcqCode = ACQ_TYPE_INVALID;
	m_strWhereStmt = _T("");
	m_pGroups = new CGroupCollection(this);
	pMakeSearchData = new CMakeSearchData(pParentMgr);
	m_bDataSortYN = FALSE;
	m_pSortItem = NULL;

	// openbase flag	
	CFileFind ff;
	if ( ff.FindFile(_T("..\\cfg\\_OPENBASE")) )
	{
		m_bOpenbase = TRUE;
	}
	else
	{
		m_bOpenbase = FALSE;
	}
}

CBO_SearchDlg::~CBO_SearchDlg()
{	
	if(m_pGroups)
	{
		delete m_pGroups;
		m_pGroups = NULL;
	}
	if(pMakeSearchData)
	{
		delete pMakeSearchData;
		pMakeSearchData = NULL; 
	}
}

BOOL CBO_SearchDlg::CreateSearchDlg(CWnd *pParentWnd)
{
	m_bCreate = TRUE;
	return CDialog::Create(m_nDialogID, pParentWnd);
}

CString CBO_SearchDlg::MakeDateCondition_Stmt(CString fieldName, CString m_Date1, CString m_Date2)
{
	CString strOption, strTmpData;
	if(!m_Date2.IsEmpty() && m_Date1.Compare(m_Date2) > 0)
	{
		strTmpData = m_Date1;
		m_Date1 = m_Date2;
		m_Date2 = strTmpData;
	}
	
	if(m_Date1.IsEmpty() && !(m_Date2.IsEmpty()))
	{
		ESLAfxMessageBox(_T("검색 시작일을 입력하십시요."));
		return _T("-1");
	}
	else if(!(m_Date1.IsEmpty()) && m_Date2.IsEmpty())
	{
		//strOption.Format(_T(" AND %s = '%s'"), fieldName, m_Date1);
		//20070615 UPDATE PJW : 검색조건에 (시:분:초)[00:00:00~23:59:59] 추가
		strOption.Format(_T(" AND %s BETWEEN to_date('%s','YYYY/MM/DD') AND to_date('%s', 'YYYY/MM/DD')+0.99999"), fieldName, m_Date1, m_Date1);
	}
	else if(!(m_Date1.IsEmpty()) && !(m_Date2.IsEmpty()))
	{
		/*
		if(m_Date1 == m_Date2)
			strOption.Format(_T(" AND %s = '%s'"), fieldName, m_Date1);
		else
			strOption.Format(_T(" AND %s >= '%s' AND %s <= '%s'"), fieldName, m_Date1, fieldName, m_Date2);
		*/
		//20070615 UPDATE PJW : 검색조건에 (시:분:초)[00:00:00~23:59:59] 추가
		strOption.Format(_T(" AND %s BETWEEN to_date('%s','YYYY/MM/DD') AND to_date('%s', 'YYYY/MM/DD')+0.99999"), fieldName, m_Date1, m_Date2);
	}
	return strOption;
}

INT CBO_SearchDlg::SetSubjectCodeData(CString strCMAlias, CString strClassNoType, CString strSubjectCode)
{
 	CMulitComboBoxManager mgr(this);
	INT ids = mgr.SetSubjectCodeData(strCMAlias, strClassNoType, strSubjectCode, _T(""));
	if(ids < 0) return -1;
	return 0;
}

VOID CBO_SearchDlg::TrimAll(CString &strData)
{
	if(strData.IsEmpty()) return;
	strData.TrimLeft();
	strData.TrimRight();
}


INT CBO_SearchDlg::ParsingString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray)
{
	if(strSource.GetLength() == 0) return 0;
	INT pos = strSource.Find(DelimeterStr, 0);
	CString strTmpData;
	while(pos > 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+1);
		TrimAll(strTmpData);
//		pMakeSearchData->GetKeyWord(strTmpData);
		if(!CheckIsHave(pTargetArray, strTmpData))
			pTargetArray->Add(strTmpData);
		pos = strSource.Find(DelimeterStr, 0);
	}
//	pMakeSearchData->GetKeyWord(strSource);
	TrimAll(strSource);
	if(!CheckIsHave(pTargetArray, strSource))
		pTargetArray->Add(strSource);
	return 0;
}

BOOL CBO_SearchDlg::CheckIsHave(CStringArray *codeValue, CString code)
{
	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
	return FALSE;
}

CString CBO_SearchDlg::MakeGroupInfoOrQuery(CString strGroupName)
{
	CString strQuery, strTmpData, strFieldName, strData;
	if(GetAcqCode() == PURCHASE) strFieldName = _T("P.ORDER_SEQ_NO");
	else if(GetAcqCode() == DONATE) strFieldName = _T("P.RECEIPT_NO");
	else if(GetAcqCode() == ACQ_TYPE_INVALID) strFieldName = _T("GROUP_NAME");
	CStringArray arrGroupName;		arrGroupName.RemoveAll();
	INT ids = ParsingString(_T(","), strGroupName, &arrGroupName);
	INT cnt = arrGroupName.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(GetAcqCode() == ACQ_TYPE_INVALID)
		{
			strData = arrGroupName.GetAt(i);
			strTmpData.Format(_T("%s = '%s'"), strFieldName, strData);
		}
		else 
			strTmpData.Format(_T("%s = %s"), strFieldName, arrGroupName.GetAt(i));
		if(strQuery.IsEmpty()) strQuery = strTmpData;
		else strQuery += _T(" OR ") + strTmpData;
	}
	return strQuery;
}

CString CBO_SearchDlg::GetTodayYear(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;
}

BOOL CheckIsDigit(CString str)
{
	if(str.IsEmpty()) return FALSE;

	TCHAR * tmp = str.GetBuffer(0);
	INT size = str.GetLength();
	for(INT i=0 ; i<size ; i++)
	{
		if(tmp[i] < '0' || tmp[i] > '9') 
			return FALSE;
	}
	return TRUE;
}

CString CBO_SearchDlg::CheckValidateGroupNo(CESL_ControlMgr * pCM, CString strGroupInfo, CString strAcqYear, CString strWorkCode/*=_T("")*/)
{
	Sleep(500);
	INT acqCode = GetAcqCode();
	INT nInitCnt = -1;

	CStringArray arrDivNo;		arrDivNo.RemoveAll();
	ParsingString(_T(","), strGroupInfo, &arrDivNo);
	
	INT cnt = arrDivNo.GetSize();
	nInitCnt = cnt;

	INT ids = -1;

	CString strMsg, strData;

	//====================================
	//20080603 ADD BY PJW : 차수번호를 체크한다.
	//20080603 ADD BY PJW : 차수번호를 체크한다.
	//2009.09.16 UPDATE BY PJW : BO_ACQ_NONDONATE 추가 
	if( (!strGroupInfo.IsEmpty()) && 
		(m_nOpenMode != BO_ACQ_IMPORT) && 
		(m_nOpenMode != BO_ACQ_BASIC) && 
		(m_nOpenMode != BO_ACQ_RECYCLE)&& 
		(m_nOpenMode != BO_ACQ_GROUP) &&
		(m_nOpenMode != BO_ACQ_DONATE) &&
		(m_nOpenMode != BO_ACQ_NONDONATE))
	{
		ids = m_pGroups->CheckAcqNo(strAcqYear, strGroupInfo , strWorkCode);
		if( ids < 0  )
		{
			AfxMessageBox(_T("입력하신 차수번호가 올바르지 않습니다."));
			strGroupInfo.Format( _T("") );
		}
	}
	//====================================

	for(INT i=0 ; i<cnt && cnt>0; i++)
	{
		strData = arrDivNo.GetAt(i);
		if(acqCode == 1 || acqCode == 2)
		{
			if(!pMakeSearchData->CheckDigit(strData))
			{
				strMsg.Format(_T("입력하신 [%s]정보는 사용할 수 없습니다."), strData);
				ESLAfxMessageBox(strMsg);
				arrDivNo.RemoveAt(i);
				i--;
				cnt--;
				continue;
			}
		}

		// 2008.03.10 REM BY PDJ
		// 차수상태에 따른 제한 없앰.
		/*
		ids = m_pGroups->CheckValidate(arrDivNo.GetAt(i), strAcqYear, strWorkCode);
		if(ids < 0)
		{
			if(acqCode == 1)
				strMsg.Format(_T("입력하신 차수번호 [%s]는 사용할 수 없습니다."), arrDivNo.GetAt(i));
			else if(acqCode == 2)
				strMsg.Format(_T("입력하신 접수번호 [%s]는 사용할 수 없습니다."), arrDivNo.GetAt(i));
			else if(acqCode == 0)
				strMsg.Format(_T("입력하신 그룹명 [%s]는 사용할 수 없습니다."), arrDivNo.GetAt(i));

			ESLAfxMessageBox(strMsg);
			arrDivNo.RemoveAt(i);
			i--;
			cnt--;
		}
		*/
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

VOID CBO_SearchDlg::SetAcqCode(ACQ_TYPE acqCode)
{
	m_nAcqCode = acqCode;
	m_pGroups->SetAcqType(acqCode);
	if(acqCode == PURCHASE)
	{
		CString strWorkStatus;
		if(GetOpenMode() == BO_ACQ_PURCHASE || GetOpenMode() == BO_ACQ_WORK_SEARCH || GetOpenMode() == BO_ACQ_HOLD_SEARCH)
			strWorkStatus = _T("구입대상");
		else if(GetOpenMode() == BO_ACQ_ORDER) strWorkStatus = _T("주문");
		else if(GetOpenMode() == BO_ACQ_MISSING || GetOpenMode() == BO_ACQ_REGOBJ) strWorkStatus = _T("검수완료");
		m_pGroups->SetWorkingStatus(strWorkStatus);
	}
}

INT CBO_SearchDlg::MakeGroupInfo(CString strAcqYear, CString strGroupInfo, CString strWorkCode/*=_T("")*/)
{
	if(strGroupInfo.IsEmpty()) return 0;
	if(GetAcqCode() == ACQ_TYPE_INVALID)
	{
		CStringArray tmpArray;
		CString strTmpData;
		ParsingString(_T(","), strGroupInfo, &tmpArray);
		INT cnt = tmpArray.GetSize();
		for(INT i=0 ; i<cnt ; i++)
		{
			if(strTmpData.IsEmpty()) strTmpData = _T("'") + tmpArray.GetAt(i) + _T("'");
			else strTmpData += _T(", '")  + tmpArray.GetAt(i) + _T("'");
		}
		strGroupInfo = strTmpData;
	}
	return m_pGroups->MakeGroupInfo(strAcqYear, strGroupInfo, strWorkCode);
}

CString CBO_SearchDlg::GetGroupInfo()
{
	CString strTmpData;
	m_pGroups->GetGroupInfo(strTmpData);
	return strTmpData;
}

VOID CBO_SearchDlg::SetParentInfo(CString strAcqCode)
{
	ACQ_TYPE nType = ACQ_TYPE_INVALID;
	if(strAcqCode == _T("1"))
		nType = PURCHASE;
	else if(strAcqCode == _T("2"))
		nType = DONATE;
	else 
		nType = ACQ_TYPE_INVALID;

	m_nAcqCode = nType;
	m_pGroups->SetAcqType(nType);
}

CString CBO_SearchDlg::MakeRangeCondition_Stmt(CString strField, CString strData1, CString strData2)
{
	CString strOption, strTmpData;
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	if(strField.Find(_T("REG_CODE")) >= 0) 
	{
		CString strCode;
		
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		INT nNum;
// 		if(!strData1.IsEmpty())
// 		{
// 			strCode = strData1.Left(2);
// 			strData1.Replace(strCode, _T(""));
// 			nNum = _ttoi(strData1.GetBuffer(0));
// 			strData1.Format(_T("%s%10d"), strCode, nNum);
// 		}
// 		if(!strData2.IsEmpty())
// 		{
// 			strCode = strData1.Left(2);
// 			strData2.Replace(strCode, _T(""));
// 			nNum = _ttoi(strData2.GetBuffer(0));
// 			strData2.Format(_T("%s%10d"), strCode, nNum);
// 		}
		if(!strData1.IsEmpty())
		{
			strCode = strData1.Left(nRegCodeLength);
			strData1.Replace(strCode, _T(""));
			strData1 = pParentMgr->m_pInfo->MakeRegNo( strCode, strData1 );
		}
		if(!strData2.IsEmpty())
		{
			strCode = strData1.Left(nRegCodeLength);
			strData2.Replace(strCode, _T(""));
			strData2 = pParentMgr->m_pInfo->MakeRegNo( strCode, strData2 );
		}
		//=====================================================
		
	}
	
	if(strData1.Compare(strData2) > 0)
	{
		strTmpData = strData1;
		strData1 = strData2;
		strData2 = strTmpData;
	}

	if(strData1.IsEmpty() && !(strData2.IsEmpty()))
		strOption.Format(_T(" AND %s <= '%s'"), strField, strData2);
	else if(!(strData1.IsEmpty()) && strData2.IsEmpty())
		strOption.Format(_T(" AND %s = '%s'"), strField, strData1);
	else if(!(strData1.IsEmpty()) && !(strData2.IsEmpty()))
	{
		if(strData1 == strData2)
			strOption.Format(_T(" AND %s = '%s'"), strField, strData1);
		else
			strOption.Format(_T(" AND %s >= '%s' AND %s <= '%s'"), strField, strData1, strField, strData2);
	}
	return strOption;
}

CString CBO_SearchDlg::MakeDivnoOrWorkCodeQuery()
{
	INT cnt = m_pGroups->GetGroupCount();
	CString strQuery, strTmpData, strGroupNo, strWorkCode;
	CGroupInfo * pGroup = NULL;
	for(INT i=0 ; i<cnt ; i++)
	{
		pGroup = m_pGroups->GetGroupInfo(i);
		if(pGroup == NULL) continue;
		strGroupNo = pGroup->GetGroupInfo();
		strWorkCode = pGroup->GetWorkCode();
		if(strWorkCode.IsEmpty()) strTmpData.Format(_T("(P.ORDER_SEQ_NO=%s AND I.WORK_CODE IS NULL)"), strGroupNo);
		else strTmpData.Format(_T("(P.ORDER_SEQ_NO=%s AND I.WORK_CODE='%s')"), strGroupNo, strWorkCode);
		if(strQuery.IsEmpty()) strQuery = strTmpData;
		else strQuery += _T(" OR ") + strTmpData;
	}
	return strQuery;
}

VOID CBO_SearchDlg::ClearScreen(CESL_ControlMgr *pCM, CStringArray * pArrIgnoreCtrlAlias/*=NULL*/)
{
	if (pCM == NULL) return ;

	CESL_Control_Element * pCE = NULL;

	POSITION pos = pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		if (pCE->CTRL_TYPE == _T("G")) continue;
		if (pCE->CTRL_TYPE == _T("M")) 
		{
			CESL_ControlMultiComboBox * pCMB = (CESL_ControlMultiComboBox*)pCE->Control;
			if(pCMB) pCMB->SetCurSel(pCMB->m_nDefaultRowIdx);
			continue;
		}
		if (pCE->CTRL_TYPE == _T("C"))
		{
			CESL_ControlComboBox * pCMB = (CESL_ControlComboBox*)pCE->Control;
			if(pCMB) pCMB->SetCurSel(0);
			continue;
		}

		if(CheckIgnoreCtrl(pArrIgnoreCtrlAlias, pCE->GetAlias())) continue;

		pCE->Clear();
	}

	// 2005-03-10 분류표구분을 Clear 해줌에 따라 주제구분도 바뀌어야 한다.
	SetSubjectCodeData(pCM->CM_ALIAS, _T("분류표구분_코드"), _T("주제구분_코드"));	
}

BOOL CBO_SearchDlg::CheckIgnoreCtrl(CStringArray *pArrIgnoreCtrlAlias, CString strAlias)
{
	if(pArrIgnoreCtrlAlias == NULL) return FALSE;
	INT cnt = pArrIgnoreCtrlAlias->GetSize();

	for(INT i=0 ; i<cnt ; i++)
	{
		if(pArrIgnoreCtrlAlias->GetAt(i) == strAlias)
			return TRUE;
	}
	return FALSE;
}

INT CBO_SearchDlg::GetECOSEARCHQuery(CString &strECOSEARCHQuery)
{
	CESL_DataMgr TmpDM;
	
	INT nCnt = m_arrECOSEARCH.GetSize();
	INT ids;
	CString strTmpData;
	strECOSEARCHQuery.Empty();
	for(INT i=0 ; i<nCnt ; i++)
	{
		strTmpData = m_arrECOSEARCH.GetAt(i);
		strTmpData.MakeUpper();
		strTmpData = strTmpData.Mid(4);
		strTmpData.Replace(_T("I.REC_KEY"), _T(""));
		strTmpData.TrimLeft();	strTmpData.TrimRight();
		ids = TmpDM.GetECOSEARCHReplace(strTmpData);
		if(strECOSEARCHQuery.IsEmpty()) 
			strECOSEARCHQuery = strTmpData;
		else
			strECOSEARCHQuery += _T(" INTERSECT ") + strTmpData;
	}
	return 0;
}

VOID CBO_SearchDlg::ShowDataSortDlg()
{
	if(m_pSortItem == NULL) return;
	CDataSortDlg dlg(this);
	dlg.SetSortItemSet(m_pSortItem);
	dlg.SetOpenMode(-1);
	dlg.DoModal();
}

VOID CBO_SearchDlg::SetIME(INT nIDC, INT nMode)
{
    HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(nIDC)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		if ( nMode == 0 ) dwConversion=IME_CMODE_ALPHANUMERIC;
		else dwConversion=IME_CMODE_HANGUL;		

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(nIDC)->m_hWnd,hIME); 
    } 
}
