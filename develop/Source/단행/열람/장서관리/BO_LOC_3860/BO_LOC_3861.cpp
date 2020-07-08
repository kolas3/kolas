// BO_LOC_3861.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3861.h"
// 2011.09.07 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3861 dialog


CBO_LOC_3861::CBO_LOC_3861(CESL_Mgr* pParent,INT nMode)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3861)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nMode = nMode;
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CBO_LOC_3861::~CBO_LOC_3861()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CBO_LOC_3861::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3861)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3861, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3861)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btn3861_CLOSE, Onbtn3861CLOSE)
	ON_BN_CLICKED(IDC_btn3861_SEARCH, Onbtn3861SEARCH)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3861 message handlers

BOOL CBO_LOC_3861::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr( _T("BO_LOC_3861") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	ChangeScreenFromMode();

	InitControl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}


BOOL CBO_LOC_3861::Create(CWnd* pParentWnd) 
{
EFS_BEGIN


	return CDialog::Create(IDD, pParentWnd);


EFS_END
return FALSE;

}

VOID CBO_LOC_3861::Onbtn3861CLOSE() 
{
EFS_BEGIN

	OnCancel();	

EFS_END
}

VOID CBO_LOC_3861::Onbtn3861SEARCH() 
{
EFS_BEGIN

	AfxGetApp()->DoWaitCursor(1);

	if( m_nMode == 0 )
		SearchUser();
	else
		ChangeUser();
	
	AfxGetApp()->DoWaitCursor(-1);

EFS_END
}


INT CBO_LOC_3861::SearchUser()
{
EFS_BEGIN

	INT ids;

	CString sWhere;

	ids = GetWhere(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchUser") );

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3860"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchUser") );

	ids = pDM->RefreshDataManager(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SearchUser") ); 

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pDM->GetRowCount())
	{
		CStringArray asDes1,asDes2;
		CString strGetData;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("E_MAIL"), i, strGetData);
			asDes1.Add(strGetData);
			pDM->GetCellData(_T("휴대폰번호"), i, strGetData);
			asDes2.Add(strGetData);
		}
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes2);
					pApi->CloseSocket();
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("E_MAIL"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("휴대폰번호"), asDes2.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
	}

	// 2009.03.24 ADD BY CJY : 그룹설명 추가
	
	CESL_DataMgr* pCodeDM = FindDM(_T("DM_USER_GROUP_CODE_INFO"));
	if(NULL == pCodeDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("SearchUser"));		
	}

	CString strCode, strCode2, strDesc;
	const ROW_COUNT = pDM->GetRowCount();
	const CODE_ROW_COUNT = pCodeDM->GetRowCount();

	int nRowCnt = 0;
	int nCodeRowCnt = 0;
	for(nRowCnt = 0; nRowCnt < ROW_COUNT; nRowCnt++)
	{
		ids = pDM->GetCellData(_T("그룹코드"), nRowCnt, strCode);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("SearchUser"));
		}

		if(_T("") == strCode)
		{
			continue;
		}

		for(nCodeRowCnt = 0; nCodeRowCnt < CODE_ROW_COUNT; nCodeRowCnt++)
		{
			ids = pCodeDM->GetCellData(_T("코드"), nCodeRowCnt, strCode2);
			if(0 > ids)
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-1020, _T("SearchUser"));
			}

			if(strCode2 == strCode)
			{
				ids = pCodeDM->GetCellData(_T("이름"), nCodeRowCnt, strDesc);
				if(0 > ids)
				{
					ERROR_MSG_RETURN_ERROR_CODE2(-1030, _T("SearchUser"));
				}				

				ids = pDM->SetCellData(_T("그룹이름"), strDesc, nRowCnt);
				if(0 > ids)
				{
					ERROR_MSG_RETURN_ERROR_CODE2(-1040, _T("SearchUser"));
				}	
			}
		}

	}

	ids = AllControlDisplay(_T("CM_BO_LOC_3860"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SearchUser") ); 

	AfxMessageBox(_T("검색되었습니다."));

	return 0;

EFS_END
return -1;

}


INT CBO_LOC_3861::GetWhere( CString &sWhere )
{
EFS_BEGIN

	INT ids;

	// 2009.03.24 ADD BY CJY : 관리구분 항목 추가
	const INT nCnt = 14;
	CString sData[nCnt];
	
	CString sControlAlias[nCnt] =
	{
		_T("그룹") , _T("소속") , _T("직급") , _T("이용자번호1") , _T("이용자번호2") , 
		_T("E_MAIL") , _T("휴대폰번호") , _T("입력자료실") , _T("생일1") , _T("생일2") , 
		_T("등록일1") , _T("등록일2") , _T("주민등록번호"), _T("관리구분")
	};

	TCHAR* sDBFieldName[nCnt] =
	{
		_T("GROUP_CODE") , _T("USER_POSITION_CODE") , _T("USER_CLASS_CODE") , _T("USER_NO") , _T("USER_NO") , 
		_T("E_MAIL") , _T("IDX_HANDPHONE") , _T("SHELF_LOC_CODE") , _T("BIRTHDAY") , _T("BIRTHDAY") ,
		_T("REG_DATE") , _T("REG_DATE") , _T("CIVIL_NO"), _T("MANAGE_CODE")
	};

	INT nDBFieldType[nCnt] = 
	{
		ETC , ONESTRING , ONESTRING , TWOSTRING , ETC , 
		ONELIKESTRING , ONELIKESTRING , ONESTRING , TWODATE , ETC ,
		TWODATE , ETC , ETC , ONESTRING
	};

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = GetControlData( _T("CM_BO_LOC_3861") , sControlAlias[i] , sData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetWhere") );
	}

	ids = CLocCommonAPI::MakeSql( sData , sDBFieldName , nDBFieldType , nCnt , sWhere , this );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetWhere") );

	if( !sData[0].IsEmpty() )
	{
		if( !sWhere.IsEmpty() ) sWhere += _T(" AND ");

		sWhere += 
			_T(" ( SELECT COUNT(*) ")
			_T("FROM MN_LOAN_USER_GROUP_LIST_TBL MLUGLT ")
			_T("WHERE MLUGLT.USER_KEY = CLU.REC_KEY AND ")
			_T("MLUGLT.GROUP_CODE='") + sData[0] + _T("' ) > 0");
	}

	if( !sData[12].IsEmpty() )
	{
		if( !sWhere.IsEmpty() ) sWhere += _T(" AND ");

		sWhere += _T(" CIVIL_NO BETWEEN '") + sData[12] + _T("' AND '") + sData[12] + _T("힝' ");
	}

	return 0;

EFS_END
return -1;

}

INT CBO_LOC_3861::InitControl()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_USER_GROUP_CODE_INFO"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitControl") );

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3861"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitControl") );

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("그룹")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("InitControl") );

	CString sInsertCode[2];
	
	pCMCB->InitMultiComboBox(2);

	sInsertCode[0] = _T("   ");
	sInsertCode[1] = _T("적용안함");
	ids = pCMCB->SetData( sInsertCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InitControl") );

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("코드") , i , sInsertCode[0] );
		pDM->GetCellData( _T("이름") , i , sInsertCode[1] );

		sInsertCode[0].TrimLeft(); sInsertCode[0].TrimRight();
		if( sInsertCode[0].IsEmpty() ) continue;
			
		ids = pCMCB->SetData( sInsertCode , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InitControl") );
	}
	
	
	pCMCB->SetCurSel(0);


	pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("변경할그룹")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("InitControl") );

	pCMCB->InitMultiComboBox(2);

	sInsertCode[0] = _T("   ");
	sInsertCode[1] = _T("적용안함");
	ids = pCMCB->SetData( sInsertCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InitControl") );

	for( i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("코드") , i , sInsertCode[0] );
		pDM->GetCellData( _T("이름") , i , sInsertCode[1] );

		sInsertCode[0].TrimLeft(); sInsertCode[0].TrimRight();
		if( sInsertCode[0].IsEmpty() ) continue;
			
		ids = pCMCB->SetData( sInsertCode , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InitControl") );
	}
	
	
	pCMCB->SetCurSel(0);

	// 2010.09.07 ADD BY KSJ : 통합도서서비스 사용 할 경우 처리
	if(_T("Y")==m_sIsUnityLoanService)
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edt3861_COND9);
		pEdit->SetLimitText(6);
		GetDlgItem(IDC_staCivilNo)->SetWindowText(_T("주민번호 앞자리"));
	}
	return 0;

EFS_END
return -1;

}


INT CBO_LOC_3861::ChangeScreenFromMode()
{
EFS_BEGIN

	if( m_nMode == 0 ) 
	{
		SetWindowText( _T("검 색") );
		GetDlgItem(IDC_btn3861_SEARCH)->SetWindowText(_T("검 색"));

		GetDlgItem(IDC_sta3861_HIDE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_sta3861_HIDE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cm3861_COND5)->ShowWindow(SW_HIDE);

		CRect WndPos;
		GetWindowRect(WndPos);
		
		MoveWindow( WndPos.left , WndPos.top , WndPos.Width() , WndPos.Height() - 65 );

	}
	else
	{
		SetWindowText( _T("그룹 변경") );
		GetDlgItem(IDC_btn3861_SEARCH)->SetWindowText(_T("변 경"));

	}



	return 0;

EFS_END
return -1;

}

INT CBO_LOC_3861::ChangeUser()
{
EFS_BEGIN


	INT ids;

	CString sSql;
	CString sWhere;
	CString sGroup;

	ids = GetWhere( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ChangeUser") );

	ids = GetControlData( _T("CM_BO_LOC_3861") , _T("변경할그룹") , sGroup , -1 , 0 ) ;
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("ChangeUser") );

	if( sGroup.IsEmpty() )
	{
		AfxMessageBox( _T("변경할 그룹을 지정해주십시오.") );
		return 1;
	}

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3860") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ChangeUser") );

	CESL_DataMgr TmpDM;
	CLocCommonAPI::MakeCopyDM( this , pDM , &TmpDM );

	CString sSQuery;
	if( sWhere.IsEmpty() )
		sSQuery.Format( _T("SELECT CLU.REC_KEY FROM CO_LOAN_USER_TBL CLU") );
	else
		sSQuery.Format( _T("SELECT CLU.REC_KEY FROM CO_LOAN_USER_TBL CLU WHERE %s") , sWhere );

	ids = TmpDM.RestructDataManager( sSQuery );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ChangeUser") );

	if( TmpDM.GetRowCount() == 0 )
	{
		AfxMessageBox( _T("변경 가능한 이용자가 존재하지 않습니다.") );
		return 1;
	}

	// 2011.09.07 ADD : 3000Api
	if(TRUE == m_pInfo->m_bLibSecuUse && TmpDM.GetRowCount())
	{
		CStringArray asDes1,asDes2;
		CString strGetData;
		for(INT i = 0; i < TmpDM.GetRowCount(); i++)
		{
			TmpDM.GetCellData(_T("E_MAIL"), i, strGetData);
			asDes1.Add(strGetData);
			TmpDM.GetCellData(_T("휴대폰번호"), i, strGetData);
			asDes2.Add(strGetData);
		}
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes2);
					pApi->CloseSocket();
				}
				for(i = 0; i < TmpDM.GetRowCount(); i++)
				{
					TmpDM.SetCellData(_T("E_MAIL"), asDes1.GetAt(i), i);
					TmpDM.SetCellData(_T("휴대폰번호"), asDes2.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
	}

	BOOL bIsAdd = FALSE;

	if( ((CButton*)GetDlgItem(IDC_chk3861_COND1))->GetCheck() == 1 ) bIsAdd = TRUE;

	INT nRowCnt = TmpDM.GetRowCount();
	TmpDM.StartFrame();
	CString sRecKey;
	CString sDeleteQuery;
	for( INT i = 0 ; i < nRowCnt ; i++ )
	{
		ids = TmpDM.GetCellData( i , 0 ,  sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ChangeGroup") );

		TmpDM.InitDBFieldData();
		sDeleteQuery.Format( _T("DELETE FROM MN_LOAN_USER_GROUP_LIST_TBL WHERE USER_KEY=%s AND GROUP_CODE='%s';") , sRecKey , sGroup );
		TmpDM.AddFrame( sDeleteQuery );

		if( bIsAdd )
		{
			TmpDM.InitDBFieldData();
			TmpDM.AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , _T("ESL_SEQ.NextVal") ); 
			TmpDM.AddDBFieldData( _T("GROUP_CODE") , _T("STRING") , sGroup );
			TmpDM.AddDBFieldData( _T("USER_KEY") , _T("NUMERIC") , sRecKey  );
			ids = TmpDM.MakeInsertFrame( _T("MN_LOAN_USER_GROUP_LIST_TBL") );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ChangeGroup") );
		}
	}
	ids = TmpDM.SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ChangeUser") );

	TmpDM.EndFrame();

	CString sMsg;
	sMsg.Format( _T("%d명의 이용자의 그룹이 변경되었습니다.") , nRowCnt );
	AfxMessageBox( sMsg );
	
	return 0;

EFS_END
return -1;

}

HBRUSH CBO_LOC_3861::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}