// SIReportManager.cpp: implementation of the CSIReportManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "..\SIReportData\SIReportConstant.h"
#include "..\SIReportData\SIReportObject_Table_Cell.h"
#include "SIReportPreviewDlg.h"
#include "..\SIReportData\SIReportObject_Barcode.h"
#include <direct.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
bool bGfxUserAbort;

BOOL CALLBACK EXPORT _GfxAbortProc( HDC, INT )
{
	MSG msg;
	while (!bGfxUserAbort && ::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
	{
		if (!AfxGetThread()->PumpMessage()) return FALSE;
	}
	return !bGfxUserAbort;
}

CSIReportManager::CSIReportManager(CWnd *pOwnerWnd)
{
	m_pWorkForm = NULL;
	m_pPageWorkForm = NULL;
	InitDMArray();
	m_DMCursor = -1;		// 아직 데이타 사용안함

	// XML IO Mgr
	m_pXMLIOMgr = new CSIReportXMLIOMgr(MASTER_IS_MANAGER);

	m_pOwnerWnd = pOwnerWnd;
	m_fpEventHandler = NULL;

	// (START) - 이하여백 메시지 설정 초기화 부분
	m_strNoMoreStatement = _T("이 하 여 백");
	m_nNoMoreStatement = 10; // 10mm

	m_fNoMoreStatement.lfCharSet = DEFAULT_CHARSET;
	ZeroMemory(&m_fNoMoreStatement,sizeof(LOGFONT));
	_tcscpy(m_fNoMoreStatement.lfFaceName,_T("굴림체"));
	m_fNoMoreStatement.lfHeight = 10;
	// ( END ) - 이하여백 메시지 설정 초기화 부분

	m_strPartSum = _T("소  계");			
	m_nColPartSum = 0;
	m_strTotalSum = _T("총  계");			
	m_nColTotalSum = 0;

	XML_MODE = 0;			// NOT XML MODE로 시작
	m_strOriginalWorkFormFileName = _T("");
	m_strPageHeader = _T("page");

	_tmkdir(_T("rpt_tmp"));
	_tmkdir(MANAGER_TEMP_DIR);
}

CSIReportManager::~CSIReportManager()
{
	if (m_pWorkForm) {
		delete m_pWorkForm;
		m_pWorkForm = NULL;
	}
	if (m_pXMLIOMgr) {
		delete m_pXMLIOMgr;
		m_pXMLIOMgr = NULL;
	}
	FreePageWorkForm();
}

INT CSIReportManager::FreePageWorkForm(VOID)
{
	if (m_pPageWorkForm) {
		delete m_pPageWorkForm;
		m_pPageWorkForm = NULL;
	}
	return 0;
}

INT CSIReportManager::PrintDocument(INT nCopy, INT nFromPage, INT nToPage, CWnd *pMsg, CProgressCtrl *pBar)
{
	// 사용할 프린터를 결정한다
	CPrintDialog pd(FALSE, PD_NONETWORKBUTTON|PD_HIDEPRINTTOFILE|PD_NOSELECTION|PD_NOSELECTION);
	if (pd.DoModal() != IDOK) return -1;
	Sleep(10);
	m_pd.hDevMode = pd.m_pd.hDevMode;
	m_pd.hDevNames = pd.m_pd.hDevNames;
	CString devicename = pd.GetDeviceName();
	bool m_bPdInit = true;
	bool bError;

	// 출력모드를 가로출력 으로 바꾼다.
	LPDEVMODE pDevMode = pd.GetDevMode();
	if (m_pWorkForm->paper == PAPER_A4_LANDSCAPE) pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
	if (m_pWorkForm->paper == PAPER_A4_PORTRAIT) pDevMode->dmOrientation = DMORIENT_PORTRAIT;
	pd.m_pd.hDevMode = pDevMode;
	Sleep(10);
	
	// PRINTER DEVICE를 얻어온다.
	LPDEVNAMES lpDevNames = (m_pd.hDevNames != NULL) ? 
					(LPDEVNAMES)::GlobalLock(m_pd.hDevNames) : NULL;
	if (lpDevNames == NULL) return -1;
	LPDEVMODE  lpDevMode = (m_pd.hDevMode != NULL) ?
					(LPDEVMODE)::GlobalLock(m_pd.hDevMode) : NULL;
	if (lpDevMode == NULL) return -1;
	Sleep(10);

	HDC hDC = ::CreateDC((LPCTSTR)lpDevNames + lpDevNames->wDriverOffset,
					  (LPCTSTR)lpDevNames + lpDevNames->wDeviceOffset,
					  (LPCTSTR)lpDevNames + lpDevNames->wOutputOffset,
					  lpDevMode);	
	if (hDC == NULL) return -1;

	if (m_pd.hDevNames != NULL) ::GlobalUnlock(m_pd.hDevNames);
	if (m_pd.hDevMode != NULL) ::GlobalUnlock(m_pd.hDevMode);


	// 그림을 그릴 DC를 생성한다
    CDC * m_pDC = new CDC;
	Sleep(10);

	m_pDC->Attach(hDC);
	INT nSaveDC = m_pDC->SaveDC();
	Sleep(10);

	// 현재 DEVICE의 여백을 가져온다
	INT width = m_pDC->GetDeviceCaps(HORZSIZE);
	INT height = m_pDC->GetDeviceCaps(VERTSIZE);
	m_printHorzR = m_pDC->GetDeviceCaps(HORZRES);
	m_printVertR = m_pDC->GetDeviceCaps(VERTRES);
	INT printLogX = m_pDC->GetDeviceCaps(LOGPIXELSX);
	INT printLogY = m_pDC->GetDeviceCaps(LOGPIXELSY);
	Sleep(10);
	
	// PAPER SIZE를 설정한다.
	m_drawSize.cx = (INT)(1440 * (double)(width / 25.4 ));//tempSize.cx ; 11225
	m_drawSize.cy = (INT)(1440 * (double)(height / 25.4));//tempSize.cy ; 16157
	Sleep(10);

	m_pDC->SetBkMode(TRANSPARENT); // 투명하게 한다.

	DOCINFO di = {sizeof(DOCINFO), _T("ECO SI Report Manager 출력"), NULL}; // 출력시 이름 설정
	m_pDC->StartDoc(&di);

	// Work Form을 출력해낸다. 페이지수는 워크폼에 포함된 쉿트의 갯수나, 데이타의 양에 따라 결정된다.
	for (INT i = 0; i < nCopy; i++) {
		PRINT_DrawWorkForm(m_pDC, nFromPage, nToPage, pMsg, pBar);
	}

	bError = FALSE;
	if (!bError) 
		m_pDC->EndDoc();	// print작업을 끝낸다.
	else 
		m_pDC->AbortDoc();	// print작업을 취소한다.
	Sleep(10);
	
	m_pDC->RestoreDC(nSaveDC);	// 다시 DC를 RETURN한다.
	Sleep(10);

	// print CDC객체 소멸
	m_pDC->DeleteDC();
	m_pDC->Detach();
    delete m_pDC;
  
    return 0;
}

INT CSIReportManager::DrawTextWithFont(INT mode, CDC *dc, CString str, CRect rect, LOGFONT logfont, UINT nFormat /*= DT_TOP|DT_CENTER |DT_WORDBREAK*/)
{
	if (dc == NULL) return -1;
	
	CFont *pOldFont = NULL;
	INT height = -1;

	INT fontsize = logfont.lfHeight;
	CFont font;

	if (mode == MANAGER_MODE_PRINT) {		// 인쇄모드일때 사이즈 계산
		logfont.lfHeight = -MulDiv(fontsize, 1440, 72);									
	} else {				// Preview모드 일때의 사이즈 계산
		logfont.lfHeight = -MulDiv(fontsize, GetDeviceCaps(dc->m_hDC, LOGPIXELSY), 72);
	}
	font.CreateFontIndirect(&logfont);

	pOldFont = dc->SelectObject(&font);

	height = dc->DrawText(str, rect, nFormat);
	dc->SelectObject(pOldFont);
	return height;
}

INT CSIReportManager::DrawTextWithFont2(INT mode, CDC *dc, CString str, INT char_len, INT char_cy, CRect rect, LOGFONT logfont, UINT nFormat /*= DT_TOP|DT_CENTER |DT_WORDBREAK*/)
{
	if (dc == NULL) return -1;
	
	CFont *pOldFont = NULL;
	INT height = -1;

	INT fontsize, nPixel_cy;
	CFont font;
	fontsize = logfont.lfHeight;
	if (mode == MANAGER_MODE_PRINT) {		// 인쇄모드
		logfont.lfHeight = -MulDiv(fontsize, 1440, 72);
	} else {								// Preview 모드
		logfont.lfHeight = -MulDiv(fontsize, GetDeviceCaps(dc->m_hDC, LOGPIXELSY), 72);
	}
	nPixel_cy = (logfont.lfHeight < 0) ? logfont.lfHeight * -1 : logfont.lfHeight;
	font.CreateFontIndirect(&logfont);

	pOldFont = dc->SelectObject(&font);
	INT nStartY = rect.top;

	CString token;
	INT pos = 0;
	while(true) {
		rect.top = nStartY;
		rect.bottom = nStartY + nPixel_cy;
		if (str.GetLength() == 0) break;
		pos = char_len;
		if (str.GetLength() > pos) {
			if (!IsCanCut(str, pos-1)) {
				pos++;
			}
		} else {
			pos = str.GetLength();
		}
		token = str.Left(pos);
		height = dc->DrawText(token, rect, nFormat);
		str = str.Mid(pos);
		str.TrimLeft();
		str.TrimRight();

		nStartY += nPixel_cy;
	}
	dc->SelectObject(pOldFont);
	return height;
}

INT CSIReportManager::DrawTextWithFont3(INT mode, CDC *dc, CString str, CRect rect, LOGFONT logfont, UINT nFormat /*= DT_TOP|DT_CENTER |DT_WORDBREAK*/)
{
	if (dc == NULL) return -1;
	
	CFont *pOldFont = NULL;
	INT height = -1;

	INT fontsize, nPixel_cy;
	CFont font;
	fontsize = logfont.lfHeight;
	if (mode == MANAGER_MODE_PRINT) {		// 인쇄모드
		logfont.lfHeight = -MulDiv(fontsize, 1440, 72);
	} else {								// Preview 모드
		logfont.lfHeight = -MulDiv(fontsize, GetDeviceCaps(dc->m_hDC, LOGPIXELSY), 72);
	}
	nPixel_cy = (logfont.lfHeight < 0) ? logfont.lfHeight * -1 : logfont.lfHeight;
	font.CreateFontIndirect(&logfont);

	pOldFont = dc->SelectObject(&font);
	INT nStartY = rect.top;

	CString token;
	INT pos = 0;
	while(true) {
		rect.top = nStartY;
		rect.bottom = nStartY + nPixel_cy;
		if (str.GetLength() == 0) break;
		pos = str.Find('\n', 0);
		if (pos == -1) pos = str.GetLength();
		token = str.Left(pos);
		token.TrimLeft();				token.TrimRight();
		height = dc->DrawText(token, rect, nFormat);
		str = str.Mid(pos);
		str.TrimLeft();
		str.TrimRight();

		nStartY += nPixel_cy;
	}
	dc->SelectObject(pOldFont);
	return height;
}

INT CSIReportManager::PRINT_DrawWorkForm(CDC *pDC, INT nStartPage, INT nFinishPage, CWnd *pMsg, CProgressCtrl *pBar)
{
	m_EventInfo.m_pSIReportManager = (CObject*)this;
	m_EventInfo.mode = MANAGER_MODE_PRINT;
	m_DMCursor = -1;		// 아직 데이타 사용안함

	INT nPrintPages = nFinishPage - nStartPage + 1;
	INT nPrintedCount = 0;
	CString msg;

	if (!pDC) return -1;

	CString strTmp;
	CSIReportWorkSheet * pSheet;
	POSITION pos;
	INT ids;
	INT nSheet;

	for (INT i = nStartPage; i <= nFinishPage; i++) {
		nPrintedCount++;
		pDC->StartPage(); // 페이지 출력 시작
		pDC->SetMapMode(MM_ANISOTROPIC);
		pDC->SetWindowExt(1440, 1440);
		pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
		INT x = pDC->GetDeviceCaps(LOGPIXELSX);
		INT y = pDC->GetDeviceCaps(LOGPIXELSY);

		// EventHandler처리
		m_EventInfo.pDC = pDC;
		m_EventInfo.m_nPage = i;
		if (m_pOwnerWnd && m_fpEventHandler) {
			( m_pOwnerWnd->*m_fpEventHandler )( &m_EventInfo );
		}

		// (START) - Page XML을 Load 하여 Print 한다.
		strTmp.Format(_T("rpt_tmp\\%s%04d.xml"), m_strPageHeader, i);
		CFile fd;
		if (fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false) continue;
		fd.Close();
		LoadWorkForm(strTmp);
		pos = m_pWorkForm->WorkSheetList.GetHeadPosition();
		// ( END ) - Page XML을 Load 하여 Print 한다.
		for (nSheet = 0; nSheet < m_pWorkForm->WorkSheetList.GetCount(); nSheet++) {
			pSheet = NULL;
			pSheet = (CSIReportWorkSheet*)m_pWorkForm->WorkSheetList.GetNext(pos);
			if (!pSheet) continue;
			ids = DrawWorkSheet(MANAGER_MODE_PRINT, pDC, pSheet);
		}
		if (pDC->EndPage() < 0 || !_GfxAbortProc(pDC->GetSafeHdc(), 0)) {
			//bError = TRUE;
			break;
		}
		msg.Format(_T("%d/%d(%d페이지 인쇄중)"), nPrintedCount, nPrintPages, i);
		if (pMsg) pMsg->SetWindowText(msg);
		if (pBar) pBar->SetPos((INT)(nPrintedCount * 100 / nPrintPages));
		Sleep(50);


	}
	strTmp.Format(_T("rpt_tmp\\%s%04d.xml"), m_strPageHeader, m_nCurrentPreviewPage);
	CFile fd;
	if (fd.Open(strTmp, CFile::modeRead | CFile::typeBinary)) {
		fd.Close();
		LoadWorkForm(strTmp);
	}
	return 0;

}

INT CSIReportManager::InitWorkFormFile(CString strRpt, CString strRptAlias, CString rpt_pk)
{
	m_strRPTALIAS = strRptAlias;
	m_strRPTPK = rpt_pk;

	m_strOriginalWorkFormFileName = strRpt;
	LoadWorkForm(strRpt);
	return 0;
}

INT CSIReportManager::LoadWorkForm(CString strRpt)
{
	FreeWorkForm();
	m_pWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	if (!m_pWorkForm) return -1;

	INT ids = m_pWorkForm->LoadWorkForm(strRpt);
	INT t = m_pWorkForm->IncludeList.GetCount();
	INT tt = m_pWorkForm->WorkSheetList.GetCount();

	return 0;
}

INT CSIReportManager::FreeWorkForm(VOID)
{
	if (m_pWorkForm) {
		delete m_pWorkForm;
		m_pWorkForm = NULL;
	}
	return 0;
}

CSIReportObject_Table * CSIReportManager::FindTable(CSIReportWorkSheet *pSheet, CString name)
{
	INT count = pSheet->ObjList.GetCount();
	if (count == 0) return NULL;

	CSIReportObject * pObject;
	POSITION pos;
	INT i;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() == OBJECT_TABLE) {
			CSIReportObject_Table *pTable = (CSIReportObject_Table *)pObject;
			if (!pTable) continue;
			if (pTable->m_strName == name) return pTable;
		}
	}
	return NULL;
}

CSIReportObject * CSIReportManager::FindObject(CSIReportWorkSheet *pSheet, CString strName)
{
	INT count = pSheet->ObjList.GetCount();
	if (count == 0) return NULL;

	CSIReportObject * pObject;
	POSITION pos;
	INT i;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->m_strName == strName) return pObject;
	}
	return NULL;
}

BOOL CSIReportManager::HaveChild(CSIReportObject_Table *pTable)
{
	if (!pTable) return false;

	INT nRow = pTable->row_count;
	INT nCol = pTable->col_count;
	INT row, col;

	CSIReportObject_Table_Cell *pCell;
	for (row = 0; row < nRow; row++) {
		for (col = 0; col < nCol; col++) {
			pCell = pTable->GetCell(row, col);
			if (!pCell) continue;
			if (pCell->child_count > 0) return true;
		}
	}	
	return false;
}

BOOL CSIReportManager::IsChild(CSIReportObject_Table *pTable)
{
	if (!pTable) return false;
	if (pTable->m_strParentName.GetLength() > 0) 
		return true;
	else 
		return false;
}

double CSIReportManager::GetHeight(CSIReportObject_Table_Cell *pCell, CString str, INT &nLines)
{
//	pCell->TCHAR_cy = MulDiv(10, 96, 72);
	nLines = GetLines(pCell->TCHAR_max, pCell->TCHAR_len, str);
	if (nLines == 1) return (pCell->GetRect().bottom - pCell->GetRect().top);
		else return ((pCell->GetRect().bottom - pCell->GetRect().top) + (pCell->TCHAR_cy * (nLines-1)));
}

INT CSIReportManager::GetLines(INT char_max, INT char_len, CString str)
{
	INT length = str.GetLength();
	if (length == 0) return 1;			// 내용이 없을땐 한줄로 간주한다.

	INT nQuotient = length / char_len;
	INT nRemain = length % char_len;
	if (nRemain == 0) return nQuotient;
		else return nQuotient+1;
}

INT CSIReportManager::GetLinesByEnter(CString str)
{
	INT length = str.GetLength();
	if (length == 0) return 1;			// 내용이 없을땐 한줄로 간주한다.

	INT pos = -1;
	INT nRowCount = 1;
	while(true) {
		pos = str.Find('\n', pos+1);
		if (pos == -1) break;
		nRowCount++;
	}
	return nRowCount;
}

INT CSIReportManager::DrawLine(CDC *pDC, INT x1, INT y1, INT x2, INT y2)
{
	if (!pDC) return 0;
	pDC->MoveTo(x1, y1);
	pDC->LineTo(x2, y2);
	return 0;
}

INT CSIReportManager::DrawSlash(CDC *pDC, CRect rect, INT mode/* 1-\, 2-/ */)
{
	if (!pDC) return 0;
	if (mode == 1) /* \ */
	{
		DrawLine(pDC, rect.left, rect.top, rect.right, rect.bottom);
	}
	if (mode == 2) /* / */
	{
		DrawLine(pDC, rect.right, rect.top, rect.left, rect.bottom);
	}
	return 0;
}

INT CSIReportManager::DrawBox(CDC *pDC, INT left, INT top, INT right, INT bottom, BOOL bleft, BOOL btop, BOOL bright, BOOL bbottom)
{
	if (!pDC) return 0;

	if (bleft) {			// LEFT
		pDC->MoveTo(left, bottom);
		pDC->LineTo(left, top);
	}
	if (btop) {				// TOP
		pDC->MoveTo(left, top);
		pDC->LineTo(right, top);
	}
	if (bright)	{			// RIGHT
		pDC->MoveTo(right, top);
		pDC->LineTo(right, bottom);
	}
	if (bbottom) {			// BOTTOM
		pDC->MoveTo(right, bottom);
		pDC->LineTo(left, bottom);
	}
	return 0;
}

INT CSIReportManager::Print()
{
	CSIReportPreviewDlg dlg;
	dlg.m_pManager = this;

	m_nCurrentPreviewPage = -1;
	MakeXMLPageFiles();
	if (dlg.DoModal() != IDOK) return 0;
	return 0;
}

INT CSIReportManager::PreviewDocument(CDC *pDC, INT page)
{
	InitArray(m_fTotalSum, MAX_COLUMN);
	INT ids = PREVIEW_DrawWorkForm(pDC, page);

	return ids;
}

INT CSIReportManager::PREVIEW_DrawWorkForm(CDC *pDC, INT page, INT startpage)
{
	m_EventInfo.m_pSIReportManager = (CObject*)this;
	m_EventInfo.mode = 2;
	m_DMCursor = -1;		// 아직 데이타 사용안함

	CDC *pPageDC;
	INT pagecount = startpage;		// 첫페이지는 1 페이지라 한다.
	INT count = m_pWorkForm->IncludeList.GetCount();
	if (count == 0) return -1;
	CSIReportWorkSheet * pSheet;
	POSITION pos;
	pos = m_pWorkForm->IncludeList.GetHeadPosition();
	INT ids;
	CString strTmp;
	for(INT i = 0; i < count; i++) {
		pSheet = NULL;
		pSheet = (CSIReportWorkSheet*)m_pWorkForm->IncludeList.GetNext(pos);
		if (!pSheet) continue;
		m_nCurrentWorkSheet = i;

		// 리턴이 0이면 쉬트 인쇄 끝, 1 이면 다음페이지에 같은 쉬트 계속
		while(true) {
			if (pagecount == page) pPageDC = pDC;
				else pPageDC = NULL;
			m_EventInfo.pDC = pPageDC;
			// (START) - 현재 페이지 저장용 WorkForm생성
			FreePageWorkForm();
			m_pPageWorkForm = NULL;
			if (XML_MODE == 1) 
			{
				m_pPageWorkForm = MakeWorkFormClone(m_pWorkForm);		// 현재 페이지 워크폼 생성(페이지 저장용)
				CSIReportWorkSheet *ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
				strTmp.Format(_T("페이지 %04d"), pagecount);
				ptmpWorkSheet->Alias = strTmp;
			}
			// ( END ) - 현재 페이지 저장용 WorkForm생성
			ids = DrawWorkSheet(MANAGER_MODE_PREVIEW, pPageDC, pSheet);
			m_EventInfo.m_nPage = pagecount;
			if (m_pOwnerWnd && m_fpEventHandler) {
				( m_pOwnerWnd->*m_fpEventHandler )( &m_EventInfo );
			}
			// (START) - 현재 페이지 저장
			if (XML_MODE == 1) 
			{
				strTmp.Format(_T("rpt_tmp\\%s%04d.xml"), m_strPageHeader, pagecount);
				m_pXMLIOMgr->SaveToXML(m_pPageWorkForm, strTmp);
				strTmp.Format(_T("rpt_tmp\\%s%04d.rpt"), m_strPageHeader, pagecount);
				m_pXMLIOMgr->SaveToXML(m_pPageWorkForm, strTmp);
			}
			// ( END ) - 현재 페이지 저장
			if (ids != 1 && i < count-1) {
				pagecount++;
				break;
			}
			if (ids != 1) break;
			//if (pagecount == page) break;
			pagecount++;
		}
	}
	m_DMCursor = -1;
	return pagecount;
}

INT CSIReportManager::MAKEPAGE_DrawWorkForm(VOID)
{
	m_EventInfo.m_pSIReportManager = (CObject*)this;
	m_EventInfo.mode = MANAGER_MODE_PREVIEW;

	INT pagecount = 1;
	INT count = m_pWorkForm->IncludeList.GetCount();
	if (count == 0) return -1;
	CSIReportWorkSheet * pSheet;
	POSITION pos = m_pWorkForm->IncludeList.GetHeadPosition();
	if (pos == NULL) return -1;
	INT ids;
	CString strTmp;
	for(INT i = 0; i < count; i++) {
		pSheet = NULL;
		pSheet = (CSIReportWorkSheet*)m_pWorkForm->IncludeList.GetNext(pos);
		if (!pSheet) continue;
		m_nCurrentWorkSheet = i;
		m_EventInfo.m_nSheetNum = i;

		m_DMCursor = -1;
		// 리턴이 0이면 쉬트 인쇄 끝, 1 이면 다음페이지에 같은 쉬트 계속
		while(true) {
			m_EventInfo.pDC = NULL;
			m_EventInfo.m_nPage = pagecount;

			// (START) - 현재 페이지 저장용 WorkForm생성
			FreePageWorkForm();
			m_pPageWorkForm = MakeWorkFormClone(m_pWorkForm);		// 현재 페이지 워크폼 생성(페이지 저장용)
			CSIReportWorkSheet *ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
			strTmp.Format(_T("페이지 %04d"), pagecount);
			ptmpWorkSheet->Alias = strTmp;
			// ( END ) - 현재 페이지 저장용 WorkForm생성

			ids = PREVIEW_DrawWorkSheet(pSheet);
			if (m_pOwnerWnd && m_fpEventHandler) {
				( m_pOwnerWnd->*m_fpEventHandler )( &m_EventInfo );
			}

			// (START) - 현재 페이지 저장
			strTmp.Format(_T("rpt_tmp\\%s%04d.xml"), m_strPageHeader, pagecount);
			m_pXMLIOMgr->SaveToXML(m_pPageWorkForm, strTmp);
			strTmp.Format(_T("rpt_tmp\\%s%04d.rpt"), m_strPageHeader, pagecount);
			m_pXMLIOMgr->SaveToXML(m_pPageWorkForm, strTmp);
			// ( END ) - 현재 페이지 저장

			if (ids != 1 && i < count-1) {
				pagecount++;
				break;
			}
			if (ids != 1) break;
			pagecount++;
		}
	}
	return pagecount;
}

CSIReportWorkForm *CSIReportManager::MakeWorkFormClone(CSIReportWorkForm *pWorkFormSource)
{
	FreePageWorkForm();
	CSIReportWorkForm *pWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);
	if (!pWorkForm) return NULL;			// 메모리 할당 실패
	if (!pWorkFormSource) return NULL;		// 원본이 없음

	pWorkForm->alias = pWorkFormSource->alias;
	pWorkForm->paper = pWorkFormSource->paper;
	pWorkForm->sheet_height = pWorkFormSource->sheet_height;
	pWorkForm->sheet_size = pWorkFormSource->sheet_size;
	pWorkForm->VERSION = pWorkFormSource->VERSION;

	pWorkForm->AddNewSheet();
	CSIReportWorkSheet *ptmpWorkSheet = pWorkForm->GetWorkSheet(0);
	if (ptmpWorkSheet) {
		ptmpWorkSheet->Idx = 0;
		ptmpWorkSheet->master_sheet = true;
		pWorkForm->IncludeList.AddTail((CObject*)ptmpWorkSheet);
	}
	return pWorkForm;
}

INT CSIReportManager::DrawWorkSheet(INT mode, CDC *pDC, CSIReportWorkSheet *pSheet)
{
	INT count = pSheet->ObjList.GetCount();
	if (count == 0) return -1;

	InitArray(m_fPartSum, MAX_COLUMN);
	m_bPrintTotal = true;		// 총계를 찍어야 한다. 즉 아직 찍지 않았다.
	// Child가 아닌 객체중에서 Child가 없는 객체 부터 인쇄한다.
	CSIReportObject * pObject;
	POSITION pos;
	INT i;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() == OBJECT_TABLE) {
			CSIReportObject_Table *pTable = (CSIReportObject_Table *)pObject;
			if (pTable->total_sum) m_bPrintTotal = false;
			if (IsChild(pTable)) continue;
			DrawObject_Table(mode, pDC, pTable);
		}
		if (pObject->GetObjType() == OBJECT_BARCODE) {
			CSIReportObject_Barcode *pBarcode = (CSIReportObject_Barcode *)pObject;
			DrawObject_Barcode(mode, pDC, pBarcode);
		}
		if (pObject->GetObjType() == OBJECT_TEXT_BOX) {
			CSIReportObject_Text_Box *pText_Box = (CSIReportObject_Text_Box *)pObject;
			DrawObject_Text_Box(mode, pDC, pText_Box);
		}
		if (pObject->GetObjType() == OBJECT_LINE) {
			CSIReportObject_Line *pLine = (CSIReportObject_Line *)pObject;
			DrawObject_Line(mode, pDC, pLine);
		}
		if (pObject->GetObjType() == OBJECT_BITMAP) {
			CSIReportObject_Bitmap *pBitmap = (CSIReportObject_Bitmap *)pObject;
			DrawObject_Bitmap(mode, pDC, pBitmap);
		}
	}

	// 안그려진것은 Child를 포함한 Cell과 Child 뿐이다. 이제 Child를 인쇄하자.
	CSIReportObject_Table_Cell *pCell;
	CSIReportObject_Table_Cell *pTmpCell;				// temporary cell
	CSIReportObject_Table * pChild_Table;
	INT row, col, child;
	CString child_name;
	CRect tmpRect;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() != OBJECT_TABLE) continue;
		
		CSIReportObject_Table *pTable = (CSIReportObject_Table *)pObject;
		if (!HaveChild(pTable)) continue;
		for (row = 0; row < pTable->row_count; row++) {
			for (col = 0; col < pTable->col_count; col++) {
				pCell = pTable->GetCell(row, col);
				if (!pCell) continue;
				if (pCell->bind_status == 2) continue;
				if (pCell->child_count == 0) continue;
				for (child = 0; child < pCell->child_count; child++) {
					pChild_Table = FindTable(pSheet, pCell->child_name[child]);
					if (!pChild_Table) continue;
					// 이제 반복 인쇄를 시작하자.
					pTmpCell = pTable->GetCell(pCell->bind_row, pCell->bind_col);				
					tmpRect = pCell->GetRect();
					tmpRect.right = pTmpCell->GetRect().right;
					tmpRect.bottom = pTmpCell->GetRect().bottom;
					DrawObject_Table_Repeat(mode, pDC, pChild_Table, tmpRect);
				}
				tmpRect = pCell->GetRect();
				if (pCell->bind_status == 1) {
					pTmpCell = pTable->GetCell(pCell->bind_row, pCell->bind_col);				
					tmpRect.right = pTmpCell->GetRect().right;
					tmpRect.bottom = pTmpCell->GetRect().bottom;
				}
				DrawBox(pDC, DrawGetMMToPixel(mode, (double)tmpRect.left), DrawGetMMToPixel(mode, (double)tmpRect.top), DrawGetMMToPixel(mode, (double)tmpRect.right), DrawGetMMToPixel(mode, (double)tmpRect.bottom));
			}
		}	
	}
	if (pDC && IsSheetHaveRepeatObject() == FALSE) return 0;
	if (m_DMCursor < 0) return 0;				// Data를 아직 사용안하므로 Sheet는 다음페이지로 넘어가지 않음
	INT nRowCount = m_pDM[0]->GetRowCount();
	if (nRowCount-1 <= m_DMCursor) {
		if (!m_bPrintTotal) return 1;			// 총계를 인쇄 안하고 끝난 경우는 한페이지를 더찍어 총계를 찍어준다.
		return 0;		// Sheet 인쇄 끝
	} 
	else
	{
		return 1;								// Sheet 인쇄 남았음(다음 페이지로 넘김)
	}
}

INT CSIReportManager::PREVIEW_DrawWorkSheet(CSIReportWorkSheet *pSheet)
{
	INT count = pSheet->ObjList.GetCount();
	if (count == 0) return 0;

	CDC *pDC = NULL;
	INT mode = MANAGER_MODE_PREVIEW;

	InitArray(m_fPartSum, MAX_COLUMN);
	m_bPrintTotal = true;		// 총계를 찍어야 한다. 즉 아직 찍지 않았다.
	// Child가 아닌 객체중에서 Child가 없는 객체 부터 인쇄한다.
	CSIReportObject * pObject;
	POSITION pos;
	INT i;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() == OBJECT_TABLE) {
			CSIReportObject_Table *pTable = (CSIReportObject_Table *)pObject;
			if (pTable->total_sum) m_bPrintTotal = false;
			if (IsChild(pTable)) continue;
			DrawObject_Table(mode, pDC, pTable);
		}
		if (pObject->GetObjType() == OBJECT_BARCODE) {
			CSIReportObject_Barcode *pBarcode = (CSIReportObject_Barcode *)pObject;
			DrawObject_Barcode(mode, pDC, pBarcode);
		}
		if (pObject->GetObjType() == OBJECT_TEXT_BOX) {
			CSIReportObject_Text_Box *pText_Box = (CSIReportObject_Text_Box *)pObject;
			DrawObject_Text_Box(mode, pDC, pText_Box);
		}
		if (pObject->GetObjType() == OBJECT_LINE) {
			CSIReportObject_Line *pLine = (CSIReportObject_Line *)pObject;
			DrawObject_Line(mode, pDC, pLine);
		}
		if (pObject->GetObjType() == OBJECT_BITMAP) {
			CSIReportObject_Bitmap *pBitmap = (CSIReportObject_Bitmap *)pObject;
			DrawObject_Bitmap(mode, pDC, pBitmap);
		}
	}

	// 안그려진것은 Child를 포함한 Cell과 Child 뿐이다. 이제 Child를 인쇄하자.
	CSIReportObject_Table_Cell *pCell;
	CSIReportObject_Table_Cell *pTmpCell;				// temporary cell
	CSIReportObject_Table * pChild_Table;
	INT row, col, child;
	CString child_name;
	CRect tmpRect;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() != OBJECT_TABLE) continue;
		
		CSIReportObject_Table *pTable = (CSIReportObject_Table *)pObject;
		if (!HaveChild(pTable)) continue;
		for (row = 0; row < pTable->row_count; row++) {
			for (col = 0; col < pTable->col_count; col++) {
				pCell = pTable->GetCell(row, col);
				if (!pCell) continue;
				if (pCell->bind_status == 2) continue;
				if (pCell->child_count == 0) continue;
				for (child = 0; child < pCell->child_count; child++) {
					pChild_Table = FindTable(pSheet, pCell->child_name[child]);
					if (!pChild_Table) continue;
					// 이제 반복 인쇄를 시작하자.
					pTmpCell = pTable->GetCell(pCell->bind_row, pCell->bind_col);				
					tmpRect = pCell->GetRect();
					tmpRect.right = pTmpCell->GetRect().right;
					tmpRect.bottom = pTmpCell->GetRect().bottom;
					DrawObject_Table_Repeat(mode, pDC, pChild_Table, tmpRect);
				}
				tmpRect = pCell->GetRect();
				if (pCell->bind_status == 1) {
					pTmpCell = pTable->GetCell(pCell->bind_row, pCell->bind_col);				
					tmpRect.right = pTmpCell->GetRect().right;
					tmpRect.bottom = pTmpCell->GetRect().bottom;
				}
				DrawBox(pDC, DrawGetMMToPixel(mode, (double)tmpRect.left), DrawGetMMToPixel(mode, (double)tmpRect.top), DrawGetMMToPixel(mode, (double)tmpRect.right), DrawGetMMToPixel(mode, (double)tmpRect.bottom));
			}
		}	
	}
	if (pDC && IsSheetHaveRepeatObject() == FALSE) return 0;
	if (m_DMCursor < 0) return 0;				// Data를 아직 사용안하므로 Sheet는 다음페이지로 넘어가지 않음
	INT nRowCount = m_pDM[0]->GetRowCount();
	if (nRowCount-1 <= m_DMCursor) {
		if (!m_bPrintTotal) return 1;			// 총계를 인쇄 안하고 끝난 경우는 한페이지를 더찍어 총계를 찍어준다.
		return 0;		// Sheet 인쇄 끝
	} 
	else
	{
		return 1;								// Sheet 인쇄 남았음(다음 페이지로 넘김)
	}
}

INT CSIReportManager::DrawGetMMToPixel(INT mode, double mm)
{
	if (mode == MANAGER_MODE_PRINT) {
		mm = 297.0 * mm / 980.0;
		return ((INT)((double)(1440*0.03937) * mm));	
	} else {
		//mm = 297.0 * mm / 980.0;
		// return ((INT)((double)(96.0*0.03937) * mm));	
		return (INT)mm;
	}
}

INT CSIReportManager::DrawObject_Table(INT mode, CDC *pDC, CSIReportObject_Table * pTable)
{
	if (!pTable) return -2;
	INT nRow = pTable->row_count;
	INT nCol = pTable->col_count;
	INT row, col;
	BOOL left, top, right, bottom;
	CSIReportObject_Table_Cell *pCell;
	CSIReportObject_Table_Cell *pTmpCell;
	CRect real_cell_rect;

// (START) - XML Page Output - Copy Table
	if (m_pPageWorkForm) 
	{
		CSIReportWorkSheet *ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
		CSIReportObject_Table *ptmpObject = (CSIReportObject_Table *)ptmpWorkSheet->AddNewObject(OBJECT_TABLE);
		pTable->Clone(ptmpObject);
		for (row = 0; row < nRow; row++) {
			for (col = 0; col < nCol; col++) {
				pTmpCell = ptmpObject->GetCell(row, col);
				if (!pTmpCell) continue;
				pTmpCell->text.font.lfHeight = CalcNegativeFontHeight(pTmpCell->text.size);
			}
		}
	}
// ( END ) - XML Page Output - Copy Table

	//if (!pDC) return -1;

	COLORREF cBLACK = RGB(0, 0, 0);
	COLORREF cWHITE = RGB(255, 255, 255);

	for (row = 0; row < nRow; row++) {
		for (col = 0; col < nCol; col++) {
			pCell = pTable->GetCell(row, col);
			left = top = right = bottom = true;
			if (!pCell) continue;
			if (pCell->bind_status == 2) continue;
			if (pCell->bind_status == 0) {
				if (pCell->child_count > 0) {		// Child 가 있는 경우는 Skip 한다.
					;
				} else {
					CRect tmpRect = pCell->GetRect();
					CalcRectOffSet(tmpRect, pCell->indent_len, pCell->indent_len, -pCell->indent_len, -pCell->indent_len);					
					real_cell_rect = ConvertRectMMToPixel(mode, pCell->GetRect());
					if (pCell->fill_color != cWHITE) {
						if (pDC) {
							CBrush bk_brush;
							bk_brush.CreateSolidBrush(pCell->fill_color);
							pDC->FillRect(real_cell_rect, &bk_brush);
							pDC->SetBkColor(pCell->fill_color);
							pDC->SetBkMode(TRANSPARENT);
						}
					}
					real_cell_rect = ConvertRectMMToPixel(mode, tmpRect);
					//DrawTextToCell(mode, pDC, pCell, real_cell_rect);
					if (pCell->text.text == _T("%BARCODE바코드%")) {
						LOC_PrintBarcode(mode, pDC, real_cell_rect);
					} else {
						DrawTextToCell2(mode, pDC, pCell, real_cell_rect);
					}
					/*
					if (pTable->horz_line == FALSE && row == 0) top = false;
					if (pTable->horz_line == FALSE && row+1 == nRow) bottom = false;
					if (pTable->vert_line == FALSE && col == 0) left = false;
					if (pTable->vert_line == FALSE && col+1 == nCol) right = false;
					*/

					real_cell_rect = ConvertRectMMToPixel(mode, pCell->GetRect());
					DrawBox(pDC, real_cell_rect.left, real_cell_rect.top, real_cell_rect.right, real_cell_rect.bottom, pTable->vert_line, pTable->horz_line, pTable->vert_line, pTable->horz_line);
					DrawSlash(pDC, real_cell_rect, pCell->slash);
					if (pDC) pDC->SetBkColor(cWHITE);
				}
			}
			if (pCell->bind_status == 1) {
				if (pCell->child_count > 0) {		// Child 가 있는 경우는 Skip 한다.
					;
				} else {
					pTmpCell = pTable->GetCell(pCell->bind_row, pCell->bind_col);
					CRect tmpRect = pCell->GetRect();
					tmpRect.right = pTmpCell->GetRect().right;
					tmpRect.bottom = pTmpCell->GetRect().bottom;
					CRect tmpRect_real = tmpRect;
					CalcRectOffSet(tmpRect, pCell->indent_len, pCell->indent_len, -pCell->indent_len, -pCell->indent_len);					
					real_cell_rect = ConvertRectMMToPixel(mode, tmpRect);
					if (pCell->fill_color != cWHITE) {
						CBrush bk_brush;
						bk_brush.CreateSolidBrush(pCell->fill_color);
						pDC->FillRect(real_cell_rect, &bk_brush);
						pDC->SetBkColor(pCell->fill_color);
						pDC->SetBkMode(TRANSPARENT);
					}
					//DrawTextToCell(mode, pDC, pCell, real_cell_rect);
					if (pCell->text.text == _T("%BARCODE바코드%")) {
						LOC_PrintBarcode(mode, pDC, real_cell_rect);
					} else {
						DrawTextToCell2(mode, pDC, pCell, real_cell_rect);
					}
					DrawSlash(pDC, real_cell_rect, pCell->slash);
					//real_cell_rect = ConvertRectMMToPixel(mode, pCell->GetRect());
					real_cell_rect = ConvertRectMMToPixel(mode, tmpRect_real);
					DrawBox(pDC, real_cell_rect.left, real_cell_rect.top, real_cell_rect.right, real_cell_rect.bottom);
					if (pDC) pDC->SetBkColor(cWHITE);
				}
			}
		}
	}

	return 0;
}

INT CSIReportManager::DrawTextToCell(INT mode, CDC *pDC, CSIReportObject_Table_Cell *pCell, CRect rect)
{
	if (!pDC) return 0;
	if (!pCell) return 0;
	
	UINT nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	if (pCell->vert_align == ALIGN_TOP)			nFormat = DT_SINGLELINE|DT_TOP;
	if (pCell->vert_align == ALIGN_VCENTER)		nFormat = DT_SINGLELINE|DT_VCENTER;
	if (pCell->vert_align == ALIGN_BOTTOM)		nFormat = DT_SINGLELINE|DT_BOTTOM;

	if (pCell->horz_align == ALIGN_LEFT)		nFormat = nFormat|DT_LEFT;
	if (pCell->horz_align == ALIGN_HCENTER)		nFormat = nFormat|DT_CENTER;
	if (pCell->horz_align == ALIGN_RIGHT)		nFormat = nFormat|DT_RIGHT;

	CalcRectOffSet(rect, 2, 2, -2, -2);
	pCell->text.font.lfHeight = pCell->text.size;
	DrawTextWithFont(mode, pDC, pCell->text.text, rect, pCell->text.font, nFormat);
	return 0;
}

INT CSIReportManager::DrawTextToCell2(INT mode, CDC *pDC, CSIReportObject_Table_Cell *pCell, CRect rect)
{
	// Enter키 단위로 라인을 짤라서 출력한다.
	if (!pDC) return 0;
	if (!pCell) return 0;
	
	UINT nFormat = DT_VCENTER|DT_SINGLELINE;
	if (pCell->horz_align == ALIGN_LEFT)		nFormat = nFormat|DT_LEFT;
	if (pCell->horz_align == ALIGN_HCENTER)		nFormat = nFormat|DT_CENTER;
	if (pCell->horz_align == ALIGN_RIGHT)		nFormat = nFormat|DT_RIGHT;

	CalcRectOffSet(rect, pCell->indent_len, pCell->indent_len, -pCell->indent_len, -pCell->indent_len);
	pCell->text.font.lfHeight = pCell->text.size;

	INT nLines = GetLinesByEnter(pCell->text.text);
	INT Height;
	if (mode == MANAGER_MODE_PRINT) {		// 인쇄모드
		Height = MulDiv(pCell->text.size, 1440, 72);
	} else {								// Preview 모드
		Height = MulDiv(pCell->text.size, 96, 72);
	}
	if (pCell->vert_align == ALIGN_VCENTER) {
		rect.top = rect.top + (((rect.bottom - rect.top) - (Height*nLines)) / 2);
	}
	if (pCell->vert_align == ALIGN_BOTTOM) {
		rect.top = rect.top + (((rect.bottom - rect.top) - (Height*nLines)));
	}

	//DrawTextWithFont(mode, pDC, pCell->text.text, rect, pCell->text.font, nFormat);
	DrawTextWithFont3(mode, pDC, pCell->text.text, rect, pCell->text.font, nFormat);
	return 0;
}

INT CSIReportManager::DrawObject_Table_Repeat(INT mode, CDC *pDC, CSIReportObject_Table * pTable, CRect rect)
{
	if (!pTable) return -2;

	double startY = (double)rect.top;								// 단위 Pixel(에디트 좌표계 A4 : 980 X 693)
	double curY = 0;
	double height, max_height;										// 단위 mm
	INT nLines, max_line;											// Line의 Column들중에서 가장 많은 라인
//	INT nPixel_cy, nOffset_top;
	INT remain_rect_height;
	INT nCol = pTable->col_count;
	INT cursor;
	INT col;
	INT nDMRow = m_pDM[0]->GetRowCount();
	INT nRectTop = rect.bottom - pTable->GetRect().Height();
	INT nIsLast = false;											// 데이타를 출력하는 마지막 페이지인가?
	CString str;
	CRect origin_rect = rect;
	CRect cell_rect;
	CRect RectNoMoreStatement;

	if (m_DMCursor < 0) m_DMCursor = 0;
	if (pTable->part_sum) rect.bottom -= pTable->GetRect().Height();

	// (START) - XML Page Output
	CSIReportWorkSheet *ptmpWorkSheet = NULL;
	CSIReportObject_Table *ptmpObject = NULL;
	CSIReportObject_Table_Cell *pCopyCell;
	INT nCurRow = 0;
	if (m_pPageWorkForm) {
		ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
		ptmpObject = (CSIReportObject_Table *)ptmpWorkSheet->AddNewObject(OBJECT_TABLE);
		ptmpObject->row_count = 0;
		ptmpObject->col_count = nCol;
	}
	// ( END ) - XML Page Output

	InitArray(m_fPartSum, MAX_COLUMN);
	CRect real_cell_rect, real_text_cell_rect;;
	CSIReportObject_Table_Cell *pCell;
	for (cursor = m_DMCursor; cursor < nDMRow; cursor++) {
		max_height = 0;
		max_line = 1;
		for (col = 0; col < nCol; col++) {
			str = _T("");
			pCell = pTable->GetCell(0, col);
			if (!pCell) continue;
			if (pCell->bind_status == 2) continue;
			cell_rect = pCell->GetRect();
			//m_pDM[0]->GetCellData(m_DMCursor, col, str);
			if (pCell->data_mgr_name.GetLength() > 0) {
				if (pCell->data_mgr_name.Find(_T("%")) == -1) {
					m_pDM[0]->GetCellData(pCell->data_field, m_DMCursor, str);
				} else {
					DM_GetCellData(pCell->data_mgr_name, m_DMCursor, str);
				}
			} else {
				m_pDM[0]->GetCellData(pCell->data_field, m_DMCursor, str);
			}
			pCell->TCHAR_cy = DrawGetchar_cy(mode, pDC, pCell->text.size, false);
			height = GetHeight(pCell, str, nLines);
			if ( height > max_height) 
			{
				max_line = nLines;
				max_height = height;
			}
		}
		remain_rect_height = rect.bottom - rect.top - (INT)(curY);
		if (max_height > remain_rect_height) {
			break;
		}
		curY += max_height;
	// (START) - XML Page Output
		if (ptmpObject) ptmpObject->AddRow();
	// ( END ) - XML Page Output
		for (col = 0; col < nCol; col++) 
		{
			str = _T("");
			pCell = pTable->GetCell(0, col);
			if (!pCell) continue;
			if (pCell->bind_status == 2) continue;
			cell_rect = pCell->GetRect();
			cell_rect.top = (INT)(startY);
			cell_rect.bottom = (INT)(cell_rect.top + max_height);

			//m_pDM[0]->GetCellData(m_DMCursor, col, str);
			m_pDM[0]->GetCellData(pCell->data_field, m_DMCursor, str);
			if (pCell->data_mgr_name.GetLength() > 0) {
				if (pCell->data_mgr_name.Find(_T("%")) == -1) {
					m_pDM[0]->GetCellData(pCell->data_field, m_DMCursor, str);
				} else {
					DM_GetCellData(pCell->data_mgr_name, m_DMCursor, str);
				}
			} else {
				m_pDM[0]->GetCellData(pCell->data_field, m_DMCursor, str);
			}
			// (START) - XML Page Output
			if (ptmpObject) {
				ptmpObject->SetCellData(nCurRow, col, pCell);
				pCopyCell = ptmpObject->GetCell(nCurRow, col);
				pCopyCell->text.font.lfHeight = CalcNegativeFontHeight(pCopyCell->text.size);
				pCopyCell->SetRect(cell_rect);
				pCopyCell->text.text = SplitStringWithLen(str, pCell->TCHAR_len, pCell->TCHAR_max);
			}
			// ( END ) - XML Page Output
			if (pCell->part_sum == 1 /*정수*/) m_fPartSum[col] += _ttoi(str.GetBuffer(0));
			if (pCell->part_sum == 2 /*실수*/) m_fPartSum[col] += atof(str.GetBuffer(0));
			if (pCell->total_sum == 1 /*정수*/) m_fTotalSum[col] += _ttoi(str.GetBuffer(0));
			if (pCell->total_sum == 2 /*실수*/) m_fTotalSum[col] += atof(str.GetBuffer(0));			
		}
		startY += max_height;
		m_DMCursor++;
		nCurRow++;
		if (pCell->GetRect().bottom - pCell->GetRect().top > remain_rect_height) return 0;
	}
	for (col = 0; col < nCol; col++) 
	{	// 이제 왼쪽 오른쪽 선을 긋는다.
		pCell = pTable->GetCell(0, col);
		if (!pCell) continue;
		if (pCell->bind_status == 2) continue;
		cell_rect = pCell->GetRect();
		cell_rect.top = rect.top;
		if (m_DMCursor == nDMRow) 
		{
			cell_rect.bottom = (INT)(startY);
			INT a = rect.bottom - cell_rect.bottom;
			INT space = (rect.bottom - cell_rect.bottom);
			if (pTable->part_sum) space -= pTable->GetRect().Height();
			if (pTable->total_sum) space -= pTable->GetRect().Height();
			if ((double)(space) > (m_nNoMoreStatement * 980.0 / 297.0)) 
			{
				real_cell_rect = rect;
				real_cell_rect.top = cell_rect.bottom;
				nRectTop = real_cell_rect.top;
				nIsLast = true;
				if (pTable->part_sum) real_cell_rect.top += pTable->GetRect().Height();
				if (pTable->total_sum) real_cell_rect.top += pTable->GetRect().Height();
				real_cell_rect.bottom = origin_rect.bottom;
				// 이하여백 출력
				// (START) - XML Page Output
				CRect rTemp = real_cell_rect;
				CalcRectOffSet(rTemp, 2, 2, -2, -2);
				if (m_pPageWorkForm) AddCopyTextWithFont(m_strNoMoreStatement, rTemp, m_fNoMoreStatement, ALIGN_VCENTER, ALIGN_HCENTER);
				// ( END ) - XML Page Output
				break;
			} else {
				if (!pTable->part_sum && !m_bPrintTotal) {
					cell_rect.bottom = rect.bottom - pTable->GetRect().Height();
				} else {
					cell_rect.bottom = (INT)startY;
				}
				if (nRectTop + pTable->GetRect().Height() > origin_rect.bottom) {
					cell_rect.bottom = rect.bottom;
				} else {
					cell_rect.bottom = (INT)startY;
				}
				nRectTop = (INT)startY;//origin_rect.bottom - (pTable->GetRect().Height()*2);
				nIsLast = true;
				break;				
			}
		} 
		else 
		{
			cell_rect.bottom = origin_rect.bottom;
		}
	}
	CString strArray[MAX_COLUMN];
	INT nBottom;
	if (pTable->part_sum) {
		for (col = 0; col < nCol; col++) {
			if ((pCell = pTable->GetCell(0, col)) == NULL) continue;
			strArray[col] = _T("");
			if (pCell->part_sum == 1 /*정수*/) strArray[col].Format(_T("%d"), (INT)m_fPartSum[col]);
			if (pCell->part_sum == 2 /*실수*/) strArray[col].Format(_T("%g"), m_fPartSum[col]);
		}
		strArray[m_nColPartSum] = m_strPartSum;
		// (START) - XML Page Output
		if (ptmpObject) {
			CopyTableRow(ptmpObject, nCurRow-1, nRectTop);
			ptmpObject->AddRow();
			CopyTableRow(pTable, 0, ptmpObject, nCurRow, nRectTop, strArray);
		}
		nCurRow++;
		// ( END ) - XML Page Output
		nBottom = -1;
		if (!pTable->total_sum && origin_rect.bottom - (nRectTop+pTable->GetRect().Height()) < 5) nBottom = origin_rect.bottom;	
		nRectTop += pTable->GetRect().Height();
	}
	if (nRectTop + pTable->GetRect().Height() > origin_rect.bottom) return 0;
	if (pTable->total_sum && nIsLast) {
		for (col = 0; col < nCol; col++) {
			if ((pCell = pTable->GetCell(0, col)) == NULL) continue;
			strArray[col] = _T("");
			if (pCell->total_sum == 1 /*정수*/) strArray[col].Format(_T("%d"), (INT)m_fTotalSum[col]);
			if (pCell->total_sum == 2 /*실수*/) strArray[col].Format(_T("%g"), m_fTotalSum[col]);
		}
		strArray[m_nColTotalSum] = m_strTotalSum;
		// (START) - XML Page Output
		if (ptmpObject) {
			CopyTableRow(ptmpObject, nCurRow-1, nRectTop);
			ptmpObject->AddRow();
			CopyTableRow(pTable, 0, ptmpObject, nCurRow, nRectTop, strArray);
		}
		nCurRow++;
		// ( END ) - XML Page Output
		nBottom = -1;
		if (origin_rect.bottom - (nRectTop+pTable->GetRect().Height()) < 5) nBottom = origin_rect.bottom;	
		m_bPrintTotal = true;
	}

	return 0;
}

INT CSIReportManager::CopyTableRow(CSIReportObject_Table * pTarget, INT nTargetRow, INT nRectBottom)
{
	if (nTargetRow < 0) return 0;

	CSIReportObject_Table_Cell *pCopyCell;
	CRect cell_rect;
	INT nCol = pTarget->col_count;
	for (INT col = 0; col < nCol; col++) {
		pCopyCell = pTarget->GetCell(nTargetRow, col);
		if (pCopyCell) {
			cell_rect = pCopyCell->GetRect();
			cell_rect.bottom = nRectBottom;
			pCopyCell->SetRect(cell_rect);	
		}
	}

	return 0;
}

INT CSIReportManager::CopyTableRow(CSIReportObject_Table * pSource, INT nSourceRow, CSIReportObject_Table * pTarget, INT nTargetRow, INT nRectTop, CString strArray[])
{
	INT nCol = pSource->col_count;
	CSIReportObject_Table_Cell *pCell;
	CSIReportObject_Table_Cell *pCopyCell;
	CRect cell_rect;

	for (INT col = 0; col < nCol; col++) {
		pCell = pSource->GetCell(nSourceRow, col);
		cell_rect = pCell->GetRect();
		cell_rect.top = nRectTop;
		cell_rect.bottom = nRectTop + pSource->GetRect().Height();
		pTarget->SetCellData(nTargetRow, col, pCell);
		pCopyCell = pTarget->GetCell(nTargetRow, col);
		pCopyCell->text.font.lfHeight = CalcNegativeFontHeight(pCopyCell->text.size);
		pCopyCell->SetRect(cell_rect);
		pCopyCell->text.text = strArray[col];
	}
	return 0;
}

INT CSIReportManager::CalcNegativeFontHeight(INT size)
{
	INT nTmp;
	nTmp = MulDiv(size, 96, 72);
	nTmp = (nTmp > 0) ? -nTmp : nTmp;	

	return nTmp;
}

INT CSIReportManager::AddCopyTextWithFont(CString str, CRect rect, LOGFONT logfont, INT vert_align, INT horz_align)
{
	CSIReportWorkSheet *ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
	CSIReportObject_Text_Box *ptmpObject = (CSIReportObject_Text_Box *)ptmpWorkSheet->AddNewObject(OBJECT_TEXT_BOX);
	ptmpObject->TCHAR_len = 100;
	ptmpObject->TCHAR_max = 100;
	ptmpObject->SetRect(rect);
	ptmpObject->text.color = RGB(0, 0, 0);
	ptmpObject->text.font = logfont;
	ptmpObject->text.size = logfont.lfHeight;
	ptmpObject->text.font.lfHeight = CalcNegativeFontHeight(ptmpObject->text.size);
	ptmpObject->text.style = 0;		// 아직 의미 없음
	ptmpObject->text.text = str;
	ptmpObject->horz_line = false;
	ptmpObject->vert_line = false;
	ptmpObject->vert_align = vert_align;
	ptmpObject->horz_align = horz_align;
	
	return 0;
}

INT CSIReportManager::DrawObject_Table_TotalLine(INT mode, CDC *pDC, CSIReportObject_Table * pTable, INT nRectTop, INT nCol, INT nIsLast, CString strTotal[], INT nBottom)
{
	INT col;				// Column index variable
	CSIReportObject_Table_Cell *pCell;
	CRect cell_rect;
	CRect real_cell_rect;
	CString strTmp;

	for (col = 0; col < nCol; col++) {
		pCell = pTable->GetCell(0, col);
		if (!pCell) continue;
		if (pCell->bind_status == 2) continue;
		cell_rect = pCell->GetRect();
		cell_rect.top = nRectTop;
		if (nBottom < 0) 
		{
			cell_rect.bottom = nRectTop + pTable->GetRect().Height();
		}
		else 
		{
			cell_rect.bottom = nBottom;
		}
		CRect tmpRect = cell_rect;
		CalcRectOffSet(tmpRect, pCell->indent_len, pCell->indent_len, -pCell->indent_len, -pCell->indent_len);					
		real_cell_rect = ConvertRectMMToPixel(mode, cell_rect);
		if (!nIsLast)				// 마지막 페이지가 아닐때
		{
			DrawBox(pDC, real_cell_rect.left, real_cell_rect.top, real_cell_rect.right, real_cell_rect.bottom, false, true, true, false);
		} 
		else
		{
			DrawBox(pDC, real_cell_rect.left, real_cell_rect.top, real_cell_rect.right, real_cell_rect.bottom, false, true, true, true);
		}
		real_cell_rect = ConvertRectMMToPixel(mode, tmpRect);
		strTmp = pCell->text.text;
		pCell->text.text = strTotal[col];
		DrawTextToCell(mode, pDC, pCell, real_cell_rect);
		pCell->text.text = strTmp;
	}

	return 0;
}

BOOL CSIReportManager::IsCanCut(CString sStr, INT nPos)
{
	INT i, nLastPos = -1;
	//글자수 초과면 FALSE
	if(nPos > sStr.GetLength())
		return FALSE;
	TCHAR ch = sStr.GetAt(nPos);
	//마지막 글자 || 한글이 아니면 TRUE
	if(nPos + 1 == sStr.GetLength() || (ch & (0x80)) != 128)
		return TRUE;
	//첫글자가 1byte문자면 TRUE 아니면 FALSE
	if(nPos == 0)
		if((ch & (0x80)) != 128)
			return TRUE;
		else
			return FALSE;

	for(i = 0; i < nPos; i++) {
		ch = sStr.GetAt(i);
		//자르는 부분에서 가장 가까운(앞쪽으로)영문의 위치
		if((ch & (0x80)) != 128)
			nLastPos = i;
	}
	if (nLastPos == -1 && nPos % 2 == 1) return FALSE;
	//짝수개 차이나면 TRUE
	return !((nPos - nLastPos) % 2);
}

INT CSIReportManager::SetTableCellText(CString tbl_name, INT rowidx, INT colidx, CString str, INT vert_align, INT horz_align)
{
	CSIReportWorkSheet * pSheet = NULL;
	POSITION pos = m_pWorkForm->WorkSheetList.FindIndex(m_nCurrentWorkSheet);
	pSheet = (CSIReportWorkSheet*)m_pWorkForm->WorkSheetList.GetAt(pos);
	if (!pSheet) return -1;

	CSIReportObject_Table *pTable = FindTable(pSheet, tbl_name);
	if (!pTable) return -2;

	CSIReportObject_Table_Cell *pCell = pTable->GetCell(rowidx, colidx);
	if (!pCell) return -3;

	UINT nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	if (vert_align == ALIGN_TOP)		nFormat = DT_SINGLELINE|DT_TOP;
	if (vert_align == ALIGN_VCENTER)	nFormat = DT_SINGLELINE|DT_VCENTER;
	if (vert_align == ALIGN_BOTTOM)		nFormat = DT_SINGLELINE|DT_BOTTOM;

	if (horz_align == ALIGN_LEFT)		nFormat = nFormat|DT_LEFT;
	if (horz_align == ALIGN_HCENTER)	nFormat = nFormat|DT_CENTER;
	if (horz_align == ALIGN_RIGHT)		nFormat = nFormat|DT_RIGHT;

	CRect real_cell_rect;
	real_cell_rect = ConvertRectMMToPixel(m_EventInfo.mode, pCell->GetRect());
	pCell->text.font.lfHeight = pCell->text.size;
	DrawTextWithFont(m_EventInfo.mode, m_EventInfo.pDC, str, real_cell_rect, pCell->text.font, nFormat);    

// (START) - TEST
	if (m_pPageWorkForm) {
		CSIReportWorkSheet *ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
		CSIReportObject_Text_Box *ptmpObject = (CSIReportObject_Text_Box *)ptmpWorkSheet->AddNewObject(OBJECT_TEXT_BOX);
		ptmpObject->TCHAR_len = 100;
		ptmpObject->TCHAR_max = 100;
		ptmpObject->SetRect(pCell->GetRect());
		ptmpObject->text.color = pCell->text.color;
		ptmpObject->text.font = pCell->text.font;
		ptmpObject->text.size = pCell->text.size;
		ptmpObject->text.font.lfHeight = CalcNegativeFontHeight(ptmpObject->text.size);
		ptmpObject->text.style = pCell->text.style;
		ptmpObject->text.text = str;
		ptmpObject->horz_line = false;
		ptmpObject->vert_line = false;
		ptmpObject->vert_align = vert_align;
		ptmpObject->horz_align = horz_align;
	}
// ( END ) - TEST

	return 0;
}

INT CSIReportManager::GetWorkFormPaper(VOID)
{
	if (m_pWorkForm) return m_pWorkForm->paper;
		else return -1;
}

INT CSIReportManager::CalcRectOffSet(CRect &rect, INT left, INT top, INT right, INT bottom)
{
	rect.left += left;
	rect.top += top;
	rect.right += right;
	rect.bottom += bottom;
	return 0;
}

CRect CSIReportManager::ConvertRectMMToPixel(INT mode, CRect rect)
{
	CRect result;
	result.left = DrawGetMMToPixel(mode, (double)rect.left);
	result.top = DrawGetMMToPixel(mode, (double)rect.top);
	result.right = DrawGetMMToPixel(mode, (double)rect.right);
	result.bottom = DrawGetMMToPixel(mode, (double)rect.bottom);
	return result;
}

INT CSIReportManager::DrawObject_Text_Box(INT mode, CDC *pDC, CSIReportObject_Text_Box * pTextBox)
{
	if (pTextBox == NULL) return -1;
	if (pTextBox->m_strName == _T("%BARCODE바코드제목%")) return 0;
	if (pTextBox->m_strName == _T("%BARCODE바코드값%")) return 0;

// (START) - XML Page Output - Copy Table
	if (m_pPageWorkForm) 
	{
		CSIReportWorkSheet *ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
		CSIReportObject_Text_Box *ptmpObject = (CSIReportObject_Text_Box *)ptmpWorkSheet->AddNewObject(OBJECT_TEXT_BOX);
		pTextBox->Clone(ptmpObject);

		INT nTmp = -MulDiv(ptmpObject->text.size, 96, 72);
		nTmp = (nTmp > 0) ? -nTmp : nTmp;
		ptmpObject->text.font.lfHeight = nTmp;
	}
// ( END ) - XML Page Output - Copy Table

	INT nLines = 0;
	CString str, token;
	str = pTextBox->text.text;
	INT pos = 0;
	while(true) {
		if (str.GetLength() == 0) break;
		pos = str.Find('\n', 0);
		if (pos == -1) pos = str.GetLength();
		token = str.Left(pos);
		token.TrimLeft();				token.TrimRight();
		nLines++;
		str = str.Mid(pos);
		str.TrimLeft();			str.TrimRight();
	}
	
	pTextBox->text.font.lfHeight = pTextBox->text.size;
	CRect rect = pTextBox->GetRect();
	CalcRectOffSet(rect, pTextBox->indent_len, pTextBox->indent_len, -pTextBox->indent_len, -pTextBox->indent_len);
	CRect real_text_cell_rect = ConvertRectMMToPixel(mode, rect);
	CRect real_cell_rect = ConvertRectMMToPixel(mode, rect);
	INT nOffset_top = 3;			// Text Box top offset - offset가 0이면 상자의 위쪽 라인과 글씨가 겹쳐진다.
	INT nPixel_cy;
	if (mode == MANAGER_MODE_PRINT) {		// 인쇄모드
		nPixel_cy = MulDiv(pTextBox->text.font.lfHeight, 1440, 72);
	} else {				// Preview 모드
		if (pDC) nPixel_cy = MulDiv(pTextBox->text.font.lfHeight, GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), 72);
			else nPixel_cy = MulDiv(pTextBox->text.font.lfHeight, 96, 72);
	}
	INT char_cy = nPixel_cy;
	nPixel_cy = (nPixel_cy < 0) ? nPixel_cy * -1 : nPixel_cy;
	nPixel_cy *= nLines;
	if (pTextBox->vert_align == ALIGN_TOP)			nOffset_top = 3;
	if (pTextBox->vert_align == ALIGN_VCENTER)		nOffset_top = (real_text_cell_rect.Height() - nPixel_cy) / 2;
	if (pTextBox->vert_align == ALIGN_BOTTOM)		nOffset_top = (real_text_cell_rect.Height() - nPixel_cy) - 3;
	
	real_text_cell_rect.top += nOffset_top;
	//CalcRectOffSet(real_text_cell_rect, 3, nOffset_top, -3, 0);

	UINT nFormat = DT_VCENTER|DT_LEFT|DT_SINGLELINE;
	if (pTextBox->horz_align == ALIGN_LEFT)		nFormat = DT_VCENTER|DT_SINGLELINE|DT_LEFT;
	if (pTextBox->horz_align == ALIGN_HCENTER)		nFormat = DT_VCENTER|DT_SINGLELINE|DT_CENTER;
	if (pTextBox->horz_align == ALIGN_RIGHT)		nFormat = DT_VCENTER|DT_SINGLELINE|DT_RIGHT;

	DrawTextWithFont3(mode, pDC, GetKeywordString(pTextBox->text.text), real_text_cell_rect, pTextBox->text.font, nFormat);
	DrawBox(pDC, real_cell_rect.left, real_cell_rect.top, real_cell_rect.right, real_cell_rect.bottom, pTextBox->vert_line, pTextBox->horz_line, pTextBox->vert_line, pTextBox->horz_line);
	return 0;
}

INT CSIReportManager::DrawObject_Line(INT mode, CDC *pDC, CSIReportObject_Line * pLine)
{
	if (!pLine) return -2;

	// (START) - XML Page Output - Copy Line
	if (m_pPageWorkForm) {
		CSIReportWorkSheet *ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
		CSIReportObject_Line *ptmpObject = (CSIReportObject_Line *)ptmpWorkSheet->AddNewObject(OBJECT_LINE);
		pLine->Clone(ptmpObject);
	}
	// ( END ) - XML Page Output - Copy Line
	if (!pDC) return -1;

	CRect rect = ConvertRectMMToPixel(mode, pLine->GetRect());
	
    CPen   pen;
	// 스타일(0:SOLID, 1:Dash, 2:Dot, 3:DashDot, 4:DashDotDot)
	INT style = PS_SOLID;
	if (pLine->style == 0) style = PS_SOLID;
	if (pLine->style == 1) style = PS_DASH;
	if (pLine->style == 2) style = PS_DOT;
	if (pLine->style == 3) style = PS_DASHDOT;
	if (pLine->style == 4) style = PS_DASHDOTDOT;

	pen.CreatePen(style, pLine->thick, RGB(0, 0, 0));
    CPen*   pOldPen   = (CPen*)   pDC->SelectObject(&pen);

    pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.right, rect.bottom);

    pDC->SelectObject(pOldPen);
    pen.DeleteObject();

	return 0;
}

INT CSIReportManager::DrawObject_Barcode(INT mode, CDC *pDC, CSIReportObject_Barcode * pBarcode)
{
	if (!pBarcode) return -2;
	if (pBarcode->m_strName == _T("%BARCODE바코드%")) return 0;

	// (START) - XML Page Output - Copy Barcode
	if (m_pPageWorkForm) {
		CSIReportWorkSheet *ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
		CSIReportObject_Barcode *ptmpObject = (CSIReportObject_Barcode *)ptmpWorkSheet->AddNewObject(OBJECT_BARCODE);
		pBarcode->Clone(ptmpObject);
	}
	// ( END ) - XML Page Output - Copy Barcode
	if (!pDC) return -1;

	CRect rect = ConvertRectMMToPixel(mode, pBarcode->GetRect());
	pBarcode->SetStyle(pBarcode->m_nStyle);
	pBarcode->SetCaption(pBarcode->m_strCaption);
	pBarcode->Draw(pDC, rect);
	return 0;
}

INT CSIReportManager::DrawObject_Bitmap(INT mode, CDC *pDC, CSIReportObject_Bitmap * pBitmap)
{
	if (!pBitmap) return 0;

	// (START) - XML Page Output - Copy Bitmap
	if (m_pPageWorkForm) {
		CSIReportWorkSheet *ptmpWorkSheet = m_pPageWorkForm->GetWorkSheet(0);
		CSIReportObject_Bitmap *ptmpObject = (CSIReportObject_Bitmap *)ptmpWorkSheet->AddNewObject(OBJECT_BITMAP);
		pBitmap->Clone(ptmpObject);
	}
	// ( END ) - XML Page Output - Copy Bitmap
	if (!pDC) return 0;

	CRect real_rect;
	if (mode == MANAGER_MODE_PRINT) {		// 인쇄모드일때 사이즈 계산
		real_rect = ConvertRectMMToPixel(mode, pBitmap->GetRect());
	} else {								// 미리보기 모드일때의 사이즈 계산
		real_rect = ConvertRectMMToPixel(mode, pBitmap->GetRect());
	}
	
	if (pBitmap->LoadBitmap() >= 0) {
		pBitmap->m_pBitmap->DrawDIB2(pDC, real_rect.left, real_rect.top, real_rect.Width(), real_rect.Height());
	}

	return 0;
}

INT CSIReportManager::DrawGetchar_cy(INT mode, CDC *pDC, INT nFontHeight, INT flag)
{
	INT result = 0;
	if (mode == MANAGER_MODE_PRINT) 
	{		// 인쇄모드
		if (flag == (INT)true) result = MulDiv(nFontHeight, 1440, 72);
			else result = MulDiv(nFontHeight, 96, 72);
	} 
	else 
	{				// Preview 모드
		if (pDC) result = MulDiv(nFontHeight, GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), 72);
			else result = MulDiv(nFontHeight, 96, 72);
	}
	return result;
}

INT CSIReportManager::DrawGetHorzFormat(INT horz_align)
{
	UINT nFormat = DT_VCENTER|DT_LEFT|DT_SINGLELINE;
	if (horz_align == ALIGN_LEFT)		nFormat = DT_VCENTER|DT_SINGLELINE|DT_LEFT;
	if (horz_align == ALIGN_HCENTER)		nFormat = DT_VCENTER|DT_SINGLELINE|DT_CENTER;
	if (horz_align == ALIGN_RIGHT)		nFormat = DT_VCENTER|DT_SINGLELINE|DT_RIGHT;	
	
	return nFormat;
}

INT CSIReportManager::InitArray(double array[], INT nColumn)
{
	for (INT i = 0; i < nColumn; i++) {
		array[i] = 0.0;
	}
	return 0;
}

INT CSIReportManager::SetBarcodeCaption(CString obj_name, CString strCaption)
{
	CSIReportWorkSheet * pSheet = NULL;
	POSITION pos = m_pWorkForm->WorkSheetList.FindIndex(m_nCurrentWorkSheet);
	pSheet = (CSIReportWorkSheet*)m_pWorkForm->WorkSheetList.GetAt(pos);
	if (!pSheet) return -1;

	CSIReportObject_Barcode *pBarcode = FindBarcode(pSheet, obj_name);
	if (!pBarcode) return -2;

	pBarcode->m_strCaption = strCaption;
	return 0;
}

CSIReportObject_Barcode * CSIReportManager::FindBarcode(CSIReportWorkSheet *pSheet, CString obj_name)
{
	INT count = pSheet->ObjList.GetCount();
	if (count == 0) return NULL;

	CSIReportObject * pObject;
	POSITION pos;
	INT i;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() == OBJECT_BARCODE) {
			CSIReportObject_Barcode *pBarcode = (CSIReportObject_Barcode *)pObject;
			if (!pBarcode) continue;
			if (pBarcode->m_strName == obj_name) return pBarcode;
		}
	}
	return NULL;
}

INT CSIReportManager::MakeXMLPageFiles(VOID)
{
	// 기존의 파일들을 모두 지운다.
	CString strTmp;
	CFile fd;
	INT i;
	for (i = 1; i < 99999; i++) {
		strTmp.Format(_T("rpt_tmp\\%s%04d.rpt"), m_strPageHeader, i);
		if (fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false) break;
		fd.Close();
		DeleteFile(strTmp);

	}
	for (i = 1; i < 99999; i++) {
		strTmp.Format(_T("rpt_tmp\\%s%04d.xml"), m_strPageHeader, i);
		if (fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false) break;
		fd.Close();
		DeleteFile(strTmp);

	}
	MAKEPAGE_DrawWorkForm();
	m_EventInfo.m_nTotalPage = GetTmpPageNum();
	return 0;
}

INT CSIReportManager::GetTmpPageNum(VOID)
{
	CFile fd;
	INT nPages = 0;
	CString strTmp;
	for (INT i = 1; i < 99999; i++) {
		strTmp.Format(_T("rpt_tmp\\%s%04d.xml"), m_strPageHeader, i);
		if (fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false) break;
		fd.Close();
		nPages = i;
	}
	return nPages;
}

INT CSIReportManager::ShowXMLFilePage(INT mode, CDC *pDC, INT nStartPage, INT nFinishPage)
{
	if (mode == MANAGER_MODE_PRINT) {

	} else {
		if (m_nCurrentPreviewPage != nStartPage) {
			LoadPageXMLWorkForm(nStartPage);
		}
		PREVIEW_DrawWorkForm(pDC,m_nCurrentPreviewPage, m_nCurrentPreviewPage);
	}
	return 0;
}


INT CSIReportManager::LoadPageXMLWorkForm(INT nPage)
{
	CString strTmp;
	strTmp.Format(_T("rpt_tmp\\%s%04d.xml"), m_strPageHeader, nPage);
	CFile fd;
	if (fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false) {
		AfxMessageBox(_T("페이지 정보를 찾을 수 없습니다.!"));
		return -1;
	}
	fd.Close();
	LoadWorkForm(strTmp);
	m_nCurrentPreviewPage = nPage;
	return 0;
}

INT CSIReportManager::SetCONNECTION_INFO(CString strCONNECTION_INFO)
{
	m_strCONNECTION_INFO = strCONNECTION_INFO;
	return 0;
}

INT CSIReportManager::GetSIReportFileFromDB(CString RptAlias)
{
	CString tmp_filename = _T("rpt_tmp\\tmp_SIReportManager.rpt");
	CString PK;
	INT ids = GetSIReportFileFromDB(RptAlias, tmp_filename, PK);
	if (ids >= 0) InitWorkFormFile(tmp_filename, RptAlias, PK);
	return ids;
}

INT CSIReportManager::GetSIReportFileFromDB(CString RptName, CString tmpFilename, CString &PK)
{
	DeleteTempBitmapFile(MASTER_IS_MANAGER);
	CESL_DataMgr *pDM = new CESL_DataMgr;
	if (!pDM) return -1000;
	pDM->SetCONNECTION_INFO(m_strCONNECTION_INFO);

	INT i, ids;
	CString szBuf, sQuery;
	CString strPK = _T(""), strRPTSize;

	sQuery.Format(_T("SELECT PK, RPTSIZE, FLAG, BITMAP_ALIAS FROM ESL_SIREPORT_TBL WHERE RPTALIAS='%s'"), RptName);
	pDM->RestructDataManager(sQuery);
	INT nRow = pDM->GetRowCount();

	// 사용자 수정 형식이 있는지 찾아본다. 있으면 이것이 원본 형식보다 우선한다.
	for (i = 0; i < nRow; i++) {
		pDM->GetCellData(i, 2, szBuf);
		szBuf.TrimLeft();			szBuf.TrimRight();
		szBuf.MakeUpper();
		if (szBuf == _T("U")) {
			pDM->GetCellData(i, 0, strPK);
			strPK.TrimLeft();		strPK.TrimRight();
			pDM->GetCellData(i, 1, strRPTSize);
			strRPTSize.TrimLeft();	strRPTSize.TrimRight();
			if (strRPTSize.GetLength() == 0) strRPTSize = _T("0");
			break;
		}
	}
	// 사용자 수정 형식이 없을때는 원본 형식을 찾는다.
	if (strPK.GetLength() == 0) {
		for (i = 0; i < nRow; i++) {
			pDM->GetCellData(i, 2, szBuf);
			szBuf.TrimLeft();			szBuf.TrimRight();
			szBuf.MakeUpper();
			if (szBuf == _T("X")) {
				pDM->GetCellData(i, 0, strPK);
				strPK.TrimLeft();		strPK.TrimRight();
				pDM->GetCellData(i, 1, strRPTSize);
				strRPTSize.TrimLeft();	strRPTSize.TrimRight();
				if (strRPTSize.GetLength() == 0) strRPTSize = _T("0");
				break;
			}
		}
	}
	if (strPK.GetLength() == 0) {
		if (pDM) {
			delete pDM;
			pDM = NULL;
		}
		return -1022;		// 해당 Report 가 없습니다.
	}
	PK = strPK;

	// Report Data File을 가져온다.
	ids = GetLOBToFile(pDM, _T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), _ttoi(strRPTSize), tmpFilename);
	if (ids < 0) {
		if (pDM) {
			delete pDM;
			pDM = NULL;
		}
		return -1025;		// Lob을 File로 다운 받는데 실패
	}

	CString strTmp, strALIAS;
	for (i = 0; i < nRow; i++) {
		pDM->GetCellData(i, 2, szBuf);
		szBuf.TrimLeft();			szBuf.TrimRight();
		szBuf.MakeUpper();
		if (szBuf == _T("B")) {
			pDM->GetCellData(i, 0, strPK);
			strPK.TrimLeft();		strPK.TrimRight();
			pDM->GetCellData(i, 1, strRPTSize);
			strRPTSize.TrimLeft();	strRPTSize.TrimRight();
			if (strRPTSize.GetLength() == 0) strRPTSize = _T("0");
			pDM->GetCellData(i, 3, strALIAS);
			strALIAS.TrimLeft();	strALIAS.TrimRight();
			if (strALIAS.GetLength() == 0) continue;
			strTmp.Format(_T("%s\\%s.bmp"), MANAGER_TEMP_DIR, strALIAS);
			ids = GetLOBToFile(pDM, _T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), _ttoi(strRPTSize), strTmp);
		}
	}
	if (pDM) {
		delete pDM;
		pDM = NULL;
	}
	
	return 0;
}

INT CSIReportManager::GetLOBToFile(CESL_DataMgr *pDM, CString table_name, CString PKFieldname, CString PKFieldtype, CString PKData, CString FieldName, INT Size, CString FileName)
{
	BYTE *pBuffer;
	INT nBufferSize = Size + 1024;
	INT ids;

	pBuffer = (BYTE*)malloc(sizeof(BYTE) * nBufferSize);
	if (!pBuffer) {
		AfxMessageBox(_T("Malloc Memory Fail!"));
		return -1024;
	}
	
	ids = pDM->LOBGet(table_name, PKFieldname, PKFieldtype, PKData, FieldName, pBuffer, &nBufferSize);
	if (ids < 0) {
		AfxMessageBox(_T("Get Lob Data Fail!"));
		free(pBuffer);
		return -1024;
	}
	if (nBufferSize <= 0) {
		AfxMessageBox(_T("Empty Lob Data!"));
		return -1024;
	}
	CFile f;
    if( !f.Open(FileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary) ) {
		free(pBuffer);
		AfxMessageBox(_T("Create Lob File Fail"));
		return - 1025;
	}
	
	if( 0 == f.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		f.Write( &cUni, sizeof(TCHAR));
	}	
		
	f.WriteHuge(pBuffer, nBufferSize);
	f.Close();
	free (pBuffer);

	return 0;
}

INT CSIReportManager::SetDataMgr(INT nPos, CESL_DataMgr *pDM, INT startcursor)
{
	m_pDM[nPos] = pDM;
	m_DMCursor = startcursor;

	return 0;
}

INT CSIReportManager::InitDMArray(VOID)
{
	for (INT i = 0; i < MAX_DM; i++) {
		m_pDM[i] = NULL;
	}
	return 0;
}

CString CSIReportManager::SplitStringWithLen(CString str, INT char_len, INT char_max)
{
	CString result;
	result.Empty();

	if (str.GetLength() > char_max) {
		if (!IsCanCut(str, char_max-1)) {
			char_max++;
		}
		str = str.Left(char_max);
	}
	CString token;
	INT pos = 0;
	while(true) {
		if (str.GetLength() == 0) break;
		pos = char_len;
		if (str.GetLength() > pos) {
			if (!IsCanCut(str, pos-1)) {
				pos++;
			}
		} else {
			pos = str.GetLength();
		}
		token.Format(_T("%s\r\n"),str.Left(pos));
		result += token;
		str = str.Mid(pos);
		str.TrimLeft();
		str.TrimRight();
	}
	
	return result;
}

INT CSIReportManager::DeleteTempBitmapFile(INT MODE) 
{
	CFileFind finder;
	CString strWildCard;
	BOOL bWorking;

	if (MODE == MASTER_IS_MANAGER) {
		strWildCard.Format(_T("%s\\*.bmp"), MANAGER_TEMP_DIR);
	} else {
		strWildCard.Format(_T("%s\\*.bmp"), EDITOR_TEMP_DIR);
	}

	CString strPath, strName;
	bWorking = finder.FindFile(strWildCard);	
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		strPath = finder.GetFilePath();
		strName = finder.GetFileName();

		DeleteFile(strPath);
	}
	
	return 0;
}

INT CSIReportManager::GetSIReportFileFromDBbyPK(CString PK, CString RptAlias, CString RptSize)
{
	CString tmp_filename = _T("rpt_tmp\\tmp_SIReportManager.rpt");
	INT ids = GetSIReportFileFromDBbyPK(PK, RptAlias, tmp_filename, RptSize);
	if (ids >= 0) InitWorkFormFile(tmp_filename, RptAlias, PK);
	return ids;
}

INT CSIReportManager::GetSIReportFileFromDBbyPK(CString PK, CString RptName, CString tmpFilename, CString RptSize)
{
	DeleteTempBitmapFile(MASTER_IS_MANAGER);
	CESL_DataMgr *pDM = new CESL_DataMgr;
	if (!pDM) return -1000;
	pDM->SetCONNECTION_INFO(m_strCONNECTION_INFO);

	INT i, ids;
	CString szBuf, sQuery;
	CString strPK = _T(""), strRPTSize;

	sQuery.Format(_T("SELECT PK, RPTSIZE, FLAG, BITMAP_ALIAS FROM ESL_SIREPORT_TBL WHERE RPTALIAS='%s'"), RptName);
	pDM->RestructDataManager(sQuery);
	INT nRow = pDM->GetRowCount();
	strPK = PK;
	strRPTSize = RptSize;

	// Report Data File을 가져온다.
	ids = GetLOBToFile(pDM, _T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), _ttoi(strRPTSize), tmpFilename);
	if (ids < 0) {
		if (pDM) {
			delete pDM;
			pDM = NULL;
		}
		return -1025;		// Lob을 File로 다운 받는데 실패
	}

	CString strTmp, strALIAS;
	for (i = 0; i < nRow; i++) {
		pDM->GetCellData(i, 2, szBuf);
		szBuf.TrimLeft();			szBuf.TrimRight();
		szBuf.MakeUpper();
		if (szBuf == _T("B")) {
			pDM->GetCellData(i, 0, strPK);
			strPK.TrimLeft();		strPK.TrimRight();
			pDM->GetCellData(i, 1, strRPTSize);
			strRPTSize.TrimLeft();	strRPTSize.TrimRight();
			if (strRPTSize.GetLength() == 0) strRPTSize = _T("0");
			pDM->GetCellData(i, 3, strALIAS);
			strALIAS.TrimLeft();	strALIAS.TrimRight();
			if (strALIAS.GetLength() == 0) continue;
			strTmp.Format(_T("%s\\%s.bmp"), MANAGER_TEMP_DIR, strALIAS);
			ids = GetLOBToFile(pDM, _T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), _ttoi(strRPTSize), strTmp);
		}
	}
	if (pDM) {
		delete pDM;
		pDM = NULL;
	}
	
	return 0;
}

CString CSIReportManager::GetKeywordString(CString str)
{
	CTime t = CTime::GetCurrentTime();
	str.TrimLeft();			str.TrimRight();
	str.MakeUpper();

	CString strTmp;
	INT pos1, pos2;
	CString strGETDM, strDMAlias, strDMFieldAlias, strDMRow;
	if (str.Find(_T("%DATE_TODAY%")) != -1) {
		strTmp.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
		str.Replace(_T("%DATE_TODAY%"), strTmp);
	}
	if (str.Find(_T("%DATE_YEAR%")) != -1) {
		strTmp.Format(_T("%d"), t.GetYear());
		str.Replace(_T("%DATE_YEAR%"), strTmp);
	}
	if (str.Find(_T("%DATE_MONTH%")) != -1) {
		strTmp.Format(_T("%d"), t.GetMonth());
		str.Replace(_T("%DATE_MONTH%"), strTmp);
	}
	if (str.Find(_T("%DATE_DAY%")) != -1) {
		strTmp.Format(_T("%d"), t.GetDay());
		str.Replace(_T("%DATE_DAY%"), strTmp);
	}
	if (str.Find(_T("%PAGE_NUM%")) != -1) {
		strTmp.Format(_T("%d"), m_EventInfo.m_nPage);
		str.Replace(_T("%PAGE_NUM%"), strTmp);
	}
	if (str.Find(_T("%TOTAL_PAGES%")) != -1) {
		strTmp.Format(_T("%d"), m_EventInfo.m_nTotalPage);
		str.Replace(_T("%TOTAL_PAGES%"), strTmp);
	}
	if (str.Find(_T("%GETDMDATA(")) != -1) {
		pos1 = str.Find(_T("%GETDMDATA("));
		pos2 = str.Find(_T(")%"), pos1);
		strGETDM = str.Mid(pos1, pos2 - pos1 + 2);
		if (pos1 != -1 && pos2 != -1) {
			strTmp = str.Mid(pos1 + 11, pos2 - pos1 - 11);
			pos1 = strTmp.Find(_T("."));
			strDMAlias = strTmp.Mid(0, pos1);
			pos2 = strTmp.Find(_T("["));
			strDMFieldAlias = strTmp.Mid(pos1+1, pos2-pos1-1);
			pos1 = pos2;
			pos2 = strTmp.Find(_T("]"));
			strDMRow = strTmp.Mid(pos1+1, pos2-pos1-1);
			GetKeywordString_DMData(strDMAlias, strDMFieldAlias, _ttoi(strDMRow), strTmp);
			str.Replace(strGETDM, strTmp);
		}
	}
	if (str.Find(_T("%GETDEFAULTDMDATA(")) != -1) {
		pos1 = str.Find(_T("%GETDEFAULTDMDATA("));
		pos2 = str.Find(_T(")%"), pos1);
		strGETDM = str.Mid(pos1, pos2 - pos1 + 2);
		if (pos1 != -1 && pos2 != -1) {
			pos2 = str.Find(_T(")%"));
			strDMFieldAlias = str.Mid(pos1 + 18, pos2 - pos1 - 18);
			strTmp = _T("");
			if (m_pDM[0]) m_pDM[0]->GetCellData(strDMFieldAlias, 0, strTmp);
			str.Replace(strGETDM, strTmp);
		}
	}
	if (str.Find(_T("%GETTEMPVARIABLE(")) != -1) {
		pos1 = str.Find(_T("%GETTEMPVARIABLE("));
		pos2 = str.Find(_T(")%"), pos1);
		strGETDM = str.Mid(pos1, pos2 - pos1 + 2);
		if (pos1 != -1 && pos2 != -1) {
			pos2 = str.Find(_T(")%"));
			strDMFieldAlias.Format(_T("%s"), str.Mid(pos1 + 17, pos2 - pos1 - 17));
			strDMFieldAlias.TrimLeft();		strDMFieldAlias.TrimRight();
			strTmp = _T("");
			GetTempVariable(strDMFieldAlias, strTmp);
			str.Replace(strGETDM, strTmp);
		}
	}
	return str;
}

INT CSIReportManager::GetKeywordString_DMData(CString strDMAlias, CString strDMFieldAlias, INT DMRow, CString &strResult)
{
	strResult = _T("");
	INT ids;
	if (strDMAlias.GetLength() == 0) {
		ids= m_pDM[0]->GetCellData(strDMFieldAlias, DMRow, strResult);
	} else {
		ids= m_pDM[0]->GetCellData(strDMFieldAlias, DMRow, strResult);
	}

	return ids;
}

INT CSIReportManager::DM_GetCellData(CString strDMs, INT nRow, CString &strResult)
{
	INT pos1, pos2;
	INT ids;
	CString strDM;
	CString strCellResult;
	CString strDMAlias, strDMFieldAlias;

	strResult = strDMs;
	if (strDMs.Find(_T(".")) != -1) {	// Default DM 이 아닌경우
		while(TRUE) {
			if (strResult.Find(_T("%")) == -1) break;
			pos1 = strResult.Find(_T("%"), 0);
			pos2 = strResult.Find(_T("%"), pos1+1);
			if (pos1 == -1 || pos2 == -1) break;
			strDM = strResult.Mid(pos1, pos2-pos1+1);
			strDMAlias = strDM.Mid(1, strDM.Find(_T("."))-1);
			strDMFieldAlias = strDM.Mid(strDM.Find(_T("."))+1, strDM.GetLength() - strDM.Find(_T("."))-2);

			ids = GetKeywordString_DMData(strDMAlias, strDMFieldAlias, nRow, strCellResult);
			strResult.Replace(strDM, strCellResult);
		}
	} else {	// Default DM인경우
		while(TRUE) {
			if (strResult.Find(_T("%")) == -1) break;
			pos1 = strResult.Find(_T("%"), 0);
			pos2 = strResult.Find(_T("%"), pos1+1);
			if (pos1 == -1 || pos2 == -1) break;
			strDMAlias = _T("");
			strDM = strResult.Mid(pos1, pos2-pos1+1);
			strDMFieldAlias = strResult.Mid(pos1+1, pos2-pos1-1);

			ids = GetKeywordString_DMData(strDMAlias, strDMFieldAlias, nRow, strCellResult);
			strResult.Replace(strDM, strCellResult);
		}
	}
	return 0;
}

/*
Return value range : -51001 ~ -51999 
*/
INT CSIReportManager::LOC_PrintBarcode(INT mode, CDC *pDC, CRect rect)
{
	m_bPrintTotal = TRUE;

	if (m_pDM[0]->GetRowCount() <= m_DMCursor) return 0;

	CString strTitleObjectName = _T("%BARCODE바코드제목%");
	CString strBarcodeObjectName = _T("%BARCODE바코드%");
	CString strValueObjectName = _T("%BARCODE바코드값%");

	CSIReportWorkSheet * pSheet = NULL;
	POSITION pos = m_pWorkForm->WorkSheetList.FindIndex(m_nCurrentWorkSheet);
	pSheet = (CSIReportWorkSheet*)m_pWorkForm->WorkSheetList.GetAt(pos);
	if (!pSheet) return -51100;

	CSIReportObject_Text_Box *pTitle = (CSIReportObject_Text_Box *)FindObject(pSheet, strTitleObjectName);
	CSIReportObject_Barcode *pBarcode = (CSIReportObject_Barcode *)FindObject(pSheet, strBarcodeObjectName);
	CSIReportObject_Text_Box *pValue = (CSIReportObject_Text_Box *)FindObject(pSheet, strValueObjectName);

	if (pTitle == NULL) return -51001;
	if (pBarcode == NULL) return -51002;
	if (pValue == NULL) return -51003;

	if (m_DMCursor < 0) m_DMCursor = 0;

	CRect rTitle_old = pTitle->GetRect();
	CRect rBarcode_old = pBarcode->GetRect();
	CRect rValue_old = pValue->GetRect();

	// CRect( INT l, INT t, INT r, INT b );
	CRect rTitle_new(rect.left, rect.top, rect.left + rTitle_old.Width(), rect.top + rTitle_old.Height());
	CRect rBarcode_new(rect.left, rTitle_new.bottom, rect.left + rBarcode_old.Width(), rTitle_new.bottom + rBarcode_old.Height());
	CRect rValue_new(rect.left, rBarcode_new.bottom, rect.left + rValue_old.Width(), rBarcode_new.bottom + rValue_old.Height());

	CString strTitleObjectNewName, strBarcodeObjectNewName, strValueObjectNewName;
	strTitleObjectNewName.Format(_T("바코드제목_%d"), m_DMCursor);
	strBarcodeObjectNewName.Format(_T("바코드_%d"), m_DMCursor);
	strValueObjectNewName.Format(_T("바코드값_%d"), m_DMCursor);

	pTitle->SetRect(rTitle_new);
	pTitle->SetName(strTitleObjectNewName);
	GetTempVariable(_T("%BARCODE바코드제목%"), pTitle->text.text);

	CString strBarcodeCaption, strBarcodeValue;
	pBarcode->GetCaption(strBarcodeCaption);
	pBarcode->SetRect(rBarcode_new);
	pBarcode->SetName(strBarcodeObjectNewName);
	strBarcodeValue = m_pDM[0]->GetCellData(strBarcodeCaption, m_DMCursor);
	pBarcode->SetCaption(strBarcodeValue);

	pValue->SetRect(rValue_new);
	pValue->SetName(strValueObjectNewName);
	pValue->text.text = strBarcodeValue;

	
	DrawObject_Text_Box(mode, pDC, pTitle);
	DrawObject_Barcode(mode, pDC, pBarcode);
	DrawObject_Text_Box(mode, pDC, pValue);


	pTitle->SetRect(rTitle_old);
	pTitle->SetName(strTitleObjectName);

	pBarcode->SetRect(rBarcode_old);
	pBarcode->SetName(strBarcodeObjectName);
	pBarcode->SetCaption(strBarcodeCaption);

	pValue->SetRect(rValue_old);
	pValue->SetName(strValueObjectName);

	m_DMCursor++;
	return 0;
}

INT CSIReportManager::SetTempVariable(CString strVariableName, CString strVariableValue)
{
	INT nTempVariables = m_arrTempVariableName.GetSize();
	for (INT i = 0; i < nTempVariables; i++) {
		if (m_arrTempVariableName.GetAt(i) == strVariableName) {
			m_arrTempVariableValue.SetAt(i, strVariableValue);
			return 0;
		}
	}
	m_arrTempVariableName.Add(strVariableName);
	m_arrTempVariableValue.Add(strVariableValue);
	return 0;
}

INT CSIReportManager::GetTempVariable(CString strVariableName, CString &strVariableValue)
{
	INT nTempVariables = m_arrTempVariableName.GetSize();
	for (INT i = 0; i < nTempVariables; i++) {
		if (m_arrTempVariableName.GetAt(i) == strVariableName) {
			strVariableValue = m_arrTempVariableValue.GetAt(i);
			return 0;
		}
	}
	return -1;
}

BOOL CSIReportManager::IsSheetHaveRepeatObject(VOID)
{
	CSIReportWorkSheet * pSheet = NULL;
	POSITION pos = m_pWorkForm->WorkSheetList.FindIndex(m_nCurrentWorkSheet);
	pSheet = (CSIReportWorkSheet*)m_pWorkForm->WorkSheetList.GetAt(pos);
	if (!pSheet) return FALSE;

	INT count = pSheet->ObjList.GetCount();
	if (count == 0) return FALSE;

	CSIReportObject * pObject;
	pos = NULL;
	pos = pSheet->ObjList.GetHeadPosition();
	for(INT i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() == OBJECT_TABLE) {
			CSIReportObject_Table *pTable = (CSIReportObject_Table *)pObject;
			if (!pTable) continue;
			if (pTable->repeat == TRUE) return TRUE;
		}
	}
	return FALSE;
}

INT CSIReportManager::ShowXMLFileOnePage(INT mode, CDC *pDC, INT nPage)
{
	if (m_pPageWorkForm) {
		FreePageWorkForm();
	}
	if (m_nCurrentPreviewPage != nPage) {
		LoadPageXMLWorkForm(nPage);
	}
	CSIReportWorkSheet *pSheet = m_pWorkForm->GetWorkSheet(0);
	if (pSheet == NULL) return -1;
	INT count = pSheet->ObjList.GetCount();
	if (count == 0) return -1;

	// Child가 아닌 객체중에서 Child가 없는 객체 부터 인쇄한다.
	CSIReportObject * pObject;
	POSITION pos;
	INT i;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) {
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() == OBJECT_TABLE) {
			CSIReportObject_Table *pTable = (CSIReportObject_Table *)pObject;
			DrawObject_Table(mode, pDC, pTable);
		}
		if (pObject->GetObjType() == OBJECT_BARCODE) {
			CSIReportObject_Barcode *pBarcode = (CSIReportObject_Barcode *)pObject;
			DrawObject_Barcode(mode, pDC, pBarcode);
		}
		if (pObject->GetObjType() == OBJECT_TEXT_BOX) {
			CSIReportObject_Text_Box *pText_Box = (CSIReportObject_Text_Box *)pObject;
			DrawObject_Text_Box(mode, pDC, pText_Box);
		}
		if (pObject->GetObjType() == OBJECT_LINE) {
			CSIReportObject_Line *pLine = (CSIReportObject_Line *)pObject;
			DrawObject_Line(mode, pDC, pLine);
		}
		if (pObject->GetObjType() == OBJECT_BITMAP) {
			CSIReportObject_Bitmap *pBitmap = (CSIReportObject_Bitmap *)pObject;
			DrawObject_Bitmap(mode, pDC, pBitmap);
		}
	}
	return 0;
}
