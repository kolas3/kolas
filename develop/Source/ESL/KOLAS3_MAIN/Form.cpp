// Form.cpp: implementation of the CForm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "Form.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CForm::CForm()
{
	m_hInstLib   = NULL;
	m_pCreateFun = NULL;
    m_pDeleteFun = NULL;

	m_pCreateWnd = NULL;
}

CForm::~CForm()
{
	if (m_hInstLib != NULL) 
	{
        if (m_pCreateFun != NULL && m_pCreateWnd != NULL) 
			(m_pDeleteFun)(m_pCreateWnd);
        
		AfxFreeLibrary(m_hInstLib);
		m_hInstLib = NULL;
    }
}

BOOL CForm::CreateForm(CWnd *pParent, TCHAR *PathDLL, CESL_Information* pInfo, TCHAR VIEW_STYLE, CString strMenuAlias)
{
	CString strErrMsg;
    m_hInstLib = AfxLoadLibrary(PathDLL);
    if (m_hInstLib == NULL)
	{
		FILE *fp = _tfopen(_T("..\\cfg\\_DEBUG"), _T("rb"));
		if (fp == NULL)
			strErrMsg.Format(_T("DLL을 적재하지 못하였습니다.[%s]"), PathDLL);
		else
		{
			strErrMsg.Format(_T("DLL을 적재하지 못하였습니다.[%s] \r\n")
							 _T("1. Dependency Walker로 체크해본다. \r\n")
							 _T("2. 관련된 DLL을 리빌드해본다.")
							 , PathDLL);
			fclose(fp);
		}
		AfxMessageBox(strErrMsg);
        return FALSE;
    } 

    m_pCreateFun = (FormStartMethod)GetProcAddress(m_hInstLib, "CreateForm");
    if (m_pCreateFun == NULL) 
	{
        AfxFreeLibrary(m_hInstLib);
        m_hInstLib = NULL;
		strErrMsg.Format(_T("GetProcAddress Error : CreateForm.[%s]"), PathDLL);
		AfxMessageBox(strErrMsg);
        return FALSE;
    }

	m_pCreateModelessDlgFun = (FormCreateModelessDlgMethod)GetProcAddress(m_hInstLib, "CreateModelessDlg");
	if (m_pCreateFun == NULL) 
	{
        AfxFreeLibrary(m_hInstLib);
        m_hInstLib = NULL;
		strErrMsg.Format(_T("GetProcAddress Error : CreateModelessDlg.[%s]"), PathDLL);
		AfxMessageBox(strErrMsg);
        return FALSE;
    }

	m_pResizeFun = (FormResizeMethod)GetProcAddress(m_hInstLib, "ResizeForm");
	if (m_pResizeFun == NULL)
	{
		AfxFreeLibrary(m_hInstLib);
        m_hInstLib = NULL;
		strErrMsg.Format(_T("GetProcAddress Error : ResizeForm.[%s]"), PathDLL);
		AfxMessageBox(strErrMsg);
        return FALSE;
	}

    m_pDeleteFun = (FormEndMethod)GetProcAddress(m_hInstLib, "DeleteForm");
	if (m_pDeleteFun == NULL) 
	{
        AfxFreeLibrary(m_hInstLib);
        m_hInstLib = NULL;
		strErrMsg.Format(_T("GetProcAddress Error : DeleteForm.[%s]"), PathDLL);
		AfxMessageBox(strErrMsg);
        return FALSE;
	}
	
	m_pCreateWnd = (m_pCreateFun)(pParent, pInfo, VIEW_STYLE, strMenuAlias);
	if (m_pCreateWnd == NULL) return FALSE;

	m_pKalendarSearchFun = (KalendarSearchMethod)GetProcAddress(m_hInstLib, "KalendarSearch");	

	return TRUE;
}

CESL_Mgr *CForm::CreateModelessDlg(CString strModelessSM, CString strButtonAlias, BOOL bReadOnly)
{
	if (m_pCreateFun && m_pCreateWnd)
		return (m_pCreateModelessDlgFun)(strModelessSM, strButtonAlias, bReadOnly, m_pCreateWnd);
	else 
		return NULL;
}

VOID CForm::ResizeForm(CRect rect)
{
	if (m_pCreateWnd == NULL) return;
	(m_pResizeFun)(rect, m_pCreateWnd);
}

void CForm::Show(BOOL bShow)
{
	m_pCreateWnd->ShowWindow(bShow);
}

VOID CForm::KalendarSearch(CString WorkType, CString strAcqCode, CString strAcqYear, CString strWorkCode, CString strSeqNo)
{	
	if ( m_pKalendarSearchFun && m_pCreateWnd)
	{
		(m_pKalendarSearchFun)(WorkType, strAcqCode, strAcqYear, strWorkCode, strSeqNo, m_pCreateWnd);
	}
}