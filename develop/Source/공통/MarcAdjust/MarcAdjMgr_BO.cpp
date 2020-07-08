// MarcAdjMgr_BO.cpp: implementation of the CMarcAdjMgr_BO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MarcAdjMgr_BO.h"

#include "..\..\공통\BO_SPECIES_API\BOFormApi.h"

#include "..\BO_ACQ_SPECIES_FORM\BOAppendixDlg.h"

#include "..\..\관리\분류기호\MNG_CLASS_SIGN_POPUP\MNG_CLASS_SIGN_POPUP_01.h"

#include "..\..\관리\목차초록관리\ContentsMgr_Editor\CONTENTMGR_BO_EDIT.h"

#include "../K2UP_Common/MakeSearchData.h"

#include "..\..\공통\Duplicate_Data_Search\DuplicateSearch_Interface.h"

#include "BO_CAT_AUTO_REGNO_GIVE.h"

#include "..\CAT_FIXED_FIELD\Cat008FixedField_TX.h"
#include "..\CAT_FIXED_FIELD\Cat008FixedField_CF.h"
#include "..\CAT_FIXED_FIELD\Cat008FixedField_MP.h"
#include "..\CAT_FIXED_FIELD\Cat008FixedField_MU.h"
#include "..\CAT_FIXED_FIELD\Cat008FixedField_VM.h"
#include "..\CAT_UNION_FIXED_FIELD\DLG_008_BK.h"
#include "..\CAT_UNION_FIXED_FIELD\DLG_008_CR.h"
#include "..\CAT_UNION_FIXED_FIELD\DLG_008_ER.h"
#include "..\CAT_UNION_FIXED_FIELD\DLG_008_MP.h"
#include "..\CAT_UNION_FIXED_FIELD\DLG_008_MU.h"
#include "..\CAT_UNION_FIXED_FIELD\DLG_008_MX.h"
#include "..\CAT_UNION_FIXED_FIELD\DLG_008_VM.h"
#include "..\CAT_UNION_FIXED_FIELD\DLG_008_BK_1.h"
#include "..\CAT_UNION_FIXED_FIELD\DLG_008_ER_1.h"
#include "..\CAT_UNION_FIXED_FIELD\DLG_008_MP_1.h"
#include "..\CAT_UNION_FIXED_FIELD\DLG_008_MU_1.h"
#include "..\CAT_UNION_FIXED_FIELD\DLG_008_CR_1.h"
#include "..\CAT_UNION_FIXED_FIELD\DLG_008_VM_1.h"
#include "..\CAT_UNION_FIXED_FIELD\DLG_008_MX_1.h"

#include "..\CAT_UNION_FIXED_FIELD\DLG_006.h"

#include "..\..\공통\BO_MARC_IMPORT\MARC_IMPORT.h"

#include "..\..\단행\정리\BO_CAT_PROC\BO_CAT_PROC_0380.h"

#include "..\..\ESL\Marc\MarcToolKit\MarcDefaultValueConfigDlg.h"

#include "..\..\관리\배가기준관리\ShelfCodeGetter\ShelfCodeGetterDlg.h"

#include "..\..\단행\정리\BO_CAT_PROC\BO_CAT_PROC_0330.h"

#include "..\..\공동목록\Light_GatherDataSearch\UC_MARC_IMPORT.h"

#include "..\..\공통\BO_VOLUME_FORM\BOVolumeDlg.h"

#include "..\..\단행\정리\BO_CAT_PROC\BO_CAT_PROC_AcqNumber.h"

#include "..\..\단행\정리\BO_CAT_REGNO_DLG\BO_CAT_REGNO_DLG1.h"

#include "..\..\단행\정리\BO_CAT_NUMBER\BO_CAT_ACQUISIT_NO.h"

#include "..\..\공통\COVERMANAGER\COVER_MAIN.h"
#include "webbrowser2.h"

#include "..\..\공통\SUBJECT_INDEX_SEARCH\CSS_2_01.h"

#include "..\..\공동목록\MOKCHA_CHOROK_API\BO_INERT_TABLE.h"

#include "MarcToolBar.h"

#include "..\\..\\..\\단행\\열람\\장서관리\\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarcAdjMgr_BO::CMarcAdjMgr_BO() : CMarcAdjMgr()
{
	AfxInitRichEdit();

	m_pCatESLMgr = NULL;
	m_pCatApi = NULL;

	m_pResourceMgr = NULL;

	m_pMarcMgr = NULL;
	m_pMarc = NULL;

	m_pCatESLMgr = NULL;

	m_nManageMode = MNG_INSERT;

	m_pDM_Parent = NULL;
	m_pGrid_Parent = NULL;
	m_nParentSelectedRow = -1;

	m_pFrameWnd = NULL;	

	m_bAcceptMessage = TRUE;

	m_nModeMONO_SERIAL = MODE_MONO;

	m_bCatHold = FALSE;

	m_pCreateAuthorCode = NULL;

}

CMarcAdjMgr_BO::~CMarcAdjMgr_BO()
{
	delete m_pMarc;
	m_pMarc = NULL;
	
	if (m_pCatESLMgr)
	{
		m_pCatESLMgr->DestroyWindow();
		delete m_pCatESLMgr;
		m_pCatESLMgr = NULL;
	}

	delete m_pResourceMgr;
	m_pResourceMgr = NULL;

	delete m_pCreateAuthorCode;
	m_pCreateAuthorCode = NULL;

}

INT CMarcAdjMgr_BO::InitManager(INT nManageMode, CESL_Mgr *pParentESLMgr, LPCTSTR lpszWorkingStatus /*= NULL*/, CMarc *pMarc /*= NULL*/)
{
	EFS_BEGIN

	if (pParentESLMgr == NULL) 
	{
		m_strLastErrMsg = _T("부모 ESL Mgr이 NULL 입니다.");
		return -1;
	}
	if (pParentESLMgr->m_pInfo == NULL) return -1;
	if (pParentESLMgr->m_pInfo->pMarcMgr == NULL) return -1;

	INT ids = -1;

	m_nModeMONO_SERIAL = MODE_MONO;
	
	SetManageMode(nManageMode);

	m_pMarcMgr = pParentESLMgr->m_pInfo->pMarcMgr;
	
	if (m_pMarc)
		delete m_pMarc;
	m_pMarc = new CMarc;
	
	if (CreateCatESLMgr(pParentESLMgr) < 0) return -1;

	m_pCatApi = m_pCatESLMgr->GetCatApi();
	if (m_pCatApi == NULL) return -1;

	if (lpszWorkingStatus == NULL)
	{
		if (!m_bCatHold)
			m_strWorkingStatus = _T("BOC113N");
		else
			m_strWorkingStatus = _T("BOL112N");
	}
	else
		m_strWorkingStatus = lpszWorkingStatus;

	if (m_nManageMode == MNG_INSERT)
	{
		if (m_pDM_Parent == NULL || m_pGrid_Parent == NULL || m_strSpeciesKeyAlias.IsEmpty())
		{
			m_strLastErrMsg = _T("부모정보가 없습니다.");
			return -1;
		}

		if (CreateDefaultMarc(m_pMarc) < 0) return -1;

		m_pCatESLMgr->m_pDM_Species->InsertRow(-1);
		m_pCatESLMgr->m_pDM_Index->InsertRow(-1);
		m_pCatESLMgr->m_pDM_Book->InsertRow(-1);
		m_pCatESLMgr->m_pDM_Purchase->InsertRow(-1);

		m_pCatESLMgr->m_pDM_Index->SetCellData(_T("IBS_목차유무"), _T("N"), 0);
		m_pCatESLMgr->m_pDM_Index->SetCellData(_T("IBS_초록유무"), _T("N"), 0);
		
		m_pCatESLMgr->m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), m_pCatApi->GetQueryFlag(MNG_INSERT, m_pCatESLMgr->GetAppMode() ), 0);
		m_pCatApi->SetRepresentationBook(m_pCatESLMgr->m_pDM_Book, _T("BB"));
	}
	else if (m_nManageMode == MNG_MODIFY)
	{
		if (m_pDM_Parent == NULL || m_pGrid_Parent == NULL || m_strSpeciesKeyAlias.IsEmpty())
		{
			m_strLastErrMsg = _T("부모정보가 없습니다.");
			return -1;
		}

		ids = MakeSelectListParentGrid();
		if (ids < 0) return -1;

		ids = MakeUpdateDM();
		if (ids < 0)
		{
			AfxMessageBox(_T("UPDATE DM 생성 실패"));
			return -1;
		}

		CString strStreamMarc = m_pCatESLMgr->m_pDM_Species->GetCellData( _T("BS_MARC"), 0);
		m_pMarcMgr->Decoding(strStreamMarc, m_pMarc);

		m_pMarcMgr->GetItem(m_pMarc, _T("090$b"), m_str090bPrev);
	}	
	else if (m_nManageMode == MNG_IMPORT || m_nManageMode == MNG_IMPORT_WORKING )
	{
		if (!m_pDM_Species_Parent || !m_pDM_Index_Parent || !m_pGrid_Parent) return -1;

		ids = MakeSelectListParentGrid();
		if (ids < 0) return -1;

		ids = MakeImportDM();
		if (ids < 0)
		{
			AfxMessageBox(_T("IMPORT DM 생성 실패"));
			return -1;
		}

		CString strStreamMarc = m_pCatESLMgr->m_pDM_Species->GetCellData( _T("BS_MARC"), 0);
		m_pMarcMgr->Decoding(strStreamMarc, m_pMarc);

		m_pMarcMgr->GetItem(m_pMarc, _T("090$b"), m_str090bPrev);
	}
	
	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::ApplyData()
{
	EFS_BEGIN

	INT ids = -1;
	INT nIdx = 0;

	if (m_pCatApi->GetRepresentationBook(m_pCatESLMgr->m_pDM_Book, _T("BB"), nIdx) < 0)
		nIdx = m_pCatApi->SetRepresentationBook(m_pCatESLMgr->m_pDM_Book, _T("BB"));
	
	CStringArray IgnoreArray;
	IgnoreArray.Add( _T("BB_관리구분_코드"));
	IgnoreArray.Add( _T("BB_이용제한구분_코드"));
	IgnoreArray.Add( _T("BB_수입구분_코드"));

	ids = m_pCatApi->MakeDMFromCM(m_pResourceMgr->m_dlgCode.m_pCM, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->GetAppMode(), nIdx, _T("BB") , &IgnoreArray );
	if( ids < 0 )
	{
		AfxMessageBox(_T("Error in Func : MakeDMFromCM"));
	}

	if (m_pCatApi->GetVolumeCount(m_pCatESLMgr->m_pDM_Book, _T("BB")) == 1)
	{
		CString strVol = m_pCatESLMgr->m_pDM_Book->GetCellData(_T("BB_편권차"), nIdx);

		if (GetManageMode() == MNG_INSERT || GetManageMode() == MNG_IMPORT|| m_nManageMode == MNG_IMPORT_WORKING)
		{
			ids = ApplyOneVolumeData(strVol, MNG_INSERT);

			if (ids < 0) return -1;
		}
		else if (GetManageMode() == MNG_MODIFY)
		{
			ids = ApplyOneVolumeData(strVol, MODE_UPDATE);

			INT nRowCount = m_pCatESLMgr->m_pDM_Book->GetRowCount();

			for (INT idx = 0; idx < nRowCount; idx++)
			{
				ids = m_pCatESLMgr->m_pDM_Book->SetCellData(_T("BB_편권차"), strVol, idx);
			}
		}
	}

	INT nRowCount = m_pCatESLMgr->m_pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!m_pCatApi->IsBook(m_pCatESLMgr->m_pDM_Book, idx)) continue;
		if (m_pCatESLMgr->m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) != _T("I")) continue;

		m_pCatESLMgr->m_pDM_Book->SetCellData(_T("BB_자료상태"), m_strWorkingStatus, idx);
	}

    nRowCount = m_pCatESLMgr->m_pDM_Appendix->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (m_pCatESLMgr->m_pDM_Appendix->GetCellData(_T("BB_DB_FLAG"), idx) != _T("I")) continue;
		m_pCatESLMgr->m_pDM_Appendix->SetCellData(_T("BB_자료상태"), _T("BOC113N"), idx);

		CString strUseLimitCode;
		m_pResourceMgr->m_dlgCode.m_pCM->GetControlMgrData( _T("IBS_이용제한구분_코드"), strUseLimitCode );
		m_pCatESLMgr->m_pDM_Appendix->SetCellData(_T("BB_이용제한구분_코드"), strUseLimitCode, idx);
	}

	INT nBookCount = m_pCatESLMgr->m_pDM_Book->GetRowCount();

	return 0;

	EFS_END
	return -1;
}

UINT CMarcAdjMgr_BO::OnVolumeUpdate()
{
	EFS_BEGIN

	INT nIdx;
	if (m_pCatApi->GetRepresentationBook(m_pCatESLMgr->m_pDM_Book, _T("BB"), nIdx) < 0)
		nIdx = m_pCatApi->SetRepresentationBook(m_pCatESLMgr->m_pDM_Book, _T("BB"));

	m_pCatApi->CopyDM(m_pCatESLMgr->m_pDM_Book, nIdx, m_pCatESLMgr->m_pDM_Index, 0, _T("IBS"));	

	if (m_pCatApi->GetVolumeCount(m_pCatESLMgr->m_pDM_Book, _T("BB")) > 1)
		m_pResourceMgr->m_dlgCode.EnableRegCodeField(FALSE);
	else
		m_pResourceMgr->m_dlgCode.EnableRegCodeField(TRUE);	
	
	ApplyDMChange(false);
    ApplyMarcChange ();

	m_pResourceMgr->m_dlgCode.Display();
	m_pResourceMgr->m_dlgVolume.Display();
	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::ApplyDMChange(bool m_bInit /*=false */)
{
	EFS_BEGIN

	m_pCatApi->CopyDM(m_pCatESLMgr->m_pDM_Index, 0, m_pCatESLMgr->m_pDM_Species, 0, _T("BS"));
	

	CString sTmpShelfCode;
	m_pCatESLMgr->m_pDM_Index->GetCellData(_T("IBS_배가위치부호"), 0, sTmpShelfCode);
	sTmpShelfCode.TrimLeft();
	sTmpShelfCode.TrimRight();

	m_pCatESLMgr->m_pDM_Index->SetCellData(_T("IBS_배가위치부호"), sTmpShelfCode, 0);

	if(m_bInit == false)
	{
		m_pCatApi->ConvertCatDMToMarc(m_pMarc, m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Purchase);
	}
	
	m_pResourceMgr->GetMarcEditor()->Display();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::ApplyMarcChange()
{
	EFS_BEGIN
	
	long nRowLocation;
	long nColLocation;
	
	INT ids = 0;

	ids = m_pCatApi->ConvertMarcToCatDM(m_pMarc, m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Purchase);
	if( ids  < 0 )
	{
		AfxMessageBox(_T("ERROR at Func : ConvertMarcToCat!!"));
		return -1;
	}

	ids = m_pCatApi->CopyDM(m_pCatESLMgr->m_pDM_Index, 0, m_pCatESLMgr->m_pDM_Species, 0, _T("BS"));
	if( ids  < 0 )
	{
		AfxMessageBox(_T("ERROR at Func : CopyDM!!"));
		return -1;
	}

	ids = m_pResourceMgr->m_dlgVolume.Display();
	if( ids  < 0 )
	{
		AfxMessageBox(_T("ERROR at Func : VolumeDlg.Display!!"));
		return -1;
	}

	if(m_pResourceMgr->GetMarcEditor()->GetGridCtrl() != NULL)
	{
		nRowLocation = m_pResourceMgr->GetMarcEditor()->GetGridCtrl()->GetRow() ;
		nColLocation = m_pResourceMgr->GetMarcEditor()->GetGridCtrl()->GetCol() ;
	}
	m_pResourceMgr->GetMarcEditor()->Display();
   
	if(m_pResourceMgr->GetMarcEditor()->GetGridCtrl() != NULL)
	{
		m_pResourceMgr->GetMarcEditor()->GetGridCtrl()->SetRow(nRowLocation) ;
		m_pResourceMgr->GetMarcEditor()->GetGridCtrl()->SetCol(nColLocation) ;
    }

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::ApplyMarcData()
{
	EFS_BEGIN

	INT ids = -1;
	INT idx = -1;
	INT nCount = -1;

	CTime time;
	
	CString strQuery;
	CString strErrMsg;
	CString strItem;
	CString strItemData;
	CString strTemp;
	CString sRegCode;
	CString sItem;

	BOOL bEssiontialTagError = FALSE;

	CArray<CString, CString&> arrItem;
	
	CStringArray arrStrEssentialTag;


	arrStrEssentialTag.Add(_T("008"));
	arrStrEssentialTag.Add(_T("090"));
	arrStrEssentialTag.Add(_T("245"));
	arrStrEssentialTag.Add(_T("260"));
		
	m_pResourceMgr->m_dlgCode.m_pCM->GetControlMgrData( _T("IBS_형식구분_코드"), sRegCode );
	if ( sRegCode != _T("CF") ) {
		arrStrEssentialTag.Add(_T("300"));
	}

	arrStrEssentialTag.Add(_T("049"));
	
	if (!m_pResourceMgr->GetMarcEditor()->GetEditCtrl()->CheckFullSyntax(TRUE))
	{
		AfxMessageBox(_T("마크에 오류가 있습니다. 수정해주세요"));
		return -1;
	}
	
	m_pMarcMgr->GetField( m_pMarc, _T("100"), sItem );
	if ( sItem == _T("") ) {
		m_pMarcMgr->GetField( m_pMarc, _T("110"), sItem );
		if ( sItem == _T("") ) {
			m_pMarcMgr->GetField( m_pMarc, _T("111"), sItem );
			if ( sItem == _T("") ) {
				m_pMarcMgr->GetField( m_pMarc, _T("130"), sItem );
				if ( sItem == _T("") ) {
					m_pMarcMgr->GetField( m_pMarc, _T("700"), sItem );
					if ( sItem == _T("") ) {
						m_pMarcMgr->GetField( m_pMarc, _T("710"), sItem );
						if ( sItem == _T("") ) {
							m_pMarcMgr->GetField( m_pMarc, _T("711"), sItem );
							if ( sItem == _T("") ) {
								m_pMarcMgr->GetField( m_pMarc, _T("740"), sItem );							
								if ( sItem == _T("") ) {
									m_pMarcMgr->CheckDeepSyntax(m_pMarc);
								}
							}
						}
					}
				}
			}
		}
	}

	if (m_pCatESLMgr->m_pDM_Index->GetCellData(_T("IBS_종구분값"), 0) == _T("1"))
	{
		ids = m_pMarcMgr->GetItem(m_pMarc, _T("001*"), strItemData);
		if (ids < 0 || strItemData.GetLength() == 7)
		{
			CESL_DataMgr dm;
			CString *pStr = NULL;
			dm.SetCONNECTION_INFO(m_pCatESLMgr->m_pDM_Book->CONNECTION_INFO);			
			
			ids = dm.StartFrame();
			if ( 0 > ids ) {
				AfxMessageBox( _T("StartFram() 확인") );
				return -1;
			}

			m_pCatApi->CompleteCataloging(&dm, m_pCatESLMgr->m_pDM_Species, 0, NULL, FALSE
											, m_pCatESLMgr->m_pDM_Index, m_pMarc);

			ids = dm.SendFrame();
			if ( 0 > ids ) return -1;
			ids = dm.EndFrame();
			if ( 0 > ids ) return -1;

			CString str;
			m_pMarcMgr->GetItem(m_pMarc, _T("001*"), str);
		}		
	}

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("049$c"), strItemData, &arrItem);
	if (ids >= 0)
	{
		nCount = arrItem.GetSize();
		for (idx = 0; idx < nCount; idx++)
		{
			strItemData = arrItem.GetAt(idx);
			ids = m_pCatApi->IsNumberCopyCode(strItemData);
			if (ids < 0) 
			{
				AfxMessageBox(_T("복본기호 값이 잘못되었습니다. \n복본기호에는 숫자만 입력할 수 있습니다. 수정해주십시오."));
				return -1;
			}
		}
	}

	CString str090c;
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("090$c"), str090c);
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("049$v"), strItemData, &arrItem);
	if ( m_pCatESLMgr->m_pDM_Volume->GetRowCount() < 2 ) 
	{
		if (str090c.GetLength() == 0 && arrItem.GetSize() > 0 )
		{
			AfxMessageBox(_T("090tag $c와 049tag $v가 일치하지 않습니다."));

		}
	}

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("049$f"), strItemData, &arrItem);
	if (ids >= 0)
	{
		nCount = arrItem.GetSize();
		for (idx = 0; idx < nCount; idx++)
		{
			strItemData = arrItem.GetAt(idx);
			ids = m_pCatESLMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"), strItemData, strItemData);
			if (ids < 0) 
			{
				AfxMessageBox(_T("별치기호 값이 잘못되었습니다. 수정해주십시오."));
				return -1;
			}
		}
	}

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("950$b"), strItemData);
	if (ids >= 0)
	{
		strItemData = m_pCatApi->GetPriceCode(strItemData, strTemp);
		if (strItemData.IsEmpty()) 
		{
			AfxMessageBox(_T("[950$b] 에 화폐구분이 없습니다. 수정해주세요"));
			return -1;
		}

		ids = m_pCatESLMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("화폐부호관리"), strItemData, strItemData);
		if (ids < 0)
		{
			AfxMessageBox(_T("[950$b] 의 화폐구분 값이 잘못됬습니다. 수정해주세요"));
			return -1;
		}
	}

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("950$d"), strItemData);
	if (ids >= 0)
	{
		nCount = strItemData.GetLength();
		for (idx = 0; idx < nCount; idx++)
		{
			if (!_istdigit(strItemData.GetAt(idx)))
			{
				AfxMessageBox(_T("[950$d] 의 값에 숫자가 아닌 값이 있습니다. 수정해주세요"));
				return -1;
			}
		}
	}

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("090$a"), strItemData);
	if (ids < 0)
	{
		AfxMessageBox(_T("청구기호가 없습니다. 수정해주세요"));
		return -1;
	}

	m_iSearchSave = 1;
	ids = CMD_SearchCallNo(FALSE);
	m_iSearchSave = 0;
	if (ids < 0)
	{
		AfxMessageBox(_T("청구기호를 다시 생성해 주세요"));
		return -1;
	}

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("653$a"), strItemData);
	if (ids < 0)
	{
		CMD_CreateKeyWord();
	}

	ids = m_pMarcMgr->DeleteEmptySubfield(m_pMarc);
	ids = m_pMarcMgr->DeleteEmptyField(m_pMarc);
	strErrMsg.Empty();
	nCount = arrStrEssentialTag.GetSize();
	for (idx = 0; idx < nCount; idx++)
	{
		strItem = arrStrEssentialTag.GetAt(idx);
		if (m_pMarc->FindTag(strItem) == NULL)
		{
			strErrMsg += _T("[") + strItem + _T("] Tag을 입력해주세요\n");
			bEssiontialTagError = TRUE;
		}
	}
	if (bEssiontialTagError)
	{
		AfxMessageBox(strErrMsg);
		return -1;
	}

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("008*"), strItemData);
	if (!strItemData.IsEmpty())
	{
		if (strItemData.GetLength() != 40) 
		{
			AfxMessageBox(_T("008의 길이는 40 이어야 합니다."));
			return -1;
		}
	}

	time = CTime::GetCurrentTime();

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("008@0-5"), strItemData);
	CIndex::TrimAll(strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		strItemData = time.Format(_T("%y%m%d"));
		m_pMarcMgr->SetItem(m_pMarc, _T("008@0-5"), strItemData);
	}
	
	ids = m_pMarcMgr->GetItem(m_pMarc, _T("008@6"), strItemData);
	if (strItemData == _T("s"))
	{
		ids = m_pMarcMgr->GetItem(m_pMarc, _T("008@7-10"), strItemData);
		ids = m_pMarcMgr->GetItem(m_pMarc, _T("260$c"), strTemp);

		for ( INT i = 0 ; i < strTemp.GetLength() ; i++ ){
			CString sOneWord = strTemp.Mid(i,1);
			if ( sOneWord >= _T("0") && sOneWord <= _T("9") ) {
				sOneWord = strTemp.Mid(i,4);
				strTemp = sOneWord;
				break;
			}
		}
		if (strItemData != strTemp)
		{
			AfxMessageBox(_T("008의 발행년과 260$c의 년도가 다릅니다."));
		}		
	}

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("008@22"), strItemData);
	
	if (m_pMarc->FindTag(_T("130")) && m_pMarc->FindTag(_T("240")))
	{
		AfxMessageBox(_T("130 TAG 과 240 TAG은 같이 쓸 수 없습니다."));
		return -1;
	}

	strItem.Format(_T("%04d%02d%02d%02d%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	m_pMarcMgr->DeleteField(m_pMarc, _T("005"));
	m_pMarcMgr->InsertField(m_pMarc, _T("005") + strItem);

	ids = m_pMarcMgr->DeleteEmptySubfield(m_pMarc);

	ids = m_pMarcMgr->DeleteEmptyField(m_pMarc);	

	if (m_pCatApi->ApplyLastModifyData(m_pMarc) < 0)
	{
		strErrMsg = m_pCatApi->GetLastError();
		AfxMessageBox ( strErrMsg );
		return -1;
	}

	return 0;

EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::ApplyOneVolumeData(CString strVol, INT nMode)
{
	EFS_BEGIN

	CString strQueryFlag;

	CString strRegCodeField = m_pResourceMgr->m_dlgCode.GetRegCodeFieldData();
	if (strRegCodeField.IsEmpty()) return -1;

	CArray<CCatRegCode, CCatRegCode> arrRegCode;
	INT ids = m_pCatApi->FetchRegCode(strRegCodeField, &arrRegCode);
	if (ids < 0 || arrRegCode.GetSize() <= 0)
	{
		m_pResourceMgr->m_dlgCode.RollBackRegCodeFieldData();
		m_strLastErrMsg = _T("등록구분 필드가 잘못되었습니다.");
		return -1;
	}

	m_pResourceMgr->m_dlgCode.SetRegCodeFieldData(strRegCodeField);
	m_pCatApi->MakeBookFromRegCode(&arrRegCode, m_pCatESLMgr->m_pDM_Book, strVol);

	m_pCatApi->SetRepresentationBook(m_pCatESLMgr->m_pDM_Book, _T("BB"));

	CString sVolSort;
	INT		nVolSortNo;

	INT nBookCount = m_pCatESLMgr->m_pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nBookCount; idx ++)
	{
		if (!m_pCatApi->IsBook(m_pCatESLMgr->m_pDM_Book, idx)) continue;
		
		CStringArray IgnoreArray;
		IgnoreArray.Add( _T("BB_관리구분_코드"));
		IgnoreArray.Add( _T("BB_이용제한구분_코드"));
		IgnoreArray.Add( _T("BB_수입구분_코드"));

		ids = m_pCatApi->MakeDMFromCM(m_pResourceMgr->m_dlgCode.m_pCM, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->GetAppMode(), idx, _T("BB"), &IgnoreArray);

		ids = m_pCatESLMgr->m_pDM_Book->SetCellData(_T("BB_편권차"), strVol, idx);

		ids = m_pCatESLMgr->m_pDM_Book->GetCellData(_T("BB_권일련번호"), idx, sVolSort);
		nVolSortNo = _ttoi( sVolSort.GetBuffer(0) );
		if ( nVolSortNo <= 0 )
		{
			ids = m_pCatESLMgr->m_pDM_Book->SetCellData(_T("BB_권일련번호"), _T("10"), idx);			
		}
		ids = m_pCatESLMgr->m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx, strQueryFlag);
		
		ids = m_pCatESLMgr->m_pDM_Book->SetCellData(_T("BB_가격잡정보"), m_pCatESLMgr->m_pDM_Purchase->GetCellData(_T("BP_가격잡정보"), 0), idx);
		CString s950c;
		m_pCatESLMgr->m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("950$c"), s950c );
		ids = m_pCatESLMgr->m_pDM_Book->SetCellData(_T("BB_가격잡정보"), s950c, idx );
	}

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::CMD_AppendixInfo()
{
	EFS_BEGIN
	if (!m_pCatESLMgr->m_pDM_Appendix) return -1;

	CBOAppendixDlg dlg(m_pCatESLMgr);
	dlg.SetAppendixDM(m_pCatESLMgr->m_pDM_Appendix);
	dlg.SetSpeciesDM(m_pCatESLMgr->m_pDM_Species);
	dlg.SetIndexDM(m_pCatESLMgr->m_pDM_Index);
	dlg.SetBookDM(m_pCatESLMgr->m_pDM_Book);

	INT nOpenMode = 1;
	if (m_nManageMode == MNG_MODIFY)
		nOpenMode = -1;
	dlg.SetOpenMode(nOpenMode);
	dlg.DoModal();
	
	return 0;

	EFS_END
	
	return -1;
}

INT CMarcAdjMgr_BO::CMD_ClassCode()
{
	EFS_BEGIN

	CMNG_CLASS_SIGN_POPUP_01 dlg(m_pCatESLMgr);
	if (IDOK != dlg.DoModal()) return -1;

	CString n056 = _T("-1");
	CString n082 = _T("-1");
	CString n085 = _T("-1");
	CString strItemData;
	INT		i;
	
	if ( dlg.m_arrayKDC.GetSize() > 0 )
	{
		for ( i=0 ; i<dlg.m_arrayKDC.GetSize() ; i++ )
		{
			m_pMarcMgr->SetItem(m_pMarc, _T("056$a"), dlg.m_arrayKDC.GetAt(i), _T(""), ADD_SUBFIELD);
		}
		m_pMarcMgr->GetItem(m_pMarc, _T("056$2"), strItemData );
		if ( strItemData.IsEmpty() ) {
			m_pCatApi->LoadClassCodeVolumeDlg(_T("..\\cfg\\분류기호판사항.txt"), n056, n082, n085);
			if (n056 != _T("-1") )
			{
				strItemData.Format(_T("%s"), n056);
				m_pMarcMgr->SetItem(m_pMarc, _T("056$2"), strItemData, _T(""), ADD_SUBFIELD);
			}
		}
	}

	if ( dlg.m_arrayDDC.GetSize() > 0 )
	{
		for ( i=0 ; i<dlg.m_arrayDDC.GetSize() ; i++ )
		{
			m_pMarcMgr->SetItem(m_pMarc, _T("082$a"), dlg.m_arrayDDC.GetAt(i), _T(""), ADD_SUBFIELD);
		}
		m_pMarcMgr->GetItem(m_pMarc, _T("082$2"), strItemData );
		if ( strItemData.IsEmpty() ) {
			m_pCatApi->LoadClassCodeVolumeDlg(_T("..\\cfg\\분류기호판사항.txt"), n056, n082, n085);
			if (n082 != _T("-1") )
			{
				strItemData.Format(_T("%s"), n082);
				m_pMarcMgr->SetItem(m_pMarc, _T("082$2"), strItemData, _T(""), ADD_SUBFIELD);
			}
		}
	}

	if ( dlg.m_arrayLC.GetSize() > 0 )
	{
		for ( i=0 ; i<dlg.m_arrayLC.GetSize() ; i++ )
		{
			m_pMarcMgr->SetItem(m_pMarc, _T("085$a"), dlg.m_arrayLC.GetAt(i), _T(""), ADD_SUBFIELD);
		}
		m_pMarcMgr->GetItem(m_pMarc, _T("085$2"), strItemData );
		if ( strItemData.IsEmpty() ) {
			m_pCatApi->LoadClassCodeVolumeDlg(_T("..\\cfg\\분류기호판사항.txt"), n056, n082, n085);
			if (n085 != _T("-1") )
			{
				strItemData.Format(_T("%s"), n085);
				m_pMarcMgr->SetItem(m_pMarc, _T("085$2"), strItemData, _T(""), ADD_SUBFIELD);
			}
		}
	}

	m_pResourceMgr->GetMarcEditor()->Display();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::CMD_ContentsMgr()
{	
	CCONTENTMGR_BO_EDIT dlg(0, m_pCatESLMgr);
	
	CString strItemRecKey;
	CString strSpeciesKey;
	INT		ids;
	m_pCatESLMgr->m_pDM_Species->GetCellData(_T("BS_종KEY"), 0, strItemRecKey);
	if ( strItemRecKey.IsEmpty() )
	{
		ids = m_pCatESLMgr->m_pDM_Species->MakeRecKey(strSpeciesKey);
		if (ids < 0) return ids;
		m_pCatESLMgr->m_pDM_Species->SetCellData( _T("BS_종KEY"), strSpeciesKey, 0);
		ApplyDMChange();
		ApplyMarcChange();
	}

	dlg.Set_Mgr(_T(""), m_pCatESLMgr->m_pDM_Species->DB_MGR_ALIAS, _T(""), _T("BS_종KEY"), 0 );
	dlg.DoModal() ;

	return 0;
}

INT CMarcAdjMgr_BO::CMD_Create505Tag()
{
	EFS_BEGIN

	m_pCatApi->ConvertBookToVolume(m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume);

	INT nVolumeCount = m_pCatESLMgr->m_pDM_Volume->GetRowCount();
	if (nVolumeCount <= 1) return 0;

	INT ids = m_pMarcMgr->DeleteField(m_pMarc, _T("505"));

	CString strVol;
	CString strVolTitle;
	CString strAuthor;

	m_pCatESLMgr->m_pDM_Volume->SORT(_T("BB_권일련번호"));

	CString strTempVolTitle;
	BOOL bAuthorSep;
	INT  nTitleCnt;
	INT  nTitleIdx;
	for (INT idx = 0; idx < nVolumeCount; idx++)
	{
		strVol.Empty();
		strVolTitle.Empty();
		strAuthor.Empty();
		bAuthorSep= TRUE;
		

		strVol = m_pCatESLMgr->m_pDM_Volume->GetCellData(_T("BB_편권차"), idx);
		strVolTitle = m_pCatESLMgr->m_pDM_Volume->GetCellData(_T("BB_편제명부록명"), idx);
		strAuthor = m_pCatESLMgr->m_pDM_Volume->GetCellData(_T("BB_저작자"), idx);

		if ( strAuthor == _T("") ) bAuthorSep = FALSE;

		if ( strVol == _T("") || strVolTitle == _T("") ) {
			continue;
		}

		if (!strVol.IsEmpty())
			m_pMarcMgr->SetItem( m_pMarc, _T("505$n"), strVol );
		if (!strVolTitle.IsEmpty())
		{
			if ( bAuthorSep == FALSE )
			{
				nTitleCnt = 0;
				nTitleIdx = 0;
				LPCTSTR lpszTok = _tcstok(strVolTitle.GetBuffer(0), _T(":"));
				while (lpszTok)
				{
					nTitleCnt++;
					lpszTok = _tcstok(NULL, _T(":"));
				}
			}

			LPCTSTR lpszTok = _tcstok(strVolTitle.GetBuffer(0), _T(":"));

			if ( bAuthorSep == FALSE )
			{
				while (lpszTok)
				{
					nTitleIdx ++;
					if ( nTitleIdx == nTitleCnt && idx < nVolumeCount - 1)
					{
						strTempVolTitle.Format(_T("%s --"), lpszTok );
						m_pMarcMgr->SetItem( m_pMarc, _T("505$t"), strTempVolTitle );					
					}
					else
					{
						m_pMarcMgr->SetItem( m_pMarc, _T("505$t"), lpszTok );					
					}
					lpszTok = _tcstok(NULL, _T(":"));
				}
			}
			else
			{
				m_pMarcMgr->SetItem( m_pMarc, _T("505$t"), lpszTok );					
				lpszTok = _tcstok(NULL, _T(":"));
			}
			
		}
		if (!strAuthor.IsEmpty())
		{
			if ( bAuthorSep == TRUE && idx < nVolumeCount - 1)
			{
				strAuthor += _T(" -- ");				
			}

			INT iPos = strAuthor.Find( _T(";") );
			INT i505e = 0;
			CString s505d = strAuthor.Left(iPos);
			if ( s505d != _T("") ) {
				m_pMarcMgr->SetItem(m_pMarc, _T("505$d"), s505d );
				i505e = 1;
			}
			CString s505e = strAuthor.Mid(iPos+1);
			INT iPosE = s505e.Find( _T(";") );
			CString s505Right = s505e;
			while ( iPosE >= 0 ) {
				s505e = s505Right.Left(iPosE);
				m_pMarcMgr->SetItem(m_pMarc, _T("505$e"), s505e );
				s505Right = s505Right.Mid(iPosE+1);
				iPosE = s505Right.Find( _T(";") );
			}
			if ( i505e == 1 ) {
				m_pMarcMgr->SetItem( m_pMarc, _T("505$e"), s505Right );
			}
			else {
				m_pMarcMgr->SetItem( m_pMarc, _T("505$d"), s505Right );
			}
		}

	}

	m_pResourceMgr->GetMarcEditor()->Display();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::CMD_CreateCallNo()
{
	EFS_BEGIN

	INT ids = -1;
	CString sCallNo;
	CString sClassNo;
	
	INT iSwCallNoUpdate = 0;
	m_pMarcMgr->GetField( m_pMarc, _T("090"), sCallNo );
	if ( sCallNo != _T("") ) 
	{
		iSwCallNoUpdate = 1;
		m_pMarcMgr->GetItem( m_pMarc, _T("090$a"), sClassNo );
	}
	
	CString strItemData;	
	m_pMarcMgr->GetItem(m_pMarc, _T("090$a"), strItemData);
	if ( strItemData != _T("") ) {
		AfxMessageBox( _T("MARC에 청구기호가 존재합니다.") );
		return -1;
	}		
	
	CString strClass = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("IBS_분류표구분"), 0);
	CIndex::TrimAll(strClass);
	if (strClass.IsEmpty())
	{
		AfxMessageBox( _T("분류표구분을 선택하세요.") );
		return -1;
	}

	CString strClassCodeTag;
	if (strClass == _T("1"))
	{
		strClassCodeTag = _T("056$a");
	}
	else if (strClass == _T("2"))
	{
		strClassCodeTag = _T("082$a");
	}
	else if (strClass == _T("7"))
	{
		strClassCodeTag = _T("085$a");
	}
	else if ( strClass == _T("9"))
	{
		strClassCodeTag = _T("085$a");
	}
	else
	{
		return -1;
	}

	ids = m_pMarcMgr->GetItem(m_pMarc, strClassCodeTag, strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		AfxMessageBox( _T("분류표구분에 맞는 분류기호가 없습니다.") );
		return -1;
	}

	ids = m_pMarcMgr->SetItem(m_pMarc, _T("090$a"), strItemData);
	
	CString s090a = strItemData;
	sClassNo = strItemData;

	CESL_DataMgr *pDmAuthorRule =  m_pCatESLMgr->FindDM( _T("DM_BO_CAT_PROC_0300_AUTHOR_RULE") );
	if ( pDmAuthorRule == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_AUTHOR_RULE error") );
		return -1;
	}

	CString strPublishFormCode;

	strPublishFormCode = _T("MO");
	
	CString sFormCode = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("IBS_형식구분_코드"), 0);
	CString sSubRegCode = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("IBS_보조등록구분_코드"), 0);

	CString strWhere;
	strWhere.Format(_T("PUBLISH_FORM_CODE = '%s' AND FORM_CODE = '%s' AND SUB_REG_CODE = '%s' AND CLASS_CODE = '%s'")
					 , strPublishFormCode, sFormCode, sSubRegCode, sClassNo);
	ids = pDmAuthorRule->RefreshDataManager(strWhere);
	if (ids < 0) return ids;
	if ( pDmAuthorRule->GetRowCount() < 1 ) {
		strWhere.Format(_T("PUBLISH_FORM_CODE = '%s' AND FORM_CODE = '%s' AND SUB_REG_CODE = '%s' AND CLASS_CODE IS NULL")
					 , strPublishFormCode, sFormCode, sSubRegCode);
		ids = pDmAuthorRule->RefreshDataManager(strWhere);
		if (ids < 0) return ids;
		if (pDmAuthorRule->GetRowCount() < 1) {
			AfxMessageBox (_T("저자기호설정이 잘못되었습니다.\n코드 값에 맞는 저자기호 설정이 있는지 확인해주세요"));
			return -1;
		}
	}
	m_pCatESLMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDmAuthorRule, _T("표준저자기호"), _T("생성방법"));
	INT nAuthorCount = pDmAuthorRule->GetRowCount();
		
	CString strTagCount;
	CString strTag;
	CString strSubfield;
	CString strRuleType;
	if ( nAuthorCount > 0 ) 
	{
		pDmAuthorRule->GetCellData( _T("TAG_CNT"), 0, strTagCount );
		pDmAuthorRule->GetCellData( _T("TAG"), 0, strTag );
		pDmAuthorRule->GetCellData( _T("식별기호"), 0, strSubfield );
		pDmAuthorRule->GetCellData( _T("생성방법"), 0, strRuleType );
	}
	
	if (strRuleType != _T("수입순번호")) 
	{	
		CString strFilePath;
		INT iTagCnt = _ttoi( strTagCount.GetBuffer(0) );
		for ( INT i = 1 ; i <= iTagCnt ; i++ ) {
			CString sTagSub;
			if ( i == 1 ) {
				if ( strTag.GetLength() < 3 || strSubfield.GetLength() < 1 ) {
					AfxMessageBox( _T("저자기호설정이 잘못되었습니다.") );
					return -1;
				}
				sTagSub = strTag.Left(3) + _T("$") + strSubfield.Left(1);
			}
			else {
				INT iStart = (i-1)*4;
				INT iTmp = (i-1)*2 +1;
				if ( strTag.GetLength() < iStart+3 || strSubfield < iTmp ) {
					AfxMessageBox( _T("저자기호설정이 잘못되었습니다.") );
					return -1;
				}
				sTagSub = strTag.Mid(iStart, 3) + _T("$") + strSubfield.Mid( (i-1)*2 , 1 );
			}
			strItemData = _T("");
			m_pMarcMgr->GetItem(m_pMarc, sTagSub, strItemData);
			if ( strItemData != _T("") ) {
				break;
			}
		}

		CMakeSearchData MakeData(m_pCatESLMgr);

		ids = MakeData.ChangeHanjaToHangul( strItemData );	

		m_pCatESLMgr->GetCatApi()->DeleteRoleWord(strItemData);

		if (ids < 0) 
		{			
			AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
			return -1;
		}
		
		if ( strRuleType == _T("카터셈본") || strRuleType == _T("카터셈본2") )
		{
			strItemData.MakeUpper();
		}
		else
		{
			ids = MakeData.GetKeyWord( strItemData );				
			if (ids < 0) 
			{			
				AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
				return -1;
			}
		}

		if (strItemData.IsEmpty())
		{
			AfxMessageBox (_T("저자기호설정이 잘못되었습니다."));
			return -1;
		}

		if (strRuleType == _T("장일세"))
		{
			if (!m_pCreateAuthorCode)
				m_pCreateAuthorCode = new CCreateAutorCode;

			CString strAuthorAddData;
			CString strAuthorCode;

			ids = m_pCreateAuthorCode->GetJangIlSeaAuthorData(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();

			if (ids < 0) return -1;

			strItemData = strAuthorCode;
		}
		else if( strRuleType == _T("정필모") )
		{
			if (!m_pCreateAuthorCode)
				m_pCreateAuthorCode = new CCreateAutorCode;
			
			CString strAuthorAddData;
			CString strAuthorCode;
			
			m_pCreateAuthorCode->GetJungPilMoAuthorData(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();
			
			if (ids < 0) return -1;
			
			strItemData = strAuthorCode;
			
		}
		else if( strRuleType == _T("동양서저자기호") )
		{
			if (!m_pCreateAuthorCode)
				m_pCreateAuthorCode = new CCreateAutorCode;

			CString strAuthorAddData;
			CString strAuthorCode;

			strAuthorCode = m_pCreateAuthorCode->GetOrientBookData(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();

			if (ids < 0) return -1;

			strItemData = strAuthorCode;

		}
		else if ( strRuleType == _T("카터셈본") )
		{
			if (!m_pCreateAuthorCode)
				m_pCreateAuthorCode = new CCreateAutorCode;
			
			CString strAuthorAddData;
			CString strAuthorCode;
			
			strAuthorCode = m_pCreateAuthorCode->GetCutterSanbornAuthorData(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();
			
			if (ids < 0) return -1;
			
			strItemData = strAuthorCode;
		
		}
		else if ( strRuleType == _T("카터셈본2") )
		{
			if (!m_pCreateAuthorCode)
				m_pCreateAuthorCode = new CCreateAutorCode;
			
			CString strAuthorAddData;
			CString strAuthorCode;
			
			strAuthorCode = m_pCreateAuthorCode->GetCutterSanborn2AuthorData(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();
			
			if (ids < 0) return -1;
			
			strItemData = strAuthorCode;
		
		}
		else if ( strRuleType == _T("ELORD") ) 
		{

			if (!m_pCreateAuthorCode)
				m_pCreateAuthorCode = new CCreateAutorCode;
			
			CString strAuthorAddData;
			CString strAuthorCode;
			
			INT iTagCnt = _ttoi( strTagCount.GetBuffer(0) );
			for ( INT i = 1 ; i <= iTagCnt ; i++ ) {
				CString sTagSub;
				if ( i == 1 ) {
					sTagSub = strTag.Left(3) + _T("$") + strSubfield.Left(1);
				}
				else {
					INT iStart = (i-1)*3+1;
					INT iTmp = (i-1)*2 +1;
					sTagSub = strTag.Mid(iStart, 3) + _T("$") + strSubfield.Mid( (i-1)*2 , 1 );
				}
				
				m_pMarcMgr->GetItem(m_pMarc, sTagSub, strItemData);
				if ( strItemData != _T("") ) {
					break;
				}
			}
			ids = MakeData.ChangeHanjaToHangul( strItemData );	
			if (ids < 0) 
			{			
				AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
				return -1;
			}
			strItemData.TrimLeft();	strItemData.TrimRight();	
			if(strItemData.GetLength() > 3)
			{
				CString strTemp;
				strItemData = strItemData.Left(3);
			}
			ids = m_pCreateAuthorCode->GetElordAuthorDate(strItemData.GetBuffer(0), strAuthorAddData.GetBuffer(10), strAuthorCode.GetBuffer(10));
			strAuthorAddData.ReleaseBuffer();
			strAuthorCode.ReleaseBuffer();
			
			if (ids < 0) return -1;
			
			strItemData = strAuthorCode;
			
		}
		else
		{
			if (strItemData.GetLength()<1)
			{
				AfxMessageBox( _T("동양서의 저자는 한글 1자리이상이거나 숫자2자리 이상이어야 합니다.") );
				return -1;
			}
				
			CString sData = strItemData.Left(1);
			CString sAuthorMid, sAuthorNo;
			if ( strItemData.Mid(1,1) == _T(" ") ) 
				sAuthorMid = _T(",") + strItemData.Mid(2,1);
			else 
				sAuthorMid = strItemData.Mid(1,1);
			
			
			if ( sAuthorMid != _T("") ) 
			{
				strFilePath = _T("..\\CFG\\") + strRuleType + _T(".txt");
				
				FILE *fp;
				fp = _tfopen( strFilePath , _T("rb") );
				
				if ( fp == NULL ) {
					AfxMessageBox( _T("저자기호설정 파일이 없습니다.") );
					return -1;
				}
				FILE* fpUni = fp;
			
				if( 0xFF == fgetc(fpUni) ) 
				{
					if( 0xFE != fgetc(fpUni) )
					{
						fseek(fpUni, 0, SEEK_SET);
					}
				}
				else
				{
					fseek(fpUni, 0, SEEK_SET);
				}	
				
				TCHAR cLine[256];
				while( _fgetts( cLine, 256, fp ) ) {
					CString sLine;
					sLine.Format( _T("%s"), cLine );
					CString tnp = sLine.Mid(0,3);
					tnp.Replace( _T(" "), NULL );
					if ( sAuthorMid == tnp ) {
						INT iPos = sLine.Find(_T("|"));
						sAuthorNo = sLine.Mid( iPos+1, 3 );
						sAuthorNo.Replace( _T(" "), NULL );
						if ( sAuthorMid.Left(1) == _T(",") ) {
							sAuthorNo = _T(",") + sAuthorNo;						
						}
						break;
					}				
				}
			}
			
			if ( sAuthorNo.IsEmpty())
			{
				AfxMessageBox( _T("저자기호를 찾을 수 없습니다. 저자기호설정 파일을 확인하세요.") );
			}
			
			strItemData = sData + sAuthorNo;
		}
		
		CString sTitle;
		m_pMarcMgr->GetItem(m_pMarc, _T("245$a"), sTitle);

		CIndex::RemoveAllParenthesis(sTitle);

		ids = MakeData.ChangeHanjaToHangul( sTitle );	
		if (ids < 0) 
		{			
			AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
			return -1;
		}
		
		ids = MakeData.GetKeyWord( sTitle );				
		if (ids < 0) 
		{			
			AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
			return -1;
		}

		CString sFirstWord;
		if ( sTitle.GetLength() > 0 ) 
		{
			sFirstWord = sTitle.Left(1);
		}
	
		CString strResult;

		CString strQuery =	_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND GROUP_2='정리' ")
							_T("AND CLASS_ALIAS='저자기호생성방법' AND MANAGE_CODE=UDF_MANAGE_CODE");

		pDmAuthorRule->GetOneValueQuery(strQuery, strResult);

		if ( (sFirstWord.Left(1) >= _T("a") && sFirstWord.Left(1) <= _T("z") ) ||
			 (sFirstWord.Left(1) >= _T("A") && sFirstWord.Left(1) <= _T("a") ) )
		{
			if  ( strRuleType.Find(_T("카터셈본")) >= 0 )
			{
				sFirstWord.MakeLower();
				strItemData = strItemData + sFirstWord.Left(1);
			}
			else
			{
				strItemData = strItemData + sFirstWord.Left(1);
			}			
		} 
		else if ( strResult != _T("서명첫자")) 
		{
			if ( sFirstWord != _T("") ) {
				FILE *fpFirst;
				strFilePath = _T("..\\CFG\\FIRST_CHAR_TBL.txt");
				fpFirst = _tfopen( strFilePath , _T("rb") );			
				if ( fpFirst == NULL ) {
					AfxMessageBox( _T("FIRST_CHAR_TBL.tx 파일이 없습니다.") );
					return -1;
				}

				FILE* fpUni = fpFirst;
			
				if( 0xFF == fgetc(fpUni) ) 
				{
					if( 0xFE != fgetc(fpUni) )
					{
						fseek(fpUni, 0, SEEK_SET);
					}
				}
				else
				{
					fseek(fpUni, 0, SEEK_SET);
				}
				
				TCHAR cLine[256];
				while( _fgetts( cLine, 256, fpFirst ) ) {
					CString sLine;
					sLine.Format( _T("%s"), cLine );
					if ( sLine.Left(1) == _T(".") ) {
						continue;
					}
					CString sTmp = sLine.Left(1);
					if ( sFirstWord == sLine.Left(1) ) {
						INT iPos = sLine.Find(_T("|"));
						CString sResult = sLine.Mid( iPos+1, 1 );
						strItemData = strItemData + sResult;
						break;
					}
				}
			}
		}
		else 
		{
			strItemData = strItemData + sFirstWord;
		}

		m_pMarcMgr->SetItem(m_pMarc, _T("090$a"), s090a);
		m_pMarcMgr->SetItem(m_pMarc, _T("090$b"), strItemData);
	}

	else if ( iSwCallNoUpdate == 0 ) 
	{	
		m_iSwAcqCallNo = 1;
		m_pMarcMgr->SetItem(m_pMarc, _T("090$a"), s090a);
	}
	
	CArray<CString, CString&> pArray;
	CString sItemData;
	m_pMarcMgr->GetItem(m_pMarc, _T("049$v"), sItemData, &pArray);
		
	CString sRegPre;
	INT iCnt = pArray.GetSize();
	for ( INT i = 1 ; i <= iCnt ; i ++ ){
		CString sReg = pArray.GetAt(i-1);
		if ( 1 == i ) {
			sItemData = sReg;
		}
		else if ( sRegPre != sReg ) {
			sItemData = sItemData + _T(",") + sReg;
		}
		sRegPre = sReg;
	}

	if ( sItemData != _T("") ) {
		m_pMarcMgr->SetItem(m_pMarc, _T("090$c"), sItemData);
	}

	m_pResourceMgr->GetMarcEditor()->Display();

	if ( strRuleType != _T("수입순번호") && iSwCallNoUpdate != 1 ) 
	{
		m_iSwCallNo = 1;
		CMD_SearchCallNo(FALSE);
	}

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::CMD_DoDupSearch()
{
	EFS_BEGIN
	BOOL bInsert = FALSE;
	CString strKey;
	CString strTitle;
	CString strResultCnt;
	CString strSetISBN;
	CString strSpeciesClass;
	
	m_pMarcMgr->GetItem(m_pMarc, _T("245$a"), strTitle);
	if (strTitle.IsEmpty()) return -1;
	
	CArray<CString, CString&> array202;
	CString sItem;
	m_pMarcMgr->GetField( m_pMarc, _T("020"), sItem, &array202 );
	for ( INT i = 0 ; i < array202.GetSize() ; i++ ) 
	{
		if ( array202.GetAt(i).Left(1) == _T("1") ) 
			strSetISBN = array202.GetAt(i);
	}
	array202.RemoveAll();

	m_pCatESLMgr->m_pDM_Index->GetCellData (_T("IBS_REC_KEY"),0,strKey) ;
    CString strLastKey ;
	strLastKey = strKey ;
    
	CDuplicateSearch_Interface impl(m_pCatESLMgr);

	if(GetManageMode() == MNG_MODIFY)
	{
		m_pCatESLMgr->m_pDM_Index->GetCellData (_T("IBS_종구분값"),0,strSpeciesClass) ;

		impl.ShowOnlyTitleDuplicateSearch(_T("서명복본조사"), strTitle, strResultCnt, strKey);
		impl.m_strSourceSpeciesClass = strSpeciesClass;
		
		if (strResultCnt == _T("0"))
		{
			AfxMessageBox(_T("서명 복본조사 결과가 없습니다."));
			return -1;
		}
		
		impl.ShowTitleDuplicateSearchDlg(_T("서명복본조사"), strKey);
		if (strKey.IsEmpty()) return -1;
	}
	else 
	{
		impl.ShowOnlyTitleDuplicateSearch(_T("정리서명복본조사"), strTitle, strResultCnt, strKey);
		
		if (strResultCnt == _T("0"))
		{
			AfxMessageBox(_T("서명 복본조사 결과가 없습니다."));
			return -1;
		}
		
		impl.ShowTitleDuplicateSearchDlg(_T("정리서명복본조사"), strKey);	
		if (strKey.IsEmpty()) return -1;
	}
		
	if(GetManageMode() == MNG_MODIFY && strKey.IsEmpty ()) bInsert = FALSE;
	else if(GetManageMode() == MNG_MODIFY && strKey != _T(""))
	{
		bInsert = TRUE; 
		m_pCatApi->GetLastSpeciesKey (strKey) ;
    }

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pCatESLMgr->m_pInfo->CONNECTION_INFO);	
	CString strQuery;
	strQuery.Format(_T("SELECT S.MARC, TESTWORKSTATUS(S.REC_KEY) FROM %s S, %s I WHERE I.REC_KEY=S.REC_KEY and S.REC_KEY = %s"), m_pCatESLMgr->m_pDM_Species->TBL_NAME, m_pCatESLMgr->m_pDM_Index->TBL_NAME, strKey);
	dm.RestructDataManager(strQuery);
	
	CString strTargetStatus = dm.GetCellData(0, 1);
	if (strTargetStatus.IsEmpty()) return -1;

	CString strSourceStatus = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("IBS_작업상태INFO"), 0);

	if ( strTargetStatus.Left(3)==_T("BOT") ||
		 strTargetStatus.Left(3)==_T("BOA") ||
		 strTargetStatus.Left(3)==_T("BOR") ||
		 ( strTargetStatus.Left(3)==_T("BOC") && strSourceStatus.Left(3)==_T("BOL")) )
	{
		AfxMessageBox(_T("지금 정리중인 자료보다 낮은 상태의 자료에는 복본추기가 불가능합니다."));
		CMD_DoDupSearch();
		return 0;
	}

	CString strStreamMarc = dm.GetCellData(0, 0);
	if (strStreamMarc.IsEmpty()) return -1;
      
	m_pMarcMgr->Decoding (strStreamMarc, m_pMarc) ;
    
	if(GetManageMode() == MNG_MODIFY) {
		INT ids;
		CESL_DataMgr *pDstDM = new CESL_DataMgr ;
		
		pDstDM->SetCONNECTION_INFO(m_pCatESLMgr->m_pInfo->CONNECTION_INFO);
		pDstDM->TBL_NAME = m_pCatESLMgr->m_pDM_Book->TBL_NAME;
		
		pDstDM->EXTRA_CONDITION = m_pCatESLMgr->m_pDM_Book->EXTRA_CONDITION;
		pDstDM->CONDITION = m_pCatESLMgr->m_pDM_Book->CONDITION;
		pDstDM->DB_MGR_ALIAS = m_pCatESLMgr->m_pDM_Book->DB_MGR_ALIAS;
		
		pDstDM->InitDataMgrRef( m_pCatESLMgr->m_pDM_Book->RefList.GetCount() );
		pDstDM->m_nCols = m_pCatESLMgr->m_pDM_Book->RefList.GetCount();
		
		CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
		for( INT k = 0 ; k < m_pCatESLMgr->m_pDM_Book->RefList.GetCount() ; k++ )
		{
			sFieldAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
			ids = m_pCatESLMgr->m_pDM_Book->GetColumnRefInfo( k , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
			if( ids < 0 ) AfxMessageBox(_T("MakeCopdyDM Error"));
			
			ids = pDstDM->SetDataMgrRef( k , sFieldAlias , sFieldName , sFieldType  , sInitValue  , sDataType );			
			if( ids < 0 ) AfxMessageBox(_T("MakeCopdyDM Error"));
		}
		
		CString temp_qry = _T("BOOK_APPENDIX_FLAG='B' AND SPECIES_KEY = ") + strKey  ; 
		pDstDM->RefreshDataManager (temp_qry ) ;
		
		CBO_SPECIES api(m_pCatESLMgr);

		api.CopyDataMerge(m_pCatESLMgr->m_pDM_Book, pDstDM, m_pMarc);
		
		CESL_DataMgr *pDmApp = m_pCatESLMgr->FindDM(_T("DM_단행_정리DM_부록정보"));
		CESL_DataMgr *pDmAppDup = m_pCatESLMgr->FindDM(_T("DM_단행_정리DM_부록정보_복본"));

		INT iAppCount = pDmApp->GetRowCount();		
		if ( iAppCount < 1 ) {
			temp_qry = _T("BOOK_APPENDIX_FLAG='A' AND SPECIES_KEY = ") + strKey  ; 
			pDmApp->RefreshDataManager(temp_qry);
		}
		else {
			temp_qry = _T("BOOK_APPENDIX_FLAG='A' AND SPECIES_KEY = ") + strKey  ; 
			pDmAppDup->RefreshDataManager(temp_qry);
			INT iAppDup = pDmAppDup->GetRowCount();
			CString sOriSpeciesKey = pDmApp->GetCellData( _T("BB_종KEY"), 0 );
			for ( INT i = 0 ; i < iAppCount ; i++ ) {				
				pDmApp->SetCellData( _T("BB_복본종KEY"), sOriSpeciesKey, i );
				pDmApp->SetCellData( _T("BB_종KEY"), strKey, i);
				pDmApp->SetCellData( _T("BB_DB_FLAG"), _T("U"), i);
			}
			for ( i = 0 ; i < iAppDup ; i++ ) {
				pDmApp->InsertRow(-1);
				for ( INT j = 0 ; j < pDmAppDup->GetColCount() ; j++ ) {
					CString sTmp = pDmAppDup->GetCellData(i,j);			
					CString sAlias, sFieldName, sFieldType, sVal, sDataType;
					pDmAppDup->GetColumnRefInfo(j,sAlias, sFieldName, sFieldType, sVal, sDataType);
					pDmApp->SetCellData( sAlias, pDmAppDup->GetCellData(sAlias,i), iAppCount );
				}
				pDmApp->SetCellData( _T("BB_DB_FLAG"), _T("U"), iAppCount);
				iAppCount++;
			}
		}

		m_pCatESLMgr->m_pDM_Book->FreeData() ;
		m_pCatESLMgr->m_pDM_Book->CopyDM(pDstDM) ;
    }

    if(GetManageMode() == MNG_INSERT)
	{
		CString strBefore001 ,strBefore049;
		CArray<CString,CString&> arrBefore049 ;
		m_pMarcMgr->GetItem (m_pMarc ,_T("001*"),strBefore001) ;
		m_pMarcMgr->GetField (m_pMarc,_T("049"),strBefore049 );
		
		m_pMarcMgr->Decoding(strStreamMarc, m_pMarc);
		
		if(!strBefore001.IsEmpty()) 
		{    
			m_pMarcMgr->DeleteField (m_pMarc ,_T("001"),_T("")) ;
			m_pMarcMgr->InsertField (m_pMarc,_T("001")+strBefore001 ) ;
		}
		else
		{
			m_pMarcMgr->DeleteField (m_pMarc ,_T("001"),_T("")) ;
		}
		
		if(!strBefore049 .IsEmpty ())
		{
			m_pMarcMgr->DeleteField (m_pMarc ,_T("049"),_T("")) ;
			m_pMarcMgr->InsertField (m_pMarc,_T("049")+strBefore049 ) ;		
		}
		else
		{
			m_pMarcMgr->DeleteField (m_pMarc ,_T("049"),_T("")) ;
		}

    }
	    
	if(GetManageMode() == MNG_MODIFY )
	{
		CString tmpQuery ;
		tmpQuery.Format (_T(" rec_key = %s"),strKey) ;
		m_pCatESLMgr->m_pDM_Index->RefreshDataManager (tmpQuery);
		m_pCatESLMgr->m_pDM_Species->RefreshDataManager (tmpQuery);
		
		m_pResourceMgr->m_dlgVolume.Display() ;

		ApplyDMChange();

		m_pCatESLMgr->m_pDM_Index -> SetCellData (_T("IBS_복본여부"),_T("Y"),0) ;				
		
		for(INT i = 0 ;i < m_pCatESLMgr->m_pDM_Book->GetRowCount () ;i++)
		{
			m_pCatESLMgr->m_pDM_Book->SetCellData (_T("BB_DB_FLAG"),_T("U"),i) ;
		}
		m_pGrid_Parent->DisplayLine(m_nParentSelectedRow);
    }	
	
	if( strSetISBN.IsEmpty() == FALSE )
	{
		sItem = _T("");
		m_pMarcMgr->GetField( m_pMarc, _T("020"), sItem, &array202 );
		m_pMarcMgr->DeleteField( m_pMarc, _T("020"), ' ', ' ');
		m_pMarcMgr->DeleteField( m_pMarc, _T("020"), '1', ' ');
		for ( INT i = 0 ; i < array202.GetSize() ; i++ ) 
		{
			if ( array202.GetAt(i).Left(1) == _T(" ") ) 
				m_pMarcMgr->InsertField(m_pMarc,_T("020")+array202.GetAt(i));
		}
		m_pMarcMgr->InsertField(m_pMarc,_T("020")+strSetISBN);
	}
	array202.RemoveAll();

	m_pResourceMgr->GetMarcEditor()->Display();
	
    m_pResourceMgr->m_dlgCode.ShowRegCodeField () ;

	m_pResourceMgr->m_dlgCode.Display();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::CMD_EditFixedField2()
{
	EFS_BEGIN
		CDLG_006 dlg(m_pCatESLMgr, m_pMarc);		
		if ( dlg.DoModal() == IDOK )
		{
			ApplyMarcChange();
			m_pResourceMgr->GetMarcEditor()->Display();
		}

	return 0;

	EFS_END

	return -1;
}

INT CMarcAdjMgr_BO::CMD_EditFixedField()
{
	EFS_BEGIN

	CString sFormCode = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("IBS_형식구분_코드"), 0);

	CString strStreamMarc;

	CString strGetGroupName = _T("");
	CString strFileName = _T("..\\cfg\\MarcRelation\\통합마크_개정_그룹명.cfg");
	CFileFind find;
	BOOL bRevision = find.FindFile(strFileName);
	find.Close();

	if ( sFormCode == _T("BK") ) 
	{
		if (TRUE == bRevision) {
			CDLG_008_BK_1 dlg(m_pCatESLMgr, m_pMarc);
			dlg.DoModal();
		} else {
			CDLG_008_BK dlg(m_pCatESLMgr, m_pMarc);
			dlg.DoModal();
		}
	}
	else if ( sFormCode == _T("CR") ) 
	{
		if (TRUE == bRevision) {
			CDLG_008_CR_1 dlg(m_pCatESLMgr, m_pMarc);
			dlg.DoModal();
		} else {
			CDLG_008_CR dlg(m_pCatESLMgr, m_pMarc);
			dlg.DoModal();
		}
	}
	else if ( sFormCode == _T("ER") ) 
	{
		if (TRUE == bRevision) {
			CDLG_008_ER_1 dlg(m_pCatESLMgr, m_pMarc);
			dlg.DoModal();
		} else {
			CDLG_008_ER dlg(m_pCatESLMgr, m_pMarc);
			dlg.DoModal();
		}
	}
	else if ( sFormCode == _T("MP") ) 
	{
		if (TRUE == bRevision) {
			CDLG_008_MP_1 dlg(m_pCatESLMgr, m_pMarc);
			dlg.DoModal();
		} else {
			CDLG_008_MP dlg(m_pCatESLMgr, m_pMarc);
			dlg.DoModal();
		}
	}
	else if ( sFormCode == _T("MU") ) 
	{
		if (TRUE == bRevision) {
			CDLG_008_MU_1 dlg(m_pCatESLMgr, m_pMarc);
			dlg.DoModal();
		} else {
			CDLG_008_MU dlg(m_pCatESLMgr, m_pMarc);
			dlg.DoModal();
		}
	}
	else if ( sFormCode == _T("MX") ) 
	{
		if (TRUE == bRevision) {
			CDLG_008_MX_1 dlg(m_pCatESLMgr, m_pMarc);
			dlg.DoModal();
		} else {
			CDLG_008_MX dlg(m_pCatESLMgr, m_pMarc);
			dlg.DoModal();
		}
	}
	else if ( sFormCode == _T("VM") ) 
	{
		if (TRUE == bRevision) {
			CDLG_008_VM_1 dlg(m_pCatESLMgr, m_pMarc);
			dlg.DoModal();
		} else {
			CDLG_008_VM dlg(m_pCatESLMgr, m_pMarc);
			dlg.DoModal();
		}
	}
	else if (sFormCode == _T("BK")) 
	{
		CCat008FixedField_TX dlg(m_pCatESLMgr, m_pMarc);
		dlg.DoModal();
	}
	else if (sFormCode == _T("CF")) 
	{
		CCat008FixedField_CF dlg(m_pCatESLMgr, m_pMarc);
		dlg.DoModal();
	}
	else if ( sFormCode == _T("MP") ) 
	{
		CCat008FixedField_MP dlg(m_pCatESLMgr, m_pMarc);
		dlg.DoModal();
	}
	else if ( sFormCode == _T("MU") ) 
	{
		CCat008FixedField_MU dlg(m_pCatESLMgr, m_pMarc);
		dlg.DoModal();
	}
	else if ( sFormCode == _T("VM") ) 
	{
		CCat008FixedField_VM dlg(m_pCatESLMgr, m_pMarc);
		dlg.DoModal();
	}	
	else 
	{
		CCat008FixedField_TX dlg(m_pCatESLMgr, m_pMarc);
		dlg.DoModal();
	}
	
	CString strKey;
	CString strTemp;
	m_pMarcMgr->GetItem(m_pMarc, _T("007@0"), strTemp);
	strKey += strTemp;

	m_pMarcMgr->GetItem(m_pMarc, _T("007@1"), strTemp);
	strKey += strTemp;		
	
	strTemp = m_pCatApi->GetCommonMatInfo(strKey, sFormCode);
	if (!strTemp.IsEmpty())
	{
		CString strItemData;
		strItemData.Format(_T("[%s]"), strTemp);
		CString sTmp;
		m_pMarcMgr->GetItem(m_pMarc, _T("245$h"), sTmp);
		if ( sTmp == _T("") ) {
			m_pMarcMgr->SetItem(m_pMarc, _T("245$h"), strItemData);
		}
	}
	
	ApplyMarcChange();

	m_pMarcMgr->GetItem(m_pMarc, _T("008@22"), strTemp);
	if ( strTemp == _T("j") ) {
		m_pCatESLMgr->m_pDM_Index->SetCellData( _T("IBS_이용대상구분_코드"), _T("JU"), 0 );	
		m_pCatESLMgr->m_pDM_Species->SetCellData( _T("BS_이용대상구분_코드"), _T("JU"), 0 );		

		m_pResourceMgr->m_dlgCode.m_pCM->SetControlMgrData(_T("IBS_이용대상구분_코드"), _T("JU"));

		INT nRowCnt = 0;
		nRowCnt = m_pCatESLMgr->m_pDM_Book->GetRowCount();
		for( INT i = 0 ; i < nRowCnt ; i++)
		{
			m_pCatESLMgr->m_pDM_Book->SetCellData( _T("BB_이용대상구분_코드"), _T("JU"), i );
		}
		nRowCnt = m_pCatESLMgr->m_pDM_Volume->GetRowCount();
		for( i = 0 ; i < nRowCnt ; i++)
		{
			m_pCatESLMgr->m_pDM_Volume->SetCellData( _T("BB_이용대상구분_코드"), _T("JU"), i );
		}
	}

	CString nTempCode;
	m_pResourceMgr->m_dlgCode.m_pCM->GetControlMgrData(_T("IBS_이용대상구분_코드"), nTempCode);
	if ( strTemp == " " && nTempCode == "JU" ) 
	{
		m_pCatESLMgr->m_pDM_Index->SetCellData( "IBS_이용대상구분_코드", "PU", 0 );	
		m_pCatESLMgr->m_pDM_Species->SetCellData( "BS_이용대상구분_코드", "PU", 0 );

		m_pResourceMgr->m_dlgCode.m_pCM->SetControlMgrData(_T("IBS_이용대상구분_코드"), _T("PU"));

		INT nRowCnt = 0;
		nRowCnt = m_pCatESLMgr->m_pDM_Book->GetRowCount();
		for( INT i = 0 ; i < nRowCnt ; i++)
		{
			m_pCatESLMgr->m_pDM_Book->SetCellData( _T("BB_이용대상구분_코드"), _T("PU"), i );
		}
		nRowCnt = m_pCatESLMgr->m_pDM_Volume->GetRowCount();
		for( i = 0 ; i < nRowCnt ; i++)
		{
			m_pCatESLMgr->m_pDM_Volume->SetCellData( _T("BB_이용대상구분_코드"), _T("PU"), i );
		}
	}

	CArray<CString, CString&> arrItem;
	CString strData;
	CString strItem;
	BOOL bFind;

	bFind = FALSE;

	m_pMarcMgr->GetItem(m_pMarc, _T("521$a"), strData, &arrItem);
	INT nCount = arrItem.GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		strData = arrItem.GetAt(idx);			
		if ( strTemp == _T("j") ) 
		{		
			strItem = _T("아동용");
			if ( strData.Find(_T("아동")) >= 0 ) bFind = TRUE;			
		}
		else if ( strTemp == _T("a") ) 
		{
			strItem = _T("취학전 아동");		
			if ( strData.Find(_T("취학전 아동")) >= 0 ) bFind = TRUE;			
		}
		else if ( strTemp == _T("b") ) 
		{
			strItem = _T("초등학생");		
			if ( strData.Find(_T("초등학생")) >= 0 ) bFind = TRUE;			
		}
		else if ( strTemp == _T("c") ) 
		{
			strItem = _T("중학생");
			if ( strData.Find(_T("중학생")) >= 0  ) bFind = TRUE;			
		}
		else if ( strTemp == _T("d") ) 
		{
			strItem = _T("고등학생");
			if ( strData.Find(_T("고등학생")) >= 0 ) bFind = TRUE;			
		}
		else if ( strTemp == _T("e") ) 
		{
			strItem = _T("성인용");
			if ( strData.Find(_T("성인용")) >= 0 ) bFind = TRUE;			
		}		
		else if ( strTemp == _T("f") ) 
		{
			strItem = _T("특수계층(시각, 청각, 발달 등의 장애인용)");
			if ( strData.Find(_T("특수계층")) >= 0 ) bFind = TRUE;			
		}		
		else if ( strTemp == _T("z") ) 
		{
			strItem = _T("미상 혹은 적용 불가");
			if ( strData.Find(_T("미상")) >= 0 ) bFind = TRUE;			
		}
	}
	if ( nCount == 0 )
	{
		bFind = FALSE;		
		if ( strTemp == "j" ) strItem = _T("아동용");
		else if ( strTemp == "a" ) strItem = _T("취학전 아동");		
		else if ( strTemp == "b" ) strItem = _T("초등학생");		
		else if ( strTemp == "c" ) strItem = _T("중학생");
		else if ( strTemp == "d" ) strItem = _T("고등학생");
		else if ( strTemp == "e" ) strItem = _T("성인용");	
		else if ( strTemp == "f" ) strItem = _T("특수계층(시각, 청각, 발달 등의 장애인용)");
		else if ( strTemp == "z" ) strItem = _T("미상 혹은 적용 불가");
	}

	if ( bFind == FALSE && !strItem.IsEmpty())
	{
		m_pMarcMgr->SetItem(m_pMarc, _T("521$a"), strItem);
	}
	
	m_pResourceMgr->GetMarcEditor()->Display();	

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::CMD_GiveRegNoAuto()
{
	EFS_BEGIN

	ApplyData();
	INT iRowCount = m_pCatESLMgr->m_pDM_Book->GetRowCount();
	INT iSwOk = 0;
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sRegNo = m_pCatESLMgr->m_pDM_Book->GetCellData( _T("BB_등록번호"), i );
		CString sRegCode = m_pCatESLMgr->m_pDM_Book->GetCellData( _T("BB_등록구분_코드"), i );
		if ( sRegNo == _T("") && sRegCode != _T("") ) {
			iSwOk = 1;
			break;
		}
	}
	if ( iSwOk == 0 ) {
		AfxMessageBox( _T("등록번호 부여할 책이 없습니다.") );
		return -1;
	}

	CESL_DataMgr *pDmLastNum = m_pCatESLMgr->FindDM( _T("DM_마지막번호") );
	if (pDmLastNum == NULL) {
		AfxMessageBox( _T("DM_마지막번호 확인") );
		return -1;
	}

	CBO_CAT_AUTO_REGNO_GIVE dlg(m_pCatESLMgr);
	if ( dlg.DoModal() == IDOK ) {
		for ( INT i = 0 ; i < dlg.m_aLastNum.GetSize() ; i++ ) {
			CString sLastNum = dlg.m_aLastNum.GetAt(i);
			CString sRegCode = dlg.m_aRegCode.GetAt(i);
			INT iYes = 0;
			for ( INT j = 0 ; j < pDmLastNum->GetRowCount() ; j++ ) {
				if ( sRegCode == pDmLastNum->GetCellData(_T("BB_등록구분_코드"),j) ) {
					pDmLastNum->SetCellData( _T("마지막번호"), sLastNum, j );
					iYes = 1;
					break;
				}
			}
			if ( iYes == 0 ) {
				pDmLastNum->InsertRow(-1);
				pDmLastNum->SetCellData( _T("마지막번호"), sLastNum, i );
				pDmLastNum->SetCellData( _T("BB_등록구분_코드"), sRegCode, i );
			}
		}
	}

	m_pResourceMgr->m_dlgVolume.Display();
	ApplyDMChange();
	
	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::CMD_MarcImport()
{
	EFS_BEGIN

	CString sTitle, sAuthor, sPublisher, sPubYear;

	m_pMarcMgr->GetItem( m_pMarc, _T("245$a"), sTitle );
	m_pMarcMgr->GetItem( m_pMarc, _T("245$d"), sAuthor );
	m_pMarcMgr->GetItem( m_pMarc, _T("260$b"), sPublisher );
	m_pMarcMgr->GetItem( m_pMarc, _T("260$c"), sPubYear );	

	CMARC_IMPORT dlg(m_pCatESLMgr);
	dlg.SetSearchCondition( sTitle, sAuthor, sPublisher, sPubYear);
	if (dlg.DoModal() != IDOK) return -1;

	CMarcEditor *pMarcEditor = m_pResourceMgr->GetMarcEditor();
	
	CString sRecKey = dlg.m_strSpecies_key;
	
	INT iMode = dlg.m_nRadBasicInfo;
	
	CString sQuery;
	sQuery.Format(_T("SELECT MARC FROM %s WHERE REC_KEY = %s"), m_pCatESLMgr->m_pDM_Species->TBL_NAME, sRecKey);
	CString sStreamMarc;
	m_pCatESLMgr->m_pDM_Book->GetOneValueQuery( sQuery , sStreamMarc );
	
	CString s001FirstData ,s049FirstData ;
	m_pMarcMgr->GetItem (m_pMarc,_T("001*"),s001FirstData) ;
    m_pMarcMgr->GetField (m_pMarc,_T("049"),s049FirstData) ;

	CMarc marcTmp;
	if ( iMode == 0 ) {			
		m_pMarc->Init();
		m_pMarcMgr->Decoding(sStreamMarc, &marcTmp);
		CString sItemData;
        
        if(s001FirstData != _T("")) {
            s001FirstData = _T("001") + s001FirstData;
			m_pMarcMgr->InsertField( m_pMarc, s001FirstData );
		}
        if(s049FirstData != _T("")) {
            s049FirstData = _T("049") + s049FirstData;
			m_pMarcMgr->InsertField( m_pMarc, s049FirstData );
		}
      
		m_pMarcMgr->GetField( &marcTmp, _T("245"), sItemData );			
		if ( sItemData != _T("") ) {
			sItemData = _T("245") + sItemData;
			m_pMarcMgr->InsertField( m_pMarc, sItemData );
		}
		
		sItemData = _T("");
		m_pMarcMgr->GetField( &marcTmp, _T("250"), sItemData );			
		if ( sItemData != _T("") ) {
			sItemData = _T("250") + sItemData;
			m_pMarcMgr->InsertField( m_pMarc, sItemData );
		}
		
		sItemData = _T("");
		m_pMarcMgr->GetField( &marcTmp, _T("260"), sItemData );
		if ( sItemData != _T("") ) {
			sItemData = _T("260") + sItemData;
			m_pMarcMgr->InsertField( m_pMarc, sItemData );
		}
		
		sItemData = _T("");
		m_pMarcMgr->GetField( &marcTmp, _T("300"), sItemData );
		if ( sItemData != _T("") ) {
			sItemData = _T("300") + sItemData;
			m_pMarcMgr->InsertField( m_pMarc, sItemData );
		}
		
		sItemData = _T("");
		m_pMarcMgr->GetField( &marcTmp, _T("950"), sItemData );
		if ( sItemData != _T("") ) {
			sItemData = _T("950") + sItemData;
			m_pMarcMgr->InsertField( m_pMarc, sItemData );
		}
		
		sItemData = _T("");
		m_pMarcMgr->GetField( &marcTmp, _T("020"), sItemData );
		if ( sItemData != _T("") ) {
			sItemData = _T("020") + sItemData;
			m_pMarcMgr->InsertField( m_pMarc, sItemData );
		}
	}
	else if ( iMode == 1 ) {
		m_pMarc->Init();
		m_pMarcMgr->Decoding(sStreamMarc, m_pMarc);
		
		m_pMarcMgr->DeleteField( m_pMarc, _T("001") );
		m_pMarcMgr->DeleteField( m_pMarc, _T("049") );

        if(s001FirstData != _T("")) {
            s001FirstData = _T("001") + s001FirstData;
			m_pMarcMgr->InsertField( m_pMarc, s001FirstData );
		}
        if(s049FirstData != _T("")) {
            s049FirstData = _T("049") + s049FirstData;
			m_pMarcMgr->InsertField( m_pMarc, s049FirstData );
		}
	}
	else if ( iMode == 2 ) {
		m_pMarc->Init();
		m_pMarcMgr->Decoding(sStreamMarc, m_pMarc);

		m_pMarcMgr->DeleteField( m_pMarc, _T("001") );
		m_pMarcMgr->DeleteField( m_pMarc, _T("049") );
		m_pMarcMgr->DeleteField( m_pMarc, _T("505") );

        if(s001FirstData != _T("")) {
            s001FirstData = _T("001") + s001FirstData;
			m_pMarcMgr->InsertField( m_pMarc, s001FirstData );
		}
        if(s049FirstData != _T("")) {
            s049FirstData = _T("049") + s049FirstData;
			m_pMarcMgr->InsertField( m_pMarc, s049FirstData );
		}
	}
	
	pMarcEditor->Display();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::CMD_Save()
{
	EFS_BEGIN

	iOnMarcEditUpdateCheckCount = 0;
	m_bOnMarcEditUpdateCheck = TRUE;
	m_bOnMarcEditUpdateCountCheck = TRUE;
	INT nTemp = OnMarcEditUpdate();

	if (nTemp == -1000 )
	{
		AfxMessageBox(m_strLastErrMsg);
		return -1;
	}
	else if(nTemp < 0 ) 
	{
		
		return -1 ;
	}

	AfxGetApp()->BeginWaitCursor();

	INT ids = -1;

	ids = m_pResourceMgr->m_dlgCode.ApplyCodeChange(FALSE);

	ids = ApplyData();
	if (ids < 0) return ids;
	
	if (m_strWorkingStatus != _T("BOC112O"))
	{
		ids = ApplyMarcData();
		if (ids < 0) return ids;
		
		INT nBookCount = m_pCatESLMgr->m_pDM_Book->GetRowCount();

		ids = ApplyMarcChange();
		if (ids < 0) return ids;
		nBookCount = m_pCatESLMgr->m_pDM_Book->GetRowCount();

		ids = ((CCatApi_BO*)m_pCatApi)->ApplyShelfCodeData(m_pCatESLMgr->m_pDM_Book, m_pMarc, m_bCatHold);
		if (ids < 0) return ids;

		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(m_pCatESLMgr->m_pDM_Book->CONNECTION_INFO);
		CString strQuery;
		CString strUseLimitCode;
		strQuery = _T("SELECT LOAN_PERIOD, CODE FROM CD_CODE_TBL WHERE CLASS = 19 AND ( CODE <> '0' OR DESCRIPTION <> '적용안함' ) ORDER BY LOAN_PERIOD");

		dm.RestructDataManager ( strQuery );

		for ( INT nCoIdx = 0 ; nCoIdx < dm.GetRowCount() ; nCoIdx++ )
		{
			for ( INT nBookIdx = 0 ; nBookIdx <  m_pCatESLMgr->m_pDM_Book->GetRowCount() ; nBookIdx++ )
			{
				CString stttt = dm.GetCellData( nCoIdx, 1 );
				CString ssssss = m_pCatESLMgr->m_pDM_Book->GetCellData(_T("BB_배가위치부호"), nBookIdx );

				if ( dm.GetCellData( nCoIdx, 1 ) ==  m_pCatESLMgr->m_pDM_Book->GetCellData(_T("BB_배가위치부호"), nBookIdx ) )
				{

					m_pCatESLMgr->m_pDM_Species->SetCellData(_T("BS_배가위치부호"), m_pCatESLMgr->m_pDM_Book->GetCellData(_T("BB_배가위치부호"), nBookIdx ) , 0);
					break;
				}
			}
		}

		ids = ((CCatApi_BO*)m_pCatApi)->ApplyShelfCodeData(m_pCatESLMgr->m_pDM_Appendix, m_pMarc, m_bCatHold);
		if (ids < 0) return ids;

		nBookCount = m_pCatESLMgr->m_pDM_Book->GetRowCount();

		m_pCatESLMgr->m_pDM_Species->SetCellData( _T("BS_정리상태"), _T("1"), 0 );

		ids = m_pMarcMgr->DeleteEmptySubfield(m_pMarc);

		ids = m_pMarcMgr->DeleteEmptyField(m_pMarc);	

	}
	
	ids = CreateAcqCallNo();
	if ( ids == 100 ) 
	{
		return 0;
	}

	m_pFrameWnd->RedrawWindow();
	AfxGetApp()->BeginWaitCursor();

	CString n056 = _T("0");
	CString n082 = _T("0");
	CString n085 = _T("0");

	CString strItemData;
	ids = m_pCatApi->LoadClassCodeVolumeDlg(_T("..\\cfg\\분류기호판사항.txt"), n056, n082, n085);

	if (n056 != _T("-1") && n082 != _T("-1") && n085 != _T("-1"))
	{
		ids = m_pMarcMgr->GetItem(m_pMarc, _T("056$a"), strItemData);
		if (ids >= 0 && !strItemData.IsEmpty())
		{
			ids = m_pMarcMgr->GetItem(m_pMarc, _T("056$2"), strItemData);
			if (ids < 0 || strItemData.IsEmpty())
			{
				strItemData.Format(_T("%s"), n056);
				ids = m_pMarcMgr->SetItem(m_pMarc, _T("056$2"), strItemData, _T(""), ADD_SUBFIELD);
			
			}
		}

		ids = m_pMarcMgr->GetItem(m_pMarc, _T("082$a"), strItemData);
		if (ids >= 0 && !strItemData.IsEmpty())
		{
			ids = m_pMarcMgr->GetItem(m_pMarc, _T("082$2"), strItemData);
			if (ids < 0 || strItemData.IsEmpty())
			{
				strItemData.Format(_T("%s"), n082);
				ids = m_pMarcMgr->SetItem(m_pMarc, _T("082$2"), strItemData, _T(""), ADD_SUBFIELD);

			}
		}

		ids = m_pMarcMgr->GetItem(m_pMarc, _T("085$a"), strItemData);
		if (ids >= 0 && !strItemData.IsEmpty())
		{
			ids = m_pMarcMgr->GetItem(m_pMarc, _T("085$2"), strItemData);
			if (ids < 0 || strItemData.IsEmpty())
			{
				strItemData.Format(_T("%s"), n085);
				ids = m_pMarcMgr->SetItem(m_pMarc, _T("085$2"), strItemData, _T(""), ADD_SUBFIELD);

			}
		}
	}

	CString strLeader;
	m_pMarc->GetLeader(strLeader);

	CString sTmp = strLeader.Mid( 5, 1 );
	if ( strLeader.Mid( 5, 1 ) == _T(" ") ) {
		CString strStatus = _T("n");
		CString strForm = _T("a");
		CString strSeoji = _T("m");
		CString strInputLevel = _T(" ");
		CString sTmp;
		sTmp.Format(_T("%5s%1s%1s%1s%s%1s%s"), 
					strLeader.Left(5), 
					strStatus, 
					strForm, 
					strSeoji, 
					strLeader.Mid(8, 9), 
					strInputLevel, 
					strLeader.Right(6));

		m_pMarc->SetLeader( sTmp.GetBuffer(0) );
	}

	INT nBookCount = m_pCatESLMgr->m_pDM_Book->GetRowCount();	

	CString strMarc;
	ids = m_pMarcMgr->Encoding(m_pMarc, strMarc);

	ids = m_pCatESLMgr->m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, 0);

	INT i;
	for ( i = 0 ; i < m_pCatESLMgr->m_pDM_Appendix->GetRowCount() ; i++ ) 
	{		
		if (m_pCatESLMgr->m_pDM_Appendix->GetCellData(_T("BB_DB_FLAG"), i) != _T("I"))
		{
			continue;
		}

		m_pCatESLMgr->m_pDM_Appendix->SetCellData(_T("BB_자료상태"), _T("BOC113N"), i );  		
	}

	if (GetManageMode() == MNG_INSERT)
	{
		CString sProvider;
		CString sAcqCode = m_pCatESLMgr->m_pDM_Book->GetCellData( _T("BB_수입구분_코드"), 0 );
		if ( sAcqCode == _T("1") ) {
			ids = m_pResourceMgr->m_dlgCode.m_pCM->GetControlMgrData( _T("BP_구입처부호"), sProvider );			
			m_pCatESLMgr->m_pDM_Book->SetCellData( _T("BB_구입처"), sProvider, 0 );
		}
		else if ( sAcqCode == _T("2") ) {
			m_pResourceMgr->m_dlgCode.GetDlgItem(IDC_eDONATOR)->GetWindowText(sProvider);
			m_pCatESLMgr->m_pDM_Book->SetCellData( _T("BB_구입처"), sProvider, 0 );
		}
		
		CString strInputStatus;
		ids = m_pResourceMgr->m_dlgCode.m_pCM->GetControlMgrData(_T("입력자료상태"), strInputStatus);

 		if ( strInputStatus == _T("소장자료") )
 		{
 			m_bCatHold = TRUE;
 			m_strWorkingStatus = _T("BOL112N");
 		}
 		else 
 		{
 			m_bCatHold = FALSE;
 			m_strWorkingStatus = _T("BOC113N");
 		}
 
 		m_pCatESLMgr->m_pDM_Index->SetCellData(_T("IBS_작업상태INFO"), m_strWorkingStatus, 0 );

 		for ( INT i = 0 ; i < m_pCatESLMgr->m_pDM_Book->GetRowCount() ; i++ ) 
 		{	
			if (!m_pCatApi->IsBook(m_pCatESLMgr->m_pDM_Book, i))
			{
				continue;
			}

			if (m_pCatESLMgr->m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), i) != _T("I")) 
			{
				continue;
			}

			m_pCatESLMgr->m_pDM_Book->SetCellData(_T("BB_자료상태"), m_strWorkingStatus, i );
 		}		

		if (strInputStatus == _T("소장자료"))
		{
			ids = m_pCatApi->ImportSpecies(m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Appendix, &m_arrStrAddFrame, CTime::GetCurrentTime().Format(_T("%Y/%m/%d")));

			if (ids == -1000 )
			{
	 			AfxMessageBox(_T("등록번호가 중복됩니다."));
				return -1;
			}
			else if ( ids < 0 ) 
			{
				return -1;
			}
			
		}
		else if (strInputStatus = _T("정리자료"))
		{
			ids = ((CCatApi_BO*)m_pCatApi)->InsertSpecies(m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Appendix, m_pCatESLMgr->GetAppMode() , &m_arrStrAddFrame);

			if (ids == -1000 )
			{
				AfxMessageBox(_T("등록번호가 중복됩니다."));
				return -1;
			}
			else if ( ids < 0 ) 
			{
				return -1;
			}
		}
	}
	else if (GetManageMode() == MNG_MODIFY)
	{
		
		ids = ((CCatApi_BO*)m_pCatApi)->UpdateSpecies(m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Appendix, m_pCatESLMgr->GetAppMode(), &m_arrStrAddFrame);
		
		if (ids == -1000 )
		{
			AfxMessageBox(_T("등록번호가 중복됩니다."));
			return -1;
		}
		else if ( ids < 0 ) 
		{
			return -1;
		}
	}
	else if (GetManageMode() == MNG_IMPORT)
	{
		m_pCatESLMgr->m_pDM_Index->SetCellData(_T("IBS_작업상태INFO"), _T("BOL112N"), 0 );
		
		ids = ((CCatApi_BO*)m_pCatApi)->ImportSpecies(m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Appendix, &m_arrStrAddFrame, CTime::GetCurrentTime().Format(_T("%Y/%m/%d")));		
		if (ids >= 0)
		{
			CString strData = m_pGrid_Parent->GetTextMatrix(m_nParentSelectedRow + 1, 0);
			if (strData.IsEmpty()) return -1;

			INT idxDM = _ttoi(strData) - 1;

			ids = m_pDM_Species_Parent->SetCellData(_T("BS_파일반입성공"), _T("Y"), idxDM);

			ids = m_pDM_Species_Parent->SetCellData(_T("BS_파일반입오류"), _T(""), idxDM);			
		}
		else if (ids == -1000 )
		{
			AfxMessageBox(_T("등록번호가 중복됩니다."));
			m_pCatESLMgr->m_pDM_Species->EndFrame();
			return -1;
		}
		else if (ids < 0 )
		{
			AfxMessageBox(_T("저장중 에러가 발생하였습니다."));
			m_pCatESLMgr->m_pDM_Species->EndFrame();
			return -1;
		}
	}
	else if (GetManageMode() == MNG_IMPORT_WORKING)
	{
		m_pCatESLMgr->m_pDM_Index->SetCellData(_T("IBS_작업상태INFO"), _T("BOC113N"), 0 );
		for ( INT i = 0 ; i < m_pCatESLMgr->m_pDM_Book->GetRowCount() ; i++ ) {
			
			m_pCatESLMgr->m_pDM_Book->SetCellData(_T("BB_자료상태"), _T("BOC113N"), i );
		}
		
		ids = ((CCatApi_BO*)m_pCatApi)->InsertSpecies(m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Appendix, m_pCatESLMgr->GetAppMode(), &m_arrStrAddFrame);
		
		if (ids >= 0)
		{
			CString strData = m_pGrid_Parent->GetTextMatrix(m_nParentSelectedRow + 1, 0);
			if (strData.IsEmpty()) return -1;

			INT idxDM = _ttoi(strData) - 1;

			ids = m_pDM_Species_Parent->SetCellData(_T("BS_파일반입성공"), _T("Y"), idxDM);
		}
		else if (ids == -1000 )
		{
			AfxMessageBox(_T("등록번호가 중복됩니다."));
			m_pCatESLMgr->m_pDM_Species->EndFrame();
			return -1;
		}
		else if (ids < 0 )
		{
			AfxMessageBox(_T("저장중 에러가 발생하였습니다."));
			m_pCatESLMgr->m_pDM_Species->EndFrame();
			return -1;
		}
	}

	CBO_INERT_TABLE api(m_pCatESLMgr);
    
    CString strSpeciesKey = m_pCatESLMgr->m_pDM_Species->GetCellData(_T("BS_종KEY"), 0);

    api.GetMarcSteam (strMarc) ;
	if ( m_pCatESLMgr->m_pDM_Index->GetCellData (_T("목차"), 0 ) != _T("") ||
		 m_pCatESLMgr->m_pDM_Index->GetCellData (_T("초록"), 0 ) != _T("") )
	{
		api.SetInsertValue(strSpeciesKey, 
							m_pCatESLMgr->m_pDM_Species->GetCellData(_T("BS_제어번호"), 0 ) , 
							m_pCatESLMgr->m_pDM_Index->GetCellData (_T("목차"), 0 ) , 
							m_pCatESLMgr->m_pDM_Index->GetCellData (_T("초록"), 0 ) );
	}

	m_arrRegNo.RemoveAll();

	m_pMarc->GetLeader(m_strLeaderPrev);

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("007*"), m_str007FieldPrev);

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("008*"), m_str008FieldPrev);

	m_pResourceMgr->GetMarcEditor()->Display();
	ids = m_pResourceMgr->m_dlgVolume.Display();

	AfxGetApp()->EndWaitCursor();

	if (ids < 0)
		m_strLastErrMsg.Format(_T("저장 실패\n%s"), m_pCatApi->GetLastError());
	else
		m_strLastErrMsg = _T("저장하였습니다.");

	if (m_strWorkingStatus != _T("BOC112O")) {
		m_pCatESLMgr->m_pDM_Species->SetCellData( _T("BS_정리상태"), _T("정리됨"), 0 );
	}
	else {
		m_pCatESLMgr->m_pDM_Species->SetCellData( _T("BS_정리상태"), _T("정리안됨"), 0 );
	}
	
	CString sDupYn = m_pCatESLMgr->m_pDM_Index->GetCellData( _T("IBS_복본여부"), 0 );

	if ( sDupYn != _T("") )
	{
		if ( m_pDM_Parent )	m_pDM_Parent->SetCellData( _T("BS_복본여부"), _T("V"), m_nParentSelectedRow );
	}
	else
	{
		if ( m_pDM_Parent ) m_pDM_Parent->SetCellData( _T("BS_복본여부"), _T(" "), m_nParentSelectedRow );
	}
	
	if ( m_pDM_Parent ) 
	{
		CString strCallNo;
		CString strCallNoQuery;
		strCallNoQuery.Format(_T("SELECT ECO$F_MAKE_CALLNO$I( SE_SHELF_CODE, CLASS_NO, BOOK_CODE, VOL_CODE) FROM IDX_BO_TBL WHERE REC_KEY = %s"), strSpeciesKey);
		m_pDM_Parent->GetOneValueQuery( strCallNoQuery, strCallNo );
		m_pDM_Parent->SetCellData( _T("종청구기호"), strCallNo, m_nParentSelectedRow );
	}

	m_pFrameWnd->RecalcLayout();
	AfxMessageBox(m_strLastErrMsg);


	if (ids >= 0)
	{
		BackupOldMarc();

		m_arrStrAddFrame.RemoveAll();

		if (GetManageMode() == MNG_INSERT)
		{
			m_pDM_Parent->InsertRow(-1);
			INT nRowIdx = m_pDM_Parent->GetRowCount() - 1;

			m_pCatApi->CopyDM(m_pCatESLMgr->m_pDM_Index, 0, m_pDM_Parent, nRowIdx);

			m_pCatApi->CopyDM(m_pCatESLMgr->m_pDM_Species, 0, m_pDM_Parent, nRowIdx);

			CString strRecKey;
			strRecKey = m_pCatESLMgr->m_pDM_Species->GetCellData(_T("BS_종KEY"), 0 );
			m_pDM_Parent->SetCellData(_T("IBS_종KEY"), strRecKey, nRowIdx);

			INT nIdx = - 1;
			ids = m_pCatApi->GetRepresentationBook(m_pCatESLMgr->m_pDM_Book, _T("BB"), nIdx);
			if (ids < 0 || nIdx < 0)
				nIdx = m_pCatApi->SetRepresentationBook(m_pCatESLMgr->m_pDM_Book, _T("BB"), nIdx);
			m_pDM_Parent->SetCellData(_T("시작등록번호"), m_pCatESLMgr->m_pDM_Book->GetCellData(_T("BB_등록번호"), nIdx), nRowIdx);

			strItemData.Format(_T("%d (%d)"), m_pCatApi->GetBookCount(m_pCatESLMgr->m_pDM_Book), m_pCatApi->GetBookCount(m_pCatESLMgr->m_pDM_Book));
			m_pDM_Parent->SetCellData(_T("책수"), strItemData, nRowIdx);

			CString strCallNo = MakeCallNo();
			m_pDM_Parent->SetCellData(_T("종청구기호"), strCallNo, nRowIdx);
	
			m_pGrid_Parent->DisplayLine(nRowIdx);

			InitManager(GetManageMode(), m_pCatESLMgr->pParentMgr, m_strWorkingStatus, NULL);
			
			CString strArrCode;
			m_pCatESLMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("등록구분"), strArrCode);

            m_pResourceMgr->m_dlgCode.GetDlgItem(IDC_eREG_CLASS_CODE_MA)->EnableWindow(TRUE);
			m_pResourceMgr->m_dlgCode.SetDlgItemText(IDC_eREG_CLASS_CODE_MA,strArrCode);

			m_pCatApi->MakeDMFromCM(m_pResourceMgr->m_dlgCode.m_pCM, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->GetAppMode(), 0, _T("IBS"));

			m_pCatApi->MakeDMFromCM(m_pResourceMgr->m_dlgCode.m_pCM, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->GetAppMode(), 0, _T("BB"));

			m_pResourceMgr->m_dlgVolume.Display();
			InitMarcEditor();

			if (m_pResourceMgr->m_dlgCode.m_bFixedFieldMaintenance)
				ApplyFiexedFieldMaintenance();

			m_pResourceMgr->GetMarcEditor()->Display();

			BackupOldMarc();
		}
		else if (GetManageMode() == MNG_MODIFY)
		{
			m_pCatApi->CopyDM(m_pCatESLMgr->m_pDM_Index, 0, m_pDM_Parent, m_nParentSelectedRow);
			
			nBookCount = m_pCatESLMgr->m_pDM_Book->GetRowCount();

			m_pCatApi->CopyDM(m_pCatESLMgr->m_pDM_Species, 0, m_pDM_Parent, m_nParentSelectedRow);
			CString strDupFlag;
			m_pDM_Parent->GetCellData(_T("BS_복본여부"), m_nParentSelectedRow, strDupFlag );

			if ( strDupFlag == _T("Y") )
			{
				m_pDM_Parent->SetCellData(_T("BS_복본여부"), _T("V"), m_nParentSelectedRow );
			}

			m_pGrid_Parent->DisplayLine(m_nParentSelectedRow);

			nBookCount = m_pCatESLMgr->m_pDM_Book->GetRowCount();

			CMD_Next();
		}
	}

	return ids;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::CMD_SearchCallNo(BOOL bShowMsg /*= TRUE*/)
{
	EFS_BEGIN

	CESL_DataMgr *pDM = m_pCatESLMgr->FindDM(_T("DM_BO_CAT_PROC_0380"));
	if (pDM == NULL) return -1;

	INT ids = -1;

	CString strCss;

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("090$a"), strCss);
	if (ids < 0 || strCss.IsEmpty())
	{
		if (bShowMsg) 
			AfxMessageBox (_T("먼저 청구기호생성을 하십시요."));
		return -1;
	}

	CESL_DataMgr *pDM_SPECIES = m_pCatESLMgr->FindDM(_T("DM_단행_정리DM_색인정보"));

	if (pDM_SPECIES == NULL) {
		pDM_SPECIES = m_pCatESLMgr->FindDM(_T("DM_연속_정리DM_색인정보"));
		if ( pDM_SPECIES == NULL ) return -1;
	}
	CString sMarcKey = pDM_SPECIES->GetCellData( _T("IBS_REC_KEY"), 0 );

	CString strBookCode;
	CString aa;
	m_pMarcMgr->GetItem(m_pMarc, _T("090$b"), strBookCode);

		if ( m_iSearchSave != 1 ) {
		INT strBookCodeLen, ii;
		CString strBookCodeTemp;
		strBookCodeLen = strBookCode.GetLength();
		strBookCodeTemp = strBookCode;
		strBookCode = _T("");

		for ( ii = 0 ; ii < strBookCodeLen ; ii++)
		{	
			if (strBookCodeTemp.Mid( ii , 1) >= _T("0")  && strBookCodeTemp.Mid(ii,1) <= _T("9"))
			{				
				break;
			}
		
			strBookCode += strBookCodeTemp.Mid( ii , 1);
		}
		for (  ; ii < strBookCodeLen ; ii++)
		{	
			if (strBookCodeTemp.Mid( ii , 1) < _T("0")  || strBookCodeTemp.Mid(ii,1) > _T("9"))
			{
				break;
			}
		
			strBookCode += strBookCodeTemp.Mid( ii , 1);
		}
	}
	strBookCode.Replace(_T("'"), _T("''"));
	CString sTmpBookCode = strBookCode;
	strBookCode.Replace(_T(","), NULL);
	strBookCode.MakeUpper();

	CString strWhere, sSub;

	CString sQuery, sItem;
	CString s049v;
	m_pMarcMgr->GetItem(m_pMarc, _T("049$v"), s049v);
	if ( s049v != _T("") ) {
		sQuery.Format(	_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
						_T("WHERE CLASS_ALIAS='청구기호키설정' AND VALUE_NAME='권책기호' AND GROUP_2='정리' ")
						_T("AND SYS_SETUP_YN='N' AND MANAGE_CODE=UDF_MANAGE_CODE") );

		pDM_SPECIES->GetOneValueQuery( sQuery, sItem );
		if ( sItem == _T("Y") ) {
			sSub = _T(" AND I.IDX_VOL LIKE '%") + s049v + _T("%'");				
		}
	}
	CString s049f;
	m_pMarcMgr->GetItem(m_pMarc, _T("049$f"), s049f);
	if ( s049f != _T("") ) {
		sQuery.Format(	_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
						_T("WHERE CLASS_ALIAS='청구기호키설정' AND VALUE_NAME='별치기호' AND GROUP_2='정리' ")
						_T("AND SYS_SETUP_YN='N' AND MANAGE_CODE=UDF_MANAGE_CODE") );

		pDM_SPECIES->GetOneValueQuery( sQuery, sItem );
		if ( sItem == _T("Y") ) {
			sSub = sSub + _T(" AND i.idx_se_shelf_code like '%") + s049f + _T("%'");
		}
	}

	if ( m_iSearchSave != 1 ) {
		strWhere.Format( _T("I.REC_KEY = S.REC_KEY AND I.WORKING_STATUS != 'BOT212O' AND ( I.CLASS_NO = '%s'%s AND (I.BOOK_CODE LIKE '%s%%' OR I.BOOK_CODE LIKE '%s%%') OR S.REC_KEY = '%s' )"), strCss, sSub, strBookCode, sTmpBookCode, sMarcKey );
	}
	else {
		if (sMarcKey.IsEmpty()) {
			strWhere.Format(_T("I.REC_KEY = S.REC_KEY AND I.WORKING_STATUS != 'BOT212O' AND I.CLASS_NO = '%s'%s AND (I.BOOK_CODE = '%s' OR I.BOOK_CODE = '%s') AND I.REC_KEY IS NOT NULL "), strCss, sSub, strBookCode, sTmpBookCode);
		}
		else {
			strWhere.Format(_T("I.REC_KEY = S.REC_KEY AND I.WORKING_STATUS != 'BOT212O' AND I.CLASS_NO = '%s'%s AND (I.BOOK_CODE = '%s' OR I.BOOK_CODE = '%s') AND I.REC_KEY != '%s'"), strCss, sSub, strBookCode, sTmpBookCode, sMarcKey);
		}
		
	}

	if (pDM->RefreshDataManager(strWhere) < 0)	return -1;

	INT iRowCount = pDM->GetRowCount();

	INT ids2 = 0;
	if (pDM->GetRowCount() >= 1 ) 	
	{
		INT iSwFind = 0;
		for( INT j = 0 ; j < pDM->GetRowCount() ; j++ ) {
			CString sQuerySpeciesKey = pDM->GetCellData(_T("BS_종KEY"), j );
			if ( sQuerySpeciesKey == sMarcKey ) {
				iRowCount--;
				iSwFind = 1;
				break;
			}
		}

		if ( iRowCount < 1 && m_iSwCallNo != 1 ) {
			AfxMessageBox( _T("중복된 청구기호가 없습니다.") );
			return 0;
		}
		if ( iRowCount < 1 ) {
			return 0;
		}

		CString sStreamMarc;
		INT ids = m_pMarcMgr->Encoding(m_pMarc, sStreamMarc);
		pDM->SetCellData( _T("BS_MARC"), sStreamMarc, j );

		CString str_tmpVolCode,str_tmpBookCode,sSeperate;
		for(INT i = 0 ; i < pDM->GetRowCount () ; i++)
		{    
			str_tmpVolCode.Empty () ;
			str_tmpBookCode.Empty () ;
			pDM->GetCellData (_T("BS_MARC.청구기호_권책기호"),i,str_tmpVolCode) ;
			pDM->GetCellData (_T("BS_MARC.청구기호_도서기호"),i,str_tmpBookCode) ;

			pDM->SetCellData (_T("권책기호"),str_tmpVolCode,i);
			pDM->SetCellData (_T("도서기호"),str_tmpBookCode,i);

			CString sWorking_status = _T("");
			ids = pDM->GetCellData(_T("배가상태"),i,sWorking_status);
			if (ids == 0)
			{
				CLocCommonAPI::GetWorkingStatusDescToCode( sWorking_status, 1);
				pDM->SetCellData(_T("배가상태"),sWorking_status,i);
			}

		}

		CBO_CAT_PROC_0380 dlg(m_pCatESLMgr);
		dlg.m_strParentSpeciesKey = m_pCatESLMgr->m_pDM_Species->GetCellData(_T("BS_종KEY"), 0);
		if (dlg.DoModal() == IDOK) 
			ids = 0;
		else
		{
			ids = -1;
			ids2 = -1;
		}
	}
	else
	{
		if (bShowMsg)
			AfxMessageBox( _T("중복된 청구기호가 없습니다.") );
	}
	
	return ids2;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::CMD_SetDefaultValue()
{
	EFS_BEGIN

	CString strGroup;
	CString strFormCode = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("IBS_형식구분_코드"), 0);
	CMarcDefaultValueConfigDlg dlg(m_pCatESLMgr);
	dlg.SetMarcGroupFormCode(strFormCode, m_pMarcMgr->GetMarcGroup());
	strGroup = dlg.GetGroup();
	if (dlg.DoModal() != IDOK) return -1;

	INT ids = -1;
	CMarcEditor *pMarcEditor = m_pResourceMgr->GetMarcEditor();
	
	CStringArray *pStrArray = NULL;
	ids = dlg.GetUserMarcAlias(strGroup, &pStrArray);
	if (ids < 0) return ids;
	
	pMarcEditor->InitUserAlias();
	
	INT nCount = pStrArray->GetSize();
	for (INT idx = 0; idx < nCount; idx++)
		pMarcEditor->AddUserAlias(pStrArray->GetAt(idx));
	
	pMarcEditor->GetGridCtrl()->Display();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::CMD_ShelfCodeSearch()
{
	EFS_BEGIN

	CShelfCodeGetterDlg dlg(m_pCatESLMgr);
	if ( dlg.DoModal() == IDOK ) 
	{
		CString sCode = dlg.m_code;
		m_pMarcMgr->SetItem(m_pMarc, _T("049$f"), sCode);
		m_pResourceMgr->GetMarcEditor()->Display();
		ApplyMarcChange();
	}

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::CMD_SupportClasification()
{
	EFS_BEGIN

	CMarcEditor *pMarcEditor = m_pResourceMgr->GetMarcEditor();

	CString sTitle;
	m_pCatESLMgr->m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("245$a"), sTitle );	

	CBO_CAT_PROC_0330 dlg(m_pCatESLMgr, sTitle);
	dlg.m_nModeMONO_SERIAL = MODE_MONO;
	if (IDOK != dlg.DoModal()) return -1;
	
	INT n056 = -1;
	INT n082 = -1;
	INT n085 = -1;
	CString s056;
	CString s082;
	CString s085;
	CString strItemData;
	m_pCatApi->LoadClassCodeVolumeDlg(_T("..\\cfg\\분류기호판사항.txt"), s056, s082, s085);

	INT iLen = dlg.m_KDC.GetLength();
	if ( iLen > 1 ) {
		m_pMarcMgr->SetItem(m_pMarc, _T("056$a"), dlg.m_KDC , _T(""), ADD_SUBFIELD);		
		m_pMarcMgr->GetItem(m_pMarc, _T("056$2"), strItemData );

		n056 = _ttoi(s056);
		if ( strItemData.IsEmpty() && n056 >= 0)
		{
			strItemData = dlg.m_KDC;
			strItemData.Format(_T("%s"), s056);
			m_pMarcMgr->SetItem(m_pMarc, _T("056$2"), strItemData, _T(""), ADD_SUBFIELD);
		}
	}
	iLen = dlg.m_DDC.GetLength();
	if ( iLen > 1 ) {
		m_pMarcMgr->SetItem(m_pMarc, _T("082$a"), dlg.m_DDC , _T(""), ADD_SUBFIELD);
		m_pMarcMgr->GetItem(m_pMarc, _T("082$2"), strItemData );

		n082 = _ttoi(s082);
		if ( strItemData.IsEmpty() && n082 >= 0)
		{
			strItemData = dlg.m_DDC;
			strItemData.Format(_T("%s"), s082);
			m_pMarcMgr->SetItem(m_pMarc, _T("082$2"), strItemData, _T(""), ADD_SUBFIELD);
		}
	}
	iLen = dlg.m_LC.GetLength();
	if ( iLen > 1 ) {
		CString str085Field;
		m_pMarcMgr->GetField(m_pMarc,_T("085"), str085Field );
		if ( str085Field.IsEmpty() ) m_pMarcMgr->InsertField(m_pMarc, _T("0855 "));
		m_pMarcMgr->SetItem(m_pMarc, _T("085$a"), dlg.m_LC , _T(""), ADD_SUBFIELD);
		m_pMarcMgr->GetItem(m_pMarc, _T("085$2"), strItemData );	

		n085 = _ttoi(s085);
		if ( strItemData.IsEmpty() && n085 >= 0)
		{
			strItemData = dlg.m_LC;
			strItemData.Format(_T("%s"), s085);
			m_pMarcMgr->SetItem(m_pMarc, _T("085$2"), strItemData, _T(""), ADD_SUBFIELD);
		}
	}

	m_pMarcMgr->SortMarc( m_pMarc );
	pMarcEditor->Display();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::CMD_UCImport()
{
	EFS_BEGIN
	CString sTitle, sAuthor, sPublisher, sPubYear;
	
	m_pMarcMgr->GetItem( m_pMarc, _T("245$a"), sTitle );
	m_pMarcMgr->GetItem( m_pMarc, _T("245$d"), sAuthor );
	m_pMarcMgr->GetItem( m_pMarc, _T("260$b"), sPublisher );
	m_pMarcMgr->GetItem( m_pMarc, _T("260$c"), sPubYear );

	CUC_MARC_IMPORT dlg(m_pCatESLMgr);
	dlg.SetSearchCondition( sTitle, sAuthor, sPublisher, sPubYear);
	if ( dlg.DoModal() != IDOK ) return -1;
	
	CString s001FirstData ,s049FirstData ;
	m_pMarcMgr->GetItem (m_pMarc,_T("001*"),s001FirstData) ;
    m_pMarcMgr->GetField (m_pMarc,_T("049"),s049FirstData) ;

	INT iMode = dlg.m_check;
	CString sStreamMarc = dlg.sMarc;
	CString s008;
	CMarc marcTmp;
	if ( iMode == 0 ) {			
		
		m_pMarcMgr->Decoding(sStreamMarc, &marcTmp);
	
		m_pMarcMgr->GetItem(&marcTmp,_T("008*"),s008);

		CTag *pTag = NULL;
		POSITION posOld = NULL;
		POSITION pos = m_pMarc->m_listTag.GetTailPosition();
		while (pos)
		{
			posOld = pos;
			pTag = (CTag*)m_pMarc->m_listTag.GetPrev(pos);
			if (!pTag) continue;

			if (pTag->GetTagCode() == _T("001") ||
				pTag->GetTagCode() == _T("049")) continue;

			delete pTag;
			m_pMarc->m_listTag.RemoveAt(posOld);
		}
		
		m_pMarcMgr->SetItem(m_pMarc,_T("008*"),s008);
		CString strField;
		
		m_pMarcMgr->GetField( &marcTmp, _T("035"), strField );			
		if ( strField != _T("") ) {
			strField = _T("035") + strField;
			m_pMarcMgr->InsertField( m_pMarc, strField );
		}

		strField = _T("");		
		m_pMarcMgr->GetField( &marcTmp, _T("245"), strField );			
		if ( strField != _T("") ) {
			strField = _T("245") + strField;
			m_pMarcMgr->InsertField( m_pMarc, strField );
		}
		
		strField = _T("");
		m_pMarcMgr->GetField( &marcTmp, _T("250"), strField );
		if ( strField != _T("") ) {
			strField = _T("250") + strField;
			m_pMarcMgr->InsertField( m_pMarc, strField );
		}
		
		strField = _T("");
		m_pMarcMgr->GetField( &marcTmp, _T("260"), strField );
		if ( strField != _T("") ) {
			strField = _T("260") + strField;
			m_pMarcMgr->InsertField( m_pMarc, strField );
		}
		
		strField = _T("");
		m_pMarcMgr->GetField( &marcTmp, _T("300"), strField );
		if ( strField != _T("") ) {
			strField = _T("300") + strField;
			m_pMarcMgr->InsertField( m_pMarc, strField );
		}
		
		strField = _T("");
		m_pMarcMgr->GetField( &marcTmp, _T("950"), strField );
		if ( strField != _T("") ) {
			strField = _T("950") + strField;
			m_pMarcMgr->InsertField( m_pMarc, strField );
		}
		
		strField = _T("");
		m_pMarcMgr->GetField( &marcTmp, _T("020"), strField );
		if ( strField != _T("") ) {
			strField = _T("020") + strField;
			m_pMarcMgr->InsertField( m_pMarc, strField );
		}
	}
	else if ( iMode == 1 ) {
		m_pMarc->Init();
		m_pMarcMgr->Decoding(sStreamMarc, m_pMarc);
      
		m_pMarcMgr->DeleteField( m_pMarc, _T("001") );
		m_pMarcMgr->DeleteField( m_pMarc, _T("049") );
        if(s001FirstData != _T("")) {
            s001FirstData = _T("001") + s001FirstData;
			m_pMarcMgr->InsertField( m_pMarc, s001FirstData );
		}
        if(s049FirstData != _T("")) {
            s049FirstData = _T("049") + s049FirstData;
			m_pMarcMgr->InsertField( m_pMarc, s049FirstData );
		}
	}
	else if ( iMode == 2 ) {
		m_pMarc->Init();
		m_pMarcMgr->Decoding(sStreamMarc, m_pMarc);

		m_pMarcMgr->DeleteField( m_pMarc, _T("001") );
		m_pMarcMgr->DeleteField( m_pMarc, _T("049") );
		m_pMarcMgr->DeleteField( m_pMarc, _T("505") );

        if(s001FirstData != _T("")) {
            s001FirstData = _T("001") + s001FirstData;
			m_pMarcMgr->InsertField( m_pMarc, s001FirstData );
		}
        if(s049FirstData != _T("")) {
            s049FirstData = _T("049") + s049FirstData;
			m_pMarcMgr->InsertField( m_pMarc, s049FirstData );
		}
	}
	
	if ( !dlg.m_Contents.IsEmpty() && dlg.m_TocCheck_CON == TRUE )
	
	{
		m_pCatESLMgr->m_pDM_Index->SetCellData(_T("목차"), dlg.m_Contents , 0 );
		m_pCatESLMgr->m_pDM_Index->SetCellData(_T("IBS_목차유무"), _T("Y") , 0 );
	}

	if ( !dlg.m_Abstracts.IsEmpty() && dlg.m_TocChek_ABS == TRUE )
	{
		m_pCatESLMgr->m_pDM_Index->SetCellData(_T("초록"), dlg.m_Abstracts , 0 );
		m_pCatESLMgr->m_pDM_Index->SetCellData(_T("IBS_초록유무"), _T("Y") , 0 );
	}

	m_pResourceMgr->GetMarcEditor()->Display();

	CString strInputStatus;
	m_pResourceMgr->m_dlgCode.m_pCM->GetControlMgrData(_T("입력자료상태"), strInputStatus);
	if ( strInputStatus == _T("소장자료") ) m_bCatHold = TRUE;
	else m_bCatHold = FALSE;
	
	if (!m_bCatHold) m_strWorkingStatus = _T("BOC113N");
	else m_strWorkingStatus = _T("BOL112N");

	m_pCatESLMgr->m_pDM_Index->SetCellData(_T("IBS_작업상태INFO"), m_strWorkingStatus, 0 );
	for ( INT i = 0 ; i < m_pCatESLMgr->m_pDM_Book->GetRowCount() ; i++ ) 
	{		
		m_pCatESLMgr->m_pDM_Book->SetCellData(_T("BB_자료상태"), m_strWorkingStatus, i );
	}

	ApplyMarcChange();
		
	return 0;

	EFS_END
	return -1;
}


INT CMarcAdjMgr_BO::CMD_VolumeInfo()
{
	EFS_BEGIN
	
	if (ApplyData() < 0) return -1;

	
	CBOVolumeDlg dlg(m_pCatESLMgr, m_pCatESLMgr->m_pDM_Book);

	if (GetManageMode() == MNG_IMPORT|| m_nManageMode == MNG_IMPORT_WORKING)
		dlg.SetOpenMode(MNG_MODIFY);
	else
		dlg.SetOpenMode(GetManageMode());

	if (dlg.DoModal() != IDOK) return -1;

	OnVolumeUpdate();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::CreateAcqCallNo()
{
EFS_BEGIN

	CString sClassNo, s090b;
	m_pMarcMgr->GetItem( m_pMarc, _T("090$a"), sClassNo );
	m_pMarcMgr->GetItem( m_pMarc, _T("090$b"), s090b );

	if (m_iSwAcqCallNo != 1 || sClassNo.IsEmpty() || !s090b.IsEmpty()) return -1;
		
	CString sShelfCode;
	CString sClassType;
	CString sAcqNo;
	CString sWhere;
	
	sClassType = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("IBS_분류표구분"), 0);
	if ( sClassType != _T("") ) {
		sWhere = _T("CLASS_NO_TYPE = '") + sClassType + _T("'");
	}
	
	if ( sClassNo != _T("") ) {
		if ( sWhere != _T("") ) {
			sWhere = sWhere + _T(" AND ");
		}
		sWhere = sWhere + _T("CLASS_NO = '") + sClassNo + _T("'");
	}
	m_pMarcMgr->GetItem( m_pMarc, _T("049$f"), sShelfCode );
	if ( sShelfCode != _T("") ) {
		if ( sWhere != _T("") ) {
			sWhere = sWhere + _T(" AND ");
		}
		sWhere = sWhere + _T("SEPARATE_SHELF_CODE = '") + sShelfCode + _T("'");
	}
	else{
		sWhere = sWhere + _T(" AND SEPARATE_SHELF_CODE IS NULL");
	}

	CString sQueryAcqNumber = _T("SELECT ACQUISIT_NO FROM CO_LAST_BOOK_ACQUISITNO_TBL ")
								_T("WHERE ") + sWhere + _T(" AND MANAGE_CODE=UDF_MANAGE_CODE");
	
	CString sAcqNumber;	
	INT nResult = m_pCatESLMgr->m_pDM_Book->GetOneValueQuery( sQueryAcqNumber , sAcqNumber );
	
	if ( sAcqNumber == _T("") ) 
		sAcqNumber = _T("0");
	else 
	{
		INT nAcqNumber = _ttoi ( sAcqNumber.GetBuffer(0) );
		sAcqNumber.Format(_T("%d"), nAcqNumber);
	}
	
	if ( m_nManageMode == MNG_MODIFY )
	{
		CBO_CAT_PROC_AcqNumber dlg(m_pCatESLMgr);
		INT nAcqNumber = _ttoi ( sAcqNumber) + 1;
		sAcqNumber.Format(_T("%d"), nAcqNumber);
		dlg.SetData( sShelfCode, sClassType, sClassNo, sAcqNumber, _T("자동 부여")/*, "수입순번호부여"*/);
		nAcqNumber = _ttoi ( sAcqNumber) - 1;
		sAcqNumber.Format(_T("%d"), nAcqNumber);
		
		if ( dlg.DoModal() == IDOK ) 
		{	
			if (nResult > 0)
			{
				CString strComp;
				INT iAcqNumber;
				BOOL bAcqNumberChange = FALSE;
				sQueryAcqNumber = sQueryAcqNumber;// + " FOR UPDATE";
				m_pCatESLMgr->m_pDM_Book->GetOneValueQuery( sQueryAcqNumber , strComp );
				if( strComp == sAcqNumber )
				{
					bAcqNumberChange = FALSE;
					iAcqNumber = _ttoi ( sAcqNumber.GetBuffer(0) );
					iAcqNumber++;
				}
				else 
				{
					bAcqNumberChange = TRUE;
					iAcqNumber = _ttoi ( strComp.GetBuffer(0) );
					iAcqNumber++;
					strComp.Format(sAcqNumber);
				}

				if ( sClassType == _T("9") )
				{
					sAcqNumber.Format( _T("%04d"), iAcqNumber );
				}
				else
				{
					sAcqNumber.Format( _T("%d"), iAcqNumber );
				}
				
				m_pMarcMgr->SetItem( m_pMarc, _T("090$b"), sAcqNumber );
				
				CString sQuery;
				
				sQuery.Format(	_T("UPDATE CO_LAST_BOOK_ACQUISITNO_TBL ")
								_T("SET ACQUISIT_NO='%s' WHERE %s AND MANAGE_CODE=UDF_MANAGE_CODE;") 
								, sAcqNumber, sWhere );
				
				m_arrStrAddFrame.Add( sQuery );		
				
				INT nTemp;
				nTemp = _ttoi(strComp.GetBuffer(0))+1;
				strComp.Format(_T("%d"),nTemp);	
				if( bAcqNumberChange == TRUE )
				{
					CString strMsg;
					strMsg.Format(_T("다른 PC에서 마지막번호를 변경하였습니다. [%s] -> [%s]"),strComp,sAcqNumber);
					AfxMessageBox(strMsg);
				}
			}
			else
			{
				CString sQuery;
				CString sAcqRecKey;
				if ( sClassType == _T("9") )
				{
					m_pMarcMgr->SetItem( m_pMarc, _T("090$b"), '0001' );
				}
				else
				{
					m_pMarcMgr->SetItem( m_pMarc, _T("090$b"), '1' );
				}
				
				INT ids = m_pCatESLMgr->m_pDM_Book->MakeRecKey( sAcqRecKey );
				if (ids < 0) return ids;
				sQuery.Format(	_T("INSERT INTO CO_LAST_BOOK_ACQUISITNO_TBL ")
								_T("(REC_KEY, CLASS_NO_TYPE, CLASS_NO, ACQUISIT_NO, SEPARATE_SHELF_CODE, ")
								_T("REG_CODE, MANAGE_CODE ) VALUES('%s','%s','%s','%s','%s','%s',UDF_MANAGE_CODE);")
								, sAcqRecKey, sClassType, sClassNo, _T("1"), sShelfCode, _T("") );
				m_arrStrAddFrame.Add(sQuery);
			}
		}
		else 
		{
			CString sAcqNumber;
			INT		OmitAcqNo;
			sAcqNumber = dlg.GetAcqNoData();
			OmitAcqNo = _ttoi(sAcqNumber);

			if ( _ttoi(sAcqNumber) <= 0)
			{
				AfxMessageBox(_T("청구기호를 생성할 수 없습니다."));
				return 100;
			}
			if ( sClassType == _T("9") ) 
			{
				sAcqNumber.Format( _T("%04d"), OmitAcqNo );
			}

			CString strQuery;
			CString strCnt;
			if( sShelfCode.IsEmpty() )
			{
				strQuery.Format(_T("SELECT COUNT(1) FROM BO_BOOK_TBL WHERE  MANAGE_CODE = UDF_MANAGE_CODE AND CLASS_NO_TYPE = '%s' AND CLASS_NO = '%s' AND SEPARATE_SHELF_CODE IS NULL AND BOOK_CODE = '%s'")
							, sClassType, sClassNo, sAcqNumber );
			}
			else
			{
				strQuery.Format(_T("SELECT COUNT(1) FROM BO_BOOK_TBL WHERE  MANAGE_CODE = UDF_MANAGE_CODE AND CLASS_NO_TYPE = '%s' AND CLASS_NO = '%s' AND SEPARATE_SHELF_CODE = '%s' AND BOOK_CODE = '%s'")
							, sClassType, sClassNo, sShelfCode, sAcqNumber );
			}
			m_pCatESLMgr->m_pDM_Index->GetOneValueQuery(strQuery,strCnt);
			if( _ttoi( strCnt ) > 0 )
			{
				AfxMessageBox(_T("수입순번호가 중복됩니다."));
				return 100;
			}
			
			m_pMarcMgr->SetItem( m_pMarc, _T("090$b"), sAcqNumber);	
			
			if( sShelfCode.IsEmpty() )
			{
				strQuery.Format(_T("DELETE FROM CO_OMIT_BOOK_ACQUISITNO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND CLASS_NO_TYPE = '%s' AND CLASS_NO = '%s' AND (SEPARATE_SHELF_CODE = '%s' OR SEPARATE_SHELF_CODE IS NULL) AND ACQUISIT_NO = %s ;")
							, sClassType, sClassNo, sShelfCode, sAcqNumber );
			}
			else 
			{
				strQuery.Format(_T("DELETE FROM CO_OMIT_BOOK_ACQUISITNO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND CLASS_NO_TYPE = '%s' AND CLASS_NO = '%s' AND SEPARATE_SHELF_CODE = '%s' AND ACQUISIT_NO = %s ;")
							, sClassType, sClassNo, sShelfCode, sAcqNumber );
			}

			m_arrStrAddFrame.Add(strQuery);
		}
	}
	else
	{
		if (nResult > 0)
		{
			INT iAcqNumber = _ttoi ( sAcqNumber.GetBuffer(0) );
			iAcqNumber++;
			
			if ( sClassType == _T("9") ) 
			{
				sAcqNumber.Format( _T("%04d"), iAcqNumber );
			}
			else
			{
				sAcqNumber.Format( _T("%d"), iAcqNumber );
			}
			m_pMarcMgr->SetItem( m_pMarc, _T("090$b"), sAcqNumber );
			
			CString sQuery;
			sQuery.Format(	_T("UPDATE CO_LAST_BOOK_ACQUISITNO_TBL ")
							_T("SET ACQUISIT_NO='%s' WHERE %s AND MANAGE_CODE=UDF_MANAGE_CODE;") 
							, sAcqNumber, sWhere );
			m_arrStrAddFrame.Add( sQuery );		
		}
		else
		{
			CString sQuery;
			CString sAcqRecKey;

			if ( sClassType == _T("9") ) 
			{
				m_pMarcMgr->SetItem( m_pMarc, _T("090$b"), '0001' );
			}
			else
			{
				m_pMarcMgr->SetItem( m_pMarc, _T("090$b"), '1' );				
			}
			INT ids = m_pCatESLMgr->m_pDM_Book->MakeRecKey( sAcqRecKey );
			if (ids < 0) return ids;
			
			sQuery.Format(	_T("INSERT INTO CO_LAST_BOOK_ACQUISITNO_TBL ")
							_T("(REC_KEY, CLASS_NO_TYPE, CLASS_NO, ACQUISIT_NO, ")
							_T("SEPARATE_SHELF_CODE, REG_CODE, MANAGE_CODE) ")
							_T("VALUES('%s','%s','%s','%s','%s','%s', UDF_MANAGE_CODE);")
							, sAcqRecKey, sClassType, sClassNo, _T("1"), sShelfCode, _T("") );
			
			m_arrStrAddFrame.Add(sQuery);
		}
	}
	m_pResourceMgr->GetMarcEditor()->Display();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::CreateCatESLMgr(CESL_Mgr *pParentESLMgr)
{
	EFS_BEGIN

	if (pParentESLMgr == NULL) return -1;

	if (m_pCatESLMgr)
	{
		if (m_pCatESLMgr->m_pDM_Book)
			m_pCatESLMgr->m_pDM_Book->FreeData();

		if (m_pCatESLMgr->m_pDM_Volume)
			m_pCatESLMgr->m_pDM_Volume->FreeData();

		if (m_pCatESLMgr->m_pDM_Donation)
			m_pCatESLMgr->m_pDM_Donation->FreeData();
		
		if (m_pCatESLMgr->m_pDM_Index)
			m_pCatESLMgr->m_pDM_Index->FreeData();

		if (m_pCatESLMgr->m_pDM_Purchase)
			m_pCatESLMgr->m_pDM_Purchase->FreeData();

		if (m_pCatESLMgr->m_pDM_HoldBook)
			m_pCatESLMgr->m_pDM_HoldBook->FreeData();

		if (m_pCatESLMgr->m_pDM_Species)
			m_pCatESLMgr->m_pDM_Species->FreeData();

		if (m_pCatESLMgr->m_pDM_Appendix)
			m_pCatESLMgr->m_pDM_Appendix->FreeData();

		if (m_pCatESLMgr->m_pDM_HoldAppendix)
			m_pCatESLMgr->m_pDM_HoldAppendix->FreeData();

		return 0;
	}
	
	m_pCatESLMgr = new CCatESL_Mgr(pParentESLMgr);
	if (m_pCatESLMgr->InitCatESL_Mgr(m_nManageMode, MODE_MONO) < 0) return -1;

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::CreateResource(CFrameWnd *pFrameWnd)
{
	EFS_BEGIN

	m_pFrameWnd = pFrameWnd;

	if (m_pCatESLMgr == NULL) return -1;
	m_pCatESLMgr->pMain = pFrameWnd;
	m_pCatESLMgr->Create(m_pFrameWnd);
	m_pCatESLMgr->ShowWindow(SW_HIDE);

	if (m_pResourceMgr)
		delete m_pResourceMgr;
	
	m_pResourceMgr = new CMarcAdjResourceMgr(m_pCatESLMgr);
	m_pResourceMgr->CreateResource(pFrameWnd);

	m_pResourceMgr->HideToolBarButton(_T("메모장"));

	m_pResourceMgr->HideToolBarButton(_T("소장정보"));

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pCatESLMgr->m_pDM_Book->CONNECTION_INFO);

	CString strQuery, strReturnValue;
	strQuery = _T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USECOVERIMG'");
	dm.GetOneValueQuery(strQuery, strReturnValue);
	if ( strReturnValue != _T("Y") )
		m_pResourceMgr->HideToolBarButton(_T("표지정보"));
	
	CString strTitle;
	if (m_nManageMode == MNG_INSERT)
	{
		strTitle = _T("MARC 정리 - [입력]");
		m_pResourceMgr->m_dlgCode.ApplyCodeChange();

		ApplyMarcChange();

		m_pResourceMgr->HideToolBarButton(_T("이전"));
		
		m_pResourceMgr->HideToolBarButton(_T("다음"));

		CString strMatCode = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("IBS_매체구분_코드"), 0);
		if(strMatCode == _T("EB"))
		{
			m_pResourceMgr->HideToolBarButton(_T("ebook보기"),TRUE);
		}
		else
		{
			m_pResourceMgr->HideToolBarButton(_T("ebook보기"));
		}
	}
	else if (m_nManageMode == MNG_MODIFY)
	{
		strTitle = _T("MARC 정리 - [수정]");

		ApplyDMChange(true);
		ApplyMarcChange();

		m_pResourceMgr->HideToolBarButton(_T("이전내용"));
		m_pResourceMgr->m_dlgCode.Display();

		if (IsFirstMarcAdjust())
		{
			CString strFormCode = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("IBS_형식구분_코드"), 0);
			ApplyDefaultMarc(m_pMarc, strFormCode);
		}

		CString strMarc;
		m_pMarcMgr->Encoding(m_pMarc, strMarc);
		
		CArray<CString, CString&> array202;
		CString sItem;
		m_pMarcMgr->GetField( m_pMarc, _T("020"), sItem, &array202 );
		m_pMarcMgr->DeleteField( m_pMarc, _T("020") );
		CString sGt, sTt;
		sGt.Format( _T("%c"), 31 );
		sTt.Format( _T("%c"), 30 );
		CString s020a, s020g;
		for ( INT i = 0 ; i < array202.GetSize() ; i++ ) {
			if ( array202.GetAt(i).Left(1) == _T(" ") ) {				
				if ( array202.GetAt(i).GetLength() != 5 ) {
					m_pMarcMgr->InsertField( m_pMarc, _T("020") + array202.GetAt(i) );
				}
			}
			else {								
				sItem = array202.GetAt(i);
				CString s020Ori = sItem;								
				INT iPos = sItem.Find( _T("a") );
				if ( iPos > -1 ) {
					sItem = sItem.Mid(iPos+1);
					iPos = sItem.Find( sGt );
					if ( iPos < 0 ) {
						iPos = sItem.Find( sTt );
					}
					if ( iPos != 0 ) {
						s020a = sItem.Left(iPos);
					}
				}
				sItem = s020Ori;
				iPos = sItem.Find( _T("g") );
				if ( iPos > -1 ) {
					sItem = sItem.Mid(iPos+1);
					iPos = sItem.Find( sGt );
					if ( iPos < 0 ) {
						iPos = sItem.Find( sTt );
					}
					if ( iPos != 0 ) {
						s020g = sItem.Left(iPos);
					}
				}
			}
		}
		CString s0201 = _T("0201 ");
		INT i020Yes = 0;
		if ( s020a != _T("") ) {
			CString s020aOri;
			s020aOri.Format( _T("%sa%s"), sGt, s020a );
			s0201 = s0201 + s020aOri;
			i020Yes = 1;
		}
		if ( s020g != _T("") ) {
			CString s020gOri;
			s020gOri.Format( _T("%sg%s"), sGt, s020g );
			s0201 = s0201 + s020gOri;
			i020Yes = 1;
		}
		if ( i020Yes == 1 ) {
			CString s0201Ori;
			s0201Ori.Format( _T("%s%s"), s0201, sTt );
			m_pMarcMgr->InsertField( m_pMarc, s0201Ori );
		}

		CString strMATClassCode;
        strMATClassCode= m_pCatESLMgr->m_pDM_Index->GetCellData(_T("IBS_매체구분_코드"),0) ; 
	
		if (strMATClassCode == _T("EB"))
		{
		m_pResourceMgr->HideToolBarButton (_T("ebook보기"),FALSE);
		}
		else 
		{
		m_pResourceMgr->HideToolBarButton (_T("ebook보기")) ;
		}
	}
	else if (m_nManageMode == MNG_IMPORT|| m_nManageMode == MNG_IMPORT_WORKING)
	{
		strTitle = _T("MARC 정리 - [파일반입]");
		ApplyDMChange();
		ApplyMarcChange();
		
		m_pResourceMgr->HideToolBarButton(_T("이전내용"));
		m_pResourceMgr->m_dlgCode.Display();

		CString strMATClassCode;
        strMATClassCode= m_pCatESLMgr->m_pDM_Index->GetCellData(_T("IBS_매체구분_코드"),0) ; 
	
		if (strMATClassCode == _T("EB"))
		{
		m_pResourceMgr->HideToolBarButton (_T("ebook보기"),FALSE);
		}
		else 
		{
		m_pResourceMgr->HideToolBarButton (_T("ebook보기")) ;
		}

		m_pResourceMgr->HideToolBarButton(_T("자동번호부여"));
	}
	
	if (m_strWorkingStatus == _T("BOC112O"))
	{
		m_pResourceMgr->HideToolBarButton(_T("고정길이(008)"));

		m_pResourceMgr->HideToolBarButton(_T("고정길이(006)"));		

		m_pResourceMgr->HideToolBarButton(_T("키워드생성"));

		m_pResourceMgr->HideToolBarButton(_T("부출지원"));

		m_pResourceMgr->HideToolBarButton(_T("자동번호부여"));

		m_pResourceMgr->HideToolBarButton(_T("등록번호부여"));

		m_pResourceMgr->DeleteMenuItem(_T("SUPPORT_별치기호"));

		m_pResourceMgr->HideToolBarButton(_T("목차초록관리"));
	}

	m_pFrameWnd->SetWindowText(strTitle);

	m_pResourceMgr->m_dlgCode.ApplyChangeAcqCode();

	CMarcToolBar *pToolBar = NULL;
	POSITION pos = m_pResourceMgr->m_listToolBar.GetHeadPosition();	
	pToolBar = static_cast<CMarcToolBar*>(m_pResourceMgr->m_listToolBar.GetNext(pos));	
	pToolBar->ToolBarItemSet();
	pToolBar->m_bSizingStart = TRUE;
	pToolBar->ToolBarSizing();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::Display(bool m_bInit /*= false*/)
{
	EFS_BEGIN

	CString strStreamMarc = m_pCatESLMgr->m_pDM_Species->GetCellData(_T("BS_MARC"), 0);
	m_pMarcMgr->Decoding(strStreamMarc, m_pMarc);

	m_pMarcMgr->GetItem(m_pMarc, _T("090$b"), m_str090bPrev);

	if (m_pResourceMgr->m_dlgCode.m_bFixedFieldMaintenance)
		ApplyFiexedFieldMaintenance();
	else{
		m_str008FieldPrev = _T("");
		m_str007FieldPrev = _T("");
		m_strLeaderPrev = _T("");
	}

	CString strWorkingStatus;
 	CString strTemp;
 	strWorkingStatus = m_pCatESLMgr->m_pDM_Index->GetCellData( _T("IBS_작업상태INFO"), 0);

 	strTemp = strWorkingStatus.Left(3);
	CComboBox* pCmb = (CComboBox*)m_pResourceMgr->m_dlgCode.GetDlgItem(IDC_cSPECIES_CLASS);
 	if( strTemp == _T("BOC") || strWorkingStatus == _T("BOL111O") )
 	{
		pCmb->SelectString(0,_T("정리자료"));
 	}
 	else 
 	{
		pCmb->SelectString(0,_T("소장자료"));		
 	}	
	m_strWorkingStatus = strWorkingStatus;

	m_pCatApi->CopyDM(m_pCatESLMgr->m_pDM_Species, 0, m_pCatESLMgr->m_pDM_Index, 0, _T("IBS"));

	if(m_bInit == false)
	{
		m_pCatApi->ConvertCatDMToMarc(m_pMarc, m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Purchase);
    }

	m_pCatApi->ConvertMarcToCatDM(m_pMarc, m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Purchase);

	m_pResourceMgr->m_dlgCode.Display();

	m_pResourceMgr->m_dlgVolume.Display();

	if (IsFirstMarcAdjust())
	{
		CString strFormCode = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("IBS_형식구분_코드"), 0);
		ApplyDefaultMarc(m_pMarc, strFormCode);
	}

	InitMarcEditor();
	m_pResourceMgr->GetMarcEditor()->Display();	

	BackupOldMarc();

	return 0;

	EFS_END
	return -1;
}
VOID CMarcAdjMgr_BO::BackupOldMarc()
{
	m_pResourceMgr->GetMarcEditor()->GetEditCtrl()->GetWindowText(m_oldEditMarc);
}

BOOL CMarcAdjMgr_BO::ISChangeMarc()
{
	CString strNewEditMarc;

	m_pResourceMgr->GetMarcEditor()->GetEditCtrl()->GetWindowText(strNewEditMarc);

	if ( strNewEditMarc == m_oldEditMarc ) return FALSE;	
	
	return TRUE;
}

INT CMarcAdjMgr_BO::InitMarcEditor()
{
	EFS_BEGIN

	m_pResourceMgr->m_MarcEditor.Init(m_pMarcMgr, m_pMarc, m_pCatESLMgr->m_pInfo);
	
	m_pResourceMgr->m_MarcEditor.InitLimitTag();
	m_pResourceMgr->m_MarcEditor.AddLimitTag(_T("020"), _T("  "));
	
	m_pResourceMgr->m_MarcEditor.InitUserAlias();

	CString strFormCode = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("IBS_형식구분_코드"), 0);

	CMarcDefaultValueConfigDlg dlg(m_pCatESLMgr);
	dlg.SetMarcGroupFormCode(strFormCode, m_pMarcMgr->GetMarcGroup());
	INT ids = -1;

	dlg.LoadDefaultUserMarcAlias();
	CStringArray *pStrArray = NULL;

	ids = dlg.LoadDefaultUserMarcAlias();
	if (ids < 0) return ids;

	ids = dlg.GetUserMarcAlias(dlg.GetGroup(), &pStrArray);
	if (ids >= 0)
	{
		INT nCount = pStrArray->GetSize();
		for (INT idx = 0; idx < nCount; idx++)
			m_pResourceMgr->m_MarcEditor.AddUserAlias(pStrArray->GetAt(idx));
	}
	
	return 0;

	EFS_END
	return -1;
}

BOOL CMarcAdjMgr_BO::IsFirstMarcAdjust()
{
	CString strApplyYN = m_pCatESLMgr->m_pDM_Species->GetCellData(_T("BS_정리된자료"), 0);
	if (strApplyYN == _T("Y")) return FALSE;

	strApplyYN = _T("Y");
	m_pCatESLMgr->m_pDM_Species->SetCellData(_T("BS_정리된자료"), strApplyYN, 0);
	
	return TRUE;
}

INT CMarcAdjMgr_BO::MakeImportDM()
{
	EFS_BEGIN

	if (m_nParentSelectedRow < 0) return -1;

	CString strData = m_pGrid_Parent->GetTextMatrix(m_nParentSelectedRow + 1 , 0);
	
	if (strData.IsEmpty()) return -1;

	INT idxDM = _ttoi(strData) - 1;

	INT ids = ((CCatApi_BO*)m_pCatApi)->MakeMarcAdjustDM(m_pDM_Species_Parent, idxDM, m_pDM_Index_Parent, idxDM, m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume);
	if (ids < 0) return -1;

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::MakeUpdateDM()
{
	EFS_BEGIN

	INT ids = -1;

	if (m_nParentSelectedRow < 0) return -1;

	CString strSpeciesKey;
	CString strPurchaseKey;

	m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, m_nParentSelectedRow, strSpeciesKey);

	CString strOption;

	if (strSpeciesKey.IsEmpty()) return -1;

	strOption.Format(_T("REC_KEY = %s"), strSpeciesKey);
	ids = m_pCatESLMgr->m_pDM_Index->RefreshDataManager(strOption);
	if (ids < 0) return ids;
	
	strOption.Format(_T("REC_KEY = %s"), strSpeciesKey);
	ids = m_pCatESLMgr->m_pDM_Species->RefreshDataManager(strOption);
	if (ids < 0) return ids;

	if (m_strBookWhere.IsEmpty())
		m_strBookWhere = _T("1 = 1");

	strOption.Format(_T("(B.REC_KEY = E.BOOK_KEY(+) AND (B.SPECIES_KEY = %s OR (B.SPECIES_KEY = %s AND B.REPRESENT_BOOK_YN = 'Y'))) AND %s AND BOOK_APPENDIX_FLAG = 'B' ORDER BY B.VOL_SORT_NO, B.REG_NO"), strSpeciesKey, strSpeciesKey, m_strBookWhere);

	if (m_strBookWhere.Find(_T("A.")) >= 0)
	{
		m_pCatESLMgr->m_pDM_Book->TBL_NAME += _T(" , CO_ACCESSION_REC_TBL A ");
	}
	if (m_strBookWhere.Find(_T("P.")) >= 0)
	{
		m_pCatESLMgr->m_pDM_Book->TBL_NAME += _T(" , BO_PURCHASE_SEQ_NO_TBL P ");
	}

    CBOFormApi api(m_pCatESLMgr) ;
    
	INT iiii = m_pCatESLMgr->m_pDM_Book->GetRowCount();
	api.MakeBookDM(strOption,m_pCatESLMgr->m_pDM_Book) ;
	iiii = m_pCatESLMgr->m_pDM_Book->GetRowCount();

	CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
	BOOL    bFind;

	strOption.Format(_T("SPECIES_KEY = %s AND BOOK_APPENDIX_FLAG = 'A' ORDER BY REG_NO"), strSpeciesKey);
	INT nColumnCnt = m_pCatESLMgr->m_pDM_Appendix->RefList.GetCount();	

	bFind = FALSE;
	for( INT i = 0 ; i < nColumnCnt ; i++ )
	{
		sFieldAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
		ids = m_pCatESLMgr->m_pDM_Appendix->GetColumnRefInfo( i , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
		if( ids < 0 ) return ids;
		
		if( sFieldAlias.Compare( _T("UDF_원부번호") ) == 0 ) 
		{	
			ids = m_pCatESLMgr->m_pDM_Appendix->SetDataMgrRef( i , sFieldAlias , sFieldName , sFieldType  , sInitValue  , _T("") );			
			if( ids < 0 ) return ids;
			bFind = TRUE;
			break;
		}
	}

	ids = m_pCatESLMgr->m_pDM_Appendix->RefreshDataManager(strOption);
	if (ids < 0) return ids;

	if ( bFind == TRUE )
	{
		ids = m_pCatESLMgr->m_pDM_Appendix->SetDataMgrRef( i , sFieldAlias , sFieldName , sFieldType  , sInitValue  , _T("udf") );
	}

	m_pCatApi->CopyDM(m_pCatESLMgr->m_pDM_Species, 0, m_pCatESLMgr->m_pDM_Index, 0, _T("IBS"));

	INT nIdx;
	m_pCatApi->GetRepresentationBook(m_pCatESLMgr->m_pDM_Book, _T("BB"), nIdx);
	if (nIdx < 0)
		m_pCatApi->SetRepresentationBook(m_pCatESLMgr->m_pDM_Book, _T("BB"));
	
	m_pCatApi->CopyDM(m_pCatESLMgr->m_pDM_Book, nIdx, m_pCatESLMgr->m_pDM_Index, 0, _T("IBS"));

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::PrevDataCopy()
{
	EFS_BEGIN

	INT nRowCount;
	INT	i;
	INT	ids;

	nRowCount = m_pDM_Parent->GetRowCount();
	if ( nRowCount <= 0 ) return 0;
	
	CString strStreamMarc = m_pDM_Parent->GetCellData(_T("BS_MARC"), nRowCount-1);
	ids = m_pMarcMgr->Decoding(strStreamMarc, m_pMarc);
	if ( ids ) return ids;
	ids = m_pMarcMgr->DeleteField(m_pMarc, _T("001"));
	if ( ids ) return ids;
	ids = m_pMarcMgr->DeleteField(m_pMarc, _T("020"));
	if ( ids ) return ids;
	ids = m_pMarcMgr->DeleteField(m_pMarc, _T("049"));
	if ( ids ) return ids;

	const INT nCtlCnt = 9;
	CString strPrevDMCodeAlias[nCtlCnt] = {_T("IBS_자료구분_코드"), _T("IBS_형식구분_코드"), _T("IBS_매체구분_코드"), 
		_T("IBS_보조등록구분_코드"), _T("IBS_이용대상구분_코드"), _T("IBS_이용제한구분_코드"), _T("IBS_관리구분_코드"),
		_T("IBS_분류표구분_코드"), _T("BS_제어자료구분_코드") };
	CString strPrevCMCodeAlias[nCtlCnt] = {_T("IBS_자료구분_코드"), _T("IBS_형식구분_코드"), _T("IBS_매체구분_코드"), 
		_T("IBS_보조등록구분_코드"), _T("IBS_이용대상구분_코드"), _T("IBS_이용제한구분_코드"), _T("IBS_관리구분_코드"),
		_T("IBS_분류표구분"), _T("BS_제어자료구분_코드") };
	CString strPrevCodeData[nCtlCnt];

    m_pDM_Parent->GetCellData(strPrevDMCodeAlias, nCtlCnt, strPrevCodeData, nRowCount-1);

	for ( i=0 ; i<nCtlCnt ; i++ )
	{
		ids = m_pResourceMgr->m_dlgCode.m_pCM->SetControlMgrData(strPrevCMCodeAlias[i],strPrevCodeData[i], -1 ); 
		if ( ids ) return ids;
	}		

	InitMarcEditor();
	m_pResourceMgr->GetMarcEditor()->Display();

	return 0;

	EFS_END
	return -1;
}


UINT CMarcAdjMgr_BO::OnCodeUpdate()
{
	EFS_BEGIN

	INT idx = -1;

	if (m_pCatApi->GetRepresentationBook(m_pCatESLMgr->m_pDM_Book, _T("BB"), idx) < 0)
		idx = m_pCatApi->SetRepresentationBook(m_pCatESLMgr->m_pDM_Book, _T("BB"));

	CESL_DataMgr* m_pBooks = m_pCatESLMgr->m_pDM_Book;
	int rwcnt = m_pBooks->GetRowCount();
	if (rwcnt > 1)
	{	
		CString strULC = m_pCatESLMgr->m_pDM_Species->GetCellData(_T("BS_이용제한구분_코드"),0);
		for ( int i = 0 ; i < rwcnt ; i++ )
		{
			m_pBooks->SetCellData(_T("BB_이용제한구분_코드"), strULC ,i);
		}
	}

	m_pCatApi->MakeDMFromCM(m_pResourceMgr->m_dlgCode.m_pCM, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->GetAppMode(), idx, _T("BB"));	

	ApplyDMChange(true);


	m_pResourceMgr->GetMarcEditor()->Display();
	m_pResourceMgr->m_dlgVolume.Display();

	return 0;

	EFS_END
	return -1;
}

BOOL CMarcAdjMgr_BO::OnCommand(UINT nID)
{
	EFS_BEGIN

	if (m_pFrameWnd == NULL) return FALSE;

	m_bAcceptMessage = FALSE;

	CString strCommandAlias;

	BOOL bReturn = m_pResourceMgr->OnCommand(nID, strCommandAlias);

	if (strCommandAlias.IsEmpty()) return bReturn;

	m_pResourceMgr->GetMarcEditor()->GetGridCtrl()->SendMessage(WM_KEYDOWN, VK_DOWN);
	m_pResourceMgr->GetMarcEditor()->ApplyEdit();
	m_pCatApi->ConvertMarcToCatDM(m_pMarc, m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Purchase);

	CString strControlMatCode;
	m_pResourceMgr->m_dlgCode.m_pCM->GetControlMgrData(_T("BS_제어자료구분_코드"), strControlMatCode);
	m_pCatESLMgr->m_pDM_Species->SetCellData(_T("BS_제어자료구분_코드"),strControlMatCode,0);

	INT ids = -1;

	if (strCommandAlias == _T("권별정보"))
		ids = CMD_VolumeInfo();
	else if (strCommandAlias == _T("부록정보"))
		ids = CMD_AppendixInfo();
	else if (strCommandAlias == _T("기본값"))
		ids = CMD_SetDefaultValue();
	else if (strCommandAlias == _T("고정길이(006)"))
		ids = CMD_EditFixedField2();
	else if (strCommandAlias == _T("고정길이(008)"))
		ids = CMD_EditFixedField();
	else if (strCommandAlias == _T("부출지원"))
		ids = CMD_Buchul();
	else if (strCommandAlias == _T("복본조사"))
		ids = CMD_DoDupSearch();
	else if (strCommandAlias == _T("키워드"))
		ids = CMD_CreateKeyWord();
	else if (strCommandAlias == _T("청구기호조사"))
		ids = CMD_SearchCallNo();
	else if (strCommandAlias == _T("청구기호생성"))
	{
		ids = CMD_CreateCallNo();
		m_pResourceMgr->GetMarcEditor()->Display();
	}
	else if (strCommandAlias == _T("MARC반입"))
		ids = CMD_MarcImport();
	else if (strCommandAlias == _T("분류지원시스템"))
		ids = CMD_SupportClasification();
	else if (strCommandAlias == _T("Web_소장_반입"))
		ids = CMD_WehHoldImport();
	else if (strCommandAlias == _T("국가자료공동목록반입"))
		ids = CMD_UCImport();
	else if (strCommandAlias == _T("학위논문반입"))
		ids = CMD_DMDPImport();
	else if (strCommandAlias == _T("SUPPORT_별치기호"))
		ids = CMD_ShelfCodeSearch();
	else if (strCommandAlias == _T("SUPPORT_수입순번호"))
		ids = CMD_AcqusitNo();
	else if (strCommandAlias == _T("SUPPORT_저자기호"))
		ids = CMD_AuthorCodeSearch();
	else if (strCommandAlias == _T("SUPPORT_역할어"))
		ids = CMD_RoleWord();
	else if (strCommandAlias == _T("SUPPORT_분류기호"))
		ids = CMD_ClassCode();
	else if (strCommandAlias == _T("SUPPORT_주제명검색"))
		ids = CMD_SubjectIndex() ;
	else if (strCommandAlias == _T("자동부여"))
		ids = CMD_GiveRegNoAuto();
	else if (strCommandAlias == _T("누락부여"))
		ids = CMD_GiveRegNo();
	else if (strCommandAlias == _T("목차초록관리"))
		ids = CMD_ContentsMgr();
	else if (strCommandAlias == _T("소장정보"))
		ids = CMD_HoldInfo();
	else if (strCommandAlias == _T("메모장"))
		ids = CMD_Memo();
	else if (strCommandAlias == _T("표지정보"))
		ids = CMD_UseCoverIMG();
	else if (strCommandAlias == _T("이전내용"))
		ids = CMD_PrevDataCopy();
	else if (strCommandAlias == _T("이전"))
		ids = CMD_Prev();
	else if (strCommandAlias == _T("다음"))
		ids = CMD_Next();
	else if (strCommandAlias == _T("저장"))
		ids = CMD_Save();
	else if (strCommandAlias == _T("SYS_CLOSE"))
		ids = CMD_Close();
    else if (strCommandAlias == _T("ebook보기"))
		ids = CMD_EbookView() ;	

	m_bAcceptMessage = TRUE;

	if (ids < 0)
		return FALSE;
	else
		return TRUE;

	EFS_END
	return FALSE;
}

INT CMarcAdjMgr_BO::CMD_AcqusitNo()
{
	EFS_BEGIN

	CBO_CAT_ACQUISIT_NO dlg(1, m_pCatESLMgr);
	
	CString strTmpCode;
	CString sTmpClassNo, sTmpShelfCode;

	m_pResourceMgr->m_dlgCode.m_pCM->GetControlMgrData( _T("IBS_분류표구분"), strTmpCode );

	if ( strTmpCode == _T("1") ) 
		m_pMarcMgr->GetItem (m_pMarc ,_T("056$a"), sTmpClassNo  );
	else if ( strTmpCode == _T("2") ) 
		m_pMarcMgr->GetItem (m_pMarc ,_T("082$a"), sTmpClassNo  );
	
	sTmpShelfCode = m_pCatESLMgr->m_pDM_Book->GetCellData(_T("BB_별치기호_코드"), 0);
	dlg.SetClassNo ( sTmpClassNo, sTmpShelfCode );

	dlg.DoModal();

	return 0;

	EFS_END
	return -1;
}

UINT CMarcAdjMgr_BO::OnMarcEditUpdate()
{
	EFS_BEGIN

	CArray<CString,CString&> arrRegNo ;
	CString strItemData;
	INT iRegNoCount = 0;
	
	iOnMarcEditUpdateCheckCount++;

    mapCheckRegNo.RemoveAll();
	
	m_pMarcMgr->GetItem (m_pMarc ,_T("049$l"),strItemData,&arrRegNo);

	INT nRegCodeLength;
	nRegCodeLength = m_pCatESLMgr->m_pInfo->nREG_CODE_LENGTH;
	
	for(INT i = 0 ;i <arrRegNo.GetSize ();i++)
	{
		CString sRegNo = arrRegNo.GetAt (i);
		if ( sRegNo != _T("") && sRegNo.GetLength() < 12 ) {
			if ( sRegNo.GetLength() > nRegCodeLength ) {
				CString sRight;
				sRegNo = m_pCatESLMgr->m_pInfo->MakeRegNo(sRegNo.Left(nRegCodeLength),sRegNo.Mid(nRegCodeLength));
				sRight = sRegNo.Mid(nRegCodeLength) ;
			}
		}

		sRegNo.TrimRight();

		mapCheckRegNo.SetAt( sRegNo, sRegNo );
		if ( arrRegNo.GetAt(i) != _T("") ) {
			iRegNoCount++;
		}
	}
	INT iMapCount = mapCheckRegNo.GetCount();

    INT iBookCount = m_pCatESLMgr->m_pDM_Book->GetRowCount();
	INT iDmBookCount = 0;
	for ( i = 0 ; i < iBookCount ; i++ ) {
		CString sRegNo = m_pCatESLMgr->m_pDM_Book->GetCellData( _T("BB_등록번호"), i );
		if ( sRegNo != _T("") ) {
			CString sDbFlag = m_pCatESLMgr->m_pDM_Book->GetCellData( _T("BB_DB_FLAG"), i );
			if ( sDbFlag != _T("D") ) {
				iDmBookCount++;
			}
		}
	}
	
	if ( iDmBookCount != iMapCount ) {
		if ( iOnMarcEditUpdateCheckCount != 2 || m_bOnMarcEditUpdateCheck == FALSE ) {
			AfxMessageBox( _T("마크의 등록번호 개수가 틀립니다.") );
		}
		return -1;
	}
	
	for ( i = 0 ; i < iRegNoCount ; i++ ) 
	{
		CString sRegNo = m_pCatESLMgr->m_pDM_Book->GetCellData( _T("BB_등록번호"), i );
		if ( sRegNo != _T("") && sRegNo.GetLength() < 12 ) 
		{
			INT nRegCodeLength;
			nRegCodeLength =  m_pCatESLMgr->m_pInfo->nREG_CODE_LENGTH;
			if ( sRegNo.GetLength() > nRegCodeLength ) 
			{
				CString sRight;
				sRegNo =  m_pCatESLMgr->m_pInfo->MakeRegNo(sRegNo.Left(nRegCodeLength),sRegNo.Mid(nRegCodeLength));
				sRight = sRegNo.Mid(nRegCodeLength);
			}
		}
		if ( sRegNo != _T("") ) 
		{
			CString sDbFlag = m_pCatESLMgr->m_pDM_Book->GetCellData( _T("BB_DB_FLAG"), i );
			if ( sDbFlag == _T("D") ) 
			{
				continue;
			}
		}
		CString strUniqueKey;
		BOOL bCheck = mapCheckRegNo.Lookup(sRegNo, strUniqueKey);
		if ( bCheck == FALSE && sRegNo != _T("")  ) 
		{
			if ( iOnMarcEditUpdateCheckCount != 2 || m_bOnMarcEditUpdateCheck == FALSE ) 
			{
				AfxMessageBox( _T("등록번호(049$l)는 마크화면에서 수정할 수 없습니다.") );			
			}
			m_bOnMarcEditUpdateCheck = FALSE;
 			return -1;
		}
	}

	if (m_bAcceptMessage)
		return ApplyMarcChange();
	else
		return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_BO::CMD_UseCoverIMG()
{	
	TCHAR szSpeciesKey[32];
	szSpeciesKey[0] = _T('\0');

	CCOVER_MAIN dlg(m_pCatESLMgr);
	dlg.SetPubType(_MONO_);
	dlg.SetLocalPath ( _T("..\\bmp\\cover_info\\") );
	
	CString strRecKey;
	strRecKey = m_pCatESLMgr->m_pDM_Species->GetCellData(_T("BS_종KEY"), 0 );
	_tcscpy ( szSpeciesKey , strRecKey.GetBuffer(0) );

	if ( m_nManageMode == MNG_INSERT )
	{
		dlg.SetInsertMode(TRUE);
		dlg.SetBasicName ( szSpeciesKey );
		dlg.SetSpeciesKey ( szSpeciesKey );
		
		if ( dlg.DoModal() == IDOK )
		{
			CString strSpeciesKey = _T("");
			dlg.GetSpeciesKey ( strSpeciesKey );
			m_pCatESLMgr->m_pDM_Species->SetCellData(_T("BS_종KEY"), strSpeciesKey , 0 );
		}
	}
	else if ( m_nManageMode == MNG_MODIFY )
	{
		dlg.SetInsertMode(FALSE);
		dlg.SetBasicName ( szSpeciesKey );
		dlg.SetSpeciesKey ( szSpeciesKey );
		dlg.DoModal();
	}

	return 0;
}

BOOL CMarcAdjMgr_BO::CMD_SubjectIndex()
{
	CMarcEditor *pMarcEditor = m_pResourceMgr->GetMarcEditor();

	CCSS_2_01 dlg(m_pCatESLMgr);
	dlg.SetMarcInfo(m_pMarcMgr, m_pMarc);

	if ( dlg.DoModal() == IDOK )
		pMarcEditor->Display();

	return TRUE;
}

CString CMarcAdjMgr_BO::MakeCallNo()
{
	CString strDmData;
	CString CallNo;

	CESL_DataMgr *pDM_SPECIES = m_pCatESLMgr->FindDM(_T("DM_단행_정리DM_색인정보"));
	if (pDM_SPECIES == NULL) {
		pDM_SPECIES = m_pCatESLMgr->FindDM(_T("DM_연속_정리DM_색인정보"));
		if ( pDM_SPECIES == NULL ) return -1;
	}
	strDmData = pDM_SPECIES->GetCellData( _T("IBS_별치기호_코드"), 0 );
	CallNo = strDmData;

	strDmData = pDM_SPECIES->GetCellData( _T("IBS_분류기호"), 0 );

	if( !strDmData.IsEmpty() )
	{
		CallNo += _T(" ") + strDmData;
	}
	
	strDmData = pDM_SPECIES->GetCellData( _T("IBS_도서기호"), 0 );
	if( !strDmData.IsEmpty() )
	{
		CallNo += _T("-") + strDmData;
	}

	strDmData = pDM_SPECIES->GetCellData( _T("IBS_권책기호"), 0 );
	if( !strDmData.IsEmpty() )
	{
		CallNo += _T("-") + strDmData;
	}

	CallNo.TrimLeft(); CallNo.TrimRight(); 

	return CallNo;
}
