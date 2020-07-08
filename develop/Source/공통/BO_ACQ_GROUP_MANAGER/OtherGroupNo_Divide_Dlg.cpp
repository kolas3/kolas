// OtherGroupNo_Divide_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "OtherGroupNo_Divide_Dlg.h"
#include "OtherGroupNo_Select_Dlg.h"
#include "GroupPair.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COtherGroupNo_Divide_Dlg dialog


COtherGroupNo_Divide_Dlg::COtherGroupNo_Divide_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(COtherGroupNo_Divide_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COtherGroupNo_Divide_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pDM_BOOK = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pParentDM = NULL;
	m_pParentGrid = NULL;
	m_nIdx = -1;
	m_nFirstIdx = -1;
	m_nLastIdx = -1;
	m_nRow = -1;
	m_pSortItemSet = NULL;
}

COtherGroupNo_Divide_Dlg::~COtherGroupNo_Divide_Dlg()
{
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}
	RemoveAllSelectDataArray();
}

VOID COtherGroupNo_Divide_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COtherGroupNo_Divide_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COtherGroupNo_Divide_Dlg, CDialog)
	//{{AFX_MSG_MAP(COtherGroupNo_Divide_Dlg)
	ON_BN_CLICKED(IDC_btnOTHER_GROUP_NO_DIVIDE_PREV, OnbtnOTHERGROUPNODIVIDEPREV)
	ON_BN_CLICKED(IDC_btnOTHER_GROUP_NO_DIVIDE_NEXT, OnbtnOTHERGROUPNODIVIDENEXT)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnDATA_SORT, OnbtnDATASORT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COtherGroupNo_Divide_Dlg message handlers

BOOL COtherGroupNo_Divide_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_수서그룹번호분리")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	GetWindowRect(&m_rcInitial);

	m_pCM = FindCM(_T("CM_수서그룹번호분리"));
	m_pDM = FindDM(_T("DM_수서그룹번호분리"));
	m_pDM_BOOK = FindDM(_T("DM_수서그룹번호분리_책"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	InitSortItem();
	Display(0);	
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID COtherGroupNo_Divide_Dlg::Display(INT idx)
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
			ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
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
	
	INT res = MakeCurSelGroupData(m_pParentDM->GetCellData(_T("수입구분"), ids), m_pParentDM->GetCellData(_T("수입년도"), ids), m_pParentDM->GetCellData(_T("그룹번호"), ids));
	if( res < 0 )
	{
		ESLAfxMessageBox(_T("그룹데이터를 구축할 수 없습니다."));
		return ;
	}
	m_pCM->AllControlDisplay(ids);
	m_pGrid->Display();
	m_nIdx = ids;
}

VOID COtherGroupNo_Divide_Dlg::InitFirstLastIndex()
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

VOID COtherGroupNo_Divide_Dlg::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnOTHER_GROUP_NO_DIVIDE_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnOTHER_GROUP_NO_DIVIDE_NEXT)->EnableWindow(TRUE);
	
	if(m_nFirstIdx == idx || idx < 0)
		GetDlgItem(IDC_btnOTHER_GROUP_NO_DIVIDE_PREV)->EnableWindow(FALSE);
	if(m_nLastIdx == idx || idx < 0)
		GetDlgItem(IDC_btnOTHER_GROUP_NO_DIVIDE_NEXT)->EnableWindow(FALSE);
}

INT COtherGroupNo_Divide_Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strAcqYear, strGroupNo, strWorkNo, strSerialNo, strTmpData;
	
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
	
	if(strDMFieldAlias == _T("UDF_수입구분"))
	{	
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("입수구분"), str, str);
	}
	else if(strDMFieldAlias == _T("UDF_자료상태설명"))
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
	}
	else if(strDMFieldAlias == _T("UDF_가격"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
	}
	else if(strDMFieldAlias == _T("BS_주제구분"))
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
	return 0 ;
}

INT COtherGroupNo_Divide_Dlg::MakeCurSelGroupData(CString strAcqCode, CString strAcqYear, CString strGroupNo)
{
	CString strQuery;
	strQuery.Format(
		_T("I.REC_KEY = S.REC_KEY AND I.REC_KEY = B.SPECIES_KEY AND B.ACQ_CODE='%s' AND B.ACQ_YEAR='%s' AND B.SEQ_NO=%s"),
		strAcqCode, strAcqYear, strGroupNo);
	INT ids = m_pDM_BOOK->RefreshDataManager(strQuery);
	if(ids < 0) return -1;

	m_pDM->FreeData();
	ids = MakeSpeciesInfoByBook();
	return ids;
}

VOID COtherGroupNo_Divide_Dlg::OnbtnOTHERGROUPNODIVIDEPREV() 
{
	// TODO: Add your control notification handler code here
	Display(-1);
}

VOID COtherGroupNo_Divide_Dlg::OnbtnOTHERGROUPNODIVIDENEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);	
}

VOID COtherGroupNo_Divide_Dlg::OnOK() 
{
	// TODO: Add extra validation here
	INT ids = CheckSelectDataValid();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}

	COtherGroupNo_Select_Dlg dlg(this);
	dlg.SetAcqYear(m_pParentDM->GetCellData(_T("수입년도"), m_nIdx));
	dlg.SetAcqCode(m_pParentDM->GetCellData(_T("수입구분"), m_nIdx));
	
	if(IDOK == dlg.DoModal())
	{
		CGroupInfo * pGroup = dlg.GetSelGroup();
		if(CheckIsSameGroup(pGroup->GetAcqYear(), pGroup->GetGroupInfo()))
		{
			ESLAfxMessageBox(_T("동일그룹을 선택하였습니다. 다시 선정해 주십시요."));
			return ;
		}
		ids = MakeSelectDataArray(m_pDM, m_pGrid, _T("IBS_종KEY"));
		if(ids < 0) return ;
		
		CGroupPair mgr(this);
		mgr.SetTargetGroup(pGroup);
		mgr.SetSelectDataArray(&m_ArrSelData);
		mgr.SetGroupType(OTHER_GROUP_NO);
		mgr.SetAcqCode(m_pParentDM->GetCellData(_T("수입구분"), m_nIdx));
		mgr.SetMainDM(m_pParentDM);
		ids = mgr.DivideGroup();
		if(ids < 0)
			ESLAfxMessageBox(_T("선정한 자료를 분리할 수 없습니다."));
		else 
		{
			ESLAfxMessageBox(_T("선정한 자료를 분리하였습니다."));
			DeleteSelectedRow();
			CGroupInfo Group;
			Group.SetAcqYear(m_pParentDM->GetCellData(_T("수입년도"), m_nIdx));
			Group.SetGroupInfo(m_pParentDM->GetCellData(_T("그룹번호"), m_nIdx));
			mgr.UpdateGroupBookCnt(&Group);
			m_pGrid->Display();
		}
	}
}

BOOL COtherGroupNo_Divide_Dlg::CheckIsSameGroup(CString strAcqYear, CString strGroupNo)
{
	CString strCurAcqYear, strCurGroupNo;
	strCurAcqYear = m_pParentDM->GetCellData(_T("수입년도"), m_nIdx);
	strCurGroupNo = m_pParentDM->GetCellData(_T("그룹번호"), m_nIdx);
	if(strCurGroupNo == strGroupNo && strCurAcqYear == strAcqYear)
		return TRUE;
	return FALSE;	
}

INT COtherGroupNo_Divide_Dlg::DeleteSelectedRow()
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

INT COtherGroupNo_Divide_Dlg::RemoveAllSelectDataArray()
{
	INT nCnt = m_ArrSelData.GetSize();
	for(INT i=0 ; i<nCnt ; i++)
	{
		delete m_ArrSelData.GetAt(i);
	}
	
	m_ArrSelData.RemoveAll();
	return 0;
}

INT COtherGroupNo_Divide_Dlg::MakeSelectDataArray(CESL_DataMgr *pDM, CESL_Grid *pGrid, CString strAlias)
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

INT COtherGroupNo_Divide_Dlg::CheckSelectDataValid()
{
	if(m_pDM->GetRowCount() == 0) return -1;
	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return ids;
	INT i = m_pGrid->SelectGetHeadPosition();
	if(i < 0) return i;
	
	return 0;
}

INT COtherGroupNo_Divide_Dlg::MakeSpeciesInfoByBook()
{
	if(!m_pDM_BOOK || !m_pDM) return -1;
	INT nRowCnt = m_pDM_BOOK->GetRowCount();
	if(nRowCnt == 0) return 0;
	
	m_pDM_BOOK->SORT(_T("IBS_종KEY"));
	
	MakeOtherInfoByBook(m_pDM, m_pDM_BOOK, 0, nRowCnt-1, _T("IBS_종KEY"));
	
	return 0;
}

INT COtherGroupNo_Divide_Dlg::MakeOtherInfoByBook(CESL_DataMgr * pTargetDM, CESL_DataMgr * pSourceDM, INT startIdx, INT endIdx, CString strAlias)
{
	CString strKey, strTmpKey, strTmpData, strDeliveryYN, strSpeciesWorkingStatus, strAcqCode, strGroupNo;
	INT ids, cnt, nDBookCnt;
	DOUBLE nTotalPrice = 0.0;
	cnt = nDBookCnt = 0;

	TCHAR* pEndPtr = NULL;
	
	for(INT i=startIdx ; i<=endIdx; i++)
	{
		pSourceDM->GetCellData(strAlias, i, strTmpKey);
		if(!strKey.IsEmpty() && strKey != strTmpKey)
		{
			ids = AddToTargetDM(pTargetDM, pSourceDM, i-1);
			if(ids < 0) break;
			strTmpData.Format(_T("%d"), cnt);
			pTargetDM->SetCellData(_T("UDF_책수"), strTmpData, ids);
			strTmpData.Format(_T("%.0f"), nTotalPrice);
			pTargetDM->SetCellData(_T("UDF_종가격"), strTmpData, ids);
			pTargetDM->SetCellData(_T("UDF_가격"), strTmpData, ids);
			pTargetDM->SetCellData(_T("UDF_자료상태설명"), strSpeciesWorkingStatus, ids);
			pTargetDM->SetCellData(_T("UDF_수입구분"), strAcqCode, ids);
			pTargetDM->SetCellData(_T("UDF_그룹번호"), strGroupNo, ids);

			strSpeciesWorkingStatus.Empty();
			strKey = strTmpKey;
			cnt = 1;
			strTmpData = pSourceDM->GetCellData(_T("BB_가격"), i);

			nTotalPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

			strSpeciesWorkingStatus = pSourceDM->GetCellData(_T("BB_자료상태"), i);
			strAcqCode = pSourceDM->GetCellData(_T("BB_수입구분"), i);
			strGroupNo = pSourceDM->GetCellData(_T("BB_그룹번호"), i);
		}
		else 
		{
			cnt++;
			strTmpData = pSourceDM->GetCellData(_T("BB_가격"), i);

			nTotalPrice += _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

			strTmpData = pSourceDM->GetCellData(_T("BB_자료상태"), i);
			strSpeciesWorkingStatus = strTmpData;
			strAcqCode = pSourceDM->GetCellData(_T("BB_수입구분"), i);
			strGroupNo = pSourceDM->GetCellData(_T("BB_그룹번호"), i);
		}
	}
	if(i > endIdx)
	{
		ids = AddToTargetDM(pTargetDM, pSourceDM, i-1);
		strTmpData.Format(_T("%d"), cnt);
		pTargetDM->SetCellData(_T("UDF_책수"), strTmpData, ids);
		strTmpData.Format(_T("%.0f"), nTotalPrice);
		pTargetDM->SetCellData(_T("UDF_종가격"), strTmpData, ids);
		pTargetDM->SetCellData(_T("UDF_가격"), strTmpData, ids);
		pTargetDM->SetCellData(_T("UDF_자료상태설명"), strSpeciesWorkingStatus, ids);
		pTargetDM->SetCellData(_T("UDF_수입구분"), strAcqCode, ids);
		pTargetDM->SetCellData(_T("UDF_그룹번호"), strGroupNo, ids);
	}
	return 0;
}

INT COtherGroupNo_Divide_Dlg::AddToTargetDM(CESL_DataMgr *pTargetDM, CESL_DataMgr *pSourceDM, INT idx)
{
	if (!pTargetDM || !pSourceDM) return -1000;
	if( idx < 0) return -1000;
	
	INT col, nCol;
	
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	INT nCurRow, nColCnt;
	pTargetDM->InsertRow(-1);
	nCurRow = pTargetDM->GetRowCount() - 1;
	nColCnt = pSourceDM->m_nCols;
	if(nColCnt < 0) nColCnt = pSourceDM->GetRefColumnCount();
	
	for (col = 0; col < nColCnt; col++) {
		if (pSourceDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0) continue;
		nCol = pTargetDM->FindColumn(FIELD_ALIAS);
		if(nCol >= 0)	pTargetDM->SetCellData(nCurRow, nCol, pSourceDM->GetCellData(idx, col));
	}
	
	return nCurRow;
}
VOID COtherGroupNo_Divide_Dlg::OnSize(UINT nType, INT cx, INT cy) 
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

VOID COtherGroupNo_Divide_Dlg::OnbtnDATASORT() 
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

BOOL COtherGroupNo_Divide_Dlg::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_정렬항목설정"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	pTmpData->Add(_T("UDF_그룹번호"));
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_주제구분설명"), _T("BS_주제구분"));
	m_pSortItemSet->ReadSortItem();
	return TRUE;
}

HBRUSH COtherGroupNo_Divide_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
