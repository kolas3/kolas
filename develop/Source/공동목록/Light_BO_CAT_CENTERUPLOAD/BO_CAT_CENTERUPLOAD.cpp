// BO_CAT_CENTERUPLOAD.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_CENTERUPLOAD.h"
#include "..\..\include\공동목록\CU_ComApi.h"
#include "..\..\공동목록\check_dup_api\rs_sql.h"

//==========================================================================
//2004.02.04
#include "..\CenterUpload_api\CenterUpload.h"
//==========================================================================



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_CENTERUPLOAD dialog


CBO_CAT_CENTERUPLOAD::CBO_CAT_CENTERUPLOAD(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCENTER_UPLOAD_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CBO_CAT_CENTERUPLOAD::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBO_CAT_CENTERUPLOAD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_CENTERUPLOAD)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_CENTERUPLOAD, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_CENTERUPLOAD)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_CENTERUPLOAD message handlers


VOID CBO_CAT_CENTERUPLOAD::SetDMAlias (CString BasicDM,CString DupDM,CString pParentCM) 
{
	m_pDMAlias      = BasicDM ;
	m_pDupDMAlias   = DupDM ;
	m_pParentCM     = pParentCM ;
}


INT CBO_CAT_CENTERUPLOAD::CenterUpload (bool m_bMarcSyntaxCheck)
{

    INT ids, i;
    INT iTotalCnt = 0;
    INT iSelectedCnt = 0;
    INT iFinishedCnt = 0;
    INT iDupMatCnt = 0;
    INT iSyntaxErrorCnt = 0;
    INT iEtcErrorCnt = 0;
    INT iCurIndex = 0;
	INT iSpeciesCnt = 0 ;
    CString sFlag;
    CString lib_code;
    CString frm;


    m_pBaseDataMgr = FindDM(m_pDMAlias ) ;
	if(m_pBaseDataMgr == NULL){
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -1 ;
	}
	
    m_pDupDataMgr = FindDM(m_pDupDMAlias  ) ;
	if(m_pDupDataMgr == NULL){
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -1 ;
	}

    CRS_SQL m_RS_SQL(this) ;
    ids  = m_RS_SQL.GetLibraryInfo (lib_code) ;
    
	CCenterUpload Check_Upload_api(this) ;
	CString str_DIRECT_UPLOAD_YN ;
	Check_Upload_api.Check_Direct_Upload (str_DIRECT_UPLOAD_YN) ;
    //==============================================================================

	CString strRegNos , strUseLimitCodes , strSpeciesLimitCode;

	iTotalCnt = m_pBaseDataMgr->GetRowCount () ;
	CString strMarc ;
	for ( i=0; i < iTotalCnt; i++ )
	{
		
        iCurIndex = i ;
		if(str_DIRECT_UPLOAD_YN == _T("N"))
		{   
			CString strSpeciesKey ;	
						CString strControlNo;
            
			m_pBaseDataMgr->GetCellData (_T("REC_KEY"),iCurIndex,strSpeciesKey) ;
			m_pBaseDataMgr->GetCellData (_T("MARC"),iCurIndex,strMarc) ;

			m_pBaseDataMgr->GetCellData (_T("제어번호"), iCurIndex, strControlNo);
		
			m_pBaseDataMgr->GetCellData (_T("이용제한구분"), i, strSpeciesLimitCode);
			MakeStringData( strControlNo, strRegNos , strUseLimitCodes );
			ids = Check_Upload_api.Input_Temp_Mater_tbl (strSpeciesKey, strMarc, _T(""), strControlNo ,strRegNos , strUseLimitCodes , strSpeciesLimitCode);
			if(ids < 0 )
			{
				ids = UpdateUploadStatus( this, m_pDMAlias, iCurIndex, _T("4") );
				if (ids<0) AfxMessageBox (_T("UpdateUploadStatus(4) Error!!!"), FALSE);
				
				iEtcErrorCnt ++ ;
				iFinishedCnt ++ ;
				continue;
			}
			iSpeciesCnt++ ;
			ids = UpdateUploadStatus( this, m_pDMAlias, iCurIndex, _T("0") );

			iFinishedCnt ++ ;
			continue ;

		}
		else 
		{			
			AfxMessageBox(_T("해당 도서관 부호가 공동목록 도서관 부호에 등록되어 있지 않습니다.")) ;
			return -1;	
		}
    }


   	if(str_DIRECT_UPLOAD_YN == _T("N"))
	{
		ids = Check_Upload_api.Update_Temp_Lib_Statistics_Tbl(iSpeciesCnt,_T("U")) ;
	}

	CMarc	 pMarc;
	CString  str035FieldData;
	CMarcMgr *pMarcMgr = m_pInfo->pMarcMgr ;
	pMarcMgr->SetMarcGroup (_T("통합마크")) ;
	if ( strMarc == _T("") )
	{
		m_pBaseDataMgr->GetCellData (_T("MARC"),iCurIndex,strMarc) ;
	}
	
	pMarcMgr->Decoding(strMarc, &pMarc);
	pMarcMgr->GetItem(&pMarc, _T("035$a"), str035FieldData);
	
	if ( str035FieldData.GetLength() > 3 )
	{
		if ( str035FieldData.Left(3) == _T("BTP") )
		{

			ids = UpdateBTPStatisticsInfo( this, _T("DM_CENTERUPLOAD_복본자료"),_T("DM_CENTERUPLOAD_복본자료"), iSpeciesCnt);

			if (ids < 0) AfxMessageBox (_T("UpdateBTPStatisticsInfo() Error!!!"), FALSE);
		}
	}
	
	CString Msg ;
	CString temp ;
	Msg.Format( _T("총 %d건의 자료 중에서 %d건이 센터로 업로드 되었습니다.\n\n"), iFinishedCnt, iSpeciesCnt);
    if ( iDupMatCnt > 0 ) {
        temp.Format( _T("확인을 요하는자료 : %d건\n"), iDupMatCnt );
        Msg += temp;
    }
    if ( iSyntaxErrorCnt > 0 ) {
        temp.Format( _T("MARC 오류자료     : %d건\n"), iSyntaxErrorCnt );
        Msg += temp;
    }
    if ( iEtcErrorCnt > 0 ) {
        temp.Format( _T("기타 오류자료     : %d건\n"), iEtcErrorCnt );
        Msg += temp;
    }
    if(iDupMatCnt > 0 || iSyntaxErrorCnt > 0 || iEtcErrorCnt > 0 )
	{
		INT errCnt ;
		errCnt = iDupMatCnt + iSyntaxErrorCnt + iEtcErrorCnt ;

		CString tmpCnt ;
		tmpCnt.Format (_T("%d"),errCnt);
	    Msg += tmpCnt + _T("건은 UPLOAD되지 않았습니다. 건별로 작업하십시오.");			
	}
    AfxMessageBox(Msg) ; 

	return 0 ;
}


VOID CBO_CAT_CENTERUPLOAD::MakeStringData( CString ControlNo , CString &Reg_nos , CString &UseLimitCodes )
{
	if(ControlNo.IsEmpty()) return;
	
	Reg_nos = _T("");
	UseLimitCodes = _T("");
	
	CESL_DataMgr pDM;
	CString strQuery;
	int ids;
	
	strQuery.Format(
		_T("SELECT BB.REG_NO , BB.USE_LIMIT_CODE FROM BO_BOOK_TBL BB ")
		_T("WHERE BB.SPECIES_KEY = ( SELECT MAX(BS.REC_KEY) FROM BO_SPECIES_TBL BS WHERE BS.CONTROL_NO = '%s' AND BS.MANAGE_CODE = UDF_MANAGE_CODE ) ")
		,ControlNo
		);
	
	pDM.SetCONNECTION_INFO(m_pBaseDataMgr->CONNECTION_INFO);
	
	ids = pDM.RestructDataManager(strQuery);
	
	if (ids < 0 ) return;
	if ( pDM.GetRowCount() <= 0 ) return;
	
	CString Reg_No = _T(""), UseLimitCode = _T("");
	
	for ( int i=0 ; i < pDM.GetRowCount()  ; i++ )
	{
		Reg_No = _T("");
		UseLimitCode = _T("");
		
		pDM.GetCellData(  i , 0 , Reg_No );		
		
		if ( Reg_nos.IsEmpty() ) { Reg_nos += Reg_No; }
		else 
		{
			if (!Reg_No.IsEmpty()) Reg_nos += _T(",") + Reg_No;
		}
		
		pDM.GetCellData( i , 1 , UseLimitCode );
		
		if ( UseLimitCodes.IsEmpty() ){ UseLimitCodes += UseLimitCode; }
		else
		{
			if (!UseLimitCode.IsEmpty())  UseLimitCodes += _T(",") + UseLimitCode;
		}
		
	}
	
}