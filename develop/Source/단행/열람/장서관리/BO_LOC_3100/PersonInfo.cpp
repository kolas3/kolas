// PersonInfo.cpp: implementation of the CPersonInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PersonInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPersonInfo::CPersonInfo()
{

}

CPersonInfo::~CPersonInfo()
{

}

void CPersonInfo::SetPrintLog(CESL_Mgr* pMgr, CESL_DataMgr *pDM, int nType, CESL_Information * pInfo, CString strContent, int nDataCount, std::vector<CString>* pList )
{
	EFS_BEGIN

	const int nCnt = 11;

	CString field[nCnt] = {
		_T("REC_KEY"),
		_T("TYPE"),
		_T("WORKER_IP"),
		_T("WORKER_ID"),
		_T("WORKER_NAME"),
		_T("USER_KEY"), 
		_T("WORK_DATE"),
		_T("FIRST_WORK"),
		_T("WORKER_SYS"),
		_T("WORKER_DO"),
		_T("PRINT_COUNT") };	

	CString type[nCnt] = {
		_T("NUMERIC"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),
		_T("NUMERIC"),
		_T("NUMERIC"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),
		_T("NUMERIC") };

	CString data[nCnt];
	if( pDM->m_dbkind == 1 )
		data[0] = _T("ESL_SEQ.NextVal");
	else
		pDM->MakeRecKey( data[0] );

	if(nType == 0)
	{
		data[1].Format(_T("E"));		
	}
	else if(nType == 1)
	{
		data[1].Format(_T("P"));
	}

	data[2] = pInfo->LOCAL_IP;
	data[3] = pInfo->USER_ID;
	data[4] = pInfo->USER_NAME;
	data[5].Format(_T(""));
	data[6].Format(_T("SYSDATE"));
	data[7] = pMgr->GetWorkLogMsg( strContent, __WFILE__, __LINE__ );
	data[8] = _T("KOLASIII");
	data[9].Format(_T("%s"),strContent);
	data[10].Format(_T("%d"),nDataCount);

	pDM->StartFrame();
	
	pDM->InitDBFieldData();	

	for(int j= 0; j < pList->size(); j++)
	{	
		for(int i = 0; i< nCnt; i++)
		{
			if(i==5)
			{
				pDM->AddDBFieldData( field[i], type[i], (*pList)[j] );
			}
			else if( data[i].GetLength() )
			{
				pDM->AddDBFieldData( field[i], type[i], data[i] );
			}
		}

		int nRtn = pDM->MakeInsertFrame( _T("MN_USER_PRINT_LOG_TBL") );
	}	

	pDM->SendFrame();
	pDM->EndFrame();	

	EFS_END
}

void CPersonInfo::SetPrintLog(CESL_Mgr* pMgr, CESL_DataMgr *pDM, int nType, CESL_Information * pInfo, CString strContent, int nDataCount)
{	
	EFS_BEGIN

	const int nCnt = 11;

	CString field[nCnt] = {
		_T("REC_KEY"),
		_T("TYPE"),
		_T("WORKER_IP"),
		_T("WORKER_ID"),
		_T("WORKER_NAME"),
		_T("USER_KEY"), 
		_T("WORK_DATE"),
		_T("FIRST_WORK"),
		_T("WORKER_SYS"),
		_T("WORKER_DO"),
		_T("PRINT_COUNT") };	

	CString type[nCnt] = {
		_T("NUMERIC"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),
		_T("NUMERIC"),
		_T("NUMERIC"),
		_T("STRING"),
		_T("STRING"),
		_T("STRING"),
		_T("NUMERIC") };

	CString data[nCnt];
	if( pDM->m_dbkind == 1 )
		data[0] = _T("ESL_SEQ.NextVal");
	else
		pDM->MakeRecKey( data[0] );

	if(nType == 0)
	{
		data[1].Format(_T("E"));		
	}
	else if(nType == 1)
	{
		data[1].Format(_T("P"));
	}

	data[2] = pInfo->LOCAL_IP;
	data[3] = pInfo->USER_ID;
	data[4] = pInfo->USER_NAME;
	data[5].Format(_T(""));
	data[6].Format(_T("SYSDATE"));
	data[7] = pMgr->GetWorkLogMsg( strContent, __WFILE__, __LINE__ );
	data[8] = _T("KOLASIII");
	data[9].Format(_T("%s"),strContent);
	data[10].Format(_T("%d"),nDataCount);

	pDM->StartFrame();
	
	pDM->InitDBFieldData();	
	for(int i = 0; i< nCnt; i++)
	{
		if( data[i].GetLength() )
			pDM->AddDBFieldData( field[i], type[i], data[i] );
	}

	int nRtn = pDM->MakeInsertFrame( _T("MN_USER_PRINT_LOG_TBL") );

	pDM->SendFrame();
	pDM->EndFrame();	

	EFS_END

}