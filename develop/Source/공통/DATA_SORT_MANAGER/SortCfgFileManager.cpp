// SortCfgFileManager.cpp: implementation of the CSortCfgFileManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SortCfgFileManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSortCfgFileManager::CSortCfgFileManager(CESL_Mgr * pParentMgr)
{
	m_pParentMgr = pParentMgr;
	m_pGrid = NULL;
	m_pArrSortElements = NULL;
}

CSortCfgFileManager::~CSortCfgFileManager()
{
	
}

INT CSortCfgFileManager::ReadSortElements()
{
	if(m_pParentMgr == NULL || m_pGrid == NULL || m_strGridAlias.IsEmpty() || m_pArrSortElements == NULL)
		return -1;
	CString strPath = _T("..\\cfg\\SortConfig\\");
	CString strFileName = _T("SortElement_") + m_pParentMgr->SM_ALIAS + _T("_") + m_strGridAlias;

	INT ids, nCnt;
	CString strTitle, strDMAlias, strPriority, strTmpData;
	
	CStdioFile File;
	CSortElement * pElement = NULL;
	CStringArray tmpArr;	tmpArr.RemoveAll();
	if(m_pArrSortElements->GetSize() > 0)
	{
		ids = RemoveSortElements();
		if(ids < 0) return ids;
	}
	
	if ( File.Open(strPath+strFileName, CFile::modeRead | CFile::typeBinary) != NULL) 
	{
		TCHAR cUni;
		File.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			File.SeekToBegin();
		}
		while(File.ReadString(strTmpData))
		{
			strTmpData.TrimLeft();	strTmpData.TrimRight();
			ids = CheckString(strTmpData);
			if(ids == -1) continue;
			else 
			{
				pElement = new CSortElement;
				if(pElement == NULL) return -1;
				tmpArr.RemoveAll();
				ids = ParsingString(_T("||"), strTmpData, &tmpArr);
				if(ids < 0) return ids;
				nCnt = tmpArr.GetSize();
				if(nCnt < 4) return -1;
				pElement->SetTitle(tmpArr.GetAt(0));
				pElement->SetDMAlias(tmpArr.GetAt(1));
				pElement->SetPriority(_ttoi(tmpArr.GetAt(2).GetBuffer(0)));
				pElement->SetSortDirection(_ttoi(tmpArr.GetAt(3).GetBuffer(0)));
				m_pArrSortElements->Add(pElement);
			}
		}
		File.Close();
	}
	
	return 0;
}

INT CSortCfgFileManager::RemoveSortElements()
{
	INT nCnt = m_pArrSortElements->GetSize();
	if(nCnt == 0) return 0;

	CSortElement * pElement = NULL;
	for(INT i=0 ; i<nCnt ; i++)
	{
		pElement = m_pArrSortElements->GetAt(i);
		if(pElement)
		{
			delete pElement;
		}
	}
	m_pArrSortElements->RemoveAll();
	return 0;
}

INT CSortCfgFileManager::CheckString(CString str)
{
	//공백문자면 return
	if(str.IsEmpty()) return -1;
	TCHAR firstchar = str.GetAt(0);
	
	if(firstchar == '#' || firstchar == '.')
		return -1;
	return 1; 
}

INT CSortCfgFileManager::ParsingString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray)
{
	if(pTargetArray == NULL) return -1;
	if(strSource.IsEmpty()) return 0;
	INT pos = strSource.Find(DelimeterStr, 0);
	CString strTmpData;
	while(pos >= 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+DelimeterStr.GetLength());
		pTargetArray->Add(strTmpData);
		pos = strSource.Find(DelimeterStr, 0);
	}
	pTargetArray->Add(strSource);
	return 0;
}

INT CSortCfgFileManager::WriteSortElements()
{
	if(m_pParentMgr == NULL || m_pGrid == NULL || m_strGridAlias.IsEmpty() || m_pArrSortElements == NULL)
		return -1;
	CString strPath = _T("..\\cfg\\SortConfig\\");
	CString strFileName = _T("SortElement_") + m_pParentMgr->SM_ALIAS + _T("_") + m_strGridAlias;
	
	if(m_pArrSortElements->GetSize() == 0) return 0;

	INT nCnt;
	CString strTmpData, strPriority, strSortDirection;
	
	CStdioFile File;
	CSortElement * pElement = NULL;
	if ( File.Open(strPath+strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) == NULL) 
	{
		strTmpData.Format(_T("[%s] 파일을 생성할수 없습니다."), strPath+strFileName);
		m_pParentMgr->ESLAfxMessageBox(strTmpData, MB_ICONSTOP);
		return -1;
	}
	else
	{
		if( 0 == File.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			File.Write( &cUni, sizeof(TCHAR));
		}	
		
		nCnt = m_pArrSortElements->GetSize();
		for(INT i=0 ; i<nCnt ; i++)
		{
			pElement = m_pArrSortElements->GetAt(i);
			strTmpData = pElement->GetTitle();
			strTmpData += _T("||") + pElement->GetDMAlias();
			strPriority.Format(_T("%d"), pElement->GetPriority());
			strTmpData += _T("||") + strPriority;
			strSortDirection.Format(_T("%d"), pElement->GetSortDirection());
			strTmpData += _T("||") + strSortDirection;
			File.WriteString(strTmpData);
		}
		File.Close();
	}
	
	return 0;
}

INT CSortCfgFileManager::MakeSortElementDM(CESL_DataMgr * pDM)
{
	if(m_pGrid == NULL || pDM == NULL) return -1;
	pDM->FreeData();
	
	INT nColCnt = m_pGrid->CountLinkColumns();
	INT nFirst, nCurIdx;

	CString strInitValue = m_pGrid->ELEMENT->INIT_VALUE;
	CString strLinkValue = m_pGrid->ELEMENT->LINK_DATA;
	strInitValue.TrimLeft();	strInitValue.TrimRight();
	strLinkValue.TrimLeft();	strLinkValue.TrimRight();
	if (strInitValue.IsEmpty() || strLinkValue.IsEmpty()) return -1;
	
	CString strLinkElement, strInitElement, strDMAlias, strColumn, strTmpData;
	for(INT i=0 ; i<nColCnt ; i++) 
	{
		strLinkElement.Empty();

		strLinkElement = strLinkValue.SpanExcluding(_T(","));
		strLinkElement.TrimLeft();		strLinkElement.TrimRight();
		if (!strLinkElement.IsEmpty()) 
		{
			if(m_pGrid->ParseMgrAndColumn(strLinkElement, strDMAlias, strTmpData) < 0) 
				return -1;
		}
		else strDMAlias.Empty();

		strTmpData = strLinkValue.Mid(strLinkElement.GetLength()+1);
		strLinkValue = strTmpData;

		strInitElement.Empty();
		strInitElement = strInitValue.SpanExcluding(_T(","));
		if(!strInitElement.IsEmpty())
		{
			strColumn.Empty();
			nFirst = 0;
			nFirst = strInitElement.Find(':', 0);
			strColumn = strInitElement.Mid(1, nFirst-1);
		}
		else strColumn.Empty();

		strTmpData = strInitValue.Mid(strInitElement.GetLength()+1);
		strInitValue = strTmpData;
		
		pDM->InsertRow(-1);
		nCurIdx = pDM->GetRowCount()-1;
		
		pDM->SetCellData(_T("이름"), strColumn, nCurIdx);
		pDM->SetCellData(_T("알리아스"), strColumn, nCurIdx);
	}
	
	return SetSortPriorityToDM(pDM);
}

INT CSortCfgFileManager::SetSortPriorityToDM(CESL_DataMgr *pDM)
{
	if(pDM == NULL || m_pArrSortElements == NULL) return -1;
	INT nElementCnt = m_pArrSortElements->GetSize();
	if(nElementCnt == 0 || pDM->GetRowCount() == 0) return 0;

	INT ids;
	for(INT i=0 ; i<nElementCnt ; i++)
	{
		ids = SetSortPriority(m_pArrSortElements->GetAt(i), pDM);
		if(ids < 0) return ids;
	}
	return 0;
}

INT CSortCfgFileManager::SetSortPriority(CSortElement * pElement, CESL_DataMgr *pDM)
{
	if(pElement == NULL || pDM == NULL) return -1;

	INT nRowCnt = pDM->GetRowCount();
	if(nRowCnt == 0) return 0;

	CString strPriority;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(pElement->GetTitle() == pDM->GetCellData(_T("이름"), i) &&
			pElement->GetDMAlias() == pDM->GetCellData(_T("알리아스"), i))
		{
			strPriority.Format(_T("%d"), pElement->GetPriority());
			pDM->SetCellData(_T("우선순위"), strPriority, i);
			return 0;
		}
	}
	return 0;
}
