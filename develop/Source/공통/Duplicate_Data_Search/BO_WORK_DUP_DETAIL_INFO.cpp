// BO_WORK_DUP_DETAIL_INFO.cpp : implementation file
//

#include "stdafx.h"
#include "BO_WORK_DUP_DETAIL_INFO.h"
#include "DuplicateSearchMgr.h"
#include "../BO_VOLUME_FORM/BOVolumeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_WORK_DUP_DETAIL_INFO dialog


CBO_WORK_DUP_DETAIL_INFO::CBO_WORK_DUP_DETAIL_INFO(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_WORK_DUP_DETAIL_INFO::IDD, pParent)
{
	
	AfxInitRichEdit();
	//{{AFX_DATA_INIT(CBO_WORK_DUP_DETAIL_INFO)
	m_SrcBookCnt = _T("");
	m_DupBookCnt = _T("");
	m_SrcJobNO = _T("");
	m_DupJobNO = _T("");
	m_DupAllCnt = _T("");
	m_DupCurIdx = _T("");
	m_SrcAllCnt = _T("");
	m_SrcCurIdx = _T("");
	m_SrcVolCnt = _T("");
	m_DupVolCnt = _T("");
	//}}AFX_DATA_INIT
	m_bGridSelectYN = FALSE;
	m_nOpenMode = OPERATION_TYPE_INVALID;
	m_pMainDM = NULL;
	m_pMainGrid = NULL;

	// KOL.UDF.022 시큐어코딩 보완
	m_nSrcIdx = -1;
	m_nDupIdx = -1;
	m_nSrcFirstIdx = -1;
	m_nDupFirstIdx = -1;
	m_nSrcLastIdx  = -1;
	m_nDupLastIdx  = -1;
	m_bGridSelectYN = FALSE;
	m_pMainDM = NULL;
	m_pMainGrid = NULL;
}

CBO_WORK_DUP_DETAIL_INFO::~CBO_WORK_DUP_DETAIL_INFO()
{
	
}


VOID CBO_WORK_DUP_DETAIL_INFO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_WORK_DUP_DETAIL_INFO)
	DDX_Text(pDX, IDC_edtBOOK_COUNT, m_SrcBookCnt);
	DDX_Text(pDX, IDC_edtBOOK_COUNT_SEL, m_DupBookCnt);
	DDX_Text(pDX, IDC_edtJOB_NO, m_SrcJobNO);
	DDX_Text(pDX, IDC_edtJOB_NO_SEL, m_DupJobNO);
	DDX_Text(pDX, IDC_edtSELGUN, m_DupAllCnt);
	DDX_Text(pDX, IDC_edtSELJAE, m_DupCurIdx);
	DDX_Text(pDX, IDC_edtSOURCEGUN, m_SrcAllCnt);
	DDX_Text(pDX, IDC_edtSOURCEJAE, m_SrcCurIdx);
	DDX_Text(pDX, IDC_edtVOL_COUNT, m_SrcVolCnt);
	DDX_Text(pDX, IDC_edtVOL_COUNT_SEL, m_DupVolCnt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_WORK_DUP_DETAIL_INFO, CDialog)
	//{{AFX_MSG_MAP(CBO_WORK_DUP_DETAIL_INFO)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnPRE_SEL, OnbtnPRESEL)
	ON_BN_CLICKED(IDC_btnNEXT_SEL, OnbtnNEXTSEL)
	ON_BN_CLICKED(IDC_btnVOL_INFO_INPUT, OnbtnVOLINFOINPUT)
	ON_BN_CLICKED(IDC_btnVOL_INFO_INPUT_SEL, OnbtnVOLINFOINPUTSEL)
	ON_BN_CLICKED(IDC_btnSELECT_DATA, OnbtnSELECTDATA)
	ON_BN_CLICKED(IDC_btnSELECT_DATA2, OnbtnSELECTDATA2)
	ON_BN_CLICKED(IDC_btnWORKPREV, OnbtnWORKPREV)
	ON_BN_CLICKED(IDC_btnWORKNEXT, OnbtnWORKNEXT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_WORK_DUP_DETAIL_INFO message handlers

VOID CBO_WORK_DUP_DETAIL_INFO::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}

BOOL CBO_WORK_DUP_DETAIL_INFO::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_marceditor_src.SubclassDlgItem(IDC_richWORK_SOURCE, -1, this);
	m_marceditor_dup.SubclassDlgItem(IDC_richWORK_DUPLICATE, -1, this);
	
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	Display(0);

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_WORK_DUP_DETAIL_INFO::Display(INT nIdx)
{
	BOOL bSrcChange = FALSE;
	switch(nIdx)
	{
	case 0:
		InitFirstLastIndex();
		GetFirstSelectIndex(m_nSrcIdx, m_nDupIdx);
		ShowSrcNextPrevButton(m_nSrcIdx);
		ShowDupNextPrevButton(m_nDupIdx);
		bSrcChange = TRUE;
		break;
	case -1:
		GetPrevSelectIndex(m_nSrcIdx, m_nDupIdx);
		ShowSrcNextPrevButton(m_nSrcIdx);
		ShowDupNextPrevButton(m_nDupIdx);
		bSrcChange = TRUE;
		break;
	case 1:
		GetNextSelectIndex(m_nSrcIdx, m_nDupIdx);
		ShowSrcNextPrevButton(m_nSrcIdx);
		ShowDupNextPrevButton(m_nDupIdx);
		bSrcChange = TRUE;
		break;
	case 1000:
		GetNextDupIndex(m_nDupIdx);
		ShowDupNextPrevButton(m_nDupIdx);
		bSrcChange = FALSE;
		break;
	case -1000:
		GetPrevDupIndex(m_nDupIdx);
		ShowDupNextPrevButton(m_nDupIdx);
		bSrcChange = FALSE;
		break;
	default:
		return ;
	}
	
	CString srcmarc, dupmarc, srckey, dupkey, strQuery;
	
	if(bSrcChange)
	{
		m_pMainDM->GetCellData(_T("식별번호"), m_nSrcIdx, m_SrcJobNO);
		m_pMainDM->GetCellData(_T("선택자료"), m_nSrcIdx, m_SrcAllCnt);
		m_pMainDM->GetCellData(_T("식별번호"), m_nSrcIdx, m_SrcCurIdx);
		m_pMainDM->GetCellData(_T("IBS_종KEY"), m_nSrcIdx, srckey);
		m_pMainDM->GetCellData(_T("BS_MARC"), m_nSrcIdx, srcmarc);
		
		strQuery.Format(_T("SELECT MARC FROM BO_SPECIES_TBL WHERE REC_KEY=%s"), srckey);
		m_pMainDM->GetOneValueQuery(strQuery, srcmarc);
		strQuery.Format(_T("SELECT COUNT(REC_KEY) FROM BO_BOOK_TBL WHERE SPECIES_KEY = %s AND (WORKING_STATUS = 'BOA111N' OR WORKING_STATUS = 'BOA112N')"), srckey);
		m_pMainDM->GetOneValueQuery(strQuery, m_SrcBookCnt);
		strQuery.Format(_T("SELECT COUNT(VOL_SORT_NO) FROM BO_BOOK_TBL WHERE SPECIES_KEY = %s AND (WORKING_STATUS = 'BOA111N' OR WORKING_STATUS = 'BOA112N')"), srckey);
		m_pMainDM->GetOneValueQuery(strQuery, m_SrcVolCnt);
	}
	m_pMainDM->GetCellData(_T("일련번호"), m_nDupIdx, m_DupJobNO);
	m_pMainDM->GetCellData(_T("선택자료"), m_nDupIdx, m_DupAllCnt);
	m_pMainDM->GetCellData(_T("일련번호"), m_nDupIdx, m_DupCurIdx);
	m_pMainDM->GetCellData(_T("IBS_종KEY"), m_nDupIdx, dupkey);
	m_pMainDM->GetCellData(_T("BS_MARC"), m_nDupIdx, dupmarc);
	strQuery.Format(_T("SELECT MARC FROM BO_SPECIES_TBL WHERE REC_KEY=%s"), dupkey);
	m_pMainDM->GetOneValueQuery(strQuery, dupmarc);
	strQuery.Format(_T("SELECT COUNT(REC_KEY) FROM BO_BOOK_TBL WHERE SPECIES_KEY = %s AND (WORKING_STATUS = 'BOA111N' OR WORKING_STATUS = 'BOA112N')"), dupkey);
	m_pMainDM->GetOneValueQuery(strQuery, m_DupBookCnt);
	strQuery.Format(_T("SELECT COUNT(VOL_SORT_NO) FROM BO_BOOK_TBL WHERE SPECIES_KEY = %s AND (WORKING_STATUS = 'BOA111N' OR WORKING_STATUS = 'BOA112N')"), dupkey);
	m_pMainDM->GetOneValueQuery(strQuery, m_DupVolCnt);
	//decoding
	CString strTagCode, strErrMsg, strHelpMsg;
	
	if(bSrcChange)
	{
		if(m_pInfo->pMarcMgr->Decoding(srcmarc, &m_marc_src))
		{
			POSITION pos;
			pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
			while (pos)
			{
				m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
				UpdateData(FALSE);
			}	
		}
		m_marceditor_src.InitUserAlias();
		m_marceditor_src.Init(m_pInfo->pMarcMgr, &m_marc_src);
		m_marceditor_src.Display();
	}

	if(m_pInfo->pMarcMgr->Decoding(dupmarc, &m_marc_dup))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}
	m_marceditor_dup.InitUserAlias();
	m_marceditor_dup.Init(m_pInfo->pMarcMgr, &m_marc_dup);
	m_marceditor_dup.Display();
		
	this->UpdateData(FALSE);
	m_pMainGrid->SetReverse(m_nDupIdx);
}

VOID CBO_WORK_DUP_DETAIL_INFO::OnbtnPRESEL() 
{
	// TODO: Add your control notification handler code here
	Display(-1000);
}

VOID CBO_WORK_DUP_DETAIL_INFO::OnbtnNEXTSEL() 
{
	// TODO: Add your control notification handler code here
	Display(1000);
}

VOID CBO_WORK_DUP_DETAIL_INFO::OnbtnVOLINFOINPUT() 
{
	// TODO: Add your control notification handler code here
	// 공통 권별정보 보기 화면을 생성한다.
	this->ShowVolumeDlg(TRUE);
}

VOID CBO_WORK_DUP_DETAIL_INFO::OnbtnVOLINFOINPUTSEL() 
{
	// TODO: Add your control notification handler code here
	// 공통 권별정보 보기 화면을 생성한다.
	this->ShowVolumeDlg(FALSE);
}

VOID CBO_WORK_DUP_DETAIL_INFO::OnbtnSELECTDATA() 
{
	// TODO: Add your control notification handler code here
	//선정자료의 자료 선정 표시를 해제한다.
	this->WriteGridCheckSymbol(_T(""), TRUE);
}

VOID CBO_WORK_DUP_DETAIL_INFO::OnbtnSELECTDATA2() 
{
	// TODO: Add your control notification handler code here
	//중복자료를 선정한다.
	this->WriteGridCheckSymbol(_T("V"), FALSE);
}

VOID CBO_WORK_DUP_DETAIL_INFO::OnbtnWORKPREV() 
{
		// TODO: Add your control notification handler code here
	Display(-1);	
}

VOID CBO_WORK_DUP_DETAIL_INFO::OnbtnWORKNEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);
}

VOID CBO_WORK_DUP_DETAIL_INFO::ShowVolumeDlg(BOOL isSource)
{
	// TODO: Add your control notification handler code here
	// 공통 권별정보 보기 화면을 생성한다.
	// 공통 권별정보 보기 화면을 생성한다.
	INT idx = -1;
	CString strSpeciesKey, strAcqKey;
	if(isSource)
		idx = m_nSrcIdx;
	else
		idx = m_nDupIdx;

	CESL_DataMgr * pDM = this->FindDM(_T("DM_중복본조사"));
	if(!pDM) return;
	CBOVolumeDlg dlg(this);
	strSpeciesKey = m_pMainDM->GetCellData(_T("IBS_종KEY"), idx);
	dlg.SetSpeciesKey(strSpeciesKey);
	strAcqKey = m_pMainDM->GetCellData(_T("BP_수서정보KEY"), idx);
	if(!strAcqKey.IsEmpty() && strAcqKey != _T("-1") && strAcqKey != _T("ALL"))
		dlg.SetPurchaseKey(strAcqKey);
	dlg.SetOpenMode(8000);
	dlg.DoModal();
}

VOID CBO_WORK_DUP_DETAIL_INFO::WriteGridCheckSymbol(CString SymbolStr, BOOL isSource) 
{
	INT idx = -1;
	if(isSource) idx = m_nSrcIdx;
	else idx = m_nDupIdx;
	m_pMainGrid->SetAt(idx, 0, SymbolStr);
}

VOID CBO_WORK_DUP_DETAIL_INFO::InitFirstLastIndex()
{
	CString strTmpData;
	INT ids = m_pMainGrid->SelectMakeList();
	ids = m_pMainGrid->SelectGetHeadPosition();
	if(ids < 0)
	{
		m_bGridSelectYN = FALSE;
		INT cnt = m_pMainDM->GetRowCount();
		m_nSrcFirstIdx = 0;
		for(INT i=cnt-1 ; i>=0 ; i--)
		{
			strTmpData = m_pMainDM->GetCellData(_T("식별번호"), i);
			if(strTmpData != _T("**"))
				break;
		}
		m_nSrcLastIdx = i;
	}
	else
	{
		m_bGridSelectYN = TRUE;
		m_nSrcLastIdx = m_pMainGrid->SelectGetTailPosition();
		m_nSrcFirstIdx = m_pMainGrid->SelectGetHeadPosition();
	}
}

VOID CBO_WORK_DUP_DETAIL_INFO::ShowSrcNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnWORKPREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnWORKNEXT)->EnableWindow(TRUE);

	if(m_nSrcFirstIdx >= idx || idx < 0)
		GetDlgItem(IDC_btnWORKPREV)->EnableWindow(FALSE);
	if(m_nSrcLastIdx <= idx || idx < 0)
		GetDlgItem(IDC_btnWORKNEXT)->EnableWindow(FALSE);
}

VOID CBO_WORK_DUP_DETAIL_INFO::ShowDupNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnPRE_SEL)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNEXT_SEL)->EnableWindow(TRUE);

	if(m_nDupFirstIdx >= idx || idx < 0)
		GetDlgItem(IDC_btnPRE_SEL)->EnableWindow(FALSE);
	if(m_nDupLastIdx <= idx || idx < 0)
		GetDlgItem(IDC_btnNEXT_SEL)->EnableWindow(FALSE);
}

INT CBO_WORK_DUP_DETAIL_INFO::GetFirstSelectIndex(INT &nSrcIdx, INT &nDupIdx)
{
	INT nFirstSelIdx = -1;
	if(m_nOpenMode == GUPDATE)
	{
		nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	else if(m_nOpenMode == SUPDATE)
	{
		if(m_bGridSelectYN)
			nFirstSelIdx = m_nSrcFirstIdx;
		else
			nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	if(nFirstSelIdx < 0) nFirstSelIdx = 0;
	CString strTmpData;
	strTmpData = m_pMainDM->GetCellData(_T("식별번호"), nFirstSelIdx);
	if(strTmpData != _T("**"))
	{
		nSrcIdx = nFirstSelIdx;
		nDupIdx = nFirstSelIdx+1;
	}
	else
	{
		for(INT i=nFirstSelIdx ; i>=0 ; i--)
		{
			strTmpData = m_pMainDM->GetCellData(_T("식별번호"), i);
			if(strTmpData != _T("**"))
				break;
		}
		nSrcIdx = i;
		nDupIdx = nFirstSelIdx;
	}
	//복본자료의 처음 마지막 인덱스를 설정한다.
	SetDupIndexInfo(nSrcIdx);
	return nFirstSelIdx;
}

INT CBO_WORK_DUP_DETAIL_INFO::GetPrevSelectIndex(INT &nSrcIdx, INT &nDupIdx)
{
	INT nPrevSelIndex = -1;
	if(m_bGridSelectYN)
	{
		nPrevSelIndex = m_pMainGrid->SelectGetPrev();
		if(nPrevSelIndex < 0)
		{
			nPrevSelIndex = m_pMainGrid->SelectGetHeadPosition();
		}
	}
	else
	{
		if(m_nSrcIdx != m_nSrcFirstIdx)
		{
			CString strTmpData;
			for(INT i=m_nSrcIdx-1 ; i>=0 ; i--)
			{
				strTmpData = m_pMainDM->GetCellData(_T("식별번호"), i);
				if(strTmpData != _T("**")) break;
			}
			nPrevSelIndex = i;
		}
	}
	nSrcIdx = nPrevSelIndex;
	nDupIdx = nPrevSelIndex + 1;
	//복본자료의 처음 마지막 인덱스를 설정한다.
	SetDupIndexInfo(nSrcIdx);
	return nPrevSelIndex;
}

INT CBO_WORK_DUP_DETAIL_INFO::GetNextDupIndex(INT &nDupIdx)
{
	INT nNextSelIndex = -1;
	if(m_nDupIdx != m_nDupLastIdx)
	{
		nNextSelIndex = m_nDupIdx + 1;
	}
	nDupIdx = nNextSelIndex;
	return nNextSelIndex;
}

INT CBO_WORK_DUP_DETAIL_INFO::GetPrevDupIndex(INT &nDupIdx)
{
	INT nPrevSelIndex = -1;
	if(m_nDupIdx != m_nDupFirstIdx)
	{
		nPrevSelIndex = m_nDupIdx - 1;
	}
	nDupIdx = nPrevSelIndex;
	return nPrevSelIndex;
}

INT CBO_WORK_DUP_DETAIL_INFO::GetNextSelectIndex(INT &nSrcIdx, INT &nDupIdx)
{
	INT nNextSelIndex = -1;
	if(m_bGridSelectYN)
	{
		nNextSelIndex = m_pMainGrid->SelectGetNext();
		if(nNextSelIndex < 0)
		{
			nNextSelIndex = m_pMainGrid->SelectGetTailPosition();
		}
	}
	else
	{
		if(m_nSrcIdx != m_nSrcLastIdx)
		{
			CString strTmpData;
			for(INT i=m_nSrcIdx+1 ; i<=m_nSrcLastIdx ; i++)
			{
				strTmpData = m_pMainDM->GetCellData(_T("식별번호"), i);
				if(strTmpData != _T("**")) break;
			}
			nNextSelIndex = i;			
		}
	}
	nSrcIdx = nNextSelIndex;
	nDupIdx = nNextSelIndex + 1;
	//복본자료의 처음 마지막 인덱스를 설정한다.
	SetDupIndexInfo(nSrcIdx);
	return nNextSelIndex;
}

INT CBO_WORK_DUP_DETAIL_INFO::SetDupIndexInfo(INT nSrcIdx)
{
	CString strTmpData;
	m_nDupFirstIdx = nSrcIdx + 1;
	for(INT i=m_nDupFirstIdx ; i<m_pMainDM->GetRowCount() ; i++)
	{
		strTmpData = m_pMainDM->GetCellData(_T("식별번호"), i);
		if(strTmpData != _T("**")) break;
	}
	m_nDupLastIdx = i - 1;
	return 0;
}

HBRUSH CBO_WORK_DUP_DETAIL_INFO::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
