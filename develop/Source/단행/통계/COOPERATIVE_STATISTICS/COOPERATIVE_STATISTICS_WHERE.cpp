// COOPERATIVE_STATISTICS_WHERE.cpp : implementation file
//

#include "stdafx.h"
#include "COOPERATIVE_STATISTICS_WHERE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCOOPERATIVE_STATISTICS_WHERE dialog


CCOOPERATIVE_STATISTICS_WHERE::CCOOPERATIVE_STATISTICS_WHERE(CESL_Mgr* pParent, CStaticsInfoMgr *pSInfoMgr)
	: CESL_Mgr(CCOOPERATIVE_STATISTICS_WHERE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCOOPERATIVE_STATISTICS_WHERE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pSInfoDM = pSInfoMgr->m_pStaticsMainInfoDM;
	m_pSInfoMgr = pSInfoMgr;

}


void CCOOPERATIVE_STATISTICS_WHERE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCOOPERATIVE_STATISTICS_WHERE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCOOPERATIVE_STATISTICS_WHERE, CDialog)
	//{{AFX_MSG_MAP(CCOOPERATIVE_STATISTICS_WHERE)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCOOPERATIVE_STATISTICS_WHERE message handlers

BOOL CCOOPERATIVE_STATISTICS_WHERE::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CCOOPERATIVE_STATISTICS_WHERE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids;
	ids = InitESL_Mgr( _T("COOPERATIVE_STATISTICS_WHERE") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	m_pParentCM = FindCM( _T("CM_COOPERATIVE_STATISTICS_MAIN") );
	if( m_pParentCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );

	m_pSInfoMgr->SetSearchCM(FindCM(_T("CM_COOPERATIVE_STATISTICS_WHERE")));

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE; 
}

INT CCOOPERATIVE_STATISTICS_WHERE::SetControlSetting()
{
	INT ids;
	CString sStaticsClass;
	ids = m_pParentCM->GetControlMgrData( _T("통계종류") , sStaticsClass , -1 , 0 );
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("SetControlDisable") );
	CESL_ControlMgr *pCM = FindCM( _T("CM_COOPERATIVE_STATISTICS_WHERE") );
	if(pCM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("SetControlDisable") );

	// Disable
	
	// 대출/반납 통계 추가
	const INT nDateCnt = 8;
	CESL_Control_Element *pCEtmp;
	pCEtmp = (CESL_Control_Element*)pCM->FindControlElement(_T("관리구분"));

	CString sDateCtrlAlias[nDateCnt] =
	{
		_T("신청일1"), _T("신청일2"), 
		_T("제공일1"), _T("제공일2"),
		_T("책두레대출일1"), _T("책두레대출일2"), 
		_T("책두레반납일1"), _T("책두레반납일2")
//		_T("신청일1"), _T("신청일2"), 
//		_T("제공일1"), _T("제공일2")
	};

	for( INT i = 0 ; i < nDateCnt ; i++ )
	{
		CESL_Control_Element *pCE;
		pCE = (CESL_Control_Element*)pCM->FindControlElement( sDateCtrlAlias[i] );
			
		if(sStaticsClass == _T("제공자료통계(발송)"))
		{
			if(i==2 || i==3)
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(TRUE);
			else
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(FALSE);
				
			SetDlgItemText(IDC_staLoanDate, _T("대출일자"));


			// 체크 해제된 날짜 컨트롤 다시 체크상태로 변경
			SYSTEMTIME time;
			memset(&time, 0x00, sizeof(SYSTEMTIME));
			CDateTimeCtrl* pTimeCtrl = NULL;
 			const INT nCnt = 2;

			INT nIDC[nCnt] =
			{
				IDC_datLOAN_DATE1, IDC_datLOAN_DATE2
 			};

			GetLocalTime(&time);						
			
			for (INT j = 0 ; j < nCnt ; j++)
			{
				pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[j]);
				pTimeCtrl->GetTime(&time);
				DateTime_SetSystemtime(pTimeCtrl->m_hWnd, GDT_VALID, &time);
			}		
			///////////////////////////////////////////////
		}
		
		// 신청자료통계(입수) 수정, 신청자료통계->요청자료통계로 명칭 변경
		else if(sStaticsClass == _T("요청자료통계(입수)"))
		{
			//입수일자 체크 해제
 			SYSTEMTIME systime;	
 			CDateTimeCtrl* pTimeCtrl = NULL;
 			const INT nCnt = 2;
 			
 			INT nIDC[nCnt] =
 			{
 				IDC_datLOAN_DATE1, IDC_datLOAN_DATE2
 			};
 			
 			for( INT j = 0 ; j < nCnt ; j++ )
			{
				pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[j]);
				pTimeCtrl->GetTime(&systime);
				DateTime_SetSystemtime(pTimeCtrl->m_hWnd , GDT_NONE , &systime );
			}
			///////////////////////////////////////
			if(i==0 || i==1 || i==4 || i==5)
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(TRUE);
			else
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(FALSE);

			SetDlgItemText(IDC_staLoanDate, _T("입수일자"));
		}
// 		else if(sStaticsClass == _T("신청자료통계(입수)"))
// 		{
// 			if(i==0 || i==1 )
// 				GetDlgItem(pCE->CTRL_ID)->EnableWindow(TRUE);
// 			else
// 				GetDlgItem(pCE->CTRL_ID)->EnableWindow(FALSE);
// 		}
		
		
		// 대출/반납 통계 추가
		else if(sStaticsClass == _T("대출자료통계"))
		{
			if(i==4 || i==5)
			{
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(FALSE);
			}
			
			SetDlgItemText(IDC_staLoanDate, _T("대출일자"));


			// 체크 해제된 날짜 컨트롤 다시 체크상태로 변경
			SYSTEMTIME time;
			memset(&time, 0x00, sizeof(SYSTEMTIME));
			CDateTimeCtrl* pTimeCtrl = NULL;
			const INT nCnt = 2;
			
			INT nIDC[nCnt] =
			{
				IDC_datLOAN_DATE1, IDC_datLOAN_DATE2
			};
			
			GetLocalTime(&time);						
			
			for (INT j = 0 ; j < nCnt ; j++)
			{
				pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[j]);
				pTimeCtrl->GetTime(&time);
				DateTime_SetSystemtime(pTimeCtrl->m_hWnd, GDT_VALID, &time);
			}		
			///////////////////////////////////////////////
		}
		else if(sStaticsClass == _T("반납자료통계"))
		{
			if(i==6 || i==7)
			{
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(FALSE);
			}
			
			SetDlgItemText(IDC_staLoanDate, _T("대출일자"));


			// 체크 해제된 날짜 컨트롤 다시 체크상태로 변경
			SYSTEMTIME time;
			memset(&time, 0x00, sizeof(SYSTEMTIME));
			CDateTimeCtrl* pTimeCtrl = NULL;
			const INT nCnt = 2;
			
			INT nIDC[nCnt] =
			{
				IDC_datLOAN_DATE1, IDC_datLOAN_DATE2
			};
			
			GetLocalTime(&time);						
			
			for (INT j = 0 ; j < nCnt ; j++)
			{
				pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[j]);
				pTimeCtrl->GetTime(&time);
				DateTime_SetSystemtime(pTimeCtrl->m_hWnd, GDT_VALID, &time);
			}		
			///////////////////////////////////////////////
		}


		// 대출자/반납자 통계 추가
		else if(sStaticsClass == _T("대출자통계"))
		{
			if(i==4 || i==5)
			{
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(FALSE);
			}
			
			SetDlgItemText(IDC_staLoanDate, _T("대출일자"));


			// 체크 해제된 날짜 컨트롤 다시 체크상태로 변경
			SYSTEMTIME time;
			memset(&time, 0x00, sizeof(SYSTEMTIME));
			CDateTimeCtrl* pTimeCtrl = NULL;
			const INT nCnt = 2;
			
			INT nIDC[nCnt] =
			{
				IDC_datLOAN_DATE1, IDC_datLOAN_DATE2
			};
			
			GetLocalTime(&time);						
			
			for (INT j = 0 ; j < nCnt ; j++)
			{
				pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[j]);
				pTimeCtrl->GetTime(&time);
				DateTime_SetSystemtime(pTimeCtrl->m_hWnd, GDT_VALID, &time);
			}		
			///////////////////////////////////////////////
		}
		else if(sStaticsClass == _T("반납자통계"))
		{
			if(i==6 || i==7)
			{
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(pCE->CTRL_ID)->EnableWindow(FALSE);
		}
		
			SetDlgItemText(IDC_staLoanDate, _T("대출일자"));


			// 체크 해제된 날짜 컨트롤 다시 체크상태로 변경
			SYSTEMTIME time;
			memset(&time, 0x00, sizeof(SYSTEMTIME));
			CDateTimeCtrl* pTimeCtrl = NULL;
			const INT nCnt = 2;
			
			INT nIDC[nCnt] =
			{
				IDC_datLOAN_DATE1, IDC_datLOAN_DATE2
			};
			
			GetLocalTime(&time);						
			
			for (INT j = 0 ; j < nCnt ; j++)
			{
				pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[j]);
				pTimeCtrl->GetTime(&time);
				DateTime_SetSystemtime(pTimeCtrl->m_hWnd, GDT_VALID, &time);
			}		
			///////////////////////////////////////////////
		}

		else
		{
			GetDlgItem(pCE->CTRL_ID)->EnableWindow(FALSE);
			
			SetDlgItemText(IDC_staLoanDate, _T("대출일자"));


			// 체크 해제된 날짜 컨트롤 다시 체크상태로 변경
			SYSTEMTIME time;
			memset(&time, 0x00, sizeof(SYSTEMTIME));
			CDateTimeCtrl* pTimeCtrl = NULL;
			const INT nCnt = 2;
			
			INT nIDC[nCnt] =
			{
				IDC_datLOAN_DATE1, IDC_datLOAN_DATE2
			};
			
			GetLocalTime(&time);						
			
			for (INT j = 0 ; j < nCnt ; j++)
			{
				pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[j]);
				pTimeCtrl->GetTime(&time);
				DateTime_SetSystemtime(pTimeCtrl->m_hWnd, GDT_VALID, &time);
			}		
			///////////////////////////////////////////////
		}	
	} 

	// 진행상태 (Default:0009(완료된자료))
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSTATE);
	pCombo->SetCurSel(4);


	return 0;
}

// ** 조건저장
INT CCOOPERATIVE_STATISTICS_WHERE::SaveCondition()
{
EFS_BEGIN

	INT ids;

	// 1. 컨트롤에서 SQL문 구성
	CString sSearchCond;
	ids = CMToSql( sSearchCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveCondition") );

	if( sSearchCond.IsEmpty() )
	{
		AfxMessageBox( _T("조건을 입력해주십시요") );
		return 1;
	}

	// 2. DM에 조건들을 저장하기
	CString sStaticsClass;
	ids = m_pParentCM->GetControlMgrData( _T("통계종류") , sStaticsClass , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SaveCondition") );

	ids = m_pSInfoMgr->SetUserCondition( sStaticsClass , sSearchCond );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SaveCondition") );

	return 0;

EFS_END
return -1;
}

// Control에서 WHERE절 생성
INT CCOOPERATIVE_STATISTICS_WHERE::CMToSql( CString &sSearchCond )
{
EFS_BEGIN

	INT ids;

	CString sStaticsClass;
	ids = m_pParentCM->GetControlMgrData( _T("통계종류"), sStaticsClass, -1, 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("SaveCondition") );

	CESL_ControlMgr *pCM = FindCM( _T("CM_COOPERATIVE_STATISTICS_WHERE") );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CMToSql") );
	
	INT nControlCnt = pCM->Control_List.GetCount();
	CString sData;

	CESL_Control_Element *pCE;
	POSITION pos;
	CString sCtrlType;
	for( INT i = 0 ; i < nControlCnt ; i++ )
	{
		pos = pCM->Control_List.FindIndex(i);
		pCE = (CESL_Control_Element*)pCM->Control_List.GetAt(pos);

		sCtrlType = pCE->CTRL_TYPE;
		sCtrlType.MakeUpper();
		if( sCtrlType == 'S' ) continue;

		CString sAlias = pCE->CTRL_ALIAS;
		ids = pCM->GetControlMgrData( sAlias, sData, -1, 0 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CMToSql") );
		sData.TrimLeft(); sData.TrimRight();

		if( !GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() )
			sData = _T("");

		if( sData.IsEmpty() ) continue;

		if( !sSearchCond.IsEmpty() ) sSearchCond += _T(",") ;

		if(sAlias == _T("관리구분"))
		{
			
			// 대출/반납 통계 추가
			if(sStaticsClass==_T("제공자료통계(발송)"))
			{
				sAlias = _T("제공관리구분");
			}
			
			// 명칭 변경(신청자료통계->요청자료통계)
			else if(sStaticsClass==_T("요청자료통계(입수)") || sStaticsClass==_T("대출자료통계") || sStaticsClass==_T("대출자통계") )
//			else if(sStaticsClass==_T("신청자료통계(입수)") || sStaticsClass==_T("대출자료통계"))
			{
				sAlias = _T("신청관리구분");
			}
			
			// 반납자 통계 여기로 이동
			else if(sStaticsClass==_T("반납자료통계") || sStaticsClass==_T("반납자통계") )
			{
				sAlias = _T("반납관리구분");
			}
//			if(sStaticsClass==_T("제공자료통계(발송)"))
//				sAlias = _T("제공관리구분");
//			else if(sStaticsClass==_T("신청자료통계(입수)"))
//				sAlias = _T("신청관리구분");
			else
				sAlias = _T("관리구분");
		}
		sSearchCond += sAlias + _T("=") + sData;
	}

	// 날짜 컨트롤 계산하기
	UpdateData(FALSE);


	// 대출/반납 통계 추가
	const INT nDateCnt = 9;
//	const INT nDateCnt = 5;


	// 입수일자 반영하도록 변경
	CString strColumnName1=_T("");
	CString strColumnName2=_T("");
	
	// 명칭변경(신청자료통계->요청자료통계)
	if(sStaticsClass==_T("요청자료통계(입수)"))
//	if(sStaticsClass==_T("신청자료통계(입수)"))
	{
		strColumnName1=_T("입수일1");
		strColumnName2=_T("입수일2");

	}else{
		strColumnName1=_T("책두레대출일1");
		strColumnName2=_T("책두레대출일2");
	}
	CString sDateCtrlAlias[nDateCnt] =
	{
		_T("신청일1"), _T("신청일2"), 
		_T("제공일1"), _T("제공일2"),
		strColumnName1, strColumnName2, 
		_T("책두레반납일1"), _T("책두레반납일2"),
		_T("진행상태")
	};

	for( i = 0 ; i < nDateCnt ; i++ )
	{
		pCE = (CESL_Control_Element*)pCM->FindControlElement( sDateCtrlAlias[i] );
				
		sData = CLocCommonAPI::GetDate(this,pCE->CTRL_ID);
		sData.TrimLeft(); sData.TrimRight();
		
		if( !GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() )
			sData = _T("");
		
		if( sData.IsEmpty() ) continue;
		
		if( !sSearchCond.IsEmpty() ) sSearchCond += _T(",") ;
		
		sSearchCond += pCE->CTRL_ALIAS + _T("=") + sData;
	}
	
	return 0;

EFS_END
return -1;
}