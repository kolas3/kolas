// SE_BINDING_ORDER_02.cpp : implementation file
//

#include "stdafx.h"
#include "SE_BINDING_ORDER_02.h"
#include "../SE_BINDING_DECISION/SE_BINDING_DECISION_07.h"
#include "..\..\수서\SE_API\SeApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_ORDER_02 dialog


CSE_BINDING_ORDER_02::CSE_BINDING_ORDER_02(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pMgr = pParent;
}

CSE_BINDING_ORDER_02::~CSE_BINDING_ORDER_02()
{
}

VOID CSE_BINDING_ORDER_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_BINDING_ORDER_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CSE_BINDING_ORDER_02::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BEGIN_MESSAGE_MAP(CSE_BINDING_ORDER_02, CDialog)
	//{{AFX_MSG_MAP(CSE_BINDING_ORDER_02)
	ON_EN_CHANGE(IDC_eACQ_YEAR, OnChangeeACQYEAR)
	ON_BN_CLICKED(IDC_bORDER_NO, OnbORDERNO)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_ORDER_02 message handlers

BOOL CSE_BINDING_ORDER_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_SE_BINDING_ORDER_02")) < 0 ) {

		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}
	//2018년 공공도서관 책이음서비스 확대구축 및 시스템개선
	//KOL.RED.019-003 JY 연속간행물 기능 안정화
	//==============================================
	CSeApi api(this);
	m_sAcquisitYear = api.GetCurrentYear();
	//==============================================

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_BINDING_ORDER_02::OnOK() 
{
    INT ids;

	CString strTitle, strPublisher, strMediaCode, strFreq, strPubStatus, strAcqYear, strSeqNo, strSearchCode;

	CString strOption, strWhere;

	CString sCondExp ;					// DB에 날릴 쿼리
	CString sClassExp = _T("");         // 검색엔진을 위한 쿼리

	ids = GetControlData( _T("CM_SE_BINDING_ORDER_02"), _T("TITLE")		, strTitle		);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_ORDER_02"), _T("PUBLISHER")	, strPublisher	);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_ORDER_02"), _T("MED_CLASS")	, strMediaCode	);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_ORDER_02"), _T("FREQ")		, strFreq		);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_ORDER_02"), _T("PUB_STATUS")	, strPubStatus	);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_ORDER_02"), _T("ACQ_YEAR")	, strAcqYear	);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_ORDER_02"), _T("ORDER_NO")	, strSeqNo		);
	if( ids ) return;

	ids = GetControlData( _T("CM_SE_BINDING_ORDER_02"), _T("SEARCH_CODE")	, strSearchCode		);
	if( ids ) return;
	

	CSeApi api(this);
	api.MakeIndexWord(strTitle);
	api.MakeIndexWord(strPublisher);


	strMediaCode.TrimLeft();		strMediaCode.TrimRight();
	strFreq.TrimLeft();				strFreq.TrimRight();
	strPubStatus.TrimLeft();		strPubStatus.TrimRight();
	strAcqYear.TrimLeft();			strAcqYear.TrimRight();
	strSeqNo.TrimLeft();			strSeqNo.TrimRight();


	strWhere += _T("BIND.BOOK_KEY = BOOK.REC_KEY AND I.REC_KEY = BOOK.SPECIES_KEY");

	
	//title
	if( !strTitle.IsEmpty() ) {
		
		if(ORA_CATSEARCH == m_pInfo->GetSearchIndexType())
			strOption.Format(_T(" AND CATSEARCH(I.IDX_TITLE, '%s*', NULL) > 0"), strTitle);
		else
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY,SE_IDX_TITLE, '%s*')"),strTitle);

		strWhere += strOption;
		
	}


	//publisher
	if( !strPublisher.IsEmpty() ) {

		if(ORA_CATSEARCH == m_pInfo->GetSearchIndexType())
			strOption.Format(_T(" AND CATSEARCH(I.IDX_PUBLISHER, '%s*', NULL) > 0"), strPublisher);
		else
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY,SE_IDX_PUBLISHER, '%s*')"),strPublisher);

		strWhere += strOption;

	}

	//mat_code
	if( !strMediaCode.IsEmpty() ) {
		strWhere += _T(" AND I.MEDIA_CODE = '") + strMediaCode + _T("' ");
	}

	//pub_freq
	if( !strFreq.IsEmpty() ) {
		strWhere += _T(" AND I.PUB_FREQ = '") + strFreq + _T("' ");
	}

	//pub_status
	if( !strPubStatus.IsEmpty() ) {

		strWhere += _T(" AND I.PUB_STATUS = '") + strPubStatus + _T("' ");
	}

	// 수입년도	// 발주차수
	if( strWhere != _T("NO_DATA") ) {

		if( !strAcqYear.IsEmpty() ) {
			strOption.Format( _T(" AND BOOK.ACQ_YEAR = '%s' "), strAcqYear );
			strWhere += strOption;
		}

		if( !strSeqNo.IsEmpty() && strSeqNo != _T("전체") ) { 
			strOption.Format( _T(" AND BIND.BINDING_SEQ_NO = '%s' "), strSeqNo );
			strWhere += strOption;
		}

		if( strSearchCode == _T("발주대상") ) {
			strOption.Format( _T(" AND BIND.BINDING_STATUS = '1' ") );
			strWhere += strOption;
		}
		else {
			strOption.Format( _T(" AND BIND.BINDING_STATUS = '2' ") );
			strWhere += strOption;
		}
	}


	m_strQuery = strWhere;

	SendQueryToParent();

	return;

	CDialog::OnOK();
}

VOID CSE_BINDING_ORDER_02::SendQueryToParent()
{
	:: SendMessage(	m_pMgr->GetSafeHwnd(), SENDQUERY , 0, 0);
}

INT CSE_BINDING_ORDER_02::DoIndexSearch(CString f_strWhere, CStringList& f_slistKey)
{
EFS_BEGIN

	INT i, ids, nRowCnt;
	CString strRecKey, strInQuery;

    CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_ORDER_01_KEYLIST"));
	if( pDataMgr == NULL ) {
		AfxMessageBox( _T("") ) ;
		return -1;
	}

	ids = pDataMgr->RefreshDataManager( f_strWhere );
	if( ids ) {
		AfxMessageBox( _T("RefreshDataManager Error!") ) ;
		return -1;
	}
	
	nRowCnt = pDataMgr->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ ) {

		pDataMgr->GetCellData( _T("REC_KEY"), i, strRecKey);
		f_slistKey.AddTail( strRecKey );
	}

	return 0;

EFS_END
return -1;
}

CString CSE_BINDING_ORDER_02::MakeReckeyInQuery(CStringList &f_keyList)
{
EFS_BEGIN

	CString Inquery;
	INT cnt = f_keyList.GetCount();
	
	if(cnt == 0) return _T("");
	
	POSITION pos = f_keyList.GetHeadPosition();
	Inquery.Format(_T("%s"), f_keyList.GetNext(pos));
	CString subquery;

	while(pos != NULL)
	{
		subquery.Format(_T(",%s"), f_keyList.GetNext(pos));
		Inquery += subquery;
	}
	CString tmpstr ;
	tmpstr.Format(_T("(%s)"), Inquery);

	return tmpstr;

EFS_END
return _T("");
}

BOOL CSE_BINDING_ORDER_02::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN ) {
		
		OnOK();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CSE_BINDING_ORDER_02::OnChangeeACQYEAR() 
{
	INT ids;
	CString sTemp;

	GetDlgItem(IDC_eACQ_YEAR) -> GetWindowText(sTemp);
	sTemp.TrimLeft();
	sTemp.TrimRight();

	m_sAcquisitYear = sTemp;

	// MOD wsw 2003.06.29 
	// 컨트롤에 발주차수 보여줌
	//ids = SelectOrderNo();
	//if( ids ) {
	//	AfxMessageBox( _T("SelectBaljuChasu Error") );
	//	return;
	//}	
}

VOID CSE_BINDING_ORDER_02::OnbORDERNO() 
{
	INT ids;

	CSE_BINDING_DECISION_07 dlg( this );

	dlg.SetAcqYear(m_sAcquisitYear);
	
	if( IDCANCEL == dlg.DoModal() ) 
		return;

	ids = SetControlData( _T("CM_SE_BINDING_ORDER_02"), _T("ACQ_YEAR"), dlg.GetAcqYear(), -1 );
	if( ids < 0 ) return ;

	ids = SetControlData( _T("CM_SE_BINDING_ORDER_02"), _T("ORDER_NO"), dlg.GetSeqNo(), -1 );
	if( ids < 0 ) return ;

}

/*
INT CSE_BINDING_ORDER_02::SelectOrderNo()
{
	CComboBox* pCombo;
	CString sSql, sNo, sAcquisitYear;

	INT ids, nRowCnt;

	// 수입년도를 구한다.
	ids = GetControlData( _T("CM_SE_BINDING_ORDER_02"), _T("ACQ_YEAR"), sAcquisitYear, -1 ) ;
	if( ids ) return -1;

	if( sAcquisitYear.IsEmpty() ) return -1;

	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_ORDER_NO"));
	if( pDataMgr == NULL ) return -2;

	
	sSql += _T("ACQUISIT_YEAR = '") + sAcquisitYear + _T("' ");
	
	// 대상 : 발주대상이거나 발주중인 자료
	sSql += _T(" AND (WORKING_STATUS = '0' OR WORKING_STATUS = '1' ) ");

	ids = pDataMgr->RefreshDataManager( sSql );
	if( ids ) {
		AfxMessageBox( _T("RefreshDataManager Error!") ) ;
		return -3;
	}

	nRowCnt = pDataMgr->GetRowCount();

	if( nRowCnt > 0 ) {
		ids = pDataMgr->SORT( _T("SEQ_NO") );
		if( ids ) return -4;
	}

	pCombo = (CComboBox*) GetDlgItem(IDC_cSERIAL_NO);
	pCombo -> ResetContent();

	for(INT i = 0 ; i < nRowCnt ; i++) {

		ids = GetDataMgrData( _T("DM_SE_BINDING_ORDER_NO"), _T("SEQ_NO"), sNo, i );
		if( ids ) return -5;

		pCombo -> AddString(sNo);
	}

	pCombo -> AddString(_T("전체"));
	pCombo -> SelectString(0,_T("전체"));

	return 0;
}
*/

HBRUSH CSE_BINDING_ORDER_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
