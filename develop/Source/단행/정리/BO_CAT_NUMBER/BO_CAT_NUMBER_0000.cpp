// BO_CAT_NUMBER_0000.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_NUMBER_0000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_NUMBER_0000 dialog


CBO_CAT_NUMBER_0000::CBO_CAT_NUMBER_0000(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_NUMBER_0000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_NUMBER_0000::~CBO_CAT_NUMBER_0000()
{
}

VOID CBO_CAT_NUMBER_0000::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_NUMBER_0000)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_NUMBER_0000, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_NUMBER_0000)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_NUMBER_0000 message handlers

BOOL CBO_CAT_NUMBER_0000::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
/*
	if (InitESL_Mgr(_T("test2")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
*/		
	if (InitESL_Mgr(_T("BO_CAT_ACQUISIT_NO")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_sWork = _T("수입순번호관리");

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_NUMBER_0000::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID);
	if (pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 0, cx, cy);

	CMSHFlexGrid *pGrid_ACQUISIT_NO = (CMSHFlexGrid*)GetDlgItem(IDC_GRD_ACQUISIT_NO);
	if (pGrid_ACQUISIT_NO->GetSafeHwnd() == NULL) return;
	pGrid_ACQUISIT_NO->MoveWindow(0, 0, cx, cy);

	CMSHFlexGrid *pGrid_ACQUISIT_NO2 = (CMSHFlexGrid*)GetDlgItem(IDC_GRD_ACQUISIT_NO2);
	if (pGrid_ACQUISIT_NO2->GetSafeHwnd() == NULL) return;
	pGrid_ACQUISIT_NO2->MoveWindow(0, 0, cx, cy);

EFS_END
}

BOOL CBO_CAT_NUMBER_0000::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_NUMBER_0000::OnbDELETE(CString m_sWork) 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CString sGridAlias;
	INT ids;
	if ( m_sWork == _T("수입순번호관리") ) {
		sGridAlias = _T("GRD_ACQUISIT_NO");
	}
	else {
		sGridAlias = _T("GRD_ACQUISIT_NO2");
	}

	SelectMakeList(_T("CM_BO_CAT_ACQUISIT_NO"), sGridAlias);
	INT idx = SelectGetTailPosition(_T("CM_BO_CAT_ACQUISIT_NO"), sGridAlias);
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
		return;
	}

	if (MessageBox(_T("선정한 자료를 삭제하시겠습니까?"), _T("삭제 확인"), MB_YESNO) == IDYES)
	{
		CESL_DataMgr *pDM;
		//if ( sWorkTbl )
		if ( m_sWork == _T("수입순번호관리") ) {
			pDM = FindDM(_T("DM_BO_CAT_ACQUISIT_NO"));
		}
		else {
			pDM = FindDM(_T("DM_BO_CAT_OMIT_NO"));
		}

		if ( pDM == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_ACQUISIT_NO") );
		}
		ids = pDM->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return;
		}


		CString sAccRecKey, sAccRecKeyTmp;
		do
		{
			CString strCondition;
			pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

			sAccRecKeyTmp += _T(",") + strCondition;		//원부키 조합

			//if ( sWorkTbl )
			if ( m_sWork == _T("수입순번호관리") ) {
				pDM->MakeDeleteFrame(_T("CO_LAST_BOOK_ACQUISITNO_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
			}
			else {
				pDM->MakeDeleteFrame(_T("CO_OMIT_BOOK_ACQUISITNO_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
			}
			pDM->DeleteRow(idx);

			idx = SelectGetPrev(_T("CM_BO_CAT_ACQUISIT_NO"), sGridAlias);			
		}		
		while (idx > 0);

		sAccRecKey = sAccRecKeyTmp.Mid(1);
		CString sQuery;
		//if ( sWorkTbl )
		if ( m_sWork == _T("수입순번호관리") ) {
			sQuery.Format( _T("DELETE FROM CO_LAST_BOOK_ACQUISITNO_TBL WHERE REC_KEY IN (%s);") , sAccRecKey );
		}
		else {
			sQuery.Format( _T("DELETE FROM CO_OMIT_BOOK_ACQUISITNO_TBL WHERE REC_KEY IN (%s);") , sAccRecKey );
		}
		INT iCh = pDM->AddFrame(sQuery);

		ids = pDM->SendFrame();
		if (ids < 0) {
			AfxMessageBox(_T("삭제사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
			return;
		}
		pDM->EndFrame();
	}	

	AllControlDisplay(_T("CM_BO_CAT_ACQUISIT_NO"));
/*
	SelectMakeList(_T("CM_BO_CAT_ACQUISIT_NO"), _T("GRD_ACQUISIT_NO"));
	INT idx = SelectGetTailPosition(_T("CM_BO_CAT_ACQUISIT_NO"), _T("GRD_ACQUISIT_NO"));
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
		return;
	}

	if (MessageBox(_T("선정한 자료를 삭제하시겠습니까?"), _T("삭제 확인"), MB_YESNO) == IDYES)
	{
		CESL_DataMgr *pDM;
		//if ( sWorkTbl )
		pDM = FindDM(_T("DM_BO_CAT_ACQUISIT_NO"));

		if ( pDM == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_ACQUISIT_NO") );
		}
		pDM->StartFrame();

		CString sAccRecKey, sAccRecKeyTmp;
		do
		{
			CString strCondition;
			pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

			sAccRecKeyTmp += _T(",") + strCondition;		//원부키 조합

			//if ( sWorkTbl )
			pDM->MakeDeleteFrame(_T("CO_LAST_BOOK_ACQUISITNO_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
			pDM->DeleteRow(idx);

			idx = SelectGetPrev(_T("CM_BO_CAT_ACQUISIT_NO"), _T("GRD_ACQUISIT_NO"));			
		}		
		while (idx > 0);

		sAccRecKey = sAccRecKeyTmp.Mid(1);
		CString sQuery;
		//if ( sWorkTbl )
		sQuery.Format( _T("DELETE FROM CO_LAST_BOOK_ACQUISITNO_TBL WHERE REC_KEY IN (%s);") , sAccRecKey );
		INT iCh = pDM->AddFrame(sQuery);

		INT ids = pDM->SendFrame();
		if (ids < 0) {
			AfxMessageBox(_T("삭제사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
			return;
		}
		pDM->EndFrame();
	}	

	AllControlDisplay(_T("CM_BO_CAT_ACQUISIT_NO"));
	*/

EFS_END
}

HBRUSH CBO_CAT_NUMBER_0000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
