// SE_CAT_REGNO_DLG1.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_REGNO_DLG1.h"
#include "SE_CAT_REGNO_GIVE_LOST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_REGNO_DLG dialog


CSE_CAT_REGNO_DLG::CSE_CAT_REGNO_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pRegNoArray = NULL;
}

CSE_CAT_REGNO_DLG::~CSE_CAT_REGNO_DLG()
{
}

VOID CSE_CAT_REGNO_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSE_CAT_REGNO_DLG, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_REGNO_DLG)
	ON_BN_CLICKED(IDC_GIVE_LOSTREGNO, OnGiveLostregno)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_REGNO_DLG message handlers

BOOL CSE_CAT_REGNO_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_연속_정리_누락등록번호")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	m_Pre_RegGroup = _T("");
	AllControlDisplay(_T("CM_연속_정리_누락등록번호"));

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_CAT_REGNO_DLG::OnGiveLostregno() 
{
	INT idx = -1;
	SelectMakeList(_T("CM_연속_정리_누락등록번호"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_연속_정리_누락등록번호"), _T("MAIN_GRID"));
	if (idx < 0)
	{
		//에러처리
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리DM_책정보"));
	if ( NULL == pDM ) 
	{
		AfxMessageBox ( _T("DM_연속_정리DM_책정보") );
		return ;
	}

	CString strRegNo;
	CString strRegCode;
	while ( idx >= 0 ) {
		strRegNo = pDM->GetCellData( _T("SB_등록번호") , idx );
		strRegCode = pDM->GetCellData(_T("SB_등록구분_코드"), idx);
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

		idx = SelectGetNext(_T("CM_연속_정리_누락등록번호"), _T("MAIN_GRID"));
	}

	//pDM->SetCellData( _T("등록구분"), strRegCode, 0 );
	// 등록번호 부여 화면을 띄운다.
	CSE_CAT_REGNO_GIVE_LOST Dlg(this);
	Dlg.m_pRegNoArray = m_pRegNoArray;
	if( Dlg.DoModal() == IDOK )
	{
		AllControlDisplay(_T("CM_연속_정리_누락등록번호"));
	}
}

BEGIN_EVENTSINK_MAP(CSE_CAT_REGNO_DLG, CDialog)
    //{{AFX_EVENTSINK_MAP(CSE_CAT_REGNO_DLG)
	ON_EVENT(CSE_CAT_REGNO_DLG, IDC_MSHFLEXGRID, -600 /* Click */, OnClickMshflexgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSE_CAT_REGNO_DLG::OnClickMshflexgrid() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid *pGrid=( CESL_Grid * )FindControl(_T("CM_연속_정리_누락등록번호"), _T("MAIN_GRID"));;

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

VOID CSE_CAT_REGNO_DLG::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

HBRUSH CSE_CAT_REGNO_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
