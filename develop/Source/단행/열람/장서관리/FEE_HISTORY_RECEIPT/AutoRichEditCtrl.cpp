// AutoRichEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AutoRichEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoRichEditCtrl

CAutoRichEditCtrl::CAutoRichEditCtrl()
{
	// KOL.UDF.022
	m_saFontList.RemoveAll();
	m_pParent = NULL;
}

CAutoRichEditCtrl::~CAutoRichEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CAutoRichEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CAutoRichEditCtrl)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoRichEditCtrl message handlers

CString CAutoRichEditCtrl::GetRTF()
{
	// Return the RTF string of the text in the control.
	
	// Stream out here.
	EDITSTREAM es;
	es.dwError = 0;
	es.pfnCallback = CBStreamOut;		// Set the callback

	CString sRTF = "";

	es.dwCookie = (DWORD) &sRTF;	// so sRTF receives the string
	
	StreamOut(SF_RTF, es);			// Call CRichEditCtrl::StreamOut to get the string.
	///

	return sRTF;

}

void CAutoRichEditCtrl::SetRTF(CString sRTF)
{
	// Put the RTF string sRTF INTo the rich edit control.

	// Read the text in
	EDITSTREAM es;
	es.dwError = 0;
	es.pfnCallback = CBStreamIn;
	es.dwCookie = (DWORD) &sRTF;
	StreamIn(SF_RTF, es);	// Do it.
	
}

/*
	Callback function to stream an RTF string INTo the rich edit control.
*/
DWORD CALLBACK CAutoRichEditCtrl::CBStreamIn(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
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
		memcpy(pbBuff, (LPCSTR)*pstr->GetBuffer(0), *pcb);
		pstr->Empty();
	}
	else
	{
		*pcb = cb;
		memcpy(pbBuff, (LPCSTR) *pstr->GetBuffer(0), *pcb);
		*pstr = pstr->Right(pstr->GetLength() - cb);
	}
	///

	return 0;
}

/*
	Callback function to stream the RTF string out of the rich edit control.
*/
DWORD CALLBACK CAutoRichEditCtrl::CBStreamOut(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	// Address of our string var is in psEntry
	CString *psEntry = (CString*) dwCookie;
	

	CString tmpEntry = "";
	tmpEntry = (CString) pbBuff;

	// And write it!!!
	*psEntry += tmpEntry.Left(cb);

	return 0;
}

BOOL CAutoRichEditCtrl::SelectionIsBold()
{
	CHARFORMAT cf = GetCharFormat();	
	
	if (cf.dwEffects & CFM_BOLD)
		return true;
	else
		return false;
}

BOOL CAutoRichEditCtrl::SelectionIsItalic()
{
	CHARFORMAT cf = GetCharFormat();	
	
	if (cf.dwEffects & CFM_ITALIC)
		return true;
	else
		return false;
}

BOOL CAutoRichEditCtrl::SelectionIsUnderlined()
{
	CHARFORMAT cf = GetCharFormat();	
	
	if (cf.dwEffects & CFM_UNDERLINE)
		return true;
	else
		return false;
}

CHARFORMAT CAutoRichEditCtrl::GetCharFormat(DWORD dwMask)
{
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);

	cf.dwMask = dwMask;

	GetSelectionCharFormat(cf);

	return cf;
}

void CAutoRichEditCtrl::SetCharStyle(INT MASK, INT STYLE, INT nStart, INT nEnd)
{
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	//cf.dwMask = MASK;
	
	GetSelectionCharFormat(cf);
	
	if (cf.dwMask & MASK)	// selection is all the same
	{
		cf.dwEffects ^= STYLE; 
	}
	else
	{
		cf.dwEffects |= STYLE;
	}
	
	cf.dwMask = MASK;

	SetSelectionCharFormat(cf);

}

void CAutoRichEditCtrl::SetSelectionBold()
{
	long start=0, end=0;
	GetSel(start, end);		// Get the current selection

	SetCharStyle(CFM_BOLD, CFE_BOLD, start, end);	// Make it bold
}

void CAutoRichEditCtrl::SetSelectionItalic()
{
	long start=0, end=0;
	GetSel(start, end);

	SetCharStyle(CFM_ITALIC, CFE_ITALIC, start, end);
}

void CAutoRichEditCtrl::SetSelectionUnderlined()
{
	long start=0, end=0;
	GetSel(start, end);

	SetCharStyle(CFM_UNDERLINE, CFE_UNDERLINE, start, end);
}

void CAutoRichEditCtrl::SetParagraphCenter()
{
	PARAFORMAT paraFormat;    
	paraFormat.cbSize = sizeof(PARAFORMAT);
	paraFormat.dwMask = PFM_ALIGNMENT;    
	paraFormat.wAlignment = PFA_CENTER;
	
	SetParaFormat(paraFormat);	// Set the paragraph.
}

void CAutoRichEditCtrl::SetParagraphLeft()
{
	PARAFORMAT paraFormat;
	paraFormat.cbSize = sizeof(PARAFORMAT);
	paraFormat.dwMask = PFM_ALIGNMENT;    
	paraFormat.wAlignment = PFA_LEFT;
	
	SetParaFormat(paraFormat);
}

void CAutoRichEditCtrl::SetParagraphRight()
{
	PARAFORMAT paraFormat;
	paraFormat.cbSize = sizeof(PARAFORMAT);
	paraFormat.dwMask = PFM_ALIGNMENT;    
	paraFormat.wAlignment = PFA_RIGHT;
	
	SetParaFormat(paraFormat);
}

BOOL CAutoRichEditCtrl::ParagraphIsCentered()
{
	PARAFORMAT pf = GetParagraphFormat();

	if (pf.wAlignment == PFA_CENTER)
		return true;
	else
		return false;
}

BOOL CAutoRichEditCtrl::ParagraphIsLeft()
{
	PARAFORMAT pf = GetParagraphFormat();

	if (pf.wAlignment == PFA_LEFT)
		return true;
	else
		return false;
}

BOOL CAutoRichEditCtrl::ParagraphIsRight()
{
	PARAFORMAT pf = GetParagraphFormat();

	if (pf.wAlignment == PFA_RIGHT)
		return true;
	else
		return false;
}

PARAFORMAT CAutoRichEditCtrl::GetParagraphFormat()
{
	PARAFORMAT pf;
	pf.cbSize = sizeof(PARAFORMAT);

	pf.dwMask = PFM_ALIGNMENT | PFM_NUMBERING;    	

	GetParaFormat(pf);

	return pf;
}

void CAutoRichEditCtrl::SetParagraphBulleted()
{
	PARAFORMAT paraformat = GetParagraphFormat();

	if ( (paraformat.dwMask & PFM_NUMBERING) && (paraformat.wNumbering == PFN_BULLET) )
	{
		paraformat.wNumbering = 0;
		paraformat.dxOffset = 0;
		paraformat.dxStartIndent = 0;
		paraformat.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
	}
	else
	{
		paraformat.wNumbering = PFN_BULLET;
		paraformat.dwMask = PFM_NUMBERING;
		if (paraformat.dxOffset == 0)
		{
			paraformat.dxOffset = 4;
			paraformat.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
		}
	}
	
	SetParaFormat(paraformat);

}

BOOL CAutoRichEditCtrl::ParagraphIsBulleted()
{
	PARAFORMAT pf = GetParagraphFormat();

	if (pf.wNumbering == PFN_BULLET)
		return true;
	else
		return false;
}

void CAutoRichEditCtrl::SelectColor()
{
	CColorDialog dlg;

	CHARFORMAT cf = GetCharFormat();

	if (cf.dwEffects & CFE_AUTOCOLOR) cf.dwEffects -= CFE_AUTOCOLOR;

	// Get a color from the common color dialog.
	if( dlg.DoModal() == IDOK )
	{	
		cf.crTextColor = dlg.GetColor();
	}

	cf.dwMask = CFM_COLOR;

	SetSelectionCharFormat(cf);
}

void CAutoRichEditCtrl::SetFontName(CString sFontName)
{
	CHARFORMAT cf = GetCharFormat();
	

	// Set the font name.
	for (INT i = 0; i <= sFontName.GetLength()-1; i++)
		cf.szFaceName[i] = (CHAR)sFontName[i];

	cf.dwMask = CFM_FACE;

	SetSelectionCharFormat(cf);
}

void CAutoRichEditCtrl::SetFontSize(INT nPoINTSize)
{
	CHARFORMAT cf = GetCharFormat();

	nPoINTSize *= 20;	// convert from to twips
	cf.yHeight = nPoINTSize;
	
	cf.dwMask = CFM_SIZE;

	SetSelectionCharFormat(cf);
}

void CAutoRichEditCtrl::GetSystemFonts(CStringArray &saFontList)
{
	CDC *pDC = GetDC ();

	EnumFonts (pDC->GetSafeHdc(),NULL,(FONTENUMPROC) CBEnumFonts,(LPARAM)&saFontList);//Enumerate

}

BOOL CALLBACK CAutoRichEditCtrl::CBEnumFonts(LPLOGFONT lplf, LPTEXTMETRIC lptm, DWORD dwType, LPARAM lpData)
{
	// This function was written with the help of CCustComboBox, by Girish Bharadwaj.
	// Available from Codeguru.

	if (dwType == TRUETYPE_FONTTYPE) 
	{
		((CStringArray *) lpData)->Add( lplf->lfFaceName );
	}

	return true;
}

CString CAutoRichEditCtrl::GetSelectionFontName()
{
	CHARFORMAT cf = GetCharFormat();

	CString sName = cf.szFaceName;

	return sName;
}

long CAutoRichEditCtrl::GetSelectionFontSize()
{
	CHARFORMAT cf = GetCharFormat();

	long nSize = cf.yHeight/20;

	return nSize;
}

void CAutoRichEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	/*
	CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
	m_pParent->SendMessage(WM_CHAR, 0, 0);
	*/
}

void CAutoRichEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	CRichEditCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

VOID CAutoRichEditCtrl::SetParent(CWnd *f_pParent)
{
	m_pParent = f_pParent;
}



void CAutoRichEditCtrl::SetMarcData(_TCHAR *f_pLineMarc)
{
	//HideSelection(TRUE,FALSE);        

	CHARFORMAT	cf;
	CString		strSel;
	_TCHAR		szChar[8];
	_TCHAR		*s_pOldContentData;
	INT			i;	
	INT			s_Textlen,s_OldTextlen;    
	BOOL		bCode;	
	CHARRANGE	cr;	
	
	SetRedraw(TRUE);

	s_pOldContentData = new _TCHAR[100000];
    GetWindowText(s_pOldContentData,100000);	
    s_OldTextlen = _tcsclen(s_pOldContentData);
	delete[] s_pOldContentData;
			
	SetWindowText(f_pLineMarc);
	//SetWindowText(f_pLineMarc);
	s_Textlen = _tcsclen(f_pLineMarc);	
	bCode = FALSE;

	INT		offset = 0;
	for ( i=0 ; i<s_Textlen ; i++ )
	{					
		SetSel(i+offset,i+1+offset);		
		GetSel(cr);				
		if ( cr.cpMax == cr.cpMin ) 
		{
			offset++;
			SetSel(i+offset,i+1+offset);
		}
		
		GetSelectionCharFormat(cf);
		GetSelText((char*)szChar);	
		strSel.Format(_T("%s"), szChar);
		if ( strSel == _T("▼") || bCode == TRUE) 
		{
			cf.bCharSet = GB2312_CHARSET;
			SetSelectionCharFormat(cf);
			if ( bCode == FALSE ) strcpy( cf.szFaceName, "Microsoft Sans Serif");			
			bCode = TRUE;
		}
		
		
		if ( bCode == FALSE )
		{
			switch ( (INT)cf.bCharSet )
			{
				case ANSI_CHARSET :
					break;
				case BALTIC_CHARSET :
					break;
				case CHINESEBIG5_CHARSET :
					break;
				case DEFAULT_CHARSET :
					break;
				case EASTEUROPE_CHARSET :
					break;
				case GB2312_CHARSET :
					cf.bCharSet = GB2312_CHARSET;
					strcpy( cf.szFaceName, "宋体");				
					break;
				case GREEK_CHARSET :
					cf.bCharSet = GB2312_CHARSET;
					strcpy( cf.szFaceName, "宋体");									
					break;
				case HANGUL_CHARSET :
					strcpy( cf.szFaceName, "Microsoft Sans Serif");
					break;
				case MAC_CHARSET :
					break;
				case OEM_CHARSET :
					break;
				case RUSSIAN_CHARSET :
					strcpy( cf.szFaceName, "Microsoft Sans Serif");				
					break;
				case SHIFTJIS_CHARSET :					
					cf.bCharSet = GB2312_CHARSET;
					strcpy( cf.szFaceName, "Microsoft Sans Serif");				
					break;
				case SYMBOL_CHARSET :
					break;
				case TURKISH_CHARSET :
					break;
				case JOHAB_CHARSET  :
					break;
				case ARABIC_CHARSET :
					break;
				case HEBREW_CHARSET  :
					break;
				case THAI_CHARSET  :
					break;
				default:
					cf.bCharSet = GB2312_CHARSET;
					strcpy( cf.szFaceName, "宋体");				
					break;					
			}
		}

		if ( strSel != _T("▼") && bCode == TRUE) bCode = FALSE;
		SetSelectionCharFormat(cf);
	}

	if ( s_OldTextlen > s_Textlen )
	{
		SetSel(s_Textlen, s_OldTextlen);
		ReplaceSel(_T(""));
	}	

	s_Textlen = GetLineCount();

	SetRedraw(TRUE);

	INT nRow =GetLineCount();
 
    HideSelection(FALSE,FALSE);	
}



/*
void CAutoRichEditCtrl::SetMarcData(CString *f_pLineMarc)
{
	//HideSelection(TRUE,FALSE);        

	CHARFORMAT	cf;
	CString		strSel;
	_TCHAR		szChar[8];
	_TCHAR		*s_pOldContentData;
	INT			i;	
	INT			s_Textlen,s_OldTextlen;    
	BOOL		bCode;	
	CHARRANGE	cr;
	
	
	SetRedraw(TRUE);

	s_pOldContentData = new _TCHAR[100000];
    GetWindowText(s_pOldContentData,100000);	
    s_OldTextlen = _tcsclen(s_pOldContentData);
	delete s_pOldContentData;
	
	SetWindowText(_T(""));
	//SetWindowText(f_pLineMarc);
	s_Textlen = _tcsclen(f_pLineMarc);	
	bCode = FALSE;
	for ( i=0 ; i<s_Textlen ; i++ )
	{					
		SetSel(i,i+1);
		GetSel(cr);		
		strSel.Format(_T("%c"), f_pLineMarc[i] );
		ReplaceSel(strSel);
		SetSel(i,i+1);
		
		GetSelectionCharFormat(cf);
		GetSelText((char*)szChar);	
		strSel.Format(_T("%s"), szChar);
		if ( strSel == _T("▼") || bCode == TRUE) 
		{
			cf.bCharSet = GB2312_CHARSET;
			SetSelectionCharFormat(cf);
			if ( bCode == FALSE ) strcpy( cf.szFaceName, "Microsoft Sans Serif");			
			bCode = TRUE;
		}
		
		
		if ( bCode == FALSE )
		{
			switch ( (INT)cf.bCharSet )
			{
				case ANSI_CHARSET :
					break;
				case BALTIC_CHARSET :
					break;
				case CHINESEBIG5_CHARSET :
					break;
				case DEFAULT_CHARSET :
					break;
				case EASTEUROPE_CHARSET :
					break;
				case GB2312_CHARSET :
					break;
				case GREEK_CHARSET :
					break;
				case HANGUL_CHARSET :
					break;
				case MAC_CHARSET :
					break;
				case OEM_CHARSET :
					break;
				case RUSSIAN_CHARSET :
					strcpy( cf.szFaceName, "Microsoft Sans Serif");				
					break;
				case SHIFTJIS_CHARSET :					
					cf.bCharSet = GB2312_CHARSET;
					strcpy( cf.szFaceName, "Microsoft Sans Serif");				
					break;
				case SYMBOL_CHARSET :
					break;
				case TURKISH_CHARSET :
					break;
				case JOHAB_CHARSET  :
					break;
				case ARABIC_CHARSET :
					break;
				case HEBREW_CHARSET  :
					break;
				case THAI_CHARSET  :
					break;
			}
		}

		if ( strSel != _T("▼") && bCode == TRUE) bCode = FALSE;
		SetSelectionCharFormat(cf);
	}

	if ( s_OldTextlen > s_Textlen )
	{
		SetSel(s_Textlen, s_OldTextlen);
		//ReplaceSel(_T(""));
	}	

	s_Textlen = GetLineCount();

	SetRedraw(TRUE);

	INT nRow =GetLineCount();
 
    HideSelection(FALSE,FALSE);	
}
*/

void CAutoRichEditCtrl::InitFont()
{   	
	GetDefaultCharFormat( m_defaultcf );	

	m_defaultcf.cbSize = sizeof(CHARFORMAT); 
	m_defaultcf.dwMask = CFM_COLOR | CFM_FACE; 	
	m_defaultcf.dwEffects = 0;
	m_defaultcf.yHeight = -13;
	//m_defaultcf.yOffset = ; 
	m_defaultcf.crTextColor =RGB(0,0,0); 
	m_defaultcf.bCharSet = 134; 
	m_defaultcf.bPitchAndFamily = 2; 
	strcpy(m_defaultcf.szFaceName, "宋体");
	SetDefaultCharFormat( m_defaultcf );
	SetRedraw(FALSE);		
	return;
}
