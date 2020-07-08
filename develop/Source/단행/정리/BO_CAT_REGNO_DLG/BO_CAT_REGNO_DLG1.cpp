// BO_CAT_REGNO_DLG1.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_REGNO_DLG1.h"
#include "BO_CAT_REGNO_GIVE_LOST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REGNO_DLG dialog


CBO_CAT_REGNO_DLG::CBO_CAT_REGNO_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REGNO_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pRegNoArray = NULL;
}


CBO_CAT_REGNO_DLG::~CBO_CAT_REGNO_DLG()
{
	
}

VOID CBO_CAT_REGNO_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REGNO_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_REGNO_DLG, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REGNO_DLG)
	ON_BN_CLICKED(IDC_GIVE_LOSTREGNO, OnGiveLostregno)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_edtOmitRegNoCnt, OnKillfocusedtOmitRegNoCnt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REGNO_DLG message handlers

BOOL CBO_CAT_REGNO_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_단행_정리_누락등록번호")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	m_Pre_RegGroup = _T("");
	AllControlDisplay(_T("CM_단행_정리_누락등록번호"));


	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_REGNO_DLG::OnGiveLostregno() 
{
	INT idx = -1;
	SelectMakeList(_T("CM_단행_정리_누락등록번호"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_단행_정리_누락등록번호"), _T("MAIN_GRID"));
	if (idx < 0)
	{
		//에러처리
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_단행_정리DM_책정보"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox ( _T("DM_단행_정리DM_책정보") );
		return ;
	}

	CString strRegNo;
	CString strRegCode;
	while ( idx >= 0 ) {
		strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );
		strRegCode = pDM->GetCellData(_T("BB_등록구분_코드"), idx);
		if (!strRegNo.IsEmpty())
		{
			AfxMessageBox(_T("등록 번호 부여된 자료가 있습니다."));
			return;
		}
		if (strRegCode.IsEmpty())
		{
			AfxMessageBox(_T("등록구분이 없는 자료가 있습니다."));
			return;
		}

		idx = SelectGetNext(_T("CM_단행_정리_누락등록번호"), _T("MAIN_GRID"));
	}

	//pDM->SetCellData( _T("등록구분"), strRegCode, 0 );
	// 등록번호 부여 화면을 띄운다.
	CBO_CAT_REGNO_GIVE_LOST Dlg(this);
	Dlg.m_pRegNoArray = m_pRegNoArray;
	if( Dlg.DoModal() == IDOK )
	{
		AllControlDisplay(_T("CM_단행_정리_누락등록번호"));
	}
}

BEGIN_EVENTSINK_MAP(CBO_CAT_REGNO_DLG, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_REGNO_DLG)
	ON_EVENT(CBO_CAT_REGNO_DLG, IDC_MSHFLEXGRID, -600 /* Click */, OnClickMshflexgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_CAT_REGNO_DLG::OnClickMshflexgrid() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid *pGrid=( CESL_Grid * )FindControl(_T("CM_단행_정리_누락등록번호"), _T("MAIN_GRID"));;

	CString strRegGroup;
	INT row = pGrid->GetRow();
	INT icount=0;
	INT iRowCount = pGrid->GetCount();

	//선정된 자료의 갯수 체크
	for (INT i = 1; i<=iRowCount; i++)
	{
		if (pGrid->GetAt(i-1,0) == _T("V"))	icount++;
	}
	
	if (row == 0) return;
	if (icount < 2) m_Pre_RegGroup = _T("");
	
	if (row > 0) {
		pGrid->SetReverse(row-1);
		strRegGroup = pGrid->GetAt(row-1,2);
		if ( strRegGroup != m_Pre_RegGroup && m_Pre_RegGroup != _T(""))
		{
			pGrid->SetAt(row-1,0,_T(""));
			AfxMessageBox(_T("등록구분이 같은 자료만 선택해주세요."));
			return;
		}
	}
	m_Pre_RegGroup = strRegGroup;
}

VOID CBO_CAT_REGNO_DLG::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}



HBRUSH CBO_CAT_REGNO_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

//===================================================
//2009.04.13 ADD BY PJW : 누락등록번호를 가져올때 갯수 제한을 둠
void CBO_CAT_REGNO_DLG::OnKillfocusedtOmitRegNoCnt() 
{
	CString strOmitRegNoCnt;
	INT nOmitRegNoCnt;
	CESL_ControlMgr * pCm = FindCM(_T("CM_단행_정리_누락등록번호"));
	pCm->GetControlMgrData(_T("누락번호갯수"), strOmitRegNoCnt);
	if( !strOmitRegNoCnt.IsEmpty() )
	{
		nOmitRegNoCnt = _ttoi(strOmitRegNoCnt); 
		if( 1 > nOmitRegNoCnt || 10000 < strOmitRegNoCnt )
		{
			AfxMessageBox(_T("최대검색건수는 1부터 10000까지 설정가능 합니다."));
			pCm->SetControlMgrData(_T("누락번호갯수"), _T("100"));
			CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtOmitRegNoCnt);
			pEdit->SetFocus();
		}
	}
	else
	{
		AfxMessageBox(_T("최대검색건수를 넣어 주세요."));
	}
}
//===================================================

