// BO_CAT_PROC_0330.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0330.h"
#include "BO_CAT_PROC_0332.h"

#include "..\..\..\공통\K2UP_Common\MakeSearchData.h"
#include "..\..\..\공통\MarcAdjustApi\MarcAdjustConstant.h"
#include "CommonDefine.h"

#define FT          30
#define GT          31

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0330 dialog


CBO_CAT_PROC_0330::CBO_CAT_PROC_0330(CESL_Mgr* pParent, CString sTitle /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0330)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nModeMONO_SERIAL = MODE_MONO;
	m_sTitle = sTitle;
	m_KDC = _T("");
	m_DDC = _T("");
	m_LC = _T("");
	m_MakeSearchData = new CMakeSearchData(this);
}

CBO_CAT_PROC_0330::~CBO_CAT_PROC_0330()
{
}

VOID CBO_CAT_PROC_0330::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0330)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0330, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0330)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_b0330MARC, Onb0330MARC)
	ON_BN_CLICKED(IDC_bCat0330CLEAR, OnbCat0330CLEAR)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0330 message handlers

BOOL CBO_CAT_PROC_0330::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0330::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0330")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	if (m_nModeMONO_SERIAL == MODE_SERIAL)
	{
		static_cast<CComboBox*>(GetDlgItem(IDC_cDATA))->SetCurSel(1);
	}

	GetDlgItem(IDC_eTITLE_ARTICLE)->SetWindowText(m_sTitle);
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0330::OnbSEARCH() 
{
EFS_BEGIN

	BeginWaitCursor();

	INT ids;
	CString sMAT_CODE , sMEDIA_CODE;
	CString sTitle , sKeyWord , sAuthor , sPublisher, sCssDiv , sCssStart , sCssEnd;
	CString sNumStart , sNumEnd;

	ids = GetControlData ( _T("CM_BO_CAT_PROC_0330") , _T("BS_자료구분_코드") , sMAT_CODE);	
	ids = GetControlData ( _T("CM_BO_CAT_PROC_0330") , _T("BS_매체구분_코드") , sMEDIA_CODE);	

	ids = GetControlData ( _T("CM_BO_CAT_PROC_0330") , _T("IBS_본표제") , sTitle);
	ids = GetControlData ( _T("CM_BO_CAT_PROC_0330") , _T("IBS_키워드") , sKeyWord);
	ids = GetControlData ( _T("CM_BO_CAT_PROC_0330") , _T("IBS_저작자") , sAuthor);
	ids = GetControlData ( _T("CM_BO_CAT_PROC_0330") , _T("IBS_발행자") , sPublisher);
	
	ids = GetControlData ( _T("CM_BO_CAT_PROC_0330") , _T("분류표구분") , sCssDiv);
	ids = GetControlData ( _T("CM_BO_CAT_PROC_0330") , _T("분류기호부터") , sCssStart);
	ids = GetControlData ( _T("CM_BO_CAT_PROC_0330") , _T("분류기호까지") , sCssEnd);	

	ids = GetControlData ( _T("CM_BO_CAT_PROC_0330") , _T("번호부터") , sNumStart);
	ids = GetControlData ( _T("CM_BO_CAT_PROC_0330") , _T("번호까지") , sNumEnd);

	if ( sTitle == _T("") && sKeyWord == _T("") && sAuthor == _T("") && sPublisher == _T("") && sCssStart == _T("") && sNumStart == _T("") ) {
		AfxMessageBox ( _T("주검색항목을 입력하여야 합니다.") );
		return;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0330"));

	CString strModeMonoSerial;
	GetControlData(_T("CM_BO_CAT_PROC_0330"), _T("자료"), strModeMonoSerial);

	CString strBookTblName;
	if (strModeMonoSerial == _T("단행"))
	{
		pDM->TBL_NAME = _T("IDX_BO_TBL I, BO_SPECIES_TBL S");
		strBookTblName = _T("BO_BOOK_TBL");
	}
	else if (strModeMonoSerial == _T("연속"))
	{
		pDM->TBL_NAME = _T("IDX_SE_TBL I, SE_SPECIES_TBL S");
		strBookTblName = _T("SE_BOOK_TBL");
	}

	CESL_DataMgr::reference *pRef = NULL;
	POSITION pos = pDM->RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)pDM->RefList.GetNext(pos);
		if (!pRef) continue;
		
		if (pRef->FIELD_ALIAS == _T("IBS_청구_권책기호"))
		{
			if (strModeMonoSerial == _T("단행"))
				pRef->DATA_TYPE = _T("");
			else if (strModeMonoSerial == _T("연속"))
				pRef->DATA_TYPE = _T("udf");
		}
	}

	CString strWhere;
	strWhere = _T("I.REC_KEY = S.REC_KEY");

	sMAT_CODE.Replace ( _T(" ") , NULL );
	if ( sMAT_CODE != _T("") ) {
		strWhere = strWhere + _T(" AND I.MAT_CODE = '") + sMAT_CODE + _T("'");
	}
	sMEDIA_CODE.Replace ( _T(" ") , NULL );
	if ( sMEDIA_CODE != _T("") ) {
		strWhere = strWhere + _T(" AND I.MEDIA_CODE = '") + sMEDIA_CODE + _T("'");
	}

	if ( sTitle != _T("") ){		
		INT iCh = m_MakeSearchData->ChangeHanjaToHangul(sTitle);	
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
			return;
		}
		iCh = m_MakeSearchData->GetKeyWord( sTitle );
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
			return;
		}

		
		CString sWhereTitle;			
		if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
			sWhereTitle.Format( _T("CATSEARCH(I.IDX_TITLE,'%s*',NULL)>0") , sTitle );
		}
		else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
			if (strModeMonoSerial == _T("연속"))
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_TITLE, '%s*')"), sTitle );
			else 
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_TITLE, '%s*')"), sTitle );
		}
		else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
			AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
			return;
		}

		if( pDM->m_bOpenbase == TRUE )
		{
			strWhere = strWhere + _T(" INTERSECT ") + sWhereTitle;
		}
		else
		{
			strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}
	}

	if ( sAuthor != _T("") ){		
		INT iCh = m_MakeSearchData->ChangeHanjaToHangul( sAuthor );
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
			return;
		}
		iCh = m_MakeSearchData->GetKeyWord( sAuthor );
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
			return;
		}
		
		CString sWhereTitle;			
		if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
			sWhereTitle.Format( _T("CATSEARCH(I.IDX_AUTHOR,'%s*',NULL)>0") , sAuthor );
		}
		else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
			if (strModeMonoSerial == _T("연속"))
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_AUTHOR, '%s*')"), sAuthor );
			else 
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_AUTHOR, '%s*')"), sAuthor );
		}
		else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
			AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
			return;
		}
		if( pDM->m_bOpenbase == TRUE )
		{
			strWhere = strWhere + _T(" INTERSECT ") + sWhereTitle;
		}
		else
		{
			strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}
	}

	if ( sPublisher != _T("") ){		
		INT iCh = m_MakeSearchData->ChangeHanjaToHangul( sPublisher );
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
			return;
		}
		iCh = m_MakeSearchData->GetKeyWord( sPublisher );
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
			return;
		}
		
		CString sWhereTitle;			
		if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
			sWhereTitle.Format( _T("CATSEARCH(I.IDX_PUBLISHER,'%s*',NULL)>0") , sPublisher );
		}
		else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
			if (strModeMonoSerial == _T("연속"))
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_PUBLISHER, '%s*')"), sPublisher );
			else 
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_PUBLISHER, '%s*')"), sPublisher );
		}
		else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
			AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
			return;
		}

		if( pDM->m_bOpenbase == TRUE )
		{
			strWhere = strWhere + _T(" INTERSECT ") + sWhereTitle;
		}
		else
		{
			strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}
	}

	if ( sKeyWord != _T("") ){	
		INT iCh = m_MakeSearchData->ChangeHanjaToHangul( sKeyWord );
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
			return;
		}
		iCh = m_MakeSearchData->GetKeyWord( sKeyWord );
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sTitle ) error") );
			return;
		}

		
		CString sWhereTitle;			
		if ( m_pInfo->GetSearchIndexType() == ORA_CATSEARCH ) {
			sWhereTitle.Format( _T("CATSEARCH(I.IDX_KEYWORD,'%s*',NULL)>0") , sKeyWord );
		}
		else if ( m_pInfo->GetSearchIndexType() == ECO_SEARCH ) {
			if (strModeMonoSerial == _T("연속"))
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, SE_IDX_KEYWORD, '%s*')"), sKeyWord );
			else 
				sWhereTitle.Format( _T("&ECOSEARCH(I.REC_KEY, BO_IDX_KEYWORD, '%s*')"), sKeyWord );
		}
		else if ( m_pInfo->GetSearchIndexType() == MNG_INVALID ) {
			AfxMessageBox( _T("SearchIndexType이 설정되지 않았습니다.") );
			return;
		}

		if( pDM->m_bOpenbase == TRUE )
		{
			strWhere = strWhere + _T(" INTERSECT ") + sWhereTitle;
		}
		else
		{
			strWhere = strWhere + _T(" AND ") + sWhereTitle;
		}
	}
	
	if ( sCssDiv == _T("1") ) {
		if ( sCssStart == _T("") && sCssEnd != _T("") ) {
			AfxMessageBox( _T("분류기호 시작을 입력하세요.") );
			return;
		}

		if ( sCssStart != _T("") && sCssEnd == _T("") ) {            
			strWhere = strWhere + _T(" AND I.KDC_CLASS LIKE '") + sCssStart  + _T("'");		
		}

		else if ( sCssStart != _T("") && sCssEnd != _T("") ) {
			strWhere = strWhere + _T(" AND I.KDC_CLASS >= '") + sCssStart  + _T("' AND I.KDC_CLASS <= '") + sCssEnd + _T("'");
		}			
	}
	else if ( sCssDiv == _T("2") ) {
		if ( sCssStart == _T("") && sCssEnd != _T("") ) {
			AfxMessageBox( _T("분류기호 시작을 입력하세요.") );
			return;
		}
		if ( sCssStart != _T("") && sCssEnd == _T("") ) {			
			strWhere = strWhere + _T(" AND I.DDC_CLASS = '") + sCssStart  + _T("'");
		}
		else if ( sCssStart != _T("") && sCssEnd != _T("") ) {
			strWhere = strWhere + _T(" AND I.DDC_CLASS >= '") + sCssStart  + _T("' AND I.DDC_CLASS <= '") + sCssEnd + _T("'");
		}	
	}
	else if ( sCssDiv == _T("7") ) {
		if ( sCssStart == _T("") && sCssEnd != _T("") ) {
			AfxMessageBox( _T("분류기호 시작을 입력하세요.") );
			return;
		}
		if ( sCssStart != _T("") && sCssEnd == _T("") ) {			
			strWhere = strWhere + _T(" AND I.LC_CLASS = '") + sCssStart  + _T("'");		
		}
		else if ( sCssStart != _T("") && sCssEnd != _T("") ) {
			strWhere = strWhere + _T(" AND I.LC_CLASS >= '") + sCssStart  + _T("' AND I.LC_CLASS <= '") + sCssEnd + _T("'");
		}	
	}

	CString sPUB_YEAR_START, sPUB_YEAR_END;
	GetControlData(_T("CM_BO_CAT_PROC_0330"), _T("IBS_발행년도2_시작"), sPUB_YEAR_START);
	GetControlData(_T("CM_BO_CAT_PROC_0330"), _T("IBS_발행년도2_까지"), sPUB_YEAR_END);
	if ( sPUB_YEAR_START == _T("") && sPUB_YEAR_END != _T("") ) {
		AfxMessageBox ( _T("시작 발행년도를 입력하세요."));
		return;
	}
	else if ( sPUB_YEAR_START != _T("") && sPUB_YEAR_END == _T("") ) {
		strWhere = _T("I.PUB_YEAR = '") + sPUB_YEAR_START  + _T("' AND ") + strWhere;
	}
	else if ( sPUB_YEAR_START != _T("") && sPUB_YEAR_END != _T("") ) {
		strWhere = _T("I.PUB_YEAR >= '") + sPUB_YEAR_START + _T("' AND I.PUB_YEAR <= '") + sPUB_YEAR_END + _T("' AND ") + strWhere;
	}

	CString sBookWhere;
	CString sNumSelect;
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	GetControlData( _T("CM_BO_CAT_PROC_0330"), _T("제어번호선택"), sNumSelect );	
	if ( sNumSelect == _T("등록번호") ) {
		
		if ( sNumStart != _T("") && sNumStart.GetLength() < 12 ) {
			sNumStart = m_pInfo->MakeRegNo(sNumStart.Left(nRegCodeLength),sNumStart.Mid(nRegCodeLength));
		}
		if ( sNumEnd != _T("") && sNumEnd.GetLength() < 12 ) {	
			sNumEnd = m_pInfo->MakeRegNo(sNumEnd.Left(nRegCodeLength),sNumEnd.Mid(nRegCodeLength));
		}

		if ( sNumStart != _T("") && sNumEnd == _T("")) {			
			if ( sBookWhere != _T("") ) {
				sBookWhere = sBookWhere + _T("AND REG_NO = '") + sNumStart + _T("'");
			}
			else if ( sBookWhere = _T("") ) {
				sBookWhere = _T("REG_NO = '") + sNumStart + _T("'");
			}
		}
		
		if ( sNumEnd != _T("") && sNumStart == _T("") ) {
			AfxMessageBox( _T("시작 등록번호를 입력 하세요."));
			return;
		}
		if ( sNumStart != _T("") && sNumEnd != _T("") ) {	
			if ( sBookWhere = _T("") ) {
				sBookWhere = _T("REG_NO >= '") + sNumStart + _T("' AND REG_NO <= '") + sNumEnd + _T("'");
			}
			else {
				sBookWhere = sBookWhere + _T("AND REG_NO >= '") + sNumStart + _T("' AND REG_NO <= '") + sNumEnd + _T("'");
			}
		}
	}
	else if ( sNumSelect == _T("제어번호") ) {
		if ( sNumStart == _T("") && sNumEnd != _T("") ) {
			AfxMessageBox( _T("시작 제어번호를 입력하세요.") );
			return;
		}
		else if ( sNumStart != _T("") && sNumEnd == _T("") ) {
			sBookWhere = _T(" I.CONTROL_NO = '") + sNumStart + _T("'");
		}
		else if ( sNumStart != _T("") && sNumEnd != _T("") ) {
			sBookWhere = _T(" I.CONTROL_NO >= '") + sNumStart + _T("' AND I.CONTROL_NO <= '") + sNumEnd + _T("'");
		}

	}

	if ( sBookWhere != _T("") ) {
		strWhere = strWhere + _T(" AND I.REC_KEY IN (SELECT SPECIES_KEY FROM ") + strBookTblName + _T(" WHERE ") + sBookWhere + _T(")");
	}

	
	if (pDM->RefreshDataManager(strWhere) < 0)	return;

	EndWaitCursor();

	INT iCount = pDM->GetRowCount();
	if ( iCount < 1 ) {
		AfxMessageBox ( _T("조건에 맞는 자료가 없습니다.") );
	}

	SetFocus();

	CString strStreamMarc;
	CMarc marc;	

	for ( INT i = 0 ; i < iCount ; i++ ) {
		CString sShelfCode = pDM->GetCellData( _T("IBS_청구_별치기호_코드"), i );
		CString sClassNo = pDM->GetCellData( _T("IBS_청구_분류기호"), i );
		CString sBookCode = pDM->GetCellData( _T("IBS_청구_도서기호"), i );
		strStreamMarc = pDM->GetCellData(_T("MARC"), i );
		m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);

		CString sVolCode;
		m_pInfo->pMarcMgr->GetItem(&marc, _T("090$c"), sVolCode );

		CString sCallNo;

		if ( sShelfCode != _T("") ) {
			
			CString sShelfCodeCon;
			INT ids = m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"),sShelfCode, sShelfCodeCon);
			if ( sShelfCodeCon == _T("") ) {
				sShelfCode.MakeLower();
				ids = m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"),sShelfCode, sShelfCodeCon);
			}
			sShelfCode = sShelfCodeCon;

			sCallNo = sShelfCode;
		}

		if ( sClassNo != _T("") ) {
			sCallNo = sCallNo + sClassNo;
		}

		if ( sBookCode != _T("") ) {
			sCallNo = sCallNo + _T("-") + sBookCode;
		}
		
		if ( sVolCode != _T("") ) {
			sCallNo = sCallNo + _T("-") + sVolCode;
		}

		pDM->SetCellData( _T("청구기호"), sCallNo, i );
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0330"));
	pCM->AllControlDisplay();


EFS_END
}

BEGIN_EVENTSINK_MAP(CBO_CAT_PROC_0330, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_PROC_0330)
	ON_EVENT(CBO_CAT_PROC_0330, IDC_MSHFLEXGRID, -600 /* Click */, OnClickMshflexgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_CAT_PROC_0330::OnClickMshflexgrid() 
{
EFS_BEGIN

	//그리드에 넣기
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0330"));
	pCM->FindControl(_T("분류지원시스템_그리드"));
	
	CESL_Grid * pGrid = NULL;
	pGrid = (CESL_Grid*)pCM->FindControl(_T("분류지원시스템_그리드"));

	if ( pGrid->GetCol() != 1 ) {
		return ;
	}
	if ( pGrid->GetTextMatrix( pGrid->GetRow(), 3 ) == _T("") && 
		 pGrid->GetTextMatrix( pGrid->GetRow(), 4 ) == _T("") &&
		 pGrid->GetTextMatrix( pGrid->GetRow(), 5 ) == _T("") ) 
	{
		return;
	}
	INT iRowCount = pGrid->GetRows();

	CString tmp;
	tmp.Format(_T("%c  %c"), FT, GT);
	
	INT nCursorRow = pGrid->GetRow();
	CString strData;
	strData = pGrid->GetTextMatrix( nCursorRow, 1 );
	if (strData.GetLength() == 0) {
		for( INT i = 0 ; i < m_KDCrow.GetSize() ; i++ ) {
			if ( m_KDCrow.GetAt(i) == nCursorRow ) {
				m_KDCrow.RemoveAt( i );
			}
		}
		for( i = 0 ; i < m_DDCrow.GetSize() ; i++ ) {
			if ( m_DDCrow.GetAt(i) == nCursorRow ) {
				m_DDCrow.RemoveAt( i );
			}
		}
		for( i = 0 ; i < m_LCrow.GetSize() ; i++ ) {
			if ( m_LCrow.GetAt(i) == nCursorRow ) {
				m_LCrow.RemoveAt( i );
			}
		}
	} 
	else {
		if ( pGrid->GetTextMatrix( pGrid->GetRow(), 3 ) != _T("") )
		{
			m_KDCrow.Add(nCursorRow);
		}
		if ( pGrid->GetTextMatrix( pGrid->GetRow(), 4 ) != _T("") )
		{
			m_DDCrow.Add(nCursorRow);
		}
		if ( pGrid->GetTextMatrix( pGrid->GetRow(), 5 ) != _T("") )
		{
			m_LCrow.Add(nCursorRow);
		}
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_PROC_0330"));

	CMarc marc;
	CString strStreamMarc;
	CString strClass;
	CString strItemData;
	CString strKDCClass;
	CString strDDCClass;
	CString strLCClass;	
	CString sSubStr;
	INT		rowCnt;
	INT		i;
	INT		ntmp;

	strStreamMarc = pDM->GetCellData(_T("MARC"), pGrid->GetRow() - 1 );
	INT ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
	if (ids >= 0)
	{
		sSubStr.Empty();
		rowCnt = m_KDCrow.GetSize();		
		for( i = 0; i < rowCnt; i++ ){
			ntmp = m_KDCrow.GetAt( i );
			strData = pGrid->GetTextMatrix( ntmp, 3 );
			if ( !strData.IsEmpty() )
			{
				CString strData_all;
				strData_all.Format( _T("%ca%s") , SUBFIELD_CODE, strData );
				sSubStr = sSubStr + strData_all;			
			}
		}	
		if ( !sSubStr.IsEmpty() )
		{
			strKDCClass.Format(_T("056  %s%c"), sSubStr, FIELD_TERMINATOR);
			m_KDC = sSubStr.Mid(2);
			strClass = strKDCClass;
		}

		sSubStr.Empty();
		rowCnt = m_DDCrow.GetSize();		
		for( i = 0; i < rowCnt; i++ ){
			ntmp = m_DDCrow.GetAt( i );
			strData = pGrid->GetTextMatrix( ntmp, 4 );
			if ( !strData.IsEmpty() )
			{
				CString strData_all;
				strData_all.Format( _T("%ca%s") , SUBFIELD_CODE, strData );
				sSubStr = sSubStr + strData_all;			
			}
		}	
		if ( !sSubStr.IsEmpty() )
		{
			strLCClass.Format(_T("082  %s%c"), sSubStr, FIELD_TERMINATOR);
			m_DDC = sSubStr.Mid(2);
			if ( !strClass.IsEmpty() ) strClass += _T("\r\n");
			strClass += strDDCClass;	
		}
		
		sSubStr.Empty();
		rowCnt = m_LCrow.GetSize();		
		for( i = 0; i < rowCnt; i++ ){
			ntmp = m_LCrow.GetAt( i );
			strData = pGrid->GetTextMatrix( ntmp, 5 );
			if ( !strData.IsEmpty() )
			{
				CString strData_all;
				strData_all.Format( _T("%ca%s") , SUBFIELD_CODE, strData );
				sSubStr = sSubStr + strData_all;			
			}
		}	
		if ( !sSubStr.IsEmpty() )
		{
			strLCClass.Format(_T("0855 %s%c"), sSubStr, FIELD_TERMINATOR);
			m_LC = sSubStr.Mid(2);
			if ( !strClass.IsEmpty() ) strClass += _T("\r\n");
			strClass += strLCClass;	
		}
		
		SetControlData(_T("CM_BO_CAT_PROC_0330"), _T("결과"), strClass);
	}
	
EFS_END
}


VOID CBO_CAT_PROC_0330::OnOK() 
{
EFS_BEGIN

	CDialog::OnOK();

EFS_END
}

VOID CBO_CAT_PROC_0330::Onb0330MARC() 
{
EFS_BEGIN

	CBO_CAT_PROC_0332 dlg(this);
	dlg.DoModal();

EFS_END
}

VOID CBO_CAT_PROC_0330::OnbCat0330CLEAR() 
{
EFS_BEGIN

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0330"));
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_BO_CAT_PROC_0330 error") );
		return;
	}
	pCM->AllControlClear();

EFS_END
}

BOOL CBO_CAT_PROC_0330::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnbSEARCH();
		return TRUE;
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CBO_CAT_PROC_0330::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
