// BO_DataSetManager.cpp: implementation of the CBO_DataSetManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_DataSetManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_DataSetManager::CBO_DataSetManager(CESL_Mgr * pParentMgr) : CBO_DataManager(pParentMgr)
{
	m_nIdx = -1;
}

CBO_DataSetManager::~CBO_DataSetManager()
{

}

INT CBO_DataSetManager::SetDMByQuery(CString strQuery)
{
	if(strQuery.IsEmpty()) return -1;
	if(m_pDM == NULL) return -1;

	INT ids = m_pDM->RefreshDataManager(strQuery);
	if(ids < 0) return -1;

	return m_pDM->GetRowCount();
}

INT CBO_DataSetManager::SetDMByGrid(CESL_Grid *pGrid, CESL_DataMgr *pDM)
{
	if(pGrid == NULL || pDM == NULL) return -1;
	
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT i = pGrid->SelectGetHeadPosition();

	m_pDM->FreeData();
	
	while(i >= 0)
	{
		ids = AddToDM(pDM, i);
		if(ids < 0) return -1;
		i = pGrid->SelectGetNext();
	}
	return 0;
}

INT CBO_DataSetManager::AddToDM(CESL_DataMgr *pDM_SOURCE, INT nSourceIdx)
{
	if(pDM_SOURCE == NULL) return -1;
	if(nSourceIdx < 0 || pDM_SOURCE->GetRowCount() <= nSourceIdx) return -1;

	m_pDM->InsertRow(-1);
	INT ids = SetDM(pDM_SOURCE, nSourceIdx, m_pDM->GetRowCount()-1);
	if(ids < 0) return -1;
	
	return 0;
}

INT CBO_DataSetManager::GetCurrentSelectData(CBO_DataManager * pCurSelData)
{
	if(0 > m_nIdx) return -1;
	if(pCurSelData == NULL) return -1;

	INT ids = GetDataByIndex(m_nIdx, pCurSelData);
	if(ids < 0) return -1;
	return 0;
}

INT CBO_DataSetManager::GetDataByIndex(INT nIdx, CBO_DataManager * pCurSelData)
{
	if(nIdx < 0 || nIdx >= m_pDM->GetRowCount()) return -1;
	if(pCurSelData == NULL) return -1;

	INT ids = pCurSelData->SetDM(m_pDM, m_nIdx);
	if(ids < 0) return -1;
	return 0;
}

INT CBO_DataSetManager::SetCurrentSelectData(INT nIdx)
{
	if(nIdx < 0 || m_pDM->GetRowCount() <= nIdx) return -1;
	m_nIdx = nIdx;
	return 0;
}

INT CBO_DataSetManager::GetDataCount()
{
	if(m_pDM == NULL) return -1;
	return m_pDM->GetRowCount();
}

INT CBO_DataSetManager::CopyDataSet(CBO_DataSetManager * pDataSet)
{
	if(pDataSet == NULL) return -1;
	
	INT nCnt = pDataSet->GetDataCount();
	if(nCnt == 0) return 0;
	INT ids;
	CBO_DataManager TmpData(m_pParentMgr);
	ids = InitDataManager(pDataSet->GetDM()->CONNECTION_INFO, pDataSet->GetDMAlias());
	if(ids < 0) return -1;
	for(INT i=0 ; i<nCnt ; i++)
	{
		ids = pDataSet->GetDataByIndex(i, &TmpData);
		if(ids < 0) return -1;
		ids = AddToDM(TmpData.GetDM(), 0);
		if(ids < 0) return -1;
	}

	return 0;
}

INT CBO_DataSetManager::SetMultiComboBoxStrData(CESL_ControlMultiComboBox * pCMB, CString strCodeAlias, CString strValueAlias, CString strInitCode)
{
	if(m_pDM == NULL || pCMB == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return 0;

	INT nCount = pCMB->m_jcbList.GetCount();
	if (nCount <= 0) return 0;	
	CJStruct * pObject;
	for (INT i = 0; i < nCount; i++) 
	{
		pObject = (CJStruct*)pCMB->m_jcbList.RemoveHead();
		if (pObject) delete pObject;
	}
	pCMB->m_jcbList.RemoveAll();
	
	pCMB->ResetContent();
	
	CString strData[2];
	INT nSelect;
	pCMB->InitMultiComboBox(2);
	for (i = 0; i < m_pDM->GetRowCount() ; i++) 
	{
		strData[0] = m_pDM->GetCellData(strCodeAlias, i);
		strData[1] = m_pDM->GetCellData(strValueAlias, i);
		if(strData[0] == strInitCode) nSelect = i;
		pCMB->SetData(strData, 2);
	}
	pCMB->SetCurSel(nSelect);
	return 0;
}

INT CBO_DataSetManager::SetSingleComboBoxStrData(CESL_ControlComboBox * pCMB, CString strValueAlias, CString strInitValue)
{
	if(pCMB == NULL || m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return 0;
	pCMB->ResetContent();
	
	CString strTmpData;
	INT nInitVal = 0;
	for(INT i=0 ; i<m_pDM->GetRowCount() ; i++)
	{
		strTmpData = m_pDM->GetCellData(strValueAlias, i);
		if(strInitValue == strTmpData) nInitVal = i;
		pCMB->AddString(strTmpData);
	}
	pCMB->SetCurSel(nInitVal);
	
	return 0;
}

INT CBO_DataSetManager::RemoveAll()
{
	if(m_pDM == NULL) return -1;
	INT ids = m_pDM->FreeData();
	if(ids < 0) return -1;
	return 0;
}

INT CBO_DataSetManager::RemoveAt(INT nIdx)
{
	if(m_pDM == NULL) return -1;
	if(nIdx < 0 || nIdx >= m_pDM->GetRowCount()) return -1;
	INT ids = m_pDM->DeleteRow(nIdx);
	if(ids < 0) return -1;

	return 0;
}