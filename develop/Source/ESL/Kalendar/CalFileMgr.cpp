// CalFileMgr.cpp: implementation of the CCalFileMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kalendar.h"
#include "CalFileMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalFileMgr::CCalFileMgr()
{
	
}

CCalFileMgr::~CCalFileMgr()
{

}

// 파일작성
BOOL CCalFileMgr::WriteMemoFile(CString stFileName, CString stData)
{		
	if(_T("") == stFileName)
	{
		LOG(_T("WriteMemoFile() : 파일이름이 없습니다."));				
		return FALSE;
	}
	CString strDir = _T("");
	Get_FILEDIR(strDir);
	ChkDir(strDir);
	stFileName = strDir + stFileName;

	if(!IsData(stData))
	{
		::DeleteFile(stFileName);
		return TRUE;
	}
	CString stTempFile = stFileName + _T("_Temp");	

	CFile file;
	BOOL bChkError = file.Open(stTempFile,CFile::modeCreate|CFile::modeWrite);	
	if(FALSE==bChkError)
	{
		CString strTmp = stTempFile;
		stTempFile = _T("WriteMemoFile(") + stTempFile + _T(") : 파일열기 에러");
		LOG(strTmp);	
		return FALSE;
	}

	if(MAXBUFFER<stData.GetLength())
	{
		CString st;
		st.Format(_T("글자수를 %d자 이내로  작성하여 주십시요."), MAXBUFFER);
		AfxMessageBox(st);
		return FALSE;
	}

	TCHAR tchar = 0xFEFF;
	file.Write(&tchar, sizeof(TCHAR) );
	file.Write(LPCTSTR(stData), stData.GetLength()* sizeof(TCHAR));		
	if(TRUE == IsFile(stFileName))
	{
		file.Remove(stFileName);	
	}

	file.Close();
	file.Rename(stTempFile, stFileName);
	return TRUE;
}

// 파일 읽기
BOOL CCalFileMgr::ReadMemoFile(CString stFileName, CString* stText)
{
	if(_T("") == stFileName)
	{
		LOG(_T("ReadMemoFile() : 메모파일이름이 없습니다."));		
		return FALSE;
	}	
	CString strDir = _T("");
	Get_FILEDIR(strDir);
	ChkDir(strDir);
	stFileName = strDir + stFileName;

	CFile file;	
	BOOL bChkError = file.Open(stFileName, CFile::modeRead);	
	if(false==bChkError)
	{
		CString strTmp = stFileName;
		strTmp = _T("ReadMemoFile(") + stFileName + _T(") : 파일열기 에러");
		LOG(strTmp);				
		return FALSE;
	}	
	TCHAR tchar;
	file.Read(&tchar, sizeof(TCHAR) );
	// 유니코드가 아니면
	if(0xFEFF!=tchar)
	{
		file.Close();
		return FALSE;
	}

	TCHAR szBuf[MAXBUFFER];
	ZeroMemory(szBuf, MAXBUFFER);

	file.Read(szBuf, file.GetLength());	

	stText->Empty();
	stText->Format(_T("%s"), szBuf);
	file.Close();
	return TRUE;
}

// 해당 메모파일이 있는지 반환
BOOL CCalFileMgr::IsFile(CString stFileName)
{
	CFileFind find;
	int Chk = stFileName.Find(_T("\\"));
	if(-1==Chk)
	{
		CString strDir = _T("");
		Get_FILEDIR(strDir);
		ChkDir(strDir);
		stFileName = strDir + stFileName;
	}
	return find.FindFile(stFileName);
}

// 해당 메모파일이 있는지를 반환
BOOL CCalFileMgr::IsFile(UINT uYear, UINT uMonth, UINT uDay)
{
	CString stFileName = GetMemoFileName(uYear,uMonth, uDay);	
	return IsFile(stFileName);
}

// 메모파일이름 지정
CString CCalFileMgr::GetMemoFileName(UINT uYear, UINT uMonth, UINT uDay)
{
 	CString stFileName;
 	stFileName.Format(MEMOFILENAME, uYear, uMonth, uDay);
 	return stFileName;
}

// 메모파일 불러오기
BOOL CCalFileMgr::ReadMemoFile(UINT uYear, UINT uMonth, UINT uDay, CString *stText)
{
	CString stFileName = GetMemoFileName(uYear, uMonth, uDay);
	return ReadMemoFile(stFileName, stText);
}

// 메모파일 기록
BOOL CCalFileMgr::WriteMemoFile(UINT uYear, UINT uMonth, UINT uDay, CString stText)
{
	CString stFileName = GetMemoFileName(uYear, uMonth, uDay);
	return WriteMemoFile(stFileName, stText);
}

// 검색된 메모를 추가
void CCalFileMgr::AddSearchMemoList(UINT uYear, UINT uMonth, UINT uDay, CString stText)
{
	CMemo* pMemo = new CMemo;
	pMemo->SetMemo(uYear, uMonth, uDay, stText);
	m_aFileForSearch.Add(pMemo);
}

// 검색된 파일의 갯수 리턴
int CCalFileMgr::GetSearchFileSize()
{
	return m_aFileForSearch.GetSize();
}

// 검색된 파일목록 삭제
void CCalFileMgr::ResetSearchMemoList()
{
	m_aFileForSearch.RemoveAll();
}

// stFileName의 이름을 가진 파일에서 stSearchText문자가 포함되어 있는지 여부를 반환
// 함수이름: IsSearchTextInFile
// 함수설명: 특정파일에서 특정문자가 포함되어있는지 여부를 판단
// 반환  형: BOOL
// 인자[CString stFileName] : 검색할 파일의 이름
// 인자[CString stSearchText] : 검색할 텍스트
BOOL CCalFileMgr::IsSearchTextInFile(CString stFileName, CString stSearchText)
{
	CString stText;
	ReadMemoFile(stFileName, &stText);		
	if(-1==stText.Find(stSearchText))
		return FALSE;
	return TRUE;
}

// 

// 함수이름: SearchTextAndAddResultInSearchMemoList
// 함수설명: 특정 년/월의 텍스트파일을 읽어들여 검색하는 텍스트가 들어있는 메모를 리스트에추가
// 반환  형: void 
// 인자[UINT uYear] : 검색할 년도
// 인자[UINT uMonth] : 검색할 월
// 인자[CString stSearchText] : 찾고자하는 문자열
void CCalFileMgr::SearchTextAndAddResultInSearchMemoList(UINT uYear, UINT uMonth, CString stSearchText)
{	
	// 해당달의 일자수를 리턴 EX>>3월의 경우 31;
	UINT Max = CKoreanDate::GetDaysOfMonth(uYear, uMonth);
	CString stFileName, stText;
	for(UINT uDay =1;uDay<=Max;uDay++)
	{
		stFileName = GetMemoFileName(uYear, uMonth, uDay);
		if(IsSearchTextInFile(stFileName, stSearchText))
		{
			ReadMemoFile(stFileName, &stText);
			AddSearchMemoList(uYear, uMonth, uDay, stText);
		}
	}
}

// 함수이름: SearchTextAndAddResultInSearchMemoList
// 함수설명: 특정 년의 텍스트파일을 읽어들여 검색하는 텍스트가 들어있는 메모를 리스트에추가
// 반환  형: void 
// 인자[UINT uYear] : 검색할 년도
// 인자[CString stSearchText] : 검색할 문자열
void CCalFileMgr::SearchTextAndAddResultInSearchMemoList(UINT uYear, CString stSearchText)
{
	for(int uMonth=1;uMonth<=12;uMonth++)
	{
		SearchTextAndAddResultInSearchMemoList(
			uYear, uMonth, stSearchText);
	}
}

CMemo* CCalFileMgr::GetSearchMemoListAt(UINT uIndex)
{
	CMemo* pMemo = (CMemo*) m_aFileForSearch.GetAt(uIndex);
	return pMemo;
}

UINT CCalFileMgr::GetSearchMemoListCount()
{
	return m_aFileForSearch.GetSize();
}

void CCalFileMgr::ChkDir(CString strDir)
{
	CFileFind file;
	CString strFile = _T("*.*");
	BOOL bResult = file.FindFile(strDir + strFile);	

	if(!bResult)		
	{		
		bResult = CreateDirectory(strDir+_T("\\"),NULL);		
		if(!bResult)			
		{	
			//Error;			
		}		
	}   
}


// 함수이름: IsData
// 함수설명: 특정 문자열에 데이터가 들어있는지 여부판단
// 반환  형: BOOL 
// 인자[CString stData] : 데이터의 유무를 판단하고자 하는 문자열
BOOL CCalFileMgr::IsData(CString stData)
{
	CString stTemp = stData;
	stTemp.TrimLeft();
	stTemp.TrimLeft(_T("\r"));
	stTemp.TrimLeft(_T("\n"));
	stTemp.TrimLeft(_T("\t"));
	if(_T("")==stTemp)	
		return FALSE;	
	return TRUE;
}

// 함수이름: WriteConfigFile
// 함수설명: 설정파일을 기록
// 반환  형: BOOL 
// 인자[STRUCT_CFGDATA CFGDATA] : 저장하고자 하는 설정값이 들어있는 데이터
BOOL CCalFileMgr::WriteConfigFile(STRUCT_CFGDATA CFGDATA)
{
//////////////////////////////////////////////////////////////
//여기서부터 오류체크
	CString stFileName = GetConfigFileName();
	if(_T("") == stFileName)
	{
		LOG(_T("WriteConfigFile() : 설정파일이름이 없습니다."));				
		return FALSE;
	}
	CString strDir = _T("");
	Get_FILEDIR(strDir);
	ChkDir(strDir);
	stFileName = strDir + stFileName;

	CString stTempFile = stFileName + _T("_Temp");	

	CFile file;
	BOOL bChkError = file.Open(stTempFile,CFile::modeCreate|CFile::modeWrite);	
	if(false==bChkError)
	{
		CString strTmp = stTempFile;
		strTmp = _T("WriteConfigFile(") + stTempFile + _T(") : 파일열기 에러");		
		return FALSE;
	}
//여기까지 오류체크
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// 여기서부터 설정 파일기록
	CString stData;	
	TCHAR tchar = 0xFEFF;	// 유니코드
	file.Write(&tchar, sizeof(TCHAR) );

	// STRUCT_CFGDATA 의 멤버변수를 줄단위로 기록
	stData.Format(_T("%d\r\n"), CFGDATA.nX);
	file.Write(LPCTSTR(stData), stData.GetLength()* sizeof(TCHAR));		
	stData.Format(_T("%d\r\n"), CFGDATA.nY);
	file.Write(LPCTSTR(stData), stData.GetLength()* sizeof(TCHAR));		
	stData.Format(_T("%d\r\n"), CFGDATA.nBorderPos);
	file.Write(LPCTSTR(stData), stData.GetLength()* sizeof(TCHAR));
	stData.Format(_T("%d\r\n"), CFGDATA.nMode);
	file.Write(LPCTSTR(stData), stData.GetLength()* sizeof(TCHAR));	
// 여기까지가 설정 파일기록
//////////////////////////////////////////////////////////////
	if(IsFile(stFileName))
		file.Remove(stFileName);	
	file.Close();
	file.Rename(stTempFile, stFileName);
	return TRUE;
}

// 함수이름: ReadConfigFile
// 함수설명: 설정파일을 읽어옴
// 반환  형: BOOL 
// 인자[STRUCT_CFGDATA *pCFGDATA] : 받아온 설정이 저장될 데이터
BOOL CCalFileMgr::ReadConfigFile(STRUCT_CFGDATA *pCFGDATA)
{
//////////////////////////////////////////////////////////////
//여기서부터 오류체크
	CString stFileName = GetConfigFileName();
	if(_T("") == stFileName)
	{
		LOG(_T("ReadConfigFile() : 설정파일이름이 없습니다."));		
		return FALSE;
	}	
	CString strDir = _T("");
	Get_FILEDIR(strDir);
	ChkDir(strDir);
	stFileName = strDir + stFileName;

	CFile file;	
	BOOL bChkError = file.Open(stFileName, CFile::modeRead);	
	if(false==bChkError)
	{
		CString strTmp = stFileName;
		strTmp = _T("ReadConfigFile(") + strTmp + _T(") : 설정파일 열기 에러");		
		return FALSE;
	}
//여기까지 오류체크
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// 여기서부터 설정 파일읽기
	TCHAR szBuf[sizeof(STRUCT_CFGDATA)];
	for(UINT i=0;i<sizeof(STRUCT_CFGDATA);i++)
	{
		szBuf[i] = 0;
	}

	TCHAR tchar;
	file.Read(&tchar, sizeof(TCHAR) );
	file.Read(szBuf, file.GetLength());	

	CString stCfg = _T("");
	for(i=0;i<file.GetLength();i++)
	{
		stCfg += szBuf[i];
	}
	
	int nOffset = 0;	
	int nCfg[4] = {0, 0, 0, 0};	
	// 3은 STRUCT_CFGDATA의 멤버변수 숫자
	// 각각의 변수는 줄 단위로 기록되어 있다.
	for(i=0;i<4;i++)
	{
		nOffset = stCfg.Find(_T("\r\n"));
		if(0>=nOffset)
			nOffset = 0;
		else
		{			
			nCfg[i] = _ttoi(LPTSTR(LPCTSTR(stCfg.Left(nOffset))));
			stCfg = stCfg.Mid(nOffset+2);
		}
	}
// 여기까지 설정 파일읽기
//////////////////////////////////////////////////////////////

	pCFGDATA->nX = nCfg[0];
	pCFGDATA->nY = nCfg[1];
	pCFGDATA->nBorderPos = nCfg[2];
	pCFGDATA->nMode = nCfg[3];

	file.Close();
	return TRUE;
}

CString CCalFileMgr::GetConfigFileName()
{
	return CONFIGFILENAME;
}

BOOL CCalFileMgr::WriteLogFile(CString stData, CString stFileName)
{	
	if(_T("") == stFileName)
	{
		stFileName = _T("Kalendar_NoLogFileName.log");
		return FALSE;
	}
	CString strDir = _T("");
	Get_FILEDIR(strDir);
	ChkDir(strDir);
	stFileName = strDir + stFileName;

	CString stTempFile = stFileName + _T("_Temp");	

	CFile file;			
	BOOL bChkError = FALSE;
	if(IsFile(stFileName))
	{
		bChkError = file.Open(stFileName,CFile::modeReadWrite);	
		if(FALSE==bChkError) return FALSE;
	}
	else
	{
		bChkError = file.Open(stFileName,CFile::modeCreate|CFile::modeReadWrite);	
		if(FALSE==bChkError) return FALSE;
		TCHAR tchar = 0xFEFF;
		file.Write(&tchar, sizeof(TCHAR) );		
	}
	file.SeekToEnd();	

	if(MAXBUFFER<stData.GetLength())
	{
		CString st;
		st.Format(_T("글자수를 %d자 이내로  작성하여 주십시요."), MAXBUFFER);
		AfxMessageBox(st);
		return FALSE;
	}
	stData += _T("\r\n");
	file.SeekToEnd();	
	file.Write(LPCTSTR(stData), stData.GetLength()* sizeof(TCHAR));		
	file.Close();

	return TRUE;
}

CString CCalFileMgr::GetLogFileName()
{
	return LOGFILENAME;
}

BOOL CCalFileMgr::WriteLogFile(CString stData)
{
	return WriteLogFile(stData, GetLogFileName());
}

void CCalFileMgr::DeleteLogFile()
{
	CString strDir = _T("");
	Get_FILEDIR(strDir);
	ChkDir(strDir);
	CString strLogFileName = strDir + GetLogFileName();

	if(TRUE == IsFile(strLogFileName)) ::DeleteFile(strLogFileName);
}

BOOL CCalFileMgr::WriteMemoFile(CData *pData)
{
	UINT uYear = 0;
	UINT uMonth = 0;
	UINT uDay = 0;
	pData->GetDate(&uYear, &uMonth, &uDay);
	CString strFileName = GetMemoFileName(uYear, uMonth, uDay);
	return WriteMemoFile(strFileName, pData->GetText());
}

BOOL CCalFileMgr::ReadMemoFile(CData *pData)
{
	UINT uYear = 0;
	UINT uMonth = 0;
	UINT uDay = 0;	
	pData->GetDate(&uYear, &uMonth, &uDay);
	CString strFileName = GetMemoFileName(uYear, uMonth, uDay);
	
	CString strText = _T("");
	if(FALSE == ReadMemoFile(strFileName, &strText)) return FALSE;
	pData->SetText(strText);
	return TRUE;
}

INT CCalFileMgr::ReadKolasHandleFile(CString strFileName, HWND *hWnd)
{
	if(_T("") == strFileName) { strFileName = GetHandleFileName(); };

	CStdioFile file;	
	BOOL bChkError = file.Open(strFileName, CFile::modeRead);	
	if(FALSE == bChkError)
	{
		CString strTmp = strFileName;
		strTmp = _T("ReadKolasHandleFile") + strFileName + _T(") : 파일열기 에러");
		LOG(strTmp);				
		return -1;
	}	

	CString strText;
	file.ReadString(strText);

	INT nWnd = _ttoi(strText);
	if(NULL != hWnd) *hWnd = HWND(nWnd);
	file.Close();
	
	return 0;
}

CString CCalFileMgr::GetHandleFileName()
{
	return HANDLEFILENAME;
}

// 내 문서에 메모파일을 저장한다.
int CCalFileMgr::Get_FILEDIR(CString &strFile)
{
 	TCHAR szSpecialPath[MAX_PATH] = {0};
 	SHGetSpecialFolderPath(NULL, szSpecialPath, CSIDL_PERSONAL, FALSE);
	strFile.Format(_T("%s\\Kalendar\\"), szSpecialPath);

	// 이전에 kolasiii폴더에 메모파일을 쓰고 있는 사용자도 있기 때문에
	// 내 문서에 kalendar폴더가 없으면 kalendar폴더를 만든 후에 
	// kolasiii폴더에 있는 메모파일을 복사시킨다.
	
	CStringArray aFileName;

	CFileFind FindFile;
	BOOL bResult = FindFile.FindFile(strFile+_T("*.*"));	
	if(FALSE == bResult)
	{
 		bResult = CreateDirectory(strFile+_T("\\"),NULL);
 		if(TRUE == bResult)
		{	
			BOOL bIsFile = FindFile.FindFile(_T("..\\cfg\\kalendar\\*.*"));
			while(TRUE == bIsFile)
			{
				bIsFile = FindFile.FindNextFile();
				if(TRUE == FindFile.IsDots())
				{
					continue;
				}
				CString strFileName = FindFile.GetFileName();
				aFileName.Add(strFileName);
			}							
			
			int nCnt = 0;
			const int SIZE = aFileName.GetSize();
			for(nCnt = 0; nCnt < SIZE; nCnt++)
			{
				CString strSrc = _T("..\\cfg\\kalendar\\") + aFileName.GetAt(nCnt);
				CString strDst = strFile + aFileName.GetAt(nCnt);

				CopyFile(strSrc, strDst, FALSE);
			}
		}
	}

	return 0;
}
