// MISSING_DATA_SEARCH.cpp : implementation file
//

#include "stdafx.h"
#include "MISSING_DATA_SEARCH.h"
#include "..\SE_API\SeApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMISSING_DATA_SEARCH dialog

CMISSING_DATA_SEARCH::CMISSING_DATA_SEARCH(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_pMgr = pParent;
}

CMISSING_DATA_SEARCH::~CMISSING_DATA_SEARCH()
{
}

VOID CMISSING_DATA_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMISSING_DATA_SEARCH)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CMISSING_DATA_SEARCH::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BEGIN_MESSAGE_MAP(CMISSING_DATA_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CMISSING_DATA_SEARCH)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMISSING_DATA_SEARCH message handlers

BOOL CMISSING_DATA_SEARCH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(InitESL_Mgr(_T("SM_SE_MISSING_SEARCH_DON")) < 0)	{
		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}
	
	((CButton*)GetDlgItem(IDC_rCLAIM1))->SetCheck(1);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMISSING_DATA_SEARCH::OnOk() 
{
	MakeQuery();
	SendQueryToParent();

	return;

	CDialog::OnOK();	
}

VOID CMISSING_DATA_SEARCH::MakeQuery()
{
    INT ids;
	
	CString sTodayDate, sData;
	CString sTitleFrom, sTitleTo, sFreq, sProviderCode, sPubDate1, sPubDate2;
	CString sControlFrom,sControlTo,sIssnFrom, sIssnTo, sClaimSeq1, sClaimSeq2;

	CString strOption, strWhere;

	CString sCondExp ;					// DB에 날릴 쿼리
	CString sClassExp = _T("");         // 검색엔진을 위한 쿼리

	ids = GetControlData( _T("CM_SE_MISSING_SEARCH_DON"), _T("TITLE_FROM")	, sTitleFrom	);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_MISSING_SEARCH_DON"), _T("TITLE_TO")	, sTitleTo		);
	if( ids ) return;

	ids = GetControlData( _T("CM_SE_MISSING_SEARCH_DON"), _T("FREQ")		, sFreq	);
	if( ids ) return;
	
	ids = GetControlData( _T("CM_SE_MISSING_SEARCH_DON"), _T("CONTROLNO_FROM")	, sControlFrom		);
	if( ids ) return;

	ids = GetControlData( _T("CM_SE_MISSING_SEARCH_DON"), _T("ISSN1")		, sIssnFrom		);
	if( ids ) return;

	ids = GetControlData( _T("CM_SE_MISSING_SEARCH_DON"), _T("ISSN2")		, sIssnTo		);
	if( ids ) return;

	ids = GetControlData( _T("CM_SE_MISSING_SEARCH_DON"), _T("DONATOR_NAME")	, sProviderCode	);
	if( ids ) return;

	ids = GetControlData( _T("CM_SE_MISSING_SEARCH_DON"), _T("CLAIM_SEQ1")	, sClaimSeq1	);
	if( ids ) return;

	ids = GetControlData( _T("CM_SE_MISSING_SEARCH_DON"), _T("CLAIM_SEQ2")	, sClaimSeq2	);
	if( ids ) return;

	ids = GetControlData( _T("CM_SE_MISSING_SEARCH_DON"), _T("PUB_DATE1")	, sPubDate1	);
	if( ids ) return;

	ids = GetControlData( _T("CM_SE_MISSING_SEARCH_DON"), _T("PUB_DATE2")	, sPubDate2	);
	if( ids ) return;

	// 오늘날짜
	CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_MISSING_DON_01"));
	if( pDataMgr == NULL ) return ;

	CTime time = pDataMgr->GetDBTime();
	sTodayDate.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );


	sTitleFrom.TrimLeft();		sTitleFrom.TrimRight();
	sTitleTo.TrimLeft();		sTitleTo.TrimRight();
	sFreq.TrimLeft();			sFreq.TrimRight();
	sControlFrom.TrimLeft();	sControlFrom.TrimRight();
	sIssnFrom.TrimLeft();		sIssnFrom.TrimRight();
	sIssnTo.TrimLeft();			sIssnTo.TrimRight();
	sClaimSeq1.TrimLeft();		sClaimSeq1.TrimRight();
	sClaimSeq2.TrimLeft();		sClaimSeq2.TrimRight();
	sPubDate1.TrimLeft();		sPubDate1.TrimRight();
	sPubDate2.TrimLeft();		sPubDate2.TrimRight();

	CSeApi api(this);
	api.MakeIndexWord(sTitleFrom);
	api.MakeIndexWord(sTitleTo);

	// ===========================================================================================================
	// title
	// ===========================================================================================================
	if( !sTitleFrom.IsEmpty() ) {
		
		if( !sTitleTo.IsEmpty() ) {
			strOption.Format(_T(" I.IDX_ITITLE >= '%s' AND I.IDX_ITITLE <= '%s' "), sTitleFrom, sTitleTo);
			strWhere += strOption;
		}
		else {		


if(ORA_CATSEARCH == m_pInfo->GetSearchIndexType())
	strOption.Format(_T(" CATSEARCH(I.IDX_TITLE, '%s*', NULL) > 0"), sTitleFrom);
else
	strOption.Format(_T(" &ECOSEARCH(I.REC_KEY,SE_IDX_TITLE, '%s*')"),sTitleFrom);


			strWhere += strOption;
		}
	}
	else {
	
		if( !sTitleTo.IsEmpty() ) {


if(ORA_CATSEARCH == m_pInfo->GetSearchIndexType())
	strOption.Format(_T(" CATSEARCH(I.IDX_TITLE, '%s*', NULL) > 0"), sTitleTo);
else
	strOption.Format(_T(" &ECOSEARCH(I.REC_KEY,SE_IDX_TITLE, '%s*')"),sTitleTo);

			strWhere += strOption;
		}
	}

	// ===========================================================================================================
	// pub_freq
	// ===========================================================================================================
	if( !sFreq.IsEmpty() ) {

		if( strWhere.IsEmpty() )
			strWhere = _T(" I.PUB_FREQ = '")	+ sFreq + _T("' ");
		else
			strWhere += _T(" AND I.PUB_FREQ = '") + sFreq + _T("' ");
	}

	// ===========================================================================================================
	// 제어번호
	// ===========================================================================================================
	if( !sControlFrom.IsEmpty() ) {

		if( strWhere.IsEmpty() )
			strWhere = _T(" I.CONTROL_NO = '") + sControlFrom + _T("' ");
		else
			strWhere += _T(" AND I.CONTROL_NO = '") + sControlFrom + _T("' ");
	}


	if( strWhere != _T("") ) {
		strOption = _T(" AND C.ACQ_TYPE_VOL_KEY (+) = A.REC_KEY ")
					_T(" AND A.ACQ_KEY = M.REC_KEY ")
					_T(" AND I.REC_KEY = M.SPECIES_KEY ")
					_T(" AND M.CLAIM_YN = 'Y' ")
					_T(" AND A.MISS_BOOK_CNT > 0 AND A.CHECKIN_STATUS <> '1' ") ;
		strWhere += strOption;
	} 
	else {
		strOption = _T(" C.ACQ_TYPE_VOL_KEY (+) = A.REC_KEY ")
					_T(" AND A.ACQ_KEY = M.REC_KEY ")
					_T(" AND I.REC_KEY = M.SPECIES_KEY ")
					_T(" AND M.CLAIM_YN = 'Y' ")
					_T(" AND A.MISS_BOOK_CNT > 0 AND A.CHECKIN_STATUS <> '1' ") ;
		strWhere += strOption;
	}
	

	if( strWhere != _T("") ) {

		// ===========================================================================================================
		// 구입처 M.PROVIDER_CODE
		// ===========================================================================================================
		if( !sProviderCode.IsEmpty() ) {
			strOption.Format( _T(" AND M.DONATOR_NAME = '%s' "), sProviderCode );
			strWhere += strOption;
		}
		
		// ===========================================================================================================
		// 독촉대상 : NVL(C.CLAIM_DATE,A.ACQ_EXP_DATE) + M.CLAIM_INTERVAL < 오늘 날짜  
		// ===========================================================================================================
		INT nSearchCode1 = ((CButton*)GetDlgItem(IDC_rCLAIM1))->GetCheck();
		if( nSearchCode1 == 1 ) {
		
			// 이쿼리는 문제가 있다. 자료에 대해서 독촉처리시마다 독촉이력이 만들어지므로
			// NVL(C.CLAIM_DATE,A.ACQ_EXP_DATE) 이부분에서 CLAIM_DATE의 맥스값과 비교하는 부분이 까다로움...
			// DM을 만들고 필요없는 부분은 지우는 방법 사용
			//strOption.Format( _T(" AND NVL(C.CLAIM_DATE,A.ACQ_EXP_DATE) + M.CLAIM_INTERVAL < '%s' "), sTodayDate );
			//strWhere += strOption;

			//*m_sSearchCode	= _T("1");
			m_sSearchCode	= _T("1");
		}

		// ===========================================================================================================
		// 1. 독촉자료
		//    1.1 독촉한 자료이면서 독촉대상이 아닌자료 : C.CLAIM_DATE + M.CLAIM_INTERVAL >= 오늘 날짜
		//    1.2 독촉한 자료 C.CLAIM_SEQ > 0
		// ===========================================================================================================
		INT nSearchCode2 = ((CButton*)GetDlgItem(IDC_rCLAIM2))->GetCheck();
		if( nSearchCode2 == 1 ) {
		
			// 1.1 
			// strOption.Format( _T(" AND C.CLAIM_DATE + M.CLAIM_INTERVAL >= '%s' "), sTodayDate );
			
			// 1.2
			strOption.Format( _T(" AND C.CLAIM_SEQ > 0 ") );
			strWhere += strOption;

			//*m_sSearchCode	= _T("2");
			m_sSearchCode	= _T("2");
		}

		// 
		// 독촉차수
		if( !sClaimSeq1.IsEmpty() ) {

			if( !sClaimSeq2.IsEmpty() ) {
				strOption.Format( _T(" AND C.CLAIM_SEQ >= '%s' AND C.CLAIM_SEQ <= '%s' "), sClaimSeq1, sClaimSeq2 );
				strWhere += strOption;
			}
			else {
				strOption.Format( _T(" AND C.CLAIM_SEQ = '%s' "), sClaimSeq1 );
				strWhere += strOption;
			}
		}
		else {

			if( !sClaimSeq2.IsEmpty() ) {
				strOption.Format( _T(" AND C.CLAIM_SEQ = '%s' "), sClaimSeq2 );
				strWhere += strOption;
			}
		}

		//발행일 조건
		if( !sPubDate1.IsEmpty() ) {
		//AND A.VOL_KEY IN (SELECT REC_KEY FROM SE_VOL_TBL WHERE PUBLISH_DATE BETWEEN '2005/01/01' AND '2005/05/01')
			if( !sPubDate2.IsEmpty() ) {
				strOption.Format( _T(" AND A.VOL_KEY IN (SELECT REC_KEY FROM SE_VOL_TBL WHERE PUBLISH_DATE >= '%s' AND PUBLISH_DATE <= '%s') "), sPubDate1, sPubDate2 );
				strWhere += strOption;
			}
			else {
				strOption.Format( _T(" AND A.VOL_KEY IN (SELECT REC_KEY FROM SE_VOL_TBL WHERE PUBLISH_DATE = '%s') "), sPubDate1 );
				strWhere += strOption;
			}
		}
		else
		{
			if( !sPubDate2.IsEmpty() ) {
				strOption.Format( _T(" AND A.VOL_KEY IN (SELECT REC_KEY FROM SE_VOL_TBL WHERE PUBLISH_DATE = '%s') "), sPubDate2 );
				strWhere += strOption;
			}
		}

	}

	m_strQuery = strWhere;
}

VOID CMISSING_DATA_SEARCH::SendQueryToParent()
{
	:: SendMessage(	m_pMgr->GetSafeHwnd(), SENDQUERY , 0, 0);
}

INT CMISSING_DATA_SEARCH::DoIndexSearch(CString f_strWhere, CStringList& f_slistKey)
{
EFS_BEGIN

	INT i, ids, nRowCnt;
	CString strRecKey;

    CESL_DataMgr *pDataMgr = FindDM(_T("DM_SE_MISSING_01_KEYLIST"));
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

		//pDataMgr->GetCellData(i, 0, strRecKey);
		pDataMgr->GetCellData(_T("REC_KEY"), i, strRecKey);
		f_slistKey.AddTail( strRecKey );
	}

	return 0;

EFS_END
return -1;
}

CString CMISSING_DATA_SEARCH::MakeReckeyInQuery(CStringList &f_keyList)
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

VOID CMISSING_DATA_SEARCH::SetAcqCode( CString f_sAcqCode )
{
	m_sAcqCode = f_sAcqCode;
}

BOOL CMISSING_DATA_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			MakeQuery();
			SendQueryToParent();

			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CMISSING_DATA_SEARCH::SetFocusOnTitle()
{
	//서명에 setfocus
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_eTITLE_FROM);
	if(pWnd) pWnd->SetFocus();
}

HBRUSH CMISSING_DATA_SEARCH::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
