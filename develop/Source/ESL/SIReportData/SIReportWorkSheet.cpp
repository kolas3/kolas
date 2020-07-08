// SIReportWorkSheet.cpp: implementation of the CSIReportWorkSheet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportWorkSheet.h"
#include "SIReportObject_Table.h"
#include "SIReportObject_Barcode.h"
#include "SIReportObject_Bitmap.h"
#include "SIReportObject_Line.h"
#include "SIReportObject_Text_Box.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSIReportWorkSheet::CSIReportWorkSheet()
{
	Alias.Empty();
	Idx = 0;
	master_sheet = 0;
}

CSIReportWorkSheet* CSIReportWorkSheet::Clone(CSIReportWorkSheet* pSheet)
{
EFS_BEGIN

	if (pSheet == NULL)
		pSheet = new CSIReportWorkSheet;

	pSheet->Alias = Alias;
	pSheet->Idx = Idx;
	pSheet->master_sheet = master_sheet;
	
	POSITION pos = ObjList.GetHeadPosition();
	while(pos != NULL)
	{
		CSIReportObject* pObject = (CSIReportObject*)ObjList.GetNext(pos);
		if (pObject == NULL) continue;

		CObject* pNewObject = pSheet->AddNewObject(pObject->GetObjType());

		pObject->Clone((CSIReportObject*)pNewObject);
	}

	return pSheet;

EFS_END
return NULL;

}

CSIReportWorkSheet::~CSIReportWorkSheet()
{
	FreeObjects();
}

INT CSIReportWorkSheet::AddNewObject(CSIReportObject *pObject)
{
EFS_BEGIN

	if (pObject == NULL) return -1;
	ObjList.AddTail((CObject*)pObject);
	
	return 0;

EFS_END
return -1;

}

CObject * CSIReportWorkSheet::AddNewObject(INT ObjType)
{
EFS_BEGIN

	CObject *pObject = NULL;
	if (ObjType == OBJECT_TABLE) {
		CSIReportObject_Table *pTable = new CSIReportObject_Table;
		pObject = (CObject*)pTable;
	}
	if (ObjType == OBJECT_BARCODE) {
		CSIReportObject_Barcode *pBarcode = new CSIReportObject_Barcode;
		pObject = (CObject*)pBarcode;
	}
	if (ObjType == OBJECT_BITMAP) {
		CSIReportObject_Bitmap *pBitmap = new CSIReportObject_Bitmap;
		pObject = (CObject*)pBitmap;
	}
	if (ObjType == OBJECT_LINE) {
		CSIReportObject_Line *pLine = new CSIReportObject_Line;
		pObject = (CObject*)pLine;
	}
	if (ObjType == OBJECT_TEXT_BOX) {
		CSIReportObject_Text_Box *pTextBox = new CSIReportObject_Text_Box;
		pObject = (CObject*)pTextBox;
	}
	if (pObject == NULL) return NULL;
	ObjList.AddTail(pObject);
	return pObject;

EFS_END
return NULL;

}

INT CSIReportWorkSheet::FreeObjects(VOID)
{
EFS_BEGIN

	INT count = ObjList.GetCount();
	if (count == 0) return 0;
	CSIReportObject * pData;
	for (INT i = 0; i < count; i++) {
		pData = (CSIReportObject*)ObjList.RemoveHead();
		if (pData) {
			delete pData;
			pData = NULL;
		}
	}
	ObjList.RemoveAll();
	return 0;

EFS_END
return -1;

}

CSIReportObject * CSIReportWorkSheet::GetSIReportObject(INT idx)
{
EFS_BEGIN

	POSITION pos;
	pos = ObjList.FindIndex(idx);
	if (pos == NULL) return NULL;
	return (CSIReportObject *)ObjList.GetAt(pos);

EFS_END
return NULL;

}

INT CSIReportWorkSheet::ReCalcAllRect(INT nLeft, INT nTop)
{
EFS_BEGIN

	INT nData = ObjList.GetCount();
	if (nData == 0) return 0;
	CSIReportObject * pData;
	POSITION pos = ObjList.GetHeadPosition();
	for (INT i = 0; i < nData; i++) {
		pData = (CSIReportObject*)ObjList.GetNext(pos);
		if (pData == NULL) continue;
		if (pData->m_nObjType == OBJECT_TABLE) {
			CSIReportObject_Table *pTable = (CSIReportObject_Table *)pData;
			if (pTable) pTable->ReCalcAllRect(nLeft, nTop);
		}
		if (pData->m_nObjType == OBJECT_BARCODE) {
			pData->ReCalcAllRect(nLeft, nTop);
		}
		if (pData->m_nObjType == OBJECT_BITMAP) {
			pData->ReCalcAllRect(nLeft, nTop);
		}
		if (pData->m_nObjType == OBJECT_LINE) {
			pData->ReCalcAllRect(nLeft, nTop);
		}
		if (pData->m_nObjType == OBJECT_TEXT_BOX) {
			pData->ReCalcAllRect(nLeft, nTop);
		}
	}
	return 0;

EFS_END
return -1;
}

CSIReportObject *CSIReportWorkSheet::FindObject(CString strAlias, INT nStart/*=0*/)
{
EFS_BEGIN
	INT nData = ObjList.GetCount();
	if (nData == 0) return 0;
	CSIReportObject * pData;
	POSITION pos = ObjList.GetHeadPosition();
	for (INT i = 0; i < nData; i++) {
		pData = (CSIReportObject*)ObjList.GetNext(pos);
		if (pData == NULL) continue;
		if (pData->m_strName == strAlias && i >= nStart) return pData;
	}
	return NULL;

EFS_END
return NULL;
}
