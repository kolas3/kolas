// Explorer.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "Explorer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExplorer dialog


CExplorer::CExplorer(CWnd* pParent /*=NULL*/)
	: CDialog(CExplorer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExplorer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	m_HtmlView = NULL;
}

CExplorer::~CExplorer()
{
	if ( m_HtmlView != NULL ) 
	{		
		//delete m_HtmlView;
	}
}


void CExplorer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExplorer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExplorer, CDialog)
	//{{AFX_MSG_MAP(CExplorer)
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 1012, OnDownloadBegin)
	ON_MESSAGE(WM_USER + 1013, OnDownloadComplete)
	ON_MESSAGE(WM_USER + 1100, OnTitleChange)	
	ON_MESSAGE(WM_USER + 1101, OnStatusTextChange)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExplorer message handlers

BOOL CExplorer::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	
	m_HtmlView = new CECO_HtmlViewer;	
	DWORD dwstyle = 0;
	dwstyle = WS_VISIBLE | WS_CHILD;
	m_HtmlView->Create(NULL, NULL, dwstyle ,
		CRect(0,0,300,300),this,		
		IDD_HTMLVIEW, NULL   );
	
	m_HtmlView->Navigate2(m_strURL);
	m_HtmlView->SetParent(this);

	m_pOldISBNList.RemoveAll();

	m_strTitle = _T("");	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CExplorer::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{	
	m_pParent = pParentWnd;
	
	return CDialog::Create(IDD, pParentWnd);
}

void CExplorer::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if (m_HtmlView->GetSafeHwnd() )
	{
		m_HtmlView->MoveWindow(0,0,cx,cy);
	}
}
VOID CExplorer::GetISBNList(CStringArray &pISBNList, CString strText)
{
	CString strBody;	
	CString strISBN;
	CString strCheck;
	CString strIsbnTemp;
	INT nIndex;
	INT	nOffset;
	INT	i;
	
	if ( strText.IsEmpty() )
	{
		strBody = m_HtmlView->GetHTMLSource();	
	}
	else
	{
		strBody = strText;
	}
	nOffset = 0;
	pISBNList.RemoveAll();
	strIsbnTemp = _T("");
	while (TRUE)
	{
		// ISBN 낱말을 찾는다.
		nIndex = strBody.Find(_T("ISBN"), nOffset);		
		if ( nIndex == -1 )
		{
			break;			
		}
		else
		{
			nOffset = nIndex;
		}

		// ISBN문자열 뒤에 오는 ISBN을 체크한다.
		nIndex = strBody.Find(_T("89"), nOffset);		
		if ( nIndex == -1) 
		{
			break;
			//nIndex = strText.Find(_T("89"), nOffset);		
			//if ( nIndex == -1) break;
		}
		
		strISBN = _T("");
		if ( nIndex > 0 )
		{
			for ( i=nIndex ; i<nIndex+20 ; i++)
			{
				strCheck = strBody.Mid(i, 1);
				if ( strCheck == _T("-") || ( strCheck >= _T("0") && strCheck <= _T("9") ) )
				{
					strISBN += strCheck;
				}
				else
				{
					break;
				}
			}
		}
		//=====================================================
		//2009.02.25 UPDATE BY PJW : str과 int를 비교한 오류 수정, continue이더라도 +1해준다.
//  		if ( strISBN < 10 ) continue;
 		if ( strISBN.GetLength() < 10 ) 
 		{
  			nOffset = i+1;
 			continue;
 		}
		//=====================================================	

		if ( strIsbnTemp.Find(strISBN) < 0 )
		{
			pISBNList.Add(strISBN);
			strIsbnTemp += strISBN + _T(" ");
		}
		
		nOffset = i+1;
	}

	nOffset = 0;	
	strIsbnTemp = _T("");
	while (TRUE)
	{
		// ISBN 낱말을 찾는다.
		nIndex = strBody.Find(_T("ISBN"), nOffset);		
		if ( nIndex == -1 )
		{
			break;			
		}
		else
		{
			nOffset = nIndex;
		}

		// ISBN문자열 뒤에 오는 ISBN을 체크한다.
		nIndex = strBody.Find(_T("978"), nOffset);		
		if ( nIndex == -1) 
		{
			break;
			//nIndex = strText.Find(_T("89"), nOffset);		
			//if ( nIndex == -1) break;
		}
		
		strISBN = _T("");
		if ( nIndex > 0 )
		{
			for ( i=nIndex ; i<nIndex+20 ; i++)
			{
				strCheck = strBody.Mid(i, 1);
				if ( strCheck == _T("-") || ( strCheck >= _T("0") && strCheck <= _T("9") ) )
				{
					strISBN += strCheck;
				}
				else
				{
					break;
				}
			}
		}

		//=====================================================
		//2009.02.25 UPDATE BY PJW : str과 int를 비교한 오류 수정, continue이더라도 +1해준다.
//  		if ( strISBN < 10 ) continue;
 		if ( strISBN.GetLength() < 10 ) 
 		{
  			nOffset = i+1;
 			continue;
 		}
		//=====================================================	

		if ( strIsbnTemp.Find(strISBN) < 0 )
		{
			pISBNList.Add(strISBN);
			strIsbnTemp += strISBN + _T(" ");
		}
		nOffset = i+1;
	}
}

VOID CExplorer::FindTextHtmlDocument(CString strText)
{	
	m_HtmlView->FindText(strText);
}

VOID CExplorer::OnDownloadBegin()
{
	if ( m_pParent->GetSafeHwnd() != NULL ) m_pParent->SendMessage(WM_USER+1016, NULL, NULL);
	//KillTimer(1);
}
VOID CExplorer::OnDownloadComplete()
{
	/*	
	BOOL bBusy = m_HtmlView->GetBusy();
	CString str2 = m_HtmlView->GetFullName();
	HICON hIcon = m_HtmlView->GetIcon(FALSE);
	CString str3 = m_HtmlView->GetLocationName();	
	BOOL bSlient = m_HtmlView->GetSilent();
	*/	

	// URL 변경 알림
	CString strLocationURL = m_HtmlView->GetLocationURL();
	if ( !strLocationURL.IsEmpty() )
	{
		m_strURL = strLocationURL;
		if ( m_strURL.Find(_T("http://"), 0) < 0 ) m_strURL = this->m_HtmlView->m_strSourceTitle;
		if ( m_pParent->GetSafeHwnd() != NULL ) m_pParent->SendMessage(WM_USER+1017, WPARAM(m_nInternetIndex), NULL);
	}	
	SetTimer(1, 100, NULL);	
	
}
void CExplorer::OnTimer(UINT nIDEvent) 
{
	// Download Complete 알림	
	READYSTATE rs = m_HtmlView->GetReadyState();
	if ( rs == READYSTATE_COMPLETE )
	{
		KillTimer(1);
		GetISBNList(m_pISBNList);
		if ( m_pParent->GetSafeHwnd() != NULL ) m_pParent->SendMessage(WM_USER+1014, WPARAM(m_nInternetIndex), NULL);
	}

	// URL 변경 알림
	CString strLocationURL = m_HtmlView->GetLocationURL();
	if ( !strLocationURL.IsEmpty() )
	{		
		m_strURL = strLocationURL;
		if ( m_strURL.Find(_T("http://"), 0) < 0 ) m_strURL = this->m_HtmlView->m_strSourceTitle;
		if ( m_pParent->GetSafeHwnd() != NULL ) m_pParent->SendMessage(WM_USER+1017, WPARAM(m_nInternetIndex), NULL);
	}
	
	CDialog::OnTimer(nIDEvent);
}


CString CExplorer::GetTitle()
{	
	return m_strTitle;
}

CString CExplorer::GetURL()
{	
	return m_strURL;
}

VOID CExplorer::SetURL(CString strURL)
{	
	m_strURL = strURL;	
}


VOID CExplorer::OnTitleChange(WPARAM wParam,LPARAM lParam)
{	
	CString *strTitle = (CString*)wParam;
	if ( !strTitle->IsEmpty() )
	{
		m_strTitle.Format(_T("%s"), *strTitle);		
		if ( m_pParent->GetSafeHwnd() != NULL ) m_pParent->SendMessage(WM_USER+1015, WPARAM(m_nInternetIndex), NULL);
	}
}

VOID CExplorer::OnStatusTextChange(WPARAM wParam,LPARAM lParam)
{	
	
	CString *StatusTextChanage = (CString*)wParam;
	if ( !StatusTextChanage->IsEmpty() )
	{	
		// 1. ISBN을 추출한다.
		CStringArray strArrayISBN;
		CString strText;
		
		strArrayISBN.RemoveAll();
		strText.Format(_T("%s"), *StatusTextChanage );		
		if ( strText == _T("") ) return;
		strText.MakeUpper();
		GetISBNList(strArrayISBN, strText);

		CString strISBN;
		CString strISBNList;
		INT nISBNCount;

		nISBNCount = strArrayISBN.GetSize();

		strISBNList = _T("");
		if (  nISBNCount > 0 )
		{
			for ( int i=0 ; i<nISBNCount ; i++ )
			{
				strISBN.Format(_T("%s"), strArrayISBN.GetAt(i) );
				strISBN = strISBN.Left(strISBN.GetLength()-1);
				strISBNList += strISBN + _T(" ");
			}		
			
			strISBNList.TrimLeft();
			strISBNList.TrimRight();

			if ( m_pParent->GetSafeHwnd() != NULL ) m_pParent->SendMessage(WM_USER+1018, WPARAM(m_nInternetIndex), (LPARAM)&strISBNList );			
		}
		
	}
	
}
