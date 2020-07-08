// MngLocMgrAPI.cpp: implementation of the CMngLocMgrAPI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MngLocMgrAPI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMngLocMgrAPI::CMngLocMgrAPI(CESL_Mgr *pWnd )
	: CSPF_Work(pWnd)
{
	SettingVariable();
}

CMngLocMgrAPI::~CMngLocMgrAPI()
{
	for( INT i = 0 ; i < 4 ; i++ )
	{
		delete []m_sControlAlias[i];
		delete []m_sManageAlias[i];
		delete []m_sManageValue[i];
		delete []m_sResultValue[i];
	}

	delete []m_sControlAlias;
	delete []m_sManageAlias;
	delete []m_sManageValue;
	delete []m_sResultValue;

}

INT CMngLocMgrAPI::AllWndView()
{
	EFS_BEGIN

	INT ids;

	ids = Wnd01View();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AllWndView") );

	ids = Wnd02View();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("AllWndView") );

	ids = Wnd03View();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("AllWndView") );

	ids = Wnd04View();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("AllWndView") );

	return 0;

	EFS_END
	return -1;

}

// 첫번째 화면 보여주기
INT CMngLocMgrAPI::Wnd01View()
{
	EFS_BEGIN

	INT ids;
	INT nIndex = 0;

	// 타입에 따라서..
	ids = ViewScreen( nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("Wnd01View") );

	return 0;

	EFS_END
	return -1;

}

// 두번째 화면 보여주기
INT CMngLocMgrAPI::Wnd02View()
{
	EFS_BEGIN

	INT ids;
	INT nIndex = 1;

	// 타입에 따라서..
	ids = ViewScreen( nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("Wnd01View") );
	return 0;

	EFS_END
	return -1;

}

// 세번째 화면 보여주기
INT CMngLocMgrAPI::Wnd03View()
{
	EFS_BEGIN

	INT ids;
	INT nIndex = 2;

	// 타입에 따라서..
	ids = ViewScreen( nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("Wnd01View") );
	return 0;

	EFS_END
	return -1;

}

// 네번째 화면 보여주기
INT CMngLocMgrAPI::Wnd04View()
{
	EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_MNG_LOC_MGR_04_DLG");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("Wnd01View") );

//	ids = pDM->RefreshDataManager( _T("") );
	ids = CLocCommonAPI::RefreshDataManager( m_pParentMgr , pDM , _T("") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("Wnd01View") ); 

	ids = m_pParentMgr->AllControlDisplay( m_sCMAlias[3] );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("Wnd01View") ); 

	return 0;

	EFS_END
	return -1;

}


INT CMngLocMgrAPI::ViewScreen( INT nIndex )
{
	EFS_BEGIN

	/*
	switch( nIndex ) {
	case 0:
			ViewScreen01()
	*/	

	/*
	INT ids;
	CString sValue;
	for( INT i = 0 ; i < m_nControlAliasCnt[nIndex] ; i++ )
	{
		// 값을 가져온다.
		// 화면에 보여준다.
		ids = m_pParentMgr->GetManageValue( m_sGroup2 , m_sGroup3 , m_sManageAlias[nIndex][i] , m_sManageValue[nIndex][i] , sValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("ViewScreen") );

		CESL_ControlMgr *pCM = m_pParentMgr->FindCM( m_sCMAlias[nIndex]);
		if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ViewScreen") );

		CESL_Control_Element *pE = pCM->FindControlElement( m_sControlAlias[nIndex][i] );
		if( pE == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("ViewScreen") );

		if( (m_sResultValue[nIndex][i].Compare(_T("Y"))==0) || (m_sResultValue[nIndex][i].Compare(_T("N"))==0) )
		{
			CButton *pB = (CButton*)(m_pParentMgr->GetDlgItem( pE->CTRL_ID ));
			if( pB == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("ViewScreen") );

			if( m_sResultValue[nIndex][i].Compare( sValue ) == 0 ) 
				pB->SetCheck(1);
			else 
				pB->SetCheck(0);
		}
		else if( m_sResultValue[nIndex][i].Compare(_T("CMB")) == 0 )
		{
			CComboBox *pComboBox = (CComboBox*)(m_pParentMgr->GetDlgItem( pE->CTRL_ID ));
			if( pComboBox == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("ViewScreen") );

			ids = pComboBox->SetCurSel( _ttoi(sValue) );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -6 , _T("ViewScreen") );

		}
		else if( m_sResultValue[nIndex][i].Compare(_T("EDT")) == 0 )
		{
			ids = m_pParentMgr->SetDataMgrData( m_sCMAlias[nIndex] , m_sControlAlias[nIndex][i] , sValue );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -7 , _T("ViewScreen") );
		}
		
	}
	*/

	return 0;

	EFS_END
	return -1;

}

VOID CMngLocMgrAPI::SettingVariable()
{
	EFS_BEGIN

	/*
	const INT nCnt = 4;
	INT nControlAliasCnt[nCnt];
	nControlAliasCnt[0] = 10;
	nControlAliasCnt[1] = 17;
	nControlAliasCnt[2] = 14;
	nControlAliasCnt[3] = 1;

	for( INT i = 0 ; i < nCnt ; i++ )
		m_nControlAliasCnt[i] = nControlAliasCnt[i] ;

	m_sControlAlias = new CString*[nCnt];
	m_sManageAlias = new CString*[nCnt];
	m_sManageValue = new CString*[nCnt];
	m_sResultValue = new CString*[nCnt];

	m_sCMAlias = new CString[nCnt];

	CString sCMAlias[nCnt] = 
	{
		_T("CM_MNG_LOC_MGR_01_DLG") , _T("CM_MNG_LOC_MGR_02_DLG") , _T("CM_MNG_LOC_MGR_03_DLG") , _T("CM_MNG_LOC_MGR_04_DLG")
	};


	CString sControlAlias01[10][4] =
	{
		{ _T("가족재대출여부Y") , _T("대출관리") , _T("가족재대출여부") , _T("Y") } ,
		{ _T("가족재대출여부N") , _T("대출관리") , _T("가족재대출여부") , _T("N") } ,
		{ _T("도서대출여부Y") , _T("대출관리") , _T("도서재대출여부") , _T("Y")} ,
		{ _T("도서대출여부N") , _T("대출관리") , _T("도서재대출여부") , _T("N") } ,
		{ _T("비도서대출여부Y") , _T("대출관리") , _T("비도서재대출여부") ,_T("Y") } ,
		{ _T("비도서대출여부N") , _T("대출관리") , _T("비도서재대출여부") , _T("N") } ,
		{ _T("개인재대출여부Y") , _T("대출관리") , _T("개인재대출여부") , _T("Y") } ,
		{ _T("개인재대출여부N") , _T("대출관리") , _T("개인재대출여부") , _T("N") } ,
		{ _T("관리구분값대출여부Y") , _T("대출관리") , _T("다른관리구분값대출여부") , _T("Y")} ,
		{ _T("관리구분값대출여부N") , _T("대출관리") , _T("다른관리구분값대출여부") , _T("N") }
	};
	CString sControlAlias02[17][4] =
	{
		{ _T("일반대출사용여부Y") , _T("대출형식사용여부") , _T("일반") , _T("Y") } ,
		{ _T("일반대출사용여부N") , _T("대출형식사용여부") , _T("일반") , _T("N") } ,
		{ _T("관내대출사용여부Y") , _T("대출형식사용여부") , _T("관내") , _T("Y")} ,
		{ _T("관내대출사용여부N") , _T("대출형식사용여부") , _T("관내") , _T("N") } ,
		{ _T("특별대출사용여부Y") , _T("대출형식사용여부") , _T("특별") , _T("Y")} ,
		{ _T("특별대출사용여부N") , _T("대출형식사용여부") , _T("특별") , _T("N") } ,
		{ _T("일반반납사용여부Y") , _T("반납형식사용여부") , _T("일반") , _T("Y")} ,
		{ _T("일반반납사용여부N") , _T("반납형식사용여부") , _T("일반") , _T("N") } ,
		{ _T("일괄반납사용여부Y") , _T("반납형식사용여부") , _T("일괄") , _T("Y")} ,
		{ _T("일괄반납사용여부N") , _T("반납형식사용여부") , _T("일괄") , _T("N") } ,
		{ _T("소급반납사용여부Y") , _T("반납형식사용여부") , _T("소급") , _T("Y")} ,
		{ _T("소급반납사용여부N") , _T("반납형식사용여부") , _T("소급") , _T("N") } ,
		{ _T("반납연기시예약우선순위결정") , _T("반납연기관리") , _T("반납연기예약우선순위") , _T("CMB")} ,
		{ _T("예약기능사용여부Y") , _T("예약관리") , _T("예약기능사용여부") , _T("Y") } ,
		{ _T("예약기능사용여부N") , _T("예약관리") , _T("예약기능사용여부") , _T("N") } ,
		{ _T("예약자동취소여부Y") , _T("예약관리") , _T("예약자동취소여부") , _T("Y") } ,
		{ _T("예약자동취소여부N") , _T("예약관리") , _T("예약자동취소여부") , _T("N") }
	};
	CString sControlAlias03[14][4] =
	{
		{ _T("대출정지일부여여부Y") , _T("연체관리") , _T("대출정지일부여여부") , _T("Y")} , 
		{ _T("대출정지일부여여부N") , _T("연체관리") , _T("대출정지일부여여부") , _T("N")} , 
		{ _T("연체료부여여부Y") , _T("연체관리") , _T("연체료부여여부") , _T("Y")} , 
		{ _T("연체료부여여부N") , _T("연체관리") , _T("연체료부여여부") , _T("N")} , 
		{ _T("연체료부여방법") , _T("연체관리") , _T("연체일모드") , _T("CMB")} , 
		{ _T("연체료가중치") , _T("연체관리") , _T("연체가중치") , _T("EDT")} , 
		{ _T("기본연체료") , _T("연체관리") , _T("연체료") , _T("EDT")} , 
		{ _T("누적연체횟수부여방법") , _T("연체관리") , _T("누적연체횟수부여방법") , _T("CMB")} , 
		{ _T("가족회원기능사용여부Y"), _T("이용자관리") , _T("가족회원기능사용여부") , _T("Y")} , 
		{ _T("가족회원기능사용여부N") , _T("이용자관리") , _T("가족회원기능사용여부") , _T("N")} , 
		{ _T("대출자번호자동부여여부Y") , _T("이용자관리") , _T("대출자번호자동부여여부") , _T("Y")} , 
		{ _T("대출자번호자동부여여부N") , _T("이용자관리") , _T("대출자번호자동부여여부") , _T("N")} , 
		{ _T("대출정지일자동일괄해제기능사용여부Y") , _T("이용자관리") , _T("대출정지일괄해제여부") , _T("Y")} , 
		{ _T("대출정지일자동일괄해제기능사용여부N") , _T("이용자관리") , _T("대출정지일괄해제여부") , _T("N")}
	};
	CString sControlAlias04[1][4] =
	{
		{ _T("대출직급별환경설정그리드") , _T("기타") , _T("기타") }
	};

	for( i=0 ; i < nCnt ; i++ )
	{
		m_sControlAlias[i] = new CString[nControlAliasCnt[i]];
		m_sManageAlias[i] = new CString[nControlAliasCnt[i]];
		m_sManageValue[i] = new CString[nControlAliasCnt[i]];
		m_sResultValue[i] = new CString[nControlAliasCnt[i]];

		m_sCMAlias[i] = sCMAlias[i];

		for( INT j = 0 ; j < nControlAliasCnt[i] ; j++ )
		{

			if( i == 0 )
			{
				m_sControlAlias[i][j] = sControlAlias01[j][0];
				m_sManageAlias[i][j] = sControlAlias01[j][1];
				m_sManageValue[i][j] = sControlAlias01[j][2];
			}
			else if( i == 1 )
			{
				m_sControlAlias[i][j] = sControlAlias02[j][0];
				m_sManageAlias[i][j] = sControlAlias02[j][1];
				m_sManageValue[i][j] = sControlAlias02[j][2];
			}
			else if( i == 2 ) 
			{
				m_sControlAlias[i][j] = sControlAlias03[j][0];
				m_sManageAlias[i][j] = sControlAlias03[j][1];
				m_sManageValue[i][j] = sControlAlias03[j][2];
			}
			else if( i == 3 )
			{
				m_sControlAlias[i][j] = sControlAlias04[j][0];
				m_sManageAlias[i][j] = sControlAlias04[j][1];
				m_sManageValue[i][j] = sControlAlias04[j][2];
			}
		}
	}
	*/

EFS_END
}