// BudgetApi.cpp: implementation of the CBudgetApi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BudgetApi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBudgetApi::CBudgetApi(CESL_Mgr *pParentMgr)
{
	m_pMgr = pParentMgr;
}

CBudgetApi::~CBudgetApi()
{

}

INT CBudgetApi::InsertBudgetList(CESL_DataMgr* pDM,
								 CESL_DataMgr* pDM_MAT,
								 CESL_DataMgr* pDM_USAGE,
								 CESL_DataMgr* pDM_FORM,
								 CESL_DataMgr* pDM_MEDIA,
								 CESL_DataMgr* pDM_SUBREG,
								 CESL_DataMgr* pDM_USEOBJ,
								 CESL_DataMgr* pDM_MANAGE,
								 CESL_DataMgr* pDM_COURCE,
								 CESL_DataMgr* pDM_DEPT,
								 CESL_DataMgr* pDM_EXECUTE
								 )
								 
{
	//예산편성입력

	if(!pDM || !pDM_MAT || !pDM_USAGE || !pDM_EXECUTE)
	{
		AfxMessageBox(_T("예산편성입력 관련 정보를 찾을 수 없습니다!"));
		return -1;
	}



	CString strBUDGET_COMPILATION_KEY;
	CString strWorkLog;
	CString strREC_KEY;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_BUDGET);

	pDM->MakeRecKey(strBUDGET_COMPILATION_KEY);
	pDM->SetCellData(_T("REC_KEY"),strBUDGET_COMPILATION_KEY,0);
	pDM->SetCellData(_T("최초작업"),strWorkLog,0);		
	pDM->SetCellData(_T("마지막작업"),strWorkLog,0);	


	CArray<CString,CString> RemoveAliasList;
	

	pDM->StartFrame();

	RemoveAliasList.RemoveAll();
	pDM->InitDBFieldData();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, _T("CO_BUDGET_COMPILATION_TBL"), 0 , pDM);
	

	//자료별
	RemoveAliasList.Add(_T("UDF_자료구분"));
	
	for(INT i=0;i<pDM_MAT->GetRowCount();i++)
	{
		pDM->MakeRecKey(strREC_KEY);
		pDM_MAT->SetCellData(_T("REC_KEY"),strREC_KEY,i);
		pDM_MAT->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
		pDM_MAT->SetCellData(_T("최초작업"),strWorkLog,i);		
		pDM_MAT->SetCellData(_T("마지막작업"),strWorkLog,i);	

		pDM->InitDBFieldData();
		pDM->MakeQueryByDM(RemoveAliasList, pDM_MAT, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
	}


	//용도별
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("UDF_용도구분"));

	for(i=0;i<pDM_USAGE->GetRowCount();i++)
	{
		pDM->MakeRecKey(strREC_KEY);
		pDM_USAGE->SetCellData(_T("REC_KEY"),strREC_KEY,i);
		pDM_USAGE->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
		pDM_USAGE->SetCellData(_T("최초작업"),strWorkLog,i);		
		pDM_USAGE->SetCellData(_T("마지막작업"),strWorkLog,i);	

		pDM->InitDBFieldData();
		pDM->MakeQueryByDM(RemoveAliasList, pDM_USAGE, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
	}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//형식구분
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("UDF_형식구분"));

	for(i=0;i<pDM_FORM->GetRowCount();i++)
	{
		pDM->MakeRecKey(strREC_KEY);
		pDM_FORM->SetCellData(_T("REC_KEY"),strREC_KEY,i);
		pDM_FORM->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
		pDM_FORM->SetCellData(_T("최초작업"),strWorkLog,i);		
		pDM_FORM->SetCellData(_T("마지막작업"),strWorkLog,i);	

		pDM->InitDBFieldData();
		pDM->MakeQueryByDM(RemoveAliasList, pDM_FORM, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
	}

	//매체구분
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("UDF_매체구분"));

	for(i=0;i<pDM_MEDIA->GetRowCount();i++)
	{
		pDM->MakeRecKey(strREC_KEY);
		pDM_MEDIA->SetCellData(_T("REC_KEY"),strREC_KEY,i);
		pDM_MEDIA->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
		pDM_MEDIA->SetCellData(_T("최초작업"),strWorkLog,i);		
		pDM_MEDIA->SetCellData(_T("마지막작업"),strWorkLog,i);	

		pDM->InitDBFieldData();
		pDM->MakeQueryByDM(RemoveAliasList, pDM_MEDIA, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
	}
	//보조등록구분
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("UDF_보조등록구분"));

	for(i=0;i<pDM_SUBREG->GetRowCount();i++)
	{
		pDM->MakeRecKey(strREC_KEY);
		pDM_SUBREG->SetCellData(_T("REC_KEY"),strREC_KEY,i);
		pDM_SUBREG->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
		pDM_SUBREG->SetCellData(_T("최초작업"),strWorkLog,i);		
		pDM_SUBREG->SetCellData(_T("마지막작업"),strWorkLog,i);	

		pDM->InitDBFieldData();
		pDM->MakeQueryByDM(RemoveAliasList, pDM_SUBREG, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
	}
	//이용대상구분
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("UDF_이용대상구분"));

	for(i=0;i<pDM_USEOBJ->GetRowCount();i++)
	{
		pDM->MakeRecKey(strREC_KEY);
		pDM_USEOBJ->SetCellData(_T("REC_KEY"),strREC_KEY,i);
		pDM_USEOBJ->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
		pDM_USEOBJ->SetCellData(_T("최초작업"),strWorkLog,i);		
		pDM_USEOBJ->SetCellData(_T("마지막작업"),strWorkLog,i);	

		pDM->InitDBFieldData();
		pDM->MakeQueryByDM(RemoveAliasList, pDM_USEOBJ, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
	}
	//관리구분
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("UDF_관리구분"));

	for(i=0;i<pDM_MANAGE->GetRowCount();i++)
	{
		pDM->MakeRecKey(strREC_KEY);
		pDM_MANAGE->SetCellData(_T("REC_KEY"),strREC_KEY,i);
		pDM_MANAGE->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
		pDM_MANAGE->SetCellData(_T("최초작업"),strWorkLog,i);		
		pDM_MANAGE->SetCellData(_T("마지막작업"),strWorkLog,i);	

		pDM->InitDBFieldData();
		pDM->MakeQueryByDM(RemoveAliasList, pDM_MANAGE, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
	}
	//학과별
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("UDF_학과별"));

	for(i=0;i<pDM_COURCE->GetRowCount();i++)
	{
		pDM->MakeRecKey(strREC_KEY);
		pDM_COURCE->SetCellData(_T("REC_KEY"),strREC_KEY,i);
		pDM_COURCE->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
		pDM_COURCE->SetCellData(_T("최초작업"),strWorkLog,i);		
		pDM_COURCE->SetCellData(_T("마지막작업"),strWorkLog,i);	

		pDM->InitDBFieldData();
		pDM->MakeQueryByDM(RemoveAliasList, pDM_COURCE, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
	}
	//학부별
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("UDF_학부별"));

	for(i=0;i<pDM_DEPT->GetRowCount();i++)
	{
		pDM->MakeRecKey(strREC_KEY);
		pDM_DEPT->SetCellData(_T("REC_KEY"),strREC_KEY,i);
		pDM_DEPT->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
		pDM_DEPT->SetCellData(_T("최초작업"),strWorkLog,i);		
		pDM_DEPT->SetCellData(_T("마지막작업"),strWorkLog,i);	

		pDM->InitDBFieldData();
		pDM->MakeQueryByDM(RemoveAliasList, pDM_DEPT, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////

	//집행내역
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("UDF_집행타입"));
	RemoveAliasList.Add(_T("UDF_집행상태"));

	for(i=0;i<pDM_EXECUTE->GetRowCount();i++)
	{
		pDM->MakeRecKey(strREC_KEY);
		pDM_EXECUTE->SetCellData(_T("REC_KEY"),strREC_KEY,i);
		pDM_EXECUTE->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
		pDM_EXECUTE->SetCellData(_T("최초작업"),strWorkLog,i);		
		pDM_EXECUTE->SetCellData(_T("마지막작업"),strWorkLog,i);	

		pDM->InitDBFieldData();
		pDM->MakeQueryByDM(RemoveAliasList, pDM_EXECUTE, _T("CO_BUDGET_EXECUTION_TBL"), i , pDM);
	}



	INT ids = pDM->SendFrame();
	pDM->EndFrame();
	if (ids < 0) {
		return -3000;	
	}


	return 0;
}

INT CBudgetApi::UpdateBudgetList(CESL_DataMgr* pDM,
								 CESL_DataMgr* pDM_MAT,
								 CESL_DataMgr* pDM_USAGE,
								 CESL_DataMgr* pDM_FORM,
								 CESL_DataMgr* pDM_MEDIA,
								 CESL_DataMgr* pDM_SUBREG,
								 CESL_DataMgr* pDM_USEOBJ,
								 CESL_DataMgr* pDM_MANAGE,
								 CESL_DataMgr* pDM_COURCE,
								 CESL_DataMgr* pDM_DEPT,
								 CESL_DataMgr* pDM_EXECUTE,
								 CStringArray& arrayDeleteSubKey,
								 CStringArray& arrayDeleteExeKey
								 )
								 
{
	//예산편성수정

	if(!pDM || !pDM_MAT || !pDM_USAGE || !pDM_EXECUTE)
	{
		AfxMessageBox(_T("예산편성수정 관련 정보를 찾을 수 없습니다!"));
		return -1;
	}

	CString strBUDGET_COMPILATION_KEY;
	CString strWorkLog;
	CString strREC_KEY;
	strWorkLog = m_pMgr->GetWorkLogMsg(WORK_LOG_BUDGET);

	strBUDGET_COMPILATION_KEY = pDM->GetCellData(_T("REC_KEY"),0);
	pDM->SetCellData(_T("마지막작업"),strWorkLog,0);	


	CArray<CString,CString> RemoveAliasList;
	
	pDM->StartFrame();

	RemoveAliasList.RemoveAll();
	pDM->InitDBFieldData();
	pDM->MakeQueryByDM(RemoveAliasList, pDM, _T("CO_BUDGET_COMPILATION_TBL"), 0 , pDM, _T("REC_KEY"), _T("NUMERIC"), strBUDGET_COMPILATION_KEY);


	//자료별
	CString strUDF;
	RemoveAliasList.Add(_T("UDF_자료구분"));
	
	for(INT i=0;i<pDM_MAT->GetRowCount();i++)
	{
		
		strUDF = pDM_MAT->GetCellData(_T("UDF_상태"),i);
		if(_T("I") == strUDF)
		{
			pDM->MakeRecKey(strREC_KEY);
			pDM_MAT->SetCellData(_T("REC_KEY"),strREC_KEY,i);
			pDM_MAT->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
			pDM_MAT->SetCellData(_T("최초작업"),strWorkLog,i);
			pDM_MAT->SetCellData(_T("마지막작업"),strWorkLog,i);	

			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_MAT, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
		}
		else if(_T("U") == strUDF)
		{

			strREC_KEY = pDM_MAT->GetCellData(_T("REC_KEY"),i);
			pDM_MAT->SetCellData(_T("마지막작업"),strWorkLog,i);	
			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_MAT, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM, _T("REC_KEY"), _T("NUMERIC"), strREC_KEY);

		}

	}


	//용도별
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("UDF_용도구분"));

	for(i=0;i<pDM_USAGE->GetRowCount();i++)
	{

		strUDF = pDM_USAGE->GetCellData(_T("UDF_상태"),i);
		if(_T("I") == strUDF)
		{
			pDM->MakeRecKey(strREC_KEY);
			pDM_USAGE->SetCellData(_T("REC_KEY"),strREC_KEY,i);
			pDM_USAGE->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
			pDM_USAGE->SetCellData(_T("최초작업"),strWorkLog,i);
			pDM_USAGE->SetCellData(_T("마지막작업"),strWorkLog,i);	

			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_USAGE, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
		}
		else if(_T("U") == strUDF)
		{

			strREC_KEY = pDM_USAGE->GetCellData(_T("REC_KEY"),i);
			pDM_USAGE->SetCellData(_T("마지막작업"),strWorkLog,i);	
			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_USAGE, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM, _T("REC_KEY"), _T("NUMERIC"), strREC_KEY);

		}
	
	}

	//형식구분
	RemoveAliasList.Add(_T("UDF_형식구분"));
	
	for(i=0;i<pDM_FORM->GetRowCount();i++)
	{
		
		strUDF = pDM_FORM->GetCellData(_T("UDF_상태"),i);
		if(_T("I") == strUDF)
		{
			pDM->MakeRecKey(strREC_KEY);
			pDM_FORM->SetCellData(_T("REC_KEY"),strREC_KEY,i);
			pDM_FORM->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
			pDM_FORM->SetCellData(_T("최초작업"),strWorkLog,i);
			pDM_FORM->SetCellData(_T("마지막작업"),strWorkLog,i);	

			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_FORM, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
		}
		else if(_T("U") == strUDF)
		{

			strREC_KEY = pDM_FORM->GetCellData(_T("REC_KEY"),i);
			pDM_FORM->SetCellData(_T("마지막작업"),strWorkLog,i);	
			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_FORM, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM, _T("REC_KEY"), _T("NUMERIC"), strREC_KEY);

		}

	}

	//매체구분
	RemoveAliasList.Add(_T("UDF_매체구분"));
	
	for(i=0;i<pDM_MEDIA->GetRowCount();i++)
	{
		
		strUDF = pDM_MEDIA->GetCellData(_T("UDF_상태"),i);
		if(_T("I") == strUDF)
		{
			pDM->MakeRecKey(strREC_KEY);
			pDM_MEDIA->SetCellData(_T("REC_KEY"),strREC_KEY,i);
			pDM_MEDIA->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
			pDM_MEDIA->SetCellData(_T("최초작업"),strWorkLog,i);
			pDM_MEDIA->SetCellData(_T("마지막작업"),strWorkLog,i);	

			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_MEDIA, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
		}
		else if(_T("U") == strUDF)
		{

			strREC_KEY = pDM_MEDIA->GetCellData(_T("REC_KEY"),i);
			pDM_MEDIA->SetCellData(_T("마지막작업"),strWorkLog,i);	
			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_MEDIA, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM, _T("REC_KEY"), _T("NUMERIC"), strREC_KEY);

		}

	}
	
	//보조등록구분
	RemoveAliasList.Add(_T("UDF_보조등록구분"));
	
	for(i=0;i<pDM_SUBREG->GetRowCount();i++)
	{
		
		strUDF = pDM_SUBREG->GetCellData(_T("UDF_상태"),i);
		if(_T("I") == strUDF)
		{
			pDM->MakeRecKey(strREC_KEY);
			pDM_SUBREG->SetCellData(_T("REC_KEY"),strREC_KEY,i);
			pDM_SUBREG->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
			pDM_SUBREG->SetCellData(_T("최초작업"),strWorkLog,i);
			pDM_SUBREG->SetCellData(_T("마지막작업"),strWorkLog,i);	

			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_SUBREG, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
		}
		else if(_T("U") == strUDF)
		{

			strREC_KEY = pDM_SUBREG->GetCellData(_T("REC_KEY"),i);
			pDM_SUBREG->SetCellData(_T("마지막작업"),strWorkLog,i);	
			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_SUBREG, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM, _T("REC_KEY"), _T("NUMERIC"), strREC_KEY);

		}

	}	

	//이용대상구분
	RemoveAliasList.Add(_T("UDF_이용대상구분"));
	
	for(i=0;i<pDM_USEOBJ->GetRowCount();i++)
	{
		
		strUDF = pDM_USEOBJ->GetCellData(_T("UDF_상태"),i);
		if(_T("I") == strUDF)
		{
			pDM->MakeRecKey(strREC_KEY);
			pDM_USEOBJ->SetCellData(_T("REC_KEY"),strREC_KEY,i);
			pDM_USEOBJ->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
			pDM_USEOBJ->SetCellData(_T("최초작업"),strWorkLog,i);
			pDM_USEOBJ->SetCellData(_T("마지막작업"),strWorkLog,i);	

			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_USEOBJ, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
		}
		else if(_T("U") == strUDF)
		{

			strREC_KEY = pDM_USEOBJ->GetCellData(_T("REC_KEY"),i);
			pDM_USEOBJ->SetCellData(_T("마지막작업"),strWorkLog,i);	
			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_USEOBJ, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM, _T("REC_KEY"), _T("NUMERIC"), strREC_KEY);

		}

	}
	
	//관리구분
	RemoveAliasList.Add(_T("UDF_관리구분"));
	
	for(i=0;i<pDM_MANAGE->GetRowCount();i++)
	{
		
		strUDF = pDM_MANAGE->GetCellData(_T("UDF_상태"),i);
		if(_T("I") == strUDF)
		{
			pDM->MakeRecKey(strREC_KEY);
			pDM_MANAGE->SetCellData(_T("REC_KEY"),strREC_KEY,i);
			pDM_MANAGE->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
			pDM_MANAGE->SetCellData(_T("최초작업"),strWorkLog,i);
			pDM_MANAGE->SetCellData(_T("마지막작업"),strWorkLog,i);	

			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_MANAGE, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
		}
		else if(_T("U") == strUDF)
		{

			strREC_KEY = pDM_MANAGE->GetCellData(_T("REC_KEY"),i);
			pDM_MANAGE->SetCellData(_T("마지막작업"),strWorkLog,i);	
			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_MANAGE, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM, _T("REC_KEY"), _T("NUMERIC"), strREC_KEY);

		}

	}
	
	//학과별
	RemoveAliasList.Add(_T("UDF_학과"));
	
	for(i=0;i<pDM_COURCE->GetRowCount();i++)
	{
		
		strUDF = pDM_COURCE->GetCellData(_T("UDF_상태"),i);
		if(_T("I") == strUDF)
		{
			pDM->MakeRecKey(strREC_KEY);
			pDM_COURCE->SetCellData(_T("REC_KEY"),strREC_KEY,i);
			pDM_COURCE->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
			pDM_COURCE->SetCellData(_T("최초작업"),strWorkLog,i);
			pDM_COURCE->SetCellData(_T("마지막작업"),strWorkLog,i);	

			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_COURCE, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
		}
		else if(_T("U") == strUDF)
		{

			strREC_KEY = pDM_COURCE->GetCellData(_T("REC_KEY"),i);
			pDM_COURCE->SetCellData(_T("마지막작업"),strWorkLog,i);	
			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_COURCE, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM, _T("REC_KEY"), _T("NUMERIC"), strREC_KEY);

		}

	}
	
	//학부별
	RemoveAliasList.Add(_T("UDF_학부"));
	
	for(i=0;i<pDM_DEPT->GetRowCount();i++)
	{
		
		strUDF = pDM_DEPT->GetCellData(_T("UDF_상태"),i);
		if(_T("I") == strUDF)
		{
			pDM->MakeRecKey(strREC_KEY);
			pDM_DEPT->SetCellData(_T("REC_KEY"),strREC_KEY,i);
			pDM_DEPT->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
			pDM_DEPT->SetCellData(_T("최초작업"),strWorkLog,i);
			pDM_DEPT->SetCellData(_T("마지막작업"),strWorkLog,i);	

			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_DEPT, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM);
		}
		else if(_T("U") == strUDF)
		{

			strREC_KEY = pDM_DEPT->GetCellData(_T("REC_KEY"),i);
			pDM_DEPT->SetCellData(_T("마지막작업"),strWorkLog,i);	
			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_DEPT, _T("CO_BUDGET_COMPILATION_SUB_TBL"), i , pDM, _T("REC_KEY"), _T("NUMERIC"), strREC_KEY);

		}

	}
	
	//집행내역
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("UDF_집행타입"));
	RemoveAliasList.Add(_T("UDF_집행상태"));

	for(i=0;i<pDM_EXECUTE->GetRowCount();i++)
	{
		
		strUDF = pDM_EXECUTE->GetCellData(_T("UDF_상태"),i);
		if(_T("I") == strUDF)
		{
			pDM->MakeRecKey(strREC_KEY);
			pDM_EXECUTE->SetCellData(_T("REC_KEY"),strREC_KEY,i);
			pDM_EXECUTE->SetCellData(_T("예산편성KEY"),strBUDGET_COMPILATION_KEY,i);
			pDM_EXECUTE->SetCellData(_T("최초작업"),strWorkLog,i);
			pDM_EXECUTE->SetCellData(_T("마지막작업"),strWorkLog,i);	

			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_EXECUTE, _T("CO_BUDGET_EXECUTION_TBL"), i , pDM);
		}
		else if(_T("U") == strUDF)
		{

			strREC_KEY = pDM_EXECUTE->GetCellData(_T("REC_KEY"),i);
			pDM_EXECUTE->SetCellData(_T("마지막작업"),strWorkLog,i);	
			pDM->InitDBFieldData();
			pDM->MakeQueryByDM(RemoveAliasList, pDM_EXECUTE, _T("CO_BUDGET_EXECUTION_TBL"), i , pDM, _T("REC_KEY"), _T("NUMERIC"), strREC_KEY);

		}

	}

	//삭제
	INT nSize = arrayDeleteSubKey.GetSize();
	CString strQuery;
	for(i=0;i<nSize;i++)
	{
		strQuery.Format(_T("DELETE FROM CO_BUDGET_COMPILATION_SUB_TBL WHERE REC_KEY = %s;"),arrayDeleteSubKey.GetAt(i));
		pDM->InitDBFieldData();
		pDM->AddFrame(strQuery);
	}
	
	nSize = arrayDeleteExeKey.GetSize();

	for(i=0;i<nSize;i++)
	{
		strQuery.Format(_T("DELETE FROM CO_BUDGET_EXECUTION_TBL WHERE REC_KEY = %s;"),arrayDeleteExeKey.GetAt(i));
		pDM->InitDBFieldData();
		pDM->AddFrame(strQuery);
	}

	INT ids = pDM->SendFrame();
	pDM->EndFrame();
	if (ids < 0) {
		return -3000;	
	}
	return 0;
}


INT CBudgetApi::DeleteBudgetList(CESL_DataMgr* pDM_OUT,
								 CString strBUDGET_COMPILATION_KEY)
{

	//예산편성삭제

	if(!pDM_OUT || strBUDGET_COMPILATION_KEY.IsEmpty())
	{
		AfxMessageBox(_T("예산편성삭제 관련 정보를 찾을 수 없습니다!"));
		return -1;
	}

	CString strQuery;

	pDM_OUT->StartFrame();


	//예산집행보조
	strQuery.Format(_T("DELETE FROM CO_BUDGET_EXECUTION_SUB_TBL WHERE BUDGET_EXECUTION_KEY IN ")
					_T("(SELECT REC_KEY FROM CO_BUDGET_EXECUTION_TBL WHERE BUDGET_COMPILATION_KEY = %s);")
					,strBUDGET_COMPILATION_KEY);
	pDM_OUT->InitDBFieldData();
	pDM_OUT->AddFrame(strQuery);


	//예산집행
	strQuery.Format(_T("DELETE FROM CO_BUDGET_EXECUTION_TBL WHERE BUDGET_COMPILATION_KEY = %s;")
					,strBUDGET_COMPILATION_KEY);
	pDM_OUT->InitDBFieldData();
	pDM_OUT->AddFrame(strQuery);



	//예산편성보조
	strQuery.Format(_T("DELETE FROM CO_BUDGET_COMPILATION_SUB_TBL WHERE BUDGET_COMPILATION_KEY = %s;")
					,strBUDGET_COMPILATION_KEY);
	pDM_OUT->InitDBFieldData();
	pDM_OUT->AddFrame(strQuery);


	//예산편성
	strQuery.Format(_T("DELETE FROM CO_BUDGET_COMPILATION_TBL WHERE REC_KEY = %s;")
					,strBUDGET_COMPILATION_KEY);
	pDM_OUT->InitDBFieldData();
	pDM_OUT->AddFrame(strQuery);


	INT ids = pDM_OUT->SendFrame();
	pDM_OUT->EndFrame();
	if (ids < 0) {
		return -3000;	
	}

	return 0;
}

INT CBudgetApi::ApplySubExecutionInfo(CESL_DataMgr* pDM_COMPIL_SUB,
									  CESL_DataMgr* pDM_EXE_SUB,
									  CString strSUB_TYPE)
{
	//예산집행보조정보 적용

	if(!pDM_COMPIL_SUB || !pDM_EXE_SUB || strSUB_TYPE.IsEmpty())
	{
		AfxMessageBox(_T("예산집행보조정보 적용 관련 정보를 찾을 수 없습니다!"));
		return -1;
	}

	INT nRowCompilSub = pDM_COMPIL_SUB->GetRowCount();
	if(0 == nRowCompilSub) return 0; //보조 편성 정보 없음

	INT nRowExeSub = pDM_EXE_SUB->GetRowCount();
	if(0 == nRowExeSub) return 0; //보조 집행 정보 없음

	INT nAssign = 0;
	INT nExeSum = 0;
	INT nResidue = 0;
	CString strCODE;
	CString strEXE_CODE;
	CString strEXE_TYPE;
	CString strResidue;
	CString strExeSum;
	CString strBUDGET_EXECUTION_ACCOUNT;
	CString strBUDGET_ASSIGN_ACCOUNT;
	

	CString str64;
	m_pMgr->GetManageValue(_T("기타"), _T("공통"),_T("육사"),_T("육사"),str64);

	for(INT i=0;i<nRowCompilSub;i++)
	{
		strCODE = pDM_COMPIL_SUB->GetCellData(_T("코드"),i);
		strBUDGET_ASSIGN_ACCOUNT = pDM_COMPIL_SUB->GetCellData(_T("예산할당액"),i);
		if(!strBUDGET_ASSIGN_ACCOUNT.IsEmpty()) nAssign = _ttoi(strBUDGET_ASSIGN_ACCOUNT.GetBuffer(0));

		for(INT j=0;j<nRowExeSub;j++)
		{
			strEXE_TYPE = pDM_EXE_SUB->GetCellData(_T("보조유형"),j);
			strEXE_CODE = pDM_EXE_SUB->GetCellData(_T("코드"),j);
			
			if(strEXE_TYPE != strSUB_TYPE || strEXE_CODE != strCODE) continue;

			
			strBUDGET_EXECUTION_ACCOUNT = pDM_EXE_SUB->GetCellData(_T("집행예산"),j);
			if(!strBUDGET_EXECUTION_ACCOUNT.IsEmpty()) nExeSum += _ttoi(strBUDGET_EXECUTION_ACCOUNT.GetBuffer(0));
			
		}
		//2005-11-02 Appended by WEEDBLACK : 육사일경 우 10원단위로 맞춤
		if(str64 == _T("Y"))
		{
			if(nExeSum != 0)
			{
				nExeSum += 5;
				(INT)nExeSum /= 10;
				nExeSum *= 10;
			}
		}
		nResidue = nAssign - nExeSum;
	
		strResidue.Format(_T("%d"),nResidue);
		strExeSum.Format(_T("%d"),nExeSum);

		
		pDM_COMPIL_SUB->SetCellData(_T("UDF_현재사용액"),strExeSum,i);
		pDM_COMPIL_SUB->SetCellData(_T("UDF_남은할당액"),strResidue,i);


		//2004-12-22
		//modified by loveisverb
		nAssign = 0;
		nExeSum = 0; 
		nResidue = 0;
	}

	return 0;
}

INT CBudgetApi::DeleteComma(CString strSrc, CString& strDes)
{
	if( strSrc.IsEmpty() == TRUE)
		return -1;
	
	if(	strSrc.Find(_T(",")) >= 0 )
		strSrc.Replace(_T(","),_T(""));

	strDes = strSrc;

	return 0;
}


INT CBudgetApi::CheckCompilationAcount(INT iMode, CESL_DataMgr * pDM, CString strAcount, CString sKey, INT row /*=0*/)
{
	if( pDM == NULL || sKey == _T(""))
		return -1 ;
	if( strAcount == _T("") )
		strAcount = _T("0");

	CString sAcount, sQuery, sTotal;
	sQuery.Format(_T("SELECT BUDGET_ACCOUNT FROM CO_BUDGET_COMPILATION_TBL WHERE REC_KEY = '%s'"),sKey);
	pDM->GetOneValueQuery(sQuery,sTotal);

	INT nRow = pDM->GetRowCount();
	INT nAcount = _ttoi(strAcount);
	INT nTotalAcount = _ttoi(sTotal);
	
	if( iMode == 1 ) // 예산편성액 보다 할당액의 합이 많으면 안됨
	{
		for(INT i = 0; i< nRow; i++)
		{
			pDM->GetCellData(_T("예산할당액"),i,sAcount);
			nAcount += _ttoi(sAcount);
		}
		
		if(nTotalAcount < nAcount)
		{
			AfxMessageBox(_T("예산편성액보다 할당액의 합은 클 수 없습니다."));
			return -1;
		}
	}
	else
	{
		for(INT i = 0; i< nRow; i++)
		{
			if(row == i)
				continue;
			
			pDM->GetCellData(_T("예산할당액"),i,sAcount);
			nAcount += _ttoi(sAcount);
		}
		
		if(nTotalAcount < nAcount)
		{
			AfxMessageBox(_T("예산편성액보다 할당액의 합은 클 수 없습니다."));
			return -1;
		}
	}
	return 0;
}

INT CBudgetApi::CheckDupCode(CESL_DataMgr * pDM, CString sCode)
{
	if( pDM  == NULL )
		return -1;
	
	CString strCode;
	INT nCnt = pDM->GetRowCount();

	for( INT i = 0; i < nCnt; i++ )
	{
		pDM->GetCellData(_T("코드"),i,strCode);
		
		if(strCode == sCode)
		{
			AfxMessageBox(_T("해당 코드는 이미 할당되었습니다."));
			return -1;
		}
	}

	return 0;
}