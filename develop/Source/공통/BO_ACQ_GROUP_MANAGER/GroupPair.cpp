// GroupPair.cpp: implementation of the CGroupPair class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GroupPair.h"
#include "CommonDefine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("그룹간작업"), _T(__FILE__), __LINE__
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGroupPair::CGroupPair(CESL_Mgr * pMgr)
{
	m_pParentMgr = pMgr;
	m_pSourceGroup = NULL;
	m_pTargetGruop = NULL;
	m_pSourceGroupCollection = NULL;
	m_pSourceData = NULL;
	m_nGroupType = GROUP_NO;
	m_pDM_IDX = new CESL_DataMgr;
	m_pDM_SPECIES = new CESL_DataMgr;
	m_pDM_ACCU = new CESL_DataMgr;
	m_pDM_BOOK = new CESL_DataMgr;
	m_pDM_WORKNO = new CESL_DataMgr;
	m_pDataProcessingDlg = NULL;
	m_pArrRecKey = NULL;
	m_bDeleteDataYN = FALSE;
}

CGroupPair::~CGroupPair()
{
	if(m_pDM_IDX)
	{
		delete m_pDM_IDX;
		m_pDM_IDX = NULL;
	}
	if(m_pDM_SPECIES)
	{
		delete m_pDM_SPECIES;
		m_pDM_SPECIES = NULL;
	}
	if(m_pDM_ACCU)
	{
		delete m_pDM_ACCU;
		m_pDM_ACCU = NULL;
	}
	if(m_pDM_BOOK)
	{
		delete m_pDM_BOOK;
		m_pDM_BOOK = NULL;
	}
	if(m_pDM_WORKNO)
	{
		delete m_pDM_WORKNO;
		m_pDM_WORKNO = NULL;
	}
	if(m_pDataProcessingDlg)
	{
		DeleteDataProcessingDlg();
	}
}

INT CGroupPair::CopyGroup()
{
	INT ids = -1;
	switch( m_nGroupType )
	{
	case GROUP_NO:		ids = CopyAcqObjGroup();
		break;
	case DIVNO:			ids = CopyDivGroup();
		break;
	case RECEIPT_NO:	ids = CopyReceiptGroup();
		break;
    case OTHER_GROUP_NO:ids = CopyOtherGroup();
		break;
	default:			ids = -1;
		break;
	}

	return ids;
}

INT CGroupPair::CopyBookData(CString strAcqCode, CString strNewKey, CString strOldKey, CString strNewAcqKey/*=_T("")*/, CString strOldAcqKey/*=_T("")*/)
{
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BB_원본등록번호"));
	CString strQuery, strGroupNo;
	if(strAcqCode == _T("0")) strGroupNo = m_pSourceGroup->GetGroupNo();
	else strGroupNo = m_pSourceGroup->GetGroupInfo();

	if(strAcqCode == _T("0"))
	{
		strQuery.Format(
			_T("(ACQ_CODE='0' OR ACQ_CODE IS NULL) AND ACQ_YEAR='%s' AND SEQ_NO=%s AND SPECIES_KEY=%s"),
			m_pSourceGroup->GetAcqYear(), strGroupNo, strOldKey);
		if(!strOldAcqKey.IsEmpty())
			strQuery += _T(" AND ACQ_KEY=") + strOldAcqKey;
	}
	else
	{
		strQuery.Format(
			_T("ACQ_CODE='%s' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND SPECIES_KEY=%s AND (WORKING_STATUS < 'BOC' OR WORKING_STATUS = 'BOR111N')"),
			strAcqCode, m_pSourceGroup->GetAcqYear(), strGroupNo, strOldKey);
		if(!strOldAcqKey.IsEmpty())
			strQuery += _T(" AND ACQ_KEY=") + strOldAcqKey;
	}
	m_pDM_BOOK->RefreshDataManager(strQuery);
	CString strTmpData, strBookKey;
	INT nRowCnt = m_pDM_BOOK->GetRowCount();
	
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		m_pDM_BOOK->MakeRecKey(strBookKey);
		m_pDM_BOOK->SetCellData(_T("BB_책KEY"), strBookKey, i);
		m_pDM_BOOK->SetCellData(_T("BB_종KEY"), strNewKey, i);
		m_pDM_BOOK->SetCellData(_T("BB_수입년도"), m_pTargetGruop->GetAcqYear(), i);
		if(strAcqCode == _T("0")) strGroupNo = m_pTargetGruop->GetGroupNo();
		else strGroupNo = m_pTargetGruop->GetGroupInfo();
		m_pDM_BOOK->SetCellData(_T("BB_그룹번호"), strGroupNo, i);
		if(!strNewAcqKey.IsEmpty())
			m_pDM_BOOK->SetCellData(_T("BB_수서정보KEY"), strNewAcqKey, i);
		m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_BOOK, m_pDM_BOOK->TBL_NAME, i, m_pDM_BOOK);
	}
	return 0;
}

INT CGroupPair::CopyDONACQData(CString strNewKey, CString strOldKey)
{
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	CString strQuery;
	strQuery.Format(
		_T("SPECIES_KEY=%s AND ACQ_YEAR='%s' AND RECEIPT_NO=%s"),
		strOldKey, m_pSourceGroup->GetAcqYear(), m_pSourceGroup->GetGroupInfo());

	m_pDM_ACCU->RefreshDataManager(strQuery);
	INT nRowCnt = m_pDM_ACCU->GetRowCount();
	CString strNewAcqKey, strOldAcqKey, strAcqYear, strGroupNo;
	strAcqYear = m_pTargetGruop->GetAcqYear();
	strGroupNo = m_pTargetGruop->GetGroupInfo();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strOldAcqKey = m_pDM_ACCU->GetCellData(_T("BP_수서정보KEY"), i);
		m_pDM_BOOK->MakeRecKey(strNewAcqKey);
		m_pDM_ACCU->SetCellData(_T("BP_수서정보KEY"), strNewAcqKey, i);
		m_pDM_ACCU->SetCellData(_T("BP_수입년도"), strAcqYear, i);
		m_pDM_ACCU->SetCellData(_T("BP_그룹번호"), strGroupNo, i);
		m_pDM_ACCU->SetCellData(_T("BP_종KEY"), strNewKey, i);
		m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_ACCU, m_pDM_ACCU->TBL_NAME, i, m_pDM_BOOK);
		CopyBookData(_T("2"), strNewKey, strOldKey, strNewAcqKey, strOldAcqKey);
	}
	return 0;
}

INT CGroupPair::CopyAcqObjGroup()
{
	INT ids = SetDataMgr(m_pDM_IDX, _T("DMUP_단행_종_색인정보"));
	if(ids < 0) return -1;
	ids = SetDataMgr(m_pDM_SPECIES, _T("DMUP_단행_종_종정보"));
	if(ids < 0) return -1;
	ids = SetDataMgr(m_pDM_BOOK, _T("DMUP_단행_종_책정보"));
	if(ids < 0) return -1;

	CString strQuery, strTmpQuery;
	strQuery.Format(
		_T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE (ACQ_CODE='0' OR ACQ_CODE IS NULL) AND ACQ_YEAR='%s' AND SEQ_NO=%s AND MANAGE_CODE = UDF_MANAGE_CODE)"),
		m_pSourceGroup->GetAcqYear(), m_pSourceGroup->GetGroupNo());
	m_pDM_IDX->RefreshDataManager(strQuery);
	m_pDM_SPECIES->RefreshDataManager(strQuery);
	
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	
	CString strNewKey, strOldKey, strMsg;

	INT nRowCnt = m_pDM_IDX->GetRowCount();
	ShowDataProcessingDlg();
	strMsg.Format(_T("%s 그룹 자료를 복사중입니다."), m_pSourceGroup->GetGroupInfo());
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);

	INT nQueryUnitCnt = 50;
	
	m_pDM_BOOK->StartFrame();
	
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);

		strOldKey = m_pDM_IDX->GetCellData(_T("IBS_REC_KEY"), i);
		m_pDM_BOOK->MakeRecKey(strNewKey);
		m_pDM_IDX->SetCellData(_T("IBS_REC_KEY"), strNewKey, i);
		m_pDM_SPECIES->SetCellData(_T("BS_종KEY"), strNewKey, i);
		m_pDM_SPECIES->SetCellData(_T("BS_최초작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_SPECIES->SetCellData(_T("BS_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_IDX->SetCellData(_T("IBS_최초작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_IDX->SetCellData(_T("IBS_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);

		ids = DeleteAcqUselessMarcTag(m_pDM_SPECIES, i);
		if(ids < 0) continue;
		ids = m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_IDX, m_pDM_IDX->TBL_NAME, i, m_pDM_BOOK);
		if(ids < 0) return ids;
		ids = m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_SPECIES, m_pDM_SPECIES->TBL_NAME, i, m_pDM_BOOK);	
		if(ids < 0) return ids;
		if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strTmpQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strNewKey);
			m_pDM_BOOK->AddFrame(strTmpQuery);
		}
		CopyBookData(_T("0"), strNewKey, strOldKey);
		if(i%nQueryUnitCnt == 0)
		{
			ids = m_pDM_BOOK->SendFrame();	
			if(ids < 0) return -1;
			ids = m_pDM_BOOK->EndFrame();
			if(ids < 0) return -1;

			if(ids < 0)
			{
				DeleteDataProcessingDlg();
				return -1;
			}
			m_pDM_BOOK->StartFrame();
		}		
	}

	ids = m_pDM_BOOK->SendFrame();
	DeleteDataProcessingDlg();
	if(ids < 0) return -1;
	ids = m_pDM_BOOK->EndFrame();
	if(ids < 0) return -1;
	
	return 0;
}

INT CGroupPair::CopyDivGroup()
{
	INT ids = SetDataMgr(m_pDM_IDX, _T("DMUP_단행_종_색인정보"));
	if(ids < 0) return -1;
	ids = SetDataMgr(m_pDM_SPECIES, _T("DMUP_단행_종_종정보"));
	if(ids < 0) return -1;
	ids = SetDataMgr(m_pDM_BOOK, _T("DMUP_단행_종_책정보"));
	if(ids < 0) return -1;
	ids = SetDataMgr(m_pDM_ACCU, _T("DMUP_단행_종_구입정보"));
	if(ids < 0) return -1;
	ids = SetDataMgr(m_pDM_WORKNO, _T("DM_작업번호관리"));
	if(ids < 0) return -1;

	CString strQuery, strTmpQuery, strWorkCode;
	strWorkCode = m_pSourceGroup->GetWorkCode();
	if( strWorkCode.IsEmpty() == TRUE )
	{
		strQuery.Format(_T("REC_KEY IN ")
						_T("(SELECT SPECIES_KEY ")
						_T("FROM BO_BOOK_TBL ")
						_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_CODE='1' ")
						_T("AND ACQ_YEAR='%s' ")
						_T("AND SEQ_NO=%s ")
						_T("AND (WORKING_STATUS < 'BOC' OR WORKING_STATUS = 'BOR111N')) ")
						_T("AND WORK_CODE IS NULL "), m_pSourceGroup->GetAcqYear(), m_pSourceGroup->GetGroupInfo());
	}
	else
		strQuery.Format(
			_T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_CODE='1' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND (WORKING_STATUS < 'BOC' OR WORKING_STATUS = 'BOR111N')) AND WORK_CODE = '%s'"),
			m_pSourceGroup->GetAcqYear(), m_pSourceGroup->GetGroupInfo(), strWorkCode);
	m_pDM_IDX->RefreshDataManager(strQuery);
	m_pDM_SPECIES->RefreshDataManager(strQuery);

	if(m_pDM_IDX->GetRowCount() != m_pDM_SPECIES->GetRowCount())
	{
		m_pDM_IDX->StartFrame();
		if(m_pDM_IDX->GetRowCount() > m_pDM_SPECIES->GetRowCount())
		{
			strTmpQuery.Format(_T("UPDATE BO_SPECIES_TBL ")
							_T("SET WORK_CODE='%s', LAST_WORK='%s' ")
							_T("WHERE REC_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE %s);"), strWorkCode, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strQuery);
			m_pDM_IDX->AddFrame(strTmpQuery);
			ids = m_pDM_IDX->SendFrame();
			if(ids < 0) return -1;
			ids = m_pDM_IDX->EndFrame();
			if(ids < 0) return -1;
			m_pDM_SPECIES->RefreshDataManager(strQuery);
		}
		else
		{
			strTmpQuery.Format(_T("UPDATE IDX_BO_TBL SET WORK_CODE='%s', LAST_WORK='%s' ")
							_T("WHERE REC_KEY IN (SELECT REC_KEY FROM BO_SPECIES_TBL WHERE %s);"), strWorkCode, m_pParentMgr->GetWorkLogMsg(WORK_LOG), strQuery);
			m_pDM_IDX->AddFrame(strTmpQuery);
			ids = m_pDM_IDX->SendFrame();
			if(ids < 0) return -1;
			ids = m_pDM_IDX->EndFrame();
			if(ids < 0) return -1;
			m_pDM_IDX->RefreshDataManager(strQuery);
		}
		m_pDM_IDX->EndFrame();
	}

	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	
	CString strNewKey, strOldKey, strMsg;
	
	INT nRowCnt = m_pDM_IDX->GetRowCount();
	INT nSRowCnt = m_pDM_SPECIES->GetRowCount();
	ShowDataProcessingDlg();
	strMsg.Format(_T("%s-%s 차수 자료를 복사중입니다."), m_pSourceGroup->GetAcqYear(), m_pSourceGroup->GetGroupInfo());
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);

	INT nQueryUnitCnt = 50;

	m_pDM_BOOK->StartFrame();

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);
		strOldKey = m_pDM_IDX->GetCellData(_T("IBS_REC_KEY"), i);
		m_pDM_BOOK->MakeRecKey(strNewKey);
		m_pDM_IDX->SetCellData(_T("IBS_REC_KEY"), strNewKey, i);
		m_pDM_SPECIES->SetCellData(_T("BS_종KEY"), strNewKey, i);
		m_pDM_SPECIES->SetCellData(_T("BS_최초작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_SPECIES->SetCellData(_T("BS_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_IDX->SetCellData(_T("IBS_최초작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_IDX->SetCellData(_T("IBS_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);

		ids = DeleteAcqUselessMarcTag(m_pDM_SPECIES, i);
		if(ids < 0) continue;
		ids = m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_IDX, m_pDM_IDX->TBL_NAME, i, m_pDM_BOOK);
		if(ids < 0) return ids;
		ids = m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_SPECIES, m_pDM_SPECIES->TBL_NAME, i, m_pDM_BOOK);	
		if(ids < 0) return ids;
		if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strTmpQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strNewKey);
			m_pDM_BOOK->AddFrame(strTmpQuery);
		}
		CopyPURCHASEACQData(strNewKey, strOldKey);
		if(i%nQueryUnitCnt == 0)
		{
			ids = m_pDM_BOOK->SendFrame();	
			if(ids < 0) return -1;
			ids = m_pDM_BOOK->EndFrame();
			if(ids < 0) return -1;
			
			if(ids < 0)
			{
				DeleteDataProcessingDlg();
				return -1;
			}
			m_pDM_BOOK->StartFrame();
		}
	}
	
	ids = m_pDM_BOOK->SendFrame();
	if(ids < 0) return -1;
	ids = m_pDM_BOOK->EndFrame();
	if(ids < 0) return -1;
	
	if(ids < 0)
	{
		DeleteDataProcessingDlg();
		return -1;
	}

	m_pDM_BOOK->StartFrame();
	m_pTargetGruop->SetRecKey(_T("1"), m_pDM_BOOK);
	m_pSourceGroup->SetRecKey(_T("1"), m_pDM_BOOK);
	strQuery.Format(_T("PURCHASE_SEQ_NO_KEY=%s"), m_pSourceGroup->GetRecKey());
	m_pDM_WORKNO->RefreshDataManager(strQuery);
	strQuery.Format(_T("DELETE FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY=%s;"), m_pTargetGruop->GetRecKey());
	m_pDM_BOOK->AddFrame(strQuery);
	RemoveAliasList.RemoveAll();
	nRowCnt = m_pDM_WORKNO->GetRowCount();
	
	strMsg.Format(_T("%s-%s 차수에 작업번호정보를 구축하고 있습니다."), m_pTargetGruop->GetAcqYear(), m_pTargetGruop->GetGroupInfo());
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);

	for(i=0 ; i<nRowCnt ; i++)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);
		
		m_pDM_BOOK->MakeRecKey(strNewKey);
		m_pDM_WORKNO->SetCellData(_T("작업번호KEY"), strNewKey, i);
		m_pDM_WORKNO->SetCellData(_T("구입차수KEY"), m_pTargetGruop->GetRecKey(), i);
		m_pDM_WORKNO->SetCellData(_T("최초작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_WORKNO->SetCellData(_T("마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_WORKNO->SetCellData(_T("작업번호생성일"), GetTodayDate(), i);
		m_pDM_WORKNO->SetCellData(_T("작업번호생성자"), m_pParentMgr->GetUserID(), i);
		m_pDM_WORKNO->MakeQueryByDM(RemoveAliasList, m_pDM_WORKNO, m_pDM_WORKNO->TBL_NAME, i, m_pDM_BOOK);
	}
	INT nLastWorkNo = GetLastWorkNo(m_pDM_BOOK, m_pSourceGroup);
	strNewKey.Format(_T("%d"), nLastWorkNo);
	strQuery.Format(_T("UPDATE BO_PURCHASE_SEQ_NO_TBL SET LAST_WORK_NO=%s WHERE REC_KEY=%s;"), strNewKey, m_pTargetGruop->GetRecKey());
	m_pDM_BOOK->AddFrame(strQuery);

	strMsg.Format(_T("%s-%s 차수에 작업번호정보를 생성중입니다."), m_pTargetGruop->GetAcqYear(), m_pTargetGruop->GetGroupInfo());
	m_pDataProcessingDlg->SetMessage(strMsg);
	ids = m_pDM_BOOK->SendFrame();
	DeleteDataProcessingDlg();
	if(ids < 0) return ids;
	ids = m_pDM_BOOK->EndFrame();
	if(ids < 0) return ids;

	return ids;
}

INT CGroupPair::UpdateSpeciesMarcbyStream(CESL_DataMgr *pDM_OUT, CString strTBL, CString strSpecies_key, CString strMarc)
{
	if (!pDM_OUT) return-1;
	
	INT ids = -1;
	if(strMarc.IsEmpty()) return 0;
	
	pDM_OUT->InitDBFieldData();
	ids = pDM_OUT->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc, 1);
	if(ids < 0) return ids;
	ids = pDM_OUT->MakeUpdateFrame(strTBL, _T("REC_KEY"), _T("NUMERIC"), strSpecies_key);
	if(ids < 0) return ids;
	return 0;	
}

INT CGroupPair::CopyReceiptGroup()
{
	INT ids = SetDataMgr(m_pDM_IDX, _T("DMUP_단행_종_색인정보"));
	if(ids < 0) return -1;
	ids = SetDataMgr(m_pDM_SPECIES, _T("DMUP_단행_종_종정보"));
	if(ids < 0) return -1;
	ids = SetDataMgr(m_pDM_BOOK, _T("DMUP_단행_종_책정보"));
	if(ids < 0) return -1;
	ids = SetDataMgr(m_pDM_ACCU, _T("DMUP_단행_종_기증정보"));
	if(ids < 0) return -1;

	CString strQuery, strTmpQuery;
	strQuery.Format(
		_T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_CODE='2' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND (WORKING_STATUS < 'BOC' OR WORKING_STATUS = 'BOR111N'))"),
		m_pSourceGroup->GetAcqYear(), m_pSourceGroup->GetGroupInfo());
	m_pDM_IDX->RefreshDataManager(strQuery);
	m_pDM_SPECIES->RefreshDataManager(strQuery);

	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	
	CString strNewKey, strOldKey, strMsg;

	INT nRowCnt = m_pDM_IDX->GetRowCount();
	ShowDataProcessingDlg();
	strMsg.Format(_T("%s-%s 접수번호 자료를 복사중입니다."), m_pSourceGroup->GetAcqYear(), m_pSourceGroup->GetGroupInfo());
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);

	INT nQueryUnitCnt = 50;
	
	m_pDM_BOOK->StartFrame();
	
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);
		strOldKey = m_pDM_IDX->GetCellData(_T("IBS_REC_KEY"), i);
		m_pDM_BOOK->MakeRecKey(strNewKey);
		m_pDM_IDX->SetCellData(_T("IBS_REC_KEY"), strNewKey, i);
		m_pDM_SPECIES->SetCellData(_T("BS_종KEY"), strNewKey, i);
		m_pDM_SPECIES->SetCellData(_T("BS_최초작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_SPECIES->SetCellData(_T("BS_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_IDX->SetCellData(_T("IBS_최초작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_IDX->SetCellData(_T("IBS_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);

		ids = DeleteAcqUselessMarcTag(m_pDM_SPECIES, i);
		if(ids < 0) continue;
		ids = m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_IDX, m_pDM_IDX->TBL_NAME, i, m_pDM_BOOK);
		if(ids < 0) return ids;
		ids = m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_SPECIES, m_pDM_SPECIES->TBL_NAME, i, m_pDM_BOOK);	
		if(ids < 0) return ids;
		if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strTmpQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strNewKey);
			m_pDM_BOOK->AddFrame(strTmpQuery);
		}
		CopyDONACQData(strNewKey, strOldKey);
		if(i%nQueryUnitCnt == 0)
		{
			ids = m_pDM_BOOK->SendFrame();	
			if(ids < 0) return ids;
			ids = m_pDM_BOOK->EndFrame();
			if(ids < 0) return ids;

			if(ids < 0)
			{
				DeleteDataProcessingDlg();
				return -1;
			}
			m_pDM_BOOK->StartFrame();
		}		
	}

	ids = m_pDM_BOOK->SendFrame();
	DeleteDataProcessingDlg();
	if(ids < 0) return -1;
	ids = m_pDM_BOOK->EndFrame();
	if(ids < 0) return ids;
	
	return 0;
}

INT CGroupPair::SetDataMgr(CESL_DataMgr * pDM, CString strAlias)
{
	pDM->SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
	pDM->InitDMPKFromFile(strAlias);
	pDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);
	return 0;
}

CString CGroupPair::FindDMPK(CString strDMAlias)
{
	CString strFileName;
	strFileName.Format(_T("..\\CFG\\Reference\\DM\\DMPK.CFG"));
	CStdioFile fd;
	if (fd.Open(strFileName, CStdioFile::modeRead | CStdioFile::shareDenyWrite | CFile::typeBinary) == 0) return -1;	// 정보파일을 찾을 수 없음
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	TCHAR szBuf[16000];
	CString strLine;
	INT pos;
	

	INT nStart = 0;
	CString PK, ALIAS;
	CString result = _T("");
	while(TRUE) {
		if (fd.ReadString(szBuf, 16000) == NULL) break;
		if (szBuf[0] == '#') continue;
		strLine.Format(_T("%s"), szBuf);
		strLine.TrimLeft();			strLine.TrimRight();
		
		nStart = 0;

		pos = strLine.Find('|', nStart);
		PK = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		pos = strLine.Find('|', nStart);
		ALIAS = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		PK.TrimLeft();		PK.TrimRight();
		ALIAS.TrimLeft();	ALIAS.TrimRight();

		if (ALIAS == strDMAlias) {
			result = PK;
			break;
		}
	}
	fd.Close();
	return result;
}

INT CGroupPair::MergeGroup()
{
	INT ids;

	switch( m_nGroupType )
	{
	case GROUP_NO:		ids = MergeAcqObjGroup();
		break;
	case DIVNO:			ids = MergeDivGroup();
		break;
	case RECEIPT_NO:	ids = MergeReceiptGroup();
		break;
	case OTHER_GROUP_NO:ids = MergeOtherGroup();
		break;
	default:			ids = -1;
		break;
	}

	return ids;
}

INT CGroupPair::UpdateAcqData(CString strAcqCode, CGroupInfo *pGroupInfo)
{
	CString strQuery, strGroupNo, strAcqYear;
	
	if(strAcqCode == _T("1"))
		strQuery.Format(_T("ORDER_YEAR='%s' AND ORDER_SEQ_NO=%s"), pGroupInfo->GetAcqYear(), pGroupInfo->GetGroupInfo());
	else if(strAcqCode == _T("2"))
		strQuery.Format(_T("ACQ_YEAR='%s' AND RECEIPT_NO=%s"), pGroupInfo->GetAcqYear(), pGroupInfo->GetGroupInfo());

	m_pDM_ACCU->RefreshDataManager(strQuery);

	if(strAcqCode == _T("1"))
	{
		CString strAlias[2] = {_T("BP_그룹번호"), _T("BP_작업번호")};
		INT nCond[2] = {ESL_DATAMGR_SORT_ASC, ESL_DATAMGR_SORT_ASC};
		m_pDM_ACCU->SORT(2, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	}
	else if(strAcqCode == _T("2"))
		m_pDM_ACCU->SORT(_T("BP_그룹번호"));

	strAcqYear = m_pTargetGruop->GetAcqYear();
	strGroupNo = m_pTargetGruop->GetGroupInfo();

	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BP_수서정보KEY"));
	
	INT nLastSerialNo, nLastWorkNo;
	if(strAcqCode == _T("1"))
	{
		nLastSerialNo = GetLastSerialNo(m_pDM_ACCU, m_pTargetGruop);
		nLastWorkNo = GetLastWorkNo(m_pDM_ACCU, m_pTargetGruop);
	}
	INT nRowCnt = m_pDM_ACCU->GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}

	INT nTmpData;
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(m_pDataProcessingDlg)
		{
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);
		}
		
		m_pDM_ACCU->SetCellData(_T("BP_수입년도"), strAcqYear, i);
		m_pDM_ACCU->SetCellData(_T("BP_그룹번호"), strGroupNo, i);
		if(strAcqCode == _T("1"))
		{
			nTmpData = _ttoi((m_pDM_ACCU->GetCellData(_T("BP_작업번호"), i)).GetBuffer(0));
			nTmpData += nLastWorkNo;
			strTmpData.Format(_T("%d"), nTmpData);
			m_pDM_ACCU->SetCellData(_T("BP_작업번호"), strTmpData, i);
			nTmpData = _ttoi((m_pDM_ACCU->GetCellData(_T("BP_일련번호"), i)).GetBuffer(0));
			nTmpData += nLastSerialNo;
			strTmpData.Format(_T("%d"), nTmpData);
			m_pDM_ACCU->SetCellData(_T("BP_일련번호"), strTmpData, i);
		}
		strTmpData = m_pDM_ACCU->GetCellData(_T("BP_수서정보KEY"), i);
		m_pDM_ACCU->MakeQueryByDM(RemoveAliasList, m_pDM_ACCU, m_pDM_ACCU->TBL_NAME, i, m_pDM_BOOK, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
	}
	return 0;
}

INT CGroupPair::UpdateBookData(CString strAcqCode, CGroupInfo *pGroupInfo, CString strAcqKey/*=_T("")*/)
{
	CString strQuery, strGroupNo, strAcqYear;
	if(strAcqCode == _T("0")) strAcqCode.Empty();
	if(strAcqCode.IsEmpty()) strGroupNo = pGroupInfo->GetGroupNo();
	else strGroupNo = pGroupInfo->GetGroupInfo();
	
	if(strAcqCode.IsEmpty())
	{
		if(m_bDeleteDataYN)
			strQuery.Format(
				_T("ACQ_YEAR='%s' AND SEQ_NO=%s"),
				pGroupInfo->GetAcqYear(), strGroupNo);
		else
			strQuery.Format(
				_T("(ACQ_CODE IS NULL OR ACQ_CODE = '0') AND ACQ_YEAR='%s' AND SEQ_NO=%s"),
				pGroupInfo->GetAcqYear(), strGroupNo);
	}
	else
		strQuery.Format(
			_T("ACQ_CODE='%s' AND ACQ_YEAR='%s' AND SEQ_NO=%s"),
			strAcqCode, pGroupInfo->GetAcqYear(), strGroupNo);
	
	if(!strAcqKey.IsEmpty()) strQuery += _T(" AND ACQ_KEY=") + strAcqKey;
	if(strAcqCode == _T("1"))
	{
		CString strWorkCode = pGroupInfo->GetWorkCode();
		if( strWorkCode.IsEmpty() == TRUE )	strQuery += _T(" AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE WORK_CODE IS NULL )");
		else								strQuery += _T(" AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE WORK_CODE = '") + strWorkCode + _T("')");
	}

	INT ids = m_pDM_BOOK->RefreshDataManager(strQuery);
	if(ids < 0) return -1;
	m_pDM_BOOK->SORT(_T("BB_일련번호"));
	INT nSerialNo = -1;
	if(strAcqCode == _T("1")) nSerialNo = m_pTargetGruop->GetBookLastSerialNo();
	else nSerialNo = m_pTargetGruop->GetLastSerialNo();
	
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BB_책KEY"));
	RemoveAliasList.Add(_T("BB_원본등록번호"));

	INT nRowCnt = m_pDM_BOOK->GetRowCount();
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}

	CString strTmpData;
	if(strAcqCode.IsEmpty()) strGroupNo = m_pTargetGruop->GetGroupNo();
	else strGroupNo = m_pTargetGruop->GetGroupInfo();
	strAcqYear = m_pTargetGruop->GetAcqYear();

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(m_pDataProcessingDlg)
		{
			m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);
		}

		strTmpData.Format(_T("%d"), ++nSerialNo);
		m_pDM_BOOK->SetCellData(_T("BB_일련번호"), strTmpData, i);
		m_pDM_BOOK->SetCellData(_T("BB_그룹번호"), strGroupNo, i);
		m_pDM_BOOK->SetCellData(_T("BB_수입년도"), strAcqYear, i);
		strTmpData = m_pDM_BOOK->GetCellData(_T("BB_책KEY"), i);
		m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_BOOK, m_pDM_BOOK->TBL_NAME, i, m_pDM_BOOK, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
	}
	if(strAcqCode == _T("1")) m_pTargetGruop->SetBookLastSerialNo(nSerialNo);
	else m_pTargetGruop->SetLastSerialNo(nSerialNo);
	return 0;
}

INT CGroupPair::MergeAcqObjGroup()
{
	CString strDMAlias;
	if(m_bDeleteDataYN)	strDMAlias = _T("DMUP_단행_종API_삭제책정보");
	else strDMAlias = _T("DMUP_단행_종_책정보");
	INT ids = SetDataMgr(m_pDM_BOOK, strDMAlias);
	if(ids < 0) return -1;

	CGroupInfo * pGroupInfo = NULL;
	CString strQuery, strMsg;
	ids = GetBookLastSerialNo(m_pDM_BOOK, _T("0"), m_pTargetGruop);
	m_pTargetGruop->SetLastSerialNo(ids);
	
	INT nGroupCnt = m_pSourceGroupCollection->GetGroupCount();
	
	ShowDataProcessingDlg();

	for(INT i=0 ; i<nGroupCnt ; i++)
	{
		m_pDM_BOOK->StartFrame();
		pGroupInfo = m_pSourceGroupCollection->GetGroupInfo(i);
		if(m_pTargetGruop->IsEqual(pGroupInfo)) continue;
		strMsg.Format(_T("%s 그룹을 통합하고 있습니다."), pGroupInfo->GetGroupInfo());
		m_pDataProcessingDlg->SetMessage(strMsg);
		UpdateBookData(_T("0"), pGroupInfo);
		pGroupInfo->SetRecKey(_T("0"), m_pDM_BOOK);
		strQuery.Format(_T("DELETE FROM BO_ACQ_GROUP_TBL WHERE REC_KEY=%s;"), pGroupInfo->GetRecKey());
		m_pDM_BOOK->AddFrame(strQuery);

		strMsg = _T("그룹통합 작업을 수행중입니다.");
		m_pDataProcessingDlg->SetMessage(strMsg);
		ids = m_pDM_BOOK->SendFrame();
		if(ids < 0) return -1;
		ids = m_pDM_BOOK->EndFrame();
		if(ids < 0) return ids;

		DeleteGroupInMainDM(pGroupInfo->GetRecKey(), _T("그룹KEY"));
	}
	DeleteDataProcessingDlg();

	m_pDM_BOOK->StartFrame();
	m_pTargetGruop->UpdateLastSerialNo(_T(""), m_pParentMgr, m_pDM_BOOK);
	ids = m_pDM_BOOK->SendFrame();
	if(ids < 0) return ids;
	ids = m_pDM_BOOK->EndFrame();
	if(ids < 0) return ids;

	UpdateGroupBookCnt(m_pTargetGruop);
	return ids;
}

INT CGroupPair::MergeDivGroup()
{
	INT i, ids, nLastSerialNo, nLastWorkNo, nGroupCnt;
	CString strMsg;
	CGroupInfo* pGroupInfo = NULL;

	ids = SetDataMgr(m_pDM_BOOK, _T("DMUP_단행_종_책정보"));
	if( ids < 0 )	return -1;
	ids = SetDataMgr(m_pDM_ACCU, _T("DMUP_단행_종_구입정보"));
	if( ids < 0 )	return -1;
	ids = SetDataMgr(m_pDM_WORKNO, _T("DM_작업번호관리"));
	if( ids < 0 )	return -1;

	ids = GetBookLastSerialNo(m_pDM_BOOK, _T("1"), m_pTargetGruop);
	m_pTargetGruop->SetLastSerialNo(ids);
	
	nLastSerialNo = GetLastSerialNo(m_pDM_ACCU, m_pTargetGruop);
	nLastWorkNo = GetLastWorkNo(m_pDM_ACCU, m_pTargetGruop);
	m_pTargetGruop->SetRecKey(_T("1"), m_pDM_WORKNO);

	nGroupCnt = m_pSourceGroupCollection->GetGroupCount();
	ShowDataProcessingDlg();
	for( i=0 ; i<nGroupCnt ; i++ )
	{
		m_pDM_BOOK->StartFrame();
		pGroupInfo = m_pSourceGroupCollection->GetGroupInfo(i);
		if( m_pTargetGruop->IsEqual(pGroupInfo) )	continue;
		
		strMsg.Format(_T("%s-%s 차수정보를 통합하고 있습니다."), pGroupInfo->GetAcqYear(), pGroupInfo->GetGroupInfo());
		m_pDataProcessingDlg->SetMessage(strMsg);
		
		UpdateDivnoAcqData(pGroupInfo, nLastSerialNo, nLastWorkNo);
		UpdateBookData(_T("1"), pGroupInfo);
		
		strMsg = _T("차수통합 작업 수행중입니다.");
		m_pDataProcessingDlg->SetMessage(strMsg);
		ids = m_pDM_BOOK->SendFrame();
		if(ids < 0) return -1;
		ids = m_pDM_BOOK->EndFrame();
		if(ids < 0) return -1;

		DeleteGroupInMainDM(pGroupInfo->GetRecKey(), _T("구입차수KEY"));
	}
	DeleteDataProcessingDlg();
	
	CString strTmpData;
	strTmpData.Format(_T("%d"), nLastWorkNo);
	m_pTargetGruop->SetLastWorkNo(strTmpData);
	m_pDM_BOOK->StartFrame();
	m_pTargetGruop->UpdateLastWorNo(m_pParentMgr, m_pDM_BOOK);
	ids = m_pDM_BOOK->SendFrame();
	if(ids < 0) return ids;
	ids = m_pDM_BOOK->EndFrame();
	if(ids < 0) return ids;

	UpdateGroupBookCnt(m_pTargetGruop);
	return ids;
}

INT CGroupPair::MergeReceiptGroup()
{
	INT i, ids, nGroupCnt;
	CString strQuery, strMsg;
	CGroupInfo* pGroupInfo = NULL;

	ids = SetDataMgr(m_pDM_BOOK, _T("DMUP_단행_종_책정보"));
	if( ids < 0 )	return -1;
	ids = SetDataMgr(m_pDM_ACCU, _T("DMUP_단행_종_기증정보"));
	if( ids < 0 )	return -1;

	ids = GetBookLastSerialNo(m_pDM_BOOK, _T("2"), m_pTargetGruop);
	m_pTargetGruop->SetLastSerialNo(ids);
	nGroupCnt = m_pSourceGroupCollection->GetGroupCount();

	ShowDataProcessingDlg();
	
	for( i=0 ; i<nGroupCnt ; i++ )
	{
		m_pDM_BOOK->StartFrame();
		pGroupInfo = m_pSourceGroupCollection->GetGroupInfo(i);
		if( m_pTargetGruop->IsEqual(pGroupInfo) )	continue;

		strMsg.Format(_T("%s-%s 접수번호를 통합하고 있습니다."), pGroupInfo->GetAcqYear(), pGroupInfo->GetGroupInfo());
		m_pDataProcessingDlg->SetMessage(strMsg);

		UpdateAcqData(_T("2"), pGroupInfo);
		UpdateBookData(_T("2"), pGroupInfo);
		pGroupInfo->SetRecKey(_T("2"), m_pDM_BOOK);
		strQuery.Format(_T("DELETE FROM BO_DON_RECEIPT_TBL WHERE REC_KEY=%s;"), pGroupInfo->GetRecKey());
		m_pDM_BOOK->AddFrame(strQuery);

		strMsg = _T("접수번호 통합 작업 수행중입니다.");
		m_pDataProcessingDlg->SetMessage(strMsg);
		ids = m_pDM_BOOK->SendFrame();
		if(ids < 0) return -1;
		ids = m_pDM_BOOK->EndFrame();
		if(ids < 0) return -1;

		DeleteGroupInMainDM(pGroupInfo->GetRecKey(), _T("접수번호KEY"));
	}
	DeleteDataProcessingDlg();

	m_pDM_BOOK->StartFrame();

	m_pTargetGruop->UpdateLastSerialNo(_T("2"), m_pParentMgr, m_pDM_BOOK);
	ids = m_pDM_BOOK->SendFrame();
	if(ids < 0) return ids;
	ids = m_pDM_BOOK->EndFrame();
	if(ids < 0) return ids;

	UpdateGroupBookCnt(m_pTargetGruop);
	return 0;
}

INT CGroupPair::GetBookLastSerialNo(CESL_DataMgr * pDM, CString strAcqCode, CGroupInfo *pGroupInfo)
{
	CString strQuery, strGroupNo;
	if(strAcqCode == _T("0")) strGroupNo = pGroupInfo->GetGroupNo();
	else strGroupNo = pGroupInfo->GetGroupInfo();

	if(strAcqCode == _T("0") || strAcqCode.IsEmpty())
		strQuery.Format(
			_T("SELECT MAX(SERIAL_NO) FROM BO_BOOK_TBL WHERE (ACQ_CODE='0' OR ACQ_CODE IS NULL) AND ACQ_YEAR='%s' AND SEQ_NO=%s AND MANAGE_CODE = UDF_MANAGE_CODE"),

			strAcqCode, pGroupInfo->GetAcqYear(), strGroupNo);
	else
		strQuery.Format(
			_T("SELECT MAX(SERIAL_NO) FROM BO_BOOK_TBL WHERE ACQ_CODE='%s' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND MANAGE_CODE = UDF_MANAGE_CODE"),

			strAcqCode, pGroupInfo->GetAcqYear(), strGroupNo);
	CString strResult;
	pDM->GetOneValueQuery(strQuery, strResult);
	if(strResult.IsEmpty()) return 0;
	return _ttoi(strResult.GetBuffer(0));
}

INT CGroupPair::GetLastSerialNo(CESL_DataMgr * pDM, CGroupInfo *pGruop)
{
	CString strQuery;
	strQuery.Format(
		_T("SELECT MAX(SERIAL_NO) FROM BO_PURCHASEINFO_TBL WHERE ORDER_YEAR='%s' AND ORDER_SEQ_NO=%s AND MANAGE_CODE = UDF_MANAGE_CODE"),
		pGruop->GetAcqYear(), pGruop->GetGroupInfo());
	CString strResult;
	pDM->GetOneValueQuery(strQuery, strResult);
	if(strResult.IsEmpty()) 0;
	return _ttoi(strResult.GetBuffer(0));
}

INT CGroupPair::GetLastWorkNo(CESL_DataMgr * pDM, CGroupInfo *pGruop)
{
	CString strQuery;
	strQuery.Format(
		_T("SELECT MAX(ORDER_WORK_NO) FROM BO_PURCHASEINFO_TBL WHERE ORDER_YEAR='%s' AND ORDER_SEQ_NO=%s AND MANAGE_CODE = UDF_MANAGE_CODE"),
		pGruop->GetAcqYear(), pGruop->GetGroupInfo());
	CString strResult;
	pDM->GetOneValueQuery(strQuery, strResult);
	if(strResult.IsEmpty()) 0;
	return _ttoi(strResult.GetBuffer(0));
}

INT CGroupPair::DivideGroup()
{
	INT ids = -1;
	if(m_nGroupType == GROUP_NO)
		ids = DivideAcqObjGroup();
	else if(m_nGroupType == DIVNO)
		ids = DivideDivGroup();
	else if(m_nGroupType == RECEIPT_NO)
		ids = DivideReceiptGroup();
	else if(m_nGroupType == OTHER_GROUP_NO)
		ids = DivideOtherGroup();
	return ids;
}

INT CGroupPair::DivideAcqObjGroup()
{
	CString strDMAlias;
	if(m_bDeleteDataYN)	strDMAlias = _T("DMUP_단행_종API_삭제책정보");
	else strDMAlias = _T("DMUP_단행_종_책정보");
	INT ids = SetDataMgr(m_pDM_BOOK, strDMAlias);
	if(ids < 0) return -1;

	CString strQuery;
	if(m_bDeleteDataYN)
		strQuery = _T("%FIELD% IN (%RECKEY%)");
	else	
		strQuery = _T("%FIELD% IN (%RECKEY%) ORDER BY SERIAL_NO");

	ids = MakeMergeDM(m_pDM_BOOK, m_pArrRecKey, strQuery, _T("SPECIES_KEY"));
	if(ids < 0) return -1;

	INT nLastSerialNo = GetBookLastSerialNo(m_pDM_BOOK, _T("0"), m_pTargetGruop);
	
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BB_책KEY"));
	RemoveAliasList.Add(_T("BB_원본등록번호"));

	INT nRowCnt = m_pDM_BOOK->GetRowCount();
	CString strTmpData;
	m_pDM_BOOK->StartFrame();
	ShowDataProcessingDlg();
	strTmpData.Format(_T("선정된 자료의 그룹정보를 %s로 수정중입니다."), m_pTargetGruop->GetGroupInfo());
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);

		m_pDM_BOOK->SetCellData(_T("BB_수입년도"), m_pTargetGruop->GetAcqYear(), i);
		m_pDM_BOOK->SetCellData(_T("BB_그룹번호"), m_pTargetGruop->GetGroupNo(), i);
		m_pDM_BOOK->SetCellData(_T("BB_마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		strTmpData.Format(_T("%d"), ++nLastSerialNo);
		m_pDM_BOOK->SetCellData(_T("BB_일련번호"), strTmpData, i);
		strTmpData = m_pDM_BOOK->GetCellData(_T("BB_책KEY"), i);
		m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_BOOK, m_pDM_BOOK->TBL_NAME, i, m_pDM_BOOK, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
	}
	
	m_pTargetGruop->SetLastSerialNo(nLastSerialNo);
	m_pTargetGruop->UpdateLastSerialNo(_T(""), m_pParentMgr, m_pDM_BOOK);
	strTmpData.Format(_T("자료를 %s 그룹으로 분리중입니다."), m_pTargetGruop->GetGroupInfo());
	m_pDataProcessingDlg->SetMessage(strTmpData);
	ids = m_pDM_BOOK->SendFrame();
	DeleteDataProcessingDlg();
	if(ids < 0) return -1;
	ids = m_pDM_BOOK->EndFrame();
	if(ids < 0) return -1;

	UpdateGroupBookCnt(m_pTargetGruop);

	return 0;
}

INT CGroupPair::DivideDivGroup()
{
	INT ids = SetDataMgr(m_pDM_BOOK, _T("DMUP_단행_종_책정보"));
	if(ids < 0) return -1;
	ids = SetDataMgr(m_pDM_ACCU, _T("DMUP_단행_종_구입정보"));
	if(ids < 0) return -1;
	ids = SetDataMgr(m_pDM_WORKNO, _T("DM_작업번호관리"));
	if(ids < 0) return -1;

	CString strQuery;
	strQuery = _T("%FIELD% IN (%RECKEY%) ORDER BY ORDER_SEQ_NO, ORDER_WORK_NO, ORDER_SERIAL_NO");
	ids = MakeMergeDM(m_pDM_ACCU, m_pArrRecKey, strQuery, _T("REC_KEY"));
	if(ids < 0) return -1;
	
	BOOL bWorkNoUSEYN = FALSE;
	INT nLastSerialNo = GetLastSerialNo(m_pDM_ACCU, m_pTargetGruop);
	INT nLastWorkNo = GetLastWorkNo(m_pDM_ACCU, m_pTargetGruop);
	if(nLastSerialNo != 0 ) bWorkNoUSEYN = TRUE;
	
	INT nRowCnt = m_pDM_ACCU->GetRowCount();
	INT nCnt = -1;
	INT nSerialNo = 1;
	m_pTargetGruop->SetRecKey(_T("1"), m_pDM_ACCU);
	
	CString strPrevWorkNo, strWorkNo, strTmpData, strKey;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strWorkNo = m_pDM_ACCU->GetCellData(_T("BP_작업번호"), i);
		if(strPrevWorkNo != strWorkNo && bWorkNoUSEYN)
		{
			nCnt = m_pDM_WORKNO->GetRowCount() - 1;
			if(nCnt >= 0)
			{
				strTmpData.Format(_T("%d"), nSerialNo);
				m_pDM_WORKNO->SetCellData(_T("마지막일련번호"), strTmpData, nCnt);
			}
			nSerialNo = 1;
			strTmpData.Format(_T("%d"), nSerialNo);
			m_pDM_ACCU->SetCellData(_T("BP_발주일련번호"), strTmpData, i);
			strTmpData.Format(_T("%d"), ++nLastWorkNo);
			m_pDM_ACCU->SetCellData(_T("BP_작업번호"), strTmpData, i);
			m_pDM_WORKNO->InsertRow(-1);
			nCnt = m_pDM_WORKNO->GetRowCount() - 1;
			m_pDM_WORKNO->MakeRecKey(strKey);
			m_pDM_WORKNO->SetCellData(_T("최초작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nCnt);
			m_pDM_WORKNO->SetCellData(_T("마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nCnt);
			m_pDM_WORKNO->SetCellData(_T("구입차수KEY"), m_pTargetGruop->GetRecKey(), nCnt);
			m_pDM_WORKNO->SetCellData(_T("작업번호KEY"), strKey, nCnt);
			m_pDM_WORKNO->SetCellData(_T("작업번호"), strTmpData, nCnt);
			m_pDM_WORKNO->SetCellData(_T("작업번호생성일"), GetTodayDate(), nCnt);
			m_pDM_WORKNO->SetCellData(_T("작업번호생성자"), m_pParentMgr->GetUserID(), nCnt);
		}
		else
		{
			strTmpData.Format(_T("%d"), ++nSerialNo);
			m_pDM_ACCU->SetCellData(_T("BP_발주일련번호"), strTmpData, i);
			
			if( 0 == nLastWorkNo )
			{
				strTmpData.Format(_T("%d"), nLastWorkNo + 1);
			}
			else
			{
				strTmpData.Format(_T("%d"), nLastWorkNo);
			}
			
			m_pDM_ACCU->SetCellData(_T("BP_작업번호"), strTmpData, i);
		}
		strTmpData.Format(_T("%d"), ++nLastSerialNo);
		m_pDM_ACCU->SetCellData(_T("BP_수입년도"), m_pTargetGruop->GetAcqYear(), i);
		m_pDM_ACCU->SetCellData(_T("BP_그룹번호"), m_pTargetGruop->GetGroupInfo(), i);		
		m_pDM_ACCU->SetCellData(_T("BP_마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		strPrevWorkNo = strWorkNo;
	}
	m_pTargetGruop->SetLastSerialNo(nLastSerialNo);
	strTmpData.Format(_T("%d"), nLastWorkNo);
	m_pTargetGruop->SetLastWorkNo(strTmpData);
	
	ids = UpdateBookDataByRecKey();
	if(ids < 0) return -1;
	
	ShowDataProcessingDlg();

	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	nRowCnt = m_pDM_WORKNO->GetRowCount();
	strTmpData.Format(_T("%s-%s 차수에 작업번호를 생성중입니다."), m_pTargetGruop->GetAcqYear(), m_pTargetGruop->GetGroupInfo());
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	
	m_pDM_BOOK->StartFrame();
	for(i=0 ; i<nRowCnt ; i++)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);
		m_pDM_WORKNO->MakeQueryByDM(RemoveAliasList, m_pDM_WORKNO, m_pDM_WORKNO->TBL_NAME, i, m_pDM_BOOK);
	}

	nRowCnt = m_pDM_ACCU->GetRowCount();
	strTmpData.Format(_T("선정된 구입정보의 차수정보를 %s-%s로 수정중입니다. "), m_pTargetGruop->GetAcqYear(), m_pTargetGruop->GetGroupInfo());
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BP_수서정보KEY"));
	for(i=0 ; i<nRowCnt ; i++)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);
		strTmpData = m_pDM_ACCU->GetCellData(_T("BP_수서정보KEY"), i);
		m_pDM_ACCU->MakeQueryByDM(RemoveAliasList, m_pDM_ACCU, m_pDM_ACCU->TBL_NAME, i, m_pDM_BOOK, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
	}
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BB_책KEY"));
	RemoveAliasList.Add(_T("BB_원본등록번호"));
	nRowCnt = m_pDM_BOOK->GetRowCount();
	strTmpData.Format(_T("선정된 책정보의 차수정보를 %s-%s로 수정중입니다. "), m_pTargetGruop->GetAcqYear(), m_pTargetGruop->GetGroupInfo());
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	
	CString strIdxQuery;
	CString str64;
	m_pParentMgr->GetManageValue( _T("기타"), _T("공통"), _T("육사"), _T("육사"), str64 );

	for(i=0 ; i<nRowCnt ; i++)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);
		strTmpData = m_pDM_BOOK->GetCellData(_T("BB_책KEY"), i);
		m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_BOOK, m_pDM_BOOK->TBL_NAME, i, m_pDM_BOOK, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
		
		if ( str64 == _T("Y") )
		{
			strIdxQuery.Format(_T("UPDATE IDX_BO_TBL SET WORK_CODE = '%s' WHERE rec_key in ( select species_key from bo_book_Tbl where REC_KEY = %s ); "), m_pTargetGruop->GetWorkCode(), strTmpData);
			m_pDM_BOOK->AddFrame(strIdxQuery);
		}
	}	
	m_pTargetGruop->UpdateLastWorNo(m_pParentMgr, m_pDM_BOOK);	

	strTmpData.Format(_T("차수정보 분리중입니다."));
	m_pDataProcessingDlg->SetMessage(strTmpData);
	ids = m_pDM_BOOK->SendFrame();
	DeleteDataProcessingDlg();
	if(ids < 0) return ids;
	ids = m_pDM_BOOK->EndFrame();
	if(ids < 0) return ids;

	UpdateGroupBookCnt(m_pTargetGruop);
	return ids;
}

INT CGroupPair::DivideReceiptGroup()
{
	INT ids = SetDataMgr(m_pDM_BOOK, _T("DMUP_단행_종_책정보"));
	if(ids < 0) return -1;
	ids = SetDataMgr(m_pDM_ACCU, _T("DMUP_단행_종_기증정보"));
	if(ids < 0) return -1;

	CString strQuery;
	strQuery = _T("%FIELD% IN (%RECKEY%) ORDER BY SERIAL_NO");
	ids = MakeMergeDM(m_pDM_BOOK, m_pArrRecKey, strQuery, _T("ACQ_KEY"));

	INT nLastSerialNo = GetBookLastSerialNo(m_pDM_BOOK, _T("2"), m_pTargetGruop);
	
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BB_책KEY"));
	RemoveAliasList.Add(_T("BB_원본등록번호"));

	INT nRowCnt = m_pDM_BOOK->GetRowCount();
	INT nRecKeyArrCnt = m_pArrRecKey->GetSize();

	CString strTmpData;
	ShowDataProcessingDlg();
	strTmpData.Format(_T("선정된 자료의 접수번호 정보를 %s-%s로 수정중입니다."), m_pTargetGruop->GetAcqYear(), m_pTargetGruop->GetGroupInfo());
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt+nRecKeyArrCnt);

	m_pDM_BOOK->StartFrame();
	
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);

		m_pDM_BOOK->SetCellData(_T("BB_수입년도"), m_pTargetGruop->GetAcqYear(), i);
		m_pDM_BOOK->SetCellData(_T("BB_그룹번호"), m_pTargetGruop->GetGroupInfo(), i);
		m_pDM_BOOK->SetCellData(_T("BB_마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		strTmpData.Format(_T("%d"), ++nLastSerialNo);
		m_pDM_BOOK->SetCellData(_T("BB_일련번호"), strTmpData, i);
		strTmpData = m_pDM_BOOK->GetCellData(_T("BB_책KEY"), i);
		m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_BOOK, m_pDM_BOOK->TBL_NAME, i, m_pDM_BOOK, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
	}
	
	CString strRecKey;
	for(INT j=0 ; j<nRecKeyArrCnt ; j++)
	{
		strRecKey = GetRecKeyList(m_pArrRecKey->GetAt(j));
		strQuery.Format(
			_T("UPDATE BO_DONINFO_TBL SET ACQ_YEAR='%s', RECEIPT_NO=%s, LAST_WORK='%s' ")
			_T("WHERE REC_KEY IN (%s);"), 
			m_pTargetGruop->GetAcqYear(), m_pTargetGruop->GetGroupInfo(), 
			m_pParentMgr->GetWorkLogMsg(WORK_LOG), strRecKey);
		m_pDM_BOOK->AddFrame(strQuery);
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+j+1);
	}
	m_pTargetGruop->SetLastSerialNo(nLastSerialNo);
	m_pTargetGruop->UpdateLastSerialNo(_T("2"), m_pParentMgr, m_pDM_BOOK);
	strTmpData.Format(_T("자료를 %s-%s 접수번호로 분리중입니다."), m_pTargetGruop->GetAcqYear(), m_pTargetGruop->GetGroupInfo());
	m_pDataProcessingDlg->SetMessage(strTmpData);
	ids = m_pDM_BOOK->SendFrame();
	DeleteDataProcessingDlg();
	if(ids < 0) return -1;
	ids = m_pDM_BOOK->EndFrame();
	if(ids < 0) return -1;

	UpdateGroupBookCnt(m_pTargetGruop);

	return 0;
}

CString CGroupPair::GetRecKeyList(CStringArray * pArr/*=NULL*/)
{
	CStringArray * pTmpArr = NULL;
	if(pArr == NULL) pTmpArr = m_pSourceData;
	else pTmpArr = pArr;
	INT cnt = pTmpArr->GetSize();
	CString strData;
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strData.IsEmpty()) strData = pTmpArr->GetAt(i);
		else strData += _T(", ") + pTmpArr->GetAt(i);
	}
	return strData;
}

CString CGroupPair::GetTodayDate()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

INT CGroupPair::UpdateBookDataByRecKey(CString strReckey/*=_T("")*/)
{
	CString strQuery;
	strQuery = _T("%FIELD% IN (%RECKEY%) ORDER BY SERIAL_NO");
	INT ids = MakeMergeDM(m_pDM_BOOK, m_pArrRecKey, strQuery, _T("ACQ_KEY"));
	if(ids < 0) return -1;	
	
	INT nLastSerialNo = GetBookLastSerialNo(m_pDM_BOOK, _T("1"), m_pTargetGruop);

	INT nRowCnt = m_pDM_BOOK->GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		m_pDM_BOOK->SetCellData(_T("BB_그룹번호"), m_pTargetGruop->GetGroupInfo(), i);
		m_pDM_BOOK->SetCellData(_T("BB_수입년도"), m_pTargetGruop->GetAcqYear(), i);
		strTmpData.Format(_T("%d"), ++nLastSerialNo);
		m_pDM_BOOK->SetCellData(_T("BB_일련번호"), strTmpData, i);
		m_pDM_BOOK->SetCellData(_T("BB_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
	}
	return 0;
}

INT CGroupPair::CopyPURCHASEACQData(CString strNewKey, CString strOldKey)
{
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	CString strQuery;
	strQuery.Format(
		_T("SPECIES_KEY=%s AND ORDER_YEAR='%s' AND ORDER_SEQ_NO=%s"),
		strOldKey, m_pSourceGroup->GetAcqYear(), m_pSourceGroup->GetGroupInfo());

	m_pDM_ACCU->RefreshDataManager(strQuery);
	INT nRowCnt = m_pDM_ACCU->GetRowCount();
	CString strNewAcqKey, strOldAcqKey;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strOldAcqKey = m_pDM_ACCU->GetCellData(_T("BP_수서정보KEY"), i);
		m_pDM_BOOK->MakeRecKey(strNewAcqKey);
		m_pDM_ACCU->SetCellData(_T("BP_수서정보KEY"), strNewAcqKey, i);
		m_pDM_ACCU->SetCellData(_T("BP_종KEY"), strNewKey, i);
		m_pDM_ACCU->SetCellData(_T("BP_수입년도"), m_pTargetGruop->GetAcqYear(), i);
		m_pDM_ACCU->SetCellData(_T("BP_그룹번호"), m_pTargetGruop->GetGroupInfo(), i);
		m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_ACCU, m_pDM_ACCU->TBL_NAME, i, m_pDM_BOOK);
		CopyBookData(_T("1"), strNewKey, strOldKey, strNewAcqKey, strOldAcqKey);
	}
	return 0;	
}

INT CGroupPair::UpdateDIVNOGroupInfo(CGroupInfo *pGroupInfo)
{
	pGroupInfo->SetRecKey(_T("1"), m_pDM_WORKNO);
	m_pTargetGruop->SetRecKey(_T("1"), m_pDM_WORKNO);
	CString strQuery;
	strQuery.Format(_T("PURCHASE_SEQ_NO_KEY=%s"), pGroupInfo->GetRecKey());
	m_pDM_WORKNO->RefreshDataManager(strQuery);
	INT nRowCnt = m_pDM_WORKNO->GetRowCount();
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("작업번호KEY"));
	CString strTmpData;
	
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		m_pDM_WORKNO->SetCellData(_T("구입차수KEY"), m_pTargetGruop->GetRecKey(), i);
		m_pDM_WORKNO->SetCellData(_T("마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		strTmpData = m_pDM_WORKNO->GetCellData(_T("작업번호KEY"), i);
		m_pDM_WORKNO->MakeQueryByDM(RemoveAliasList, m_pDM_WORKNO, m_pDM_WORKNO->TBL_NAME, i, m_pDM_BOOK, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
	}
	strQuery.Format(_T("DELETE FROM BO_PURCHASE_SEQ_NO_TBL WHERE REC_KEY = %s;"), pGroupInfo->GetRecKey());
	m_pDM_BOOK->AddFrame(strQuery);
	return 0;
}

INT CGroupPair::UpdateDivnoAcqData(CGroupInfo *pGroupInfo, INT &nLastSerialNo, INT &nLastWorkNo)
{
	INT i, nRowCnt, nIdx;
	INT nCond[2] = {ESL_DATAMGR_SORT_ASC, ESL_DATAMGR_SORT_ASC};
	CString strQuery, strGroupNo, strAcqYear, strWorkCode, strKey, strTmpData, strPervWorkNo, strWorkNo;
	CString strAlias[2] = {_T("BP_그룹번호"), _T("BP_작업번호")};
	CArray <CString, CString> RemoveAliasList;	
	
	strWorkCode = pGroupInfo->GetWorkCode();
	if( strWorkCode.IsEmpty() == TRUE )
	{
		strQuery.Format(_T("ORDER_YEAR='%s' AND ORDER_SEQ_NO=%s AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE WORK_CODE IS NULL)"), pGroupInfo->GetAcqYear(), pGroupInfo->GetGroupInfo());
	}
	else
	{
		strQuery.Format(_T("ORDER_YEAR='%s' AND ORDER_SEQ_NO=%s AND SPECIES_KEY IN (SELECT REC_KEY FROM IDX_BO_TBL WHERE WORK_CODE = '%s')"), pGroupInfo->GetAcqYear(), pGroupInfo->GetGroupInfo(), strWorkCode);
	}
	m_pDM_ACCU->RefreshDataManager(strQuery);

	pGroupInfo->SetRecKey(_T("1"), m_pDM_WORKNO);
	strKey = pGroupInfo->GetRecKey();
	if( strKey.IsEmpty() == TRUE )	strQuery.Format(_T("PURCHASE_SEQ_NO_KEY IS NULL ORDER BY WORK_NO"));
	else							strQuery.Format(_T("PURCHASE_SEQ_NO_KEY='%s' ORDER BY WORK_NO"), strKey);
	m_pDM_WORKNO->RefreshDataManager(strQuery);

	
	m_pDM_ACCU->SORT(2, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	
	strAcqYear = m_pTargetGruop->GetAcqYear();
	strGroupNo = m_pTargetGruop->GetGroupInfo();

	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BP_수서정보KEY"));
	
	nRowCnt = m_pDM_ACCU->GetRowCount();
	if( m_pDataProcessingDlg )
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	}

	for( i=0 ; i<nRowCnt ; i++ )
		{
		if( m_pDataProcessingDlg )		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);

		m_pDM_ACCU->SetCellData(_T("BP_수입년도"), strAcqYear, i);
		m_pDM_ACCU->SetCellData(_T("BP_그룹번호"), strGroupNo, i);
		
		strWorkNo = m_pDM_ACCU->GetCellData(_T("BP_작업번호"), i);
		if( strWorkNo != strPervWorkNo )
		{
			strPervWorkNo = strWorkNo;
			strTmpData.Format(_T("%d"), ++nLastWorkNo);
			nIdx = FindRowIdx(_T("작업번호"), strWorkNo, m_pDM_WORKNO);
			if( nIdx >= 0 )
			{
				m_pDM_WORKNO->SetCellData(_T("작업번호"), strTmpData, nIdx);
				m_pDM_WORKNO->SetCellData(_T("구입차수KEY"), m_pTargetGruop->GetRecKey(), nIdx);
				m_pDM_WORKNO->SetCellData(_T("마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), nIdx);
			}
		}
		else		strTmpData.Format(_T("%d"), nLastWorkNo);

		m_pDM_ACCU->SetCellData(_T("BP_작업번호"), strTmpData, i);
		strTmpData.Format(_T("%d"), ++nLastSerialNo);
		m_pDM_ACCU->SetCellData(_T("BP_일련번호"), strTmpData, i);
		strTmpData = m_pDM_ACCU->GetCellData(_T("BP_수서정보KEY"), i);
		m_pDM_ACCU->MakeQueryByDM(RemoveAliasList, m_pDM_ACCU, m_pDM_ACCU->TBL_NAME, i, m_pDM_BOOK, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
	}
	
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("작업번호KEY"));
	nRowCnt = m_pDM_WORKNO->GetRowCount();
	for( i=0 ; i<nRowCnt ; i++ )
	{
		strTmpData = m_pDM_WORKNO->GetCellData(_T("작업번호KEY"), i);
		m_pDM_WORKNO->MakeQueryByDM(RemoveAliasList, m_pDM_WORKNO, m_pDM_WORKNO->TBL_NAME, i, m_pDM_BOOK, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
	}
	strQuery.Format(_T("DELETE FROM BO_PURCHASE_SEQ_NO_TBL WHERE REC_KEY = %s;"), pGroupInfo->GetRecKey());
	m_pDM_BOOK->AddFrame(strQuery);
	strQuery.Format(_T("DELETE FROM BO_PURCHASE_WORK_NO_TBL WHERE PURCHASE_SEQ_NO_KEY = %s;"), pGroupInfo->GetRecKey());
	m_pDM_BOOK->AddFrame(strQuery);

	return 0;
}

INT CGroupPair::FindRowIdx(CString strAlias, CString strData, CESL_DataMgr *pDM)
{
	INT nRowCnt = pDM->GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM->GetCellData(strAlias, i);
		if(strTmpData == strData) return i;
	}
	return -1;
}

INT CGroupPair::DeleteGroupInMainDM(CString strRecKey, CString strKeyAlias)
{
	INT nRowCnt = m_pMainDM->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(strRecKey == m_pMainDM->GetCellData(strKeyAlias, i))
			m_pMainDM->DeleteRow(i);
	}
	return 0;
}

INT CGroupPair::UpdateGroupBookCnt(CGroupInfo *pTargetGruop)
{
	INT i;
	CString strQuery, strResult, strYearAlias, strGroupNoAlias, strBookCntAlias, strGroupNo, strAcqCode, strWorkCode;
	
	switch( m_nGroupType )
	{
	case GROUP_NO:
	{
		strYearAlias = _T("그룹생성년도");
		strGroupNoAlias = _T("그룹이름");
		strBookCntAlias = _T("그룹책수");
		strGroupNo = pTargetGruop->GetGroupNo();
		strAcqCode = _T("0");
	}
		break;
	case DIVNO:
	{
		strYearAlias = _T("수입년도");
		strGroupNoAlias = _T("차수번호");
		strBookCntAlias = _T("책수");
		strGroupNo = pTargetGruop->GetGroupInfo();
		strAcqCode = _T("1");
	}
		break;
	case RECEIPT_NO:
	{
		strYearAlias = _T("수입년도");
		strGroupNoAlias = _T("접수번호");
		strBookCntAlias = _T("책수");
		strGroupNo = pTargetGruop->GetGroupInfo();
			strAcqCode		= _T("2");
	}
		break;
	case OTHER_GROUP_NO:
	{
		strYearAlias = _T("수입년도");
		strGroupNoAlias = _T("그룹번호");
		strBookCntAlias = _T("책수");
		strGroupNo = pTargetGruop->GetGroupInfo();
		strAcqCode = m_strAcqCode;		
	}
		break;
	}

	if( strAcqCode == _T("0") )
	{
		if( m_bDeleteDataYN == TRUE )
		{
			strQuery.Format(_T("SELECT COUNT(*) ")
							_T("FROM BO_DELETE_BOOK_TBL ")
							_T("WHERE ACQ_YEAR='%s' ")
							_T("AND SEQ_NO='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), pTargetGruop->GetAcqYear(), strGroupNo);
		}
		else
		{
			strQuery.Format(_T("SELECT COUNT(*) ")
							_T("FROM BO_BOOK_TBL ")
							_T("WHERE (ACQ_CODE IS NULL OR ACQ_CODE='%s') ")
							_T("AND ACQ_YEAR='%s' AND SEQ_NO='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strAcqCode, pTargetGruop->GetAcqYear(), strGroupNo);	
		}
	}
	else if ( strAcqCode == _T("2") )
	{
		strQuery.Format(_T("SELECT COUNT(*) ")
						_T("FROM BO_BOOK_TBL B, BO_SPECIES_TBL S ")
						_T("WHERE B.SPECIES_KEY = S.REC_KEY ")
						_T("AND B.MANAGE_CODE = UDF_MANAGE_CODE AND B.ACQ_CODE='%s' ")								
						_T("AND B.ACQ_YEAR='%s' ")
						_T("AND B.SEQ_NO='%s' "), strAcqCode, pTargetGruop->GetAcqYear(), strGroupNo);
	}
		else
	{
		strWorkCode = pTargetGruop->GetWorkCode();
		if( strWorkCode.IsEmpty() == TRUE )
		{
			strQuery.Format(_T("SELECT COUNT(*) ")
							_T("FROM BO_BOOK_TBL B, BO_SPECIES_TBL S ")
							_T("WHERE B.SPECIES_KEY = S.REC_KEY ")
							_T("AND B.MANAGE_CODE = UDF_MANAGE_CODE AND B.ACQ_CODE='%s' ")
							_T("AND B.ACQ_YEAR='%s' ")
							_T("AND B.SEQ_NO='%s' ")
							_T("AND S.WORK_CODE IS NULL "), strAcqCode, pTargetGruop->GetAcqYear(), strGroupNo);
	}	
	else
	{
			strQuery.Format(_T("SELECT COUNT(*) ")
							_T("FROM BO_BOOK_TBL B, BO_SPECIES_TBL S ")
							_T("WHERE B.SPECIES_KEY = S.REC_KEY ")
							_T("AND B.MANAGE_CODE = UDF_MANAGE_CODE AND B.ACQ_CODE='%s' ")
							_T("AND B.ACQ_YEAR='%s' ")
							_T("AND B.SEQ_NO='%s' ")
							_T("AND S.WORK_CODE='%s' "), strAcqCode, pTargetGruop->GetAcqYear(), strGroupNo, strWorkCode);
		}
	}
	m_pMainDM->GetOneValueQuery(strQuery, strResult);
	
	
	for( i=0 ; i<m_pMainDM->GetRowCount(); i++ )
	{
		if( pTargetGruop->GetAcqYear() == m_pMainDM->GetCellData(strYearAlias, i)	&& 
			pTargetGruop->GetGroupInfo() == m_pMainDM->GetCellData(strGroupNoAlias, i) )
	{
			m_pMainDM->SetCellData(strBookCntAlias, strResult, i);
		}
	}
	return 0;
}

VOID CGroupPair::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) m_pDataProcessingDlg = new CDataProcessingDlg(m_pParentMgr);

	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(m_pParentMgr))
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();
	
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, 0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CGroupPair::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

INT CGroupPair::SendFrame(CESL_DataMgr *pDM_OUT, INT nSendCount, BOOL bShowDataProcessStatus/*=TRUE*/)
{
	if (!pDM_OUT) return -1;
	if (nSendCount <= 0) return -2;
	if(m_pDataProcessingDlg && bShowDataProcessStatus)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
		m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, pDM_OUT->frame.GetCount());
	}
	INT ids;
	while(TRUE) {
		if (pDM_OUT->frame.IsEmpty() == TRUE) break;
		ids = pDM_OUT->ORACLE_SendFrame2(nSendCount);
		if(m_pDataProcessingDlg && bShowDataProcessStatus) m_pDataProcessingDlg->GetProgressCtrl()->SetPos(m_pDataProcessingDlg->GetProgressCtrl()->GetPos() + nSendCount);
		if (ids < 0) return ids;
	}
	return 0;
}

INT CGroupPair::MakeMergeDM(CESL_DataMgr *pDM, CArray<CStringArray *, CStringArray *> * pArrSelectRecKey, CString strTmpQuery, CString strField)
{
	if(pDM == NULL || pArrSelectRecKey == NULL) return -1;

	RemoveAllDMArray(&m_ArrData);

	INT ids;
	CString strRecKeyList, strQuery;
	INT nCnt = pArrSelectRecKey->GetSize();
	CESL_DataMgr *pTmpDM = NULL;
	for(INT i=0 ; i<nCnt ; i++)
	{
		pTmpDM = new CESL_DataMgr;
		pTmpDM->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		pTmpDM->InitDMPKFromFile(pDM->DB_MGR_ALIAS);
		pTmpDM->SetMarcMgr(m_pParentMgr->m_pInfo->pMarcMgr);

		strQuery = strTmpQuery;
		strRecKeyList = GetRecKeyList(pArrSelectRecKey->GetAt(i));
		strQuery.Replace(_T("%FIELD%"), strField);
		strQuery.Replace(_T("%RECKEY%"), strRecKeyList);
		ids = pTmpDM->RefreshDataManager(strQuery);
		if(ids < 0) return -1;
		m_ArrData.Add(pTmpDM);
	}

	pDM->FreeData();
	
	INT nColCnt, nRowCnt, nCurIdx;
	nCnt = m_ArrData.GetSize();
	for(i=0 ; i<nCnt ; i++)
	{
		pTmpDM = m_ArrData.GetAt(i);
		nColCnt = pTmpDM->GetRefColumnCount();
		nRowCnt = pTmpDM->GetRowCount();
		for(INT j=0 ; j<nRowCnt ; j++)
		{
			pDM->InsertRow(-1);
			nCurIdx = pDM->GetRowCount()-1;
			for(INT k=0 ; k<nColCnt ; k++)
			{
				pDM->SetCellData(nCurIdx, k, pTmpDM->GetCellData(j, k));
			}
		}
	}
	
	RemoveAllDMArray(&m_ArrData);

	return 0;
}

INT CGroupPair::RemoveAllDMArray(CArray<CESL_DataMgr*, CESL_DataMgr*> * pArrDM)
{
	if(pArrDM == NULL) return 0;

	INT nCnt = pArrDM->GetSize();
	if(nCnt == 0) return 0;
	for(INT i=0 ; i<nCnt ; i++)
	{
		delete pArrDM->GetAt(i);
	}

	pArrDM->RemoveAll();
	return 0;
}

INT CGroupPair::DivideOtherGroup()
{
	if(m_strAcqCode.IsEmpty()) return -1;

	INT ids = SetDataMgr(m_pDM_BOOK, _T("DMUP_단행_종_책정보"));
	if(ids < 0) return -1;
	
	CString strQuery;
	strQuery = _T("%FIELD% IN (%RECKEY%) ORDER BY SERIAL_NO");
	ids = MakeMergeDM(m_pDM_BOOK, m_pArrRecKey, strQuery, _T("SPECIES_KEY"));
	if(ids < 0) return -1;	
	
	INT nLastSerialNo = GetBookLastSerialNo(m_pDM_BOOK, m_strAcqCode, m_pTargetGruop);
	
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("BB_책KEY"));
	RemoveAliasList.Add(_T("BB_원본등록번호"));
	
	INT nRowCnt = m_pDM_BOOK->GetRowCount();
	CString strTmpData;
	m_pDM_BOOK->StartFrame();
	ShowDataProcessingDlg();
	strTmpData.Format(_T("선정된 자료의 그룹정보를 %s-%s그룹정보로 수정중입니다."), m_pTargetGruop->GetAcqYear(), m_pTargetGruop->GetGroupInfo());
	m_pDataProcessingDlg->SetMessage(strTmpData);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);
		
		m_pDM_BOOK->SetCellData(_T("BB_수입년도"), m_pTargetGruop->GetAcqYear(), i);
		m_pDM_BOOK->SetCellData(_T("BB_그룹번호"), m_pTargetGruop->GetGroupInfo(), i);
		m_pDM_BOOK->SetCellData(_T("BB_마지막작업"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		strTmpData.Format(_T("%d"), ++nLastSerialNo);
		m_pDM_BOOK->SetCellData(_T("BB_일련번호"), strTmpData, i);
		strTmpData = m_pDM_BOOK->GetCellData(_T("BB_책KEY"), i);
		m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_BOOK, m_pDM_BOOK->TBL_NAME, i, m_pDM_BOOK, _T("REC_KEY"), _T("NUMERIC"), strTmpData);
	}
	
	m_pTargetGruop->SetLastSerialNo(nLastSerialNo);
	m_pTargetGruop->UpdateLastSerialNo(m_strAcqCode, m_pParentMgr, m_pDM_BOOK);
	strTmpData.Format(_T("자료를 %s-%s 그룹으로 분리중입니다."), m_pTargetGruop->GetAcqYear(), m_pTargetGruop->GetGroupInfo());
	m_pDataProcessingDlg->SetMessage(strTmpData);
	ids = m_pDM_BOOK->SendFrame();
	DeleteDataProcessingDlg();
	if(ids < 0) return -1;
	ids = m_pDM_BOOK->EndFrame();
	if(ids < 0) return -1;
	
	UpdateGroupBookCnt(m_pTargetGruop);
	
	return 0;
}

INT CGroupPair::CopyOtherGroup()
{
	INT ids = SetDataMgr(m_pDM_IDX, _T("DMUP_단행_종_색인정보"));
	if(ids < 0) return -1;
	ids = SetDataMgr(m_pDM_SPECIES, _T("DMUP_단행_종_종정보"));
	if(ids < 0) return -1;
	ids = SetDataMgr(m_pDM_BOOK, _T("DMUP_단행_종_책정보"));
	if(ids < 0) return -1;
	
	CString strQuery, strTmpQuery;
	strQuery.Format(
		_T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_CODE='%s' AND ACQ_YEAR='%s' AND SEQ_NO=%s AND (WORKING_STATUS < 'BOC' OR WORKING_STATUS = 'BOR111N'))"),
		m_pSourceGroup->GetGroupNo(), m_pSourceGroup->GetAcqYear(), m_pSourceGroup->GetGroupInfo());
	m_pDM_IDX->RefreshDataManager(strQuery);
	m_pDM_SPECIES->RefreshDataManager(strQuery);
	
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	
	CString strNewKey, strOldKey, strMsg;
	
	INT nRowCnt = m_pDM_IDX->GetRowCount();
	ShowDataProcessingDlg();
	strMsg.Format(_T("%s 그룹 자료를 복사중입니다."), m_pSourceGroup->GetGroupInfo());
	m_pDataProcessingDlg->SetMessage(strMsg);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, nRowCnt);
	
	INT nQueryUnitCnt = 50;
	
	m_pDM_BOOK->StartFrame();
	
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		m_pDataProcessingDlg->GetProgressCtrl()->SetPos(i+1);
		
		strOldKey = m_pDM_IDX->GetCellData(_T("IBS_REC_KEY"), i);
		m_pDM_BOOK->MakeRecKey(strNewKey);
		m_pDM_IDX->SetCellData(_T("IBS_REC_KEY"), strNewKey, i);
		m_pDM_SPECIES->SetCellData(_T("BS_종KEY"), strNewKey, i);
		m_pDM_SPECIES->SetCellData(_T("BS_최초작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_SPECIES->SetCellData(_T("BS_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_IDX->SetCellData(_T("IBS_최초작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);
		m_pDM_IDX->SetCellData(_T("IBS_마지막작업정보"), m_pParentMgr->GetWorkLogMsg(WORK_LOG), i);

		ids = DeleteAcqUselessMarcTag(m_pDM_SPECIES, i);
		if(ids < 0) continue;
		ids = m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_IDX, m_pDM_IDX->TBL_NAME, i, m_pDM_BOOK);
		if(ids < 0) return ids;
		ids = m_pDM_BOOK->MakeQueryByDM(RemoveAliasList, m_pDM_SPECIES, m_pDM_SPECIES->TBL_NAME, i, m_pDM_BOOK);	
		if(ids < 0) return ids;
		if(m_pParentMgr->m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{
			strTmpQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), strNewKey);
			m_pDM_BOOK->AddFrame(strTmpQuery);
		}
		CopyBookData(m_pSourceGroup->GetGroupNo(), strNewKey, strOldKey);
		if(i%nQueryUnitCnt == 0)
		{
			ids = m_pDM_BOOK->SendFrame();	
			if(ids < 0) return -1;
			ids = m_pDM_BOOK->EndFrame();
			if(ids < 0) return -1;
			
			if(ids < 0)
			{
				DeleteDataProcessingDlg();
				return -1;
			}
			m_pDM_BOOK->StartFrame();
		}		
	}
	
	ids = m_pDM_BOOK->SendFrame();
	DeleteDataProcessingDlg();
	if(ids < 0) return -1;
	ids = m_pDM_BOOK->EndFrame();
	if(ids < 0) return -1;
	
	return 0;
}

INT CGroupPair::MergeOtherGroup()
{
	if(m_strAcqCode.IsEmpty()) return -1;

	INT ids = SetDataMgr(m_pDM_BOOK, _T("DMUP_단행_종_책정보"));
	if(ids < 0) return -1;
	
	CGroupInfo * pGroupInfo = NULL;
	CString strQuery, strMsg;
	ids = GetBookLastSerialNo(m_pDM_BOOK, m_strAcqCode, m_pTargetGruop);
	m_pTargetGruop->SetLastSerialNo(ids);
	
	INT nGroupCnt = m_pSourceGroupCollection->GetGroupCount();
	
	ShowDataProcessingDlg();
	
	for(INT i=0 ; i<nGroupCnt ; i++)
	{
		m_pDM_BOOK->StartFrame();
		pGroupInfo = m_pSourceGroupCollection->GetGroupInfo(i);
		if(m_pTargetGruop->IsEqual(pGroupInfo)) continue;
		strMsg.Format(_T("%s-%s 그룹을 통합하고 있습니다."), pGroupInfo->GetAcqYear(), pGroupInfo->GetGroupInfo());
		m_pDataProcessingDlg->SetMessage(strMsg);
		ids = UpdateBookData(m_strAcqCode, pGroupInfo);
		if(ids < 0) return -1;
		pGroupInfo->SetRecKey(m_strAcqCode, m_pDM_BOOK);
		strQuery.Format(_T("DELETE FROM BO_ACQ_SEQ_NO_TBL WHERE REC_KEY=%s;"), pGroupInfo->GetRecKey());
		m_pDM_BOOK->AddFrame(strQuery);
		
		strMsg = _T("그룹통합 작업을 수행중입니다.");
		m_pDataProcessingDlg->SetMessage(strMsg);
		ids = m_pDM_BOOK->SendFrame();
		if(ids < 0) return -1;
		ids = m_pDM_BOOK->EndFrame();
		if(ids < 0) return ids;
		
		DeleteGroupInMainDM(pGroupInfo->GetRecKey(), _T("그룹번호KEY"));
	}
	DeleteDataProcessingDlg();
	
	m_pDM_BOOK->StartFrame();
	m_pTargetGruop->UpdateLastSerialNo(m_strAcqCode, m_pParentMgr, m_pDM_BOOK);
	ids = m_pDM_BOOK->SendFrame();
	if(ids < 0) return ids;
	ids = m_pDM_BOOK->EndFrame();
	if(ids < 0) return ids;
	
	UpdateGroupBookCnt(m_pTargetGruop);
	return ids;
}

INT CGroupPair::DeleteAcqUselessMarcTag(CESL_DataMgr *pDM_SPECIES, INT nIdx)
{
	if(pDM_SPECIES == NULL || nIdx < 0) return -1;
	if(pDM_SPECIES->GetRowCount() <= nIdx) return -1;

	CMarc marc;
	CString strMarc = pDM_SPECIES->GetCellData(_T("BS_MARC"), nIdx);
	if(strMarc.IsEmpty()) return -1;
	INT ids = m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	if(ids < 0) return -1;

	m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("001"));
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("005"));
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));
	m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("090"));
	ids = m_pParentMgr->m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	if(ids < 0) return -1;
	pDM_SPECIES->SetCellData(_T("BS_MARC"), strMarc, nIdx);
	return 0;
}
