// UC_KOLISNetUseChecker.cpp : implementation file
//

#include "stdafx.h"
#include "UC_KOLISNetUseChecker.h"
#include "ESL_DataMgr.h"
#include "ReadCfgFile.h"
#include "KOLISNetMessageDlg.h"

#include "KolisNetService.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// UC_KOLISNetUseChecker

UC_KOLISNetUseChecker::UC_KOLISNetUseChecker(CESL_Mgr* pParent)
: CESL_Mgr(IDD, pParent)	
{
	m_strUserID = _T("");
}

UC_KOLISNetUseChecker::~UC_KOLISNetUseChecker()
{
}


BEGIN_MESSAGE_MAP(UC_KOLISNetUseChecker, CWnd)
	//{{AFX_MSG_MAP(UC_KOLISNetUseChecker)
		// NOTE - the ClassWizard will add and _tremove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// UC_KOLISNetUseChecker message handlers

BOOL UC_KOLISNetUseChecker::GetUseFlag(BOOL bMessageShow) 
{
    CESL_DataMgr pDM;
    CString sPath ;
	CReadCfgFile ReadCfg;	
	ReadCfg.ReadCfgFile(sPath);

	pDM.SetCONNECTION_INFO (ReadCfg.m_sConnection) ;
	
	if( m_strUserID.IsEmpty() )
		m_strUserID = GetUserID () ;
	
	//++2008.10.15 UPDATE BY PWR {{++
	// 통합시스템 적용
	CString sQuery_libcode ;
	sQuery_libcode.Format (_T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE") );   
     
    CString tmp_Lib_Code ;
	pDM.GetOneValueQuery (sQuery_libcode,tmp_Lib_Code) ; 

    if(tmp_Lib_Code.IsEmpty ())
	{
		if ( bMessageShow == TRUE )	AfxMessageBox (_T("도서관 부호가 설정 되어 있지 않습니다.")) ;
		return FALSE ;
	}	

	CString strFlag;
	if (!CKolisNetService::GetLibraryKolisNetUseFlag(this, tmp_Lib_Code, strFlag))
	{
		if (bMessageShow == TRUE)
		{
			AfxMessageBox(_T("공동목록 서버에 연결 할 수 없습니다. 관리자에게 문의하십시오."));
		}
		return FALSE;
	}

	if (strFlag.CompareNoCase(_T("N")) == 0 || strFlag.IsEmpty())
	{
		if (bMessageShow == TRUE)
		{
			CKOLISNetMessageDlg dlg;
			CString strMsg;
			strMsg.Format(_T("도서관부호 [%s]에 해당하는 도서관 정보가 없습니다.\n센터에 문의하십시오."), tmp_Lib_Code);
			dlg.SetMsg(strMsg);		
			dlg.DoModal();
		}
		return FALSE; 
	}

	return TRUE;
}
