// RESEARCH_Mgr.cpp: implementation of the CRESEARCH_Mgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RESEARCH_Mgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRESEARCH_Mgr::CRESEARCH_Mgr(CESL_Mgr* pParent /*= NULL*/)
: CESL_Mgr(IDD , pParent)
{
	m_pMainDM          = NULL ;
	m_pResearchInfo_DM = NULL ;
	m_pQuestionInfo_DM = NULL ;
	m_pSampleInfo_DM   = NULL ;
}

CRESEARCH_Mgr::~CRESEARCH_Mgr()
{
}

INT CRESEARCH_Mgr::SetDefaultDM(CString strMain,CString strResearch, CString strQuestion, CString strSample)
{
	if(strMain.IsEmpty () ||strResearch.IsEmpty () || strQuestion.IsEmpty () || strSample.IsEmpty () )
	{
		AfxMessageBox (_T("DM ALIAS 를 입력 하십시요.")) ;
		return -1 ;
	}

	m_pMainDM = FindDM(strMain);
	if(m_pMainDM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed")) ;
		return -100 ;
	}

	m_pResearchInfo_DM = FindDM(strResearch);
	if(m_pResearchInfo_DM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed")) ;
		return -200 ;
	}
	m_pQuestionInfo_DM = FindDM(strQuestion);
	if(m_pResearchInfo_DM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed")) ;
		return -300 ;
	}
	m_pSampleInfo_DM   = FindDM(strSample);
	if(m_pResearchInfo_DM == NULL)
	{
		AfxMessageBox (_T("FindDM() Failed")) ;
		return -400 ;
	}


	return 0 ;
}
