#include "stdafx.h"
#include "GipyoPrintAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CALLBACK __stdcall
#define WINAPI __stdcall

typedef INT (__stdcall *fp_SetConfig)(INT);
typedef INT (__stdcall *fp_PutData)(char*);
typedef INT (__stdcall *fp_PutEmsData)(char*);
typedef INT (__stdcall *fp_ExecPrint)(VOID);
typedef INT (__stdcall *fp_ExecPrintEms)(VOID);


CGipyoPrintAPI::CGipyoPrintAPI()
{
	m_hDll = NULL;

	int i = 0;
	for(i = 0;i < 2048; i++){
		m_postData[i] = NULL;
	}
}

CGipyoPrintAPI::~CGipyoPrintAPI()
{
	if(NULL != m_hDll) 
	{ 
		delete m_hDll;
		m_hDll = NULL;
	}
}

INT CGipyoPrintAPI::LoadPrintLibrary()
{
	m_hDll = LoadLibrary(_T("xpostgipyo.dll"));

	INT nErrorCode = GetLastError();
	if(126 == nErrorCode)
	{
		AfxMessageBox(_T("xpostgipyo.dll 파일을 찾을 수 없습니다.(126)"));
		return -126;
	}

	if(m_hDll==NULL)
	{
		AfxMessageBox(_T("xpostgipyo.dll 파일을 찾을 수 없습니다.(100)"));
		return -100;
	}
	return 0;
}

VOID CGipyoPrintAPI::FreePrintLibrary()
{
	FreeLibrary(m_hDll); 
}

INT CGipyoPrintAPI::PutData(char *pChrData)
{
	fp_PutData fpPutData = (fp_PutData)GetProcAddress(m_hDll,"GetPutdata");

	if(fpPutData==NULL)
	{
		AfxMessageBox(_T("GetProcAddress Error!"));
		return -100;
	}
	else
	{ 
		int nIds = fpPutData(pChrData);

		if(1 != nIds)
		{
			AfxMessageBox(_T("라이브러리에서 GetPutdata처리가 실패하였습니다."));
			return -101;		
		}
	}	 

	return 0;
}

INT CGipyoPrintAPI::PutEmsData(char *pChrData)
{
	fp_PutEmsData fpPutEmsData = (fp_PutEmsData)GetProcAddress(m_hDll,"GetPutems");

	if(fpPutEmsData==NULL)
	{
		AfxMessageBox(_T("GetProcAddress Error!"));
		return -100;
	}
	else
	{ 
		int nIds = fpPutEmsData(pChrData);

		if(1 != nIds)
		{
			AfxMessageBox(_T("라이브러리에서 GetPutdata처리가 실패하였습니다."));
			return -101;		
		}
	}	 

	return 0;
}

INT CGipyoPrintAPI::About()
{
	INT (*fp_About)(VOID) = (INT (*)(VOID))GetProcAddress(m_hDll, "about");

	if(fp_About==NULL)
	{
		AfxMessageBox(_T("GetProcAddress Error!"));
		return -100;
	}
	else
	{ 
		if(1 != fp_About())
		{
			AfxMessageBox(_T("라이브러리에서 About처리가 실패하였습니다."));
			return -101;		
		}
	}	 

	return 0;
}

INT CGipyoPrintAPI::ExecPrint()
{

	fp_ExecPrint fpExecPrint = (fp_ExecPrint)GetProcAddress(m_hDll,"ExecPrint");

	if(NULL == fpExecPrint)
	{
		AfxMessageBox(_T("GetProcAddress Error!"));
		return -100;
	}
	else
	{ 
		if(1 != fpExecPrint())
		{
			AfxMessageBox(_T("라이브러리에서 ExecPrint처리가 실패하였습니다."));
			return -101;		
		}
	}	 
	return 0;
}

INT CGipyoPrintAPI::ExecPrintEms()
{
	fp_ExecPrintEms fpExecPrintEms = (fp_ExecPrintEms)GetProcAddress(m_hDll,"ExecPrintEms");

	if(NULL == fpExecPrintEms)
	{
		AfxMessageBox(_T("GetProcAddress Error!"));
		return -100;
	}
	else
	{ 
		if(1 != fpExecPrintEms())
		{
			AfxMessageBox(_T("라이브러리에서 ExecPrintEms처리가 실패하였습니다."));
			return -101;		
		}
	}	 

	return 0;
}


INT CGipyoPrintAPI::SetConfig(INT nType)
{
	fp_SetConfig fpSetConfig = (fp_SetConfig)GetProcAddress(m_hDll,"SetConfig");

	if(NULL == fpSetConfig)
	{
		AfxMessageBox(_T("GetProcAddress Error!"));
		return -100;
	}
	else
	{ 
		if(1 != fpSetConfig(nType))
		{
			AfxMessageBox(_T("라이브러리에서 SetConfig처리가 실패하였습니다."));
			return -101;		
		}
		else
		{
			AfxMessageBox(_T("성공"));
			return -101;		
		}
	}	 
	return 0;
}

CString CGipyoPrintAPI::MakeDataFormat(CString straPutData[57])
{
	CString strResult;
	CString strTemp;
	char chrSeperateCode = char(30);

	for(INT i=0; i<57; i++)
	{
		strTemp = straPutData[i];
		if(56 != i)
		{
			strResult = strResult + strTemp + chrSeperateCode;	
		}
		else
		{
			strResult = strResult + strTemp;	
		}
	}
	AfxMessageBox(strResult);

	return strResult;
}

CString CGipyoPrintAPI::MakeDataFormat2(CString straPutData[60])
{
	CString strResult;
	CString strTemp;
	char chrSeperateCode = char(30);

	for(INT i=0; i<60; i++)
	{
		strTemp = straPutData[i];
		if(59 != i)
		{
			strResult = strResult + strTemp + chrSeperateCode;	
		}
		else
		{
			strResult = strResult + strTemp;	
		}
	}
	AfxMessageBox(strResult);

	return strResult;
}


INT CGipyoPrintAPI::ConvertUnicodeToMBCS(CString strPutData)
{
	WCHAR wszBuffer[2048];

	wsprintf(wszBuffer, _T("%s"), strPutData);

	::ZeroMemory(m_postData, sizeof(m_postData));

	::WideCharToMultiByte(CP_ACP, 0, wszBuffer, -1, m_postData, 2048, NULL, NULL);

	return 0;
}

INT CGipyoPrintAPI::ConvertMBCStoUnicode()
{

	return 0;
}
