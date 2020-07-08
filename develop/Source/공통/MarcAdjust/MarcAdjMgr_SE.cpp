// MarcAdjMgr_SE.cpp: implementation of the CMarcAdjMgr_SE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MarcAdjMgr_SE.h"

#include "..\..\관리\분류기호\MNG_CLASS_SIGN_POPUP\MNG_CLASS_SIGN_POPUP_01.h"

#include "../K2UP_Common/MakeSearchData.h"

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

#include "BO_CAT_AUTO_REGNO_GIVE.h"

#include "HoldInfoDlg.h"

#include "..\..\공통\BO_MARC_IMPORT\MARC_IMPORT.h"

#include "..\..\단행\정리\BO_CAT_PROC\BO_CAT_PROC_0380.h"

#include "..\..\ESL\Marc\MarcToolKit\MarcDefaultValueConfigDlg.h"

#include "..\..\단행\정리\BO_CAT_PROC\BO_CAT_PROC_0330.h"

#include "..\..\공동목록\Light_GatherDataSearch\UC_MARC_IMPORT.h"

#include "..\..\연속\정리\SE_VOLUME_FORM\SEVolumeDlg.h"

#include "..\..\단행\정리\BO_CAT_PROC\BO_CAT_PROC_AcqNumber.h"

#include "..\..\단행\정리\BO_CAT_NUMBER\BO_CAT_ACQUISIT_NO.h"

#include "..\CAT_UNION_FIXED_FIELD\DLG_006.h"

#include "..\..\연속\정리\SE_CAT_REGNO_DLG\SE_CAT_REGNO_DLG1.h"

#include "..\\MarcAdjustApi\\CatProgramMode.h"

#include "..\..\공통\SUBJECT_INDEX_SEARCH\CSS_2_01.h"

#include "MarcToolBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarcAdjMgr_SE::CMarcAdjMgr_SE() : CMarcAdjMgr()
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

	m_nModeMONO_SERIAL = MODE_SERIAL;

	m_bCatHold = FALSE;

	m_pCreateAuthorCode = NULL;

}

CMarcAdjMgr_SE::~CMarcAdjMgr_SE()
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

INT CMarcAdjMgr_SE::InitManager(INT nManageMode, CESL_Mgr *pParentESLMgr, LPCTSTR lpszWorkingStatus /*= NULL*/, CMarc *pMarc /*= NULL*/)
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

	SetManageMode(nManageMode);

	m_pMarcMgr = pParentESLMgr->m_pInfo->pMarcMgr;
	
	m_pMarcMgr->SetMarcGroup(_T("통합마크"));
	
	if (m_pMarc)
		delete m_pMarc;
	m_pMarc = new CMarc;
	
	if (CreateCatESLMgr(pParentESLMgr) < 0) return -1;

	m_pCatApi = m_pCatESLMgr->GetCatApi();
	if (m_pCatApi == NULL) return -1;

	if (lpszWorkingStatus == NULL)
	{
		if (!m_bCatHold)
			m_strWorkingStatus = _T("SEC111N");
		else
			m_strWorkingStatus = _T("SEL212N");
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
		m_pCatESLMgr->m_pDM_Purchase->InsertRow(-1);
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

		CString strStreamMarc = m_pCatESLMgr->m_pDM_Species->GetCellData(_T("SS_MARC"), 0);
		m_pMarcMgr->Decoding(strStreamMarc, m_pMarc);

		m_pMarcMgr->GetItem(m_pMarc, _T("090$b"), m_str090bPrev);
	}	
	else if (m_nManageMode == MNG_IMPORT)
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

		CString strStreamMarc = m_pCatESLMgr->m_pDM_Species->GetCellData(_T("SS_MARC"), 0);
		m_pMarcMgr->Decoding(strStreamMarc, m_pMarc);

		m_pMarcMgr->GetItem(m_pMarc, _T("090$b"), m_str090bPrev);
	}
	
	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::ApplyData()
{
	EFS_BEGIN

	INT nRowCount = m_pCatESLMgr->m_pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if (!((CCatApi_SE*)m_pCatApi)->IsBook(m_pCatESLMgr->m_pDM_Book, idx)) continue;
		if (m_pCatESLMgr->m_pDM_Book->GetCellData(_T("SB_DB_FLAG"), idx) != _T("I")) continue;
		
		m_pCatESLMgr->m_pDM_Book->SetCellData(_T("SB_자료상태"), m_strWorkingStatus, idx);
	}

    nRowCount = m_pCatESLMgr->m_pDM_Appendix->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		if (m_pCatESLMgr->m_pDM_Appendix->GetCellData(_T("SB_DB_FLAG"), idx) != _T("I")) continue;

		CString strWS = m_pCatESLMgr->m_pDM_Appendix->GetCellData(_T("SB_자료상태"), idx);
		if ( strWS == _T("SEL212N") ) continue;

		m_pCatESLMgr->m_pDM_Appendix->SetCellData(_T("SB_자료상태"), m_strWorkingStatus, idx);
	}

	return 0;

	EFS_END
	return -1;
}

UINT CMarcAdjMgr_SE::OnVolumeUpdate()
{
	EFS_BEGIN

	ApplyDMChange(false);
    ApplyMarcChange ();

	m_pResourceMgr->m_dlgCode.Display();
	m_pResourceMgr->m_dlgVolume.Display();
	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::ApplyDMChange(bool m_bInit /*=false */)
{
	EFS_BEGIN

	((CCatApi_SE*)m_pCatApi)->CopyDM(m_pCatESLMgr->m_pDM_Index, 0, m_pCatESLMgr->m_pDM_Species, 0, _T("SS"));
	

	CString sTmpShelfCode;
	m_pCatESLMgr->m_pDM_Index->GetCellData(_T("ISS_배가위치부호"), 0, sTmpShelfCode);
	sTmpShelfCode.TrimLeft();
	sTmpShelfCode.TrimRight();

	m_pCatESLMgr->m_pDM_Index->SetCellData(_T("ISS_배가위치부호"), sTmpShelfCode, 0);

	if(m_bInit == false)
	{
		((CCatApi_SE*)m_pCatApi)->ConvertCatDMToMarc(m_pMarc, m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Purchase);
	}

	m_pResourceMgr->GetMarcEditor()->Display();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::ApplyMarcChange()
{
	EFS_BEGIN

	((CCatApi_SE*)m_pCatApi)->ConvertMarcToCatDM(m_pMarc, m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Purchase);
	((CCatApi_SE*)m_pCatApi)->CopyDM(m_pCatESLMgr->m_pDM_Index, 0, m_pCatESLMgr->m_pDM_Species, 0, _T("SS"));
	m_pResourceMgr->m_dlgVolume.Display();

    long nRowLocation;
	long nColLocation;

	if(m_pResourceMgr->GetMarcEditor()->GetGridCtrl () != NULL)
	{
		nRowLocation = m_pResourceMgr->GetMarcEditor()->GetGridCtrl ()->GetRow () ;
		nColLocation = m_pResourceMgr->GetMarcEditor()->GetGridCtrl ()->GetCol () ;
	}
	m_pResourceMgr->GetMarcEditor()->Display();
   
	if(m_pResourceMgr->GetMarcEditor()->GetGridCtrl () != NULL)
	{
		m_pResourceMgr->GetMarcEditor()->GetGridCtrl ()->SetRow (nRowLocation)  ;
		m_pResourceMgr->GetMarcEditor()->GetGridCtrl ()->SetCol(nColLocation) ;
    }
    
	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::ApplyMarcData()
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

	BOOL bEssiontialTagError = FALSE;

	CArray<CString, CString&> arrItem;

	CStringArray arrStrEssentialTag;
	arrStrEssentialTag.Add(_T("008"));
	arrStrEssentialTag.Add(_T("090"));
	arrStrEssentialTag.Add(_T("245"));
	arrStrEssentialTag.Add(_T("260"));
	CString sRegCode;	
	m_pResourceMgr->m_dlgCode.m_pCM->GetControlMgrData( _T("ISS_형식구분_코드"), sRegCode );
	if ( sRegCode != _T("CF") ) {
		arrStrEssentialTag.Add(_T("300"));
	}

	arrStrEssentialTag.Add(_T("310"));
	arrStrEssentialTag.Add(_T("980"));

	if (!m_pResourceMgr->GetMarcEditor()->GetEditCtrl()->CheckFullSyntax(TRUE))
	{
		AfxMessageBox(_T("마크에 오류가 있습니다. 수정해주세요"));
		return -1;
	}

	CString sItem;
	m_pMarcMgr->GetField( m_pMarc, _T("100"), sItem );
	if ( sItem == _T("") ) {
		m_pMarcMgr->GetField( m_pMarc, _T("111"), sItem );
		if ( sItem == _T("") ) {
			m_pMarcMgr->GetField( m_pMarc, _T("110"), sItem );
			if ( sItem == _T("") ) {
				m_pMarcMgr->GetField( m_pMarc, _T("130"), sItem );
				if ( sItem == _T("") ) {
					m_pMarcMgr->GetField( m_pMarc, _T("700"), sItem );
					if ( sItem == _T("") ) {
						m_pMarcMgr->GetField( m_pMarc, _T("710"), sItem );
						if ( sItem == _T("") ) {
							m_pMarcMgr->GetField( m_pMarc, _T("711"), sItem );
							if ( sItem == _T("") ) {
								m_pMarcMgr->CheckDeepSyntax(m_pMarc);
							}
						}
					}
				}
			}
		}
	}

	if (m_pCatESLMgr->m_pDM_Index->GetCellData(_T("ISS_종구분값"), 0) == _T("1"))
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
			((CCatApi_SE*)m_pCatApi)->CompleteCataloging(&dm, m_pCatESLMgr->m_pDM_Species, 0, NULL, FALSE
											, m_pCatESLMgr->m_pDM_Index, m_pMarc);

			ids = dm.SendFrame();
			if ( 0 > ids ) return -1;
			ids = dm.EndFrame();
			if ( 0 > ids ) return -1;

			CString str;
			m_pMarcMgr->GetItem(m_pMarc, _T("001*"), str);
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
				AfxMessageBox(_T("별치기호 값이 잘못됬습니다. 수정해주세요"));
				return -1;
			}
		}
	}

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("950$b"), strItemData);
	if (ids >= 0)
	{
		strItemData = ((CCatApi_SE*)m_pCatApi)->GetPriceCode(strItemData, strTemp);
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

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("653$a"), strItemData);
	if (ids < 0)
	{
		CMD_CreateKeyWord();
	}

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

	if (m_pMarc->FindTag(_T("130")) && m_pMarc->FindTag(_T("240")))
	{
		AfxMessageBox(_T("130 TAG 과 240 TAG은 같이 쓸 수 없습니다."));
		return -1;
	}

	m_pMarcMgr->DeleteField(m_pMarc, _T("049"));

	strItem.Format(_T("%04d%02d%02d%02d%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	m_pMarcMgr->DeleteField(m_pMarc, _T("005"));
	m_pMarcMgr->InsertField(m_pMarc, _T("005") + strItem);

	ids = m_pMarcMgr->DeleteEmptySubfield(m_pMarc);
	
	ids = m_pMarcMgr->DeleteEmptyField(m_pMarc);	
	
	if (((CCatApi_SE*)m_pCatApi)->ApplyLastModifyData(m_pMarc) < 0)
	{
		strErrMsg = ((CCatApi_SE*)m_pCatApi)->GetLastError();
		AfxMessageBox ( strErrMsg );
		return -1;
	}

	return 0;

EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::CMD_ClassCode()
{
	EFS_BEGIN

	CMNG_CLASS_SIGN_POPUP_01 dlg(m_pCatESLMgr);
	if (IDOK != dlg.DoModal()) return -1;

	CString n056 = _T("-1");
	CString n082 = _T("-1");
	CString n085 = _T("-1");
		
	if (dlg.m_KDCtag != _T("") && dlg.m_KDCtag != _T(".")) 
	{
		INT iLen = dlg.m_KDCtag.GetLength();
		if (iLen > 1) 
		{
			CString strItemData = dlg.m_KDCtag.Mid(7 , dlg.m_KDCtag.GetLength() - 8);
			m_pMarcMgr->SetItem(m_pMarc, _T("056$a"), strItemData , _T(""), ADD_SUBFIELD);
			m_pMarcMgr->GetItem(m_pMarc, _T("056$2"), strItemData );
			if ( strItemData.IsEmpty() ) {
				
				((CCatApi_SE*)m_pCatApi)->LoadClassCodeVolumeDlg(_T("..\\cfg\\분류기호판사항.txt"), n056, n082, n085);
				if (n056 != _T("-1"))
				{
					strItemData.Format(_T("%s"), n056);
					m_pMarcMgr->SetItem(m_pMarc, _T("056$2"), strItemData, _T(""), ADD_SUBFIELD);
				}
			}
		}
	}
	
	if (dlg.m_DDCtag != _T("")) 
	{
		INT iLen = dlg.m_DDCtag.GetLength();
		if (iLen > 1) 
		{
			CString strItemData = dlg.m_DDCtag.Mid(7 , dlg.m_DDCtag.GetLength() - 8);
			m_pMarcMgr->SetItem(m_pMarc, _T("082$a"), strItemData , _T(""), ADD_SUBFIELD);
			m_pMarcMgr->GetItem(m_pMarc, _T("082$2"), strItemData );
			if ( strItemData.IsEmpty() ) {
				
				((CCatApi_SE*)m_pCatApi)->LoadClassCodeVolumeDlg(_T("..\\cfg\\분류기호판사항.txt"), n056, n082, n085);
				if (n082 != _T("-1"))
				{
					strItemData.Format(_T("%s"), n082);
					m_pMarcMgr->SetItem(m_pMarc, _T("082$2"), strItemData, _T(""), ADD_SUBFIELD);
				}
			}
		}
	}

	if (dlg.m_LCtag != _T("")) 
	{
		INT iLen = dlg.m_LCtag.GetLength();
		if (iLen > 1) 
		{
			CString strItemData = dlg.m_LCtag.Mid(7 , dlg.m_LCtag.GetLength() - 8);
			CString str085Field;
			m_pMarcMgr->GetField(m_pMarc,_T("085"), str085Field );
			if ( str085Field.IsEmpty() ) m_pMarcMgr->InsertField(m_pMarc, _T("0855 "));
			m_pMarcMgr->SetItem(m_pMarc, _T("085$a"), strItemData , _T(""), ADD_SUBFIELD);
			m_pMarcMgr->GetItem(m_pMarc, _T("085$2"), strItemData );
			if ( strItemData.IsEmpty() ) {
				
				((CCatApi_SE*)m_pCatApi)->LoadClassCodeVolumeDlg(_T("..\\cfg\\분류기호판사항.txt"), n056, n082, n085);
				if (n082 != _T("-1"))
				{
					strItemData.Format(_T("%s"), n085);
					m_pMarcMgr->SetItem(m_pMarc, _T("082$2"), strItemData, _T(""), ADD_SUBFIELD);
				}
			}
		}
	}
	
	m_pResourceMgr->GetMarcEditor()->Display();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::CMD_CreateCallNo()
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
	
	CString strClass = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("ISS_분류표구분"), 0);
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
	else
		return -1;

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
	strPublishFormCode = _T("SE");

	CString sFormCode = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("ISS_형식구분_코드"), 0);
	CString sSubRegCode = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("ISS_보조등록구분_코드"), 0);

	CString strWhere;
	strWhere.Format(_T("PUBLISH_FORM_CODE = '%s' AND FORM_CODE = '%s' AND SUB_REG_CODE = '%s' AND CLASS_CODE = '%s'")
					 , strPublishFormCode, sFormCode, sSubRegCode, sClassNo);
	if (sSubRegCode.GetLength()>0)
		strWhere.Format(_T("PUBLISH_FORM_CODE = '%s' AND FORM_CODE = '%s' AND SUB_REG_CODE = '%s' AND CLASS_CODE = '%s'")
					 , strPublishFormCode, sFormCode, sSubRegCode, sClassNo);
	else 
		strWhere.Format(_T("PUBLISH_FORM_CODE = '%s' AND FORM_CODE = '%s' AND SUB_REG_CODE is NULL AND CLASS_CODE = '%s'")
					 , strPublishFormCode, sFormCode, sClassNo);
	ids = pDmAuthorRule->RefreshDataManager(strWhere);
	if (ids < 0) return ids;

	if ( pDmAuthorRule->GetRowCount() < 1 ) {
		if (sSubRegCode.GetLength()>0)
			strWhere.Format(_T("PUBLISH_FORM_CODE = '%s' AND FORM_CODE = '%s' AND SUB_REG_CODE = '%s' AND CLASS_CODE IS NULL")
						 , strPublishFormCode, sFormCode, sSubRegCode);
		else 
			strWhere.Format(_T("PUBLISH_FORM_CODE = '%s' AND FORM_CODE = '%s' AND SUB_REG_CODE is NULL AND CLASS_CODE IS NULL")
						 , strPublishFormCode, sFormCode);
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
		if (ids < 0) 
		{			
			AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
			return -1;
		}
		
		ids = MakeData.GetKeyWord( strItemData );				
		if (ids < 0) 
		{			
			AfxMessageBox ( _T("MakeData.ChangeHanjaToHangul( sTitle ) error") );
			return -1;
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

		m_pDM_Parent->GetOneValueQuery(strQuery, strResult);


		if ( strResult != _T("서명첫자")) {
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
			strItemData = strItemData + sFirstWord;

		m_pMarcMgr->SetItem(m_pMarc, _T("090$a"), s090a);
		m_pMarcMgr->SetItem(m_pMarc, _T("090$b"), strItemData);
	}
	else if ( iSwCallNoUpdate == 0 ) 
	{	
		m_iSwAcqCallNo = 1;
		m_pMarcMgr->SetItem(m_pMarc, _T("090$a"), s090a);
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
INT CMarcAdjMgr_SE::CMD_EditFixedField2()
{
	EFS_BEGIN
		CDLG_006 dlg(m_pCatESLMgr, m_pMarc);		
		dlg.DoModal();		 

	return 0;

	EFS_END

	return -1;
}

INT CMarcAdjMgr_SE::CMD_EditFixedField()
{
	EFS_BEGIN

	CString sFormCode = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("ISS_형식구분_코드"), 0);

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

	ApplyMarcChange();

	CString strTemp;
	m_pMarcMgr->GetItem(m_pMarc, _T("008@22"), strTemp);
	if ( strTemp == _T("j") ) {
		m_pCatESLMgr->m_pDM_Index->SetCellData( _T("ISS_이용대상구분_코드"), _T("JU"), 0 );	
		m_pCatESLMgr->m_pDM_Species->SetCellData( _T("SS_이용대상구분_코드"), _T("JU"), 0 );
	}

	CString str260c, strYear1, strYear2, strYearType, str260from008;
	INT ids;

	m_pMarcMgr->GetItem(m_pMarc, _T("008@6"), strYearType);
	m_pMarcMgr->GetItem(m_pMarc, _T("008@7-10"), strYear1);
	m_pMarcMgr->GetItem(m_pMarc, _T("008@11-14"), strYear2);

	strYear1.TrimLeft();
	strYear1.TrimRight();
	strYear2.TrimLeft();
	strYear2.TrimRight();

	if ( strYearType == _T("c") || strYearType == _T("u") )
		str260from008.Format(_T("%s-"), strYear1);
	else if ( strYearType == _T("d") )
	{
		if (strYear2.GetLength() > 0)
			str260from008.Format(_T("%s-%s"), strYear1, strYear2);
		else str260from008.Format(_T("%s"), strYear1);
	}
	else str260from008.Format(_T("%s"), strYear1);

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("260$c"), str260c);
	if (ids < 0 || str260c.IsEmpty())
	{
		m_pMarcMgr->SetItem(m_pMarc, _T("260$c"), str260from008 );
	}
	else if ( str260from008.Compare( str260c ) )
	{
		CString strMsg;
		strMsg.Format(_T("고정길이의 발행년 사항과 발행사항필드의 발행년 사항이 다릅니다.\n\n")
					_T("008@6-14 : %s%s%s\n260$c : %s\n고정길이에 맞는 발행년사항은 %s입니다.")
					, strYearType, strYear1, strYear2, str260c, str260from008);
		AfxMessageBox(strMsg);
	}

	m_pResourceMgr->GetMarcEditor()->Display();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::CMD_GiveRegNoAuto()
{
	EFS_BEGIN

	ApplyData();
	INT iRowCount = m_pCatESLMgr->m_pDM_Book->GetRowCount();
	INT iSwOk = 0;
	INT i;

	CString sRegNo, sRegCode, sDBFlag;
	for ( i = 0 ; i < iRowCount ; i++ ) {
		sDBFlag = m_pCatESLMgr->m_pDM_Book->GetCellData( _T("SB_DB_FLAG"), i );
		sRegNo = m_pCatESLMgr->m_pDM_Book->GetCellData( _T("SB_등록번호"), i );
		sRegCode = m_pCatESLMgr->m_pDM_Book->GetCellData( _T("SB_등록구분_코드"), i );
		if ( (sRegNo == _T("") && sRegCode != _T("")) && sDBFlag.Compare(_T("D")) ) {
			iSwOk = 1;
			break;
		}
	}
	if ( iSwOk == 0 ) {
		iRowCount = m_pCatESLMgr->m_pDM_Appendix->GetRowCount();
		for ( i = 0 ; i < iRowCount ; i++ ) {
			sDBFlag = m_pCatESLMgr->m_pDM_Appendix->GetCellData( _T("SB_DB_FLAG"), i );
			sRegNo = m_pCatESLMgr->m_pDM_Appendix->GetCellData( _T("SB_등록번호"), i );
			sRegCode = m_pCatESLMgr->m_pDM_Appendix->GetCellData( _T("SB_등록구분_코드"), i );
			if ( (sRegNo == _T("") && sRegCode != _T("")) && sDBFlag.Compare(_T("D")) ) {
				iSwOk = 1;
				break;
			}
		}
		if ( iSwOk == 0 ) {
			AfxMessageBox( _T("등록번호 부여할 책(부록)이 없습니다.") );
			return -1;
		}
	}

	CBO_CAT_AUTO_REGNO_GIVE dlg(m_pCatESLMgr);
	dlg.SetModeNomo_Serial(MODE_SERIAL);
	dlg.DoModal();

	m_pResourceMgr->m_dlgVolume.Display();
	ApplyDMChange();
	
	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::CMD_HoldInfo()
{
	EFS_BEGIN

	if ( m_pCatESLMgr->m_pDM_Book->GetRowCount() ==0 && m_nManageMode == MNG_INSERT)
	{
		AfxMessageBox(_T("입력된 책정보가 없습니다.\n권호정보에서 권/책 정보를 입력하여 주십시오."));
		return -2;
	}

	if ( ((CCatApi_SE*)m_pCatApi)->ConvertBookTo049(m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Book, m_nManageMode) < 0) return -1;
	CHoldInfoDlg dlg(m_pCatESLMgr);
	dlg.SetParentInfo(_T("SS_소장정보"), m_pCatESLMgr->m_pDM_Species, 0);
	dlg.DoModal();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::CMD_MarcImport()
{
	EFS_BEGIN

	CMARC_IMPORT dlg(m_pCatESLMgr);
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

INT CMarcAdjMgr_SE::CMD_Save()
{
	EFS_BEGIN

	iOnMarcEditUpdateCheckCount = 0;
		m_bOnMarcEditUpdateCheck = TRUE;
		m_bOnMarcEditUpdateCountCheck = TRUE;
		INT nTemp = OnMarcEditUpdate();
		if(nTemp < 0 ) 
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

		ids = ApplyMarcChange();
		if (ids < 0) return ids;

		ids = ((CCatApi_SE*)m_pCatApi)->ApplyShelfCodeData(m_pCatESLMgr->m_pDM_Book, m_pMarc, m_bCatHold);
		if (ids < 0) return ids;

		ids = ((CCatApi_SE*)m_pCatApi)->ApplyShelfCodeData(m_pCatESLMgr->m_pDM_Appendix, m_pMarc, m_bCatHold);
		if (ids < 0) return ids;

		m_pCatESLMgr->m_pDM_Species->SetCellData( _T("SS_정리상태"), _T("1"), 0 );

		ids = m_pMarcMgr->DeleteEmptySubfield(m_pMarc);

		ids = m_pMarcMgr->DeleteEmptyField(m_pMarc);	
	}
	
	CreateAcqCallNo();

	m_pFrameWnd->RedrawWindow();
	AfxGetApp()->BeginWaitCursor();

	CString n056 = _T("0");
	CString n082 = _T("0");
	CString n085 = _T("0");
	CString strItemData;
	ids = ((CCatApi_SE*)m_pCatApi)->LoadClassCodeVolumeDlg(_T("..\\cfg\\분류기호판사항.txt"), n056, n082, n085);

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

	CString strMarc;
	ids = m_pMarcMgr->Encoding(m_pMarc, strMarc);

	ids = m_pCatESLMgr->m_pDM_Species->SetCellData(_T("SS_MARC"), strMarc, 0);

	if (GetManageMode() == MNG_INSERT)
	{
		if ( m_pCatESLMgr->m_pDM_Book->GetRowCount() ==0 )
		{
			AfxMessageBox(_T("입력된 책정보가 없습니다.\n권호정보에서 권/책 정보를 입력하여 주십시오."));
			return -2;
		}

		CString sProvider;
		CString sAcqCode = m_pCatESLMgr->m_pDM_Book->GetCellData( _T("SB_수입구분_코드"), 0 );
		if ( sAcqCode == _T("1") ) {
			ids = m_pResourceMgr->m_dlgCode.m_pCM->GetControlMgrData( _T("BP_구입처부호"), sProvider );
			m_pCatESLMgr->m_pDM_Book->SetCellData( _T("SB_구입처"), sProvider, 0 );
		}
		else if ( sAcqCode == _T("2") ) {
			m_pResourceMgr->m_dlgCode.GetDlgItem(IDC_eDONATOR)->GetWindowText(sProvider);
			m_pCatESLMgr->m_pDM_Book->SetCellData( _T("SB_구입처"), sProvider, 0 );
		}
		
		CString strInputStatus;
		ids = m_pResourceMgr->m_dlgCode.m_pCM->GetControlMgrData(_T("입력자료상태"), strInputStatus);
	
		if (strInputStatus == _T("소장자료"))
		{
			ids = ((CCatApi_SE*)m_pCatApi)->ImportSpecies(m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Appendix, m_pCatESLMgr->m_pDM_HoldBook, m_pCatESLMgr->m_pDM_HoldAppendix, &m_arrStrAddFrame);
			if ( ids < 0 ) return -1;
		}
		else if (strInputStatus = _T("정리자료"))
		{
			ids = ((CCatApi_SE*)m_pCatApi)->InsertSpecies(m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Appendix, m_pCatESLMgr->m_pDM_HoldBook, m_pCatESLMgr->m_pDM_HoldAppendix, &m_arrStrAddFrame);
			if ( ids < 0 ) return -1;
		}
	}
	else if (GetManageMode() == MNG_MODIFY)
	{
		ids = ((CCatApi_SE*)m_pCatApi)->UpdateSpecies(m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Appendix, m_pCatESLMgr->m_pDM_HoldBook, m_pCatESLMgr->m_pDM_HoldAppendix, &m_arrStrAddFrame);
		if ( ids < 0 ) return -1;
	}
	else if (GetManageMode() == MNG_IMPORT)
	{
		ids = ((CCatApi_SE*)m_pCatApi)->ImportSpecies(m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Appendix, m_pCatESLMgr->m_pDM_HoldBook, m_pCatESLMgr->m_pDM_HoldAppendix, &m_arrStrAddFrame);
		if ( ids < 0 ) return -1;

		if (ids >= 0)
		{
			CString strData = m_pGrid_Parent->GetTextMatrix(m_nParentSelectedRow + 1, 0);
			if (strData.IsEmpty()) return -1;

			INT idxDM = _ttoi(strData) - 1 ;

			ids = m_pDM_Species_Parent->SetCellData(_T("SS_파일반입성공"), _T("Y"), idxDM);
		}
	}
	else if ( m_pCatESLMgr->GetAppMode() == MNG_IMPORT_WORKING)
	{
		ids = ((CCatApi_SE*)m_pCatApi)->InsertSpecies(m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Appendix, m_pCatESLMgr->m_pDM_HoldBook, m_pCatESLMgr->m_pDM_HoldAppendix, &m_arrStrAddFrame);

		if (ids >= 0)
		{
			CString strData = m_pGrid_Parent->GetTextMatrix(m_nParentSelectedRow + 1, 0);
			if (strData.IsEmpty()) return -1;

			INT idxDM = _ttoi(strData) - 1;
			ids = m_pDM_Species_Parent->SetCellData(_T("SS_파일반입성공"), _T("Y"), idxDM);
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

	m_arrRegNo.RemoveAll();

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("007*"), m_str007FieldPrev);

	ids = m_pMarcMgr->GetItem(m_pMarc, _T("008*"), m_str008FieldPrev);

	m_pResourceMgr->GetMarcEditor()->Display();
	ids = m_pResourceMgr->m_dlgVolume.Display();

	AfxGetApp()->EndWaitCursor();

	if (ids < 0)
		m_strLastErrMsg.Format(_T("저장 실패\n%s"), ((CCatApi_SE*)m_pCatApi)->GetLastError());
	else
		m_strLastErrMsg = _T("저장하였습니다.");
	
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

			((CCatApi_SE*)m_pCatApi)->CopyDM(m_pCatESLMgr->m_pDM_Index, 0, m_pDM_Parent, nRowIdx);
			((CCatApi_SE*)m_pCatApi)->CopyDM(m_pCatESLMgr->m_pDM_Species, 0, m_pDM_Parent, nRowIdx);

			strItemData.Format(_T("%d"), ((CCatApi_SE*)m_pCatApi)->GetBookCount(m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Appendix));
			m_pDM_Parent->SetCellData(_T("책수"), strItemData, nRowIdx);

			INT nIdx = - 1;
			ids = ((CCatApi_SE*)m_pCatApi)->GetRepresentationBook(m_pCatESLMgr->m_pDM_Book, _T("SB"), nIdx);
			if (ids < 0 || nIdx < 0)
				nIdx = ((CCatApi_SE*)m_pCatApi)->SetRepresentationBook(m_pCatESLMgr->m_pDM_Book, _T("SB"), nIdx);
			m_pDM_Parent->SetCellData(_T("시작등록번호"), m_pCatESLMgr->m_pDM_Book->GetCellData(_T("SB_등록번호"), nIdx), nRowIdx);

			m_pGrid_Parent->DisplayLine(nRowIdx);

			InitManager(GetManageMode(), m_pCatESLMgr->pParentMgr, m_strWorkingStatus, NULL);
			
			CString strArrCode;
			m_pCatESLMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("등록구분"), strArrCode);

            m_pResourceMgr->m_dlgCode.GetDlgItem(IDC_eREG_CLASS_CODE_MA)->EnableWindow(TRUE);
			m_pResourceMgr->m_dlgCode.SetDlgItemText(IDC_eREG_CLASS_CODE_MA,strArrCode);
			
			((CCatApi_SE*)m_pCatApi)->MakeDMFromCM(m_pResourceMgr->m_dlgCode.m_pCM, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->GetAppMode(), 0, _T("ISS") );
			m_pResourceMgr->m_dlgVolume.Display();
			InitMarcEditor();

			if (m_pResourceMgr->m_dlgCode.m_bFixedFieldMaintenance)
			{
				m_pMarc->GetLeader(m_strLeaderPrev);
				ApplyFiexedFieldMaintenance();
			}

			m_pResourceMgr->GetMarcEditor()->Display();

			BackupOldMarc();
		}
		else if (GetManageMode() == MNG_MODIFY)
		{
			((CCatApi_SE*)m_pCatApi)->CopyDM(m_pCatESLMgr->m_pDM_Index, 0, m_pDM_Parent, m_nParentSelectedRow);
			
			((CCatApi_SE*)m_pCatApi)->CopyDM(m_pCatESLMgr->m_pDM_Species, 0, m_pDM_Parent, m_nParentSelectedRow);

			strItemData.Format(_T("%d"), ((CCatApi_SE*)m_pCatApi)->GetBookCount(m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Appendix));
			m_pDM_Parent->SetCellData(_T("책수"), strItemData, m_nParentSelectedRow);

			m_pGrid_Parent->DisplayLine(m_nParentSelectedRow);

			CMD_Next();
		}
	}

	return ids;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::CMD_SearchCallNo(BOOL bShowMsg /*= TRUE*/)
{
	EFS_BEGIN

	CESL_DataMgr *pDM = m_pCatESLMgr->FindDM(_T("DM_SE_CAT_PROC_0380"));
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
	CString sMarcKey = pDM_SPECIES->GetCellData( _T("ISS_REC_KEY"), 0 );
	
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
		strWhere.Format( _T("I.REC_KEY = S.REC_KEY AND ( I.CLASS_NO = '%s'%s AND (I.BOOK_CODE LIKE '%s%%' OR I.BOOK_CODE LIKE '%s%%') OR S.REC_KEY = '%s' )"), strCss, sSub, strBookCode, sTmpBookCode, sMarcKey );
	}
	else {
		strWhere.Format( _T("I.REC_KEY = S.REC_KEY AND I.CLASS_NO = '%s'%s AND (I.BOOK_CODE = '%s' OR I.BOOK_CODE = '%s') AND I.REC_KEY != '%s'"), strCss, sSub, strBookCode, sTmpBookCode, sMarcKey );		
	}

	if (pDM->RefreshDataManager(strWhere) < 0)	return -1;
	
	INT iRowCount = pDM->GetRowCount();

	INT ids2 = 0;
	if (pDM->GetRowCount() >= 1 ) 	
	{
		INT iSwFind = 0;
		for( INT j = 0 ; j < pDM->GetRowCount() ; j++ ) {
			CString sQuerySpeciesKey = pDM->GetCellData(_T("SS_종KEY"), j );
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
		pDM->SetCellData( _T("SS_MARC"), sStreamMarc, j );

		CString str_tmpVolCode,str_tmpBookCode,sSeperate;
		for(INT i = 0 ; i < pDM->GetRowCount () ; i++)
		{    
			str_tmpVolCode.Empty () ;
			str_tmpBookCode.Empty () ;
			pDM->GetCellData (_T("SS_MARC.청구기호_권책기호"),i,str_tmpVolCode) ;
			pDM->GetCellData (_T("SS_MARC.청구기호_도서기호"),i,str_tmpBookCode) ;

			pDM->SetCellData (_T("권책기호"),str_tmpVolCode,i);
			pDM->SetCellData (_T("도서기호"),str_tmpBookCode,i);
		}

		CBO_CAT_PROC_0380 dlg(m_pCatESLMgr);
		dlg.m_nModeMONO_SERIAL = MODE_SERIAL;
		dlg.m_strParentSpeciesKey = m_pCatESLMgr->m_pDM_Species->GetCellData(_T("SS_종KEY"), 0);
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

INT CMarcAdjMgr_SE::CMD_SetDefaultValue()
{
	EFS_BEGIN

	CString strGroup;
	CString strFormCode = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("ISS_형식구분_코드"), 0);
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

INT CMarcAdjMgr_SE::CMD_SupportClasification()
{
	EFS_BEGIN

	CMarcEditor *pMarcEditor = m_pResourceMgr->GetMarcEditor();

	CString sTitle;
	m_pCatESLMgr->m_pInfo->pMarcMgr->GetItem( m_pMarc, _T("245$a"), sTitle );	

	CBO_CAT_PROC_0330 dlg(m_pCatESLMgr, sTitle);
	dlg.m_nModeMONO_SERIAL = MODE_SERIAL;
	if (IDOK != dlg.DoModal()) return -1;
	
	CString n056 = -1;
	CString n082 = -1;
	CString n085 = -1;
	CString strItemData;
	((CCatApi_SE*)m_pCatApi)->LoadClassCodeVolumeDlg(_T("..\\cfg\\분류기호판사항.txt"), n056, n082, n085);

	INT iLen = dlg.m_KDC.GetLength();
	if ( iLen > 1 ) {
		m_pMarcMgr->SetItem(m_pMarc, _T("056$a"), dlg.m_KDC , _T(""), ADD_SUBFIELD);		
		m_pMarcMgr->GetItem(m_pMarc, _T("056$2"), strItemData );
		if ( strItemData.IsEmpty()  && n056 != _T("-1"))
		{
			strItemData = dlg.m_KDC;
			strItemData.Format(_T("%s"), n056);
			m_pMarcMgr->SetItem(m_pMarc, _T("056$2"), strItemData, _T(""), ADD_SUBFIELD);
		}
	}
	iLen = dlg.m_DDC.GetLength();
	if ( iLen > 1 ) {
		m_pMarcMgr->SetItem(m_pMarc, _T("082$a"), dlg.m_DDC , _T(""), ADD_SUBFIELD);
		m_pMarcMgr->GetItem(m_pMarc, _T("082$2"), strItemData );
		if ( strItemData.IsEmpty() && n082 != _T("-1"))
		{
			strItemData = dlg.m_DDC;
			strItemData.Format(_T("%s"), n082);
			m_pMarcMgr->SetItem(m_pMarc, _T("082$2"), strItemData, _T(""), ADD_SUBFIELD);
		}
	}

	iLen = dlg.m_DDC.GetLength();
	if ( iLen > 1 ) {
		CString str085Field;
		m_pMarcMgr->GetField(m_pMarc,_T("085"), str085Field );
		if ( str085Field.IsEmpty() ) m_pMarcMgr->InsertField(m_pMarc, _T("0855 "));
		m_pMarcMgr->SetItem(m_pMarc, _T("085$a"), dlg.m_DDC , _T(""), ADD_SUBFIELD);
		m_pMarcMgr->GetItem(m_pMarc, _T("085$2"), strItemData );
		if ( strItemData.IsEmpty() && n085 != _T("-1"))
		{
			strItemData = dlg.m_LC;
			strItemData.Format(_T("%s"), n085);
			m_pMarcMgr->SetItem(m_pMarc, _T("085$2"), strItemData, _T(""), ADD_SUBFIELD);
		}
	}

	m_pMarcMgr->SortMarc( m_pMarc );
	pMarcEditor->Display();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::CMD_UCImport()
{
	EFS_BEGIN

	CUC_MARC_IMPORT dlg(m_pCatESLMgr);
	if ( dlg.DoModal() != IDOK ) return -1;
	
	
	CString s001FirstData ,s049FirstData ;
	m_pMarcMgr->GetItem (m_pMarc,_T("001*"),s001FirstData) ;
    m_pMarcMgr->GetField (m_pMarc,_T("049"),s049FirstData) ;

	INT iMode = dlg.m_check;
	CString sStreamMarc = dlg.sMarc;
	CMarc marcTmp;
	if ( iMode == 0 ) {			
		
		m_pMarcMgr->Decoding(sStreamMarc, &marcTmp);

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
	
	m_pResourceMgr->GetMarcEditor()->Display();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::CMD_VolumeInfo()
{
	EFS_BEGIN

	if (ApplyData() < 0) return -1;

	CSEVolumeDlg dlg(m_pCatESLMgr);
	dlg.SetCatApi(m_pCatApi);

	dlg.SetParentInfo(m_pCatESLMgr->m_pDM_Volume, 
					  m_pCatESLMgr->m_pDM_Book, 
					  m_pCatESLMgr->m_pDM_HoldBook, 
					  m_pCatESLMgr->m_pDM_Appendix, 
					  m_pCatESLMgr->m_pDM_HoldAppendix,
					  m_pCatESLMgr->GetAppMode() );
	dlg.DoModal();

	OnVolumeUpdate();
	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::CreateAcqCallNo()
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
	
	sClassType = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("ISS_분류표구분"), 0);
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

	CString sQueryAcqNumber =	_T("SELECT ACQUISIT_NO FROM CO_LAST_BOOK_ACQUISITNO_TBL ")
								_T("WHERE ") + sWhere + _T(" AND MANAGE_CODE=UDF_MANAGE_CODE");

	CString sAcqNumber;	
	INT nResult = m_pCatESLMgr->m_pDM_Book->GetOneValueQuery( sQueryAcqNumber , sAcqNumber );
	if ( sAcqNumber == _T("") ) 
		sAcqNumber = _T("1");
	else 
	{
		INT nAcqNumber = _ttoi ( sAcqNumber.GetBuffer(0) );
		sAcqNumber.Format(_T("%d"), nAcqNumber);
	}

	if ( m_nManageMode == MNG_MODIFY )
	{
		if (m_str090bPrev.IsEmpty()) 
		{
			CBO_CAT_PROC_AcqNumber dlg(m_pCatESLMgr);
			dlg.SetData( sShelfCode, sClassType, sClassNo, m_str090bPrev, _T("자동 부여")/*, _T("수입순번호부여")*/);
	
			if ( dlg.DoModal() == IDOK ) 
			{	
				if (nResult > 0)
				{
					INT iAcqNumber = _ttoi ( sAcqNumber.GetBuffer(0) );
					iAcqNumber++;
					sAcqNumber.Format( _T("%d"), iAcqNumber );
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
					m_pMarcMgr->SetItem( m_pMarc, _T("090$b"), '1' );
					INT ids = m_pCatESLMgr->m_pDM_Book->MakeRecKey( sAcqRecKey );
					if (ids < 0) return ids;
					sQuery.Format(	_T("INSERT INTO CO_LAST_BOOK_ACQUISITNO_TBL ")
									_T("(REC_KEY, CLASS_NO_TYPE, CLASS_NO, ACQUISIT_NO, ")
									_T("SEPARATE_SHELF_CODE, REG_CODE, MANAGE_CODE ) ")
									_T("VALUES('%s','%s','%s','%s','%s','%s',UDF_MANAGE_CODE);")
									, sAcqRecKey, sClassType, sClassNo, _T("1"), sShelfCode, _T("") );
					
					m_arrStrAddFrame.Add(sQuery);
				}
			}
			else 
			{	
				CString sAcqNumber;
				sAcqNumber = dlg.GetAcqNoData();
				m_pMarcMgr->SetItem( m_pMarc, _T("090$b"), sAcqNumber);
			}
		}
		else 
		{
			CBO_CAT_PROC_AcqNumber dlg(m_pCatESLMgr);
			dlg.SetData( sShelfCode, sClassType, sClassNo, m_str090bPrev, _T("재부여"));
			
			if ( dlg.DoModal() == IDOK ) 
			{
				if ( dlg.m_iDropAcqSw == 1 ) 
				{	
					sAcqNumber = dlg.m_sAcqNumber;
					m_pMarcMgr->SetItem( m_pMarc, _T("090$b"), sAcqNumber );
				}
				else 
				{
					INT iAcqNumber = _ttoi ( sAcqNumber.GetBuffer(0) );
					iAcqNumber++;
					sAcqNumber.Format( _T("%d"), iAcqNumber );
					m_pMarcMgr->SetItem( m_pMarc, _T("090$b"), sAcqNumber );
					m_str090bPrev = sAcqNumber;
					
					CString sQuery;
					sQuery.Format(	_T("UPDATE CO_LAST_BOOK_ACQUISITNO_TBL ")
									_T("SET ACQUISIT_NO='%s' WHERE %s AND MANAGE_CODE=UDF_MANAGE_CODE;") 
									, sAcqNumber, sWhere );
					m_arrStrAddFrame.Add( sQuery );					
				}
			}
			else 
			{
				m_pMarcMgr->SetItem( m_pMarc, _T("090$b"), m_str090bPrev);
			}
		}
	}
	else
	{
		if (nResult > 0)
		{
			INT iAcqNumber = _ttoi ( sAcqNumber.GetBuffer(0) );
			iAcqNumber++;
			sAcqNumber.Format( _T("%d"), iAcqNumber );
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
			m_pMarcMgr->SetItem( m_pMarc, _T("090$b"), '1' );
			INT ids = m_pCatESLMgr->m_pDM_Book->MakeRecKey( sAcqRecKey );
			if (ids < 0) return ids;

			sQuery.Format(	_T("INSERT INTO CO_LAST_BOOK_ACQUISITNO_TBL ")
							_T("(REC_KEY, CLASS_NO_TYPE, CLASS_NO, ACQUISIT_NO, ")
							_T("SEPARATE_SHELF_CODE, REG_CODE, MANAGE_CODE) ")
							_T("VALUES('%s','%s','%s','%s','%s','%s',UDF_MANAGE_CODE);")
							, sAcqRecKey, sClassType, sClassNo, _T("1"), sShelfCode, _T("") );
			m_arrStrAddFrame.Add(sQuery);
		}
	}
	m_pResourceMgr->GetMarcEditor()->Display();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::CreateCatESLMgr(CESL_Mgr *pParentESLMgr)
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
	if (m_pCatESLMgr->InitCatESL_Mgr(m_nManageMode, MODE_SERIAL) < 0) return -1;

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::CreateResource(CFrameWnd *pFrameWnd)
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

	m_pResourceMgr->HideToolBarButton(_T("복본조사"));

	m_pResourceMgr->HideToolBarButton(_T("자동번호부여"));
	
	m_pResourceMgr->HideToolBarButton(_T("등록번호부여"));

	m_pResourceMgr->DeleteMenuItem(_T("SUPPORT_별치기호"));

	m_pResourceMgr->SetToolBarButtonCatpion(_T("권별정보"), _T("권호정보"));

	m_pResourceMgr->HideToolBarButton(_T("부록정보"));	
	
	m_pResourceMgr->HideToolBarButton(_T("목차초록관리"));

	m_pResourceMgr->HideToolBarButton(_T("표지정보"));

	CString strTitle;
	if (m_nManageMode == MNG_INSERT)
	{
		strTitle = _T("MARC 정리 - [입력]");
		m_pResourceMgr->m_dlgCode.ApplyCodeChange();

		ApplyMarcChange();

		m_pResourceMgr->m_dlgCode.Display();

		m_pResourceMgr->HideToolBarButton(_T("이전"));

		m_pResourceMgr->HideToolBarButton(_T("다음"));

		m_pResourceMgr->HideToolBarButton(_T("목차초록관리"));

		CString strMatCode = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("ISS_매체구분_코드"), 0);
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

		m_pResourceMgr->m_dlgCode.Display();

		if (IsFirstMarcAdjust())
		{
			CString strFormCode = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("ISS_형식구분_코드"), 0);
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
        strMATClassCode= m_pCatESLMgr->m_pDM_Index->GetCellData(_T("ISS_매체구분_코드"),0) ; 
	
		if (strMATClassCode == _T("EB"))
		{
		m_pResourceMgr->HideToolBarButton (_T("ebook보기"),FALSE);
		}
		else 
		{
		m_pResourceMgr->HideToolBarButton (_T("ebook보기")) ;
		}
	}
	else if (m_nManageMode == MNG_IMPORT)
	{
		strTitle = _T("MARC 정리 - [파일반입]");
		ApplyDMChange();
		ApplyMarcChange();
		
		m_pResourceMgr->m_dlgCode.Display();

		CString strMATClassCode;
        strMATClassCode= m_pCatESLMgr->m_pDM_Index->GetCellData(_T("ISS_매체구분_코드"),0) ; 
	
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

INT CMarcAdjMgr_SE::Display(bool m_bInit /*= false*/)
{
	EFS_BEGIN

	CString strStreamMarc = m_pCatESLMgr->m_pDM_Species->GetCellData(_T("SS_MARC"), 0);
	m_pMarcMgr->Decoding(strStreamMarc, m_pMarc);

	m_pMarcMgr->GetItem(m_pMarc, _T("090$b"), m_str090bPrev);

	if (m_pResourceMgr->m_dlgCode.m_bFixedFieldMaintenance)
		ApplyFiexedFieldMaintenance();
	
	((CCatApi_SE*)m_pCatApi)->CopyDM(m_pCatESLMgr->m_pDM_Species, 0, m_pCatESLMgr->m_pDM_Index, 0, _T("ISS"));

	if(m_bInit == false)
	{
		((CCatApi_SE*)m_pCatApi)->ConvertCatDMToMarc(m_pMarc, m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Purchase);
    }

	((CCatApi_SE*)m_pCatApi)->ConvertMarcToCatDM(m_pMarc, m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Purchase);

	m_pResourceMgr->m_dlgCode.Display();
	
	m_pResourceMgr->m_dlgVolume.Display();

	if (IsFirstMarcAdjust())
	{
		CString strFormCode = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("ISS_형식구분_코드"), 0);
		ApplyDefaultMarc(m_pMarc, strFormCode);
	}

	InitMarcEditor();
	m_pResourceMgr->GetMarcEditor()->Display();

	BackupOldMarc();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::InitMarcEditor()
{
	EFS_BEGIN

	m_pResourceMgr->m_MarcEditor.Init(m_pMarcMgr, m_pMarc, m_pCatESLMgr->m_pInfo);
	
	m_pResourceMgr->m_MarcEditor.InitLimitTag();
	m_pResourceMgr->m_MarcEditor.AddLimitTag(_T("020"), _T("  "));
	
	m_pResourceMgr->m_MarcEditor.InitUserAlias();

	CString strFormCode = m_pCatESLMgr->m_pDM_Index->GetCellData(_T("ISS_형식구분_코드"), 0);

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

BOOL CMarcAdjMgr_SE::IsFirstMarcAdjust()
{
	CString strApplyYN = m_pCatESLMgr->m_pDM_Species->GetCellData(_T("SS_정리된자료"), 0);
	if (strApplyYN == _T("Y")) return FALSE;

	strApplyYN = _T("Y");
	m_pCatESLMgr->m_pDM_Species->SetCellData(_T("SS_정리된자료"), strApplyYN, 0);
	
	return TRUE;
}

INT CMarcAdjMgr_SE::MakeImportDM()
{
EFS_BEGIN

	if (m_nParentSelectedRow < 0) return -1;

	CString strData = m_pGrid_Parent->GetTextMatrix(m_nParentSelectedRow + 1, 0);
	if (strData.IsEmpty()) return -1;

	INT idxDM = _ttoi(strData) - 1;

	INT ids = ((CCatApi_SE*)m_pCatApi)->MakeMarcAdjustDM(m_pDM_Species_Parent, idxDM, m_pDM_Index_Parent, idxDM, m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume);
	if (ids < 0) return -1;

	return 0;

EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::MakeUpdateDM()
{
	EFS_BEGIN

	INT ids = -1;

	if (m_nParentSelectedRow < 0) return -1;

	CString strSpeciesKey;
	CString strPurchaseKey;
	CString strTbl, strTbl2;
	CString strQuery;
	CString strOption;

	m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, m_nParentSelectedRow, strSpeciesKey);

	if (strSpeciesKey.IsEmpty()) return -1;

	strOption.Format(_T("REC_KEY = %s"), strSpeciesKey);
	ids = m_pCatESLMgr->m_pDM_Index->RefreshDataManager(strOption);
	if (ids < 0) return ids;
	
	strOption.Format(_T("REC_KEY = %s"), strSpeciesKey);
	ids = m_pCatESLMgr->m_pDM_Species->RefreshDataManager(strOption);
	if (ids < 0) return ids;

	CString str049MarcStream;
	CString strHoldInfoKey = m_pCatESLMgr->m_pDM_Species->GetCellData(_T("SS_소장정보KEY"), 0);
	if (!strHoldInfoKey.IsEmpty())
	{
		strQuery.Format(_T("SELECT HOLDING_DATA FROM SE_HOLDING_TBL WHERE REC_KEY = %s"), strHoldInfoKey, 0);
		ids = m_pCatESLMgr->m_pDM_Species->GetOneValueQuery(strQuery, str049MarcStream);
		if (ids >= 0 && !str049MarcStream.IsEmpty())
		{
			m_pCatESLMgr->m_pDM_Species->SetCellData(_T("SS_소장정보"), str049MarcStream, 0);
		}
	}

	strTbl = _T("SE_BOOK_TBL B ");
	if (m_strBookWhere.Find(_T("A.")) >= 0)
	{
		strTbl += _T(" , CO_ACCESSION_REC_TBL A ");
	}
	CString strWorkingStatus = _T("b.WORKING_STATUS IN ('SEC111N', 'SEC112O', 'SEC211O', 'SEL211O', 'SEL212N', 'SEL213O', 'SEL214O', 'SEL215O', 'SEL311O', 'SEL312O', 'SEL313O', 'SEL314O', 'SEL315O', 'SEL411O', 'SEL511O', 'SEL611O')");

	strOption.Format(_T("SPECIES_KEY = %s AND REC_KEY IN (SELECT VOL_KEY FROM %s WHERE b.SPECIES_KEY = %s AND (b.BINDING_BOOK_KEY IS NULL) and (b.BOOK_TYPE in ('C', 'B')) AND %s) ORDER BY VOL_SORT_NO, VOL_TITLE"), strSpeciesKey, strTbl, strSpeciesKey, strWorkingStatus);

	ids = m_pCatESLMgr->m_pDM_Volume->RefreshDataManager(strOption);
	if (ids < 0) return ids;

	strOption.Format(_T("b.SPECIES_KEY = %s AND %s"), strSpeciesKey, m_strBookWhere);
	strTbl = m_pCatESLMgr->m_pDM_Book->TBL_NAME;
	strTbl2 = m_pCatESLMgr->m_pDM_Appendix->TBL_NAME;

	if (m_strBookWhere.Find(_T("A.")) >= 0)
	{
		m_pCatESLMgr->m_pDM_Book->TBL_NAME += _T(" , CO_ACCESSION_REC_TBL A ");
		m_pCatESLMgr->m_pDM_Appendix->TBL_NAME += _T(" , CO_ACCESSION_REC_TBL A ");
	}

	ids = m_pCatESLMgr->m_pDM_Book->RefreshDataManager(strOption);
	if (ids < 0) return ids;
	
	m_pCatESLMgr->m_pDM_Book->TBL_NAME = strTbl;

	CString strHoldWorkingStatus = _T("'SEL212N', 'SEL213O', 'SEL214O', 'SEL215O', ")
								   _T("'SEL311O', 'SEL312O', 'SEL313O', 'SEL314O', 'SEL315O', 'SEL411O', 'SEL511O', 'SEL611O'");
	
	strOption.Format(_T("B.SPECIES_KEY = %s AND B.WORKING_STATUS IN (%s)"), strSpeciesKey, strHoldWorkingStatus);
	ids = m_pCatESLMgr->m_pDM_HoldBook->RefreshDataManager(strOption);
	if (ids < 0) return ids;

	strOption.Format(_T("B.SPECIES_KEY = %s AND %s"), strSpeciesKey, m_strBookWhere);
	ids = m_pCatESLMgr->m_pDM_Appendix->RefreshDataManager(strOption);
	if (ids < 0) return ids;
	m_pCatESLMgr->m_pDM_Appendix->TBL_NAME = strTbl2;

	strOption.Format(_T("B.SPECIES_KEY = %s AND B.WORKING_STATUS IN (%s)"), strSpeciesKey, strHoldWorkingStatus);
	ids = m_pCatESLMgr->m_pDM_HoldAppendix->RefreshDataManager(strOption);
	if (ids < 0) return ids;
	
	ids = m_pCatApi->SyncVolumeBookAppendixDM(m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_HoldBook, m_pCatESLMgr->m_pDM_Appendix, m_pCatESLMgr->m_pDM_HoldAppendix);

	((CCatApi_SE*)m_pCatApi)->CopyDM(m_pCatESLMgr->m_pDM_Species, 0, m_pCatESLMgr->m_pDM_Index, 0, _T("ISS"));

	CString strCodeValue;
	
	if (m_pCatESLMgr->m_pDM_Index->GetCellData(_T("ISS_분류표구분"), 0) == _T("") )
	{
		m_pCatESLMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("분류표구분"), strCodeValue);
		m_pCatESLMgr->m_pDM_Index->SetCellData(_T("ISS_분류표구분"), strCodeValue, 0);
	}

	return 0;

	EFS_END
	return -1;
}

UINT CMarcAdjMgr_SE::OnCodeUpdate()
{
	EFS_BEGIN

	INT idx = -1;

#ifdef _CAT_PROGRAM_MODE_UNIV_
	INT nBookRowCount = m_pCatESLMgr->m_pDM_Book->GetRowCount();
	INT nAppendixRowCount = m_pCatESLMgr->m_pDM_Book->GetRowCount();
	
	CString strUsageCode;
	m_pResourceMgr->m_dlgCode.m_pCM->GetControlMgrData(_T("ISS_용도구분_코드"), strUsageCode);
	for (idx = 0; idx < nBookRowCount; idx++)
	{
		m_pCatESLMgr->m_pDM_Book->SetCellData(_T("SB_용도구분_코드"), strUsageCode, idx);
	}
	for (idx = 0; idx < nAppendixRowCount; idx++)
	{
		m_pCatESLMgr->m_pDM_Appendix->SetCellData(_T("SB_용도구분_코드"), strUsageCode, idx);
	}
#endif

	ApplyDMChange(true);

	m_pResourceMgr->m_dlgVolume.Display();
	return 0;

	EFS_END
	return -1;
}

BOOL CMarcAdjMgr_SE::OnCommand(UINT nID)
{
	EFS_BEGIN

	if (m_pFrameWnd == NULL) return FALSE;

	m_bAcceptMessage = FALSE;

	CString strCommandAlias;

	BOOL bReturn = m_pResourceMgr->OnCommand(nID, strCommandAlias);

	if (strCommandAlias.IsEmpty()) return bReturn;

	m_pResourceMgr->GetMarcEditor()->ApplyEdit();
	((CCatApi_SE*)m_pCatApi)->ConvertMarcToCatDM(m_pMarc, m_pCatESLMgr->m_pDM_Species, m_pCatESLMgr->m_pDM_Index, m_pCatESLMgr->m_pDM_Book, m_pCatESLMgr->m_pDM_Volume, m_pCatESLMgr->m_pDM_Purchase);

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
	else if (strCommandAlias == _T("국가자료공동목록반입"))
		ids = CMD_UCImport();
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

INT CMarcAdjMgr_SE::CMD_AcqusitNo()
{
	EFS_BEGIN

	CBO_CAT_ACQUISIT_NO dlg(1, m_pCatESLMgr);
	
	CString sTmpClassNo, sTmpShelfCode;
	sTmpClassNo = m_pCatESLMgr->m_pDM_Book->GetCellData(_T("SB_분류기호"), 0);
	sTmpShelfCode = m_pCatESLMgr->m_pDM_Book->GetCellData(_T("SB_별치기호_코드"), 0);

	dlg.SetClassNo ( sTmpClassNo, sTmpShelfCode );

	dlg.DoModal();

	return 0;

	EFS_END
	return -1;
}

UINT CMarcAdjMgr_SE::OnMarcEditUpdate()
{
	EFS_BEGIN

	if (m_bAcceptMessage)
		return ApplyMarcChange();
	else
		return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::CMD_GiveRegNo()
{
	EFS_BEGIN

	ApplyData();

	CSE_CAT_REGNO_DLG dlg(m_pCatESLMgr);
	dlg.m_pRegNoArray = &m_arrRegNo;
	dlg.DoModal();

	m_pResourceMgr->m_dlgVolume.Display();
	ApplyDMChange();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr_SE::CMD_UseCoverIMG()
{
	return 0;
}

BOOL CMarcAdjMgr_SE::CMD_SubjectIndex()
{
	CMarcEditor *pMarcEditor = m_pResourceMgr->GetMarcEditor();

	CCSS_2_01 dlg(m_pCatESLMgr);
	dlg.SetMarcInfo(m_pMarcMgr, m_pMarc);

	if ( dlg.DoModal() == IDOK )
		pMarcEditor->Display();

	return TRUE;
}