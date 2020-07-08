// BO_CAT_REG_0400.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_REG_0400.h"
#include "BO_REG_MGR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0400 dialog


CBO_CAT_REG_0400::CBO_CAT_REG_0400(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_0400)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_REG_0400::~CBO_CAT_REG_0400()
{	
	
} 

VOID CBO_CAT_REG_0400::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_0400)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_0400, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_0400)
	ON_BN_CLICKED(IDC_bACCESSIONNO_GIVE, OnbACCESSIONNOGIVE)
	ON_BN_CLICKED(IDC_rCanRegNo, OnrCanRegNo)
	ON_BN_CLICKED(IDC_rAcqBack, OnrAccCan)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0400 message handlers

BOOL CBO_CAT_REG_0400::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_REG_0400")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	if(OpenMode == 0)
	{
		m_DM_ALIAS = _T("DM_단행_정리_등록_메인");
		m_CM_ALIAS = _T("CM_단행_정리_등록_메인");
	}
	else if(OpenMode == 2)
	{
		m_DM_ALIAS = _T("DM_단행_정리_원부_메인"); 
		m_CM_ALIAS = _T("CM_단행_정리_원부_메인");
	}
	else 
	{
		AfxMessageBox(_T("OpenMode Error"));
		return FALSE;

	}



	INT iReturn = Display(0);

	//선택이 안되어 있으면 화면을 띄우지 않는다.
	if ( iReturn == -1 ) {
		m_bInitDlg = FALSE;
		return FALSE;
	}

	CButton * pCanRegNo;
	pCanRegNo=(CButton *)GetDlgItem(IDC_rCanRegNo);
	pCanRegNo->SetCheck(1);	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

BOOL CBO_CAT_REG_0400::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

INT CBO_CAT_REG_0400::Display(INT nDir)
{
EFS_BEGIN

	INT idx;
	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));

	if(idx < 0)
	{
		//에러처리
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;
	}	

	CESL_DataMgr *pDM = FindDM(m_DM_ALIAS);
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_단행_정리_등록_메인") );
		return -1;
	}

	CString BookKey,strStatus;
	strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

	if (strStatus != _T("BOR112N")) //등록자료 
	{
		//에러처리
		ESLAfxMessageBox(_T("등록 자료만 처리 할 수 있습니다."));
		return -1;
	}

	idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

		if (strStatus != _T("BOR112N")) //등록자료 
		{
			//에러처리
			ESLAfxMessageBox(_T("등록 자료만 처리 할 수 있습니다."));
			return -1;
		}

		idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	}

	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_REG_0400::OnbACCESSIONNOGIVE() 
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
EFS_BEGIN


	if(OpenMode == 0)
	{
		CancelRegNo();
	}
	else if(OpenMode == 2)
	{
		CancelRegNo();
	}




EFS_END
}

VOID CBO_CAT_REG_0400::OnCancel() 
{
EFS_BEGIN

	// TODO: Add extra cleanup here
	DestroyWindow();		
	//CDialog::OnCancel();

EFS_END
}

VOID CBO_CAT_REG_0400::OnrCanRegNo() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CButton * chkRegCan;
	chkRegCan=(CButton *)GetDlgItem(IDC_rCanRegNo);
	chkRegCan->SetCheck(1);

EFS_END
}


VOID CBO_CAT_REG_0400::OnrAccCan() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CButton * chkRegCan;
	chkRegCan=(CButton *)GetDlgItem(IDC_rAcqBack);
	chkRegCan->SetCheck(1);

EFS_END
}



INT CBO_CAT_REG_0400::CancelRegNo()
{	
	//등록번호 취소
	CButton *pCanRegNo;
	pCanRegNo = (CButton *)GetDlgItem(IDC_rCanRegNo);

	//수서 복귀 
	CButton *pAcqBack;
	pAcqBack = (CButton *)GetDlgItem(IDC_rAcqBack);
	

	INT iCh;
	CESL_DataMgr *pDM;	
	pDM = FindDM(m_DM_ALIAS);		

	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return -1;

	//누락등록번호 추가 변수
	INT iBookCount = -1; 
	CESL_DataMgr DM_BOOK;


	if ( pCanRegNo->GetCheck() == 1 ) {	//등록 번호 취소


		INT ids;// = 6;

		ids = ::MessageBox( GetSafeHwnd() , _T("선정하신 자료의 등록번호를 삭제하시겠습니까?") ,
			_T("삭 제") ,
			MB_YESNO | MB_ICONQUESTION );
	
		if ( ids == IDYES ) {		

			DeleteRegNo();

		}
		else {//선택취소
			return -1;
		}

	}
	else if ( pAcqBack->GetCheck() == 1 ) //수서 복귀
	{

		INT ids;// = 6;

		ids = ::MessageBox( GetSafeHwnd() , _T("선정하신 자료의 등록번호를 삭제하고 수서 복귀 하시겠습니까?") ,
			_T("삭 제") ,
			MB_YESNO | MB_ICONQUESTION );
	
		if ( ids == IDYES ) {	
			

/* 수정 사항 : 수서복귀시 수입구분이 없으면 return 되도록 수정 +++
 * 수정 날짜 : 2003.10.17
 * 수정 사람 : 김홍수 
 */
			
			INT idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));
			CString strRegCode;
			pDM->GetCellData(_T("BB_수입구분"), idx, strRegCode);
			if (strRegCode == _T(""))
			{
				AfxMessageBox(_T("수입구분이 없습니다"));
				return -1;
			}
// 수정 사항 : 수서복귀시 등록구분이 없으면 return 되도록 수정 ---

			DeleteRegNo();
			BackAcq();

		}
		else {//선택취소
			return -1;
		}

	}
	
	iCh = pDM->SendFrame();
	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return -5;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return -1;

	DestroyWindow();	
	
	return 0;
}

INT CBO_CAT_REG_0400::DeleteRegNo()
{

	INT iCh;	
	CString sQuery;

	CESL_DataMgr *pDM;	
	pDM = FindDM(m_DM_ALIAS);		

	CESL_ControlMgr *pCM;
	pCM = FindCM(m_CM_ALIAS);		
	
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	
	// 등록 번호를 선택하여 누락등록 번호에 넣는다. 
	INT idx;
	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));
	
	CString strRegNo,strRegCode,strAccNo,sRecKey;
	strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );
	
	strRegNo.TrimLeft();
	strRegNo.TrimRight();

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	
	if (strRegNo != _T("")) //등록자료 
	{
		strRegCode = pDM->GetCellData( _T("BB_등록구분_코드") , idx );
		strAccNo = pDM->GetCellData( _T("원부번호") , idx );
		sRecKey = pDM->GetCellData( _T("BB_책KEY") , idx );
		
		// 등록 번호 제거
		sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL , WORKING_STATUS = 'BOR111N' WHERE REC_KEY = %s;") , sRecKey );
		iCh = pDM->AddFrame( sQuery );	
		if ( 0 > iCh ) return -1;
		
		//누락 등록 테이블에 있는 지 조사 
		CString sQuery,sCount;
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%s AND MANAGE_CODE = UDF_MANAGE_CODE") , strRegCode , strRegNo.Mid(2,10) );
		sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%s AND MANAGE_CODE = UDF_MANAGE_CODE") , strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength ) );
		//=====================================================
		pDM->GetOneValueQuery( sQuery , sCount );
		//없으면 입력 
		if ( sCount == _T("0") ) {
			CString sRecKey;
			INT ids = pDM->MakeRecKey(sRecKey);
			if ( 0 > ids ) return -1;
			//=====================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 			sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, FIRST_WORK, LAST_WORK ) VALUES ( %s, '%s', '%s', '%s', %s, '%s', '%s' );") , sRecKey , _T("MO") , _T("") , strRegCode , strRegNo.Mid(2,10), GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG), GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG) );
			sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, FIRST_WORK, LAST_WORK ) VALUES ( %s, '%s', '%s', '%s', %s, '%s', '%s' );") , sRecKey , _T("MO") , _T("") , strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength ), GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG), GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG) );
			//=====================================================
			
			iCh = pDM->AddFrame( sQuery );
			if ( 0 > iCh ) return -1;			
		}
		
		// DM 바로 반영 해준다 
		pDM->SetCellData( _T("원부번호") ,_T(""), idx );
		pDM->SetCellData( _T("BB_등록번호") ,_T(""), idx );
		pDM->SetCellData( _T("BB_자료상태_코드") ,_T("BOR111N"), idx );

		// 해당 그리드 반영 
		pGrid->DisplayLine(idx);
		
	}
	
	idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	
	
	while ( idx != -1 ) {
		strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );
		
		if (strRegNo != _T("")) //등록자료 
		{
			strRegCode = pDM->GetCellData( _T("BB_등록구분_코드") , idx );
			strAccNo = pDM->GetCellData( _T("원부번호") , idx );
			sRecKey = pDM->GetCellData( _T("BB_책KEY") , idx );

			// 등록 번호 제거
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL , WORKING_STATUS = 'BOR111N' WHERE REC_KEY = %s;") , sRecKey );
			iCh = pDM->AddFrame( sQuery );	
			if ( 0 > iCh ) return -1;

			//누락 등록 테이블에 있는 지 조사 
			CString sQuery,sCount;
			//=====================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 			sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%s AND MANAGE_CODE = UDF_MANAGE_CODE") , strRegCode , strRegNo.Mid(2,10));
			sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%s AND MANAGE_CODE = UDF_MANAGE_CODE") , strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength ));
			//=====================================================
			
			pDM->GetOneValueQuery( sQuery , sCount );
			//없으면 입력 
			if ( sCount == _T("0") ) {
				CString sRecKey;
				INT ids = pDM->MakeRecKey(sRecKey);
				if ( 0 > ids ) return -1;
			
				//=====================================================
				//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
//	 			sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, FIRST_WORK, LAST_WORK ) VALUES ( %s, '%s', '%s', '%s', %s, '%s', '%s' );") , sRecKey , _T("MO") , _T("") , strRegCode , strRegNo.Mid(2,10), GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG), GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG) );
				sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, FIRST_WORK, LAST_WORK ) VALUES ( %s, '%s', '%s', '%s', %s, '%s', '%s' );") , sRecKey , _T("MO") , _T("") , strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength ), GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG), GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG) );
				//=====================================================
				
				iCh = pDM->AddFrame( sQuery );		
				if ( 0 > iCh ) return -1;				
			}		
			
			pDM->SetCellData( _T("원부번호") ,_T(""), idx );
			pDM->SetCellData( _T("BB_등록번호") ,_T(""), idx );
			pDM->SetCellData( _T("BB_자료상태_코드") ,_T("BOR111N"), idx );

			// 해당 그리드 반영 
			pGrid->DisplayLine(idx);
			
		}
		
		idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	}
	
	
	return 0;

}

INT CBO_CAT_REG_0400::BackAcq()
{

	
	INT iCh;	
	CString sQuery;

	CESL_DataMgr *pDM;	
	pDM = FindDM(m_DM_ALIAS);		

	CESL_ControlMgr *pCM;
	pCM = FindCM(m_CM_ALIAS);		
	
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	
	// 등록 번호를 선택하여 누락등록 번호에 넣는다. 
	INT idx,Dm_index=0;
	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));
	
	CString AcqCode,sRecKey;
	AcqCode = pDM->GetCellData( _T("BB_수입구분") , idx );
	sRecKey = pDM->GetCellData( _T("BB_책KEY") , idx );
	
	AcqCode.TrimLeft();
	AcqCode.TrimRight();
	
	if (AcqCode == _T("")) //등록자료 
	{
		AfxMessageBox(_T("수입구분이 없습니다. 복귀 할수 없습니다.!")); 
		return 0;
	} 
	else if ( AcqCode == _T("1")) // 구입자료 
	{
		// 검수취소일 경우, 미납자료로 상태복귀 [2003/09/30/박현석]
		if (CancelType == 1)
		{
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET DELIVERY_YN ='N', LAST_WORK='%s', WORKING_STATUS = 'BOA113O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG), sRecKey );
			iCh = pDM->AddFrame( sQuery );	
			if ( 0 > iCh ) return -1;
		}
		else 
		{
			// 구입 인계 대상자료 복귀d
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = 'BOA114O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG), sRecKey );
			iCh = pDM->AddFrame( sQuery );
			if ( 0 > iCh ) return -1;
		}

	}
	else if ( AcqCode == _T("2")) // 기증자료 
	{
		// 기증 인계 대상자료 복귀
		sQuery.Format( _T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = 'BOA211O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG), sRecKey );
		iCh = pDM->AddFrame( sQuery );	
		if ( 0 > iCh ) return -1;
	}
		
	// DM 바로 반영 해준다 
	pDM->DeleteRow(idx);
	Dm_index++;		
	
	idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	
	
	while ( idx != -1 ) {
		AcqCode = pDM->GetCellData( _T("BB_수입구분") , idx - Dm_index);
		sRecKey = pDM->GetCellData( _T("BB_책KEY") , idx - Dm_index);
		
		AcqCode.TrimLeft();
		AcqCode.TrimRight();
		
		if (AcqCode == _T("")) //등록자료 
		{
			AfxMessageBox(_T("수입구분이 없습니다. 복귀 할수 없습니다.!")); 
		} 
		else if ( AcqCode == _T("1")) // 구입자료 
		{
			if (CancelType == 1)
			{
				sQuery.Format( _T("UPDATE BO_BOOK_TBL SET DELIVERY_YN ='N', LAST_WORK='%s', WORKING_STATUS = 'BOA113O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG), sRecKey );
				iCh = pDM->AddFrame( sQuery );	
				if ( 0 > iCh ) return -1;
			}
			else 
			{
				// 구입 인계 대상자료 복귀
				sQuery.Format( _T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = 'BOA114O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG), sRecKey );
				iCh = pDM->AddFrame( sQuery );
				if ( 0 > iCh ) return -1;
			}
			
		}
		else if ( AcqCode == _T("2")) // 기증자료 
		{
			// 기증 인계 대상자료 복귀
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = 'BOA211O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG), sRecKey );
			iCh = pDM->AddFrame( sQuery );	
			if ( 0 > iCh ) return -1;			
		}
		
		// DM 바로 반영 해준다 
		pDM->DeleteRow(idx - Dm_index);
		Dm_index++;		
		
		idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	}
	
	pGrid->Display();


	return 0;
}


HBRUSH CBO_CAT_REG_0400::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
