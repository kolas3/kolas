// DupKeyMakerByTitle.cpp: implementation of the CDupKeyMakerByTitle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DupKeyMakerByTitle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDupKeyMakerByTitle::CDupKeyMakerByTitle(CESL_Mgr * pMgr) : CDupKeyMaker(pMgr)
{

}

CDupKeyMakerByTitle::~CDupKeyMakerByTitle()
{

}

INT CDupKeyMakerByTitle::MakeKeyDM()
{
	if(m_pKeyDM == NULL || m_strTitle.IsEmpty()) return -1;
	
	m_pKeyDM->FreeData();
	m_pKeyDM->InsertRow(-1);
	m_pKeyDM->SetCellData(_T("IBS_º»Ç¥Á¦"), m_strTitle, 0);

	return 0;
}
