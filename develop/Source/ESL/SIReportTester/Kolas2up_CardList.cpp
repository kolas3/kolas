// Kolas2up_CardList.cpp: implementation of the CKolas2up_CardList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportTester.h"
#include "Kolas2up_CardList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKolas2up_CardList::CKolas2up_CardList()
{
	DATA.RemoveAll();
	m_pPageWorkForm = NULL;

	m_nPageCount = 0;

	nLineHeight = 15;
	nStartX = 50;
	nStartY = 50;
	nTableGap = 20;
	nTableWidth = 500;
	bShow_VertLine = FALSE;
	bShow_HorzLine = FALSE;

	nPaper = PAPER_A4_PORTRAIT;
}

CKolas2up_CardList::~CKolas2up_CardList()
{
	FreeData();
	FreeWorkForm();
}

INT CKolas2up_CardList::FreeData(VOID)
{
	INT nRows = DATA.GetCount();
	if (nRows == 0) return 0;
	CKolas2up_Card *pData;
	POSITION pos = DATA.GetHeadPosition();
	for (INT i = 0; i < nRows; i++) {
		pData = NULL;
		pData = (CKolas2up_Card*)DATA.GetNext(pos);
		if (pData) {
			delete pData;
			pData = NULL;
		}
	}
	DATA.RemoveAll();
	return 0;
}

INT CKolas2up_CardList::GetCount(VOID)
{
	return DATA.GetCount();
}

CKolas2up_Card * CKolas2up_CardList::GetAt(INT nIdx)
{
	POSITION pos = DATA.FindIndex(nIdx);
	CKolas2up_Card * pData = (CKolas2up_Card *)DATA.GetAt(pos);
	return pData;
}

CKolas2up_Card * CKolas2up_CardList::AddCard(VOID)
{
	CKolas2up_Card * pData = new CKolas2up_Card;
	DATA.AddTail((CObject*)pData);

	return pData;
}

INT CKolas2up_CardList::Print(CSIReportManager *pSIReportManager)
{
	INT nRows = DATA.GetCount();
	if (nRows == 0) return 0;
	CKolas2up_Card *pData;
	POSITION pos = DATA.GetHeadPosition();
	for (INT i = 0; i < nRows; i++) {
		pData = NULL;
		pData = (CKolas2up_Card*)DATA.GetNext(pos);

		if (i % 3 == 0) {
			SaveToFile();
			InitWorkForm();
		}

		Print_AddCard(i % 3, pData);

	}
	if (m_pPageWorkForm) {
		SaveToFile();		
	}
	return 0;
}

INT CKolas2up_CardList::Print_AddCard(INT nPos, CKolas2up_Card* pData)
{
	INT ids = 0;

	INT X, Y;
	X = nStartX; 
	Y = nStartY;

	Y = Y + ((MAX_CARD_LINE * nLineHeight + nTableGap) * nPos);

	CSIReportWorkSheet *pSheet = m_pPageWorkForm->FindSheet(0);
	if (pSheet == NULL) return -1;
	m_pPageWorkForm->paper = nPaper;
	
	CSIReportObject_Table *pTable = (CSIReportObject_Table *)pSheet->AddNewObject(OBJECT_TABLE);
	if (pTable == NULL) return -2;
	pTable->horz_line = bShow_HorzLine;
	pTable->vert_line = bShow_VertLine;

	CString strTableName;
	strTableName.Format(_T("Å×ÀÌºí%d"), nPos);
	pTable->SetName(strTableName);
	pTable->InitCell(MAX_CARD_LINE, 1);

	CSIReportObject_Table_Cell *pCell;
	CRect rect;
	for (INT i = 0; i < MAX_CARD_LINE; i++) {
		pCell = pTable->GetCell(i, 0);

		rect.top = Y + (i * nLineHeight);
		rect.bottom = rect.top + nLineHeight;
		rect.left = X;
		rect.right = X + nTableWidth;

		pCell->text.text = pData->LINE[i];
		pCell->SetRect(rect);
		
	}
	return 0;
}

INT CKolas2up_CardList::InitWorkForm(VOID)
{
	INT ids;
	if (m_pPageWorkForm) FreeWorkForm();
	m_pPageWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	ids = m_pPageWorkForm->AddNewSheet();

	return ids;
}

INT CKolas2up_CardList::FreeWorkForm(VOID)
{
	if (m_pPageWorkForm) {
		delete m_pPageWorkForm;
		m_pPageWorkForm = NULL;
	}
	return 0;
}

INT CKolas2up_CardList::SaveToFile(VOID)
{
	if (m_pPageWorkForm == NULL) return -2;
	m_nPageCount++;
	INT ids;

	CSIReportXMLIOMgr *pXMLIOMgr = new CSIReportXMLIOMgr(MASTER_IS_MANAGER);
	if (pXMLIOMgr == NULL) return -1;

	CString strFileName;
	strFileName.Format(_T("rpt_tmp\\page%04d.xml"), m_nPageCount);

	ids = pXMLIOMgr->SaveToXML(m_pPageWorkForm, strFileName);

	if (pXMLIOMgr) {
		delete pXMLIOMgr;
		pXMLIOMgr = NULL;
	}
	FreeWorkForm();
	return 0;
}