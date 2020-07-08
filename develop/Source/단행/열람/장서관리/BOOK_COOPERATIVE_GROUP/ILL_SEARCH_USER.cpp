


#include "stdafx.h"
#include "ILL_SEARCH_USER.h"
#include "ILL_SEARCH_USER_LIST.h"
#include "imm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





CILL_SEARCH_USER::CILL_SEARCH_USER(CESL_Mgr* pParent)
	: CESL_Mgr(IDD, pParent)
{
	
		
	








	{	
		m_lpszTemplateName = MAKEINTRESOURCE( IDD );
		m_nIDHelp = IDD;
	}	

	m_pLoanMangeValue = NULL;
}

CILL_SEARCH_USER::~CILL_SEARCH_USER()
{
}

VOID CILL_SEARCH_USER::DoDataExchange(CDataExchange* pDX)
{
	

	CDialog::DoDataExchange(pDX);
	
		
	

	
}


BEGIN_MESSAGE_MAP(CILL_SEARCH_USER, CDialog)
	//{{AFX_MSG_MAP(CILL_SEARCH_USER)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnFIND, OnbtnFIND)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnDETAIL, OnbtnDETAIL)
	ON_BN_CLICKED(IDC_btnCLEAR, OnbtnSEARCHCLEAR)
	ON_EN_SETFOCUS(IDC_edtFIND8, OnFocusedtFIND8)
	ON_EN_SETFOCUS(IDC_edtFIND4, OnFocusedtFIND4)
	ON_EN_SETFOCUS(IDC_edtFIND14, OnFocusedtFIND14)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CILL_SEARCH_USER::Create(CWnd* pParentWnd) 
{
	

	
	



		return CDialog::Create(IDD, pParentWnd);


	
	return FALSE;

}

VOID CILL_SEARCH_USER::OnClose() 
{
	

	
	OnCancel();

	
}

BOOL CILL_SEARCH_USER::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 


	
	if(InitESL_Mgr(_T("ILL_SEARCH_USER")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	
	((CEdit*)GetDlgItem(IDC_edtFIND1))->SetLimitText(13);


	SetWindowSize( CString(_T("간략검색")) );
	
	return TRUE;  
	              

	
	return FALSE;

}

VOID CILL_SEARCH_USER::OnbtnFIND() 
{
	

	INT ids;

	ids = SearchLoanUser();
	if( ids < 0 )
	{
		AfxMessageBox( _T("기능을 정상적으로 수행하지 못하였습니다." ) );
	}
	else
	{
		CDialog::OnCancel();
	}

	
}

INT CILL_SEARCH_USER::SearchLoanUser()
{
	
	INT ids = 0;
	CString strFuncName = _T("CILL_SEARCH_USER::SearchLoanUser()");

	CString strWhere;
	ids = GetSearchWhereSql(strWhere);
	if(0 != ids) return 1;

	CILL_SEARCH_USER_LIST Dlg(this, strWhere);
	Dlg.m_strUnityLoanCntYN = m_strUnityLoanCntYN;
	Dlg.m_pLoanMangeValue = m_pLoanMangeValue;

	Dlg.DoModal();

	
	if(-1 < Dlg.m_nCloseState)
	{
		int nIdx = Dlg.m_nCloseState;

		CESL_DataMgr* pUserDM = Dlg.FindDM(_T("DM_ILL_SEARCH_USER_LIST"));
		if(NULL == pUserDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);
		
		CESL_ControlMgr* pAplCM = FindCM(_T("CM_ILL_APL"));
		if(NULL == pUserDM) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
	
		
		
		const int GET_COUNT = 6;
		CString strDMAlias[GET_COUNT] = 
		{
			_T("대출자번호"),
			_T("이름"),
			_T("E_MAIL"),
			_T("대출직급정보_코드"),
			_T("REC_KEY"),
			_T("대출자번호_설명")
		};
		CString strEditAlias[GET_COUNT] = 
		{
			_T("이용자_이용자번호"),
			_T("이용자_이름"),
			_T("이용자_이메일"),
			_T("이용자_직급코드"),
			_T("이용자_키"),
			_T("이용자_이용자번호설명")
		};

		for(int nCnt = 0; nCnt < GET_COUNT; nCnt++)
		{
			CString strGet;
			ids = pUserDM->GetCellData(strDMAlias[nCnt], nIdx, strGet);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

			CESL_ControlEdit* pEdit = (CESL_ControlEdit*)pAplCM->FindControl(
				strEditAlias[nCnt]);
			if(NULL == pEdit) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);

			ids = pEdit->SetData(strGet);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-5, strFuncName);

		}



	}

	return 0;

	
	return -1;

}


VOID CILL_SEARCH_USER::OnbtnCLOSE() 
{
	

	OnCancel();	

	
}


INT CILL_SEARCH_USER::ViewGrid()
{
	

	INT ids = 0;




	
	
	



	



	return 0;

	
	return -1;

}









VOID CILL_SEARCH_USER::SetWindowSize( CString &sCaption )
{
	

	CRect ChangeWndPos;		
	CRect WndPos;				  
	
	INT nIDC;
	
	TCHAR* szCaption[2] = { _T("상세검색") , _T("간략검색") };

	
	if( sCaption.Compare( szCaption[0] ) == 0 )
	{
		GetDlgItem( IDC_btnDETAIL )->SetWindowText( CString(szCaption[1]) );
		nIDC = IDC_staVIEWDETAIL;
	}
	
	else
	{
		GetDlgItem( IDC_btnDETAIL )->SetWindowText( CString(szCaption[0]) );
		nIDC = IDC_staVIEWSIMPLE;
	}

	
	GetWindowRect( &WndPos );
	
	
	GetDlgItem( nIDC )->GetWindowRect( &ChangeWndPos );
	
	ClientToScreen(&WndPos);
	ClientToScreen(&ChangeWndPos);

	
	WndPos.SetRect( WndPos.left , WndPos.top , WndPos.right , ChangeWndPos.bottom );
	
	ScreenToClient(&WndPos);

	MoveWindow( WndPos );	

	
}

VOID CILL_SEARCH_USER::OnbtnDETAIL() 
{
	

	CString sCaption;
	GetDlgItem( IDC_btnDETAIL )->GetWindowText( sCaption );
	SetWindowSize( sCaption );		

	
}

BOOL CILL_SEARCH_USER::PreTranslateMessage(MSG* pMsg) 
{
	

	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
	{
		OnCancel();
		return TRUE;
	}
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		OnbtnFIND();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);

	
	return FALSE;

}

VOID CILL_SEARCH_USER::OnbtnSEARCHCLEAR() 
{
	

	CString sCMAlias = _T("CM_ILL_SEARCH_USER");
	
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnCLEAR") );

	pCM->AllControlClear();

	((CComboBox*)GetDlgItem(IDC_cmbFIND3))->SetCurSel(0);

	
}


VOID CILL_SEARCH_USER::OnFocusedtFIND8()
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtFIND8)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_ALPHANUMERIC;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtFIND8)->m_hWnd,hIME); 
    }
}



VOID CILL_SEARCH_USER::OnFocusedtFIND4()
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtFIND4)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtFIND4)->m_hWnd,hIME); 
    }
}



VOID CILL_SEARCH_USER::OnFocusedtFIND14()
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtFIND14)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtFIND14)->m_hWnd,hIME); 
    }
}

INT CILL_SEARCH_USER::GetSearchWhereSql( CString &sWhere )
{
	

	INT ids;

	
	
	INT nSFieldCnt = 0;

	CString sCMAlias = _T("CM_ILL_SEARCH_USER");
	
	sWhere.Empty();

	const INT nControlCnt = 34;
	TCHAR *sControlAlias[nControlCnt] =
	{
		_T("소속")			, _T("직급")			, _T("회원상태")		, _T("주민등록번호")	, _T("대출자번호1") , 
		_T("대출자번호2")	, _T("대출자명1")		, _T("대출자명2")		, _T("등록일자1")		, _T("등록일자2") , 
		_T("최종대출일자1") , _T("최종대출일자2")	, _T("누적연체일1")		, _T("누적연체일2")		, _T("미대출기간1") , 
		_T("미대출기간2")	, _T("대출정지일자1")	, _T("대출정지일자2")	, _T("누적연체횟수1")	, _T("누적연체횟수2") , 
		_T("E_MAIL")		, _T("대출횟수")		, _T("대출횟수이상이하"), _T("가족ID")			, _T("핸드폰"),
		_T("집전화")		, _T("교번1")			, _T("교번2")			, _T("군별")			, _T("대출자생일1") , 
		_T("대출자생일2")	, _T("자료실")			, _T("회원관리구분")	, _T("집주소")
	};
	
	CString sGetData[ nControlCnt ];

 	
 	

 	for( INT i = 0 ; i < nControlCnt ; i++ )
 	{
 		ids = GetControlData( sCMAlias , sControlAlias[i] , sGetData[i] , -1 , 0 );
 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("BL_LOC_USER_PROC::GetSearchWhereSql( CString &sWhere )") );
 		sGetData[i].TrimRight();
 		
 		
 		
 		


 		
 	}
 	
	
	
	
	sGetData[ 24 ].TrimLeft();
	sGetData[ 24 ].TrimRight();
	sGetData[ 24 ].Replace( _T(" "), _T("") );
	sGetData[ 24 ].Replace( _T("-"), _T("") );

	
	if( !sGetData[ 24 ].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
			sWhere += _T(" AND ");

		
			sWhere += _T("IDX_HANDPHONE LIKE '%") + sGetData[ 24 ] + _T("%'");
		
	}	
	
	
	if( !sGetData[4].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[5].IsEmpty() )
			sWhere += _T(" USER_NO BETWEEN '") + sGetData[4] + _T("' AND '") + sGetData[5]+ _T("' ");
		else
			sWhere += _T(" USER_NO = '") + sGetData[4] + _T("' ");

		nSFieldCnt++;
	}

	
	if( !sGetData[6].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[7].IsEmpty() )
			sWhere += _T(" IDX_USER_NAME BETWEEN '") + sGetData[6] + _T("' AND '") + sGetData[7]+ _T("' ");
		else
			
			
			sWhere += _T(" IDX_USER_NAME BETWEEN '") + sGetData[6] + _T("' AND '") + sGetData[6]+ _T("힝' ");
			
		nSFieldCnt++;
	}

	if ( !sGetData[26].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[27].IsEmpty() )
			sWhere += _T(" CLASS_NO BETWEEN '") + sGetData[26] + _T("' AND '") + sGetData[27]+ _T("' ");
		else
			sWhere += _T(" CLASS_NO LIKE '") + sGetData[26] + _T("%' ");			

		nSFieldCnt++;
	}

	
	CString sModeAlias;
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBL_BOSHELF::GetSqlBO_LOC_1110( CString &sWhere )") );
	CESL_Control_Element *pCE = pCM->FindControlElement( _T("윈도우모드") );
	if( pCE == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBL_BOSHELF::GetSqlBO_LOC_1110( CString &sWhere )") );
	(GetDlgItem( pCE->CTRL_ID ))->GetWindowText( sModeAlias );
	
	if( sModeAlias.Compare( _T("상세검색") ) == 0 )
	{
		if( nSFieldCnt == 0 )
		{
			ids = MessageBox(_T("검색 조건이 입력되지 않아 검색 시간이 오래 걸릴 수 있습니다.\r\n")
													 _T("무시하고 검색하시겠습니까? \r\n( 예 : 검색 , 아니오 : 검색 중지 ) ")
												   , _T("이용자 검색 경고") , MB_ICONWARNING | MB_YESNO );
			if( ids == IDNO )
				return 1;
		}

		return 0;
	}

	
	
	sGetData[ 25 ].TrimLeft();
	sGetData[ 25 ].TrimRight();
	if( !sGetData[ 25 ].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
		sWhere += _T(" AND ");

		
			sWhere += _T("H_PHONE LIKE '%") + sGetData[ 25 ] + _T("%'");
		
	}

	
	
	sGetData[ 33 ].TrimLeft();
	sGetData[ 33 ].TrimRight();
	if( !sGetData[ 33 ].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 )
		sWhere += _T(" AND ");

		
			sWhere += _T("H_ADDR1 LIKE '%") + sGetData[ 33 ] + _T("%'");
		

		nSFieldCnt++;
	}
	

	
	sGetData[0].TrimRight();
	if( sGetData[0].Compare(_T("0")) != 0 && !sGetData[0].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" USER_POSITION_CODE = '") + sGetData[0] + _T("' ");
	}

	
	
	sGetData[28].TrimRight();
	if( sGetData[28].Compare(_T("0")) != 0 && !sGetData[28].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" USER_DEFINE_CODE1 = '") + sGetData[28] + _T("' ");
	}

	
	
	











	
	if( sGetData[1].Compare(_T("0")) != 0 && !sGetData[1].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" USER_CLASS_CODE = '") + sGetData[1] + _T("' ");
	}

	
	
	if( sGetData[32].Compare(_T("0")) != 0 && !sGetData[32].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" MANAGE_CODE = '") + sGetData[32] + _T("' ");
	}
	

	
	if( sGetData[2].Compare(_T("0")) != 0 && !sGetData[2].IsEmpty() && (sGetData[2].Compare(_T("적용안함"))) != 0)
	{
		CString sCode;
		if( sGetData[2].Compare(_T("정상")) == 0 || (sGetData[2].Compare(_T("정상회원")) == 0) ) sCode = _T("0");
		else if( sGetData[2].Compare(_T("대출정지")) == 0 ) sCode = _T("1");
		else if( sGetData[2].Compare(_T("제적")) == 0 || (sGetData[2].Compare(_T("제적회원")) == 0) ) sCode = _T("2");
		
		
		else if( sGetData[2].Compare(_T("탈퇴")) == 0 || (sGetData[2].Compare(_T("탈퇴회원")) == 0) ) sCode = _T("3");
		
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" USER_CLASS = '") + sCode + _T("' ");
	}
	

	
	
	if( !sGetData[8].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[9].IsEmpty() )
			sWhere += _T(" to_char(REG_DATE,'YYYY/MM/DD') BETWEEN '") + sGetData[8] + _T("' AND '") + sGetData[9]+ _T("' ");
		else
			sWhere += _T(" to_char(REG_DATE,'YYYY/MM/DD') = '") + sGetData[8] + _T("' ");

		nSFieldCnt++;
	}

	
	if( !sGetData[10].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[11].IsEmpty() )
			sWhere += _T(" to_char(LAST_LOAN_DATE,'YYYY/MM/DD') BETWEEN '") + sGetData[10] + _T("' AND '") + sGetData[11]+ _T("' ");
		else
			sWhere += _T(" to_char(LAST_LOAN_DATE,'YYYY/MM/DD') = '") + sGetData[10] + _T("' ");

		nSFieldCnt++;
	}

	
	
	if( !sGetData[18].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[19].IsEmpty() )
			sWhere += _T(" DELAY_COUNT BETWEEN ") + sGetData[18] + _T(" AND ") + sGetData[19]+ _T(" ");
		else
			sWhere += _T(" DELAY_COUNT = ") + sGetData[18] + _T(" ");

		nSFieldCnt++;
		
	}
	
	
	if( !sGetData[12].IsEmpty() )
	{

		CString sColumnName = _T("DELAYDAY_COUNT");
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[13].IsEmpty() )
			sWhere += sColumnName +
					CString(_T(" BETWEEN ")) + sGetData[12] + _T(" AND ") + sGetData[13]+ _T(" ");
		else
			sWhere += sColumnName + _T(" = ") + sGetData[12] + _T(" ");

		nSFieldCnt++;
		
	}

	
	if( !sGetData[20].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

		
			sWhere += _T(" E_MAIL LIKE '") + sGetData[20] + _T("%%' ");
		

		nSFieldCnt++;
		
	}
	
	
	
	
	INT nLoanCnt = _ttoi(sGetData[21]);	
	if( sGetData[22].Compare(_T("이상")) == 0 )	
		sGetData[22] = _T(">=");
	else
		sGetData[22] = _T("<=");

	if( !sGetData[14].IsEmpty() )
	{
		if( nLoanCnt == 0 )
		{
			sGetData[21] = _T("0");
			sGetData[22] = _T("=");
		}

		CString sDefaultSql = _T(" ( (SELECT COUNT(*) FROM LH_STORAGE_V01 CL WHERE CL.STATUS IN ('0','1','2') AND CL.USER_KEY=CLU.REC_KEY ");
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[15].IsEmpty() )
			sWhere += sDefaultSql + _T(" AND CL.LOAN_DATE BETWEEN to_date('")+sGetData[14]+_T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")+sGetData[15]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ) ") + sGetData[22] + sGetData[21] + _T(") ");
		else
			sWhere += sDefaultSql + _T(" AND CL.LOAN_DATE =to_date('")+sGetData[14]+_T(" 00:00:00') AND to_date('")+sGetData[14]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') )") + sGetData[22] + sGetData[21] + _T(") ");

		
		
		
		
		if( !sGetData[15].IsEmpty() )
		{
			sWhere += _T(" AND CLU.REG_DATE <= to_date('") + sGetData[15] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}
		else
		{
			sWhere += _T(" AND CLU.REG_DATE <= to_date('") + sGetData[14] + _T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}
		

		nSFieldCnt++;
		
	}
	else
	{
		if( !sGetData[21].IsEmpty() )
		{
			CString sDefaultSql = _T(" ( (SELECT COUNT(*) FROM LH_STORAGE_V01 CL WHERE CL.STATUS IN ('0','1','2') AND CL.USER_KEY=CLU.REC_KEY ");
			if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
			if( !sGetData[15].IsEmpty() )
				sWhere += sDefaultSql + _T(")") + sGetData[22] + sGetData[21] + _T(") ");
			else
				sWhere += sDefaultSql + _T(")") + sGetData[22] + sGetData[21] + _T(") ");

			nSFieldCnt++;
		}
	}

	

	
	if( !sGetData[16].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		if( !sGetData[17].IsEmpty() )
			sWhere += _T(" to_char(LOAN_STOP_DATE,'YYYY/MM/DD') BETWEEN '") + sGetData[16] + _T("' AND '") + sGetData[17]+ _T("' ");
		else
			sWhere += _T(" to_char(LOAN_STOP_DATE,'YYYY/MM/DD') = '") + sGetData[16] + _T("' ");

		nSFieldCnt++;
		
	}

	
	CString sCenterWhere;
	const INT nCenterCondCnt = 3;

	CString sCenterDBFieldName = _T("CLU.CENTER_UPLOAD_CHECK");

	CString sCenterControlAlias[nCenterCondCnt] = { _T("적용한대출자") , _T("미적용대출자") , _T("실패한대출자") };
	CString sCenterValue[nCenterCondCnt] = { _T("Y") , _T("N") , _T("F") };
	
	CButton* pCheckBox;
	for(i = 0 ; i < nCenterCondCnt ; i++ )
	{
		pCE = (CESL_Control_Element*)pCM->FindControlElement(sCenterControlAlias[i]);
		pCheckBox = (CButton*)(GetDlgItem(pCE->CTRL_ID));
		
	
		if( pCheckBox->GetCheck() == 1 )
		{
			if( !sCenterWhere.IsEmpty() ) sCenterWhere += _T(" OR ");
			sCenterWhere += sCenterDBFieldName + _T(" = '") + sCenterValue[i] + _T("'");
		}
	}

	if( !sCenterWhere.IsEmpty()  )
	{
		if( !sWhere.IsEmpty() ) sWhere += _T(" AND ");
		sWhere += _T("( ") + sCenterWhere + _T(" )");
	}

	
	if( !sGetData[23].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
		sWhere += _T(" (SELECT  MAX(ID) FROM CO_LOAN_FAMILY_ID_TBL CLFI WHERE CLFI.REC_KEY = CLU.FID_KEY ) LIKE ('") + sGetData[23] + _T("%') ");
		nSFieldCnt++;
	}

	
	if( !sGetData[29].IsEmpty() )
	{
		if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");

		if( !sGetData[30].IsEmpty() )
		{
			sWhere += _T(" CLU.BIRTHDAY BETWEEN '") + sGetData[29] + _T("' AND '") + sGetData[30] + _T("' ");
		}
		else
		{
			sWhere += _T(" CLU.BIRTHDAY='") + sGetData[29] +_T("' ");
		}
		nSFieldCnt++;
	}
	

	if( nSFieldCnt == 0 )
	{
		ids = MessageBox(_T("검색 조건이 부족하여 검색 시간이 오래 걸릴 수 있습니다.\r\n")
			_T("무시하고 검색하시겠습니까? \r\n( 예 : 검색 , 아니오 : 검색 중지 ) ")
			, _T("이용자 검색 경고") , MB_ICONWARNING | MB_YESNO );
		if( ids == IDNO )
			return 1;
	}
	
	return 0;

	
	return -1;

}
