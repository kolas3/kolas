// DivisionNo_DataView_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "DivisionNo_DataView_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_DataView_Dlg dialog


CDivisionNo_DataView_Dlg::CDivisionNo_DataView_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDivisionNo_DataView_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDivisionNo_DataView_Dlg)
	m_bDupYN = FALSE;
	m_bReviewYN = FALSE;
	m_bCopyYN = FALSE;
	//}}AFX_DATA_INIT
	m_nIdx = -1;
	m_nFirstIdx = -1;
	m_nLastIdx = -1;
	m_pCM = NULL;
	m_pMainCM = NULL;
	m_pDM = NULL;
	m_pMainDM = NULL;
}

CDivisionNo_DataView_Dlg::~CDivisionNo_DataView_Dlg()
{
}

VOID CDivisionNo_DataView_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDivisionNo_DataView_Dlg)
	DDX_Check(pDX, IDC_chkDIVNO_HOLD_DUP_SEARCH, m_bDupYN);
	DDX_Check(pDX, IDC_chkDIVNO_PURCHASE_CHECK, m_bReviewYN);
	DDX_Check(pDX, IDC_chkDIVNO_WORK_DUP_SEARCH, m_bCopyYN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDivisionNo_DataView_Dlg, CDialog)
	//{{AFX_MSG_MAP(CDivisionNo_DataView_Dlg)
	ON_BN_CLICKED(IDC_btnDIVNO_INFO_NEXT, OnbtnDIVNOINFONEXT)
	ON_BN_CLICKED(IDC_btnDIVNO_INFO_PREV, OnbtnDIVNOINFOPREV)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDivisionNo_DataView_Dlg message handlers

BOOL CDivisionNo_DataView_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_구입차수자료확인")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	m_pCM = FindCM(_T("CM_구입차수자료확인"));
	m_pDM = FindDM(_T("DM_구입차수자료확인"));
	
	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	
	GetDlgItem(IDC_btnDIVNO_INFO_PRINT)->ShowWindow(SW_HIDE);
	Display(0);

	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_edtDIVNO_STATUS);
	pEdit->ShowWindow(SW_HIDE);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDivisionNo_DataView_Dlg::InitFirstLastIndex()
{
	CESL_Grid * pGrid = (CESL_Grid*)m_pMainCM->FindControl(_T("MainGrid"));
	INT ids = pGrid->SelectMakeList();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	m_nFirstIdx = pGrid->SelectGetHeadPosition();
	m_nLastIdx = pGrid->SelectGetTailPosition();
}

VOID CDivisionNo_DataView_Dlg::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnDIVNO_INFO_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnDIVNO_INFO_NEXT)->EnableWindow(TRUE);

	if(m_nFirstIdx == idx || idx < 0)
		GetDlgItem(IDC_btnDIVNO_INFO_PREV)->EnableWindow(FALSE);
	if(m_nLastIdx == idx || idx < 0)
		GetDlgItem(IDC_btnDIVNO_INFO_NEXT)->EnableWindow(FALSE);
}

VOID CDivisionNo_DataView_Dlg::Display(INT idx)
{
	INT ids;
	INT nSelectRow;
	CString errstr;
	CESL_Grid * pGrid = (CESL_Grid*)m_pMainCM->FindControl(_T("MainGrid"));
	switch(idx)
	{
	case 0:
		InitFirstLastIndex();
		pGrid->SelectMakeList();
		ids = pGrid->SelectGetHeadPosition();
		ShowNextPrevButton(ids);
		if (ids < 0)
		{
			ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
			EndDialog(IDCANCEL);
			return;
		}
		break;
	case -1:
		ids = pGrid->SelectGetPrev();
		ShowNextPrevButton(ids);
		if (ids == -1) return;
		break;
	case 1:
		ids = pGrid->SelectGetNext();
		ShowNextPrevButton(ids);
		if (ids == -1) return;
		break;
	}
	nSelectRow = ids;

	CString strTmpData;
	m_pCM->SetControlMgrData(_T("수입년도"), m_pMainDM->GetCellData(_T("수입년도"), ids));
	strTmpData = m_pMainDM->GetCellData(_T("작업상태"), ids);
	if(strTmpData == _T("0")) strTmpData = _T("구입대상");
	else if(strTmpData == _T("1")) strTmpData = _T("주문");
	else if(strTmpData == _T("2")) strTmpData = _T("검수완료");
	m_pCM->SetControlMgrData(_T("작업상태"), strTmpData);
	m_pCM->SetControlMgrData(_T("차수번호"), m_pMainDM->GetCellData(_T("차수번호"), ids));
	m_pCM->SetControlMgrData(_T("차수생성일"), m_pMainDM->GetCellData(_T("차수생성일"), ids));
	m_pCM->SetControlMgrData(_T("차수생성자"), m_pMainDM->GetCellData(_T("차수생성자"), ids));
	m_pCM->SetControlMgrData(_T("업무구분"), m_pMainDM->GetCellData(_T("업무구분"), ids));
	strTmpData = m_pMainDM->GetCellData(_T("중복조사여부"), ids);
	if(strTmpData == _T("Y")) m_bCopyYN = TRUE;		else m_bCopyYN = FALSE;
	strTmpData = m_pMainDM->GetCellData(_T("복본조사여부"), ids);
	if(strTmpData == _T("Y")) m_bDupYN = TRUE;		else m_bDupYN = FALSE;
	strTmpData = m_pMainDM->GetCellData(_T("심의완료여부"), ids);
	if(strTmpData == _T("Y")) m_bReviewYN = TRUE;	else m_bReviewYN = FALSE;
	UpdateData(FALSE);

	ids = GetTotalPriceAndBookCount();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정차수의 총계정보를 추출할 수 없습니다."));
		return;
	}
	ids = MakeDM();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정차수에 대한 정보를 구축할 수 없습니다."));
		return;
	}
	DisplayGrid(_T("MainGrid"));

	pGrid->SetReverse(nSelectRow);
	m_nIdx = nSelectRow;
}

INT CDivisionNo_DataView_Dlg::GetTotalPriceAndBookCount()
{
	CString str64;
	GetManageValue(_T("기타"), _T("공통"), _T("육사"), _T("육사"),str64);
	if(str64 == _T("Y"))
		str64 = _T("-1");
	else
		str64 = _T("0");
	
	CString strAcqYear, strDivNo, strTotalPrice, strBookCnt, strWorkCode;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("차수번호"), strDivNo);
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempData(_T("AcqCode"), 1);
	mgr.SetTempData(_T("AcqYear"), strAcqYear.GetBuffer(0));
	mgr.SetTempData(_T("DivNo"), strDivNo.GetBuffer(0));
	mgr.SetTempData(_T("WorkCode"), strWorkCode.GetBuffer(0));
	mgr.SetTempData(_T("str64"), str64.GetBuffer(0));
	INT ids = mgr.SPFExecutePath(_T("총계정보추출"));
	if(ids < 0) return ids;

	mgr.GetTempData(_T("TotalPrice"), strTotalPrice);
	mgr.GetTempData(_T("BookCount"), strBookCnt);
	
	if(strTotalPrice.IsEmpty()) strTotalPrice = _T("0");
	if(strBookCnt.IsEmpty()) strBookCnt = _T("0");
	m_pCM->SetControlMgrData(_T("총책수"), strBookCnt);
	m_PriceConvertMgr.ConversionPriceInfoToDesc(strTotalPrice, strTotalPrice);
	m_pCM->SetControlMgrData(_T("총가격"), strTotalPrice);
	return 0;
}

INT CDivisionNo_DataView_Dlg::MakeDM()
{
	CString str64;
	GetManageValue(_T("기타"), _T("공통"), _T("육사"), _T("육사"),str64);
	if(str64 == _T("Y"))
		str64 = _T("-1");
	else
		str64 = _T("0");

	CString strAcqYear, strDivNo, strTotalPrice, strBookCnt, strWorkCode;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("차수번호"), strDivNo);
	m_pCM->GetControlMgrData(_T("총책수"), strBookCnt);
	m_pCM->GetControlMgrData(_T("총가격"), strTotalPrice);
	strTotalPrice.Replace(_T(","), _T(""));
	m_pCM->GetControlMgrData(_T("업무구분"), strWorkCode);
	INT totalBookCnt = _ttoi(strBookCnt.GetBuffer(0));
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempData(_T("AcqCode"), 1);
	mgr.SetTempData(_T("AcqYear"), strAcqYear.GetBuffer(0));
	mgr.SetTempData(_T("DivNo"), strDivNo.GetBuffer(0));
	mgr.SetTempData(_T("BookCount"), totalBookCnt);
	mgr.SetTempData(_T("TotalPrice"), strTotalPrice.GetBuffer(0));
	mgr.SetTempData(_T("WorkCode"), strWorkCode.GetBuffer(0));
	mgr.SetTempData(_T("str64"), str64.GetBuffer(0));
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	INT ids = mgr.SPFExecutePath(_T("자료확인DM구성"));
	if(ids < 0) return ids;
	
	return 0;
}

INT CDivisionNo_DataView_Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	if(strDMFieldAlias == _T("자료상태"))
	{
		CString strWorkingStatus = m_pDM->GetCellData(_T("자료상태"), nRow);
		if(strWorkingStatus == _T("BOT111O")) str = _T("기초자료");
		else if(strWorkingStatus == _T("BOT112O")) str = _T("반입자료");
		else if(strWorkingStatus == _T("BOA111N")) str = _T("구입대상자료");
		else if(strWorkingStatus == _T("BOA112N")) str = _T("주문자료");
		else if(strWorkingStatus == _T("BOA113O")) str = _T("미납품자료");
		else if(strWorkingStatus == _T("BOA114O")) str = _T("구입인계대상자료");
		else if(strWorkingStatus == _T("BOA211O")) str = _T("기증접수자료");
		else if(strWorkingStatus == _T("BOA212O")) str = _T("기증미등록자료");
		else if(strWorkingStatus == _T("BOR111N")) str = _T("등록인계자료");
		else if(strWorkingStatus == _T("BOR112N")) str = _T("등록자료");
		else if(strWorkingStatus == _T("BOR113O")) str = _T("가원부번호부여자료");
		else if(strWorkingStatus == _T("BOC111O")) str = _T("복본조사대상자료");
		else if(strWorkingStatus == _T("BOC112O")) str = _T("분류대상자료");
		else if(strWorkingStatus == _T("BOC113N")) str = _T("정리대상자료");
		else if(strWorkingStatus == _T("BOC114O")) str = _T("열람인계대상자료");
		else if(strWorkingStatus == _T("BOL111O")) str = _T("열람인계자료");
		else if(strWorkingStatus == _T("BOL112N")) str = _T("배가자료");
		else if(strWorkingStatus == _T("BOL113O")) str = _T("수리제본자료");
		else if(strWorkingStatus == _T("BOL114O")) str = _T("파손자료");
		else if(strWorkingStatus == _T("BOL115O")) str = _T("제적자료");
		else if(strWorkingStatus == _T("BOL211O")) str = _T("관외대출자료");
		else if(strWorkingStatus == _T("BOL212O")) str = _T("관내대출자료");
		else if(strWorkingStatus == _T("BOL411O")) str = _T("단행책두레업무진행자료");
		else if(strWorkingStatus == _T("BOL511O")) str = _T("통합도서타관반납");
		else if(strWorkingStatus == _T("BOL611O")) str = _T("통합도서타관대출");
		else if(strWorkingStatus == _T("BOL213O")) str = _T("특별대출자료");
		else if(strWorkingStatus == _T("BOL214O")) str = _T("긴급대출자료");
		else if(strWorkingStatus == _T("BOL215O")) str = _T("배달자료");
		else if(strWorkingStatus == _T("BOL252N")) str = _T("분실자료");
		else if(strWorkingStatus == _T("BOL116O")) str = _T("재정리자료");
		else if(strWorkingStatus == _T("BOL218O")) str = _T("상호대차자료");							
		else if(strWorkingStatus == _T("BOL219O")) str = _T("책나래대출자료");
		else if(strWorkingStatus == _T("BOL221O")) str = _T("관외대출자료");
		else if(strWorkingStatus == _T("BOL222O")) str = _T("관내대출자료");
		else if(strWorkingStatus == _T("BOL223O")) str = _T("특별대출자료");
		else if(strWorkingStatus == _T("BOL224O")) str = _T("긴급대출자료");
		else if(strWorkingStatus == _T("BOL225O")) str = _T("배달자료");
		else if(strWorkingStatus == _T("BOL251N")) str = _T("수리제본자료");
		else if(strWorkingStatus == _T("BOL253N")) str = _T("제적자료");
		else if(strWorkingStatus == _T("BOL217O")) str = _T("장기대출자료");		
		else if(strWorkingStatus == _T("BOT211O")) str = _T("수서복본추기자료");
		else if(strWorkingStatus == _T("BOT212O")) str = _T("정리복본추기자료");
	}
	else if(strDMFieldAlias == _T("금액"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
	}
	return 0;
}

VOID CDivisionNo_DataView_Dlg::DisplayGrid(CString gridAlias)
{
	CESL_Grid * pGrid = (CESL_Grid*)m_pCM->FindControl(gridAlias);
	if(!pGrid) return;
	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	pGrid->Display();
}

VOID CDivisionNo_DataView_Dlg::OnbtnDIVNOINFONEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);
}

VOID CDivisionNo_DataView_Dlg::OnbtnDIVNOINFOPREV() 
{
	// TODO: Add your control notification handler code here
	Display(-1);
}

HBRUSH CDivisionNo_DataView_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
