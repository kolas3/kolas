// BO_CAT_REG_0500.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_REG_0500.h"
#include "BO_CAT_REG_0510.h"
#include "BO_CAT_REG_0520.h"
//#include _T("..\..\..\ESL\SIReportMgr\SIReportManager.h")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0500 dialog


CBO_CAT_REG_0500::CBO_CAT_REG_0500(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_0500)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CBO_CAT_REG_0500::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_0500)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_0500, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_0500)
	ON_BN_CLICKED(IDC_bAccessionInput, OnbAccessionInput)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bAccessionUpdate, OnbAccessionUpdate)
	ON_BN_CLICKED(IDC_rACCESSION_NO, OnrACCESSIONNO)
	ON_BN_CLICKED(IDC_rACCESSION_REC_NO, OnrACCESSIONRECNO)
	ON_BN_CLICKED(IDC_rACCESSION_DATE, OnrACCESSIONDATE)
	ON_BN_CLICKED(IDC_rACCESSION_DATE2, OnrACCESSIONDATE2)
	ON_BN_CLICKED(IDC_bAccessionDelete, OnbAccessionDelete)
	ON_BN_CLICKED(IDC_bCat0500PRINT, OnbCat0500PRINT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0500 message handlers

BOOL CBO_CAT_REG_0500::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_REG_0500")) < 0)
	//if (InitESL_Mgr(_T("BO_CAT_REG")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CButton * pRadio1;
	pRadio1=(CButton *)GetDlgItem(IDC_rACCESSION_REC_NO);
	pRadio1->SetCheck(1);	

	CEdit * pEdit3;
	pEdit3=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_START);
	pEdit3->EnableWindow(FALSE);
	//pEdit3->SetFocus();

	CEdit * pEdit4;
	pEdit4=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_END);
	pEdit4->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

BOOL CBO_CAT_REG_0500::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_REG_0500::OnbAccessionInput() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CBO_CAT_REG_0510 dlg(this);
	dlg.DoModal();	

EFS_END
}

VOID CBO_CAT_REG_0500::OnbSEARCH() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	//CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
	if ( pDM == NULL ) return;

	CButton *pRegNo, *pAccNo, *pRegDay, *pComDay;
	pRegNo=(CButton *)GetDlgItem(IDC_rACCESSION_NO);
	pAccNo=(CButton *)GetDlgItem(IDC_rACCESSION_REC_NO);
	pRegDay=(CButton *)GetDlgItem(IDC_rACCESSION_DATE);
	pComDay=(CButton *)GetDlgItem(IDC_rACCESSION_DATE2);
	
	CString strWhere;	
	CString sAccessionNumStart, sAccessionNumEnd;
	CString sDayStart, sDayEnd;
	GetControlData(_T("CM_BO_CAT_REG_0500"), _T("번호부터"), sAccessionNumStart);
	GetControlData(_T("CM_BO_CAT_REG_0500"), _T("번호까지"), sAccessionNumEnd);
	GetControlData(_T("CM_BO_CAT_REG_0500"), _T("날짜부터"), sDayStart);
	GetControlData(_T("CM_BO_CAT_REG_0500"), _T("날짜까지"), sDayEnd);
	//GetControlData(_T("CM_MAIN_SAMPLE_SEARCH"), _T("직급"), strLevel, -1, 0);

	//수입구분, 작성년도
	CString sACQUISIT_CODE, sACQUISIT_YEAR;
	GetControlData(_T("CM_BO_CAT_REG_0500"), _T("수입구분"), sACQUISIT_CODE);
	GetControlData(_T("CM_BO_CAT_REG_0500"), _T("작성년도"), sACQUISIT_YEAR);

	sACQUISIT_CODE.Replace( _T(" ") , NULL );
	sACQUISIT_YEAR.Replace( _T(" ") , NULL );

	CString sWhere;
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	if(pRegNo->GetCheck()==1){
		CString sQuery;
		if ( sAccessionNumStart == _T("") && sAccessionNumEnd != _T("") ) {
			AfxMessageBox ( _T("시작 등록번호를 입력하세요.") );
			return;
		}
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		if ( sAccessionNumStart != _T("") && sAccessionNumStart.GetLength() < 12 ) {
			//=====================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 			if ( sAccessionNumStart.GetLength() > 2 ) {
// 				CString sRight;
// 				sRight.Format( _T("%010s") , sAccessionNumStart.Mid(2) );
// 				sAccessionNumStart = sAccessionNumStart.Left(2) + sRight;
// 			}
			//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
			if ( sAccessionNumStart.GetLength() > nRegCodeLength ) {
				sAccessionNumStart = m_pParent->m_pInfo->MakeRegNo(sAccessionNumStart.Left(nRegCodeLength), sAccessionNumStart.Mid(nRegCodeLength) );
			}
			//=====================================================
			
		}
		if ( sAccessionNumEnd != _T("") && sAccessionNumEnd.GetLength() < 12 ) {
			//=====================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 			if ( sAccessionNumEnd.GetLength() > 2 ) {
// 				CString sRight;
// 				sRight.Format( _T("%010s") , sAccessionNumEnd.Mid(2) );
// 				sAccessionNumEnd = sAccessionNumEnd.Left(2) + sRight;
// 			}
			//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
			if ( sAccessionNumEnd.GetLength() > nRegCodeLength ) {
				sAccessionNumEnd = m_pParent->m_pInfo->MakeRegNo(sAccessionNumEnd.Left(nRegCodeLength), sAccessionNumEnd.Mid(nRegCodeLength) );
			}
			//=====================================================
			
		}
		if ( sAccessionNumStart != _T("") && sAccessionNumEnd == _T("") ) {
			sQuery = _T("SELECT DISTINCT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_NO = '") + sAccessionNumStart + _T("'");
			sWhere = _T("REC_KEY IN (SELECT DISTINCT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE REG_NO = '") + sAccessionNumStart + _T("')");
		}
		if ( sAccessionNumStart != _T("") && sAccessionNumEnd != _T("") ) {
			sQuery = _T("SELECT DISTINCT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_NO >= '") + sAccessionNumStart + _T("' AND REG_NO <= '") + sAccessionNumEnd + _T("'");
			sWhere = _T("REC_KEY IN (SELECT DISTINCT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE REG_NO >= '") + sAccessionNumStart + _T("' AND REG_NO <= '") + sAccessionNumEnd + _T("')");
		}
		/*
		dm.RestructDataManager(sQuery);
		//sCount.Format( _T("%d"), dm.GetRowCount());
		//pDM->SetCellData( i, 16, sCount);
		//sWhere = _T("AND (");
		for ( INT i = 0; i < dm.GetRowCount(); i++) {	
			CString sAccNum;
			dm.GetCellData( 0, 0, sAccNum);

			if ( i==0 ) {
				sWhere = _T("REC_KEY = ") + sAccNum;
			}
			else {
				sWhere = sWhere + _T(" OR REC_KEY = ") + sAccNum;
			}
			/*
			CString sCount;
			CString sQuery;
		
			//책수 구하기
			sQuery = _T("select COUNT(REC_KEY) from bo_book_tbl where ACCESSION_NO = ") + sAccNum;
			//pDM->GetOneValueQuery( _T("select COUNT(REC_KEY) from bo_book_tbl where ACCESSION_NO = 11"), sCount);
			pDM->GetOneValueQuery( sQuery , sCount);
			pDM->SetCellData( i, 17, sCount);			
			*/
		//}
		/*
		CESL_ControlMgr *pCM;
		pCM = FindCM(_T("CM_BO_CAT_REG_0500"));
		pCM->AllControlDisplay();	
		return;
		*/
	}
	
/*
	CString strOption;
	if (!strName.IsEmpty())
	{
		strOption.Format(_T("NAME like '%%%s%%' "), strName);
		strWhere += strOption;
	}

	strLevel=_T("2");
	if (strLevel != _T("0"))
	{
		if (strWhere.IsEmpty())
			strOption.Format(_T("GRADE = %s"), strLevel);
		else
			strOption.Format(_T("AND GRADE = %s"), strLevel);

		strWhere += strOption;
	}
*/
	//strWhere=_T("REC_KEY=50014143");
	/*
	strWhere = _T("rownum >= 0");
	if (!sAccessionNumStart.IsEmpty()) {
		strWhere=_T("ACCESSION_REC_NO = ") + sAccessionNumStart;
	}	
*/
	if(pRegNo->GetCheck()==1){
		strWhere = sWhere;
		if ( sWhere == _T("") ) {
			//strWhere = _T("rownum < 0 ");
		}
	}
	
	if(pAccNo->GetCheck()==1){		//원부번호 검색
		
		if ( sAccessionNumStart != _T("") || sAccessionNumEnd !=_T("") ) {
			if ( sAccessionNumStart >= _T("0") && sAccessionNumStart <= _T("99999")) {
			}
			else if ( sAccessionNumEnd >= _T("0") && sAccessionNumEnd <= _T("99999")) {
			}
			else {
				AfxMessageBox (_T("원부번호는 숫자를 입력하세요"));
				return;
			}
			/*
			else if ( sAccessionNumEnd >= _T("0") && sAccessionNumEnd <= _T("99999")) {
			}
			else {
				AfxMessageBox (_T("원부번호는 숫자를 입력하세요"));
				return;
			}
			*/
		}

		if ( sAccessionNumStart == _T("") && sAccessionNumEnd != _T("") ) {
			AfxMessageBox ( _T("시작 원부번호를 입력하세요.") );
			return;
		}

		if ( sAccessionNumStart != _T("") && sAccessionNumEnd == _T("") ) {			
			strWhere = _T("ACCESSION_REC_NO = '") + sAccessionNumStart + _T("'");
		}
		if ( sAccessionNumStart != _T("") && sAccessionNumEnd != _T("") ) {
			strWhere = _T("ACCESSION_REC_NO >= '") + sAccessionNumStart + _T("' AND ACCESSION_REC_NO <= '") + sAccessionNumEnd + _T("'");
		}
	}

	if(pRegDay->GetCheck()==1){		
		if ( sDayStart == _T("") && sDayEnd != _T("") ) {
			AfxMessageBox( _T("시작 등록일자를 입력하세요.") );
			return;
		}
		else if ( sDayStart != _T("") && sDayEnd == _T("") ) {
			strWhere = _T("REG_DATE = '") + sDayStart + _T("'");
		}
		else if ( sDayStart !=_T("") && sDayEnd != _T("") ) {
			strWhere = _T("REG_DATE >= '") + sDayStart + _T("' AND REG_DATE <= '") + sDayEnd + _T("'");
		}
	}
	if(pComDay->GetCheck()==1){
		if ( sDayStart == _T("") && sDayEnd != _T("") ) {
			AfxMessageBox( _T("시작 완료일자를 입력하세요.") );
			return;
		}
		else if ( sDayStart != _T("") && sDayEnd == _T("") ) {
			//strWhere = _T("CATALOG_DATE = '") + sDayStart + _T("'");
			strWhere = _T("PROCESSING_COMPLETE_DATE = '") + sDayStart + _T("'");
		}
		else if ( sDayStart !=_T("") && sDayEnd != _T("") ) {
			//strWhere = _T("CATALOG_DATE >= '") + sDayStart + _T("' AND CATALOG_DATE <= '") + sDayEnd + _T("'");
			strWhere = _T("PROCESSING_COMPLETE_DATE >= '") + sDayStart + _T("' AND PROCESSING_COMPLETE_DATE <= '") + sDayEnd + _T("'");			
		}
		//strWhere = _T("CATALOG_DATE >= '") + sDayStart +_T("'");
	}
	
	if ( sACQUISIT_YEAR != _T("") ) {
		if ( strWhere != _T("") ) {
			strWhere = _T("ACCESSION_REC_MAKE_YEAR = ") + sACQUISIT_YEAR + _T(" AND ") + strWhere;		
		}
		else {
			strWhere = _T("ACCESSION_REC_MAKE_YEAR = ") + sACQUISIT_YEAR + strWhere;
		}
	}
	if ( sACQUISIT_CODE != _T("") ) {
		if ( strWhere != _T("") ) {
			strWhere = _T("ACQUISIT_CODE = '") + sACQUISIT_CODE + _T("' AND ") + strWhere;	
		}
		else {
			strWhere = _T("ACQUISIT_CODE = '") + sACQUISIT_CODE + _T("' ") + strWhere;
		}
	}

	if (pDM->RefreshDataManager(strWhere) < 0)	return;
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("입수구분"), _T("수입구분"));	

	INT iRowCount = pDM->GetRowCount();
	if ( iRowCount < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}
	
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sFirstRegNo = pDM->GetCellData( _T("등록번호"), i );
		CString sLastRegNo = pDM->GetCellData( _T("마지막등록번호"), i );
		if ( sFirstRegNo != _T("") ) {
			CString sRegNo = sFirstRegNo + _T(" ~ ") + sLastRegNo;
			pDM->SetCellData( _T("등록번호"), sRegNo , i );
		}
	}

	/*
	for ( INT i = 0; i < iRowCount; i++) {
		CString sAccNum;
		//pDM->GetCellData( i, 6, sAccNum);
		pDM->GetCellData( _T("원부번호") , i, sAccNum);

		CString sCount;
		CString sQuery;
		
		//sQuery = _T("select COUNT(REC_KEY) from bo_book_tbl where ACCESSION_NO = ") + sAccNum;

		if ( sAccNum == _T("")) {
			CString sErrorMessage;
			sErrorMessage.Format( _T("%d번째 자료에 원부번호가 없습니다.") , i+1 );
			AfxMessageBox( sErrorMessage );
			//return;
			//sQuery = _T("select COUNT(REC_KEY) from bo_book_tbl where ACCESSION_NO = -1");
			sAccNum = _T("-1");
		}
		//책수 구하기
		sQuery = _T("select COUNT(REC_KEY) from bo_book_tbl where ACCESSION_NO = ") + sAccNum;
		pDM->GetOneValueQuery( sQuery , sCount);
		pDM->SetCellData( i, 17, sCount);
		
		//종수 구하기
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		sQuery = _T("select distinct species_key from bo_book_tbl where ACCESSION_NO = ") + sAccNum;
		dm.RestructDataManager(sQuery);
		sCount.Format( _T("%d"), dm.GetRowCount());
		pDM->SetCellData( i, 16, sCount);

		
		//등록번호 구하기
		CString sRegNum;
		CString sRegNum_start;
		CString sRegNum_end;
		sQuery = _T("select min(reg_no) from bo_book_tbl where ACCESSION_NO = ") + sAccNum;
		pDM->GetOneValueQuery( sQuery , sRegNum_start);
		sQuery = _T("select max(reg_no) from bo_book_tbl where ACCESSION_NO = ") + sAccNum;
		pDM->GetOneValueQuery( sQuery , sRegNum_end);

		sRegNum = sRegNum_start + _T(" ~ ") + sRegNum_end;

		pDM->SetCellData( i, 18, sRegNum);
		
	}
	*/

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_REG_0500"));
	pCM->AllControlDisplay();	

	
	

EFS_END
}

VOID CBO_CAT_REG_0500::OnbAccessionUpdate() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CBO_CAT_REG_0520 dlg(this);
	dlg.DoModal();	
	

EFS_END
}

VOID CBO_CAT_REG_0500::OnrACCESSIONNO() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CEdit * pEdit1;
	pEdit1=(CEdit *)GetDlgItem(IDC_eACCESSION_NO_START);
	pEdit1->EnableWindow(TRUE);
	pEdit1->SetFocus();

	CEdit * pEdit2;
	pEdit2=(CEdit *)GetDlgItem(IDC_eACCESSION_NO_END);
	pEdit2->EnableWindow(TRUE);

	CEdit * pEdit3;
	pEdit3=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_START);
	pEdit3->EnableWindow(FALSE);

	CEdit * pEdit4;
	pEdit4=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_END);
	pEdit4->EnableWindow(FALSE);
	

EFS_END
}

VOID CBO_CAT_REG_0500::OnrACCESSIONRECNO() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CEdit * pEdit1;
	pEdit1=(CEdit *)GetDlgItem(IDC_eACCESSION_NO_START);
	pEdit1->EnableWindow(TRUE);
	pEdit1->SetFocus();

	CEdit * pEdit2;
	pEdit2=(CEdit *)GetDlgItem(IDC_eACCESSION_NO_END);
	pEdit2->EnableWindow(TRUE);

	CEdit * pEdit3;
	pEdit3=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_START);
	pEdit3->EnableWindow(FALSE);

	CEdit * pEdit4;
	pEdit4=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_END);
	pEdit4->EnableWindow(FALSE);

EFS_END
}

VOID CBO_CAT_REG_0500::OnrACCESSIONDATE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CEdit * pEdit1;
	pEdit1=(CEdit *)GetDlgItem(IDC_eACCESSION_NO_START);
	pEdit1->EnableWindow(FALSE);
	//pEdit1->SetFocus();

	CEdit * pEdit2;
	pEdit2=(CEdit *)GetDlgItem(IDC_eACCESSION_NO_END);
	pEdit2->EnableWindow(FALSE);

	CEdit * pEdit3;
	pEdit3=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_START);
	pEdit3->EnableWindow(TRUE);
	pEdit3->SetFocus();

	CEdit * pEdit4;
	pEdit4=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_END);
	pEdit4->EnableWindow(TRUE);

EFS_END
}

VOID CBO_CAT_REG_0500::OnrACCESSIONDATE2() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CEdit * pEdit1;
	pEdit1=(CEdit *)GetDlgItem(IDC_eACCESSION_NO_START);
	pEdit1->EnableWindow(FALSE);
	

	CEdit * pEdit2;
	pEdit2=(CEdit *)GetDlgItem(IDC_eACCESSION_NO_END);
	pEdit2->EnableWindow(FALSE);

	CEdit * pEdit3;
	pEdit3=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_START);
	pEdit3->EnableWindow(TRUE);
	pEdit3->SetFocus();

	CEdit * pEdit4;
	pEdit4=(CEdit *)GetDlgItem(IDC_eACCESSION_DATE_END);
	pEdit4->EnableWindow(TRUE);

EFS_END
}

VOID CBO_CAT_REG_0500::OnbAccessionDelete() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	SelectMakeList(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
	INT idx = SelectGetTailPosition(_T("CM_BO_CAT_REG_0500"), _T("GRID"));
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
		return;
	}

	if (MessageBox(_T("선정한 원부를 삭제하시겠습니까?"), _T("삭제 확인"), MB_YESNO) == IDYES)
	{
		CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
		pDM->StartFrame();

		CString sAccRecKey, sAccRecKeyTmp;
		do
		{
			CString strCondition;
			pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

			sAccRecKeyTmp += _T(",") + strCondition;		//원부키 조합

			pDM->MakeDeleteFrame(_T("CO_ACCESSION_REC_TBL"), _T("REC_KEY"), _T("STRING"), strCondition);
			pDM->DeleteRow(idx);

			idx = SelectGetPrev(_T("CM_BO_CAT_REG_0500"), _T("GRID"));			
		}		
		while (idx > 0);

		sAccRecKey = sAccRecKeyTmp.Mid(1);
		
		CString sQuery;
		sQuery.Format( _T("DELETE FROM BO_BOOK_TBL WHERE ACCESSION_REC_KEY IN (%s);") , sAccRecKey );
		INT iCh = pDM->AddFrame(sQuery);
		
		INT ids = pDM->SendFrame();
		if (ids < 0) {
			AfxMessageBox(_T("삭제사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
			return;
		}
		pDM->EndFrame();
	}
	

	AllControlDisplay(_T("CM_BO_CAT_REG_0500"));
	

EFS_END
}

VOID CBO_CAT_REG_0500::OnCancel() 
{
EFS_BEGIN

	// TODO: Add extra cleanup here
	DestroyWindow();	
	//CDialog::OnCancel();

EFS_END
}

VOID CBO_CAT_REG_0500::OnbCat0500PRINT() 
{
EFS_BEGIN
/*
	// TODO: Add your control notification handler code here
	pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
	if ( pDM == NULL ) return;
	if ( pDM->GetRowCount() < 1 ) {
		return;
	}
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return;
	}
	INT ids;
	pSIReportManager->SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("정리_등록원부관리대장"));		
	
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return;
	}
	
	//CString sLibraryName;
	//GetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("도서관명"), sLibraryName );
	//pSIReportManager->SetTempVariable(_T("%BARCODE바코드제목%"), sLibraryName);

	pSIReportManager->SetDataMgr(0, pDM);
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}
*/
EFS_END
}

/*
INT CBO_CAT_REG_0500::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{

EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

EFS_END
return -1;

}
*/