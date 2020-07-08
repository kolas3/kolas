// ILL_SEARCH_BOOK.cpp : implementation file
//

#include "stdafx.h"
#include "ILL_SEARCH_BOOK.h"
#include "Imm.h"
#include "..\..\..\..\공통\K2UP_Common\MakeSearchData.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"
#include "ILL_SEARCH_BOOK_LIST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CILL_SEARCH_BOOK dialog


CILL_SEARCH_BOOK::CILL_SEARCH_BOOK(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CILL_SEARCH_BOOK)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CILL_SEARCH_BOOK::~CILL_SEARCH_BOOK()
{
}

VOID CILL_SEARCH_BOOK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CILL_SEARCH_BOOK)	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CILL_SEARCH_BOOK, CDialog)
	//{{AFX_MSG_MAP(CILL_SEARCH_BOOK)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnUSERFIND1, OnbtnUSERFIND1)
	ON_BN_CLICKED(IDC_btnUSERCLOSE, OnbtnUSERCLOSE)
	ON_BN_CLICKED(IDC_btnCLEAR, OnbtnCLEAR)
	ON_BN_CLICKED(IDC_btnSEARCH_DETAIL, OnbtnSEARCHDETAIL)
	ON_EN_SETFOCUS(IDC_edtUSERFIND1, OnSetfocusedtUSERFIND1)
	ON_WM_SHOWWINDOW()
	ON_EN_SETFOCUS(IDC_edtSREG_NO, OnSetfocusedtSREGNO)
	ON_EN_CHANGE(IDC_edtUSERFIND9, OnChangeedtUSERFIND9)
	ON_EN_CHANGE(IDC_edtUSERFIND10, OnChangeedtUSERFIND10)
	ON_EN_SETFOCUS(IDC_edtUSERFIND2, OnSetfocusedtUSERFIND2)
	ON_EN_SETFOCUS(IDC_edtUSERFIND3, OnSetfocusedtUSERFIND3)
	ON_EN_SETFOCUS(IDC_edtUSERFIND4, OnSetfocusedtUSERFIND4)
	ON_EN_SETFOCUS(IDC_edtUSERFIND5, OnSetfocusedtUSERFIND5)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MENU, OnSelchangeTabMenu)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SHOWED, OnShowed)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CILL_SEARCH_BOOK message handlers

BOOL CILL_SEARCH_BOOK::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::Create(IDD, pParentWnd);
}

VOID CILL_SEARCH_BOOK::OnClose() 
{
	
	CDialog::OnClose();

	
}

BOOL CILL_SEARCH_BOOK::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("ILL_SEARCH_BOOK")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	((CButton*)GetDlgItem(IDC_chkUSE_LIMIT))->SetCheck(1);


	SetWindowSize( CString(_T("간략검색")) );
	((CComboBox*)GetDlgItem(IDC_cmbUSERFIND13))->SetCurSel(0);

	InitTabControl();

	return TRUE;  // return TRUE unless you set the focus to a control
}

VOID CILL_SEARCH_BOOK::OnbtnUSERFIND1() 
{
	
	
	INT ids = 0;
	CString strFuncName = _T("CILL_SEARCH_BOOK::OnbtnUSERFIND1()");
	
	{
		CString sWhere;
		ids = GetSqlSearchBookInfo(sWhere, 0);
		if(0 != ids) return;

		CILL_SEARCH_BOOK_LIST Dlg(this);
		Dlg.m_pInfo = m_pInfo;
		Dlg.m_strSearch_Option_BO = sWhere;
		Dlg.m_strSearch_Option_SE = sWhere;
		Dlg.m_sBookCooperativeMode = m_sBookCooperativeMode;

		CESL_ControlComboBox* pCMB = (CESL_ControlComboBox*)FindControl(
			_T("CM_ILL_SEARCH_BOOK"), _T("자료구분"));
		if(NULL == pCMB) ERROR_MSG_RETURN_VOID2(-5, strFuncName);

		CString strResult;
		pCMB->GetWindowText(strResult);
		if(_T("단행") == strResult)
		{
			Dlg.m_bIs_BO_Search = TRUE;
		}
		else if(_T("연속") == strResult)
		{
			Dlg.m_bIs_SE_Search = TRUE;
		}
		else if(_T("전체") == strResult)
		{
			Dlg.m_bIs_BO_Search = TRUE;
			Dlg.m_bIs_SE_Search = TRUE;
		}
		else ERROR_MSG_RETURN_VOID2(-6, strFuncName);

		if(IDOK == Dlg.DoModal())
		{
			int nIdx = Dlg.m_nSelectIdx;
			
			CESL_DataMgr* pBookDM = Dlg.m_pSelectDM;
			if(NULL == pBookDM) ERROR_MSG_RETURN_VOID2(-1, strFuncName);
			
			CESL_ControlMgr* pAplCM = FindCM(_T("CM_ILL_APL"));
			if(NULL == pAplCM) ERROR_MSG_RETURN_VOID2(-2, strFuncName);

			const int GET_COUNT = 12;
			CString strDMAlias[GET_COUNT] = 
			{
				_T("등록번호"),
				_T("청구기호"),
				_T("서명"),
				_T("저작자"),
				_T("발행자"),
				_T("발행년도"),
				_T("편권차"),
				_T("도서관_이름"),
				_T("도서관_코드"),
				_T("발행형태구분"),
				_T("책KEY"),
				_T("관리구분")

			};
			CString strEditAlias[GET_COUNT] = 
			{
				_T("책_등록번호"),
				_T("책_청구기호"),
				_T("책_서명"),
				_T("책_저자"),
				_T("책_발행자"),
				_T("책_발행년도"),
				_T("책_권"),
				_T("신청_제공도서관이름"),
				_T("신청_제공도서관코드"),
				_T("책_발행형태구분"),
				_T("책_키"),
				_T("책_관리구분")
			};
			
			for(int nCnt = 0; nCnt < GET_COUNT; nCnt++)
			{
				CString strGet;
				ids = pBookDM->GetCellData(strDMAlias[nCnt], nIdx, strGet);
				if(0 > ids) ERROR_MSG_RETURN_VOID2(-3, strFuncName);
				
				CESL_ControlEdit* pEdit = (CESL_ControlEdit*)pAplCM->FindControl(
					strEditAlias[nCnt]);
				if(NULL == pEdit) ERROR_MSG_RETURN_VOID2(-4, strFuncName);
				
				ids = pEdit->SetData(strGet);
				if(0 > ids) ERROR_MSG_RETURN_VOID2(-5, strFuncName);
				
			}			

			CDialog::OnOK();
		}
	}

	
}

VOID CILL_SEARCH_BOOK::OnbtnUSERCLOSE() 
{
	OnCancel();	
}

VOID CILL_SEARCH_BOOK::OnbtnCLEAR() 
{
	

	INT ids;
	CString sCMAlias = _T("CM_ILL_SEARCH_BOOK");
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnCLEAR") );

	ids = pCM->AllControlClear();
	if( ids < 0 ) return ;

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(_T("CM_ILL_SEARCH_BOOK"), _T("관리구분"));
	if(NULL != pCMB)
	{
		const int CMB_COUNT = pCMB->GetRowCount();
		for(int nCnt = 0; nCnt < CMB_COUNT; nCnt++)
		{
			CString strData;
			pCMB->GetData(strData, nCnt, 1);
			if(_T("적용안함") == strData)
			{
				pCMB->SetCurSel(nCnt);
				break;
			}
		}
	}

	GetDlgItem(IDC_edtUSERFIND1)->SetFocus();
	
}

VOID CILL_SEARCH_BOOK::SetWindowSize( CString &sCaption )
{
	

	CRect ChangeWndPos;
	CRect WndPos;
	
	INT nIDC;
	
	TCHAR* szCaption[2] = { _T("상세검색") , _T("간략검색") };

	if( sCaption.Compare( szCaption[0] ) == 0 )
	{
		GetDlgItem( IDC_btnSEARCH_DETAIL )->SetWindowText( CString(szCaption[1]) );
		nIDC = IDC_staSEARCH_VIEWDETAIL;
	}
	else
	{
		GetDlgItem( IDC_btnSEARCH_DETAIL )->SetWindowText( CString(szCaption[0]) );
		nIDC = IDC_staSEARCH_VIEWSIMPLE;
	}

	GetWindowRect( &WndPos );

	GetDlgItem( nIDC )->GetWindowRect( &ChangeWndPos );
	
	ClientToScreen(&WndPos);
	ClientToScreen(&ChangeWndPos);

	WndPos.SetRect( WndPos.left , WndPos.top , WndPos.right , ChangeWndPos.bottom );
	
	ScreenToClient(&WndPos);

	MoveWindow( WndPos );	

	
}

BOOL CILL_SEARCH_BOOK::PreTranslateMessage(MSG* pMsg) 
{
	

	DWORD lstyle = GetStyle();
	if ( WS_POPUP == (lstyle & WS_POPUP) ) {
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
		{
			OnbtnUSERFIND1();
			return TRUE;
		}
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
		{
			OnbtnUSERCLOSE();
			return TRUE;
		}
	}
	
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

VOID CILL_SEARCH_BOOK::OnbtnSEARCHDETAIL() 
{
	

	CString sCaption;
	GetDlgItem( IDC_btnSEARCH_DETAIL )->GetWindowText( sCaption );
	SetWindowSize( sCaption );		

	
}

VOID CILL_SEARCH_BOOK::OnSetfocusedtUSERFIND1() 
{
	
    HIMC hIME;
    DWORD dwConversion, dwSentence;
	
    hIME = ::ImmGetContext(GetDlgItem(IDC_edtUSERFIND1)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;
		
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
		
        ImmReleaseContext(GetDlgItem(IDC_edtUSERFIND1)->m_hWnd,hIME); 
    } 
	
	
}

VOID CILL_SEARCH_BOOK::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	PostMessage( WM_SHOWED );

}


LRESULT CILL_SEARCH_BOOK::OnShowed(WPARAM wParam, LPARAM lParam)
{
	if( IsWindowVisible() )
	{
		OnbtnCLEAR();
		GetDlgItem(IDC_edtUSERFIND1)->SetFocus();
	}

	return 0;
}
VOID CILL_SEARCH_BOOK::OnSetfocusedtSREGNO() 
{
	// TODO: Add your control notification handler code here
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtSREG_NO)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion = dwConversion & ~IME_CMODE_LANGUAGE;
		dwConversion |= IME_CMODE_ALPHANUMERIC;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtSREG_NO)->m_hWnd,hIME); 
    }
}

VOID CILL_SEARCH_BOOK::OnChangeedtUSERFIND9() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	CheckClassNo( IDC_edtUSERFIND9 );
}

VOID CILL_SEARCH_BOOK::OnChangeedtUSERFIND10() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	CheckClassNo( IDC_edtUSERFIND10 );
}

VOID CILL_SEARCH_BOOK::CheckClassNo( UINT ID )
{
	return;
	
	CEdit *pEdt = ( CEdit * )GetDlgItem( ID );
	CString sClassNo, sNewClassNo;
	pEdt->GetWindowText( sClassNo );

	sClassNo.TrimLeft();
	sClassNo.TrimRight();
	sClassNo.Replace( _T(" "), _T("") );
	INT len = sClassNo.GetLength();
	if( !len )
		return;

	TCHAR cClassNo;
	for( INT i = 0; i < len; i++ )
	{
		cClassNo = sClassNo.GetAt( i );

		if( ( cClassNo >= '0' && cClassNo <= '9' ) || cClassNo == '.' )
			sNewClassNo += cClassNo;
	}
		
	pEdt->SetWindowText( sNewClassNo );
	len = sNewClassNo.GetLength();
	pEdt->SetSel( len, len );
}

VOID CILL_SEARCH_BOOK::OnSetfocusedtUSERFIND2() 
{
	// TODO: Add your control notification handler code here
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtUSERFIND2)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtUSERFIND2)->m_hWnd,hIME); 
    }
}

VOID CILL_SEARCH_BOOK::OnSetfocusedtUSERFIND3() 
{
	// TODO: Add your control notification handler code here
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtUSERFIND3)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtUSERFIND3)->m_hWnd,hIME); 
    }
}

VOID CILL_SEARCH_BOOK::OnSetfocusedtUSERFIND4() 
{
	// TODO: Add your control notification handler code here
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtUSERFIND4)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtUSERFIND4)->m_hWnd,hIME); 
    }
}

VOID CILL_SEARCH_BOOK::OnSetfocusedtUSERFIND5() 
{
	// TODO: Add your control notification handler code here
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtUSERFIND5)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtUSERFIND5)->m_hWnd,hIME); 
    }
}

void CILL_SEARCH_BOOK::OnSelchangeTabMenu(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_MENU);
	if( pTab )
	{
		INT nFlag = pTab->GetCurSel();

		CButton* pBtn = (CButton*)GetDlgItem(IDC_chkUSE_UserSearch);
		if( pBtn )
		{
			pBtn->SetCheck(nFlag);
		}
	}

	*pResult = 0;
}

void CILL_SEARCH_BOOK::InitTabControl()
{


	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_MENU);
	if( NULL == pTab )	return;

	TC_ITEM item;
	item.mask = TCIF_IMAGE | TCIF_TEXT;
	item.iImage = 0;
	
	item.pszText = _T("업무용 자료찾기");
	pTab->InsertItem(0, &item);
	
	item.pszText = _T("이용자용 자료찾기");
	pTab->InsertItem(1, &item);

	pTab->SetCurSel(0);


}


HBRUSH CILL_SEARCH_BOOK::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

INT CILL_SEARCH_BOOK::GetSqlSearchBookInfo( CString &sWhere , INT nMode )
{


	INT ids;

	INT nSearchField = 0;

	CString sCMAlias = _T("CM_ILL_SEARCH_BOOK");
	const INT nFieldCnt = 4;

	TCHAR *sFieldAlias[nFieldCnt] = 
	{
		_T("책제목") , _T("저자명") , _T("발행자명") , _T("전체")
	};

	CString sGetData[nFieldCnt];

	for( INT i = 0 ; i < nFieldCnt ;i++ )
	{
		ids = GetControlData( sCMAlias , sFieldAlias[i] , sGetData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );
		sGetData[i].TrimRight();
	}

	CMakeSearchData MakeSearchData(this);

	FILE *fd = _tfopen(_T("..\\CFG\\_newecosearch"), _T("rb"));

	CString sEcoSearchWhere;
	INT nEcoSearchCnt = 0;
		
	BOOL bIsEqualSearch;

	///////
	if( !sGetData[0].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		
			bIsEqualSearch = FALSE;
			if( sGetData[0].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			MakeSearchData.GetKeyWord(sGetData[0]);
			if( !bIsEqualSearch ) 
				sGetData[0] += _T("*");
			
			if( 1 == m_pInfo->GetSearchIndexType() )
			{
				if( fd != NULL )
				{
					
					if( nMode == 0 )
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , BO_IDX_TITLE, '") + sGetData[0] + _T("') ");
					else
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_TITLE, '") + sGetData[0] + _T("') ");
				}
				else
				{
					if( nMode == 0 )
						sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_TITLE, '") + sGetData[0] + _T("') ");
					else
						sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_TITLE, '") + sGetData[0] + _T("') ");
					
					nEcoSearchCnt++;
					
				}
			}
			else
				sWhere += _T(" CATSEARCH( IB.IDX_TITLE, '") + sGetData[0] + _T("',NULL) > 0 ");

		nSearchField++;
	}

	if( !sGetData[1].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

			bIsEqualSearch = FALSE;
			if( sGetData[1].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			MakeSearchData.GetKeyWord(sGetData[1]);
			
			if( !bIsEqualSearch ) 
				sGetData[1] += _T("*");

			if( 1 == m_pInfo->GetSearchIndexType() )
			{
				if( fd != NULL )
				{
					
					if( nMode == 0 )
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , BO_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
					else
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
					
				}
				else
				{
					if( !sEcoSearchWhere.IsEmpty() )
						sEcoSearchWhere += _T(" INTERSECT ");
					
					if( nMode == 0 )
						sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
					else
						sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
					
					nEcoSearchCnt++;
				}
				
			}
			else
				sWhere += _T(" CATSEARCH( IB.IDX_AUTHOR, '") + sGetData[1] + _T("',NULL) > 0 ");

		nSearchField++;
	}

	if( !sGetData[2].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

			bIsEqualSearch = FALSE;
			if( sGetData[2].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			MakeSearchData.GetKeyWord(sGetData[2]);
			if( !bIsEqualSearch ) 
				sGetData[2] += _T("*");
			if( 1 == m_pInfo->GetSearchIndexType() )
			{
				
				if( fd != NULL )
				{
					if( nMode == 0 )
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , BO_IDX_PUBLISHER , '") + sGetData[2] + _T("') ");
					else
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_PUBLISHER , '") + sGetData[2] + _T("') ");
				}
				else
				{
					if( !sEcoSearchWhere.IsEmpty() )
						sEcoSearchWhere += _T(" INTERSECT ");
					
					if( nMode == 0 )
						sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_PUBLISHER , '") + sGetData[2] + _T("') ");
					else
						sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_PUBLISHER , '") + sGetData[2] + _T("') ");
					
					nEcoSearchCnt++;
				}
			}
			else			
				sWhere += _T(" CATSEARCH( IB.IDX_PUBLISHER, '") + sGetData[2] + _T("',NULL) > 0 ");

		nSearchField++;
	}

	if( !sGetData[3].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

			bIsEqualSearch = FALSE;
			if( sGetData[3].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			MakeSearchData.GetKeyWord(sGetData[3]);
			if( !bIsEqualSearch ) 
				sGetData[3] += _T("*");
			if( 1 == m_pInfo->GetSearchIndexType() )
			{
				
				if( fd != NULL )
				{
					if( nMode == 0 )
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , BO_IDX_ALLITEM , '") + sGetData[3] + _T("') ");
					else
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , SE_IDX_ALLITEM , '") + sGetData[3] + _T("') ");
				}
				else
				{
					if( !sEcoSearchWhere.IsEmpty() )
						sEcoSearchWhere += _T(" INTERSECT ");
					
					if( nMode == 0 )
						sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_ALLITEM , '") + sGetData[3] + _T("') ");
					else
						sEcoSearchWhere += _T(" &ECOSEARCH( , SE_IDX_ALLITEM , '") + sGetData[3] + _T("') ");
					
					nEcoSearchCnt++;
				}
			}
			else			
				sWhere += _T(" CATSEARCH( IB.IDX_ALL_ITEM, '") + sGetData[3] + _T("',NULL) > 0 ");
		
		nSearchField++;
	}

 	const INT nComboFieldCnt = 3;
 	TCHAR *sComboAlias[nComboFieldCnt] =
 	{
 		_T("자료형태") , _T("매체구분") , _T("자료실")
 	};
 
 	TCHAR *sDBFieldAlias[ nComboFieldCnt ] =
 	{
 		_T("IB.FORM_CODE") , _T("IB.MEDIA_CODE") , _T("BB.SHELF_LOC_CODE")
 	};
 
 	CString sComboGetData[nComboFieldCnt];
 
 	for( i = 0 ; i < nComboFieldCnt ; i++ )
 	{
 		ids = GetControlData( sCMAlias , sComboAlias[i] , sComboGetData[i] );
 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );
 		sComboGetData[i].TrimRight();
 		if( sComboGetData[i].IsEmpty() || (sComboGetData[i].Compare(_T("0"))==0) || (sComboGetData[i].Compare(_T("적용안함"))==0) ) continue;
 		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
 		sWhere += sDBFieldAlias[i] + CString(_T(" = '")) + sComboGetData[i] + CString(_T("' "));
 	}

	const INT nKeyWordFieldCnt = 5;
	TCHAR *sKeyWordAlias[nKeyWordFieldCnt] =
	{
		_T("키워드1") , _T("키워드2") , _T("키워드3") , _T("키워드4") , _T("키워드5")
	};
	CString sKeyWordGetData[nKeyWordFieldCnt];

	for( i = 0 ; i < nKeyWordFieldCnt ; i++ )
	{
		ids = GetControlData( sCMAlias , sKeyWordAlias[i] , sKeyWordGetData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );
		sKeyWordGetData[i].TrimRight();
	}
	
	CString sKeyWordWhere;
	if( !sKeyWordGetData[0].IsEmpty() )
	{
			bIsEqualSearch = FALSE;
			if( sKeyWordGetData[0].GetAt(0) == '\"' )
				bIsEqualSearch = TRUE;
			
			MakeSearchData.GetKeyWord(sKeyWordGetData[0]);
			if( !bIsEqualSearch ) 
				sKeyWordGetData[0] += _T("*");
			sKeyWordWhere = sKeyWordGetData[0];

		if( !sKeyWordGetData[2].IsEmpty() )
		{
				bIsEqualSearch = FALSE;
				if( sKeyWordGetData[2].GetAt(0) == '\"' )
					bIsEqualSearch = TRUE;
				
				MakeSearchData.GetKeyWord(sKeyWordGetData[0]);
				if( !bIsEqualSearch ) 
					sKeyWordGetData[2] += _T("*");
							
				sKeyWordGetData[1].MakeUpper();
				if( sKeyWordGetData[1].Compare(_T("OR")) == 0 )
					sKeyWordWhere += _T(" | ");
				else
					sKeyWordWhere += _T(" ");
				sKeyWordWhere += sKeyWordGetData[2];				
		}
		if( !sKeyWordGetData[4].IsEmpty() )
		{
		
				bIsEqualSearch = FALSE;
				if( sKeyWordGetData[4].GetAt(0) == '\"' )
					bIsEqualSearch = TRUE;
				
				MakeSearchData.GetKeyWord(sKeyWordGetData[4]);
				if( !bIsEqualSearch ) 
					sKeyWordGetData[4] += _T("*");
				sKeyWordGetData[3].MakeUpper();
				if( sKeyWordGetData[3].Compare(_T("OR")) == 0 )
					sKeyWordWhere += _T(" | ");
				else
					sKeyWordWhere += _T(" ");
				sKeyWordWhere += sKeyWordGetData[4];				
		}

			if( !sKeyWordWhere.IsEmpty() )
			{

				if( 1 == m_pInfo->GetSearchIndexType() )
				{
					if( fd != NULL )
					{
						sWhere += _T(" &ECOSEARCH( IB.REC_KEY , BO_IDX_KEYWORD , '") + sKeyWordWhere + _T("') ");
					}
					else
					{
						if( !sEcoSearchWhere.IsEmpty() )
							sEcoSearchWhere += _T(" INTERSECT ");
						
						sEcoSearchWhere += _T(" &ECOSEARCH( , BO_IDX_KEYWORD , '") + sKeyWordWhere + _T("') ");
					}
					nEcoSearchCnt++;
				}
				else
				{
					if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
					sWhere += CString(_T(" CATSEARCH(IB.IDX_KEYWORD,'")) + sKeyWordWhere + CString(_T("',NULL)>0 "));
				}
				nSearchField++;
			}

		if( fd != NULL )
		sWhere += _T(" )");
	}
	
	const INT nPublishYearFieldCnt = 2;
	TCHAR *sPublishYearAlias[nPublishYearFieldCnt] =
	{
		_T("발행년도1") , _T("발행년도2")
	};
	CString sPublishYearGetData[nPublishYearFieldCnt];

	for( i = 0 ; i < nPublishYearFieldCnt ; i++ )
	{
		ids = GetControlData( sCMAlias , sPublishYearAlias[i] , sPublishYearGetData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );
		sPublishYearGetData[i].TrimRight();
	}

	if( !sPublishYearGetData[0].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( sPublishYearGetData[1].IsEmpty() )
		{

				sWhere += CString(_T(" IB.IDX_IPUB_YEAR = '")) + sPublishYearGetData[0] + CString(_T("' "));
		}
		else
			sWhere += CString(_T(" ( IB.IDX_IPUB_YEAR >= '")) + sPublishYearGetData[0] + CString(_T("' AND "))
						+ CString(_T(" IB.IDX_IPUB_YEAR <= '")) + sPublishYearGetData[1] + CString(_T("')"));
	}

	const INT nClassNoCnt = 2;
	TCHAR* sClassNoAlias[nClassNoCnt] =
	{
		_T("분류기호1") , _T("분류기호2")
	};
	CString sClassNoData[nClassNoCnt];

	for( i = 0 ; i < nClassNoCnt ; i++ )
	{
		ids = GetControlData( sCMAlias , sClassNoAlias[i] , sClassNoData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );
		sClassNoData[i].TrimRight();
	}

	if( !sClassNoData[0].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

		if( sClassNoData[1].IsEmpty() )
		{
			sWhere += _T(" BB.CLASS_NO = '") + sClassNoData[0] + _T("' ");
		}
		else
		{
			sWhere += _T(" BB.CLASS_NO BETWEEN '") + sClassNoData[0] + _T("' AND '") + sClassNoData[1] + _T("' ") ;
		}
	}
	

	CString sRegNo;
	ids = GetControlData( sCMAlias , _T("등록번호") , sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );

	if( !sRegNo.IsEmpty() )
	{
		CLocCommonAPI::MakeRegNoSpaceToZero( this , sRegNo , NULL);

		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" BB.REG_NO = '") + sRegNo +_T("'");

		nSearchField++;
	}

	if( fd != NULL ) fclose(fd);


	if( !sEcoSearchWhere.IsEmpty() )
	{
		if( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");
		if( nEcoSearchCnt == 1 )
			sWhere += _T(" IB.REC_KEY IN ") + sEcoSearchWhere;
		else
			sWhere += _T(" IB.REC_KEY IN ( ") + sEcoSearchWhere + _T(" ) ");
	}

	CString sUseLimit;
	CESL_ControlMgr* pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );

	CESL_Control_Element *pCE = pCM->FindControlElement( _T("사서제한") );
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBL_BOSHELF::SearchBO_LOC_1310()") );
	
	CButton* pUseLimit = (CButton*)(GetDlgItem( pCE->CTRL_ID ));
	if( pUseLimit == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );

	if( pUseLimit->GetCheck() == 1 )
	{
		if( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");
		
		sWhere += _T(" BB.USE_LIMIT_CODE != 'CL' ");
	}


	if( sWhere.IsEmpty() )
	{
		CString sMsg = _T("조건을 입력하시고 검색하십시오.");
		AfxMessageBox( sMsg );
		return 1;
	}

	if( nSearchField == 0 && (nMode == 0) )
	{
		CString sMsg = _T("검색 조건이 부족하여 시간이 오래 걸리거나 제대로 기능을 수행하지 못할수 있습니다. \r\n검색하시겠습니까?");
		ids = MessageBox( sMsg , _T("경고") , MB_ICONWARNING | MB_YESNO );
		if( ids == IDNO )
			return 1;
	}

	pCE = pCM->FindControlElement( _T("이용자용검색") );
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBL_BOSHELF::SearchBO_LOC_1310()") );
	
	CButton* pUserSearch = (CButton*)(GetDlgItem( pCE->CTRL_ID ));
	if( pUserSearch == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );

	if( pUserSearch->GetCheck() == 1 )
	{				
		if( !sWhere.IsEmpty() )
			sWhere += _T(" AND ");

		if( 0 == nMode )
			sWhere += _T(" BB.WORKING_STATUS IN ('BOL112N','BOL113O','BOL114O','BOL115O','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL217O','BOL218O','BOL411O','BOL511O') ");
		else if( 1 == nMode )
			sWhere += _T(" BB.WORKING_STATUS IN ('SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL317O','SEL318O','SEL411O','SEL511O') ");
	}	
	else
	{
		if( 0 == nMode )
		{
			if( !sWhere.IsEmpty() )
				sWhere += _T(" AND ");

			sWhere += _T(" BB.WORKING_STATUS NOT IN ('BOT211O','BOT212O') ");				
		}
	}


	sWhere += _T(" AND BB.REG_NO IS NOT NULL ") ;

   	CString sComboGetData2;
  	ids = GetControlData(sCMAlias, _T("관리구분"), sComboGetData2);
  	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo( CString &sWhere )") );
  	sComboGetData2.TrimRight();
  	if( sComboGetData2.IsEmpty() || (sComboGetData2.Compare(_T("적용안함"))==0) )
 	{
		if(_T("D") == m_sBookCooperativeMode)
		{
			AfxMessageBox(_T("책의 관리구분을 선택하여 주십시요."));
			return 1;
		}
 	}
 	else
 	{ 	 		
		sWhere = CString(_T("BB.MANAGE_CODE = '")) + sComboGetData2 + CString(_T("' AND ")) + sWhere;
 	}


	return 0;
}
