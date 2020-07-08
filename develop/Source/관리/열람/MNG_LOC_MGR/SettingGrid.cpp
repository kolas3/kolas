// SettingGrid.cpp: implementation of the CSettingGrid class.
// 2008.07.29 MAKE BY PWR
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SettingGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSettingGrid::CSettingGrid()
{
	// 2010.07.28 ADD BY KSJ : 통합 회원 관리 시스템 사용여부
	m_bIsUnityLoanService = FALSE;
}

CSettingGrid::~CSettingGrid()
{

}

// 01Dlg (대출/반납형식&이용자)
VOID CSettingGrid::SettingGrid01(CESL_DataMgr *pDM, CESL_Grid *pGrid)
{
	INT nRowCnt = pDM->GetRowCount();
	if(nRowCnt == 0) return ;

	CString sValue , strUserNoAutoMode , strUserNoToUserID;
	// 가족재대출허용, 부록자료대출에 따른 제어
	// DM수정
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		sValue = pDM->GetCellData( _T("기능"), i );
		if( sValue == _T("가족재대출허용") || sValue == _T("부록자료대출") )
		{
			sValue = pDM->GetCellData( _T("설정값"), i );
			if( sValue != _T("Y") )
			{
				pDM->SetCellData( _T("설정값"), _T("N"), i+1 ); 
				pGrid->SetCol(4);
				pGrid->Display(i+2);
			}
		}

		// 2009.10.19 ADD BY KSJ : 대출자번호 자동부여가 '사용자ID'일 경우 대출자번호 사용자ID로 자동입력 옵션 '사용안함'
		if( _T("대출자번호 자동부여") == sValue )
		{
			strUserNoAutoMode = pDM->GetCellData( _T("설정값"), i );
			// 2010.12.22 DELETE BY KSJ : 일반회원이라면 대출자번호 자동부여 관리값 사용 할 수 있도록 수정
			// 2010.07.28 ADD BY KSJ : 통합 회원 관리 시스템 사용 할 때 대출자번호 자동부여는 항상 '자동부여'
//DEL 			if(m_bIsUnityLoanService)
//DEL 			{
//DEL 				pDM->SetCellData( _T("설정값"), _T("Y"), i ); 
//DEL 				pGrid->SetCol(4);
//DEL 				pGrid->Display(i+1);			
//DEL 			}

			if( _T("U") == strUserNoAutoMode  )
			{
				pDM->SetCellData( _T("설정값"), _T("N"), i+1 ); 
				pGrid->SetCol(4);
				pGrid->Display(i+2);
			}
		}

		if( _T("대출자번호 사용자ID로 자동입력") == sValue )
		{
			strUserNoToUserID = pDM->GetCellData( _T("설정값"), i );
		}

		if( _T("주민등록번호 뒷자리 비밀번호로 자동입력") == sValue )
		{
			if( _T("U") != strUserNoAutoMode && _T("Y") != strUserNoToUserID )
			{
				pDM->SetCellData( _T("설정값"), _T("N"), i ); 
				pGrid->SetCol(4);
				pGrid->Display(i+1);
			}
		}
	}

	// 그리드에 변경안되는 부분 -표시
	for( i = 0; i < pDM->GetRowCount(); i++ )
	{
		sValue = pDM->GetCellData( _T("기능"), i );
		if( sValue == _T("가족재대출허용") || sValue == _T("부록자료대출") )
		{
			sValue = pDM->GetCellData( _T("설정값"), i );
			if( sValue == _T("Y") )
			{
				pGrid->SetCol(4);
				pGrid->SetRow(i+2);
				pGrid->SetCellFontStrikeThrough(FALSE);
			}
			else
			{
				pGrid->SetCol(4);
				pGrid->SetRow(i+2);
				pGrid->SetCellFontStrikeThrough(TRUE);
			}
		}

		// 2009.10.19 ADD BY KSJ : 대출자번호 자동부여가 '사용자ID'일 경우 대출자번호 사용자ID로 자동입력 옵션 수정불가
		if( _T("대출자번호 자동부여") == sValue )
		{
			// 2010.12.22 DELETE BY KSJ : 일반회원이라면 대출자번호 자동부여 관리값 사용 할 수 있도록 수정
			// 2010.07.28 ADD BY KSJ : 통합 회원 관리 시스템 사용 할 때 대출자번호 자동부여는 항상 '자동부여'
//DEL 			if(m_bIsUnityLoanService)
//DEL 			{
//DEL 				pGrid->SetCol(4);
//DEL 				pGrid->SetRow(i+1);
//DEL 				pGrid->SetCellFontStrikeThrough(TRUE);
//DEL 			}
//DEL 			else
//DEL 			{
				strUserNoAutoMode = pDM->GetCellData( _T("설정값"), i );
				if( _T("U") == strUserNoAutoMode )
				{
					pGrid->SetCol(4);
					pGrid->SetRow(i+2);
					pGrid->SetCellFontStrikeThrough(TRUE);
				}
				else
				{
					pGrid->SetCol(4);
					pGrid->SetRow(i+2);
					pGrid->SetCellFontStrikeThrough(FALSE);
				}
//DEL 			}
		}

		if( _T("대출자번호 사용자ID로 자동입력") == sValue )
		{
			strUserNoToUserID = pDM->GetCellData( _T("설정값"), i );
		}

		if( _T("주민등록번호 뒷자리 비밀번호로 자동입력") == sValue )
		{
			if( _T("U") != strUserNoAutoMode && _T("Y") != strUserNoToUserID )
			{
				pGrid->SetCol(4);
				pGrid->SetRow(i+1);
				pGrid->SetCellFontStrikeThrough(TRUE);
			}
			else
			{
				pGrid->SetCol(4);
				pGrid->SetRow(i+1);
				pGrid->SetCellFontStrikeThrough(FALSE);
			}
		}
	}
	// 셋커서원위치
	pGrid->SetCol(0);
	pGrid->SetRow(0);
}

// 02Dlg (반납연기&예약환경)
VOID CSettingGrid::SettingGrid02(CESL_DataMgr *pDM, CESL_Grid *pGrid)
{
	INT nRowCnt = pDM->GetRowCount();
	if(nRowCnt == 0) return ;

	// 반납연기사용에 따른 제어
	const int nCnt = 5;
	INT nNumber[nCnt] = { 2, 3, 4, 5, 6 };
	CString sValue;
	sValue = pDM->GetCellData( _T("설정값"), 1 - 1 );
	pGrid->SetCol(4);
	if( sValue == _T("Y") )
	{
		for( INT i = 0; i < nCnt; i++ )
		{
			pGrid->SetRow(nNumber[i]);
			pGrid->SetCellFontStrikeThrough(FALSE);
		}
	}
	else
	{
		for( INT i = 0; i < nCnt; i++ )
		{
			pGrid->SetRow(nNumber[i]);
			pGrid->SetCellFontStrikeThrough(TRUE);
		}
	}

	// 예약기능사용에 따른 제어
	const int nCnt2 = 2;
	INT nNumber2[nCnt] = { 7, 11 };
	sValue = pDM->GetCellData( _T("설정값"), 9 - 1 );
	if( sValue == _T("0") )
	{
		for( INT i = 0; i < nCnt2; i++ )
		{
			pGrid->SetRow(nNumber2[i]);
			pGrid->SetCellFontStrikeThrough(TRUE);
		}
	}
	else
	{
		for( INT i = 0; i < nCnt2; i++ )
		{
			pGrid->SetRow(nNumber2[i]);
			pGrid->SetCellFontStrikeThrough(FALSE);
		}
	}
	// 셋커서 원위치
	pGrid->SetCol(0);
	pGrid->SetRow(0);
}

VOID CSettingGrid::SettingGrid08(CESL_DataMgr *pDM, CESL_Grid *pGrid)
{
	INT nRowCnt = pDM->GetRowCount();
	if(nRowCnt == 0) return ;

	// 14/07/16 박대우 : 공공아이핀 적용으로 인해 강제 설정부분 제거
/*//REM--------------------------------------------------------------------------
	// 통합 회원 관리 시스템 사용에 따른 제어
	CString strValue;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		strValue = pDM->GetCellData( _T("기능"), i );
		if( _T("주민등록번호 표시여부") == strValue )
		{
			if(m_bIsUnityLoanService)
			{
				pDM->SetCellData( _T("설정값"), _T("Y"), i ); 
				pGrid->SetCol(4);
				pGrid->Display(i+1);
			}
		}
	}

	// 그리드에 변경안되는 부분 -표시
	for( i = 0; i < pDM->GetRowCount(); i++ )
	{
		strValue = pDM->GetCellData( _T("기능"), i );
		if( _T("주민등록번호 표시여부") == strValue )
		{
			if(m_bIsUnityLoanService)
			{
				pGrid->SetCol(4);
				pGrid->SetRow(i+1);
				pGrid->SetCellFontStrikeThrough(TRUE);
			}
		}
	}
*///END--------------------------------------------------------------------------
	// 셋커서 원위치
	pGrid->SetCol(0);
	pGrid->SetRow(0);
}

VOID CSettingGrid::SetIsUnityLoanService(BOOL bIsUnityLoanService) 
{
	m_bIsUnityLoanService = bIsUnityLoanService;
};