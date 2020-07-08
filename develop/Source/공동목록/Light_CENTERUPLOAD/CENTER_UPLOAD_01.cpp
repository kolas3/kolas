// CENTER_UPLOAD_01.cpp : implementation file
//

#include "stdafx.h"
#include "CENTER_UPLOAD_01.h"
#include "CENTER_UPLOAD_02.h"
#include "CENTER_UPLOAD_03.h"
#include "CENTER_UPLOAD_04.h"

#include "..\CenterUpload_api\CenterUpload.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCENTER_UPLOAD_01 dialog
CCENTER_UPLOAD_01::CCENTER_UPLOAD_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCENTER_UPLOAD_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	// KOL.UDF.022 시큐어코딩 보완
	m_pBaseDataMgr = NULL;
	m_sWhere = _T("");
    strBookWhere = _T("");
}

CCENTER_UPLOAD_01::~CCENTER_UPLOAD_01()
{
}

VOID CCENTER_UPLOAD_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCENTER_UPLOAD_01)
//	DDX_Control(pDX, IDC_PROGRESS_search, m_progress_search);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCENTER_UPLOAD_01, CDialog)
	//{{AFX_MSG_MAP(CCENTER_UPLOAD_01)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCENTER_UPLOAD_01 message handlers


BOOL CCENTER_UPLOAD_01::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CCENTER_UPLOAD_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);	
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MainGrid);
	if (pGrid->GetSafeHwnd() == NULL) return;
    pGrid->MoveWindow(0, 0, cx, cy);


}

BOOL CCENTER_UPLOAD_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
		                 
    if (InitESL_Mgr(_T("SM_Light_CENTERUPLOAD")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
      
    m_pBaseDataMgr = FindDM(_T("DM_CENTERUPLOAD_01")) ;
	if(m_pBaseDataMgr == NULL) 
	{
       AfxMessageBox (_T("FindDM() Error")) ;
	   return FALSE ;
	}
  
	CCenterUpload UploadApi(this);
	CString strDirectUpload;
	UploadApi.Check_Direct_Upload(strDirectUpload);
	// 운영DB 업로드값에 따라 건별전송기능 사용유무가 결정된다
	pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&CString(_T("건별전송")), (strDirectUpload.CompareNoCase(_T("Y")) != 0));

    BtnEnable() ;

	EnableThemeDialogTexture(GetSafeHwnd());

    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CCENTER_UPLOAD_01::OnbFIND() 
{
    INT ids;
    CString sWhere;

    // 컨트롤의 데이타를 읽어온다.
    const alias_cnt = 7;
    CString  alias[alias_cnt] = {
        _T("UPLOAD상태"), _T("배가일자-부터"), _T("배가일자-까지"), _T("배가위치"),
        _T("번호구분"),   _T("번호부터"), _T("번호까지")
    };

	CString data[alias_cnt]  ;

    CESL_ControlMgr *pCM = FindCM(_T("CM_CENTERUPLOAD_01")) ;
    
	for(INT i = 0 ; i < 7 ; i ++) 
	{
       ids = pCM->GetControlMgrData (alias[i],data[i],-1,i) ;
	   data[i].TrimLeft() ;
	   data[i].TrimRight () ;

	   if(ids <0 ) AfxMessageBox (_T("GetControlMgrData() Error")) ;
	}
	
    

    // 전체자료에서 찾는경우 다른 조건을 더 입력해야한다.
    if (  data[0] == _T("전체") ) {
        if ((  data[1].IsEmpty () || data[2].IsEmpty () ) && (  data[3].IsEmpty () ) && (  data[5].IsEmpty () )) {
            AfxMessageBox (_T("전체자료에서 검색시 다른 조건을 더 입력해야 합니다."));
        }
    }

  
    if ( data[0] == _T("전체") ) sWhere = _T("1=1 ");
	// ----------------------------------------------------------------------------------------------------
    
	else if (  data[0] == _T("안올린자료") ) sWhere = _T("s.CENTER_UPLOAD_STATUS  = '1' OR s.CENTER_UPLOAD_STATUS IS NULL");
    else if (  data[0] == _T("안올린자료(확인요망)")) sWhere = _T("s.CENTER_UPLOAD_STATUS = '2' ");
    else if (  data[0] == _T("안올린자료(MARC에러)") ) sWhere = _T("s.CENTER_UPLOAD_STATUS = '3' ");
    else if (  data[0] == _T("안올린자료(기타)")) sWhere = _T("s.CENTER_UPLOAD_STATUS = '4' ");
	else if (  data[0] == _T("임시 DB로 올린자료")) sWhere = _T("s.CENTER_UPLOAD_STATUS = '5' ");
	else if (  data[0] == _T("반출 후 국중에 제공한 자료")) sWhere = _T("CENTER_UPLOAD_STATUS = '6' ");
	// ---------------------------------------------------------------------------------------------
    
   

	if( !data[1].IsEmpty() )
	{
		if( sWhere.GetLength() > 1) sWhere += _T(" AND S.REC_KEY IN ( select distinct species_key from bo_book_tbl b where ");
		if(data[2].IsEmpty () )
		{
			sWhere += _T("( b.shelf_Date > to_date  ('") +data[1] + _T("' , 'yyyy/mm/dd' )) ")  ;
		}
		else
		{    
			sWhere += _T("(b.SHELF_DATE between to_date( ' ") +data[1]+ _T("', 'yyyy/mm/dd' ) and to_date( ' ") +data[2]+ _T("', 'yyyy/mm/dd' ) ))") ;
		}
	}
    
	if(!data[3].IsEmpty ())
	{
	    
        sWhere += _T(" AND  b.SHELF_LOC_CODE = ' ") + data[3] + _T("'") ;	
   
	}
    
    if (data[5].GetLength () > 0 ) {
        if (  data[4] == _T("등록번호") ) {
            CString sTemp;
			TCHAR *tempdata = _T("") ;
			tempdata = (TCHAR*)malloc(sizeof(TCHAR)*(data[5].GetLength ()+sizeof(TCHAR))) ;
			_tcscpy(tempdata,data[5].GetBuffer (0)) ;

            sWhere += _T(" AND ");
            sWhere += _T("s.rec_key in (select distinct species_key from bo_book_tbl b where b.reg_NO in ( '") + data[5] + _T("' ))") ;
        }
        else if ( data[4] == _T("제어번호") ) {
            sWhere +=  _T("and s.CONTROL_NO = '") + data[5] + _T("'") ;
        }
    }
    
    ids = m_pBaseDataMgr->RefreshDataManager(sWhere)  ;
    if(ids < 0) AfxMessageBox (_T("RefreshDataManager Error ")) ;
 
    if ( 0 == m_pBaseDataMgr->GetRowCount() ) AfxMessageBox (_T("조건에 맞는 자료가 없습니다."));
      
    ids = ShowStatus ( m_pBaseDataMgr->GetRowCount (),0 ) ;

	DisplayData () ;
   
	(CProgressCtrl *)GetDlgItem (IDC_PROGRESS_search)->ShowWindow(SW_HIDE) ;
	BtnEnable();

}


INT  CCENTER_UPLOAD_01::MakeAccessionNoWhere ( TCHAR *lpFieldName , TCHAR *lpValue , CString & sWhere , INT MaxArrayCount )
{
    TCHAR   *TokenStr = NULL;
	TCHAR    szTmp[128] , szAccessionNo[128];
	INT     DummyCount = 0 , AccessionNo = 0 , i = 0;
	INT     AccessionCount = 0;

	TCHAR   *mValue = (TCHAR *)malloc( sizeof(TCHAR) * _tcsclen(lpValue) +sizeof(TCHAR) );

	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//===================================================================
	_tcscpy ( mValue , lpValue );
	

	////////////////////////////////////////////////////////
	// 1. '+' , _T("-' 구분자를 체크한다.
	////////////////////////////////////////////////////////
	// 1.1 '+'  구분자를 체크한다.
    TokenStr = _tcstok( mValue, _T("+") );
	if ( _tcscmp ( TokenStr , lpValue ) !=0 ) {
        _tcscpy ( szAccessionNo , TokenStr );
		TokenStr = _tcstok( NULL, _T("+") );
		if ( TokenStr != NULL ) {
			DummyCount = _ttoi(TokenStr);
			if ( DummyCount > MaxArrayCount && MaxArrayCount > 0 ) {
				free ( mValue );
				return -100;
			}
		}
		if ( szAccessionNo[0] >= 97 && szAccessionNo[0] < 123 ) szAccessionNo[0] -= 32;
		if ( szAccessionNo[1] >= 97 && szAccessionNo[1] < 123 ) szAccessionNo[1] -= 32;

		if ( ( szAccessionNo[0] >= '0' && szAccessionNo[0] <= '9' ) &&
			 ( szAccessionNo[1] >= '0' && szAccessionNo[1] <= '9' ) )
		{
			_tcscpy( szTmp, szAccessionNo );
			AccessionNo = _ttoi( szAccessionNo );
		} else {			
		
			for( INT i = 0 ; i < nRegCodeLength ; i++)
			{
				szTmp[i] = szAccessionNo[i];
			}
			
			CString strRegNoTemp;
			strRegNoTemp.Format(_T("%d"),_ttoi( &szAccessionNo[nRegCodeLength] ));
			strRegNoTemp = m_pInfo->MakeRegNo(_T(""),strRegNoTemp);
			
			_stprintf( &szTmp[nRegCodeLength], _T("%s"), strRegNoTemp.Mid(nRegCodeLength) );
			AccessionNo = _ttoi( &szAccessionNo[nRegCodeLength] );
			//=====================================================
			
			
		}

		sWhere.Format( _T(" %s in (") , lpFieldName );
		sWhere += _T("'");
		sWhere +=  szTmp;
		sWhere += _T("'");

		for ( i = 0; i < DummyCount; i ++ ) 
		{
			if ( ( szAccessionNo[0] >= '0' && szAccessionNo[0] <= '9' ) &&
				 ( szAccessionNo[1] >= '0' && szAccessionNo[1] <= '9' ) )
			{
				_stprintf( szTmp, _T("%d"), AccessionNo +i+1  );
			} else 
			{
				CString strRegNoTemp;
				strRegNoTemp.Format(_T("%d"),AccessionNo +i+1);
				strRegNoTemp = m_pInfo->MakeRegNo(_T(""),strRegNoTemp);

				_stprintf( &szTmp[nRegCodeLength], _T("%s"), strRegNoTemp.Mid(nRegCodeLength)  );				
				//=====================================================
			}

			sWhere += _T(",");
			sWhere += _T("'");
			sWhere +=  szTmp;
			sWhere += _T("'");
		}
		sWhere += _T(")");
		free ( mValue );
		return 0;
	}

	// 1.2 '-'  구분자를 체크한다.
	TokenStr = NULL;
    TokenStr = _tcstok( mValue, _T("-") );
	if ( _tcscmp ( TokenStr , lpValue ) !=0 ) {
        _tcscpy ( szAccessionNo , TokenStr );
		TokenStr = _tcstok( NULL, _T("-") );
		if ( TokenStr != NULL ) {
			DummyCount = _ttoi(TokenStr);
			if ( DummyCount > MaxArrayCount && MaxArrayCount > 0 ) {
				free ( mValue );
				return -100;
			}
		}
		if ( szAccessionNo[0] >= 97 && szAccessionNo[0] < 123 ) szAccessionNo[0] -= 32;
		if ( szAccessionNo[1] >= 97 && szAccessionNo[1] < 123 ) szAccessionNo[1] -= 32;

		//  등록구분이 없는 등록번호 처리.
		if ( ( szAccessionNo[0] >= '0' && szAccessionNo[0] <= '9' ) &&
			 ( szAccessionNo[1] >= '0' && szAccessionNo[1] <= '9' ) )
		{
			_tcscpy( szTmp, szAccessionNo );
			AccessionNo = _ttoi( szAccessionNo );
		} else {

			for( INT i = 0 ; i < nRegCodeLength ; i++)
			{
				szTmp[i] = szAccessionNo[i];
			}
			
			CString strRegNoTemp;
			strRegNoTemp.Format(_T("%d"),_ttoi( &szAccessionNo[nRegCodeLength] ));
			strRegNoTemp = m_pInfo->MakeRegNo(_T(""),strRegNoTemp);
			
			_stprintf( &szTmp[nRegCodeLength], _T("%s"), strRegNoTemp.Mid(nRegCodeLength) );
			AccessionNo = _ttoi( &szAccessionNo[nRegCodeLength] );
			//=====================================================
		}

		sWhere.Format( _T(" %s in (") , lpFieldName );
		sWhere += _T("'");
		sWhere +=  szTmp;
		sWhere += _T("'");

		for ( i = 0; i < DummyCount; i ++ ) {
			if ( AccessionNo - i <= 0 ) break;
			if ( ( szAccessionNo[0] >= '0' && szAccessionNo[0] <= '9' ) &&
				 ( szAccessionNo[1] >= '0' && szAccessionNo[1] <= '9' ) )
			{
				_stprintf( szTmp, _T("%d"), AccessionNo - i -1 );
			} else {
				
				CString strRegNoTemp;
				strRegNoTemp.Format(_T("%d"),AccessionNo - i -1 );
				strRegNoTemp = m_pInfo->MakeRegNo(_T(""),strRegNoTemp);

				_stprintf( &szTmp[nRegCodeLength], _T("%s"), strRegNoTemp.Mid(nRegCodeLength)  );				
				//=====================================================
				
			}

			sWhere += _T(",");
			sWhere += _T("'");
			sWhere +=  szTmp;
			sWhere += _T("'");
		}
		sWhere += _T(")");
		free ( mValue );
		return 0;
	}

	////////////////////////////////////////////////////////
	// 2. ' '  구분자를 체크한다.
	////////////////////////////////////////////////////////
	TokenStr = NULL;
    TokenStr = _tcstok( mValue, _T(" ") );
	if ( _tcscmp ( TokenStr , lpValue ) !=0 ) {
		if ( TokenStr[0] >= 97 && TokenStr[0] < 123 ) TokenStr[0] -= 32;
		if ( TokenStr[1] >= 97 && TokenStr[1] < 123 ) TokenStr[1] -= 32;

		//  등록구분이 없는 등록번호 처리.
		if ( ( TokenStr[0] >= '0' && TokenStr[0] <= '9' ) &&
			 ( TokenStr[1] >= '0' && TokenStr[1] <= '9' ) )
		{
			_tcscpy( szTmp, TokenStr );
		} else {
		
			for( INT i = 0 ; i < nRegCodeLength ; i++)
			{
				szTmp[i] = TokenStr[i];
			}
			
			CString strRegNoTemp;
			strRegNoTemp.Format(_T("%d"),_ttoi( &TokenStr[nRegCodeLength] ));
			strRegNoTemp = m_pInfo->MakeRegNo(_T(""),strRegNoTemp);
			
			_stprintf( &szTmp[nRegCodeLength], _T("%s"), strRegNoTemp.Mid(nRegCodeLength) );
			//=====================================================
			
			szTmp[12] = 0;
		}

		sWhere.Format( _T(" %s in (") , lpFieldName );
		sWhere += _T("'");
		sWhere +=  szTmp;
		sWhere += _T("'");
		AccessionCount++;

		while ( 1 ) {
			TokenStr = _tcstok( NULL , _T(" ") );
			if ( TokenStr == NULL ) break;
			if ( AccessionCount > MaxArrayCount && MaxArrayCount > 0 ) {
				free ( mValue );
				return -100;
			}
			if ( NULL != TokenStr ) {
				if ( TokenStr[0] >= 97 && TokenStr[0] < 123 ) TokenStr[0] -= 32;
				if ( TokenStr[1] >= 97 && TokenStr[1] < 123 ) TokenStr[1] -= 32;

				//  등록구분이 없는 등록번호 처리.
				if ( ( TokenStr[0] >= '0' && TokenStr[0] <= '9' ) &&
					 ( TokenStr[1] >= '0' && TokenStr[1] <= '9' ) )
				{
					_stprintf( szTmp, _T("%s"), TokenStr );
				} else {
				
					for( INT i = 0 ; i < nRegCodeLength ; i++)
					{
						szTmp[i] = TokenStr[i];
					}
					
					CString strRegNoTemp;
					strRegNoTemp.Format(_T("%d"),_ttoi( &TokenStr[nRegCodeLength] ));
					strRegNoTemp = m_pInfo->MakeRegNo(_T(""),strRegNoTemp);
					
					_stprintf( &szTmp[nRegCodeLength], _T("%s"), strRegNoTemp.Mid(nRegCodeLength) );
					//=====================================================
					
					szTmp[12] = 0;
				}
				sWhere += _T(",");
				sWhere += _T("'");
				sWhere +=  szTmp;
				sWhere += _T("'");
				AccessionCount++;
			}
		}
		if ( AccessionCount > 0 ) {
			sWhere += _T(")");
		}
		free ( mValue );
		return 0;
	}
	if ( lpValue[0] >= 97 && lpValue[0] < 123 ) lpValue[0] -= 32;
	if ( lpValue[1] >= 97 && lpValue[1] < 123 ) lpValue[1] -= 32;

	//  등록구분이 없는 등록번호 처리.
	if ( ( TokenStr[0] >= '0' && TokenStr[0] <= '9' ) &&
		 ( TokenStr[1] >= '0' && TokenStr[1] <= '9' ) )
	{
		_tcscpy( szTmp, lpValue );
	} else {

		for( INT i = 0 ; i < nRegCodeLength ; i++)
		{
			szTmp[i] = lpValue[i];
		}
		
		CString strRegNoTemp;
		strRegNoTemp.Format(_T("%d"),_ttoi( &lpValue[nRegCodeLength] ));
		strRegNoTemp = m_pInfo->MakeRegNo(_T(""),strRegNoTemp);
		
		_stprintf( &szTmp[nRegCodeLength], _T("%s"), strRegNoTemp.Mid(nRegCodeLength) );
		
	}

	sWhere.Format( _T(" %s in ( '%s' )") , lpFieldName , szTmp );
	free ( mValue );

	return 0;
}


VOID CCENTER_UPLOAD_01::BtnEnable()
{

}
/*
VOID CCENTER_UPLOAD_01::OnbUPLOADALL() 
{
	// TODO: Add your control notification handler code here
	
	 CCENTER_UPLOAD_02 dlg( this, TRUE );
     if ( IDOK == dlg.DoModal() ) {
        DisplayData();
        BtnEnable();
    }


}
*/
INT CCENTER_UPLOAD_01::DisplayData( BOOL bAllRefresh )
{
    INT ids;
    if ( bAllRefresh ) {
        ids = AllControlDisplay( _T("CM_CENTERUPLOAD_01"), -1);
        if (ids<0) return -1;
    }

    CString frm;
    frm.Format( _T("전체 : %d 건"), m_pBaseDataMgr->GetRowCount() );
    GetDlgItem( IDC_STATIC_1 )->SetWindowText( frm );
    
    return 0;
}

/*
VOID CCENTER_UPLOAD_01::OnbUPLOADONE() 
{
	// TODO: Add your control notification handler code here
	CCENTER_UPLOAD_03 dlg( this, TRUE );
    dlg.DoModal();

    DisplayData();
    BtnEnable();
}
*/

VOID CCENTER_UPLOAD_01::OnbDETAILFIND() 
{
	// TODO: Add your control notification handler code here
	CCENTER_UPLOAD_04 dlg( this );
    dlg.DoModal();
    BtnEnable();
}


VOID CCENTER_UPLOAD_01::insert_data( VOID *pAppend_data_search, INT RowCount )
{   
	DB_DATA_ARRAY_TYPE_SEARCH *pAppend_data = (DB_DATA_ARRAY_TYPE_SEARCH*)pAppend_data_search;

    CString temp_m_szDataMgrAlias;
    temp_m_szDataMgrAlias.Format (_T("%s"),_T("DM_Test"));

	CESL_DataMgr *pDM = FindDM(temp_m_szDataMgrAlias);
	if (pDM == NULL) return;

	pDM->FreeData();

	INT ColumnCount = pDM->RefList.GetCount();
	INT row, col;
	TCHAR *cell;
	CString strData;	
	
	for (row = 0; row < RowCount; row++) {
		pDM->InsertRow(-1);
		for (col = 0; col < ColumnCount; col++) {
			cell = NULL;
			cell = pAppend_data[col].db_data[row].stringData;
			if (cell) {
				strData.Format(_T("%s"), cell);
				pDM->SetCellData(row, col, strData);
			}
		}
	}
	return;
}


INT CCENTER_UPLOAD_01::ShowStatus( INT iSelectedCnt, INT iFinishedCnt )
{

   return 0 ;
}


VOID CCENTER_UPLOAD_01::OnSearch (CESL_Mgr *pMgr)
{
    INT ids;
	INT	i;
    CString sWhere;

	CString strTmpBookWhere = _T("");	

    // 컨트롤의 데이타를 읽어온다.
    const alias_cnt = 14;
    CString  alias[alias_cnt] = {
        _T("UPLOAD상태"), _T("배가일자-부터"), _T("배가일자-까지"), _T("배가위치"),
        _T("번호구분"),   _T("번호부터"), _T("번호까지"), _T("원부작성년도"), _T("원부번호부터"), _T("원부번호까지"),
		_T("차수작성년도"), _T("업무구분"), _T("차수번호부터"), _T("차수번호까지")
    };
     
	// KOL.UDF.022 시큐어코딩 보완
    //CString data[13];
	CString data[alias_cnt];

    CESL_ControlMgr *pCM = pMgr->FindCM(_T("CM_CENTERUPLOAD_SEARCH")) ;	
    
	for( i = 0 ; i < alias_cnt ; i ++) 
	{
       ids = pCM->GetControlMgrData (alias[i],data[i],-1,0) ;
	   data[i].TrimLeft() ;
	   data[i].TrimRight () ;

	   if(ids <0 ) AfxMessageBox (_T("GetControlMgrData() Error")) ;
	}
	
    

    // 전체자료에서 찾는경우 다른 조건을 더 입력해야한다.
    if (  data[0] == _T("전체") ) 
	{
        if( data[7].IsEmpty() == FALSE || data[8].IsEmpty() == FALSE || data[9].IsEmpty() == FALSE )
		{
			;
		}
		else
		{
			if ((  data[1].IsEmpty () || data[2].IsEmpty () ) ) {
				AfxMessageBox (_T("전체자료에서 검색시 배가일자 조건을 모두 입력해야 합니다."));
		  		return ;
			}
		}
		
    }
	 
	if( data[7].IsEmpty() == FALSE || data[8].IsEmpty() == FALSE || data[9].IsEmpty() == FALSE )
	{
		if( data[7].IsEmpty() )
		{
			AfxMessageBox(_T("원부년도를 입력해주세요"));
			return;
		}
	}
	else
	{
		if ((  data[1].IsEmpty () && data[2].IsEmpty () ) ) {
			AfxMessageBox (_T("검색시 배가일자 조건을 입력해야 합니다."));
			return ;
		}
	}
    

    if ( data[0] == _T("전체") ) sWhere = _T("1=1 ");   
    else if (  data[0] == _T("안올린자료") ) sWhere = _T("( s.CENTER_UPLOAD_STATUS ='1' OR s.CENTER_UPLOAD_STATUS IS NULL) ");
    else if (  data[0] == _T("안올린자료(확인요망)")) sWhere = _T("s.CENTER_UPLOAD_STATUS = '2' ");
    else if (  data[0] == _T("안올린자료(MARC에러)") ) sWhere = _T("s.CENTER_UPLOAD_STATUS = '3' ");
    else if (  data[0] == _T("안올린자료(기타)")) sWhere = _T("s.CENTER_UPLOAD_STATUS = '4' ");
	else if (  data[0] == _T("올린자료") ) sWhere = _T("s.CENTER_UPLOAD_STATUS = '0' ");
	else if (  data[0] == _T("임시 DB로 올린자료")) sWhere = _T("s.CENTER_UPLOAD_STATUS = '5' ");
	else if (  data[0] == _T("반출 후 국중에 제공한 자료")) sWhere = _T("s.CENTER_UPLOAD_STATUS = '6' ");

	if( !data[1].IsEmpty() )
	{
		if( sWhere.GetLength() > 1) sWhere += _T(" AND S.REC_KEY IN ( select distinct species_key from bo_book_tbl b where ");
		if(data[2].IsEmpty () )
		{
			if(!data[3].IsEmpty ())
			{
			sWhere += _T(" b.SHELF_DATE between to_date( '") +data[1]+ _T("', 'yyyy/mm/dd' ) and to_date( '") +data[1]+ _T("', 'yyyy/mm/dd' ) ") ;
			sWhere += _T(" AND  b.SHELF_LOC_CODE = '") + data[3] + _T("')") ;	
            strTmpBookWhere += _T(" b.SHELF_DATE between to_date( '") +data[1]+ _T("', 'yyyy/mm/dd' ) and to_date( '") +data[1]+ _T("', 'yyyy/mm/dd' ) ") ;
			strTmpBookWhere += _T(" AND b.SHELF_LOC_CODE = ' ") + data[3] + _T("')") ;
			}
			else
			{             
                sWhere += _T(" b.SHELF_DATE between to_date( '") +data[1]+ _T("', 'yyyy/mm/dd' ) and to_date( '") +data[1]+ _T("', 'yyyy/mm/dd' ) )") ;
				
			    strTmpBookWhere += _T("b.SHELF_DATE between to_date( '") +data[1]+ _T("', 'yyyy/mm/dd' ) and to_date( '") +data[1]+ _T("', 'yyyy/mm/dd' )  ") ;

			}
		}
		else
		{    
			
			if(!data[3].IsEmpty ())
			{
                 sWhere += _T(" b.SHELF_DATE between to_date( '") +data[1]+ _T("', 'yyyy/mm/dd' ) and to_date( '") +data[2]+ _T("', 'yyyy/mm/dd' ) ") ;
			     sWhere += _T(" AND  b.SHELF_LOC_CODE = '") + data[3] + _T("')") ;	

                
				 strTmpBookWhere += _T("b.SHELF_DATE between to_date( '") +data[1]+ _T("', 'yyyy/mm/dd' ) and to_date( '") +data[2]+ _T("', 'yyyy/mm/dd' ) ") ;
                 strTmpBookWhere += _T(" AND  b.SHELF_LOC_CODE = '") + data[3] + _T("')") ;	
			}
			else
			{
               	sWhere += _T("b.SHELF_DATE between to_date( '") +data[1]+ _T("', 'yyyy/mm/dd' ) and to_date( '") +data[2]+ _T("', 'yyyy/mm/dd' ) )") ;
				
                strTmpBookWhere += _T("b.SHELF_DATE between to_date( '") +data[1]+ _T("', 'yyyy/mm/dd' ) and to_date( '") +data[2]+ _T("', 'yyyy/mm/dd' ) ") ;
			}
		}
       
	}
    else
    {
		if(!data[3].IsEmpty ())
		{
			sWhere += _T(" AND S.REC_KEY IN ( select distinct species_key from bo_book_tbl b where ");
			sWhere += _T(" b.SHELF_LOC_CODE = '") + data[3] + _T("')") ;
			
			strTmpBookWhere += _T("and b.SHELF_LOC_CODE = '") + data[3] + _T("')") ;
   
		}
    }

    if(data[5].GetLength ()>12 || data[6].GetLength ()>12)
	{
       AfxMessageBox (_T("[등록번호 OR 제어번호] 길이를 확인하여 주십시요.")) ;
	}
	

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
    if (data[5].GetLength () > 0) {
        if (  data[4] == _T("등록번호") ) {
            CString sTemp;
			

			
			if ( data[5].GetLength() < 12 ) {
				

				if ( data[5].GetLength() > nRegCodeLength ) 
				{
					data[5] = m_pInfo->MakeRegNo( data[5].Left(nRegCodeLength), data[5].Mid(nRegCodeLength) );
				}
				
			}

			if ( !data[6].IsEmpty() ) 
			{
				if ( data[6].GetLength() < 12 ) {
				

					if ( data[6].GetLength() > nRegCodeLength ) 
					{
						data[6] = m_pInfo->MakeRegNo( data[6].Left(nRegCodeLength), data[6].Mid(nRegCodeLength) );
					}
				
				}
				sTemp = _T("b.reg_NO >= '") + data[5] + _T("' AND b.reg_NO <= '") + data[6] +_T("'");
			}
			else 
			{
				sTemp = _T("b.reg_NO = '") + data[5] + _T("'");
			}

            if (ids) {
                if ( ids == -100 ) AfxMessageBox (_T("등록번호는 100개까지만 검색이 가능합니다."));     
            }
       

            sWhere += _T(" AND ");
			sWhere += _T("s.rec_key in (select distinct species_key from bo_book_tbl b where ")+sTemp+_T(")") ;
			
            strTmpBookWhere += _T(" and ") + sTemp ;

        }
        else if ( data[4] == _T("제어번호") ) {
			if ( data[5].GetLength() < 12 ) {
			

				if ( data[5].GetLength() > nRegCodeLength ) 
				{
					data[5] = m_pInfo->MakeRegNo( data[5].Left(nRegCodeLength), data[5].Mid(nRegCodeLength) );
				}
			
				
			}

			if ( !data[6].IsEmpty() ) 
			{
				if ( data[6].GetLength() < 12 ) {
			

					if ( data[5].GetLength() > nRegCodeLength ) 
					{
						data[5] = m_pInfo->MakeRegNo( data[5].Left(nRegCodeLength), data[5].Mid(nRegCodeLength) );
					}
					
					
				}

				sWhere +=  _T("and s.CONTROL_NO >= '") + data[5] + _T("' and s.CONTROL_NO <= '") + data[6] + _T("'");
			}
			else
			{
				sWhere +=  _T("and s.CONTROL_NO = '") + data[5] + _T("'") ;
			}
            
        }
    }
    

	//7 원부작성년도
	//8,9 원부번호시작, 끝
	if ( data[7].GetLength () > 0 ) 
	{
		CString strTmp;
        if ( data[8].IsEmpty() && data[9].IsEmpty() ) 
		{
			//원부년도가 입력은 됐는데 원부번호들이 모두 입력이 안되었을때...
			AfxMessageBox (_T("원부번호를 입력해 주세요")) ;
			return;
		}

		//원부년도가 입력은 됐는데 원부번호가 시작만 입력이 되었을때..
		if ( !data[8].IsEmpty() && data[9].IsEmpty() ) 
		{	
			strTmp.Format ( _T("AND s.rec_key in ( select distinct b.species_key from BO_BOOK_TBL B, CO_ACCESSION_REC_TBL A WHERE B.ACCESSION_REC_KEY = A.REC_KEY(+) AND accession_rec_no = %s )"), data[8] );
			sWhere += strTmp;
		}

		//원부년도가 입력은 됐는데 원부번호가 모두
		if ( !data[8].IsEmpty() && !data[9].IsEmpty() ) 
		{
			strTmp.Format ( _T("AND s.rec_key in ( select distinct b.species_key from BO_BOOK_TBL B, CO_ACCESSION_REC_TBL A WHERE B.ACCESSION_REC_KEY = A.REC_KEY(+) AND accession_rec_no >= %s AND accession_rec_no <= %s )"), data[8], data[9] );
			sWhere += strTmp;
		}

		//원부년도가 입력은 됐는데 원부번호가 끝만
		if ( data[8].IsEmpty() && !data[9].IsEmpty() ) 
		{
			strTmp.Format ( _T("AND s.rec_key in ( select distinct b.species_key from BO_BOOK_TBL B, CO_ACCESSION_REC_TBL A WHERE B.ACCESSION_REC_KEY = A.REC_KEY(+) AND accession_rec_no = %s  )"), data[9] );
			sWhere += strTmp;
		}
	}

			// 10              11            12              13
	//_T("차수작성년도"), _T("업무구분"), _T("차수번호부터"), _T("차수번호까지")
	if ( data[10].GetLength() > 0 && (data[12].GetLength() > 0 || data[13].GetLength() > 0)) 
	{
		CString strTmp;        

		//차수년도가 입력은 됐는데 차수번호가 시작만 입력이 되었을때..
		if ( !data[10].IsEmpty() && data[12].IsEmpty() ) 
		{	
			if( data[11].IsEmpty() )
				strTmp.Format ( _T("AND s.rec_key in ( select distinct b.species_key from BO_BOOK_TBL B, BO_PURCHASE_SEQ_NO_TBL p WHERE b.seq_no = %s and b.seq_no = p.seq_no and to_char(p.SEQ_NO_CREATE_DATE, 'YYYY') = '%s' )"), data[12], data[10]);
			else
				strTmp.Format ( _T("AND s.rec_key in ( select distinct b.species_key from BO_BOOK_TBL B, BO_PURCHASE_SEQ_NO_TBL p WHERE b.seq_no = %s and b.seq_no = p.seq_no and to_char(p.SEQ_NO_CREATE_DATE, 'YYYY') = '%s' and i.work_code = '%s' )"), data[12], data[10], data[11]);
			sWhere += strTmp;
		}

		//차수년도가 입력은 됐는데 차수번호가 끝만 입력이 되었을때..
		else if ( !data[10].IsEmpty() && data[13].IsEmpty() ) 
		{	
			if( data[11].IsEmpty() )
				strTmp.Format ( _T("AND s.rec_key in ( select distinct b.species_key from BO_BOOK_TBL B, BO_PURCHASE_SEQ_NO_TBL p WHERE b.seq_no <= %s and b.seq_no = p.seq_no and to_char(p.SEQ_NO_CREATE_DATE, 'YYYY') = '%s' )"), data[12], data[10]);
			else
				strTmp.Format ( _T("AND s.rec_key in ( select distinct b.species_key from BO_BOOK_TBL B, BO_PURCHASE_SEQ_NO_TBL p WHERE b.seq_no <= %s and b.seq_no = p.seq_no and to_char(p.SEQ_NO_CREATE_DATE, 'YYYY') = '%s' and i.work_code = '%s' )"), data[12], data[10], data[11]);
			sWhere += strTmp;
		}

		//차수년도가 입력은 됐는데 차수번호가 시작만 입력이 되었을때..
		else if ( !data[10].IsEmpty() && !data[12].IsEmpty() && !data[13].IsEmpty() ) 
		{
			if( data[11].IsEmpty() )
				strTmp.Format ( _T("AND s.rec_key in ( select distinct b.species_key from BO_BOOK_TBL B, BO_PURCHASE_SEQ_NO_TBL p WHERE b.seq_no >= %s and b.seq_no <= %s and b.seq_no = p.seq_no and to_char(p.SEQ_NO_CREATE_DATE, 'YYYY') = '%s' )"), data[12], data[13], data[10]);
			else
				strTmp.Format ( _T("AND s.rec_key in ( select distinct b.species_key from BO_BOOK_TBL B, BO_PURCHASE_SEQ_NO_TBL p WHERE b.seq_no >= %s and b.seq_no <= %s and b.seq_no = p.seq_no and to_char(p.SEQ_NO_CREATE_DATE, 'YYYY') = '%s' and i.work_code = '%s' )"), data[12], data[13], data[10], data[11]);
			sWhere += strTmp;
		}
	}
	else if ( data[10].IsEmpty() && data[12].IsEmpty() && data[13].IsEmpty() ) 
	{}
	else
	{
		//차수번호를 이용하여 검색할때는 모든 차수 조건을 입력하여야 한다.
		AfxMessageBox (_T("차수에 관련된 모든 조건을 입력해 주세요. [작성년도, 차수번호]")) ;
		return;		
	}
	
	//=========================================================
	//
	if (  !sWhere.IsEmpty() )
	{
		sWhere += _T(" AND I.MANAGE_CODE = UDF_MANAGE_CODE") ;
	}
	else 
	{
		sWhere = _T( "I.MANAGE_CODE = UDF_MANAGE_CODE" );
	}
	//=========================================================

	m_sWhere = sWhere ;

    ids = m_pBaseDataMgr->RefreshDataManager(sWhere)  ;
    if(ids < 0) AfxMessageBox (_T("RefreshDataManager Error ")) ;
     
	if ( 0 == m_pBaseDataMgr->GetRowCount() ) AfxMessageBox (_T("조건에 맞는 자료가 없습니다."));
 
   
    CString temp ;
	CString value ;
    for(i = 0 ; i < m_pBaseDataMgr->GetRowCount () ; i ++) {

		   ids = m_pBaseDataMgr->GetCellData (_T("UPLOAD상태"),i, temp ) ;
            
		   INT m_dir ; 

		   if(temp.IsEmpty ())
		   { 
			  
			   m_dir = 1 ;
			   //=================================
		   }
			
		   else {
		       m_dir = _ttoi(temp.GetBuffer (0)) ;
		   }
			
	
		   switch(m_dir) {
		        case 0 :
                    value = _T("올린자료") ;
			    	break;
			    
				case  1 : 
				    value = _T("안올린자료") ;
			    	break;
				case 2 :
				   value = _T("안올린자료(확인요망)");
			    	break;
				case 3 :
				   value = _T("안올린자료(MARC에러)") ;
				   break;
				case 4 :
				    value = _T("안올린자료(기타)") ;

				    break ;
				case 5 :

					value = _T("임시 DB로 올린자료") ;
			    	break;
				case 6 : 
					value = _T("반출 후 국중에 제공한 자료");
					break;
		   }			
		   // --------------------------------------------------------------------------------------
		   ids = m_pBaseDataMgr->SetCellData (_T("UPLOAD상태"),value ,i) ;

	   }
    
	CString 
	m_ParentCM = _T("CM_CENTERUPLOAD_01") ;
    AllControlDisplay (m_ParentCM ,-1) ;
    
    INT nTotalCnt ;
	nTotalCnt = m_pBaseDataMgr->GetRowCount () ;
	CString str ;
	str.Format (_T("%d 건 검색 되었습니다."),nTotalCnt);
	static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_SET_STATUS_TEXT, NULL, (long)str.GetBuffer(0));
 
}


INT CCENTER_UPLOAD_01::CopyToCenterFromCat (INT nDir)
{
	CESL_DataMgr *pCenterDM ;
	CESL_DataMgr *pCatDM ;
   
    CESL_ControlMgr *pCM = FindCM(_T("CM_CENTERUPLOAD_01")) ;
	if(pCM == NULL) AfxMessageBox (_T("FindCM() Error~!")) ;

    CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl (_T("MainGrid")) ;


	pCenterDM = FindDM(_T("DM_CENTERUPLOAD_01")) ;
	if(pCenterDM == NULL) {
		AfxMessageBox (_T("FindDM() Error~!")) ;
        return -1 ;
	}

	pCatDM = FindDM(_T("DM_BO_CAT_PROC_0000")) ;
    if(pCenterDM == NULL) {
	    AfxMessageBox (_T("FindDM() Error~!")) ;
        return -1 ;
	}
      
    

	pCatDM->FreeData () ;
	INT nRowCount = pCenterDM->GetRowCount () ;
    
	

	CString Result ;
     
	CString Cat_Alias [19] = { _T("IBS_저작자"),_T("IBS_제어자료구분_코드"),_T("IBS_형식구분_코드"), 
		                        _T("IBS_관리구분_코드")  ,_T("IBS_자료구분_코드"),_T("IBS_매체구분_코드"),
                                _T("IBS_발행자"),   _T("IBS_발행년도"), _T("BS_종KEY"), _T("IBS_본표제"), 
                                _T("IBS_이용제한구분_코드"), _T("IBS_이용대상구분_코드"), _T("IBS_청구_권책기호"), 
                                _T("IBS_작업상태INFO"), _T("BS_수입년도"), _T("BS_MARC"),  _T("BS_자료상태_코드"),   
                                _T("끝등록번호"), _T("시작등록번호") } ;
	
	
	CString Center_Alias[19] = { _T("IBS_저작자"),_T("IBS_제어자료구분_코드"),_T("형식구분"),
                                 _T("관리구분"),_T("자료구분"),_T("매체구분"),
								 _T("IBS_발행자"), _T("IBS_발행년도"),_T("REC_KEY"),_T("IBS_본표제"),
								 _T("이용제한구분"),_T("이용대상구분"),_T("IBS_청구_권책기호"),
                                 _T("IBS_작업상태INFO"), _T("BS_수입년도"), _T("MARC"),  _T("BS_자료상태_코드"),   
                                 _T("끝등록번호"), _T("시작등록번호") } ;



   
    CString temp ;

	for(INT i = 0 ; i < nRowCount ; i++)
	{   
	  
		   pCatDM->InsertRow (-1) ;
		   for(INT j =0 ;j <19 ;j++)
		   {
			   pCenterDM->GetCellData (Center_Alias[j],i,Result) ;
			   pCatDM->SetCellData (Cat_Alias[j],Result,i) ;
		   }	   
	}

	return 0 ;

}


INT CCENTER_UPLOAD_01::CopyToCatFromCenter (INT n) 
{   
	CESL_DataMgr *pCenterDM ;
	CESL_DataMgr *pCatDM ;
     

    CString AddWhere ;
    AddWhere.Format (_T("S.rec_key = I.rec_key AND %s"),m_sWhere) ;

	CESL_DataMgr *whereDM = FindDM(_T("DM_BO_CAT_PROC_WHERE")) ;
	whereDM->FreeData () ;
	whereDM->InsertRow (-1) ;
	whereDM->SetCellData (_T("조건절"),AddWhere,0) ;
    
  


	pCenterDM = FindDM(_T("DM_CENTERUPLOAD_01")) ;
	if(pCenterDM == NULL) {
		AfxMessageBox (_T("FindDM() Error~!")) ;
        return -1 ;
	}

	pCatDM = FindDM(_T("DM_BO_CAT_PROC_0000")) ;
    if(pCenterDM == NULL) {
	    AfxMessageBox (_T("FindDM() Error~!")) ;
        return -1 ;
	}
  
	
    pCatDM->RefreshDataManager (AddWhere) ;


	CString Result ;
     
	CString Cat_Alias [19] = { _T("IBS_저작자"),_T("IBS_제어자료구분_코드"),_T("IBS_형식구분_코드"), 
		                        _T("IBS_관리구분_코드")  ,_T("IBS_자료구분_코드"),_T("IBS_매체구분_코드"),
                                _T("IBS_발행자"),   _T("IBS_발행년도"), _T("BS_종KEY"), _T("IBS_본표제"), 
                                _T("IBS_이용제한구분_코드"), _T("IBS_이용대상구분_코드"), _T("IBS_청구_권책기호"), 
                                _T("IBS_작업상태INFO"), _T("BS_수입년도"), _T("BS_MARC"),  _T("BS_자료상태_코드"),   
                                _T("끝등록번호"), _T("시작등록번호") } ;
	
	
	CString Center_Alias[19] = { _T("IBS_저작자"),_T("IBS_제어자료구분_코드"),_T("형식구분"),
                                 _T("관리구분"),_T("자료구분"),_T("매체구분"),
								 _T("IBS_발행자"), _T("IBS_발행년도"),_T("REC_KEY"),_T("IBS_본표제"),
								 _T("이용제한구분"),_T("이용대상구분"),_T("IBS_청구_권책기호"),
                                 _T("IBS_작업상태INFO"), _T("BS_수입년도"), _T("MARC"),  _T("BS_자료상태_코드"),   
                                 _T("끝등록번호"), _T("시작등록번호") } ;
   
    CString str_IDX ;
	INT ids ;
    INT nRowCount = pCatDM->GetRowCount () ;
	for(INT i = 0 ; i < nRowCount ; i++)
	{   
	     for(INT j = 0 ; j <19 ;j++) 
		 {
		   ids = pCatDM ->GetCellData (Cat_Alias[j],i,Result) ;
		   ids= pCenterDM->SetCellData (Center_Alias[j],Result,i) ;
		 }
	}

	return 0 ;

}


INT CCENTER_UPLOAD_01::ShowModifyDlg ()
{

	CESL_DataMgr *m_pDM = FindDM(_T("DM_CENTERUPLOAD_01")) ;
	if(m_pDM == NULL) 
	{
		AfxMessageBox (_T("FindDM(DM_CENTERUPLOAD_01) Error~!")) ;
		return -1;
	}
    
    CESL_ControlMgr *m_pCM = FindCM(_T("CM_CENTERUPLOAD_01")) ;
	if(m_pCM == NULL)
	{
		AfxMessageBox(_T("FindCM(CM_CENTERUPLOAD_01) Error~!")) ;
		return -2 ;
	}

	CESL_Grid* m_pGrid = (CESL_Grid*)m_pCM->FindControl (_T("MainGrid")) ;
    if(m_pCM == NULL)
	{
		AfxMessageBox(_T("FindControl(MainGrid) Error~!")) ;
		return -2 ;
	}

	//===================================================
	//2009.04.08 ADD BY PJW : 그리드 수정을 할경우 선정된자료가 있을경우 현재커서를 선정에 추가한다.
	m_pGrid->SelectMakeList();
	if( m_pGrid->SelectGetCount() > 0 )
	{
		m_pGrid->SetAt( m_pGrid->GetIdx() , 0 , _T("V") );
		m_pGrid->SelectMakeList();
	}
	//===================================================
    

	CString strTmpBookWhere;
	strTmpBookWhere = strBookWhere;

	strTmpBookWhere += _T(" B.WORKING_STATUS IN ('BOL112N','BOL113O','BOL114O','BOL115O','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL511O','BOL611O')") ;
	
    INT nn = m_pGrid->GetRow() - 1;
	
    m_MarcAdjLaunCher.SetParentInfo(m_pDM, m_pGrid, _T("REC_KEY"), strTmpBookWhere, m_pGrid->GetRow() - 1, TRUE);

    m_MarcAdjLaunCher.LaunchMarcAdjust(GetParentFrame(), this, NULL, MNG_MODIFY);
  
	return 0 ;
}

HBRUSH CCENTER_UPLOAD_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;

}

BEGIN_EVENTSINK_MAP(CCENTER_UPLOAD_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CCENTER_UPLOAD_01)
	ON_EVENT(CCENTER_UPLOAD_01, IDC_MainGrid, -601 /* DblClick */, OnDblClickMainGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCENTER_UPLOAD_01::OnDblClickMainGrid() 
{
	CESL_ControlMgr* pCM = FindCM(_T("CM_CENTERUPLOAD_01")) ;
	if(pCM == NULL)
	{
		AfxMessageBox(_T("FindCM(CM_CENTERUPLOAD_01) Error~!")) ;
		return ;
	}

	CESL_Grid* pGrid = (CESL_Grid*)pCM->FindControl (_T("MainGrid")) ;
    if(pGrid == NULL)
	{
		AfxMessageBox(_T("FindControl(MainGrid) Error~!")) ;
		return ;
	}

	if (pGrid->GetMouseRow() == 0) 
	{
		return;
	}

	ShowModifyDlg();
	
}
