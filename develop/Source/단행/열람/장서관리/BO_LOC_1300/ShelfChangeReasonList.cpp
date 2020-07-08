// ShelfChangeReasonList.cpp: implementation of the CShelfChangeReasonList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShelfChangeReasonList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShelfChangeReasonList::CShelfChangeReasonList(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	INT ids;

	ids = InitESL_Mgr(_T("SHELF_CHANGE_REASON_LIST"));
	if( ids < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return ;
	}
	
	ids = MakeDM();
	if( ids < 0 ){
		AfxMessageBox(_T("MakeDM()"));
		return ;
	} 
	
}

CShelfChangeReasonList::~CShelfChangeReasonList()
{

}

INT CShelfChangeReasonList::GetList( CString sList[] , INT &nRowCnt )
{
	EFS_BEGIN
	
	INT ids;

	CString sDMAlias = _T("DM_SHELF_CHANGE_REASON_LIST");

	CESL_DataMgr *pDM = FindDM(sDMAlias);
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CShelfChangeReasonList::GetList( CString *sList , INT &nRowCnt )") );


	nRowCnt = pDM->GetRowCount();

	for( INT i = 0 ; i < nRowCnt ; i++ )
	{
		ids = pDM->GetCellData( _T("코드명") , i , sList[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CShelfChangeReasonList::GetList( CString *sList , INT &nRowCnt )") );
	}
	
	if( nRowCnt == 0 )
	{
		sList[0] = _T("적용안함");
		sList[1] = _T("ETC");
	}

	return 0;

	EFS_END
	return -1;

}

INT CShelfChangeReasonList::GetListCount()
{
	EFS_BEGIN

	CString sDMAlias = _T("DM_SHELF_CHANGE_REASON_LIST");

	CESL_DataMgr *pDM = FindDM(sDMAlias);
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CShelfChangeReasonList::GetList( CString *sList , INT &nRowCnt )") );

	INT nRowCnt = pDM->GetRowCount();

	if( nRowCnt < 1 ) return 2;

	return nRowCnt;

	EFS_END
	return -1;

}

INT CShelfChangeReasonList::MakeDM()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_SHELF_CHANGE_REASON_LIST");

	CESL_DataMgr *pDM = FindDM(sDMAlias);
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CShelfChangeReasonList::GetList( CString *sList , INT &nRowCnt )") );

	ids = CLocCommonAPI::RefreshDataManager( this , pDM , _T("") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CShelfChangeReasonList::GetList( CString *sList , INT &nRowCnt )") );

	return 0;

	EFS_END
	return -1;

}


INT CShelfChangeReasonList::GetDesc( CString sCode , CString &sReasonData )
{
	EFS_BEGIN
	
	INT ids;

	CString sDMAlias = _T("DM_SHELF_CHANGE_REASON_LIST");

	CESL_DataMgr *pDM = FindDM(sDMAlias);
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CShelfChangeReasonList::GetDesc( CString sCode , CString &sReasonData )") );

	CString sGetCode;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("코드명") , i , sGetCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CShelfChangeReasonList::GetDesc( CString sCode , CString &sReasonData )") );

		if( sCode.Compare( sGetCode ) == 0 ) 
		{
			ids = pDM->GetCellData( _T("설명") , i , sReasonData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CShelfChangeReasonList::GetDesc( CString sCode , CString &sReasonData )") );
			break;
		}
	}
	

	return 0;	

	EFS_END
	return -1;

}