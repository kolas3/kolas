// DupKeyMakerByDM.cpp: implementation of the CDupKeyMakerByDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DupKeyMakerByDM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDupKeyMakerByDM::CDupKeyMakerByDM(CESL_Mgr * pMgr) : CDupKeyMaker(pMgr)
{
	m_pSourceDM = NULL;
	m_pSourceGrid = NULL;
}

CDupKeyMakerByDM::~CDupKeyMakerByDM()
{

}

INT CDupKeyMakerByDM::MakeKeyDM()
{
	if(m_pKeyDM == NULL || m_pSourceDM == NULL || m_pSourceGrid == NULL) return -1;
	
	m_pKeyDM->FreeData();
	
	INT cnt = -1;
	INT nPos = -1;
	INT nCurIdx;
	CMarc marc;
	CString strMarc, title, author, publisher, pubyear, isbn, strTmpData, regNo;
	CArray<CString, CString&> arrIsbn;
	
	INT ids = m_pSourceGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT i = m_pSourceGrid->SelectGetHeadPosition();
	if(i < 0) return -1;

	while(i >= 0)
	{
		m_pSourceDM->GetCellData(_T("BS_MARC"), i, strMarc);
		if(strMarc.GetLength() == 0)
		{
			i = m_pSourceGrid->SelectGetNext();
			continue;
		}
		ids = m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		if(ids < 0) 
		{
			i = m_pSourceGrid->SelectGetNext();
			continue;
		}

		nCurIdx = ExtractKeyByMarc(&marc);
		if(nCurIdx < 0)
		{
			i = m_pSourceGrid->SelectGetNext();
			continue;
		}
		m_pKeyDM->SetCellData(_T("IBS_종KEY"), m_pSourceDM->GetCellData(_T("IBS_종KEY"), i), nCurIdx);
		m_pKeyDM->SetCellData(_T("BB_매체구분"), m_pSourceDM->GetCellData(_T("IBS_매체구분"), i), nCurIdx);
		m_pKeyDM->SetCellData(_T("BB_별치기호"), m_pSourceDM->GetCellData(_T("IBS_색인별치기호"), i), nCurIdx);
		m_pKeyDM->SetCellData(_T("BB_등록구분"), m_pSourceDM->GetCellData(_T("IBS_색인등록구분"), i), nCurIdx);
		m_pKeyDM->SetCellData(_T("BB_편권차"), m_pSourceDM->GetCellData(_T("IBS_색인편권차"), i), nCurIdx);
		m_pKeyDM->SetCellData(_T("BB_권서명"), m_pSourceDM->GetCellData(_T("IBS_색인권서명"), i), nCurIdx);
		//===================================================
		//2010.11.29 ADD BY PJW : 조건으로 총서명을 추가한다.
//		m_pKeyDM->SetCellData(_T("UDF_총서명"), m_pSourceDM->GetCellData(_T("UDF_총서명"), i), nCurIdx);
		//===================================================
		m_pKeyDM->SetCellData(_T("BS_MARC"), m_pSourceDM->GetCellData(_T("BS_MARC"), i), nCurIdx);
		// 17/05/10 김혜영 : 복본검색 개선 ('서명검색', 'ISBN검색'으로 선택)
//*/ ADD ---------------------------------------------------------------------------
		//m_pKeyDM->SetCellData(_T("IBS_ISBN"), m_pSourceDM->GetCellData(_T("BB_낱권_ISBN"), i), nCurIdx);//IBS_ISBN
//*/ END ---------------------------------------------------------------------------
		//JOB.2018.0018 EDIT BY KYJ 반입자료관리 ISBN으로 복본조사가 되지 않는 현상 수정 
		//m_pKeyDM->SetCellData(_T("BB_낱권_ISBN"), m_pSourceDM->GetCellData(_T("BB_낱권_ISBN"), i), nCurIdx);
		//JOB.2019.0036 EDIT BY KYJ
		m_pKeyDM->SetCellData(_T("BB_낱권_ISBN"), m_pSourceDM->GetCellData(_T("BB_낱권ISBN"), i), nCurIdx);
		i = m_pSourceGrid->SelectGetNext();
	}
	return 0;
}
