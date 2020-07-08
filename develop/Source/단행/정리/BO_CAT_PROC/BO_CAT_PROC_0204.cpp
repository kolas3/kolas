// BO_CAT_PROC_0204.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_PROC_0204.h"
#include "BO_CAT_PROC_0204_01.h"

#include "BO_CAT_PROC_0204_CLICK_LIST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBO_CAT_PROC_0204::CBO_CAT_PROC_0204(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0204)
		m_strValue=_T("");
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0204::~CBO_CAT_PROC_0204()
{
}

VOID CBO_CAT_PROC_0204::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0204)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0204, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0204)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_b0204SEARCH, Onb0204SEARCH)
	ON_BN_CLICKED(IDC_b0204PRINT, Onb0204PRINT)
	ON_CBN_EDITCHANGE(IDC_cCLASS_TYPE, OnEditchangecCLASSTYPE)
	ON_CBN_SELCHANGE(IDC_cCLASS_TYPE, OnSelchangecCLASSTYPE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0204 message handlers

BOOL CBO_CAT_PROC_0204::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0204::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0204")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	GetManageValue( _T("기타") , _T("공통") , _T("육사") , _T("육사") , m_strValue );

	if ( m_strValue == _T("Y") )
	{
		CESL_ControlMgr *pCM;
		pCM = FindCM(_T("CM_BO_CAT_PROC_0204"));
		if ( pCM == NULL ) {
			AfxMessageBox ( _T("CM_BO_CAT_PROC_0204 error") );
			return FALSE;
		}
		CESL_Grid *pGrid = (CESL_Grid*)FindControl(_T("CM_BO_CAT_PROC_0204"), _T("그리드_원부류별통계"));		
		pGrid->SetTextMatrix(0,7,_T("3\r\n/군사"));
		pGrid->SetRowHeight(0, 500);
		pGrid->SetColWidth(7,0, 600);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0204::Onb0204SEARCH() 
{
	m_arStrQuery_GridCellClick.RemoveAll();
	m_arStr_GridCellClick_sRegNo.RemoveAll();

	INT	ids;

	ids = Search(0);
	if (  ids == 100 ) return;
	ids = Search(1);
	if (  ids == 100 ) return;

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0204"));
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_BO_CAT_PROC_0204 error") );
		return ;
	}	

	pCM->AllControlDisplay();	
	
	CESL_DataMgr *pDM;
	CESL_DataMgr *pDM_LC;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0204_RESULT"));
	pDM_LC = FindDM(_T("DM_BO_CAT_PROC_0204_RESULT_LC"));				
	if ( pDM->GetRowCount() < 1 && pDM_LC->GetRowCount() < 1 )
	{
		AfxMessageBox(_T("검색된 결과가 없습니다."));
	}
	
}

VOID CBO_CAT_PROC_0204::Onb0204PRINT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CBO_CAT_PROC_0204_01 dlg(this);
	dlg.DoModal();

EFS_END
}

VOID CBO_CAT_PROC_0204::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CBO_CAT_PROC_0204::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->lParam == VK_RETURN )
		{
			Onb0204SEARCH();
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_CAT_PROC_0204::OnEditchangecCLASSTYPE() 
{
	
}

VOID CBO_CAT_PROC_0204::OnSelchangecCLASSTYPE() 
{
EFS_BEGIN
	CString sClassType;

	CESL_Grid *pGrid = (CESL_Grid*)FindControl(_T("CM_BO_CAT_PROC_0204"), _T("그리드_원부류별통계"));
	CESL_Grid *pGrid2 = (CESL_Grid*)FindControl(_T("CM_BO_CAT_PROC_0204"), _T("그리드_원부류별통계_LC"));	

	GetControlData( _T("CM_BO_CAT_PROC_0204"), _T("분류기호구분"), sClassType );
	if ( sClassType != '7' )
	{
		pGrid->ShowWindow(SW_SHOW);
		pGrid2->ShowWindow(SW_HIDE);				
	}
	else
	{
		pGrid->ShowWindow(SW_HIDE);
		pGrid2->ShowWindow(SW_SHOW);		
	}	
EFS_END
}

INT CBO_CAT_PROC_0204::Search(INT nType)
{
	EFS_BEGIN

	CString strSearchClass;	
	GetControlData(_T("CM_BO_CAT_PROC_0204"), _T("검색구분"), strSearchClass);

	CString sACQUISIT_YEAR;
	CString sAccessionNo;

	GetControlData(_T("CM_BO_CAT_PROC_0204"), _T("수입년도"), sACQUISIT_YEAR);
	GetControlData(_T("CM_BO_CAT_PROC_0204"), _T("원부번호"), sAccessionNo);	

	if ( sACQUISIT_YEAR.IsEmpty() || sAccessionNo.IsEmpty() ) {
		AfxMessageBox( _T("조건항목이 모두 입력되지 않았습니다.") );
		return 100;
	}

	CESL_DataMgr *pDM;

	if ( nType == 0 )
	{
		pDM = FindDM(_T("DM_BO_CAT_PROC_0204_RESULT"));
	}
	else
	{
		pDM = FindDM(_T("DM_BO_CAT_PROC_0204_RESULT_LC"));				
	}	
	if ( pDM == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0204 error") );
		return -100;
	}	

	CString strGroup;
	CString strNon;
	if (strSearchClass == _T("등록구분"))
	{
		strGroup = _T("REG_CODE");
		strNon = _T("별치기호");
	}
	else if (strSearchClass == _T("별치기호"))
	{
		strGroup = _T("SEPARATE_SHELF_CODE");
		strNon = _T("등록구분");
	}
	
	CESL_DataMgr::reference *pRef = NULL;
	POSITION pos = pDM->RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)pDM->RefList.GetNext(pos);
		if (!pRef) continue;


		if (pRef->FIELD_ALIAS.Find(strNon) >= 0)
		{
			pRef->DATA_TYPE = _T("udf");
		}
		if (pRef->FIELD_ALIAS.Find(strSearchClass) >= 0)
		{
			pRef->DATA_TYPE = _T("");
		}
		if (pRef->FIELD_ALIAS == _T("구분설명"))
		{
			pRef->FIELD_NAME = strGroup;
		}
	}

	CString strAppendixBookIncludeYN;
	GetManageValue( _T("정리") , _T("단행") , _T("류별통계 부록포함여부") , _T("") , strAppendixBookIncludeYN );	

	CString strWhere;
	if ( strAppendixBookIncludeYN == _T("Y") )
	{
		strWhere.Format( _T("ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR = '%s' AND ACCESSION_REC_NO = '%s' AND MANAGE_CODE=UDF_MANAGE_CODE) GROUP BY %s"), sACQUISIT_YEAR, sAccessionNo, strGroup );
	}
	else
	{
		strWhere.Format( _T("BOOK_APPENDIX_FLAG='B' AND ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR = '%s' AND ACCESSION_REC_NO = '%s' AND MANAGE_CODE=UDF_MANAGE_CODE) GROUP BY %s"), sACQUISIT_YEAR, sAccessionNo, strGroup );
	}
	if (pDM->RefreshDataManager(strWhere) < 0)	return -100;
	
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("등록구분"), _T("등록구분_설명"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("별치기호"), _T("별치기호_설명"));

	if (strSearchClass == _T("등록구분"))
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("등록구분"), _T("구분설명"));
	else if (strSearchClass == _T("별치기호"))
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("별치기호"), _T("구분설명"));

	INT iRegCodeCount = pDM->GetRowCount();
	if ( iRegCodeCount < 1 ) {		
		return 0;
	}
	for ( INT j = 0 ; j < iRegCodeCount ; j ++ ) {
		CString strSearchCode = pDM->GetCellData( strSearchClass, j );
		pDM->SetCellData( _T("원부번호"), sAccessionNo, j );

		CString sTmp = pDM->GetCellData( _T("책수"), j );
		
		CESL_DataMgr *pDM_0204;
		pDM_0204 = FindDM(_T("DM_BO_CAT_PROC_0204"));
		if ( pDM_0204 == NULL ) {
			AfxMessageBox ( _T("DM_BO_CAT_PROC_0204 error") );
			return -110;
		}

		if (strSearchCode.IsEmpty())
			strSearchCode = ' ';

		strWhere.Format( _T("ACCESSION_REC_MAKE_YEAR = '%s' AND ACCESSION_REC_NO = '%s' AND REC_KEY IN ( SELECT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE %s ='%s' AND MANAGE_CODE=UDF_MANAGE_CODE )"), sACQUISIT_YEAR, sAccessionNo, strGroup, strSearchCode);
		strSearchCode.Replace( _T(" "), NULL );
		if ( strSearchCode == _T("") ) {
			strWhere.Format( _T("ACCESSION_REC_MAKE_YEAR = '%s' AND ACCESSION_REC_NO = '%s' AND REC_KEY IN ( SELECT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE %s is null AND MANAGE_CODE=UDF_MANAGE_CODE)"), sACQUISIT_YEAR, sAccessionNo, strGroup);
		}
		if (pDM_0204->RefreshDataManager(strWhere) < 0)	return -120;
		if ( pDM_0204->GetRowCount() < 1 ) {
			continue;
		}

		INT iRowCount = pDM_0204->GetRowCount();
		
		CESL_DataMgr DmRegNo;
		DmRegNo.SetCONNECTION_INFO(pDM->CONNECTION_INFO);	
		CString sQueryRegNo;
		
		if ( strAppendixBookIncludeYN == _T("Y") )
		{
			sQueryRegNo.Format( _T("SELECT MIN (reg_no), MAX (reg_no) FROM bo_book_tbl WHERE %s = '%s' AND accession_rec_key IN (SELECT rec_key FROM co_accession_rec_tbl WHERE accession_rec_make_year = '%s' AND accession_rec_no = '%s') AND MANAGE_CODE = UDF_MANAGE_CODE"), strGroup, strSearchCode, sACQUISIT_YEAR, sAccessionNo );
			strSearchCode.Replace( _T(" "), NULL );
			if ( strSearchCode == _T("") ) {
				sQueryRegNo.Format( _T("SELECT MIN (reg_no), MAX (reg_no) FROM bo_book_tbl WHERE %s is null AND accession_rec_key IN (SELECT rec_key FROM co_accession_rec_tbl WHERE accession_rec_make_year = '%s' AND accession_rec_no = '%s') AND MANAGE_CODE = UDF_MANAGE_CODE"), strGroup, sACQUISIT_YEAR, sAccessionNo );		
			}
		}
		else
		{
			sQueryRegNo.Format( _T("SELECT MIN (reg_no), MAX (reg_no) FROM bo_book_tbl WHERE %s = '%s' AND BOOK_APPENDIX_FLAG='B' AND accession_rec_key IN (SELECT rec_key FROM co_accession_rec_tbl WHERE accession_rec_make_year = '%s' AND accession_rec_no = '%s') AND MANAGE_CODE = UDF_MANAGE_CODE"), strGroup, strSearchCode, sACQUISIT_YEAR, sAccessionNo );
			strSearchCode.Replace( _T(" "), NULL );
			if ( strSearchCode == _T("") ) {
				sQueryRegNo.Format( _T("SELECT MIN (reg_no), MAX (reg_no) FROM bo_book_tbl WHERE %s is null AND BOOK_APPENDIX_FLAG='B' AND accession_rec_key IN (SELECT rec_key FROM co_accession_rec_tbl WHERE accession_rec_make_year = '%s' AND accession_rec_no = '%s') AND MANAGE_CODE = UDF_MANAGE_CODE"), strGroup, sACQUISIT_YEAR, sAccessionNo );		
			}
		}
		
		DmRegNo.RestructDataManager(sQueryRegNo);
		INT iRegNoCount = DmRegNo.GetRowCount();
		CString sStartRegNo = DmRegNo.GetCellData( 0, 0 );
		CString sEndRegNo = DmRegNo.GetCellData( 0, 1 );
		CString sRegNo ;
		if ( nType == 0 )
		{			
			sRegNo = sStartRegNo + _T(" ~ ") + sEndRegNo;
		}
		else
		{
			sRegNo = sStartRegNo + _T(" ~\n") + sEndRegNo;
		}
		pDM->SetCellData( _T("등록번호"), sRegNo, j );

		CString sAcqCode = pDM_0204->GetCellData( _T("입수구분"), 0 );
		pDM->SetCellData( _T("입수구분"), sAcqCode, j );
		
		CESL_DataMgr dm_css;
		dm_css.SetCONNECTION_INFO(pDM->CONNECTION_INFO);	
		
		CString	sGridCellClick = _T("");
		
		CString sQuery_css;				
		if ( strSearchClass == _T("별치기호") ) {
			strSearchCode.Replace(_T(" "), NULL);		
			if ( m_strValue != _T("Y") )
			{	
				if ( strAppendixBookIncludeYN == _T("Y") )
				{
					sQuery_css = _T("SELECT SUBSTR(CLASS_NO,0,1), COUNT(*) FROM BO_BOOK_TBL WHERE ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("' AND ACCESSION_REC_NO = '") + sAccessionNo + _T("') AND REG_NO >= '") + sStartRegNo + _T("' AND REG_NO <= '") + sEndRegNo + _T("' AND separate_shelf_code = '") + strSearchCode + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE GROUP BY SUBSTR(CLASS_NO,0,1)");
					
					sGridCellClick = _T("SELECT b.rec_key , B.REG_NO , i.title_info FROM BO_BOOK_TBL B , idx_bo_tbl i WHERE I.REC_KEY=B.SPECIES_KEY AND B.ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("' AND ACCESSION_REC_NO = '") + sAccessionNo + _T("') AND B.REG_NO >= '") + sStartRegNo + _T("' AND B.REG_NO <= '") + sEndRegNo + _T("' AND B.separate_shelf_code = '") + strSearchCode + _T("' AND B.MANAGE_CODE = UDF_MANAGE_CODE AND substr(b.class_no,0,1) = '%CELL_CLICK_CLASS%'");					

					if ( strSearchCode == _T("") ) {
						sQuery_css = _T("SELECT SUBSTR(CLASS_NO,0,1), COUNT(*) FROM BO_BOOK_TBL WHERE ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("' AND ACCESSION_REC_NO = '") + sAccessionNo + _T("') AND REG_NO >= '") + sStartRegNo + _T("' AND REG_NO <= '") + sEndRegNo + _T("' AND separate_shelf_code is null AND MANAGE_CODE = UDF_MANAGE_CODE GROUP BY SUBSTR(CLASS_NO,0,1)");
						
						sGridCellClick = _T("SELECT b.rec_key , B.REG_NO , i.title_info FROM BO_BOOK_TBL B , idx_bo_tbl i WHERE  I.REC_KEY=B.SPECIES_KEY AND B.ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("' AND ACCESSION_REC_NO = '") + sAccessionNo + _T("') AND B.REG_NO >= '") + sStartRegNo + _T("' AND B.REG_NO <= '") + sEndRegNo + _T("' AND B.separate_shelf_code is null AND B.MANAGE_CODE = UDF_MANAGE_CODE AND substr(b.class_no,0,1) = '%CELL_CLICK_CLASS%'");						
					}				
				}
				else
				{
					sQuery_css = _T("SELECT SUBSTR(CLASS_NO,0,1), COUNT(*) FROM BO_BOOK_TBL WHERE BOOK_APPENDIX_FLAG='B' AND ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("' AND ACCESSION_REC_NO = '") + sAccessionNo + _T("') AND REG_NO >= '") + sStartRegNo + _T("' AND REG_NO <= '") + sEndRegNo + _T("' AND separate_shelf_code = '") + strSearchCode + _T("' AND MANAGE_CODE = UDF_MANAGE_CODE GROUP BY SUBSTR(CLASS_NO,0,1)");
					
					sGridCellClick = _T("SELECT b.rec_key , B.REG_NO , i.title_info FROM BO_BOOK_TBL B , idx_bo_tbl i WHERE  I.REC_KEY=B.SPECIES_KEY AND B.BOOK_APPENDIX_FLAG='B' AND B.ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("' AND ACCESSION_REC_NO = '") + sAccessionNo + _T("') AND B.REG_NO >= '") + sStartRegNo + _T("' AND B.REG_NO <= '") + sEndRegNo + _T("' AND B.separate_shelf_code = '") + strSearchCode + _T("' AND B.MANAGE_CODE = UDF_MANAGE_CODE AND substr(b.class_no,0,1) = '%CELL_CLICK_CLASS%'");
					
					if ( strSearchCode == _T("") ) {
						sQuery_css = _T("SELECT SUBSTR(CLASS_NO,0,1), COUNT(*) FROM BO_BOOK_TBL WHERE BOOK_APPENDIX_FLAG='B' AND ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("' AND ACCESSION_REC_NO = '") + sAccessionNo + _T("') AND REG_NO >= '") + sStartRegNo + _T("' AND REG_NO <= '") + sEndRegNo + _T("' AND separate_shelf_code is null AND MANAGE_CODE = UDF_MANAGE_CODE GROUP BY SUBSTR(CLASS_NO,0,1)");

						sGridCellClick = _T("SELECT b.rec_key , B.REG_NO , i.title_info FROM BO_BOOK_TBL B , idx_bo_tbl i WHERE  I.REC_KEY=B.SPECIES_KEY AND B.BOOK_APPENDIX_FLAG='B' AND B.ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("' AND ACCESSION_REC_NO = '") + sAccessionNo + _T("') AND B.REG_NO >= '") + sStartRegNo + _T("' AND B.REG_NO <= '") + sEndRegNo + _T("' AND B.separate_shelf_code is null AND B.MANAGE_CODE = UDF_MANAGE_CODE AND substr(b.class_no,0,1) = '%CELL_CLICK_CLASS%'");
					}				
				}				
			}
			else
			{
				sQuery_css = _T("SELECT DECODE( SIGN(SUBSTR(LTRIM(CLASS_NO, ' '),0,3)-'355'),NULL,'X', -1, SUBSTR(LTRIM(CLASS_NO, ' '),0,1), ")
					_T("DECODE( SIGN('359'-LTRIM(CLASS_NO, ' ')),-1,SUBSTR(LTRIM(CLASS_NO, ' '),0,1), '355-9' ) ) ,COUNT(*) ")
					_T("FROM BO_BOOK_TBL WHERE ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ")
					_T("ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("' AND ")
					_T("ACCESSION_REC_NO = '") + sAccessionNo + _T("') AND ")
					_T("REG_NO >= '") + sStartRegNo + _T("' AND ")
					_T("REG_NO <= '") + sEndRegNo + _T("' AND ")
					_T("separate_shelf_code = '") + strSearchCode + _T("' ")
					_T("AND MANAGE_CODE = UDF_MANAGE_CODE ")
					_T("GROUP BY DECODE( SIGN(SUBSTR(LTRIM(CLASS_NO, ' '),0,3)-'355'),NULL,'X', -1, SUBSTR(LTRIM(CLASS_NO, ' '),0,1), ")
					_T("DECODE( SIGN('359'-LTRIM(CLASS_NO, ' ')),-1,SUBSTR(LTRIM(CLASS_NO, ' '),0,1), '355-9' ) ) ");
				if ( strSearchCode == _T("") ) {
					sQuery_css = _T("SELECT DECODE( SIGN(SUBSTR(LTRIM(CLASS_NO, ' '),0,3)-'355'),NULL,'X', -1, SUBSTR(LTRIM(CLASS_NO, ' '),0,1), ")
						_T("DECODE( SIGN('359'-LTRIM(CLASS_NO, ' ')),-1,SUBSTR(LTRIM(CLASS_NO, ' '),0,1), '355-9' ) ) ,COUNT(*) ")
						_T("FROM BO_BOOK_TBL WHERE ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ")
						_T("ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("' AND ")
						_T("ACCESSION_REC_NO = '") + sAccessionNo + _T("') AND ")
						_T("REG_NO >= '") + sStartRegNo + _T("' AND ")
						_T("REG_NO <= '") + sEndRegNo + _T("' AND ")
						_T("separate_shelf_code is null ")
						_T("AND MANAGE_CODE = UDF_MANAGE_CODE")
						_T("GROUP BY DECODE( SIGN(SUBSTR(LTRIM(CLASS_NO, ' '),0,3)-'355'),NULL,'X', -1, SUBSTR(LTRIM(CLASS_NO, ' '),0,1), ")
						_T("DECODE( SIGN('359'-LTRIM(CLASS_NO, ' ')),-1,SUBSTR(LTRIM(CLASS_NO, ' '),0,1), '355-9' ) )");
				}
			}
		}
		else {
			
			if ( m_strValue != _T("Y") )
			{
				sQuery_css = _T("SELECT SUBSTR(LTRIM(CLASS_NO, ' '),0,1), COUNT(LTRIM(CLASS_NO, ' ')) FROM BO_BOOK_TBL WHERE ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE NVL(") + strGroup + _T(", ' ') = '") + strSearchCode + _T("' AND ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("' AND ACCESSION_REC_NO = '") + sAccessionNo + _T("') AND MANAGE_CODE = UDF_MANAGE_CODE GROUP BY SUBSTR(LTRIM(CLASS_NO, ' '),0,1)");
			
				sGridCellClick = _T("SELECT b.rec_key , B.REG_NO , i.title_info FROM BO_BOOK_TBL B , idx_bo_tbl i WHERE  I.REC_KEY=B.SPECIES_KEY AND B.ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE NVL(b.") + strGroup + _T(", ' ') = '") + strSearchCode + _T("' AND ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("' AND ACCESSION_REC_NO = '") + sAccessionNo + _T("') AND B.MANAGE_CODE = UDF_MANAGE_CODE AND SUBSTR(LTRIM(b.CLASS_NO, ' '),0,1) = '%CELL_CLICK_CLASS%'");				
			}
			else
			{
				sQuery_css = _T("SELECT DECODE( SIGN(SUBSTR(LTRIM(CLASS_NO, ' '),0,3)-'355'),NULL,'X', -1, SUBSTR(LTRIM(CLASS_NO, ' '),0,1), ")
					_T("DECODE( SIGN('359'-LTRIM(CLASS_NO, ' ')),-1,SUBSTR(LTRIM(CLASS_NO, ' '),0,1), '355-9' ) ) ,COUNT(*) ")
					_T("FROM BO_BOOK_TBL WHERE ACCESSION_REC_KEY IN (SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ")
					_T("NVL(") + strGroup + _T(", ' ') = '") + strSearchCode + _T("' AND ")
					_T("ACCESSION_REC_MAKE_YEAR = '") + sACQUISIT_YEAR + _T("' AND ")
					_T("ACCESSION_REC_NO = '") + sAccessionNo + _T("') ")
					_T("AND MANAGE_CODE = UDF_MANAGE_CODE");
					_T("GROUP BY DECODE( SIGN(SUBSTR(LTRIM(CLASS_NO, ' '),0,3)-'355'),NULL,'X',-1, SUBSTR(LTRIM(CLASS_NO, ' '),0,1), ")
					_T("DECODE( SIGN('359'-LTRIM(CLASS_NO, ' ')),-1,SUBSTR(LTRIM(CLASS_NO, ' '),0,1),'355-9' ) )");
			}
		}

		m_arStrQuery_GridCellClick.Add( sGridCellClick );
		m_arStr_GridCellClick_sRegNo.Add( sRegNo );

		dm_css.RestructDataManager(sQuery_css);
		
		CString sCssTotal;
		CString sCss355_9;
		INT iCssTotal = 0;

		INT iCssCount = dm_css.GetRowCount();
		for ( INT i = 0 ; i < iCssCount ; i++ ) {
			CString sCss = dm_css.GetCellData( i, 0 );
			CString sCount = dm_css.GetCellData( i, 1 );
			INT iRow = _ttoi ( sCss.GetBuffer(0) );
			if ( sCss != _T("") || sCss != _T(" ") ) {
				
				if (m_strValue != _T("Y") )
				{
					if( _T("0") <= sCss && _T("9") >= sCss )
					{
						pDM->SetCellData( sCss, sCount, j );
						iCssTotal = iCssTotal + _ttoi ( sCount.GetBuffer(0) );
					}
				}
				else
				{
					if ( sCss == _T("355-9") )
					{
						sCss355_9 = sCount;
						iCssTotal = iCssTotal + _ttoi ( sCount.GetBuffer(0) );
					}
					else
					{
						pDM->SetCellData( sCss, sCount, j );
						iCssTotal = iCssTotal + _ttoi ( sCount.GetBuffer(0) );
					}
				}				
			}
		}

		INT iEtc = _ttoi ( sTmp.GetBuffer(0) ) - iCssTotal;
		CString sEtc;
		sEtc.Format( _T("%d"), iEtc );
		pDM->SetCellData( _T("기타"), sEtc, j );

		iCssTotal = iCssTotal + iEtc;
		sCssTotal.Format( _T("%d"), iCssTotal );
		pDM->SetCellData( _T("계"), sCssTotal, j );	
		
		if ( nType == 0 )
		{
			CString sCss0 = pDM->GetCellData( _T("0"), j );
			if ( sCss0 == _T("") ) pDM->SetCellData( _T("0"), _T("0"), j );
			CString sCss1 = pDM->GetCellData( _T("1"), j );
			if ( sCss1 == _T("") ) pDM->SetCellData( _T("1"), _T("0"), j );
			CString sCss2 = pDM->GetCellData( _T("2"), j );
			if ( sCss2 == _T("") ) pDM->SetCellData( _T("2"), _T("0"), j );
			CString sCss3 = pDM->GetCellData( _T("3"), j );
			if ( sCss3 == _T("") ) pDM->SetCellData( _T("3"), _T("0"), j );		
			if ( m_strValue == _T("Y"))
			{
				if ( sCss3 == _T("") ) sCss3 = _T("0");
				if ( sCss355_9 == _T("") ) sCss355_9 = _T("0");
				
				CString strTmp;			
				strTmp.Format(_T("%s\r\n/%s"), sCss3, sCss355_9);
				pDM->SetCellData( _T("3"), strTmp, j );
			}
			CString sCss4 = pDM->GetCellData( _T("4"), j );
			if ( sCss4 == _T("") ) pDM->SetCellData( _T("4"), _T("0"), j );
			CString sCss5 = pDM->GetCellData( _T("5"), j );
			if ( sCss5 == _T("") ) pDM->SetCellData( _T("5"), _T("0"), j );
			CString sCss6 = pDM->GetCellData( _T("6"), j );
			if ( sCss6 == _T("") ) pDM->SetCellData( _T("6"), _T("0"), j );
			CString sCss7 = pDM->GetCellData( _T("7"), j );
			if ( sCss7 == _T("") ) pDM->SetCellData( _T("7"), _T("0"), j );
			CString sCss8 = pDM->GetCellData( _T("8"), j );
			if ( sCss8 == _T("") ) pDM->SetCellData( _T("8"), _T("0"), j );
			CString sCss9 = pDM->GetCellData( _T("9"), j );
			if ( sCss9 == _T("") ) pDM->SetCellData( _T("9"), _T("0"), j );
			CString sCssEtc = pDM->GetCellData( _T("기타"), j );
			if ( sCssEtc == _T("") ) pDM->SetCellData( _T("기타"), _T("0"), j );
			CString sCssTot = pDM->GetCellData( _T("계"), j );
			if ( sCssTot == _T("") ) pDM->SetCellData( _T("계"), _T("0"), j );
		}
		else
		{
			CString sCssA = pDM->GetCellData( _T("A"), j );
			if ( sCssA == _T("") ) pDM->SetCellData( _T("A"), _T("0"), j );
			CString sCssB = pDM->GetCellData( _T("B"), j );
			if ( sCssB == _T("") ) pDM->SetCellData( _T("B"), _T("0"), j );
			CString sCssC = pDM->GetCellData( _T("C"), j );
			if ( sCssC == _T("") ) pDM->SetCellData( _T("C"), _T("0"), j );
			CString sCssD = pDM->GetCellData( _T("D"), j );
			if ( sCssD == _T("") ) pDM->SetCellData( _T("D"), _T("0"), j );					
			CString sCssE = pDM->GetCellData( _T("E"), j );
			if ( sCssE == _T("") ) pDM->SetCellData( _T("E"), _T("0"), j );
			CString sCssF = pDM->GetCellData( _T("F"), j );
			if ( sCssF == _T("") ) pDM->SetCellData( _T("F"), _T("0"), j );
			CString sCssG = pDM->GetCellData( _T("G"), j );
			if ( sCssG == _T("") ) pDM->SetCellData( _T("G"), _T("0"), j );
			CString sCssH = pDM->GetCellData( _T("H"), j );
			if ( sCssH == _T("") ) pDM->SetCellData( _T("H"), _T("0"), j );
			CString sCssJ = pDM->GetCellData( _T("J"), j );
			if ( sCssJ == _T("") ) pDM->SetCellData( _T("J"), _T("0"), j );
			CString sCssK = pDM->GetCellData( _T("K"), j );
			if ( sCssK == _T("") ) pDM->SetCellData( _T("K"), _T("0"), j );
			CString sCssL = pDM->GetCellData( _T("L"), j );
			if ( sCssL == _T("") ) pDM->SetCellData( _T("L"), _T("0"), j );
			CString sCssM = pDM->GetCellData( _T("M"), j );
			if ( sCssM == _T("") ) pDM->SetCellData( _T("M"), _T("0"), j );
			CString sCssN = pDM->GetCellData( _T("N"), j );
			if ( sCssN == _T("") ) pDM->SetCellData( _T("N"), _T("0"), j );
			CString sCssP = pDM->GetCellData( _T("P"), j );
			if ( sCssP == _T("") ) pDM->SetCellData( _T("P"), _T("0"), j );					
			CString sCssQ = pDM->GetCellData( _T("Q"), j );
			if ( sCssQ == _T("") ) pDM->SetCellData( _T("Q"), _T("0"), j );
			CString sCssR = pDM->GetCellData( _T("R"), j );
			if ( sCssR == _T("") ) pDM->SetCellData( _T("R"), _T("0"), j );
			CString sCssS = pDM->GetCellData( _T("S"), j );
			if ( sCssS == _T("") ) pDM->SetCellData( _T("S"), _T("0"), j );
			CString sCssT = pDM->GetCellData( _T("T"), j );
			if ( sCssT == _T("") ) pDM->SetCellData( _T("T"), _T("0"), j );
			CString sCssU = pDM->GetCellData( _T("U"), j );
			if ( sCssU == _T("") ) pDM->SetCellData( _T("U"), _T("0"), j );
			CString sCssV = pDM->GetCellData( _T("V"), j );
			if ( sCssV == _T("") ) pDM->SetCellData( _T("V"), _T("0"), j );
			CString sCssZ = pDM->GetCellData( _T("Z"), j );
			if ( sCssZ == _T("") ) pDM->SetCellData( _T("Z"), _T("0"), j );
			CString sCssEtc = pDM->GetCellData( _T("기타"), j );
			if ( sCssEtc == _T("") ) pDM->SetCellData( _T("기타"), _T("0"), j );
			CString sCssTot = pDM->GetCellData( _T("계"), j );
			if ( sCssTot == _T("") ) pDM->SetCellData( _T("계"), _T("0"), j );
		}
	}
	
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("입수구분"), _T("입수구분"));
		
	CESL_Grid *pGrid;
	if ( nType == 0 )
	{
		pGrid = (CESL_Grid*)FindControl(_T("CM_BO_CAT_PROC_0204"), _T("그리드_원부류별통계"));
		if (pGrid)
		{
			pGrid->SetTextMatrix(0, 3, strSearchClass);
		}		
	}
	else
	{
		pGrid = (CESL_Grid*)FindControl(_T("CM_BO_CAT_PROC_0204"), _T("그리드_원부류별통계_LC"));
		if (pGrid)
		{
			pGrid->SetTextMatrix(0, 3, strSearchClass);
		}
	}
	
	if ( m_strValue == _T("Y") )
	{		
		INT	nRowCnt = pDM->GetRowCount();
		pGrid->SetColWidth(7,0, 600);
		pGrid->m_nHeight = 600/23;
	}
	

	return 0;
	
	EFS_END
	return -1;	
}

VOID CBO_CAT_PROC_0204::ClassNoTypeCheck()
{
EFS_BEGIN

	CESL_DataMgr *pDM;
	CString sACQUISIT_YEAR;
	CString sAccessionNo;
	CString sClassType;	
	CString strWhere;

	GetControlData(_T("CM_BO_CAT_PROC_0204"), _T("수입년도"), sACQUISIT_YEAR);
	GetControlData(_T("CM_BO_CAT_PROC_0204"), _T("원부번호"), sAccessionNo);		
	
	strWhere.Format( _T("SELECT S.CLASS_NO_TYPE FROM BO_BOOK_TBL B, BO_SPECIES_TBL S ")
					 _T("WHERE B.SPECIES_KEY = S.REC_KEY AND ")
					 _T("ACCESSION_REC_KEY IN ( SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE ACCESSION_REC_MAKE_YEAR = '%s' ")
					 _T("AND ACCESSION_REC_NO = '%s') AND S.CLASS_NO_TYPE IS NOT NULL AND ROWNUM = 1 ")
					 _T(" AND B.MANAGE_CODE = UDF_MANAGE_CODE ")
					 _T("GROUP BY S.CLASS_NO_TYPE"), sACQUISIT_YEAR, sAccessionNo );

	pDM = FindDM(_T("DM_BO_CAT_PROC_0204_RESULT"));
	if ( pDM == NULL ) {
		return;
	}		

	pDM->GetOneValueQuery(strWhere, sClassType );

	if ( !sClassType.IsEmpty() )
	{
		SetControlData( _T("CM_BO_CAT_PROC_0204"), _T("분류기호구분"), sClassType );
		OnSelchangecCLASSTYPE();
	}
	
EFS_END
}
HBRUSH CBO_CAT_PROC_0204::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

CString	CBO_CAT_PROC_0204::GetQuery_CellClick()
{
	int i;
	CESL_Grid *pGrid = (CESL_Grid*)FindControl(_T("CM_BO_CAT_PROC_0204"), _T("그리드_원부류별통계"));

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	if( nCol < 5 || nCol > 14 )
		return _T("");

	if( (nRow-1) >= m_arStrQuery_GridCellClick.GetSize() ) 
		return _T("");

	CString click_cell = pGrid->GetTextMatrix(nRow, nCol );

	if( click_cell == _T("0") )
		return _T("");

	CString		Desc_x , Desc_y;
	Desc_x = pGrid->GetTextMatrix( 0, nCol );
	Desc_y = pGrid->GetTextMatrix( nRow , 1 );

	int query_idx = 0;
	for( i = 0 ; i < m_arStr_GridCellClick_sRegNo.GetSize() ; i++ )
	{
		CString sRegNo = m_arStr_GridCellClick_sRegNo[i];
		if( sRegNo == Desc_y )
		{
			query_idx = i;
			break;
		}
	}
	CString		queryCell = m_arStrQuery_GridCellClick.GetAt( query_idx );
	queryCell.Replace( _T("%CELL_CLICK_CLASS%") , Desc_x );
	return queryCell;
}



BEGIN_EVENTSINK_MAP(CBO_CAT_PROC_0204, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_PROC_0204)
	ON_EVENT(CBO_CAT_PROC_0204, IDC_g0204CssPrint, -600 /* Click */, OnClickg0204CssPrint, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CBO_CAT_PROC_0204::OnClickg0204CssPrint() 
{
	// TODO: Add your control notification handler code here
	CString cell_query = GetQuery_CellClick();

	if( cell_query.IsEmpty() ) return;
	CBO_CAT_PROC_0204_CLICK_LIST dlg;
	dlg.m_strQuery = cell_query;
	dlg.DoModal();
}
