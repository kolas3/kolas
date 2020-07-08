// DupKeyMakeMgr.cpp: implementation of the CDupKeyMakeMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DupKeyMakeMgr.h"
#include "DupKeyMakerByGrid.h"
#include "DupKeyMakerByDM.h"
#include "DupKeyMakerByTitle.h"
#include "DupKeyMakerByRequestInfo.h"
#include "DupKeyMakerByIndividualInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDupKeyMakeMgr::CDupKeyMakeMgr(CESL_Mgr * pMgr, CDuplicateSearchMgr * pDupMgr) : CSPF_Work(pMgr)
{
	m_pDupMgr = pDupMgr;
}

CDupKeyMakeMgr::~CDupKeyMakeMgr()
{

}

INT CDupKeyMakeMgr::SPFExecutePath(INT nPath)
{
	if(nPath < 0) 
		return -1;
	else if(nPath == CAT_DUPLICATE_SEARCH )
		return SetKeyDMByGrid();
	else if(nPath == WORK_DUPLICATE_SEARCH ||
			nPath == HOLDING_DUPLICATE_SEARCH || 
			nPath == HOLDING_DUPLICATE_SEARCH_BASIC_DATA || 
			nPath == HOLDING_DUPLICATE_SEARCH_IMPORT_DATA)
		return SetKeyDMByDM();
	else if(nPath == HOLDING_DUPLICATE_SEARCH_REQUEST_DATA)
		return SetKeyDMByRequestInfo();
	else if(nPath == CAT_TITLE_DUPLICATE_SEARCH || 
			nPath == TITLE_DUPLICATE_SEARCH || 
			nPath == TITLE_AUTO_DUPLICATE_SEARCH || 
			nPath == FURNISH_DATA_DUPLICATE_SEARCH || 
			nPath == SERIAL_TITLE_DUPLICATE_SEARCH)
		return SetKeyDMByTitle();
	else if(nPath == HOLDING_DUPLICATE_SEARCH_INDIVIDUAL_DATA)
		return SetKeyDMByIndividualInfo();
	else if(nPath == WORK_DUPLICATE_SEARCH_DIVNO || 
			nPath == HOLDING_DUPLICATE_SEARCH_DIVNO)
		return SetKeyDMByDM();
	
	return -1;
}

INT CDupKeyMakeMgr::SetKeyDMByGrid()
{	
	CString strKeyDMAlias, strMainCMAlias, strMainDMAlias, strMainGridAlias;
	m_pDupMgr->GetTempData(_T("KeyDMAlias"), strKeyDMAlias);
	m_pDupMgr->GetTempData(_T("CMAlias"), strMainCMAlias);
	m_pDupMgr->GetTempData(_T("SourceDMAlias"), strMainDMAlias);
	m_pDupMgr->GetTempData(_T("GridAlias"), strMainGridAlias);

	if(strKeyDMAlias.IsEmpty() || strMainGridAlias.IsEmpty() || strMainCMAlias.IsEmpty() || strMainDMAlias.IsEmpty())
		return -1;

	CESL_DataMgr * pKeyDM = m_pParentMgr->FindDM(strKeyDMAlias);
	CESL_DataMgr * pMainDM = m_pParentMgr->FindDM(strMainDMAlias);
	CESL_Grid * pMainGrid = (CESL_Grid*)m_pParentMgr->FindControl(strMainCMAlias, strMainGridAlias);

	if(pKeyDM == NULL || pMainDM == NULL || pMainGrid == NULL) 
		return -1;

	CDupKeyMakerByGrid KeyDMMaker(m_pParentMgr);
	KeyDMMaker.SetKeyDM(pKeyDM);
	KeyDMMaker.SetSourceDM(pMainDM);
	KeyDMMaker.SetSourceGrid(pMainGrid);

	return KeyDMMaker.MakeKeyDM();
}

INT CDupKeyMakeMgr::SetKeyDMByDM()
{	
	CString strKeyDMAlias, strMainCMAlias, strMainDMAlias, strMainGridAlias;
	m_pDupMgr->GetTempData(_T("KeyDMAlias"), strKeyDMAlias);
	m_pDupMgr->GetTempData(_T("CMAlias"), strMainCMAlias);
	m_pDupMgr->GetTempData(_T("SourceDMAlias"), strMainDMAlias);
	m_pDupMgr->GetTempData(_T("GridAlias"), strMainGridAlias);
	
	if(strKeyDMAlias.IsEmpty() || strMainGridAlias.IsEmpty() || strMainCMAlias.IsEmpty() || strMainDMAlias.IsEmpty())
		return -1;
	
	CESL_DataMgr * pKeyDM = m_pParentMgr->FindDM(strKeyDMAlias);
	CESL_DataMgr * pMainDM = m_pParentMgr->FindDM(strMainDMAlias);
	CESL_Grid * pMainGrid = (CESL_Grid*)m_pParentMgr->FindControl(strMainCMAlias, strMainGridAlias);
	
	if(pKeyDM == NULL || pMainDM == NULL || pMainGrid == NULL) 
		return -1;
	
	CDupKeyMakerByDM KeyDMMaker(m_pParentMgr);
	KeyDMMaker.SetKeyDM(pKeyDM);
	KeyDMMaker.SetSourceDM(pMainDM);
	KeyDMMaker.SetSourceGrid(pMainGrid);
	
	return KeyDMMaker.MakeKeyDM();
}

INT CDupKeyMakeMgr::SetKeyDMByTitle()
{
	CString strKeyDMAlias, strTitle;
	m_pDupMgr->GetTempData(_T("KeyDMAlias"), strKeyDMAlias);
	m_pDupMgr->GetTempData(_T("Title"), strTitle);

	if(strKeyDMAlias.IsEmpty() || strTitle.IsEmpty())
		return -1;

	CESL_DataMgr * pKeyDM = m_pParentMgr->FindDM(strKeyDMAlias);
	if(pKeyDM == NULL) return -1;

	CDupKeyMakerByTitle KeyDMMaker(m_pParentMgr);
	KeyDMMaker.SetKeyDM(pKeyDM);
	KeyDMMaker.SetTitle(strTitle);
	
	return KeyDMMaker.MakeKeyDM();
}

INT CDupKeyMakeMgr::SetKeyDMByIndividualInfo()
{
	CString strKeyDMAlias, strMarc, strMediaCodeInfo, strVolInfo, strVolTitleInfo, strShelfCodeInfo, strRegCodeInfo;
	m_pDupMgr->GetTempData(_T("KeyDMAlias"), strKeyDMAlias);
	m_pDupMgr->GetTempData(_T("Marc"), strMarc);
	m_pDupMgr->GetTempData(_T("MediaCodeInfo"), strMediaCodeInfo);
	m_pDupMgr->GetTempData(_T("VolInfo"), strVolInfo);
	m_pDupMgr->GetTempData(_T("VolTitleInfo"), strVolTitleInfo);
	m_pDupMgr->GetTempData(_T("ShelfCodeInfo"), strShelfCodeInfo);
	m_pDupMgr->GetTempData(_T("RegCodeInfo"), strRegCodeInfo);

	if(strKeyDMAlias.IsEmpty() || strMarc.IsEmpty()) return -1;

	CESL_DataMgr * pKeyDM = m_pParentMgr->FindDM(strKeyDMAlias);
	if(pKeyDM == NULL) return -1;

	INT ids;
	CDupKeyMakerByIndividualInfo KeyDMMaker(m_pParentMgr);
	KeyDMMaker.SetKeyDM(pKeyDM);
	ids = KeyDMMaker.SetMarc(strMarc);
	if(ids < 0) return ids;
	KeyDMMaker.SetMediaCode(strMediaCodeInfo);
	KeyDMMaker.SetRegCode(strRegCodeInfo);
	KeyDMMaker.SetShelfCode(strShelfCodeInfo);
	KeyDMMaker.SetVolInfo(strVolInfo);
	KeyDMMaker.SetVolTitleInfo(strVolTitleInfo);
	
	return KeyDMMaker.MakeKeyDM();
}

INT CDupKeyMakeMgr::SetKeyDMByDivNoInfo()
{
	CString strKeyDMAlias;
	m_pDupMgr->GetTempData(_T("KeyDMAlias"), strKeyDMAlias);
	CGroupInfo * pGroup = (CGroupInfo*)m_pDupMgr->GetTempPtr(_T("GroupInfo"));

	if(strKeyDMAlias.IsEmpty() || pGroup == NULL) return -1;
	
	CESL_DataMgr * pKeyDM = m_pParentMgr->FindDM(strKeyDMAlias);
	if(pKeyDM == NULL) return -1;
	
	return 0;
}

INT CDupKeyMakeMgr::SetKeyDMByRequestInfo()
{
	CString strKeyDMAlias, strMainCMAlias, strMainDMAlias, strMainGridAlias;
	m_pDupMgr->GetTempData(_T("KeyDMAlias"), strKeyDMAlias);
	m_pDupMgr->GetTempData(_T("CMAlias"), strMainCMAlias);
	m_pDupMgr->GetTempData(_T("SourceDMAlias"), strMainDMAlias);
	m_pDupMgr->GetTempData(_T("GridAlias"), strMainGridAlias);
	
	if(strKeyDMAlias.IsEmpty() || strMainGridAlias.IsEmpty() || strMainCMAlias.IsEmpty() || strMainDMAlias.IsEmpty())
		return -1;
	
	CESL_DataMgr * pKeyDM = m_pParentMgr->FindDM(strKeyDMAlias);
	CESL_DataMgr * pMainDM = m_pParentMgr->FindDM(strMainDMAlias);
	CESL_Grid * pMainGrid = (CESL_Grid*)m_pParentMgr->FindControl(strMainCMAlias, strMainGridAlias);
	
	if(pKeyDM == NULL || pMainDM == NULL || pMainGrid == NULL) 
		return -1;
	
	CDupKeyMakerByRequestInfo KeyDMMaker(m_pParentMgr);
	KeyDMMaker.SetKeyDM(pKeyDM);
	KeyDMMaker.SetSourceDM(pMainDM);
	KeyDMMaker.SetSourceGrid(pMainGrid);
	
	return KeyDMMaker.MakeKeyDM();
}
