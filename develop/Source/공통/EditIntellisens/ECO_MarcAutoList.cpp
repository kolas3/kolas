// ECO_ControlAutoList.cpp : implementation file
//
// @Copyright(c) ECO / Design By PDJc++ / Since 2003.7.15
//
//
#include "stdlib.h"
#include "stdafx.h"
#include "ECO_MarcAutoList.h"
#include "MarcEditCtrl.h"
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
// CECO_MarcAutoList dialog


CECO_MarcAutoList::CECO_MarcAutoList(CWnd* pParent /*=NULL*/)
	: CDialog(CECO_MarcAutoList::IDD, pParent)
{
EFS_BEGIN
	m_strarrayTagList.RemoveAll();
	m_strArraySearchKeyWordList.RemoveAll();
	m_strarrayDeleteString.RemoveAll();
	m_bResourceFileSearchFlag = TRUE;
	m_pParentEdit = NULL;
	m_nResourceID = 0;
	m_pParentWnd = (CMarcEditCtrl*) pParent;
	m_bSelectAllDisplay = FALSE;
	m_bAutoListUseFlag = FALSE;

	m_TagInfo		= NULL;	
	m_1st_Info		= NULL;
	m_2st_Info		= NULL;

	// KOL.UDF.022 시큐어코딩 보완
	m_nResourceID = 0;
	m_nTagCount = 0;
	m_n1stCount = 0;
	m_n2stCount = 0;
	m_nSearchType = -1;
	m_nItemIndex = -1;
EFS_END
}

CECO_MarcAutoList::~CECO_MarcAutoList()
{
EFS_BEGIN
	// Tag Free
	// 
	
	// Indicator Free
	//

EFS_END
}


void CECO_MarcAutoList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CECO_MarcAutoList)
	DDX_Control(pDX, IDC_MARC_INTELLISENS_LIST, m_ctlListCtrl);
	DDX_Control(pDX, IDC_LIST_BOX, m_ctlList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CECO_MarcAutoList, CDialog)
	//{{AFX_MSG_MAP(CECO_MarcAutoList)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_MARC_INTELLISENS_LIST, OnSelchangeListBox)
	ON_WM_CHAR()
	ON_WM_TIMER()
	ON_LBN_DBLCLK(IDC_MARC_INTELLISENS_LIST, OnDblclkListBox)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_MARC_INTELLISENS_LIST, OnCustomdrawList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECO_MarcAutoList message handlers

BOOL CECO_MarcAutoList::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CECO_MarcAutoList::OnInitDialog() 
{
EFS_BEGIN
	CDialog::OnInitDialog();	

	if ( m_strarrayTagList.GetSize() > 0 ) {return FALSE;}
	m_bResourceFileSearchFlag = ReferenceLoad();
	if ( m_bResourceFileSearchFlag == FALSE ) {return FALSE;}	

	m_ctlListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT ^ LVS_EX_MULTIWORKAREAS);
	m_ctlListCtrl.ModifyStyle(0,LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL );	
	
	SetTimer(1, 100, NULL);
	
EFS_END
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CECO_MarcAutoList::SearchKeyWord(INT nType)
{
EFS_BEGIN
	CString strTag;
	CString strIndi;
	CString strSubfield;
	INT		i, y;
	INT		nSearchCount;
	INT		nSearchIndex;	
	INT		nTagIndex;
	INT		nItemCount;
	BOOL	bFind;
	
	//////////////////////////////////////////////////
	// List Control Init.
	/*
	nListCount = m_ctlList.GetCount(); 	
	for ( i=0 ; i<nListCount ; i++ )
	{
		m_ctlList.DeleteString(0);		
	}	
	*/	
	m_ctlListCtrl.DeleteAllItems();
	
	///////////////////////////////////////////////////
	// Display Keywords Clean
	m_strArraySearchKeyWordList.RemoveAll();
		
	///////////////////////////////////////////////////
	// Make Keyword
	CString	str1, str2;
	CString strTemp;
	BOOL	nDeleteFind;
	nTagIndex = 0;
	m_nSearchType = nType; 
	m_ctlListCtrl.DeleteColumn(0);
	m_ctlListCtrl.DeleteColumn(0);
	m_ctlListCtrl.DeleteColumn(0);
	m_ctlListCtrl.DeleteColumn(0);
	nItemCount = 0;
	if ( nType == _TAG_ )
	{	
		m_ctlListCtrl.InsertColumn(0, _T("TAG"), LVCFMT_CENTER, 30 );		
		m_ctlListCtrl.InsertColumn(1, _T("설명"), LVCFMT_LEFT, 250 );		
		m_ctlListCtrl.InsertColumn(2, _T("반복여부"), LVCFMT_LEFT, 70 );		
		m_ctlListCtrl.InsertColumn(3, _T("필수여부"), LVCFMT_LEFT, 90 );		
		//m_ctlListCtrl.InsertItem();		
		if ( !m_strKeyWord.IsEmpty() )
		{
			for ( i=0 ; i<m_nTagCount ; i++ )
			{
				str1.Format(_T("%s"), m_TagInfo[i].szValue );
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
				
				nSearchIndex = str1.Find(m_strKeyWord);
				if ( nSearchIndex == 0 )
				{
					str1.Format(_T("%s | %s"),m_TagInfo[i].szValue, m_TagInfo[i].szDesc ); 
					m_strArraySearchKeyWordList.Add(str1);					
					m_ctlList.AddString(str1);
					// TAG No
					m_ctlListCtrl.InsertItem(nItemCount	,	m_TagInfo[i].szValue);
					// Tag Desc
					m_ctlListCtrl.SetItemText(nItemCount, 1, m_TagInfo[i].szDesc);

					// Tag Repeat					
					if ( m_TagInfo[i].bRepeat == TRUE ) strTemp = _T("반복");
					else strTemp = _T("반복불가");
					m_ctlListCtrl.SetItemText(nItemCount, 2, strTemp);

					// Tag Level
					if ( m_TagInfo[i].nLevel == 1 ) strTemp = _T("필수");
					else if ( m_TagInfo[i].nLevel == 2 ) strTemp = _T("해당시 필수"); 
					else strTemp = _T("재량");
					m_ctlListCtrl.SetItemText(nItemCount, 3, strTemp);
					nItemCount ++;
				}

			}
		}
		else
		{
			nSearchCount = 0;
		}

	}
	else if ( nType == _1ST_ )
	{
		m_ctlListCtrl.InsertColumn(0, _T("제1지시기호"), LVCFMT_CENTER, 20 );				
		m_ctlListCtrl.InsertColumn(1, _T("설명"), LVCFMT_LEFT, 100 );				

		if ( !m_strKeyWord.IsEmpty() )
		{
			strTag = m_strKeyWord.Left(3);
			strIndi = m_strKeyWord.Mid(4,1);
			
			if ( strIndi == _T(" ") )
			{
				m_strKeyWord = _T("b");
			}
			else
			{
				m_strKeyWord = strIndi;
			}
			
			nTagIndex = -1;
			// Tag Struct Find
			bFind = FALSE;
			for ( i=0 ; i<m_n1stCount ; i++ )
			{
				str1.Format(_T("%s"), m_1st_Info[i].szTag );
				if ( str1 == strTag )
				{
					bFind = TRUE;
					nTagIndex = i;
					break;
				}			
			}
			if ( bFind == TRUE )
			{
				for ( i=0 ; i<m_1st_Info[nTagIndex].nCount ; i++ )
				{
					
					str1.Format(_T("%s"), m_1st_Info[nTagIndex].pIndicator[i].szValue );
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
					
					nSearchIndex = str1.Find(m_strKeyWord);
					if ( nSearchIndex == 0 || m_strKeyWord == _T("?") )
					{
						str1.Format(_T("%s | %s"),m_1st_Info[nTagIndex].pIndicator[i].szValue, m_1st_Info[nTagIndex].pIndicator[i].szDesc ); 
						m_strArraySearchKeyWordList.Add(str1);					
						m_ctlList.AddString(str1);
						m_ctlListCtrl.InsertItem(nItemCount, m_1st_Info[nTagIndex].pIndicator[i].szValue );
						m_ctlListCtrl.SetItemText(nItemCount, 1, m_1st_Info[nTagIndex].pIndicator[i].szDesc );
						nItemCount ++;
					}					
				}
			}
			else
			{
				nSearchCount = 0;
			}
		}
		else
		{
			nSearchCount = 0;
		}
		
	}
	else if ( nType == _2ST_ )
	{
		m_ctlListCtrl.InsertColumn(0, _T("제2지시기호"), LVCFMT_CENTER, 20 );				
		m_ctlListCtrl.InsertColumn(1, _T("설명"), LVCFMT_LEFT, 80 );
		if ( !m_strKeyWord.IsEmpty() )
		{
			strTag = m_strKeyWord.Left(3);
			strIndi = m_strKeyWord.Mid(4,1);
			if ( strIndi == _T(" ") )
			{
				m_strKeyWord = _T("b");
			}
			else
			{
				m_strKeyWord = strIndi;
			}
			
			nTagIndex = -1;
			// Tag Struct Find
			bFind = FALSE;
			for ( i=0 ; i<m_n2stCount ; i++ )
			{
				str1.Format(_T("%s"), m_2st_Info[i].szTag );
				if ( str1 == strTag )
				{
					bFind = TRUE;
					nTagIndex = i;
					break;
				}			
			}
			if ( bFind == TRUE )
			{
				for ( i=0 ; i<m_2st_Info[nTagIndex].nCount ; i++ )
				{
					
					str1.Format(_T("%s"), m_2st_Info[nTagIndex].pIndicator[i].szValue );
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
					
					nSearchIndex = str1.Find(m_strKeyWord);
					if ( nSearchIndex == 0 || m_strKeyWord == _T("?"))
					{
						str1.Format(_T("%s | %s"),m_2st_Info[nTagIndex].pIndicator[i].szValue, m_2st_Info[nTagIndex].pIndicator[i].szDesc ); 
						m_strArraySearchKeyWordList.Add(str1);					
						m_ctlList.AddString(str1);
						m_ctlListCtrl.InsertItem(nItemCount, m_2st_Info[nTagIndex].pIndicator[i].szValue );
						m_ctlListCtrl.SetItemText(nItemCount, 1, m_2st_Info[nTagIndex].pIndicator[i].szDesc );
						nItemCount ++;
					}					
				}
			}
			else
			{
				nSearchCount = 0;
			}
		}
		else
		{
			nSearchCount = 0;
		}		
	}
	else if ( nType == _SUBFIELD_ )
	{
		m_ctlListCtrl.InsertColumn(0, _T("식별기호"), LVCFMT_CENTER	, 30	);		
		m_ctlListCtrl.InsertColumn(1, _T("설명")	, LVCFMT_LEFT	, 250	);		
		m_ctlListCtrl.InsertColumn(2, _T("반복여부"), LVCFMT_LEFT	, 70	);	
		if ( !m_strKeyWord.IsEmpty() )
		{
			strTag = m_strKeyWord.Left(3);
			strSubfield = m_strKeyWord.Mid(4,2);
			m_strKeyWord = strSubfield;
			
			nTagIndex = -1;
			// Tag Struct Find
			bFind = FALSE;
			for ( i=0 ; i<m_nTagCount ; i++ )
			{
				str1.Format(_T("%s"), m_TagInfo[i].szValue );
				if ( str1 == strTag )
				{
					bFind = TRUE;
					nTagIndex = i;
					break;
				}			
			}
			if ( bFind == TRUE )
			{
				for ( i=0 ; i<m_TagInfo[nTagIndex].nSubfieldCount ; i++ )
				{
					
					str1.Format(_T("%s"), m_TagInfo[nTagIndex].pSubfieldCodes[i].szValue );
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
					
					nSearchIndex = str1.Find(m_strKeyWord);
					if ( nSearchIndex == 0 || m_strKeyWord==_T("?") )
					{
						str1.Format(_T("%s | %s"),m_TagInfo[nTagIndex].pSubfieldCodes[i].szValue, m_TagInfo[nTagIndex].pSubfieldCodes[i].szDesc ); 
						m_strArraySearchKeyWordList.Add(str1);					
						m_ctlList.AddString(str1);
						m_ctlListCtrl.InsertItem(nItemCount, m_TagInfo[nTagIndex].pSubfieldCodes[i].szValue );
						m_ctlListCtrl.SetItemText(nItemCount, 1, m_TagInfo[nTagIndex].pSubfieldCodes[i].szDesc );
						if ( m_TagInfo[nTagIndex].pSubfieldCodes[i].bRepeat == TRUE ) strTemp = _T("반복");
						else strTemp = _T("반복불가");
						m_ctlListCtrl.SetItemText(nItemCount, 2, strTemp);
						nItemCount ++;
					}					
				}
			}
			else
			{
				nSearchCount = 0;
			}
		}
		else
		{
			nSearchCount = 0;
		}
	}
	// subfield
	/*
	if ( !m_strKeyWord.IsEmpty() )
		{
			strTag = m_strKeyWord.Left(3);
			strIndi = m_strKeyWord.Mid(4,1);
			m_strKeyWord = strIndi;
			
			nTagIndex = -1;
			// Tag Struct Find
			bFind = FALSE;
			for ( i=0 ; i<m_nTagCount ; i++ )
			{
				str1.Format(_T("%s"), m_TagInfo[i].szValue );
				if ( str1 == strTag )
				{
					bFind = TRUE;
					nTagIndex = i;
					break;
				}			
			}
			if ( bFind == TRUE )
			{
				for ( i=0 ; i<m_TagInfo[nTagIndex].nSubfieldCount ; i++ )
				{
					
					str1.Format(_T("%s"), m_TagInfo[i].szValue );
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
					
					nSearchIndex = str1.Find(m_strKeyWord);
					if ( nSearchIndex == 0 )
					{
						str1.Format(_T("%s | %s"),m_TagInfo[i].szValue, m_TagInfo[i].szDesc ); 
						m_strArraySearchKeyWordList.Add(str1);					
						m_ctlList.AddString(str1);
					}					
				}
			}
			else
			{
				nSearchCount = 0;
			}
		}
		else
		{
			nSearchCount = 0;
		}
	*/


	/*
	nListCount = m_strarrayTagList.GetSize();	
	CString	str1, str2;
	BOOL	nDeleteFind;
	if ( !m_strKeyWord.IsEmpty() )
	{
		for ( i=0 ; i<nListCount ; i++ )
		{
			str1 = m_strarrayTagList.GetAt(i);
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
			
			nSearchIndex = m_strarrayTagList.GetAt(i).Find(m_strKeyWord);
			if ( nSearchIndex == 0 )
			{
				m_strArraySearchKeyWordList.Add(m_strarrayTagList.GetAt(i));					
				m_ctlList.AddString(m_strarrayTagList.GetAt(i));
			}
		}
	}
	else
	{
		nSearchCount = 0;
	}
	*/


	/////////////////////////////////////////////////
	nSearchCount = m_strArraySearchKeyWordList.GetSize();
	if ( nSearchCount <= 0 ) 
	{
		m_pParentWnd->m_bAutoListFocus = FALSE;
		ShowWindow(SW_HIDE);
		{return FALSE;}
	} 
	else if ( nSearchCount == 1 )
	{
		/*
		if ( m_strarrayTagList.GetAt(0) == m_strKeyWord )
		{
			m_pParentWnd->m_bAutoListFocus = FALSE;
			ShowWindow(SW_HIDE);
			{return FALSE;			}
		}	
		else 
		*/
		{
			//ShowWindow(SW_SHOW);
			m_pParentEdit->ShowAutoList(TRUE);
			m_ctlList.SetSel(0, -1);
			m_pParentWnd->SetFocus();
		}
	}
	else
	{
		//ShowWindow(SW_SHOW);
		m_pParentEdit->ShowAutoList(TRUE);
		m_ctlList.SetSel(0, -1);
		m_pParentWnd->SetFocus();
	}

EFS_END
	return TRUE;
}

void CECO_MarcAutoList::SetKeyWord(_TCHAR *f_pKeyWordIn)
{
EFS_BEGIN
	m_strKeyWord.Format(_T("%s"), f_pKeyWordIn);
EFS_END
}

void CECO_MarcAutoList::Display()
{	
EFS_BEGIN
			
/*
	if ( m_strarrayTagList.GetSize() > 0 ) {return;}
	m_bResourceFileSearchFlag = ReferenceLoad();
	if ( m_bResourceFileSearchFlag == FALSE ) {return;}
*/
EFS_END
}

BOOL CECO_MarcAutoList::ReferenceLoad()
{
EFS_BEGIN
	FILE	*fp;
	_TCHAR	szFileName[64];
	_TCHAR	szTemp[1024];
	INT		line_length;
	
	m_bAutoListUseFlag = TRUE;
	// Tag Resource File Check
	/*
	_stprintf(szFileName, _T("..\\Cfg\\MarcIntellisens\\Tag.txt") );	
	fp = FdkFopen( szFileName, _T("r") );
	if ( fp == NULL ) {return FALSE;}

	line_length = 0;
	do {
		if (CECO_FileIO::next_line(fp, szTemp, &line_length) == -1) { break; }
		if ( szTemp[0] == _T('.') ) continue;
		if ( szTemp[0] == _T('\n') ) continue;
		CECO_FileIO::str_rtrim(szTemp, szTemp, '\n');
		m_strarrayTagList.Add( (LPCTSTR)&szTemp );
	} while(1);

	if ( m_strarrayTagList.GetSize() <= 0 ) 
	{
		FdkFclose( fp );
		{return FALSE;}
	}
	FdkFclose( fp );	
	*/
	

	// Common
	TCHAR	*pToken;
	CString strTemp;
	INT nOffset;	
	m_nTagCount = 0;
	
	// Tag Load	
	_stprintf(szFileName, _T("..\\Cfg\\MarcIntellisens\\tag.txt") );
	fp = FdkFopen( szFileName, _T("r") );
	if ( fp == NULL ) {return FALSE;}
	
	line_length = 0;		
	do 
	{
		if (CECO_FileIO::next_line(fp, szTemp, &line_length) == -1) { break; }
		if ( szTemp[0] == _T('.') ) continue;
		if ( szTemp[0] == _T('\n') ) continue;
		CECO_FileIO::str_rtrim(szTemp, szTemp, '\n');		
		strTemp.Format(_T("%s"), szTemp);

		if ( szTemp[0] != _T('$') )
		{
			// Tag 정보 읽기 시작
			m_nTagCount++;
			if ( m_nTagCount == 1 )
			{
				m_TagInfo = (tTAG_INTELLISENS_STRU*) malloc (sizeof(tTAG_INTELLISENS_STRU)*(m_nTagCount));
			}
			else
			{
				m_TagInfo = (tTAG_INTELLISENS_STRU*) realloc (m_TagInfo, sizeof(tTAG_INTELLISENS_STRU)*(m_nTagCount));
			}	
			
			// Subfield 변수 초기화
			m_TagInfo[m_nTagCount-1].nSubfieldCount = 0;
			m_TagInfo[m_nTagCount-1].pSubfieldCodes = NULL;
			
			// Sample Data
			//szTemp	0x0013f298 "056 분류기호¶¶
			_tcsncpy(m_TagInfo[m_nTagCount-1].szValue, szTemp, 3);
			m_TagInfo[m_nTagCount-1].szValue[3] = _T('\0');
			nOffset = strTemp.Find(_T("¶"));
			_tcsncpy(m_TagInfo[m_nTagCount-1].szDesc, szTemp+4, nOffset-4);		
			m_TagInfo[m_nTagCount-1].szDesc[nOffset-4] = _T('\0');		
			if ( szTemp[nOffset+1] == _T('O') )
			{
				m_TagInfo[m_nTagCount-1].bRepeat = TRUE;
			}
			else
			{
				m_TagInfo[m_nTagCount-1].bRepeat = FALSE;
			}

			if ( szTemp[nOffset+3] == _T('1') )
			{
				m_TagInfo[m_nTagCount-1].nLevel = 1;
			}
			else if ( szTemp[nOffset+3] == _T('2') )
			{
				m_TagInfo[m_nTagCount-1].nLevel = 2;
			}
			else
			{
				m_TagInfo[m_nTagCount-1].nLevel = 3;
			}
		}
		else
		{
			// Sample Data
			// $a:본표제¶O
			// $b:표제 관련 정보¶O

			// 식별기호 읽기
			m_TagInfo[m_nTagCount-1].nSubfieldCount ++;
			if ( m_TagInfo[m_nTagCount-1].nSubfieldCount == 1 )
			{
				m_TagInfo[m_nTagCount-1].pSubfieldCodes = (tSUBFIELD_CODE_ITEM_STRU*) malloc (sizeof(tSUBFIELD_CODE_ITEM_STRU)*1);
			}
			else
			{
				m_TagInfo[m_nTagCount-1].pSubfieldCodes = (tSUBFIELD_CODE_ITEM_STRU*) realloc (m_TagInfo[m_nTagCount-1].pSubfieldCodes, sizeof(tSUBFIELD_CODE_ITEM_STRU)*(m_TagInfo[m_nTagCount-1].nSubfieldCount));
			}

			// 서브필드 코드
			_tcsncpy(m_TagInfo[m_nTagCount-1].pSubfieldCodes[m_TagInfo[m_nTagCount-1].nSubfieldCount-1].szValue, szTemp, 2);
			m_TagInfo[m_nTagCount-1].pSubfieldCodes[m_TagInfo[m_nTagCount-1].nSubfieldCount-1].szValue[0] = _T('▼');
			m_TagInfo[m_nTagCount-1].pSubfieldCodes[m_TagInfo[m_nTagCount-1].nSubfieldCount-1].szValue[2] = _T('\0');

			// 서브필드 설명
			nOffset = strTemp.Find(_T("¶"));
			_tcsncpy(m_TagInfo[m_nTagCount-1].pSubfieldCodes[m_TagInfo[m_nTagCount-1].nSubfieldCount-1].szDesc, szTemp+3, nOffset-3);
			m_TagInfo[m_nTagCount-1].pSubfieldCodes[m_TagInfo[m_nTagCount-1].nSubfieldCount-1].szDesc[nOffset-3] = _T('\0');
			
			// 반복여부
			if ( szTemp[nOffset+1] == _T('O') )
			{
				m_TagInfo[m_nTagCount-1].pSubfieldCodes[m_TagInfo[m_nTagCount-1].nSubfieldCount-1].bRepeat = TRUE;
			}
			else
			{
				m_TagInfo[m_nTagCount-1].pSubfieldCodes[m_TagInfo[m_nTagCount-1].nSubfieldCount-1].bRepeat = FALSE;
			}
		}
	} while(1);
	FdkFclose( fp );
	
	// Indicator Load
	INT nIndyPos;
	m_n1stCount = 0;
	m_n2stCount = 0;
	_stprintf(szFileName, _T("..\\Cfg\\MarcIntellisens\\indicator.txt") );
	fp = FdkFopen( szFileName, _T("r") );
	if ( fp == NULL ) {return FALSE;}
	
	line_length = 0;		
	do 
	{		
		if (CECO_FileIO::next_line(fp, szTemp, &line_length) == -1) { break; }
		if ( szTemp[0] == _T('.') ) continue;
		if ( szTemp[0] == _T('\n') ) continue;
		CECO_FileIO::str_rtrim(szTemp, szTemp, '\n');
		
		strTemp.Format(_T("%s"), szTemp);
		nIndyPos = 0;
		// 지시기호 구분		
		if ( strTemp.Mid(3,1) == _T("1") ) nIndyPos = 1;
		else nIndyPos = 2;
						
		if ( nIndyPos == 1 )
		{
			// 제1지시기호 로드 
			m_n1stCount++;
			if ( m_n1stCount == 1 )
			{
				m_1st_Info = (tINDICATOR_INTELLISENS_STRU*) malloc (sizeof(tINDICATOR_INTELLISENS_STRU)*(m_n1stCount));
			}
			else
			{
				m_1st_Info = (tINDICATOR_INTELLISENS_STRU*) realloc (m_1st_Info, sizeof(tINDICATOR_INTELLISENS_STRU)*(m_n1stCount));
			}	
			
		}
		else
		{
			// 제2지시기호 로드
			m_n2stCount++;
			if ( m_n2stCount == 1 )
			{
				m_2st_Info = (tINDICATOR_INTELLISENS_STRU*) malloc (sizeof(tINDICATOR_INTELLISENS_STRU)*(m_n2stCount));
			}
			else
			{
				m_2st_Info = (tINDICATOR_INTELLISENS_STRU*) realloc (m_2st_Info, sizeof(tINDICATOR_INTELLISENS_STRU)*(m_n2stCount));
			}
		}		

		// Sample Data
		/*
		2451 표제의 부출¶0:표제를 부출하지 않음¶1:표제를 부출함¶2:관제를 포함해서 표제를 부출함
		2452 관제 및 관사 출력 형태¶0:그대로 인쇄¶1:원괄호를 제외하고 인쇄
		2601 _:적용불가/제공되는 정보 없음/최초 발행처¶ 2:중간발행처(Intervening publisher)¶ 3:현행/최근 발행처(current/latest publisher)
		2602 b:
		*/		
		if ( nIndyPos == 1 )
		{
			// 1st Indicator Tag			
			_tcsncpy(m_1st_Info[m_n1stCount-1].szTag, szTemp, 3);
			m_1st_Info[m_n1stCount-1].szTag[3] = _T('\0');			
			
			pToken = _tcstok(szTemp, _T("¶")); 
			_tcscpy(m_1st_Info[m_n1stCount-1].szIndiDesc, pToken+5);

			m_1st_Info[m_n1stCount-1].nCount = 0;
			while ( 1 ) 
			{
				pToken = _tcstok(NULL, _T("¶"));
				if ( pToken == NULL ) break;
				m_1st_Info[m_n1stCount-1].nCount ++;
				if ( m_1st_Info[m_n1stCount-1].nCount == 1 )
				{
					m_1st_Info[m_n1stCount-1].pIndicator = (tINDICATOR_ITEM_STRU*) malloc (sizeof(tINDICATOR_ITEM_STRU) * 1 );
				}
				else
				{
					m_1st_Info[m_n1stCount-1].pIndicator = (tINDICATOR_ITEM_STRU*) realloc (m_1st_Info[m_n1stCount-1].pIndicator, sizeof(tINDICATOR_ITEM_STRU) * m_1st_Info[m_n1stCount-1].nCount );
				}

				_tcsncpy(m_1st_Info[m_n1stCount-1].pIndicator[m_1st_Info[m_n1stCount-1].nCount-1].szValue, pToken, 1 );
				m_1st_Info[m_n1stCount-1].pIndicator[m_1st_Info[m_n1stCount-1].nCount-1].szValue[1] = _T('\0');
				_tcscpy(m_1st_Info[m_n1stCount-1].pIndicator[m_1st_Info[m_n1stCount-1].nCount-1].szDesc, pToken+2 ); 
			}			
		}
		else
		{
			// 2st Indicator Tag						
			_tcsncpy(m_2st_Info[m_n2stCount-1].szTag, szTemp, 3);
			m_2st_Info[m_n2stCount-1].szTag[3] = _T('\0');			
			
			pToken = _tcstok(szTemp, _T("¶")); 
			_tcscpy(m_2st_Info[m_n2stCount-1].szIndiDesc, pToken+5);

			m_2st_Info[m_n2stCount-1].nCount = 0;
			while ( 1 ) 
			{
				pToken = _tcstok(NULL, _T("¶"));
				if ( pToken == NULL ) break;
				m_2st_Info[m_n2stCount-1].nCount ++;
				if ( m_2st_Info[m_n2stCount-1].nCount == 1 )
				{
					m_2st_Info[m_n2stCount-1].pIndicator = (tINDICATOR_ITEM_STRU*) malloc (sizeof(tINDICATOR_ITEM_STRU) * 1 );
				}
				else
				{
					m_2st_Info[m_n2stCount-1].pIndicator = (tINDICATOR_ITEM_STRU*) realloc (m_2st_Info[m_n2stCount-1].pIndicator, sizeof(tINDICATOR_ITEM_STRU) * m_2st_Info[m_n2stCount-1].nCount );
				}

				_tcsncpy(m_2st_Info[m_n2stCount-1].pIndicator[m_2st_Info[m_n2stCount-1].nCount-1].szValue, pToken, 1 );
				m_2st_Info[m_n2stCount-1].pIndicator[m_2st_Info[m_n2stCount-1].nCount-1].szValue[1] = _T('\0');
				_tcscpy(m_2st_Info[m_n2stCount-1].pIndicator[m_2st_Info[m_n2stCount-1].nCount-1].szDesc, pToken+2 ); 
			}
		}		
		
	} while(1);
	FdkFclose( fp );	
EFS_END
	return TRUE;
}

void CECO_MarcAutoList::SetResource(UINT f_nResourceIDIn, CWnd *pWnd)
{
EFS_BEGIN
	m_nResourceID = f_nResourceIDIn;
	m_pParentEdit = (CMarcEditCtrl*) pWnd;
EFS_END
}

INT CECO_MarcAutoList::ReferenceWrite()
{
	/*
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
	nListCount = m_strarrayTagList.GetSize();
	for ( i=0 ; i<nListCount ; i++ )
	{
		if ( m_strarrayTagList.GetAt(i).Find(m_strKeyWord, 0) == 0 )
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
	nListCount = m_strarrayTagList.GetSize();
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
			str1 = m_strarrayTagList.GetAt(i);
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
			offset += _stprintf( szTemp+offset, _T("%s\n"), m_strarrayTagList.GetAt(i) );
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
*/
	return TRUE;	
}

void CECO_MarcAutoList::OnSize(UINT nType, int cx, int cy) 
{
EFS_BEGIN
	CDialog::OnSize(nType, cx, cy);	

	if ( m_ctlList.GetSafeHwnd() != NULL ) {
		CRect rect;	
		GetClientRect(&rect);				
		//m_ctlList.MoveWindow(rect);
		m_ctlListCtrl.MoveWindow(rect);
	}
EFS_END
}

void CECO_MarcAutoList::OnSelchangeListBox() 
{
EFS_BEGIN
	PrevNextSelection(4);	
EFS_END
}

void CECO_MarcAutoList::OnDblclkListBox() 
{	
EFS_BEGIN

	CString	strData;
	
	SelectSel();		
	m_pParentWnd->m_bAutoListFocus = FALSE;
	ShowWindow(SW_HIDE);
EFS_END
}

void CECO_MarcAutoList::SelectSel()
{
EFS_BEGIN
	CString	strData;
	CString strItem;
	INT		nSelectedSel;
	INT		nCurIdx;

	//nSelectedSel = m_ctlList.GetCurSel();	
	//m_ctlList.GetText(nSelectedSel, strData);	
	nSelectedSel = GetSelectedIndex();
	strData = m_ctlListCtrl.GetItemText(nSelectedSel, 0);
	INT nOffset = m_strKeyWord.GetLength();
	CHARRANGE cr;
	m_pParentWnd->GetSel(cr);
	m_pParentWnd->HideSelection(TRUE, FALSE);
	
	if (m_nSearchType == _TAG_ )
	{
		m_pParentWnd->SetSel(cr.cpMax-3, cr.cpMax);
		m_pParentWnd->ReplaceSel(strData.Left(3));
		m_strKeyWord = strData.Left(3);
	}
	else if ( m_nSearchType == _1ST_  || m_nSearchType == _2ST_ ) 
	{
		m_pParentWnd->SetSel(cr.cpMax-1, cr.cpMax);
		if ( strData.Left(1) == _T("b") )
		{
			m_pParentWnd->ReplaceSel(_T(" "));
			m_strKeyWord = _T(" ");
		}
		else
		{
			m_pParentWnd->ReplaceSel(strData.Left(1));
			m_strKeyWord = strData.Left(1);
		}		
	}	
	else if ( m_nSearchType == _SUBFIELD_ ) 
	{
		m_pParentWnd->SetSel(cr.cpMax-2, cr.cpMax);
		strItem.Format(_T("%c%s"), 31, strData.Mid(1,1));
		m_pParentWnd->ReplaceSel(strItem);
		m_strKeyWord = strData.Left(2);
	}
	
	//m_pParentWnd->SetWindowText(strData.Left(3));
	
	m_strArraySearchKeyWordList.RemoveAll();
	m_pParentWnd->HideSelection(FALSE, FALSE);

	nCurIdx = strData.GetLength();		
	m_pParentWnd->SetFocus();
	//m_pParentWnd->SetSel(nCurIdx, nCurIdx );
	
EFS_END
}

INT CECO_MarcAutoList::SelectAllList()
{
EFS_BEGIN
	INT		i, y;
	INT		nListCount;

	m_bSelectAllDisplay = TRUE;	
	
	/*
	nListCount = m_ctlList.GetCount();	
	for ( i=0 ; i<nListCount ; i++ )
	{
		m_ctlList.DeleteString(0);
	}
	*/	
	m_ctlListCtrl.DeleteAllItems();
	m_strArraySearchKeyWordList.RemoveAll();

	nListCount = m_strarrayTagList.GetSize();
	if ( nListCount <=  0 ) {return 0;}

	CString	str1, str2;
	BOOL	nDeleteFind;
	for ( i=0 ; i<nListCount ; i++ )
	{			
		str1 = m_strarrayTagList.GetAt(i);
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
		m_strArraySearchKeyWordList.Add(m_strarrayTagList.GetAt(i));					
		m_ctlList.AddString(m_strarrayTagList.GetAt(i));	
	}	
	
	if ( m_ctlList.GetCount() > 0 )	ShowWindow(SW_SHOW);

EFS_END
	return 0;

}

void CECO_MarcAutoList::OnTimer(UINT nIDEvent) 
{	
	if ( nIDEvent == 1 )
	{
		// Delete AutoList Check
		CWnd*	pWnd;
		pWnd = GetFocus();
		if ( pWnd == NULL ) {return;}

		CWnd	*p1;	
		p1 =  GetDlgItem(IDC_MARC_INTELLISENS_LIST);
		
		if ( pWnd != m_pParentWnd && pWnd != p1) 
		{
			KillTimer(1);
			m_pParentWnd->DeleteAutoList();			
			{return;}
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CECO_MarcAutoList::DeleteAutoListKeyWord()
{
EFS_BEGIN
	CString	strData;
	INT		nSelectedSel;
	INT		nCurIdx;
	INT		nCount;
	
	// Delete Auto ListBox Display
	nSelectedSel = m_ctlList.GetCurSel();
	m_ctlList.GetText(nSelectedSel, strData);
	m_strarrayDeleteString.Add(strData);
	m_ctlList.DeleteString(nSelectedSel);	

	if ( m_strArraySearchKeyWordList.GetSize() > 0 )
		m_strArraySearchKeyWordList.RemoveAt(nSelectedSel);	
	nCount = m_ctlList.GetCount();
	
	// Set Cursor
	if ( nCount == 0 ) 
	{
		m_pParentWnd->m_bAutoListFocus = FALSE;
		ShowWindow(SW_HIDE);
		{return;}	
	}
	if ( nCount-1 >= nSelectedSel ) m_ctlList.SetCurSel(nSelectedSel);
	else if ( (nCount-1 < nSelectedSel) && (nCount > 0)) m_ctlList.SetCurSel(nSelectedSel-1);		
	
	// Set Edit Box
	nSelectedSel = m_ctlList.GetCurSel();	
	m_ctlList.GetText(nSelectedSel, strData);
	INT nOffset = m_strKeyWord.GetLength();
	CHARRANGE cr;
	m_pParentWnd->GetSel(cr);
	m_pParentWnd->HideSelection(TRUE, FALSE);
	m_pParentWnd->SetSel(cr.cpMax-m_strKeyWord.GetLength(), cr.cpMax);
	m_pParentWnd->ReplaceSel(strData.Left(3));
	m_pParentWnd->HideSelection(FALSE, FALSE);
	//m_pParentWnd->SetWindowText(strData);
	m_strKeyWord = strData.Left(3);
	nCurIdx = strData.GetLength();		
	m_pParentWnd->SetSel(nCurIdx, nCurIdx+1 );			
		
EFS_END
	return;

}

void CECO_MarcAutoList::PrevNextSelection(INT nType)
{
EFS_BEGIN
	CString strItem;
	INT		nIndex, nCount;
	//nIndex = m_ctlList.GetCurSel();
	//nCount = m_ctlList.GetCount();
	nCount = m_ctlListCtrl.GetItemCount();
	nIndex = GetSelectedIndex();

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
					//m_ctlList.GetText(0, strText);
					strText = m_ctlListCtrl.GetItemText(0,0);
					INT nOffset = m_strKeyWord.GetLength();
					CHARRANGE cr;
					m_pParentWnd->GetSel(cr);
					//m_pParentWnd->SetSel(cr.cpMax-m_strKeyWord.GetLength(), cr.cpMax);

					if (m_nSearchType == _TAG_ )
					{
						m_pParentWnd->ReplaceSel(strText.Mid(nOffset, 3-nOffset));
					}
					else if ( m_nSearchType == _1ST_  || m_nSearchType == _2ST_ ) 
					{
						if ( m_strKeyWord == _T("?") )
						{
							if ( strText.Left(1) == _T("b") )
							{
								m_pParentWnd->ReplaceSel(_T(" "));								
							}
							else
							{
								m_pParentWnd->ReplaceSel(strText.Left(1));
							}							
						}
						else
						{							
							m_pParentWnd->SetSel(cr.cpMax-1, cr.cpMax);
							m_pParentWnd->ReplaceSel(strText.Left(1));
						}						
					}
					else if ( m_nSearchType == _SUBFIELD_ ) 
					{
						if ( m_strKeyWord == _T("?") )
						{
							strItem.Format(_T("%c%s"), 31, strText.Mid(1,1));
							m_pParentWnd->ReplaceSel(strItem);
						}
						else
						{
							strItem.Format(_T("%c%s"), 31, strText.Mid(1,1));
							m_pParentWnd->SetSel(cr.cpMax-2, cr.cpMax);
							m_pParentWnd->ReplaceSel(strItem);
						}						
					}					

					//CListCtrl
					//m_pParentWnd->SetWindowText(strText.Left(3-nOffset));					
					//m_ctlList.SetCurSel(0);					
					m_ctlListCtrl.SetItemState(0,LVIS_SELECTED,LVIS_SELECTED);					
					m_pParentWnd->SetFocus();
				}
			}
			break;
		case 2: // Down		
			if ( nCount-1 > nIndex )
			{
				//m_ctlList.SetCurSel(nIndex+1);
				m_ctlListCtrl.SetItemState(nIndex+1, LVIS_SELECTED,LVIS_SELECTED);
				SelectSel();	
			}						
			break;
		case 3: // Up
			if ( nIndex > 0 )
			{
				//m_ctlList.SetCurSel(nIndex-1);
				m_ctlListCtrl.SetItemState(nIndex-1, LVIS_SELECTED,LVIS_SELECTED);
				SelectSel();				
			}
			break;
		case 4: // Mouse Click
			SelectSel();
			break;
	}	
EFS_END
}
INT	CECO_MarcAutoList::MaxSearchKeywordSize()
{
	INT nItemCount;
	CString	strItem;
	INT nLength;
	INT	nMaxLength;
	CString strMaxString;

	nItemCount = m_ctlListCtrl.GetItemCount();


	nLength = 0;
	nMaxLength = 0;
	for ( int i=0 ; i<nItemCount ; i++ )
	{
		strItem = m_ctlListCtrl.GetItemText(i, 1);
		nLength = strItem.GetLength();
		
		if ( nMaxLength < nLength ) 
		{
			strMaxString = strItem;
			nMaxLength = nLength;		
		}
	}

	// Real Size 구하기	
	nMaxLength = WideCharToMultiByte(CP_ACP, 0, strMaxString, -1, NULL, 0, NULL,NULL);	
	m_ctlListCtrl.SetColumnWidth(1, nMaxLength*7 + 6);
	
	return nMaxLength;
}

INT	CECO_MarcAutoList::GetSelectedIndex()
{
	INT nIndex = -1;
	INT nCheck;
	INT nCount = m_ctlListCtrl.GetItemCount();
	for ( int i=0 ; i<nCount ; i++ )
	{		
		nCheck = m_ctlListCtrl.GetItemState(i, LVNI_SELECTED);
		if ( nCheck > 0 )
		{
			nIndex = i;
			break;
		}
	}

	return nIndex;
}

void CECO_MarcAutoList::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{	
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
    *pResult = 0;
	
    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
    {
        *pResult = CDRF_NOTIFYITEMDRAW;
    }
    else
	{
        if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
        {
			
			if( (pLVCD->nmcd.dwItemSpec % 2)== 0  )     // 바꿀 row 와 비교
			{
				if(0)
				{
					pLVCD->clrText   = RGB(0, 0, 0);          
					pLVCD->clrTextBk = RGB(200, 200, 200);
				}
				else
				{   
					pLVCD->clrText   = RGB(0, 0, 0);            
					pLVCD->clrTextBk = RGB(247,247,247);
				}
			}
			else       // 기본색...이다.
			{
				pLVCD->clrText   = RGB(0, 0, 0);          
				//pLVCD->clrTextBk = RGB(255, 255, 255);
			}		
			
			
			*pResult = CDRF_DODEFAULT;
        }
	}
	
}


