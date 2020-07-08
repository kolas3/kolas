// ECO_HtmlViewer.cpp : implementation file
//

#include "stdafx.h"
#include "ECO_HtmlViewer.h"
#include <atlbase.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CECO_HtmlViewer

IMPLEMENT_DYNCREATE(CECO_HtmlViewer, CHtmlView)

CECO_HtmlViewer::CECO_HtmlViewer()
{
	//{{AFX_DATA_INIT(CECO_HtmlViewer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = NULL;
}

CECO_HtmlViewer::~CECO_HtmlViewer()
{
}

void CECO_HtmlViewer::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CECO_HtmlViewer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CECO_HtmlViewer, CHtmlView)
	//{{AFX_MSG_MAP(CECO_HtmlViewer)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECO_HtmlViewer diagnostics

#ifdef _DEBUG
void CECO_HtmlViewer::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CECO_HtmlViewer::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CECO_HtmlViewer message handlers



void CECO_HtmlViewer::FindText(CString searchText, 
   long lFlags /* =2 */, BOOL bNNF /* =FALSE  (for internal use)*/)
{
    static CString sLastSearch;
    static BSTR lastBookmark = NULL;

    if(sLastSearch != searchText)
        lastBookmark = NULL;
    sLastSearch = searchText;
    

    IHTMLDocument2 *lpHtmlDocument = NULL;
    LPDISPATCH lpDispatch = NULL;
    lpDispatch = GetHtmlDocument();
    ASSERT(lpDispatch);

    lpDispatch->QueryInterface(IID_IHTMLDocument2, 
                                (void**)&lpHtmlDocument);
    ASSERT(lpHtmlDocument);

    lpDispatch->Release();

    IHTMLElement *lpBodyElm;
    IHTMLBodyElement *lpBody;
    IHTMLTxtRange *lpTxtRange;

    lpHtmlDocument->get_body(&lpBodyElm);
    ASSERT(lpBodyElm);
    lpHtmlDocument->Release();
    lpBodyElm->QueryInterface(IID_IHTMLBodyElement,(void**)&lpBody);
    ASSERT(lpBody);
    lpBodyElm->Release();
    lpBody->createTextRange(&lpTxtRange);
    ASSERT(lpTxtRange);
    lpBody->Release();

    CComBSTR search(searchText.GetLength()+1,(LPCTSTR)searchText);
    bool bFound,bTest;
    long t;

    if(lastBookmark!=NULL)
    {
        lpTxtRange->moveToBookmark(lastBookmark,
                               (VARIANT_BOOL*)&bTest);
        if(!bTest)
        {
            lastBookmark=NULL;
            lpTxtRange->moveStart((BSTR)CComBSTR("Textedit"),1,&t);
            lpTxtRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
        } else
        {
            lpTxtRange->moveStart((BSTR)CComBSTR("Character"),1,&t);
            lpTxtRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
        }
    } else
    {
        lpTxtRange->moveStart((BSTR)CComBSTR("Textedit"),0,&t);
        lpTxtRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
    }
    lpTxtRange->findText((BSTR)search,0,
                    lFlags,(VARIANT_BOOL*)&bFound);

    if(!bFound)
    {
        if(lastBookmark==NULL && !bNNF)
        {
            CString message;
            message.Format(_T("Cannot find the string: '%s'"),searchText);
            AfxMessageBox(message);
        } else if(lastBookmark!=NULL)
        {
            lastBookmark = NULL;
            FindText(searchText,lFlags,TRUE);
        }
    } else
    {
        if(lpTxtRange->getBookmark(&lastBookmark)!=S_OK)
            lastBookmark=NULL;
        lpTxtRange->select();
        lpTxtRange->scrollIntoView(TRUE);
    }

    lpTxtRange->Release();
}

void CECO_HtmlViewer::FindText2(CString searchText, 
   long lFlags /* =2 */, CString matchStyle, CString searchID)
{
    ClearSearchResults(searchID);

    IHTMLDocument2 *lpHtmlDocument = NULL;
    LPDISPATCH lpDispatch = NULL;
    lpDispatch = GetHtmlDocument();
    ASSERT(lpDispatch);

    lpDispatch->QueryInterface(IID_IHTMLDocument2, 
                               (void**)&lpHtmlDocument);
    ASSERT(lpHtmlDocument);

    lpDispatch->Release();

    IHTMLElement *lpBodyElm;
    IHTMLBodyElement *lpBody;
    IHTMLTxtRange *lpTxtRange;

    lpHtmlDocument->get_body(&lpBodyElm);
    ASSERT(lpBodyElm);
    lpHtmlDocument->Release();
    lpBodyElm->QueryInterface(IID_IHTMLBodyElement,(void**)&lpBody);
    ASSERT(lpBody);
    lpBodyElm->Release();
    lpBody->createTextRange(&lpTxtRange);
    ASSERT(lpTxtRange);
    lpBody->Release();

    CComBSTR html;
    CComBSTR newhtml;
    CComBSTR search(searchText.GetLength()+1,(LPCTSTR)searchText);

    long t;
    bool bFound;
    while(lpTxtRange->findText(search,0,lFlags,
                        (VARIANT_BOOL*)&bFound),bFound)
    {
        newhtml.Empty();
        lpTxtRange->get_htmlText(&html);
        newhtml.Append(_T(""));
        newhtml.Append((LPCTSTR)searchID);
        newhtml.Append("' style='");
        newhtml.Append((LPCTSTR)matchStyle);
        newhtml.Append("'>");
        if(searchText==" ")
            // doesn't work very well, but prevents (some) shit
            newhtml.Append(" ;"); 
        else 
            newhtml.AppendBSTR(html);
        newhtml.Append("");
        lpTxtRange->pasteHTML(newhtml);
                
        lpTxtRange->moveStart((BSTR)CComBSTR("Character"),1,&t);
        lpTxtRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
    }

    lpTxtRange->Release();
} 

void CECO_HtmlViewer::ClearSearchResults(CString searchID)
{
    CComBSTR testid(searchID.GetLength()+1,searchID);
    CComBSTR testtag(5,"SPAN");
    IHTMLDocument2 *lpHtmlDocument = NULL;
    LPDISPATCH lpDispatch = NULL;
    lpDispatch = GetHtmlDocument();
    ASSERT(lpDispatch);

    lpDispatch->QueryInterface(IID_IHTMLDocument2, 
                                (void**)&lpHtmlDocument);
    ASSERT(lpHtmlDocument);
    lpDispatch->Release();

    IHTMLElementCollection *lpAllElements;
    lpHtmlDocument->get_all(&lpAllElements);
    ASSERT(lpAllElements);
    lpHtmlDocument->Release();

    IUnknown *lpUnk;
    IEnumVARIANT *lpNewEnum;
    if (SUCCEEDED(lpAllElements->get__newEnum(&lpUnk)) && lpUnk != NULL)
    {
        lpUnk->QueryInterface(IID_IEnumVARIANT,(void**)&lpNewEnum);
        ASSERT(lpNewEnum);
        VARIANT varElement;
        IHTMLElement *lpElement;

        while (lpNewEnum->Next(1, &varElement, NULL) == S_OK)
        {
            _ASSERTE(varElement.vt == VT_DISPATCH);
            varElement.pdispVal->QueryInterface
                   (IID_IHTMLElement,(void**)&lpElement);
            ASSERT(lpElement);
            if (lpElement)
            {
                CComBSTR id;
                CComBSTR tag;
                lpElement->get_id(&id);
                lpElement->get_tagName(&tag);
                if((id==testid)&&(tag==testtag))
                {
                    BSTR innerText;
                    lpElement->get_innerHTML(&innerText);
                    lpElement->put_outerHTML(innerText);
                }
            }
            VariantClear(&varElement);
        }
    }
} 
BOOL CECO_HtmlViewer::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{		
	return CHtmlView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
CString CECO_HtmlViewer::GetHTMLSource()
{
	/*
	CComPtr<IDispatch> pDisp = GetHtmlDocument();
    CComQIPtr<IHTMLDocument2> pDoc = pDisp ;
    CComPtr<IHTMLElementCollection> pCol = NULL;
    pDoc->get_all( &pCol) ;
    long lSize ;
    pCol->get_length( &lSize);
    CString strHtml;
    if( lSize != 0){
        // all ªÎ?é»ªò«ê«¹«È«¢«Ã«×
        USES_CONVERSION ;
        BSTR bStr;
        for( long i = 0; i < lSize; i++){
            CComQIPtr<IHTMLElement> pElement ;
            CComVariant  vintName( i) ;
            CComVariant  vintIndex( 0) ;
            CComPtr  pdsp2 ;
            pCol->item( vintName, vintIndex, &pdsp2) ;
            pElement = pdsp2 ;
            pElement->get_innerHTML(&bStr);
            strHtml = bStr;
        }
    }
	*/
	CString strText;
	IDispatch* pDispatch = GetHtmlDocument();
    //Only if a resource or web page is loaded can we get the document
    if( pDispatch != NULL )
    {
        IHTMLDocument2* pHtmlDoc;
        HRESULT hr = pDispatch->QueryInterface( __uuidof( IHTMLDocument2 ), (void**)&pHtmlDoc );
        pDispatch->Release();
        if( SUCCEEDED( hr ) )
        {
            //Get access to the internal document
            IHTMLElement *pElement;
			BSTR bsHeaderText;	
			hr = pHtmlDoc->get_title(&bsHeaderText);
			if( SUCCEEDED( hr ) )
            {
				CString sBody( OLE2T(bsHeaderText) );
                SysFreeString( bsHeaderText );				
				m_strSourceTitle = bsHeaderText;
			}
			

            hr = pHtmlDoc->get_body( &pElement );
            if( SUCCEEDED( hr ) )
            {
                //Extract the data
                USES_CONVERSION;
                BSTR bsMainText;
				
				if ( pElement != NULL )
				{				
					pElement->get_innerHTML( &bsMainText );
					CString sBody( OLE2T(bsMainText) );
					SysFreeString( bsMainText );
					pElement->Release();
					strText = sBody;
				}

				/*
                //Append the new text
                sBody += _T("<p>Just added this line at ");
                sBody += CTime::GetCurrentTime().Format( _T("%H:%M:%S") );
                sBody += _T("</p>");

                //Assign the new text
                
				  bsMainText = SysAllocString( (sBody) );
                pElement->put_innerHTML( bsMainText );
                SysFreeString( bsMainText );
                pElement->Release();
				*/
            }
        }
    }


	return strText;
}
/*
static LRESULT ViewSource(IHTMLDocument3* pDoc3)
{
	LPDISPATCH lpDispatch = NULL;
	LPOLECOMMANDTARGET lpOleCommandTarget = NULL;



	try
	{
		lpDispatch = pDoc3; //GetHtmlDocument();
		if(lpDispatch == NULL)
			return S_OK;
		ASSERT(lpDispatch);

		// Get an IDispatch pointer for the IOleCommandTarget interface.
		lpDispatch->QueryInterface(IID_IOleCommandTarget, (void**)&lpOleCommandTarget);
		ASSERT(lpOleCommandTarget);

		lpDispatch->Release();
		lpDispatch = NULL;

		// Invoke the given command id for the WebBrowser control
		if(lpOleCommandTarget != NULL)
		{
			HRESULT hr = lpOleCommandTarget->Exec(&CGID_IWebBrowser, 2, 0,
				NULL, NULL);
			lpOleCommandTarget->Release();
			lpOleCommandTarget = NULL;
			return hr;
		}
		else
			return S_OK;
	}
	catch(...)
	{	
	}
	return S_OK;
}
*/


/*
const CString CECO_HtmlViewer::GetDocumentHtml() const
{
    MsHtml::IHTMLDocument2Ptr doc = GetHtmlDocument();

    IPersistStreamInitPtr stream = doc;

      // From AFXHTML.H.

    CStreamOnCString sstream;

  

    stream->Save( static_cast<IStream*>(&sstream), false );

    

    CString result;

    VERIFY(sstream.CopyData( result ));

  

    return result;

}
*/
void CECO_HtmlViewer::OnDownloadBegin() 
{
	if ( m_pParent->GetSafeHwnd() != NULL ) m_pParent->SendMessage(WM_USER+1012, NULL, NULL);	
	CHtmlView::OnDownloadBegin();
}

void CECO_HtmlViewer::OnDownloadComplete() 
{
	if ( m_pParent->GetSafeHwnd() != NULL ) m_pParent->SendMessage(WM_USER+1013, NULL, NULL);
	CHtmlView::OnDownloadComplete();
}

void CECO_HtmlViewer::OnFinalRelease() 
{
	//AfxMessageBox(_T("OnFinalRelease"));
	CHtmlView::OnFinalRelease();
}

void CECO_HtmlViewer::SetParent(CWnd* pParent)
{
	m_pParent = pParent;
}


BOOL CECO_HtmlViewer::OnEraseBkgnd(CDC* pDC) 
{	
	//return CHtmlView::OnEraseBkgnd(pDC);
	return FALSE;
}

void CECO_HtmlViewer::OnSize(UINT nType, int cx, int cy) 
{
	if (::IsWindow(m_wndBrowser.m_hWnd))
    {
        CRect rect;
        GetClientRect(rect);
        //::AdjustWindowRectEx(rect,m_wndBrowser.GetStyle(), FALSE, WS_EX_CLIENTEDGE);
        m_wndBrowser.SetWindowPos(NULL, rect.left-2, rect.top-2,rect.Width(), rect.Height()+4, SWP_NOACTIVATE | SWP_NOZORDER);

    }
}

void CECO_HtmlViewer::OnTitleChange(LPCTSTR lpszText) 
{	
	if ( m_pParent->GetSafeHwnd() != NULL ) m_pParent->SendMessage(WM_USER + 1100, (WPARAM)&lpszText, NULL );
	CHtmlView::OnTitleChange(lpszText);
}

void CECO_HtmlViewer::OnStatusTextChange(LPCTSTR lpszText) 
{
	if ( m_pParent->GetSafeHwnd() != NULL ) m_pParent->SendMessage(WM_USER + 1101, (WPARAM)&lpszText, NULL );	
	// 18.11.07 JMJ KOL.RED.2018.020
	CString strText = lpszText;
	strText.MakeLower();
	if(strText.Find(_T("event:")) == -1)
		CHtmlView::OnStatusTextChange(lpszText);
}
// 18.11.07 JMJ KOL.RED.2018.020
VOID CECO_HtmlViewer::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
{
	CString strText = lpszURL;
	strText.MakeLower();
	if(strText.Find(_T("event:")) >= 0)
	{
		strText.Replace(_T("event:"), _T(""));
		if ( m_pParent->GetSafeHwnd() != NULL ) 
			m_pParent->SendMessage(WM_USER + 87, NULL, (LPARAM)strText.GetBuffer(0));	
		*pbCancel = TRUE;
		return;
	}
}