// BO_CAT_RegNoCancel.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_RegNoCancel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_RegNoCancel dialog


CBO_CAT_RegNoCancel::CBO_CAT_RegNoCancel(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_RegNoCancel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CBO_CAT_RegNoCancel::~CBO_CAT_RegNoCancel()
{
	
}


VOID CBO_CAT_RegNoCancel::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_RegNoCancel)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}

BEGIN_MESSAGE_MAP(CBO_CAT_RegNoCancel, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_RegNoCancel)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_RegNoCancel message handlers

BOOL CBO_CAT_RegNoCancel::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	/*
	if (InitESL_Mgr(_T("BO_CAT_REG_0300_NEW")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	*/


	EnableThemeDialogTexture(GetSafeHwnd()); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

INT CBO_CAT_RegNoCancel::RegNoCancel(CString sBookKey)
{
EFS_BEGIN

	if (InitESL_Mgr(_T("BO_CAT_REGNO_CANCEL")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return -1;
	}

	//책dm 구성
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_REG_NO_CANCEL"));
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_REG_NO_CANCEL error") );
		return FALSE;
	}

	CString sWhere;
	sWhere.Format( _T(" B.SPECIES_KEY = S.REC_KEY AND B.REC_KEY IN (%s) ORDER BY S.REC_KEY"), sBookKey);
	if (pDM->RefreshDataManager(sWhere) < 0)	return -1;

	INT iRowCount = pDM->GetRowCount();
	if ( pDM->GetRowCount() < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
		return FALSE;
	}
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	/*
	//종dm 구성
	CESL_DataMgr *pDM_species;
	pDM_species = FindDM(_T("DM_BO_CAT_REG_NO_SPECIES"));
	if ( pDM_species == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_REG_NO_SPECIES ERROR") );
		return -1;
	}
	*/
	//sWhere.Format( _T(" 

	INT ids;
	CString sQuery;
	//원부 테이블 삭제
	//sQuery.Format( _T("DELETE FROM CO_ACCESSION_REC_TBL WHERE REC_KEY IN (SELECT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE SPECIES_KEY IN (%s) );") , pParent->sRecKey );
	//iCh = pDM->AddFrame( sQuery );

	//책테이블 삭제	
	sQuery.Format( _T("UPDATE BO_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL WHERE REC_KEY IN (%s);") , sBookKey );
	ids = pDM->AddFrame( sQuery );
		
	CString sSpeciesKeyPre;
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sDmBookKey;
		CString sRegNo;
		CString sSpeciesKey;
		pDM->GetCellData( _T("BB_책KEY"), i, sDmBookKey );
		pDM->GetCellData( _T("BB_등록번호"), i, sRegNo );
		pDM->GetCellData( _T("BS_종KEY"), i, sSpeciesKey );

		//MARC에서 등록번호 삭제
		CString sMarc;
		CMarc m_marc;
		if ( sSpeciesKeyPre != sSpeciesKey ) {
			pDM->GetCellData( _T("BS_MARC"), i, sMarc );
			//decoding				
			m_pInfo->pMarcMgr->Decoding(sMarc, &m_marc);
			m_pInfo->pMarcMgr->DeleteField( &m_marc , _T("049") );
			//encoding	//마크에 저장		
			m_pInfo->pMarcMgr->Encoding(&m_marc, sMarc);
			
			sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC = '%s' WHERE REC_KEY ='%s';") , sMarc , sSpeciesKey );
			ids = pDM->AddFrame( sQuery );
			if ( 1 != ids ) {
				AfxMessageBox ( _T("pDM->AddFrame Error") );
				return -1;
			}
		}

		//누락등록번호추가		
		CString sRecKey;			
		pDM->MakeRecKey( sRecKey );
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		CString sRegCode = sRegNo.Left(2);
// 		INT iNumber = _ttoi(sRegNo.Right(10).GetBuffer(0));
	
		CString sRegCode = sRegNo.Left(nRegCodeLength);
		INT iNumber = _ttoi(sRegNo.Right(12-nRegCodeLength).GetBuffer(0));
		//=====================================================
		
		CString sQuery;
		sQuery.Format( _T("select count(accession_reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and accession_reg_no =%d AND MANAGE_CODE = UDF_MANAGE_CODE") , sRegCode , iNumber);
		CString sCount;
		pDM->GetOneValueQuery( sQuery , sCount );
		if ( sCount == _T("0") ) {
			sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_NO , REG_CODE , ACCESSION_REG_NO, FIRST_WORK, LAST_WORK, MANAGE_CODE ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s', UDF_MANAGE_CODE );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG), GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG) );
//DEL 			sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_NO , REG_CODE , ACCESSION_REG_NO, FIRST_WORK, LAST_WORK ) VALUES ( %s, '%s', '%s', '%s', %d, '%s', '%s' );") , sRecKey , _T("MO") , _T("") , sRegCode , iNumber, GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG), GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG) );
			ids = pDM->AddFrame( sQuery );				
		}
	}

	ids = pDM->SendFrame();
	if ( ids == -3 || ids == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return -1;
	}
	ids = pDM->EndFrame();	

	return 0;

EFS_END
return -1;

}


HBRUSH CBO_CAT_RegNoCancel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

