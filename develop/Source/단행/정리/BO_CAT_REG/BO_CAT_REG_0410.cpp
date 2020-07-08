// BO_CAT_REG_0410.cpp : implementation file
//

#include "stdafx.h"
#include "BO_REG_MGR.h"
#include "BO_CAT_REG_0410.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0410 dialog


CBO_CAT_REG_0410::CBO_CAT_REG_0410(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_0410)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CBO_CAT_REG_0410::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_0410)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_0410, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_0410)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0410 message handlers

INT CBO_CAT_REG_0410::CancelRegNo()
{
	m_DM_ALIAS = _T("DM_단행_정리_등록_메인");
	m_CM_ALIAS = _T("CM_단행_정리_등록_메인");

// DM, CM, GRID 선정자료 체크 
	CESL_DataMgr *pDM;	
	pDM = FindDM(m_DM_ALIAS);
	if (!pDM)
	{
		AfxMessageBox(_T("DM 에러!")); 
		return -1; 
	}
	CESL_ControlMgr *pCM;
	pCM = FindCM(m_CM_ALIAS);		
	if (!pCM)
	{
		AfxMessageBox(_T("CM 에러!")); 
		return -1; 
	}
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));
	pGrid->SelectMakeList();
	INT Index = pGrid->SelectGetCount();
	if (Index < 1)
	{
		AfxMessageBox(_T("선정한 자료가 없습니다.")); 
		return -1;
	}	

	INT iCh; 
	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return -1;

//누락등록번호 추가 변수
	INT iBookCount = -1; 
	CESL_DataMgr DM_BOOK;
//번호취소 
	if ( m_nType == 0 ) 
		DeleteRegNo();
//등록인계취소, 검수취소
	else if ( m_nType == 1 ) 
	{
		DeleteRegNo();
		BackAcq();
	}
	
	iCh = pDM->SendFrame();
	if ( iCh == -3 || iCh == -1) 
	{
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return -5;
	}
	iCh = pDM->EndFrame();	
	if ( 0 > iCh ) return -1;

	return 0;	
}

INT CBO_CAT_REG_0410::BackAcq()
{
	INT iCh;	
	CString sQuery;

	CESL_DataMgr *pDM;	
	pDM = FindDM(m_DM_ALIAS);		

	CESL_ControlMgr *pCM;
	pCM = FindCM(m_CM_ALIAS);		
	
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

// 등록 번호를 선택하여 누락등록 번호에 넣는다. 

	INT nNoAcqCode = 0; 
	CString AcqCode, sRecKey, strSpeciesKey, strWorkingStatus;
	CArray <INT, INT> RemoveAllList;
	INT idx;  

	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));
		
	while ( idx != -1 ) 
	{
		AcqCode = pDM->GetCellData( _T("BB_수입구분") , idx );
		sRecKey = pDM->GetCellData( _T("BB_책KEY") , idx);
		strSpeciesKey = pDM->GetCellData(_T("IBS_종KEY"), idx);
		
		AcqCode.TrimLeft();
		AcqCode.TrimRight();
		
		if (AcqCode != _T("1") && AcqCode != _T("2")) //등록자료 	
			nNoAcqCode++;
		else
		{
			if ( AcqCode == _T("1")) // 구입자료 
			{
				if (CancelType == 1)
				{
					strWorkingStatus = _T("BOA113O");
					sQuery.Format(
						_T("UPDATE BO_BOOK_TBL SET DELIVERY_YN ='N', LAST_WORK='%s', WORKING_STATUS = '%s', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;"),
						GetWorkLogMsg(_T("등록취소"),WORK_LOG), strWorkingStatus, sRecKey);
					iCh = pDM->AddFrame( sQuery );	
					if ( 0 > iCh ) return -1;
				}
				else 
				{
					// 구입 인계 대상자료 복귀
					strWorkingStatus = _T("BOA114O");
					sQuery.Format( 
						_T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = '%s', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;"),
						GetWorkLogMsg(_T("등록취소"),WORK_LOG), strWorkingStatus, sRecKey);
					iCh = pDM->AddFrame( sQuery );
					if ( 0 > iCh ) return -1;
				}
			}
			else if ( AcqCode == _T("2")) // 기증자료 
			{
				// 기증 인계 대상자료 복귀
				strWorkingStatus = _T("BOA211O");
				sQuery.Format( 
					_T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = '%s', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;"),
					GetWorkLogMsg(_T("등록취소"),WORK_LOG), strWorkingStatus, sRecKey);
				iCh = pDM->AddFrame( sQuery );	
				if ( 0 > iCh ) return -1;
			}
			sQuery.Format(
				_T("UPDATE IDX_BO_TBL SET LAST_WORK='%s', WORKING_STATUS=TESTWORKSTATUS(%s) WHERE REC_KEY=%s;"),
				GetWorkLogMsg(_T("등록취소"),WORK_LOG), strSpeciesKey, strSpeciesKey);
			iCh = pDM->AddFrame( sQuery );	
			if ( 0 > iCh ) return -1;
			RemoveAllList.Add(idx); 
		}
		idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	}
// DM삭제	 
	for ( INT i = RemoveAllList.GetSize()-1 ; i >= 0 ; i-- )
	{ 
		INT RowIdx = RemoveAllList.GetAt(i); 
		pDM->DeleteRow(RowIdx);
		
	}
			
	if (nNoAcqCode > 0) //등록자료 
	{	
		CString strNoAcqCode; 
		if (CancelType == 1)
		{
			strNoAcqCode.Format(_T("수입구분이 구입/기증이 아닌 자료 %d건은 검수취소하지 못했습니다."), nNoAcqCode);
			AfxMessageBox(strNoAcqCode); 
		}
		else
		{
			strNoAcqCode.Format(_T("수입구분이 구입/기증이 아닌 자료 %d건은 등록인계취소하지 못했습니다."), nNoAcqCode);
			AfxMessageBox(strNoAcqCode); 
		}
	}
	
	pGrid->Display();
	return 0;
}

INT CBO_CAT_REG_0410::DeleteRegNo()
{
	INT iCh;
	INT nIds;
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
	
	CString strRegNo,strRegCode,strAccNo,sRecKey, strSpeciesKey;
	strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );
	
	strRegNo.TrimLeft();
	strRegNo.TrimRight();

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	
	CMarc pMarc;
	CString sMarc;
	//m_pInfo->pMarcMgr->Decoding(strMarc, &pMarc);

	while ( idx != -1 ) {
		strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );
		
		if (strRegNo != _T("")) //등록자료 
		{
			strRegCode = pDM->GetCellData( _T("BB_등록구분_코드") , idx );
			strAccNo = pDM->GetCellData( _T("원부번호") , idx );
			sRecKey = pDM->GetCellData( _T("BB_책KEY") , idx );
			strSpeciesKey = pDM->GetCellData(_T("IBS_종KEY"), idx);

			//===================================================
			//2009.05.06 ADD BY LKS : 입력된(마크교체를 통해) 049태그를 지워준다.			
			sMarc = pDM->GetCellData( _T("BS_MARC") , idx );
			if(!sMarc.IsEmpty())
			{
				m_pInfo->pMarcMgr->Decoding(sMarc, &pMarc);
				m_pInfo->pMarcMgr->DeleteField(&pMarc, _T("049"));
				m_pInfo->pMarcMgr->Encoding(&pMarc, sMarc);	
				//===================================================
				//2009.08.20 UPDATE BY PJW : MARC길이가길경우를 대비해서 수정함
//				//===================================================
//				//2009.06.01 ADD BY LKS : UPDATE시 " ' "가 있을경우 error!!
//				//							" ' "를 " '' "로 교체
//				
//				sMarc.Replace(_T("'"),_T("''"));
//				//===================================================
//				sQuery.Format(
//					_T("UPDATE BO_SPECIES_TBL SET MARC = '%s' WHERE REC_KEY = %s;"), 
//					sMarc, strSpeciesKey);
//				iCh = pDM->AddFrame( sQuery );
//				if ( 0 > iCh ) return -1;
				pDM->InitDBFieldData();
				nIds = pDM->AddDBFieldData(_T("MARC"), _T("STRING"), sMarc, 1);
				if(nIds < 0) return nIds;
				nIds = pDM->MakeUpdateFrame(_T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);
				if(nIds < 0) return nIds;
				//===================================================
			}
			//===================================================
			
			// 등록 번호 제거
			sQuery.Format(
				_T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL , REG_DATE = '', WORKING_STATUS = 'BOR111N', LAST_WORK='%s' WHERE REC_KEY = %s;"), 
				GetWorkLogMsg(_T("등록취소"),WORK_LOG), sRecKey);
			iCh = pDM->AddFrame( sQuery );	
			if ( 0 > iCh ) return -1;
			sQuery.Format(
				_T("UPDATE IDX_BO_TBL SET WORKING_STATUS = TESTWORKSTATUS(%s), LAST_WORK='%s' ")
				_T("WHERE REC_KEY = %s;"), 
				strSpeciesKey, GetWorkLogMsg(_T("등록취소"),WORK_LOG), strSpeciesKey);
			iCh = pDM->AddFrame( sQuery );	
			if ( 0 > iCh ) return -1;
			
			//누락 등록 테이블에 있는 지 조사 
			CString sQuery,sCount;
			
			//=====================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 			sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%s AND MANAGE_CODE = UDF_MANAGE_CODE") , strRegCode , strRegNo.Mid(2,10));
			sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%s AND MANAGE_CODE = UDF_MANAGE_CODE") , strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength ) );
			//=====================================================
			
			pDM->GetOneValueQuery( sQuery , sCount );
			//없으면 입력 
			if ( sCount == _T("0") ) {
				CString sRecKey;
				iCh = pDM->MakeRecKey(sRecKey);
				if ( 0 > iCh ) return -1;
		
				//=====================================================
				//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 				sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, MANAGE_CODE ) VALUES ( %s, '%s', '%s', '%s', %s, UDF_MANAGE_CODE );") , sRecKey , _T("MO") , _T("") , strRegCode , strRegNo.Mid(2,10) );				
				sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, MANAGE_CODE ) VALUES ( %s, '%s', '%s', '%s', %s, UDF_MANAGE_CODE );") , sRecKey , _T("MO") , _T("") , strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength ) );
				//=====================================================
				
//DEL 				sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO ) VALUES ( %s, '%s', '%s', '%s', %s );") , sRecKey , _T("MO") , _T("") , strRegCode , strRegNo.Mid(2,10) );
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

VOID CBO_CAT_REG_0410::SetOperationType(INT nType)
{
	m_nType = nType;
}
