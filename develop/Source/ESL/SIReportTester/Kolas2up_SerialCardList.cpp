// Kolas2up_SerialCardList.cpp: implementation of the CKolas2up_SerialCardList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportTester.h"
#include "Kolas2up_SerialCardList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKolas2up_SerialCardList::CKolas2up_SerialCardList()
{
	FreeData();
	m_pPageWorkForm = NULL;
	m_nPageCount = 0;
	m_nRows = 1;			m_nCols = 2;
	nLineHeight = 10;
	nStartX = 20;			nStartY = 20;
	nTableWidthGap = 10;	nTableHeightGap = 92;
	nPaper = SIReportMgr_UserDefinePaper;			// 사용자 설정 용지를 사용해야 함
	nTableWidth = 400;
	bShow_VertLine = FALSE;	bShow_HorzLine = FALSE;

	// 디폴트 폰트 설정
	m_Font.lfHeight = -12;
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
}

CKolas2up_SerialCardList::~CKolas2up_SerialCardList()
{
	FreeData();
	FreeWorkForm();
}

INT CKolas2up_SerialCardList::FreeData(VOID)
{
	INT nRows = DATA.GetCount();
	if (nRows == 0) return 0;
	CKolas2up_SerialCard *pData;
	POSITION pos = DATA.GetHeadPosition();
	for (INT i = 0; i < nRows; i++) {
		pData = NULL;
		pData = (CKolas2up_SerialCard*)DATA.GetNext(pos);
		if (pData) {
			delete pData;
			pData = NULL;
		}
	}
	DATA.RemoveAll();
	return 0;
}

INT CKolas2up_SerialCardList::GetCount(VOID)
{
	return DATA.GetCount();
}

CKolas2up_SerialCard * CKolas2up_SerialCardList::GetAt(INT nIdx)
{
	POSITION pos = DATA.FindIndex(nIdx);
	if (pos == NULL) return NULL;
	CKolas2up_SerialCard * pData = (CKolas2up_SerialCard *)DATA.GetAt(pos);
	return pData;
}

CKolas2up_SerialCard * CKolas2up_SerialCardList::AddCard(INT nMAX_BOOKCARD_LINE)
{
	CKolas2up_SerialCard * pData = new CKolas2up_SerialCard;
	DATA.AddTail((CObject*)pData);
	pData->nMAX_BOOKCARD_LINE = nMAX_BOOKCARD_LINE;

	return pData;
}


INT CKolas2up_SerialCardList::InitWorkForm(VOID)
{
	INT ids;
	CSIReportWorkForm *pPageWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	ids = pPageWorkForm->AddNewSheet();
	m_pPageWorkForm = pPageWorkForm;
	return ids;
}

INT CKolas2up_SerialCardList::FreeWorkForm(VOID)
{
	if (m_pPageWorkForm) {
		delete m_pPageWorkForm;
		m_pPageWorkForm = NULL;
	}
	return 0;
}

INT CKolas2up_SerialCardList::Print(CSIReportManager *pSIReportManager)
{
	INT nRows = DATA.GetCount();
	if (nRows == 0) return 0;
	CKolas2up_SerialCard *pData;
	POSITION pos = DATA.GetHeadPosition();
	INT nCPos, nRPos;
	for (INT i = 0; i < nRows; i++) {
		pData = NULL;
		pData = (CKolas2up_SerialCard*)DATA.GetNext(pos);
		if (i % (m_nRows*m_nCols) == 0) {
			SaveToFile();
			InitWorkForm();
		}
		
		nRPos = (i / m_nCols) % m_nRows;
		nCPos = i % m_nCols;

		Print_AddCard(i, nRPos, nCPos, pData);
	}
	if (m_pPageWorkForm) {
		SaveToFile();		
	}
	return 0;
}

INT CKolas2up_SerialCardList::Print_AddCard(INT nIdx, INT nRPos, INT nCPos, CKolas2up_SerialCard* pData)
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
		pCell->vert_align = ALIGN_TOP;
	}
	return 0;
}

INT CKolas2up_SerialCardList::SaveToFile(VOID)
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

INT CKolas2up_SerialCardList::SetFont(LOGFONT font)
{
	INT nBeforeFontHeight = ABS(m_Font.lfHeight);
	INT nAfterFontHeight = ABS(font.lfHeight);
	m_Font = font;
	m_nFontHeight = CalcPositiveFontHeight(m_Font.lfHeight);
	if (nAfterFontHeight > nBeforeFontHeight) nLineHeight = nLineHeight + (nAfterFontHeight-nBeforeFontHeight);
	return 0;
}

INT CKolas2up_SerialCardList::SetFont(INT nFontHeight, CString strFontFace)
{
	LOGFONT tmpFont = m_Font;
	tmpFont.lfHeight = CalcNegativeFontHeight(nFontHeight);
	_stprintf(tmpFont.lfFaceName, _T("%s"), strFontFace.GetBuffer(0));

	SetFont(tmpFont);

	return 0;
}

INT CKolas2up_SerialCardList::CalcNegativeFontHeight(INT nSize)
{
	INT nTmp;
	nTmp = MulDiv(nSize, 96, 72);
	nTmp = (nTmp > 0) ? -nTmp : nTmp;	

	return nTmp;
}

INT CKolas2up_SerialCardList::CalcPositiveFontHeight(INT nSize)
{
	INT nTmp;
	nTmp = MulDiv(nSize, 72, 96);
	nTmp = (nTmp < 0) ? -nTmp : nTmp;	

	return nTmp;
}

INT CKolas2up_SerialCardList::ABS(INT nData)
{
	return ((nData < 0) ? -nData : nData);
}

INT CKolas2up_SerialCardList::SetRows(INT nRows)
{
	m_nRows = nRows;
	return 0;
}

INT CKolas2up_SerialCardList::SetCols(INT nCols)
{
	m_nCols = nCols;
	return 0;
}

INT CKolas2up_SerialCardList::SetShowHorzLine(BOOL bShow)
{
	bShow_HorzLine = bShow;
	return 0;
}

INT CKolas2up_SerialCardList::SetShowVertLine(BOOL bShow)
{
	bShow_VertLine = bShow;	
	return 0;
}

INT CKolas2up_SerialCardList::SetLineHeight(INT nHeight)
{
	nLineHeight = nHeight;
	return 0;
}

INT CKolas2up_SerialCardList::SetStartCoordinate(INT nSX, INT nSY)
{
	nStartX = nSX;
	nStartY = nSY;
	return 0;
}

INT CKolas2up_SerialCardList::SetTableWidthGap(INT nGap)
{
	nTableWidthGap = nGap;
	return 0;
}

INT CKolas2up_SerialCardList::SetTableHeightGap(INT nGap)
{
	nTableHeightGap = nGap;
	return 0;
}

INT CKolas2up_SerialCardList::SetTableWidth(INT nWidth)
{
	nTableWidth = nWidth;
	return 0;
}
