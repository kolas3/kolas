// MarcEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MarcEditCtrl.h"
#include "MarcEditor.h"
#include "DlgSymbolColor.h"

//#include _T("..\..\..\관리\전거\ACMarcGetter\ACMARCGetterDlg.h")

//#include _T("..\..\..\..\관리\전거\MNG_AC_MANAGER\ACMARCGetterDlg.h")

#include "..\..\esl\LIB_F2KEY_SpecialChar.h"
#include "..\..\esl\ConvertToHanjaDlg.h"
#include "CharConvert.h"

#include "efs.h"
#include <Htmlhelp.h>

// AutoList
#include <imm.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_LINE_LOCATION	WM_USER + 100

// 16/05/19 박대우 : 다국어(캄보디아어) 마크처리 에러수정
//*/ ADD -------------------------------------------------------------------------------------------------
#define _IS_DIGIT(CH)		(CH>=48 && CH<=57)
//*/ END -------------------------------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// CMarcEditCtrl
CMarcEditCtrl::CMarcEditCtrl()
{
	m_hinstRichEdit2 = LoadLibrary(_T("RICHED20.DLL"));	

	m_pParentEditor = NULL;
	m_pMarcMgr = NULL;
	m_pMarc = NULL;
	m_bIsAnalyzer = FALSE;

	m_pDlgMarcSyntaxError = NULL;		// error dialog

	m_changeType = ctUndo;
	
	m_bInForcedChange = FALSE;
	
	// set format..
	SetTextColor(RGB(0, 0, 0), FALSE);
	SetNumberColor(RGB(0, 0, 0), FALSE);
	SetTagCodeColor(RGB(0, 0, 255), FALSE);
	SetIndicatorCodeColor(RGB(255, 0, 255), FALSE);
	SetSubfieldCodeColor(RGB(255, 0, 128), FALSE);
	SetFieldTerminatorColor(RGB(0, 255, 0), FALSE);
	SetRecordTerminatorColor(RGB(0, 255, 0), FALSE);
	SetErrorColor(RGB(255, 0, 0), FALSE);

	m_clrBkColor = RGB(255, 255, 255);
	
	// create marc syntax error dialog
	CreateSyntaxErrorDialog();

	m_strEditorFormatConfFile = _T("..\\cfg\\MarcEditor\\EditorFormat.cfg");
	m_strSubfieldWrapConfFile = _T("..\\cfg\\MarcEditor\\SubfieldWrap.cfg");

	m_pFRDlg = NULL;

	ZeroMemory(&m_osvi, sizeof(OSVERSIONINFOEX));
	m_osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx ((OSVERSIONINFO*) &m_osvi);
	
	// auto list	
	m_FocusFlag = FALSE;
	m_bAutoListFocus = FALSE;
	m_ctlAutoList = NULL;
	m_bAltKey = FALSE;
	m_nType = -1;
	m_strSubfieldCode.Format(_T("%c"), 31);
	m_hLinkCursor = AfxGetApp()->LoadCursor (IDC_LINK);	//Should have the hand cursor in resource	

	// 2009.05.14 ADD BY PDJ : 마크보기에서는 무조건 나오도록.
	m_bAutoComplete = TRUE;
}

VOID CMarcEditCtrl::PreSubclassWindow() 
{
	EFS_BEGIN
	

	// para
	PARAFORMAT pf;	
	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_TABSTOPS;
	pf.cTabCount = MAX_TAB_STOPS;
	for (INT iTab = 0; iTab < pf.cTabCount; iTab++)
		pf.rgxTabs[iTab] = (iTab + 1) * 1440 / 5;

	SetParaFormat(pf);
	
	// character	
	m_cf.cbSize = sizeof(m_cf);
	m_cf.dwMask = CFM_BOLD | CFM_ITALIC | CFM_SIZE | CFM_CHARSET | CFM_PROTECTED | CFM_FACE;
	m_cf.dwEffects = CFE_PROTECTED;
	m_cf.yHeight = 200;
	m_cf.bCharSet = DEFAULT_CHARSET;	

	strcpy(m_cf.szFaceName, "새굴림");		

	SetDefaultCharFormat(m_cf);
	SetEventMask(ENM_CHANGE | ENM_SELCHANGE | ENM_PROTECTED);

	LoadEditorFormatConfig();
	LoadSubfieldWrapConfig();

	CRichEditCtrl::PreSubclassWindow();

	EFS_END
}

VOID CMarcEditCtrl::LoadSubfieldWrapConfig()
{
	EFS_BEGIN

	CStdioFile file;
	if (!file.Open(m_strSubfieldWrapConfFile, CFile::modeRead | CFile::typeBinary))
		return;

	CString *pStrAlias = NULL;
	
	INT nFind = -1;
	CString strLine;
	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.IsEmpty() || strLine.Left(1) == _T("#")) continue;

		strLine.Replace('$', SUBFIELD_CODE);

		pStrAlias = new CString;
		if (pStrAlias == NULL) continue;
		pStrAlias->Format(_T("%s"), strLine);
		m_listSubfieldWordWrap.AddTail((CObject*)pStrAlias);
	}

	file.Close();

	EFS_END
}

BOOL CMarcEditCtrl::IsWrapSubfield(CString strAlias)
{
	EFS_BEGIN

	if (strAlias.IsEmpty()) return FALSE;

	CString *pStr;
	POSITION pos = m_listSubfieldWordWrap.GetHeadPosition();
	while(pos)
	{
		pStr = (CString*)m_listSubfieldWordWrap.GetNext(pos);
		if (pStr == NULL) continue;

		if (pStr->CompareNoCase(strAlias) == 0) return TRUE;
	}

	return FALSE;

	EFS_END
	return FALSE;

}

VOID CMarcEditCtrl::LoadEditorFormatConfig()
{
	EFS_BEGIN

	CStdioFile file;
	if (!file.Open(m_strEditorFormatConfFile, CFile::modeRead | CFile::typeBinary))
		return;

	CString strKey;
	CString strLParam;
	CString strRParam;

	INT nFind = -1;
	CString strLine;
	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.IsEmpty() || strLine.Left(1) == _T("#")) continue;


		nFind = strLine.Find(_T("="));
		if (nFind == -1) continue;

		strKey = strLine.Left(nFind);
		strKey.TrimLeft();
		strKey.TrimRight();

		strLine = strLine.Mid(nFind + 1);
		strLine.TrimLeft();
		strLine.TrimRight();

		nFind = strLine.Find(_T(","));
		if (nFind < 0) continue;

		strLParam = strLine.Left(nFind);
		strRParam = strLine.Mid(nFind + 1);

		strLParam.TrimLeft();
		strLParam.TrimRight();

		strRParam.TrimLeft();
		strRParam.TrimRight();

		if (strKey.CompareNoCase(_T("BK_COLOR")) == 0)
		{
			SetBkColor(_ttoi(strLParam), _ttoi(strRParam));
		}
		else if (strKey.CompareNoCase(_T("TEXT")) == 0)
		{
			SetTextColor(_ttoi(strLParam), _ttoi(strRParam));
		}
		else if (strKey.CompareNoCase(_T("NUMBER")) == 0)
		{
			SetNumberColor(_ttoi(strLParam), _ttoi(strRParam));
		}
		else if (strKey.CompareNoCase(_T("TAG_CODE")) == 0)
		{
			SetTagCodeColor(_ttoi(strLParam), _ttoi(strRParam));
		}
		else if (strKey.CompareNoCase(_T("INDICATOR")) == 0)
		{
			SetIndicatorCodeColor(_ttoi(strLParam), _ttoi(strRParam));
		}
		else if (strKey.CompareNoCase(_T("SUBFIELD_CODE")) == 0)
		{
			SetSubfieldCodeColor(_ttoi(strLParam), _ttoi(strRParam));
		}
		else if (strKey.CompareNoCase(_T("FIELD_TERMINATOR")) == 0)
		{
			SetFieldTerminatorColor(_ttoi(strLParam), _ttoi(strRParam));
		}
		else if (strKey.CompareNoCase(_T("RECORD_TERMINATOR")) == 0)
		{
			SetRecordTerminatorColor(_ttoi(strLParam), _ttoi(strRParam));
		}
		else if (strKey.CompareNoCase(_T("ERROR")) == 0)
		{
			SetErrorColor(_ttoi(strLParam), _ttoi(strRParam));
		}
		else if (strKey.CompareNoCase(_T("FONT")) == 0)
		{			
			GetDefaultCharFormat(m_cf);			
			
			m_cf.yHeight = _ttoi(strLParam);			

			CHAR *szFaceName;
			CharConvert cvt;
			INT	nLen;

			cvt.UnicodeToMultiByte(strRParam.GetBuffer(0), &szFaceName, &nLen );

			strcpy(m_cf.szFaceName, szFaceName );
			free(szFaceName);			

			//strcpy(m_cf.szFaceName, "새굴림" );
			SetDefaultCharFormat(m_cf);
		}
	}

	file.Close();

	EFS_END
}

VOID CMarcEditCtrl::SaveEditorFormatConfig()
{
	EFS_BEGIN

	CStdioFile file;
	if (!file.Open(m_strEditorFormatConfFile, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return;

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	CString strLine;
	CString strKey;
	CString strLParam;
	CString strRParam;

	COLORREF clr;
	BOOL bBold;

	GetBkColor(clr, bBold);	
	strKey = _T("BK_COLOR");
	strLParam.Format(_T("%d"), clr);
	strRParam.Format(_T("%d"), bBold);
	strLine = strKey + _T(" = ") + strLParam + _T(", ") + strRParam + _T("\r\n");
	file.WriteString(strLine);

	GetTextColor(clr, bBold);
	strKey = _T("TEXT");
	strLParam.Format(_T("%d"), clr);
	strRParam.Format(_T("%d"), bBold);
	strLine = strKey + _T(" = ") + strLParam + _T(", ") + strRParam + _T("\r\n");
	file.WriteString(strLine);

	GetNumberColor(clr, bBold);
	strKey = _T("NUMBER");
	strLParam.Format(_T("%d"), clr);
	strRParam.Format(_T("%d"), bBold);
	strLine = strKey + _T(" = ") + strLParam + _T(", ") + strRParam + _T("\r\n");
	file.WriteString(strLine);

	GetTagCodeColor(clr, bBold);
	strKey = _T("TAG_CODE");
	strLParam.Format(_T("%d"), clr);
	strRParam.Format(_T("%d"), bBold);
	strLine = strKey + _T(" = ") + strLParam + _T(", ") + strRParam + _T("\r\n");
	file.WriteString(strLine);

	GetIndicatorCodeColor(clr, bBold);
	strKey = _T("INDICATOR");
	strLParam.Format(_T("%d"), clr);
	strRParam.Format(_T("%d"), bBold);
	strLine = strKey + _T(" = ") + strLParam + _T(", ") + strRParam + _T("\r\n");
	file.WriteString(strLine);

	GetSubfieldCodeColor(clr, bBold);
	strKey = _T("SUBFIELD_CODE");
	strLParam.Format(_T("%d"), clr);
	strRParam.Format(_T("%d"), bBold);
	strLine = strKey + _T(" = ") + strLParam + _T(", ") + strRParam + _T("\r\n");
	file.WriteString(strLine);

	GetFieldTerminatorColor(clr, bBold);
	strKey = _T("FIELD_TERMINATOR");
	strLParam.Format(_T("%d"), clr);
	strRParam.Format(_T("%d"), bBold);
	strLine = strKey + _T(" = ") + strLParam + _T(", ") + strRParam + _T("\r\n");
	file.WriteString(strLine);

	GetRecordTerminatorColor(clr, bBold);
	strKey = _T("RECORD_TERMINATOR");
	strLParam.Format(_T("%d"), clr);
	strRParam.Format(_T("%d"), bBold);
	strLine = strKey + _T(" = ") + strLParam + _T(", ") + strRParam + _T("\r\n");
	file.WriteString(strLine);

	GetErrorColor(clr, bBold);
	strKey = _T("ERROR");
	strLParam.Format(_T("%d"), clr);
	strRParam.Format(_T("%d"), bBold);
	strLine = strKey + _T(" = ") + strLParam + _T(", ") + strRParam + _T("\r\n");
	file.WriteString(strLine);
	
	GetDefaultCharFormat(m_cf);

	// 2009.03.20 ADD BY PDJ : Font Save
	TCHAR **szFaceName;
	CharConvert cvt;
	INT	nLen;
	
	szFaceName = (TCHAR**) malloc (sizeof(TCHAR*));	
	cvt.MultiByteToUnicode(m_cf.szFaceName, szFaceName, &nLen );		

	strKey = _T("FONT");
	strLParam.Format(_T("%d"), m_cf.yHeight);
	strRParam.Format(_T("%s"), *szFaceName);
	strLine = strKey + _T(" = ") + strLParam + _T(", ") + strRParam + _T("\r\n");
	file.WriteString(strLine);
	free(szFaceName);

	file.Close();

	EFS_END
}

CMarcEditCtrl::~CMarcEditCtrl()
{
	FreeSaveTagList();
	FreeSubfieldWordWrapList();
	if (m_pDlgMarcSyntaxError != NULL)
	{
		m_pDlgMarcSyntaxError->DestroyWindow();
		delete m_pDlgMarcSyntaxError;
		m_pDlgMarcSyntaxError = NULL;
	}

	if(m_hinstRichEdit2) FreeLibrary(m_hinstRichEdit2);

	// 16/08/10 김혜영 : 목록완성에서 마크편집하면 메모리릭이 발생하여 수정
//*/ ADD ----------------------------------------------------------------------------------
	if(m_ctlAutoList) 
	{
		delete m_ctlAutoList;
		m_ctlAutoList = NULL;
	}
//*/ END ----------------------------------------------------------------------------------
}

INT CMarcEditCtrl::FreeSaveTagList()
{
	EFS_BEGIN

	INT nListCount = m_listSavedTag.GetCount();
	if (nListCount == 0) return 0;
	CTag *pTag;
	for (INT i = 0; i < nListCount; i++)
	{
		pTag = (CTag*)m_listSavedTag.RemoveHead();
		if (pTag) delete pTag;
	}
	m_listSavedTag.RemoveAll();
	return nListCount;

	EFS_END
	return -1;

}

INT CMarcEditCtrl::FreeSubfieldWordWrapList()
{
	EFS_BEGIN

	INT nListCount = m_listSubfieldWordWrap.GetCount();
	if (nListCount == 0) return 0;
	CString *pStr;
	for (INT i = 0; i < nListCount; i++)
	{
		pStr = (CString*)m_listSubfieldWordWrap.RemoveHead();
		if (pStr) delete pStr;
	}
	m_listSubfieldWordWrap.RemoveAll();
	return nListCount;

	EFS_END
	return -1;

}

/////////////////////////////////////////////////////////////////////////////
// CMarcEditCtrl Proc
VOID CMarcEditCtrl::Init(CMarcMgr *pMarcMgr, CMarc *pMarc)
{
	EFS_BEGIN

	// Limit Tag
	//InitLimitTag();	

	// marc mgr
	SetMarcMgr(pMarcMgr);
	SetMarc(pMarc);	

	m_pDlgMarcSyntaxError->m_pMarcMgr = pMarcMgr;

	CreateAutoList();

	EFS_END
}

VOID CMarcEditCtrl::InitLimitTag()
{
	EFS_BEGIN

	m_arrayLimitTag.RemoveAll();

	EFS_END
}

VOID CMarcEditCtrl::AddLimitTag(CString strTagCode, CString strIndicator /* = _T("__")*/)
{
	EFS_BEGIN

	if (strTagCode.GetLength() != 3) return;

	if (strIndicator.GetLength() != 2)
		strIndicator = _T("__");

	m_arrayLimitTag.Add(strTagCode + strIndicator);

	EFS_END
}

BOOL CMarcEditCtrl::IsLimitTag(CString strTagCode, CString strIndicator /*= _T("__")*/)
{
	EFS_BEGIN
	
	if (strTagCode.GetLength() != 3) return FALSE;

	if (strIndicator.GetLength() != 2)
		strIndicator = _T("__");

	INT nCount = m_arrayLimitTag.GetSize();

	CString strLimitTag;
	for (INT idx = 0; idx < nCount; idx++)
	{
		strLimitTag = m_arrayLimitTag.GetAt(idx);
		if (strLimitTag.Right(2) == _T("__"))
		{
			if (strTagCode == strLimitTag.Left(3)) return TRUE;
		}
		else
		{
			if (strTagCode + strIndicator == strLimitTag) return TRUE;
		}
	}
	
	return FALSE;

	EFS_END

	return FALSE;
}

VOID CMarcEditCtrl::FormatAll()
{
	EFS_BEGIN

	
	CString strText;
	CString strShow;
	INT		nLen;

	GetWindowText(strText);
	strText.Replace(_T("\r"),_T(""));
	nLen = strText.GetLength();

	GetWindowText(strText);
	nLen = strText.GetLength();
	FormatTextRange(0, nLen);
	
	/*
	strWebCheck = GetExSelText();
	if ( strWebCheck.Find(_T("www.")) == 0 )
	{
		SetRawHyperText(strWebCheck.GetBuffer(0));	
	}
	*/
	
	
	//LockWindowUpdate();	
	//UnlockWindowUpdate();	
	

	EFS_END
}

VOID CMarcEditCtrl::FormatTextLines(INT nLineStart, INT nLineEnd)
{
	EFS_BEGIN

	
	LONG nIdx;
	
	nIdx = LineFromChar(nLineStart);
	LONG nStart = LineIndex(nIdx);

	nIdx = LineFromChar(nLineEnd);	
	LONG nEnd = LineIndex(nIdx);
	
	nEnd += LineLength(nLineEnd);	

	//GetFieldRange(nStart, nEnd);
	FormatTextRange(nStart, nEnd);
	CheckSimpleSyntax(nStart, nEnd);

	EFS_END
}

BOOL CMarcEditCtrl::CheckFieldTerminator(LONG &nStart, LONG &nEnd)
{
	EFS_BEGIN	
	
	CString strText;	

	SetSel(nStart, nEnd);
	strText = GetExSelText();		

	if ( !((strText.GetAt(strText.GetLength()-1) == FIELD_TERMINATOR) ||
		 (strText.GetAt(strText.GetLength()-1) == RECORD_TERMINATOR)) )		
	{
		return FALSE;
	}	

	return TRUE;

	EFS_END

	return FALSE;
}

VOID CMarcEditCtrl::FormatTextRange(INT nStart, INT nEnd)
{
	EFS_BEGIN		

	if (nStart > nEnd)
		return;

	HideSelection(TRUE, FALSE);

	m_bInForcedChange = TRUE;

	CHARRANGE crOldSel;
	GetSel(crOldSel);
	//LockWindowUpdate();
	if( GetParentFrame() != NULL )
	{
		::LockWindowUpdate(GetParentFrame()->m_hWnd);
	}

	INT nIndxStart;				
	INT nIndxEnd;
	
	try
	{
		SetSel(nStart, nEnd);
		
		CString strSelText = GetExSelText();
		LONG nLen = strSelText.GetLength();
		
		//TCHAR *pBuffer = new TCHAR[nLen+1];
		TCHAR *pBuffer = (TCHAR*) malloc ( sizeof(TCHAR) * ( nLen + 2 ) );
		ASSERT(nLen <= nEnd - nStart);
		pBuffer[nLen+1] = 0;	

		_tcscpy(pBuffer, strSelText.GetBuffer(0));		

		TCHAR *pStart, *pPtr;
		pStart = pPtr = pBuffer;
		TCHAR *pSymbolStart = NULL;	

		CSymbolFormat sf;

		// 디폴트로 돌려놓고 작업한다.		
		GetDefaultCharFormat(m_cf);
		SetSelectionCharFormat(m_cf);
		
		BOOL bIsTagColored = FALSE;
		BOOL bISIndColored = FALSE;

		TCHAR szTagCode[3];
		ZeroMemory(szTagCode, 3);
		INT n = -1;

		INT bSpecial = FALSE;

		while(*pPtr != 0)
		{
			TCHAR ch = *pPtr;

			// CRLF를 제거
			if (ch == 13 || ch == 10)
			{
				pPtr++;
				bIsTagColored = 0;
			}
			// tag
			else
			if (!bIsTagColored)
			{
				if (_istdigit(ch))
				{
					pSymbolStart = pPtr;
					n = 0;
					do 
					{
						wmemcpy(szTagCode + n, pPtr, 1);
						ch = *(++pPtr);						
						n++;
					}
					while(_istdigit(ch) && n < 3);					
										
					sf = m_sfTagCode;
				}
				else
					sf = m_sfNumber;
				bIsTagColored = TRUE;
				bISIndColored = FALSE;
				bSpecial = 3;
			}
			// indicator
			else if (!bISIndColored)
			{
				// determine control field
				if (bIsTagColored)
				{					
					if (!m_pMarcMgr->IsControlField(szTagCode))
					{
						if (_istdigit(ch) || _istspace(ch))
						{
							pSymbolStart = pPtr;
							n = 0;
							do 
							{
								ch = *(++pPtr);
								n++;
							}
							while((_istdigit(ch) || _istspace(ch)) && n < 2);
							sf = m_sfIndicator;
						}
						else sf = m_sfNumber;
					}
					else sf = m_sfNumber;
				}
				
				bISIndColored = TRUE;
				bSpecial = 3;
			}
			// number
			// 16/05/19 박대우 : 다국어(캄보디아어) 마크처리 에러수정
/*// BEFORE ----------------------------------------------------------------------------------------------	
			else if (_istdigit(ch))
*/// AFTER -----------------------------------------------------------------------------------------------
			else if (_IS_DIGIT(ch))
//*/ END -------------------------------------------------------------------------------------------------
			{
				pSymbolStart = pPtr;
				_tcstod(pSymbolStart, &pPtr);
				sf = m_sfNumber;
			}
			// subfield code
			else if (ch == SUBFIELD_CODE)
			{				
				pSymbolStart = pPtr;
				pPtr = pPtr + 2;
				sf = m_sfSubfieldCode;
				bSpecial = 1;
			}
			// field terminator
			else if (ch == FIELD_TERMINATOR)
			{
				pSymbolStart = pPtr;
				pPtr++;
				sf = m_sfFieldTerminator;
				bIsTagColored = FALSE;
				bSpecial = 1;
			}
			// record terminator
			else if (ch == RECORD_TERMINATOR)
			{
				pSymbolStart = pPtr;
				pPtr++;
				sf = m_sfRecordTerminator;
				bSpecial = 1;
			}
			else
			{
				pSymbolStart = pPtr;
				pPtr++;
				sf = m_sfText;
			}
			
			if (pSymbolStart != NULL) 
			{
				if ( _tcslen(pSymbolStart) < 1 ) break;
				
				//if ( _tcscmp(szTagCode, _T("049")) == 0 ) bSpecial = 2;				

				nIndxStart = nStart + _tcslen(pBuffer) - _tcslen(pSymbolStart) ;
				nIndxEnd = nStart + _tcslen(pBuffer) - _tcslen(pPtr) ;

				ASSERT(pSymbolStart < pPtr);
				//--SetFormatRange(nStart + pStart - pBuffer, nStart + pSymbolStart - pBuffer, FALSE, FALSE, RGB(0,0,0));
				//SetFormatRange(nStart + pSymbolStart - pBuffer, nStart + pPtr - pBuffer, sf.m_bBold, FALSE, sf.m_clrSymbol, bSpecial);
				SetFormatRange(nIndxStart, nIndxEnd, sf.m_bBold, FALSE, sf.m_clrSymbol, bSpecial);
				pStart = pPtr;
				pSymbolStart = 0;
				bSpecial = FALSE; 				
			} 
			else if (*pPtr == 0)
			{
				SetFormatRange(nStart + pStart - pBuffer, nStart + pPtr - pBuffer, FALSE, FALSE, RGB(0,0,0));
			}				
		}		

		free(pBuffer);
	}
	catch (...)
	{
	}

	SetSel(crOldSel);
	HideSelection(FALSE, FALSE);
	UnlockWindowUpdate();
	m_bInForcedChange = FALSE;
	

	EFS_END
}

VOID CMarcEditCtrl::SetFormatRange(INT nStart, INT nEnd, BOOL bBold, BOOL bUnderLine, COLORREF clr, INT bSpecial /*= FALSE*/)
{
	EFS_BEGIN

	if (nStart >= nEnd)
		return;

	SetSel(nStart, nEnd);

	DWORD dwEffects = 0;
	dwEffects = bBold?CFE_BOLD:0;
	if (bUnderLine)	dwEffects |= CFE_UNDERLINE;	
	m_cf.cbSize = sizeof(m_cf);
    GetSelectionCharFormat(m_cf);
	
	if ((m_cf.dwMask & CFM_COLOR)  && m_cf.crTextColor == clr && 
		(m_cf.dwMask & CFM_BOLD) && (m_cf.dwEffects & CFE_BOLD) == dwEffects)
		return;
	
	m_cf.crTextColor = clr;
	m_cf.dwEffects = dwEffects;
	m_cf.dwMask = CFM_FACE | CFM_BOLD | CFM_COLOR;
	if (bUnderLine)	m_cf.dwMask |= CFM_UNDERLINE;

	// 2007.02.27 REM AND UPDATE BY PDJ
	// Window Vista에서도 마크기호가 표시되지 않는다.
	// 새로운 버전에 대한 예외처리 추가
	/*
	if (bSpecial && m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 1)
	{
		_tcscpy(m_cf.szFaceName, _T("Terminal"));
	}
	*/
	if (bSpecial == 1 )
	{
		if ( ( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 1 ) || m_osvi.dwMajorVersion == 6 )
		{
			strcpy(m_cf.szFaceName, "Terminal");
		}
	}
	else if (bSpecial == 2 )
	{
		if ( ( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 1 ) || m_osvi.dwMajorVersion == 6 )
		{
			strcpy(m_cf.szFaceName, "새굴림");
		}
	}	
	else if (bSpecial == 3 )
	{
		if ( ( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 1 ) || m_osvi.dwMajorVersion == 6 )
		{
			strcpy(m_cf.szFaceName, "굴림체");
		}
	}	


	SetSelectionCharFormat(m_cf);


	EFS_END
}

VOID CMarcEditCtrl::InsertMarcCode(UINT nType, UINT nChar /*=NULL*/)
{
	EFS_BEGIN

	CHARRANGE crCurSel;
	GetSel(crCurSel);

	TCHAR szSubfieldCode[3];
	ZeroMemory(szSubfieldCode, 3);
	
	if (nChar == NULL)
		_stprintf(szSubfieldCode, _T("%c\0"), nType);
	else 
		_stprintf(szSubfieldCode, _T("%c%c\0"), nType, nChar);
	
	ReplaceSel(szSubfieldCode, TRUE);
	// 2008.02.21 REM 주석잡아도 잘됨
	FormatTextLines(m_crOldSel.cpMin, crCurSel.cpMax);

	EFS_END
}

BOOL CMarcEditCtrl::Acceclerator(UINT nChar)
{
	EFS_BEGIN	

	// ReadOnly 이면 처리안한다.

	//===================================================
	//2009.06.01 UPDATE BY PJW : ReadOnly 이더라도 Ctrl+C는 동작하도록 한다.
// 	if (GetStyle() & ES_READONLY) return FALSE;
 	if (GetStyle() & ES_READONLY) 
	{
		if (GetAsyncKeyState(VK_CONTROL))
		{
			switch(nChar)
			{
			case 'C' :
				return TRUE;
				break;
			default:
				return FALSE;
			}
		}
		return FALSE;
	}
	//===================================================

	CStdioFile file;

	if (GetAsyncKeyState(VK_CONTROL))
	{
		CString strFilePath = _T("..\\cfg\\1.cfg") ;	
		if(!file.Open (strFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary ) ) return FALSE ;
		{
			CString str;
			str.Format(_T("%d, %d(%c)\r\n"), GetAsyncKeyState(VK_CONTROL),nChar,nChar);
		
			if( 0 == file.GetLength() )
			{			
				TCHAR cUni = 0xFEFF;
				file.Write( &cUni, sizeof(TCHAR));
			}
		
			file.WriteString(str);
		}

		switch(nChar)
		{
		case 'V' :
			{	
				TCHAR * buffer = NULL;				
				if ( OpenClipboard() ) 					
				{	
					HANDLE hData = GetClipboardData( CF_UNICODETEXT );					
					buffer = (TCHAR*)GlobalLock( hData );					
					GlobalUnlock( hData );					
					CloseClipboard();
					ReplaceSel(buffer, TRUE);					
				}
			}
			return FALSE;
			break;
		// field terminator Ctrl + Y		
		case 89:
			OnPopupInsertFieldTerminator();
			return TRUE;
		// record terminator Ctrl + R
		case 82:
			OnPopupInsertRecordTerminator();
			return TRUE;
		// Print Ctrl + P
		case 80:
			OnPopupPrint();
			return TRUE;
		// Find Ctrl + F
		case 70: 
			OnPopupEditFind();
			return TRUE;
		// Replace Ctrl + H
		case 72:
			OnPopupEditReplace();
			return TRUE;
		case VK_F7:
			CheckFullSyntax();
			return TRUE;
		case VK_F2:			
			//ApplyEdit();
			return TRUE;
		case VK_F11:
			m_pDlgMarcSyntaxError->ShowWindow(SW_SHOW);
			m_pDlgMarcSyntaxError->SetFocus();
			return TRUE;
		default:
			return TRUE;
		}
	}
	
	return TRUE;

	EFS_END
	return FALSE;

}

VOID CMarcEditCtrl::CreateSyntaxErrorDialog()
{
	EFS_BEGIN

	CRect rect;
	CWnd* pWnd = GetDesktopWindow();
	pWnd->GetWindowRect(&rect);
	
	m_pDlgMarcSyntaxError = new CDlgMarcSyntaxError(this);
	m_pDlgMarcSyntaxError->Create(this);
	m_pDlgMarcSyntaxError->MoveWindow(rect.right - 550, rect.top , 550, 200);

	EFS_END
}

INT CMarcEditCtrl::ApplyEdit(INT nMode /*= 0*/)
{
	EFS_BEGIN

	if (!GetModify()) return 0;

	BOOL bShowError = TRUE;
	
	if (nMode == 1)
	{
		bShowError = FALSE;
	}

	BOOL bIsCheck = CheckFullSyntax(bShowError);

	// TODO : not completed error handling

	INT ids = GetMarc(m_pMarc);
	if (ids < 0) return -1;
	
	//((CMarcEditor*)m_pParentEditor)->UpdateEditCtrls(GRID_CTRL);

	if (!bIsCheck)
		return -1;

	return 0;

	EFS_END
	return -1;

}

VOID CMarcEditCtrl::SetMarc(CMarc *pMarc)
{
	EFS_BEGIN

	FreeSaveTagList();
	m_pMarc = pMarc;

	if (m_pMarc == NULL) return;

	// Limit tag to Save Tag
	INT nCount = m_arrayLimitTag.GetSize();
	if (nCount == 0) return;

	CString strLimitTag;
	CString strInd;
	CTag *pTag = NULL;
	CTag *pTagClone = NULL;
	POSITION pos = NULL;
	for (INT idx = 0; idx < nCount; idx++)
	{
		strLimitTag = m_arrayLimitTag.GetAt(idx);

		pos = m_pMarc->m_listTag.GetHeadPosition();
		while (pos)
		{
			pTag = reinterpret_cast<CTag*>(m_pMarc->m_listTag.GetNext(pos));
			if (pTag == NULL) continue;
			
			// Check Tag Code
			if (pTag->GetTagCode() != strLimitTag.Left(3)) continue;		
			
			// Check Indicator -->  지시기호가 '__'이면 체크하지 않는다.
			strInd = strLimitTag.Right(2);
			if (strInd != _T("__")) 
				if (pTag->GetFirstIndicator() + pTag->GetSecondIndicator() != strInd) continue;
			
			// Make Tag Clone And Save to Tag List
			pTagClone = pTag->Clone();
			if (pTagClone)
				m_listSavedTag.AddTail(reinterpret_cast<CObject*>(pTagClone));
		}

	}

	CHARRANGE cr;
	cr.cpMax = 0;
	cr.cpMin = 0;

	SetSel(cr);

	EFS_END
}

INT CMarcEditCtrl::GetMarc(CMarc *pMarc)
{
	EFS_BEGIN

	if (pMarc == NULL) return -1;
	if (m_pMarcMgr == NULL) return -1;

	// 마크를 초기화
	pMarc->Init();
	
	CString strEditMarcText;
	GetWindowText(strEditMarcText);
	strEditMarcText.TrimLeft();
	strEditMarcText.TrimRight();
	
	// 필드 종단기호 단위로 패치 시작
	CString strField;
	CString strTagCode;
	CTag *pTag = NULL;
	INT ids = -1;
	INT nStart = 0;
	INT nEnd = strEditMarcText.Find(FIELD_TERMINATOR);

	while (nEnd > 0)
	{
		strField = strEditMarcText.Mid(nStart, nEnd - nStart + 1);

		nStart = nEnd + 1;
		nEnd = strEditMarcText.Find(FIELD_TERMINATOR, nStart);

		strField.TrimLeft();
		strField.TrimRight();
		RemoveCRLFSpace(strField);

		pTag = NULL;
		strTagCode.Empty();

		// Field 길이가 3보다 작으면 continue;
		if (strField.GetLength() < 3) continue;
				
		// 테그코드
		CString strTagCode = strField.Left(3);

		// 지시기호
		CString strInd;
		if (strField.GetLength() >= 5)
			strInd = strField.Mid(3, 2);

		// 제한 테그이면 저장하지 않는다.
		if (IsLimitTag(strTagCode, strInd)) continue;

		// 필드의 길이가 3보다 작거나 같으면 continue
		if (strField.GetLength() <= 3) continue;

		// 필드를 패치하여 저장한다.
		pTag = new CTag;
		if (pTag == NULL) continue;
		
		ids = m_pMarcMgr->FetchField(pTag, strTagCode, strField.Mid(3));
		if (ids < 0)
			delete pTag;
		else
			pMarc->AddTag(pTag);
	}

	// 저장했던 제한테그를 넣는다.
	POSITION pos = m_listSavedTag.GetHeadPosition();
	while (pos)
	{
		pTag = (CTag*)m_listSavedTag.GetNext(pos);
		if (pTag == NULL) continue;

		pMarc->AddTag(pTag->Clone());
	}
	
	// record terminator --> auto
	pMarc->SetRecordTerminator(RECORD_TERMINATOR);

	m_pMarcMgr->SortMarc(pMarc);

	return 0;

	EFS_END
	return -1;

}

INT CMarcEditCtrl::GetMarc(CMarc &marc)
{
	EFS_BEGIN

	return GetMarc(&marc);

	EFS_END
	return -1;

}

VOID CMarcEditCtrl::RemoveCRLFSpace(CString &strSrc)
{
	EFS_BEGIN

	strSrc.Replace(_T("\r"), _T(""));
	strSrc.Replace(_T("\n"), _T(""));

	/*
	CString strReturn;
	INT nCount = strSrc.GetLength();
	for (INT idx = 0; idx < nCount; idx++)
	{
		if (strSrc.GetAt(idx) == '\r')
		{
			if (idx < nCount && strSrc.GetAt(idx + 1) == '\n')
			{
				idx++;

				do
				{
					idx++;	
				}
				while ((idx < nCount) && strSrc.GetAt(idx) == ' ');

			}		
		}
		if (idx < nCount)
			strReturn += strSrc.GetAt(idx);
	}

	strSrc = strReturn;
	*/

	EFS_END
}

BOOL CMarcEditCtrl::Display()
{
	EFS_BEGIN
	
	if (m_pMarcMgr == NULL) return FALSE;
	if (m_pMarc == NULL)	return FALSE;

	CHARRANGE cr;
	GetSel(cr);

	m_pMarcMgr->SortMarc(m_pMarc);

	// 추가..
	SetMarc(m_pMarc);

	SetWindowText(_T(""));
	//UpdateWindow();

	CString strText;

	INT n = 0;
	CTag *pTag = NULL;
	CSubfield *pSubfield;
	BOOL bIsFirstSubfield = TRUE;

	POSITION posTag = m_pMarc->m_listTag.GetHeadPosition();
	CString strField;
	while (posTag)
	{
		n = 0;
		strField.Empty();
		pTag = (CTag*)m_pMarc->m_listTag.GetNext(posTag);
				
		// tag code
		strField = pTag->GetTagCode();

		// indicator
		if (m_pMarcMgr->IsControlField(pTag->GetTagCode()))
		{
		}
		else
		{
			strField += pTag->GetFirstIndicator();
			strField += pTag->GetSecondIndicator();
		}
		
		bIsFirstSubfield = TRUE;
		POSITION posSubfield = pTag->m_listSubfield.GetHeadPosition();
		// subfield
		while (posSubfield)
		{
			pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);

			if (pSubfield->GetSubfieldCode().IsEmpty()) continue;

			// check first subfield
			if (!bIsFirstSubfield)
			{
				if (IsWrapSubfield(pTag->GetTagCode() + pSubfield->GetSubfieldCode()))
				{
					strField += _T("\r\n     ");
				}
			}

			// 첫번째는 보여주지 않음
			// punctuation mark
			if (n++ != 0)
				strField += pSubfield->GetPunctuationMark();
			
			// subfield code
			strField += pSubfield->GetSubfieldCode();

			// subfield data
			strField += pSubfield->GetItemData();

			bIsFirstSubfield = FALSE;
		}

		strField += pTag->GetItemData();
		strField += pTag->GetFieldTerminator();

	//	InsertLine(strField);
		
		strText += strField + _T("\r\n");
	}

	//InsertLine(m_pMarc->GetRecordTerminator());
	strText += m_pMarc->GetRecordTerminator();

	//LockWindowUpdate();
	if( GetParentFrame() != NULL )
	{
		::LockWindowUpdate(GetParentFrame()->m_hWnd);
	}	
	SetWindowText(strText);
	UnlockWindowUpdate();
	//UnlockWindowUpdate();
	
	SetSel(cr);
	//SetSel

	SetModify(FALSE);

	return TRUE;

	EFS_END
	return FALSE;

}

VOID CMarcEditCtrl::InsertLine(CString strLine)
{
	EFS_BEGIN

	strLine += _T("\r\n");

	m_bInForcedChange = FALSE;
	ReplaceSel(strLine.GetBuffer(0), TRUE);

	//FormatAll();

	EFS_END
}

VOID CMarcEditCtrl::InsertSpecialChar()
{
	EFS_BEGIN

	CLIB_F2KEY_SpecialChar sp;
	CString strSpecialChar;
	if (sp.DoModal() == IDOK)
	{
		sp.GetStringData(strSpecialChar);

		ReplaceSel(strSpecialChar, TRUE);
	}

	EFS_END
}

VOID CMarcEditCtrl::ConvertCharToHira()
{
	EFS_BEGIN

	CHARRANGE crOldSel;

	GetSel(crOldSel);

	if (crOldSel.cpMax == crOldSel.cpMin)
	{
		SetSel(crOldSel.cpMin - 2, crOldSel.cpMax);
	}

	CString strSelText = GetExSelText();

	if (strSelText.IsEmpty()) return;

	strSelText.TrimLeft();
	strSelText.TrimRight();

	CESL_CharSetMgr charSetMgr;
	charSetMgr.InitData(_T("KOREAN_CHAR"), _T("HIRA_CHAR"));

	INT ids;
	CString strTo;
	ids = charSetMgr.Find(strSelText, strTo);
	if (ids < 0 || strTo.IsEmpty()) 
	{
		SetSel(crOldSel);
		return;
	}

	ReplaceSel(strTo, TRUE);

	EFS_END
}

VOID CMarcEditCtrl::ConvertCharToKata()
{
	EFS_BEGIN

	CHARRANGE crOldSel;

	GetSel(crOldSel);

	if (crOldSel.cpMax == crOldSel.cpMin)
	{
		SetSel(crOldSel.cpMin - 2, crOldSel.cpMax);
	}

	CString strSelText = GetExSelText();

	if (strSelText.IsEmpty()) return;

	strSelText.TrimLeft();
	strSelText.TrimRight();

	CESL_CharSetMgr charSetMgr;
	charSetMgr.InitData(_T("KOREAN_CHAR"), _T("KATA_CHAR"));

	INT ids;
	CString strTo;
	ids = charSetMgr.Find(strSelText, strTo);
	if (ids < 0 || strTo.IsEmpty()) 
	{
		SetSel(crOldSel);
		return;
	}

	ReplaceSel(strTo, TRUE);

	EFS_END
}

VOID CMarcEditCtrl::ConvertCharToRoma()
{
	EFS_BEGIN

	CHARRANGE crOldSel;

	GetSel(crOldSel);

	if (crOldSel.cpMax == crOldSel.cpMin)
	{
		SetSel(crOldSel.cpMin - 2, crOldSel.cpMax);
	}

	CString strSelText = GetExSelText();

	if (strSelText.IsEmpty()) return;

	strSelText.TrimLeft();
	strSelText.TrimRight();

	CESL_CharSetMgr charSetMgr;
	charSetMgr.InitData(_T("KOREAN_CHAR"), _T("ROMA_CHAR"));

	INT ids;
	CString strTo;
	ids = charSetMgr.Find(strSelText, strTo);
	if (ids < 0 || strTo.IsEmpty()) 
	{
		SetSel(crOldSel);
		return;
	}

	ReplaceSel(strTo, TRUE);

	EFS_END
}

VOID CMarcEditCtrl::ConvertWordKorToChi()
{
	EFS_BEGIN

	CHARRANGE crOldSel;
	GetSel(crOldSel);

	HideSelection(TRUE, TRUE);

	CString strLine;
	CString strSelText;
	CString strHangul;
	
	if (crOldSel.cpMax == crOldSel.cpMin)
	{
		SetSel(LineIndex(), crOldSel.cpMax);
		strLine = GetExSelText();

		INT i = 0;
		for (i = strLine.GetLength() - 1; i >= 0; i--)
		{
			TCHAR ch = strLine.GetAt(i);
			if (ch == ' '  ||
				ch == '\0' ||
				ch == '('  ||
				ch == ')'  ||
				(_TUCHAR)ch < 0x80)
			{
				break;
			}
		}

		strSelText = strLine.Mid(i + 1);
	}
	else
		strSelText = GetExSelText();

	_TUCHAR ch1 = 0;
	_TUCHAR ch2 = 0;
	for (INT i = strSelText.GetLength() - 1; i > 0; i--)
	{
		if ((_TUCHAR)strSelText.GetAt(i) >= 0x80)
		{
			ch1 = (_TUCHAR)strSelText.GetAt(i - 1);
			ch2 = (_TUCHAR)strSelText.GetAt(i);
			if( ch1 >= 0xCA && ch2 >= 0xA1 && ch1 <= 0xFD && ch2 <= 0xFE )
			{
				break;
			}
			i--;
		}
		else
		{
			break;
		}
	}

	//===================================================
	//2009.05.22 UPDATE BY PJW : 홀수일경우 앞에 글자가 짤려서 변경함
// 	strSelText = strSelText.Mid(i + 1);
	strSelText = strSelText.Mid( i );
	//===================================================
	
	SetSel(crOldSel.cpMax - strSelText.GetLength(), crOldSel.cpMax);
	
	HideSelection(FALSE, TRUE);

	if (strSelText.IsEmpty()) return;

	CConvertToHanjaDlg dlg;
	dlg.m_strInput = strSelText;
	if (dlg.DoModal() == IDOK)
	{
		if (!dlg.m_strSelChi.IsEmpty())
		{
			ReplaceSel(dlg.m_strSelChi, TRUE);
		}
	}

	EFS_END
}



VOID CMarcEditCtrl::AddErrorLog(INT nMode /*=1*/)
{
	EFS_BEGIN

	if (m_pMarcMgr == NULL) return;

	INT nLineNumber;
	POSITION pos = m_pMarcMgr->m_listErrMsg.GetHeadPosition();
	while (pos != NULL)
	{
		CErrorMessage* pErrMsg = (CErrorMessage*)m_pMarcMgr->m_listErrMsg.GetNext(pos);
		if (pErrMsg == NULL) continue;
		if (nMode == 1)
		{
			nLineNumber = LineFromChar(-1);
		}
		else if (nMode == 2)
		{
			if (!FindLineNumber(pErrMsg->m_strTagCode, nLineNumber)) continue;
		}

		m_pDlgMarcSyntaxError->AddErrorMessage(nLineNumber, pErrMsg->m_strTagCode, pErrMsg->m_strErrMsg, pErrMsg->m_strHelpMsg);
	}

	EFS_END
}

BOOL CMarcEditCtrl::CheckFullSyntax(BOOL bShowError /*=TRUE*/)
{
	EFS_BEGIN

	INT ids;
	INT nLineCount;
	BOOL bError = FALSE;
	
	m_pDlgMarcSyntaxError->RemoveAll();
	INT nTextLength = GetTextLength();	

	// 먼저 구두점 자동생성을 위해 돌려준당
	GetMarc(m_pMarc);
	m_pMarcMgr->CheckSimpleSyntax(m_pMarc);
	Display();
		
	LONG nStart = 0;
	LONG nEnd = 0;
	LONG nOldStart = 0;
	LONG nOldEnd = 0;

	INT nLineLength = 0;
	
	HideSelection(TRUE, FALSE);
	CHARRANGE crOldSel;
	GetSel(crOldSel);
	//LockWindowUpdate();
	
	CMap <CString, LPCTSTR, INT, INT> mapTagCode;
	CString strField;
	CString strTagCode;
	CString strTemp;
	INT nTemp;	

	nLineCount = GetLineCount();
	for (INT i = 0; i < nLineCount; i++)
	{
		nLineLength = LineLength(LineIndex(i));
		nStart = LineIndex(i);	
		nEnd = LineIndex(i+1)-1;
		if ( nEnd < 0 ) break;

		while ( !CheckFieldTerminator(nStart, nEnd) )
		{
			i++;
			nEnd = LineIndex(i+1)-1;			
		}		
		
		nOldStart = nStart;
		nOldEnd = nEnd;
		
		if (!CheckSimpleSyntax(nStart, nEnd, TRUE, FALSE))
			bError = TRUE;

		// for repeat Tag
		//HideSelection(TRUE, FALSE);
		SetSel(nStart, nEnd);
		strField = GetExSelText();
		RemoveCRLFSpace(strField);
		if (strField.GetLength() >= 3)
		{
			if (mapTagCode.Lookup(strField.Left(3), nTemp))
			{
				mapTagCode.SetAt(strField.Left(3), ++nTemp);
			}
			else 
				mapTagCode.SetAt(strField.Left(3), 1);
		}
	}

	// Find Repeat Tag And Check
	POSITION pos = mapTagCode.GetStartPosition();
	while (pos)
	{
		mapTagCode.GetNextAssoc(pos, strTagCode, nTemp);
		if (nTemp > 1)
		{
			ids = m_pMarcMgr->CheckSimpleSyntax(strTagCode, CHK_REPEAT_TAG, strTemp, FALSE);
			if (ids < 0)
			{
				AddErrorLog(2);
				bError = TRUE;
			}
		}
	}

	SetSel(crOldSel);
	HideSelection(FALSE, FALSE);
	//UnlockWindowUpdate();

	// deep check 하지 않는다.
	//if (!CheckDeepSyntax())
	//bError = TRUE;
	Display();

	// 변경 적용
	((CMarcEditor*)m_pParentEditor)->UpdateEditCtrls(EDIT_CTRL);

	if (bError && bShowError) 
	{
		m_pDlgMarcSyntaxError->ShowWindow(SW_SHOW);
		m_pDlgMarcSyntaxError->SetFocus();
	}

	if (bError)
		return FALSE;
	else
		return TRUE;

	EFS_END
	return FALSE;

}

INT CMarcEditCtrl::GetSubfieldIdx(CString strField, INT nOrder)
{
	EFS_BEGIN

	BOOL bFind = FALSE;
	INT n = 0;
	INT nLength = strField.GetLength();
	for (INT idx = 0; idx < nLength; idx++)
	{
		if (strField.GetAt(idx) == SUBFIELD_CODE && nOrder == ++n) 
		{
			bFind = TRUE;
			break;
		}
	}
	
	if (bFind)
		return idx;
	else
		return -1;

	EFS_END
	return -1;

}

INT CMarcEditCtrl::ErrorShowProc(INT nErrorStart, INT nErrorEnd, BOOL bIsLogError)
{
	EFS_BEGIN

	SetFormatRange(nErrorStart, nErrorEnd, FALSE, TRUE, m_sfError.m_clrSymbol, TRUE);

	if (bIsLogError)
		AddErrorLog();	

	return 0;

	EFS_END
	return -1;

}

BOOL CMarcEditCtrl::CheckSimpleSyntax(INT nStart, INT nEnd, BOOL bIsLogError /*= FALSE*/, BOOL bIsHideSelection /*= TRUE*/)
{
	EFS_BEGIN

	// Check Validation
	if (m_pMarcMgr == NULL || !m_pMarcMgr->m_bUseRelation) return FALSE;
	if (nEnd - nStart < 3) return FALSE;

	// Pre Check...
	m_bInForcedChange = TRUE;
	CHARRANGE crOldSel;
	GetSel(crOldSel);
	//LockWindowUpdate();
	if (bIsHideSelection) HideSelection(TRUE, FALSE);

	// 에러가 발생했는지.
	BOOL bIsErrorOccured = FALSE;
	INT ids = -1;
	INT nErrorStart = -1;

	// 결과 값
	CString strResult;

	// 키밸류
	CString strKeyValue;
	
	// 테그
	CTag *pTag = NULL;

	// 서브필드
	CSubfield *pSubfield = NULL;

	// 테크코드
	CString strTagCode;

	// 테크코드를 제외한 필드..
	CString strField;

	// 서브필드코드
	CString strSubfieldCode;
	CString strSubfieldCode2;

	// 구두점
	CString strPuncMarc;

	// 포지션 
	POSITION pos = NULL;
	POSITION pos2 = NULL;

	// 서브필드 idx
	INT n = 0;
	INT n2 = 0;

	CString EOR;
	EOR.Format(_T("%c"), 30);

	// 텍스트 가져오기
	SetSel(nStart, nEnd);
	CString strSelText = GetExSelText();
	if (strSelText.GetLength() < 2) goto END;

	// CR/LF를 제거한다.

	strSelText.Replace(_T("\r"), _T(""));
	strSelText.Replace(_T("\n"), _T(""));	

	/*
	if (strSelText.GetAt(0) == '\r')
	{
		if (strSelText.GetAt(1) == '\n')
		{
			nStart += 2;
			strSelText.TrimLeft();
		}
	}
	*/

	strSelText.TrimRight();
	if (strSelText.GetLength() < 3) goto END;

	strTagCode = strSelText.Left(3);
	strField = strSelText.Mid(3);

	// 먼저 테그 코드를 체크해서 에러가 발생하면 리턴한다.
	ids = m_pMarcMgr->CheckSimpleSyntax(strTagCode, CHK_TAG, strResult);
	if (ids == 0)
	{
		// 예제를 보여준다.
		((CMarcEditor*)m_pParentEditor)->ShowExample(strResult);
	}
	else if (ids < 0)
	{
		// 에러를 보여주고 종료한다.
		ErrorShowProc(nStart, nStart + 3, bIsLogError);
		bIsErrorOccured = TRUE;
		goto END;
	}

	pTag = new CTag;
	m_pMarcMgr->FetchField(pTag, strTagCode, strField);

	// 제어필드가 아니면 지시기호 체크한다.
	if (!m_pMarcMgr->IsControlField(strTagCode))
	{
		CString strIndicator;
		strIndicator.Format(_T("%s%s"), pTag->GetFirstIndicator(), pTag->GetSecondIndicator());
	
		// ' ' -> '_'
		strIndicator.Replace(' ', '_');

		strKeyValue = strTagCode + strIndicator;
		

		ids = m_pMarcMgr->CheckSimpleSyntax(strKeyValue, CHK_INDICATOR, strResult);
		if (ids < 0)
		{
			bIsErrorOccured = TRUE;
			ErrorShowProc(nStart + 3, nStart + 5, bIsLogError);
		}
	}

	pos = pTag->m_listSubfield.GetHeadPosition();
	pos2 = NULL;
	while (pos)
	{
		pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(pos);
		if (pSubfield == NULL) continue;

		n++;

		strSubfieldCode = pSubfield->GetSubfieldCode();
		strPuncMarc = pSubfield->GetPunctuationMark();

		strSubfieldCode.Replace(SUBFIELD_CODE, '$');

		// 식별기호		
		strKeyValue = strTagCode + strSubfieldCode;
		if ( strSubfieldCode.Left(1) != EOR )
		{
			ids = m_pMarcMgr->CheckSimpleSyntax(strKeyValue, CHK_SUBFIELD, strResult);
			if (ids < 0)
			{
				bIsErrorOccured = TRUE;
				nErrorStart = GetSubfieldIdx(strTagCode + strField, n);
				if (nErrorStart >= 0)
					ErrorShowProc(nStart + nErrorStart, nStart + nErrorStart + 2, bIsLogError);
			}	
		}
		
		// 구두점은 체크할 필요가 없다...
		// 식별기호 반복 체크 --> 현재 식별기호가 반복이 되는것이면 체크할 필요 없다.
		ids = m_pMarcMgr->CheckSimpleSyntax(strKeyValue, CHK_REPEAT_SUBFIELD, strResult);
		if (ids >= 0) continue;

		// 같은 식별기호를 찾아 에러로 표시한다.
		n2 = n;
		pos2 = pos;
		while (pos2)
		{
			pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(pos2);
			if (pSubfield == NULL) continue;
			
			n2++;

			strSubfieldCode2 = pSubfield->GetSubfieldCode();
			strSubfieldCode2.Replace(SUBFIELD_CODE, '$');

			if ( strSubfieldCode2 == strSubfieldCode)
			{
				bIsErrorOccured = TRUE;
				nErrorStart = GetSubfieldIdx(strTagCode + strField, n2);
				if (nErrorStart >= 0)
					ErrorShowProc(nStart + nErrorStart, nStart + nErrorStart + 2, bIsLogError);				
			}

		}
	}

END:
	if (pTag)
		delete pTag;

	SetSel(crOldSel);
	if (bIsHideSelection) HideSelection(FALSE, FALSE);
	//UnlockWindowUpdate();

	m_bInForcedChange = FALSE;

	if (bIsLogError && bIsErrorOccured)
		return FALSE;
	else 
		return TRUE;


	EFS_END
	return FALSE;

}

BOOL CMarcEditCtrl::CheckSimpleSyntax2(INT nStart, INT nEnd, BOOL bIsLog /*=FALSE*/)
{
	EFS_BEGIN

	if (m_pMarcMgr == NULL) return FALSE;
	if (!m_pMarcMgr->m_bUseRelation) return TRUE;

	if (nEnd - nStart < 3)
		return FALSE;

	m_bInForcedChange = TRUE;
	CHARRANGE crOldSel;
	GetSel(crOldSel);
	//LockWindowUpdate();
	HideSelection(TRUE, TRUE);

	INT ids;
	INT i;
	INT nErrorStart;
	INT nErrorEnd;

	BOOL bIsErrorOccured = FALSE;

	TCHAR *pBuffer = NULL;

	CString strField;
	CString strTagCode;
	CString strIndicator;
	CString strSubfieldCode;
	CString strPunctuationMark;
	
	CString strResult;
	CString strTemp;
	CString strErrMsg;
	CString strHelpMsg;
	
	SetSel(nStart, nEnd);
	strField = GetExSelText();

	if (strField.IsEmpty()) return FALSE;
	
	if (strField.GetAt(0) == '\r')
	{
		if (strField.GetAt(1) == '\n')
		{
			nStart += 2;
			strField.TrimLeft();
		}
	}

	strField.TrimRight();
	
	// 테그 코드를 체크한다.
	if (strField.GetLength() < 3)
		goto END;

	strTagCode = strField.Left(3);
	ids = m_pMarcMgr->CheckSimpleSyntax(strTagCode, CHK_TAG, strResult);
	if (ids == 0)
	{
		// 예제를 보여준다.
		((CMarcEditor*)m_pParentEditor)->ShowExample(strResult);
	}
	else if (ids < 0)
	{
		// 에러를 보여주고 종료한다.
		nErrorStart = nStart;
		nErrorEnd = nErrorStart + 3;
		SetFormatRange(nErrorStart, nErrorEnd, FALSE, TRUE, m_sfError.m_clrSymbol);

		// add error log
		if (bIsLog)
		{
			AddErrorLog();
			bIsErrorOccured = TRUE;
		}

		goto END;
	}
	
	// indicator : blank indicator --> '_'
	if (strField.GetLength() > 5) 
	{
		strIndicator = strField.Mid(3, 2);
		strIndicator.Replace(_T(" "), _T("_"));
		strIndicator = strTagCode + strIndicator;

		// subfield & punctuation mark
		CArray<CString, CString&> arraySubfieldCode;
		CArray<CString, CString&> arrayPunctuationMark;

		INT nSubfieldCodeStart = 0;

		nSubfieldCodeStart = strField.Find(SUBFIELD_CODE, nSubfieldCodeStart);
		while (nSubfieldCodeStart != -1)
		{	
			strSubfieldCode = _T("$") + strField.Mid(nSubfieldCodeStart +1, 1);
			strPunctuationMark = strField.Mid(nSubfieldCodeStart - 1, 1);
			
			if (!m_pMarcMgr->IsPunctuationMark(strTagCode, strSubfieldCode + strPunctuationMark))
				strPunctuationMark = _T("_");
			
			arraySubfieldCode.Add(strTagCode + strSubfieldCode);
			arrayPunctuationMark.Add(strPunctuationMark + strTagCode + strSubfieldCode);

			nSubfieldCodeStart = strField.Find(SUBFIELD_CODE, nSubfieldCodeStart + 1);
		}
		
		/////////////////////////////////////////////////////////////////////////////////////
		// check start

		// 제어필드가 아니면 지시기호를 체그한다.
		if (!m_pMarcMgr->IsControlField(strTagCode))
		{
			ids = m_pMarcMgr->CheckSimpleSyntax(strIndicator, CHK_INDICATOR, strResult);
			if (ids < 0)
			{
				nErrorStart = nStart + 3;
				nErrorEnd = nErrorStart + 2;
				SetFormatRange(nErrorStart, nErrorEnd, FALSE, TRUE, m_sfError.m_clrSymbol);

				// add error log
				if (bIsLog)
				{
					AddErrorLog();
					bIsErrorOccured = TRUE;
				}
			}
		}
		
		// 식별기호 체크
		for (i = 0 ; i < arraySubfieldCode.GetSize(); i++)
		{
			ids = m_pMarcMgr->CheckSimpleSyntax(arraySubfieldCode.GetAt(i), CHK_SUBFIELD, strResult);
			if (ids < 0)
			{
				strTemp.Format(_T("%c%s"), SUBFIELD_CODE, arraySubfieldCode.GetAt(i).Right(1));
				
				INT nSubfieldStart = 0;
				nSubfieldStart = strField.Find(strTemp);
				while (nSubfieldStart != -1)
				{
					nErrorStart = nStart + nSubfieldStart;
					nErrorEnd = nErrorStart + 2;
					SetFormatRange(nErrorStart, nErrorEnd, FALSE, TRUE, m_sfError.m_clrSymbol);
					nSubfieldStart = strField.Find(strTemp, nSubfieldStart + 1);
				}

				// add error log
				if (bIsLog)
				{
					AddErrorLog();
					bIsErrorOccured = TRUE;
				}
			}
		}

		// 구두점 체크
		for (i = 0 ; i < arrayPunctuationMark.GetSize(); i++)
		{
			ids = m_pMarcMgr->CheckSimpleSyntax(arrayPunctuationMark.GetAt(i), CHK_PUNCTUATION_MARK, strResult);
			if (ids < 0)
			{
				CString strTemp;
				CString strPunctuationMark;
				strTemp.Format(_T("%c%s"), SUBFIELD_CODE, arrayPunctuationMark.GetAt(i).Right(1));
				strPunctuationMark = arrayPunctuationMark.GetAt(i).Left(1);
				
				INT nSubfieldStart = 0;
				nSubfieldStart = strField.Find(strTemp);

				CString strCheck;
				while (nSubfieldStart != -1)
				{
					strCheck = strField.Mid(nSubfieldStart - 1, 1);
					if (!m_pMarcMgr->IsPunctuationMark(strTagCode, strTemp + strCheck))
						strCheck = _T("_");

					if (strCheck == strPunctuationMark)
					{
						nErrorStart = nStart + nSubfieldStart - 1;
						nErrorEnd = nErrorStart + 1;

						// 자동고침 기능 없앰
						/*if (bIsLog)// && strPunctuationMark == _T("_"))
						{
							SetSel(nErrorEnd, nErrorEnd);
							ReplaceSel(strResult.Left(1), TRUE);
						}
						else
						*/
						SetFormatRange(nErrorStart, nErrorEnd, FALSE, TRUE, m_sfError.m_clrSymbol);
					}
					nSubfieldStart = strField.Find(strTemp, nSubfieldStart + 1);
				}				

				// add error log
				if (bIsLog)
				{
					AddErrorLog();
					bIsErrorOccured = TRUE;
				}
			}
		}

		// 반복 식별기호 체크
		for (i = 0; i < arraySubfieldCode.GetSize(); i++)
		{
			INT nEqualCount = 0;
			for (INT j = 0; j < arraySubfieldCode.GetSize(); j++)
			{
				if (arraySubfieldCode.GetAt(i) == arraySubfieldCode.GetAt(j))
				{
					nEqualCount ++;
				}

				if (nEqualCount > 1)
				{
					ids = m_pMarcMgr->CheckSimpleSyntax(arraySubfieldCode.GetAt(j), CHK_REPEAT_SUBFIELD, strResult);
					if (ids < 0)
					{
						CString strTemp;
						strTemp.Format(_T("%c%s"), SUBFIELD_CODE, arraySubfieldCode.GetAt(i).Right(1));
						
						INT nSubfieldStart = 0;
						nSubfieldStart = strField.Find(strTemp);
						while (nSubfieldStart != -1)
						{
							nErrorStart = nStart + nSubfieldStart;
							nErrorEnd = nErrorStart + 2;
							SetFormatRange(nErrorStart, nErrorEnd, FALSE, TRUE, m_sfError.m_clrSymbol);
							nSubfieldStart = strField.Find(strTemp, nSubfieldStart + 1);
						}

						// add error log
						if (bIsLog)
						{
							AddErrorLog();
							bIsErrorOccured = TRUE;
						}
					}
					break;
				}
			}
		}
	}

END:
	SetSel(crOldSel);
	HideSelection(FALSE, TRUE);
	//UnlockWindowUpdate();

	m_bInForcedChange = FALSE;

	if (bIsErrorOccured)
		return FALSE;
	else 
		return TRUE;

	EFS_END
	return FALSE;

}

BOOL CMarcEditCtrl::CheckDeepSyntax()
{
	EFS_BEGIN

	INT ids;
	ids = GetMarc(m_pMarc);
	if (ids < 0)
		return FALSE;

	if (m_pMarcMgr == NULL) return FALSE;
	ids = m_pMarcMgr->CheckDeepSyntax(m_pMarc);
	if (ids < 0)
	{
		AddErrorLog(2);
		return FALSE;
	}

	return TRUE;

	EFS_END
	return FALSE;

}

BOOL CMarcEditCtrl::FindLineNumber(CString strTagCode, INT &nLineNumber)
{
	EFS_BEGIN

	INT nLineCount;
	INT nLineLength;
	CString strLine;

	nLineCount = GetLineCount();

	for (INT i = 0; i < nLineCount; i++)
	{
		nLineLength = LineLength(LineIndex(i));
		GetLine(i, strLine.GetBuffer(nLineLength), nLineLength);
		strLine.ReleaseBuffer();
				
		if (strTagCode == strLine.Left(3))
		{
			nLineNumber = i;
			return TRUE;
		}
	}
	return FALSE;

	EFS_END
	return FALSE;

}

VOID CMarcEditCtrl::ShowACMarcGetter()
{
EFS_BEGIN
	
	// 이제 보여줍니다.
	INT ids;
	ids = GetMarc(m_pMarc);
	if (ids < 0) return;
	
	CString strTagCode;
	CString strSubfieldCode;
	CString strItemData;
	
	LONG nStartChar;
	LONG nEndChar;
	
	// 전거구분
	// 1 - 저자 : 245$d
	// 2 - 서명 : 245$a
	// 3 - 주제 :
	// 4 - 단어
	
	CString strField;
	CString strTextFront;
	CString strTextBack;
	GetLine(LineFromChar(LineIndex()), strField.GetBuffer(MAX_PATH), MAX_PATH);
	strField.ReleaseBuffer();
	if (strField.IsEmpty()) return;
	
	CHARRANGE crOldSel;
	GetSel(crOldSel);
	
	strTagCode = strField.Left(3);
	if (strTagCode == _T("245"))
	{
		GetSel(nStartChar, nEndChar);
		
		SetSel(LineIndex(), nEndChar);
		strTextFront = GetExSelText();
		
		SetSel(nEndChar, LineIndex() + LineLength(nEndChar));
		strTextBack = GetExSelText();
		
		SetSel(crOldSel);
		
		// Find Subfield Code Front
		INT nFind = strTextFront.ReverseFind(SUBFIELD_CODE);
		if (nFind < 0)
			return;
		
		strSubfieldCode = strTextFront.Mid(nFind, 2);
		strSubfieldCode.Replace(SUBFIELD_CODE, '$');

		// 041006_HSS $와 기호 사이에서 누르는 경우 에러 수정 +++++
		ids = strTextFront.GetLength();
		if (ids < nFind + 2)
		{
			AfxMessageBox(_T("전거대상을 결정할 수 없습니다."));
			return;
		}
		// ---------------------------------------------------------

		strTextFront = strTextFront.Mid(nFind + 2);
		
		// Find Subfield Code Back
		nFind = strTextBack.Find(SUBFIELD_CODE);
		if (nFind < 0)
		{
			nFind = strTextBack.Find(FIELD_TERMINATOR);
			if (nFind < 0)
				return;
		}
		
		strTextBack = strTextBack.Left(nFind);
		
		
		strItemData = strTextFront + strTextBack;
		
		// Find Punctuation Mark
		if (m_pMarcMgr->IsPunctuationMark(strItemData.Right(1)))
			strItemData = strItemData.Left(strItemData.GetLength() - 1);
		
		// Show AC Getter Dialog
		// 1 - 저자 : 245$d, 245$e
		if (strSubfieldCode == _T("$d") || strSubfieldCode == _T("$e"))
		{
			if ( strTagCode == _T("245")) {
				CWnd *pParent = ((CMarcEditor*)m_pParentEditor)->m_pParentWnd;
				if (pParent) 
					pParent->PostMessage(WM_AC_AUTHOR, 0, 0);
			}
		}
		// 2 - 서명 : 245$a, 440$a, 830$a
		else if (strSubfieldCode == _T("$a"))
		{
			if ( strTagCode == _T("245") || strTagCode == _T("440") || strTagCode == _T("830") )
			{
				CWnd *pParent = ((CMarcEditor*)m_pParentEditor)->m_pParentWnd;
				if (pParent) 
					pParent->PostMessage(WM_AC_TITLE, 3, 0);
			}				
		}
		else
		{
			AfxMessageBox(_T("전거통제 대상이 아닙니다."));
			return;
		}
		
		//if (dlg.DoModal() == IDOK)
		//	Display();

		//DestroyWindow();
	}
	
EFS_END

}

/////////////////////////////////////////////////////////////////////////////

static UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);

BEGIN_MESSAGE_MAP(CMarcEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CMarcEditCtrl)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_GETDLGCODE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_POPUP_INSERT_FIELD_TERMINATOR, OnPopupInsertFieldTerminator)
	ON_COMMAND(ID_POPUP_INSERT_RECORD_TERMINATOR, OnPopupInsertRecordTerminator)
	ON_COMMAND(ID_POPUP_SYNTAX_CHECK, OnPopupSyntaxCheck)
	ON_COMMAND(ID_POPUP_COLOR, OnPopupColor)
	ON_COMMAND(ID_POPUP_FONT, OnPopupFont)
	ON_COMMAND(ID_POPUP_SHOW_ERROR, OnPopupShowError)
	ON_COMMAND(ID_POPUP_SPECIAL_CHAR, OnPopupSpecialChar)
	ON_COMMAND(ID_POPUP_CHINESE, OnPopupChinese)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_POPUP_KOR_TO_HIRA, OnPopupKorToHira)
	ON_COMMAND(ID_POPUP_KOR_TO_KATA, OnPopupKorToKata)
	ON_COMMAND(ID_POPUP_KOR_TO_ROMA, OnPopupKorToRoma)
	ON_COMMAND(ID_POPUP_ACMARC, OnPopupAcmarc)
	ON_COMMAND(ID_POPUP_PRINT, OnPopupPrint)
	ON_COMMAND(ID_POPUP_EDIT_FIND, OnPopupEditFind)
	ON_COMMAND(ID_POPUP_EDIT_REPLACE, OnPopupEditReplace)
	ON_COMMAND(ID_ANALYZE_MARC, OnMarcAnalyzer)	
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_MESSAGE(WM_IME_CHAR , OnImeChar)
	ON_WM_TIMER()
	ON_WM_MOVE()
	ON_WM_SYSCHAR()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_POPUP_EDIT_COPY, Copy)
	ON_COMMAND(ID_POPUP_EDIT_CUT, Cut)
	ON_COMMAND(ID_POPUP_EDIT_PASTE, Paste)
	ON_WM_LBUTTONDOWN()	
	//ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(EN_PROTECTED, OnProtected)
	ON_NOTIFY_REFLECT(EN_SELCHANGE, OnSelChange)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_LINE_LOCATION, OnLineLocation)
	ON_REGISTERED_MESSAGE( WM_FINDREPLACE, OnFindReplace )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcEditCtrl message handlers

UINT CMarcEditCtrl::OnGetDlgCode() 
{
	EFS_BEGIN

	UINT uCode = CRichEditCtrl::OnGetDlgCode();
	
	uCode = DLGC_WANTALLKEYS | DLGC_WANTARROWS | DLGC_WANTCHARS | DLGC_WANTMESSAGE | DLGC_WANTTAB;

	return uCode;

	EFS_END
	return -1;
}


VOID CMarcEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	EFS_BEGIN

	CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);

	IntellisendRun();	

	EFS_END		
}



VOID CMarcEditCtrl::OnChange() 
{
	EFS_BEGIN

	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CRichEditCtrl::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	if (m_bInForcedChange)
		return;

	CHARRANGE crCurSel;
	GetSel(crCurSel);

	if (m_changeType == ctMove && crCurSel.cpMin == crCurSel.cpMax)
		m_changeType = ctPaste;

	switch (m_changeType)
	{
	case ctReplSel:
	case ctPaste:
		FormatTextLines(m_crOldSel.cpMin, crCurSel.cpMax);
		break;
	case ctDelete:
	case ctBack:
	case ctCut:
		FormatTextLines(crCurSel.cpMin, crCurSel.cpMax);
		break;
	case ctUndo:
		FormatTextLines(crCurSel.cpMin, crCurSel.cpMax);
		break;
	case ctMove:
		FormatTextLines(crCurSel.cpMin, crCurSel.cpMax);
		if (crCurSel.cpMin > m_crOldSel.cpMin) // move after
			FormatTextLines(m_crOldSel.cpMin, m_crOldSel.cpMin);
		else // move before
			FormatTextLines(m_crOldSel.cpMax, m_crOldSel.cpMax);
		break;
	default:
		FormatAll();
		break;
	}

	m_changeType = ctUndo;

	EFS_END
}

LRESULT CMarcEditCtrl::OnSetText(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	LRESULT res = Default();
	//FormatAll();
	return res;	

	EFS_END
	return NULL;

}

VOID CMarcEditCtrl::OnProtected(NMHDR* pNMHDR, LRESULT* pResult)
{
	EFS_BEGIN

	ENPROTECTED* pEP = (ENPROTECTED*)pNMHDR;

	// determine type of change will occur

	switch (pEP->msg) 
	{
	case WM_KEYDOWN:
		switch (pEP->wParam) 
		{
		case VK_DELETE:
			m_changeType = ctDelete;
			break;
		case VK_BACK:
			m_changeType = ctBack;
			break;
		default:
			m_changeType = ctUnknown;
			break;
		}
		break;
	case EM_REPLACESEL:
	case WM_CHAR:
		m_changeType = ctReplSel;
		break;
	case WM_PASTE:
		m_changeType = (m_changeType == ctCut)?ctMove:ctPaste;
		break;
	case WM_CUT:
		m_changeType = ctCut;
		break;
	default:
		m_changeType = ctUnknown;
		break;
	};

	if (pEP->msg != EM_SETCHARFORMAT && m_changeType != ctMove)
		m_crOldSel = pEP->chrg;

	*pResult = FALSE;

	EFS_END
}

VOID CMarcEditCtrl::OnSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	EFS_BEGIN

	SELCHANGE* pSC = (SELCHANGE*)pNMHDR;

	*pResult = 0;

	EFS_END
}

/*
VOID CMarcEditCtrl::OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	EFS_BEGIN
		
	if (GetKeyState(VK_MENU))
	{
		// ReadOnly 이면 처리안한다.
		if (GetStyle() & ES_READONLY) return;

		if (_istalnum(nChar))
		{
			// 알파벳이면 소문자로 바꾼다.
			if (_istalpha(nChar) && _istupper(nChar))
				nChar = _tolower(nChar);
			InsertMarcCode(SUBFIELD_CODE, nChar);
			return;
		}
	}	
	
	CRichEditCtrl::OnSysChar(nChar, nRepCnt, nFlags);

	EFS_END
}
*/

/*
VOID CMarcEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	EFS_BEGIN


	if (Acceclerator(nChar)) return;
	if (nChar == VK_TAB) return;


	CRichEditCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

	EFS_END
}
*/
LONG CMarcEditCtrl::OnLineLocation(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	SetFocus();

	INT nCharIdx = LineIndex((INT)wParam);
	SetSel(nCharIdx, nCharIdx);
	return 0;

	EFS_END
	return -1;

}

LRESULT CMarcEditCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	EFS_BEGIN

	if (message == EM_FINDTEXTEX)
	{
		wParam += 1;
	}	
	return CRichEditCtrl::WindowProc(message, wParam, lParam);

	EFS_END
	return NULL;

}

VOID CMarcEditCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	EFS_BEGIN

	CRichEditCtrl::OnLButtonDown(nFlags, point);	
	
	if (m_pMarcMgr == NULL) return;

	CString strField;
	GetLine(LineFromChar(LineIndex()), strField.GetBuffer(MAX_PATH), MAX_PATH);
	strField.ReleaseBuffer();

	CString strResult;
	INT ids = m_pMarcMgr->CheckSimpleSyntax(strField.Left(3), CHK_TAG, strResult);
	if (ids == 0)
	{
		// strResult = example
		((CMarcEditor*)m_pParentEditor)->ShowExample(strResult);
	}

	SetTimer(1,1, NULL);

	EFS_END	
}

VOID CMarcEditCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
EFS_BEGIN

	CRichEditCtrl::OnRButtonDown(nFlags, point);

	if( m_bIsAnalyzer == TRUE )	return;
	CMenu menu;
	menu.LoadMenu(IDR_MENU);
	
	CMenu *pMenu = menu.GetSubMenu(0);
	ClientToScreen(&point);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);

EFS_END
}

LONG CMarcEditCtrl::OnFindReplace(WPARAM wParam, LPARAM lParam)
{
	EFS_BEGIN

	CFindReplaceDialog* pFindReplace = CFindReplaceDialog::GetNotifier(lParam);
    ASSERT(pFindReplace != NULL);

	if (pFindReplace->IsTerminating())
    {
        pFindReplace = NULL;
    }
	else if (pFindReplace->FindNext())
	{
		DWORD dwFindFlag = 0;
		
		// match case
		if (pFindReplace->MatchCase())
			dwFindFlag |= FR_MATCHCASE;
		// match whole word
		if (pFindReplace->MatchWholeWord())
			dwFindFlag |= FR_WHOLEWORD;

		
		// downward or upward
		CHARRANGE cr;
		GetSel(cr);
		FINDTEXTEX ft;

		if (pFindReplace->SearchDown())
		{
			ft.chrg.cpMin = cr.cpMax;
			ft.chrg.cpMax = -1;	
		}
		else
		{			
			ft.chrg.cpMin = 0;
			ft.chrg.cpMax = cr.cpMax;
		}
				
		CString strFindString = pFindReplace->GetFindString();
		ft.lpstrText = (char*)strFindString.GetBuffer(0);

		
		LONG n = FindText(dwFindFlag, &ft);
		if (n == -1)
		{
			ft.chrg.cpMin = 0;
			ft.chrg.cpMax = -1;

			n = FindText(dwFindFlag, &ft);
		}

		if (n == -1)
			goto NOT_FIND;
		else
			SetSel(ft.chrgText);
	}
	else if (pFindReplace->ReplaceCurrent())
	{
		CString strSelText = GetExSelText();

		if (strSelText == pFindReplace->GetFindString())
		{
			ReplaceSel(pFindReplace->GetReplaceString(), TRUE);
		}
	}
	else if (pFindReplace->ReplaceAll())
	{
		CString strText;
		GetWindowText(strText);

		strText.Replace(pFindReplace->GetFindString(), pFindReplace->GetReplaceString());
		
		SetWindowText(strText);
	}

	return 0;

NOT_FIND:
	CString strMsg;
	strMsg.Format(_T("%s을(를) 찾을 수 없습니다."));
	AfxMessageBox(strMsg, MB_ICONINFORMATION);

	return 0;
	

	EFS_END
	return -1;

}

//////////////////////////////////////////////////////////////////////
// popup menu message handler

VOID CMarcEditCtrl::OnPopupInsertFieldTerminator() 
{
	EFS_BEGIN

	InsertMarcCode(FIELD_TERMINATOR);		

	EFS_END
}

VOID CMarcEditCtrl::OnPopupInsertRecordTerminator() 
{
	EFS_BEGIN

	InsertMarcCode(RECORD_TERMINATOR);

	EFS_END
}

VOID CMarcEditCtrl::OnPopupShowError() 
{
	EFS_BEGIN

	if (m_pDlgMarcSyntaxError->IsWindowVisible())
		m_pDlgMarcSyntaxError->ShowWindow(SW_HIDE);
	else
		m_pDlgMarcSyntaxError->ShowWindow(SW_SHOW);

	EFS_END
}

VOID CMarcEditCtrl::OnPopupSyntaxCheck() 
{
	EFS_BEGIN

	CheckFullSyntax();		

	EFS_END
}

VOID CMarcEditCtrl::OnPopupColor() 
{
	EFS_BEGIN

	CDlgSymbolColor dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		SaveEditorFormatConfig();
	}

	EFS_END
}

VOID CMarcEditCtrl::OnPopupFont() 
{
	EFS_BEGIN

	CClientDC dc(this);	
	GetDefaultCharFormat(m_cf);

	LOGFONT logfont;
	logfont.lfCharSet = m_cf.bCharSet;
	logfont.lfHeight = -(dc.GetDeviceCaps(LOGPIXELSY) * m_cf.yHeight/20) / 72;

	//===================================================
	//2009.04.09 UPDATE BY PJW : 폰트 다이얼로그를 띄울때 인자를 MULTIBYTE에서 UNICODE로 변경한다.
// 	_tcscpy(logfont.lfFaceName, (TCHAR*)m_cf.szFaceName);
	TCHAR * pFaceName;
	INT nNeedByte;
	CharConvert cvt;	

	cvt.MultiByteToUnicode(m_cf.szFaceName, &pFaceName, &nNeedByte );
	_tcscpy(logfont.lfFaceName, pFaceName);
	//===================================================
	
	CFontDialog dlg(&logfont, CF_SCREENFONTS | CF_NOSCRIPTSEL | CF_NOSTYLESEL | CF_NOVERTFONTS | CF_NOVECTORFONTS);
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCurrentFont(&logfont);
		
		m_cf.yHeight = dlg.GetSize() * 2;
		//strcpy(m_cf.szFaceName, (char*)logfont.lfFaceName);
		CHAR **szFaceName;
		//===================================================
		//2009.04.09 REM BY PJW : 위로올림
// 		CharConvert cvt;
		//===================================================
		
		INT	nLen;

		szFaceName = (CHAR**) malloc (sizeof(CHAR*));
		
		cvt.UnicodeToMultiByte(logfont.lfFaceName, szFaceName, &nLen );
		
		strcpy(m_cf.szFaceName, *szFaceName );
		free(szFaceName);

		SetDefaultCharFormat(m_cf);

		SaveEditorFormatConfig();
		FormatAll();				
	}
	EFS_END
}

VOID CMarcEditCtrl::OnPopupSpecialChar() 
{
	EFS_BEGIN

	InsertSpecialChar();	

	EFS_END
}

VOID CMarcEditCtrl::OnPopupChinese() 
{
	EFS_BEGIN

	ConvertWordKorToChi();	

	EFS_END
}

VOID CMarcEditCtrl::OnPopupKorToHira() 
{
	EFS_BEGIN

	ConvertCharToHira();		

	EFS_END
}

VOID CMarcEditCtrl::OnPopupKorToKata() 
{
	EFS_BEGIN

	ConvertCharToKata();	

	EFS_END
}

VOID CMarcEditCtrl::OnPopupKorToRoma() 
{
	EFS_BEGIN

	ConvertCharToRoma();	

	EFS_END
}

VOID CMarcEditCtrl::OnPopupAcmarc() 
{
	EFS_BEGIN

	ShowACMarcGetter();	

	EFS_END
}

VOID CMarcEditCtrl::OnPopupPrint() 
{
	EFS_BEGIN

	/*
	CDC             dc;
    CPrintDialog    printDlg(FALSE);
    CRect           r;
    INT             nHeight;
    
    // ask the user to select a printer
    if (printDlg.DoModal() == IDCANCEL)
        return;
    
    // Attach a printer DC
    dc.Attach(printDlg.GetPrinterDC());
    dc.m_bPrinting = TRUE;

    // use Textmappingmode, that's easiest to map the fontsize
    dc.SetMapMode(MM_TEXT);
    
    // setup font specifics
    LOGFONT LogFont;
    
    CFont	aFont, *oldFont;
    
    LogFont.lfHeight = -MulDiv(10, GetDeviceCaps(dc, LOGPIXELSY), 72);
    LogFont.lfWidth = 0;
    LogFont.lfEscapement = 0;
    LogFont.lfOrientation = 0;
    LogFont.lfWeight = 0;
    LogFont.lfItalic = false;
    LogFont.lfUnderline = 0;
    LogFont.lfStrikeOut = 0;
    LogFont.lfCharSet = ANSI_CHARSET;
    LogFont.lfOutPrecision = OUT_TT_PRECIS;
    LogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    LogFont.lfQuality = DEFAULT_QUALITY;
    LogFont.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
    lstrcpy (LogFont.lfFaceName, _T("MS Sans Serif"));
    dc.SetBkMode(OPAQUE);
    aFont.CreateFontIndirect ( &LogFont );
    // ok, we've build the font, now use it
    oldFont = dc.SelectObject( &aFont );        
        
    // Get the application title
    CString strTitle;
    strTitle.LoadString(AFX_IDS_APP_TITLE);
    
    // Initialise print document details
    
    DOCINFO di;
    ZeroMemory (&di, sizeof(DOCINFO));
    di.cbSize = sizeof (DOCINFO);
    // application title appears in the spooler view
    di.lpszDocName = strTitle;
    
    // Begin a new print job
    BOOL bPrintingOK = dc.StartDoc( &di );
        
    // Get the printing extents and store in the m_rectDraw field of a 
    // CPrintInfo object
    CPrintInfo Info;
    INT w = dc.GetDeviceCaps(HORZRES);
    INT h = dc.GetDeviceCaps(VERTRES);
    Info.m_rectDraw.SetRect(0,0, w, h);
    
    TCHAR *startAt = _T("aaaaaa");
    INT totalDone = 0;
    //INT lengthToGo = pSize;
	INT lengthToGo = 6;
    
    for (UINT page = Info.GetMinPage();
    bPrintingOK & totalDone < lengthToGo; page++)
    {
        // begin new page
        dc.StartPage();
        Info.m_nCurPage = page;
        
        // calc how much text fits on one page
        r = Info.m_rectDraw;
        r.bottom = r.top;
        INT i = 0;
        while (r.bottom < Info.m_rectDraw.bottom & totalDone + i < lengthToGo)
        {
            r.right = Info.m_rectDraw.right;
            nHeight = dc.DrawText(startAt, i++, r, 
                DT_CALCRECT|DT_WORDBREAK|DT_NOCLIP|DT_EXPANDTABS);
        }
        // go one back to assure correct height
        if (r.bottom >= Info.m_rectDraw.bottom)
            i--;
        
        
        // print that text
        dc.DrawText(startAt, i, r, DT_WORDBREAK|DT_NOCLIP|DT_EXPANDTABS);
        
        // go to next page
        startAt += i;
        totalDone += i;
        
        // end page
        bPrintingOK = (dc.EndPage() > 0);
    }
    
    // end a print job
    if (bPrintingOK)
        dc.EndDoc();
    else
        // abort job.
        dc.AbortDoc();
    
    
    // restore font
    dc.SelectObject(oldFont);
    // free font memory
    aFont.DeleteObject();
    // detach the printer DC
    dc.Detach();
	*/


	CDC dc;
	CPrintDialog dlg(FALSE);
	INT LineCnt,ncyScreen,StrLen,StrLenth,i,FindReturnidx;
	TEXTMETRIC tm;
	TCHAR NullChar,ReturnChar;
	
	CString buffer,TempBuffer;;
	
	NullChar = 0;
	ReturnChar = 13;

	LineCnt = ncyScreen = StrLen = StrLenth = i = FindReturnidx = 0;
	if(dlg.DoModal() == IDOK){
		dc.Attach(dlg.GetPrinterDC());
		DOCINFO di;
		memset (&di, 0, sizeof( DOCINFO) );
		di.cbSize = sizeof(DOCINFO);
		di.lpszDocName = _T("인쇄중!!");
		dc.SetWindowExt(2100,3000);  // Set the page size as A4
		dc.SetWindowOrg(0,0);
		if(dc.StartDoc(&di) != SP_ERROR)
		{
			dc.StartPage();
			HDC hdc=dlg.GetPrinterDC();
			GetWindowText(buffer);
			dc.GetTextMetrics(&tm );
		    ncyScreen = tm.tmHeight + tm.tmExternalLeading;

			while(1){

				FindReturnidx = buffer.Find(ReturnChar);
				
				if(FindReturnidx >= 0){
					StrLen = buffer.GetLength();
					TempBuffer = buffer.Left(FindReturnidx);
					StrLenth = TempBuffer.GetLength();
					::TextOut(hdc,0,ncyScreen*i,TempBuffer,StrLenth);
					buffer = buffer.Right(StrLen - (FindReturnidx +2));
					i++;
				}
				else{
					if(buffer.Find(NullChar) >= 0){
						StrLenth = buffer.GetLength();
						::TextOut(hdc,0,ncyScreen*i,buffer,StrLenth);
						break;
					}
					else if (buffer.IsEmpty())
					{
						break;
					}
				}
				
			}
			dc.EndPage();
			dc.EndDoc(); 
		}
	}

	/*
	CDC dc;
	CPrintDialog dlg(FALSE);

	if (dlg.DoModal() == IDOK)
	{
		dc.Attach(dlg.GetPrinterDC());
		

		FORMATRANGE fr;

		LONG lPageWidth = ::MulDiv(dc.GetDeviceCaps(PHYSICALWIDTH), 1440, dc.GetDeviceCaps(LOGPIXELSX));
		LONG lPageHeight = ::MulDiv(dc.GetDeviceCaps(PHYSICALHEIGHT), 1440, dc.GetDeviceCaps(LOGPIXELSY));

		CRect rcPage(0, 0, lPageWidth, lPageHeight);

		fr.hdc  = dlg.GetPrinterDC();
		fr.hdcTarget = dlg.GetPrinterDC();
		fr.rc = rcPage;
		fr.rcPage = rcPage;
		fr.chrg.cpMin = 0;
		fr.chrg.cpMax = -1;

		FormatRange(&fr, TRUE);

		RECT rcClient;
		GetClientRect(&rcClient);
		
		SetTargetDevice(dlg.GetPrinterDC(), lPageWidth);


		DisplayBand(&rcClient);
	}
	*/

	EFS_END
}

VOID CMarcEditCtrl::OnPopupEditFind() 
{
	EFS_BEGIN

	// find / replace
	m_pFRDlg = new CFindReplaceDialog();
	m_pFRDlg->Create(TRUE, _T(""), _T(""), FR_DOWN | FR_HIDEUPDOWN, this);
	
	m_pFRDlg->SetActiveWindow();
	m_pFRDlg->ShowWindow(SW_SHOW);

	EFS_END
}

VOID CMarcEditCtrl::OnPopupEditReplace() 
{
	EFS_BEGIN

	m_pFRDlg = new CFindReplaceDialog();
	m_pFRDlg->Create(FALSE, _T(""), _T(""), FR_DOWN | FR_REPLACE | FR_HIDEUPDOWN | FR_NOWHOLEWORD, this);

	m_pFRDlg->SetActiveWindow();
	m_pFRDlg->ShowWindow(SW_SHOW);

	EFS_END
}

VOID CMarcEditCtrl::ShowACMarcGetter_SubJect()
{
EFS_BEGIN
		
	//주제어 전거통제

	CWnd *pParent = ((CMarcEditor*)m_pParentEditor)->m_pParentWnd;
	if (pParent) 
	pParent->PostMessage(WM_AC_AUTHOR, 4, 0);

EFS_END
}

BOOL CMarcEditCtrl::PreTranslateMessage(MSG* pMsg) 
{
	INT	s_CtrlKeyFlag, s_AltKeyFlag;

	if ( pMsg->message == WM_IME_COMPOSITION )
	{
		if ( m_ctlAutoList != NULL )
		{
			m_ctlAutoList->SetKeyWord(_T(""));
			m_ctlAutoList->SearchKeyWord(-1);					
			m_ctlAutoList->ShowWindow(SW_HIDE);
			m_bAltKey = FALSE;		
		}
		return 0;
	}
	
	if ( pMsg->message == WM_SYSKEYDOWN )
	{
		INT nAltKey = GetKeyState(VK_MENU);
		if (nAltKey < 0 && m_bAltKey == FALSE )
		{
			m_bAltKey = TRUE;			
			IntellisendRun(TRUE);			
			return TRUE;
		}

		if (GetKeyState(VK_MENU))
		{
			TCHAR	nChar;
			// ReadOnly 이면 처리안한다.
			if (GetStyle() & ES_READONLY) return TRUE;
			
			nChar = pMsg->wParam;

			if ( m_szSubfieldCode == nChar ) return TRUE;
			
			if (_istalnum(nChar))
			{
				CString subFieldCode;
				// 알파벳이면 소문자로 바꾼다.
				if (_istalpha(nChar) && _istupper(nChar))
				{
					nChar = _tolower(nChar);
				}
				InsertMarcCode(SUBFIELD_CODE, nChar);
				IntellisendRun();
				m_szSubfieldCode = nChar;
				return TRUE;
			}
		}
	}

	if ( pMsg->message == WM_SYSKEYUP )
	{
		m_bAltKey = FALSE;		
	}	
	
	if ( pMsg->message == WM_KEYDOWN )
	{	
		CHARRANGE cr;
		s_CtrlKeyFlag = GetKeyState(VK_CONTROL);
		s_AltKeyFlag  = GetKeyState(VK_LMENU);
		switch ( pMsg->wParam )
		{
			case VK_DELETE :
				break;
			default:				
				CreateAutoList();			
				break;
		}	
		if (GetAsyncKeyState(VK_CONTROL))
		{			
			if ( Acceclerator(pMsg->wParam) == FALSE )
			{
				//===================================================
				//2009.06.01 UPDATE BY PJW : ReadOnly 가 아닐 경우만 에디트를 REFRESH한다.
// 				OnChange();
 				if (!(GetStyle() & ES_READONLY)) 
				{
					OnChange();
				}
				//===================================================
				return TRUE;
			}
		}		
		else if ( !(s_CtrlKeyFlag < 0  || s_AltKeyFlag < 0) )
		{
			switch ( pMsg->wParam )
			{
				case VK_DOWN:
					if ( m_ctlAutoList != NULL && m_ctlAutoList->IsWindowVisible() ) 
					{
						if ( m_bAutoListFocus == FALSE ) 
						{	
							if ( m_ctlAutoList->m_ctlListCtrl.GetItemCount() == 1 )
							{
								m_ctlAutoList->ShowWindow(SW_HIDE);
								m_bAltKey = FALSE;
								SetTimer(1,10, NULL);
								break;
							}
							else 
							{
								if ( m_nType > -1 ) m_ctlAutoList->PrevNextSelection(1);		
							}							
						}
						else 
						{
							if ( m_nType > -1 ) m_ctlAutoList->PrevNextSelection(2);
						}
						return TRUE;
					}
					else
					{
						SetTimer(1,1, NULL);
					}
					break;
				case VK_UP:
					if ( m_ctlAutoList != NULL && m_ctlAutoList->IsWindowVisible() ) 
					{
						if ( m_ctlAutoList->GetSelectedIndex() >= 0 )
						{
							if ( m_bAutoListFocus == TRUE ) 
							{
								if ( m_nType > -1 ) m_ctlAutoList->PrevNextSelection(3);
							}
						}
						else
						{
							m_ctlAutoList->ShowWindow(SW_HIDE);
							m_bAltKey = FALSE;
							SetTimer(1,1, NULL);
							break;
						}
						return TRUE;
					}
					else
					{
						SetTimer(1,1, NULL);
					}
					break;
				case VK_LEFT:						
					if ( m_ctlAutoList != NULL && m_ctlAutoList != NULL ) 
					{
						GetSel(cr);
						if ( cr.cpMin == cr.cpMax )
						{
							SetSel(cr.cpMax -1, cr.cpMax -1);
						}
						else
						{
							SetSel(cr.cpMin, cr.cpMin);	
						}
						m_ctlAutoList->SetKeyWord(_T(""));
						m_ctlAutoList->SearchKeyWord(-1);					
						m_ctlAutoList->ShowWindow(SW_HIDE);
						m_bAltKey = FALSE;
						this->IntellisendRun();					
						return TRUE;
					}
					break;
				case VK_RIGHT:
					if ( m_ctlAutoList != NULL ) 
					{
						GetSel(cr);
						if ( cr.cpMin == cr.cpMax )
						{
							SetSel(cr.cpMax +1, cr.cpMax +1);
						}
						else
						{
							SetSel(cr.cpMax, cr.cpMax);	
						}
						m_ctlAutoList->SetKeyWord(_T(""));
						m_ctlAutoList->SearchKeyWord(-1);					
						m_ctlAutoList->ShowWindow(SW_HIDE);
						m_bAltKey = FALSE;
						this->IntellisendRun();	
						return TRUE;
					}
					break;
				case VK_RETURN :
					{	
						if ( m_ctlAutoList != NULL )
						{
							if ( m_bAutoListFocus == TRUE )
							{							
								m_bAutoListFocus = FALSE;					
								m_ctlAutoList->ShowWindow(SW_HIDE);
								
								return TRUE;
							}
							else
							{
								m_ctlAutoList->ShowWindow(SW_HIDE);							
							}					
						}
					}
					break;
				case VK_DELETE :					
					break;
				case VK_ESCAPE:
					{
						if ( m_ctlAutoList != NULL )
						{		
							if ( m_bAutoListFocus == TRUE )	
							{
								m_bAutoListFocus = FALSE;					
								m_ctlAutoList->ShowWindow(SW_HIDE);		
								return TRUE;
							}
							else
							{
								return GetParent()->PreTranslateMessage(pMsg);
							}
						}
					}
					break;
				case VK_BACK:
					if ( m_ctlAutoList != NULL )
					{		
						if ( m_bAutoListFocus == TRUE )	
						{
							m_bAutoListFocus = FALSE;					
							m_ctlAutoList->ShowWindow(SW_HIDE);		
						}
					}
					break;
				case VK_TAB:
					//IntellisendRun(TRUE);					
					((CMarcEditor*)m_pParentEditor)->SetVolFocus();
					return TRUE;
					break;
				case VK_F1:				
					OnShowHtmlHelp();
					return TRUE;				
					break;
				case VK_F2:
					InsertSpecialChar();
					return TRUE;
				case VK_F3:
					ConvertCharToHira();
					return TRUE;
					break;
				case VK_F4:
					ConvertCharToKata();
					return TRUE;
					break;
				case VK_F5:
					ConvertCharToRoma();
					return TRUE;
					break;
				case VK_F6:
					ConvertWordKorToChi();
					return TRUE;
					break;
				case VK_F7:
					if( m_bIsAnalyzer == TRUE )	return FALSE;
						CheckFullSyntax();
					break;
				case VK_F8:
					if( m_bIsAnalyzer == TRUE )	return FALSE;
						ShowACMarcGetter_SubJect();
					return TRUE;
					break;
				case VK_F9:
					if( m_bIsAnalyzer == TRUE )	return FALSE;	
						ShowACMarcGetter();
					return TRUE;
					break;
				case VK_F10:
					break;
				case VK_F11:
					break;
				case VK_F12:
					break;
				case VK_HOME:
					break;
				case VK_END:
					break;
				case VK_INSERT:
					break;
				default:
					break;
			}
		}
		else
		{	
			if (!GetAsyncKeyState(VK_CONTROL))
			{
				switch(pMsg->wParam)
				{
				case VK_F2:
					InsertSpecialChar();
					return TRUE;
				case VK_F3:
					ConvertCharToHira();
					return TRUE;
				case VK_F4:
					ConvertCharToKata();
					return TRUE;
				case VK_F5:
					ConvertCharToRoma();
					return TRUE;
				case VK_F6:
					ConvertWordKorToChi();
					return TRUE;
				case VK_F7:
					if( m_bIsAnalyzer == TRUE )	return FALSE;
						CheckFullSyntax();
					return TRUE;
				case VK_F8:
					if( m_bIsAnalyzer == TRUE )	return FALSE;
						ShowACMarcGetter_SubJect();
					return TRUE;
				case VK_F9:
					if( m_bIsAnalyzer == TRUE )	return FALSE;	
						ShowACMarcGetter();
					return TRUE;
				case VK_F11:	// 2003.03.07 제거
					//m_pDlgMarcSyntaxError->ShowWindow(SW_SHOW);
					//m_pDlgMarcSyntaxError->SetFocus();
					//return TRUE;
				default:
					return FALSE;
				}
			}
		}	
	}
	
	if ( pMsg->wParam == VK_RETURN )
	{
		return CRichEditCtrl::PreTranslateMessage(pMsg);
	}	
	
	
	return CRichEditCtrl::PreTranslateMessage(pMsg);
}

void CMarcEditCtrl::CreateAutoList()
{	
EFS_BEGIN

	// 2009.05.14 ADD BY PDJ : 옵션 추가함
	if ( m_bAutoComplete == FALSE ) {return;}	
	if ( m_ctlAutoList != NULL ) {return;}	
	if ( m_bAutoListFocus == TRUE ) {return;}
	if ( GetSafeHwnd() == NULL ) {return;}		

	CWnd	*EditWnd;
	UINT   nEditID;

	EditWnd = GetFocus();
	if ( EditWnd == NULL ) {return;}
	nEditID = EditWnd->GetDlgCtrlID();	
	
	
	EditWnd->GetClientRect(&m_rect);
	ClientToScreen(m_rect);		
	
	CPoint capos = this->GetCaretPos();
	m_rect.left += capos.x;
	m_rect.top += capos.y+20;
	m_rect.right = m_rect.left + 200;
	m_rect.bottom = m_rect.top + 100;

	/*m_rect.left -= 2;
	m_rect.top += m_rect.bottom - m_rect.top+2;
	//m_rect.right = m_rect.left + 337;
	m_rect.bottom = m_rect.top + 150; 
	*/	
	
	m_ctlAutoList = new CECO_MarcAutoList( this );
	if ( m_ctlAutoList == NULL ) {return;}
	m_ctlAutoList->Create(NULL, NULL, LVS_SINGLESEL, m_rect, GetParent(), NULL, NULL);	
	//m_ctlAutoList->MoveWindow(m_rect);
	m_ctlAutoList->SetResource( nEditID, this );
	m_ctlAutoList->Display();
	m_ctlAutoList->ShowWindow(SW_HIDE);
	SetFocus();
EFS_END
}

VOID CMarcEditCtrl::OnMarcAnalyzer()
{
	INT nStart, nEnd;
	CString strMarc, strDirectory, strFieldTemp, strTemp, strTemp2;
	CStringArray astrDir, astrData;

	strMarc = m_pMarcMgr->m_strStreamMarc;
	
	if(strMarc.IsEmpty())
		return;

	INT pos = strMarc.Find(FIELD_TERMINATOR);
		
	if( pos > 0 )
	{
		strFieldTemp = strMarc.Mid(pos+1);
		strDirectory = strMarc.Left(pos);
	}	

	strDirectory = strDirectory.Mid(24);
	strDirectory.Replace(_T(" "), _T(""));

	strMarc.Empty();
	nStart = 0;
	nEnd = strDirectory.GetLength();
	while( nEnd > nStart+4 )
	{
		strTemp = strDirectory.Mid(nStart, 3);
		astrDir.Add(strTemp);
		nStart += 12;
	}
	nStart = 0;
	nEnd = strFieldTemp.GetLength();
	
	while( TRUE )
	{	
		if( nStart == 0 )
			strFieldTemp = strFieldTemp.Mid(nStart);
		nEnd = strFieldTemp.GetLength();
		if( nEnd <= 0 )	break;
		INT nPos = strFieldTemp.Find(FIELD_TERMINATOR);
		if( nPos<0 )	nPos = nEnd-1;
		astrData.Add(strFieldTemp.Left(nPos+1));
		nStart = nPos+1;
		strFieldTemp = strFieldTemp.Mid(nStart);
	}

	INT nTags, nFields;
	nTags = astrDir.GetSize();
	nFields = astrData.GetSize();
	
	for( INT i = 0; i<nFields ; i++)
	{
		if( i < nTags )	strTemp = astrDir.GetAt(i);
		else			strTemp = _T("???");
		if( i < nFields )strTemp2 = astrData.GetAt(i);
		else			strTemp2 = _T("???");

		if( strTemp2 == RECORD_TERMINATOR )	strMarc += strTemp2;
		else								strMarc += strTemp + strTemp2 + _T("\r\n");
	}

	CMARC_ANALYZER * p_ma;
	p_ma = new CMARC_ANALYZER;
	p_ma->SetMarc(strMarc);
	
	if( IDOK == p_ma->DoModal())
	{	
		(((CMarcEditor*)m_pParentEditor)->GetEditCtrl())->SetWindowText(p_ma->m_strMarc);
		(((CMarcEditor*)m_pParentEditor)->GetEditCtrl())->OnPopupSyntaxCheck();
	}
	if(p_ma)
		delete p_ma;
}



VOID CMarcEditCtrl::ExSetSel(long nStart, long nEnd)
{
	
	CString strSelText1;
	CString strSelText2;
	LONG	laddpos;
	LONG	llength;
	LONG	lstart;
	INT		nStartIgnory = 0;
	INT		nEndIgnory = 0;
	laddpos = 0;

	llength = nEnd - nStart;

	// start pos
	HideSelection(TRUE, TRUE);
	lstart = 0;
	for ( int i=0 ; i<nStart ; i ++ )
	{
		SetSel(lstart,lstart+1);
		strSelText1 = GetExSelText();
		SetSel(lstart+1,lstart+2);
		strSelText2 = GetExSelText();

		// 특수문자체크		
		//if ( strSelText1 != "" && strSelText1.GetAt(0) == 0x000d && strSelText1.GetAt(1) == 0x000a )		
		//{
		//	nStartIgnory += 1;
		//}
		
		
		if ( strSelText1 != "" && strSelText2 == "" )
		{
			lstart += 2;
		}
		else
		{
			lstart += 1;
		}		
	}

	nStart = lstart;
	if ( llength <= 0 )
	{
		HideSelection(FALSE, FALSE);
		SetSel(nStart, nStart);
		return;
	}
	
	// end pos	
	for ( i = nStart ; i<nStart+llength ; i ++) 
	{		
		SetSel(lstart,lstart+1);
		strSelText1 = GetExSelText();
		SetSel(lstart+1,lstart+2);
		strSelText2 = GetExSelText();

		// 특수문자체크		
		//if ( strSelText1 != "" && strSelText1.GetAt(0) == 0x000d && strSelText1.GetAt(1) == 0x000a )		
		//{
		//	nEndIgnory += 1;
		//}

		// 특수문자체크
		if ( strSelText1 != "" && strSelText2 == "" )
		{
			lstart += 2;
		}
		else
		{
			lstart += 1;
		}		
	}	

	nEnd = lstart;

	//HideSelection(FALSE, FALSE);
	SetSel(nStart-nStartIgnory, nEnd-nEndIgnory);	

	//CString strShow;
	//strShow.Format(_T("Pos:%d,%d, String:%s,%s"), nStart,nEnd, strSelText1, strSelText2);
	//AfxMessageBox(strShow);
}

VOID CMarcEditCtrl::ExSetSel(CHARRANGE crSel)
{
	ExSetSel(crSel.cpMin, crSel.cpMax);
}


BOOL CMarcEditCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{	
		
	return CWnd::Create(_T("RichEdit20W"), lpszWindowName, dwStyle | WS_VSCROLL   , rect, pParentWnd, nID, pContext);
}


BOOL CMarcEditCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	if(m_hinstRichEdit2)
    {        
        if(!CRichEditCtrl::PreCreateWindow(cs))
            return FALSE;
 
		cs.lpszClass = _T("RichEdit20W");
        cs.lpszName = _T("");
        cs.cx = cs.cy = 100; // necessary to avoid bug with ES_SELECTIONBAR and zero for cx and cy
        cs.style |= WS_CLIPSIBLINGS;
        return TRUE;
    }
    else
    {
        AfxMessageBox(_T("LOAD FAIL"));
        return CRichEditCtrl::PreCreateWindow(cs);
    }
}

CString CMarcEditCtrl::GetExSelText() 
{
	this->HideSelection(TRUE, FALSE);
	ASSERT(::IsWindow(m_hWnd));
	CHARRANGE cr;
	cr.cpMin = cr.cpMax = 0;
	::SendMessage(m_hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);
	LPTSTR lpsz = (TCHAR*)_alloca((cr.cpMax - cr.cpMin + 1)*2);
	lpsz[0] = NULL;
	::SendMessage(m_hWnd, EM_GETSELTEXT, 0, (LPARAM)lpsz);

	CString strBuffer(lpsz);	
	strBuffer.Replace( _T("\r\n"), _T("\n"));		

	return strBuffer;
}

void CMarcEditCtrl::GetExWindowText(CString &strText) 
{	
	GetWindowText(strText);
	strText.Replace(_T("\r"), _T(""));	
}

INT CMarcEditCtrl::OnShowHtmlHelp(VOID)
{
	CString strCHMfileName = _T("..\\HELP\\marc.chm");

	HH_AKLINK link;
	link.cbStruct =     sizeof(HH_AKLINK);
	link.fReserved =    FALSE;
	link.pszKeywords =  NULL;
	link.pszUrl =       NULL;
	link.pszMsgText =   NULL;
	link.pszMsgTitle =  NULL;
	link.pszWindow =    NULL;
	link.fIndexOnFail = TRUE;

	HtmlHelp(m_hWnd, strCHMfileName, HH_KEYWORD_LOOKUP, (DWORD)&link);

	return 0;
}

void CMarcEditCtrl::DeleteAutoList()
{
//EFS_BEGIN
	if ( m_ctlAutoList != NULL && GetSafeHwnd() != NULL) 
	{
		CString strText;

		if(!IsWindow(GetSafeHwnd()))
		{
			m_ctlAutoList = NULL;
			m_bAutoListFocus = FALSE;
			return;
		}

		GetWindowText(strText);

		m_ctlAutoList->SetKeyWord(strText.GetBuffer(0));
		m_ctlAutoList->ReferenceWrite();
		delete m_ctlAutoList;
		m_ctlAutoList = NULL;
		m_bAutoListFocus = FALSE;
	}
//EFS_END
}

int CMarcEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;	
	
	if ( m_ctlAutoList != NULL && m_ctlAutoList->m_pParentWnd != NULL )
		CreateAutoList();	
	return 0;
	
	return 0;
}


VOID CMarcEditCtrl::ShowAutoList(BOOL bFlag)
{
	if ( m_ctlAutoList == NULL ) {return;}	
	if ( m_bAutoListFocus == TRUE ) {return;}	
	

	INT nItemCount = m_ctlAutoList->m_strArraySearchKeyWordList.GetSize();
	if ( nItemCount <= 0 ) return;

	CWnd	*EditWnd;
	UINT   nEditID;

	EditWnd = GetFocus();
	if ( EditWnd == NULL ) {return;}
	nEditID = EditWnd->GetDlgCtrlID();	
	
	
	EditWnd->GetClientRect(&m_rect);
	ClientToScreen(m_rect);		
	
	CPoint capos = this->GetCaretPos();
	
	if ( m_nType == _TAG_ )
	{
		m_rect.left += 0;
	}
	else if ( m_nType == _1ST_ )
	{
		m_rect.left += capos.x-8;
	}
	else if ( m_nType == _2ST_ )
	{
		m_rect.left += capos.x-8;
	}
	else 
	{
		m_rect.left += capos.x-20;
	}

	m_rect.top += capos.y+20;

	
	INT nLength = m_ctlAutoList->MaxSearchKeywordSize();

	/*
	if ( m_rect.left + ( nLength * 6 ) > 500 )
	{
		m_rect.right = m_rect.left + ( nLength * 6 )+150;
	}
	else
	{
		m_rect.right = m_rect.left + 250;
	}
	*/
	if ( m_nType == _TAG_ )
	{
		m_rect.right = m_rect.left + ( nLength * 7 ) + 200 -2;
	}
	else if ( m_nType == _1ST_ || m_nType == _2ST_ )
	{
		m_rect.right = m_rect.left + ( nLength * 7 ) + 30 - 2;
	}
	else if ( m_nType == _SUBFIELD_ )
	{
		m_rect.right = m_rect.left + ( nLength * 7 ) + 110 - 2;
	}
	else
	{
	}		

	m_rect.bottom = m_rect.top + (nItemCount*21)+2;		
	
	if ( m_nType == _1ST_ || m_nType == _2ST_ )
	{
		m_rect.bottom += 21;
	}
	if ( (nItemCount*21)+2 > 300 ) 
	{
		m_rect.right += 17;
		m_rect.bottom = m_rect.top + 300;
	}
	
	//m_rect.bottom = m_rect.top + (nItemCount*20)+6;	

	m_ctlAutoList->MoveWindow(m_rect);	
	if ( bFlag == TRUE )
	{
		m_ctlAutoList->ShowWindow(SW_SHOWNOACTIVATE);
	}
	else
	{
		m_ctlAutoList->ShowWindow(SW_HIDE);
	}

	// Font Size & String Size
	
}

VOID CMarcEditCtrl::IntellisendRun(BOOL bAbsPos)
{
	if ( m_bAutoComplete == FALSE ) return;
	if ( m_ctlAutoList == NULL ) 
	{
		CreateAutoList();
	}

	//LockWindowUpdate();
	HideSelection(TRUE, FALSE);
	// 자동완성
	CString		strPrevString;
	CString		strTag;	
	CString		strIndi;
	CString		strSubfieldCode;
	CHARRANGE	cr;
	INT			nCodeIndex;
	INT			nLine;
	 
	//_TAG_:0, _1ST_:1, _2ST_:2, _SUBFIELD_:3, _DATA_:4		
	// 시작일 경우
	// 0 부터 시작
	strTag = _T("");	
	nLine = GetFirstVisibleLine();	
	CPoint nCap = GetCaretPos();	

	this->GetSel(cr);		
	if ( nLine == 0 && nCap.y == 1)
	{
		nCodeIndex = cr.cpMax;
		SetSel(0, nCodeIndex);
		strPrevString = this->GetExSelText();
	}
	else
	{	
		SetSel(0, cr.cpMax );
		strPrevString = GetExSelText();
		INT nPos = strPrevString.ReverseFind(_T('\r'));		
		strPrevString = strPrevString.Mid(nPos+1);
		nCodeIndex = strPrevString.GetLength();
	}	
			
	if ( nCodeIndex >= 3 )
	{
		m_nType = _TAG_;
		strTag = strPrevString.Mid(0, 3);
	}
	else if ( nCodeIndex < 3 )
	{
		m_nType = _TAG_;
		strTag = strPrevString.Mid(0, nCodeIndex);		
	}
	
	if ( (nCodeIndex == 4 && bAbsPos == FALSE) || (nCodeIndex == 3 && bAbsPos == TRUE) )
	{
		m_nType = _1ST_;
		
		if ( nCodeIndex == 4 ) strIndi = strPrevString.Mid(nCodeIndex-1, nCodeIndex);
		else strIndi=_T("?");
	}
	else if ( ( nCodeIndex == 5 && bAbsPos == FALSE) || (nCodeIndex == 4 && bAbsPos == TRUE))
	{
		m_nType = _2ST_;
		
		if ( nCodeIndex == 5 ) strIndi = strPrevString.Mid(nCodeIndex-1, nCodeIndex);
		else strIndi = _T("?");
	}		
	else if ( nCodeIndex > 5 || (nCodeIndex == 5 && bAbsPos == TRUE) )
	{
		CString temp;
		m_nType = _SUBFIELD_;
		
		if ( m_bAltKey == TRUE )
		{
			strSubfieldCode = _T("?");
			temp = strPrevString.Mid(nCodeIndex-2, nCodeIndex);
			if ( temp.Left(1) == m_strSubfieldCode )
			{
				//strSubfieldCode = temp;
				strSubfieldCode.Format(_T("▼%s"), temp.Mid(1,1));
			}				
		}
		else
		{
			temp = strPrevString.Mid(nCodeIndex-2, nCodeIndex);
			if ( temp.Left(1) == m_strSubfieldCode )
			{
				//strSubfieldCode = temp;
				strSubfieldCode.Format(_T("▼%s"), temp.Mid(1,1));				
			}				
			else
			{
				//m_nType = -1;
				SetSel(cr);
				m_ctlAutoList->SetKeyWord(_T(""));
				m_ctlAutoList->SearchKeyWord(-1);
				m_ctlAutoList->ShowWindow(SW_HIDE);
				m_bAltKey = FALSE;
				HideSelection(FALSE, FALSE);	
				//UnlockWindowUpdate();
				return;
			}
		}		
	}	

	SetSel(cr);

	if ( m_ctlAutoList != NULL ) 
	{	
		if ( m_nType == _TAG_ ) 
		{
			m_ctlAutoList->SetKeyWord(strTag.GetBuffer(0));
		}
		else if ( m_nType == _1ST_  || m_nType == _2ST_ ) 
		{
			strTag += _T(":") + strIndi;
			m_ctlAutoList->SetKeyWord(strTag.GetBuffer(0));
		}
		else if ( m_nType == _SUBFIELD_ )
		{
			strTag += _T(":") + strSubfieldCode;
			m_ctlAutoList->SetKeyWord(strTag.GetBuffer(0));
		}
		
		m_ctlAutoList->SearchKeyWord(m_nType);	
		ShowAutoList(TRUE);
		HideSelection(FALSE, FALSE);
		//UnlockWindowUpdate();
		return;
	}
	
	CreateAutoList();
	HideSelection(FALSE, FALSE);
	//UnlockWindowUpdate();
}


void CMarcEditCtrl::OnImeChar()
{
	return ;	  
}

void CMarcEditCtrl::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == 1 )
	{
		KillTimer(1);
		this->IntellisendRun();
	}
	
	CRichEditCtrl::OnTimer(nIDEvent);
}


// Web Link
void CMarcEditCtrl::SetRTF(CString sRTF)
{
	// Put the RTF string sRTF into the rich edit control.

	// Read the text in
	EDITSTREAM es;
	es.dwError = 0;
	es.pfnCallback = CBStreamIn;
	es.dwCookie = (DWORD) &sRTF;
	StreamIn(SF_RTF, es);	// Do it.
	
}

DWORD CALLBACK CMarcEditCtrl::CBStreamIn(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	// We insert the rich text here.
/*	
	This function taken from CodeGuru.com
	http://www.codeguru.com/richedit/rtf_string_streamin.shtml
	Zafir Anjum
*/
	CString *pstr = (CString *) dwCookie;

	if (pstr->GetLength() < cb)
	{
		*pcb = pstr->GetLength();
		memcpy(pbBuff, (LPCTSTR) *pstr, *pcb);
		pstr->Empty();
	}
	else
	{
		*pcb = cb;
		memcpy(pbBuff, (LPCTSTR) *pstr, *pcb);
		*pstr = pstr->Right(pstr->GetLength() - cb);
	}
	return 0;
}

DWORD CALLBACK CMarcEditCtrl::CBStreamOut(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	// Address of our string var is in psEntry
	CString *psEntry = (CString*) dwCookie;
	

	CString tmpEntry = _T("");
	tmpEntry = (CString) pbBuff;

	// And write it!!!
	*psEntry += tmpEntry.Left(cb);

	return 0;
}

CHARFORMAT CMarcEditCtrl::GetCharFormat(DWORD dwMask)
{	
	m_cf.cbSize = sizeof(CHARFORMAT);

	m_cf.dwMask = dwMask;

	GetSelectionCharFormat(m_cf);

	return m_cf;
}

void CMarcEditCtrl::SetCharStyle(int MASK, int STYLE, int nStart, int nEnd)
{	
	m_cf.cbSize = sizeof(CHARFORMAT);
	//m_cf.dwMask = MASK;
	
	GetSelectionCharFormat(m_cf);
	
	if (m_cf.dwMask & MASK)	// selection is all the same
	{
		m_cf.dwEffects ^= STYLE; 
	}
	else
	{
		m_cf.dwEffects |= STYLE;
	}
	
	m_cf.dwMask = MASK;

	SetSelectionCharFormat(m_cf);

}

PARAFORMAT CMarcEditCtrl::GetParagraphFormat()
{
	PARAFORMAT pf;
	pf.cbSize = sizeof(PARAFORMAT);

	pf.dwMask = PFM_ALIGNMENT | PFM_NUMBERING;    	

	GetParaFormat(pf);

	return pf;
}

BOOL CALLBACK CMarcEditCtrl::CBEnumFonts(LPLOGFONT lplf, LPTEXTMETRIC lptm, DWORD dwType, LPARAM lpData)
{
	if (dwType == TRUETYPE_FONTTYPE) 
	{
		((CStringArray *) lpData)->Add( lplf->lfFaceName );
	}

	return true;
}

BOOL CMarcEditCtrl::AddLink(CString csLinkText, CRect rectLink, UINT unLinkToDlgID)
{
	BOOL bRetVal = TRUE;
	do
	{
		try
		{
			if(csLinkText.IsEmpty ())
			{
				bRetVal = FALSE;
				break;
			}
			if(NULL == unLinkToDlgID )
			{
				bRetVal = FALSE;
				break;
			}

			HYPERLINKINFO linkInfo;

			linkInfo.csLinkText	   = csLinkText;
			linkInfo.rectDimension = rectLink;
			linkInfo.unLinkDlgID   = unLinkToDlgID;
			//Add to list
			//Add to head if list is empty ; otherwise add to tail
			if(m_lsLinks.IsEmpty ())
			{
				m_lsLinks.AddHead(linkInfo);
			}
			else
			{
				m_lsLinks.AddTail (linkInfo);
			}			
		}
		catch(...)
		{
			bRetVal = FALSE;
		}
	}while(0);

	return bRetVal;
}

BOOL CMarcEditCtrl::IsPointInsideALink(POINT pt,HYPERLINKINFO &linkInfo,const char *csText)
{
	POSITION posInList = NULL;
	BOOL bReturn	   = FALSE;
	do
	{
		try
		{
			if(m_lsLinks.IsEmpty ())
			{
				posInList = NULL;				
				break;
			}
			
			posInList = m_lsLinks.GetHeadPosition ();
			do
			{
				linkInfo = m_lsLinks.GetAt (posInList);
				if(linkInfo.rectDimension.IsRectEmpty ()) {break;};
				if(linkInfo.rectDimension.PtInRect (pt))
				{	
					bReturn = TRUE;
					break;
				}
				m_lsLinks.GetNext (posInList);
			}while(NULL != posInList);
			
		}
		catch(...)
		{
			bReturn = FALSE;
		}
	}while(0);
	
	return bReturn;
}


void CMarcEditCtrl::SetRawHyperText(const TCHAR *csRawText)
{
	m_csRAWText = csRawText;
	do
	{
		try
		{
			if(m_csRAWText.IsEmpty ()){break;}
			//Refresh the links information
			if(!m_lsLinks.IsEmpty ()){m_lsLinks.RemoveAll();}
			CString csTemp,csTempRaw,csTempRawOld,csTempMisc;
			csTempRawOld = RTF_SKELTON;//Have the rtf skelton with a <@RAW@> to replace our text
			csTempRaw    = m_csRAWText;
			int nStart=0,nEnd=0;
			do
			{
			//Find the link mark up ...
			nStart = csTempRaw.Find (_T("<%"),nStart);
			nEnd   = csTempRaw.Find (_T("%>"),nStart);
			//Get the text in between <% and %> store it for final replace
			csTempMisc = csTempRaw.Mid (nStart,(nEnd - nStart) + 2);
			//We don't need these markups for further steps so discard it.
			csTemp     = csTempMisc;
			csTemp.Remove ('<');csTemp.Remove ('%');csTemp.Remove ('>');
			//Now pass the mark-up raw text to parse and add the link information
			//to m_lsLinks list. The fuction ParseAndAddToList will return the
			//rtf formated text to replace csTempMisc in csTempRaw
			ParseAndAddToList(csTemp);
			csTempRaw.Replace (csTempMisc,csTemp);
			//Search again for another occurance of mark-up
			nStart = csTempRaw.Find (_T("<%"),nStart);	

			}while(nStart > 0);

			//Now replace the skelton with formatted link text
			csTempRawOld.Replace (_T("<@RAW@>"),csTempRaw);
			SetRTF(csTempRawOld);
			//The stage is ready for the show.. before proceeding
			//we have to store the boudaries to links
			SetBoundariesToLinks ();
		}
		catch(...)
		{
			break;
		}
		
	}while(0);
}


void CMarcEditCtrl::ParseAndAddToList(CString &csTemp)
{
	CString csLinkText(_T("")),csMisc(_T(""));
	UINT    unDlgID;
	int nStart=0,nEnd=0;
	do
	{
		try
		{
			nStart = csTemp.Find (_T("$"),nStart);
			nStart += 1;
			nEnd   = csTemp.Find (_T("$"),nStart);
			
			csLinkText = csTemp.Mid (nStart,(nEnd - nStart));
			nStart = nEnd;
			nStart = csTemp.Find (_T("#"),nStart);
			nStart += 1;
			nEnd   = csTemp.Find (_T("#"),nStart);
			unDlgID = _ttoi(csTemp.Mid (nStart,(nEnd - nStart)));
			if(!AddLink (csLinkText,NULL,unDlgID)) break;
			csMisc = RTF_LINK;
			csMisc.Replace (_T("<@UTEXT@>"),csLinkText);
			csTemp = csMisc;			
		}
		catch(...)
		{
			break;
		}
	}while(0);

}

void CMarcEditCtrl::SetBoundariesToLinks()
{
	/*
	CDC *pdc = GetDC();
	do{
		POSITION posInList;
		FINDTEXTEXW ftext;
		int nMin = 0;

		HYPERLINKINFO linkInfo;
		try
		{
			if(m_lsLinks.IsEmpty ())
			{
				posInList = NULL;				
				break;
			}			
			posInList = m_lsLinks.GetHeadPosition ();
			do
			{
				linkInfo = m_lsLinks.GetAt (posInList);
				//Find the text dimensions height and width for 
				//current logfont, of given text
				CSize size =  pdc->GetTextExtent(linkInfo.csLinkText);
				//Find the given text range.
				ftext.lpstrText = new TCHAR [linkInfo.csLinkText.GetLength () + 1];
				_tcscpy(ftext.lpstrText,linkInfo.csLinkText);
				ftext.chrg.cpMin = nMin;
				ftext.chrg.cpMax = -1; //Search the text in full content
				if(-1 != FindTextEx(FR_MATCHCASE|FR_WHOLEWORD,&ftext))
				{
					//If found store the first and last points of the string					
					CPoint oFirst,oLast;
					oFirst = GetCharPos (ftext.chrgText.cpMin);
					oLast  = GetCharPos (ftext.chrgText.cpMax);
					linkInfo.rectDimension.top    = oFirst.y;
					linkInfo.rectDimension.left   = oFirst.x;
					linkInfo.rectDimension.right  = (oFirst.x + size.cx);// - 10; //x + width
					linkInfo.rectDimension.bottom = (oFirst.y + size.cy); //y + height					
					nMin   = ftext.chrgText.cpMax;
					m_lsLinks.SetAt (posInList,linkInfo);
				}			
				delete [] ftext.lpstrText;
				m_lsLinks.GetNext (posInList);
			}while(NULL != posInList);
			
			
		}
		catch(...)
		{
			break;
		}
	}while(0);
	
	ReleaseDC(pdc);
	*/
}

void CMarcEditCtrl::FindNthLink(int nLinkNo,CString &csLink)
{
	int nStart,nEnd,nCount;
	nStart = nEnd = nCount = 0;
	do
	{
	nStart	   = m_csRAWText.Find ('$',nEnd);
	nStart += 1;
	nEnd	   = m_csRAWText.Find ('$',nStart);
	nEnd++;
	nCount++;
	}while(nCount == nLinkNo);
	nEnd--;
	csLink = "$" + m_csRAWText.Mid(nStart,nEnd - nStart) + "$";
	
}
void CMarcEditCtrl::ReplaceNthLink(int nLinkNo,const CString csNewValue)
{	
	CString csOldValue(_T(""));
	FindNthLink (nLinkNo,csOldValue);
	CString csTemp2 = _T("$") + csNewValue + _T("$");	
	m_csRAWText.Replace (csOldValue,csTemp2);	
	SetRawHyperText (m_csRAWText);
}

long FindTextEx(DWORD dwFlags, FINDTEXTEXW* pFindText)
{
	return -1;
}

void CMarcEditCtrl::OnMove(int x, int y) 
{
	CRichEditCtrl::OnMove(x, y);	
}

VOID CMarcEditCtrl::MoveMain()
{
	ShowAutoList(FALSE);

}

/*
void CMarcEditCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetWindowRect(rect);

	if ( rect != m_rectEdit )
	{
		::Beep(1000,50);
	}

	m_rectEdit = rect;
	
}
*/

INT	CMarcEditCtrl::MultiByteToUnicode(CHAR *fi_pSourceData, _TCHAR **fo_pDestinationData, INT *fo_nByteDes )
{

	_TCHAR	*pszDestinationData;
	INT		nWCharNeeded;
	INT		nSouceDataByte;
	

	nSouceDataByte = strlen(fi_pSourceData);
	
	/* Query the number of WChar required to store the Dest string */
	nWCharNeeded = MultiByteToWideChar( CP_ACP , 
										0  , 
										fi_pSourceData   , 
										nSouceDataByte   , 
										NULL             , 
										0 );
	if (nWCharNeeded == 0) 
	{
		*fo_nByteDes = 0;
		return GENERAL_ERROR ;
	}
	

	/* Allocate the required amount of space */	
	/* We need more 2 bytes for '\0' */
	pszDestinationData = (_TCHAR*) malloc (sizeof(_TCHAR)*nWCharNeeded + sizeof(_TCHAR));	
		
	/* Do the conversion */
	nWCharNeeded = MultiByteToWideChar(	CP_ACP				, 
										0						,
										fi_pSourceData					, 
										nSouceDataByte			   		,
										pszDestinationData          	, 
										nWCharNeeded);
	if (nWCharNeeded == 0) 
	{
		*fo_nByteDes = 0;
		free(pszDestinationData);
		return GENERAL_ERROR ;
	}	

	*(LPWSTR)((LPWSTR)pszDestinationData + nWCharNeeded) = L'\0';
	
	/* MultiByteToWideChar returns # WCHAR, so multiply by 2 */
	*fo_nByteDes = 2*nWCharNeeded ;	

	*fo_pDestinationData = pszDestinationData;


	return NOT_ERROR;
}