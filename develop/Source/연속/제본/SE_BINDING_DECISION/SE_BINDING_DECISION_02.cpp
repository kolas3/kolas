// SE_BINDING_DECISION_02.cpp : implementation file
//

#include "stdafx.h"
#include "SE_BINDING_DECISION_02.h"
#include "..\..\수서\SE_API\SeApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_DECISION_02 dialog

CSE_BINDING_DECISION_02::CSE_BINDING_DECISION_02(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pMgr = pParent;
}

CSE_BINDING_DECISION_02::~CSE_BINDING_DECISION_02()
{
}

VOID CSE_BINDING_DECISION_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_BINDING_DECISION_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CSE_BINDING_DECISION_02::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BEGIN_MESSAGE_MAP(CSE_BINDING_DECISION_02, CDialog)
	//{{AFX_MSG_MAP(CSE_BINDING_DECISION_02)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_BINDING_DECISION_02 message handlers

BOOL CSE_BINDING_DECISION_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr(_T("SM_SE_BINDING_DECISION_02")) < 0 ) {

		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_BINDING_DECISION_02::OnOK() 
{
   

	INT ids;

	CString strTitle, strPublisher, strMediaCode, strFreq, strPubStatus, strDateFrom, strDateTo;

	CString strOption, strWhere;

	CString sCondExp ;					// DB에 날릴 쿼리
	CString sClassExp = _T("");         // 검색엔진을 위한 쿼리

	ids = GetControlData( _T("CM_SE_BINDING_DECISION_02"), _T("TITLE")		, strTitle		);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_DECISION_02"), _T("PUBLISHER")	, strPublisher	);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_DECISION_02"), _T("MED_CLASS")	, strMediaCode	);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_DECISION_02"), _T("FREQ")		, strFreq		);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_DECISION_02"), _T("PUB_STATUS")	, strPubStatus	);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_DECISION_02"), _T("DATE_FROM")	, strDateFrom	);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_BINDING_DECISION_02"), _T("DATE_TO")	, strDateTo		);
	if( ids ) return;


	CSeApi api(this);
	api.MakeIndexWord(strTitle);
	api.MakeIndexWord(strPublisher);



	strMediaCode.TrimLeft();		strMediaCode.TrimRight();
	strFreq.TrimLeft();				strFreq.TrimRight();
	strPubStatus.TrimLeft();		strPubStatus.TrimRight();
	strDateFrom.TrimLeft();			strDateFrom.TrimRight();
	strDateTo.TrimLeft();			strDateTo.TrimRight();


	strWhere += _T("S.REC_KEY = I.REC_KEY");

	
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

	BOOL bIsParenthesisNeed = FALSE;

	if( strWhere != _T("NO_DATA") ) {
		strOption = _T(" AND I.REC_KEY IN (SELECT DISTINCT SPECIES_KEY FROM SE_BOOK_TBL WHERE WORKING_STATUS = 'SEB111N' ");
		strWhere += strOption;
		bIsParenthesisNeed = TRUE;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 일자부터 ~ 일자까지
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if( strWhere != _T("NO_DATA") ) {
	
		if( !strDateFrom.IsEmpty() ) {
			
			if( !strDateTo.IsEmpty() ) {
				strOption.Format( _T(" AND BINDING_TRANSFER_DATE >= '%s' AND BINDING_TRANSFER_DATE <= '%s' )"), strDateFrom, strDateTo );
				strWhere += strOption;
				bIsParenthesisNeed = FALSE;
			}
			else {
				strOption.Format( _T(" AND BINDING_TRANSFER_DATE >= '%s' )"), strDateFrom );
				strWhere += strOption;
				bIsParenthesisNeed = FALSE;
			}
		}
		else {
			if( !strDateTo.IsEmpty() ) {
				strOption.Format( _T(" AND BINDING_TRANSFER_DATE = '%s' )"), strDateTo );
				strWhere += strOption;
				bIsParenthesisNeed = FALSE;
			}
		}
	}
	
	if(bIsParenthesisNeed)
	{
		strWhere += _T(")");
	}
	
	m_strQuery	= strWhere;
	SendQueryToParent();

	return;

	CDialog::OnOK();
}

VOID CSE_BINDING_DECISION_02::SendQueryToParent()
{
	:: SendMessage(	m_pMgr->GetSafeHwnd(), SENDQUERY , 0, 0);
}

INT CSE_BINDING_DECISION_02::DoIndexSearch(CString f_strWhere, CStringList& f_slistKey)
{
EFS_BEGIN

	INT i, ids, nRowCnt;
	CString strRecKey, strInQuery;

    CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_BINDING_DECISION_01_KEYLIST"));
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

CString CSE_BINDING_DECISION_02::MakeReckeyInQuery(CStringList &f_keyList)
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


BOOL CSE_BINDING_DECISION_02::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN ) {
		
		OnOK();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

//(SELECT COUNT(REC_KEY) FROM SE_BOOK_TBL B WHERE B.ACQ_YEAR = S.ACQUISIT_YEAR AND B.RECEIPT_NO = S.SEQ_NO AND S.WORKING_STATUS IS NOT NULL)

HBRUSH CSE_BINDING_DECISION_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
