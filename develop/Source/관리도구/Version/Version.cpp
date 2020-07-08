// Version.cpp: implementation of the CVersion class.
//
//////////////////////////////////////////////////////////////////////

#include "Version.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>

#define VERSION_MAX_SIZE 512
#define ARGV_VERSION "-VERSION"

#pragma comment(lib,"KOLASIIIVer.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllimport) bool GetKOLASIIIVer(wchar_t*);

int main(int argc, char* argv[])
{
	CVersion version;

	char szVersion[VERSION_MAX_SIZE] = {0,};

	for(int i=1; i<argc; i++)
	{
		int j=0;
		char c;
		while(argv[i][j])
		{
			c = argv[i][j];
			argv[i][j] = toupper(c);
			j++;
		}

		if(strcmp((const char*)argv[i], ARGV_VERSION) == 0)
		{
			if(version.GetKOLASVersion(szVersion))
			{
				printf("%s",szVersion);
			}
			else
			{
				printf("ERROR\n");
			}
		}
	}	 		
	return 0;
}

CVersion::CVersion()
{

}

bool CVersion::GetKOLASVersion(char* _pszVesion)
{
	char* pszVersionRtn = NULL;
	wchar_t *pwsVersion = new wchar_t[VERSION_MAX_SIZE];
	memset(pwsVersion,0x00,sizeof(wchar_t)*VERSION_MAX_SIZE);
	if(!GetKOLASIIIVer(pwsVersion))
		return false;

	// KOL.UDF.022 시큐어코딩 보완
    if(&pwsVersion == NULL)
  		return false;

	int nSize = wcstombs(NULL, pwsVersion, 0);

	memset(_pszVesion, 0x00, VERSION_MAX_SIZE);

	if(nSize < 0 || nSize > VERSION_MAX_SIZE) 
		return false;
	
	wcstombs(_pszVesion, pwsVersion, nSize);

	// KOL.UDF.022 시큐어코딩 보완
 	if(&pwsVersion != NULL)
  		delete [] pwsVersion;

	return true;
}

CVersion::~CVersion()
{

}
