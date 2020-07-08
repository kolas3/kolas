// TransferMgr.cpp: implementation of the CTransferMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TransferMgr.h"
#include "..\SE_API\SeApi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTransferMgr::CTransferMgr(CESL_Mgr * pParentMgr):CSPF_Work(pParentMgr)
{
	m_pDM = m_pParentMgr->FindDM(_T("DM_연속_수서_자료인계"));
	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(_T("CM_연속_수서_자료인계"));
	if(pCM) m_pGrid = (CESL_Grid*)pCM->FindControl(_T("그리드"));
}

CTransferMgr::~CTransferMgr()
{

}

INT CTransferMgr::SPFExecutePath(CString strPath)
{
	if(strPath.GetLength() == 0)
		return -1;
	
	if(strPath == _T("실시간인계"))
		return TransferToRealTime();
	if(strPath == _T("실시간배가"))
		return TransferToShelf();
	if(strPath == _T("등록대상처리"))
		return TransferToReg();
	if(strPath == _T("제본대상처리"))
		return TransferToBinding();

	return 0;
}


INT CTransferMgr::TransferToRealTime()
{
	//실시간 인계(SEL111O)
	m_pGrid->SelectMakeList();
	INT nCount = m_pGrid->SelectGetCount();
	if(0 == nCount) {
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return 0;
	}

	INT nIdx = m_pGrid->SelectGetHeadPosition();
	
	CString strWorkingStatus;

	while(nIdx>=0)
	{
		strWorkingStatus = _T("");
		strWorkingStatus = m_pDM->GetCellData(_T("SB_자료상태"),nIdx);
		
		if(_T("SEL112N") == strWorkingStatus) {
			AfxMessageBox(_T("선택된 항목중 이미 실시간 배가처리된 자료가 있습니다! \n 확인해 주십시오!"));
			return 0;
		}
	
		nIdx = m_pGrid->SelectGetNext();
	}


	CString msg;

	msg.Format(_T("%d 건의 자료를 실시간 인계처리 하시겠습니까?"),nCount);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;

	INT ids = -1;
	CSeApi api(m_pParentMgr);
	CString strBookKey;
	
	m_pParentMgr->BeginWaitCursor();
	nIdx = m_pGrid->SelectGetHeadPosition();

	while(nIdx>=0)
	{
		strBookKey = _T("");
		strBookKey = m_pDM->GetCellData(_T("SB_책KEY"),nIdx);
		
		ids = 0;
		if(!strBookKey.IsEmpty()) {
			ids = api.BOOK_TRANSFER_TO_REAL_TIME(m_pDM,strBookKey);
		}
		
		if(ids<0)
		{
			msg.Format(_T("%d 번째 항목의 작업을 수행하던 중 문제가 생겨 \n 작업을 중단합니다."),nIdx);
			AfxMessageBox(msg);
			m_pParentMgr->EndWaitCursor();
			return -1;
		}
		else
		{
			m_pDM->SetCellData(_T("UDF_자료상태"),_T("실시간인계자료"),nIdx);
			m_pDM->SetCellData(_T("SB_자료상태"),_T("SEL111O"),nIdx);
			m_pDM->SetCellData(_T("SB_인계여부"),_T("Y"),nIdx);
			m_pGrid->DisplayLine(nIdx);

		}


		nIdx = m_pGrid->SelectGetNext();
	}
	
	m_pParentMgr->EndWaitCursor();
	

	//인계 처리한 자료는 삭제
	nIdx = m_pGrid->SelectGetTailPosition();
	
	while(nIdx>=0)
	{
		m_pDM->DeleteRow(nIdx);
		nIdx = m_pGrid->SelectGetPrev();
	}
	
	m_pGrid->Display();

	msg.Format(_T("%d 건의 자료를 실시간 인계처리 하였습니다!"),nCount);
	AfxMessageBox(msg);

	


	return 0;
}

INT CTransferMgr::TransferToReg()
{
	//등록대상(SER111N) 처리
	m_pGrid->SelectMakeList();
	INT nCount = m_pGrid->SelectGetCount();
	if(0 == nCount) {
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return 0;
	}

	INT nIdx = m_pGrid->SelectGetHeadPosition();
	
	CString strWorkingStatus;

	while(nIdx>=0)
	{
		strWorkingStatus = _T("");
		strWorkingStatus = m_pDM->GetCellData(_T("SB_자료상태"),nIdx);
		
		if(_T("SEL112N") == strWorkingStatus) {
			AfxMessageBox(_T("선택된 항목중 이미 실시간 배가처리된 자료가 있습니다! \n 확인해 주십시오!"));
			return 0;
		}
	
		nIdx = m_pGrid->SelectGetNext();
	}

	
	CString msg;

	msg.Format(_T("%d 건의 자료를 등록대상 처리 하시겠습니까?"),nCount);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;

	INT ids = -1;
	CSeApi api(m_pParentMgr);
	CString strBookKey;
	
	m_pParentMgr->BeginWaitCursor();
	nIdx = m_pGrid->SelectGetHeadPosition();

	while(nIdx>=0)
	{
		strBookKey = _T("");
		strBookKey = m_pDM->GetCellData(_T("SB_책KEY"),nIdx);
		
		if(!strBookKey.IsEmpty()) {
			ids = api.BOOK_TRANSFER_TO_REG(m_pDM,strBookKey);
		}
		
		if(ids<0)
		{
			msg.Format(_T("%d 번째 항목의 작업을 수행하던 중 문제가 생겨 \n 작업을 중단합니다."),nIdx);
			AfxMessageBox(msg);
			m_pParentMgr->EndWaitCursor();
			return -1;
		}
		else
		{
			m_pDM->SetCellData(_T("UDF_자료상태"),_T("등록인계자료"),nIdx);
			m_pDM->SetCellData(_T("SB_자료상태"),_T("SER111N"),nIdx);
			m_pDM->SetCellData(_T("SB_인계여부"),_T("Y"),nIdx);
			m_pGrid->DisplayLine(nIdx);

		}

		nIdx = m_pGrid->SelectGetNext();
	}
	
	m_pParentMgr->EndWaitCursor();
	
	//인계 처리한 자료는 삭제
	nIdx = m_pGrid->SelectGetTailPosition();
	
	while(nIdx>=0)
	{
		m_pDM->DeleteRow(nIdx);
		nIdx = m_pGrid->SelectGetPrev();
	}
	
	m_pGrid->Display();

	msg.Format(_T("%d 건의 자료를 등록대상 처리 하였습니다!"),nCount);
	AfxMessageBox(msg); 

	return 0;
}

INT CTransferMgr::TransferToBinding()
{
	//제본 대상(SEB111N) 처리
	m_pGrid->SelectMakeList();
	INT nCount = m_pGrid->SelectGetCount();
	if(0 == nCount) {
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return 0;
	}


	INT nIdx = m_pGrid->SelectGetHeadPosition();
	
	CString strWorkingStatus;

	while(nIdx>=0)
	{
		strWorkingStatus = _T("");
		strWorkingStatus = m_pDM->GetCellData(_T("SB_자료상태"),nIdx);
		
		if(_T("SEL112N") == strWorkingStatus) {
			AfxMessageBox(_T("선택된 항목중 이미 실시간 배가처리된 자료가 있습니다! \n 확인해 주십시오!"));
			return 0;
		}
	
		nIdx = m_pGrid->SelectGetNext();
	}

	
	CString msg;

	msg.Format(_T("%d 건의 자료를 제본대상 처리 하시겠습니까?"),nCount);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;

	INT ids = -1;
	CSeApi api(m_pParentMgr);
	CString strBookKey;
	
	m_pParentMgr->BeginWaitCursor();
	nIdx = m_pGrid->SelectGetHeadPosition();


	while(nIdx>=0)
	{
		strBookKey = _T("");
		strBookKey = m_pDM->GetCellData(_T("SB_책KEY"),nIdx);
		
		if(!strBookKey.IsEmpty()) {
			ids = api.BOOK_TRANSFER_TO_BINDING(m_pDM,strBookKey);
		}
		
		if(ids<0)
		{
			msg.Format(_T("%d 번째 항목의 작업을 수행하던 중 문제가 생겨 \n 작업을 중단합니다."),nIdx);
			AfxMessageBox(msg);
			m_pParentMgr->EndWaitCursor();
			return -1;
		}
		else
		{
			m_pDM->SetCellData(_T("UDF_자료상태"),_T("제본인계자료"),nIdx);
			m_pDM->SetCellData(_T("SB_자료상태"),_T("SEB111N"),nIdx);
			m_pDM->SetCellData(_T("SB_인계여부"),_T("Y"),nIdx);
			m_pGrid->DisplayLine(nIdx);

		}

		nIdx = m_pGrid->SelectGetNext();
	}
	
	m_pParentMgr->EndWaitCursor();
	
	msg.Format(_T("%d 건의 자료를 제본대상 처리 하였습니다!"),nCount);


	//인계 처리한 자료는 삭제
	nIdx = m_pGrid->SelectGetTailPosition();
	
	while(nIdx>=0)
	{
		m_pDM->DeleteRow(nIdx);
		nIdx = m_pGrid->SelectGetPrev();
	}
	
	m_pGrid->Display();

	AfxMessageBox(msg);

	return 0;
}

INT CTransferMgr::TransferToShelf()
{
	//실시간 배가(SEL112N);
	m_pGrid->SelectMakeList();
	INT nCount = m_pGrid->SelectGetCount();
	if(0 == nCount) {
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return 0;
	}


	INT nIdx = m_pGrid->SelectGetHeadPosition();
	
	CString strWorkingStatus;

	while(nIdx>=0)
	{
		strWorkingStatus = _T("");
		strWorkingStatus = m_pDM->GetCellData(_T("SB_자료상태"),nIdx);
		
		if(_T("SEL112N") == strWorkingStatus) {
			AfxMessageBox(_T("선택된 항목중 이미 실시간 배가처리된 자료가 있습니다! \n 확인해 주십시오!"));
			return 0;
		}
	
		nIdx = m_pGrid->SelectGetNext();
	}

	
	CString msg;

	msg.Format(_T("%d 건의 자료를 실시간 배가 처리 하시겠습니까?"),nCount);
	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return 0;

	INT ids = -1;
	CSeApi api(m_pParentMgr);
	CString strBookKey;
	CString strVolKey;
	CString strSpeciesKey;
	CString strPrevWorkingStatus;

	m_pParentMgr->BeginWaitCursor();
	nIdx = m_pGrid->SelectGetHeadPosition();


	while(nIdx>=0)
	{
		strBookKey = _T("");
		strVolKey = _T("");
		strSpeciesKey = _T("");

		strBookKey = m_pDM->GetCellData(_T("SB_책KEY"),nIdx);
		strVolKey = m_pDM->GetCellData(_T("SB_권KEY"),nIdx);
		strSpeciesKey = m_pDM->GetCellData(_T("SB_종KEY"),nIdx);
		strPrevWorkingStatus = _T("SEL111O");


		ids = api.BOOK_TRANSFER_TO_SHELF(m_pDM,
										strPrevWorkingStatus,
										strSpeciesKey,
										strVolKey,
										strBookKey);

		if(ids<0)
		{
			msg.Format(_T("%d 번째 항목의 작업을 수행하던 중 문제가 생겨 \n 작업을 중단합니다."),nIdx);
			AfxMessageBox(msg);
			m_pParentMgr->EndWaitCursor();
			return -1;
		}
		else
		{
			m_pDM->SetCellData(_T("UDF_자료상태"),_T("실시간배가자료"),nIdx);
			m_pDM->SetCellData(_T("SB_자료상태"),_T("SEL112N"),nIdx);
			m_pDM->SetCellData(_T("SB_인계여부"),_T("Y"),nIdx);
			m_pGrid->DisplayLine(nIdx);

		}

		nIdx = m_pGrid->SelectGetNext();
	}
	
	m_pParentMgr->EndWaitCursor();
	
	msg.Format(_T("%d 건의 자료를 실시간 배가 처리 하였습니다!"),nCount);

	//인계 처리한 자료는 삭제
	nIdx = m_pGrid->SelectGetTailPosition();
	
	while(nIdx>=0)
	{
		m_pDM->DeleteRow(nIdx);
		nIdx = m_pGrid->SelectGetPrev();
	}
	
	m_pGrid->Display();

	AfxMessageBox(msg);
	return 0;
}
