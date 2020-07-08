// BO_LOC_3831.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3831.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3831 dialog


CBO_LOC_3831::CBO_LOC_3831(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3831)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CBO_LOC_3831::~CBO_LOC_3831()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}
VOID CBO_LOC_3831::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3831)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3831, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3831)
	ON_BN_CLICKED(IDC_btn3831_Close, Onbtn3831Close)
	ON_BN_CLICKED(IDC_btn3831_Search, Onbtn3831Search)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3831 message handlers

BOOL CBO_LOC_3831::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	INT ids = InitESL_Mgr( _T("BO_LOC_3831") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	ids = InitMailClass();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );
	
	// 2009.08.31 ADD BY CJY : 통합 관리자에 경우에만 관리구분을 선택가능
	if(NULL != m_pInfo && _T("S") != m_pInfo->USER_PRIVILEGE)
	{
		GetDlgItem(IDC_sta3831_MANAGE_CODE)->ShowWindow(SW_HIDE);
 		GetDlgItem(IDC_cmb3831_MANAGE_CODE)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}



BOOL CBO_LOC_3831::Create(CWnd* pParentWnd) 
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_LOC_3831::Onbtn3831Close() 
{
EFS_BEGIN

	OnCancel();	

EFS_END
}




INT CBO_LOC_3831::InitMailClass()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3830_MAIL_CLASS"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitMailClass") );

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3831"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitMailClass") );

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("메일종류")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("InitMailClass") );

	CString sInsertCode[2];
	
	pCMCB->InitMultiComboBox(2);

	sInsertCode[0] = _T("  ");
	sInsertCode[1] = _T("적용안함");
	ids = pCMCB->SetData( sInsertCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InitMailClass") );

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("코드") , i , sInsertCode[0] );
		pDM->GetCellData( _T("설명") , i , sInsertCode[1] );
			
		ids = pCMCB->SetData( sInsertCode , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InitMailClass") );
	}
	
	
	pCMCB->SetCurSel(0);

	return 0;

EFS_END
return -1;

}

VOID CBO_LOC_3831::Onbtn3831Search() 
{
EFS_BEGIN

	AfxGetApp()->DoWaitCursor(1);

	INT ids = SearchHistory();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("Onbtn3831Search") );

	AfxGetApp()->DoWaitCursor(-1);

EFS_END
}


INT CBO_LOC_3831::SearchHistory()
{
EFS_BEGIN

	INT ids;

	CString sWhere;

	ids = GetWhere( sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SearchHistory") );
	if( ids > 0 ) return ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3830"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SearchHistory") );

	ids = pDM->RefreshDataManager(sWhere);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SearchHistory") ); 

	ids = AllControlDisplay(_T("CM_BO_LOC_3830"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SearchHistory") ); 

	AfxMessageBox( _T("검색되었습니다.") );

	INT nRowCnt = pDM->GetRowCount();

	CString sMsg;
	sMsg.Format( _T("%d개의 이력이 검색되었습니다.") , nRowCnt );

	pParentMgr->GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (LPARAM)sMsg.GetBuffer(0));


	return 0;

EFS_END
return -1;

}

INT CBO_LOC_3831::GetWhere( CString &sWhere )
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

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3831"));
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
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmd3831_COMOB2);

	pCombo->GetLBText( pCombo->GetCurSel() , sSendYN );

	if( sSendYN.Compare(_T("전체")) != 0 )
	{
		sWhere += _T(" AND MAIL_SEND_YN = '") + sSendYN+_T("'");
	}

	

	// 수신여부
	CString sReceiptYN;
	pCombo = (CComboBox*)GetDlgItem(IDC_cmd3831_COMOB3);

	pCombo->GetLBText( pCombo->GetCurSel() , sReceiptYN );

	if( sReceiptYN.Compare(_T("전체")) != 0 )
	{
		sWhere += _T(" AND MAIL_CHECK_YN = '") + sReceiptYN+_T("'");
	}

	CString strManageCode;
	CESL_ControlMultiComboBox* pCmb = (CESL_ControlMultiComboBox*)FindControl(_T("CM_BO_LOC_3831"), _T("관리구분"));
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

HBRUSH CBO_LOC_3831::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
