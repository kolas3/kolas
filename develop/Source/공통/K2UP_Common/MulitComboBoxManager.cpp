// MulitComboBoxManager.cpp: implementation of the CMulitComboBoxManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MulitComboBoxManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMulitComboBoxManager::CMulitComboBoxManager(CESL_Mgr * pWin):CSPF_Work(pWin)
{

}

CMulitComboBoxManager::~CMulitComboBoxManager()
{

}

INT CMulitComboBoxManager::FreeMultiComboData(CESL_ControlMultiComboBox *pCMB)
{
	INT nCount = pCMB->m_jcbList.GetCount();
	if (nCount <= 0) return 0;	
	CJStruct * pObject;
	for (INT i = 0; i < nCount; i++) {
		pObject = (CJStruct*)pCMB->m_jcbList.RemoveHead();
		if (pObject) delete pObject;
	}
	pCMB->m_jcbList.RemoveAll();

	pCMB->ResetContent();
	pCMB->InitMultiComboBox(2);
	return 0;
}

INT CMulitComboBoxManager::SetMulTiComboBoxStrData(CString CMAlias, CString initType, CString strData[][2], CString alias, INT nCount)
{
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(CMAlias);
	CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)pCM->FindControl(alias);
	FreeMultiComboData(pCMB);

	SetMulTiComboBoxData(pCMB, strData, nCount, initType);
	
	return 0;
}

INT CMulitComboBoxManager::SetMulTiComboBoxData(CESL_ControlMultiComboBox *pCMB, CString strData[][2], INT nCount, CString initType)
{
	INT nSelect;
	pCMB->InitMultiComboBox(2);
	for (INT i = 0; i < nCount; i++) {
		if(strData[i][1] == initType)
			nSelect = i;
		pCMB->SetData(strData[i], 2);
	}
	if(nSelect < 0) nSelect = 0;
	pCMB->SetCurSel(nSelect);
	return 0;
}

VOID CMulitComboBoxManager::SetComboBoxSelect(CString CMAlias, CString CtrlAlias, CString initVal)
{
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(CMAlias);
	CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)pCM->FindControl(CtrlAlias);
	INT nCount = pCMB->GetRowCount();
	if(nCount == 0) return;
	CString data;
	for(INT i=0 ; i<nCount ; i++)
	{
		pCMB->GetData(data, i, 1);
		if(data == initVal)
		{
			pCMB->SetCurSel(i);
			break;
		}
	}
}

CString CMulitComboBoxManager::GetComboBoxSelect(CString CMAlias, CString CtrlAlias, CString Code)
{
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(CMAlias);
	CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)pCM->FindControl(CtrlAlias);
	INT nCount = pCMB->GetRowCount();
	if(nCount == 0) return _T("");
	CString data;
	for(INT i=0 ; i<nCount ; i++)
	{
		pCMB->GetData(data, i, 0);
		if(data == Code)
		{
			pCMB->GetData(data, i, 1);
			return data;
		}
	}
	return _T("");
}

INT CMulitComboBoxManager::SetSubjectCodeData(CString CMAlias, CString ClassNoTypeAlias, CString SubjectCodeAlias, CString initSubjectCode)
{
    CESL_ControlMgr* pCM = m_pParentMgr->FindCM(CMAlias);
	if (!pCM) return -1;
	
	CESL_ControlMultiComboBox *pCM_WhichCode = (CESL_ControlMultiComboBox *)pCM->FindControl(ClassNoTypeAlias);
	if (!pCM_WhichCode) return -1;
    CESL_ControlMultiComboBox *pCM_Subject = (CESL_ControlMultiComboBox *)pCM->FindControl(SubjectCodeAlias);
	if (!pCM_Subject) return -1;

	CString class_code;
	pCM_WhichCode->GetData(class_code, pCM_WhichCode->GetCurSel(), 0);

	class_code.TrimLeft();		class_code.TrimRight();

	CString class_name;
	class_code.MakeUpper();
	class_name = _T("");

	pCM_Subject->FreeComboData();
	pCM_Subject->ResetContent();
	pCM_Subject->InitMultiComboBox(2);
	
	if		( class_code == _T("1") )	class_name = _T("KDC대주제부호관리");
	else if ( class_code == _T("2") )	class_name = _T("DDC대주제부호관리");
	else if ( class_code == _T("3") || 
			  class_code == _T("7")	)	class_name = _T("LC주제부호");
	else return 0;
	
	INT nCode = 256;
	INT nDefaultValue = 0;
	CString strCode[256], strDesc[256];
	
	m_pParentMgr->m_pInfo->pCodeMgr->GetCode(class_name, strCode, strDesc, nCode, nDefaultValue);	
	if (nCode == 0) return -1;
	nDefaultValue = 0;
	for (INT i = 0; i < nCode; i++) {
		//============================================================
		//2009.03.10 ADD BY LKS : 공백 넓이 수정(SpaceBar->적용안함 이동X 오류)
		//if (strCode[i].GetLength() == 0) strCode[i] = _T("  ");
		if (strCode[i].GetLength() == 0) strCode[i] = _T("    ");
		//============================================================
		pCM_Subject->AddItem(strCode[i], 0, i);
		pCM_Subject->AddItem(strDesc[i], 1, i);
		if(!initSubjectCode.IsEmpty() && strCode[i] == initSubjectCode)
			nDefaultValue = i;
	}

	pCM_Subject->SetCurSel(nDefaultValue);	
	return 0;
}

INT CMulitComboBoxManager::SetSingleComboBoxStrData(CString CMAlias, CString initType, CStringArray * parrData, CString alias)
{
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(CMAlias);
	CESL_ControlComboBox *pCMB = (CESL_ControlComboBox *)pCM->FindControl(alias);
	if(pCMB == NULL) return -1;
	pCMB->ResetContent();
	
	INT cnt = parrData->GetSize();
	if(cnt == 0) return 0;

	CString strTmpData;
	INT nInitVal = 0;
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = parrData->GetAt(i);
		if(initType == strTmpData) nInitVal = i;
		pCMB->AddString(strTmpData);
	}
	pCMB->SetCurSel(nInitVal);
	
	return 0;
}

INT CMulitComboBoxManager::SetSingleComboBoxStrData(CString CMAlias, CString initType, CString strData[], CString alias, INT nCount)
{
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(CMAlias);
	CESL_ControlComboBox *pCMB = (CESL_ControlComboBox *)pCM->FindControl(alias);
	pCMB->ResetContent();
	
	SetSingleComboBoxData(pCMB, strData, nCount, initType);
	
	return 0;
}

INT CMulitComboBoxManager::SetSingleComboBoxData(CESL_ControlComboBox *pCMB, CString strData[], INT nCount, CString initType)
{
	INT nSelect;
	for (INT i = 0; i < nCount; i++) {
		if(strData[i] == initType)
			nSelect = i;
		pCMB->AddString(strData[i]);
	}
	pCMB->SetCurSel(nSelect);
	return 0;
}

VOID CMulitComboBoxManager::SetSingleComboBoxSelect(CString CMAlias, CString CtrlAlias, CString strData[], CString initValue, INT nCount)
{
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(CMAlias);
	CESL_ControlComboBox *pCMB = (CESL_ControlComboBox *)pCM->FindControl(CtrlAlias);
	for(INT i=0 ; i<nCount ; i++)
	{
		if(strData[i] == initValue)
			break;
	}
	if(i < nCount)
		pCMB->SetCurSel(i);
	else
		pCMB->SetCurSel(0);
}

INT CMulitComboBoxManager::AddMultiComboBoxStrData(CString strCMAlias, CString strData[][2], CString strCtrlAlias)
{
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(strCMAlias);
	CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)pCM->FindControl(strCtrlAlias);

	pCMB->SetData(strData[0], 2);
	
	return 0;
}

VOID CMulitComboBoxManager::SetSingleComboBoxData(CESL_ControlComboBox *pCMB, CStringArray * pArrData, CString initType)
{
	if(pCMB == NULL) return ;
	
	INT cnt = pArrData->GetSize();
	if(cnt == 0) return;

	CString strTmpData;
	INT nInitVal = 0;
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = pArrData->GetAt(i);
		if(initType == strTmpData) nInitVal = i;
		pCMB->AddString(strTmpData);
	}
	pCMB->SetCurSel(nInitVal);
	
	return ;
}
