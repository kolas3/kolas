// SE_CENTERUPLOAD_API.cpp: implementation of the CSE_CENTERUPLOAD_API class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SE_CENTERUPLOAD_API.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSE_CENTERUPLOAD_API::CSE_CENTERUPLOAD_API(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(IDD, pParent)
{


}

CSE_CENTERUPLOAD_API::~CSE_CENTERUPLOAD_API()
{

}


INT CSE_CENTERUPLOAD_API::SE_UpdateUploadStatus( CESL_DataMgr *pDataMgr , INT iRowIndex, CString sUploadStatus )
{

 
	CString species_key;
    
    pDataMgr->SetCellData (_T("UPLOAD»óÅÂ"),sUploadStatus,iRowIndex) ;
    

	pDataMgr->GetCellData (_T("REC_KEY"),iRowIndex,species_key) ;
     
	pDataMgr->StartFrame () ;
	pDataMgr->InitDBFieldData();
	pDataMgr->AddDBFieldData( _T("CENTER_UPLOAD_STATUS"), _T("STRING"), sUploadStatus );
	
	pDataMgr->MakeUpdateFrame (_T("SE_SPECIES_TBL"), _T("REC_KEY"),_T("NUMERIC"), species_key);

    pDataMgr->SendFrame ();
	pDataMgr->EndFrame () ;

    return 0;

}
