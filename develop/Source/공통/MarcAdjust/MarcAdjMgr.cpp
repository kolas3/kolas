// MarcAdjMgr.cpp: implementation of the CMarcAdjMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <direct.h>
#include "MarcAdjMgr.h"

#include "..\..\ESL\Marc\MarcToolKit\MarcDefaultValueConfigDlg.h"

#include "..\..\공통\BO_VOLUME_FORM\BOVolumeDlg.h"

#include "MarcAdjFrame.h"

#include "..\..\단행\정리\BO_CAT_PROC_KEYWORD\BO_CAT_PROC_KEYWORD_0000.h"

#include "..\..\관리\정리\CatCodeGetter\CatCodeGetterDlg.h"

#include "..\..\단행\정리\BO_CAT_REGNO_DLG\BO_CAT_REGNO_DLG1.h"

#include "MarcAdjMgrMemoDlg.h"

#include "..\..\관리\문자변환관리\MngCharSetManagement\CatCharSetManagementDlg.h"

#include "..\..\공통\BO_SPECIES_API\BO_SPECIES.h"

#include "..\..\관리\전거\MNG_AC_MANAGER\ACMARCGetterDlg.h"

#include "..\..\단행\정리\WEB_HOLD_IMPORT\WebHoldImport.h"

#include "..\..\공통\COVERMANAGER\COVER_MAIN.h"

#include "MarcToolBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMarcAdjMgr::CMarcAdjMgr()
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

CMarcAdjMgr::~CMarcAdjMgr()
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

//////////////////////////////////////////////////////////////////////
// CMarcAdjMgr Operations

INT CMarcAdjMgr::InitManager(INT nManageMode, CESL_Mgr *pParentESLMgr, LPCTSTR lpszWorkingStatus /*= NULL*/, CMarc *pMarc /*= NULL*/)
{
	return 0;
}


INT CMarcAdjMgr::MakeSelectListParentGrid()
{
	EFS_BEGIN

	INT ids = -1;

	ids = m_pGrid_Parent->SelectMakeList();
	if (ids < 0)
	{
		AfxMessageBox(_T("선택목록을 구성할 수 없습니다."));
		return -1;
	}
	
	if (m_nParentSelectedRow == -1)
	{
		m_nParentSelectedRow = m_pGrid_Parent->SelectGetHeadPosition();
		if (m_nParentSelectedRow < 0)
		{
			AfxMessageBox(_T("선택목록을 구성할 수 없습니다."));
			return -1;
		}
	}
	else
	{
		POSITION pos = m_pGrid_Parent->m_arraySelect.GetHeadPosition();
		while (pos)
		{
			m_pGrid_Parent->m_arrayPos = pos;
			if (m_nParentSelectedRow == m_pGrid_Parent->m_arraySelect.GetNext(pos))
				break;				
		}
		
		if (m_pGrid_Parent->m_arrayPos == NULL)
		{
			m_pGrid_Parent->SelectRemoveAll();
			INT nRowCount = m_pGrid_Parent->GetCount();
			for (INT idx = 0; idx < nRowCount; idx++)
				m_pGrid_Parent->m_arraySelect.AddTail(idx);
		}
		
		if (m_pGrid_Parent->m_arraySelect.GetCount() > 0)
			m_pGrid_Parent->m_arrayPos = m_pGrid_Parent->m_arraySelect.GetHeadPosition();
		else
		{
			AfxMessageBox(_T("선택목록을 구성할 수 없습니다."));
			return -1;
		}
		
		pos = m_pGrid_Parent->m_arraySelect.GetHeadPosition();
		while (pos)
		{
			m_pGrid_Parent->m_arrayPos = pos;
			if (m_nParentSelectedRow == m_pGrid_Parent->m_arraySelect.GetNext(pos))
				break;				
		}
	}
	
	m_pGrid_Parent->SetReverse(m_nParentSelectedRow);
	
	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr::SetParentInfo(CESL_DataMgr *pDM_Parent, CESL_Grid *pGrid_Parent, CString strSpeciesKeyAlias, CString strBookWhere, INT nSelectedGridRow /*= -1*/, BOOL bCatHold /*= FALSE*/)
{
	EFS_BEGIN

	m_pDM_Parent = pDM_Parent;
	m_pGrid_Parent = pGrid_Parent;
	m_strSpeciesKeyAlias = strSpeciesKeyAlias;
	m_strBookWhere = strBookWhere;
	m_nParentSelectedRow = nSelectedGridRow;
	m_bCatHold = bCatHold;

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr::SetParentInfo(CESL_DataMgr *pDM_Species_Parent, CESL_DataMgr *pDM_Index_Parent, CESL_Grid *pGrid_Parent, INT nSelectedGridRow /*= -1*/, BOOL bCatHold /*= FALSE*/)
{
	EFS_BEGIN

	m_pDM_Species_Parent = pDM_Species_Parent;
	m_pDM_Index_Parent = pDM_Index_Parent;
	m_pGrid_Parent = pGrid_Parent;
	m_nParentSelectedRow = nSelectedGridRow;
	m_bCatHold = bCatHold;

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr::CreateDefaultMarc(CMarc *pMarc)
{
	EFS_BEGIN

	if (pMarc == NULL) return -1;

	CString strFormCode;

	m_pCatESLMgr->m_pInfo->pCodeMgr->GetDefaultCode(_T("형식구분"),strFormCode);

	CMarcDefaultValueConfigDlg dlg(m_pCatESLMgr);
	dlg.SetMarcGroupFormCode(strFormCode, m_pMarcMgr->GetMarcGroup());
	INT ids = -1;

	ids = dlg.LoadDefaultMarc();
	if (ids < 0) return ids;

	CMarc *pMarcTemp;

	ids = dlg.GetUserMarc(dlg.GetGroup(), &pMarcTemp);
	if (ids >= 0)
		*pMarc = *pMarcTemp;

	return 0;

	EFS_END
	return -1;
}

BOOL CMarcAdjMgr::IsFirstMarcAdjust()
{
	return TRUE;
}

INT CMarcAdjMgr::ApplyDefaultMarc(CMarc *pMarc, LPCTSTR lpszFormCode)
{
	EFS_BEGIN

	if (!pMarc) return -1;

	CMarcDefaultValueConfigDlg dlg(m_pCatESLMgr);
	dlg.SetMarcGroupFormCode(lpszFormCode, m_pMarcMgr->GetMarcGroup());
	INT ids = -1;

	ids = dlg.LoadDefaultMarc();
	if (ids < 0) return ids;

	CMarc *pMarcTemp;

	ids = dlg.GetUserMarc(dlg.GetGroup(), &pMarcTemp);
	if (ids < 0) return ids;

	CTag *pTag = NULL;
	CSubfield *pSubfield = NULL;
	CString strTagCode;
	CString strSubfieldCode;
	CString strItemData;
	POSITION posTag = NULL;
	POSITION posSubfield = NULL;
	posTag = pMarcTemp->m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = (CTag*)pMarcTemp->m_listTag.GetNext(posTag);
		if (!pTag) continue;

		strTagCode = pTag->GetTagCode();

		if (m_pMarcMgr->IsControlField(strTagCode))
		{
			if (pMarc->FindTag(strTagCode)) 
				continue;
			else
				pMarc->AddTag(pTag->Clone());
		}
		else
		{
			posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield)
			{
				pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (!pSubfield) continue;

				strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');

				if (m_pMarcMgr->GetItem(pMarc, strTagCode + strSubfieldCode, strItemData) >= 0) continue;

				strItemData = pSubfield->GetItemData();
				m_pMarcMgr->SetItem(pMarc, strTagCode + strSubfieldCode, strItemData);
			}
		}	
	}

	m_pMarcMgr->CheckSimpleSyntax(pMarc);

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr::CreateCatESLMgr(CESL_Mgr *pParentESLMgr)
{
	return 0;
}

INT CMarcAdjMgr::MakeUpdateDM()
{
	return 0;
}

INT CMarcAdjMgr::MakeImportDM()
{
	return 0;
}

INT CMarcAdjMgr::PrevDataCopy()
{
	return 0;
}
INT CMarcAdjMgr::InitMarcEditor()
{
	return 0;
}

INT CMarcAdjMgr::CreateResource(CFrameWnd *pFrameWnd)
{
	return 0;
}

INT CMarcAdjMgr::Display(bool m_bInit /*= false*/)
{
	return 0;
}

BOOL CMarcAdjMgr::ISChangeMarc()
{
	CString strNewEditMarc;

	m_pResourceMgr->GetMarcEditor()->GetEditCtrl()->GetWindowText(strNewEditMarc);

	if ( strNewEditMarc == m_oldEditMarc ) return FALSE;

	return TRUE;	
}

VOID CMarcAdjMgr::BackupOldMarc()
{
	m_pResourceMgr->GetMarcEditor()->GetEditCtrl()->GetWindowText(m_oldEditMarc);

	return;
}

INT CMarcAdjMgr::ApplyFiexedFieldMaintenance()
{
	if (!m_strLeaderPrev.IsEmpty())
	{
		CString strLeader;
		m_pMarc->GetLeader(strLeader);

		TCHAR tch = 0;

		tch = m_strLeaderPrev.GetAt(5);
		strLeader.SetAt(5, tch);

		tch = m_strLeaderPrev.GetAt(6);
		strLeader.SetAt(6, tch);

		tch = m_strLeaderPrev.GetAt(7);
		strLeader.SetAt(7, tch);

		m_pMarc->SetLeader(strLeader.GetBuffer(0));
	}

	if (!m_str007FieldPrev.IsEmpty())
	{
		m_pMarcMgr->DeleteField(m_pMarc, _T("007"));

		m_pMarcMgr->InsertField(m_pMarc, _T("007") + m_str007FieldPrev);
	}

	if (!m_str008FieldPrev.IsEmpty())
	{
		m_pMarcMgr->DeleteField(m_pMarc, _T("008"));

		m_pMarcMgr->InsertField(m_pMarc, _T("008") + m_str008FieldPrev);
	}

	return 0;
}

INT CMarcAdjMgr::ApplyDMChange(bool m_bInit /*=false */)
{
	return 0;
}

INT CMarcAdjMgr::ApplyMarcChange()
{
	return 0;
}

UINT CMarcAdjMgr::OnMarcGridUpdate()
{
	EFS_BEGIN

	if (m_bAcceptMessage)
		return ApplyMarcChange();
	else
		return 0;

	EFS_END
	return -1;
}

UINT CMarcAdjMgr::OnMarcEditUpdate()
{
	return 0;
}

UINT CMarcAdjMgr::OnCodeUpdate()
{
	return 0;
}

BOOL CMarcAdjMgr::OnCmdMsg(UINT nID)
{
	EFS_BEGIN

	if (m_pFrameWnd == NULL) return FALSE;

	return m_pResourceMgr->OnCmdMsg(nID);

	EFS_END
	return FALSE;
}

BOOL CMarcAdjMgr::OnCommand(UINT nID)
{
	return TRUE;
}

INT CMarcAdjMgr::CMD_VolumeInfo()
{
	return 0;
}

INT CMarcAdjMgr::CMD_AppendixInfo()
{
	return 0;
}

INT CMarcAdjMgr::CMD_SetDefaultValue()
{
	return 0;
}

INT CMarcAdjMgr::CMD_EditFixedField()
{
	return 0;
}

INT CMarcAdjMgr::CMD_EditFixedField2()
{	
	return 0;
}

INT CMarcAdjMgr::CMD_Buchul()
{
	EFS_BEGIN

	m_pMarcMgr->CheckDeepSyntax(m_pMarc);		
	m_pResourceMgr->GetMarcEditor()->Display();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr::CMD_DoDupSearch()
{
	return 0;
}

INT CMarcAdjMgr::CMD_CreateKeyWord()
{
	EFS_BEGIN

	CBO_CAT_PROC_KEYWORD_0000 dlg(m_pCatESLMgr, *m_pMarc);
	if (IDOK != dlg.DoModal()) return -1;

	*m_pMarc = dlg.m_marc;
	
	m_pResourceMgr->GetMarcEditor()->Display();

	CString strLeader;
	m_pMarc->GetLeader(strLeader);
	
	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr::CMD_CreateCallNo()
{
	return 0;
}

INT CMarcAdjMgr::CMD_SearchCallNo(BOOL bShowMsg /*= TRUE*/)
{
	return 0;
}

INT CMarcAdjMgr::CMD_MarcImport()
{
	return 0;
}

INT CMarcAdjMgr::CMD_UseCoverIMG()
{
	return 0;
}

INT CMarcAdjMgr::CMD_SupportClasification()
{
	return 0;
}

INT CMarcAdjMgr::CMD_UCImport()
{
	return 0;
}

INT CMarcAdjMgr::CMD_ShelfCodeSearch()
{
	return 0;
}

INT CMarcAdjMgr::CMD_WehHoldImport()
{
EFS_BEGIN
	CWebHoldImport dlg(m_pCatESLMgr);
	dlg.DoModal();
	return 0;
EFS_END
return -1;
}

INT CMarcAdjMgr::CMD_DMDPImport()
{
EFS_BEGIN
	CWebHoldImport dlg(m_pCatESLMgr);
	dlg.DoModal();
	return 0;
EFS_END
return -1;
}
INT CMarcAdjMgr::CMD_AuthorCodeSearch()
{
	EFS_BEGIN

	CCatCodeGetterDlg *pDlg = new CCatCodeGetterDlg(m_pCatESLMgr);
	if (pDlg->Create(m_pCatESLMgr))
	{
		pDlg->ShowWindow(SW_SHOW);
		pDlg->CenterWindow();
	}

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr::CMD_RoleWord()
{
	EFS_BEGIN

	CCatCharSetManagementDlg dlg(m_pCatESLMgr);
	dlg.SetCatagory(_T("역할어"));
	dlg.DoModal();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr::CMD_SubjectIndex()
{
	return 0;
}

INT CMarcAdjMgr::CMD_ClassCode()
{
	return 0;
}

INT CMarcAdjMgr::CMD_Create505Tag()
{
	return 0;
}

INT CMarcAdjMgr::CMD_GiveRegNoAuto()
{
	return 0;
}

INT CMarcAdjMgr::CMD_GiveRegNo()
{
	EFS_BEGIN

	ApplyData();

	CBO_CAT_REGNO_DLG dlg(m_pCatESLMgr);
	dlg.m_pRegNoArray = &m_arrRegNo;
	dlg.DoModal();

	m_pResourceMgr->m_dlgVolume.Display();
	ApplyDMChange();

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr::CMD_ContentsMgr()
{
	return 0;
}

INT CMarcAdjMgr::CMD_HoldInfo()
{
	return 0;
}

INT CMarcAdjMgr::CMD_Memo()
{
	EFS_BEGIN

	CMarcAdjMgrMemoDlg dlg(m_pCatESLMgr);
	dlg.SetSpeciesDM(m_pCatESLMgr->m_pDM_Species);
	dlg.SetMemoDMFieldAlias(_T("SS_메모"));
	dlg.DoModal();
	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr::CMD_PrevDataCopy()
{
	EFS_BEGIN

	PrevDataCopy();
	
	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr::CMD_Prev()
{
	EFS_BEGIN

	if ( ISChangeMarc() == TRUE )
	{
		if ( AfxMessageBox(_T("변경된 내용이 존재합니다.\r\n그래도 닫으시겠습니까?"), MB_YESNO) == IDNO )
		{
			return 0;
		}
	}

	if (m_nParentSelectedRow < 0) return -1;
	INT nIdx = m_pGrid_Parent->SelectGetPrev();
	if (nIdx >= 0)
		m_nParentSelectedRow = nIdx;
	else
		m_nParentSelectedRow = m_pGrid_Parent->SelectGetHeadPosition();

	m_pGrid_Parent->SetReverse(m_nParentSelectedRow);

	if ( m_arrRegNo.GetSize() >= 1 ) {
		m_pCatApi->InsertToDropAccessionNoTbl(&m_arrRegNo, m_pCatESLMgr->m_pDM_Species, TRUE);
	}
	m_arrRegNo.RemoveAll();

	if (GetManageMode() == MNG_MODIFY)
	{
		if (MakeUpdateDM() < 0)
		{
			AfxMessageBox(_T("UPDATE DM 생성 실패"));
			return -1;
		}
	}
	else if (GetManageMode() == MNG_IMPORT || GetManageMode() == MNG_IMPORT_WORKING)
	{
		if (MakeImportDM() < 0)
		{
			AfxMessageBox(_T("IMPORT DM 생성 실패"));
			return -1;
		}
	}

	return Display(true);

	EFS_END
	return -1;
}

INT CMarcAdjMgr::CMD_Next()
{
	EFS_BEGIN

	if ( ISChangeMarc() == TRUE )
	{
		if ( AfxMessageBox(_T("변경된 내용이 존재합니다.\r\n그래도 닫으시겠습니까?"), MB_YESNO) == IDNO )
		{
			return 0;
		}
	}

	if (m_nParentSelectedRow < 0) return -1;
	INT nIdx = m_pGrid_Parent->SelectGetNext();
	if (nIdx >= 0)
		m_nParentSelectedRow = nIdx;
	else
		m_nParentSelectedRow = m_pGrid_Parent->SelectGetTailPosition();

	m_pGrid_Parent->SetReverse(m_nParentSelectedRow);

	if ( m_arrRegNo.GetSize() >= 1 ) {
		m_pCatApi->InsertToDropAccessionNoTbl(&m_arrRegNo, m_pCatESLMgr->m_pDM_Species, TRUE);
	}
	m_arrRegNo.RemoveAll();
	
	if (GetManageMode() == MNG_MODIFY)
	{
		if (MakeUpdateDM() < 0)
		{
			AfxMessageBox(_T("UPDATE DM 생성 실패"));
			return -1;
		}
	}
	else if (GetManageMode() == MNG_IMPORT || GetManageMode() == MNG_IMPORT_WORKING )
	{
		if (MakeImportDM() < 0)
		{
			AfxMessageBox(_T("IMPORT DM 생성 실패"));
			return -1;
		}
	}

	return Display(true);

	EFS_END
	return -1;
}

INT CMarcAdjMgr::CMD_Save()
{
	return 0;
}

INT CMarcAdjMgr::CMD_Close()
{
	EFS_BEGIN

	if ( ISChangeMarc() == TRUE )
	{
		if ( AfxMessageBox(_T("변경된 내용이 존재합니다.\r\n그래도 닫으시겠습니까?"), MB_YESNO) == IDNO )
		{
			return 0;
		}
	}

	if ( m_arrRegNo.GetSize() >= 1 ) {
		m_pCatApi->InsertToDropAccessionNoTbl(&m_arrRegNo, m_pCatESLMgr->m_pDM_Species, TRUE);
	}
	m_arrRegNo.RemoveAll();

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO( m_pCatESLMgr->m_pDM_Book->CONNECTION_INFO);

	CString strQuery;
	CString strReturnValue;
	CString strSpeciesKey;

	if ( (m_pCatESLMgr->GetAppMode() == MNG_INSERT) )
	{
		strSpeciesKey.Empty();

		if (  m_pCatESLMgr->m_nModeMONO_SERIAL == MODE_MONO )
		{
			m_pCatESLMgr->m_pDM_Species->GetCellData(_T("BS_종KEY"), 0 , strSpeciesKey ) ;
		
			if ( !strSpeciesKey.IsEmpty() )
			{
				strQuery.Format(_T("SELECT REC_KEY FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY=%s"), strSpeciesKey);
				dm.GetOneValueQuery(strQuery, strReturnValue);	

				if ( strReturnValue != _T("") )
				{
					strQuery.Format(_T("DELETE FROM BO_CONTENTS_TBL WHERE TOC_COMMON_KEY = %s;DELETE FROM BO_ABSTRACTS_TBL WHERE TOC_COMMON_KEY = %s;DELETE FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY = %s;")	
						, strReturnValue, strReturnValue, strSpeciesKey	);					
					dm.ExecuteQuery(strQuery, TRUE);
					
				}
			}
		}	
	}
	
	strQuery = _T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USECOVERIMG'");
	dm.GetOneValueQuery(strQuery, strReturnValue);	

	if ( strReturnValue == _T("Y") && (m_pCatESLMgr->GetAppMode() == MNG_INSERT))
	{		
		strSpeciesKey.Empty();

		if (  m_pCatESLMgr->m_nModeMONO_SERIAL == MODE_MONO )
		{
			m_pCatESLMgr->m_pDM_Species->GetCellData(_T("BS_종KEY"), 0 , strSpeciesKey ) ;
		
			if ( !strSpeciesKey.IsEmpty() )
			{
				CCOVER_MAIN dlg(m_pCatESLMgr);
				dlg.DeleteCoverInfo ( strSpeciesKey.GetBuffer(0) ) ;
			}
		}
		else if ( m_pCatESLMgr->m_nModeMONO_SERIAL == MODE_SERIAL )
		{
			for(INT i=0; i<m_pCatESLMgr->m_pDM_Volume->GetRowCount(); i++)
			{
				m_pCatESLMgr->m_pDM_Volume->GetCellData(_T("SV_권KEY"), i , strSpeciesKey ) ;

				if ( !strSpeciesKey.IsEmpty() )
				{
					CCOVER_MAIN dlg(m_pCatESLMgr);
					dlg.DeleteCoverInfo ( strSpeciesKey.GetBuffer(0) ) ;
				
				}
			}
		}		
	}

	m_pFrameWnd->PostMessage(WM_CLOSE);

	return 0;

	EFS_END
	return -1;
}

INT CMarcAdjMgr::CreateAcqCallNo()
{
	return 0;
}

INT CMarcAdjMgr::ApplyMarcData()
{
	return 0;
}

INT CMarcAdjMgr::ApplyData()
{
	return 0;
}

INT CMarcAdjMgr::ApplyOneVolumeData(CString strVol, INT nMode)
{
	return 0;
}

INT CMarcAdjMgr::CMD_EbookView ()
{
EFS_BEGIN
    
	CBOFormApi api(m_pCatESLMgr) ;
    INT nIdx ;
	
    if (m_pCatApi->GetVolumeCount(m_pCatESLMgr->m_pDM_Book) == 1)
    {	
		api.GetRepresentationBook (m_pCatESLMgr->m_pDM_Book,nIdx) ;
		CString strBookKey = m_pCatESLMgr->m_pDM_Book->GetCellData (_T("BB_책KEY"),nIdx ) ;
		CString strExeFile = m_pCatESLMgr->m_pDM_Book->GetCellData (_T("BB_실행파일"),nIdx);
		CString strEbookFile = m_pCatESLMgr->m_pDM_Book->GetCellData(_T("BB_원본파일"),nIdx) ;

        api.EBookView (strBookKey,strExeFile,strEbookFile) ; 
    }
    else
	{
		CBOVolumeDlg dlg(m_pCatESLMgr, m_pCatESLMgr->m_pDM_Book);
		if (GetManageMode() == MNG_IMPORT)
			dlg.SetOpenMode(MNG_MODIFY);
		else
			dlg.SetOpenMode(GetManageMode());
            dlg.SetTabOrder (BOOK_FIRST) ;
		if (dlg.DoModal() != IDOK) return -1;
	}

	return 0 ;

EFS_END
	return -1;
}

UINT CMarcAdjMgr::OnACDlgOpen(INT iACCode)
{
EFS_BEGIN

	if ( iACCode < 0 ) return -1;

	CMarcEditor *pMarcEditor = m_pResourceMgr->GetMarcEditor();

	CACMARCGetterDlg dlg(m_pCatESLMgr);

	CString strTmpItem = _T("");
	if ( iACCode == 0 )
	{
		m_pMarcMgr->GetItem(m_pMarc, _T("245$d"), strTmpItem);
		if ( strTmpItem != _T("") )
			dlg.SetACInfo(m_pMarcMgr, m_pMarc, 1, strTmpItem);

		strTmpItem = _T("");
		m_pMarcMgr->GetItem(m_pMarc, _T("245$e"), strTmpItem);
		if ( strTmpItem != _T("") )
			dlg.SetACInfo(m_pMarcMgr, m_pMarc, 1, strTmpItem);
	}
	else if ( iACCode == 3 )
	{
		m_pMarcMgr->GetItem(m_pMarc, _T("245$a"), strTmpItem);
		if ( strTmpItem != _T("") )
			dlg.SetACInfo(m_pMarcMgr, m_pMarc, 2, strTmpItem);
		
		strTmpItem = _T("");
		m_pMarcMgr->GetItem(m_pMarc, _T("440$a"), strTmpItem);
		if ( strTmpItem != _T("") )
			dlg.SetACInfo(m_pMarcMgr, m_pMarc, 2, strTmpItem);

		strTmpItem = _T("");
		m_pMarcMgr->GetItem(m_pMarc, _T("830$a"), strTmpItem);
		if ( strTmpItem != _T("") )
			dlg.SetACInfo(m_pMarcMgr, m_pMarc, 2, strTmpItem);

	}
	else if ( iACCode == 4 )
	{
		dlg.SetACInfo(m_pMarcMgr, m_pMarc, 3, _T(""));
	}

	dlg.SetDMAlias(_T("DM_ACMARCGetterDlg_MASTER"));
	dlg.SetCMAlias(_T("CM_ACMARCGetterDlg"));
	
	if ( dlg.DoModal() == IDOK )
		pMarcEditor->Display();

	return 0;
EFS_END
return -1;
}

LRESULT CMarcAdjMgr::OnTouchPad(WPARAM wParam, LPARAM lParam)
{
	CMarcToolBar *pToolBar = NULL;
	POSITION pos = m_pResourceMgr->m_listToolBar.GetHeadPosition();		
	pToolBar = static_cast<CMarcToolBar*>(m_pResourceMgr->m_listToolBar.GetNext(pos));	
	if(NULL != pToolBar)
	{
		return pToolBar->GetToolBarItemID((LPCTSTR)wParam);
	}

	return 0;
}