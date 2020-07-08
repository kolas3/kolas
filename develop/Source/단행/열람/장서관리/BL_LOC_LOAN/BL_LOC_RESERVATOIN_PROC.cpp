#include "stdafx.h"
#include "BL_LOC_RESERVATOIN_PROC.h"
#include "InputReservatoinExpireDay.h"
#include "..\CLocMailMgr\LocMailMgrAPI.h"
#include "SMSPreviewDlg.h"
#include "BO_LOC_3000Api.h"


CBL_LOC_RESERVATOIN_PROC::CBL_LOC_RESERVATOIN_PROC(CESL_Mgr *pWnd,CLoanShareManager* pLoanShareManager , CLonaManageValue *pLona)
	: CSPF_Work(pWnd)
{
	m_pLona = pLona;

	m_pLoanShareManager = pLoanShareManager;

	m_pLoanProc = NULL;
	m_pLoanProc = new CBL_LOC_LOAN_PROC(m_pParentMgr , m_pLoanShareManager  , m_pLona);

	m_nMailClass = 1;

	m_strUserID = pWnd->GetUserID();
	m_strLibCode=_T("");
}

CBL_LOC_RESERVATOIN_PROC::~CBL_LOC_RESERVATOIN_PROC()
{
	if( m_pLoanProc != NULL ) delete m_pLoanProc;
}

INT CBL_LOC_RESERVATOIN_PROC::SPFExcutePath( CString sPathAlias )
{
	EFS_BEGIN

	INT ids;

	if( sPathAlias.Compare( _T("예약정보검색") ) == 0 )
	{
		ids = SearchReservationInfo();
	}
	else if( sPathAlias.Compare( _T("예약취소") ) == 0 )
	{
		ids = CancelReservation();
	}
	else if( sPathAlias.Compare( _T("예약만기일부여") ) == 0 )
	{
		ids = RecordReservationExpireDate();
	}
	else if( sPathAlias.Compare( _T("EMAIL보내기") ) == 0 )
	{
		ids = SendExpireEMAIL();
	}
	else if( sPathAlias.Compare( _T("자관자료체크") ) == 0 )
	{
		ids	= CheckManageCode();
	}
	else
	{
		ids = 0;
	}

	if( ids < 0 ) 
	{
		m_pLoanProc->ViewMessage( _T("기능을 정상적으로 수행하지 못하였습니다.") );
		return -1;
	}
	if( ids > 0 ) 
		return ids;

	return 0;

	EFS_END
	return -1;
}

INT CBL_LOC_RESERVATOIN_PROC::SearchReservationInfo()
{
	EFS_BEGIN

	INT ids;
	
	ids = CelarDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchReservatoinInfo") ); 
	
	if( m_nMenuFlag == 0 )
	{
		if ( !m_pLona->m_bVolReserve )
		{
			CESL_Mgr* pMgr = m_pParentMgr->FindSM(_T("BO_LOC_3100"));
			if(NULL != pMgr)
			{
				
				ids = m_pLoanProc->IsSearchedUser();
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchReservatoinInfo") );
				if( ids > 0 ) return 1;

				
				ids = m_pLoanProc->IsSearchedLoanInfo();
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchReservatoinInfo") ); 
				if( ids > 0 )
				{
					CString sMsg;
					sMsg.Format( _T("예약정보를 검색할 자료가 존재하지 않습니다.") );
					m_pLoanProc->ViewMessage( sMsg );
					return 1;
				}
			}
		}
		else 
		{
			if( m_sSpeciesKey.IsEmpty() )
			{
				CString sMsg;
				sMsg.Format( _T("예약정보를 검색할 자료가 존재하지 않습니다.") );
				m_pLoanProc->ViewMessage( sMsg );
				return 1;
			}
		}
	}

	
	ids = SearchDB();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("SearchReservatoinInfo") ); 
	if( ids > 0 ) return ids;

	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_RESERVATOIN_PROC::CelarDM()
{
	EFS_BEGIN

	INT ids = 0;

	const INT nDMCnt = 3;

	CString sDMAlias[nDMCnt];
	CESL_DataMgr *pDM;

	if ( !m_pLona->m_bVolReserve )
	{
		sDMAlias[0] = _T("DM_BO_LOC_3170_BOOK_INFO");
		sDMAlias[1] = _T("DM_BO_LOC_3170_LOAN_USER_INFO");
		sDMAlias[2] = _T("DM_BO_LOC_3170_RESERVE_INFO");
	}
	else
	{
		sDMAlias[0] = _T("DM_BO_LOC_3170_VR_BOOK_INFO");
		sDMAlias[1] = _T("DM_BO_LOC_3170_VR_LOAN_USER_INFO");
		sDMAlias[2] = _T("DM_BO_LOC_3170_VR_RESERVE_INFO");
	}

	for( INT i = 0 ; i < nDMCnt ; i++ )
	{
		pDM = m_pParentMgr->FindDM( sDMAlias[i] );
		ids = pDM->FreeData();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CelarDM") );
	}

	return 0;

	EFS_END
	return -1;
}


INT CBL_LOC_RESERVATOIN_PROC::SearchDB()
{
	EFS_BEGIN

	INT ids = 0;
	INT nIndex = 0;

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	if( m_nMenuFlag == 0 )
	{
 		CESL_Mgr* pMgr = m_pParentMgr->FindSM(_T("BO_LOC_3100"));

		
		if(NULL == pMgr)
 		{
			pMgr = m_pParentMgr->FindSM(_T("SM_BO_LOC_RESERVE"));
			if(NULL == pMgr)
 			{
 				sCMAlias = _T("CM_BOOK_COOPERATIVE_GROUP_OFFER");
 				sGridAlias = _T("그리드_제공자료");
			}
			else
			{
				sCMAlias = _T("CM_BO_LOC_RESERVE");
 				sGridAlias = _T("MainGrid");
			}
 		}

		CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
		if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("SearchDB") ); 
		CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
		if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("SearchDB") ); 


		nIndex = pGrid->SelectGetIdx();
		if( nIndex < 0 )
		{
			if(NULL != pMgr)
			{
				m_pLoanProc->ViewMessage( _T("자료를 먼저 선택해주십시오") );
				return 1;
			}
		}
	}
	
	ids = SearchBookInfoDB( nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SearchDB") );
	if( ids == 1 ) return 1;

	ids = SearchLoanInfoDB( nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchDB") );
	
	
	ids = SearchReserveInfoDB( nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchDB") );

	return 0;

	EFS_END
	return -1;

}


INT CBL_LOC_RESERVATOIN_PROC::SearchBookInfoDB( INT nIndex )
{
	EFS_BEGIN

	INT ids = 0;
	
	CString sDMAlias = _T("DM_BO_LOC_3170");
	CString sDstDMAlias = _T("DM_BO_LOC_3170");
	CString sWhere = _T("");

	if ( !m_pLona->m_bVolReserve )
	{
		sDMAlias += _T("_BOOK_INFO");
		sDstDMAlias += _T("_BOOK_INFO");
		sWhere.Format( _T(" BB.REC_KEY = %s "), m_sBookKey );
	}
	else
	{
		sDMAlias += _T("_VR_BOOK_INFO");
		sDstDMAlias += _T("_VR_BOOK_INFO");
		sWhere.Format( _T(" BB.SPECIES_KEY = %s"), m_sSpeciesKey );
	}

	if( 0==m_sPublishFormCode.Compare(_T("MO")) )
	{
		sDMAlias += _T("_BO");
		if ( m_pLona->m_bVolReserve )
		{
			
			if( m_sVolInfoKey.IsEmpty() )	
			{
				sWhere += _T(" AND BB.VOL_SORT_NO IS NULL");
			}
			else							
			{
				sWhere += _T(" AND BB.VOL_SORT_NO = ") + m_sVolInfoKey;
			}
		}
	}
	else if( 0==m_sPublishFormCode.Compare(_T("SE")) )
	{
		sDMAlias += _T("_SE");
		if ( m_pLona->m_bVolReserve )
		{
			sWhere += _T(" AND BB.VOL_KEY");
			if( m_sVolInfoKey.IsEmpty() )	sWhere += _T(" IS NULL");
			else							sWhere += _T(" = ") + m_sVolInfoKey;
		}
	}
	else	return 1;
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchBookInfoDB") );
	pDM->FreeData();


	ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pDM , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("SearchBookInfoDB") ); 

	ids = m_pLoanProc->CopyDMToDM( pDM , sDstDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("SearchBookInfoDB") ); 

	
	if ( m_pLona->m_bVolReserve )
	{
		CString sData, sDesc;
		CESL_DataMgr *pBookDM = m_pParentMgr->FindDM( sDstDMAlias );
		for(INT i = 0; i < pBookDM->GetRowCount(); i++)
		{
			ids = pBookDM->GetCellData( _T("관리구분"), i, sData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_LOC_LOAN_PROC::AddLoanInfoGrid()") );
			
			m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분"), sData, sDesc );

			ids = pBookDM->SetCellData( _T("관리구분설명"), sDesc, i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CBL_LOC_LOAN_PROC::AddLoanInfoGrid()") );
		}
	}
	
	return 0;

	EFS_END
	return -1;
}

INT CBL_LOC_RESERVATOIN_PROC::SearchLoanInfoDB( INT nIndex )
{	
	EFS_BEGIN

	INT ids = 0;

	CString sDMAlias = _T("");
	CString sWhere;

	
	if ( !m_pLona->m_bVolReserve )
		sDMAlias = _T("DM_BO_LOC_3170_LOAN_USER_INFO");
	else
		sDMAlias = _T("DM_BO_LOC_3170_VR_LOAN_USER_INFO");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchLoanInfoDB") );
	pDM->FreeData();

	
	
	if ( !m_pLona->m_bVolReserve )
	{	
		
		sWhere.Format( _T(" CL.BOOK_KEY=%s AND CL.STATUS IN ('0','2','5','L','D')"), m_sBookKey );
	}
	else
	{
		
		sWhere.Format( _T(" CL.SPECIES_KEY=%s AND CL.STATUS IN ('0','2','5','L','D')"), m_sSpeciesKey );

		CString sDM = _T("");
		CESL_Mgr* pMgr = m_pParentMgr;
		do
		{
			if(NULL == pMgr) break;
			if(_T("BO_LOC_3100") == pMgr->SM_ALIAS)
			{
				sDM = _T("DM_BO_LOC_3100_LOAN_INFO");
				break;
			}
			else if(_T("BO_LOC_3200") == pMgr->SM_ALIAS)
			{
				sDM = _T("DM_BO_LOC_3200");
				break;
			}
			
			else if(_T("SM_BO_LOC_RESERVE") == pMgr->SM_ALIAS)
			{
				sDM = _T("DM_BO_LOC_RESERVE");
				break;
			}
			if(NULL == pMgr->pParentMgr)
			{
				pMgr = NULL;
				break;
			}
			pMgr = pMgr->pParentMgr;
		}while(TRUE);

		CString sPUBLISH_FORM_CODE;
		
		if(NULL == pMgr)
		{

			sPUBLISH_FORM_CODE = m_sPublishFormCode;
		}
		else
		{
			if( sDM == _T("") ) sDM = _T("DM_BO_LOC_3100_LOAN_INFO");

			ids = m_pParentMgr->GetDataMgrData( sDM, _T("단행/연속구분"), sPUBLISH_FORM_CODE, nIndex );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("SearchLoanInfoDB") );
		}

		if(sPUBLISH_FORM_CODE==_T("MO"))
		{
			pDM->TBL_NAME.Replace(_T("%책테이블%"), _T("BO_BOOK_TBL BB"));
			if( m_sVolInfoKey.IsEmpty() )
				sWhere += _T(" AND BB.VOL_SORT_NO IS NULL");
			else
				sWhere += _T(" AND BB.VOL_SORT_NO = ") + m_sVolInfoKey;
		}
		else if(sPUBLISH_FORM_CODE==_T("SE"))
		{
			pDM->TBL_NAME.Replace(_T("%책테이블%"), _T("SE_BOOK_TBL BB"));
			if( m_sVolInfoKey.IsEmpty() )
				sWhere += _T(" AND BB.VOL_KEY IS NULL");
			else
				sWhere += _T(" AND BB.VOL_KEY = ") + m_sVolInfoKey;
		}
		else if(sPUBLISH_FORM_CODE==_T("NB"))
		{
			pDM->TBL_NAME.Replace(_T("%책테이블%"), _T("CO_NON_DB_BOOK_TBL BB"));
			
			sWhere.Format( _T(" CL.BOOK_KEY=%s AND CL.STATUS IN ('0','2','5','L','D')"), m_sBookKey );
		}
		else
		{
			pDM->TBL_NAME.Replace(_T("%책테이블%"), _T("BO_BOOK_TBL BB"));
			
			sWhere.Format( _T(" CL.BOOK_KEY=%s AND CL.STATUS IN ('0','2','5','L','D')"), m_sBookKey );
		}		
	}


	ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pDM , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("SearchLoanInfoDB") ); 

	
	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
	{
		if(TRUE == m_pLona->m_bVolReserve)
		{
			CString strEnc;
			CStringArray asDes1, asDes2;
			for(INT i = 0; i < pDM->GetRowCount(); i++)
			{
				pDM->GetCellData(_T("휴대폰번호"), i, strEnc);
				asDes1.Add(strEnc);
				pDM->GetCellData(_T("집전화번호"), i, strEnc);
				asDes2.Add(strEnc);
			}
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
			if(NULL != pApi)
			{
				if(TRUE == pApi->CheckConnection())
				{
					if(TRUE == pApi->ConnectSocket())
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes1);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket())
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes2);
						pApi->CloseSocket();
					}
					for(i = 0; i < pDM->GetRowCount(); i++)
					{
						pDM->SetCellData(_T("휴대폰번호"), asDes1.GetAt(i), i);
						pDM->SetCellData(_T("집전화번호"), asDes2.GetAt(i), i);
					}
				}
				delete pApi;
				pApi = NULL;
			}
			asDes1.RemoveAll(); asDes1.FreeExtra();
			asDes2.RemoveAll(); asDes2.FreeExtra();
		}
	}

	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_RESERVATOIN_PROC::SearchReserveInfoDB( INT nIndex )
{
	EFS_BEGIN

	INT ids = 0;

	CString sDMAlias = _T("");
	CString sWhere = _T("");

	if ( !m_pLona->m_bVolReserve )
	{
		sDMAlias = _T("DM_BO_LOC_3170_RESERVE_INFO");
		sWhere.Format( _T(" CL.BOOK_KEY = %s AND CL.STATUS IN ('3')"), m_sBookKey );
	}
	else
	{
		sDMAlias = _T("DM_BO_LOC_3170_VR_RESERVE_INFO");
		
		sWhere.Format( _T(" CL.SPECIES_KEY = %s AND CL.STATUS IN ('3') AND CL.VOL_INFO"), m_sSpeciesKey );
		if( m_sVolInfoKey.IsEmpty( ) )
			sWhere += _T(" IS NULL");
		else
			sWhere += _T(" = ") + m_sVolInfoKey;
	}

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("SearchLoanInfoDB") );
	pDM->FreeData();


	ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pDM , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("SearchLoanInfoDB") ); 

	
	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
	{
		CString strEnc;
		CStringArray asDes1, asDes2, asDes3, asDes4;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			if(TRUE == m_pLona->m_bVolReserve)
			{ 
				pDM->GetCellData(_T("EMAIL"), i, strEnc);
				asDes1.Add(strEnc);
				pDM->GetCellData(_T("전화번호"), i, strEnc);
				asDes2.Add(strEnc);
				pDM->GetCellData(_T("핸드폰"), i, strEnc);
				asDes3.Add(strEnc);
				pDM->GetCellData(_T("직장전화번호"), i, strEnc);
				asDes4.Add(strEnc);
			}
			else
			{ 
				pDM->GetCellData(_T("EMAIL"), i, strEnc);
				asDes1.Add(strEnc);
				pDM->GetCellData(_T("전화번호"), i, strEnc);
				asDes2.Add(strEnc);
				pDM->GetCellData(_T("핸드폰"), i, strEnc);
				asDes3.Add(strEnc);
			}	
		}
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes2);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes3);
					pApi->CloseSocket();	
				}
				if(TRUE == m_pLona->m_bVolReserve)
				{
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes4);
						pApi->CloseSocket();	
					}
				}
			}
			for(i = 0; i < pDM->GetRowCount(); i++)
			{
				pDM->SetCellData(_T("EMAIL"), asDes1.GetAt(i), i);
				pDM->SetCellData(_T("전화번호"), asDes2.GetAt(i), i);
				pDM->SetCellData(_T("핸드폰"), asDes3.GetAt(i), i);
				if(TRUE == m_pLona->m_bVolReserve)
				{
					pDM->SetCellData(_T("직장전화번호"), asDes4.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
	}

	return 0;

	EFS_END
	return -1;

}


INT CBL_LOC_RESERVATOIN_PROC::CancelReservation()
{
	EFS_BEGIN

	INT ids;
	
	CString sDMAlias = _T("");
	CString sCMAlias = _T("");
	CString sGridAlias = _T("");
	CString sTableName = _T("LS_WORK_T01");

	if ( !m_pLona->m_bVolReserve )
	{
		sDMAlias = _T("DM_BO_LOC_3170_RESERVE_INFO");
		sCMAlias = _T("CM_BO_LOC_3170_RESERVE_INFO");
		sGridAlias = _T("ReserveGrid");
	}
	else
	{
		sDMAlias = _T("DM_BO_LOC_3170_VR_RESERVE_INFO");
		sCMAlias = _T("CM_BO_LOC_3170_VR_RESERVE_INFO");
		sGridAlias = _T("VRReserveGrid");
	}

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CancelReservation") );
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl( sGridAlias );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CancelReservation") );

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CancelReservation") );

	INT nIndex;
	CString sRecKey;
	CString sUserRecKey;
	CString sReservationBookCnt;

	INT nCount = 0;
	nIndex = pGrid->GetIdx();
	if( nIndex < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CancelReservation") );

	ids = pDM->GetCellData( _T("대출/반납정보KEY") , nIndex , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CancelReservation") );

	
	if(_T("Y") == m_pLona->m_sUnmannedReserveSys)
	{
		CString strQuery, strValue, strMsg;;
		strQuery.Format(_T("SELECT UNMANNED_RESERVATION_LOAN FROM LS_WORK_T01 WHERE REC_KEY=%s"), sRecKey);
		pDM->GetOneValueQuery(strQuery, strValue);
		if(_T("Y") == strValue)
		{ 
			strMsg.Format(_T("무인예약신청 자료입니다.\r\n예약취소 하시겠습니까?"));
			if(IDYES != MessageBox(m_pParentMgr->m_hWnd, strMsg, _T("경고"), MB_ICONWARNING | MB_YESNO))
			{
				return 100;
			}
		}
		else if(_T("O") == strValue)
		{ 
			strMsg = _T("무인예약대기 자료는 예약취소 할 수 없습니다.");
			MessageBox(m_pParentMgr->m_hWnd, strMsg, _T("경고"), MB_ICONWARNING);
			return 200;
		}
	}

	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , pDM );

	pDM->StartFrame();
	pDM->InitDBFieldData();
	ids = pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("4") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CancelReservation") );

	CString sReservationCancelDate=_T("SYSDATE");	
	
	ids = pDM->AddDBFieldData( _T("RESERVATION_CANCEL_TYPE") , _T("STRING") ,_T("1") , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CBL_LOC_LOAN_PROC::ReservationCancel()") );	

	ids = pDM->AddDBFieldData( _T("RESERVE_CANCEL_DATE") , _T("NUMERIC") ,sReservationCancelDate , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("CBL_LOC_LOAN_PROC::ReservationCancel()") );
	
	CString sLog;	
	
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	
	pDM->AddDBFieldData( _T("R_DEVICE"), _T("STRING"), _T("KOLASIII"), TRUE );
	
	ids = pMobileApi->MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -8 , _T("CancelReservation") );
		
	ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("총예약책수") , sReservationBookCnt , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -9 , _T("CancelReservation") );
	sReservationBookCnt.Format( _T("%d") , _ttoi(sReservationBookCnt) - 1 );

	ids = m_pParentMgr->GetDataMgrData( sDMAlias , _T("대출자KEY") , sUserRecKey , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("CancelReservation") );

	pDM->InitDBFieldData();
	ids = pDM->AddDBFieldData( _T("RESERVATION_COUNT") , _T("NUMERIC") , sReservationBookCnt , TRUE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -11 , _T("CancelReservation") );
	
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );

	ids = pMobileApi->MakeUpdateFrame( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sUserRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -12 , _T("CancelReservation") );

	ids = pMobileApi->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -13 , _T("CancelReservation") );
	pDM->EndFrame();

	ids = CLocCommonAPI::MobileApiEndLog( pMobileApi );
	
	if ( TRUE )
	{
		UPLOAD_SHARE_INFO UploadShareInfo;
		UploadShareInfo.nType = 0;
		
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
				
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("STATUS,RESERVATION_CANCEL_TYPE,RESERVE_CANCEL_DATE,R_DEVICE");
		UploadShareInfo.UploadShareLoanInfo.sLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
		UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
			
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sUserRecKey;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("RESERVATION_COUNT");			
		
		UploadShareInfo.UploadShareLoanInfo.sLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
		m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	}	
		
	if( (m_pParentMgr->m_pInfo->MODE==10000) || (m_pParentMgr->m_pInfo->MODE==30000) )
	{
	}
	else
	{
		
		//JOB.2019.0073 : 충돌 License
// 		if( m_pLoanProc->m_pCheckLoan->IsSendCancelMail() )
// 		{
// 			CLocMailMgrAPI MailMgr(m_pParentMgr);
// 			MailMgr.CreateMailInfo();
// 
// 			CString sMailSql;
// 			sMailSql.Format( _T(" CL.REC_KEY = %s ") , sRecKey );
// 			MailMgr.MakeRecord( 6 , sMailSql , FALSE );
// 		}
	}
	
	return 0;

	EFS_END
	return -1;

}



INT CBL_LOC_RESERVATOIN_PROC::RecordReservationExpireDate()
{
	EFS_BEGIN

	INT ids;
	BOOL bIsReturnBook = TRUE;
	
	ids = IsReturn();
	if( ids == 1 )
	{
		AfxMessageBox(_T("지정 완료가 되지 않아 예약만기일을 부여할 수 없습니다."));
		return 1;	
	}
	else if( ids == 2 )
	{
		bIsReturnBook = FALSE;
		ids = MessageBox( m_pParentMgr->m_hWnd , _T("아직 반납되지 않은 책입니다. \r\n그래도 예약만기일을 부여하시겠습니까? ") , _T("예약 만기일 부여") , MB_ICONQUESTION | MB_YESNO );
		if( ids == IDNO ) return 2;
	}
	else if( ids == 3 )
	{
		AfxMessageBox(_T("자관 자료일 경우에만 해당 기능을 수행 할 수 있습니다.") , MB_ICONINFORMATION );
		return 3;
	}
	else if( ids > 3 ) return ids;
	else if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("RecordReservationExpireDate") );

	
	
	CESL_DataMgr *pDM;
	if ( !m_pLona->m_bVolReserve )
	{
		pDM	=	m_pParentMgr->FindDM( _T("DM_BO_LOC_3170_RESERVE_INFO") );
	}
	else
	{
		pDM	=	m_pParentMgr->FindDM( _T("DM_BO_LOC_3170_VR_RESERVE_INFO") );
	}
	if( NULL	==	pDM ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RecordReservationExpireDate") );
	
	CString		strExpireDate;
	INT			nIndex = GetCurrentGridIndex();

	pDM->GetCellData( _T("예약만기일") , nIndex , strExpireDate );
	if( _T("")	!=	strExpireDate )
	{
		AfxMessageBox( _T("이미 예약만기일이 부여 된 자료입니다.") , MB_ICONINFORMATION);
		return 5;
	}
	
	INT nExpireDay = 0;
	ids = GetReservationExpireDay( nExpireDay , m_sPositionCode );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("RecordReservationExpireDate") );
	
	if( 0 > nExpireDay ) return 1;
		
	CString sExpireDate;
	ids = MakeReservationExpireDate( nExpireDay , sExpireDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("RecordReservationExpireDate") );

	ids = ExpireDBProc( sExpireDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("RecordReservationExpireDate") );

	m_sExpireDate = sExpireDate;
	
	CString sIsSMS;
	ids = m_pParentMgr->m_pInfo->GetSMSUseMode();
	if( 0 == ids )
	{
		sIsSMS = _T("N");
	}
	else if( 1 == ids )
	{
		sIsSMS = _T("Y");
	}
	else
	{
		sIsSMS = _T("N");
	}

	
	if(m_pLona->m_bVolReserve)
	{
		CString strUnmanned;
		ids = pDM->GetCellData(_T("무인예약상태"), nIndex, strUnmanned);
		if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, _T("RecordReservationExpireDate"));}
		if(_T("Y") == strUnmanned)
		{ 
			return 1050;
		}
	}

	CString		strSMSUseYN;
	ids = pDM->GetCellData(_T("SMS수신여부") , nIndex, strSMSUseYN );
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1000 , _T("RecordReservationExpireDate") );
	}

	if(		_T("Y") == m_pLona->m_sReserveExpireSMS 
		&&	_T("Y") == sIsSMS						
		&&	_T("Y") == strSMSUseYN					
		&&  10000	!= m_pParentMgr->m_pInfo->MODE	
		&&	30000	!= m_pParentMgr->m_pInfo->MODE	
		&&	TRUE	== bIsReturnBook				
		)				
	{
		CESL_DataMgr TmpDM;
 		TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
 		TmpDM.SetManageCode(m_pParentMgr->m_pInfo->MANAGE_CODE);

		CString sMsg, sUserName, sUserKey, sLoanKey, sTitle, sReceiveCallNo, sSMSType;
		sSMSType = _T("SMS03");
		pDM->GetCellData(_T("대출자"), nIndex, sUserName);			
		pDM->GetCellData(_T("대출자KEY"), nIndex, sUserKey);		
		pDM->GetCellData(_T("대출/반납정보KEY"), nIndex, sLoanKey);	
		pDM->GetCellData(_T("서명"), nIndex, sTitle);				
		pDM->GetCellData(_T("핸드폰"), nIndex, sReceiveCallNo);		
		
		CSMSPreviewDlg SMSDlg(m_pParentMgr);
		SMSDlg.SetInfo( sUserKey, sLoanKey, sUserName, sTitle, sExpireDate, sReceiveCallNo, sSMSType);
		ids = SMSDlg.MakeMsg();
		if( 1030 == ids )
		{			
			return 1030;
		}

		if( 0 > ids )
		{
			return 0;
		}

		ids = SMSDlg.SendMsg();
		if( 0 > ids ) 
		{			
			return 1040;
		}

		return 1010;
	}
	
	return 0;

	EFS_END
	return -1;

}


INT CBL_LOC_RESERVATOIN_PROC::GetReservationExpireDay( INT &nExpireDay , CString sPositionCode )
{
	EFS_BEGIN

	INT ids;
	
	ids = m_pLoanProc->m_pCheckLoan->GetReservationExpireDay( sPositionCode , nExpireDay );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetReservationExpireDay") );
	
	ids = m_pLoanProc->m_pCheckLoan->GetAutoSetReserveExpireDay();
	if( ids ==  0 ) return 0;
	
	CInputReservatoinExpireDay InputDlg(m_pParentMgr);
	InputDlg.SetDay( nExpireDay );
	InputDlg.DoModal();

	InputDlg.GetDay( nExpireDay );

	return 0;

	EFS_END
	return -1;

}


INT CBL_LOC_RESERVATOIN_PROC::MakeReservationExpireDate( INT nExpireDay , CString &sExpireDate )
{
	EFS_BEGIN


	CString strGetData;
	CTime tExpireDate = m_pLoanProc->m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strGetData);

	CTimeSpan tExpireDay( nExpireDay , 0 , 0 , 0 );
	
	tExpireDate += tExpireDay;

	sExpireDate = tExpireDate.Format( _T("%Y/%m/%d") );

	return 0;

	EFS_END
	return -1;

}


INT CBL_LOC_RESERVATOIN_PROC::ExpireDBProc( CString sExpireDate )
{
	EFS_BEGIN

	INT ids;
	INT nIndex = 0;
	CString sDMAlias = _T("");
	CString sRecKey = _T("");

	if ( !m_pLona->m_bVolReserve )
		sDMAlias = _T("DM_BO_LOC_3170_RESERVE_INFO");
	else
		sDMAlias = _T("DM_BO_LOC_3170_VR_RESERVE_INFO");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MakeReservationExpireDate") );

	nIndex = GetCurrentGridIndex();
	pDM->GetCellData( _T("대출/반납정보KEY") ,  nIndex  , sRecKey );

	ids = ExpireDBProc( pDM , sRecKey , sExpireDate );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("MakeReservationExpireDate") );


	return 0;


	EFS_END
	return -1;

}


INT CBL_LOC_RESERVATOIN_PROC::SendExpireEMAIL()
{
EFS_BEGIN
	
	INT ids;
	
	ids = OpenMailMgr();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("SendExpireEMAIL") );
	}
	
	return ids;

EFS_END
return -1;
}


INT CBL_LOC_RESERVATOIN_PROC::OpenMailMgr()
{
EFS_BEGIN
	//JOB.2019.0073 : 충돌 License
// 	if( (10000 == m_pParentMgr->m_pInfo->MODE) || (30000 == m_pParentMgr->m_pInfo->MODE) )
// 	{				
// 		return 1020;
// 	}
// 
// 	INT ids;
// 
// 	CString sReserveDMAlias = _T("");
// 	CString sBookDMAlias = _T("");
// 	CString sCMAlias = _T("");
// 	CString sGridAlias = _T("");
// 
// 	if ( FALSE == m_pLona->m_bVolReserve )
// 	{
// 		sReserveDMAlias = _T("DM_BO_LOC_3170_RESERVE_INFO");
// 		sBookDMAlias = _T("DM_BO_LOC_3170_BOOK_INFO");
// 		sCMAlias = _T("CM_BO_LOC_3170_RESERVE_INFO");
// 		sGridAlias = _T("ReserveGrid");
// 	}
// 	else
// 	{
// 		sReserveDMAlias = _T("DM_BO_LOC_3170_VR_RESERVE_INFO");
// 		sBookDMAlias = _T("DM_BO_LOC_3170_VR_BOOK_INFO");
// 		sCMAlias = _T("CM_BO_LOC_3170_VR_RESERVE_INFO");
// 		sGridAlias = _T("VRReserveGrid");
// 	}
// 
// 	
// 	CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
// 	if( NULL == pCM ) 
// 	{
// 		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1010 , _T("OpenMailMgr") );
// 	}
// 
// 	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl( sGridAlias );
// 	if( NULL == pGrid )
// 	{
// 		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1020 , _T("OpenMailMgr") );
// 	}
// 
// 	
// 	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sReserveDMAlias );
// 	if( NULL == pDM )
// 	{
// 		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1030, _T("OpenMailMgr") );
// 	}
// 
// 	CString sLoanKey;
// 	CString sWorkingStatus;
// 	CString	strSpeciesKey;
// 	CString	strRegNo;
// 
// 	INT nDenyReasonWorkingStatus = 0;
// 	INT nMailSendList = 0;
// 	
// 	CLocMailMgrAPI MailMgr(m_pParentMgr);
// 	
// 	INT nIndex = 0;
// 	nIndex = pGrid->GetRowSel()-1;  
// 	if( 0 > nIndex ) 
// 	{
// 		nIndex = 0;
// 	}
// 
// 	ids = pDM->GetCellData( _T("대출/반납정보KEY") , nIndex , sLoanKey );
// 	if( 0 > ids ) 
// 	{
// 		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1040 , _T("OpenMailMgr") );
// 	}
// 
// 	
// 	if ( FALSE == m_pLona->m_bVolReserve )
// 	{
// 		ids = m_pParentMgr->GetDataMgrData( sBookDMAlias, _T("배가상태") ,  sWorkingStatus , 0 );
// 	}
// 	else
// 	{
// 		ids = pDM->GetCellData( _T("종KEY") , nIndex , strSpeciesKey );
// 		if( 0 > ids )
// 		{
// 			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1050 , _T("OpenMailMgr") );
// 		}
// 
// 		ids = pDM->GetCellData( _T("등록번호") , nIndex , strRegNo );
// 		if( 0 > ids )
// 		{
// 			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1060 , _T("OpenMailMgr") );
// 		}
// 
// 		
// 		CString		strReserveMark;
// 		pDM->GetCellData( _T("지정여부") , nIndex , strReserveMark );
// 		if( _T("O") != strReserveMark )
// 		{
// 			
// 			return	1030;
// 		}
// 
// 		
// 		CString strUnmanned;
// 		ids = pDM->GetCellData(_T("무인예약상태"), nIndex, strUnmanned);
// 		if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1060, _T("OpenMailMgr"));}
// 		if(!strUnmanned.IsEmpty())
// 		{
// 			return 1070;
// 		}
// 
// 		CString strForm;
// 		CString strQuery;
// 		
// 		if( TRUE == sLoanKey.IsEmpty() ) 
// 		{
// 			return 0;
// 		}
// 
// 		ids = pDM->GetCellData( _T("단행/연속구분") , nIndex , strForm );
// 
// 		if(_T("MO") == strForm)
// 		{
// 			strQuery.Format(_T("SELECT WORKING_STATUS FROM BO_BOOK_TBL WHERE SPECIES_KEY='%s' AND REG_NO='%s'")
// 				,strSpeciesKey, strRegNo);  
// 
// 		}
// 		else if(_T("SE") == strForm)
// 		{
// 			strQuery.Format(_T("SELECT WORKING_STATUS FROM SE_BOOK_TBL WHERE SPECIES_KEY='%s' AND REG_NO='%s'")
// 				,strSpeciesKey, strRegNo);  
// 
// 		}
// 		
// 		pDM->GetOneValueQuery(strQuery,sWorkingStatus);
// 	}
// 
// 	if( 0 > ids ) 
// 	{
// 		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1070 , _T("OpenMailMgr") );
// 	}
// 
// 	sWorkingStatus.TrimLeft();		sWorkingStatus.TrimRight();
// 	
// 	if( 0 != sWorkingStatus.Compare(_T("비치자료")) &&
// 		0 != sWorkingStatus.Compare(_T("실시간배가자료")) &&
// 		0 != sWorkingStatus.Compare(_T("연속비치자료")) &&
// 		0 != sWorkingStatus.Compare(_T("BOL112N")) &&
// 		0 != sWorkingStatus.Compare(_T("SEL212N")) &&
// 		0 != sWorkingStatus.Compare(_T("SEL112N")) )
// 	{
// 		nDenyReasonWorkingStatus++;
// 
// 		return 1040;
// 	}
// 
// 	
// 	CString		strEmailUseYN;
// 	pDM->GetCellData( _T("메일수신여부") , nIndex , strEmailUseYN );
// 	if( _T("Y") != strEmailUseYN )
// 	{
// 		return	1050;
// 	}
// 
// 	CString		strEMail;
// 	pDM->GetCellData( _T("EMAIL") , nIndex , strEMail );
// 	if( _T("") == strEMail )
// 	{		
// 		return	1060;
// 	}
// 
// 	CString sWhere;
// 	sWhere.Format( _T(" CL.REC_KEY = %s ") , sLoanKey );
// 
// 	ids = MailMgr.MakeRecord( m_nMailClass , sWhere , TRUE );
// 	if( 0 > ids ) 
// 	{
// 		ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1080 , _T("OpenMailMgr") );
// 	}
// 	
// 	else if( 1010 == ids )
// 	{
// 		return 1010;
// 	}
// 
// 	nMailSendList++;
// 
// 	
// 	CString sMsg;
// 	CString sTmpMsg;
// 	BOOL bMessageView = TRUE;
// 
// 	if( nMailSendList < 1 )
// 	{
// 		sTmpMsg.Format( _T("메일을 보낼 자료가 존재하지 않습니다.\r\n") , nMailSendList );
// 		sMsg += sTmpMsg;
// 	}
// 	if( nDenyReasonWorkingStatus > 0 ) 
// 	{
// 		sTmpMsg.Format( _T("반납된 자료가 아니므로 메일을 보낼수 없습니다.\r\n") , nDenyReasonWorkingStatus );
// 		sMsg += sTmpMsg;
// 	}
// 
// 	if( (nDenyReasonWorkingStatus) < 1 )
// 	{
// 		bMessageView = FALSE;
// 	}
// 
// 	
// 	if( bMessageView )
// 	{
// 		AfxMessageBox(sMsg);
// 		if( 1 > nMailSendList ) 
// 		{
// 			return 0;
// 		}
// 	}

	return 0;

	EFS_END
	return -1;

}


INT CBL_LOC_RESERVATOIN_PROC::GetMailBody( CString &sBody )
{
	EFS_BEGIN

	sBody = _T("예약하신 책이 반납되었습니다. <br> 도서관에 오셔서 책을 대출하시기 바랍니다.");
	return 0;

	EFS_END
	return -1;

}


INT CBL_LOC_RESERVATOIN_PROC::GetMailBody( CESL_DataMgr *pDM , INT nIndex , CString &sBody )
{
	EFS_BEGIN

	INT ids;

	CString sBookName;
	ids = pDM->GetCellData( _T("MARC.본표제") , nIndex , sBookName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetMailBody") );
	
	sBody.Format(_T("예약하신 책('%s')이 반납되었습니다. <br> 도서관에 오셔서 책을 대출하시기 바랍니다."),sBookName);

	return 0;

	EFS_END
	return -1;

}


INT CBL_LOC_RESERVATOIN_PROC::GetEMail( CString &sEMail )
{
	EFS_BEGIN

	INT ids;

	INT nIndex;
	nIndex = GetCurrentGridIndex();

	ids = m_pParentMgr->GetDataMgrData( _T("DM_BO_LOC_3170_RESERVE_INFO") , _T("EMAIL") , sEMail , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetEMail") );
	
	if( sEMail.IsEmpty() )
	{
		m_pLoanProc->ViewMessage( _T("EMAIL 정보가  존재하지 않는 사용자입니다.") );
		return 1;
	}


	return 0;

	EFS_END
	return -1;

}
INT CBL_LOC_RESERVATOIN_PROC::GetEMail( CESL_DataMgr *pDM , INT nIndex , CString &sEMail )
{
	EFS_BEGIN

	INT ids;

	ids = pDM->GetCellData( _T("E_MAIL") , nIndex , sEMail );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetEMail") );
	
	if( sEMail.IsEmpty() )
	{
		return 1;
	}

	return 0;


	EFS_END
	return -1;

}



INT CBL_LOC_RESERVATOIN_PROC::GetCurrentGridIndex()
{
	EFS_BEGIN

	INT nRowIndex = 0;
	CString sCMAlias = _T("CM_BO_LOC_3170_VR_RESERVE_INFO");
	CString sGridAlias = _T("VRReserveGrid");

	if ( m_pLona->m_bVolReserve )
	{
		CESL_ControlMgr *pCM = m_pParentMgr->FindCM( sCMAlias );
		if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetCurrentGridIndex") );
		CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl( sGridAlias );
		if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GetCurrentGridIndex") );

		nRowIndex = pGrid->GetIdx( );
	}
	
	
	return nRowIndex;

	EFS_END
	return -1;

}



INT CBL_LOC_RESERVATOIN_PROC::GetLoanUserName( CString &sLoanUserName )
{
	EFS_BEGIN

	INT ids;

	INT nIndex;
	nIndex = GetCurrentGridIndex();

	ids = m_pParentMgr->GetDataMgrData( _T("DM_BO_LOC_3170_RESERVE_INFO") , _T("대출자") , sLoanUserName , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetLoanUserName") );
	
	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_RESERVATOIN_PROC::GetLoanUserName( CESL_DataMgr *pDM , INT nIndex , CString &sLoanUserName )
{
	EFS_BEGIN

	INT ids;

	ids = pDM->GetCellData( _T("대출자") , nIndex , sLoanUserName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GetLoanUserName") );
	
	return 0;

	EFS_END
	return -1;

}



INT CBL_LOC_RESERVATOIN_PROC::GetReservationAutoMode()
{
	EFS_BEGIN

	INT ids = m_pLoanProc->m_pCheckLoan->GetAutoSetReserveExpireDay();

	return ids;

	EFS_END
	return -1;

}


INT CBL_LOC_RESERVATOIN_PROC::IsReturn()
{
	EFS_BEGIN

	INT ids = 0;
	INT nSIndex = 0;
	INT nRowCount = 0;

	CString sDMAlias = _T("");
	CString sRegNo = _T("");
	CString sTmpRegNo = _T("");
	CString sComplete = _T("");
	CString sWorkingStatus = _T("");

	if ( !m_pLona->m_bVolReserve )
		sDMAlias = _T("DM_BO_LOC_3170_LOAN_USER_INFO");
		
	else
		sDMAlias = _T("DM_BO_LOC_3170_VR_RESERVE_INFO");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("IsReturn") );

	if ( !m_pLona->m_bVolReserve )
	{
		nRowCount = pDM->GetRowCount();
		if( nRowCount < 1 ) return 0;

		
		return 2;
	}

	CESL_Grid* pGrid = (CESL_Grid*)m_pParentMgr->FindControl( _T("CM_BO_LOC_3170_VR_RESERVE_INFO"), _T("VRReserveGrid") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("IsReturn") );

	nSIndex = pGrid->GetIdx();

	ids = pDM->GetCellData( _T("지정여부") , nSIndex , sComplete );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE(  m_pParentMgr->m_hWnd ,-1 , _T("IsReturn") );

	if( 0==sComplete.Compare(_T("X")) )	return 1;

	ids = pDM->GetCellData( _T("등록번호") , nSIndex , sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("IsReturn") );

	CESL_DataMgr* pBookInfoDM = m_pParentMgr->FindDM( _T("DM_BO_LOC_3170_VR_BOOK_INFO_BO") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("IsReturn") ); 

	for( INT i = 0 ; i < pBookInfoDM->GetRowCount() ; i++ )
	{
		ids = pBookInfoDM->GetCellData( _T("등록번호") , i , sTmpRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("IsReturn") );

		if( 0==sRegNo.Compare(sTmpRegNo) )
		{
			ids = pBookInfoDM->GetCellData( _T("배가상태") , i , sWorkingStatus );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("IsReturn") );

			if( sWorkingStatus.Compare( _T("BOL112N") ) == 0 || 
				sWorkingStatus.Compare( _T("SEL212N") ) == 0 ||
				sWorkingStatus.Compare( _T("SEL112N") ) == 0 )
				return 0;

			return 2;
		}
	}

	return 3;

	EFS_END
	return -1;
}


INT CBL_LOC_RESERVATOIN_PROC::ExpireDBProc( CESL_DataMgr *pDM , CString sLoanKey , CString sExpireDate )
{
	EFS_BEGIN

	INT ids;

	
	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( m_pParentMgr , pDM );

	pDM->StartFrame();
	pDM->InitDBFieldData();
	
	sExpireDate = _T("TO_DATE('")+sExpireDate+_T(" 23:59:59")+_T("', 'YYYY/MM/DD HH24:MI:SS')");
	pDM->AddDBFieldData( _T("RESERVATION_EXPIRE_DATE") , _T("NUMERIC") , sExpireDate , TRUE );

	
	CString sLog;
	sLog.Format( _T("[%s] ID : %s") , CTime::GetCurrentTime().Format(_T("%Y/%m/%d %H:%M")) , m_pParentMgr->GetUserID() );
	
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	
	pMobileApi->MakeUpdateFrame( _T("LS_WORK_T01") , _T("REC_KEY") , _T("NUMERIC") , sLoanKey );
	ids = pMobileApi->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ExpireDBProc") );

	pDM->EndFrame();
	
	
	ids = CLocCommonAPI::MobileApiEndLog( pMobileApi );

	
	
	
	
	
	
	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	
	
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sLoanKey;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("RESERVATION_EXPIRE_DATE");				

	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
	m_pLoanShareManager->SendLoanInfoUpload( UploadShareInfo );
	
	
	

	return 0;


	EFS_END
	return -1;

}


INT CBL_LOC_RESERVATOIN_PROC::IsReturn( CESL_DataMgr *pDM , CString sBookKey )
{
	EFS_BEGIN

	INT ids;

	CString sSQL;
	sSQL.Format(	_T(" SELECT WORKING_STATUS FROM BO_BOOK_TBL WHERE REC_KEY = %s")
					_T(" UNION")
					_T(" SELECT WORKING_STATUS FROM SE_BOOK_TBL WHERE REC_KEY = %s"), sBookKey, sBookKey );

	CString sResult;


	ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr , pDM , sSQL , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("IsReturn") );

	if( sResult.Compare( _T("BOL112N") ) == 0 || 
		sResult.Compare( _T("SEL112N") ) == 0 ||
		sResult.Compare( _T("SEL212N") ) == 0 
	  ) return 0;

	return 1;


	EFS_END
	return -1;

}


INT CBL_LOC_RESERVATOIN_PROC::IsFirstReservation( CESL_DataMgr *pDM , CString sBookKey , CString sLoanKey )
{
	EFS_BEGIN

	INT ids;

	CString sSQL;
	sSQL.Format( _T("SELECT REC_KEY FROM LS_WORK_T01 WHERE STATUS = '3' AND BOOK_KEY = %s AND ")
		_T("RESERVATION_DATE = ")
		_T("( SELECT MIN(RESERVATION_DATE) FROM LS_WORK_T01 WHERE STATUS = '3' AND BOOK_KEY = %s ) ") , sBookKey , sBookKey );

	CString sResult;


	ids = CLocCommonAPI::GetOneValueQuery( m_pParentMgr , pDM , sSQL , sResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("IsFirstReservation") );

	if( sResult.Compare( sLoanKey ) == 0 ) return 0;

	return 1;

	EFS_END
	return -1;

}



INT CBL_LOC_RESERVATOIN_PROC::OpenMailMgr( CESL_DataMgr *pDM , INT *nIndex , INT nCnt )
{
	EFS_BEGIN

	return 0;


	EFS_END
	return -1;

}


INT CBL_LOC_RESERVATOIN_PROC::SetPositionCode( CString sPositionCode )
{
	EFS_BEGIN

	m_sPositionCode = sPositionCode;

	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_RESERVATOIN_PROC::SetMailClass( INT nMailClass )
{
	EFS_BEGIN

	m_nMailClass = nMailClass;

	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_RESERVATOIN_PROC::CheckManageCode()
{
EFS_BEGIN

	CESL_DataMgr*	pDM		=	m_pParentMgr->FindDM( _T("DM_BO_LOC_3170_VR_BOOK_INFO") );
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CheckManageCode") );
	
	CESL_Grid*		pGrid	=	(CESL_Grid*)m_pParentMgr->FindControl( _T("CM_BO_LOC_3170_VR_RESERVE_INFO") , _T("VRReserveGrid") );
	if( NULL == pGrid ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("CheckManageCode") );

	INT		nSelectIndex	=	pGrid->GetIdx();	
	
	CString	strManageCode	=	_T("");
	
	INT		ids		=	m_pParentMgr->GetDataMgrData( _T("DM_BO_LOC_3170_VR_RESERVE_INFO") , _T("관리구분") , strManageCode , nSelectIndex );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CheckManageCode") );
		
	if( strManageCode	!=	m_pParentMgr->m_pInfo->MANAGE_CODE )
	{
		return 1;
	}
	
	return 0;

EFS_END
return -1;
}
