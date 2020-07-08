// BO_LOC_38501.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_38501.h"
#include "BO_LOC_3852.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3850 dialog


CBO_LOC_3850::CBO_LOC_3850(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3850)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CBO_LOC_3850::~CBO_LOC_3850()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CBO_LOC_3850::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3850)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3850, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3850)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3850 message handlers

BOOL CBO_LOC_3850::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr( _T("BO_LOC_3850") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	SearchAllInfo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}




VOID CBO_LOC_3850::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN


	CDialog::OnSize(nType, cx, cy);
	
	if( GetSafeHwnd() == NULL ) return;

	CWnd* pWnd = GetDlgItem(IDC_grid3850_Main);

	if( pWnd == NULL ) return ;

	if( pWnd->GetSafeHwnd() == NULL ) return;

	pWnd->MoveWindow( 0 , 0 , cx , cy );
	


EFS_END
}


BOOL CBO_LOC_3850::Create(CWnd* pParentWnd) 
{
EFS_BEGIN


	return CDialog::Create(IDD, pParentWnd);


EFS_END
return FALSE;

}



INT CBO_LOC_3850::SearchAllInfo()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3850") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchAllInfo") );

	ids = pDM->RefreshDataManager(_T(""));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchAllInfo") ); 

	ids = AllControlDisplay( _T("CM_BO_LOC_3850") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SearchAllInfo") ); 


	return 0;

EFS_END
return -1;

}

BEGIN_EVENTSINK_MAP(CBO_LOC_3850, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_3850)
	ON_EVENT(CBO_LOC_3850, IDC_grid3850_Main, -601 /* DblClick */, OnDblClickgrid3850Main, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_3850::OnDblClickgrid3850Main() 
{
EFS_BEGIN

	
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3850") , _T("메인그리드") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnDblClickgrid3850Main") );

	if( pGrid->GetMouseRow() < 1 ) return;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3850") );
	if( pDM == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("SearchAllInfo") );

	if( pDM->GetRowCount() < 1 ) return ;

	pGrid->SetAt( pGrid->GetIdx() , 0 , _T("V") );

	CBO_LOC_3852 Dlg(this);
	Dlg.SetDlgType( __ECO_MODIFY_DBCLICK_MODE , _T("CM_BO_LOC_3850") , _T("메인그리드") , _T("DM_BO_LOC_3850") );
	Dlg.DoModal();
	

EFS_END
}

INT CBO_LOC_3850::DeleteGroup()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3850") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DeleteGroup") );

	if( pDM->GetRowCount() < 1 ) return 1;

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3850") , _T("메인그리드") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DeleteGroup") );

	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt < 1 )
	{
		AfxMessageBox( _T("삭제할 목록을 선정해주십시오.") );
		return 2;
	}


	// 선정된걸 삭제하기
	CArray <INT,INT> aDIndexs;
	CString sCode;
	CString sRecKey;

	pGrid->SelectGetTailPosition();

	pDM->StartFrame();

	INT nIndex;
	INT nDCnt=0;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		
		ids = pDM->GetCellData( _T("코드") , nIndex , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("DeleteGroup") );
		sCode.TrimLeft(); sCode.TrimRight();

		if( sCode.IsEmpty() )
		{
			AfxMessageBox( _T("적용안함 그룹은 삭제할수 없습니다.") );
			pGrid->SelectGetPrev();

			continue;
		}

		// 2005.10.13 ADD BY PDW(Jr)/////////////////////////////////////////////
		// 해당 코드의 회원 리스트가 존재한다면 회원 목록을 삭제 한다.
		/////////////////////////////////////////////////////////////////////////
		CString query = _T(""), result=_T(""), cnt=_T("");

		/////////////////////////////////////////////////////////////////////////
		// 1. 대출상태의 여부에 따라 참조 테이블이 달라진다.
		/////////////////////////////////////////////////////////////////////////														
		query.Format(_T("select loan_user_yn from mn_loan_user_group_tbl where code = '%s' "), sCode);
		pDM->GetOneValueQuery(query, result);
		result.TrimLeft();result.TrimRight();

		if (result == _T("Y"))
			query.Format(_T("select count(*) from mn_loan_user_group_list_tbl where group_code ='%s'"), sCode);
		else
			query.Format(_T("select count(*) from co_nonmember_tbl where group_code ='%s'"), sCode);

		pDM->GetOneValueQuery(query, cnt);
		if ( _ttoi(cnt) > 0 )						// 리스트가 존재한다.
		{
			CString mng;
			mng.Format(_T("선택된 그룹은 회원목록을 가지고 있습니다.\r\n")
				       _T("그룹을 삭제시 그룹의 회원목록도 삭제가 됩니다.\r\n")
					   _T("계속 진행하시겠습니까?"));
			if ( IDYES == AfxMessageBox(mng, MB_YESNO) )	// 삭제 진행
			{
				/////////////////////////////////////////////////////////////////////////
				// 2. 대출상태의 여부에 따라 참조 테이블이 달라진다.
				/////////////////////////////////////////////////////////////////////////
				if (result == _T("Y"))
					ids = pDM->MakeDeleteFrame( _T("MN_LOAN_USER_GROUP_LIST_TBL") , _T("GROUP_CODE") ,
					                         _T("STRING") , sCode );
				else
					ids = pDM->MakeDeleteFrame( _T("CO_NONMEMBER_TBL") , _T("GROUP_CODE") ,
					                         _T("STRING") , sCode );

				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("DeleteGroup") );

				pDM->SendFrame();
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("DeleteGroup") );
			}
			else 
				return 0;
		}

		ids = pDM->GetCellData( _T("그룹정보코드키") , nIndex , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("DeleteGroup") );


		ids = pDM->MakeDeleteFrame( _T("MN_LOAN_USER_GROUP_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("DeleteGroup") );
		nDCnt++;

		aDIndexs.Add(nIndex);

		pGrid->SelectGetPrev();

	}


	if( nDCnt > 0 )
	{
		ids = pDM->SendFrame();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("DeleteGroup") );
	}

	pDM->EndFrame();

	if( nDCnt == 0 )
	{
		AfxMessageBox( _T("삭제할 그룹이 존재하지 않습니다. ") );
		return 1;
	}

	/// DM 삭제
	for( i = 0 ; i < aDIndexs.GetSize() ; i++ )
	{
		ids = pDM->DeleteRow( aDIndexs.GetAt(i) );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("DeleteGroup") );
	}

	/// 그리드 적용
	ids = AllControlDisplay( _T("CM_BO_LOC_3850") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("DeleteGroup") ); 

	AfxMessageBox( _T("삭제되었습니다.") );

	return 0;

EFS_END
return -1;

}

HBRUSH CBO_LOC_3850::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
