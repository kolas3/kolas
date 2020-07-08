// Check_Dup Dlg.cpp : implementation file
//

#include  "stdafx.h"
#include  "Check_Dup.h"
#include   "..\..\include\공동목록\search_struct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

#define		PAGE				1         
#define     MAXCOUNTFORPAGE		100
//#define		KOLISNET_BO			_T("KOLISNET")
#define     KOLISNET_BO         _T("dan-gm,dan-limit,dan-etc,dan-rb,dan-ca,dan-dmdp,dan-ha")
#define		MAX_FETCH_COUNT		2000	


// Error Code
#define	   GENERAL_ERROR        -1
#define	   COMMINI_ERROR         2
#define	   ORACLE_ERROR          3


CCheck_Dup::CCheck_Dup (CESL_Mgr *pParent /*=NULL*/)
{	
    m_BibReckey = _T("");
	m_pMarcMgr = NULL ;
}

CCheck_Dup ::~CCheck_Dup ()
{

}
