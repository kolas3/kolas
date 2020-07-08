// MakeRFIDInfoFromDB.cpp : implementation file
//


#include "stdafx.h"
#include "MakeRFIDInfoFromDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMakeRFIDInfoFromDB dialog


CMakeRFIDInfoFromDB::CMakeRFIDInfoFromDB(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMakeRFIDInfoFromDB)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pInfo = pParent->m_pInfo;
	InitESL_Mgr( _T("SM_RFID입력항목") );
}

CMakeRFIDInfoFromDB::~CMakeRFIDInfoFromDB()
{

}

VOID CMakeRFIDInfoFromDB::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMakeRFIDInfoFromDB)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMakeRFIDInfoFromDB, CDialog)
	//{{AFX_MSG_MAP(CMakeRFIDInfoFromDB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakeRFIDInfoFromDB message handlers
INT CMakeRFIDInfoFromDB::MakeRFIDInfoFromDB(CString strSerial, TAGDATASTRU* pTagStru)
{
EFS_BEGIN

	INT				ids;
	CRFID_Interface RFID( this );
	CString strFuncInfo;

	ids = RFID.GetSerialCountFromBookTable( strSerial );
	if( 0 > ids ) return 0;
	if ( ids == 0 ) return 0;	
	if( 0 < ids )
	{
		ids = RFID.GetTagDataFromBookTable( strSerial , pTagStru );
		if( 0 > ids ) {
			strFuncInfo.Format( _T("MakeRFIDInfoFromDB()-RFID.GetTagDataFromBookTable Return:%d "),  ids);
			CLocCommonAPI::MakeRFIDLog( strFuncInfo, _T("RFID DB 동기화"), __WFILE__, __LINE__, ids );			
			return 0;
		}
		return 1;
	}	

EFS_END
	return -1;
}
