#include "stdafx.h"
#include "AutoRichEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAutoRichEditCtrl::CAutoRichEditCtrl()
{
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

CString CAutoRichEditCtrl::GetRTF()
{
	EDITSTREAM es;
	es.dwError = 0;
	es.pfnCallback = CBStreamOut;		
	CString sRTF = _T("");
	es.dwCookie = (DWORD) &sRTF;	
	StreamOut(SF_RTF, es);			
	
	return sRTF;

}

VOID CAutoRichEditCtrl::SetRTF(CString sRTF)
{	
	EDITSTREAM es;
	es.dwError = 0;
	es.pfnCallback = CBStreamIn;
	es.dwCookie = (DWORD) &sRTF;
	StreamIn(SF_RTF, es);		
}

DWORD CALLBACK CAutoRichEditCtrl::CBStreamIn(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	
	CString *pstr = (CString *) dwCookie;

	if (pstr->GetLength() < cb)
	{
		*pcb = pstr->GetLength();
		memcpy(pbBuff, (LPCTSTR)*pstr->GetBuffer(0), *pcb);
		pstr->Empty();
	}
	else
	{
		*pcb = cb;
		memcpy(pbBuff, (LPCTSTR) *pstr->GetBuffer(0), *pcb);
		*pstr = pstr->Right(pstr->GetLength() - cb);
	}

	return 0;
}

DWORD CALLBACK CAutoRichEditCtrl::CBStreamOut(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	
	CString *psEntry = (CString*) dwCookie;
	

	CString tmpEntry = _T("");
	tmpEntry = (CString) pbBuff;

	
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

VOID CAutoRichEditCtrl::SetCharStyle(INT MASK, INT STYLE, INT nStart, INT nEnd)
{
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	
	
	GetSelectionCharFormat(cf);
	
	if (cf.dwMask & MASK)	
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

VOID CAutoRichEditCtrl::SetSelectionBold()
{
	long start=0, end=0;
	GetSel(start, end);		

	SetCharStyle(CFM_BOLD, CFE_BOLD, start, end);	
}

VOID CAutoRichEditCtrl::SetSelectionItalic()
{
	long start=0, end=0;
	GetSel(start, end);

	SetCharStyle(CFM_ITALIC, CFE_ITALIC, start, end);
}

VOID CAutoRichEditCtrl::SetSelectionUnderlined()
{
	long start=0, end=0;
	GetSel(start, end);

	SetCharStyle(CFM_UNDERLINE, CFE_UNDERLINE, start, end);
}

VOID CAutoRichEditCtrl::SetParagraphCenter()
{
	PARAFORMAT paraFormat;    
	paraFormat.cbSize = sizeof(PARAFORMAT);
	paraFormat.dwMask = PFM_ALIGNMENT;    
	paraFormat.wAlignment = PFA_CENTER;
	
	SetParaFormat(paraFormat);	
}

VOID CAutoRichEditCtrl::SetParagraphLeft()
{
	PARAFORMAT paraFormat;
	paraFormat.cbSize = sizeof(PARAFORMAT);
	paraFormat.dwMask = PFM_ALIGNMENT;    
	paraFormat.wAlignment = PFA_LEFT;
	
	SetParaFormat(paraFormat);
}

VOID CAutoRichEditCtrl::SetParagraphRight()
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

VOID CAutoRichEditCtrl::SetParagraphBulleted()
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

VOID CAutoRichEditCtrl::SelectColor()
{
	CColorDialog dlg;

	CHARFORMAT cf = GetCharFormat();

	if (cf.dwEffects & CFE_AUTOCOLOR) cf.dwEffects -= CFE_AUTOCOLOR;

	
	if( dlg.DoModal() == IDOK )
	{	
		cf.crTextColor = dlg.GetColor();
	}

	cf.dwMask = CFM_COLOR;

	SetSelectionCharFormat(cf);
}

VOID CAutoRichEditCtrl::SetFontName(CString sFontName)
{
	CHARFORMAT cf = GetCharFormat();
	

	
	for (INT i = 0; i <= sFontName.GetLength()-1; i++)
		cf.szFaceName[i] = (CHAR)sFontName[i];

	cf.dwMask = CFM_FACE;

	SetSelectionCharFormat(cf);
}

VOID CAutoRichEditCtrl::SetFontSize(INT nPoINTSize)
{
	CHARFORMAT cf = GetCharFormat();

	nPoINTSize *= 20;	
	cf.yHeight = nPoINTSize;
	
	cf.dwMask = CFM_SIZE;

	SetSelectionCharFormat(cf);
}

VOID CAutoRichEditCtrl::GetSystemFonts(CStringArray &saFontList)
{
	CDC *pDC = GetDC ();

	EnumFonts (pDC->GetSafeHdc(),NULL,(FONTENUMPROC) CBEnumFonts,(LPARAM)&saFontList);

}

BOOL CALLBACK CAutoRichEditCtrl::CBEnumFonts(LPLOGFONT lplf, LPTEXTMETRIC lptm, DWORD dwType, LPARAM lpData)
{
	
	

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

VOID CAutoRichEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	

}

VOID CAutoRichEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	CRichEditCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

VOID CAutoRichEditCtrl::SetParent(CWnd *f_pParent)
{
	m_pParent = f_pParent;
}



VOID CAutoRichEditCtrl::SetMarcData(_TCHAR *f_pLineMarc)
{
	

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
		GetSelText((LPSTR)szChar);	
		strSel.Format(_T("%s"), szChar);
		if ( strSel == _T("▼") || bCode == TRUE) 
		{
			cf.bCharSet = GB2312_CHARSET;
			SetSelectionCharFormat(cf);
			if ( bCode == FALSE ) _tcscpy( (LPTSTR)(LPCTSTR)cf.szFaceName, _T("Microsoft Sans Serif"));			
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
					_tcscpy( (LPTSTR)(LPCTSTR)cf.szFaceName, _T("宋体"));				
					break;
				case GREEK_CHARSET :
					cf.bCharSet = GB2312_CHARSET;
					_tcscpy( (LPTSTR)(LPCTSTR)cf.szFaceName, _T("宋体"));									
					break;
				case HANGUL_CHARSET :
					_tcscpy( (LPTSTR)(LPCTSTR)cf.szFaceName, _T("Microsoft Sans Serif"));
					break;
				case MAC_CHARSET :
					break;
				case OEM_CHARSET :
					break;
				case RUSSIAN_CHARSET :
					_tcscpy( (LPTSTR)(LPCTSTR)cf.szFaceName, _T("Microsoft Sans Serif"));				
					break;
				case SHIFTJIS_CHARSET :					
					cf.bCharSet = GB2312_CHARSET;
					_tcscpy( (LPTSTR)(LPCTSTR)cf.szFaceName, _T("Microsoft Sans Serif"));				
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
					_tcscpy( (LPTSTR)(LPCTSTR)cf.szFaceName, _T("宋体"));				
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

VOID CAutoRichEditCtrl::InitFont()
{   	
	GetDefaultCharFormat( m_defaultcf );	

	m_defaultcf.cbSize = sizeof(CHARFORMAT); 
	m_defaultcf.dwMask = CFM_COLOR | CFM_FACE; 	
	m_defaultcf.dwEffects = 0;
	m_defaultcf.yHeight = -13;
	
	m_defaultcf.crTextColor =RGB(0,0,0); 
	m_defaultcf.bCharSet = 134; 
	m_defaultcf.bPitchAndFamily = 2; 
	_tcscpy((LPTSTR)(LPCTSTR)m_defaultcf.szFaceName, _T("宋体"));
	SetDefaultCharFormat( m_defaultcf );
	SetRedraw(FALSE);		
	return;
}
