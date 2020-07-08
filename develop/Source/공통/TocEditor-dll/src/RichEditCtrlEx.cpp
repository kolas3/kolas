// RichEditCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "RichEditCtrlEx.h"
#include "TocMetaData.h"

#include "TocEditorDlg.h"
#include "resource.h"
#include "Parse.h"
#include "mathconvertor.h"
#include "../../../../ESL/ESL/LIB_F2KEY_SpecialChar.h"

#include "CharConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RGB_NORMAL_COLOR	RGB(0,0,0)		// 검정색
#define RGB_TAG_COLOR		RGB(0,100,0)	// 청록색
#define RGB_CHOROK_COLOR	RGB(255,0,0)	// 빨강색

DWORD __stdcall MEditStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CString *psBuffer = (CString *)dwCookie;

	if (cb > psBuffer->GetLength()) cb = psBuffer->GetLength();

	for (INT i=0;i<cb;i++) {
		*(pbBuff+i) = psBuffer->GetAt(i);
	}

	*pcb = cb;

	*psBuffer = psBuffer->Mid(cb);

	return 0;
}

DWORD __stdcall MEditStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CString sThisWrite;
	sThisWrite.GetBufferSetLength(cb);

	CString *psBuffer = (CString *)dwCookie;
	
	for (INT i=0;i<cb;i++) {
		sThisWrite.SetAt(i,*(pbBuff+i));
	}

	*psBuffer += sThisWrite;

	*pcb = sThisWrite.GetLength();
	sThisWrite.ReleaseBuffer();
	return 0;
}


const CString sViewerFileName = _T("TOCVIEWER.cfg");
/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlEx

CRichEditCtrlEx::CRichEditCtrlEx()
: CRichEditCtrl()
{
	m_iCurrentLine = 0;
	m_preScrollPos = 0 ;
	m_pParent = NULL ;
	bViewer = FALSE;

	m_pRichEditOle = NULL ;
	
	
}

CRichEditCtrlEx::~CRichEditCtrlEx()
{
	
}


BEGIN_MESSAGE_MAP(CRichEditCtrlEx, CRichEditCtrl)
	//{{AFX_MSG_MAP(CRichEditCtrlEx)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_MATH_DELETE, OnMathDelete)
	ON_COMMAND(ID_MATH_EDIT, OnMathEdit)
	ON_COMMAND(ID_MATH_TAGVIEW, OnMathTagView)
	ON_COMMAND(ID_TOC_UNDO, OnUndo)
	ON_COMMAND(ID_TOC_REDO, OnRedo)
	ON_COMMAND(ID_TOC_CUT, OnTocCut)
	ON_COMMAND(ID_TOC_COPY, OnTocCopy)
	ON_COMMAND(ID_TOC_PASTE, OnTocPaste)
	ON_COMMAND(ID_CHECK_SYNTAX, OnCheckSyntax)
	ON_COMMAND(IDC_BUTTON_SOOSIKEDITOR, OnSoosikEditor)
	ON_COMMAND(IDC_BUTTON_VIEWER, OnViewer)
	ON_COMMAND(ID_TOC_NEW, OnTocNewEdit)
	ON_COMMAND(MSG_EDIT_DAN_CHOROK, OnEditDanChorok)
	ON_COMMAND(MSG_EDIT_YON_CHOROK, OnEditYonChorok)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlEx message handlers

VOID CRichEditCtrlEx::SetSelectionLine(INT iIdx)
{
	m_iCurrentLine = iIdx;
	
	LONG nStartChar = 0;
	LONG nEndChar = 0;

	nStartChar = LineIndex(iIdx);
	nEndChar = LineIndex(iIdx);
	
	TCHAR pszText[MAX_LINE];
	nEndChar += GetLine(iIdx,pszText);

//	SetSel(nStartChar,nEndChar);

	// LineScroll(-1000);
	// LineScroll(iIdx);

}

VOID CRichEditCtrlEx::SetWindowText(LPCTSTR lpszString)
{
//	m_pParent->LockWindowUpdate() ;
	CMathConvertor m_mathc;
	

	TCHAR m_exeDir[256];
	GetCurrentDirectory( 256 , m_exeDir ) ;
	CString sWorkDir = m_exeDir ;
	sWorkDir.TrimRight(_T("\\"));
	INT idx= sWorkDir.ReverseFind( '\\' ) ;
	if ( idx<=0 ) {
//		AfxMessageBox( _T("cfg 파일의 경로를 얻을수가 없습니다.\r\nTAG 에 색을 입힐 수 없습니다!") , MB_ICONERROR | MB_OK ) ;
		return ;
	}
	sWorkDir= sWorkDir.Mid(0,idx) ;
//	sWorkDir += _T("\\cfg") ;
	sWorkDir += PATH_MATH_TMP ;
	
//	ExecuteProc(_T("del"), _TEXT(_T(" *.bmp")), FALSE );
//	ExecuteProc(_T("del"), _TEXT(_T(" *.html")), FALSE );
/*	CHARFORMAT Origincf;
	GetDefaultCharFormat(Origincf);
	Origincf.dwEffects = 0;
	Origincf.crTextColor = RGB(0,0,0);
	SetDefaultCharFormat(Origincf);
*/
		
	CDC *pDC = AfxGetMainWnd()->GetDC();
	
	CString cstmp(lpszString), cstmp2;  CString csfilename;
	cstmp2.Format(_T("%s\\%s"), sWorkDir, _T("*.bmp"));
	ExecuteProc(_T("del"), sWorkDir, FALSE );
	cstmp2.Format(_T("%s\\%s"), sWorkDir, _T("*.html"));
	ExecuteProc(_T("del"), sWorkDir, FALSE );
	

	if(bViewer) Convert2ViewerTag(cstmp);	
	INT i=1; INT nDeletedCnt; INT* nChar = m_nMathSit; INT nStart;
	nChar = (INT *)malloc(sizeof(INT));

	while( GetMathTag(1, &cstmp, &cstmp2, &(nChar[i-1])) ) {
		CFile cf;
		csfilename.Format(_T("%s\\%s%03d.html"), sWorkDir, MATH_TMP_FILENAME, i);
		if(cf.Open(csfilename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)) {
			if( 0 == cf.GetLength() )
			{			
				TCHAR cUni = 0xFEFF;
				cf.Write( &cUni, sizeof(TCHAR));
			}	
			
			cf.Write( cstmp2, cstmp2.GetLength() );
			cf.Close();
		}
		
//		ConvertTag_To_Bitmap(csfilename, i);
		m_mathc.Load(csfilename, pDC);
		csfilename.Format(_T("%s\\%s%03d.BMP"), sWorkDir, MATH_TMP_FILENAME, i);
		m_mathc.WriteWindowToDIB((TCHAR *)(LPCTSTR)csfilename);
		m_mathc.Close();
		i++;
		
		nChar = (INT *)realloc(nChar, sizeof(INT)*i);
		cstmp.Replace(cstmp2, _T(""));

	}
//	m_nMaxMathCnt++;
	INT nChanged = 0;
	
	m_mathlist.deleteAll();

	CWnd::SetWindowText(cstmp);
	if(bViewer) Convert2ViewerTag(cstmp, TRUE);

	for(INT i2=0; i2< i-1;i2++) {
		csfilename.Format(_T("%s\\%s%03d.BMP"), sWorkDir, MATH_TMP_FILENAME, i2+1);
		SetSel(nChar[i2]+i2 + nChanged, nChar[i2]+i2 + nChanged);
		InsertImg(5, csfilename);
	}

	ReleaseDC(pDC);
//	if(!bViewer)
	//DisplayTagColor(bViewer);
	DisplayTagColor(FALSE);
	DisplayTagColor(TRUE);

//	m_pParent->UnlockWindowUpdate() ;

}

VOID CRichEditCtrlEx::GetWindowText(LPCTSTR lpstr ) 
{
	CString cstmp;
	cstmp = lpstr;
	GetWindowText(cstmp);
}

VOID CRichEditCtrlEx::GetWindowTextNoMath(CString& cstmp )
{
	CWnd::GetWindowText(cstmp);
}

VOID CRichEditCtrlEx::GetWindowText(CString& cstmp ) 
{

	
	CString cstmp2, csfilename;
	LONG lMathSit;
	CFile cf; TCHAR *pBuf; FILE *fp;

	TCHAR m_exeDir[256];
	GetCurrentDirectory( 256 , m_exeDir ) ;
	CString sWorkDir = m_exeDir ;
	sWorkDir.TrimRight(_T("\\"));
	INT idx= sWorkDir.ReverseFind( '\\' ) ;
	if ( idx<=0 ) {
//		AfxMessageBox( _T("cfg 파일의 경로를 얻을수가 없습니다.\r\nTAG 에 색을 입힐 수 없습니다!") , MB_ICONERROR | MB_OK ) ;
		return ;
	}
	sWorkDir= sWorkDir.Mid(0,idx) ;
	sWorkDir += PATH_MATH_TMP ;


	CWnd::GetWindowText(cstmp);
	if(GetMathCnt()<1) return;
	for(INT i=GetMathCnt()-1;i>=0;i--) {
		lMathSit = GetMathSit(i);
		if( lMathSit < 0 ) continue;
		csfilename.Format(_T("%s\\%s%03d.html"), sWorkDir, MATH_TMP_FILENAME, i+1);

		if(cf.Open(csfilename, CFile::modeRead | CFile::typeBinary)) {
			TCHAR cUni;
			cf.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				cf.SeekToBegin();
			}
			
			INT nSize = cf.GetLength() ;
			UINT nnSize = (UINT)nSize;
			pBuf = new TCHAR[nSize];
			nnSize = cf.Read( (VOID *)pBuf, (UINT)nnSize );
			cf.Close();
			pBuf[nSize] = '\0';
			cstmp2.Format(_T("%s"), pBuf);
			if(cstmp.Insert( lMathSit+1, cstmp2) < 0 ) AfxMessageBox(_T("Error Insert"));

			INT nnnSize = cstmp2.GetLength();
			TCHAR *tmpchar;
			tmpchar = (TCHAR*)(LPCTSTR)cstmp;
			if(tmpchar[lMathSit] == ' ') 
				cstmp.Delete(lMathSit, 1);
			else 
				cstmp.Delete(lMathSit+cstmp2.GetLength()+1, 1);
			nnnSize = lMathSit;

		}
		else AfxMessageBox(_T("파일 Open Error!!"));
		
	}

//	free(MathSit);
//	wmemcpy((VOID*)lpstr, (VOID*)(LPCTSTR)cstmp, cstmp.GetLength());

}
/*
VOID CRichEditCtrlEx::DisplayTagColor(BOOL bViewer)
{
	TCHAR pszText[MAX_LINE];
	CString cstmp;
	INT iMaxCount = GetLineCount();
	INT iLineLength = 0;

//	cfg 파일 경로설정.
	TCHAR m_exeDir[256];
	GetCurrentDirectory( 256 , m_exeDir ) ;
	CString sWorkDir = m_exeDir ;
	sWorkDir.TrimRight(_T("\\")) ;
	INT idx= sWorkDir.ReverseFind( '\\' ) ;
	if ( idx<=0 ) {
//		AfxMessageBox( _T("cfg 파일의 경로를 얻을수가 없습니다.\r\nTAG 에 색을 입힐 수 없습니다!") , MB_ICONERROR | MB_OK ) ;
		return ;
	}
	sWorkDir= sWorkDir.Mid(0,idx) ;
	sWorkDir += PATH_CFG ;

	if(!bViewer) {
		SetSel(0, -1);
		SetSelectionCharFormat(cfBase);
		return;
	}
	
	GetWindowTextNoMath(cstmp);
	
	CString sWriteText; //Where the text will be streamed from

	sWriteText=_T("{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang2057{\\fonttbl{\\f0\froman\\fprq2\\fcharset0 Times New Roman;}{\\f1\\fswiss\\fprq2\\fcharset0 System;}}{\\colortbl ;\\red255\\green0\\blue0;\\red51\\green153\\blue102;\\red0\\green0\\blue255;}\\viewkind4\\uc1\\pard\\cf1\\i\\f0\\fs24 Inputted\\cf0\\i0  \\cf2\\b rich\\cf0\\b0  \\cf3 text\\cf0 !\\b\\f1\\fs20 \\par }");
//	sWriteText=_T("{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang2057{\\fonttbl{\\f0\froman\\fprq2\\fcharset0 Times New Roman;}{\\f1\\fswiss\\fprq2\\fcharset0 System;}}{\\colortbl ;\\red255\\green0\\blue0;\\red51\\green153\\blue102;\\red0\\green0\\blue255;}\\viewkind4\\uc1\\pard\\cf1\\i\\f0\\fs24 Inputted\\cf0\\i0  \\cf2\\b rich\\cf0\\b0  \\cf3 text\\cf0 !\\b\\f1\\fs20 \\par }");
	// This is hard-coded for example purposes. It is likely this would
	// be read from file or another source.

	EDITSTREAM es;

	es.dwCookie = (DWORD)&sWriteText; // Pass a pointer to the CString to the callback function
	es.pfnCallback = MEditStreamInCallback; // Specify the pointer to the callback function
											// Function defined in RichEg.cpp

	StreamIn(SF_RTF,es); // Perform the streaming

}
*/

VOID CRichEditCtrlEx::DisplayTagColor(BOOL bViewer)
{
	TCHAR pszText[MAX_LINE];
	CString sText;
	INT iMaxCount = GetLineCount();
	INT iLineLength = 0;

//	cfg 파일 경로설정.
	TCHAR m_exeDir[256];
	GetCurrentDirectory( 256 , m_exeDir ) ;
	CString sWorkDir = m_exeDir ;
	sWorkDir.TrimRight(_T("\\"));
	INT idx= sWorkDir.ReverseFind( _T('\\'));
	if ( idx<=0 ) {
//		AfxMessageBox( _T("cfg 파일의 경로를 얻을수가 없습니다.\r\nTAG 에 색을 입힐 수 없습니다!") , MB_ICONERROR | MB_OK ) ;
		return ;
	}
	sWorkDir= sWorkDir.Mid(0,idx) ;
	sWorkDir += PATH_CFG ;

	if(!bViewer) {
		SetSel(0, -1);
		SetSelectionCharFormat(cfBase);
		return;
	}
//	if(!bViewer) {
	CTocMetaData m_TocMetaData;
	m_TocMetaData.Init(sWorkDir, m_nIsKindOf);   //수정 이재영 2003.03.19
	
	INT iMaxTagCount = m_TocMetaData.GetTagCount();
	
	CString sRight ;
	CString sTag;
	INT iStartIdx = 0;
	INT iEndIdx = 0;
	
	for(INT i = 0 ; i < iMaxCount ; i++)
	{
		iLineLength = GetLine(i,pszText);
		pszText[iLineLength] = '\0';
		sText.Format(_T("%s"),pszText);
		sRight= sText ;
		
		INT nLineIndex = 0 ;
		iStartIdx = 0;
		iEndIdx = 0;
		
		
		
		for(INT j = 0 ; j < iMaxTagCount ; j++)
		{
			iStartIdx += m_TocMetaData.ParseTocTag(sRight,sTag);
			if (iStartIdx < 0) {
				break;
			}
			
			iEndIdx += iStartIdx + sTag.GetLength() + 2;
			nLineIndex= LineIndex(i) ;
			SetSel(nLineIndex + iStartIdx, nLineIndex + iEndIdx);
			CHARFORMAT cf;
			GetSelectionCharFormat(cf);
			
			cf.dwEffects = CFE_BOLD;
			cf.crTextColor = RGB(0,0,128);
			
			SetSelectionCharFormat(cf);
		}
	}

	SetSel(0, 0);
	

}


VOID CRichEditCtrlEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	INT prePos = GetScrollPos( SB_VERT ) ;
	CRichEditCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	if(bViewer) return ;
	m_preScrollPos = GetScrollPos( SB_VERT ) ;
	if ( nSBCode != SB_THUMBTRACK && prePos == 0 && m_preScrollPos == 0)	return ;	// 스크롤이 top일 때
	if ( nSBCode == SB_ENDSCROLL || nSBCode == SB_THUMBPOSITION ) return ;

	((CTocEditorDlg*)m_pParent)->DisplayCurrentLineNumber() ;

}

BOOL CRichEditCtrlEx::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	INT prePos = GetScrollPos( SB_VERT ) ;
	BOOL retval = CRichEditCtrl::OnMouseWheel(nFlags, zDelta, pt);
	if(bViewer) return retval ;
	m_preScrollPos = GetScrollPos( SB_VERT ) ;
	if ( prePos == 0 && m_preScrollPos == 0 ) return retval ;	// 스크롤이 top일 때

	((CTocEditorDlg*)m_pParent)->DisplayCurrentLineNumber() ;

	return retval ;
}

BOOL CRichEditCtrlEx::PreTranslateMessage(MSG* pMsg) 
{
//	long nStartChar, nEndChar ;
//
//	REOBJECT reobj;
	BOOL bChanged		= FALSE;
	BOOL bIsValidKey	= FALSE;
	long lStart, lEnd; CString cstmp; INT nStrSize = 0; CString cstmp2; INT i;INT nMathSit, nMathSize;
	long lStartOffset;
	//=====================================================
	//2009.02.02 UPDATE BY PJW : 유니코드형식으로 변경
	INT nFindCnt = 0;
	INT nReturnLoc = 0;
	INT nLength1 = 0;
	INT nLength2 = 0;
	//=====================================================

	if( bViewer ) return FALSE;
	if( pMsg->message == WM_KEYDOWN) {

		if ( pMsg->wParam == VK_F2 )
		{
			CLIB_F2KEY_SpecialChar Dlg(this);
			Dlg.DoModal();			
			ReplaceSel(Dlg.AppendData);
			return TRUE;
		}
		
		if ( GetAsyncKeyState(VK_CONTROL) )
		{
			switch( pMsg->wParam ) {
			case 86 - 'V' + 'C':
/*				GetSel(lStart, lEnd);
				lStartOffset = 0;
				nStrSize = lEnd - lStart ;
				for(i=0;i<GetMathCnt();i++)  {
					nMathSit = GetMathSit(i) ;
					nMathSize = GetMathStringSize(i) + 4;
					if( nMathSit < lStart)
						lStartOffset+= nMathSize - 1;
					else if( nMathSit < lEnd )	nStrSize+= nMathSize - 1;
				}
						
				GetWindowText(cstmp);
				cstmp2 = cstmp.Mid(lStart + lStartOffset, nStrSize );
//				AfxMessageBox(cstmp2);
				if( ::OpenClipboard(m_hWnd) ) {
					::EmptyClipboard();
					HANDLE hData = ::GlobalAlloc(GMEM_MOVEABLE, nStrSize + 1);
					LPCTSTR pData = (LPCTSTR) ::GlobalLock(hData);
					::lstrcpy(pData, (LPCTSTR)cstmp2);
					::GlobalUnlock(hData);

					::SetClipboardData(CF_TEXT, hData);
					::CloseClipboard();
				}
*/

				GetSel(lStart, lEnd);
				lStartOffset = 0;
				nStrSize = lEnd - lStart ;
				for(i=0;i<GetMathCnt();i++)  {
					nMathSit = GetMathSit(i) ;
					nMathSize = GetMathStringSize(i) + 4;
					if( nMathSit < lStart)
						lStartOffset+= nMathSize - 1;
					else if( nMathSit < lEnd )	nStrSize+= nMathSize - 1;
				}
						
				GetWindowText(cstmp);
				//=====================================================
				//2009.02.02 UPDATE BY PJW : 유니코드형식으로 변경
// 				cstmp2 = cstmp.Mid(lStart + lStartOffset, nStrSize );
				nLength1 = cstmp.GetLength();
				cstmp.Replace(_T("\r\n"), _T("\r"));

				nReturnLoc = (INT)lStart;
				while(1)
				{
//  					nReturnLoc = cstmp.Find(_T("\r\n"), nReturnLoc+1);
					nReturnLoc = cstmp.Find(_T("\r"), nReturnLoc+1);
					if( lEnd > (LONG)nReturnLoc && nReturnLoc > 0)
					{
						++nFindCnt; 
					}
					else
					{
						break;
					}
				}

				nStrSize += nFindCnt;
				cstmp2 = cstmp.Mid(lStart + lStartOffset, nStrSize);
				//=====================================================
				
//				AfxMessageBox(cstmp2);
				if( ::OpenClipboard(m_hWnd) ) {
					::EmptyClipboard();
					//=====================================================
					//2009.02.02 UPDATE BY PJW : 유니코드형식으로 변경
// 	 				HANDLE hData = ::GlobalAlloc(GMEM_MOVEABLE, nStrSize + 1);
					HGLOBAL hData = ::GlobalAlloc(GMEM_MOVEABLE, (nStrSize + 1)*sizeof(TCHAR));
					//=====================================================
					
#ifdef _UNICODE
					LPWSTR pData = (LPWSTR) ::GlobalLock(hData);
					::lstrcpy(pData, (LPCTSTR)cstmp2);
					::GlobalUnlock(hData);

					::SetClipboardData(CF_UNICODETEXT, hData);
					::CloseClipboard();
#else
					LPCTSTR pData = (LPCTSTR) ::GlobalLock(hData);
					::lstrcpy(pData, (LPCTSTR)cstmp2);
					::GlobalUnlock(hData);

					::SetClipboardData(CF_TEXT, hData);
					::CloseClipboard();
#endif
				}
				
				

				return TRUE; 
//				for(INT i=lStart;i<lEnd;i++)
//					if( IsMath(i) ) GetMath(
			break;

			case 86 - 'V' + 'A': {
				GetWindowText(cstmp);
				
				CFile cf;
				CString csfilename;
				csfilename.Format(_T("c:\Toctemp.html"));
				if(cf.Open(csfilename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)) {
				
					if( 0 == cf.GetLength() )
					{			
						TCHAR cUni = 0xFEFF;
						cf.Write( &cUni, sizeof(TCHAR));
					}	
					cf.Write( cstmp, cstmp.GetLength() );
					cf.Close();
				}
				AfxMessageBox(cstmp);
				return TRUE;
			}
//				SetWindowText(cstmp);
				
			break;
			case 86 :	//	Ctrl + 'V'	- Paste
				GetSel(lStart, lEnd);
				lStartOffset = 0;
				nStrSize = lEnd - lStart ;
				for(i=0;i<GetMathCnt();i++) {
					nMathSit = GetMathSit(i) ;
					nMathSize = GetMathStringSize(i) + 4;
					if( nMathSit < lStart)
						lStartOffset+= nMathSize - 1;
					else if( nMathSit < lEnd )	nStrSize+= nMathSize - 1;
				}
						
				GetWindowText(cstmp);
				
				//=====================================================
				//2009.02.02 ADD BY PJW : 유니코드형식으로 변경
				nLength1 = cstmp.GetLength();
				cstmp.Replace(_T("\r\n"), _T("\r"));
				cstmp.Replace(_T("\n"), _T("\r"));
				cstmp.Replace(_T("\r"), _T("\r\n"));
				nLength2 = cstmp.GetLength();
				if( nLength1 > nLength2 )
				{
					nFindCnt = nLength1 - nLength2;
				}
				else 
				{
					nFindCnt = nLength2 - nLength1;
				}
				while(1)
				{
 					nReturnLoc = cstmp.Find(_T("\r\n"), nReturnLoc+1);
					if( lStart > (LONG)nReturnLoc && nReturnLoc > 0)
					{
						++nFindCnt; 
					}
					else
					{
						break;
					}
				}
				//=====================================================
				
			
				LPCTSTR pData;
				if( ::OpenClipboard(NULL) ) {

				//=====================================================
				//2009.02.02 UPDATE BY PJW : 유니코드형식으로 변경
// 				HANDLE hData = ::GetClipboardData(CF_UNICODETEXT);
				HGLOBAL hData = ::GetClipboardData(CF_UNICODETEXT);
				//=====================================================
				
					if (hData != NULL) {
						pData = (LPCTSTR) ::GlobalLock(hData);

						::GlobalUnlock(hData);
					}
					
					::CloseClipboard();
				}
//				cstmp.Delete(lStart, lEnd - lStart);
				//=====================================================
				//2009.02.02 UPDATE BY PJW : 유니코드형식으로 변경
// 				cstmp.Insert(lStart + lStartOffset, pData);
				cstmp.Insert(lStart + lStartOffset + nFindCnt, pData);
				//=====================================================
				
				SetWindowText(cstmp);
				//OnCheckSyntax();
				return TRUE; break;
			case 88 :	//	Ctrl + 'X'	- Cut
				bIsValidKey = TRUE;
				break ;
			default :
				bIsValidKey = FALSE;
				break ;
			}
		}
		if( pMsg->wParam == VK_INSERT ) return TRUE; 
		if(!bIsValidKey) {
			switch( pMsg->wParam ) {
			case VK_SPACE: 
			case VK_DELETE:
			case VK_RETURN:
			case VK_BACK:
				bIsValidKey = TRUE; break;
			default :
				bIsValidKey= FALSE ;
				break ;
			}
		}

		
		if( pMsg->wParam >= 0x30 && pMsg->wParam <= 0x39 )		bIsValidKey = TRUE ;	// '0' ~ '9'
		else if( pMsg->wParam >= 0x41 && pMsg->wParam <= 0x5A )	bIsValidKey = TRUE ;	// 'A' ~ 'Z'
		else if( pMsg->wParam >= 0x60 && pMsg->wParam <= 0x6f )	bIsValidKey = TRUE ;	// numpad ; 96<pMsg->wParam<111
		else if( pMsg->wParam >= 0xba && pMsg->wParam <= 0xc0 )	bIsValidKey = TRUE ;	// 186<pMsg->wParam<192
		else if( pMsg->wParam >= 0xdb && pMsg->wParam <= 0xde )	bIsValidKey = TRUE ;	// 219<pMsg->wParam<222
		else if( pMsg->wParam == 0xe5 )	{	// 한글 입력시
//			m_TocEdit.GetWindowText( m_strBak ) ;
			bIsValidKey= TRUE ;
		}
		
		if(bIsValidKey) {
			GetSel(lStart, lEnd);
 				if( lStart == lEnd ) {
					switch( pMsg->wParam ) {
//					case VK_SPACE: 
					case VK_DELETE: lEnd = lStart+1; break;
					case VK_BACK: lStart = lEnd - 1; break;
					}
				}
				for(INT i=lStart; i<lEnd; i++) {
					if(!bChanged && IsMath(i)) {
						bChanged = TRUE;
						break;
					}

				}
			if(bChanged) { 
				if(IDYES != AfxMessageBox(IDS_MATH_DELETE, MB_YESNO) ) 
					return TRUE;
			}
		}
	}

	if ( pMsg -> message == WM_KEYDOWN ) {
		if ( pMsg -> wParam == VK_PRIOR || pMsg -> wParam == VK_NEXT )
			m_preScrollPos = GetScrollPos( SB_VERT ) ;
	} else if ( pMsg -> message == WM_RBUTTONDOWN ) {
//		if ( m_nIsKindOf==_YON_)
//			SendMessage( WM_LBUTTONDOWN, MK_LBUTTON, pMsg->lParam ) ;

	} else if ( pMsg -> message == WM_RBUTTONUP ) {
			CMenu menu;



			menu.CreatePopupMenu();

			if(!m_bSetCursor) {
				menu.AppendMenu(MF_STRING|MF_ENABLED, ID_CHECK_SYNTAX, _T("오류체크(&E)"));
				menu.AppendMenu(MF_SEPARATOR);
				menu.AppendMenu(MF_STRING|MF_ENABLED, ID_TOC_NEW, _T("새로작성(&N)"));
				menu.AppendMenu(MF_SEPARATOR);
				if ( ((CTocEditorDlg*)m_pParent)->ExistUndonRedo(TRUE) )
					menu.AppendMenu(MF_STRING|MF_ENABLED, ID_TOC_UNDO, _T("취소(&U)"));
				else
					menu.AppendMenu(MF_STRING|MF_GRAYED, ID_TOC_UNDO, _T("취소(&U)"));
				if ( ((CTocEditorDlg*)m_pParent)->ExistUndonRedo(FALSE) )
					menu.AppendMenu(MF_STRING|MF_ENABLED, ID_TOC_REDO, _T("다시실행(&R)"));
				else
					menu.AppendMenu(MF_STRING|MF_GRAYED, ID_TOC_REDO, _T("다시실행(&R)"));
				menu.AppendMenu(MF_SEPARATOR);

				if ( m_nIsKindOf==_YON_)
				{
//					menu.AppendMenu(MF_STRING|MF_ENABLED, MSG_EDIT_YON_CHOROK, _T("초록편집(&O)"));
				}
				else
				{
					menu.AppendMenu(MF_STRING|MF_ENABLED, ID_TOC_CUT, _T("잘라내기(&T)"));
					menu.AppendMenu(MF_STRING|MF_ENABLED, ID_TOC_PASTE, _T("붙여넣기(&P)"));
					menu.AppendMenu(MF_STRING|MF_ENABLED, ID_TOC_COPY, _T("복사(&C)"));
					menu.AppendMenu(MF_SEPARATOR);
//					menu.AppendMenu(MF_STRING|MF_ENABLED, MSG_EDIT_DAN_CHOROK, _T("초록편집(&O)"));
				}

				menu.AppendMenu(MF_STRING|MF_ENABLED, IDC_BUTTON_SOOSIKEDITOR, _T("수식편집기(&S)"));
				menu.AppendMenu(MF_STRING|MF_ENABLED, IDC_BUTTON_VIEWER, _T("뷰어(&V)"));
				menu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, pMsg->pt.x, pMsg->pt.y, this);
			}
			else {
				menu.AppendMenu(MF_STRING|MF_ENABLED, ID_MATH_DELETE, _T("삭제(&R)"));
				menu.AppendMenu(MF_STRING|MF_ENABLED, ID_MATH_EDIT, _T("편집(&N)"));
				menu.AppendMenu(MF_SEPARATOR);
				menu.AppendMenu(MF_STRING|MF_ENABLED, ID_MATH_TAGVIEW, _T("TAG 보기(&T)"));
				
				menu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, pMsg->pt.x, pMsg->pt.y, this);
			}
	}

	return CRichEditCtrl::PreTranslateMessage(pMsg);
}


VOID CRichEditCtrlEx::OnInitialUpdate()
{
	m_pRichEditOle = GetIRichEditOle();
	m_bitTooltip.Create(this);

	GetSelectionCharFormat(cfBase);
}

VOID CRichEditCtrlEx::SetKindOf(INT nIsKindOf, CWnd* pParent)
{
	m_nIsKindOf= nIsKindOf;
	m_pParent= pParent;
}

VOID CRichEditCtrlEx::OnUndo() 
{
	((CTocEditorDlg*)m_pParent)->OnUndo();
}

VOID CRichEditCtrlEx::OnRedo() 
{
	((CTocEditorDlg*)m_pParent)->OnRedo();
}

VOID CRichEditCtrlEx::OnTocCut() 
{
	if ( m_nIsKindOf==_YON_)
		((CTocEditorDlg*)m_pParent)->SendMessage(WM_KEYDOWN, 88, VK_CONTROL);		// 기사제어번호 때문에.
	else
		((CTocEditorDlg*)m_pParent)->OnTocCut();
}

VOID CRichEditCtrlEx::OnTocCopy() 
{
	((CTocEditorDlg*)m_pParent)->OnTocCopy();
}

VOID CRichEditCtrlEx::OnTocPaste() 
{
	((CTocEditorDlg*)m_pParent)->OnTocPaste();
}

VOID CRichEditCtrlEx::OnCheckSyntax() 
{
	((CTocEditorDlg*)m_pParent)->OnCheckSyntax();
}

VOID CRichEditCtrlEx::OnTocNewEdit()
{
	((CTocEditorDlg*)m_pParent)->TocNewEdit();
	
}

VOID CRichEditCtrlEx::OnSoosikEditor()
{
	OnMathNew();
//	((CTocEditorDlg*)m_pParent)->ExecuteSoosikEditor() ;
}

VOID CRichEditCtrlEx::OnViewer()
{
//	((CTocEditorDlg*)m_pParent)->ShowViewer() ;
	((CTocEditorDlg*)m_pParent)->SendMessage(MSG_SHOW_VIEWER) ;
}

// 리치에디트에서 오른쪽 버튼 팝업메뉴의 단행본 초록 편집
VOID CRichEditCtrlEx::OnEditDanChorok()
{
	((CTocEditorDlg*)m_pParent)->SendMessage(MSG_EDIT_DAN_CHOROK) ;
}

// 리치에디트에서 오른쪽 버튼 팝업메뉴의 연간물 초록 편집
VOID CRichEditCtrlEx::OnEditYonChorok()
{
	((CTocEditorDlg*)m_pParent)->SendMessage(MSG_EDIT_YON_CHOROK) ;
}

VOID CRichEditCtrlEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	RECT rect;
//	RECT rectTooltip;
	BOOL m_bCursor = FALSE;
	CDC* pDC = GetDC();
/*
	for( INT i=0;i<GetMathCnt();i++) {
		rect = GetMathRect(i);
		pDC->Rectangle(&rect); }
*/
	for( INT i=0;i<GetMathCnt();i++) {
		rect = GetMathRect(i);

		if(( (point.x > rect.left) && (point.x <  rect.right) ) &&
			( (point.y > rect.top) && (point.y <  rect.bottom) )) {
			
			m_nMathSelected = i;
			m_bCursor = TRUE;
			
			if(point == m_prepoint) break;

			
			m_prepoint = point;

			m_bitTooltip.Show( point, m_mathlist.GetMath(m_nMathSelected));
		
		}
	}

	if(m_bCursor)
		m_bSetCursor = TRUE;
	else {
		m_bSetCursor = FALSE; 
		if(m_bitTooltip.m_bActive) m_bitTooltip.Close();
	}

	CRichEditCtrl::OnMouseMove(nFlags, point);
}

VOID CRichEditCtrlEx::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
		// TODO: Add your message handler code here and/or call default
/*	CString cstmp;
	if(m_bSetCursor) {
		cstmp.Format(_T("%d, index:%d"), m_nMathSelected, m_mathlist.GetMathIndex(m_nMathSelected));
		AfxMessageBox(cstmp);
	}
*/
	CRichEditCtrl::OnLButtonDblClk(nFlags, point);
}


INT CRichEditCtrlEx::GetMathSit()
{
	REOBJECT reobj;
	long lStart, lEnd; INT lCnt=0;

	ZeroMemory(&reobj, sizeof(REOBJECT));
	reobj.cbStruct = sizeof(REOBJECT);

	GetSel(lStart, lEnd);
//	lStart = 0L;
//	SetSel(lStart, lEnd);
	for(INT i=0;i<GetMathCnt();i++) {
		if( !FAILED(m_pRichEditOle->GetObject( i, &reobj, REO_GETOBJ_POLEOBJ)) )
				if(reobj.cp < lEnd) lCnt++;
	}  return lCnt;
	
}

INT CRichEditCtrlEx::GetMathSit(INT nMath)
{
	REOBJECT reobj;
	long lStart =0, lEnd = -1;
	INT lCnt=0;

	ZeroMemory(&reobj, sizeof(REOBJECT));
	reobj.cbStruct = sizeof(REOBJECT);

//	GetSel(lStart, lEnd);
//	lStart = 0L;
//	SetSel(lStart, lEnd);
	//for(INT i=0;i<GetMathCnt();i++) {
	if( !FAILED(m_pRichEditOle->GetObject( nMath, &reobj, REO_GETOBJ_POLEOBJ)) )
		return (INT)reobj.cp;
	else return -1;
}

INT CRichEditCtrlEx::GetMathStringSize(INT nMath)
{
//	if(!IsMath(nMath)) return -1;
	
	CString csfilename;
	
	CFile cf; INT nSize;

	TCHAR m_exeDir[256];
	GetCurrentDirectory( 256 , m_exeDir ) ;
	CString sWorkDir = m_exeDir ;
	sWorkDir.TrimRight(_T("\\"));
	INT idx= sWorkDir.ReverseFind( _T('\\') );
	if ( idx<=0 ) return -2;
	sWorkDir= sWorkDir.Mid(0,idx) ;
	sWorkDir += PATH_MATH_TMP ;
	csfilename.Format(_T("%s\\%s%03d.html"), sWorkDir, MATH_TMP_FILENAME, nMath+1);
	if(cf.Open(csfilename, CFile::modeRead | CFile::typeBinary)) {
		nSize = cf.GetLength();
		cf.Close();
		return nSize;
	}
	return -3;
	
}


INT CRichEditCtrlEx::InsertImg(UINT nSize, LPCTSTR pstrFilename)
{
	BITMAP		bitmapbits;
//	RECT		rect;
	CBitmap*	mybitmap; CPalette pPal;
//	HBITMAP		hbitmap;
	LONG		lStart, lEnd;
	INT nMath = GetMathSit();
	mybitmap = m_mathlist.InsertMath(nMath, nSize * 100);
//	m_sizel.cy = nSize * 100;
//	m_sizel.cx = m_sizel.cy * bitmap.bmWidth / bitmap.bmHeight;
	GetSel(lStart, lEnd);
	for(INT i=lStart;i<lEnd;i++) if(IsMath(i)) return -1;
	LoadBMP(pstrFilename, *mybitmap, &pPal);
	mybitmap->GetBitmap(&bitmapbits);
	m_mathlist.findMath(nMath)->nHeight = nSize * 100; // * 100;
	m_mathlist.findMath(nMath)->nWidth = nSize * 100 * bitmapbits.bmWidth / bitmapbits.bmHeight;

	
	if ((HBITMAP)*mybitmap)
	{
		CImageDataObject::InsertBitmap(m_pRichEditOle, (HBITMAP)*mybitmap, bitmapbits, nSize * 100);
	}
	
		
	return i;
}

BOOL CRichEditCtrlEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if ( m_bSetCursor )
	{
		SetCursor (AfxGetApp()->LoadCursor (IDC_MYCURSOR));
		return 1 ;		// 반드시...
	}

	return CRichEditCtrl::OnSetCursor(pWnd, nHitTest, message);
}


/*
VOID CRichEditCtrlEx::GetOle()
{
	REOBJECT reobj;

	ZeroMemory(&reobj, sizeof(REOBJECT));
	reobj.cbStruct = sizeof(REOBJECT);

	CHARRANGE cr;
	cr.cpMax = -1;
	cr.cpMin = 0;

//	SetSel(cr);

	IStream *ppstrm;
	CPoint point;
//	LONG dd = m_pRichEditOle->GetLinkCount();
	INT i=0;
	while(!FAILED(m_pRichEditOle->GetObject( i, &reobj, REO_GETOBJ_POLEOBJ)) ) {
		point = GetCharPos(reobj.cp);
		i++;
	}
	
	CString cstmp;
	cstmp.Format(_T("%d"), i);
	AfxMessageBox(cstmp);

	return ;
}
*/

INT CRichEditCtrlEx::GetMathCnt()
{
	REOBJECT reobj;

	ZeroMemory(&reobj, sizeof(REOBJECT));
	reobj.cbStruct = sizeof(REOBJECT);
	if(!m_pRichEditOle) return -1;
	INT i=0; 
	while(!FAILED(m_pRichEditOle->GetObject( i, &reobj, REO_GETOBJ_POLEOBJ)) )
		i++;
	
	return i;
}

RECT CRichEditCtrlEx::GetMathRect(INT nMath)
{
	REOBJECT reobj;
	RECT rect;
//	SIZEL sizel;
	INT nLine = 0; //, nLineFor;
	LONG lStartChar, lEndChar;
	LONG lMaxHeight = 0, lMinHeight = 0;
	

	ZeroMemory(&reobj, sizeof(REOBJECT));
	reobj.cbStruct = sizeof(REOBJECT);
	memset(&rect, 0, sizeof(RECT));

	CPoint point, pointtmp; INT nIndex = 0;

	if( !FAILED(m_pRichEditOle->GetObject( nMath, &reobj, REO_GETOBJ_POLEOBJ)) ) {
		point = GetCharPos(reobj.cp);
		//nLine = LineIndex(reobj.cp);
		//nLine = LineFromChar(reobj.cp);
		//lStartChar = LineIndex(nLine);
		for(nLine = 0; nLine<GetLineCount(); nLine++) {
			nIndex = LineIndex(nLine);
			if(nIndex > reobj.cp) break;
		}
		lStartChar = LineIndex(nLine-1); lEndChar = LineLength(nLine -1) + lStartChar;
		if(nLine >= GetLineCount()) lEndChar = GetTextLength();
		else lEndChar = LineIndex(nLine) - 2;
		
		

//		rect.right = reobj.sizel.cx / 26;
//		rect.bottom = reobj.sizel.cy / 26;
		
//		rect.left = point.x;
//		rect.top = point.y;
		INT nMaxcnt = GetMathCnt();
		for(INT i=0;i<=nMaxcnt;i++) {
			if( !FAILED(m_pRichEditOle->GetObject( i, &reobj, REO_GETOBJ_POLEOBJ)) ) {
				pointtmp = GetCharPos(reobj.cp);
				if( (lStartChar <= reobj.cp && lEndChar >= reobj.cp) )
					if( lMaxHeight < (m_mathlist.findMath(i)->nHeight / 26 ) )
						// MaxHeight = reobj.sizel.cy / 26;
						lMaxHeight = m_mathlist.findMath(i)->nHeight / 26;
//					else if( lMinHeight > pointtmp.y )
//						lMinHeight = m_mathlist.findMath(i)->nHeight / 26;
			}
		}
		m_pRichEditOle->GetObject( 0, &reobj, REO_GETOBJ_POLEOBJ );
/*		rect.left = point.x;
		rect.top = point.y + ( lMaxHeight - m_mathlist.findMath(nMath)->nHeight / 26 );

		

		rect.bottom = rect.top + m_mathlist.findMath(nMath)->nHeight / 26;
		rect.right = rect.left + m_mathlist.findMath(nMath)->nWidth / 26;
*/		INT nHeight = m_mathlist.findMath(nMath)->nHeight / 26;
		rect.left = point.x;
		rect.top = point.y + lMaxHeight - nHeight;

		

		rect.bottom = rect.top + nHeight;
		rect.right = rect.left + m_mathlist.findMath(nMath)->nWidth / 26;

//		rect.left = point.x;
//		rect.top = point.y + 10;// + lMaxHeight - rect.bottom;

		
	}
			
	return rect;
}

BOOL CRichEditCtrlEx::IsMath(INT nChar)
{
	REOBJECT reobj;
	ZeroMemory(&reobj, sizeof(REOBJECT));
	reobj.cbStruct = sizeof(REOBJECT);
	for(INT i=0;i<GetMathCnt(); i++) {

		if( !FAILED(m_pRichEditOle->GetObject( i, &reobj, REO_GETOBJ_POLEOBJ)) )
			if(reobj.cp == nChar) return TRUE;
	}
	return FALSE;
}

VOID CRichEditCtrlEx::OnMathDelete()
{
//	INT lMathSit;
	REOBJECT reobj;

	ZeroMemory(&reobj, sizeof(REOBJECT));
	reobj.cbStruct = sizeof(REOBJECT);

	if(FAILED(m_pRichEditOle->GetObject( m_nMathSelected, &reobj, REO_GETOBJ_POLEOBJ)) )
		AfxMessageBox(_T("수식 지우기 실패"));
	else {
//		m_mathlist.DeleteMath(m_nMathSelected);
		m_mathlist.deleteMath(m_nMathSelected);
		SetSel(reobj.cp, reobj.cp+1);
		ReplaceSel(NULL);
	}
}


BOOL CRichEditCtrlEx::ExecuteProc(LPCTSTR szstr, LPCTSTR szstrcmd, BOOL bOnly)
{

	CString cstmp;	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	BOOL bOK;

	
	GetStartupInfo( &si );

	si.wShowWindow = SW_SHOW;
	si.dwFlags     = STARTF_USESHOWWINDOW;
	
	if(szstrcmd)
		cstmp.Format(_T("%s %s"), szstr, szstrcmd);
	else cstmp.Format(_T("%s"), szstr);
//	ShellExecute(NULL, NULL, szstr, (TCHAR *)szstrcmd,  NULL, 0);
//	return TRUE;

	bOK = CreateProcess( NULL, (TCHAR *)(LPCTSTR)cstmp, 0, 0, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi );
//	bOK = CreateProcess( szstr, (TCHAR *)szstrcmd, 0, 0, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi );
//	NULL,NULL,FALSE,0,NULL,NULL
	if ( !bOK ){
//		AfxMessageBox(IDS_ERROR_MATH_RUN);
		return FALSE;
	}
	ShowWindow(SW_HIDE);

	CloseHandle( pi.hThread  );

	if(bOnly) WaitForSingleObject( pi.hProcess, INFINITE );
	CloseHandle( pi.hProcess );
	ShowWindow(SW_SHOW);

	return TRUE;
}

VOID CRichEditCtrlEx::OnMathEdit()
{
	TCHAR m_exeDir[256];
	GetCurrentDirectory( 256 , m_exeDir ) ;
	CString cstmp;
	CString csfilename, csfilename2;
	CString sWorkDir = m_exeDir;
	sWorkDir.TrimRight(_T("\\"));
	INT idx= sWorkDir.ReverseFind( _T('\\') ) ;
	if ( idx<=0 ) {
//		AfxMessageBox( _T("cfg 파일의 경로를 얻을수가 없습니다.\r\nTAG 에 색을 입힐 수 없습니다!") , MB_ICONERROR | MB_OK ) ;
		return ;
	}
	sWorkDir= sWorkDir.Mid(0,idx) ;
	csfilename2.Format(_T("%s\\temp.txt"), m_exeDir);
//	sWorkDir += _T("\\cfg") ;
	sWorkDir += PATH_MATH_TMP ;


	DeleteFile(csfilename2);

	INT nMathSit, nMathSize, nStrSize;
	long lStart = 0, lStartOffset = 0, lEnd = 0;

	GetSel(lStart, lEnd);
	lStartOffset = 0;
	nStrSize = lEnd - lStart ;

	if(m_nMathSelected < 0) return;
	csfilename.Format(_T("%s\\%s%03d.html"), sWorkDir, MATH_TMP_FILENAME, m_nMathSelected + 1);

	
	CFile fp;	TCHAR *lpBuffer;
	if(!fp.Open(csfilename, CFile::modeRead | CFile::typeBinary)) return;
	TCHAR cUni;
	fp.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fp.SeekToBegin();
	}
	
	DWORD dSize = fp.GetLength();

	lpBuffer = (TCHAR *)malloc(sizeof(TCHAR) * dSize + sizeof(TCHAR));
	fp.ReadHuge(lpBuffer, dSize);
	lpBuffer[dSize] = '\0';
	fp.Close(); cstmp.Format(_T("%s"), lpBuffer);
	
	CMathConvertor m_Math;
	m_Math.RemoveObjectTag(&cstmp);


//	csfilename.Format(_T("c:\Toctemp.html"));
	if(fp.Open(csfilename2+_T("2"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)) {
		if( 0 == fp.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			fp.Write( &cUni, sizeof(TCHAR));
		}
		
		fp.Write( cstmp, cstmp.GetLength() );
		fp.Close();
	}

	
	if(!ExecuteProc(ECOMATHEDITOR, csfilename2+_T("2"), TRUE))
		AfxMessageBox(IDS_ERROR_MATH_RUN);
	GetWindowText(cstmp);

	for(INT i=0;i<GetMathCnt();i++) {
		nMathSit = GetMathSit(i);
		nMathSize = GetMathStringSize(i) + 4;
		if( nMathSit < lStart)
			lStartOffset+= nMathSize - 1;
		else if( nMathSit < lEnd )	nStrSize+= nMathSize - 1;
	}
			
	GetWindowText(cstmp);
	

	TCHAR *lpBuffer2;
//	csfilename2.Format(_T("%s\\temp.txt"), sWorkDir);
	if(!fp.Open(csfilename2, CFile::modeRead | CFile::typeBinary)) return;
	
	fp.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fp.SeekToBegin();
	}
	
	dSize = fp.GetLength();

	lpBuffer2 = (TCHAR *)malloc(sizeof(TCHAR) * dSize + sizeof(TCHAR));
	fp.ReadHuge(lpBuffer2, dSize);
	lpBuffer2[dSize] = '\0';
	fp.Close();


//	INT ddd = GetMathStringSize(m_nMathSelected);
//	cstmp.Delete(lStart + lStartOffset + 2, ddd );
//	cstmp.Insert(lStart + lStartOffset + 2, lpBuffer);
//	AfxMessageBox(cstmp);
	CFileFind mm;
	if(mm.FindFile(csfilename2)) {
		cstmp.Replace(lpBuffer, lpBuffer2);
		mm.Close();
	}
	
	SetWindowText(cstmp);
	free(lpBuffer);
	free(lpBuffer2);
//	ExecuteProc(_TEXT(_T("Copy")), _TEXT(_T(" /y temp.txt ")) + csfilename, FALSE );

}



VOID CRichEditCtrlEx::OnMathNew()
{
	CString cstmp;
	INT nMathSit, nMathSize, nStrSize;
	long lStart = 0, lStartOffset = 0, lEnd = 0;

	TCHAR m_exeDir[256];
	GetCurrentDirectory( 256 , m_exeDir ) ;

	CString csfilename, csfilename2;

	csfilename2.Format(_T("%s\\temp.txt"), m_exeDir);


	DeleteFile(csfilename2);

	GetSel(lStart, lEnd);

	if(!ExecuteProc(ECOMATHEDITOR, NULL, TRUE))
		AfxMessageBox(IDS_ERROR_MATH_RUN);
//	GetWindowText(cstmp);

/*	for(INT i=0;i<GetMathCnt();i++) {
		nMathSit = GetMathSit(i);
		nMathSize = GetMathStringSize(i) + 4;
		if( nMathSit < lStart)
			lStartOffset+= nMathSize - 1;
		else if( nMathSit < lEnd )	nStrSize+= nMathSize - 1;
	}
			
	GetWindowText(cstmp);
	
*/
	//===================================================
	//2010.05.02 ADD BY PJW : 
//	TCHAR *lpBuffer;
//	CFile fp;
//	if(!fp.Open(csfilename2, CFile::modeRead | CFile::typeBinary)) return;
//	fp.SeekToBegin();
//	
//	DWORD dSize = fp.GetLength();
//
//	lpBuffer = (TCHAR *)malloc(sizeof(TCHAR) * (dSize ) + sizeof(TCHAR));
//	fp.ReadHuge(lpBuffer, dSize);
//	lpBuffer[dSize] = '\0';
//	fp.Close();
	CHAR *cpData;
	CFile fp;
	if(!fp.Open(csfilename2, CFile::modeRead | CFile::typeBinary)) return;
	fp.SeekToBegin();
	
	DWORD dSize = fp.GetLength();

	cpData = (CHAR *)malloc(sizeof(CHAR) * (dSize ));
	fp.ReadHuge(cpData, dSize);
	cpData[dSize] = '\0';
	fp.Close();

	CharConvert cvt;
	TCHAR *lpBuffer;
	INT nNeedByte;
	nNeedByte = MultiByteToWideChar(
									CP_ACP				, 
									0					,
									cpData		, 
									-1					,
									NULL				, 
									NULL );
	lpBuffer = (TCHAR *)malloc( sizeof( TCHAR )*nNeedByte+sizeof( TCHAR ) );
	cvt.MultiByteToUnicode(cpData, &lpBuffer, &nNeedByte );	
	//===================================================

/*
	lStartOffset = 0;
	nStrSize = lEnd - lStart ;
	for(i=0;i<GetMathCnt();i++)  {
		nMathSit = GetMathSit(i) ;
		nMathSize = GetMathStringSize(i) + 4;
		if( nMathSit < lStart)
			lStartOffset+= nMathSize - 1;
		else if( nMathSit < lEnd )	nStrSize+= nMathSize - 1;
	}
			
	GetWindowText(cstmp);
	cstmp2 = cstmp.Mid(lStart + lStartOffset, nStrSize );
//				AfxMessageBox(cstmp2);
*/
	if( ::OpenClipboard(m_hWnd) ) {
		::EmptyClipboard();
		//=====================================================
		//2009.02.02 UPDATE BY PJW : 유니코드형식으로 변경
// 	 	HANDLE hData = ::GlobalAlloc(GMEM_MOVEABLE, dSize + 1);
		HGLOBAL hData = ::GlobalAlloc(GMEM_MOVEABLE, (dSize + 1)*sizeof(TCHAR));
		//=====================================================
		
#ifdef _UNICODE
		LPWSTR pData = (LPWSTR) ::GlobalLock(hData);
		::lstrcpy(pData, (LPCTSTR)lpBuffer);
		::GlobalUnlock(hData);

		::SetClipboardData(CF_UNICODETEXT, hData);
		::CloseClipboard();
#else
		LPCTSTR pData = (LPCTSTR) ::GlobalLock(hData);
		::lstrcpy(pData, (LPCTSTR)lpBuffer);
		::GlobalUnlock(hData);

		::SetClipboardData(CF_TEXT, hData);
		::CloseClipboard();
#endif
	}

	SetSel(lStart, lEnd);
	Paste();
	m_nMaxMathCnt++;

	GetWindowText(cstmp);

//	cstmp.Insert(lStart + 2, lpBuffer);

	SetWindowText(cstmp);
	free(lpBuffer);
//	ExecuteProc(_TEXT(_T("Copy")), _TEXT(_T(" /y c:\\temp.txt ")) + csfilename, FALSE );

}

BOOL CRichEditCtrlEx::GetMathTag(INT nChar, CString *str, CString *strTag, INT *nStart)
{
	INT nEnd = 0; *nStart = 0;
	if(nChar < 0) return FALSE;

	for(INT i=0;i<nChar;i++) {
		*nStart	= str->Find(ECOMATH_START_TAG, nEnd);
		nEnd	= str->Find(ECOMATH_END_TAG, *nStart + 2);
		if( *nStart < 0 || nEnd < 0 ) return FALSE;		
		if( i + 1 == nChar ) *strTag= str->Mid(*nStart , nEnd - *nStart + sizeof(ECOMATH_END_TAG) -1);
		nEnd+=1;
//		AfxMessageBox(*strTag);
	}
	return TRUE;
}

BOOL CRichEditCtrlEx::LoadBMP( LPCTSTR sBMPFile, CBitmap& bitmap, CPalette *pPal )
{
	CFile file;
	if( !file.Open( sBMPFile, CFile::modeRead | CFile::typeBinary) )
		return FALSE;
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	BITMAPFILEHEADER bmfHeader;

	// Read file header
	if (file.Read(&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return FALSE;

	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
		return FALSE;

	// Get length of the remainder of the file and allocate memory
	DWORD nPackedDIBLen = file.GetLength() - sizeof(BITMAPFILEHEADER);
	HGLOBAL hDIB = ::GlobalAlloc(GMEM_FIXED, nPackedDIBLen);
	if (hDIB == 0)
		return FALSE;

	// Read the remainder of the bitmap file.
	if (file.ReadHuge(hDIB, nPackedDIBLen) != nPackedDIBLen )
	{
		::GlobalFree(hDIB);
		return FALSE;
	}


	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;

	// If bmiHeader.biClrUsed is zero we have to infer the number
	// of colors from the number of bits used to specify it.
	INT nColors = bmiHeader.biClrUsed ? bmiHeader.biClrUsed : 
						1 << bmiHeader.biBitCount;

	LPVOID lpDIBBits;
	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + bmInfo.bmiHeader.biClrUsed) + 
			((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);

	// Create the logical palette
	if( pPal != NULL )
	{
		// Create the palette
		if( nColors <= 256 )
		{
			UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
			LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

			pLP->palVersion = 0x300;
			pLP->palNumEntries = nColors;

			for( INT i=0; i < nColors; i++)
			{
				pLP->palPalEntry[i].peRed = bmInfo.bmiColors[i].rgbRed;
				pLP->palPalEntry[i].peGreen = bmInfo.bmiColors[i].rgbGreen;
				pLP->palPalEntry[i].peBlue = bmInfo.bmiColors[i].rgbBlue;
				pLP->palPalEntry[i].peFlags = 0;
			}

			pPal->CreatePalette( pLP );

			delete[] pLP;
		}
	}

	CClientDC dc(NULL);
	CPalette* pOldPalette = NULL;
	if( pPal )
	{
		pOldPalette = dc.SelectPalette( pPal, FALSE );
		dc.RealizePalette();
	}

	HBITMAP hBmp = CreateDIBitmap( dc.m_hDC,		// handle to device context 
				&bmiHeader,	// pointer to bitmap size and format data 
				CBM_INIT,	// initialization flag 
				lpDIBBits,	// pointer to initialization data 
				&bmInfo,	// pointer to bitmap color-format data 
				DIB_RGB_COLORS);		// color-data usage 
	bitmap.Attach( hBmp );

	if( pOldPalette )
		dc.SelectPalette( pOldPalette, FALSE );

	::GlobalFree(hDIB);
	return TRUE;
}

INT CRichEditCtrlEx::Convert2ViewerTag(CString &sResult, BOOL bColor)
{

	CString sText;
	INT iMaxCount = GetLineCount();
	INT iLineLength = 0;
	INT nChangedChar = 0;
	CStringArray m_saHeadTag;
	CStringArray m_saTailTag;
	CStringArray m_saConverted;



//	cfg 파일 경로설정.
	TCHAR m_exeDir[256];
	GetCurrentDirectory( 256 , m_exeDir ) ;
	CString sWorkDir = m_exeDir ;
	sWorkDir.TrimRight(_T("\\"));
	INT idx= sWorkDir.ReverseFind( '\\' ) ;
	if ( idx<=0 ) {
//		AfxMessageBox( _T("cfg 파일의 경로를 얻을수가 없습니다.\r\nTAG 에 색을 입힐 수 없습니다!") , MB_ICONERROR | MB_OK ) ;
		return 0;
	}
	sWorkDir= sWorkDir.Mid(0,idx) ;
	sWorkDir += PATH_CFG ;
	sWorkDir += _T("\\");
	sWorkDir += sViewerFileName;

	CStdioFile m_ViewerCfgFile;
	CFileException ex;
	if( ! m_ViewerCfgFile.Open(sWorkDir, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary , &ex) ) {
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		AfxMessageBox(szError) ;
		return 0;
	}
	TCHAR cUni;
	m_ViewerCfgFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		m_ViewerCfgFile.SeekToBegin();
	}
	

	CString sHeadTag = _T("") ;
	CString sTailTag = _T("") ;
	CString sConv = _T("") ;
	CString sRecord;
	

	if ( m_nIsKindOf )		// 목차일 경우에만 적용시키자.
	{
		while (m_ViewerCfgFile.ReadString(sRecord))	// 태그를 치환해 주자.
		{
			sRecord.TrimLeft();
			sRecord.TrimRight();

			if(sRecord.IsEmpty()) continue;

			// =================================================================================================
			// 주석 제거
			// =================================================================================================
			INT iCurIdx = sRecord.Find(_T(".."));
			if(iCurIdx >= 0) {
				sRecord = sRecord.Left(iCurIdx);
				sRecord.TrimLeft();
				sRecord.TrimRight();

				if(sRecord.IsEmpty()) {
					continue;
				}
			}

			// =================================================================================================
			// 레코드 형식 [TAG] | [태그이름]
			// =================================================================================================
			CParse tokenParse(sRecord.GetBuffer(0));
			tokenParse.SetDelimeter(_T("|"));

			const INT iElementCnt = 2;
			CString sElement[iElementCnt];

			for(INT i = 0 ; tokenParse.HasMoreToken() && i < iElementCnt ; i++)
			{
				sElement[i] = tokenParse.NextToken();
				sElement[i].TrimLeft();
				sElement[i].TrimRight();
			}

			sHeadTag= _T("<") + sElement[0] + _T(">") ;
			sTailTag= _T("</") + sElement[0] + _T(">") ;
			sConv= CONVERT_HEAD_TAG ;	// _T("[") 
			sConv+=	sElement[1] ;
			sConv+= CONVERT_END_TAG ;	// _T("] ")
			sConv+= _T(" ") ;

			m_saConverted.Add( sConv ) ;
			m_saHeadTag.Add( sHeadTag );
			m_saTailTag.Add( sTailTag );

			sHeadTag.MakeLower() ;	sTailTag.MakeLower() ;
		}

	}

	long lStart = 0;
	for(INT i=0; i<m_saHeadTag.GetSize(); i++) {
		if(!bColor) {
			sResult.Replace( m_saHeadTag[i], m_saConverted[i] );
			sResult.Replace( m_saTailTag[i], _T("") );
		}
		else {
			while((lStart = sResult.Find(m_saConverted[i], lStart)) > -1 ) {
				SetSel(lStart, m_saConverted[i].GetLength() + lStart);
				CHARFORMAT cf;
				GetSelectionCharFormat(cf);
				
				cf.dwEffects = CFE_BOLD;
				cf.crTextColor = RGB_TAG_COLOR;

				SetSelectionCharFormat(cf);
				lStart = m_saConverted[i].GetLength() + lStart;
			}
		lStart = 0;
		}
	}

	// 우선 기사제어번호 태그부터 없애준다.
	INT nFindStart= 0;
	INT nFindEnd = 0;
	while(1) {
		nFindStart = sResult.Find(_START_CHOROK_KEY_);
		if(nFindStart < 0 ) break;
		if ( nFindStart >= 0 )
		{
			nFindEnd = sResult.Find(_END_CHOROK_KEY_) ;
			if ( nFindEnd >= 0 )
			{
				if(nFindEnd-nFindStart != 12) break;
				sResult.Delete( nFindStart, nFindEnd-nFindStart + 3) ;
			}
		}
	}


	return 0;
}

VOID CRichEditCtrlEx::OnMathTagView()
{
	CString csfilename;
	TCHAR m_exeDir[256];
	GetCurrentDirectory( 256 , m_exeDir ) ;
	CString sWorkDir = m_exeDir ;
	sWorkDir.TrimRight(_T("\\"));
	INT idx= sWorkDir.ReverseFind( _T('\\') );
	if ( idx<=0 ) {
		return ;
	}
	sWorkDir= sWorkDir.Mid(0,idx) ;
	sWorkDir += PATH_MATH_TMP ;

	csfilename.Format(_T("%s\\%s%03d.html"), sWorkDir, MATH_TMP_FILENAME, m_nMathSelected + 1);
	::ShellExecute(NULL, _T("open"), _T("notepad"), csfilename, NULL, SW_SHOWNORMAL);

}
