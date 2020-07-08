// OtherGroupNo_DataView_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "OtherGroupNo_DataView_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COtherGroupNo_DataView_Dlg dialog


COtherGroupNo_DataView_Dlg::COtherGroupNo_DataView_Dlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(COtherGroupNo_DataView_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COtherGroupNo_DataView_Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nIdx = -1;
	m_nFirstIdx = -1;
	m_nLastIdx = -1;
	m_pCM = NULL;
	m_pMainCM = NULL;
	m_pDM = NULL;
	m_pMainDM = NULL;
	m_pGrid = NULL;
}
COtherGroupNo_DataView_Dlg::~COtherGroupNo_DataView_Dlg()
{
}
VOID COtherGroupNo_DataView_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COtherGroupNo_DataView_Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COtherGroupNo_DataView_Dlg, CDialog)
	//{{AFX_MSG_MAP(COtherGroupNo_DataView_Dlg)
	ON_BN_CLICKED(IDC_btnOTHER_GROUP_NO_INFO_PREV, OnbtnOTHERGROUPNOINFOPREV)
	ON_BN_CLICKED(IDC_btnOTHER_GROUP_NO_INFO_NEXT, OnbtnOTHERGROUPNOINFONEXT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COtherGroupNo_DataView_Dlg message handlers

BOOL COtherGroupNo_DataView_Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("K2UPGRADE_수서그룹번호자료확인")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		

	m_pCM = FindCM(_T("CM_수서그룹번호자료확인"));
	m_pDM = FindDM(_T("DM_수서그룹번호자료확인"));
	SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	
	Display(0);	

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID COtherGroupNo_DataView_Dlg::Display(INT idx)
{
	INT ids;
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
	
	m_nIdx = ids;
	m_pCM->SetControlMgrData(_T("수입구분"), m_strAcqCode);
	GetDlgItem(IDC_cmbOTHER_GROUP_NO_ACQ_CODE)->EnableWindow(FALSE);
	m_pCM->SetControlMgrData(_T("수입년도"), m_pMainDM->GetCellData(_T("수입년도"), m_nIdx));
	m_pCM->SetControlMgrData(_T("그룹번호"), m_pMainDM->GetCellData(_T("그룹번호"), m_nIdx));
	m_pCM->SetControlMgrData(_T("그룹번호생성일"), m_pMainDM->GetCellData(_T("그룹번호생성일"), m_nIdx));
	m_pCM->SetControlMgrData(_T("그룹번호생성자"), m_pMainDM->GetCellData(_T("그룹번호생성자"), m_nIdx));
	
	ids = GetTotalPriceAndBookCount();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정 그룹번호의 총계정보를 추출할 수 없습니다."));
		return;
	}
	
	ids = MakeDM();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정 그룹번호에 대한 정보를 구축할 수 없습니다."));
		return;
	}
	m_pGrid->Display();
	pGrid->SetReverse(m_nIdx);
}

VOID COtherGroupNo_DataView_Dlg::InitFirstLastIndex()
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

VOID COtherGroupNo_DataView_Dlg::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnOTHER_GROUP_NO_INFO_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnOTHER_GROUP_NO_INFO_NEXT)->EnableWindow(TRUE);
	
	if(m_nFirstIdx == idx || idx < 0)
		GetDlgItem(IDC_btnOTHER_GROUP_NO_INFO_PREV)->EnableWindow(FALSE);
	if(m_nLastIdx == idx || idx < 0)
		GetDlgItem(IDC_btnOTHER_GROUP_NO_INFO_NEXT)->EnableWindow(FALSE);
}

INT COtherGroupNo_DataView_Dlg::GetTotalPriceAndBookCount()
{
	CString str64;
	GetManageValue(_T("기타"), _T("공통"), _T("육사"), _T("육사"),str64);
	if(str64 == _T("Y"))
		str64 = _T("-1");
	else
		str64 = _T("0");
	CString strAcqYear, strReqNo, strTotalPrice, strBookCnt;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("그룹번호"), strReqNo);
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempData(_T("AcqCode"), _ttoi(m_strAcqCode.GetBuffer(0)));
	mgr.SetTempData(_T("AcqYear"), strAcqYear.GetBuffer(0));
	mgr.SetTempData(_T("DivNo"), strReqNo.GetBuffer(0));
	mgr.SetTempData(_T("str64"), str64.GetBuffer(0));
	INT ids = mgr.SPFExecutePath(_T("총계정보추출"));
	if(ids < 0) return ids;
	
	mgr.GetTempData(_T("TotalPrice"), strTotalPrice);
	mgr.GetTempData(_T("BookCount"), strBookCnt);
	
	m_pCM->SetControlMgrData(_T("총책수"), strBookCnt);
	m_PriceConvertMgr.ConversionPriceInfoToDesc(strTotalPrice, strTotalPrice);
	m_pCM->SetControlMgrData(_T("총가격"), strTotalPrice);
	return 0;
}

INT COtherGroupNo_DataView_Dlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
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
	}
	else if(strDMFieldAlias == _T("금액"))
	{
		m_PriceConvertMgr.ConversionPriceInfoToDesc(str, str);
	}
	return 0;
}

INT COtherGroupNo_DataView_Dlg::MakeDM()
{
	CString strAcqYear, strReqNo, strTotalPrice, strBookCnt;
	m_pCM->GetControlMgrData(_T("수입년도"), strAcqYear);
	m_pCM->GetControlMgrData(_T("그룹번호"), strReqNo);
	m_pCM->GetControlMgrData(_T("총책수"), strBookCnt);
	m_pCM->GetControlMgrData(_T("총가격"), strTotalPrice);
	strTotalPrice.Replace(_T(","), _T(""));
	INT totalBookCnt = _ttoi(strBookCnt.GetBuffer(0));
	CGroupMgr mgr(this);
	mgr.InitTempData();
	mgr.SetTempData(_T("AcqCode"), _ttoi(m_strAcqCode.GetBuffer(0)));
	mgr.SetTempData(_T("AcqYear"), strAcqYear.GetBuffer(0));
	mgr.SetTempData(_T("DivNo"), strReqNo.GetBuffer(0));
	mgr.SetTempData(_T("BookCount"), totalBookCnt);
	mgr.SetTempData(_T("TotalPrice"), strTotalPrice.GetBuffer(0));
	mgr.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	INT ids = mgr.SPFExecutePath(_T("자료확인DM구성"));
	if(ids < 0) return ids;
	
	return 0;
}

VOID COtherGroupNo_DataView_Dlg::OnbtnOTHERGROUPNOINFOPREV() 
{
	// TODO: Add your control notification handler code here
	Display(-1);	
}

VOID COtherGroupNo_DataView_Dlg::OnbtnOTHERGROUPNOINFONEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);	
}

HBRUSH COtherGroupNo_DataView_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{ 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
