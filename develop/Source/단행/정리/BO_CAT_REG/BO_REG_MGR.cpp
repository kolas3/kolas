// BO_REG_MGR.cpp: implementation of the BO_REG_MGR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_REG_MGR.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "BO_CAT_REG_0400.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BO_REG_MGR::BO_REG_MGR(CESL_Mgr* pParent) : CSPF_Work(pParent)
{

}

BO_REG_MGR::~BO_REG_MGR()
{

}
INT BO_REG_MGR::SPFExecutePath(CString strPath)
{
EFS_BEGIN

	CSPF_Work::SPFExecutePath(strPath);
	
	INT ids=0;
	
	if(strPath.GetLength() == 0) return -1;

	if (strPath == _T("등록메인취소")){
		AccessionNoGive();
	}

	if (strPath == _T("등록구분변경")){
		ChangeCode();
	}

	if (strPath == _T("자료복귀")){
		RollBackData();
	}




EFS_END
return -1;

}

INT BO_REG_MGR::ChangeCode()
{

	INT idx;
	CString BookKey,strQry,sTmp,sReg;
	
	CESL_ControlMgr *pCM;
	pCM = m_pParentMgr->FindCM(_T("CM_단행_정리_등록_메인"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_단행_정리_등록_메인 error") );
		return -2;
	}
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_단행_정리_등록_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_단행_정리_등록_메인") );
		return -1;
	}

	// 변경할 등록 구분 코드 가져오기 
	m_pParentMgr->GetControlData(_T("CM_단행_정리_등록_코드"), _T("BB_등록구분"), sReg);


	// 대상 책키 가저 오기 
	m_pParentMgr->SelectMakeList(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	idx = m_pParentMgr->SelectGetHeadPosition(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	BookKey = pDM->GetCellData( _T("BB_책KEY") , idx );

	// 변경 spl 실행
	INT ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REG_CODE"), _T("STRING"), sReg );
	ids = pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);
	if ( 0 > ids ) return -1;
	ids = pDM->SendFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("SendFrame 를 확인하세요"));
		return -1;
	}

	pDM->SetCellData(_T("BB_등록구분_코드"), sReg, idx);
	pGrid->DisplayLine(idx);
	
	idx = m_pParentMgr->SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		BookKey = pDM->GetCellData( _T("BB_책KEY") , idx );

		ids = pDM->StartFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return -1;
		}

		pDM->InitDBFieldData();
		pDM->AddDBFieldData(_T("REG_CODE"), _T("STRING"), sReg );
		ids = pDM->MakeUpdateFrame(_T("BO_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), BookKey, idx);
		if ( 0 > ids ) return -1;
		ids = pDM->SendFrame();
		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("SendFrame 를 확인하세요"));
			return -1;
		}


		pDM->SetCellData(_T("BB_등록구분_코드"), sReg, idx);
		pGrid->DisplayLine(idx);
		idx = m_pParentMgr->SelectGetNext(_T("CM_단행_정리_등록_메인"), _T("MAIN_GRID"));
	}

	AfxMessageBox(_T("변경 되었습니다."));



//	pCM->AllControlDisplay();

	pGrid->ReshowSelectedRows();

	return 0;	

}
VOID BO_REG_MGR::RollBackData() {
	
EFS_BEGIN

	CBO_CAT_REG_0400 *pParent = (CBO_CAT_REG_0400*)m_pParentMgr;
	
	CButton *pAccCan;
	pAccCan = (CButton *)pParent->GetDlgItem(IDC_rAccCan);
	CButton *pRegCan;
	pRegCan = (CButton *)pParent->GetDlgItem(IDC_rRegCan);
	CButton *pAcqBack;
	pAcqBack = (CButton *)pParent->GetDlgItem(IDC_rAcqBack);	
	CButton *pChkRegCan;
	pChkRegCan = (CButton *)pParent->GetDlgItem(IDC_chkRegCan);
	CString sQuery;

	CESL_DataMgr *pDM;	
	pDM = pParent->FindDM(_T("DM_단행_정리_원부_메인"));		


	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return;

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = pParent->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//누락등록번호 추가 변수
	INT iBookCount = -1; 
	CESL_DataMgr DM_BOOK;

	if ( pAccCan->GetCheck() == 1 ) {	//원부번호 삭제
		//원부번호 부여 체크
		//CString sQueryCount = _T("select count(accession_rec_key) from BO_BOOK_TBL WHERE species_key in(") + pParent->sRecKey + _T(")");
		CString sQueryCount = _T("select count(accession_rec_key) from BO_BOOK_TBL WHERE rec_key in(") + pParent->sRecKey + _T(")");
		CString sCount;	//시작 등록번호
		pDM->GetOneValueQuery( sQueryCount , sCount );
		if ( sCount == _T("0") ) {
			AfxMessageBox (_T("원부번호 부여된 자료가 없습니다."));
			//m_bInitDlg = FALSE;
			return;
		}

		INT ids;

/*		
		ids = ::MessageBox( pParent->GetSafeHwnd() , _T("선정하신 자료의 원부번호를 삭제하시겠습니까?") ,
			_T("삭 제") ,
			MB_YESNO | MB_ICONQUESTION );
*/
		
		ids = MessageBox( m_pParentMgr->m_hWnd , _T("선정하신 자료의 원부번호를 삭제하시겠습니까?") ,
			_T("삭 제") ,
			MB_YESNO | MB_ICONQUESTION );
		if ( ids == IDYES ) {
			CString sQuery;
			//원부 테이블 삭제
			//sQuery.Format( _T("DELETE FROM CO_ACCESSION_REC_TBL WHERE REC_KEY IN (SELECT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY IN (%s) );") , pParent->sRecKey );
			//iCh = pDM->AddFrame( sQuery );

			//sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL WHERE SPECIES_KEY IN (%s);") , pParent->sRecKey );
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL WHERE REC_KEY IN (%s);") , pParent->sRecKey );
			iCh = pDM->AddFrame( sQuery );
			if ( 0 > iCh ) return;

			//그리드에 원부 삭제하기
			for ( INT j = 1 ; j <= pParent->ArrSelectCount.GetSize() ; j ++ ) {				
				//pDM->SetCellData( _T("BB_등록번호") , _T("") , pParent->ArrSelectCount.GetAt(j-1) );
				pDM->SetCellData( _T("원부번호") , _T("") , pParent->ArrSelectCount.GetAt(j-1) );

				//pDM->DeleteRow(ArrSelectCount.GetAt(j-1));
			}

		}
		else {//선택취소
			return;
		}
	}
	//else if ( pRegCan->GetCheck() == 1 && pAccCan->GetCheck() == 1)	//등록번호 삭제
	else if ( pRegCan->GetCheck() == 1 )	//등록번호 삭제
	{
		//등록번호 검색하여 삭제		
		DM_BOOK.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		//CString sQuery_BOOK = _T("SELECT REG_NO FROM BO_BOOK_TBL WHERE species_key in(") + pParent->sRecKey + _T(")");
		CString sQuery_BOOK = _T("SELECT REG_NO FROM BO_BOOK_TBL WHERE rec_key in(") + pParent->sRecKey + _T(")");
		DM_BOOK.RestructDataManager(sQuery_BOOK);
		iBookCount = DM_BOOK.GetRowCount();

		//등록번호 부여 체크
		//CString sQueryCount = _T("select count(reg_no) from BO_BOOK_TBL WHERE species_key in(") + sRecKey + _T(")");
		//CString sCount;	//시작 등록번호
		//pDM->GetOneValueQuery( sQueryCount , sCount );
		//if ( sCount == _T("0") ) {
		if ( iBookCount < 1 ) {
			AfxMessageBox (_T("등록번호 부여된 자료가 없습니다."));
			//m_bInitDlg = FALSE;
			return;
		}
		INT ids;// = 6;

		ids = ::MessageBox( pParent->GetSafeHwnd() , _T("선정하신 자료의 원부번호, 등록번호를 삭제하시겠습니까?") ,
			_T("삭 제") ,
			MB_YESNO | MB_ICONQUESTION );
	
		if ( ids == IDYES ) {		
			CString sQuery;

			//책테이블 삭제
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL WHERE REC_KEY IN (%s);") , pParent->sRecKey );
			iCh = pDM->AddFrame( sQuery );	
			if ( 0 > iCh ) return;

			//DM에서 삭제, array에 저장
			CArray <CString, CString> RemoveListArray;
			for ( INT j = 1 ; j <= pParent->ArrSelectCount.GetSize() ; j ++ ) {
				CString sRegNo = pDM->GetCellData( _T("BB_등록번호"), pParent->ArrSelectCount.GetAt(j-1) );
				RemoveListArray.Add( sRegNo );
				pDM->SetCellData( _T("BB_등록번호") , _T("") , pParent->ArrSelectCount.GetAt(j-1) );
				pDM->SetCellData( _T("원부번호") , _T("") , pParent->ArrSelectCount.GetAt(j-1) );
					//pDM->DeleteRow(ArrSelectCount.GetAt(j-1));
			}
			
			//마크에서 등록번호 삭제
			CESL_DataMgr DM_MARC;
			DM_MARC.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
			//CString sQuery_marc = _T("SELECT MARC , REC_KEY FROM BO_SPECIES_TBL WHERE REC_KEY IN (") + pParent->sRecKey + _T(")");
			CString sQuery_marc = _T("SELECT MARC , REC_KEY FROM BO_SPECIES_TBL WHERE REC_KEY IN (") + pParent->sSpeciesKey + _T(")");
			DM_MARC.RestructDataManager(sQuery_marc);
			INT iMarcCount = DM_MARC.GetRowCount();
			for ( INT i = 0 ; i < iMarcCount ; i ++ ) {
				CString sMarc = DM_MARC.GetCellData( i , 0 );
				CMarc m_marc;
				DM_MARC.SetMarcMgr(pParent->m_pInfo->pMarcMgr);
				pParent->m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

				//decoding
				CString strTagCode, strErrMsg, strHelpMsg;
			
				if(pParent->m_pInfo->pMarcMgr->Decoding(sMarc, &m_marc))
				{
					POSITION pos;
					pos = pParent->m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
					while (pos)
					{
						pParent->m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);	
						//UpdateData(FALSE);
					}	
				}
				//등록번호 삭제하기
				for ( INT j = 1 ; j <= RemoveListArray.GetSize() ; j ++ ) {
					CString sRegNo = RemoveListArray.GetAt(j-1);
					pParent->m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$l"), sRegNo );
				}
				//pParent->m_pInfo->pMarcMgr->DeleteField( &m_marc , _T("049") );
				CString sRegNo;// = m_pInfo->pMarcMgr->GetItem( &m_marc , _T("등록번호") );
				pParent->m_pInfo->pMarcMgr->GetItem(&m_marc, _T("049$l"), sRegNo);	
				//encoding	//마크에 저장		
				pParent->m_pInfo->pMarcMgr->Encoding(&m_marc, sMarc);
				CString sRecKey = DM_MARC.GetCellData( i , 1 );
				sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC = '%s' WHERE REC_KEY ='%s';") , sMarc , pParent->sSpeciesKey );
				iCh = pDM->AddFrame( sQuery );
				//if ( 0 > iCh ) return;
				if ( 1 != iCh ) {
					AfxMessageBox ( _T("DM_MARC.AddFrame Error") );
					return ;
				}
			}
			//누락등록번호추가
			//CESL_DataMgr *pDM_DROP;	
			//pDM_DROP = FindDM(_T("DM_BO_CAT_REG_0400_DROP"));	
			CESL_DataMgr pDM_DROP;
			pDM_DROP.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
			//iCh = pDM_DROP.StartFrame();
			for ( i = 1 ; i <= iBookCount ; i ++ ) {				
				CString sRecKey;			
				ids = pDM_DROP.MakeRecKey( sRecKey );
				if ( 0 > ids ) return;
				//=====================================================
				//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 				CString sRegCode = DM_BOOK.GetCellData(i-1,0).Left(2);
// 				INT iNumber = _ttoi(DM_BOOK.GetCellData(i-1,0).Right(10).GetBuffer(0));

				CString sRegCode = DM_BOOK.GetCellData(i-1,0).Left(nRegCodeLength);
				INT iNumber = _ttoi(DM_BOOK.GetCellData(i-1,0).Right(12-nRegCodeLength).GetBuffer(0));
				//=====================================================
				

				CString sQuery;
				sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%d AND MANAGE_CODE = UDF_MANAGE_CODE") , sRegCode , iNumber);
				CString sCount;	//시작 등록번호
				pDM->GetOneValueQuery( sQuery , sCount );
				if ( sCount == _T("0") ) {
					sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, FIRST_WORK, LAST_WORK, MANAGE_CODE ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s', UDF_MANAGE_CODE );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, m_pParentMgr->GetWorkLogMsg(_T("원부등록"),WORK_LOG), m_pParentMgr->GetWorkLogMsg(_T("원부등록"),WORK_LOG) );
//DEL 					sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, FIRST_WORK, LAST_WORK ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s' );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, m_pParentMgr->GetWorkLogMsg(_T("원부등록"),WORK_LOG), m_pParentMgr->GetWorkLogMsg(_T("원부등록"),WORK_LOG) );
					iCh = pDM->AddFrame( sQuery );
					if ( 0 > iCh ) return;
				}
			}
			//iCh = pDM_DROP.SendFrame();
			//iCh = pDM_DROP.EndFrame();
			
		}
		else {//선택취소
			return;
		}
	}
	else if ( pAcqBack->GetCheck() == 1 ) {
		INT ids;
		ids = ::MessageBox( pParent->GetSafeHwnd() , _T("선정하신 자료를 수서복귀 하시겠습니까?") ,
			_T("삭 제") ,
			MB_YESNO | MB_ICONQUESTION );
		if ( ids == IDYES ) {	
			/*
			CButton *pChkRegCan;
			pChkRegCan = (CButton *)GetDlgItem(IDC_chkRegCan);
			*/
			CString sQuery;
			if ( pChkRegCan->GetCheck() == 1 ) {	//수서복귀, 등록번호(O)	
			//if ( true ) {	
				//등록번호 부여 체크
				CString sQueryCount = _T("select count(reg_no) from BO_BOOK_TBL WHERE rec_key in(") + pParent->sRecKey + _T(")");
				CString sCount;	//시작 등록번호
				pDM->GetOneValueQuery( sQueryCount , sCount );
				if ( sCount == _T("0") ) {
					AfxMessageBox (_T("등록번호 부여된 자료가 없습니다."));
					//m_bInitDlg = FALSE;
					return;
				}
				sQuery.Format( _T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = 'BOA141N', CAT_TRANSFER_DATE ='', ACCESSION_REC_KEY = NULL, REG_NO = NULL WHERE REC_KEY IN (%s);") , pParent->sRecKey );
				iCh = pDM->AddFrame( sQuery );
				if ( 0 > iCh ) return;
				
				//sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET WORKING_STATUS = 'BOA141N' , CAT_TRANSFER_DATE = '' WHERE REC_KEY IN (%s);") , pParent->sRecKey );
				//iCh = pDM->AddFrame( sQuery );		

				sQuery.Format( _T("UPDATE IDX_BO_TBL SET WORK_STATUS = 'BOA141N' WHERE REC_KEY IN (%s);") , pParent->sSpeciesKey );
				iCh = pDM->AddFrame( sQuery );
				if ( 0 > iCh ) return;
				//마크에서 등록번호 삭제				
				CESL_DataMgr DM_MARC;
				DM_MARC.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
				CString sQuery_marc = _T("SELECT MARC , REC_KEY FROM BO_SPECIES_TBL WHERE REC_KEY IN (") + pParent->sSpeciesKey + _T(")");
				DM_MARC.RestructDataManager(sQuery_marc);
				INT iMarcCount = DM_MARC.GetRowCount();
				for ( INT i = 0 ; i < iMarcCount ; i ++ ) {
					CString sMarc = DM_MARC.GetCellData( i , 0 );
					CMarc m_marc;
					DM_MARC.SetMarcMgr(pParent->m_pInfo->pMarcMgr);
					pParent->m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

					//decoding
					CString strTagCode, strErrMsg, strHelpMsg;
				
					if(pParent->m_pInfo->pMarcMgr->Decoding(sMarc, &m_marc))
					{
						POSITION pos;
						pos = pParent->m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
						while (pos)
						{
							pParent->m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);	
							//UpdateData(FALSE);
						}	
					}
					pParent->m_pInfo->pMarcMgr->DeleteField( &m_marc , _T("049") );
					CString sRegNo;// = m_pInfo->pMarcMgr->GetItem( &m_marc , _T("등록번호") );
					pParent->m_pInfo->pMarcMgr->GetItem(&m_marc, _T("049$l"), sRegNo);	
					//encoding	//마크에 저장		
					pParent->m_pInfo->pMarcMgr->Encoding(&m_marc, sMarc);
					CString sRecKey = DM_MARC.GetCellData( i , 1 );
					sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC = '%s' WHERE REC_KEY ='%s';") , sMarc , pParent->sSpeciesKey );	
					iCh = pDM->AddFrame( sQuery );
					if ( 1 != iCh ) {
						AfxMessageBox ( _T("DM_MARC.AddFrame Error") );
						return ;
					}
				}
				//누락등록번호추가
				CESL_DataMgr pDM_DROP;
				pDM_DROP.SetCONNECTION_INFO(pDM->CONNECTION_INFO);				
				for ( i = 1 ; i <= iBookCount ; i ++ ) {				
					CString sRecKey;			
					iCh = pDM_DROP.MakeRecKey( sRecKey );
					if ( 0 > iCh ) return;
					//=====================================================
					//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 					CString sRegCode = DM_BOOK.GetCellData(i-1,0).Left(2);
// 					INT iNumber = _ttoi(DM_BOOK.GetCellData(i-1,0).Right(10).GetBuffer(0));
				
					CString sRegCode = DM_BOOK.GetCellData(i-1,0).Left(nRegCodeLength);
					INT iNumber = _ttoi(DM_BOOK.GetCellData(i-1,0).Right(12-nRegCodeLength).GetBuffer(0));
					//=====================================================
					

					CString sQuery;
					sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%d AND MANAGE_CODE = UDF_MANAGE_CODE") , sRegCode , iNumber);
					CString sCount;	//시작 등록번호

					pDM->GetOneValueQuery( sQuery , sCount );
					if ( sCount == _T("0") ) {
						sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, FIRST_WORK, LAST_WORK, MANAGE_CODE ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s', UDF_MANAGE_CODE );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, m_pParentMgr->GetWorkLogMsg(_T("원부등록"),WORK_LOG), m_pParentMgr->GetWorkLogMsg(_T("원부등록"),WORK_LOG) );
//DEL 						sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, FIRST_WORK, LAST_WORK ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s' );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, m_pParentMgr->GetWorkLogMsg(_T("원부등록"),WORK_LOG), m_pParentMgr->GetWorkLogMsg(_T("원부등록"),WORK_LOG) );
						iCh = pDM->AddFrame( sQuery );
						if ( 0 > iCh ) return;
					}
					//sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_NO , REG_CODE , ACCESSION_REG_NO ) VALUES ( %s, '%s', '%s', '%s', %d );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber );	
					//iCh = pDM->AddFrame( sQuery );				
				}
				for ( INT j = 1 ; j <= pParent->ArrSelectCount.GetSize() ; j ++ ) {				
					pDM->DeleteRow(pParent->ArrSelectCount.GetAt(j-1));
				}
			}
			else {	//수서복귀, 등록번호(X)
				sQuery.Format( _T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = 'BOA_1140', CAT_TRANSFER_DATE = '' WHERE REC_KEY IN (%s);"), pParent->sRecKey );
				iCh = pDM->AddFrame( sQuery );	
				if ( 0 > iCh ) return;
								
				for ( INT j = 1 ; j <= pParent->ArrSelectCount.GetSize() ; j ++ ) {				
					pDM->DeleteRow(pParent->ArrSelectCount.GetAt(j-1));
				}
			}
		}
		else {	//선택취소
		
			return;
		
		}
	}
	
	iCh = pDM->SendFrame();
	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return;
	pParent->AllControlDisplay(_T("CM_단행_정리_원부_메인"));	
	pParent->DestroyWindow();	

EFS_END
}

VOID BO_REG_MGR::AccessionNoGive() {
	
EFS_BEGIN

	CBO_CAT_REG_0400 *pParent = (CBO_CAT_REG_0400*)m_pParentMgr;
	
	CButton *pAccCan;
	pAccCan = (CButton *)pParent->GetDlgItem(IDC_rAccCan);
	CButton *pRegCan;
	pRegCan = (CButton *)pParent->GetDlgItem(IDC_rRegCan);
	CButton *pAcqBack;
	pAcqBack = (CButton *)pParent->GetDlgItem(IDC_rAcqBack);	
	CButton *pChkRegCan;
	pChkRegCan = (CButton *)pParent->GetDlgItem(IDC_chkRegCan);
	CString sQuery;

	CESL_DataMgr *pDM;	
	pDM = pParent->FindDM(_T("DM_단행_정리_등록_메인"));		


	INT iCh;	
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return;

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = pParent->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//누락등록번호 추가 변수
	INT iBookCount = -1; 
	CESL_DataMgr DM_BOOK;

	if ( pAccCan->GetCheck() == 1 ) {	//원부번호 삭제
		//원부번호 부여 체크
		//CString sQueryCount = _T("select count(accession_rec_key) from BO_BOOK_TBL WHERE species_key in(") + pParent->sRecKey + _T(")");
		CString sQueryCount = _T("select count(accession_rec_key) from BO_BOOK_TBL WHERE rec_key in(") + pParent->sRecKey + _T(")");
		CString sCount;	//시작 등록번호
		pDM->GetOneValueQuery( sQueryCount , sCount );
		if ( sCount == _T("0") ) {
			AfxMessageBox (_T("원부번호 부여된 자료가 없습니다."));
			//m_bInitDlg = FALSE;
			return;
		}

		INT ids;

/*		
		ids = ::MessageBox( pParent->GetSafeHwnd() , _T("선정하신 자료의 원부번호를 삭제하시겠습니까?") ,
			_T("삭 제") ,
			MB_YESNO | MB_ICONQUESTION );
*/
		
		ids = MessageBox( m_pParentMgr->m_hWnd , _T("선정하신 자료의 원부번호를 삭제하시겠습니까?") ,
			_T("삭 제") ,
			MB_YESNO | MB_ICONQUESTION );
		if ( ids == IDYES ) {
			CString sQuery;
			//원부 테이블 삭제
			//sQuery.Format( _T("DELETE FROM CO_ACCESSION_REC_TBL WHERE REC_KEY IN (SELECT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY IN (%s) );") , pParent->sRecKey );
			//iCh = pDM->AddFrame( sQuery );

			//sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL WHERE SPECIES_KEY IN (%s);") , pParent->sRecKey );
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL WHERE REC_KEY IN (%s);") , pParent->sRecKey );
			iCh = pDM->AddFrame( sQuery );
			if ( 0 > iCh ) return;

			//그리드에 원부 삭제하기
			for ( INT j = 1 ; j <= pParent->ArrSelectCount.GetSize() ; j ++ ) {				
				//pDM->SetCellData( _T("BB_등록번호") , _T("") , pParent->ArrSelectCount.GetAt(j-1) );
				pDM->SetCellData( _T("원부번호") , _T("") , pParent->ArrSelectCount.GetAt(j-1) );

				//pDM->DeleteRow(ArrSelectCount.GetAt(j-1));
			}

		}
		else {//선택취소
			return;
		}
	}
	//else if ( pRegCan->GetCheck() == 1 && pAccCan->GetCheck() == 1)	//등록번호 삭제
	else if ( pRegCan->GetCheck() == 1 )	//등록번호 삭제
	{
		//등록번호 검색하여 삭제		
		DM_BOOK.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
		//CString sQuery_BOOK = _T("SELECT REG_NO FROM BO_BOOK_TBL WHERE species_key in(") + pParent->sRecKey + _T(")");
		CString sQuery_BOOK = _T("SELECT REG_NO FROM BO_BOOK_TBL WHERE rec_key in(") + pParent->sRecKey + _T(")");
		DM_BOOK.RestructDataManager(sQuery_BOOK);
		iBookCount = DM_BOOK.GetRowCount();

		//등록번호 부여 체크
		//CString sQueryCount = _T("select count(reg_no) from BO_BOOK_TBL WHERE species_key in(") + sRecKey + _T(")");
		//CString sCount;	//시작 등록번호
		//pDM->GetOneValueQuery( sQueryCount , sCount );
		//if ( sCount == _T("0") ) {
		if ( iBookCount < 1 ) {
			AfxMessageBox (_T("등록번호 부여된 자료가 없습니다."));
			//m_bInitDlg = FALSE;
			return;
		}
		INT ids;// = 6;

		ids = ::MessageBox( pParent->GetSafeHwnd() , _T("선정하신 자료의 원부번호, 등록번호를 삭제하시겠습니까?") ,
			_T("삭 제") ,
			MB_YESNO | MB_ICONQUESTION );
	
		if ( ids == IDYES ) {		
			CString sQuery;
			//원부 테이블 삭제
			//sQuery.Format( _T("DELETE FROM CO_ACCESSION_REC_TBL WHERE REC_KEY IN (SELECT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY IN (%s) );") , pParent->sRecKey );
			//iCh = pDM->AddFrame( sQuery );

			//책테이블 삭제
			//sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL WHERE SPECIES_KEY IN (%s);") , pParent->sRecKey );
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL WHERE REC_KEY IN (%s);") , pParent->sRecKey );
			iCh = pDM->AddFrame( sQuery );	
			if ( 0 > iCh ) return;
			//DM에서 삭제, array에 저장
			CArray <CString, CString> RemoveListArray;
			for ( INT j = 1 ; j <= pParent->ArrSelectCount.GetSize() ; j ++ ) {
				CString sRegNo = pDM->GetCellData( _T("BB_등록번호"), pParent->ArrSelectCount.GetAt(j-1) );
				RemoveListArray.Add( sRegNo );
				pDM->SetCellData( _T("BB_등록번호") , _T("") , pParent->ArrSelectCount.GetAt(j-1) );
				pDM->SetCellData( _T("원부번호") , _T("") , pParent->ArrSelectCount.GetAt(j-1) );
					//pDM->DeleteRow(ArrSelectCount.GetAt(j-1));
			}
			
			//마크에서 등록번호 삭제
			CESL_DataMgr DM_MARC;
			DM_MARC.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
			//CString sQuery_marc = _T("SELECT MARC , REC_KEY FROM BO_SPECIES_TBL WHERE REC_KEY IN (") + pParent->sRecKey + _T(")");
			CString sQuery_marc = _T("SELECT MARC , REC_KEY FROM BO_SPECIES_TBL WHERE REC_KEY IN (") + pParent->sSpeciesKey + _T(")");
			DM_MARC.RestructDataManager(sQuery_marc);
			INT iMarcCount = DM_MARC.GetRowCount();
			for ( INT i = 0 ; i < iMarcCount ; i ++ ) {
				CString sMarc = DM_MARC.GetCellData( i , 0 );
				CMarc m_marc;
				DM_MARC.SetMarcMgr(pParent->m_pInfo->pMarcMgr);
				pParent->m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

				//decoding
				CString strTagCode, strErrMsg, strHelpMsg;
			
				if(pParent->m_pInfo->pMarcMgr->Decoding(sMarc, &m_marc))
				{
					POSITION pos;
					pos = pParent->m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
					while (pos)
					{
						pParent->m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);	
						//UpdateData(FALSE);
					}	
				}
				//등록번호 삭제하기
				for ( INT j = 1 ; j <= RemoveListArray.GetSize() ; j ++ ) {
					CString sRegNo = RemoveListArray.GetAt(j-1);
					pParent->m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$l"), sRegNo );
				}
				//pParent->m_pInfo->pMarcMgr->DeleteField( &m_marc , _T("049") );
				CString sRegNo;// = m_pInfo->pMarcMgr->GetItem( &m_marc , _T("등록번호") );
				pParent->m_pInfo->pMarcMgr->GetItem(&m_marc, _T("049$l"), sRegNo);	
				//encoding	//마크에 저장		
				pParent->m_pInfo->pMarcMgr->Encoding(&m_marc, sMarc);
				CString sRecKey = DM_MARC.GetCellData( i , 1 );
				sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC = '%s' WHERE REC_KEY ='%s';") , sMarc , pParent->sSpeciesKey );
				iCh = pDM->AddFrame( sQuery );				
				if ( 1 != iCh ) {
					AfxMessageBox ( _T("DM_MARC.AddFrame Error") );
					return ;
				}
			}
			//누락등록번호추가
			//CESL_DataMgr *pDM_DROP;	
			//pDM_DROP = FindDM(_T("DM_BO_CAT_REG_0400_DROP"));	
			CESL_DataMgr pDM_DROP;
			pDM_DROP.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
			//iCh = pDM_DROP.StartFrame();
			for ( i = 1 ; i <= iBookCount ; i ++ ) {				
				CString sRecKey;			
				iCh = pDM_DROP.MakeRecKey( sRecKey );
				if ( 0 > iCh ) return;
				//=====================================================
				//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 				CString sRegCode = DM_BOOK.GetCellData(i-1,0).Left(2);
// 				INT iNumber = _ttoi(DM_BOOK.GetCellData(i-1,0).Right(10).GetBuffer(0));

				CString sRegCode = DM_BOOK.GetCellData(i-1,0).Left(nRegCodeLength);
				INT iNumber = _ttoi(DM_BOOK.GetCellData(i-1,0).Right(12-nRegCodeLength).GetBuffer(0));
				//=====================================================
				

				CString sQuery;
				sQuery.Format( _T("select count(accession_reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and accession_reg_no =%d AND MANAGE_CODE = UDF_MANAGE_CODE") , sRegCode , iNumber);
				CString sCount;	//시작 등록번호
				pDM->GetOneValueQuery( sQuery , sCount );
				if ( sCount == _T("0") ) {
					sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_NO , REG_CODE , ACCESSION_REG_NO, FIRST_WORK, LAST_WORK, UDF_MANAGE_CODE ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s', UDF_MANAGE_CODE );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, m_pParentMgr->GetWorkLogMsg(_T("원부등록"),WORK_LOG), m_pParentMgr->GetWorkLogMsg(_T("원부등록"),WORK_LOG) );
//DEL 					sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_NO , REG_CODE , ACCESSION_REG_NO, FIRST_WORK, LAST_WORK ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s' );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, m_pParentMgr->GetWorkLogMsg(_T("원부등록"),WORK_LOG), m_pParentMgr->GetWorkLogMsg(_T("원부등록"),WORK_LOG) );
					iCh = pDM->AddFrame( sQuery );
					if ( 0 > iCh ) return;
				}
			}
			//iCh = pDM_DROP.SendFrame();
			//iCh = pDM_DROP.EndFrame();
			
		}
		else {//선택취소
			return;
		}
	}
	else if ( pAcqBack->GetCheck() == 1 ) {
		INT ids;
		ids = ::MessageBox( pParent->GetSafeHwnd() , _T("선정하신 자료를 수서복귀 하시겠습니까?") ,
			_T("삭 제") ,
			MB_YESNO | MB_ICONQUESTION );
		if ( ids == IDYES ) {	
			/*
			CButton *pChkRegCan;
			pChkRegCan = (CButton *)GetDlgItem(IDC_chkRegCan);
			*/
			CString sQuery;
			if ( pChkRegCan->GetCheck() == 1 ) {	//수서복귀, 등록번호(O)	
			//if ( true ) {	
				//등록번호 부여 체크
				CString sQueryCount = _T("select count(reg_no) from BO_BOOK_TBL WHERE rec_key in(") + pParent->sRecKey + _T(")");
				CString sCount;	//시작 등록번호
				pDM->GetOneValueQuery( sQueryCount , sCount );
				if ( sCount == _T("0") ) {
					AfxMessageBox (_T("등록번호 부여된 자료가 없습니다."));
					//m_bInitDlg = FALSE;
					return;
				}
				sQuery.Format( _T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = 'BOA141N', CAT_TRANSFER_DATE ='', ACCESSION_REC_KEY = NULL, REG_NO = NULL WHERE REC_KEY IN (%s);") , pParent->sRecKey );
				iCh = pDM->AddFrame( sQuery );	
				if ( 0 > iCh ) return;
				
				//sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET WORKING_STATUS = 'BOA141N' , CAT_TRANSFER_DATE = '' WHERE REC_KEY IN (%s);") , pParent->sRecKey );
				//iCh = pDM->AddFrame( sQuery );		

				sQuery.Format( _T("UPDATE IDX_BO_TBL SET WORK_STATUS = 'BOA141N' WHERE REC_KEY IN (%s);") , pParent->sSpeciesKey );
				iCh = pDM->AddFrame( sQuery );	
				if ( 0 > iCh ) return;
				//마크에서 등록번호 삭제				
				CESL_DataMgr DM_MARC;
				DM_MARC.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
				CString sQuery_marc = _T("SELECT MARC , REC_KEY FROM BO_SPECIES_TBL WHERE REC_KEY IN (") + pParent->sSpeciesKey + _T(")");
				DM_MARC.RestructDataManager(sQuery_marc);
				INT iMarcCount = DM_MARC.GetRowCount();
				for ( INT i = 0 ; i < iMarcCount ; i ++ ) {
					CString sMarc = DM_MARC.GetCellData( i , 0 );
					CMarc m_marc;
					DM_MARC.SetMarcMgr(pParent->m_pInfo->pMarcMgr);
					pParent->m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

					//decoding
					CString strTagCode, strErrMsg, strHelpMsg;
				
					if(pParent->m_pInfo->pMarcMgr->Decoding(sMarc, &m_marc))
					{
						POSITION pos;
						pos = pParent->m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
						while (pos)
						{
							pParent->m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);	
							//UpdateData(FALSE);
						}	
					}
					pParent->m_pInfo->pMarcMgr->DeleteField( &m_marc , _T("049") );
					CString sRegNo;// = m_pInfo->pMarcMgr->GetItem( &m_marc , _T("등록번호") );
					pParent->m_pInfo->pMarcMgr->GetItem(&m_marc, _T("049$l"), sRegNo);	
					//encoding	//마크에 저장		
					pParent->m_pInfo->pMarcMgr->Encoding(&m_marc, sMarc);
					CString sRecKey = DM_MARC.GetCellData( i , 1 );
					sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC = '%s' WHERE REC_KEY ='%s';") , sMarc , pParent->sSpeciesKey );	
					iCh = pDM->AddFrame( sQuery );
					if ( 1 != iCh ) {
						AfxMessageBox ( _T("DM_MARC.AddFrame Error") );
						return ;
					}
				}
				//누락등록번호추가
				CESL_DataMgr pDM_DROP;
				pDM_DROP.SetCONNECTION_INFO(pDM->CONNECTION_INFO);				
				for ( i = 1 ; i <= iBookCount ; i ++ ) {				
					CString sRecKey;			
					iCh = pDM_DROP.MakeRecKey( sRecKey );
					if ( 0 > iCh ) return;
					//=====================================================
					//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 					CString sRegCode = DM_BOOK.GetCellData(i-1,0).Left(2);
// 					INT iNumber = _ttoi(DM_BOOK.GetCellData(i-1,0).Right(10).GetBuffer(0));
					
					CString sRegCode = DM_BOOK.GetCellData(i-1,0).Left(nRegCodeLength);
					INT iNumber = _ttoi(DM_BOOK.GetCellData(i-1,0).Right(12-nRegCodeLength).GetBuffer(0));
					//=====================================================
					

					CString sQuery;
					sQuery.Format( _T("select count(accession_reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and accession_reg_no =%d AND MANAGE_CODE = UDF_MANAGE_CODE") , sRegCode , iNumber);
					CString sCount;	//시작 등록번호
					pDM->GetOneValueQuery( sQuery , sCount );
					if ( sCount == _T("0") ) {
						sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_NO , REG_CODE , ACCESSION_REG_NO, FIRST_WORK, LAST_WORK, MANAGE_CODE ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s', UDF_MANAGE_CODE );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, m_pParentMgr->GetWorkLogMsg(_T("원부등록"),WORK_LOG), m_pParentMgr->GetWorkLogMsg(_T("원부등록"),WORK_LOG) );
//DEL 						sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_NO , REG_CODE , ACCESSION_REG_NO, FIRST_WORK, LAST_WORK ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s' );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, m_pParentMgr->GetWorkLogMsg(_T("원부등록"),WORK_LOG), m_pParentMgr->GetWorkLogMsg(_T("원부등록"),WORK_LOG) );
						iCh = pDM->AddFrame( sQuery );
						if ( 0 > iCh ) return;
					}
					//sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_NO , REG_CODE , ACCESSION_REG_NO ) VALUES ( %s, '%s', '%s', '%s', %d );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber );	
					//iCh = pDM->AddFrame( sQuery );				
				}
				for ( INT j = 1 ; j <= pParent->ArrSelectCount.GetSize() ; j ++ ) {				
					pDM->DeleteRow(pParent->ArrSelectCount.GetAt(j-1));
				}
			}
			else {	//수서복귀, 등록번호(X)
				sQuery.Format( _T("UPDATE BO_BOOK_TBL SET WORKING_STATUS = 'BOA141N', CAT_TRANSFER_DATE = '' WHERE REC_KEY IN (%s);"), pParent->sRecKey );
				iCh = pDM->AddFrame( sQuery );
				if ( 0 > iCh ) return;
				
				//sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET WORKING_STATUS = 'BOA141N' , CAT_TRANSFER_DATE = '' WHERE REC_KEY IN (%s);") , pParent->sRecKey );
				//iCh = pDM->AddFrame( sQuery );	
				
				sQuery.Format( _T("UPDATE IDX_BO_TBL SET WORK_STATUS = 'BOA141N' WHERE REC_KEY IN (%s);") , pParent->sSpeciesKey );
				iCh = pDM->AddFrame( sQuery );
				if ( 0 > iCh ) return;
				
				for ( INT j = 1 ; j <= pParent->ArrSelectCount.GetSize() ; j ++ ) {				
					pDM->DeleteRow(pParent->ArrSelectCount.GetAt(j-1));
				}
			}
		}
		else {	//선택취소
		
			return;
		
		}
	}
	
	iCh = pDM->SendFrame();
	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return;
	}
	iCh = pDM->EndFrame();
	if ( 0 > iCh ) return;
	pParent->AllControlDisplay(_T("CM_단행_정리_등록_메인"));	
	pParent->DestroyWindow();	

EFS_END
}
