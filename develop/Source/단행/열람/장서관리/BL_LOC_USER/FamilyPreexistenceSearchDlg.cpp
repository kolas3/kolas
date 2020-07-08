#include "stdafx.h"
#include "FamilyPreexistenceSearchDlg.h"

#include "..\..\..\..\공통\IndexToolKit\Index.h"

#include "BO_LOC_3000Api.h"

#include "imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CFamilyPreexistenceSearchDlg::CFamilyPreexistenceSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CFamilyPreexistenceSearchDlg::IDD, pParent)
{
		
}


CFamilyPreexistenceSearchDlg::~CFamilyPreexistenceSearchDlg()
{
	
}

void CFamilyPreexistenceSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CFamilyPreexistenceSearchDlg) 
	//}}AFX_DATA_MAP		
	
}

BEGIN_MESSAGE_MAP(CFamilyPreexistenceSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CFamilyPreexistenceSearchDlg)
	ON_BN_CLICKED(IDC_BTN_USER_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDT_USER_NAME1, OnFocusedtUserName1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CFamilyPreexistenceSearchDlg::OnBtnClose() 
{
	CDialog::OnCancel();
}

void CFamilyPreexistenceSearchDlg::OnBtnSearch() 
{
	INT ids = MakeSearchWhere();
	if(1010==ids)
	{
		AfxMessageBox(_T("잘못된 주민등록번호입니다.\r\n")
					  _T("올바른 주민등록번호를 입력해주십시오"));
		GetDlgItem(IDC_EDT_CIVIL_NO)->SetFocus();
		return;
	}
	
	if( 0 == m_strWhere.GetLength() )
	{
		AfxMessageBox(_T("검색 조건을 입력해야 합니다"), MB_ICONEXCLAMATION);
		return;
	}

	CDialog::OnOK();
}

INT	CFamilyPreexistenceSearchDlg::GetSearchWhere(CString &strGetWhere)
{
	strGetWhere = m_strWhere;

	return 0;
}

INT	CFamilyPreexistenceSearchDlg::MakeSearchWhere()
{
	CIndex index;
		
	const int nCtrlCnt = 5;
	INT nID[nCtrlCnt] = {
		IDC_EDT_USER_NO1,   IDC_EDT_USER_NO2,
		IDC_EDT_USER_NAME1, IDC_EDT_USER_NAME2,
		IDC_EDT_CIVIL_NO
	};

	CString strGetData[nCtrlCnt];

	for( INT i=0; i<nCtrlCnt; i++ )
	{
		GetDlgItemText(nID[i], strGetData[i]);

		index.RemoveSpace( strGetData[i] );
		index.RemoveChar( strGetData[i], '\'' );
	}
	
	m_strWhere.Empty();
	
	if( !strGetData[0].IsEmpty() )
	{
		if( !m_strWhere.IsEmpty() )
		{
			m_strWhere += _T(" AND ");
		}

		if( strGetData[1].IsEmpty() )
		{
			strGetData[1] = strGetData[0];
		}

		m_strWhere += ( _T(" CLU.USER_NO BETWEEN '") + strGetData[0] + _T("' AND '") + strGetData[1] + _T("' "));
	}

	
	
	if( !strGetData[2].IsEmpty() )
	{
		
		strGetData[2].MakeUpper();

		if( !m_strWhere.IsEmpty() )
		{
			m_strWhere += _T(" AND ");
		}

		if( strGetData[3].IsEmpty() )
		{
			strGetData[3] = strGetData[2] + _T("힝");
		}
		else
		{
			
			strGetData[3].MakeUpper();
		}

		m_strWhere += ( _T(" CLU.IDX_USER_NAME BETWEEN '") + strGetData[2] + _T("' AND '") + strGetData[3] + _T("' "));
	}
	
	if( !strGetData[4].IsEmpty() )
	{
		strGetData[4].TrimLeft();	strGetData[4].TrimRight();
		strGetData[4].Replace( _T(" ") , _T("") );
		strGetData[4].Replace( _T("-") , _T("") );

		CString strBuf = strGetData[4];
		if( 13 <= strBuf.GetLength() )
		{
			strGetData[4] = strBuf.Mid(0,6) + _T("-") + strBuf.Mid(6);
		}

		if( !m_strWhere.IsEmpty() )
		{
			m_strWhere += _T(" AND ");
		}
		
		if(TRUE == m_pInfo->m_bLibSecuUse)
		{
			CString strData = strGetData[4];
			CString strEnc1, strEnc2;
			if(13 == strData.GetLength())
			{
				strEnc1 = strData;
				strEnc2 = strData.Left(6) + _T("-") + strData.Right(7);
			}
			else
			{
				strEnc1 = strData;
				strEnc2.Empty();
			}
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
			if(NULL != pApi)
			{
				CStringArray DataArray;
				DataArray.Add(strEnc1);
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), DataArray);
					pApi->CloseSocket();
				}
				strEnc1 = DataArray.GetAt(0);
				DataArray.RemoveAll(); DataArray.FreeExtra();
				if(!strEnc2.IsEmpty())
				{
					DataArray.Add(strEnc2);
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), DataArray);
						pApi->CloseSocket();
					}
					strEnc2 = DataArray.GetAt(0);
					DataArray.RemoveAll(); DataArray.FreeExtra();
				}	
				delete pApi;
				pApi = NULL;
			}
			CString sTmpWhere;
			if(!strEnc2.IsEmpty())
			{
				sTmpWhere.Format(_T(" CIVIL_NO IN('%s','%s') "), strEnc1, strEnc2);
			}
			else
			{
				sTmpWhere.Format(_T(" CIVIL_NO='%s' "), strEnc1);
			}
			m_strWhere += sTmpWhere;
		}
		else
		{
			m_strWhere += ( _T(" CLU.CIVIL_NO LIKE '") + strGetData[4] + _T("%' ") );
		}
	}
	
	if( !m_strWhere.IsEmpty() )
	{
		m_strWhere = _T(" (CLU.USER_CLASS!='3' OR CLU.MEMBER_CLASS!='1') AND ") + m_strWhere;
		m_strWhere += _T(" AND ( CLU.FID_KEY IS NULL ) ");
	}	

	return 0;
}

BOOL CFamilyPreexistenceSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDT_CIVIL_NO);
	
	if("Y"==m_sIsUnityLoanService)
	{
		CRect rEditCivilNo , rStaCivilNo;
		pEdit->SetLimitText(6);
		pEdit->ModifyStyle(NULL,ES_NUMBER);
		pEdit->GetWindowRect(&rEditCivilNo);
		ScreenToClient(&rEditCivilNo);
		rEditCivilNo.left += 20;
		pEdit->MoveWindow(rEditCivilNo);

	}
	else
	{
		pEdit->SetLimitText(13);
	}

	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	
	return TRUE;  
	              
}

HBRUSH CFamilyPreexistenceSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

BOOL CFamilyPreexistenceSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( WM_KEYDOWN == pMsg->message &&
		VK_RETURN  == pMsg->wParam )
	{
		OnBtnSearch();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CFamilyPreexistenceSearchDlg::OnFocusedtUserName1()
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_EDT_USER_NAME1)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_EDT_USER_NAME1)->m_hWnd,hIME); 
    }
}