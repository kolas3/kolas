// BO_LOC_3841.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3841.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3841 dialog

#define ECO_MAIL_RESULT_FILE _T("..\\cfg\\메일링시스템\\통계결과.cfg")

CBO_LOC_3841::CBO_LOC_3841(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3841)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CBO_LOC_3841::~CBO_LOC_3841()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CBO_LOC_3841::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3841)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3841, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3841)
	ON_BN_CLICKED(IDC_btn3841_Search, Onbtn3841Search)
	ON_BN_CLICKED(IDC_btn3841_Search2, Onbtn3841Search2)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3841 message handlers

BOOL CBO_LOC_3841::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr( _T("BO_LOC_3841") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3841") );

	ids = InitMailClass();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3841") );

	ids = InitResultCfg();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3841") );

	if(NULL != m_pInfo && _T("S") != m_pInfo->USER_PRIVILEGE)
	{
		GetDlgItem(IDC_sta3841_MANAGE_CODE)->ShowWindow(SW_HIDE);
 		GetDlgItem(IDC_cmb3841_MANAGE_CODE)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END 

	return FALSE;
}

VOID CBO_LOC_3841::Onbtn3841Search() 
{
	EFS_BEGIN

	AfxGetApp()->DoWaitCursor(1);

	INT ids = SearchResult();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("Onbtn3841Search") );

	AfxGetApp()->DoWaitCursor(-1);

	EFS_END
	
}

VOID CBO_LOC_3841::Onbtn3841Search2() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}


BOOL CBO_LOC_3841::Create(CWnd* pParentWnd) 
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END

	return FALSE;
}




INT CBO_LOC_3841::InitMailClassInfo()
{
EFS_BEGIN


	INT ids;

	CESL_DataMgr* pMailClassDM = FindDM(_T("DM_BO_LOC_3830_MAIL_CLASS"));
	if( pMailClassDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitMailClassInfo") );

	ids = pMailClassDM->RefreshDataManager(_T(""));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitMailClassInfo") );

	pMailClassDM->SORT( _T("코드") );

	return 0;



EFS_END
return -1;

}


INT CBO_LOC_3841::ConvertMailClassCodeToDesc(CString sCode,CString &sDesc)
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pMailClassDM = FindDM(_T("DM_BO_LOC_3830_MAIL_CLASS"));
	if( pMailClassDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ConvertMailClassCodeToDesc") );

	CString sTmpCode;
	for( INT i = 0 ; i < pMailClassDM->GetRowCount() ; i++ )
	{
		ids = pMailClassDM->GetCellData( _T("코드") , i , sTmpCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ConvertMailClassCodeToDesc") );

		if( sCode.Compare(sTmpCode) == 0 )
		{
			ids = pMailClassDM->GetCellData( _T("설명") , i , sDesc );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ConvertMailClassCodeToDesc") );
			
			return 0;
		}
	}


	return 1;

EFS_END
return -1;

}



INT CBO_LOC_3841::InitMailClassControl()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3830_MAIL_CLASS"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitMailClassControl") );

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3841"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitMailClassControl") );

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("메일종류")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("InitMailClassControl") );

	CString sInsertCode[2];
	
	pCMCB->InitMultiComboBox(2);

	sInsertCode[0] = _T("  ");
	sInsertCode[1] = _T("적용안함");
	ids = pCMCB->SetData( sInsertCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InitMailClassControl") );

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("코드") , i , sInsertCode[0] );
		pDM->GetCellData( _T("설명") , i , sInsertCode[1] );
			
		ids = pCMCB->SetData( sInsertCode , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InitMailClassControl") );
	}
	
	
	pCMCB->SetCurSel(0);

	return 0;

EFS_END
return -1;

}



INT CBO_LOC_3841::InitMailClass()
{
EFS_BEGIN

	INT ids;

	ids = InitMailClassInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitMailClass") );

	ids = InitMailClassControl();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitMailClass") );


	return 0;

EFS_END
return -1;

}


INT CBO_LOC_3841::InitResultCfg()
{
EFS_BEGIN
	
	INT ids;

	/// 첫라인 : 열
	/// 두번째 라인 : 행
	/// 세번째 라인 : 결과값
	CStdioFile fCfgFile;
	ids = fCfgFile.Open( ECO_MAIL_RESULT_FILE , CFile::modeRead | CFile::typeBinary );
	if( ids == 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitResultCfg") );
		TCHAR cUni;
		fCfgFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fCfgFile.SeekToBegin();
		}
	
	CString sColAlias;
	CString sRowAlias;
	CString sResultAlias;

	CString sLine;
	INT nIndex = 0;
	while( fCfgFile.ReadString( sLine ) )
	{
		sLine.TrimRight();
		sLine.TrimLeft();
		if( nIndex == 0 )
			sColAlias = sLine;
		else if( nIndex == 1 )
			sRowAlias = sLine;
		else
		{
			if( nIndex != 2 ) sResultAlias += _T("\r\n");
			sResultAlias += sLine;
		}
		
		nIndex++;
	}

	fCfgFile.Close();

	/// 1. 열 셋팅
	CString sTmp;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmd3841_COMOB7);
	for( INT i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText(i,sTmp);
		if( sColAlias.Compare( sTmp ) == 0 )
		{
			pCombo->SetCurSel(i);
		}
	}

	/// 2. 행 셋팅
	pCombo = (CComboBox*)GetDlgItem(IDC_cmd3841_COMOB6);
	for( i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText(i,sTmp);
		if( sRowAlias.Compare( sTmp ) == 0 )
		{
			pCombo->SetCurSel(i);
		}
	}

	/// 3. 결과 셋팅
	GetDlgItem(IDC_edt3841_EDIT8)->SetWindowText(sResultAlias);
	GetDlgItem(IDC_edt3841_EDIT9)->SetWindowText(_T("(참조 : 총발송건수(발송성공건수(수신건수/미수신건수)/발송실패건수))"));
	

	return 0;



EFS_END

return -1;
}

INT CBO_LOC_3841::SearchResult()
{
EFS_BEGIN

	INT ids;
	CString sWhere;
	CESL_DataMgr SendDM,ResultDM;
	CString sColAlias,sRowAlias,sResultAlias;

	/// 1. 결과값이 제내로 되어있는지 검사하기
	ids = CheckResultValue(sColAlias,sRowAlias,sResultAlias);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchResult") );
	if( ids > 0 ) return ids;

	/// 2. WHERE절 구성하기
	ids = GetWhere( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchResult") );
	if( ids > 0 ) return ids;

	/// 3. 쿼리 날리기
	ids = SendQuery(sWhere,sColAlias,sRowAlias,&SendDM);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchResult") );
	if( ids > 0 ) return ids;

	/// 4. 결과로 결과 DM구축하기
	ids = ReStructDM(&SendDM);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchResult") );
	if( ids > 0 ) return ids;

	/// 5. 그리드에 보여주기
	ids = ShowResult(sColAlias,sRowAlias,sResultAlias);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchResult") );
	if( ids > 0 ) return ids;


	return 0;

EFS_END

return -1;
}


INT CBO_LOC_3841::GetWhere( CString &sWhere )
{
EFS_BEGIN

	INT ids;

	const INT nControlCnt = 10;
	CString sControlAlias[nControlCnt] =
	{
		_T("메일종류") , _T("메일발송일1") , _T("메일발송일2") , _T("배치작업명") , _T("메일주소") , 
		_T("수신자이름") , _T("소속") ,_T("직급") , _T("나이1") , _T("나이2") 
	};

	CString sGetData[nControlCnt];

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3841"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetWhere") );

	for( INT i = 0 ; i < nControlCnt ; i++ )
	{

		ids = pCM->GetControlMgrData( sControlAlias[i] , sGetData[i] , -1 , 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetWhere") );

		sGetData[i].TrimLeft(); sGetData[i].TrimRight();

	}

	// 기본쿼리
	sWhere = _T("MAIL_CLASS NOT IN ( '9' )");

	// 메일종류
	if( !sGetData[0].IsEmpty() )
	{
		sWhere += _T(" AND MAIL_CLASS = '")+sGetData[0]+_T("' ");
	}

	// 메일발송일
	if( !sGetData[1].IsEmpty() )
	{
		if( sGetData[2].IsEmpty() )
		{
			sWhere += _T(" AND SEND_DATE BETWEEN TO_DATE('")+sGetData[1]+_T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND ")
					  _T("TO_DATE('")+sGetData[1]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}
		else
		{
			sWhere += _T(" AND SEND_DATE BETWEEN TO_DATE('")+sGetData[1]+_T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND ")
					  _T("TO_DATE('")+sGetData[2]+_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS') ");
		}
	}

	// 배치 작업명
	if( !sGetData[3].IsEmpty() )
	{
		sWhere += _T(" AND BATCH_WORK_NAME BETWEEN '")+sGetData[3]+_T("' AND '")+sGetData[3]+_T("힝' ");
	}

	// 메일주소
	if( !sGetData[4].IsEmpty() )
	{
		sWhere += _T(" AND E_MAIL BETWEEN '")+sGetData[4]+_T("' AND '")+sGetData[4]+_T("힝' ");
	}

	// 수신자이름
	if( !sGetData[5].IsEmpty() )
	{
		sWhere += _T(" AND NAME BETWEEN '")+sGetData[5]+_T("' AND '")+sGetData[5]+_T("힝' ");
	}

	// 소속
	if( !sGetData[6].IsEmpty() )
	{
		sWhere += _T(" AND USER_POSITION_CODE BETWEEN '")+sGetData[6]+_T("' AND '")+sGetData[6]+_T("힝' ");
	}

	// 직급
	if( !sGetData[7].IsEmpty() )
	{
		sWhere += _T(" AND USER_CLASS_CODE BETWEEN '")+sGetData[7]+_T("' AND '")+sGetData[7]+_T("힝' ");
	}

	// 나이
	if( !sGetData[8].IsEmpty() )
	{
		if( sGetData[9].IsEmpty() )
		{
			sWhere += _T(" AND AGE = ") + sGetData[8];
		}
		else
		{
			sWhere += _T(" AND AGE BETWEEN ")+sGetData[8]+_T(" AND ")+sGetData[9]+_T(" ");
		}
	}

	// 발송여부
	CString sSendYN;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmd3841_COMOB2);

	pCombo->GetLBText( pCombo->GetCurSel() , sSendYN );

	if( sSendYN.Compare(_T("전체")) != 0 )
	{
		sWhere += _T(" AND MAIL_SEND_YN = '") + sSendYN+_T("'");
	}

	

	// 수신여부
	CString sReceiptYN;
	pCombo = (CComboBox*)GetDlgItem(IDC_cmd3841_COMOB3);

	pCombo->GetLBText( pCombo->GetCurSel() , sReceiptYN );

	if( sReceiptYN.Compare(_T("전체")) != 0 )
	{
		sWhere += _T(" AND MAIL_CHECK_YN = '") + sReceiptYN+_T("'");
	}

	CString strManageCode;
	CESL_ControlMultiComboBox* pCmb = (CESL_ControlMultiComboBox*)FindControl(_T("CM_BO_LOC_3841"), _T("관리구분"));
	if(NULL != pCmb)
	{	
		const int nIdx = pCmb->GetCurSel();
		if(0 > nIdx)
		{
			return -1000;
		}

		pCmb->GetData(strManageCode, nIdx, 0);
		if(FALSE == strManageCode.IsEmpty())
		{
			sWhere += _T(" AND MANAGE_CODE = '") + strManageCode +_T("'");
		}
	}

	return 0;

EFS_END
return -1;

}


INT CBO_LOC_3841::CheckResultValue( CString &sColAlias,CString &sRowAlias,CString &sResultAlias)
{
EFS_BEGIN

	INT ids;

	/// 열 가져오기
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmd3841_COMOB7);
	pCombo->GetLBText( pCombo->GetCurSel() , sColAlias );

	/// 행 가져오기
	pCombo = (CComboBox*)GetDlgItem(IDC_cmd3841_COMOB6);
	pCombo->GetLBText( pCombo->GetCurSel() , sRowAlias );


	/// 결과 가져오기
	GetDlgItem(IDC_edt3841_EDIT8)->GetWindowText(sResultAlias);

	if( sResultAlias.IsEmpty() )
	{
		AfxMessageBox( _T("결과 단위를 입력해주십시오.") );
		return 1;
	}

	/// 설정값을 파일에 저장

	CStdioFile fCfgFile;
	ids = fCfgFile.Open( ECO_MAIL_RESULT_FILE , CFile::modeCreate | CFile::modeWrite | CFile::typeBinary );
	if( ids == 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitResultCfg") );

	if( 0 == fCfgFile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fCfgFile.Write( &cUni, sizeof(TCHAR));
	}	
	
	fCfgFile.WriteString( sColAlias );
	fCfgFile.WriteString( _T("\r\n") );
	fCfgFile.WriteString( sRowAlias );
	fCfgFile.WriteString( _T("\r\n") );
	fCfgFile.WriteString( sResultAlias );
	fCfgFile.WriteString( _T("\r\n") );

	fCfgFile.Close();

	return 0;

EFS_END

return -1;
}


INT CBO_LOC_3841::SendQuery( CString sWhere,CString sColAlias,CString sRowAlias,CESL_DataMgr* pSendDM)
{
EFS_BEGIN

	INT ids;

	/// 열 , 행 필드로 변환하기
	CString sColFieldName , sRowFieldName;

	ids = ConvertAliasToFieldName( sColAlias , sColFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SendQuery") );
	if( ids > 0 ) 
	{
		AfxMessageBox( _T("열을 DB Field이름으로 변경할 수 없습니다.") );
		return ids;
	}

	ids = ConvertAliasToFieldName( sRowAlias , sRowFieldName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SendQuery") );
	if( ids > 0 ) 
	{
		AfxMessageBox( _T("열을 DB Field이름으로 변경할 수 없습니다.") );
		return ids;
	}

	/// 쿼리 만들기
	CString sQuery;
	sQuery.Format( _T("SELECT %s,%s,MAIL_SEND_YN,MAIL_CHECK_YN,COUNT(*) FROM MN_MAIL_LOG_TBL WHERE %s ")
				   _T(" GROUP BY %s,%s,MAIL_SEND_YN,MAIL_CHECK_YN ") ,
				   sColFieldName , sRowFieldName , sWhere , sColFieldName , sRowFieldName );

	pSendDM->SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );

	ids = pSendDM->RestructDataManager( sQuery );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SendQuery") );

	if( pSendDM->GetRowCount() == 0 )
	{
		AfxMessageBox( _T("자료가 존재하지 않습니다.") );
		return 1;
	}

	return 0;

EFS_END

return -1;
}


INT CBO_LOC_3841::ConvertAliasToFieldName( CString sAlias , CString &sFieldName )
{
EFS_BEGIN

	const INT nCnt = 8;

	CString sFieldInfo[nCnt][2] =
	{
		{ _T("메일종류") , _T("MAIL_CLASS") } , 
		{ _T("소속자료실") , _T("SHELF_LOC_CODE") } , 
		{ _T("직급") , _T("USER_CLASS_CODE") } , 
		{ _T("소속") , _T("USER_POSITION_CODE") } , 
		{ _T("발송일") , _T("TO_CHAR(SEND_DATE,'YYYY/MM/DD')") } , 
		{ _T("발송월") , _T("TO_CHAR(SEND_DATE,'YYYY/MM')") } , 
		{ _T("발송년") , _T("TO_CHAR(SEND_DATE,'YYYY')") } , 
		{ _T("나이") , _T("FLOOR(AGE/10)*10") } 
	};

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		if( sFieldInfo[i][0].Compare( sAlias ) == 0 )
		{
			sFieldName = sFieldInfo[i][1];
			return 0;
		}
	}


return 1;

EFS_END

return -1;
}

INT CBO_LOC_3841::ReStructDM( CESL_DataMgr* pSendDM)
{
EFS_BEGIN

	INT ids;
	CString sColFieldName , sRowFieldName , sSendYN , sCheckYN , sResult;

	CESL_DataMgr* pResultDM = FindDM( _T("DM_BO_LOC_3840") );
	if( pResultDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("ReStructDM") );

	pResultDM->FreeData();


	INT nRowCnt = pSendDM->GetRowCount();

	for( INT i = 0 ; i < nRowCnt ; i++ )
	{
		ids = pSendDM->GetCellData( i , 0 , sColFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ReStructDM") );
		ids = pSendDM->GetCellData( i , 1 , sRowFieldName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ReStructDM") );
		ids = pSendDM->GetCellData( i , 2 , sSendYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ReStructDM") );
		ids = pSendDM->GetCellData( i , 3 , sCheckYN );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ReStructDM") );
		ids = pSendDM->GetCellData( i , 4 , sResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ReStructDM") );


		ids = InsertValueToResultDM(sColFieldName , sRowFieldName , sSendYN , sCheckYN , sResult);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ReStructDM") );
	}

	/// 합산 계산
	ids = SumCal();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ReStructDM") ); 


	return 0;


EFS_END

return -1;

}


INT CBO_LOC_3841::ShowResult(CString sColAlias,CString sRowAlias,CString sResultAlias)
{
EFS_BEGIN

	INT ids;

	/// 그리드 셋팅
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3840") , _T("메인그리드") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ShowResult") ); 

	pGrid->SetRedraw(FALSE);

	// 열과 행 구성하기
	CESL_DataMgr* pResultDM = FindDM( _T("DM_BO_LOC_3840") );
	if( pResultDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ShowResult") );

	CESL_DataMgr* pColDM = FindDM( _T("DM_BO_LOC_3840_열") );
	if( pColDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ShowResult") );

	CESL_DataMgr* pRowDM = FindDM( _T("DM_BO_LOC_3840_행") );
	if( pRowDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ShowResult") );

	pColDM->FreeData();
	pRowDM->FreeData();
	

	CString sColField , sRowField;
	for( INT i = 0 ; i < pResultDM->GetRowCount() ; i++ )
	{
		ids = pResultDM->GetCellData( _T("열") , i , sColField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ShowResult") );

		ids = pResultDM->GetCellData( _T("행") , i , sRowField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ShowResult") );

		ids = AddRemoveDuplicate( pColDM , sColField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ShowResult") );

		ids = AddRemoveDuplicate( pRowDM , sRowField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("ShowResult") );
	}

	// 정렬하기 , 코드 설명으로 바꾸기
	ids = ConvertCodeToDescSort( pColDM , sColAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("ShowResult") );

	ids = ConvertCodeToDescSort( pRowDM , sRowAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("ShowResult") );

	// 그리드에 쓰기
	INT nColCnt = pColDM->GetRowCount();
	INT nRowCnt = pRowDM->GetRowCount();
	pGrid->SetCols(0,2);
	pGrid->SetRows(2);
	pGrid->SetCols( 0 , pColDM->GetRowCount()+2 );
	pGrid->SetRows( pRowDM->GetRowCount()+2 );

	CString sCheckYCnt , sCheckNCnt , sSendNCnt;
	CString sSendCnt,sSendYCnt;
	CString sInputResult;
	INT nIndex;
	for( i = 0 ; i < pColDM->GetRowCount() ; i++ )
	{
		ids = pColDM->GetCellData( _T("코드") , i , sColField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("ShowResult") ); 
		for( INT j = 0 ; j < pRowDM->GetRowCount() ; j++ )
		{
			ids = pRowDM->GetCellData( _T("코드") , j , sRowField );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("ShowResult") ); 
			
			nIndex = GetResultIdx( sColField , sRowField );

			sSendCnt.Empty();
			sSendYCnt.Empty();
			sCheckYCnt.Empty();
			sCheckNCnt.Empty();
			sSendNCnt.Empty();

			if( nIndex >= 0 )
			{
				ids = pResultDM->GetCellData( _T("총발송건수") , nIndex , sSendCnt );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("ShowResult") ); 
				ids = pResultDM->GetCellData( _T("발송송공건수") , nIndex , sSendYCnt);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("ShowResult") ); 
				ids = pResultDM->GetCellData( _T("수신건수") , nIndex , sCheckYCnt );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -16 , _T("ShowResult") ); 
				ids = pResultDM->GetCellData( _T("미수신건수") , nIndex , sCheckNCnt);
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("ShowResult") ); 
				ids = pResultDM->GetCellData( _T("발송실패건수") , nIndex ,sSendNCnt  );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("ShowResult") ); 
			}

			sInputResult = sResultAlias;

			if( sSendCnt.IsEmpty() ) sSendCnt = _T("0");
			if( sSendYCnt.IsEmpty() ) sSendYCnt = _T("0");
			if( sCheckYCnt.IsEmpty() ) sCheckYCnt = _T("0");
			if( sCheckNCnt.IsEmpty() ) sCheckNCnt = _T("0");
			if( sSendNCnt.IsEmpty() ) sSendNCnt = _T("0");

			sInputResult.Replace( _T("%총발송건수%") , sSendCnt );
			sInputResult.Replace( _T("%발송성공건수%") , sSendYCnt );
			sInputResult.Replace( _T("%수신건수%") , sCheckYCnt );
			sInputResult.Replace( _T("%미수신건수%") , sCheckNCnt );
			sInputResult.Replace( _T("%발송실패건수%") , sSendNCnt );

			pGrid->SetTextMatrix( j+1 , i+1 , sInputResult );
		}
	}

	// 합계 구성하기
	for( i = 0 ; i < pColDM->GetRowCount() ; i++ )
	{
		ids = pColDM->GetCellData( _T("코드") , i , sColField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("ShowResult") ); 

		ids = GetSumInfo( 0 , sColField , sResultAlias , sInputResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -22 , _T("ShowResult") ); 

		pGrid->SetTextMatrix( pRowDM->GetRowCount()+1 , i+1 , sInputResult );

	}

	for( i = 0 ; i < pRowDM->GetRowCount() ; i++ )
	{
		ids = pRowDM->GetCellData( _T("코드") , i , sRowField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -23 , _T("ShowResult") ); 

		ids = GetSumInfo( 1 , sRowField , sResultAlias , sInputResult );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -24 , _T("ShowResult") ); 

		pGrid->SetTextMatrix( i+1 ,  pColDM->GetRowCount()+1 , sInputResult );

	}

	// 전체 합계 구성하기
	ids = GetTotalSumInfo( sResultAlias, sInputResult );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -24 , _T("ShowResult") ); 

	pGrid->SetTextMatrix( pRowDM->GetRowCount()+1 , pColDM->GetRowCount()+1 , sInputResult );


	// 헤더 정보 입력하기
	pGrid->SetColAlignment( 0 , 1 );
	pGrid->SetColAlignment( pColDM->GetRowCount()+1 , 1 );
	pGrid->SetColWidth( 0 , 0 , 2000 );
	pGrid->SetTextMatrix( pRowDM->GetRowCount()+1 , 0 , _T("합계") );
	pGrid->SetCol( pColDM->GetRowCount() + 1 );
	pGrid->SetRow( 0 );
	pGrid->SetCellFontBold(TRUE);
	pGrid->SetCol( 0 );
	pGrid->SetRow( pRowDM->GetRowCount() + 1 );
	pGrid->SetCellFontBold(TRUE);
	pGrid->SetCol( pColDM->GetRowCount() + 1 );
	pGrid->SetRow( pRowDM->GetRowCount() + 1 );
	pGrid->SetCellFontBold(TRUE);


	for( i = 0 ; i < pColDM->GetRowCount() ; i++ )
	{
		pGrid->SetColAlignment( i+1 , 1 );
		ids = pColDM->GetCellData( _T("설명") , i , sColField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("ShowResult") ); 

		pGrid->SetTextMatrix( 0 , i+1 , sColField );
		pGrid->SetCol(i+1);
		pGrid->SetRow(0);
		pGrid->SetCellFontBold(TRUE);
		pGrid->SetColWidth( i+1 , 0 , 2000 );
	}

	pGrid->SetColWidth( i+1 , 0 , 2000 );
	
	// 헤더 정보 입력하기
	pGrid->SetTextMatrix( 0 , pColDM->GetRowCount()+1 , _T("합계") );
	pGrid->SetRowHeight(0,600);
	for( i = 0 ; i < pRowDM->GetRowCount() ; i++ )
	{
		ids = pRowDM->GetCellData( _T("설명") , i , sColField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("ShowResult") ); 

		pGrid->SetTextMatrix( i+1 , 0 , sColField );
		pGrid->SetCol(0);
		pGrid->SetRow(i+1);
		pGrid->SetCellFontBold(TRUE);
		pGrid->SetRowHeight(i+1,600);
	}
	pGrid->SetRowHeight(i+1,600);

	pGrid->SetRedraw(TRUE);

	AfxMessageBox( _T("완료되었습니다.") );


	return 0;


EFS_END

return -1;

}


INT CBO_LOC_3841::InsertValueToResultDM( CString sColFieldName , CString sRowFieldName , CString sSendYN , CString sCheckYN , CString sResult)
{
EFS_BEGIN

	INT ids;

	INT nIndex = -1;
	CESL_DataMgr* pResultDM = FindDM( _T("DM_BO_LOC_3840") );
	if( pResultDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertValueToResultDM") );

	CString sTmpColName , sTmpRowName;
	BOOL bStop = FALSE;
	for( INT i = 0 ; i < pResultDM->GetRowCount() ; i++ )
	{
		ids = pResultDM->GetCellData( _T("열") , i , sTmpColName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertValueToResultDM") );

		ids = pResultDM->GetCellData( _T("행") , i , sTmpRowName );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertValueToResultDM") );

		if( (sTmpColName.Compare(sColFieldName) == 0) && (sTmpRowName.Compare( sRowFieldName ) == 0) )
		{
			nIndex = i;
			bStop = TRUE;
		}
		if( bStop ) break;
	}

	if( nIndex == -1 )
	{
		pResultDM->InsertRow(-1);
		nIndex = pResultDM->GetRowCount() - 1;

		ids = pResultDM->SetCellData( _T("열")  , sColFieldName , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("InsertValueToResultDM") );
		ids = pResultDM->SetCellData( _T("행")  , sRowFieldName , nIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("InsertValueToResultDM") );

	}

	CString sInsertColAlias;
	if( sSendYN.Compare(_T("Y")) == 0 )
	{
		if( sCheckYN.Compare(_T("Y")) == 0 )
			sInsertColAlias = _T("수신건수");
		else
			sInsertColAlias = _T("미수신건수");
	}
	else
	{
		sInsertColAlias = _T("발송실패건수");
	}


	CString sPreCnt;
	ids = pResultDM->GetCellData( sInsertColAlias , nIndex , sPreCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertValueToResultDM") );

	sResult.Format( _T("%d") , _ttoi(sResult) + _ttoi(sPreCnt) );

	ids = pResultDM->SetCellData( sInsertColAlias  , sResult , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InsertValueToResultDM") );


	return 0;

EFS_END

return -1;
}

INT CBO_LOC_3841::SumCal()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pResultDM = FindDM( _T("DM_BO_LOC_3840") );
	if( pResultDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SumCal") );
	
	INT nRowCnt = pResultDM->GetRowCount();

	CString sCheckYCnt , sCheckNCnt , sSendNCnt;
	CString sSendCnt,sSendYCnt;


	for( INT i = 0 ; i < nRowCnt ; i++ )
	{
		ids = pResultDM->GetCellData( _T("수신건수") , i , sCheckYCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SumCal") ); 

		ids = pResultDM->GetCellData( _T("미수신건수") , i , sCheckNCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SumCal") ); 

		ids = pResultDM->GetCellData( _T("발송실패건수") , i , sSendNCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SumCal") ); 
		
		sSendCnt.Format( _T("%d") , _ttoi(sCheckYCnt) + _ttoi(sCheckNCnt) + _ttoi(sSendNCnt) );

		ids = pResultDM->SetCellData( _T("총발송건수") , sSendCnt , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("SumCal") ); 

		sSendYCnt.Format( _T("%d") , _ttoi(sCheckYCnt) + _ttoi(sCheckNCnt) ) ;

		ids = pResultDM->SetCellData( _T("발송송공건수") , sSendYCnt , i );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("SumCal") ); 
	}

	


	return 0;

EFS_END

return -1;
}


INT CBO_LOC_3841::AddRemoveDuplicate( CESL_DataMgr* pDM , CString sField )
{
EFS_BEGIN

	INT ids;
	INT nIndex=-1;
	
	CString sCode;
	BOOL bStop = FALSE;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("코드") , i , sCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("AddRemoveDuplicate") );

		if( sCode.Compare(sField) == 0 )
		{
			nIndex = i;
			bStop = TRUE;
		}

		if( bStop ) break;

	}

	if( nIndex != -1 ) return 0;

	pDM->InsertRow(-1);

	ids = pDM->SetCellData( _T("코드") ,  sField , pDM->GetRowCount()-1);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("AddRemoveDuplicate") );


	return 0;

EFS_END

return -1;
}


INT CBO_LOC_3841::ConvertCodeToDescSort( CESL_DataMgr* pDM , CString sAlias )
{
EFS_BEGIN

	INT ids;

	CString sSortAlias;
	CString sCode;
	CString sDesc;
	if( sAlias.Compare( _T("메일종류") ) == 0 )
	{
		sSortAlias = _T("코드");

		for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
		{
			ids = pDM->GetCellData( _T("코드") , i , sCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ConvertCodeToDescSort") );

			ids = ConvertMailClassCodeToDesc( sCode , sDesc );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ConvertCodeToDescSort") );

			if( sDesc.IsEmpty() ) sDesc = sCode;

			ids = pDM->SetCellData( _T("설명") , sDesc , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ConvertCodeToDescSort") );

		}
	}
	else if( sAlias.Compare( _T("소속자료실") ) == 0 )
	{
		sSortAlias = _T("코드");

		for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
		{
			ids = pDM->GetCellData( _T("코드") , i , sCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ConvertCodeToDescSort") );

			ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분") ,  sCode , sDesc );
//			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ConvertCodeToDescSort") );

			if( sDesc.IsEmpty() ) sDesc = sCode;
			sDesc += _T("(")+sCode+_T(")");

			ids = pDM->SetCellData( _T("설명") , sDesc , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ConvertCodeToDescSort") );

		}

	}
	else if( sAlias.Compare( _T("직급") ) == 0 )
	{
		sSortAlias = _T("코드");

		for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
		{
			ids = pDM->GetCellData( _T("코드") , i , sCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ConvertCodeToDescSort") );

			ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출직급정보") ,  sCode , sDesc );
//			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("ConvertCodeToDescSort") );

			if( sDesc.IsEmpty() ) sDesc = sCode;
			sDesc += _T("(")+sCode+_T(")");

			ids = pDM->SetCellData( _T("설명") , sDesc , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("ConvertCodeToDescSort") );

		}

	}
	else if( sAlias.Compare( _T("소속") ) == 0 )
	{
		sSortAlias = _T("코드");

		for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
		{
			ids = pDM->GetCellData( _T("코드") , i , sCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("ConvertCodeToDescSort") );

			ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출소속정보") ,  sCode , sDesc );
//			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("ConvertCodeToDescSort") );

			if( sDesc.IsEmpty() ) sDesc = sCode;
			sDesc += _T("(")+sCode+_T(")");

			ids = pDM->SetCellData( _T("설명") , sDesc , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("ConvertCodeToDescSort") );

		}

	}
	else if( sAlias.Compare( _T("발송일") ) == 0 || sAlias.Compare( _T("발송월") ) == 0 || sAlias.Compare( _T("발송년") ) == 0 
		|| sAlias.Compare( _T("나이") ) == 0 )
	{
		sSortAlias = _T("설명");

		for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
		{
			ids = pDM->GetCellData( _T("코드") , i , sCode );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("ConvertCodeToDescSort") );

			sDesc = sCode;

			ids = pDM->SetCellData( _T("설명") , sDesc , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("ConvertCodeToDescSort") );

		}

	}

	ids = pDM->SORT( sSortAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("ConvertCodeToDescSort") ); 

	return 0;

EFS_END

return -1;

}

INT CBO_LOC_3841::GetResultIdx( CString sColField , CString sRowField )
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pResultDM = FindDM( _T("DM_BO_LOC_3840") );
	if( pResultDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SumCal") );

	CString sTmpCol , sTmpRow;
	for( INT i = 0 ; i < pResultDM->GetRowCount() ; i++ )
	{
		ids = pResultDM->GetCellData( _T("열") , i , sTmpCol );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetResultIdx") );

		ids = pResultDM->GetCellData( _T("행") , i , sTmpRow );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetResultIdx") );

		if( (sTmpCol.Compare( sColField ) == 0) && (sTmpRow.Compare(sRowField) == 0) ) 
		{
			return i;
		}

	}


return -1;

EFS_END

return -1;
}


INT CBO_LOC_3841::GetSumInfo( INT nMode , CString sField , CString sResultAlias , CString &sInputResult )
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pResultDM = FindDM( _T("DM_BO_LOC_3840") );
	if( pResultDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetSumInfo") );


	CString sColAlias;

	if( nMode == 0 ) 
		sColAlias = _T("열");
	else
		sColAlias = _T("행");

	CString sTmpField;
	CString sCheckYCnt , sCheckNCnt , sSendNCnt;
	CString sSendCnt,sSendYCnt;
	CString sTmpCheckYCnt , sTmpCheckNCnt , sTmpSendNCnt;
	CString sTmpSendCnt,sTmpSendYCnt;
	for( INT i = 0 ; i < pResultDM->GetRowCount() ; i++ )
	{
		ids = pResultDM->GetCellData( sColAlias , i , sTmpField );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetSumInfo") );

		if( sTmpField.Compare(sField) == 0 )
		{
			ids = pResultDM->GetCellData( _T("총발송건수") , i , sTmpSendCnt );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("GetSumInfo") ); 
			ids = pResultDM->GetCellData( _T("발송송공건수") , i , sTmpSendYCnt);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("GetSumInfo") ); 
			ids = pResultDM->GetCellData( _T("수신건수") , i , sTmpCheckYCnt );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -16 , _T("GetSumInfo") ); 
			ids = pResultDM->GetCellData( _T("미수신건수") , i , sTmpCheckNCnt);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("GetSumInfo") ); 
			ids = pResultDM->GetCellData( _T("발송실패건수") , i ,sTmpSendNCnt  );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("GetSumInfo") ); 

			sSendCnt.Format( _T("%d") , _ttoi(sSendCnt)+_ttoi(sTmpSendCnt) );
			sSendYCnt.Format( _T("%d") , _ttoi(sSendYCnt)+_ttoi(sTmpSendYCnt) );
			sCheckYCnt.Format( _T("%d") , _ttoi(sCheckYCnt)+_ttoi(sTmpCheckYCnt) );
			sCheckNCnt.Format( _T("%d") , _ttoi(sCheckNCnt)+_ttoi(sTmpCheckNCnt) );
			sSendNCnt.Format( _T("%d") , _ttoi(sSendNCnt)+_ttoi(sTmpSendNCnt) );

		}
	}
	sInputResult = sResultAlias;

	if( sSendCnt.IsEmpty() ) sSendCnt = _T("0");
	if( sSendYCnt.IsEmpty() ) sSendYCnt = _T("0");
	if( sCheckYCnt.IsEmpty() ) sCheckYCnt = _T("0");
	if( sCheckNCnt.IsEmpty() ) sCheckNCnt = _T("0");
	if( sSendNCnt.IsEmpty() ) sSendNCnt = _T("0");

	sInputResult.Replace( _T("%총발송건수%") , sSendCnt );
	sInputResult.Replace( _T("%발송성공건수%") , sSendYCnt );
	sInputResult.Replace( _T("%수신건수%") , sCheckYCnt );
	sInputResult.Replace( _T("%미수신건수%") , sCheckNCnt );
	sInputResult.Replace( _T("%발송실패건수%") , sSendNCnt );

	return 0;

EFS_END

return -1;

}


INT CBO_LOC_3841::GetTotalSumInfo( CString sResultAlias, CString &sInputResult )
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pResultDM = FindDM( _T("DM_BO_LOC_3840") );
	if( pResultDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GetSumInfo") );


	CString sCheckYCnt , sCheckNCnt , sSendNCnt;
	CString sSendCnt,sSendYCnt;
	CString sTmpCheckYCnt , sTmpCheckNCnt , sTmpSendNCnt;
	CString sTmpSendCnt,sTmpSendYCnt;
	for( INT i = 0 ; i < pResultDM->GetRowCount() ; i++ )
	{
		ids = pResultDM->GetCellData( _T("총발송건수") , i , sTmpSendCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("GetTotalSumInfo") ); 
		ids = pResultDM->GetCellData( _T("발송송공건수") , i , sTmpSendYCnt);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("GetTotalSumInfo") ); 
		ids = pResultDM->GetCellData( _T("수신건수") , i , sTmpCheckYCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -16 , _T("GetTotalSumInfo") ); 
		ids = pResultDM->GetCellData( _T("미수신건수") , i , sTmpCheckNCnt);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("GetTotalSumInfo") ); 
		ids = pResultDM->GetCellData( _T("발송실패건수") , i ,sTmpSendNCnt  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("GetTotalSumInfo") ); 

		sSendCnt.Format( _T("%d") , _ttoi(sSendCnt)+_ttoi(sTmpSendCnt) );
		sSendYCnt.Format( _T("%d") , _ttoi(sSendYCnt)+_ttoi(sTmpSendYCnt) );
		sCheckYCnt.Format( _T("%d") , _ttoi(sCheckYCnt)+_ttoi(sTmpCheckYCnt) );
		sCheckNCnt.Format( _T("%d") , _ttoi(sCheckNCnt)+_ttoi(sTmpCheckNCnt) );
		sSendNCnt.Format( _T("%d") , _ttoi(sSendNCnt)+_ttoi(sTmpSendNCnt) );

	}
	sInputResult = sResultAlias;

	if( sSendCnt.IsEmpty() ) sSendCnt = _T("0");
	if( sSendYCnt.IsEmpty() ) sSendYCnt = _T("0");
	if( sCheckYCnt.IsEmpty() ) sCheckYCnt = _T("0");
	if( sCheckNCnt.IsEmpty() ) sCheckNCnt = _T("0");
	if( sSendNCnt.IsEmpty() ) sSendNCnt = _T("0");

	sInputResult.Replace( _T("%총발송건수%") , sSendCnt );
	sInputResult.Replace( _T("%발송성공건수%") , sSendYCnt );
	sInputResult.Replace( _T("%수신건수%") , sCheckYCnt );
	sInputResult.Replace( _T("%미수신건수%") , sCheckNCnt );
	sInputResult.Replace( _T("%발송실패건수%") , sSendNCnt );



	return 0;


EFS_END

return -1;

}

HBRUSH CBO_LOC_3841::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
