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
	m_nItemIndex = -1;
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
	DDX_Control(pDX, IDC_MARC_INTELLISENS_LIST, m_ctlListCtrl);
	DDX_Control(pDX, IDC_LIST_BOX, m_ctlListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CECO_ControlAutoList, CDialog)
	//{{AFX_MSG_MAP(CECO_ControlAutoList)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_LIST_BOX, OnSelchangeListBox)
	ON_WM_TIMER()
	ON_LBN_DBLCLK(IDC_LIST_BOX, OnDblclkListBox)
	ON_WM_CHAR()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MARC_INTELLISENS_LIST, OnItemchangedMarcIntellisensList)
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
	INT		i, y;
	INT		nListCount;
	INT		nSearchCount;
	INT		nSearchIndex;
	
	
	nListCount = m_ctlListBox.GetCount(); 
	for ( i=0 ; i<nListCount ; i++ )
	{
		m_ctlListBox.DeleteString(0);
	}	
	m_strArraySearchKeyWordList.RemoveAll();
	nListCount = m_strarrayKeyWordList.GetSize();
	
	CString	str1, str2;
	BOOL	nDeleteFind;
	if ( !m_strKeyWord.IsEmpty() )
	{
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
			
			nSearchIndex = m_strarrayKeyWordList.GetAt(i).Find(m_strKeyWord);
			if ( nSearchIndex == 0 )
			{
				m_strArraySearchKeyWordList.Add(m_strarrayKeyWordList.GetAt(i));					
				m_ctlListBox.AddString(m_strarrayKeyWordList.GetAt(i));
			}
		}
	}
	else
	{
		nSearchCount = 0;
	}

	nSearchCount = m_strArraySearchKeyWordList.GetSize();
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
			ShowWindow(SW_SHOW);
			m_ctlListBox.SetSel(0, -1);
			m_pParentWnd->SetFocus();
		}
	}
	else
	{
		ShowWindow(SW_SHOW);
		m_ctlListBox.SetSel(0, -1);
		m_pParentWnd->SetFocus();
	}

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
		m_strarrayKeyWordList.Add( (LPCTSTR)&szTemp );
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
	
	nListCount = m_ctlListBox.GetCount();	
	for ( i=0 ; i<nListCount ; i++ )
	{
		m_ctlListBox.DeleteString(0);
	}	
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
	
	if ( m_ctlListBox.GetCount() > 0 )	ShowWindow(SW_SHOW);

EFS_END
	return 0;

}

void CECO_ControlAutoList::OnTimer(UINT nIDEvent) 
{	
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
EFS_BEGIN
	INT		nIndex, nCount;

	nIndex = m_ctlListBox.GetCurSel();
	nCount = m_ctlListBox.GetCount();

	switch ( nType )
	{
		case 1:  // Show AutoList
			{				
				INT	nSearchKeywordCount;
				CString	strText;			
				m_pParentWnd->GetWindowText(strText);
				if ( strText.IsEmpty() ) SelectAllList();
				
				nSearchKeywordCount = m_strArraySearchKeyWordList.GetSize();
				if ( nSearchKeywordCount > 0 )
				{
					m_pParentWnd->m_bAutoListFocus = TRUE;				
					m_ctlListBox.GetText(0, strText);
					m_pParentWnd->SetWindowText(strText);			
					m_ctlListBox.SetCurSel(0);
					nIndex = strText.GetLength();
					m_pParentWnd->SetSel(nIndex, nIndex+1 );
					m_pParentWnd->SetFocus();

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
EFS_END
}


void CECO_ControlAutoList::OnItemchangedMarcIntellisensList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

		
	*pResult = 0;
}

/*
m_listctrl.SetItemState(nIndex,LVIS_SELECTED,LVIS_SELECTED);
이렇게 하면 nIndex에 해당하는 Row가 반전됩니다.
마치 마우스로 선택한 것 처럼요...

m_listctrl.SetItemState(nIndex,0,LVIS_SELECTED);
*/
