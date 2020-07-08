// SIReportWorkForm.cpp: implementation of the CSIReportWorkForm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportWorkForm.h"
#include "SIReportXMLIOMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSIReportWorkForm::CSIReportWorkForm(INT MASTER_FLAG)
{
	paper = PAPER_A4_LANDSCAPE;
	VERSION = 0;
	sheet_height = 0;
	sheet_size = 0;
	alias = _T("WORKFORM");
	this->MASTER_FLAG = MASTER_FLAG;
}

CSIReportWorkForm::~CSIReportWorkForm()
{
	FreeSheets();
}

INT CSIReportWorkForm::AddNewSheet(VOID)
{
EFS_BEGIN

	CSIReportWorkSheet *pWorkSheet = new CSIReportWorkSheet;
	if (!pWorkSheet) return -1;
	WorkSheetList.AddTail((CObject*)pWorkSheet);
	return 0;

EFS_END
return -1;

}

INT CSIReportWorkForm::AddNewSheet(CSIReportWorkSheet *pWorkSheet)
{
EFS_BEGIN

	if (!pWorkSheet) return -1;
	WorkSheetList.AddTail((CObject*)pWorkSheet);
	return 0;

EFS_END
return -1;

}

INT CSIReportWorkForm::FreeSheets(VOID)
{
EFS_BEGIN

	INT count = WorkSheetList.GetCount();
	if (count == 0) return 0;
	CSIReportWorkSheet * pData;
	for (INT i = 0; i < count; i++) {
		pData = (CSIReportWorkSheet*)WorkSheetList.RemoveHead();
		if (pData) {
			delete pData;
			pData = NULL;
		}
	}
	WorkSheetList.RemoveAll();
	return 0;

EFS_END
return -1;

}

CSIReportWorkSheet * CSIReportWorkForm::GetWorkSheet(INT idx) 
{
EFS_BEGIN

	POSITION pos;
	pos = WorkSheetList.FindIndex(idx);
	if (pos == NULL) return NULL;
	return (CSIReportWorkSheet *)WorkSheetList.GetAt(pos);

EFS_END
return NULL;

}

INT CSIReportWorkForm::LoadWorkForm(CString strFileName)
{
EFS_BEGIN

	CSIReportXMLIOMgr xmlIOMgr(MASTER_FLAG);

	return xmlIOMgr.LoadWorkForm(this, strFileName);

EFS_END
return -1;

}

INT CSIReportWorkForm::ReCalcAllRect(INT nLeft, INT nTop)
{
EFS_BEGIN

	INT nData = WorkSheetList.GetCount();
	if (nData == 0) return 0;
	CSIReportWorkSheet * pData;
	POSITION pos = WorkSheetList.GetHeadPosition();
	for (INT i = 0; i < nData; i++) {
		pData = (CSIReportWorkSheet*)WorkSheetList.GetNext(pos);
		if (pData) {
			pData->ReCalcAllRect(nLeft, nTop);
		}
	}
	return 0;

EFS_END
return -1;
}

CSIReportWorkSheet* CSIReportWorkForm::FindSheet(INT nIdx)
{
EFS_BEGIN
	INT nData = WorkSheetList.GetCount();
	if (nData == 0) return 0;
	CSIReportWorkSheet * pData;
	POSITION pos = WorkSheetList.GetHeadPosition();
	for (INT i = 0; i < nData; i++) {
		pData = NULL;
		pData = (CSIReportWorkSheet*)WorkSheetList.GetNext(pos);
		if (pData) {
			if (i == nIdx) return pData;
		}
	}
	return NULL;
EFS_END
return NULL;
}

CSIReportWorkSheet* CSIReportWorkForm::FindSheet(CString strAlias)
{
EFS_BEGIN
	INT nData = WorkSheetList.GetCount();
	if (nData == 0) return 0;
	CSIReportWorkSheet * pData;
	POSITION pos = WorkSheetList.GetHeadPosition();
	for (INT i = 0; i < nData; i++) {
		pData = NULL;
		pData = (CSIReportWorkSheet*)WorkSheetList.GetNext(pos);
		if (pData) {
			if (pData->Alias == strAlias) return pData;
		}
	}
	return NULL;
EFS_END
return NULL;
}