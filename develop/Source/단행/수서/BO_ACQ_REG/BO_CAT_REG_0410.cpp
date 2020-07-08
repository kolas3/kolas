// BO_CAT_REG_0410.cpp : implementation file
//

#include "stdafx.h"
// #include _T("BO_REG_MGR.h")
#include "BO_CAT_REG_0410.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WORK_LOG GetWorkLogMsg(_T("등록취소"), _T(__FILE__), __LINE__)
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
	DestroyWindow();	
	
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
	CString AcqCode, sRecKey;
	CArray <INT, INT> RemoveAllList;
	INT idx;  

	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));
		
	while ( idx != -1 ) 
	{
		AcqCode = pDM->GetCellData( _T("BB_수입구분") , idx );
		sRecKey = pDM->GetCellData( _T("BB_책KEY") , idx);
		
		AcqCode.TrimLeft();
		AcqCode.TrimRight();
		
		if (AcqCode == _T("") || AcqCode == _T("3")) //등록자료 	
			nNoAcqCode++;

		else
		{
			if ( AcqCode == _T("1")) // 구입자료 
			{
				if (CancelType == 1)
				{
					sQuery.Format( _T("UPDATE BO_BOOK_TBL SET DELIVERY_YN ='N', LAST_WORK='%s', WORKING_STATUS = 'BOA113O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,WORK_LOG, sRecKey );
					iCh = pDM->AddFrame( sQuery );	
				}
				else 
				{
					// 구입 인계 대상자료 복귀
					sQuery.Format( _T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = 'BOA114O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,WORK_LOG, sRecKey );
					iCh = pDM->AddFrame( sQuery );
				}
			}
			else if ( AcqCode == _T("2")) // 기증자료 
			{
				// 기증 인계 대상자료 복귀
				sQuery.Format( _T("UPDATE BO_BOOK_TBL SET LAST_WORK='%s', WORKING_STATUS = 'BOA211O', PREV_WORKING_STATUS = 'BOR112N' WHERE REC_KEY = %s;") ,WORK_LOG, sRecKey );
				iCh = pDM->AddFrame( sQuery );	
			}
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
			strNoAcqCode.Format(_T("수입구분이 없거나 교환인 자료 %d건은 검수취소하지 못했니다."), nNoAcqCode);
			AfxMessageBox(strNoAcqCode); 
		}
		strNoAcqCode.Format(_T("수입구분이 없거나 교환인 자료 %d건은 등록인계취소하지 못했니다."), nNoAcqCode);
		AfxMessageBox(strNoAcqCode); 
	}
	
	pGrid->Display();
	return 0;
}

INT CBO_CAT_REG_0410::DeleteRegNo()
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
	
	while ( idx != -1 ) {
		strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );
		
		if (strRegNo != _T("")) //등록자료 
		{
			strRegCode = pDM->GetCellData( _T("BB_등록구분_코드") , idx );
			strAccNo = pDM->GetCellData( _T("원부번호") , idx );
			sRecKey = pDM->GetCellData( _T("BB_책KEY") , idx );

			// 등록 번호 제거
			sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL , WORKING_STATUS = 'BOR111N' WHERE REC_KEY = %s AND MANAGE_CODE = UDF_MANAGE_CODE;") , sRecKey );
			iCh = pDM->AddFrame( sQuery );	

			//누락 등록 테이블에 있는 지 조사 
			CString sQuery,sCount;
			sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%s AND MANAGE_CODE = UDF_MANAGE_CODE") , strRegCode , strRegNo.Mid(2,10));
			pDM->GetOneValueQuery( sQuery , sCount );
			//없으면 입력 
			if ( sCount == _T("0") ) {
				CString sRecKey;
				pDM->MakeRecKey(sRecKey);
				
				sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO , MANAGE_CODE) VALUES ( %s, '%s', '%s', '%s', %s , UDF_MANAGE_CODE);") , sRecKey , _T("MO") , _T("") , strRegCode , strRegNo.Mid(2,10) );
				iCh = pDM->AddFrame( sQuery );		
				
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
