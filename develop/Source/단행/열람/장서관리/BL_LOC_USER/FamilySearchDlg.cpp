#include "stdafx.h"
#include "FamilySearchDlg.h"

#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFamilySearchDlg::CFamilySearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
		m_sFamilyID = _T("");
		m_nSelectIndex = 0;
		m_nSelectIndexFamilyList = 0;
		m_nCloseState = 0;
}


CFamilySearchDlg::~CFamilySearchDlg()
{
	
}
VOID CFamilySearchDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFamilySearchDlg) 
	//}}AFX_DATA_MAP
		
	EFS_END
}
BEGIN_MESSAGE_MAP(CFamilySearchDlg, CDialog)
	//{{AFX_MSG_MAP(CFamilySearchDlg)
	ON_BN_CLICKED(IDC_btnFAMILY_SEARCH, OnbtnFAMILYSEARCH)
	ON_BN_CLICKED(IDC_btnCREATE_FAMILY_ID, OnbtnCREATEFAMILYID)
	ON_BN_CLICKED(IDC_btnFAMILY_SELECT, OnbtnFAMILYSELECT)
	ON_BN_CLICKED(IDC_btnFAMILY_CLOSE, OnbtnFAMILYCLOSE)
	ON_BN_CLICKED(IDC_bDELETE_FAMILY_ID, OnbDELETEFAMILYID)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CFamilySearchDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	if(InitESL_Mgr(_T("FAMILY_ID")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_nCloseState = 1;
	SetFamilyID();

	INT ids = SearchFamilyID();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CFamilySearchDlg::OnInitDialog() ") );

	return TRUE;  
	              

	EFS_END
	return FALSE;

}

INT CFamilySearchDlg::SearchFamilyID()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_FAMILY_ID");
	CString sCMAlias = _T("CM_FAMILY_ID");

	CString sFamilyID;
	ids = GetControlData( sCMAlias , _T("가족ID") , sFamilyID );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CFamilySearchDlg::SearchFamilyID() ") );

	CString sWhere;
	if( !sFamilyID.IsEmpty() )
		sWhere.Format( _T("ID like '%s%%'") , sFamilyID );
	else
		return 0;
	
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CFamilySearchDlg::SearchFamilyID() ") );


	ids = CLocCommonAPI::RefreshDataManager( this , pDM , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -3 , _T("CFamilySearchDlg::SearchFamilyID() ") );

	ids = AllControlDisplay( sCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -4 , _T("CFamilySearchDlg::SearchFamilyID() ") );

	if( pDM->GetRowCount() == 0 )
	{
		AfxMessageBox(_T("검색 결과 조건과 일치하는 ID를 찾지 못했습니다."));
		return 0;
	}

	ids = DisplayFamilyList( 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -5 , _T("CFamilySearchDlg::SearchFamilyID() ") );

	return 0;

	EFS_END
	return -1;

}

INT CFamilySearchDlg::DisplayFamilyList( INT nRowIndex )
{
EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_FAMILY_ID");
	
	CString sFamilyRecKey;
	ids = GetDataMgrData( sDMAlias , _T("가족KEY") , sFamilyRecKey , nRowIndex );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("DisplayFamilyList") );

	sFamilyRecKey.TrimRight(); sFamilyRecKey.TrimLeft();

	if( sFamilyRecKey.IsEmpty() ) return 0;

	CString sWhere;
	sWhere.Format( _T("FID_KEY = %s") , sFamilyRecKey );

	CString sFamilyCMAlias = _T("CM_FAMILY_LIST_DM");
	CString sFamilyDMAlias = _T("DM_FAMILY_LIST_DM");
	CESL_DataMgr *pFamilyListDM = FindDM( sFamilyDMAlias );
	if( NULL == pFamilyListDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("DisplayFamilyList") );


	ids = CLocCommonAPI::RefreshDataManager( this , pFamilyListDM , sWhere );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("DisplayFamilyList") );

	
	CStringArray asDes1,asDes2,asDes3;

	
	CString strValue;
	ids = GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("DisplayFamilyList") );
	strValue.TrimLeft(); strValue.TrimRight(); strValue.MakeUpper();
	INT nRowCnt = pFamilyListDM->GetRowCount();
	if(_T("Y") != strValue || TRUE == m_pInfo->m_bLibSecuUse)
	{
		for(INT i = 0; i < nRowCnt; i++)
		{
			if(_T("Y") != strValue)
			{
				CString strUserNo;
				ids = pFamilyListDM->GetCellData( _T("대출자번호"), i , strUserNo );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("DisplayFamilyList") );
				CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_sIsUnityLoanService );
				ids = pFamilyListDM->SetCellData( _T("대출자번호"), strUserNo , i );
				if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("DisplayFamilyList") );
			}
			
			if(TRUE == m_pInfo->m_bLibSecuUse)
			{
				CString strGetData;
				pFamilyListDM->GetCellData(_T("전화번호"), i, strGetData);
				asDes1.Add(strGetData);
				pFamilyListDM->GetCellData(_T("주소(집)"), i, strGetData);
				asDes2.Add(strGetData);
				pFamilyListDM->GetCellData(_T("전화(집)"), i, strGetData);
				asDes3.Add(strGetData);
			}
		}
	}
	
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < nRowCnt)
	{
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->CheckConnection())
			{
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes1);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes2);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes3);
					p3000Api->CloseSocket();	
				}
				for(INT i = 0; i < nRowCnt; i++)
				{
					pFamilyListDM->SetCellData(_T("전화번호"), asDes1.GetAt(i), i);
					pFamilyListDM->SetCellData(_T("주소(집)"), asDes2.GetAt(i), i);
					pFamilyListDM->SetCellData(_T("전화(집)"), asDes3.GetAt(i), i);
				}
			}
			delete p3000Api;
			p3000Api = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
	}
	
	ids = AllControlDisplay( sFamilyCMAlias );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("DisplayFamilyList") );

	return 0;

EFS_END
return -1;
}

VOID CFamilySearchDlg::SetFamilyID()
{
	EFS_BEGIN

	INT ids;

	CString sCMAlias = _T("CM_FAMILY_ID");
	ids = SetControlData( sCMAlias , _T("가족ID") , m_sFamilyID );
	if( ids < 0 ) return;	

	EFS_END
}

VOID CFamilySearchDlg::SetFamilyID( CString sFamilyID )
{
	EFS_BEGIN

	m_sFamilyID = sFamilyID;

	EFS_END
}

INT CFamilySearchDlg::GetFamilyID( CString &sFamilyID , CString &sFamilyRecKey )
{
	EFS_BEGIN

	INT ids;
	sFamilyID.Empty();
	sFamilyRecKey.Empty();

	CString sDMAlias = _T("DM_FAMILY_ID");

	ids = GetDataMgrData( sDMAlias , _T("가족KEY") ,sFamilyRecKey ,  m_nSelectIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CFamilySearchDlg::GetFamilyID( CString &sFamilyID , CString &sFamilyRecKey )") );

	ids = GetDataMgrData( sDMAlias , _T("가족ID") , sFamilyID , m_nSelectIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CFamilySearchDlg::GetFamilyID( CString &sFamilyID , CString &sFamilyRecKey )") );

	return 0;

	EFS_END
	return -1;

}

INT CFamilySearchDlg::GetAddressHome( CString &sZipCodeHome , CString &sAddressHome, CString &sPhoneHome)
{
	EFS_BEGIN

	INT ids;
	sZipCodeHome.Empty();
	sAddressHome.Empty();
	sPhoneHome.Empty();

	CString sDMAlias = _T("DM_FAMILY_LIST_DM");

	ids = GetDataMgrData( sDMAlias , _T("우편번호(집)") ,sZipCodeHome ,  m_nSelectIndexFamilyList );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CFamilySearchDlg::GetAddressHome( CString &sZipCodeHome , CString &sAddressHome, CString &sPhoneHome)") );

	ids = GetDataMgrData( sDMAlias , _T("주소(집)") , sAddressHome , m_nSelectIndexFamilyList );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CFamilySearchDlg::GetAddressHome( CString &sZipCodeHome , CString &sAddressHome, CString &sPhoneHome)") );

	ids = GetDataMgrData( sDMAlias , _T("전화(집)") , sPhoneHome , m_nSelectIndexFamilyList );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CFamilySearchDlg::GetAddressHome( CString &sZipCodeHome , CString &sAddressHome, CString &sPhoneHome)") );

	return 0;

	EFS_END
	return -1;

}


BEGIN_EVENTSINK_MAP(CFamilySearchDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CFamilySearchDlg)
	ON_EVENT(CFamilySearchDlg, IDC_MAINGRID, -600 /* Click */, OnClickMaingrid, VTS_NONE)
	ON_EVENT(CFamilySearchDlg, IDC_MAINGRID, -601 /* DblClick */, OnDblClickMaingrid, VTS_NONE)
	ON_EVENT(CFamilySearchDlg, IDC_MAINGRID, -605 /* MouseDown */, OnMouseDownMaingrid, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CFamilySearchDlg::OnClickMaingrid() 
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_FAMILY_ID");
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) return ;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("FAMILY_GRID") ));
	if( pGrid == NULL ) return;

	INT nCurrentIdx = pGrid->GetIdx();

	pGrid->SetReverse( nCurrentIdx );
	
	DisplayFamilyList( nCurrentIdx );


	EFS_END
}


INT CFamilySearchDlg::GetCloseState()
{
	EFS_BEGIN

	if( m_nCloseState == 1 ) return 1;
	return 0;

	EFS_END
	return -1;

}

VOID CFamilySearchDlg::OnDblClickMaingrid() 
{
	EFS_BEGIN


	m_nSelectIndex = GetCurrentSelectIndex();
	if( m_nSelectIndex < 0 ) return;

	
	m_nSelectIndexFamilyList = GetCurrentSelectIndexFamilyList();
	if( m_nSelectIndexFamilyList < 0 ) m_nSelectIndexFamilyList = 0;

	m_nCloseState = 0;	

	OnCancel();

	EFS_END
}

INT CFamilySearchDlg::GetCurrentSelectIndex()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_FAMILY_ID");
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CFamilySearchDlg::GetCurrentSelectIndex()") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("FAMILY_GRID") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CFamilySearchDlg::GetCurrentSelectIndex()") );

	INT nCurrentIdx = -1;
	nCurrentIdx = pGrid->GetIdx();

	return nCurrentIdx;

	EFS_END
	return -1;

}

INT CFamilySearchDlg::GetCurrentSelectIndexFamilyList()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_FAMILY_LIST_DM");
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -1 , _T("CFamilySearchDlg::GetCurrentSelectIndexFamilyList()") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("가족회원리스트") ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(  -2 , _T("CFamilySearchDlg::GetCurrentSelectIndexFamilyList()") );

	INT nCurrentIdx = -1;
	nCurrentIdx = pGrid->GetIdx();

	return nCurrentIdx;

	EFS_END
	return -1;

}

VOID CFamilySearchDlg::OnbtnFAMILYSEARCH() 
{
	EFS_BEGIN

	INT ids = SearchFamilyID();
	if( ids ) return;

	EFS_END
}

VOID CFamilySearchDlg::OnbtnCREATEFAMILYID() 
{
	EFS_BEGIN

	INT ids;

	CString sNewFamilyID , sNewFamilyIDTemp;
	GetDlgItem( IDC_edtFAMILY_ID )->GetWindowText( sNewFamilyID );
	if( sNewFamilyID.IsEmpty() )
	{
		AfxMessageBox( _T("ID를 입력해주십시오"));
		return ;
	}

	sNewFamilyIDTemp = sNewFamilyID;

	CString sDMAlias = _T("DM_FAMILY_ID");
	CESL_DataMgr *pDM = FindDM( sDMAlias );

	
	
	CString query, result;

	sNewFamilyID.TrimLeft();
	sNewFamilyID.TrimRight();
	query.Format( _T("SELECT COUNT( * ) FROM CO_LOAN_FAMILY_ID_TBL WHERE ID = '%s'"), sNewFamilyID );
	pDM->GetOneValueQuery( query, result );

	if( _ttoi( result ) > 0 )
	{
		MessageBox( _T("중복되는 가족ID가 있습니다.\n다시 입력하여 주십시요"), _T("ID 중복조사"),MB_ICONSTOP );
		return;
	}

	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( this->m_pInfo->MODE );
	MobileApi.SetDM( pDM );
	MobileApi.StartLog();

	CString sNewRecKey;

	ids = MobileApi.MakeRecKey( sNewRecKey );
	if( ids < 0 ) return ;

	pDM->InitDBFieldData();


	ids = pDM->AddDBFieldData( _T("ID") , _T("STRING") , sNewFamilyID );
	if( ids < 0 ) return ;

	pDM->StartFrame();

	ids = MobileApi.MakeInsertFrame( _T("CO_LOAN_FAMILY_ID_TBL") , _T("REC_KEY")  , _T("NUMERIC") , sNewRecKey );
	if( ids < 0 ) return ;

	ids = MobileApi.SendFrame();
	if( ids < 0 ) return ;
	pDM->EndFrame();
	MobileApi.EndLog();

	ids = pDM->InsertRow(-1);
	if( ids < 0 ) return ;
	
	
	ids = pDM->SetCellData( _T("가족ID") , sNewFamilyIDTemp , pDM->GetRowCount()-1 );
	if( ids < 0 ) return ;
	ids = pDM->SetCellData( _T("가족KEY") , sNewRecKey , pDM->GetRowCount()-1 );
	if( ids < 0 ) return ;

	AllControlDisplay( _T("CM_FAMILY_ID") );
	return ;

	EFS_END
}

VOID CFamilySearchDlg::OnbtnFAMILYSELECT() 
{
	EFS_BEGIN

	m_nSelectIndex = GetCurrentSelectIndex();
	if( m_nSelectIndex < 0 )
	{
		AfxMessageBox(_T("가족 ID를 먼저 선택해주십시오"));
		return ;
	}

	
	m_nSelectIndexFamilyList = GetCurrentSelectIndexFamilyList();
	if( m_nSelectIndexFamilyList < 0 ) m_nSelectIndexFamilyList = 0;

	m_nCloseState = 0;	
	OnCancel();

	EFS_END
}

VOID CFamilySearchDlg::OnbtnFAMILYCLOSE() 
{
	EFS_BEGIN

	m_nCloseState = 1;
	OnCancel();	

	EFS_END
}

VOID CFamilySearchDlg::OnMouseDownMaingrid(short Button, short Shift, long x, long y) 
{
	

}

VOID CFamilySearchDlg::OnbDELETEFAMILYID() 
{
	
	
	DeleteFamilyID();
}

VOID CFamilySearchDlg::DeleteFamilyID()
{
	CESL_ControlMgr *pCM = FindCM( _T("CM_FAMILY_ID") );
	CESL_Grid *pGrid = ( CESL_Grid * )( pCM->FindControl( _T("FAMILY_GRID") ) );
	CESL_DataMgr *pDM = FindDM( _T("DM_FAMILY_ID") );
	
	CArray< INT, INT >checkedRow;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
		if( _T("V") == pGrid->GetTextMatrix( i + 1, 1 ) )
			checkedRow.Add( i );

	if( !checkedRow.GetSize() )
	{
		
		
		AfxMessageBox( _T("삭제할 가족ID를 선정하여 주십시요") );
		return;
	}

	CString notice;
	notice.Format( _T("선택된 %d개의 가족ID를 삭제하시겠습니까?"), checkedRow.GetSize() );
	if( IDNO == AfxMessageBox( notice, MB_YESNO ) )
		return;

	pDM->StartFrame();
	pDM->InitDBFieldData();	
	
	CString query, familyKey;
	for( i = checkedRow.GetSize() - 1; i > -1; i-- )
	{
		familyKey = pDM->GetCellData( _T("가족KEY"), checkedRow.GetAt( i ) );
		query.Format( 
				_T("DELETE FROM CO_LOAN_FAMILY_ID_TBL WHERE REC_KEY = %s;")
				_T("UPDATE CO_LOAN_USER_TBL SET FID_KEY = '' WHERE FID_KEY = %s;"), 
				familyKey, familyKey );
		pDM->AddFrame( query );

		pDM->DeleteRow( checkedRow.GetAt( i ) );

		
		
		if( !m_sFamilyID.IsEmpty() )
		{
			CString sFamilyRecKey;
			pParentMgr->GetControlData( _T("CM_BO_LOC_3320"), _T("가족KEY"), sFamilyRecKey );

			if( sFamilyRecKey = familyKey )
			{
				m_sFamilyID = _T("");
				pParentMgr->SetControlData( _T("CM_BO_LOC_3320") , _T("가족ID") , _T("") );
				pParentMgr->SetControlData( _T("CM_BO_LOC_3320") , _T("가족KEY") , _T("") );
			}		
		}
	}

	pDM->SendFrame();
	pDM->EndFrame();

	AfxMessageBox( _T("가족ID가 삭제되었습니다.") );
	pGrid->Display();
}

HBRUSH CFamilySearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

BOOL CFamilySearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( ( WM_KEYDOWN == pMsg->message ) && 
		( VK_RETURN  == pMsg->wParam )  &&
		( GetDlgItem(IDC_edt_FAMILY_ID_SEARCH)->GetSafeHwnd() == pMsg->hwnd) )
	{
		OnbtnFAMILYSEARCH();
		return TRUE;
	}

	return CESL_Mgr::PreTranslateMessage(pMsg);
}

