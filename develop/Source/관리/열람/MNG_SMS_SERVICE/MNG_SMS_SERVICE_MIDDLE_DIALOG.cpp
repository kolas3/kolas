// MNG_SMS_SERVICE_MIDDLE_DIALOG.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_SMS_SERVICE_MIDDLE_DIALOG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_SMS_SERVICE_MIDDLE_DIALOG dialog


CMNG_SMS_SERVICE_MIDDLE_DIALOG::CMNG_SMS_SERVICE_MIDDLE_DIALOG( INT nIDD , CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr( nIDD , pParent)
{
	//{{AFX_DATA_INIT(CMNG_SMS_SERVICE_MIDDLE_DIALOG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CMNG_SMS_SERVICE_MIDDLE_DIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_SMS_SERVICE_MIDDLE_DIALOG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_SMS_SERVICE_MIDDLE_DIALOG, CDialog)
	//{{AFX_MSG_MAP(CMNG_SMS_SERVICE_MIDDLE_DIALOG)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_SMS_SERVICE_MIDDLE_DIALOG message handlers


BOOL CMNG_SMS_SERVICE_MIDDLE_DIALOG::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CMNG_SMS_SERVICE_MIDDLE_DIALOG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CMNG_SMS_SERVICE_MIDDLE_DIALOG::ReView()
{
	return 0;
}

INT CMNG_SMS_SERVICE_MIDDLE_DIALOG::Save(CESL_DataMgr* pTypeEnvDM , CESL_DataMgr* pTotalEnvDM )
{
	return 0;
}

/// 타입별 구성정보 DM
CESL_DataMgr* CMNG_SMS_SERVICE_MIDDLE_DIALOG::GetTypeEnvDM()
{
	EFS_BEGIN


	return FindDM(_T("DM_MNG_SMS_TYPE_ENV"));

	EFS_END

	return NULL;
}

/// 전체 구성정보 DM
CESL_DataMgr* CMNG_SMS_SERVICE_MIDDLE_DIALOG::GetTotalEnvDM()
{
	EFS_BEGIN

	return FindDM(_T("DM_MNG_SMS_TOTAL_ENV"));

	EFS_END

	return NULL;
}

