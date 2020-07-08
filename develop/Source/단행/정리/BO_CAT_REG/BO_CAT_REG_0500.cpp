// BO_CAT_REG_0500.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "..\..\..\단행\정리\BO_CAT_REG\BO_CAT_REG_0500.h"
#include "..\..\..\단행\정리\BO_CAT_REG\BO_CAT_REG_0510.h"
#include "..\..\..\단행\정리\BO_CAT_REG\BO_CAT_REG_0520.h"
#include "..\..\..\ESL\SIReportMgr\SIReportManager.h"

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
	m_pDM = NULL;
}

CBO_CAT_REG_0500::~CBO_CAT_REG_0500()
{
	
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
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0500 message handlers

BOOL CBO_CAT_REG_0500::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
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
	
	// KOL.RED.2018.220 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//번호까지|E|1120|||||||
	//번호부터|E|1121|||||||
	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_CAT_REG_0500"), _T("번호부터"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_CAT_REG_0500"), _T("번호까지"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}
	// KOL.RED.2018.220 ---------------------------------------------------------------------------	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  

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

	CBO_CAT_REG_0510 dlg(this);
	dlg.DoModal();	

EFS_END
}

VOID CBO_CAT_REG_0500::OnbSEARCH() 
{
EFS_BEGIN

	//CESL_DataMgr *m_pDM;
	m_pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
	if( m_pDM == NULL ) return;

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

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	// 등록번호
	if( pRegNo->GetCheck()==1 )
	{
		CString sRight;
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

		if( sAccessionNumStart.IsEmpty() == TRUE && sAccessionNumEnd.IsEmpty() == FALSE )
		{
			AfxMessageBox ( _T("시작 등록번호를 입력하세요.") );
			return;
		}
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		if( sAccessionNumStart.IsEmpty() == FALSE && sAccessionNumStart.GetLength() < 12 )
// 		{
// 			if( sAccessionNumStart.GetLength() > 2 )
// 			{				
// 				sRight.Format( _T("%010s") , sAccessionNumStart.Mid(2) );
// 				sAccessionNumStart = sAccessionNumStart.Left(2) + sRight;
// 			}
// 		}
// 		if( sAccessionNumEnd.IsEmpty() == FALSE && sAccessionNumEnd.GetLength() < 12 )
// 		{
// 			if( sAccessionNumEnd.GetLength() > 2 )
// 			{
// 				sRight.Format( _T("%010s") , sAccessionNumEnd.Mid(2) );
// 				sAccessionNumEnd = sAccessionNumEnd.Left(2) + sRight;
// 			}
// 		}

		if( sAccessionNumStart.IsEmpty() == FALSE && sAccessionNumStart.GetLength() < 12 )
		{
			if( sAccessionNumStart.GetLength() > nRegCodeLength )
			{				
				sAccessionNumStart = m_pInfo->MakeRegNo(sAccessionNumStart.Left(nRegCodeLength),  sAccessionNumStart.Mid(nRegCodeLength) );
			}
		}
		if( sAccessionNumEnd.IsEmpty() == FALSE && sAccessionNumEnd.GetLength() < 12 )
		{
			if( sAccessionNumEnd.GetLength() > nRegCodeLength )
			{
				sAccessionNumEnd = m_pInfo->MakeRegNo(sAccessionNumEnd.Left(nRegCodeLength), sAccessionNumEnd.Mid(nRegCodeLength) );
			}
		}
		//=====================================================
		
		

		if( sAccessionNumStart.IsEmpty() == FALSE && sAccessionNumEnd.IsEmpty() == TRUE )
		{
			//sQuery = _T("SELECT DISTINCT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE REG_NO = '") + sAccessionNumStart + _T("'");
			strWhere = _T("REC_KEY IN (")
					_T("SELECT DISTINCT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE REG_NO = '") + sAccessionNumStart + _T("') ")
					_T("OR REC_KEY IN (")
					_T("SELECT DISTINCT ACCESSION_REC_KEY FROM SE_BOOK_TBL WHERE REG_NO = '") + sAccessionNumStart + _T("')");
		}
		if( sAccessionNumStart.IsEmpty() == FALSE && sAccessionNumEnd.IsEmpty() == FALSE )
		{
			//sQuery = _T("SELECT DISTINCT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE REG_NO >= '") + sAccessionNumStart + _T("' AND REG_NO <= '") + sAccessionNumEnd + _T("'");
			strWhere = _T("REC_KEY IN (")
				_T("SELECT DISTINCT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE REG_NO >= '") + sAccessionNumStart + _T("' AND REG_NO <= '") + sAccessionNumEnd + _T("') ")
				_T("OR REC_KEY IN (")
				_T("SELECT DISTINCT ACCESSION_REC_KEY FROM SE_BOOK_TBL WHERE REG_NO >= '") + sAccessionNumStart + _T("' AND REG_NO <= '") + sAccessionNumEnd + _T("')");
		}
		}
	//원부번호
	else if(pAccNo->GetCheck()==1)
	{		
		
		if ( sAccessionNumStart != _T("") || sAccessionNumEnd !=_T("") ) {
			if ( (sAccessionNumStart >= _T("0") && sAccessionNumStart <= _T("99999")) 
				|| (sAccessionNumEnd >= _T("0") && sAccessionNumEnd <= _T("99999")) ) {
			}
			else {
				AfxMessageBox (_T("원부번호는 숫자를 입력하세요"));
				return;
			}
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
	// 등록일자 검색
	else if(pRegDay->GetCheck()==1)
	{		
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
	// 완료일자
	else if(pComDay->GetCheck()==1)
	{
		if ( sDayStart == _T("") && sDayEnd != _T("") ) {
			AfxMessageBox( _T("시작 완료일자를 입력하세요.") );
			return;
		}
		else if ( sDayStart != _T("") && sDayEnd == _T("") )
			strWhere = _T("PROCESSING_COMPLETE_DATE = '") + sDayStart + _T("'");

		else if ( sDayStart !=_T("") && sDayEnd != _T("") )
			strWhere = _T("PROCESSING_COMPLETE_DATE >= '") + sDayStart + _T("' AND PROCESSING_COMPLETE_DATE <= '") + sDayEnd + _T("'");			
	}
	
	if( sACQUISIT_YEAR.IsEmpty() == FALSE ) 
	{
		if( strWhere.IsEmpty() == FALSE )	strWhere = _T("ACCESSION_REC_MAKE_YEAR = ") + sACQUISIT_YEAR + _T(" AND ") + strWhere;		
		else								strWhere = _T("ACCESSION_REC_MAKE_YEAR = ") + sACQUISIT_YEAR;
	}
	if( sACQUISIT_CODE.IsEmpty() == FALSE ) 
	{
		if ( strWhere.IsEmpty() == FALSE )	strWhere = _T("ACQUISIT_CODE = '") + sACQUISIT_CODE + _T("' AND ") + strWhere;	
		else strWhere = _T("ACQUISIT_CODE = '") + sACQUISIT_CODE + _T("' ");
	}
	if ( strWhere.IsEmpty() == FALSE )	
	{
		strWhere = _T("MANAGE_CODE = UDF_MANAGE_CODE AND ") + strWhere;	
	}
	else
	{
		strWhere = _T("MANAGE_CODE = UDF_MANAGE_CODE ");
	}

	if( m_pDM->RefreshDataManager(strWhere) < 0 )	return;
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM, _T("입수구분"), _T("수입구분"));	

	INT iRowCount = m_pDM->GetRowCount();
	if( iRowCount < 1 ) 
	{
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}
	
	for ( INT i = 0 ; i < iRowCount ; i++ ) 
	{
		CString sFirstRegNo = m_pDM->GetCellData( _T("등록번호"), i );
		CString sLastRegNo = m_pDM->GetCellData( _T("마지막등록번호"), i );
		if ( sFirstRegNo != _T("") ) {
			CString sRegNo = sFirstRegNo + _T(" ~ ") + sLastRegNo;
			m_pDM->SetCellData( _T("등록번호"), sRegNo , i );
		}
	}

	// 040310_HSS  셀별로 변경값 넣어주기 (등록번호, 종수, 책수) +++++++++++++++++++++++
	CESL_DataMgr DM_BOOK;
	CString sAccRecKey;
	CString sQuery, sRegnoRange;
	INT nCnt1, nCnt2;
	CString sData1, sData2, sPutData;

	for ( i = 0 ; i < m_pDM->GetRowCount() ; i++ )
	{
		// 종수 - 단행과 연속 합쳐주기
		sData1 = m_pDM->GetCellData( _T("종수"), i );
		sData2 = m_pDM->GetCellData( _T("종수_연속"), i );

		sPutData.Format(_T("%d"), _ttoi(sData1)+_ttoi(sData2));
		m_pDM->SetCellData( _T("종수"), sPutData , i );

		// 책수 - 단행과 연속 합쳐주기
		sData1 = m_pDM->GetCellData( _T("책수"), i );
		sData2 = m_pDM->GetCellData( _T("책수_연속"), i );

		sPutData.Format(_T("%d"), _ttoi(sData1)+_ttoi(sData2));
		m_pDM->SetCellData( _T("책수"), sPutData , i );

		// DB에서 Select하여 등록번호 범위 넣어주기 
		sAccRecKey = m_pDM->GetCellData( _T("REC_KEY"), i );
		DM_BOOK.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
		sRegnoRange = _T("");
		
		// 단행 Select (단행 책수가 1 이상인 경우)
		if (_ttoi(sData1) > 0)
		{
			sQuery.Format( _T("SELECT MIN (reg_no) || ' ~ ' || MAX (reg_no) ")
				_T("FROM bo_book_tbl b, co_accession_rec_tbl a ")
				_T("WHERE a.rec_key = b.accession_rec_key AND a.rec_key = %s ")
				_T("GROUP BY b.reg_code"), sAccRecKey );
			DM_BOOK.RestructDataManager( sQuery );
			
			nCnt1 = DM_BOOK.GetRowCount();		
			if ( nCnt1 > 0 ) {	
				for ( INT j = 0 ; j < nCnt1 ; j++ )
				{
					if (sRegnoRange.GetLength() > 0) sRegnoRange += _T("\n");
					sRegnoRange +=  DM_BOOK.GetCellData( j, 0 );
				}
			}
		}

		// 연속 Select (연속 책수가 1 이상인 경우)
		if (_ttoi(sData2) > 0)
		{
			sQuery.Format( _T("SELECT MIN (reg_no) || ' ~ ' || MAX (reg_no) ")
				_T("FROM se_book_tbl b, co_accession_rec_tbl a ")
				_T("WHERE a.rec_key = b.accession_rec_key AND a.rec_key = %s ")
				_T("GROUP BY b.reg_code"), sAccRecKey );
			DM_BOOK.RestructDataManager( sQuery );
			
			nCnt2 = DM_BOOK.GetRowCount();
			if ( nCnt2 > 0 ) {	
				for ( INT j = 0 ; j < nCnt2 ; j++ )
				{
					if (sRegnoRange.GetLength() > 0) sRegnoRange += _T("\n");
					sRegnoRange +=  DM_BOOK.GetCellData( j, 0 );
				}
			}
		}

		m_pDM->SetCellData( _T("등록번호"), sRegnoRange , i );
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_REG_0500"));
	pCM->AllControlDisplay();	
	SetGridHeight();
	// 040310_HSS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

EFS_END
}

INT CBO_CAT_REG_0500::SetGridHeight() 
{
	INT nHeight, ids;
	CString strTmp;

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_REG_0500"));
	CESL_Grid * pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM->FindControl(_T("GRID"));

	for ( INT i = 0 ; i < m_pDM->GetRowCount() ; i++ ) 
	{
		strTmp = m_pDM->GetCellData( _T("등록번호"), i );
		nHeight = 1;
		while ( strTmp.Find(_T("\n"),0) != -1 )
		{
			nHeight++;
			ids = strTmp.Find(_T("\n"),0);
			strTmp = strTmp.Mid(ids+1);
		}
		pGrid_sub->SetRowHeight( i+1, 200 * nHeight );
	}

	return 0;
}

VOID CBO_CAT_REG_0500::OnbAccessionUpdate() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CBO_CAT_REG_0520 dlg(this);
	//dlg.DoModal();
	if ( dlg.DoModal() == IDOK ) {
		OnbSEARCH();
	}

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
		m_pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
		INT ids = m_pDM->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return;
		}

		CString sAccRecKey, sAccRecKeyTmp;
		do
		{
			CString strCondition;
			m_pDM->GetCellData(_T("REC_KEY"), idx, strCondition);

			sAccRecKeyTmp += _T(",") + strCondition;		//원부키 조합

			m_pDM->DeleteRow(idx);

			idx = SelectGetPrev(_T("CM_BO_CAT_REG_0500"), _T("GRID"));			
		}
		while (idx > 0);

		sAccRecKey = sAccRecKeyTmp.Mid(1);

		// 040225_HSS_추가 : 원부 삭제 한번에 하기	++++++++++++++++++++++++++++++
		CString sQuery;
		sQuery.Format( _T("DELETE FROM CO_ACCESSION_REC_TBL WHERE REC_KEY IN (%s);") , sAccRecKey );
		INT iCh = m_pDM->AddFrame(sQuery);
		if ( 0 > iCh ) return;
		// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		ids = m_pDM->SendFrame();
		if (ids < 0) {
			AfxMessageBox(_T("삭제사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
			return;
		}
		ids = m_pDM->EndFrame();
		if (ids < 0) {
			AfxMessageBox(_T("삭제사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return;
		}
	}
	
	AllControlDisplay(_T("CM_BO_CAT_REG_0500"));
	SetGridHeight();
	
EFS_END
}

VOID CBO_CAT_REG_0500::OnbCat0500PRINT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	m_pDM = FindDM(_T("DM_BO_CAT_REG_0500"));
	if ( m_pDM == NULL ) return;
	if ( m_pDM->GetRowCount() < 1 ) {
		return;
	}
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
	if (!pSIReportManager) {
		AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return;
	}
	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("정리_등록원부관리대장"));		
	
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return;
	}
	
	//종수, 책수 구하기
	INT iSpeciesCount = 0;
	INT iBookCount = 0;
	CString sSpeciesCount;
	CString sBookCount;
	for ( INT i = 0 ; i < m_pDM->GetRowCount() ; i++ ) {
		sSpeciesCount = m_pDM->GetCellData( _T("종수"), i );
		iSpeciesCount = iSpeciesCount + _ttoi(sSpeciesCount);
		sBookCount = m_pDM->GetCellData( _T("책수"), i );
		iBookCount = iBookCount + _ttoi(sBookCount);
	}
	sSpeciesCount.Format( _T("%d"), iSpeciesCount );
	sBookCount.Format( _T("%d"), iBookCount );
	CString sTotal;
	sTotal = _T("   합 계 :   종수 ") + sSpeciesCount + _T("   책수 ") + sBookCount;
	pSIReportManager->SetTempVariable(_T("합계"), sTotal);

	//CString sLibraryName;
	//GetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("도서관명"), sLibraryName );
	//pSIReportManager->SetTempVariable(_T("%BARCODE바코드제목%"), sLibraryName);

	pSIReportManager->SetDataMgr(0, m_pDM);
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}

EFS_END
}


INT CBO_CAT_REG_0500::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{

EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

EFS_END
return -1;

}

HBRUSH CBO_CAT_REG_0500::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

