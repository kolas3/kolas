// MNG_USER_MANAGER_02.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_USER_MANAGER_02.h"
#include "MNG_USER_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_MANAGER_02 dialog


CMNG_USER_MANAGER_02::CMNG_USER_MANAGER_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_USER_MANAGER_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CMNG_USER_MANAGER_02::~CMNG_USER_MANAGER_02()
{
	
}


BOOL CMNG_USER_MANAGER_02::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	

VOID CMNG_USER_MANAGER_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_USER_MANAGER_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_USER_MANAGER_02, CDialog)
	//{{AFX_MSG_MAP(CMNG_USER_MANAGER_02)
	
	ON_WM_CTLCOLOR()  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_USER_MANAGER_02 message handlers

BOOL CMNG_USER_MANAGER_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if( InitESL_Mgr( _T("MNG_USER_MANAGER_02") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error") );
		return false;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	const INT nCtrlCount = 2;
	INT nCtrlID[nCtrlCount] = {
		IDC_STATIC_MANAGECODE, IDC_COMBO_MANAGECODE
	};

	if (this->m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("S")) == 0)
	{
		for (INT i=0; i<nCtrlCount; i++)
			GetDlgItem(nCtrlID[i])->ShowWindow(SW_SHOW);
	}
	else
	{
		for (INT i=0; i<nCtrlCount; i++)
			GetDlgItem(nCtrlID[i])->ShowWindow(SW_HIDE);

		CESL_ControlMultiComboBox* pWnd = (CESL_ControlMultiComboBox*)FindControl(_T("CM_MNG_USER_MANAGER_02"), _T("관리구분"));
		ASSERT(pWnd);
		pWnd->SelectItem(this->m_pInfo->MANAGE_CODE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CMNG_USER_MANAGER_02::OnOK() 
{
	// TODO: Add extra validation here
	SearchUser();

}

VOID CMNG_USER_MANAGER_02::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

INT CMNG_USER_MANAGER_02::SearchData( CString where )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_01") );

	if( CLocCommonAPI::RefreshDataManager( this , pDM , where ) )
		return -1;

	CMNG_USER_MANAGER_01 *pParent = ( CMNG_USER_MANAGER_01 * )pParentMgr;
	pParent->ShowGrid(TRUE);
	
	return 0;
}

INT CMNG_USER_MANAGER_02::SearchUser()
{
	INT ids;
	CString sWhere;

	ids = MakeSql( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchUser") );

	ids = SearchData( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchUser") );


	return 0;
}

INT CMNG_USER_MANAGER_02::MakeSql( CString &sWhere )
{
	INT ids;

	CString sCMAlias = _T("CM_MNG_USER_MANAGER_02");
	const INT nControlCnt = 6;
	TCHAR *sControlAlias[nControlCnt] =
	{
		_T("직급") , _T("이름") , _T("ID") , _T("등급") , _T("부서") ,_T("관리구분")
	};
	TCHAR * sDBFieldName[nControlCnt] =
	{
		_T("CLASS_NAME") , _T("NAME") , _T("ID")  , _T("PERMITION") , _T("DIVISION_NAME") ,_T("MANAGE_CODE")
	};

	INT nDBFieldType[nControlCnt] = 
	{
		ONESTRING , ONELIKESTRING , ONELIKESTRING , ETC  , ONESTRING , ONELIKESTRING
	};

	CString sData[nControlCnt];
	for( INT i = 0 ; i < nControlCnt ; i++ )
	{
		ids = GetControlData( sCMAlias , sControlAlias[i] , sData[i] , -1 , 0 );
		if( ids < 0 ) 
		{
			CString sEMsg;
			sEMsg.Format( _T("ERROR_Func : GetControlData( %s , %s , sData[i] , -1 , 0 ) \r\nids : %d") , sCMAlias , sControlAlias[i] , ids);
			AfxMessageBox(sEMsg);
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeSql( CString &sWhere )") );
		sData[i].TrimLeft(); sData[i].TrimRight();
	}

	ids = CLocCommonAPI::MakeSql( sData , sDBFieldName , nDBFieldType , 6 , sWhere , this);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeSql( CString &sWhere )") );

	if( !sWhere.IsEmpty() ) sWhere += _T(" AND ");
	sWhere += _T(" ID <> 'TKATLDH'");

	if( !sData[3].IsEmpty() && (sData[3].Compare(_T("적용안함"))!=0) )
	{
		if( sData[3].Compare(_T("시스템관리자")) == 0 )
			sData[3] = _T("'A'");
		else if( sData[3].Compare(_T("사용자관리자")) == 0 )
			sData[3] = _T("'M'");
		else if( sData[3].Compare(_T("관리자")) == 0 )
			sData[3] = _T("'A','M'");
		else if( sData[3].Compare(_T("일반사용자")) == 0 )
			sData[3] = _T("'U'");
		else if( sData[3].Compare(_T("가등록자")) == 0 )
			sData[3] = _T("'N'");
		else if( sData[3].Compare(_T("통합관리자")) == 0 )
			sData[3] = _T("'S'");
		

		if( !sWhere.IsEmpty() ) sWhere += _T(" AND ");
		sWhere += CString(sDBFieldName[3]) + CString(_T(" IN ( ")) + sData[3] + CString(_T(" ) "));
		
		
	}

	return 0;
}

BOOL CMNG_USER_MANAGER_02::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN) 
		{
			SearchUser();
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CMNG_USER_MANAGER_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
