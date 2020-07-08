// CfgFileApi.cpp : implementation file
//

#include "stdafx.h"
#include "CfgFileApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCfgFileApi

CCfgFileApi::CCfgFileApi()
{
	m_strFile = _T("");
}

CCfgFileApi::~CCfgFileApi()
{
}

INT CCfgFileApi::SetFile(CString strFile)
{
	m_strFile = strFile;
	return 0;
}

CString CCfgFileApi::GetFile()
{
	return m_strFile;
}

INT CCfgFileApi::GetData(CString strAlias, CString &strValue)
{
	CStringList pStringList;
	CString strFindAlias;
	CString strLine;
	CString strTemp;
	INT ids;
	INT nFind;	

	ids = ReadFile(pStringList);
	if ( ids ) return ids;		

	strValue = _T("");
	strFindAlias = strAlias+_T("|");

	POSITION pos = pStringList.GetHeadPosition();
	while( pos )
	{
		strLine = pStringList.GetAt( pos );		

		nFind = strLine.Find(strFindAlias,0);
		if( 0 <= nFind )
		{
			AfxExtractSubString( strTemp , strLine , 1 , '|' );
			strValue = strTemp;
			return 0;
		}

		pStringList.GetNext( pos );		
	}

	return 1010;
}

INT CCfgFileApi::SetData(CString strAlias, CString strValue)
{	
	CStringList pStringList;
	CString strFindAlias;
	CString strLine;
	CString strTmp;
	INT ids;
	INT nFind;

	strAlias.TrimLeft();
	strAlias.TrimRight();

	strValue.TrimLeft();
	strValue.TrimRight();

	ids = ReadFile(pStringList);
	if ( ids ) return ids;		

	strFindAlias = strAlias+_T("|");			
	
	BOOL bFind;
	bFind = FALSE;
	POSITION pos = pStringList.GetHeadPosition();
	while( pos )
	{
		strLine = pStringList.GetAt(pos);
		
		nFind = strLine.Find(strFindAlias,0);
		if( 0 <= nFind )
		{
			nFind = strLine.Find(_T("|"),0);
			strTmp = strLine.Left(nFind+1) + strValue;			
			pStringList.SetAt(pos, strTmp);
			bFind = TRUE;
			break;
		}

		pStringList.GetNext( pos );		
	}
	
	if ( bFind == FALSE )
	{
		strTmp = strAlias + _T("|") + strValue;
		pStringList.AddTail(strTmp);
	}	

	WriteFile(pStringList);
	
	return 0;
}

INT CCfgFileApi::WriteFile(CStringList &pStringList)
{
	DeleteFile(GetFile());
	FILE *fp = _tfopen(GetFile(), _T("w+b"));
	fputc(0xFF, fp); fputc(0xFE, fp);
	fseek(fp, 0, SEEK_END);
	CString strLine;
	POSITION pos = pStringList.GetHeadPosition();
 	while(pos)
 	{
 		strLine = pStringList.GetNext(pos) + _T("\r\n");
		_ftprintf(fp, _T("%s"), strLine);
 	}
	fclose(fp);

	return 0;
}

INT CCfgFileApi::ReadFile(CStringList &pStringList)
{
	CStdioFile	fFile;	
	
	if( fFile.Open( GetFile() ,  CFile::modeReadWrite | CFile::typeBinary) == FALSE ) 
	{
		return -1010;
	}

	INT nFileLength = fFile.GetLength();
	if( 1 > nFileLength )
	{
		AfxMessageBox( _T("비어 있는 File 입니다. File을 확인하여 주십시요.") , MB_ICONINFORMATION );
		return -1020;
	}

	TCHAR cUni;
	fFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fFile.SeekToBegin();
		fFile.Read( &cUni, 3 );
		if(  0xBBEF == cUni  )
		{

		}
		else
		{
			fFile.SeekToBegin();
		}
	}

	CString strFileList,strLine,strTemp;
	CStringList fileContents;
	while( fFile.ReadString( strLine ) )
	{
		strLine.TrimLeft();		strLine.TrimRight();

		if( !strLine.GetLength() )
		{
			continue;
		}

		pStringList.AddTail( strLine );
	}

	fFile.Close();

	return 0;
}

INT CCfgFileApi::GetLineCount()
{	
	CStringList pStringList;
	CString strLine;
	INT ids;
	INT nCnt = 0;

	ids = ReadFile(pStringList);
	if ( ids ) return ids;		

	POSITION pos = pStringList.GetHeadPosition();
	while( pos )
	{
		strLine = pStringList.GetAt( pos );		
		nCnt++;
		pStringList.GetNext( pos );		
	}

	return nCnt;
}

CString CCfgFileApi::GetAlias(INT nLine)
{	
	CString strAlias = _T("");
	CStringList pStringList;
	CString strLine;
	INT ids;
	INT nCnt = 0;

	ids = ReadFile(pStringList);
	if ( ids ) return ids;		

	POSITION pos = pStringList.GetHeadPosition();
	while( pos )
	{
		strLine = pStringList.GetAt( pos );	
		ids = strLine.Find(_T("|"));
		if(0 < ids) strAlias = strLine.Left(ids);
		if(nLine == nCnt) break;
		nCnt++;
		pStringList.GetNext( pos );		
	}

	return strAlias;
}


BEGIN_MESSAGE_MAP(CCfgFileApi, CWnd)
	//{{AFX_MSG_MAP(CCfgFileApi)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCfgFileApi message handlers
