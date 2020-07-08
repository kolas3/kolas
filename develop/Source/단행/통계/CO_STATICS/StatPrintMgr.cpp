// StatPrintMgr.cpp : implementation file
//

#include "stdafx.h"
#include "StatPrintMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatPrintMgr dialog


CStatPrintMgr::CStatPrintMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatPrintMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	INT ids = InitESL_Mgr( _T("STAT_PRINT") );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("CStatPrintMgr") );
}


VOID CStatPrintMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatPrintMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatPrintMgr, CDialog)
	//{{AFX_MSG_MAP(CStatPrintMgr)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatPrintMgr message handlers


CESL_DataMgr* CStatPrintMgr::GetPrintDM( CString sXAlias )
{
	EFS_BEGIN

	CString sDMAlias;

	sDMAlias.Format( _T("DM_%s") , sXAlias );

	

	CESL_DataMgr* pPrintDM;

	// 2007.06.20 update by pdw 
	// 대출횟수 통계 예외처리
	// 18.09.27 JMJ KOL.RED.2018.008
	if( _T("대출횟수통계") == sXAlias || _T("대출횟수통계(책두레포함)") == sXAlias)
	{
		pPrintDM = FindDM( _T("DM_BO_LOAN_CNT_STATICS") );
	}
	else if( _T("연체자통계") == sXAlias )
	{
		pPrintDM = FindDM( _T("DM_연체자료통계") );
	}
	else
	{
		pPrintDM = FindDM( sDMAlias );
	}

	return pPrintDM;

	EFS_END

	return NULL;
}


INT CStatPrintMgr::ShowDMViewer()
{
	EFS_BEGIN

	OnShowDMViewer(0);

	return 0;

	EFS_END

	return -1;
}