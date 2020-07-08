// SortItemSet.cpp: implementation of the CSortItemSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SortItemSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSortItemSet::CSortItemSet(CESL_Mgr * pParentMgr) : CBO_DataSetManager(pParentMgr)
{
	m_strSortCfgFile.Empty();
}

CSortItemSet::~CSortItemSet()
{

}

INT CSortItemSet::ReadSortItem(CString strFileName/*=_T("")*/)
{
	if(strFileName.IsEmpty()) strFileName = m_strSortCfgFile;
	
	INT ids, nCnt, nCurIdx;
	CString strTmpData;
	
	CStdioFile File;
	CStringArray tmpArr;	tmpArr.RemoveAll();
		
	if ( File.Open(strFileName, CFile::modeRead | CFile::typeBinary) != NULL) 
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
				tmpArr.RemoveAll();
				ids = ParsingString(_T("||"), strTmpData, &tmpArr, FALSE);
				if(ids < 0) return ids;
				nCnt = tmpArr.GetSize();
				if(nCnt < 4) return -1;
				m_pDM->InsertRow(-1);
				nCurIdx = m_pDM->GetRowCount()-1;
				m_pDM->SetCellData(_T("이름"), tmpArr.GetAt(0), nCurIdx);
				m_pDM->SetCellData(_T("알리아스"), tmpArr.GetAt(1), nCurIdx);
				m_pDM->SetCellData(_T("우선순위"), tmpArr.GetAt(2), nCurIdx);
				m_pDM->SetCellData(_T("정렬방법"), tmpArr.GetAt(3), nCurIdx);
			}
		}
		File.Close();
	}
	else return -1;

	return 0;
}

INT CSortItemSet::CheckString(CString str)
{
	//공백문자면 return
	if(str.IsEmpty()) return -1;
	TCHAR firstchar = str.GetAt(0);
	
	if(firstchar == '#' || firstchar == '.')
		return -1;
	return 1; 
}

INT CSortItemSet::WriteSortItem(CString strFileName/*=_T("")*/)
{
	if(strFileName.IsEmpty()) strFileName = m_strSortCfgFile;
	
	INT nCnt;
	CString strTmpData, strFolder;
	strFolder = _T("..\\cfg\\SortConfig");
	
	CStdioFile File;
	CFileFind ff;
	strFolder.Replace(_T("\\"), _T("/"));
	ff.FindFile(strFolder);	
    if(!ff.FindFile(strFolder))
	{ 
		if(!CreateDirectory(strFolder.GetBuffer(0), NULL)) 
			return -1000;
	} 
	ff.Close();

	if ( File.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) == NULL) 
	{
		strTmpData.Format(_T("[%s] 파일을 생성할수 없습니다."), strFileName);
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
		
		nCnt = m_pDM->GetRowCount();
		for(INT i=0 ; i<nCnt ; i++)
		{
			strTmpData = m_pDM->GetCellData(_T("이름"), i);
			strTmpData += _T("||") + m_pDM->GetCellData(_T("알리아스"), i);
			strTmpData += _T("||") + m_pDM->GetCellData(_T("우선순위"), i);
			strTmpData += _T("||") + m_pDM->GetCellData(_T("정렬방법"), i);
			strTmpData += _T("\r\n");
			File.WriteString(strTmpData);
		}
		File.Close();
	}

	return 0;
}

INT CSortItemSet::GetSortItemOfPriority(CStringArray *pArray, INT nPriority, CString &strInitValue)
{
	if(pArray == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return 0;
	
	pArray->RemoveAll();
	strInitValue.Empty();
	CString strAlias = _T("이름");
	INT nTmpData;
	for(INT i=0 ; i<m_pDM->GetRowCount() ; i++)
	{
		nTmpData = _ttoi(m_pDM->GetCellData(_T("우선순위"), i).GetBuffer(0));
		if(nPriority <= nTmpData || nTmpData == 0)
		{
			if(nPriority == nTmpData) strInitValue = m_pDM->GetCellData(strAlias, i);
			pArray->Add(m_pDM->GetCellData(strAlias, i));
		}
	}

	if(strInitValue.IsEmpty()) strInitValue = _T("사용안함");
	pArray->Add(_T("사용안함"));
	return 0;
}

INT CSortItemSet::GetSortPriority(INT nPriority)
{
	return _ttoi(m_pDM->GetCellData(_T("우선순위"), nPriority-1).GetBuffer(0));
}

INT CSortItemSet::GetSortDirection(INT nPriority)
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return 0;
	
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(nPriority == _ttoi(m_pDM->GetCellData(_T("우선순위"), i).GetBuffer(0)))
			return _ttoi(m_pDM->GetCellData(_T("정렬방법"), i).GetBuffer(0));
	}
	return 0;
}

INT CSortItemSet::GetSortDirection(CString strTitle)
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return 0;
	
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(strTitle == _ttoi(m_pDM->GetCellData(_T("이름"), i).GetBuffer(0)))
			return _ttoi(m_pDM->GetCellData(_T("정렬방법"), i).GetBuffer(0));
	}
	return 0;
}

INT CSortItemSet::GetSortPriority(CString strTitle)
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return -1;
	
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = m_pDM->GetCellData(_T("이름"), i);
		if(strTmpData == strTitle)
		{
			return _ttoi(m_pDM->GetCellData(_T("우선순위"), i).GetBuffer(0));
		}
	}
	return -1;
}

INT CSortItemSet::GetSortElementIndex(INT nPriority)
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return -1;

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(nPriority == _ttoi(m_pDM->GetCellData(_T("우선순위"), i).GetBuffer(0)))
			return i;
	}
	return -1;
}

INT CSortItemSet::GetSortElementIndex(CString strTitle)
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return -1;
	
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(strTitle == m_pDM->GetCellData(_T("이름"), i))
			return i;
	}
	return -1;
}

INT CSortItemSet::SetSortPriority(CString strTitle, INT nPriority)
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return 0;
	m_pDM->SORT(_T("우선순위"));

	CString strAlias = _T("우선순위");
	CString strTmpData;
	INT nSrcPriority, nTgtIdx, nSrcIdx;
	if(strTitle.IsEmpty() || strTitle == _T("사용안함"))
	{
		for(INT i=0 ; i<nRowCnt ; i++)
		{
			if(nPriority == _ttoi(m_pDM->GetCellData(strAlias, i).GetBuffer(0)))
			{
				m_pDM->SetCellData(strAlias, _T(""), i);
				return 0;
			}
		}
	}
	else
	{
		nSrcPriority = GetSortPriority(strTitle);
		if(nSrcPriority <= 0)
		{
			nSrcIdx = GetSortElementIndex(strTitle);
			if(nSrcIdx < 0) return -1;
			strTmpData.Format(_T("%d"), nPriority);
			m_pDM->SetCellData(strAlias, strTmpData, nSrcIdx);		
		}
		
		nTgtIdx = GetSortElementIndex(nPriority);
		if(nTgtIdx < 0) 
		{
			return -1;
		}
		nSrcIdx = GetSortElementIndex(nSrcPriority);
		if(nSrcIdx < 0)
		{
			return -1;
		}
		strTmpData.Format(_T("%d"), nSrcPriority);
		m_pDM->SetCellData(strAlias, strTmpData, nTgtIdx);
		strTmpData.Format(_T("%d"), nPriority);
		m_pDM->SetCellData(strAlias, strTmpData, nSrcIdx);
	}	
	return 0;
}

INT CSortItemSet::SetSortDirection(INT nSortDirection, INT nPriority)
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return 0;
	m_pDM->SORT(_T("우선순위"));
	CString strAlias = _T("우선순위");
	CString strTmpData;

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(nPriority == _ttoi(m_pDM->GetCellData(strAlias, i).GetBuffer(0)))
		{
			strTmpData.Format(_T("%d"), nSortDirection);
			m_pDM->SetCellData(_T("정렬방법"), strTmpData, i);
			return 0;
		}
	}	

	return 0;
}

INT CSortItemSet::GetSortItem(INT nPriority, CString &strDMAlias)
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(nRowCnt == 0) return 0;
	m_pDM->SORT(_T("우선순위"));
	CString strAlias = _T("우선순위");
	CString strTmpData;
	
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(nPriority == _ttoi(m_pDM->GetCellData(strAlias, i).GetBuffer(0)))
		{
			strDMAlias = m_pDM->GetCellData(_T("알리아스"), i);
			return _ttoi(m_pDM->GetCellData(_T("정렬방법"), i).GetBuffer(0));
		}
	}	
	
	return 0;
}

INT CSortItemSet::SetSortItem(CESL_DataMgr *pDM, CESL_Grid * pGrid)
{
	if(pGrid == NULL || pDM == NULL) return -1;
	if(pDM->GetRowCount() == 0) return 0;

	INT nRowCnt = pGrid->GetRows();
	INT nCurIdx;
	CString strTmpData;
	for(INT i=1 ; i<nRowCnt ; i++)
	{
		strTmpData = pGrid->GetTextMatrix(i, 2);
		if(strTmpData.IsEmpty()) continue;
		m_pDM->InsertRow(-1);
		nCurIdx = m_pDM->GetRowCount()-1;
		SetDM(pDM, i-1, nCurIdx);
	}
	return 0;
}

VOID CSortItemSet::SetSortCfgFileName(CString strSMAlias, CString strGridAlias)
{
	if(strSMAlias.IsEmpty() || strGridAlias.IsEmpty()) return;
	m_strSortCfgFile = _T("..\\cfg\\SortConfig\\") + strSMAlias + _T("_") + strGridAlias + _T(".cfg");
}

INT CSortItemSet::DataSort(CESL_DataMgr *pDM, BOOL bBasic/*=FALSE*/)
{
	if(pDM == NULL) return -1;
	// SORT
	INT nCondColumns = 0;
	CString strAlias[32];
	INT nCond[32];
	
	INT i;
	for(i=0 ; i<m_HeadSortList.GetSize() ; i++)
	{
		strAlias[nCondColumns] = m_HeadSortList.GetAt(i);
		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;
		nCondColumns++;
	}
	
	if(!bBasic)
	{
		CString strDMAlias, strValue;
		INT nSortDirection;
		nSortDirection = GetSortItem(1, strDMAlias);
		//=====================================================
		//2009.02.23 ADD BY PJW : 서명정렬은 정렬용본표제로 정렬한다.
		CString strOrigAlias;
		INT nColumnChk = -1;
		strOrigAlias = _T("");
		if ( strDMAlias == _T("IBS_표제사항") ||
			strDMAlias == _T("ISS_표제사항") ||
			strDMAlias == _T("본표제")       ||
			strDMAlias == _T("서명")         ||
			strDMAlias == _T("IBS_본표제")   ||
			strDMAlias == _T("TITLE_INFO")   ||
			strDMAlias == _T("표제사항")	  ||
			strDMAlias == _T("UDF_표제사항")	)
		{				
			nColumnChk = pDM->FindColumn(_T("정렬용본표제"));
			if ( nColumnChk >= 0 )
			{
				strOrigAlias = strDMAlias;
				strDMAlias = _T("정렬용본표제");
			}
		}
		//=====================================================

		if(nSortDirection != 0)
		{
			if(m_MapConvertUDFToRealAlias.Lookup(strDMAlias, strValue)) strDMAlias = strValue;
			strAlias[nCondColumns] = strDMAlias;	nCond[nCondColumns] = nSortDirection;		nCondColumns++;
		}
		nSortDirection = GetSortItem(2, strDMAlias);
		if(nSortDirection != 0)
		{
			if(m_MapConvertUDFToRealAlias.Lookup(strDMAlias, strValue)) strDMAlias = strValue;
			strAlias[nCondColumns] = strDMAlias;	nCond[nCondColumns] = nSortDirection;		nCondColumns++;
		}
		nSortDirection = GetSortItem(3, strDMAlias);
		if(nSortDirection != 0)
		{
			if(m_MapConvertUDFToRealAlias.Lookup(strDMAlias, strValue)) strDMAlias = strValue;
			strAlias[nCondColumns] = strDMAlias;	nCond[nCondColumns] = nSortDirection;		nCondColumns++;
		}
		nSortDirection = GetSortItem(4, strDMAlias);
		if(nSortDirection != 0)
		{
			if(m_MapConvertUDFToRealAlias.Lookup(strDMAlias, strValue)) strDMAlias = strValue;
			strAlias[nCondColumns] = strDMAlias;	nCond[nCondColumns] = nSortDirection;		nCondColumns++;
		}
		//=====================================================
		//2009.02.23 ADD BY PJW : 서명정렬은 정렬용본표제로 정렬한다.
		if ( strOrigAlias != _T("") && nColumnChk >= 0 )
		{
			strDMAlias = strOrigAlias;
		}
		//=====================================================
		
	}
	
	for(i=0 ; i<m_TailSortList.GetSize() ; i++)
	{
		strAlias[nCondColumns] = m_TailSortList.GetAt(i);
		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;
		nCondColumns++;
	}
	
	return pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
}

BOOL CSortItemSet::IsInit()
{
	if(m_pDM->GETDMAlias().IsEmpty() || m_strSortCfgFile.IsEmpty() || m_pGrid == NULL)
		return FALSE;
	return TRUE;
}

BOOL CSortItemSet::IsExist(CString strTitle)
{
	if(strTitle.IsEmpty()) return FALSE;
	INT nRowCnt = m_pDM->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(strTitle == m_pDM->GetCellData(_T("이름"), i))
			return TRUE;
	}
	return FALSE;
}

INT CSortItemSet::DataSortOfSelectedColumn(CESL_DataMgr * pDM, CString strDMAlias, INT nSortDirection)
{
	if(pDM == NULL) return -1;
	// SORT
	INT nCondColumns = 0;
	CString strAlias[32];
	INT nCond[32];
	CString strValue;
	
	INT i;
	for(i=0 ; i<m_HeadSortList.GetSize() ; i++)
	{
		strAlias[nCondColumns] = m_HeadSortList.GetAt(i);
		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;
		nCondColumns++;
	}
	
	if(m_MapConvertUDFToRealAlias.Lookup(strDMAlias, strValue)) strDMAlias = strValue;
	strAlias[nCondColumns] = strDMAlias;	nCond[nCondColumns] = nSortDirection;		nCondColumns++;
	
	for(i=0 ; i<m_TailSortList.GetSize() ; i++)
	{
		strAlias[nCondColumns] = m_TailSortList.GetAt(i);
		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;
		nCondColumns++;
	}
	
	return pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
}
