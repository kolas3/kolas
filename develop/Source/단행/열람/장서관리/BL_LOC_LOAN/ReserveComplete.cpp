// ReserveComplete.cpp: implementation of the CReserveComplete class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReserveComplete.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReserveComplete::CReserveComplete()
{

}

CReserveComplete::~CReserveComplete()
{

}

INT CReserveComplete::DBReserveComplete(CReserveInfo& RI,CESL_Mgr* pEslMgr,CString sLoanKey,CESL_DataMgr* pUpdateDM)
{
EFS_BEGIN
	pUpdateDM->StartFrame();

	pUpdateDM->InitDBFieldData();

	if ( !RI.sBookKey.IsEmpty( ) )
		pUpdateDM->AddDBFieldData( _T("BOOK_KEY") , _T("NUMERIC") , RI.sBookKey );

	if ( !RI.sSpeciesKey.IsEmpty( ) )
		pUpdateDM->AddDBFieldData( _T("SPECIES_KEY") , _T("NUMERIC") , RI.sSpeciesKey );

	if ( !RI.sVolInfo.IsEmpty( ) )
		pUpdateDM->AddDBFieldData( _T("VOL_INFO") , _T("NUMERIC") , RI.sVolInfo );

	pUpdateDM->AddDBFieldData( _T("SHELF_LOC_CODE") , _T("STRING") , RI.sShelfLocCode );
	pUpdateDM->AddDBFieldData( _T("CLASS_NO_TYPE") , _T("STRING") , RI.sClassNoType );
	pUpdateDM->AddDBFieldData( _T("CLASS_NO") , _T("STRING") , RI.sClassNo );
	pUpdateDM->AddDBFieldData( _T("MANAGE_CODE") , _T("STRING") , RI.sManageCode );
	pUpdateDM->AddDBFieldData( _T("REG_CODE") , _T("STRING") , RI.sRegCode );
	pUpdateDM->AddDBFieldData( _T("SEPARATE_SHELF_CODE") , _T("STRING") , RI.sSeparateShelfCode );
	pUpdateDM->AddDBFieldData( _T("CALL_NO") , _T("STRING") , RI.sCallNo );
	pUpdateDM->AddDBFieldData( _T("REG_NO") , _T("STRING") , RI.sRegNo );
	pUpdateDM->AddDBFieldData( _T("LAST_WORK") , _T("STRING") , pEslMgr->GetWorkLogMsg( _T("예약지정완료") , __WFILE__ , __LINE__ ));

	CString sQuery;
	INT ids = pUpdateDM->MakeUpdateFrameQuery( _T("LS_WORK_T01") , _T("REC_KEY") , _T("NUMERIC") , sLoanKey , sQuery );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -1 , _T("DBReserveComplete") );

	ids = pUpdateDM->AddFrame( sQuery );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -2 , _T("DBReserveComplete") );

	ids = pUpdateDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -3 , _T("DBReserveComplete") );

	pUpdateDM->EndFrame();

	return 0;

EFS_END
return -1;
}
