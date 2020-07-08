// ECO_ControlAutoList.cpp : implementation file
//
// @Copyright(c) ECO / Design By PDJc++ / Since 2003.7.15
//
//
#include "stdafx.h"
#include "ECO_ControlAutoList.h"
#include "ESL_ControlEdit.h"
#include "ECO_FILE_API.h"
#include "ECO_FileIO.h"
#include "ECO_STANDARD_FUNC.h"
#include "except.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CECO_ControlAutoList dialog


CECO_ControlAutoList::CECO_ControlAutoList(CWnd* pParent /*=NULL*/)
	: CDialog(CECO_ControlAutoList::IDD, pParent)
{
EFS_BEGIN
	m_strarrayKeyWordList.RemoveAll();
	m_strArraySearchKeyWordList.RemoveAll();
	m_strarrayDeleteString.RemoveAll();
	m_bResourceFileSearchFlag = TRUE;
	m_pParentEdit = NULL;
	m_nResourceID = 0;
	m_pParentWnd = (CESL_ControlEdit*) pParent;
	m_bSelectAllDisplay = FALSE;
	m_bAutoListUseFlag = FALSE;
EFS_END
}

CECO_ControlAutoList::~CECO_ControlAutoList()
{
EFS_BEGIN
	
EFS_END
}


void CECO_ControlAutoList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CECO_ControlAutoList)
	DDX_Control(pDX, IDC_LIST_BOX, m_ctlListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CECO_ControlAutoList, CDialog)
	//{{AFX_MSG_MAP(CECO_ControlAutoList)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_LIST_BOX, OnSelchangeListBox)
	ON_WM_CHAR()
	ON_WM_TIMER()
	ON_LBN_DBLCLK(IDC_LIST_BOX, OnDblclkListBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECO_ControlAutoList message handlers

BOOL CECO_ControlAutoList::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CECO_ControlAutoList::OnInitDialog() 
{
EFS_BEGIN
	CDialog::OnInitDialog();	
	
	SetTimer(1, 100, NULL);
	
EFS_END
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CECO_ControlAutoList::SearchKeyWord()
{
EFS_BEGIN
	INT		i;
	INT		nListCount;
	INT		nSearchCount;
		
	nListCount = m_ctlListBox.GetCount(); 	
	m_ctlListBox.ResetContent();
	
	m_strArraySearchKeyWordList.RemoveAll();
	nListCount = m_strarrayKeyWordList.GetSize();

	if ( nListCount == 0 ) return FALSE;		
	

	//ÇÔ¼ö
	Search(m_strKeyWord);
	//end	
	nSearchCount = m_strArraySearchKeyWordList.GetSize();	
	for ( i=0 ; i<nSearchCount ; i++ )
	{
		m_ctlListBox.AddString(m_strArraySearchKeyWordList.GetAt(i));
	}

	
	if ( nSearchCount <= 0 ) 
	{
		m_pParentWnd->m_bAutoListFocus = FALSE;
		ShowWindow(SW_HIDE);
		{return FALSE;}
	} 
	else if ( nSearchCount == 1 )
	{
		if ( m_strarrayKeyWordList.GetAt(0) == m_strKeyWord )
		{
			m_pParentWnd->m_bAutoListFocus = FALSE;
			ShowWindow(SW_HIDE);
			{return FALSE;			}
		}	
		else 
		{
			ShowWindow(SW_SHOWNOACTIVATE);
			m_ctlListBox.SetSel(0, -1);
			m_pParentWnd->SetFocus();
		}
	}
	else
	{
		ShowWindow(SW_SHOWNOACTIVATE);
		m_ctlListBox.SetSel(0, -1);
		m_pParentWnd->SetFocus();
	}

	CRect m_rect;
	m_pParentWnd->GetClientRect(&m_rect);
	m_pParentWnd->ClientToScreen(m_rect);		
	m_rect.left -= 2;
	m_rect.top += m_rect.bottom - m_rect.top+2;
	if ( (nSearchCount * 13 + 5) > 150 )
	{
		m_rect.bottom = m_rect.top + 150;
	}
	else
	{
		m_rect.bottom = m_rect.top + nSearchCount * 13 + 5; 
	}
	MoveWindow(m_rect);	

EFS_END
	return TRUE;
}

void CECO_ControlAutoList::SetKeyWord(_TCHAR *f_pKeyWordIn)
{
EFS_BEGIN
	m_strKeyWord.Format(_T("%s"), f_pKeyWordIn);
EFS_END
}

void CECO_ControlAutoList::Display()
{	
EFS_BEGIN
			
	if ( m_strarrayKeyWordList.GetSize() > 0 ) {return;}
	m_bResourceFileSearchFlag = ReferenceLoad();
	if ( m_bResourceFileSearchFlag == FALSE ) {return;}

EFS_END
}

BOOL CECO_ControlAutoList::ReferenceLoad()
{
EFS_BEGIN
	FILE	*fp;
	_TCHAR	szFileName[64];
	_TCHAR	szTemp[1024];
	INT		line_length;
	UINT	nResourceID;
	BOOL	bFlag;
    
	// Resource Define Check
	fp = FdkFopen( _T("..\\CFG\\AutoList.ini"), _T("r") );
	if ( fp == NULL ) {return FALSE;}
	
	line_length = 0;
	bFlag = FALSE;
	do {
		if (CECO_FileIO::next_line(fp, szTemp, &line_length) == -1) { break; }
		nResourceID = _ttoi(szTemp);
		if ( nResourceID == m_nResourceID )
		{
			bFlag = TRUE;
			break;
		}		
	} while(1);
	FdkFclose( fp );

	if ( bFlag == FALSE ) {return FALSE;}
	m_bAutoListUseFlag = TRUE;

	// Resource File Check
	_stprintf(szFileName, _T("..\\Cfg\\EditBoxAutoList\\%d.txt"), m_nResourceID );
	fp = FdkFopen( szFileName, _T("r") );
	if ( fp == NULL ) {return FALSE;}

	line_length = 0;
	do {
		if (CECO_FileIO::next_line(fp, szTemp, &line_length) == -1) { break; }
		if ( szTemp[0] == _T('.') ) continue;
		if ( szTemp[0] == _T('\n') ) continue;
		CECO_FileIO::str_rtrim(szTemp, szTemp, '\n');
		if ( _tcslen(szTemp) >1 ) 
		{
			m_strarrayKeyWordList.Add( (LPCTSTR)&szTemp );		
		}
	} while(1);

	if ( m_strarrayKeyWordList.GetSize() <= 0 ) 
	{
		FdkFclose( fp );
		{return FALSE;}
	}
	FdkFclose( fp );

EFS_END
	return TRUE;
}

void CECO_ControlAutoList::SetResource(UINT f_nResourceIDIn, CWnd *pWnd)
{
EFS_BEGIN
	m_nResourceID = f_nResourceIDIn;
	m_pParentEdit = (CESL_ControlEdit*) pWnd;
EFS_END
}

INT CECO_ControlAutoList::ReferenceWrite()
{	
EFS_BEGIN
	if ( m_bAutoListUseFlag == FALSE ) {return 0;}
	INT		i, y;
	INT		nListCount;	
	INT		offset;
	INT		nSearchIndex;
	BOOL	bFlag;	

	// memory Search
	if ( m_strKeyWord.IsEmpty() ) {return 0;}

	bFlag = FALSE;
	m_strArraySearchKeyWordList.RemoveAll();
	nListCount = m_strarrayKeyWordList.GetSize();
	for ( i=0 ; i<nListCount ; i++ )
	{
		if ( m_strarrayKeyWordList.GetAt(i).Find(m_strKeyWord, 0) == 0 )
		{
			bFlag = TRUE;
			nSearchIndex = i;
			break;
		}
	}		
	if ( bFlag == FALSE && m_strKeyWord.IsEmpty() ) {return 0;}

	// File Write
	FILE	*fp;
	_TCHAR	szFileName[64];
	_TCHAR	szTemp[1024];
    
	// Resource File Check
	_stprintf(szFileName, _T("..\\cfg\\EditBoxAutoList\\%d.txt"), m_nResourceID );
	fp = FdkFopen( szFileName, _T("w") );
	if ( fp == NULL ) {return FALSE;}

	offset = 0;
	// write new keyword
	offset += _stprintf(szTemp+offset, _T("%s\n"), m_strKeyWord );

	// write old keyword
	nListCount = m_strarrayKeyWordList.GetSize();
	CString	str1, str2;
	BOOL	nDeleteFind;
	if ( nListCount > 0 )
	{		
		for ( i=0 ; i<nListCount ; i++ )
		{	
			// History Limit Count
			if ( i+1 >= 100 ) continue;
			if ( bFlag == TRUE && i == nSearchIndex ) continue;
			
			// Delete Keyword
			str1 = m_strarrayKeyWordList.GetAt(i);
			nDeleteFind = FALSE;
			for ( y=0 ; y<m_strarrayDeleteString.GetSize() ; y++ )
			{				
				str2 = m_strarrayDeleteString.GetAt(y); 
				if ( str1 == str2  )
				{
					m_strarrayDeleteString.RemoveAt(y);
					nDeleteFind = TRUE;
					break;
				}				
			}				

			if ( nDeleteFind == TRUE ) continue;

			// Add Keyword
			offset += _stprintf( szTemp+offset, _T("%s\n"), m_strarrayKeyWordList.GetAt(i) );
			if ( _tcslen(szTemp) > 512 )
			{
				FdkFprintf( fp, szTemp );
				szTemp[0] = '\0';
				offset = 0;
			}
		}				
	}	

	if ( szTemp[0] != NULL ) FdkFprintf( fp, szTemp );
	FdkFclose( fp );

EFS_END
	return TRUE;	
}

void CECO_ControlAutoList::OnSize(UINT nType, int cx, int cy) 
{
EFS_BEGIN
	CDialog::OnSize(nType, cx, cy);	

	if ( m_ctlListBox.GetSafeHwnd() != NULL ) {
		CRect rect;	
		GetClientRect(&rect);				
		m_ctlListBox.MoveWindow(rect);
	}
EFS_END
}

void CECO_ControlAutoList::OnSelchangeListBox() 
{
EFS_BEGIN
	PrevNextSelection(4);	
EFS_END
}

void CECO_ControlAutoList::OnDblclkListBox() 
{	
EFS_BEGIN

	CString	strData;
	
	SelectSel();		
	m_pParentWnd->m_bAutoListFocus = FALSE;
	ShowWindow(SW_HIDE);
EFS_END
}

void CECO_ControlAutoList::SelectSel()
{
EFS_BEGIN
	CString	strData;
	INT		nSelectedSel;
	INT		nCurIdx;

	nSelectedSel = m_ctlListBox.GetCurSel();	
	m_ctlListBox.GetText(nSelectedSel, strData);
	
	m_pParentWnd->m_bAutoListFocus = TRUE;				
	m_pParentWnd->SetWindowText(strData);
	
	m_strKeyWord = strData;
	m_strArraySearchKeyWordList.RemoveAll();

	nCurIdx = strData.GetLength();		
	m_pParentWnd->SetFocus();
	m_pParentWnd->SetSel(nCurIdx, nCurIdx+1 );
	
EFS_END
}

INT CECO_ControlAutoList::SelectAllList()
{
EFS_BEGIN
	INT		i, y;
	INT		nListCount;

	m_bSelectAllDisplay = TRUE;	
	m_ctlListBox.ResetContent();	
	m_strArraySearchKeyWordList.RemoveAll();

	nListCount = m_strarrayKeyWordList.GetSize();
	if ( nListCount <=  0 ) {return 0;}

	CString	str1, str2;
	BOOL	nDeleteFind;
	for ( i=0 ; i<nListCount ; i++ )
	{			
		str1 = m_strarrayKeyWordList.GetAt(i);
		nDeleteFind = FALSE;
		for ( y=0 ; y<m_strarrayDeleteString.GetSize() ; y++ )
		{
			str2 = m_strarrayDeleteString.GetAt(y); 
			if ( str1 == str2  )
			{
				nDeleteFind = TRUE;
				break;
			}
		}
		if ( nDeleteFind == TRUE ) continue;
		m_strArraySearchKeyWordList.Add(m_strarrayKeyWordList.GetAt(i));					
		m_ctlListBox.AddString(m_strarrayKeyWordList.GetAt(i));	
	}	
	
	if ( m_ctlListBox.GetCount() > 0 )	
	{
		CRect m_rect;
		m_pParentWnd->GetClientRect(&m_rect);
		m_pParentWnd->ClientToScreen(m_rect);		
		m_rect.left -= 2;
		m_rect.top += m_rect.bottom - m_rect.top+2;
		if ( (nListCount * 13 + 5) > 150 )
		{
			m_rect.bottom = m_rect.top + 150;
		}
		else
		{
			m_rect.bottom = m_rect.top + nListCount * 13 + 5; 
		}
		MoveWindow(m_rect);
		ShowWindow(SW_SHOWNOACTIVATE);
	}

EFS_END
	return 0;

}

void CECO_ControlAutoList::OnTimer(UINT nIDEvent) 
{	
EFS_BEGIN

	if ( nIDEvent == 1 )
	{
		// Delete AutoList Check
		CWnd*	pWnd;
		pWnd = GetFocus();
		if ( pWnd == NULL ) {return;}

		CWnd	*p1;	
		p1 =  GetDlgItem(IDC_LIST_BOX);
		
		if ( pWnd != m_pParentWnd && pWnd != p1) 
		{
			KillTimer(1);
			m_pParentWnd->DeleteAutoList();			
			{return;}
		}
	}
	
	CDialog::OnTimer(nIDEvent);

EFS_END
}

void CECO_ControlAutoList::DeleteAutoListKeyWord()
{
EFS_BEGIN
	CString	strData;
	INT		nSelectedSel;
	INT		nCurIdx;
	INT		nCount;
	
	// Delete Auto ListBox Display
	nSelectedSel = m_ctlListBox.GetCurSel();
	m_ctlListBox.GetText(nSelectedSel, strData);
	m_strarrayDeleteString.Add(strData);
	m_ctlListBox.DeleteString(nSelectedSel);	

	if ( m_strArraySearchKeyWordList.GetSize() > 0 )
		m_strArraySearchKeyWordList.RemoveAt(nSelectedSel);	
	nCount = m_ctlListBox.GetCount();
	
	// Set Cursor
	if ( nCount == 0 ) 
	{
		m_pParentWnd->m_bAutoListFocus = FALSE;
		ShowWindow(SW_HIDE);
		{return;}	
	}
	if ( nCount-1 >= nSelectedSel ) m_ctlListBox.SetCurSel(nSelectedSel);
	else if ( (nCount-1 < nSelectedSel) && (nCount > 0)) m_ctlListBox.SetCurSel(nSelectedSel-1);		
	
	// Set Edit Box
	nSelectedSel = m_ctlListBox.GetCurSel();	
	m_ctlListBox.GetText(nSelectedSel, strData);
	m_pParentWnd->SetWindowText(strData);
	m_strKeyWord = strData;
	nCurIdx = strData.GetLength();		
	m_pParentWnd->SetSel(nCurIdx, nCurIdx+1 );			
		
EFS_END
	return;

}

void CECO_ControlAutoList::PrevNextSelection(INT nType)
{
	INT		nIndex, nCount;

	CString	strText;			
	nIndex = m_ctlListBox.GetCurSel();
	nCount = m_ctlListBox.GetCount();

	m_pParentWnd->GetWindowText(strText);

	if ( strText.IsEmpty() )
	{
		nType = 1;
	}	

	switch ( nType )
	{
		case 1:  // Show AutoList
			{				
				INT	nSearchKeywordCount;				
				
				if ( strText.IsEmpty() ) 
				{
					SelectAllList();
				}
				
				nSearchKeywordCount = m_strArraySearchKeyWordList.GetSize();
				if ( nSearchKeywordCount > 0 )
				{
					m_pParentWnd->m_bAutoListFocus = TRUE;				
					m_ctlListBox.GetText(0, strText);
					m_pParentWnd->SetWindowText(strText);			
					m_ctlListBox.SetCurSel(0);
					nIndex = strText.GetLength();
					m_pParentWnd->SetSel(nIndex, nIndex+1 );
					//m_pParentWnd->SetFocus();

				}
			}
			break;
		case 2: // Down		
			if ( nCount-1 > nIndex )
			{
				m_ctlListBox.SetCurSel(nIndex+1);
				SelectSel();	
			}						
			break;
		case 3: // Up
			if ( nIndex > 0 )
			{
				m_ctlListBox.SetCurSel(nIndex-1);
				SelectSel();				
			}
			break;
		case 4: // Mouse Click
			SelectSel();
			break;
	}
}



// KOL.UDF.022
wint_t CECO_ControlAutoList::FindBlock(TCHAR *str ,INT (*Infor)[2] , INT k )
//wint_t CECO_ControlAutoList::FindBlock(TCHAR *str ,INT (*Infor)[3] , INT k )
{ 
	//°Ë»ö¾îÀÇ ¹è¿­Á¤º¸¸¦ Ã£¾Æ¼­ ÀúÀåÇÏ´Â ÇÔ¼ö
	BOOL bChangeCho = FALSE;
	INT nTail = 0;
	
	INT Chosung[19] = {0, 1, 3, 6, 7, 8, 16, 17, 18, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29};

	if( k != 3 )
	{	
		if( 12593<=*str && *str <=12622 /*"¤¾"°ª 0x3140*/ ) //ÃÊ¼ºÀÏ °æ¿ì
		{ 			
			INT nJongSung = 0;
			
			INT nInputChosung = *str - 0x3131/*"¤¡"°ª 0x3131*/;
			while( nInputChosung < 30 )
			{
				if( Chosung[nJongSung] == nInputChosung ) //¹è¿­°ª°ú ºñ±³ÈÄ ¼øÂ÷ÀûÀÎ p°ªÀ¸·Î °è»êÇÏµµ·Ï ÇÔ
				{
					nInputChosung = nJongSung;
					break;
				}
				else
				{
					nJongSung++;
				}
				// 2011.09.16 ADD BY PWR : ¹«ÇÑ·çÇÁÁ¦°Å
				if(19 <= nJongSung)
				{
					return 0;
				}
			}
			Infor[k][0] = 588*nInputChosung;
			Infor[k][1] = 587+(588*nInputChosung);
		}
		else if( 44032<=*str && *str<=55203 /*ÇÑ±Û¹üÀ§ °¡ ~ ÆR*/ ) 
		{ 				
			INT i=((*str - 0xAC00)/28)/21; //19È©°³ ºí·°ÁßÇÏ³ª
			INT j=((*str - 0xAC00)/28)%21; //ÇÏ³ªÀÇ ºí·°³»¿¡¼­ 21ÁÙ Áß ÇÏ³ª¶õ ¼Ò¸®
			
			Infor[k][0]=*str-0xAC00; // Ã¹ ±ÛÀÚ°ªÀ» Infor¹è¿­ÀÇ 0¹øÂ° ³Ö±â
			
			if( (*str - 0xAC00)%28 == 0 ) // ÇÑÁÙÀÇ ¸Ç¾Õ±ÛÀÚ¶ó¸é
			{
				if( *str-0xAC00 == i*588+224 ) // "¤Ç" ÀÏ¶§ "¤È" "¤É" "¤Ê" ±îÁö °Ë»ö
					Infor[k][1] = Infor[k][0] + 111;
				else if( *str-0xAC00 == i*588+364 ) // "¤Ì" ÀÏ¶§ "¤Í" "¤Î" "¤Ï" ±îÁö °Ë»ö
					Infor[k][1] = Infor[k][0] + 111;
				else if( *str-0xAC00 == i*588+504 ) // "¤Ñ" ÀÏ¶§ "¤Ò" ±îÁö °Ë»ö
					Infor[k][1] = Infor[k][0] + 51;
				else
					Infor[k][1] = Infor[k][0] + 27; //±× ±ÛÀÚ°¡ ÀÖ´Â ÁÙ¿¡¼­ ¸¶Áö¸· ±ÛÀÚ (27¹øÂ°)
			}
			else
				Infor[k][1]= (i*588) + 27 + (28*j); //±×±ÛÀÚ°¡ ÀÖ´Â ÁÙ¿¡¼­ ¸¶Áö¸·¿¡ÀÖ´Â ±ÛÀÚ
		} 
		else
		{
			Infor[k][0]=*str;
			Infor[k][1]=2; //±âÅ¸¾ð¾î ´Ü¼øºñ±³
		}
	}
	else
	{
		if( Infor[0][0]%28 != 0 ) //Á¾¼ºÀ» °¡Áö°í ÀÖ´Â ±ÛÀÚ¶õ ¼Ò¸®
		{
			INT i=((Infor[0][0])/28)/21; //19È©°³ ºí·°ÁßÇÏ³ª
			INT j=((Infor[0][0])/28)%21; //ÇÏ³ªÀÇ ºí·°³»¿¡¼­ 21ÁÙ Áß ÇÏ³ª¶õ ¼Ò¸®
			INT w = (i*588) + (28*j); //44032¸¦ »« ±×ÁÙÀÇ °¡Àå ¾ÕÀÇ ¼ýÀÚ
			
			nTail = Infor[0][0] - w;//Á¾¼ºÀÌ ÀÖ´Â±ÛÀÚ¿¡¼­ Á¾¼ºÀÌ ¾ø´Â ±ÛÀÚ¸¦ »©¸é Á¾¼º ¹è¿­ÀÇ ¼ø¼­
			INT nDivideTail = 0;
			
			if( nTail==3 )//ÀÔ·ÂÇÑ Á¾¼ºÀÌ µÎ°³ÀÇ ÃÊ¼ºÀ¸·Î ÀÌ·ç¾îÁ® ÀÖ´Ù¸é..
			{
				bChangeCho = TRUE;
				nTail = 19;
				nDivideTail = 2;
			}
			else if( nTail==12 )
			{
				bChangeCho = TRUE;
				nTail = 19;
				nDivideTail = 4;
			}
			else if( nTail==18 )
			{
				bChangeCho = TRUE;
				nTail = 19;
				nDivideTail = 1;
			}
			else if( nTail==5 )
			{
				bChangeCho = TRUE;
				nTail = 22;
				nDivideTail = 1;
			}
			else if( nTail==6 )
			{
				bChangeCho = TRUE;
				nTail = 27;
				nDivideTail = 2;
			}
			else if( nTail==9 )
			{	
				bChangeCho = TRUE;
				nTail = 1;
				nDivideTail = 1;
			}
			else if( nTail==10 )
			{
				bChangeCho = TRUE;
				nTail = nTail+6;
				nDivideTail = 2;
			}
			else if( nTail==11 )
			{
				bChangeCho = TRUE;
				nTail = nTail+6;
				nDivideTail = 3;
			}
			else if( nTail==13 )
			{
				bChangeCho = TRUE;
				nTail = nTail+12;
				nDivideTail = 5;
			}
			else if( nTail==14 )
			{
				bChangeCho = TRUE;
				nTail = nTail+12;
				nDivideTail = 6;
			}
			else if( nTail==15 )
			{
				bChangeCho = TRUE;
				nTail = nTail+12;
				nDivideTail = 7;
			}	
			else
				nTail = 0;
			
			if( bChangeCho )//º¯ÇßÀ¸¸é µÎ°³ÀÌ»óÀÇ Á¾¼ºÀÌ ÀÖ´Ù´Â °ÍÀÓ
			{
				Infor[2][0] = Infor[0][0] - nDivideTail;					
			}
		}
	}	
	return nTail;
}

BOOL CECO_ControlAutoList::SelectLine(TCHAR *str, INT nIsChangedJongSung, INT (*Infor)[2])
{
	BOOL bCheck = FALSE;
	
	INT nTail = 0;
	INT i=((Infor[0][0])/28)/21; 
	INT j=((Infor[0][0])/28)%21; 
	INT nNoJongSung = (i*588) + (28*j); 

	INT nGarbage = FindBlock( str, Infor, 1 );

	static const TCHAR wcTop[] = {L'¤¡', 
		L'¤¢', L'¤¤', L'¤§', L'¤¨', L'¤©',       L'¤±', L'¤²', L'¤³', L'¤µ', L'¤¶', 
		L'¤·', L'¤¸', L'¤¹', L'¤º', L'¤»',       L'¤¼', L'¤½', L'¤¾'}; 

    static const TCHAR wcTail[] = {L' ', 
		L'¤¡', L'¤¢', L'¤£', L'¤¤', L'¤¥',       L'¤¦', L'¤§', L'¤©', L'¤ª', L'¤«', 
		L'¤¬', L'¤­', L'¤®', L'¤¯', L'¤°',       L'¤±', L'¤²', L'¤´', L'¤µ', L'¤¶', 
		L'¤·', L'¤¸', L'¤º', L'¤»', L'¤¼',       L'¤½', L'¤¾'};


	if( Infor[1][1] != 2 && Infor[0][1] != 2)
	{
		if( Infor[0][0]%28 != 0 ) //Á¾¼ºÀ» °¡Áö°í ÀÖ´Â ±ÛÀÚ¶õ ¼Ò¸®
		{
			nTail = Infor[0][0] - nNoJongSung;//Á¾¼ºÀÌ ÀÖ´Â±ÛÀÚ¿¡¼­ Á¾¼ºÀÌ ¾ø´Â ±ÛÀÚ¸¦ »©¸é Á¾¼º ¹è¿­ÀÇ ¼ø¼­

			if( nIsChangedJongSung != 0 ) //Á¾¼ºÀ» µÎ°³°¡Áø ±ÛÀÚ¶ó¸é
			{
				if( *str-0XAC00 == Infor[0][0] )//´Ü¼øºñ±³¿¡¼­ ¸Â¾Æ¹ö¸°´Ù¸é ¤¡¤¡
				{
					bCheck = TRUE;
				}
				else
				{
					if( Infor[2][0] == *str-0XAC00 ) //¹Ù²ï ±ÛÀÚ·Î ºñ±³¸¦ ÇØº¸°í
					{		
						str++;
												
						//ÀÔ·ÂÀÇ Á¾¼º°ú ´ÙÀ½¹®ÀÚ¿­ÀÇ ÃÊ¼ºÀÌ °°´Ù¸é
						if(wcTail[nIsChangedJongSung]==wcTop[(((*str-0XAC00)/28)/21)])
							bCheck = TRUE;
						else
							bCheck = FALSE;
					}
					else
						bCheck = FALSE;
				}
			}
			else//Á¾¼ºÀ» ÇÏ³ª°¡Áø ±ÛÀÚ¶ó¸é
			{
				if( *str-0XAC00 == Infor[0][0] )//´Ü¼øºñ±³¿¡¼­ ¸Â¾Æ¹ö¸°´Ù¸é ¤¡¤¡
				{
					bCheck = TRUE;				
				}
				else
				{
					if( *str-0XAC00 == nNoJongSung )//ÀÔ·Â°ªÀÇ Á¾¼ºÀ» »«°Í°ú ºñ±³¸¦ ÇÏ°í
					{
						str++;
												
						//ÀÔ·ÂÀÇ Á¾¼º°ú ´ÙÀ½¹®ÀÚ¿­ÀÇ ÃÊ¼ºÀÌ °°´Ù¸é
						if(wcTail[nTail]==wcTop[(((*str-0XAC00)/28)/21)])
							bCheck = TRUE;
						else
							bCheck = FALSE;	
					}
					else
						bCheck = FALSE;
				}
			}
		}
		else //Á¾¼ºÀÌ ¾ø´Â ±ÛÀÚ
		{
			if( Infor[0][0] <= *str-0XAC00 && *str-0XAC00 <= Infor[0][1] ) //¹üÀ§¾È¿¡ µé¾î¿Ã¶§
				bCheck = TRUE;
			else
				bCheck = FALSE;
		}
	}
	else //2. ±âÅ¸¾ð¾î ´Ü¼øºñ±³
	{
		if(*str == Infor[0][0]) //Á¤È®È÷ °°´Ù¸é
			bCheck = TRUE;
		else
			bCheck = FALSE;		
	}	

	return bCheck;
}


CString CECO_ControlAutoList::Search(CString strText)
{
	CString strShowData = _T("");	

	if( strText.GetLength() > 0 )
	{
		if( strText.GetLength() == 1 )
		{
			TCHAR *strFirstOne = LPTSTR(LPCTSTR(strText));//Çüº¯È¯ //ÀÔ·Â¹®ÀÚ¿­
			
			if( !(44032 <= *strFirstOne && *strFirstOne <= 55203) /*ÇÑ±Û¹üÀ§ °¡ ~ ÆR*/ ) 
			{
				Search_before(strFirstOne);
			}
		}

		strShowData = SearchBegin(strText);
	}

	return strShowData;
}


VOID CECO_ControlAutoList::Search_before(TCHAR *strFirstOne)
{
	INT Chosung[19] = {0, 1, 3, 6, 7, 8, 16, 17, 18, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29};
	
	CString strLine = _T("");
	CString Line = _T("");	

	long Range[6];
	for(INT nTemp=0; nTemp<6; nTemp++)
	{
		Range[nTemp] = 0;
	}
	
	
	BOOL Check = FALSE;//¹üÀ§ ³¡À» ¾Ë¸®´Â 	
	INT count = 0;
	for (INT i=0 ; i<m_strarrayKeyWordList.GetSize() ; i++ )
	{		
		count++;
		strLine = m_strarrayKeyWordList.GetAt(i);
		
		strLine.TrimLeft();
		strLine.TrimRight();
		strLine.Replace(_T(" ") , _T(""));
		
		if( strLine == L"" )
			continue;		
		
		strLine = strLine.Left(1);
		
		TCHAR *sLine = (LPTSTR)(LPCTSTR)strLine; //Çüº¯È¯ //ÅØ½ºÆ®¹®ÀÚ¿­
		
		if( 12593<=*strFirstOne && *strFirstOne <=12622 /*"¤¾"°ª 0x3140*/ ) //ÃÊ¼ºÀÏ °æ¿ì
		{				
			INT nJongSung = 0;
			INT nstrLineChosung = *strFirstOne - 0x3131/*"¤¡"°ª 0x3131*/;
			while( nstrLineChosung < 30 )
			{
				if( Chosung[nJongSung] == nstrLineChosung ) //¹è¿­°ª°ú ºñ±³ÈÄ ¼øÂ÷ÀûÀÎ p°ªÀ¸·Î °è»êÇÏµµ·Ï ÇÔ
				{
					nstrLineChosung = nJongSung;
					break;
				}
				else
				{
					nJongSung++;
				}
			}				
			Range[0] = 588*nstrLineChosung;//¤¡ÀÌ¸é °¡ °ªÀÌ ³ª¿Â´Ù´Â °ÅÀÓ
			Range[1] = 587+(588*nstrLineChosung);
			
			if( Range[0] <= *sLine-0XAC00 && *sLine-0XAC00 <= Range[1] )
			{
				/*
				if( !Check )
				{
					Range[4] = count;
					Check = TRUE;
					continue;
				}	
				*/
				m_strArraySearchKeyWordList.Add(m_strarrayKeyWordList.GetAt(i));
			}
			else
			{
				if( Check )
				{
					Range[5] = count;
					Check = FALSE;
					break;
				}					
			}				
		}
		else//±âÅ¸ Æ¯¼ö¹®ÀÚ³ª ¼ýÀÚÀÏ °æ¿ì
		{
			if( *strFirstOne == *sLine )
			{
				if( !Check )
				{
					Range[4] = count;
					Check = TRUE;
					continue;
				}
			}
			else
			{
				if( Check )
				{
					Range[5] = count;
					Check = FALSE;
					break;
				}
			}				
		}
	}
}


CString CECO_ControlAutoList::SearchBegin(CString strText)
{
	strText.TrimLeft();
	strText.TrimLeft();
	strText.Replace(_T(" ") , _T(""));
	m_strArraySearchKeyWordList.RemoveAll();

	CString a = _T("");
	CString b = _T("");

	CString strReturnData = _T("");	
	
	CString strLine = _T("");
	CString Line = _T("");	
	
	INT nlen=0; // ¹®ÀÚ¿­ÀÇ ±æÀÌ(ÀÔ·ÂµÈ)
	
	BOOL bCheck = FALSE; // ÀÌÁÙÀÌ ¼±ÅÃ¿¡ ´ëÇÑ bool º¯¼ö

	CFile file;
			
	INT nPrintCnt=0; //Ç¥½ÃÇÒ ¿­ÀÇ ¼ýÀÚ 
	
	strText.TrimLeft();
	strText.TrimLeft();
	strText.Replace(_T(" ") , _T(""));
	
	nlen = strText.GetLength();//¹®ÀÚÀÇ °¹¼ö¸¦ ¼¼´Â..

	INT Infor[3][2];

	// KOL.UDF.022
	//long Position[2];/////	
	long Position[3];

	for(INT nTemp=0; nTemp<3; nTemp++)/////
	{
		Position[nTemp] = 0;
	}

	// KOL.UDF.022
	for(INT q=0; q<3; q++)
	//for(INT q=0; q<2; q++)
	{
		for(INT w=0; w<2; w++)
		{
			Infor[q][w] = 0;
		}
	}

	INT cnt = 0;
	CString strTok;
	while(AfxExtractSubString(strTok, m_strSearchRange, cnt, '|'))//¹®ÀÚ¸¦ Àß¶óÁØ´Ù.
	{
		Position[cnt] = _ttoi(strTok);
		cnt++;
	}
		
	if( nlen > 1)
	{
		a = strText.Left(nlen-1);
		strText = strText.Right(1);
	}

	TCHAR *sText = LPTSTR(LPCTSTR(strText));//Çüº¯È¯ //ÀÔ·Â¹®ÀÚ¿­
	INT nIsChangedJongSung = FindBlock( sText, Infor, 0 );
	nIsChangedJongSung = FindBlock( sText, Infor, 3 );	
	
	INT nLineCount = 0;	
	for (INT i=0 ; i<m_strarrayKeyWordList.GetSize() ; i++ )
	{
		nLineCount++;
		strLine = m_strarrayKeyWordList.GetAt(i);		
		bCheck = FALSE;
		
		strLine.TrimLeft();
		strLine.TrimLeft();
		strLine.Replace(_T(" ") , _T(""));
		
		INT count = strLine.GetLength();//¹®ÀÚÀÇ °¹¼ö¸¦ ¼¼´Â..
		
		if( nlen > count)
		{				
			continue;
		}
		
		if( nlen > 1)
		{
			b = strLine.Left(nlen-1);
			
			if( a != b )
			{
				continue;
			}
		}
		
		strLine = strLine.Mid( nlen-1 , 2 );
		
		TCHAR *sLine = (LPTSTR)(LPCTSTR)strLine; //Çüº¯È¯ //ÅØ½ºÆ®¹®ÀÚ¿­
		
		bCheck = SelectLine(sLine , nIsChangedJongSung , Infor);
		
		if (bCheck == FALSE) 
		{
			continue;
		}
		else
		{				
			nPrintCnt++;
			
			m_strArraySearchKeyWordList.Add(m_strarrayKeyWordList.GetAt(i));
		}
		
		if( nPrintCnt >= 20 )
		{				
			break;
		}				
	}

	return strReturnData;
}