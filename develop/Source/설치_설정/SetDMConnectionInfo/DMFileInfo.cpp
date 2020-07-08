// DMFileInfo.cpp: implementation of the CDMFileInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SetDMConnectionInfo.h"
#include "DMFileInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDMFileInfo::CDMFileInfo()
{

}

CDMFileInfo::~CDMFileInfo()
{

}

INT CDMFileInfo::LoadFile(CString strFileName)
{
	nLINE = 0;
	CStdioFile fd;
	if (fd.Open(strFileName, CStdioFile::modeRead | CFile::typeBinary) == FALSE) return -1;
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	CString strLine;
	while(TRUE) {
		if (fd.ReadString(strLine) == FALSE) break;
		
		LINE[nLINE] = strLine;
		nLINE++;
	}

	fd.Close();
	return 0;
}

INT CDMFileInfo::SaveFile(CString strFileName)
{
	FILE *fp = NULL;
	fp = _tfopen(strFileName, _T("wb"));
	if (fp == NULL) return -1;

		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE 수정작업
		// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
		fseek(fp, 0, SEEK_END);		
		if( 0 == ftell(fp) )
		{
			/*UNCHANGE*/fputc(0xFF, fp);
			/*UNCHANGE*/fputc(0xFE, fp);
		}
		// ------------------------------------------------------------------------------
		
	for (INT i = 0; i < nLINE; i++) {
		_ftprintf(fp, _T("%s\r\n"), LINE[i]);		
	}

	fclose(fp);

	return 0;
}

INT CDMFileInfo::SetCONNECTION_INFO(CString strCONNECTION_INFO)
{
	// CONNECTION_INFO=kolas2@kolas2/kolas2#[192.168.1.230]

	INT nPos;
	CString strTag, strData;
	for (INT i = 0; i < nLINE; i++) {
		nPos = LINE[i].Find(_T("="), 0);
		if (nPos == -1) continue;

		strTag = LINE[i].Mid(0, nPos);
		strData = LINE[i].Mid(nPos+1);

		strTag.TrimLeft();				strTag.TrimRight();
		strTag.MakeUpper();
		if (strTag == _T("CONNECTION_INFO")) {
			LINE[i].Format(_T("CONNECTION_INFO=%s"), strCONNECTION_INFO);
			break;
		}
	}

	return 0;
}

INT CDMFileInfo::GetCONNECTION_INFO(CString *pstrCONNECTION_INFO)
{
	// CONNECTION_INFO=kolas2@kolas2/kolas2#[192.168.1.230]

	INT nPos;
	CString strTag, strData;
	for (INT i = 0; i < nLINE; i++) {
		nPos = LINE[i].Find(_T("="), 0);
		if (nPos == -1) continue;

		strTag = LINE[i].Mid(0, nPos);
		strData = LINE[i].Mid(nPos+1);

		strTag.TrimLeft();				strTag.TrimRight();
		strTag.MakeUpper();
		if (strTag == _T("CONNECTION_INFO")) {
			pstrCONNECTION_INFO->Format(_T("%s"), strData);
			break;
		}
	}

	return 0;
}
