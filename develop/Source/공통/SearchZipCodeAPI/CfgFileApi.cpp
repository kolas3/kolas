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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// cpp source //////////////////////////////////////////////////
/*
@Create
 - 2010.03.17 BY PWR
@Description
 - cfg파일을 설정
@Parameter
 - strFile : cfg파일 경로 및 이름
@Return Value
 - 
*/
INT CCfgFileApi::SetFile(CString strFile)
{
	m_strFile = strFile;
	return 0;
}

/*
@Create
 - 2010.03.17 BY PWR
@Description
 - cfg파일이름을 리턴
@Parameter
 - 
@Return Value
 - cfg파일 경로 및 이름
*/
CString CCfgFileApi::GetFile()
{
	return m_strFile;
}

/*
@Description
 - File에서 Alias의 Index를 가져온다
@Return Value
 - 0 : 정상
 - 1010 : data find 실패
*/
INT CCfgFileApi::GetData(CString strAlias, CString &strValue)
{
	CStringList pStringList;
	CString strFindAlias;
	CString strLine;
	CString strTemp;
	INT ids;
	INT nFind;	

	// 1. ReadFile()
	ids = ReadFile(pStringList);
	if ( ids ) return ids;		

	strValue = _T("");
	strFindAlias = strAlias+_T("|");

	// 2. FIND
	POSITION pos = pStringList.GetHeadPosition();
	while( pos )
	{
		strLine = pStringList.GetAt( pos );		

		// 3. strAlias Find.
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

/*
@Description
 - File에 Alias가 있으면 Index를 수정, 없으면 Alias 추가
@Return Value
 - 0 : 정상
 - 그 외 : 실패
*/
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

	// 1. ReadFile()
	ids = ReadFile(pStringList);
	if ( ids ) return ids;		

	strFindAlias = strAlias+_T("|");			
	
	BOOL bFind;
	bFind = FALSE;
	POSITION pos = pStringList.GetHeadPosition();
	while( pos )
	{
		strLine = pStringList.GetAt(pos);
		
		// 2. strAlias Find.
		nFind = strLine.Find(strFindAlias,0);
		if( 0 <= nFind )
		{
			// 3. IF strAlias = fileReadAlias THEN fileReadValue = strValue
			nFind = strLine.Find(_T("|"),0);
			strTmp = strLine.Left(nFind+1) + strValue;			
			pStringList.SetAt(pos, strTmp);
			bFind = TRUE;
			break;
		}

		pStringList.GetNext( pos );		
	}
	
	// 4. IF NOTFOUND THEN strArrr.add()	
	if ( bFind == FALSE )
	{
		strTmp = strAlias + _T("|") + strValue;
		pStringList.AddTail(strTmp);
	}	

	// 5. WriteFile()
	WriteFile(pStringList);
	
	return 0;
}

/*
@Description
 - File write
@Return Value
 - 0 : 정상
 - -1010: File open 실패
*/
INT CCfgFileApi::WriteFile(CStringList &pStringList)
{
	// 2010.05.28 UPDATE BY PWR : 지우고 다시 만들도록
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
// 	// 1.File open
// 	CStdioFile	fFile;
// 	CString strLine;
// 	if( fFile.Open( GetFile() ,  CFile::modeReadWrite | CFile::typeBinary) == FALSE ) 
// 	{
// 		AfxMessageBox( GetFile() );
// 		return -1010;
// 	}
// 	
// 	// 2.File write
// 	TCHAR cUni;	
// 	cUni = 0xFEFF;
// 
// 	fFile.SeekToBegin();
// 	fFile.Write(&cUni,sizeof(TCHAR));
// 	POSITION pos = pStringList.GetHeadPosition();
// 	while( pos )
// 	{
// 		strLine = pStringList.GetNext( pos );
// 		fFile.WriteString( strLine );
// 		fFile.WriteString( _T("\r\n") );
// 	}
// 
// 	fFile.Close();

	return 0;
}

/*
@Description
 - File read
@Return Value
 - 0 : 정상
 - -1010 : File open 실패
 - -1020 : File에 Data 없음
*/
INT CCfgFileApi::ReadFile(CStringList &pStringList)
{
	// 1.File open
	CStdioFile	fFile;	
	
	if( fFile.Open( GetFile() ,  CFile::modeReadWrite | CFile::typeBinary) == FALSE ) 
	{
		AfxMessageBox( GetFile() );
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
	{/// Unicode 아니라면
		fFile.SeekToBegin();
		fFile.Read( &cUni, 3 );
		if(  0xBBEF == cUni  )
		{/// UTF일 경우
			//fFile.SetCodePage(CP_UTF8);
		}
		else
		{/// ANSI일 경우
			//fFile.SetCodePage(CP_ACP);
			fFile.SeekToBegin();
		}
	}

	// 2. File read
	CString strFileList,strLine,strTemp;
	CStringList fileContents;
	while( fFile.ReadString( strLine ) )
	{
		strLine.TrimLeft();		strLine.TrimRight();
		//DEL strLine.Replace( _T(" ") , _T("") );
		if( !strLine.GetLength() )
		{
			continue;
		}

		// 3. Data 저장
		pStringList.AddTail( strLine );
	}

	// 2010.09.06 ADD BY PWR : CLOSE
	fFile.Close();

	return 0;
}


BEGIN_MESSAGE_MAP(CCfgFileApi, CWnd)
	//{{AFX_MSG_MAP(CCfgFileApi)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCfgFileApi message handlers
