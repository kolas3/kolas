// BO_CAT_PROC_0202.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_PROC_0202.h"
#include "..\..\..\ESL\SIReportMgr\SIReportManager.h"
#include "Kolas2up_CardList.h"
#include "BO_CAT_PROC_0202_PRINT.h"

#include "..\..\..\공통\MarcAdjustApi\CatApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0202 dialog


CBO_CAT_PROC_0202::CBO_CAT_PROC_0202(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0202)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nModeMONO_SERIAL = MODE_MONO;

	m_iLine = 0;
}

VOID CBO_CAT_PROC_0202::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0202)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0202, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0202)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bCat0202SEARCH, OnbCat0202SEARCH)
	ON_BN_CLICKED(IDC_bCat0202PRINT, OnbCat0202PRINT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0202 message handlers

BOOL CBO_CAT_PROC_0202::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0202::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0202")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_strStreamMarc = _T("");

	CButton * pGeneralMarc;
	pGeneralMarc = (CButton *)GetDlgItem(IDC_rCARD);
	pGeneralMarc->SetCheck(1);

	if ( m_list != NULL ) m_list = NULL;

	m_list = new CKolas2up_CardList();

	EnableThemeDialogTexture(GetSafeHwnd()); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0202::OnbCat0202SEARCH() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	CString sWhere = MakeSearchWhereQuery();
	if ( sWhere == _T("") ) return;
	
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0202"));
	if ( pDM == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0202 error") );
		return;
	}

	if (m_nModeMONO_SERIAL == MODE_SERIAL)
		pDM->TBL_NAME = _T("IDX_SE_TBL I, SE_SPECIES_TBL S");

	CString strQuery;
	if (m_nModeMONO_SERIAL == MODE_SERIAL)
		strQuery = _T("select s2.marc from se_species_tbl s2, ( SELECT min(b.reg_no) reg, species_key species FROM SE_SPECIES_TBL S, SE_BOOK_TBL B WHERE B.MANAGE_CODE = UDF_MANAGE_CODE AND ") + sWhere ;
	else 
		strQuery = _T("select s2.marc from bo_species_tbl s2, ( SELECT min(b.reg_no) reg, species_key species FROM BO_SPECIES_TBL S, BO_BOOK_TBL B WHERE B.MANAGE_CODE = UDF_MANAGE_CODE AND ") + sWhere ;

	pDM->RestructDataManager(strQuery);

	if ( pDM->GetRowCount() < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
		return;
	}

	m_list->FreeData();
	m_sCard = _T("");
	m_iLine = 0;
	INT iRowCount = pDM->GetRowCount();

	CButton * pCARD;
	pCARD = (CButton *)GetDlgItem(IDC_rCARD);
	if ( pCARD->GetCheck() == 1 ) {
		for( INT i = 0 ; i < iRowCount ; i++ ) {

			m_iPageNumber = 1;

			CMarc pMarc;
			CString sStreamMarc = pDM->GetCellData( _T("BS_MARC"), i );
			m_pInfo->pMarcMgr->Decoding(sStreamMarc, &pMarc);
			
			m_strStreamMarc = sStreamMarc;

			CString sItemData;
			
			CString sAllCardList = _T("");

			sAllCardList = sAllCardList + CardList_2XXTag(&pMarc);
			sAllCardList = sAllCardList + CardList_3XXTag(&pMarc);
			sAllCardList = sAllCardList + CardList_4XXTag(&pMarc);
			sAllCardList = sAllCardList + CardList_Title_5XXTag(&pMarc);
			sAllCardList = sAllCardList + CardList_Title_7XXTag(&pMarc);
			sAllCardList = sAllCardList + CardList_Title_OtherTag(&pMarc);
			sAllCardList = sAllCardList + CardList_Author_5XXTag(&pMarc);
			sAllCardList = sAllCardList + CardList_Author_7XXTag(&pMarc);
			sAllCardList = sAllCardList+ CardList_Other_AllTag(&pMarc);

			ExecuteCutString(sAllCardList, &pMarc);
			
			CArray<CString, CString&> p049lArray;
			CArray<CString, CString&> p049vArray;
			CArray<CString, CString&> p049cArray;
			CArray<CString, CString&> p049Array;
			CString s049Filed;
			m_pInfo->pMarcMgr->GetField( &pMarc, _T("049"), s049Filed, &p049Array );

			m_pInfo->pMarcMgr->GetItem( &pMarc, _T("049$l"), sItemData, &p049lArray );

			INT i049lCount = p049lArray.GetSize();

			m_sCard = m_sCard + _T("\r\n");
			m_iLine++;

				CString s049l;
				s049Filed.Replace(SUBFIELD_CODE, '$');
				INT iPos = s049Filed.Find(_T("$l"));
				while ( iPos != -1)
				{
					s049Filed = s049Filed.Mid(iPos+2);
					s049Filed.Replace(_T("$v"), _T("-v."));
					s049Filed.Replace(_T("$c"), _T(" .c"));
					s049Filed.Replace( FIELD_TERMINATOR , ' ');
				
					iPos = s049Filed.Find(_T("$l"));
					if ( iPos > 0 )
						s049l = s049Filed.Left(iPos);
					else
						s049l = s049Filed;

					INT iPosf = s049l.Find(_T("$f"));
					if ( iPosf > 0  ) s049l = s049l.Left(iPosf);


					if ( m_iLine == 0 && m_iPageNumber > 1 )
					{
						CString sCardPrint;
						sCardPrint.Format(_T("( 카드 %d )"), m_iPageNumber );
						m_sCard = m_sCard + _T("\r\n        ") + s049l + _T("                       ") + sCardPrint ;
					}
					else
						m_sCard = m_sCard + _T("\r\n        ") + s049l;

					m_iLine++;
				

					if ( m_iLine == 15  && iPos != -1 ) 
					{	
						m_sCard = m_sCard + _T("        (다음 카드에 계속)\r\n\r\n\r\n");
						m_iLine = 0;
						m_iPageNumber++;
					}
				}
				m_iLine++;

			INT i049Count = p049Array.GetSize();
			for ( INT j = 0 ; j < i049Count ; j++ ) {
				CString s049 = p049Array.GetAt(j);
				p049Array.Add( s049 );
			}

			for ( j = 0 ; j < i049Count ; j++ ) {
				CString s020 = p049Array.GetAt(j);
			}

			if ( m_iLine < 16 ) 
			{
				for ( INT i = m_iLine; i <= 19; i++ )
				{
					m_sCard = m_sCard + _T("\r\n");
					m_iLine++;
				}
			}
			m_iLine = 0;
		}
		m_sCard = m_sCard + _T("\r\n");

		INT iStartX = m_list->nStartX;
		INT iStartY = m_list->nStartY;

		m_list->nStartX = 10;
		m_list->nStartX = 30;
		
		GetDlgItem(IDC_eCat0202Card)->SetWindowText(m_sCard);
	}
	else {

		m_sCard = _T("");
		for( INT i = 0 ; i < iRowCount ; i++ ) {
			
			CString sStreamMarc = pDM->GetCellData( _T("BS_MARC"), i );
			CMarc pMarc;
			m_pInfo->pMarcMgr->Decoding(sStreamMarc, &pMarc);

			MakeBuchulList( &pMarc );
		}
		GetDlgItem(IDC_eCat0202Card)->SetWindowText(m_sCard);
	}
		
	GetDlgItem(IDC_bCat0202PRINT)->EnableWindow(true);
	EndWaitCursor();
	
EFS_END
}

VOID CBO_CAT_PROC_0202::OnbCat0202PRINT() 
{
EFS_BEGIN

	CBO_CAT_PROC_0202_PRINT *pDlg = new CBO_CAT_PROC_0202_PRINT(this);

	CButton * pCARD;
	pCARD = (CButton *)GetDlgItem(IDC_rCARD);
	if ( pCARD->GetCheck() == 1 ) 
	{
		pDlg->SetParentInfo(_T("카드목록"));
		pDlg->SetCardListInfo(m_list, m_sCard, m_strStreamMarc);
		pDlg->DoModal();
	}
	else 
	{
		pDlg->SetParentInfo(_T("부출지시서"));
		pDlg->SetCardListInfo(m_list, m_sCard, m_strStreamMarc);
		pDlg->DoModal();
	}

	delete pDlg;

EFS_END
}

INT CBO_CAT_PROC_0202::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;

	pSIReportManager->SetTableCellText( _T("테이블_6"), 0, 0, m_sCard, false, false);
	return 0;

EFS_END
return -1;
}

BOOL CBO_CAT_PROC_0202::IsCanCut(CString sStr, INT nPos)
{
EFS_BEGIN

	INT i, nLastPos = -1;
	
	if(nPos > sStr.GetLength())
		return FALSE;

	if ( sStr.GetLength() <= nPos)
		return TRUE;

	TCHAR ch = sStr.GetAt(nPos);
	
	if(nPos == 0)
		if((ch & (0x80)) != 128)
			return TRUE;
		else
			return FALSE;

	for(i = 0; i < nPos; i++) {
		ch = sStr.GetAt(i);
		if((ch & (0x80)) != 128)
			nLastPos = i;
	}
	if (nLastPos == -1 && nPos % 2 == 1) return FALSE;
	
	return !((nPos - nLastPos) % 2);

EFS_END
return FALSE;

}

CString CBO_CAT_PROC_0202::CardList_2XXTag(CMarc *pMarc)
{
EFS_BEGIN
	CString s2XXTagCardList = _T("");
	if ( pMarc == NULL ) return _T("");
	
	CString sItemData;
	CString sMarcField;

	m_pInfo->pMarcMgr->GetField( pMarc, _T("240"), sMarcField );
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("240$a"), sItemData );

	
	if ( sItemData != _T("")  && sMarcField != _T("")) {
		if ( sMarcField.Mid(1,1) == '0' ) 
		{
			s2XXTagCardList = s2XXTagCardList + _T("[") + sItemData + _T("]");
		}
		else if ( sMarcField.Mid(1,1) == '1' )
		{
			s2XXTagCardList.Replace(_T("("), _T(""));
			s2XXTagCardList.Replace(_T(")"), _T(""));
			s2XXTagCardList = s2XXTagCardList + _T("[") + sItemData + _T("]");
		}

	}

	s2XXTagCardList = s2XXTagCardList + _T("\r\n");

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetField( pMarc, _T("245"),  sItemData );
	sItemData.Replace(SUBFIELD_CODE, '$');

	if ( !sItemData.IsEmpty() )
	{
		sItemData = sItemData.Mid(2);
		sItemData.Replace(_T("$a"), _T(" "));
		sItemData.Replace(_T("$d"), _T(" "));
		sItemData.Replace(_T("$e"), _T(" "));
		sItemData.Replace(_T("$b"), _T(" "));
		sItemData.Replace(_T("$n"), _T(" "));
		sItemData.Replace(_T("$p"), _T(" "));
		sItemData.Replace(_T("$x"), _T(" "));
		sItemData.Replace(_T("$c"), _T(" "));
		sItemData.Replace(_T("$h"), _T(" "));
		sItemData.Replace(_T("$6"), _T(" "));
	}

	sItemData.Replace( FIELD_TERMINATOR , ' ');

	s2XXTagCardList = s2XXTagCardList + sItemData;

	if ( s2XXTagCardList != _T("") )
		s2XXTagCardList = s2XXTagCardList + _T(" .--");

	BOOL b250Tag = FALSE;
	
	sItemData = _T("");
	m_pInfo->pMarcMgr->GetField( pMarc, _T("250"),  sItemData );
	sItemData.Replace(SUBFIELD_CODE, '$');

	if ( !sItemData.IsEmpty()) 
	{
		sItemData = sItemData.Mid(2);
		sItemData.Replace(_T("$a"), _T(" "));
		sItemData.Replace(_T("$b"), _T(" "));
		b250Tag = TRUE;
	}

	sItemData.Replace( FIELD_TERMINATOR , ' ');

	s2XXTagCardList = s2XXTagCardList + _T(" (") + sItemData + _T(") ");

	if ( b250Tag == TRUE )
		s2XXTagCardList = s2XXTagCardList + _T(" .--");
	
	BOOL b255Tag = FALSE;

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("255$a"), sItemData );
	if ( sItemData != _T("") ) {
		s2XXTagCardList = s2XXTagCardList + _T(" ") + sItemData;
		b255Tag = TRUE;
	}

	if ( b255Tag == TRUE )
		s2XXTagCardList = s2XXTagCardList + _T(" .--");
	
	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("260$a"), sItemData );
	if ( sItemData != _T("") ) {
		s2XXTagCardList = s2XXTagCardList + sItemData + _T(" : ");
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("260$b"), sItemData );
	if ( sItemData != _T("") ) {
		s2XXTagCardList = s2XXTagCardList + sItemData + _T(" , ");
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("260$c"), sItemData );
	if ( sItemData != _T("") ) {
		s2XXTagCardList = s2XXTagCardList + sItemData;
	}
	
	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("260$e"), sItemData );
	if ( sItemData != _T("") ) {
		s2XXTagCardList = s2XXTagCardList + sItemData + _T(" : ");
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("260$f"), sItemData );
	if ( sItemData != _T("") ) {
		s2XXTagCardList = s2XXTagCardList + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("260$g"), sItemData );
	if ( sItemData != _T("") ) {
		s2XXTagCardList = s2XXTagCardList + sItemData;
	}

	if ( s2XXTagCardList != _T("") )
		s2XXTagCardList = s2XXTagCardList + _T("\r\n");

	return s2XXTagCardList;
	
EFS_END
return _T("");
}

CString CBO_CAT_PROC_0202::CardList_3XXTag(CMarc *pMarc)
{
EFS_BEGIN

	CString s3XXTagCardList = _T("");
	if ( pMarc == NULL ) return _T("");

	CString sItemData;
	CString sMarcField;

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("300$a"), sItemData );
	if ( sItemData != _T("") ) {
		s3XXTagCardList = s3XXTagCardList + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("300$b"), sItemData );
	if ( sItemData != _T("") ) {
		s3XXTagCardList = s3XXTagCardList + _T(" .: ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("300$c"), sItemData );
	if ( sItemData != _T("") ) {
		s3XXTagCardList = s3XXTagCardList + _T(" ; ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("300$e"), sItemData );
	if ( sItemData != _T("") ) {
		s3XXTagCardList = s3XXTagCardList + _T(" + ") + sItemData;
	}

	CString sMarc440Field, sMarc490Field;
	
	m_pInfo->pMarcMgr->GetField( pMarc, _T("440"), sMarc440Field );
	m_pInfo->pMarcMgr->GetField( pMarc, _T("490"), sMarc490Field );

	if ( sMarc440Field != _T("") || sMarc490Field != _T(""))	
		s3XXTagCardList = s3XXTagCardList + _T(" .--");


	sItemData = _T("");
	m_pInfo->pMarcMgr->GetField( pMarc, _T("490"),  sItemData );
	sItemData.Replace(SUBFIELD_CODE, '$');

	if ( !sItemData.IsEmpty()) 
	{
		sItemData = sItemData.Mid(2);
		sItemData.Replace(_T("$a"), _T(" "));
		sItemData.Replace(_T("$n"), _T(" "));
		sItemData.Replace(_T("$p"), _T(" "));
		sItemData.Replace(_T("$s"), _T(" "));
		sItemData.Replace(_T("$x"), _T(" "));
		sItemData.Replace(_T("$v"), _T(" "));
	}

	sItemData.Replace( FIELD_TERMINATOR , ' ');

	s3XXTagCardList = s3XXTagCardList + _T(" (") + sItemData + _T(") ");


	return s3XXTagCardList;


EFS_END
return _T("");
}

CString CBO_CAT_PROC_0202::CardList_4XXTag(CMarc *pMarc)
{
EFS_BEGIN
		
	CString s4XXTagCardList;
	if ( pMarc == NULL ) return _T("");
	
	CString sItemData;
	CString sMarcField;
	
	CArray <CString, CString&> pArray;
	pArray.RemoveAll();
	sItemData = _T("");
	m_pInfo->pMarcMgr->GetField( pMarc, _T("440"),  sItemData , &pArray);
	
	for (INT i = 0; i < pArray.GetSize(); i++ )
	{
		sItemData = pArray.GetAt(i);
		sItemData.Replace(SUBFIELD_CODE, '$');

		if ( !sItemData.IsEmpty() )
		{
			sItemData = sItemData.Mid(2);
			sItemData.Replace(_T("$a"), _T(" "));
			sItemData.Replace(_T("$n"), _T(" "));
			sItemData.Replace(_T("$p"), _T(" "));
			sItemData.Replace(_T("$s"), _T(" "));
			sItemData.Replace(_T("$v"), _T(" "));
			sItemData.Replace(_T("$x"), _T(" "));
		}
		sItemData.Replace( FIELD_TERMINATOR , ' ');

		s4XXTagCardList = s4XXTagCardList + _T("(") + sItemData + _T(")") ;
	}


	return s4XXTagCardList;
	
	
EFS_END
return _T("");
}

CString CBO_CAT_PROC_0202::CardList_Title_5XXTag(CMarc *pMarc)
{
EFS_BEGIN

	CString sTitlet5XXTagCardList = _T("\r\n|\r\n");
	if ( pMarc == NULL ) return _T("");

	CString sItemData;
	CString sMarcField;

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$a"), sItemData );
	if ( sItemData != _T("") ) {
		sTitlet5XXTagCardList =  sTitlet5XXTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$b"), sItemData );
	if ( sItemData != _T("") ) {
		sTitlet5XXTagCardList =  sTitlet5XXTagCardList + _T(" 관제 : ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$e"), sItemData );
	if ( sItemData != _T("") ) {
		sTitlet5XXTagCardList = sTitlet5XXTagCardList + _T(" 부서명 : ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$f"), sItemData );
	if ( sItemData != _T("") ) {
		sTitlet5XXTagCardList = sTitlet5XXTagCardList + _T("  판권기서명 : ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$g"), sItemData );
	if ( sItemData != _T("") ) {
		sTitlet5XXTagCardList = sTitlet5XXTagCardList + _T(" 표지서명 : ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$h"), sItemData );
	if ( sItemData != _T("") ) {
		sTitlet5XXTagCardList = sTitlet5XXTagCardList + _T(" 책등서명 : ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$i"), sItemData );
	if ( sItemData != _T("") ) {
		sTitlet5XXTagCardList = sTitlet5XXTagCardList + _T(" 번역서명 : ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$j"), sItemData );
	if ( sItemData != _T("") ) {
		sTitlet5XXTagCardList = sTitlet5XXTagCardList + _T(" 대등서명 : ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$k"), sItemData );
	if ( sItemData != _T("") ) {
		sTitlet5XXTagCardList = sTitlet5XXTagCardList + _T(" 잡정보 : ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$m"), sItemData );
	if ( sItemData != _T("") ) {
		sTitlet5XXTagCardList = sTitlet5XXTagCardList + _T(" 부논문 : ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$r"), sItemData );
	if ( sItemData != _T("") ) {
		sTitlet5XXTagCardList = sTitlet5XXTagCardList + _T(" 총서면관제 : ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$c"), sItemData );
	if ( sItemData != _T("") ) {
		sTitlet5XXTagCardList = sTitlet5XXTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$t"), sItemData );
	if ( sItemData != _T("") ) {
		sTitlet5XXTagCardList = sTitlet5XXTagCardList + _T(" ") + sItemData;
	}

	if ( sTitlet5XXTagCardList != _T("") ) 
	{
		sTitlet5XXTagCardList.Replace(_T(")"), _T(""));
		sTitlet5XXTagCardList.Replace(_T("("), _T(""));
	}


	return sTitlet5XXTagCardList;

EFS_END
return _T("");
}

CString CBO_CAT_PROC_0202::CardList_Title_7XXTag(CMarc *pMarc)
{
EFS_BEGIN

	CString sTitlet7XXTagCardList;
	if ( pMarc == NULL ) return _T("");

	CString sItemData;
	CString sMarcField;
	
	m_pInfo->pMarcMgr->GetField( pMarc, _T("780"), sMarcField );
	
	if ( sMarcField != _T("")) {
		if ( sMarcField.Mid(0,1) == '0' ) 
		{
			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("780$a"), sItemData );
			if ( sItemData != _T("") ) {
				sTitlet7XXTagCardList = sTitlet7XXTagCardList + _T(" ") + sItemData;
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("780$g"), sItemData );
			if ( sItemData != _T("") ) {
				sTitlet7XXTagCardList = sTitlet7XXTagCardList + _T(" ") + sItemData;
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("780$t"), sItemData );
			if ( sItemData != _T("") ) {
				sTitlet7XXTagCardList = sTitlet7XXTagCardList + _T(" ") + sItemData;
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("780$x"), sItemData );
			if ( sItemData != _T("") ) {
				sTitlet7XXTagCardList =  _T("ISSN ") + sTitlet7XXTagCardList + _T(" ") + sItemData;
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("780$z"), sItemData );
			if ( sItemData != _T("") ) {
				sTitlet7XXTagCardList =  _T("ISBN ") + sTitlet7XXTagCardList + _T(" ") + sItemData;
			}
		}
	}
	

	m_pInfo->pMarcMgr->GetField( pMarc, _T("785"), sMarcField );
	
	if ( sMarcField != _T("") )
		{
		if ( sMarcField.Mid(0,1) == '0' ) 
		{
			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("780$a"), sItemData );
			if ( sItemData != _T("") ) {
				sTitlet7XXTagCardList = sTitlet7XXTagCardList + _T(" ") + sItemData;
			}
			
			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("780$g"), sItemData );
			if ( sItemData != _T("") ) {
				sTitlet7XXTagCardList = sTitlet7XXTagCardList + _T(" ") + sItemData;
			}
			
			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("780$t"), sItemData );
			if ( sItemData != _T("") ) {
				sTitlet7XXTagCardList = sTitlet7XXTagCardList + _T(" ") + sItemData;
			}
			
			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("780$x"), sItemData );
			if ( sItemData != _T("") ) {
				sTitlet7XXTagCardList =  _T("ISSN ") + sTitlet7XXTagCardList + _T(" ") + sItemData;
			}
			
			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("780$z"), sItemData );
			if ( sItemData != _T("") ) {
				sTitlet7XXTagCardList =  _T("IBN ") + sTitlet7XXTagCardList + _T(" ") + sItemData;
			}
		}
	}

	return sTitlet7XXTagCardList;

EFS_END
return _T("");
}

CString CBO_CAT_PROC_0202::CardList_Title_OtherTag(CMarc *pMarc)
{
EFS_BEGIN
		
	CString sTitletOtherTagCardList;
	if ( pMarc == NULL ) return _T("");
	
	CString sItemData;
	CString sMarcField;
	
	m_pInfo->pMarcMgr->GetField( pMarc, _T("507"), sMarcField );

	sItemData = _T("");
	CArray <CString, CString&> pArray;
	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("507$t"),  sItemData, &pArray );
	INT iCount = pArray.GetSize();
	for ( INT i = 0 ; i < iCount ; i++ ) 
	{
		sItemData = pArray.GetAt(i);
		sTitletOtherTagCardList = sTitletOtherTagCardList + _T("\r\n원서명: ") +  sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("507$z"), sItemData );
	if ( sItemData != _T("") ) {
		sTitletOtherTagCardList = _T("ISBN ") + sTitletOtherTagCardList + _T(" ") + sItemData.Mid(0,2) + _T("-") + sItemData.Mid(2,3) + _T("-") + sItemData.Mid(5,4) + _T("-") + sItemData.Mid(9,1) + _T("\r\n");
	}

	if ( sItemData != _T("")  && sMarcField != _T("")) {
		if ( sMarcField.Mid(1,1) == '1' ) 
		{
			sTitletOtherTagCardList.Replace(_T(")"), _T(""));
			sTitletOtherTagCardList.Replace(_T("("), _T(""));

		}
	}

	m_pInfo->pMarcMgr->GetField( pMarc, _T("765"), sMarcField );

	if (  sMarcField != _T("") )
	{
		if ( sMarcField.Mid(0,1) == '0' ) 
		{
			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("765$t"), sItemData );
			if ( sItemData != _T("") ) {
				sTitletOtherTagCardList = _T("서명: ") + sTitletOtherTagCardList + _T(" ") + sItemData;
			}
			
			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("765$z"), sItemData );
			if ( sItemData != _T("") ) {
				
				sTitletOtherTagCardList = _T("ISBN ") + sTitletOtherTagCardList + _T(" ") + sItemData.Mid(0,2) + _T("-") + sItemData.Mid(2,3) + _T("-") + sItemData.Mid(5,4) + _T("-") + sItemData.Mid(9,1);
			}
		}
	}

	m_pInfo->pMarcMgr->GetField( pMarc, _T("767"), sMarcField );

	if ( sMarcField != _T("") ) 
	{
		if ( sMarcField.Mid(0,1) == '0' ) 
		{
			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("767$t"), sItemData );
			if ( sItemData != _T("") ) {
				sTitletOtherTagCardList = _T("서명: ") + sTitletOtherTagCardList + _T(" ") + sItemData;
			}
			
			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("767$z"), sItemData );
			if ( sItemData != _T("") ) {
				
				sTitletOtherTagCardList = _T("ISBN ") + sTitletOtherTagCardList + _T(" ") + sItemData.Mid(0,2) + _T("-") + sItemData.Mid(2,3) + _T("-") + sItemData.Mid(5,4) + _T("-") + sItemData.Mid(9,1);
			}
		}	
	}

	return sTitletOtherTagCardList;

EFS_END
return _T("");
}

CString CBO_CAT_PROC_0202::CardList_Author_5XXTag(CMarc *pMarc)
{
EFS_BEGIN
	
	CString sAuthor5XXTagCardList;
	if ( pMarc == NULL ) return _T("");
	
	CString sItemData;
	CString sMarcField;
	
	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$l"), sItemData );
	if ( sItemData != _T("") ) {
		sAuthor5XXTagCardList = _T("번역저작자: ") + sAuthor5XXTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$s"), sItemData );
	if ( sItemData != _T("") ) {
		sAuthor5XXTagCardList = _T("총서명편자: ") + sAuthor5XXTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$d"), sItemData );
	if ( sItemData != _T("") ) {
		sAuthor5XXTagCardList = sAuthor5XXTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	CArray <CString, CString&> pArray;
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("507$a"),  sItemData, &pArray );
	INT iCount = pArray.GetSize();
	for ( INT i = 0 ; i < iCount ; i++ ) 
	{
		sAuthor5XXTagCardList += _T("\r\n"); 
		sItemData = pArray.GetAt(i);
		sAuthor5XXTagCardList = sAuthor5XXTagCardList + _T("원저자명: ") + sItemData;
	}

	m_pInfo->pMarcMgr->GetField( pMarc, _T("500"), sMarcField );
	
	if ( sItemData != _T("")  && sMarcField != _T("")) {
		if ( sMarcField.Mid(1,1) == '1' ) 
		{
			sAuthor5XXTagCardList.Replace(_T(")"), _T(""));
			sAuthor5XXTagCardList.Replace(_T("("), _T(""));
		}
	}
	return sAuthor5XXTagCardList;


EFS_END
return _T("");
}

CString CBO_CAT_PROC_0202::CardList_Author_7XXTag(CMarc *pMarc)
{
EFS_BEGIN
	CString sAuthor7XXTagCardList;
	if ( pMarc == NULL ) return _T("");

	CString sItemData;
	CString sMarcField;

	m_pInfo->pMarcMgr->GetField( pMarc, _T("765"), sMarcField );
	
	if (  sMarcField != _T("") )
	{
		if ( sMarcField.Mid(0,1) == '0' ) 
		{		
			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("765$a"), sItemData );
			if ( sItemData != _T("") ) {
				sAuthor7XXTagCardList = _T("원저자명: ") + sAuthor7XXTagCardList + _T(" ") + sItemData;
			}
		}
	}

	
	m_pInfo->pMarcMgr->GetField( pMarc, _T("767"), sMarcField );
	
	if ( sMarcField != _T("") )
	{
		if ( sMarcField.Mid(0,1) == '0' ) 
		{		
			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("767$a"), sItemData );
			if ( sItemData != _T("") ) {
				sAuthor7XXTagCardList = _T("번역자명: ") + sAuthor7XXTagCardList + _T(" ") + sItemData;
			}
		}
	}

	return sAuthor7XXTagCardList;
	
EFS_END
return _T("");
	
}

CString CBO_CAT_PROC_0202::CardList_Other_AllTag(CMarc *pMarc)
{
EFS_BEGIN
	
	CString sOtherAllTagCardList;
	if ( pMarc == NULL ) return _T("");

	CString sItemData;
	CString sMarcField;
	CString sInd1, sInd2;

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$n"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}
	
	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$p"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$q"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("500$t"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("580$a"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("787$t"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("502$a"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData + _T(" - ");
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("502$b"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("502$c"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" , ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("502$d"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(",") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("506$a"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}	

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("521$a"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = _T("\r\n이용대상자:") + sOtherAllTagCardList + _T(" ") + sItemData;
	}	

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetField( pMarc, _T("511"),  sItemData );
	sItemData.Replace(SUBFIELD_CODE, '$');

	if ( !sItemData.IsEmpty() )
	{
		sItemData = sItemData.Mid(2);
		sItemData.Replace(_T("$a"), _T(" "));

		sItemData.Replace( FIELD_TERMINATOR , ' ');

		sOtherAllTagCardList = sOtherAllTagCardList + _T("\r\n배역") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("504$a"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sMarcField = _T("");
	m_pInfo->pMarcMgr->GetField( pMarc, _T("770"), sMarcField );
	sItemData = _T("");
	if (  sMarcField != _T("") )
	{
		sInd1 = sMarcField.Mid(0,1);

		if( sInd1 == _T("0") ){
			sItemData = sMarcField.Mid(2);

			sItemData.Replace(SUBFIELD_CODE,'$');
			sItemData.Replace(_T("$a"),_T(" "));
			sItemData.Replace(_T("$b"),_T(" "));
			sItemData.Replace(_T("$c"),_T(" "));
			sItemData.Replace(_T("$d"),_T(" "));
			sItemData.Replace(_T("$g"),_T(" "));
			sItemData.Replace(_T("$k"),_T(" "));
			sItemData.Replace(_T("$s"),_T(" "));
			sItemData.Replace(_T("$t"),_T(" "));
			sItemData.Replace(_T("$w"),_T(" "));
			sItemData.Replace(_T("$x"),_T(" ISSN "));
			sItemData.Replace(_T("$y"),_T(" "));
			sItemData.Replace(_T("$z"),_T(" ISBN "));
			
			sItemData.Replace( FIELD_TERMINATOR , ' ');

			CString strLeader;
			pMarc->GetLeader(strLeader);
			if( _T("c") == strLeader.Mid(18,1) || _T(" ") == strLeader.Mid(18,1) )
				sOtherAllTagCardList = sOtherAllTagCardList + _T("\r\n부록:") + sItemData;
			else
				sOtherAllTagCardList = sOtherAllTagCardList + _T("\r\n부록/보유:") + sItemData;

		}
		
	}

	sMarcField = _T("");
	m_pInfo->pMarcMgr->GetField( pMarc, _T("775"), sMarcField );
	sItemData = _T("");
	if ( sMarcField != _T("") )
	{
		sInd1 = sMarcField.Mid(0,1);

		if( sInd1 == _T("0") ){
		sItemData = sMarcField.Mid(2);

		sItemData.Replace(SUBFIELD_CODE,'$');
		sItemData.Replace(_T("$a"),_T(" "));
		sItemData.Replace(_T("$b"),_T(" "));
		sItemData.Replace(_T("$c"),_T(" "));
		sItemData.Replace(_T("$d"),_T(" "));
		sItemData.Replace(_T("$e"),_T(" "));
		sItemData.Replace(_T("$f"),_T(" "));
		sItemData.Replace(_T("$g"),_T(" "));
		sItemData.Replace(_T("$k"),_T(" "));
		sItemData.Replace(_T("$s"),_T(" "));
		sItemData.Replace(_T("$t"),_T(" "));
		sItemData.Replace(_T("$w"),_T(" "));
		sItemData.Replace(_T("$x"),_T(" ISSN "));
		sItemData.Replace(_T("$y"),_T(" "));
		
		sItemData.Replace( FIELD_TERMINATOR , ' ');
		sOtherAllTagCardList = sOtherAllTagCardList + _T("\r\n이용가능한 다른 판:") + sItemData;
		}
	}

	sMarcField = _T("");
	m_pInfo->pMarcMgr->GetField( pMarc, _T("772"), sMarcField );
	sItemData = _T("");
	if ( sMarcField != _T("") )
	{
		sInd1 = sMarcField.Mid(0,1);

		if( sInd1 == _T("0") ){
			sItemData = sMarcField.Mid(2);

			sItemData.Replace(SUBFIELD_CODE,'$');
			sItemData.Replace(_T("$a"),_T(" "));
			sItemData.Replace(_T("$b"),_T(" "));
			sItemData.Replace(_T("$c"),_T(" "));
			sItemData.Replace(_T("$d"),_T(" "));
			sItemData.Replace(_T("$g"),_T(" "));
			sItemData.Replace(_T("$k"),_T(" "));
			sItemData.Replace(_T("$s"),_T(" "));
			sItemData.Replace(_T("$t"),_T(" "));
			sItemData.Replace(_T("$w"),_T(" "));
			sItemData.Replace(_T("$x"),_T(" ISSN "));
			sItemData.Replace(_T("$y"),_T(" "));
					
			sItemData.Replace( FIELD_TERMINATOR , ' ');
			CString strLeader;
			pMarc->GetLeader(strLeader);
			if( _T("c") == strLeader.Mid(18,1) || _T(" ") == strLeader.Mid(18,1) )
				sOtherAllTagCardList = sOtherAllTagCardList + _T("\r\n모체자료:") + sItemData;
			else
				sOtherAllTagCardList = sOtherAllTagCardList + _T("\r\n모체지:") + sItemData;
		}
	}

	sMarcField = _T("");
	m_pInfo->pMarcMgr->GetField( pMarc, _T("773"), sMarcField );
	sItemData = _T("");

	if (  sMarcField != _T("") )
	{
		sInd1 = sMarcField.Mid(0,1);

		if( sInd1 == _T("0") ){
		sItemData = sMarcField.Mid(2);

		sItemData.Replace(SUBFIELD_CODE,'$');
		sItemData.Replace(_T("$a"),_T(" "));
		sItemData.Replace(_T("$b"),_T(" "));
		sItemData.Replace(_T("$d"),_T(" "));
		sItemData.Replace(_T("$g"),_T(" "));
		sItemData.Replace(_T("$k"),_T(" "));
		sItemData.Replace(_T("$p"),_T(" "));
		sItemData.Replace(_T("$r"),_T(" "));
		sItemData.Replace(_T("$s"),_T(" "));
		sItemData.Replace(_T("$t"),_T(" "));
		sItemData.Replace(_T("$u"),_T(" "));
		sItemData.Replace(_T("$w"),_T(" "));
		sItemData.Replace(_T("$x"),_T(" ISSN "));
		sItemData.Replace(_T("$y"),_T(" "));
		sItemData.Replace(_T("$z"),_T(" ISBN"));
				
		sItemData.Replace( FIELD_TERMINATOR , ' ');

		sOtherAllTagCardList = sOtherAllTagCardList + _T("\r\n수록처:") + sItemData;
		}
	}

	m_pInfo->pMarcMgr->GetField( pMarc, _T("776"), sMarcField );

	if ( sMarcField != _T("") ) 
	{
		if ( sMarcField.Mid(0,1) == '0' )
		{
			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("776$t"), sItemData );
			if ( sItemData != _T("") ) {
				sOtherAllTagCardList = _T("이용가능한 다른 형태자료: ") +sOtherAllTagCardList + _T(" ") + sItemData;
			}

			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("776$z"), sItemData );
			if ( sItemData != _T("") ) {
				sOtherAllTagCardList = _T("이용가능한 다른 형태자료: ") +sOtherAllTagCardList + _T(" ") + sItemData.Mid(0,2) + _T("-") + sItemData.Mid(2,3) + _T("-") + sItemData.Mid(5,4) + _T("-") + sItemData.Mid(9,1);
			}
		}
	}

	sMarcField = _T("");
	m_pInfo->pMarcMgr->GetField( pMarc, _T("777"), sMarcField );
	sItemData = _T("");
	if ( sMarcField != _T("") ) 
	{
		sInd1 = sMarcField.Mid(0,1);

		if( sInd1 == _T("0") ){
			sItemData = sMarcField.Mid(2);

			sItemData.Replace(SUBFIELD_CODE,'$');
			sItemData.Replace(_T("$a"),_T(" "));
			sItemData.Replace(_T("$b"),_T(" "));
			sItemData.Replace(_T("$c"),_T(" "));
			sItemData.Replace(_T("$d"),_T(" "));
			sItemData.Replace(_T("$g"),_T(" "));
			sItemData.Replace(_T("$k"),_T(" "));
			sItemData.Replace(_T("$s"),_T(" "));
			sItemData.Replace(_T("$t"),_T(" "));
			sItemData.Replace(_T("$u"),_T(" "));
			sItemData.Replace(_T("$w"),_T(" "));
			sItemData.Replace(_T("$x"),_T(" ISSN "));
			sItemData.Replace(_T("$y"),_T(" "));

			sItemData.Replace( FIELD_TERMINATOR , ' ');
			CString strLeader;
			pMarc->GetLeader(strLeader);
			if( _T("c") == strLeader.Mid(18,1) || _T(" ") == strLeader.Mid(18,1) )
				sOtherAllTagCardList = sOtherAllTagCardList + _T("\r\n합철: ") + sItemData;
			else		
				sOtherAllTagCardList = sOtherAllTagCardList + _T("\r\n합쇄: ") + sItemData;
		}
		
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("533$a"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = _T("복제: ") +  sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("533$b"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("533$c"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("533$d"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("533$e"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("534$p"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("534$c"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}


	sMarcField = _T("");
	m_pInfo->pMarcMgr->GetField( pMarc, _T("555"), sMarcField );
	sItemData = _T("");
	if ( sMarcField != _T("") ) 
	{
		sItemData = sMarcField.Mid(2);

		sItemData.Replace(SUBFIELD_CODE,'$');
		sItemData.Replace(_T("$a"),_T(" "));

		sItemData.Replace( FIELD_TERMINATOR , ' ');
		CString strLeader;
		pMarc->GetLeader(strLeader);
		if( _T("c") == strLeader.Mid(18,1) || _T(" ") == strLeader.Mid(18,1) )
			sOtherAllTagCardList = sOtherAllTagCardList + _T("\r\n색인:") + sItemData;
		else
			sOtherAllTagCardList = sOtherAllTagCardList + _T("\r\n누적색인:") + sItemData;
	}
	m_pInfo->pMarcMgr->GetField( pMarc, _T("856"), sMarcField );
	
	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("856$u"), sItemData );
	if ( sItemData != _T("") && sMarcField != _T("") ) {
		if ( sMarcField.Mid(1,1) == ' ' ) sOtherAllTagCardList = sOtherAllTagCardList + _T("전자자료: ") + sItemData;
		else if ( sMarcField.Mid(1,1) == '0' ) sOtherAllTagCardList = sOtherAllTagCardList + _T("전자자료: ") + sItemData;
		else if ( sMarcField.Mid(1,1) == '1' ) sOtherAllTagCardList = sOtherAllTagCardList + _T("전자 버전: ") + sItemData;
		else if ( sMarcField.Mid(1,1) == '2' ) sOtherAllTagCardList = sOtherAllTagCardList + _T("관련 전자자료: ") + sItemData;
		else if ( sMarcField.Mid(1,1) == '8' ) sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;

	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("510$a"), sItemData );
	if ( sItemData != _T("") ) {
		CString strLeader;
		pMarc->GetLeader(strLeader);
		if( _T("c") == strLeader.Mid(18,1) || _T(" ") == strLeader.Mid(18,1) )
			sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
		else
			sOtherAllTagCardList = _T("해제/초록수록지 : ") + sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("536$a"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("586$a"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = _T("수상: ") + sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("538$a"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}

	m_pInfo->pMarcMgr->GetField( pMarc, _T("590"), sMarcField );

	if ( sMarcField != _T("") ) 
	{
		if ( sMarcField.Mid(0,1) == '1' )
		{
			sItemData = _T("");
			m_pInfo->pMarcMgr->GetItem( pMarc, _T("590$b"), sItemData );
			if ( sItemData != _T("") ) {
				sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
			}
		}
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetField( pMarc, _T("505"), sItemData );

	sItemData.Replace(SUBFIELD_CODE, '$');

	CString strInd;
	if ( sItemData != _T("") )
	{
		strInd = sItemData.Mid(0,1);
		sItemData = sItemData.Mid(2);
		sItemData.Replace(_T("$a"), _T(" "));
		sItemData.Replace(_T("$b"), _T(" "));
		sItemData.Replace(_T("$c"), _T(" "));
		sItemData.Replace(_T("$i"), _T(" "));
		sItemData.Replace(_T("$n"), _T(" "));
		sItemData.Replace(_T("$t"), _T(" "));
		sItemData.Replace(_T("$g"), _T(" "));
		sItemData.Replace(_T("$d"), _T(" "));
		sItemData.Replace(_T("$e"), _T(" "));
		sItemData.Replace( FIELD_TERMINATOR , ' ');
	}

	if ( sItemData != _T("") ) {
		if( strInd == _T("0") )
			sOtherAllTagCardList = sOtherAllTagCardList + _T("\r\n내용:") + sItemData;
		else if( strInd == _T("1") )
			sOtherAllTagCardList = sOtherAllTagCardList + _T("\r\n불완전내용:") + sItemData;
		else if( strInd == _T("2") )
			sOtherAllTagCardList = sOtherAllTagCardList + _T("\r\n완전내용:") + sItemData;
		else if( strInd == _T(" ") )
			sOtherAllTagCardList = sOtherAllTagCardList + _T("\r\n내용:") + sItemData;

	}
	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("501$a"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = _T("합철: ") + sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("520$a"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = _T("요약: ") + sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("520$b"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = _T("초록: ") + sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("520$c"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = _T("해제: ") + sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	CArray <CString, CString&> pArraya;
	CArray <CString, CString&> pArrayg;
	CArray <CString, CString&> pArrayc;
	CArray <CString, CString&> pArrayz;
	pArraya.RemoveAll();
	pArrayg.RemoveAll();
	pArrayc.RemoveAll();
	pArrayz.RemoveAll();
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("020$a"),  sItemData, &pArraya );
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("020$g"),  sItemData, &pArrayg );
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("020$c"),  sItemData, &pArrayc );
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("020$z"),  sItemData, &pArrayz );
	INT ISBNCount = pArraya.GetSize();

	INT nSizeg = pArrayg.GetSize();
	INT nSizec = pArrayc.GetSize();
	INT nSizez = pArrayz.GetSize();

	for ( INT i = 0 ; i < ISBNCount ; i++ ) 
	{
		sOtherAllTagCardList += _T("\r\n");
		sItemData = pArraya.GetAt(i);

		if ( sItemData.GetLength() >= 10 )
		{
			sOtherAllTagCardList = sOtherAllTagCardList + _T("ISBN ") + sItemData.Mid(0,2) + _T("-") + sItemData.Mid(2,3) + _T("-") + sItemData.Mid(5,4) + _T("-") + sItemData.Mid(9,sItemData.GetLength()) ;
		}
		else
		{
			sOtherAllTagCardList = sOtherAllTagCardList + _T("ISBN ") + sItemData;
		}
		
		sItemData = _T("");
		if ( nSizeg > i ) 
			sItemData = pArrayg.GetAt(i);
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;

		sItemData = _T("");
		if ( nSizec > i ) 
			sItemData = pArrayc.GetAt(i);
		if ( !sItemData.IsEmpty() )
			sOtherAllTagCardList = sOtherAllTagCardList + _T(" : ") + sItemData;

		sItemData = _T("");
		if ( nSizez > i ) 
			sItemData = pArrayz.GetAt(i);
		if ( !sItemData.IsEmpty() )
			sOtherAllTagCardList = sOtherAllTagCardList + _T(" : ") + sItemData;

	} 

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("022$a"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = _T("ISSN ") + sOtherAllTagCardList + _T(" ") + sItemData;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("022$g"), sItemData );
	if ( sItemData != _T("") ) {
		sOtherAllTagCardList = sOtherAllTagCardList + _T(" ") + sItemData;
	}
	return sOtherAllTagCardList;

EFS_END
return _T("");
}

CString CBO_CAT_PROC_0202::GetBookCodes(CMarc *pMarc)
{
EFS_BEGIN

	CString sCardPrint;
	if ( m_iPageNumber > 1 )
		sCardPrint.Format(_T("                     ( 카드 %d )"), m_iPageNumber );
	else
		sCardPrint = _T("");

	CString sItemData;

	m_pInfo->pMarcMgr->GetItem( pMarc, _T("049$f"), sItemData );
	if ( sItemData != _T("") ) {
		m_sCard = m_sCard + _T("        ") + sItemData + sCardPrint + _T("\r\n");
	}else m_sCard = m_sCard + sCardPrint + _T("\r\n");

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("056$a"), sItemData );
	if ( sItemData != _T("") ) {
		m_sCard = m_sCard + _T("        ") + sItemData + _T("\r\n") ;
	}else m_sCard = m_sCard + _T("\r\n");
	m_iLine++;

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("090$b"), sItemData );
	if ( sItemData != _T("") ) {
		m_sCard = m_sCard + _T("        ") + sItemData + _T("\r\n");
	}else m_sCard = m_sCard + _T("\r\n");
	m_iLine++;

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("090$c"), sItemData );
	if ( sItemData != _T("") ) {
		m_sCard = m_sCard + _T("        ") + sItemData;
	}
	m_iLine++;
	m_iLine++;

EFS_END
return _T("");
}

CString CBO_CAT_PROC_0202::ExecuteCutString(CString sPostString, CMarc *pMarc)
{
EFS_BEGIN

	if ( sPostString == _T("") )  return _T("");
	CString s4Line;

	if ( m_iLine == 0 ) 
		GetBookCodes(pMarc);
		
	while( sPostString.GetLength() > 45 )
	{
		INT iPosCut = sPostString.Find( _T("\n") );
		if ( iPosCut < 2 ) {
			CString sRightStr = sPostString.Mid(iPosCut+1);
			sPostString = sRightStr;
			iPosCut = sPostString.Find( _T("\n") );
		}

		if ( iPosCut > -2 && iPosCut < 0 ) break;

		if ( iPosCut != -1 ) {
			CString sLeftStr = sPostString.Left(iPosCut-1);

			if ( m_iLine == 0 ) 
				GetBookCodes(pMarc);
			
			while( sLeftStr.GetLength() > 45) {
				INT iWordCount = 45;
				s4Line = sLeftStr.Left(45);
				sLeftStr.TrimLeft();

				INT iPos = sPostString.Find(_T("\r\n"));
				if ( iPos < 45 && iPos > 0 ) 
				{
					s4Line = sPostString.Left(iPos);
					iWordCount = iPos;
				}
				else 
				{
					s4Line = sPostString.Left(45);
					iWordCount = 45;
				}
				

				BOOL bCh = IsCanCut( sLeftStr, iWordCount );
				if ( bCh == TRUE ) {
					iWordCount--;
					s4Line = sLeftStr.Left(iWordCount);
				}
				else {
					s4Line = sLeftStr.Left(iWordCount);
				}
				m_iLine++;
				if ( m_iLine >= 14 ) {
					m_sCard = m_sCard + _T("         (다음 카드에 계속)\r\n\r\n\r\n\r\n");
					m_iPageNumber++;
					m_iLine++;

					if ( m_iLine == 15 ) 
					{
						m_sCard = m_sCard;
						CString sSiLine = _T("           ") + s4Line;
						m_iLine = 0;						
					}
				}
				CString sSiLine = _T("           ") + s4Line;
				m_sCard = m_sCard + _T("    \r\n           ") + s4Line;
				sLeftStr = sLeftStr.Mid(iWordCount);
			}
		
			if ( sLeftStr != _T("") )
			{
			
				m_iLine++;
				if ( m_iLine >= 14 ) {
					m_sCard = m_sCard + _T("         (다음 카드에 계속)\r\n\r\n\r\n\r\n");
					m_iPageNumber++;
					m_iLine++;

					if ( m_iLine == 15 ) 
					{
						m_sCard = m_sCard;
						m_iLine = 0;
					}
				}
				if ( m_iLine > 14 ) m_iLine = 0;
				if ( sLeftStr == _T("|") ) m_sCard = m_sCard + _T("\r\n");
				else m_sCard = m_sCard + _T("\r\n           ") + sLeftStr;

				CString sRightStr = sPostString.Mid(iPosCut+1);
				sPostString = sRightStr;
				if ( sPostString.GetLength() <= 45 ) break;
				INT iTmp = 1;
			}
			
		}

		if ( m_iLine == 0 ) 
			GetBookCodes(pMarc);

		if ( m_iLine != 9)
		{
			sPostString.TrimLeft();
			INT iWordCount = 45;
			INT iPos = sPostString.Find(_T("\r\n"));
			if ( iPos < 45 && iPos > 0 ) 
			{
				s4Line = sPostString.Left(iPos);
				iWordCount = iPos;
			}
			else 
			{
				s4Line = sPostString.Left(45);
				iWordCount = 45;
			}
			
			sPostString.TrimLeft();
			BOOL bCh = IsCanCut( sPostString, iWordCount );
			if ( bCh == TRUE ) {
				iWordCount--;
				s4Line = sPostString.Left(iWordCount);
			}
			else {
				s4Line = sPostString.Left(iWordCount);
			}
			
			if ( s4Line == _T("|") )  s4Line = _T("");
			else s4Line = _T("           ") + s4Line;

			m_iLine++;
			if ( m_iLine < 13 ) 
			{
				m_sCard = m_sCard + _T("\r\n") + s4Line;
				sPostString = sPostString.Mid(iWordCount);
			}
	
			if ( m_iLine >= 13 ) {

				m_sCard = m_sCard + _T("\r\n                  (다음 카드에 계속)\r\n\r\n\r\n\r\n");
				m_iPageNumber++;
				m_iLine = 1 ;

				if ( m_iLine == 15 ) 
				{
					m_sCard = m_sCard;
					m_iLine = 0;
				}
				
			}
		}		
		else if ( m_iLine == 15 )
		{
			m_sCard = m_sCard + _T("              ");
			m_iLine++;
		}
		
	}

	INT iPos = sPostString.Find( _T("\n") );
	
	if ( iPos < 45 && iPos != -1) 
	{
		
	}
	
	
	INT iWordCount = 45;
	sPostString.TrimLeft();
	iPos = sPostString.Find(_T("\r\n"));
	if ( iPos < 45 && iPos > 0 ) 
	{
		s4Line = sPostString.Left(iPos);
		iWordCount = iPos;
	}
	else 
	{
		s4Line = sPostString.Left(45);
		iWordCount = 45;
	}
	s4Line.TrimLeft();
	if ( s4Line == _T("|") )  s4Line = _T("");
	else s4Line = _T("           ") + s4Line;
	m_iLine++;
	m_sCard = m_sCard + _T("\r\n") + s4Line;
	if ( iPos > 0 )
	{
		sPostString = sPostString.Mid(iPos+2);
		sPostString.TrimLeft();
		sPostString.Replace(_T("|"), _T(" "));
		if ( sPostString.Find(_T("\r\n\r\n")) > 0 )
		{
			sPostString.Replace(_T("\r\n\r\n"), _T("\r\n\r\n           "));
			m_iLine++;
			m_iLine++;
			s4Line = _T("           ") + sPostString;
			m_sCard = m_sCard + s4Line;
		}
		else if ( sPostString.Find(_T("\r\n")) > 0 )
		{
			m_iLine++;
			s4Line = _T("           ") + sPostString;
			m_sCard = m_sCard + s4Line;
		}
		else
		{
			s4Line = _T("           ") + sPostString;
			m_sCard = m_sCard + s4Line;
		}
	}

	return sPostString;

	
EFS_END
return _T("");
}


CBO_CAT_PROC_0202::~CBO_CAT_PROC_0202()
{
EFS_BEGIN

	
	if ( m_pCard != NULL) 
		m_pCard = NULL;

	if ( m_list != NULL )  
		delete m_list;


EFS_END
}

VOID CBO_CAT_PROC_0202::OnCancel() 
{
EFS_BEGIN	

	CDialog::OnCancel();

EFS_END
}

BOOL CBO_CAT_PROC_0202::MakeBuchulList(CMarc *pMarc)
{
EFS_BEGIN

	if ( pMarc == NULL ) return FALSE;

	Buchul_Title_List(pMarc);

	Buchul_Author_List(pMarc);

	Buchul_Classify_List(pMarc);

	Buchul_Subject_List(pMarc);
	
	Buchul_RegNo_List(pMarc);

	if ( m_iLine < 14 ) 
	{
		for ( INT i = m_iLine; i < 14; i++ )
		{
			m_sCard = m_sCard + _T("\r\n");
			m_iLine++;
		}
	}
	m_iLine = 0;

	return TRUE;
		
EFS_END
return FALSE;
}

VOID CBO_CAT_PROC_0202::Buchul_Title_List(CMarc *pMarc)
{
EFS_BEGIN

	CString sItemData;
	INT iPrintCnt = 0;
	sItemData = _T("");

	m_pInfo->pMarcMgr->GetItem( pMarc, _T("245$a"), sItemData );
	if ( sItemData != _T("") ) {
		m_sCard = m_sCard + sItemData + _T("\r\n부출지시사항\r\n\r\n");
		m_iLine++;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("240$a"), sItemData );
	if ( sItemData != _T("") ) {
		iPrintCnt++;
		CString sTmpPrintFormat;
		sTmpPrintFormat.Format(_T("T%02d       %s\r\n"), iPrintCnt, sItemData );
		m_sCard = m_sCard + sTmpPrintFormat;
		m_iLine++;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("245$a"), sItemData );
	if ( sItemData != _T("") ) {
		iPrintCnt++;
		CString sTmpPrintFormat;
		sTmpPrintFormat.Format(_T("T%02d       %s\r\n"), iPrintCnt, sItemData );
		m_sCard = m_sCard + sTmpPrintFormat;
		m_iLine++;
	}
	
	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("245$x"), sItemData );
	if ( sItemData != _T("") ) {
		iPrintCnt++;
		CString sTmpPrintFormat;
		sTmpPrintFormat.Format(_T("T%02d       %s\r\n"), iPrintCnt, sItemData );
		m_sCard = m_sCard + sTmpPrintFormat;
		m_iLine++;
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("440$x"), sItemData );
	if ( sItemData != _T("") ) {
		iPrintCnt++;
		CString sTmpPrintFormat;
		sTmpPrintFormat.Format(_T("T%02d       %s\r\n"), iPrintCnt, sItemData );
		m_sCard = m_sCard + sTmpPrintFormat;
		m_iLine++;
	}
	
	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("440$a"), sItemData );
	if ( sItemData != _T("") ) {
		iPrintCnt++;
		CString sTmpPrintFormat;
		sTmpPrintFormat.Format(_T("T%02d       %s\r\n"), iPrintCnt, sItemData );
		m_sCard = m_sCard + sTmpPrintFormat;
		m_iLine++;
	}

	sItemData = _T("");
	CString strTagField = _T("");
	m_pInfo->pMarcMgr->GetField( pMarc, _T("440"), strTagField );
	
	if ( strTagField != _T("") )
	{
		m_pInfo->pMarcMgr->GetItem( pMarc, _T("440$s"), sItemData );
		if ( sItemData != _T("") ) {
			iPrintCnt++;
			CString sTmpPrintFormat;
			sTmpPrintFormat.Format(_T("T%02d       %s."), iPrintCnt, sItemData );
			m_sCard = m_sCard + sTmpPrintFormat;
			m_iLine++;
		}

		if ( sItemData == _T("") ) {
			iPrintCnt++;
			CString sTmpPrintFormat;
			sTmpPrintFormat.Format(_T("T%02d"), iPrintCnt);
			m_sCard = m_sCard + sTmpPrintFormat;
			m_iLine++;

		}

		sItemData = _T("");
		m_pInfo->pMarcMgr->GetItem( pMarc, _T("440$n"), sItemData );
		if ( sItemData != _T("") ) {
			m_sCard = m_sCard + sItemData + _T(", ");
			m_iLine++;
		}

		sItemData = _T("");
		m_pInfo->pMarcMgr->GetItem( pMarc, _T("440$p"), sItemData );
		if ( sItemData != _T("") ) {
			m_sCard = m_sCard + sItemData + _T("; ");
			m_iLine++;
		}

		if ( sItemData == _T("") ) {
			iPrintCnt++;
			CString sTmpPrintFormat;
			sTmpPrintFormat.Format(_T("T%02d"), iPrintCnt);
			m_sCard = m_sCard + sTmpPrintFormat;
			m_iLine++;
			
		}

		sItemData = _T("");
		m_pInfo->pMarcMgr->GetItem( pMarc, _T("440$v"), sItemData );
		if ( sItemData != _T("") ) {
			m_sCard = m_sCard + sItemData;
			m_iLine++;
		}
		m_sCard = m_sCard + _T("\r\n");
	}

EFS_END
}

VOID CBO_CAT_PROC_0202::Buchul_Author_List(CMarc *pMarc)
{
EFS_BEGIN

	CString sItemData = _T("");
	INT iPrintCnt = 0;

	CString strTagField = _T("");
	m_pInfo->pMarcMgr->GetField( pMarc, _T("110"), strTagField );
	
	if ( strTagField != _T("") )
	{
		m_pInfo->pMarcMgr->GetItem( pMarc, _T("100$a"), sItemData );
		if ( sItemData != _T("") ) {
			iPrintCnt++;
			CString sTmpPrintFormat;
			sTmpPrintFormat.Format(_T("A%02d       %s\r\n"), iPrintCnt, sItemData );
			m_sCard = m_sCard + sTmpPrintFormat;
			m_iLine++;
		}

		if ( sItemData == _T("") ) {
			iPrintCnt++;
			CString sTmpPrintFormat;
			sTmpPrintFormat.Format(_T("A%02d"), iPrintCnt);
			m_sCard = m_sCard + sTmpPrintFormat;
			m_iLine++;
			
		}

		m_pInfo->pMarcMgr->GetItem( pMarc, _T("100$c"), sItemData );
		if ( sItemData != _T("") ) {
			m_sCard = m_sCard + sItemData + _T(",");
		}

		if ( sItemData == _T("") ) {
			iPrintCnt++;
			CString sTmpPrintFormat;
			sTmpPrintFormat.Format(_T("A%02d"), iPrintCnt);
			m_sCard = m_sCard + sTmpPrintFormat;
			m_iLine++;
			
		}

		m_pInfo->pMarcMgr->GetItem( pMarc, _T("100$d"), sItemData );
		if ( sItemData != _T("") ) {
			m_sCard = m_sCard + sItemData + _T("\r\n");
			m_iLine++;
		}

	}

	m_pInfo->pMarcMgr->GetItem( pMarc, _T("245$d"), sItemData );
	if ( sItemData != _T("") ) {
		iPrintCnt++;
		CString sTmpPrintFormat;
		sTmpPrintFormat.Format(_T("A%02d       %s\r\n"), iPrintCnt, sItemData );
		m_sCard = m_sCard + sTmpPrintFormat;
		m_iLine++;
	}

	m_pInfo->pMarcMgr->GetItem( pMarc, _T("507$a"), sItemData );
	if ( sItemData != _T("") ) {
		iPrintCnt++;
		CString sTmpPrintFormat;
		sTmpPrintFormat.Format(_T("A%02d       %s\r\n"), iPrintCnt, sItemData );
		m_sCard = m_sCard +  sTmpPrintFormat;
		m_iLine++;
	}

	return;

EFS_END
return;
}

VOID CBO_CAT_PROC_0202::Buchul_Classify_List(CMarc *pMarc)
{
EFS_BEGIN

	CString sItemData = _T("");
	INT iPrintCnt = 0;
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("056$a"), sItemData );
	if ( sItemData != _T("") ) {
		iPrintCnt++;
		CString sTmpPrintFormat;
		sTmpPrintFormat.Format(_T("K%02d       %s\r\n"), iPrintCnt, sItemData );
		m_sCard = m_sCard + sTmpPrintFormat;
		m_iLine++;
	}

EFS_END
}

VOID CBO_CAT_PROC_0202::Buchul_Subject_List(CMarc *pMarc)
{
EFS_BEGIN

	CString sItemData = _T("");
	INT iPrintCnt = 0;
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("090$a"), sItemData );
	if ( sItemData != _T("") ) {
		iPrintCnt++;
		CString sTmpPrintFormat;
		sTmpPrintFormat.Format(_T("CAL       %s"), sItemData );
		m_sCard = m_sCard + sTmpPrintFormat;
		m_iLine++;
	}

	m_pInfo->pMarcMgr->GetItem( pMarc, _T("090$b"), sItemData );
	if ( sItemData != _T("") ) {
		m_sCard = m_sCard + sItemData + _T("\r\n\r\n");
		m_iLine++;
	}

EFS_END
}

VOID CBO_CAT_PROC_0202::Buchul_RegNo_List(CMarc *pMarc)
{
EFS_BEGIN

	CString sItemData;
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("049$l"), sItemData );
	if ( sItemData != _T("") ) {
		m_sCard = m_sCard + sItemData + _T("\r\n");
		m_iLine++;
	}

	return;
EFS_END
return;
}


CString CBO_CAT_PROC_0202::MakeSearchWhereQuery()
{
EFS_BEGIN

	CString sACQUISIT_YEAR;
	CString sAccessionNo;
	CString sRegNoStart, sRegNoEnd, sRight;

	BeginWaitCursor();

	GetControlData(_T("CM_BO_CAT_PROC_0202"), _T("BS_수입년도"), sACQUISIT_YEAR);
	GetControlData(_T("CM_BO_CAT_PROC_0202"), _T("원부번호"), sAccessionNo);
	GetControlData(_T("CM_BO_CAT_PROC_0202"), _T("등록번호부터"), sRegNoStart);
	GetControlData(_T("CM_BO_CAT_PROC_0202"), _T("등록번호까지"), sRegNoEnd);

	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	
	if( sRegNoStart.GetLength() < 12  && sRegNoStart.GetLength() > nRegCodeLength )
	{
		sRegNoStart = m_pInfo->MakeRegNo(sRegNoStart.Left(nRegCodeLength),sRegNoStart.Mid(nRegCodeLength));
	}

	if( sRegNoEnd.GetLength() < 12 && sRegNoEnd.GetLength() > nRegCodeLength )
	{
		sRegNoEnd = m_pInfo->MakeRegNo(sRegNoEnd.Left(nRegCodeLength),sRegNoEnd.Mid(nRegCodeLength));
	}
	
	CString strBookTblName;
	if (m_nModeMONO_SERIAL == MODE_MONO)
		strBookTblName = _T("BO_BOOK_TBL");
	else
		strBookTblName = _T("SE_BOOK_TBL");
	
	CString sWhere = _T("");
	CString sBookWhere = _T("");


	sWhere = _T("S.REC_KEY = B.SPECIES_KEY ");

	if ( sACQUISIT_YEAR != _T("") && sAccessionNo != _T("") && sRegNoStart == _T("") && sRegNoEnd == _T("") )
	{
		sBookWhere = _T(" AND S.REC_KEY IN (SELECT B.SPECIES_KEY FROM CO_ACCESSION_REC_TBL A, ") + strBookTblName + _T(" B WHERE A.REC_KEY = B.ACCESSION_REC_KEY AND A.ACCESSION_REC_NO = '") + sAccessionNo + _T("'") + _T(" AND ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'") ;
		sWhere = sWhere + sBookWhere + _T(")");
	}
	else if ( sACQUISIT_YEAR == _T("") && sAccessionNo == _T("") && sRegNoStart != _T("") && sRegNoEnd != _T("") )
	{
		sBookWhere = _T(" AND S.REC_KEY IN (SELECT B.SPECIES_KEY FROM ") + strBookTblName + _T(" B WHERE ");
		sBookWhere = sBookWhere + _T(" B.REG_NO >= '") + sRegNoStart + _T("'");
		sBookWhere = sBookWhere + _T(" AND B.REG_NO <= '") + sRegNoEnd + _T("'");
		sWhere = sWhere + sBookWhere + _T(")");
	}
	else if ( sACQUISIT_YEAR != _T("") && sAccessionNo != _T("") && sRegNoStart != _T("") && sRegNoEnd != _T("") )
	{
		sBookWhere = _T(" AND S.REC_KEY IN (SELECT B.SPECIES_KEY FROM CO_ACCESSION_REC_TBL A, ") + strBookTblName + _T(" B WHERE A.REC_KEY = B.ACCESSION_REC_KEY AND A.ACCESSION_REC_NO = '") + sAccessionNo + _T("'") + _T(" AND ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("'") ;
		
		if ( sRegNoStart != _T("") ) {
			sBookWhere = sBookWhere + _T(" AND B.REG_NO >= '") + sRegNoStart + _T("'");
		}
		if ( sRegNoEnd != _T("") ) {
			sBookWhere = sBookWhere + _T(" AND B.REG_NO <= '") + sRegNoEnd + _T("'");
		}

		sWhere = sWhere + sBookWhere + _T(")");
	}	
	else 
	{
		AfxMessageBox(_T("검색 조건 항목을 확인해 주세요"));
		return _T("");
	}

	return sWhere + _T(" group BY B.species_key) reg_table where reg_table.species=s2.rec_key order by reg_table.reg ") ;
	
EFS_END
return _T("");
}


HBRUSH CBO_CAT_PROC_0202::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
