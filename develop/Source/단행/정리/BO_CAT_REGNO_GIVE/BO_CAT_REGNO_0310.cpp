// BO_CAT_REGNO_0310.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_REGNO_0310.h"
//#include _T("BO_CAT_REGNO_0320_POPUP.h")

#include "..\..\..\관리\정리\BO_CAT_REG_SEARCH\BO_CAT_REG_SEARCH_POPUP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REGNO_0310 dialog


CBO_CAT_REGNO_0310::CBO_CAT_REGNO_0310(CESL_Mgr* pParent, CString sWhere/*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	sWhereGlo = sWhere;
	m_nOpenMode = -1;
	m_strMissRegNo = _T("");
	m_bSE = FALSE;
	//{{AFX_DATA_INIT(CBO_CAT_REGNO_0310)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CBO_CAT_REGNO_0310::~CBO_CAT_REGNO_0310()
{
	
}

VOID CBO_CAT_REGNO_0310::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REGNO_0310)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REGNO_0310, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REGNO_0310)
	ON_BN_CLICKED(IDC_BTN_Search, OnBTNSearch)
	ON_BN_CLICKED(IDC_BTN_RegSearch, OnBTNRegSearch)
	ON_BN_CLICKED(IDC_BTN_SELECT, OnBtnSelect)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabLostReg, OnSelchangetabLostReg)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_WM_CTLCOLOR() 
                            
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REGNO_0310 message handlers

BOOL CBO_CAT_REGNO_0310::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_REGNO_0310::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	iBookTotCount = 0; //총책수
	iCount = 0;
	if (InitESL_Mgr(_T("BO_CAT_REG_0310")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CTabCtrl	*m_tabPurchasing = (CTabCtrl*)GetDlgItem(IDC_tabLostReg);
	m_tabPurchasing->InsertItem(0, _T("선택"));
	m_tabPurchasing->InsertItem(1, _T("결과"));

	//등록구분, 책수 넣기
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_REG_0310"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_REG_0310 error") );
		return FALSE;
	}
	
	CString strTblName;
	if (m_bSE)
		strTblName = _T("SE_BOOK_TBL");
	else
		strTblName = _T("BO_BOOK_TBL");

	m_DM_SUB.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString sQuery_sub = _T("SELECT REG_CODE, COUNT(REG_CODE), '0' FROM ") + strTblName + _T(" WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ") + sWhereGlo;// division_value='_T(" + sRegCode[i-1] + ")'_T(";
	m_DM_SUB.RestructDataManager(sQuery_sub);
	INT iRowCount = m_DM_SUB.GetRowCount();
	 
	CComboBox * pRegCode;	
	pRegCode=(CComboBox *)GetDlgItem(IDC_COB_RegCode);

	for( INT i = 1 ; i <= iRowCount ; i++ ) {
		CString sRegCode;
		m_DM_SUB.GetCellData( i - 1 , 0 , sRegCode );
		pRegCode->AddString(sRegCode);
//		pGrid_sub->SetTextMatrix( i, 1, sRegCode);
//		sRegCodeGlo[i-1] = sRegCode;
		//책수
		CString sBookCount;
		m_DM_SUB.GetCellData( i - 1 , 1 , sBookCount );
		INT iBookCount = _ttoi( sBookCount );
		iBookTotCount = iBookTotCount + iBookCount;

//		pGrid_sub->SetTextMatrix( i, 2, sBookCount);
	}

	

	//pRegCode->SetItemData(1,_T("aa"));	
	//pRegCode->AddString(_T("EM"));
	//pRegCode->AddString(_T("BU"));
	pRegCode->SetCurSel(0);
	/*
	CString aa;
	pRegCode->GetLBText(0, aa );
	INT iTmp = pRegCode->GetTopIndex();
	pRegCode->GetLBText(1, aa );
	*/

	//CString aa = pRegCode->GetItemData(0);
	//m_combo.SetCursel(n);

	/*
	CESL_ControlMgr *pCM_3100 = FindCM(_T("CM_BO_CAT_REG_3100"));
	if (!pCM_3100) return FALSE;

	CESL_ControlMultiComboBox *pCM_Subject = (CESL_ControlMultiComboBox *)pCM_3100->FindControl(_T("등록구분"));
	if (!pCM_Subject) return FALSE;

	pCM_Subject->AddItem(_T("AA"), 0, 0);
	*/
	CESL_DataMgr *pDM_311_DROP;
	pDM_311_DROP= FindDM(_T("DM_BO_CAT_REG_311_DROP"));
	if ( NULL == pDM_311_DROP ) {
		AfxMessageBox ( _T("DM_BO_CAT_REG_311_DROP error") );
		return FALSE;
	}
	INT iDropCount = pDM_311_DROP->GetRowCount();
	if ( iDropCount != 0 ) {
		CString sDropCount;
		sDropCount.Format( _T("%d"), iDropCount );
		GetDlgItem(IDC_EDT_COUNT)->SetWindowText(sDropCount);
	}

	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_REGNO_0310::OnBTNSearch() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CButton * pBtn;
	pBtn = (CButton *)GetDlgItem( IDC_COB_RegCode );
	CString sRegCode;
	pBtn->GetWindowText( sRegCode );
	
	INT ids;
	CString sRegStart , sRegEnd;
	ids = GetControlData(_T("CM_BO_CAT_REG_0310"), _T("등록번호부터"), sRegStart);
	ids = GetControlData(_T("CM_BO_CAT_REG_0310"), _T("등록번호까지"), sRegEnd);

	if ( sRegStart == _T("") && sRegEnd == _T("") ) {
		AfxMessageBox ( _T("등록번호조건을 입력하세요.") );
		return;
	}
	else if ( sRegStart == _T("") && sRegEnd != _T("") ) {
		AfxMessageBox ( _T("시작 등록번호를 입력하세요.") );
		return;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_REG_0310_DROP"));

	//CString strWhere = _T("REG_NO >= 'EM0000000001' AND REG_NO <'EM0000000100' ORDER BY ACCESSION_NO");
	
	CString strWhere = _T("REG_CODE = '") + sRegCode + _T("'");
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	if ( sRegStart != _T("") && sRegEnd == _T("") ) {
// 		CString sRegStartNo;
// 		sRegStartNo.Format( _T("%s%010s") , sRegCode , sRegStart );
// 		strWhere = _T("REG_CODE = '") + sRegCode + _T("' AND REG_NO = '") + sRegStart + _T("' AND ") + strWhere;
// 	}
// 	if ( sRegStart != _T("") && sRegEnd != _T("") ) {
// 		CString sRegStartNo;
// 		sRegStartNo.Format( _T("%s%010s") , sRegCode , sRegStart );
// 		CString sRegEndNo;
// 		sRegEndNo.Format( _T("%s%010s") , sRegCode , sRegEnd );
// 		strWhere = _T("REG_NO >= ") + sRegStart + _T(" AND REG_NO <= ") + sRegEnd + _T(" AND ") + strWhere;
// 	}
	
	if ( sRegStart != _T("") && sRegEnd == _T("") ) {
		CString sRegStartNo;
		sRegStartNo = pParentMgr->m_pInfo->MakeRegNo(sRegCode , sRegStart );
		strWhere = _T("REG_CODE = '") + sRegCode + _T("' AND REG_NO = ") + sRegStart + _T(" AND ") + strWhere;
	}
	if ( sRegStart != _T("") && sRegEnd != _T("") ) {
		CString sRegStartNo;
		sRegStartNo = pParentMgr->m_pInfo->MakeRegNo(sRegCode , sRegStart );
		CString sRegEndNo;
		sRegEndNo = pParentMgr->m_pInfo->MakeRegNo(sRegCode , sRegEnd );
		strWhere = _T("REG_NO >= ") + sRegStart + _T(" AND REG_NO <= ") + sRegEnd + _T(" AND ") + strWhere;
	}
	//=====================================================
	
	

	if (pDM->RefreshDataManager(strWhere) < 0)	return;
	INT iRowCount;
	iRowCount = pDM->GetRowCount();
	if ( iRowCount < 1) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
		return;
	}

	CString strTmp;
	for (INT i = 0; i < iRowCount; i++) {
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		strTmp.Format(_T("%s%010s"), pDM->GetCellData(_T("등록코드"), i), pDM->GetCellData(_T("등록번호"), i));

		strTmp = pParentMgr->m_pInfo->MakeRegNo(pDM->GetCellData(_T("등록코드"), i),pDM->GetCellData(_T("등록번호"), i));
		//=====================================================
		

		pDM->SetCellData(_T("누락등록번호"), strTmp, i);
	}

	CString tmp = pDM->GetCellData(0,0);

/*
	CESL_DataMgr *pDM_UDF;
	pDM_UDF = FindDM(_T("DM_BO_CAT_REG_0310_UDF"));

	CString sRegSerial;
	sRegSerial = _T("EM00000000001");
	pDM_UDF->SetCellData( 0 , 0 , sRegSerial); 

	CString tmp = pDM_UDF->GetCellData( 0 , 0 );
*/

	m_DM_SUB.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
//	CString sQuery_sub = _T("SELECT REG_CODE, COUNT(REG_CODE) FROM BO_BOOK_TBL WHERE ") + sWhere;// division_value='_T(" + sRegCode[i-1] + ")'_T(";
//	m_DM_SUB.RestructDataManager(sQuery_sub);

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_REG_0310"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_REG_0310") );
		return;
	}
	pCM->AllControlDisplay();

	CTabCtrl	*m_tabPurchasing = (CTabCtrl*)GetDlgItem(IDC_tabLostReg);
	m_tabPurchasing->SetCurFocus(0);

EFS_END
}

VOID CBO_CAT_REGNO_0310::OnBTNRegSearch() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	//CBO_CAT_REGNO_0320_POPUP dlg(this);
	CBO_CAT_REG_SEARCH_POPUP dlg(this);
	dlg.DoModal();

EFS_END
}

VOID CBO_CAT_REGNO_0310::OnBtnSelect() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_ControlMgr *pCM_SUB;
	pCM_SUB = FindCM(_T("CM_BO_CAT_REG_0310"));
	pCM_SUB->FindControl(_T("GRD_0310"));
	
	CESL_Grid * pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM_SUB->FindControl(_T("GRD_0310"));

	INT iRow = pGrid_sub->GetRow();

	//COUNT CHECK
	CComboBox * pRegCode;	
	pRegCode=(CComboBox *)GetDlgItem(IDC_COB_RegCode);

	CString sRegCode;
	GetDlgItem(IDC_COB_RegCode)->GetWindowText(sRegCode);

	INT iBookCnt;
	INT nTotSelectedCount;
	CString sTmpData;
	INT nDMSubidx;
	for ( INT i = 0 ; i < m_DM_SUB.GetRowCount() ; i++ ){
		m_DM_SUB.GetCellData( i , 0 , sTmpData );
		if ( sRegCode == sTmpData ) {
			m_DM_SUB.GetCellData( i , 1 , sTmpData );
			iBookCnt = _ttoi ( sTmpData.GetBuffer(0) );
			m_DM_SUB.GetCellData( i, 2, sTmpData );
			nTotSelectedCount = _ttoi ( sTmpData.GetBuffer(0) );
			nDMSubidx = i;
			break;
		}
	}
	
	INT iGrdRow = pGrid_sub->GetRows();
	for ( i = 1 ; i < iGrdRow ; i++ ) {
		CString sCh = pGrid_sub->GetTextMatrix( i , 1 );
		if ( sCh == _T("V") ) {
			nTotSelectedCount ++ ;
			if ( nTotSelectedCount > iBookCnt ) {
				AfxMessageBox ( _T("책수를 확인하세요.") );
				pGrid_sub->SetTextMatrix( iRow , 1 , _T("") );
				return;
			}
		}
	}
	//책수 check 끝

//	CESL_ControlMgr *pCM_SUB;
	pCM_SUB = FindCM(_T("CM_BO_CAT_REG_0310"));
	pCM_SUB->FindControl(_T("GRD_0310"));
	
//	CESL_Grid * pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM_SUB->FindControl(_T("GRD_0310"));
	
	pDM_0310_DROP = FindDM(_T("DM_BO_CAT_REG_311_DROP"));
	
	pGrid_sub->SelectMakeList();

	INT idx = pGrid_sub->SelectGetHeadPosition();
	if (idx < 0) return;

	CString sItem;
	while (idx >= 0 )
	{
		sItem = pGrid_sub->GetTextMatrix( idx + 1, 2 );

		INT iSw = 0;
		for( INT i = 0 ; i < pDM_0310_DROP->GetRowCount() ; i++ ) {
			if ( sItem == pDM_0310_DROP->GetCellData( _T("누락등록번호"), i ) ) {
				iSw = 1; 
			}			
		}
		if ( iSw == 0 ) {
			pDM_0310_DROP->InsertRow(-1);
			pDM_0310_DROP->SetCellData(pDM_0310_DROP->GetRowCount() -1, 0 , sItem );
		}

		idx = pGrid_sub->SelectGetNext();
	}

	// 040305_HSS 등록구분별 선택된 갯수 넣기
	sTmpData.Format(_T("%d"), nTotSelectedCount);
	m_DM_SUB.SetCellData( nDMSubidx , 2 , sTmpData );
	
	INT iRowCount = pDM_0310_DROP->GetRowCount();
	CString sRowCount;
	sRowCount.Format( _T("%d") , iRowCount );
	GetDlgItem(IDC_EDT_COUNT)->SetWindowText(sRowCount);

EFS_END
}

BEGIN_EVENTSINK_MAP(CBO_CAT_REGNO_0310, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_REGNO_0310)
	ON_EVENT(CBO_CAT_REGNO_0310, IDC_GRD_0310, -600 /* Click */, OnClickGrd0310, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_CAT_REGNO_0310::OnClickGrd0310() 
{
EFS_BEGIN
	// TODO: Add your control notification handler code here
	CESL_ControlMgr *pCM_SUB;
	pCM_SUB = FindCM(_T("CM_BO_CAT_REG_0310"));
	pCM_SUB->FindControl(_T("GRD_0310"));
	
	CESL_Grid * pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM_SUB->FindControl(_T("GRD_0310"));

	INT iRow = pGrid_sub->GetRow();

	//COUNT CHECK
	CComboBox * pRegCode;	
	pRegCode=(CComboBox *)GetDlgItem(IDC_COB_RegCode);

	CString sRegCode;
	GetDlgItem(IDC_COB_RegCode)->GetWindowText(sRegCode);
	
	// 등록구분별 책수 가져오기
	INT nTotCnt;
	INT nTotSelectedCount;
	CString sTmpData;
	INT idx;
	for ( INT i = 0 ; i < m_DM_SUB.GetRowCount() ; i++ ){
		m_DM_SUB.GetCellData( i, 0, sTmpData );
		if ( sRegCode == sTmpData ) {
			m_DM_SUB.GetCellData( i, 1, sTmpData );
			nTotCnt = _ttoi ( sTmpData.GetBuffer(0) );
			m_DM_SUB.GetCellData( i, 2, sTmpData );
			nTotSelectedCount = _ttoi ( sTmpData.GetBuffer(0) );
			idx = i;
			break;
		}
	}

	INT iGrdRow = pGrid_sub->GetRows();
	for ( i = 1 ; i < iGrdRow ; i++ ) {
		CString sCh = pGrid_sub->GetTextMatrix( i , 1 );
		if ( sCh == _T("V") ) {
			nTotSelectedCount ++ ;
			if ( nTotSelectedCount > nTotCnt ) {
				CString sMessage;
				sMessage.Format( _T("%d개의 누락등록번호만 선택할 수 있습니다."), nTotCnt );
				AfxMessageBox( sMessage );
				pGrid_sub->SetTextMatrix( iRow , 1 , _T("") );
				return;
			}
		}
	}
	
	CString sItem = pGrid_sub->GetTextMatrix( iRow , 2 );
	CString sCh = pGrid_sub->GetTextMatrix( iRow , 1 );
	
	pDM_0310_DROP = FindDM(_T("DM_BO_CAT_REG_311_DROP"));
	INT iRowCount = pDM_0310_DROP->GetRowCount();

	if ( sCh == _T("V") ) {
		for ( INT i = 0 ; i < iRowCount ; i++ ) {
			CString sResult = pDM_0310_DROP->GetCellData( i , 0 );
			if ( sResult == sItem ) {
				AfxMessageBox ( _T("이미 선택된 자료입니다.") );
				pGrid_sub->SetTextMatrix( iRow , 1 , _T("") );
				return;					
			}
		}
		iCount++;
	}
	else {
		for ( INT i = 0 ; i < iRowCount ; i++ ) {
			CString sResult = pDM_0310_DROP->GetCellData( i , 0 );
			if ( sResult == sItem ) {
				iCount--;
			}
		}
	}

	m_nTotCount ++;

EFS_END
}

VOID CBO_CAT_REGNO_0310::OnSelchangetabLostReg(NMHDR* pNMHDR, LRESULT* pResult) 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CTabCtrl	*pTab = (CTabCtrl*)GetDlgItem(IDC_tabLostReg);
	if (pTab == NULL) return;

	INT nSel = pTab->GetCurSel();


	CRect rect;
	GetDlgItem(IDC_BTN_SELECT)->GetWindowRect(rect);

	ScreenToClient(rect);


	if (nSel == 0)
	{
		GetDlgItem(IDC_GRD_0310)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_GRD_311)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SELECT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_DELETE)->ShowWindow(SW_HIDE);

	}
	else if (nSel == 1)
	{
		GetDlgItem(IDC_GRD_0310)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GRD_311)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_SELECT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_DELETE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_DELETE)->MoveWindow(rect);	


		CESL_ControlMgr *pCM_SUB = FindCM(_T("CM_BO_CAT_REG_0310"));
		if (pCM_SUB == NULL) return;

		CESL_Grid * pGrid_sub = (CESL_Grid*)pCM_SUB->FindControl(_T("GRD_311"));
		if (pGrid_sub == NULL) return;

		pGrid_sub->Display();	
		
		//m_marcEditor.ApplyEdit();
	}

	*pResult = 0;

EFS_END
}

VOID CBO_CAT_REGNO_0310::OnBtnDelete() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_ControlMgr *pCM_SUB;
	pCM_SUB = FindCM(_T("CM_BO_CAT_REG_0310"));	
	
	CESL_Grid * pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM_SUB->FindControl(_T("GRD_311"));

	pDM_0310_DROP = FindDM(_T("DM_BO_CAT_REG_311_DROP"));

	//=====================================
	//2008.07.03 ADD BY PJW : 삭제된 등록번호는 선택된 DM에서 지운다.
	CESL_Grid * pGrid_0310 = NULL;
	pGrid_0310 = (CESL_Grid*)pCM_SUB->FindControl(_T("GRD_0310"));
	CString str311RegNo;
	CString str0310RegNo;
	INT n0310Idx;
	INT n0310Cnt;
	pGrid_0310->SelectMakeList();
	n0310Cnt = pGrid_0310->SelectGetCount();
	n0310Idx = pGrid_0310->SelectGetHeadPosition(); 
	//=====================================

	pGrid_sub->SelectMakeList();

	INT idx = pGrid_sub->SelectGetTailPosition();
	if (idx < 0) return;

	while (idx >= 0 )
	{
		//=====================================
		//2008.07.03 ADD BY PJW : 삭제된 등록번호는 선택된 DM에서 지운다.
		str311RegNo = pGrid_sub->GetTextMatrix(idx+1,2);
		for( INT i = 0 ; i < n0310Cnt ; i++ )
		{
			str0310RegNo = pGrid_0310->GetTextMatrix(n0310Idx+1,2);
			if( str311RegNo == str0310RegNo )
			{
				pGrid_0310->SetTextMatrix( n0310Idx+1 , 1, _T(""));
				m_DM_SUB.SetCellData( n0310Idx , 2 , _T("") );
				pGrid_0310->SelectMakeList();
				n0310Cnt = pGrid_0310->SelectGetCount();
				n0310Idx = pGrid_0310->SelectGetHeadPosition(); 
				break;
			}
			n0310Idx = pGrid_0310->SelectGetNext();

		}
		//=====================================
		pDM_0310_DROP->DeleteRow(idx);
		idx = pGrid_sub->SelectGetPrev();
	}

	pGrid_sub->Display();

	/////////////////////////////////////////////////////////////////////
	INT nCnt = m_DM_SUB.GetRowCount();
	for(INT i=0 ; i<nCnt ; i++)
	{
		m_DM_SUB.SetCellData( i , 2 , _T("0") );
	}
	/////////////////////////////////////////////////////////////////////

	//선택한 개수
	INT iRowCount = pDM_0310_DROP->GetRowCount();
	CString sRowCount;
	sRowCount.Format( _T("%d") , iRowCount );
	GetDlgItem(IDC_EDT_COUNT)->SetWindowText(sRowCount);

EFS_END
}

VOID CBO_CAT_REGNO_0310::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here
	if ( pDM_0310_DROP == NULL ) {
		AfxMessageBox ( _T("선택한 개수가 없습니다.") );
		return;
	}

	/*
	INT iRowCount = pDM_0310_DROP->GetRowCount();
	if ( iRowCount < 1 ) {
		AfxMessageBox ( _T("선택한 개수가 없습니다.") );
		return;
	}
	CString sRowCount;
	sRowCount.Format( _T("%d") , iRowCount );
	GetDlgItem(IDC_EDT_COUNT)->SetWindowText(sRowCount);
	*/
	
	if(m_nOpenMode != 1000)
	{
		CESL_DataMgr *pDM_0300 = FindDM( _T("DM_BO_CAT_REG_0300") );
		if ( pDM_0300 == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_REG_0300") );
			return;
		}
		//INT iRowCount = pDM_0300->GetRowCount();
		CESL_DataMgr *pDM_0310_DROP = FindDM( _T("DM_BO_CAT_REG_311_DROP") );
		if ( pDM_0310_DROP == NULL ) {
			AfxMessageBox( _T("DM_BO_CAT_REG_0300") );
			return;
		}
		INT iRowCount = pDM_0310_DROP->GetRowCount();
		if ( iRowCount == 0 ) {
			AfxMessageBox( _T("선택한 개수가 없습니다.") );
			return;
		}
		
		CString sSelectCount;
		GetControlData( _T("CM_BO_CAT_REG_0310"), _T("선택개수"), sSelectCount );
		INT iSelectCount;
		iSelectCount = _ttoi( sSelectCount.GetBuffer(0) );

		//iRowCount = pDM_0300->GetRowCount();
		//m_DM_SUB.GetCellData( i - 1 , 1 , iRowCount );
		iRowCount = iBookTotCount;
		if ( iRowCount != iSelectCount ) {
			AfxMessageBox( _T("선택한 개수가 책수와 틀립니다.") );
			return;
		}	
	}
	//등록구분별 선택된 개수 파악
	CESL_ControlMgr *pCM_SUB;
	pCM_SUB = FindCM(_T("CM_BO_CAT_REG_0310"));

	CESL_Grid * pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM_SUB->FindControl(_T("GRD_0310"));

	INT iBookCount = 0;
	INT iGrdRow = pGrid_sub->GetRows();

	CDialog::OnOK();

EFS_END
}




HBRUSH CBO_CAT_REGNO_0310::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
