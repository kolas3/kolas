// 18.10.30 JMJ KOL.RED.2018.021
#include "stdafx.h"
#include "ExportToCSV.h"
#include "StdioFileEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CExportToCSV::CExportToCSV()
{
	m_nColCnt = 0;
	m_nRowCnt = 0;
	m_strData.Empty();
	m_aColumnList.RemoveAll();
}

CExportToCSV::~CExportToCSV()
{
}

// CSV파일 열기 (탭단위)
BOOL CExportToCSV::Open(CString strFileName)
{
	CStdioFileEx	fFile;

	if( fFile.Open( strFileName ,  CFile::modeRead | CFile::typeText) == FALSE ) 
	{
		return FALSE;
	}
	INT nFileLength = fFile.GetLength();
	if( 1 > nFileLength )
	{
		return FALSE;
	}
		
	TCHAR cUni;
	fFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{/// Unicode 아니라면
		fFile.SeekToBegin();
		fFile.Read( &cUni, 3 );
		if(  0xBBEF == cUni  )
		{/// UTF일 경우
			fFile.SetCodePage(CP_UTF8);
		}
		else
		{/// ANSI일 경우
			//fFile.SetCodePage(CP_ACP);
			fFile.SeekToBegin();
		}
	}

	CString strLine;
	CString strTemp, strColumnData;
	m_strData.Empty();

	INT nColCnt = 0, nTempColCnt = 0, nRowCnt = 0, i = 0;
	while( fFile.ReadString( strLine ) )
	{
		strLine.TrimLeft();		strLine.TrimRight();

		strTemp = strLine;
		strTemp.Replace(_T("\t"), _T(""));
		nTempColCnt = strLine.GetLength() - strTemp.GetLength() + 1;

		if(m_aColumnList.GetSize() > 0)
		{
			nRowCnt++;
			m_aData.Add(strLine);
		}
		if(nColCnt == 0)
		{
			nColCnt = nTempColCnt;

			while(AfxExtractSubString(strColumnData, strLine, i, '\t'))
			{
				m_aColumnList.Add(strColumnData);
				i++;
			}
		}
		if(nColCnt != nTempColCnt)
		{
			AfxMessageBox(_T("데이터가 올바르지 않습니다.\r\n탭단위로 구분된 CSV형식인지 확인하여주십시오."));
			break;
		}
		
		
		m_strData += strLine;
	}
	SetColCount(nColCnt);
	SetRowCount(nRowCnt);

	fFile.Close();

	if(nColCnt != nTempColCnt)
	{
		return FALSE;
	}
	return TRUE;
}

VOID CExportToCSV::GetAlias(CStringArray &saColumn)
{
	INT i = 0;

	saColumn.RemoveAll();
	for(i = 0; i < m_aColumnList.GetSize(); i++)
	{
		saColumn.Add(m_aColumnList.GetAt(i));
	}

}

// CSV파일 저장 (기본형 데이터)
/*
	예시 strData

	1번컬럼\t2번컬럼\t3번컬럼\r\n
	1번데이터\t2번데이터\t3번데이터

	= GetColCount() = 3, GetRowCount() = 1
*/
BOOL CExportToCSV::Save(CString strFileName, CString strData)
{
	CFile fFile;
	if( fFile.Open( strFileName ,  CFile::modeCreate | CFile::modeWrite ) == FALSE ) 
	{
		AfxMessageBox( strFileName );
		return FALSE;
	}
	USHORT nShort = 0xfeff;  // Unicode BOM(Byte Order Mark).
	fFile.Write(&nShort,2);
	fFile.Write(strData, strData.GetLength()*2);
	fFile.Close();
	return TRUE;
}

VOID CExportToCSV::SetColCount(INT nColCnt)
{
	m_nColCnt = nColCnt;
}

VOID CExportToCSV::SetRowCount(INT nRowCnt)
{
	m_nRowCnt = nRowCnt;
}

// Open -> GetColCount()
INT CExportToCSV::GetColCount()
{
	return m_nColCnt;
}

// Open -> GetRowCount();
INT CExportToCSV::GetRowCount()
{
	return m_nRowCnt;
}

VOID CExportToCSV::GetCellData(CString strAlias, INT nRowIndex, CString &strResult)
{
	INT i = 0, nColIndex = 0;

	if(m_nColCnt < 1 || m_nRowCnt < 1)
		return;

	CString strRowData;
	strRowData = m_aData.GetAt(nRowIndex);

	for(i = 0; i < m_nColCnt; i++)
	{
		if(strAlias.Compare(m_aColumnList.GetAt(i)) == 0)
		{
			nColIndex = i;
			break;
		}
	}

	AfxExtractSubString(strResult, strRowData, nColIndex, '\t');
}

VOID CExportToCSV::GetCellData(INT nRowIndex, INT nColIndex, CString &strResult)
{
	if(m_nColCnt < 1 || m_nRowCnt < 1)
		return;

	CString strRowData;
	strRowData = m_aData.GetAt(nRowIndex);

	AfxExtractSubString(strResult, strRowData, nColIndex, '\t');
}

VOID CExportToCSV::SetCellData(CString strAlias, CString strResult, INT nRowIndex)
{
	CString strTemp;
	INT i = 0, nColIndex = 0;

	if(m_nColCnt < 1 || m_nRowCnt < 1)
		return;

	CString strRowData;
	strRowData = m_aData.GetAt(nRowIndex);

	for(i = 0; i < m_nColCnt; i++)
	{
		if(strAlias.Compare(m_aColumnList.GetAt(i)) == 0)
		{
			nColIndex = i;
			break;
		}
	}

	CString strPrevData, strAliasData, strNextData;
	for(i = 0;i < m_nColCnt;i++)
	{
		strTemp.Empty();
		AfxExtractSubString(strTemp, strRowData, i, '\t');
		if(i < nColIndex)
		{
			strPrevData += strTemp;
			if(i < m_nColCnt && i < nColIndex) strPrevData += _T("\t");
		}
		else if(i == nColIndex)
		{
			strAliasData = strTemp;
		}
		else if(i > nColIndex)
		{
			strNextData += strTemp;
			if(i < m_nColCnt && i > nColIndex) strNextData += _T("\t");
		}
	}

	m_aData.SetAt(nRowIndex, strPrevData + strAliasData + _T("\t") + strNextData);
}