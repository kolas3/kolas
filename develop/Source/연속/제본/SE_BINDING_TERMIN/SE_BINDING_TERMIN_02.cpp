// SE_BINDING_TERMIN_02.cpp : implementation file
//

#include "stdafx.h"
#include "SE_BINDING_TERMIN_02.h"
#include "../SE_BINDING_DECISION/SE_BINDING_DECISION_07.h"
#include "..\..\수서\SE_API\SeApi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_TERMIN_02 dialog

CSE_BINDING_TERMIN_02::CSE_BINDING_TERMIN_02(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pMgr = pParent;
	m_nSearchOption = 0;
}

CSE_BINDING_TERMIN_02::~CSE_BINDING_TERMIN_02()
{
}

VOID CSE_BINDING_TERMIN_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_BINDING_TERMIN_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CSE_BINDING_TERMIN_02::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BEGIN_MESSAGE_MAP(CSE_BINDING_TERMIN_02, CDialog)
	//{{AFX_MSG_MAP(CSE_BINDING_TERMIN_02)
	ON_EN_CHANGE(IDC_eACQ_YEAR, OnChangeeACQYEAR)
	ON_BN_CLICKED(IDC_bORDER_NO, OnbORDERNO)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_TERMIN_02 message handlers

BOOL CSE_BINDING_TERMIN_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_SE_BINDING_TERMIN_02")) < 0 ) {

		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_BINDING_TERMIN_02::OnOK() 
{
    INT ids;

	CString strTitle, strPublisher, strMediaCode, strFreq, strPubStatus, strAcqYear, strSeqNo, strSearchCode;

	CString strOption, strWhere;

	CString sCondExp ;					// DB에 날릴 쿼리
	CString sClassExp = _T("");         // 검색엔진을 위한 쿼리

	ids = GetControlData( _T("CM_SE_BINDING_TERMIN_02"), _T("TITLE")		, strTitle		);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_TERMIN_02"), _T("PUBLISHER")	, strPublisher	);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_TERMIN_02"), _T("MED_CLASS")	, strMediaCode	);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_TERMIN_02"), _T("FREQ")		, strFreq		);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_TERMIN_02"), _T("PUB_STATUS")	, strPubStatus	);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_TERMIN_02"), _T("ACQ_YEAR")	, strAcqYear	);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_TERMIN_02"), _T("ORDER_NO")	, strSeqNo		);
	if( ids ) return;

	ids = GetControlData( _T("CM_SE_BINDING_TERMIN_02"), _T("SEARCH_CODE")	, strSearchCode		);
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

		if( strSearchCode == _T("검수대상") ) {
			strOption.Format( _T(" AND BIND.BINDING_STATUS = '2' ") );
			m_nSearchOption = 2;
			strWhere += strOption;
		}
		else if( strSearchCode == _T("검수완료") ) {
			strOption.Format( _T(" AND BIND.BINDING_STATUS = '3' ") );
			m_nSearchOption = 3;
			strWhere += strOption;
		}
		else if( strSearchCode == _T("검수포기") ) {
			strOption.Format( _T(" AND BIND.BINDING_STATUS = '4' ") );
			m_nSearchOption = 4;
			strWhere += strOption;
		}
	}


	m_strQuery = strWhere;

	SendQueryToParent();

	return;

	CDialog::OnOK();
}

VOID CSE_BINDING_TERMIN_02::SendQueryToParent()
{
	:: SendMessage(	m_pMgr->GetSafeHwnd(), SENDQUERY , 0, 0);
}

INT CSE_BINDING_TERMIN_02::DoIndexSearch(CString f_strWhere, CStringList& f_slistKey)
{
EFS_BEGIN

	INT i, ids, nRowCnt;
	CString strRecKey, strInQuery;

    CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_TERMIN_01_KEYLIST"));
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

		pDataMgr->GetCellData(_T("REC_KEY"), i, strRecKey);
		f_slistKey.AddTail( strRecKey );
	}

	return 0;

EFS_END
return -1;
}

CString CSE_BINDING_TERMIN_02::MakeReckeyInQuery(CStringList &f_keyList)
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

BOOL CSE_BINDING_TERMIN_02::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN ) {
		
		OnOK();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CSE_BINDING_TERMIN_02::OnChangeeACQYEAR() 
{
	CString sTemp;

	GetDlgItem(IDC_eACQ_YEAR) -> GetWindowText(sTemp);
	sTemp.TrimLeft();
	sTemp.TrimRight();

	m_sAcquisitYear = sTemp;
}

//========================================================================
// [차수정보 Dlg를 보여주고 수입년도 차수정보를 얻는다.]
//========================================================================
VOID CSE_BINDING_TERMIN_02::OnbORDERNO() 
{
	INT ids;

	CSE_BINDING_DECISION_07 dlg( this );

	//--------------------------------------------------------------------
    // 2005-10-28 by weedblack: 입수년도를 받아 넘겨준다.
	//--------------------------------------------------------------------
	GetControlData( _T("CM_SE_BINDING_TERMIN_02"), _T("ACQ_YEAR")	, m_sAcquisitYear	);
	m_sAcquisitYear.TrimLeft();
	m_sAcquisitYear.TrimRight();
	dlg.SetAcqYear(m_sAcquisitYear);
	
	if( IDCANCEL == dlg.DoModal() ) 
		return;

	ids = SetControlData( _T("CM_SE_BINDING_TERMIN_02"), _T("ACQ_YEAR"), dlg.GetAcqYear(), -1 );
	if( ids < 0 ) return ;

	ids = SetControlData( _T("CM_SE_BINDING_TERMIN_02"), _T("ORDER_NO"), dlg.GetSeqNo(), -1 );
	if( ids < 0 ) return ;
}



HBRUSH CSE_BINDING_TERMIN_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
