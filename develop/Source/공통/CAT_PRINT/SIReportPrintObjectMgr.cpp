// SIReportPrintObjectMgr.cpp: implementation of the CSIReportPrintObjectMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportPrintObjectMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSIReportPrintObjectMgr::CSIReportPrintObjectMgr()
{
	MODE = _T("일반");

	FreeData();
	m_pPageWorkForm = NULL;
	m_nPageCount = 0;
	m_nRows = 3;			m_nCols = 4;
	nLineHeight = 15;
	nStartX = 50;			nStartY = 50;
	nTableWidthGap = 20;	nTableHeightGap = 15;
	nPaper = PAPER_A4_LANDSCAPE;
	nTableWidth = 150;
	bShow_VertLine = FALSE;	bShow_HorzLine = FALSE;

	m_Font.lfHeight = -13;
	m_Font.lfWidth = 0;
	m_Font.lfEscapement = 0;
	m_Font.lfOrientation = 0;
	m_Font.lfWeight = 400;
	m_Font.lfItalic = 0;
	m_Font.lfUnderline = 0;
	m_Font.lfStrikeOut = 0;
	m_Font.lfCharSet = 129;
	m_Font.lfOutPrecision = 3;
	m_Font.lfClipPrecision = 2;
	m_Font.lfQuality = 1;
	m_Font.lfPitchAndFamily = 49;
	_stprintf(m_Font.lfFaceName, _T("굴림체"));

	m_nFontHeight = CalcPositiveFontHeight(m_Font.lfHeight);

	m_bFrawOuterLine = FALSE;

	m_strBarcodeStyle = _T("6");
	m_nBarcodeWidthGap = 5;
	m_nBarcodeHeight = 30;
	m_nHorzAlign = ALIGN_HCENTER;
}

CSIReportPrintObjectMgr::~CSIReportPrintObjectMgr()
{
	FreeData();
	FreeWorkForm();
}

INT CSIReportPrintObjectMgr::FreeData(VOID)
{
	INT nRows = DATA.GetCount();
	if (nRows == 0) return 0;
	CSIReportPrintObject *pData;
	POSITION pos = DATA.GetHeadPosition();
	for (INT i = 0; i < nRows; i++) {
		pData = NULL;
		pData = (CSIReportPrintObject*)DATA.GetNext(pos);
		if (pData) {
			delete pData;
			pData = NULL;
		}
	}
	DATA.RemoveAll();
	return 0;
}

INT CSIReportPrintObjectMgr::GetCount(VOID)
{
	return DATA.GetCount();
}

CSIReportPrintObject * CSIReportPrintObjectMgr::GetAt(INT nIdx)
{
	POSITION pos = DATA.FindIndex(nIdx);
	if (pos == NULL) return NULL;
	CSIReportPrintObject * pData = (CSIReportPrintObject *)DATA.GetAt(pos);
	return pData;
}

CSIReportPrintObject * CSIReportPrintObjectMgr::AddCard(INT nMAX_BOOKCARD_LINE)
{
	CSIReportPrintObject * pData = new CSIReportPrintObject;
	DATA.AddTail((CObject*)pData);
	pData->nMAX_BOOKCARD_LINE = nMAX_BOOKCARD_LINE;

	return pData;
}


INT CSIReportPrintObjectMgr::InitWorkForm(VOID)
{
	INT ids;
	m_pPageWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	ids = m_pPageWorkForm->AddNewSheet();

	return ids;
}

INT CSIReportPrintObjectMgr::FreeWorkForm(VOID)
{
	if (m_pPageWorkForm) {
		delete m_pPageWorkForm;
		m_pPageWorkForm = NULL;
	}
	return 0;
}

INT CSIReportPrintObjectMgr::Print(CSIReportManager *pSIReportManager)
{
	INT nRows = DATA.GetCount();
	if (nRows == 0) return 0;
	CSIReportPrintObject *pData;
	POSITION pos = DATA.GetHeadPosition();
	INT nCPos, nRPos;
	for (INT i = 0; i < nRows; i++) {
		pData = NULL;
		pData = (CSIReportPrintObject*)DATA.GetNext(pos);
		if (i % (m_nRows*m_nCols) == 0) {
			SaveToFile();
			InitWorkForm();
		}
		
		nRPos = (i / m_nCols) % m_nRows;
		nCPos = i % m_nCols;

		if (MODE == _T("일반")) Print_AddCard(i, nRPos, nCPos, pData);
		if (MODE == _T("바코드")) Print_AddBarcode(i, nRPos, nCPos, pData);
	}
	if (m_pPageWorkForm) {
		SaveToFile();		
	}
	return 0;
}

INT CSIReportPrintObjectMgr::Print_AddCard(INT nIdx, INT nRPos, INT nCPos, CSIReportPrintObject* pData)
{
	if (pData == NULL) return -1;
	INT ids = 0;

	INT X, Y;
	X = nStartX + ((nTableWidth + nTableWidthGap) * nCPos);
	Y = nStartY + ((pData->nMAX_BOOKCARD_LINE * nLineHeight + nTableHeightGap) * nRPos);

	CSIReportWorkSheet *pSheet = m_pPageWorkForm->FindSheet(0);
	if (pSheet == NULL) return -1;
	m_pPageWorkForm->paper = nPaper;
	
	CSIReportObject_Table *pTable = (CSIReportObject_Table *)pSheet->AddNewObject(OBJECT_TABLE);
	if (pTable == NULL) return -2;
	pTable->horz_line = bShow_HorzLine;
	pTable->vert_line = bShow_VertLine;

	CString strTableName;
	strTableName.Format(_T("북카드%05d"), nIdx);
	pTable->SetName(strTableName);
	pTable->InitCell(pData->nMAX_BOOKCARD_LINE, 1);

	CSIReportObject_Table_Cell *pCell;
	CRect rect;
	CRect LeftTop_rect, RightBottom_rect;
	for (INT i = 0; i < pData->nMAX_BOOKCARD_LINE; i++) {
		pCell = pTable->GetCell(i, 0);
		
		rect.top = Y + (i * nLineHeight);
		rect.bottom = rect.top + nLineHeight;
		rect.left = X;
		rect.right = X + nTableWidth;

		if (i == 0) LeftTop_rect = rect;
		if (i == (pData->nMAX_BOOKCARD_LINE)-1) RightBottom_rect = rect;
		pCell->text.text = pData->LINE[i];
		pCell->text.font = m_Font;
		pCell->text.size = m_nFontHeight;
		pCell->SetRect(rect);	
		pCell->vert_align = pData->V_ALIGN[i];
		pCell->horz_align = pData->H_ALIGN[i];
	}

	if (m_bFrawOuterLine == TRUE) {
		CRect OuterRectangle_rect;
		OuterRectangle_rect.top = LeftTop_rect.top;
		OuterRectangle_rect.left = LeftTop_rect.left;
		OuterRectangle_rect.bottom = RightBottom_rect.bottom;
		OuterRectangle_rect.right = RightBottom_rect.right;

		CSIReportObject_Table *pTable_Outer = (CSIReportObject_Table *)pSheet->AddNewObject(OBJECT_TABLE);
		pTable_Outer->horz_line = TRUE;
		pTable_Outer->vert_line = TRUE;

		if (pTable_Outer) {
			strTableName.Format(_T("북카드_외곽선%05d"), nIdx);
			pTable_Outer->SetName(strTableName);
			pTable_Outer->InitCell(1, 1);
			pCell = pTable_Outer->GetCell(0, 0);
			if (pCell) {
				pCell->SetRect(OuterRectangle_rect);
			}
		}
	}

	return 0;
}

INT CSIReportPrintObjectMgr::Print_AddBarcode(INT nIdx, INT nRPos, INT nCPos, CSIReportPrintObject* pData)
{
	if (pData == NULL) return -1;
	INT ids = 0;

	INT X, Y;
	X = nStartX + ((nTableWidth + nTableWidthGap) * nCPos);
	Y = nStartY + ((nLineHeight*2 + m_nBarcodeHeight + nTableHeightGap) * nRPos);

	CSIReportWorkSheet *pSheet = m_pPageWorkForm->FindSheet(0);
	if (pSheet == NULL) return -1;
	m_pPageWorkForm->paper = nPaper;
	
	CString strTableName;
	CSIReportObject_Table *pTable;
	CSIReportObject_Table_Cell *pCell;
	CRect rect;
	
	pTable = (CSIReportObject_Table *)pSheet->AddNewObject(OBJECT_TABLE);
	if (pTable == NULL) return -2;
	strTableName.Format(_T("바코드_타이틀_%05d"), nIdx);
	pTable->SetName(strTableName);
	pTable->horz_line = bShow_HorzLine;
	pTable->vert_line = bShow_VertLine;
	pTable->InitCell(1, 1);
	pCell = pTable->GetCell(0, 0);
	if (pCell == NULL) return -3;
	
	if ( pData->LINE[0] == _T("대출자번호_바코드") )
	{
		if ( nRPos > 10 ) Y += -1;

		rect.top = Y;				rect.bottom = Y;
		pCell->text.size = 0;
	}
	else
	{
		rect.top = Y;				rect.bottom = Y + nLineHeight;	
		pCell->text.size = m_nFontHeight;
	}
	rect.left = X;				rect.right = X + nTableWidth;
	pCell->SetRect(rect);
	pCell->text.text = pData->LINE[0];
	pCell->text.font = m_Font;
	pCell->SetRect(rect);	
	pCell->vert_align = pData->V_ALIGN[0];
	pCell->horz_align = m_nHorzAlign;

	CSIReportObject_Barcode *pBarcode = (CSIReportObject_Barcode *)pSheet->AddNewObject(OBJECT_BARCODE);
	if (pBarcode == NULL) return -4;
	strTableName.Format(_T("바코드_%05d"), nIdx);
	pBarcode->SetName(strTableName);
	rect.top = rect.bottom;
	rect.bottom = rect.top + m_nBarcodeHeight;
	rect.DeflateRect(m_nBarcodeWidthGap, 0);
	
	if ( pData->LINE[0] == _T("대출자번호_바코드") )
		pBarcode->SetStyle(18);
	else
		pBarcode->SetStyle(m_strBarcodeStyle);

	pBarcode->SetCaption(pData->LINE[1]);
	pBarcode->SetRect(rect);

	pTable = (CSIReportObject_Table *)pSheet->AddNewObject(OBJECT_TABLE);
	if (pTable == NULL) return -2;
	strTableName.Format(_T("바코드_캡션_%05d"), nIdx);
	pTable->SetName(strTableName);
	pTable->horz_line = bShow_HorzLine;
	pTable->vert_line = bShow_VertLine;
	pTable->InitCell(1, 1);
	pCell = pTable->GetCell(0, 0);
	if (pCell == NULL) return -3;
	rect.top = rect.bottom;		rect.bottom = rect.top + nLineHeight;
	rect.left = X;				rect.right = X + nTableWidth;
	pCell->SetRect(rect);
	pCell->text.text = pData->LINE[2];
	pCell->text.font = m_Font;
	pCell->text.size = m_nFontHeight;
	pCell->SetRect(rect);	
	pCell->vert_align = pData->V_ALIGN[2];
	pCell->horz_align = m_nHorzAlign;

	if (m_bFrawOuterLine == TRUE) {
		CRect OuterRectangle_rect;
		OuterRectangle_rect.top = Y;
		OuterRectangle_rect.left = X;
		OuterRectangle_rect.bottom = Y + (nLineHeight*2) + m_nBarcodeHeight;
		OuterRectangle_rect.right = X + nTableWidth;

		CSIReportObject_Table *pTable_Outer = (CSIReportObject_Table *)pSheet->AddNewObject(OBJECT_TABLE);
		pTable_Outer->horz_line = TRUE;
		pTable_Outer->vert_line = TRUE;

		if (pTable_Outer) {
			strTableName.Format(_T("외곽선%05d"), nIdx);
			pTable_Outer->SetName(strTableName);
			pTable_Outer->InitCell(1, 1);
			pCell = pTable_Outer->GetCell(0, 0);
			if (pCell) {
				pCell->SetRect(OuterRectangle_rect);
			}
		}
	}

	return 0;
}

INT CSIReportPrintObjectMgr::SaveToFile(VOID)
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

INT CSIReportPrintObjectMgr::SetFont(LOGFONT font)
{
	INT nBeforeFontHeight = ABS(m_Font.lfHeight);
	INT nAfterFontHeight = ABS(font.lfHeight);
	m_Font = font;
	m_nFontHeight = CalcPositiveFontHeight(m_Font.lfHeight);
	if (nAfterFontHeight > nBeforeFontHeight) nLineHeight = nLineHeight + (nAfterFontHeight-nBeforeFontHeight);
	return 0;
}

INT CSIReportPrintObjectMgr::SetFont(INT nFontHeight, CString strFontFace, INT nFontStyle)
{
	LOGFONT tmpFont = m_Font;
	tmpFont.lfHeight = CalcNegativeFontHeight(nFontHeight);
	tmpFont.lfWeight = nFontStyle;
	_stprintf(tmpFont.lfFaceName, _T("%s"), strFontFace.GetBuffer(0));

	SetFont(tmpFont);

	return 0;
}

INT CSIReportPrintObjectMgr::CalcNegativeFontHeight(INT nSize)
{
	INT nTmp;
	nTmp = MulDiv(nSize, 96, 72);
	nTmp = (nTmp > 0) ? -nTmp : nTmp;	

	return nTmp;
}

INT CSIReportPrintObjectMgr::CalcPositiveFontHeight(INT nSize)
{
	INT nTmp;
	nTmp = MulDiv(nSize, 72, 96);
	nTmp = (nTmp < 0) ? -nTmp : nTmp;	

	return nTmp;
}

INT CSIReportPrintObjectMgr::ABS(INT nData)
{
	return ((nData < 0) ? -nData : nData);
}
