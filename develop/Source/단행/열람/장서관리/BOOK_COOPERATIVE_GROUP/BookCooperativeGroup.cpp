#include "stdafx.h"
#include "BookCooperativeGroup.h"
#include "ILL_VIEW_USER_INFO_DLG.h"
#include "ILL_VIEW_STATE_INFO_DLG.h"
#include "ILL_RFID_SECURITY_CHANGE.h"
#include "ILL_RFID_SECURITY_CHANGE2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBookCooperativeGroup::CBookCooperativeGroup(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	INT nCnt = 0;
	INT nBtnStateCnt = 6;
	for(nCnt=0; nCnt < nBtnStateCnt; nCnt++)
	{
		m_pBtnState[nCnt] = new CButton;
	}
	m_bAppendixSelectFlag = FALSE;
}

CBookCooperativeGroup::~CBookCooperativeGroup()	
{
	INT nCnt = 0;
	INT nBtnStateCnt = 6;
	for(nCnt=0; nCnt < nBtnStateCnt; nCnt++)
	{
		if(NULL != m_pBtnState[nCnt])
		{
			delete m_pBtnState[nCnt];
		}
	}
}

void CBookCooperativeGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CBookCooperativeGroup) 
	//}}AFX_DATA_MAP	
}


BEGIN_MESSAGE_MAP(CBookCooperativeGroup, CDialog)
	//{{AFX_MSG_MAP(CBookCooperativeGroup)
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

int CBookCooperativeGroup::UnCheckGrid(CStringArray& aCheckArray)
{
	int ids = 0;
	CString strFuncName = _T("CBookCooperativeGroup::UnCheckGrid(CStringArray&)");

	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount()) return 0;
	
	int SIZE = aCheckArray.GetSize();
	CString strCheck = _T("");
	CString strCheckAlias = _T(""); 
	CString strCheckStyle = _T(""); 
	CString strCheckValue = _T(""); 
	CString strGetValue = _T("");	

	int nIdx = m_pGrid->SelectGetHeadPosition();
	do
	{			
		for(int nCnt = 0; nCnt < SIZE; nCnt++)
		{
			strCheck = aCheckArray.GetAt(nCnt);
			AfxExtractSubString(strCheckAlias, strCheck, 0, '|');
			AfxExtractSubString(strCheckStyle, strCheck, 1, '|');
			AfxExtractSubString(strCheckValue, strCheck, 2, '|');
			strCheckValue = Get_AliasToValue(strCheckValue);

			ids = m_pDM->GetCellData(strCheckAlias, nIdx, strGetValue);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);

			BOOL bIsUnCheck = FALSE;
			if(_T("==") == strCheckStyle)
			{
				if(strGetValue == strCheckValue) bIsUnCheck = TRUE;
			}
			else if(_T("!=") == strCheckStyle)
			{
				if(strGetValue != strCheckValue) bIsUnCheck = TRUE;
			}
			if(TRUE == bIsUnCheck)
			{
				m_pGrid->SetAt(
					nIdx,
					m_pGrid->m_nSelectIdx,
					_T("")
					);
				break;
			}
		}
		nIdx = m_pGrid->SelectGetNext();
	}while(0 <= nIdx);

	return 0;
}


CString CBookCooperativeGroup::Get_AliasToValue(CString strAlias)
{
	CString strReturn = strAlias;
	if(_T("%m_strLibCode%") == strAlias)
	{
		strReturn.Format(_T("%s"), m_strLibCode);
	}
	else if(_T("%NULL%") == strAlias)
	{
		strReturn = _T("");
	}

	return strReturn;
}

CString CBookCooperativeGroup::Get_Select_TRANS_NO_List(BOOL bAppendixFlag)
{

	if(NULL == m_pDM || NULL == m_pGrid) return _T("");

	m_pGrid->SelectMakeList();
	int nIdx = m_pGrid->SelectGetHeadPosition();

	CString strNo = _T("'") + m_pDM->GetCellData(_T("트랜잭션번호"),nIdx) + _T("'");

	int ids = 0;
	CString strGet = _T("");
	while(true)
	{
		nIdx = m_pGrid->SelectGetNext();
		if(0 > nIdx) break;

		ids = m_pDM->GetCellData(_T("트랜잭션번호"), nIdx, strGet);
		if(0 > ids) return _T("");
		if(_T("") == strGet)
		{
			continue;
		}
		else
		{			
			strNo = strNo + _T(",'");
			strNo = strNo + strGet + _T("'");
		}
	}
	
	CString strTemp = _T("");
	CString strMotherKey = _T("");
	CString strAppendixNo = _T("");
	CString strAppendixNoList = _T("");
	
	CString strUserNo = _T("");
	CString strTempUserNo = _T("");

	for(INT i=0; i<m_pDM->GetRowCount(); i++)
	{
		strTemp = m_pGrid->GetAt(i, 0);
		if(_T("V") != strTemp)
		{
			strMotherKey = m_pDM->GetCellData(_T("모체자료_키"), i);
			strAppendixNo = _T("'") + m_pDM->GetCellData(_T("트랜잭션번호"), i) + _T("'");

			strUserNo = m_pDM->GetCellData(_T("이용자_번호"), i);

			if(_T("") != strMotherKey)
			{
				
				BOOL bMotherSelected = FALSE;
				m_pGrid->SelectMakeList();
				nIdx = m_pGrid->SelectGetHeadPosition();
				ids = m_pDM->GetCellData(_T("모체자료_키"), nIdx, strTemp);
			
				m_pDM->GetCellData(_T("이용자_번호"), nIdx, strTempUserNo);
				if(strTemp == strMotherKey && strTempUserNo == strUserNo)
				{
					
					bMotherSelected = TRUE;
				}
				while(true)
				{
					nIdx = m_pGrid->SelectGetNext();
					if(0 > nIdx) 
					{
						break;
					}
					ids = m_pDM->GetCellData(_T("모체자료_키"), nIdx, strTemp);
			
					m_pDM->GetCellData(_T("이용자_번호"), nIdx, strTempUserNo);
					if(strTemp == strMotherKey && strTempUserNo == strUserNo)
					{
						
						bMotherSelected = TRUE;
					}
				}
				if(bMotherSelected)
				{					
					
					if(_T("") == strAppendixNoList)
					{
						strAppendixNoList = strAppendixNo; 
					}
					else
					{
						strAppendixNoList = strAppendixNoList + _T(",") + strAppendixNo;
					}
				}
			}
		}
	}
	if(_T("") != strNo && _T("") != strAppendixNoList)
	{
		
		if(bAppendixFlag)
		{
			if ( IDNO != MessageBox( _T("선정되지 않은 부록자료가 존재합니다.\r\n부록자료 포함하여 처리합니다."), NULL, MB_YESNO|MB_ICONQUESTION ) )
			{
				m_bAppendixSelectFlag = TRUE;
				strNo = strNo + _T(",") + strAppendixNoList;
			}
			else
			{
				m_bAppendixSelectFlag = FALSE;
			}
		}
		else if(!bAppendixFlag && m_bAppendixSelectFlag)
		{
			
			strNo = strNo + _T(",") + strAppendixNoList;
			m_bAppendixSelectFlag = FALSE;
		}
	}
	

	return strNo;
}


int CBookCooperativeGroup::View_UserInfo()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() == 0)
	{
		m_pGrid->SelectRemoveAll();
		INT nRows = m_pDM->GetRowCount();		
		for (int i = 0; i < nRows; i++)
		{
			m_pGrid->m_arraySelect.AddTail(i);
		}
		int nIdx = m_pGrid->GetIdx();
		if(0 > nIdx) nIdx = 0;
		m_pGrid->m_arrayPos = m_pGrid->m_arraySelect.FindIndex(nIdx);
	}
 	int nCnt = m_pGrid->SelectGetCount();
 	if(0 >= nCnt) 
 	{
 		AfxMessageBox(_T("보여줄 정보가 존재하지 않습니다."));
 		return -1;
 	}	

	CILL_VIEW_USER_INFO_DLG Dlg;
	Dlg.m_pLoanMangeValue = m_pLoanMangeValue;
	Dlg.m_pInfo = m_pInfo;
	Dlg.m_pParentDM = m_pDM;
	Dlg.m_pParentGrid = m_pGrid;
	Dlg.m_nIdx = m_pGrid->SelectGetIdx();

	Dlg.DoModal();

	return 0;
}


int CBookCooperativeGroup::View_StateInfo()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() == 0)
	{
		m_pGrid->SelectRemoveAll();
		INT nRows = m_pDM->GetRowCount();		
		for (int i = 0; i < nRows; i++)
		{
			m_pGrid->m_arraySelect.AddTail(i);
		}
		int nIdx = m_pGrid->GetIdx();
		if(0 > nIdx) nIdx = 0;
		m_pGrid->m_arrayPos = m_pGrid->m_arraySelect.FindIndex(nIdx);
	}
 	int nCnt = m_pGrid->SelectGetCount();
 	if(0 >= nCnt) 
 	{
 		AfxMessageBox(_T("보여줄 정보가 존재하지 않습니다."));
 		return -1;
 	}	

	CILL_VIEW_STATE_INFO_DLG Dlg;
	Dlg.m_pParentDM = m_pDM;
	Dlg.m_pParentGrid = m_pGrid;
	Dlg.m_nIdx = m_pGrid->SelectGetIdx();
	Dlg.m_sBookCooperativeMode = m_sBookCooperativeMode;
	Dlg.DoModal();

	return 0;
}

int CBookCooperativeGroup::RefreshMainDM(CString strTableName, CString strOption)
{
	int ids = -1;
	CString strFuncName = _T("CBookCooperativeGroup::RefreshMainDM");

	if(NULL == m_pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);
	if(_T("") == strTableName)
	{

		if(0 > ids) return ids;
	};
	m_pDM->TBL_NAME = strTableName;
	m_pDM->RefreshDataManager(strOption);

	ids = SetDescDM();
	if(0 > ids) return ids;

	m_strTableName = strTableName;
	m_strWhere = strOption;

	return 0;
}

int CBookCooperativeGroup::SetDescDM()
{
	int ids = -1;
	CString strFuncName = _T("CBookCooperativeGroup::SetDescDM");

	if(NULL == m_pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	CString strData;
	CString strData2;
	CString strCancelTypeDesc;
	const int ROW_COUNT = m_pDM->GetRowCount();
	for(int nRow = 0; nRow < ROW_COUNT; nRow++)
	{		
		m_pDM->GetCellData(_T("상태"), nRow, strData);		
		if(_T("0014") == strData)
		{
			ids = m_pDM->GetCellData(_T("대출일"), nRow, strData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
			
			if(_T("") == strData)
			{
				ids = m_pDM->SetCellData(_T("대출상태"), _T("대출대기중"), nRow);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);
			}
			else
			{
				ids = m_pDM->GetCellData(_T("반납도서관_코드"), nRow, strData);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);
				if(_T("") == strData)
				{
					ids = m_pDM->SetCellData(_T("대출상태"), _T("대출중"), nRow);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, strFuncName);
				}
				else
				{
					ids = m_pDM->SetCellData(_T("대출상태"), _T("반납완료"), nRow);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);
				}
			}
		}
		else if(_T("0011") == strData)
		{
			ids = m_pDM->GetCellData(_T("완료유형"), nRow, strData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-7, strFuncName);
			if(_T("") != strData)
			{
				ids = m_pDM->SetCellData(_T("진행상태"), _T("발송거절확인된자료"), nRow);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-8, strFuncName);
			}
		}

		
		ids = m_pDM->GetCellData(_T("자료실_코드"), nRow, strData);
		if(0 > ids)
		{
			return -1000;
		}
		ids = m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("자료실구분") , strData, strData);
		ids = m_pDM->SetCellData(_T("자료실_설명"), strData, nRow);
		if(0 > ids)
		{
			return -1010;
		}

	}	
	return 0;
}

int CBookCooperativeGroup::Get_TblName(class CString &strValue, CString strMode, CString strSearchState)
{
	return 0;
}

int CBookCooperativeGroup::DisplayGrid()
{
	if(NULL == m_pGrid) return -1;

	INT ids = 0;
	CString strValue1 = _T("Y");
	m_pLoanMangeValue->GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue1 );
	
	strValue1.TrimLeft();strValue1.TrimRight();
	strValue1.MakeUpper();
	if( _T("Y") != strValue1 )
	{
		INT i;
		CString strUserNo;
		for(i = 0; i < m_pDM->GetRowCount(); i++)
		{
			m_pDM->GetCellData( _T("이용자_번호"), i , strUserNo );
			
			CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_pLoanMangeValue->m_sIsUnityLoanService );
			m_pDM->SetCellData( _T("이용자_번호"), strUserNo , i );
		}
	}

	return m_pGrid->Display();
}

int CBookCooperativeGroup::Refresh()
{
	int ids = -1;

	Refresh(m_strQuery);

	return 0;
}

int CBookCooperativeGroup::Refresh(CString strState, CString strWhere)
{
	CString strQuery[4];
	Get_SQL(strQuery[1], _T("MO"), strState);
	Get_SQL(strQuery[2], _T("SE"), strState);
	Get_SQL(strQuery[3], _T("NB"), strState);

	if(FALSE == strWhere.IsEmpty())
	{
		strWhere.TrimLeft();
		strWhere = _T("WHERE ") + strWhere;
	}

	strQuery[0].Format(_T("SELECT * FROM (( %s ) UNION ALL ( %s ) UNION ALL ( %s )) T %s %s"), strQuery[1], strQuery[2], strQuery[3], strWhere, _T("ORDER BY LENGTH(TRANS_NO) DESC, TRANS_NO DESC"));

	Refresh(strQuery[0]);
	return 0;
}
int CBookCooperativeGroup::Refresh(CString strQuery)
{
	int ids = -1;

	m_pDM->RestructDataManager(strQuery);
	m_strQuery = strQuery;

	ids = SetDescDM();
	

	if(_T("") != m_pGrid->m_strSortAlias)
	{
		m_pDM->SORT(m_pGrid->m_strSortAlias, m_pGrid->m_bIsDesc);
	}

	
	return DisplayGrid();
}


int CBookCooperativeGroup::Get_UpdateIdxTblQuery(int nIdx, CString strLastWork, CString &strQuery)
{
	int ids = 0;
	CString strFuncName = _T("Get_UpdateIdxTblQuery");

	if( (m_pInfo->MODE == 10000) || (m_pInfo->MODE == 30000) )
	{
		return 1000;
	}

	CString strBookKey = _T("");
 	ids = m_pDM->GetCellData(_T("책KEY"), nIdx, strBookKey);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	}

	CString strManageCode = _T("");
	ids = m_pDM->GetCellData(_T("제공도서관_관리구분"), nIdx, strManageCode);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	}

	CString strPublishFormCode = _T("");
	ids = m_pDM->GetCellData(_T("발행형태구분"), nIdx, strPublishFormCode);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
	}

	CString strTblName, strIDXTblName, strProcName;
	if(_T("MO") == strPublishFormCode || _T("단행") == strPublishFormCode)
	{
		strTblName = _T("BO_BOOK_TBL");
		strIDXTblName = _T("IDX_BO_TBL");			
		strProcName = _T("TESTWORKSTATUS");
	}
	else if(_T("SE") == strPublishFormCode || _T("연속") == strPublishFormCode)
	{
		strTblName = _T("SE_BOOK_TBL");
		strIDXTblName = _T("IDX_SE_TBL");			
		strProcName = _T("TESTSEWORKSTATUS");
	}
	else if(_T("NB") == strPublishFormCode || _T("긴급") == strPublishFormCode)
	{
		return 1010;
	}
	else
	{
		return -1030;
	}

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	
	CString strGetQuery, strSpeciesKey;
	strGetQuery.Format(
		_T("SELECT BB.SPECIES_KEY FROM %s BB WHERE BB.REC_KEY = %s AND BB.MANAGE_CODE = '%s' AND ROWNUM = 1"),
		strTblName, strBookKey, strManageCode);
	ids = TmpDM.GetOneValueQuery(strGetQuery, strSpeciesKey);
	if(0 > ids)
	{
		return -1040;
	}
		
	strQuery.Format(
		_T("UPDATE %s SET WORKING_STATUS = %s(%s) WHERE REC_KEY = %s;"),
		strIDXTblName, strProcName , strSpeciesKey , strSpeciesKey);	

	return 0;
}

int CBookCooperativeGroup::Get_UpdateIdxTblQuery(CDataMgr* pDataMgr, int nIdx, CString strLastWork, CString &strQuery)
{
	int ids = 0;
	CString strFuncName = _T("Get_UpdateIdxTblQuery");

	if( (m_pInfo->MODE == 10000) || (m_pInfo->MODE == 30000) )
	{
		return 1000;
	}

	if(NULL == pDataMgr)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1060, strFuncName);
	}

	if(nIdx >= pDataMgr->GetCount())
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1070, strFuncName);
	}

	CString strBookKey = _T("");
 	ids = pDataMgr->GetData(_T("책KEY"), nIdx, strBookKey);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	}

	CString strManageCode = _T("");
	ids = pDataMgr->GetData(_T("제공도서관_관리구분"), nIdx, strManageCode);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	}

	CString strPublishFormCode = _T("");
	ids = pDataMgr->GetData(_T("발행형태구분"), nIdx, strPublishFormCode);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
	}

	CString strTblName, strIDXTblName, strProcName;
	if(_T("MO") == strPublishFormCode || _T("단행") == strPublishFormCode)
	{
		strTblName = _T("BO_BOOK_TBL");
		strIDXTblName = _T("IDX_BO_TBL");			
		strProcName = _T("TESTWORKSTATUS");
	}
	else if(_T("SE") == strPublishFormCode || _T("연속") == strPublishFormCode)
	{
		strTblName = _T("SE_BOOK_TBL");
		strIDXTblName = _T("IDX_SE_TBL");			
		strProcName = _T("TESTSEWORKSTATUS");
	}
	else if(_T("NB") == strPublishFormCode || _T("긴급") == strPublishFormCode)
	{
		return 1030;
	}
	else
	{
		return -1040;
	}

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	
	CString strGetQuery, strSpeciesKey;
	strGetQuery.Format(
		_T("SELECT BB.SPECIES_KEY FROM %s BB WHERE BB.REC_KEY = %s AND BB.MANAGE_CODE = '%s' AND ROWNUM = 1"),
		strTblName, strBookKey, strManageCode);
	ids = TmpDM.GetOneValueQuery(strGetQuery, strSpeciesKey);
	if(0 > ids)
	{
		return -1050;
	}
		
	strQuery.Format(
		_T("UPDATE %s SET WORKING_STATUS = %s(%s) WHERE REC_KEY = %s;"),
		strIDXTblName, strProcName , strSpeciesKey , strSpeciesKey);	

	return 0;
}

int CBookCooperativeGroup::Get_UpdateBookTblQuery_For_CompleteTransaction
(
    int nIdx,
    CString strLastWork, 
    CString &strQuery
    )
{
	int ids = 0;
	CString strFuncName = _T("CBOOK_COOPERATIVE_GROUP_REQUEST::Get_UpdateBookTblQuery_For_CompleteTransaction");

	if(nIdx >= m_pDM->GetRowCount()) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	CString strBookKey = _T("");
	ids = m_pDM->GetCellData(_T("책KEY"), nIdx, strBookKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);

	
	CString strPublishFormCode = _T("");
	ids = m_pDM->GetCellData(_T("발행형태구분"), nIdx, strPublishFormCode);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);
	
	CString strGiveManageCode = _T("");
	ids = m_pDM->GetCellData(_T("제공도서관_관리구분"), nIdx, strGiveManageCode);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);
	
	CString strTblName = _T("");
	if(_T("MO") == strPublishFormCode) strTblName = _T("BO_BOOK_TBL");		
	else if(_T("SE") == strPublishFormCode) strTblName = _T("SE_BOOK_TBL");
	else if(_T("NB") == strPublishFormCode) strTblName = _T("CO_NON_DB_BOOK_TBL");
	else return -1;
	
	CString strWorkingStatus = _T(""), strUpdateWorkingStatus = _T("");
	if(_T("MO") == strPublishFormCode) {
		strWorkingStatus = _T("BOL411O");		
		strUpdateWorkingStatus = _T("BOL112N");
	}
	else if(_T("SE") == strPublishFormCode) {
		strWorkingStatus = _T("SEL411O");
		strUpdateWorkingStatus = _T("SEL212N");
	}
	else if(_T("NB") == strPublishFormCode) {
		strWorkingStatus = _T("BOL411O");
		strUpdateWorkingStatus = _T("BOL112N");
	}
	else return -2;
	
	strQuery.Format(
		_T("UPDATE ")
			_T("%s ") 
		_T("SET ")
		    
		    _T("WORKING_STATUS = '%s',")
			_T("PREV_WORKING_STATUS = '%s', ") 
			_T("LAST_WORK = '%s' ")
		_T("WHERE ")
			_T("REC_KEY = %s AND MANAGE_CODE = '%s' AND WORKING_STATUS = '%s';"),
			strTblName,
			strUpdateWorkingStatus,
			strWorkingStatus,
			strLastWork,
			strBookKey,
		 	strGiveManageCode,	
			strWorkingStatus
			);

	return 0;
}

int CBookCooperativeGroup::Get_UpdateLSWorkQuery(int nIdx, CString strLastWork, CString &strQuery)
{
	int ids = 0;	
	CString strLoanKey = _T("");
	CString strAPLLibCode = _T("");
	CString strManageCode = _T("");
	CString strLoanStatus = _T("");
	CString strPublishFormCode = _T("");
	CString strUpdateTblName = _T("");
	CString strGetQuery, strSpeciesKey;

	CESL_DataMgr TmpDM;	

	CString strFuncName = _T("Get_UpdateLSWorkQuery");		

	if ( nIdx >= m_pDM->GetRowCount() ) {
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	} 	

	ids = m_pDM->GetCellData(_T("대출정보_키"), nIdx, strLoanKey);	
	if(0 > ids) {
		ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
	}
	
	ids = m_pDM->GetCellData(_T("요청도서관_코드"), nIdx, strAPLLibCode);		
	if(0 > ids) {
		ERROR_MSG_RETURN_ERROR_CODE2(-1030, strFuncName);
	}

	if ( strLoanKey.GetLength() <= 0 ) {
		return -1040;
	}

	if ( strAPLLibCode.GetLength() > 0 ) {
		strQuery = _T("");
		return 100;
	}

	TmpDM.SetCONNECTION_INFO(_T(""));

	strGetQuery.Format( _T("SELECT STATUS FROM LS_WORK_T01 WHERE REC_KEY=%s "), strLoanKey);
	ids = TmpDM.GetOneValueQuery(strGetQuery, strLoanStatus);
	if(0 > ids) {
		return -1050;
	}

	strUpdateTblName.Format(_T("LS_WORK_T01"));

	if ( strLoanStatus.GetLength() <= 0 ) {
		strGetQuery.Format( _T("SELECT STATUS FROM CO_LOAN_TBL WHERE REC_KEY=%s "), strLoanKey);

		ids = TmpDM.GetOneValueQuery(strGetQuery, strLoanStatus);
		if(0 > ids) {
			return -1060;
		}

		strUpdateTblName.Format(_T("CO_LOAN_TBL"));
	}

	if ( strLoanStatus.Compare(_T("R")) != 0 ) {
		strQuery = _T("");
		return 100;
	}
		
	strQuery.Format( _T("UPDATE %s SET STATUS = '1', LAST_WORK='%s' WHERE REC_KEY = %s;")
		, strUpdateTblName, strLastWork , strLoanKey);	

	return 0;
}

int CBookCooperativeGroup::Get_UpdateBookTblQuery_For_CompleteTransaction
(
	CDataMgr* pDataMgr,
    int nIdx,
    CString strLastWork, 
    CString &strQuery
    )
{
	int ids = 0;
	CString strFuncName = _T("Get_UpdateBookTblQuery_For_CompleteTransaction");

	if(NULL == pDataMgr)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
	}

	if(nIdx >= pDataMgr->GetCount())
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
	}

	CString strBookKey = _T("");
	ids = pDataMgr->GetData(_T("책KEY"), nIdx, strBookKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);

	
	CString strPublishFormCode = _T("");
	ids = pDataMgr->GetData(_T("발행형태구분"), nIdx, strPublishFormCode);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);
	
	CString strGiveManageCode = _T("");
	ids = pDataMgr->GetData(_T("제공도서관_관리구분"), nIdx, strGiveManageCode);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);
	
	
	CString strTblName = _T("");
	if(_T("MO") == strPublishFormCode) strTblName = _T("BO_BOOK_TBL");		
	else if(_T("SE") == strPublishFormCode) strTblName = _T("SE_BOOK_TBL");
	else if(_T("NB") == strPublishFormCode) strTblName = _T("CO_NON_DB_BOOK_TBL");
	else return -1;
	
	CString strWorkingStatus = _T(""), strUpdateWorkingStatus = _T("");
	if(_T("MO") == strPublishFormCode) {
		strWorkingStatus = _T("BOL411O");
		strUpdateWorkingStatus = _T("BOL112N");
	}
	else if(_T("SE") == strPublishFormCode) {
		strWorkingStatus = _T("SEL411O");
		strUpdateWorkingStatus = _T("SEL212N");
	}
	else if(_T("NB") == strPublishFormCode) {
		strWorkingStatus = _T("BOL411O");
		strUpdateWorkingStatus = _T("BOL112N");
	}
	else return -2;
	
	strQuery.Format(
		_T("UPDATE ")
			_T("%s ")
		_T("SET ")
			_T("WORKING_STATUS = '%s',")
			_T("PREV_WORKING_STATUS = '%s', ")
			_T("LAST_WORK = '%s' ")
		_T("WHERE ")
			_T("REC_KEY = %s AND MANAGE_CODE = '%s' AND WORKING_STATUS = '%s';"),
			strTblName,
			strUpdateWorkingStatus,
			strWorkingStatus,
			strLastWork,
			strBookKey,
		 	strGiveManageCode,	
			strWorkingStatus
			);

	return 0;
}

INT CBookCooperativeGroup::ShareInfoCenterUpload(CString strRecKeyList, CString strType, CString strTableName, CString strUpdateField)
{
	UPLOAD_SHARE_INFO UploadShareInfo;	
	UploadShareInfo.nType = 0;

	if(_T("LH_STORAGE_V01") == strTableName)
	{
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	}
	else if(_T("CO_LOAN_USER_TBL") == strTableName)
	{
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
	}
	else if(_T("ILL_TRANS_TBL") == strTableName)
	{
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("71");
	}	

	UploadShareInfo.UploadShareLoanInfo.sType = strType;	
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = strTableName;
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_pInfo->LIB_CODE;

	if(_T("U") == UploadShareInfo.UploadShareLoanInfo.sType)
	{
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = strUpdateField;
	}
	
	
	if(
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields.Find(_T("USER_CLASS")) >= 0 ||
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields.Find(_T("LOAN_STOP_DATE")) >= 0 )
	{
		UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("N");
	}

	CString strRecKey;
	INT nIdx = 0;
	while(FALSE != AfxExtractSubString(strRecKey, strRecKeyList, nIdx, ','))
	{
		strRecKey.TrimLeft(_T("'"));
		strRecKey.TrimRight(_T("'"));
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = strRecKey;
		m_pLoanShareManager->SendLoanInfoUpload(UploadShareInfo);
		nIdx++;
	}
	return 0;
}


int CBookCooperativeGroup::RFID_SecurityChange(CString strTransNoList, BOOL bSecurity, BOOL bIsNotSelect)
{
	return 0;
}

int CBookCooperativeGroup::RFID_SecurityChange()
{
	return 0;
}


INT CBookCooperativeGroup::MakeTransNoList(CStringArray& aTransNoList, CString& strTransNoList)
{
	INT nCnt = 0;
	const INT SIZE = aTransNoList.GetSize();
	CString strTransNo;
	strTransNoList.Empty();

	for(nCnt = 0; nCnt < SIZE; nCnt++)
	{		
		strTransNoList = strTransNoList + _T(",'") + aTransNoList.GetAt(nCnt) + _T("'");
	}
	strTransNoList.TrimLeft(_T(","));

	return 0;
}


INT CBookCooperativeGroup::MakeTransNoList(CDataMgr *pDataMgr, CString& strTransNoList, BOOL bAppendixFlag)

{
	INT nCnt = 0;
	const INT SIZE = pDataMgr->GetCount();
	CString strTransNo;
	strTransNoList.Empty();

	for(nCnt = 0; nCnt < SIZE; nCnt++)
	{		
		pDataMgr->GetData(_T("트랜잭션번호"), nCnt, strTransNo);
		strTransNoList = strTransNoList + _T(",'") + strTransNo + _T("'");
	}
	strTransNoList.TrimLeft(_T(","));

	
	
	INT nIdx = 0;
	INT ids = 0;
	CString strTemp = _T("");
	CString strMotherKey = _T("");
	CString strAppendixNo = _T("");
	CString strAppendixNoList = _T("");
	
	
	CString strUserNo;
	CString strTempUserNo;

	for(INT i=0; i<m_pDM->GetRowCount(); i++)
	{
		strTemp = m_pGrid->GetAt(i, 0);
		if(_T("V") != strTemp)
		{
			
			strMotherKey = m_pDM->GetCellData(_T("모체자료_키"), i);
			strAppendixNo = _T("'") + m_pDM->GetCellData(_T("트랜잭션번호"), i) + _T("'");
			
			
			strUserNo = m_pDM->GetCellData(_T("이용자_번호"), i);

			if(_T("") != strMotherKey)
			{
				
				BOOL bMotherSelected = FALSE;
				m_pGrid->SelectMakeList();
				nIdx = m_pGrid->SelectGetHeadPosition();
				ids = m_pDM->GetCellData(_T("모체자료_키"), nIdx, strTemp);
				
				
				ids = m_pDM->GetCellData(_T("이용자_번호"), nIdx, strTempUserNo);
				if(strTemp == strMotherKey && strTempUserNo == strUserNo)

				{
					
					bMotherSelected = TRUE;
				}
				while(true)
				{
					nIdx = m_pGrid->SelectGetNext();
					if(0 > nIdx) 
					{
						break;
					}
					ids = m_pDM->GetCellData(_T("모체자료_키"), nIdx, strTemp);
					
					
					ids = m_pDM->GetCellData(_T("이용자_번호"), nIdx, strTempUserNo);
					if(strTemp == strMotherKey && strTempUserNo == strUserNo)

					{
						
						bMotherSelected = TRUE;
					}
				}
				if(bMotherSelected)
				{					
					
					if(_T("") == strAppendixNoList)
					{
						strAppendixNoList = strAppendixNo; 
					}
					else
					{
						strAppendixNoList = strAppendixNoList + _T(",") + strAppendixNo;
					}
				}
			}
		}
	}
	if(_T("") != strTransNoList && _T("") != strAppendixNoList)
	{
		
		if(bAppendixFlag)
		{
			if ( IDNO != MessageBox( _T("선정되지 않은 부록(모체)자료가 존재합니다.\r\n해당자료 포함하여 처리합니다."), NULL, MB_YESNO|MB_ICONQUESTION ) )
			{
				m_bAppendixSelectFlag = TRUE;
				strTransNoList = strTransNoList + _T(",") + strAppendixNoList;
			}
			else
			{
				m_bAppendixSelectFlag = FALSE;
			}
		}
		else if(!bAppendixFlag && m_bAppendixSelectFlag)
		{
			
			strTransNoList = strTransNoList + _T(",") + strAppendixNoList;
			m_bAppendixSelectFlag = FALSE;
		}
	}
	
	

	return 0;
}

INT CBookCooperativeGroup::Get_DefaultWhere(CString &strWhere, CString strMode)
{
	if(_T("MO") == strMode)
	{
		strWhere = _T("A.BOOK_KEY = BB.REC_KEY AND BB.SPECIES_KEY = IB.REC_KEY AND A.USER_KEY = CLU.REC_KEY");
	}
	else if(_T("SE") == strMode)
	{
		strWhere = _T("A.BOOK_KEY = BB.REC_KEY AND BB.SPECIES_KEY=IB.REC_KEY AND BB.VOL_KEY=V.REC_KEY AND A.USER_KEY = CLU.REC_KEY AND A.PUBLISH_FORM_CODE = 'SE'");
	}
	else if(_T("NB") == strMode)
	{
		strWhere = _T("A.BOOK_KEY = BB.REC_KEY AND A.USER_KEY = CLU.REC_KEY AND A.PUBLISH_FORM_CODE = 'NB'");
	}
	return 0;
}

INT CBookCooperativeGroup::Get_SQL(CString &strGetQuery, CString strMode, CString strState)
{
	INT aColIndex[256];	
	CString strTblName, strWhere, strBakTblName, strBakCondition, strBakCondition2;
	Get_TblName(strTblName, strMode, strState);
	Get_DefaultWhere(strWhere, strMode);

	strBakTblName = m_pDM->TBL_NAME;
	strBakCondition = m_pDM->CONDITION;
	strBakCondition2 = m_pDM->EXTRA_CONDITION; 

	m_pDM->TBL_NAME = strTblName;
	m_pDM->CONDITION = _T("");
	m_pDM->EXTRA_CONDITION = _T("");

	strGetQuery = m_pDM->MakeSelectQuery(strWhere, aColIndex);

	m_pDM->TBL_NAME = strBakTblName;
	m_pDM->CONDITION = strBakCondition;
	m_pDM->EXTRA_CONDITION = strBakCondition2; 

	if(_T("MO") == strMode)
	{
		
		strGetQuery.Replace(_T("<$청구기호$>"), _T("ECO$F_MAKE_CALLNO$I(BB.SEPARATE_SHELF_CODE, BB.CLASS_NO, BB.BOOK_CODE, BB.VOL_CODE, BB.COPY_CODE)"));
		strGetQuery.Replace(_T("<$ISBN$>"), _T("BB.EA_ISBN"));
		strGetQuery.Replace(_T("<$ISSN$>"), _T("NULL"));
		strGetQuery.Replace(_T("<$자료실코드$>"), _T("BB.SHELF_LOC_CODE"));
		strGetQuery.Replace(_T("<$자료상태$>"), _T("BB.WORKING_STATUS"));	
		strGetQuery.Replace(_T("<$발행년$>"),			_T("BB.PUBLISH_YEAR"));
		strGetQuery.Replace(_T("<$편권차$>"), _T("BB.VOL"));
	}
	else if(_T("SE") == strMode)
	{
		
		strGetQuery.Replace(_T("<$청구기호$>"), _T("ECO$F_MAKE_CALLNO$I(BB.SEPARATE_SHELF_CODE, BB.CLASS_NO, BB.BOOK_CODE, BB.VOL_CODE, BB.COPY_CODE)"));
		strGetQuery.Replace(_T("<$ISBN$>"), _T("NULL"));
		strGetQuery.Replace(_T("<$ISSN$>"), _T("IB.ST_ISSN"));
		strGetQuery.Replace(_T("<$자료실코드$>"), _T("BB.SHELF_LOC_CODE"));
		strGetQuery.Replace(_T("<$자료상태$>"), _T("BB.WORKING_STATUS"));
		strGetQuery.Replace(_T("<$발행년$>"),	_T("NULL"));
		strGetQuery.Replace(_T("<$편권차$>"), _T("NULL"));
	}
	else if(_T("NB") == strMode)
	{
		strGetQuery.Replace(_T("<$청구기호$>"), _T("BB.CALL_NO"));
		strGetQuery.Replace(_T("<$ISBN$>"), _T("NULL"));
		strGetQuery.Replace(_T("<$ISSN$>"), _T("NULL"));
		strGetQuery.Replace(_T("<$자료실코드$>"), _T("BB.SHELF_LOC_CODE"));
		strGetQuery.Replace(_T("<$자료상태$>"), _T("BB.WORKING_STATUS"));
		strGetQuery.Replace(_T("IB.TITLE_INFO"), _T("BB.TITLE"));
		strGetQuery.Replace(_T("IB.AUTHOR_INFO"), _T("BB.AUTHOR"));
		strGetQuery.Replace(_T("IB.IDX_TITLE"), _T("BB.TITLE"));
		strGetQuery.Replace(_T("IB.PUB_INFO"), _T("BB.PUBLISHER"));
		strGetQuery.Replace(_T("<$발행년$>"), _T("NULL"));
		strGetQuery.Replace(_T("<$편권차$>"), _T("NULL"));
	}

	return 0;
}

CString CBookCooperativeGroup::EndDivDesc(CString strCode)
{
	CString strDesc;

	strDesc=_T("");
	if ( strCode == _T("0001") ) strDesc=_T("이용자취소");
	else if ( strCode == _T("0002") ) strDesc=_T("요청거절");
	else if ( strCode == _T("0003") ) strDesc=_T("무응답/미대출");
	else if ( strCode == _T("0004") ) strDesc=_T("발송거절(도서대출불가)");
	else if ( strCode == _T("0010") ) strDesc=_T("완료");
	else if ( strCode == _T("0011") ) strDesc=_T("기타");

	return strDesc;
}





INT CBookCooperativeGroup::SetCheckAppendixAndMother(BOOL bReturnTrans)



{
	INT ids = 0;
	CString strFuncName = _T("SetCheckAppendixAndMother()");
	CString strMotherKey;
	CString strTemp;

	
	
	CString strUserNo;

	m_pGrid->SelectMakeList();

	INT nCnt = 0;
	INT nResult = IDNO;
	BOOL bContinue = TRUE;
	int nIdx = m_pGrid->SelectGetHeadPosition();
	do
	{
		ids = m_pDM->GetCellData(_T("모체자료_키"), nIdx, strMotherKey);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
		}
		if(_T("") == strMotherKey)
		{
			break;
		}
		
		
		ids = m_pDM->GetCellData(_T("이용자_번호"), nIdx, strUserNo);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);
		}
		
		for(INT i=0; i<m_pDM->GetRowCount(); i++)
		{
			if(_T("V") == m_pGrid->GetAt(i, 0))
			{
				continue;
			}
			else
			{
				
				
				if(nIdx == i)
				{
					
					continue;
				}
				ids = m_pDM->GetCellData(_T("이용자_번호"), i, strTemp);
				if(0 > ids) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2(-5, strFuncName);
				}
				if(strTemp != strUserNo)
				{
					continue;
				}

				
				ids = m_pDM->GetCellData(_T("모체자료_키"), i, strTemp);
				if(0 > ids) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);
				}

				
				
				if(TRUE == bReturnTrans)
				{
					CString strStatus;
					ids = m_pDM->GetCellData(_T("대출상태"), i, strStatus);
					if(0 > ids) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2(-6, strFuncName);
					}
					if(_T("반납완료") != strStatus)
					{
						continue;
					}
				}

				if(strTemp == strMotherKey)
				{
					
					if(0 == nCnt && bContinue)
					{
						nResult = AfxMessageBox(_T("선정되지 않은 부록(모체)자료가 존재합니다.\r\r해당 자료를 일괄처리 하시겠습니까?"), MB_YESNO);
					}
					if(IDYES == nResult)
					{					
						nCnt++;
						m_pGrid->SetAt(i, 0, _T("V"));
						continue;
					}
					else
					{
						bContinue = FALSE;
						break;
					}
				}
			}
		}
		if(!bContinue)
		{
			break;
		}
		nIdx = m_pGrid->SelectGetNext();
	}
	while(0 <= nIdx);

	return 0;
}