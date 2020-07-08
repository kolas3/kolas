// BL_BO_LOC_34.cpp: implementation of the CBL_BO_LOC_34 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BL_BO_LOC_34.h"
#include "direct.h"
// SBL
#include "SBL.h"
// 2011.08.26 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBL_BO_LOC_34::CBL_BO_LOC_34( CESL_Mgr *pWnd , CLonaManageValue* pLoanManageValue )
	: CSPF_Work(pWnd)
{
	m_pLoanManageValue = pLoanManageValue;

	m_pParent = pWnd;
}

CBL_BO_LOC_34::~CBL_BO_LOC_34()
{

}

INT CBL_BO_LOC_34::GetSelectMainGridCnt()
{
	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetSelectMainGridCnt") );

	if( pDM->GetRowCount() < 1 ) return 0;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetSelectMainGridCnt") );

	pGrid->SelectMakeList();

	if( pGrid->SelectGetCount() < 1 ) return 0;

	return pGrid->SelectGetCount();
}

CESL_Grid* CBL_BO_LOC_34::GetMainGridPtr()
{
	CString sCMAlias = _T("CM_BO_LOC_3400");
	CString sGridAlias = _T("grid1");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) return NULL;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) return NULL;
	
	return pGrid;
}


INT CBL_BO_LOC_34::SPFExcutePath( CString sPathAlias )
{
	INT ids = -1;

	if( sPathAlias.Compare( SEARCH ) == 0 )
	{
		ids = SearchBO_LOC_3410();
	}
	if( sPathAlias.Compare( RFID_SEARCH ) == 0 )
	{
		ids = SearchBO_LOC_3410FromRFID();
	}
	else if( sPathAlias.Compare( INSERT_DELIVERY ) == 0 )
	{
		ids = InsertDelivery();
	}
	else if( sPathAlias.Compare( UPDATE_DELIVERY ) == 0 )
	{
		ids = UpdateDelivery();
	}
	else if( sPathAlias.Compare( RECEIPT ) == 0 )
	{
		ids = Receipt();
	}
	else if( sPathAlias.Compare( DELIVERY ) == 0 )
	{
		ids = Delivery();
	}
	else if( sPathAlias.Compare( DELETE_DELIVERY ) == 0 )
	{
		ids = DeleteDelivery();
	}
	if( ids < 0 ) 
		AfxMessageBox(_T("기능을 정상적으로 수행하지 못하였습니다.") );

	return ids;
}


/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-08 ~ 2005-12-08
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Sort
 	 
   [   함수설명   ] 
      배달자료관리 검색 업무
   [  PARAMETERS  ]
      NULL 

   [ RETURN VALUE ]
      1    : 취소
      0    : 성공 
      음수 : 실패

   [   미니스펙   ]
      1. SQL문 생성
	  2. 검색결과 DM을 만든다.	  
	  3. 화면에 보여주기 
--------------------------------------------------------------------------------------------*/
INT CBL_BO_LOC_34::SearchBO_LOC_3410()
{
	BEGIN_SBL();

	INT ids;

	// ----------------------------------------------------------------------------
	// 1. SQL문 생성     
	// ----------------------------------------------------------------------------
	CString sWhere;
	ids = GetWhereBO_LOC_3410( sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BO_LOC_34::SearchBO_LOC_3410()") );
	if( ids > 0 ) return 1;

	// ----------------------------------------------------------------------------
	// 2. 검색결과 DM을 만든다.    
	// ----------------------------------------------------------------------------
	ids = MakeDMFromDB( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BO_LOC_34::SearchBO_LOC_3410()") );
	
	// ----------------------------------------------------------------------------
	// 3. 화면에 보여주기     
	// ----------------------------------------------------------------------------
	ids = GridView();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BO_LOC_34::SearchBO_LOC_3410()") );

	CESL_DataMgr *pSBLDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3400") );
	INT nRowCount = pSBLDM->GetRowCount();
	END_SBL(nRowCount);

	return 0;
}


/// 검색 PATH
/// 1. SQL문 생성
/// 2. 단행 자료 get
/// 2.1.단행 자료 Sum 기본 관리 DM
/// 3. 연속 자료 get
/// 3.1 연속 자료 Sum 기본 관리 DM
/// 4. 화면에 보여주기
INT CBL_BO_LOC_34::SearchBO_LOC_3410FromRFID()
{
	BEGIN_SBL();

	INT ids;
	CString sWhere;

	GetTempData( _T("WHERE") , sWhere );
	
	ids = MakeDMFromDB( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchBO_LOC_3410") );
	
	ids = GridView();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchBO_LOC_3410") );
	
	CESL_DataMgr *pSBLDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3400") );
	INT nRowCount = pSBLDM->GetRowCount();
	END_SBL(nRowCount);
	
	return 0;
}

/// SQL 문 생성
INT CBL_BO_LOC_34::GetWhereBO_LOC_3410( CString &sWhere )
{
	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3410");

	const INT nControlCnt = 3;
	TCHAR* sControlAlias[nControlCnt] =
	{
		_T("상태") , _T("일자1") , _T("일자2")
	};

	//++2008.04.15 UPDATE BY PWR {{+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 날짜컨트롤로 변경
	//
	// 상태값 받아오기
	CString sGetData[nControlCnt];
	ids = m_pParentMgr->GetControlData( sCMAlias, sControlAlias[0], sGetData[0], -1, 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetWhereBO_LOC_3410") );
	sGetData[0].TrimLeft(); sGetData[0].TrimRight();
	//
	// (TO)일자1값 받아오기
	CESL_ControlMgr* pCM = m_pParentMgr->FindCM( sCMAlias );
	if( !pCM ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("GetWhereBO_LOC_3410") );
	CESL_Control_Element *pCETo = (CESL_Control_Element*)pCM->FindControlElement( sControlAlias[1] );
	CDateTimeCtrl *pDateTo = ( CDateTimeCtrl * )m_pParentMgr->GetDlgItem( pCETo->CTRL_ID );
	SYSTEMTIME tTime;
	DWORD dwResult = pDateTo->GetTime( &tTime );
	if (dwResult != GDT_VALID )
	{
		sGetData[1] = _T("");
	}
	else
	{
		sGetData[1].Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	}
	//
	// (FROM)일자2값 받아오기
	CESL_Control_Element *pCEFrom = (CESL_Control_Element*)pCM->FindControlElement( sControlAlias[2] );
	CDateTimeCtrl *pDateFrom = ( CDateTimeCtrl * )m_pParentMgr->GetDlgItem( pCEFrom->CTRL_ID );
	dwResult = pDateFrom->GetTime( &tTime );
	if (dwResult != GDT_VALID )
	{
		sGetData[2] = _T("");
	}
	else
	{
		sGetData[2].Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	}
	//
	/// 수정전내용 
	/*CString sGetData[nControlCnt];
	for( INT i = 0 ; i < nControlCnt ; i++ )
	{
		ids = m_pParentMgr->GetControlData( sCMAlias , sControlAlias[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , "GetWhereBO_LOC_3410" );
		sGetData[i].TrimLeft(); sGetData[i].TrimRight();
	}*/
	//--2008.04.15 UPDATE BY PWR -----------------------------------------------------------------------}}

	INT nType = -1; // 전체

	if( sGetData[0].Compare( _T("신청자료") ) == 0 )
	{
		sWhere += _T(" D.STATUS = '0' ");
		nType = 0;

	}
	else if( sGetData[0].Compare( _T("접수완료") ) == 0 )
	{
		sWhere += _T(" D.STATUS = '1' ");
		nType = 1;

	}
	else if( sGetData[0].Compare( _T("배달완료") ) == 0 )
	{
		sWhere += _T(" D.STATUS = '2' ");
		nType = 2;
	}

	// 2007.08.15 add by pdw
	// 날짜조건을 필수로 변경한다.
	if( sGetData[1].IsEmpty() ) 
	{
		if( sGetData[0].Compare(_T("전체")) == 0 )
		{
			sGetData[0] = _T("신청");
		}

		CString strMsg;
		strMsg.Format( _T("%s일자를 입력해야 합니다."), sGetData[0].Mid(0,4) );
		AfxMessageBox(strMsg, MB_ICONSTOP);
		return 1;
	}

	TCHAR* sFieldName[3] =
	{
		_T("D.REQUEST_DATE"), _T("D.RECEIPT_DATE") , _T("D.DELIVERY_DATE")
	};

	if ( sGetData[1][0] != '\0' || sGetData[2][0] != '\0' )
	{
		if( !sWhere.IsEmpty() ) sWhere += _T(" AND ");
		if ( nType == -1 ) sWhere += _T("( ");
	}
	
	for( INT i = 0 ; i < 3 ; i++ )
	{
		if( ( nType == -1 ) || ( nType == i ) )
		{			
			if( sGetData[2].IsEmpty() )
				sWhere += CString(sFieldName[i]) + CString(_T(" BETWEEN TO_DATE('"))+sGetData[1]+CString(_T(" 00:00:00' , 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('"))+sGetData[1]+CString(_T(" 23:59:59' , 'YYYY/MM/DD HH24:MI:SS')")); 
			else
				sWhere += CString(sFieldName[i]) + CString(_T(" BETWEEN TO_DATE('"))+sGetData[1]+CString(_T(" 00:00:00' , 'YYYY/MM/DD HH24:MI:SS') AND TO_DATE('"))+sGetData[2]+CString(_T(" 23:59:59' , 'YYYY/MM/DD HH24:MI:SS')")); 			
		
			if ( i < 2 )
			{
				if ( nType == -1 )
				{
					if( !sWhere.IsEmpty() ) sWhere += _T(" OR ");					
				}			
			}
		}		
	}

	if ( sGetData[1][0] != '\0' || sGetData[2][0] != '\0' )
	{
		if ( nType == -1 ) sWhere += _T(") ");
	}	

	return 0;
}


/// DB에서 레코드 가져오기
INT CBL_BO_LOC_34::MakeDMFromDB( CString sWhere )
{
	INT ids;

	CESL_DataMgr TmpDM;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3400") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeDMFromDB") );

	pDM->FreeData();

	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , &TmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeDMFromDB") );

	ids = MakeDMFromBODB( sWhere , &TmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeDMFromDB") );

	ids = CLocCommonAPI::AddDM( &TmpDM , pDM , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeDMFromBODB") );

	ids = MakeDMFromSEDB( sWhere , &TmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeDMFromDB") );

	ids = CLocCommonAPI::AddDM( &TmpDM , pDM , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeDMFromBODB") );

	//ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("자료실구분"), _T("자료실설명"));

	return 0;
}

/// 단행 테이블에서 레코드 가져오기
INT CBL_BO_LOC_34::MakeDMFromBODB( CString sWhere , CESL_DataMgr *pTmpDM )
{
	INT ids;

	ids = pTmpDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeDMFromBODB") );

	return 0;
}

/// 연속 테이블에서 레코드 가져오기
INT CBL_BO_LOC_34::MakeDMFromSEDB( CString sWhere , CESL_DataMgr *pTmpDM  )
{
	INT ids;

	CString sTableNames = _T("SE_BOOK_TBL B, CO_DELIVERY_VIEW D, CO_LOAN_USER_TBL U, IDX_SE_TBL I, SE_VOL_TBL V ");
	CString sDefaultWhere = _T("D.BOOK_KEY=B.REC_KEY AND D.USER_KEY=U.REC_KEY AND B.SPECIES_KEY=I.REC_KEY AND B.VOL_KEY=V.REC_KEY ");

	// 테이블 이름 변경
	pTmpDM->TBL_NAME = sTableNames;
	// 기본 WHERE절 변경
	pTmpDM->CONDITION = sDefaultWhere;

	// 필드 정보 변경
	ids = CLocCommonAPI::ChangeDMFieldName( m_pParentMgr , pTmpDM , _T("저작자") , _T("I.AUTHOR") , _T("") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeDMFromBODB") );

	ids = pTmpDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MakeDMFromBODB") );

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [Func Desc]
//  배달자료검색결과를 그리드에 반영한다.
//////////////////////////////////////////////////////////////////////
INT CBL_BO_LOC_34::GridView()
{
	INT ids;

	CString  sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( !pDM ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1010 , _T("GridView") );
		
	INT nRowCnt = pDM->GetRowCount();
	if( !nRowCnt )
	{
		AfxMessageBox( _T("검색 결과가 없습니다.") );		
	}	
	else
	{		
		// 청구기호 생성
		ids = CLocCommonAPI::MakeShelfCode( m_pParentMgr , sDMAlias );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("GridView") ); 

		CString sWorkingStatus;			// 배가상태
		CString sDeliveryStatus;		// 배달상태 

		// 2011.09.05 ADD : 3000Api
		CStringArray asDes1,asDes2,asDes3;

		// 2009.10.09 ADD BY KSJ : 대출자번호가 주민번호일 경우 처리
		CString strValue , strUserNo;
		ids = m_pParentMgr->GetManageValue( _T("열람") , _T("공통") , _T("이용자관리") , _T("대출화면주민번호표시유무") , strValue );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1030 , _T("GridView") );
		strValue.TrimLeft(); strValue.TrimRight(); strValue.MakeUpper();
		for(INT i = 0; i < nRowCnt; i++)
		{
			// 배가상태
			ids = pDM->GetCellData( _T("배가상태") , i , sWorkingStatus );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1030 , _T("GridView") ); 
			ids = CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1040 , _T("GridView") ); 
			ids = pDM->SetCellData( _T("배가상태") , sWorkingStatus , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1050 , _T("GridView") ); 
			// 배달상태
			ids = pDM->GetCellData( _T("배달상태") , i , sDeliveryStatus );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1060 , _T("GridView") ); 
			if( sDeliveryStatus.Compare( _T("0") ) == 0 )
				sDeliveryStatus = _T("신청자료");
			else if( sDeliveryStatus.Compare( _T("1") ) == 0 )
				sDeliveryStatus = _T("접수완료");
			else if( sDeliveryStatus.Compare( _T("2") ) == 0 )
				sDeliveryStatus = _T("배달완료");
			ids = pDM->SetCellData( _T("배달상태") , sDeliveryStatus , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1070 , _T("GridView") );
				
			// 2009.10.09 ADD BY KSJ : 대출자번호가 주민번호일 경우 처리
			ids = pDM->GetCellData( _T("대출자번호"), i , strUserNo );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1080 , _T("GridView") );
			if( _T("Y") != strValue )
			{
				CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , strUserNo , m_pLoanManageValue->m_sIsUnityLoanService );
			}
			ids = pDM->SetCellData( _T("대출자번호설명") , strUserNo , i );
			if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1110 , _T("GridView") );

			// 2011.09.05 ADD : 3000Api
			if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
			{
				// 1:주민등록번호, 2:이메일, 3:휴대폰, 4:대출자주소, 5:자택전화, 6:근무지전화, 7:회원증비밀번호
				CString strGetData;
				pDM->GetCellData(_T("주소"), i, strGetData);
				asDes1.Add(strGetData);
				pDM->GetCellData(_T("연락처"), i, strGetData);
				asDes2.Add(strGetData);
				pDM->GetCellData(_T("이메일"), i, strGetData);
				asDes3.Add(strGetData);
			}
		}

		// 2011.09.05 ADD : 3000Api
		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse && 0 < nRowCnt)
		{
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
			if(NULL != pApi)
			{
				if(TRUE == pApi->CheckConnection())
				{
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_DELIVERY_TBL"), _T("ADDRESS"), asDes1);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_DELIVERY_TBL"), _T("PHONE"), asDes2);
						pApi->CloseSocket();
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes3);
						pApi->CloseSocket();
					}
					for(i = 0; i < nRowCnt; i++)
					{
						pDM->SetCellData(_T("주소"), asDes1.GetAt(i), i);
						pDM->SetCellData(_T("연락처"), asDes2.GetAt(i), i);
						pDM->SetCellData(_T("이메일"), asDes3.GetAt(i), i);
					}
				}
				delete pApi;
				pApi = NULL;
			}
			asDes1.RemoveAll(); asDes1.FreeExtra();
			asDes2.RemoveAll(); asDes2.FreeExtra();
			asDes3.RemoveAll(); asDes3.FreeExtra();
		}
		
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("대출소속정보") , _T("대출소속정보") );
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , _T("대출직급정보") , _T("대출직급정보") );

		// 2010.05.20 ADD BY KSJ : 자료실구분 코드->설명으로 보이도록 수정
		ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("자료실구분"), _T("자료실설명"));	
	}
	
	CString sCMAlias = _T("CM_BO_LOC_3400");
	ids = m_pParentMgr->AllControlDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1120 , _T("GridView") ); 	 
	
	return 0;
}

/// 입력 PATH
/// 1. CONTROL,DM에서 입력값 가져옴 - 입력값 검사
/// 2. 배달정보 DB INSERT
INT CBL_BO_LOC_34::InsertDelivery()
{
	BEGIN_SBL();

	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3420");
	CString sUserDMAlias = _T("DM_BO_LOC_3420_USER");
	CString sBookDMAlias = _T("DM_BO_LOC_3420_BOOK");
	
	CString sMainDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pMainDM = m_pParentMgr->FindDM( sMainDMAlias );
	if( pMainDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("") );

	CESL_DataMgr *pUserDM = m_pParentMgr->FindDM( sUserDMAlias );
	if( pMainDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("") );
	
	CESL_DataMgr *pBookDM = m_pParentMgr->FindDM( sBookDMAlias );
	if( pMainDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("") );

	if( pUserDM->GetRowCount() == 0 )
	{
		AfxMessageBox( _T("이용자를 선택해주십시오.") );
		return 1;
	}
	if( pBookDM->GetRowCount() == 0 )
	{
		AfxMessageBox( _T("책정보를 선택해주십시오.") );
		return 2;
	}

	// 이미 신청이 신청 , 접수된 자료인지 조사
	CString sSql , sUserKey , sBookKey , sValue;
	ids = pUserDM->GetCellData( _T("이용자KEY") , 0 , sUserKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	ids = pBookDM->GetCellData( _T("책정보KEY") , 0 , sBookKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	//++2008.10.22 UPDATE BY PWR {{++
	// 통합시스템 적용
	sSql.Format( _T("SELECT COUNT(*) FROM CO_DELIVERY_TBL ")
				 _T("WHERE USER_KEY=%s AND BOOK_KEY=%s AND STATUS IN('0','1') ")
				 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), sUserKey , sBookKey );
// 	sSql.Format( _T("SELECT COUNT(*) FROM CO_DELIVERY_TBL WHERE USER_KEY=%s AND BOOK_KEY=%s AND STATUS IN('0','1')") , sUserKey , sBookKey );
	//--2008.10.22 UPDATE BY PWR --}}
	ids = pMainDM->GetOneValueQuery( sSql , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );

	if( _ttoi(sValue) > 0 )
	{
		AfxMessageBox( _T("배달 신청을 할수 없습니다.\r\n현재 이용자가 배달 신청이나 접수중인 자료입니다.") );
		return 1;
	}

	const INT nCnt = 8 + 1;
	CString sInputValue[nCnt];

	pMainDM->MakeRecKey( sInputValue[0] );

	ids = pBookDM->GetCellData( _T("발행형태구분") , 0 , sInputValue[1] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );

	ids = pBookDM->GetCellData( _T("책정보KEY") , 0 , sInputValue[2] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	
	ids = pUserDM->GetCellData( _T("이용자KEY") , 0 , sInputValue[3] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("신청일자") , sInputValue[4] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	if( sInputValue[4].IsEmpty() )
	{
		AfxMessageBox( _T("신청일자를 입력하십시오") );
		return 3;
	}

	sInputValue[5] = _T("0") ;
	
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("주소") , sInputValue[6] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	if( sInputValue[6].IsEmpty() )
	{
		AfxMessageBox( _T("주소를 입력하십시오") );
		return 3;
	}

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("우편번호") , sInputValue[7] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	if( sInputValue[7].IsEmpty() )
	{
		AfxMessageBox( _T("우편번호를 입력하십시오") );
		return 3;
	}

	// 연락처가 빠져있음 
	// 그래서 김용배 추가함
	// 031031
	ids = m_pParentMgr->GetControlData( sCMAlias, _T("연락처"), sInputValue[ 8 ] );
	if( ids < 0 ) 
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	if( sInputValue[ 8 ].IsEmpty() )
	{
		AfxMessageBox( _T("연락처를 입력하시오.") );
		return 3;
	}

	// 2007.03.14 ADD BY PDJ
	// 수정된 주소를 DM에서 업데이트 한다.
	ids = pUserDM->SetCellData( _T("주소") , sInputValue[6], 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );

	ids = pUserDM->SetCellData( _T("우편번호") ,sInputValue[7], 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	
	ids = pUserDM->SetCellData( _T("연락처") ,sInputValue[8], 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	
	// DB field Name
	CString sDBFieldName[nCnt] =
	{
		_T("REC_KEY") , _T("PUBLISH_FORM_CODE") , _T("BOOK_KEY") , _T("USER_KEY") , _T("REQUEST_DATE") , 
		_T("STATUS") , _T("ADDRESS") , _T("ZIP_CODE"), _T("PHONE")
	};
	CString sDBFieldType[nCnt] =
	{
		_T("NUMERIC") , _T("STRING") , _T("NUMERIC") , _T("NUMERIC") , _T("STRING") , 
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") 
	};

	// 2011.09.06 ADD : 3000Api
	BOOL bSBUse = FALSE;
	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
	{	
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(m_pParentMgr);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->CheckConnection())
			{
				pMainDM->StartFrame();
				pMainDM->InitDBFieldData();
				CStringArray DataArray;
				for(INT i = 0; i < nCnt; i++)
				{
					if(_T("ADDRESS") == sDBFieldName[i] || _T("PHONE") == sDBFieldName[i])
					{
						if(TRUE == p3000Api->ConnectSocket())
						{
							DataArray.Add(sInputValue[i]);
							if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_DELIVERY_TBL"), sDBFieldName[i], DataArray))
							{
								sInputValue[i] = DataArray.GetAt(0);
							}
							p3000Api->CloseSocket();
							DataArray.RemoveAll(); DataArray.FreeExtra();
						}
					}
					pMainDM->AddDBFieldData(sDBFieldName[i], sDBFieldType[i], sInputValue[i]);
				}
				bSBUse = TRUE;
			}
			delete p3000Api;
			p3000Api = NULL;
		}
	}
	if(FALSE == bSBUse)
	{
		pMainDM->StartFrame();
		pMainDM->InitDBFieldData();
		for( INT i = 0 ; i < nCnt ; i++ )
		{
			pMainDM->AddDBFieldData(sDBFieldName[i], sDBFieldType[i], sInputValue[i]);
		}
	}

	pMainDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );

	//++2008.10.22 ADD BY PWR {{++
	// 통합시스템 적용
	pMainDM->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE") );
	//--2008.10.22 ADD BY PWR --}}

	pMainDM->MakeInsertFrame( _T("CO_DELIVERY_TBL") );

	ids = pMainDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InsertDelivery") );

	pMainDM->EndFrame();

	// (M_2004-1109_HAN)
	// 한번에 읽어와 입력하기에는 너무 많은 데이터라
	// DB에서 등록번호를 이용해 읽어와 버린다.
	////////////////////////////
	// 메인 DM을 구성한다.
	CESL_DataMgr TmpDM;
	CString sRegNo = _T("");
	CString sWhere = _T("");
	INT nIndex = 0;

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pMainDM , &TmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("DeliveryDBProc") );

	if ( !sBookKey.IsEmpty( ) )
		sWhere.Format( _T("B.REC_KEY=%s AND D.USER_KEY=%s "), sBookKey, sUserKey );

	// 2007.09.03 add by pdw
	// 연속자료 입력시 테이블명 변경되도록 수정
	if( _T("SE") == sInputValue[1] )
	{
		TmpDM.TBL_NAME.Replace( _T("BO_BOOK_TBL") , _T("SE_BOOK_TBL") );
		TmpDM.TBL_NAME.Replace( _T("IDX_BO_TBL") , _T("IDX_SE_TBL") );
	}

	ids = TmpDM.RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );

	pMainDM->InsertRow(-1);
	nIndex = pMainDM->GetRowCount() - 1 ;
	// 2. SELECT 한 값을 메인 DM으로 이동
	ids = CLocCommonAPI::AddDM( &TmpDM, pMainDM, 0, nIndex, m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("DeliveryDBProc") ); 

	GridView( );

	CESL_DataMgr *pSBLDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3400") );
	INT nRowCount = pSBLDM->GetRowCount();
	END_SBL(nRowCount);	
	
	AfxMessageBox( _T("신청되었습니다.") );

	return 0;
/*
CString sUpdateData[16];
ids = pBookDM->GetCellData( _T("저작자"), 0 , sUpdateData[0] );
ids = pBookDM->GetCellData( _T("등록번호"), 0 , sUpdateData[1] );
ids = pBookDM->GetCellData( _T("배가상태"), 0 , sUpdateData[2] );
ids = pBookDM->GetCellData( _T("발행자"), 0 , sUpdateData[3] );
ids = pBookDM->GetCellData( _T("서명"), 0 , sUpdateData[4] );
ids = pUserDM->GetCellData( _T("대출자명"), 0 , sUpdateData[5] );
ids = pUserDM->GetCellData( _T("대출자번호"), 0 , sUpdateData[6] );
pMainDM->InsertRow(-1);
INT nIndex = pMainDM->GetRowCount() -1 ;
pMainDM->SetCellData( _T("저작자") , sUpdateData[0] , nIndex );
pMainDM->SetCellData( _T("책정보KEY") , sInputValue[2] , nIndex );
pMainDM->SetCellData( _T("등록번호") , sUpdateData[1] , nIndex );
pMainDM->SetCellData( _T("배가상태") , sUpdateData[2] , nIndex );
pMainDM->SetCellData( _T("주소") , sInputValue[6] , nIndex );
pMainDM->SetCellData( _T("연락처") , sInputValue[ 8 ] , nIndex );
pMainDM->SetCellData( _T("신청일자") , sInputValue[4] , nIndex );
pMainDM->SetCellData( _T("배달상태") , _T("신청자료") , nIndex );
pMainDM->SetCellData( _T("우편번호") , sInputValue[7] , nIndex );
pMainDM->SetCellData( _T("발행자") , sUpdateData[3] , nIndex );
pMainDM->SetCellData( _T("서명") , sUpdateData[4] , nIndex );
pMainDM->SetCellData( _T("대출자명") , sUpdateData[5] , nIndex );
pMainDM->SetCellData( _T("이용자KEY") , sInputValue[3] , nIndex );
pMainDM->SetCellData( _T("대출자번호") , sUpdateData[6] , nIndex );
pMainDM->SetCellData( _T("배달정보KEY") , sInputValue[0] , nIndex );
pMainDM->SetCellData( _T("발행형태구분") , sInputValue[1] , nIndex );
// 김용배 추가	2004-02-13
// 청구기호 빠져 있음
pMainDM->SetCellData( _T("청구기호"), m_pParentMgr->FindDM( _T("DM_BO_LOC_3420_BOOK") )->GetCellData( _T("청구기호"), 0 ), nIndex );
//CLocCommonAPI::InsertGridRow( m_pParentMgr , _T("CM_BO_LOC_3400") , _T("grid1") , nIndex , nIndex );
*/
}

/// 수정 PATH
/// 1. CONTROL에서 입력값 가져옴 - 입력값 검사
/// 2. 배달정보 DB UPDATE
INT CBL_BO_LOC_34::UpdateDelivery()
{
	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3420_1");
	
	CString sMainDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pMainDM = m_pParentMgr->FindDM( sMainDMAlias );
	if( pMainDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("") );

	INT nIndex;
	GetTempData( _T("선택인덱스") , nIndex );



	const INT nCnt = 10;
	CString sInputValue[nCnt];

	pMainDM->GetCellData( _T("배달정보KEY") , nIndex , sInputValue[0] );

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("신청일자") , sInputValue[4] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("접수일자") , sInputValue[7] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("배달일자") , sInputValue[8] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("연락처") , sInputValue[9] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	
	ids = m_pParentMgr->GetControlData( sCMAlias , _T("주소") , sInputValue[5] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	if( sInputValue[5].IsEmpty() )
	{
		AfxMessageBox( _T("주소를 입력하십시오") );
		return 3;
	}

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("우편번호") , sInputValue[6] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("") );
	if( sInputValue[6].IsEmpty() )
	{
		AfxMessageBox( _T("우편번호를 입력하십시오") );
		return 3;
	}

	// DB field Name
	CString sDBFieldName[nCnt] =
	{
		_T("REC_KEY") , _T("PUBLISH_FORM_CODE") , _T("BOOK_KEY") , _T("USER_KEY") , _T("REQUEST_DATE") , 
		_T("ADDRESS") , _T("ZIP_CODE") , _T("RECEIPT_DATE") , _T("DELIVERY_DATE") , _T("PHONE")
	};
	CString sDBFieldType[nCnt] =
	{
		_T("NUMERIC") , _T("STRING") , _T("NUMERIC") , _T("NUMERIC") , _T("STRING") , 
		_T("STRING") , _T("STRING") , _T("STRING") , _T("STRING") , _T("STRING")
	};

	pMainDM->StartFrame();
	pMainDM->InitDBFieldData();

	// 2011.09.06 ADD : 3000Api
	BOOL bSBUse = FALSE;
	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
	{
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(m_pParentMgr);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->CheckConnection())
			{
				CString strGetData;
				CStringArray DataArray;
				for(INT i = 4; i < nCnt; i++)
				{
					strGetData = sInputValue[i];
					if(_T("ADDRESS") == sDBFieldName[i] || _T("PHONE") == sDBFieldName[i])
					{
						if(TRUE == p3000Api->ConnectSocket())
						{
							DataArray.Add(strGetData);
							if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_DELIVERY_TBL"), sDBFieldName[i], DataArray))
							{
								strGetData = DataArray.GetAt(0);
							}
							DataArray.RemoveAll(); DataArray.FreeExtra();
							p3000Api->CloseSocket();
						}
					}
					pMainDM->AddDBFieldData(sDBFieldName[i], sDBFieldType[i], strGetData);
				}
				bSBUse = TRUE;
			}
			delete p3000Api;
			p3000Api = NULL;
		}
	}
	if(FALSE == bSBUse)
	{
		for(INT i = 4; i < nCnt; i++)
		{
			pMainDM->AddDBFieldData(sDBFieldName[i], sDBFieldType[i], sInputValue[i]);
		}
	}

	pMainDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	pMainDM->MakeUpdateFrame( _T("CO_DELIVERY_TBL") , _T("REC_KEY") , _T("NUMERIC") , sInputValue[0] );

	ids = pMainDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InsertDelivery") );

	pMainDM->EndFrame();

	pMainDM->SetCellData( _T("주소") , sInputValue[5] , nIndex );
	pMainDM->SetCellData( _T("신청일자") , sInputValue[4] , nIndex );
	pMainDM->SetCellData( _T("접수일자") , sInputValue[7] , nIndex );
	pMainDM->SetCellData( _T("배달일자") , sInputValue[8] , nIndex );
	pMainDM->SetCellData( _T("우편번호") , sInputValue[6] , nIndex );
	pMainDM->SetCellData( _T("연락처") , sInputValue[9] , nIndex );
	CLocCommonAPI::InsertGridRow( m_pParentMgr , _T("CM_BO_LOC_3400") , _T("grid1") , nIndex , nIndex );
	AfxMessageBox( _T("수정하였습니다.") );

	return 0;
}




/// 접수처리 PATH
/// 일괄 변경
/// 1. 선정된 자료에 대해
/// 2. 배달정보 DB UPDATE
/// 3. 책정보 DB UPDATE
INT CBL_BO_LOC_34::Receipt()
{
	INT ids;
	CString strFuncName = _T("CBL_BO_LOC_34::Receipt()");

	// 그리드 포인터 참조
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , strFuncName);

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return 1;
	}

	// 입력값 검사
	CString sInputDate;
	ids = GetReceiptInputDate( sInputDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , strFuncName);

	if( sInputDate.IsEmpty() )
	{
		AfxMessageBox( _T("접수일자를 입력해주십시오.") );
		return 2;
	}

	// DM 참조 선택
	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , strFuncName);

	INT nSuccessCnt = 0;	// 작업에 성공한 자료 수 
	CString sWorkingStatus; // 자료상태
	CString sStatus ; // 배달 상태
	
	// 프로그래스바 생성
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	INT nReservationFail = 0;	// 예약이력 실패
	INT nUserStatusFail  = 0;   // 회원상태 실패

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));

	INT nIndex;
	CString sGet[4];
	CString sSql , sResult ;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// 진행상황 보여주기
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CString sBookKey , sPublishForm , strDeliveryKey;
		ids = pDM->GetCellData( _T("책정보KEY") , nIndex , sBookKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , strFuncName);

		// 2010.02.09 ADD BY KSJ : 검색 조건 누락으로 인한 추가
		ids = pDM->GetCellData( _T("배달정보KEY") , nIndex , strDeliveryKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , strFuncName);

		sSql.Format(
			_T("SELECT D.STATUS, B.WORKING_STATUS FROM BO_BOOK_TBL B, CO_DELIVERY_VIEW D WHERE D.BOOK_KEY=B.REC_KEY AND B.REC_KEY = %s AND D.REC_KEY = %s")
			,sBookKey,strDeliveryKey);
		TmpDM.GetValuesQuery(sSql, sGet, 2);

		if(sGet[0].Compare(_T("0")) != 0)
		{
			pGrid->SelectGetNext();
			continue;
		}
		ids = CLocCommonAPI::GetWorkingStatusDescToCode( sGet[1] , 1 );
		if( sGet[1].Compare( _T("비치자료") ) != 0 )
		{
			pGrid->SelectGetNext();
			continue;
		}
// 		ids = pDM->GetCellData( _T("배달상태") , nIndex , sStatus );
// 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , strFuncName);
// 
// 		if( sStatus.Compare( _T("신청자료") ) != 0 )
// 		{
// 			pGrid->SelectGetNext();
// 			continue;
// 		}
// 		
// 		// 2006-04-12 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		// 비치자료만 접수처리가 가능하다.
// 		ids = pDM->GetCellData( _T("배가상태") , nIndex , sWorkingStatus );
// 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , strFuncName);
// 
// 		if( sWorkingStatus.Compare( _T("비치자료") ) != 0 )
// 		{
// 			pGrid->SelectGetNext();
// 			continue;
// 		}
// 		// ------------------------------------------------------------------------------------------------------
// 		
		// 2007.08.08 add by pdw
		// 예약자료는 접수처리가 불가능하다.

		// 2009.05.19 UPDATE BY CJY : DM의 책상태가 아닌 DB에 있는 최신상태로 판별한다. (예약뿐만 아니라 대출중인 자료인지도 판별)
// 		sSql.Format( _T("SELECT COUNT(*) FROM LS_WORK_T01 WHERE BOOK_KEY=%s AND STATUS = '3'") , sBookKey );
// 		ids = pDM->GetOneValueQuery( sSql , sResult );
// 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , strFuncName);
// 		if( _ttoi(sResult) > 0 )
		sSql.Format( _T("SELECT 'L' FROM LS_WORK_T01 WHERE BOOK_KEY=%s AND STATUS IN ('0', '2', 'L', 'D', '5', '6', '7') UNION SELECT 'R' FROM LS_WORK_T01 WHERE BOOK_KEY=%s AND STATUS = '3' UNION SELECT NULL FROM DUAL") , sBookKey, sBookKey );
		ids = pDM->GetOneValueQuery( sSql , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , strFuncName);
		if(_T("") != sResult)
		{
			if(_T("R") == sResult)
			{
				nReservationFail++;
			}
			pGrid->SelectGetNext();
			continue;
		}

		BOOL bIsReceiptUserStatus = TRUE;
		
		CString sUserKey;
 		ids = pDM->GetCellData( _T("이용자KEY") , nIndex , sUserKey );
 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );		
 
		sSql.Format( _T("SELECT U.USER_CLASS, U.LOAN_STOP_DATE, U.MEMBER_CLASS, U.LOST_USER_CARD FROM CO_LOAN_USER_TBL U WHERE U.REC_KEY=%s") , sUserKey );
 		ids = TmpDM.GetValuesQuery(sSql , sGet, 4);
 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );
 
		if(_T("1") == sGet[0])
		{
 			// 대출정지회원이라도 대출제한일이 현재일보다 이전이면 대출정지회원으로 보지 않는다.
 			CString strLoanStopDate;
 			strLoanStopDate = sGet[1];
 			
 			COleDateTime oleLoanStopDate;
 			int nYear = 1, nMonth = 1, nDay = 1;
 			_stscanf(strLoanStopDate.GetBuffer(0), _T("%d/%d/%d"), &nYear, &nMonth, &nDay);
 			
 			oleLoanStopDate.SetDate(nYear, nMonth, nDay);
 			
 			COleDateTime oleToday = COleDateTime::GetCurrentTime();
 			oleToday.SetDate(oleToday.GetYear(), oleToday.GetMonth(), oleToday.GetDay());
 			
 			if(oleToday > oleLoanStopDate)
 			{
 			}
 			else
 			{
 				bIsReceiptUserStatus = FALSE;
 			}
 		}
 		if(_T("2") == sGet[0] || _T("3") == sGet[0])
 		{
 			bIsReceiptUserStatus = FALSE;
 		}
 		// 회원구분 
 		if(_T("1") == sGet[2]) 
 		{ // 비회원
 			bIsReceiptUserStatus = FALSE; 
 		}
 		// 2008.05.28 ADD BY PWR : 준회원은 회원인증후에 대출이 가능하므로 제외
 		else if(_T("2") == sGet[2])
 		{ // 준회원
 			bIsReceiptUserStatus = FALSE; 
 		}
 		// 회원증분실회원이면
 		if(_T("Y") == sGet[3]) { bIsReceiptUserStatus = FALSE; }
 
 		if(FALSE == bIsReceiptUserStatus)
 		{
  			nUserStatusFail++;
  			pGrid->SelectGetNext();
  			continue;
 		}

		//++2009.01.09 UPDATE BY CJY {{++
		//DEL	DM을 참조하도록 변경
// 		BOOL bIsReceiptUserStatus = TRUE;
// 
// 		CString strUserClass;
// 		ids = pDM->GetCellData( _T("불량회원구분") , nIndex, strUserClass);
// 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , strFuncName); 
// 		if(_T("1") == strUserClass) 
// 		{ 
// 			// 대출정지회원이라도 대출제한일이 현재일보다 이전이면 대출정지회원으로 보지 않는다.
// 			CString strLoanStopDate;
// 			ids = pDM->GetCellData(_T("대출정지일"), nIndex, strLoanStopDate);
// 			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , strFuncName);
// 			
// 			COleDateTime oleLoanStopDate;
// 			int nYear = 1, nMonth = 1, nDay = 1;
// 			_stscanf(strLoanStopDate.GetBuffer(0), _T("%d/%d/%d"), &nYear, &nMonth, &nDay);
// 			
// 			oleLoanStopDate.SetDate(nYear, nMonth, nDay);
// 			
// 			COleDateTime oleToday = COleDateTime::GetCurrentTime();
// 			oleToday.SetDate(oleToday.GetYear(), oleToday.GetMonth(), oleToday.GetDay());
// 			
// 			if(oleToday > oleLoanStopDate)
// 			{
// 			}
// 			else
// 			{
// 				bIsReceiptUserStatus = FALSE;
// 			}
// 		}
// 		if(_T("2") == strUserClass || _T("3") == strUserClass)
// 		{
// 			bIsReceiptUserStatus = FALSE;
// 		}
// 
// 		// 회원구분 
// 		CString strMemberClass;
// 		ids = pDM->GetCellData( _T("회원구분") , nIndex, strMemberClass);
// 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , strFuncName); 
// 		if(_T("1") == strMemberClass) 
// 		{ // 비회원
// 			bIsReceiptUserStatus = FALSE; 
// 		}
// 		// 2008.05.28 ADD BY PWR : 준회원은 회원인증후에 대출이 가능하므로 제외
// 		else if(_T("2") == strMemberClass)
// 		{ // 준회원
// 			bIsReceiptUserStatus = FALSE; 
// 		}
// 		
// 		// 회원증분실회원이면
// 		CString strLostUserCard;
// 		ids = pDM->GetCellData( _T("회원증분실여부") , nIndex, strLostUserCard);
// 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , strFuncName); 
// 		if(_T("Y") == strLostUserCard) { bIsReceiptUserStatus = FALSE; }
// 
// 		if(FALSE == bIsReceiptUserStatus)
// 		{
//  			nUserStatusFail++;
//  			pGrid->SelectGetNext();
//  			continue;
// 		}
//DEL	이전소스
//DEL 		// 2007.08.28 add by pdw
//DEL 		// 회원상태에 따라 접수처리가 불가능하다.
//DEL 		CString sUserKey;
//DEL 		ids = pDM->GetCellData( _T("이용자KEY") , nIndex , sUserKey );
//DEL 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );		
//DEL 
//DEL 		sSql.Format( _T("SELECT USER_CLASS FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s") , sUserKey );
//DEL 		ids = pDM->GetOneValueQuery( sSql , sResult );
//DEL 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );
//DEL 
//DEL 		if( _T("2") == sResult || _T("3") == sResult )
//DEL 		{
//DEL 			nUserStatusFail++;
//DEL 			pGrid->SelectGetNext();
//DEL 			continue;
//DEL 		}
//DEL 		else if( _T("1") == sResult )
//DEL 		{
//DEL 			sSql.Format(_T("SELECT TO_CHAR(LOAN_STOP_DATE, 'YYYY/MM/DD') FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s") , sUserKey);
//DEL 			ids = pDM->GetOneValueQuery( sSql , sResult );
//DEL 			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );
//DEL 
//DEL 			if( !sResult.IsEmpty() )
//DEL 			{
//DEL 				//++2008.05.13 UPDATE BY PWR {{++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//DEL 				// CTime -> COleDateTime 으로 변경 ( 대출정지일이 커서 에러발생 )
//DEL 				COleDateTime tTmpCurrent = COleDateTime::GetCurrentTime();
//DEL 				//CTime tTmpCurrent = CLocCommonAPI::GetDBTime(m_pParentMgr);
//DEL 				COleDateTime tCurrent = COleDateTime( tTmpCurrent.GetYear() , tTmpCurrent.GetMonth() , tTmpCurrent.GetDay() , 0 , 0 , 0 );
//DEL 				COleDateTime tLoanStopDate = COleDateTime( _ttoi(sResult.Mid(0,4)) , _ttoi(sResult.Mid(5,2)) , _ttoi(sResult.Mid(8,2)) , 0 , 0 , 0 );
//DEL 				//--2008.05.13 UPDATE BY PWR ------------------------------------------------------------}}
//DEL 				if( tCurrent <= tLoanStopDate )
//DEL 				{
//DEL 					nUserStatusFail++;
//DEL 					pGrid->SelectGetNext();
//DEL 					continue;
//DEL 				}
//DEL 			}
//DEL 			else
//DEL 			{
//DEL 				nUserStatusFail++;
//DEL 				pGrid->SelectGetNext();
//DEL 				continue;
//DEL 			}
//DEL 
//DEL 		}
//DEL 		//++2008.07.15 ADD BY PWR {{++
//DEL 		// 대출증분실회원 접수처리 불가능
//DEL 		sResult = m_pLoanManageValue->m_sLoanIsLostUserCard;
//DEL 		if(sResult == _T("Y") )
//DEL 		{
//DEL 			sSql.Format( _T("SELECT LOST_USER_CARD FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), sUserKey );
//DEL 			ids = pDM->GetOneValueQuery( sSql , sResult );
//DEL 			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("IsLoan()") );
//DEL 			if( _T("Y") == sResult ) 
//DEL 			{
//DEL 				nUserStatusFail++;
//DEL 				pGrid->SelectGetNext();
//DEL 				continue;
//DEL 			}
//DEL 		}
//DEL 		//--2008.07.15 ADD BY PWR --}}
		//--2009.01.09 UPDATE BY CJY --}}


		//  DB작업
		ids = ReceiptDBProc( nIndex , &TmpDM , sInputDate );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , strFuncName);
		if( ids > 0 ) return ids;

		//  DM 작업
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , strFuncName);

		//  화면 작업
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -14 , strFuncName);

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// 결과 메시지
	if( nSelectCnt == nSuccessCnt )
	{
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료를 접수 처리 하였습니다.") , nSelectCnt );
	}
	else 
	{
		sResultMsg.Format( _T("[선택한 전체 자료 건수]           : %d \r\n")
			               _T("[접수처리 성공 건수]               : %d \r\n")
			               _T("[접수처리 실패 건수(자료상태)] : %d \r\n")
						   _T("[접수처리 실패 건수(예약자료)] : %d \r\n")
						   _T("[접수처리 실패 건수(회원상태)] : %d "), nSelectCnt, nSuccessCnt, 
						   nSelectCnt-nSuccessCnt-nReservationFail-nUserStatusFail, nReservationFail, nUserStatusFail );
	}
	AfxMessageBox( sResultMsg );

	return 0;
}

/// 접수 오나료 DB 작업
/// 2. 배달정보 UPDATE
/// 3. TmpDM에 변경된 정보 기록 - 음 소스가 길어지는군.. 분리할까.. ㅡㅡa
INT CBL_BO_LOC_34::ReceiptDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sInputDate )
{
	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairDBProc") );

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDBProc") ); 

	// 2010.01.27 ADD BY PWR : TEMP초기화
	pTmpDM->FreeData();

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RegRepairDBProc") ); 

	pDM->StartFrame();	

	// 프레임 생성
	pDM->InitDBFieldData();
	// 2. 배달 정보 UPDATE
	CString sDeliveryRecKey;
	ids = pTmpDM->GetCellData( _T("배달정보KEY") , 0 , sDeliveryRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepairDBProc") ); 

	CString sReceiptDate;	
	CTime tReceiptDate = CTime::GetCurrentTime();
	sReceiptDate.Format(
		_T("TO_DATE('%s %0d:%d:%d', 'YYYY/MM/DD HH24:MI:SS')"), 
		sInputDate, tReceiptDate.GetHour(), tReceiptDate.GetMinute(), tReceiptDate.GetSecond());

	pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("1") );
// 	pDM->AddDBFieldData( _T("RECEIPT_DATE") , _T("STRING") , sInputDate );
	pDM->AddDBFieldData( _T("RECEIPT_DATE") , _T("NUMERIC") , sReceiptDate );
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	pDM->MakeUpdateFrame( _T("CO_DELIVERY_TBL") , _T("REC_KEY") , _T("NUMERIC") , sDeliveryRecKey );
	pDM->InitDBFieldData();

	// 2. 1 TmpDM에 기록
	ids = pTmpDM->SetCellData( _T("배달상태") , _T("접수완료"), 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("RegRepairDBProc") ); 
	ids = pTmpDM->SetCellData( _T("접수일자") , sInputDate , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("RegRepairDBProc") ); 


	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();

	// 2005.03.14 ADD BY PDJ 
	// 책정보 업데이트
	// 접수후 이용자가 대출할 수 없도록 
	// 책의 상태를 접수완료로 변경한다.
	CString sBookRecKey , sWorkingStatus , sPreWorkingStatus , sPublishForm , sNewWorkingStatus;
	CString sUserRecKey;
	ids = pTmpDM->GetCellData( _T("책정보KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->GetCellData( _T("배가상태") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->GetCellData( _T("발행형태구분") , 0 , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->GetCellData( _T("이용자KEY") , 0 , sUserRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("DeliveryDBProc") ); 

	CString sTableName;	
	CString sIDXTblName;	
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	CString sTmpIDXTblName;	
	//=====================================================
	
	CString sProcName;	

	if( sPublishForm.Compare( _T("MO") ) == 0 )
	{
		sTableName = _T("BO_BOOK_TBL");
		sNewWorkingStatus = _T("BOL215O");
		sIDXTblName = _T("IDX_BO_TBL");
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		sTmpIDXTblName = _T("IDX_TMP_BO_TBL");
		//=====================================================		
		sProcName = _T("TESTWORKSTATUS");
	}
	else
	{
		sTableName = _T("SE_BOOK_TBL");
		sNewWorkingStatus = _T("SEL315O");
		sIDXTblName = _T("IDX_SE_TBL");
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		sTmpIDXTblName = _T("IDX_TMP_SE_TBL");
		//=====================================================		
		sProcName = _T("TESTSEWORKSTATUS");
	}


	// 프레임 생성
	// 1. 책정보 UPDATE
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , sNewWorkingStatus );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );
	pDM->MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	ids = CLocCommonAPI::GetWorkingStatusDescToCode( sNewWorkingStatus , 1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeDMFromDB") ); 

	ids = pTmpDM->SetCellData( _T("배가상태") , sNewWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("DeliveryDBProc") ); 

	// 김용배 추가
	CString speciesKey = pDM->GetCellData( _T("종정보KEY"), nDMIndex );
	CString query;
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	query.Format( _T("UPDATE %s SET WORKING_STATUS=%s( %s ) WHERE REC_KEY=%s;\n")
// 				  _T("INSERT INTO %s( SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE, REC_KEY) VALUES( %s, 'I', SYSDATE, ESL_SEQ.NEXTVAL );"),
// 				sTableName, sProcName, speciesKey, speciesKey, sTmpIDXTblName, speciesKey );
// 	if( !speciesKey.IsEmpty() )
// 		pDM->AddFrame( query );	
	query.Format( _T("UPDATE %s SET WORKING_STATUS=%s( %s ) WHERE REC_KEY=%s;"),sTableName, sProcName, speciesKey, speciesKey);
	if( !speciesKey.IsEmpty() )
 		pDM->AddFrame( query );	
	//=====================================================
	

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("DeliveryDBProc") ); 

	ids = pDM->EndFrame();

	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RegRepairDBProc") ); 

	return 0;
}

/// 자료 변경에 성공한 DM의 내용을 실제 사용되는 DM에 기록한다.
INT CBL_BO_LOC_34::DBInsertUpdateDMProc( INT nDMIndex , CESL_DataMgr *pTmpDM )
{
	INT ids;
	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairDMProc") );

	ids = CLocCommonAPI::AddDM( pTmpDM , pDM , 0 , nDMIndex , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairDMProc") );

	return 0;
}

/// 1. INDEX로부터 그리드의 내용 변경
INT CBL_BO_LOC_34::DBInsertUpdateGridProc( INT nDMIndex )
{
	INT ids;

	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairGridProc") );

	ids = CLocCommonAPI::InsertGridRow( m_pParentMgr , pGrid , nDMIndex , nDMIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepairGridProc") );

	return 0;
}


INT CBL_BO_LOC_34::GetReceiptInputDate( CString &sInputDate )
{
	CString sCMAlias = _T("CM_BO_LOC_3430");

	INT ids;

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("접수일자") , sInputDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetReceiptInputDate") );
	sInputDate.TrimRight(); sInputDate.TrimLeft();

	return 0;
}

// 프로그래스바 생성
CLocProgressBarDlg* CBL_BO_LOC_34::CreateProgressBarDlg( INT nUpper )
{
	CLocProgressBarDlg* pProgressBar = new CLocProgressBarDlg(m_pParentMgr);
	pProgressBar->Create( m_pParentMgr );
	pProgressBar->ShowWindow( SW_SHOW );
	pProgressBar->InitRange(nUpper);

	return pProgressBar;
}

// 프로그래스바 종료
INT CBL_BO_LOC_34::DeleteProgressBarDlg( CLocProgressBarDlg *pProgressBarDlg )
{
	if( pProgressBarDlg != NULL ) 
		delete pProgressBarDlg;
	pProgressBarDlg = NULL;
	
	return 0;
}




/// 배달 처리 PATH
/// 일괄 변경
/// 1. 선정된 자료에 대해
/// 2. 배달정보 DB UPDATE
/// 3. 책정보 DB UPDATE
/// 4. 이용자정보 DB UPDATE
/// 5. 대출/반납 정보 DB UPDATE
INT CBL_BO_LOC_34::Delivery()
{
	INT ids;

	// 그리드 포인터 참조
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("Delivery") );

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return 1;
	}

	// 입력값 검사
	CString sInputDate,sLoanPeriod;
	ids = GetDeliveryInputDate( sInputDate ,sLoanPeriod );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("Delivery") );

	if( sInputDate.IsEmpty() )
	{
		AfxMessageBox( _T("접수일자를 입력해주십시오.") );
		return 2;
	}
	if( sLoanPeriod.IsEmpty() )
	{
		AfxMessageBox( _T("대출기한을 입력해주십시오.") );
		return 3;
	}

	// DM 참조 선택
	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("Delivery") );

	INT nSuccessCnt = 0;	// 작업에 성공한 자료 수 
	CString sWorkingStatus; // 자료상태
	CString sStatus ; // 배달 상태

	// 프로그래스바 생성
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);

	InitLog(_T("-- 배달완료 처리 에러 사항 --\r\n"));

	INT nIndex;
	CString sRegNo , sUserNo;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		// 진행상황 보여주기
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		CESL_DataMgr TmpDM;

		ids = pDM->GetCellData( _T("배달상태") , nIndex , sStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("Delivery") );

		ids = pDM->GetCellData( _T("등록번호") , nIndex , sRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("Delivery") );
		
		ids = pDM->GetCellData( _T("대출자번호") , nIndex , sUserNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("Delivery") );

		if( sStatus.Compare( _T("접수완료") ) != 0 )
		{
			AddingLog( sRegNo , sUserNo , nIndex , -1 );
			pGrid->SelectGetNext();
			continue;
		}

		ids = pDM->GetCellData( _T("배가상태") , nIndex , sWorkingStatus );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("Delivery") );

		// 2005.03.23 UPDATE BY PDJ
		// 접수완료시 배달을 하려고 했는데 대출이 가능하므로 접수시 배달자료로 만듬.
		// 접수완료는 배달직전으로 발송할려고 책을 가져온 상태이기 때문임.
		//if( ( status.Compare(_T("비치자료")) !=0 ) && (sWorkingStatus.Compare( _T("연속비치자료") ) != 0) && (sWorkingStatus.Compare( _T("실시간배가자료") ) != 0))
		if( !(sStatus.Compare(_T("접수완료"))==0 && sWorkingStatus.Compare( _T("배달자료") )==0)  && 
			!(sStatus.Compare(_T("접수완료"))==0 && sWorkingStatus.Compare( _T("연속배달자료") )==0) )
		{
			AddingLog( sRegNo , sUserNo , nIndex , -2 );
			pGrid->SelectGetNext();
			continue;
		}

		// 이용자가 대출할수 있는 상태인지 검사
		ids = IsLoan(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("Delivery") );
		if( ids > 0 )
		{
			AddingLog( sRegNo , sUserNo , nIndex , ids );
			pGrid->SelectGetNext();
			continue;
		}

		// 자료 상태는 실시간으로 알아본다.
		// 자료가 예약이 되어 있는지 알아본다.
		// 예약된 자료라면 배달이 불가능하다.
		ids = IsReservation( nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("Delivery") );
		if( ids > 0 )
		{
			AddingLog( sRegNo , sUserNo , nIndex , ids );			
			pGrid->SelectGetNext();
			continue;
		}

		// 배달 처리 DB 작업
		ids = DeliveryDBProc( nIndex , &TmpDM , sInputDate , sLoanPeriod);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("Delivery") );
		if( ids > 0 ) return ids;

		// 배달 처리 DM 작업
		ids = DBInsertUpdateDMProc( nIndex , &TmpDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("Delivery") );

		// 배달 처리 화면 작업
		ids = DBInsertUpdateGridProc(nIndex);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("Delivery") );

		nSuccessCnt++;
		pGrid->SelectGetNext();
	}
	
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar(i);
	DeleteProgressBarDlg( pProgressBar );

	CString sResultMsg;	// 결과 메시지
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료를 배달 완료 처리 하였습니다.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료가 배달 완료 처리 할 수 없는 상태이기 때문에 작업에 실패하였습니다.")
							_T("\r\n에러 내역을 '%s'파일에 기록하였습니다.") , nSelectCnt , m_sLogFile );
	else
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료중 '%d'개의 자료를 배달 완료 처리하였습니다.\r\n")
						   _T("'%d'개의 자료는 자료상태가 배달 완료 처리 할 수 없는 상태이기 때문에 작업에 실패하였습니다.") 
						   _T("\r\n에러 내역을 '%s'파일에 기록하였습니다."), 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt , m_sLogFile );

	AfxMessageBox( sResultMsg );

	return 0;
}

/// 배달 완료 DB 작업
/// 1. 책정보 UPDATE
/// 2. 배달정보 UPDATE
/// 3. 이용자정보 DB UPDATE
/// 4. 대출/반납 정보 DB UPDATE
/// 5. TmpDM에 변경된 정보 기록 - 음 소스가 길어지는군.. 분리할까.. ㅡㅡa
INT CBL_BO_LOC_34::DeliveryDBProc( INT nDMIndex , CESL_DataMgr *pTmpDM , CString sInputDate , CString sLoanPeriod )
{
	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DeliveryDBProc") );

	// 1. Column정보 pTmpDM에 복사
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , pDM , pTmpDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("DeliveryDBProc") ); 

	// 2010.01.27 ADD BY PWR : TEMP초기화
	pTmpDM->FreeData();

	// 2. nDMIndex의 정보를 pTmpDM에 차가
	ids = CLocCommonAPI::AddDM( pDM , pTmpDM , nDMIndex , -1 , m_pParentMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("DeliveryDBProc") ); 

	CString sLoanRecKey;
	pDM->MakeRecKey( sLoanRecKey );

	pDM->StartFrame();

	CString sBookRecKey , sWorkingStatus , sPreWorkingStatus , sPublishForm , sNewWorkingStatus;
	CString sUserRecKey;
	ids = pTmpDM->GetCellData( _T("책정보KEY") , 0 , sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->GetCellData( _T("배가상태") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->GetCellData( _T("발행형태구분") , 0 , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->GetCellData( _T("이용자KEY") , 0 , sUserRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("DeliveryDBProc") ); 

	CString sTableName;
	
	CString sIDXTblName;
	//=====================================================
	//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 	CString sTmpIDXTblName;
	//=====================================================	
	CString sProcName;
	if( sPublishForm.Compare( _T("MO") ) == 0 )
	{
		sTableName = _T("BO_BOOK_TBL");
		sNewWorkingStatus = _T("BOL215O");
		sIDXTblName = _T("IDX_BO_TBL");
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		sTmpIDXTblName = _T("IDX_TMP_BO_TBL");
		//=====================================================		
		sProcName = _T("TESTWORKSTATUS");
	}
	else
	{
		sTableName = _T("SE_BOOK_TBL");
		sNewWorkingStatus = _T("SEL315O");
		sIDXTblName = _T("IDX_SE_TBL");
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		sTmpIDXTblName = _T("IDX_TMP_SE_TBL");
		//=====================================================
		sProcName = _T("TESTSEWORKSTATUS");
	}


	/*
	// 프레임 생성
	// 1. 책정보 UPDATE
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 0 );
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , sNewWorkingStatus );
	pDM->AddDBFieldData( _T("PREV_WORKING_STATUS") , _T("STRING") , sWorkingStatus );
	pDM->MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->InitDBFieldData();

	// 김용배 추가
	CString speciesKey = pDM->GetCellData( _T("종정보KEY"), nDMIndex );
	CString query;
	query.Format( 
				_T("UPDATE %s SET WORKING_STATUS = %s( %s ) WHERE REC_KEY = %s;\n")
				_T("INSERT INTO %s( SPECIES_KEY, IDX_WORK_FLAG, INPUT_DATE, REC_KEY ) VALUES( %s, 'I', SYSDATE, ESL_SEQ.NEXTVAL );"),
				sTableName, sProcName, speciesKey, speciesKey, sTmpIDXTblName, speciesKey );
	if( !speciesKey.IsEmpty() )
		pDM->AddFrame( query );
	*/

	
	// 1.1. TmpDM에 기록
	CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1 );
	CLocCommonAPI::GetWorkingStatusDescToCode( sNewWorkingStatus , 1 );

	ids = pTmpDM->SetCellData( _T("배가상태") , sNewWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->SetCellData( _T("이전자료상태") , sWorkingStatus , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("DeliveryDBProc") ); 

	// 2. 배달 정보 UPDATE
	CString sDeliveryRecKey;
	ids = pTmpDM->GetCellData( _T("배달정보KEY") , 0 , sDeliveryRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("DeliveryDBProc") ); 

	CString sDeliveryDate;	
	CTime tsDeliveryDate = CTime::GetCurrentTime();
	sDeliveryDate.Format(
		_T("TO_DATE('%s %0d:%d:%d', 'YYYY/MM/DD HH24:MI:SS')"), 
		sInputDate, tsDeliveryDate.GetHour(), tsDeliveryDate.GetMinute(), tsDeliveryDate.GetSecond());


	pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("2") );
// 	pDM->AddDBFieldData( _T("DELIVERY_DATE") , _T("STRING") , sInputDate );
	pDM->AddDBFieldData( _T("DELIVERY_DATE") , _T("NUMERIC") , sDeliveryDate );
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	pDM->MakeUpdateFrame( _T("CO_DELIVERY_TBL") , _T("REC_KEY") , _T("NUMERIC") , sDeliveryRecKey );
	pDM->InitDBFieldData();

	// 2. 1 TmpDM에 기록
	ids = pTmpDM->SetCellData( _T("배달상태") , _T("배달완료"), 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->SetCellData( _T("배달일자") , sInputDate , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("DeliveryDBProc") ); 

	// 3. 대출/반납 정보 생성
	CTime tLoanDate( _ttoi(sInputDate.Mid(0,4)) , _ttoi(sInputDate.Mid(5,2)) , _ttoi(sInputDate.Mid(8,2)) , 0 , 0 , 0 );
	// 공휴일인지 검사
	INT nLoanPeriod = _ttoi(sLoanPeriod);
	CCheckLoanBook CheckValue( m_pParentMgr , m_pLoanManageValue );
	CheckValue.GetAddHolyDay(nLoanPeriod);

	CTimeSpan tPeriod( nLoanPeriod , 0 , 0, 0);

	tLoanDate += tPeriod;

	CString sReturnPlanDate = tLoanDate.Format( _T("%Y/%m/%d") );

	pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sLoanRecKey );
	pDM->AddDBFieldData( _T("BOOK_KEY") , _T("NUMERIC") , sBookRecKey );
	pDM->AddDBFieldData( _T("USER_KEY") , _T("NUMERIC") , sUserRecKey );
	pDM->AddDBFieldData( _T("PUBLISH_FORM_CODE") , _T("STRING") , sPublishForm );
	pDM->AddDBFieldData( _T("LOAN_TYPE_CODE") , _T("STRING") , _T("0") );
	pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("0") );
	pDM->AddDBFieldData( _T("RETURN_PLAN_DATE") , _T("STRING") , sReturnPlanDate );
// 	pDM->AddDBFieldData( _T("LOAN_DATE") , _T("STRING") , sInputDate );
	pDM->AddDBFieldData( _T("LOAN_DATE") , _T("NUMERIC") , sDeliveryDate );
	// 2009.01.08 ADD BY CJY : 배달처리시에도 대출한곳을 KOLASIII로 설정
	pDM->AddDBFieldData( _T("L_DEVICE"), _T("STRING"), _T("KOLASIII"));
	// 2009.04.02 ADD BY PWR : 반납처리자 추가
	pDM->AddDBFieldData( _T("L_WORKER"), _T("STRING"), m_pParentMgr->m_pInfo->USER_ID);

	// 입력할게 더 많다.. ㅡㅡ;; 통계 정보와 관련해서..
	// (M_2004-1109_HAN)대출시 입력되는 모든 정보를 입력해 주어야한다.
	const INT nEtcDMColumnCnt = 15;
	CString sEtcDBFieldName[nEtcDMColumnCnt] =
	{
		_T("SHELF_LOC_CODE")		, _T("CLASS_NO_TYPE")	, _T("CLASS_NO")			, _T("MANAGE_CODE")				, _T("REG_CODE")	, 
		_T("REG_NO")				, _T("SPECIES_KEY")		, _T("USER_CLASS_CODE"), _T("USER_POSITION_CODE") , _T("MEMBER_CLASS"),
		_T("SEPARATE_SHELF_CODE")	, _T("TITLE")			, _T("AUTHOR")				, _T("PUBLISHER")				, _T("CALL_NO")
	};
	CString sEtcDMColumnName[nEtcDMColumnCnt] = 
	{
		_T("자료실구분")			, _T("분류기호구분"), _T("분류기호")	, _T("관리구분")	, _T("등록구분"),
		_T("등록번호")				, _T("종정보KEY")	, _T("대출직급정보"), _T("대출소속정보"), _T("회원구분"),
		_T("IBS_청구_별치기호_코드"), _T("서명")		, _T("저작자")		, _T("발행자")		, _T("청구기호")
	};
	CString sEtcValue[nEtcDMColumnCnt];
	pTmpDM->GetCellData( sEtcDMColumnName , nEtcDMColumnCnt , sEtcValue , 0);
	// 2006-04-08 ADD BY PDW(Jr) ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Desc를 Code로 변경한다
	// 직급정보
	m_pParentMgr->m_pInfo->pCodeMgr->ConvertDescToCode( _T("대출직급정보"), sEtcValue[7], sEtcValue[7]);
	// 소속정보	
	m_pParentMgr->m_pInfo->pCodeMgr->ConvertDescToCode( _T("대출소속정보"), sEtcValue[8], sEtcValue[8]);
    // ----------------------------------------------------------------------------------------------------------
	pDM->AddDBFieldData(_T("DELAY_CNT") , _T("NUMERIC") , _T("0") )	;//반납연기횟수
	for( INT i = 0 ; i < nEtcDMColumnCnt ; i++ )
		pDM->AddDBFieldData(sEtcDBFieldName[i] , _T("STRING") , sEtcValue[i] )	;

	pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	pDM->MakeInsertFrame( _T("LS_WORK_T01") );
	pDM->InitDBFieldData();

	// 3. 2 TmpDM에 기록
	ids = pTmpDM->SetCellData( _T("대출/반납정보KEY") ,sLoanRecKey, 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("DeliveryDBProc") ); 
	ids = pTmpDM->SetCellData( _T("발행형태구분") ,sPublishForm, 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("DeliveryDBProc") ); 

	// 4. 이용자 정보 수정
	// 2010.10.11 ADD BY KSJ : LAST_LOAN_DATE UPDATE
	pDM->AddDBFieldData( _T("LAST_LOAN_DATE") , _T("NUMERIC") , sDeliveryDate );
	pDM->AddDBFieldData( _T("LOAN_COUNT") , _T("NUMERIC") , _T("LOAN_COUNT+1") );
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	pDM->MakeUpdateFrame( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sUserRecKey );
	pDM->InitDBFieldData();

	// 5. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("DeliveryDBProc") ); 

	ids = pDM->EndFrame();

	// 2010.10.11 ADD BY KSJ : 통합도서서비스 대출 처리
	CString strKLMemberYN;
	pTmpDM->GetCellData(_T("통합회원여부"), 0, strKLMemberYN);
	if(_T("Y")==m_pLoanManageValue->m_sIsUnityLoanService && _T("Y")==strKLMemberYN )
	{
		CString strFormatLoanDate, strFormatReturnPlanDate;
		strFormatLoanDate.Format(_T("%s %0d:%d:%d"), sInputDate, tsDeliveryDate.GetHour(), tsDeliveryDate.GetMinute(), tsDeliveryDate.GetSecond());
		strFormatReturnPlanDate.Format(_T("%s 23:59:59"), sReturnPlanDate);

		// 대출 프레임
		CFrameWriter Writer(m_pParentMgr->m_pInfo);
		Writer.setCommand(_T("IL_K10_LOC_B01_SERVICE"));
		Writer.addRecord();
		CString strGetData;
		ids = pTmpDM->GetCellData(_T("대출자번호설명"), 0, strGetData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, _T("DeliveryDBProc"));
		Writer.addElement(_T("통합대출자번호"), strGetData);
		Writer.addElement(_T("대출도서관"), m_pParentMgr->m_pInfo->LIB_CODE);
		Writer.addElement(_T("대출일"), strFormatLoanDate);
		Writer.addElement(_T("반납예정일"), strFormatReturnPlanDate);
		Writer.addElement(_T("상태"), _T("0"));
		Writer.addElement(_T("등록번호"), sEtcValue[5]);
		Writer.addElement(_T("서명"), sEtcValue[11]);
 	 	Writer.addElement(_T("저자"), sEtcValue[12]);
 	 	Writer.addElement(_T("발행자"), sEtcValue[13]);
		Writer.addElement(_T("청구기호"), sEtcValue[14]);
		Writer.addElement(_T("최종대출일"), strFormatLoanDate);
		ids = pTmpDM->GetCellData(_T("IBS_청구_권책기호"), 0, strGetData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, _T("DeliveryDBProc"));
		Writer.addElement(_T("편권차"), strGetData);
		Writer.addElement(_T("로컬KEY"), sLoanRecKey);

		// 프레임 발송
 	 	CFrameSender Sender(m_pParentMgr->m_pInfo);
 	 	Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("배달자료관리-배달처리"), __WFILE__, __LINE__));
	}

	// 2009.05.29 UPDATE BY CJY : 대출자공유 주석처리 해제
	// --------------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------------
	// 6. 대출 공유 대출/반납 정보 , 대출자 변경시 센터 도서관에 알리기 
	// (A_2004-0913_HAN...)
	// --------------------------------------------------------------------------------------
	// 6.1  대출/반납 변경 정보 기록
	SetTempData( SHARE_CO_LOAN_TBL_INSERT_REC_KEY , CComVariant(sLoanRecKey) );

	// 6.2  대출자 변경 정보 기록
	SetTempData( SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY , CComVariant(sUserRecKey) );
	SetTempData( SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS , CComVariant(_T("LOAN_COUNT")) );

	return 0;
}



INT CBL_BO_LOC_34::GetDeliveryInputDate( CString &sInputDate , CString &sLoanPeriod)
{
	CString sCMAlias = _T("CM_BO_LOC_3440");

	INT ids;

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("배달완료일자") , sInputDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetReceiptInputDate") );
	sInputDate.TrimRight(); sInputDate.TrimLeft();

	ids = m_pParentMgr->GetControlData( sCMAlias , _T("대출기한") , sLoanPeriod );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetReceiptInputDate") );
	sLoanPeriod.TrimRight(); sLoanPeriod.TrimLeft();

	return 0;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2005-12-08 ~ 2005-12-08
   @Author   : Park, Daewoo(Jr)   
   @WorkType : Modify
 	 
   [   함수설명   ] 
      배달자료관리 삭제 업무
   [  PARAMETERS  ]
      NULL 

   [ RETURN VALUE ]
      1    : 취소
      0    : 성공 
      음수 : 실패

   [   미니스펙   ]
      1. 그리드에 선택된 자료가 있는지 확인
	  2. 삭제 업무 진행
	  3. 마무리 작업
--------------------------------------------------------------------------------------------*/
INT CBL_BO_LOC_34::DeleteDelivery()
{
	BEGIN_SBL();

	INT ids;
	
	// ----------------------------------------------------------------------------
	// 1. 그리드에 선택된 자료가 있는지 확인     
	// ----------------------------------------------------------------------------
	CESL_Grid *pGrid = GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CBL_BO_LOC_34::DeleteDelivery()") );

	pGrid->SelectMakeList();
	pGrid->SelectGetTailPosition();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("자료를 선택해주십시오.") );
		return 1;
	}

	// 2005-12-08 ADD BY PDW(Jr) ==================================================
	if ( AfxMessageBox( _T("선택한 자료를 삭제 하시겠습니까 ?"), MB_ICONQUESTION | MB_YESNO ) == IDNO )
		return 1;
	// ============================================================================

	// 2005-12-08 REM BY PDW(Jr) ==================================================
	// 사용하지 않는다 
	// DM 참조 선택
	/*
	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_BO_LOC_34::DeleteDelivery()") );
	*/
	// ============================================================================

	// ----------------------------------------------------------------------------
	// 2. 삭제 업무 진행   
	// ----------------------------------------------------------------------------
	// 프로그래스바 생성
	CLocProgressBarDlg* pProgressBar;
	pProgressBar = CreateProgressBarDlg(nSelectCnt);
	if ( pProgressBar == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CBL_BO_LOC_34::DeleteDelivery()") );

	INT nSuccessCnt = 0;	// 작업에 성공한 자료 수 
	CString sWorkingStatus; // 자료상태
	CString sStatus ;       // 배달 상태
	INT nIndex;

	// DM 참조 선택
	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RegRepair") );

	for( INT i = 0;i < nSelectCnt;i++ )
	{
		// 진행상황 보여주기
		pProgressBar->ChangeProgressBar(i);

		nIndex = pGrid->SelectGetIdx();

		ids = pDM->GetCellData( _T("배달상태") , nIndex , sStatus );
		if ( ids ) return ids;

		if ( sStatus == _T("배달완료") ) 
		{
			pGrid->SelectGetPrev();
			continue;
		}

		//  DB작업
		ids = DeleteDBProc( nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CBL_BO_LOC_34::DeleteDelivery()") );
		if( ids > 0 ) return ids;

		// 2010.02.18 ADD BY PWR : 여러자료 삭제에러 수정
		pGrid->SelectGetPrev();
		nSuccessCnt++;
	}
	
	// ----------------------------------------------------------------------------
	// 3. 마무리 작업
	// ----------------------------------------------------------------------------
	// 프로그래스바 죽임
	pProgressBar->ChangeProgressBar( i );
	DeleteProgressBarDlg( pProgressBar );

	m_pParentMgr->AllControlDisplay( _T("CM_BO_LOC_3400") );

	CESL_DataMgr *pSBLDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3400") );
	INT nRowCount = pSBLDM->GetRowCount();
	END_SBL(nRowCount);

	// 결과 메시지
	CString sResultMsg;	
	if( nSelectCnt == nSuccessCnt )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료를 삭제 처리 하였습니다.") , nSelectCnt );
	else if( nSuccessCnt == 0 )
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료가 삭제 처리 할 수 없는 상태이기 때문에 작업에 실패하였습니다.") , nSelectCnt );
	else
		sResultMsg.Format( _T("선택하신 총 '%d'개의 자료중 '%d'개의 자료를 삭제 처리하였습니다.\r\n")
						   _T("'%d'개의 자료는 자료상태가 삭제 처리 할 수 없는 상태이기 때문에 작업에 실패하였습니다.") , 
						   nSelectCnt , nSuccessCnt , nSelectCnt-nSuccessCnt );

	AfxMessageBox( sResultMsg );

	return 0;
}

INT CBL_BO_LOC_34::DeleteDBProc( INT nDMIndex )
{
	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RegRepairDBProc") );

	pDM->StartFrame();	

	// 프레임 생성
	pDM->InitDBFieldData();
	// 2. 배달 정보 UPDATE
	CString sDeliveryRecKey;
	CString sBookKey;
	CString sPublishFormCode;
	ids = pDM->GetCellData( _T("배달정보KEY") , nDMIndex , sDeliveryRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepairDBProc") ); 

	ids = pDM->GetCellData( _T("책정보KEY") , nDMIndex , sBookKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepairDBProc") ); 

	ids = pDM->GetCellData( _T("발행형태구분") , nDMIndex , sPublishFormCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("RegRepairDBProc") ); 	

	pDM->MakeDeleteFrame( _T("CO_DELIVERY_TBL") , _T("REC_KEY") , _T("NUMERIC") , sDeliveryRecKey );
	
	// 2009.03.18 UPDATE BY PWR : WORKING_STATUS가 배달자료(BOL215O)일때만 비치로 변경
	// DM에서 가져올수도있는데 삭제의 경우 그시점의 상태값이 어떻게 변할지 모르지 디비에서 바로가져온다.
	CString sWorkingStatus;
	CString sQuery;
	if ( sPublishFormCode == _T("MO") )
	{
		sQuery.Format( _T("SELECT WORKING_STATUS FROM BO_BOOK_TBL WHERE REC_KEY=%s"), sBookKey );
 		ids = pDM->GetOneValueQuery( sQuery , sWorkingStatus );
		sWorkingStatus.TrimLeft(); sWorkingStatus.TrimRight();
		if(_T("BOL215O") == sWorkingStatus)
		{
			pDM->InitDBFieldData();
			pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("BOL112N") );		
			pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
			pDM->MakeUpdateFrame( _T("BO_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookKey );
		}
	}
	else
	{
		sQuery.Format( _T("SELECT WORKING_STATUS FROM SE_BOOK_TBL WHERE REC_KEY=%s"), sBookKey );
 		ids = pDM->GetOneValueQuery( sQuery , sWorkingStatus );
		sWorkingStatus.TrimLeft(); sWorkingStatus.TrimRight();
		if(_T("SEL315O") == sWorkingStatus)
		{
			pDM->InitDBFieldData();
			pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL212N") );		
			pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
			pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookKey );
		}
	}
// 	if ( sPublishFormCode == _T("MO") )
// 	{
// 		pDM->InitDBFieldData();
// 		pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("BOL112N") );		
// 		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
// 		pDM->MakeUpdateFrame( _T("BO_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookKey );
// 	}
// 	else
// 	{
// 		pDM->InitDBFieldData();
// 		pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("STRING") , _T("SEL212N") );		
// 		pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
// 		pDM->MakeUpdateFrame( _T("SE_BOOK_TBL") , _T("REC_KEY") , _T("NUMERIC") , sBookKey );
// 	}

	// 3. 프레임 DB에 Send~
	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("RegRepairDBProc") ); 

	pDM->DeleteRow( nDMIndex );

	return 0;
}

/// 대출이 가능한 상태인지 알아본다.
// 1. 대출정지일이 지났는지 검사
// 2. 총대출책수를 초과했는지 검사
// 3. 회원상태 검사 (제적, 회원증분실회원 대출제한)		// 2008.03.26 ADD BY PWR
// 4. 재대출 허용 여부 검사								// 2008.06.17 ADD BY PWR
INT CBL_BO_LOC_34::IsLoan( INT nIndex )
{
	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("IsLoan") );

	// 이용자KEY를 얻어온다.
	CString sUserKey;
	ids = pDM->GetCellData( _T("이용자KEY") , nIndex , sUserKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("IsLoan") );

	// 이용자 직급 , 현재 대출 책수 , 대출 정지일을 얻어온다.
	CString sWhere;
	sWhere.Format( _T("REC_KEY = %s") , sUserKey );

	CESL_DataMgr *pUserDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3400_USER_INFO") );
	if( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("IsLoan") );
	
	ids = pUserDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("IsLoan") ); 


	CString sLoanClassCode , sLoanBookCnt , sLoanStopDate;
	ids = pUserDM->GetCellData( _T("대출직급정보") , 0 , sLoanClassCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("IsLoan") ); 
	ids = pUserDM->GetCellData( _T("총대출책수") , 0 , sLoanBookCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsLoan") ); 
	ids = pUserDM->GetCellData( _T("대출정지일") , 0 , sLoanStopDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("IsLoan") ); 
	
	// 1. 대출정지일이 지났는지 검사
	if( !sLoanStopDate.IsEmpty() )
	{
		//++2008.05.13 UPDATE BY PWR {{++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// CTime -> COleDateTime 으로 변경 ( 대출정지일이 커서 에러발생 )
		COleDateTime tTmpCurrent = COleDateTime::GetCurrentTime();
		//CTime tTmpCurrent = CLocCommonAPI::GetDBTime(m_pParentMgr);
		COleDateTime tCurrent = COleDateTime( tTmpCurrent.GetYear() , tTmpCurrent.GetMonth() , tTmpCurrent.GetDay() , 0 , 0 , 0 );
		COleDateTime tLoanStopDate = COleDateTime( _ttoi(sLoanStopDate.Mid(0,4)) , _ttoi(sLoanStopDate.Mid(5,2)) , _ttoi(sLoanStopDate.Mid(8,2)) , 0 , 0 , 0 );
		//--2008.05.13 UPDATE BY PWR ------------------------------------------------------------}}

		if( tCurrent <= tLoanStopDate )
			return 1;
	}

	// 2. 총대출책수를 초과했는지 검사
	// 17/08/07 김혜영 : 배달자료 대출권수에 부록자료가 포함되지 않도록 수정
//*/ ADD ---------------------------------------------------------------------------
	CString sExAppedixLoanCnt = _T("");
	// 부록자료 대출책수 포함여부확인
	if( m_pLoanManageValue->m_sIsLoanAppendix.Compare( _T("Y") ) == 0  && m_pLoanManageValue->m_sIsIncludeLoanCnt.Compare( _T("N") ) == 0 )
	{ // 부록자료 포함안할때
		CString sBookKey  = _T("");
		ids = pDM->GetCellData( _T("책정보KEY") , nIndex , sBookKey );
		if(!sBookKey.IsEmpty())
		{
			CString sSql, sValue;				
			sSql.Format( _T("SELECT COUNT(1) FROM LS_WORK_T01 CL, BO_BOOK_TBL B WHERE CL.STATUS IN ('0','2') AND CL.USER_KEY=%s AND CL.BOOK_KEY = B.REC_KEY AND B.BOOK_APPENDIX_FLAG <> 'A' AND CL.MANAGE_CODE=UDF_MANAGE_CODE"), sUserKey);							
			ids = pUserDM->GetOneValueQuery( sSql , sValue );
			sExAppedixLoanCnt = sValue;

			sSql.Format( _T("SELECT BOOK_APPENDIX_FLAG FROM BO_BOOK_TBL WHERE REC_KEY = %s "), sBookKey);							
			ids = pUserDM->GetOneValueQuery( sSql , sValue );
			INT nExAppedixLoanCnt = _ttoi(sExAppedixLoanCnt);
			if(sValue == "A" &&  nExAppedixLoanCnt > 0) //부록자료이면
			{
				sExAppedixLoanCnt.Format(_T("%d"),nExAppedixLoanCnt-1);
			}
		}
	}
//*/ END ---------------------------------------------------------------------------
	CString sResult;
	// 2009.03.24 UPDATE BY PWR : 통합대출가능권수 사용유무에 따른 처리
	if(m_pLoanManageValue->m_sUnityLoanCntYN == _T("Y"))
	{ // 사용함
		// 도서관별 자관대출가능권수
		ids = m_pLoanManageValue->GetIndividualUserPosition( sLoanClassCode, _T("총대출가능수"), sResult );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-8 , _T("IsLoan()") ); 
		}
		if( sResult.IsEmpty() )
		{
			sResult = _T("0");
		}
		INT nMaxLoanBookCnt = _ttoi( sResult );
		
		if( nMaxLoanBookCnt < (_ttoi(sLoanBookCnt)+1) ) 
		{
			// 17/08/07 김혜영 : 배달자료 대출권수에 부록자료가 포함되지 않도록 수정
//*/ ADD ---------------------------------------------------------------------------
			if( sExAppedixLoanCnt.IsEmpty() || nMaxLoanBookCnt < (_ttoi(sExAppedixLoanCnt)+1) )
//*/ END ---------------------------------------------------------------------------	
				return 2;
		}
	}
	else
	{ // 사용안함
		// 자관대출가능권수
		ids = m_pLoanManageValue->GetUserPosition( sLoanClassCode, _T("총대출가능수"), sResult );
 		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-8 , _T("IsLoan()") ); 
		}
		if( sResult.IsEmpty() )
		{
			sResult = _T("0");
		}
		
		INT nMaxLoanBookCnt = _ttoi( sResult );

		if( nMaxLoanBookCnt < (_ttoi(sLoanBookCnt)+1) ) 
		{
			// 17/08/07 김혜영 : 배달자료 대출권수에 부록자료가 포함되지 않도록 수정
//*/ ADD ---------------------------------------------------------------------------
			if( sExAppedixLoanCnt.IsEmpty() || nMaxLoanBookCnt < (_ttoi(sExAppedixLoanCnt)+1) )
//*/ END ---------------------------------------------------------------------------	
				return 2;
		}
	}
//  	ids = m_pLoanManageValue->GetUserPosition( sLoanClassCode , _T("총대출가능수") , sResult );
//  	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,-8 , _T("IsLoan()") ); 
// 	
// 	if( sResult.IsEmpty() ) sResult = _T("0");
// 	
// 	INT nMaxLoanBookCnt = _ttoi( sResult );
// 
// 	if( nMaxLoanBookCnt < (_ttoi(sLoanBookCnt)+1) ) 
// 		return 2;
	
	//++2009.01.09 ADD BY CJY {{++
	// 비회원에 대한 배달처리제한
	// 정상회원이 아니면
	CString strUserClass;
	ids = pDM->GetCellData( _T("불량회원구분") , nIndex, strUserClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("IsLoan") ); 
 	if(_T("1") == strUserClass) 
	{ 
		// 대출정지회원이라도 대출제한일이 현재일보다 이전이면 대출정지회원으로 보지 않는다.
		CString strLoanStopDate;
		ids = pDM->GetCellData(_T("대출정지일"), nIndex, strLoanStopDate);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("SettingDelayDay") );
		
		COleDateTime oleLoanStopDate;
		int nYear = 1, nMonth = 1, nDay = 1;
		_stscanf(strLoanStopDate.GetBuffer(0), _T("%d/%d/%d"), &nYear, &nMonth, &nDay);
		
		oleLoanStopDate.SetDate(nYear, nMonth, nDay);
		
		COleDateTime oleToday = COleDateTime::GetCurrentTime();
		oleToday.SetDate(oleToday.GetYear(), oleToday.GetMonth(), oleToday.GetDay());
		
		if(oleToday > oleLoanStopDate)
		{
		}
		else return 15;
	}
	if(_T("2") == strUserClass) { return 10; }
	if(_T("3") == strUserClass) { return 11; }

	// 회원구분
	CString strMemberClass;
	ids = pDM->GetCellData( _T("회원구분") , nIndex, strMemberClass);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("IsLoan") ); 
	if(_T("1") == strMemberClass) 
	{ // 비회원
		return 16; 
	}
	// 2008.05.28 ADD BY PWR : 준회원은 회원인증후에 대출이 가능하므로 제외
	else if(_T("2") == strMemberClass)
	{ // 준회원
		return 17; 
	}

	// 회원증분실회원이면
	CString strLostUserCard;
	ids = pDM->GetCellData( _T("회원증분실여부") , nIndex, strLostUserCard);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("IsLoan") ); 
	if(_T("Y") == strLostUserCard) { return 14; }
	
	
//DEL	이전소스
//DEL 	//++2008.04.15 ADD BY PWR {{++
//DEL 	// 3. 회원상태 검사 (제적, 탈퇴회원 대출제한)
//DEL 	CString sSql;
//DEL 	sSql.Format( _T("SELECT USER_CLASS FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), sUserKey );
//DEL 	ids = pDM->GetOneValueQuery( sSql , sResult );
//DEL 	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("IsLoan()") );
//DEL 	// 제적회원
//DEL 	if( _T("2") == sResult )
//DEL 	{
//DEL 		return 10;
//DEL 	}
//DEL 	// 탈퇴회원
//DEL 	else if( _T("3") == sResult )
//DEL 	{
//DEL 		return 11;
//DEL 	}
//DEL 	//--2008.04.15 ADD BY PWR --}}
//DEL 	//++2008.07.15 ADD BY PWR {{++
//DEL 	// 대출증분실회원 대출제한
//DEL 	sResult = m_pLoanManageValue->m_sLoanIsLostUserCard;
//DEL 	if(sResult == _T("Y") )
//DEL 	{
//DEL 		sSql.Format( _T("SELECT LOST_USER_CARD FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), sUserKey );
//DEL 		ids = pDM->GetOneValueQuery( sSql , sResult );
//DEL 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("IsLoan()") );
//DEL 		if( _T("Y") == sResult ) return 14;
//DEL 	}
//DEL 	//--2008.07.15 ADD BY PWR --}}
	//--2009.01.09 ADD BY CJY --}}


	//++2008.06.17 ADD BY PWR {{++
	// 4. 재대출 허용 여부 검사
	ids = IsReLoanBook( nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -12, _T("IsLoan()") );
	if( ids > 0 )
	{
		// 재대출했던 자료인데 허용안한다면 대출가능상태가 아니다.
		if( ids == 12 || ids == 13 )
			return ids;
	}
	//--2008.06.17 ADD BY PWR --}}
	
	return 0;
}



/// 자료 상태를 확인한다.
INT CBL_BO_LOC_34::IsReservation( INT nIndex )
{
	INT ids;
	
	CString sDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("IsReservation") );

	CString sBookKey , sPublishForm;
	ids = pDM->GetCellData( _T("책정보KEY") , nIndex , sBookKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("IsReservation") );

	ids = pDM->GetCellData( _T("발행형태구분") , nIndex , sPublishForm );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("IsReservation") );
	
	CESL_DataMgr *pBookDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3400_BOOK_INFO") );
	if( pBookDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("IsReservation") );

	CString sWhere;
	sWhere.Format( _T("B.REC_KEY = %s") , sBookKey );

	CString sTableName;
	CString sUseObjCodeFieldName;
	if( sPublishForm.Compare(_T("MO")) == 0 )
	{
		sTableName = _T("BO_BOOK_TBL B");
		sUseObjCodeFieldName = _T("B.USE_OBJECT_CODE");
	}
	else if( sPublishForm.Compare(_T("SE")) == 0 )
	{
		sTableName = _T("SE_BOOK_TBL B , IDX_SE_TBL I");
		sUseObjCodeFieldName = _T("I.USE_OBJ_CODE");
		sWhere += _T(" AND B.SPECIES_KEY = I.REC_KEY ");
	}

	pBookDM->TBL_NAME = sTableName;

	CString sColumnAlias , sFieldName , sFieldType , sInitValue , sDataType;
	for( INT k = 0 ; k < pBookDM->RefList.GetCount() ; k++ )
	{
		sColumnAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
		ids = pBookDM->GetColumnRefInfo( k , sColumnAlias , sFieldName , sFieldType , sInitValue , sDataType );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("IsReservation") );
		
		if( sColumnAlias.Compare(_T("이용대상구분")) == 0 )
			sFieldName = sUseObjCodeFieldName;

		ids = pBookDM->SetDataMgrRef( k , sColumnAlias , sFieldName , sFieldType  , sInitValue  , sDataType );			
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("IsReservation") );
	}		
	
	ids = pBookDM->RefreshDataManager( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("IsReservation") );

	// 1. 자료 상태 확인
	CString sWorkingStatus;
	ids = pBookDM->GetCellData( _T("책상태") , 0 , sWorkingStatus );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("IsReservation") );

	// 2005.03.23 UPDATE BY PDJ
	//if( sWorkingStatus.Compare(_T("BOL112N")) !=0 && (sWorkingStatus.Compare(_T("SEL112N")) !=0) && (sWorkingStatus.Compare(_T("SEL212N")) !=0) )
	if( sWorkingStatus.Compare(_T("BOL215O")) !=0 && (sWorkingStatus.Compare(_T("SEL315O")) !=0) && (sWorkingStatus.Compare(_T("SEL212N")) !=0) )
		return 3;
	
	//++2008.06.08 DEL BY PWR {{++
	// 접수처리가 예약보다 먼저 이루어졌으므로 대출처리가 가능해야한다 
	// 접수처리에서 이미 예약자료 확인했으므로 대출처리에서 확인안하도록 한다.
	/*
	// 2. 예약된 자료가 존재하는지 확인
	CString sSql , sResult ;
	sSql.Format( _T("SELECT COUNT(*) FROM LH_STORAGE_V01 WHERE BOOK_KEY=%s AND STATUS = '3'") , sBookKey );
	ids = pBookDM->GetOneValueQuery( sSql , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );

	if( _ttoi(sResult) > 0 )
		return 4;
	*/
	//--2008.06.08 DEL BY PWR --}}

	// 1. 현재 프로그램을 사용하고 있는 도서관의 관리구분과 책의 관리구분값이 다를 경우 
	// 대출을 해줄 것인지 설정값을 읽어온 후 결정
	if( m_pLoanManageValue->m_sIsLoanManageCode.Compare(_T("N"))==0 ) 
	{
		CString sExpManageCode;		
		ids = pBookDM->GetCellData( _T("관리구분") , 0 , sExpManageCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );
		
		CString sManageCode;
		ids = m_pParentMgr->m_pInfo->pCodeMgr->GetDefaultCode( _T("관리구분") , sManageCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );
		
		if( sExpManageCode.Compare(sManageCode) != 0 )
			return 5;
	}
	
	
	// 2. 대출자와 관련없는 책정보에 따라서 대출이 가능한지 검사
	//    - 등록구분 , 별치기호 , 이용제한구분
	const INT nCnt = 3;
	TCHAR *sFieldAlias[nCnt] = {
		_T("등록구분") , _T("별치기호") , _T("이용제한구분")
	};
	CString sCode[nCnt];
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = pBookDM->GetCellData( sFieldAlias[i] , 0 , sCode[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );
	}
	for( i = 0 ; i < nCnt ; i++ )
	{
		CString sValue;
		ids = m_pLoanManageValue->IsLoanCodeYN( sFieldAlias[i], sCode[i] , sValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );
		
		if( sValue.Compare(_T("N")) == 0 )
			return 6+i;
	}

	// 3. 대출자 직급별 설정값에서 대출이 가능한지 불가능한지 검사
	//    - 이용대상자료구분
	CString sMemberDMAlias = _T("DM_BO_LOC_3400_USER_INFO");
	CString sUserClassCode;
	ids = m_pParentMgr->GetDataMgrData( sMemberDMAlias , _T("대출직급정보") , sUserClassCode , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );

	// 이용 대상 자료구분
	CString sExpUseObjCode;
	ids = pBookDM->GetCellData( _T("이용대상구분") , 0 , sExpUseObjCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );
	
	if( sExpUseObjCode.Compare(_T("AD"))==0 )
	{
		CString sUseObjCode;	
		ids = m_pLoanManageValue->GetUserPosition( sUserClassCode , _T("이용대상구분") , sUseObjCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("IsReservation") );

		if( sUseObjCode.Compare(_T("N"))==0 ) 
			return 9;
		
	}
	
	
	return 0;
}

// 로그 파일에 기록
INT CBL_BO_LOC_34::AddingLog( CString sRegNo , CString sUserNo , INT nGridIndex , INT nErrorCode )
{
	_tmkdir(DELIVERY_LOG_DIR_PATH);
	
	CStdioFile fLogFile;
	INT ids = fLogFile.Open( m_sLogFile , CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite | CFile::typeBinary);
	if( ids == 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AddingLog") );

	if( 0 == fLogFile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fLogFile.Write( &cUni, sizeof(TCHAR));
	}
		
	CString sEMsg;
	switch(nErrorCode)
	{
		case -1 :
			sEMsg.Format( _T("접수 완료 상태가 아니므로 배달 처리에 실패했습니다.") );
			break;
		case -2 :
			sEMsg.Format( _T("비치 상태가 아니므로 배달 처리에 실패했습니다.") );
			break;
		case 1 :
			sEMsg.Format( _T("대출정지일이 지나지 않은 이용자이기 때문에 배달 처리에 실패했습니다.") );
			break;
		case 2 :
			sEMsg.Format( _T("이용가능한 총대출책수를 초과했기 때문에 배달 처리에 실패했습니다.") );
			break;
		case 3 :
			sEMsg.Format( _T("비치 상태가 아니므로 배달 처리에 실패했습니다.") );
			break;
		case 4 :
			sEMsg.Format( _T("예약된 자료이기 때문에 배달 처리에 실패했습니다.") );
			break;
		case 5 :
			sEMsg.Format( _T("관리구분이 다른 자료이기 때문에 배달 처리에 실패했습니다.") );
			break;
		case 6 :
			sEMsg.Format( _T("등록구분이 대출할수 없는 자료입니다.") );
			break;
		case 7 :
			sEMsg.Format( _T("별치기호가 대출할수 없는 자료입니다.") );
			break;
		case 8 :
			sEMsg.Format( _T("이용제한구분이 대출할수 없는 자료입니다.") );
			break;
		case 9 :
			sEMsg.Format( _T("성인 자료를 대출할 수 없는 이용자입니다.") );
			break;
		// 2008.04.15 ADD BY PWR
		case 10 :
			sEMsg.Format( _T("회원상태가 제적회원이기 때문에 배달 처리에 실패했습니다.") );
			break;
		case 11 :
			sEMsg.Format( _T("회원상태가 탈퇴회원이기 때문에 배달 처리에 실패했습니다.") );
			break;
		// 2008.06.17 ADD BY PWR
		case 12 :
			sEMsg.Format( _T("예전에 대출 되었던 책이기 때문에 배달 처리에 실패했습니다.") );
			break;
		case 13 :
			sEMsg.Format( _T("가족중 한명이 예전에 대출했었던 책이기 때문에 배달 처리에 실패했습니다.") );
			break;
		// 2008.07.15 ADD BY PWR
		case 14 :
			sEMsg.Format( _T("대출증 분실회원이기 때문에 배달 처리에 실패했습니다.") );
			break;
		case 15 :
			sEMsg.Format( _T("대출정지회원이기 때문에 배달 처리에 실패했습니다.") );
			break;
		case 16 :
			sEMsg.Format( _T("비회원이기 때문에 배달 처리에 실패했습니다.") );
			break;
		// 2008.05.28 ADD BY PWR : 준회원은 회원인증후에 대출이 가능하므로 제외
		case 17 :
			sEMsg.Format( _T("준회원이기 때문에 배달 처리에 실패했습니다.") );
			break;
		default:
			sEMsg.Format( _T("알수 없는 문제.") );
			break;
	}
	
	CString sMsg;
	sMsg.Format(_T("그리드 번호 [%d] , 등록번호[%s] , 이용자번호[%s] : %s \r\n") , nGridIndex+1 , sRegNo , sUserNo , sEMsg);
	
	fLogFile.SeekToEnd();
	fLogFile.WriteString( sMsg );
	
	fLogFile.Close();
	
	return 0;
}

// 로그 파일 초기화
INT CBL_BO_LOC_34::InitLog( CString sTitle )
{
	_tmkdir(DELIVERY_LOG_DIR_PATH);

	// 2010.08.27 UPDATE BY PWR : 로그인시 기록한 현재일자 사용
	CString strGetData;
	m_sLogFile.Format( _T("%s[%s].txt") , DELIVERY_LOG_FILE_PATH , (CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData)).Format(_T("%Y%m%d%H%M%S")) );
// 	m_sLogFile.Format( _T("%s[%s].txt") , DELIVERY_LOG_FILE_PATH , (CLocCommonAPI::GetDBTime(m_pParentMgr)).Format(_T("%Y%m%d%H%M%S")) );

	CStdioFile fLogFile;
	INT ids = fLogFile.Open( m_sLogFile , CFile::modeCreate|CFile::modeWrite | CFile::typeBinary);
	if( ids == 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("InitLog") );
	
	if( 0 == fLogFile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fLogFile.Write( &cUni, sizeof(TCHAR));
	}
	
	fLogFile.WriteString( sTitle );

	fLogFile.Close();

	return 0;
}

// 2008.06.17 ADD BY PWR
// ** 재대출 여부 조사
INT CBL_BO_LOC_34::IsReLoanBook( INT nIndex )
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3400") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -1, _T("IsReLoanBook()") );

	// 개인재대출
	CString sValue;
	sValue = m_pLoanManageValue->m_sIsOneManReLoan;

	// 재대출제한일
	CString sReLoanLimitDay;
	sReLoanLimitDay = m_pLoanManageValue->m_sReLoanLimitDay;

	// 개인 재대출
	// 개인 재대출 N + 가족 재대출 Y -> 개인 재대출만 검사
	// 개인 재대출 N + 가족 재대출 N -> 개인, 가족
	INT nReLoanMode		= 0;
	INT nReLoanYN		= 0;
	INT nFamilyReLoanYN = 0;

	if( sValue.Compare(_T("Y")) != 0 )
	{// 개인 재대출 허용안함
		nReLoanYN = 1;
		sValue = m_pLoanManageValue->m_sIsFamilyReLoan;
		if( sValue.Compare(_T("Y")) != 0 )
		{//가족 재대출 허용안함
			nFamilyReLoanYN = 1;
		}
	}
	else
	{// 개인 재대출 허용
		nReLoanYN = 0;	
	}

	CString sSql;
	CString sBookRecKey;
	CString sUserRecKey;
	CString sFamilyRecKey;

	ids = pDM->GetCellData( _T("책정보KEY"), nIndex, sBookRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -3, _T("IsReLoanBook()") );
	ids = pDM->GetCellData( _T("이용자KEY"), nIndex, sUserRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -4, _T("IsReLoanBook()") );
	ids = pDM->GetCellData( _T("가족KEY"), nIndex, sFamilyRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -5, _T("IsReLoanBook()") );

	INT nCount=0;
	INT nOneManCnt=0;
	INT nFamilyCnt=0;
	
	CString strLimitWhere;
	if( _ttoi(sReLoanLimitDay) != 0 )
	{
		//++2008.09.02 DEL BY LKS {{++
//REMOVE▶ 		if( m_pParentMgr->m_pInfo->MODE == 10000 || m_pParentMgr->m_pInfo->MODE == 30000 )
//REMOVE▶ 			strLimitWhere = _T("AND to_date(now(),'YYYY/MM/DD') - to_date(CL.RETURN_DATE,'YYYY/MM/DD') < ") + sReLoanLimitDay ;
//REMOVE▶ 		else
		//--2008.09.02 DEL BY LKS --}}
			strLimitWhere = _T("AND TO_DATE(SYSDATE,'YYYY/MM/DD') - TO_DATE(CL.RETURN_DATE,'YYYY/MM/DD') < ") + sReLoanLimitDay ;
	}

	sSql.Format( _T(" SELECT SUM(CNT) FROM ( ")
		         _T(" SELECT COUNT(1) AS CNT FROM CO_LOAN_USER_TBL CLU, LS_WORK_T01 CL ")
				 _T(" WHERE CL.STATUS IN ('0','2') ")
				 _T(" AND CL.USER_KEY = CLU.REC_KEY ")
				 _T(" AND CL.BOOK_KEY = %s ")
				 _T(" AND CL.USER_KEY = %s ")
				 _T(" UNION ")
				 _T(" SELECT COUNT(1) AS CNT FROM CO_LOAN_USER_TBL CLU, LH_STORAGE_V01 CL ")
				 _T(" WHERE CL.STATUS IN ('1','5','6') ")
				 _T(" AND CL.USER_KEY = CLU.REC_KEY ")
				 _T(" AND CL.BOOK_KEY = %s ")
				 _T(" AND CL.USER_KEY = %s ")
				 _T(" %s )"), sBookRecKey , sUserRecKey, sBookRecKey , sUserRecKey , strLimitWhere );

	ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr , pDM , sSql , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -6, _T("IsReLoanBook()") );

	nOneManCnt = _ttoi(sValue);

	if( !sFamilyRecKey.IsEmpty() )
	{
		sSql.Format( _T(" SELECT SUM(CNT) FROM ( ")
					 _T(" SELECT COUNT(1) AS CNT FROM CO_LOAN_USER_TBL CLU, LS_WORK_T01 CL ")
					 _T(" WHERE CL.STATUS IN ('0','2') ")
					 _T(" AND CL.USER_KEY = CLU.REC_KEY ")
					 _T(" AND CL.BOOK_KEY = %s ")
					 _T(" AND ( CLU.FID_KEY = %s ) ")
				     _T(" UNION ")
				     _T(" SELECT COUNT(1) AS CNT FROM CO_LOAN_USER_TBL CLU, LH_STORAGE_V01 CL ")
					 _T(" WHERE CL.STATUS IN ('1','5','6') ")
					 _T(" AND CL.USER_KEY = CLU.REC_KEY ")
					 _T(" AND CL.BOOK_KEY = %s ")
					 _T(" AND ( CLU.FID_KEY = %s ) ")
					 _T(" %s )"), sBookRecKey , sFamilyRecKey, sBookRecKey , sFamilyRecKey , strLimitWhere );

		ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr , pDM , sSql , sValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -7, _T("IsReLoanBook()") );

		nFamilyCnt = _ttoi(sValue);
	}

	if( nOneManCnt > 0 )
	{
		if( nReLoanYN == 1 ) 
		{// 개인재대출 허용안함
			return 12;
		}
	}
	else if( nFamilyCnt > 0 )
	{
		if( nFamilyReLoanYN == 1 )
		{// 가족재대출 허용안함
			return 13;
		}
	}

	return 0;

EFS_END
return -1;
}
