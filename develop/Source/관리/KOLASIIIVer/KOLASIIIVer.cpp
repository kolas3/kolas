// KOLASIIIVer.cpp: implementation of the CKOLASIIIVer class.
//
//////////////////////////////////////////////////////////////////////

#include "KOLASIIIVer.h"
#include "afxwin.h"
#include <cwchar>

#pragma comment(lib,"version")

#define VERSION_MAX_SIZE 512

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport)

bool GetKOLASIIIVer(wchar_t* _pswVersionArg)
{
	if(!_pswVersionArg)
		return false;

	wchar_t wsPath[VERSION_MAX_SIZE] = {0,};
	wchar_t* pwsVersion = NULL;

	int nVersionSize = 0;
	GetModuleFileName(AfxGetInstanceHandle(), wsPath, VERSION_MAX_SIZE);

	nVersionSize = GetFileVersionInfoSize(wsPath, 0);
	if(nVersionSize <= 0)
		return false;

	pwsVersion = new wchar_t[nVersionSize];	

	WORD majorVer, minorVer, buildNum, revisionNum;

	if(GetFileVersionInfo(wsPath, 0, VERSION_MAX_SIZE, pwsVersion)!=0)
	{
		VS_FIXEDFILEINFO* pFileInfo = NULL;
		UINT bBuffLen = 0;

		if(VerQueryValue(pwsVersion, _T("\\"), (LPVOID*)&pFileInfo, &bBuffLen)!=0)
		{			
            majorVer = HIWORD(pFileInfo->dwFileVersionMS);
            minorVer = LOWORD(pFileInfo->dwFileVersionMS);
            buildNum = HIWORD(pFileInfo->dwFileVersionLS);
            revisionNum = LOWORD(pFileInfo->dwFileVersionLS);
		}
	}

	if(pwsVersion)
		delete []pwsVersion;

	CString strRtn = _T("");
	strRtn.Format(_T("%d%02d%02d%03d"),majorVer,minorVer,buildNum,revisionNum);

	memcpy(_pswVersionArg, strRtn.GetBuffer(0), sizeof(wchar_t)*strRtn.GetLength());

	return true;
}

extern "C" __declspec(dllexport)

bool GetKOLASIIIBuildDate(wchar_t* _pswDateArg)
{
	if(!_pswDateArg)
		return false;

	wchar_t wsPath[VERSION_MAX_SIZE] = {0,};
	wchar_t* pwsVersion = NULL;

	int nVersionSize = 0;
	GetModuleFileName(AfxGetInstanceHandle(), wsPath, VERSION_MAX_SIZE);

	nVersionSize = GetFileVersionInfoSize(wsPath, 0);
	if(nVersionSize <= 0)
		return false;

	pwsVersion = new wchar_t[nVersionSize];	

	WORD majorVer, minorVer, buildNum, revisionNum;

	if(GetFileVersionInfo(wsPath, 0, VERSION_MAX_SIZE, pwsVersion)!=0)
	{
		VS_FIXEDFILEINFO* pFileInfo = NULL;
		UINT bBuffLen = 0;

		if(VerQueryValue(pwsVersion, _T("\\"), (LPVOID*)&pFileInfo, &bBuffLen)!=0)
		{			
            majorVer = HIWORD(pFileInfo->dwFileVersionMS);
            minorVer = LOWORD(pFileInfo->dwFileVersionMS);
            buildNum = HIWORD(pFileInfo->dwFileVersionLS);
            revisionNum = LOWORD(pFileInfo->dwFileVersionLS);
		}
	}

	if(pwsVersion)
		delete []pwsVersion;

	CString strRtn = _T("");
	strRtn.Format(_T("%02d.%02d.%02d"),int(majorVer%100),minorVer,buildNum,revisionNum);

	memcpy(_pswDateArg, strRtn.GetBuffer(0), sizeof(wchar_t)*strRtn.GetLength());
	
	return true;
}

CKOLASIIIVer::CKOLASIIIVer()
{

}

CKOLASIIIVer::~CKOLASIIIVer()
{

}
