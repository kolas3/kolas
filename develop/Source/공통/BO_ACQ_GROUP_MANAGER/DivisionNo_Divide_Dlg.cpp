// DivisionNo_Divide_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "DivisionNo_Divide_Dlg.h"
#include "DivisionNo_Select_Dlg.h"
#include "../BO_SPECIES_API/GroupInfo.h"
#include "GroupPair.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_Divide_Dlg dialog


CDivisionNo_Divide_Dlg::CDivisionNo_Divide_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDivisionNo_Divide_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDivisionNo_Divide_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pParentDM = NULL;
	m_pParentGrid = NULL;
	m_nIdx = -1;
	m_nFirstIdx = -1;
	m_nLastIdx = -1;
	RemoveAllSelectDataArray();
	m_nRow = -1;
	m_pSortItemSet = NULL;
}

CDivisionNo_Divide_Dlg::~CDivisionNo_Divide_Dlg()
{
	RemoveAllSelectDataArray();
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}
}

VOID CDivisionNo_Divide_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDivisionNo_Divide_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDivisionNo_Divide_Dlg, CDialog)
	//{{AFX_MSG_MAP(CDivisionNo_Divide_Dlg)
	ON_BN_CLICKED(IDC_btnDIVNO_DIVIDE_PREV, OnbtnDIVNODIVIDEPREV)
	ON_BN_CLICKED(IDC_btnDIVNO_DIVIDE_NEXT, OnbtnDIVNODIVIDENEXT)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnDATA_SORT, OnbtnDATASORT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_Divide_Dlg message handlers

BOOL CDivisionNo_Divide_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("K2UPGRADE_구입차수분리")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	GetWindowRect(m_rcInitial);

	m_pCM = FindCM(_T("CM_구입차수분리"));
	m_pDM = FindDM(_T("DM_구입차수분리"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	InitSortItem();
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	Display(0);

	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_edtDIVNO_DIVIDE_WORKSTATUS);
	pEdit->ShowWindow(SW_HIDE);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;
}

VOID CDivisionNo_Divide_Dlg::OnOK() 
{
	INT ids = CheckSelectDataValid();
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	
	CString strWorkStatrus;
	m_pCM->GetControlMgrData(_T("작업상태"), strWorkStatrus);
	CDivisionNo_Select_Dlg dlg(this);
	dlg.SetAcqYear(m_pParentDM->GetCellData(_T("수입년도"), m_nIdx));
	dlg.SetWorkStatus(strWorkStatrus);
	dlg.SetAllowNULLGroup(FALSE);
	if( IDOK == dlg.DoModal() )
	{
		CGroupInfo * pGroup = dlg.GetSelGroup();
		if(CheckIsSameGroup(pGroup->GetAcqYear(), pGroup->GetGroupInfo(), pGroup->GetWorkCode()))
		{
			ESLAfxMessageBox(_T("동일그룹을 선택하였습니다. 다시 선정해 주십시요."));
			return ;
		}
		
		if( CheckGroupWorkCode(pGroup->GetWorkCode()) )
		{
			ESLAfxMessageBox(_T("차수분리는 동일 업무구분을 갖는 차수에 한하여 수행됩니다."));
			return;
		}

		ids = MakeSelectDataArray(m_pDM, m_pGrid, _T("BP_수서정보KEY"));
		if(ids < 0) return ;
		
		CGroupPair mgr(this);
		mgr.SetTargetGroup(pGroup);
		mgr.SetSelectDataArray(&m_ArrSelData);
		mgr.SetGroupType(DIVNO);
		mgr.SetMainDM(m_pParentDM);
		ids = mgr.DivideGroup();
		if( ids < 0 )	ESLAfxMessageBox(_T("선정한 자료를 분리할 수 없습니다."));
		else 
		{
			ESLAfxMessageBox(_T("선정한 자료를 분리하였습니다."));
			DeleteSelectedRow();
			CGroupInfo Group;
			Group.SetAcqYear(m_pParentDM->GetCellData(_T("수입년도"), m_nIdx));
			Group.SetGroupInfo(m_pParentDM->GetCellData(_T("차수번호"), m_nIdx));
			mgr.UpdateGroupBookCnt(&Group);
			m_pGrid->Display();
		}
	}
}

VOID CDivisionNo_Divide_Dlg::OnbtnDIVNODIVIDEPREV() 
{
	// TODO: Add your control notification handler code here
	RemoveAllSelectDataArray();
	Display(-1);
}

VOID CDivisionNo_Divide_Dlg::OnbtnDIVNODIVIDENEXT() 
{
	// TODO: Add your control notification handler code here
	RemoveAllSelectDataArray();
	Display(1);
}

VOID CDivisionNo_Divide_Dlg::Display(INT idx)
{
	INT ids;
	CString errstr;
	switch(idx)
	{
	case 0:
		InitFirstLastIndex();
		m_pParentGrid->SelectMakeList();
		ids = m_pParentGrid->SelectGetHeadPosition();
		ShowNextPrevButton(ids);
		if (ids < 0)
		{
			ESLAfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
			EndDialog(IDCANCEL);
			return;
		}
		break;
	case -1:
		ids = m_pParentGrid->SelectGetPrev();
		ShowNextPrevButton(ids);
		if (ids == -1) return;
		break;
	case 1:
		ids = m_pParentGrid->SelectGetNext();
		ShowNextPrevButton(ids);
		if (ids == -1) return;
		break;
	}

	INT res = MakeCurSelGroupData(m_pParentDM->GetCellData(_T("수입년도"), ids), m_pParentDM->GetCellData(_T("차수번호"), ids), m_pParentDM->GetCellData(_T("업무구분"), ids));
	if( res < 0 )
	{
		ESLAfxMessageBox(_T("그룹데이터를 구축할 수 없습니다."));
		return ;
	}
	m_pCM->AllControlDisplay(ids);
	m_pGrid->Display();
	m_nIdx = ids;
	m_pCM->SetControlMgrData(_T("작업상태"), m_strWorkingStatus);
}

VOID CDivisionNo_Divide_Dlg::InitFirstLastIndex()
{
	INT ids = m_pParentGrid->SelectMakeList();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	m_nFirstIdx = m_pParentGrid->SelectGetHeadPosition();
	m_nLastIdx = m_pParentGrid->SelectGetTailPosition();
}

VOID CDivisionNo_Divide_Dlg::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnDIVNO_DIVIDE_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnDIVNO_DIVIDE_NEXT)->EnableWindow(TRUE);

	if(m_nFirstIdx == idx || idx < 0)
		GetDlgItem(IDC_btnDIVNO_DIVIDE_PREV)->EnableWindow(FALSE);
	if(m_nLastIdx == idx || idx < 0)
		GetDlgItem(IDC_btnDIVNO_DIVIDE_NEXT)->EnableWindow(FALSE);
}

INT CDivisionNo_Divide_Dlg::MakeCurSelGroupData(CString strAcqYear, CString strGroupNo, CString strWorkCode)
{
	CString strQuery;
	if(strWorkCode.IsEmpty())
	{
		strQuery.Format(
			_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND I.WORK_CODE IS NULL AND P.REC_KEY IN (SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE ACQ_CODE='1' AND ACQ_YEAR='%s' AND SEQ_NO=%s) ORDER BY P.ORDER_SEQ_NO, P.ORDER_WORK_NO, P.ORDER_SERIAL_NO"),
			strAcqYear, strGroupNo);
	}
	else 
	{
		strQuery.Format(
			_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = P.SPECIES_KEY AND I.WORK_CODE = '%s' AND P.REC_KEY IN (SELECT ACQ_KEY FROM BO_BOOK_TBL WHERE ACQ_CODE='1' AND ACQ_YEAR='%s' AND SEQ_NO=%s)  ORDER BY P.ORDER_SEQ_NO, P.ORDER_WORK_NO, P.ORDER_SERIAL_NO"),
			strWorkCode, strAcqYear, strGroupNo);
	}
	INT ids = m_pDM->RefreshDataManager(strQuery);
	return ids;
}

INT CDivisionNo_Divide_Dlg::CheckSelectDataValid()
{
	if(m_pDM->GetRowCount() == 0) return -1;
	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return ids;
	INT i = m_pGrid->SelectGetHeadPosition();
	if(i < 0) return i;
	return 0;
}

VOID CDivisionNo_Divide_Dlg::SetWorkingStatus(CString strWorkStatus)
{
	m_strWorkingStatus= strWorkStatus;
}

BOOL CDivisionNo_Divide_Dlg::CheckIsSameGroup(CString strAcqYear, CString strGroupNo, CString strWorkCode)
{
	CString strCurAcqYear, strCurGroupNo, strCurWorkCode;
	strCurAcqYear = m_pParentDM->GetCellData(_T("수입년도"), m_nIdx);
	strCurGroupNo = m_pParentDM->GetCellData(_T("차수번호"), m_nIdx);
	strCurWorkCode = m_pParentDM->GetCellData(_T("업무구분"), m_nIdx);
	if(strCurGroupNo == strGroupNo && strCurAcqYear == strAcqYear && strWorkCode == strCurWorkCode)
		return TRUE;
	return FALSE;
}

INT CDivisionNo_Divide_Dlg::GetSelectDataKeys(CStringArray *pArray)
{
	INT ids = m_pGrid->SelectMakeList();
	INT i = m_pGrid->SelectGetHeadPosition();
	pArray->RemoveAll();
	CString strTmpData;
	while(i >= 0)
	{
		strTmpData = m_pDM->GetCellData(_T("BP_수서정보KEY"), i);
		pArray->Add(strTmpData);
		i = m_pGrid->SelectGetNext();
	}
	return 0;
}

INT CDivisionNo_Divide_Dlg::DeleteSelectedRow()
{
	INT ids = m_pGrid->SelectMakeList();
	INT i = m_pGrid->SelectGetTailPosition();
	while(i>=0)
	{
		m_pDM->DeleteRow(i);
		i = m_pGrid->SelectGetPrev();
	}
	return 0;
}

INT CDivisionNo_Divide_Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData, strWorkCode;
	INT nGroupNo, nWorkNo, nSerialNo;
	if(m_nRow != nRow)
	{
		CMarc marc;
		INT ids;
		strTmpData = m_pDM->GetCellData(_T("BS_MARC"), nRow);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("UDF_총서사항"), strTmpData, nRow);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("IBS_판사항"), strTmpData, nRow);
		strTmpData.Empty();	 strTmpData = m_pDM->GetCellData(_T("IBS_본표제"), nRow);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("UDF_표제사항"), strTmpData, nRow);
		ids = m_MarcConvertMgr.GetPublishPlace(&marc, strTmpData);
		if(ids >= 0) m_pDM->SetCellData(_T("UDF_발행지"), strTmpData, nRow);
		m_nRow = nRow;
		if(m_nRow == m_pDM->GetRowCount()-1) m_nRow = -1;
	}
	
	if(strDMFieldAlias == _T("UDF_차수정보"))
	{	
		strAcqYear = m_pDM->GetCellData(_T("BP_수입년도"), nRow);
		strWorkCode = m_pDM->GetCellData(_T("IBS_업무구분"), nRow);
		strGroupNo = m_pDM->GetCellData(_T("BP_그룹번호"), nRow);
		nGroupNo = _ttoi(strGroupNo.GetBuffer(0));
		strWorkNo = m_pDM->GetCellData(_T("BP_작업번호"), nRow);
		nWorkNo = _ttoi(strWorkNo.GetBuffer(0));
		strSerialNo = m_pDM->GetCellData(_T("BP_일련번호"), nRow);
		nSerialNo = _ttoi(strSerialNo.GetBuffer(0));
		str.Format(_T("%s-%s-%04d-%03d-%05d"), strAcqYear, strWorkCode, nGroupNo, nWorkNo, nSerialNo);// = strAcqYear+_T("-")+strGroupNo+_T("-")+strWorkNo+_T("-")+strSerialNo;
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("UDF_구입처부호설명"))
	{
		strTmpData = m_pDM->GetCellData(_T("BP_구입처부호"), nRow);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("구입처관리"), strTmpData, str);
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	else if(strDMFieldAlias == _T("IBS_자료상태"))
	{
		if(str == _T("BOT111O")) str = _T("기초자료");
		else if(str == _T("BOT112O")) str = _T("반입자료");
		else if(str == _T("BOT211O")) str = _T("수서복본추기자료");
		else if(str == _T("BOT212O")) str = _T("정리복본추기자료");
		else if(str == _T("BOA111N")) str = _T("구입대상자료");
		else if(str == _T("BOA112N")) str = _T("주문자료");
		else if(str == _T("BOA113O")) str = _T("미납품자료");
		else if(str == _T("BOA114O")) str = _T("구입인계대상자료");
		else if(str == _T("BOA211O")) str = _T("기증접수자료");
		else if(str == _T("BOA212O")) str = _T("기증미등록자료");
		else if(str == _T("BOR111N")) str = _T("등록인계자료");
		else if(str == _T("BOR112N")) str = _T("등록자료");
		else if(str == _T("BOR113O")) str = _T("가원부번호부여자료");
		else if(str == _T("BOC111O")) str = _T("복본조사대상자료");
		else if(str == _T("BOC112O")) str = _T("분류대상자료");
		else if(str == _T("BOC113N")) str = _T("정리대상자료");
		else if(str == _T("BOC114O")) str = _T("열람인계대상자료");
		else if(str == _T("BOL111O")) str = _T("열람인계자료");
		else if(str == _T("BOL112N")) str = _T("배가자료");
		else if(str == _T("BOL113O")) str = _T("수리제본자료");
		else if(str == _T("BOL114O")) str = _T("파손자료");
		else if(str == _T("BOL115O")) str = _T("제적자료");
		else if(str == _T("BOL211O")) str = _T("관외대출자료");
		else if(str == _T("BOL212O")) str = _T("관내대출자료");
		else if(str == _T("BOL411O")) str = _T("단행책두레업무진행자료");
		else if(str == _T("BOL511O")) str = _T("통합도서타관반납");
		else if(str == _T("BOL611O")) str = _T("통합도서타관대출");
		else if(str == _T("BOL213O")) str = _T("특별대출자료");
		else if(str == _T("BOL214O")) str = _T("긴급대출자료");
		else if(str == _T("BOL215O")) str = _T("배달자료");
		else if(str == _T("BOL116O")) str = _T("재정리자료");
	}	
	else if(strDMFieldAlias == _T("BP_주제구분"))
	{
		strTmpData = m_pDM->GetCellData(_T("IBS_분류표구분"), nRow);
		if(strTmpData.IsEmpty()) str = _T("");
		else if(strTmpData == _T("1"))
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("KDC대주제부호관리"), str, str);
		}
		else if(strTmpData == _T("2"))
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("DDC대주제부호관리"), str, str);
		}
	}
	return 0;
}

BOOL CDivisionNo_Divide_Dlg::CheckGroupWorkCode(CString strWorkCode)
{
	CString strCurWorkCode;
	strCurWorkCode = m_pParentDM->GetCellData(_T("업무구분"), m_nIdx);
	if( strCurWorkCode != strWorkCode )		return TRUE;

	return FALSE;
}

INT CDivisionNo_Divide_Dlg::MakeSelectDataArray(CESL_DataMgr *pDM, CESL_Grid *pGrid, CString strAlias)
{
	if(pGrid == NULL || pDM == NULL || strAlias.IsEmpty()) return -1;
	
	RemoveAllSelectDataArray();

	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT i = pGrid->SelectGetHeadPosition();
	
	CStringArray * pArrRecKey = new CStringArray;
	pArrRecKey->RemoveAll();
	
	INT nCnt = 0;
	CString strRecKey;
	while(i >= 0)
	{
		++nCnt;
		strRecKey = pDM->GetCellData(strAlias, i);
		if(MAX_REC_KEY_CNT == nCnt)
		{
			m_ArrSelData.Add(pArrRecKey);
			pArrRecKey = new CStringArray;
			pArrRecKey->Add(strRecKey);
			nCnt = 1;
		}
		else 
		{
			pArrRecKey->Add(strRecKey);
		}

		i = pGrid->SelectGetNext();
	}
	m_ArrSelData.Add(pArrRecKey);
	return 0;
}

INT CDivisionNo_Divide_Dlg::RemoveAllSelectDataArray()
{
	INT nCnt = m_ArrSelData.GetSize();
	if(nCnt == 0) return 0;
	for(INT i=0 ; i<nCnt ; i++)
	{
		delete m_ArrSelData.GetAt(i);
	}

	m_ArrSelData.RemoveAll();
	return 0;
}

VOID CDivisionNo_Divide_Dlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_pGrid->GetSafeHwnd() == NULL) return;
	CRect rcWnd;
	GetWindowRect(rcWnd);
	
	if (rcWnd.Width() < m_rcInitial.Width())
	{
		rcWnd.right = rcWnd.left + m_rcInitial.Width();
		if (rcWnd.Height() < m_rcInitial.Height())
			rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		
		MoveWindow(rcWnd);
		return;
	}
	
	if (rcWnd.Height() < m_rcInitial.Height())
	{
		rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		if (rcWnd.Width() < m_rcInitial.Width())
			rcWnd.right = rcWnd.left + m_rcInitial.Width();			
		
		MoveWindow(rcWnd);
		return;
	}		
	CRect rect;
	GetClientRect(rect);
	rect.top += 58;
	rect.bottom -= 4;
	rect.left += 4;
	rect.right -= 4;
	
	m_pGrid->MoveWindow(rect);		
}

VOID CDivisionNo_Divide_Dlg::OnbtnDATASORT() 
{
	// TODO: Add your control notification handler code here
	if(m_pSortItemSet == NULL || m_pDM == NULL) 
	{
		ESLAfxMessageBox(_T("정렬화면을 초기화 할 수 없습니다."));
		return;
	}
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("정렬할 자료가 없습니다."));
		return ;
	}
	
	CDataSortDlg dlg(this);
	dlg.SetMainDM(m_pDM);
	dlg.SetSortItemSet(m_pSortItemSet);
	dlg.SetOpenMode(1);
	if(IDOK == dlg.DoModal())
		m_pGrid->Display();
}

BOOL CDivisionNo_Divide_Dlg::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_정렬항목설정"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("BP_수입년도"));
	pTmpData->Add(_T("IBS_업무구분"));
	pTmpData->Add(_T("BP_그룹번호"));
	pTmpData = m_pSortItemSet->GetTailSortList();
	pTmpData->Add(_T("BP_작업번호"));
	pTmpData->Add(_T("BP_일련번호"));
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_주제구분설명"), _T("BP_주제구분"));
	pMap->SetAt(_T("UDF_입력구분설명"), _T("BP_입력구분"));
	pMap->SetAt(_T("UDF_구입처부호설명"), _T("BP_구입처부호"));
	
	m_pSortItemSet->ReadSortItem();
	return TRUE;
}

HBRUSH CDivisionNo_Divide_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
