// DupKeyMakerByRequestInfo.cpp: implementation of the CDupKeyMakerByRequestInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DupKeyMakerByRequestInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDupKeyMakerByRequestInfo::CDupKeyMakerByRequestInfo(CESL_Mgr * pMgr) : CDupKeyMaker(pMgr)
{
	m_pSourceDM = NULL;
	m_pSourceGrid = NULL;
}

CDupKeyMakerByRequestInfo::~CDupKeyMakerByRequestInfo()
{

}

INT CDupKeyMakerByRequestInfo::MakeKeyDM()
{
	if(m_pKeyDM == NULL || m_pSourceGrid == NULL || m_pSourceDM == NULL) return -1;

	m_pKeyDM->FreeData();
	
	CString strTitle, strAuthor, strPublisher;
	INT ids = m_pSourceGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT i = m_pSourceGrid->SelectGetHeadPosition();
	INT nRow = -1;	
	// 17/05/10 김혜영 : 복본검색 개선 ('서명검색', 'ISBN검색'으로 선택)
//*/ ADD ---------------------------------------------------------------------------
	CString strISBN = _T("");
//*/ END ---------------------------------------------------------------------------

	while(i >= 0)
	{
		strTitle = m_pSourceDM->GetCellData(_T("본표제"), i);
		strAuthor = m_pSourceDM->GetCellData(_T("저작자"), i);
		strPublisher = m_pSourceDM->GetCellData(_T("발행자"), i);
		// 17/05/10 김혜영 : 복본검색 개선 ('서명검색', 'ISBN검색'으로 선택)
/*//BEFORE----------------------------------------------------------------------------------------
		if(strTitle.IsEmpty())
*///AFTER-----------------------------------------------------------------------------------------
		strISBN = m_pSourceDM->GetCellData(_T("ISBN"), i); 
		if(strTitle.IsEmpty() && strISBN.IsEmpty())
/*//END-------------------------------------------------------------------------------------------*/		
		{
			if(strAuthor.IsEmpty() && strPublisher.IsEmpty())
			{
				i = m_pSourceGrid->SelectGetNext();
				continue;
			}
		}
		m_pKeyDM->InsertRow(-1);
		nRow = m_pKeyDM->GetRowCount()-1;
		m_pKeyDM->SetCellData(_T("IBS_본표제"), strTitle, nRow);
		// 17/05/10 김혜영 : 복본검색 개선 ('서명검색', 'ISBN검색'으로 선택)
//*/ ADD ---------------------------------------------------------------------------
		m_pKeyDM->SetCellData(_T("BB_낱권_ISBN"), strISBN, nRow);
//*/ END ---------------------------------------------------------------------------
		//=====================================================
		//2009.01.05 ADD PJW : 저자역할어 제외 
		strAuthor.TrimLeft();	strAuthor.TrimRight();
		RemoveRoleWord(strAuthor);
		//=====================================================

		m_pKeyDM->SetCellData(_T("IBS_저작자"), strAuthor, nRow);
		m_pKeyDM->SetCellData(_T("IBS_발행자"), strPublisher, nRow);
		m_pKeyDM->SetCellData(_T("IBS_발행년"), m_pSourceDM->GetCellData(_T("발행년"), i), nRow);
		m_pKeyDM->SetCellData(_T("IBS_종KEY"), m_pSourceDM->GetCellData(_T("비치희망KEY"), i), nRow);

		i = m_pSourceGrid->SelectGetNext();
	}
	
	return 0;
}
